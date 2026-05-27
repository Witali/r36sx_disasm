# Fuse ZX Spectrum 48K core

Experimental MIPS32 little-endian build of `libretro/fuse-libretro` for the `cubegm/rkgame` launcher.

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

## Verification

- ELF32 little-endian shared object.
- Machine: MIPS.
- Flags: `0x70001007`.
- Exported symbols include `retro_get_system_av_info`, `retro_load_game`, `retro_run`, `retro_set_environment`, `retro_set_video_refresh`, `check_encrypty`, and `CheckEncrypty`.
- `NEEDED` list is empty because this build uses `-nostdlib`; standard libc/libm symbols are expected to resolve from the `rkgame` process at `dlopen` time.
- Microsoft Defender found no threats in the built `.so`.
- SHA256: `BEBCFF5B456A10C6888ED20F5AACE173CC60B8668D037F98E2FD9411EAFB62A5`

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
