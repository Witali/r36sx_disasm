# HC16xx Linux clock-control investigation

Date: 2026-10-17. Purpose: establish whether Tiny MC can offer a runtime CPU
frequency switch on the stock R36SX kernel.

## Input and reproduction

- Original container: `disk_image/cubegm/advapi32.dll`, U-Boot Linux uImage.
- Existing unpacked input:
  `disasm/disk_image/kernel/advapi32_vmlinux/advapi32_vmlinux_decompressed.bin`.
- Size: 8,709,472 bytes.
- SHA256: `821ed54f7118c63c15f7632648edf577c785012a3e09d178ac9c5f9e8e6c2b97`.
- The uImage load address is `0x80000000`, entry `0x803337c0`.
- Ghidra 12.0.4, raw binary loader, `MIPS:LE:32:default`, base `0x80000000`.
  Functions were disassembled and decompiled explicitly without a full automatic
  kernel analysis. Forced labels and C types are decompiler guesses.

The uncompressed exported symbol strings and absolute `__ksymtab` address/name
pairs identify these entry points independently of Ghidra's forced names:

| Exported name | Address | Address/name pair file offset |
|---|---|---|
| `hc16xx_set_cpu_clk` | `0x800011ec` | `0x43f438` |
| `clk_set_rate` | `0x802b61d8` | `0x443060` |
| `__clocksource_update_freq_scale` | `0x8006c2cc` | `0x4427a8` |

Headless command, from the repository root:

```powershell
$env:JAVA_HOME = 'C:\Program Files\Eclipse Adoptium\jdk-25.0.3.9-hotspot'
& .\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects r36sx_clock `
  -import .\disasm\disk_image\kernel\advapi32_vmlinux\advapi32_vmlinux_decompressed.bin `
  -processor MIPS:LE:32:default -loader BinaryLoader -loader-baseAddr 0x80000000 `
  -noanalysis -scriptPath .\ghidra_scripts `
  -postScript DecompileAddresses.java disasm/linux/hc16xx-clock/functions.c 800011ec 802b61d8 `
  -postScript DumpDisasmRanges.java disasm/linux/hc16xx-clock/register-switch.txt 800011ec 0x160
```

The first sandboxed launch could not access Ghidra's saved Java preferences;
the explicit JDK path and approved normal launch completed successfully.
No new tools were downloaded, and no device code was executed or modified.

## Findings and limits

`hc16xx_set_cpu_clk()` takes no frequency argument. It reads `/cpus/cpu@0`,
`mcpu-dig-pll-clk` and `clock` from the boot device tree, then programs the PLL
and clock selection. It writes selector bits 10:8 at uncached address
`0xb8800074` and triggers the change with bit 22. This matches the retained
vendor SDK's boot initialization and the saved DTS. It contains no calls to
update Linux clocksource/clockevent frequency or delay calibration.

This function is evidence of boot configuration, not a userspace frequency
service or a verified runtime transition API. `clk_set_rate()` is an exported
kernel wrapper, not a userspace syscall. Its presence does not establish that
the CPU clock provider and its consumers handle a runtime change correctly.

The raw kernel contains no literal `cpufreq` string. The returned device report
also had no CPUFreq current-frequency files and only the `MIPS` clocksource.
Together these support the lack of an established standard switching path;
they do not prove every possible vendor control path is absent.

References checked: retained HCRTOS source at
`1dd13c6ddc034313c0c3b9a31a123449f7baf110`,
`components/kernel/source/arch/mips/init_hook.c:mcpu_clock_init()` and
`drivers/standby/standby.c:standby_set_cpu_clock()`; saved DTS CPU clock binding;
Linux [Common Clock Framework](https://www.kernel.org/doc/html/latest/driver-api/clk.html)
and [CPUFreq architecture](https://www.kernel.org/doc/html/latest/admin-guide/pm/cpufreq.html).
The official Linux 4.4 source-file web endpoints failed to load during this
inquiry; no newer upstream implementation is asserted to describe this vendor
kernel's exact timing behavior.

No runtime frequency selector was installed in Tiny MC. Requirements for that
separate implementation are in `homebrew/tiny_mc/CPU_FREQUENCY.md`.
