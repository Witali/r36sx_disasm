# disk_image_patch_fb_test

This patch adds `fb-test`, a direct framebuffer diagnostic program.

Copy the contents of this directory over the root of the SD card, preserving
directories.

## Files

```text
MIPS_NATIVE/fb-test/fb-test
MIPS_NATIVE/fb-test/README.md
MIPS_NATIVE/fb-test/README_ru.md
```

## Launch

Run:

```sh
/mnt/sdcard/MIPS_NATIVE/fb-test/fb-test
```

The program probes `/dev/dis`, probes `/dev/ge` without feeding GE commands,
maps `/dev/fb0`, draws a direct framebuffer RGB test pattern, and tries a
standard framebuffer pan/flip.

