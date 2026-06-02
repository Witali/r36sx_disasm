# disk_image_patch_Shell

This patch adds the native Shell framebuffer terminal.

Copy the contents of this directory over the root of the original SD card,
preserving directories.

## Files

```text
MIPS_NATIVE/Shell/Shell
MIPS_NATIVE/Shell/README.md
MIPS_NATIVE/common/fonts/
```

## Launch

Run:

```sh
/mnt/sdcard/MIPS_NATIVE/Shell/Shell
```

The frontend uses `driver.so` for the framebuffer, starts `/bin/sh -i` through
a pseudo terminal, accepts the shared on-screen keyboard, and also scans
`/dev/input/event*` for physical USB keyboards.

Hold `FN` and press D-pad `Up` to save a framebuffer screenshot, matching the
Pico-286 shortcut. Shell writes 24-bit BMP files under:

```text
/mnt/sdcard/MIPS_NATIVE/Shell/screenshots/
```

Shell redirection and pipes work normally, for example:

```sh
ls / > /mnt/sdcard/root.txt
dmesg | tail -40 > /mnt/sdcard/dmesg_tail.txt
```

## Notes

The program expects the stock firmware paths:

```text
/mnt/sdcard/cubegm/driver.so
/mnt/sdcard/cubegm/lib/libfreetype.so.6
/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf
```

The included common fonts are shared with the other native homebrew programs.
