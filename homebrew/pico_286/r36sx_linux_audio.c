/*
 * linux-audio.h implementation for pico-286 on R36SX.
 *
 * Upstream Linux tries PulseAudio and OSS.  The console already has a proven
 * driver.so PCM path, so this shim keeps pico-286's audio API but routes
 * interleaved stereo int16 samples to sound_driver_playframe().
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linux-audio.h"
#include "../common/driver_audio.h"

struct r36sx_pico286_audio {
    void *handle;
    r36sx_cube_ioctl_fn cube_ioctl;
    struct r36sx_driver_audio_state driver;
    int16_t *mix_buffer;
    size_t mix_buffer_frames;
    uint32_t source_rate;
    uint32_t resample_ratio;
    uint32_t shutter_pos;
    uint32_t shutter_len;
    uint32_t shutter_noise;
    int initialized;
};

static struct r36sx_pico286_audio g_audio;
static pthread_mutex_t g_audio_lock = PTHREAD_MUTEX_INITIALIZER;

static void *r36sx_pico286_audio_open_driver(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        void *handle = dlopen(paths[i], RTLD_NOW);
        if (handle) {
            r36sx_pico286_debug_log("audio: dlopen ok path=%s handle=%p",
                                    paths[i], handle);
            return handle;
        }
        r36sx_pico286_debug_log("audio: dlopen failed path=%s err=%s",
                                paths[i], dlerror());
    }
    return NULL;
}

static int16_t r36sx_pico286_audio_clamp_i16(int32_t sample)
{
    if (sample > 32767) {
        return 32767;
    }
    if (sample < -32768) {
        return -32768;
    }
    return (int16_t)sample;
}

static int16_t r36sx_pico286_audio_next_shutter_sample(void)
{
    const uint32_t rate = R36SX_DRIVER_AUDIO_RATE;
    uint32_t pos = g_audio.shutter_pos++;
    uint32_t amp = 0;
    int32_t noise;

    if (pos >= g_audio.shutter_len) {
        return 0;
    }

    if (pos < rate / 180u) {
        amp = 18000u;
    } else if (pos < rate / 70u) {
        amp = 9000u;
    } else if (pos >= rate / 34u && pos < rate / 23u) {
        amp = 12000u;
    } else if (pos >= rate / 23u && pos < rate / 12u) {
        uint32_t tail = pos - rate / 23u;
        uint32_t tail_len = rate / 12u - rate / 23u;
        amp = 5000u - (4000u * tail) / tail_len;
    }

    g_audio.shutter_noise =
        g_audio.shutter_noise * 1664525u + 1013904223u;
    noise = (int32_t)((g_audio.shutter_noise >> 16) & 0xffffu) - 32768;
    return (int16_t)((noise * (int32_t)amp) / 32768);
}

void r36sx_pico286_audio_play_shutter(void)
{
    pthread_mutex_lock(&g_audio_lock);
    g_audio.shutter_pos = 0;
    g_audio.shutter_len = R36SX_DRIVER_AUDIO_RATE / 8u;
    g_audio.shutter_noise = 0x5eed1234u;
    pthread_mutex_unlock(&g_audio_lock);
}

int linux_audio_init(int sample_rate, int channels, int buffer_size)
{
    r36sx_pico286_debug_log("audio: init rate=%d channels=%d buffer=%d",
                            sample_rate, channels, buffer_size);
    linux_audio_close();
    if ((sample_rate != 22050 &&
         sample_rate != (int)R36SX_DRIVER_AUDIO_RATE) ||
        channels != (int)R36SX_DRIVER_AUDIO_CHANNELS ||
        (R36SX_DRIVER_AUDIO_RATE % (uint32_t)sample_rate) != 0u) {
        r36sx_pico286_debug_log("audio: unsupported format rate=%d channels=%d",
                                sample_rate, channels);
        return -1;
    }

    memset(&g_audio, 0, sizeof(g_audio));
    g_audio.source_rate = (uint32_t)sample_rate;
    g_audio.resample_ratio = R36SX_DRIVER_AUDIO_RATE / g_audio.source_rate;
    r36sx_driver_audio_init(&g_audio.driver);
    if (buffer_size > 0) {
        g_audio.mix_buffer_frames =
            (size_t)buffer_size * (size_t)g_audio.resample_ratio;
        g_audio.mix_buffer =
            (int16_t *)calloc(g_audio.mix_buffer_frames *
                                  R36SX_DRIVER_AUDIO_CHANNELS,
                              sizeof(g_audio.mix_buffer[0]));
        if (!g_audio.mix_buffer) {
            r36sx_pico286_debug_log("audio: mix buffer allocation failed");
            return -1;
        }
    }
    g_audio.handle = r36sx_pico286_audio_open_driver();
    if (!g_audio.handle) {
        r36sx_pico286_debug_log("audio: driver.so open failed");
        linux_audio_close();
        return -1;
    }

    g_audio.cube_ioctl =
        (r36sx_cube_ioctl_fn)dlsym(g_audio.handle, "cube_ioctl");
    r36sx_pico286_debug_log("audio: cube_ioctl=%p", g_audio.cube_ioctl);
    if (r36sx_driver_audio_bind_preserve_volume(&g_audio.driver,
                                                g_audio.handle,
                                                g_audio.cube_ioctl) != 0) {
        r36sx_pico286_debug_log("audio: driver bind failed");
        linux_audio_close();
        return -1;
    }
    g_audio.initialized = 1;
    r36sx_pico286_debug_log("audio: initialized source_rate=%u driver_rate=%u ratio=%u",
                            g_audio.source_rate, R36SX_DRIVER_AUDIO_RATE,
                            g_audio.resample_ratio);
    return 0;
}

int linux_audio_start(void)
{
    r36sx_pico286_debug_log("audio: start initialized=%d", g_audio.initialized);
    return g_audio.initialized ? 0 : -1;
}

int linux_audio_write(const int16_t *buffer, size_t samples)
{
    int rc;
    const int16_t *play_buffer = buffer;
    size_t play_frames = samples;
    static int logged_failures;

    if (!g_audio.initialized || !g_audio.driver.playframe || !buffer) {
        if (logged_failures < 8) {
            r36sx_pico286_debug_log("audio: write rejected initialized=%d play=%p buffer=%p",
                                    g_audio.initialized, g_audio.driver.playframe,
                                    buffer);
            logged_failures++;
        }
        return -1;
    }

    pthread_mutex_lock(&g_audio_lock);
    if (g_audio.resample_ratio > 1u &&
        g_audio.mix_buffer &&
        samples * g_audio.resample_ratio <= g_audio.mix_buffer_frames) {
        size_t out_frame = 0;

        for (size_t frame = 0; frame < samples; frame++) {
            const int16_t left = buffer[frame * R36SX_DRIVER_AUDIO_CHANNELS];
            const int16_t right =
                buffer[frame * R36SX_DRIVER_AUDIO_CHANNELS + 1u];
            for (uint32_t dup = 0; dup < g_audio.resample_ratio; dup++) {
                size_t idx = out_frame++ * R36SX_DRIVER_AUDIO_CHANNELS;

                g_audio.mix_buffer[idx] = left;
                g_audio.mix_buffer[idx + 1u] = right;
            }
        }
        play_buffer = g_audio.mix_buffer;
        play_frames = samples * g_audio.resample_ratio;
    }

    if (g_audio.shutter_pos < g_audio.shutter_len &&
        g_audio.mix_buffer && play_frames <= g_audio.mix_buffer_frames) {
        size_t stereo_samples = play_frames * R36SX_DRIVER_AUDIO_CHANNELS;

        if (play_buffer != g_audio.mix_buffer) {
            memcpy(g_audio.mix_buffer, play_buffer,
                   stereo_samples * sizeof(g_audio.mix_buffer[0]));
            play_buffer = g_audio.mix_buffer;
        }
        for (size_t frame = 0; frame < play_frames; frame++) {
            int16_t click = r36sx_pico286_audio_next_shutter_sample();
            size_t idx = frame * R36SX_DRIVER_AUDIO_CHANNELS;

            g_audio.mix_buffer[idx] =
                r36sx_pico286_audio_clamp_i16(
                    (int32_t)g_audio.mix_buffer[idx] + click);
            g_audio.mix_buffer[idx + 1u] =
                r36sx_pico286_audio_clamp_i16(
                    (int32_t)g_audio.mix_buffer[idx + 1u] + click);
        }
        play_buffer = g_audio.mix_buffer;
    }
    pthread_mutex_unlock(&g_audio_lock);

    rc = g_audio.driver.playframe(play_buffer, (int)play_frames);
    if (rc != 0 && logged_failures < 8) {
        r36sx_pico286_debug_log("audio: playframe rc=%d samples=%lu",
                                rc, (unsigned long)play_frames);
        logged_failures++;
    }
    return rc;
}

void linux_audio_stop(void)
{
    r36sx_pico286_debug_log("audio: stop");
}

void linux_audio_close(void)
{
    r36sx_pico286_debug_log("audio: close initialized=%d handle=%p",
                            g_audio.initialized, g_audio.handle);
    pthread_mutex_lock(&g_audio_lock);
    r36sx_driver_audio_close(&g_audio.driver);
    if (g_audio.handle) {
        dlclose(g_audio.handle);
    }
    if (g_audio.mix_buffer) {
        free(g_audio.mix_buffer);
    }
    memset(&g_audio, 0, sizeof(g_audio));
    pthread_mutex_unlock(&g_audio_lock);
}

linux_audio_backend_t linux_audio_get_backend(void)
{
    return g_audio.initialized ? LINUX_AUDIO_ALSA : LINUX_AUDIO_NONE;
}

const char *linux_audio_get_backend_name(void)
{
    return g_audio.initialized ? "driver.so" : "none";
}
