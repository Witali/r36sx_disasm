# R36SX MIPS Notes

## Target

The captured R36SX/SF3000-style target is a MIPS 74Kc-class Linux device. Local
hardware notes report MIPS32r2 plus `mips16`, `dsp`, and `dsp2` ASEs. Normal
Pico-286 native builds are little-endian Linux/MIPS executables.

## Build Path

Use the WSL/GCC wrapper for normal device builds:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
```

The wrapper invokes `build_pico_286_wsl.sh` and the Linux
`mips-mti-linux-gnu-gcc` / `g++` tools from the repository toolchain area. The
Zig/Clang MIPS path is fallback only.

## Verification

- Run `elf_mips_summary.py` on generated ELF files before deploying when the
  task touches toolchain flags or binary format.
- Record size and SHA256 for generated device-test binaries.
- Copy release patch binaries into
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286`.
- After copying the MIPS `pico_286` patch binary, update patch `pico_286.conf`
  `[rtc] rtc_start_time` to the current local time.

## Disassembly

- Prefer toolchain `objdump`/`readelf` for ELF files when available.
- Track virtual address, file offset, section, and symbol context separately.
- Remember MIPS branch delay slots when auditing control flow.
- Treat generated disassembly as evidence, not the architectural authority.
