# disk_image_patch_pico_286

Adds the first native pico-286 port build for TinyMC.

Copy this patch over the original SD-card filesystem root.  It installs:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/README.md`
- `MIPS_NATIVE/pico_286/keypresets.conf`
- `MIPS_NATIVE/pico_286/test386.bin`
- `MIPS_NATIVE/pico_286/images/cpu_tests.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS1.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS2.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS3.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS4.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS5.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS6.img`
- `MIPS_NATIVE/pico_286/images/FreeDOS7.img`
- `MIPS_NATIVE/pico_286/images/sopwith.img`
- `MIPS_NATIVE/pico_286/images/hdd.img`
- `MIPS_NATIVE/pico_286/images/hdd2.img`

Launch it from TinyMC by opening `MIPS_NATIVE/pico_286/pico_286`.

The default binary is built with `DEBUG=0`.  Builds made with
`build_pico_286.ps1 -DebugLog` write a startup/runtime log to:

- `MIPS_NATIVE/pico_286/pico_286.log`

If that path cannot be opened on the device, it falls back to:

- `pico_286.log` in the SD-card root

## 2026-05-31 POST-code overlay and standard test ROM port

The current `pico_286` binary can show POST-code writes with `Fn` + D-pad
`Right`.  It captures standard BIOS POST writes to port `80h` and keeps legacy
support for the older R36SX test386 POST port `190h`.  The embedded Turbo XT
BIOS image has no obvious `OUT 80h` POST sequence, so the normal BIOS may not
produce values.

The current `test386.bin` was rebuilt with `POST_PORT equ 0x80`; ASCII debug
output still uses `OUT_PORT equ 0x191`.  `images/cpu_tests.img` was also
rebuilt so its `TEST386.BIN` payload matches.

pico_286 size: 1389532 bytes
pico_286 SHA256: 637792DF7404B28B93576CB4D84764FCD83719492DFDDFD096973700526C4029
test386.bin size: 65536 bytes
test386.bin SHA256: 9BD72BB8ACC9FD8BDCB81E7DD9E5E756D16A5608B66CF8E44D6F26751107F67B
images/cpu_tests.img size: 1474560 bytes
images/cpu_tests.img SHA256: EEA493C9E526B8083F96F3FB4A93326459A4D425F28E8E4F4DF106C116D42447

## 2026-05-31 stable direct-overlay present buffer

The current `pico_286` binary fixes flicker in small direct overlays.  App
statistics, the disk LED, and the overlay keyboard now draw into a stable output
frame before calling `driver.so`, instead of drawing into `SCREEN` and
restoring it immediately after `video_driver_disp_frame()`.  Plain DOS frames
still present `SCREEN` directly when no overlay is active.

pico_286 size: 1386804 bytes
pico_286 SHA256: 37CB2FD6156F8EE3E6270E6AE3BF0202D09D3484CC717C20CD5F78876A8DD9A5

## 2026-05-31 fullscreen menu direct draw

The current `pico_286` binary skips DOS-frame composition while the full-screen
disk menu or key preset editor is visible.  Those menus fill the whole output
frame themselves, so MiniFB no longer prepares `base_frame`, copies it into the
present frame, or mixes small overlays over those menu frames.

pico_286 size: 1391064 bytes
pico_286 SHA256: 3258BA6D20AD2591F4E9F6D0F7827E6D17D8BF250BDE468DC7033CA5A332425D

## 2026-05-31 cached overlay keyboard

The current `pico_286` binary optimizes `[video] keyboard_mode=overlay`.
The keyboard panel is rendered into a dedicated `640x96` RGB565 cache that is
updated only when the visible keyboard state changes.  A later build draws that
cached panel into a stable output frame instead of restoring `SCREEN`
immediately after present.  If the cache buffer cannot be allocated, the old
full composition path remains as a fallback.

pico_286 size: 1389944 bytes
pico_286 SHA256: A8C42591D887C254D37E212EDC64557E4E745B1F9DAE9CBB4247F351486A1620

## 2026-05-31 configurable on-screen keyboard mode

The current `pico_286` binary reads `[video] keyboard_mode` from
`pico_286.conf`.

- `normal` keeps the previous behavior: the DOS image is resized above the
  on-screen keyboard.
- `overlay` draws the on-screen keyboard over the DOS image without resizing
  the underlying framebuffer.

pico_286 size: 1381244 bytes
pico_286 SHA256: 74B49EADAB77E66333D07D7E058FFDFE5514DB24E18A235B2546D81888922CB3

## 2026-05-31 dirty video rendering

The current `pico_286` binary tracks visible video changes and only rerenders
the DOS framebuffer after emulated VRAM, video registers, palette state,
active page, or cursor/blink state changes.  Small overlays such as disk LED
and app statistics later changed to use a stable output frame without forcing a
full DOS-screen render.

pico_286 size: 1374052 bytes
pico_286 SHA256: 27B016E967771004E0D489E9977A17B262A6E90CF718BBC60613E64004857D8D

## 2026-05-31 configurable audio sample rate

The current `pico_286` binary reads `[audio] audio_sample_rate` from
`pico_286.conf`.

- `44100` is the default R36SX native mixer rate.
- `22050` halves the internal Pico-286 mixer work and is duplicated back to
  44.1 kHz before sending samples to the stock `driver.so` output path.

pico_286 size: 1368624 bytes
pico_286 SHA256: 29F5235670480F9B648267CC5A4FCA29AEB03D72C0864E05EE5F0CCB89759D91

## 2026-05-31 configurable audio device mix

The current `pico_286` binary reads `[audio]` switches from `pico_286.conf`.
The built-in PC speaker/beeper is always enabled; the following non-speaker
devices can be muted independently while their emulated I/O ports remain
visible to DOS software:

- `audio_adlib`
- `audio_sound_blaster`
- `audio_cms`
- `audio_sn76489`
- `audio_mpu401`
- `audio_disney`
- `audio_covox`

pico_286 size: 1363884 bytes
pico_286 SHA256: 558284BB5DBC900280A412071ACE7049069C3AC1BAB8686D0560DE52B0019CFE

## 2026-05-31 cached pixel-font stats overlay

The current `pico_286` binary draws the `Fn` + D-pad `Down` statistics overlay
with a compact built-in pixel font.  The rendered stats block is cached between
one-second statistics samples.  When no large menu or keyboard overlay is
active, the block is drawn into `SCREEN` using the same save/present/restore
path as the disk activity LED instead of forcing full-frame composition.

pico_286 size: 1350728 bytes
pico_286 SHA256: D379B1E0CA43D28A40FB1E8A760E8DE4672514EBA27A9F2A44B6CCE64570D1B5

## 2026-05-31 configurable scaling filter

The current `pico_286` binary reads `[video] scaling_filter` from
`pico_286.conf`.

- `nearest` is the default and keeps sharp pixel edges.
- `bilinear` blends neighboring source rows when the DOS image is resized
  under large overlays such as the on-screen keyboard.

pico_286 size: 1337836 bytes
pico_286 SHA256: 0006C7F8D02D7F726CCEDEB8D7D2D1AD7853032B78BDFA85110A2D884C5C39E3

## 2026-05-31 CRTC stride for tweaked VGA modes

The current `pico_286` binary uses VGA CRTC Offset Register `13h` when
rendering graphics rows and preserves both bytes of the CRTC start address.
This is intended to fix tweaked planar layouts such as the Supaplex/SPFIX
320-wide mode with a 122-byte logical row.

pico_286 size: 1330908 bytes
pico_286 SHA256: 7A56639A52C887A0CF831BB97976B4C7418FD3978174DC50400CE1FF7570B9E2

## 2026-05-31 DOSBox 386 CPU comparison fixes

The current `pico_286` binary adopts several small CPU semantics from DOSBox:
protected-mode-only `0F 00`, implemented `CLTS`, operand-size-aware
`LGDT`/`LIDT`, 386-style CR0 reserved-bit reads, and page-directory-aligned
CR3 reads/writes.

pico_286 size: 1332700 bytes
pico_286 SHA256: 46FB1C062F4400D0CC54CAA8B2586CDD8CE38667C8290436FC680AEAE29C3C10

## 2026-05-31 minimum adaptive quantum raised

The current `pico_286` binary keeps the adaptive `exec86()` quantum at no less
than 1000 instructions per frame.

pico_286 size: 1332160 bytes
pico_286 SHA256: B5E9101584E255B90BA51C61EC094F8D4AB1CF680896292366BC0F2717B60681

## 2026-05-31 adaptive quantum ignores overlay rendering cost

The current `pico_286` binary adjusts the `exec86()` quantum from the elapsed
time of the `exec86()` call itself.  Rendering overlays, including the
on-screen keyboard, no longer cause the x86 instruction quantum to drop.

pico_286 size: 1332176 bytes
pico_286 SHA256: A4B62760CA64B10D8171D39A91EB64F8E97735731FD87B5CA8D2319941D89194

## 2026-05-31 adaptive exec86 quantum and target FPS

The current `pico_286` binary adds adaptive main-loop CPU quantum control.

- `pico_286.conf` now contains `[timing] target_fps=60`.
- The target frame budget is derived from `target_fps` instead of a hard-coded
  16 ms value.
- The maximum `exec86()` quantum is still derived from `cpu_mhz`.
- Slow active frames reduce the quantum toward the budget by at most one
  quarter per frame.
- Fast active frames grow the quantum back by one quarter per frame.
- The minimum quantum is 1000 instructions per frame.

pico_286 size: 1332016 bytes
pico_286 SHA256: 4D8F4D8C89C6E5BDA4831D375B2449A4C246FF0702CB3084C994F960F6F71604

## 2026-05-31 disk image directory migration

All Pico-286 `.img` files in the patch are now stored under
`MIPS_NATIVE/pico_286/images/`.

- `pico_286.conf` uses `images/...` paths for floppy and hard-disk bindings.
- The disk menu scans `images/` first, then also scans the Pico-286 root for
  legacy root-level images.
- Legacy config values without a directory separator, such as `FreeDOS1.img`,
  resolve to `images/FreeDOS1.img` when the root file is missing.
- `cpu_tests.img`, FreeDOS floppy images, Sopwith, `hdd.img`, and `hdd2.img`
  should be copied inside `MIPS_NATIVE/pico_286/images/`.

pico_286 size: 1325040 bytes
pico_286 SHA256: 23C2463906890A1DC830F0528E66E6C14AF29DBA86EB4384C9B1D4D32D7D0D7F
images/cpu_tests.img size: 1474560 bytes
images/cpu_tests.img SHA256: C4A8CC49F87BD3BBFEDC77061B18065603BB5297E0D97BF13AA18B06F9A1C483

## 2026-05-31 BIOS mode menu and test386 ROM

The current patch includes an updated `cpu_tests.img` with `TEST386.BIN`, an
R36SX debug build of `barotto/test386.asm`.

- `TEST386.BIN` is a 64 KB BIOS replacement ROM payload, not a DOS `.COM`
  program.
- The source copy is stored in `homebrew/pico_286/tests/test386.asm`.
- The build used `DEBUG=1`, POST port `190h`, and ASCII output port `191h` at
  the time; a later build changed the POST port to standard `80h`.
- The current `pico_286` binary logs those ports to `pico_286.log` as
  `test386:` lines.
- The disk menu BIOS row switches `bios=normal` / `bios=test386`; the external
  ROM file is configured as `test_bios_rom=test386.bin`.
- `MIPS_NATIVE/pico_286/test386.bin` is copied next to the executable and is
  mapped at physical `F0000h-FFFFFh` when the test BIOS is selected.

pico_286 size: 1325040 bytes
pico_286 SHA256: 23C2463906890A1DC830F0528E66E6C14AF29DBA86EB4384C9B1D4D32D7D0D7F
test386.bin size: 65536 bytes
test386.bin SHA256: 7E91F03B910FE52508D28ADD2AC4CF4F73B3D23F5DB7B77A5315D6F0DD234497
cpu_tests.img size: 1474560 bytes
cpu_tests.img SHA256: C4A8CC49F87BD3BBFEDC77061B18065603BB5297E0D97BF13AA18B06F9A1C483

## 2026-05-31 VGA DAC palette handling

The current `pico_286` binary fixes VGA DAC palette programming for mode 13h
software that writes palette entries through ports `3C7h`, `3C8h`, and `3C9h`.

- DAC write/read component counters are reset when the guest writes `3C8h` or
  `3C7h`.
- `3C9h` writes now accept real VGA 6-bit RGB values and expand them to RGB888.
- `3C9h` reads now return RGB components sequentially and auto-increment after
  blue.
- BIOS palette services `INT 10h AX=1010h/1012h/1015h/1017h` use the same DAC
  helpers, and block set/read now handles `CX=256`.

pico_286 size: 1308072 bytes
pico_286 SHA256: 61DE9B773E8814C44D1EDDF626C73B8B29D4BFA794E1EBBD883BAF9E8747B9CB

## 2026-05-30 compiler warning cleanup

The current `pico_286` binary was rebuilt after cleaning the remaining native
R36SX compiler warnings:

- DOS redirector 8.3 names now write into a byte buffer, avoiding pointer-sign
  warnings for DTA file names.
- VGA DAC block palette loading no longer increments `memloc` multiple times
  inside a single `rgb()` macro call.

pico_286 size: 1128984 bytes
pico_286 SHA256: F14EC8268BD4B4CFF86DB65BCB16FE70C0675353EF658FC49DC1EA6A285B9D10

## 2026-05-30 CHKDSK invalid-opcode investigation

The current `pico_286` binary adds the 80386 bit-test instruction family:
`BT`, `BTS`, `BTR`, `BTC`, and the `0F BA /4..7` immediate-bit forms.  These
were still missing from the 386 real-mode path and are plausible candidates for
the FreeDOS `CHKDSK.EXE` `Invalid Opcode` trap shown while checking drive C:.

Debug builds also log the exact faulting `CS:IP`, the next 8 opcode bytes,
flags, and key registers before raising `INT 6`.

pico_286 size: 1128748 bytes
pico_286 SHA256: F8D3502B5D27E1ED9DA73EE624E20933C4E38AB7732DE5B962BF45ED39E2FCC9

## 2026-05-30 60 Hz main-loop frame pacing

The current `pico_286` binary now paces the native main loop to 60 Hz.  Each
iteration targets a `16666 us` frame period and yields the remaining time to
Linux with `usleep()`.

The configured `cpu_mhz` value still means CPU work per millisecond; the binary
scales that value to a per-frame `exec86()` quantum so the emulator does not
become roughly 16 times slower after the loop pacing change.

pico_286 size: 1126056 bytes
pico_286 SHA256: B4AC18F1E4C0D75B16A5FD2BA8EFEDBC3034DDA9E75A9C4F190C9CA1BD8083C7

## 2026-05-30 sleep only on emulated HLT

The current `pico_286` binary now yields to Linux only when the emulated CPU has
executed `HLT` and is waiting for an interrupt.  Active DOS code no longer needs
a fixed sleep in every main-loop iteration.  The idle sleep is currently
`1000 us`; timer IRQs from the ticks thread wake the CPU on the next `exec86()`
call.

pico_286 size: 1124696 bytes
pico_286 SHA256: 2F59AB363B2DAA1B28B07F86BC4415D3B5745B5913D090344714E69D084BDEC3

## 2026-05-30 80386 real-mode instruction coverage

The current `pico_286` binary expands the 80386 real-mode path:

- common `66h` operand-size override forms now execute as 32-bit operations
  for `MOV`, ALU, stack, shifts, strings, near branches, and group opcodes;
- selected 386 `0F xx` user instructions are supported: near `Jcc`, `SETcc`,
  `PUSH`/`POP FS/GS`, `IMUL`, `MOVZX`, `MOVSX`, `BSF`, and `BSR`;
- 32-bit ADD/ADC/SUB/SBB flag handling was corrected.

Protected-mode 386 execution is still WIP; keep `cpu_mode=real` for normal DOS
use.

pico_286 size: 1124260 bytes
pico_286 SHA256: C3D87363CBC3B75902E618C6D26857E76240D56C0F81D4DBFB39AC37F0567451

## 2026-05-30 configurable emulated memory sizes

`pico_286.conf` now has a `[memory]` section:

```ini
[memory]
conventional_kb=640
upper_kb=176
extended_kb=64
xms_kb=4096
```

The values are runtime limits over the compiled-in maximum buffers.
`conventional_kb` is reported through the BIOS Data Area, `upper_kb` limits XMS
UMB allocations from `D000:0000` upward, `extended_kb` is returned by
`INT 15h AH=88h`, and `xms_kb` limits the built-in XMS handler.

```text
pico_286 size: 1018680 bytes
pico_286 SHA256: 14524A7A06ADB151CAD455BB42293D4165E6866075013B499E7FC935DB50446B
Defender scan: found no threats
```

## 2026-05-30 experimental 386 protected mode core

The current `pico_286` binary includes an experimental protected-mode layer.
The VM still starts in real mode, but guest code can now enter protected mode
through `LMSW` or `MOV CR0`, load `GDTR`/`IDTR`, reload GDT-backed segment
descriptors through far control transfers, and dispatch basic protected
interrupt/trap gates.  Implemented 386 system opcodes include `0F 00`,
`0F 01`, `0F 20`, and `0F 22`.

Still incomplete: paging, privilege checks, call gates, task switching, TSS
stack switching, and full 32-bit `EIP` execution.

```text
pico_286 size: 1304904 bytes
pico_286 SHA256: 3FCAADC76A38CE59A854A75E0AF2D6664C04FA20CE0DA94915801CEBAFB5564A
Defender scan: found no threats
```

## 2026-05-30 PCjs CPU test floppy

Added `MIPS_NATIVE/pico_286/cpu_tests.img`, a 1.44 MB FAT12 floppy image with
PCjs `ID.COM` and `TEST386.COM`, plus `CPUID.ASM`, `ID.ASM`, and `README.TXT`.
Boot FreeDOS, mount this image as `B:` from the Pico-286 disk menu or set
`fdd1=cpu_tests.img`, then run `ID` or `TEST386`.

```text
cpu_tests.img size: 1474560 bytes
cpu_tests.img SHA256: 8005293284DC5486E1268C495356B319EEBCE4F3ECB3CD76FE09CCC5DEDCF76D
Defender scan: found no threats
```

## 2026-05-30 centered screenshot toast

The screenshot confirmation toast now appears centered on the screen.  The
small screenshot preview remains inside the toast box below the status message.

```text
pico_286 size: 1009420 bytes
pico_286 SHA256: 8738F3780244DAD130E7DAF70EA010AD6C10E9255FD49168CDD7D6330BB585B0
Defender scan: found no threats
```

## 2026-05-30 statistics x86 rate in K/s

The `X86` row in the on-screen statistics table now displays thousands of
decoded x86 instruction loops per second as `K/s`, matching the compact disk
read/write rate style.

```text
pico_286 size: 1009336 bytes
pico_286 SHA256: 16F3DD7ED93EADB8557433A71EF576AE297F7F6C993110870BDA34FD58BE9FCF
Defender scan: found no threats
```

## 2026-05-30 lower-right statistics table

The `Fn` + D-pad `Down` statistics overlay now draws a compact lower-right
two-column table directly above the disk LED instead of a single top-left line.
The MiniFB 8x8 text helper also uses the same bit direction as the DOS text
renderer, fixing mirrored characters in the statistics overlay.

```text
pico_286 size: 1009372 bytes
pico_286 SHA256: ED98F6095FD5858A155A5AAA3131FB76D8FAC32D15327FB47E7CEEE38350CFDC
Defender scan: found no threats
```

## 2026-05-30 CGA raw video memory path

The Pico-286 binary was rebuilt to keep CGA/text/Tandy/Hercules video memory as
raw byte-addressable memory instead of always passing `0xA0000..0xBFFFF` reads
and writes through the VGA planar memory path.  EGA/VGA modes `0Dh..13h` still
use the VGA path.

This targets Sopwith's corrupted top HUD rows: mode `04h` CGA writes to
`B800:0000` should no longer be affected by stale VGA register state.

```text
pico_286 size: 1007400 bytes
pico_286 SHA256: 7E91BA0398575E9BE6B46F1C0AFC20F1F683B6FCD82FF3C113EF8C168B301709
Defender scan: found no threats
```

## 2026-05-30 Fn shortcut help overlay

`Fn` + D-pad `Left` now toggles an on-screen help overlay that lists the active
Fn shortcuts and what each one does.  The panel hides automatically before
opening the on-screen keyboard, disk menu, key preset editor, or soft-resetting
the emulated PC.

```text
pico_286 size: 1005252 bytes
pico_286 SHA256: 70B2BF9661790A0D69966416C862B09D13386641B3D86441134201E737D16C12
```

## 2026-05-30 screenshot feedback

`Fn` + D-pad `Up` still saves the configured PNG/BMP screenshot, and now also
mixes a short camera-shutter click into the normal `driver.so` audio stream.
After the frame is saved, Pico-286 shows a half-second `SCREENSHOT SAVED` or
`SCREENSHOT FAILED` overlay with a 160x120 preview.

```text
pico_286 size: 1001852 bytes
pico_286 SHA256: F3338BF89F34057B38C1DD8135544D93D664F56AE8FE5F7A4E255AE8A35E5743
```

## 2026-05-30 on-screen app statistics

The current `pico_286` binary supports a compact top-left runtime statistics
overlay.  When enabled in `pico_286.conf`, press `Fn` + D-pad `Down` to show
or hide:

- decoded x86 instruction loop count per second;
- host disk image read KB/s;
- host disk image write KB/s;
- presented FPS.

The config option is:

```ini
[stats]
app_stats_enabled=1
```

Set `app_stats_enabled=0` to disable the shortcut and overlay.

```text
pico_286 size: 993864 bytes
pico_286 SHA256: D35D17E92C3832F9EFFC88E3030E181A49F4667E6D06FD8E45FE928E6DBB1816
```

## 2026-05-30 computed-goto opcode dispatch

The current `pico_286` binary is built with `R36SX_CPU_COMPUTED_GOTO=1`.
The main `exec86()` opcode decoder uses a GNU labels-as-values dispatch table
before the original switch body, so the normal hot path jumps directly to the
opcode handler label.  The old switch decoder is still available by rebuilding
with:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -DisableComputedGoto
```

```text
pico_286 size: 982416 bytes
pico_286 SHA256: 880BF4A3BC3387A22F0D034AC644F238D540A9AE71409750D4423BC337CBE914
```

## 2026-05-30 grouped configuration sections

The current `pico_286.conf` template and the runtime config writer group every
option under an INI section: `[cpu]`, `[boot]`, `[screenshot]`,
`[disk_cache]`, `[profiling]`, `[floppy_drives]`, and `[hard_drives]`.

## 2026-05-30 configurable screenshot format

The current `pico_286` binary reads `screenshot_format` from `pico_286.conf`.
Supported values are `png` and `bmp`; `png` is the default.

## 2026-05-30 PNG screenshots

The current `pico_286` binary saves `Fn` + D-pad `Up` screenshots as compressed
PNG files under `MIPS_NATIVE/pico_286/screenshots`:

```text
pico_286_YYYYMMDD_HHMMSS_NNN.png
```

The writer uses zlib directly.  The current build links the target
`libz.so.1.2.11` explicitly because the SDK sysroot does not provide an
unversioned `libz.so` linker symlink.  The original disk image already ships
compatible `libz.so.1` libraries.

## 2026-05-30 direct-present video path

The current `pico_286` binary presents normal DOS frames directly from the
emulator `SCREEN` buffer to `driver.so`.  Full-screen menus later changed to
draw directly into the output frame without precomposing a DOS frame underneath
them.  Small overlays later changed to use a stable output frame, so they do
not leave pixels inside the emulator framebuffer.

## 2026-05-30 runtime profiling option

The current binary includes optional profiling support controlled by
`pico_286.conf`:

```ini
[profiling]
profiling_enabled=0
profiling_log_ms=5000
```

Set `profiling_enabled=1` to write periodic performance summaries to
`pico_286.log`.  Profiling output does not require a `DEBUG=1` build.  To
compile the profiling code out entirely, build with
`build_pico_286.ps1 -DisableProfiling`.

## 2026-05-30 native RGB565 video buffer

The current `pico_286` binary removes the 32-bit intermediate video frame from
the active R36SX Pico-286 path:

- `r36sx_linux-main.cpp` renders DOS output into a 16-bit RGB565
  `SCREEN[640 * 480]`.
- The renderer keeps RGB565 shadow palettes for CGA/TGA/VGA output.
- `r36sx_minifb.c` copies RGB565 rows directly to its cached base frame.
- When the on-screen keyboard is visible, the vertical compression blends
  RGB565 pixels directly.

This removes the previous full-frame RGB888-to-RGB565 conversion before each
new Pico-286 frame is presented through `driver.so`.

```text
pico_286 size: 917496 bytes
pico_286 SHA256: BED9F7092FD1E03A169EB34A40BD1964FC6F9ED403F5E1DDADF67F0E0E2D6EDB
Defender scan: found no threats
```

## 2026-05-29 disk menu boot order row

The current `pico_286` binary adds a `BOOT ORDER` row to the disk image menu.
Use it to switch between:

- `A,C`: try the floppy drive before the hard disk.
- `C,A`: try the hard disk before the floppy drive.

Left/Right or A/Y cycles the row.  `SAVE/APPLY` writes the selected order to
`pico_286.conf` as `boot_order=fdd0,hdd0` or `boot_order=hdd0,fdd0` and then
applies the current disk bindings.

```text
pico_286 size: 915372 bytes
pico_286 SHA256: F9CBF5F875428A33E568E07110E814597D13177C69C8CBA07099D0438D58D429
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard D-pad repeat

The current `pico_286` binary adds auto-repeat for D-pad navigation in the
shared on-screen keyboard.  Holding Left/Right/Up/Down moves the highlighted
key once immediately, waits about 280 ms, then repeats about every 85 ms.  This
works both in the normal DOS on-screen keyboard and in the key-preset picker.

Only navigation repeats; A/Start, B, X, Y, and Select remain one-shot actions.

```text
pico_286 size: 912504 bytes
pico_286 SHA256: 5AB34A3F4B7C9DE260E990228549C66B55629D6039D0CF1E1C567B59153E8810
Defender scan: found no threats
```

## 2026-05-29 Fn+B soft reset

The current `pico_286` binary adds a native soft-reset shortcut:

- `Fn+B`: reset the emulated PC without exiting Pico-286.

The reset path releases held virtual keys, closes the on-screen keyboard and
settings overlays, then resets the emulated CPU, RAM/video state, keyboard
queue, PIC, PIT, PC speaker/Covox, Sound Blaster, SN76489, OPL, and the host
audio buffer.

```text
pico_286 size: 910824 bytes
pico_286 SHA256: BAE91FC64A41F8D4AC86E9DCBB89220D0D99695967643CE65EAB590B36B39F21
Defender scan: found no threats
```

## 2026-05-29 REP MOVS/STOS batching

The current `pico_286` binary batches common REP string move/store
instructions in the R36SX CPU core:

- `REP MOVSB`
- `REP MOVSW`
- `REP STOSB`
- `REP STOSW`

Each decoded REP instruction can now process up to 1024 byte/word elements
before returning to the main CPU loop, instead of re-decoding the REP prefix
and opcode for every element.  The implementation still uses the existing
memory handlers, so RAM, VRAM, and mapped memory behavior is preserved.  The
Trap Flag path falls back to one element at a time.

Expected effect:

- faster DOS screen clears and memory copies,
- faster installer/file-manager style workloads,
- less CPU time spent decoding repeated string instructions.

```text
pico_286 size: 908356 bytes
pico_286 SHA256: 19C64013150F4A659BE965DCD8D12045E00CC30F7C3639E6FBDE33FD46137DF7
Defender scan: found no threats
```

## 2026-05-29 ticks thread 1 ms sleep

The current `pico_286` binary increases `R36SX_TICKS_THREAD_SLEEP_US` from
`250u` to `1000u`.  The ticks thread still catches up timer, audio, Sound
Blaster, Disney Sound Source, cursor blink, and video frame events in batches,
but it now wakes the Linux scheduler about 1000 times per second instead of up
to 4000 times per second.

Expected effect:

- lower scheduler overhead,
- audio batching of about 44 output samples per 1 ms wakeup,
- no intended guest-visible timing change because missed ticks are still
  generated in catch-up loops.

```text
pico_286 size: 902332 bytes
pico_286 SHA256: CF3AD6C1C68D5640F9C4EE2C59C62ECF85A1DB39001EFD7E005C4D596BDE89F0
Defender scan: found no threats
```

## 2026-05-29 HLT idle wake-on-IRQ

The current `pico_286` binary implements real CPU halt behavior for opcode
`F4` / `HLT`.  The CPU core now sets a halt state and returns from `exec86()`
instead of continuing to burn the rest of the host execution quantum.  Later
`exec86()` calls keep the guest halted until an unmasked PIC interrupt is
pending and the guest interrupt flag is enabled.  The interrupt then wakes the
CPU and is serviced through the existing `nextintr()` / `intcall86()` path.

This is intended to reduce host CPU use while DOS, BIOS, installers, and menus
are idle and waiting for timer or keyboard interrupts.

```text
pico_286 size: 902332 bytes
pico_286 SHA256: 3BE291DAC81BE5BA61F8FF7824C1870E8F301E9EFE07DA203F118069F8F2C88B
Defender scan: found no threats
```

## 2026-05-29 RGB565 video present cache

The current `pico_286` binary reduces video work in the R36SX MiniFB layer.
The DOS renderer marks completed frames with a generation counter, and
`mfb_update()` keeps a converted RGB565 base frame.  It now rebuilds that base
only when a new DOS frame is ready or when the on-screen keyboard changes the
layout.  Menus, the keyboard, and the disk LED are drawn on a separate overlay
frame copied from the cached base image.

If no overlay is active and no new DOS frame is ready, the binary skips both
the RGB888-to-RGB565 conversion and the `driver.so` frame submit.  The
no-keyboard path also converts only the active DOS video height and clears the
remaining bottom area as black.

```text
pico_286 size: 899216 bytes
pico_286 SHA256: F4D7FF18F098A744C640FC8B63395D05C6E78453FF7E22E43901153597985BF7
Defender scan: found no threats
```

## 2026-05-29 Sopwith CGA mode 04h renderer fix

`SW.EXE` from `sopwith.img` normally selects BIOS video mode `04h`, the CGA
320x200 4-color graphics mode.  The game uses a generic `int 10h AH=00h`
wrapper and passes either `04h` or `06h`; the bundled no-argument launch path
keeps its selector flag zero, so it reaches `04h`.

The current `pico_286` binary fixes CGA modes `04h`, `05h`, and `06h` in the
R36SX renderer.  These modes now read the low byte from each logical
`uint32_t VIDEORAM[]` slot instead of treating the host storage as a packed
`uint8_t *`, which scrambled CGA images such as Sopwith.

```text
pico_286 size: 896604 bytes
pico_286 SHA256: D1D5787DC9C83E8BBC1EC825B0AB623F0E6F5EB98801365DF8892EA4A187F874
Defender scan: found no threats
```

## 2026-05-29 EGA mode 0Fh monochrome renderer

The current `pico_286` binary adds BIOS video mode `0Fh`, the standard
EGA/VGA 640x350 monochrome graphics mode.  The renderer treats a pixel as
foreground if the corresponding bit is set in any packed EGA/VGA plane, which
keeps monochrome output visible across different plane-write paths.

```text
pico_286 size: 896284 bytes
pico_286 SHA256: C9280E6B8A3F8838D2C050AAE91C04D39E1784BFFBAE2DAAAC403088E7038600
Defender scan: found no threats
```

## 2026-05-29 MDA mode 07h text renderer

The current `pico_286` binary fixes BIOS video mode `07h`.  It now renders as
MDA-compatible 80x25 monochrome text instead of falling through to the Hercules
bitplane graphics renderer.  The real MDA layout is 720 pixels wide with
9-pixel character cells; this build uses 8x14 cells so 80 columns fit exactly
into the 640-pixel R36SX framebuffer.

The BIOS text helper also uses the MDA `B000:` text memory base while mode
`07h` is active.

```text
pico_286 size: 894948 bytes
pico_286 SHA256: 230E13863C3760CF975602A5AA774B8269F48E85F81D1E4FD8C32EA2E5A2DD19
Defender scan: found no threats
```

## 2026-05-29 active-height keyboard compression

The current `pico_286` binary compresses only the active DOS video area when
the on-screen keyboard is visible.  Normal 80x25 text mode draws into the top
400 rows of the 640x480 framebuffer, so the keyboard view now scales those
400 rows into the 384-pixel area above the keyboard instead of scaling the
extra black margin below the text.

```text
pico_286 size: 891968 bytes
pico_286 SHA256: 78498E29EE66FC29570D30CABA35699EAC46FC053DA5D1D981821FAC00589A94
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard direct B/X keys

The current `pico_286` binary keeps physical B and X as direct keyboard keys
whenever the on-screen keyboard is visible:

- B sends Backspace.
- X sends Escape.

This also applies to the preset editor's picker keyboard.  B no longer cancels
that picker; in name editing it deletes one character, and in key-binding mode
it assigns Backspace.

```text
pico_286 size: 891276 bytes
pico_286 SHA256: 366C47D675782BAD84EC4F353DCF733CA79BB525F716FEE6A3369CE93396A28C
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard compact edge padding

The current `pico_286` binary corrects the previous edge-to-edge keyboard
layout.  The panel remains flush with the screen edges, the header/status text
is restored, and the key rows leave only a one-pixel inner gap from the panel
border on the left, right, and bottom edges.

```text
pico_286 size: 891076 bytes
pico_286 SHA256: 521FD0C4E9F1D518A79CEA6F72FD1AC4C4FE21362A3646C737F68466983C51F2
Defender scan: found no threats
```

## 2026-05-29 remaining FreeDOS package floppies

The patch now includes the full official FreeDOS 1.4 Floppy Edition 1.44 MB
image set:

- `FreeDOS1.img`: `144m/x86BOOT.img`
- `FreeDOS2.img`: `144m/x86DSK01.img`
- `FreeDOS3.img`: `144m/x86DSK02.img`
- `FreeDOS4.img`: `144m/x86DSK03.img`
- `FreeDOS5.img`: `144m/x86DSK04.img`
- `FreeDOS6.img`: `144m/x86DSK05.img`
- `FreeDOS7.img`: `144m/x86DSK06.img`

Use the Pico-286 disk image menu (`Fn+Select`) to mount the later package
diskettes when the installer asks for them.  The downloaded archive, extracted
tree, final `disk_image` copies, consolidated patch copies, and
`disk_image_patch_078` were scanned with `tools/scan-download.ps1`; Microsoft
Defender reported no threats.

## 2026-05-29 held on-screen-keyboard press animation

The current `pico_286` binary adds a held press animation to the shared
R36SX on-screen keyboard.  A pressed virtual key shifts down-right and darkens
until the physical button that activated it is released.

Direct shortcuts animate their matching visible keys:

- `B`: `BS`
- `X`: `ESC`
- `Y`: `ENT`
- `A` or `Start`: the highlighted virtual key

```text
pico_286 size: 917844 bytes
pico_286 SHA256: 570E55DE4A0357D392A30C418B6152F88DD4A70F76B137F24DB997E9935699A1
Defender scan: found no threats
```

## 2026-05-30 Select preset row default

The current `pico_286` binary makes physical `Select` a normal remappable key
preset entry.  The preset editor now shows `SELECT` in the left column bottom
slot opposite `START` instead of drawing the old `X/B CANCEL` label over that
slot.  The default preset maps `Select` to `Shift`.

Older `keypresets.conf` presets that do not contain `select=...` are filled
from current defaults before explicit values are applied, so they receive the
new `Select = Shift` binding.  An explicit `select=NONE` or another value still
overrides the default.

```text
pico_286 size: 981136 bytes
pico_286 SHA256: 980E3291F2227EE9109551C83DB4D6679FBC3FFB665D80C4BCA3CDBD86FB5AD3
```

```text
pico_286 size: 914632 bytes
pico_286 SHA256: 04E15547B6DEB47BB9CCC781572B0070F672174B6E9AA099AC76BA96A8CE561E
Defender scan: found no threats
```

## 2026-05-29 preset-only cursor-key block

The current `pico_286` binary hides the right-side on-screen cursor-key block
during normal DOS use.  The key picker inside the preset editor enables that
block explicitly, so cursor keys remain available while assigning physical
buttons.

The default `pico_286.conf` no longer contains `osk_cursor_keys`, and the disk
config save path no longer writes that legacy setting.  Existing configs may
still contain the old key; it is accepted for compatibility but has no effect.

```text
pico_286 size: 914080 bytes
pico_286 SHA256: 5B1D9D8887C512FFF48C125B084CFEA6B05CC9F4F7E481E0D65D2804EEB07E7F
Defender scan: found no threats
```

## 2026-05-29 edge-to-edge on-screen keyboard panel

The current `pico_286` binary updates the shared R36SX on-screen keyboard
layout.  The panel touches the left, right, and bottom screen borders, keeps
the header/status text, and leaves only a one-pixel inner gap between the
buttons and the panel border on the left, right, and bottom edges.

```text
pico_286 size: 921312 bytes
pico_286 SHA256: F818B656F70CD88212B16B4EC1B5F460489A9573966FC8102AD187336FC3C3C9
Defender scan: found no threats
```

## 2026-05-29 remove PSRAM payload from Linux executable

The current `pico_286` binary no longer stores the Pico/ESP-style `.psram`
section inside the Linux/MIPS executable.  The emulated RAM/EMS/XMS arrays are
still allocated at runtime as zero-initialized storage, but they no longer add
about 7 MB of zeros to the file on the SD card.

```text
pico_286 size: 922472 bytes
pico_286 SHA256: 68CCFDD09C2230CD862A5BBEABD88277F9BCA10087F5F2F0F9F12B0217CA5913
Defender scan: found no threats
```

## 2026-05-29 executable README

Adds `MIPS_NATIVE/pico_286/README.md`, a short user-facing control guide for
the native `pico_286` executable.  It documents main DOS controls, Fn
shortcuts, the on-screen keyboard, the disk image menu, and the key preset
editor.

## 2026-05-29 settings menu button defaults

The current `pico_286` binary normalizes settings-menu buttons:

- `A` or `Y`: accept/activate the selected row, cycle a disk image, or accept
  the highlighted on-screen key in picker mode,
- `B` or `X`: cancel the disk menu, key preset editor, or picker mode.

This affects the disk image binding menu, the key preset editor, and the
on-screen keyboard picker used by preset binding/name editing.  The normal DOS
on-screen keyboard keeps its existing direct shortcuts.

```text
pico_286 size: 8114624 bytes
pico_286 SHA256: 0CC16E9BBD854D14EAF7DC5AF7D8E684383F4087A3B2EDE8E14A4E0AB27D99F5
Defender scan: found no threats
```

## 2026-05-29 preset rename cursor

The current `pico_286` binary improves the key preset editor.  When editing a
preset name, the `RENAME PRESET:` line shows a blinking underscore cursor next
to the current text.

```text
pico_286 size: 8114908 bytes
pico_286 SHA256: C98A6ABE633D9D22AC9E153D695EE6B059DE42065E753B1F651FCF07D6942485
Defender scan: found no threats
```

## 2026-05-29 Fn+X and long-Fn exit

The current `pico_286` binary removes the Select+Start application exit.

Current exit controls:

- Fn+X: exit back to TinyMC,
- hold Fn for more than 3 seconds: emergency exit back to TinyMC.

Other Fn controls remain:

- tap Fn: toggle the on-screen keyboard,
- Fn+Select: open the disk image binding menu,
- Fn+Start: open the key preset editor.

```text
pico_286 size: 8114488 bytes
pico_286 SHA256: 6DD9DC4A8528C38EE909FDF9CF2F13A6E42890CBBF61AAAC89FEEA98DBC809AD
Defender scan: found no threats
```

## 2026-05-29 Fn chord disk menu

The current `pico_286` binary changes Fn handling:

- tap Fn: toggle the on-screen keyboard,
- Fn+Select: open the disk image binding menu,
- Fn+Start: open the key preset editor.

The disk menu scans `.img` files next to `pico_286.conf`, lets the user bind
images to `FDD0`, `FDD1`, `HDD0`, and `HDD1`, and includes `SAVE/APPLY`,
`EXIT APP`, and `CANCEL` rows.  `SAVE/APPLY` writes `pico_286.conf` and calls
`insertdisk()` for the running emulator.  The `EXIT APP` row is useful after
hard-disk changes because DOS can cache drive state.

```text
pico_286 size: 8113476 bytes
pico_286 SHA256: FC94F00C0F93F611F6C52CDEA587C3DF8A7F685594ED82A773B8D3376E0FFFCD
Defender scan: found no threats
```

## 2026-05-29 optional on-screen cursor-key block

The current `pico_286` binary includes an optional right-side cursor-key block
in the on-screen keyboard.  It is enabled by `pico_286.conf`:

```ini
osk_cursor_keys=on
```

The cursor block uses an inverted-T physical keyboard layout: Up above
Left/Down/Right.  The main on-screen keys are narrowed while the block is
enabled so the keyboard still fits in the existing bottom panel.

```text
pico_286 size: 8076964 bytes
pico_286 SHA256: F8FA17D225EBF84BAE8AE5D1D7364FE1AD11B3D5D36002BE1DD8D9C67C6DFC4F
Defender scan: found no threats
```

## 2026-05-29 boot order and HDD geometry config

`pico_286.conf` now includes:

```ini
boot_order=fdd0,hdd0
hdd0_geometry=65,16,63
hdd1_geometry=65,16,63
```

`boot_order` accepts `fdd0`, `fdd1`, `hdd0`, and `hdd1`; `boot_order=rom`
keeps the embedded ROM BIOS boot path.  The R36SX `INT 19h` hook probes boot
sectors in the configured order, checks for the `55 AA` signature, loads the
selected sector to `0000:7C00`, and sets `DL` to the selected BIOS drive.

`hdd0_geometry` and `hdd1_geometry` are CHS overrides in
`cylinders,heads,sectors` order.  The bundled hard disk images match
`65,16,63`.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8037084 bytes
SHA256: DB5654656F9C97D19557E63E38E46D8A5E4A604916C4F52092670C5698088B58
Defender scan: found no threats
```

## 2026-05-29 BIOS boot prompt mode

`pico_286.conf` now includes:

```ini
boot_mode=normal
```

`normal` attaches the configured disks during BIOS `INT 19h` and boots DOS.
`bios_prompt` leaves disks detached at `INT 19h`, so the embedded Turbo XT BIOS
stops at its boot prompt.  The ROM contains boot/diagnostic text, but no full
interactive CMOS/BIOS setup utility.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8016928 bytes
SHA256: 4AF76508FCF896130572E53E0D4B1A6E3BF5060FAB02817888CDFF17FE4194AD
Defender scan: found no threats
```

## 2026-05-29 configurable CPU speed

`pico_286.conf` now includes:

```ini
cpu_mhz=32.768
```

This controls the R36SX host execution quantum passed to `exec86()`.
`32.768` preserves the previous hard-coded behavior (`exec86(32768)` per
millisecond-like host loop).  The setting is a practical speed knob for this
port, not a cycle-exact 80286 clock.

The rebuilt binary was copied into this patch and scanned with
`tools/scan-download.ps1`; Microsoft Defender reported no threats.

```text
Size: 8010412 bytes
SHA256: FE190E432D63957A91728F0E9BCEC7895BF5ECE7FC3D99291D0211E10BD84B43
Defender scan: found no threats
```

Optional PC disk images should be placed in the same directory:

- `MIPS_NATIVE/pico_286/pico_286.conf`
- `MIPS_NATIVE/pico_286/FreeDOS1.img`
- `MIPS_NATIVE/pico_286/FreeDOS2.img`
- `MIPS_NATIVE/pico_286/FreeDOS3.img`
- `MIPS_NATIVE/pico_286/FreeDOS4.img`
- `MIPS_NATIVE/pico_286/FreeDOS5.img`
- `MIPS_NATIVE/pico_286/FreeDOS6.img`
- `MIPS_NATIVE/pico_286/FreeDOS7.img`
- `MIPS_NATIVE/pico_286/sopwith.img`
- `MIPS_NATIVE/pico_286/hdd.img`
- `MIPS_NATIVE/pico_286/hdd2.img`

Button presets are stored in:

- `MIPS_NATIVE/pico_286/keypresets.conf`

The default preset matches the original direct DOS mapping:

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

When DOS is running and the on-screen keyboard is hidden, Select opens or
closes the full-screen preset editor.  D-pad chooses a row.  The current editor
uses a two-column physical button list, opens the shared on-screen keyboard as
a picker for key assignments and preset renames, and saves only through the
visual `OK` button.  `Cancel` or Select closes without saving the draft.

The included image set is based on official FreeDOS 1.4 Floppy Edition:

- `FreeDOS1.img`: `144m/x86BOOT.img`, bootable FreeDOS floppy.
- `FreeDOS2.img`: `144m/x86DSK01.img`.
- `FreeDOS3.img`: `144m/x86DSK02.img`.
- `FreeDOS4.img`: `144m/x86DSK03.img`.
- `FreeDOS5.img`: `144m/x86DSK04.img`.
- `FreeDOS6.img`: `144m/x86DSK05.img`.
- `FreeDOS7.img`: `144m/x86DSK06.img`.
- `hdd.img` and `hdd2.img`: 33,546,240-byte hard disk images with MBR and one
  FAT16 primary partition, 65/16/63 CHS.

The downloaded FreeDOS archive, extracted files, and final copied image
directories were scanned with `tools/scan-download.ps1`; Microsoft Defender
reported no threats.

The current patch also includes a separate Sopwith (The Author's Edition)
game floppy:

- `sopwith.img`: 1.44 MB FAT12 floppy with `SW.EXE`, `SOPWTH.BAT`, and
  `README.TXT`.

`pico_286.conf` currently maps DOS `B:` / BIOS drive `01h` to this game
floppy:

```ini
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd1=hdd2.img
```

Sopwith was downloaded from DOS Games Archive (`swae-box.zip`, SHA256
`689BC2B80BFE9A7079F94C7A550BA2E0D7FA1842DC4F9C6BCED225A7424BAFBA`) and
packed with `tools/create_fat12_floppy.py`.  The ZIP, extracted files, and
all three final image copies were scanned with `tools/scan-download.ps1`;
Microsoft Defender reported no threats.

The current binary is an unstripped ELF32 little-endian MIPS executable.  Zig
0.16.0 `objcopy --strip-all` reports `error: unimplemented` for this file, so
the build keeps symbols for now.

This patch was rebuilt after the first device log showed:

- `main: pthread_create sound=0 ticks=0`
- `main: fatal signal 11`

The new binary adds first-iteration logs for `sound_thread`, `ticks_thread`,
`exec86()`, and `mfb_update()`.  It also avoids the upstream audio cast from
`int16_t *` to `int32_t *` in `get_sound_sample()`.

Follow-up fix: the first diagnostic run reached `main: before exec loop=0` and
then crashed.  The Linux entrypoint had not initialized `read86`, `write86`,
`readw86`, `writew86`, `readdw86`, or `writedw86`; the new binary initializes
them to the same `*_ob` memory backend used by upstream `win32-main.cpp`.

Current build also includes an on-screen disk activity indicator: a small
blinking red circle appears in the lower-right corner when Pico-286 reads from
or writes to disk image sectors.

It also maps BIOS video modes `0x20` and `0x30` to the 80x25 text renderer so
FreeDOS/BIOS boot text can be shown instead of `unsupported video mode 20/30`.

The current build adds the missing text-mode BIOS `INT 10h` services needed by
BIOS/DOS boot screens, including cursor shape/position, scroll/clear window,
read/write character and attribute, teletype output, mode query, active page
selection, and write string.  It also fixes the normal text-mode renderer to
read from logical `VIDEORAM` cells so text printed by those BIOS calls can be
shown on screen.

The on-screen debug overlay has been disabled.  Diagnostics still go to
`MIPS_NATIVE/pico_286/pico_286.log`, but `DEBUG_VRAM` is no longer drawn into
the bottom of the framebuffer, and the emulated screen text is no longer dumped
to the log as `screen_text:*` entries.

Device testing confirmed that this patch set boots DOS successfully from the
included `FreeDOS1.img` floppy image.

The current binary includes a joystick-controlled on-screen keyboard and a key
preset editor:

- Fn toggles the keyboard.
- Select opens the key preset editor when the keyboard is hidden.
- D-pad moves the highlighted key.
- A or Start types the highlighted key.
- B sends Backspace immediately.
- Select closes the keyboard.
- X sends Escape immediately.
- Y sends Enter immediately.
- Select+Start still exits back to TinyMC.

F1-F10, Delete, common DOS punctuation, and latched Shift/Ctrl/Alt modifiers
are available from the on-screen keyboard.  Shift is available as the on-screen
`SHF` key.

The keyboard uses a compact bottom layout: 96 pixels tall, about 20% of the
640x480 framebuffer.  While it is visible, the DOS screen is vertically
compressed with a halftone-style area filter into the remaining 384 pixels
above the keyboard instead of being covered.

Disk image bindings are configurable through `MIPS_NATIVE/pico_286/pico_286.conf`:

```ini
cpu_mhz=32.768
boot_mode=normal
boot_order=fdd0,hdd0
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd0_geometry=65,16,63
hdd1=hdd2.img
hdd1_geometry=65,16,63
```

`cpu_mhz` controls the R36SX execution quantum passed to `exec86()`;
`32.768` keeps the previous hard-coded speed (`exec86(32768)` per
millisecond-like host loop).  It is a practical speed knob, not a cycle-exact
80286 timing model.

`boot_mode=normal` attaches the configured disks during BIOS `INT 19h` and
boots DOS.  `boot_mode=bios_prompt` leaves disks detached at `INT 19h`, so the
embedded Turbo XT BIOS stops at its boot prompt.  The ROM does not include a
full interactive CMOS/BIOS setup utility.

`boot_order` selects the boot sector probe order used by the R36SX `INT 19h`
hook.  Supported names are `fdd0`, `fdd1`, `hdd0`, and `hdd1`; use
`boot_order=rom` to chain to the embedded ROM BIOS boot path instead.

These map to BIOS drives `00h`, `01h`, `80h`, and `81h` respectively.  Paths
are relative to the Pico-286 directory unless absolute paths are used.  Empty
values disable a drive, and a missing config file falls back to the same four
default filenames.

`hdd0_geometry` and `hdd1_geometry` are optional CHS overrides in
`cylinders,heads,sectors` order.  The bundled hard disk images are
33,546,240 bytes, matching `65,16,63`.

## 2026-05-29 rebuild

The binary was rebuilt after moving the R36SX Pico-286 source patches out of
`build_pico_286.ps1` into explicit source files under
`homebrew/pico_286/r36sx_port/`.  Runtime behavior should be unchanged; the
build script now compiles those files directly instead of generating patched
copies in `obj/`.

```text
Size: 7936004 bytes
SHA256: CB9551333236CBEDEC8A173E212459D95B055A6FABAB6BC9DEC74BC1EF3A59DA
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard timing fix

The binary was rebuilt so the joystick-controlled on-screen keyboard sends a
PC-style make code first, holds it for about 80 ms, then sends the break code on
a later input poll.  This avoids losing key presses when DOS reads the emulated
keyboard controller after the old immediate key-down/key-up pair.

```text
Size: 7938128 bytes
SHA256: 51D01BF7DC78EC2C38E9428C7C42FDE3E0FDDAF42538D3984C8E488001B79E99
Defender scan: found no threats
```

## 2026-05-29 keyboard controller FIFO

The binary was rebuilt again to replace the temporary on-screen keyboard
delayed-release workaround with a proper FIFO behind emulated ports `0x60` and
`0x64`.  Key make/break scancodes are now queued, and reads from port `0x60`
pop one byte at a time while raising another IRQ1 if more bytes remain.

```text
Size: 7940088 bytes
SHA256: AB360DAE43112F5A4B72286E9BBE968C172A1F1241DBC86591ED0D6F5725681A
Defender scan: found no threats
```

## 2026-05-29 keyboard FIFO size reduction

The emulated keyboard controller FIFO was reduced from 32 scancode bytes to 8.
This is enough for the current joystick/on-screen keyboard path while keeping
the model smaller.

```text
Size: 7940104 bytes
SHA256: 4EB23B444826C36A60B734F4F66FEB58676DED6FF0FD9676FBE7D77F4EAD01B9
Defender scan: found no threats
```

## 2026-05-29 keyboard scancode pacing

The emulated keyboard controller now waits 1 ms before making each queued
scancode byte available through port `0x60`.  This approximates PS/2 keyboard
wire timing: one device-to-host byte is an 11-bit frame, and a 10-16.7 kHz
clock gives about 660-1100 us per scancode byte.

```text
Size: 7942728 bytes
SHA256: 7DF7757CB0A685ACF8BB5C09F9EAC292D6AD737AA0342CF67DB93F208144F9D3
Defender scan: found no threats
```

## 2026-05-29 reusable on-screen keyboard module

The Pico-286 binary was rebuilt after moving the joystick-controlled on-screen
keyboard implementation into the reusable shared module
`homebrew/common/r36sx_screen_keyboard.*`.  The device-facing keyboard behavior
should remain the same; Pico-286 now uses the shared module through a small
callback adapter.

```text
Size: 7948928 bytes
SHA256: 1C1BC7F76802DB1B2974704AB0B73E13473A5A335B83280BB73DE1A4C07E85B3
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard face-button remap

The Pico-286 binary was rebuilt after changing the shared on-screen keyboard
bindings.  While the keyboard is visible, `B` now sends Backspace immediately
and `Y` now sends Enter immediately.  `Select` closes the keyboard, and the
on-screen `CLOSE` key remains available.

```text
Size: 7949544 bytes
SHA256: B5A395D7A8CEDA18537659AC27339D950BD9F1E98C48061E6893073AD87BAE79
Defender scan: found no threats
```

## 2026-05-29 disk image config

Added `MIPS_NATIVE/pico_286/pico_286.conf` and rebuilt the binary so Pico-286
reads floppy and hard disk bindings from the config file before BIOS boot.

Default bindings:

```ini
fdd0=FreeDOS1.img
fdd1=sopwith.img
hdd0=hdd.img
hdd1=hdd2.img
```

These map to BIOS drives `00h`, `01h`, `80h`, and `81h`.  Relative paths are
resolved from the directory that contains `pico_286.conf`.  Empty values
disable a drive.  If the config file is missing, Pico-286 falls back to these
legacy internal names: `fdd0.img`, `fdd1.img`, `hdd.img`, and `hdd2.img`.

```text
Size: 7965128 bytes
SHA256: A63CD65C36BE12E56E59EFF677F5C298DE6BD701993DC6EBC0E004D5A1151623
Defender scan: found no threats
```

## 2026-05-29 on-screen keyboard X Escape binding

The Pico-286 binary was rebuilt after changing the shared on-screen keyboard
bindings.  While the keyboard is visible, the physical `X` button now sends
Escape immediately.  Shift remains available through the on-screen `SHF` key.

```text
Size: 7965676 bytes
SHA256: 91D85743E2A32C304407E8BFA1B1D92F732C2FA024F6E73CD4F9DE82C4B66048
Defender scan: found no threats
```

## 2026-05-29 key preset editor

Added `MIPS_NATIVE/pico_286/keypresets.conf` and rebuilt Pico-286 with a
full-screen key preset editor.  Select opens or closes it while DOS is running
and the on-screen keyboard is hidden.  The editor can switch presets, copy the
active preset into a new automatically named preset, and assign each game
button to a supported keyboard key.

```text
Size: 8005860 bytes
SHA256: 583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5
Defender scan: found no threats
```

## 2026-05-29 FreeDOS image rename

The FreeDOS floppy images were renamed:

- `fdd0.img` -> `FreeDOS1.img`
- `fdd1.img` -> `FreeDOS2.img`
- `fdd2.img` -> `FreeDOS3.img`

`pico_286.conf` now boots DOS from `FreeDOS1.img` and keeps `sopwith.img`
mounted as DOS `B:`.  The binary keeps the legacy missing-config fallback names
inside the app.

```text
Size: 8005860 bytes
SHA256: 583587D4DFA6A0CD7D817370DBA8CDB4397957513818F6D454C30D9DC1F56BC5
Defender scan: found no threats
```

## 2026-05-29 hard disk write flush fix

The disk layer was rebuilt so writes to `hdd.img` survive an in-emulator reboot
immediately after formatting DOS `C:`.

`hdd0=hdd.img` is mounted as BIOS drive `80h`, then normalized to internal disk
slot `2`.  Previously, `writedisk()` used `fwrite()` without `fflush()`, and
`ejectdisk()` did not close the old `FILE *` before `INT 19h` mounted the disk
again.  A freshly formatted `hdd.img` could therefore be reopened before the old
buffered writes reached the image.

The current binary flushes after sector writes, flushes/closes the image on
eject/remount, and fixes hard-disk/floppy counters to use normalized slots
`0`/`1` for floppies and `2`/`3` for hard disks.

```text
Size: 8006344 bytes
SHA256: 451570BAC02DBADDF73AC3A9B2EB6CF96A9A31C1E2943ADC766C8A6A455F03EC
Defender scan: found no threats
```

## 2026-05-29 BIOS hard disk registration

The current binary also reports inserted hard disks through BIOS-visible state
for DOS/FDISK:

- updates BIOS Data Area byte `0040:0075` with the fixed-disk count,
- answers `INT 13h AH=10` and `AH=11` for ready/recalibrate checks,
- answers `INT 13h AH=15` with hard-disk type `0x03` and `CX:DX` total sectors.

Pico-286 has no interactive BIOS setup screen.  The hard disk geometry is
derived from image size when `hdd.img` is inserted.  For the included
33,546,240-byte image the geometry is 65 cylinders, 16 heads, 63 sectors.

```text
Size: 8006884 bytes
SHA256: 0737E66B6DB7C0165F3A1B22BBF3200678CEA8E7BDC339A827DA56392234ECBF
Defender scan: found no threats
```

## 2026-05-29 key preset editor draft UI

The Pico-286 binary was rebuilt after reworking the Select key preset editor.
The binding list now uses two columns so all physical buttons stay inside the
outer frame.  Opening the editor starts a draft session: choosing another
preset, adding a preset, renaming it, or changing bindings does not touch
`keypresets.conf` until the visual `OK` button is selected.  `Cancel` and
`Select` close the editor without saving the draft.

The shared on-screen keyboard now has a picker mode.  Pico-286 uses it for
renaming presets and for choosing the key assigned to each physical button row.

```text
Size: 8054724 bytes
SHA256: 598071628DA6ECA5B608625CB636AEF76977BB5C59D50039B4E93BB37FC2E4E1
Defender scan: found no threats
```

## 2026-05-29 FAT16 hard disk images and disk read diagnostics

The previous `hdd.img` and `hdd2.img` files were blank raw images: sector 0 had
no MBR, no BPB, and no `55 AA` signature.  On the device this can surface as
DOS `DIR C:` reporting `Cannot read` / retry / abort because BIOS drive `80h`
exists but the backing image is not partitioned or formatted.

Both hard disk images are now generated on the host with
`tools/create_fat16_hdd.py`:

- CHS: `65,16,63`
- MBR primary partition: active FAT16 type `0x06`
- partition start: sector `63`
- partition size: `65457` sectors
- VBR: FAT16, `63` sectors per track, `16` heads, hidden sectors `63`
- root directory contains `README.TXT` so `DIR C:` should show at least one
  file immediately.

Pico-286 was also rebuilt with extra `INT 13h` diagnostics.  Failed reads,
writes, verifies, invalid CHS requests, out-of-range transfers, `fseek`,
`fread`, `fwrite`, and `fflush` failures are written to
`/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log` in debug builds.

```text
pico_286 size: 8058240 bytes
pico_286 SHA256: 43E4C5A227FE8E83F4A643F1546F6E9B51E79D509B78BA3E13C4B1C75CDC0332
hdd.img SHA256: 26953A16F571AB8452570E37E2C0688C0B60A2859E72B81D6E9EBA8D80379818
hdd2.img SHA256: 45A095789D0C5A4F8E0BB2717493874C761789AF199D602BF794DBAB12448C71
Defender scan: found no threats
```

## 2026-05-29 key preset editor physical layout

The current `pico_286` binary uses an explicit two-column layout for the key
preset editor, matching the console sides:

- left column: d-pad, `L`, `L2`, and static `SELECT CANCEL` at the bottom,
- right column: `X`, `Y`, `A`, `B`, `R`, `R2`, and `START` at the bottom.

Saved `keypresets.conf` keys are unchanged.  `Select` remains the service
button for opening/canceling the preset editor.

```text
pico_286 size: 8061088 bytes
pico_286 SHA256: 7770B1BD818DA27E1E4F7B924ACABC95B193509314341B24B6CC769F690D78C9
Defender scan: found no threats
```
