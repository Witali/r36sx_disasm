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

## 2026-05-28 rkgame-style input rebuild

Device test result:

Tiny MC rendered correctly through the `driver.so` display path, but built-in
buttons did not respond.

Reverse engineering result:

`rkgame` reads built-in controls through `driver.so` `cube_ioctl` and the
`/tmp/joy_key` shared memory area:

```text
cube_ioctl(0x40050209, &KEY_VALUE_Addr)
KEY_VALUE_Addr[0] -> joy_key
KEY_VALUE_Addr[1] -> second joy word
cube_ioctl(0x40050208, &Cube_State_Reg)
```

The `joykey_explaned()` mapping in `rkgame` is:

```text
SELECT 0x00000001
START  0x00000008
UP     0x00000010
RIGHT  0x00000020
DOWN   0x00000040
LEFT   0x00000080
X      0x00001000
A      0x00002000
B      0x00004000
Y      0x00008000
FN     0x00010000
```

Code change:

- Tiny MC now `dlsym()`s `cube_ioctl` from `driver.so`.
- It obtains the `/tmp/joy_key` shared memory pointer with command
  `0x40050209`.
- It polls raw words and the game/status register every frame.
- It translates `rkgame` masks to Tiny MC navigation bits.
- It keeps `/dev/input/js*` and evdev as fallback input paths.
- `tiny_mc.log` records raw input changes as:
  `cube input raw0=... raw1=... state=... buttons=...`.

Patch directory:

```text
disk_image_patch_024
```

Patch files:

```text
disk_image_patch_024\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: cube_ioctl, cube input raw, joy key shm, tiny_mc.log
Size: 37040 bytes
SHA256: 91F96858C58AA6D2E4DFBA7539A0C01061DE39F195174C9EB80AA6541E9D1037
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_024\cubegm\rkgame: found no threats
```

## 2026-05-28 DEBUG-gated logging rebuild

Purpose:

Make Tiny MC debug logging conditional on a compile-time flag at the beginning
of the program.

Code change:

- Added `#define DEBUG 1` near the top of `tiny_mc.c`.
- Wrapped the `tiny_mc.log` file descriptor/path and logging implementation in
  `#if DEBUG`.
- Added no-op `log_open()`, `log_close()`, and `log_msg()` implementations for
  `DEBUG=0`, so setting `#define DEBUG 0` and rebuilding disables log file
  creation and log writes.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_025
```

Patch files:

```text
disk_image_patch_025\cubegm\rkgame
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

## 2026-05-28 icube heartbeat rebuild

Device test result:

Tiny MC rendered correctly and buttons worked, but about every 5 seconds it
returned to its initial state.

Reverse engineering result:

Ghidra analysis of `cubegm/icube` showed that this is not a kernel watchdog.
`icube` is a small userspace supervisor:

- It calls `ShareMemCreat()`, using `shmget(0x4d2, 0x1c4, IPC_CREAT | 0666)`.
- It starts `/mnt/sdcard/cubegm/rkgame` with `fork()` + `execl()`.
- After an initial `sleep(6)`, it checks `shm[1]` once per second.
- If `shm[1]` is unchanged or zero, it runs `system("killall rkgame")` and
  starts `rkgame` again.

Stock `rkgame` has `ShareMemCreat()` and `XintiaoThread`; the thread runs every
20 ms and updates:

```text
shm[0] = 1
shm[1]++
```

Analysis command:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects icube_supervisor -import .\disk_image\cubegm\icube -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\icube -deleteProject
```

Code change:

- Tiny MC now attaches the same SysV shared memory segment.
- It updates the `icube` heartbeat from the main loop.
- It also updates the heartbeat while waiting for a launched child process with
  `waitpid(..., WNOHANG)`, so a long-running child does not make `icube`
  restart Tiny MC.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_026
```

Patch files:

```text
disk_image_patch_026\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl
Size: 38676 bytes
SHA256: AD81FE4356DE2B49FF505256FB233614557AD1EA518447C84821BCC005925BC0
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_026\cubegm\rkgame: found no threats
```

## 2026-05-28 shared hardware header rebuild

Purpose:

Move device-specific constants into `homebrew/common/hardware.h` so Tiny MC and
other homebrew programs use one shared source of truth.

Code change:

- Tiny MC now includes `../common/hardware.h`.
- Moved 640x480 RGB565 geometry, `driver.so` paths, `cube_ioctl` command IDs,
  raw `rkgame` joy_key masks, and `icube` heartbeat constants out of
  `tiny_mc.c`.
- Runtime behavior is intended to remain the same as `disk_image_patch_026`.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_027
```

Patch file:

```text
disk_image_patch_027\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl
Size: 38484 bytes
SHA256: 122FA794691B1BDA725E8F85411499C324C7127142AD9EF657B8E9E8463FEF7F
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_027\cubegm\rkgame: found no threats
```

## 2026-05-28 scrollbar rebuild

Purpose:

Show a visual scrollbar in Tiny MC when the current directory has more entries
than fit on screen.

Code change:

- Added list and scrollbar UI constants.
- Added `draw_scrollbar()`.
- Scrollbar is rendered only when `g_entry_count > visible_rows()`.
- Reduced list text and selection highlight width while the scrollbar is shown.
- Clamped `g_scroll` to the maximum valid scroll offset in
  `ensure_selection_visible()`.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_029
```

Patch file:

```text
disk_image_patch_029\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl
Size: 39636 bytes
SHA256: 3B50D99E3F450205D83015FF5D99D6F30D04207E8E641DF8D112940B81FE9150
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_029\cubegm\rkgame: found no threats
```

## 2026-05-28 per-directory list state rebuild

Purpose:

Remember Tiny MC selection and scroll offset when moving between directories.

Code change:

- Added a small in-memory directory state cache.
- The cache stores directory path, selected entry index, selected entry name,
  and scroll offset.
- Before leaving a directory, Tiny MC saves its current list state.
- After scanning a directory, Tiny MC restores the previous state for that path.
- Selection is restored by filename first, with the saved index as fallback.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
disk_image_patch_030
```

Patch file:

```text
disk_image_patch_030\cubegm\rkgame
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

## 2026-05-28 MIPS_NATIVE integration

Purpose:

Move Tiny MC out of `cubegm/rkgame` and into the root-level native application
area, then make the stock game transition script enter Tiny MC directly.

File changes:

- Copied `homebrew\tiny_mc\tiny_mc` to
  `disk_image\MIPS_NATIVE\tiny_mc\tiny_mc`.
- Restored `disk_image\cubegm\rkgame` from `disk_image\cubegm\rkgame.stock`.
- Changed `disk_image\cubegm\icube_start.sh` to run:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

- Added `disk_image\MIPS_NATIVE\tiny_mc\README.txt`.
- Created overlay directory `disk_image_patch_031` with the same changed files.

Commands from repository root:

```powershell
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame.stock -Destination disk_image\cubegm\rkgame -Force
New-Item -ItemType Directory -Force -Path disk_image_patch_031\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_031\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_031\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame -Destination disk_image_patch_031\cubegm\rkgame -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_031\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_031\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Tiny MC size: 40768 bytes
Tiny MC SHA256: D6F2FAD29C234F200D7BE73825935BE49F29085ECE96F1B3A0EF7B70A6334949

Restored stock rkgame ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Restored stock rkgame size: 1178732 bytes
Restored stock rkgame SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_031\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_031\cubegm\rkgame: found no threats
```
