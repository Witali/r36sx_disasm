#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
Usage: bash homebrew/pico_286/build_pico_286_wsl.sh [options]

Build Pico-286 under WSL with the Linux mips-mti-linux-gnu GCC toolchain.

Options:
  --debug-log              Build with DEBUG=1.
  --disable-profiling      Compile out runtime profiling helpers.
  --disable-computed-goto  Use the switch opcode dispatcher.
  --disable-fast-memory    Use indirect memory function pointers in CPU code.
  --opt-level LEVEL        GCC optimization level. Default: O2.
  --enable-mips-dsp        Experimental buffer helpers with MIPS DSP Rev2.
  --strip                  Run mips-mti-linux-gnu-strip on the output.
  --out PATH               Output path. Default: homebrew/pico_286/pico_286.gcc
  --help                   Show this help.
USAGE
}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
PICO_ROOT="$SCRIPT_DIR/pico-286"
PORT_ROOT="$SCRIPT_DIR/r36sx_port"
SDK_ROOT="$ROOT/tools/mipsel-buildroot-linux-gnu_sdk-buildroot"
TOOLCHAIN_ROOT="$SDK_ROOT/opt/ext-toolchain"
SYSROOT="$SDK_ROOT/mipsel-buildroot-linux-gnu/sysroot"
CC="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-gcc"
CXX="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-g++"
LD="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-ld"
STRIP="$TOOLCHAIN_ROOT/bin/mips-mti-linux-gnu-strip"
OUT="$SCRIPT_DIR/pico_286.gcc"
OBJ_DIR="$SCRIPT_DIR/obj-gcc"
COMPAT_HEADER="$SCRIPT_DIR/r36sx_pico286_compat.h"
TARGET_ZLIB="$SYSROOT/usr/lib/libz.so.1.2.11"
GCC_LIB="$TOOLCHAIN_ROOT/lib/gcc/mips-mti-linux-gnu/6.3.0/mipsel-r2-hard/lib"
CXX_LIB="$TOOLCHAIN_ROOT/mips-mti-linux-gnu/lib/mipsel-r2-hard/lib"
CRT1="$SYSROOT/usr/lib/crt1.o"
CRTI="$SYSROOT/usr/lib/crti.o"
CRTN="$SYSROOT/usr/lib/crtn.o"
CRTBEGIN="$GCC_LIB/crtbegin.o"
CRTEND="$GCC_LIB/crtend.o"
DEBUG_VALUE=0
PROFILING_VALUE=1
COMPUTED_GOTO_VALUE=1
FAST_MEMORY_VALUE=1
DO_STRIP=0
OPT_LEVEL=O2
MIPS_DSP_VALUE=0

while (($#)); do
    case "$1" in
        --debug-log)
            DEBUG_VALUE=1
            shift
            ;;
        --disable-profiling)
            PROFILING_VALUE=0
            shift
            ;;
        --disable-computed-goto)
            COMPUTED_GOTO_VALUE=0
            shift
            ;;
        --disable-fast-memory)
            FAST_MEMORY_VALUE=0
            shift
            ;;
        --opt-level)
            if (($# < 2)); then
                echo "--opt-level requires a value such as O2 or O3" >&2
                exit 2
            fi
            case "$2" in
                O0|O1|O2|O3|Os|Og|Ofast)
                    OPT_LEVEL="$2"
                    ;;
                *)
                    echo "Unsupported --opt-level value: $2" >&2
                    exit 2
                    ;;
            esac
            shift 2
            ;;
        --enable-mips-dsp)
            MIPS_DSP_VALUE=1
            shift
            ;;
        --strip)
            DO_STRIP=1
            shift
            ;;
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

for path in "$PICO_ROOT" "$PORT_ROOT" "$CC" "$CXX" "$LD" "$SYSROOT" "$TARGET_ZLIB"; do
    if [[ ! -e "$path" ]]; then
        echo "Missing required path: $path" >&2
        exit 1
    fi
done

OBJ_DIR_FULL="$(readlink -m "$OBJ_DIR")"
EXPECTED_OBJ_DIR="$(readlink -m "$SCRIPT_DIR/obj-gcc")"
if [[ "$OBJ_DIR_FULL" != "$EXPECTED_OBJ_DIR" ]]; then
    echo "Refusing to clean unexpected object directory: $OBJ_DIR_FULL" >&2
    exit 1
fi
rm -rf "$OBJ_DIR_FULL"
mkdir -p "$OBJ_DIR_FULL"

include_args=(
    "-I$PORT_ROOT"
    "-I$SCRIPT_DIR"
    "-I$ROOT/homebrew/common"
    "-I$PICO_ROOT/src"
    "-I$PICO_ROOT/src/emulator"
    "-I$PICO_ROOT/src/emu8950"
    "-I$PICO_ROOT/src/printf"
    "-I$PICO_ROOT/findfirst"
    "-isystem" "$SYSROOT/usr/include"
)

common_args=(
    "-EL"
    "-mips32r2"
    "-mabi=32"
    "-march=mips32r2"
    "-mtune=74kc"
    "-DPICO_RP2040=0"
    "-DPICO_RP2350=0"
    "-DDEBUG=$DEBUG_VALUE"
    "-DR36SX_ENABLE_PROFILING=$PROFILING_VALUE"
    "-DR36SX_CPU_COMPUTED_GOTO=$COMPUTED_GOTO_VALUE"
    "-DR36SX_NATIVE_FAST_MEMORY=$FAST_MEMORY_VALUE"
    "-DR36SX_SEGMENT_BASE_CACHE=1"
    "-DCPU_386_EXTENDED_OPS=1"
    "-DR36SX_RUNTIME_SOUND_FREQUENCY=1"
    "-DR36SX_VIDEO_DIRTY_TRACKING=1"
    "-DR36SX_MIPS_DSP=$MIPS_DSP_VALUE"
    "-DINI_HANDLER_LINENO=1"
    "-DINI_MAX_LINE=512"
    "-DINI_ALLOW_MULTILINE=0"
    "-DUSE_EMU8950_OPL"
    "-DEMU8950_SLOT_RENDER=1"
    "-DEMU8950_ASM=0"
    "-DEMU8950_NO_RATECONV=1"
    "-DEMU8950_NO_WAVE_TABLE_MAP=1"
    "-DEMU8950_NO_TLL=1"
    "-DEMU8950_NO_FLOAT=1"
    "-DEMU8950_NO_TIMER=1"
    "-DEMU8950_NO_TEST_FLAG=1"
    "-DEMU8950_SIMPLER_NOISE=1"
    "-DEMU8950_SHORT_NOISE_UPDATE_CHECK=1"
    "-DEMU8950_LINEAR_SKIP=1"
    "-DEMU8950_LINEAR_END_OF_NOTE_OPTIMIZATION"
    "-DEMU8950_NO_PERCUSSION_MODE=1"
    "-DEMU8950_LINEAR=1"
    "-include" "$COMPAT_HEADER"
    "-$OPT_LEVEL"
    "-fPIC"
    "-fms-extensions"
    "-fno-strict-aliasing"
    "-fno-builtin-memset"
    "-fno-builtin-memcpy"
    "-Wall"
    "-Wextra"
    "-Wno-unused-parameter"
    "-Wno-unused-function"
    "-Wno-missing-field-initializers"
)

if ((MIPS_DSP_VALUE)); then
    common_args+=("-mdspr2")
fi

objects=()

object_path_for() {
    local src="$1"
    local rel="${src#$ROOT/}"
    rel="${rel//\//_}"
    rel="${rel// /_}"
    printf '%s/%s.o' "$OBJ_DIR_FULL" "${rel%.*}"
}

compile_c() {
    local src="$1"
    local obj
    obj="$(object_path_for "$src")"
    "$CC" "${common_args[@]}" "${include_args[@]}" -std=gnu11 -c "$src" -o "$obj"
    objects+=("$obj")
}

compile_cpp() {
    local src="$1"
    local obj
    obj="$(object_path_for "$src")"
    "$CXX" "${common_args[@]}" "${include_args[@]}" \
        -std=gnu++14 -fpermissive -fno-exceptions -fno-rtti \
        -c "$src" -o "$obj"
    objects+=("$obj")
}

compile_asm() {
    local src="$1"
    local obj
    local asm_args=(
        -EL
        -mips32r2
        -mabi=32
        -march=mips32r2
        -mtune=74kc
        "-DR36SX_MIPS_DSP=$MIPS_DSP_VALUE"
    )
    obj="$(object_path_for "$src")"
    if ((MIPS_DSP_VALUE)); then
        asm_args+=("-mdspr2")
    fi
    "$CC" "${asm_args[@]}" -x assembler-with-cpp -c "$src" -o "$obj"
    objects+=("$obj")
}

mapfile -t emulator_c < <(
    find "$PICO_ROOT/src/emulator" -type f -name '*.c' \
        ! -name 'cpu.c' ! -name 'ports.c' | sort
)
for src in "${emulator_c[@]}"; do
    compile_c "$src"
done

mapfile -t emu8950_c < <(find "$PICO_ROOT/src/emu8950" -maxdepth 1 -type f -name '*.c' | sort)
for src in "${emu8950_c[@]}"; do
    compile_c "$src"
done

mapfile -t findfirst_c < <(find "$PICO_ROOT/findfirst" -maxdepth 1 -type f -name '*.c' | sort)
for src in "${findfirst_c[@]}"; do
    compile_c "$src"
done

for src in \
    "$SCRIPT_DIR/r36sx_mips_dsp_probe.S" \
    "$PICO_ROOT/src/printf/printf.c" \
    "$ROOT/homebrew/common/r36sx_screen_keyboard.c" \
    "$ROOT/homebrew/common/inih/ini.c" \
    "$SCRIPT_DIR/r36sx_disk_menu.c" \
    "$SCRIPT_DIR/r36sx_key_presets.c" \
    "$SCRIPT_DIR/r36sx_minifb.c" \
    "$SCRIPT_DIR/r36sx_linux_audio.c" \
    "$PORT_ROOT/r36sx_app_stats.c" \
    "$PORT_ROOT/r36sx_bios_rom.c" \
    "$PORT_ROOT/r36sx_host_disk_io.c" \
    "$PORT_ROOT/r36sx_disk_config.c" \
    "$PORT_ROOT/r36sx_profile.c" \
    "$PORT_ROOT/r36sx_cpu.c" \
    "$PORT_ROOT/r36sx_ports.c"; do
    case "$src" in
        *.S) compile_asm "$src" ;;
        *) compile_c "$src" ;;
    esac
done

compile_cpp "$PICO_ROOT/src/emu8950/slot_render.cpp"
compile_cpp "$PORT_ROOT/r36sx_linux-main.cpp"

"$LD" \
    --sysroot="$SYSROOT" \
    -EL \
    -m elf32ltsmip \
    --dynamic-linker=/lib/ld.so.1 \
    --hash-style=sysv \
    "-L$SYSROOT/lib" \
    "-L$SYSROOT/usr/lib" \
    "-L$GCC_LIB" \
    "-L$CXX_LIB" \
    -o "$OUT" \
    "$CRT1" \
    "$CRTI" \
    "$CRTBEGIN" \
    "${objects[@]}" \
    -lpthread \
    -ldl \
    -lm \
    "$TARGET_ZLIB" \
    -lstdc++ \
    -lgcc_s \
    -lc \
    "$CRTEND" \
    "$CRTN"

if ((DO_STRIP)); then
    "$STRIP" --strip-all "$OUT"
fi

echo "Built $OUT"
