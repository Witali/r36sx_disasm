# disk_image_patch_009

Date: 2026-05-27

Purpose: add a small gap between on-screen button log lines.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/filelist.xml
GB/button.gb
GB/filelist.csv
```

## Runtime behavior

Same raw input diagnostic build as Patch008, but the button log now uses a 16-pixel line step for 14-pixel-tall text, leaving a 2-pixel gap between rows.

## Verification

SHA256:

```text
8D1ECEC9D40E8BC3823F8F85467EC8B35D7959B646EA87626AF693631A003A7F
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
