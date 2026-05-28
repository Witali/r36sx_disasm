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
    int initialized;
};

static struct r36sx_pico286_audio g_audio;

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

int linux_audio_init(int sample_rate, int channels, int buffer_size)
{
    (void)buffer_size;

    r36sx_pico286_debug_log("audio: init rate=%d channels=%d buffer=%d",
                            sample_rate, channels, buffer_size);
    linux_audio_close();
    if (sample_rate != (int)R36SX_DRIVER_AUDIO_RATE ||
        channels != (int)R36SX_DRIVER_AUDIO_CHANNELS) {
        r36sx_pico286_debug_log("audio: unsupported format rate=%d channels=%d",
                                sample_rate, channels);
        return -1;
    }

    memset(&g_audio, 0, sizeof(g_audio));
    r36sx_driver_audio_init(&g_audio.driver);
    g_audio.handle = r36sx_pico286_audio_open_driver();
    if (!g_audio.handle) {
        r36sx_pico286_debug_log("audio: driver.so open failed");
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
    r36sx_pico286_debug_log("audio: initialized");
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
    rc = g_audio.driver.playframe(buffer, (int)samples);
    if (rc != 0 && logged_failures < 8) {
        r36sx_pico286_debug_log("audio: playframe rc=%d samples=%lu",
                                rc, (unsigned long)samples);
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
    r36sx_driver_audio_close(&g_audio.driver);
    if (g_audio.handle) {
        dlclose(g_audio.handle);
    }
    memset(&g_audio, 0, sizeof(g_audio));
}

linux_audio_backend_t linux_audio_get_backend(void)
{
    return g_audio.initialized ? LINUX_AUDIO_ALSA : LINUX_AUDIO_NONE;
}

const char *linux_audio_get_backend_name(void)
{
    return g_audio.initialized ? "driver.so" : "none";
}
