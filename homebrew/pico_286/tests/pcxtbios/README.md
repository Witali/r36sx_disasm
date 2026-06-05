# Pico-286 Embedded PC/XT BIOS Source

This folder stores the NASM rebuild source for the embedded 8 KB BIOS ROM used
by Pico-286 in normal BIOS mode.

## Source

- `pcxtbios_from_ghidra_bytes.asm`

This is a byte-preserving source file.  It emits the original ROM with `db`
directives, while preserving disassembly comments from the Ghidra analysis.
It is intentionally kept byte-identical to `BIOS/pcxtbios.bin` until individual
BIOS blocks are manually converted to symbolic NASM code.

## Build

```sh
nasm -f bin homebrew/pico_286/tests/pcxtbios/pcxtbios_from_ghidra_bytes.asm \
  -o BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

Verify:

```sh
cmp -s BIOS/pcxtbios.bin BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
sha256sum BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

Expected SHA-256:

```text
468396458c74542e6fdf675fa53e9552b3037a5d6119e3232f486e8543922b96
```

