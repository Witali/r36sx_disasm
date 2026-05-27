# ZX Spectrum Emulator Integration Candidates

Research date: 2026-05-27.

Goal: find open-source ZX Spectrum emulators or emulator cores that could be
integrated into the R36SX / `cubegm/rkgame` homebrew module path as a MIPS
`.so`, with fewer dependencies and simpler code than the current Fuse-based
experiment.

## Local Constraints

- Target module format: ELF32 little-endian MIPS shared object.
- Frontend style: stock `rkgame` loads libretro-like cores from
  `cubegm/cores`.
- Known working baseline: our Button Demo core can draw RGB565 video, read
  input, and output generated PCM audio.
- Risk from the Fuse experiment: large cores with desktop/frontend assumptions
  can fail before the first useful frame.
- Best initial feature target: ZX Spectrum 48K only, preferably snapshot first,
  then TAP/TZX later.

## Quick Metrics

These repository metrics were collected through the public GitHub API. They are
rough file-count heuristics, not full code audits.

| Project | Repo | Files | C/H | C++/H++ | JS/TS | Build files | Dependency hints | License |
| --- | --- | ---: | ---: | ---: | ---: | ---: | ---: | --- |
| Fuse libretro | https://github.com/libretro/fuse-libretro | 1116 | 577 | 5 | 0 | 106 | 41 | GPL-3.0 |
| Spectral | https://github.com/r-lyeh/Spectral | 418 | 146 | 2 | 0 | 1 | 5 | Unlicense / 0-BSD / MIT-style notice |
| ZEsarUX | https://github.com/chernandezba/zesarux | 1508 | 418 | 0 | 0 | 1 | 12 | GPL |
| EightyOne | https://github.com/charlierobson/EightyOne | 1011 | 273 | 110 | 0 | 7 | 0 | not obvious from API |
| pico-zxspectrum | https://github.com/fruit-bat/pico-zxspectrum | 466 | 81 | 77 | 0 | 9 | 133 | MIT |
| ESPectrum | https://github.com/EremusOne/ESPectrum | 173 | 71 | 32 | 0 | 7 | 37 | GPL-3.0 |
| redcode/Z80 | https://github.com/redcode/Z80 | 44 | 6 | 0 | 0 | 2 | 6 | LGPL-3.0 |
| JSSpeccy 3 | https://github.com/gasman/jsspeccy3 | 53 | 0 | 0 | 13 | 1 | 0 | GPL-3.0 |
| zx_go | https://github.com/conorarmstrong/zx_go | 139 | 0 | 0 | 0 | 0 | 0 | not obvious from API |

## Candidate Ranking

### 1. redcode/Z80 + our own minimal 48K wrapper

Link: https://github.com/redcode/Z80

This is not a complete Spectrum emulator. It is a small ANSI C Z80 CPU core.
Its README says Zeta is the sole dependency and that the emulator does not
depend on the C standard library.

Why it is the strongest practical direction:

- Very small source surface: only a handful of C/header files for the CPU core.
- C, not C++/Rust/Go/C#.
- Designed as an embeddable library.
- No desktop UI/audio/video framework to remove.
- Lets us keep the proven Button Demo libretro shell and add only Spectrum
  hardware pieces.

What we would need to write:

- 64K memory map with 16K ROM and 48K RAM.
- ULA port `0xfe` keyboard matrix and border/beeper bits.
- RGB565 renderer for Spectrum bitmap/attributes.
- Frame scheduler around 50 Hz.
- Minimal sound beeper output.
- Snapshot loader, probably `.sna` first.
- Later: TAP fast-loader or ROM tape emulation.

Recommendation: best long-term path for a small, understandable homebrew core.
This is the closest match to "minimum dependencies, minimum files".

### 2. ESPectrum

Link: https://github.com/EremusOne/ESPectrum

ESPectrum is an ESP32 emulator for ZX Spectrum 48K, 128K, +2A, Pentagon 128,
and TK90X/TK95. It claims cycle-accurate emulation for several models and uses
a Z80 implementation by Jose Luis Sanchez.

Pros:

- Embedded-oriented, so it is closer to our target than desktop SDL/Qt projects.
- Already has video/audio/input split for a constrained device.
- Smaller than Fuse/ZEsarUX by repository file count.
- Supports practical formats and models.

Cons:

- C++ and ESP32/FabGL/PlatformIO assumptions.
- Source tree includes board, PS/2, VGA, SD-card, and ESP components that would
  need to be cut away.
- GPL-3.0.

Recommendation: good source to study or salvage a 48K/128K core from, but likely
more porting work than redcode/Z80 plus a purpose-built wrapper.

### 3. Spectral

Link: https://github.com/r-lyeh/Spectral

Spectral is a portable Windows/Linux/macOS emulator, advertised as a single
executable. The README calls it experimental and says accuracy/performance are
long-term goals. It includes multiple Z80 backends and many single-header
third-party pieces.

Pros:

- C-heavy and broadly portable.
- Permissive/public-domain style licensing.
- Interesting source layout: `emu.h`, `emu_z80*.h`, `emu_spk.h`, `emu_ay.h`,
  loaders, etc.
- Single executable model means it may be easier to carve out than GUI-heavy
  desktop applications.

Cons:

- Still has about 148 C/C++ source-ish files.
- Many bundled third-party headers and application features: dialogs, archive
  support, gamepad, database/browser-ish code.
- README itself warns it is experimental.

Recommendation: promising as a donor for ideas or individual components, but
not as clean as starting from a small CPU core.

### 4. Spectemu

Links:

- https://sourceforge.net/projects/spectemu/
- https://sources.debian.org/src/spectemu/0.94a-23/README

Spectemu is an old GPLv2 48K ZX Spectrum emulator. Debian's source page reports
roughly 15k ANSI C SLOC and 5k assembly SLOC.

Pros:

- 48K-focused, which matches a minimal first target.
- Much smaller conceptually than Fuse/ZEsarUX.
- Written in C and historically portable across Unix-like systems.

Cons:

- Old codebase.
- X11/SVGAlib/Linux host assumptions.
- Assembly parts may be awkward for our MIPS shared object.
- Less convenient than GitHub repos for quick iteration.

Recommendation: worth checking if we want an existing complete 48K emulator in
C, but not the first choice.

### 5. FBZX

Link: https://www.rastersoft.com/programas/fbzx.html

FBZX is a C/Linux emulator designed for framebuffer or X-Windows. Public
descriptions say it uses Z80Free and supports accurate screen, keyboard, sound,
TAP/TZX, snapshots, joysticks, Interface I/Microdrive, ULAPlus, and 128K models.

Pros:

- Framebuffer target is philosophically close to our direct RGB565 output.
- C codebase.
- More complete than a minimal custom wrapper.

Cons:

- Host assumptions around Linux framebuffer/X-Windows.
- More features than we need for first boot.
- GPLv3.

Recommendation: interesting if we want an existing framebuffer-oriented emulator
to mine for display/tape ideas.

### 6. pico-zxspectrum

Link: https://github.com/fruit-bat/pico-zxspectrum

This is an RP2040/RP2350 ZX Spectrum emulator with HDMI/LCD/VGA, USB/PS2 input,
audio, quicksave, `.z80`, `.tap`, and `.tzx`.

Pros:

- Embedded hardware-oriented.
- MIT license.
- Uses redcode/Z80 and Zeta, so it validates that redcode/Z80 can be used in a
  Spectrum context.

Cons:

- Build instructions require a pile of Pico-specific repos: PicoDVI, TinyUSB,
  pico helpers, Pimoroni drivers, etc.
- More C++/hardware integration than we need.

Recommendation: useful reference, not a direct integration base.

### 7. Projects To Avoid For This Port

- Fuse / fuse-libretro: already integrated experimentally, but large and
  frontend-sensitive.
- ZEsarUX: excellent emulator, but much too large and feature-heavy for this
  module.
- EightyOne: source is available, but the project is Windows/Wine/GUI-heavy and
  covers many Sinclair machines.
- JSSpeccy / JSSpeccy 3: good browser emulator, but JavaScript/WebAssembly is
  the wrong implementation language for our MIPS `.so`.
- Spectron / Zero / Zen: C#/.NET projects, not practical for this firmware.
- zx_go: Go project; interesting, but a Go runtime/toolchain is a poor fit for
  this tiny `.so` integration path.
- Marat Fayzullin's Speccy: portable C, but source licensing is not open for
  our use.

## Recommended Plan

1. Stop trying to make the full Fuse core the only path.
2. Build a new `libemu_zx48mini.so` using the working Button Demo shell.
3. Embed or load a legal 48K ROM.
4. Integrate `redcode/Z80` as the CPU.
5. Add only the minimal 48K hardware:
   - 64K memory;
   - screen memory and attribute renderer;
   - `0xfe` keyboard/border/beeper port;
   - 50 Hz frame pacing;
   - `.sna` loader first.
6. Once `.sna` works, add TAP fast-load or a tiny TAP block injector.

This path should give us the smallest, most debuggable Spectrum core and avoids
the desktop/frontend assumptions that made Fuse hard to diagnose on the real
device.
