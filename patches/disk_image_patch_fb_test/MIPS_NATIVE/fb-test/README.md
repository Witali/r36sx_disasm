# fb-test

`fb-test` is a small native MIPS diagnostic program for the stock R36SX Linux
firmware. It intentionally does not use `driver.so`; it repeats the safe,
observable parts of the framebuffer path found in the `driver.so` decompilation.

The program:

- probes `/dev/dis` with ioctl `0xc00c0e0c`;
- probes `/dev/ge` by opening it, mapping registers, and querying command queue
  metadata with ioctl `0x20002305`;
- opens `/dev/fb0`;
- reads standard Linux framebuffer info with `FBIOGET_FSCREENINFO` and
  `FBIOGET_VSCREENINFO`;
- maps `/dev/fb0` memory;
- draws a RGB test pattern directly into the framebuffer;
- pans/flips the display with `FBIOPAN_DISPLAY` when a second framebuffer page
  is available.

It does not submit GE commands yet. The command queue format is still
vendor-specific, so the GE probe is read-only from the point of view of the
command stream.

## Build

From the repository root:

```sh
wsl bash -lc "cd /mnt/c/Work/r36sx_disasm && ./homebrew/fb-test/build_fb_test_wsl.sh --strip --install"
```

Installed paths:

```text
disk_image/MIPS_NATIVE/fb-test/fb-test
patches/disk_image_patch_fb_test/MIPS_NATIVE/fb-test/fb-test
```

## Run

On the device:

```sh
/mnt/sdcard/MIPS_NATIVE/fb-test/fb-test
```

Useful options:

```text
--hold SECONDS  keep the test pattern visible before exit
--no-ge         skip /dev/ge probing
--no-pan        draw into the currently visible framebuffer page only
--restore       pan back to the previous page before exit
```

Example:

```sh
/mnt/sdcard/MIPS_NATIVE/fb-test/fb-test --hold 20 --restore
```

## Expected Result

If direct framebuffer writes work, the LCD should show a bordered color-bar and
gradient test pattern. If the pattern is wrong, rotated, invisible, or only
appears after using `driver.so`, that tells us which parts of the vendor display
pipeline still need to be replicated: GE blit/scale/rotate, framebuffer page
setup, or vendor display commit ioctls.

