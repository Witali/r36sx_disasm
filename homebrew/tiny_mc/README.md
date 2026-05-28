# Tiny MC

Tiny MC is a small one-panel framebuffer file manager / launcher for the
R36SX/SF3000-like firmware.

It is meant to test replacing `cubegm/rkgame` with a standalone MIPS executable
while preserving the original launcher as `cubegm/rkgame.stock`.

## Controls

- D-pad / left stick up and down: select a file.
- Left, B, or Select: parent directory.
- Right: enter the selected directory.
- A or Start: enter a directory or run the selected file.

## Runtime model

The program uses:

- `cubegm/driver.so` for the normal display path, via `dlopen()`,
  `video_driver_setting()`, `video_drivers_init()`, and
  `video_driver_disp_frame()`;
- an internal RGB565 640x480 framebuffer;
- `/dev/fb0` direct drawing only as fallback if `driver.so` is unavailable;
- `/dev/input/js0..js3` for Linux joystick input;
- `/dev/input/event0..event9` as an evdev fallback;
- `fork()` and `execl()` to run selected files.

No SDL, DirectFB, ncurses, or Midnight Commander runtime is required.

The `driver.so` path is important on the real device: stock `rkgame` relies on
that library to configure the framebuffer, rotation/scaling, blank/unblank
state, HCGE, and render/painter threads. Tiny MC closes its display and input
devices before launching a selected executable, so the child process can acquire
the vendor driver cleanly.

## Build

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

The output file is:

```text
homebrew\tiny_mc\tiny_mc
```

For SD-card testing, copy it as:

```text
cubegm\rkgame
```

and preserve the stock binary as:

```text
cubegm\rkgame.stock
```
