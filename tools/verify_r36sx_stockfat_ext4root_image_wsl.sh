#!/usr/bin/env bash
set -euo pipefail

IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_mipsel_stockfat_ext4root.img}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

LOOP=$(losetup --find --show -P "$IMG")
BOOT_MNT=/mnt/verify-stockfat-boot
ROOT_MNT=/mnt/verify-stockfat-root

cleanup() {
    set +e
    umount "$ROOT_MNT" 2>/dev/null || true
    umount "$BOOT_MNT" 2>/dev/null || true
    losetup -d "$LOOP" 2>/dev/null || true
}
trap cleanup EXIT

mkdir -p "$BOOT_MNT" "$ROOT_MNT"
mount -o ro "${LOOP}p1" "$BOOT_MNT"
mount -o ro "${LOOP}p2" "$ROOT_MNT"

echo "image=$IMG"
echo "loop=$LOOP"
lsblk -o NAME,SIZE,TYPE,FSTYPE,LABEL "$LOOP"

echo
echo "top_level_fat:"
find "$BOOT_MNT" -maxdepth 1 -mindepth 1 -printf '%f\n' | sort

echo
echo "fat_sizes:"
du -sh \
    "$BOOT_MNT/cubegm" \
    "$BOOT_MNT/MIPS_NATIVE" \
    "$BOOT_MNT/rootfs" \
    "$BOOT_MNT/Movie" \
    "$BOOT_MNT/Ebook" \
    "$BOOT_MNT/Music"

echo
echo "dtb_bootargs:"
dtc -I dtb -O dts "$BOOT_MNT/cubegm/dtb.bin" 2>/dev/null | grep 'bootargs ='

echo
echo "debian:"
cat "$ROOT_MNT/etc/debian_version"
grep -E '^(PRETTY_NAME|VERSION_CODENAME)=' "$ROOT_MNT/etc/os-release"
