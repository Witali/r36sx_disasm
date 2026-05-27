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

Known limitation at initial overlay creation:

The core had not yet been tested on the physical device, and the first overlay
did not include any game IWAD. A free `freedoom1.wad` was added in the later
entry below.

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

## 2026-05-27 Freedoom IWAD addition

Reason:

PRBoom cannot run a game with only `prboom.wad`; it also needs an IWAD. To make
the test patch usable without commercial Doom data, `freedoom1.wad` was
downloaded from the official Freedoom 0.13.0 GitHub release and added to the
Game Boy test route.

Source:

```text
URL: https://github.com/freedoom/freedoom/releases/download/v0.13.0/freedoom-0.13.0.zip
Release page: https://github.com/freedoom/freedoom/releases/tag/v0.13.0
```

Download command:

```powershell
Invoke-WebRequest -Uri https://github.com/freedoom/freedoom/releases/download/v0.13.0/freedoom-0.13.0.zip -OutFile .\internet_sources\freedoom\freedoom-0.13.0.zip
```

Downloaded archive:

```text
Path: internet_sources\freedoom\freedoom-0.13.0.zip
Size: 24,143,781 bytes
SHA256: 3F9B264F3E3CE503B4FB7F6BDCB1F419D93C7B546F4DF3E874DD878DB9688F59
Defender scan: found no threats
```

Extracted and copied into patch:

```text
disk_image_patch_018\GB\freedoom1.wad
disk_image_patch_018\GB\freedoom-0.13.0-COPYING.txt
disk_image_patch_018\GB\freedoom-0.13.0-CREDITS.txt
disk_image_patch_018\GB\freedoom-0.13.0-CREDITS-MUSIC.txt
```

IWAD verification:

```text
freedoom1.wad size: 28,795,076 bytes
freedoom1.wad SHA256: 7323BCC168C5A45FF10749B339960E98314740A734C30D4B9F3337001F9E703D
Defender scan: found no threats
```

## 2026-05-27 R36SX export compatibility rebuild

Reason:

The physical device test of the first PRBoom overlay showed a black screen and
returned to the launcher. The generated PRBoom core exported only `retro_*`
symbols because upstream `libretro/link.T` hides every other symbol. Working
local cores export three extra vendor-facing hooks:

```text
check_encrypty
CheckEncrypty
SetFrameSkip
```

The earlier Ghidra analysis of stock cores showed that `check_encrypty()` must
return non-zero for the launcher path to continue.

Changes:

```text
homebrew\libretro_prboom\r36sx_compat.c
homebrew\libretro_prboom\r36sx_link.T
homebrew\libretro_prboom\build_prboom.ps1
```

`r36sx_compat.c` implements `check_encrypty()` and `CheckEncrypty(...)` with a
return value of `1`, matching Button Demo and Pong. `r36sx_link.T` exports
those symbols plus `SetFrameSkip`.

Build command:

```powershell
.\homebrew\libretro_prboom\build_prboom.ps1
```

Verification:

```text
Path: homebrew\libretro_prboom\prboom_libretro.so
Size: 1,630,504 bytes
SHA256: 408280389E3853468A10A227E5FBA282A7BC41BA04A9DD410B2A7E9FF5B6757F
Required export strings: check_encrypty, CheckEncrypty, SetFrameSkip
GLIBC strings: GLIBC_2.0, GLIBC_2.2, GLIBC_2.3, GLIBC_2.7, GLIBC_2.15
Defender scan: found no threats
```

Patch directory:

```text
disk_image_patch_019
```

## 2026-05-27 WAD adjacency and Doom shareware test patch

Device result:

```text
Doom Shareware: did not start.
Freedoom Phase 1: showed Loading..., then black screen, then returned to launcher.
Doom Playlist: did not start.
```

Interpretation:

`Doom Shareware` and `Doom Playlist` were menu/config entries only; the files
`GB\doom1.wad` and `GB\doom.m3u` were not present in patch 019. `Freedoom
Phase 1` did start loading, so the PRBoom core is being selected, but it still
fails during early engine/content initialization.

Changes in `disk_image_patch_020`:

```text
GB\doom1.wad
GB\doom.m3u
GB\prboom.wad
GB\doom-1.8.wad.txt
```

`GB\prboom.wad` duplicates the support WAD next to the loaded content, avoiding
reliance on `RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY`.

Official idgames downloads:

```powershell
Invoke-WebRequest -Uri https://www.gamers.org/pub/idgames/idstuff/doom/doom19s.zip -OutFile .\internet_sources\doom_shareware\doom19s.zip
Invoke-WebRequest -Uri https://www.gamers.org/pub/idgames/idstuff/doom/doom-1.8.wad.gz -OutFile .\internet_sources\doom_shareware\doom-1.8.wad.gz
Invoke-WebRequest -Uri https://www.gamers.org/pub/idgames/idstuff/doom/doom-1.8.wad.txt -OutFile .\internet_sources\doom_shareware\doom-1.8.wad.txt
```

The older `doom19s.zip` package was scanned and extracted only far enough to
inspect its contents. It contains a DOS `DEICE.EXE` installer plus data slices,
so it was not used for the patch. The patch instead uses the directly
available gzip-compressed IWAD.

Download verification:

```text
doom19s.zip size: 2,450,688 bytes
doom19s.zip SHA256: CACF0142B31CA1AF00796B4A0339E07992AC5F21BC3F81E7532FE1B5E1B486E6
doom19s.zip Defender scan: found no threats
DEICE.EXE Defender scan: found no threats

doom-1.8.wad.gz size: 1,756,050 bytes
doom-1.8.wad.gz SHA256: 58CF6A563B631A47561630AFA6276D853C27079342F7969EC3006965DA8557A1
doom-1.8.wad.gz Defender scan: found no threats
```

Patch IWAD verification:

```text
doom1.wad size: 4,196,020 bytes
doom1.wad SHA256: BB449C7480E9A02A62012D041406E8E43DAA51CAA0650646D1307D8650B8F837
doom1.wad header: IWAD
doom1.wad Defender scan: found no threats
prboom.wad SHA256: B4DD3642932193CC42BCA0EE98BF30004888CA4850D69E85023B8BAACFBA1D1D
doom.m3u contents: doom1.wad
```

Patch directory:

```text
disk_image_patch_020
```
