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
Any newly pressed button also plays a short generated sound through /dev/auddec.

This native build uses /mnt/sdcard/cubegm/driver.so for display and controls.
Audio follows the same /dev/auddec packet model used by stock hcprojector UI
WAV effects, but generates PCM internally and does not require external WAV
files.
