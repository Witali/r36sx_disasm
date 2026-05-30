# DOSBox R36SX Build Log

## 2026-05-30 - Initial port scaffold

Purpose:

- Start adapting official DOSBox 0.74-3 as a native R36SX/SF3000 application.
- Reuse the downloaded official source archive instead of fetching anything new.
- Prefer a conservative first build through the firmware's SDL 1.2 runtime.

Inputs:

- Official source archive:
  `internet_sources/dosbox_official/dosbox-0.74-3.real.tar.gz`
- Extracted source mirror:
  `internet_sources/dosbox_official/dosbox-0.74-3`
- Active port copy:
  `homebrew/dosbox_r36sx/dosbox-0.74-3`
- Device SDL runtime found in:
  `disk_image/cubegm/usr/lib/libSDL-1.2.so.0`

Added files:

- `homebrew/dosbox_r36sx/r36sx_port/config.h`
- `homebrew/dosbox_r36sx/build_dosbox_r36sx.ps1`
- `homebrew/dosbox_r36sx/README.md`
- `homebrew/dosbox_r36sx/BUILD_LOG.md`

Build command planned:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Important compile defaults:

- target: `mipsel-linux-gnu`
- CPU flags: `-march=mips32r2`
- optimization: `-O2`
- C++ mode: `gnu++03`
- disabled features: OpenGL, screenshots, IPX, modem, dynrec, x86 dynamic core,
  x86 FPU assembly, unaligned memory access.

First build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- failed before compiling source files;
- Zig/Clang rejected `-mtune=74kc` with `unknown CPU: '74kc'`;
- the script was changed to keep only `-march=mips32r2`.

Second build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- source compilation started successfully;
- failed in `src/dos/dos_programs.cpp`;
- DOSBox uses C++ `try` and `dynamic_cast`, so the build cannot use
  `-fno-exceptions` or `-fno-rtti`;
- those flags were removed for this port;
- `config.h` now undefines the compiler's built-in `MIPSEL` token before
  assigning DOSBox's target CPU constants.

Third build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- source compilation progressed further;
- failed in `src/gui/sdl_mapper.cpp` because SDL's sysroot headers exposed the
  X11 backend and DOSBox tried to include `X11/XKBlib.h`;
- X11 is not needed on the device, so the build now undefines
  `SDL_VIDEO_DRIVER_X11`;
- `-Wno-null-pointer-subtraction` was added to keep old DOSBox structure-offset
  macros from flooding the build log.

Fourth build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- still failed in `src/gui/sdl_mapper.cpp`;
- the actual condition was `#ifdef C_X11_XKB`, not SDL's backend define;
- DOSBox also uses `#ifdef C_HEAVY_DEBUG` and `#if defined(C_SDL_SOUND)`;
- disabled `#ifdef` features now remain undefined instead of being defined as
  `0`.

Fifth build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- all configured C++ source files compiled;
- link failed with unresolved `std::__1::*` symbols;
- cause: Zig/Clang used Zig libc++ headers while the final binary links against
  target `libstdc++`;
- the script now puts the MIPS GCC C++ include directories first and compiles
  with `-nostdinc++`.

Sixth build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- failed immediately because the GCC C++ target include path was still too
  generic;
- `bits/c++config.h` is under
  `mips-mti-linux-gnu/include/c++/6.3.0/mips-mti-linux-gnu/mipsel-r2-hard/lib`;
- the build script now uses that ABI-specific include path and adds the matching
  target `libstdc++` directory to the linker search path.

Seventh build attempt:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
```

Result:

- build succeeded;
- output: `homebrew/dosbox_r36sx/dosbox_r36sx`;
- size: `8015740` bytes;
- SHA256:
  `B1BFECC8462577EE8F2F0A2A74EB35B0035E93B18DE200FFB4F0A6C8BBC1AA66`;
- Microsoft Defender scan:
  `Scanning ... found no threats. Scan completed successfully.`;
- ELF header verified with Windows `mipsel-none-elf-readelf.exe` from the
  already scanned `tools/mips_gcc_windows` toolchain:
  `ELF32`, little-endian, MIPS, o32, `mips32r2`, interpreter `/lib/ld.so.1`;
- dynamic dependencies:
  `libSDL-1.2.so.0`, `libpthread.so.0`, `libdl.so.2`, `libm.so.6`,
  `libstdc++.so.6`, `libgcc_s.so.1`, `libc.so.6`.

Patch overlay prepared:

- `patches/disk_image_patch_dosbox_r36sx/MIPS_NATIVE/dosbox_r36sx/dosbox_r36sx`
- `patches/disk_image_patch_dosbox_r36sx/MIPS_NATIVE/dosbox_r36sx/dosbox.conf`
- `patches/disk_image_patch_dosbox_r36sx/MIPS_NATIVE/dosbox_r36sx/README.md`
- `patches/disk_image_patch_dosbox_r36sx/MIPS_NATIVE/dosbox_r36sx/drive_c/README.TXT`
- `patches/disk_image_patch_dosbox_r36sx/MANIFEST.md`

Overlay binary verification:

- SHA256 matches the build output:
  `B1BFECC8462577EE8F2F0A2A74EB35B0035E93B18DE200FFB4F0A6C8BBC1AA66`;
- Microsoft Defender scan of the overlay copy also reported no threats.

R36SX entry wrapper added:

- `homebrew/dosbox_r36sx/r36sx_port/r36sx_main.cpp`;
- `src/gui/sdlmain.cpp` is compiled with `-Dmain=dosbox_main`;
- wrapper `main()` sets `LD_LIBRARY_PATH`, changes into the app directory,
  injects `-conf dosbox.conf`, and redirects stdout/stderr to
  `/mnt/sdcard/MIPS_NATIVE/dosbox_r36sx/dosbox.*.log`.

Wrapper rebuild:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
Copy-Item -Force homebrew\dosbox_r36sx\dosbox_r36sx patches\disk_image_patch_dosbox_r36sx\MIPS_NATIVE\dosbox_r36sx\dosbox_r36sx
.\tools\scan-download.ps1 .\homebrew\dosbox_r36sx\dosbox_r36sx
.\tools\scan-download.ps1 .\patches\disk_image_patch_dosbox_r36sx\MIPS_NATIVE\dosbox_r36sx\dosbox_r36sx
```

Result:

- output size: `8022396` bytes;
- SHA256:
  `46144C0C093B58360E1AC4B78C2B665510143D1607CC218C6DC54D4B6B101ABB`;
- dynamic dependencies unchanged:
  `libSDL-1.2.so.0`, `libpthread.so.0`, `libdl.so.2`, `libm.so.6`,
  `libstdc++.so.6`, `libgcc_s.so.1`, `libc.so.6`;
- Microsoft Defender scans of both the build output and overlay copy reported
  no threats.

Device test feedback:

- the baseline SDL build reached execution but showed a black screen;
- likely cause: the firmware SDL 1.2 video path does not reliably present the
  SDL surface when DOSBox is launched as a native app.

R36SX direct-present rebuild:

```powershell
.\homebrew\dosbox_r36sx\build_dosbox_r36sx.ps1
.\tools\scan-download.ps1 .\homebrew\dosbox_r36sx\dosbox_r36sx
Copy-Item -Force homebrew\dosbox_r36sx\dosbox_r36sx patches\disk_image_patch_dosbox_r36sx\MIPS_NATIVE\dosbox_r36sx\dosbox_r36sx
.\tools\scan-download.ps1 .\patches\disk_image_patch_dosbox_r36sx\MIPS_NATIVE\dosbox_r36sx\dosbox_r36sx
tools\mips_gcc_windows\g++-mipsel-none-elf-10.3.0\bin\mipsel-none-elf-readelf.exe -d homebrew\dosbox_r36sx\dosbox_r36sx
```

Changes:

- `src/gui/sdlmain.cpp` now loads `/mnt/sdcard/cubegm/driver.so`;
- each `GFX_EndUpdate()` converts the current SDL surface to a 640x480 RGB565
  framebuffer;
- the converted framebuffer is sent to `video_driver_disp_frame()`;
- the SDL path remains compiled in for now, so this is a conservative mirror of
  the final frame rather than a full custom frontend.

Result:

- output size: `8031028` bytes;
- SHA256:
  `E43BC187D7373D117080675A35FDC5ED5B4829B22DF0EE8973DEFE12113B8FBF`;
- dynamic dependencies unchanged:
  `libSDL-1.2.so.0`, `libpthread.so.0`, `libdl.so.2`, `libm.so.6`,
  `libstdc++.so.6`, `libgcc_s.so.1`, `libc.so.6`;
- Microsoft Defender scans of both the build output and overlay copy reported
  no threats.
