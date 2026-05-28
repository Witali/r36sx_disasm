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

#include "../common/hardware.h"

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    FB_WIDTH = R36SX_SCREEN_WIDTH,
    FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    SQUARE_SIZE = 48,
    LOG_LINES = 5,
    LOG_TEXT_LEN = 18,
    LOG_X = 20,
    LOG_Y = 370,
    FONT_SCALE = 2,
    LOG_LINE_STEP = 20,
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

static struct driver_state g_driver;
static uint16_t g_frame[FB_WIDTH * FB_HEIGHT];
static int g_square_x = (FB_WIDTH - SQUARE_SIZE) / 2;
static int g_square_y = (FB_HEIGHT - SQUARE_SIZE) / 2;
static unsigned g_color_index;
static int g_checker;
static uint32_t g_prev_buttons;
static char g_button_log[LOG_LINES][LOG_TEXT_LEN];

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

static uint8_t glyph_row(char c, unsigned row)
{
    static const uint8_t blank[7] = { 0, 0, 0, 0, 0, 0, 0 };
    static const uint8_t glyph_a[7] = { 14, 17, 17, 31, 17, 17, 17 };
    static const uint8_t glyph_b[7] = { 30, 17, 17, 30, 17, 17, 30 };
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

static void draw_text(int x, int y, const char *text, uint16_t color)
{
    for (unsigned i = 0; text[i] != '\0'; i++) {
        draw_char(x + (int)(i * 6 * FONT_SCALE), y, text[i], color);
    }
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
    draw_rect(0, LOG_Y - 8, FB_WIDTH, 104, rgb565(4, 10, 14));
    draw_rect(0, LOG_Y - 10, FB_WIDTH, 2, rgb565(40, 160, 180));

    draw_rect(g_square_x, g_square_y, SQUARE_SIZE, SQUARE_SIZE,
              square_colors[g_color_index % ARRAY_COUNT(square_colors)]);
    draw_rect(g_square_x + 12, g_square_y + 12, 24, 24, rgb565(0, 0, 0));

    draw_text(20, 18, "BUTTON DEMO", rgb565(220, 240, 245));
    draw_text(20, 42, "DPAD MOVE  A B X Y COLOR  START BG  SELECT RESET  FN EXIT",
              rgb565(180, 210, 220));

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
            usleep(FRAME_USEC);
        }
    }

    display_close();
    return 0;
}
