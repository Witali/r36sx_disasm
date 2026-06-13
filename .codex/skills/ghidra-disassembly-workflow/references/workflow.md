# Ghidra Workflow Details

## Output Layout

Durable generated files belong under `disasm/`. Keep raw inputs in their
original source tree and keep Ghidra project databases in `ghidra_projects/`.

Sort first by executable/target type, not by where the file was found.
Recommended output roots:

- DOS programs: `disasm/dos/<program>/`
- MIPS raw firmware, MIPS bare/native artifacts, or MIPS-focused reverse
  engineering: `disasm/mips/<binary-or-firmware>/`
- Linux executables and shared libraries, including Linux/MIPSEL ELF files:
  `disasm/linux/<binary-or-library>/`
- BIOS and option ROMs: `disasm/BIOS/<rom-name>/`
- Firmware/container analysis that is not better described by executable type:
  `disasm/firmware/<image-or-partition>/`

Historical folders such as `disasm/disk_image/mips_elf/` may exist, but new
Ghidra exports should prefer the type-first layout above.

For each target, prefer a small `README.md` or findings note in the output
folder that records:

- original input path and SHA256 when useful;
- Ghidra version;
- language/compiler spec chosen by Ghidra;
- headless command or script used;
- analysis assumptions and unresolved questions.

## Existing Scripts

Reusable Ghidra scripts live in `ghidra_scripts/`:

- `ExportDisasmAndDecompile.java`: exports summary, functions, symbols,
  disassembly, and decompiler output.
- `DumpDisasmRanges.java`: dumps selected address ranges.
- `DecompileAddresses.java`: decompiles selected addresses.
- `ExportMtdBootDecompile.java`: MTD boot-specific export helper.
- `PowerRefsAndDecompile.java`: reference/decompile helper used by power
  analysis.

When a script accepts an output directory argument, pass a path under `disasm/`.
Several older scripts have fallback output under `ghidra_exports/`; avoid that
fallback for durable work.

## Verification

- Compare Ghidra's detected language, image base, and memory blocks with the
  source artifact's expected architecture and loader behavior.
- For raw binaries, explicitly reason about base address and load map before
  trusting labels or cross-references.
- For ELF/PE/MZ/ROM formats, inspect headers with independent tools when
  possible.
- Record places where Ghidra guessed wrong or where manual retyping was needed.
