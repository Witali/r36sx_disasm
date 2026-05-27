# Murmulator / pico-spec Porting Analysis

Date: 2026-05-27

## Sources

- https://murmulator.ru/zxmurmulator
- https://github.com/DnCraptor/pico-spec
- https://github.com/EremusOne/ESPectrum

Downloaded source snapshot:

```text
internet_sources\murmulator\pico-spec-main.zip
internet_sources\murmulator\pico-spec-main
```

Security scan:

```text
.\tools\scan-download.ps1 .\internet_sources\murmulator\pico-spec-main.zip
Result: Microsoft Defender found no threats.
```

## What It Is

`pico-spec` is the Murmulator ZX Spectrum firmware. It is a C++17/C11 bare-metal
Raspberry Pi Pico SDK port of ESPectrum for RP2040/RP2350 boards, not a Linux
userspace emulator core.

It supports ZX Spectrum 48K, 128K, Pentagon 128/512/1024K, Byte, ALF TV Game,
AY/TurboSound, SAA1099, tape/snapshot/disk formats, OSD menus, VGA/HDMI/TFT/TV
output, PS/2/USB keyboard, joysticks, SD card browsing, and RP2350-only extras.

## Local Source Shape

Local source snapshot:

```text
internet_sources\murmulator\pico-spec-main
```

Top-level files include:

```text
CMakeLists.txt
src\
drivers\
memmap.ld
rp2350-memmap.ld
pico_sdk_import.cmake
```

Observed source size:

```text
src: 138 files, about 16.2 MB total
file types: 41 .cpp, 55 .c, 112 .h, 10 .pio, plus ROM/media/test files
```

Important files:

```text
src\main.cpp          Pico hardware init, multicore setup, input glue
src\ESPectrum.cpp     setup/reset/main emulation loop/audio mix
src\CPU.cpp           frame CPU loop and HALT flushing
src\Z80_JLS.cpp       Z80 emulator implementation
src\Ports.cpp         ULA/AY/PIT/FDD/Kempston I/O ports
src\Video.cpp         VGA/HDMI/TFT/TV renderer and framebuffer logic
src\MemESP.cpp        RAM, ROM, paging, contention
src\OSDMain.cpp       large OSD/menu layer
drivers\*             Pico hardware drivers
```

## Platform Dependencies

The project links directly against Pico SDK and hardware libraries:

```text
pico_runtime
pico_stdlib
pico_multicore
hardware_dma
hardware_pio
hardware_i2c
hardware_interp
hardware_timer
hardware_clocks
hardware_pwm
hardware_uart
hardware_flash
hardware_exception
tinyusb_host
tinyusb_board
```

It also builds board-specific drivers:

```text
drivers\vga-nextgen
drivers\hdmi
drivers\tv
drivers\tv-software
drivers\st7789
drivers\psram
drivers\fatfs
drivers\sdcard
drivers\nespad
drivers\audio
```

This is the main porting problem for our console: our target is a MIPS32 little
endian Linux-like `cubegm` environment that loads libretro-like `.so` files.
Murmulator/pico-spec is a bare-metal ARM/RP2040/RP2350 firmware.

## Main Loop Notes

`ESPectrum::loop()` is an infinite firmware loop. Per iteration it:

- writes the previous frame of audio through `pwm_audio_write()`;
- resets per-frame audio counters;
- calls `CPU::loop()`;
- completes beeper/AY/PIT/SAA/MIDI/FDD audio buffers;
- processes keyboard/input and OSD state.

`CPU::loop()` executes Z80 instructions up to interrupt/frame boundaries and
calls `VIDEO::EndFrame()` at the end of the frame.

The code has its own Z80 HALT handling:

```text
CPU::loop()
  bool halted = Z80::isHalted();
  if halted: CPU::FlushOnHalt()
```

`CPU::FlushOnHalt()` advances emulated T-states and renders the remaining video
work. This is not a host CPU sleep/wait. It is an emulated-Z80 optimization and
cycle-accurate video flush.

## Porting Assessment For R36SX/cubegm

Direct build for our device:

```text
Not practical.
```

Reasons:

- CPU/ABI mismatch: Murmulator firmware targets ARM RP2040/RP2350, our modules
  must be MIPS32 little-endian Linux shared objects.
- OS mismatch: Murmulator is bare-metal Pico SDK firmware; our launcher loads a
  libretro-like `.so` from `cubegm`.
- Heavy hardware coupling: video, audio, SD, PSRAM, USB/PS2 keyboard, timers,
  multicore, DMA, PIO, flash, and board GPIO are used directly.
- Large frontend/OSD surface: `OSDMain.cpp` alone is large and depends on the
  project's own file/input/display stack.
- It is feature-rich, not minimal. That is good for reference quality but poor
  for a first successful port.

Possible but expensive path:

1. Build a new platform abstraction layer for Linux/libretro callbacks.
2. Remove or stub Pico drivers: VGA/HDMI/TFT/TV, PWM/I2S audio, SD/FatFS,
   TinyUSB, PS/2, PSRAM, flash, multicore.
3. Keep only a reduced ZX 48K subset first:
   - `Z80_JLS`;
   - 48K ROM/RAM;
   - ULA screen renderer;
   - keyboard/joystick mapping;
   - beeper and maybe AY later;
   - TAP/SNA loading later.
4. Replace `ESPectrum::loop()` infinite firmware loop with a libretro-style
   `retro_run()` that executes exactly one 50 Hz frame and returns to `rkgame`.
5. Convert the framebuffer into our RGB565 640x480 output path.
6. Convert mixed audio buffers into `retro_audio_sample_batch()`.
7. Replace file browser/OSD with launcher-provided file loading or a very small
   in-core menu.

Estimated complexity:

```text
High. Weeks, not hours, if the goal is a usable emulator.
Medium-high for a stripped 48K-only technical prototype if we discard most UI,
disk, HDMI, SD, PSRAM, USB, and advanced machine support.
```

## Recommendation

Use Murmulator/pico-spec as a reference for:

- accurate Z80 timing and HALT-frame behavior;
- ULA/border/rendering logic;
- keyboard/joystick mapping ideas;
- AY/beeper mixing details;
- feature roadmap for a serious Spectrum emulator.

Do not use it as the first codebase to port to this console. For this device,
a smaller libretro or SDL ZX Spectrum emulator remains a better starting point.
Murmulator becomes attractive only after our libretro-like integration and
frame/audio/input path is stable enough to absorb a large C++ port.
