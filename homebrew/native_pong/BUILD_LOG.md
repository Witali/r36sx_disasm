# Build log: native Pong

## 2026-05-28 initial Tiny MC standalone build

Purpose:

Provide a Pong executable that can be launched directly from Tiny MC through
`fork()` + `execl()`.

Background:

The earlier Pong implementation at `homebrew/libretro_pong/libemu_pong.so` is a
libretro-style shared object for the stock `rkgame` loader. Tiny MC launches
normal executables, so this build rewrites Pong as a standalone MIPS executable
with a `main()`.

Implementation:

- Uses `/mnt/sdcard/cubegm/driver.so` with `video_driver_setting()`,
  `video_drivers_init()`, and `video_driver_disp_frame()`.
- Reads built-in controls through `cube_ioctl()` and the `/tmp/joy_key` pointer.
- Runs a 640x480 RGB565 Pong playfield with one human paddle and an AI/manual
  right paddle.
- Sleeps for `16666` microseconds per frame so the main loop yields to Linux.
- Select or Fn exits back to Tiny MC.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

Patch directory:

```text
disk_image_patch_035
```

Patch files:

```text
disk_image_patch_035\MIPS_NATIVE\pong\pong
disk_image_patch_035\MIPS_NATIVE\pong\README.txt
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Contains strings: /mnt/sdcard/cubegm/driver.so, cube_ioctl
Does not contain string: retro_run
Size: 14088 bytes
SHA256: E2BB7EF51806085202B92B97837079E67BB4174997BD4DFF3AC18F694EAADBFF

Defender scan homebrew\native_pong\pong: found no threats
Defender scan disk_image_patch_035\MIPS_NATIVE\pong\pong: found no threats
```

## 2026-05-28 win/lose message rebuild

Purpose:

Add explicit English result messages to native Pong.

Implementation:

- Added a small built-in 5x7 pixel font for the required uppercase letters.
- Added game-over state when either side reaches score `9`.
- Shows `YOU WIN` when the left/player score reaches `9`.
- Shows `YOU LOSE` when the right/opponent score reaches `9`.
- Pressing A or Start after the result starts a new game.
- Select or Fn still exits back to Tiny MC.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

Patch directory:

```text
disk_image_patch_041
```

Patch files:

```text
disk_image_patch_041\MIPS_NATIVE\pong\pong
disk_image_patch_041\MIPS_NATIVE\pong\README.txt
```

Verification:

```text
Contains strings: YOU WIN, YOU LOSE, /mnt/sdcard/cubegm/driver.so, cube_ioctl
Size: 16540 bytes
SHA256: 9C9FBA430AFB47B1685BEC13B524CC877C6BB4CE301816AAA33619C30AB8C650
Defender scan homebrew\native_pong\pong: found no threats
Defender scan disk_image_patch_041\MIPS_NATIVE\pong\pong: found no threats
```
