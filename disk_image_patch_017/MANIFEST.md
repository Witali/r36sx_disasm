# disk_image_patch_017: Pong `.gb` launch compatibility patch

Date: 2026-05-27

Purpose:

The first Pong device test from `disk_image_patch_016` briefly showed a black
screen and returned to the launcher. That matches the earlier Button Demo
failure mode where custom extensions were less reliable. This patch adds a
stock Game Boy extension launch file and keeps an explicit core override.

## Files

Copy these files over the SD-card/image root:

```text
cubegm\cores\libemu_pong.so
cubegm\cores\config.xml
cubegm\cores\filelist.xml
GB\filelist.csv
GB\pong.gb
GB\pong.pong
```

## Integration Route

Use this entry from the Game Boy list first:

```text
Pong Demo GB
```

The new primary override is:

```xml
<file name="GB/pong.gb" core="libemu_pong.so" />
```

The older custom-extension route remains as a secondary diagnostic entry:

```xml
<file name="GB/pong.pong" core="libemu_pong.so" />
```

## Build

Command from repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Source compatibility change:

```text
valid_extensions = "gb|pong"
```

## Verification

```text
libemu_pong.so size: 41512 bytes
libemu_pong.so SHA256: CDA9C881BA3A40AA3984B8B4274508022F64C743341C690EEAA61F2BCE63AC55
GB\pong.gb SHA256: A6946699DA8EDCAA153BDA223166CC4766FD480D6E872B62EF66FDA6C7D29FC6
GB\pong.pong SHA256: A6946699DA8EDCAA153BDA223166CC4766FD480D6E872B62EF66FDA6C7D29FC6
Required symbol strings include retro_run, retro_load_game,
retro_set_video_refresh, retro_set_audio_sample,
retro_set_audio_sample_batch, check_encrypty, CheckEncrypty.
No NEEDED/UND/__ubsan/GLIBC/ld-linux/libc.so/libm.so/memset strings found.
Defender scan of homebrew SO: found no threats.
Defender scan of disk_image_patch_017: found no threats.
```
