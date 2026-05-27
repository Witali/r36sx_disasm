# disk_image_patch_007

Date: 2026-05-27

Purpose: add an on-screen scrolling button log to the working Button Demo core.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/filelist.xml
GB/button.gb
GB/filelist.csv
```

## Runtime behavior

- The square remains on screen.
- D-pad still moves the square.
- Select centers the square.
- Start toggles checker background.
- A/B/X/Y change square color.
- New button presses are drawn in the lower part of the screen.
- Older button names scroll upward as new ones are added.

Audio callback is still disabled.

## Verification

SHA256:

```text
2AB1C612DB2A495B1CB82C753A92497A993CE793C985A0745372B9838E6A76DA
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
