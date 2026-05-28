# disk_image_patch_049

## Purpose

Rebuild Tiny MC with optional TrueType text rendering through the stock
`cubegm/lib/libfreetype.so.6` library. The patch bundles JetBrains Mono NL
Regular v2.304 under OFL-1.1. If FreeType or the TTF file is unavailable,
Tiny MC falls back to its old built-in 5x7 bitmap font.

## Files To Copy

Copy these files over the original disk image paths:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
MIPS_NATIVE\tiny_mc\README.txt
MIPS_NATIVE\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
MIPS_NATIVE\tiny_mc\fonts\JetBrainsMono-OFL.txt
```

This patch assumes the existing Tiny MC boot route from earlier patches is
already installed:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

## Build And Download Commands

From repository root:

```powershell
New-Item -ItemType Directory -Force .\homebrew\tiny_mc\fonts
Invoke-WebRequest -Uri https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/fonts/ttf/JetBrainsMonoNL-Regular.ttf -OutFile .\homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
Invoke-WebRequest -Uri https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/OFL.txt -OutFile .\homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt
.\tools\scan-download.ps1 .\homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
.\tools\scan-download.ps1 .\homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

## Runtime Paths

Tiny MC tries these FreeType paths first:

```text
/mnt/sdcard/cubegm/lib/libfreetype.so.6
/mnt/sdcard/cubegm/lib/libfreetype.so
libfreetype.so.6
```

Preferred font:

```text
/mnt/sdcard/MIPS_NATIVE/tiny_mc/fonts/JetBrainsMonoNL-Regular.ttf
```

## Verification

```text
MIPS_NATIVE\tiny_mc\tiny_mc
Size: 46712 bytes
SHA256: 67560BCC8983DDE6306238CD19204881D90BF82E697E8FBCE85ACA8932E4040A

MIPS_NATIVE\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
Size: 208576 bytes
SHA256: FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A

MIPS_NATIVE\tiny_mc\fonts\JetBrainsMono-OFL.txt
Size: 4399 bytes
SHA256: 60D55F23C6CE05A81099A762CB67CA2C9B6EA251C7912720998B4C89EBFD4FAA

Contains strings: libfreetype.so.6, FT_Init_FreeType, FT_New_Face,
FT_Load_Char, JetBrainsMonoNL-Regular.ttf
Still contains dynamic dependencies: libdl.so.2, libc.so.6

Defender scan homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf: found no threats
Defender scan homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt: found no threats
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf: found no threats
Defender scan patches\disk_image_patch_049: found no threats
```
