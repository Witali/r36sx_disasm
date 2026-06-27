# Disassembly Results

This directory is organized primarily by executable or analysis target type.

| Directory | Contents |
| --- | --- |
| `dos/` | Disassembly/decompilation outputs for DOS programs. |
| `mips/` | MIPS raw firmware, MIPS native artifacts, and MIPS-focused reverse engineering. |
| `linux/` | Linux executables and shared libraries, including Linux/MIPSEL ELF files. |
| `BIOS/` | PC BIOS and option ROM analysis. |
| `firmware/` | Firmware/container analysis that is not better described by executable type. |
| `mtd/` | Disassembly, strings, scans, and notes for MTD/NOR dump files. |

Raw input files should stay in their original source directories. Put generated
analysis artifacts here. Older source-oriented folders such as
`disk_image/mips_elf/` may exist for historical work; new Ghidra exports should
prefer the type-first layout above.
