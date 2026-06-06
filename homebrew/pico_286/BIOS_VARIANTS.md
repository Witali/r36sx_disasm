# BIOS Variants Considered for Pico-286/R36SX

Last reviewed: 2026-06-06.

This note collects the BIOS candidates that were considered while improving the
Pico-286 port.  The emulator currently uses a compact Turbo XT compatible BIOS
image and implements several AT/386-era services in the emulator-side BIOS
interrupt handlers.  Replacing the ROM wholesale is possible, but every BIOS has
different assumptions about chipset, timers, CMOS, keyboard controller, disk
controller, option ROMs, and VGA services.

## Best Current Fit

### Embedded Turbo XT / `pcxtbios.asm`

- Local source: `homebrew/pico_286/bios/pcxtbios.asm`
- Origin: `xrip/pico-xt` BIOS source, based on the Turbo XT BIOS lineage.
- Upstream link: https://github.com/xrip/pico-xt/blob/master/bios/pcxtbios.asm
- Target class: PC/XT, 8088/V20-style BIOS.
- Strengths:
  - Small and already integrated.
  - Fits our current reset path and FE000h ROM mapping.
  - Good enough for booting DOS when the emulator supplies missing services.
- Weaknesses:
  - Not a real AT/386 BIOS.
  - No full AT CMOS/setup flow.
  - No native 386 protected-mode services beyond what we emulate outside the ROM.
- Project recommendation:
  - Keep it as the stable default for now.
  - Continue adding emulator-side BIOS services: `INT 10h`, `INT 13h`,
    `INT 15h`, `INT 1Ah`, A20, memory reporting, and VGA/VBE compatibility.

## XT-Class Open BIOS Candidates

### GLaBIOS

- Main site: https://glabios.org/
- Source: https://github.com/640-KB/GLaBIOS/tree/main/src
- Target class: PC, XT, 8088 clone, Turbo XT, homebrew 8088 systems.
- License: GPLv3 according to the project site.
- Strengths:
  - Modern scratch-built open source BIOS.
  - Designed for PC/XT compatibility and emulator use.
  - Compact 8 KiB ROM goal, useful POST screen, configurable features.
- Weaknesses:
  - Still XT-class, not AT/386-class.
  - Does not remove our need to emulate AT memory, CMOS, A20, and 386 behavior.
- Project recommendation:
  - Good candidate for an alternate XT BIOS experiment.
  - Good reference for clean PC/XT BIOS behavior and POST diagnostics.
  - Not enough by itself for DOS/4GW or 386 protected-mode software.

### Sergey Kiselev `8088_bios`

- Related board repo: https://github.com/skiselev/micro_8088
- The Micro 8088 page points to a separate `8088_bios` repository for BIOS
  source.
- Target class: IBM PC/XT compatible 8088 boards such as Micro 8088, Xi 8088,
  Sergey's XT, NuXT, and similar systems.
- Strengths:
  - Mature real-hardware-oriented XT BIOS work.
  - Useful reference for keyboard, floppy, fixed disk, RTC add-ons, and POST on
    small systems.
- Weaknesses:
  - Hardware-project-specific.
  - XT-class, not AT/386-class.
- Project recommendation:
  - Good reference material for XT services.
  - Less attractive as a direct drop-in unless we match its expected board
    hardware or adapt the platform layer.

## AT/386-Class BIOS Candidates

### SeaBIOS

- Source: https://github.com/coreboot/seabios
- Official upstream mirror: https://git.seabios.org/seabios.git
- Target class: x86 legacy BIOS, commonly used by QEMU and as a coreboot
  payload.
- Strengths:
  - Real open source legacy x86 BIOS.
  - Designed for 386+ class execution and modern emulator/virtual-machine
    workflows.
  - Contains system BIOS and VGA BIOS source areas.
- Weaknesses:
  - Much larger and more complex than our current ROM.
  - Expects a QEMU/coreboot-like machine model unless configured/adapted.
  - Likely requires more complete chipset, PCI/option-ROM, CMOS, disk, and VGA
    behavior than we currently emulate.
- Project recommendation:
  - Best long-term reference for a proper 386-capable BIOS.
  - Not a near-term drop-in until our chipset/device model is closer to a PC/AT
    or QEMU-compatible machine.
  - Useful for studying `INT 10h`, memory reporting, boot flow, option ROM
    scanning, and VBE behavior.

### Bochs BIOS / i440fx BIOS

- Bochs source: https://github.com/bochs-emu/Bochs
- Bochs organization: https://github.com/bochs-emu
- i440fx BIOS notes: https://fysnet.github.io/i440fx/
- Target class: Bochs/QEMU-style emulated PC.
- Strengths:
  - Written for emulators.
  - Historically important reference for PC-compatible BIOS behavior.
  - The newer i440fx direction is an open adaptation with fixes and enhancements.
- Weaknesses:
  - Expects Bochs/i440fx-style virtual hardware.
  - Larger integration surface than our current embedded BIOS.
- Project recommendation:
  - Useful reference when implementing AT/386 BIOS services.
  - Possible future alternate ROM only if we add the expected chipset model.

### Bochs VGABIOS

- Source: https://github.com/bochs-emu/VGABIOS
- Target class: VGA option ROM, not a full system BIOS.
- Strengths:
  - Focused reference for VGA BIOS calls, register setup, and VBE behavior.
  - More directly relevant to Doom/Mode X/VBE issues than a full system BIOS.
- Weaknesses:
  - Not a boot BIOS.
  - Still assumes a VGA device model compatible with its register expectations.
- Project recommendation:
  - Strong reference for our `INT 10h` and VBE implementation.
  - A real option-ROM integration could be investigated later, but matching the
    VGA register model comes first.

### coreboot

- Source: https://github.com/coreboot/coreboot
- Project: https://www.coreboot.org/
- Target class: firmware framework that initializes real hardware, then runs a
  payload such as SeaBIOS.
- Strengths:
  - Open firmware ecosystem.
  - SeaBIOS payload workflow is relevant for real machines.
- Weaknesses:
  - Not a DOS BIOS by itself.
  - Far too hardware/platform-heavy for our emulator target.
- Project recommendation:
  - Not a direct candidate.
  - Mentioned only because SeaBIOS is often used as a coreboot payload.

## Historical / Reference-Only Material

### IBM PC/AT BIOS Listings

- Historical IBM technical references include BIOS listings for original IBM PC,
  XT, AT, and related machines.
- Target class: original IBM hardware.
- Strengths:
  - Excellent behavioral reference for BIOS interrupt semantics and boot flow.
- Weaknesses:
  - Copyrighted historical material; do not copy code into this project.
  - AT BIOS is not directly the same as a generic 386 clone BIOS.
- Project recommendation:
  - Use only as a specification/behavior reference.

### PCjs Annotated 386 ROM Work

- PCjs article: https://www.pcjs.org/blog/2015/04/16/
- Target class: Compaq DeskPro 386 ROM analysis.
- Strengths:
  - Useful for understanding real 386 POST/BIOS behavior.
- Weaknesses:
  - ROM-derived material; not suitable as code to copy.
- Project recommendation:
  - Reference only.

### `appendix-bios`

- Mentioned in prior research as an IBM AT/80286 BIOS codebase candidate.
- Concern:
  - Licensing was unclear in the material found during research.
- Project recommendation:
  - Avoid direct use until the license is confirmed.
  - Can be revisited only as research/reference material.

## Practical Direction for This Project

1. Keep the current Turbo XT BIOS as default because it is small and already
   proven in our boot path.
2. Treat SeaBIOS and Bochs BIOS/VGABIOS as implementation references, especially
   for `INT 10h`, `INT 15h`, option ROM scanning, memory maps, VGA, and VBE.
3. Do not try to replace the ROM with SeaBIOS until the emulator has a more
   complete AT/386 chipset model.
4. For near-term DOS/4GW and Doom compatibility, improve emulator-side CPU,
   VGA, memory, and BIOS interrupt behavior rather than swapping BIOS first.
5. Consider GLaBIOS only as an alternate XT BIOS experiment, not as a solution
   for 386 protected-mode software.

