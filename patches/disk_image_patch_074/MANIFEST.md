# disk_image_patch_074

Rebuilds Pico-286 so the right-side on-screen cursor-key block is only shown
inside the key preset editor picker.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The normal DOS on-screen keyboard now always hides the right-side cursor-key
block.  The key picker used by the preset editor enables that block explicitly,
so cursor keys are still available when assigning physical buttons.

The default `pico_286.conf` no longer contains `osk_cursor_keys`, and the disk
config save path no longer writes that legacy setting.  Existing configs may
still contain the old key; it is accepted for compatibility but has no effect.

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

```text
pico_286 size: 914080 bytes
pico_286 SHA256: 5B1D9D8887C512FFF48C125B084CFEA6B05CC9F4F7E481E0D65D2804EEB07E7F
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_074\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
Defender scan: found no threats
```
