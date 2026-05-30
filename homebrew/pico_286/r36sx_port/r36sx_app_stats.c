#include "r36sx_app_stats.h"

#include <string.h>
#include <time.h>

#include "r36sx_disk_config.h"

static int stats_enabled = 1;
static int stats_visible = 0;
static uint64_t stats_last_sample_us = 0;
static uint64_t stats_x86_count = 0;
static uint64_t stats_disk_read_bytes = 0;
static uint64_t stats_disk_write_bytes = 0;
static uint64_t stats_frames = 0;
static r36sx_app_stats_snapshot_t stats_snapshot;

static uint64_t stats_now_us(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull +
           (uint64_t)ts.tv_nsec / 1000ull;
}

static uint32_t rate_per_sec(uint64_t units, uint64_t elapsed_us)
{
    if (elapsed_us == 0) {
        return 0;
    }
    return (uint32_t)((units * 1000000ull) / elapsed_us);
}

void r36sx_app_stats_init(void)
{
    stats_enabled = r36sx_pico286_app_stats_enabled();
    stats_visible = 0;
    stats_last_sample_us = stats_now_us();
    stats_x86_count = 0;
    stats_disk_read_bytes = 0;
    stats_disk_write_bytes = 0;
    stats_frames = 0;
    memset(&stats_snapshot, 0, sizeof(stats_snapshot));
}

int r36sx_app_stats_is_enabled(void)
{
    return stats_enabled;
}

int r36sx_app_stats_is_visible(void)
{
    return stats_enabled && stats_visible;
}

void r36sx_app_stats_set_visible(int visible)
{
    if (!stats_enabled) {
        stats_visible = 0;
        return;
    }
    stats_visible = visible ? 1 : 0;
}

void r36sx_app_stats_toggle_visible(void)
{
    r36sx_app_stats_set_visible(!stats_visible);
}

void r36sx_app_stats_record_x86(uint32_t instructions)
{
    if (stats_enabled) {
        stats_x86_count += instructions;
    }
}

void r36sx_app_stats_record_disk_read(size_t bytes)
{
    if (stats_enabled) {
        stats_disk_read_bytes += (uint64_t)bytes;
    }
}

void r36sx_app_stats_record_disk_write(size_t bytes)
{
    if (stats_enabled) {
        stats_disk_write_bytes += (uint64_t)bytes;
    }
}

void r36sx_app_stats_record_frame(void)
{
    if (stats_enabled) {
        stats_frames++;
    }
}

void r36sx_app_stats_snapshot(r36sx_app_stats_snapshot_t *snapshot)
{
    uint64_t now_us;
    uint64_t elapsed_us;

    if (!snapshot) {
        return;
    }

    if (!stats_enabled) {
        memset(snapshot, 0, sizeof(*snapshot));
        return;
    }

    now_us = stats_now_us();
    elapsed_us = now_us - stats_last_sample_us;
    if (stats_last_sample_us == 0 || elapsed_us >= 1000000ull) {
        stats_snapshot.x86_per_sec =
            rate_per_sec(stats_x86_count, elapsed_us);
        stats_snapshot.disk_read_kb_per_sec =
            rate_per_sec(stats_disk_read_bytes, elapsed_us) / 1024u;
        stats_snapshot.disk_write_kb_per_sec =
            rate_per_sec(stats_disk_write_bytes, elapsed_us) / 1024u;
        stats_snapshot.fps = rate_per_sec(stats_frames, elapsed_us);

        stats_x86_count = 0;
        stats_disk_read_bytes = 0;
        stats_disk_write_bytes = 0;
        stats_frames = 0;
        stats_last_sample_us = now_us;
    }

    *snapshot = stats_snapshot;
}
