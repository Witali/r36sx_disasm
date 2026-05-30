# DOSBox R36SX Port

This directory is the first R36SX/SF3000 native port attempt for official
DOSBox 0.74-3.

## Source

The active source copy is:

```text
homebrew/dosbox_r36sx/dosbox-0.74-3
```

It was copied from the already downloaded and scanned official DOSBox release:

```text
internet_sources/dosbox_official/dosbox-0.74-3.real.tar.gz
```

The original upstream project is:

```text
https://sourceforge.net/projects/dosbox/
```

## Current Port Strategy

The first milestone is a conservative MIPS Linux executable that links against
the SDL 1.2 runtime already present in the device firmware:

```text
disk_image/cubegm/usr/lib/libSDL-1.2.so.0
```

This is intentionally less invasive than writing a full custom DOSBox frontend
immediately. The current build keeps the stock SDL frontend, but additionally
mirrors each `GFX_EndUpdate()` surface to the firmware `driver.so` display path
as a 640x480 RGB565 frame. This is the same final presentation route used by
Pico-286 and the native apps, and avoids relying on the firmware SDL video
backend to make the frame visible.

The R36SX build also forces DOSBox away from 8bpp paletted SDL output and into
RGB output before the `driver.so` mirror. On the device, the palette path can
show only the blinking VGA cursor while normal shell text stays invisible. The
first 30 mirrored frames are summarized in `dosbox.stderr.log` with surface
size, bpp, non-black pixel count, and bounding box. The conversion path uses
direct SDL pixel-format mask extraction, including a fast pass-through for
RGB565 surfaces, instead of calling `SDL_GetRGB()` for every pixel.

Next steps are:

- route input through the same raw `cube_ioctl()`/joy shared-memory path used by
  Pico-286 and native apps;
- route audio through `sound_driver_playframe()` if SDL audio is unreliable;
- reuse `homebrew/common/r36sx_screen_keyboard.*`;
- reuse or adapt the Pico-286 disk menu, key preset menu, screenshot, and stats
  overlays;
- add a device-friendly default `dosbox.conf`;
- package the binary under `MIPS_NATIVE/dosbox_r36sx`.

## Build

Build from PowerShell:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

The script uses Zig/Clang to emit MIPS little-endian Linux code and links with
the same Buildroot sysroot used by the other native applications. The target
CPU flag is currently `-march=mips32r2`; Zig/Clang does not accept GCC's
`-mtune=74kc` spelling for this target.

The generated binary path is:

```text
homebrew/dosbox_r36sx/dosbox_r36sx
```

The R36SX wrapper `r36sx_port/r36sx_main.cpp` makes direct launcher execution
more predictable. It sets `LD_LIBRARY_PATH`, changes into
`/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx` when that directory exists, injects
`-conf dosbox.conf` when the caller did not pass a config, and writes stdout and
stderr logs next to the binary.

## Initial Config

`r36sx_port/config.h` disables the risky or unnecessary first-pass features:

- no OpenGL;
- no screenshots through DOSBox's own libpng path;
- no IPX or modem networking;
- no x86 dynamic core;
- no dynrec core;
- no x86 FPU assembly;
- no unaligned memory access.

The first build targets the portable interpreter CPU cores. This should be
slower than DOSBox dynrec, but it is the safest baseline before device-specific
optimizations.
