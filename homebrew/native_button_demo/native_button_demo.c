/*
 * Standalone native Button Demo for Tiny MC.
 *
 * This is a normal MIPS executable, not a libretro core. It keeps the visual
 * behavior of the libretro button demo: a 640x480 RGB565 screen, a movable
 * square, background toggle, color changes, and a scrolling button log.
 */

#define _GNU_SOURCE

#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../common/driver_audio.h"
#include "../common/hardware.h"

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    FB_WIDTH = R36SX_SCREEN_WIDTH,
    FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    SQUARE_SIZE = 48,
    LOG_LINES = 5,
    LOG_TEXT_LEN = 24,
    LOG_X = 20,
    LOG_Y = 342,
    LOG_FONT_PX = 22,
    FONT_SCALE = 2,
    LOG_LINE_STEP = 24,
    FONT_CACHE_SLOTS = 96,
    FRAME_USEC = 16666
};

enum button_bits {
    BTN_UP_BIT = 1u << 0,
    BTN_DOWN_BIT = 1u << 1,
    BTN_LEFT_BIT = 1u << 2,
    BTN_RIGHT_BIT = 1u << 3,
    BTN_A_BIT = 1u << 4,
    BTN_B_BIT = 1u << 5,
    BTN_X_BIT = 1u << 6,
    BTN_Y_BIT = 1u << 7,
    BTN_START_BIT = 1u << 8,
    BTN_SELECT_BIT = 1u << 9,
    BTN_FN_BIT = 1u << 10
};

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);
typedef FT_Error (*ft_init_free_type_fn)(FT_Library *);
typedef FT_Error (*ft_new_face_fn)(FT_Library, const char *, FT_Long, FT_Face *);
typedef FT_Error (*ft_done_face_fn)(FT_Face);
typedef FT_Error (*ft_done_free_type_fn)(FT_Library);
typedef FT_Error (*ft_select_charmap_fn)(FT_Face, FT_Encoding);
typedef FT_Error (*ft_set_pixel_sizes_fn)(FT_Face, FT_UInt, FT_UInt);
typedef FT_Error (*ft_load_char_fn)(FT_Face, FT_ULong, FT_Int32);

struct driver_state {
    void *handle;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
    int active;
};

struct ft_glyph_cache_entry {
    uint32_t codepoint;
    int valid;
    int width;
    int rows;
    int pitch;
    int bitmap_left;
    int bitmap_top;
    int advance;
    uint8_t *buffer;
};

struct font_state {
    void *handle;
    FT_Library library;
    FT_Face face;
    ft_init_free_type_fn init_free_type;
    ft_new_face_fn new_face;
    ft_done_face_fn done_face;
    ft_done_free_type_fn done_free_type;
    ft_select_charmap_fn select_charmap;
    ft_set_pixel_sizes_fn set_pixel_sizes;
    ft_load_char_fn load_char;
    struct ft_glyph_cache_entry cache[FONT_CACHE_SLOTS];
    int active;
};

static struct driver_state g_driver;
static struct font_state g_font;
static uint16_t g_frame[FB_WIDTH * FB_HEIGHT];
static int g_square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
static int g_square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
static unsigned g_color_index;
static int g_checker;
static uint32_t g_prev_buttons;
static char g_button_log[LOG_LINES][LOG_TEXT_LEN];
static struct r36sx_driver_audio_state g_audio;

static void font_close(void);
static void display_close(void);

static uint16_t rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
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

static void draw_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0) {
        return;
    }
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > FB_WIDTH) {
        w = FB_WIDTH - x;
    }
    if (y + h > FB_HEIGHT) {
        h = FB_HEIGHT - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int yy = y; yy < y + h; yy++) {
        uint16_t *row = &g_frame[(size_t)yy * FB_WIDTH];
        for (int xx = x; xx < x + w; xx++) {
            row[xx] = color;
        }
    }
}

static void put_pixel_alpha(int x, int y, uint16_t color, unsigned alpha)
{
    if (x < 0 || y < 0 || x >= FB_WIDTH || y >= FB_HEIGHT || alpha == 0) {
        return;
    }
    if (alpha >= 255) {
        g_frame[(size_t)y * FB_WIDTH + x] = color;
        return;
    }

    uint16_t dst = g_frame[(size_t)y * FB_WIDTH + x];
    unsigned inv = 255u - alpha;
    unsigned sr = (color >> 11) & 0x1fu;
    unsigned sg = (color >> 5) & 0x3fu;
    unsigned sb = color & 0x1fu;
    unsigned dr = (dst >> 11) & 0x1fu;
    unsigned dg = (dst >> 5) & 0x3fu;
    unsigned db = dst & 0x1fu;
    unsigned r = (sr * alpha + dr * inv) / 255u;
    unsigned g = (sg * alpha + dg * inv) / 255u;
    unsigned b = (sb * alpha + db * inv) / 255u;

    g_frame[(size_t)y * FB_WIDTH + x] =
        (uint16_t)((r << 11) | (g << 5) | b);
}

static void font_cache_clear(void)
{
    for (int i = 0; i < FONT_CACHE_SLOTS; i++) {
        free(g_font.cache[i].buffer);
        memset(&g_font.cache[i], 0, sizeof(g_font.cache[i]));
    }
}

static void font_close(void)
{
    font_cache_clear();
    if (g_font.face && g_font.done_face) {
        g_font.done_face(g_font.face);
    }
    if (g_font.library && g_font.done_free_type) {
        g_font.done_free_type(g_font.library);
    }
    if (g_font.handle) {
        dlclose(g_font.handle);
    }
    memset(&g_font, 0, sizeof(g_font));
}

static int font_bind_symbols(void)
{
    g_font.init_free_type = (ft_init_free_type_fn)dlsym(g_font.handle, "FT_Init_FreeType");
    g_font.new_face = (ft_new_face_fn)dlsym(g_font.handle, "FT_New_Face");
    g_font.done_face = (ft_done_face_fn)dlsym(g_font.handle, "FT_Done_Face");
    g_font.done_free_type = (ft_done_free_type_fn)dlsym(g_font.handle, "FT_Done_FreeType");
    g_font.select_charmap = (ft_select_charmap_fn)dlsym(g_font.handle, "FT_Select_Charmap");
    g_font.set_pixel_sizes = (ft_set_pixel_sizes_fn)dlsym(g_font.handle, "FT_Set_Pixel_Sizes");
    g_font.load_char = (ft_load_char_fn)dlsym(g_font.handle, "FT_Load_Char");
    return g_font.init_free_type && g_font.new_face && g_font.done_face &&
           g_font.done_free_type && g_font.set_pixel_sizes && g_font.load_char;
}

static int font_open(void)
{
    static const char *library_paths[] = {
        R36SX_CUBEGM_DIR "/lib/libfreetype.so.6",
        R36SX_CUBEGM_DIR "/usr/lib/libfreetype.so.6",
        "libfreetype.so.6"
    };
    static const char *font_paths[] = {
        R36SX_CUBEGM_DIR "/Arial_en.ttf",
        R36SX_CUBEGM_DIR "/Arial_kr.ttf",
        R36SX_CUBEGM_DIR "/font.ttf"
    };

    if (g_font.active) {
        return 0;
    }

    memset(&g_font, 0, sizeof(g_font));
    for (size_t i = 0; i < ARRAY_COUNT(library_paths); i++) {
        g_font.handle = dlopen(library_paths[i], RTLD_NOW);
        if (g_font.handle) {
            break;
        }
    }
    if (!g_font.handle || !font_bind_symbols()) {
        font_close();
        return -1;
    }
    if (g_font.init_free_type(&g_font.library) != 0) {
        font_close();
        return -1;
    }
    for (size_t i = 0; i < ARRAY_COUNT(font_paths); i++) {
        if (access(font_paths[i], R_OK) != 0) {
            continue;
        }
        if (g_font.new_face(g_font.library, font_paths[i], 0, &g_font.face) == 0) {
            break;
        }
    }
    if (!g_font.face) {
        font_close();
        return -1;
    }
    if (g_font.select_charmap) {
        g_font.select_charmap(g_font.face, FT_ENCODING_UNICODE);
    }

    g_font.active = 1;
    return 0;
}

static struct ft_glyph_cache_entry *font_cache_slot(uint32_t codepoint)
{
    unsigned slot = codepoint >= 32u && codepoint < 128u
        ? codepoint - 32u
        : codepoint % FONT_CACHE_SLOTS;
    struct ft_glyph_cache_entry *entry = &g_font.cache[slot];

    if (entry->valid && entry->codepoint == codepoint) {
        return entry;
    }

    free(entry->buffer);
    memset(entry, 0, sizeof(*entry));
    return entry;
}

static struct ft_glyph_cache_entry *font_load_glyph(uint32_t codepoint)
{
    if (!g_font.active || !g_font.face) {
        return NULL;
    }

    struct ft_glyph_cache_entry *entry = font_cache_slot(codepoint);
    if (entry->valid) {
        return entry;
    }

    if (g_font.set_pixel_sizes(g_font.face, 0, LOG_FONT_PX) != 0) {
        return NULL;
    }
    if (g_font.load_char(g_font.face, codepoint,
                         FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT) != 0) {
        if (codepoint != '?') {
            return font_load_glyph('?');
        }
        return NULL;
    }

    FT_GlyphSlot slot = g_font.face->glyph;
    FT_Bitmap *bitmap = &slot->bitmap;
    if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) {
        return NULL;
    }

    entry->codepoint = codepoint;
    entry->bitmap_left = slot->bitmap_left;
    entry->bitmap_top = slot->bitmap_top;
    entry->width = (int)bitmap->width;
    entry->rows = (int)bitmap->rows;
    entry->pitch = entry->width;
    entry->advance = (int)(slot->advance.x >> 6);
    if (entry->advance <= 0) {
        entry->advance = entry->width + 1;
    }

    if (entry->width > 0 && entry->rows > 0) {
        size_t bytes = (size_t)entry->width * (size_t)entry->rows;
        int pitch = bitmap->pitch < 0 ? -bitmap->pitch : bitmap->pitch;

        entry->buffer = (uint8_t *)malloc(bytes);
        if (!entry->buffer) {
            memset(entry, 0, sizeof(*entry));
            return NULL;
        }
        for (int row = 0; row < entry->rows; row++) {
            const uint8_t *src = bitmap->pitch >= 0
                ? bitmap->buffer + (size_t)row * (size_t)pitch
                : bitmap->buffer + (size_t)(entry->rows - 1 - row) * (size_t)pitch;
            memcpy(entry->buffer + (size_t)row * (size_t)entry->pitch,
                   src, (size_t)entry->width);
        }
    }

    entry->valid = 1;
    return entry;
}

static void draw_freetype_glyph(const struct ft_glyph_cache_entry *glyph,
                                int x, int y, uint16_t color, unsigned alpha_mul)
{
    for (int row = 0; row < glyph->rows; row++) {
        for (int col = 0; col < glyph->width; col++) {
            unsigned alpha = glyph->buffer[(size_t)row * (size_t)glyph->pitch + (size_t)col];
            alpha = (alpha * alpha_mul) / 255u;
            put_pixel_alpha(x + col, y + row, color, alpha);
        }
    }
}

static int draw_text_freetype(int x, int y, const char *text, uint16_t color)
{
    if (!g_font.active || !text) {
        return 0;
    }

    int baseline = y + LOG_FONT_PX - 2;
    int limit = FB_WIDTH - LOG_X;
    const unsigned char *p = (const unsigned char *)text;

    while (*p != '\0') {
        uint32_t cp = *p++;
        struct ft_glyph_cache_entry *glyph = font_load_glyph(cp);
        if (!glyph) {
            return 0;
        }
        if (x + glyph->advance > limit) {
            break;
        }
        if (glyph->buffer) {
            int gx = x + glyph->bitmap_left;
            int gy = baseline - glyph->bitmap_top;
            draw_freetype_glyph(glyph, gx + 1, gy + 1, rgb565(0, 0, 0), 150);
            draw_freetype_glyph(glyph, gx, gy, color, 255);
        }
        x += glyph->advance;
    }

    return 1;
}

static uint8_t glyph_row(char c, unsigned row)
{
    static const uint8_t blank[7] = { 0, 0, 0, 0, 0, 0, 0 };
    static const uint8_t glyph_a[7] = { 14, 17, 17, 31, 17, 17, 17 };
    static const uint8_t glyph_b[7] = { 30, 17, 17, 30, 17, 17, 30 };
    static const uint8_t glyph_c[7] = { 14, 17, 16, 16, 16, 17, 14 };
    static const uint8_t glyph_d[7] = { 30, 17, 17, 17, 17, 17, 30 };
    static const uint8_t glyph_e[7] = { 31, 16, 16, 30, 16, 16, 31 };
    static const uint8_t glyph_f[7] = { 31, 16, 16, 30, 16, 16, 16 };
    static const uint8_t glyph_g[7] = { 14, 17, 16, 23, 17, 17, 15 };
    static const uint8_t glyph_h[7] = { 17, 17, 17, 31, 17, 17, 17 };
    static const uint8_t glyph_i[7] = { 14, 4, 4, 4, 4, 4, 14 };
    static const uint8_t glyph_l[7] = { 16, 16, 16, 16, 16, 16, 31 };
    static const uint8_t glyph_n[7] = { 17, 25, 21, 19, 17, 17, 17 };
    static const uint8_t glyph_o[7] = { 14, 17, 17, 17, 17, 17, 14 };
    static const uint8_t glyph_p[7] = { 30, 17, 17, 30, 16, 16, 16 };
    static const uint8_t glyph_r[7] = { 30, 17, 17, 30, 20, 18, 17 };
    static const uint8_t glyph_s[7] = { 15, 16, 16, 14, 1, 1, 30 };
    static const uint8_t glyph_t[7] = { 31, 4, 4, 4, 4, 4, 4 };
    static const uint8_t glyph_u[7] = { 17, 17, 17, 17, 17, 17, 14 };
    static const uint8_t glyph_w[7] = { 17, 17, 17, 21, 21, 21, 10 };
    static const uint8_t glyph_x[7] = { 17, 17, 10, 4, 10, 17, 17 };
    static const uint8_t glyph_y[7] = { 17, 17, 10, 4, 4, 4, 4 };
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
                draw_rect(x + (int)(col * FONT_SCALE),
                          y + (int)(row * FONT_SCALE),
                          FONT_SCALE, FONT_SCALE, color);
            }
        }
    }
}

static void draw_text_bitmap(int x, int y, const char *text, uint16_t color)
{
    for (unsigned i = 0; text[i] != '\0'; i++) {
        draw_char(x + (int)(i * 6 * FONT_SCALE), y, text[i], color);
    }
}

static void draw_text(int x, int y, const char *text, uint16_t color)
{
    if (draw_text_freetype(x, y, text, color)) {
        return;
    }
    draw_text_bitmap(x, y, text, color);
}

static void copy_label(char *dst, const char *src)
{
    size_t i = 0;

    while (i + 1 < LOG_TEXT_LEN && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static void log_button(const char *label)
{
    for (unsigned i = LOG_LINES - 1; i > 0; i--) {
        memcpy(g_button_log[i], g_button_log[i - 1], LOG_TEXT_LEN);
    }
    copy_label(g_button_log[0], label);
}

static void log_pressed_buttons(uint32_t changed)
{
    if ((changed & BTN_UP_BIT) != 0) {
        log_button("UP");
    }
    if ((changed & BTN_DOWN_BIT) != 0) {
        log_button("DOWN");
    }
    if ((changed & BTN_LEFT_BIT) != 0) {
        log_button("LEFT");
    }
    if ((changed & BTN_RIGHT_BIT) != 0) {
        log_button("RIGHT");
    }
    if ((changed & BTN_A_BIT) != 0) {
        log_button("A");
    }
    if ((changed & BTN_B_BIT) != 0) {
        log_button("B");
    }
    if ((changed & BTN_X_BIT) != 0) {
        log_button("X");
    }
    if ((changed & BTN_Y_BIT) != 0) {
        log_button("Y");
    }
    if ((changed & BTN_START_BIT) != 0) {
        log_button("START");
    }
    if ((changed & BTN_SELECT_BIT) != 0) {
        log_button("SELECT");
    }
    if ((changed & BTN_FN_BIT) != 0) {
        log_button("FN EXIT");
    }
}

static void draw_frame(void)
{
    static const uint16_t square_colors[] = {
        0xffff, 0xf800, 0x07e0, 0x001f, 0xffe0
    };

    for (int y = 0; y < FB_HEIGHT; y++) {
        for (int x = 0; x < FB_WIDTH; x++) {
            int tile = (((x >> 4) ^ (y >> 4)) & 1) != 0;
            g_frame[(size_t)y * FB_WIDTH + x] =
                g_checker && tile ? rgb565(28, 72, 96) : rgb565(10, 18, 30);
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
    draw_rect(0, LOG_Y - 8, FB_WIDTH, 132, rgb565(4, 10, 14));
    draw_rect(0, LOG_Y - 10, FB_WIDTH, 2, rgb565(40, 160, 180));

    draw_rect(g_square_x, g_square_y, SQUARE_SIZE, SQUARE_SIZE,
              square_colors[g_color_index % ARRAY_COUNT(square_colors)]);
    draw_rect(g_square_x + 12, g_square_y + 12, 24, 24, rgb565(0, 0, 0));

    for (unsigned row = 0; row < LOG_LINES; row++) {
        draw_text(LOG_X, LOG_Y + (int)(row * LOG_LINE_STEP),
                  g_button_log[row], rgb565(220, 240, 245));
    }
}

static int display_open(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    memset(&g_driver, 0, sizeof(g_driver));
    for (size_t i = 0; i < ARRAY_COUNT(paths); i++) {
        g_driver.handle = dlopen(paths[i], RTLD_NOW);
        if (g_driver.handle) {
            break;
        }
    }
    if (!g_driver.handle) {
        fprintf(stderr, "native_button_demo: cannot open driver.so: %s\n", dlerror());
        return -1;
    }

    g_driver.setting = (video_driver_setting_fn)dlsym(g_driver.handle, "video_driver_setting");
    g_driver.init = (video_drivers_init_fn)dlsym(g_driver.handle, "video_drivers_init");
    g_driver.disp_frame = (video_driver_disp_frame_fn)dlsym(g_driver.handle, "video_driver_disp_frame");
    g_driver.deinit = (video_driver_deinit_fn)dlsym(g_driver.handle, "video_driver_deinit");
    g_driver.cube_ioctl = (cube_ioctl_fn)dlsym(g_driver.handle, "cube_ioctl");
    if (!g_driver.setting || !g_driver.init || !g_driver.disp_frame || !g_driver.deinit) {
        fprintf(stderr, "native_button_demo: driver.so display symbols missing\n");
        display_close();
        return -1;
    }
    r36sx_driver_audio_init(&g_audio);
    (void)r36sx_driver_audio_bind(&g_audio, g_driver.handle);

    {
        int cfg[5] = {
            R36SX_DRIVER_SETTING_0,
            R36SX_DRIVER_SETTING_1,
            R36SX_DRIVER_SETTING_2,
            R36SX_DRIVER_SETTING_WIDTH,
            R36SX_DRIVER_SETTING_HEIGHT
        };
        g_driver.setting(cfg);
    }
    if (g_driver.init() < 0) {
        fprintf(stderr, "native_button_demo: video_drivers_init failed\n");
        display_close();
        return -1;
    }

    g_driver.active = 1;
    return 0;
}

static void display_close(void)
{
    r36sx_driver_audio_close(&g_audio);
    font_close();
    if (g_driver.active && g_driver.deinit) {
        g_driver.deinit();
    }
    if (g_driver.handle) {
        dlclose(g_driver.handle);
    }
    memset(&g_driver, 0, sizeof(g_driver));
}

static void present_frame(void)
{
    if (g_driver.active && g_driver.disp_frame) {
        g_driver.disp_frame(g_frame, FB_WIDTH, FB_HEIGHT, R36SX_RGB565_FRAME_STRIDE);
    }
}

static uint32_t translate_rkgame_keys(uint32_t raw)
{
    uint32_t buttons = 0;

    if ((raw & R36SX_RKGAME_KEY_UP) != 0) {
        buttons |= BTN_UP_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_DOWN) != 0) {
        buttons |= BTN_DOWN_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_LEFT) != 0) {
        buttons |= BTN_LEFT_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_RIGHT) != 0) {
        buttons |= BTN_RIGHT_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_A) != 0) {
        buttons |= BTN_A_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_B) != 0) {
        buttons |= BTN_B_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_X) != 0) {
        buttons |= BTN_X_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_Y) != 0) {
        buttons |= BTN_Y_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_START) != 0) {
        buttons |= BTN_START_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_SELECT) != 0) {
        buttons |= BTN_SELECT_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_FN) != 0) {
        buttons |= BTN_FN_BIT;
    }

    return buttons;
}

static uint32_t input_buttons(void)
{
    uint32_t state = 0;

    if (!g_driver.cube_ioctl) {
        return 0;
    }

    if (g_driver.cube_key_addr == 0) {
        uint32_t addr = 0;
        g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_driver.cube_key_addr = addr;
        g_driver.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
    }
    if (!g_driver.cube_key_mem) {
        return 0;
    }

    g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);
    (void)state;
    return translate_rkgame_keys(g_driver.cube_key_mem[0]) |
           translate_rkgame_keys(g_driver.cube_key_mem[1]);
}

static void reset_demo(void)
{
    g_square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
    g_square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
}

static void tick_demo(uint32_t buttons)
{
    uint32_t changed = buttons & ~g_prev_buttons;

    log_pressed_buttons(changed);
    if (changed != 0) {
        r36sx_driver_audio_play_button(&g_audio);
    }

    if ((buttons & BTN_LEFT_BIT) != 0) {
        g_square_x -= 3;
    }
    if ((buttons & BTN_RIGHT_BIT) != 0) {
        g_square_x += 3;
    }
    if ((buttons & BTN_UP_BIT) != 0) {
        g_square_y -= 3;
    }
    if ((buttons & BTN_DOWN_BIT) != 0) {
        g_square_y += 3;
    }

    if ((buttons & BTN_A_BIT) != 0) {
        g_color_index = 1;
    } else if ((buttons & BTN_B_BIT) != 0) {
        g_color_index = 2;
    } else if ((buttons & BTN_X_BIT) != 0) {
        g_color_index = 3;
    } else if ((buttons & BTN_Y_BIT) != 0) {
        g_color_index = 4;
    }

    if ((changed & BTN_START_BIT) != 0) {
        g_checker = !g_checker;
    }
    if ((changed & BTN_SELECT_BIT) != 0) {
        reset_demo();
    }

    g_square_x = clamp_int(g_square_x, 0, FB_WIDTH - SQUARE_SIZE);
    g_square_y = clamp_int(g_square_y, 0, FB_HEIGHT - SQUARE_SIZE);
    g_prev_buttons = buttons;
}

int main(void)
{
    int running = 1;

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    if (display_open() != 0) {
        display_close();
        return 1;
    }
    (void)font_open();
    memset(g_button_log, 0, sizeof(g_button_log));
    copy_label(g_button_log[0], "PRESS BUTTONS");

    while (running) {
        uint32_t buttons = input_buttons();
        uint32_t changed = buttons & ~g_prev_buttons;

        if ((changed & BTN_FN_BIT) != 0) {
            running = 0;
        } else {
            tick_demo(buttons);
            draw_frame();
            present_frame();
            r36sx_driver_audio_update(&g_audio);
            usleep(FRAME_USEC);
        }
    }

    display_close();
    return 0;
}
