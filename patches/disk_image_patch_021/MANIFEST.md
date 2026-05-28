# disk_image_patch_021: Tiny MC rkgame replacement

Date: 2026-05-28

Purpose:

Add a small one-panel MC-like file manager that can replace `cubegm\rkgame`.
Before installing this patch, preserve the original stock launcher as
`cubegm\rkgame.stock`, so the new manager can launch it like any other
executable file.

## Files

```text
cubegm/rkgame
cubegm/icubemp_start.sh
```

## Behavior

- `cubegm/rkgame` is now Tiny MC.
- `cubegm/rkgame.stock` should be the original stock game launcher, created by
  renaming the existing file before copying this patch.
- `icubemp_start.sh` now kills `rkgame`, `rkgame.stock`, `tiny_mc`, and
  `icube` before returning to `/usr/bin/hcprojector`.

## Controls

- D-pad / left stick up and down: select a file.
- Left, B, or Select: parent directory.
- Right: enter a directory.
- A or Start: enter a directory or run the selected file.

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

The final build uses `zig clang` plus explicit `zig ld.lld` linking against:

```text
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/mipsel-buildroot-linux-gnu/sysroot
```

## Verification

Tiny MC binary:

```text
ELF32 little-endian executable
Machine: MIPS
Interpreter: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, GLIBC_2.0
Size: 28824 bytes
SHA256: 44189734CE2ABA6653DF8A22AA4E9CF7FA7CBA590779E03969EF0CDE4D0EB2C6
Defender scan: found no threats
```

## Install

First, on the SD card/image, rename the stock launcher:

```text
cubegm/rkgame -> cubegm/rkgame.stock
```

Then copy this patch directory over the SD card/image root, preserving paths.

After copying, the important files should be:

```text
cubegm/rkgame        <- Tiny MC
cubegm/rkgame.stock  <- original stock launcher created before patch copy
```

To start the stock launcher from Tiny MC, navigate to:

```text
/mnt/sdcard/cubegm/rkgame.stock
```

and press `A` or `Start`.

## Known limitations

- This is the first direct-input launcher build outside `rkgame`. If the device
  exposes its built-in controls differently from `/dev/input/js*` or
  `/dev/input/event*`, the UI may draw but not respond yet.
- It launches executable files and shell scripts. It does not yet pass selected
  ROM files into the stock `rkgame` core loader.
