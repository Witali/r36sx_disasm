# Build log: libretro PRBoom

Date: 2026-05-27

Workspace:

```text
C:\Work\r36sx_disasm
```

Goal:

Build a `libretro-prboom` core for the MIPS32 little-endian `cubegm` launcher
environment and prepare an installable overlay without bundling a commercial
Doom IWAD.

## Source

```text
Repository: https://github.com/libretro/libretro-prboom.git
Commit: 648223372e24773821a2041e0c39728f723badf4
Local path: internet_sources\libretro-prboom
```

Source scan:

```text
.\tools\scan-download.ps1 .\internet_sources\libretro-prboom
Result: found no threats
```

## Build attempts

`make` was not available locally, so the build uses
`homebrew\libretro_prboom\build_prboom.ps1` to read upstream
`Makefile.common`, collect sources, and invoke Zig directly.

Initial compile findings:

- `mipsel-linux-gnu` compile with explicit sysroot include path succeeds.
- `mipsel-linux-gnu` link fails because Zig tries to provide libc for an
  unsupported GNU target.
- `mipsel-linux-gnueabihf` link succeeds with the local glibc sysroot.
- `zig objcopy --strip-*` is unusable here (`--strip-unneeded` unrecognized,
  `--strip-all` unimplemented), so stripping is done at link time with
  `-Wl,-s`.
- A full `mipsel-linux-gnueabihf` compile/link produced a `GLIBC_2.33`
  requirement, which is too new for the device rootfs. The final build uses
  `mipsel-linux-gnu` for compile and `mipsel-linux-gnueabihf` for link.

Final command:

```powershell
.\homebrew\libretro_prboom\build_prboom.ps1
```

## Final binary

```text
Path: homebrew\libretro_prboom\prboom_libretro.so
Format: ELF32 little-endian DYN(shared object)
Machine: MIPS
Flags: 0x70001007
Size: 1,630,056 bytes
SHA256: 2829973D0AC96F7B23F816BDDB5A085E594DE4652333840CAA70613891B68FD9
NEEDED: libm.so.6, libc.so.6
GLIBC strings: GLIBC_2.0, GLIBC_2.2, GLIBC_2.3, GLIBC_2.7, GLIBC_2.15
```

Rootfs compatibility check:

```text
disk_image\rootfs\lib\libc.so.6 exposes GLIBC versions through GLIBC_2.19.
disk_image\rootfs\lib\libm.so.6 exposes GLIBC_2.0, GLIBC_2.2, GLIBC_2.4,
GLIBC_2.15, and GLIBC_2.18.
```

Final binary scan:

```text
.\tools\scan-download.ps1 .\homebrew\libretro_prboom\prboom_libretro.so
Result: found no threats
```

## Runtime support file

```text
Path: homebrew\libretro_prboom\prboom.wad
Source: internet_sources\libretro-prboom\prboom.wad
Size: 143,312 bytes
SHA256: B4DD3642932193CC42BCA0EE98BF30004888CA4850D69E85023B8BAACFBA1D1D
```

`prboom.wad` is a PRBoom support WAD, not a Doom IWAD.

## Patch overlay

```text
Patch directory: disk_image_patch_018
```

Files:

```text
cubegm\cores\libemu_prboom.so
cubegm\cores\system\prboom\prboom.wad
cubegm\cores\config.xml
cubegm\cores\filelist.xml
GB\filelist.csv
MANIFEST.md
```

Overlay scan:

```text
.\tools\scan-download.ps1 .\disk_image_patch_018
Result: found no threats
```

Known limitation:

The core has not yet been tested on the physical device with a legal Doom IWAD.
The overlay does not include `doom1.wad`, `doom.wad`, `doom2.wad`, or Freedoom
IWAD files.

## 2026-05-27 patch CSV correction

Reason:

The first `disk_image_patch_018` overlay added `cubegm\cores\filelist.xml`
overrides for Doom test names, but did not include `GB\filelist.csv`. That made
the patch incomplete for the existing Game Boy menu test route.

Added:

```text
disk_image_patch_018\GB\filelist.csv
```

Rows added at the top of the copied GB list:

```text
doom1.wad,Doom Shareware,Doom Shareware
freedoom1.wad,Freedoom Phase 1,Freedoom Phase 1
doom.m3u,Doom Playlist,Doom Playlist
```
