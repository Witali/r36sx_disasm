# TODO

## Pico-286 Performance Optimizations

- Use the new Pico-286 profiling log before deeper rewrites.  Enable
  `[profiling] profiling_enabled=1` in `pico_286.conf` and compare time spent
  in `exec86()`, `renderer()`, `mfb_update()`, audio generation, and disk I/O
  over a fixed 5-10 second run on hardware.
- Add a fast instruction-fetch path in
  `homebrew/pico_286/r36sx_port/r36sx_cpu.c`.  Opcode and immediate reads
  currently go through `getmem8()` -> `read86()` function pointers.  Fast-path
  `CS:IP` reads that land in plain RAM/BIOS, and fall back to the existing
  memory handlers only for video/EMS/HMA/A20-sensitive regions.
- Add inline RAM read/write helpers for the CPU hot path.  Most guest memory
  accesses are plain RAM; avoiding indirect function-pointer calls in common
  `read86()`/`write86()` cases should help every decoded instruction.
- Consider a CPU dispatch fast path for instructions without prefixes.  The
  current `exec86()` loop does a prefix loop and then a large opcode `switch`;
  a no-prefix path or computed-goto dispatch may reduce branch overhead, but
  should be done after profiling because it is higher risk.
- Extend REP batching with direct RAM operations.  `MOVSB/MOVSW/STOSB/STOSW`
  already batch up to 1024 elements, but each element still uses `getmem` /
  `putmem`.  For RAM-to-RAM and RAM-to-video cases, use specialized loops or
  `memcpy()`/`memset()` where the emulator semantics allow it.
- Add VRAM dirty page/row tracking.  `renderer()` currently renders a full
  640x480 frame at 60 Hz and then marks a frame ready.  Mark dirty rows/pages
  from `vga_mem_write()`/`vga_mem_write16()` and skip or partially render
  unchanged text/graphics rows.
- Add audio chip auto-gating in
  `homebrew/pico_286/r36sx_port/r36sx_ports.c`.
  `get_sound_sample()` currently mixes OPL, PC speaker, SN76489, MIDI/CMS
  paths every 44.1 kHz sample even when most chips are inactive.  Track writes
  to the relevant ports and skip inactive chip mixers.
- Add config switches for optional audio chips: `audio_opl`, `audio_cms`,
  `audio_sn76489`, and `audio_midi`.  Keep PC speaker and Sound Blaster paths
  available, but allow Tandy/SN76489/CMS/MIDI to be disabled by default or
  auto-enabled on port access.
- Hardware-test the `HLT` idle behavior implemented in
  `homebrew/pico_286/r36sx_port/r36sx_cpu.c`.  The CPU now leaves `exec86()`
  while halted and wakes on the next unmasked PIC IRQ when `IF=1`; confirm DOS
  prompt/menu idle feels responsive and does not regress games.
- Hardware-test the direct-present Pico-286 video path.  Normal DOS frames now
  go directly from `SCREEN` to `driver.so`, while keyboard/menu overlays still
  use composition buffers and the disk LED uses save/restore.
- Hardware-test `R36SX_TICKS_THREAD_SLEEP_US=1000` in
  `homebrew/pico_286/r36sx_port/r36sx_linux-main.cpp`.  This reduces the
  timer/audio thread wakeup rate from up to 4000/s to about 1000/s; confirm
  audio remains stable and keyboard/timer responsiveness is still good.
- Hardware-test batched `REP MOVSB/MOVSW/STOSB/STOSW` in
  `homebrew/pico_286/r36sx_port/r36sx_cpu.c`.  The implementation now batches
  up to 1024 elements per decoded REP instruction while preserving direction
  flag, register wraparound, memory handlers, and one-element execution when
  Trap Flag is active.
- Optimize disk image I/O in `homebrew/pico_286/r36sx_port/disks-win32.c.inl`.
  Read/write multiple sectors per request instead of sector-by-sector loops,
  and consider a configurable `disk_flush=immediate|deferred` policy.
- Apply similar deferred flushing/buffering to the DOS network redirector if
  it is used for host-file access.  `network-redirector.c.inl` still flushes
  files directly after writes.
- Rate-limit input polling in `homebrew/pico_286/r36sx_minifb.c`.  It is useful
  to poll often, but it does not need to happen on every early `mfb_update()`
  return if the loop is running much faster than the display.
- Evaluate enabling `CPU_LIMIT_SHIFT_COUNT` for 286-compatible shift/rotate
  behavior.  Masking large `CL` counts can remove worst-case bit-by-bit loops.
- Experiment with build flags after functional changes are stable:
  `-O3`, LTO, and `-ffunction-sections -fdata-sections` plus linker garbage
  collection.  Measure both speed and binary size, because Zig/MIPS LTO may be
  fragile.
- Revisit CPU-specific tuning with a compiler that supports the real device
  core.  The current Windows Zig/Clang MIPS backend rejects `-mtune=74kc` with
  `unknown CPU: '74kc'`; its supported MIPS CPUs include generic `mips32r2`
  but not 74Kc.  Try SDK GCC from Linux/WSL or another MIPS GCC before adding
  `-mtune=74kc` to build scripts.
- Re-check `-fno-builtin-memset` and `-fno-builtin-memcpy` in
  `homebrew/pico_286/build_pico_286.ps1`.  They may block useful compiler
  lowering for the new bulk memory paths; keep them only for files that truly
  need them.

## Native Audio Stream Stability

- Update `homebrew/common/driver_audio.h` so native applications send silence
  to `sound_driver_playframe()` every frame, not only short effect sounds.
- `driver.so` appears to be designed for a continuous 44.1 kHz stereo stream,
  so a steady silent frame stream may make audio behavior more stable.
- After changing the helper, rebuild and test at least:
  - `homebrew/native_button_demo`
  - `homebrew/pong`
  - `homebrew/tiny_mc`

## Button Demo Exit Shortcut

- Add the same native exit shortcut used by Pong: `SELECT + START`.
- Keep or reconsider the current `FN` exit behavior after hardware testing.
- Rebuild Button Demo and update the relevant `disk_image` and patch copies.

## Shared Native Video Helper

- Extract the repeated `driver.so` video setup into a common helper/header.
- Candidate repeated logic:
  - `dlopen()` / `dlsym()` for `driver.so`
  - `video_driver_setting()`
  - `video_drivers_init()`
  - `video_driver_disp_frame()`
  - `video_driver_deinit()`
  - joypad `cube_ioctl()` setup where it naturally belongs
- Replace duplicated video initialization code in native applications with the
  shared helper after one app proves the interface.
