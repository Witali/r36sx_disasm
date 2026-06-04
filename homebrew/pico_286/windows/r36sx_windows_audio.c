#include "linux-audio.h"

int linux_audio_init(int sample_rate, int channels, int buffer_size)
{
    (void)sample_rate;
    (void)channels;
    (void)buffer_size;
    return -1;
}

int linux_audio_start(void)
{
    return -1;
}

int linux_audio_write(const int16_t *buffer, size_t samples)
{
    (void)buffer;
    (void)samples;
    return 0;
}

void linux_audio_stop(void)
{
}

void linux_audio_close(void)
{
}

void r36sx_pico286_audio_play_shutter(void)
{
}

linux_audio_backend_t linux_audio_get_backend(void)
{
    return LINUX_AUDIO_NONE;
}

const char *linux_audio_get_backend_name(void)
{
    return "win32-stub";
}
