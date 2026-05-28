/*
 * Audio helper for standalone native MIPS programs that already use driver.so.
 *
 * Stock rkgame sends LibRetro audio to driver.so sound_driver_playframe().
 * This keeps the same path and avoids talking to /dev/auddec directly from
 * small homebrew executables.
 */

#ifndef R36SX_DRIVER_AUDIO_H
#define R36SX_DRIVER_AUDIO_H

#include <dlfcn.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define R36SX_DRIVER_AUDIO_RATE 44100u
#define R36SX_DRIVER_AUDIO_CHANNELS 2u
#define R36SX_DRIVER_AUDIO_SAMPLES_PER_FRAME (R36SX_DRIVER_AUDIO_RATE / 60u)
#define R36SX_DRIVER_AUDIO_MAX_SEGMENTS 4u

#if defined(__GNUC__)
#define R36SX_DRIVER_AUDIO_MAYBE_UNUSED __attribute__((unused))
#else
#define R36SX_DRIVER_AUDIO_MAYBE_UNUSED
#endif

typedef void (*r36sx_sound_driver_init_fn)(int, int, int);
typedef int (*r36sx_sound_driver_playframe_fn)(const int16_t *, int);
typedef void (*r36sx_sound_driver_flush_fn)(void);
typedef void (*r36sx_sound_driver_deinit_fn)(void);

struct r36sx_driver_audio_segment {
    uint32_t start_freq;
    uint32_t end_freq;
    uint32_t samples;
    uint32_t gap_samples;
    uint32_t amplitude;
};

struct r36sx_driver_audio_state {
    r36sx_sound_driver_init_fn init;
    r36sx_sound_driver_playframe_fn playframe;
    r36sx_sound_driver_flush_fn flush;
    r36sx_sound_driver_deinit_fn deinit;
    int active;
    uint32_t phase;
    uint32_t segment_index;
    uint32_t segment_pos;
    uint32_t segment_count;
    struct r36sx_driver_audio_segment segments[R36SX_DRIVER_AUDIO_MAX_SEGMENTS];
    int16_t frame[R36SX_DRIVER_AUDIO_SAMPLES_PER_FRAME *
                  R36SX_DRIVER_AUDIO_CHANNELS];
};

static void r36sx_driver_audio_init(struct r36sx_driver_audio_state *audio)
{
    memset(audio, 0, sizeof(*audio));
}

static int r36sx_driver_audio_bind(struct r36sx_driver_audio_state *audio,
                                   void *driver_handle)
{
    if (!driver_handle) {
        return -1;
    }

    audio->init = (r36sx_sound_driver_init_fn)dlsym(driver_handle,
                                                   "sound_driver_init");
    audio->playframe =
        (r36sx_sound_driver_playframe_fn)dlsym(driver_handle,
                                              "sound_driver_playframe");
    audio->flush = (r36sx_sound_driver_flush_fn)dlsym(driver_handle,
                                                     "sound_driver_flush");
    audio->deinit = (r36sx_sound_driver_deinit_fn)dlsym(driver_handle,
                                                       "sound_driver_deinit");

    if (!audio->init || !audio->playframe || !audio->deinit) {
        return -1;
    }

    audio->init(0, (int)R36SX_DRIVER_AUDIO_RATE,
                (int)R36SX_DRIVER_AUDIO_CHANNELS);
    audio->active = 1;
    return 0;
}

static void r36sx_driver_audio_close(struct r36sx_driver_audio_state *audio)
{
    if (audio->active) {
        if (audio->flush) {
            audio->flush();
        }
        if (audio->deinit) {
            audio->deinit();
        }
    }
    r36sx_driver_audio_init(audio);
}

static int r36sx_driver_audio_tone_active(
    const struct r36sx_driver_audio_state *audio)
{
    return audio->segment_index < audio->segment_count;
}

static int16_t
r36sx_driver_audio_next_sample(struct r36sx_driver_audio_state *audio)
{
    while (r36sx_driver_audio_tone_active(audio)) {
        struct r36sx_driver_audio_segment *seg =
            &audio->segments[audio->segment_index];
        uint32_t total = seg->samples + seg->gap_samples;

        if (seg->samples == 0 || audio->segment_pos >= total) {
            audio->segment_index++;
            audio->segment_pos = 0;
            audio->phase = 0;
            continue;
        }

        if (audio->segment_pos >= seg->samples) {
            audio->segment_pos++;
            return 0;
        }

        {
            int32_t delta = (int32_t)seg->end_freq -
                            (int32_t)seg->start_freq;
            uint32_t freq = seg->start_freq +
                (uint32_t)((delta * (int32_t)audio->segment_pos) /
                           (int32_t)seg->samples);
            uint32_t remain = seg->samples - audio->segment_pos;
            uint32_t amp = (seg->amplitude * remain) / seg->samples;
            uint32_t step = (freq << 16) / R36SX_DRIVER_AUDIO_RATE;
            int16_t sample;

            if (amp < 500u) {
                amp = 500u;
            }
            audio->phase += step;
            audio->segment_pos++;
            sample = (audio->phase & 0x8000u) ? (int16_t)amp : -(int16_t)amp;
            return sample;
        }
    }

    return 0;
}

static void
r36sx_driver_audio_play_segments(struct r36sx_driver_audio_state *audio,
                                 const struct r36sx_driver_audio_segment *segments,
                                 uint32_t count)
{
    if (count > R36SX_DRIVER_AUDIO_MAX_SEGMENTS) {
        count = R36SX_DRIVER_AUDIO_MAX_SEGMENTS;
    }
    memcpy(audio->segments, segments, sizeof(segments[0]) * count);
    audio->segment_count = count;
    audio->segment_index = 0;
    audio->segment_pos = 0;
    audio->phase = 0;
}

static void R36SX_DRIVER_AUDIO_MAYBE_UNUSED
r36sx_driver_audio_play_button(struct r36sx_driver_audio_state *audio)
{
    const struct r36sx_driver_audio_segment segments[] = {
        { 1300, 720, 2205, 500, 5200 },
        { 980, 520, 2205, 0, 4800 }
    };
    r36sx_driver_audio_play_segments(
        audio, segments, (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

static void R36SX_DRIVER_AUDIO_MAYBE_UNUSED
r36sx_driver_audio_play_bounce(struct r36sx_driver_audio_state *audio)
{
    const struct r36sx_driver_audio_segment segments[] = {
        { 780, 680, 1200, 0, 4200 }
    };
    r36sx_driver_audio_play_segments(audio, segments, 1);
}

static void R36SX_DRIVER_AUDIO_MAYBE_UNUSED
r36sx_driver_audio_play_score(struct r36sx_driver_audio_state *audio)
{
    const struct r36sx_driver_audio_segment segments[] = {
        { 420, 300, 2400, 400, 5200 },
        { 260, 220, 1800, 0, 4200 }
    };
    r36sx_driver_audio_play_segments(
        audio, segments, (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

static void R36SX_DRIVER_AUDIO_MAYBE_UNUSED
r36sx_driver_audio_play_win(struct r36sx_driver_audio_state *audio)
{
    const struct r36sx_driver_audio_segment segments[] = {
        { 660, 880, 1800, 300, 4800 },
        { 880, 1320, 2600, 300, 5200 },
        { 1320, 1760, 3000, 0, 5600 }
    };
    r36sx_driver_audio_play_segments(
        audio, segments, (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

static void R36SX_DRIVER_AUDIO_MAYBE_UNUSED
r36sx_driver_audio_play_lose(struct r36sx_driver_audio_state *audio)
{
    const struct r36sx_driver_audio_segment segments[] = {
        { 360, 260, 2400, 300, 5200 },
        { 260, 180, 3000, 0, 4800 }
    };
    r36sx_driver_audio_play_segments(
        audio, segments, (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

static void r36sx_driver_audio_update(struct r36sx_driver_audio_state *audio)
{
    if (!audio->active || !audio->playframe ||
        !r36sx_driver_audio_tone_active(audio)) {
        return;
    }

    for (uint32_t i = 0; i < R36SX_DRIVER_AUDIO_SAMPLES_PER_FRAME; i++) {
        int16_t sample = r36sx_driver_audio_next_sample(audio);
        audio->frame[i * 2u] = sample;
        audio->frame[i * 2u + 1u] = sample;
    }

    (void)audio->playframe(audio->frame,
                           (int)R36SX_DRIVER_AUDIO_SAMPLES_PER_FRAME);
}

#undef R36SX_DRIVER_AUDIO_MAYBE_UNUSED

#endif
