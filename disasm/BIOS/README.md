# Pico-286 Embedded BIOS ROM

This directory contains the 8 KiB BIOS ROM extracted from the embedded
`BIOS[]` byte array used by the Pico-286 emulator port.

## Files

- `pcxtbios.bin` - raw 8 KiB BIOS image extracted from
  `homebrew/pico_286/pico-286/src/emulator/includes/bios.h`.
  SHA-256: `468396458C74542E6FDF675FA53E9552B3037A5D6119E3232F486E8543922B96`.
- `pcxtbios_ghidra_full.s` - Ghidra linear disassembly of the full ROM range.
- `pcxtbios_ghidra_reset.s` - Ghidra disassembly starting at the reset entry.
- `pcxtbios_init_annotated.asm` - hand-annotated init/reset notes.
- `make_pcxtbios_init_nasm.ps1` - generator for the NASM rebuild source above.
- `ghidra_pcxtbios/` - Ghidra summary, symbols, function list, disassembly,
  and decompiler output exported by the existing project script.

The generated NASM source is kept with the BIOS disassembly notes:

```text
disasm/BIOS/nasm_attempt/pcxtbios_init_annotated_nasm.asm
```

It converts leading `F000:XXXX` listing addresses into `loc_XXXX` labels and
preserves unannotated ROM gaps from `pcxtbios.bin`.

## Addressing

The BIOS ROM image starts at physical address `FE000h`. The emulator maps the
8 KiB image at physical `FE000h..FFFFFh`.
Ghidra imports it as a 16-bit real-mode segment range:

```text
F000:E000 .. F000:FFFF
```

Therefore:

```text
file offset = ghidra offset - E000h
physical    = F0000h + ghidra offset
```

The x86 reset vector at `FFFF0h` is at file offset `1FF0h` and contains:

```asm
F000:FFF0  ea 5b e0 00 f0    jmp far F000:E05B
```

So the BIOS reset/init entry point is `F000:E05B`.
In physical-address form that reset/init entry is `FE05Bh`.

## What This BIOS Is

This is a compact Turbo XT / Murmulator-style BIOS, not a complete AT/386
BIOS. It performs XT-class startup work directly in ROM:

- disables interrupts and sets direction forward;
- initializes display-related ports, PIT, DMA, and PIC;
- tests and clears conventional RAM up to video memory;
- sets BIOS Data Area values such as memory size, equipment flags,
  keyboard buffer pointers, LPT/COM addresses, and timer state;
- builds the IVT entries for `INT 08h..1Fh`;
- scans option ROM space beginning at `C000h`;
- initializes video and prints POST/status text;
- finally calls `INT 19h` to boot.

AT/386-era services that this ROM does not really contain are currently
provided by the emulator-side BIOS interrupt shims in:

```text
homebrew/pico_286/r36sx_port/r36sx_cpu.c
```

That includes extended memory discovery, A20, E801/E820, VBE, and RTC work.

## IVT Table Installed By The ROM

During reset the ROM fills vectors `00h..07h` with a default stub at
`F000:FF23`, then copies the following table for `INT 08h..1Fh`:

```text
08 -> F000:FEA5  timer tick / IRQ0 path
09 -> F000:E987  keyboard IRQ / scan-code path
0A -> F000:FF23  default IRET/stub
0B -> F000:FF23  default IRET/stub
0C -> F000:FF23  default IRET/stub
0D -> F000:FF23  default IRET/stub
0E -> F000:EF57  diskette IRQ/service helper
0F -> F000:FF23  default IRET/stub
10 -> F000:F065  video BIOS services
11 -> F000:F84D  equipment list
12 -> F000:F841  conventional memory size
13 -> F000:EC59  disk services
14 -> F000:E739  serial services
15 -> F000:F859  minimal/unsupported system services
16 -> F000:E82E  keyboard BIOS services
17 -> F000:EFD2  printer services
18 -> F000:FF23  ROM BASIC/default path
19 -> F000:E6F2  bootstrap loader
1A -> F000:FE6E  time-of-day services
1B -> F000:FF53  Ctrl-Break vector
1C -> F000:FF53  timer user hook
1D -> F000:F0A4  video parameter table
1E -> F000:EFC7  diskette parameter table
1F -> F000:0000  graphics character table pointer placeholder
```

## Reproduction Commands

The raw ROM was generated from `bios.h` by parsing every `0xNN` byte literal
and writing them in order to `disasm/BIOS/pcxtbios.bin`.

Ghidra was run as:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat `
  .\ghidra_projects pico286_pcxtbios_export `
  -import .\disasm\BIOS\pcxtbios.bin `
  -loader BinaryLoader `
  -loader-baseAddr 0xfe000 `
  -processor "x86:LE:16:Real Mode" `
  -scriptPath .\ghidra_scripts `
  -postScript DumpDisasmRanges.java .\disasm\BIOS\pcxtbios_ghidra_full.s 0xFE000 0x2000 `
  -postScript ExportDisasmAndDecompile.java .\disasm\BIOS\ghidra_pcxtbios `
  -deleteProject
```

The annotated init notes can be converted into a compilable NASM source and
rebuilt as a byte-identical ROM:

```powershell
powershell -ExecutionPolicy Bypass -File .\disasm\BIOS\make_pcxtbios_init_nasm.ps1
wsl sh -lc 'cd /mnt/c/Work/r36sx_disasm && nasm -f bin disasm/BIOS/nasm_attempt/pcxtbios_init_annotated_nasm.asm -o disasm/BIOS/nasm_attempt/pcxtbios_init_annotated.bin'
wsl sh -lc 'cd /mnt/c/Work/r36sx_disasm && cmp -s disasm/BIOS/pcxtbios.bin disasm/BIOS/nasm_attempt/pcxtbios_init_annotated.bin && echo cmp_identical'
```
