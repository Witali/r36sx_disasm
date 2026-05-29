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
- Fn+X: exit back to TinyMC.
- Hold Fn for more than 3 seconds: emergency exit back to TinyMC.

## On-Screen Keyboard

- D-pad: move between keys.
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
- `SAVE/APPLY`: write current bindings to `pico_286.conf` and apply them.
- `EXIT APP`: exit Pico-286 so hard-disk changes can be seen after restart.

The menu searches for `.img` files next to `pico_286.conf`.

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
