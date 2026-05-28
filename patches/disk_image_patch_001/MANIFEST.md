# disk_image_patch_001

Date: 2026-05-23

Purpose: add a minimal `Button Demo` libretro core to the existing `disk_image` tree without replacing stock emulator cores.

## Files

### Added

- `cubegm\cores\libemu_buttondemo.so`
  - Built MIPS32 little-endian ELF shared object.
  - Source: `homebrew\libretro_button_demo\button_demo.c`
  - SHA256: `7E11D6F6566A244DEF38F93EB6F620DED7CE7F239A0AE9E4C2F885BDDC51430C`

- `GB\button.demo`
  - Dummy launch file used to make `rkgame` load the demo core through an existing platform folder.
  - SHA256: `87F14C3E75507D980DE3458DBE6C89561E29ACAD02AD2CD1CE10350704364C0F`

### Modified

- `cubegm\cores\config.xml`
  - Added:

```xml
<core>
<emucore name="Button Demo" file="libemu_buttondemo.so" />
<supported_extensions>DEMO</supported_extensions>
</core>
```

  - SHA256: `D5B6E76F6CB28E0A7578C310FDBCCFC956E1F77F0AE65B37D9BED69C91B6883C`

- `cubegm\cores\filelist.xml`
  - Added:

```xml
<file name="GB/button.demo" core="libemu_buttondemo.so" />
```

  - SHA256: `11609CA873830658EDC4FDAAC767F1FE4C0363402978CE0CEDFE9366487D2F3A`

## Install

Copy the contents of this patch folder over the root of the SD/disk image filesystem.

Expected target paths:

```text
disk_image\cubegm\cores\libemu_buttondemo.so
disk_image\cubegm\cores\config.xml
disk_image\cubegm\cores\filelist.xml
disk_image\GB\button.demo
```

## Notes

This patch uses the existing `GB` platform folder because `rkgame` may not show arbitrary new root platform folders. The `filelist.xml` override points only `GB/button.demo` at `libemu_buttondemo.so`, so stock GB/GBC emulator cores are not replaced.
