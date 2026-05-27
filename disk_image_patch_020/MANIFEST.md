# disk_image_patch_020: PRBoom WAD adjacency and Doom shareware test

Date: 2026-05-27

Purpose:

Patch 019 still returned to the launcher after `Freedoom Phase 1` started
loading. The two other menu rows, `Doom Shareware` and `Doom Playlist`, did not
start because `GB\doom1.wad` and `GB\doom.m3u` were not present.

This patch keeps the R36SX-compatible PRBoom core from patch 019, adds a small
Doom shareware IWAD, creates the missing playlist, and places `prboom.wad`
directly next to the content WADs in `GB\`. This avoids depending on
`RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY`, which may not be implemented by
`rkgame` the same way as RetroArch.

## Files

Copy these files over the SD-card/image root:

```text
cubegm\cores\libemu_prboom.so
cubegm\cores\system\prboom\prboom.wad
cubegm\cores\config.xml
cubegm\cores\filelist.xml
GB\filelist.csv
GB\freedoom1.wad
GB\doom1.wad
GB\doom.m3u
GB\prboom.wad
GB\doom-1.8.wad.txt
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

The patch includes `freedoom1.wad`, `doom1.wad`, and `doom.m3u`. It also
includes `GB\filelist.csv` rows for these test entries:

```text
doom1.wad,Doom Shareware,Doom Shareware
freedoom1.wad,Freedoom Phase 1,Freedoom Phase 1
doom.m3u,Doom Playlist,Doom Playlist
```

Recommended device test order:

```text
Doom Shareware
Doom Playlist
Freedoom Phase 1
```

`Doom Shareware` is much smaller than Freedoom, so it is a better first check
if the prior crash was memory pressure during WAD loading.

## Bundled free IWAD

```text
Source: https://github.com/freedoom/freedoom/releases/tag/v0.13.0
Archive: freedoom-0.13.0.zip
Archive SHA256: 3F9B264F3E3CE503B4FB7F6BDCB1F419D93C7B546F4DF3E874DD878DB9688F59
File: GB\freedoom1.wad
Size: 28,795,076 bytes
SHA256: 7323BCC168C5A45FF10749B339960E98314740A734C30D4B9F3337001F9E703D
```

## Bundled Doom shareware IWAD

```text
Source: https://www.gamers.org/pub/idgames/idstuff/doom/doom-1.8.wad.gz
Sidecar text: https://www.gamers.org/pub/idgames/idstuff/doom/doom-1.8.wad.txt
Downloaded gzip SHA256: 58CF6A563B631A47561630AFA6276D853C27079342F7969EC3006965DA8557A1
File: GB\doom1.wad
Size: 4,196,020 bytes
SHA256: BB449C7480E9A02A62012D041406E8E43DAA51CAA0650646D1307D8650B8F837
Header: IWAD
```

`GB\doom.m3u` contains:

```text
doom1.wad
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
libemu_prboom.so size: 1,630,504 bytes
libemu_prboom.so SHA256: 408280389E3853468A10A227E5FBA282A7BC41BA04A9DD410B2A7E9FF5B6757F
prboom.wad size: 143,312 bytes
prboom.wad SHA256: B4DD3642932193CC42BCA0EE98BF30004888CA4850D69E85023B8BAACFBA1D1D
ELF32 little-endian DYN(shared object), machine=MIPS, flags=0x70001007
NEEDED: libm.so.6, libc.so.6
GLIBC strings: GLIBC_2.0, GLIBC_2.2, GLIBC_2.3, GLIBC_2.7, GLIBC_2.15
Required R36SX export strings now include check_encrypty, CheckEncrypty,
and SetFrameSkip.
Defender scan of homebrew SO: found no threats.
Defender scan of downloaded Freedoom ZIP: found no threats.
Defender scan of Freedoom IWAD: found no threats.
Defender scan of downloaded Doom shareware gzip: found no threats.
Defender scan of extracted Doom shareware IWAD: found no threats.
Defender scan of disk_image_patch_020: found no threats.
```

Known limitation:

The core is built and integrated as an overlay, but it has not yet been
physical-device tested.
