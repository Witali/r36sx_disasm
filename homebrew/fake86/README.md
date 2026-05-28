# Fake86 R36SX Port

This directory contains an experimental native R36SX build of Fake86, based on:

- Source: https://github.com/lgblgblgb/fake86
- Local source checkout: `internet_sources/fake86`
- Upstream license: GPL-2.0

The port avoids SDL2 as an external dependency. Instead, `SDL.h` and
`r36sx_sdl.c` provide the small SDL subset Fake86 needs and draw frames through
the console `driver.so`.

## Output Files

- `fake86` - MIPS executable for the console.
- `build_fake86.ps1` - local build script.
- `SDL.h` - minimal SDL compatibility header.
- `r36sx_sdl.c` - R36SX video/input/audio compatibility layer.
- `r36sx_fake86_compat.h` - compiler compatibility include forced into the
  upstream C files.

## Runtime Layout

The executable expects this layout on the SD card:

```text
/mnt/sdcard/MIPS_NATIVE/fake86/fake86
/mnt/sdcard/MIPS_NATIVE/fake86/data/asciivga.dat
/mnt/sdcard/MIPS_NATIVE/fake86/data/pcxtbios.bin
/mnt/sdcard/MIPS_NATIVE/fake86/data/rombasic.bin
/mnt/sdcard/MIPS_NATIVE/fake86/data/videorom.bin
```

When Fake86 is launched without arguments, the patched main function injects:

```text
fake86 -internalbios -boot rom -nosound -speed 4770000 -delay 16
```

This should boot into ROM BASIC using the internal BIOS/data files. Floppy or
hard-disk boot can be enabled later by passing upstream Fake86 arguments such
as `-fd0` or `-hd0`.

## Device Logs

The port writes logs to:

- `/mnt/sdcard/MIPS_NATIVE/fake86/fake86.log`
- `/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stdout.log`
- `/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stderr.log`

If the main path is unavailable, `fake86.log` falls back to `/mnt/sdcard/`.

## Controls

- D-pad: PC arrow keys
- A or Start: Enter
- B or Select: Escape
- X: Space
- Y: Ctrl
- L: Alt
- R: Shift
- L2: F1
- R2: F2
- Fn: F3
- Select + Start: quit

## Current Limitations

- Audio is disabled in the first build with `-nosound`.
- This is an experimental native port and still needs device-side testing.
- The current default launch path does not mount a floppy or HDD image.
