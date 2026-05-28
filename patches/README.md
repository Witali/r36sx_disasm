# Disk Image Patches

Each `disk_image_patch_*` directory is an overlay patch for the original SD-card
disk contents.

The files inside each patch directory keep the same relative paths as files on
the original disk. To apply a patch, copy the contents of the selected patch
directory to the root of the original disk or SD card and overwrite existing
files when asked.

Example:

- `disk_image_patch_tiny_mc/cubegm/icube.sh` should overwrite
  `cubegm/icube.sh` on the original disk.
- `disk_image_patch_tiny_mc/MIPS_NATIVE/tiny_mc/tiny_mc` should be copied to
  `MIPS_NATIVE/tiny_mc/tiny_mc` on the original disk.
- `disk_image_patch_tiny_mc/MIPS_NATIVE/tiny_mc/tiny_mc.conf` should be copied
  to `MIPS_NATIVE/tiny_mc/tiny_mc.conf` on the original disk.
- `disk_image_patch_tiny_mc/MIPS_NATIVE/common/fonts/*` should be copied to
  `MIPS_NATIVE/common/fonts/` on the original disk.

Make a backup of the original disk before applying patches. Numbered patch
directories are the experiment history; `disk_image_patch_tiny_mc` is the
current consolidated Tiny MC launcher overlay.
