# Official Documentation Policy

Before changing MIPS assembly semantics, compiler flags, ABI behavior, ELF
layout assumptions, dynamic loader behavior, or device-facing Linux integration,
consult official documentation for the exact target.

## Preferred Sources

- MIPS architecture manuals:
  - MIPS32 Architecture for Programmers, Volume I: Introduction.
  - MIPS32 Architecture for Programmers, Volume II: Instruction Set.
  - MIPS32 Architecture for Programmers, Volume III: Privileged Resource
    Architecture, when exceptions, TLB, cache, or kernel behavior matters.
  - MIPS DSP ASE documentation when using or auditing DSP instructions.
- Official ABI and toolchain documentation:
  - System V ABI / MIPS ABI supplements for calling convention, ELF,
    relocation, register usage, and dynamic linking.
  - GNU GCC manuals for MIPS target options.
  - GNU binutils manuals for `as`, `ld`, `objdump`, `readelf`, and `strip`.
- Linux documentation:
  - Linux kernel documentation or UAPI headers for syscall, input, framebuffer,
    audio, and file behavior.
  - libc/toolchain documentation for userspace ABI details.
- Vendor SDK documentation when behavior comes from the SF3000/R36SX toolchain
  or `driver.so` integration.

## Source Rules

- Record the exact manual title, revision/date when visible, and section used
  when the fact affects future work.
- Prefer official manuals and source repositories over wiki pages, blog posts,
  or forum notes.
- If an official document is unavailable, state that limitation and use the most
  authoritative archived vendor document available.
- Cross-check disassembly conclusions against ABI and instruction docs before
  patching code or changing compiler flags.
