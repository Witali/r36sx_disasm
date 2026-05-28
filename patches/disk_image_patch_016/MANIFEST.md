# disk_image_patch_016: Pong Demo core

Date: 2026-05-27

Purpose:

Add a tiny Pong-style game module built on the same working libretro-style
integration shell as Button Demo.

## Files

Copy these files over the SD-card/image root:

```text
cubegm\cores\libemu_pong.so
cubegm\cores\config.xml
cubegm\cores\filelist.xml
GB\filelist.csv
GB\pong.pong
```

## Integration route

The launcher is still tested through the existing `GB` menu path:

```xml
<file name="GB/pong.pong" core="libemu_pong.so" />
```

The new core entry is:

```xml
<core>
<emucore name="Pong Demo" file="libemu_pong.so" />
<supported_extensions>PONG</supported_extensions>
</core>
```

## Controls

```text
D-pad Up/Down: left paddle
A/X: right paddle up
B/Y: right paddle down
No right-paddle input: simple AI
Start: pause/resume
Select: reset match
```

## Build

Command from repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

## Verification

```text
ELF32 little-endian DYN(shared object), machine=MIPS, flags=0x70001007
Size: 41496 bytes
SHA256: B4DFD05CAD16A365BBF062CBB124BC73BAC67CC0BCE1130C33C575D15A989EE0
No __ubsan/GLIBC/libc/ld-linux/memset dependency strings found.
Zig objdump search found no UND/undefined/NEEDED/memset/__ubsan/GLIBC lines.
Defender scan of homebrew SO: found no threats.
Defender scan of disk_image_patch_016: found no threats.
```
