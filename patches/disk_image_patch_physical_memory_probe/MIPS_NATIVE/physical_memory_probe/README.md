# R36SX physical memory probe

A small standalone Linux/MIPSEL executable that runs from the SD card and
reads **total DDR capacity configured in the hardware controller**, including
memory reserved for AVP. Linux `MemTotal` is collected separately as context.

No firmware replacement, kernel module, NOR update or bootloader modification
is required. The program needs the stock root privileges and `/dev/mem`
access already used successfully by our CPU clock probe.

## Run on the console

Copy this folder from the prepared overlay to the SD card:

```text
patches/disk_image_patch_physical_memory_probe/MIPS_NATIVE/physical_memory_probe/
  -> SD:/MIPS_NATIVE/physical_memory_probe/
```

Launch `physical_memory_probe` in Tiny MC, or run in Shell:

```sh
/mnt/sdcard/MIPS_NATIVE/physical_memory_probe/physical_memory_probe
```

It exits automatically. Open the new report in Tiny MC's text viewer:

```text
MIPS_NATIVE/physical_memory_probe/diagnostics/physical_memory_XXXXXX.txt
```

The key line is:

```text
TOTAL DDR configured in hardware: 128 MiB (134217728 bytes)
```

That is an **example**, not a measurement of this console. If it says 256 MiB,
the controller is configured for 256 MiB even if Linux sees only 42 MiB.
Return the whole report, including raw register and chip-ID values.
Failure to access hardware is reported as UNKNOWN, never replaced with a
Linux or device-tree estimate. Reports have unique names and survive closed
launcher stdout pipes.

## What this establishes

The program reproduces stock Stage 3 `meminfo` at `0x89dabf90`:

- DDR configuration at MIPS uncached `0xb8801000`, physical `0x18801000`.
- Size in MiB: `16 << (register & 7)`.
- DDR generation: `2 + ((register >> 23) & 1)`.
- HC16xx chip family is checked with `/proc/cpuinfo` and the chip-ID word at
  physical `0x18800000`, high byte `0x16`.

Only the chip-ID and DDR configuration registers are read. `/dev/mem` is
opened `O_RDONLY | O_SYNC`, mapping has `PROT_READ`, and reads use volatile
32-bit accesses. Two DDR reads must agree. Register access runs in a child
with a three-second alarm and core dumps disabled. There are no physical RAM
write probes, MMIO writes, stress allocations or MTD writes.

This reads hardware configuration, **not the physical chip's identity or a
full memory-cell test**. Incorrect DDR initialization or address-window
aliasing could make configured and physically usable capacity differ. A
definitive alias/geometry test needs an early-boot environment with controlled
ownership of RAM, or hardware identification. Running it through `/dev/mem`
after Linux/AVP have started would not reserve the tested memory safely.

In particular, `malloc()` plus writes only tests memory allocated by Linux;
it cannot determine RAM reserved for AVP or hidden by the controller.

## Build and check

```sh
bash homebrew/physical_memory_probe/build_wsl.sh
mkdir -p homebrew/physical_memory_probe/.build
gcc -O2 -Wall -Wextra -Werror -std=gnu11 \
  homebrew/physical_memory_probe/physical_memory_probe.c \
  -o homebrew/physical_memory_probe/.build/physical_memory_probe_host
python3 homebrew/physical_memory_probe/test_probe.py \
  homebrew/physical_memory_probe/.build/physical_memory_probe_host
```

The existing WSL/Codescape GCC produces a static little-endian MIPS32r2/O32
soft-float executable. `--decode-register VALUE` checks a supplied value
offline without creating reports or accessing hardware. `--no-hardware`
checks report creation without reading registers. Neither is a physical
measurement.

## Sources

- Actual device boot dump `hardware_info/mtd-dump/mtd1_boot.bin`, unpacked by
  `disasm/mtd/unpack_stage3_lzma.py`; instruction and C exports:
  `disasm/mips/r36sx-linux-boot/stage3_ddr_info.s` and `.c`.
- `disasm/mips/r36sx-linux-boot/DDR_READOUT.md`: addresses, commands and limits.
- Older local HCRTOS source `components/applications/apps-bootloader/source/cmd/meminfo.c`
  reads DDR-init metadata from flash instead. It does **not** implement the
  live-register method in this newer binary; do not substitute it silently.
- [Linux MIPS address translation definitions](https://raw.githubusercontent.com/torvalds/linux/master/arch/mips/include/asm/addrspace.h).
- [Linux /dev/mem interface](https://man7.org/linux/man-pages/man4/mem.4.html).
- [GNU GCC MIPS options](https://gcc.gnu.org/onlinedocs/gcc/MIPS-Options.html).

Build/host/emulated checks do not establish successful hardware operation.
The current console's live DDR value is pending its first device run.
