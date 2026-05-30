#include "r36sx_screen_keyboard.h"

#include <stddef.h>
#include <time.h>

#define R36SX_OSK_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_OSK_KEY_W 47
#define R36SX_OSK_COMPACT_KEY_W 39
#define R36SX_OSK_KEY_H 13
#define R36SX_OSK_KEY_GAP 2
#define R36SX_OSK_TEXT_SCALE 1
#define R36SX_OSK_INNER_PAD 1
#define R36SX_OSK_HEADER_H 16
#define R36SX_OSK_CURSOR_KEY_W 28
#define R36SX_OSK_CURSOR_GAP 2
#define R36SX_OSK_CURSOR_BLOCK_GAP 10
#define R36SX_OSK_CURSOR_BLOCK_COLS 3
#define R36SX_OSK_CURSOR_BLOCK_ROWS 2
#define R36SX_OSK_CURSOR_BLOCK_Y_ROW 3
#define R36SX_OSK_NAV_MASK \
    (R36SX_RKGAME_KEY_LEFT | R36SX_RKGAME_KEY_RIGHT | \
     R36SX_RKGAME_KEY_UP | R36SX_RKGAME_KEY_DOWN)
#define R36SX_OSK_KEY_REPEAT_MASK \
    (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START | R36SX_RKGAME_KEY_B | \
     R36SX_RKGAME_KEY_X | R36SX_RKGAME_KEY_Y)
#define R36SX_OSK_NAV_REPEAT_DELAY_US 280000ull
#define R36SX_OSK_NAV_REPEAT_INTERVAL_US 85000ull
#define R36SX_OSK_KEY_REPEAT_DELAY_US 420000ull
#define R36SX_OSK_KEY_REPEAT_INTERVAL_US 70000ull
#define R36SX_OSK_CURSOR_BLOCK_W \
    (R36SX_OSK_CURSOR_BLOCK_COLS * R36SX_OSK_CURSOR_KEY_W + \
     (R36SX_OSK_CURSOR_BLOCK_COLS - 1) * R36SX_OSK_CURSOR_GAP)

#define R36SX_OSK_FLAG_SHIFTED 0x01u
#define R36SX_OSK_FLAG_SHIFT_MOD 0x02u
#define R36SX_OSK_FLAG_CTRL_MOD 0x04u
#define R36SX_OSK_FLAG_ALT_MOD 0x08u
#define R36SX_OSK_FLAG_CLOSE 0x10u

#define R36SX_OSK_ZONE_MAIN 0u
#define R36SX_OSK_ZONE_CURSOR 1u
#define R36SX_OSK_NO_CURSOR_KEY (-1)
#define R36SX_OSK_LABEL_LEFT "\x11"
#define R36SX_OSK_LABEL_UP "\x12"
#define R36SX_OSK_LABEL_RIGHT "\x13"
#define R36SX_OSK_LABEL_DOWN "\x14"

struct r36sx_osk_key {
    const char *label;
    uint16_t keycode;
    uint8_t flags;
};

static const struct r36sx_osk_key g_osk_row0[] = {
    { "ESC", R36SX_SCREEN_KEY_ESCAPE, 0 },
    { "1", '1', 0 }, { "2", '2', 0 }, { "3", '3', 0 },
    { "4", '4', 0 }, { "5", '5', 0 }, { "6", '6', 0 },
    { "7", '7', 0 }, { "8", '8', 0 }, { "9", '9', 0 },
    { "0", '0', 0 }, { "-", R36SX_SCREEN_KEY_OEM_MINUS, 0 },
    { "BS", R36SX_SCREEN_KEY_BACK, 0 }
};

static const struct r36sx_osk_key g_osk_row1[] = {
    { "TAB", R36SX_SCREEN_KEY_TAB, 0 },
    { "Q", 'Q', 0 }, { "W", 'W', 0 }, { "E", 'E', 0 },
    { "R", 'R', 0 }, { "T", 'T', 0 }, { "Y", 'Y', 0 },
    { "U", 'U', 0 }, { "I", 'I', 0 }, { "O", 'O', 0 },
    { "P", 'P', 0 }, { "\\", R36SX_SCREEN_KEY_OEM_5, 0 },
    { "=", R36SX_SCREEN_KEY_OEM_PLUS, 0 }
};

static const struct r36sx_osk_key g_osk_row2[] = {
    { "CTRL", R36SX_SCREEN_KEY_CONTROL, R36SX_OSK_FLAG_CTRL_MOD },
    { "A", 'A', 0 }, { "S", 'S', 0 }, { "D", 'D', 0 },
    { "F", 'F', 0 }, { "G", 'G', 0 }, { "H", 'H', 0 },
    { "J", 'J', 0 }, { "K", 'K', 0 }, { "L", 'L', 0 },
    { ":", R36SX_SCREEN_KEY_OEM_1, R36SX_OSK_FLAG_SHIFTED },
    { "'", R36SX_SCREEN_KEY_OEM_7, 0 },
    { "ENT", R36SX_SCREEN_KEY_RETURN, 0 }
};

static const struct r36sx_osk_key g_osk_row3[] = {
    { "SHF", R36SX_SCREEN_KEY_SHIFT, R36SX_OSK_FLAG_SHIFT_MOD },
    { "Z", 'Z', 0 }, { "X", 'X', 0 }, { "C", 'C', 0 },
    { "V", 'V', 0 }, { "B", 'B', 0 }, { "N", 'N', 0 },
    { "M", 'M', 0 }, { ",", R36SX_SCREEN_KEY_OEM_COMMA, 0 },
    { ".", R36SX_SCREEN_KEY_OEM_PERIOD, 0 },
    { "/", R36SX_SCREEN_KEY_OEM_2, 0 },
    { "SPC", R36SX_SCREEN_KEY_SPACE, 0 }
};

static const struct r36sx_osk_key g_osk_row4[] = {
    { "F1", R36SX_SCREEN_KEY_F1 + 0, 0 },
    { "F2", R36SX_SCREEN_KEY_F1 + 1, 0 },
    { "F3", R36SX_SCREEN_KEY_F1 + 2, 0 },
    { "F4", R36SX_SCREEN_KEY_F1 + 3, 0 },
    { "F5", R36SX_SCREEN_KEY_F1 + 4, 0 },
    { "F6", R36SX_SCREEN_KEY_F1 + 5, 0 },
    { "F7", R36SX_SCREEN_KEY_F1 + 6, 0 },
    { "F8", R36SX_SCREEN_KEY_F1 + 7, 0 },
    { "F9", R36SX_SCREEN_KEY_F1 + 8, 0 },
    { "F10", R36SX_SCREEN_KEY_F1 + 9, 0 },
    { "ALT", R36SX_SCREEN_KEY_MENU, R36SX_OSK_FLAG_ALT_MOD },
    { "DEL", R36SX_SCREEN_KEY_DELETE, 0 },
    { "CLOSE", 0, R36SX_OSK_FLAG_CLOSE }
};

static const struct r36sx_osk_key g_osk_symbol_row0[] = {
    { "ESC", R36SX_SCREEN_KEY_ESCAPE, 0 },
    { "!", '1', R36SX_OSK_FLAG_SHIFTED },
    { "@", '2', R36SX_OSK_FLAG_SHIFTED },
    { "#", '3', R36SX_OSK_FLAG_SHIFTED },
    { "$", '4', R36SX_OSK_FLAG_SHIFTED },
    { "%", '5', R36SX_OSK_FLAG_SHIFTED },
    { "^", '6', R36SX_OSK_FLAG_SHIFTED },
    { "&", '7', R36SX_OSK_FLAG_SHIFTED },
    { "*", '8', R36SX_OSK_FLAG_SHIFTED },
    { "(", '9', R36SX_OSK_FLAG_SHIFTED },
    { ")", '0', R36SX_OSK_FLAG_SHIFTED },
    { "_", R36SX_SCREEN_KEY_OEM_MINUS, R36SX_OSK_FLAG_SHIFTED },
    { "BS", R36SX_SCREEN_KEY_BACK, 0 }
};

static const struct r36sx_osk_key g_osk_symbol_row1[] = {
    { "TAB", R36SX_SCREEN_KEY_TAB, 0 },
    { "~", R36SX_SCREEN_KEY_OEM_3, R36SX_OSK_FLAG_SHIFTED },
    { "`", R36SX_SCREEN_KEY_OEM_3, 0 },
    { "[", R36SX_SCREEN_KEY_OEM_4, 0 },
    { "]", R36SX_SCREEN_KEY_OEM_6, 0 },
    { "{", R36SX_SCREEN_KEY_OEM_4, R36SX_OSK_FLAG_SHIFTED },
    { "}", R36SX_SCREEN_KEY_OEM_6, R36SX_OSK_FLAG_SHIFTED },
    { "+", R36SX_SCREEN_KEY_OEM_PLUS, R36SX_OSK_FLAG_SHIFTED },
    { "=", R36SX_SCREEN_KEY_OEM_PLUS, 0 },
    { "|", R36SX_SCREEN_KEY_OEM_5, R36SX_OSK_FLAG_SHIFTED },
    { "\\", R36SX_SCREEN_KEY_OEM_5, 0 },
    { "?", R36SX_SCREEN_KEY_OEM_2, R36SX_OSK_FLAG_SHIFTED },
    { "/", R36SX_SCREEN_KEY_OEM_2, 0 }
};

static const struct r36sx_osk_key g_osk_symbol_row2[] = {
    { "CTRL", R36SX_SCREEN_KEY_CONTROL, R36SX_OSK_FLAG_CTRL_MOD },
    { "<", R36SX_SCREEN_KEY_OEM_COMMA, R36SX_OSK_FLAG_SHIFTED },
    { ">", R36SX_SCREEN_KEY_OEM_PERIOD, R36SX_OSK_FLAG_SHIFTED },
    { ",", R36SX_SCREEN_KEY_OEM_COMMA, 0 },
    { ".", R36SX_SCREEN_KEY_OEM_PERIOD, 0 },
    { ";", R36SX_SCREEN_KEY_OEM_1, 0 },
    { ":", R36SX_SCREEN_KEY_OEM_1, R36SX_OSK_FLAG_SHIFTED },
    { "'", R36SX_SCREEN_KEY_OEM_7, 0 },
    { "\"", R36SX_SCREEN_KEY_OEM_7, R36SX_OSK_FLAG_SHIFTED },
    { "-", R36SX_SCREEN_KEY_OEM_MINUS, 0 },
    { "ENT", R36SX_SCREEN_KEY_RETURN, 0 }
};

static const struct r36sx_osk_key g_osk_symbol_row3[] = {
    { "SHF", R36SX_SCREEN_KEY_SHIFT, R36SX_OSK_FLAG_SHIFT_MOD },
    { "#", '3', R36SX_OSK_FLAG_SHIFTED },
    { "$", '4', R36SX_OSK_FLAG_SHIFTED },
    { "%", '5', R36SX_OSK_FLAG_SHIFTED },
    { "^", '6', R36SX_OSK_FLAG_SHIFTED },
    { "&", '7', R36SX_OSK_FLAG_SHIFTED },
    { "*", '8', R36SX_OSK_FLAG_SHIFTED },
    { "<", R36SX_SCREEN_KEY_OEM_COMMA, R36SX_OSK_FLAG_SHIFTED },
    { ">", R36SX_SCREEN_KEY_OEM_PERIOD, R36SX_OSK_FLAG_SHIFTED },
    { "~", R36SX_SCREEN_KEY_OEM_3, R36SX_OSK_FLAG_SHIFTED },
    { "`", R36SX_SCREEN_KEY_OEM_3, 0 },
    { "SPC", R36SX_SCREEN_KEY_SPACE, 0 }
};

static const struct r36sx_osk_key g_osk_symbol_row4[] = {
    { "F1", R36SX_SCREEN_KEY_F1 + 0, 0 },
    { "F2", R36SX_SCREEN_KEY_F1 + 1, 0 },
    { "F3", R36SX_SCREEN_KEY_F1 + 2, 0 },
    { "F4", R36SX_SCREEN_KEY_F1 + 3, 0 },
    { "F5", R36SX_SCREEN_KEY_F1 + 4, 0 },
    { "F6", R36SX_SCREEN_KEY_F1 + 5, 0 },
    { "F7", R36SX_SCREEN_KEY_F1 + 6, 0 },
    { "F8", R36SX_SCREEN_KEY_F1 + 7, 0 },
    { "F9", R36SX_SCREEN_KEY_F1 + 8, 0 },
    { "F10", R36SX_SCREEN_KEY_F1 + 9, 0 },
    { "ALT", R36SX_SCREEN_KEY_MENU, R36SX_OSK_FLAG_ALT_MOD },
    { "DEL", R36SX_SCREEN_KEY_DELETE, 0 },
    { "CLOSE", 0, R36SX_OSK_FLAG_CLOSE }
};

static const struct r36sx_osk_key g_osk_cursor_keys[] = {
    { R36SX_OSK_LABEL_UP, R36SX_SCREEN_KEY_UP, 0 },
    { R36SX_OSK_LABEL_LEFT, R36SX_SCREEN_KEY_LEFT, 0 },
    { R36SX_OSK_LABEL_DOWN, R36SX_SCREEN_KEY_DOWN, 0 },
    { R36SX_OSK_LABEL_RIGHT, R36SX_SCREEN_KEY_RIGHT, 0 },
};

static const int8_t g_osk_cursor_grid
    [R36SX_OSK_CURSOR_BLOCK_ROWS][R36SX_OSK_CURSOR_BLOCK_COLS] = {
    { R36SX_OSK_NO_CURSOR_KEY, 0, R36SX_OSK_NO_CURSOR_KEY },
    { 1, 2, 3 },
};

static const struct r36sx_osk_key *const g_osk_rows[] = {
    g_osk_row0, g_osk_row1, g_osk_row2, g_osk_row3, g_osk_row4
};

static const struct r36sx_osk_key *const g_osk_symbol_rows[] = {
    g_osk_symbol_row0, g_osk_symbol_row1, g_osk_symbol_row2,
    g_osk_symbol_row3, g_osk_symbol_row4
};

static const uint8_t g_osk_row_counts[] = {
    R36SX_OSK_ARRAY_COUNT(g_osk_row0),
    R36SX_OSK_ARRAY_COUNT(g_osk_row1),
    R36SX_OSK_ARRAY_COUNT(g_osk_row2),
    R36SX_OSK_ARRAY_COUNT(g_osk_row3),
    R36SX_OSK_ARRAY_COUNT(g_osk_row4)
};

static const uint8_t g_osk_symbol_row_counts[] = {
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row0),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row1),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row2),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row3),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row4)
};

static const uint8_t *active_row_counts(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->symbol_mode ? g_osk_symbol_row_counts :
        g_osk_row_counts;
}

static const struct r36sx_osk_key *const *active_rows(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->symbol_mode ? g_osk_symbol_rows : g_osk_rows;
}

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r & 0xf8u) << 8) |
                      ((uint16_t)(g & 0xfcu) << 3) |
                      ((uint16_t)b >> 3));
}

static uint64_t now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static void fill_rect(uint16_t *frame, int width, int height, int stride,
                      int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0 || !frame) {
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
    if (x + w > width) {
        w = width - x;
    }
    if (y + h > height) {
        h = height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = frame + (size_t)(y + row) * (size_t)stride +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void stroke_rect(uint16_t *frame, int width, int height, int stride,
                        int x, int y, int w, int h, uint16_t color)
{
    fill_rect(frame, width, height, stride, x, y, w, 1, color);
    fill_rect(frame, width, height, stride, x, y + h - 1, w, 1, color);
    fill_rect(frame, width, height, stride, x, y, 1, h, color);
    fill_rect(frame, width, height, stride, x + w - 1, y, 1, h, color);
}

static uint8_t glyph_row(unsigned char c, int row)
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
    static const uint8_t glyph_left[7] = {0, 4, 8, 31, 8, 4, 0};
    static const uint8_t glyph_up[7] = {0, 4, 14, 21, 4, 4, 0};
    static const uint8_t glyph_right[7] = {0, 4, 2, 31, 2, 4, 0};
    static const uint8_t glyph_down[7] = {0, 4, 4, 21, 14, 4, 0};
    const uint8_t *glyph = blank;

    if (c >= 'a' && c <= 'z') {
        c = (unsigned char)(c - ('a' - 'A'));
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
    case 0x11: glyph = glyph_left; break;
    case 0x12: glyph = glyph_up; break;
    case 0x13: glyph = glyph_right; break;
    case 0x14: glyph = glyph_down; break;
    default: break;
    }
    return glyph[row];
}

static int text_width(const char *text, int scale)
{
    int len = 0;
    while (text && text[len] != '\0') {
        len++;
    }
    return len > 0 ? (len * 6 - 1) * scale : 0;
}

static void draw_char(uint16_t *frame, int width, int height, int stride,
                      int x, int y, char c, uint16_t color, int scale)
{
    for (int row = 0; row < 7; row++) {
        uint8_t bits = glyph_row((unsigned char)c, row);
        for (int col = 0; col < 5; col++) {
            if ((bits & (uint8_t)(1u << (4 - col))) != 0) {
                fill_rect(frame, width, height, stride,
                          x + col * scale, y + row * scale,
                          scale, scale, color);
            }
        }
    }
}

static void draw_text(uint16_t *frame, int width, int height, int stride,
                      int x, int y, const char *text, uint16_t color,
                      int scale)
{
    for (int i = 0; text && text[i] != '\0'; i++) {
        draw_char(frame, width, height, stride, x + i * 6 * scale, y,
                  text[i], color, scale);
    }
}

static void emit_key(struct r36sx_screen_keyboard *keyboard,
                     r36sx_screen_keyboard_emit_fn emit,
                     void *emit_user,
                     uint16_t keycode,
                     int force_shift)
{
    int use_shift = force_shift || keyboard->shift;
    int use_ctrl = keyboard->ctrl;
    int use_alt = keyboard->alt;

    if (!emit) {
        return;
    }
    if (use_ctrl) {
        emit(emit_user, R36SX_SCREEN_KEY_CONTROL, 1);
    }
    if (use_alt) {
        emit(emit_user, R36SX_SCREEN_KEY_MENU, 1);
    }
    if (use_shift) {
        emit(emit_user, R36SX_SCREEN_KEY_SHIFT, 1);
    }
    emit(emit_user, keycode, 1);
    emit(emit_user, keycode, 0);
    if (use_shift) {
        emit(emit_user, R36SX_SCREEN_KEY_SHIFT, 0);
    }
    if (use_alt) {
        emit(emit_user, R36SX_SCREEN_KEY_MENU, 0);
        keyboard->alt = 0;
    }
    if (use_ctrl) {
        emit(emit_user, R36SX_SCREEN_KEY_CONTROL, 0);
        keyboard->ctrl = 0;
    }
}

static int key_width(const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->cursor_block ?
        R36SX_OSK_COMPACT_KEY_W : R36SX_OSK_KEY_W;
}

static int cursor_key_index(int row, int col)
{
    if (row < 0 || row >= R36SX_OSK_CURSOR_BLOCK_ROWS ||
        col < 0 || col >= R36SX_OSK_CURSOR_BLOCK_COLS) {
        return R36SX_OSK_NO_CURSOR_KEY;
    }
    return g_osk_cursor_grid[row][col];
}

static const struct r36sx_osk_key *cursor_key_at(int row, int col)
{
    int index = cursor_key_index(row, col);
    if (index < 0 ||
        index >= (int)R36SX_OSK_ARRAY_COUNT(g_osk_cursor_keys)) {
        return NULL;
    }
    return &g_osk_cursor_keys[index];
}

static void normalize_cursor_selection(struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard->cursor_block || keyboard->zone != R36SX_OSK_ZONE_CURSOR) {
        keyboard->zone = R36SX_OSK_ZONE_MAIN;
        return;
    }

    if (keyboard->row >= R36SX_OSK_CURSOR_BLOCK_ROWS) {
        keyboard->row = 0;
    }
    if (keyboard->col >= R36SX_OSK_CURSOR_BLOCK_COLS) {
        keyboard->col = 1;
    }
    if (!cursor_key_at(keyboard->row, keyboard->col)) {
        keyboard->row = 0;
        keyboard->col = 1;
    }
}

static int cursor_row_for_main_row(int row)
{
    return row >= R36SX_OSK_CURSOR_BLOCK_Y_ROW + 1 ? 1 : 0;
}

static void enter_cursor_block(struct r36sx_screen_keyboard *keyboard)
{
    keyboard->zone = R36SX_OSK_ZONE_CURSOR;
    keyboard->row = (uint8_t)cursor_row_for_main_row(keyboard->row);
    keyboard->col = 1;
}

static void leave_cursor_block(struct r36sx_screen_keyboard *keyboard)
{
    const uint8_t *counts = active_row_counts(keyboard);
    int row = R36SX_OSK_CURSOR_BLOCK_Y_ROW + (int)keyboard->row;
    int row_count = (int)R36SX_OSK_ARRAY_COUNT(g_osk_rows);

    if (row >= row_count) {
        row = row_count - 1;
    }
    keyboard->zone = R36SX_OSK_ZONE_MAIN;
    keyboard->row = (uint8_t)row;
    keyboard->col = (uint8_t)(counts[row] - 1);
}

static void move_cursor_selection(struct r36sx_screen_keyboard *keyboard,
                                  int dx, int dy)
{
    if (dy != 0) {
        int row = (int)keyboard->row + dy;
        if (row < 0) {
            row = R36SX_OSK_CURSOR_BLOCK_ROWS - 1;
        } else if (row >= R36SX_OSK_CURSOR_BLOCK_ROWS) {
            row = 0;
        }
        keyboard->row = (uint8_t)row;
        if (row == 0) {
            keyboard->col = 1;
        } else if (keyboard->col >= R36SX_OSK_CURSOR_BLOCK_COLS) {
            keyboard->col = R36SX_OSK_CURSOR_BLOCK_COLS - 1;
        }
        if (!cursor_key_at(keyboard->row, keyboard->col)) {
            keyboard->col = 1;
        }
        return;
    }

    if (dx < 0) {
        if (keyboard->row == 1 && keyboard->col > 0) {
            keyboard->col--;
        } else {
            leave_cursor_block(keyboard);
        }
    } else if (dx > 0) {
        if (keyboard->row == 1 &&
            keyboard->col + 1 < R36SX_OSK_CURSOR_BLOCK_COLS) {
            keyboard->col++;
        }
    }
}

static void move_main_selection(struct r36sx_screen_keyboard *keyboard, int dx,
                                int dy)
{
    const uint8_t *counts = active_row_counts(keyboard);
    int row_count = (int)R36SX_OSK_ARRAY_COUNT(g_osk_rows);
    int row = (int)keyboard->row + dy;
    int col = (int)keyboard->col + dx;

    if (row < 0) {
        row = row_count - 1;
    } else if (row >= row_count) {
        row = 0;
    }

    if (dy != 0) {
        int max_col = (int)counts[row] - 1;
        if (col > max_col) {
            col = max_col;
        }
    } else {
        int count = (int)counts[row];
        if (dx > 0 && keyboard->cursor_block && col >= count) {
            keyboard->row = (uint8_t)row;
            enter_cursor_block(keyboard);
            return;
        }
        if (col < 0) {
            col = count - 1;
        } else if (col >= count) {
            col = 0;
        }
    }

    keyboard->row = (uint8_t)row;
    keyboard->col = (uint8_t)col;
}

static void move_selection(struct r36sx_screen_keyboard *keyboard, int dx,
                           int dy)
{
    normalize_cursor_selection(keyboard);
    if (keyboard->zone == R36SX_OSK_ZONE_CURSOR) {
        move_cursor_selection(keyboard, dx, dy);
    } else {
        move_main_selection(keyboard, dx, dy);
    }
}

static uint32_t first_nav_button(uint32_t buttons)
{
    if ((buttons & R36SX_RKGAME_KEY_LEFT) != 0) {
        return R36SX_RKGAME_KEY_LEFT;
    }
    if ((buttons & R36SX_RKGAME_KEY_RIGHT) != 0) {
        return R36SX_RKGAME_KEY_RIGHT;
    }
    if ((buttons & R36SX_RKGAME_KEY_UP) != 0) {
        return R36SX_RKGAME_KEY_UP;
    }
    if ((buttons & R36SX_RKGAME_KEY_DOWN) != 0) {
        return R36SX_RKGAME_KEY_DOWN;
    }
    return 0;
}

static void reset_nav_repeat(struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard) {
        return;
    }
    keyboard->nav_repeat_button = 0;
    keyboard->nav_repeat_next_us = 0;
}

static void reset_key_repeat(struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard) {
        return;
    }
    keyboard->key_repeat_button = 0;
    keyboard->key_repeat_keycode = 0;
    keyboard->key_repeat_force_shift = 0;
    keyboard->key_repeat_next_us = 0;
}

static uint32_t first_key_repeat_button(uint32_t buttons)
{
    if ((buttons & R36SX_RKGAME_KEY_A) != 0) {
        return R36SX_RKGAME_KEY_A;
    }
    if ((buttons & R36SX_RKGAME_KEY_START) != 0) {
        return R36SX_RKGAME_KEY_START;
    }
    if ((buttons & R36SX_RKGAME_KEY_B) != 0) {
        return R36SX_RKGAME_KEY_B;
    }
    if ((buttons & R36SX_RKGAME_KEY_X) != 0) {
        return R36SX_RKGAME_KEY_X;
    }
    if ((buttons & R36SX_RKGAME_KEY_Y) != 0) {
        return R36SX_RKGAME_KEY_Y;
    }
    return 0;
}

static void start_key_repeat(struct r36sx_screen_keyboard *keyboard,
                             uint32_t buttons,
                             uint16_t keycode,
                             int force_shift)
{
    uint32_t repeat_button = first_key_repeat_button(buttons);

    if (!keyboard || repeat_button == 0 || keycode == 0) {
        return;
    }

    keyboard->key_repeat_button = repeat_button;
    keyboard->key_repeat_keycode = keycode;
    keyboard->key_repeat_force_shift = (uint8_t)(force_shift != 0);
    keyboard->key_repeat_next_us = now_us() + R36SX_OSK_KEY_REPEAT_DELAY_US;
}

static void handle_key_repeat(struct r36sx_screen_keyboard *keyboard,
                              uint32_t held,
                              r36sx_screen_keyboard_emit_fn emit,
                              void *emit_user)
{
    uint64_t now;

    if (!keyboard || keyboard->key_repeat_button == 0 ||
        keyboard->key_repeat_keycode == 0) {
        return;
    }
    if ((held & keyboard->key_repeat_button) == 0) {
        reset_key_repeat(keyboard);
        return;
    }

    now = now_us();
    if ((int64_t)(now - keyboard->key_repeat_next_us) < 0) {
        return;
    }

    emit_key(keyboard, emit, emit_user, keyboard->key_repeat_keycode,
             keyboard->key_repeat_force_shift != 0);
    keyboard->key_repeat_next_us = now + R36SX_OSK_KEY_REPEAT_INTERVAL_US;
}

static uint32_t nav_buttons_with_repeat(struct r36sx_screen_keyboard *keyboard,
                                        uint32_t pressed,
                                        uint32_t held)
{
    uint32_t nav_pressed = pressed & R36SX_OSK_NAV_MASK;
    uint32_t nav_held = held & R36SX_OSK_NAV_MASK;
    uint64_t now;

    if (nav_pressed != 0) {
        keyboard->nav_repeat_button = first_nav_button(nav_pressed);
        keyboard->nav_repeat_next_us =
            now_us() + R36SX_OSK_NAV_REPEAT_DELAY_US;
        return nav_pressed;
    }
    if (nav_held == 0) {
        reset_nav_repeat(keyboard);
        return 0;
    }

    nav_held = first_nav_button(nav_held);
    now = now_us();
    if (keyboard->nav_repeat_button != nav_held) {
        keyboard->nav_repeat_button = nav_held;
        keyboard->nav_repeat_next_us = now + R36SX_OSK_NAV_REPEAT_DELAY_US;
        return 0;
    }
    if ((int64_t)(now - keyboard->nav_repeat_next_us) < 0) {
        return 0;
    }

    keyboard->nav_repeat_next_us = now + R36SX_OSK_NAV_REPEAT_INTERVAL_US;
    return nav_held;
}

static void handle_navigation(struct r36sx_screen_keyboard *keyboard,
                              uint32_t buttons)
{
    if ((buttons & R36SX_RKGAME_KEY_LEFT) != 0) {
        move_selection(keyboard, -1, 0);
    }
    if ((buttons & R36SX_RKGAME_KEY_RIGHT) != 0) {
        move_selection(keyboard, 1, 0);
    }
    if ((buttons & R36SX_RKGAME_KEY_UP) != 0) {
        move_selection(keyboard, 0, -1);
    }
    if ((buttons & R36SX_RKGAME_KEY_DOWN) != 0) {
        move_selection(keyboard, 0, 1);
    }
}

static const struct r36sx_osk_key *current_key(
    struct r36sx_screen_keyboard *keyboard)
{
    const struct r36sx_osk_key *const *rows = active_rows(keyboard);
    const uint8_t *counts = active_row_counts(keyboard);

    normalize_cursor_selection(keyboard);
    if (keyboard->zone == R36SX_OSK_ZONE_CURSOR) {
        const struct r36sx_osk_key *key =
            cursor_key_at(keyboard->row, keyboard->col);
        if (key) {
            return key;
        }
        keyboard->zone = R36SX_OSK_ZONE_MAIN;
        keyboard->row = 0;
        keyboard->col = 0;
    }
    if (keyboard->row >= R36SX_OSK_ARRAY_COUNT(g_osk_rows)) {
        keyboard->row = 0;
    }
    if (keyboard->col >= counts[keyboard->row]) {
        keyboard->col = 0;
    }
    return &rows[keyboard->row][keyboard->col];
}

static void update_press_animation(struct r36sx_screen_keyboard *keyboard,
                                   uint32_t held)
{
    if (keyboard && keyboard->press_buttons != 0 &&
        (held & keyboard->press_buttons) == 0) {
        keyboard->press_buttons = 0;
    }
}

static void start_current_press_animation(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t buttons)
{
    current_key(keyboard);
    keyboard->press_zone = keyboard->zone;
    keyboard->press_row = keyboard->row;
    keyboard->press_col = keyboard->col;
    keyboard->press_buttons = buttons;
}

static int keycode_matches(uint16_t a, uint16_t b)
{
    if (a == b) {
        return 1;
    }
    if (a >= 'A' && a <= 'Z' && a + ('a' - 'A') == b) {
        return 1;
    }
    return b >= 'A' && b <= 'Z' && b + ('a' - 'A') == a;
}

static void start_keycode_press_animation(
    struct r36sx_screen_keyboard *keyboard,
    uint16_t keycode,
    uint32_t buttons)
{
    const struct r36sx_osk_key *const *rows = active_rows(keyboard);
    const uint8_t *counts = active_row_counts(keyboard);

    for (size_t row = 0; row < R36SX_OSK_ARRAY_COUNT(g_osk_rows); row++) {
        for (size_t col = 0; col < counts[row]; col++) {
            if (keycode_matches(rows[row][col].keycode, keycode)) {
                keyboard->press_zone = R36SX_OSK_ZONE_MAIN;
                keyboard->press_row = (uint8_t)row;
                keyboard->press_col = (uint8_t)col;
                keyboard->press_buttons = buttons;
                return;
            }
        }
    }

    if (!keyboard->cursor_block) {
        return;
    }
    for (int row = 0; row < R36SX_OSK_CURSOR_BLOCK_ROWS; row++) {
        for (int col = 0; col < R36SX_OSK_CURSOR_BLOCK_COLS; col++) {
            const struct r36sx_osk_key *key = cursor_key_at(row, col);
            if (key && keycode_matches(key->keycode, keycode)) {
                keyboard->press_zone = R36SX_OSK_ZONE_CURSOR;
                keyboard->press_row = (uint8_t)row;
                keyboard->press_col = (uint8_t)col;
                keyboard->press_buttons = buttons;
                return;
            }
        }
    }
}

static uint32_t activate_current(struct r36sx_screen_keyboard *keyboard,
                                 r36sx_screen_keyboard_emit_fn emit,
                                 void *emit_user,
                                 uint32_t buttons)
{
    const struct r36sx_osk_key *key = current_key(keyboard);

    start_current_press_animation(keyboard, buttons);
    if ((key->flags & R36SX_OSK_FLAG_CLOSE) != 0) {
        r36sx_screen_keyboard_set_visible(keyboard, 0);
        return R36SX_SCREEN_KEYBOARD_RESULT_CLOSED;
    }
    if ((key->flags & R36SX_OSK_FLAG_SHIFT_MOD) != 0) {
        keyboard->shift ^= 1u;
        return 0;
    }
    if ((key->flags & R36SX_OSK_FLAG_CTRL_MOD) != 0) {
        keyboard->ctrl ^= 1u;
        return 0;
    }
    if ((key->flags & R36SX_OSK_FLAG_ALT_MOD) != 0) {
        keyboard->alt ^= 1u;
        return 0;
    }
    if (key->keycode != 0) {
        emit_key(keyboard, emit, emit_user, key->keycode,
                 (key->flags & R36SX_OSK_FLAG_SHIFTED) != 0);
        start_key_repeat(keyboard, buttons, key->keycode,
                         (key->flags & R36SX_OSK_FLAG_SHIFTED) != 0);
    }
    return 0;
}

static int key_is_pressed(const struct r36sx_screen_keyboard *keyboard,
                          uint8_t zone,
                          uint8_t row,
                          uint8_t col)
{
    return keyboard && keyboard->press_buttons != 0 &&
        keyboard->press_zone == zone &&
        keyboard->press_row == row &&
        keyboard->press_col == col;
}

static void draw_key(const struct r36sx_screen_keyboard *keyboard,
                     const struct r36sx_osk_key *key,
                     uint16_t *frame,
                     int width,
                     int height,
                     int stride,
                     int x,
                     int y,
                     int key_w,
                     int selected,
                     int pressed)
{
    uint16_t bg = rgb565(32, 42, 54);
    uint16_t fg = rgb565(235, 242, 232);
    uint16_t border = rgb565(110, 132, 150);
    int active_modifier =
        ((key->flags & R36SX_OSK_FLAG_SHIFT_MOD) && keyboard->shift) ||
        ((key->flags & R36SX_OSK_FLAG_CTRL_MOD) && keyboard->ctrl) ||
        ((key->flags & R36SX_OSK_FLAG_ALT_MOD) && keyboard->alt);

    if (active_modifier) {
        bg = rgb565(40, 120, 78);
        border = rgb565(120, 235, 160);
    }
    if (selected) {
        bg = rgb565(238, 172, 62);
        fg = rgb565(18, 22, 26);
        border = rgb565(255, 238, 168);
    }
    if (pressed) {
        x++;
        y++;
        if (selected) {
            bg = rgb565(176, 104, 34);
            border = rgb565(178, 136, 72);
        } else if (active_modifier) {
            bg = rgb565(24, 78, 48);
            border = rgb565(72, 146, 96);
        } else {
            bg = rgb565(18, 24, 32);
            border = rgb565(64, 78, 92);
        }
    }

    fill_rect(frame, width, height, stride, x, y, key_w,
              R36SX_OSK_KEY_H, bg);
    stroke_rect(frame, width, height, stride, x, y, key_w,
                R36SX_OSK_KEY_H, border);
    {
        int text_w = text_width(key->label, R36SX_OSK_TEXT_SCALE);
        int text_x = x + (key_w - text_w) / 2;
        int text_y = y + (R36SX_OSK_KEY_H - 7 * R36SX_OSK_TEXT_SCALE) / 2;
        draw_text(frame, width, height, stride, text_x, text_y, key->label, fg,
                  R36SX_OSK_TEXT_SCALE);
    }
}

void r36sx_screen_keyboard_init(struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard) {
        return;
    }
    keyboard->visible = 0;
    keyboard->zone = R36SX_OSK_ZONE_MAIN;
    keyboard->row = 0;
    keyboard->col = 0;
    keyboard->shift = 0;
    keyboard->ctrl = 0;
    keyboard->alt = 0;
    keyboard->symbol_mode = 0;
    keyboard->cursor_block = 0;
    keyboard->press_zone = R36SX_OSK_ZONE_MAIN;
    keyboard->press_row = 0;
    keyboard->press_col = 0;
    keyboard->press_buttons = 0;
    keyboard->nav_repeat_button = 0;
    keyboard->nav_repeat_next_us = 0;
    keyboard->key_repeat_button = 0;
    keyboard->key_repeat_keycode = 0;
    keyboard->key_repeat_force_shift = 0;
    keyboard->key_repeat_next_us = 0;
}

static void toggle_symbol_mode(struct r36sx_screen_keyboard *keyboard)
{
    const uint8_t *counts;

    if (!keyboard) {
        return;
    }
    keyboard->symbol_mode ^= 1u;
    keyboard->press_buttons = 0;
    reset_nav_repeat(keyboard);
    reset_key_repeat(keyboard);
    if (keyboard->zone == R36SX_OSK_ZONE_MAIN &&
        keyboard->row < R36SX_OSK_ARRAY_COUNT(g_osk_rows)) {
        counts = active_row_counts(keyboard);
        if (keyboard->col >= counts[keyboard->row]) {
            keyboard->col = (uint8_t)(counts[keyboard->row] - 1);
        }
    }
}

int r36sx_screen_keyboard_is_visible(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->visible != 0;
}

void r36sx_screen_keyboard_set_visible(
    struct r36sx_screen_keyboard *keyboard, int visible)
{
    if (!keyboard) {
        return;
    }
    keyboard->visible = (uint8_t)(visible != 0);
    if (!keyboard->visible) {
        keyboard->shift = 0;
        keyboard->ctrl = 0;
        keyboard->alt = 0;
        keyboard->symbol_mode = 0;
        keyboard->press_buttons = 0;
        reset_nav_repeat(keyboard);
        reset_key_repeat(keyboard);
    }
}

void r36sx_screen_keyboard_set_cursor_block(
    struct r36sx_screen_keyboard *keyboard, int enabled)
{
    if (!keyboard) {
        return;
    }
    keyboard->cursor_block = (uint8_t)(enabled != 0);
    if (!keyboard->cursor_block && keyboard->zone == R36SX_OSK_ZONE_CURSOR) {
        keyboard->zone = R36SX_OSK_ZONE_MAIN;
        keyboard->row = 0;
        keyboard->col = 0;
    }
}

int r36sx_screen_keyboard_cursor_block_enabled(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->cursor_block != 0;
}

int r36sx_screen_keyboard_panel_y(int framebuffer_height)
{
    return framebuffer_height > R36SX_SCREEN_KEYBOARD_PANEL_H ?
        framebuffer_height - R36SX_SCREEN_KEYBOARD_PANEL_H : 0;
}

int r36sx_screen_keyboard_content_height(
    const struct r36sx_screen_keyboard *keyboard, int framebuffer_height)
{
    return r36sx_screen_keyboard_is_visible(keyboard) ?
        r36sx_screen_keyboard_panel_y(framebuffer_height) : framebuffer_height;
}

uint16_t r36sx_screen_keyboard_current_keycode(
    struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard) {
        return 0;
    }
    return current_key(keyboard)->keycode;
}

const char *r36sx_screen_keyboard_current_label(
    struct r36sx_screen_keyboard *keyboard)
{
    if (!keyboard) {
        return "";
    }
    return current_key(keyboard)->label;
}

uint32_t r36sx_screen_keyboard_handle_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
    uint32_t held,
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user)
{
    uint32_t result = 0;
    uint32_t nav_buttons;

    if (!keyboard || !keyboard->visible) {
        return 0;
    }
    update_press_animation(keyboard, held);
    if ((pressed & (R36SX_RKGAME_KEY_L | R36SX_RKGAME_KEY_R)) != 0) {
        toggle_symbol_mode(keyboard);
    }
    nav_buttons = nav_buttons_with_repeat(keyboard, pressed, held);
    handle_navigation(keyboard, nav_buttons);
    if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
        r36sx_screen_keyboard_set_visible(keyboard, 0);
        return R36SX_SCREEN_KEYBOARD_RESULT_CLOSED;
    }
    if ((pressed & R36SX_RKGAME_KEY_B) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_BACK,
                                      pressed & R36SX_RKGAME_KEY_B);
        emit_key(keyboard, emit, emit_user, R36SX_SCREEN_KEY_BACK, 0);
        start_key_repeat(keyboard, pressed & R36SX_RKGAME_KEY_B,
                         R36SX_SCREEN_KEY_BACK, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_X) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_ESCAPE,
                                      pressed & R36SX_RKGAME_KEY_X);
        emit_key(keyboard, emit, emit_user, R36SX_SCREEN_KEY_ESCAPE, 0);
        start_key_repeat(keyboard, pressed & R36SX_RKGAME_KEY_X,
                         R36SX_SCREEN_KEY_ESCAPE, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_Y) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_RETURN,
                                      pressed & R36SX_RKGAME_KEY_Y);
        emit_key(keyboard, emit, emit_user, R36SX_SCREEN_KEY_RETURN, 0);
        start_key_repeat(keyboard, pressed & R36SX_RKGAME_KEY_Y,
                         R36SX_SCREEN_KEY_RETURN, 0);
    }
    if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START)) != 0) {
        result |= activate_current(
            keyboard, emit, emit_user,
            pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START));
    }
    handle_key_repeat(keyboard, held & R36SX_OSK_KEY_REPEAT_MASK, emit,
                      emit_user);
    return result;
}

uint32_t r36sx_screen_keyboard_handle_picker_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
    uint32_t held,
    uint16_t *keycode)
{
    const struct r36sx_osk_key *key;
    uint32_t nav_buttons;

    if (!keyboard || !keyboard->visible) {
        return 0;
    }
    update_press_animation(keyboard, held);
    if ((pressed & (R36SX_RKGAME_KEY_L | R36SX_RKGAME_KEY_R)) != 0) {
        toggle_symbol_mode(keyboard);
    }
    nav_buttons = nav_buttons_with_repeat(keyboard, pressed, held);
    handle_navigation(keyboard, nav_buttons);
    if ((pressed & R36SX_RKGAME_KEY_B) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_BACK,
                                      pressed & R36SX_RKGAME_KEY_B);
        if (keycode) {
            *keycode = R36SX_SCREEN_KEY_BACK;
        }
        return R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
    }
    if ((pressed & R36SX_RKGAME_KEY_X) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_ESCAPE,
                                      pressed & R36SX_RKGAME_KEY_X);
        if (keycode) {
            *keycode = R36SX_SCREEN_KEY_ESCAPE;
        }
        return R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
    }
    if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_Y)) == 0) {
        return 0;
    }

    key = current_key(keyboard);
    start_current_press_animation(
        keyboard, pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_Y));
    if ((key->flags & R36SX_OSK_FLAG_CLOSE) != 0) {
        r36sx_screen_keyboard_set_visible(keyboard, 0);
        return R36SX_SCREEN_KEYBOARD_RESULT_CLOSED;
    }
    if (keycode) {
        *keycode = key->keycode;
    }
    return R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
}

void r36sx_screen_keyboard_draw(
    const struct r36sx_screen_keyboard *keyboard,
    uint16_t *frame,
    int width,
    int height,
    int stride_pixels)
{
    const struct r36sx_osk_key *const *rows = active_rows(keyboard);
    const uint8_t *counts = active_row_counts(keyboard);
    const int rows_h =
        (int)R36SX_OSK_ARRAY_COUNT(g_osk_rows) * R36SX_OSK_KEY_H +
        ((int)R36SX_OSK_ARRAY_COUNT(g_osk_rows) - 1) *
        R36SX_OSK_KEY_GAP;
    const int panel_x = 0;
    const int panel_w = width;
    const int panel_y = r36sx_screen_keyboard_panel_y(height);
    const int content_x = panel_x + 1 + R36SX_OSK_INNER_PAD;
    const int content_w = panel_w - 2 * (1 + R36SX_OSK_INNER_PAD);
    const int keys_y = panel_y + R36SX_SCREEN_KEYBOARD_PANEL_H - 1 -
        R36SX_OSK_INNER_PAD - rows_h;
    const int compact = r36sx_screen_keyboard_cursor_block_enabled(keyboard);
    const int main_key_w = key_width(keyboard);
    const int cursor_x = content_x + content_w - R36SX_OSK_CURSOR_BLOCK_W;
    const int main_w = compact ?
        content_w - R36SX_OSK_CURSOR_BLOCK_W - R36SX_OSK_CURSOR_BLOCK_GAP :
        content_w;
    const uint16_t panel = rgb565(12, 18, 24);
    const uint16_t header = rgb565(24, 54, 70);
    const uint16_t border = rgb565(160, 192, 204);
    const uint16_t text = rgb565(228, 236, 224);

    if (!r36sx_screen_keyboard_is_visible(keyboard) || !frame || width <= 0 ||
        height <= 0 || stride_pixels <= 0 || content_w <= 0) {
        return;
    }

    fill_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
              R36SX_SCREEN_KEYBOARD_PANEL_H, panel);
    stroke_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
                R36SX_SCREEN_KEYBOARD_PANEL_H, border);
    fill_rect(frame, width, height, stride_pixels, panel_x + 1, panel_y + 1,
              panel_w - 2, R36SX_OSK_HEADER_H, header);
    draw_text(frame, width, height, stride_pixels, panel_x + 6, panel_y + 5,
              keyboard->symbol_mode ?
              "FN KBD  L/R ABC  A/START TYPE  B BS  X ESC  Y ENT" :
              "FN KBD  L/R SYM  A/START TYPE  B BS  X ESC  Y ENT", text,
              R36SX_OSK_TEXT_SCALE);

    for (size_t row = 0; row < R36SX_OSK_ARRAY_COUNT(g_osk_rows); row++) {
        int count = counts[row];
        int row_w = count * main_key_w +
                    (count - 1) * R36SX_OSK_KEY_GAP;
        int x = content_x + (main_w - row_w) / 2;
        int y = keys_y +
                (int)row * (R36SX_OSK_KEY_H + R36SX_OSK_KEY_GAP);
        for (int col = 0; col < count; col++) {
            draw_key(keyboard, &rows[row][col], frame, width, height,
                     stride_pixels, x, y, main_key_w,
                     keyboard->zone == R36SX_OSK_ZONE_MAIN &&
                     row == keyboard->row && col == keyboard->col,
                     key_is_pressed(keyboard, R36SX_OSK_ZONE_MAIN,
                                    (uint8_t)row, (uint8_t)col));
            x += main_key_w + R36SX_OSK_KEY_GAP;
        }
    }

    if (compact) {
        int cursor_y = keys_y +
            R36SX_OSK_CURSOR_BLOCK_Y_ROW *
            (R36SX_OSK_KEY_H + R36SX_OSK_KEY_GAP);
        for (int row = 0; row < R36SX_OSK_CURSOR_BLOCK_ROWS; row++) {
            for (int col = 0; col < R36SX_OSK_CURSOR_BLOCK_COLS; col++) {
                const struct r36sx_osk_key *key = cursor_key_at(row, col);
                if (!key) {
                    continue;
                }
                draw_key(keyboard, key, frame, width, height, stride_pixels,
                         cursor_x + col * (R36SX_OSK_CURSOR_KEY_W +
                                           R36SX_OSK_CURSOR_GAP),
                         cursor_y + row * (R36SX_OSK_KEY_H +
                                           R36SX_OSK_CURSOR_GAP),
                         R36SX_OSK_CURSOR_KEY_W,
                         keyboard->zone == R36SX_OSK_ZONE_CURSOR &&
                         row == keyboard->row && col == keyboard->col,
                         key_is_pressed(keyboard, R36SX_OSK_ZONE_CURSOR,
                                        (uint8_t)row, (uint8_t)col));
            }
        }
    }
}
