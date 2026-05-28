# disk_image_patch_002

Date: 2026-05-24

Purpose: rebuild `libemu_buttondemo.so` with compatibility fixes found while comparing it to `libemu_tgbdual.so`.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
```

## Source changes

Source file:

```text
homebrew/libretro_button_demo/button_demo.c
```

Changes:

- `check_encrypty()` now returns `1`, matching the stock core's "continue loading" condition.
- Added `CheckEncrypty(const void *)`, also returning `1`, for compatibility with the upper-case stock export.
- `retro_set_audio_sample_batch()` now stores the callback.
- `retro_run()` sends a silent 735-frame stereo audio batch when the callback exists.
- `retro_get_system_av_info()` now reports `44100.0` Hz sample rate, closer to `libemu_tgbdual.so`.

## Build command

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

## Verification

SHA256:

```text
57EDDED0F60023B0473E7E5CC36AD5E635A1CF95A8C7EE4AC2EC2035126FAA91
```

ELF header:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008
```

Meaning:

```text
ELF32, little-endian, shared object, MIPS
```

Important symbols found:

```text
retro_run
retro_load_game
retro_set_environment
retro_set_video_refresh
retro_set_audio_sample_batch
retro_set_input_poll
retro_set_input_state
SetFrameSkip
check_encrypty
CheckEncrypty
```

Defender scan:

```text
found no threats
```

## Applied to working disk image

The rebuilt file was copied to:

```text
disk_image/cubegm/cores/libemu_buttondemo.so
```
