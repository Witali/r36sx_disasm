# disk_image_patch_089

Pico-286 `Fn+B` soft-reset update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- `Fn+B` now soft-resets the emulated PC without exiting Pico-286.
- The input backend releases held virtual keys and closes the on-screen
  keyboard or settings overlays before requesting reset.
- The main emulation loop performs the reset before the next `exec86()` call.

Reset coverage:

- CPU, RAM, VRAM, VGA state
- keyboard queue and keyboard controller status
- PIC and PIT state
- PC speaker/Covox, Sound Blaster, SN76489, and OPL
- host-side pending audio buffer

Build details:

```text
pico_286 size: 910824 bytes
pico_286 SHA256: BAE91FC64A41F8D4AC86E9DCBB89220D0D99695967643CE65EAB590B36B39F21
Defender scan: found no threats
```
