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

`dosbox_r36sx` is a first-pass build of official DOSBox 0.74-3 for the
R36SX/SF3000 MIPS Linux firmware. It links against the firmware SDL 1.2
runtime, but also mirrors every DOSBox `GFX_EndUpdate()` surface to
`/mnt/sdcard/cubegm/driver.so` as a 640x480 RGB565 framebuffer.

This revision forces RGB SDL output instead of 8bpp paletted output for the
mirrored video path, and logs the first 30 frame presents to
`dosbox.stderr.log`. Frame conversion now uses direct SDL pixel-format mask
extraction instead of per-pixel `SDL_GetRGB()`.

It does not yet contain the Pico-286 style R36SX menu overlays or on-screen
keyboard.

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
size:   8033552 bytes
sha256: 09FEDCDF5612B36B8A19A90DF386245596C39E20EDC698BA8B63F46EA875066A
scan:   Microsoft Defender found no threats
```
