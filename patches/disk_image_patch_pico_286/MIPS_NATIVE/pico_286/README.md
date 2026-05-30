# Pico-286 Native EXE Controls

This file documents the `pico_286` native executable for TinyMC.

## Main DOS Controls

- D-pad: PC cursor keys.
- A or Start: Enter.
- B: Escape.
- X: Space.
- Y: Ctrl.
- Select: Space.
- L: Alt.
- R: Shift.
- L2: F1.
- R2: F2.

## Fn Shortcuts

- Tap Fn: show or hide the on-screen keyboard.
- Fn+Select: open the disk image menu.
- Fn+Start: open the key preset editor.
- Fn+D-pad Up: save a 640x480 screenshot.
- Fn+B: soft-reset the emulated PC.
- Fn+X: exit back to TinyMC.
- Hold Fn for more than 3 seconds: emergency exit back to TinyMC.

Screenshots are written to `MIPS_NATIVE/pico_286/screenshots` as
`pico_286_YYYYMMDD_HHMMSS_NNN.png` or
`pico_286_YYYYMMDD_HHMMSS_NNN.bmp`, depending on `screenshot_format` in
`pico_286.conf`.  If that absolute SD-card path is not available, Pico-286
falls back to a local `screenshots` directory.

## On-Screen Keyboard

- D-pad: move between keys.
- Hold D-pad: repeat movement after a short delay.
- A or Start: type the highlighted key.
- B: Backspace.
- X: Escape.
- Y: Enter.
- Select: close the keyboard.

The on-screen Shift, Ctrl, and Alt keys act as latched modifiers.  The normal
DOS keyboard does not show the right-side cursor-key block; that block is only
shown by the key picker inside the preset editor.  The keyboard panel is drawn
edge-to-edge along the bottom of the screen, keeps the header/status text, and
uses a one-pixel inner gap around the key rows at the panel border.  A pressed
virtual key shifts down-right and darkens until the physical button is
released.  While the keyboard is visible, Pico-286 compresses only the active
height of the emulated video mode into the remaining screen area; normal DOS
text mode uses its 400 drawn rows rather than the full 480-pixel framebuffer.

## Disk Image Menu

Open it with Fn+Select.

- D-pad Up/Down: select a row.
- Left/Right: change the selected disk image.
- A or Y: accept/change the selected row.
- B or X: cancel and close the menu.
- `BOOT ORDER`: choose `A,C` or `C,A`.
- `SAVE/APPLY`: write current bindings to `pico_286.conf` and apply them.
- `EXIT APP`: exit Pico-286 so hard-disk changes can be seen after restart.

The menu searches for `.img` files next to `pico_286.conf`.

## Disk Image Cache

The host image I/O layer reads and writes contiguous BIOS sector transfers in
bulk when the DOS DMA buffer is ordinary RAM.  Each opened image also gets a
stdio buffer controlled by:

```text
[cpu]
cpu_mhz=32.768

[boot]
boot_mode=normal
boot_order=fdd0,hdd0

[screenshot]
screenshot_format=png

[disk_cache]
disk_cache_buffer_kb=64
disk_cache_flush_sectors=4
disk_cache_flush_ms=2000

[profiling]
profiling_enabled=0
profiling_log_ms=5000

[floppy_drives]
fdd0=FreeDOS1.img
fdd1=sopwith.img

[hard_drives]
hdd0=hdd.img
hdd0_geometry=65,16,63
hdd1=hdd2.img
hdd1_geometry=65,16,63
```

Dirty writes are flushed after 4 sectors, after 2 seconds without another
write, on INT 13h disk reset, when an image is changed/closed, and when the
application exits.

Set `profiling_enabled=1` to write periodic performance summaries to
`pico_286.log`.  `profiling_log_ms` controls the interval.  Profiling can be
compiled out with `build_pico_286.ps1 -DisableProfiling`.

Set `screenshot_format=png` for compressed screenshots or
`screenshot_format=bmp` for the old uncompressed 24-bit BMP output.

Normal DOS frames are now presented directly from the emulator `SCREEN` buffer.
The separate composition buffer is still used for the on-screen keyboard,
disk menu, and key preset editor.  The small disk activity LED is drawn into
`SCREEN` only for the present call, then its saved rectangle is restored.

## Key Preset Editor

Open it with Fn+Start.

- D-pad: select rows.
- Left/Right: change preset or column.
- A or Y: activate the selected row, accept OK, or accept the highlighted
  on-screen key in picker mode.
- B or X: cancel the editor without saving when no picker keyboard is open.
- In picker mode, B enters Backspace and X enters Escape.
- `RENAME`: edit the preset name with the on-screen keyboard.
- `OK`: save all draft changes to `keypresets.conf`.
- `CANCEL`: close without saving.

The default preset is stored in `keypresets.conf` next to the executable.
