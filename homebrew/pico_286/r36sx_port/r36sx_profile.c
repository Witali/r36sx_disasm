#include "r36sx_profile.h"

#include "r36sx_disk_config.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#if R36SX_ENABLE_PROFILING

#ifndef R36SX_PICO286_LOG_PATH
#define R36SX_PICO286_LOG_PATH "/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log"
#endif
#ifndef R36SX_PICO286_FALLBACK_LOG_PATH
#define R36SX_PICO286_FALLBACK_LOG_PATH "/mnt/sdcard/pico_286.log"
#endif

typedef struct {
    const char *name;
    uint64_t total_us;
    uint64_t max_us;
    uint64_t calls;
    uint64_t units;
} r36sx_profile_bucket_t;

static pthread_mutex_t profile_lock = PTHREAD_MUTEX_INITIALIZER;
static int profile_enabled;
static uint32_t profile_log_ms;
static uint64_t profile_last_log_us;
static r36sx_profile_bucket_t profile_buckets[R36SX_PROFILE_COUNT] = {
    [R36SX_PROFILE_EXEC86] = { "exec86", 0, 0, 0, 0 },
    [R36SX_PROFILE_DISK_FLUSH] = { "disk_flush_pending", 0, 0, 0, 0 },
    [R36SX_PROFILE_MFB_UPDATE] = { "mfb_update", 0, 0, 0, 0 },
    [R36SX_PROFILE_KEYBOARD_TICK] = { "keyboard_tick", 0, 0, 0, 0 },
    [R36SX_PROFILE_TIMER_IRQ] = { "timer_irq", 0, 0, 0, 0 },
    [R36SX_PROFILE_DSS_SAMPLE] = { "dss_sample", 0, 0, 0, 0 },
    [R36SX_PROFILE_SB_SAMPLE] = { "sb_sample", 0, 0, 0, 0 },
    [R36SX_PROFILE_AUDIO_SAMPLE] = { "audio_sample", 0, 0, 0, 0 },
    [R36SX_PROFILE_AUDIO_WRITE] = { "audio_write", 0, 0, 0, 0 },
    [R36SX_PROFILE_RENDERER] = { "renderer", 0, 0, 0, 0 },
    [R36SX_PROFILE_SOFT_RESET] = { "soft_reset", 0, 0, 0, 0 },
};

static void r36sx_profile_log(const char *format, ...)
{
    FILE *fp = fopen(R36SX_PICO286_LOG_PATH, "a");

    if (!fp) {
        fp = fopen(R36SX_PICO286_FALLBACK_LOG_PATH, "a");
    }
    if (fp) {
        struct timeval tv;
        va_list args;

        gettimeofday(&tv, NULL);
        fprintf(fp, "[%ld.%03ld] ", (long)tv.tv_sec,
                (long)(tv.tv_usec / 1000));
        va_start(args, format);
        vfprintf(fp, format, args);
        va_end(args);
        fputc('\n', fp);
        fclose(fp);
    }
}

uint64_t r36sx_profile_now_us(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull +
           (uint64_t)ts.tv_nsec / 1000ull;
}

void r36sx_profile_init(void)
{
    profile_enabled = r36sx_pico286_profiling_enabled();
    profile_log_ms = r36sx_pico286_profiling_log_ms();
    profile_last_log_us = r36sx_profile_now_us();

    if (profile_enabled) {
        r36sx_profile_log(
            "profile: enabled log_ms=%u compile=on",
            (unsigned int)profile_log_ms);
    }
}

int r36sx_profile_is_enabled(void)
{
    return profile_enabled;
}

void r36sx_profile_record(r36sx_profile_id_t id, uint64_t elapsed_us,
                          uint32_t units)
{
    r36sx_profile_bucket_t *bucket;

    if (!profile_enabled || id < 0 || id >= R36SX_PROFILE_COUNT) {
        return;
    }

    bucket = &profile_buckets[id];
    pthread_mutex_lock(&profile_lock);
    bucket->total_us += elapsed_us;
    if (elapsed_us > bucket->max_us) {
        bucket->max_us = elapsed_us;
    }
    bucket->calls++;
    bucket->units += units ? units : 1u;
    pthread_mutex_unlock(&profile_lock);
}

void r36sx_profile_maybe_log(void)
{
    r36sx_profile_bucket_t snapshot[R36SX_PROFILE_COUNT];
    uint64_t now_us;
    uint64_t elapsed_ms;

    if (!profile_enabled) {
        return;
    }

    now_us = r36sx_profile_now_us();
    elapsed_ms = (now_us - profile_last_log_us) / 1000ull;
    if (elapsed_ms < profile_log_ms) {
        return;
    }

    pthread_mutex_lock(&profile_lock);
    memcpy(snapshot, profile_buckets, sizeof(snapshot));
    for (int i = 0; i < R36SX_PROFILE_COUNT; i++) {
        profile_buckets[i].total_us = 0;
        profile_buckets[i].max_us = 0;
        profile_buckets[i].calls = 0;
        profile_buckets[i].units = 0;
    }
    profile_last_log_us = now_us;
    pthread_mutex_unlock(&profile_lock);

    r36sx_profile_log(
        "profile: interval_ms=%lu",
        (unsigned long)elapsed_ms);
    for (int i = 0; i < R36SX_PROFILE_COUNT; i++) {
        const r36sx_profile_bucket_t *bucket = &snapshot[i];
        uint64_t avg_us;
        uint64_t unit_avg_us;

        if (bucket->calls == 0) {
            continue;
        }

        avg_us = bucket->total_us / bucket->calls;
        unit_avg_us = bucket->units ? bucket->total_us / bucket->units : 0;
        r36sx_profile_log(
            "profile: %-18s calls=%lu units=%lu total_ms=%lu avg_us=%lu unit_us=%lu max_us=%lu",
            bucket->name,
            (unsigned long)bucket->calls,
            (unsigned long)bucket->units,
            (unsigned long)(bucket->total_us / 1000ull),
            (unsigned long)avg_us,
            (unsigned long)unit_avg_us,
            (unsigned long)bucket->max_us);
    }
}

#else

uint64_t r36sx_profile_now_us(void)
{
    return 0;
}

void r36sx_profile_init(void)
{
}

int r36sx_profile_is_enabled(void)
{
    return 0;
}

void r36sx_profile_record(r36sx_profile_id_t id, uint64_t elapsed_us,
                          uint32_t units)
{
    (void)id;
    (void)elapsed_us;
    (void)units;
}

void r36sx_profile_maybe_log(void)
{
}

#endif
