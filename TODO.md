# TODO

## Pico-286 Performance Optimizations

- Add audio chip auto-gating in
  `homebrew/pico_286/r36sx_port/r36sx_ports.c`.
  `get_sound_sample()` currently mixes OPL, PC speaker, SN76489, MIDI/CMS
  paths every 44.1 kHz sample even when most chips are inactive.  Track writes
  to the relevant ports and skip inactive chip mixers.
- Hardware-test the `HLT` idle behavior implemented in
  `homebrew/pico_286/r36sx_port/r36sx_cpu.c`.  The CPU now leaves `exec86()`
  while halted and wakes on the next unmasked PIC IRQ when `IF=1`; confirm DOS
  prompt/menu idle feels responsive and does not regress games.
- Follow up on the RGB565 video present cache added in commit `32e7a3a`.
  Test on hardware, then consider rendering directly to RGB565 for modes that
  do not need keyboard scaling or overlays.
- Try increasing `R36SX_TICKS_THREAD_SLEEP_US` in
  `homebrew/pico_286/r36sx_port/r36sx_linux-main.cpp` from `250` to `500` or
  `1000`.  The current setting can wake the timer/audio thread up to 4000
  times per second.
- Batch common `REP MOVS/STOS` string operations in
  `homebrew/pico_286/r36sx_port/r36sx_cpu.c`, especially RAM-to-RAM and
  RAM-to-video copies.  Keep batches capped so timer and keyboard interrupts
  remain responsive.
- Optimize disk image I/O in `homebrew/pico_286/r36sx_port/disks-win32.c.inl`.
  Read/write multiple sectors per request instead of sector-by-sector loops,
  and consider a configurable `disk_flush=immediate|deferred` policy.
- Rate-limit input polling in `homebrew/pico_286/r36sx_minifb.c`.  It is useful
  to poll often, but it does not need to happen on every early `mfb_update()`
  return if the loop is running much faster than the display.
- Evaluate enabling `CPU_LIMIT_SHIFT_COUNT` for 286-compatible shift/rotate
  behavior.  Masking large `CL` counts can remove worst-case bit-by-bit loops.
- Experiment with build flags after functional changes are stable:
  `-O3`, LTO, and `-ffunction-sections -fdata-sections` plus linker garbage
  collection.  Measure both speed and binary size, because Zig/MIPS LTO may be
  fragile.

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
