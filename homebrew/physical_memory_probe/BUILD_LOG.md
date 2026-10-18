# Physical memory probe build log

## 2026-10-18: standalone SD diagnostic

Purpose: read total hardware DDR configuration independently of Linux's RAM
reservation, following the actual stock bootloader's live-register `meminfo`.
Reverse engineering, input hash, source differences and exact formula are in
`disasm/mips/r36sx-linux-boot/DDR_READOUT.md`.

Build from repository root using the existing WSL/Codescape SDK:

```powershell
wsl bash -lc 'cd /mnt/c/Work/r36sx_disasm && bash homebrew/physical_memory_probe/build_wsl.sh'
```

- Compiler: `mips-mti-linux-gnu-gcc`, Codescape GNU Tools 2018.09-02, GCC 6.3.0.
- Flags: `--sysroot=<SDK>/opt/ext-toolchain/sysroot/mipsel-r2-soft`
  `-EL -mips32r2 -mabi=32 -msoft-float -O2 -Wall -Wextra -Werror -std=gnu11 -static`.
- Relocated SDK paths: `-isystem <sysroot>/usr/include`,
  `-B<sysroot>/usr/lib/`, `-L<sysroot>/usr/lib`, `-L<sysroot>/lib`.
- Stripped with matching `mips-mti-linux-gnu-strip`.
- Artifact:
  `patches/disk_image_patch_physical_memory_probe/MIPS_NATIVE/physical_memory_probe/physical_memory_probe`.
- Final size: **619,716 bytes**.
- SHA256: `ab75c1b54a8a86ae1a0201ec337b06635234222edffef910f1e3bc537bab7f6a`.
- ELF32 little-endian, MIPS32r2, O32, static; readelf shows no interpreter
  or dynamic segment. Project `elf_mips_summary.py` agrees.

Initial sandboxed WSL access returned `WSL/E_ACCESSDENIED`; the authorized
WSL invocation succeeded. The first compile produced an ELF but packaging
failed because README had not yet been created. After adding documentation
and simplifying the readout to two register addresses, the final build and
packaging completed without warnings under `-Werror`.

Checks:

1. WSL host GCC built the same source with `-O2 -Wall -Wextra -Werror -std=gnu11`.
2. `test_probe.py` passed five valid capacity/type combinations, including
   DDR2/DDR3 and 128/256 MiB, plus five invalid-value cases and CLI validation.
3. Two native host runs with stdout closed produced distinct complete reports
   ending in `Done.`; skipped hardware was clearly UNKNOWN.
4. The final MIPS executable passed the same decoder/error cases under existing
   SDK `qemu-mipsel` via:

   ```sh
   python3 homebrew/physical_memory_probe/test_probe.py \
     patches/disk_image_patch_physical_memory_probe/MIPS_NATIVE/physical_memory_probe/physical_memory_probe \
     tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/qemu-mipsel
   ```

5. All 81 instructions in the DDR readout export matched the raw Stage 3 bytes.
6. `tools/scan-download.ps1 patches/disk_image_patch_physical_memory_probe`
   completed with Microsoft Defender: **no threats**. No tools were downloaded.

The program opens `/dev/mem` read-only, maps it PROT_READ, checks the HC16xx
family, reads the DDR field twice, and never writes MMIO, physical RAM or MTD.
Report files under its own `diagnostics/` are the only persistent device
writes. Register faults are isolated with a three-second child alarm.

Limit: hardware-configured capacity is not an independent memory-cell test.
No hardware run, live DDR result or SD-card deployment occurred in this task.
The user can copy only the new application folder and run it through Tiny MC.

## 2026-10-18: deployed to the user-connected console SD card

The user explicitly requested copying the program to the inserted card.
Identified `E:` as a removable FAT32 volume (31,263,981,568 bytes), with the
existing R36SX `cubegm`, `rootfs` and `MIPS_NATIVE` directories.

Copied only these two files using PowerShell `Copy-Item -LiteralPath` into the
new `E:/MIPS_NATIVE/physical_memory_probe/` directory, then called
`FileStream.Flush(true)` on each destination and compared source/destination
SHA256 with `Get-FileHash`:

| File | Size | Verified destination SHA256 |
| --- | ---: | --- |
| `physical_memory_probe` | 619716 | `ab75c1b54a8a86ae1a0201ec337b06635234222edffef910f1e3bc537bab7f6a` |
| `README.md` | 4517 | `42a3235f5899a4eb25a13bd87fdf531601a8d7890ffeb5038fc1030a5710a0ce` |

Both hashes match the repository files. The binary is unchanged from the
Defender-scanned build above. No existing application, boot file, DTB, AVP,
kernel, NOR content or Pico-286 configuration was changed. The diagnostic has
not been executed on hardware; the user can launch it in Tiny MC and return
the new `diagnostics/physical_memory_*.txt` report.

## 2026-10-18: two complete device reports recovered

The user ran the program and returned the card. Recovered
`physical_memory_nsxaOY.txt` and `physical_memory_R9FyQc.txt`, each 2,163 bytes,
to `hardware_info/physical_memory_probe/2026-10-18/`. Source/destination SHA256
matched; full hashes and paths are in the saved `manifest.json`.

Both runs report chip `0x1600ac01`, DDR register `0x0d894084`, **DDR3 and
256 MiB configured controller capacity**, with Linux `MemTotal: 43164 kB`
and kernel build #21. Both completed without a reported hardware-read fault.
The binary on the returned card still matches SHA256
`ab75c1b54a8a86ae1a0201ec337b06635234222edffef910f1e3bc537bab7f6a`.

The earlier assumption of 128 MiB installed solely from the smaller layout is
withdrawn. This measures controller configuration, not an independent test of
all physical cells or address aliases. See `hardware_info/physical_memory_probe/README.md`.
The host read/copied reports only; the card and firmware were not modified.
