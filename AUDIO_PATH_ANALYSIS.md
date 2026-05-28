# Audio Path Analysis

Date: 2026-05-28

Purpose: compare stock LibRetro audio output with the current standalone native
audio helper used by TinyMC-launched programs.

## Stock LibRetro Path

`rkgame` does not open the audio device directly for LibRetro cores. It loads
`cubegm\driver.so` and resolves the sound API from that library:

- `sound_driver_init`
- `sound_driver_playframe`
- `pcm_get_buffering_percent`
- `sound_driver_flush`
- `sound_driver_deinit`

Evidence:

- `ghidra_exports\rkgame\decompiled_all.c`
  - `InitSound()` resolves the symbols above through `dlsym()`.
  - It calls `sound_driver_init(USE_HDMI_OUT, 0xac44, 2)`, where `0xac44` is
    44100 Hz and `2` is stereo.
  - `Load_Proc1()` gives LibRetro cores `retro_set_audio_sample_batch` with
    `PlayFrame`.
  - `PlayFrame()` calls `PlaySound()`.
  - MIPS disassembly confirms `PlayFrame()` does not overwrite `$a0/$a1`
    before calling `PlaySound()`, so the original LibRetro callback arguments
    reach `sound_driver_playframe(samples, frames)`.

## driver.so Audio Backend

`driver.so` uses `/dev/auddec` for PCM output:

- `ghidra_exports\driver\decompiled_all.c`
  - `pcm_decoder_init()` opens `/dev/auddec`.
  - It configures codec `0x10000`, 16-bit samples, stereo, and 44100 Hz.
  - It calls ioctl `0x82780301` (`AUDDEC_INIT`-like), then ioctl `0x20000304`
    (`AUDDEC_START`-like).
  - It queries audio buffer size with ioctl `0x40041503`.
  - `pcm_decode()` writes a 16-byte `AvPktHd`-style packet header, then the PCM
    payload.

Important difference from our first native helper:

- `driver.so` sets additional decoder config fields that our helper currently
  does not mirror exactly:
  - offset `0x18` = `1`
  - offset `0x1c` = `100`
  - offset `0x240` = `0`
  - offset `0x244` = `100`
- `sound_driver_playframe()` calls `pcm_get_buffering_percent()` and only writes
  a frame when buffering is below `0x5b` (91%). This gives the decoder a simple
  backpressure mechanism.
- `pcm_decode()` retries partial writes every 20 ms until the full 16-byte
  header and full PCM payload are accepted.

`driver.so` also touches `/dev/sndC0i2so`:

- `sound_driver_init()` reads the saved volume and opens `/dev/sndC0i2so`.
- It applies volume/output ioctls `0x8001080b` and `0x2000080f`.
- Other `driver.so`/`hcprojector` paths use `/dev/sndC0i2so` for volume, mute,
  and output route control.

## Current Native Path

`homebrew\common\native_audio.h` bypasses `driver.so` and talks directly to
`/dev/auddec`:

- opens `/dev/auddec`;
- calls ioctl `0x82780301`;
- calls ioctl `0x20000304`;
- writes generated 44.1 kHz stereo PCM packets.

This works, but it is not identical to the stock LibRetro path:

- it does not use `driver.so`'s `sound_driver_playframe()` buffering gate;
- it does not query decoder buffer size or buffering percent;
- it does not retry partial writes for as long as stock `pcm_decode()`;
- it does not apply `/dev/sndC0i2so` volume/output ioctls;
- it only feeds audio while a generated effect is active, so the hardware buffer
  can drain between short sounds.

## Likely Causes Of Worse Native Sound

The lower-level audio device is mostly the same (`/dev/auddec`), but the stock
LibRetro route is better buffered and goes through `driver.so`'s audio control
layer. The rougher native sound is likely caused by a combination of:

1. underruns between short native sound effects;
2. missing `driver.so` decoder config fields;
3. missing `/dev/sndC0i2so` route/volume setup;
4. no `pcm_get_buffering_percent()` backpressure;
5. harsh square-wave generated test sounds, while real LibRetro cores usually
   feed continuous emulator audio.

## Recommended Fix Direction

Prefer reusing the same path as `rkgame`:

1. In native programs, resolve `sound_driver_init`, `sound_driver_playframe`,
   `pcm_get_buffering_percent`, `sound_driver_flush`, and `sound_driver_deinit`
   from `cubegm\driver.so`.
2. Call `sound_driver_init(USE_HDMI_OUT, 44100, 2)`.
3. Mix native effects into a continuous 735-frame stereo buffer every 60 Hz.
4. Always submit a frame, including silence, through `sound_driver_playframe()`.
5. Use `sound_driver_flush()`/`sound_driver_deinit()` on exit.

If direct `/dev/auddec` remains necessary, update `native_audio.h` to mirror
`pcm_decoder_init()` and `pcm_decode()` more closely, including the extra config
fields, buffering-percent ioctl, longer write retry loop, and `/dev/sndC0i2so`
volume/output setup.
