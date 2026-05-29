# disk_image_patch_069

Rebuilds Pico-286 with a clearer preset-name editing indicator.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

## Change

The key preset editor now shows a blinking underscore cursor next to the
current preset name while the `RENAME PRESET:` on-screen keyboard prompt is
active.  The embedded 5x7 editor font now includes an underscore glyph.

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_069\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
pico_286 size: 8114908 bytes
pico_286 SHA256: C98A6ABE633D9D22AC9E153D695EE6B059DE42065E753B1F651FCF07D6942485
Defender scan: found no threats
```
