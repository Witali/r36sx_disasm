/*
 * Tiny Pong-style libretro core for the SF3000/R36SX-like cubegm launcher.
 *
 * This intentionally reuses the working Button Demo shape: 640x480 RGB565
 * framebuffer, no libc dependency, simple joypad polling, and generated audio.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../common/libretro_hardware.h"

#define RETRO_API_VERSION 1

enum retro_device {
    RETRO_DEVICE_JOYPAD = 1
};

enum retro_device_id_joypad {
    RETRO_DEVICE_ID_JOYPAD_B = 0,
    RETRO_DEVICE_ID_JOYPAD_Y = 1,
    RETRO_DEVICE_ID_JOYPAD_SELECT = 2,
    RETRO_DEVICE_ID_JOYPAD_START = 3,
    RETRO_DEVICE_ID_JOYPAD_UP = 4,
    RETRO_DEVICE_ID_JOYPAD_DOWN = 5,
    RETRO_DEVICE_ID_JOYPAD_LEFT = 6,
    RETRO_DEVICE_ID_JOYPAD_RIGHT = 7,
    RETRO_DEVICE_ID_JOYPAD_A = 8,
    RETRO_DEVICE_ID_JOYPAD_X = 9,
    RETRO_DEVICE_ID_JOYPAD_L = 10,
    RETRO_DEVICE_ID_JOYPAD_R = 11
};

struct retro_game_info {
    const char *path;
    const void *data;
    size_t size;
    const char *meta;
};

struct retro_system_info {
    const char *library_name;
    const char *library_version;
    const char *valid_extensions;
    bool need_fullpath;
    bool block_extract;
};

struct retro_game_geometry {
    unsigned base_width;
    unsigned base_height;
    unsigned max_width;
    unsigned max_height;
    float aspect_ratio;
};

struct retro_system_timing {
    double fps;
    double sample_rate;
};

struct retro_system_av_info {
    struct retro_game_geometry geometry;
    struct retro_system_timing timing;
};

typedef bool (*retro_environment_t)(unsigned cmd, void *data);
typedef void (*retro_video_refresh_t)(const void *data, unsigned width, unsigned height, size_t pitch);
typedef void (*retro_audio_sample_t)(int16_t left, int16_t right);
typedef size_t (*retro_audio_sample_batch_t)(const int16_t *data, size_t frames);
typedef void (*retro_input_poll_t)(void);
typedef int16_t (*retro_input_state_t)(unsigned port, unsigned device, unsigned index, unsigned id);

static retro_environment_t environ_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_sample_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

enum {
    FB_WIDTH = R36SX_SCREEN_WIDTH,
    FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    PLAY_TOP = 30,
    PLAY_BOTTOM = 470,
    PLAY_LEFT = 12,
    PLAY_RIGHT = 628,
    PADDLE_W = 12,
    PADDLE_H = 76,
    PADDLE_MARGIN = 34,
    PADDLE_SPEED = 7,
    BALL_SIZE = 10,
    BALL_BASE_SPEED_X = 5,
    BALL_BASE_SPEED_Y = 3,
    SCORE_LIMIT = 9,
    SERVE_FRAMES = 40,
    AUDIO_RATE = R36SX_LIBRETRO_AUDIO_SAMPLE_RATE,
    AUDIO_FRAMES_PER_VIDEO_FRAME = R36SX_LIBRETRO_AUDIO_FRAMES_PER_VIDEO_FRAME
};

static uint16_t frame[FB_WIDTH * FB_HEIGHT];
static int16_t audio_frame[AUDIO_FRAMES_PER_VIDEO_FRAME * 2];

static int left_paddle_y;
static int right_paddle_y;
static int ball_x;
static int ball_y;
static int ball_vx;
static int ball_vy;
static unsigned left_score;
static unsigned right_score;
static unsigned serve_wait;
static unsigned frame_counter;
static bool paused;
static uint32_t prev_buttons;

static uint32_t tone_phase;
static uint32_t tone_step;
static uint32_t tone_remaining;
static uint32_t tone_total;

static void zero_memory(void *ptr, size_t size)
{
    uint8_t *bytes = (uint8_t *)ptr;
    for (size_t i = 0; i < size; i++) {
        bytes[i] = 0;
    }
}

static uint16_t rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3));
}

static bool pressed(unsigned id)
{
    if (!input_state_cb) {
        return false;
    }
    return input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, id) != 0;
}

static uint32_t read_buttons(void)
{
    uint32_t mask = 0;

    for (unsigned id = 0; id < 32; id++) {
        if (pressed(id)) {
            mask |= 1u << id;
        }
    }

    return mask;
}

static void draw_rect(int x, int y, int w, int h, uint16_t color)
{
    for (int yy = y; yy < y + h; yy++) {
        if (yy < 0 || yy >= FB_HEIGHT) {
            continue;
        }
        for (int xx = x; xx < x + w; xx++) {
            if (xx < 0 || xx >= FB_WIDTH) {
                continue;
            }
            frame[(size_t)yy * FB_WIDTH + xx] = color;
        }
    }
}

static void fill_frame(uint16_t color)
{
    for (unsigned y = 0; y < FB_HEIGHT; y++) {
        for (unsigned x = 0; x < FB_WIDTH; x++) {
            frame[(size_t)y * FB_WIDTH + x] = color;
        }
    }
}

static int clamp_int(int value, int min_value, int max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static void trigger_tone(unsigned freq, unsigned samples)
{
    tone_phase = 0;
    tone_step = (freq << 16) / AUDIO_RATE;
    tone_remaining = samples;
    tone_total = samples;
}

static int16_t synth_tone_sample(void)
{
    if (tone_remaining == 0) {
        return 0;
    }

    uint32_t envelope = tone_remaining;
    uint32_t amp = 3600u;
    if (tone_total != 0) {
        amp = 1200u + ((3600u * envelope) / tone_total);
    }

    tone_remaining--;
    tone_phase += tone_step;

    return (tone_phase & 0x8000u) ? (int16_t)amp : (int16_t)-(int32_t)amp;
}

static void emit_audio(void)
{
    if (!audio_batch_cb && !audio_sample_cb) {
        return;
    }

    bool had_sound = tone_remaining != 0;

    for (unsigned i = 0; i < AUDIO_FRAMES_PER_VIDEO_FRAME; i++) {
        int16_t sample = synth_tone_sample();
        audio_frame[i * 2] = sample;
        audio_frame[i * 2 + 1] = sample;
    }

    if (!had_sound) {
        return;
    }

    if (audio_batch_cb) {
        audio_batch_cb(audio_frame, AUDIO_FRAMES_PER_VIDEO_FRAME);
        return;
    }

    for (unsigned i = 0; i < AUDIO_FRAMES_PER_VIDEO_FRAME; i++) {
        audio_sample_cb(audio_frame[i * 2], audio_frame[i * 2 + 1]);
    }
}

static void reset_round(int direction)
{
    ball_x = (FB_WIDTH - BALL_SIZE) / 2;
    ball_y = (PLAY_TOP + PLAY_BOTTOM - BALL_SIZE) / 2;
    ball_vx = direction < 0 ? -BALL_BASE_SPEED_X : BALL_BASE_SPEED_X;
    ball_vy = ((frame_counter + left_score + right_score) & 1u) != 0 ?
              -BALL_BASE_SPEED_Y : BALL_BASE_SPEED_Y;
    serve_wait = SERVE_FRAMES;
}

static void reset_game(void)
{
    left_paddle_y = (PLAY_TOP + PLAY_BOTTOM - PADDLE_H) / 2;
    right_paddle_y = left_paddle_y;
    left_score = 0;
    right_score = 0;
    paused = false;
    prev_buttons = 0;
    tone_remaining = 0;
    tone_total = 0;
    reset_round(1);
}

static void move_paddles(uint32_t buttons)
{
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_UP)) != 0) {
        left_paddle_y -= PADDLE_SPEED;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_DOWN)) != 0) {
        left_paddle_y += PADDLE_SPEED;
    }

    bool manual_right_up =
        (buttons & (1u << RETRO_DEVICE_ID_JOYPAD_A)) != 0 ||
        (buttons & (1u << RETRO_DEVICE_ID_JOYPAD_X)) != 0;
    bool manual_right_down =
        (buttons & (1u << RETRO_DEVICE_ID_JOYPAD_B)) != 0 ||
        (buttons & (1u << RETRO_DEVICE_ID_JOYPAD_Y)) != 0;

    if (manual_right_up) {
        right_paddle_y -= PADDLE_SPEED;
    }
    if (manual_right_down) {
        right_paddle_y += PADDLE_SPEED;
    }

    if (!manual_right_up && !manual_right_down) {
        int target = ball_y + BALL_SIZE / 2;
        int center = right_paddle_y + PADDLE_H / 2;
        int ai_speed = 5;

        if (target < center - 8) {
            right_paddle_y -= ai_speed;
        } else if (target > center + 8) {
            right_paddle_y += ai_speed;
        }
    }

    left_paddle_y = clamp_int(left_paddle_y, PLAY_TOP, PLAY_BOTTOM - PADDLE_H);
    right_paddle_y = clamp_int(right_paddle_y, PLAY_TOP, PLAY_BOTTOM - PADDLE_H);
}

static bool ball_overlaps_paddle(int paddle_x, int paddle_y)
{
    return ball_x < paddle_x + PADDLE_W &&
           ball_x + BALL_SIZE > paddle_x &&
           ball_y < paddle_y + PADDLE_H &&
           ball_y + BALL_SIZE > paddle_y;
}

static void update_ball(void)
{
    if (serve_wait != 0) {
        serve_wait--;
        return;
    }

    ball_x += ball_vx;
    ball_y += ball_vy;

    if (ball_y <= PLAY_TOP) {
        ball_y = PLAY_TOP;
        ball_vy = -ball_vy;
        trigger_tone(640, 900);
    } else if (ball_y + BALL_SIZE >= PLAY_BOTTOM) {
        ball_y = PLAY_BOTTOM - BALL_SIZE;
        ball_vy = -ball_vy;
        trigger_tone(640, 900);
    }

    int left_x = PADDLE_MARGIN;
    int right_x = FB_WIDTH - PADDLE_MARGIN - PADDLE_W;

    if (ball_vx < 0 && ball_overlaps_paddle(left_x, left_paddle_y)) {
        int paddle_center = left_paddle_y + PADDLE_H / 2;
        int ball_center = ball_y + BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        ball_x = left_x + PADDLE_W;
        ball_vx = BALL_BASE_SPEED_X + (int)((left_score + right_score) / 4);
        ball_vy = clamp_int(ball_vy + offset, -7, 7);
        if (ball_vy == 0) {
            ball_vy = offset < 0 ? -2 : 2;
        }
        trigger_tone(880, 1200);
    } else if (ball_vx > 0 && ball_overlaps_paddle(right_x, right_paddle_y)) {
        int paddle_center = right_paddle_y + PADDLE_H / 2;
        int ball_center = ball_y + BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        ball_x = right_x - BALL_SIZE;
        ball_vx = -(BALL_BASE_SPEED_X + (int)((left_score + right_score) / 4));
        ball_vy = clamp_int(ball_vy + offset, -7, 7);
        if (ball_vy == 0) {
            ball_vy = offset < 0 ? -2 : 2;
        }
        trigger_tone(880, 1200);
    }

    if (ball_x + BALL_SIZE < PLAY_LEFT) {
        if (right_score < SCORE_LIMIT) {
            right_score++;
        } else {
            right_score = 0;
            left_score = 0;
        }
        trigger_tone(220, 5000);
        reset_round(-1);
    } else if (ball_x > PLAY_RIGHT) {
        if (left_score < SCORE_LIMIT) {
            left_score++;
        } else {
            right_score = 0;
            left_score = 0;
        }
        trigger_tone(220, 5000);
        reset_round(1);
    }
}

static uint8_t digit_segments(unsigned digit)
{
    static const uint8_t segments[10] = {
        0x3f, 0x06, 0x5b, 0x4f, 0x66,
        0x6d, 0x7d, 0x07, 0x7f, 0x6f
    };

    return segments[digit % 10];
}

static void draw_digit(int x, int y, unsigned digit, uint16_t color)
{
    const int w = 46;
    const int h = 78;
    const int t = 8;
    uint8_t seg = digit_segments(digit);

    if ((seg & 0x01) != 0) {
        draw_rect(x + t, y, w - t * 2, t, color);
    }
    if ((seg & 0x02) != 0) {
        draw_rect(x + w - t, y + t, t, h / 2 - t, color);
    }
    if ((seg & 0x04) != 0) {
        draw_rect(x + w - t, y + h / 2, t, h / 2 - t, color);
    }
    if ((seg & 0x08) != 0) {
        draw_rect(x + t, y + h - t, w - t * 2, t, color);
    }
    if ((seg & 0x10) != 0) {
        draw_rect(x, y + h / 2, t, h / 2 - t, color);
    }
    if ((seg & 0x20) != 0) {
        draw_rect(x, y + t, t, h / 2 - t, color);
    }
    if ((seg & 0x40) != 0) {
        draw_rect(x + t, y + h / 2 - t / 2, w - t * 2, t, color);
    }
}

static void draw_center_line(uint16_t color)
{
    for (int y = PLAY_TOP; y < PLAY_BOTTOM; y += 26) {
        draw_rect(FB_WIDTH / 2 - 2, y + 4, 4, 14, color);
    }
}

static void draw_pause_icon(uint16_t color)
{
    draw_rect(FB_WIDTH / 2 - 22, FB_HEIGHT / 2 - 28, 14, 56, color);
    draw_rect(FB_WIDTH / 2 + 8, FB_HEIGHT / 2 - 28, 14, 56, color);
}

static void draw_frame(void)
{
    uint16_t black = rgb565(0, 0, 0);
    uint16_t dim = rgb565(45, 56, 64);
    uint16_t white = rgb565(232, 238, 232);
    uint16_t accent = rgb565(80, 170, 205);

    fill_frame(black);
    draw_rect(0, PLAY_TOP - 2, FB_WIDTH, 2, dim);
    draw_rect(0, PLAY_BOTTOM, FB_WIDTH, 2, dim);
    draw_rect(0, 0, FB_WIDTH, 1, white);
    draw_rect(0, FB_HEIGHT - 1, FB_WIDTH, 1, white);
    draw_rect(0, 0, 1, FB_HEIGHT, white);
    draw_rect(FB_WIDTH - 1, 0, 1, FB_HEIGHT, white);
    draw_center_line(dim);

    draw_digit(238, 52, left_score, white);
    draw_digit(356, 52, right_score, white);

    draw_rect(PADDLE_MARGIN, left_paddle_y, PADDLE_W, PADDLE_H, white);
    draw_rect(FB_WIDTH - PADDLE_MARGIN - PADDLE_W, right_paddle_y,
              PADDLE_W, PADDLE_H, white);

    if ((serve_wait & 8u) == 0 || serve_wait == 0) {
        draw_rect(ball_x, ball_y, BALL_SIZE, BALL_SIZE, white);
    }

    if (paused) {
        draw_pause_icon(accent);
    }
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
    audio_sample_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
    audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
    input_state_cb = cb;
}

void retro_get_system_info(struct retro_system_info *info)
{
    zero_memory(info, sizeof(*info));
    info->library_name = "Pong Demo";
    info->library_version = "0.1";
    info->valid_extensions = "gb|pong";
    info->need_fullpath = false;
    info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    zero_memory(info, sizeof(*info));
    info->geometry.base_width = FB_WIDTH;
    info->geometry.base_height = FB_HEIGHT;
    info->geometry.max_width = FB_WIDTH;
    info->geometry.max_height = FB_HEIGHT;
    info->geometry.aspect_ratio = 4.0f / 3.0f;
    info->timing.fps = 60.0;
    info->timing.sample_rate = 44100.0;
}

void retro_init(void)
{
    frame_counter = 0;
    reset_game();
}

void retro_deinit(void)
{
}

bool retro_load_game(const struct retro_game_info *game)
{
    (void)game;
    reset_game();
    return true;
}

void retro_unload_game(void)
{
}

void retro_reset(void)
{
    retro_init();
}

void retro_run(void)
{
    if (input_poll_cb) {
        input_poll_cb();
    }

    uint32_t buttons = read_buttons();
    uint32_t changed = buttons & ~prev_buttons;

    if ((changed & (1u << RETRO_DEVICE_ID_JOYPAD_START)) != 0) {
        paused = !paused;
        trigger_tone(paused ? 330u : 520u, 2200);
    }

    if ((changed & (1u << RETRO_DEVICE_ID_JOYPAD_SELECT)) != 0) {
        reset_game();
        trigger_tone(740, 2500);
    }

    if (!paused) {
        move_paddles(buttons);
        update_ball();
    }

    prev_buttons = buttons;
    frame_counter++;
    draw_frame();

    if (video_cb) {
        video_cb(frame, FB_WIDTH, FB_HEIGHT, FB_WIDTH * sizeof(frame[0]));
    }

    emit_audio();
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info)
{
    (void)game_type;
    (void)info;
    (void)num_info;
    return false;
}

unsigned retro_get_region(void)
{
    return 0;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
    (void)port;
    (void)device;
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    (void)data;
    (void)size;
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    (void)data;
    (void)size;
    return false;
}

void *retro_get_memory_data(unsigned id)
{
    (void)id;
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    (void)id;
    return 0;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
    (void)index;
    (void)enabled;
    (void)code;
}

void SetFrameSkip(int skip)
{
    (void)skip;
}

int check_encrypty(void)
{
    return 1;
}

int CheckEncrypty(const void *data)
{
    (void)data;
    return 1;
}
