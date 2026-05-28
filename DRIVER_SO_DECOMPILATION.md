# driver.so Decompilation Notes

Date: 2026-05-28

Input binary:

```text
disk_image/cubegm/driver.so
```

Fresh Ghidra export:

```text
ghidra_exports/driver/decompiled_all.c
ghidra_exports/driver/disassembly.s
ghidra_exports/driver/functions.tsv
ghidra_exports/driver/symbols.tsv
ghidra_exports/driver/summary.txt
```

Ghidra identified the binary as `MIPS:LE:32:default`. The binary MD5 reported
by Ghidra is:

```text
f19ff1cebbb57a9a657084e1ce27b02c
```

The full decompile was regenerated with:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects driver_decompile_refresh -import .\disk_image\cubegm\driver.so -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\driver -deleteProject
```

`decompiled_key_functions.c` is empty because the current export script's
"key function" list is tuned for LibRetro entry points, not for `driver.so`.
Use `decompiled_all.c`, `functions.tsv`, and `symbols.tsv` for this binary.

## What driver.so Is

`driver.so` is the vendor hardware abstraction library used by `rkgame`. It
wraps:

- framebuffer and display output through `/dev/fb0`, `/dev/dis`, `/dev/ge`;
- audio output through `/dev/auddec` plus volume/output control through
  `/dev/sndC0i2so`;
- input shared memory through `/tmp/joy_key`;
- game status shared memory through `/tmp/cubegame_status`;
- process messages through `/tmp/process_msg` and `/tmp/cube_process_msg`;
- backlight through `/dev/backlight`;
- boot slot / standby control through `/dev/standby`.

## Important Functions

Video:

- `video_drivers_init` at `000140d0`
- `video_driver_deinit` at `00014250`
- `video_driver_disp_frame` at `000142e0`
- `video_driver_setting` at `000144ac`
- `video_driver_setmode` at `00014504`
- `video_driver_get_size` at `000145bc`
- `video_driver_get_screen_size` at `000145e4`
- `fbdev_init` at `00016a34`
- `fbdev_draw_frame` at `00015cc8`
- `fb_render_task` at `00014fd0`
- `fb_paint_task` at `000152d8`

Audio:

- `sound_driver_init` at `00018950`
- `sound_driver_deinit` at `00018aa4`
- `sound_driver_playframe` at `00018b18`
- `sound_driver_flush` at `00018bd8`
- `pcm_decoder_init` at `000182e0`
- `pcm_get_buffering_percent` at `000184d0`
- `pcm_decode` at `00018538`

Input / status / control:

- `cube_ioctl` at `00013ae8`
- `cube_shmget_init` at `00012e74`
- `key_task` at `000146fc`
- `exit_game` at `000147e4`
- `enter_multios_app` at `00014d84`
- `set_bootup_slot` at `00014670`
- `set_backlight_value` at `00012dd8`
- `get_volume` at `00012c40`

## cube_ioctl Map

The central control entry is `cube_ioctl(command, arg, ...)`.

Observed commands:

```text
0x20010201  set backlight value through /dev/backlight
0x20010203  set audio volume through /dev/sndC0i2so ioctl 0x8001080b
0x40010204  get audio volume through /dev/sndC0i2so ioctl 0x4001080c
0x20010205  save aspect ratio and update display aspect when HDMI is active
0x40010206  read persistent memory value
0x40050202  get saved backlight value, unless HDMI is active
0x40050207  read current joy key word from /tmp/joy_key shared memory
0x40050208  read game status from /tmp/cubegame_status shared memory
0x40050209  return pointer/address of /tmp/joy_key shared memory
```

This confirms that native applications can use the same `/tmp/joy_key` shared
memory path as the original game stack. `cube_shmget_init()` creates/attaches
shared memory via `ftok(path, 0x61)`, `shmget(...)`, and `shmat(...)`.

## Video Path

`video_drivers_init()` allocates a small frame descriptor, derives bytes per
pixel from the color mode, then calls `fbdev_init(width, height, color_mode)`.

`fbdev_init()`:

- queries display parameters through `/dev/dis`;
- initializes the vendor GE backend through `/dev/ge`;
- opens `/dev/fb0`;
- uses framebuffer ioctls and `mmap`;
- clears the framebuffer;
- starts two worker threads: `fb_render_task` and `fb_paint_task`.

`video_driver_disp_frame(buffer, width, height, pitch)` is the main frame
submission function. When frame dimensions change it tears down and reinitializes
the framebuffer path. It has special handling for:

- `1280x720`;
- width `0x356` (`854`);
- otherwise it falls back to `640x480`.

After that it calls `fbdev_draw_frame(...)`, which queues/copies the frame into
the framebuffer pipeline and signals the render thread. If the internal queue is
full, it prints `drop frame`.

## Audio Path

`sound_driver_init(mode, rate, channels)` calls:

```text
pcm_decoder_init(0x10, channels, rate)
```

For the stock game path this is normally 16-bit stereo at 44100 Hz. The decoder
opens:

```text
/dev/auddec
```

and configures/starts it with ioctls including:

```text
0x82780301
0x20000304
0x40041503
```

`sound_driver_init()` also reads saved volume and applies output control through:

```text
/dev/sndC0i2so
```

with ioctls including:

```text
0x8001080b
0x2000080f
```

`sound_driver_playframe(samples, frames)` checks decoder buffering first. It
only sends more PCM when buffering is below `0x5b` (91%). The size passed to
`pcm_decode()` is:

```text
frames << 2
```

So the public function expects 16-bit stereo frames: 4 bytes per frame.

## Exit / Return Path

`exit_game()`:

- calls `set_bootup_slot(0x11)`;
- runs `/mnt/sdcard/cubegm/avexit` if present;
- executes `reboot -f`.

`enter_multios_app()`:

- deinitializes framebuffer and audio;
- updates the multios app state;
- calls `set_bootup_slot(0x21)`;
- runs `/mnt/sdcard/cubegm/avexit` if present;
- changes aspect/OSD state;
- starts `/mnt/sdcard/cubegm/icubemp_start.sh &`.

This explains why stock app transitions often reboot or bounce through shell
scripts instead of simply returning to a parent process.

## Power Notes

The string `/dev/standby` is present, but the decompiled code shows it is used
for boot-slot control in `set_bootup_slot()`. This decompilation did not reveal
a MIPS `wait`/halt loop, cpufreq control, or a userspace CPU sleep mechanism in
`driver.so`.

## Practical Conclusions For Our Native Apps

- Input should continue to use `/tmp/joy_key` shared memory. This matches the
  stock `cube_ioctl(0x40050209)` path.
- Audio should prefer the exported `sound_driver_*` functions from
  `driver.so`, not a hand-rolled direct `/dev/auddec` path.
- Native apps should send continuous 44100 Hz stereo frames, including silence,
  so the decoder does not underrun between short effects.
- Exiting to the original shell is not a normal `return` in the stock stack.
  The vendor flow often uses `icubemp_start.sh`, boot-slot ioctls, and sometimes
  `reboot -f`.
- For exact display behavior, `video_driver_disp_frame()` and `fbdev_init()`
  are the key routines to keep comparing against our native framebuffer code.
