#!/bin/sh

LOG_FILE=/mnt/sdcard/cubegm/boot_route.log
LOG_FALLBACK=/mnt/sdcard/boot_route.log

trace_log()
{
    TS="$(date 2>/dev/null)"
    [ -n "$TS" ] || TS="no-date"
    {
        echo "[$TS] script=$0 pid=$$ ppid=$PPID event=$1"
        echo "[$TS] script=$0 cwd=$(pwd 2>/dev/null)"
        echo "[$TS] script=$0 ps-begin"
        ps 2>/dev/null
        echo "[$TS] script=$0 ps-end"
    } >> "$LOG_FILE" 2>/dev/null || {
        echo "[$TS] script=$0 pid=$$ ppid=$PPID event=$1" >> "$LOG_FALLBACK" 2>/dev/null
    }
}

trace_log "start args=$*"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
trace_log "launching /mnt/sdcard/cubegm/icube LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
/mnt/sdcard/cubegm/icube &
ICUBE_PID=$!
trace_log "started /mnt/sdcard/cubegm/icube child=$ICUBE_PID"
trace_log "calling init -q"
init -q
trace_log "init -q returned rc=$?"
