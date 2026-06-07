#ifndef R36SX_WINDOWS_LINUX_AUDIO_H
#define R36SX_WINDOWS_LINUX_AUDIO_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LINUX_AUDIO_NONE = 0,
    LINUX_AUDIO_WIN32_STUB = 100,
    LINUX_AUDIO_WINMM = 101
} linux_audio_backend_t;

int linux_audio_init(int sample_rate, int channels, int buffer_size);
int linux_audio_start(void);
int linux_audio_write(const int16_t *buffer, size_t samples);
void linux_audio_stop(void);
void linux_audio_close(void);
void r36sx_pico286_audio_play_shutter(void);
linux_audio_backend_t linux_audio_get_backend(void);
const char *linux_audio_get_backend_name(void);

#ifdef __cplusplus
}
#endif

#endif
