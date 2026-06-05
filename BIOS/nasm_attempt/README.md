# NASM Rebuild Attempt For Pico-286 BIOS

This directory records an experiment to rebuild the extracted Pico-286 BIOS ROM
with NASM.

## Result

Directly assembling `BIOS/pcxtbios_ghidra_full.s` does not work because the
Ghidra dump is a listing, not NASM source.  It contains address columns, raw
byte columns, Ghidra-specific mnemonics such as `STOSW.REP ES:DI`, and mixed
code/data decoding.

`pcxtbios_ndisasm_16.asm` is a Linux/WSL `ndisasm` listing produced with:

```sh
ndisasm -b 16 -o 0xE000 BIOS/pcxtbios.bin \
  > BIOS/nasm_attempt/pcxtbios_ndisasm_16.asm
```

It is closer to NASM syntax than the Ghidra dump, but it is still a listing,
not a ready source file.  It also linearly decodes banner strings and tables as
instructions until a human marks those ranges as data.

The working first step is:

- `pcxtbios_from_ghidra_bytes.asm`

This file is a byte-preserving NASM source.  It emits the BIOS bytes with `db`
directives and preserves the Ghidra disassembly as comments.  It is not yet a
symbolic, hand-maintainable BIOS source, but it proves that the extracted ROM
can be rebuilt through NASM.

## Build

```sh
nasm -f bin BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.asm \
  -o BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

## Verification

The rebuilt file is byte-identical to `BIOS/pcxtbios.bin`:

```text
8192 BIOS/pcxtbios.bin
8192 BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
cmp_identical
468396458c74542e6fdf675fa53e9552b3037a5d6119e3232f486e8543922b96  BIOS/pcxtbios.bin
468396458c74542e6fdf675fa53e9552b3037a5d6119e3232f486e8543922b96  BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

## Next Step

The next useful step is to replace important `db` ranges with real NASM
instructions and labels, one BIOS block at a time:

1. Reset entry at `F000:E05B`.
2. Hardware init / PIT / DMA / PIC setup.
3. RAM probe and clear loop.
4. IVT table setup.
5. Boot path and `INT 19h`.
6. Individual BIOS interrupt handlers.
