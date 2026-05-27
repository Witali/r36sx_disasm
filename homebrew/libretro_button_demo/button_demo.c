/*
 * Minimal libretro core for the SF3000/R36SX-like cubegm launcher.
 *
 * It draws a 640x480 RGB565 framebuffer and reacts to joypad buttons:
 * D-pad moves the square, A/B change colors, Start toggles background,
 * Select resets the square position. New button presses trigger a short
 * integer-generated "pew-pew" sound through the libretro audio batch callback.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    FB_WIDTH = 640,
    FB_HEIGHT = 480,
    SQUARE_SIZE = 48,
    AUDIO_RATE = 44100,
    AUDIO_FRAMES_PER_VIDEO_FRAME = 735,
    PEW_CHIRP_SAMPLES = 4410,
    PEW_GAP_SAMPLES = 1470,
    PEW_TOTAL_SAMPLES = (PEW_CHIRP_SAMPLES * 2) + PEW_GAP_SAMPLES
};

enum {
    LOG_LINES = 5,
    LOG_TEXT_LEN = 16,
    LOG_X = 20,
    LOG_Y = 370,
    FONT_SCALE = 2,
    LOG_LINE_STEP = 18
};

static uint16_t frame[FB_WIDTH * FB_HEIGHT];
static int16_t audio_frame[AUDIO_FRAMES_PER_VIDEO_FRAME * 2];
static int square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
static int square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
static unsigned color_index;
static bool checker;
static bool prev_start;
static uint32_t prev_buttons;
static uint32_t pew_sample;
static uint32_t pew_phase;
static bool pew_active;
static char button_log[LOG_LINES][LOG_TEXT_LEN];

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

static uint8_t glyph_row(char c, unsigned row)
{
    static const uint8_t blank[7] = {0, 0, 0, 0, 0, 0, 0};
    static const uint8_t glyph_a[7] = {14, 17, 17, 31, 17, 17, 17};
    static const uint8_t glyph_b[7] = {30, 17, 17, 30, 17, 17, 30};
    static const uint8_t glyph_c[7] = {14, 17, 16, 16, 16, 17, 14};
    static const uint8_t glyph_d[7] = {30, 17, 17, 17, 17, 17, 30};
    static const uint8_t glyph_e[7] = {31, 16, 16, 30, 16, 16, 31};
    static const uint8_t glyph_f[7] = {31, 16, 16, 30, 16, 16, 16};
    static const uint8_t glyph_g[7] = {14, 17, 16, 23, 17, 17, 15};
    static const uint8_t glyph_h[7] = {17, 17, 17, 31, 17, 17, 17};
    static const uint8_t glyph_i[7] = {14, 4, 4, 4, 4, 4, 14};
    static const uint8_t glyph_l[7] = {16, 16, 16, 16, 16, 16, 31};
    static const uint8_t glyph_n[7] = {17, 25, 21, 19, 17, 17, 17};
    static const uint8_t glyph_o[7] = {14, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_p[7] = {30, 17, 17, 30, 16, 16, 16};
    static const uint8_t glyph_r[7] = {30, 17, 17, 30, 20, 18, 17};
    static const uint8_t glyph_s[7] = {15, 16, 16, 14, 1, 1, 30};
    static const uint8_t glyph_t[7] = {31, 4, 4, 4, 4, 4, 4};
    static const uint8_t glyph_u[7] = {17, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_w[7] = {17, 17, 17, 21, 21, 21, 10};
    static const uint8_t glyph_x[7] = {17, 17, 10, 4, 10, 17, 17};
    static const uint8_t glyph_y[7] = {17, 17, 10, 4, 4, 4, 4};
    static const uint8_t glyph_0[7] = {14, 17, 19, 21, 25, 17, 14};
    static const uint8_t glyph_1[7] = {4, 12, 4, 4, 4, 4, 14};
    static const uint8_t glyph_2[7] = {14, 17, 1, 2, 4, 8, 31};
    static const uint8_t glyph_3[7] = {30, 1, 1, 14, 1, 1, 30};
    static const uint8_t glyph_4[7] = {2, 6, 10, 18, 31, 2, 2};
    static const uint8_t glyph_5[7] = {31, 16, 16, 30, 1, 1, 30};
    static const uint8_t glyph_6[7] = {14, 16, 16, 30, 17, 17, 14};
    static const uint8_t glyph_7[7] = {31, 1, 2, 4, 8, 8, 8};
    static const uint8_t glyph_8[7] = {14, 17, 17, 14, 17, 17, 14};
    static const uint8_t glyph_9[7] = {14, 17, 17, 15, 1, 1, 14};
    const uint8_t *glyph = blank;

    switch (c) {
    case 'A': glyph = glyph_a; break;
    case 'B': glyph = glyph_b; break;
    case 'C': glyph = glyph_c; break;
    case 'D': glyph = glyph_d; break;
    case 'E': glyph = glyph_e; break;
    case 'F': glyph = glyph_f; break;
    case 'G': glyph = glyph_g; break;
    case 'H': glyph = glyph_h; break;
    case 'I': glyph = glyph_i; break;
    case 'L': glyph = glyph_l; break;
    case 'N': glyph = glyph_n; break;
    case 'O': glyph = glyph_o; break;
    case 'P': glyph = glyph_p; break;
    case 'R': glyph = glyph_r; break;
    case 'S': glyph = glyph_s; break;
    case 'T': glyph = glyph_t; break;
    case 'U': glyph = glyph_u; break;
    case 'W': glyph = glyph_w; break;
    case 'X': glyph = glyph_x; break;
    case 'Y': glyph = glyph_y; break;
    case '0': glyph = glyph_0; break;
    case '1': glyph = glyph_1; break;
    case '2': glyph = glyph_2; break;
    case '3': glyph = glyph_3; break;
    case '4': glyph = glyph_4; break;
    case '5': glyph = glyph_5; break;
    case '6': glyph = glyph_6; break;
    case '7': glyph = glyph_7; break;
    case '8': glyph = glyph_8; break;
    case '9': glyph = glyph_9; break;
    default: break;
    }

    return glyph[row];
}

static void draw_char(int x, int y, char c, uint16_t color)
{
    for (unsigned row = 0; row < 7; row++) {
        uint8_t bits = glyph_row(c, row);
        for (unsigned col = 0; col < 5; col++) {
            if ((bits & (uint8_t)(1u << (4 - col))) != 0) {
                draw_rect(x + (int)(col * FONT_SCALE), y + (int)(row * FONT_SCALE),
                          FONT_SCALE, FONT_SCALE, color);
            }
        }
    }
}

static void draw_text(int x, int y, const char *text, uint16_t color)
{
    for (unsigned i = 0; text[i] != '\0'; i++) {
        draw_char(x + (int)(i * 6 * FONT_SCALE), y, text[i], color);
    }
}

static void log_button(const char *label)
{
    for (unsigned row = 0; row + 1 < LOG_LINES; row++) {
        for (unsigned col = 0; col < LOG_TEXT_LEN; col++) {
            button_log[row][col] = button_log[row + 1][col];
        }
    }

    for (unsigned col = 0; col < LOG_TEXT_LEN; col++) {
        char c = label[col];
        button_log[LOG_LINES - 1][col] = c;
        if (c == '\0') {
            break;
        }
    }
    button_log[LOG_LINES - 1][LOG_TEXT_LEN - 1] = '\0';
}

static const char *button_name(unsigned id)
{
    switch (id) {
    case RETRO_DEVICE_ID_JOYPAD_B: return "B";
    case RETRO_DEVICE_ID_JOYPAD_Y: return "Y";
    case RETRO_DEVICE_ID_JOYPAD_SELECT: return "SELECT";
    case RETRO_DEVICE_ID_JOYPAD_START: return "START";
    case RETRO_DEVICE_ID_JOYPAD_UP: return "UP";
    case RETRO_DEVICE_ID_JOYPAD_DOWN: return "DOWN";
    case RETRO_DEVICE_ID_JOYPAD_LEFT: return "LEFT";
    case RETRO_DEVICE_ID_JOYPAD_RIGHT: return "RIGHT";
    case RETRO_DEVICE_ID_JOYPAD_A: return "A";
    case RETRO_DEVICE_ID_JOYPAD_X: return "X";
    case RETRO_DEVICE_ID_JOYPAD_L: return "L";
    case RETRO_DEVICE_ID_JOYPAD_R: return "R";
    default: return "";
    }
}

static void log_button_id(unsigned id)
{
    const char *name = button_name(id);
    char label[LOG_TEXT_LEN];
    unsigned pos = 0;

    label[pos++] = 'I';
    label[pos++] = 'D';
    label[pos++] = (char)('0' + ((id / 10) % 10));
    label[pos++] = (char)('0' + (id % 10));
    if (name[0] != '\0') {
        label[pos++] = ' ';
        for (unsigned i = 0; name[i] != '\0' && pos + 1 < LOG_TEXT_LEN; i++) {
            label[pos++] = name[i];
        }
    }
    label[pos] = '\0';
    log_button(label);
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

static void log_pressed_buttons(uint32_t changed)
{
    for (unsigned id = 0; id < 32; id++) {
        if ((changed & (1u << id)) != 0) {
            log_button_id(id);
        }
    }
}

static void trigger_pew(void)
{
    pew_sample = 0;
    pew_phase = 0;
    pew_active = true;
}

static int16_t synth_pew_sample(void)
{
    if (!pew_active) {
        return 0;
    }

    uint32_t local = pew_sample;
    pew_sample++;

    if (local >= PEW_TOTAL_SAMPLES) {
        pew_active = false;
        return 0;
    }

    if (local >= PEW_CHIRP_SAMPLES && local < PEW_CHIRP_SAMPLES + PEW_GAP_SAMPLES) {
        return 0;
    }

    uint32_t chirp_pos = local;
    if (local >= PEW_CHIRP_SAMPLES + PEW_GAP_SAMPLES) {
        chirp_pos = local - PEW_CHIRP_SAMPLES - PEW_GAP_SAMPLES;
    }

    uint32_t freq = 1800u - ((1200u * chirp_pos) / PEW_CHIRP_SAMPLES);
    uint32_t step = (freq << 16) / AUDIO_RATE;
    uint32_t envelope = PEW_CHIRP_SAMPLES - chirp_pos;
    int32_t amp = (int32_t)((5000u * envelope) / PEW_CHIRP_SAMPLES);

    pew_phase += step;
    return (pew_phase & 0x8000u) ? (int16_t)amp : (int16_t)-amp;
}

static void emit_audio(void)
{
    if (!audio_batch_cb && !audio_sample_cb) {
        return;
    }

    bool had_sound = pew_active;

    for (unsigned i = 0; i < AUDIO_FRAMES_PER_VIDEO_FRAME; i++) {
        int16_t sample = synth_pew_sample();
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
    draw_rect(0, 0, FB_WIDTH, 1, rgb565(255, 255, 255));
    draw_rect(0, 0, 1, FB_HEIGHT, rgb565(255, 255, 255));
    draw_rect(FB_WIDTH - 1, 0, 1, FB_HEIGHT, rgb565(255, 255, 255));
    draw_rect(0, FB_HEIGHT - 1, FB_WIDTH, 1, rgb565(255, 255, 255));
    draw_rect(FB_WIDTH / 2, 0, 1, FB_HEIGHT, rgb565(80, 120, 140));
    draw_rect(0, FB_HEIGHT / 2, FB_WIDTH, 1, rgb565(80, 120, 140));
    draw_rect(0, LOG_Y - 8, FB_WIDTH, 98, rgb565(4, 10, 14));
    draw_rect(0, LOG_Y - 10, FB_WIDTH, 2, rgb565(40, 160, 180));
    draw_rect(square_x, square_y, SQUARE_SIZE, SQUARE_SIZE,
              square_colors[color_index % (sizeof(square_colors) / sizeof(square_colors[0]))]);
    draw_rect(square_x + 12, square_y + 12, 24, 24, rgb565(0, 0, 0));

    for (unsigned row = 0; row < LOG_LINES; row++) {
        draw_text(LOG_X, LOG_Y + (int)(row * LOG_LINE_STEP), button_log[row], rgb565(220, 240, 245));
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
    prev_buttons = 0;
    pew_sample = 0;
    pew_phase = 0;
    pew_active = false;
    zero_memory(button_log, sizeof(button_log));
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

    unsigned buttons = read_buttons();
    unsigned changed = buttons & ~prev_buttons;

    log_pressed_buttons(changed);
    if (changed != 0) {
        trigger_pew();
    }

    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_LEFT)) != 0) {
        square_x -= 2;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_RIGHT)) != 0) {
        square_x += 2;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_UP)) != 0) {
        square_y -= 2;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_DOWN)) != 0) {
        square_y += 2;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_SELECT)) != 0) {
        square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
        square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
    }
    if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_A)) != 0) {
        color_index = 1;
    } else if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_B)) != 0) {
        color_index = 2;
    } else if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_X)) != 0) {
        color_index = 3;
    } else if ((buttons & (1u << RETRO_DEVICE_ID_JOYPAD_Y)) != 0) {
        color_index = 4;
    }

    bool start = (buttons & (1u << RETRO_DEVICE_ID_JOYPAD_START)) != 0;
    if (start && !prev_start) {
        checker = !checker;
    }
    prev_start = start;
    prev_buttons = buttons;

    clamp_square();
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
