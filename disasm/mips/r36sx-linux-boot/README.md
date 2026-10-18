# R36SX Linux boot and memory-variant analysis

Research date: 2026-10-18. Ghidra 12.0.4, Java 25.0.3,
language/compiler `MIPS:LE:32:default:default`, raw base `0x89da0000`.

Follow-up: [DDR_READOUT.md](DDR_READOUT.md) identifies the stock bootloader's
live DDR-controller capacity field and documents an SD-launched diagnostic.
Two returned device runs now report DDR3 and **256 MiB configured capacity**.
The earlier 128 MiB installed-capacity inference from the smaller layout is
withdrawn; see `hardware_info/physical_memory_probe/README.md` for limits.

Input: `disasm/mtd/stage3_89da0000.bin`, 1,106,748 bytes,
SHA256 `b9a9d1ae074bcd68109161c1219fe98b8c4f7e3fd70045b394dd9aa4ab15981d`.
It was previously extracted from `hardware_info/mtd-dump/mtd1_boot.bin`;
see `disasm/mtd/unpack_stage3_lzma.py` and the MTD notes.

## Results

| Address | Interpretation | Export |
| --- | --- | --- |
| `0x89da926c` | Resolve filename, optionally substitute memory-variant alias, check `/media/<device>/<prefix>/<filename>` | `stage3_boot_selected.c` |
| `0x89da9474` | Resolve internal/external boot resource using DTB properties | `stage3_boot_selected.c` |
| `0x89daa7ac` | `bootm` wrapper, state mask `0x74f` | `stage3_boot_selected.c` |
| `0x89daaa28` | Read 64-byte legacy header, allocate `payload-size + 64`, read image | `stage3_boot_selected.c` |
| `0x89dad8b0` | Boot state machine: magic, header/data CRC, architecture, image type, load/entry, decompression, dispatch | `stage3_image_validation_handoff.c` |
| `0x89dadec0` | Header CRC verification with CRC field cleared | `stage3_image_validation_handoff.c` |
| `0x89dadf18` | Payload CRC verification | `stage3_image_validation_handoff.c` |
| `0x89dae5ec` | Linux handler: indirect call to image entry with `(-2, dtb, 0, 0)` | `stage3_image_validation_handoff.c` |
| `0x89daa730` | RAM-alias probe controlling filename selection | `stage3_memory_variant.c`, **see caveat below** |

The observed legacy-image path validates CRCs and MIPS architecture, then uses
the header entry point. No cryptographic signature check was identified on
this path. This is a statement about the inspected SD-image path, not an audit
of every BootROM or firmware-update path.

The state machine supplies `0x4000000` as the decompressor output limit. Do not
treat that 64 MiB value as an available-RAM measurement or a guaranteed usable
kernel/initramfs size.

## Memory selection: disassembly takes precedence over decompiler output

At `0x89daa758..0x89daa770` the instructions do the following:

```c
/* Interpretation of instructions, not original source. Accesses are volatile. */
*(volatile unsigned int *)0xa8000020 = 0;
*(volatile unsigned int *)0xa0000020 = 0xaabbccdd;
if (*(volatile unsigned int *)0xa8000020 == 0xaabbccdd)
    *(volatile unsigned int *)0x89ea5290 = 0;
```

The selector initially contains `1`. If the two RAM addresses alias, the
selector becomes zero and `FUN_89da926c` uses the table at `0x89ea51d0`:

| Requested name | Name when selector is zero |
| --- | --- |
| `vmlinux.uImage` | `advapi32.dll` |
| `dtb.bin` | `Bubbles.scr` |
| `dtc.bin` | `Bubbles.scr` |
| `avp.uImage` | `ApplicationFrame.dll` |
| `xgame-logo.bmp` | `xgame-logo.bmp` |
| `app.uImage` | `app.uImage` |

Linux's MIPS `CPHYSADDR` definitions map the probe addresses to physical
`0x08000020` and `0x00000020`, 128 MiB apart. Selection by RAM aliasing,
the 128 MiB shift between the two DTBs, and the two AVP load addresses are
evidence for smaller/larger address layouts. They do not independently prove
the amount of physically installed RAM: the returned live controller readout
reports 256 MiB despite the smaller Linux layout.

**Ghidra's C output is wrong for this probe.** It reports removal of an
unreachable block and omits the conditional selector store. It assumes
distinct RAM addresses instead of accounting for hardware aliasing. It also
extends the apparent function past a non-returning call. Preserve the raw
export as evidence of the limitation; use `memory_probe_and_linux_jump.s` for
the actual comparison, branch, delay slot, and selector store.

The saved live-device `uname.txt` reports kernel build `#21`, which matches
`advapi32.dll`; `vmlinux.uImage` contains build `#7`. The saved 42.2 MiB
`MemTotal` is also consistent with the smaller `Bubbles.scr` Linux region.
These identify the smaller boot layout, not the installed physical capacity.
The new live reports still show #21 and 42.15 MiB Linux MemTotal while the DDR
controller's size field says 256 MiB.

## Reproduction and tool log

Project: `ghidra_projects/r36sx_linux_feasibility` (ignored local state).
The initial import used auto-analysis with a 120-second per-file limit;
analysis completed successfully in approximately 43 seconds. Later exports
used `-process stage3_89da0000.bin -noanalysis`.

Equivalent normal headless arguments, from the repository root:

```powershell
$headless = '.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat'
& $headless .\ghidra_projects r36sx_linux_feasibility `
  -import .\disasm\mtd\stage3_89da0000.bin `
  -loader BinaryLoader -loader-baseAddr 0x89da0000 `
  -processor MIPS:LE:32:default -analysisTimeoutPerFile 120 `
  -scriptPath .\ghidra_scripts `
  -postScript DecompileAddresses.java `
  C:\Work\r36sx_disasm\disasm\mips\r36sx-linux-boot\stage3_boot_selected.c `
  89da926c 89da9474 89daa7ac 89daaa28
```

Subsequent `DecompileAddresses.java` exports used:

- `stage3_image_validation_handoff.c`: `89dad8b0 89dae5ec 89dadec0 89dadf18`.
- `stage3_memory_variant.c`: `89daa730`.
- `DumpDisasmRanges.java memory_probe_and_linux_jump.s`:
  `89daa730 0x7c 89dae5ec 0xb8`.

The standard launcher failed while trying to save Java selection under the
restricted user profile. Successful runs invoked the existing Java executable
directly, with the following prefix followed by the headless arguments above:

```powershell
& 'C:/Program Files/Eclipse Adoptium/jdk-25.0.3.9-hotspot/bin/java.exe' `
  '-Duser.home=C:/Work/r36sx_disasm/local_artifacts/ghidra-linux-feasibility-home' `
  '-Dapplication.settingsdir=C:/Work/r36sx_disasm/local_artifacts/ghidra-linux-feasibility-home' `
  '-Djava.system.class.loader=ghidra.GhidraClassLoader' `
  '-Dfile.encoding=UTF8' '-Duser.language=en' '-Duser.country=US' '-Xmx2G' `
  -cp .\ghidra_12.0.4_PUBLIC\Ghidra\Framework\Utility\lib\Utility.jar `
  ghidra.Ghidra ghidra.app.util.headless.AnalyzeHeadless
```

Logs: `ghidra.log` in this directory (ignored). No firmware was executed,
rebuilt, flashed, or deployed. No tools or executable packages were downloaded;
the existing local Ghidra/JDK and Python runtime were used.
Trailing whitespace and surplus end-of-file blank lines in the C exports
were normalized after export; no decompiler statements were changed.

## Sources checked

- [Linux MIPS address definitions](https://raw.githubusercontent.com/torvalds/linux/master/arch/mips/include/asm/addrspace.h):
  `CPHYSADDR`, `KSEG0`, `KSEG1`.
- [U-Boot bootm documentation](https://docs.u-boot.org/en/latest/usage/cmd/bootm.html):
  legacy image and `bootm <image> - <fdt>` arguments.
- [U-Boot image definitions](https://raw.githubusercontent.com/u-boot/u-boot/v2024.01/include/image.h).
- Local HCRTOS source mirror,
  `internet_sources/hcrtos/components/applications/apps-bootloader/source/`:
  `cmd/mtdload.c`, `cmd/bootm.c`, `common/bootm.c`, `common/bootm_os.c`,
  `main.c`. It corroborates the boot state machine and `kernel(-2, dtb, 0, 0)`;
  mirror HEAD `1dd13c6ddc034313c0c3b9a31a123449f7baf110`, remote
  `https://git.maschath.de/ignatz/hcrtos.git`. It is not assumed to be the exact
  source revision of the stock binary.

Verification: all 77 instruction byte sequences in the selected `.s` export
were compared with the raw Stage 3 bytes and matched. Independent Python
checks passed for both CRCs of all four stock uImages, kernel build identities,
380-entry CPIO inventory, and identical extracted startup text.
