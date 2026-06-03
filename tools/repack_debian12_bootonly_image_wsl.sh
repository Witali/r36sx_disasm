#!/usr/bin/env bash
set -euo pipefail

SRC_IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_mipsel_essential.img}
OUT_IMG=${2:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_mipsel_bootonly.img}
WORKSPACE=${WORKSPACE:-/mnt/c/Work/r36sx_disasm}
NEW_SIZE_MIB=${NEW_SIZE_MIB:-4096}
BOOT_SIZE_MIB=${BOOT_SIZE_MIB:-256}
BOOTARGS=${BOOTARGS:-"root=/dev/mmcblk0p2 rootfstype=ext4 rootwait rw init=/sbin/init console=tty1 earlycon= no_console_suspend noirqdebug"}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ ! -e "$SRC_IMG" ]]; then
    echo "Source image not found: $SRC_IMG" >&2
    exit 1
fi

if [[ -e "$OUT_IMG" ]]; then
    echo "Output image already exists: $OUT_IMG" >&2
    exit 1
fi

for cmd in dtc losetup parted partprobe mkfs.vfat mkfs.ext4 rsync mount umount truncate; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "Missing required command: $cmd" >&2
        exit 1
    }
done

for file in dtb.dts avp.uImage vmlinux.uImage xgame-logo.bmp; do
    if [[ ! -e "$WORKSPACE/disk_image/cubegm/$file" ]]; then
        echo "Missing boot artifact: $WORKSPACE/disk_image/cubegm/$file" >&2
        exit 1
    fi
done

SRC_LOOP=
OUT_LOOP=
SRC_ROOT=/mnt/r36sx-src-root
OUT_BOOT=/mnt/r36sx-out-boot
OUT_ROOT=/mnt/r36sx-out-root
DTB_WORK=

cleanup() {
    set +e
    umount "$SRC_ROOT" 2>/dev/null || true
    umount "$OUT_ROOT" 2>/dev/null || true
    umount "$OUT_BOOT" 2>/dev/null || true
    [[ -n "$SRC_LOOP" ]] && losetup -d "$SRC_LOOP" 2>/dev/null || true
    [[ -n "$OUT_LOOP" ]] && losetup -d "$OUT_LOOP" 2>/dev/null || true
    [[ -n "$DTB_WORK" ]] && rm -rf "$DTB_WORK" 2>/dev/null || true
}
trap cleanup EXIT

mkdir -p "$(dirname "$OUT_IMG")"
truncate -s "${NEW_SIZE_MIB}M" "$OUT_IMG"

OUT_LOOP=$(losetup --find --show "$OUT_IMG")
parted -s "$OUT_LOOP" mklabel msdos
parted -s "$OUT_LOOP" mkpart primary fat32 4MiB "${BOOT_SIZE_MIB}MiB"
parted -s "$OUT_LOOP" set 1 boot on
parted -s "$OUT_LOOP" mkpart primary ext4 "${BOOT_SIZE_MIB}MiB" 100%
partprobe "$OUT_LOOP" || true
sleep 2

mkfs.vfat -F 32 -n R36SXBOOT "${OUT_LOOP}p1"
mkfs.ext4 -F -L R36SXROOT "${OUT_LOOP}p2"

mkdir -p "$SRC_ROOT" "$OUT_BOOT" "$OUT_ROOT"
mount -t vfat -o utf8=1,shortname=mixed "${OUT_LOOP}p1" "$OUT_BOOT"
mount "${OUT_LOOP}p2" "$OUT_ROOT"

mkdir -p "$OUT_BOOT/cubegm"
DTB_WORK=$(mktemp -d)
cp "$WORKSPACE/disk_image/cubegm/dtb.dts" "$DTB_WORK/dtb.ext4-root.dts"
perl -0pi -e "s#bootargs = \"[^\"]*\"#bootargs = \"$BOOTARGS\"#" "$DTB_WORK/dtb.ext4-root.dts"
dtc -I dts -O dtb -o "$OUT_BOOT/cubegm/dtb.bin" "$DTB_WORK/dtb.ext4-root.dts"
rsync -rt "$WORKSPACE/disk_image/cubegm/avp.uImage" "$OUT_BOOT/cubegm/"
rsync -rt "$WORKSPACE/disk_image/cubegm/vmlinux.uImage" "$OUT_BOOT/cubegm/"
rsync -rt "$WORKSPACE/disk_image/cubegm/xgame-logo.bmp" "$OUT_BOOT/cubegm/"

SRC_LOOP=$(losetup --find --show -P "$SRC_IMG")
mount -o ro "${SRC_LOOP}p2" "$SRC_ROOT"
rsync -aH --numeric-ids --info=progress2 "$SRC_ROOT/" "$OUT_ROOT/"

sync
umount "$SRC_ROOT"
umount "$OUT_ROOT"
umount "$OUT_BOOT"
losetup -d "$SRC_LOOP"
losetup -d "$OUT_LOOP"
SRC_LOOP=
OUT_LOOP=

echo "Created boot-only Debian image:"
ls -lh "$OUT_IMG"
echo
echo "FAT boot files:"
mdir -i "$OUT_IMG@@$((4 * 1024 * 1024))" ::/cubegm 2>/dev/null || true
