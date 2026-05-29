# disk_image_patch_082

Pico-286 update for BIOS video mode `07h`.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- BIOS video mode `07h` now renders as MDA-compatible 80x25 monochrome text.
- The real MDA mode is normally 720x350 with 9x14 character cells; this port
  uses 8x14 cells so the 80-column text layout fits exactly into the R36SX
  640-pixel-wide framebuffer.
- BIOS text output uses the MDA `B000:` text memory base while mode `07h` is
  active.

Build details:

```text
pico_286 size: 894948 bytes
pico_286 SHA256: 230E13863C3760CF975602A5AA774B8269F48E85F81D1E4FD8C32EA2E5A2DD19
Defender scan: found no threats
```
