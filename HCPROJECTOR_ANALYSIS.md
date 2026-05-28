# hcprojector Analysis

Date: 2026-05-28

## Input

- Binary: `disk_image/rootfs/usr/bin/hcprojector`
- Size: 14,737,552 bytes
- SHA256: `95B89D1503CBD281A1C3F7C1C14D87763F046CD29D217DCEE0BC7FBD51D70801`
- Ghidra language: `MIPS:LE:32:default`
- Image base: `0x00400000`

## Commands

Full export:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects hcprojector_analysis -import .\disk_image\rootfs\usr\bin\hcprojector -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\hcprojector -deleteProject
```

Forced decompilation of thread entrypoints that Ghidra did not initially turn
into functions:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects hcprojector_forced -import .\disk_image\rootfs\usr\bin\hcprojector -scriptPath .\ghidra_scripts -postScript DecompileAddresses.java .\ghidra_exports\hcprojector\forced_threads.c 0042a9a0 0042af20 00427414 00420470 004220f0 0044d554 -deleteProject
```

Generated files:

- `ghidra_exports/hcprojector/summary.txt`
- `ghidra_exports/hcprojector/functions.tsv`
- `ghidra_exports/hcprojector/symbols.tsv`
- `ghidra_exports/hcprojector/disassembly.s`
- `ghidra_exports/hcprojector/decompiled_all.c`
- `ghidra_exports/hcprojector/forced_threads.c`

These exports are ignored by git because `ghidra_exports/` can become large.

## Matching Source Tree

The binary closely matches the local HCRTOS source tree:

- `internet_sources/hcrtos/components/applications/apps-projector/source/hcprojector_app/projector.c`
- `internet_sources/hcrtos/components/applications/apps-projector/source/hcprojector_app/public/com_api.c`
- `internet_sources/hcrtos/components/applications/apps-projector/source/hcprojector_app/power.c`

Confirmed matching strings/functions include:

- `Welcome to Hichip world!`
- `stroage_hotplug_handle`
- `hotplug_receive_event_func`
- `api_osd_show_onoff`
- `/tmp/hotplug.socket`
- `ACTION=mount`
- `ACTION=umount`

The stock console binary also contains an extra vendor/game-mode hook that is
not present in the public HCRTOS source snapshot:

- `/mnt/sdcard/cubegm/icube_start.sh &`
- `find_icube_enter_multi_os`

## What hcprojector Does

`hcprojector` is the stock Hichip projector/media UI application, not the game
launcher itself. It is a dynamic MIPS32 little-endian ELF that uses LVGL,
ffplayer/media libraries, Hichip display/audio/input APIs, persistent settings,
FOTA/update code, and local media-player UI modules.

Main startup flow:

1. Initialize timing and internal state.
2. Load factory/system parameters through `/dev/persistentmem`.
3. Initialize media/ffplayer support.
4. Initialize LVGL and the framebuffer UI.
5. Open `/dev/input/event0` and `/dev/input/event1`.
6. Set the framebuffer mode to 640x480 RGB.
7. Initialize setup, channel, HDMI/CVBS, local media, main page, and volume UI
   screens.
8. Enter the main loop.

Main loop behavior:

- Processes control messages.
- Handles USB/storage hotplug state.
- Feeds the watchdog periodically.
- Runs the LVGL task/timer handler.
- Sleeps for about 10 ms per loop with `usleep(10000)`.

## Input Handling

The program has two layers of input handling:

- `projector.c` opens `/dev/input/event0` and `/dev/input/event1` for the LVGL
  keypad device.
- A lower-level key task scans `/dev/input/event0` through `/dev/input/event9`
  with `poll()` and stores input events in a small queue.

The hotkey preprocessing handles projector-style keys:

- power: enters standby;
- menu/channel: opens channel screen;
- setup: opens setup screen;
- volume up/down and mute: updates audio volume and the LVGL volume overlay;
- rotate/flip: changes display orientation.

This code is useful as a reference for our own input mapping, but it is not the
same runtime path as `rkgame`/Tiny MC once `hcprojector` has been killed.

## Hotplug and Mount Handling

`api_system_init()` starts hotplug handling:

- runs `/etc/wifiprobe.sh &`;
- creates `/tmp/hotplug.socket`;
- registers an epoll loop;
- registers HDMI notifier messages through `/dev/hdmi`;
- handles socket messages such as `ACTION=mount INFO=%s` and
  `ACTION=umount INFO=%s`.

For mount/umount messages, it updates the internal partition list and sends UI
control messages such as USB disk plugin/plugout. The main loop then calls
`stroage_hotplug_handle()` to refresh the local media pages or return from a
media screen if the active storage disappears.

## Power, Display, and Watchdog

Important devices used by `hcprojector`:

- `/dev/fb0`: LVGL framebuffer and OSD blank/unblank via `FBIOBLANK`.
- `/dev/dis`: display output/scaling/aspect control.
- `/dev/standby`: system standby entry.
- `/dev/lvds`, `/dev/mipi`, `/dev/lcddev`, `/dev/backlight`: panel/backlight
  shutdown and brightness control.
- `/dev/sndC0i2so`: volume/mute/audio path control.
- `/dev/persistentmem`: persistent projector settings.
- `/dev/watchdog`: watchdog setup/feed in the matching HCRTOS source.

The matching source sets watchdog timeout to 30,000 ms and feeds it from the
main loop after about 80 iterations. This matches our earlier observation that
custom replacements need to keep both the `icube` heartbeat and the hardware
watchdog behavior in mind.

## Transition to RKGame

The game transition is implemented as a vendor-specific extension in the stock
binary:

- `FUN_0042f000` prints `find_icube_enter_multi_os`;
- it hides/blanks the OSD through `/dev/fb0` and `FBIOBLANK`;
- then it executes:

```c
system("/mnt/sdcard/cubegm/icube_start.sh &");
```

There is also a signal/exit handler that performs cleanup and can run
`icube_start.sh` for specific signals before exiting.

The practical chain is:

```text
hcprojector
 -> /mnt/sdcard/cubegm/icube_start.sh
 -> killall hcprojector
 -> /mnt/sdcard/cubegm/icube
 -> /mnt/sdcard/cubegm/rkgame
```

Returning in the opposite direction is handled by:

```text
/mnt/sdcard/cubegm/icubemp_start.sh
 -> killall rkgame rkgame.stock tiny_mc icube
 -> /usr/bin/hcprojector &
```

## Limitations

Ghidra did not load external libraries into the same project, so imports from
`liblvgl.so`, `libffplayer.so`, `libhudi.so`, `libhcfota.so`, `libglist.so`,
`libfreetype.so.6`, `libspectrum.so`, `libzip.so.5`, `libstdc++.so.6`,
`libpthread.so.0`, `libm.so.6`, and `libc.so.6` remain unresolved wrappers.

The public HCRTOS source tree is a very close map of the stock binary, but not
an exact source match: the `icube_start.sh` game-mode hook appears to be a
vendor/custom addition in the console build.
