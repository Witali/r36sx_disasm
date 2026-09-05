# Intel 80386 Instruction TODO

This list tracks gaps found while comparing the Pico-286 CPU core with the
Intel 80386 Programmer's Reference Manual instruction set.

The follow-up [x86 source audit](TODO_X86_SOURCE_AUDIT.md) tracks correctness
bugs, exception-path gaps and regression probes found on 2026-09-05. Completed
opcode entries below indicate implementation presence, not full conformance.

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
- [x] `LDS` / `LES`: add `r32, m16:32` forms for operand-size 32.
- [x] `CMPSD`: implement `A7` as dword compare when operand-size is 32.
- [x] `INS` / `OUTS`: implement `INSD` and `OUTSD` when operand-size is 32.
- [x] `MOV moffs`: make `A0..A3` use address-size for offset width and
      operand-size for 16/32-bit accumulator forms.
- [x] `XLAT`: use `EBX + AL` when address-size is 32.
- [x] `LOOP` / `LOOPcc` / `JCXZ`: use `ECX` for address-size 32, including
      `JECXZ`.

## Lower-priority conformance checks

- [x] Recheck x87 `WAIT/FWAIT` and `ESC D8..DF` behavior against 80386+80387:
      `EM`, `MP`, and `TS` now route to `#NM` where required; absent x87 keeps
      `WAIT/FWAIT` as no-op and decodes/skips `ESC` operands without touching
      memory or FPU state.
- [x] Emulate `DR0..DR3` execute-breakpoint `#DB` delivery and `DR6` `B0..B3`
      / `BS` status for instruction breakpoints and single-step traps.
- [x] Emulate `DR0..DR3` data write watchpoints with delayed `#DB` delivery
      after the instruction completes.
- [ ] Emulate `DR0..DR3` data read watchpoints; this needs data-read hooks that
      do not misclassify opcode/immediate fetches as watched reads.
- [ ] Add small instruction-level DOS tests for the fixed opcodes, especially
      flags and protected-mode exception paths.
