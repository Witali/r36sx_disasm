# Shell

Small Linux shell frontend for the stock R36SX firmware. It uses `driver.so`
for the RGB565 framebuffer, the shared R36SX on-screen keyboard, and a pseudo
terminal connected to `/bin/sh -i`. Physical USB keyboards are also accepted
through Linux evdev devices.

## Build

From Windows PowerShell:

```powershell
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/Shell/build_Shell_wsl.sh --strip --install"
```

The `--install` option writes:

```text
disk_image/MIPS_NATIVE/Shell/Shell
disk_image/MIPS_NATIVE/common/fonts/*.ttf
```

## Run on the device

```sh
/mnt/sdcard/MIPS_NATIVE/Shell/Shell
```

The terminal launches `/bin/sh -i`, so normal shell redirection and pipes work:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
cat /mnt/sdcard/root.txt
```

## Controls

Physical USB keyboards should appear as `/dev/input/event*` and work directly
after hotplug. The frontend scans for keyboard-like evdev devices every couple
of seconds and sends key presses to the shell PTY.

- `D-pad`: navigate the on-screen keyboard.
- `A` or `START`: press the selected key.
- `B`: Backspace.
- `X`: Esc while keyboard is visible, Tab while hidden.
- `Y`: Enter while keyboard is visible, Space while hidden.
- `SELECT`: expand/collapse the keyboard while visible, show it while hidden.
- `FN`: show/hide the keyboard.
- `FN + D-pad Up`: save a screenshot.
- `FN + START`: exit the shell frontend.

When the keyboard is hidden, the D-pad sends terminal arrow keys. If the child
shell exits, hide the keyboard and press `START` to launch a new `/bin/sh`.

## Notes

USB keyboard support depends on the stock kernel exposing the device through
`usbhid`/`hid-generic`/`evdev`. The extracted kernel has those pieces built in,
but the hardware port still needs host wiring and enough 5V power; use an OTG
adapter or a powered hub if the keyboard does not enumerate.

The renderer uses FreeType with the monospace fonts from
`MIPS_NATIVE/common/fonts`; `JetBrainsMonoNL-Regular.ttf` is the default. Set
`R36SX_SHELL_FONT=/path/to/font.ttf` before launching to override it. The
terminal keeps a fixed 80-column grid and supports a small VT100/ANSI subset
that is enough for BusyBox shell prompts, colors, cursor movement, clearing,
and ordinary command output. Full ncurses compatibility and double-width CJK
layout are not implemented.

`FN + D-pad Up` follows the Pico-286 shortcut and saves the current 640x480
framebuffer as a 24-bit BMP in:

```text
/mnt/sdcard/MIPS_NATIVE/Shell/screenshots/Shell_YYYYMMDD_HHMMSS_NNN.bmp
```

If that SD-card path cannot be created, the frontend falls back to a local
`screenshots` directory next to the current working directory.
