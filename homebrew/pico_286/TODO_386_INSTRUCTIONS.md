# Intel 80386 Instruction TODO

This list tracks gaps found while comparing the Pico-286 CPU core with the
Intel 80386 Programmer's Reference Manual instruction set.

## Missing 80386 opcodes

- [x] `0F A4` / `0F A5`: `SHLD r/m16|32, r16|32, imm8|CL`.
- [x] `0F AC` / `0F AD`: `SHRD r/m16|32, r16|32, imm8|CL`.
- [x] `0F B2`: `LSS r16|32, m16:16|32`.
- [x] `0F B4`: `LFS r16|32, m16:16|32`.
- [x] `0F B5`: `LGS r16|32, m16:16|32`.
- [x] `0F 21` / `0F 23`: `MOV r32, DRx` and `MOV DRx, r32`.
- [x] `0F 24` / `0F 26`: `MOV r32, TR6|TR7` and `MOV TR6|TR7, r32`.

## Incomplete operand/address-size behavior

- [x] `BOUND`: add 32-bit bounds-table form for operand-size 32.
- [ ] `LDS` / `LES`: add `r32, m16:32` forms for operand-size 32.
- [ ] `CMPSD`: implement `A7` as dword compare when operand-size is 32.
- [ ] `INS` / `OUTS`: implement `INSD` and `OUTSD` when operand-size is 32.
- [ ] `MOV moffs`: make `A0..A3` use address-size for offset width and
      operand-size for 16/32-bit accumulator forms.
- [ ] `XLAT`: use `EBX + AL` when address-size is 32.
- [ ] `LOOP` / `LOOPcc` / `JCXZ`: use `ECX` for address-size 32, including
      `JECXZ`.

## Lower-priority conformance checks

- [ ] Recheck x87 `WAIT/FWAIT` and `ESC D8..DF` behavior against 80386+80387
      after the integer core is closer to complete.
- [ ] Emulate debug-register breakpoint side effects (`DR0..DR3`, `DR6`,
      `DR7`) and `#DB`; current `MOV DRx` support only stores and reports the
      register state.
- [ ] Add small instruction-level DOS tests for the fixed opcodes, especially
      flags and protected-mode exception paths.
