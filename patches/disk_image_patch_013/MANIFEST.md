# disk_image_patch_013

Minimal-size, no-`NEEDED` rebuild of the experimental Fuse ZX Spectrum 48K core.

## Why

Device tests of the previous Fuse builds still returned to the stock launcher.
This rebuild moves back toward the dependency model used by the first
homebrew Button Demo: no explicit dynamic library dependencies in the ELF
`NEEDED` list.

The goal is to reduce the loader surface:

- no `libm.so.6` `NEEDED` entry;
- no `libgcc_s.so.1` `NEEDED` entry;
- no `libc.so.6` `NEEDED` entry;
- smaller shared object;
- keep the R36SX `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT` compatibility patch;
- add local `exit()` / `abort()` traps so Fuse cannot terminate the whole
  `rkgame` process through those libc calls.

## Build Model

The core was rebuilt from all `fuse-libretro` C sources with size-oriented
flags:

```text
-Oz
-fomit-frame-pointer
-fPIC
-ffunction-sections
-fdata-sections
-fno-unwind-tables
-fno-asynchronous-unwind-tables
-fno-ident
-fmerge-all-constants
-fno-stack-protector
-fno-math-errno
-D__LIBRETRO__
-DHAVE_CONFIG_H
-DNDEBUG
```

Link flags:

```text
-shared
-nostdlib
-Wl,--gc-sections
-Wl,--strip-all
-Wl,--discard-all
-Wl,-soname,libemu_fuse.so
-Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T
```

No runtime libraries were passed to the linker.

## Important Limitation

`NEEDED` is empty, but the shared object still has unresolved standard C/POSIX
symbols such as `malloc`, `free`, `memcpy`, `fopen`, `fread`, `open`, `read`,
and `write`.

That means this build has no explicit dynamic dependencies, but it is not a
fully self-contained binary. It still depends on the already-running `rkgame`
process / dynamic loader making those symbols available.

The attempted static-libc build was rejected by the linker because the SDK
`libc.a` and `libm.a` contain non-PIC MIPS relocations that cannot be embedded
into a shared object.

## Changed/added files

- `cubegm/cores/libemu_fuse.so`
- `cubegm/cores/libemu_fuse.so.cfg`
- `cubegm/cores/config.xml`
- `cubegm/cores/filelist.xml`
- `ATARI/filelist.csv`
- `ATARI/R36SX_ZX48.tap`

## Verification

- `libemu_fuse.so` is ELF32 little-endian MIPS.
- ELF flags: `0x70001007`.
- Size: `2254084` bytes.
- `NEEDED`: empty.
- Required exported symbols are present:
  - `retro_get_system_av_info`
  - `retro_load_game`
  - `retro_run`
  - `retro_set_environment`
  - `retro_set_video_refresh`
  - `check_encrypty`
  - `CheckEncrypty`
- Undefined dynamic symbols: `75`.
- `exit` and `abort` are resolved inside `r36sx_shim.c`.
- Microsoft Defender scan: no threats found.
- `libemu_fuse.so` SHA256: `64ED74EDA1873D7B0E5A3BDCFFB3EF4B85CB38A22DA5054E65E6F32B8E4BA179`

## Install

Copy the contents of this patch directory over the root of the SD card/image.
This patch is intended to replace the previous ZX Spectrum test files from
`disk_image_patch_011` or `disk_image_patch_012`.

The test entry remains:

```text
ATARI -> R36SX ZX48 Test
```
