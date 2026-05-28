# disk_image_patch_028

Purpose:

Create a dedicated root-level directory for standalone MIPS native programs
that are launched directly from Tiny MC.

Files:

```text
MIPS_NATIVE/README.txt
```

Layout convention:

```text
MIPS_NATIVE/
  program_name/
    program_name
    README.txt
    data/
```

Rules:

- Put each native program in its own directory.
- Name the directory after the program.
- Name the main executable after the directory when practical.
- Keep program data next to the executable, inside the same directory.
- Tiny MC runs executable files directly with `fork()` + `execl()`.
- Tiny MC runs files with a `.sh` suffix through `/bin/sh`.

Apply:

Copy this patch directory over the SD card/image root.

Repository change:

`homebrew/common/hardware.h` now defines:

```c
#define R36SX_MIPS_NATIVE_DIR "/mnt/sdcard/MIPS_NATIVE"
```

No binaries were changed in this patch.
