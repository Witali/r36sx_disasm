/*
 * MiniFB-compatible backend for running xrip/pico-286 as a native R36SX app.
 *
 * The upstream Linux backend uses X11.  On the console we keep the same tiny
 * MiniFB API surface, but present RGB888 pixels through the stock driver.so
 * RGB565 framebuffer path and translate console buttons to PC key codes.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "MiniFB.h"
#include "../common/hardware.h"

#define R36SX_PICO286_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_PICO286_FRAME_USEC 16666u
#define R36SX_PICO286_DISK_LED_HOLD_MS 350u
#define R36SX_PICO286_DISK_LED_BLINK_MS 120u
#define R36SX_PICO286_DISK_LED_RADIUS 8
#define R36SX_OSK_PANEL_H 96
#define R36SX_OSK_KEY_W 44
#define R36SX_OSK_KEY_H 13
#define R36SX_OSK_KEY_GAP 2
#define R36SX_OSK_TEXT_SCALE 1

enum {
    R36SX_OSK_VK_BACK = 8,
    R36SX_OSK_VK_TAB = 9,
    R36SX_OSK_VK_RETURN = 13,
    R36SX_OSK_VK_SHIFT = 16,
    R36SX_OSK_VK_CONTROL = 17,
    R36SX_OSK_VK_MENU = 18,
    R36SX_OSK_VK_ESCAPE = 27,
    R36SX_OSK_VK_SPACE = 32,
    R36SX_OSK_VK_LEFT = 37,
    R36SX_OSK_VK_RIGHT = 39,
    R36SX_OSK_VK_DELETE = 46,
    R36SX_OSK_VK_F1 = 112,
    R36SX_OSK_VK_OEM_1 = 186,
    R36SX_OSK_VK_OEM_PLUS = 187,
    R36SX_OSK_VK_OEM_COMMA = 188,
    R36SX_OSK_VK_OEM_MINUS = 189,
    R36SX_OSK_VK_OEM_PERIOD = 190,
    R36SX_OSK_VK_OEM_2 = 191,
    R36SX_OSK_VK_OEM_5 = 220,
    R36SX_OSK_VK_OEM_7 = 222
};

#define R36SX_OSK_FLAG_SHIFTED 0x01u
#define R36SX_OSK_FLAG_SHIFT_MOD 0x02u
#define R36SX_OSK_FLAG_CTRL_MOD 0x04u
#define R36SX_OSK_FLAG_ALT_MOD 0x08u
#define R36SX_OSK_FLAG_CLOSE 0x10u

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);

struct r36sx_mfb_driver {
    void *handle;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    uint16_t *frame;
    int width;
    int height;
    int stride;
    int active;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
    uint8_t key_down[256];
    uint64_t last_present_us;
    uint32_t last_raw_keys;
    uint8_t osk_visible;
    uint8_t osk_row;
    uint8_t osk_col;
    uint8_t osk_shift;
    uint8_t osk_ctrl;
    uint8_t osk_alt;
    uint8_t input_release_guard;
};

struct r36sx_key_map {
    uint32_t raw_mask;
    uint8_t keycode;
};

struct r36sx_osk_key {
    const char *label;
    uint16_t keycode;
    uint8_t flags;
};

static struct r36sx_mfb_driver g_mfb;
static uint32_t g_palette[256];
static volatile uint32_t g_disk_activity_until_ms;

extern void HandleInput(unsigned int keycode, int isKeyDown);

static const struct r36sx_osk_key g_osk_row0[] = {
    { "ESC", R36SX_OSK_VK_ESCAPE, 0 },
    { "1", '1', 0 }, { "2", '2', 0 }, { "3", '3', 0 },
    { "4", '4', 0 }, { "5", '5', 0 }, { "6", '6', 0 },
    { "7", '7', 0 }, { "8", '8', 0 }, { "9", '9', 0 },
    { "0", '0', 0 }, { "-", R36SX_OSK_VK_OEM_MINUS, 0 },
    { "BS", R36SX_OSK_VK_BACK, 0 }
};

static const struct r36sx_osk_key g_osk_row1[] = {
    { "TAB", R36SX_OSK_VK_TAB, 0 },
    { "Q", 'Q', 0 }, { "W", 'W', 0 }, { "E", 'E', 0 },
    { "R", 'R', 0 }, { "T", 'T', 0 }, { "Y", 'Y', 0 },
    { "U", 'U', 0 }, { "I", 'I', 0 }, { "O", 'O', 0 },
    { "P", 'P', 0 }, { "\\", R36SX_OSK_VK_OEM_5, 0 },
    { "=", R36SX_OSK_VK_OEM_PLUS, 0 }
};

static const struct r36sx_osk_key g_osk_row2[] = {
    { "CTRL", R36SX_OSK_VK_CONTROL, R36SX_OSK_FLAG_CTRL_MOD },
    { "A", 'A', 0 }, { "S", 'S', 0 }, { "D", 'D', 0 },
    { "F", 'F', 0 }, { "G", 'G', 0 }, { "H", 'H', 0 },
    { "J", 'J', 0 }, { "K", 'K', 0 }, { "L", 'L', 0 },
    { ":", R36SX_OSK_VK_OEM_1, R36SX_OSK_FLAG_SHIFTED },
    { "'", R36SX_OSK_VK_OEM_7, 0 },
    { "ENT", R36SX_OSK_VK_RETURN, 0 }
};

static const struct r36sx_osk_key g_osk_row3[] = {
    { "SHF", R36SX_OSK_VK_SHIFT, R36SX_OSK_FLAG_SHIFT_MOD },
    { "Z", 'Z', 0 }, { "X", 'X', 0 }, { "C", 'C', 0 },
    { "V", 'V', 0 }, { "B", 'B', 0 }, { "N", 'N', 0 },
    { "M", 'M', 0 }, { ",", R36SX_OSK_VK_OEM_COMMA, 0 },
    { ".", R36SX_OSK_VK_OEM_PERIOD, 0 },
    { "/", R36SX_OSK_VK_OEM_2, 0 },
    { "SPC", R36SX_OSK_VK_SPACE, 0 }
};

static const struct r36sx_osk_key g_osk_row4[] = {
    { "F1", R36SX_OSK_VK_F1 + 0, 0 },
    { "F2", R36SX_OSK_VK_F1 + 1, 0 },
    { "F3", R36SX_OSK_VK_F1 + 2, 0 },
    { "F4", R36SX_OSK_VK_F1 + 3, 0 },
    { "F5", R36SX_OSK_VK_F1 + 4, 0 },
    { "F6", R36SX_OSK_VK_F1 + 5, 0 },
    { "F7", R36SX_OSK_VK_F1 + 6, 0 },
    { "F8", R36SX_OSK_VK_F1 + 7, 0 },
    { "F9", R36SX_OSK_VK_F1 + 8, 0 },
    { "F10", R36SX_OSK_VK_F1 + 9, 0 },
    { "ALT", R36SX_OSK_VK_MENU, R36SX_OSK_FLAG_ALT_MOD },
    { "DEL", R36SX_OSK_VK_DELETE, 0 },
    { "CLOSE", 0, R36SX_OSK_FLAG_CLOSE }
};

static const struct r36sx_osk_key *const g_osk_rows[] = {
    g_osk_row0, g_osk_row1, g_osk_row2, g_osk_row3, g_osk_row4
};

static const uint8_t g_osk_row_counts[] = {
    R36SX_PICO286_ARRAY_COUNT(g_osk_row0),
    R36SX_PICO286_ARRAY_COUNT(g_osk_row1),
    R36SX_PICO286_ARRAY_COUNT(g_osk_row2),
    R36SX_PICO286_ARRAY_COUNT(g_osk_row3),
    R36SX_PICO286_ARRAY_COUNT(g_osk_row4)
};

static uint64_t r36sx_mfb_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static uint32_t r36sx_mfb_now_ms32(void)
{
    return (uint32_t)(r36sx_mfb_now_us() / 1000ull);
}

void r36sx_pico286_disk_activity(void)
{
    g_disk_activity_until_ms =
        r36sx_mfb_now_ms32() + R36SX_PICO286_DISK_LED_HOLD_MS;
}

static uint16_t r36sx_mfb_rgb888_to_rgb565(uint32_t color)
{
    uint32_t r = (color >> 16) & 0xffu;
    uint32_t g = (color >> 8) & 0xffu;
    uint32_t b = color & 0xffu;
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static int r36sx_osk_panel_y(void)
{
    return g_mfb.height > R36SX_OSK_PANEL_H ?
        g_mfb.height - R36SX_OSK_PANEL_H : 0;
}

static uint16_t r36sx_mfb_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r & 0xf8u) << 8) |
                      ((uint16_t)(g & 0xfcu) << 3) |
                      ((uint16_t)b >> 3));
}

static void r36sx_mfb_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0 || !g_mfb.frame) {
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
    if (x + w > g_mfb.width) {
        w = g_mfb.width - x;
    }
    if (y + h > g_mfb.height) {
        h = g_mfb.height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = g_mfb.frame + (size_t)(y + row) * (size_t)g_mfb.width +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void r36sx_mfb_stroke_rect(int x, int y, int w, int h, uint16_t color)
{
    r36sx_mfb_fill_rect(x, y, w, 1, color);
    r36sx_mfb_fill_rect(x, y + h - 1, w, 1, color);
    r36sx_mfb_fill_rect(x, y, 1, h, color);
    r36sx_mfb_fill_rect(x + w - 1, y, 1, h, color);
}

static uint8_t r36sx_osk_glyph_row(char c, int row)
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
    static const uint8_t glyph_j[7] = {1, 1, 1, 1, 17, 17, 14};
    static const uint8_t glyph_k[7] = {17, 18, 20, 24, 20, 18, 17};
    static const uint8_t glyph_l[7] = {16, 16, 16, 16, 16, 16, 31};
    static const uint8_t glyph_m[7] = {17, 27, 21, 21, 17, 17, 17};
    static const uint8_t glyph_n[7] = {17, 25, 21, 19, 17, 17, 17};
    static const uint8_t glyph_o[7] = {14, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_p[7] = {30, 17, 17, 30, 16, 16, 16};
    static const uint8_t glyph_q[7] = {14, 17, 17, 17, 21, 18, 13};
    static const uint8_t glyph_r[7] = {30, 17, 17, 30, 20, 18, 17};
    static const uint8_t glyph_s[7] = {15, 16, 16, 14, 1, 1, 30};
    static const uint8_t glyph_t[7] = {31, 4, 4, 4, 4, 4, 4};
    static const uint8_t glyph_u[7] = {17, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_v[7] = {17, 17, 17, 17, 17, 10, 4};
    static const uint8_t glyph_w[7] = {17, 17, 17, 21, 21, 21, 10};
    static const uint8_t glyph_x[7] = {17, 17, 10, 4, 10, 17, 17};
    static const uint8_t glyph_y[7] = {17, 17, 10, 4, 4, 4, 4};
    static const uint8_t glyph_z[7] = {31, 1, 2, 4, 8, 16, 31};
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
    static const uint8_t glyph_minus[7] = {0, 0, 0, 31, 0, 0, 0};
    static const uint8_t glyph_equal[7] = {0, 0, 31, 0, 31, 0, 0};
    static const uint8_t glyph_colon[7] = {0, 4, 4, 0, 4, 4, 0};
    static const uint8_t glyph_quote[7] = {4, 4, 8, 0, 0, 0, 0};
    static const uint8_t glyph_comma[7] = {0, 0, 0, 0, 4, 4, 8};
    static const uint8_t glyph_period[7] = {0, 0, 0, 0, 0, 12, 12};
    static const uint8_t glyph_slash[7] = {1, 1, 2, 4, 8, 16, 16};
    static const uint8_t glyph_backslash[7] = {16, 16, 8, 4, 2, 1, 1};
    const uint8_t *glyph = blank;

    if (c >= 'a' && c <= 'z') {
        c = (char)(c - ('a' - 'A'));
    }

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
    case 'J': glyph = glyph_j; break;
    case 'K': glyph = glyph_k; break;
    case 'L': glyph = glyph_l; break;
    case 'M': glyph = glyph_m; break;
    case 'N': glyph = glyph_n; break;
    case 'O': glyph = glyph_o; break;
    case 'P': glyph = glyph_p; break;
    case 'Q': glyph = glyph_q; break;
    case 'R': glyph = glyph_r; break;
    case 'S': glyph = glyph_s; break;
    case 'T': glyph = glyph_t; break;
    case 'U': glyph = glyph_u; break;
    case 'V': glyph = glyph_v; break;
    case 'W': glyph = glyph_w; break;
    case 'X': glyph = glyph_x; break;
    case 'Y': glyph = glyph_y; break;
    case 'Z': glyph = glyph_z; break;
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
    case '-': glyph = glyph_minus; break;
    case '=': glyph = glyph_equal; break;
    case ':': glyph = glyph_colon; break;
    case '\'': glyph = glyph_quote; break;
    case ',': glyph = glyph_comma; break;
    case '.': glyph = glyph_period; break;
    case '/': glyph = glyph_slash; break;
    case '\\': glyph = glyph_backslash; break;
    default: break;
    }
    return glyph[row];
}

static int r36sx_osk_text_width(const char *text, int scale)
{
    int len = 0;
    while (text && text[len] != '\0') {
        len++;
    }
    return len > 0 ? (len * 6 - 1) * scale : 0;
}

static void r36sx_osk_draw_char(int x, int y, char c, uint16_t color, int scale)
{
    for (int row = 0; row < 7; row++) {
        uint8_t bits = r36sx_osk_glyph_row(c, row);
        for (int col = 0; col < 5; col++) {
            if ((bits & (uint8_t)(1u << (4 - col))) != 0) {
                r36sx_mfb_fill_rect(x + col * scale, y + row * scale,
                                    scale, scale, color);
            }
        }
    }
}

static void r36sx_osk_draw_text(int x, int y, const char *text,
                                uint16_t color, int scale)
{
    for (int i = 0; text && text[i] != '\0'; i++) {
        r36sx_osk_draw_char(x + i * 6 * scale, y, text[i], color, scale);
    }
}

static void r36sx_mfb_release_all_keys(void)
{
    for (size_t code = 0; code < sizeof(g_mfb.key_down); code++) {
        if (g_mfb.key_down[code]) {
            HandleInput((unsigned int)code, 0);
            g_mfb.key_down[code] = 0;
        }
    }
}

static void r36sx_osk_set_visible(int visible)
{
    visible = visible != 0;
    if (g_mfb.osk_visible == (uint8_t)visible) {
        return;
    }
    r36sx_mfb_release_all_keys();
    g_mfb.osk_visible = (uint8_t)visible;
    g_mfb.input_release_guard = 1;
    if (!visible) {
        g_mfb.osk_shift = 0;
        g_mfb.osk_ctrl = 0;
        g_mfb.osk_alt = 0;
    }
    r36sx_pico286_debug_log("minifb: osk %s", visible ? "open" : "close");
}

static void r36sx_osk_move(int dx, int dy)
{
    int row_count = (int)R36SX_PICO286_ARRAY_COUNT(g_osk_rows);
    int row = (int)g_mfb.osk_row + dy;
    int col = (int)g_mfb.osk_col + dx;

    if (row < 0) {
        row = row_count - 1;
    } else if (row >= row_count) {
        row = 0;
    }

    if (dy != 0) {
        int max_col = (int)g_osk_row_counts[row] - 1;
        if (col > max_col) {
            col = max_col;
        }
    } else {
        int count = (int)g_osk_row_counts[row];
        if (col < 0) {
            col = count - 1;
        } else if (col >= count) {
            col = 0;
        }
    }

    g_mfb.osk_row = (uint8_t)row;
    g_mfb.osk_col = (uint8_t)col;
}

static const struct r36sx_osk_key *r36sx_osk_current_key(void)
{
    if (g_mfb.osk_row >= R36SX_PICO286_ARRAY_COUNT(g_osk_rows)) {
        g_mfb.osk_row = 0;
    }
    if (g_mfb.osk_col >= g_osk_row_counts[g_mfb.osk_row]) {
        g_mfb.osk_col = 0;
    }
    return &g_osk_rows[g_mfb.osk_row][g_mfb.osk_col];
}

static void r36sx_osk_emit_key(uint16_t keycode, int force_shift)
{
    int use_shift = force_shift || g_mfb.osk_shift;
    int use_ctrl = g_mfb.osk_ctrl;
    int use_alt = g_mfb.osk_alt;

    if (use_ctrl) {
        HandleInput(R36SX_OSK_VK_CONTROL, 1);
    }
    if (use_alt) {
        HandleInput(R36SX_OSK_VK_MENU, 1);
    }
    if (use_shift) {
        HandleInput(R36SX_OSK_VK_SHIFT, 1);
    }
    HandleInput(keycode, 1);
    HandleInput(keycode, 0);
    if (use_shift) {
        HandleInput(R36SX_OSK_VK_SHIFT, 0);
    }
    if (use_alt) {
        HandleInput(R36SX_OSK_VK_MENU, 0);
        g_mfb.osk_alt = 0;
    }
    if (use_ctrl) {
        HandleInput(R36SX_OSK_VK_CONTROL, 0);
        g_mfb.osk_ctrl = 0;
    }
}

static void r36sx_osk_activate_current(void)
{
    const struct r36sx_osk_key *key = r36sx_osk_current_key();

    if ((key->flags & R36SX_OSK_FLAG_CLOSE) != 0) {
        r36sx_osk_set_visible(0);
        return;
    }
    if ((key->flags & R36SX_OSK_FLAG_SHIFT_MOD) != 0) {
        g_mfb.osk_shift ^= 1u;
        return;
    }
    if ((key->flags & R36SX_OSK_FLAG_CTRL_MOD) != 0) {
        g_mfb.osk_ctrl ^= 1u;
        return;
    }
    if ((key->flags & R36SX_OSK_FLAG_ALT_MOD) != 0) {
        g_mfb.osk_alt ^= 1u;
        return;
    }
    if (key->keycode != 0) {
        r36sx_osk_emit_key(key->keycode,
                           (key->flags & R36SX_OSK_FLAG_SHIFTED) != 0);
    }
}

static void r36sx_osk_handle_buttons(uint32_t pressed)
{
    if ((pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
        r36sx_osk_move(-1, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_RIGHT) != 0) {
        r36sx_osk_move(1, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_UP) != 0) {
        r36sx_osk_move(0, -1);
    }
    if ((pressed & R36SX_RKGAME_KEY_DOWN) != 0) {
        r36sx_osk_move(0, 1);
    }
    if ((pressed & (R36SX_RKGAME_KEY_B | R36SX_RKGAME_KEY_SELECT)) != 0) {
        r36sx_osk_set_visible(0);
        return;
    }
    if ((pressed & R36SX_RKGAME_KEY_X) != 0) {
        g_mfb.osk_shift ^= 1u;
    }
    if ((pressed & R36SX_RKGAME_KEY_Y) != 0) {
        r36sx_osk_emit_key(R36SX_OSK_VK_BACK, 0);
    }
    if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START)) != 0) {
        r36sx_osk_activate_current();
    }
}

static void r36sx_osk_draw_key(const struct r36sx_osk_key *key, int x, int y,
                               int selected)
{
    uint16_t bg = r36sx_mfb_rgb565(32, 42, 54);
    uint16_t fg = r36sx_mfb_rgb565(235, 242, 232);
    uint16_t border = r36sx_mfb_rgb565(110, 132, 150);
    int active_modifier =
        ((key->flags & R36SX_OSK_FLAG_SHIFT_MOD) && g_mfb.osk_shift) ||
        ((key->flags & R36SX_OSK_FLAG_CTRL_MOD) && g_mfb.osk_ctrl) ||
        ((key->flags & R36SX_OSK_FLAG_ALT_MOD) && g_mfb.osk_alt);

    if (active_modifier) {
        bg = r36sx_mfb_rgb565(40, 120, 78);
        border = r36sx_mfb_rgb565(120, 235, 160);
    }
    if (selected) {
        bg = r36sx_mfb_rgb565(238, 172, 62);
        fg = r36sx_mfb_rgb565(18, 22, 26);
        border = r36sx_mfb_rgb565(255, 238, 168);
    }

    r36sx_mfb_fill_rect(x, y, R36SX_OSK_KEY_W, R36SX_OSK_KEY_H, bg);
    r36sx_mfb_stroke_rect(x, y, R36SX_OSK_KEY_W, R36SX_OSK_KEY_H, border);
    {
        int text_w = r36sx_osk_text_width(key->label, R36SX_OSK_TEXT_SCALE);
        int text_x = x + (R36SX_OSK_KEY_W - text_w) / 2;
        int text_y = y + (R36SX_OSK_KEY_H - 7 * R36SX_OSK_TEXT_SCALE) / 2;
        r36sx_osk_draw_text(text_x, text_y, key->label, fg,
                            R36SX_OSK_TEXT_SCALE);
    }
}

static void r36sx_osk_draw(void)
{
    const int panel_x = 8;
    const int panel_w = g_mfb.width - 16;
    const int panel_y = r36sx_osk_panel_y();
    const uint16_t panel = r36sx_mfb_rgb565(12, 18, 24);
    const uint16_t header = r36sx_mfb_rgb565(24, 54, 70);
    const uint16_t border = r36sx_mfb_rgb565(160, 192, 204);
    const uint16_t text = r36sx_mfb_rgb565(228, 236, 224);

    if (!g_mfb.osk_visible) {
        return;
    }

    r36sx_mfb_fill_rect(panel_x, panel_y, panel_w, R36SX_OSK_PANEL_H, panel);
    r36sx_mfb_stroke_rect(panel_x, panel_y, panel_w, R36SX_OSK_PANEL_H, border);
    r36sx_mfb_fill_rect(panel_x + 2, panel_y + 2, panel_w - 4, 12, header);
    r36sx_osk_draw_text(panel_x + 10, panel_y + 5,
                        "FN KBD  D-PAD MOVE  A/START TYPE  B/SELECT CLOSE",
                        text, 1);

    for (size_t row = 0; row < R36SX_PICO286_ARRAY_COUNT(g_osk_rows); row++) {
        int count = g_osk_row_counts[row];
        int row_w = count * R36SX_OSK_KEY_W +
                    (count - 1) * R36SX_OSK_KEY_GAP;
        int x = panel_x + (panel_w - row_w) / 2;
        int y = panel_y + 18 + (int)row * (R36SX_OSK_KEY_H +
                                           R36SX_OSK_KEY_GAP);
        for (int col = 0; col < count; col++) {
            r36sx_osk_draw_key(&g_osk_rows[row][col], x, y,
                               row == g_mfb.osk_row &&
                               col == g_mfb.osk_col);
            x += R36SX_OSK_KEY_W + R36SX_OSK_KEY_GAP;
        }
    }
}

static void r36sx_mfb_draw_disk_led(uint32_t now_ms)
{
    int cx;
    int cy;
    const int radius = R36SX_PICO286_DISK_LED_RADIUS;
    const int outer_radius = radius + 2;
    const uint16_t red = 0xf800u;
    const uint16_t dark_red = 0x6000u;
    const uint16_t outline = 0x0000u;

    if ((int32_t)(g_disk_activity_until_ms - now_ms) <= 0) {
        return;
    }
    if (((now_ms / R36SX_PICO286_DISK_LED_BLINK_MS) & 1u) == 0u) {
        return;
    }

    cx = g_mfb.width - radius - 12;
    cy = (g_mfb.osk_visible ? r36sx_osk_panel_y() : g_mfb.height) -
         radius - 12;
    for (int y = -outer_radius; y <= outer_radius; y++) {
        int py = cy + y;
        if (py < 0 || py >= g_mfb.height) {
            continue;
        }
        for (int x = -outer_radius; x <= outer_radius; x++) {
            int px = cx + x;
            int dist2 = x * x + y * y;
            if (px < 0 || px >= g_mfb.width) {
                continue;
            }
            if (dist2 <= radius * radius) {
                g_mfb.frame[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    dist2 <= (radius - 3) * (radius - 3) ? red : dark_red;
            } else if (dist2 <= outer_radius * outer_radius) {
                g_mfb.frame[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    outline;
            }
        }
    }
}

static int r36sx_mfb_load_driver(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    for (size_t i = 0; i < R36SX_PICO286_ARRAY_COUNT(paths); i++) {
        g_mfb.handle = dlopen(paths[i], RTLD_NOW);
        if (g_mfb.handle) {
            r36sx_pico286_debug_log("minifb: dlopen ok path=%s handle=%p",
                                    paths[i], g_mfb.handle);
            break;
        }
        r36sx_pico286_debug_log("minifb: dlopen failed path=%s err=%s",
                                paths[i], dlerror());
    }
    if (!g_mfb.handle) {
        r36sx_pico286_debug_log("minifb: all driver.so dlopen attempts failed");
        return -1;
    }

    g_mfb.setting =
        (video_driver_setting_fn)dlsym(g_mfb.handle, "video_driver_setting");
    g_mfb.init =
        (video_drivers_init_fn)dlsym(g_mfb.handle, "video_drivers_init");
    g_mfb.disp_frame =
        (video_driver_disp_frame_fn)dlsym(g_mfb.handle,
                                          "video_driver_disp_frame");
    g_mfb.deinit =
        (video_driver_deinit_fn)dlsym(g_mfb.handle, "video_driver_deinit");
    g_mfb.cube_ioctl = (cube_ioctl_fn)dlsym(g_mfb.handle, "cube_ioctl");

    if (!g_mfb.setting || !g_mfb.init || !g_mfb.disp_frame ||
        !g_mfb.deinit) {
        r36sx_pico286_debug_log(
            "minifb: driver symbols missing setting=%p init=%p disp=%p deinit=%p ioctl=%p",
            g_mfb.setting, g_mfb.init, g_mfb.disp_frame, g_mfb.deinit,
            g_mfb.cube_ioctl);
        return -1;
    }
    r36sx_pico286_debug_log(
        "minifb: driver symbols ok setting=%p init=%p disp=%p deinit=%p ioctl=%p",
        g_mfb.setting, g_mfb.init, g_mfb.disp_frame, g_mfb.deinit,
        g_mfb.cube_ioctl);
    return 0;
}

static uint32_t r36sx_mfb_read_raw_keys(void)
{
    uint32_t state = 0;

    if (!g_mfb.cube_ioctl) {
        return 0;
    }
    if (g_mfb.cube_key_addr == 0) {
        uint32_t addr = 0;
        int rc = g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_mfb.cube_key_addr = addr;
        g_mfb.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
        r36sx_pico286_debug_log("minifb: joy shm rc=%d addr=0x%08x", rc, addr);
    }
    if (!g_mfb.cube_key_mem) {
        return 0;
    }

    g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);
    (void)state;
    return g_mfb.cube_key_mem[0] | g_mfb.cube_key_mem[1];
}

static int r36sx_mfb_poll_input(void)
{
    static const struct r36sx_key_map keys[] = {
        { R36SX_RKGAME_KEY_LEFT, 37 },
        { R36SX_RKGAME_KEY_UP, 38 },
        { R36SX_RKGAME_KEY_RIGHT, 39 },
        { R36SX_RKGAME_KEY_DOWN, 40 },
        { R36SX_RKGAME_KEY_A, 13 },
        { R36SX_RKGAME_KEY_B, 27 },
        { R36SX_RKGAME_KEY_X, 32 },
        { R36SX_RKGAME_KEY_Y, 17 },
        { R36SX_RKGAME_KEY_L, 18 },
        { R36SX_RKGAME_KEY_R, 16 },
        { R36SX_RKGAME_KEY_L2, 112 },
        { R36SX_RKGAME_KEY_R2, 113 },
        { R36SX_RKGAME_KEY_START, 13 },
        { R36SX_RKGAME_KEY_SELECT, 27 }
    };
    uint8_t new_down[256];
    uint32_t raw = r36sx_mfb_read_raw_keys();
    uint32_t pressed = raw & ~g_mfb.last_raw_keys;

    if (raw != g_mfb.last_raw_keys) {
        r36sx_pico286_debug_log("minifb: raw keys=0x%08x", raw);
    }

    if ((raw & (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) ==
        (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) {
        r36sx_pico286_debug_log("minifb: Select+Start exit requested");
        return -1;
    }

    if (g_mfb.input_release_guard) {
        if (raw == 0) {
            g_mfb.input_release_guard = 0;
        }
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if ((pressed & R36SX_RKGAME_KEY_FN) != 0) {
        r36sx_osk_set_visible(!g_mfb.osk_visible);
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if (g_mfb.osk_visible) {
        r36sx_osk_handle_buttons(pressed);
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    memset(new_down, 0, sizeof(new_down));
    for (size_t i = 0; i < R36SX_PICO286_ARRAY_COUNT(keys); i++) {
        if ((raw & keys[i].raw_mask) != 0) {
            new_down[keys[i].keycode] = 1;
        }
    }

    for (size_t code = 0; code < sizeof(new_down); code++) {
        if (new_down[code] != g_mfb.key_down[code]) {
            HandleInput((unsigned int)code, new_down[code] != 0);
            g_mfb.key_down[code] = new_down[code];
        }
    }
    g_mfb.last_raw_keys = raw;
    return 0;
}

int mfb_open(const char *name, int width, int height, int scale)
{
    (void)scale;
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "?");
    }
    r36sx_pico286_debug_log("minifb: open name=%s size=%dx%d scale=%d cwd=%s",
                            name ? name : "(null)", width, height, scale, cwd);

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    memset(&g_mfb, 0, sizeof(g_mfb));
    g_mfb.width = width;
    g_mfb.height = height;
    g_mfb.stride = width * R36SX_RGB565_BYTES_PER_PIXEL;
    g_mfb.frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                     sizeof(g_mfb.frame[0]));
    if (!g_mfb.frame) {
        r36sx_pico286_debug_log("minifb: framebuffer allocation failed");
        return 0;
    }
    if (r36sx_mfb_load_driver() != 0) {
        mfb_close();
        return 0;
    }

    {
        int cfg[5] = {
            R36SX_DRIVER_SETTING_0,
            R36SX_DRIVER_SETTING_1,
            R36SX_DRIVER_SETTING_2,
            R36SX_DRIVER_SETTING_WIDTH,
            R36SX_DRIVER_SETTING_HEIGHT
        };
        r36sx_pico286_debug_log("minifb: video_driver_setting cfg={%d,%d,%d,%d,%d}",
                                cfg[0], cfg[1], cfg[2], cfg[3], cfg[4]);
        g_mfb.setting(cfg);
    }
    {
        int rc = g_mfb.init();
        r36sx_pico286_debug_log("minifb: video_drivers_init rc=%d", rc);
        if (rc < 0) {
            r36sx_pico286_debug_log("minifb: video_drivers_init failed");
            mfb_close();
            return 0;
        }
    }

    if (g_mfb.cube_ioctl) {
        uint32_t joy = 0;
        int rc = g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &joy, 0, 0);
        g_mfb.cube_key_addr = joy;
        g_mfb.cube_key_mem = (volatile uint32_t *)(uintptr_t)joy;
        r36sx_pico286_debug_log("minifb: initial joy shm rc=%d addr=0x%08x", rc, joy);
    } else {
        r36sx_pico286_debug_log("minifb: cube_ioctl unavailable, input disabled");
    }

    if (!g_mfb.disp_frame) {
        r36sx_pico286_debug_log("minifb: no display frame function after init");
        mfb_close();
        return 0;
    }

    g_mfb.active = 1;
    g_mfb.last_present_us = 0;
    r36sx_pico286_debug_log("minifb: opened");
    return 1;
}

int mfb_update(void *buffer, int fps_limit)
{
    uint64_t now;
    uint32_t *src = (uint32_t *)buffer;
    int content_h;
    (void)fps_limit;

    if (!g_mfb.active || !g_mfb.disp_frame || !src) {
        r36sx_pico286_debug_log("minifb: update rejected active=%d disp=%p src=%p",
                                g_mfb.active, g_mfb.disp_frame, src);
        return -1;
    }
    if (r36sx_mfb_poll_input() != 0) {
        return -1;
    }

    now = r36sx_mfb_now_us();
    if (g_mfb.last_present_us != 0 &&
        now - g_mfb.last_present_us < R36SX_PICO286_FRAME_USEC) {
        usleep(1000);
        return 0;
    }

    content_h = g_mfb.osk_visible ? r36sx_osk_panel_y() : g_mfb.height;
    for (int y = 0; y < content_h; y++) {
        uint16_t *dst_row = g_mfb.frame + (size_t)y * (size_t)g_mfb.width;

        if (g_mfb.osk_visible) {
            int start = y * g_mfb.height;
            int end = (y + 1) * g_mfb.height;
            int sy0 = start / content_h;
            int sy1 = (end - 1) / content_h;
            int weight0;
            int weight1;
            const uint32_t *src_row0;
            const uint32_t *src_row1;

            if (sy1 >= g_mfb.height) {
                sy1 = g_mfb.height - 1;
            }
            weight0 = (sy0 + 1) * content_h - start;
            if (weight0 > g_mfb.height) {
                weight0 = g_mfb.height;
            }
            if (sy0 == sy1) {
                weight1 = 0;
                weight0 = g_mfb.height;
            } else {
                weight1 = g_mfb.height - weight0;
            }

            src_row0 = src + (size_t)sy0 * (size_t)g_mfb.width;
            src_row1 = src + (size_t)sy1 * (size_t)g_mfb.width;
            if (g_mfb.height == 480 && content_h == 384) {
                uint32_t w0 = (uint32_t)(weight0 / 96);
                uint32_t w1 = (uint32_t)(weight1 / 96);
                for (int x = 0; x < g_mfb.width; x++) {
                    uint32_t c0 = src_row0[x];
                    uint32_t c1 = src_row1[x];
                    uint32_t r = (((c0 >> 16) & 0xffu) * w0 +
                                  ((c1 >> 16) & 0xffu) * w1 + 2u) / 5u;
                    uint32_t g = (((c0 >> 8) & 0xffu) * w0 +
                                  ((c1 >> 8) & 0xffu) * w1 + 2u) / 5u;
                    uint32_t b = ((c0 & 0xffu) * w0 +
                                  (c1 & 0xffu) * w1 + 2u) / 5u;
                    dst_row[x] = (uint16_t)(((r & 0xf8u) << 8) |
                                            ((g & 0xfcu) << 3) | (b >> 3));
                }
            } else {
                for (int x = 0; x < g_mfb.width; x++) {
                    uint32_t c0 = src_row0[x];
                    uint32_t c1 = src_row1[x];
                    uint32_t r = (((c0 >> 16) & 0xffu) *
                                  (uint32_t)weight0 +
                                  ((c1 >> 16) & 0xffu) *
                                  (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    uint32_t g = (((c0 >> 8) & 0xffu) *
                                  (uint32_t)weight0 +
                                  ((c1 >> 8) & 0xffu) *
                                  (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    uint32_t b = ((c0 & 0xffu) * (uint32_t)weight0 +
                                  (c1 & 0xffu) * (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    dst_row[x] = (uint16_t)(((r & 0xf8u) << 8) |
                                            ((g & 0xfcu) << 3) | (b >> 3));
                }
            }
        } else {
            uint32_t *src_row = src + (size_t)y * (size_t)g_mfb.width;
            for (int x = 0; x < g_mfb.width; x++) {
                dst_row[x] = r36sx_mfb_rgb888_to_rgb565(src_row[x]);
            }
        }
    }
    if (content_h < g_mfb.height) {
        r36sx_mfb_fill_rect(0, content_h, g_mfb.width,
                            g_mfb.height - content_h,
                            r36sx_mfb_rgb565(0, 0, 0));
    }
    r36sx_mfb_draw_disk_led((uint32_t)(now / 1000ull));
    r36sx_osk_draw();
    g_mfb.disp_frame(g_mfb.frame, g_mfb.width, g_mfb.height, g_mfb.stride);
    g_mfb.last_present_us = now;
    return 0;
}

void mfb_set_pallete_array(const uint32_t *new_palette, uint8_t start,
                           uint8_t count)
{
    if (!new_palette) {
        return;
    }
    for (uint8_t i = 0; i < count; i++) {
        g_palette[(uint8_t)(start + i)] = new_palette[i];
    }
}

void mfb_set_pallete(const uint8_t color_index, const uint32_t color)
{
    g_palette[color_index] = color;
}

void mfb_close(void)
{
    r36sx_pico286_debug_log("minifb: close active=%d handle=%p frame=%p",
                            g_mfb.active, g_mfb.handle, g_mfb.frame);
    if (g_mfb.active && g_mfb.deinit) {
        g_mfb.deinit();
    }
    if (g_mfb.frame) {
        free(g_mfb.frame);
    }
    if (g_mfb.handle) {
        dlclose(g_mfb.handle);
    }
    memset(&g_mfb, 0, sizeof(g_mfb));
}

char *mfb_keystatus(void)
{
    return (char *)g_mfb.key_down;
}
