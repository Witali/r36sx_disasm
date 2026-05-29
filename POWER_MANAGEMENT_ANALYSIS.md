# Power Management Analysis In The Game Side

## Short Conclusion

In `cubegm` / the game emulator frontend, I did not find signs of CPU halt, MIPS `wait`, or dynamic CPU frequency scaling.

What exists:

- normal `usleep(...)` / `SDL_Delay`-style delays;
- input polling roughly every 16 ms;
- frame pacing through delays, frame skip, and audio/video timing;
- backlight/MIPI/HDMI control;
- boot slot switching / reboot during exit or transitions between the game and projector UI.

What was not found:

- `cpufreq`;
- `scaling_governor`;
- `scaling_cur_freq`;
- `scaling_setspeed`;
- `scaling_min_freq`;
- `scaling_max_freq`;
- `/sys/devices/system/cpu/cpu*/cpufreq`;
- `/sys/power/state`;
- MIPS instruction `wait` (`0x42000020`) in the main binaries.

## MIPS `wait` / Halt Check

I searched for the little-endian encoding of MIPS `wait`: bytes `20 00 00 42`.

Results for the main binaries:

- `disk_image\cubegm\rkgame`: `0`
- `disk_image\cubegm\icube`: `0`
- `disk_image\cubegm\MyExecutable`: `0`
- `disk_image\cubegm\usr\bin\icube`: `0`
- `disk_image\rootfs\usr\bin\cubevol`: `0`
- `disk_image\rootfs\usr\bin\hcprojector`: `0`

The same pattern was also not found in MIPS cores from `disk_image\cubegm\cores` up to 15 MB in size.

Conclusion: userspace binaries do not show direct use of the MIPS CPU idle/halt instruction.

## `rkgame`

`disk_image\cubegm\rkgame` is the main frontend / emulator core loader.

Ghidra showed:

- `run_game()` selects a core, loads a ROM/ZIP, and calls `FBA_Load`, `Gpsp_Load`, `PCSX_Load`, or `Core_Load`.
- `DrawFrame()` handles rotation/skipframe/displayfps and calls `dispFlip(...)`.
- `PlayFrame()` effectively calls `PlaySound()`.
- Pause/menu/UI code contains many `usleep(...)` calls, but these are ordinary delays, not CPU control.
- Frame skip exists:
  - `skipframe_count = 0x14`
  - `skipframe_count = 0x1e`
  - `skipframe_count = 0x3c`
- There is audio-buffer-based behavior through strings/symbols such as `sound_driver_buffering_percent` and `retro_audio_buff_occupancy`.

Not found:

- CPU governor/frequency control;
- writes to cpufreq sysfs;
- direct CPU halt/wait;
- standby through `/dev/standby` inside the main game loop.

## `MyExecutable`

`disk_image\cubegm\MyExecutable` is a hardware helper for buttons, HDMI hotplug, MIPI, and backlight.

Important decompilation:

```c
void main(void) {
    ...
    do {
        key = key_getvalue();
        if (key == 0x100) {
            if (screen_off) {
                screen_off = false;
                set_mipi_onoff(1);
                set_backlight_value2(0xff);
                gpio_set_output(0x13, 0);
                *cubegame_state &= ~2;
                is_joykey_enabled = 1;
            } else {
                screen_off = true;
                set_backlight_value2(0);
                gpio_set_output(6, 0);
                gpio_set_output(0x13, 1);
                is_joykey_enabled = 0;
                *cubegame_state |= 2;
            }
        }
        usleep(16000);
    } while (true);
}
```

This looks like screen/backlight on/off handling, not CPU halt.

`set_backlight_value2()`:

```c
open64("/dev/backlight", O_RDWR);
write(fd, &brightness, 4);
fsync(fd);
close(fd);
```

`set_mipi_onoff()`:

- opens `/dev/backlight`;
- opens `/dev/mipi`;
- performs several `ioctl(...)` calls;
- sets backlight to `0` when switching off;
- calls `usleep(500000)` and `usleep(200000)`;
- restores brightness through `/dev/persistentmem` when switching on, if needed.

Conclusion: power saving here means turning off the display/MIPI/backlight, not lowering CPU frequency.

## `driver.so`

`disk_image\cubegm\driver.so` is loaded dynamically from `rkgame`.

It contains the string `/dev/standby`, but Ghidra shows that it is used in `set_bootup_slot()`:

```c
set_bootup_slot(slot) {
    fd = open64("/dev/standby", O_RDWR);
    ioctl(fd, 0x20002709, slot);
    close(fd);
}
```

`exit_game()`:

```c
set_bootup_slot(0x11);
if (exists("/mnt/sdcard/cubegm/avexit")) {
    system("/mnt/sdcard/cubegm/avexit");
}
system("reboot -f");
```

`enter_multios_app()`:

```c
fbdev_deinit();
sound_driver_deinit();
set_multios_app(...);
set_bootup_slot(0x21);
if (exists("/mnt/sdcard/cubegm/avexit")) {
    system("/mnt/sdcard/cubegm/avexit");
}
fbdev_aspect_ratio_wrok(0);
api_osd_show_onoff(0);
system("/mnt/sdcard/cubegm/icubemp_start.sh &");
```

Conclusion: `/dev/standby` is used here as a boot slot / multi-OS switching interface, not as suspend/halt in the game loop.

## Where Real Standby Exists

Real standby/sleep was found not in `rkgame`, but in the system/projector side:

- `disk_image\rootfs\usr\bin\hcprojector`
  - strings: `MSG_TYPE_STANDBY_SLEEP`, `MSG_TYPE_WAKE_UP`, `api_system_standby`, `api_dis_suspend`, `/dev/standby`, `Enter standby?`, `Auto Sleep`.

- `disk_image\rootfs\usr\bin\cubevol`
  - strings/symbols: `standby_sleep.c`, `mp5_enter_standby`, `enter_sleep_mode_countdown`, `power_handle_proc`, `battery_monitor`, `Low battery`.

- `disk_image\rootfs\usr\lib\libhudi.so`
  - functions: `hudi_standby_enter`, `hudi_standby_ddr_pwroff_set`, `hudi_standby_saradc_wakeup_set`, `hudi_standby_gpio_wakeup_set`, `hudi_standby_ir_wakeup_set`.
  - device: `/dev/standby`.

The HCRTOS SDK contains a bootloader standby example:

- `internet_sources\hcrtos\components\applications\apps-bootloader\source\cmd\standby.c`

Its logic:

```c
fd = open("/dev/standby", O_RDWR);
ioctl(fd, STANDBY_GET_BOOTUP_MODE, &mode);
usleep(500000);
ioctl(fd, STANDBY_ENTER, 0);
close(fd);
```

This is the real standby path of the platform.

## Likely Power-Saving Model

1. During normal gameplay, userspace does not stop the CPU and does not lower CPU frequency.
2. The emulator keeps speed through `usleep`, frame skip, audio buffering, and `dispFlip`.
3. In pause/menu, the CPU is still not halted, but some threads wait through `usleep`.
4. Helper code can turn off the screen/backlight/MIPI. This likely saves much more power than an idle frontend, but the CPU keeps running.
5. Full standby is handled by the system side through `/dev/standby` and Hichip/HUDI APIs, probably with involvement from the driver/AVP/RTOS.

## What To Try For Our Own Frontend

If the goal is saving power:

- Use `usleep(16000)` or longer in idle/menu states if 60 Hz polling is not needed.
- Avoid busy-wait loops like `OS_Delay()` from `MyExecutable`; they spin the CPU.
- Turn off backlight through `/dev/backlight`.
- Turn off MIPI/LCD through the same ioctl path used by `set_mipi_onoff()`.
- For full sleep, study `/dev/standby` ioctls from `libhudi.so`, `hcprojector`, `cubevol`, and HCRTOS `standby.c`.
- Do not rely on standard Linux cpufreq: the dump does not show the usual cpufreq sysfs/userspace interface.

## `goph-R/SF3000-RE` Check

Repository: https://github.com/goph-R/SF3000-RE

Local copy: `internet_sources\SF3000-RE`

State at the time of checking: the git tree contains only `README.md` and `LICENSE`; there are no C/C++/asm sources, Makefile, drivers, or launcher code.

Search results across `README.md` and the repository tree:

- `halt`: not found.
- `wait`: not found.
- `powersave` / `power saving`: not found.
- `cpufreq` / `frequency`: not found.
- `standby`: not found.
- `suspend`: not found.
- `sleep`: not found.
- `idle`: not found.

What is useful in that repository for this topic:

- confirms MIPS32 little-endian / MIPS32r2;
- confirms Buildroot `2021.05-rc2`;
- confirms the `rootfs/` + `cubegm/` model;
- confirms `cubegm/icube` as a working launch point for a custom MIPS32 static binary;
- mentions DirectFB in `cubegm` and the lack of SDL/EGL/GLES in the stock system.

Conclusion: `goph-R/SF3000-RE` is useful as a note about homebrew/custom launcher work, but it does not provide source files that could confirm `halt`, MIPS `wait`, cpufreq, or a power-saving mode. Answering the power-management question still depends primarily on local binary analysis of `rkgame`, `driver.so`, `hcprojector`, `cubevol`, `libhudi.so`, and HCRTOS standby code.

## libc `usleep` And Kernel `nanosleep` Check

The local `disk_image\rootfs\lib\libc.so.6` was additionally checked in Ghidra:

- `usleep` is at address `000f5ed0`;
- `nanosleep` is at address `000bfb84`;
- `clock_nanosleep` is at address `0010ea90`;
- Ghidra identified the library as `MIPS:LE:32`.

Decompiled `usleep` logic:

```c
local_10.tv_sec = __useconds / 1000000;
local_10.tv_nsec = (__useconds % 1000000) * 1000;
return nanosleep(&local_10, 0);
```

In the MIPS disassembly, `nanosleep` loads `v0 = 0x1046` before `syscall`. In other words, userspace `usleep` does not contain its own wait loop, busy-wait, `halt`, or MIPS `wait`; it converts microseconds to a `timespec` and hands control to the kernel through the `nanosleep` syscall.

`disk_image\cubegm\advapi32.dll` was confirmed as a U-Boot `uImage` named `vmlinux`, MIPS, gzip. The payload was unpacked to `ghidra_exports\kernel\advapi32_vmlinux_decompressed.bin`; inside it there is the string:

```text
Linux version 4.4.186-release (linsen.chen@hichip01) (gcc version 6.3.0 (Codescape GNU Tools 2018.09-02 for MIPS MTI Linux) ) #21 PREEMPT Thu Dec 18 18:13:45 CST 2025
```

The unpacked kernel also contains strings `cpu_wait` and `wait instruction`, so the kernel idle path probably can use MIPS `wait`. But this is a separate layer: the application calls `usleep`/`nanosleep`, the scheduler removes the task from execution, and if the system has nothing else to run, the kernel can enter idle/wait.
