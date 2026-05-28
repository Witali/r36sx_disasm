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
trace_log "before killall rkgame rkgame.stock tiny_mc icube"
killall rkgame rkgame.stock tiny_mc icube
trace_log "after killall rc=$?"
trace_log "launching /usr/bin/hcprojector"
/usr/bin/hcprojector &
HCPROJECTOR_PID=$!
trace_log "started /usr/bin/hcprojector child=$HCPROJECTOR_PID"
