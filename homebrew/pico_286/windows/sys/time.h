#ifndef R36SX_WINDOWS_SYS_TIME_H
#define R36SX_WINDOWS_SYS_TIME_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdint.h>
#include <time.h>
#include <windows.h>

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif

static int gettimeofday(struct timeval *tv, void *tz)
{
    FILETIME ft;
    uint64_t ticks;
    (void)tz;

    if (!tv) {
        return -1;
    }

    GetSystemTimeAsFileTime(&ft);
    ticks = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    ticks -= 116444736000000000ull;
    tv->tv_sec = (long)(ticks / 10000000ull);
    tv->tv_usec = (long)((ticks % 10000000ull) / 10ull);
    return 0;
}

#endif
