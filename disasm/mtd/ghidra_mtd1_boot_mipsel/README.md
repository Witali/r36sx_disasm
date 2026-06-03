# Ghidra MTD1 boot decompile

This directory contains Ghidra output for `hardware_info/mtd-dump/mtd1_boot.bin`.
The dump is raw stripped MIPS boot code/data, not an ELF image, so it has no
section table or symbol table. It was imported as little-endian MIPS and analyzed
with two address profiles.

## Load profiles

| Profile | Source bytes | Import base | Code range | Reason |
| --- | --- | --- | --- | --- |
| `first_stage/` | file offset `0x0`, length `0x3000` | `0xbfe00000` | `0xbfe00800..0xbfe02e4f` | Early NOR/KSEG1 code. The entry checks the boot magic at `0xbfe00014` and copies from the flash alias around `0xafc02000` to `0xbfe02000`. |
| `stage2_ram/` | file offset `0x3000`, length `0x2e30` | `0x89eb0000` | `0x89eb0000..0x89eb2e2f` | RAM-resident second stage. The code starts with CP0/cache setup and then continues into loader/decode routines. |

The second profile is important. Loading the whole partition at one base address
mixes code that runs in NOR with code that is meant to be copied into RAM, which
makes the decompiler invent worse control flow.

## Generated files

The boot-chain notes from the Ghidra output are in `boot_chain_analysis.md`.

Each profile contains:

| File | Purpose |
| --- | --- |
| `decompiled_all.c` | C-like decompiler output with a short comment before each function. |
| `disassembly.s` | Linear disassembly with function headers and notes. |
| `functions.tsv` | Function address, generated/manual name, body range, and analysis note. |
| `symbols.tsv` | Ghidra symbols discovered or created during analysis. |
| `summary.txt` | Import profile, language, and memory block summary. |

## Manual annotations

The comments are best-effort reverse-engineering notes, not recovered source
symbols. The strongest current anchors are:

| Address | Name | Note |
| --- | --- | --- |
| `0xbfe00800` | `first_stage_entry_copy_and_init` | NOR entry, boot magic check, flash alias copy, low-level init calls. |
| `0xbfe01278` | `first_stage_spi_nor_command_path` | SPI/SF controller path around `0xb882e0c8`. |
| `0xbfe015d4` | `first_stage_peripheral_gate_for_usb_blocks` | Checks `0xb8844000` and `0xb8850000`, matching the DTS USB MMIO windows. |
| `0xbfe01648` | `first_stage_peripheral_reset_sequence` | Writes selected peripheral control/status registers after clock/reset gating. |
| `0xbfe02db4` | `first_stage_sfspi_transfer_setup` | Programs SPI/SF transfer registers and waits for status. |
| `0x89eb0000` | `stage2_entry_cp0_cache_and_handoff` | RAM stage2 entry, CP0/cache setup, stack/trampoline setup. |
| `0x89eb0130` | `stage2_cache_probe_and_invalidate` | Reads CP0 Config1 and performs MIPS cache operations over ways/sets. |
| `0x89eb032c` | `stage2_main_handoff_wrapper` | Wrapper before deeper loader/decode routines. |
| `0x89eb23c8` | `stage2_stream_copy_or_decode_loop` | Packed payload copy/decode loop candidate. |
| `0x89eb2a28` | `stage2_payload_header_parse` | Parses and validates packed payload header fields. |
| `0x89eb2b98` | `stage2_payload_decode_dispatch` | Higher-level packed payload decode dispatcher candidate. |

## Recreate

The exports were generated with `ghidra_scripts/ExportMtdBootDecompile.java`:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects mtd1_first_stage -import .\hardware_info\mtd-dump\mtd1_boot.bin -loader BinaryLoader -loader-baseAddr 0xbfe00000 -loader-fileOffset 0x0 -loader-length 0x3000 -processor MIPS:LE:32:default -cspec default -scriptPath .\ghidra_scripts -postScript ExportMtdBootDecompile.java .\disasm\mtd\ghidra_mtd1_boot_mipsel\first_stage first -deleteProject -overwrite
```

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects mtd1_stage2_ram -import .\hardware_info\mtd-dump\mtd1_boot.bin -loader BinaryLoader -loader-baseAddr 0x89eb0000 -loader-fileOffset 0x3000 -loader-length 0x2e30 -processor MIPS:LE:32:default -cspec default -scriptPath .\ghidra_scripts -postScript ExportMtdBootDecompile.java .\disasm\mtd\ghidra_mtd1_boot_mipsel\stage2_ram stage2 -deleteProject -overwrite
```

## Limitations

Some generated function boundaries are still guesses because the input is a raw
binary with no metadata. `first_stage/` has one Ghidra pcode warning near
`0xbfe008a8`; `stage2_ram/` has harmless end-of-range warnings where analysis
reaches just past the imported byte range. Function names beginning with `FUN_`
or `sub_` should still be treated as unclassified until more call-site evidence
or hardware behavior confirms them.
