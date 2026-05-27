# libretro Pong demo

Small Pong-style libretro core for the MIPS32 little-endian `cubegm` launcher.

This is based on the proven Button Demo integration pattern:

- 640x480 RGB565 framebuffer
- no libc dependency
- libretro callback surface compatible with `rkgame`
- generated 44.1 kHz stereo square-wave sound

Controls:

- D-pad Up/Down: left paddle
- A/X: right paddle up
- B/Y: right paddle down
- no right-paddle input: simple AI controls the right paddle
- Start: pause/resume
- Select: reset match

## Build

From the repository root on the current Windows workspace:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Expected output:

```text
homebrew\libretro_pong\libemu_pong.so
```

## Install on SD image/card

Copy the shared object to:

```text
cubegm\cores\libemu_pong.so
```

Add a core entry to `cubegm\cores\config.xml`:

```xml
<core>
<emucore name="Pong Demo" file="libemu_pong.so" />
<supported_extensions>PONG</supported_extensions>
</core>
```

For the current launcher, the safest test path is to reuse the `GB` menu:

```text
GB\pong.pong
```

and add an override to `cubegm\cores\filelist.xml`:

```xml
<file name="GB/pong.pong" core="libemu_pong.so" />
```
