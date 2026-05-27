# Fuse ZX Spectrum 48K Build Commands

This file documents the practical build procedure used for the experimental
R36SX / `cubegm/rkgame` Fuse ZX Spectrum 48K core.

The build was done from Windows PowerShell in:

```text
C:\Work\r36sx_disasm
```

## Inputs

Source tree:

```text
internet_sources/fuse-libretro
```

Source repository and revision:

```text
https://github.com/libretro/fuse-libretro.git
commit bce196f
```

Local compiler:

```text
tools/zig-x86_64-windows-0.16.0/zig.exe
```

MIPS sysroot used for headers and runtime library names:

```text
tools/mipsel-buildroot-linux-gnu_sdk-buildroot/mipsel-buildroot-linux-gnu/sysroot
```

Runtime libraries copied from the inspected firmware image for explicit
`NEEDED` entries:

```text
disk_image/cubegm/lib/libm.so.6
disk_image/cubegm/lib/libgcc_s.so.1
disk_image/cubegm/lib/libc.so.6
```

R36SX compatibility files:

```text
homebrew/fuse_zx48/r36sx_shim.c
homebrew/fuse_zx48/r36sx_link.T
homebrew/fuse_zx48/r36sx_fuse_compat.patch
homebrew/fuse_zx48/libemu_fuse.so.cfg
homebrew/fuse_zx48/R36SX_ZX48.tap
```

## Download And Scan

The source tree was downloaded with:

```powershell
git clone https://github.com/libretro/fuse-libretro.git internet_sources/fuse-libretro
```

The downloaded source tree was scanned before use:

```powershell
.\tools\scan-download.ps1 .\internet_sources\fuse-libretro
```

## Generated Source Files

The upstream tree needs generated configuration/version files before compiling
without its normal Makefile flow:

```text
internet_sources/fuse-libretro/fuse/config.h
internet_sources/fuse-libretro/libspectrum/config.h
internet_sources/fuse-libretro/src/version.c
```

They were generated locally during the first manual build. The values are kept
minimal and match the local MIPS/Linux libretro build assumptions:

- `__LIBRETRO__`
- `HAVE_CONFIG_H`
- internal zlib and bzip2 sources from the Fuse tree
- little-endian MIPS Linux target

## R36SX Source Patch

The first device test returned to the launcher after a black screen. The
important compatibility patch is stored in:

```text
homebrew/fuse_zx48/r36sx_fuse_compat.patch
```

It changes `src/libretro.c` so `retro_load_game()` does not fail if `rkgame`
ignores this frontend request:

```text
RETRO_ENVIRONMENT_SET_PIXEL_FORMAT
```

The core continues with RGB565, which is the same pixel format used by the
working Button Demo core.

## Compile Flags

All Fuse C sources are compiled with Zig as a MIPS little-endian Linux compiler:

```text
zig cc
-target mipsel-linux-gnu
-march=mips32r2
--sysroot=<sysroot>
-isystem <sysroot>/usr/include
-O2
-fomit-frame-pointer
-fPIC
-D__LIBRETRO__
-DHAVE_CONFIG_H
-Wall
```

Include directories:

```text
-I internet_sources/fuse-libretro
-I internet_sources/fuse-libretro/src
-I internet_sources/fuse-libretro/src/compat
-I internet_sources/fuse-libretro/fuse
-I internet_sources/fuse-libretro/fuse/compat
-I internet_sources/fuse-libretro/libspectrum
-I internet_sources/fuse-libretro/zlib
-I internet_sources/fuse-libretro/bzip2
```

The source file list comes from:

```text
internet_sources/fuse-libretro/build/Makefile.common
```

The manual build script parsed every active `SOURCES_C += ...` entry in that
file and compiled each source into:

```text
internet_sources/fuse-libretro/build-r36sx/*.o
```

One example compile command:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc `
  -target mipsel-linux-gnu `
  -march=mips32r2 `
  --sysroot=.\tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot `
  -isystem .\tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot\usr\include `
  -O2 `
  -fomit-frame-pointer `
  -fPIC `
  -D__LIBRETRO__ `
  -DHAVE_CONFIG_H `
  -Wall `
  -I .\internet_sources\fuse-libretro `
  -I .\internet_sources\fuse-libretro\src `
  -I .\internet_sources\fuse-libretro\src\compat `
  -I .\internet_sources\fuse-libretro\fuse `
  -I .\internet_sources\fuse-libretro\fuse\compat `
  -I .\internet_sources\fuse-libretro\libspectrum `
  -I .\internet_sources\fuse-libretro\zlib `
  -I .\internet_sources\fuse-libretro\bzip2 `
  -c .\internet_sources\fuse-libretro\src\libretro.c `
  -o .\internet_sources\fuse-libretro\build-r36sx\src__libretro.c.o
```

## Compatibility Shim

The stock launcher expects more than the standard libretro symbols. The shim is:

```text
homebrew/fuse_zx48/r36sx_shim.c
```

It exports:

```c
int check_encrypty(void) { return 1; }
int CheckEncrypty(const void *data) { (void)data; return 1; }
```

The shim is compiled with the same target flags:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc `
  -target mipsel-linux-gnu `
  -march=mips32r2 `
  -O2 `
  -fomit-frame-pointer `
  -fPIC `
  -c .\homebrew\fuse_zx48\r36sx_shim.c `
  -o .\internet_sources\fuse-libretro\build-r36sx\r36sx_shim.c.o
```

## Link Command

The first build used `-nostdlib` and produced an empty `NEEDED` list. That was
too optimistic for `rkgame`.

The current compatibility build keeps `-nostdlib` to avoid Zig trying to provide
its own Linux libc, but it passes the firmware runtime libraries explicitly so
the dynamic section gets normal `NEEDED` entries.

Current link flags:

```text
zig cc
-target mipsel-linux-gnu
-march=mips32r2
-shared
-nostdlib
-Wl,--no-as-needed
-Wl,-soname,libemu_fuse.so
-Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T
-o homebrew/fuse_zx48/libemu_fuse.so
<all build-r36sx/*.o>
disk_image/cubegm/lib/libm.so.6
disk_image/cubegm/lib/libgcc_s.so.1
disk_image/cubegm/lib/libc.so.6
```

The link version script is:

```text
homebrew/fuse_zx48/r36sx_link.T
```

It exports only:

```text
retro_*
check_encrypty
CheckEncrypty
```

Example link command shape:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc `
  -target mipsel-linux-gnu `
  -march=mips32r2 `
  -shared `
  -nostdlib `
  -Wl,--no-as-needed `
  -Wl,-soname,libemu_fuse.so `
  -Wl,-version-script=.\homebrew\fuse_zx48\r36sx_link.T `
  -o .\homebrew\fuse_zx48\libemu_fuse.so `
  <all .\internet_sources\fuse-libretro\build-r36sx\*.o> `
  .\disk_image\cubegm\lib\libm.so.6 `
  .\disk_image\cubegm\lib\libgcc_s.so.1 `
  .\disk_image\cubegm\lib\libc.so.6
```

## Verification Commands

The built module is checked for:

- ELF32 little-endian MIPS shared object.
- MIPS flags `0x70001007`.
- required exports.
- `NEEDED` entries.
- SHA256.

The current verified output is:

```text
homebrew/fuse_zx48/libemu_fuse.so
size:   6713460
sha256: 29DEE42C31586965415A57CE69A17E19DB0AD3D813656CFEDCF2EAA9AE9AE10D
NEEDED: libm.so.6, libgcc_s.so.1, libc.so.6
```

The Defender scan command is:

```powershell
.\tools\scan-download.ps1 .\homebrew\fuse_zx48\libemu_fuse.so
```

The copied patch artifact is scanned too:

```powershell
.\tools\scan-download.ps1 .\disk_image_patch_012\cubegm\cores\libemu_fuse.so
```

## Minimal No-NEEDED Rebuild

After the device also rejected the explicit-`NEEDED` build, a smaller build was
produced for `disk_image_patch_013`.

This build uses the same source patch and shim, but compiles all sources into a
separate object directory:

```text
internet_sources/fuse-libretro/build-r36sx-oz
```

Compile flags:

```text
zig cc
-target mipsel-linux-gnu
-march=mips32r2
--sysroot=<sysroot>
-isystem <sysroot>/usr/include
-Oz
-fomit-frame-pointer
-fPIC
-ffunction-sections
-fdata-sections
-fno-unwind-tables
-fno-asynchronous-unwind-tables
-fno-ident
-fmerge-all-constants
-fno-stack-protector
-fno-math-errno
-D__LIBRETRO__
-DHAVE_CONFIG_H
-DNDEBUG
-Wall
```

Link flags:

```text
zig cc
-target mipsel-linux-gnu
-march=mips32r2
-shared
-nostdlib
-Wl,--gc-sections
-Wl,--strip-all
-Wl,--discard-all
-Wl,-soname,libemu_fuse.so
-Wl,-version-script=homebrew/fuse_zx48/r36sx_link.T
-o homebrew/fuse_zx48/libemu_fuse.so
<all build-r36sx-oz/*.o>
```

No `libm.so.6`, `libgcc_s.so.1`, or `libc.so.6` files are passed to this link.
The resulting dynamic `NEEDED` list is empty.

The attempted fully static shared-object build with `libc.a` / `libm.a` did not
work: the SDK static libraries contain non-PIC MIPS relocations such as
`R_MIPS_HI16` and `R_MIPS_26`, which cannot be embedded into a MIPS shared
object.

Current minimal output:

```text
homebrew/fuse_zx48/libemu_fuse.so
size:   2254084
sha256: 64ED74EDA1873D7B0E5A3BDCFFB3EF4B85CB38A22DA5054E65E6F32B8E4BA179
NEEDED: empty
undefined dynamic symbols: 75
```

Important caveat: this is not a completely self-contained `.so`. It has no
explicit library dependencies, but still has unresolved standard C/POSIX symbols
that must be supplied by the already-running process or loader.

The current `r36sx_shim.c` also defines local `exit()` and `abort()` traps. They
are intentionally not exported, but they prevent Fuse-internal error paths from
calling the process-wide libc implementations and terminating `rkgame`.

## Patch Directory Creation

The working image copy is updated with:

```powershell
Copy-Item -LiteralPath .\homebrew\fuse_zx48\libemu_fuse.so `
  -Destination .\disk_image\cubegm\cores\libemu_fuse.so `
  -Force
```

The test overlay is:

```text
disk_image_patch_012
```

It contains:

```text
ATARI/filelist.csv
ATARI/R36SX_ZX48.tap
cubegm/cores/config.xml
cubegm/cores/filelist.xml
cubegm/cores/libemu_fuse.so
cubegm/cores/libemu_fuse.so.cfg
MANIFEST.md
```

The files are copied from the updated `disk_image` tree and from
`homebrew/fuse_zx48/libemu_fuse.so`.

## Scripts Actually Invoked

Persistent scripts:

```text
tools/scan-download.ps1
```

One-shot PowerShell/Python build drivers:

- inline Python was used to parse `build/Makefile.common`, generate the object
  list, and invoke `zig cc` for each source file.
- inline Python was used to invoke the final `zig cc` link command with all
  `build-r36sx/*.o` objects.
- inline Python was used to inspect ELF dynamic sections and verify `NEEDED`
  entries, undefined symbols, and required exports.

These one-shot drivers were not saved as standalone scripts; this file records
the exact flags, inputs, outputs, and persistent helper scripts used.
