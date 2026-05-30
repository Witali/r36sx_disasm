# Disk Image Patch 097

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 with the direct-present video path:

- normal DOS/game frames are sent directly from the emulator `SCREEN` buffer to
  `driver.so`;
- the on-screen keyboard, disk menu, and key preset editor still use the
  separate composition buffer;
- the red disk activity LED saves and restores its small rectangle around the
  present call.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
