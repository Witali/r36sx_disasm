# Fuse ZX Spectrum 48K

Experimental ZX Spectrum 48K libretro core for the `cubegm/rkgame` launcher.

## Files

- `libemu_fuse.so`: MIPS32 little-endian Fuse libretro core.
- `libemu_fuse.so.cfg`: core options for Spectrum 48K and joypad key mapping.
- `R36SX_ZX48.tap`: tiny generated autostart BASIC test tape.
- `BUILD_LOG.md`: source, build, verification, and risk notes.

## Install

Use `disk_image_patch_011` to copy the ready files into the SD/image.

The first test entry appears in the existing `ATARI` menu as:

```text
R36SX ZX48 Test
```

It is launched through a `cores/filelist.xml` override and should use `libemu_fuse.so`.

## Expected Test

If the core loads, the test TAP should autostart as a Spectrum 48K BASIC program:

```basic
10 PRINT "R36SX ZX48 OK"
```

If it returns to the launcher immediately, check whether `dlopen` failed because unresolved libc/libm symbols were not visible to the core.
