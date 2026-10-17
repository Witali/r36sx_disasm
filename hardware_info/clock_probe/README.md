# Live R36SX clock diagnostic

On 2026-10-17 the creator ran `cpu_clock_probe` on the handheld and returned
its SD card. Raw output and hashes are in `2026-10-17/`. One report is complete
(`cpu_clock_report_ef0quf.txt`, 3,259 bytes); another is empty. The reason for
the empty attempt is unknown and it is not used as evidence.

## Findings

- CPU: **MIPS 74Kc V4.12**, one processor exposed to Linux. Both `online` and
  `present` contain `0`. This describes the Linux/application CPU; the SoC's
  separate RTOS side is not included in that count.
- Clock information register `0x18800070`: **`0x0007f510`**.
- Clock control register `0x18800074`: **`0x00000510`**.
- Bits 10:8 in both registers select **5**. The vendor MCPU source table maps
  selector 5 to **1188 MHz**, or approximately **1.2 GHz**. This is a live
  register read, rather than an inference from the saved device tree alone.
- RDHWR reports **CCRes = 2 CPU cycles/count**. Three counter samples give
  593.997–593.998 MHz, or 1187.994–1187.995 MHz after applying that divisor.
- The active and only reported Linux clocksource is **MIPS**. The counter and
  the Linux time reference are therefore not independent physical clocks.
  These sample values are a consistency check, not an independent precision
  calibration or evidence for MHz accuracy to three decimal places.
- CPUFreq current-frequency files and `clk_summary` are not exposed on this
  firmware. The program made no clock, PLL, firmware or sysfs changes.
- `MemTotal` remains **43164 kB**, about **42.15 MiB** usable by Linux.

The evidence supports describing this device's Linux CPU as a single MIPS
74Kc core with a live clock selection of about **1.2 GHz**. The earlier guess
of 600 MHz confused a roughly 594 MHz counter rate with the CPU rate; the
589.82 BogoMIPS field is also not a MHz specification.

## Interpretation sources

Register locations, shifts and width come from
`hardware_info/dtb/dtb.dts`, `/soc/clocks/base_clk/cpu_clk@18800070`.
The selector table comes from the retained vendor SDK source:

- Repository: https://git.maschath.de/ignatz/hcrtos
- Commit: `1dd13c6ddc034313c0c3b9a31a123449f7baf110`
- File: `components/kernel/source/arch/mips/init_hook.c`
- Function: `mcpu_clksel2clk()`, selector 5 returns `1188 * 1000 * 1000`.

RDHWR semantics, Linux emulation limitations, build and deployment are
documented in `homebrew/cpu_clock_probe/README.md` and `BUILD_LOG.md`.
