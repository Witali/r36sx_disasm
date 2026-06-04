#!/usr/bin/env bash
set -euo pipefail

OUT_IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_stage3_shell.img}
WORKSPACE=${WORKSPACE:-/mnt/c/Work/r36sx_disasm}
SIZE_MIB=${SIZE_MIB:-4096}
BOOT_SIZE_MIB=${BOOT_SIZE_MIB:-2048}
PART_START_MIB=${PART_START_MIB:-4}
DEBIAN_SUITE=${DEBIAN_SUITE:-bookworm}
DEBIAN_MIRROR=${DEBIAN_MIRROR:-http://deb.debian.org/debian}
COPY_MEDIA=${COPY_MEDIA:-1}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ -e "$OUT_IMG" ]]; then
    echo "Output image already exists: $OUT_IMG" >&2
    exit 1
fi

for cmd in losetup parted partprobe mkfs.vfat mkfs.ext4 fsck.vfat e2fsck \
    rsync mount umount truncate sha256sum find sort mktemp cp rm debootstrap \
    chroot qemu-mipsel-static; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "Missing required command: $cmd" >&2
        exit 1
    }
done

required_sources=(
    "$WORKSPACE/disk_image/cubegm/dtb.bin"
    "$WORKSPACE/disk_image/cubegm/avp.uImage"
    "$WORKSPACE/disk_image/cubegm/vmlinux.uImage"
    "$WORKSPACE/disk_image/cubegm/xgame-logo.bmp"
    "$WORKSPACE/disk_image/cubegm"
    "$WORKSPACE/disk_image/rootfs"
    "$WORKSPACE/disk_image/MIPS_NATIVE/shell/shell"
    "$WORKSPACE/disk_image/MIPS_NATIVE/common/fonts"
    "$WORKSPACE/disk_image/Chkdsk.bat"
    "$WORKSPACE/disk_image/chkdsk.exe"
)

for source_path in "${required_sources[@]}"; do
    if [[ ! -e "$source_path" ]]; then
        echo "Missing source path: $source_path" >&2
        exit 1
    fi
done

if (( SIZE_MIB < 3072 )); then
    echo "SIZE_MIB is too small; use at least 3072" >&2
    exit 1
fi
if (( BOOT_SIZE_MIB < 1536 )); then
    echo "BOOT_SIZE_MIB is too small for the stock runtime set; use at least 1536" >&2
    exit 1
fi
if (( BOOT_SIZE_MIB >= SIZE_MIB - 512 )); then
    echo "BOOT_SIZE_MIB leaves too little room for Debian ext4" >&2
    exit 1
fi

OUT_LOOP=
BOOT_MNT=/mnt/r36sx-debian-stage3-boot
ROOT_MNT=/mnt/r36sx-debian-stage3-root
WORK_IMG=
COPY_DONE=0

is_mounted() {
    findmnt -rn "$1" >/dev/null 2>&1
}

cleanup() {
    set +e
    umount "$ROOT_MNT/dev/pts" 2>/dev/null || true
    umount "$ROOT_MNT/dev" 2>/dev/null || true
    umount "$ROOT_MNT/sys" 2>/dev/null || true
    umount "$ROOT_MNT/proc" 2>/dev/null || true
    umount "$ROOT_MNT" 2>/dev/null || true
    umount "$BOOT_MNT" 2>/dev/null || true
    [[ -n "$OUT_LOOP" ]] && losetup -d "$OUT_LOOP" 2>/dev/null || true
    [[ -n "$WORK_IMG" && -e "$WORK_IMG" ]] && rm -f "$WORK_IMG" 2>/dev/null || true
    [[ "$COPY_DONE" == "0" && -e "$OUT_IMG" ]] && rm -f "$OUT_IMG" 2>/dev/null || true
}
trap cleanup EXIT

mkdir -p "$(dirname "$OUT_IMG")"
WORK_IMG=$(mktemp /tmp/r36sx-debian-stage3.XXXXXX.img)
truncate -s "${SIZE_MIB}M" "$WORK_IMG"

OUT_LOOP=$(losetup --find --show "$WORK_IMG")
parted -s "$OUT_LOOP" mklabel msdos
parted -s "$OUT_LOOP" mkpart primary fat32 "${PART_START_MIB}MiB" "${BOOT_SIZE_MIB}MiB"
parted -s "$OUT_LOOP" set 1 boot on
parted -s "$OUT_LOOP" mkpart primary ext4 "${BOOT_SIZE_MIB}MiB" 100%
partprobe "$OUT_LOOP" || true
sleep 2

mkfs.vfat -F 32 -n R36SXBOOT "${OUT_LOOP}p1"
mkfs.ext4 -F -L R36SXROOT "${OUT_LOOP}p2"

mkdir -p "$BOOT_MNT" "$ROOT_MNT"
mount -t vfat -o utf8=1,shortname=mixed "${OUT_LOOP}p1" "$BOOT_MNT"
mount "${OUT_LOOP}p2" "$ROOT_MNT"

echo "Copying Stage-3-safe FAT runtime..."
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/cubegm" "$BOOT_MNT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/rootfs" "$BOOT_MNT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/MIPS_NATIVE" "$BOOT_MNT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/Chkdsk.bat" "$BOOT_MNT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/chkdsk.exe" "$BOOT_MNT/"

if [[ "$COPY_MEDIA" == "1" ]]; then
    for item in Movie Ebook Music; do
        if [[ -e "$WORKSPACE/disk_image/$item" ]]; then
            rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/$item" "$BOOT_MNT/"
        fi
    done
fi

mkdir -p "$BOOT_MNT/MIPS_NATIVE/debian"
cat > "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian_shell.sh" <<'EOF'
#!/bin/sh
set -u

ROOT=/mnt/debian

log() {
    echo "[debian] $*"
}

is_mounted() {
    grep -qs " $1 " /proc/mounts
}

show_diagnostics() {
    log "diagnostics: /proc/partitions"
    cat /proc/partitions 2>/dev/null || true
    log "diagnostics: /dev/mmc* /dev/sd*"
    ls -l /dev/mmc* /dev/sd* 2>/dev/null || true
    log "diagnostics: mounted filesystems"
    cat /proc/mounts 2>/dev/null || true
    log "diagnostics: recent mmc/ext/partition dmesg"
    dmesg 2>/dev/null | grep -i 'mmc\|partition\|ext2\|ext3\|ext4' | tail -40 || true
}

create_block_nodes_from_proc() {
    [ -r /proc/partitions ] || return 0
    while read major minor blocks name rest; do
        case "$major:$minor:$name" in
            major:minor:name)
                continue
                ;;
        esac
        case "$name" in
            mmcblk*|sd[a-z]|sd[a-z][0-9])
                if [ -n "$major" ] && [ -n "$minor" ] && [ -n "$name" ] &&
                   [ ! -e "/dev/$name" ]; then
                    mknod "/dev/$name" b "$major" "$minor" 2>/dev/null || true
                fi
                ;;
        esac
    done < /proc/partitions
}

rescan_devices() {
    mkdir -p /dev /dev/pts /sys /proc
    grep -qs ' /sys ' /proc/mounts || mount -t sysfs sysfs /sys 2>/dev/null || true
    grep -qs ' /dev/pts ' /proc/mounts || mount -t devpts devpts /dev/pts 2>/dev/null || true
    echo /sbin/mdev > /proc/sys/kernel/hotplug 2>/dev/null || true
    /sbin/mdev -s 2>/dev/null || /bin/mdev -s 2>/dev/null || true
    create_block_nodes_from_proc
    modprobe ext4 2>/dev/null || true
    modprobe ext3 2>/dev/null || true
    modprobe ext2 2>/dev/null || true
}

find_debian_device() {
    for dev in \
        /dev/mmcblk0p2 \
        /dev/mmcblk1p2 \
        /dev/mmcblk2p2 \
        /dev/sda2 \
        /dev/sdb2 \
        /dev/sdc2
    do
        if [ -b "$dev" ]; then
            echo "$dev"
            return 0
        fi
    done
    return 1
}

mount_debian_root() {
    dev=$1
    mkdir -p "$ROOT"
    if is_mounted "$ROOT"; then
        return 0
    fi
    for fstype in ext4 ext3 ext2 auto; do
        log "mounting $dev as $fstype on $ROOT"
        if mount -t "$fstype" "$dev" "$ROOT" 2>/tmp/debian-mount.err; then
            return 0
        fi
        cat /tmp/debian-mount.err 2>/dev/null || true
    done
    return 1
}

fallback_stock_shell() {
    log "Debian root is unavailable; staying in stock shell"
    log "try: cat /proc/partitions; ls -l /dev/mmc*; dmesg | grep -i mmc"
    exec /bin/sh -i
}

log "preparing Debian root partition"
DEBIAN_DEV=
tries=0
while [ "$tries" -lt 12 ]; do
    rescan_devices
    DEBIAN_DEV=$(find_debian_device 2>/dev/null || true)
    if [ -n "$DEBIAN_DEV" ]; then
        break
    fi
    log "waiting for second partition... ($tries)"
    sleep 1
    tries=$((tries + 1))
done

if [ -z "$DEBIAN_DEV" ]; then
    log "could not find a second partition device"
    show_diagnostics
    fallback_stock_shell
fi

log "using $DEBIAN_DEV"
mkdir -p "$ROOT"
if ! mount_debian_root "$DEBIAN_DEV"; then
    log "could not mount $DEBIAN_DEV"
    show_diagnostics
    fallback_stock_shell
fi

mkdir -p "$ROOT/proc" "$ROOT/sys" "$ROOT/dev" "$ROOT/dev/pts" "$ROOT/mnt/sdcard"
is_mounted "$ROOT/proc" || mount -t proc proc "$ROOT/proc"
is_mounted "$ROOT/sys" || mount -t sysfs sysfs "$ROOT/sys"
is_mounted "$ROOT/dev" || mount --bind /dev "$ROOT/dev"
is_mounted "$ROOT/dev/pts" || mount -t devpts devpts "$ROOT/dev/pts"
is_mounted "$ROOT/mnt/sdcard" || mount --bind /mnt/sdcard "$ROOT/mnt/sdcard"

export HOME=/root
export TERM=linux
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

if [ -x "$ROOT/bin/bash" ]; then
    log "entering Debian bash"
    exec chroot "$ROOT" /bin/bash -i
fi
if [ -x "$ROOT/bin/busybox" ]; then
    log "entering Debian BusyBox sh"
    exec chroot "$ROOT" /bin/busybox sh
fi
log "entering Debian /bin/sh"
exec chroot "$ROOT" /bin/sh -i
EOF
chmod 0755 "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian_shell.sh"

cat > "$BOOT_MNT/MIPS_NATIVE/debian/start_debian_shell.sh" <<'EOF'
#!/bin/sh
set -eu

export LD_LIBRARY_PATH=/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib
export R36SX_SHELL_COMMAND='sh /mnt/sdcard/MIPS_NATIVE/debian/enter_debian_shell.sh'
exec /mnt/sdcard/MIPS_NATIVE/shell/shell
EOF
chmod 0755 "$BOOT_MNT/MIPS_NATIVE/debian/start_debian_shell.sh"

cat > "$BOOT_MNT/MIPS_NATIVE/debian/README.md" <<'EOF'
# Debian 12 mipsel framebuffer shell

This image keeps the confirmed Stage 3 FAT boot path on `/dev/mmcblk0p1` and
stores a minimal Debian 12 mipsel rootfs on `/dev/mmcblk0p2`.

The graphical terminal is still the native `MIPS_NATIVE/shell/shell` frontend
using `/mnt/sdcard/cubegm/driver.so` for the framebuffer and buttons. It starts:

```sh
sh /mnt/sdcard/MIPS_NATIVE/debian/enter_debian_shell.sh
```

That helper mounts `/dev/mmcblk0p2`, binds `/dev`, `/proc`, `/sys`, and
`/mnt/sdcard`, then enters Debian with `/bin/bash -i`; if bash is unavailable it
falls back to BusyBox `sh`.
EOF

cat > "$BOOT_MNT/cubegm/icube.sh" <<'EOF'
#!/bin/sh

export LD_LIBRARY_PATH=/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib
/mnt/sdcard/MIPS_NATIVE/debian/start_debian_shell.sh &
init -q
EOF
chmod 0755 "$BOOT_MNT/cubegm/icube.sh"

cat > "$BOOT_MNT/cubegm/icube_start.sh" <<'EOF'
#!/bin/sh

killall hcprojector 2>/dev/null || true
export LD_LIBRARY_PATH=/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib
/mnt/sdcard/MIPS_NATIVE/debian/start_debian_shell.sh &
EOF
chmod 0755 "$BOOT_MNT/cubegm/icube_start.sh"

echo "Creating Debian ${DEBIAN_SUITE} mipsel minbase rootfs..."
debootstrap --arch=mipsel --foreign --variant=minbase "$DEBIAN_SUITE" "$ROOT_MNT" "$DEBIAN_MIRROR"
cp "$(command -v qemu-mipsel-static)" "$ROOT_MNT/usr/bin/"
chroot "$ROOT_MNT" /usr/bin/qemu-mipsel-static /bin/sh /debootstrap/debootstrap --second-stage

cat > "$ROOT_MNT/etc/apt/sources.list" <<EOF
deb $DEBIAN_MIRROR $DEBIAN_SUITE main
deb $DEBIAN_MIRROR ${DEBIAN_SUITE}-updates main
deb http://security.debian.org/debian-security ${DEBIAN_SUITE}-security main
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

cp /etc/resolv.conf "$ROOT_MNT/etc/resolv.conf" 2>/dev/null || true
mount -t proc proc "$ROOT_MNT/proc"
mount -t sysfs sysfs "$ROOT_MNT/sys"
mount --bind /dev "$ROOT_MNT/dev"
mount -t devpts devpts "$ROOT_MNT/dev/pts"

chroot "$ROOT_MNT" /usr/bin/qemu-mipsel-static /bin/sh -lc '
set -e
export DEBIAN_FRONTEND=noninteractive
apt-get update
apt-get install --no-install-recommends -y \
  bash \
  busybox-static \
  ca-certificates \
  e2fsprogs \
  iproute2 \
  kmod \
  mount \
  procps
apt-get clean
rm -rf /var/lib/apt/lists/*
'

cat > "$ROOT_MNT/root/README-r36sx.txt" <<'EOF'
This is a minimal Debian 12 mipsel rootfs for the R36SX Stage-3-safe SD image.

The display terminal is not Debian framebuffer console. It is the native
/mnt/sdcard/MIPS_NATIVE/shell/shell program from the stock FAT partition, using
/mnt/sdcard/cubegm/driver.so, then chrooting into this ext4 rootfs.

Useful checks:
  cat /etc/os-release
  uname -a
  mount
  ls /mnt/sdcard
EOF

sync
umount "$ROOT_MNT/dev/pts"
umount "$ROOT_MNT/dev"
umount "$ROOT_MNT/sys"
umount "$ROOT_MNT/proc"

echo "Stage 3 boot-resource checks:"
for file in dtb.bin avp.uImage vmlinux.uImage xgame-logo.bmp; do
    test -f "$BOOT_MNT/cubegm/$file"
    src_hash=$(sha256sum "$WORKSPACE/disk_image/cubegm/$file" | awk '{print $1}')
    out_hash=$(sha256sum "$BOOT_MNT/cubegm/$file" | awk '{print $1}')
    if [[ "$src_hash" != "$out_hash" ]]; then
        echo "Hash mismatch for cubegm/$file" >&2
        exit 1
    fi
    printf '  ok  cubegm/%s  %s\n' "$file" "$out_hash"
done

echo
echo "Debian rootfs:"
cat "$ROOT_MNT/etc/debian_version"
grep -E '^(PRETTY_NAME|VERSION_CODENAME)=' "$ROOT_MNT/etc/os-release"
test -x "$ROOT_MNT/bin/bash"
test -x "$ROOT_MNT/bin/busybox"

umount "$ROOT_MNT"
umount "$BOOT_MNT"
fsck.vfat -vn "${OUT_LOOP}p1"
e2fsck -fn "${OUT_LOOP}p2"
losetup -d "$OUT_LOOP"
OUT_LOOP=

cp --sparse=always "$WORK_IMG" "$OUT_IMG"
COPY_DONE=1

echo
echo "Created Debian Stage-3-shell image:"
ls -lh "$OUT_IMG"
