# Native Button Demo

Standalone MIPS32 little-endian Button Demo executable for Tiny MC.

Unlike `homebrew/libretro_button_demo/libemu_buttondemo.so`, this program is
not a libretro core. It has a normal `main()` and Tiny MC can launch it
directly with `fork()` + `execl()`.

## Controls

- D-pad: move the square.
- A, B, X, Y: change square color.
- Start: toggle checkerboard background.
- Select: reset square position.
- Fn: exit back to Tiny MC.

The lower part of the screen shows a small scrolling log of newly pressed
buttons. The log is rendered with FreeType using the stock firmware Arial font
at `/mnt/sdcard/cubegm/Arial_en.ttf`; if FreeType or the font is unavailable,
the program falls back to its built-in bitmap font.

## Build

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
```

The output file is:

```text
homebrew\native_button_demo\button_demo
```

For Tiny MC, copy it to:

```text
MIPS_NATIVE\button_demo\button_demo
```

This native build uses `/mnt/sdcard/cubegm/driver.so` for display, controls,
and audio. It plays a short generated button sound through
`sound_driver_playframe`, the same driver-level path used by stock LibRetro
cores in `rkgame`.
