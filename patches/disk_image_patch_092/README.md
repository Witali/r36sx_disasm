# disk_image_patch_092

This patch updates the native Pico-286 executable and its local control guide.

Copy this directory over the original SD-card filesystem root.  It installs or
replaces:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The active Pico-286 video path now uses RGB565 end-to-end on R36SX:

- The DOS renderer writes into a 16-bit `SCREEN[640 * 480]`.
- CGA/TGA/VGA output uses RGB565 shadow palettes.
- The MiniFB backend copies RGB565 rows directly instead of converting a
  32-bit RGB888 frame before presentation.
- On-screen keyboard compression blends RGB565 pixels directly.

The older Fake86 SDL shim still has an ARGB8888 texture compatibility path and
can be converted separately if that port becomes active again.

## Build

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

```text
pico_286 size: 917496 bytes
pico_286 SHA256: BED9F7092FD1E03A169EB34A40BD1964FC6F9ED403F5E1DDADF67F0E0E2D6EDB
Defender scan: found no threats
```
