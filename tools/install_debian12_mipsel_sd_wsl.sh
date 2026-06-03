#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
Usage:
  I_UNDERSTAND_ERASE_DISK3=yes ./tools/install_debian12_mipsel_sd_wsl.sh /dev/sdX

This script ERASES the target disk and prepares it for the HC16xx/R36SX device:
  - p1: FAT32 stock boot/userspace files copied from disk_image/
  - p2: ext4 Debian 12 Bookworm mipsel rootfs

Run it from WSL as root after attaching the confirmed SD card with:
  wsl --mount \\.\PHYSICALDRIVE3 --bare

Safety checks:
  - target must be a block disk
  - target must be about 32 GB
  - target must not be the WSL root disk mounted at /
  - confirmation env var must be set
USAGE
}

if [[ ${1:-} == "-h" || ${1:-} == "--help" || $# -ne 1 ]]; then
    usage
    exit 1
fi

if [[ ${I_UNDERSTAND_ERASE_DISK3:-} != "yes" ]]; then
    echo "Refusing to run without I_UNDERSTAND_ERASE_DISK3=yes" >&2
    usage
    exit 1
fi

if [[ $(id -u) -ne 0 ]]; then
    echo "Run this script as root in WSL, for example: wsl -d Ubuntu -u root -- bash ..." >&2
    exit 1
fi

TARGET_DISK=$1
WORKSPACE=${WORKSPACE:-/mnt/c/Work/r36sx_disasm}
DISK_IMAGE_SRC=${DISK_IMAGE_SRC:-$WORKSPACE/disk_image}
BOOT_MNT=${BOOT_MNT:-/mnt/r36sx-boot}
ROOT_MNT=${ROOT_MNT:-/mnt/r36sx-root}
BOOT_SIZE_MIB=${BOOT_SIZE_MIB:-18432}
DEBIAN_MIRROR=${DEBIAN_MIRROR:-http://deb.debian.org/debian}
BOOT_COPY_MODE=${BOOT_COPY_MODE:-full}

need_cmd() {
    command -v "$1" >/dev/null 2>&1 || {
        echo "Missing required command: $1" >&2
        exit 1
    }
}

for cmd in blockdev lsblk parted partprobe mkfs.vfat mkfs.ext4 rsync debootstrap chroot mount umount; do
    need_cmd "$cmd"
done
need_cmd qemu-mipsel-static

if [[ ! -b "$TARGET_DISK" ]]; then
    echo "Target is not a block device: $TARGET_DISK" >&2
    exit 1
fi

disk_type=$(lsblk -dn -o TYPE "$TARGET_DISK")
if [[ "$disk_type" != "disk" && "$disk_type" != "loop" ]]; then
    echo "Target is not a whole disk or loop image: $TARGET_DISK has TYPE=$disk_type" >&2
    exit 1
fi

size_bytes=$(blockdev --getsize64 "$TARGET_DISK")
if [[ "$disk_type" == "disk" ]]; then
    if (( size_bytes < 30000000000 || size_bytes > 34000000000 )); then
        echo "Target size $size_bytes bytes is not in the expected 32 GB SD range" >&2
        lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,MODEL "$TARGET_DISK" >&2
        exit 1
    fi
else
    if (( size_bytes < 4000000000 )); then
        echo "Loop image is too small for FAT boot files plus Debian rootfs: $size_bytes bytes" >&2
        lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,MODEL "$TARGET_DISK" >&2
        exit 1
    fi
fi

root_source=$(findmnt -n -o SOURCE / || true)
if [[ -n "$root_source" && "$root_source" == "$TARGET_DISK"* ]]; then
    echo "Refusing to partition the disk backing WSL root: $root_source" >&2
    exit 1
fi

if [[ ! -d "$DISK_IMAGE_SRC/cubegm" ]]; then
    echo "disk_image source not found or incomplete: $DISK_IMAGE_SRC" >&2
    exit 1
fi

part_path() {
    local disk=$1
    local num=$2
    case "$disk" in
        *[0-9]) printf '%sp%s' "$disk" "$num" ;;
        *) printf '%s%s' "$disk" "$num" ;;
    esac
}

BOOT_PART=$(part_path "$TARGET_DISK" 1)
ROOT_PART=$(part_path "$TARGET_DISK" 2)

echo "About to ERASE and repartition:"
lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,MODEL "$TARGET_DISK"
echo "FAT32 partition size: ${BOOT_SIZE_MIB} MiB"
echo "Debian mirror: $DEBIAN_MIRROR"

for mnt in "$BOOT_MNT" "$ROOT_MNT"; do
    if findmnt -rn "$mnt" >/dev/null 2>&1; then
        umount "$mnt"
    fi
done

swapoff "$TARGET_DISK"* 2>/dev/null || true

parted -s "$TARGET_DISK" mklabel msdos
parted -s "$TARGET_DISK" mkpart primary fat32 4MiB "${BOOT_SIZE_MIB}MiB"
parted -s "$TARGET_DISK" set 1 boot on
parted -s "$TARGET_DISK" mkpart primary ext4 "${BOOT_SIZE_MIB}MiB" 100%
partprobe "$TARGET_DISK" || true
sleep 3

if [[ ! -b "$BOOT_PART" || ! -b "$ROOT_PART" ]]; then
    echo "Partition nodes did not appear: $BOOT_PART / $ROOT_PART" >&2
    lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,MODEL "$TARGET_DISK" >&2
    exit 1
fi

mkfs.vfat -F 32 -n R36SXBOOT "$BOOT_PART"
mkfs.ext4 -F -L R36SXROOT "$ROOT_PART"

mkdir -p "$BOOT_MNT" "$ROOT_MNT"
mount -t vfat -o utf8=1,shortname=mixed "$BOOT_PART" "$BOOT_MNT"
mount "$ROOT_PART" "$ROOT_MNT"

echo "Copying stock files to FAT boot partition ($BOOT_COPY_MODE mode)..."
if [[ "$BOOT_COPY_MODE" == "essential" ]]; then
    for item in cubegm MIPS_NATIVE rootfs Chkdsk.bat chkdsk.exe; do
        if [[ -e "$DISK_IMAGE_SRC/$item" ]]; then
            rsync -rt --inplace --info=progress2 --modify-window=2 "$DISK_IMAGE_SRC/$item" "$BOOT_MNT/"
        fi
    done
elif [[ "$BOOT_COPY_MODE" == "full" ]]; then
    rsync -rt --inplace --info=progress2 --modify-window=2 "$DISK_IMAGE_SRC/" "$BOOT_MNT/"
else
    echo "Unknown BOOT_COPY_MODE: $BOOT_COPY_MODE" >&2
    exit 1
fi

echo "Creating Debian 12 Bookworm mipsel rootfs..."
debootstrap --arch=mipsel --foreign --variant=minbase bookworm "$ROOT_MNT" "$DEBIAN_MIRROR"
cp "$(command -v qemu-mipsel-static)" "$ROOT_MNT/usr/bin/"

chroot "$ROOT_MNT" /usr/bin/qemu-mipsel-static /bin/sh /debootstrap/debootstrap --second-stage

cat > "$ROOT_MNT/etc/apt/sources.list" <<'EOF'
deb http://deb.debian.org/debian bookworm main
deb http://deb.debian.org/debian bookworm-updates main
deb http://security.debian.org/debian-security bookworm-security main
EOF

cat > "$ROOT_MNT/etc/fstab" <<'EOF'
proc            /proc       proc    defaults                     0 0
sysfs           /sys        sysfs   defaults                     0 0
devtmpfs        /dev        devtmpfs mode=0755                   0 0
devpts          /dev/pts    devpts  gid=5,mode=620,ptmxmode=0666 0 0
tmpfs           /run        tmpfs   mode=0755,nosuid,nodev       0 0
/dev/mmcblk0p2  /           ext4    defaults,noatime             0 1
/dev/mmcblk0p1  /mnt/sdcard vfat    defaults,utf8               0 0
EOF

echo r36sx-debian > "$ROOT_MNT/etc/hostname"
mkdir -p "$ROOT_MNT/mnt/sdcard" "$ROOT_MNT/dev/pts" "$ROOT_MNT/proc" "$ROOT_MNT/sys" "$ROOT_MNT/run"
ln -sf /dev/pts/ptmx "$ROOT_MNT/dev/ptmx"

mount -t proc proc "$ROOT_MNT/proc"
mount -t sysfs sysfs "$ROOT_MNT/sys"
mount --bind /dev "$ROOT_MNT/dev"
mount -t devpts devpts "$ROOT_MNT/dev/pts"

cleanup_chroot_mounts() {
    umount "$ROOT_MNT/dev/pts" 2>/dev/null || true
    umount "$ROOT_MNT/dev" 2>/dev/null || true
    umount "$ROOT_MNT/sys" 2>/dev/null || true
    umount "$ROOT_MNT/proc" 2>/dev/null || true
}
trap cleanup_chroot_mounts EXIT

chroot "$ROOT_MNT" /usr/bin/qemu-mipsel-static /bin/sh -lc '
set -e
export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install --no-install-recommends -y \
  sysvinit-core \
  busybox-static \
  ifupdown \
  netbase \
  iproute2 \
  procps \
  kmod \
  mount \
  e2fsprogs \
  ca-certificates \
  nano
apt-get clean
rm -rf /var/lib/apt/lists/*
'

mkdir -p "$BOOT_MNT/MIPS_NATIVE/debian"
cat > "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian.sh" <<'EOF'
#!/bin/sh
set -eu

ROOT=/mnt/debian

is_mounted() {
    grep -qs " $1 " /proc/mounts
}

mkdir -p "$ROOT"
is_mounted "$ROOT" || mount -t ext4 /dev/mmcblk0p2 "$ROOT"

mkdir -p "$ROOT/proc" "$ROOT/sys" "$ROOT/dev" "$ROOT/dev/pts" "$ROOT/mnt/sdcard"
is_mounted "$ROOT/proc" || mount -t proc proc "$ROOT/proc"
is_mounted "$ROOT/sys" || mount -t sysfs sysfs "$ROOT/sys"
is_mounted "$ROOT/dev" || mount --bind /dev "$ROOT/dev"
is_mounted "$ROOT/dev/pts" || mount -t devpts devpts "$ROOT/dev/pts"

export LD_LIBRARY_PATH=/mnt/sdcard/cubegm:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib
exec chroot "$ROOT" /bin/sh
EOF

cat > "$BOOT_MNT/MIPS_NATIVE/debian/README.md" <<'EOF'
# Debian 12 mipsel

The ext4 Debian rootfs is on `/dev/mmcblk0p2`.

From the stock firmware shell, enter it with:

```sh
sh /mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh
```

The stock FAT partition remains `/dev/mmcblk0p1` and should still contain
`cubegm/dtb.bin`, `cubegm/avp.uImage`, `cubegm/vmlinux.uImage`, and
`cubegm/xgame-logo.bmp`.
EOF

sync
cleanup_chroot_mounts
trap - EXIT

umount "$ROOT_MNT"
umount "$BOOT_MNT"

echo "Done. Final partition table:"
lsblk -o NAME,SIZE,TYPE,FSTYPE,LABEL,MOUNTPOINTS,MODEL "$TARGET_DISK"
