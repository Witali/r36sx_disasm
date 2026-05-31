#!/bin/sh

DEFAULT_TIME="2026-05-31 00:00:00"
TIME_VALUE=""

for stamp_file in /etc/last_time /mnt/sdcard/.last_time; do
    if [ -r "$stamp_file" ]; then
        read TIME_VALUE < "$stamp_file"
        [ -n "$TIME_VALUE" ] && break
    fi
done

case "$TIME_VALUE" in
    [0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]\ [0-9][0-9]:[0-9][0-9]:[0-9][0-9])
        ;;
    *)
        TIME_VALUE="$DEFAULT_TIME"
        ;;
esac

/bin/date -s "$TIME_VALUE" >/dev/null 2>&1

return 0 2>/dev/null || exit 0
