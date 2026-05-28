# disk_image_patch_008

Date: 2026-05-27

Purpose: diagnose physical X/Y/A/B button mapping.

## Why

On device, physical X and A both appeared as `A`, while physical Y and B both appeared as `B`.

This build scans joypad IDs `0..31` instead of only the standard libretro IDs `0..11`. New presses are logged as raw IDs:

```text
ID08 A
ID00 B
ID09 X
ID01 Y
```

If physical X and A both show `ID08 A`, and physical Y and B both show `ID00 B`, then `rkgame` has already collapsed those buttons before the core sees input. If X/Y show some other IDs, we can add a corrected mapping.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/filelist.xml
GB/button.gb
GB/filelist.csv
```

## Verification

SHA256:

```text
B0BE2570EB749689CC29198BE3A63AC02C91582091ECD7AEEB7C0A6C22412075
```

ELF header:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008; flags=0x70001007
```

String scan:

```text
No __ubsan/GLIBC/libc/ld-linux runtime dependency strings found.
```

Defender scan:

```text
found no threats
```
