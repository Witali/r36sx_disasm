#ifndef R36SX_HOST_DISK_IO_H
#define R36SX_HOST_DISK_IO_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *stdio_buffer;
    size_t stdio_buffer_size;
    uint32_t dirty_sectors;
    uint32_t last_write_ms;
    uint8_t dirty;
} r36sx_host_disk_cache_t;

void r36sx_host_disk_cache_init(r36sx_host_disk_cache_t *cache);
FILE *r36sx_host_disk_open(const char *path, r36sx_host_disk_cache_t *cache,
                           size_t *file_size);
void r36sx_host_disk_close(FILE **file, r36sx_host_disk_cache_t *cache,
                           uint8_t drive);
int r36sx_host_disk_read_at(FILE *file, size_t offset, void *dst,
                            size_t bytes);
int r36sx_host_disk_write_at(FILE *file, r36sx_host_disk_cache_t *cache,
                             uint8_t drive, size_t offset, const void *src,
                             size_t bytes, uint32_t dirty_sectors);
int r36sx_host_disk_flush(FILE *file, r36sx_host_disk_cache_t *cache,
                          uint8_t drive, const char *reason);
int r36sx_host_disk_flush_due(FILE *file, r36sx_host_disk_cache_t *cache,
                              uint8_t drive);

#ifdef __cplusplus
}
#endif

#endif
