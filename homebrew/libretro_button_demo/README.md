# libretro button demo

Minimal libretro core for the MIPS32 little-endian `cubegm` launcher.

It draws a 640x480 RGB565 framebuffer and reacts to buttons:

- D-pad: move square
- A/B/X/Y: change square color
- Start: toggle checker background
- Select: reset square position

## Build

Requires a MIPS little-endian toolchain:

```sh
make CC=mipsel-linux-gnu-gcc
```

On the current Windows workspace, the module was built with local Zig:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path ..\..\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path ..\..\tools\zig-cache).Path
..\..\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o libemu_buttondemo.so button_demo.c
```

Expected output:

```text
libemu_buttondemo.so
```

## Install on SD image/card

Copy the shared object to:

```text
cubegm/cores/libemu_buttondemo.so
```

Add a core entry to `cubegm/cores/config.xml`:

```xml
<core>
<emucore name="Button Demo" file="libemu_buttondemo.so" />
<supported_extensions>DEMO</supported_extensions>
</core>
```

Create a platform folder and dummy ROM file:

```text
DEMO/button.demo
```

If the launcher does not show a new platform for `DEMO`, reuse an existing
platform folder for testing and add a `filelist.xml` override for one dummy
file.
