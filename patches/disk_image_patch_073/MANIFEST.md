# disk_image_patch_073

Rebuilds Pico-286 with an edge-to-edge on-screen keyboard panel.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The shared R36SX on-screen keyboard panel is now drawn flush to the left,
right, and bottom screen borders.  The old header/status text inside the panel
was removed, and the key rows are centered with compact equal inner padding
around the key area.

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

```text
pico_286 size: 921312 bytes
pico_286 SHA256: F818B656F70CD88212B16B4EC1B5F460489A9573966FC8102AD187336FC3C3C9
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_073\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
Defender scan: found no threats
```
