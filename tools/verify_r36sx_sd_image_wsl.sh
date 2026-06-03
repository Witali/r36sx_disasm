#!/usr/bin/env bash
set -euo pipefail

IMG=${1:?image path required}

if [[ ! -f "$IMG" ]]; then
    echo "Image not found: $IMG" >&2
    exit 1
fi

LOOP=$(losetup --find --show -P "$IMG")
cleanup() {
    set +e
    umount /mnt/verify-boot 2>/dev/null || true
    umount /mnt/verify-root 2>/dev/null || true
    losetup -d "$LOOP" 2>/dev/null || true
}
trap cleanup EXIT

mkdir -p /mnt/verify-boot /mnt/verify-root
mount -o ro "${LOOP}p1" /mnt/verify-boot
mount -o ro "${LOOP}p2" /mnt/verify-root

echo "loop=$LOOP"
lsblk -o NAME,SIZE,TYPE,FSTYPE,LABEL "$LOOP"

test -f /mnt/verify-boot/cubegm/vmlinux.uImage
test -f /mnt/verify-boot/cubegm/dtb.bin
test -f /mnt/verify-boot/cubegm/avp.uImage
test -f /mnt/verify-boot/MIPS_NATIVE/debian/enter_debian.sh
test -f /mnt/verify-root/etc/debian_version
test -x /mnt/verify-root/bin/sh

echo "debian=$(cat /mnt/verify-root/etc/debian_version)"
echo "boot_files_ok"
echo "root_files_ok"
