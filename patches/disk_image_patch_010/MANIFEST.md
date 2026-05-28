# disk_image_patch_010

Native-resolution button demo test build.

## Changed files

- `cubegm/cores/libemu_buttondemo.so`

## Purpose

- Rebuilds the Button Demo libretro core with a 640x480 RGB565 framebuffer.
- Keeps the moving square and button log.
- Adds a 1-pixel white border and 1-pixel center crosshair to make scaling easier to see on the device.

## Build command

From `homebrew/libretro_button_demo`:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path ..\..\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path ..\..\tools\zig-cache).Path
..\..\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o libemu_buttondemo.so button_demo.c
```

## Verification

- ELF32 little-endian shared object, MIPS.
- Exported symbols include `retro_run`, `retro_get_system_av_info`, `retro_set_video_refresh`, `check_encrypty`, and `CheckEncrypty`.
- Defender scan: found no threats.
- SHA256: `5B6EBEC0430295A7952E19963F3B592092CA6A803DB58AAA7BD7A98435BFB6A6`
