# Pico-286 Test Payloads

This directory stores CPU and platform test sources that are useful for the
R36SX Pico-286 port.

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
