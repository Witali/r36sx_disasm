#include "r36sx_key_presets.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../common/hardware.h"
#include "../common/r36sx_screen_keyboard.h"

#define R36SX_KEY_PRESETS_CONF "keypresets.conf"
#define R36SX_KEY_PRESETS_ABS_CONF "/mnt/sdcard/MIPS_NATIVE/pico_286/keypresets.conf"
#define R36SX_KEY_PRESET_ROW_COUNT (2 + R36SX_KEY_PRESET_BUTTON_COUNT)

struct r36sx_key_preset_button_info {
    const char *config_key;
    const char *label;
    uint32_t raw_mask;
    uint16_t default_keycode;
};

struct r36sx_key_choice {
    const char *label;
    uint16_t keycode;
};

static const struct r36sx_key_preset_button_info g_buttons[] = {
    { "up", "UP", R36SX_RKGAME_KEY_UP, R36SX_SCREEN_KEY_UP },
    { "down", "DOWN", R36SX_RKGAME_KEY_DOWN, R36SX_SCREEN_KEY_DOWN },
    { "left", "LEFT", R36SX_RKGAME_KEY_LEFT, R36SX_SCREEN_KEY_LEFT },
    { "right", "RIGHT", R36SX_RKGAME_KEY_RIGHT, R36SX_SCREEN_KEY_RIGHT },
    { "a", "A", R36SX_RKGAME_KEY_A, R36SX_SCREEN_KEY_RETURN },
    { "b", "B", R36SX_RKGAME_KEY_B, R36SX_SCREEN_KEY_ESCAPE },
    { "y", "Y", R36SX_RKGAME_KEY_Y, R36SX_SCREEN_KEY_CONTROL },
    { "x", "X", R36SX_RKGAME_KEY_X, R36SX_SCREEN_KEY_SPACE },
    { "start", "START", R36SX_RKGAME_KEY_START, R36SX_SCREEN_KEY_RETURN },
    { "l", "L", R36SX_RKGAME_KEY_L, R36SX_SCREEN_KEY_MENU },
    { "l2", "L2", R36SX_RKGAME_KEY_L2, R36SX_SCREEN_KEY_F1 },
    { "r", "R", R36SX_RKGAME_KEY_R, R36SX_SCREEN_KEY_SHIFT },
    { "r2", "R2", R36SX_RKGAME_KEY_R2, R36SX_SCREEN_KEY_F1 + 1 },
};

static const struct r36sx_key_choice g_choices[] = {
    { "NONE", 0 },
    { "A", 'A' }, { "B", 'B' }, { "C", 'C' }, { "D", 'D' },
    { "E", 'E' }, { "F", 'F' }, { "G", 'G' }, { "H", 'H' },
    { "I", 'I' }, { "J", 'J' }, { "K", 'K' }, { "L", 'L' },
    { "M", 'M' }, { "N", 'N' }, { "O", 'O' }, { "P", 'P' },
    { "Q", 'Q' }, { "R", 'R' }, { "S", 'S' }, { "T", 'T' },
    { "U", 'U' }, { "V", 'V' }, { "W", 'W' }, { "X", 'X' },
    { "Y", 'Y' }, { "Z", 'Z' },
    { "0", '0' }, { "1", '1' }, { "2", '2' }, { "3", '3' },
    { "4", '4' }, { "5", '5' }, { "6", '6' }, { "7", '7' },
    { "8", '8' }, { "9", '9' },
    { "UP", R36SX_SCREEN_KEY_UP },
    { "DOWN", R36SX_SCREEN_KEY_DOWN },
    { "LEFT", R36SX_SCREEN_KEY_LEFT },
    { "RIGHT", R36SX_SCREEN_KEY_RIGHT },
    { "ENTER", R36SX_SCREEN_KEY_RETURN },
    { "ESC", R36SX_SCREEN_KEY_ESCAPE },
    { "SPACE", R36SX_SCREEN_KEY_SPACE },
    { "CTRL", R36SX_SCREEN_KEY_CONTROL },
    { "ALT", R36SX_SCREEN_KEY_MENU },
    { "SHIFT", R36SX_SCREEN_KEY_SHIFT },
    { "TAB", R36SX_SCREEN_KEY_TAB },
    { "BACK", R36SX_SCREEN_KEY_BACK },
    { "DEL", R36SX_SCREEN_KEY_DELETE },
    { "F1", R36SX_SCREEN_KEY_F1 + 0 },
    { "F2", R36SX_SCREEN_KEY_F1 + 1 },
    { "F3", R36SX_SCREEN_KEY_F1 + 2 },
    { "F4", R36SX_SCREEN_KEY_F1 + 3 },
    { "F5", R36SX_SCREEN_KEY_F1 + 4 },
    { "F6", R36SX_SCREEN_KEY_F1 + 5 },
    { "F7", R36SX_SCREEN_KEY_F1 + 6 },
    { "F8", R36SX_SCREEN_KEY_F1 + 7 },
    { "F9", R36SX_SCREEN_KEY_F1 + 8 },
    { "F10", R36SX_SCREEN_KEY_F1 + 9 },
    { "F11", R36SX_SCREEN_KEY_F1 + 10 },
    { "F12", R36SX_SCREEN_KEY_F1 + 11 },
    { "MINUS", R36SX_SCREEN_KEY_OEM_MINUS },
    { "EQUAL", R36SX_SCREEN_KEY_OEM_PLUS },
    { "COMMA", R36SX_SCREEN_KEY_OEM_COMMA },
    { "PERIOD", R36SX_SCREEN_KEY_OEM_PERIOD },
    { "SLASH", R36SX_SCREEN_KEY_OEM_2 },
    { "BACKSLASH", R36SX_SCREEN_KEY_OEM_5 },
    { "QUOTE", R36SX_SCREEN_KEY_OEM_7 },
    { "COLON", R36SX_SCREEN_KEY_OEM_1 },
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
    if (!frame || w <= 0 || h <= 0) {
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
    static const uint8_t glyph_plus[7] = {0, 4, 4, 31, 4, 4, 0};
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
    case '+': glyph = glyph_plus; break;
    default: break;
    }
    return glyph[row];
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

static int str_equals(const char *left, const char *right)
{
    while (*left && *right) {
        if (tolower((unsigned char)*left) != tolower((unsigned char)*right)) {
            return 0;
        }
        left++;
        right++;
    }
    return *left == '\0' && *right == '\0';
}

static char *trim_space(char *text)
{
    char *end;

    while (*text && isspace((unsigned char)*text)) {
        text++;
    }
    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';
    return text;
}

static const char *choice_label(uint16_t keycode)
{
    for (size_t i = 0; i < sizeof(g_choices) / sizeof(g_choices[0]); i++) {
        if (g_choices[i].keycode == keycode) {
            return g_choices[i].label;
        }
    }
    return "NONE";
}

static int choice_index_for_keycode(uint16_t keycode)
{
    for (size_t i = 0; i < sizeof(g_choices) / sizeof(g_choices[0]); i++) {
        if (g_choices[i].keycode == keycode) {
            return (int)i;
        }
    }
    return 0;
}

static uint16_t keycode_for_label(const char *label)
{
    for (size_t i = 0; i < sizeof(g_choices) / sizeof(g_choices[0]); i++) {
        if (str_equals(label, g_choices[i].label)) {
            return g_choices[i].keycode;
        }
    }
    return 0;
}

static int button_index_for_config_key(const char *key)
{
    for (size_t i = 0; i < sizeof(g_buttons) / sizeof(g_buttons[0]); i++) {
        if (str_equals(key, g_buttons[i].config_key)) {
            return (int)i;
        }
    }
    return -1;
}

static void set_default_preset(struct r36sx_key_preset *preset)
{
    snprintf(preset->name, sizeof(preset->name), "Default");
    for (size_t i = 0; i < sizeof(g_buttons) / sizeof(g_buttons[0]); i++) {
        preset->keycodes[i] = g_buttons[i].default_keycode;
    }
}

void r36sx_key_presets_init(struct r36sx_key_presets *state)
{
    if (!state) {
        return;
    }
    memset(state, 0, sizeof(*state));
    state->count = 1;
    state->active = 0;
    set_default_preset(&state->presets[0]);
}

static FILE *open_config_for_read(struct r36sx_key_presets *state)
{
    FILE *fp = fopen(R36SX_KEY_PRESETS_CONF, "r");

    if (fp) {
        snprintf(state->config_path, sizeof(state->config_path), "%s",
                 R36SX_KEY_PRESETS_CONF);
        return fp;
    }

    fp = fopen(R36SX_KEY_PRESETS_ABS_CONF, "r");
    if (fp) {
        snprintf(state->config_path, sizeof(state->config_path), "%s",
                 R36SX_KEY_PRESETS_ABS_CONF);
    }
    return fp;
}

void r36sx_key_presets_load(struct r36sx_key_presets *state)
{
    FILE *fp;
    char line[256];
    int current_preset = -1;
    char active_name[R36SX_KEY_PRESET_NAME_LEN] = "";

    if (!state) {
        return;
    }
    r36sx_key_presets_init(state);

    fp = open_config_for_read(state);
    if (!fp) {
        r36sx_pico286_debug_log("keypresets: config missing, writing defaults");
        r36sx_key_presets_save(state);
        return;
    }

    state->count = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *text = trim_space(line);
        char *equals;

        if (text[0] == '\0' || text[0] == '#' || text[0] == ';') {
            continue;
        }
        if (text[0] == '[') {
            char *end = strchr(text, ']');
            if (!end || state->count >= R36SX_KEY_PRESETS_MAX) {
                current_preset = -1;
                continue;
            }
            *end = '\0';
            text++;
            if (strlen(text) >= 6 &&
                tolower((unsigned char)text[0]) == 'p' &&
                tolower((unsigned char)text[1]) == 'r' &&
                tolower((unsigned char)text[2]) == 'e' &&
                tolower((unsigned char)text[3]) == 's' &&
                tolower((unsigned char)text[4]) == 'e' &&
                tolower((unsigned char)text[5]) == 't') {
                char *name = trim_space(text + 6);
                current_preset = (int)state->count++;
                memset(&state->presets[current_preset], 0,
                       sizeof(state->presets[current_preset]));
                snprintf(state->presets[current_preset].name,
                         sizeof(state->presets[current_preset].name),
                         "%s", name);
            }
            continue;
        }

        equals = strchr(text, '=');
        if (!equals) {
            continue;
        }
        *equals = '\0';
        {
            char *key = trim_space(text);
            char *value = trim_space(equals + 1);
            if (str_equals(key, "active")) {
                snprintf(active_name, sizeof(active_name), "%s", value);
            } else if (current_preset >= 0) {
                int button = button_index_for_config_key(key);
                if (button >= 0) {
                    state->presets[current_preset].keycodes[button] =
                        keycode_for_label(value);
                }
            }
        }
    }
    fclose(fp);

    if (state->count == 0) {
        state->count = 1;
        set_default_preset(&state->presets[0]);
    }

    for (uint8_t i = 0; i < state->count; i++) {
        if (str_equals(active_name, state->presets[i].name)) {
            state->active = i;
            break;
        }
    }
    r36sx_pico286_debug_log("keypresets: loaded count=%u active=%u",
                            state->count, state->active);
}

void r36sx_key_presets_save(struct r36sx_key_presets *state)
{
    FILE *fp;
    const char *path;

    if (!state) {
        return;
    }

    path = state->config_path[0] ? state->config_path : R36SX_KEY_PRESETS_CONF;
    fp = fopen(path, "w");
    if (!fp && !str_equals(path, R36SX_KEY_PRESETS_ABS_CONF)) {
        path = R36SX_KEY_PRESETS_ABS_CONF;
        fp = fopen(path, "w");
    }
    if (!fp) {
        r36sx_pico286_debug_log("keypresets: save failed");
        return;
    }

    snprintf(state->config_path, sizeof(state->config_path), "%s", path);
    fprintf(fp, "# Pico-286 key presets for the R36SX native port.\n");
    fprintf(fp, "# Values use labels such as A, ENTER, ESC, SPACE, CTRL, F1.\n\n");
    fprintf(fp, "active=%s\n\n", state->presets[state->active].name);
    for (uint8_t preset = 0; preset < state->count; preset++) {
        fprintf(fp, "[preset %s]\n", state->presets[preset].name);
        for (size_t i = 0; i < sizeof(g_buttons) / sizeof(g_buttons[0]); i++) {
            fprintf(fp, "%s=%s\n", g_buttons[i].config_key,
                    choice_label(state->presets[preset].keycodes[i]));
        }
        fputc('\n', fp);
    }
    fclose(fp);
    r36sx_pico286_debug_log("keypresets: saved %s", path);
}

int r36sx_key_presets_is_visible(const struct r36sx_key_presets *state)
{
    return state && state->visible != 0;
}

void r36sx_key_presets_set_visible(struct r36sx_key_presets *state, int visible)
{
    if (!state) {
        return;
    }
    state->visible = (uint8_t)(visible != 0);
    if (state->selected_row >= R36SX_KEY_PRESET_ROW_COUNT) {
        state->selected_row = 0;
    }
}

uint16_t r36sx_key_presets_key_for_mask(
    const struct r36sx_key_presets *state, uint32_t raw_mask)
{
    const struct r36sx_key_preset *preset;

    if (!state || state->count == 0 || state->active >= state->count) {
        return 0;
    }
    preset = &state->presets[state->active];
    for (size_t i = 0; i < sizeof(g_buttons) / sizeof(g_buttons[0]); i++) {
        if (g_buttons[i].raw_mask == raw_mask) {
            return preset->keycodes[i];
        }
    }
    return 0;
}

static void select_next_preset(struct r36sx_key_presets *state, int direction)
{
    int active;

    if (state->count == 0) {
        return;
    }
    active = (int)state->active + direction;
    if (active < 0) {
        active = (int)state->count - 1;
    } else if (active >= state->count) {
        active = 0;
    }
    state->active = (uint8_t)active;
    r36sx_key_presets_save(state);
}

static void add_preset(struct r36sx_key_presets *state)
{
    struct r36sx_key_preset *preset;

    if (state->count >= R36SX_KEY_PRESETS_MAX) {
        return;
    }
    preset = &state->presets[state->count];
    *preset = state->presets[state->active];
    snprintf(preset->name, sizeof(preset->name), "Preset %u",
             (unsigned int)state->count + 1u);
    state->active = state->count;
    state->count++;
    r36sx_key_presets_save(state);
}

static void cycle_binding(struct r36sx_key_presets *state, int button,
                          int direction)
{
    int choice;
    int count = (int)(sizeof(g_choices) / sizeof(g_choices[0]));

    if (button < 0 || button >= R36SX_KEY_PRESET_BUTTON_COUNT) {
        return;
    }
    choice = choice_index_for_keycode(
        state->presets[state->active].keycodes[button]);
    choice += direction;
    if (choice < 0) {
        choice = count - 1;
    } else if (choice >= count) {
        choice = 0;
    }
    state->presets[state->active].keycodes[button] =
        g_choices[choice].keycode;
    r36sx_key_presets_save(state);
}

uint32_t r36sx_key_presets_handle_buttons(struct r36sx_key_presets *state,
                                          uint32_t pressed)
{
    int row;

    if (!state || !state->visible) {
        return 0;
    }

    if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
        r36sx_key_presets_set_visible(state, 0);
        r36sx_key_presets_save(state);
        return R36SX_KEY_PRESET_RESULT_CLOSED;
    }

    if ((pressed & R36SX_RKGAME_KEY_UP) != 0) {
        state->selected_row =
            state->selected_row == 0 ?
                (uint8_t)(R36SX_KEY_PRESET_ROW_COUNT - 1) :
                (uint8_t)(state->selected_row - 1);
    }
    if ((pressed & R36SX_RKGAME_KEY_DOWN) != 0) {
        state->selected_row =
            (uint8_t)((state->selected_row + 1) % R36SX_KEY_PRESET_ROW_COUNT);
    }

    row = state->selected_row;
    if (row == 0) {
        if ((pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
            select_next_preset(state, -1);
        }
        if ((pressed & (R36SX_RKGAME_KEY_RIGHT |
                        R36SX_RKGAME_KEY_A |
                        R36SX_RKGAME_KEY_START)) != 0) {
            select_next_preset(state, 1);
        }
        return 0;
    }

    if (row == 1) {
        if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_START)) != 0) {
            add_preset(state);
        }
        return 0;
    }

    {
        int button = row - 2;
        if ((pressed & (R36SX_RKGAME_KEY_RIGHT |
                        R36SX_RKGAME_KEY_A |
                        R36SX_RKGAME_KEY_START)) != 0) {
            cycle_binding(state, button, 1);
        }
        if ((pressed & (R36SX_RKGAME_KEY_LEFT |
                        R36SX_RKGAME_KEY_B)) != 0) {
            cycle_binding(state, button, -1);
        }
        if ((pressed & R36SX_RKGAME_KEY_Y) != 0) {
            state->presets[state->active].keycodes[button] = 0;
            r36sx_key_presets_save(state);
        }
    }

    return 0;
}

static void draw_row(const struct r36sx_key_presets *state,
                     uint16_t *frame,
                     int width,
                     int height,
                     int stride,
                     int row,
                     int x,
                     int y,
                     int row_h,
                     const char *text)
{
    int selected = row == state->selected_row;
    uint16_t bg = selected ? rgb565(232, 174, 66) : rgb565(28, 34, 42);
    uint16_t fg = selected ? rgb565(15, 18, 20) : rgb565(232, 238, 226);
    uint16_t border = selected ? rgb565(255, 239, 158) : rgb565(74, 91, 108);

    fill_rect(frame, width, height, stride, x, y, width - x * 2, row_h, bg);
    stroke_rect(frame, width, height, stride, x, y, width - x * 2, row_h,
                border);
    draw_text(frame, width, height, stride, x + 10, y + 5, text, fg, 2);
}

void r36sx_key_presets_draw(const struct r36sx_key_presets *state,
                            uint16_t *frame,
                            int width,
                            int height,
                            int stride_pixels)
{
    char line[96];
    int x = 28;
    int y = 86;
    const int row_h = 23;

    if (!r36sx_key_presets_is_visible(state) || !frame) {
        return;
    }

    fill_rect(frame, width, height, stride_pixels, 0, 0, width, height,
              rgb565(10, 13, 18));
    stroke_rect(frame, width, height, stride_pixels, 12, 12, width - 24,
                height - 24, rgb565(94, 118, 134));
    draw_text(frame, width, height, stride_pixels, 28, 28, "KEY PRESETS",
              rgb565(238, 236, 196), 3);
    draw_text(frame, width, height, stride_pixels, 28, 60,
              "SELECT CLOSE  UP/DOWN ROW  LEFT/RIGHT KEY  A NEXT/ADD  B PREV  Y NONE",
              rgb565(180, 202, 208), 1);

    snprintf(line, sizeof(line), "PRESET: %s  %u/%u",
             state->presets[state->active].name,
             (unsigned int)state->active + 1u,
             (unsigned int)state->count);
    draw_row(state, frame, width, height, stride_pixels, 0, x, y, row_h, line);
    y += row_h + 4;

    snprintf(line, sizeof(line), "%s",
             state->count < R36SX_KEY_PRESETS_MAX ?
                "+ ADD NEW PRESET" : "MAX PRESETS REACHED");
    draw_row(state, frame, width, height, stride_pixels, 1, x, y, row_h, line);
    y += row_h + 8;

    for (int i = 0; i < R36SX_KEY_PRESET_BUTTON_COUNT; i++) {
        snprintf(line, sizeof(line), "%-6s : %s", g_buttons[i].label,
                 choice_label(state->presets[state->active].keycodes[i]));
        draw_row(state, frame, width, height, stride_pixels, i + 2, x, y,
                 row_h, line);
        y += row_h + 3;
    }
}
