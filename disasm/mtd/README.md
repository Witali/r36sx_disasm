# MTD dump analysis

This directory documents the MTD dumps captured from the device and the boot
evidence found in them.

## Inputs

The raw dumps are in `hardware_info/mtd-dump/`:

| Device | Name | Size | File |
| --- | --- | ---: | --- |
| `/dev/mtd0` | `nor` | `0x80000` | `mtd0_nor.bin` |
| `/dev/mtd1` | `boot` | `0x6c000` | `mtd1_boot.bin` |
| `/dev/mtd2` | `eromfs` | `0x4000` | `mtd2_eromfs.bin` |
| `/dev/mtd3` | `persistentmem` | `0x10000` | `mtd3_persistentmem.bin` |

`mtd0_nor.bin` is the whole NOR image. It matches the concatenation:

```text
mtd0[0x00000:0x6c000] == mtd1_boot.bin
mtd0[0x6c000:0x70000] == mtd2_eromfs.bin
mtd0[0x70000:0x80000] == mtd3_persistentmem.bin
```

The same layout is described in `hardware_info/dtb/dtb.dts` under the RTOS
`sfspi/spi_nor_flash/partitions` block.

## Generated files

| File | Purpose |
| --- | --- |
| `mtd1_boot.mipsel.disasm` | MIPS32r2 little-endian disassembly of the boot partition. |
| `mtd1_boot.mmio_hits.txt` | Compact list of notable MMIO constants found in the little-endian disassembly. |
| `mtd1_boot.strings.txt` | Raw string extraction from the boot partition. |
| `ghidra_mtd1_boot_mipsel/` | Ghidra C-like decompile, disassembly, symbol table, and annotated function notes for the MTD boot partition. |
| `mtd2_eromfs.files.txt` | Parsed ROMFS file list. |
| `mtd2_eromfs.strings.txt` | Raw string extraction from the ROMFS partition. |
| `mtd2_eromfs_extracted/` | Extracted ROMFS gzip bitmap resources and decompressed BMP files. |
| `mtd3_persistentmem.strings.txt` | Raw string extraction from persistent memory. |
| `mtd_magic_scan.txt` | Search for common uImage, FDT, gzip, ROMFS, and SquashFS signatures. |
| `stage3_sd_boot_findings.md` / `stage3_sd_boot_findings_ru.md` | Confirmed Stage 3 SD resource requirements and the working single-FAT image result. |

## Partition notes

`mtd1_boot.bin` is raw stripped MIPS boot code/data, not an ELF file. The first
valid-looking code starts around file offset `0x800`. It is little-endian MIPS32r2
and uses KSEG1/MMIO constants such as `0xb882...`, `0xb884...`, `0xb880...`,
`0xbfe0...`, and `0xafc0...`. The `0xb8844000` hits line up with the USB0 block
from DTS, while the DTS SD/MMC controller is at physical `0x1884c000`, mirrored
as KSEG1 `0xb884c000`.

`ghidra_mtd1_boot_mipsel/` decompiles the boot code as two raw-load profiles:
the NOR first stage at `0xbfe00000` and the RAM second stage at `0x89eb0000`
from file offset `0x3000`. The exported `decompiled_all.c` files include short
comments before functions, and `functions.tsv` is the fastest way to scan the
current function names and confidence notes.

The boot partition does not contain obvious ASCII paths such as `cubegm`,
`dtb.bin`, `vmlinux.uImage`, `FAT`, or `mmcblk`. The magic scan also did not find
a Linux `uImage`, FDT/DTB, SquashFS, or gzip payload in `mtd1_boot.bin`.

`mtd2_eromfs.bin` is a ROMFS volume named `rom 696a1190`. It contains only two
gzip-compressed bitmap resources:

```text
popup.bmp.gz
batterylow.bmp.gz
```

They were extracted from ROMFS and decompressed into
`mtd2_eromfs_extracted/popup.bmp` and
`mtd2_eromfs_extracted/batterylow.bmp`. Both decode as 640x480, 32-bit Windows
BMP files.

`mtd3_persistentmem.bin` is persistent settings/data. The readable strings include
media paths such as `/media/mmc/Movie/sample.mp4`, `/media/mmc/Ebook/...`, and
`/media/mmc/Music/...`.

## SD boot evidence

The Linux kernel and DTB are not stored in the MTD dump. They are present on the
SD/FAT tree under `disk_image/cubegm/`:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

`file` identifies `cubegm/vmlinux.uImage` as a gzip-compressed Linux/MIPS U-Boot
legacy image with load address `0x80000000` and entry point `0x803337c0`.
`cubegm/avp.uImage` is a gzip-compressed MIPS firmware/standalone image loaded at
`0x8bda4000`.

The decompiled DTS has an RTOS `external_files` block that explicitly names the
same SD-card files:

```dts
external_files {
    path-prefix = "cubegm";
    part1-label = "dtb";
    part1-filename = "dtb.bin";
    part2-label = "avp";
    part2-filename = "avp.uImage";
    part3-label = "linux";
    part3-filename = "vmlinux.uImage";
    part4-label = "logo";
    part4-filename = "xgame-logo.bmp";
};
```

The captured kernel command line is:

```text
root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 earlycon= no_console_suspend noirqdebug
```

So Linux starts from a RAM root with `/linuxrc`, not directly from an ext4 root
partition. The captured `/proc/mounts` then shows `/dev/mmcblk0p1` mounted as
VFAT on `/media/mmc`, `/mnt/sdcard`, and over the main userspace directories:

```text
/lib
/usr
/bin
/sbin
/etc
```

## Current boot-chain conclusion

The evidence points to this chain:

1. The SoC starts from SPI NOR and executes the `boot` partition from MTD.
2. The NOR boot code initializes low-level SoC blocks and hands off to SD-card
   boot artifacts rather than embedding Linux in MTD.
3. The SD/FAT `cubegm/` directory supplies the DTB, AVP firmware, Linux uImage,
   and boot logo named by the DTS `external_files` block.
4. The Linux kernel boots with `root=/dev/ram0 init=/linuxrc`.
5. Early userspace mounts `/dev/mmcblk0p1` as VFAT and overlays the SD-card
   copies of `/bin`, `/sbin`, `/etc`, `/lib`, and `/usr`, so the practical
   userspace continues from SD.

What is still not proven from the raw MTD code alone: the exact function and FAT
path parser inside `mtd1_boot.bin`. The boot partition has no readable file-path
strings, so the SD handoff is inferred from the DTS `external_files`, the actual
SD files, the uImage headers, and the runtime mount/cmdline captures.

## Confirmed SD image baseline

The conservative image `local_artifacts/r36sx_stage3_pass_stockfat.img` was
built with `tools/build_stage3_pass_stock_sd_image_wsl.sh` and verified with
`tools/verify_stage3_pass_stock_sd_image_wsl.sh`. It is a 2 GiB MBR image with a
single FAT32 partition starting at 4 MiB, stock `cubegm/dtb.bin`, and the
required `cubegm/dtb.bin`, `cubegm/avp.uImage`, `cubegm/vmlinux.uImage`, and
`cubegm/xgame-logo.bmp` resources.

The user wrote this image to SD and the device booted into TinyMC. This confirms
that Stage 3 accepts that single-FAT layout and that the original full SD-card
size is not required for the external resource lookup. `local_artifacts/` stays
ignored and is not committed to Git.
