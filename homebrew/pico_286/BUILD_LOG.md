# pico-286 Build Log

## 2026-06-03 386 real-to-protected entry fix

Audited the Intel 80386 real-to-protected transition path against the Intel
80386 Programmer's Reference Manual and DPMI 0.9 entry rules while investigating
DOS/4GW hangs with only a blinking cursor.

Fixed two entry-path issues:

- Real-mode CS hidden-cache entries are now modeled as executable while CR0.PE
  has just been set and before the mandatory protected far transfer reloads CS.
  Intel documents that after setting PE the segment registers still point to the
  same linear addresses and CPL starts at zero.
- The DPMI real-to-protected entry now enables PE through `r36sx_cpu_set_cr0()`
  instead of directly setting the bit.  This switches the interpreter to the
  protected memory path before the entry stub returns through protected `RETF`
  and pops from a selector-based stack.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip
```

## 2026-06-03 strict 8086 address space and AT-memory gating

Audited the `cpu_model=8086` path against Intel 8086/8088 documentation and
AMD second-source 8086/8088 material.  The documented 8086 opcode set remains
enabled, while 80186/80286/80386 opcode gates continue to reject later
instructions in 8086 mode.

Fixed two capability leaks that were not plain opcodes:

- Real-mode address translation now enforces the original 8086/8088 20-bit
  physical address wrap.  `FFFF:0010` maps to physical `00000h`, and 16/32-bit
  accesses crossing `0FFFFFh` are split byte-wise so they wrap correctly.
- AT/286 memory services are hidden from `cpu_model=8086`: BIOS
  `INT 15h AH=87h/88h` now reports unsupported, and XMS installation checks
  through `INT 2Fh AX=4300h/4310h` no longer expose an XMS entry point.

Reference documents used for the audit:

- Intel, *The 8086 Family User's Manual*, October 1979.
- AMD, *Am8086/Am8088 16-Bit HMOS Microprocessors* data-sheet/manual material.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- size: `567528` bytes
- SHA256:
  `6158F7B471800C286BC5297C0D4FD509CB88D7F2038732760D8989B17A265DAE`

## 2026-06-03 strict documented 8086 opcode filtering

Audited the `cpu_model=8086` decoder against Intel's documented 8086/8088
instruction set and the later 80186 additions.  The generation gates were
already present for the main 80186+ opcodes (`PUSHA/POPA`, `BOUND`,
`PUSH imm`, immediate `IMUL`, `INS/OUTS`, `ENTER/LEAVE`, `C0/C1` shifts) and
for 80386 prefixes/opcodes (`FS/GS`, `66h/67h`, and the `0Fh` extended map).

Tightened invalid ModR/M forms that were still accepted:

- `8D mod=3` (`LEA` requires a memory addressing form).
- `8F /1..7` (`POP Ev` is only `/0`).
- `C4/C5 mod=3` (`LES/LDS` require a memory far pointer).
- `C6/C7 /1..7` (`MOV r/m,imm` is only `/0`).
- `D6h`, `F6/F7 /1` undocumented compatibility aliases.
- `FE /2..7` (`INC/DEC byte` are only `/0` and `/1`).
- `FF /7`, plus far `CALL/JMP` with register ModR/M operands.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- size: `567456` bytes
- SHA256:
  `A14013A8B176F11F20A428C8468E57EDDF938C12FEC674E57E83E4D1FA47165A`

## 2026-06-03 runtime x87 coprocessor switch

Added a runtime-configurable x87 math-coprocessor presence flag:

- `pico_286.conf` now writes `[cpu] x87_enabled=1` by default.
- The disk menu now has an `X87  ON/OFF` row.  Left/Right or A/Y toggles it.
- Saving an X87 change requests a soft reset, matching BIOS/CPU model changes,
  because most DOS software probes for a math coprocessor at startup.
- When X87 is disabled, `WAIT/FWAIT` behaves as no-op and ESC opcodes
  `D8h..DFh` only decode their ModR/M/displacement bytes without touching FPU
  state or memory.  This lets DOS probes see a machine without a coprocessor.
- Startup debug logging now includes `x87=on/off`.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- size: `567248` bytes
- SHA256:
  `B351EE887D9EC1D9090F47638FC21ECC54419B38C38D53F145A1B1EF9F595573`

## 2026-06-03 disk menu frequency row cleanup

Removed the old visual decrement/increment markers from the disk menu frequency
row. The row now shows only the current value, for example
`FREQUENCY  20MHZ`; Left/Right still decrease/increase `cpu_mhz` in 1 MHz
steps.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- size: `564128` bytes
- SHA256:
  `5528A16B21DF119C2C5F978CE0DCF97DDF2CE276BAD197C88A0A53ABC23B1C34`

## 2026-06-03 disk menu CPU and frequency controls

Added CPU controls to the Pico-286 disk menu.  The menu now includes:

- `CPU`: cycles between `8086`, `80286`, and `80386`.
- `FREQUENCY`: shows the current value, for example `20MHZ`, and edits
  `cpu_mhz` from 1 to 30 MHz in 1 MHz steps.

`OK` writes the selected values back to `pico_286.conf`.  CPU model changes
request a soft reset so opcode gating and BIOS startup use the new model.
Frequency-only changes update the runtime `exec86()` quantum without restarting
the app.  The config API now exposes setters for CPU model and whole-MHz
frequency, and the main loop refreshes the CPU timing cap when those values
change.

Rebuilt with:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Build succeeded.  Remaining warnings are the existing FPU/VGA/XMS/audio
warnings; the new disk-menu signed-compare warning was fixed before this build.

- `homebrew/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
- size: `564128` bytes
- SHA256:
  `5528A16B21DF119C2C5F978CE0DCF97DDF2CE276BAD197C88A0A53ABC23B1C34`

## 2026-06-03 real/protected interpreter memory split

Split the hot x86 interpreter memory model into separate real-mode and
protected-mode include files:

- `homebrew/pico_286/r36sx_port/r36sx_cpu_80386_real.inl`
- `homebrew/pico_286/r36sx_port/r36sx_cpu_80386_protected.inl`

`exec86()` is now a small router.  It calls the real-mode interpreter entry
while `CR0.PE=0`, and the protected-mode entry while PE is set.  The real-mode
path translates `segment:offset` directly as `segment << 4` and uses the
physical memory backend, avoiding descriptor-limit and paging checks for normal
DOS/BIOS execution.  The protected path keeps descriptor access checks, VM86
handling, and paging translation.  `r36sx_cpu_set_cr0()` updates the active
interpreter memory model immediately when guest code changes PE during an
instruction stream.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
```

Updated binaries:

- `homebrew/pico_286/pico_286`
- `disk_image/MIPS_NATIVE/pico_286/pico_286`
- `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`

Result:

- `pico_286` size: `562468` bytes
- `pico_286` SHA256:
  `E9D19700FDDE4EC583E9BAC76D64FCED92F2E8A0DA9AA0ED5633CC519D14ED45`
- WSL/GCC build succeeded.  The compiler still reports the existing warning
  set in FPU, XMS, renderer, audio/helper code, and the old dispatch `res8`
  maybe-uninitialized warning.

## 2026-06-03 screenshot shared object and static archive

Moved screenshot file encoding out of the main Pico-286 executable into one
common runtime shared object, then added the same encoder as a static archive
for projects that want to link it directly:

- `homebrew/common/r36sx_screenshot_module.h`
- `homebrew/common/r36sx_screenshot_module.c`
- `homebrew/common/build_screenshot_so_wsl.sh`
- `homebrew/common/build_screenshot_a_wsl.sh`
- `homebrew/common/screenshot.so`
- `homebrew/common/screenshot.a`

`r36sx_screenshot.c` keeps filename generation and directory fallback, then
loads `screenshot.so` lazily with `dlopen()` and passes the requested format to
`r36sx_screenshot_write_rgb565(path, pixels, width, height, format)`.  Format
`0` writes BMP and format `1` writes PNG.  BMP still has a small built-in
fallback if the module is missing.

Runtime lookup order:

1. `R36SX_SCREENSHOT_SO` environment variable.
2. `/mnt/sdcard/MIPS_NATIVE/common/screenshot.so`.
3. `./screenshot.so`.
4. `screenshot.so` through the dynamic loader path.

This removes zlib from the main `pico_286` ELF.  `readelf -d` now shows zlib
only on `screenshot.so`:

- `pico_286`: no `NEEDED libz.so.1`.
- `screenshot.so`: `NEEDED libz.so.1`, `NEEDED libc.so.6`.
- `screenshot.a`: exports `r36sx_screenshot_write_rgb565`; static users must
  link the target zlib when PNG output is enabled.

Rebuild commands:

```powershell
bash homebrew/common/build_screenshot_a_wsl.sh
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/shell/build_shell_wsl.sh --strip --install"
```

Patch copy commands:

```powershell
New-Item -ItemType Directory -Force .\patches\disk_image_patch_pico_286\MIPS_NATIVE\common
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\common\screenshot.so -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\common\screenshot.so -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\common\screenshot.so -Destination .\disk_image\MIPS_NATIVE\common\screenshot.so -Force
```

Result:

- `pico_286` size: `557824` bytes
- `pico_286` SHA256:
  `6D235F5CC1189B635C2DE6729318E91CE831E7836D01A1009672D8CDC83BEBC3`
- `screenshot.so` size: `6232` bytes
- `screenshot.so` SHA256:
  `FA4DC587DB3788B67AFAAFD53AE8B2C612D057BAEA0CC8C0F5DF097BC86D267E`
- `screenshot.a` size: `4178` bytes
- `screenshot.a` SHA256:
  `7A1D0128C816ED5681CB80E61F01BE1B21B7E62999B546BD5171D9CC1549D6CC`
- `shell` size: `58700` bytes
- `shell` SHA256:
  `D9E619E2C406334E9BAF4082FDCC00412686416D0F3D6BFEF684797F9B88435A`
- Pico and Shell WSL/GCC builds succeeded.  Pico still reports the existing
  warning set in FPU, XMS, renderer, and audio/helper code.

## 2026-06-03 shared screenshot helper

Moved RGB565 screenshot saving into the shared homebrew helper:

- `homebrew/common/r36sx_screenshot.h`
- `homebrew/common/r36sx_screenshot.c`

Pico-286 now uses that helper for screenshot directory fallback, timestamped
filenames, optional build-hash filename suffixes, BMP output, and PNG output.
The Pico-specific screenshot preview/toast UI remains in `r36sx_minifb.c`.
Shell now uses the same helper for BMP screenshots.

Rebuild commands:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/shell/build_shell_wsl.sh --strip --install"
```

Result:

- `pico_286` size: `558496` bytes
- `pico_286` SHA256:
  `D2325F9C5DDA123ABAE4050EB2F090EF530AE8AA1146A0E8883BA970E8E5A6CC`
- `shell` size: `58668` bytes
- `shell` SHA256:
  `EC78C89301F0070812D5BF2F70061A28DA55227D99A5EF82E178FA19193CFD59`
- Pico patch copy and Shell patch/install copies were updated with the same
  binaries.
- WSL/GCC builds succeeded.  Pico still reports the existing warning set in
  FPU, XMS, renderer, and audio/helper code.

## 2026-06-03 BIOS RTC date/time services

Fixed DOS-visible RTC time by replacing the old hardcoded BIOS `INT 1Ah`
date/time stubs with values from the emulated CMOS/RTC counter.

Implemented:

- `INT 1Ah AH=02h`: read RTC time as packed BCD `HH:MM:SS`.
- `INT 1Ah AH=03h`: set RTC time from packed BCD.
- `INT 1Ah AH=04h`: read RTC date as packed BCD `CC:YY:MM:DD`.
- `INT 1Ah AH=05h`: set RTC date from packed BCD.
- Shared RTC setter used by both BIOS calls and CMOS writes, so DOS and CMOS
  port access update the same emulated clock state.

Config updates:

- Source and patch `pico_286.conf` now use
  `rtc_start_time=2026-06-03 09:07:02`.
- Built-in fallback RTC start time is now `2026-06-03 00:00:00`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `557696` bytes
- `pico_286` SHA256:
  `4819842F9B274A1034EE3B82D15DC057EC62FA97752F89CB9001C49B7EF86F69`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-03 DPMI entry, vectors, and memory services

Extended the DPMI scaffold into a working real-to-protected entry path and added
the next group of `INT 31h` services checked against DPMI 1.0.

Implemented:

- `INT 2Fh AX=1687h`: installation check now reports the host and returns a
  magic far-call entry point.  The entry switches the client to protected mode,
  creates initial CS/SS/DS/ES selectors, converts the PSP environment pointer,
  and returns through the normal protected `RETF` path.
- Protected-mode `INT 31h` dispatch is now intercepted before IDT delivery, so
  DPMI calls work after the mode switch.
- `AX=000Dh/000Eh/000Fh`: allocate specific selector and get/set multiple raw
  descriptors.
- `AX=0200h..0205h` plus `AX=0210h..0213h`: real-mode vector get/set, protected
  interrupt vector get/set, and exception vector storage.
- `AX=0500h..0503h`: committed linear memory info, allocate, free, and resize
  blocks using a small first-fit allocator over the configured XMS-backed linear
  memory area.
- `AX=0604h`: page size query, currently `4096` bytes.

Still not a full DPMI 1.0 host: real-mode interrupt/procedure simulation
(`0300h..0302h`), real-mode callbacks (`0303h/0304h`), raw mode switch
addresses/state save-restore (`0305h/0306h`), DOS memory block services
(`0100h..0102h`), locking/page mapping APIs, and full protected interrupt
reflection remain in the protected-mode TODO.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `555024` bytes
- `pico_286` SHA256:
  `03FA997F3A6A56A7B746161AA468D3FFCDEBFB62CD7E8D2FF388628E4946DF09`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 DPMI raw descriptor services

Extended the DPMI descriptor scaffold with additional DPMI 1.0 selector
services:

- `AX=0002h`: map a real-mode segment to a descriptor.
- `AX=000Ah`: create a data alias descriptor.
- `AX=000Bh`: copy a descriptor into an 8-byte client buffer.
- `AX=000Ch`: copy an 8-byte client buffer into a DPMI descriptor.

The implementation converts between the emulator's descriptor cache and Intel's
raw descriptor layout, and keeps all descriptors in the internal DPMI LDT-style
pool added by the previous step.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `546792` bytes
- `pico_286` SHA256:
  `B9B575D8C929320ABBC28BDBF4F4FC8608042E617FB6F65FEC332FBC863DDE80`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 DPMI LDT descriptor services

Added the first DPMI LDT-style descriptor pool and wired it into the protected
CPU descriptor decoder.  This step follows the DPMI 1.0 LDT descriptor services
and keeps the real-mode `AX=1687h` installation check disabled until a real
mode-switch entry exists.

Implemented `INT 31h` functions:

- `AX=0000h`: allocate one or more LDT descriptors.
- `AX=0001h`: free an LDT descriptor.
- `AX=0006h`: get segment base address.
- `AX=0007h`: set segment base address.
- `AX=0008h`: set segment limit with 386 page-granularity validation.
- `AX=0009h`: set descriptor access rights with DPMI DPL/type checks.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `544768` bytes
- `pico_286` SHA256:
  `9F7A2F69385CCAA8F23F22909557F7FD97F30C0B2A3177FC4B056A9F3857374D`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 DPMI INT 31h version scaffold

Added the first protected-mode `INT 31h` services checked against the DPMI
Committee Version 1.0 specification:

- `AX=0003h`: get selector increment value.
- `AX=0400h`: get DPMI version, host flags, CPU type, and IRQ bases.
- `AX=0401h`: get capabilities and vendor information string.

The real-mode `INT 2Fh AX=1687h` installation check still reports no host
because the protected-mode entry point is not implemented yet.  This keeps DOS
extenders from attempting a DPMI switch before the entry code and client state
block exist.

Reference checkpoints used:

- DPMI 1.0 descriptor selector increment service.
- DPMI 1.0 version/capabilities service return registers.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `541420` bytes
- `pico_286` SHA256:
  `4E4CAFB77756F6231A8101DD461E0520276AF05FF11F48BDE0CA31B1D3C3E9E3`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 DPMI probe and dispatcher groundwork

Created the first DPMI host scaffold against the DPMI Committee Version 1.0
specification.  `INT 2Fh AX=1686h/1687h` is now routed through a named DPMI
multiplex helper, and `INT 31h` has a separate dispatcher stub.  The host still
reports not installed for `AX=1687h` because there is no protected-mode entry
point yet; the `INT 31h` stub returns the DPMI 1.0 unsupported-function error
shape (`CF=1`, `AX=8001h`) and is ready for service-by-service expansion.

Reference checkpoints used:

- DPMI 1.0 client initialization and `INT 2Fh AX=1687h` entry discovery.
- DPMI 1.0 Appendix B error reporting, including unsupported function
  `8001h`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `540304` bytes
- `pico_286` SHA256:
  `85628D655471201CE2D2843BE17FD8C0BDF349A2D8E77C5075EC3E7ACC87DB54`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 VM86 rejects protected descriptor instructions

Descriptor-management and selector-validation instructions now distinguish
native protected mode from virtual-8086 mode.  `ARPL`, `LAR`, `LSL`, and the
`0F 00` group (`SLDT`, `STR`, `LLDT`, `LTR`, `VERR`, `VERW`) now raise invalid
opcode in VM86 instead of treating VM86 as normal protected mode.  This matches
the Intel rule that these protected-mode instructions are not recognized in
real-address or virtual-8086 mode.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `540744` bytes
- `pico_286` SHA256:
  `CC195A27C9B704E53EE499EEB7B5CB805305A73CE8F14CDEEB7676DDE684795E`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 VM86 LOCK IOPL trap

The `LOCK` prefix is now checked during prefix decoding in virtual-8086 mode.
When a VM86 task executes `LOCK` with `IOPL < 3`, Pico-286 raises `#GP(0)` at
the prefix address instead of silently accepting it.  This completes the
currently implemented set of Intel 80386 VM86 IOPL-sensitive opcodes around
flags, software interrupts, interrupt returns, `CLI`/`STI`, and `LOCK`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `539872` bytes
- `pico_286` SHA256:
  `A98F25789BED6C46F896CB9C4C351A046C2206EE59AF6FBB91C4B1CD630A0AFD`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 VM86 IOPL-sensitive flag and interrupt opcodes

Virtual-8086 mode now treats `PUSHF`, `POPF`, `INT imm8`, `IRET`, and `IRETD`
as IOPL-sensitive instructions.  If a VM86 task runs with `IOPL < 3`, these
opcodes now raise `#GP(0)` at the faulting instruction so a protected-mode
monitor can emulate or reflect them.  `CLI` and `STI` already used the shared
IOPL check, while `LOCK` is covered by the following build entry.  `INT3` and
`INTO` remain on their existing interrupt path, matching the 80386 behavior
that only `INT n` is intercepted this way.

Reference checked while implementing this: Intel 80386 Programmer's Reference
Manual, section 15.4, "Additional Sensitive Instructions".

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `540704` bytes
- `pico_286` SHA256:
  `09948F41345D0E032277F3ED1BF43FADADBC0839FDD9AC012E61295AD6FD96CF`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 protected software INT gate checks

Software interrupt opcodes (`INT3`, `INT imm8`, and `INTO`) now enter the
protected interrupt path with a software-interrupt flag.  In protected mode the
IDT gate DPL is checked against CPL, and a denied software interrupt raises
`#GP` with an IDT-style error code.  Hardware interrupts and CPU exceptions keep
using the non-software path, so their delivery is not incorrectly blocked by
gate DPL.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `540268` bytes
- `pico_286` SHA256:
  `EEFB97563D7565BECDD7EC722BC7BA20356B0D26DF1EBA880A4C563054F6AC7C`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 protected interrupt VM86 frames

Protected-mode interrupt/trap gates now switch to an inner-privilege stack
from the current TSS when the target handler runs at a numerically lower CPL.
Interrupts delivered while the guest is in VM86 mode now build the 386 VM86
interrupt frame: `GS`, `FS`, `DS`, `ES`, outer `SS:ESP`, `EFLAGS`, `CS`, and
`EIP`, plus an optional error code.  The handler entry clears `VM`/`RF`, clears
the VM86 data-segment caches, and enters the protected target `CS:EIP`.

`IRETD` with `EFLAGS.VM=1` can now consume that extended frame from ring 0 and
restore the VM86 task's real-mode-style visible segments and stack.  This is
still not a full VM86 monitor; software interrupt reflection and virtual
interrupt-flag semantics remain follow-up items.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `539712` bytes
- `pico_286` SHA256:
  `DC37C17D464E6A803F25D5A5241A70F0568322F67D142DFCB4A7C52563CDB989`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 386 TSS I/O permission bitmap

Added 80386 I/O permission checks for `IN`, `OUT`, `INSB`, `INSW`, `OUTSB`,
and `OUTSW`.  When protected code runs with `CPL > IOPL`, or when VM86 mode
is active, the emulator now checks the current 32-bit TSS I/O permission
bitmap at offset `66h`.  A set bit, missing 32-bit TSS, too-short bitmap, or
word access past the 16-bit I/O-port space raises `#GP(0)`.

`CLI` and `STI` still use the stricter IOPL-only helper because they are not
port I/O instructions.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `488664` bytes
- `pico_286` SHA256:
  `C9B1D05D1CCB895A2D636CA16947C70A51ED85189F0BC1FBBA85AA6790EA710A`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 initial VM86 task entry

Added the first virtual-8086 task-entry support for the 80386 protected-mode
path.  A 32-bit TSS whose saved `EFLAGS` has `VM=1` now loads `CS`, `SS`,
`DS`, `ES`, `FS`, and `GS` as real-mode segment values instead of validating
them as protected descriptors.  In VM86 mode, segment linear addresses use
`selector << 4`, segment reloads cache real-mode bases, and the effective CPL
reported to paging and privilege helpers is 3.

This is intentionally not a complete VM86 monitor yet.  I/O bitmap checks,
interrupt reflection, and full DOS-extender services still need their own
follow-up items.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `535908` bytes
- `pico_286` SHA256:
  `2314D0DA5972D8DAC6C02F5CE8D62311DD2B9380989701057FF405B2DCEF323B`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio/helper code.

## 2026-06-02 hardware task switching

Implemented the first hardware task-switching layer for 286/386 protected
mode.  `LTR` now marks the loaded TSS descriptor busy.  Far `CALL`/`JMP` can
target 16-bit or 32-bit TSS descriptors and task gates.  IDT task gates now
switch tasks instead of being rejected.  `IRET` with `NT` follows the current
TSS backlink.  Task switches save and restore the 16-bit/32-bit TSS register
images, update descriptor busy bits, set backlink for task `CALL`/interrupt
switches, clear the outgoing busy bit for task `JMP`/nested-task `IRET`, load
`LDTR`/`CR3`, and set `CR0.TS`.

The 16-bit and 32-bit immediate far `JMP` decoders now advance `IP/EIP` past
the selector before the protected transfer, so a task switch saves the address
after the full instruction.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Out .\homebrew\pico_286\pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `538512` bytes
- `pico_286` SHA256:
  `E1B3348B6402002390736455B81AB8405808C4F3D485C9103FB18BABBC16B3FF`
- Patch copy was updated with the same binary.
- WSL/GCC build succeeded with the existing warning set in FPU, XMS, renderer,
  and audio helper code.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 FWAIT and CPU-specific FLAGS masks

Implemented the `WAIT/FWAIT` opcode path and tightened `FLAGS`/`EFLAGS`
masking by configured CPU model.  `FWAIT` now checks `CR0.MP && CR0.TS` for
`#NM`, synchronizes the emulated x87 status word, and raises the x87 error
exception when an unmasked pending x87 fault is latched.  `PUSHF`, `POPF`, and
`IRET` now use explicit 8086/80286/80386 masks so 80286 real mode cannot
inherit stray 386-only bits.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `484632` bytes
- `pico_286` SHA256:
  `B704FF27A5D29755E62315AABF74E55F1937EF8B80838D0602A6A773D85DB22E`
- Embedded screenshot `HASH8`: `83c6c3d8`.
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 14:25:50`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 divide-error fault IP

Fixed the x86 `#DE` path for `DIV`, `IDIV`, and `AAM 0`.  These faults are
detected after the decoder has already fetched operands, so the CPU core now
restores IP to the start of the faulting instruction before calling interrupt
0.  The fix covers 8-bit, 16-bit, and 32-bit divide helpers.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `483248` bytes
- `pico_286` SHA256:
  `8C727096F86F44FDE82250F423CDE552FC638B692EC335B05407D8E56CF5CE1F`
- Embedded screenshot `HASH8`: `6c74c964`.
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 13:48:47`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 R2 physical Alt for on-screen keyboard

Added a third held physical modifier while the on-screen keyboard is visible:
`R2` now sends PC `Alt` down while the trigger is held and releases it on
trigger release, matching `L` for Shift and `R` for Ctrl.  The Alt keys on the
keyboard also highlight while physical `R2` is held, and the fit-height header
hint now includes `R2=ALT`.  Latched Shift/Ctrl/Alt no longer release an
equivalent physically held modifier, so holding `R2` cannot be cancelled by a
latched Alt keypress.  The context-menu key label was also expanded from `MNU`
to `MENU`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `483120` bytes
- `pico_286` SHA256:
  `968045A4CFDE3464566F86D1B3AD14CB4D84F2579F7149E7301766F5FBC2A356`
- Embedded screenshot `HASH8`: `b46f459c`.
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 13:39:49`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 neutral on-screen modifier labels

Changed the shared on-screen keyboard renderer so side-specific modifier keys
are drawn with neutral PC-style labels: `SHIFT`, `CTRL`, and `ALT`.  The
underlying key entries still keep their assignment labels (`LSHIFT`,
`RSHIFT`, `LCTRL`, `RCTRL`, `LALT`, `RALT`), so the key preset picker can still
save side-specific modifiers.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `482928` bytes
- `pico_286` SHA256:
  `D94D4F01C1FFB4D4B64DF9486BB5B2806F4D24CE6E68BE097C7691FAEE6EFD4A`
- Embedded screenshot `HASH8`: `1d6d1704`.
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 13:24:20`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 screenshot build hash filenames

Added `screenshot_build_hash` to `[screenshot]` in `pico_286.conf`.  When it
is `1`, screenshots include the first 8 hex digits of the embedded build
commit-object SHA-256:

```text
pico_286_YYYYMMDD_HHMMSS_HASH8_NNN.png
```

The current build embeds `c93853b3` as `HASH8`.  Set
`screenshot_build_hash=0` to keep the older timestamp-only name.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `482832` bytes
- `pico_286` SHA256:
  `FD3DD2C638D413FD900E3CF507F9BF39E68FE9D6A020B7BA54F0E475EE992C61`
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 13:17:46`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 CPU MHz throughput multipliers

Changed the `cpu_mhz` speed knob so it no longer maps one emulated x86
instruction to one host-side "cycle" at 1 MIPS/MHz.  Pico-286 now converts
`cpu_mhz` to `exec86()` instruction budgets with model-specific, round
historical throughput estimates:

- `8086`: `75,000` instructions/sec per MHz.
- `80286`: `150,000` instructions/sec per MHz.
- `80386`: `300,000` instructions/sec per MHz.

These round values are based on common period-performance figures: 8086 around
`0.75 MIPS @ 10 MHz`, 80286 around `1.5 MIPS @ 10 MHz`, and 386-class
systems around `11.4 MIPS @ 33 MHz`.  The parser stores the numeric MHz value
and recalculates `exec_loops` whenever either `cpu_model` or `cpu_mhz` changes,
so config key order no longer changes the result.

Reference pages used while choosing the rounded multipliers:

- https://www.microelec.patricklecoq.fr/guide/8086.html
- https://svho.omeka.net/items/show/2058
- https://www.dosdays.co.uk/topics/cpus.php

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `481852` bytes
- `pico_286` SHA256:
  `75420D3EAC08C84D2010CAA7A7F6857E1D50273D5AD49A8B991C22082D029A90`
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 13:07:26`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 screenshot RTC timestamp fix

Changed screenshot filename timestamps to use Pico-286's emulated RTC counter
instead of the Linux host clock.  The screenshot backend now calls
`r36sx_pico286_rtc_current_time_unix()`, which initializes CMOS/RTC state if
needed and then returns the same emulated time base that DOS reads through the
RTC ports.  Screenshot names keep the existing
`pico_286_YYYYMMDD_HHMMSS_NNN` format.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `481468` bytes
- `pico_286` SHA256:
  `187B8AD2D50664D4A7398084BEC2D074B10685BF63F83136906FF1FA3BC9C348`
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 12:54:34`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 BOUND register operand fix

Fixed the 80186+ `BOUND` instruction handling in the native Pico-286 CPU core.
The ModR/M `mod=11b` register-operand case is now named
`R36SX_MODRM_MOD_REGISTER` and rejected as an invalid opcode, because Intel
`BOUND` requires a memory bounds table.  Bounds failures now reset IP to the
start of the instruction before raising `R36SX_EXCEPTION_BOUND`, so INT 5 saves
the faulting instruction address instead of the IP after ModR/M decoding.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `481080` bytes
- `pico_286` SHA256:
  `E3DE5792CB365F3509B0D723546B56AAD0DA208C070E72B3B54E74973E580646`
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 12:46:43`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 ENTER nesting frame fix

Fixed the 80186+ `ENTER imm16, imm8` implementation in the native Pico-286
CPU core.  The nesting level is now masked to five bits, and nested ENTER
copies saved frame-pointer words from the caller's frame chain instead of
pushing BP addresses.  Plain `ENTER n,0` keeps its previous behavior, while
nested block-structured stack frames now match Intel's 16-bit ENTER semantics.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `480920` bytes
- `pico_286` SHA256:
  `9C01097017292A334D01D244837642D10ED8C2F6EA63B7D7A29F57B0C02FA21E`
- Patch config `[rtc] rtc_start_time` updated to `2026-06-02 12:35:04`.
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 active combo key preset switching

Changed native input handling so physical buttons with complex key preset
bindings are mutually exclusive.  If `Ctrl+A` is held and another physical
button mapped to `Alt+S` is pressed, Pico-286 now releases `Ctrl+A` before
pressing `Alt+S` instead of merging them into `Ctrl+Alt+A+S`.  Single-key
bindings still combine normally with other held single keys.  Side-specific
Shift/Ctrl/Alt keys were also marked non-repeatable.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath 'homebrew\pico_286\pico_286' -Destination 'patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286' -Force
```

Result:

- `pico_286` size: `480892` bytes
- `pico_286` SHA256:
  `6F67F1ACCE6D0E4169C6187E4CC42BB94D1B63BA6F169F709B15270AC025A806`
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 side-specific key preset modifiers

Added distinct on-screen keyboard picker keycodes for left/right modifiers:
`LSHIFT`, `RSHIFT`, `LCTRL`, `RCTRL`, `LALT`, and `RALT`.  The normal keyboard
still treats those visible modifier keys as latched Shift/Ctrl/Alt keys, while
the key preset picker can now save the side-specific labels into
`keypresets.conf`.  Pico-286 maps right Ctrl and right Alt through `E0`
extended scancodes.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `479036` bytes
- `pico_286` SHA256:
  `2A305B44DC1D75AD6DC3860A7A1EB3B0FE5F9EA886ADF4BF0D160AC0C7CF0723`
- Microsoft Defender scan: no threats found in the main and patch copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 debug builds force O2

Updated the WSL GCC build scripts so any `DEBUG=1`/debug-log build forces
`-O2`.  This applies both to the Windows wrapper and to the WSL shell script,
so an accidental `-DebugLog -OptLevel O3` or `--debug-log --opt-level O3`
is downgraded to `O2` before compilation.  Release builds can still use `O3`.

Verification command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --debug-log --opt-level O3 --strip --out .tmp/pico_286_debug_o2_test
```

Result:

- Build succeeded.
- The script printed: `Debug build requested; forcing --opt-level O2 instead of O3`.
- The normal release `homebrew/pico_286/pico_286` binary was not rebuilt by this
  change.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 host-drive commit and write-error handling

Fixed the DOS network redirector path used by `MAPDRIVE.COM` when copying from
an image-backed DOS drive to the host `H:` directory.

- `INT 2Fh AX=1107h` is now a real commit operation: it flushes the host file
  but keeps the SFT handle open. The old code shared the close path with
  `AX=1106h`, which could close a destination file while DOS still expected to
  keep writing it.
- Remote writes now treat short `fwrite()` results and `fflush()` failures as
  DOS errors instead of reporting success. This should surface SD-card full or
  host I/O failures as destination-write errors instead of letting DOS continue
  with inconsistent state.
- Successful remote writes now keep the SFT file size in sync with the updated
  file position.
- Error-only redirector log messages were added for failed remote read/write,
  commit, close, and flush paths. They are emitted only when the build uses
  `DEBUG=1`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- `pico_286` size: `468844` bytes
- `pico_286` SHA256:
  `F7BB9242D3E92303EA4AB51358EF0832F2CAAB44C4456765DDF8CD2562ADFD7E`
- Microsoft Defender scan: no threats found in the main, patch, and
  `disk_image` copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 on-screen keyboard Scroll Lock LED

Added a small green lock-state LED to the on-screen `SL` key in the
cursor/system-key block. It reuses the Caps Lock LED renderer, stores the
`scroll_lock` state in the shared keyboard object, toggles that state when
`SL` is emitted, excludes Scroll Lock from key repeat, and includes the flag in
the Pico-286 MiniFB overlay cache signature.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

- `pico_286` size: `467716` bytes
- `pico_286` SHA256:
  `88F69DF93A51D391A769748F9156085996B7D65931CD9F620D8632F124E124B0`
- Microsoft Defender scan: no threats found in the main, patch, and
  `disk_image` copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 parameterized MAPDRIVE.COM

Updated `homebrew/pico_286/pico-286/tools/mapdrive.asm` so the DOS mapper
accepts an optional drive-letter argument.  `MAPDRIVE.COM` still defaults to
`H:`, but `MAPDRIVE G:` and `MAPDRIVE G` now register the same host directory
under another DOS drive letter.  Invalid arguments print a short usage message.
Pico-286 was also rebuilt because the generated `pico_286.conf` host-drive
comments are compiled string literals in `r36sx_disk_config.c`.

MAPDRIVE build command:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -f bin .\homebrew\pico_286\pico-286\tools\mapdrive.asm -o .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\mapdrive.com
```

- `mapdrive.com` size: `445` bytes
- `mapdrive.com` SHA256:
  `1E13203134903B01E7C131D4A0B366F45336F519AA6A42BEC4D5B245AE76FA0E`
- The rebuilt `.COM` was mirrored to
  `disk_image/MIPS_NATIVE/pico_286/mapdrive.com`.
- Microsoft Defender scan: no threats found in the patch and `disk_image`
  copies.

Pico-286 rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

- `pico_286` size: `467716` bytes
- `pico_286` SHA256:
  `12B3F725F87E2F9A9D374E49EB1F5CFA8101F90568EFB55545482E55F3DFA4F0`
- Microsoft Defender scan: no threats found in the main, patch, and
  `disk_image` copies.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-02 remove embedded host-drive mapper launch

Removed the R36SX-only embedded MAPDRIVE trampoline from Pico-286.  The disk
menu no longer has a `CONNECT DISK H:` action, the CPU core no longer injects a
COM stub at `9000:0100`, and the private `INT F1h` return path was removed.
Drive `H:` is now registered only by running the standalone `MAPDRIVE.COM`
inside DOS.  The `host_drive_path` config and `INT 2Fh/11h` network redirector
remain in place as the backend used by that DOS command.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

- `pico_286` size: `467652` bytes
- `pico_286` SHA256:
  `E86895862817FA229CEC141E259103E6453C1AA5EE702125E80D4F6E68D50A04`
- Microsoft Defender scan: no threats found in the main binary and patch copy.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 move on-screen keyboard F5-F8 group

The on-screen keyboard function-key row now moves the `F5`-to-`F8` block one
pixel to the right while keeping the `F9`-to-`F12` block in place.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `471108` bytes
- `pico_286` SHA256:
  `8BE6C8DA6E707415F4E66D792EA93B16AEC6D2AB15D4A049C2B550FF86BC93B8`
- Microsoft Defender scan: no threats found.

## 2026-06-01 fake on-screen keyboard navigation cells

The on-screen keyboard navigation grid now treats the visual gap between
`Esc` and `F1` as another `Esc` cell, and the gap between `F8` and `F9` as
another `F8` cell.  This keeps vertical movement predictable without adding a
separate redirect rule.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `471108` bytes
- `pico_286` SHA256:
  `1A349F9FCB7C4AEAA391705A261BD01D0DACEB8585D061C4E94D3BEFE51F7565`
- Microsoft Defender scan: no threats found.

## 2026-06-01 remember on-screen keyboard height mode

The on-screen keyboard now keeps its expanded/compact height choice across
close/open cycles.  `Select` still toggles the mode while the keyboard is
visible, but closing it no longer resets the choice.  The first open still
defaults to the expanded/full keyboard.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `471108` bytes
- `pico_286` SHA256:
  `268D8387EC0A4D1F531B9527E1836581EC2C8D141B1CC5D7FBADFCBF3D1BB721`
- Microsoft Defender scan: no threats found.

## 2026-06-01 shorter on-screen keyboard system-key labels

Shortened the on-screen keyboard cursor/system block labels: Scroll Lock now
draws as `SL`, and Pause Break now draws as `P/B`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `471108` bytes
- `pico_286` SHA256:
  `404F89A9C23A13E4AE93A1F62484600F85D5F182FC0C0BD3F86E6B5ACDAC31D7`
- Microsoft Defender scan: no threats found.

## 2026-06-01 on-screen keyboard navigation grid

The on-screen keyboard now uses the provided `key-map.csv` style 6x18
navigation grid instead of plain row/column index movement.  Repeated adjacent
cells model wide keys such as Backspace, Enter, Shift, Space, Menu, and Ctrl.
Horizontal movement skips over repeated cells and `empty` holes; vertical
movement starts from the wide key's middle cell, choosing the left-middle cell
when the width is even.  Hidden cursor-block keys are treated like empty cells.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `471108` bytes
- `pico_286` SHA256:
  `F9FAB2346903264F63C3976CE9AFF6CB33ECC2E17B5ED2380A0318110D4189EA`
- Microsoft Defender scan: no threats found.

## 2026-06-01 disk menu host-drive connected state

The disk menu now mirrors the emulator-side MAPDRIVE state.  Before `H:` is
connected the row reads `CONNECT DISK H: HOST/`; while the trampoline is
pending it reads `DISK H: CONNECTING TO HOST/`; after a successful MAPDRIVE
completion it reads `DISK H: CONNECTED TO HOST/` and repeat presses no longer
start another trampoline.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `469748` bytes
- `pico_286` SHA256:
  `75A00B846C9499BC03D698461E19829FE0F00D0193A5CFFE7999AABD7DFE6726`
- Microsoft Defender scan: no threats found.

## 2026-06-01 remove host-drive interrupt-depth gate

Removed the guest interrupt-depth gate from the disk menu `CONNECT DISK H:`
path.  That gate could keep the MAPDRIVE request pending forever at a normal
DOS prompt, because COMMAND.COM and DOS console input can wait inside an
`INT 21h` handler.  The connection path is back to the direct embedded
trampoline behavior, while retaining the unrelated repeated-press guard:
after a successful `H:` connection, later connect requests are ignored until
the VM is reset.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `468068` bytes
- `pico_286` SHA256:
  `19FD8A33A3B917E0E8D87951078F0EE85AA3A3E5EE2CD057BE7AA123CA89A77E`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 safer host-drive connect requests

Hardened the disk menu `CONNECT DISK H:` path against two edge cases:

- if the button is pressed while the emulated CPU is inside a guest interrupt
  handler, the MAPDRIVE trampoline request now stays pending and starts only
  after the corresponding real-mode `IRET` returns the interrupt depth to zero;
- after a successful connection, repeated `CONNECT DISK H:` presses are ignored
  instead of running the redirector registration again.

The guest interrupt depth is incremented only when `intcall86()` falls through
to the real-mode IVT path and decremented by real-mode `IRET`.  VM reset clears
the depth counter, pending request, in-progress marker, and connected flag.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `468196` bytes
- `pico_286` SHA256:
  `CC2E8CF295FCBBC60C628C5C21A4577ACC63965AA8EEF145810E03BE0601289B`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 disk menu action button spacing

Moved the disk menu `OK`/`Cancel` action row so it is anchored to the last menu
row instead of the bottom of the screen.  The action row now sits 58 pixels
below `EXIT APP`, matching the current gap between the last key preset binding
row and the `OK`/`Cancel` buttons in the key preset editor.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `467972` bytes
- `pico_286` SHA256:
  `0BDE17425438F4D9E41BFAEC21FD9A2BDA72805636A2F354B24467973B9C2925`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 disk menu host-drive connector

Updated the disk image menu to match the key preset editor layout more closely:
drive/config rows use the same compact row height and gap, the bottom action
row now has side-by-side `OK` and `Cancel` buttons, and the old `SAVE/APPLY`
row is replaced by `OK`.  Added a `CONNECT DISK H:` row after `HDD1 D:`.

`CONNECT DISK H:` no longer needs a separate `MAPDRIVE.COM` file copied into
the DOS disk.  Pico-286 now embeds a tiny MAPDRIVE-compatible COM trampoline in
the native executable, backs up RAM at `9000:0000`, copies the trampoline to
`9000:0100`, runs it in real mode, catches a private `INT F1h`, restores the
previous CPU/RAM scratch state, and leaves DOS's CDS/redirector changes in
place.  The command is refused if protected mode is active, if the scratch
segment is outside conventional RAM, or if DOS has not installed an `INT 21h`
vector yet.  `CONFIG.SYS` still needs `LASTDRIVE=H` or higher.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `467972` bytes
- `pico_286` SHA256:
  `1AB5A08924450FC5DA85239A68E8306EB1B791AF78C47A40B1F38B7CF92D81E4`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 compact 386 condition-code helper

Rewrote the shared 386 `Jcc`/`SETcc` condition helper so it evaluates the
eight even x86 condition predicates and applies the low opcode bit as the
standard inverse selector.  The comments now name the paired mnemonics
(`O/NO`, `B/NB`, `Z/NZ`, `LE/G`, and so on), which keeps the compact form
readable while avoiding 16 nearly duplicated switch cases.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `466180` bytes
- `pico_286` SHA256:
  `183D55AE623785F7DA6DF08BDFF7D37C5431B17AA959ACDF2A6EE70D63255FB1`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 debug invalid-opcode hex dump

Added a debug-only 256-byte code-context dump to the shared invalid-opcode
handler.  When `DEBUG` is enabled and the CPU trips INT 6, the log now prints
the usual register snapshot followed by 16 hex rows of 16 bytes from `CS`,
centered around the faulting opcode.  The byte at the faulting `IP/EIP` is
marked with `^`, which makes it easier to see the surrounding instruction
stream when DOS programs hit an unsupported opcode.

Validation and rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --debug-log --opt-level O3 --strip --out .tmp/pico_286_debug
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- Debug-log build succeeded; the new `DEBUG` path compiles.
- Release `pico_286` size: `466756` bytes
- Release `pico_286` SHA256:
  `D10CD2D26A83E7F735EA3E7FB2C9072903155E5B8876F6CA6C2022F42A790866`
- Defender CLI scan: found no threats in the main binary.  The patch and
  `disk_image` copies are bit-identical to the scanned binary.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 CPU physical memory width fast path

Changed the CPU core's paging-aware physical memory helpers to use the existing
native fast 16-bit and 32-bit memory accessors when `R36SX_NATIVE_FAST_MEMORY`
is enabled.  This avoids splitting every word/dword CPU memory access into
multiple byte backend calls for normal RAM while keeping byte fallback behavior
for unaligned/non-fast paths.  The optimization benefits many opcode paths at
once: `MOV`, stack operations, far control transfers, string operations, and
386 dword instructions all pass through these helpers.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `466756` bytes
- `pico_286` SHA256:
  `D10CD2D26A83E7F735EA3E7FB2C9072903155E5B8876F6CA6C2022F42A790866`
- Defender CLI scan: found no threats in the main binary, patch copy, and
  `disk_image` copy.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 protected far control transfers

Changed protected-mode `CALL FAR`, `JMP FAR`, `RETF`, and `IRET` handling so
the CPU core no longer just loads a new `CS:IP`.  Immediate and indirect far
forms now decode protected descriptors, accept 16-bit and 32-bit call gates,
check `CPL`/`DPL`/`RPL`, validate target code descriptors and offsets, and use
the current TSS `SSn:SPn/ESPn` entries for inter-privilege call-gate stack
switches.  Protected `RETF`/`IRET` now validate return selectors and restore an
outer `SS:SP/ESP` when returning to a less privileged ring.  Hardware task
switching through task gates/TSS descriptors is still intentionally rejected
with a protected fault and remains a separate TODO.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath .\homebrew\pico_286\pico_286 -Destination .\disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `467012` bytes
- `pico_286` SHA256:
  `66A75F6CB6B79E3B949E149F33CD1AA1864E9B434FEA2247417FB152A91BE2F8`
- Defender CLI scan: found no threats in the main binary, patch copy, and
  `disk_image` copy.
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 protected-mode segment access checks

Changed the CPU core so `getmem*` and `putmem*` go through protected-mode
segment limit/type checks instead of directly doing `segbase + offset`.
The REP MOVS/STOS RAM fast path now validates the whole source/destination
span before using the block-copy shortcut, and the remaining direct `ea`
reads for BOUND, LES/LDS, 386 far CALL/JMP, and memory bit-test operations are
guarded by the same segment access helper.

## 2026-06-01 stats Q glyph fix

Fixed the compact statistics overlay font by adding a 3x5 `Q` glyph.  The
`QPS` row no longer loses its first letter when the `Fn` + D-pad `Down`
statistics overlay is visible.

## 2026-06-01 app statistics QPS

Added `QPS` to the `Fn` + D-pad `Down` statistics overlay.  It counts main-loop
quanta per second, i.e. how many host `while (running)` loop passes completed
per second.  The overlay now shows five rows: `X86`, `QPS`, `READ`, `WRITE`,
and `FPS`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `461352` bytes
- `pico_286` SHA256:
  `7AEF3381E8430036DF134D6EE5925E164E86200C738BA0AEC4A428D2DE53DB8D`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard Caps LED and Backspace label

Adjusted the `CAPS` key label/LED layout: the green Caps Lock LED moved one
pixel left, and the `CAPS` text is centered between the key's left edge and the
LED's left edge.  Backspace now uses the same left-arrow glyph as the cursor
block before the `BS` label.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `462100` bytes
- `pico_286` SHA256:
  `B7FB1F62C4FDA67DE2D46B31240145B03EECFC6CAE8C1695D6E509FFE2AF1490`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard bottom-row alignment

Adjusted the PC-style on-screen keyboard bottom row so its visual guide
columns match the rows above: `WIN` is now 1.5 units wide and ends under `A`,
the spacebar ends under comma, and `MNU` starts under the `=`/`F10` guide
column.  The row remainder is split between the spacebar, right Alt, and right
Ctrl instead of putting all extra pixels into one key.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `462100` bytes
- `pico_286` SHA256:
  `0A4A6C2E0D489CD15B49C24280530B0BB658EBD21566D60C13750BC1F36DF418`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard default fit-height compact 80

The on-screen keyboard now opens in fit-height mode by default, just tall
enough to show all key rows.  Select collapses it to an 80 px compact panel and
toggles back to fit-height mode.  The compact panel hides the header hints so
the fixed 80 px area is used for keys and scrolling; fit-height mode still
shows the button hints in the header.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `462100` bytes
- `pico_286` SHA256:
  `103BB2C61FCDF47AD007FC75864E21B6BDD9B75ABC2C9B20A6C49714A23C1689`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard header spacing

Removed the `DOS KBD ABC` / `DOS KBD SYM` prefix from the on-screen keyboard
header.  The header now shows only the physical-button hints, with double
spaces between each hint so the labels are easier to scan.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `462100` bytes
- `pico_286` SHA256:
  `69EA74C2893E6F49E527D6D3A812B870E1C20535289E702785DFA8D1423450AC`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard fit-height toggle

Select now toggles the on-screen keyboard between the compact 96 px panel and a
fit-height panel instead of closing the keyboard.  The expanded panel is only
tall enough to show all six key rows at once; it is not full-screen, and keeps a
one-pixel key-area gap above and below the keys.  Fn tap remains the show/hide
shortcut.

The Pico-286 overlay path now allocates keyboard overlay buffers for the
maximum fit-height panel and stores the exact saved overlay rectangle before
restoring it, so switching panel height cannot restore the wrong rows.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `462100` bytes
- `pico_286` SHA256:
  `EDF4B2D97354206AA1153B197510758B9C0ABA955589BCBA74825994A8DE0D66`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard Caps Lock LED

Added a small green Caps Lock indicator to the right side of the on-screen
`CAPS` key.  The keyboard module now tracks Caps Lock state, uses it when
choosing uppercase/lowercase labels, and excludes Caps Lock from key repeat so
holding the physical confirm button does not toggle it repeatedly.  Pico-286's
keyboard overlay cache signature now includes Caps Lock state so the LED and
labels refresh correctly in overlay mode.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `461920` bytes
- `pico_286` SHA256:
  `4404F86A95107A1AAC20822A130EFB8D4FFA331BDBFB6DFE392E2DA8E9C900DB`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard bottom row cleanup

Removed the on-screen `CLS` key from the bottom keyboard row.  The bottom row
now ends as `ALT MNU CTRL`; `MNU` is a wider context-menu key, and `SPC` uses a
6.5-unit base width plus the row remainder so the visible gaps stay consistent
with the upper rows.  Closing the keyboard remains available through `Select`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457840` bytes
- `pico_286` SHA256:
  `38D53E9AD44F517882AF3078E1C8A8320AA42FEF45C0D631FB2D2D8367815F80`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard header hints

Restored compact physical-button hints in the on-screen keyboard header.  The
header now keeps the `ABC`/`SYM` mode label and also shows:
`A/START=TYPE B=BACK Y=ENTER X=ESC L=SHIFT R=CTRL SEL=CLOSE`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457856` bytes
- `pico_286` SHA256:
  `F25ABAD146132C1EF5F29A61F162AA55E4B7CB9A8CB1EB4357315332DD04DA02`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 cursor-block navigation

Fixed vertical navigation in the optional right-side cursor/navigation block.
The selector now searches the next key in the same physical column and skips
empty rows instead of snapping to the nearest key on the immediate row.  This
makes Left Arrow + Up select `DEL`, and `DEL` + Down select Left Arrow again.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457840` bytes
- `pico_286` SHA256:
  `2D30081FA9979BE97F75FA41101BEDD9AD6696E33F026DBF3E7D68E5627A9BDC`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard bottom row spacing

Adjusted the bottom keyboard row so the row-width remainder is applied to the
spacebar instead of the right `CTRL` key.  This keeps the visible gaps around
`CTRL`, `WIN`, `ALT`, `SPC`, `MENU`, and `CLS` consistent with the upper rows
while preserving the aligned right edge.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457840` bytes
- `pico_286` SHA256:
  `E1AE608C8AA8D86A6F2E6EF0EB49DD6C7B3AB9CC4226BC15107297D91A1ACB38`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard right edge alignment

Adjusted the shared on-screen keyboard geometry so the right edge is visually
even across `F12`, Backspace, `\`, Enter, right `SHIFT`, and right `CTRL`.
The F1-F4 group is nudged one pixel to the right while the gap before F5 is
reduced by the same amount, keeping the later function-key groups in place.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457840` bytes
- `pico_286` SHA256:
  `8C168A641D8528DCE7915537B7599179322F2CC33460C84F89BB8586C21060DE`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 key preset combinations

Added combo bindings to the full-screen key preset editor.  A preset entry can
now be a single key label such as `ENTER` or a modifier combo such as
`CTRL+S`, `SHIFT+A`, or `CTRL+ALT+DEL`.  Existing single-key configs remain
valid.  In key-picker mode, physical `L` toggles Shift, `R` toggles Ctrl, and
`L2` toggles Alt before A/Y accepts the highlighted key.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `457840` bytes
- `pico_286` SHA256:
  `CCEF18294841D693E6FD7C4588C2F65EE416BB283251E42EE970CFC3422426BF`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 on-screen keyboard left alignment

Updated the shared on-screen keyboard layout to better match a physical PC
keyboard:

- Main rows are now left-aligned, so `ESC`, `TAB`, `CAPS`, left `SHIFT`, and
  left `CTRL` share the same left edge.
- `SHIFT` is shown with its full label instead of `SHF`.
- Added `WIN` between left `CTRL` and left `ALT`.
- Added `MENU` between right `ALT` and `CLS`.
- Stretched `SPC` across the remaining bottom-row space.
- Nudged the function-key grouping so the `F10`/`F11` split aligns with the
  `=`/Backspace split.
- Added Pico-286 scancode emission for `WIN` (`E0 5B`) and context menu
  (`E0 5D`).  These keys are also available in the key-preset picker.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `454012` bytes
- `pico_286` SHA256:
  `29EEEADB7F8722ED6D8093107154CF71C66A9875EEC87D14B5D78F5024992D0F`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-06-01 VBE 640x480 modes and BIOS services

Expanded the minimal VBE implementation:

- Added banked VBE modes `101h` (640x480x8 packed pixel) and `111h`
  (640x480x16 RGB565), matching the console's native 640x480 panel.
- Kept `103h` (800x600x8) and `114h` (800x600x16) as downsampled modes.
- Mode info blocks now report the actual width, height, pitch, bank count,
  image pages, memory model, RGB565 masks, and 64 KiB bank granularity.
- `4F00h` reports the full mode list and the 8-bit DAC capability flag.
- Added `4F06h` logical scanline length, `4F08h` DAC width, and `4F09h`
  palette data services.
- The SVGA renderer now uses the active mode width, height, and pitch, so
  640x480 modes render 1:1 without downsampling.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `454156` bytes
- `pico_286` SHA256:
  `A6AAF2096D7229E4DD752FBFC62D8278C3FBEA568733A51B0DD2CD3A83CB4E7B`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-05-31 On-screen keyboard L/R modifiers

Changed the shared on-screen keyboard physical trigger behavior:

- While the normal DOS on-screen keyboard is visible, holding physical `L`
  now sends and holds PC `Shift`.
- Holding physical `R` now sends and holds PC `Ctrl`.
- The physical modifier state is included in the cached keyboard overlay
  signature, so the key labels and modifier highlighting update immediately
  when `L`/`R` are pressed or released.
- OSK key events now pass through the MiniFB key-state layer instead of calling
  `HandleInput()` directly, allowing global key release paths to release these
  held modifiers safely when the keyboard or a menu is closed.
- In the key-preset picker, pressing physical `L` selects `SHIFT`; pressing
  physical `R` selects `CTRL`.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `453160` bytes
- `pico_286` SHA256:
  `7294ABCE3B440F27D2ADF13AE0049135966221D4B9A01F6DDB917FB360F78E5B`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-05-31 On-screen keyboard row alignment

Updated the shared on-screen keyboard layout to use explicit key widths of
`1`, `1.5`, `2`, and `2.5` units:

- The function-key row now has a full gap between `ESC` and `F1`, plus smaller
  group gaps between `F4`/`F5` and `F8`/`F9`.
- Main keyboard rows are drawn right-aligned inside the keyboard block, so
  `F12`, `BS`, `\`, `ENT`, right `SHF`, and right `CTRL` land on the same
  right edge.
- The close key was moved before the right `CTRL` key on the bottom row so
  `CTRL` stays the rightmost key.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `453160` bytes
- `pico_286` SHA256:
  `05E30F223D37442D156F8172F0C016486DA9B269DEC1664B3EFAE47FC31AC584`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-05-31 Arial FreeType on-screen keyboard labels

Updated the shared `r36sx_screen_keyboard` renderer so virtual key labels use
FreeType and the firmware Arial font when available:

- FreeType is loaded lazily through `dlopen()` from the firmware paths under
  `/mnt/sdcard/cubegm`.
- The preferred font is `/mnt/sdcard/cubegm/Arial_en.ttf`, with `Arial_kr.ttf`,
  `font.ttf`, and `Tahoma.ttf` as fallbacks.
- Glyph bitmaps are cached by codepoint and pixel size; the old 5x7 bitmap font
  remains the fallback when FreeType or Arial cannot be opened.
- Internal cursor labels `0x11..0x14` are rendered as Unicode arrows
  `U+2190..U+2193` through the TrueType path.
- The WSL/GCC and legacy Windows/Zig build scripts now include
  `usr/include/freetype2` so `ft2build.h` is available at compile time.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `449560` bytes
- `pico_286` SHA256:
  `2CDA4DE1D09ED6A0E505DEB541C9A05E379804F4512FD96593029203F7D1CDEA`
- `readelf -d` still shows no `NEEDED` dependency on `libfreetype`; the font
  path is optional at runtime through `dlopen()`.
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-05-31 CPU interpreter source split

Split the R36SX CPU interpreter helpers out of the large `r36sx_cpu.c`
translation unit into logical include files:

- `r36sx_cpu_8086.inl` keeps 8086/80186-compatible arithmetic, flags, divide,
  and 16-bit group-op helpers used by the base interpreter.
- `r36sx_cpu_80286.inl` keeps protected-mode descriptor, selector, CR0/LMSW,
  and segment-cache helpers.
- `r36sx_cpu_80286_interrupts.inl` keeps protected-mode interrupt delivery and
  the current VCPI-facing stub.
- `r36sx_cpu_80386.inl` keeps 32-bit operand-size helpers, bit-test helpers,
  32-bit group ops, and the extended `0F` opcode dispatcher.

The main `r36sx_cpu.c` still includes these files into one translation unit so
GCC can inline the hot interpreter paths exactly as before.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination disk_image\MIPS_NATIVE\pico_286\pico_286 -Force
```

Result:

- `pico_286` size: `445420` bytes
- `pico_286` SHA256:
  `09564384CDA4E216723AECB087D0891B23BEA63F2117C1DE6CF03BE28BB0FD7B`
- Defender scan: found no threats in the main binary, patch copy, and
  `disk_image` copy
- DSP side builds remain paused and were not rebuilt.

## 2026-05-31 INT 13h EDD/LBA hard-disk support

Implemented basic Enhanced Disk Drive services in the R36SX native disk backend:

- `INT 13h AH=41h` reports packet-access and enhanced-parameter support for
  mounted hard disks.
- `INT 13h AH=42h` reads sectors by LBA from a Disk Address Packet at `DS:SI`.
- `INT 13h AH=43h` writes sectors by LBA from a Disk Address Packet at `DS:SI`.
- `INT 13h AH=48h` fills the extended drive parameter buffer with geometry,
  total sector count, and 512-byte sector size.

The legacy CHS path is still used for `AH=02h`, `AH=03h`, and `AH=08h`.  LBA
transfers go directly from the packet LBA to the raw image file offset and use
the existing bulk host-image I/O path when the guest buffer is ordinary RAM.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out homebrew\pico_286\pico_286
Copy-Item -LiteralPath homebrew\pico_286\pico_286 -Destination patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286 -Force
```

Scan commands:

```powershell
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
```

Result:

- `pico_286` size: `445420` bytes
- `pico_286` SHA256:
  `09564384CDA4E216723AECB087D0891B23BEA63F2117C1DE6CF03BE28BB0FD7B`
- Defender scan: found no threats

## 2026-05-31 shift-aware on-screen key labels

Updated the shared on-screen keyboard rendering so the visible key labels match
the latched Shift state:

- normal letter keys draw lowercase labels by default and uppercase labels
  when the on-screen Shift key is active;
- the number row changes from `1234567890` to `!@#$%^&*()` when Shift is
  active;
- shifted punctuation labels such as `_`, `+`, `{`, `}`, `|`, `:`, `"`, `<`,
  `>`, and `?` are shown when Shift changes the key that will be emitted;
- the tiny pixel font now has real lowercase glyphs instead of silently drawing
  all lowercase text as uppercase.

The input path is unchanged: the keyboard still emits the same PC scancodes and
uses the existing latched Shift modifier around the selected key.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

Result:

- `pico_286` size: `445420` bytes
- `pico_286` SHA256:
  `09564384CDA4E216723AECB087D0891B23BEA63F2117C1DE6CF03BE28BB0FD7B`

Verification: rebuilt only the normal WSL/GCC binary, copied it to
`disk_image` and the Pico-286 patch, and scanned all three `pico_286` copies
with `tools/scan-download.ps1`; Defender found no threats.  DSP side builds
remain paused and were not rebuilt.

## 2026-05-31 PC-style on-screen keyboard layout

Reworked the shared on-screen keyboard module toward a compact PC keyboard
layout:

- the first row now contains `Esc` and `F1` through `F12`;
- the normal keyboard uses wider modifier/space/enter/backspace keys;
- the right-side block is enabled for the DOS keyboard and contains
  `Print Screen`, `Scroll Lock`, `Pause`, `Insert`, `Home`, `Page Up`,
  `Delete`, `End`, `Page Down`, and cursor arrows;
- the panel keeps taller keys and larger labels, then scrolls the key rows
  vertically inside the fixed bottom keyboard area with a small scrollbar.

Added keycode/scancode support for Caps Lock, Insert, Home, Page Up, End,
Page Down, Scroll Lock, Pause, and Print Screen.  Print Screen and Pause send
their multi-byte PC set-1 sequences through the existing emulated keyboard
queue.  The key preset picker can now name and assign these new keys as well.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

Result:

- `pico_286` size: `440636` bytes
- `pico_286` SHA256:
  `A4B2683B4587D64A15882F90D9FEB838B5EB46A120E93AE914902B3099122F02`

Verification: rebuilt only the normal WSL/GCC binary, copied it to
`disk_image` and the Pico-286 patch, and scanned all three `pico_286` copies
with `tools/scan-download.ps1`; Defender found no threats.  DSP side builds
remain paused and were not rebuilt.

## 2026-05-31 protected-mode selector instructions

Added the missing 286/386 selector-validation instructions used by protected
mode software and DPMI clients:

- `ARPL Ew,Gw` (`63 /r`) adjusts the destination selector RPL in protected
  mode and updates ZF; it is rejected as invalid opcode in real mode and in
  the configured 8086 CPU model.
- `LAR Gv,Ew` (`0F 02 /r`) validates selector visibility/type and loads the
  descriptor access-rights mask when allowed; on failure it clears ZF and
  leaves the destination register unchanged.
- `LSL Gv,Ew` (`0F 03 /r`) validates selector visibility/type and loads the
  decoded segment limit when allowed; on failure it clears ZF and leaves the
  destination register unchanged.

`VERR`/`VERW` now use the same CPL/RPL/DPL visibility check so descriptor
probing is consistent across the selector-validation family.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

Result:

- `pico_286` size: `436284` bytes
- `pico_286` SHA256:
  `FEF529EA1CBD1DEFD09332FB1E02B53FFE3E61040EF1E03B1DC195E5B61E6109`

Verification: rebuilt only the normal WSL/GCC binary, copied it to
`disk_image` and the Pico-286 patch, and scanned all three `pico_286` copies
with `tools/scan-download.ps1`; Defender found no threats.  DSP side builds
remain paused and were not rebuilt.

## 2026-05-31 protected-mode paging and exception groundwork

Added the next protected-mode CPU layer against the Intel 80386 protected-mode
model:

- protected exceptions can now push Intel-style error codes through the
  protected IDT path;
- segment loads validate basic CPL/DPL/RPL privilege rules for CS, SS, and data
  segment registers;
- CR0 rejects `PG=1` while `PE=0` with a general-protection exception;
- CPU linear memory accesses now pass through a basic 80386 two-level paging
  translator when `CR0.PG` is set, using CR3, PDE/PTE present checks, CR2, and
  accessed/dirty bits;
- common `readrm*`/`writerm*` memory operands now perform segment type and limit
  checks before touching memory.

This is still not a complete protected-mode kernel model: call gates, task
gates, hardware task switching, ring-changing IRET, v86, and a DPMI host remain
future work.

Rebuild command:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
```

Result:

- `pico_286` size: `436136` bytes
- `pico_286` SHA256:
  `142F9C8EF68F17AA16917C37330EB24D09E5A4D017A079DBF911A0C4623649FE`

Verification: rebuilt only the normal WSL/GCC binary, copied it to
`disk_image` and the Pico-286 patch, and scanned all three `pico_286` copies
with `tools/scan-download.ps1`; Defender found no threats.  DSP side builds
remain paused and were not rebuilt.

## 2026-05-31 DSP side build paused

The experimental MIPS DSP Rev2 side build is paused.  From this point, normal
Pico-286 rebuilds should update only `pico_286`; do not rebuild, copy, refresh,
or scan `pico_286.dsp` together with the regular executable unless DSP testing
is explicitly requested.

No binaries were rebuilt for this policy update.

## 2026-05-31 HMA request/release support

Implemented real High Memory Area state in the XMS handler.  `REQUEST_HMA`
now checks whether configured XMS memory is large enough to expose HMA,
rejects a second request with the XMS "HMA in use" error, marks HMA allocated,
and enables A20 so `FFFF:0010..FFFF:FFFF` reaches the physical area above
1 MB.  `RELEASE_HMA` now rejects release without a prior allocation and frees
the reservation.  While HMA is allocated, XMS free-memory queries reserve
64 KB for it.

The physical HMA bytes are the first part of the XMS-backed extended memory
range introduced earlier; this change makes the XMS API ownership semantics
match that mapping.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Result:

- `pico_286` size: `440048` bytes
- `pico_286` SHA256:
  `E5AE18EA8706D342362819F3860008E433659F7B54B2D5C4513C72226ACFFDA4`
- `pico_286.dsp` size: `432928` bytes
- `pico_286.dsp` SHA256:
  `24D28BDE0836A412C82DFC24A56BB12136F66F6D6A9856F513C367CA404B2CF0`

Verification: rebuilt both WSL/GCC binaries and scanned the homebrew,
`disk_image`, and patch copies with `tools/scan-download.ps1`; Defender found
no threats.

## 2026-05-31 16 MB memory limit

Raised the R36SX Pico-286 memory ceiling to 16 MB of usable emulated RAM.  The
compiled XMS backing buffer is now 15568 KB, so `total_memory_kb=16384` can be
split into 640 KB conventional RAM, 176 KB upper/UMB RAM, and 15568 KB
XMS/extended RAM.  The default config still uses `total_memory_kb=4912` until
the user explicitly increases it.

Updated `pico_286.conf` comments and config save output to allow:

- `total_memory_kb`: `64..16384`
- `xms_kb`: `0..15568`
- `extended_kb`: `0..15568`

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Result:

- `pico_286` size: `440048` bytes
- `pico_286` SHA256:
  `89F6B4BC77469D3943BE714399612E351E59AC5A74414100B9FFBC07ABEEE375`
- `pico_286.dsp` size: `432704` bytes
- `pico_286.dsp` SHA256:
  `3CD0DC8CD56EBC46919A898EE62D72D6B111F8D315C406D45CE83D068D9EC158`

Verification: rebuilt both WSL/GCC binaries and scanned the homebrew,
`disk_image`, and patch copies with `tools/scan-download.ps1`; Defender found
no threats.

## 2026-05-31 total memory auto layout and flat extended RAM

Added `total_memory_kb` to `pico_286.conf`.  When only the total is set, the
runtime maps conventional and upper/UMB memory inside the first 1 MB PC address
space, then maps XMS/extended memory above the 1 MB boundary.  Explicit
`conventional_kb`, `upper_kb`,
`xms_kb`, or `extended_kb` lines still override the automatic split.

The R36SX memory backend now maps linear physical addresses from `0x100000`
through the configured XMS size to the same XMS backing array.  That means
flat 386 protected-mode descriptors can read and write configured RAM above
1 MB instead of receiving `0xFF`/no-op accesses.  `extended_kb` defaults to
the effective `xms_kb` value unless explicitly configured, keeping INT 15h
and the XMS handler consistent for the default setup.

## 2026-06-02 physical total memory accounting

Fixed `total_memory_kb` auto-layout so the first megabyte is treated as the PC
physical address space rather than only the sum of conventional RAM and UMB RAM.
With `total_memory_kb=8192`, automatic XMS/extended memory is now `8192 - 1024`
= 7168 KB instead of `8192 - 640 - 176` = 7376 KB.  This prevents FreeDOS
`MEM` from reporting the extra 208 KB that came from ignoring the reserved
`A0000h..FFFFFh` area.

## 2026-06-02 MAPDRIVE.COM NASM build

`test386.bin` is built with the local NASM 3.01 toolchain.  `tools/mapdrive.asm`
was originally documented for FASM, but only the `NET|PHY` constant expression
was not NASM-compatible.  Replaced that expression with the already-computed
`0C000h` value so the same NASM executable can build it:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -f bin .\homebrew\pico_286\pico-286\tools\mapdrive.asm -o .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\mapdrive.com
```

The generated `mapdrive.com` is 265 bytes and was copied to the Pico-286 patch
folder and local `disk_image` copy.  Microsoft Defender scan found no threats.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Result:

- `pico_286` size: `440036` bytes
- `pico_286` SHA256:
  `849F3E5EF5AD38CFCF918F16A34B7D8CAC8DAE1B72D5462B54E4E97B73952EFC`
- `pico_286.dsp` size: `432656` bytes
- `pico_286.dsp` SHA256:
  `EF33EFCD2607D445FA3378355714CE51513DD0669D5EE9B4940D66F7623B7408`

Verification:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Defender reported no threats for all six scanned files.

## 2026-05-31 Shadow Palette RGB565 updates

Added explicit Shadow Palette comments and moved dynamic palette conversion out
of the per-frame renderer path.  The emulated VGA/TGA RGB888 palette arrays
remain the authoritative hardware state for DAC reads.  The R36SX renderer now
keeps RGB565 shadow arrays in sync when the original palette is reset or
updated, so normal frame rendering uses preconverted colors.

The DSP build also routes batch `RGB888 -> RGB565` conversion through the
existing MIPS DSP helper for two-color palette updates where that path is used.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Result:

- `pico_286` size: `422640` bytes
- `pico_286` SHA256:
  `9C718E6C34219EB52FA7D938E28CB6335F7557CBE67F075F015689F9C8C334F5`
- `pico_286.dsp` size: `414880` bytes
- `pico_286.dsp` SHA256:
  `ABB8068DCF5FCC2BB830E8B73E0C418530E089A5AAA4F99E9AF702524F2D8F1A`

Verification:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Microsoft Defender reported no threats for all rebuilt binaries and patch
copies.  WSL `objdump` confirmed that `pico_286.dsp` still contains MIPS DSP
instructions including `addu.qb`, `addu.ph`, `shrl.ph`, and `shll.ph`.

## 2026-05-31 DSP RGB565 screenshot conversion helpers

Extended `r36sx_mips_dsp.h` with batch `RGB565 -> RGB24/BGR24` helpers used by
the screenshot writers.  The normal build keeps the scalar fallback.  The DSP
build aligns the RGB565 source row and converts two pixels per iteration with
MIPS DSP halfword shifts (`shrl.ph` / `shll.ph`).  The assembly probe now also
emits a byte-lane `addu.qb` instruction so the DSP binary clearly shows both
packed byte and packed halfword DSP coverage in `objdump`.

This path was deliberately limited to screenshot export because the active
frame renderer already runs in RGB565.  A later Shadow Palette update moved
dynamic palette conversion out of the frame renderer and added a DSP-assisted
batch `RGB888 -> RGB565` helper for palette initialization/update paths.

Rebuild commands:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -EnableMipsDsp -Strip -Out .\homebrew\pico_286\pico_286.dsp
```

Result:

- `pico_286` size: `418436` bytes
- `pico_286` SHA256:
  `637193DE3D0CCD038AE1B2C9E5C5EF95E0CCF0DFAB1F0594B9D4BA816F43921C`
- `pico_286.dsp` size: `414436` bytes
- `pico_286.dsp` SHA256:
  `5BEA04BE036B91D9C30F16C3B8D255EFF2E6924984A682A40D81B0B6B1C77B97`

Verification:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
```

Microsoft Defender reported no threats for both files.  WSL `objdump`
confirmed that `pico_286.dsp` contains `addu.qb`, `shrl.ph`, and `shll.ph`,
while the normal binary contains none of those DSP instructions.  The
`disk_image` and patch copies are byte-identical by SHA256.

## 2026-05-31 test386 VGA breadcrumbs

Added direct VGA text-memory breadcrumbs to the R36SX `test386.asm` BIOS
payload.  The ROM now writes short markers to `B800:0000` while running the
early `POST 01` branch/loop diagnostics:

- `JCC8`
- `JCC16`
- `LOOP`
- `LOOPZ`
- `LOOPNZ`

The active breadcrumbs use only `MOV` stores into VGA text memory, so they do
not call BIOS services or DOS.  Finer-grained Jcc breadcrumbs are left compiled
out because inserting bytes inside the upstream Jcc macro changes the rel8
branch distances being tested.  If the test ROM stops at `POST 080:01`, the
top-left VGA marker narrows the failure to the last displayed subtest.

Rebuild command:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
```

Result:

- `test386.bin` size: `65536` bytes
- `test386.bin` SHA256:
  `CD75263B4D856EF0AC03FC2B9718A0D2C51290DD98B080E8AC48B29F989E7F66`
- `images/cpu_tests.img` size: `1474560` bytes
- `images/cpu_tests.img` SHA256:
  `1B3B5B663E7B7D03BCC24E002052A52B22573A5EB1D09F3EF229B0309106F81E`

Verification:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\test386.bin
.\tools\scan-download.ps1 .\homebrew\pico_286\images\cpu_tests.img
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 unified MIPS DSP helper build

Renamed the experimental MIPS DSP build switch to a project-wide define and
added a tiny assembly probe object:

- PowerShell wrapper: `-EnableMipsDsp`
- Shell script: `--enable-mips-dsp`
- compiler flags/defines: `-mdspr2`, `R36SX_MIPS_DSP=1`
- probe object: `r36sx_mips_dsp_probe.S`

`r36sx_mips_dsp.h` now owns the `R36SX_MIPS_DSP` / `R36SX_MIPS_DSP_ENABLED`
logic and provides portable fallbacks for every helper.  The first accelerated
paths cover hot 16-bit buffer operations:

- RGB565 framebuffer row copies and fills in `r36sx_minifb.c`
- audio mix/playback buffer copies in `r36sx_linux_audio.c`
- audio queue copies in `r36sx_port/r36sx_linux-main.cpp`

The DSP path aligns pointers, operates on two 16-bit values per 32-bit word,
and uses packed-halfword `addu.ph` as the first hardware compatibility probe.
The normal build compiles the same helpers to the portable C path and contains
no `addu.ph` instruction.

Primary non-DSP rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
```

Experimental DSP rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -EnableMipsDsp -Strip -Out .\homebrew\pico_286\pico_286.dsp
```

Result:

- `pico_286` size: `418436` bytes
- `pico_286` SHA256:
  `F5F3CDB962223B5997834BD45A29A3320B9F65F8A38E1BF6E1ADA5A46EC048E5`
- `pico_286.dsp` size: `412276` bytes
- `pico_286.dsp` SHA256:
  `AD8FDA75D9278FEC176BE692F5E0DD25D77D31F77165886B02B3C4DAC5715257`

Verification:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
```

Microsoft Defender reported no threats for both files.  WSL `objdump`
confirmed that the DSP binary contains `addu.ph` instructions and that the
normal binary contains none.  The `disk_image` and patch copies are
byte-identical by SHA256.

Reference notes:

- GCC documents `-mdsp` / `-mdspr2` for MIPS DSP ASE; `-mdspr2` implies
  `-mdsp` and defines `__mips_dsp` / `__mips_dspr2`.
- The MIPS 74Kc datasheet describes DSP ASE Rev2 support, but the runtime
  still needs testing on this exact console SoC.

## 2026-05-31 BIOS VGA mode reset and GCC primary build

Moved the active Pico-286 rebuild path to WSL/GCC.  The deployed
`homebrew/pico_286/pico_286` binary is now produced by the Linux
`mips-mti-linux-gnu-gcc` toolchain with `-march=mips32r2 -mtune=74kc -O3` and
stripped by `mips-mti-linux-gnu-strip`.

The BIOS `INT 10h AH=00h` set-video-mode handler now calls
`vga_set_standard_mode()`.  That routine loads standard VGA register tables for
text mode `03h` and graphics modes `0Dh`, `0Eh`, `0Fh/10h`, `11h`, `12h`, and
`13h`: Misc Output, Sequencer, CRTC, Graphics Controller, Attribute Controller,
DAC state, default VGA palette, start address, and derived cache flags are
reset together.  This mirrors the way VGA BIOS mode setting reprograms the
controller instead of leaving tweaked registers from the previous program.

This specifically targets the Supaplex/SPFIX -> SuperScape/3DBENCH sequence:
Supaplex can leave nonstandard VGA/CRTC state behind, and a later mode set must
restore a complete standard mode before rendering the next program.

Build command from repository root:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
```

Result:

- `pico_286` size: `418356` bytes
- `pico_286` SHA256:
  `627130BAAC6ABF8FA39015A5C328340BDF1B20620D7A4761B7D7080440638784`

ELF check:

```text
ELF32, little-endian MIPS, executable, o32, mips32r2, hard-float, stripped
interpreter: /lib/ld.so.1
needed: libpthread.so.0, libdl.so.2, libm.so.6, libz.so.1,
        libstdc++.so.6, libgcc_s.so.1, libc.so.6
```

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 WSL GCC O3 build trial

Added an explicit optimization-level switch to the WSL/GCC build scripts:

- `build_pico_286_wsl.ps1 -OptLevel O3`
- `build_pico_286_wsl.sh --opt-level O3`

The default remains `O2`; this keeps the normal WSL build stable while allowing
performance experiments with `O3`, `Os`, or other GCC levels.

Build command from repository root:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286.gcc.o3
```

Result:

- `pico_286.gcc.o3` size: `416032` bytes
- `pico_286.gcc.o3` SHA256:
  `9B9FD752574DB3C81BDBA304A31F6CC0AC1FBCAB495192B42D0CB3303B31BA08`

ELF check:

```text
ELF32, little-endian MIPS, executable, o32, mips32r2, hard-float, stripped
interpreter: /lib/ld.so.1
needed: libpthread.so.0, libdl.so.2, libm.so.6, libz.so.1,
        libstdc++.so.6, libgcc_s.so.1, libc.so.6
```

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.gcc.o3
```

Microsoft Defender reported no threats.  This binary was left as a separate
experimental artifact and was not copied over the normal deployed `pico_286`
binary.  GCC emitted existing warnings in the emulator sources, including
sequence-point warnings in renderer pixel writes; those should be fixed before
treating `O3` as the default build.

## 2026-05-31 lower-left POST-code overlay

Moved the optional POST-code overlay from the upper-right corner to the lower
left corner and enlarged it by drawing the `font8x8` text at 2x scale.  The
save/restore rectangle used by direct-present frames now covers the larger
lower-left box, so overlay cleanup remains aligned when the DOS frame is not
rerendered.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1433476` bytes
- `pico_286` SHA256:
  `22058A510063091ECAED3F83014C97609061E0C2E7A0D8ED3ED728D6761CB396`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 WSL GCC build path

Added an alternate Pico-286 build path for WSL using the Linux
`mips-mti-linux-gnu-gcc` / `g++` toolchain already present in the SF3000 SDK.

- New script: `homebrew/pico_286/build_pico_286_wsl.sh`.
- New Windows wrapper: `homebrew/pico_286/build_pico_286_wsl.ps1`.
- The WSL/GCC build uses `-march=mips32r2 -mtune=74kc`.
- The default output is `homebrew/pico_286/pico_286.gcc`, so this experimental
  build does not overwrite the normal deployed `pico_286` binary.
- The normal Windows/Zig build remains `-march=mips32r2` only.  Zig/LLVM 0.16
  rejects `-mtune=74kc` with `unknown CPU: '74kc'`, while the Codescape GCC
  toolchain accepts it.

Build command from repository root:

```powershell
.\homebrew\pico_286\build_pico_286_wsl.ps1
```

Equivalent direct WSL command:

```powershell
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && bash homebrew/pico_286/build_pico_286_wsl.sh"
```

Result:

- `pico_286.gcc` size: `377660` bytes
- `pico_286.gcc` SHA256:
  `006265EEB65F26A76CCD250B66BFB990B908AEAA5F5EA8CB691124CFFDA04078`

ELF check:

```text
ELF32, little-endian MIPS, executable, o32, mips32r2, hard-float
interpreter: /lib/ld.so.1
needed: libpthread.so.0, libdl.so.2, libm.so.6, libz.so.1,
        libstdc++.so.6, libgcc_s.so.1, libc.so.6
```

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.gcc
```

Microsoft Defender reported no threats.

## 2026-05-31 vendored inih config parser

Replaced Pico-286's hand-written config readers with the vendored `inih`
release `r62` source in `homebrew/common/inih`.

- `pico_286.conf` is parsed through `ini_parse_file()` in
  `r36sx_disk_config.c`.
- `keypresets.conf` is parsed through `ini_parse_file()` in
  `r36sx_key_presets.c`.
- The existing config file format and save paths remain unchanged.
- The build compiles `homebrew/common/inih/ini.c` with
  `INI_HANDLER_LINENO=1`, `INI_MAX_LINE=512`, and `INI_ALLOW_MULTILINE=0`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.  The rebuild still emits existing upstream warnings
from inherited emulator/audio sources.

Result:

- `pico_286` size: `1431956` bytes
- `pico_286` SHA256:
  `C5C3DA77147592CD380F04B61D227FF8EBA1DE2E572790CE58A35BBD8FA16290`

Scan commands:

```powershell
.\tools\scan-download.ps1 .tmp\inih-r62\ini.c
.\tools\scan-download.ps1 .tmp\inih-r62\ini.h
.\tools\scan-download.ps1 .tmp\inih-r62\LICENSE.txt
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 disabled audio de-click ramp

Removed the experimental 32-frame audio de-click ramp from `r36sx_linux_audio.c`.
The audio path still uses elapsed-time packet sizing, the four-buffer producer
queue, and the private playback buffer before `sound_driver_playframe()`.  This
build is intended to compare device behavior with raw packet boundaries while
keeping the timing/buffering fixes.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1424544` bytes
- `pico_286` SHA256:
  `E87616ABAEEDF0473E92CFAE425BE5818411594787892D39D3E959AE2A69EFE9`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 VBE 800x600 SVGA modes

Added minimal banked VBE/SVGA support for 800x600 output.

- `INT 10h AX=4F00h` now reports a VBE 2.0 info block with modes `103h` and
  `114h`.
- `INT 10h AX=4F01h` returns mode info for `103h` (800x600x8 packed pixel)
  and `114h` (800x600x16 RGB565 direct color).
- `INT 10h AX=4F02h` can set those two modes.  Linear framebuffer requests
  (`BX bit 14`) are rejected because this port currently implements the banked
  VGA window path only.
- `INT 10h AX=4F05h` switches/queries the 64 KB bank mapped at `A000:0000`.
- Added a 960 KB SVGA framebuffer and downsampled 800x600 rendering into the
  native 640x480 RGB565 output.  Mode `103h` uses the VGA DAC palette; mode
  `114h` reads RGB565 pixels directly.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.  A clean rebuild still emits existing upstream
warnings from FPU/audio helper sources; the final incremental rebuild after the
SVGA changes only reported the known objcopy failure.

Result:

- `pico_286` size: `1425620` bytes
- `pico_286` SHA256:
  `B34BDB7284498B790937B1D8C6526CE743F04833E2392AE422BE9C40239BE50B`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 elapsed-time audio packets

Changed Pico-286 audio packet sizing to follow real elapsed time.

- The audio source buffers now keep a per-packet frame count instead of using
  one fixed global block length.
- The producer queue was expanded to four source buffers.  Each buffer can hold
  up to 100 ms of source audio, enough for a 10 Hz packet cadence at 44.1 kHz.
- `target_fps` now sets the nominal audio packet cadence.  If the host loop is
  delayed, the packet sent to `driver.so` contains the number of samples that
  match the elapsed time since the previous audio packet, capped by the 100 ms
  buffer capacity.
- `linux_audio_init()` now receives the full buffer capacity so the resampling
  and shutter-mix path can handle the largest packet.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1411108` bytes
- `pico_286` SHA256:
  `F4C963069C0EEAEB204703DD24117CE29D172B331E3E8CE38C9AFF9AF9A7C8AC`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 audio frame de-clicking

Reduced audible clicks at Pico-286 audio block boundaries.

- The audio producer now uses two source buffers and the sound thread copies a
  completed block into a private playback buffer before calling `driver.so`.
  This prevents the tick thread from overwriting a block while it is being sent
  to `sound_driver_playframe()`.
- Audio block size is now derived from `target_fps`.  At the default 60 Hz and
  44.1 kHz output this produces about 735 stereo frames per block instead of
  the previous 100 ms chunk.
- `r36sx_linux_audio.c` applies a short 32-frame linear de-click ramp between
  consecutive blocks before passing audio to `driver.so`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1408448` bytes
- `pico_286` SHA256:
  `F73F874CF6868D3AD9F2B55A407B1B19CC1DC2EA5713824EC762F32EE89AB611`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 drive-type image extensions

Separated disk image selection by emulated drive type.

- Floppy drive rows accept `.img`, `.ima`, `.flp`, `.fdd`, `.vfd`, and `.dsk`.
- Hard-disk rows accept `.hdd`, `.hd`, `.hdi`, and `.raw`.
- `.img` remains floppy-only so the FreeDOS floppy images keep their original
  names.
- Bundled hard-disk images are now named `hdd.hdd` and `hdd2.hdd`.
- `.hdd` and other disk-image extensions were added to `.gitignore` so large
  raw images are not staged accidentally.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1406268` bytes
- `pico_286` SHA256:
  `3ACBCB6CF26D6FE424FB55EE6EF2CEC5AB8091C774C3069DD7C1460D41FD68E5`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 configurable image directory

Moved the Pico-286 disk image directory into `pico_286.conf`.

- Added `[disk_images] image_dir=images`.
- Drive bindings now store only file names such as `FreeDOS1.img`,
  `sopwith.img`, `hdd.img`, and `hdd2.img`.
- The disk menu scans only the configured `image_dir`; it no longer lists
  root-level legacy `.img` files.
- Short drive values are resolved through `image_dir`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.  The build also printed the existing upstream
audio/pragma warnings in `r36sx_ports.c` and included `.inl` sources.

Result:

- `pico_286` size: `1398776` bytes
- `pico_286` SHA256:
  `E5BE7E33C4858A223C1A89E3CCDD63376E94A6C069C462EAB5390DF25407F20C`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 delayed direct-overlay restore

Optimized the small-overlay present path to avoid full-frame copies.

- App statistics, POST-code overlay, disk LED, and overlay keyboard now draw
  directly into `SCREEN`.
- Before drawing, MiniFB saves only the covered rectangles: small fixed
  rectangles for stats/POST/LED and a `640x96` underlay buffer for the overlay
  keyboard panel.
- The saved pixels are restored at the start of the next `mfb_update()` after
  the next x86 execution slice.  If the DOS renderer already produced a dirty
  frame, the pending restore is discarded because `SCREEN` has already been
  regenerated.
- Plain DOS frames still go directly from `SCREEN` to `driver.so`; full-screen
  menus still draw directly into the output frame without DOS-frame
  composition underneath them.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1397156` bytes
- `pico_286` SHA256:
  `D50AC2DCE79CD8B2BC4058B8A18DBE7155797CB047C2AF93F154CC12D3640FE2`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 POST-code overlay and standard test ROM port

Added an optional on-screen POST-code overlay.

- `Fn` + D-pad `Right` now toggles a compact `POST port:code` display.
- Pico-286 captures standard BIOS POST writes to port `80h`.
- Pico-286 also keeps legacy support for the older R36SX test386 POST port
  `190h`.
- The embedded Turbo XT BIOS image was scanned for obvious `OUT 80h` patterns;
  none were found, so the normal BIOS may not emit POST values.
- The R36SX `test386.asm` build now uses `POST_PORT equ 0x80`; `OUT_PORT`
  remains `0x191`.
- `cpu_tests.img` was rebuilt so its `TEST386.BIN` payload matches the new
  ROM.

Rebuild commands:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped Pico-286 binary was kept.  The build also printed the existing
upstream audio/pragma warnings in `r36sx_ports.c` and included `.inl` sources.

Results:

- `pico_286` size: `1389532` bytes
- `pico_286` SHA256:
  `637792DF7404B28B93576CB4D84764FCD83719492DFDDFD096973700526C4029`
- `test386.bin` size: `65536` bytes
- `test386.bin` SHA256:
  `9BD72BB8ACC9FD8BDCB81E7DD9E5E756D16A5608B66CF8E44D6F26751107F67B`
- `images/cpu_tests.img` size: `1474560` bytes
- `images/cpu_tests.img` SHA256:
  `EEA493C9E526B8083F96F3FB4A93326459A4D425F28E8E4F4DF106C116D42447`

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\test386.bin
.\tools\scan-download.ps1 .\homebrew\pico_286\images\cpu_tests.img
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 stable direct-overlay present buffer

Fixed flicker in the direct-present overlay path.

- Small overlays no longer draw into `SCREEN` and restore it immediately after
  `video_driver_disp_frame()`.
- When the app statistics overlay, disk LED, or overlay keyboard is visible,
  MiniFB copies the current `SCREEN` into the stable output frame, draws the
  overlay there, and passes that stable frame to `driver.so`.
- Plain DOS frames still present `SCREEN` directly when no overlay is active.
- The overlay keyboard still uses its cached `640x96` panel, but no longer
  needs a separate underlay restore buffer.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1386804` bytes
- `pico_286` SHA256:
  `37CB2FD6156F8EE3E6270E6AE3BF0202D09D3484CC717C20CD5F78876A8DD9A5`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 fullscreen menu direct draw

Optimized the full-screen MiniFB menu path.

- The disk menu and key preset editor already fill the whole output frame.
- When either full-screen menu is visible, `mfb_update()` now skips
  `SCREEN` -> `base_frame` preparation and skips copying the cached DOS frame
  back into the output frame.
- The small overlay layers are not mixed over these menus; the menu draw owns
  the full frame for that present.
- Normal DOS frames and the cached overlay keyboard path keep their previous
  direct-present behavior.  Small overlays later changed to use a stable output
  buffer instead of save/restore inside `SCREEN`.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1391064` bytes
- `pico_286` SHA256:
  `3258BA6D20AD2591F4E9F6D0F7827E6D17D8BF250BDE468DC7033CA5A332425D`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 cached overlay keyboard

Optimized `[video] keyboard_mode=overlay`.

- The on-screen keyboard no longer makes MiniFB use the full large-overlay
  composition path by itself.
- The keyboard panel is rendered into a dedicated `640x96` RGB565 cache.
- The cache is refreshed only when visible keyboard state changes: selection,
  modifiers, symbol mode, press animation, cursor block, or visibility.
- Present-time drawing blits the cached keyboard panel through the direct
  overlay path.
- If the keyboard cache buffers cannot be allocated, the old full composition
  path is used as a fallback.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1389944` bytes
- `pico_286` SHA256:
  `A8C42591D887C254D37E212EDC64557E4E745B1F9DAE9CBB4247F351486A1620`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 configurable on-screen keyboard mode

Added `[video] keyboard_mode` to `pico_286.conf`.

- `normal` keeps the previous behavior: when the on-screen keyboard is open,
  the DOS image is resized into the area above the keyboard.
- `overlay` draws the on-screen keyboard over the DOS image without resizing
  the underlying framebuffer.
- `scaling_filter` is still used only when resizing is needed.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1381244` bytes
- `pico_286` SHA256:
  `74B49EADAB77E66333D07D7E058FFDFE5514DB24E18A235B2546D81888922CB3`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 dirty video rendering

Added R36SX video dirty tracking so the expensive `VIDEORAM`/register/palette
to `SCREEN` renderer runs only after visible emulated video state changes.

- Visible video memory writes mark the frame dirty.
- VGA/CGA/Tandy port writes, DAC/palette updates, CRTC register writes, active
  page changes, cursor position/shape changes, BIOS text writes, and cursor
  blink toggles also mark the frame dirty.
- `ticks_thread` still follows the configured target FPS, but it only calls
  `renderer()` when the dirty flag was set since the last rendered frame.
- MiniFB overlays such as disk LED, screenshot toast, stats, keyboard, disk
  menu, and key presets still present through their existing overlay paths.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1374052` bytes
- `pico_286` SHA256:
  `27B016E967771004E0D489E9977A17B262A6E90CF718BBC60613E64004857D8D`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 configurable audio sample rate

Added runtime audio mixer rate selection to `pico_286.conf`.

- `[audio] audio_sample_rate` accepts `44100` and `22050`.
- The R36SX native default remains `44100` Hz stereo.
- `22050` Hz halves the internal Pico-286 mixer rate and duplicates samples
  back to 44.1 kHz before calling the stock `driver.so` output path.
- `SOUND_FREQUENCY` is now runtime-configurable for the R36SX build, so PC
  speaker, SN76489, MIDI, OPL, and the main audio tick rate use the selected
  value.
- `driver.so` is still initialized at 44.1 kHz through the existing
  `driver_audio.h` helper.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1368624` bytes
- `pico_286` SHA256:
  `29F5235670480F9B648267CC5A4FCA29AEB03D72C0864E05EE5F0CCB89759D91`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 configurable audio device mix

Added `[audio]` switches to `pico_286.conf` so non-PC-speaker playback
devices can be muted independently while keeping their emulated I/O ports
visible to DOS software.

- The built-in PC speaker/beeper remains always enabled.
- Configurable devices: AdLib/OPL2, Sound Blaster, CMS/GameBlaster,
  SN76489/Tandy PSG, MPU-401/MIDI, Disney Sound Source, and Covox Speech
  Thing.
- Disabled Sound Blaster and Disney Sound Source devices still tick their
  sample clocks for DMA/FIFO side effects, but contribute silence to the
  mixer.
- Other disabled devices are skipped in the mixer so they do not contribute to
  the final stereo stream.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1363884` bytes
- `pico_286` SHA256:
  `558284BB5DBC900280A412071ACE7049069C3AC1BAB8686D0560DE52B0019CFE`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

## 2026-05-31 cached pixel-font stats overlay

Changed the `Fn` + D-pad `Down` statistics overlay to avoid full-frame
composition when it is the only overlay on screen.

- The statistics table now uses a small built-in pixel font instead of the
  regular 8x8 font path.
- The rendered stats block is cached in a small RGB565 buffer and refreshed at
  most once per second, matching the statistics sampling period.
- Direct-present frames save the covered `SCREEN` rectangle, blit the cached
  stats block, present through `driver.so`, and then restore the original
  pixels, matching the disk LED approach.
- Statistics no longer make `mfb_update()` enter the large-overlay composition
  path by themselves.

Rebuild command:

```powershell
.\homebrew\pico_286\build_pico_286.ps1 -TryStrip
```

`zig objcopy --strip-all` still returned `error: unimplemented`, so the
unstripped binary was kept.

Result:

- `pico_286` size: `1350728` bytes
- `pico_286` SHA256:
  `D379B1E0CA43D28A40FB1E8A760E8DE4672514EBA27A9F2A44B6CCE64570D1B5`

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
```

Microsoft Defender reported no threats.  The `disk_image` and patch copies are
byte-identical by SHA256.

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

- `POST_PORT equ 0x190` at the time; a later build changed this to the
  standard `0x80`.
- `OUT_PORT equ 0x191`
- `DEBUG equ 1`

`r36sx_ports.c` now logs writes to those ports as `test386:` lines in
`pico_286.log`.  Port `190h` logged POST byte values at the time, and port `191h` collects
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

## 2026-05-31 protected-mode diagnostics

Implemented the first protected-mode TODO item: debug builds now log the main
startup probes used by 386 DOS extenders.  With `DEBUG=1`, the CPU core logs:

- DPMI detection probes through `INT 2Fh AX=1686h/1687h`.
- The first 32 `INT 31h` DPMI service calls.
- The first 32 VCPI-style `INT 67h AX=DE00h..DEFFh` calls.
- `LMSW`, `MOV CR0`, `LGDT`, `LIDT`, `LLDT`, and `LTR`.
- The first protected-mode fault/unsupported opcode, including bytes at
  `CS:EIP`, CR0/CR2/CR3, GDTR/IDTR, LDTR/TR, stack registers, and flags.

This is diagnostic-only: it does not add DPMI, VCPI, paging, privilege checks,
or missing protected-mode semantics yet.

Build command:

```powershell
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --debug-log --opt-level O3 --strip --out homebrew/pico_286/pico_286.debug
```

Scan command:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.debug
```

Result:

- `pico_286.debug` built successfully.
- Defender scan: found no threats.

## 2026-05-31 VCPI probe handling

Added an explicit VCPI handler for `INT 67h AX=DE00h..DE0Ch`.  The current
port does not claim VCPI availability because VCPI is normally provided by an
EMS/V86 control program such as EMM386, and Pico-286 still boots real-mode DOS
directly.  Reporting VCPI as present would make DOS extenders call into a
server interface that does not yet have V86 mode, paging, or a real VCPI
protected-mode entry point behind it.

Behavior:

- `INT 67h AX=DE00h` now returns `AH=80h`, `BH=0`, `BL=0` to report VCPI not
  present.
- `INT 67h AX=DE01h..DE0Ch` now returns `AH=80h` instead of falling through an
  unrelated interrupt vector.
- Unknown `INT 67h AX=DExxh` subfunctions return `AH=8Fh`.
- Debug builds log these probes with the existing protected-mode diagnostics.

References:

- RBIL VCPI installation check:
  `https://fd.lod.bz/rbil/interrup/dos_extenders/67de00.html`
- RBIL VCPI protected-mode interface:
  `https://fd.lod.bz/rbil/interrup/dos_extenders/67de01.html`
- RBIL VCPI switch-to-protected-mode summary:
  `https://files.mpoli.fi/unpacked/software/texts/computer/inter58d.zip/interrup.o`

Build commands:

```powershell
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `pico_286` size: 440,048 bytes
- `pico_286` SHA256:
  `8C25DF78734B272D4BF5BECA081875B9B7BCFF4F6A09145F965CB2064512E5CC`
- `pico_286.dsp` size: 432,896 bytes
- `pico_286.dsp` SHA256:
  `48A60D02A8199F5AC132DCCAB8FCBA46EFD7B00F93F4885A549751DDB26D2F3F`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.dsp`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.dsp`

## 2026-05-31 protected-mode descriptor checks

Reviewed Intel 80386 protected-mode descriptor behavior and DOSBox-X CPU
source before updating the port.  The current `pico_286` and `pico_286.dsp`
binaries add `R36SX_ENABLE_PROTECTED_MODE=1` as a compile-time switch; passing
`-DisableProtectedMode` to the PowerShell build scripts or
`--disable-protected-mode` to the WSL shell script forces `CR0.PE` off.

The protected-mode path now supports LDT-backed selectors after `LLDT`, loads
`TR` through a TSS descriptor, validates code/data/stack descriptor types when
loading segment registers, and validates the target CS descriptor before
entering a protected-mode interrupt/trap gate.  This is still not a complete
386 protected-mode implementation: paging, privilege checks, task switching,
call gates, and full 32-bit `EIP` execution remain open.

References:

- Intel 80386 protected-mode descriptor tables:
  `https://www.ardent-tool.com/CPU/docs/Intel/386/manuals/prref386/s06_03.htm`
- DOSBox-X CPU source:
  `https://dosbox-x.com/doxygen/html/cpu_8cpp_source.html`

Build commands:

```powershell
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `pico_286` size: 435,612 bytes
- `pico_286` SHA256:
  `AEE2FAD17D650E032FD6DAD60FDFFD2F7604C2E1081B1A7C3B7B9403F6969908`
- `pico_286.dsp` size: 427,564 bytes
- `pico_286.dsp` SHA256:
  `2F1EE781BAF4E3DAD76569658524861566FE22CC8528FB9D021CA5DE33D05657`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.dsp`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.dsp`

## 2026-05-31 32-bit protected-mode execution groundwork

Protected-mode DOS extenders such as the ones used by Doom need more than
GDT/IDT entry: they normally switch to 386 code descriptors where the CS D/B
bit makes operand and address size default to 32 bits, and the SS D/B bit makes
stack operations use `ESP`.  The current `pico_286` and `pico_286.dsp`
binaries now keep `CPU_IP` as a 32-bit `EIP`, mask it only for 16-bit code
segments, toggle effective operand/address size with `66h`/`67h`, use 32-bit
defaults from CS descriptors, and route `push`/`pop` through `ESP` when SS is a
32-bit stack segment.  The 32-bit opcode helper also no longer truncates near
and far 32-bit calls, jumps, returns, and protected interrupt target offsets to
16 bits.

This is still not enough to guarantee DOS/4GW or Doom compatibility.  Paging,
privilege transitions, task gates/TSS stack switching, call gates, DPMI/VCPI,
and missing 32-bit forms of less common opcodes remain likely blockers.

Build commands:

```powershell
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd C:\Work\r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286
.\tools\scan-download.ps1 .\homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `pico_286` size: 437,392 bytes
- `pico_286` SHA256:
  `E36ABD32EDB0697687802E5B446AAF85CE48B64270FD4784BDFF40CEAE2A6651`
- `pico_286.dsp` size: 431,740 bytes
- `pico_286.dsp` SHA256:
  `8D27534FA936910F7114C7EEF06CB4312EDF693BD6823829EE523A1F88A51886`
- Defender scan: found no threats
- Updated copies:
  - `disk_image/MIPS_NATIVE/pico_286/pico_286`
  - `disk_image/MIPS_NATIVE/pico_286/pico_286.dsp`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286`
  - `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286.dsp`

## 2026-05-31 native fast memory path

Added `R36SX_NATIVE_FAST_MEMORY`, enabled by default in both Pico-286 build
scripts.  In the R36SX native build, CPU `getmem*`/`putmem*` now use inline
fast helpers for conventional RAM and fall back to the existing `*_ob` memory
map for video RAM, EMS, BIOS ROM, UMB, HMA, and A20 wrapping.  The old
function-pointer memory backend remains available with
`--disable-fast-memory` in the WSL build wrapper.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `homebrew/pico_286/pico_286`
  - Size: 428,896 bytes
  - SHA256: `404C7D8D581EED25958640B34F06E3AFA333C08AB62FE55CABEBA9ABF7C248EF`
- `homebrew/pico_286/pico_286.dsp`
  - Size: 422,960 bytes
  - SHA256: `28D85715B02BFC4D400DD2A29690C598FE56C3EF0D96C59D8D0287AC718AF354`
- Defender scan: found no threats for the rebuilt binaries and their
  `disk_image` / `patches` copies.

## 2026-05-31 REP MOVS/STOS RAM block-copy path

Added a conservative fast path for repeated string operations in the R36SX
native build.  `REP MOVSB/MOVSW/MOVSD` now uses direct RAM block copies when
`DF=0` and both source and destination spans are entirely conventional RAM.
Overlapping forward copies are still handled byte/word/dword in x86 order, not
with `memmove`, so overlap semantics remain the same as the CPU instruction.

`REP STOSB/STOSW/STOSD` also bypasses per-byte `getmem`/`putmem` when the
destination is conventional RAM.  VGA, EMS, HMA, A20 wrapping, BIOS ROM, and
reverse-direction string ops continue to use the exact older memory path.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `homebrew/pico_286/pico_286`
  - Size: 431,456 bytes
  - SHA256: `03802F2B799638D3B4DFA21518E26880961B27DC43C1EF66AC1AF3B4F026B12C`
- `homebrew/pico_286/pico_286.dsp`
  - Size: 424,224 bytes
  - SHA256: `6792F3E53E786690776A18CD61D37FB45022DFE02E1A933A6BCFDED8022A70F7`
- Defender scan: found no threats for the rebuilt binaries and their
  `disk_image` / `patches` copies.

## 2026-05-31 segment base cache

Added `R36SX_SEGMENT_BASE_CACHE=1` to both Pico-286 build scripts.  The R36SX
CPU now keeps a parallel selector/base cache for `ES`, `CS`, `SS`, `DS`, `FS`,
and `GS`.  `segbase(selector)` resolves current segment selectors through this
cache before falling back to the protected-mode descriptor lookup helper.

The effective-address hot path also tracks `useseg_base` next to `useseg`, so
default segments and segment override prefixes update both selector and base.
`getea()` and `LEA` no longer need to call `segbase(useseg)` on their common
paths.

Rebuild commands:

```powershell
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --strip --out homebrew/pico_286/pico_286
wsl.exe --cd /mnt/c/Work/r36sx_disasm bash homebrew/pico_286/build_pico_286_wsl.sh --opt-level O3 --enable-mips-dsp --strip --out homebrew/pico_286/pico_286.dsp
```

Scan commands:

```powershell
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286
.\tools\scan-download.ps1 -Path homebrew\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path disk_image\MIPS_NATIVE\pico_286\pico_286.dsp
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286
.\tools\scan-download.ps1 -Path patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\pico_286.dsp
```

Result:

- `homebrew/pico_286/pico_286`
  - Size: 435,548 bytes
  - SHA256: `DCF5FE101B11F4FF09FA1DE6492A434FA1F32F415C197845A12EFC68ACF74CC8`
- `homebrew/pico_286/pico_286.dsp`
  - Size: 427,724 bytes
  - SHA256: `6C68272A6461B994D18970A93279D89D597157CDB67D7159F9862761A34B59D1`
- Defender scan: found no threats for the rebuilt binaries and their
  `disk_image` / `patches` copies.

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

The red disk activity LED originally used a small save/restore rectangle in
direct mode: the LED was drawn into `SCREEN` for the
`video_driver_disp_frame()` call, then the original pixels were restored
immediately.  Small overlays later changed to use a stable output frame instead
of restoring `SCREEN` right after handing the frame pointer to `driver.so`;
then later changed again to delayed rectangle restore to avoid full-frame
copies.

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
