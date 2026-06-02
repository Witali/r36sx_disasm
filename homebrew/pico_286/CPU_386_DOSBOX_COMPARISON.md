# Pico-286 386 CPU Notes vs DOSBox

This note records the small DOSBox-inspired CPU correctness fixes applied to
the R36SX Pico-286 port.  DOSBox has a much larger CPU subsystem with paging,
descriptor privilege checks, protected-mode task switching, v86 mode, and
multiple decoder cores, so this is not a wholesale CPU core import.

## Adopted Fixes

| Area | Pico-286 before | DOSBox behavior used as reference | R36SX change |
| --- | --- | --- | --- |
| `0F 00` group 6 | Worked in real mode. | DOSBox rejects it outside protected mode. | `SLDT`, `STR`, `LLDT`, `LTR`, `VERR`, and `VERW` now raise invalid opcode in real mode. |
| `0F 06 CLTS` | Not implemented. | DOSBox clears the CR0 task-switched bit. | `CLTS` now clears `CR0.TS` for 286+ CPU modes. |
| `LGDT`/`LIDT` base size | Always loaded a 32-bit base. | DOSBox masks the base to 24 bits without operand-size override and uses full 32 bits with `66h`. | Descriptor-table loads now follow the same 24-bit vs 32-bit base rule. |
| `MOV Rd,CR0` | Returned the raw local CR0 value. | DOSBox reports 386 CR0 reads with reserved bits 4..30 set. | CR0 reads now OR in the 386 reserved-bit read mask. |
| `MOV CR3`/`MOV Rd,CR3` | Preserved low 12 bits. | DOSBox exposes CR3 as a page-directory base. | CR3 writes and reads now keep only the page-directory-aligned upper bits. |
| Far protected transfers | Far `CALL`/`JMP`/`RET`/`IRET` mostly loaded a new `CS:IP`. | DOSBox validates protected descriptors, gates, and privilege transitions. | Immediate and indirect far forms now validate code descriptors, support 16/32-bit call gates, and use TSS stack slots for call-gate privilege switches. |
| Hardware task switching | TSS descriptors and task gates were rejected. | DOSBox supports task switches through TSS descriptors, task gates, nested-task returns, and descriptor busy-bit updates. | Far `CALL`/`JMP` and IDT task gates now switch to 16-bit or 32-bit TSS images; `IRET` with `NT` follows the backlink; `LTR`, task `CALL`/`JMP`/`IRET`, backlink, busy-bit, `CR3`, `LDTR`, segment selectors, and `CR0.TS` are updated. |
| Initial VM86 task entry | A task switch into a 32-bit TSS with `EFLAGS.VM=1` still tried to load `CS`/`SS` as protected descriptors. | DOSBox treats VM86 tasks as CPL 3 real-mode-style segment contexts while keeping protected paging/privilege machinery around them. | 32-bit TSS task loads with `VM=1` now cache visible segments as `selector << 4`, report CPL 3, and use real-mode segment linear addresses. |
| TSS I/O permission bitmap | `IN`/`OUT` only checked IOPL, while string I/O bypassed IOPL entirely. | DOSBox checks the 386 TSS I/O bitmap for VM86 or `CPL > IOPL` I/O instructions. | `IN`, `OUT`, `INSB`, `INSW`, `OUTSB`, and `OUTSW` now consult the current 32-bit TSS bitmap and raise `#GP(0)` when the port is denied or the bitmap is absent. |
| Protected interrupt VM86 frames | Protected interrupts pushed only `FLAGS/CS/IP` on the current stack, so CPL transitions and VM86 interrupts had the wrong frame. | DOSBox switches stacks through the current TSS and uses the 386 VM86 interrupt/IRETD frame when entering and leaving VM86. | Interrupt/trap gates now switch to the inner stack for lower-CPL handlers; VM86 interrupts push `GS/FS/DS/ES`, outer `SS:ESP`, and the normal frame; `IRETD` with `VM=1` restores real-mode-style VM86 segments. |
| Software interrupt gate DPL | `INT3`, `INT imm8`, and `INTO` used the same path as hardware interrupt delivery, so protected-mode software gate DPL checks were missing. | DOSBox distinguishes software interrupts from hardware/exceptions and raises `#GP` when CPL is numerically greater than the IDT gate DPL. | Software interrupt opcodes now pass a software flag into the protected interrupt path, check gate DPL, and leave hardware interrupts/exceptions on the non-software path. |
| VM86 IOPL-sensitive opcodes | VM86 `LOCK`, `PUSHF`, `POPF`, `INT imm8`, and `IRET/IRETD` executed directly even when `IOPL < 3`. | Intel/DOSBox trap these sensitive VM86 operations through `#GP(0)` so the protected monitor can emulate IF and reflect DOS interrupts. | VM86 now raises `#GP(0)` for those opcodes when `IOPL < 3`; `CLI`/`STI` already used the IOPL helper, while `INT3`/`INTO` keep their separate interrupt behavior. |

## Deferred

- Full DOSBox protected-mode privilege model for every CPU path and exception.
- Paging and page faults.
- More task-switch conformance work around invalid TSS/error-code edge cases.
- Full v86 monitor behavior: software interrupt reflection and virtual
  interrupt-flag state emulation after the new `#GP(0)` traps.
- A full replacement decoder based on DOSBox `core_normal` or `core_full`.

Those are larger architectural changes and should be ported only behind focused
tests, ideally using the existing `test386` BIOS and the DOS CPU test disk.
