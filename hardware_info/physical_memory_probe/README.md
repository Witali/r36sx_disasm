# Live R36SX DDR capacity readout

Received 2026-10-18 after the user ran the SD-launched `physical_memory_probe`
on the console. Two complete 2,163-byte reports are preserved byte-for-byte in
`2026-10-18/`, with source paths and hashes in `manifest.json`. Both end with
`Done.` and agree on every hardware field.

| Measurement | Result |
| --- | --- |
| Chip-ID word at physical `0x18800000` | `0x1600ac01` |
| DDR configuration at physical `0x18801000` | `0x0d894084` |
| Capacity code, low three bits | `4` |
| Total capacity configured in DDR controller | **256 MiB / 268,435,456 bytes** |
| DDR generation, bit 23 plus two | **DDR3** |
| Linux `MemTotal` in both runs | **43164 KiB / 42.15 MiB** |
| Linux `MemAvailable` | 33496 / 33488 KiB |
| Kernel | 4.4.186-release, build **#21** |

The executable read back from the returned card has SHA256
`ab75c1b54a8a86ae1a0201ec337b06635234222edffef910f1e3bc537bab7f6a`,
matching the built, Defender-scanned and QEMU-tested version. Report dates on
FAT are 1979-12-31 and are not treated as reliable acquisition times; the date
above is the host-side receipt date.

## Interpretation and correction

The register is decoded exactly like stock Stage 3 `meminfo`:
`16 << (0x0d894084 & 7) = 16 << 4 = 256 MiB`.
The readout is independent of Linux's reservation and successfully worked
through stock `/dev/mem` without replacing firmware.

**Withdraw the inference that the smaller boot layout proves 128 MiB of
installed RAM.** The live controller is configured for 256 MiB while Linux is
still using the restricted layout. Earlier observations of kernel #21,
42 MiB MemTotal, and the Bubbles memory map establish a smaller Linux address
layout; they do not independently identify the physical RAM capacity.

This is not proof that all 256 MiB are physically backed, distinct and working.
A controller can be configured beyond the installed capacity, or the CPU's
address window can alias. Resolving that remaining distinction requires a
controlled early-boot test or hardware identification. The tool did neither.
Do not conclude that another 128 MiB is immediately usable, that all memory
outside Linux belongs to AVP, or that switching DTBs is sufficient.

The immediate conclusion is therefore **DDR3, controller configured for
256 MiB; Linux usable total 42.15 MiB; independently tested physical capacity
still unresolved**.

## Provenance and handling

- Original card directory:
  `E:/MIPS_NATIVE/physical_memory_probe/diagnostics/`.
- Copied only the two named reports into the repository with PowerShell
  `Copy-Item -LiteralPath`; source and destination SHA256 matched.
- Register formula and instruction evidence:
  `disasm/mips/r36sx-linux-boot/DDR_READOUT.md` and `stage3_ddr_info.s`.
- Tool source/build/limitations: `homebrew/physical_memory_probe/`.
- The host performed no SD writes during report recovery. No firmware,
  kernel, AVP, DTB, MMIO or Pico-286 configuration was changed.
