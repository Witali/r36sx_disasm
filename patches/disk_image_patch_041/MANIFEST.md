# Patch 041: Pong win/lose messages

Date: 2026-05-28

## Purpose

Update the standalone native Pong executable launched from Tiny MC so it shows
English end-of-game messages.

## Files

Copy these files over the matching paths on the SD card:

```text
MIPS_NATIVE/pong/pong
MIPS_NATIVE/pong/README.txt
```

## Behavior

- The left/player side wins at score `9` and the screen shows `YOU WIN`.
- If the right/opponent side reaches score `9`, the screen shows `YOU LOSE`.
- Press A or Start after a result to start a new game.
- Select or Fn still exits back to Tiny MC.

## Build

Built from the repository root with:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

## Verification

```text
Contains strings: YOU WIN, YOU LOSE, /mnt/sdcard/cubegm/driver.so, cube_ioctl
Size: 16540 bytes
SHA256: 9C9FBA430AFB47B1685BEC13B524CC877C6BB4CE301816AAA33619C30AB8C650
Defender scan homebrew\native_pong\pong: found no threats
Defender scan disk_image_patch_041\MIPS_NATIVE\pong\pong: found no threats
```
