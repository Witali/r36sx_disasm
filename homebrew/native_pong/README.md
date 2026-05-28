# Native Pong

Standalone MIPS32 little-endian Pong executable for Tiny MC.

Unlike `homebrew/libretro_pong/libemu_pong.so`, this program is not a libretro
core. It has a normal `main()` and Tiny MC can launch it directly with
`fork()` + `execl()`.

## Controls

- D-pad up/down: left paddle.
- A/X: right paddle up.
- B/Y: right paddle down.
- Start: pause.
- A while paused: reset.
- A or Start after `YOU WIN` / `YOU LOSE`: new game.
- Select or Fn: exit back to Tiny MC.

If the right paddle is not controlled manually, a small built-in AI follows the
ball. The left player wins when the left score reaches 9. If the right side
reaches 9 first, the game shows `YOU LOSE`.

## Build

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

The output file is:

```text
homebrew\native_pong\pong
```

For Tiny MC, copy it to:

```text
MIPS_NATIVE\pong\pong
```

The program uses `/mnt/sdcard/cubegm/driver.so` for display and controls. It
plays generated bounce, score, pause, win, and lose sounds through `/dev/auddec`,
following the same packet model that `hcprojector` uses for its UI WAV effects.
It returns to Tiny MC when it exits.
