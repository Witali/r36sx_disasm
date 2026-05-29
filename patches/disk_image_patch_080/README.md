# disk_image_patch_080

Updates the Pico-286 native executable so physical B and X are direct
on-screen keyboard shortcuts everywhere the keyboard is active.

Copy this patch over the SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior:

- B sends Backspace.
- X sends Escape.

This also applies inside the key preset picker keyboard.  In preset-name
editing, B deletes one character and X closes the picker through Escape.
