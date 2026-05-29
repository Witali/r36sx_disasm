# disk_image_patch_075

Rebuilds Pico-286 with physical `Select` as a remappable preset key.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/keypresets.conf`
- `MIPS_NATIVE/pico_286/README.md`

## Change

Physical `Select` is now a normal remappable key preset entry.  The default
preset maps it to `Space`, and the preset editor shows `SELECT` in the left
column bottom slot opposite `START`.

Older `keypresets.conf` presets that do not contain `select=...` are filled
from current defaults before explicit values are applied, so they receive the
new `Select = Space` binding.  An explicit `select=NONE` or another value still
overrides the default.

`Fn+Select` remains the disk image binding menu shortcut.

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

```text
pico_286 size: 914632 bytes
pico_286 SHA256: 04E15547B6DEB47BB9CCC781572B0070F672174B6E9AA099AC76BA96A8CE561E
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_075\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
Defender scan: found no threats
```
