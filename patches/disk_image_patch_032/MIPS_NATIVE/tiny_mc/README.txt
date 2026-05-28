Tiny MC
-------

This directory contains the canonical Tiny MC native launcher copy.

Main executable:

  tiny_mc

The stock game transition script still starts icube:

  /mnt/sdcard/cubegm/icube &

icube hardcodes /mnt/sdcard/cubegm/rkgame, so the same Tiny MC executable is
also installed as cubegm/rkgame for compatibility. When launched with no
arguments, Tiny MC starts in /mnt/sdcard/MIPS_NATIVE so each standalone native
program can live in its own subdirectory.
