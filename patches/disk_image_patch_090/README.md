# disk_image_patch_090

Pico-286 on-screen keyboard D-pad repeat update.

Copy this patch over the SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

Behavior change:

- Holding D-pad Left/Right/Up/Down in the on-screen keyboard repeats movement.
- The first move still happens immediately.
- Repeat begins after about 280 ms and continues about every 85 ms.
- This also applies to the key picker inside the preset editor.
- A/Start, B, X, Y, and Select remain one-shot actions.

Build details:

```text
pico_286 size: 912504 bytes
pico_286 SHA256: 5AB34A3F4B7C9DE260E990228549C66B55629D6039D0CF1E1C567B59153E8810
Defender scan: found no threats
```
