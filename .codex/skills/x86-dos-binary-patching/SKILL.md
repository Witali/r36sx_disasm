---
name: x86-dos-binary-patching
description: Edit, rebuild, patch, disassemble, or verify DOS x86 assembly and binary artifacts including .ASM, .COM, .EXE, BIOS ROM, boot sectors, option ROMs, Ghidra exports, and ndisasm listings. Use when preserving binary layout, offsets, segment bases, file size, checksums, or byte-identical rebuild behavior matters; always verify instruction/API behavior against official Intel, IBM PC/AT, Microsoft DOS, or FreeDOS documentation before semantic changes.
---

# x86 DOS Binary Patching

## Overview

Use this skill for fragile real-mode x86 assembly and binary patch work where a
small offset, segment, or size mistake can break a DOS program, BIOS ROM, boot
sector, or device driver.

## Workflow

1. Identify the binary format before editing: `.COM`, MZ `.EXE`, boot sector,
   BIOS ROM, option ROM, raw table, or disassembly listing.
2. Determine the load base and addressing model: `ORG 100h`, BIOS segment,
   boot `7C00h`, option ROM segment, far pointer, or file offset.
3. Consult official documentation before changing semantics:
   - Intel processor manuals for instruction flags, undefined behavior, prefix
     behavior, and exception behavior.
   - IBM PC/XT/AT technical references for BIOS data areas, interrupt vectors,
     option ROM conventions, and hardware ports.
   - Microsoft DOS or FreeDOS source/documentation for DOS APIs, redirector
     callbacks, SFT/SDA/DTA structures, and PSP behavior.
4. Make the smallest assembly/source change that preserves layout assumptions.
5. Rebuild with the repository's existing assembler/script, normally NASM for
   DOS tools.
6. Verify size, SHA256, and changed byte ranges with the scripts in this skill.
7. If the artifact is copied into a FAT image, use WSL `mtools` and verify the
   in-image file when practical.
8. Record important build or patch facts in the appropriate project log before
   the final response.

## Guardrails

- Do not assume flags or undefined opcodes from memory. Check the relevant
  official Intel generation manual for 8086, 80286, or 80386 behavior.
- Keep original file size unless the artifact format and all loaders/callers
  allow growth.
- Preserve `ORG`, alignment, padding, vectors, checksums, and table offsets.
- Treat Ghidra/ndisasm output as context, not source of truth.
- Use secondary references such as RBIL only after checking official docs or
  when official docs do not cover the behavior; label that as secondary.
- Do not overwrite generated `.COM` files in the patch root. Project DOS tools
  belong under `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/`.

## Scripts

Use `scripts/binary_diff.py` to compare old/new binaries:

```powershell
python .codex\skills\x86-dos-binary-patching\scripts\binary_diff.py old.bin new.bin
```

Use `scripts/check_fixed_size.py` to assert a rebuilt artifact did not grow or
shrink:

```powershell
python .codex\skills\x86-dos-binary-patching\scripts\check_fixed_size.py old.bin new.bin
```

## References

- Read `references/official-docs.md` for the required documentation policy and
  source selection.
- Read `references/formats.md` for format-specific checks and offset rules.
- For Ghidra imports, decompiler exports, or generated disassembly, use the
  `ghidra-disassembly-workflow` skill as well and store durable outputs under
  `disasm/dos/`, `disasm/BIOS/`, or another type-first `disasm/` folder.
