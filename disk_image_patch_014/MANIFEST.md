# disk_image_patch_014

Button Demo rebuild with a short generated sound on button press.

## Purpose

Adds a small "pew-pew" audio effect to the working Button Demo core. The square,
button log, raw joypad ID display, and 640x480 framebuffer behavior remain.

## Runtime behavior

- Any newly pressed button triggers a short two-part descending chirp.
- Audio is generated internally as 44.1 kHz stereo PCM.
- The core uses the libretro audio batch callback when available.
- If only the single-sample callback is available, the same generated samples
  are sent through that callback.
- Audio is only emitted while the effect is active, keeping callback use narrow.

## Changed/added files

- `cubegm/cores/libemu_buttondemo.so`
- `cubegm/cores/config.xml`
- `cubegm/cores/filelist.xml`
- `GB/filelist.csv`
- `GB/button.gb`
- `GB/button.demo`

## Build command

From the repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

The `-fno-builtin` flag is required here. Without it, Clang/Zig optimized local
zeroing code into an external `memset` symbol.

## Verification

- `libemu_buttondemo.so` is ELF32 little-endian MIPS.
- ELF flags: `0x70001007`.
- Size: `30220` bytes.
- `NEEDED`: empty.
- Undefined dynamic symbols: `0`.
- Required exported symbols are present, including:
  - `retro_run`
  - `retro_load_game`
  - `retro_set_audio_sample`
  - `retro_set_audio_sample_batch`
  - `retro_get_system_av_info`
  - `check_encrypty`
  - `CheckEncrypty`
- No `__ubsan`, `GLIBC`, `ld-linux`, `libc.so`, `libm.so`, or `memset` strings
  were found in the built shared object.
- Microsoft Defender scan: no threats found.
- `libemu_buttondemo.so` SHA256: `6B9DAD3DCB3CE4B7B9B7CC63C1CAE8660839603B81D5AF0AF3D3D36CA452B378`
- `GB/button.gb` SHA256: `87F14C3E75507D980DE3458DBE6C89561E29ACAD02AD2CD1CE10350704364C0F`
- `GB/button.demo` SHA256: `87F14C3E75507D980DE3458DBE6C89561E29ACAD02AD2CD1CE10350704364C0F`

## Install

Copy the contents of this patch directory over the root of the SD card/image.

The known test entry remains:

```text
GB -> Button Demo GB
```
