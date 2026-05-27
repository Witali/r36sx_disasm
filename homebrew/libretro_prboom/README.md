# Libretro PRBoom for R36SX research image

This directory contains a first MIPS32 little-endian build of
`libretro-prboom` for the `cubegm` launcher environment.

Upstream source:

```text
https://github.com/libretro/libretro-prboom.git
commit 648223372e24773821a2041e0c39728f723badf4
```

Built output:

```text
homebrew/libretro_prboom/prboom_libretro.so
```

Patch overlay output:

```text
disk_image_patch_018/cubegm/cores/libemu_prboom.so
disk_image_patch_018/cubegm/cores/system/prboom/prboom.wad
disk_image_patch_018/GB/freedoom1.wad
```

## Runtime files

`prboom.wad` is the PRBoom support WAD from upstream. It is not the Doom game
IWAD. A legal IWAD still needs to be supplied separately, for example
`doom1.wad`, `doom.wad`, `doom2.wad`, or a Freedoom IWAD.

The libretro core searches runtime WAD files in this order:

- the directory containing the loaded WAD;
- the frontend system directory under `prboom`;
- the frontend system directory itself;
- parent directories of the loaded WAD directory, when recursive search is on.

For the current launcher experiments, the safest route is still the existing
Game Boy menu path with a core override in `cubegm/cores/filelist.xml`.
`disk_image_patch_018` adds overrides for:

```xml
<file name="GB/doom1.wad" core="libemu_prboom.so" />
<file name="GB/freedoom1.wad" core="libemu_prboom.so" />
<file name="GB/doom.m3u" core="libemu_prboom.so" />
```

`disk_image_patch_018` also includes `GB/filelist.csv` rows for those test
entries and bundles the free `freedoom1.wad` from Freedoom 0.13.0. The bundled
entry is:

```text
freedoom1.wad,Freedoom Phase 1,Freedoom Phase 1
```

For commercial Doom, copy a legal IWAD into `GB/` using one of the other names,
for example `doom1.wad`.

## Build

Run from repository root:

```powershell
.\homebrew\libretro_prboom\build_prboom.ps1
```

The build intentionally compiles C sources as `mipsel-linux-gnu` but links the
shared object as `mipsel-linux-gnueabihf`. A full `gnueabihf` compile stamped a
`GLIBC_2.33` requirement into the output, while the device rootfs only exposes
GLIBC versions through `GLIBC_2.19`. The hybrid route keeps the output to
`GLIBC_2.0`, `GLIBC_2.2`, `GLIBC_2.3`, `GLIBC_2.7`, and `GLIBC_2.15`.

## Current status

The core builds and links as an ELF32 MIPS shared object. `disk_image_patch_018`
now includes a free IWAD for first device testing, but the core has not yet been
physical-device tested in this workspace.
