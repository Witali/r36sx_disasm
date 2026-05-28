# disk_image_patch_052

## Purpose

Add a Tiny MC runtime config file next to the executable and rebuild Tiny MC so
the FreeType font path and text sizes can be changed without recompiling.

Config path:

```text
MIPS_NATIVE\tiny_mc\tiny_mc.conf
```

Supported keys:

```text
font=/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf
small_px=12
large_px=19
list_row_h=22
```

`font` can point at any TTF file readable by FreeType. `small_px` controls the
header/path/footer text, `large_px` controls the file-list text, and
`list_row_h` controls the file-list row height. If `list_row_h` is omitted,
Tiny MC keeps it at least `large_px + 3`.

## Files To Copy

Copy these files over the original disk image paths:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
MIPS_NATIVE\tiny_mc\tiny_mc.conf
MIPS_NATIVE\tiny_mc\README.txt
```

This patch assumes the common font pool from `disk_image_patch_050` is already
installed under:

```text
MIPS_NATIVE\common\fonts
```

## Build Command

From repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

## Verification

```text
MIPS_NATIVE\tiny_mc\tiny_mc
Size: 54208 bytes
SHA256: FEA903CABAEFEE5CB8871512903C821A0ABC2A67248C5FC6827A9C7CEBAD16C8

MIPS_NATIVE\tiny_mc\tiny_mc.conf
Size: 452 bytes
SHA256: 06F53A12D7B4EBC88B587FABF0CB531FB601041E78D025AC0F58CECF443F5D70

MIPS_NATIVE\tiny_mc\README.txt
SHA256: 24FC02117F37648231C9F6BE829BB5BC335915E5B61A2799363BED1253980395

Contains strings: tiny_mc.conf, config loaded, font_size, small_px, large_px,
list_row_h, MIPS_NATIVE/common/fonts, libfreetype.so.6

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan homebrew\tiny_mc\tiny_mc.conf: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc.conf: found no threats
Defender scan patches\disk_image_patch_052: found no threats
```
