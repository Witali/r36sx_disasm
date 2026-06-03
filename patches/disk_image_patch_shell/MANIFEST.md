# disk_image_patch_shell

This patch adds the native shell framebuffer terminal.

Copy the contents of this directory over the root of the original SD card,
preserving directories.

## Files

```text
MIPS_NATIVE/shell/shell
MIPS_NATIVE/shell/README.md
MIPS_NATIVE/shell/dump_mtd.sh
MIPS_NATIVE/common/fonts/
```

## Launch

Run:

```sh
/mnt/sdcard/MIPS_NATIVE/shell/shell
```

The frontend uses `driver.so` for the framebuffer, starts `/bin/sh -i` through
a pseudo terminal, accepts the shared on-screen keyboard, and also scans
`/dev/input/event*` for physical USB keyboards.

If `/dev/ptmx`/`devpts` is missing on the stock firmware, the frontend quietly uses a
pipe-backed fallback shell so commands and redirection remain usable.

Hold `FN` and press D-pad `Up` to save a framebuffer screenshot, matching the
Pico-286 shortcut. The frontend writes 24-bit BMP files through the shared
`homebrew/common/r36sx_screenshot.*` helper under:

```text
/mnt/sdcard/MIPS_NATIVE/shell/screenshots/
```

Current WSL/GCC stripped binary:

- `MIPS_NATIVE/shell/shell`
- size: `58668` bytes
- SHA256:
  `EC78C89301F0070812D5BF2F70061A28DA55227D99A5EF82E178FA19193CFD59`

Console output keeps 512 lines of scrollback. Use `FN+L` / `FN+R` for one-line
history scrolling, `FN+D-pad Left` / `FN+D-pad Right` for page scrolling, and
`FN+D-pad Down` to return to live output. With the on-screen keyboard hidden,
D-pad `Up` / `Down` act like `Shift+Up` / `Shift+Down` and scroll one line.

shell redirection and pipes work normally, for example:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
```

To save all MTD partitions to the SD card:

```sh
/mnt/sdcard/MIPS_NATIVE/shell/dump_mtd.sh
```

The script writes `/proc/mtd`, a partition map, logs, checksums, and one
`mtdN_name.bin` file per readable MTD partition under
`/mnt/sdcard/mtd-dump/YYYYMMDD_HHMMSS/`.

## Notes

The program expects the stock firmware paths:

```text
/mnt/sdcard/cubegm/driver.so
/mnt/sdcard/cubegm/lib/libfreetype.so.6
/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf
```

The included common fonts are shared with the other native homebrew programs.
