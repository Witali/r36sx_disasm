#include "r36sx_screen_keyboard.h"

#include <stddef.h>

#define R36SX_OSK_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_OSK_KEY_W 44
#define R36SX_OSK_KEY_H 13
#define R36SX_OSK_KEY_GAP 2
#define R36SX_OSK_TEXT_SCALE 1

#define R36SX_OSK_FLAG_SHIFTED 0x01u
#define R36SX_OSK_FLAG_SHIFT_MOD 0x02u
#define R36SX_OSK_FLAG_CTRL_MOD 0x04u
#define R36SX_OSK_FLAG_ALT_MOD 0x08u
#define R36SX_OSK_FLAG_CLOSE 0x10u

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

static const struct r36sx_osk_key *const g_osk_rows[] = {
    g_osk_row0, g_osk_row1, g_osk_row2, g_osk_row3, g_osk_row4
};

static const uint8_t g_osk_row_counts[] = {
    R36SX_OSK_ARRAY_COUNT(g_osk_row0),
    R36SX_OSK_ARRAY_COUNT(g_osk_row1),
    R36SX_OSK_ARRAY_COUNT(g_osk_row2),
    R36SX_OSK_ARRAY_COUNT(g_osk_row3),
    R36SX_OSK_ARRAY_COUNT(g_osk_row4)
};

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r & 0xf8u) << 8) |
                      ((uint16_t)(g & 0xfcu) << 3) |
                      ((uint16_t)b >> 3));
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

static uint8_t glyph_row(char c, int row)
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
        uint8_t bits = glyph_row(c, row);
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

static void move_selection(struct r36sx_screen_keyboard *keyboard, int dx,
                           int dy)
{
    int row_count = (int)R36SX_OSK_ARRAY_COUNT(g_osk_rows);
    int row = (int)keyboard->row + dy;
    int col = (int)keyboard->col + dx;

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

    keyboard->row = (uint8_t)row;
    keyboard->col = (uint8_t)col;
}

static const struct r36sx_osk_key *current_key(
    struct r36sx_screen_keyboard *keyboard)
{
    if (keyboard->row >= R36SX_OSK_ARRAY_COUNT(g_osk_rows)) {
        keyboard->row = 0;
    }
    if (keyboard->col >= g_osk_row_counts[keyboard->row]) {
        keyboard->col = 0;
    }
    return &g_osk_rows[keyboard->row][keyboard->col];
}

static uint32_t activate_current(struct r36sx_screen_keyboard *keyboard,
                                 r36sx_screen_keyboard_emit_fn emit,
                                 void *emit_user)
{
    const struct r36sx_osk_key *key = current_key(keyboard);

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
    }
    return 0;
}

static void draw_key(const struct r36sx_screen_keyboard *keyboard,
                     const struct r36sx_osk_key *key,
                     uint16_t *frame,
                     int width,
                     int height,
                     int stride,
                     int x,
                     int y,
                     int selected)
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

    fill_rect(frame, width, height, stride, x, y, R36SX_OSK_KEY_W,
              R36SX_OSK_KEY_H, bg);
    stroke_rect(frame, width, height, stride, x, y, R36SX_OSK_KEY_W,
                R36SX_OSK_KEY_H, border);
    {
        int text_w = text_width(key->label, R36SX_OSK_TEXT_SCALE);
        int text_x = x + (R36SX_OSK_KEY_W - text_w) / 2;
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
    keyboard->row = 0;
    keyboard->col = 0;
    keyboard->shift = 0;
    keyboard->ctrl = 0;
    keyboard->alt = 0;
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
    }
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

uint32_t r36sx_screen_keyboard_handle_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user)
{
    uint32_t result = 0;

    if (!keyboard || !keyboard->visible) {
        return 0;
    }
    if ((pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
        move_selection(keyboard, -1, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_RIGHT) != 0) {
        move_selection(keyboard, 1, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_UP) != 0) {
        move_selection(keyboard, 0, -1);
    }
    if ((pressed & R36SX_RKGAME_KEY_DOWN) != 0) {
        move_selection(keyboard, 0, 1);
    }
    if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
        r36sx_screen_keyboard_set_visible(keyboard, 0);
        return R36SX_SCREEN_KEYBOARD_RESULT_CLOSED;
    }
    if ((pressed & R36SX_RKGAME_KEY_B) != 0) {
        emit_key(keyboard, emit, emit_user, R36SX_SCREEN_KEY_BACK, 0);
    }
    if ((pressed & R36SX_RKGAME_KEY_X) != 0) {
        keyboard->shift ^= 1u;
    }
    if ((pressed & R36SX_RKGAME_KEY_Y) != 0) {
        emit_key(keyboard, emit, emit_user, R36SX_SCREEN_KEY_RETURN, 0);
    }
    if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START)) != 0) {
        result |= activate_current(keyboard, emit, emit_user);
    }
    return result;
}

void r36sx_screen_keyboard_draw(
    const struct r36sx_screen_keyboard *keyboard,
    uint16_t *frame,
    int width,
    int height,
    int stride_pixels)
{
    const int panel_x = 8;
    const int panel_w = width - 16;
    const int panel_y = r36sx_screen_keyboard_panel_y(height);
    const uint16_t panel = rgb565(12, 18, 24);
    const uint16_t header = rgb565(24, 54, 70);
    const uint16_t border = rgb565(160, 192, 204);
    const uint16_t text = rgb565(228, 236, 224);

    if (!r36sx_screen_keyboard_is_visible(keyboard) || !frame || width <= 0 ||
        height <= 0 || stride_pixels <= 0) {
        return;
    }

    fill_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
              R36SX_SCREEN_KEYBOARD_PANEL_H, panel);
    stroke_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
                R36SX_SCREEN_KEYBOARD_PANEL_H, border);
    fill_rect(frame, width, height, stride_pixels, panel_x + 2, panel_y + 2,
              panel_w - 4, 12, header);
    draw_text(frame, width, height, stride_pixels, panel_x + 10, panel_y + 5,
              "FN KBD  D-PAD MOVE  A/START TYPE  B BS  Y ENT", text, 1);

    for (size_t row = 0; row < R36SX_OSK_ARRAY_COUNT(g_osk_rows); row++) {
        int count = g_osk_row_counts[row];
        int row_w = count * R36SX_OSK_KEY_W +
                    (count - 1) * R36SX_OSK_KEY_GAP;
        int x = panel_x + (panel_w - row_w) / 2;
        int y = panel_y + 18 +
                (int)row * (R36SX_OSK_KEY_H + R36SX_OSK_KEY_GAP);
        for (int col = 0; col < count; col++) {
            draw_key(keyboard, &g_osk_rows[row][col], frame, width, height,
                     stride_pixels, x, y,
                     row == keyboard->row && col == keyboard->col);
            x += R36SX_OSK_KEY_W + R36SX_OSK_KEY_GAP;
        }
    }
}
