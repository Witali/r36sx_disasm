# Key Facts About `disk_image`

## Overall Identification

- `disk_image` looks like the contents of an SD/FAT partition rather than one monolithic firmware image: ROM collections, media, UI resources, plus firmware artifacts in `cubegm` and an extracted/related `rootfs`.
- The device belongs to the Hichip HC16xx / HC1600A platform.
- Main Linux/userspace architecture: ELF32 little-endian MIPS, O32 ABI.
- CPU from the DTB: `MIPS 74Kc`.
- Board/variant from the DTB: `hc1600a@dbE3100v20`.
- `rootfs` is based on Buildroot 2021.05-rc2.

## Important Disguised Files

- `disk_image\cubegm\advapi32.dll` is not a Windows DLL, but a U-Boot legacy `uImage` named `vmlinux`.
- `disk_image\cubegm\ApplicationFrame.dll` is not a Windows DLL, but a U-Boot legacy `uImage` named `avp`.
- `disk_image\cubegm\Bubbles.scr` is not a screensaver, but a DTB/FDT device tree.
- This looks like intentional hiding of boot artifacts on the FAT partition under harmless Windows-style names.

## Boot Artifacts

- `disk_image\cubegm\advapi32.dll`
  - Type: U-Boot legacy uImage
  - Name: `vmlinux`
  - Data size: `3905915`
  - Load: `0x80000000`
  - Entry: `0x803337C0`
  - Arch code: `5` = MIPS
  - SHA256: `8DB1F56E59A05C58BCCE15EE3B2927A4E9118ED02FE3ED8094AB1890D773DD61`

- `disk_image\cubegm\vmlinux.uImage`
  - Type: U-Boot legacy uImage
  - Name: `vmlinux`
  - Data size: `3905906`
  - Load: `0x80000000`
  - Entry: `0x803337C0`
  - Arch code: `5` = MIPS
  - SHA256: `53B3E0B3D57FCDBEF40D448AE2D3A00159BC84F2FD5BE4C10A826827C7F2E01E`

- `disk_image\cubegm\ApplicationFrame.dll`
  - Type: U-Boot legacy uImage
  - Name: `avp`
  - Data size: `1381415`
  - Load: `0x83DA4000`
  - Entry: `0x83DA4000`
  - Arch code: `5` = MIPS
  - SHA256: `7FE578665A9AA9707FDE83CAE04A798CA332C7164E56C531C143E3136D88CF5C`

- `disk_image\cubegm\avp.uImage`
  - Type: U-Boot legacy uImage
  - Name: `avp`
  - Data size: `1381540`
  - Load: `0x8BDA4000`
  - Entry: `0x8BDA4000`
  - Arch code: `5` = MIPS
  - SHA256: `A9788995DB80197DB6D743B8B237EADBFD3C424FCF1AB448FCD99C003B0950B2`

- `disk_image\cubegm\dtb.bin`
  - Type: DTB/FDT
  - SHA256: `1258F1EBA809E43540C581B815C87815540A9E5897A4E8584363AB7DE5CC27BB`

- `disk_image\cubegm\Bubbles.scr`
  - Type: DTB/FDT
  - SHA256: `220BE381988651E73F0538D41B446762AA5ABD7FD72A808F2F26036970A992CB`

## DTB / Device Tree Facts

The following strings were found in `dtb.bin` and `Bubbles.scr`:

- `Hichip hc16xx`
- `Hichip,1600`
- `MIPS 74Kc`
- `hichip,hc16xx`
- `soc-16xx`
- `hc1600a@dbE3100v20`
- `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc`
- `external_files`
- `cubegm`
- `dtb.bin`
- `avp.uImage`
- `vmlinux.uImage`

This confirms that `cubegm` contains not only the game frontend, but also external boot files for the platform.

## Main Executables

- `disk_image\rootfs\usr\bin\hcprojector`
  - MIPS ELF32 LE EXEC
  - Size: `14737552`
  - Main projector application.
  - SHA256: `95B89D1503CBD281A1C3F7C1C14D87763F046CD29D217DCEE0BC7FBD51D70801`

- `disk_image\rootfs\usr\bin\cubevol`
  - MIPS ELF32 LE EXEC
  - Size: `268544`
  - System controller for power, standby, backlight, HDMI/MIPI, and input.
  - SHA256: `930288B2DBB8D3B5E723701AC365D7180DD8E747279693BAC0D3240A4224B5A0`

- `disk_image\cubegm\rkgame`
  - MIPS ELF32 LE EXEC
  - Size: `1178732`
  - Main game frontend / emulator core loader.
  - SHA256: `57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9`

- `disk_image\cubegm\icube`
  - MIPS ELF32 LE EXEC
  - Size: `11976`
  - Small launcher that starts `rkgame`.
  - SHA256: `21EADC68EDC6298137E941D4C50DADB4314FAC0FC68F9856649284308B000522`

- `disk_image\cubegm\usr\bin\icube`
  - MIPS ELF32 LE EXEC
  - Size: `63616`
  - Alternative/older `icube`; its strings make it look more like a GBA/gpsp-oriented app.
  - SHA256: `2146C4C61C6D5AE1A7C3B5F53E05445A8D3913952C9130EC113BF6D5F57828F7`

- `disk_image\cubegm\MyExecutable`
  - MIPS ELF32 LE EXEC
  - Size: `24672`
  - Hardware helper for MIPI, HDMI hotplug, backlight, joystick/status.
  - SHA256: `B1F8C1D3EBA5882D6528F503E6EC73C2382783BBB1CD82A106C17A4BAF5F9418`

## ELF Statistics For The Firmware Area

Across `disk_image\cubegm` and `disk_image\rootfs`:

- `430` ELF32 LE MIPS shared object / DYN
- `342` ELF32 LE MIPS executable / EXEC
- `24` ELF32 LE MIPS relocatable / REL
- `5` ELF32 LE ARM shared object / DYN

The ARM `.so` files stand out from the main architecture and are most likely foreign, misplaced, or unused files:

- `disk_image\cubegm\cores\libemu_fbalpha.so`
- `disk_image\cubegm\lib\libcrypto.so`
- `disk_image\cubegm\lib\libcrypto.so.1.0.0`
- `disk_image\cubegm\lib\libfreetype.so.6`
- `disk_image\cubegm\lib\libz.so.1`

The primary target architecture for our own binaries is still MIPS32 little-endian, not ARM.

## Frontend / App Startup And Switching

`disk_image\rootfs\etc\inittab` starts the following through BusyBox init:

- `/etc/init.d/rcS`
- `/usr/bin/cubevol > /dev/console 2>&1` in `respawn`

The latest check on real hardware with `boot_route.log` showed that cold boot does not enter through `icube_start.sh`, but directly through:

- `/mnt/sdcard/cubegm/icube.sh`

Key log lines:

- `script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=start args=`
- `event=launching /mnt/sdcard/cubegm/icube`
- `event=started /mnt/sdcard/cubegm/icube child=556`
- `ps` also showed `hcdaemon`, `/mnt/sdcard/cubegm/icube`, and `rkgame`

Considering `rootfs/etc/init.d/S41hcdaemon`, which contains only `hcdaemon&`, the most likely cold boot chain is:

- `init -> /etc/init.d/rcS -> /etc/init.d/S41hcdaemon -> hcdaemon -> /mnt/sdcard/cubegm/icube.sh`

Practical conclusion: replacing the frontend on cold boot requires patching `cubegm/icube.sh`; `icube_start.sh` remains important for transitions from `hcprojector`, but it was not involved in the captured boot route. Details: `BOOT_ROUTE_FINDINGS.md`.

The `cubegm` directory contains switching scripts:

- `disk_image\cubegm\icube.sh`
  - Adds `cubegm\lib` and `cubegm\usr\lib` to `LD_LIBRARY_PATH`.
  - Starts `/mnt/sdcard/cubegm/icube`.
  - Runs `init -q`.

- `disk_image\cubegm\icube_start.sh`
  - Kills `hcprojector`.
  - Starts `/mnt/sdcard/cubegm/icube`.

- `disk_image\cubegm\icubemp_start.sh`
  - Kills `rkgame` and `icube`.
  - Starts `/usr/bin/hcprojector`.

This is a convenient soft-customization point without repacking the kernel.

## Emulators And The Game Frontend

- `disk_image\cubegm\rkgame` uses `dlopen/dlsym` and loads `.so` cores from `disk_image\cubegm\cores`.
- `disk_image\cubegm\cores\config.xml` maps ROM extensions to emulator cores.
- `rkgame` strings show signs of a libretro-like API: `retro_*`, save states, screenshots, settings, PCSX/PSX, FBA/MAME/SNES/GBA/MD/FC, and so on.
- `disk_image\cubegm\setting.xml` contains user settings, hotkeys, and resource paths.

### Possible Frontend Replacement / File Manager

`disk_image\cubegm\icube` contains the direct string `/mnt/sdcard/cubegm/rkgame` and `killall rkgame`, so a soft frontend replacement is possible by replacing `cubegm\rkgame`: the original binary can be renamed, and our own MIPS ELF launcher/file manager can be placed under the original name. That launcher can start the original `rkgame` when needed.

Things to account for in this setup:

- `icubemp_start.sh` currently kills only `rkgame` and `icube`; if the original is renamed, the return script must be extended for the new process name.
- Before launching another program, the file manager should correctly release framebuffer/audio/input, or launch the child in a way that avoids conflicts over display/sound.
- Real Midnight Commander (`mc`) is a full-screen text-mode app based on ncurses/S-Lang. Cross-building it is possible to try, but showing it on the console screen would require a working terminal environment/fbcon/tty and translation of the device buttons into keyboard events. A small MC-like framebuffer file manager controlled by the same buttons as our homebrew modules is the more practical first step.

### Fn Button And Hotkeys

Fn is explicitly present in user settings:

- `disk_image\cubegm\setting.xml`
  - `<gamemenuhotkey>SELECT+START</gamemenuhotkey>`
  - `<quicksavehotkey>FN+A</quicksavehotkey>`
  - `<quickloadhotkey>FN+B</quickloadhotkey>`
  - `<quicksnaphotkey>FN+START</quicksnaphotkey>`
  - `<savestatehotkey>DISABLE</savestatehotkey>`

Fn is also present in the device tree:

- `disk_image\cubegm\dtb.bin` and the disguised duplicate `disk_image\cubegm\Bubbles.scr`
  - `/panel/key-fn = 0x00000002`
  - nearby properties describe `/panel/key-do = 0x00000009`, `key-clk = 0x00000008`, `key-tl1 = 0x0000000b`, `key-tr1 = 0x00000007`, `key-shoulder-left = 0x0000000b`, `key-shoulder-right = 0x00000007`, `key-volume-down = 0x0000000c`, `key-volume-up = 0x0000000d`.

`rkgame` strings contain names such as `quicksavehotkey`, `quickloadhotkey`, `quicksnaphotkey`, `savestatehotkey`, `GameMenuHotKey`, `KeyMapping`, `KeyName`, `SetKeyMap`, and `gpio_get_input`. This suggests Fn is handled as a frontend/system modifier rather than a regular libretro button. `driver.so` and `MyExecutable` also contain `/tmp/joy_key` / `gpio_get_input`; `MyExecutable` reads several `/proc/device-tree/panel/key-*` properties, but the string `/proc/device-tree/panel/key-fn` was not found in it.

Practical conclusion: Fn is described in hardware and used by launcher settings for quicksave/quickload/screenshot. For homebrew cores it should be tested with a separate raw-scanner module: the launcher may intercept Fn combinations before `retro_input_state()` sees them.

## `rkgame` Display Path: What The First Tiny MC Missed

After the first Tiny MC test on hardware, the screen stayed white for a few seconds and then became black with only the battery icon visible. Decompiling `rkgame` and `driver.so` showed that the stock launcher does not draw directly to `/dev/fb0` like a regular Linux framebuffer app.

Important `rkgame` sequence:

1. `main` calls `InitDisplay()` before `InitSound()` and `InitJoystick()`.
2. `InitDisplay()` calls `dlopen("%s/driver.so", work_path, RTLD_NOW)`.
3. Then `dlsym()` looks up `video_driver_setting`, `video_drivers_init`, `video_driver_disp_frame`, `video_driver_setmode`, `video_driver_get_size`, `api_cube_malloc`, `api_cube_free`, `check_exit_game`, `api_cube_send_msg`, and `cube_ioctl`.
4. `video_driver_setting()` is called with a five-int block, effectively `{1, 1, 1, 0x356, 0x1e0}`.
5. Then `video_drivers_init()` is started, and frames are displayed through `video_driver_disp_frame()`.

What `driver.so` does:

- `video_drivers_init()` creates a frame structure and calls `fbdev_init()`.
- `fbdev_init()` opens `/dev/dis` and `/dev/fb0`, performs blank/unblank ioctls (`0x4611`), reads/changes framebuffer geometry, opens HCGE, configures rotate/aspect/fullscreen state, and starts render/painter threads.
- `video_driver_disp_frame(ptr, w, h, pitch)` can reinitialize the framebuffer on size changes, then calls `fbdev_draw_frame()`.
- `ReadJoystickProc()` in `rkgame` is also tied to `driver.so`: if shared key memory is not attached, it calls `cube_ioctl(0x40050209)`, then always calls `cube_ioctl(0x40050208, &Cube_State_Reg)`.

Practical conclusion: a standalone replacement for `rkgame` must either use `driver.so`, or almost fully reproduce its `/dev/dis` + `/dev/fb0` + HCGE/rotation/blanking initialization. The first Tiny MC skipped this layer, which explains the white/black screen while the battery overlay remained alive. `disk_image_patch_022` rebuilt Tiny MC so 640x480 frames are sent through `video_driver_disp_frame()`. The next risk after fixing the display path is input, because stock `rkgame` reads buttons not only through Linux evdev/js, but also through `cube_ioctl`.

## `rkgame` Button Masks And `/tmp/joy_key`

After Tiny MC displayed an image but did not react to buttons, it became clear that the built-in controller should be read like `rkgame` reads it, not primarily through normal `/dev/input/js*`.

`ReadJoystickProc()` in `rkgame` does the following:

1. If `KEY_VALUE_Addr == NULL`, it calls `cube_ioctl(0x40050209, &KEY_VALUE_Addr)`. In `driver.so` this command runs `cube_shmget_init("/tmp/joy_key", 8)` and returns the shared memory address.
2. Reads `KEY_VALUE_Addr[0]` into `joy_key`.
3. Reads `KEY_VALUE_Addr[1]` into the second joy word.
4. Calls `cube_ioctl(0x40050208, &Cube_State_Reg)`.

`joykey_explaned()` shows these masks:

```text
SELECT 0x00000001
L3     0x00000002
R3     0x00000004
START  0x00000008
UP     0x00000010
RIGHT  0x00000020
DOWN   0x00000040
LEFT   0x00000080
L2     0x00000100
R2     0x00000200
L      0x00000400
R      0x00000800
X      0x00001000
A      0x00002000
B      0x00004000
Y      0x00008000
FN     0x00010000
```

An additional check of `rkgame` `joy_key_mask` at address `0x004f3548` showed that shoulder buttons/triggers exist in the stock libretro map:

```text
retro id 10 L  -> 0x00000400
retro id 11 R  -> 0x00000800
retro id 12 L2 -> 0x00000100
retro id 13 R2 -> 0x00000200
retro id 14 L3 -> 0x00000002
retro id 15 R3 -> 0x00000004
```

This explains why the early native Button Demo did not see the rear buttons: `homebrew/common/hardware.h` originally stored only D-pad, A/B/X/Y, Start/Select, and Fn. Starting with the next Button Demo build, these masks are added and logged as `L`, `R`, `L2`, `R2`, `L3`, `R3`.

`disk_image_patch_024` changes Tiny MC: it now `dlsym()`s `cube_ioctl`, obtains `/tmp/joy_key` through `0x40050209`, reads raw masks every frame, logs `raw0/raw1/state/buttons` to `tiny_mc.log`, and only then supplements the result with fallback `/dev/input/js*` and evdev sources.

## `icube` Restarts `rkgame` Without A Heartbeat

Ghidra analysis of `cubegm/icube` showed that it is a userspace supervisor for the stock launcher, not just a helper utility:

1. `icube` creates/attaches SysV shared memory: `shmget(0x4d2, 0x1c4, IPC_CREAT | 0666)`.
2. It starts `/mnt/sdcard/cubegm/rkgame` through `fork()` + `execl()`.
3. After the first `sleep(6)`, it checks `shm[1]` once per second.
4. If `shm[1]` does not change or is zero, it calls `system("killall rkgame")` and starts `rkgame` again.

The stock `rkgame` services this through `XintiaoThread`: every 20 ms it calls `xintiao()`, which sets `shm[0] = 1` and increments `shm[1]`.

Consequence: any replacement for `cubegm/rkgame` must reproduce this heartbeat, or `icube` will restart it after roughly 5-6 seconds. Tiny MC does this starting with `disk_image_patch_026`.

## Public Sources / SDKs Found

The public HCRTOS source/SDK tree was found under `internet_sources\hcrtos`.

Especially important parts:

- `internet_sources\hcrtos\components\applications\apps-projector\source\hcprojector_app`
  - Projector application sources.
  - `projector.c` contains the string `Welcome to Hichip world!`, matching strings from `hcprojector`.

- `internet_sources\hcrtos\configs`
  - Many defconfigs for HC16xx projector.
  - Especially close names: `hichip_hc16xx_*_d3100_*_projector_*`.

- `internet_sources\hcrtos\board\hc16xx\projector`
  - DTS/board files for HC16xx projector variants.

- `internet_sources\hcrtos\components\hcfota`
  - OTA/update components.

- `internet_sources\hcrtos\components\liblvgl`
  - LVGL-related sources.

Conclusion: for custom firmware, it is better to start from this HCRTOS SDK/tree instead of starting from scratch.

## Practical Modification Options

### Safest Option

Keep the vendor kernel, DTB, AVP, and rootfs as they are, and replace/add a userspace application:

1. Build our own ELF for MIPS32 little-endian O32.
2. Link against existing libraries from `cubegm\lib`, `cubegm\usr\lib`, `rootfs\lib`, and `rootfs\usr\lib`.
3. Start our binary through `icube_start.sh`, replace `rkgame`, or replace the `icube` launcher.
4. For UI/output, use available framebuffer/SDL/DirectFB/LVGL/Hichip libraries.

### Medium Risk

Build our own version of `hcprojector` or the game frontend based on the found HCRTOS SDK, while preserving the vendor kernel/AVP/DTB.

### High Risk

Change boot artifacts:

- `vmlinux.uImage` / `advapi32.dll`
- `avp.uImage` / `ApplicationFrame.dll`
- `dtb.bin` / `Bubbles.scr`

This requires correct U-Boot legacy headers, correct load/entry addresses, and a compatible DTS. Before any replacement, keep originals and their SHA256 hashes.

### Fully Custom Firmware

Theoretically possible, but in practice the HCRTOS SDK is the better path. Mainline Linux would be difficult because the hardware depends on vendor drivers and the Hichip AVP:

- `/dev/ge`
- `/dev/dis`
- `/dev/hdmi`
- `/dev/auddec`
- `/dev/backlight`
- AVP proxy devices

## Recommended Next Work

1. Decompile/disassemble `rkgame`, `icube`, and `MyExecutable` in Ghidra.
2. Compare the current `dtb.bin` with DTS files from `internet_sources\hcrtos\board\hc16xx\projector`.
3. Set up a MIPS32 little-endian O32 toolchain.
4. Build a minimal test ELF that writes to stdout/log and can be started through `icube_start.sh`.
5. Then build our own frontend or `rkgame` replacement.
