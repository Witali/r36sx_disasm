# Tiny MC FreeType Font Research

Date: 2026-05-28

## Goal

Choose a freely redistributable monospaced TrueType font for Tiny MC's
FreeType renderer. The font should be readable on a 640x480 handheld screen,
small enough for an SD-card patch, and useful for Latin plus Cyrillic file
names.

## Existing Fonts In The Stock Image

The stock image already contains these TTF files:

```text
disk_image\cubegm\Arial_en.ttf                         907236 bytes
disk_image\cubegm\Arial_kr.ttf                        1100700 bytes
disk_image\cubegm\font.ttf                            6110584 bytes
disk_image\cubegm\Tahoma.ttf                           800788 bytes
disk_image\cubegm\usr\share\directfb-1.7.7\decker.ttf   37048 bytes
```

These are useful as runtime fallbacks, but their redistribution/licensing story
inside our patch is less clear than a project-shipped open font.

## Selected Font

`JetBrains Mono NL Regular` from JetBrains Mono v2.304.

Reasons:

- OFL-1.1 license;
- official JetBrains page lists Cyrillic and Russian support;
- large x-height and simple shapes are designed for small code/editor sizes;
- the `NL` variant avoids ligature-oriented behavior, keeping direct
  FreeType-per-codepoint rendering simple;
- the single TTF is modest for our patch: 208576 bytes.

Downloaded files:

```text
homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
SHA256: FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A

homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt
SHA256: 60D55F23C6CE05A81099A762CB67CA2C9B6EA251C7912720998B4C89EBFD4FAA
```

Sources:

- https://github.com/JetBrains/JetBrainsMono
- https://www.jetbrains.com/lp/mono/
- https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/fonts/ttf/JetBrainsMonoNL-Regular.ttf
- https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/OFL.txt

## Other Good Candidates

- `Liberation Mono`: OFL-1.1 in current releases, metrically compatible with
  Courier New, Latin/Greek/Cyrillic oriented, conservative and likely small.
  Source: https://github.com/liberationfonts/liberation-fonts
- `DejaVu Sans Mono`: free license, broad character coverage, proven Linux
  fallback font, but visually wider/heavier and usually larger than needed.
  Source: https://dejavu-fonts.github.io/
- `Noto Sans Mono`: OFL-1.1, excellent Google/Noto coverage, but broader
  coverage usually means larger files.
  Source: https://github.com/notofonts/noto-fonts
- `Source Code Pro`: OFL-1.1, readable code font with Cyrillic support, a
  solid alternative if JetBrains Mono looks too tall on device.
  Source: https://github.com/adobe-fonts/source-code-pro
- `Hack`: source-code font with Latin/Greek/Cyrillic coverage and good 8-14px
  readability; regular TTF is larger than JetBrains Mono NL.
  Source: https://github.com/source-foundry/Hack
- `Roboto Mono`: Apache-2.0 Google font, supports Cyrillic subsets, readable
  and familiar, but less compact visually than JetBrains Mono on small UI rows.
  Source: https://fonts.google.com/specimen/Roboto+Mono
