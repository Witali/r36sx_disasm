# Common Monospaced Fonts

These fonts are copied to the SD-card path:

```text
MIPS_NATIVE\common\fonts
```

Tiny MC prefers `JetBrainsMonoNL-Regular.ttf` and falls back through the other
regular mono fonts if the preferred file is missing. License files are kept next
to the TTF files so each font can be redistributed as part of a patch overlay.

## Files

| Font | File | License File | Size | SHA256 |
| --- | --- | --- | ---: | --- |
| JetBrains Mono NL Regular 2.304 | `JetBrainsMonoNL-Regular.ttf` | `JetBrainsMono-OFL.txt` | 208576 | `FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A` |
| Liberation Mono Regular 2.1.5 | `LiberationMono-Regular.ttf` | `Liberation-OFL.txt` | 319508 | `F2B83C763E8AFD21709333370BED4774337FAE82267937E2B5AEA7E2FBD922C1` |
| DejaVu Sans Mono 2.37 | `DejaVuSansMono.ttf` | `DejaVu-LICENSE.txt` | 340712 | `B4A6C3E4FAAB8773F4FF761D56451646409F29ABEDD68F05D38C2DF667D3C582` |
| Noto Sans Mono Regular | `NotoSansMono-Regular.ttf` | `Noto-OFL.txt` | 512836 | `D9E2B23D19F8230BE7146F409A52B1D23117E635E28F2E2892CF91B7382F325B` |
| Source Code Pro Regular | `SourceCodePro-Regular.ttf` | `SourceCodePro-OFL.md` | 210312 | `74BD80D3E42A08517CD7E1108BA3D86F2DA29AC0F3065BE95E0357956AB9DB37` |
| Hack Regular 3.003 | `Hack-Regular.ttf` | `Hack-LICENSE.md` | 309408 | `15F55CC0C85A2988D2B4B3A8CDB5D77FDFBAF319E1BB5309D725DB9818FB7125` |
| Roboto Mono Regular | `RobotoMono-Regular.ttf` | `RobotoMono-OFL.txt` | 125748 | `AF0BFF7599C3DF3831755C16E39B3C496DF74B8C8D8A1161B14DC8461BE17CB4` |

License file hashes:

```text
DejaVu-LICENSE.txt       F88D5294AF5A772F9114EB385009E3478D62B21F3E5BBC34D159D328047C8867
Hack-LICENSE.md          1F61BB7C790C59B4B0ECDF304628B94E42AE4C8020094A8C3DA73381AB212623
JetBrainsMono-OFL.txt    60D55F23C6CE05A81099A762CB67CA2C9B6EA251C7912720998B4C89EBFD4FAA
Liberation-OFL.txt       A0C060F758D33EE59A674CD7B45914A560D0A5935968AF5E87B0C6981FD89DDC
Noto-OFL.txt             0DAB92D0544F7B233403F14B84A663BDBFA746982EDA629E7F4F9FFE1B036FEB
RobotoMono-OFL.txt       B6CAB49AED2CCFB5F63D27FD252EB0226A4BC882528BA5FDCCCD593883B9BF55
SourceCodePro-OFL.md     F0B1039AC64B39993C4BC05DD6C68EC7136381C941A4DD60485E1D0E9ACB8DCC
```

## Sources

- JetBrains Mono: https://github.com/JetBrains/JetBrainsMono
- Liberation Fonts: https://github.com/liberationfonts/liberation-fonts
- DejaVu Fonts: https://dejavu-fonts.github.io/
- Noto Fonts: https://github.com/notofonts/noto-fonts
- Source Code Pro: https://github.com/adobe-fonts/source-code-pro
- Hack: https://github.com/source-foundry/Hack
- Roboto Mono: https://github.com/googlefonts/RobotoMono

Downloaded archives/raw files were scanned with `tools\scan-download.ps1`
before extraction/copying. The final `homebrew\common\fonts` directory was also
scanned after assembly.
