# R36SX CPU clock probe

A short, read-only Linux diagnostic for the actual handheld. It collects CPU
and memory reports, online CPUs, CPUFreq values if available, the kernel
clocksource, and an existing clock-debug summary. It does not mount debugfs.

On an identified Hichip HC16xx device it attempts a read-only `/dev/mem` mapping
of two clock registers: `0x18800070` (information) and `0x18800074` (control).
The selector is bits 10:8. Vendor MCPU source mapping is 594, 396, 297, 198,
900, 1188, 24 MHz, or a programmable PLL. No MMIO writes occur. Use `--no-mem`
to skip this snapshot. Register interpretation is separate from measurement.

The program also samples MIPS RDHWR Count and CCRes against Linux monotonic
time three times under a short busy load. It retains raw counter frequency,
reported resolution and derived CPU MHz. Linux may emulate RDHWR. If the
monotonic clock uses the same CPU counter, this is not an independent physical
calibration. Analyze the timebase and register evidence before concluding MHz.
Unsupported counter instructions or failed register accesses are isolated in
child processes. The tool exits automatically after about two seconds under
normal conditions, with an eight-second timeout for each isolated operation.

## Device use

Copy the patch's `MIPS_NATIVE/cpu_clock_probe/` folder beside the other native
programs on the SD card. In Tiny MC, open it and run `cpu_clock_probe`, or use:

```sh
/mnt/sdcard/MIPS_NATIVE/cpu_clock_probe/cpu_clock_probe
```

Read the newly created `diagnostics/cpu_clock_report_*` file. Reports use unique
names and do not overwrite earlier runs. The tool does not change CPU frequency,
PLL settings, firmware, device-tree files, emulator configuration or system time.

## Build

Run from the repository in WSL:

```sh
bash homebrew/cpu_clock_probe/build_wsl.sh
```

The existing Codescape GCC path builds a static MIPSEL MIPS32r2 O32 soft-float
ELF, avoiding dependencies on the target's dynamic loader and C library.
The output is in `patches/disk_image_patch_cpu_clock_probe/MIPS_NATIVE/cpu_clock_probe/`.

## References

- Vendor source snapshot `internet_sources/hcrtos`, commit
  `1dd13c6ddc034313c0c3b9a31a123449f7baf110`,
  `components/kernel/source/arch/mips/init_hook.c`, `mcpu_clksel2clk()`.
  Origin: https://git.maschath.de/ignatz/hcrtos
- Saved clock binding: `hardware_info/dtb/dtb.dts`,
  `/soc/clocks/base_clk/cpu_clk@18800070`.
- MIPS *Programming the MIPS32 74K Core Family*, rev. 02.14, section 5.6,
  HWREna/RDHWR Count and CCRes. The official download endpoint was unavailable
  when opened; its indexed manufacturer text was consulted. Do not hard-code
  the counter divisor: older 74K manual revisions differ.
- Linux MIPS `arch/mips/kernel/traps.c`, `simulate_rdhwr()`, documents the
  emulated Count and CCRes behavior:
  https://code.googlesource.com/linux/torvalds/linux/+/24a0b5e196cf70ccff97bc0add6fa7178ad50cc4/arch/mips/kernel/traps.c
- https://man7.org/linux/man-pages/man3/clock_gettime.3.html
- https://man7.org/linux/man-pages/man4/mem.4.html
- https://gcc.gnu.org/onlinedocs/gcc/MIPS-Options.html
