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
