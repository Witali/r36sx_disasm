# Build log: Tiny MC

## 2026-05-28 initial one-panel launcher

Purpose:

Build a small MC-like file manager that can replace `cubegm/rkgame`, show a
single file list, move selection with the left stick/D-pad, and launch the
selected file with `A` or `Start`.

Design:

- Standalone MIPS ELF executable, not a libretro core.
- Draws directly to `/dev/fb0`.
- Reads `/dev/input/js0..js3` and `/dev/input/event0..event9`.
- Uses `fork()` + `execl()` to run the selected file.
- Does not depend on SDL, DirectFB, ncurses, or Midnight Commander.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

The final build script compiles with `zig clang` and links with `zig ld.lld`
against the local Buildroot SDK sysroot:

```text
tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot
```

A simpler `zig cc -target mipsel-linux-gnueabi` dynamic build was rejected for
device use because it produced a binary with `GLIBC_2.34` references. The
stock rootfs libc only exposes symbols up to `GLIBC_2.19`, so the final build
uses the SDK `crt1.o`, `crti.o`, `crtn.o`, and `libc.so`.

The generated binary is:

```text
homebrew\tiny_mc\tiny_mc
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, GLIBC_2.0
Size: 28824 bytes
SHA256: 44189734CE2ABA6653DF8A22AA4E9CF7FA7CBA590779E03969EF0CDE4D0EB2C6
Defender scan: found no threats
```

Patch/install model:

```text
cubegm\rkgame.stock  <- original stock rkgame backup
cubegm\rkgame        <- Tiny MC replacement launcher
```

Patch directory:

```text
disk_image_patch_021
```

Patch files:

```text
disk_image_patch_021\cubegm\rkgame
disk_image_patch_021\cubegm\icubemp_start.sh
```

Patch file hashes:

```text
disk_image_patch_021\cubegm\rkgame
SHA256: 44189734CE2ABA6653DF8A22AA4E9CF7FA7CBA590779E03969EF0CDE4D0EB2C6
```

Defender scan results:

```text
homebrew\tiny_mc\tiny_mc: found no threats
disk_image_patch_021\cubegm\rkgame: found no threats
```

Known limitations:

- Input mapping is the first direct-input attempt outside `rkgame`; if the
  device exposes buttons differently, a raw input diagnostic build may be
  needed.
- It only launches executable files or shell scripts. It does not yet pass ROMs
  into the stock `rkgame` core loader.
- It waits for the launched program to exit, then returns to the file manager.

## 2026-05-28 driver.so display rebuild

Device test result:

The first Tiny MC build showed a white screen for a few seconds, then a black
screen with only the battery overlay. That suggested the system overlay stayed
alive, but our replacement launcher had not initialized the vendor display path
the way `rkgame` does.

Reverse engineering result:

- `rkgame` `InitDisplay()` loads `%s/driver.so` from `work_path`.
- It resolves `video_driver_setting`, `video_drivers_init`,
  `video_driver_disp_frame`, `video_driver_setmode`, `video_driver_get_size`,
  `api_cube_malloc`, `api_cube_free`, `check_exit_game`, `api_cube_send_msg`,
  and `cube_ioctl`.
- It calls `video_driver_setting()` with a five-int setup block equivalent to
  `{1, 1, 1, 0x356, 0x1e0}`, then starts `video_drivers_init()`.
- `driver.so` `fbdev_init()` opens `/dev/dis` and `/dev/fb0`, performs
  framebuffer blank/unblank ioctls, configures rotation/aspect handling, opens
  HCGE, and starts render/painter threads.

Code change:

- Tiny MC now tries `dlopen("/mnt/sdcard/cubegm/driver.so")` first.
- Frames are drawn into an internal RGB565 640x480 buffer and presented through
  `video_driver_disp_frame()`.
- Direct `/dev/fb0` rendering is kept only as fallback.
- Before launching a selected executable, Tiny MC deinitializes the display and
  closes input devices so the child program can acquire `driver.so` cleanly.
- The link step now includes `-ldl`.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_022
```

Patch files:

```text
disk_image_patch_022\cubegm\rkgame
disk_image_patch_022\cubegm\icubemp_start.sh
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Size: 32592 bytes
SHA256: A6B1755B19D5882C11D6203D73CA30B9DE64A8742C60168347C53E3623C37B41
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_022\cubegm\rkgame: found no threats
```

## 2026-05-28 debug log rebuild and menu cleanup

Purpose:

Add on-device debug logging to Tiny MC and remove known non-working duplicate
menu entries for `.demo` and `.pong` launch routes.

Code change:

- Added append-only log output with fallback paths:
  - `/mnt/sdcard/cubegm/tiny_mc.log`
  - `/mnt/sdcard/tiny_mc.log`
  - `tiny_mc.log`
- Logged startup, start directory selection, display driver load attempts,
  `video_drivers_init()` return value, fallback framebuffer setup, input device
  opens/closes, directory scans, button state changes, launched path, child wait,
  and child exit status.
- The render loop intentionally does not log every frame.

Menu cleanup:

- Removed `button.demo,Button Demo,Button Demo` from `GB\filelist.csv`.
- Removed `pong.pong,Pong Demo,Pong Demo` from `GB\filelist.csv`.
- Removed matching `GB/button.demo` and `GB/pong.pong` overrides from
  `cubegm\cores\filelist.xml`.
- Kept the working `button.gb` and `pong.gb` routes.

Extension decision:

- Fuse already uses the proper Spectrum tape extension through
  `ATARI\R36SX_ZX48.tap` and an explicit `libemu_fuse.so` override.
- Doom/PRBoom should use `.wad`/`.m3u`, as in `disk_image_patch_020`, but those
  files are not present in the current local `disk_image`, so this patch does
  not add new Doom menu rows.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_023
```

Patch files:

```text
disk_image_patch_023\cubegm\rkgame
disk_image_patch_023\cubegm\cores\filelist.xml
disk_image_patch_023\GB\filelist.csv
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains log path string: tiny_mc.log
Size: 36332 bytes
SHA256: BB7EC1F3D97A283DD8C1D80E078A20120A694D51E0A11F11C2E3536E8B076E9B
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_023\cubegm\rkgame: found no threats
Removed-menu check for button.demo/pong.pong in patch_023: no matches
```
