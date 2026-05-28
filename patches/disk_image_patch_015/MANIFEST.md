# disk_image_patch_015

Fuse ZX Spectrum 48K rebuild with a guard against the stock launcher staying on
the `Loading` screen.

## Why

`disk_image_patch_013` loaded far enough to show the stock `Loading` overlay, but
did not return to the launcher or reach the Spectrum display.

The strongest local suspect is the upstream Fuse libretro loop in
`retro_run()`:

```text
do {
   input_poll_cb();
   z80_do_opcodes();
   event_do_events();
} while (!some_audio);
```

On RetroArch this is meant to keep audio/video pacing stable. On the stock
`rkgame` frontend, if the expected audio batch path does not become active, the
first `retro_run()` call can spin forever and the launcher remains stuck on
`Loading`.

## Source Patch

Stored as:

```text
homebrew/fuse_zx48/r36sx_loading_guard.patch
```

The patch:

- stores `retro_set_audio_sample()` as a fallback callback;
- makes `sound_lowlevel_frame()` tolerate missing audio batch callbacks;
- marks `some_audio` even when no audio callback is available;
- bounds the `retro_run()` wait-for-audio loop to 1024 iterations;
- returns a safe default 320x240 AV geometry before Fuse initializes;
- skips video submit if geometry/callbacks are not ready.

## Build

Only these objects were rebuilt from the minimal `build-r36sx-oz` build:

```text
internet_sources/fuse-libretro/src/libretro.c
internet_sources/fuse-libretro/src/compat/sound.c
```

The module was relinked with the same no-`NEEDED` model:

```text
zig cc -target mipsel-linux-gnu -march=mips32r2 -shared -nostdlib
  -Wl,--gc-sections -Wl,--strip-all -Wl,--discard-all
  -Wl,-soname,libemu_fuse.so
  -Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T
  -o homebrew/fuse_zx48/libemu_fuse.so
  <all build-r36sx-oz/*.o>
```

## Changed/Added Files

- `cubegm/cores/libemu_fuse.so`
- `cubegm/cores/libemu_fuse.so.cfg`
- `cubegm/cores/config.xml`
- `cubegm/cores/filelist.xml`
- `ATARI/filelist.csv`
- `ATARI/R36SX_ZX48.tap`

## Verification

- `libemu_fuse.so` is ELF32 little-endian MIPS.
- ELF flags: `0x70001007`.
- Size: `2254248` bytes.
- `NEEDED`: empty.
- Required exported symbols are present:
  - `retro_get_system_av_info`
  - `retro_load_game`
  - `retro_run`
  - `retro_set_audio_sample`
  - `retro_set_audio_sample_batch`
  - `check_encrypty`
  - `CheckEncrypty`
- Undefined dynamic symbols: `75`.
- Microsoft Defender scan: no threats found.
- `libemu_fuse.so` SHA256:
  `FAE075F3F90E5B5E459455AA5082DB1EB849A11B31A252B86DEAFA6D2655B837`

## Install

Copy the contents of this patch directory over the root of the SD card/image.
This patch is intended to replace the previous ZX Spectrum test files from
`disk_image_patch_011`, `disk_image_patch_012`, or `disk_image_patch_013`.

The test entry remains:

```text
ATARI -> R36SX ZX48 Test
```
