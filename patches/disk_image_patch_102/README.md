# Disk Image Patch 102

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 key presets:

- the key preset editor shows the physical `SELECT` assignment row in the
  lower-left button slot instead of the old `X/B CANCEL` label;
- the default preset maps physical `Select` to `Shift`;
- the visual `CANCEL` button remains at the bottom of the editor, and B/X still
  cancel the editor when no picker keyboard is open.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
- `MIPS_NATIVE/pico_286/keypresets.conf`

```text
pico_286 size: 981136 bytes
pico_286 SHA256: 980E3291F2227EE9109551C83DB4D6679FBC3FFB665D80C4BCA3CDBD86FB5AD3
```
