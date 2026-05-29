# disk_image_patch_061

Tiny MC built-in text viewer.

## Purpose

This patch adds a built-in text viewer to Tiny MC. Files whose extensions are
listed in `MIPS_NATIVE/tiny_mc/tiny_mc.conf` open inside Tiny MC instead of
being executed.

Default configured extensions:

```text
text_extensions=txt,log,conf
```

## Controls

Inside the text viewer:

```text
UP/DOWN      scroll one line
SELECT       page up
A or START   page down
LEFT or B    return to the file list
```

Matching files are shown in the file list with a `[TXT]` prefix.

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
patches\disk_image_patch_061\MIPS_NATIVE\tiny_mc\tiny_mc
```

The updated runtime config was copied to:

```text
disk_image\MIPS_NATIVE\tiny_mc\tiny_mc.conf
patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc.conf
patches\disk_image_patch_061\MIPS_NATIVE\tiny_mc\tiny_mc.conf
```

## Verification

```text
Tiny MC size: 63864 bytes
Tiny MC SHA256:
  B448F38D3962508167EFC9D897D85A2875B7BACE8E5D7E2BA5F0EFDE0AA3710E
Contains strings: text_extensions, text viewer, [TXT], VIEW, PGUP, PGDN,
TRUNCATED

tiny_mc.conf size: 629 bytes
tiny_mc.conf SHA256:
  C1CCD97FF41FCC4A9E66F923FECB05F969443CF9B21FBF29A768308E54218CE7

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
Defender scan patches\disk_image_patch_061: found no threats
```
