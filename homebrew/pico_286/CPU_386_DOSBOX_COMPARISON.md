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

## Deferred

- Full DOSBox protected-mode privilege model (`CPL`, `DPL`, `RPL`, gate checks).
- Paging and page faults.
- Task switching and TSS descriptors.
- v86 mode.
- A full replacement decoder based on DOSBox `core_normal` or `core_full`.

Those are larger architectural changes and should be ported only behind focused
tests, ideally using the existing `test386` BIOS and the DOS CPU test disk.
