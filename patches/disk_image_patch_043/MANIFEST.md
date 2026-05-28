# Patch 043: native Button Demo for Tiny MC

Date: 2026-05-28

## Purpose

Move Button Demo into `MIPS_NATIVE` as a standalone executable that Tiny MC can
launch directly.

## Files

Copy these files over the matching paths on the SD card:

```text
MIPS_NATIVE/button_demo/button_demo
MIPS_NATIVE/button_demo/README.txt
```

## Behavior

- D-pad moves the square.
- A, B, X, and Y change square color.
- Start toggles the checkerboard background.
- Select resets the square position.
- Fn exits back to Tiny MC.
- The bottom of the screen shows a scrolling log of newly pressed buttons.

## Notes

The old `homebrew/libretro_button_demo/libemu_buttondemo.so` remains available
for stock `rkgame`. This patch adds a separate native executable because Tiny
MC launches normal MIPS executables with `fork()` + `execl()`.

Native audio is not included in this first move. The previous sound path used
libretro audio callbacks from `rkgame`; Tiny MC does not provide those callbacks
to ordinary child processes.

## Build

Built from the repository root with:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

## Verification

```text
Contains strings: BUTTON DEMO, PRESS BUTTONS, FN EXIT,
/mnt/sdcard/cubegm/driver.so, cube_ioctl
Does not contain string: retro_run
ELF: class=1, data=1, type=2, machine=8, interpreter /lib/ld.so.1
Size: 15568 bytes
SHA256: AB6C3B948F689172306336317E71A64F641E89E2C1316734AA75596D5EB71D79
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan disk_image_patch_043\MIPS_NATIVE\button_demo\button_demo: found no threats
```
