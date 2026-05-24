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
