# HC16xx total DDR readout from the actual stock bootloader

Date: 2026-10-18. This extends the earlier memory-variant analysis with the
actual hardware readout used by the device's bootloader.

## Verified implementation

Stage 3 function `0x89dabf90` prints `DDR Info`, type, size and frequency.
Its size and type come from **live MMIO**, not Linux MemTotal, a DTB region or
the DDR-init metadata in flash:

```text
89dabfa4  lw    s1,0x1000(v0)       ; v0 = 0xb8800000
89dabfa8  lw    s2,0x1000(v0)
89dabfb0  srl   s1,s1,23
89dabfb4  andi  v0,s2,7
89dabfc0  li    s2,16
89dabfc4  andi  s1,s1,1
89dabfc8  sllv  s2,s2,v0
89dabfd8  addiu s1,s1,2             ; branch delay slot
```

Thus `size_MiB = 16 << (DDR_CONFIG & 7)` and
`DDR_type = 2 + ((DDR_CONFIG >> 23) & 1)`. Codes 3 and 4 mean 128 and 256 MiB.
Physical address `0x18801000` follows the Linux/MIPS `CPHYSADDR` mask
`0x1fffffff` applied to uncached address `0xb8801000`.

The same function checks chip-family byte `0xb8800003` when interpreting DDR
frequency; our tool reads the containing word at physical `0x18800000` for
the family check and does not implement frequency decoding.

Outputs:

- `stage3_ddr_info.c`: C-like decompile of the full function.
- `stage3_ddr_info.s`: 324-byte instruction range.
- All 81 instruction byte sequences were compared with the original raw input
  and matched. The size/type formula is independently visible in the assembly.

## Input and reproduction

Input `disasm/mtd/stage3_89da0000.bin`, 1,106,748 bytes, SHA256
`b9a9d1ae074bcd68109161c1219fe98b8c4f7e3fd70045b394dd9aa4ab15981d`.
Ghidra 12.0.4, raw `MIPS:LE:32:default:default`, base `0x89da0000`.

Use the direct Java launcher prefix documented in `README.md`, followed by:

```text
.\ghidra_projects r36sx_ddr_readout
-import .\disasm\mtd\stage3_89da0000.bin
-loader BinaryLoader -loader-baseAddr 0x89da0000
-processor MIPS:LE:32:default -noanalysis
-scriptPath .\ghidra_scripts
-postScript DecompileAddresses.java
C:\Work\r36sx_disasm\disasm\mips\r36sx-linux-boot\stage3_ddr_info.c 89dabf90
-postScript DumpDisasmRanges.java
C:\Work\r36sx_disasm\disasm\mips\r36sx-linux-boot\stage3_ddr_info.s 89dabf90 0x144
```

The initial exploratory export targeted an interior address, `0x89dac020`.
It was replaced by the complete function in a fresh local project. Only the
corrected full export is retained. Trailing whitespace was normalized.

## Older SDK meminfo is different

The retained HCRTOS source mirror at commit
`1dd13c6ddc034313c0c3b9a31a123449f7baf110`,
`components/applications/apps-bootloader/source/cmd/meminfo.c`, searches the
first 8 KiB of flash for magic `0x0df0adba` and reads DDR-init build metadata.
That older implementation must not be presented as a physical measurement.

In the saved `hardware_info/mtd-dump/mtd0_nor.bin`, the marker is at `0x8d4`,
followed by date `20220907` and packed DDR descriptor `0x3100042a`: DDR3,
**256 MiB**, 1066 MHz. This is metadata describing the generic initialization
image. The same boot image contains a runtime 128-MiB alias probe and alternate
filenames. The metadata alone therefore cannot establish installed capacity.

This reinforces the need to read the **current controller register** from the
live device before making a stronger 128/256 MiB claim.

## Scope of the new diagnostic

`homebrew/physical_memory_probe/` implements this live-register method as a
static MIPS/Linux executable launched from SD. It does not require a kernel,
AVP, NOR, DTB or boot-script update. The prepared overlay is
`patches/disk_image_patch_physical_memory_probe/`.

Hardware configuration is more direct evidence than a Linux reservation, but
still does not independently identify the silicon or prove that every mapped
byte is backed by a distinct working cell. A wrongly configured controller or
an address-window alias can require an early-boot test or chip identification
to resolve. Never run the bootloader's destructive alias probe after Linux and
AVP own those addresses. The program does no such writes.

Current hardware result, 2026-10-18: **two successful device runs** both read
chip word `0x1600ac01` and DDR configuration `0x0d894084`, which decodes to
**DDR3, 256 MiB configured capacity**. Linux MemTotal remains 42.15 MiB on
kernel build #21. Raw reports, hashes and interpretation are preserved in
`hardware_info/physical_memory_probe/`.

The earlier inference of 128 MiB installed from the smaller layout is withdrawn.
The size field confirms the controller's configuration, not all installed
cells or lack of aliasing. No firmware update or physical-memory write test
was performed. No tools were downloaded.

## Physical RAM verification: privilege versus ownership

Follow-up inspection, 2026-10-18: elevated privileges can permit physical
access, but do not establish that a region is safe to overwrite. The returned
probe reports already demonstrate access to the two MMIO registers through
stock `/dev/mem`. This does not establish that the kernel permits every RAM
range to be mapped. A kernel module could provide accesses that a userspace
mapping cannot, but neither method reserves memory used by AVP or DMA.

To distinguish independent RAM from an address alias, a diagnostic needs
controlled writes and readback at multiple addresses, checking whether a write
at one address changes another. Merely reading all addresses, inspecting zeroes,
or allocating Linux memory cannot prove total installed capacity. A sparse
address-alias test and a test of every memory cell are different claims.

The boot sequence matters on this board. The existing stock Stage 3 export
`disasm/mtd/stage3_89da0000_forced_decompile.c`, lines 569-590, loads and boots
the AVP resource, checks `0xb880006b == 2`, then loads and boots Linux. The local
HCRTOS mirror corroborates this sequence in
`components/applications/apps-bootloader/source/main.c`, lines 346-381.
Its `common/bootm_os.c` also distinguishes the standalone application handoff
from the Linux entry. The mirror is supporting evidence, not the exact stock
source revision. **Replacing only the SD Linux image does not ensure that AVP
has stopped or that its RAM is available for testing.**

The appropriate design is a standalone early-boot diagnostic, analogous to a
bootloader memory test, with these prerequisites:

- Verify an SD execution entry before the normal AVP handoff, or a documented
  way to park the other core and stop all memory-writing engines. SD resource
  overrides are established; a custom diagnostic's pre-AVP handoff has not yet
  been implemented or tested. No NOR change is intrinsically required by this
  design, but the SD-only execution path still needs validation.
- Protect the diagnostic's code, stack, exception state, bootloader allocations,
  and result storage. A guessed high address is not sufficient: an alias can
  overwrite a protected lower address. Reset-time state and active DMA must
  also be checked; running before Linux alone is insufficient.
- Account for MIPS caching, ordering, exceptions and watchdog behavior. A cached
  readback must not be mistaken for a successful physical RAM write. Do not
  change DDR geometry merely to make a larger range appear accessible.
- Start with an address-independence test of the candidate 128/256-MiB mapping.
  Only after establishing the mapping and exclusions should a wider pattern
  test report coverage and failures. If the controller's current window hides
  installed cells, a CPU test of that window alone cannot identify those cells.
- State the excluded bytes in the report. A test executing from DDR cannot
  overwrite its own live footprint; complete coverage requires a verified
  relocation strategy or a suitable independent execution memory. Availability
  of such memory on this device has not been established here.

No ready-made `mtest`/RAM-capacity test was found by `rg` in the inspected
bootloader, kernel and command source directories of the retained mirror.
Generic U-Boot documentation describes the desired write/readback mechanism;
it is not evidence that this stock firmware includes the U-Boot `mtest` command.
This follow-up inspected existing sources and exports only. No RAM-write
diagnostic was built, executed, copied to SD or represented as device-tested.

References checked:

- Stock bootloader binary and instructions listed above: direct vendor
  implementation for this device, rather than a generic DDR register guess.
- [Linux MIPS address definitions](https://raw.githubusercontent.com/torvalds/linux/master/arch/mips/include/asm/addrspace.h).
- [Linux /dev/mem interface and access restrictions](https://man7.org/linux/man-pages/man4/mem.4.html).
- [U-Boot mtest documentation](https://docs.u-boot.org/en/latest/usage/cmd/mtest.html):
  write/readback and address-interaction checks, used as a design reference.
