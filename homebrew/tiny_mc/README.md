# Tiny MC

Tiny MC is a small one-panel framebuffer file manager / launcher for the
R36SX/SF3000-like firmware.

The current integration model launches Tiny MC directly from the confirmed boot
route, `cubegm/icube.sh`, without starting the stock `icube` supervisor. Tiny
MC is installed as `MIPS_NATIVE/tiny_mc/tiny_mc` and receives
`/mnt/sdcard/MIPS_NATIVE` as its start directory argument.

## Controls

- D-pad / left stick up and down: select a file.
- Left, B, or Select: parent directory.
- Right: enter the selected directory.
- A or Start: enter a directory or run the selected file.
- Fn: close Tiny MC and `execl()` the stock `/mnt/sdcard/cubegm/icube`
  supervisor directly.

Directories with more entries than fit on screen show a thin scrollbar on the
right side of the file list.

Tiny MC remembers the selected entry and scroll offset for recently visited
directories. When you return to a directory, it restores the previous position
by filename first and by index as fallback.

## Runtime model

The program uses:

- `cubegm/driver.so` for the normal display path, via `dlopen()`,
  `video_driver_setting()`, `video_drivers_init()`, and
  `video_driver_disp_frame()`;
- `cubegm/driver.so` for short button-click audio, via the same
  `sound_driver_init()` / `sound_driver_playframe()` path used by stock
  `rkgame` LibRetro audio;
- `cubegm/lib/libfreetype.so.6` for TrueType text rendering when available,
  loaded with `dlopen()` so the built-in 5x7 bitmap font remains a fallback;
- `MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf` as the preferred
  FreeType font, with the other common mono fonts as fallbacks;
- `MIPS_NATIVE/tiny_mc/tiny_mc.conf` for the runtime font path and text-size
  settings;
- an internal RGB565 640x480 framebuffer;
- `/dev/fb0` direct drawing only as fallback if `driver.so` is unavailable;
- `/dev/input/js0..js3` for Linux joystick input;
- `/dev/input/event0..event9` as an evdev fallback;
- `fork()` and `execl()` to run selected files.

No SDL, DirectFB, ncurses, or Midnight Commander runtime is required.

The current direct-launch build sets `USE_ICUBE_HEARTBEAT 0`, so Tiny MC does
not attach the old `icube` SysV shared memory heartbeat and does not depend on
the `icube` watchdog contract. The older icube-compatible build used SysV
shared memory key `0x4d2`, size `0x1c4`, and updated `shm[0]=1` / `shm[1]++`.

The `driver.so` path is important on the real device: stock `rkgame` relies on
that library to configure the framebuffer, rotation/scaling, blank/unblank
state, HCGE, and render/painter threads. Tiny MC closes its display and input
devices before launching a selected executable, so the child process can acquire
the vendor driver cleanly.

## Debug log

Logging is controlled at compile time by the `DEBUG` macro at the top of
`tiny_mc.c`. With `#define DEBUG 1`, Tiny MC appends a debug log to the first
writable path:

```text
/mnt/sdcard/cubegm/tiny_mc.log
/mnt/sdcard/tiny_mc.log
tiny_mc.log
```

It logs startup, display initialization, input device opens, directory scans,
button state changes, launched executables, and child exit status. The render
loop does not log every frame. Set `#define DEBUG 0` and rebuild to disable log
file creation and log writes.

## Runtime config

Tiny MC reads this optional config file at startup:

```text
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc.conf
```

The format is simple `key=value`, with `#` comments. Current keys:

```text
font=/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf
small_px=12
large_px=19
list_row_h=22
```

`font` can point at any TTF file readable by FreeType. `small_px` controls the
header/path/footer text, `large_px` controls the file-list text, and
`list_row_h` controls the file-list row height. If `list_row_h` is omitted,
Tiny MC keeps it at least `large_px + 3`.

## Input model

Built-in controls are read through the same path used by stock `rkgame`: Tiny
MC resolves `cube_ioctl` from `cubegm/driver.so`, obtains the `/tmp/joy_key`
shared memory pointer with command `0x40050209`, and polls the game/status
register with command `0x40050208`.

The known `rkgame` masks are:

```text
SELECT=0x1 START=0x8 UP=0x10 RIGHT=0x20 DOWN=0x40 LEFT=0x80
L2=0x100 R2=0x200 L=0x400 R=0x800
X=0x1000 A=0x2000 B=0x4000 Y=0x8000 FN=0x10000
L3=0x2 R3=0x4
```

`/dev/input/js*` and `/dev/input/event*` remain as fallback sources.

## Build

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

The output file is:

```text
homebrew\tiny_mc\tiny_mc
```

For the current SD-card integration, copy the rebuilt executable as:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
MIPS_NATIVE\tiny_mc\tiny_mc.conf
MIPS_NATIVE\common\fonts\
```

restore the stock binary as:

```text
cubegm\rkgame.stock
cubegm\rkgame
```

and route `cubegm\icube.sh` directly to:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

This bypasses `icube`; if Tiny MC hangs, there is no `icube` supervisor restart.
Fn handling is enabled in Tiny MC. The shortcut is armed only after Fn has been
released once, so a raised Fn bit during startup is ignored. A later Fn press
closes Tiny MC and starts `/mnt/sdcard/cubegm/icube` directly, not
`icube.sh`, to avoid looping back into Tiny MC.

## Native Program Folder

Standalone MIPS programs launched directly by Tiny MC should live under:

```text
/mnt/sdcard/MIPS_NATIVE
```

Recommended layout:

```text
MIPS_NATIVE/
  program_name/
    program_name
    README.txt
    data/
```

Each program gets its own directory named after the program. Tiny MC runs
executable files directly with `fork()` + `execl()`, and runs `.sh` files
through `/bin/sh`.
