# disk_image_patch_018: Libretro PRBoom core

Date: 2026-05-27

Purpose:

Add a first `libretro-prboom` MIPS32 little-endian core build for the `cubegm`
launcher. This patch provides the core, PRBoom support WAD, and the free
Freedoom Phase 1 IWAD for an out-of-the-box test route. It does not include any
commercial Doom IWAD.

## Files

Copy these files over the SD-card/image root:

```text
cubegm\cores\libemu_prboom.so
cubegm\cores\system\prboom\prboom.wad
cubegm\cores\config.xml
cubegm\cores\filelist.xml
GB\filelist.csv
GB\freedoom1.wad
GB\freedoom-0.13.0-COPYING.txt
GB\freedoom-0.13.0-CREDITS.txt
GB\freedoom-0.13.0-CREDITS-MUSIC.txt
```

## Core config

Added to `cubegm\cores\config.xml`:

```xml
<core>
<emucore name="PrBoom" file="libemu_prboom.so" />
<supported_extensions>WAD</supported_extensions>
<supported_extensions>IWAD</supported_extensions>
<supported_extensions>PWAD</supported_extensions>
<supported_extensions>LMP</supported_extensions>
<supported_extensions>M3U</supported_extensions>
</core>
```

Added to `cubegm\cores\filelist.xml` as optional Game Boy menu test routes:

```xml
<file name="GB/doom1.wad" core="libemu_prboom.so" />
<file name="GB/freedoom1.wad" core="libemu_prboom.so" />
<file name="GB/doom.m3u" core="libemu_prboom.so" />
```

The patch includes `freedoom1.wad`, but does not include `doom1.wad` or
`doom.m3u`. It also includes `GB\filelist.csv` rows for these test entries:

```text
doom1.wad,Doom Shareware,Doom Shareware
freedoom1.wad,Freedoom Phase 1,Freedoom Phase 1
doom.m3u,Doom Playlist,Doom Playlist
```

This patch includes `GB\freedoom1.wad`, so `Freedoom Phase 1` should be the
first device test entry. For commercial Doom, copy a legal IWAD into `GB\` with
one of the other names, or create `GB\doom.m3u` that points to the IWAD.

## Bundled free IWAD

```text
Source: https://github.com/freedoom/freedoom/releases/tag/v0.13.0
Archive: freedoom-0.13.0.zip
Archive SHA256: 3F9B264F3E3CE503B4FB7F6BDCB1F419D93C7B546F4DF3E874DD878DB9688F59
File: GB\freedoom1.wad
Size: 28,795,076 bytes
SHA256: 7323BCC168C5A45FF10749B339960E98314740A734C30D4B9F3337001F9E703D
```

## Build

Command from repository root:

```powershell
.\homebrew\libretro_prboom\build_prboom.ps1
```

Source:

```text
https://github.com/libretro/libretro-prboom.git
commit 648223372e24773821a2041e0c39728f723badf4
```

## Verification

```text
libemu_prboom.so size: 1,630,056 bytes
libemu_prboom.so SHA256: 2829973D0AC96F7B23F816BDDB5A085E594DE4652333840CAA70613891B68FD9
prboom.wad size: 143,312 bytes
prboom.wad SHA256: B4DD3642932193CC42BCA0EE98BF30004888CA4850D69E85023B8BAACFBA1D1D
ELF32 little-endian DYN(shared object), machine=MIPS, flags=0x70001007
NEEDED: libm.so.6, libc.so.6
GLIBC strings: GLIBC_2.0, GLIBC_2.2, GLIBC_2.3, GLIBC_2.7, GLIBC_2.15
Defender scan of homebrew SO: found no threats.
Defender scan of downloaded Freedoom ZIP: found no threats.
Defender scan of Freedoom IWAD: found no threats.
Defender scan of disk_image_patch_018: found no threats.
```

Known limitation:

The core is built and integrated as an overlay, but it has not yet been
physical-device tested.
