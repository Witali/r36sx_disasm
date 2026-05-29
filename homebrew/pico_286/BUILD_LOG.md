# pico-286 Build Log

## 2026-05-29 source extraction build

Purpose:

Move R36SX Pico-286 source patches out of `build_pico_286.ps1`. The build
script had grown several `New-Patched-*` helpers that read upstream files,
performed string replacements, and wrote generated patched sources into `obj/`.
That made the real port changes hard to review and maintain.

Implementation:

- Added `homebrew/pico_286/r36sx_port/` as the explicit R36SX port source tree.
- Moved the generated patched files into that directory:
  - `r36sx_linux-main.cpp`
  - `r36sx_cpu.c`
  - `r36sx_ports.c`
  - `disks-win32.c.inl`
- Rewrote `build_pico_286.ps1` so it no longer reads upstream files and applies
  inline string patches during build.
- `build_pico_286.ps1` now compiles the upstream source tree plus the explicit
  `r36sx_port` files; `obj/` is again only a compiler output directory.

Build command from repository root:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Verification:

```text
Build succeeded.
Output: homebrew\pico_286\pico_286
Size: 7936004 bytes
SHA256: CB9551333236CBEDEC8A173E212459D95B055A6FABAB6BC9DEC74BC1EF3A59DA
Defender scan homebrew\pico_286\pico_286: found no threats
Defender scan disk_image\MIPS_NATIVE\pico_286\pico_286: found no threats
Defender scan patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286:
found no threats
```

The warnings are the same kind of upstream/port warnings as previous builds:
unused variables in `fpu.c`, unknown `#pragma GCC optimize`, pointer-sign
warnings in `network-redirector.c.inl`, and audio inline linkage warnings.

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

## 2026-05-28 disk activity LED

Added a visible disk activity indicator for hardware testing:

- `r36sx_minifb.c` exports `r36sx_pico286_disk_activity()`.
- The build now writes a patched `obj/disks-win32.c.inl` next to
  `obj/r36sx_cpu.c`, so the `#include "disks-win32.c.inl"` in generated CPU
  code resolves to the local copy.
- The patched disk backend calls `r36sx_pico286_disk_activity()` for sector
  reads and writes.
- `mfb_update()` draws a blinking red circle in the lower-right corner for
  about 350 ms after disk activity.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,897,020 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 BIOS video mode aliases

Device test with the FreeDOS disk images showed text in the debug area:

```text
unsupported video mode 30
unsupported video mode 20
```

The renderer prints video mode numbers as hexadecimal, so these are BIOS modes
`0x30` and `0x20`.  The local build patch now aliases both modes to the normal
80x25 text renderer (`0x03`), which draws B800-style text memory into the
640x480 framebuffer.  This should remove the unsupported-mode spam and make the
DOS boot text visible while keeping the existing debug area below the main
400-line text output.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,896,908 bytes
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

## 2026-05-28 screen text diagnostics

Added debug logging for text written into Pico-286's emulated screen area.
The logger runs from the renderer and emits rows only when the visible text
contents change, with a per-view suppression limit to avoid filling the SD card.

The log now includes these diagnostic views:

- `screen_text:logical-b800`: logical B800-style `VIDEORAM` cells.
- `screen_text:renderer-byte-view`: the byte view used by the current renderer.
- `screen_text:logical-small-text`: logical cells for small text modes
  `0x77`/`0x78`.
- `screen_text:renderer-byte-small-text`: renderer byte view for small text
  modes `0x77`/`0x78`.

This should show whether BIOS messages such as the boot prompt are present in
logical video memory even when the current renderer does not display them.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,904,292 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 BIOS INT 10h text services

Added missing text-mode BIOS services to the patched CPU source so BIOS and DOS
boot screens can print through standard `INT 10h` calls instead of relying only
on direct video-memory writes.

Implemented services:

- `AH=01h`: set cursor shape.
- `AH=02h`: set cursor position.
- `AH=03h`: read cursor position and shape.
- `AH=06h` / `AH=07h`: scroll or clear text window.
- `AH=08h`: read character and attribute at cursor.
- `AH=09h` / `AH=0Ah`: write character with/without explicit attribute.
- `AH=0Eh`: teletype output with CR/LF/backspace/tab handling.
- `AH=0Fh`: read video mode, columns, and active page.
- `AH=13h`: write string from `ES:BP`.
- normal `AH=05h`: active page selection.

Also corrected the text-mode renderer so 40/80-column modes read characters
and attributes from logical `VIDEORAM` cells, matching upstream Win32 renderer
semantics.  The previous byte-pointer cast could read from the wrong layout and
hide text that was present in emulated video memory.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,917,604 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 disable on-screen debug overlay

Disabled the renderer path that drew `DEBUG_VRAM` into the bottom 80 pixels of
the 640x480 framebuffer.  The debug messages are still collected in
`pico_286.log`; only the on-screen overlay is removed.  The bottom area is now
filled with black when the emulated mode itself uses only the top 400 lines.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,916,620 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 disable screen text log dumps

Disabled the `screen_text:*` diagnostics that copied changed text rows from the
emulated screen area into `pico_286.log`.  General runtime logging remains
enabled, but the emulator no longer mirrors BIOS/DOS screen contents into the
log file.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,909,160 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-28 DOS boot confirmed

Device testing confirmed that DOS boots successfully from the included FreeDOS
floppy image.  This confirms the current Pico-286 R36SX port has a working
startup path through:

- `driver.so` video/input/audio host integration;
- initialized `read86`/`write86` memory backend pointers before `reset86()`;
- local `fdd0.img`/`fdd1.img`/`hdd.img` disk image paths;
- BIOS video mode aliases for `0x20` and `0x30`;
- added `INT 10h` text services for BIOS/DOS text output;
- corrected logical `VIDEORAM` text renderer layout.

The on-screen debug overlay and `screen_text:*` log dumps remained disabled in
the confirmed booting build.

## 2026-05-28 joystick on-screen keyboard

Added a joystick-controlled on-screen keyboard to the R36SX MiniFB backend.
Fn now toggles the keyboard instead of sending F3 directly; F3 remains available
from the keyboard's F-key row.

Controls:

- D-pad moves the highlighted key.
- A or Start types the highlighted key.
- B or Select closes the keyboard.
- X toggles Shift.
- Y sends Backspace.
- Select+Start still exits Pico-286 back to TinyMC.

The keyboard draws over the bottom of the framebuffer after each DOS frame.  It
sends key events through the existing `HandleInput()` path, so DOS sees normal
PC keyboard scancodes.  The build script also adds missing host keycode
translations for Delete and DOS punctuation used by the keyboard: `:`, `\`,
apostrophe, comma, period, slash, minus, and equals.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,933,564 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 compact on-screen keyboard layout

Adjusted the joystick keyboard so it stays on the bottom of the screen and uses
only 96 vertical pixels, roughly 20% of the 640x480 framebuffer.  Fn still
toggles the keyboard.

When the keyboard is visible, the DOS framebuffer is vertically compressed into
the remaining 384-pixel area above it instead of being overdrawn by the
keyboard.  The compression uses a halftone-style area filter, blending the
overlapping source rows rather than dropping rows.  The disk activity LED is
also moved to the lower-right corner of the compressed DOS area while the
keyboard is open.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,935,924 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 vendor pico-286 source tree

Moved the upstream Pico-286 source snapshot into
`homebrew/pico_286/pico-286` so the port can build from sources kept next to
the R36SX wrapper code.  The copied tree excludes the upstream `.git`
directory and `.gitignore`; it is a plain source snapshot, not a nested
repository.  The old ignored clone at `internet_sources/pico-286` was removed
after the new tree built successfully.

The source snapshot contains 165 files, about 3.9 MB total.  The copied tree
also excludes upstream `tools/mapdrive.com`, because that is a generated DOS
binary; `tools/mapdrive.asm` remains as source.  The build script now resolves
`$PicoRoot` from `homebrew/pico_286/pico-286` instead of the old ignored
`internet_sources/pico-286` mirror.

Scanned the vendored source snapshot:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico-286
```

Result: Microsoft Defender reported no threats.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,935,964 bytes
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 delayed on-screen keyboard key release

Fixed the Pico-286 on-screen keyboard event timing.  The previous implementation
sent key-down and key-up back-to-back inside one input poll, which could let the
emulated keyboard controller overwrite the make code with the break code before
DOS had a chance to read it.

The on-screen keyboard now sends the make code immediately, keeps the key and
any selected modifiers held for about 80 ms, and releases them on a later poll.
The backend logs `minifb: osk key down ...` and `minifb: osk key up ...` events
to `MIPS_NATIVE/pico_286/pico_286.log` for device debugging.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,938,128 bytes
- SHA256: `51D01BF7DC78EC2C38E9428C7C42FDE3E0FDDAF42538D3984C8E488001B79E99`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 on-screen keyboard face-button remap

Updated the shared on-screen keyboard physical button bindings:

- `B` now sends Backspace immediately.
- `Y` now sends Enter immediately.
- `Select` closes the keyboard.
- The on-screen `CLOSE` key is still available.

Previously `Y` sent Backspace and `B` closed the keyboard.  Pico-286 was
rebuilt against the updated shared module and the on-screen keyboard hint text
was updated accordingly.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,949,544 bytes
- SHA256: `B5A395D7A8CEDA18537659AC27339D950BD9F1E98C48061E6893073AD87BAE79`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 reusable on-screen keyboard module

Moved the joystick-controlled on-screen keyboard out of Pico-286's MiniFB
backend into reusable shared files:

- `homebrew/common/r36sx_screen_keyboard.h`
- `homebrew/common/r36sx_screen_keyboard.c`
- `homebrew/common/README.md`

The module owns keyboard state, button navigation, layout, and RGB565 drawing.
Pico-286 now supplies a small adapter callback that maps emitted key-down/key-up
events to its existing `HandleInput()` path, where they become PC scancodes for
the keyboard-controller FIFO.

The Pico-286 build script now compiles `homebrew/common/r36sx_screen_keyboard.c`
as a normal source file.  Device-facing behavior should stay the same; the
keyboard can now be reused by other native projects without copying the old
MiniFB-local implementation.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,948,928 bytes
- SHA256: `1C1BC7F76802DB1B2974704AB0B73E13473A5A335B83280BB73DE1A4C07E85B3`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 keyboard scancode pacing

Added 1 ms pacing between scancode bytes becoming available in the emulated
keyboard controller output buffer.  Internet references agree that PS/2
device-to-host transfers use an 11-bit frame: start bit, 8 data bits,
odd-parity bit, and stop bit.  The device clock is typically 10-16.7 kHz, so
one scancode byte takes roughly 660-1100 us to transmit.

References checked:

- [The PS/2 Mouse/Keyboard Protocol](https://www-ug.eecg.toronto.edu/msl/nios_devices/datasheets/PS2%20Protocol.htm)
- [AT Keyboard/Mouse protocol - Hardware Book](https://www.hardwarebook.info/AT_Keyboard-Mouse_protocol)
- [How Fast Is a PS/2 Keyboard? - OS/2 Museum](https://www.os2museum.com/wp/how-fast-is-a-ps-2-keyboard/)

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,942,728 bytes
- SHA256: `7DF7757CB0A685ACF8BB5C09F9EAC292D6AD737AA0342CF67DB93F208144F9D3`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 keyboard FIFO size reduction

Reduced the emulated keyboard controller FIFO from 32 scancode bytes to 8.
Eight bytes are enough for the current input path: a normal key produces a
make/break pair, and a key with a latched modifier still fits comfortably.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,940,104 bytes
- SHA256: `4EB23B444826C36A60B734F4F66FEB58676DED6FF0FD9676FBE7D77F4EAD01B9`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 keyboard controller FIFO

Replaced the temporary delayed-release on-screen keyboard workaround with a
small FIFO behind the emulated keyboard controller ports.  `HandleInput()` now
converts key events to PC scancodes and enqueues them instead of overwriting
`port60` directly.  Reading port `0x60` pops one queued scancode, refreshes the
controller status in `port64`, and raises another IRQ1 when more queued bytes
remain.

This matches the expected make/break flow more closely: the on-screen keyboard
can send key-down and key-up events back-to-back, and the controller buffers
both scancodes for DOS/BIOS to read in order.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,940,088 bytes
- SHA256: `AB360DAE43112F5A4B72286E9BBE968C172A1F1241DBC86591ED0D6F5725681A`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
