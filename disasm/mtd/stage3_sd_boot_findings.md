# Stage 3 SD Boot Findings

Date: 2026-06-03.

This document records the current confirmed findings about the third boot stage,
the SD card, and the image that actually passed Stage 3 on the device.

## What Stage 3 Looks For

Stage 3 does not look for a Debian partition and does not visibly enforce a
fixed SD-card size. In the code found so far, the key condition is availability
of boot resources described by the DTB.

`FUN_89da9474` resolves a resource in this order:

1. First as an internal MTD partition. If the label is found, it builds
   `/dev/mtdblock%d`.
2. Then via the DTB node `/hcrtos/external_partitions`. In that mode, the
   resource maps to `/dev/mmcblk0p%d`.
3. Then via the DTB node `/hcrtos/external_files`. In that mode, Stage 3 reads
   `part-num`, `path-prefix`, `part%d-label`, and `part%d-filename`, then opens
   the file on SD.

Our DTB uses `external_files`:

```dts
external_files {
    part-num = <0x4>;
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

In practice, Stage 3 must read:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

The main boot flow also requests an `initramdisk` resource, but our DTB does not
define it. If it is not found, the code skips that step, so it is considered
optional for the current stock path.

## "Please Insert TF Card" Screen

The image is stored inside the internal MTD2 EROMFS as `popup.bmp.gz`. Stage 3
shows it not because the image itself is missing, but when a described external
resource cannot be opened or prepared.

Found chain:

```text
FUN_89da9474
  -> retry external resource
  -> after timer 2000: "show popup"
  -> FUN_89da98ec(0)
  -> try eromfs / eromfs2 / eromfs3
  -> showlogo /eromfs/popup.bmp
  -> fallback /eromfs/popup.bmp.gz
  -> fallback /eromfs/popup.hc
```

High-probability popup causes:

- the card is not physically detected;
- early MMC init does not create the expected device;
- the first FAT/VFAT partition is not readable by Stage 3;
- the `cubegm/...` path is missing or a file cannot be opened;
- a modified DTB changes early boot conditions;
- extra partitioning confuses a simple early MBR/FAT parser.

## Confirmed Working Image

A conservative image was created:

```text
local_artifacts/r36sx_stage3_pass_stockfat.img
```

`local_artifacts/` is not committed to Git. The image is reproducible with:

```text
tools/build_stage3_pass_stock_sd_image_wsl.sh
```

It is verified with:

```text
tools/verify_stage3_pass_stock_sd_image_wsl.sh
```

Image parameters:

```text
size:             2 GiB
partition table:  msdos / MBR
partition 1:      FAT32
partition start:  4 MiB
partition size:   2044 MiB
partition flags:  boot, lba
label:            R36SXBOOT
```

The FAT partition contains:

```text
cubegm/
rootfs/
MIPS_NATIVE/
Chkdsk.bat
chkdsk.exe
Movie/
Ebook/
Music/
```

The important detail: `cubegm/dtb.bin` remains stock. Its bootargs were not
changed:

```text
root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 earlycon= no_console_suspend noirqdebug
```

WSL verification confirmed that the image contains all Stage 3 resources, and
their SHA256 hashes match the source `disk_image/cubegm/*` files:

```text
cubegm/dtb.bin
cubegm/avp.uImage
cubegm/vmlinux.uImage
cubegm/xgame-logo.bmp
```

## Hardware Result

The user wrote the image to an SD card and the device booted. Instead of showing
`Please Insert TF Card`, the boot chain reached TinyMC.

This confirms:

1. Stage 3 accepts a 2 GiB MBR image.
2. The first partition can be FAT32 starting at 4 MiB.
3. The original full card size is not required to pass Stage 3.
4. Stock `dtb.bin`, `avp.uImage`, `vmlinux.uImage`, and `xgame-logo.bmp` under
   `cubegm/` are sufficient to pass the external resource lookup.
5. The stock `root=/dev/ram0 init=/linuxrc` path still starts userspace, which
   then uses the SD/FAT runtime.

The current working chain is:

```text
SPI NOR / MTD boot
  -> Stage 2 unpacks LZMA Stage 3 into RAM at 0x89da0000
  -> Stage 3 reads external_files from the DTB
  -> FAT32:/cubegm/dtb.bin
  -> FAT32:/cubegm/avp.uImage
  -> FAT32:/cubegm/vmlinux.uImage
  -> FAT32:/cubegm/xgame-logo.bmp
  -> Linux root=/dev/ram0 init=/linuxrc
  -> SD/FAT runtime overlays /bin, /lib, /usr, /etc
  -> hcdaemon / icube route
  -> TinyMC
```

## What This Says About Failed Debian Images

Earlier Debian-style images with changed bootargs, a second ext4 partition, or a
minimal FAT set were not accepted by the firmware. After the successful
single-FAT test, the likely breaking factor was one of:

- modified `dtb.bin`;
- added second partition;
- insufficiently stock-like FAT runtime;
- different partition order or geometry;
- switching too early to `root=/dev/mmcblk0p2`.

It is not yet proven that a second ext4 partition alone breaks Stage 3. That
needs to be tested from the now-working FAT baseline.

## Next Safe Plan

1. Keep the working single-FAT image as the baseline.
2. Create a variant with the same first FAT32 partition and the same stock
   `dtb.bin`, but add a second ext4 partition.
3. Check whether that image still passes Stage 3 and reaches TinyMC.
4. If it does, use stock userspace/TinyMC/Shell to run Debian as a chroot or
   pivot test from `/dev/mmcblk0p2`.
5. Only then try a modified DTB with
   `root=/dev/mmcblk0p2 rootfstype=ext4 rootwait`.

## Open Questions

- The exact helper implementation that opens the SD path in `external_files`
  mode has not been fully analyzed yet.
- It is not proven whether Stage 3 accepts a two-partition card.
- It is not proven whether Debian can be booted directly without the vendor
  initramfs or `/linuxrc`.
- It is not proven which stock runtime files are strictly minimal after Stage 3;
  the current working image intentionally keeps a broad runtime set.
