# disk_image_patch_063

Purpose: update the Pico-286 native app with the revised key preset editor.

Copy this patch directory over the root of the SD-card disk image.

## Files

- `MIPS_NATIVE/pico_286/pico_286`
  - Rebuilt MIPS32 little-endian Pico-286 executable.
  - The Select key preset editor now uses a two-column physical button list.
  - Preset edits are staged in a draft and are saved only through the visual
    `OK` button.
  - `Cancel` or `Select` closes the editor without saving the draft.
  - Preset names can be edited through the shared on-screen keyboard picker.
  - Binding rows open the same on-screen keyboard as a key picker.
- `MIPS_NATIVE/pico_286/keypresets.conf`
  - Default key preset config, included so a fresh install has the expected
    fallback preset.

## Build

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` still reports Zig objcopy `unimplemented`, so the patch keeps the
unstripped executable.

## Verification

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_063\MIPS_NATIVE\pico_286\pico_286
```

Result: Microsoft Defender found no threats.

```text
Size: 8054724 bytes
SHA256: 598071628DA6ECA5B608625CB636AEF76977BB5C59D50039B4E93BB37FC2E4E1
```
