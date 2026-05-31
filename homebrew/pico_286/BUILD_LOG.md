# pico-286 Build Log

## 2026-05-31 configurable scaling filter

Added a configurable scaling filter for cases where the DOS image is resized
by the R36SX MiniFB layer.

- `pico_286.conf` now has `[video] scaling_filter=nearest`.
- Supported values are `nearest` and `bilinear`.
- `nearest` is the default and keeps sharp pixel edges.
- `bilinear` blends neighboring source rows when the DOS image is compressed
  under large overlays such as the on-screen keyboard.
- `r36sx_pico286_save_config()` preserves the setting when rewriting config.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1337836` bytes
- `pico_286` SHA256:
  `0006C7F8D02D7F726CCEDEB8D7D2D1AD7853032B78BDFA85110A2D884C5C39E3`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 CRTC stride for tweaked VGA modes

Updated the R36SX renderer to respect VGA CRTC row layout for graphics modes.

- CRTC start-address writes (`0Ch`/`0Dh`) now preserve the other byte instead
  of partially resetting `vram_offset`.
- The renderer now uses CRTC Offset Register `13h` for planar and chunky VGA
  row stride.
- Mode `0Dh` keeps the normal 40-byte visible stride, but handles tweaked
  layouts such as Supaplex/SPFIX `3Dh` as 122-byte rows.
- Modes `0Eh`, `0Fh`, `10h`, `11h`, `12h`, and `13h` also use the CRTC offset
  instead of hard-coded 80/320-byte rows.

Line compare and pixel-panning behavior are still deferred.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1330908` bytes
- `pico_286` SHA256:
  `7A56639A52C887A0CF831BB97976B4C7418FD3978174DC50400CE1FF7570B9E2`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 DOSBox 386 CPU comparison fixes

Compared the local 386-related CPU code against DOSBox 0.74-3 and adopted a
small set of compatible semantics instead of importing the whole CPU core:

- `0F 00` group 6 (`SLDT`, `STR`, `LLDT`, `LTR`, `VERR`, `VERW`) now rejects
  real-mode execution, matching DOSBox's protected-mode-only behavior.
- `0F 06 CLTS` is implemented for 286+ CPU modes and clears `CR0.TS`.
- `LGDT`/`LIDT` now load a 24-bit base without `66h` and a full 32-bit base
  with `66h`, matching DOSBox's `prefix_0f` vs `prefix_66_0f` handlers.
- `MOV Rd,CR0` now reports 386-style reserved bits 4..30 set.
- CR3 writes/reads now keep only the page-directory-aligned upper bits.

Detailed notes: `CPU_386_DOSBOX_COMPARISON.md`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1332700` bytes
- `pico_286` SHA256:
  `46FB1C062F4400D0CC54CAA8B2586CDD8CE38667C8290436FC680AEAE29C3C10`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 raise minimum adaptive exec86 quantum

Raised `R36SX_EXEC86_MIN_LOOPS` from `100` to `1000` instructions.
This keeps the adaptive controller from shrinking a frame's emulated CPU work
into very tiny chunks under heavy host-side load.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1332160` bytes
- `pico_286` SHA256:
  `B5E9101584E255B90BA51C61EC094F8D4AB1CF680896292366BC0F2717B60681`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 keep overlay cost out of adaptive exec86 quantum

Changed adaptive quantum timing to measure only the `exec86()` call.

- The previous implementation measured the whole active host frame, including
  `mfb_update()` overlay composition.
- Opening the on-screen keyboard made the frame more expensive and caused the
  adaptive controller to reduce emulated x86 instructions even though the CPU
  core itself was not slower.
- The controller now compares `exec86()` elapsed time against the configured
  `target_fps` frame budget.  Keyboard, menu, screenshot, and other overlay
  rendering costs no longer reduce the CPU quantum.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1332176` bytes
- `pico_286` SHA256:
  `A4B62760CA64B10D8171D39A91EB64F8E97735731FD87B5CA8D2319941D89194`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 adaptive exec86 quantum and target FPS

Added adaptive main-loop CPU quantum control for the R36SX native build.

- `pico_286.conf` now has a `[timing]` section with `target_fps=60`.
- The main-loop frame budget is derived from `target_fps`, so the old 16.6 ms
  frame time is no longer a hard-coded magic value.
- The initial maximum quantum is still derived from `cpu_mhz`.
- If an active frame exceeds the target budget, the loop estimates the quantum
  that would fit and steps downward by at most one quarter of the current
  value.
- If an active frame has spare time, the loop grows the quantum by one quarter
  back toward the `cpu_mhz`-derived limit.
- The quantum is clamped to at least 100 `exec86()` instructions per frame.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.  The compiler emitted one pre-existing upstream
warning from `#pragma GCC optimize("Ofast")` in `xms.c`.

Result:

- `pico_286` size: `1332016` bytes
- `pico_286` SHA256:
  `4D8F4D8C89C6E5BDA4831D375B2449A4C246FF0702CB3084C994F960F6F71604`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 disk image directory migration

Moved Pico-286 disk images into an `images/` subdirectory so they no longer
mix with the executable, config files, ROMs, and logs.

- Default disk config paths now use `images/FreeDOS1.img`,
  `images/sopwith.img`, `images/hdd.img`, and `images/hdd2.img`.
- The disk menu scans `images/` first, while still listing legacy root-level
  `.img` files for compatibility.
- Legacy relative config values without a directory separator, such as
  `FreeDOS1.img`, resolve to `images/FreeDOS1.img` if the root file is absent.
- `rebuild_cpu_tests_disk.ps1` now writes
  `homebrew/pico_286/images/cpu_tests.img`.

Rebuild commands:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1325040` bytes
- `pico_286` SHA256:
  `23C2463906890A1DC830F0528E66E6C14AF29DBA86EB4384C9B1D4D32D7D0D7F`
- `test386.bin` size: `65536` bytes
- `test386.bin` SHA256:
  `7E91F03B910FE52508D28ADD2AC4CF4F73B3D23F5DB7B77A5315D6F0DD234497`
- `images/cpu_tests.img` size: `1474560` bytes
- `images/cpu_tests.img` SHA256:
  `C4A8CC49F87BD3BBFEDC77061B18065603BB5297E0D97BF13AA18B06F9A1C483`

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\images\cpu_tests.img
```

Microsoft Defender reported no threats for both files.  The `disk_image` and
patch copies are byte-identical by SHA256.

## 2026-05-31 BIOS mode menu switch

Added a configurable BIOS ROM provider for the native R36SX Pico-286 build.

- `pico_286.conf` now contains a `[bios]` section.
- `bios=normal` uses the embedded Turbo XT compatible BIOS.
- `bios=test386` loads the external 64 KB test BIOS from
  `test_bios_rom=test386.bin`.
- `test386.bin` is resolved relative to `pico_286.conf`.
- The disk image menu now has a `BIOS  NORMAL/TEST386` row.  Saving after a
  BIOS change requests a soft reset so the selected ROM starts immediately.
- In `test386` mode, memory reads from `F0000h-FFFFFh` come from the external
  ROM, and writes to that range are ignored like ROM.

The external ROM file is copied next to the native executable:

- `homebrew/pico_286/test386.bin`
- `disk_image/MIPS_NATIVE/pico_286/test386.bin`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/test386.bin`

Rebuild commands:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.  The compiler emitted only pre-existing upstream
warnings from `#pragma GCC optimize("Ofast")` and the EMS inline helper.

Result:

- `pico_286` size: `1322776` bytes
- `pico_286` SHA256:
  `18BDBBEF191F7A463EACF756B93F6F3A92D6EA5FEA7E2E31836159CE845A4016`
- `test386.bin` size: `65536` bytes
- `test386.bin` SHA256:
  `7E91F03B910FE52508D28ADD2AC4CF4F73B3D23F5DB7B77A5315D6F0DD234497`
- `cpu_tests.img` size: `1474560` bytes
- `cpu_tests.img` SHA256:
  `6FF84B315AF6235934DE90CB775C517CDB2909BF2C9DFB567625D86BFCBB2A06`

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\test386.bin
.\tools\scan-download.ps1 .\homebrew\pico_286\images\cpu_tests.img
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.  The disk-image
and patch copies are byte-identical by SHA256.

## 2026-05-31 test386.asm debug ROM payload

Downloaded `barotto/test386.asm` into `homebrew/pico_286/tests/test386.asm`
and kept it as a local source copy.  The downloaded source tree was scanned
with Microsoft Defender through `tools/scan-download.ps1`; no threats were
reported.

The R36SX build changes `src/configuration.asm` to use Pico-286 debug ports:

- `POST_PORT equ 0x190`
- `OUT_PORT equ 0x191`
- `DEBUG equ 1`

`r36sx_ports.c` now logs writes to those ports as `test386:` lines in
`pico_286.log`.  Port `190h` logs POST byte values, and port `191h` collects
ASCII text until newline.

Downloaded official NASM 3.01 for Windows x86-64 from:

```text
https://www.nasm.us/pub/nasm/releasebuilds/3.01/win64/nasm-3.01-win64.zip
```

The ZIP archive, extracted NASM directory, and `nasm.exe` were scanned with
Microsoft Defender; no threats were reported.

Build command:

```powershell
.\homebrew\pico_286\tests\build_test386_r36sx.ps1
```

Equivalent NASM command:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\tests\test386.asm\src\ -f bin .\homebrew\pico_286\tests\test386.asm\src\test386.asm -w-all -l .\homebrew\pico_286\tests\test386.asm\build\test386.lst -o .\homebrew\pico_286\tests\test386.asm\build\test386.bin
```

ROM payload:

- `homebrew/pico_286/tests/test386.asm/build/test386.bin`
- Size: `65536` bytes
- SHA256: `7E91F03B910FE52508D28ADD2AC4CF4F73B3D23F5DB7B77A5315D6F0DD234497`

`test386.bin` was scanned with Microsoft Defender; no threats were
reported.

Rebuilt the CPU test floppy:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
```

The resulting `cpu_tests.img` contains:

- `ID.COM`
- `TEST386.COM`
- `TEST386.BIN`
- `CPUID.ASM`
- `ID.ASM`
- `README.TXT`

`TEST386.BIN` is the R36SX `test386.asm` build.  It is a 64 KB BIOS replacement
ROM, not a DOS `.COM` program, so the floppy stores it as a payload/reference;
executing it requires an emulator path that maps it at physical `F0000h` and
resets into its reset vector.

CPU test floppy:

- `homebrew/pico_286/cpu_tests.img`
- Size: `1474560` bytes
- SHA256: `6FF84B315AF6235934DE90CB775C517CDB2909BF2C9DFB567625D86BFCBB2A06`

The rebuilt floppy image was scanned with Microsoft Defender; no threats were
reported.  The same image was copied to:

- `disk_image/MIPS_NATIVE/pico_286/cpu_tests.img`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/cpu_tests.img`

Rebuilt Pico-286 so the debug ports are active:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1308684` bytes
- SHA256: `8C141C077990A7A8A6CD7A6BE6A3F96729280DE3ED2B2DF2138C6AFF36056FA0`

The homebrew and disk-image binary copies were scanned with Microsoft Defender;
no threats were reported.  The patch binary is byte-identical to those copies
by SHA256.

## 2026-05-31 VGA DAC palette handling

Fixed VGA DAC palette programming for mode 13h/VGA software that writes the
palette directly through ports `3C7h`, `3C8h`, and `3C9h`:

- `3C8h` write now resets the DAC write RGB component counter.
- `3C7h` write now resets the DAC read RGB component counter and switches the
  DAC state to read mode.
- `3C9h` write now masks incoming DAC components to 6 bits and expands them to
  RGB888 with `(value << 2) | (value >> 4)`.
- `3C9h` read now returns red, green, and blue sequentially and auto-increments
  the read palette index after blue.
- BIOS `INT 10h AX=1010h/1012h/1015h/1017h` uses the same DAC helpers.
  `AX=1012h` and `AX=1017h` now iterate exactly `CX` palette entries, so a
  full 256-color block starting at index 0 works correctly.

Reference behavior: standard VGA mode 13h palette programming writes the DAC
start index to `3C8h`, then writes 6-bit red/green/blue components to `3C9h`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The compiler still reports pre-existing warnings in upstream FPU/audio/pragma
code.  No new warnings came from the VGA DAC changes.  `zig objcopy
--strip-all` still returned `error: unimplemented`, so the unstripped binary
was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1308072` bytes
- SHA256: `61DE9B773E8814C44D1EDDF626C73B8B29D4BFA794E1EBBD883BAF9E8747B9CB`

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 compiler warning cleanup

Cleaned up the remaining Pico-286 compile warnings seen in the native R36SX
build:

- `network-redirector.c.inl`: `to_dos_name()` now accepts a raw DOS 8.3 output
  buffer and writes it as bytes, avoiding `char *` vs `unsigned char *`
  pointer-sign warnings for DTA `fname[11]`.
- `r36sx_cpu.c`: VGA DAC block palette loading now reads red, green, and blue
  bytes into temporaries before calling the `rgb()` macro, avoiding multiple
  unsequenced `memloc++` side effects inside one macro invocation.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The C/C++ compiler emitted no warnings.  `zig objcopy --strip-all` still
returned `error: unimplemented`, so the unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1128984` bytes
- SHA256: `F14EC8268BD4B4CFF86DB65BCB16FE70C0675353EF658FC49DC1EA6A285B9D10`

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 CHKDSK invalid-opcode investigation

The FreeDOS `CHKDSK.EXE` screenshot showed an `Invalid Opcode` trap while
checking drive C:.  The executable is UPX-packed, so the bytes shown by the DOS
exception handler are from the unpacked in-memory image rather than a direct
file offset in `CHKDSK.EXE`.

Added two CPU-core updates:

- debug builds now log the exact faulting `CS:IP`, the next 8 opcode bytes, key
  registers, and flags before raising `INT 6`;
- implemented the 80386 bit-test instruction family `BT`, `BTS`, `BTR`, and
  `BTC` for register and memory operands, including the `0F BA /4..7`
  immediate-bit forms.  These instructions are common in bitmap/FAT scanning
  code and were still missing from the 386 real-mode path.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1128748` bytes
- SHA256: `F8D3502B5D27E1ED9DA73EE624E20933C4E38AB7732DE5B962BF45ED39E2FCC9`

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 60 Hz main-loop frame pacing

Changed the native R36SX main loop to run on a fixed 60 Hz cadence.  Each loop
now targets a `16666 us` frame period and yields the remaining time to Linux
with `usleep()`.

The configured `cpu_mhz` value still maps to an execution quantum per
millisecond.  To avoid slowing the emulated CPU by the new 16 ms pacing, the
main loop scales it to a per-frame quantum before calling `exec86()`:

```c
cpu_exec_loops_per_frame =
    cpu_exec_loops_per_ms * R36SX_MAIN_LOOP_FRAME_US / 1000;
```

If a frame overruns, the loop skips the normal frame sleep.  If the emulated
CPU is also halted in that overrun state, it still performs the short HLT idle
sleep.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1126056` bytes
- SHA256: `B4AC18F1E4C0D75B16A5FD2BA8EFEDBC3034DDA9E75A9C4F190C9CA1BD8083C7`

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 sleep only while emulated CPU is halted

Changed the R36SX main loop so it no longer needs a fixed sleep/yield in every
iteration.  The emulated `HLT` instruction already sets `hltstate` and makes
`exec86()` return.  The CPU core now exposes:

```c
uint8_t r36sx_cpu_waiting_for_interrupt(void);
```

The function reports true only when the emulated CPU is halted and there is no
unmasked pending IRQ ready to wake it.  The Linux main loop checks it at the end
of each iteration and calls:

```c
usleep(R36SX_HLT_SLEEP_US); /* currently 1000 us */
```

only in that state.  Active DOS code still runs without a fixed per-loop sleep.
Timer IRQs from `ticks_thread()` wake the halted CPU on the next `exec86()`
call, matching the normal PC `HLT`/interrupt flow closely enough for this
single-process emulator.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1124696` bytes
- SHA256: `2F59AB363B2DAA1B28B07F86BC4415D3B5745B5913D090344714E69D084BDEC3`

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 80386 real-mode instruction coverage

Reviewed the current 386 path and filled in the practical real-mode gaps that
were still falling back to 16-bit execution or `INT 6`.

Implemented:

- 32-bit operand-size override forms for common `MOV`, ALU, immediate ALU,
  register `INC`/`DEC`, stack `PUSH`/`POP`, shifts, `TEST`, `XCHG`, `LEA`,
  `CBW/CWDE`, `CWD/CDQ`, `MOVSD`, `STOSD`, `LODSD`, `SCASD`, near
  `CALL`/`JMP`/`RET`, `MUL`/`IMUL`/`DIV`/`IDIV`, and group `FF` operations;
- 386 `0F xx` real-mode user instructions: near `Jcc`, `SETcc`, `PUSH`/`POP`
  `FS`/`GS`, `IMUL Gv,Ev`, `MOVZX`, `MOVSX`, `BSF`, and `BSR`;
- correct 32-bit carry/overflow flag handling for ADD/ADC/SUB/SBB/logical
  operations.

Still intentionally out of scope: full 386 protected-mode execution and
system-control `0F xx` instructions (`LGDT`, `LIDT`, `SMSW`, `LMSW`, `MOV CRx`,
task gates, privilege checks, descriptor cache, paging).

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- Size: `1124260` bytes
- SHA256: `C3D87363CBC3B75902E618C6D26857E76240D56C0F81D4DBFB39AC37F0567451`

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Microsoft Defender reported no threats for all three files.

## 2026-05-30 configurable DOS host drive H:

Implemented the R36SX side of Pico-286's upstream host filesystem passthrough.
The bundled network redirector still uses the DOS `INT 2Fh/11h` interface, but
its host base directory is now read from `pico_286.conf` instead of the
upstream Linux default `/tmp`.

New config section:

```ini
[host_drive]
host_drive_path=host
```

Relative paths are resolved next to `pico_286.conf`, so the default maps DOS
drive `H:` to `MIPS_NATIVE/pico_286/host` on the SD card.  The directory is
created on startup when possible.  DOS still has to run upstream
`MAPDRIVE.COM` after boot to mark `H:` as a network drive, and `CONFIG.SYS`
must set `LASTDRIVE=H` or higher.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,091,492 bytes
- SHA256: `43CBA5D04F6F62037B922EADA6E56D93B3724C2C4C406785A925B395D3941A11`
- Defender scan: found no threats in all three updated copies
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 PCjs CPU test floppy

Downloaded the PCjs CPU test sources with a sparse Git checkout:

```powershell
git clone --filter=blob:none --sparse https://github.com/jeffpar/pcjs.git internet_sources\pcjs_cpu_tests
git -C internet_sources\pcjs_cpu_tests sparse-checkout set software/pcx86/test/cpu
```

The ready DOS programs are in `software/pcx86/test/cpu/bin`:

- `ID.COM`
- `TEST386.COM`

Created `homebrew/pico_286/cpu_tests.img`, a 1.44 MB FAT12 floppy image, with:

- `ID.COM`
- `TEST386.COM`
- `CPUID.ASM`
- `ID.ASM`
- `README.TXT`

Build command:

```powershell
python tools\create_fat12_floppy.py --output homebrew\pico_286\cpu_tests.img --label CPUTESTS --file ID.COM=internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\bin\id.com --file TEST386.COM=internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\bin\test386.com --file CPUID.ASM=internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\cpuid.asm --file ID.ASM=internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\id.asm --file README.TXT=homebrew\pico_286\dos_files\cpu_tests_readme.txt
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\bin\id.com
.\tools\scan-download.ps1 .\internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu\bin\test386.com
.\tools\scan-download.ps1 .\homebrew\pico_286\cpu_tests.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\cpu_tests.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\cpu_tests.img
```

Result:

- Output: `homebrew/pico_286/cpu_tests.img`
- Size: 1,474,560 bytes
- SHA256: `8005293284DC5486E1268C495356B319EEBCE4F3ECB3CD76FE09CCC5DEDCF76D`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/cpu_tests.img`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/cpu_tests.img`

## 2026-05-30 experimental 386 protected mode core

Added the first usable protected-mode layer to the R36SX Pico-286 CPU core.
The guest still boots in real mode like a real PC, but it can now enter
protected mode through `LMSW` or `MOV CR0`, load `GDTR`/`IDTR`, and reload
segment descriptors through protected far control transfers.

Implemented pieces:

- `CR0`, `CR2`, `CR3`, `GDTR`, `IDTR`, `LDTR` selector, and `TR` selector
  state;
- hidden segment descriptor caches for `CS`, `DS`, `ES`, `SS`, `FS`, and `GS`;
- GDT descriptor decoding with base, limit, access byte, and granularity bits;
- protected-mode segment-base address calculation for normal memory operands;
- system opcode handlers for `0F 00`, `0F 01`, `0F 20`, and `0F 22`;
- descriptor-cache reloads for `MOV Sreg`, `POP Sreg`, `LES`, `LDS`, far
  `CALL`, far `JMP`, far `RET`, and `IRET`;
- basic protected interrupt/trap gate delivery through `IDTR`.

Known limitations remain: paging, privilege checks, call gates, task switching,
TSS stack switching, and full 32-bit `EIP` execution are not complete.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.  The remaining compiler warnings are the known upstream
FPU/pragma/audio-inline warnings; this change did not add warnings in
`r36sx_cpu.c`.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,304,904 bytes
- SHA256: `3FCAADC76A38CE59A854A75E0AF2D6664C04FA20CE0DA94915801CEBAFB5564A`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 default 80386 compatibility mode

The R36SX Pico-286 port now defaults to `cpu_model=80386` when no config file
or `cpu_model` key is present.  The shipped configs in `homebrew`, `disk_image`,
and `patches/disk_image_patch_pico_286` also set:

```ini
[cpu]
cpu_model=80386
cpu_mode=real
```

The config parser still accepts `8086`, `80286`, and `80386`.  The CPU decoder
uses that model to reject higher-generation instructions with `INT 6`: 80186+
opcodes remain blocked in `8086`, and 386-only prefixes/segment forms remain
blocked unless `80386` is selected.  The 386 real-mode path now also handles
operand-size overridden `PUSHFD`/`POPFD` and `PUSHAD`/`POPAD`, which are common
in DOS CPU detection code.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,088,212 bytes
- SHA256: `D95AE71E8C14DE73AFA18D2FD0C684FFCB374916E73EC3B93F911C22F1E04F49`
- Defender scan: found no threats in all three updated copies
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 VGA register and memory model fixes

Applied the first pass of VGA model fixes from the register/memory audit.

Source changes were intentionally split into separate commits:

- `Route VGA register reads`: routed sequencer/graphics/attribute/DAC reads
  through `vga_portin()` and reset the Attribute Controller flip-flop when
  reading `0x3DA`.
- `Track VGA attribute registers`: stored Attribute Controller registers,
  used a 5-bit Attribute Controller index, and added minimal DAC mask and
  Misc Output register readback.
- `Fix VGA write mode 3 masking`: made write mode 3 use
  `(rotated_cpu_byte & bit_mask)` as the selector and Set/Reset as the source.
- `Respect VGA memory map selection`: honored Graphics Controller register 6
  memory map selection for `A000`, `B000`, and `B800` VGA windows.
- `Fix VGA chain4 mode 13 rendering`: corrected Sequencer Memory Mode chain-4
  bit handling and made unchained Mode 13h read the same packed plane-byte
  layout that the write path stores.
- `Apply VGA start address in graphics renderer`: made EGA/VGA graphics
  renderers apply CRTC Start Address (`vram_offset`) and wrap indexes inside
  the 64 KiB logical VRAM window.

The remaining larger VGA items are still intentionally deferred: full CRTC
Offset/stride timing semantics and VGA text font plane 2 rendering need a
separate, narrower test case because they can change many text and graphics
modes at once.

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
- Size: 1,085,216 bytes
- SHA256: `4DF6DF0894F39D2310F66E3819DACB2DC6C9C88004B6C1AAA4A69D1757022CE2`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 3DBENCH VGA Mode 13h rendering fix

Investigated `C:\Temp\3DBENCH.EXE` with Ghidra 12.0.4.  The DOS EXE is
self-relocating, so the default analysis only found the startup stub.  Added
`ghidra_scripts/DumpDisasmRanges.java` and dumped the relevant video ranges.

Findings:

- `3DBENCH.EXE` enters graphics with `MOV AX,0013h` / `INT 10h`, so it uses
  standard VGA Mode 13h, 320x200x256.
- It loads palette blocks with `MOV AX,1012h` / `INT 10h`.
- It clears and draws the screen with direct writes to `A000:0000`.
- No direct VGA sequencer/graphics-controller port programming was found in
  the inspected video setup path.

Fix:

- `vga_mem_read()` / `vga_mem_write()` / 16-bit variants now treat non-planar
  Mode 13h as linear 8-bit video memory: one emulated byte per
  `VIDEORAM[offset]` cell.
- The R36SX renderer no longer reads non-planar Mode 13h by casting
  `VIDEORAM` to `uint8_t *`; it reads the low byte from the logical
  `VIDEORAM[]` cell instead.

Ghidra commands:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects 3dbench_dos -import C:\Temp\3DBENCH.EXE -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\3DBENCH.EXE -deleteProject
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects 3dbench_disasm_ranges -import C:\Temp\3DBENCH.EXE -scriptPath .\ghidra_scripts -postScript DumpDisasmRanges.java .\ghidra_exports\3DBENCH.EXE\video_ranges.s 1000:3d8f 0x80 1000:3f50 0xd0 1000:40c8 0x40 1000:44c0 0x30 1000:69d8 0x30 -deleteProject
```

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
- Size: 1,079,928 bytes
- SHA256: `DDD866F5E2550E4B9A046F44E1C3247257DDFF4B47C1BDD9E54644B70ED199C3`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 memory bounds hardening

Audited emulator memory access paths and fixed host-array boundary hazards:

- RAM/UMB/HMA/BIOS fast word and dword paths now use direct casts only when
  the whole access fits in the target region; boundary-crossing accesses fall
  back to byte reads/writes.
- A20-disabled high addresses wrap with a 20-bit mask instead of recursive
  subtract-and-retry.
- EMS page selectors are constrained to the configured EMS size, and EMS
  word/dword accesses are assembled from bounded byte operations.
- VGA planar 16-bit reads/writes wrap `0xffff + 1` inside the 64 KiB VGA
  window instead of taking `VIDEORAM[0x10000]`.
- Disk insertion/ejection rejects unsupported drive indices before indexing
  `disk[4]`.
- DOS network redirector SDA/SFT/DTA pointers are range-checked before direct
  `RAM[]` access.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,079,216 bytes
- SHA256: `89390CE019A88F8366E727D9FD18A151A4F182245BCA6D88195CB431B7ABA263`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 on-screen keyboard symbol layout

Added a second on-screen keyboard layout for DOS symbols and punctuation.

- `L` or `R` toggles between the normal keyboard and the symbol keyboard while
  the on-screen keyboard is visible.
- The symbol layout includes shifted DOS/US-keyboard characters such as `!`,
  `@`, `#`, `$`, `%`, `^`, `&`, `*`, `(`, `)`, `_`, `<`, `>`, `?`, `|`, and
  `~`.
- Added missing keycode/scancode support for the backtick/tilde key and the
  bracket keys (`[`, `]`, `{`, `}`).
- The key preset picker can also switch layouts with `L` or `R`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,062,020 bytes
- SHA256: `B85E15CF6EABF28C44E92F0563443627BBB92407B21B25DF4E37C35C5CCCFF80`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 emulator key auto-repeat

Added typematic-style auto-repeat for keys that are sent into the emulated PC:

- physical R36SX buttons mapped by the active key preset now send one PC make
  scancode on press, repeated make scancodes while held, and one break scancode
  on release;
- Shift, Ctrl, and Alt are not repeated;
- the on-screen keyboard now repeats emitted keys while holding A, Start, B, X,
  or Y, so Backspace/Escape/Enter and the highlighted virtual key can repeat;
- the existing on-screen keyboard D-pad navigation repeat is unchanged.

The repeat timing is currently fixed in code: 420 ms initial delay, then one
repeat about every 70 ms.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,058,080 bytes
- SHA256: `3C59964570128A81702E1645508CDC4694573ADCFE7F068A74AEB60B84525222`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 configurable 386 CPU model mode

Added a configurable CPU identity layer for the R36SX Pico-286 port:

- `cpu_model=8086`, `80286`, or `80386` can now be parsed from the `[cpu]`
  config section;
- `cpu_mode=real` or `protected` can now be parsed and logged;
- the build enables the existing `CPU_386_EXTENDED_OPS` code path;
- `FS`/`GS`, `66h`, and `67h` are now accepted only when `cpu_model=80386`;
- 80186+ opcodes are rejected with `INT 6` when `cpu_model=8086`;
- 386 address-size override handling was corrected to use real 32-bit ModR/M
  and SIB addressing;
- 32-bit memory operands now read/write full dwords instead of truncating
  memory reads to 16 bits.

Important limitation: full 386 protected-mode execution is not implemented
yet.  `cpu_mode=protected` is accepted so configs can express the requested
mode, but startup logs that the protected-mode CPU core is still WIP and boots
the normal real-mode path.  A real protected-mode implementation still needs
descriptor-table state, segment descriptor cache, CR0/CR3 handling, protection
faults, privilege checks, interrupt/task gates, and the 386 `0F xx` system
instruction subset.

Example config:

```ini
[cpu]
cpu_model=80386
cpu_mode=real
cpu_mhz=32.768
```

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,051,696 bytes
- SHA256: `D6CAC3199D3DD75CB883798743524786893D8C0D2531BB860BDA7A0CCBC470B0`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 80286 real-mode correctness fixes

Fixed several obvious 80286 real-mode CPU emulation mismatches found during
the ISA audit:

- enabled 80286+ shift/rotate count masking to the low 5 bits;
- made default invalid opcodes raise `INT 6` at the faulting instruction;
- rejected 386-only `FS`/`GS` segment prefixes and segment-register encodings;
- fixed `INSB`/`INSW` to update only `DI`, and `OUTSB`/`OUTSW` to update only
  `SI`;
- made opcode `D6h` a no-op compatibility hole instead of falling through into
  `XLAT`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,026,204 bytes
- SHA256: `786758A39DEC9DCF4D7D0EE86152640C3596C4C553FAF8F6FE0ECBEBEEBF24D3`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 realistic CGA status port timing

Moved CGA status port `3DAh` emulation out of the renderer path.  `cga_portin`
now calculates a simple CGA raster phase from a monotonic timer and reports:

- bit 0 during horizontal/vertical blanking, when VRAM access is safe;
- bit 3 during the vertical retrace window.

The renderer no longer writes `port3DA` while drawing rows, so programs that
poll `3DAh` for CGA snow avoidance are no longer limited by host frame
presentation.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,020,252 bytes
- SHA256: `B4C8B3C4AF8B2A893249ED1F8D6AC053141AE41AE28AEF5C8D355955B375536C`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 opcode 0F invalid-opcode handling

CPUID is intentionally not implemented for the emulated 286-class CPU.  The
bug was that opcode `0F` was routed to a no-op default path, so a CPU probe
executing `0F A2` did not get the expected invalid-opcode exception and then
mis-decoded `A2` as a standalone instruction.

The CPU core now routes opcode `0F` to a real handler in both switch and
computed-goto dispatch.  In non-8086 builds it restores `IP` to the faulting
opcode and raises `INT 6`, matching the behavior CPU detector programs expect
when probing for unsupported CPUID/386+ instructions.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,019,272 bytes
- SHA256: `04F7A40008CC059DFD9EA3FA7378F362480CF70A754CBDC883188225CEA70B39`
- Defender scan: found no threats in the updated deployed copies
- `homebrew/pico_286/pico_286` has the same SHA256 as the scanned deployed
  copies
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 configurable emulated memory sizes

Added a `[memory]` section to `pico_286.conf`:

```ini
[memory]
conventional_kb=640
upper_kb=176
extended_kb=64
xms_kb=4096
```

The values are runtime limits over the compiled-in maximum buffers.  The port
now uses them as follows:

- `conventional_kb`: written to the BIOS Data Area memory-size fields;
- `upper_kb`: limits the XMS UMB allocator from `D000:0000` upward;
- `extended_kb`: returned by `INT 15h AH=88h`;
- `xms_kb`: limits the built-in XMS handler's free/allocated memory reporting.

XMS allocations now track requested KB per handle and fail when the configured
XMS pool is exhausted.  XMS moves are also bounds-checked against the configured
pool.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,018,680 bytes
- SHA256: `14524A7A06ADB151CAD455BB42293D4165E6866075013B499E7FC935DB50446B`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 centered screenshot toast

The screenshot confirmation toast is now centered on the screen instead of
being anchored in the upper-right corner.  The preview remains in the same
toast box below the `SCREENSHOT SAVED` / `SCREENSHOT FAILED` message.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,009,420 bytes
- SHA256: `8738F3780244DAD130E7DAF70EA010AD6C10E9255FD49168CDD7D6330BB585B0`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 statistics x86 rate in K/s

The `X86` row in the on-screen statistics table now displays thousands of
decoded x86 instruction loops per second, matching the compact `K/s` style used
for disk read/write rates.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,009,336 bytes
- SHA256: `16F3DD7ED93EADB8557433A71EF576AE297F7F6C993110870BDA34FD58BE9FCF`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 lower-right statistics table

Changed the `Fn` + D-pad `Down` statistics overlay from a single top-left text
line to a compact two-column table anchored in the lower-right corner directly
above the disk activity LED.  Metric names now stay in one column and values in
the next column:

```text
X86    123K/s
READ   0K/s
WRITE  0K/s
FPS    60
```

The shared MiniFB 8x8 text helper now reads `font_8x8` with the same
least-significant-bit-left convention as the DOS text renderer, fixing the
per-character horizontal mirroring that was visible in the stats line.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,009,372 bytes
- SHA256: `ED98F6095FD5858A155A5AAA3131FB76D8FAC32D15327FB47E7CEEE38350CFDC`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 CGA raw video memory path

Sopwith still showed corrupted colored rows in the top HUD area even after the
CGA renderer itself was fixed.  The remaining issue was lower in the memory
backend: every write to the `0xA0000..0xBFFFF` video range was routed through
the VGA planar `vga_mem_write()` path.  That is correct for EGA/VGA modes
`0Dh..13h`, but CGA/text/Tandy/Hercules modes expect raw byte-addressable
video memory at `B800:0000` or adjacent ranges.

`memory.c` now routes video reads/writes through small helpers:

- EGA/VGA modes `0Dh..13h` keep the existing VGA planar path;
- CGA/text/Tandy/Hercules and other non-VGA modes store and read raw bytes from
  the low byte of the `VIDEORAM[]` cells.

This should stop CGA programs such as Sopwith from being affected by stale VGA
sequencer/graphics-controller state.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 1,007,400 bytes
- SHA256: `7E91BA0398575E9BE6B46F1C0AFC20F1F683B6FCD82FF3C113EF8C168B301709`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-30 Fn shortcut help overlay

Added `Fn` + D-pad `Left` as an on-screen help toggle.  The overlay lists the
active Fn shortcuts: keyboard toggle, help, screenshot, statistics, disk menu,
key presets, soft reset, TinyMC exit, and long-Fn emergency exit.  Opening the
on-screen keyboard, disk menu, key preset editor, or soft-resetting the VM hides
the help panel so it does not stack over full-screen controls.

## 2026-05-30 screenshot feedback

`Fn` + D-pad `Up` now gives immediate feedback when a screenshot is requested:
the R36SX audio backend mixes a short camera-shutter click into the normal
`driver.so` PCM stream, and the MiniFB compositor shows a half-second
`SCREENSHOT SAVED`/`SCREENSHOT FAILED` message with a 160x120 preview of the
captured frame.  The preview is drawn only as an overlay after the saved frame
has already been written, so the screenshot file itself does not include the
confirmation UI.

## 2026-05-30 on-screen app statistics

Added an optional on-screen runtime statistics overlay for Pico-286.  When
enabled in `pico_286.conf`, `Fn` + D-pad `Down` toggles a compact top-left
overlay with:

- decoded x86 instruction loop count per second;
- host disk image read KB/s;
- host disk image write KB/s;
- presented FPS.

The config option is:

```ini
[stats]
app_stats_enabled=1
```

Set `app_stats_enabled=0` to disable the feature and ignore `Fn` + D-pad
`Down`.  The overlay uses the normal compositing path only while visible.

## 2026-05-30 Select key preset row

Changed the key preset editor so the lower-left physical button slot shows the
actual `SELECT` assignment row instead of drawing the old `X/B CANCEL` label
over it.  The editor still has the visual `CANCEL` button at the bottom and
B/X still cancel the editor when no picker keyboard is open.

The default preset now maps physical `Select` to `Shift`:

```ini
select=SHIFT
```

## 2026-05-30 computed-goto opcode dispatch

Added an optional GNU labels-as-values dispatch table to the main `exec86()`
opcode decoder in `r36sx_cpu.c`.  The existing `switch (opcode)` body remains
as the fallback implementation, but the normal R36SX build now jumps directly
to the opcode handler label through a 256-entry table before the switch.

This removes one large switch dispatch from the hot path without rewriting the
individual opcode handlers.  Prefix opcodes that are handled before the main
decoder, and unavailable 286/386 opcodes in this build, still route to the
default handler.

Build options:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
.\homebrew\pico_286\build_pico_286.ps1 -DisableComputedGoto
```

Both commands were tested.  The first command is the optimized default build;
the second keeps the old switch-based decoder for comparison or fallback.

## 2026-05-30 grouped configuration sections

Grouped all `pico_286.conf` options under INI section headers.  The parser
continues to accept keys by name, but the default template and
`r36sx_pico286_save_config()` now write `[cpu]`, `[boot]`, `[screenshot]`,
`[disk_cache]`, `[profiling]`, `[floppy_drives]`, and `[hard_drives]`
sections so no option remains at top level.

## 2026-05-30 configurable screenshot format

Added `screenshot_format` to `pico_286.conf`.  Supported values are `png` and
`bmp`; `png` remains the default.  The screenshot shortcut keeps the same
`Fn` + D-pad `Up` binding and chooses the filename extension from the
configured format.

## 2026-05-30 PNG screenshots

Changed the `Fn` + D-pad `Up` screenshot writer from uncompressed 24-bit BMP
to compressed PNG.  The R36SX MiniFB backend now writes PNG chunks directly and
uses zlib `compress2()` for the `IDAT` stream, avoiding a libpng dependency
while still producing standard `.png` files:

```text
/mnt/sdcard/MIPS_NATIVE/pico_286/screenshots/pico_286_YYYYMMDD_HHMMSS_NNN.png
```

The build links the target `libz.so.1.2.11` explicitly because the SDK sysroot
does not provide an unversioned `libz.so` linker symlink.  The original disk
image already ships compatible `libz.so.1` libraries.

## 2026-05-30 direct-present video path

Optimized the R36SX MiniFB present path for the normal no-menu case.
`mfb_update()` now sends the emulator `SCREEN` buffer directly to
`driver.so` when the on-screen keyboard, disk menu, and key preset editor are
not visible.  This removes the extra full-frame `SCREEN` -> `base_frame` copy
from ordinary DOS/game frames.

The red disk activity LED now uses a small save/restore rectangle in direct
mode: the LED is drawn into `SCREEN` for the `video_driver_disp_frame()` call,
then the original pixels are restored immediately.  Full-screen overlays still
use the separate composition buffers so the keyboard scaling and menus do not
pollute the emulator framebuffer.

## 2026-05-30 runtime profiling option

Added an optional Pico-286 profiling helper, compiled by default and controlled
at runtime through `pico_286.conf`:

```text
[profiling]
profiling_enabled=0
profiling_log_ms=5000
```

When enabled, it writes periodic summaries to `pico_286.log` independent of
the normal `DEBUG` build flag.  It covers the main hot paths: `exec86()`,
pending disk flush checks, `mfb_update()`, keyboard ticks, timer IRQ
generation, DSS/Sound Blaster/audio sample generation, audio writes, renderer
time, and soft reset.  Each line includes calls, units, total milliseconds,
average microseconds, per-unit microseconds, and max microseconds.

The build script also accepts `-DisableProfiling`, which sets
`R36SX_ENABLE_PROFILING=0` so profiling macros compile out as no-ops.

## 2026-05-30 disk image I/O cache

Split the host disk-image operations into `r36sx_host_disk_io.c/.h`. The BIOS
INT 13h layer now keeps CHS/DMA validation in `disks-win32.c.inl`, while the
new helper owns host `FILE*` reads/writes, per-image stdio buffering, and dirty
flush policy.

The default config now enables:

```text
[disk_cache]
disk_cache_buffer_kb=64
disk_cache_flush_sectors=4
disk_cache_flush_ms=2000
```

Contiguous sector transfers into ordinary DOS RAM use bulk `fread()`/`fwrite()`
directly against `RAM[]`. Writes flush after 4 dirty sectors, after 2 seconds
without another write, on INT 13h reset, before boot-sector reads, when an image
is closed, during soft reset, and on emulator exit.

## 2026-05-30 Fn+Up screenshots

Added a native screenshot shortcut to the R36SX MiniFB backend. Holding `Fn`
and pressing D-pad `Up` now captures the currently presented 640x480 RGB565
frame and writes it as a 24-bit BMP file under:

```text
/mnt/sdcard/MIPS_NATIVE/pico_286/screenshots/
```

Screenshots are named `pico_286_YYYYMMDD_HHMMSS_NNN.bmp`. If the SD-card
absolute path is unavailable, the backend falls back to a local `screenshots`
directory. The capture is handled as an Fn chord, so it does not toggle the
on-screen keyboard or send the Up arrow to DOS.

## 2026-05-30 native RGB565 video buffer

Converted the R36SX Pico-286 video path from a 32-bit intermediate screen
buffer to native RGB565.  `r36sx_linux-main.cpp` now declares `SCREEN` as a
`uint16_t[640 * 480]`, keeps RGB565 shadow palettes for CGA/TGA/VGA renderer
output, and writes 16-bit pixels directly.  `r36sx_minifb.c` now treats the
MiniFB source pointer as RGB565, copies rows directly when no overlay scaling is
needed, and blends RGB565 pixels when the on-screen keyboard compresses the DOS
image.  This removes the per-frame RGB888-to-RGB565 framebuffer conversion from
the active Pico-286 present path.

The remaining ARGB8888-to-RGB565 match found by the scan is in the older
Fake86 SDL shim (`homebrew/fake86/r36sx_sdl.c`).  That shim emulates SDL's
ARGB8888 texture API and should be changed separately with its SDL
LockTexture/UpdateTexture contract if Fake86 is revived.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_092\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 917,496 bytes
- SHA256: `BED9F7092FD1E03A169EB34A40BD1964FC6F9ED403F5E1DDADF67F0E0E2D6EDB`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_092/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_092/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 disk menu boot order row

Added a `BOOT ORDER` row to the Pico-286 disk image menu.  It displays
`A,C` or `C,A`; Left/Right or A/Y cycles between them.  `SAVE/APPLY` writes
canonical `boot_order=fdd0,hdd0` or `boot_order=hdd0,fdd0` to
`pico_286.conf`.  Existing `boot_order=rom` is preserved if the row is not
touched; touching the row switches to a normal A/C order.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_091\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 915,372 bytes
- SHA256: `F9CBF5F875428A33E568E07110E814597D13177C69C8CBA07099D0438D58D429`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_091/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_091/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 on-screen keyboard D-pad repeat

Added D-pad auto-repeat to the shared on-screen keyboard helper.  In normal
keyboard mode and in the key-preset picker, holding Left/Right/Up/Down now
moves the highlighted key once immediately, waits about 280 ms, then repeats
movement about every 85 ms while the direction remains held.  Only navigation
repeats; A/Start, B, X, Y, and Select keep their previous one-shot behavior.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_090\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 912,504 bytes
- SHA256: `5AB34A3F4B7C9DE260E990228549C66B55629D6039D0CF1E1C567B59153E8810`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_090/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_090/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 Fn+B soft reset

Added a native Pico-286 soft-reset shortcut.  Holding `Fn` and pressing `B`
now requests a reset of the emulated PC without exiting the native app.  The
input backend releases all virtual keys and closes overlays before the reset
request is applied.  The main emulation loop then resets the CPU, RAM/video
state, keyboard queue, PIC, PIT, PC speaker/Covox, Sound Blaster, SN76489,
OPL, and the host audio buffer before continuing.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_089\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 910,824 bytes
- SHA256: `BAE91FC64A41F8D4AC86E9DCBB89220D0D99695967643CE65EAB590B36B39F21`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_089/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_089/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 REP MOVS/STOS batching

Optimized the R36SX CPU core string move/store path.  `REP MOVSB`,
`REP MOVSW`, `REP STOSB`, and `REP STOSW` now execute up to 1024 elements per
decoded REP instruction instead of re-decoding the REP prefix and opcode for
every byte or word.  The batched path still uses the existing `getmem*()` and
`putmem*()` handlers, so RAM, VRAM, and mapped memory side effects keep their
current behavior.

The batching preserves direction flag movement, 16-bit `SI`/`DI` wraparound,
and partial progress through `CX`.  If the Trap Flag path is active, batching
falls back to one element so single-step behavior stays close to the previous
implementation.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `zig objcopy --strip-all` step still reports `error: unimplemented`; the
script kept the working unstripped binary.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_088\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 908,356 bytes
- SHA256: `19C64013150F4A659BE965DCD8D12045E00CC30F7C3639E6FBDE33FD46137DF7`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_088/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 ticks thread 1 ms sleep

Increased `R36SX_TICKS_THREAD_SLEEP_US` in the R36SX Pico-286 Linux entrypoint
from `250u` to `1000u`.  The ticks thread still catches up timer, audio,
Sound Blaster, Disney Sound Source, cursor blink, and video frame events in
batches, but it now wakes the Linux scheduler about 1000 times per second
instead of up to 4000 times per second.

This should reduce scheduler overhead while keeping audio batching modest
at roughly 44 output samples per 1 ms wakeup.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `zig objcopy --strip-all` step still reports `error: unimplemented`; the
script kept the working unstripped binary.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_087\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 902,332 bytes
- SHA256: `CF3AD6C1C68D5640F9C4EE2C59C62ECF85A1DB39001EFD7E005C4D596BDE89F0`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_087/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 HLT idle wake-on-IRQ

Implemented real `HLT` idle behavior in the R36SX CPU core.  Opcode `F4` now
sets a CPU halt state and returns from `exec86()` instead of continuing to burn
the rest of the host execution quantum.  At the start of later `exec86()`
calls, a pending unmasked PIC interrupt wakes the halted CPU when `IF=1`, then
the existing `nextintr()` / `intcall86()` path services the IRQ normally.

This should reduce host CPU use in DOS prompts, installers, menus, BIOS idle
loops, and other guest code that halts until timer or keyboard interrupts.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `zig objcopy --strip-all` step still reports `error: unimplemented`; the
script kept the working unstripped binary.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_086\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 902,332 bytes
- SHA256: `3BE291DAC81BE5BA61F8FF7824C1870E8F301E9EFE07DA203F118069F8F2C88B`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_086/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 RGB565 video present cache

Optimized the R36SX MiniFB video path.  `renderer()` now marks a completed DOS
frame with `r36sx_mfb_mark_frame_ready()`.  `mfb_update()` keeps a converted
RGB565 base frame and only rebuilds it when a new renderer generation arrives
or when the keyboard layout changes.  Menus, the on-screen keyboard, and the
disk LED are drawn on a separate overlay frame copied from that base image.

When no overlay is active and no new DOS frame is ready, `mfb_update()` now
skips both the RGB888-to-RGB565 conversion and the `driver.so` frame submit.
The normal no-keyboard path also converts only the active DOS video height and
clears the remaining bottom rows as black instead of converting already-black
renderer rows one pixel at a time.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `zig objcopy --strip-all` step still reports `error: unimplemented`; the
script kept the working unstripped binary.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_085\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 899,216 bytes
- SHA256: `F4D7FF18F098A744C640FC8B63395D05C6E78453FF7E22E43901153597985BF7`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_085/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 Sopwith CGA mode 04h renderer fix

Investigated `homebrew/pico_286/sopwith.img`.  The FAT12 root contains
`SW.EXE`, `SOPWTH.BAT`, and `README.TXT`; `SOPWTH.BAT` runs `SW.EXE` without
arguments.

`SW.EXE` uses a small BIOS video wrapper at file offset `0x66ac`:
`mov al,[bp+4]; xor ah,ah; int 10h`.  Its startup path at file offsets
`0x5d94..0x5da1` compares the BSS flag at `0x6bba` with `1`, then passes
`04h` when the flag is zero or `06h` when it is nonzero.  The runtime startup
clears the BSS range that contains `0x6bba`, and the bundled BAT file provides
no video override, so normal Sopwith startup selects BIOS video mode `04h`
(CGA 320x200 4-color graphics).  The alternate path is mode `06h` (CGA
640x200 2-color graphics).

The R36SX renderer for CGA modes `04h`, `05h`, and `06h` was still treating
`VIDEORAM` as a dense `uint8_t *`.  The emulator stores each emulated video
byte in the low byte of one `uint32_t VIDEORAM[]` slot, so that pointer cast
read padding bytes from the same slot and scrambled CGA graphics.  The CGA
renderer now indexes `VIDEORAM[]` by logical video byte and reads the low byte
of each slot.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `zig objcopy --strip-all` step still reports `error: unimplemented`; the
script kept the working unstripped binary.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_084\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: `896604` bytes
- SHA256: `D1D5787DC9C83E8BBC1EC825B0AB623F0E6F5EB98801365DF8892EA4A187F874`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_084/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 EGA mode 0Fh monochrome renderer

Added renderer support for BIOS video mode `0Fh`, the standard EGA/VGA
640x350 monochrome graphics mode.  The port now draws it as a 640x350 active
image.  Because Pico-286 stores EGA/VGA memory as packed plane bytes in each
`VIDEORAM` word, the mono renderer treats a pixel as foreground if the bit is
set in any available plane; this keeps output visible for software that writes
through different EGA plane configurations.

`r36sx_pico286_video_active_height()` now reports 350 rows for mode `0Fh`, so
the on-screen keyboard compression uses the correct active DOS image height.

Reference notes:

- HelpPC lists `0Fh` as 640x350 monochrome graphics for EGA/VGA.
- PCjs / Norton Programmer's Guide likewise lists `0Fh` as 640x350 mono.

Commands run:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still reports `error: unimplemented`, so the build
keeps the unstripped executable.

Output binary:

- Path: `homebrew/pico_286/pico_286`
- Size: `896284` bytes
- SHA256: `C9280E6B8A3F8838D2C050AAE91C04D39E1784BFFBAE2DAAAC403088E7038600`
- Defender scan: found no threats

## 2026-05-29 MDA mode 07h text renderer

Fixed BIOS video mode `07h`.  It previously fell through into the Hercules
bitplane graphics renderer and did not behave like MDA-compatible 80x25
monochrome text.  The renderer now draws mode `07h` as 80 columns by 25 rows
with 8x14 cells, producing a 640x350 active image.  This intentionally drops
the ninth MDA character pixel so the 720-pixel-wide text layout fits the R36SX
640-pixel framebuffer without horizontal scaling.

The BIOS text helper now also switches its text memory base to the MDA
`B000:` region for mode `07h`; other text modes keep the CGA-compatible
`B800:` base.

Commands run:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still reports `error: unimplemented`, so the build
keeps the unstripped executable.

Output binary:

- Path: `homebrew/pico_286/pico_286`
- Size: `894948` bytes
- SHA256: `230E13863C3760CF975602A5AA774B8269F48E85F81D1E4FD8C32EA2E5A2DD19`
- Defender scan: found no threats

## 2026-05-29 clean object directory before build

Updated `build_pico_286.ps1` so every Pico-286 build starts from a clean
`homebrew/pico_286/obj` directory.  The script verifies the absolute object
directory path before removing it, then recreates it for fresh compiler output.

Commands run:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still reports `error: unimplemented`, so the build
keeps the unstripped executable.  The clean rebuild produced the same binary
content as the previous Pico-286 build:

- Path: `homebrew/pico_286/pico_286`
- Size: `891968` bytes
- SHA256: `78498E29EE66FC29570D30CABA35699EAC46FC053DA5D1D981821FAC00589A94`
- Defender scan: found no threats

## 2026-05-29 active-height keyboard compression

Changed the Pico-286 MiniFB backend so the on-screen keyboard no longer
compresses the whole 640x480 framebuffer.  While the keyboard is visible,
`mfb_update()` now asks the renderer for the active output height of the
current emulated video mode and scales only that area into the 384-pixel DOS
viewport above the keyboard.

The current renderer reports:

- 400 rows for the normal DOS/CGA/Tandy/VGA modes handled by the existing
  `y < 400` renderer path, including text modes `0x02`, `0x03`, `0x20`, and
  `0x30`.
- 350 rows for EGA `0x10`.
- 348 rows for the Hercules `0x1e` path.

This avoids wasting vertical space on the black margin below the default
80x25 text-mode area.

Commands run:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
Copy-Item -Force -Path homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286
Copy-Item -Force -Path homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
Copy-Item -Force -Path homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_081\MIPS_NATIVE\pico_286\pico_286
Copy-Item -Force -Path homebrew\pico_286\EXE_README.md -Destination disk_image\MIPS_NATIVE\pico_286\README.md
Copy-Item -Force -Path homebrew\pico_286\EXE_README.md -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\README.md
Copy-Item -Force -Path homebrew\pico_286\EXE_README.md -Destination patches\disk_image_patch_081\MIPS_NATIVE\pico_286\README.md
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_081
```

`zig objcopy --strip-all` still reports `error: unimplemented`, so the build
keeps the unstripped executable.

Output binary:

- Path: `homebrew/pico_286/pico_286`
- Size: `891968` bytes
- SHA256: `78498E29EE66FC29570D30CABA35699EAC46FC053DA5D1D981821FAC00589A94`
- Defender scan: found no threats

## 2026-05-29 on-screen keyboard direct B/X keys

Updated the shared R36SX on-screen keyboard picker behavior so physical B and X
are direct keyboard shortcuts everywhere the on-screen keyboard is active.

- B returns `Backspace`.
- X returns `Escape`.

The normal DOS on-screen keyboard already sent those keys.  The preset editor's
picker keyboard now does the same instead of treating B/X as a picker cancel.
For preset-name editing, B deletes one character and X closes the picker through
the existing Escape handling.  For key-binding rows, B assigns Backspace and X
assigns Escape.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_080
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 891,276 bytes
- SHA256: `366C47D675782BAD84EC4F353DCF733CA79BB525F716FEE6A3369CE93396A28C`
- Defender scan: no threats.

## 2026-05-29 on-screen keyboard compact edge padding

Adjusted the shared R36SX on-screen keyboard layout after the previous
edge-to-edge change introduced large centered padding around the keys.

The panel is still flush with the left, right, and bottom screen edges, but the
header/status text is restored.  The key rows now use a one-pixel inner gap from
the panel border on the left, right, and bottom edges instead of the previous
computed vertical inset.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_079
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 891,076 bytes
- SHA256: `521FD0C4E9F1D518A79CEA6F72FD1AC4C4FE21362A3646C737F68466983C51F2`
- Defender scan: no threats.

## 2026-05-29 remaining FreeDOS package floppies

Downloaded the official FreeDOS 1.4 Floppy Edition archive again:

```powershell
Invoke-WebRequest -Uri "https://download.freedos.org/1.4/FD14-FloppyEdition.zip" -OutFile ".tmp\FD14-FloppyEdition.zip"
```

The archive contains:

- `144m/x86BOOT.img`
- `144m/x86DSK01.img`
- `144m/x86DSK02.img`
- `144m/x86DSK03.img`
- `144m/x86DSK04.img`
- `144m/x86DSK05.img`
- `144m/x86DSK06.img`

The project already had `FreeDOS1.img` through `FreeDOS3.img`, so the remaining
package floppies were copied as:

- `FreeDOS4.img`: `144m/x86DSK03.img`
- `FreeDOS5.img`: `144m/x86DSK04.img`
- `FreeDOS6.img`: `144m/x86DSK05.img`
- `FreeDOS7.img`: `144m/x86DSK06.img`

Copied into:

- `disk_image/MIPS_NATIVE/pico_286/`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/`
- `patches/disk_image_patch_078/MIPS_NATIVE/pico_286/`

Scan commands:

```powershell
.\tools\scan-download.ps1 .\.tmp\FD14-FloppyEdition.zip
.\tools\scan-download.ps1 .\.tmp\FD14-FloppyEdition
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\FreeDOS4.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\FreeDOS5.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\FreeDOS6.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\FreeDOS7.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\FreeDOS4.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\FreeDOS5.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\FreeDOS6.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\FreeDOS7.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_078
```

Result: Microsoft Defender reported no threats.

## 2026-05-29 timing thread busy-loop reduction

Reviewed the Pico-286 hot loops after the emulator felt slow on the device.
The main CPU loop still intentionally calls `exec86()` once per host loop and
then lets `mfb_update()` enforce the configured `cpu_mhz` pacing.  The suspicious
busy loop was in `ticks_thread()`: it continuously polled `clock_gettime()` with
no sleep while also generating audio/timer/render ticks.

Changes:

- `ticks_thread()` now catches up missed timer/audio ticks in bounded batches.
- The thread sleeps for 250 us after each pass so the host CPU can run the
  `exec86()` thread instead of burning cycles on timer polling.
- `build_pico_286.ps1` now builds with `DEBUG=0` by default.  Pass `-DebugLog`
  to re-enable `pico_286.log` diagnostics.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_077\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 890,172 bytes
- SHA256: `3429FB3DEC4F85A11120F277180B70F84ABA39F212238F70BE94DDE6CD7AEBE5`
- Defender scan: no threats.

## 2026-05-29 held on-screen-keyboard press animation

Rebuilt Pico-286 after adding pressed-key animation to the shared R36SX
on-screen keyboard.  When a physical accept shortcut presses a virtual key, the
drawn key shifts down-right and darkens.  The pressed state now follows the
physical button's held bit and stays visible until that physical button is
released.

This affects both normal DOS keyboard mode and the preset editor key picker.
Direct shortcuts animate their matching visible keys:

- `B`: `BS`
- `X`: `ESC`
- `Y`: `ENT`
- `A` or `Start`: the highlighted virtual key

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_076\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 917,844 bytes
- SHA256: `570E55DE4A0357D392A30C418B6152F88DD4A70F76B137F24DB997E9935699A1`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_076/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_076/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 remappable Select preset key

Rebuilt Pico-286 after making physical `Select` a normal remappable key preset
entry.  The default preset maps `Select` to `Space`, and the preset editor now
shows `SELECT` in the left column bottom slot opposite `START`.  Missing keys
in older `keypresets.conf` presets are filled from current defaults before
explicit config values are applied, so old configs receive the new `Select =
Space` binding unless they add an explicit `select=...` line.

`Fn+Select` remains the disk image binding menu shortcut.  When the on-screen
keyboard or a settings menu is visible, that UI keeps handling Select according
to its own rules instead of passing it to DOS.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_075\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 914,632 bytes
- SHA256: `04E15547B6DEB47BB9CCC781572B0070F672174B6E9AA099AC76BA96A8CE561E`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `disk_image/MIPS_NATIVE/pico_286/keypresets.conf`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/keypresets.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_075/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_075/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_075/MIPS_NATIVE/pico_286/keypresets.conf`
  - `patches/disk_image_patch_075/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 preset-only cursor-key block

Rebuilt Pico-286 after splitting the shared on-screen keyboard cursor-key
block behavior.  The normal DOS on-screen keyboard now always hides the
right-side cursor-key block.  The key picker used inside the preset editor
enables the block explicitly, so cursor keys are still available when assigning
physical buttons.

The default `pico_286.conf` no longer contains `osk_cursor_keys`, and the disk
config save path no longer writes that legacy setting.  Existing configs may
still contain the old key; it is accepted for compatibility but has no effect.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_074\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 914,080 bytes
- SHA256: `5B1D9D8887C512FFF48C125B084CFEA6B05CC9F4F7E481E0D65D2804EEB07E7F`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_074/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_074/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_074/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 edge-to-edge on-screen keyboard panel

Rebuilt Pico-286 after changing the shared R36SX on-screen keyboard geometry.
The keyboard panel is now drawn flush with the left, right, and bottom screen
borders.  The old header/status text was removed from the panel, and the key
rows are vertically centered with compact equal inner padding around the key
area.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_073\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 921,312 bytes
- SHA256: `F818B656F70CD88212B16B4EC1B5F460489A9573966FC8102AD187336FC3C3C9`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `disk_image/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/README.md`
  - `patches/disk_image_patch_073/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_073/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_073/MIPS_NATIVE/pico_286/README.md`

## 2026-05-29 remove PSRAM payload from Linux executable

Rebuilt Pico-286 after changing the upstream RAM/EMS/XMS storage attributes.
The `.psram` section is needed for Pico/RP-style hardware builds, but in this
Linux/MIPS native port it made the executable store 7 MB of zero-filled
emulated memory.

The new `PICO286_PSRAM_ATTR` macro keeps `.psram` only when `PICO_ON_DEVICE`
is true.  In the Linux/MIPS host build, these arrays now become ordinary
zero-initialized storage:

- `RAM`
- `UMB`
- `HMA`
- `EMS`
- `XMS`

Size check:

- Before: about 8,114,624 bytes.
- After: 922,472 bytes.
- `.psram`: removed from the file.
- `.bss`: now contains the emulated memory and is about 8.7 MiB at runtime.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_072\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 922,472 bytes
- SHA256: `68CCFDD09C2230CD862A5BBEABD88277F9BCA10087F5F2F0F9F12B0217CA5913`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_072/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_072/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 settings menu button defaults

Rebuilt Pico-286 after normalizing button behavior in settings menus.

Current settings-menu controls:

- `A` or `Y`: accept/activate the selected row, cycle a disk image, or accept
  the highlighted on-screen key in picker mode,
- `B` or `X`: cancel the disk menu, key preset editor, or picker mode.

This affects the disk image binding menu, the key preset editor, and the
on-screen keyboard picker used by preset binding/name editing.  The normal DOS
on-screen keyboard keeps its existing direct shortcuts.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_070\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,114,624 bytes
- SHA256: `0CC16E9BBD854D14EAF7DC5AF7D8E684383F4087A3B2EDE8E14A4E0AB27D99F5`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_070/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_070/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 preset rename cursor

Rebuilt Pico-286 after improving the preset-name editor.  While editing a
preset name, the `RENAME PRESET:` line now draws a blinking underscore cursor
next to the current text.  The small built-in 5x7 font used by the preset
editor now includes an underscore glyph.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_069\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,114,908 bytes
- SHA256: `C98A6ABE633D9D22AC9E153D695EE6B059DE42065E753B1F651FCF07D6942485`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_069/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_069/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 Fn+X and long-Fn exit

Rebuilt Pico-286 after changing the application exit gesture.  Select+Start no
longer exits, so DOS programs and games can use that combination without
closing the emulator.

Current exit controls:

- holding Fn and pressing X exits back to TinyMC,
- holding Fn by itself for more than 3 seconds exits back to TinyMC.

Existing Fn chord controls remain unchanged:

- tapping and releasing Fn toggles the on-screen keyboard,
- holding Fn and pressing Select opens the disk menu,
- holding Fn and pressing Start opens the key preset editor.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_068\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,114,488 bytes
- SHA256: `6DD9DC4A8528C38EE909FDF9CF2F13A6E42890CBBF61AAAC89FEEA98DBC809AD`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_068/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_068/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 Fn chord disk menu

Rebuilt Pico-286 with a new Fn chord layer and a disk image binding menu.

Input changes:

- tapping and releasing Fn toggles the on-screen keyboard,
- holding Fn and pressing Select opens the disk menu,
- holding Fn and pressing Start opens the key preset editor,
- Select alone no longer opens the key preset editor,
- Select+Start still exits the app.

The disk menu scans `.img` files next to `pico_286.conf`, lets the user cycle
bindings for `FDD0`, `FDD1`, `HDD0`, and `HDD1`, and includes `SAVE/APPLY`,
`EXIT APP`, and `CANCEL` rows.  `SAVE/APPLY` updates `pico_286.conf` and calls
`insertdisk()` for the current emulator session.  Hard-disk changes may still
need an emulator restart because DOS can cache mounted drive state; the menu's
`EXIT APP` row is intended for that path.

The config layer now keeps writable in-memory values for disk image bindings
and can write the current config back to `pico_286.conf`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_067\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,113,476 bytes
- SHA256: `FC94F00C0F93F611F6C52CDEA587C3DF8A7F685594ED82A773B8D3376E0FFFCD`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_067/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_067/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 optional on-screen cursor-key block

Rebuilt Pico-286 after adding an optional cursor-key cluster to the shared
on-screen keyboard module.  The cluster is drawn on the right side of the
keyboard in an inverted-T physical keyboard layout: Up above Left/Down/Right.
When enabled, the normal on-screen keys shrink from 44 to 36 pixels wide so the
full keyboard still fits in the existing 96-pixel panel.

The shared module defaults to no cursor block for compatibility.  Pico-286 now
enables it from `pico_286.conf`:

```ini
osk_cursor_keys=on
```

The option accepts `on`, `off`, `yes`, `no`, `true`, `false`, `1`, and `0`.

Checked local TTF coverage for arrow glyphs.  DejaVu Sans Mono, Hack,
JetBrains Mono NL, Liberation Mono, Noto Sans Mono, and Source Code Pro contain
`U+2190..U+2193`; Roboto Mono does not.  The runtime keyboard still uses its
small built-in 5x7 bitmap glyphs, so no new font dependency was added.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.  The remaining compiler warnings are the known upstream
network redirector pointer-sign warnings and one VGA palette sequencing
warning.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_066\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,076,964 bytes
- SHA256: `F8FA17D225EBF84BAE8AE5D1D7364FE1AD11B3D5D36002BE1DD8D9C67C6DFC4F`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_066/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_066/MIPS_NATIVE/pico_286/pico_286.conf`

## 2026-05-29 FAT16 hard disk images and disk read diagnostics

Investigated DOS `DIR` failing with `Cannot read` / retry / abort.  The local
`disk_image/MIPS_NATIVE/pico_286/hdd.img` and `hdd2.img` were still blank raw
images: sector 0 had no MBR, no BPB, and no `55 AA` signature.  With BIOS drive
`80h` attached to a blank image, DOS can see a disk device but fail when reading
the filesystem.

Added `tools/create_fat16_hdd.py` and generated both hard disk images as
readable FAT16 disks:

- disk size: 33,546,240 bytes
- CHS: `65,16,63`
- MBR primary partition: active FAT16 type `0x06`
- partition start: sector `63`
- partition size: `65457` sectors
- FAT16 VBR: `63` sectors per track, `16` heads, hidden sectors `63`
- root directory contains `README.TXT` for an immediate `DIR C:` check.

Also rebuilt Pico-286 with `INT 13h` diagnostics in
`r36sx_port/disks-win32.c.inl`.  Failed reads, writes, verifies, invalid CHS
requests, out-of-range transfers, and host file I/O failures are logged to
`pico_286.log` in debug builds.

Commands used:

```powershell
python .\tools\create_fat16_hdd.py --output .\disk_image\MIPS_NATIVE\pico_286\hdd.img --geometry 65,16,63 --label "R36SX HDD" --file README.TXT=.\homebrew\pico_286\dos_files\hdd\README.TXT
python .\tools\create_fat16_hdd.py --output .\disk_image\MIPS_NATIVE\pico_286\hdd2.img --geometry 65,16,63 --label "R36SX HDD2" --file README.TXT=.\homebrew\pico_286\dos_files\hdd\README.TXT
Copy-Item -LiteralPath .\disk_image\MIPS_NATIVE\pico_286\hdd.img -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\hdd.img -Force
Copy-Item -LiteralPath .\disk_image\MIPS_NATIVE\pico_286\hdd2.img -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\hdd2.img -Force
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

The `-TryStrip` step still reports Zig objcopy `unimplemented`, so the build
kept the unstripped executable.  Existing warnings are from the upstream
network redirector pointer-sign conversions and one old VGA palette expression.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_064\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\hdd.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\hdd2.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\hdd.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\hdd2.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_064\MIPS_NATIVE\pico_286\hdd.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_064\MIPS_NATIVE\pico_286\hdd2.img
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,058,240 bytes
- SHA256: `43E4C5A227FE8E83F4A643F1546F6E9B51E79D509B78BA3E13C4B1C75CDC0332`
- `hdd.img` SHA256:
  `26953A16F571AB8452570E37E2C0688C0B60A2859E72B81D6E9EBA8D80379818`
- `hdd2.img` SHA256:
  `45A095789D0C5A4F8E0BB2717493874C761789AF199D602BF794DBAB12448C71`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/hdd.img`
  - `disk_image/MIPS_NATIVE/pico_286/hdd2.img`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/hdd.img`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/hdd2.img`
  - `patches/disk_image_patch_064/MIPS_NATIVE/pico_286/*`

## 2026-05-29 boot order and HDD geometry config

Added two BIOS-like configuration controls to `pico_286.conf`:

```ini
boot_order=fdd0,hdd0
hdd0_geometry=65,16,63
hdd1_geometry=65,16,63
```

Behavior:

- `boot_order` accepts `fdd0`, `fdd1`, `hdd0`, and `hdd1` in comma/space
  separated order.  `boot_order=rom` disables the R36SX boot-sector loader and
  lets the embedded ROM BIOS boot path run.
- The R36SX `INT 19h` hook now attaches configured disks, probes boot sectors
  in `boot_order`, checks the `55 AA` signature, loads the selected sector to
  `0000:7C00`, sets `DL` to the selected BIOS drive, and jumps there.
- `hdd0_geometry` and `hdd1_geometry` accept CHS triples in
  `cylinders,heads,sectors` order.  Values are range checked and ignored if
  they describe a disk larger than the backing image.
- `INT 13h AH=15` now reports total sectors from active CHS geometry rather
  than raw file size, so geometry overrides are visible to DOS.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,037,084 bytes
- SHA256: `DB5654656F9C97D19557E63E38E46D8A5E4A604916C4F52092670C5698088B58`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries

## 2026-05-29 BIOS boot prompt mode

Checked whether Pico-286 can enter an interactive BIOS setup screen at startup.
The embedded ROM is `Turbo XT BIOS v3.1`, mapped at `0xFE000`, and it contains
boot/diagnostic strings such as `No ROM BASIC, booting from disk...` and
`Insert BOOT disk in A:`, but no full CMOS/BIOS setup utility.

Added a config boot mode instead:

```ini
boot_mode=normal
```

Supported values:

- `normal`: attach configured disks during BIOS `INT 19h` and boot DOS.
- `bios_prompt` (aliases: `bios`, `prompt`): leave disks detached at
  `INT 19h`, so the ROM BIOS stops at its boot prompt.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,016,928 bytes
- SHA256: `4AF76508FCF896130572E53E0D4B1A6E3BF5060FAB02817888CDFF17FE4194AD`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries

## 2026-05-29 configurable CPU speed

Checked the active R36SX Pico-286 main loop for a CPU speed binding.  The port
does not model a cycle-exact 80286 MHz clock.  Its practical CPU speed knob is
the execution quantum passed to `exec86()` once per millisecond-like host loop;
before this change it was hard-coded as `exec86(32768)`.

Changes:

- Added `cpu_mhz` parsing to `pico_286.conf`.
- `cpu_mhz=32.768` maps to `exec86(32768)`, preserving the previous behavior.
- Invalid values outside `0.100` to `250.000` MHz are ignored and the built-in
  fallback is used.
- Added the default `cpu_mhz=32.768` line to the homebrew, `disk_image`, and
  patch configs.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,010,412 bytes
- SHA256: `FE190E432D63957A91728F0E9BCEC7895BF5ECE7FC3D99291D0211E10BD84B43`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries

## 2026-05-29 BIOS hard disk registration

Added BIOS-visible hard disk presence reporting for DOS/FDISK.

Pico-286 does not have an interactive BIOS setup screen.  The hard disk
geometry is derived by the emulator when `hdd.img` is inserted: hard disks use
63 sectors, 16 heads, and the cylinder count is calculated from image size.
For the current 33,546,240-byte `hdd.img`, that is 65/16/63.

The previous build answered `INT 13h AH=08` with geometry, but did not update
the BIOS Data Area fixed-disk count at `0040:0075`.  Some DOS code checks that
byte, and some disk tools also ask `INT 13h AH=15` for the drive type.

Changes:

- Update BIOS Data Area byte `0040:0075` whenever a hard disk is inserted or
  ejected.
- Add `INT 13h AH=10` drive-ready and `AH=11` recalibrate success responses for
  inserted drives.
- Add `INT 13h AH=15` get-disk-type support: hard disks return type `0x03`
  and `CX:DX` total sectors, floppies return type `0x02`.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,006,884 bytes
- SHA256: `0737E66B6DB7C0165F3A1B22BBF3200678CEA8E7BDC339A827DA56392234ECBF`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries

## 2026-05-29 hard disk write flush fix

Investigated why FreeDOS could appear to format DOS `C:` successfully, but the
hard disk was not visible after rebooting inside Pico-286.  DOS `C:` is mounted
as BIOS drive `80h`, normalized to internal disk slot `2`, from `hdd0=hdd.img`
in `pico_286.conf`.

The disk layer had two persistence problems:

- `writedisk()` wrote sectors with `fwrite()` but did not flush the host file.
- Re-running BIOS `INT 19h` mounted the disk images again, but `ejectdisk()`
  only cleared the inserted flag; it did not `fflush()`/`fclose()` the old host
  `FILE *`.

This meant a format/write could remain buffered on the old handle while reboot
opened the same `hdd.img` through a new handle and saw stale contents.

Changes:

- `ejectdisk()` now flushes and closes the old image file before clearing the
  slot.
- Hard-disk/floppy counters now use the normalized internal slot number:
  slots `0`/`1` are floppy, slots `2`/`3` are hard disks.
- `writedisk()` now checks `fwrite()` result and calls `fflush()` after sector
  writes, returning BIOS write-fault status `0xCC` on failure.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,006,344 bytes
- SHA256: `451570BAC02DBADDF73AC3A9B2EB6CF96A9A31C1E2943ADC766C8A6A455F03EC`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries

## 2026-05-29 FreeDOS image rename

Renamed the local FreeDOS floppy images to make their role clearer:

- `fdd0.img` -> `FreeDOS1.img` (`144m/x86BOOT.img`, bootable FreeDOS floppy)
- `fdd1.img` -> `FreeDOS2.img` (`144m/x86DSK01.img`)
- `fdd2.img` -> `FreeDOS3.img` (`144m/x86DSK02.img`)

The current `pico_286.conf` keeps Sopwith mounted as DOS `B:`:

```ini
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd1=hdd2.img
```

The binary fallback names in `r36sx_cpu.c` intentionally remain the legacy
`fdd0.img`/`fdd1.img`; the active renamed disks are selected through
`pico_286.conf`.

Commands used:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,005,860 bytes
- SHA256: `583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5`
- Defender scan: found no threats for the rebuilt binary and both copied
  binaries
- Renamed FreeDOS image copies in both `disk_image` and
  `patches/disk_image_patch_pico_286` were also scanned; Defender reported no
  threats.

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

## 2026-05-29 key preset editor physical layout

Rebuilt Pico-286 after changing the key preset editor from an automatic
half-list split to an explicit physical console layout:

- left column: d-pad, `L`, `L2`, and static `SELECT CANCEL` at the bottom,
- right column: `X`, `Y`, `A`, `B`, `R`, `R2`, and `START` at the bottom.

The saved `keypresets.conf` keys are unchanged.  `Select` remains a service
button for opening/canceling the preset editor instead of becoming a remappable
DOS key.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`-TryStrip` again reported Zig objcopy `unimplemented`, so the unstripped
executable was kept.

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_065\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,061,088 bytes
- SHA256: `7770B1BD818DA27E1E4F7B924ACABC95B193509314341B24B6CC769F690D78C9`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_065/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 key preset editor draft UI

Rebuilt Pico-286 after reworking the Select key preset editor:

- the physical button binding list is now split into two columns so it stays
  inside the outer frame;
- opening the editor starts a draft session, and changes are written only by
  selecting the visual `OK` button;
- `Cancel` or `Select` closes the editor without saving the draft;
- the preset name can be edited with the shared on-screen keyboard picker;
- pressing A/Start on a button binding row opens the same on-screen keyboard as
  a key picker instead of cycling through a hardcoded list.

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

The `-TryStrip` step still reports Zig objcopy `unimplemented`, so the build
kept the unstripped executable.

Copy commands:

```powershell
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_063\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\keypresets.conf -Destination patches\disk_image_patch_063\MIPS_NATIVE\pico_286\keypresets.conf -Force
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_063\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,054,724 bytes
- SHA256: `598071628DA6ECA5B608625CB636AEF76977BB5C59D50039B4E93BB37FC2E4E1`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_063/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 Sopwith DOS game floppy

Downloaded Sopwith (The Author's Edition) from DOS Games Archive as a small
286-compatible DOS game candidate.  The selected package is listed by DOS Games
Archive as `Freeware (installed)`, `MS-DOS`, 42 kB, and the page description
states that the author's edition was released under the GNU GPL with source
available separately.

Commands used:

```powershell
New-Item -ItemType Directory -Force -Path .\internet_sources\sopwith_ae
Invoke-WebRequest -Uri 'https://www.dosgamesarchive.com/file.php?id=2395' -OutFile .\internet_sources\sopwith_ae\swae-box.zip
.\tools\scan-download.ps1 .\internet_sources\sopwith_ae\swae-box.zip
Expand-Archive -LiteralPath .\internet_sources\sopwith_ae\swae-box.zip -DestinationPath .\internet_sources\sopwith_ae\swae-box -Force
.\tools\scan-download.ps1 .\internet_sources\sopwith_ae\swae-box
python .\tools\create_fat12_floppy.py --output .\homebrew\pico_286\sopwith.img --label SOPWITH --file SW.EXE=.\internet_sources\sopwith_ae\swae-box\sopwith\sw.exe --file SOPWTH.BAT=.\homebrew\pico_286\dos_games\sopwith\SOPWTH.BAT --file README.TXT=.\homebrew\pico_286\dos_games\sopwith\README.TXT
Copy-Item -LiteralPath .\homebrew\pico_286\sopwith.img -Destination .\disk_image\MIPS_NATIVE\pico_286\sopwith.img -Force
Copy-Item -LiteralPath .\homebrew\pico_286\sopwith.img -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\sopwith.img -Force
.\tools\scan-download.ps1 .\homebrew\pico_286\sopwith.img
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\sopwith.img
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\sopwith.img
```

Result:

- Downloaded ZIP: `internet_sources/sopwith_ae/swae-box.zip`
- ZIP SHA256: `689BC2B80BFE9A7079F94C7A550BA2E0D7FA1842DC4F9C6BCED225A7424BAFBA`
- New image: `homebrew/pico_286/sopwith.img`
- Image size: 1,474,560 bytes
- Image SHA256: `14D8A70F3288DA8B0A9AAF4577F2BCA84E89E35D614F1FAA913EA6490ED8ACB2`
- FAT12 root entries: `SW.EXE`, `SOPWTH.BAT`, `README.TXT`
- Defender scan: found no threats for the ZIP, extracted folder, and all three
  `sopwith.img` copies
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/sopwith.img`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/sopwith.img`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`

`pico_286.conf` now maps `fdd1=sopwith.img`, so after FreeDOS boots from
`A:`, the game floppy is available as DOS drive `B:`.

## 2026-05-29 key preset editor

Added `keypresets.conf` and a full-screen key preset editor to the Pico-286
MiniFB backend.  While DOS is running and the on-screen keyboard is hidden,
Select opens or closes the editor.  D-pad chooses a row, Left/Right or A/Start
changes a binding, B cycles backward, Y clears a binding, and the `ADD NEW
PRESET` row creates a copy of the active preset with an automatic name.

The default preset matches the previous direct mapping:

```ini
active=Default

[preset Default]
up=UP
down=DOWN
left=LEFT
right=RIGHT
a=ENTER
b=ESC
y=CTRL
x=SPACE
start=ENTER
l=ALT
l2=F1
r=SHIFT
r2=F2
```

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Copy commands:

```powershell
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 8,005,860 bytes
- SHA256: `583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/keypresets.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/keypresets.conf`

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

## 2026-05-29 on-screen keyboard X Escape binding

The shared on-screen keyboard module was rebuilt into Pico-286 after changing
the physical `X` button behavior while the keyboard is visible.  `X` now sends
Escape immediately instead of toggling Shift.  Shift remains available through
the on-screen `SHF` key.

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Copy commands:

```powershell
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,965,676 bytes
- SHA256: `91D85743E2A32C304407E8BFA1B1D92F732C2FA024F6E73CD4F9DE82C4B66048`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

## 2026-05-29 keyboard scancode pacing

The emulated keyboard controller now waits 1 ms before making each queued
scancode byte available through port `0x60`.  This approximates PS/2 keyboard
wire timing: one device-to-host byte is an 11-bit frame, and a 10-16.7 kHz
clock gives about 660-1100 us per scancode byte.

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

## 2026-05-29 reusable on-screen keyboard module

The Pico-286 binary was rebuilt after moving the joystick-controlled on-screen
keyboard implementation into the reusable shared module
`homebrew/common/r36sx_screen_keyboard.*`.  The device-facing keyboard behavior
should remain the same; Pico-286 now uses the shared module through a small
callback adapter.

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

## 2026-05-29 on-screen keyboard face-button remap

The Pico-286 binary was rebuilt after changing the shared on-screen keyboard
bindings.  While the keyboard is visible, `B` now sends Backspace immediately
and `Y` now sends Enter immediately.  `Select` closes the keyboard, and the
on-screen `CLOSE` key remains available.

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

## 2026-05-29 disk image config

Added `pico_286.conf` so the native build no longer has to hard-code all disk
image filenames in `r36sx_cpu.c`.  The config lives next to the executable and
maps `fdd0`, `fdd1`, `hdd0`, and `hdd1` to BIOS drives `00h`, `01h`, `80h`,
and `81h`.  Relative paths are resolved from the directory that contains
`pico_286.conf`.

Default config:

```ini
fdd0=fdd0.img
fdd1=fdd1.img
hdd0=hdd.img
hdd1=hdd2.img
```

Empty values disable a drive.  If the config is missing, the build falls back
to the same four filenames.

Build command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1
```

Copy commands:

```powershell
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- Output: `homebrew/pico_286/pico_286`
- Size: 7,965,128 bytes
- SHA256: `A63CD65C36BE12E56E59EFF677F5C298DE6BD701993DC6EBC0E004D5A1151623`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.conf`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.conf`

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
