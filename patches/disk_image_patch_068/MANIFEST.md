# disk_image_patch_068

Rebuilds Pico-286 with revised application exit controls.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

## Controls

- Tap Fn: toggle the on-screen keyboard.
- Fn+Select: open the disk image binding menu.
- Fn+Start: open the key preset editor.
- Fn+X: exit back to TinyMC.
- Hold Fn for more than 3 seconds: emergency exit back to TinyMC.

Select+Start no longer exits Pico-286.

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
.\tools\scan-download.ps1 .\patches\disk_image_patch_068\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
pico_286 size: 8114488 bytes
pico_286 SHA256: 6DD9DC4A8528C38EE909FDF9CF2F13A6E42890CBBF61AAAC89FEEA98DBC809AD
Defender scan: found no threats
```
