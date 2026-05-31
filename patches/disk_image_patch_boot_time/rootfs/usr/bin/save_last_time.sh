#!/bin/sh

NOW="$(/bin/date '+%Y-%m-%d %H:%M:%S' 2>/dev/null)"
[ -n "$NOW" ] || exit 0

echo "$NOW" > /etc/last_time 2>/dev/null

if [ -d /mnt/sdcard ]; then
    echo "$NOW" > /mnt/sdcard/.last_time 2>/dev/null
fi

sync
