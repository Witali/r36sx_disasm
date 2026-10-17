# CPU clock probe build log

## 2026-10-17 diagnostic prepared for the actual device

Purpose: distinguish the user's approximate 600 MHz recollection from the
saved firmware's selector value before stating a measured CPU frequency in
the documentary. `hardware_info/cpu.txt` reports MIPS 74Kc V4.12 and 589.82
BogoMIPS. That calibration score is not a MHz field. The saved DTS selects
`clock = <5>`; the vendor source's MCPU mapping assigns selector 5 to 1188 MHz.
Neither fact alone is a fresh frequency measurement on this unit.

Implementation: static Linux command-line diagnostic, read-only CPU/sysfs
reports, existing clock-debug summary, whitelisted read-only MMIO clock
information/control registers, and RDHWR Count/CCRes sampling. It records the
timebase and interpretation limits, creates a unique `.txt` report beside
the executable under `diagnostics/`, and exits automatically. MMIO and counter
access run in separate children with timeouts and core dumps disabled. It
does not change clocks, PLLs, firmware, emulator configuration or RTC.

Build command, using the existing SDK (no new downloads):

```sh
bash homebrew/cpu_clock_probe/build_wsl.sh
```

- WSL x86_64; Codescape GNU Tools 2018.09-02, GCC 6.3.0.
- Flags: `-EL -mips32r2 -mabi=32 -msoft-float -O2 -Wall -Wextra -std=gnu11 -static`.
- Sysroot: `tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/sysroot/mipsel-r2-soft`.
- Explicit sysroot include/library paths and `-B<sysroot>/usr/lib/` are required
  by this relocated SDK. The first attempt lacked `errno.h` in GCC's default
  search path; the second lacked startup objects `crt1.o`/`crti.o`. Both were
  resolved by those explicit paths.
- Link: `-lrt`; release binary stripped with the matching toolchain strip.
- Output: `patches/disk_image_patch_cpu_clock_probe/MIPS_NATIVE/cpu_clock_probe/cpu_clock_probe`.
- ELF32, little-endian, MIPS32r2, O32, static executable; no interpreter or
  dynamic C-library dependency.
- Size: 625,284 bytes.
- SHA256: `0f91022a2b4eb4846d5ca6fb0cc1aebf08ee8eec110d8c5ec1b3f5bc2a4a1656`.
- `readelf`, the project ELF summary and `objdump` confirmed the target ABI
  and RDHWR counter/resolution instructions.
- A separate WSL host build exercised report creation, file collection and
  the non-MIPS skip path. Its only warning was the MIPS-only elapsed helper
  unused in the host build. This was not a target-hardware frequency test.
- Microsoft Defender scanned the patch directory: no threats.
- Deployed to `E:/MIPS_NATIVE/cpu_clock_probe/` on the user-connected FAT32
  console card. The destination binary SHA256 matches the repository artifact.
  Only this new application folder was copied; existing apps were preserved.

The user will launch the diagnostic via Shell or Tiny MC, return the SD card
to the computer, and provide `diagnostics/cpu_clock_report_*.txt`. A device
result is still pending. Instruction and API references are in `README.md`.

## 2026-10-17 returned device report

The user ran the tool and returned the card. One 3,259-byte complete report and
one empty file were recovered and preserved with hashes in
`hardware_info/clock_probe/2026-10-17/`. The complete run confirms clock
information/control selector 5, mapping to 1188 MHz in the vendor table.
CCRes is 2; the counter runs at about 594 MHz and the derived CPU value is
about 1188 MHz. Linux's timebase is itself MIPS, so that ratio is a consistency
check rather than independent physical calibration. One Linux CPU is online
and present; MemTotal is 43164 kB. Full findings and limits are in
`hardware_info/clock_probe/README.md`. The empty report's cause is unknown.
