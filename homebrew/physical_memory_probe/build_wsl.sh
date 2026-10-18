#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROBE_REPO="$(cd "$SCRIPT_DIR/../.." && pwd)"
PROBE_TC="$PROBE_REPO/tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain"
PROBE_SYSROOT="$PROBE_TC/sysroot/mipsel-r2-soft"
PROBE_OUT="$PROBE_REPO/patches/disk_image_patch_physical_memory_probe/MIPS_NATIVE/physical_memory_probe"
mkdir -p "$PROBE_OUT"
"$PROBE_TC/bin/mips-mti-linux-gnu-gcc" --sysroot="$PROBE_SYSROOT" \
    -EL -mips32r2 -mabi=32 -msoft-float -O2 -Wall -Wextra -Werror -std=gnu11 -static \
    -isystem "$PROBE_SYSROOT/usr/include" -B"$PROBE_SYSROOT/usr/lib/" \
    -L"$PROBE_SYSROOT/usr/lib" -L"$PROBE_SYSROOT/lib" \
    "$SCRIPT_DIR/physical_memory_probe.c" -o "$PROBE_OUT/physical_memory_probe"
"$PROBE_TC/bin/mips-mti-linux-gnu-strip" "$PROBE_OUT/physical_memory_probe"
"$PROBE_TC/bin/mips-mti-linux-gnu-readelf" -h "$PROBE_OUT/physical_memory_probe"
cp "$SCRIPT_DIR/README.md" "$PROBE_OUT/README.md"
