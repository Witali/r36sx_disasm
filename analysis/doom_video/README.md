# Doom video mode switching notes

Investigated files:

- `C:\Temp\DM.EXE`: LZEXE 0.91-compressed DOS MZ executable. Raw disassembly is noisy until it is unpacked.
- `C:\Temp\DOOM.EXE`: MZ loader with an embedded DOS/4GW LE protected-mode payload.
- `C:\Temp\DEFAULT.CFG`: text config; no special video mode setting was found.

## Findings

`DOOM.EXE` does not rely on plain BIOS mode 13h only. It enters VGA 320x200x256 through BIOS/DOS4GW setup, then the protected-mode code directly reprograms VGA ports into an unchained planar 256-color mode, from the Mode X/Mode Y family.

Important disassembly offsets in `DOOM.EXE`:

- `0x5C067`: selects Sequencer Memory Mode register 4 through port `3C4h`, reads/writes data through `3C5h`, clears chain-4 and changes memory mode.
- `0x5C08E`: selects Graphics Controller Mode register 5 through `3CEh/3CFh`, clearing mode bits.
- `0x5C0A1`: selects Graphics Controller Misc register 6 through `3CEh/3CFh`.
- `0x5C0B4`: writes `AX=0F02h` to `DX=3C4h`, which sets Sequencer Map Mask register 2 to `0Fh` using `OUT DX,AX`.
- `0x5C0D1` and `0x5C0E4`: modifies CRTC registers `14h` and `17h` through `3D4h/3D5h`.
- `0x5C15A`: loops over four VGA planes by writing Graphics Controller Read Map Select register 4, then copies `0x3E80` bytes per plane.
- `0x5CDD4`: uses Sequencer Map Mask `0Fh` and Graphics Controller write mode 1, so reads from VGA memory must load latches before writes copy those latches.

The visible failure mode looked like a wrong VGA memory layout or stale CRTC/page state rather than a simple palette bug.

## Emulator implication

The emulator must preserve byte-by-byte VGA side effects for 386 dword accesses to video RAM. In particular, Doom's unchained mode depends on:

- `OUT DX,AX` splitting into adjacent VGA index/data ports.
- CRTC start-address updates through `3D4h/3D5h`.
- Graphics-controller read latches when copying video pages in write mode 1.
- VGA dword reads/writes still going through the same aperture mapping and latch path as byte writes.
