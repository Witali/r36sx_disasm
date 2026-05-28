# disk_image_patch_048

## Purpose

Rebuild Pong with large game-over text. `You win` and `You lose` now render
using a 5x5 variable-width pixel font scaled to roughly match the score digits.

## Files To Copy

Copy these files over the original disk image paths:

```text
cubegm\cores\libemu_pong.so
MIPS_NATIVE\pong\pong
```

## Source

```text
homebrew\pong\pong.c
```

Target selection:

```text
R36SX_PONG_TARGET=1  libretro core for rkgame
R36SX_PONG_TARGET=2  native Tiny MC executable using driver.so
```

## Build Commands

Libretro core:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -DR36SX_PONG_TARGET=1 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\pong\pong.c
```

Native executable:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

## Verification

```text
homebrew\libretro_pong\libemu_pong.so
Size: 39460 bytes
SHA256: BEE64253C023BFDDC5D539808F8257477AAABB874D1350225881970C1238FE1F
Defender scan: found no threats

homebrew\native_pong\pong
Size: 19188 bytes
SHA256: A84DA563B8DE044F0CB686CC11FE7983985B232733AC9A72BA80A15872E1B2DF
Defender scan: found no threats

disk_image\cubegm\cores\libemu_pong.so
SHA256: BEE64253C023BFDDC5D539808F8257477AAABB874D1350225881970C1238FE1F
Defender scan: found no threats

disk_image\MIPS_NATIVE\pong\pong
SHA256: A84DA563B8DE044F0CB686CC11FE7983985B232733AC9A72BA80A15872E1B2DF
Defender scan: found no threats

patches\disk_image_patch_048
Defender scan: found no threats
```
