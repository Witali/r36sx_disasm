# chkdsk.exe Ghidra Analysis Notes

Analyzed on 2026-05-27 with Ghidra 12.0.4 headless.

## Input

```text
disk_image/chkdsk.exe
```

SHA256:

```text
B825E4BB74036156A48F03EFF9244091E1FC1CFD4E183062E267A6C091B7E263
```

## Command

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat `
  .\ghidra_projects chkdsk_pe64 `
  -import .\disk_image\chkdsk.exe `
  -scriptPath .\ghidra_scripts `
  -postScript ExportDisasmAndDecompile.java .\ghidra_exports\chkdsk_exe `
  -deleteProject
```

## Exported Files

```text
summary.txt
functions.tsv
symbols.tsv
disassembly.s
decompiled_all.c
decompiled_key_functions.c
NOTES.md
```

`decompiled_key_functions.c` is empty because the reusable export script looks
for libretro/firmware-oriented names, while this PE uses generic names such as
`entry` and `FUN_1400018f8`.

## Main Findings

- Ghidra loaded the file as `Portable Executable (PE)`.
- Language/compiler: `x86:LE:64:default` / `windows`.
- Image base: `0x140000000`.
- 37 functions were identified.
- The entry function is `entry` at `0x140001590`.
- The largest and likely main application function is `FUN_1400018f8` at
  `0x1400018f8`.
- The binary imports normal Windows chkdsk support libraries:
  - `ULIB.DLL`
  - `IFSUTIL.DLL`
  - `MSVCRT.DLL`
  - `NTDLL.DLL`
  - Windows API-set DLLs
- Decompiled strings include the source path
  `onecore\base\fs\utils\chkdsk\chkdsk.cxx`.
- The binary references `ChkdskEx`, `CHKDSK`, `RegisterEventSourceW`, and
  `ReportEventW`.

## Interpretation

This is a Windows x86-64 console program, not code for the MIPS handheld. It
looks like a small Microsoft Windows Check Disk frontend copied into the SD-card
image root together with `Chkdsk.bat`.
