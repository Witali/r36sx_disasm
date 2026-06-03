# libemu_tgbdual.so Ghidra export

Date: 2026-05-24

Input binary:

```text
disk_image\cubegm\cores\libemu_tgbdual.so
```

Tool:

```text
ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat
```

Ghidra language/compiler:

```text
MIPS:LE:32:default / default
```

## Exported files

- `summary.txt` - program metadata and memory blocks.
- `functions.tsv` - function table.
- `symbols.tsv` - symbol table.
- `disassembly.s` - full instruction listing exported from Ghidra.
- `decompiled_all.c` - C-like Ghidra decompiler output for all detected functions.
- `decompiled_key_functions.c` - C-like output for selected libretro/key functions.

Start with:

```text
decompiled_key_functions.c
disassembly.s
functions.tsv
```

Useful function names:

```text
retro_load_game
retro_run
retro_reset
retro_get_system_info
retro_get_system_av_info
dmy_renderer
gb::run
cpu::exec
render
SetFrameSkip
check_encrypty
```

## SHA256

```text
568FAE5B64A240FEF1A1C859A445CD38D24944719537447A31445AE542AEF2BF  summary.txt
8537940917D20F773E20169D65792C773DD308537ACDEA9575EB96BB1387BB3C  functions.tsv
0F9918FA952315945B460EA473D0AC76F46AD9211B98BCAFEE9EFC7B06C0190B  symbols.tsv
C4C38701DD9190E2D90ED7FA7A539A22DCAC9C0FFE08F24B76E4986365EE1C60  disassembly.s
529D07D96F644ED3A418D2E1F360160480621E72F20A589AC22F75F897D7C2D9  decompiled_all.c
A308B565A4389153965A960C051A8F6C125E4E647D521EE14CAA7114A2C2110A  decompiled_key_functions.c
```

## Command used

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects tgbdual_export2 -import .\disk_image\cubegm\cores\libemu_tgbdual.so -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\libemu_tgbdual -deleteProject
```
