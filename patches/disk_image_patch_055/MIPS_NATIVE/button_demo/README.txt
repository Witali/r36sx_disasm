Native Button Demo
------------------

Standalone MIPS Button Demo executable for Tiny MC.

Main executable:

  button_demo

Controls:

- D-pad: move the square.
- A, B, X, Y: change square color.
- L, R, L2, R2, L3, R3: logged when pressed.
- Start: toggle checkerboard background.
- Select: reset square position.
- Fn: exit back to Tiny MC.

The lower part of the screen shows a scrolling log of newly pressed buttons.
The log uses FreeType with the stock firmware Arial font:

  /mnt/sdcard/cubegm/Arial_en.ttf

If FreeType or Arial cannot be opened, the program falls back to its built-in
bitmap font.

This native build uses /mnt/sdcard/cubegm/driver.so for display, controls, and
audio. Button sounds are generated locally and sent through
sound_driver_playframe, matching the stock rkgame LibRetro audio path.
