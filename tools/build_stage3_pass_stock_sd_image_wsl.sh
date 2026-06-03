#!/usr/bin/env bash
set -euo pipefail

OUT_IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_stage3_pass_stockfat.img}
WORKSPACE=${WORKSPACE:-/mnt/c/Work/r36sx_disasm}
SIZE_MIB=${SIZE_MIB:-2048}
PART_START_MIB=${PART_START_MIB:-4}
COPY_MEDIA=${COPY_MEDIA:-1}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ -e "$OUT_IMG" ]]; then
    echo "Output image already exists: $OUT_IMG" >&2
    exit 1
fi

for cmd in losetup parted partprobe mkfs.vfat fsck.vfat rsync mount umount truncate sha256sum find sort mktemp cp rm; do
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
    "$WORKSPACE/disk_image/MIPS_NATIVE"
    "$WORKSPACE/disk_image/Chkdsk.bat"
    "$WORKSPACE/disk_image/chkdsk.exe"
)

for source_path in "${required_sources[@]}"; do
    if [[ ! -e "$source_path" ]]; then
        echo "Missing source path: $source_path" >&2
        exit 1
    fi
done

if (( SIZE_MIB < 1536 )); then
    echo "SIZE_MIB is too small for the stock runtime set; use at least 1536" >&2
    exit 1
fi

OUT_LOOP=
OUT_BOOT=/mnt/r36sx-stage3-pass-boot
WORK_IMG=
COPY_DONE=0

cleanup() {
    set +e
    umount "$OUT_BOOT" 2>/dev/null || true
    [[ -n "$OUT_LOOP" ]] && losetup -d "$OUT_LOOP" 2>/dev/null || true
    [[ -n "$WORK_IMG" && -e "$WORK_IMG" ]] && rm -f "$WORK_IMG" 2>/dev/null || true
    [[ "$COPY_DONE" == "0" && -e "$OUT_IMG" ]] && rm -f "$OUT_IMG" 2>/dev/null || true
}
trap cleanup EXIT

mkdir -p "$(dirname "$OUT_IMG")"
WORK_IMG=$(mktemp /tmp/r36sx-stage3-pass.XXXXXX.img)
truncate -s "${SIZE_MIB}M" "$WORK_IMG"

OUT_LOOP=$(losetup --find --show "$WORK_IMG")
parted -s "$OUT_LOOP" mklabel msdos
parted -s "$OUT_LOOP" mkpart primary fat32 "${PART_START_MIB}MiB" 100%
parted -s "$OUT_LOOP" set 1 boot on
partprobe "$OUT_LOOP" || true
sleep 2

mkfs.vfat -F 32 -n R36SXBOOT "${OUT_LOOP}p1"

mkdir -p "$OUT_BOOT"
mount -t vfat -o utf8=1,shortname=mixed "${OUT_LOOP}p1" "$OUT_BOOT"

rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/cubegm" "$OUT_BOOT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/rootfs" "$OUT_BOOT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/MIPS_NATIVE" "$OUT_BOOT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/Chkdsk.bat" "$OUT_BOOT/"
rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/chkdsk.exe" "$OUT_BOOT/"

if [[ "$COPY_MEDIA" == "1" ]]; then
    for item in Movie Ebook Music; do
        if [[ -e "$WORKSPACE/disk_image/$item" ]]; then
            rsync -rt --inplace --modify-window=2 "$WORKSPACE/disk_image/$item" "$OUT_BOOT/"
        fi
    done
fi

sync

echo "Stage 3 boot-resource checks:"
for file in dtb.bin avp.uImage vmlinux.uImage xgame-logo.bmp; do
    test -f "$OUT_BOOT/cubegm/$file"
    src_hash=$(sha256sum "$WORKSPACE/disk_image/cubegm/$file" | awk '{print $1}')
    out_hash=$(sha256sum "$OUT_BOOT/cubegm/$file" | awk '{print $1}')
    if [[ "$src_hash" != "$out_hash" ]]; then
        echo "Hash mismatch for cubegm/$file" >&2
        exit 1
    fi
    printf '  ok  cubegm/%s  %s\n' "$file" "$out_hash"
done

echo
echo "Top-level FAT contents:"
find "$OUT_BOOT" -maxdepth 1 -mindepth 1 -printf '  %f\n' | sort

umount "$OUT_BOOT"
fsck.vfat -vn "${OUT_LOOP}p1"
losetup -d "$OUT_LOOP"
OUT_LOOP=

cp --sparse=always "$WORK_IMG" "$OUT_IMG"
COPY_DONE=1

echo
echo "Created Stage-3-compatible stock FAT image:"
ls -lh "$OUT_IMG"
