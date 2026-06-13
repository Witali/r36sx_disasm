---
name: r36sx-pico286
description: Work on the R36SX Pico-286 emulator port and patch workflow. Use when Codex edits or investigates homebrew/pico_286, r36sx_port, Pico-286 CPU/VGA/BIOS/disk/input/audio/hostrpc code, DOS test utilities, patch disk images, MIPS/Windows builds, or patch files under patches/disk_image_patch_pico_286.
---

# R36SX Pico-286

## Overview

Use this skill to keep Pico-286 emulator work aligned with the project's local
build, patch, disk-image, and commit rules. Read
`references/workflows.md` when you need concrete commands, paths, or subsystem
notes.

## First Steps

- Check `git status -sb` before editing. Preserve unrelated user changes.
- Search with `rg`/`rg --files`.
- Inspect the relevant existing subsystem before choosing an implementation
  style.
- Use `apply_patch` for manual edits.
- Prefer Python scripts for extra processing when a shell pipeline would become
  fragile.

## Common Workflows

- For Windows debugging, build with
  `homebrew\pico_286\build_pico_286_windows.ps1 -DebugLog`.
- For MIPS/device builds, use the WSL/GCC path:
  `homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip`.
- When updating the active patch copy, remember the patch directory is
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286`.
- Use WSL `mtools` for FAT image edits.
- Keep generated DOS test `.COM` files under the patch `tools` directory.
- Commit each successful fix or meaningful rule/documentation update; push when
  the branch is ahead by three commits or when the user asks.

## Guardrails

- Do not stage user-local `pico_286.conf` changes in the patch copy unless the
  user explicitly asks.
- Do not edit `disk_image` when the user says they are testing only the patch
  copy.
- Do not rebuild or copy experimental MIPS DSP binaries during normal Pico-286
  release builds.
- Do not reintroduce manual disk CHS config; infer geometry at mount time.
- Preserve lower CPU model hot paths: 8086 code should not pay for 286/386
  checks, and 286 code should not pay for 386 checks.

## References

- Read `references/workflows.md` for exact commands, paths, and subsystem notes.
- Read `tools/PROJECT_RULES.md` for logging, git, and RTC patch-copy rules.
- Read `tools/SECURITY_RULES.md` before using downloaded tools or binaries.
- For DOS x86 assembly, BIOS ROM, `.COM`, `.EXE`, boot-sector, or disassembly
  patch work, use the `x86-dos-binary-patching` skill as well.
- For MIPS/MIPSEL ELF, toolchain, ABI, disassembly, or R36SX native binary
  work, use the `mips-r36sx-binary-workflow` skill as well.
- For Ghidra imports, headless analysis, decompiler exports, or generated
  disassembly, use the `ghidra-disassembly-workflow` skill as well and keep
  durable outputs under `disasm/`.
