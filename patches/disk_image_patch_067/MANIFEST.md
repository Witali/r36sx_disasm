# disk_image_patch_067

Purpose: rebuild Pico-286 with Fn chord controls and an in-app disk image
binding menu.

Copy this patch directory over the root of the SD-card disk image.

## Files

- `MIPS_NATIVE/pico_286/pico_286`
  - Rebuilt MIPS32 little-endian Pico-286 executable.
  - Tap and release Fn to toggle the on-screen keyboard.
  - Hold Fn and press Select to open the disk menu.
  - Hold Fn and press Start to open the key preset editor.
  - Select alone no longer opens key presets.
  - The disk menu can bind `.img` files to `FDD0`, `FDD1`, `HDD0`, and `HDD1`.
  - The disk menu includes `SAVE/APPLY`, `EXIT APP`, and `CANCEL`.
- `MIPS_NATIVE/pico_286/pico_286.conf`
  - Included beside the binary because the disk menu writes the same config
    file when bindings are changed.

## Notes

`SAVE/APPLY` writes `pico_286.conf` and calls `insertdisk()` immediately.
Hard-disk changes can still require an emulator restart because DOS may cache
drive state; use the menu's `EXIT APP` row for a clean return to TinyMC.

## Build

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` still reports Zig objcopy `unimplemented`, so this patch keeps the
unstripped executable.

## Verification

Microsoft Defender scans found no threats in all updated executable copies.

```text
pico_286 size: 8113476 bytes
pico_286 SHA256: FC94F00C0F93F611F6C52CDEA587C3DF8A7F685594ED82A773B8D3376E0FFFCD
Defender scan: found no threats
```
