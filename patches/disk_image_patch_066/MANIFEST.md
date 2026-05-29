# disk_image_patch_066

Purpose: rebuild Pico-286 with an optional cursor-key block in the on-screen
keyboard.

Copy this patch directory over the root of the SD-card disk image.

## Files

- `MIPS_NATIVE/pico_286/pico_286`
  - Rebuilt MIPS32 little-endian Pico-286 executable.
  - Adds a right-side inverted-T cursor-key block to the shared on-screen
    keyboard when enabled by config.
  - The normal on-screen keys shrink while the cursor block is visible so the
    keyboard stays inside the existing bottom panel.
- `MIPS_NATIVE/pico_286/pico_286.conf`
  - Adds `osk_cursor_keys=on`.
  - Set it to `off` to hide the cursor-key block.

## Build

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` still reports Zig objcopy `unimplemented`, so this patch keeps the
unstripped executable.

## Verification

Microsoft Defender scans found no threats in all updated executable copies.

Local TTF check: DejaVu Sans Mono, Hack, JetBrains Mono NL, Liberation Mono,
Noto Sans Mono, and Source Code Pro contain arrow glyphs `U+2190..U+2193`;
Roboto Mono does not.  The runtime keyboard uses built-in 5x7 bitmap arrows, so
no new font file was required.

```text
pico_286 size: 8076964 bytes
pico_286 SHA256: F8FA17D225EBF84BAE8AE5D1D7364FE1AD11B3D5D36002BE1DD8D9C67C6DFC4F
Defender scan: found no threats
```
