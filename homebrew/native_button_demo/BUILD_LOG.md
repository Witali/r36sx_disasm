# Build log: native Button Demo

## 2026-05-28 initial Tiny MC standalone build

Purpose:

Move Button Demo into `MIPS_NATIVE` so Tiny MC can launch it directly as a
normal MIPS executable.

Background:

The existing Button Demo at
`homebrew/libretro_button_demo/libemu_buttondemo.so` is a libretro-style shared
object for the stock `rkgame` loader. Tiny MC launches ordinary executables, so
this build ports the useful visual/input behavior into a standalone program
with `main()`.

Implementation:

- Uses `/mnt/sdcard/cubegm/driver.so` with `video_driver_setting()`,
  `video_drivers_init()`, and `video_driver_disp_frame()`.
- Reads built-in controls through `cube_ioctl()` and the `/tmp/joy_key`
  pointer.
- Draws a 640x480 RGB565 screen with the original colored bars, crosshair,
  movable square, and bottom button log.
- D-pad moves the square.
- A/B/X/Y change square color.
- Start toggles checkerboard background.
- Select resets the square position.
- Fn exits back to Tiny MC.
- Sleeps for `16666` microseconds per frame so the main loop yields to Linux.

Native audio is intentionally not included in this first move. The older sound
path used libretro audio callbacks from `rkgame`; Tiny MC does not provide that
callback interface to normal child processes.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
disk_image_patch_043
```

Patch files:

```text
disk_image_patch_043\MIPS_NATIVE\button_demo\button_demo
disk_image_patch_043\MIPS_NATIVE\button_demo\README.txt
```

Verification:

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
