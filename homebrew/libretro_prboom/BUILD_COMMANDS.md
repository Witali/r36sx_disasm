# Build commands: libretro PRBoom

Date: 2026-05-27

## Fetch and scan upstream source

```powershell
git clone https://github.com/libretro/libretro-prboom.git .\internet_sources\libretro-prboom
.\tools\scan-download.ps1 .\internet_sources\libretro-prboom
git -C .\internet_sources\libretro-prboom rev-parse HEAD
```

Expected commit:

```text
648223372e24773821a2041e0c39728f723badf4
```

## Build

Run from repository root:

```powershell
.\homebrew\libretro_prboom\build_prboom.ps1
```

The script:

- reads `internet_sources/libretro-prboom/Makefile.common`;
- excludes the optional FluidSynth block;
- compiles 101 C files into `homebrew/libretro_prboom/obj`;
- compiles `homebrew/libretro_prboom/r36sx_compat.c` for the vendor launcher
  hooks;
- links `homebrew/libretro_prboom/prboom_libretro.so`;
- prints size and SHA256.

Important compiler flags:

```text
-target mipsel-linux-gnu
--sysroot tools/mipsel-buildroot-linux-gnu_sdk-buildroot/mipsel-buildroot-linux-gnu/sysroot
-isystem <sysroot>/usr/include
-march=mips32r2
-O2
-DNDEBUG
-DFPM_DEFAULT
-DINLINE=inline
-DHAVE_LIBMAD
-DMUSIC_SUPPORT
-D_POSIX_C_SOURCE=199309L
-std=c99
-ffast-math
-fomit-frame-pointer
-fPIC
-include z_zone.h
```

Important linker flags:

```text
-target mipsel-linux-gnueabihf
--sysroot <sysroot>
-nostdlib
-march=mips32r2
-shared
-fPIC
-Wl,--version-script=homebrew/libretro_prboom/r36sx_link.T
-Wl,--no-undefined
-Wl,--as-needed
-Wl,-s
-L/lib
-L/usr/lib
-lm
-lc
-lgcc_s
```

## Verify

```powershell
Get-FileHash .\homebrew\libretro_prboom\prboom_libretro.so -Algorithm SHA256
.\tools\scan-download.ps1 .\homebrew\libretro_prboom\prboom_libretro.so
rg -a -o "GLIBC_[0-9.]+|libc\.so\.6|libm\.so\.6|ld\.so\.1|retro_[A-Za-z0-9_]+" .\homebrew\libretro_prboom\prboom_libretro.so | Sort-Object -Unique
```

The dynamic section was parsed with a small PowerShell ELF reader; final
`NEEDED` libraries are:

```text
libm.so.6, libc.so.6
```

Required R36SX export strings:

```text
check_encrypty
CheckEncrypty
SetFrameSkip
```
