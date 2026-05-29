# disk_image_patch_065

Purpose: rebuild Pico-286 with a key preset editor layout that matches the
physical sides of the handheld.

Copy this patch directory over the root of the SD-card disk image.

## Files

- `MIPS_NATIVE/pico_286/pico_286`
  - Rebuilt MIPS32 little-endian Pico-286 executable.
  - The preset editor now shows d-pad, `L`, `L2`, and static `SELECT CANCEL`
    on the left column.
  - The right column now shows `X`, `Y`, `A`, `B`, `R`, `R2`, and `START`.
  - `Select` and `Start` are at the bottom of their columns.
  - Saved `keypresets.conf` keys are unchanged.

## Build

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` still reports Zig objcopy `unimplemented`, so this patch keeps the
unstripped executable.

## Verification

Microsoft Defender scans found no threats in all updated executable copies.

```text
pico_286 size: 8061088 bytes
pico_286 SHA256: 7770B1BD818DA27E1E4F7B924ACABC95B193509314341B24B6CC769F690D78C9
```
