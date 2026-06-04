#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
SDK_ROOT="$ROOT_DIR/tools/mipsel-buildroot-linux-gnu_sdk-buildroot"
TOOLCHAIN_ROOT="$SDK_ROOT/opt/ext-toolchain"
SYSROOT="$TOOLCHAIN_ROOT/sysroot/mipsel-r2-hard"
PATCH_ROOT="$ROOT_DIR/patches/disk_image_patch_fb_test"

CC="${CC:-$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-gcc}"
STRIP="${STRIP:-$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-strip}"
OUT="$SCRIPT_DIR/fb-test"
DO_INSTALL=0
DO_STRIP=0

usage() {
    cat <<EOF
Usage: $(basename "$0") [--install] [--strip] [--out PATH]

Builds the R36SX direct framebuffer test with the local MIPS GCC toolchain.

  --install  copy the binary and README files into disk_image/MIPS_NATIVE/fb-test
             and patches/disk_image_patch_fb_test/MIPS_NATIVE/fb-test
  --strip    strip the output binary
  --out PATH write the binary to PATH
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --install)
            DO_INSTALL=1
            ;;
        --strip)
            DO_STRIP=1
            ;;
        --out)
            shift
            [[ $# -gt 0 ]] || { usage; exit 2; }
            OUT="$1"
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
    shift
done

if [[ ! -x "$CC" ]]; then
    echo "MIPS GCC not found: $CC" >&2
    exit 1
fi
if [[ ! -d "$SYSROOT/usr/include" ]]; then
    echo "Target sysroot headers not found: $SYSROOT/usr/include" >&2
    exit 1
fi

OBJ_DIR="$SCRIPT_DIR/.build"
PATCHED_LIB_DIR="$OBJ_DIR/sysroot-lib"
mkdir -p "$OBJ_DIR" "$(dirname "$OUT")" "$PATCHED_LIB_DIR"

sed \
    -e "s# /lib/libc.so.6# $SYSROOT/lib/libc.so.6#g" \
    -e "s# /usr/lib/libc_nonshared.a# $SYSROOT/usr/lib/libc_nonshared.a#g" \
    -e "s# /lib/ld.so.1# $SYSROOT/lib/ld.so.1#g" \
    "$SYSROOT/usr/lib/libc.so" > "$PATCHED_LIB_DIR/libc.so"

CFLAGS=(
    --sysroot="$SYSROOT"
    -EL
    -mips32r2
    -mabi=32
    -mhard-float
    -march=74kf
    -mtune=74kf
    -O2
    -g0
    -fno-pic
    -fno-strict-aliasing
    -Wall
    -Wextra
    -std=gnu11
    -isystem "$SYSROOT/usr/include"
)

LDFLAGS=(
    --sysroot="$SYSROOT"
    -B"$SYSROOT/usr/lib/"
    -Wl,--sysroot="$SYSROOT"
    -Wl,--dynamic-linker=/lib/ld.so.1
    -Wl,--hash-style=sysv
    -Wl,-rpath-link,"$SYSROOT/lib"
    -Wl,-rpath-link,"$SYSROOT/usr/lib"
    -L"$PATCHED_LIB_DIR"
    -L"$SYSROOT/lib"
    -L"$SYSROOT/usr/lib"
)

"$CC" "${CFLAGS[@]}" -c "$SCRIPT_DIR/fb_test.c" \
    -o "$OBJ_DIR/fb_test.o"
"$CC" "${CFLAGS[@]}" "${LDFLAGS[@]}" \
    "$OBJ_DIR/fb_test.o" \
    -o "$OUT"

if [[ "$DO_STRIP" -eq 1 ]]; then
    "$STRIP" "$OUT"
fi

if [[ "$DO_INSTALL" -eq 1 ]]; then
    install -d "$ROOT_DIR/disk_image/MIPS_NATIVE/fb-test"
    install -d "$PATCH_ROOT/MIPS_NATIVE/fb-test"
    install -m 0755 "$OUT" \
        "$ROOT_DIR/disk_image/MIPS_NATIVE/fb-test/fb-test"
    install -m 0755 "$OUT" \
        "$PATCH_ROOT/MIPS_NATIVE/fb-test/fb-test"
    install -m 0644 "$SCRIPT_DIR/README.md" \
        "$ROOT_DIR/disk_image/MIPS_NATIVE/fb-test/README.md"
    install -m 0644 "$SCRIPT_DIR/README.md" \
        "$PATCH_ROOT/MIPS_NATIVE/fb-test/README.md"
    install -m 0644 "$SCRIPT_DIR/README_ru.md" \
        "$ROOT_DIR/disk_image/MIPS_NATIVE/fb-test/README_ru.md"
    install -m 0644 "$SCRIPT_DIR/README_ru.md" \
        "$PATCH_ROOT/MIPS_NATIVE/fb-test/README_ru.md"
fi

file "$OUT"
echo "Built: $OUT"
if [[ "$DO_INSTALL" -eq 1 ]]; then
    echo "Installed: $ROOT_DIR/disk_image/MIPS_NATIVE/fb-test/fb-test"
    echo "Patch: $PATCH_ROOT/MIPS_NATIVE/fb-test/fb-test"
fi

