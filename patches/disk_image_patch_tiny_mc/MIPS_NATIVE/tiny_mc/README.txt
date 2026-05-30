Tiny MC
-------

This directory contains the Tiny MC direct-launch executable.

Main executable:

  tiny_mc

The stock game transition script now starts Tiny MC directly:

  /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &

This bypasses cubegm/icube and its heartbeat watchdog. Tiny MC is built with
USE_ICUBE_HEARTBEAT=0 and starts in /mnt/sdcard/MIPS_NATIVE so each standalone
native program can live in its own subdirectory.

Tiny MC resolves driver.so sound_driver_init/sound_driver_playframe and plays
short button-click sounds through the same driver.so audio path used by rkgame
LibRetro cores.

Tiny MC also tries to load FreeType from:

  /mnt/sdcard/cubegm/lib/libfreetype.so.6

and uses this bundled monospaced font for smoother file-list text:

  /mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf

The font and size are configured next to the executable:

  /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc.conf

Useful keys are font, small_px, large_px, and list_row_h.

If FreeType or the TTF file is unavailable, Tiny MC falls back to its old
built-in 5x7 bitmap font.

Controls:

  Up/Down selects a file.
  A/Start opens directories, configured text files, or executables.
  Y views the selected regular file as text, regardless of extension.
  Left/B/Select moves to the parent directory.

Fn handling is enabled in Tiny MC. The shortcut is armed only after Fn has been
released once, so a raised Fn bit during startup is ignored. A later Fn press
closes Tiny MC and starts /mnt/sdcard/cubegm/icube directly, not icube.sh, to
avoid looping back into Tiny MC.
