# Pico-286 Native EXE Controls

This file documents the `pico_286` native executable for TinyMC.

## Main DOS Controls

The default key preset maps physical buttons like this:

- D-pad: PC cursor keys.
- A or Start: Enter.
- B: Escape.
- X: Space.
- Y: Ctrl.
- Select: Shift.
- L: Alt.
- R: Shift.
- L2: F1.
- R2: F2.

These mappings can be changed in the key preset editor.  There is no
Select+Start exit shortcut; use Fn+X or the long Fn hold described below.
Held mapped keys auto-repeat after a short delay, except modifier keys such as
Shift, Ctrl, and Alt.

## Fn Shortcuts

- Tap Fn: show or hide the on-screen keyboard.
- Fn+Select: open the disk image menu.
- Fn+Start: open the key preset editor.
- Fn+D-pad Left: show or hide the Fn shortcut help overlay.
- Fn+D-pad Up: save a 640x480 screenshot, play a shutter click, and briefly show a preview.
- Fn+D-pad Down: show or hide app statistics.
- Fn+D-pad Right: show or hide the POST-code overlay.
- Fn+B: soft-reset the emulated PC.
- Fn+X: exit back to TinyMC.
- Hold Fn for more than 3 seconds: emergency exit back to TinyMC.

The active Fn shortcuts are exactly the ones listed above.  Fn+A, Fn+Y,
Fn+L, Fn+L2, Fn+R, and Fn+R2 currently have no action.

Screenshots are written to `MIPS_NATIVE/pico_286/screenshots` as
`pico_286_YYYYMMDD_HHMMSS_NNN.png` or
`pico_286_YYYYMMDD_HHMMSS_NNN.bmp`, depending on `screenshot_format` in
`pico_286.conf`.  If that absolute SD-card path is not available, Pico-286
falls back to a local `screenshots` directory.  After each capture, the screen
briefly shows `SCREENSHOT SAVED` or `SCREENSHOT FAILED` with a small preview.

## On-Screen Keyboard

- D-pad: move between keys.
- Hold D-pad: repeat movement after a short delay.
- L or R: switch between the normal keyboard and symbol keyboard.
- A or Start: type the highlighted key.
- B: Backspace.
- X: Escape.
- Y: Enter.
- Select: close the keyboard.

Holding A, Start, B, X, or Y repeats the emitted DOS key after a short delay.
The on-screen Shift, Ctrl, and Alt keys act as latched modifiers.  The normal
DOS keyboard does not show the right-side cursor-key block; that block is only
shown by the key picker inside the preset editor.  The keyboard panel is drawn
edge-to-edge along the bottom of the screen, keeps the header/status text, and
uses a one-pixel inner gap around the key rows at the panel border.  A pressed
virtual key shifts down-right and darkens until the physical button is
released.  While the keyboard is visible, Pico-286 compresses only the active
height of the emulated video mode into the remaining screen area; normal DOS
text mode uses its 400 drawn rows rather than the full 480-pixel framebuffer.
Set `[video] keyboard_mode=overlay` to draw the keyboard over the DOS image
without resizing it; `keyboard_mode=normal` keeps the compressed layout.
Overlay mode caches the keyboard panel and refreshes it only when the visible
keyboard state changes.

## Disk Image Menu

Open it with Fn+Select.

- D-pad Up/Down: select a row.
- Left/Right: change the selected disk image, boot order, or BIOS mode.
- A or Y on a drive row: select the next disk image.
- A or Y on `BOOT ORDER`: switch between `A,C` and `C,A`.
- A or Y on `BIOS`: switch between the normal embedded BIOS and `TEST386`.
- A or Y on `SAVE/APPLY`: write current bindings to `pico_286.conf` and apply them.
- A or Y on `EXIT APP`: exit Pico-286 so hard-disk changes can be seen after restart.
- A or Y on `CANCEL`: close the menu.
- B or X: cancel and close the menu.

The menu searches the configured `image_dir` by drive type.  Floppy rows accept
`.img`, `.ima`, `.flp`, `.fdd`, `.vfd`, and `.dsk`; hard-disk rows accept
`.hdd`, `.hd`, `.hdi`, and `.raw`.  If the BIOS mode changes, `SAVE/APPLY`
also requests a soft reset so the selected ROM starts.

## Disk Image Cache

The host image I/O layer reads and writes contiguous BIOS sector transfers in
bulk when the DOS DMA buffer is ordinary RAM.  Each opened image also gets a
stdio buffer controlled by:

```text
[cpu]
cpu_model=80386
cpu_mode=real
cpu_mhz=32.768

[timing]
target_fps=60

[video]
scaling_filter=nearest
keyboard_mode=normal

[boot]
boot_mode=normal
boot_order=fdd0,hdd0

[bios]
bios=normal
test_bios_rom=test386.bin

[memory]
conventional_kb=640
upper_kb=176
extended_kb=64
xms_kb=4096

[screenshot]
screenshot_format=png

[stats]
app_stats_enabled=1

[host_drive]
host_drive_path=host

[disk_images]
image_dir=images

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
hdd0=hdd.hdd
hdd0_geometry=65,16,63
hdd1=hdd2.hdd
hdd1_geometry=65,16,63
```

`cpu_model` can be `8086`, `80286`, or `80386`.  The default is `80386`.
Instructions above the selected model raise `INT 6`; 386-only real-mode
features such as `66h`/`67h`, `FS`/`GS`, `PUSHFD`/`POPFD`, `PUSHAD`/`POPAD`,
32-bit `MOV`/ALU/stack/shift forms, `MOVZX`/`MOVSX`, `SETcc`, near `Jcc`,
`BSF`/`BSR`, and `IMUL` require `cpu_model=80386`.  Protected mode is
experimental: guest code can enter through `LMSW`/`MOV CR0`, load GDT/IDT with
`LGDT`/`LIDT`, reload descriptors through far control transfers, and use basic
protected interrupt gates.  Paging, privilege checks, task switching, call
gates, and full 32-bit `EIP` execution are still incomplete, so keep
`cpu_mode=real` for normal DOS use.

`target_fps` controls the main-loop frame budget.  The default `60` targets
about 16.7 ms per `exec86()` pass.  Pico-286 automatically reduces the
`exec86()` quantum when emulation itself overruns, grows it back when there is
spare CPU time, caps it at the `cpu_mhz` limit, and never drops below 1000
instructions per frame.  Rendering overlays such as the on-screen keyboard do
not reduce the CPU quantum.  The same target also sets the nominal audio packet
cadence.  At 60 Hz this is about 735 stereo frames at 44.1 kHz, but delayed
packets are sized from the actual elapsed time since the previous audio send,
up to the 100 ms source buffer capacity.  Block boundaries are smoothed with a
short de-click ramp.

Minimal banked VBE modes `103h` (800x600x8 packed pixel) and `114h`
(800x600x16 RGB565) are supported.  The banked VGA window is `A000:0000`, and
the 800x600 image is downsampled to the native 640x480 screen.  Linear
framebuffer requests are rejected.

Dirty writes are flushed after 4 sectors, after 2 seconds without another
write, on INT 13h disk reset, when an image is changed/closed, and when the
application exits.

`cpu_tests.img` is an optional FAT12 floppy image stored under `image_dir`.
It contains PCjs `ID.COM` and `TEST386.COM`; boot FreeDOS, mount it as `B:`,
then run `ID` or `TEST386`.  It also contains `TEST386.BIN`, the R36SX debug
build of `barotto/test386.asm`.  That file is a 64 KB BIOS replacement ROM,
not a DOS `.COM` program; Pico-286 currently stores it as a payload/reference
and logs its configured debug ports `80h`/`191h` when it is loaded as a ROM.
The disk image menu can switch the executable BIOS setting between `NORMAL`
and `TEST386`; the test ROM file is `test386.bin` next to `pico_286`.

The `[memory]` values are in KB.  `conventional_kb` is reported through the
BIOS Data Area, `extended_kb` is returned by `INT 15h AH=88h`, `upper_kb`
limits XMS UMB allocations from `D000:0000` upward, and `xms_kb` limits the
built-in XMS handler.

Set `profiling_enabled=1` to write periodic performance summaries to
`pico_286.log`.  `profiling_log_ms` controls the interval.  Profiling can be
compiled out with `build_pico_286.ps1 -DisableProfiling`.

Set `screenshot_format=png` for compressed screenshots or
`screenshot_format=bmp` for the old uncompressed 24-bit BMP output.

Set `app_stats_enabled=1` to allow the `Fn` + D-pad `Down` statistics overlay.
It shows a lower-right two-column table above the disk LED with decoded x86
instruction loops in K/s, host disk image read/write KB/s, and presented FPS.
Set it to `0` to disable the shortcut and overlay.

`Fn` + D-pad `Right` toggles a compact POST-code overlay.  Pico-286 captures
standard BIOS POST writes to port `80h` and the legacy R36SX test386 POST port
`190h`.  The rebuilt `test386.bin` writes POST codes to the standard `80h`
port.  The embedded Turbo XT BIOS image has no obvious `OUT 80h` POST sequence,
so the normal BIOS may not produce values.

Set `host_drive_path` to the directory exposed to DOS as network drive `H:`.
Relative paths are resolved next to `pico_286.conf`; the default `host` maps
to `MIPS_NATIVE/pico_286/host` on the SD card.  DOS must run `MAPDRIVE.COM`
after boot, and `CONFIG.SYS` needs `LASTDRIVE=H` or higher.

Normal DOS frames are now presented directly from the emulator `SCREEN` buffer.
Full-screen menus such as the disk menu and key preset editor draw directly into
the output frame and skip DOS-frame composition underneath them.
`keyboard_mode=normal` resizes the DOS image above the on-screen keyboard;
`keyboard_mode=overlay` keeps the DOS image unscaled and draws the keyboard over
it from a cached keyboard buffer.  Small overlays such as app statistics, the
POST-code box, disk LED, and overlay keyboard save only their covered
rectangles, draw into `SCREEN`, and restore those rectangles at the next
`mfb_update()` unless the DOS renderer already produced a dirty frame.

This build also enables the computed-goto CPU opcode dispatcher, so the main
`exec86()` loop jumps directly to opcode handlers instead of entering the large
switch decoder on every instruction.

## Key Preset Editor

Open it with Fn+Start.

- D-pad: select rows.
- Left/Right: change preset or column.
- A or Y on `PRESET`: select the next preset.
- A or Y on `RENAME`: edit the preset name with the on-screen keyboard.
- A or Y on `ADD NEW`: create a new preset by copying the current one.
- A or Y on a button row: open the key picker for that physical button.
- A or Y in key picker mode: assign the highlighted on-screen key.
- B in key picker mode: assign Backspace.
- X in key picker mode: assign Escape.
- B or X outside picker mode: cancel the editor without saving.
- `OK`: save all draft changes to `keypresets.conf`.
- `CANCEL`: close without saving.

The default preset is stored in `keypresets.conf` next to the executable.
