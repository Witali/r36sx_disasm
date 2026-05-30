# Disk Image Patch 098

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 screenshots:

- `Fn` + D-pad `Up` now saves compressed PNG screenshots instead of
  uncompressed BMP files;
- screenshots are written under `MIPS_NATIVE/pico_286/screenshots` using names
  like `pico_286_YYYYMMDD_HHMMSS_NNN.png`;
- the PNG writer uses zlib directly and does not depend on libpng.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
