# DOSBox R36SX Test Overlay

Copy this directory over the original SD card contents to add a first-pass
native DOSBox test binary:

```text
MIPS_NATIVE/dosbox_r36sx/dosbox_r36sx
```

This build is an early baseline:

- official DOSBox 0.74-3 source;
- MIPS little-endian Linux executable;
- SDL 1.2 frontend from the firmware;
- forced RGB SDL output for the R36SX video mirror, avoiding the 8bpp palette
  path that can leave only the VGA cursor visible;
- direct R36SX display mirror: each DOSBox frame is converted to 640x480 RGB565
  and sent to `/mnt/sdcard/cubegm/driver.so` with `video_driver_disp_frame()`;
- direct SDL pixel-format mask conversion, with fast pass-through for RGB565
  surfaces;
- wrapper main sets `LD_LIBRARY_PATH`, changes into this directory, loads
  `dosbox.conf`, and writes `dosbox.stdout.log` / `dosbox.stderr.log`;
- `dosbox.stderr.log` includes the first 30 frame-present summaries for
  display debugging;
- no R36SX on-screen keyboard yet;
- no R36SX disk/key preset menus yet;
- conservative `dosbox.conf` with a small mounted `drive_c` folder.

Expected next porting steps are to replace or wrap SDL input/audio with the
same driver.so, keyboard, menu, screenshot, and stats helpers used by Pico-286.
