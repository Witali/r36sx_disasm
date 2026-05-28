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

Press Fn in Tiny MC to close Tiny MC and start /mnt/sdcard/cubegm/icube.
The Fn shortcut is armed only after Tiny MC first observes Fn released, so a
startup Fn-high state does not immediately jump to icube.
