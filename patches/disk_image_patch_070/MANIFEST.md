# disk_image_patch_070

Rebuilds Pico-286 with consistent settings-menu buttons.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

## Controls

In Pico-286 settings menus:

- `A` or `Y`: accept/activate the selected row, cycle a disk image, or accept
  the highlighted on-screen key in picker mode.
- `B` or `X`: cancel the disk menu, key preset editor, or picker mode.

This affects the disk image binding menu, the key preset editor, and the
on-screen keyboard picker used by preset binding/name editing.  The normal DOS
on-screen keyboard keeps its existing direct shortcuts.

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
.\tools\scan-download.ps1 .\patches\disk_image_patch_070\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
pico_286 size: 8114624 bytes
pico_286 SHA256: 0CC16E9BBD854D14EAF7DC5AF7D8E684383F4087A3B2EDE8E14A4E0AB27D99F5
Defender scan: found no threats
```
