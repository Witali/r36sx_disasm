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

## 2026-05-28 native /dev/auddec audio rebuild

Purpose:

Restore Button Demo sound in the standalone Tiny MC executable. The original
libretro Button Demo sound went through `rkgame` audio callbacks, but the native
Tiny MC child process does not have those callbacks.

Firmware audio reference:

- `ghidra_exports/icube/decompiled_all.c` shows `icube` is only a supervisor
  for `rkgame`; it does not play audio.
- `ghidra_exports/hcprojector/decompiled_all.c` shows the stock UI sound path:
  open `/dev/auddec`, ioctl `0x82780301` for init, ioctl `0x20000304` for
  start, then write `AvPktHd` + PCM payload packets.

Implementation:

- Added `homebrew/common/native_audio.h`.
- Button Demo now plays a short generated two-part button sound when any new
  button press is detected.
- The audio helper uses generated 44.1 kHz stereo PCM and does not require
  external WAV files.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
patches\disk_image_patch_044
```

Patch files:

```text
patches\disk_image_patch_044\MIPS_NATIVE\button_demo\button_demo
patches\disk_image_patch_044\MIPS_NATIVE\button_demo\README.txt
```

Verification:

```text
Contains strings: BUTTON DEMO, /mnt/sdcard/cubegm/driver.so, /dev/auddec, cube_ioctl
Does not contain string: retro_run
ELF: class=1, data=1, type=2, machine=8, interpreter /lib/ld.so.1
Size: 17304 bytes
SHA256: CF8F8C91432DA770FBCA6BFC9C7C957C9820D351F033E38FA7BB38263CA52FA2
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_044\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
```

## 2026-05-28 driver.so audio rebuild

Purpose:

Move Button Demo sound from the direct `/dev/auddec` packet path to the stock
`rkgame` LibRetro-style path through `driver.so` `sound_driver_playframe`.

Implementation:

- Added `homebrew/common/driver_audio.h`.
- Button Demo now resolves `sound_driver_init`, `sound_driver_playframe`,
  `sound_driver_flush`, and `sound_driver_deinit` from the already loaded
  `driver.so` handle used for display and controls.
- Button press tones are still generated as 44.1 kHz stereo PCM, but are now
  passed to `driver.so` instead of opening `/dev/auddec` directly.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Build note:

The first rebuild failed with `ld.lld: error: undefined symbol: __udivdi3`
because the new helper briefly used a 64-bit divide for the tone phase step.
The helper now uses the original 32-bit calculation, avoiding extra libgcc
runtime helpers.

Patch directory:

```text
patches\disk_image_patch_046
```

Patch files:

```text
patches\disk_image_patch_046\MIPS_NATIVE\button_demo\button_demo
patches\disk_image_patch_046\MIPS_NATIVE\button_demo\README.txt
```

Verification:

```text
Contains strings: BUTTON DEMO, /mnt/sdcard/cubegm/driver.so, sound_driver_init, sound_driver_playframe, cube_ioctl
Does not contain string: /dev/auddec
Does not contain string: retro_run
ELF: class=1, data=1, type=2, machine=8, interpreter /lib/ld.so.1
Size: 16956 bytes
SHA256: 3BEB30973118B04747D992C79FBFFA2E7555BE9D793B576013B2B3C30C596D1C
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_046: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
```
