# disk_image_patch_035

Date: 2026-05-28

Purpose:

Add a standalone native Pong executable for launching directly from Tiny MC.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/pong/pong
MIPS_NATIVE/pong/README.txt
```

## Behavior

`MIPS_NATIVE/pong/pong` is a normal MIPS executable with `main()`. It is not the
older libretro `libemu_pong.so` core. Tiny MC launches it with `fork()` +
`execl()`, waits for it to exit, and then reopens Tiny MC display/input.

The native Pong executable:

- loads `/mnt/sdcard/cubegm/driver.so`;
- presents a 640x480 RGB565 framebuffer through `video_driver_disp_frame()`;
- reads built-in controls through `cube_ioctl()` and `/tmp/joy_key`;
- sleeps `16666` microseconds per frame so the loop yields to Linux;
- exits back to Tiny MC on Select or Fn.

## Controls

```text
D-pad up/down: left paddle
A/X: right paddle up
B/Y: right paddle down
Start: pause
A while paused: reset
Select or Fn: exit back to Tiny MC
```

The right paddle is AI-controlled unless A/X or B/Y is held.

## Verification

```text
MIPS_NATIVE/pong/pong
  size: 14088 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains strings: /mnt/sdcard/cubegm/driver.so, cube_ioctl
  does not contain string: retro_run
  SHA256: E2BB7EF51806085202B92B97837079E67BB4174997BD4DFF3AC18F694EAADBFF
  Defender: found no threats
```
