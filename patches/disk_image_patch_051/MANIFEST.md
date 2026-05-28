# disk_image_patch_051

## Purpose

Rebuild Tiny MC with FreeType UI text about 20 percent larger.

Changes:

```text
TINY_FT_SMALL_PX: 10 -> 12
TINY_FT_LARGE_PX: 16 -> 19
FreeType baseline offsets: 9/15 -> 11/18
LIST_ROW_H: 18 -> 22
```

The row height was increased so the larger file-list text has enough vertical
space.

## Files To Copy

Copy this file over the original disk image path:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
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
Size: 46916 bytes
SHA256: 845CE784ECDE9241C509C67F305895C43A997326BF070F783AC632261CD84A09

Contains strings: MIPS_NATIVE/common/fonts, libfreetype.so.6,
JetBrainsMonoNL-Regular.ttf, LiberationMono-Regular.ttf, RobotoMono-Regular.ttf

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_051: found no threats
```
