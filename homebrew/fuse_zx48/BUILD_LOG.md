# Fuse ZX Spectrum 48K core

Experimental MIPS32 little-endian build of `libretro/fuse-libretro` for the `cubegm/rkgame` launcher.

Command-level reproduction notes are documented in:

```text
homebrew/fuse_zx48/BUILD_COMMANDS.md
```

## Source

- Repository: `https://github.com/libretro/fuse-libretro.git`
- Local path: `internet_sources/fuse-libretro`
- Commit: `bce196f`
- Scan: Microsoft Defender found no threats in the downloaded source tree.

## Build

The workspace did not have `make` on `PATH`, so the Makefile source list was reused from `build/Makefile.common` and compiled with local Zig.

Generated files:

```text
internet_sources/fuse-libretro/fuse/config.h
internet_sources/fuse-libretro/libspectrum/config.h
internet_sources/fuse-libretro/src/version.c
```

Compilation settings:

```text
zig cc -target mipsel-linux-gnu -march=mips32r2 --sysroot=<mipsel sysroot> -isystem <sysroot>/usr/include -O2 -fomit-frame-pointer -fPIC -D__LIBRETRO__ -DHAVE_CONFIG_H
```

Link settings:

```text
zig cc -target mipsel-linux-gnu -march=mips32r2 -shared -nostdlib -Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T -o libemu_fuse.so <objects> r36sx_shim.o
```

The final file was copied as:

```text
homebrew/fuse_zx48/libemu_fuse.so
```

## Verification Of First Build

- ELF32 little-endian shared object.
- Machine: MIPS.
- Flags: `0x70001007`.
- Exported symbols include `retro_get_system_av_info`, `retro_load_game`, `retro_run`, `retro_set_environment`, `retro_set_video_refresh`, `check_encrypty`, and `CheckEncrypty`.
- `NEEDED` list is empty because this build uses `-nostdlib`; standard libc/libm symbols are expected to resolve from the `rkgame` process at `dlopen` time.
- Microsoft Defender found no threats in the built `.so`.
- SHA256: `BEBCFF5B456A10C6888ED20F5AACE173CC60B8668D037F98E2FD9411EAFB62A5`

## R36SX Compatibility Rebuild

After the first device test showed a short black screen followed by a return to
the launcher, two likely compatibility problems were addressed.

First, `src/libretro.c` was patched so `retro_load_game()` does not fail when
`rkgame` ignores `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT`. The core continues with
the default RGB565 format.

Patch file:

```text
homebrew/fuse_zx48/r36sx_fuse_compat.patch
```

Second, the module was relinked with explicit dynamic dependencies from the
device image:

```text
libm.so.6
libgcc_s.so.1
libc.so.6
```

New verification:

- ELF32 little-endian shared object.
- Machine: MIPS.
- Flags: `0x70001007`.
- `NEEDED`: `libm.so.6`, `libgcc_s.so.1`, `libc.so.6`.
- Required exports are present, including `retro_load_game`, `retro_run`,
  `check_encrypty`, and `CheckEncrypty`.
- Microsoft Defender found no threats in the rebuilt `.so`.
- SHA256: `29DEE42C31586965415A57CE69A17E19DB0AD3D813656CFEDCF2EAA9AE9AE10D`

## Minimal No-NEEDED Rebuild

The explicit-`NEEDED` build also returned to the launcher on device, so a
smaller no-`NEEDED` build was produced for `disk_image_patch_013`.

This build keeps the R36SX `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT` compatibility
patch, but compiles with size-oriented flags and links without passing any
runtime libraries:

```text
-Oz
-ffunction-sections
-fdata-sections
-fno-unwind-tables
-fno-asynchronous-unwind-tables
-fno-ident
-fmerge-all-constants
-fno-stack-protector
-fno-math-errno
-Wl,--gc-sections
-Wl,--strip-all
-Wl,--discard-all
```

New verification:

- ELF32 little-endian shared object.
- Machine: MIPS.
- Flags: `0x70001007`.
- Size: `2254084` bytes.
- `NEEDED`: empty.
- Required exports are present.
- Undefined dynamic symbols: `75`.
- `exit` and `abort` are resolved inside `r36sx_shim.c` to avoid terminating
  the `rkgame` process from Fuse error paths.
- Microsoft Defender found no threats in the rebuilt `.so`.
- SHA256: `64ED74EDA1873D7B0E5A3BDCFFB3EF4B85CB38A22DA5054E65E6F32B8E4BA179`

Attempting to embed static `libc.a` / `libm.a` failed because the SDK static
libraries contain non-PIC MIPS relocations that cannot be linked into a shared
object.

## Loading Guard Rebuild

The minimal no-`NEEDED` build from `disk_image_patch_013` hung on the stock
launcher's `Loading` screen. The likely culprit is Fuse's libretro main loop:
`retro_run()` keeps executing Z80/events until `some_audio` becomes true. If the
device frontend does not provide the expected audio batch path, the first frame
can never return.

Patch file:

```text
homebrew/fuse_zx48/r36sx_loading_guard.patch
```

The patch changes:

- `retro_set_audio_sample()` now stores the single-sample callback instead of
  discarding it.
- `sound_lowlevel_frame()` uses audio batch when available, falls back to the
  single-sample callback, and still marks `some_audio` if audio output is not
  configured.
- `retro_run()` has a 1024-iteration guard around the `while (!some_audio)`
  loop.
- `retro_get_system_av_info()` reports 320x240 before Fuse has initialized its
  geometry.
- `render_video()` returns early if video callbacks or geometry are not ready.

The build reused `build-r36sx-oz` and rebuilt only:

```text
internet_sources/fuse-libretro/src/libretro.c
internet_sources/fuse-libretro/src/compat/sound.c
```

The final link command kept the same minimal no-`NEEDED` model:

```text
zig cc -target mipsel-linux-gnu -march=mips32r2 -shared -nostdlib
  -Wl,--gc-sections -Wl,--strip-all -Wl,--discard-all
  -Wl,-soname,libemu_fuse.so
  -Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T
  -o homebrew/fuse_zx48/libemu_fuse.so
  <all build-r36sx-oz/*.o>
```

New verification:

- ELF32 little-endian shared object.
- Machine: MIPS.
- Flags: `0x70001007`.
- Size: `2254248` bytes.
- `NEEDED`: empty.
- Required exports are present, including `retro_load_game`, `retro_run`,
  `retro_set_audio_sample`, `retro_set_audio_sample_batch`,
  `retro_get_system_av_info`, `check_encrypty`, and `CheckEncrypty`.
- Undefined dynamic symbols: `75`.
- Microsoft Defender found no threats in `homebrew/fuse_zx48/libemu_fuse.so`
  and `disk_image_patch_015/cubegm/cores/libemu_fuse.so`.
- SHA256: `FAE075F3F90E5B5E459455AA5082DB1EB849A11B31A252B86DEAFA6D2655B837`

## Runtime Config

`libemu_fuse.so.cfg` pins the machine to `Spectrum 48K`, enables tape auto-load and fast-load, and maps joypad buttons to common Spectrum keys:

```text
D-pad: O/P/Q/A
A: Space
B: Enter
X: Symbol Shift
Y: Caps Shift
L: Enter
R: Space
```

The config deliberately includes all `fuse_joypad_*` options because the `rkgame` environment callback does not provide RetroArch-style default core options.

## Test Content

`R36SX_ZX48.tap` is a tiny generated TAP with an autostart BASIC program:

```basic
10 PRINT "R36SX ZX48 OK"
```
