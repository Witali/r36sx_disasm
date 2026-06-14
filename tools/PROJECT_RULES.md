# Project Rules

## Activity Logging Rule

Rule: every non-trivial project action must be logged in the repository.

This applies to:

- firmware/image analysis;
- downloaded repositories, tools, SDKs, archives, and binaries;
- disassembly/decompilation runs;
- build attempts, successful or failed;
- generated binaries, ROM/test files, configs, and patch overlays;
- edits to `disk_image`;
- changes intended for a real device test;
- security scans and verification steps.

The log must be written before the final task response whenever the action
changes files, creates artifacts, downloads content, runs a build, updates a
patch directory, or produces a fact that future work may depend on.

Use the most specific existing log location:

- `homebrew/<component>/BUILD_LOG.md` for component build history.
- `homebrew/<component>/BUILD_COMMANDS.md` for repeatable command-level build
  instructions.
- `disk_image_patch_<NNN>/MANIFEST.md` for SD/image overlay contents.
- root analysis files such as `FIRMWARE_FINDINGS.md`,
  `POWER_MANAGEMENT_ANALYSIS.md`, `RELATED_PROJECTS.md`, or
  `MOST_SIMILAR_REPOSITORY.md` for research findings.
- `tools/*.md` for toolchain, security, and project workflow rules.

Each log entry should include the useful parts of the action:

- date when relevant;
- purpose;
- source URL, repository, release, commit, or local input path;
- exact commands, scripts, or tool names used;
- generated or modified file paths;
- compiler/linker flags for builds;
- binary format, architecture, size, and SHA256 for generated binaries;
- scan results for downloaded or generated executable artifacts;
- known limitations, failed attempts, and assumptions.

For quick read-only exploration, it is acceptable to summarize the result in the
final response instead of creating a new log entry. If that exploration reveals
an important reusable fact, add it to the appropriate notes file.

Do not rely only on chat history for project state. If a future task would need
the information to reproduce, verify, or understand the work, put it in a file.

## Git Snapshot Rule

Rule: create a git commit whenever the repository has meaningful completed
changes.

Meaningful completed changes include:

- a working build or rebuilt binary;
- a new device-test patch directory;
- a new analysis/decompilation result that is tracked by git;
- changed project rules or documentation that future work depends on;
- a coherent fix after a device test result.

Prefer small, topic-focused commits. Do not wait until many unrelated changes
pile up.

Push cadence: after each successful commit, check how many local commits are
ahead of the remote branch. If the branch is ahead by three or more commits,
run `git push` before continuing with more project changes.

It is also fine to push sooner when the user explicitly asks for `git push`,
when a remote backup is important, or before a risky next step. Do not let a
long queue of local commits accumulate without pushing.

## Native App Exit Rule

Rule: native Tiny MC applications should use `Select + Start` held together as
the standard exit gesture.

This avoids accidental exits from a single `Select` press and gives native apps
a consistent escape hatch independent of app-specific controls. `Fn` may remain
as a secondary hardware escape when it is already supported or useful for device
testing, but new native app behavior should not depend on `Fn` as the only exit
path.

## Pico-286 Patch RTC Rule

Rule: whenever a rebuilt `homebrew/pico_286/pico_286` executable is copied into
`patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`, update the
same patch directory's `pico_286.conf` so `[rtc] rtc_start_time` is set to the
current local build/copy time in `YYYY-MM-DD HH:MM:SS` format.

This keeps device-test patches from booting DOS with a stale synthetic RTC
date after the console's Linux environment starts without real-time clock
hardware.

## Disk Image Editing Rule

Rule: use WSL `mtools` for FAT disk image inspection and edits.

Do not loop-mount FAT images for normal Pico-286 and patch workflow tasks.
Only use mounting when a task explicitly requires it and the target image,
partition offset, and write scope have been verified first.

## Codex Agent Files Rule

Rule: keep `AGENTS.md` and project-local `.codex/skills/` files in sync with
the repository's actual workflow.

When a repeated project practice becomes stable, such as a preferred build
command, disk-image workflow, patch-copy convention, or commit/push cadence,
add it to the appropriate agent file instead of relying on chat history. Keep
`AGENTS.md` short and immediately useful; put subsystem-specific details in a
project skill reference file.

When changing agent files, validate the skill structure when the local
validator is usable. If validation cannot run because a local dependency is
missing, inspect the frontmatter and generated metadata manually and mention
the validator limitation in the task result.

## Official Documentation Rule

Rule: for semantic changes to CPU emulation, assembly/binary patches, ABI
behavior, BIOS/DOS interfaces, hardware ports, or native MIPS build/toolchain
assumptions, consult official documentation for the target before changing
behavior.

Prefer vendor manuals, official source trees, ABI specifications, and toolchain
manuals over wiki/blog/forum summaries. If official documentation is unavailable
or incomplete, state that limitation and label any secondary source used. Record
the exact manual/source and section when the fact is likely to affect future
work.

## Ghidra Disassembly Artifact Rule

Rule: durable Ghidra-generated disassembly, decompiler output, symbol/function
exports, range dumps, and reverse-engineering notes belong under `disasm/`.

Use `ghidra_projects/` only for local Ghidra project databases and caches. Treat
`ghidra_exports/` as legacy scratch output; do not rely on it for files that
future work should read. When running Ghidra scripts, pass an explicit output
directory under `disasm/<type>/<target>/`, sorting first by executable or target
type such as `dos`, `mips`, `linux`, `BIOS`, or `firmware`.
