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
buttons.

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

This native build uses `/mnt/sdcard/cubegm/driver.so` for display and controls.
It plays a short generated button sound through `/dev/auddec`, following the
same packet model that `hcprojector` uses for its UI WAV effects.
