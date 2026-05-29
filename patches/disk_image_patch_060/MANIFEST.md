# disk_image_patch_060

Tiny MC row-spacing rebuild.

## Purpose

This patch changes the Tiny MC file-list layout so extra row height above the
configured `large_px` text size is split around the text instead of being left
entirely below it. For example, with the current config:

```text
large_px=19
list_row_h=22
```

the extra `3` pixels are applied as `1` pixel above the text and `2` pixels
below the text.

## Files

Copy these files over the original disk contents:

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/tiny_mc.conf
```

## Build

Command run from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

The rebuilt executable was copied to:

```text
disk_image\MIPS_NATIVE\tiny_mc\tiny_mc
patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc
patches\disk_image_patch_060\MIPS_NATIVE\tiny_mc\tiny_mc
```

The updated runtime config was copied to:

```text
disk_image\MIPS_NATIVE\tiny_mc\tiny_mc.conf
patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc.conf
patches\disk_image_patch_060\MIPS_NATIVE\tiny_mc\tiny_mc.conf
```

## Verification

```text
Tiny MC size: 56892 bytes
Tiny MC SHA256:
  7357C4376E545BCBE6AB3CE150EA6D5915C3EDF1E5BAA8D8C4981B5D19ACE6AA

tiny_mc.conf size: 507 bytes
tiny_mc.conf SHA256:
  DE24AE69C7255CE3E770BD80F42A7D79EF7EDDBBF3AE899068A6F9101D3B9803

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
Defender scan patches\disk_image_patch_060: found no threats
```
