# Similar Projects And Useful Links

This file collects links useful for researching `disk_image`, Hichip/HC16xx, iCube/cubegm, R36SX/GB350/SF3000, and related MIPS handheld platforms.

## Closest To Our Dump

### HCRTOS / Hichip SDK

- https://git.maschath.de/ignatz/hcrtos
  - Public HCRTOS/Hichip SDK tree.
  - A local copy already exists in `internet_sources\hcrtos`.
  - Important directories:
    - `components\applications\apps-projector\source\hcprojector_app`
    - `board\hc16xx\projector`
    - `configs\hichip_hc16xx_*_projector_*`
    - `components\hcfota`
    - `components\liblvgl`
  - Why it matters: it contains projector application sources and DTS/defconfig files for HC16xx projector platforms. `projector.c` contains the string `Welcome to Hichip world!`, matching strings from our `hcprojector`.

- https://git.maschath.de/ignatz/hcrtos/-/blob/7356e416d0d07a6f844faf89f433fb106677e9d8/configs/hichip_hc16xx_linux_bl_tiny_defconfig
  - Example HC16xx defconfig from the same HCRTOS tree.

### R36SX / GB350 / SF3000

- https://r36sx-hub.neocities.org/
  - R36SX / GB350 / SF3000 owner hub.
  - Useful as a link aggregator and for current community status.
  - Important matches with our analysis: MIPS SoC, iCube/cubegm firmware, stock OS only, incompatibility with ArkOS/JELOS/EmuELEC.

- https://www.reddit.com/r/R36S/comments/1qdqocz/does_anyone_have_a_backup_of_this_gb350_r36sx/
  - R36SX/GB350 clone discussion.
  - Claims that R36SX uses HiChip HC16xx, MIPS32_R2, Linux 4.4.186, U-Boot, and hichip_hc16xx drivers.
  - Matches our `dtb.bin` and `rootfs` well.

- https://github.com/LiamJ74/R36S-V2.6_Wiki
  - Wiki/tools for R36S-V2.6 / GB350 / R36SX-like cards.
  - Community reports mention `sync_sd_card` and `download_covers`.
  - Useful for restoring game lists, covers, and SD card structure.

- https://www.reddit.com/r/R36S/comments/1td4nxu/guide_r36sv260712_gb350_r36sx_how_to_add_games/
  - Practical guide for adding games and fixing the SD card for R36S-V2.6 / GB350 / R36SX.
  - Useful for understanding `allfiles.lst`, `filelist.csv`, covers, and the user-facing card area.

## Related Hichip/HCSEMI Handheld Platforms

These projects are not necessarily directly compatible, but they are useful as references for approaches: bootloader/BIOS patching, resource handling, game lists, themes, custom frontends, and MIPS binaries.

### SF2000

- https://github.com/vonmillhausen/sf2000
  - Large knowledge base for the Data Frog SF2000.
  - Useful as an example of reverse engineering a cheap Hichip/HCSEMI handheld.
  - Contains information on firmware, the bootloader bug, tools, resources, and modding.

- https://vonmillhausen.github.io/sf2000/
  - Web version of SF2000 documentation/tools.
  - Useful for quick access to tools and descriptions.

- https://github.com/Dteyn/Datafrog_SF2000_Vanilla
  - Stripped-down / vanilla SF2000 OS project.
  - Useful as an example of firmware-only / complete OS files / full image layouts.

- https://github.com/tzlion/frogtool
  - Game list rebuilding tool for SF2000.
  - May be useful as a reference for game-list formats, although R36SX/cubegm formats may differ.

- https://q-ta-s.github.io/sf2000.html
  - Set of SF2000/GB300 tools: themes, boot logo, and other small mods.

- https://grgadam.github.io/SF2000LinkTree/
  - Link tree for SF2000 firmware/tools.

### GB300

- https://github.com/nummacway/gb300
  - Information about the GB300 handheld.
  - Important because it discusses the relationship between GB300 and SF2000, firmware versions, multicore, and hardware.

- https://github.com/nummacway/gb300-sf2000-tool
  - All-in-one tool for GB300 and SF2000.
  - May be useful as an example GUI tool for themes, lists, bootloader patching, and firmware/resource management.

- https://retromods.pl/blog/gb300-setup
  - SF2000/GB300 first steps and multicore setup.

## Custom Frontends / CFW Initiatives For Related Devices

- https://github.com/axgdev/UniFrog
  - UniFrog CFW for SF2000/GB300.
  - Not directly suitable for our dump, but useful as a reference for custom firmware/frontend ideas on related cheap handhelds.

- https://github.com/axgdev/UniFrog/releases
  - UniFrog releases.

- https://www.reddit.com/r/DataFrogCentral/comments/1t4wvn1/alpha_testers_unifrog_v040_cfw_released/
  - UniFrog v0.4.x discussion.

- https://www.reddit.com/r/SBCGaming/comments/1oyqgwq/frogui_v010_released_a_modern_frontend_for_sf2000/
  - FrogUI: frontend/UI replacement for SF2000/GB300.
  - Useful as an example of replacing the stock UI without necessarily replacing the whole kernel/rootfs.

## Emulators / Additional Cores

- https://murmulator.ru/zxmurmulator
  - Murmulator ZX Spectrum firmware / `pico-spec`.
  - This is a rich ZX Spectrum/Pentagon/Byte/ALF emulator for RP2040/RP2350, but it is bare-metal Pico SDK firmware rather than a Linux/libretro `.so`.
  - Detailed local porting assessment: `MURMULATOR_PORTING_ANALYSIS.md`.

- https://github.com/DnCraptor/pico-spec
  - Murmulator/pico-spec sources.
  - Local copy downloaded to `internet_sources\murmulator\pico-spec-main`; the archive was scanned by Defender with no threats found.

- https://github.com/EremusOne/ESPectrum
  - Original ESPectrum project inherited by pico-spec.
  - May be useful for comparing the platform layer and finding less embedded-dependent parts.

- https://github.com/angree/sf2000-uae-amiga-emulator/releases
  - Amiga emulator for SF2000/GB300.
  - Useful as an example of adding a new emulator component to a similar ecosystem.

- https://github.com/madcock/sf2000_multicore_cores
  - Multicore cores for SF2000.
  - If available, inspect as an example of core build/adaptation.

## PC / DOS / x86 Emulators

- https://github.com/xrip/pico-286
  - Compact PC 8086/8088/80186/286 emulator for RP2040/RP2350/Murmulator.
  - Important for us: it has Linux/Windows host frontends, so the emulator core can be attempted as a MIPS native app with our framebuffer/input/audio.
  - Local copy: `homebrew\pico_286\pico-286`, commit `2ba88bf17de8e69c86f9256e22a241e8d31e8c96`, Defender scan passed without threats.
  - Detailed assessment: `PC_EMULATION_RESEARCH.md`.

- https://github.com/hchunhui/tiny386
  - C99 i386 PC emulator: CPU core plus PIC/PIT/keyboard/CMOS/VGA/IDE/NE2000/DMA, PC speaker, optional AdLib OPL2, and Sound Blaster 16.
  - Interesting as the closest 386 candidate, but heavier than `pico-286` in RAM, BIOS/VGABIOS, storage, and overall complexity.
  - Local copy: `internet_sources\tiny386`, commit `5cad7585d7a9e3081e8c5f50e3865fdbf09f5ebb`, Defender scan passed without threats.
  - Detailed assessment: `PC_EMULATION_RESEARCH.md`.

- https://github.com/schellingb/dosbox-pure
  - Libretro DOSBox fork with controller-friendly DOS game UX.
  - Could be useful as a source of ideas for a gamepad mapper/on-screen keyboard, but a direct port to our stock/libretro path looks riskier than a native app.

- https://github.com/dosbox-staging/dosbox-staging
  - Modern DOSBox fork/continuation.
  - Good upstream for standalone DOSBox, but porting it to our MIPS/Linux console would be significantly heavier than `pico-286`.

- https://github.com/joncampbell123/dosbox-x
  - Very complete DOSBox fork with broad DOS/Windows 3.x/9x coverage.
  - Useful as a reference project, but too large and complex for a first port.

- https://github.com/copy/v86
  - Browser/WASM x86 PC emulator.
  - Useful as a reference, but not as a direct MIPS native port target.

- https://github.com/86Box/86Box
  - Full x86 machine emulator focused on accuracy.
  - More of a reference project; it is too large for our console as a first step.

## Themes, Resources, Music, Images

- https://github.com/zerter555/SF2000_BGM
  - BGM examples/resources for SF2000.

- https://zerter555.github.io/sf2000-collection/
  - SF2000 theme/resource collections.

- https://q-ta-s.github.io/sf2000_theme.html
  - Theme tools/resources for SF2000/GB300.

## General Tools Useful For Our Analysis

- https://github.com/NationalSecurityAgency/ghidra
  - Ghidra decompiler/disassembler.
  - The workspace already contains `ghidra_12.0.4_PUBLIC`.
  - Used for MIPS ELF analysis: `hcprojector`, `rkgame`, `icube`, `cubevol`, `MyExecutable`.

- https://github.com/u-boot/u-boot
  - U-Boot source.
  - Useful for legacy `uImage` format, `mkimage`, load/entry/arch headers.

- https://github.com/dgibson/dtc
  - Device Tree Compiler.
  - Needed to decompile `dtb.bin` / `Bubbles.scr` to DTS and compare with HCRTOS DTS.

- https://github.com/ReFirmLabs/binwalk
  - Binwalk firmware analysis.
  - Useful for finding nested images, compressed payloads, and file systems.

## Notes

- SF2000/GB300 projects are useful methodological analogs, but they are not direct firmware replacements for the R36SX/GB350/SF3000-like `cubegm` dump.
- ArkOS, JELOS, EmuELEC, and most firmware for the real R36S usually target ARM/RK3326 or other platforms and do not fit this MIPS/Hichip dump.
- The most promising path for our device:
  1. HCRTOS SDK as a base for understanding Hichip HC16xx.
  2. Local `cubegm` as the soft-customization point.
  3. Our own MIPS32 LE userspace binary/frontend instead of `rkgame` or through `icube_start.sh`.
  4. Change DTB/uImage only after a complete backup and load/entry/SHA256 verification.
