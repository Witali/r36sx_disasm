# Disk Image Patch 100

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 configuration layout:

- every `pico_286.conf` option is now inside an INI section;
- `r36sx_pico286_save_config()` writes the grouped layout when settings are
  saved from the disk image menu;
- existing option names and values are unchanged.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
- `MIPS_NATIVE/pico_286/pico_286.conf`
