#!/usr/bin/env bash
set -euo pipefail

IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_stage3_pass_stockfat.img}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ ! -e "$IMG" ]]; then
    echo "Image not found: $IMG" >&2
    exit 1
fi

for cmd in losetup parted lsblk mount umount sha256sum dtc grep; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "Missing required command: $cmd" >&2
        exit 1
    }
done

LOOP=
MNT=/mnt/r36sx-stage3-final-verify
DTS=/tmp/r36sx-stage3-final-verify.dts

cleanup() {
    set +e
    umount "$MNT" 2>/dev/null || true
    [[ -n "$LOOP" ]] && losetup -d "$LOOP" 2>/dev/null || true
}
trap cleanup EXIT

LOOP=$(losetup --find --show -P "$IMG")
mkdir -p "$MNT"

echo "image=$IMG"
echo "loop=$LOOP"
lsblk -o NAME,SIZE,TYPE,FSTYPE,LABEL "$LOOP"
parted -s "$LOOP" unit MiB print

mount -o ro "${LOOP}p1" "$MNT"

echo
echo "stage3_files:"
for file in dtb.bin avp.uImage vmlinux.uImage xgame-logo.bmp; do
    test -f "$MNT/cubegm/$file"
    sha256sum "$MNT/cubegm/$file"
done

dtc -I dtb -O dts -o "$DTS" "$MNT/cubegm/dtb.bin" 2>/dev/null

echo
echo "dtb_stage3_properties:"
grep -E -e "external_files" \
    -e "path-prefix" \
    -e "part[0-9]-(label|filename)" \
    -e "bootargs" \
    "$DTS"
