/*
 * Minimal native audio helper for standalone MIPS programs.
 *
 * Stock hcprojector's decompiled cube_wav path opens /dev/auddec, calls
 * AUDDEC_INIT/AUDDEC_START, then writes AvPktHd + PCM payload packets. This
 * header keeps the same device-facing shape without depending on HCRTOS
 * headers being present in the Buildroot sysroot.
 */

#ifndef R36SX_NATIVE_AUDIO_H
#define R36SX_NATIVE_AUDIO_H

#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define R36SX_AUDIO_DEVICE_PATH "/dev/auddec"
#define R36SX_AUDDEC_INIT_IOCTL 0x82780301ul
#define R36SX_AUDDEC_START_IOCTL 0x20000304ul
#define R36SX_AUDDEC_CONFIG_SIZE 0x278u
#define R36SX_AUDIO_CODEC_PCM_S16LE 0x10000u
#define R36SX_AUDIO_RATE 44100u
#define R36SX_AUDIO_CHANNELS 2u
#define R36SX_AUDIO_BYTES_PER_SAMPLE 2u
#define R36SX_AUDIO_SAMPLES_PER_FRAME (R36SX_AUDIO_RATE / 60u)
#define R36SX_AUDIO_MAX_SEGMENTS 4u

#if defined(__GNUC__)
#define R36SX_AUDIO_MAYBE_UNUSED __attribute__((unused))
#else
#define R36SX_AUDIO_MAYBE_UNUSED
#endif

enum {
    R36SX_AV_PACKET_ES_DATA = 0,
    R36SX_AV_PACKET_EOS = 2
};

struct r36sx_av_packet_header {
    int32_t pts;
    int32_t dur;
    uint32_t size_and_flag;
    uint16_t video_rotate_mode;
    uint16_t video_mirror_mode;
} __attribute__((packed));

struct r36sx_audio_segment {
    uint32_t start_freq;
    uint32_t end_freq;
    uint32_t samples;
    uint32_t gap_samples;
    uint32_t amplitude;
};

struct r36sx_audio_state {
    int fd;
    int disabled;
    uint32_t phase;
    uint32_t segment_index;
    uint32_t segment_pos;
    uint32_t segment_count;
    struct r36sx_audio_segment segments[R36SX_AUDIO_MAX_SEGMENTS];
    int16_t frame[R36SX_AUDIO_SAMPLES_PER_FRAME * R36SX_AUDIO_CHANNELS];
};

/* Store a 32-bit integer in the little-endian layout expected by auddec. */
static void r36sx_audio_put_u32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)value;
    dst[1] = (uint8_t)(value >> 8);
    dst[2] = (uint8_t)(value >> 16);
    dst[3] = (uint8_t)(value >> 24);
}

/* Reset helper state and mark the audio device as unopened. */
static void r36sx_audio_init(struct r36sx_audio_state *audio)
{
    memset(audio, 0, sizeof(*audio));
    audio->fd = -1;
}

/*
 * Open /dev/auddec and configure it for freerun 44.1 kHz stereo S16LE PCM.
 * On failure it disables future open attempts for this helper instance.
 */
static int r36sx_audio_open(struct r36sx_audio_state *audio)
{
    uint8_t cfg[R36SX_AUDDEC_CONFIG_SIZE];

    if (audio->disabled) {
        return -1;
    }
    if (audio->fd >= 0) {
        return 0;
    }

    audio->fd = open(R36SX_AUDIO_DEVICE_PATH, O_RDWR);
    if (audio->fd < 0) {
        audio->disabled = 1;
        return -1;
    }

    memset(cfg, 0, sizeof(cfg));
    cfg[1] = 0;  /* AVSYNC_TYPE_FREERUN */
    cfg[2] = 16; /* bits_per_coded_sample */
    cfg[3] = 2;  /* channels */
    r36sx_audio_put_u32(cfg + 4, R36SX_AUDIO_CODEC_PCM_S16LE);
    r36sx_audio_put_u32(cfg + 12, R36SX_AUDIO_RATE);
    r36sx_audio_put_u32(cfg + 20,
                        R36SX_AUDIO_CHANNELS * R36SX_AUDIO_BYTES_PER_SAMPLE);

    if (ioctl(audio->fd, R36SX_AUDDEC_INIT_IOCTL, cfg) < 0 ||
        ioctl(audio->fd, R36SX_AUDDEC_START_IOCTL, 0) < 0) {
        close(audio->fd);
        audio->fd = -1;
        audio->disabled = 1;
        return -1;
    }

    return 0;
}

/* Write a whole buffer to the device, retrying short waits on transient stalls. */
static int r36sx_audio_write_all(int fd, const void *data, size_t size)
{
    const uint8_t *ptr = (const uint8_t *)data;
    unsigned retries = 0;

    while (size != 0) {
        ssize_t wrote = write(fd, ptr, size);
        if (wrote > 0) {
            ptr += wrote;
            size -= (size_t)wrote;
            retries = 0;
            continue;
        }
        if (wrote < 0 && errno == EINTR) {
            continue;
        }
        if (retries++ < 8) {
            usleep(2000);
            continue;
        }
        return -1;
    }

    return 0;
}

/* Wrap raw PCM in the AvPktHd shape used by the firmware audio decoder. */
static int r36sx_audio_write_packet(struct r36sx_audio_state *audio,
                                    const int16_t *samples, size_t sample_count)
{
    struct r36sx_av_packet_header header;
    uint32_t byte_count = (uint32_t)(sample_count * sizeof(samples[0]));

    header.pts = -1;
    header.dur = 0;
    header.size_and_flag = (byte_count & 0x3fffffffu) |
                           ((uint32_t)R36SX_AV_PACKET_ES_DATA << 30);
    header.video_rotate_mode = 0;
    header.video_mirror_mode = 0;

    if (r36sx_audio_write_all(audio->fd, &header, sizeof(header)) < 0 ||
        r36sx_audio_write_all(audio->fd, samples, byte_count) < 0) {
        close(audio->fd);
        audio->fd = -1;
        audio->disabled = 1;
        return -1;
    }

    return 0;
}

/* Return nonzero while a generated tone still has samples or gaps left. */
static int r36sx_audio_active(const struct r36sx_audio_state *audio)
{
    return audio->segment_index < audio->segment_count;
}

/*
 * Generate one signed 16-bit mono sample from the current tone segment.
 * Segments are simple fading square waves with optional silent gaps.
 */
static int16_t r36sx_audio_next_sample(struct r36sx_audio_state *audio)
{
    while (r36sx_audio_active(audio)) {
        struct r36sx_audio_segment *seg = &audio->segments[audio->segment_index];
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
            int32_t delta = (int32_t)seg->end_freq - (int32_t)seg->start_freq;
            uint32_t freq = seg->start_freq +
                (uint32_t)((delta * (int32_t)audio->segment_pos) /
                           (int32_t)seg->samples);
            uint32_t remain = seg->samples - audio->segment_pos;
            uint32_t amp = (seg->amplitude * remain) / seg->samples;
            uint32_t step = (freq << 16) / R36SX_AUDIO_RATE;
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

/* Replace the queued tone with a new short sequence of generated segments. */
static void r36sx_audio_play_segments(struct r36sx_audio_state *audio,
                                      const struct r36sx_audio_segment *segments,
                                      uint32_t count)
{
    if (count > R36SX_AUDIO_MAX_SEGMENTS) {
        count = R36SX_AUDIO_MAX_SEGMENTS;
    }
    memcpy(audio->segments, segments, sizeof(segments[0]) * count);
    audio->segment_count = count;
    audio->segment_index = 0;
    audio->segment_pos = 0;
    audio->phase = 0;
}

/* Queue the two-part "button/click" sound used by Button Demo and pause UI. */
static void R36SX_AUDIO_MAYBE_UNUSED
r36sx_audio_play_button(struct r36sx_audio_state *audio)
{
    const struct r36sx_audio_segment segments[] = {
        { 1300, 720, 2205, 500, 5200 },
        { 980, 520, 2205, 0, 4800 }
    };
    r36sx_audio_play_segments(audio, segments,
                              (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

/* Queue a short paddle/wall bounce sound. */
static void R36SX_AUDIO_MAYBE_UNUSED
r36sx_audio_play_bounce(struct r36sx_audio_state *audio)
{
    const struct r36sx_audio_segment segments[] = {
        { 780, 680, 1200, 0, 4200 }
    };
    r36sx_audio_play_segments(audio, segments, 1);
}

/* Queue a descending score-change sound. */
static void R36SX_AUDIO_MAYBE_UNUSED
r36sx_audio_play_score(struct r36sx_audio_state *audio)
{
    const struct r36sx_audio_segment segments[] = {
        { 420, 300, 2400, 400, 5200 },
        { 260, 220, 1800, 0, 4200 }
    };
    r36sx_audio_play_segments(audio, segments,
                              (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

/* Queue an ascending win fanfare. */
static void R36SX_AUDIO_MAYBE_UNUSED
r36sx_audio_play_win(struct r36sx_audio_state *audio)
{
    const struct r36sx_audio_segment segments[] = {
        { 660, 880, 1800, 300, 4800 },
        { 880, 1320, 2600, 300, 5200 },
        { 1320, 1760, 3000, 0, 5600 }
    };
    r36sx_audio_play_segments(audio, segments,
                              (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

/* Queue a low descending lose sound. */
static void R36SX_AUDIO_MAYBE_UNUSED
r36sx_audio_play_lose(struct r36sx_audio_state *audio)
{
    const struct r36sx_audio_segment segments[] = {
        { 360, 260, 2400, 300, 5200 },
        { 260, 180, 3000, 0, 4800 }
    };
    r36sx_audio_play_segments(audio, segments,
                              (uint32_t)(sizeof(segments) / sizeof(segments[0])));
}

/*
 * Emit one video-frame worth of stereo PCM through /dev/auddec. Call this once
 * per main-loop tick after any play_* function has queued a tone.
 */
static void r36sx_audio_update(struct r36sx_audio_state *audio)
{
    if (!r36sx_audio_active(audio)) {
        return;
    }
    if (r36sx_audio_open(audio) < 0) {
        return;
    }

    for (uint32_t i = 0; i < R36SX_AUDIO_SAMPLES_PER_FRAME; i++) {
        int16_t sample = r36sx_audio_next_sample(audio);
        audio->frame[i * 2u] = sample;
        audio->frame[i * 2u + 1u] = sample;
    }

    (void)r36sx_audio_write_packet(audio, audio->frame,
                                   R36SX_AUDIO_SAMPLES_PER_FRAME *
                                   R36SX_AUDIO_CHANNELS);
}

/* Close the raw auddec device if this helper opened it. */
static void r36sx_audio_close(struct r36sx_audio_state *audio)
{
    if (audio->fd >= 0) {
        close(audio->fd);
        audio->fd = -1;
    }
}

#undef R36SX_AUDIO_MAYBE_UNUSED

#endif
