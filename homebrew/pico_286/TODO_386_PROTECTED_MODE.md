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
- DPMI Specification Version 1.0, DPMI Committee, March 12 1991:
  https://docs.pcjs.org/specs/dpmi/1991_03_12-DPMI_Spec_v10.pdf

## Current Order

0. 386 instruction-set audit.
   - Status: started.
   - Current audit file: `INTEL_386_INSTRUCTION_AUDIT.md`.
   - Next target: implement the missing 386-only opcodes listed there before
     expanding the higher-level DPMI/VCPI surface.

1. Minimal DPMI host groundwork.
   - Status: started.
   - Specification checkpoints: DPMI client initialization, `INT 2Fh
     AX=1686h/1687h`, protected-mode entry, and `INT 31h` dispatcher rules.
   - Completed so far: `INT 2Fh AX=1686h/1687h` and `INT 31h` now route
     through named DPMI helpers.  `AX=1687h` still reports no host until a real
     protected-mode entry exists, and `INT 31h` returns the DPMI unsupported
     function shape (`CF=1`, `AX=8001h`) as a safe scaffold.
   - Next target: add the real `AX=1687h` installation-check success path only
     after a protected-mode entry routine and client state block exist.
   - Done when: the code has a clean DPMI probe/dispatch structure, debug logs
     identify all required client-entry values, and later commits can add real
     `INT 31h` services without touching the generic `intcall86()` flow.

2. Minimal `INT 31h` services for DOS extenders.
   - Status: started.
   - Specification checkpoints: descriptor allocation/free, descriptor
     base/limit/access rights, version query, real-mode interrupt/procedure
     bridge, DOS memory allocate/free, exception/vector get/set, and terminate.
   - Completed so far: the protected-mode `INT 31h` dispatcher now implements
     selector increment (`AX=0003h`), version query (`AX=0400h`), and the
     capabilities/vendor string query (`AX=0401h`) with conservative feature
     flags.  A small DPMI LDT-style descriptor pool now handles descriptor
     allocation/free (`AX=0000h/0001h`) plus base, limit, and access-rights
     updates (`AX=0006h/0007h/0008h/0009h`), and the CPU descriptor decoder can
     resolve those selectors.  Segment-to-descriptor mapping, alias
     descriptors, and raw descriptor copy services (`AX=0002h/000Ah/000Bh/000Ch`)
     are also scaffolded on the same pool.  The real-mode `AX=1687h`
     installation check still reports no host until the mode-switch entry point
     exists.
   - Done when: common DOS extender probes can query DPMI version, allocate
     descriptors, configure flat 32-bit code/data descriptors, and return clean
     failure for unimplemented functions.

3. Full VM86 monitor behavior.
   - Status: partial.
   - Already present: VM86 task entry, VM86 interrupt frame/`IRETD`, TSS I/O
     bitmap checks, IOPL-sensitive traps for `LOCK`, `PUSHF`, `POPF`,
     `INT imm8`, and `IRET/IRETD`.
   - Missing: monitor-side emulation/reflection after `#GP(0)`, virtual IF
     state, software interrupt reflection, and BIOS/DOS call bridging.
   - Done when: a VM86 task can run DOS/BIOS calls under a protected monitor
     without leaving the emulator in a fault loop.

4. Protected-mode paging conformance.
   - Status: partial.
   - Already present: basic 80386 two-level paging, `CR2`, `CR3`, `#PF`,
     present/user/write checks, and accessed/dirty updates.
   - Missing: conformance review for supervisor write-protect behavior,
     reserved-bit behavior for the selected 386 model, exact page-fault error
     codes, and interactions with task switches/exceptions.
   - Done when: paging behavior matches the 80386 PRM for all supported CR0/CR3
     states used by DOS extenders.

5. Full protected-mode privilege model.
   - Status: partial.
   - Already present: basic CPL/DPL/RPL checks for segment loads, far control
     transfers, call gates, interrupt/trap gates, and some stack transitions.
   - Missing: complete privilege checks on every CPU path, exact exception
     choice/error code for edge cases, and full ring-transition validation.
   - Done when: descriptor, gate, stack, and exception behavior can be tested
     systematically against Intel 80386 rules.

6. Hardware task-switching conformance.
   - Status: partial.
   - Already present: 16-bit/32-bit TSS task switches, task gates, busy-bit
     updates, backlink, `IRET` with `NT`, `CR3`, `LDTR`, segments, and `CR0.TS`.
   - Missing: invalid TSS edge cases, exact error-code selectors, busy-state
     corner cases, and stricter 16-bit/32-bit TSS validation.
   - Done when: `test386.asm` protected TSS tests no longer expose task-switch
     failures.

7. Regression and conformance tests.
   - Status: ongoing.
   - Keep using `test386.bin`, PCjs CPU tests, DOS benchmark pack, and the
     existing debug logs.  Add a short build-log entry for every completed
     protected-mode step.
