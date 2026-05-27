# Local Emulation Options

This file records what can realistically be emulated locally for the inspected
R36SX / GB350 / SF3000-like MIPS handheld firmware.

## Short Answer

Yes, we can run some code from this console locally, but there are different
levels of emulation:

1. MIPS Linux user-mode emulation: realistic and useful.
2. Libretro core harness emulation: realistic and probably the best next step
   for testing our `.so` modules.
3. Full console/system emulation: theoretically possible, but not currently
   practical without implementing the Hichip HC16xx board/devices.

## Current Local State

Checked on 2026-05-27 from Windows PowerShell.

Windows QEMU binaries were not found in `PATH`:

```text
qemu-mipsel.exe
qemu-mipsel-static.exe
qemu-system-mipsel.exe
```

The downloaded SF3000/MIPS SDK contains QEMU user-mode binaries:

```text
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/qemu-mipsel
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/qemu-mips64el
```

`qemu-mipsel` from that SDK is an ELF64 x86_64 Linux executable, not a Windows
`.exe`, so it cannot run directly from the current PowerShell session.

WSL is available as `wsl.exe`, but no Linux distribution is installed yet. Running
`wsl.exe -l -v` reported that Windows Subsystem for Linux has no installed
distributions and suggested `wsl.exe --install`.

## Portable QEMU for Windows

Downloaded and checked on 2026-05-27.

Source:

- QEMU download page: <https://www.qemu.org/download/>
- Windows binary provider linked by QEMU: <https://qemu.weilnetz.de/w64/>
- Downloaded installer:
  `https://qemu.weilnetz.de/w64/qemu-w64-setup-20260501.exe`
- Downloaded checksum:
  `https://qemu.weilnetz.de/w64/qemu-w64-setup-20260501.sha512`

Local files:

```text
tools/qemu-windows-download/qemu-w64-setup-20260501.exe
tools/qemu-windows-download/qemu-w64-setup-20260501.sha512
tools/qemu-windows-portable/qemu/
```

The SHA512 file matched the downloaded installer:

```text
3d6b996bb904666f3b7ff62bed233b2d21dffe96f512af0a7151cfcc828bc5c8f9b62623cf2a9d363a3ae48111761f4630cce776eacb9b70d83e61e6ae50de47
```

Security checks:

- Scanned the downloaded installer with `tools/scan-download.ps1`: no threats.
- Scanned the extracted/local install directory with `tools/scan-download.ps1`:
  no threats.

The file is an NSIS installer, not a plain ZIP archive. It was installed into a
workspace-local directory with:

```powershell
.\tools\qemu-windows-download\qemu-w64-setup-20260501.exe /S /D=C:\Work\r36sx_disasm\tools\qemu-windows-portable\qemu
```

Installed version:

```text
QEMU emulator version 11.0.50 (v11.0.0-12631-g54e84cdc7a)
```

Useful MIPS system emulators now available:

```text
tools/qemu-windows-portable/qemu/qemu-system-mips.exe
tools/qemu-windows-portable/qemu/qemu-system-mipsel.exe
tools/qemu-windows-portable/qemu/qemu-system-mips64el.exe
```

`qemu-system-mipsel.exe -machine help` reports `malta`, so generic MIPS system
emulation is available.

Important limitation: this Windows package does not include user-mode
`qemu-mipsel.exe`. It can help with generic MIPS virtual machines, but it cannot
directly execute firmware ELF programs such as `disk_image/cubegm/rkgame`.
For that, use Linux/WSL with the SDK-provided `qemu-mipsel`, or find a Windows
QEMU build that includes user-mode MIPS binaries.

## What User-Mode QEMU Can Do

With a working Linux/WSL environment, `qemu-mipsel` should be able to execute
ELF32 little-endian MIPS userspace programs from this firmware.

Useful targets:

- `disk_image/cubegm/icube`
- `disk_image/cubegm/rkgame`
- `disk_image/cubegm/MyExecutable`
- `disk_image/rootfs/usr/bin/cubevol`
- small MIPS test programs that we build ourselves

Typical command shape under Linux/WSL:

```sh
cd /mnt/c/Work/r36sx_disasm
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/qemu-mipsel \
  -L disk_image/rootfs \
  disk_image/cubegm/icube
```

For binaries that expect libraries from `cubegm/lib`, the environment would need
to be adjusted:

```sh
LD_LIBRARY_PATH=/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib \
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/qemu-mipsel \
  -L disk_image/rootfs \
  disk_image/cubegm/rkgame
```

However, many stock programs expect the real device layout:

```text
/mnt/sdcard/cubegm
/dev/fb0
/dev/backlight
/dev/mipi
/dev/standby
/dev/input/event*
```

Those paths either need a prepared fake root/layout, stubs, or they will fail.

## Best Practical Target: A MIPS Libretro Harness

The most useful local emulation path for our current work is not booting the
whole launcher. It is building a small MIPS Linux test harness that:

1. `dlopen()`s one of our cores, for example `libemu_buttondemo.so` or
   `libemu_fuse.so`.
2. Resolves the `retro_*` symbols and launcher compatibility symbols.
3. Supplies fake libretro callbacks:
   - video callback that writes raw RGB565 frames to files;
   - audio callback that writes PCM samples to files;
   - input callback that simulates button presses;
   - environment callback that imitates the subset of `rkgame` we know.
4. Calls `retro_init()`, `retro_load_game()`, and `retro_run()` for several
   frames.

Then run that harness under `qemu-mipsel`.

This would let us test:

- whether the `.so` loads at all under MIPS dynamic linking;
- whether required exports are present;
- whether `retro_load_game()` returns true;
- whether `retro_run()` produces video/audio without crashing;
- whether Button Demo audio callbacks work;
- whether Fuse returns early because of frontend/environment expectations.

This will not prove the real device will run the core, but it would give much
better diagnostics than repeatedly copying to the SD card.

## Full System Emulation

Full system emulation would mean booting the vendor kernel and running the whole
rootfs/cubegm environment in a virtual machine.

This is hard because the firmware is tied to a Hichip HC16xx / HC1600A-like SoC
and board. QEMU has generic MIPS machines such as `malta`, but those do not
emulate the device-specific hardware used by this firmware:

- MIPI/display pipeline;
- framebuffer/display driver details;
- backlight device;
- standby device;
- input wiring;
- AVP/vendor co-processor or companion firmware;
- Hichip-specific platform drivers from the DTB/kernel.

Booting the extracted kernel on a generic QEMU MIPS machine is therefore unlikely
to reach the real launcher unchanged. It would probably require either a custom
QEMU board model or a modified kernel/rootfs that replaces hardware-specific
drivers with generic virtual devices.

## Recommended Next Step

Install or use a Linux environment with QEMU user-mode support, then build the
MIPS libretro harness described above.

Priority order:

1. Use WSL/Linux and the SDK-provided `qemu-mipsel`.
2. If that is inconvenient, find a Windows QEMU build that includes
   `qemu-mipsel.exe`.
3. Build a small MIPS harness for our `.so` cores.
4. Use it to test Button Demo and Fuse locally before copying patches to the
   device.

This gives us local execution of the console's MIPS code without pretending to
fully emulate the Hichip hardware.
