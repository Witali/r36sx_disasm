/*
 * Forced-include compatibility header for compiling pico-286's Linux/host path
 * with Clang as a MIPS Linux native executable.
 */

#ifndef R36SX_PICO286_COMPAT_H
#define R36SX_PICO286_COMPAT_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <dirent.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#ifndef DEBUG
#define DEBUG 0
#endif

#define R36SX_PICO286_LOG_PATH "/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log"
#define R36SX_PICO286_FALLBACK_LOG_PATH "/mnt/sdcard/pico_286.log"

static inline void r36sx_pico286_debug_log(const char *format, ...)
{
#if DEBUG
    FILE *fp = fopen(R36SX_PICO286_LOG_PATH, "a");
    if (!fp) {
        fp = fopen(R36SX_PICO286_FALLBACK_LOG_PATH, "a");
    }
    if (fp) {
        struct timeval tv;
        va_list args;
        gettimeofday(&tv, NULL);
        fprintf(fp, "[%ld.%03ld] ", (long)tv.tv_sec, (long)(tv.tv_usec / 1000));
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
    unlink(R36SX_PICO286_LOG_PATH);
    unlink(R36SX_PICO286_FALLBACK_LOG_PATH);
    r36sx_pico286_debug_log("log reset");
#endif
}

/*
 * The host build uses normal arrays for RAM/EMS.  Some upstream branches still
 * mention Pico PSRAM/swap helpers even when constants make those branches dead.
 * Clang parses those branches strictly, so provide harmless internal stubs.
 */
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
