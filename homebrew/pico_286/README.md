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
- `r36sx_pico286_compat.h` is forced into the build to provide POSIX prototypes
  and harmless Pico PSRAM/swap stubs for upstream branches that are parsed but
  not used by the Linux/host configuration.
- `build_pico_286.ps1` cross-compiles the upstream C/C++ emulator sources into
  a native MIPS executable named `pico_286`.

The build script leaves upstream source files untouched.  It writes temporary
patched copies into `homebrew/pico_286/obj/`:

- `r36sx_linux-main.cpp` adds byte-pointer casts for renderer `VIDEORAM`
  arithmetic that Clang refuses in C++.
- `r36sx_cpu.c` changes the host disk image paths from `../fdd0.img`,
  `../fdd1.img`, `../hdd.img`, and `../hdd2.img` to local files in the app
  directory, matching TinyMC's `chdir()` before launch.

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
put optional images next to the executable as `fdd0.img`, `fdd1.img`, `hdd.img`,
and `hdd2.img`.  The upstream network redirector still maps DOS drive H: to
`/tmp/`.

Build:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

`zig objcopy --strip-all` currently reports `error: unimplemented` for this
MIPS ELF.  The script therefore leaves the executable unstripped by default.
Pass `-TryStrip` only when testing a newer Zig that supports stripping it.

Before using an updated `internet_sources/pico-286` checkout, scan it with:

```powershell
.\tools\scan-download.ps1 .\internet_sources\pico-286
```
