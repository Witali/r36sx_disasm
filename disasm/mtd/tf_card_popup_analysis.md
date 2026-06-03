# "Please Insert TF Card" Screen Analysis

Date: 2026-06-03.

## Image

The "Please Insert TF Card" image is in MTD2 `eromfs`:

```text
mtd2_eromfs.bin
  popup.bmp.gz       offset 0x0080, size 0x22ae
  batterylow.bmp.gz  offset 0x2360, size 0x15b6
```

In the full NOR dump, the same ROMFS starts at `0x6c000` in `mtd0_nor.bin`, so
the popup gzip payload is at MTD0 offset `0x6c080`.

`popup.bmp.gz` decompresses to a 640x480 32-bit BMP. It is the black screen with
the spinner and `Please Insert TF Card` text.

## DTB Clues

The DTB has an enabled node:

```dts
Unplug_card_display {
    status = "okay";
};
```

The nearby `panel` node carries the SD detect input:

```dts
sdio-det = <0x6d>;
```

The RTOS/MMC node defines the card controller and detect delay:

```dts
mmc {
    hcrtos-compatible = "hichip,dw-mshc";
    reg = <0x1884c000 0x2000>;
    card-detect-delay = <0xc8>;
    bus-width = <0x4>;
    cap-sd-highspeed;
    sd-uhs-sdr12;
    sd-uhs-sdr25;
};
```

The same DTB lists the required SD boot files:

```dts
external_files {
    path-prefix = "cubegm";
    part1-filename = "dtb.bin";
    part2-filename = "avp.uImage";
    part3-filename = "vmlinux.uImage";
    part4-filename = "xgame-logo.bmp";
};
```

## What MTD1 Shows

The already decompiled MTD1 first/stage2 code does not contain readable strings
such as `popup.bmp.gz`, `Please Insert TF Card`, `cubegm`, `FAT`, or `mmc`.

MTD1 stage2 decodes the next packed payload:

```text
source MTD offset: 0x5e48
packed length:    0x62f4f
decoded address:  0x89da0000
```

The key Ghidra call is:

```c
FUN_89eb2dcc(0x89eb2e48, 0x62f4f, 0, 0, &LAB_89da0000, 0, 0, auStack_10, 0x89eb0300);
```

The packed payload header begins with:

```text
5d 00 00 80 00 3c e3 10 00 00 00 00 00 00 00 69
```

This is an LZMA-Alone stream. The first 13 bytes define properties `0x5d`,
dictionary `0x00800000`, and unpacked size `0x0010e33c`.

Unpacking produced:

```text
stage3_89da0000.lzma  size 0x62f4f   sha256 378ae082fbb6c2b52a9cfeac3e8f6ebb4248a2436f4cadc41110b98c9cc79680
stage3_89da0000.bin   size 0x10e33c  sha256 b9a9d1ae074bcd68109161c1219fe98b8c4f7e3fd70045b394dd9aa4ab15981d
```

The reproducer script is `disasm/mtd/unpack_stage3_lzma.py`.

Stage2 decodes this LZMA stream through these functions:

```text
stage2_payload_header_parse
stage2_stream_copy_or_decode_loop
stage2_payload_decode_dispatch
FUN_89eb2dcc
```

## Found Display Logic

After unpacking stage3, the exact popup path is visible in the code based at
`0x89da0000`:

- `FUN_89da9474` resolves a boot resource first as an MTD partition, then via
  `/hcrtos/external_partitions`, then via `/hcrtos/external_files`.
- In external-files mode it reads `part-num`, `part%d-label`, `path-prefix`,
  and `part%d-filename`, then builds the SD path.
- In external-partitions mode it builds `/dev/mmcblk0p%d`.
- If the selected resource or partition is still unavailable, the function loops
  and retries. After a `2000` timer it prints `show popup` and calls
  `FUN_89da98ec(0)`.
- `FUN_89da98ec(0)` tries `eromfs`, `eromfs2`, and `eromfs3`.
- `FUN_89da97c0` mounts ROMFS and runs `showlogo` for `/eromfs/popup.bmp`,
  then fallback `/eromfs/popup.bmp.gz`, then `/eromfs/popup.hc`.

So the screen is not shown because the popup image itself is missing. It is a
wait/error indicator for an unavailable external SD boot resource.

## Conditions

High confidence: the popup appears when `FUN_89da9474` has found a resource
description but cannot get the actual resource:

- `/hcrtos/external_partitions` points to `part%d-label`, but
  `/dev/mmcblk0p%d` does not become available;
- `/hcrtos/external_files` points to `part%d-label`, `path-prefix`, and
  `part%d-filename`, but the corresponding SD file cannot be opened;
- the card is not physically detected or MMC init does not create the expected
  `/dev/mmcblk0p%d`;
- FAT/VFAT on the first partition is not readable early enough for the RTOS
  loader;
- one of the boot resources is missing or unreadable.

The main stage3 boot flow later requests `dtb`, `initramdisk`, `avp`, and
`linux`. Through `external_files`, those correspond to SD files such as
`dtb.bin`, `avp.uImage`, `vmlinux.uImage`, and `xgame-logo.bmp`.

## Why This Matters for Our Images

If the physical card is inserted but firmware still displays this screen, the
problem is not necessarily a missing individual FAT file. Possible causes are:

- the new MBR/partition geometry is rejected by the early loader;
- the loader expects one FAT partition with a known type or size;
- the second ext4 partition breaks a simple MBR/FAT parser;
- the modified `dtb.bin` is accepted at first but changes an early boot
  condition;
- the loader never reaches `external_files` loading and fails earlier during
  card/FAT initialization.

## Still Worth Checking

The low-level point where MMC/card-detect decides "no card" is deeper in the
MMC driver stack. Stage3 already contains `mmc`, `sdio`, `card-detect-delay`,
and `dw_mmc_hc` strings and code, but the popup branch found in the boot flow is
triggered at the external-resource availability layer.

The next technical step is to decompile the helpers called by `FUN_89da9474`:

```text
FUN_89da926c
FUN_89de0894
FUN_89ddc438
FUN_89ddc3c0
```

## Practical Image Tests

The safest test sequence is:

1. Original single-FAT geometry with a minimal visible marker file.
2. Single-FAT image with stock `cubegm`, `rootfs`, `/bin`, `/lib`, `/etc`, and
   `/usr`.
3. The same single-FAT image with our `MIPS_NATIVE/shell`.
4. Add a second partition only after the loader accepts the image.
5. Reintroduce modified `dtb.bin` with `root=/dev/mmcblk0p2` only after the
   second partition itself is known not to trigger the popup.
