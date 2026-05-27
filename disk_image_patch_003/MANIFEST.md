# disk_image_patch_003

Date: 2026-05-27

Purpose: fix the `loading` hang seen on device after Patch001/Patch002.

## Likely root cause

Patch002 was built by Zig/Clang in a mode that left unresolved UBSan runtime symbols in the shared object:

```text
__ubsan_handle_type_mismatch_v1
__ubsan_handle_pointer_overflow
__ubsan_handle_sub_overflow
__ubsan_handle_add_overflow
__ubsan_handle_load_invalid_value
__ubsan_handle_shift_out_of_bounds
__ubsan_handle_out_of_bounds
__ubsan_handle_divrem_overflow
```

The device firmware is unlikely to provide the UBSan runtime, so `rkgame` may hang or fail while loading the `.so`.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/config.xml
cubegm/cores/filelist.xml
GB/button.demo
GB/filelist.csv
```

Copy the contents of `disk_image_patch_003` into the SD/image root so these paths merge with the existing root paths. Do not copy the patch directory itself as a new top-level folder.

## Source/build changes

Source file:

```text
homebrew/libretro_button_demo/button_demo.c
```

Changes:

- Removed the early `RETRO_ENVIRONMENT_SET_PIXEL_FORMAT` call from `retro_set_environment()` to stay closer to stock `libemu_tgbdual.so` behavior.
- Kept `check_encrypty()` and `CheckEncrypty()` returning success.
- Kept silent 735-frame audio batches.

Build command:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

## Verification

SHA256:

```text
37F348DD99255CDB81F6EC6B06F519C64A509FFCAEC51715864611BCDB557BC4
```

ELF header:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008; flags=0x70001007
```

Meaning:

```text
ELF32, little-endian, shared object, MIPS, same e_flags as libemu_tgbdual.so
```

String scan:

```text
No __ubsan symbols found.
No GLIBC/libc dependency strings found.
```

Defender scan:

```text
found no threats
```
