# pico-286 R36SX Port

This directory contains the first native R36SX/SF3000-style port layer for
[xrip/pico-286](https://github.com/xrip/pico-286).  The upstream project is MIT
licensed and is stored locally under `internet_sources/pico-286`, which is
ignored by git because it is a downloaded third-party source mirror.

The port reuses the upstream Linux/host emulator core and replaces only the host
integration pieces:

- `r36sx_minifb.c` implements the small upstream `MiniFB` API through
  `/mnt/sdcard/cubegm/driver.so` video and joypad input.
- `r36sx_linux_audio.c` implements upstream `linux-audio.h` through
  `driver.so` `sound_driver_playframe()`, preserving mixer volume after audio
  initialization.
- `r36sx_minifb.c` also draws a blinking red disk activity indicator in the
  lower-right corner when the emulator reads or writes disk image sectors.
- `r36sx_pico286_compat.h` is forced into the build to provide POSIX prototypes
  and harmless Pico PSRAM/swap stubs for upstream branches that are parsed but
  not used by the Linux/host configuration.
- `build_pico_286.ps1` cross-compiles the upstream C/C++ emulator sources into
  a native MIPS executable named `pico_286`.

The build script leaves upstream source files untouched.  It writes temporary
patched copies into `homebrew/pico_286/obj/`:

- `r36sx_linux-main.cpp` adds byte-pointer casts for renderer `VIDEORAM`
  arithmetic that Clang refuses in C++ and adds early startup/thread/main-loop
  debug logs.  It also initializes the host memory backend pointers before
  `reset86()`, matching the upstream Win32 entrypoint.  BIOS video modes
  `0x20` and `0x30` are aliased to the normal 80x25 text renderer and scaled
  into the 640x480 output.  The debug build also logs changed text rows from
  the emulated screen area with `screen_text:*` prefixes.  Normal 40/80-column
  text modes now read from the same logical `VIDEORAM` cell layout used by the
  emulator and the upstream Win32 renderer.
- `r36sx_cpu.c` changes the host disk image paths from `../fdd0.img`,
  `../fdd1.img`, `../hdd.img`, and `../hdd2.img` to local files in the app
  directory, matching TinyMC's `chdir()` before launch.  It also adds missing
  text-mode BIOS `INT 10h` services used by BIOS/DOS boot screens: cursor
  shape/position, scroll/clear window, read/write character and attribute,
  teletype output, mode query, active page selection, and write string.
- `disks-win32.c.inl` is copied into `obj/` and patched so sector reads and
  writes notify the R36SX MiniFB layer for the on-screen disk activity LED.
- `r36sx_ports.c` routes OPL output through a temporary `int32_t` sample buffer
  before clamping to the emulator's `int16_t` stereo buffer, avoiding the
  upstream host cast from `int16_t *` to `int32_t *` on MIPS.

Input mapping for the first build:

- D-pad: PC cursor keys.
- A or Start: Enter.
- B or Select: Escape.
- X: Space.
- Y: Ctrl.
- L: Alt.
- R: Shift.
- L2/R2: F1/F2.
- Fn: F3.
- Select+Start: exit back to TinyMC.

The upstream PC disk images are still expected by the emulator.  In this port,
put images next to the executable as `fdd0.img`, `fdd1.img`, `hdd.img`, and
`hdd2.img`.  A compatibility `fdd2.img` can also be placed there for older
test builds that tried to attach a third floppy image.  The upstream network
redirector still maps DOS drive H: to `/tmp/`.

The local test image set uses official FreeDOS 1.4 Floppy Edition images:

- `fdd0.img`: `144m/x86BOOT.img`, bootable FreeDOS floppy.
- `fdd1.img`: `144m/x86DSK01.img`, first FreeDOS package floppy.
- `fdd2.img`: `144m/x86DSK02.img`, second FreeDOS package floppy.
- `hdd.img`: blank raw hard disk image, 65 cylinders, 16 heads, 63 sectors.
- `hdd2.img`: blank raw hard disk image with the same geometry.

Build:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

The current build is a debug build.  On the device it writes:

```text
/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log
```

If that file cannot be opened, it writes:

```text
/mnt/sdcard/pico_286.log
```

Screen text diagnostics appear only when the text contents change.  The most
useful tags are:

- `screen_text:logical-b800`: characters read from logical B800-style
  `VIDEORAM` cells.
- `screen_text:renderer-byte-view`: characters read the same way the current
  renderer sees them.
- `screen_text:logical-small-text` and `screen_text:renderer-byte-small-text`:
  the same comparison for Pico-286's small text modes `0x77` and `0x78`.

The old on-screen debug text area is disabled.  Runtime diagnostics still go to
`pico_286.log`, but the lower part of the framebuffer is cleared instead of
drawing `DEBUG_VRAM` contents.

`zig objcopy --strip-all` currently reports `error: unimplemented` for this
MIPS ELF.  The script therefore leaves the executable unstripped by default.
Pass `-TryStrip` only when testing a newer Zig that supports stripping it.

Before using an updated `internet_sources/pico-286` checkout, scan it with:

```powershell
.\tools\scan-download.ps1 .\internet_sources\pico-286
```

Downloaded FreeDOS archives/images should also be scanned before copying them
into `disk_image`.
