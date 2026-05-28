# disk_image_patch_025

Purpose:

Enable compile-time control of Tiny MC logging. If `#define DEBUG 1` is present
near the top of `homebrew/tiny_mc/tiny_mc.c`, Tiny MC writes `tiny_mc.log`.
Changing it to `#define DEBUG 0` and rebuilding disables log file creation and
log writes.

Files:

```text
cubegm/rkgame
```

Apply:

Copy `cubegm/rkgame` from this patch over the SD card/image `cubegm/rkgame`.
Keep the stock launcher backup as `cubegm/rkgame.stock`.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Copy/update commands:

```powershell
New-Item -ItemType Directory -Force -Path .\disk_image_patch_025\cubegm
Copy-Item -Force -Path .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image\cubegm\rkgame
Copy-Item -Force -Path .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image_patch_025\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings in DEBUG=1 build: tiny_mc.log, cube_ioctl
Size: 37040 bytes
SHA256: 91F96858C58AA6D2E4DFBA7539A0C01061DE39F195174C9EB80AA6541E9D1037
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_025\cubegm\rkgame: found no threats
```
