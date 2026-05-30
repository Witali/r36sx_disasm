#ifndef R36SX_APP_STATS_H
#define R36SX_APP_STATS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t x86_per_sec;
    uint32_t disk_read_kb_per_sec;
    uint32_t disk_write_kb_per_sec;
    uint32_t fps;
} r36sx_app_stats_snapshot_t;

void r36sx_app_stats_init(void);
int r36sx_app_stats_is_enabled(void);
int r36sx_app_stats_is_visible(void);
void r36sx_app_stats_set_visible(int visible);
void r36sx_app_stats_toggle_visible(void);
void r36sx_app_stats_record_x86(uint32_t instructions);
void r36sx_app_stats_record_disk_read(size_t bytes);
void r36sx_app_stats_record_disk_write(size_t bytes);
void r36sx_app_stats_record_frame(void);
void r36sx_app_stats_snapshot(r36sx_app_stats_snapshot_t *snapshot);

#ifdef __cplusplus
}
#endif

#endif
