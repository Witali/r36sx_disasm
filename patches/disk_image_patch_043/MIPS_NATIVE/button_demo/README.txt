Native Button Demo
------------------

Standalone MIPS Button Demo executable for Tiny MC.

Main executable:

  button_demo

Controls:

- D-pad: move the square.
- A, B, X, Y: change square color.
- Start: toggle checkerboard background.
- Select: reset square position.
- Fn: exit back to Tiny MC.

The lower part of the screen shows a scrolling log of newly pressed buttons.

This native build uses /mnt/sdcard/cubegm/driver.so for display and controls.
The old libretro Button Demo sound is not included here because Tiny MC child
processes do not receive libretro audio callbacks.
