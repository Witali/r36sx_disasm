# VGA Device TODO

This file preserves the earlier working note about the VGA device model that
Pico-286 should emulate. It is a practical checklist for making DOS programs
detect and use VGA hardware more like a real IBM-compatible PC.

## Hardware Map

### Memory Areas

- `A0000h..AFFFFh`: primary graphics window for EGA/VGA modes. In standard VGA
  this maps into four 64 KiB planes, with planar, chained, odd/even, and packed
  interpretations controlled by sequencer and graphics-controller registers.
- `B0000h..B7FFFh`: monochrome text / MDA / Hercules-compatible window.
- `B8000h..BFFFFh`: color text, CGA-compatible graphics, and related color
  display modes.
- `C0000h..C7FFFh`: typical VGA BIOS ROM area. Programs may probe for the ROM
  signature and strings here, though most DOS programs use `INT 10h` instead.

### I/O Ports

- `3C0h`: Attribute Controller index/data. Reads of Input Status Register 1
  (`3BAh` or `3DAh`) reset the attribute-controller flip-flop.
- `3C1h`: Attribute Controller data read.
- `3C2h`: Miscellaneous Output write; Input Status Register 0 read.
- `3C3h`: VGA subsystem enable on some adapters.
- `3C4h/3C5h`: Sequencer index/data.
- `3C6h`: DAC pixel mask.
- `3C7h`: DAC read index / DAC state read.
- `3C8h`: DAC write index.
- `3C9h`: DAC data. Standard VGA DAC components are 6-bit values per channel.
- `3CAh`: Feature Control read on some VGA-compatible mappings.
- `3CCh`: Miscellaneous Output read.
- `3CEh/3CFh`: Graphics Controller index/data.
- `3B4h/3B5h`: Monochrome CRTC index/data.
- `3D4h/3D5h`: Color CRTC index/data.
- `3BAh`: Monochrome Input Status Register 1.
- `3DAh`: Color Input Status Register 1. Important bits:
  - bit 0: display-enable / horizontal timing style status;
  - bit 3: vertical retrace.

### IRQ Behavior

Classic VGA software usually polls `3DAh` instead of relying on a VGA IRQ.
Some EGA/VGA hardware could expose vertical retrace interrupt behavior through
IRQ2/IRQ9 paths, but most DOS games and utilities do not require it. For the
emulator, a realistic timed `3DAh` is more important than adding a VGA IRQ.

## How DOS Programs Detect VGA

- BIOS `INT 10h AX=1A00h`: Display Combination Code. This is the clean
  BIOS-level way to report EGA/VGA-class hardware.
- VBE `INT 10h AX=4F00h`: VESA BIOS Extensions information block, for SVGA/VBE
  programs.
- Direct port probes: programs may write/read Sequencer, Graphics Controller,
  CRTC, Attribute Controller, and DAC registers.
- DAC probes: writing and reading `3C7h/3C8h/3C9h` is a common way to test VGA
  palette behavior.
- Memory probes: programs may test `A0000h`, `B0000h`, and `B8000h` behavior.
- VGA BIOS ROM probes: some utilities inspect `C0000h` for a ROM signature or
  adapter strings.

## Emulator TODO

### Register Reset And Mode Set

- Reset VGA registers to BIOS-compatible defaults when switching modes through
  `INT 10h`, especially after non-standard VGA modes.
- Reset the Attribute Controller flip-flop on `3DAh`/`3BAh` reads.
- Track CRTC base selection from the Miscellaneous Output register.
- Keep Sequencer, Graphics Controller, CRTC, Attribute Controller, and DAC state
  separate instead of collapsing everything into a mode number.

### Status Timing

- Keep `3DAh` timing independent from renderer execution.
- Model bit 3 as a vertical-retrace window at the configured refresh rate.
- Model bit 0 as a fast display-enable style signal so programs waiting for
  display disable/retrace do not stall.
- Validate with Volkov Commander and other programs that poll `3DAh` for CGA
  snow avoidance or retrace synchronization.

### VGA Memory Access

- Implement standard VGA latches for reads and writes.
- Implement Graphics Controller write modes `0..3`.
- Implement set/reset, enable set/reset, bit mask, data rotate, raster op, read
  map select, and read mode `0/1`.
- Support Sequencer map mask, chain-4, odd/even, and plane selection.
- Keep fast paths for simple packed or text modes, but preserve correct planar
  semantics for VGA/EGA programs.

### Palette And DAC

- Store the programmer-visible DAC palette as original 6-bit RGB values.
- Keep a shadow converted palette, such as RGB565, for fast rendering.
- Reads from DAC ports must return original DAC values, not the converted
  shadow palette.
- Respect Attribute Controller palette indexes, overscan/border color, and DAC
  pixel mask.

### CRTC And Display Addressing

- Implement start address, cursor address/shape, line compare, offset/pitch,
  maximum scanline, underline location, double-scan, and byte/word/dword address
  modes.
- Handle 8-dot and 9-dot text rendering correctly.
- Make display dimensions derive from CRTC/Sequencer/Graphics state where
  possible, not only from the BIOS mode number.

### Text Modes

- Support loading fonts into plane 2.
- Support 8x8, 8x14, and 8x16 text fonts.
- Implement blink versus bright-background behavior.
- Keep cursor rendering and blink behavior compatible enough for DOS shells,
  Norton/Volkov-style managers, and installers.

### VBE/SVGA

- Keep minimal VBE modes implemented for our display:
  - `101h`: 640x480x8;
  - `111h`: 640x480x16;
  - `103h`: 800x600x8;
  - `114h`: 800x600x16.
- Complete VBE info and mode-info blocks: pitch, bank granularity, window size,
  memory model, DAC width, pages, and capabilities.
- Implement `4F05h` bank switching, `4F06h` logical scanline, and `4F09h`
  palette services where practical.

### Compatibility Tests

- Supaplex: stresses non-standard VGA/EGA register programming.
- 3DBench: useful for mode and palette correctness.
- SuperScape benchmark: catches stale VGA register state after another program
  used a non-standard mode.
- Volkov Commander: confirms realistic `3DAh` timing and text-mode behavior.
- FASTVID / MTRRLFBE style tools: useful for checking detection behavior, even
  when their acceleration semantics are only stubbed.

## Notes

- For this emulator, the priority order should be:
  1. Correct direct VGA register behavior for common DOS games.
  2. Correct `3DAh` timing and palette/DAC behavior.
  3. Correct planar memory write/read modes.
  4. Better CRTC-derived rendering for tweaked modes.
  5. Optional VGA IRQ behavior only if a real program requires it.

## References

- Michael Abrash, *Graphics Programming Black Book*, Chapter 47,
  "Mode X: 256-Color VGA Magic":
  https://www.phatcode.net/res/224/files/html/ch47/47-02.html
  - Use for the canonical BIOS `13h` -> Mode X register setup sequence:
    Sequencer Memory Mode `0604h`, synchronous reset, Misc Output clock
    selection, CRTC unlock/reprogramming, map mask `0F02h`, and full VRAM
    clear.
- Michael Abrash, *Graphics Programming Black Book*, Chapter 47 pixel layout:
  https://www.phatcode.net/res/224/files/html/ch47/47-03.html
  - Use for Mode X address math: byte offset `x / 4`, plane `x mod 4`, map
    mask register `3C4h:02h`, and read-map register `3CEh:04h`.
- Michael Abrash, *Graphics Programming Black Book*, Chapter 48,
  "Mode X Marks the Latch":
  https://www.phatcode.net/res/224/files/html/ch48/48-01.html
  - Use for VGA latch behavior: a display-memory read loads one byte from each
    of the four planes into the latches, and later writes can use those latches
    for four-pixel copies/fills.
- OSDev Wiki, "VGA Hardware":
  https://wiki.osdev.org/VGA_Hardware
  - Use as a compact register/memory map reference for VGA ports, indexed
    register access, DAC behavior, memory windows, chain-4, latches, and sample
    register settings.
- FreeVGA, "VGA Sequencer Registers":
  https://www.osdever.net/FreeVGA/vga/seqreg.htm
  - Use for Sequencer register definitions, especially Map Mask register `02h`
    and Memory Mode register `04h`.
- FreeVGA, "VGA Sequencer Operation":
  https://www.osdever.net/FreeVGA/vga/vgaseq.htm
  - Use for display-side Sequencer behavior and 256-color shift-mode details.
