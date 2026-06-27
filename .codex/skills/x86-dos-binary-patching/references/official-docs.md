# Official Documentation Policy

Before changing x86 assembly semantics, instruction emulation tests, BIOS
behavior, DOS API behavior, or hardware-port behavior, consult official
documentation for the exact target generation.

## Preferred Sources

- Intel manuals for CPU behavior:
  - 8086/8088 Family User's Manual for strict 8086/8088 behavior.
  - 80286 Programmer's Reference Manual for protected-mode 286 behavior.
  - Intel386 Programmer's Reference Manual for 80386 behavior.
  - Intel Software Developer's Manual only when checking later-compatible
    behavior and only after confirming it does not contradict the older target.
- IBM PC, PC/XT, and PC/AT technical references for BIOS data areas, interrupt
  vectors, PIT/PIC/DMA, keyboard controller, disk BIOS, and option ROM behavior.
- Microsoft DOS documentation, MS-DOS Programmer's Reference, or FreeDOS source
  and documentation for DOS API and redirector behavior.
- Vendor datasheets for hardware being emulated, such as Sound Blaster, VGA,
  IDE/ATAPI, or controller chips.

## Source Rules

- Record the exact manual title, revision/date when visible, and section used
  in the task notes, commit message, or relevant project document when the fact
  affects future work.
- Use non-official sources only as secondary orientation. If official docs are
  unavailable, say that explicitly and prefer well-known archival references.
- For x86 CPU behavior, do not let later manuals silently override older silicon
  behavior. Strict 8086, 80286, and 80386 behavior can differ.
- For undefined or reserved behavior, preserve what the project intentionally
  models and document the chosen compatibility target.
