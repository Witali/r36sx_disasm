# Анализ цепочки загрузки MTD1 boot

Этот файл описывает, что удалось восстановить по Ghidra-дизассемблированию
`hardware_info/mtd-dump/mtd1_boot.bin`, и где именно заканчиваются доказательства
из MTD-кода.

## Короткий вывод

Ghidra показывает не прямую загрузку `cubegm/vmlinux.uImage`, а более раннюю
цепочку:

```text
SPI NOR / mtd1_boot.bin
  -> first_stage @ 0xbfe00800
  -> stage2 copied from NOR alias 0xafc03000 to RAM alias 0xa9eb0000
  -> stage2 runs as cached alias 0x89eb0000
  -> stage2 decodes a packed blob from loaded MTD data into 0x89da0000
  -> stage2 jumps to 0x89da0000
  -> the 0x89da0000 stage is the likely SD/RTOS loader
  -> SD files from DTS external_files are used:
       cubegm/dtb.bin
       cubegm/avp.uImage
       cubegm/vmlinux.uImage
       cubegm/xgame-logo.bmp
  -> Linux uImage loads at 0x80000000 and enters at 0x803337c0
```

So the currently decompiled MTD first/stage2 code proves the handoff to a third
stage at `0x89da0000`. The exact FAT parser and the exact call that opens
`cubegm/vmlinux.uImage` are not present in the current first/stage2 Ghidra output.

## Ghidra load profiles

The same raw MTD file had to be imported twice:

| Profile | File bytes | Ghidra base | Meaning |
| --- | --- | --- | --- |
| `first_stage/` | offset `0x0`, length `0x3000` | `0xbfe00000` | Early NOR/KSEG1 boot code. |
| `stage2_ram/` | offset `0x3000`, length `0x2e30` | `0x89eb0000` | RAM-resident second-stage code. |

The second profile is required because the bytes at file offset `0x3000` are not
meant to execute at `0xbfe03000`; they are copied into RAM and executed through
MIPS cached/uncached aliases.

## First stage: NOR to RAM stage2

Entry:

```text
first_stage_entry_copy_and_init @ 0xbfe00800
```

Relevant Ghidra files:

```text
first_stage/decompiled_all.c
first_stage/disassembly.s
first_stage/functions.tsv
```

The first-stage header fields at MTD file offset `0x20` decode as little-endian
words:

| File offset | Raw bytes as word | Meaning inferred from code |
| --- | --- | --- |
| `0x20` | `0x00065da0` | Number of bytes to load/copy. |
| `0x24` | `0xa9eb0000` | Destination RAM address, uncached alias. |
| `0x28` | `0xafc03000` | Source NOR/flash alias, low bits indicate flash offset `0x3000`. |
| `0x2c` | `0xa9eb0000` | Entry pointer written to `DAT_bfe02e90`. |

`FUN_bfe011a4` copies the entry pointer:

```c
DAT_bfe02e90 = DAT_bfe0002c;
```

Then, when the header magic is valid, it calls:

```c
FUN_bfe029c8(DAT_bfe00020, DAT_bfe00024, DAT_bfe00028, DAT_bfe00040 & 0xff);
```

With the decoded header this is effectively:

```text
FUN_bfe029c8(0x65da0, 0xa9eb0000, 0xafc03000, flags)
```

`FUN_bfe029c8` uses the SPI/SF helper path (`FUN_bfe02604`, `FUN_bfe0284c`,
`FUN_bfe028b0`) to read from flash into RAM in block-sized chunks. In other
words, first stage loads the stage2 region plus its packed payload from MTD/NOR
offset `0x3000` into RAM at `0xa9eb0000`.

The final dynamic handoff is:

```asm
bfe00e30: lw  t9,0x2e90(v0)
bfe00e34: jr  t9
```

Because `DAT_bfe02e90` was filled from header field `0x2c`, this jumps to
`0xa9eb0000`. That is the uncached alias of the same code Ghidra imported as
`0x89eb0000`.

## Stage2: RAM decode to 0x89da0000

Entry:

```text
stage2_entry_cp0_cache_and_handoff @ 0x89eb0000
```

The first instructions save boot arguments, edit CP0 config/cache state, clear a
small state area around `0x89f15da0`, set a stack, and jump to a wrapper.

Important transition in `stage2_ram/disassembly.s`:

```asm
89eb00dc: lui   ra,0x89eb
89eb00e0: addiu ra,ra,0xf4
89eb00e4: lui   k0,0x89eb
89eb00e8: addiu k0,k0,0x32c
89eb00ec: jr    k0

89eb00f4: or    a0,s0,zero
89eb00f8: or    a1,s1,zero
89eb00fc: or    a2,s2,zero
89eb0100: or    a3,s3,zero
89eb0104: lui   k0,0x89da
89eb0108: jr    k0
```

This means stage2 calls `stage2_main_handoff_wrapper` at `0x89eb032c` with
return address `0x89eb00f4`. When the wrapper returns, execution immediately
jumps to `0x89da0000`.

The wrapper prepares heap/state bounds:

```c
_DAT_89f15da8 = param_1;
_DAT_89f15da4 = param_1 + 0x400000;
```

Then it decodes a packed blob:

```c
FUN_89eb2dcc(0x89eb2e48, 0x62f4f, 0, 0, &LAB_89da0000, 0, 0, auStack_10, 0x89eb0300);
FUN_89eb0298(&LAB_89da0000, auStack_10[0], cache_line_size);
```

`FUN_89eb0298` performs cache operations over the decoded output range. The line
size is selected as `0x10` or `0x20` depending on the SoC ID at `0xb8800000`.

The source pointer `0x89eb2e48` maps back into the first-stage-loaded MTD copy:

```text
stage2 Ghidra base         = 0x89eb0000
stage2 file offset         = 0x3000
source virtual offset      = 0x2e48
source MTD file offset     = 0x3000 + 0x2e48 = 0x5e48
payload length             = 0x62f4f
payload end in mtd1        = 0x5e48 + 0x62f4f = 0x68d97
mtd1_boot.bin size         = 0x6c000
```

So the packed third-stage payload is inside `mtd1_boot.bin`, from approximately
`0x5e48` through `0x68d97`.

The payload helpers are named in the export as:

| Address | Name | Role |
| --- | --- | --- |
| `0x89eb2a28` | `stage2_payload_header_parse` | Parses a small packed-payload header. |
| `0x89eb23a4` | `stage2_stream_context_prepare` | Prepares stream/decode context. |
| `0x89eb23c8` | `stage2_stream_copy_or_decode_loop` | Copies or decodes packed payload data. |
| `0x89eb2890` | `stage2_stream_context_release` | Releases stream/decode state. |
| `0x89eb2b98` | `stage2_payload_decode_dispatch` | Dispatches the packed-payload decode path. |
| `0x89eb2dcc` | `FUN_89eb2dcc` | Thin wrapper around size probe/decode into the output address. |

The decoded output address is `0x89da0000`. That is the next executable stage.

## Why this is not directly the Linux kernel

The MTD dump does not contain obvious Linux boot artifacts:

```text
uImage magic 0x27051956: not found in mtd1_boot.bin
gzip magic 0x1f8b:       not found in mtd1_boot.bin
DTB magic 0xd00dfeed:    not found in mtd1_boot.bin
strings cubegm/FAT/mmc:  not found in mtd1_boot.bin as useful paths
```

The stage2 output address `0x89da0000` also does not match the Linux uImage
runtime addresses from the SD card. The SD Linux image header says:

```text
disk_image/cubegm/vmlinux.uImage
  Load Address: 0x80000000
  Entry Point:  0x803337c0
```

The SD AVP/RTOS image header says:

```text
disk_image/cubegm/avp.uImage
  Load Address: 0x8bda4000
  Entry Point:  0x8bda4000
```

Therefore the code currently decoded from MTD reaches an intermediate loader at
`0x89da0000`; it is not yet the final Linux kernel entry.

## Evidence for SD Linux loading

The evidence that Linux is loaded from SD comes from DTS, SD files, and runtime
captures rather than direct strings in first/stage2 MTD code.

The DTS RTOS `external_files` block names the SD files:

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

The same DTS area defines the RTOS MMC controller used to find those files:

```dts
mmc {
    hcrtos-compatible = "hichip,dw-mshc";
    reg = <0x1884c000 0x2000>;
    status = "okay";
    bus-width = <0x4>;
    cap-sd-highspeed;
    sd-uhs-sdr12;
    sd-uhs-sdr25;
};
```

The physical SD/MMC base `0x1884c000` corresponds to KSEG1-style MMIO address
`0xb884c000`. The first-stage Ghidra output has several `0xb884...` MMIO regions
classified as likely USB/MMC-side peripheral initialization, but the exact SD
read and FAT path parser are not proven in first/stage2.

Runtime captures after Linux boot show:

```text
root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 earlycon= no_console_suspend noirqdebug
```

and:

```text
/dev/mmcblk0p1 /media/mmc vfat ...
/dev/mmcblk0p1 /mnt/sdcard vfat ...
/dev/mmcblk0p1 /lib vfat ...
/dev/mmcblk0p1 /usr vfat ...
/dev/mmcblk0p1 /bin vfat ...
/dev/mmcblk0p1 /sbin vfat ...
/dev/mmcblk0p1 /etc vfat ...
```

So Linux itself starts with a RAM root, and early userspace mounts the SD card
as VFAT and overlays the practical userspace directories from `/dev/mmcblk0p1`.

## Current confidence

High confidence:

- `first_stage` starts from NOR/KSEG1 at `0xbfe00800`.
- The first-stage header points to a load from `0xafc03000` to `0xa9eb0000`.
- `0xa9eb0000` is the uncached alias of the stage2 code imported as
  `0x89eb0000`.
- stage2 decodes a packed payload from MTD offset `0x5e48` into `0x89da0000`.
- stage2 jumps to `0x89da0000`.
- Linux uImage on SD has load `0x80000000` and entry `0x803337c0`.

Medium confidence:

- The `0x89da0000` payload is the loader/RTOS-side code that consumes DTS
  `external_files` and reads `cubegm/*` from SD.
- `0xb884...` first-stage MMIO init includes the SD/MMC-side peripheral path,
  but it also overlaps nearby USB/peripheral windows.

Not yet proven from current files:

- The exact function that parses FAT/VFAT.
- The exact function that opens `cubegm/vmlinux.uImage`.
- The exact branch from the `0x89da0000` stage to Linux entry `0x803337c0`.

## Next useful step

Extract or emulate-decode the packed payload from `mtd1_boot.bin[0x5e48..0x68d97]`
using the stage2 decode routine, then import the decoded bytes at `0x89da0000`
in Ghidra. That should expose the SD/FAT loader code and likely the
`external_files` handling path.
