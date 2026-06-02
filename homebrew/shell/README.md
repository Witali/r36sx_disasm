# shell

Small Linux shell frontend for the stock R36SX firmware. It uses `driver.so`
for the RGB565 framebuffer, the shared R36SX on-screen keyboard, and a pseudo
terminal connected to `/bin/sh -i`. Physical USB keyboards are also accepted
through Linux evdev devices.

## Build

From Windows PowerShell:

```powershell
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/shell/build_shell_wsl.sh --strip --install"
```

The `--install` option writes:

```text
disk_image/MIPS_NATIVE/shell/shell
disk_image/MIPS_NATIVE/common/fonts/*.ttf
```

## Run on the device

```sh
/mnt/sdcard/MIPS_NATIVE/shell/shell
```

The terminal launches `/bin/sh -i`, so normal shell redirection and pipes work:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
cat /mnt/sdcard/root.txt
```

To save all MTD partitions to the SD card:

```sh
/mnt/sdcard/MIPS_NATIVE/shell/dump_mtd.sh
```

The optional first argument overrides the output root. By default, dumps are
written under `/mnt/sdcard/mtd-dump/YYYYMMDD_HHMMSS/` with `/proc/mtd`, a TSV
map, a log, and checksums.

If the firmware has no usable `/dev/ptmx`/`devpts`, the frontend quietly falls back to
a pipe-backed shell. Commands and redirection still work in that mode, but line
editing is local to the frontend and job-control features from a real TTY are
unavailable.

## Controls

Physical USB keyboards should appear as `/dev/input/event*` and work directly
after hotplug. The frontend scans for keyboard-like evdev devices every couple
of seconds and sends key presses to the shell PTY.

- `D-pad`: navigate the on-screen keyboard; while hidden, Up/Down browse scrollback.
- `A` or `START`: press the selected key.
- `B`: Backspace.
- `X`: Esc while keyboard is visible, Tab while hidden.
- `Y`: Enter while keyboard is visible, Space while hidden.
- `SELECT`: expand/collapse the keyboard while visible, show it while hidden.
- `FN`: show/hide the keyboard.
- `FN + D-pad Up`: save a screenshot.
- `FN + L` / `FN + R`: scroll console history up/down by one text line.
- `FN + D-pad Left` / `FN + D-pad Right`: scroll console history up/down by one page.
- `FN + D-pad Down`: return to live console output.
- `FN + X`: exit the shell frontend.

When the keyboard is hidden, D-pad Up/Down act like `Shift+Up`/`Shift+Down`
for scrollback, and D-pad Left/Right send terminal arrow keys. If the child
shell exits, hide the keyboard and press `START` to launch a new `/bin/sh`.
USB and on-screen keyboards can also browse history with `Shift+Up`,
`Shift+Down`, `Shift+PageUp`, and `Shift+PageDown`; `Ctrl+End` returns to the
live output.

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
ordinary command output, and 512 lines of scrollback history. Full ncurses
compatibility and double-width CJK layout are not implemented.

`FN + D-pad Up` follows the Pico-286 shortcut and saves the current 640x480
framebuffer as a 24-bit BMP in:

```text
/mnt/sdcard/MIPS_NATIVE/shell/screenshots/shell_YYYYMMDD_HHMMSS_NNN.bmp
```

If that SD-card path cannot be created, the frontend falls back to a local
`screenshots` directory next to the current working directory.
