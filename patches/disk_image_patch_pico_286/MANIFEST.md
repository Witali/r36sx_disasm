# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/fdd0.img`
- `MIPS_NATIVE/pico_286/fdd1.img`
- `MIPS_NATIVE/pico_286/fdd2.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

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
- `MIPS_NATIVE/pico_286/fdd2.img` for older test builds that request a third
  floppy image

The included image set is based on official FreeDOS 1.4 Floppy Edition:

- `fdd0.img`: `144m/x86BOOT.img`, bootable FreeDOS floppy.
- `fdd1.img`: `144m/x86DSK01.img`.
- `fdd2.img`: `144m/x86DSK02.img`.
- `hdd.img` and `hdd2.img`: blank raw hard disk images, 65/16/63 CHS,
  33,546,240 bytes each.

The downloaded FreeDOS archive, extracted files, and final copied image
directories were scanned with `tools/scan-download.ps1`; Microsoft Defender
reported no threats.

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

Current build also includes an on-screen disk activity indicator: a small
blinking red circle appears in the lower-right corner when Pico-286 reads from
or writes to disk image sectors.

It also maps BIOS video modes `0x20` and `0x30` to the 80x25 text renderer so
FreeDOS/BIOS boot text can be shown instead of `unsupported video mode 20/30`.
