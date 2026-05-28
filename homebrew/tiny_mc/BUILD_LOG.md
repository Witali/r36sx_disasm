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

## 2026-05-28 icube-compatible MIPS_NATIVE startup

Purpose:

Restore the stock `icube` launch chain while still making Tiny MC open the
root-level native program directory.

`ghidra_exports/icube/decompiled_all.c` shows the relevant `icube` contract:

```c
ShareMemCreat();
fork();
execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0);
sleep(6);
/* then compare shm[1] once per second and run system("killall rkgame") on stall */
```

So a direct `icube_start.sh -> MIPS_NATIVE/tiny_mc/tiny_mc` jump bypasses the
supervisor. The compatible setup is:

```text
hcprojector
  -> cubegm/icube_start.sh
  -> cubegm/icube
  -> cubegm/rkgame        # Tiny MC compatibility entrypoint
  -> default directory /mnt/sdcard/MIPS_NATIVE
```

Code change:

- `choose_start_dir()` now prefers `R36SX_MIPS_NATIVE_DIR` when no valid
  command-line directory is supplied.
- `cubegm/icube_start.sh` again launches `/mnt/sdcard/cubegm/icube &`.

File changes:

- Copied the rebuilt Tiny MC to `disk_image\MIPS_NATIVE\tiny_mc\tiny_mc`.
- Copied the same rebuilt Tiny MC to `disk_image\cubegm\rkgame`.
- Created overlay directory `disk_image_patch_032` with the same changed files.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_032\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_032\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\cubegm\rkgame -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_032\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_032\cubegm\rkgame -Force
Copy-Item -LiteralPath disk_image\cubegm\icube_start.sh -Destination disk_image_patch_032\cubegm\icube_start.sh -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_032\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_032\cubegm\rkgame
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Contains string: /mnt/sdcard/MIPS_NATIVE
Size: 40940 bytes
SHA256: F7D7E26864F247B01EC1660AE1DF91F12E49E3A41F21F7ABDB6B473602ECA0E8

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_032\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_032\cubegm\rkgame: found no threats
```

## 2026-05-28 direct launch without icube heartbeat

Purpose:

Bypass `icube` completely and remove the Tiny MC dependency on the
`icube`/heartbeat watchdog contract.

Code change:

- Added `#define USE_ICUBE_HEARTBEAT 0`.
- Wrapped the heartbeat state and SysV shared-memory implementation in
  `#if USE_ICUBE_HEARTBEAT`.
- In the direct build, `heartbeat_open()`, `heartbeat_tick()`, and
  `heartbeat_close()` are no-op helpers, so the main loop can keep the same
  shape without calling `shmget()` or `shmat()`.
- `cubegm/icube_start.sh` now runs Tiny MC directly:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

File changes:

- Copied the direct Tiny MC build to `disk_image\MIPS_NATIVE\tiny_mc\tiny_mc`.
- Restored `disk_image\cubegm\rkgame` from `disk_image\cubegm\rkgame.stock`.
- Created overlay directory `disk_image_patch_033` with the same changed files.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_033\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_033\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame.stock -Destination disk_image\cubegm\rkgame -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_033\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame -Destination disk_image_patch_033\cubegm\rkgame -Force
Copy-Item -LiteralPath disk_image\cubegm\icube_start.sh -Destination disk_image_patch_033\cubegm\icube_start.sh -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_033\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_033\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Tiny MC contains string: /mnt/sdcard/MIPS_NATIVE
Tiny MC does not contain strings: shmget, heartbeat
Tiny MC size: 39572 bytes
Tiny MC SHA256: F8DECFD3C397D1783FC64E505C6091D6013CA8514C03AB11ACAA73FBB52BB30A

Restored stock rkgame size: 1178732 bytes
Restored stock rkgame SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_033\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_033\cubegm\rkgame: found no threats
```

## 2026-05-28 FN shortcut to icube

Purpose:

Let the direct-launch Tiny MC build jump back into the stock `icube` supervisor
without relying on the `icube` heartbeat contract.

Code change:

- Added `R36SX_ICUBE_PATH` to `homebrew/common/hardware.h`.
- Added `BTN_FN_BIT` and mapped `R36SX_RKGAME_KEY_FN` to it through the
  existing `driver.so` `cube_ioctl` input path.
- Added `launch_icube()`: saves Tiny MC state, draws a status update, closes
  display and input devices, sets the stock `LD_LIBRARY_PATH`, changes to
  `/mnt/sdcard/cubegm`, and runs:

```c
execl("/mnt/sdcard/cubegm/icube", "icube", (char *)NULL);
```

- If `execl()` fails, Tiny MC reopens display/input and shows the error.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_034\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_034\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_034\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\cubegm\icube_start.sh -Destination disk_image_patch_034\cubegm\icube_start.sh -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame.stock -Destination disk_image\cubegm\rkgame -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame -Destination disk_image_patch_034\cubegm\rkgame -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_034\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_034\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Tiny MC contains strings: /mnt/sdcard/MIPS_NATIVE, /mnt/sdcard/cubegm/icube, FN iCube
Tiny MC does not contain strings: shmget, heartbeat
Tiny MC size: 40512 bytes
Tiny MC SHA256: 3AE2ED9DD32E1053310F2E10612A474568E4E92092B8D4C86D283A3C490582B5

Restored stock rkgame size: 1178732 bytes
Restored stock rkgame SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_034\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_034\cubegm\rkgame: found no threats
```

## 2026-05-28 FN startup debounce rebuild

Purpose:

Prevent Tiny MC from immediately launching `icube` on startup if the Fn bit is
already set in the initial `/tmp/joy_key` state.

Observed device behavior:

After the `disk_image_patch_034` Fn shortcut, Tiny MC appeared not to start;
instead the stock `icube` path started immediately. The likely cause is that
`g_prev_buttons` starts at zero, so an already-raised Fn bit is interpreted as a
new press on the first frame.

Code change:

- Added `g_fn_shortcut_armed`.
- Fn shortcut is armed only after Tiny MC observes Fn released.
- A startup Fn-high state is logged and ignored until release.

Behavior:

```text
Fn high during first frame -> ignored
Fn remains high            -> ignored
Fn released                -> shortcut armed
Fn pressed again           -> launch /mnt/sdcard/cubegm/icube
```

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_036\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_036\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_036\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\MIPS_NATIVE\tiny_mc\README.txt -Destination disk_image_patch_036\MIPS_NATIVE\tiny_mc\README.txt -Force
Copy-Item -LiteralPath disk_image\cubegm\icube_start.sh -Destination disk_image_patch_036\cubegm\icube_start.sh -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame.stock -Destination disk_image\cubegm\rkgame -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame -Destination disk_image_patch_036\cubegm\rkgame -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_036\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_036\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Tiny MC contains strings: /mnt/sdcard/cubegm/icube, FN startup state ignored until release, FN shortcut armed after release
Tiny MC does not contain strings: shmget, heartbeat
Tiny MC size: 40776 bytes
Tiny MC SHA256: 7E930951F3E14B60C030123234E249E9DCC3865B0F78EF7482EA3D2554FDBFFB

Restored stock rkgame size: 1178732 bytes
Restored stock rkgame SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_036\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_036\cubegm\rkgame: found no threats
```

## 2026-05-28 disable Fn shortcut rebuild

Purpose:

Temporarily disable Tiny MC Fn handling completely because the device still
entered `icube` after the debounce build.

Code change:

- Added `#define ENABLE_FN_ICUBE_SHORTCUT 0`.
- Wrapped `BTN_FN_BIT`, Fn input translation, `launch_icube()`, Fn footer text,
  and Fn command handling in `#if ENABLE_FN_ICUBE_SHORTCUT`.
- The direct build no longer contains the `/mnt/sdcard/cubegm/icube` path or
  `FN` strings.

iCube structure check:

- `hcprojector` calls `/mnt/sdcard/cubegm/icube_start.sh &`.
- Original `icube_start.sh` starts `/mnt/sdcard/cubegm/icube &`.
- `icube v1.3` creates SysV shm key `0x4d2`, forks, and
  `execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0)`.
- It waits 6 seconds, then checks `shm[1]` once per second.
- If the counter is unchanged or zero, it runs `system("killall rkgame")` and
  starts `/mnt/sdcard/cubegm/rkgame` again.
- `driver.so` contains the return path `/mnt/sdcard/cubegm/icubemp_start.sh &`.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_037\MIPS_NATIVE\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_037\cubegm
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image_patch_037\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\MIPS_NATIVE\tiny_mc\README.txt -Destination disk_image_patch_037\MIPS_NATIVE\tiny_mc\README.txt -Force
Copy-Item -LiteralPath disk_image\cubegm\icube_start.sh -Destination disk_image_patch_037\cubegm\icube_start.sh -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame.stock -Destination disk_image\cubegm\rkgame -Force
Copy-Item -LiteralPath disk_image\cubegm\rkgame -Destination disk_image_patch_037\cubegm\rkgame -Force
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_037\MIPS_NATIVE\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_037\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: ELF32 little-endian executable, machine=MIPS, interpreter /lib/ld.so.1
Tiny MC does not contain strings: /mnt/sdcard/cubegm/icube, FN, shmget, heartbeat
Tiny MC size: 39572 bytes
Tiny MC SHA256: F8DECFD3C397D1783FC64E505C6091D6013CA8514C03AB11ACAA73FBB52BB30A

Restored stock rkgame size: 1178732 bytes
Restored stock rkgame SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_037\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_037\cubegm\rkgame: found no threats
```

## 2026-05-28 restore Fn iCube shortcut rebuild

Purpose:

Restore the Tiny MC Fn shortcut now that the device confirmed Tiny MC starts
correctly through the real `icube.sh` boot route.

Code change:

- Changed `#define ENABLE_FN_ICUBE_SHORTCUT 0` to `1`.
- Restored Fn input translation and the footer hint through the existing
  compile-time guarded code.
- The shortcut still arms only after Fn has been released once, so a raised Fn
  bit during startup is ignored.
- Fn executes `/mnt/sdcard/cubegm/icube` directly. It intentionally does not
  execute `icube.sh`, because `icube.sh` now launches Tiny MC and would loop
  back into Tiny MC.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
Copy-Item -Force homebrew\tiny_mc\tiny_mc disk_image\MIPS_NATIVE\tiny_mc\tiny_mc
New-Item -ItemType Directory -Force -Path disk_image_patch_042\MIPS_NATIVE\tiny_mc
Copy-Item -Force homebrew\tiny_mc\tiny_mc disk_image_patch_042\MIPS_NATIVE\tiny_mc\tiny_mc
Copy-Item -Force disk_image\MIPS_NATIVE\tiny_mc\README.txt disk_image_patch_042\MIPS_NATIVE\tiny_mc\README.txt
```

Verification:

```text
Contains strings: /mnt/sdcard/cubegm/icube, FN shortcut armed after release,
FN startup state ignored until release, FN pressed: launching %s
Does not contain strings: icube heartbeat, shmget
Tiny MC size: 40776 bytes
Tiny MC SHA256: 1C01D7B90C58FBB3ADCDA99AC18C6BB5D8AA666AFEA34D09FC8F2B1360A21B5F
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_042\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
```

## 2026-05-28 button click audio rebuild

Purpose:

Add short audible clicks to Tiny MC when buttons are newly pressed.

Design:

- Tiny MC now resolves `sound_driver_init`, `sound_driver_playframe`,
  `sound_driver_flush`, and `sound_driver_deinit` from the already loaded
  `cubegm/driver.so`.
- This follows the stock `rkgame` LibRetro audio route documented in
  `AUDIO_PATH_ANALYSIS.md`, instead of writing directly to `/dev/auddec`.
- `sound_driver_init(0, 44100, 2)` is called after `driver.so` loads.
- A new button press starts a short decaying 2200 Hz stereo click.
- The click is mixed into the 50 ms Tiny MC loop buffer and submitted through
  `sound_driver_playframe(samples, frames)`.
- The first raised Fn state during startup is still ignored until release, so it
  does not create a startup click.
- Audio is flushed/deinitialized before `driver.so` is closed, including before
  launching child programs or returning to stock `icube`.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc,patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\MIPS_NATIVE\tiny_mc\README.txt -Destination patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\README.txt -Force
New-Item -ItemType Directory -Force -Path patches\disk_image_patch_045
Copy-Item -LiteralPath patches\disk_image_patch_tiny_mc\cubegm -Destination patches\disk_image_patch_045\cubegm -Recurse -Force
Copy-Item -LiteralPath patches\disk_image_patch_tiny_mc\MIPS_NATIVE -Destination patches\disk_image_patch_045\MIPS_NATIVE -Recurse -Force
.\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
.\tools\scan-download.ps1 .\disk_image\MIPS_NATIVE\tiny_mc\tiny_mc
.\tools\scan-download.ps1 .\patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc
.\tools\scan-download.ps1 .\patches\disk_image_patch_045\MIPS_NATIVE\tiny_mc\tiny_mc
.\tools\scan-download.ps1 .\patches\disk_image_patch_tiny_mc\cubegm\rkgame
.\tools\scan-download.ps1 .\patches\disk_image_patch_045\cubegm\rkgame
```

Verification:

```text
Tiny MC ELF: class=1 (ELF32), data=1 (little-endian), machine=0x0008 (MIPS)
Tiny MC size: 41640 bytes
Tiny MC SHA256: E43432F1800BDF7049E825CE471063E82694A1972523D50B799E0A3210B7E660

Contains strings:
  /mnt/sdcard/cubegm/icube
  click audio
  driver.so sound
  sound_driver_init
  sound_driver_playframe
  sound_driver_flush
  sound_driver_deinit

Does not contain strings:
  icube heartbeat
  shmget

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_045\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_tiny_mc\cubegm\rkgame: found no threats
Defender scan patches\disk_image_patch_045\cubegm\rkgame: found no threats
```

Note:

An initial `zig objdump -f` / `zig objdump --file-headers` check failed because
this bundled Zig objdump wrapper did not accept those arguments. The ELF header
was then verified with a PowerShell byte read: `e_ident[4]=1`, `e_ident[5]=1`,
`e_machine=0x0008`.

## 2026-05-28 FreeType text renderer rebuild

Purpose:

Add TrueType text rendering to Tiny MC through the stock firmware FreeType
library, while keeping the old built-in 5x7 bitmap font as fallback.

Design:

- Tiny MC loads `/mnt/sdcard/cubegm/lib/libfreetype.so.6` with `dlopen()`.
- FreeType symbols are resolved with `dlsym()`; the executable is not linked
  directly against `libfreetype`, so startup can fall back safely.
- Preferred font:
  `/mnt/sdcard/MIPS_NATIVE/tiny_mc/fonts/JetBrainsMonoNL-Regular.ttf`.
- The downloaded font is JetBrains Mono NL Regular v2.304 under OFL-1.1.
- UTF-8 is decoded before rendering, so Cyrillic filenames can be displayed if
  the filesystem provides UTF-8 names.
- Glyphs are rendered into a small in-memory grayscale cache and alpha-blended
  into the RGB565 framebuffer.
- If FreeType, the font file, or a glyph load fails, the existing bitmap font
  path is used.

Commands from repository root:

```powershell
New-Item -ItemType Directory -Force .\homebrew\tiny_mc\fonts
Invoke-WebRequest -Uri https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/fonts/ttf/JetBrainsMonoNL-Regular.ttf -OutFile .\homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
Invoke-WebRequest -Uri https://raw.githubusercontent.com/JetBrains/JetBrainsMono/v2.304/OFL.txt -OutFile .\homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt
.\tools\scan-download.ps1 .\homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf
.\tools\scan-download.ps1 .\homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force .\disk_image\MIPS_NATIVE\tiny_mc\fonts
Copy-Item -LiteralPath .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath .\homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf -Destination .\disk_image\MIPS_NATIVE\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf -Force
Copy-Item -LiteralPath .\homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt -Destination .\disk_image\MIPS_NATIVE\tiny_mc\fonts\JetBrainsMono-OFL.txt -Force
```

Patch directory:

```text
patches\disk_image_patch_049
```

Verification:

```text
Tiny MC size: 46712 bytes
Tiny MC SHA256: 67560BCC8983DDE6306238CD19204881D90BF82E697E8FBCE85ACA8932E4040A

JetBrainsMonoNL-Regular.ttf size: 208576 bytes
JetBrainsMonoNL-Regular.ttf SHA256: FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A

JetBrainsMono-OFL.txt size: 4399 bytes
JetBrainsMono-OFL.txt SHA256: 60D55F23C6CE05A81099A762CB67CA2C9B6EA251C7912720998B4C89EBFD4FAA

Contains strings: libfreetype.so.6, FT_Init_FreeType, FT_New_Face,
FT_Load_Char, JetBrainsMonoNL-Regular.ttf
Still contains dynamic dependencies: libdl.so.2, libc.so.6

Defender scan homebrew\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf: found no threats
Defender scan homebrew\tiny_mc\fonts\JetBrainsMono-OFL.txt: found no threats
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\fonts\JetBrainsMonoNL-Regular.ttf: found no threats
Defender scan patches\disk_image_patch_049: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc: found no threats
```

## 2026-05-28 common font pool rebuild

Purpose:

Move bundled TrueType fonts from Tiny MC's private directory to the shared
native-app path `MIPS_NATIVE/common/fonts`, then add additional free
monospaced fallback fonts.

Downloaded fonts:

```text
JetBrainsMonoNL-Regular.ttf  208576 bytes  FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A
LiberationMono-Regular.ttf   319508 bytes  F2B83C763E8AFD21709333370BED4774337FAE82267937E2B5AEA7E2FBD922C1
DejaVuSansMono.ttf           340712 bytes  B4A6C3E4FAAB8773F4FF761D56451646409F29ABEDD68F05D38C2DF667D3C582
NotoSansMono-Regular.ttf     512836 bytes  D9E2B23D19F8230BE7146F409A52B1D23117E635E28F2E2892CF91B7382F325B
SourceCodePro-Regular.ttf    210312 bytes  74BD80D3E42A08517CD7E1108BA3D86F2DA29AC0F3065BE95E0357956AB9DB37
Hack-Regular.ttf             309408 bytes  15F55CC0C85A2988D2B4B3A8CDB5D77FDFBAF319E1BB5309D725DB9818FB7125
RobotoMono-Regular.ttf       125748 bytes  AF0BFF7599C3DF3831755C16E39B3C496DF74B8C8D8A1161B14DC8461BE17CB4
```

Source layout:

```text
homebrew\common\fonts
disk_image\MIPS_NATIVE\common\fonts
```

Build changes:

- Added `R36SX_MIPS_NATIVE_COMMON_DIR`,
  `R36SX_MIPS_NATIVE_COMMON_FONTS_DIR`, and `R36SX_DEFAULT_MONO_FONT_PATH` to
  `homebrew\common\hardware.h`.
- Tiny MC now searches common fonts first, then falls back to the old
  `MIPS_NATIVE/tiny_mc/fonts` location and stock firmware fonts.

Commands from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force .\disk_image\MIPS_NATIVE\common\fonts
Copy-Item -Path .\homebrew\common\fonts\* -Destination .\disk_image\MIPS_NATIVE\common\fonts -Force
Copy-Item -LiteralPath .\homebrew\tiny_mc\tiny_mc -Destination .\disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
```

Patch directory:

```text
patches\disk_image_patch_050
```

Verification:

```text
Tiny MC size: 46900 bytes
Tiny MC SHA256: DFC466EFEBFA07E4A0E8DEC6A0C4BCA3B18E5A1B028B5F8664633E169FD88D07

Contains strings: MIPS_NATIVE/common/fonts, LiberationMono-Regular.ttf,
DejaVuSansMono.ttf, NotoSansMono-Regular.ttf, SourceCodePro-Regular.ttf,
Hack-Regular.ttf, RobotoMono-Regular.ttf, libfreetype.so.6

Defender scan downloaded archives/raw files: found no threats
Defender scan homebrew\common\fonts: found no threats
Defender scan disk_image\MIPS_NATIVE\common\fonts: found no threats
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_050: found no threats
Defender scan patches\disk_image_patch_tiny_mc: found no threats
```

## 2026-05-28 larger FreeType UI text rebuild

Purpose:

Make Tiny MC's FreeType text roughly 20 percent larger for readability on the
device screen.

Source changes:

- `TINY_FT_SMALL_PX`: 10 -> 12
- `TINY_FT_LARGE_PX`: 16 -> 19
- FreeType baseline offsets: 9/15 -> 11/18
- `LIST_ROW_H`: 18 -> 22, so file-list rows keep enough vertical room.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
patches\disk_image_patch_051
```

Verification:

```text
Tiny MC size: 46916 bytes
Tiny MC SHA256: 845CE784ECDE9241C509C67F305895C43A997326BF070F783AC632261CD84A09

Contains strings: MIPS_NATIVE/common/fonts, libfreetype.so.6,
JetBrainsMonoNL-Regular.ttf, LiberationMono-Regular.ttf, RobotoMono-Regular.ttf

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_051: found no threats
Defender scan patches\disk_image_patch_tiny_mc: found no threats
```

## 2026-05-28 runtime font config rebuild

Purpose:

Add `MIPS_NATIVE\tiny_mc\tiny_mc.conf` next to the Tiny MC executable and make
the FreeType font path and text sizes configurable at runtime.

Config file:

```text
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc.conf
```

Default contents:

```text
font=/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf
small_px=12
large_px=19
list_row_h=22
```

Behavior:

- Tiny MC tries the absolute config path first, then `tiny_mc.conf` from the
  current working directory.
- Supported aliases include `font_path`, `font_small_px`, `font_large_px`,
  `font_size`, and `row_h`.
- Integer values are clamped to conservative ranges.
- If `list_row_h` is omitted, Tiny MC keeps it at least `large_px + 3`.
- If the configured font cannot be read or opened by FreeType, Tiny MC falls
  back through the common mono font pool and then stock firmware fonts.

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directory:

```text
patches\disk_image_patch_052
```

Verification:

```text
Tiny MC size: 54208 bytes
Tiny MC SHA256: FEA903CABAEFEE5CB8871512903C821A0ABC2A67248C5FC6827A9C7CEBAD16C8

tiny_mc.conf size: 452 bytes
tiny_mc.conf SHA256: 06F53A12D7B4EBC88B587FABF0CB531FB601041E78D025AC0F58CECF443F5D70

Contains strings: tiny_mc.conf, config loaded, font_size, small_px, large_px,
list_row_h, MIPS_NATIVE/common/fonts, libfreetype.so.6

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan homebrew\tiny_mc\tiny_mc.conf: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc.conf: found no threats
Defender scan patches\disk_image_patch_052: found no threats
Defender scan patches\disk_image_patch_tiny_mc: found no threats
```

## 2026-05-28 preserve mixer volume rebuild

Purpose:

Avoid audible volume jumps when Tiny MC initializes click audio through
`driver.so`. The vendor `sound_driver_init()` path opens `/dev/auddec` and also
reapplies the saved AV volume to `/dev/sndC0i2so`. Tiny MC now reads the current
mixer volume with `cube_ioctl(0x40010204)` before sound init and restores it
with `cube_ioctl(0x20010203)` immediately afterward.

Implementation:

- Added volume preserve/restore around `audio_driver_open()`.
- With `DEBUG=1`, Tiny MC logs:
  - `preserving mixer volume before sound init: N`
  - `restored mixer volume after sound init: N`

Build command from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directories:

```text
patches\disk_image_patch_056
patches\disk_image_patch_tiny_mc
```

Verification:

```text
Tiny MC size: 54516 bytes
Tiny MC SHA256: 144F89FA72FE1372B7E3F7E522BFD40C4F66243B149B4963E8D877AB51177B94
Contains strings: preserving mixer volume, restored mixer volume,
sound_driver_init, sound_driver_playframe
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_056: found no threats
```

## 2026-05-28 child-return input/display rebuild

Purpose:

Fix a black-screen/failed-return symptom after leaving native Pong or Button
Demo back to Tiny MC. The likely trigger is the standard native-app exit chord
`Select + Start`: when Tiny MC resumes, `Start` can still be physically held or
left in the input queue, and Tiny MC treats a fresh `Start` press as "run the
selected file" and immediately relaunches the same child.

Implementation:

- After a child process exits, Tiny MC now reopens the display with up to five
  attempts and redraws the menu immediately.
- After input devices are reopened, Tiny MC polls the current state and
  suppresses post-launch button input until all buttons are released once.
- `handle_buttons()` returns immediately after launching a child so it does not
  overwrite the refreshed post-child input state with stale pre-launch buttons.

Build command from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directories:

```text
patches\disk_image_patch_057
patches\disk_image_patch_tiny_mc
```

Verification:

```text
Tiny MC size: 55780 bytes
Tiny MC SHA256: B5E01B7DAFF20C0D82867702F6664A55E969EFDB60A13593F5962A49D14E40B0
Contains strings: suppressing post-launch buttons, post-launch buttons
released, display reopened after child, display reopen after child failed
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_057: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```

## 2026-05-28 self-exec child return rebuild

Purpose:

Fix the persistent black-screen return from native Pong/Button Demo. The device
log showed that after Pong exited, Tiny MC successfully reached:

```text
launch result: pong exited: 0 raw_status=0x0
Display via driver.so 640x480
scan_directory done ...
```

but the LCD still showed only a black screen with the battery OSD. That means
the child process exits and Tiny MC is alive, but reopening/reusing the vendor
display path inside the same process is not reliable after a native app has
also owned `driver.so`.

Implementation:

- After a real child process exits, Tiny MC now `execl()`s
  `/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc` with the current directory as its
  startup argument.
- This gives Tiny MC the same clean display initialization path as a cold
  launch from `icube.sh`.
- The old display reopen path remains as a fallback only if self-exec fails.

Build command from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directories:

```text
patches\disk_image_patch_059
patches\disk_image_patch_tiny_mc
```

Verification:

```text
Tiny MC size: 56796 bytes
Tiny MC SHA256: 72DB5041C4496DA9489D18CB22E4D14F860274B37336CBEB7DCE64B86EAC427B
Contains strings: restarting tiny_mc after child,
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc, tiny_mc self-exec failed
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_059: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```

## 2026-05-28 directory-enter debounce rebuild

Purpose:

Fix a regression from the child-return input/display rebuild: pressing `A` to
enter a directory could enter it and immediately return to the parent. The
cause was that `launch_selected()` also handles directories, but the new early
`return` after `launch_selected()` skipped updating `g_prev_buttons` for the
non-child directory case. A held `A` was therefore seen as a fresh press on the
next frame, often selecting `..`.

Implementation:

- Added `finish_button_frame()` to keep the common end-of-input-frame state
  update in one place.
- `launch_selected()` now returns whether it actually ran/recovered from a
  child process. Directory navigation returns `0`; child launch/reopen returns
  `1`.
- Directory enter/back actions now update `g_prev_buttons` before returning, so
  held buttons cannot immediately trigger the opposite navigation.

Build command from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Patch directories:

```text
patches\disk_image_patch_058
patches\disk_image_patch_tiny_mc
```

Verification:

```text
Tiny MC size: 56420 bytes
Tiny MC SHA256: FA262555F116BE3E5C37E386B519D5C70EDDF7D1619766BEED71B9BBBF153CC0
Contains strings: suppressing post-launch buttons, post-launch buttons
released, display reopened after child
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_058: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```
