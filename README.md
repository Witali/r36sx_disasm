# R36SX / SF3000 Firmware Reverse Engineering Notes

This repository collects notes, experiments, build artifacts, and patch sets from
an investigation of an R36SX / GB350 / SF3000-like handheld console firmware.

The work is focused on understanding the stock SD-card layout, identifying the
hardware and software platform, reverse engineering the game launcher, and
building small homebrew libretro-style modules for the stock `cubegm/rkgame`
environment.

## Current Findings

The inspected image is not a single monolithic firmware blob. It looks like the
contents of a FAT/SD partition plus an extracted or companion `rootfs` tree.

Important platform facts:

- Main userspace architecture: ELF32 little-endian MIPS, O32 ABI.
- CPU string from the device tree: `MIPS 74Kc`.
- Board/platform strings: `Hichip HC16xx`, `Hichip,1600`, `hc1600a@dbE3100v20`.
- Root filesystem base: Buildroot `2021.05-rc2`.
- Game environment: `cubegm`, launched through `icube`, with `rkgame` loading
  emulator cores from `cubegm/cores`.

The most important summary file is:

- [`FIRMWARE_FINDINGS.md`](FIRMWARE_FINDINGS.md)

## Hidden Boot Artifacts

Several files in `cubegm` are intentionally named like harmless Windows files,
but they are actually boot artifacts:

- `cubegm/advapi32.dll` is a U-Boot legacy `uImage` named `vmlinux`.
- `cubegm/ApplicationFrame.dll` is a U-Boot legacy `uImage` named `avp`.
- `cubegm/Bubbles.scr` is a DTB/FDT device tree, not a screensaver.

This appears to be a camouflage scheme for boot components stored on a
FAT-visible partition.

## Launcher And Emulator Core Model

The stock launcher uses a libretro-like model:

- `cubegm/rkgame` is the main game frontend and emulator core loader.
- `cubegm/cores/config.xml` maps file extensions to emulator cores.
- `cubegm/cores/filelist.xml` can override the core for specific files.
- Emulator modules are MIPS32 little-endian shared objects.
- Stock cores export regular `retro_*` functions plus launcher-specific symbols
  such as `check_encrypty` / `CheckEncrypty`.

The safest modification path found so far is to leave the vendor kernel, DTB,
AVP, rootfs, and hardware drivers in place, and add or replace userspace
components inside `cubegm`.

## Reverse Engineering Work

Ghidra 12.0.4 was used to inspect MIPS ELF binaries and extracted kernel
payloads. The local Ghidra installation, projects, and generated exports are
ignored by git because they are large.

Important analyzed binaries include:

- `cubegm/rkgame`: main game launcher and core loader.
- `cubegm/icube`: small launcher that starts `rkgame`.
- `cubegm/driver.so`: helper library used by the launcher.
- `cubegm/MyExecutable`: hardware helper for display, input, and backlight.
- `rootfs/usr/bin/hcprojector`: stock projector application.
- `rootfs/usr/bin/cubevol`: power, input, and system controller.
- `rootfs/lib/libc.so.0`: uClibc runtime used to inspect `usleep`.
- `cubegm/cores/libemu_tgbdual.so`: simpler stock core used as an ABI reference.

Generated exports are stored locally under `ghidra_exports/` when present. They
are intentionally not tracked.

## Power Management Research

The game side does not appear to use direct CPU halt, MIPS `wait`, or cpufreq
control in the normal frame loop.

What was found:

- `rkgame` and related userspace binaries use normal delays such as `usleep`.
- The common MIPS `wait` instruction encoding was not found in the main game
  binaries or stock emulator cores that were scanned.
- Backlight and MIPI/LCD power toggling exists in helper code.
- Real standby functionality appears to live in the projector/system side via
  `/dev/standby`, HUDI APIs, `hcprojector`, `cubevol`, and HCRTOS examples.

Detailed notes:

- [`POWER_MANAGEMENT_ANALYSIS.md`](POWER_MANAGEMENT_ANALYSIS.md)

## Related Public Projects

The closest public reverse engineering reference found so far is:

- [`goph-R/SF3000-RE`](https://github.com/goph-R/SF3000-RE)

It matches the MIPS32 little-endian platform, Buildroot version, `rootfs` plus
`cubegm` layout, and the practical approach of running custom MIPS code through
the stock environment.

Other useful links and adjacent projects are collected in:

- [`RELATED_PROJECTS.md`](RELATED_PROJECTS.md)
- [`MOST_SIMILAR_REPOSITORY.md`](MOST_SIMILAR_REPOSITORY.md)

## Local Tooling

The repository keeps small notes and scripts under `tools/`, while downloaded
toolchains and large SDK archives are ignored.

Important local notes:

- [`LOCAL_EMULATION.md`](LOCAL_EMULATION.md)
- [`ZX_SPECTRUM_EMULATOR_CANDIDATES.md`](ZX_SPECTRUM_EMULATOR_CANDIDATES.md)
- [`tools/PROJECT_RULES.md`](tools/PROJECT_RULES.md)
- [`tools/SF3000_TOOLCHAIN.md`](tools/SF3000_TOOLCHAIN.md)
- [`tools/SECURITY_RULES.md`](tools/SECURITY_RULES.md)
- [`tools/scan-download.ps1`](tools/scan-download.ps1)

A matching SF3000 MIPS SDK was found, but its compiler binaries are Linux
x86_64 ELF programs. On Windows, Zig was used as a practical cross-compiler for
the current homebrew experiments. The SDK should be usable from WSL/Linux after
proper extraction and relocation.

Security rule for this project: every downloaded tool, archive, SDK, extracted
tool directory, or executable bundle must be scanned before use. The local scan
helper is `tools/scan-download.ps1`.

Logging rule for this project: every non-trivial analysis, build, download,
generated artifact, device-test patch, and verification step must be recorded in
the appropriate notes, build log, command log, or patch manifest.

## Homebrew Experiments

### Button Demo Core

The first working homebrew experiment is a tiny libretro-style core:

- Source: [`homebrew/libretro_button_demo/button_demo.c`](homebrew/libretro_button_demo/button_demo.c)
- Notes: [`homebrew/libretro_button_demo/README.md`](homebrew/libretro_button_demo/README.md)
- Build log: [`homebrew/libretro_button_demo/BUILD_LOG.md`](homebrew/libretro_button_demo/BUILD_LOG.md)

It draws an RGB565 framebuffer, shows a moving square, logs button presses, and
was iterated until it could run through the stock `rkgame` loader. Later builds
use a 640x480 framebuffer so display scaling can be inspected on the real
device, and `disk_image_patch_014` adds a short generated sound on new button
presses.

### Pong Demo Core

A small Pong-style game module was built using the same no-libc libretro-style
shell as Button Demo:

- Source: [`homebrew/libretro_pong/pong.c`](homebrew/libretro_pong/pong.c)
- Notes: [`homebrew/libretro_pong/README.md`](homebrew/libretro_pong/README.md)
- Build log: [`homebrew/libretro_pong/BUILD_LOG.md`](homebrew/libretro_pong/BUILD_LOG.md)
- Built module: `homebrew/libretro_pong/libemu_pong.so`

It renders a 640x480 RGB565 playfield, supports D-pad left-paddle control,
optional face-button right-paddle control, simple AI when the right paddle is
not controlled manually, Start pause/resume, Select reset, score display, and
generated tones.

Important discoveries from these homebrew experiments:

- A dummy ROM with a stock extension such as `.gb` was more reliable than a new
  custom extension.
- `cores/filelist.xml` can force a specific file to launch through a custom
  `.so` core.
- The stock launcher appears to collapse some physical face buttons before the
  core sees them.
- The custom core must export the expected `retro_*` API and the launcher
  compatibility symbols.

### ZX Spectrum 48K Core

An experimental ZX Spectrum 48K core was built from `libretro/fuse-libretro`:

- Notes: [`homebrew/fuse_zx48/README.md`](homebrew/fuse_zx48/README.md)
- Build log: [`homebrew/fuse_zx48/BUILD_LOG.md`](homebrew/fuse_zx48/BUILD_LOG.md)
- Build commands: [`homebrew/fuse_zx48/BUILD_COMMANDS.md`](homebrew/fuse_zx48/BUILD_COMMANDS.md)
- Test tape: `homebrew/fuse_zx48/R36SX_ZX48.tap`
- Built module: `homebrew/fuse_zx48/libemu_fuse.so`

The ready integration patch is:

- [`disk_image_patch_011/MANIFEST.md`](disk_image_patch_011/MANIFEST.md)

This is still experimental. The first device tests returned to the launcher
after a black screen. `disk_image_patch_012` tried explicit runtime
dependencies, `disk_image_patch_013` switched to a smaller no-`NEEDED` build,
and `disk_image_patch_015` adds a guard for the device-side `Loading` hang by
bounding Fuse's audio-driven `retro_run()` loop.

### Tiny MC Launcher

A small one-panel file manager / launcher was added as the first standalone
replacement candidate for `cubegm/rkgame`:

- Source: [`homebrew/tiny_mc/tiny_mc.c`](homebrew/tiny_mc/tiny_mc.c)
- Notes: [`homebrew/tiny_mc/README.md`](homebrew/tiny_mc/README.md)
- Build log: [`homebrew/tiny_mc/BUILD_LOG.md`](homebrew/tiny_mc/BUILD_LOG.md)
- Build commands: [`homebrew/tiny_mc/BUILD_COMMANDS.md`](homebrew/tiny_mc/BUILD_COMMANDS.md)
- Built executable: `homebrew/tiny_mc/tiny_mc`

It now follows the stock `rkgame` display path by loading `cubegm/driver.so`,
drawing to an internal RGB565 640x480 buffer, and presenting frames through
`video_driver_disp_frame()`. Direct `/dev/fb0` rendering remains as fallback.
It reads `/dev/input/js*` and `/dev/input/event*`, and launches selected files
with `fork()` plus `execl()`. `disk_image_patch_022` copies the rebuilt Tiny MC
as `cubegm/rkgame`; the stock launcher should be preserved as
`cubegm/rkgame.stock`.

## Patch Sets

Patch directories are used to record changes that can be copied onto the SD
image/card without committing the full original image to git.

Notable patch history:

- `disk_image_patch_001`: initial Button Demo core.
- `disk_image_patch_002`: compatibility fixes based on `libemu_tgbdual.so`.
- `disk_image_patch_003`: rebuild without UBSan runtime dependencies.
- `disk_image_patch_004`: conservative static-frame test.
- `disk_image_patch_005`: switched the test route to a normal `.gb` extension.
- `disk_image_patch_006`: re-enabled input.
- `disk_image_patch_007`: added an on-screen scrolling button log.
- `disk_image_patch_008`: scanned raw joypad IDs to diagnose X/Y/A/B mapping.
- `disk_image_patch_009`: adjusted text spacing.
- `disk_image_patch_010`: native-resolution 640x480 Button Demo test.
- `disk_image_patch_011`: experimental Fuse ZX Spectrum 48K core.
- `disk_image_patch_012`: Fuse ZX Spectrum 48K compatibility rebuild after the
  first black-screen test.
- `disk_image_patch_013`: minimal no-`NEEDED` Fuse ZX Spectrum 48K rebuild.
- `disk_image_patch_014`: Button Demo rebuild with generated button-press
  audio.
- `disk_image_patch_015`: Fuse ZX Spectrum 48K rebuild with a guard against the
  `Loading` hang.
- `disk_image_patch_016`: Pong Demo core based on the working Button Demo shell.
- `disk_image_patch_017`: Pong Demo compatibility patch using a stock
  `GB/pong.gb` launch file plus an explicit `filelist.xml` core override.
- `disk_image_patch_018`: first libretro PRBoom core with Freedoom test files.
- `disk_image_patch_019`: PRBoom rebuild with R36SX vendor hook exports.
- `disk_image_patch_020`: PRBoom WAD adjacency and Doom shareware test files.
- `disk_image_patch_021`: Tiny MC standalone file manager replacing `rkgame`
  while preserving stock `rkgame.stock`.
- `disk_image_patch_022`: Tiny MC rebuild using the stock `driver.so` display
  initialization/present path after the first white/black-screen device test.
- `disk_image_patch_023`: Tiny MC rebuild with `tiny_mc.log` debug logging and
  cleanup of non-working `.demo`/`.pong` Game Boy menu rows.

Each patch directory should contain a `MANIFEST.md` explaining what changed,
what files to copy, and what was verified.

## Repository Layout

Tracked or intentionally small project files:

- `README.md`: this overview.
- `FIRMWARE_FINDINGS.md`: main firmware and architecture findings.
- `POWER_MANAGEMENT_ANALYSIS.md`: power management, `usleep`, standby, and idle
  behavior notes.
- `RELATED_PROJECTS.md`: public project links and research references.
- `MOST_SIMILAR_REPOSITORY.md`: closest public repository comparison.
- `homebrew/`: source and build logs for custom modules.
- `disk_image_patch_*`: copyable patch overlays.
- `tools/*.md` and `tools/scan-download.ps1`: local rules, notes, and helper
  scripts.

Ignored local data:

- `disk_image/`: original firmware/image contents.
- `ghidra_12.0.4_PUBLIC/`: local Ghidra installation.
- `ghidra_projects/` and `ghidra_exports/`: generated reverse engineering data.
- `internet_sources/`: downloaded third-party repositories.
- downloaded SDKs, archives, caches, and large binary images.

## Practical Direction

The current best path for further work is:

1. Keep the vendor kernel, DTB, AVP, rootfs, and hardware drivers unchanged.
2. Continue building MIPS32 little-endian userspace modules for `cubegm/rkgame`.
3. Use stock cores such as `libemu_tgbdual.so` as ABI references.
4. Use `filelist.xml` overrides to test new cores through existing platform
   menus.
5. Only patch boot artifacts after full backups, hash verification, and a clear
   recovery path.

Longer term, a custom frontend could replace or bypass `rkgame`, but a full
firmware replacement should start from Hichip/HCRTOS platform knowledge rather
than from generic ARM handheld firmware projects.
