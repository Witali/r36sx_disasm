# Fuse ZX Spectrum 48K

Experimental ZX Spectrum 48K libretro core for the `cubegm/rkgame` launcher.

## Files

- `libemu_fuse.so`: MIPS32 little-endian Fuse libretro core.
- `libemu_fuse.so.cfg`: core options for Spectrum 48K and joypad key mapping.
- `R36SX_ZX48.tap`: tiny generated autostart BASIC test tape.
- `BUILD_LOG.md`: source, build, verification, and risk notes.
- `BUILD_COMMANDS.md`: repeatable command-level build notes.

## Install

Use `disk_image_patch_013` to copy the ready files into the SD/image.

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

## R36SX Compatibility Rebuild

The device test of the first build returned to the launcher after a black screen.
The current build includes `r36sx_fuse_compat.patch`, which lets
`retro_load_game()` continue when `rkgame` ignores
`RETRO_ENVIRONMENT_SET_PIXEL_FORMAT`.

The current `libemu_fuse.so` also has explicit dynamic dependencies:

```text
libm.so.6
libgcc_s.so.1
libc.so.6
```

## Minimal No-NEEDED Build

The explicit dependency build also returned to the launcher on device.
`disk_image_patch_013` replaces it with a smaller no-`NEEDED` build:

```text
size:   2254084
sha256: 64ED74EDA1873D7B0E5A3BDCFFB3EF4B85CB38A22DA5054E65E6F32B8E4BA179
NEEDED: empty
```

This is not a fully self-contained binary: it still has unresolved standard
C/POSIX symbols that must be visible from the already-running process or loader.
It does define local `exit()` and `abort()` traps so Fuse error paths should not
terminate the launcher process directly.

Use `disk_image_patch_013` for the next device test.
