# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

This debug build writes a startup/runtime log to:

- `MIPS_NATIVE/pico_286/pico_286.log`

If that path cannot be opened on the device, it falls back to:

- `pico_286.log` in the SD-card root

Optional PC disk images should be placed in the same directory:

- `MIPS_NATIVE/pico_286/fdd0.img`
- `MIPS_NATIVE/pico_286/fdd1.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

The current binary is an unstripped ELF32 little-endian MIPS executable.  Zig
0.16.0 `objcopy --strip-all` reports `error: unimplemented` for this file, so
the build keeps symbols for now.

This patch was rebuilt after the first device log showed:

- `main: pthread_create sound=0 ticks=0`
- `main: fatal signal 11`

The new binary adds first-iteration logs for `sound_thread`, `ticks_thread`,
`exec86()`, and `mfb_update()`.  It also avoids the upstream audio cast from
`int16_t *` to `int32_t *` in `get_sound_sample()`.

Follow-up fix: the first diagnostic run reached `main: before exec loop=0` and
then crashed.  The Linux entrypoint had not initialized `read86`, `write86`,
`readw86`, `writew86`, `readdw86`, or `writedw86`; the new binary initializes
them to the same `*_ob` memory backend used by upstream `win32-main.cpp`.
