# Disk Image Patch 099

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 screenshots:

- `pico_286.conf` now has a `[screenshot]` section;
- `screenshot_format=png` saves compressed PNG files;
- `screenshot_format=bmp` restores the old uncompressed 24-bit BMP output;
- `Fn` + D-pad `Up` keeps the same screenshot shortcut.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
- `MIPS_NATIVE/pico_286/pico_286.conf`
