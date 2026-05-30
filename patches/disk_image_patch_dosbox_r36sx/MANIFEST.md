# DOSBox R36SX Patch Overlay

Copy the files in this patch directory over the original SD card contents.

## Files

```text
MIPS_NATIVE/dosbox_r36sx/dosbox_r36sx
MIPS_NATIVE/dosbox_r36sx/dosbox.conf
MIPS_NATIVE/dosbox_r36sx/README.md
MIPS_NATIVE/dosbox_r36sx/drive_c/README.TXT
```

## Binary

`dosbox_r36sx` is the first baseline build of official DOSBox 0.74-3 for the
R36SX/SF3000 MIPS Linux firmware. It currently links against the firmware SDL
1.2 runtime and does not yet contain the Pico-286 style R36SX menu overlays.

Known dynamic dependencies:

```text
libSDL-1.2.so.0
libpthread.so.0
libdl.so.2
libm.so.6
libstdc++.so.6
libgcc_s.so.1
libc.so.6
```

These libraries are present in the inspected firmware image.

Current binary:

```text
size:   8022396 bytes
sha256: 46144C0C093B58360E1AC4B78C2B665510143D1607CC218C6DC54D4B6B101ABB
scan:   Microsoft Defender found no threats
```
