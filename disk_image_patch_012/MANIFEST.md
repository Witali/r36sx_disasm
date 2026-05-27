# disk_image_patch_012

Compatibility rebuild for the experimental Fuse ZX Spectrum 48K core.

## Why

The first device test of `disk_image_patch_011` showed a black screen for a
short moment, then returned to the launcher.

Two likely compatibility problems were found:

- `fuse-libretro` returned `false` from `retro_load_game()` if the frontend did
  not accept `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT`.
- The previous `libemu_fuse.so` had an empty `NEEDED` list because it was linked
  with `-nostdlib`, unlike stock cores such as `libemu_tgbdual.so`.

## Changes

- Patched `src/libretro.c` so the core continues with RGB565 when
  `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT` is ignored by `rkgame`.
- Relinked `libemu_fuse.so` with explicit dynamic dependencies:
  - `libm.so.6`
  - `libgcc_s.so.1`
  - `libc.so.6`
- Kept the same `check_encrypty` / `CheckEncrypty` compatibility shim.
- Kept the same temporary ATARI menu test entry and Spectrum 48K config.

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
- `NEEDED`: `libm.so.6`, `libgcc_s.so.1`, `libc.so.6`.
- Required exported symbols are present:
  - `retro_get_system_av_info`
  - `retro_load_game`
  - `retro_run`
  - `retro_set_environment`
  - `retro_set_video_refresh`
  - `check_encrypty`
  - `CheckEncrypty`
- Microsoft Defender scan: no threats found.
- `libemu_fuse.so` SHA256: `29DEE42C31586965415A57CE69A17E19DB0AD3D813656CFEDCF2EAA9AE9AE10D`

## Install

Copy the contents of this patch directory over the root of the SD card/image.
This patch is intended to replace the previous ZX Spectrum test files from
`disk_image_patch_011`.

The test entry remains:

```text
ATARI -> R36SX ZX48 Test
```
