# disk_image_patch_027

Purpose:

Rebuild Tiny MC, Button Demo, and Pong after moving shared R36SX/SF3000-like
hardware constants into `homebrew/common/hardware.h`.

Code change:

- Added `homebrew/common/hardware.h`.
- Moved common device facts into the shared header:
  - native RGB565 framebuffer size: 640x480;
  - firmware paths such as `driver.so`, `rkgame`, `/dev/fb0`;
  - `driver.so` `cube_ioctl` command IDs;
  - stock `rkgame` raw joy_key button masks;
  - `icube` SysV shared-memory heartbeat constants;
  - 60 Hz / 44.1 kHz timing used by small libretro demos.
- Updated Tiny MC, Button Demo, and Pong to include the shared header.

Files:

```text
cubegm/rkgame
cubegm/cores/libemu_buttondemo.so
cubegm/cores/libemu_pong.so
```

Apply:

Copy this patch directory over the SD card/image root.

Build commands:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1

$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Verification:

```text
cubegm/rkgame
Size: 38484 bytes
SHA256: 122FA794691B1BDA725E8F85411499C324C7127142AD9EF657B8E9E8463FEF7F
Dependency strings: /lib/ld.so.1, libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl

cubegm/cores/libemu_buttondemo.so
Size: 30220 bytes
SHA256: 748A21F27385AF0A22DD6CB27869AD729660F8BA415C0860328EF7C1115977BE
No NEEDED/UND/__ubsan/GLIBC/ld-linux/libc.so/libm.so/memset strings found.

cubegm/cores/libemu_pong.so
Size: 41512 bytes
SHA256: A769CA8D5710D4863D0348E7ECC8F41C9CD28CC3F14505F4E934014AFC782EA4
No NEEDED/UND/__ubsan/GLIBC/ld-linux/libc.so/libm.so/memset strings found.

Defender scans for all homebrew outputs and patch_027 files: found no threats.
```
