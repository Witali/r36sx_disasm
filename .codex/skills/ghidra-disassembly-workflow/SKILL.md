---
name: ghidra-disassembly-workflow
description: Import, analyze, decompile, export, or inspect binaries with Ghidra in this repository, including headless analyzeHeadless runs, Java Ghidra scripts, Ghidra projects, decompiler output, disassembly ranges, function/symbol exports, and reverse-engineering notes. Use when Codex works with ghidra_12.0.4_PUBLIC, ghidra_projects, ghidra_scripts, Ghidra exports, or generated disassembly/decompilation artifacts; store generated disassembled files under disasm/ and verify target semantics against official CPU, ABI, file-format, OS, or vendor documentation before drawing conclusions.
---

# Ghidra Disassembly Workflow

## Overview

Use this skill for Ghidra-based reverse engineering in this repository. Keep
Ghidra projects and caches separate from durable analysis artifacts, and put
all generated disassembly/decompilation files under `disasm/`.

## Workflow

1. Check the input artifact and its origin before import. Keep raw inputs in
   their source location; do not move original firmware, disk-image, DOS, or ELF
   files into `disasm/`.
2. Choose a `disasm/` output directory by executable/target type first, then
   target name, for example:
   - `disasm/dos/<program>/`
   - `disasm/mips/<binary-or-firmware>/`
   - `disasm/linux/<binary-or-library>/`
   - `disasm/BIOS/<rom-name>/`
   - `disasm/firmware/<image-or-partition>/`
3. Use `ghidra_projects/` only for local Ghidra project databases. They are
   workspace-local generated state and are ignored by git.
4. Use `ghidra_scripts/` for reusable Java scripts. Existing scripts include
   `ExportDisasmAndDecompile.java`, `DumpDisasmRanges.java`, and
   `DecompileAddresses.java`.
5. When running `analyzeHeadless`, pass an explicit script output path under
   `disasm/`. Do not rely on script fallbacks such as `ghidra_exports/`.
6. Verify conclusions against official documentation for the target CPU, ABI,
   file format, OS API, BIOS/DOS interface, or hardware component before making
   semantic claims or patches.
7. Summarize important findings in a durable note under `disasm/` or the
   relevant project analysis document.

## Headless Pattern

Use the local bundled Ghidra install when present:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat `
  .\ghidra_projects project_name `
  -import path\to\binary `
  -scriptPath .\ghidra_scripts `
  -postScript ExportDisasmAndDecompile.java disasm\category\target
```

If a project already exists, use the appropriate Ghidra headless option for
that project state and be careful not to overwrite unrelated analysis.

## Guardrails

- Store durable generated disassembly, decompiler output, symbol tables, range
  dumps, and notes under `disasm/`.
- Do not store durable outputs under `ghidra_exports/`; that path is ignored and
  should be treated as legacy scratch output.
- Do not commit `ghidra_projects/` or the local Ghidra installation.
- Do not treat Ghidra decompiler output as source of truth. Cross-check control
  flow, calling conventions, memory maps, and instruction behavior.
- For x86/DOS/BIOS targets, use `x86-dos-binary-patching` as well.
- For MIPS/MIPSEL ELF/native targets, use `mips-r36sx-binary-workflow` as well.

## Scripts

Use `scripts/make_disasm_dir.py` to create and print a normalized output
directory inside `disasm/`:

```powershell
python .codex\skills\ghidra-disassembly-workflow\scripts\make_disasm_dir.py dos spfix63
```

## References

- Read `references/workflow.md` for output layout and command details.
- Read `references/official-docs.md` for documentation/source requirements.
