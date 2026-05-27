# disk_image_patch_018: Libretro PRBoom core

Date: 2026-05-27

Purpose:

Add a first `libretro-prboom` MIPS32 little-endian core build for the `cubegm`
launcher. This patch provides the core and PRBoom support WAD only; it does not
include a Doom game IWAD.

## Files

Copy these files over the SD-card/image root:

```text
cubegm\cores\libemu_prboom.so
cubegm\cores\system\prboom\prboom.wad
cubegm\cores\config.xml
cubegm\cores\filelist.xml
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

The actual WAD/M3U files are not included. To test through the existing Game
Boy menu route, copy a legal IWAD into `GB\` and add a row to `GB\filelist.csv`,
for example:

```text
doom1.wad,Doom Shareware,Doom Shareware
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
Defender scan of disk_image_patch_018: found no threats.
```

Known limitation:

The core is built and integrated as an overlay, but it has not yet been
physical-device tested with a legal Doom IWAD.
