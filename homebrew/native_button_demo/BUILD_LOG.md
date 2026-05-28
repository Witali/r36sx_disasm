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

## 2026-05-28 Arial FreeType button log rebuild

Purpose:

Render the scrolling pressed-button log with the stock firmware TrueType Arial
font and remove the static instruction labels from the top of the screen.

Implementation:

- Added a small dynamic FreeType loader to `native_button_demo.c`.
- The program tries `/mnt/sdcard/cubegm/lib/libfreetype.so.6`, then
  `/mnt/sdcard/cubegm/usr/lib/libfreetype.so.6`, then `libfreetype.so.6`.
- The preferred font is `/mnt/sdcard/cubegm/Arial_en.ttf`, with
  `/mnt/sdcard/cubegm/Arial_kr.ttf` and `/mnt/sdcard/cubegm/font.ttf` as
  fallbacks.
- The old built-in bitmap text path remains as a fallback if FreeType or Arial
  cannot be opened.
- Removed the top `BUTTON DEMO` and control-help strings from `draw_frame()`.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
patches\disk_image_patch_053
```

Patch files:

```text
patches\disk_image_patch_053\MIPS_NATIVE\button_demo\button_demo
patches\disk_image_patch_053\MIPS_NATIVE\button_demo\README.txt
```

Verification:

```text
Contains strings: /mnt/sdcard/cubegm/Arial_en.ttf, libfreetype.so.6,
FT_Init_FreeType, sound_driver_playframe
Does not contain strings: BUTTON DEMO, DPAD MOVE
Size: 19400 bytes
SHA256: 17010FEB9EC4C674DB0E79482B61C2FA7E2D57142AAB38038BB4E7DC916FA2A4
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_053: found no threats
```

## 2026-05-28 B/Y color mapping rebuild

Purpose:

Swap the face-button color mapping requested during device testing:

```text
B button: yellow
Y button: green
```

Implementation:

- `native_button_demo.c` now maps `BTN_B_BIT` to square color index `4`
  (`0xffe0`, yellow).
- `native_button_demo.c` now maps `BTN_Y_BIT` to square color index `2`
  (`0x07e0`, green).

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
patches\disk_image_patch_054
```

Patch files:

```text
patches\disk_image_patch_054\MIPS_NATIVE\button_demo\button_demo
patches\disk_image_patch_054\MIPS_NATIVE\button_demo\README.txt
```

Verification:

```text
Size: 19400 bytes
SHA256: 0D2FE9A4A80C8CE58EFD13F8226FE42A65CC5CBC2081A879DFCB74B78C1EE2E5
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_054: found no threats
```

## 2026-05-28 rear trigger mask rebuild

Purpose:

Find why the console rear triggers were not logged in Button Demo, then add the
missing raw key masks.

Reverse-engineering note:

- `rkgame` `ReadJoystickProc()` reads `/tmp/joy_key` through
  `driver.so` `cube_ioctl(0x40050209)`: `KEY_VALUE_Addr[0]` becomes
  `joy_key`, and `KEY_VALUE_Addr[1]` becomes the second joy word.
- `driver.so` maps `/tmp/joy_key` as 8 bytes, so both words are available to
  native programs.
- The `rkgame` `joy_key_mask` table at virtual address `0x004f3548` maps
  extra libretro joypad IDs:

```text
L  -> 0x00000400
R  -> 0x00000800
L2 -> 0x00000100
R2 -> 0x00000200
L3 -> 0x00000002
R3 -> 0x00000004
```

Implementation:

- Added these masks to `homebrew/common/hardware.h`.
- Button Demo now translates and logs `L`, `R`, `L2`, `R2`, `L3`, and `R3`.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
patches\disk_image_patch_055
```

Patch files:

```text
patches\disk_image_patch_055\MIPS_NATIVE\button_demo\button_demo
patches\disk_image_patch_055\MIPS_NATIVE\button_demo\README.txt
```

Verification:

```text
Contains strings: L2, R2, L3, R3, FT_Init_FreeType, Arial_en,
sound_driver_playframe
Size: 19816 bytes
SHA256: A602363BDA172E20BB0FBE70E83F68AB37D5D040661FA1249CCEE1C237D40A8A
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_055: found no threats
```

## 2026-05-28 preserve mixer volume rebuild

Purpose:

Avoid audible volume jumps when native Button Demo initializes audio through
`driver.so`. Reverse-engineering showed that vendor `sound_driver_init()`
reapplies the saved AV volume to `/dev/sndC0i2so`, so the native helper now
reads the current mixer volume through `cube_ioctl(0x40010204)` before audio
init and restores it through `cube_ioctl(0x20010203)` afterward.

Implementation:

- Added `r36sx_driver_audio_bind_preserve_volume()` to
  `homebrew/common/driver_audio.h`.
- Button Demo now passes its resolved `cube_ioctl` symbol to that helper.

Build command from repository root:

```powershell
.\homebrew\native_button_demo\build_native_button_demo.ps1
```

Patch directory:

```text
patches\disk_image_patch_056
```

Verification:

```text
Size: 19928 bytes
SHA256: 348B37F0F4ACCC0CEC54F396EA5B94EE5E655181DA419CD8D7D01A0299E9D59F
Contains strings: sound_driver_init, sound_driver_playframe
Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan patches\disk_image_patch_056: found no threats
```
