# Intel 386 Protected-Mode TODO

This file tracks the remaining Intel 80386/protected-mode work for the R36SX
Pico-286 port.  Work through the list top-to-bottom unless a later item becomes
a direct blocker for the current test program.  Each implementation step should
be checked against the relevant specification and committed separately.

## References

- Intel 80386 Programmer's Reference Manual, 1986:
  https://pdos.csail.mit.edu/6.828/2018/readings/i386.pdf
- Intel 64 and IA-32 Architectures Software Developer's Manual:
  https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
## Current Order

0. 386 instruction-set audit.
   - Status: started.
   - Current audit file: `INTEL_386_INSTRUCTION_AUDIT.md`.
   - Next target: implement the missing 386-only opcodes listed there before
     expanding protected-mode privilege, paging, and VM86 behavior.  DPMI and
     VCPI belong in guest DOS drivers or TSRs, not in the emulator core.

1. Guest-side extender boundary.
   - Status: decided.
   - DPMI and VCPI are outside the emulator core.  A DOS extender host should be
     loaded as a guest program or driver and should own `INT 2Fh`, `INT 31h`,
     or `INT 67h` itself.
   - Done when: the emulator does not intercept those interfaces and the CPU
     protected-mode implementation is accurate enough for a guest-side host.

2. Full VM86 monitor behavior.
   - Status: partial.
   - Already present: VM86 task entry, VM86 interrupt frame/`IRETD`, TSS I/O
     bitmap checks, IOPL-sensitive traps for `LOCK`, `PUSHF`, `POPF`,
     `INT imm8`, and `IRET/IRETD`.
   - Missing: monitor-side emulation/reflection after `#GP(0)`, virtual IF
     state, software interrupt reflection, and BIOS/DOS call bridging.
   - Done when: a VM86 task can run DOS/BIOS calls under a protected monitor
     without leaving the emulator in a fault loop.

3. Protected-mode paging conformance.
   - Status: partial.
   - Already present: basic 80386 two-level paging, `CR2`, `CR3`, `#PF`,
     present/user/write checks, and accessed/dirty updates.
   - Missing: conformance review for supervisor write-protect behavior,
     reserved-bit behavior for the selected 386 model, exact page-fault error
     codes, and interactions with task switches/exceptions.
   - Done when: paging behavior matches the 80386 PRM for all supported CR0/CR3
     states used by DOS extenders.

4. Full protected-mode privilege model.
   - Status: partial.
   - Already present: basic CPL/DPL/RPL checks for segment loads, far control
     transfers, call gates, interrupt/trap gates, and some stack transitions.
   - Missing: complete privilege checks on every CPU path, exact exception
     choice/error code for edge cases, and full ring-transition validation.
   - Done when: descriptor, gate, stack, and exception behavior can be tested
     systematically against Intel 80386 rules.

5. Hardware task-switching conformance.
   - Status: partial.
   - Already present: 16-bit/32-bit TSS task switches, task gates, busy-bit
     updates, backlink, `IRET` with `NT`, `CR3`, `LDTR`, segments, and `CR0.TS`.
   - Missing: invalid TSS edge cases, exact error-code selectors, busy-state
     corner cases, and stricter 16-bit/32-bit TSS validation.
   - Done when: `test386.asm` protected TSS tests no longer expose task-switch
     failures.

6. Regression and conformance tests.
   - Status: ongoing.
   - Keep using `test386.bin`, PCjs CPU tests, DOS benchmark pack, and the
     existing debug logs.  Add a short build-log entry for every completed
     protected-mode step.
