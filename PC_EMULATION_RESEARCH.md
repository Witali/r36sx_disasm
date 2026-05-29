# PC Emulation Porting Research

Date: 2026-05-28

This note records the evaluation of PC/DOS emulators as possible native
applications for the R36SX/GB350-like MIPS/Linux console.

## Local Downloads And Scans

### xrip/pico-286

- Source URL: https://github.com/xrip/pico-286
- Current vendored path: `homebrew\pico_286\pico-286`
- Original downloaded path: `internet_sources\pico-286` (removed after vendoring)
- Checked commit: `2ba88bf17de8e69c86f9256e22a241e8d31e8c96`
- Commit summary: `2ba88bf MIDI: add per-instrument ADSR envelope with GM program table`
- Download command:
  - `git clone --depth 1 https://github.com/xrip/pico-286.git internet_sources\pico-286`
- Security scan:
  - Command: `.\tools\scan-download.ps1 .\internet_sources\pico-286`
  - Vendored scan command: `.\tools\scan-download.ps1 .\homebrew\pico_286\pico-286`
  - Scanner: Microsoft Defender `MpCmdRun.exe`
  - Result: no threats found

### hchunhui/tiny386

- Source URL: https://github.com/hchunhui/tiny386
- Local path: `internet_sources\tiny386`
- Checked commit: `5cad7585d7a9e3081e8c5f50e3865fdbf09f5ebb`
- Download command:
  - `git clone --depth 1 https://github.com/hchunhui/tiny386.git internet_sources\tiny386`
- Security scan:
  - Command: `.\tools\scan-download.ps1 .\internet_sources\tiny386`
  - Scanner: Microsoft Defender `MpCmdRun.exe`
  - Result: no threats found

`internet_sources\` is ignored by git, so the downloaded repositories are local
reference copies only. Pico-286 was later vendored into
`homebrew\pico_286\pico-286` without `.git` metadata so the port can build from
tracked local sources. This tracked file records the sources and exact commits.

## pico-286 Summary

`pico-286` is a compact PC-family emulator aimed at Raspberry Pi Pico
RP2040/RP2350 boards and the Murmulator hardware family. It emulates an
8086/8088/80186/286-class PC with CGA, Hercules, EGA/Tandy, VGA modes,
floppy/hard disk images, PC speaker, Covox/DSS, AdLib/OPL2, Sound Blaster,
CMS/GameBlaster, Tandy/PCjr PSG, MPU-401, mouse, EMS/XMS-style services, and
host filesystem mapping.

The most useful property for us is that it is not only Pico firmware. The tree
contains desktop frontends:

- `src\linux-main.cpp`
- `src\win32-main.cpp`
- `src\LinuxMiniFB.c`
- `src\WinMiniFB.c`
- `src\linux-audio.c`

The host build uses `PICO_PLATFORM=host`, X11/MiniFB for display, pthreads for
timing/audio threads, and a Linux audio backend that first tries PulseAudio and
then OSS devices. The emulator core itself is under `src\emulator\`, with the
CPU entry point exposed as `exec86(uint32_t execloops)`.

Important portability points:

- The main Linux loop repeatedly calls `exec86(32768)` and updates the MiniFB
  window with a 640x480 software framebuffer.
- A separate timing thread generates PIT IRQ0, audio samples, cursor blink, and
  60 FPS renderer updates.
- Keyboard input is converted into PC set-1 scancodes by writing `port60`,
  setting `port64`, and calling `doirq(1)`.
- Disk access for host builds is already normal C file I/O in
  `src\emulator\disks-win32.c.inl`; Pico builds use FatFS.
- The Linux audio path is replaceable. On our console we should bypass
  PulseAudio/OSS and use the native `/dev/auddec` packet path already proven by
  `homebrew\common\native_audio.h`.

Limitations:

- Despite the name, this is not a full 386 emulator. There are guarded fragments
  for `CPU_386_EXTENDED_OPS`, but the normal build and project scope are
  8086/8088/80186/286. The 0x0F opcode path is not a complete 386/protected-mode
  implementation.
- The Linux timing thread currently busy-waits with no sleep. A console port
  should add frame pacing with `clock_gettime` plus `usleep`/`nanosleep`, or use
  our usual cooperative loop pattern, to avoid unnecessary battery drain.
- A usable DOS experience needs bootable disk images (`fdd0.img`, `hdd.img`,
  etc.) placed in a path expected by the port.

## Feasibility On Our Console

Target platform known from this project:

- MIPS32 little-endian Linux userspace.
- Native programs live under `disk_image\MIPS_NATIVE\<app>\<app>`.
- Display path is available through `cubegm\driver.so` wrappers used by
  TinyMC/Pong/Button Demo.
- Input path is available through `cube_ioctl` and `/tmp/joy_key`, wrapped in
  `homebrew\common\hardware.h`.
- Audio output is available through `/dev/auddec`, wrapped in
  `homebrew\common\native_audio.h`.

Recommended first port shape:

1. Create `homebrew\pico286_native`.
2. Import only `src\emulator\`, `src\emu8950\`, fonts, and minimal support files.
3. Replace `linux-main.cpp`, MiniFB, X11, and `linux-audio.c` with a native
   frontend using our 640x480 framebuffer, button mapper, and `/dev/auddec`.
4. Start with no mouse, no host drive mapping, and preferably no Sound Blaster
   digital audio until video/input/disk boot are stable.
5. Map D-pad and face buttons to useful PC scancodes first: arrows, Enter,
   Escape, Space, Ctrl/Alt, maybe F1/F2. Add an on-screen keyboard later.
6. Store disk images under `disk_image\MIPS_NATIVE\pico286\`.

Estimated difficulty: medium. A first "shows DOS boot screen and accepts a few
buttons" build looks realistic. A pleasant DOS-gaming frontend with keyboard
mapper, save handling, and stable audio is a larger follow-up.

The performance risk is real but not obviously fatal. `pico-286` is designed for
microcontrollers, while our device is a Linux/MIPS handheld. Simple real-mode
DOS programs, CGA/EGA games, text UI, and early 286-era software are plausible.
VGA/Sound Blaster-heavy workloads need measurement.

## 386 Emulator Candidates

### tiny386

`tiny386` is the closest 386 candidate found so far. It is a C99 x86 PC emulator
with a compact i386 CPU core and a fuller PC system around it: PIC, PIT,
keyboard controller, CMOS, VGA with Bochs VBE, IDE, NE2000, DMA, PC speaker,
optional AdLib OPL2, and Sound Blaster 16. Its README says it can boot Windows
9x/NT on MCU-class hardware such as ESP32-S3.

Why it is interesting for us:

- Mostly C, not a huge C++/SDL-only application.
- Already has rawdraw, SDL, Win32, WASM, and ESP frontends.
- Has a real 386 protected-mode direction, unlike `pico-286`.
- Has config-file-driven BIOS/VGA BIOS/disk setup.

Why it is harder than `pico-286`:

- It expects much more RAM by default, for example 32 MB RAM and 2 MB VGA memory
  in the sample config.
- It needs BIOS/VGABIOS files from SeaBIOS unless using the linuxstart path.
- It is a fuller PC, so input, storage, display, audio, and timing are more
  complex to validate.
- Windows 9x/NT booting on the console is not a good first target; DOS text/CGA
  or very small protected-mode DOS programs would be a safer benchmark.

Estimated difficulty: high for a polished app, medium-high for a technical
prototype.

### v86

`copy/v86` is a browser/JavaScript/WASM x86 PC emulator with an x86-to-WASM JIT.
It is useful as a reference for PC emulation behavior and disk/BIOS workflows,
but it is not a practical direct port target for our MIPS native console.

### 86Box / PCem Family

86Box is a full x86-machine emulator. It is valuable as an accuracy reference,
but it is too large and dependency-heavy for the first native console attempt.
It targets detailed machine emulation, not a small embedded-style app.

## DOSBox Family

Relevant repositories:

- https://github.com/dosbox-staging/dosbox-staging
- https://github.com/joncampbell123/dosbox-x
- https://github.com/schellingb/dosbox-pure
- https://github.com/realnc/dosbox-core

DOSBox is attractive because it targets DOS games directly and already solves a
lot of DOS usability problems. For our device, the difficulty is high:

- Standalone DOSBox/DOSBox-X/DOSBox Staging generally expects a larger desktop
  style runtime: SDL/video/audio/input/filesystem integration and a C++ build.
- `dosbox-pure` is a libretro core and has excellent controller/keyboard UI
  ideas, but integrating a large libretro DOS core into the stock `rkgame`
  environment may be more fragile than using our TinyMC-native app model.
- On MIPS, we should expect to use the interpreter/normal core path rather than
  a fast host-specific dynamic recompiler. That is the main performance risk for
  386/protected-mode games.
- A small static build would require disabling optional features and auditing
  libc/libstdc++ dependencies carefully.

Estimated difficulty:

- Minimal native DOSBox experiment: high.
- Reliable game launcher with sound, keyboard mapper, save directories, and
  acceptable speed: very high.
- 386-era protected-mode games: likely limited by CPU speed unless the chosen
  DOSBox variant has a usable MIPS dynamic core, which is not the default
  assumption.

## Recommendation

The best next experiment is `pico-286`, not DOSBox.

Concrete plan:

1. Build/run the `pico-286` host version locally if the host toolchain is
   available, using a tiny bootable DOS image.
2. Create a MIPS native frontend with our existing framebuffer/input/audio
   helpers.
3. First milestone: boot a floppy image and display 80x25 text or a simple CGA
   program.
4. Second milestone: map buttons to keyboard scancodes and add a tiny on-screen
   key overlay.
5. Third milestone: add audio and joystick/mouse mapping.
6. Only after that, evaluate `tiny386` as a separate 386/protected-mode track.

For DOSBox, the safest approach is not to start with full DOSBox-X. If we try
DOSBox at all, start from `dosbox-pure` or a small libretro DOSBox core as a
reference, but implement it as a native TinyMC-launched application only after
`pico-286` proves the screen/input/timing approach.
