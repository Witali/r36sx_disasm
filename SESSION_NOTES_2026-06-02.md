# Chat Summary for 2026-06-02

This document records what was done during the current chat: DTB/DTS analysis,
Linux/Debian boot evaluation, ABI/FPU checks, and adding the native `shell`
program for the device.

## DTB/DTS

- Found the device DTB: `disk_image/cubegm/dtb.bin`.
- Decompiled the DTB through Linux/WSL `dtc` into:
  `disk_image/cubegm/dtb.dts`.
- Checked the DTS by compiling it back into a DTB. Only the original `/hcrtos/sfspi/*`
  `reg` warnings remained.
- Added comments to `dtb.dts` for devices based on local analysis and open
  sources: CPU, SoC, UART, AVP/hcRTOS, SPI NOR, USB, MMC, GPIO, ADC,
  display/panel.
- These DTS comments are already committed:
  `c46e0ba Add annotated HC16xx device tree source`.

## Disk Image and Linux Boot

- `disk_image` is currently an unpacked SD-card/firmware structure, not a single
  raw disk image.
- Main boot artifacts in `disk_image/cubegm`:
  - `vmlinux.uImage` - U-Boot legacy Linux/MIPS kernel image.
  - `advapi32.dll` - also a U-Boot legacy Linux/MIPS kernel image with a similar
    purpose despite the DLL extension.
  - `avp.uImage` - firmware image for AVP/hcRTOS.
  - `dtb.bin` - device tree.
  - `Bubbles.scr` - also a DTB-like blob with a different memory map.
- In the DTS, `external_files` points to:
  - `cubegm/dtb.bin`
  - `cubegm/avp.uImage`
  - `cubegm/vmlinux.uImage`
  - `cubegm/xgame-logo.bmp`
- Current `bootargs` use ramfs:
  `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 ...`
- The kernel/modules contain support for FAT/VFAT/MSDOS, MMC/DW-MMC, and
  ext2/ext3/ext4. Therefore an SD card with a small FAT boot partition and an
  ext4 root partition looks technically feasible.
- Booting from an ext4 root will require changing bootargs/DTS or the initramfs
  script: the stock boot scheme still expects `root=/dev/ram0`.

## Debian and ABI

- The stock ELF binaries and the native programs we built use this ABI:
  - ELF32 little-endian MIPS.
  - MIPS32r2.
  - O32.
  - hard-float.
  - dynamic linker `/lib/ld.so.1`.
- The stock rootfs uses glibc; the previously observed level was around
  GLIBC 2.20.
- Debian is potentially possible as a `mipsel` hard-float rootfs/chroot,
  especially based on Debian Bookworm. Newer Debian releases must be checked
  separately because `mipsel` support has changed or been reduced.
- A full Debian boot needs separate work:
  - a rootfs for the correct MIPS ABI;
  - bootargs for an ext4 root;
  - correct `/dev`, `/proc`, `/sys`, and init setup;
  - preserving access to the vendor `driver.so`/AVP/hcRTOS side if screen,
    sound, and buttons are needed.

## Floating Point

- Checked local binaries (`linuxrc`, `rkgame`, `cubevol`, `driver.so`): they are
  built as hard-float.
- Kernel strings contain signs of FPU handling, including strings mentioning FPU
  revision.
- Conclusion: the firmware targets a hard-float ABI; hardware FPU is very likely,
  but final confirmation on hardware should be done with:
  - `cat /proc/cpuinfo`
  - `dmesg | grep -i fpu`
- After checking on the device, an FPU line was found in `dmesg`. Because of
  that, the WSL/GCC builds of `shell` and Pico-286 were switched from 74Kc to
  74Kf:
  `-march=74kf -mtune=74kf`.

## shell

Added a new native program named `shell`, without the `r36sx` prefix.

Sources and build:

```text
homebrew/shell/shell.c
homebrew/shell/build_shell_wsl.sh
homebrew/shell/README.md
```

Built/installed copies:

```text
homebrew/shell/shell
disk_image/MIPS_NATIVE/shell/shell
patches/disk_image_patch_shell/MIPS_NATIVE/shell/shell
```

Patch overlay:

```text
patches/disk_image_patch_shell/MANIFEST.md
patches/disk_image_patch_shell/MIPS_NATIVE/shell/README.md
patches/disk_image_patch_shell/MIPS_NATIVE/common/fonts/
```

What `shell` does:

- Opens `driver.so` and displays an RGB565 framebuffer through
  `video_driver_disp_frame`.
- Uses the shared on-screen keyboard from `homebrew/common`.
- Renders the terminal through FreeType and monospace fonts from
  `MIPS_NATIVE/common/fonts`.
- Starts a real `/bin/sh -i` through a pseudo-terminal.
- Allows normal Linux commands, pipes, and redirects, for example:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
cat /mnt/sdcard/root.txt
```

- Supports simple VT100/ANSI rendering: colors, cursor movement, clear
  screen/line, and printable UTF-8 through FreeType glyphs.
- If `/dev/ptmx`/`devpts` does not work on the firmware, `shell` now tries to
  create `/dev/ptmx`, mount `devpts` on `/dev/pts`, retry PTY startup, and then
  fall back to a pipe-backed shell. In fallback mode, commands and redirects
  work, but full TTY/job-control is unavailable.
- Built-in button controls:
  - `FN` - show/hide the on-screen keyboard.
  - `FN + D-pad Up` - save a framebuffer screenshot.
  - `FN + X` - exit.
  - `SELECT` - show the keyboard or expand/collapse it.
  - With the keyboard hidden, D-pad sends terminal arrow keys.
- Screenshots were added with the same key combination as Pico 286:
  `FN + D-pad Up`. In `shell` they are saved as 24-bit BMP files:
  `/mnt/sdcard/MIPS_NATIVE/shell/screenshots/shell_YYYYMMDD_HHMMSS_NNN.bmp`.
  If the SD path is unavailable, the local `screenshots` directory is used.

## Physical USB Keyboard

- The DTS contains two USB host controllers, `hichip,hc16xx-musb`, with
  `dr_mode = "host"`.
- `modules.builtin` and kernel strings contain:
  - `usbcore`
  - `musb`
  - `hid`
  - `hid-generic`
  - `usbhid`
  - `input-core`
  - `evdev`
- `mdev` is enabled through `/proc/sys/kernel/hotplug`, so a USB HID keyboard
  should appear as `/dev/input/event*` if the physical USB host port and power
  path work.
- An evdev bridge was added to `shell`:
  - periodically scans `/dev/input/event*`;
  - filters keyboard-like devices by `EV_KEY`, `KEY_A`, and `KEY_ENTER`;
  - maps physical USB keyboard keys into the `/bin/sh` PTY;
  - supports letters, digits, punctuation, Enter, Backspace, Tab, Esc, arrows,
    Home/End/Page, Insert/Delete, F1-F12, and Ctrl combinations.
- On hardware, check:

```sh
dmesg
ls /dev/input/event*
```

On successful connection, `shell` should print a line like:

```text
[usb keyboard: /dev/input/eventX]
```

If the keyboard is not detected, likely causes are:

- an OTG adapter is needed;
- the USB port does not provide 5V in the required mode;
- a powered USB hub is needed;
- the particular device port is not physically wired as host.

## Shared Component Changes

- `homebrew/common/r36sx_screen_keyboard.c` now includes paths to common
  monospace fonts:
  `MIPS_NATIVE/common/fonts/*.ttf`.
- This lets the on-screen keyboard use the same fonts as `shell`, `Tiny MC`,
  and other native programs.

## Current Working Copy State

At the time this document was created, the following changes were uncommitted:

```text
homebrew/common/r36sx_screen_keyboard.c
homebrew/shell/
patches/disk_image_patch_shell/
```

There were also unrelated/older untracked artifacts in the working copy that
this chat did not intentionally modify:

```text
disasm/
homebrew/pico_286/pico_286.hardfloat.gcc
```

Old directories/paths named `r36sx_shell` were renamed to `shell`.

## shell Update: Scrollback

- Added a 512-line ring scrollback buffer to `homebrew/shell/shell.c`.
- When the terminal scrolls, the top line is saved to history, and rendering can
  show either the live screen or older output.
- Device controls:
  - `FN + L` / `FN + R` - scroll history one line up/down.
  - `FN + D-pad Left` / `FN + D-pad Right` - scroll history by one page.
  - `FN + D-pad Down` - return to the current live output.
- Through the USB or on-screen keyboard:
  - `Shift+Up` / `Shift+Down` - scroll by one line.
  - `Shift+PageUp` / `Shift+PageDown` - scroll by one page.
  - `Ctrl+End` - return to the current output.
- Any shell input automatically returns the display to live output.
- `ESC[3J` clears scrollback, while a normal screen clear leaves history intact.

## shell Update: Quiet Fallback

- Visible PTY/devpts failure messages were removed:
  `posix_openpt failed`, `[trying /dev/pts setup]`, `mount devpts failed`,
  `[PTY unavailable...]`.
- The pipe fallback remains enabled so commands and redirects continue to work
  on firmware without a usable `/dev/ptmx`.
- The one-time BusyBox line `sh: can't access tty; job control turned off` is
  filtered from fallback-shell output, while normal command stderr is preserved.

## shell Rename

- The native program `shell` was renamed to lowercase, matching the other
  homebrew programs.
- Current paths:
  - `homebrew/shell/shell.c`
  - `homebrew/shell/build_shell_wsl.sh`
  - `disk_image/MIPS_NATIVE/shell/shell`
  - `patches/disk_image_patch_shell/MIPS_NATIVE/shell/shell`
- The screenshot directory is now:
  `/mnt/sdcard/MIPS_NATIVE/shell/screenshots/`.
- BMP screenshot names now start with `shell_`.

## shell Update: D-pad Scrollback

- With the on-screen keyboard hidden, D-pad `Up` / `Down` now behave like
  `Shift+Up` / `Shift+Down`: they scroll back one line.
- With the on-screen keyboard hidden, D-pad `Left` / `Right` still send normal
  terminal arrow keys to the shell.

## MTD Dump Script

- Added `homebrew/shell/dump_mtd.sh`.
- During `shell` installation, the script is copied to:
  - `disk_image/MIPS_NATIVE/shell/dump_mtd.sh`
  - `patches/disk_image_patch_shell/MIPS_NATIVE/shell/dump_mtd.sh`
- Run it on the device with:

```sh
/mnt/sdcard/MIPS_NATIVE/shell/dump_mtd.sh
```

- By default, it saves all partitions from `/proc/mtd` under
  `/mnt/sdcard/mtd-dump/YYYYMMDD_HHMMSS/`, together with `proc_mtd.txt`,
  `mtd_map.tsv`, `dump.log`, and a checksum file.
