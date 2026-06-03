#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SDK_ROOT="$ROOT/tools/mipsel-buildroot-linux-gnu_sdk-buildroot"
TOOLCHAIN_ROOT="$SDK_ROOT/opt/ext-toolchain"
SYSROOT="$TOOLCHAIN_ROOT/sysroot/mipsel-r2-hard"
BUILDROOT_SYSROOT="$SDK_ROOT/mipsel-buildroot-linux-gnu/sysroot"
CC="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-gcc"
STRIP="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-strip"
TARGET_ZLIB="$BUILDROOT_SYSROOT/usr/lib/libz.so.1.2.11"
OUT="$SCRIPT_DIR/screenshot.so"
OBJ_DIR="$SCRIPT_DIR/.build-screenshot-so"
PATCHED_LIB_DIR="$OBJ_DIR/sysroot-lib"
DO_STRIP=0

usage() {
    cat <<'USAGE'
Usage: bash homebrew/common/build_screenshot_so_wsl.sh [options]

Build the optional RGB565 screenshot shared object for R36SX homebrew.

Options:
  --out PATH  Output .so path. Default: homebrew/common/screenshot.so
  --strip     Strip the output shared object.
  --help      Show this help.
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
        --strip)
            DO_STRIP=1
            shift
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

for path in "$CC" "$SYSROOT" "$TARGET_ZLIB"; do
    if [[ ! -e "$path" ]]; then
        echo "Missing required path: $path" >&2
        exit 1
    fi
done

mkdir -p "$(dirname "$OUT")"
mkdir -p "$PATCHED_LIB_DIR"

sed \
    -e "s# /lib/libc.so.6# $SYSROOT/lib/libc.so.6#g" \
    -e "s# /usr/lib/libc_nonshared.a# $SYSROOT/usr/lib/libc_nonshared.a#g" \
    -e "s# /lib/ld.so.1# $SYSROOT/lib/ld.so.1#g" \
    "$SYSROOT/usr/lib/libc.so" > "$PATCHED_LIB_DIR/libc.so"

"$CC" \
    --sysroot="$SYSROOT" \
    -EL \
    -mips32r2 \
    -mabi=32 \
    -mhard-float \
    -march=74kf \
    -mtune=74kf \
    -O2 \
    -fPIC \
    -shared \
    -B"$SYSROOT/usr/lib/" \
    -L"$PATCHED_LIB_DIR" \
    -L"$SYSROOT/lib" \
    -L"$SYSROOT/usr/lib" \
    -Wl,--hash-style=sysv \
    -Wl,-soname,screenshot.so \
    -Wl,-rpath-link,"$SYSROOT/lib" \
    -Wl,-rpath-link,"$SYSROOT/usr/lib" \
    -I"$SCRIPT_DIR" \
    -isystem "$SYSROOT/usr/include" \
    -isystem "$BUILDROOT_SYSROOT/usr/include" \
    "$SCRIPT_DIR/r36sx_screenshot_module.c" \
    "$TARGET_ZLIB" \
    -o "$OUT"

if ((DO_STRIP)); then
    "$STRIP" --strip-all "$OUT"
fi

file "$OUT"
echo "Built screenshot module: $OUT"
