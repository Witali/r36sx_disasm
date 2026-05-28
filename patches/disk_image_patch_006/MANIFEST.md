# disk_image_patch_006

Date: 2026-05-27

Purpose: re-enable input after Patch005 successfully displayed the static frame.

## Changed files

```text
cubegm/cores/libemu_buttondemo.so
cubegm/cores/filelist.xml
GB/button.gb
GB/filelist.csv
```

## Runtime behavior

This build keeps the successful `button.gb` launch route and re-enables input callbacks:

- D-pad moves the square.
- Select centers the square.
- Start toggles checker background.
- A/B/X/Y change square color.

Audio callback is still disabled for now.

## Verification

SHA256:

```text
12E52A942BA90688E2DFA6EF0B641C0F09B5FE8EFBD169569EB37B41D81F1861
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
