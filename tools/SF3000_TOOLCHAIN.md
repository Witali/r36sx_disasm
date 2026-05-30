# SF3000 / R36SX MIPS toolchain

## What was installed locally

Downloaded archive:

```text
tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

Source:

```text
https://github.com/game-de-it/sf3000/releases/tag/sf3000_toolchain_v0.1
```

GitHub release notes say to extract:

```text
mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

and use:

```text
bin/mips-mti-linux-gnu-gcc
bin/mips-mti-linux-gnu-g++
bin/mips-mti-linux-gnu-ld
```

with:

```text
CFLAGS="-O2 -march=mips32r2"
```

## Current host limitation

The current Codex shell is Windows PowerShell. The SF3000 SDK binaries are Linux x86_64 ELF programs, not Windows `.exe` programs.

Example signature found in the compiler binary:

```text
/lib64/ld-linux-x86-64.so.2
```

So this SDK cannot run directly from Windows PowerShell. It should work from WSL/Linux after extraction with symlink support.

Windows `tar` also cannot fully extract the SDK because the archive contains Linux symlinks and hardlinks. It created a partial tree:

```text
tools\mipsel-buildroot-linux-gnu_sdk-buildroot
```

but that tree is not a reliable Windows installation.

## Recommended usage with WSL/Linux

From WSL/Linux, from the same workspace:

```sh
cd /mnt/c/Work/r36sx_disasm
mkdir -p tools/sf3000-linux
tar -xzf tools/mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz -C tools/sf3000-linux
cd tools/sf3000-linux/mipsel-buildroot-linux-gnu_sdk-buildroot
./relocate-sdk.sh
```

Then build the demo core:

```sh
cd /mnt/c/Work/r36sx_disasm/homebrew/libretro_button_demo
make CC=/mnt/c/Work/r36sx_disasm/tools/sf3000-linux/mipsel-buildroot-linux-gnu_sdk-buildroot/bin/mips-mti-linux-gnu-gcc
```

If the compiler is actually under `opt/ext-toolchain/bin`, use:

```sh
make CC=/mnt/c/Work/r36sx_disasm/tools/sf3000-linux/mipsel-buildroot-linux-gnu_sdk-buildroot/opt/ext-toolchain/bin/mips-mti-linux-gnu-gcc
```

Expected output:

```text
homebrew/libretro_button_demo/libemu_buttondemo.so
```

## Why this SDK is still the best match

- Target is MIPS little-endian Linux.
- Tool prefix is `mips-mti-linux-gnu-`.
- Suggested flags include `-march=mips32r2`.
- This matches the device family better than a generic bare-metal `mipsel-elf` compiler or a musl toolchain.

For replacing `cubegm/cores/*.so`, prefer this SDK over bare-metal MIPS toolchains.

## Windows-hosted GCC experiment

Installed test toolchain:

```text
tools\mips_gcc_windows\g++-mipsel-none-elf-10.3.0
```

Source found through the PCSX-Redux build instructions:

```text
https://static.grumpycoder.net/pixel/mips/g++-mipsel-none-elf-10.3.0.zip
```

Security checks completed before keeping the toolchain:

```powershell
.\tools\scan-download.ps1 .\tools\downloads\g++-mipsel-none-elf-10.3.0.zip
.\tools\scan-download.ps1 .\tools\mips_gcc_windows\g++-mipsel-none-elf-10.3.0
```

Both Windows Defender scans reported no threats.

Compiler identity:

```text
mipsel-none-elf-gcc.exe (GCC) 10.3.0
```

This is a Windows executable GCC that generates little-endian MIPS, but its
target is `mipsel-none-elf`, not `mipsel-linux-gnu`. It is useful for testing
GCC code generation, CPU tuning, and DSP flags, but it is not a direct
replacement for the Linux/uClibc SDK.

Useful observations:

- `-march=mips32r2 -mtune=74kc` is accepted.
- `-mdsp` and `-mdspr2` are accepted.
- A minimal `-nostdlib` bare-metal ELF can be linked by this toolchain.
- Direct `.o` files from this GCC are not accepted cleanly by `zig ld.lld` in
  our current Linux executable link step (`invalid binding` symbol errors).
- The workable route is: GCC emits assembly, Zig/LLVM assembles it for
  `mipsel-linux-gnu`, and Zig LLD links with the existing sysroot.

Proof-of-concept command sequence for native Pong:

```powershell
& "tools\mips_gcc_windows\g++-mipsel-none-elf-10.3.0\bin\mipsel-none-elf-gcc.exe" `
    -O2 -march=mips32r2 -mtune=74kc -mabicalls -fPIC -G0 -nostdinc `
    "-isystemtools\mips_gcc_windows\g++-mipsel-none-elf-10.3.0\lib\gcc\mipsel-none-elf\10.3.0\include" `
    "-isystemtools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\include" `
    -Wall -Wextra -std=c99 -DR36SX_PONG_TARGET=2 `
    -S "homebrew\pong\pong.c" -o ".tmp\pong_gcc_mtune74kc.s"

& "tools\zig-x86_64-windows-0.16.0\zig.exe" clang `
    --target=mipsel-linux-gnu -march=mips32r2 `
    -c ".tmp\pong_gcc_mtune74kc.s" -o ".tmp\pong_gcc_via_zig_as.o"

& "tools\zig-x86_64-windows-0.16.0\zig.exe" ld.lld `
    "--sysroot=tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot" `
    -EL -m elf32ltsmip "--dynamic-linker=/lib/ld.so.1" "--hash-style=sysv" `
    "-Ltools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\lib" `
    "-Ltools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\lib" `
    -o ".tmp\pong_gcc_via_zig_as" `
    "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\lib\crt1.o" `
    "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\lib\crti.o" `
    ".tmp\pong_gcc_via_zig_as.o" "-ldl" "-lc" `
    "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\lib\crtn.o"
```

The resulting test executable:

```text
.tmp\pong_gcc_via_zig_as
ELF32, little-endian MIPS, o32, mips32r2, CPIC
interpreter: /lib/ld.so.1
needed: libdl.so.2, libc.so.6
size: 16652 bytes
```

This path has not yet replaced the normal build scripts. Treat it as an
experimental route for testing whether GCC 10.3.0 with `-mtune=74kc` produces
better code than Zig/Clang for hot files.
