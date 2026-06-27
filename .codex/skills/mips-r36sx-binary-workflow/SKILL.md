---
name: mips-r36sx-binary-workflow
description: Analyze, patch, rebuild, disassemble, or verify MIPS/MIPSEL binaries and R36SX/SF3000 Linux-native artifacts including ELF executables, shared objects, driver.so interactions, MIPS assembly, toolchain flags, and device performance issues. Use when working with MIPS ABI, endian, relocations, GCC/binutils output, objdump/readelf traces, or R36SX native builds; always verify CPU/ABI/toolchain behavior against official MIPS, GNU, Linux, or vendor documentation before semantic changes.
---

# MIPS R36SX Binary Workflow

## Overview

Use this skill when working below the C/C++ source level on R36SX/SF3000 MIPS
artifacts, or when build flags, ABI details, ELF layout, disassembly, or device
performance are part of the task.

## Workflow

1. Identify the artifact: ELF executable, shared object, raw firmware block,
   disassembly listing, assembly source, generated object, or toolchain output.
2. Confirm architecture details before reasoning about code:
   - MIPS32 revision and ASEs.
   - Endianness; R36SX Pico-286 builds are normally little-endian MIPSEL.
   - ABI, relocation model, PIC/non-PIC, and dynamic/static linking.
3. Consult official documentation before semantic changes:
   - MIPS architecture manuals for instruction behavior, delay slots,
     exceptions, unaligned access, DSP ASE, and cache/TLB behavior.
   - GNU GCC/binutils manuals for compiler, assembler, linker, `objdump`,
     `readelf`, and strip behavior.
   - Linux kernel/uapi or libc documentation for syscalls, ABI, file I/O, and
     dynamic loader behavior.
   - Vendor SDK/toolchain documentation for SF3000/R36SX-specific build paths.
4. Use the repository's WSL/GCC path for normal MIPS Pico-286 builds.
5. Verify ELF headers, size, SHA256, and changed sections before replacing a
   deployed binary.
6. Log reusable toolchain findings or build results in the appropriate project
   document.

## Guardrails

- Do not assume x86-like behavior for delay slots, branch/load hazards,
  alignment, overflow, or unaligned memory access. Check MIPS documentation.
- Preserve little-endian MIPS32/Linux ABI expectations for R36SX native builds.
- Use WSL `mips-mti-linux-gnu-gcc`/`g++` for normal MIPS Pico-286 builds; treat
  Zig/Clang MIPS as fallback unless the user requests it.
- Do not strip debug/test binaries unless the task is producing a release or
  patch copy.
- Do not rebuild experimental MIPS DSP binaries during normal release work.
- Record exact compiler/linker flags when a binary will be used for device
  testing.

## Scripts

Use `scripts/elf_mips_summary.py` for a dependency-free ELF header check:

```powershell
python .codex\skills\mips-r36sx-binary-workflow\scripts\elf_mips_summary.py path\to\pico_286
```

Use `scripts/binary_fingerprint.py` to record size and SHA256:

```powershell
python .codex\skills\mips-r36sx-binary-workflow\scripts\binary_fingerprint.py path\to\pico_286
```

## References

- Read `references/official-docs.md` for the required documentation policy and
  source selection.
- Read `references/r36sx-mips-notes.md` for project-specific MIPS/R36SX build
  and verification notes.
- For Ghidra imports, decompiler exports, or generated disassembly, use the
  `ghidra-disassembly-workflow` skill as well and store durable outputs under
  `disasm/mips/` or `disasm/linux/` depending on the target.
