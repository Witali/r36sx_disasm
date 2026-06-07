# Intel 80386 Instruction Audit

Date: 2026-06-02

This is a source audit of the R36SX Pico-286 CPU core against Intel 80386
documentation.  It is not a substitute for a full conformance run, but it
records the current instruction coverage, obvious gaps, and protected-mode
risks that should be worked through next.

## References

- Intel 64 and IA-32 Architectures Software Developer's Manual:
  https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- Intel 64 and IA-32 Architectures SDM Volume 2, Instruction Set Reference:
  https://www.intel.com/content/www/us/en/content-details/774492/intel-64-and-ia-32-architectures-software-developer-s-manual-combined-volumes-2a-2b-2c-and-2d-instruction-set-reference-a-z.html
- Intel 64 and IA-32 Architectures SDM Volume 3, System Programming Guide:
  https://www.intel.com/content/www/us/en/content-details/868145/intel-64-and-ia-32-architectures-software-developer-s-manual-combined-volumes-3a-3b-3c-and-3d-system-programming-guide.html
- Intel 80386 Programmer's Reference Manual, 1986 copy:
  https://pdos.csail.mit.edu/6.828/2018/readings/i386.pdf

## Code Map

- 386 operand-size dispatch: `r36sx_port/r36sx_cpu_80386.inl`
- Main 8086/286/386 opcode loop: `r36sx_port/r36sx_cpu.c`
- 286 descriptor helpers used by 386 protected mode:
  `r36sx_port/r36sx_cpu_80286.inl`
- Protected interrupt delivery:
  `r36sx_port/r36sx_cpu_80286_interrupts.inl`
- Protected-mode feature switch:
  `r36sx_port/r36sx_debug_config.h`

## Current Coverage

Implemented or substantially implemented:

- 66h operand-size override and 67h address-size override are gated to the
  80386 CPU model.
- 32-bit GPR data path exists for ALU, immediate ALU groups, register
  push/pop, `PUSH imm32`, `IMUL`, `MOV`, `XCHG`, `LEA`, `TEST`, `CALL`, `JMP`,
  `RET`, `IRETD`, `CWDE`, `CDQ`, `MOVSD`, `STOSD`, `LODSD`, and `SCASD`.
- 32-bit addressing includes SIB support and uses checked segmented linear
  translation in protected mode.
- `0F 80..8F` near conditional jumps are implemented.
- `0F 90..9F` `SETcc r/m8` is implemented.
- `0F 00` protected/system subfunctions include `SLDT`, `STR`, `LLDT`, `LTR`,
  `VERR`, and `VERW`.
- `0F 01` descriptor-table and machine-status operations include `SGDT`,
  `SIDT`, `LGDT`, `LIDT`, `SMSW`, and `LMSW`.
- `0F 02` `LAR` and `0F 03` `LSL` are implemented.
- `0F 06` `CLTS` is implemented with CPL checking.
- `0F 20` / `0F 22` control-register moves support `CR0`, `CR2`, and `CR3`.
- `0F A0/A1/A8/A9` `PUSH/POP FS/GS` are implemented.
- `0F A3/AB/B3/BB/BA` bit test and bit test-and-modify instructions are
  implemented.
- `0F AF` two-operand `IMUL`, `0F B6/B7` `MOVZX`, `0F BC/BD` `BSF/BSR`, and
  `0F BE/BF` `MOVSX` are implemented.
- Protected-mode memory reads and writes now go through segment limit/access
  checks before paging.
- Paging has a basic 80386 two-level model with `CR2`, `CR3`, present/user/write
  checks, and accessed/dirty updates.
- Protected far calls/jumps/returns/IRET have call gate, task gate, TSS, ring
  transition, and VM86 paths.
- Hardware task switching has 16-bit and 32-bit TSS save/load paths, busy-bit
  updates, backlink handling, `IRET` with `NT`, `CR3`, `LDTR`, segments, and
  `CR0.TS`.
- DPMI and VCPI host services are intentionally not part of the emulator core.
  Programs that need them should load guest DOS drivers or TSRs that install the
  appropriate interrupt interfaces.

## Missing 386 Instructions

These are official 80386 instructions that are not currently handled in the
`0F` decoder and fall through to invalid opcode:

- `0F A4` / `0F A5` - `SHLD r/m16/32, r16/32, imm8/CL`.
- `0F AC` / `0F AD` - `SHRD r/m16/32, r16/32, imm8/CL`.
- `0F B2` - `LSS r16/32, m16:16/32`.
- `0F B4` - `LFS r16/32, m16:16/32`.
- `0F B5` - `LGS r16/32, m16:16/32`.
- `0F 21` / `0F 23` - `MOV` from/to debug registers.
- `0F 24` / `0F 26` - `MOV` from/to test registers.  These are 386-era
  instructions and should either be implemented or explicitly rejected in the
  exact cases where Intel specifies `#UD` or privilege faults.

Not 80386 instructions and should remain invalid for a strict 386 model:

- `CPUID`, `RDTSC`, `CMPXCHG`, `XADD`, `BSWAP`, `INVD`, `WBINVD`, `CMOVcc`,
  `UD2`, and later 486/Pentium/MMX/SSE instructions.

## Protected-Mode Risks

The code is now much closer to a real 386 than before, but Intel's protected
mode is strict and many edge cases still need tests:

- Paging needs conformance review for supervisor writes, exact `#PF` error
  bits, and reserved-bit behavior for a 386-class CPU.
- Debug registers are not implemented.  This can break diagnostic tools and any
  program using hardware breakpoints.
- Test registers are not implemented.  They matter mostly for strict 386 test
  suites, not normal DOS games.
- DPMI and VCPI are no longer implemented in the emulator core.  DOS extenders
  that require those APIs need external guest-side hosts.
- `LOCK`, I/O privilege, VM86, interrupt reflection, and monitor behavior need
  conformance tests around every privileged path.
- TSS and gate code is substantial, but still needs selector/error-code tests
  against Intel's exact `#TS`, `#NP`, `#SS`, and `#GP` rules.
- `R36SX_CPU_COMPUTED_GOTO` is enabled by default on GCC/Clang builds.  This
  is a performance option rather than a 386 correctness gap.

## Suggested Order

1. Add `SHLD`/`SHRD`.  These are common in 386-targeted compiler output and
   are self-contained compared with protected-mode services.
2. Add `LSS`/`LFS`/`LGS`, including protected-mode selector validation and
   correct fault ordering.
3. Add 386 debug/test register moves, or document and test strict invalid
   behavior if we intentionally omit test registers.
4. Build a small 386 opcode conformance disk that explicitly exercises every
   386-only instruction above and records pass/fail output.
5. Add protected-mode exception tests for paging, gates, TSS, privilege
   transitions, VM86, and I/O bitmap paths.
