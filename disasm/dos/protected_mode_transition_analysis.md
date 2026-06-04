# DOS/4GW and SEA.EXE Protected-Mode Transition Notes

Date: 2026-06-03

Analyzed local copies:

- `disasm/dos/DOS4GW.EXE/DOS4GW.EXE`
- `disasm/dos/SEA.EXE/SEA.EXE`

Generated local Ghidra exports:

- `disasm/dos/DOS4GW.EXE/ghidra_export/`
- `disasm/dos/SEA.EXE/ghidra_export/`
- focused range dumps in `disasm/dos/protected_mode_windows/`

## File Format Summary

`DOS4GW.EXE` is a 16-bit MZ executable containing the DOS/4G / DOS/16M extender runtime. Ghidra loads it as `x86:LE:16:Real Mode`. It contains both real-mode startup code and protected-mode support code. The file also has extra packed/overlay data after the MZ image.

`SEA.EXE` is an MZ stub followed by a Linear Executable (`LE`) image at file offset `0x2C90`. The LE header says CPU type `0x2`, which is the 386 class. The LE entry is object `1`, `EIP=0x93CDC`, stack object `3`, `ESP=0x483D0`, page size `0x1000`. This means `SEA.EXE` expects a DOS extender to create the 32-bit protected-mode environment before the application entry runs.

## DOS4GW Protected-Mode Paths

### DPMI path

The key DPMI installation probe is in `DOS4GW.EXE`:

- `11bd:6d9e`: `MOV AX,0x1687`
- `11bd:6da1`: `INT 0x2f`
- If `AX == 0`, the code stores the DPMI host entry data:
  - `SI` saved at `CS:[0x6d64]`
  - `DI` saved at `CS:[0x6e86]`
  - `ES` saved at `CS:[0x6e88]`

Relevant dump:

- `disasm/dos/protected_mode_windows/dos4gw_dpmi_probe_entry.txt`

After entering/initializing DPMI it uses many INT 31h services. The important observed calls include:

- `AX=0000h/0001h/0003h`: selector allocation/free/increment handling.
- `AX=0006h/0007h/0008h/0009h`: descriptor base, limit, and access-rights handling.
- `AX=000Ah/000Bh/000Ch`: alias and full descriptor get/set.
- `AX=0203h/0204h/0205h`: protected-mode interrupt/exception vector setup.
- `AX=0305h/0306h`: state save/restore and raw mode switch address queries.
- `AX=0A00h`: vendor/private DPMI or DOS/4G-specific capability path.

Relevant dump:

- `disasm/dos/protected_mode_windows/dos4gw_dpmi_int31_services.txt`

This is the most important path for our emulator if we want DOS/4GW/DOS4GW-based programs to run reliably. A minimally useful DPMI host must make the `INT 2Fh AX=1687h` probe succeed, provide a callable real-mode entry point, then support the INT 31h descriptor, vector, real-mode bridge, and raw-switch services that DOS/4GW asks for.

### XMS path

DOS4GW checks XMS:

- `11bd:6c74`: `MOV AX,4300h`, `INT 2Fh`
- `11bd:6c8c`: `MOV AX,4310h`, `INT 2Fh`
- It stores the XMS driver entry at `[0x0aec]` and calls it via `CALLF [0x0aec]`.

This is likely used for extended-memory allocation/fallback support.

### VCPI path

DOS4GW has a VCPI path through `INT 67h`:

- `11bd:76e0`: `MOV AX,3567h`, `INT 21h` to read the INT 67h vector.
- It checks an EMM/VCPI signature at the handler.
- `11bd:7725`: `MOV AX,DE00h`, `INT 67h`.
- `11bd:7760`: `MOV AX,DE0Ah`, `INT 67h`.
- Later it manipulates `CR3`:
  - `11bd:7abd`: `MOV EAX,CR3`
  - `11bd:7ac0`: `MOV CR3,EAX`

Relevant dumps:

- `disasm/dos/protected_mode_windows/dos4gw_vcpi_probe.txt`
- `disasm/dos/protected_mode_windows/dos4gw_vcpi_cr3.txt`

VCPI support is probably optional if our DPMI path is good enough, but the DOS4GW runtime contains this fallback and may reach it when DPMI probing fails or reports an unsuitable host.

### Raw protected-mode switch path

DOS4GW also contains direct switch code:

- `11bd:645c`: `LGDT SS:[0x8c8]`
- `11bd:6462`: `LIDT SS:[0x8d0]`
- `11bd:6468`: `SMSW AX`
- `11bd:646b`: `OR AL,1`
- `11bd:646d`: `LMSW AX`
- `11bd:6470`: far jump

There is a second similar switch at `11bd:64e6..64fa`.

Relevant dump:

- `disasm/dos/protected_mode_windows/dos4gw_raw_switch_lgdt_lidt_lmsw.txt`

For accurate raw switch support, the emulator needs correct `LGDT`, `LIDT`, `SMSW`, `LMSW`, protected-mode far jump semantics, descriptor loading, IDT format, and exception handling.

## SEA.EXE Behavior

The MZ stub starts at `1000:038c` and performs DOS setup:

- resizes the DOS memory block with `INT 21h AH=4Ah`;
- reads PSP/environment/command line data;
- sets up small callback/thunk tables;
- opens/writes files through DOS functions.

Relevant dumps:

- `disasm/dos/protected_mode_windows/sea_mz_entry_1000_038c.txt`
- `disasm/dos/protected_mode_windows/sea_mz_entry_stub.txt`

The `1000:2500` range is mostly an interrupt thunk table: `INT n; RET` entries. It includes `INT 2Fh`, `INT 31h`, and `INT 67h` thunk entries, but not the logic that decides the protected-mode switch.

Relevant dump:

- `disasm/dos/protected_mode_windows/sea_stub_pm_probe_1000_2500.txt`

Raw 32-bit scanning found `MOV CR0` instructions inside `SEA.EXE`, but these do not set protected mode. They toggle CR0 bits around x87/FPU handling:

- `000c75ea`: `MOV ECX,CR0`
- `000c75ed`: `OR ECX,4`
- `000c75f0`: `AND ECX,0xfffffffd`
- `000c75f3`: `MOV CR0,ECX`
- `000cd96c`: `MOV EAX,CR0`
- `000cd96f`: `OR AL,4`
- `000cd973`: `MOV CR0,EAX`
- `000cd9d3`: `MOV EAX,CR0`
- `000cd9d6`: `AND AL,0xfb`
- `000cd9d8`: `MOV CR0,EAX`

Relevant dump:

- `disasm/dos/protected_mode_windows/sea_raw32_cr_switch_ranges.txt`

So `SEA.EXE` itself looks like a 386 LE application loaded by DOS/4G. Its protected-mode transition is performed by the DOS/4G/DOS4GW layer, not by the application entry code.

## Emulator Implications

To run DOS4GW/SEA-style programs, our Pico-286 emulator should prioritize:

1. A real DPMI 0.9 path: `INT 2Fh AX=1687h` entry, DPMI state setup, and enough `INT 31h` services for descriptors, vectors, callbacks, real-mode bridge, and raw mode switch queries.
2. Correct descriptor instructions used by DOS4GW: `LAR`, `LSL`, `VERR/VERW`, selector limits/access rights, and 16/32-bit descriptor behavior.
3. Correct protected-mode far control transfers: far call/jump/ret/iret, including operand-size prefixes and `IRETD`.
4. Correct raw switch fallback: `LGDT`, `LIDT`, `SMSW`, `LMSW`, far jump into protected code, protected IDT, and exception delivery.
5. Optional VCPI if we want compatibility when DPMI is disabled or rejected.
6. Correct CR0 x87 bits (`EM`, `MP`, `TS`) and `WAIT/FWAIT`/x87 behavior, because `SEA.EXE` and DOS4G runtime manipulate these bits.

The current observed invalid-opcode failures are consistent with one of these failures:

- DOS4GW entered a path where the emulator did not provide a complete DPMI/VCPI/raw-switch contract.
- The CPU mode or CS default operand size was wrong when 32-bit protected code started.
- A DPMI `INT 31h` service returned a value that made DOS4GW build bad descriptors or bad real-mode bridge frames.

