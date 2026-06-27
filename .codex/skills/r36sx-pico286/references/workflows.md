# R36SX Pico-286 Workflows

## Paths

- Active emulator source: `homebrew/pico_286`.
- R36SX-specific host port sources: `homebrew/pico_286/r36sx_port`.
- Upstream Pico-286 snapshot: `homebrew/pico_286/pico-286`.
- Active patch/test copy:
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286`.
- Patch disk images:
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/images`.
- DOS test tools:
  `patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools`.

## Builds

Prefer the Windows build while debugging CPU, BIOS, VGA, disk, DOS, UI, mouse,
keyboard, Sound Blaster, or host RPC behavior:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew\pico_286\build_pico_286_windows.ps1 -DebugLog
```

Use WSL/GCC for the MIPS/device binary:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew\pico_286\build_pico_286_wsl.ps1 -OptLevel O3 -Strip -Out .\homebrew\pico_286\pico_286
```

The MIPS build uses the Linux `mips-mti-linux-gnu-gcc` toolchain through WSL.
The older Zig/Clang MIPS build path is a fallback only. Normal release MIPS
builds should use `-OptLevel O3 -Strip`; add debug options only for an explicit
diagnostic task.

When the user asks to rebuild the patch Pico-286 binary, copy the fresh MIPS
binary to:

```text
patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/pico_286
```

After copying, update `[rtc] rtc_start_time` in that patch directory's
`pico_286.conf` to the current local time in `YYYY-MM-DD HH:MM:SS` format.

Computed-goto opcode dispatch is the default. Use `-DisableComputedGoto` only
when comparing against the switch decoder.

## Disk Images and DOS Tools

Use WSL `mtools` for FAT image edits. Prefer offset image specs such as:

```sh
MTOOLS_SKIP_CHECK=1 mcopy -o -i patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/images/hdd.hdd@@32256 FILE ::/FILE
```

Keep generated DOS `.COM` files in the patch `tools` directory, not in the
patch root. Current HOSTRPC/HOSTDRV sources live under:

```text
homebrew/pico_286/pico-286/tools/
```

Relevant files:

- `hostdrv.asm`: resident DOS network redirector.
- `hostrpc_test.asm`: HOSTRPC protocol test utility.
- `hostrpc.inc`: shared guest ABI constants.
- `homebrew/pico_286/HOST_RPC.md`: protocol notes.
- `homebrew/pico_286/pico-286/tools/HOSTDRV_REDIRECTOR.md`: redirector design.

## CPU and Emulation Notes

- Keep CPU model behavior separated: `exec86`, `exec286`, and `exec386` should
  avoid checks for higher CPU models in lower-model hot paths.
- Strict 8086 emulation should not raise later-generation `#UD`/INT 6 behavior
  for undefined opcodes or LOCK prefix edge cases.
- 8086 real-mode memory access must wrap to 20 bits.
- 286 paths should avoid unnecessary 386 checks and use the correct register
  widths.
- Preserve 286/386 behavior when changing shared CPU helpers; build and smoke
  test affected paths when possible.

## Patch Config and User Edits

The patch `pico_286.conf` often contains user-local test settings such as CPU
model, MHz, x87, and selected disk images. Do not stage it unless the user
explicitly asks or the task specifically changes committed patch defaults.

Generated diagnostic files belong under the active patch copy's `diagnostics/`
directory, not in the patch root. This includes `debug_*`, `diag_*`,
`memory_dump_*`, `emergency_dump_*`, logs, debug-control responses, framebuffer
captures, and ad-hoc memory/register dumps. Emulator code should route such
paths through `diagnostics_dir` and `r36sx_pico286_resolve_diagnostics_path()`.

Disk CHS should be inferred automatically when images are mounted. Avoid
reintroducing manual CHS settings in config unless a future compatibility task
requires an explicit override.

## Logging and Commits

Record reusable work in the most specific existing file:

- `homebrew/pico_286/BUILD_LOG.md` for builds, generated binaries, and image
  updates.
- `homebrew/pico_286/README.md` for durable usage and architecture notes.
- Protocol and redirector documents for HOSTRPC/HOSTDRV changes.
- Root analysis documents for research that future tasks need.

After a successful fix, make a focused commit. Check the branch's ahead count;
push after three local commits or when the user asks.
