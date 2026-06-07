#include "linux-audio.h"

#include <mmsystem.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define R36SX_WINMM_CHANNELS 2u
#define R36SX_WINMM_BITS_PER_SAMPLE 16u
#define R36SX_WINMM_BUFFER_COUNT 6u
#define R36SX_WINMM_MIN_BUFFER_FRAMES 256u
#define R36SX_WINMM_MAX_WAIT_MS 30u

struct r36sx_winmm_audio {
    HWAVEOUT device;
    WAVEHDR headers[R36SX_WINMM_BUFFER_COUNT];
    int16_t *buffers[R36SX_WINMM_BUFFER_COUNT];
    size_t buffer_frames;
    unsigned int next_buffer;
    int sample_rate;
    int initialized;
    int started;
    int lock_initialized;
    CRITICAL_SECTION lock;
    uint32_t shutter_pos;
    uint32_t shutter_len;
    uint32_t shutter_noise;
};

static struct r36sx_winmm_audio g_audio;

static int16_t r36sx_winmm_clamp_i16(int32_t sample)
{
    if (sample > 32767) {
        return 32767;
    }
    if (sample < -32768) {
        return -32768;
    }
    return (int16_t)sample;
}

static int16_t r36sx_winmm_next_shutter_sample(void)
{
    const uint32_t rate =
        g_audio.sample_rate > 0 ? (uint32_t)g_audio.sample_rate : 44100u;
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

static void r36sx_winmm_mix_shutter(int16_t *buffer, size_t frames)
{
    if (!buffer || g_audio.shutter_pos >= g_audio.shutter_len) {
        return;
    }

    for (size_t frame = 0; frame < frames; frame++) {
        int16_t click = r36sx_winmm_next_shutter_sample();
        size_t idx = frame * R36SX_WINMM_CHANNELS;

        buffer[idx] =
            r36sx_winmm_clamp_i16((int32_t)buffer[idx] + click);
        buffer[idx + 1u] =
            r36sx_winmm_clamp_i16((int32_t)buffer[idx + 1u] + click);
    }
}

static WAVEHDR *r36sx_winmm_find_free_header(void)
{
    for (unsigned int attempt = 0; attempt < R36SX_WINMM_BUFFER_COUNT;
         attempt++) {
        unsigned int index =
            (g_audio.next_buffer + attempt) % R36SX_WINMM_BUFFER_COUNT;
        WAVEHDR *header = &g_audio.headers[index];

        if ((header->dwFlags & WHDR_INQUEUE) == 0) {
            g_audio.next_buffer =
                (index + 1u) % R36SX_WINMM_BUFFER_COUNT;
            return header;
        }
    }
    return NULL;
}

static int r36sx_winmm_queue_chunk(const int16_t *buffer, size_t frames)
{
    const size_t stereo_samples = frames * R36SX_WINMM_CHANNELS;
    const size_t byte_count = stereo_samples * sizeof(buffer[0]);

    if (byte_count > 0xffffffffu) {
        return -1;
    }

    for (unsigned int wait_ms = 0; wait_ms <= R36SX_WINMM_MAX_WAIT_MS;
         wait_ms++) {
        WAVEHDR *header = NULL;
        MMRESULT rc;

        EnterCriticalSection(&g_audio.lock);
        if (!g_audio.initialized || !g_audio.started || !g_audio.device) {
            LeaveCriticalSection(&g_audio.lock);
            return -1;
        }

        header = r36sx_winmm_find_free_header();
        if (header) {
            int16_t *dst = (int16_t *)header->lpData;

            memcpy(dst, buffer, byte_count);
            r36sx_winmm_mix_shutter(dst, frames);
            header->dwBufferLength = (DWORD)byte_count;
            rc = waveOutWrite(g_audio.device, header, sizeof(*header));
            LeaveCriticalSection(&g_audio.lock);
            if (rc == MMSYSERR_NOERROR) {
                return 0;
            }
            r36sx_pico286_debug_log("audio: waveOutWrite failed rc=%u",
                                    (unsigned int)rc);
            return -1;
        }
        LeaveCriticalSection(&g_audio.lock);
        Sleep(1);
    }

    r36sx_pico286_debug_log("audio: waveOut queue full frames=%llu",
                            (unsigned long long)frames);
    return -1;
}

int linux_audio_init(int sample_rate, int channels, int buffer_size)
{
    WAVEFORMATEX format;
    MMRESULT rc;

    r36sx_pico286_debug_log("audio: winmm init rate=%d channels=%d buffer=%d",
                            sample_rate, channels, buffer_size);
    linux_audio_close();

    if ((sample_rate != 22050 && sample_rate != 44100) ||
        channels != (int)R36SX_WINMM_CHANNELS || buffer_size <= 0) {
        r36sx_pico286_debug_log("audio: winmm unsupported format rate=%d channels=%d buffer=%d",
                                sample_rate, channels, buffer_size);
        return -1;
    }

    memset(&g_audio, 0, sizeof(g_audio));
    InitializeCriticalSection(&g_audio.lock);
    g_audio.lock_initialized = 1;
    g_audio.sample_rate = sample_rate;
    g_audio.buffer_frames = (size_t)buffer_size;
    if (g_audio.buffer_frames < R36SX_WINMM_MIN_BUFFER_FRAMES) {
        g_audio.buffer_frames = R36SX_WINMM_MIN_BUFFER_FRAMES;
    }

    memset(&format, 0, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = (WORD)channels;
    format.nSamplesPerSec = (DWORD)sample_rate;
    format.wBitsPerSample = R36SX_WINMM_BITS_PER_SAMPLE;
    format.nBlockAlign =
        (WORD)((format.nChannels * format.wBitsPerSample) / 8u);
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

    rc = waveOutOpen(&g_audio.device, WAVE_MAPPER, &format, 0, 0,
                     CALLBACK_NULL);
    if (rc != MMSYSERR_NOERROR) {
        r36sx_pico286_debug_log("audio: waveOutOpen failed rc=%u",
                                (unsigned int)rc);
        linux_audio_close();
        return -1;
    }

    for (unsigned int i = 0; i < R36SX_WINMM_BUFFER_COUNT; i++) {
        size_t buffer_samples =
            g_audio.buffer_frames * R36SX_WINMM_CHANNELS;
        size_t buffer_bytes = buffer_samples * sizeof(g_audio.buffers[i][0]);

        if (buffer_bytes > 0xffffffffu) {
            linux_audio_close();
            return -1;
        }
        g_audio.buffers[i] =
            (int16_t *)calloc(buffer_samples, sizeof(g_audio.buffers[i][0]));
        if (!g_audio.buffers[i]) {
            r36sx_pico286_debug_log("audio: winmm buffer allocation failed");
            linux_audio_close();
            return -1;
        }

        memset(&g_audio.headers[i], 0, sizeof(g_audio.headers[i]));
        g_audio.headers[i].lpData = (LPSTR)g_audio.buffers[i];
        g_audio.headers[i].dwBufferLength = (DWORD)buffer_bytes;
        rc = waveOutPrepareHeader(g_audio.device, &g_audio.headers[i],
                                  sizeof(g_audio.headers[i]));
        if (rc != MMSYSERR_NOERROR) {
            r36sx_pico286_debug_log("audio: waveOutPrepareHeader failed rc=%u",
                                    (unsigned int)rc);
            linux_audio_close();
            return -1;
        }
    }

    g_audio.initialized = 1;
    r36sx_pico286_debug_log("audio: winmm initialized rate=%d frames=%llu",
                            sample_rate,
                            (unsigned long long)g_audio.buffer_frames);
    return 0;
}

int linux_audio_start(void)
{
    if (!g_audio.lock_initialized) {
        return -1;
    }

    EnterCriticalSection(&g_audio.lock);
    g_audio.started = g_audio.initialized ? 1 : 0;
    LeaveCriticalSection(&g_audio.lock);
    r36sx_pico286_debug_log("audio: winmm start initialized=%d",
                            g_audio.initialized);
    return g_audio.started ? 0 : -1;
}

int linux_audio_write(const int16_t *buffer, size_t samples)
{
    size_t offset = 0;

    if (!buffer || !g_audio.initialized || !g_audio.started ||
        g_audio.buffer_frames == 0) {
        return -1;
    }

    while (offset < samples) {
        size_t frames = samples - offset;

        if (frames > g_audio.buffer_frames) {
            frames = g_audio.buffer_frames;
        }
        if (r36sx_winmm_queue_chunk(
                &buffer[offset * R36SX_WINMM_CHANNELS], frames) != 0) {
            return -1;
        }
        offset += frames;
    }
    return 0;
}

void linux_audio_stop(void)
{
    if (!g_audio.lock_initialized) {
        return;
    }

    EnterCriticalSection(&g_audio.lock);
    if (g_audio.device) {
        waveOutReset(g_audio.device);
    }
    g_audio.started = 0;
    LeaveCriticalSection(&g_audio.lock);
    r36sx_pico286_debug_log("audio: winmm stop");
}

void linux_audio_close(void)
{
    int lock_initialized = g_audio.lock_initialized;

    if (lock_initialized) {
        EnterCriticalSection(&g_audio.lock);
    }

    if (g_audio.device) {
        waveOutReset(g_audio.device);
    }
    for (unsigned int i = 0; i < R36SX_WINMM_BUFFER_COUNT; i++) {
        if (g_audio.device &&
            (g_audio.headers[i].dwFlags & WHDR_PREPARED) != 0) {
            waveOutUnprepareHeader(g_audio.device, &g_audio.headers[i],
                                   sizeof(g_audio.headers[i]));
        }
        free(g_audio.buffers[i]);
        g_audio.buffers[i] = NULL;
    }
    if (g_audio.device) {
        waveOutClose(g_audio.device);
    }

    if (lock_initialized) {
        LeaveCriticalSection(&g_audio.lock);
        DeleteCriticalSection(&g_audio.lock);
    }
    memset(&g_audio, 0, sizeof(g_audio));
}

void r36sx_pico286_audio_play_shutter(void)
{
    if (!g_audio.lock_initialized) {
        return;
    }

    EnterCriticalSection(&g_audio.lock);
    g_audio.shutter_pos = 0;
    g_audio.shutter_len =
        g_audio.sample_rate > 0 ? (uint32_t)g_audio.sample_rate / 8u : 0u;
    g_audio.shutter_noise = 0x5eed1234u;
    LeaveCriticalSection(&g_audio.lock);
}

linux_audio_backend_t linux_audio_get_backend(void)
{
    return g_audio.initialized ? LINUX_AUDIO_WINMM : LINUX_AUDIO_NONE;
}

const char *linux_audio_get_backend_name(void)
{
    return g_audio.initialized ? "winmm/waveOut" : "none";
}
