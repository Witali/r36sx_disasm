#!/bin/sh

killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
