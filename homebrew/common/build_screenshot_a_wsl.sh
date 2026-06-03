#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SDK_ROOT="$ROOT/tools/mipsel-buildroot-linux-gnu_sdk-buildroot"
TOOLCHAIN_ROOT="$SDK_ROOT/opt/ext-toolchain"
SYSROOT="$TOOLCHAIN_ROOT/sysroot/mipsel-r2-hard"
BUILDROOT_SYSROOT="$SDK_ROOT/mipsel-buildroot-linux-gnu/sysroot"
CC="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-gcc"
AR="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-ar"
RANLIB="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-ranlib"
OUT="$SCRIPT_DIR/screenshot.a"
OBJ_DIR="$SCRIPT_DIR/.build-screenshot-a"
OBJ="$OBJ_DIR/r36sx_screenshot_module.o"

usage() {
    cat <<'USAGE'
Usage: bash homebrew/common/build_screenshot_a_wsl.sh [options]

Build the RGB565 screenshot static archive for R36SX homebrew.

Options:
  --out PATH  Output .a path. Default: homebrew/common/screenshot.a
  --help      Show this help.

The archive contains the screenshot encoder object.  Applications that link it
statically must also link the target zlib used by the PNG writer.
USAGE
}

while (($#)); do
    case "$1" in
        --out)
            if (($# < 2)); then
                echo "--out requires a path" >&2
                exit 2
            fi
            OUT="$2"
            shift 2
            ;;
        --help|-h)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

for path in "$CC" "$AR" "$RANLIB" "$SYSROOT"; do
    if [[ ! -e "$path" ]]; then
        echo "Missing required path: $path" >&2
        exit 1
    fi
done

mkdir -p "$(dirname "$OUT")" "$OBJ_DIR"

"$CC" \
    --sysroot="$SYSROOT" \
    -EL \
    -mips32r2 \
    -mabi=32 \
    -mhard-float \
    -march=74kf \
    -mtune=74kf \
    -O2 \
    -fno-pic \
    -I"$SCRIPT_DIR" \
    -isystem "$SYSROOT/usr/include" \
    -isystem "$BUILDROOT_SYSROOT/usr/include" \
    -c "$SCRIPT_DIR/r36sx_screenshot_module.c" \
    -o "$OBJ"

rm -f "$OUT"
"$AR" rcs "$OUT" "$OBJ"
"$RANLIB" "$OUT"

file "$OUT"
echo "Built screenshot static archive: $OUT"
