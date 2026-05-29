# R36SX / SF3000 Firmware Reverse Engineering Notes

This repository collects notes, experiments, build artifacts, and patch sets from
an investigation of an R36SX / GB350 / SF3000-like handheld console firmware.

The work is focused on understanding the stock SD-card layout, identifying the
hardware and software platform, reverse engineering the game launcher, and
building small homebrew libretro-style modules for the stock `cubegm/rkgame`
environment.

Project video:

- [R36SX / SF3000 firmware research video](https://youtu.be/LYrICpUo6_A?si=NqGBL9BxAsdRZkY5)
- [R36SX project follow-up video](https://youtu.be/MHDpizjIZhw?si=tPpelQQvGKRtDBP2)

## Current Findings

The inspected image is not a single monolithic firmware blob. It looks like the
contents of a FAT/SD partition plus an extracted or companion `rootfs` tree.

## Original SD Card Warning

The SD card shipped with the console was labeled as `64 GB`, but its actual
capacity was only `16 GB`.  The partition table had been erased or damaged, so
the card was not recognized as a usable volume and Windows offered to format
it.  TestDisk was able to recover the partition table.

Before doing any experiments, writes, repairs, or filesystem operations on a
stock card, make a full raw backup image first with a tool such as
Win32DiskImager.  Work on copies of that image whenever possible.

Important platform facts:

- Main userspace architecture: ELF32 little-endian MIPS, O32 ABI.
- CPU string from the device tree: `MIPS 74Kc`.
- Board/platform strings: `Hichip HC16xx`, `Hichip,1600`, `hc1600a@dbE3100v20`.
- Root filesystem base: Buildroot `2021.05-rc2`.
- Game environment: `cubegm`, launched through `icube`, with `rkgame` loading
  emulator cores from `cubegm/cores`.

The most important summary file is:

- [`FIRMWARE_FINDINGS.md`](FIRMWARE_FINDINGS.md)
- [`BOOT_ROUTE_FINDINGS.md`](BOOT_ROUTE_FINDINGS.md)

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

- Shared source: [`homebrew/pong/pong.c`](homebrew/pong/pong.c)
- Notes: [`homebrew/pong/README.md`](homebrew/pong/README.md)
- Build log: [`homebrew/pong/BUILD_LOG.md`](homebrew/pong/BUILD_LOG.md)
- Build script: `homebrew/pong/build_libretro_pong.ps1`
- Built module: `homebrew/pong/libemu_pong.so`

It renders a 640x480 RGB565 playfield, supports D-pad left-paddle control,
optional face-button right-paddle control, simple AI when the right paddle is
not controlled manually, Start pause/resume, Select reset, score display, and
generated tones.

### Native Pong

A standalone Pong executable was added for direct launch from Tiny MC:

- Shared source: [`homebrew/pong/pong.c`](homebrew/pong/pong.c)
- Notes: [`homebrew/pong/README.md`](homebrew/pong/README.md)
- Build log: [`homebrew/pong/BUILD_LOG.md`](homebrew/pong/BUILD_LOG.md)
- Build script: `homebrew/pong/build_native_pong.ps1`
- Built executable: `homebrew/pong/pong`

Unlike the older `libemu_pong.so`, this is a normal MIPS executable with
`main()`. It uses `driver.so` for display/input and exits back to Tiny MC when
Select and Start are held together; Fn remains a secondary hardware escape.

### Native Button Demo

A standalone Button Demo executable was added for direct launch from Tiny MC:

- Source: [`homebrew/native_button_demo/native_button_demo.c`](homebrew/native_button_demo/native_button_demo.c)
- Notes: [`homebrew/native_button_demo/README.md`](homebrew/native_button_demo/README.md)
- Build log: [`homebrew/native_button_demo/BUILD_LOG.md`](homebrew/native_button_demo/BUILD_LOG.md)
- Build script: `homebrew/native_button_demo/build_native_button_demo.ps1`
- Built executable: `homebrew/native_button_demo/button_demo`

Unlike the older `libemu_buttondemo.so`, this is a normal MIPS executable with
`main()`. It uses `driver.so` for display/input and exits back to Tiny MC on Fn.

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

- [`patches/disk_image_patch_011/MANIFEST.md`](patches/disk_image_patch_011/MANIFEST.md)

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

It follows the stock `rkgame` display path by loading `cubegm/driver.so`,
drawing to an internal RGB565 640x480 buffer, and presenting frames through
`video_driver_disp_frame()`. Direct `/dev/fb0` rendering remains as fallback.
It reads the built-in controls through the same `driver.so` `cube_ioctl` path as
stock `rkgame`, and launches selected files with `fork()` plus `execl()`.

The current direct-launch integration makes `icube_start.sh` run
`/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE` directly,
without starting `icube`. The Tiny MC build has `USE_ICUBE_HEARTBEAT 0`, so it
does not call `shmget()`/`shmat()` or update the old supervisor heartbeat.

## Patch Sets

Patch directories live under `patches/` and are used to record changes that can
be copied onto the SD image/card without committing the full original image to
git. See [`patches/README.md`](patches/README.md) for the overlay-copy rule.

Names below omit the `patches/` prefix for readability.

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
- `disk_image_patch_024`: Tiny MC rebuild that reads built-in buttons through
  the same `driver.so` `cube_ioctl` + `/tmp/joy_key` path used by `rkgame`.
- `disk_image_patch_025`: Tiny MC rebuild with `tiny_mc.log` writes gated by
  the top-of-file `#define DEBUG 1` / `#define DEBUG 0` switch.
- `disk_image_patch_026`: Tiny MC rebuild that feeds the `icube` supervisor
  heartbeat shared memory so the launcher is not restarted every 5-6 seconds.
- `disk_image_patch_027`: Tiny MC, Button Demo, and Pong rebuild after moving
  shared hardware constants into `homebrew/common/hardware.h`.
- `disk_image_patch_028`: creates root-level `MIPS_NATIVE` for standalone MIPS
  programs launched directly from Tiny MC.
- `disk_image_patch_029`: Tiny MC rebuild with a right-side scrollbar for
  directories that contain more entries than fit on screen.
- `disk_image_patch_030`: Tiny MC rebuild that remembers selected entry and
  scroll offset per visited directory.
- `disk_image_patch_031`: moves Tiny MC to `MIPS_NATIVE/tiny_mc/tiny_mc`,
  restores stock `cubegm/rkgame`, and routes `cubegm/icube_start.sh` directly
  to Tiny MC.
- `disk_image_patch_032`: restores the stock `icube` launch chain, installs
  Tiny MC as the `cubegm/rkgame` compatibility entrypoint, and makes no-arg
  Tiny MC startup open `/mnt/sdcard/MIPS_NATIVE`.
- `disk_image_patch_033`: switches Tiny MC back to direct launch from
  `cubegm/icube_start.sh`, disables the `icube` heartbeat contract at compile
  time, and restores stock `cubegm/rkgame`.
- `disk_image_patch_034`: adds a Tiny MC FN shortcut that closes Tiny MC and
  `exec`s `/mnt/sdcard/cubegm/icube`, while keeping direct launch and stock
  `cubegm/rkgame`.
- `disk_image_patch_035`: adds standalone native Pong under
  `MIPS_NATIVE/pong/pong` for launch directly from Tiny MC.
- `disk_image_patch_036`: fixes the Tiny MC Fn shortcut so an initially raised
  Fn bit is ignored until Fn has been released once.
- `disk_image_patch_037`: disables Tiny MC Fn handling entirely and documents
  the original `icube` supervisor structure.
- `disk_image_patch_038`: adds `boot_route.log` tracing to `icube.sh`,
  `icube_start.sh`, and `icubemp_start.sh` to discover the real shell route.
- `disk_image_patch_039`: changes the real boot route, `icube.sh`, to launch
  Tiny MC directly after `boot_route.log` showed `icube_start.sh` is skipped.
- `disk_image_patch_040`: removes temporary `boot_route.log` tracing while
  keeping the confirmed Tiny MC boot route through `icube.sh`.
- `disk_image_patch_041`: rebuilds native Pong with `YOU WIN` / `YOU LOSE`
  end-of-game messages.
- `disk_image_patch_042`: restores Tiny MC Fn shortcut to start stock
  `/mnt/sdcard/cubegm/icube` directly, with startup Fn-release arming.
- `disk_image_patch_043`: adds native Button Demo under
  `MIPS_NATIVE/button_demo/button_demo` for launch directly from Tiny MC.
- `disk_image_patch_044`: restores sound in native Pong and native Button Demo
  through the stock `/dev/auddec` audio packet path.
- `disk_image_patch_045`: adds short Tiny MC button click sounds through
  `driver.so` audio symbols.
- `disk_image_patch_046`: rebuilds native Pong and native Button Demo so their
  generated sounds use `driver.so` `sound_driver_playframe`, matching the stock
  `rkgame` LibRetro audio path instead of writing to `/dev/auddec` directly.
- `disk_image_patch_047`: rebuilds both libretro and native Pong from the
  single shared source under `homebrew/pong/pong.c` and changes native Pong to
  the standard `Select + Start` exit gesture.
- `disk_image_patch_048`: rebuilds both libretro and native Pong so
  end-of-game messages render as large 5x5 pixel text: `You win` / `You lose`.
- `disk_image_patch_049`: rebuilds Tiny MC with optional FreeType text
  rendering and bundles JetBrains Mono NL Regular under `MIPS_NATIVE/tiny_mc/fonts`.
- `disk_image_patch_050`: moves bundled mono fonts to
  `MIPS_NATIVE/common/fonts`, adds Liberation Mono, DejaVu Sans Mono,
  Noto Sans Mono, Source Code Pro, Hack, and Roboto Mono, and rebuilds Tiny MC
  to prefer the common font directory.
- `disk_image_patch_051`: rebuilds Tiny MC with FreeType text about 20 percent
  larger and increases file-list row height to match.
- `disk_image_patch_052`: adds `MIPS_NATIVE/tiny_mc/tiny_mc.conf` and rebuilds
  Tiny MC so the font path and text sizes are configurable at runtime.
- `disk_image_patch_053`: rebuilds native Button Demo to render the pressed-key
  log through FreeType with the stock Arial font and removes the old top labels.
- `disk_image_patch_054`: rebuilds native Button Demo with adjusted face-button
  colors, including green `Y` and yellow `B`.
- `disk_image_patch_055`: rebuilds native Button Demo with the rear-trigger
  masks used by stock `rkgame`: `L`, `R`, `L2`, `R2`, `L3`, and `R3`.
- `disk_image_patch_056`: rebuilds Tiny MC, native Button Demo, and native Pong
  so they preserve the current mixer volume around `driver.so`
  `sound_driver_init()`.
- `disk_image_patch_057`: rebuilds Tiny MC so returning from native apps
  reopens/redraws the display and suppresses held `Select + Start`/`Start`
  input until all buttons are released once.
- `disk_image_patch_058`: fixes the Tiny MC directory-enter regression from
  patch 057 by updating normal navigation button state even when `A` enters a
  directory instead of launching a child process.
- `disk_image_patch_059`: changes Tiny MC native-app return to self-`exec()`
  after child exit, because the device log showed in-process `driver.so`
  display reopen succeeds by return code but leaves the LCD black.
- `disk_image_patch_060`: rebuilds Tiny MC so extra file-list row height is
  split around the text; odd extra pixels put the larger half below the text.
- `disk_image_patch_061`: adds the Tiny MC built-in text viewer for configured
  extensions, defaulting to `txt`, `log`, and `conf`.
- `disk_image_patch_062`: rebuilds Tiny MC so the orange selection marker is
  vertically aligned to the highlighted row padding instead of the text
  baseline.
- `disk_image_patch_063`: rebuilds Pico-286 with a safer key preset editor:
  two-column physical button list, draft edits, on-screen-keyboard assignment,
  preset rename, and explicit OK/Cancel save controls.
- `disk_image_patch_064`: replaces Pico-286 blank hard disk images with
  host-created FAT16 `hdd.img`/`hdd2.img` and rebuilds Pico-286 with detailed
  INT 13h disk read/write failure logging.

Each patch directory should contain a `MANIFEST.md` explaining what changed,
what files to copy, and what was verified.

## Repository Layout

Tracked or intentionally small project files:

- `README.md`: this overview.
- `FIRMWARE_FINDINGS.md`: main firmware and architecture findings.
- `POWER_MANAGEMENT_ANALYSIS.md`: power management, `usleep`, standby, and idle
  behavior notes.
- `ICUBE_ANALYSIS.md`: original `icube` launch and watchdog structure.
- `BOOT_ROUTE_FINDINGS.md`: device-side `boot_route.log` result showing cold
  boot enters `cubegm/icube.sh`, not `icube_start.sh`.
- `RELATED_PROJECTS.md`: public project links and research references.
- `MOST_SIMILAR_REPOSITORY.md`: closest public repository comparison.
- `homebrew/tiny_mc/FONT_RESEARCH.md`: monospaced open-font candidates and the
  JetBrains Mono NL selection notes for Tiny MC FreeType rendering.
- `homebrew/common/fonts/`: common redistributable monospaced TTF pool for
  native apps, with license files and hashes.
- `homebrew/`: source and build logs for custom modules.
  - `homebrew/pong/pong.c`: unified Pong source compiled as either libretro
    (`R36SX_PONG_TARGET=1`) or native Tiny MC
    (`R36SX_PONG_TARGET=2`).
  - `homebrew/common/hardware.h`: shared R36SX/SF3000-like hardware and
    firmware constants used by homebrew programs.
  - `homebrew/common/libretro_hardware.h`: libretro-specific screen/audio
    cadence constants layered on top of `hardware.h`.
  - `homebrew/common/driver_audio.h`: documented `driver.so` audio helper for
    standalone native programs that already load `driver.so`.
  - `homebrew/common/native_audio.h`: minimal `/dev/auddec` PCM packet helper
    for standalone Tiny MC programs.
- `patches/`: copyable patch overlays and patch application notes.
- `patches/disk_image_patch_tiny_mc`: consolidated install overlay for the
  current Tiny MC boot route and Fn-to-iCube behavior.
- `patches/disk_image_patch_028/MIPS_NATIVE/README.txt`: native-program folder
  layout convention for direct Tiny MC launches.
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
