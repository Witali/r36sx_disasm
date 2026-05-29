# disk_image_patch_076

Rebuilds Pico-286 with held press animation for the shared on-screen keyboard.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The shared R36SX on-screen keyboard now keeps a virtual key visually pressed
until the physical button that activated it is released.  The pressed key shifts
down-right by one pixel and darkens.

Direct shortcuts animate their matching visible keys:

- `B`: `BS`
- `X`: `ESC`
- `Y`: `ENT`
- `A` or `Start`: the highlighted virtual key

The same behavior is used by the preset editor key picker.

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

```text
pico_286 size: 917844 bytes
pico_286 SHA256: 570E55DE4A0357D392A30C418B6152F88DD4A70F76B137F24DB997E9935699A1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_076\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
Defender scan: found no threats
```
