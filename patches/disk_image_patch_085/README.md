# disk_image_patch_085

Pico-286 video present cache update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`

Behavior change:

- The R36SX MiniFB layer now keeps a converted RGB565 base frame and only
  rebuilds it when the DOS renderer marks a new frame or when the on-screen
  keyboard changes the video layout.
- Menus, the on-screen keyboard, and the disk LED draw on a separate overlay
  frame copied from that cached base image.
- When no overlay is active and no new DOS frame is ready, `mfb_update()` skips
  both the RGB888-to-RGB565 conversion and the `driver.so` frame submit.
- The no-keyboard path also converts only the active DOS video height and
  clears the remaining bottom rows as black.

Build details:

```text
pico_286 size: 899216 bytes
pico_286 SHA256: F4D7FF18F098A744C640FC8B63395D05C6E78453FF7E22E43901153597985BF7
Defender scan: found no threats
```
