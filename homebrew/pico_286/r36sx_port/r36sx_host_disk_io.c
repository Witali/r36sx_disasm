#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "r36sx_host_disk_io.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "r36sx_disk_config.h"
#include "r36sx_pico286_compat.h"

static uint32_t r36sx_host_disk_now_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)((uint64_t)ts.tv_sec * 1000ull +
                      (uint64_t)ts.tv_nsec / 1000000ull);
}

void r36sx_host_disk_cache_init(r36sx_host_disk_cache_t *cache)
{
    if (cache) {
        memset(cache, 0, sizeof(*cache));
    }
}

FILE *r36sx_host_disk_open(const char *path, r36sx_host_disk_cache_t *cache,
                           size_t *file_size)
{
    FILE *file;
    uint32_t buffer_bytes;

    if (cache) {
        r36sx_host_disk_cache_init(cache);
    }
    if (file_size) {
        *file_size = 0;
    }

    file = fopen(path, "rb+");
    if (!file) {
        return NULL;
    }

    buffer_bytes = r36sx_pico286_disk_cache_buffer_bytes();
    if (cache && buffer_bytes > 0) {
        cache->stdio_buffer = (uint8_t *)malloc(buffer_bytes);
        if (cache->stdio_buffer) {
            if (setvbuf(file, (char *)cache->stdio_buffer, _IOFBF,
                        buffer_bytes) == 0) {
                cache->stdio_buffer_size = buffer_bytes;
                r36sx_pico286_debug_log(
                    "hostdisk: setvbuf path='%s' bytes=%lu",
                    path, (unsigned long)buffer_bytes);
            } else {
                r36sx_pico286_debug_log(
                    "hostdisk: setvbuf failed path='%s' bytes=%lu",
                    path, (unsigned long)buffer_bytes);
                free(cache->stdio_buffer);
                cache->stdio_buffer = NULL;
            }
        } else {
            r36sx_pico286_debug_log(
                "hostdisk: buffer malloc failed path='%s' bytes=%lu",
                path, (unsigned long)buffer_bytes);
        }
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        r36sx_host_disk_close(&file, cache, 0xffu);
        return NULL;
    }
    if (file_size) {
        long size = ftell(file);
        if (size < 0) {
            r36sx_host_disk_close(&file, cache, 0xffu);
            return NULL;
        }
        *file_size = (size_t)size;
    }
    rewind(file);
    return file;
}

int r36sx_host_disk_flush(FILE *file, r36sx_host_disk_cache_t *cache,
                          uint8_t drive, const char *reason)
{
    if (!file || !cache || !cache->dirty) {
        return 0;
    }

    if (fflush(file) != 0) {
        r36sx_pico286_debug_log("hostdisk: fflush failed drive=%u reason=%s",
                                drive, reason ? reason : "?");
        return -1;
    }

    r36sx_pico286_debug_log(
        "hostdisk: fflush drive=%u sectors=%lu reason=%s",
        drive, (unsigned long)cache->dirty_sectors,
        reason ? reason : "?");
    cache->dirty = 0;
    cache->dirty_sectors = 0;
    cache->last_write_ms = 0;
    return 0;
}

void r36sx_host_disk_close(FILE **file, r36sx_host_disk_cache_t *cache,
                           uint8_t drive)
{
    if (file && *file) {
        r36sx_host_disk_flush(*file, cache, drive, "close");
        fclose(*file);
        *file = NULL;
    }
    if (cache) {
        free(cache->stdio_buffer);
        r36sx_host_disk_cache_init(cache);
    }
}

int r36sx_host_disk_read_at(FILE *file, size_t offset, void *dst,
                            size_t bytes)
{
    if (!file || !dst) {
        return -1;
    }
    if (fseek(file, (long)offset, SEEK_SET) != 0) {
        return -1;
    }
    return fread(dst, 1, bytes, file) == bytes ? 0 : -1;
}

int r36sx_host_disk_write_at(FILE *file, r36sx_host_disk_cache_t *cache,
                             uint8_t drive, size_t offset, const void *src,
                             size_t bytes, uint32_t dirty_sectors)
{
    uint32_t flush_sectors;

    if (!file || !src || !cache) {
        return -1;
    }
    if (fseek(file, (long)offset, SEEK_SET) != 0) {
        return -1;
    }
    if (fwrite(src, 1, bytes, file) != bytes) {
        return -1;
    }

    cache->dirty = 1;
    cache->dirty_sectors += dirty_sectors;
    cache->last_write_ms = r36sx_host_disk_now_ms();
    flush_sectors = r36sx_pico286_disk_cache_flush_sectors();
    if (flush_sectors == 0 || cache->dirty_sectors >= flush_sectors) {
        return r36sx_host_disk_flush(file, cache, drive, "sector-threshold");
    }
    return 0;
}

int r36sx_host_disk_flush_due(FILE *file, r36sx_host_disk_cache_t *cache,
                              uint8_t drive)
{
    uint32_t elapsed;
    uint32_t flush_ms;

    if (!file || !cache || !cache->dirty || cache->last_write_ms == 0) {
        return 0;
    }

    flush_ms = r36sx_pico286_disk_cache_flush_ms();
    elapsed = r36sx_host_disk_now_ms() - cache->last_write_ms;
    if (elapsed >= flush_ms) {
        return r36sx_host_disk_flush(file, cache, drive, "timeout");
    }
    return 0;
}
