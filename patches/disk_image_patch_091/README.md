# disk_image_patch_091

This patch updates the native Pico-286 executable and its local control guide.

Copy this directory over the original SD-card filesystem root.  It installs or
replaces:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`

## Change

The Pico-286 disk image menu now includes a `BOOT ORDER` row.  Use it to switch
between:

- `A,C`: try the floppy drive before the hard disk.
- `C,A`: try the hard disk before the floppy drive.

Left/Right or A/Y cycles the row.  `SAVE/APPLY` writes the selected order to
`pico_286.conf` as `boot_order=fdd0,hdd0` or `boot_order=hdd0,fdd0` and applies
the current disk bindings.  Existing `boot_order=rom` is preserved if the row
is not touched.

## Build

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

```text
pico_286 size: 915372 bytes
pico_286 SHA256: F9CBF5F875428A33E568E07110E814597D13177C69C8CBA07099D0438D58D429
Defender scan: found no threats
```
