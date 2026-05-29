# disk_image_patch_062

Tiny MC selection marker alignment fix.

## Purpose

This patch fixes a one-pixel vertical alignment issue in the Tiny MC file list:
the orange marker at the left edge of the selected row is now aligned from the
row padding instead of the text baseline.

## Files

Copy these files over the original disk contents:

```text
MIPS_NATIVE/tiny_mc/tiny_mc
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
patches\disk_image_patch_062\MIPS_NATIVE\tiny_mc\tiny_mc
```

## Verification

```text
Tiny MC size: 64888 bytes
Tiny MC SHA256:
  4FF4C45964101461FD795CF21565A7E3D2FB55307D88FD9C0C8F5BABFB5587A1

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
Defender scan patches\disk_image_patch_062\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```
