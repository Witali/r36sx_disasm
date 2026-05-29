# disk_image_patch_084

Pico-286 update for Sopwith CGA graphics.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- `SW.EXE` from the bundled Sopwith floppy normally selects BIOS video mode
  `04h`, CGA 320x200 4-color graphics.
- The Pico-286 renderer now reads CGA modes `04h`, `05h`, and `06h` from the
  logical `uint32_t VIDEORAM[]` slots instead of byte-walking the host storage.
  This fixes scrambled CGA graphics while keeping the existing 640x480 output
  scaling.

Build details:

```text
pico_286 size: 896604 bytes
pico_286 SHA256: D1D5787DC9C83E8BBC1EC825B0AB623F0E6F5EB98801365DF8892EA4A187F874
Defender scan: found no threats
```
