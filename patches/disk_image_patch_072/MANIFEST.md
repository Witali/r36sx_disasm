# disk_image_patch_072

Rebuilds Pico-286 without embedding the Pico/ESP-style `.psram` memory section
in the Linux/MIPS executable.

Copy this patch over the original SD-card filesystem root.  It updates:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The upstream emulator used a `.psram` section for RP/Pico-style hardware with
external PSRAM.  In the R36SX Linux/MIPS native executable, that section was
being stored as normal file contents, so the binary carried about 7 MB of
zero-filled RAM/EMS/XMS data.

The Linux/MIPS build now keeps those arrays as normal zero-initialized runtime
storage.  The executable is much smaller, while runtime memory capacity is
unchanged.

```text
old pico_286 size: about 8,114,624 bytes
new pico_286 size: 922,472 bytes
new pico_286 SHA256: 68CCFDD09C2230CD862A5BBEABD88277F9BCA10087F5F2F0F9F12B0217CA5913
```

## Verification

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` reported Zig objcopy `unimplemented`, so the unstripped executable
was kept.

Section check:

```text
file size: 922472 bytes
.psram: not present as a file payload
.bss: about 8.7 MiB runtime zero-initialized memory
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_072\MIPS_NATIVE\pico_286\pico_286
```

Result:

```text
Defender scan: found no threats
```
