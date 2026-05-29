# disk_image_patch_064

Purpose: fix Pico-286 DOS hard disk reads by replacing blank HDD images with
host-created FAT16 images and adding disk I/O diagnostics.

Copy this patch directory over the root of the SD-card disk image.

Note: `*.img` files are intentionally ignored by git because they are large.
They were generated and copied into this local patch directory; if they are
missing in a fresh checkout, regenerate them with the commands below.

## Files

- `MIPS_NATIVE/pico_286/pico_286`
  - Rebuilt MIPS32 little-endian Pico-286 executable.
  - Adds `INT 13h` diagnostics for failed reads, writes, verifies, invalid CHS
    requests, out-of-range transfers, and host file I/O failures.
  - In debug builds the diagnostics go to
    `/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log`.
- `MIPS_NATIVE/pico_286/hdd.img`
  - 33,546,240-byte FAT16 hard disk image for BIOS drive `80h` / DOS `C:`.
  - CHS `65,16,63`, active MBR partition type `0x06`, partition start sector
    `63`, partition size `65457` sectors.
  - Contains `README.TXT` so `DIR C:` should show at least one file.
- `MIPS_NATIVE/pico_286/hdd2.img`
  - Same layout for BIOS drive `81h` / DOS `D:`.
- `MIPS_NATIVE/pico_286/pico_286.conf`
  - Included to keep `hdd0_geometry=65,16,63` and `hdd1_geometry=65,16,63`
    beside the new images.

## Build

```powershell
python .\tools\create_fat16_hdd.py --output .\disk_image\MIPS_NATIVE\pico_286\hdd.img --geometry 65,16,63 --label "R36SX HDD" --file README.TXT=.\homebrew\pico_286\dos_files\hdd\README.TXT
python .\tools\create_fat16_hdd.py --output .\disk_image\MIPS_NATIVE\pico_286\hdd2.img --geometry 65,16,63 --label "R36SX HDD2" --file README.TXT=.\homebrew\pico_286\dos_files\hdd\README.TXT
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` still reports Zig objcopy `unimplemented`, so the patch keeps the
unstripped executable.

## Verification

The generated hard disk images were checked locally:

```text
hdd.img MBR: active FAT16 type 0x06, start=63, size=65457, signature=55AA
hdd.img VBR: FAT16, bps=512, spc=4, fats=2, root=512, spt=63, heads=16, hidden=63, signature=55AA
hdd.img root: volume label R36SX HDD and README.TXT
```

Microsoft Defender scans found no threats in the rebuilt executable or the new
HDD images.

```text
pico_286 size: 8058240 bytes
pico_286 SHA256: 43E4C5A227FE8E83F4A643F1546F6E9B51E79D509B78BA3E13C4B1C75CDC0332
hdd.img SHA256: 26953A16F571AB8452570E37E2C0688C0B60A2859E72B81D6E9EBA8D80379818
hdd2.img SHA256: 45A095789D0C5A4F8E0BB2717493874C761789AF199D602BF794DBAB12448C71
```
