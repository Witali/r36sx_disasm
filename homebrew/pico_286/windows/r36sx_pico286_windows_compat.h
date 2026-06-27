/*
 * Forced-include compatibility header for the Pico-286 Windows debug host.
 *
 * The device build uses Linux/MIPS paths and driver.so.  This header keeps the
 * emulator core source shared while redirecting logs/configs to files beside
 * pico_286_win.exe and providing tiny Win32 replacements for POSIX helpers.
 */

#ifndef R36SX_PICO286_WINDOWS_COMPAT_H
#define R36SX_PICO286_WINDOWS_COMPAT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <stdarg.h>
#include <direct.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <windows.h>

#define mkdir(path, mode) _mkdir(path)

static inline struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_s(result, timep) == 0 ? result : NULL;
}

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef SIGBUS
#define SIGBUS SIGSEGV
#endif

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1
#endif
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

#if !defined(_TIMESPEC_DEFINED) && !defined(_STRUCT_TIMESPEC)
#define _TIMESPEC_DEFINED
struct timespec {
    long tv_sec;
    long tv_nsec;
};
#endif

static inline int r36sx_windows_clock_gettime(int clock_id,
                                              struct timespec *ts)
{
    if (!ts) {
        return -1;
    }

    if (clock_id == CLOCK_REALTIME) {
        FILETIME ft;
        uint64_t ticks;
        GetSystemTimeAsFileTime(&ft);
        ticks = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
        ticks -= 116444736000000000ull;
        ts->tv_sec = (long)(ticks / 10000000ull);
        ts->tv_nsec = (long)((ticks % 10000000ull) * 100ull);
        return 0;
    }

    static LARGE_INTEGER freq;
    LARGE_INTEGER now;
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&now);
    ts->tv_sec = (long)(now.QuadPart / freq.QuadPart);
    ts->tv_nsec =
        (long)(((now.QuadPart % freq.QuadPart) * 1000000000ull) /
               (uint64_t)freq.QuadPart);
    return 0;
}

#define clock_gettime r36sx_windows_clock_gettime

#define R36SX_PICO286_LOG_PATH "pico_286.log"
#define R36SX_PICO286_FALLBACK_LOG_PATH "pico_286_fallback.log"
#define R36SX_PICO286_MAX_LOG_BYTES (2u * 1024u * 1024u)
#define R36SX_PICO286_HAS_LOG_OPEN_HELPER 1

#include "r36sx_debug_config.h"
#include "r36sx_disk_config.h"

static inline FILE *r36sx_pico286_open_log_for_append(void)
{
    uint32_t max_log_bytes =
        r36sx_pico286_log_max_bytes(R36SX_PICO286_MAX_LOG_BYTES);
    char log_path[512];
    char fallback_log_path[512];
    const char *paths[] = {
        log_path,
        fallback_log_path,
    };

    r36sx_pico286_ensure_diagnostics_dir();
    r36sx_pico286_resolve_diagnostics_path(
        log_path, sizeof(log_path), R36SX_PICO286_LOG_PATH);
    r36sx_pico286_resolve_diagnostics_path(
        fallback_log_path, sizeof(fallback_log_path),
        R36SX_PICO286_FALLBACK_LOG_PATH);

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i) {
        struct stat st;

        if (max_log_bytes > 0 &&
            stat(paths[i], &st) == 0 &&
            st.st_size >= (long)max_log_bytes) {
            return NULL;
        }

        FILE *fp = fopen(paths[i], "a");
        if (fp) {
            return fp;
        }
    }

    return NULL;
}

static inline void r36sx_pico286_truncate_log_file(const char *path)
{
    FILE *fp = fopen(path, "w");
    if (fp) {
        fclose(fp);
    }
}

static inline void r36sx_pico286_debug_log(const char *format, ...)
{
#if DEBUG
    FILE *fp = r36sx_pico286_open_log_for_append();
    if (fp) {
        struct timespec ts;
        va_list args;
        clock_gettime(CLOCK_REALTIME, &ts);
        fprintf(fp, "[%lld.%03ld] ", (long long)ts.tv_sec,
                ts.tv_nsec / 1000000L);
        va_start(args, format);
        vfprintf(fp, format, args);
        va_end(args);
        fputc('\n', fp);
        fclose(fp);
    }
#else
    (void)format;
#endif
}

static inline void r36sx_pico286_debug_reset(void)
{
#if DEBUG
    if (r36sx_pico286_log_truncate_on_start()) {
        char log_path[512];
        char fallback_log_path[512];
        r36sx_pico286_ensure_diagnostics_dir();
        r36sx_pico286_resolve_diagnostics_path(
            log_path, sizeof(log_path), R36SX_PICO286_LOG_PATH);
        r36sx_pico286_resolve_diagnostics_path(
            fallback_log_path, sizeof(fallback_log_path),
            R36SX_PICO286_FALLBACK_LOG_PATH);
        r36sx_pico286_truncate_log_file(log_path);
        r36sx_pico286_truncate_log_file(fallback_log_path);
    }
    r36sx_pico286_debug_log("log start");
#endif
}

static inline void r36sx_pico286_debug_log_build_info(void)
{
#if R36SX_DEBUG_BUILD_INFO
    r36sx_pico286_debug_log(
        "build: git_commit=%s short=%s commit_object_sha256=%s dirty=%d host=windows",
        R36SX_BUILD_GIT_COMMIT,
        R36SX_BUILD_GIT_COMMIT_SHORT,
        R36SX_BUILD_COMMIT_OBJECT_SHA256,
        R36SX_BUILD_GIT_DIRTY);
#endif
}

static inline uint8_t read8psram(uint32_t address)
{
    (void)address;
    return 0;
}

static inline uint16_t read16psram(uint32_t address)
{
    (void)address;
    return 0;
}

static inline uint32_t read32psram(uint32_t address)
{
    (void)address;
    return 0;
}

static inline void write8psram(uint32_t address, uint8_t value)
{
    (void)address;
    (void)value;
}

static inline void write16psram(uint32_t address, uint16_t value)
{
    (void)address;
    (void)value;
}

static inline void write32psram(uint32_t address, uint32_t value)
{
    (void)address;
    (void)value;
}

static inline uint8_t swap_read(uint32_t address)
{
    (void)address;
    return 0;
}

static inline uint16_t swap_read16(uint32_t address)
{
    (void)address;
    return 0;
}

static inline uint32_t swap_read32(uint32_t address)
{
    (void)address;
    return 0;
}

static inline void swap_write(uint32_t address, uint8_t value)
{
    (void)address;
    (void)value;
}

static inline void swap_write16(uint32_t address, uint16_t value)
{
    (void)address;
    (void)value;
}

static inline void swap_write32(uint32_t address, uint32_t value)
{
    (void)address;
    (void)value;
}

#endif
