# disk_image_patch_050

## Purpose

Move Tiny MC's bundled TrueType fonts to the shared native-app directory:

```text
MIPS_NATIVE\common\fonts
```

This patch also adds additional free monospaced fonts:

```text
LiberationMono-Regular.ttf
DejaVuSansMono.ttf
NotoSansMono-Regular.ttf
SourceCodePro-Regular.ttf
Hack-Regular.ttf
RobotoMono-Regular.ttf
```

Tiny MC was rebuilt to search `MIPS_NATIVE/common/fonts` first. The old
`MIPS_NATIVE/tiny_mc/fonts` path remains as a fallback for compatibility.

## Files To Copy

Copy these files over the original disk image paths:

```text
MIPS_NATIVE\tiny_mc\tiny_mc
MIPS_NATIVE\tiny_mc\README.txt
MIPS_NATIVE\common\fonts\
```

This patch assumes the Tiny MC boot route from earlier patches is already
installed.

## Download Sources

```text
JetBrains Mono:
https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/fonts/ttf/JetBrainsMonoNL-Regular.ttf

Liberation Fonts:
https://github.com/liberationfonts/liberation-fonts/files/7261482/liberation-fonts-ttf-2.1.5.tar.gz

DejaVu Fonts:
https://github.com/dejavu-fonts/dejavu-fonts/releases/download/version_2_37/dejavu-fonts-ttf-2.37.zip

Noto Sans Mono:
https://raw.githubusercontent.com/notofonts/noto-fonts/main/hinted/ttf/NotoSansMono/NotoSansMono-Regular.ttf

Source Code Pro:
https://raw.githubusercontent.com/adobe-fonts/source-code-pro/release/TTF/SourceCodePro-Regular.ttf

Hack:
https://github.com/source-foundry/Hack/releases/download/v3.003/Hack-v3.003-ttf.zip

Roboto Mono:
https://raw.githubusercontent.com/googlefonts/RobotoMono/main/fonts/ttf/RobotoMono-Regular.ttf
```

## Verification

```text
MIPS_NATIVE\tiny_mc\tiny_mc
Size: 46900 bytes
SHA256: DFC466EFEBFA07E4A0E8DEC6A0C4BCA3B18E5A1B028B5F8664633E169FD88D07

MIPS_NATIVE\common\fonts\JetBrainsMonoNL-Regular.ttf
Size: 208576 bytes
SHA256: FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A

MIPS_NATIVE\common\fonts\LiberationMono-Regular.ttf
Size: 319508 bytes
SHA256: F2B83C763E8AFD21709333370BED4774337FAE82267937E2B5AEA7E2FBD922C1

MIPS_NATIVE\common\fonts\DejaVuSansMono.ttf
Size: 340712 bytes
SHA256: B4A6C3E4FAAB8773F4FF761D56451646409F29ABEDD68F05D38C2DF667D3C582

MIPS_NATIVE\common\fonts\NotoSansMono-Regular.ttf
Size: 512836 bytes
SHA256: D9E2B23D19F8230BE7146F409A52B1D23117E635E28F2E2892CF91B7382F325B

MIPS_NATIVE\common\fonts\SourceCodePro-Regular.ttf
Size: 210312 bytes
SHA256: 74BD80D3E42A08517CD7E1108BA3D86F2DA29AC0F3065BE95E0357956AB9DB37

MIPS_NATIVE\common\fonts\Hack-Regular.ttf
Size: 309408 bytes
SHA256: 15F55CC0C85A2988D2B4B3A8CDB5D77FDFBAF319E1BB5309D725DB9818FB7125

MIPS_NATIVE\common\fonts\RobotoMono-Regular.ttf
Size: 125748 bytes
SHA256: AF0BFF7599C3DF3831755C16E39B3C496DF74B8C8D8A1161B14DC8461BE17CB4

MIPS_NATIVE\common\fonts\README.md
Size: 2796 bytes
SHA256: 6F1042BB337FB27FEF841F5DB370B6878AB24B0139C53BC497DA6C008DE79ACB

Contains strings: MIPS_NATIVE/common/fonts, LiberationMono-Regular.ttf,
DejaVuSansMono.ttf, NotoSansMono-Regular.ttf, SourceCodePro-Regular.ttf,
Hack-Regular.ttf, RobotoMono-Regular.ttf, libfreetype.so.6

Defender scan downloaded archives/raw files: found no threats
Defender scan homebrew\common\fonts: found no threats
Defender scan disk_image\MIPS_NATIVE\common\fonts: found no threats
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_050: found no threats
```
