#!/usr/bin/env bash
set -euo pipefail

IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_mipsel_bootonly.img}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ ! -e "$IMG" ]]; then
    echo "Image not found: $IMG" >&2
    exit 1
fi

LOOP=$(losetup --find --show -P "$IMG")
BOOT_MNT=/mnt/verify-bootonly-boot
ROOT_MNT=/mnt/verify-bootonly-root

cleanup() {
    set +e
    umount "$BOOT_MNT" 2>/dev/null || true
    umount "$ROOT_MNT" 2>/dev/null || true
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
echo "FAT files:"
find "$BOOT_MNT" -maxdepth 3 -type f -printf '%P\t%s\n' | sort

echo
echo "DTB bootargs:"
dtc -I dtb -O dts "$BOOT_MNT/cubegm/dtb.bin" 2>/dev/null | grep 'bootargs ='

echo
echo "Debian rootfs:"
cat "$ROOT_MNT/etc/debian_version"
grep -E '^(PRETTY_NAME|VERSION_CODENAME)=' "$ROOT_MNT/etc/os-release"
