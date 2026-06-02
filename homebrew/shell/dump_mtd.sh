#!/bin/sh
#
# Dump all MTD partitions to the SD card.
#
# Usage:
#   /mnt/sdcard/MIPS_NATIVE/shell/dump_mtd.sh [output-root]
#
# Default output root:
#   /mnt/sdcard/mtd-dump

set -u

DEFAULT_OUT_ROOT="/mnt/sdcard/mtd-dump"
OUT_ROOT="${1:-$DEFAULT_OUT_ROOT}"
PROC_MTD="/proc/mtd"
DD_BS=65536

log()
{
    echo "$*"
    if [ -n "${LOG_FILE:-}" ]; then
        echo "$*" >> "$LOG_FILE"
    fi
}

die()
{
    log "ERROR: $*"
    exit 1
}

timestamp()
{
    date +%Y%m%d_%H%M%S 2>/dev/null || echo unknown_time
}

unique_output_dir()
{
    base="$OUT_ROOT/$(timestamp)"
    dir="$base"
    n=1

    while [ -e "$dir" ]; do
        dir="${base}_$n"
        n=$((n + 1))
    done
    echo "$dir"
}

sanitize_name()
{
    echo "$1" | sed 's/^"//; s/"$//; s/[^A-Za-z0-9_.-]/_/g'
}

pick_mtd_device()
{
    num="$1"

    for dev in "/dev/mtd$num" "/dev/mtd${num}ro" "/dev/mtdblock$num"; do
        if [ -e "$dev" ]; then
            echo "$dev"
            return 0
        fi
    done

    return 1
}

dump_one_mtd()
{
    num="$1"
    size_hex="$2"
    erase_hex="$3"
    name="$4"
    safe_name="$(sanitize_name "$name")"
    [ -n "$safe_name" ] || safe_name="unnamed"

    src="$(pick_mtd_device "$num")" || {
        log "mtd$num: no readable /dev/mtd node found"
        printf 'mtd%s\tmissing\t%s\t%s\t%s\t-\t-\tmissing\n' \
            "$num" "$size_hex" "$erase_hex" "$name" >> "$MAP_FILE"
        return 1
    }

    out_base="mtd${num}_${safe_name}.bin"
    out_path="$OUT_DIR/$out_base"
    method="dd"

    log "Dumping mtd$num ($name) from $src to $out_base"

    if command -v nanddump >/dev/null 2>&1; then
        method="nanddump"
        if nanddump -f "$out_path" "$src" >> "$LOG_FILE" 2>&1; then
            printf 'mtd%s\t%s\t%s\t%s\t%s\t%s\t%s\tok\n' \
                "$num" "$src" "$size_hex" "$erase_hex" "$name" \
                "$out_base" "$method" >> "$MAP_FILE"
            DUMP_FILES="$DUMP_FILES $out_base"
            return 0
        fi
        log "mtd$num: nanddump failed, retrying with dd"
        rm -f "$out_path"
        method="dd"
    fi

    if dd if="$src" of="$out_path" bs="$DD_BS" >> "$LOG_FILE" 2>&1; then
        printf 'mtd%s\t%s\t%s\t%s\t%s\t%s\t%s\tok\n' \
            "$num" "$src" "$size_hex" "$erase_hex" "$name" \
            "$out_base" "$method" >> "$MAP_FILE"
        DUMP_FILES="$DUMP_FILES $out_base"
        return 0
    fi

    log "mtd$num: dump failed"
    rm -f "$out_path"
    printf 'mtd%s\t%s\t%s\t%s\t%s\t%s\t%s\tfailed\n' \
        "$num" "$src" "$size_hex" "$erase_hex" "$name" \
        "$out_base" "$method" >> "$MAP_FILE"
    return 1
}

[ -r "$PROC_MTD" ] || die "$PROC_MTD is not readable"

OUT_DIR="$(unique_output_dir)"
mkdir -p "$OUT_DIR" || die "cannot create $OUT_DIR"

LOG_FILE="$OUT_DIR/dump.log"
MAP_FILE="$OUT_DIR/mtd_map.tsv"
DUMP_FILES=""
ok_count=0
fail_count=0

log "MTD dump output: $OUT_DIR"
log "Started: $(date 2>/dev/null || echo unknown)"

cp "$PROC_MTD" "$OUT_DIR/proc_mtd.txt" 2>> "$LOG_FILE" || \
    die "cannot copy $PROC_MTD"
[ -r /proc/cmdline ] && cp /proc/cmdline "$OUT_DIR/proc_cmdline.txt" 2>> "$LOG_FILE"
[ -r /proc/mounts ] && cp /proc/mounts "$OUT_DIR/proc_mounts.txt" 2>> "$LOG_FILE"
uname -a > "$OUT_DIR/uname.txt" 2>> "$LOG_FILE"
df -h "$OUT_ROOT" > "$OUT_DIR/df_output_root.txt" 2>> "$LOG_FILE"

printf 'mtd\tdevice\tsize_hex\terase_hex\tname\tfile\tmethod\tstatus\n' \
    > "$MAP_FILE"

while read dev size_hex erase_hex name_rest; do
    case "$dev" in
        mtd*:)
            num="${dev#mtd}"
            num="${num%:}"
            name="${name_rest:-unnamed}"
            if dump_one_mtd "$num" "$size_hex" "$erase_hex" "$name"; then
                ok_count=$((ok_count + 1))
            else
                fail_count=$((fail_count + 1))
            fi
            ;;
    esac
done < "$PROC_MTD"

if [ "$ok_count" -eq 0 ]; then
    die "no MTD partitions were dumped"
fi

if command -v sha256sum >/dev/null 2>&1; then
    (
        cd "$OUT_DIR" || exit 1
        for f in $DUMP_FILES; do
            sha256sum "$f"
        done
    ) > "$OUT_DIR/sha256sum.txt"
elif command -v md5sum >/dev/null 2>&1; then
    (
        cd "$OUT_DIR" || exit 1
        for f in $DUMP_FILES; do
            md5sum "$f"
        done
    ) > "$OUT_DIR/md5sum.txt"
fi

sync

log "Finished: $(date 2>/dev/null || echo unknown)"
log "Dumped: $ok_count partition(s), failed: $fail_count"
log "Output directory: $OUT_DIR"

if [ "$fail_count" -ne 0 ]; then
    exit 2
fi
exit 0
