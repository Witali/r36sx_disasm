# Codex Project Rules

This repository contains R36SX/SF3000 reverse-engineering notes, patch overlays,
and the active Pico-286 emulator port under `homebrew/pico_286`.

## Core Rules

- Start by checking `git status -sb`. The worktree may contain user edits; never
  revert or stage unrelated changes unless the user explicitly asks.
- Prefer `rg`/`rg --files` for searches and `apply_patch` for manual edits.
- Prefer small Python scripts for extra one-off processing when shell commands
  become awkward or error-prone.
- Keep completed work in focused commits. After every successful fix or
  meaningful documentation update, commit it. Push after every third local
  commit, or sooner when the user asks.
- Follow `tools/PROJECT_RULES.md`: log non-trivial builds, artifacts, image
  edits, analysis findings, and reusable facts in repository files.
- Follow `tools/SECURITY_RULES.md`: scan downloaded tools, archives,
  executables, scripts, and extracted directories before use.

## Pico-286 Workflow

- Use the project skill `.codex/skills/r36sx-pico286` for Pico-286/R36SX work.
- Treat `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286` as the user's
  active test copy. The user normally tests that patch copy, not `disk_image`.
- Use WSL/GCC for MIPS Pico-286 builds. The Zig/Clang MIPS path is fallback
  only.
- Use WSL `mtools` for FAT disk image edits. Do not loop-mount images unless a
  task explicitly needs it and the target/offset has been verified.
- Keep generated DOS test utilities under
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/`.
- When copying a rebuilt MIPS `pico_286` into the patch directory, update the
  same patch directory's `pico_286.conf` `[rtc] rtc_start_time` to the current
  local time.
- Do not stage the user's local patch `pico_286.conf` changes unless the user
  explicitly asks.

## Build Shortcuts

Windows debug build:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew\pico_286\build_pico_286_windows.ps1 -DebugLog
```

MIPS release build:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
```
