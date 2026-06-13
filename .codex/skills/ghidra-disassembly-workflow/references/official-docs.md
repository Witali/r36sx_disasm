# Official Documentation Policy

Ghidra output is analysis evidence, not the final authority on semantics.
Before changing code, documenting behavior as fact, or patching a binary based
on Ghidra output, consult official documentation for the target domain.

## Preferred Sources

- CPU manuals:
  - Intel manuals for x86, 8086/80286/80386, BIOS-facing behavior, and flags.
  - MIPS32/MIPS ABI manuals for MIPS instruction, ABI, and ELF behavior.
- File-format specifications:
  - MZ/PE, ELF, ROM, boot-sector, filesystem, or firmware container specs.
- OS and firmware documentation:
  - IBM PC/XT/AT technical references.
  - Microsoft DOS documentation or FreeDOS source/documentation.
  - Linux kernel/UAPI documentation for native Linux binaries.
- Vendor documentation:
  - Hardware datasheets, SDK documentation, and official source trees for the
    specific device, driver, or subsystem.
- Ghidra documentation:
  - Use official Ghidra help/docs for scripting APIs and headless usage.

## Source Rules

- Record the manual/source name, revision/date when visible, and section used
  when the fact affects future work.
- Use blogs, wiki pages, forum posts, and decompiler comments only as secondary
  orientation.
- If official documentation is unavailable, say so and use the most
  authoritative archived source available.
- When Ghidra disagrees with official documentation, trust the documentation and
  investigate loader settings, language choice, base address, or decompiler
  assumptions.
