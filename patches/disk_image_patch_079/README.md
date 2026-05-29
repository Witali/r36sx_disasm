# disk_image_patch_079

Updates the Pico-286 native executable with a corrected on-screen keyboard
layout.

Copy this patch over the SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

The keyboard panel remains edge-to-edge at the bottom of the screen, restores
the header/status text, and leaves only a one-pixel inner gap between the key
rows and the panel border on the left, right, and bottom edges.
