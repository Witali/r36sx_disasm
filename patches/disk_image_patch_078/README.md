# disk_image_patch_078

Adds the remaining official FreeDOS 1.4 Floppy Edition package images to the
Pico-286 native app directory.

Copy this patch over the SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/FreeDOS4.img` from `144m/x86DSK03.img`
- `MIPS_NATIVE/pico_286/FreeDOS5.img` from `144m/x86DSK04.img`
- `MIPS_NATIVE/pico_286/FreeDOS6.img` from `144m/x86DSK05.img`
- `MIPS_NATIVE/pico_286/FreeDOS7.img` from `144m/x86DSK06.img`

Use the Pico-286 disk image menu (`Fn+Select`) to mount these images when the
FreeDOS installer asks for later package diskettes.
