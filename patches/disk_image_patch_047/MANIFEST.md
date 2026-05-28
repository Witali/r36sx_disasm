# disk_image_patch_047

Purpose: rebuild both Pong variants from a single source file,
`homebrew/pong/pong.c`, and apply the native-app exit rule to native Pong.

Apply by copying this directory over the root of the SD card / disk image.

## Files

```text
cubegm/cores/libemu_pong.so
MIPS_NATIVE/pong/pong
MIPS_NATIVE/pong/README.txt
```

## Source Layout

```text
homebrew/pong/pong.c
```

Targets:

```text
R36SX_PONG_TARGET=1  libretro core for rkgame
R36SX_PONG_TARGET=2  native Tiny MC executable using driver.so
```

Native Pong now exits when `Select + Start` are held together. `Fn` remains a
secondary hardware escape.

## Build Commands

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -DR36SX_PONG_TARGET=1 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\pong\pong.c
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

## Verification

```text
libemu_pong.so: ELF32 little-endian DYN, machine=MIPS
libemu_pong.so size: 36436 bytes
libemu_pong.so SHA256: F8A114C8D5CE9A34DF06521A2FA69D611176DDB901FCBE52E3290C18E811AC7D
libemu_pong.so strings include: retro_run, check_encrypty, CheckEncrypty, YOU WIN, YOU LOSE
libemu_pong.so strings do not include: libc.so, ld-linux, __ubsan, /dev/auddec

native pong: ELF32 little-endian EXEC, machine=MIPS
native pong size: 17844 bytes
native pong SHA256: C0B64C219BFDD16ACE0E41E945931AB4E04CC61F21F230B60ED06690AD701FF9
native pong strings include: main, /mnt/sdcard/cubegm/driver.so, sound_driver_playframe, YOU WIN, YOU LOSE
native pong strings do not include: /dev/auddec

Defender scan homebrew\libretro_pong\libemu_pong.so: found no threats
Defender scan homebrew\native_pong\pong: found no threats
Defender scan disk_image\cubegm\cores\libemu_pong.so: found no threats
Defender scan disk_image\MIPS_NATIVE\pong\pong: found no threats
Defender scan patches\disk_image_patch_047: found no threats
```
