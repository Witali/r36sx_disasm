# Pico-286 Optimization TODO

This file collects possible future optimizations for the R36SX Pico-286 port.
These are candidates, not committed design decisions; each item should be
verified with profiling and compatibility tests before implementation.

## Tiny386 Comparison Candidates

Source project: <https://github.com/hchunhui/tiny386>

- Compare the instruction-table approach in `tiny386/i386ins.def` with our
  current `r36sx_cpu_*` opcode handlers.  The table can be used as a compact
  checklist for missing 386 opcodes and as inspiration for cleaner opcode
  grouping.
- Evaluate tiny386-style lazy flags for more ALU paths.  Tiny386 stores the
  last arithmetic operation and computes flags only when they are consumed; this
  may reduce flag bookkeeping in hot DOS UI and game loops.
- Study tiny386's paging/TLB cache.  Our protected-mode paging path aims for
  higher correctness, but a small TLB for translated pages may help 386 DOS
  extenders and graphics-heavy protected-mode programs.
- Add an instruction-fetch cache for the current CS/EIP page, similar in spirit
  to tiny386's `ifetch` state.  This is likely useful on MIPS because opcode
  fetch currently goes through normal memory helpers.
- Continue splitting CPU execution paths by model: keep `exec86`, `exec286`,
  and `exec386` selected once from configuration, then remove remaining
  higher-generation checks from lower-model hot paths where it is safe.
- Keep the 8086 path strictly real-mode and minimal.  It should avoid 286/386
  descriptor, paging, VM86, and protected-mode branches unless a shared helper
  genuinely requires them.
- Keep the 286 path free of 386-only paging, VM86, FS/GS, 32-bit control
  register, and 32-bit protected-mode checks where possible.
- Use tiny386 as a performance reference, not as a correctness oracle.  Tiny386
  is intentionally practical and portable, and its own README notes missing
  hardware tasking and permission checks.

## Larger Architecture Experiments

- Test whether SeaBIOS/VGABIOS integration is feasible as an optional
  experiment.  Tiny386 uses SeaBIOS/VGABIOS, while Pico-286 currently relies on
  its own BIOS/INT 10h/VGA implementation tailored to R36SX.
- Compare IDE/VGA/RTC/PIC/PIT device models against tiny386's TinyEMU/QEMU
  derived devices.  Borrow only small, well-understood behavior fixes; do not
  replace R36SX-specific framebuffer, input, audio, and patch workflow code
  wholesale.
- Keep MIPS build size and runtime cost in mind.  Any borrowed idea should be
  measured on both the Windows debug build and the R36SX MIPS release build.
