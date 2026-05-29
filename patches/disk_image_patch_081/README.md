# disk_image_patch_081

Pico-286 update for active-height on-screen keyboard compression.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- When the on-screen keyboard is visible, Pico-286 now compresses only the
  active emulated video area into the remaining top 384 pixels.
- Normal DOS 80x25 text mode uses 400 active rows, so the black margin below
  the text area is no longer included in the compression.
- EGA `0x10` reports 350 active rows, and the Hercules `0x1e` path reports
  348 active rows.

Build details:

```text
pico_286 size: 891968 bytes
pico_286 SHA256: 78498E29EE66FC29570D30CABA35699EAC46FC053DA5D1D981821FAC00589A94
Defender scan: found no threats
```
