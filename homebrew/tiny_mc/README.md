# Tiny MC

Tiny MC is a small one-panel framebuffer file manager / launcher for the
R36SX/SF3000-like firmware.

The current integration model keeps the stock `icube` supervisor in the launch
chain. `icube` hardcodes `execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0)`, so
Tiny MC is installed both as `MIPS_NATIVE/tiny_mc/tiny_mc` and as the
`cubegm/rkgame` compatibility entrypoint. With no start directory argument,
Tiny MC opens `/mnt/sdcard/MIPS_NATIVE`.

## Controls

- D-pad / left stick up and down: select a file.
- Left, B, or Select: parent directory.
- Right: enter the selected directory.
- A or Start: enter a directory or run the selected file.

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
- an internal RGB565 640x480 framebuffer;
- `/dev/fb0` direct drawing only as fallback if `driver.so` is unavailable;
- `/dev/input/js0..js3` for Linux joystick input;
- `/dev/input/event0..event9` as an evdev fallback;
- `fork()` and `execl()` to run selected files.

No SDL, DirectFB, ncurses, or Midnight Commander runtime is required.

Tiny MC also mirrors the stock `rkgame` heartbeat expected by the `icube`
supervisor. It attaches SysV shared memory key `0x4d2`, size `0x1c4`, and
updates `shm[0]=1` / `shm[1]++` from the main loop. Without this heartbeat,
`icube` treats `rkgame` as hung and restarts it after roughly 5-6 seconds.

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

## Input model

Built-in controls are read through the same path used by stock `rkgame`: Tiny
MC resolves `cube_ioctl` from `cubegm/driver.so`, obtains the `/tmp/joy_key`
shared memory pointer with command `0x40050209`, and polls the game/status
register with command `0x40050208`.

The known `rkgame` masks are:

```text
SELECT=0x1 START=0x8 UP=0x10 RIGHT=0x20 DOWN=0x40 LEFT=0x80
X=0x1000 A=0x2000 B=0x4000 Y=0x8000 FN=0x10000
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

For the current SD-card integration, copy the rebuilt executable as both:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
cubegm\rkgame
```

preserving the stock binary as:

```text
cubegm\rkgame.stock
```

and keep `cubegm\icube_start.sh` launching the stock supervisor:

```sh
/mnt/sdcard/cubegm/icube &
```

`icube` passes no arguments to `rkgame`, so Tiny MC chooses
`/mnt/sdcard/MIPS_NATIVE` as its default start directory.

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
