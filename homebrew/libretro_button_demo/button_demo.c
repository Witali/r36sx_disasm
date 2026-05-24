/*
 * Minimal libretro core for the SF3000/R36SX-like cubegm launcher.
 *
 * It draws a 320x240 RGB565 framebuffer and reacts to joypad buttons:
 * D-pad moves the square, A/B change colors, Start toggles background,
 * Select resets the square position.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define RETRO_API_VERSION 1
#define RETRO_ENVIRONMENT_SET_PIXEL_FORMAT 10

enum retro_pixel_format {
    RETRO_PIXEL_FORMAT_0RGB1555 = 0,
    RETRO_PIXEL_FORMAT_XRGB8888 = 1,
    RETRO_PIXEL_FORMAT_RGB565 = 2
};

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
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

enum {
    FB_WIDTH = 320,
    FB_HEIGHT = 240,
    SQUARE_SIZE = 32,
    AUDIO_FRAMES_PER_RUN = 735
};

static uint16_t frame[FB_WIDTH * FB_HEIGHT];
static int16_t silence[AUDIO_FRAMES_PER_RUN * 2];
static int square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
static int square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
static unsigned color_index;
static bool checker;
static bool prev_start;

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

static void clamp_square(void)
{
    if (square_x < 0) {
        square_x = 0;
    }
    if (square_y < 0) {
        square_y = 0;
    }
    if (square_x > FB_WIDTH - SQUARE_SIZE) {
        square_x = FB_WIDTH - SQUARE_SIZE;
    }
    if (square_y > FB_HEIGHT - SQUARE_SIZE) {
        square_y = FB_HEIGHT - SQUARE_SIZE;
    }
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

static void draw_frame(void)
{
    static const uint16_t square_colors[] = {
        0xffff, /* white */
        0xf800, /* red */
        0x07e0, /* green */
        0x001f, /* blue */
        0xffe0  /* yellow */
    };

    for (unsigned y = 0; y < FB_HEIGHT; y++) {
        for (unsigned x = 0; x < FB_WIDTH; x++) {
            bool tile = (((x >> 4) ^ (y >> 4)) & 1) != 0;
            uint16_t color = checker && tile ? rgb565(28, 72, 96) : rgb565(10, 18, 30);
            frame[y * FB_WIDTH + x] = color;
        }
    }

    draw_rect(0, 0, FB_WIDTH, 8, rgb565(40, 160, 180));
    draw_rect(0, FB_HEIGHT - 8, FB_WIDTH, 8, rgb565(180, 80, 40));
    draw_rect(square_x, square_y, SQUARE_SIZE, SQUARE_SIZE,
              square_colors[color_index % (sizeof(square_colors) / sizeof(square_colors[0]))]);
    draw_rect(square_x + 8, square_y + 8, 16, 16, rgb565(0, 0, 0));
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
    if (environ_cb) {
        enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
        environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt);
    }
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
    (void)cb;
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
    info->library_name = "Button Demo";
    info->library_version = "0.1";
    info->valid_extensions = "term|demo";
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
    square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
    square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
    color_index = 0;
    checker = false;
    prev_start = false;
}

void retro_deinit(void)
{
}

bool retro_load_game(const struct retro_game_info *game)
{
    (void)game;
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

    if (pressed(RETRO_DEVICE_ID_JOYPAD_LEFT)) {
        square_x -= 2;
    }
    if (pressed(RETRO_DEVICE_ID_JOYPAD_RIGHT)) {
        square_x += 2;
    }
    if (pressed(RETRO_DEVICE_ID_JOYPAD_UP)) {
        square_y -= 2;
    }
    if (pressed(RETRO_DEVICE_ID_JOYPAD_DOWN)) {
        square_y += 2;
    }
    if (pressed(RETRO_DEVICE_ID_JOYPAD_SELECT)) {
        square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
        square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
    }
    if (pressed(RETRO_DEVICE_ID_JOYPAD_A)) {
        color_index = 1;
    } else if (pressed(RETRO_DEVICE_ID_JOYPAD_B)) {
        color_index = 2;
    } else if (pressed(RETRO_DEVICE_ID_JOYPAD_X)) {
        color_index = 3;
    } else if (pressed(RETRO_DEVICE_ID_JOYPAD_Y)) {
        color_index = 4;
    }

    bool start = pressed(RETRO_DEVICE_ID_JOYPAD_START);
    if (start && !prev_start) {
        checker = !checker;
    }
    prev_start = start;

    clamp_square();
    draw_frame();

    if (video_cb) {
        video_cb(frame, FB_WIDTH, FB_HEIGHT, FB_WIDTH * sizeof(frame[0]));
    }

    if (audio_batch_cb) {
        audio_batch_cb(silence, AUDIO_FRAMES_PER_RUN);
    }
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
