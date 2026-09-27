# Pico-286 Test Payloads

This directory stores CPU and platform test sources that are useful for the
R36SX Pico-286 port.

## Instruction fault regression ROM

`cpu386_faults.asm` is a standalone 64 KiB BIOS ROM testing real production
opcode decode, segmentation, paging and exception delivery, without DOS or
disk images. Build a Windows debug EXE first, then run:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_faults.ps1
# Also exercise the GCC/computed-goto core:
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_faults.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-faults-mingw
```

The runner assembles with the local NASM, temporarily changes only the build
config, checks POST `80:FF`, the exact `CPU386 FAULTS PASS cases=35` message,
the debug mailbox and framebuffer, then restores the config. POST `80:FE`
means failure. ROM/listing artifacts are in `build/`; diagnostics are in the
patch `diagnostics/compiler-<Tag>/` directory. Do not run multiple instances
of these runners concurrently: they share the build config.
The common `smoke_windows_build.ps1` also accepts `-CpuModel 80286` and
`-AllowBlankFrame` for port-only ROMs such as `test286`; the latter still
checks framebuffer dimensions and the requested POST/text completion markers.

Coverage (35 cases):

- 24 memory MOV loads: byte/word/dword operands, 16/32-bit addresses, CS.D=0/1,
  with null DS (#GP) and a non-present data page (#PF).
- Six immediate MOV fetches crossing a page boundary, covering byte/word/
  dword immediates in both code sizes, including operand-size prefixes.
- One opcode fetch from a non-present page.
- One valid INC at the last byte of a mapped page. It must complete before
  the next opcode fetch faults; diagnostic lookahead must not cause #PF.
- One fault while TF is set: no stale single-step trap over the fault handler.
- One #GP followed by a not-present #GP gate, producing #DF(0), followed by
  another ordinary #GP to check delivery-state cleanup.

Checks include preserved EAX/defined arithmetic flags, handler entry, saved
CS:EIP, error code and CR2. Data-load CR2 is checked exactly; instruction-fetch
CR2 is checked at page precision. Intel 9.8.14 specifies the faulting access
address but not an instruction-fetch granule/order within a split immediate;
this suite does not establish bus-level fetch precision. Saved fault EIP is
still exact. #DF restart EIP is deliberately not asserted.
This covers instruction abortion, not rollback of earlier writes in compound
instructions, REP progress, task-switch restart state, or every MOV encoding.
Those remain separate items in `TODO_X86_SOURCE_AUDIT.md`.

Specifications: [Intel 80386 PRM 9.1](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm),
[9.8](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm),
[MOV](https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOV.htm), and
[AMD APM vol. 3 rev. 3.19, MOV pp. 213-215](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf).
Only legacy behavior is used; original 80386 rules take precedence over later
CPU extensions.

## test386.asm

`test386.asm` is vendored from:

https://github.com/barotto/test386.asm

The R36SX copy is configured in `test386.asm/src/configuration.asm` with:

- `POST_PORT equ 0x80`
- `OUT_PORT equ 0x191`
- `DEBUG equ 1`
- `VGA_DEBUG equ 1`

Pico-286 captures standard POST writes to `80h`, keeps legacy support for the
older R36SX `190h` test port, and logs output-port text from `191h` as
`test386:` lines in `pico_286.log`.

With `VGA_DEBUG` enabled, the ROM also writes short breadcrumbs directly to
VGA text memory at `B800:0000` during the early `POST 01` branch/loop tests:
`JCC8`, `JCC16`, `LOOP`, `LOOPZ`, and `LOOPNZ`.

Build the ROM payload with:

```powershell
.\homebrew\pico_286\tests\build_test386_r36sx.ps1
```

The script uses the local NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\tests\test386.asm\src\ -f bin .\homebrew\pico_286\tests\test386.asm\src\test386.asm -w-all -l .\homebrew\pico_286\tests\test386.asm\build\test386.lst -o .\homebrew\pico_286\tests\test386.asm\build\test386.bin
```

Rebuild `cpu_tests.img` with:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
```

The script writes the floppy image to
`homebrew/pico_286/images/cpu_tests.img`.

The generated `test386.bin` is a 64 KB BIOS replacement ROM. It is not a
DOS `.COM` program, so it cannot be launched from the DOS prompt. The test disk
stores it as `TEST386.BIN` for reference and for future emulator BIOS-loading
work.  `rebuild_cpu_tests_disk.ps1` also copies the same ROM to
`homebrew/pico_286/test386.bin`, which is the default `test_bios_rom` used by
the native executable.

## test286.asm

`test286.asm` is a small R36SX-specific NASM BIOS replacement ROM for 80286
smoke testing.  It is not a full instruction conformance suite like
`test386.asm`; it focuses on compact POST-driven coverage for 286 behavior:

- real-mode `PUSH SP`, `PUSHA`/`POPA`, 5-bit shift-count masking, `IMUL`,
  `BOUND`, `SGDT`, `SIDT`, and `SMSW`;
- raw protected-mode entry through `LMSW` and a far jump;
- protected-mode `LSL`, `LAR`, `VERR`, `VERW`, and `ARPL` descriptor checks.

Build the ROM payload with:

```powershell
.\homebrew\pico_286\tests\build_test286_r36sx.ps1
```

The script uses the local NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\tests\test286.asm\src\ -f bin .\homebrew\pico_286\tests\test286.asm\src\test286.asm -w-all -l .\homebrew\pico_286\tests\test286.asm\build\test286.lst -o .\homebrew\pico_286\tests\test286.asm\build\test286.bin
```

The generated `test286.bin` is also 64 KB and is copied to
`homebrew/pico_286/test286.bin`, so it can be selected with:

```ini
bios=test286
test_bios_rom=test286.bin
cpu_model=80286
```

## pcxtbios

`pcxtbios/pcxtbios_from_ghidra_bytes.asm` is a byte-preserving NASM rebuild
source for the embedded 8 KB PC/XT-style BIOS ROM used by Pico-286.  It was
generated from `BIOS/pcxtbios_ghidra_full.s` and emits the original ROM bytes
with `db` directives while keeping Ghidra/ndisasm context as comments.

This source is intentionally conservative: it rebuilds byte-identically to
`BIOS/pcxtbios.bin` before we start replacing understood ranges with symbolic
labels and real instructions.

Build it with WSL NASM or any NASM-compatible binary:

```sh
nasm -f bin homebrew/pico_286/tests/pcxtbios/pcxtbios_from_ghidra_bytes.asm \
  -o BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

Expected SHA-256 for the rebuilt ROM:

```text
468396458c74542e6fdf675fa53e9552b3037a5d6119e3232f486e8543922b96
```

## MAPDRIVE.COM

`homebrew/pico_286/pico-286/tools/mapdrive.asm` is the standalone DOS utility
source for registering a host-backed network drive.  It defaults to `H:` and
also accepts a drive parameter such as `MAPDRIVE G:` or `MAPDRIVE G`.  The
R36SX native executable no longer embeds or launches a MAPDRIVE trampoline, so
the patch folder keeps a real `.COM` copy for DOS-side mapping and testing.

Build it with the same NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -f bin .\homebrew\pico_286\pico-286\tools\mapdrive.asm -o .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\tools\MAPDRIVE.COM
```

Generated DOS `.COM` files for the patch are kept under
`patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/`.

## SBPROBE.COM

`sound_blaster/sb_probe.asm` is a DOS `.COM` probe for the Sound Blaster DSP
emulation.  It checks reset/version/identity commands, speaker status, DMA
identification, and a single-cycle DMA playback IRQ acknowledge path.

Build it with:

```powershell
.\homebrew\pico_286\tests\sound_blaster\build_sb_probe.ps1
```

The script also mirrors the generated DOS test to
`patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/SBPROBE.COM`.
