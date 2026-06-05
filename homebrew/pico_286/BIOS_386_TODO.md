# 386-Class BIOS TODO

This file preserves the earlier research answer to:

> Search what BIOS functions should exist for a 386 PC and which interrupts are
> necessary.

The main conclusion is that there is no separate universal "386 BIOS" API.
A 386-era DOS machine is still mostly a PC/AT-compatible BIOS plus VGA/VBE and
memory-reporting extensions.  The CPU protected-mode behavior itself belongs in
the x86 core, while DOS extenders, DPMI, VCPI, XMS, and EMS should normally be
guest-side drivers or TSRs rather than emulator-side BIOS services.

## References

- Intel 80386 Programmer's Reference Manual, 1986:
  https://www.read.seas.harvard.edu/~kohler/class/aosref/i386.pdf
- Current Intel Software Developer Manuals:
  https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- ACPI specification, System Address Map Interfaces / INT 15h E820h:
  https://uefi.org/specs/ACPI/6.5/15_System_Address_Map_Interfaces.html
- Ralf Brown's Interrupt List, BIOS index:
  https://fd.lod.bz/rbil/interrup/bios/index.html
- RBIL INT 15h AH=87h, copy extended memory:
  https://fd.lod.bz/rbil/interrup/bios/1587.html
- RBIL INT 15h AH=88h, get extended memory size:
  https://fd.lod.bz/rbil/interrup/bios/1588.html
- RBIL INT 15h AH=89h, BIOS switch to protected mode:
  https://fd.lod.bz/rbil/interrup/bios/1589.html
- RBIL INT 15h AH=C7h, PS/2 memory-map information:
  https://fd.lod.bz/rbil/interrup/bios/15c7.html
- RBIL INT 15h AH=C9h, CPU type and mask revision:
  https://fd.lod.bz/rbil/interrup/bios/15c9.html
- VESA BIOS Extension Core Functions Standard 2.0:
  https://docslib.org/doc/7558269/vesa-bios-extension-vbe-core-functions-standard-version

## Scope Boundary

### Belongs In BIOS Or Emulator BIOS Shims

- Real-mode PC/AT BIOS interrupt services used before protected-mode entry.
- BIOS Data Area and optional EBDA values that DOS and BIOS callers inspect.
- CMOS/RTC ports and `INT 1Ah` time/date services.
- Disk, keyboard, timer, video, memory-size, and boot services.
- VBE real-mode interface for DOS software that asks video BIOS for SVGA modes.

### Belongs In The CPU Core, Not BIOS

- CR0.PE and CR0.PG behavior.
- GDT, LDT, IDT, TSS, paging, VM86, privilege checks, and exceptions.
- Correct protected-mode interrupt/trap/task gate behavior.
- `#GP`, `#SS`, `#NP`, `#TS`, `#PF`, and exact error-code generation.

### Belongs In Guest DOS Drivers Or TSRs

- DPMI: `INT 31h` host interface and `INT 2Fh AX=1687h` discovery.
- VCPI: `INT 67h` protected-mode services.
- XMS/HIMEM: `INT 2Fh AX=4310h` driver discovery and XMS entry point.
- EMS/EMM386: `INT 67h` EMS services and UMB management.
- Network redirector style DOS hooks, except for a hardware-like host RPC port.

## Required BIOS Interrupts And Data

### Priority 1: Bootable PC/AT Baseline

- `INT 10h`: video BIOS services.
  - Required: mode set/get, cursor set/get, teletype output, scroll/clear,
    character/attribute read-write, display combination code `AX=1A00h`.
  - For our VGA work, keep direct register behavior consistent with `INT 10h`.
- `INT 13h`: disk BIOS services.
  - Required: reset, read/write sectors, get drive parameters, status.
  - Desirable: EDD install check and extended read/write (`AH=41h`, `42h`,
    `43h`, `48h`) for newer tools, though many DOS programs still use CHS.
- `INT 16h`: keyboard BIOS services.
  - Required: read key, check key, shift flags.
  - Desirable: extended keyboard calls `AH=10h`, `11h`, `12h`.
- `INT 1Ah`: timer and RTC services.
  - Required: time-of-day tick read/set.
  - Required for AT-like behavior: RTC time/date read/set.
- `INT 11h`: equipment word.
- `INT 12h`: conventional memory size.
- `INT 18h`: ROM BASIC/no-boot fallback.
- `INT 19h`: bootstrap loader.
- Data vectors/tables:
  - `INT 1Dh`: video parameter table.
  - `INT 1Eh`: diskette parameter table.
  - `INT 41h` / `INT 46h`: fixed-disk parameter tables when CHS HDDs are
    reported through classic BIOS services.

### Priority 2: 286/386 Memory And Platform Services

- `INT 15h AH=86h`: wait in microseconds.  Useful for installers and timing
  loops that expect AT BIOS behavior.
- `INT 15h AH=87h`: copy extended memory block.  Required by some 286/386-era
  software and memory managers.
- `INT 15h AH=88h`: report contiguous extended memory above 1 MiB.  Keep this
  consistent with the configured memory map and below-16-MiB compatibility.
- `INT 15h AX=E801h`: report memory above 1 MiB in the later AT-compatible
  format.  Keep it consistent with E820.
- `INT 15h EAX=E820h`: system address map.  This is the best modern way to
  describe usable and reserved physical address ranges.
- `INT 15h AX=2400h..2403h`: A20 gate disable, enable, query, and support.
- `INT 15h AH=C0h`: get system configuration.  Optional but useful for probes.
- `INT 15h AH=C1h`: return EBDA segment.  Optional but useful if we reserve an
  EBDA-like area.
- `INT 15h AH=C9h`: CPU type and mask revision.  Optional; useful for tools
  that probe 386/486 class through BIOS rather than CPU instructions.
- `INT 15h AH=89h`: BIOS switch to protected mode.  Optional and rare in DOS
  games, but document any unsupported return carefully if not implemented.

### Priority 3: VGA/VBE Compatibility

- `INT 10h AX=4F00h`: return VBE controller information.
- `INT 10h AX=4F01h`: return VBE mode information.
- `INT 10h AX=4F02h`: set VBE mode.
- `INT 10h AX=4F03h`: return current VBE mode.
- `INT 10h AX=4F05h`: window/bank control if banked VBE modes are exposed.
- `INT 10h AX=4F06h`: logical scanline length.
- `INT 10h AX=4F07h`: display start.
- `INT 10h AX=4F08h`: DAC width get/set.
- `INT 10h AX=4F09h`: palette data.
- `INT 10h AX=4F0Ah`: protected-mode interface.  Optional; only implement if
  a target program is known to request it.

## Hardware/Port State BIOS Should Expose

- BDA at `0040:0000`, including equipment flags, COM/LPT bases, keyboard
  buffer, video state, timer ticks, and diskette/HDD status bytes.
- Optional EBDA segment pointer at BDA `0040:000Eh`.
- CMOS/RTC ports `70h/71h`, including time/date/status registers.
- Keyboard controller ports `60h/64h`, at least enough for reset/A20 probes.
- Fast A20 gate port `92h` if enabled for AT/386-style compatibility.
- PIC, PIT, DMA state consistent enough for BIOS IRQ handlers and DOS probes.
- VGA ports and memory windows as tracked in `VGA_TODO.md`.

## Implementation Checklist For Pico-286

1. Audit current `r36sx_cpu.c` BIOS shims against the Priority 1 list.
   - Done when DOS boot, keyboard, disk, timer, and basic video calls have
     clear supported/unsupported behavior and debug logging for failures.

2. Make all memory-reporting paths share one memory-map source.
   - `INT 12h`, `INT 15h AH=88h`, `E801h`, `E820h`, and config comments must
     agree about conventional, reserved, upper, HMA, and extended memory.

3. Keep E820 authoritative.
   - If E820 disagrees with older calls, fix the older calls so their
     compatibility values do not contradict the E820 map.

4. Review A20 behavior end-to-end.
   - `INT 15h AX=240x`, port `92h`, keyboard-controller-style toggles, and
     memory wrapping must all report the same state.

5. Review RTC/CMOS behavior.
   - DOS file timestamps, `DATE`, `TIME`, screenshots, and BIOS `INT 1Ah`
     should use the same emulated RTC source.

6. Decide whether to expose EBDA.
   - If yes, reserve it explicitly below 640 KiB, report it in BDA, and make
     memory reports account for it.

7. Add EDD only if a target needs it.
   - Classic CHS is enough for the current HDD images, but EDD improves
     compatibility with newer DOS tools and bootloaders.

8. Keep DPMI/VCPI out of emulator BIOS.
   - Use guest-side drivers/hosts for DPMI, VCPI, XMS, EMS, and UMB linking.

