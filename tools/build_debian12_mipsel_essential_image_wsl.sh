#!/usr/bin/env bash
set -euo pipefail

IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_mipsel_essential.img}
SIZE_BYTES=${SIZE_BYTES:-31275876352}
WORKSPACE=${WORKSPACE:-/mnt/c/Work/r36sx_disasm}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ -e "$IMG" ]]; then
    echo "Image already exists: $IMG" >&2
    exit 1
fi

mkdir -p "$(dirname "$IMG")"
truncate -s "$SIZE_BYTES" "$IMG"

LOOP=$(losetup --find --show "$IMG")
echo "loop=$LOOP"
echo "image=$IMG"

cleanup() {
    set +e
    umount /mnt/r36sx-root/dev/pts 2>/dev/null || true
    umount /mnt/r36sx-root/dev 2>/dev/null || true
    umount /mnt/r36sx-root/sys 2>/dev/null || true
    umount /mnt/r36sx-root/proc 2>/dev/null || true
    umount /mnt/r36sx-root 2>/dev/null || true
    umount /mnt/r36sx-boot 2>/dev/null || true
    losetup -d "$LOOP" 2>/dev/null || true
}
trap cleanup EXIT

I_UNDERSTAND_ERASE_DISK3=yes \
WORKSPACE="$WORKSPACE" \
BOOT_SIZE_MIB=4096 \
BOOT_COPY_MODE=essential \
bash "$WORKSPACE/tools/install_debian12_mipsel_sd_wsl.sh" "$LOOP"

sync
cleanup
trap - EXIT

ls -lh "$IMG"
