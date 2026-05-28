# disk_image_patch_030

Purpose:

Remember Tiny MC list position per directory.

Code change:

- Added a small in-memory directory state cache.
- Before leaving a directory, Tiny MC saves:
  - current directory path;
  - selected entry index;
  - selected entry name;
  - scroll offset.
- When scanning a directory, Tiny MC restores the saved selection and scroll
  for that directory.
- Selection is restored by filename first, with the old index as fallback if the
  file no longer exists or the directory contents changed.

Files:

```text
cubegm/rkgame
```

Apply:

Copy this patch directory over the SD card/image root.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: dir state restore, icube heartbeat, tiny_mc.log, cube_ioctl
Size: 40768 bytes
SHA256: D6F2FAD29C234F200D7BE73825935BE49F29085ECE96F1B3A0EF7B70A6334949
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_030\cubegm\rkgame: found no threats
```
