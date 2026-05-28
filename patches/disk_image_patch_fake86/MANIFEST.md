# disk_image_patch_fake86

This patch adds an experimental native Fake86 build for the R36SX console.

Copy the contents of this directory over the root of the original SD card image.

## Files

```text
MIPS_NATIVE/fake86/fake86
MIPS_NATIVE/fake86/data/asciivga.dat
MIPS_NATIVE/fake86/data/pcxtbios.bin
MIPS_NATIVE/fake86/data/rombasic.bin
MIPS_NATIVE/fake86/data/videorom.bin
```

## Launch

Run `/mnt/sdcard/MIPS_NATIVE/fake86/fake86` from TinyMC.

When no arguments are provided, it starts with:

```text
-boot rom -nosound -speed 4770000 -delay 16
```

## Logs

Device logs are written to:

```text
/mnt/sdcard/MIPS_NATIVE/fake86/fake86.log
/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stdout.log
/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stderr.log
```

`fake86.log` also includes sampled frame diagnostics from `SDL_RenderPresent()`.
