# disk_image_patch_083

Pico-286 update for BIOS video mode `0Fh`.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- BIOS video mode `0Fh` now renders as EGA/VGA 640x350 monochrome graphics.
- The renderer treats a pixel as foreground if the corresponding bit is set in
  any packed EGA/VGA plane, which keeps output visible across different
  plane-write paths.
- The on-screen keyboard scaler now sees mode `0Fh` as a 350-row active image.

Build details:

```text
pico_286 size: 896284 bytes
pico_286 SHA256: C9280E6B8A3F8838D2C050AAE91C04D39E1784BFFBAE2DAAAC403088E7038600
Defender scan: found no threats
```
