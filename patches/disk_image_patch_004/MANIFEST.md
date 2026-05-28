# disk_image_patch_004

Date: 2026-05-27

Purpose: test a more conservative `libemu_buttondemo.so` after the device briefly black-screened and returned to the Game Boy launcher.

## Changed file

```text
cubegm/cores/libemu_buttondemo.so
```

## Runtime behavior

This build intentionally avoids calling input and audio callbacks from `retro_run()`.

It only:

- draws the static 320x240 RGB565 test frame;
- calls `video_cb(frame, 320, 240, 640)`;
- keeps `check_encrypty()` and `CheckEncrypty()` returning success.

If this stays on screen, the previous return-to-launcher behavior was likely caused by input/audio callback interaction. If it still returns to launcher, the issue is likely earlier: launcher contract, ROM path/core mapping, `retro_load_game`, or video geometry/pixel format expectations.

## Build command

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

## Verification

SHA256:

```text
B9279DFB25510373527466A8287AA51A9CE56B547BB63A0DFAED70FDFF092493
```

ELF header:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008; flags=0x70001007
```

String scan:

```text
No __ubsan/GLIBC/libc/ld-linux runtime dependency strings found.
```

Defender scan:

```text
found no threats
```
