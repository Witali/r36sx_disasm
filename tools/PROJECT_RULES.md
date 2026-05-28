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

Push less often than commit. Default cadence: push after about three local
commits, or sooner when the user explicitly asks for `git push`, when a remote
backup is important, or before a risky next step.

## Native App Exit Rule

Rule: native Tiny MC applications should use `Select + Start` held together as
the standard exit gesture.

This avoids accidental exits from a single `Select` press and gives native apps
a consistent escape hatch independent of app-specific controls. `Fn` may remain
as a secondary hardware escape when it is already supported or useful for device
testing, but new native app behavior should not depend on `Fn` as the only exit
path.
