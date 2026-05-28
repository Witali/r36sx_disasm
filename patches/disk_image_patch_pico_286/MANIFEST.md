# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

Optional PC disk images should be placed in the same directory:

- `MIPS_NATIVE/pico_286/fdd0.img`
- `MIPS_NATIVE/pico_286/fdd1.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

The current binary is an unstripped ELF32 little-endian MIPS executable.  Zig
0.16.0 `objcopy --strip-all` reports `error: unimplemented` for this file, so
the build keeps symbols for now.
