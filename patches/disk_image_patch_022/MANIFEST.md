# disk_image_patch_022: Tiny MC driver.so display path

Date: 2026-05-28

Purpose:

Rebuild Tiny MC after device testing showed a white screen for a few seconds,
then a black screen with only the battery overlay. Decompilation showed that
stock `rkgame` does not draw directly to `/dev/fb0`; it loads `cubegm/driver.so`
and lets that library configure the display, rotation, framebuffer blanking,
HCGE, and render/painter threads.

## Files

```text
cubegm/rkgame
cubegm/icubemp_start.sh
```

## Behavior

- `cubegm/rkgame` is Tiny MC rebuilt to use `driver.so` first.
- Tiny MC calls `video_driver_setting`, `video_drivers_init`, and presents each
  RGB565 640x480 frame through `video_driver_disp_frame`.
- Direct `/dev/fb0` rendering remains as fallback if `driver.so` cannot be
  loaded.
- Before launching a selected executable, Tiny MC deinitializes its display and
  closes input devices so the launched program can acquire the vendor display
  driver itself.
- `cubegm/rkgame.stock` should still be the original stock game launcher,
  created by renaming the existing file before copying the Tiny MC patch.

## Build

Source:

```text
homebrew/tiny_mc/tiny_mc.c
homebrew/tiny_mc/build_tiny_mc.ps1
```

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

The build links against the local Buildroot SDK sysroot and now includes
`-ldl`, because Tiny MC uses `dlopen()`/`dlsym()` to load `driver.so`.

## Verification

Tiny MC binary:

```text
ELF32 little-endian executable
Machine: MIPS
Interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Size: 32592 bytes
SHA256: A6B1755B19D5882C11D6203D73CA30B9DE64A8742C60168347C53E3623C37B41
Defender scan: found no threats
```

Commands run:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
rg -a -o "ld\.so\.1|libc\.so\.6|libdl\.so\.[0-9]+|GLIBC_[0-9]+\.[0-9]+|driver\.so|video_driver_[a-z_]+" .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
Copy-Item -LiteralPath .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image_patch_022\cubegm\rkgame -Force
Copy-Item -LiteralPath .\disk_image\cubegm\icubemp_start.sh -Destination .\disk_image_patch_022\cubegm\icubemp_start.sh -Force
Copy-Item -LiteralPath .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image\cubegm\rkgame -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_022\cubegm\rkgame
Get-FileHash -Algorithm SHA256 .\disk_image_patch_022\cubegm\rkgame
```

## Install

If this is the first Tiny MC install, first rename the stock launcher on the SD
card/image:

```text
cubegm/rkgame -> cubegm/rkgame.stock
```

Then copy this patch directory over the SD card/image root, preserving paths.

After copying, the important files should be:

```text
cubegm/rkgame        <- Tiny MC
cubegm/rkgame.stock  <- original stock launcher
```

## Device-test expectation

This patch targets the display-initialization miss found in the first Tiny MC
device test. If the UI now appears but controls do not respond, the next likely
area is input: stock `rkgame` reads buttons through `driver.so` `cube_ioctl`
and shared state, not only through plain Linux joystick/event devices.
