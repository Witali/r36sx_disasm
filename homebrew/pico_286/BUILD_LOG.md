# pico-286 Build Log

## 2026-05-28

- Reused the already downloaded `internet_sources/pico-286` checkout at commit
  `2ba88bf` (`MIDI: add per-instrument ADSR envelope with GM program table`).
- Scanned the downloaded source mirror:

```powershell
.\tools\scan-download.ps1 .\internet_sources\pico-286
```

Result: Microsoft Defender reported no threats.

- Checked upstream structure and confirmed the useful target is the Linux/host
  build path, not the RP2040/RP2350 Pico firmware path.
- Verified the project license is MIT.
- Verified the local MIPS sysroot has `libstdc++.so`, `libgcc_s.so`, and GCC
  6.3.0 C++ headers under `tools/mipsel-buildroot-linux-gnu_sdk-buildroot`.
- Added:
  - `r36sx_minifb.c`: driver.so framebuffer and input backend for upstream
    `MiniFB.h`.
  - `r36sx_linux_audio.c`: driver.so audio backend for upstream
    `linux-audio.h`.
  - `build_pico_286.ps1`: Zig-based MIPS cross-build script.

Initial build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

The first script revision used `-fno-pic` with `zig cc`; Zig rejected the
selected Linux target as requiring position-independent code.  The build script
was changed to emit PIC objects and to throw on any nonzero native tool exit
code, so failed compiles/links can no longer fall through to the final message.

The next build showed that upstream mixes `#if PICO_ON_DEVICE` with
`#ifdef PICO_ON_DEVICE`.  Defining `PICO_ON_DEVICE=0` still enabled the
`#ifdef` branch and pulled in `psram_spi.h`, so the script now leaves
`PICO_ON_DEVICE` undefined for the host/Linux path.

The following compile reached `network-redirector.c.inl` and strict Clang C17
diagnostics.  Added `r36sx_pico286_compat.h` as a forced include for POSIX file
operation prototypes and no-op Pico PSRAM/swap helper stubs.  Those helper
names remain in parsed dead branches of the host build, while actual host EMS
storage uses the upstream `EMS[]` array because `butter_psram_size` is `1`.

The next compile reached `findfirst/findfirst.c`; added `_GNU_SOURCE` and
`_DEFAULT_SOURCE` before system headers in the forced include so glibc exposes
`strdup`, `realpath`, `readdir_r`, `fstatat`, and `dirfd` prototypes.

The next C++ compile reached `slot_render.cpp` and exposed missing upstream
`EMU8950_*` compile definitions.  Mirrored the common OPL2 definitions from
upstream `CMakeLists.txt` so `slot_render.h` and `slot_render.cpp` agree on the
optimized `SLOT_RENDER` layout.

The next C compile reached `emu8950.c` and needed upstream's `-fms-extensions`
flag.  `emu8950.h` embeds `struct SLOT_RENDER;` as an anonymous member in
`OPL_SLOT`, which Clang only accepts with that compatibility extension.

The next C++ compile reached `linux-main.cpp`, whose renderer uses C-style
implicit conversions from `uint32_t *VIDEORAM` to `uint8_t *`.  Added
`-fpermissive` to match the upstream host build's permissive C++ expectations.

Clang still rejects those pointer conversions even with `-fpermissive`, so the
build script now generates `homebrew/pico_286/obj/r36sx_linux-main.cpp` from the
upstream file and mechanically casts `VIDEORAM` to `uint8_t *` in renderer
pointer arithmetic.  The upstream checkout remains untouched.

TinyMC launches native programs after `chdir()` into the selected file's
directory.  The upstream Linux `cpu.c` host path inserts disk images as
`../fdd0.img`, `../fdd1.img`, `../hdd.img`, and `../hdd2.img`, which would point
to `/mnt/sdcard/MIPS_NATIVE/` instead of the app folder.  The build script now
generates `obj/r36sx_cpu.c` with those paths changed to local `fdd0.img`,
`fdd1.img`, `hdd.img`, and `hdd2.img`.

The first successful link produced `homebrew/pico_286/pico_286` as a 32-bit
little-endian MIPS ELF executable.  Raw size before stripping was 7,876,176
bytes.  Dynamic strings show dependencies on `/lib/ld.so.1`, `libc.so.6`,
`libdl.so.2`, `libgcc_s.so.1`, `libm.so.6`, `libpthread.so.0`, and
`libstdc++.so.6`.  Added `zig objcopy --strip-all` to the build script.

`zig objcopy --strip-all` then failed with `error: unimplemented` for this MIPS
ELF.  The strip step is now behind an explicit `-TryStrip` switch, so normal
builds finish cleanly and keep the unstripped executable.

Final clean build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,876,196 bytes after the local disk-path build patch
- ELF header bytes: `7f 45 4c 46 01 01 01 00 ... 02 00 08 00`, meaning ELF32
  little-endian executable for MIPS.
- Dynamic dependencies found by binary string scan: `/lib/ld.so.1`,
  `libc.so.6`, `libdl.so.2`, `libgcc_s.so.1`, `libm.so.6`,
  `libpthread.so.0`, `libstdc++.so.6`.
- Binary string checks confirm local disk image names `fdd0.img`, `fdd1.img`,
  `hdd.img`, and `hdd2.img`, with no `../` disk paths left.
- Binary string checks also confirm `driver.so`, `cube_ioctl`,
  `video_driver_disp_frame`, and `sound_driver_playframe`.

## 2026-05-28 debug logging build

Device test result: the first `pico_286` build returned to TinyMC/shell without
showing enough information in `tiny_mc.log`.

Added DEBUG logging to the R36SX port layer and rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

The build script now passes:

```text
-DDEBUG=1
```

The application writes its own log to:

```text
/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log
```

If that path cannot be opened, it falls back to:

```text
/mnt/sdcard/pico_286.log
```

New log coverage:

- early `main()` start and normal cleanup;
- fatal signals: `SIGSEGV`, `SIGBUS`, `SIGILL`, `SIGABRT`;
- MiniFB/driver.so path loading and resolved display symbols;
- framebuffer initialization return code;
- `/tmp/joy_key` shared-memory address returned by `cube_ioctl`;
- raw key state changes and Select+Start exit;
- audio `driver.so` loading, bind/start/close, and early write failures;
- upstream `printf`/`printf_` output line capture through `_putchar`;
- int 19h disk image attach results for `fdd0.img`, `fdd1.img`, `hdd.img`,
  and `hdd2.img`.

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,893,192 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 device log follow-up build

Device log received from `/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log`:

```text
main: pthread_create sound=0 ticks=0
main: fatal signal 11
```

That means `driver.so` video, joypad shared memory, OPL allocation, emulator
reset, and `driver.so` audio initialization all completed.  The crash happens
immediately after the sound and timer threads start, before the previous build
could prove whether the first `exec86()` or one of the helper threads caused
the segmentation fault.

Changes in this build:

- Added early logs at the start of `sound_thread()` and `ticks_thread()`.
- Added first-iteration logs before and after `exec86()` and `mfb_update()`.
- Added first audio-buffer write logs from `sound_thread()`.
- Added `-fno-strict-aliasing` to reduce risk from upstream host pointer casts.
- Added generated `obj/r36sx_ports.c`, replacing `get_sound_sample()` so OPL
  audio renders into an `int32_t` temporary and is clamped into the `int16_t`
  stereo buffer.  The upstream host code casts `int16_t *` to `int32_t *`,
  which is fragile on MIPS.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,894,916 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Next device test expectation: if it still crashes, the log should now show
whether the last successful point was `ticks_thread`, `sound_thread`, first
`exec86()`, or first `mfb_update()`.

## 2026-05-28 memory backend fix

Device log after the follow-up build:

```text
main: pthread_create sound=0 ticks=0
main: before exec loop=0 videomode=0x3
main: fatal signal 11
```

This narrowed the crash to the first `exec86()` call.  Inspecting upstream
entrypoints showed the cause: `win32-main.cpp` and `pico-main.c` initialize the
memory backend function pointers before `reset86()`:

```c
write86 = write86_ob;
writew86 = writew86_ob;
writedw86 = writedw86_ob;
read86 = read86_ob;
readw86 = readw86_ob;
readdw86 = readdw86_ob;
```

The Linux entrypoint we reuse did not do that, leaving `read86` and friends as
null pointers.  The first BIOS opcode fetch inside `exec86()` then jumped
through `read86 == NULL` and produced `SIGSEGV`.

Fix:

- Patch generated `obj/r36sx_linux-main.cpp` to initialize the same host memory
  backend pointers before clearing the screen and calling `reset86()`.
- Add a log line showing the resolved `read86` and `write86` pointer values.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,895,136 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 FreeDOS disk images

Device test reached the BIOS disk attach stage and reported missing disk image
files:

```text
disk error: cannot open disk file fdd0.img for drive 00h
disk error: cannot open disk file fdd1.img for drive 01h
disk error: cannot open disk file hdd.img for drive 02h
disk error: cannot open disk file fdd2.img for drive 03h
```

Downloaded the official FreeDOS 1.4 Floppy Edition archive:

```powershell
Invoke-WebRequest -Uri "https://download.freedos.org/1.4/FD14-FloppyEdition.zip" -OutFile ".tmp\FD14-FloppyEdition.zip"
```

Scanned the downloaded archive and the extracted files:

```powershell
.\tools\scan-download.ps1 .\.tmp\FD14-FloppyEdition.zip
Expand-Archive -Path .\.tmp\FD14-FloppyEdition.zip -DestinationPath .\.tmp\FD14-FloppyEdition -Force
.\tools\scan-download.ps1 .\.tmp\FD14-FloppyEdition
```

Result: Microsoft Defender reported no threats.

Created and copied these images into both `disk_image/MIPS_NATIVE/pico_286/`
and `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/`:

- `fdd0.img`: copied from FreeDOS `144m/x86BOOT.img`, 1,474,560 bytes.
- `fdd1.img`: copied from FreeDOS `144m/x86DSK01.img`, 1,474,560 bytes.
- `fdd2.img`: copied from FreeDOS `144m/x86DSK02.img`, 1,474,560 bytes.
- `hdd.img`: blank raw image, 33,546,240 bytes.
- `hdd2.img`: blank raw image, 33,546,240 bytes.

The blank hard disk size is exactly `65 * 16 * 63 * 512` bytes, matching the
host disk geometry Pico-286 calculates for hard disk images.  The copied
`fdd0.img` boot sector ends with `55 AA`.

Scanned the final image directories:

```powershell
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286
```

Result: Microsoft Defender reported no threats.
