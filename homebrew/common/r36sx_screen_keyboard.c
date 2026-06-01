#include "r36sx_screen_keyboard.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define R36SX_OSK_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_OSK_KEY_H 18
#define R36SX_OSK_KEY_GAP 1
#define R36SX_OSK_TEXT_SCALE 2
#define R36SX_OSK_INNER_PAD 1
#define R36SX_OSK_HEADER_H 11
#define R36SX_OSK_UNIT_1 4
#define R36SX_OSK_UNIT_1_5 6
#define R36SX_OSK_UNIT_2 8
#define R36SX_OSK_UNIT_2_5 10
#define R36SX_OSK_UNIT_6_5 26
#define R36SX_OSK_KEY_DEFAULT_UNITS R36SX_OSK_UNIT_1
#define R36SX_OSK_MIN_UNIT_W 6
#define R36SX_OSK_SIDE_KEY_W 38
#define R36SX_OSK_CURSOR_GAP 2
#define R36SX_OSK_CURSOR_BLOCK_GAP 8
#define R36SX_OSK_CURSOR_BLOCK_COLS 3
#define R36SX_OSK_CURSOR_BLOCK_ROWS 6
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
    (R36SX_OSK_CURSOR_BLOCK_COLS * R36SX_OSK_SIDE_KEY_W + \
     (R36SX_OSK_CURSOR_BLOCK_COLS - 1) * R36SX_OSK_CURSOR_GAP)
#define R36SX_OSK_ROW_STEP (R36SX_OSK_KEY_H + R36SX_OSK_KEY_GAP)
#define R36SX_OSK_FONT_PX 13
#define R36SX_OSK_FONT_SMALL_PX 10
#define R36SX_OSK_FONT_CACHE_SLOTS 128
#define R36SX_OSK_CAPS_LED_RADIUS 4
#define R36SX_OSK_CAPS_LED_OUTER_RADIUS 5

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
    uint8_t units;
};

typedef FT_Error (*r36sx_ft_init_free_type_fn)(FT_Library *);
typedef FT_Error (*r36sx_ft_new_face_fn)(
    FT_Library, const char *, FT_Long, FT_Face *);
typedef FT_Error (*r36sx_ft_done_face_fn)(FT_Face);
typedef FT_Error (*r36sx_ft_done_free_type_fn)(FT_Library);
typedef FT_Error (*r36sx_ft_select_charmap_fn)(FT_Face, FT_Encoding);
typedef FT_Error (*r36sx_ft_set_pixel_sizes_fn)(FT_Face, FT_UInt, FT_UInt);
typedef FT_Error (*r36sx_ft_load_char_fn)(FT_Face, FT_ULong, FT_Int32);

struct r36sx_osk_glyph_cache_entry {
    uint32_t codepoint;
    uint16_t pixel_height;
    uint8_t valid;
    int width;
    int rows;
    int pitch;
    int bitmap_left;
    int bitmap_top;
    int advance;
    uint32_t age;
    uint8_t *buffer;
};

struct r36sx_osk_font_state {
    void *handle;
    FT_Library library;
    FT_Face face;
    r36sx_ft_init_free_type_fn init_free_type;
    r36sx_ft_new_face_fn new_face;
    r36sx_ft_done_face_fn done_face;
    r36sx_ft_done_free_type_fn done_free_type;
    r36sx_ft_select_charmap_fn select_charmap;
    r36sx_ft_set_pixel_sizes_fn set_pixel_sizes;
    r36sx_ft_load_char_fn load_char;
    struct r36sx_osk_glyph_cache_entry cache[R36SX_OSK_FONT_CACHE_SLOTS];
    uint32_t cache_age;
    uint8_t attempted;
    uint8_t active;
};

#define R36SX_OSK_KEY(label, keycode, flags) \
    { label, keycode, flags, R36SX_OSK_KEY_DEFAULT_UNITS }
#define R36SX_OSK_WIDE(label, keycode, flags, units) \
    { label, keycode, flags, units }

static const struct r36sx_osk_key g_osk_row0[] = {
    R36SX_OSK_KEY("ESC", R36SX_SCREEN_KEY_ESCAPE, 0),
    R36SX_OSK_KEY("F1", R36SX_SCREEN_KEY_F1 + 0, 0),
    R36SX_OSK_KEY("F2", R36SX_SCREEN_KEY_F1 + 1, 0),
    R36SX_OSK_KEY("F3", R36SX_SCREEN_KEY_F1 + 2, 0),
    R36SX_OSK_KEY("F4", R36SX_SCREEN_KEY_F1 + 3, 0),
    R36SX_OSK_KEY("F5", R36SX_SCREEN_KEY_F1 + 4, 0),
    R36SX_OSK_KEY("F6", R36SX_SCREEN_KEY_F1 + 5, 0),
    R36SX_OSK_KEY("F7", R36SX_SCREEN_KEY_F1 + 6, 0),
    R36SX_OSK_KEY("F8", R36SX_SCREEN_KEY_F1 + 7, 0),
    R36SX_OSK_KEY("F9", R36SX_SCREEN_KEY_F1 + 8, 0),
    R36SX_OSK_KEY("F10", R36SX_SCREEN_KEY_F1 + 9, 0),
    R36SX_OSK_KEY("F11", R36SX_SCREEN_KEY_F1 + 10, 0),
    R36SX_OSK_KEY("F12", R36SX_SCREEN_KEY_F1 + 11, 0)
};

static const struct r36sx_osk_key g_osk_row1[] = {
    R36SX_OSK_KEY("`", R36SX_SCREEN_KEY_OEM_3, 0),
    R36SX_OSK_KEY("1", '1', 0), R36SX_OSK_KEY("2", '2', 0),
    R36SX_OSK_KEY("3", '3', 0), R36SX_OSK_KEY("4", '4', 0),
    R36SX_OSK_KEY("5", '5', 0), R36SX_OSK_KEY("6", '6', 0),
    R36SX_OSK_KEY("7", '7', 0), R36SX_OSK_KEY("8", '8', 0),
    R36SX_OSK_KEY("9", '9', 0), R36SX_OSK_KEY("0", '0', 0),
    R36SX_OSK_KEY("-", R36SX_SCREEN_KEY_OEM_MINUS, 0),
    R36SX_OSK_KEY("=", R36SX_SCREEN_KEY_OEM_PLUS, 0),
    R36SX_OSK_WIDE("BS", R36SX_SCREEN_KEY_BACK, 0, R36SX_OSK_UNIT_2)
};

static const struct r36sx_osk_key g_osk_row2[] = {
    R36SX_OSK_WIDE("TAB", R36SX_SCREEN_KEY_TAB, 0, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_KEY("Q", 'Q', 0), R36SX_OSK_KEY("W", 'W', 0),
    R36SX_OSK_KEY("E", 'E', 0), R36SX_OSK_KEY("R", 'R', 0),
    R36SX_OSK_KEY("T", 'T', 0), R36SX_OSK_KEY("Y", 'Y', 0),
    R36SX_OSK_KEY("U", 'U', 0), R36SX_OSK_KEY("I", 'I', 0),
    R36SX_OSK_KEY("O", 'O', 0), R36SX_OSK_KEY("P", 'P', 0),
    R36SX_OSK_KEY("[", R36SX_SCREEN_KEY_OEM_4, 0),
    R36SX_OSK_KEY("]", R36SX_SCREEN_KEY_OEM_6, 0),
    R36SX_OSK_WIDE("\\", R36SX_SCREEN_KEY_OEM_5, 0,
                   R36SX_OSK_UNIT_1_5)
};

static const struct r36sx_osk_key g_osk_row3[] = {
    R36SX_OSK_WIDE("CAPS", R36SX_SCREEN_KEY_CAPITAL, 0, R36SX_OSK_UNIT_2),
    R36SX_OSK_KEY("A", 'A', 0), R36SX_OSK_KEY("S", 'S', 0),
    R36SX_OSK_KEY("D", 'D', 0), R36SX_OSK_KEY("F", 'F', 0),
    R36SX_OSK_KEY("G", 'G', 0), R36SX_OSK_KEY("H", 'H', 0),
    R36SX_OSK_KEY("J", 'J', 0), R36SX_OSK_KEY("K", 'K', 0),
    R36SX_OSK_KEY("L", 'L', 0),
    R36SX_OSK_KEY(";", R36SX_SCREEN_KEY_OEM_1, 0),
    R36SX_OSK_KEY("'", R36SX_SCREEN_KEY_OEM_7, 0),
    R36SX_OSK_WIDE("ENT", R36SX_SCREEN_KEY_RETURN, 0, R36SX_OSK_UNIT_2)
};

static const struct r36sx_osk_key g_osk_row4[] = {
    R36SX_OSK_WIDE("SHIFT", R36SX_SCREEN_KEY_SHIFT,
                   R36SX_OSK_FLAG_SHIFT_MOD, R36SX_OSK_UNIT_2_5),
    R36SX_OSK_KEY("Z", 'Z', 0), R36SX_OSK_KEY("X", 'X', 0),
    R36SX_OSK_KEY("C", 'C', 0), R36SX_OSK_KEY("V", 'V', 0),
    R36SX_OSK_KEY("B", 'B', 0), R36SX_OSK_KEY("N", 'N', 0),
    R36SX_OSK_KEY("M", 'M', 0),
    R36SX_OSK_KEY(",", R36SX_SCREEN_KEY_OEM_COMMA, 0),
    R36SX_OSK_KEY(".", R36SX_SCREEN_KEY_OEM_PERIOD, 0),
    R36SX_OSK_KEY("/", R36SX_SCREEN_KEY_OEM_2, 0),
    R36SX_OSK_WIDE("SHIFT", R36SX_SCREEN_KEY_SHIFT,
                   R36SX_OSK_FLAG_SHIFT_MOD, R36SX_OSK_UNIT_2_5)
};

static const struct r36sx_osk_key g_osk_row5[] = {
    R36SX_OSK_WIDE("CTRL", R36SX_SCREEN_KEY_CONTROL,
                   R36SX_OSK_FLAG_CTRL_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_KEY("WIN", R36SX_SCREEN_KEY_LWIN, 0),
    R36SX_OSK_WIDE("ALT", R36SX_SCREEN_KEY_MENU,
                   R36SX_OSK_FLAG_ALT_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("SPC", R36SX_SCREEN_KEY_SPACE, 0,
                   R36SX_OSK_UNIT_6_5),
    R36SX_OSK_WIDE("ALT", R36SX_SCREEN_KEY_MENU,
                   R36SX_OSK_FLAG_ALT_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("MNU", R36SX_SCREEN_KEY_APPS, 0, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("CTRL", R36SX_SCREEN_KEY_CONTROL,
                   R36SX_OSK_FLAG_CTRL_MOD, R36SX_OSK_UNIT_1_5)
};

static const struct r36sx_osk_key g_osk_symbol_row0[] = {
    R36SX_OSK_KEY("ESC", R36SX_SCREEN_KEY_ESCAPE, 0),
    R36SX_OSK_KEY("F1", R36SX_SCREEN_KEY_F1 + 0, 0),
    R36SX_OSK_KEY("F2", R36SX_SCREEN_KEY_F1 + 1, 0),
    R36SX_OSK_KEY("F3", R36SX_SCREEN_KEY_F1 + 2, 0),
    R36SX_OSK_KEY("F4", R36SX_SCREEN_KEY_F1 + 3, 0),
    R36SX_OSK_KEY("F5", R36SX_SCREEN_KEY_F1 + 4, 0),
    R36SX_OSK_KEY("F6", R36SX_SCREEN_KEY_F1 + 5, 0),
    R36SX_OSK_KEY("F7", R36SX_SCREEN_KEY_F1 + 6, 0),
    R36SX_OSK_KEY("F8", R36SX_SCREEN_KEY_F1 + 7, 0),
    R36SX_OSK_KEY("F9", R36SX_SCREEN_KEY_F1 + 8, 0),
    R36SX_OSK_KEY("F10", R36SX_SCREEN_KEY_F1 + 9, 0),
    R36SX_OSK_KEY("F11", R36SX_SCREEN_KEY_F1 + 10, 0),
    R36SX_OSK_KEY("F12", R36SX_SCREEN_KEY_F1 + 11, 0)
};

static const struct r36sx_osk_key g_osk_symbol_row1[] = {
    R36SX_OSK_KEY("~", R36SX_SCREEN_KEY_OEM_3, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("!", '1', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("@", '2', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("#", '3', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("$", '4', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("%", '5', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("^", '6', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("&", '7', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("*", '8', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("(", '9', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY(")", '0', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("_", R36SX_SCREEN_KEY_OEM_MINUS,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("+", R36SX_SCREEN_KEY_OEM_PLUS,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_WIDE("BS", R36SX_SCREEN_KEY_BACK, 0, R36SX_OSK_UNIT_2)
};

static const struct r36sx_osk_key g_osk_symbol_row2[] = {
    R36SX_OSK_WIDE("TAB", R36SX_SCREEN_KEY_TAB, 0, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_KEY("[", R36SX_SCREEN_KEY_OEM_4, 0),
    R36SX_OSK_KEY("]", R36SX_SCREEN_KEY_OEM_6, 0),
    R36SX_OSK_KEY("{", R36SX_SCREEN_KEY_OEM_4, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("}", R36SX_SCREEN_KEY_OEM_6, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("<", R36SX_SCREEN_KEY_OEM_COMMA,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY(">", R36SX_SCREEN_KEY_OEM_PERIOD,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("|", R36SX_SCREEN_KEY_OEM_5, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("\\", R36SX_SCREEN_KEY_OEM_5, 0),
    R36SX_OSK_KEY("?", R36SX_SCREEN_KEY_OEM_2, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("/", R36SX_SCREEN_KEY_OEM_2, 0),
    R36SX_OSK_KEY("=", R36SX_SCREEN_KEY_OEM_PLUS, 0),
    R36SX_OSK_KEY("-", R36SX_SCREEN_KEY_OEM_MINUS, 0)
};

static const struct r36sx_osk_key g_osk_symbol_row3[] = {
    R36SX_OSK_WIDE("CAPS", R36SX_SCREEN_KEY_CAPITAL, 0, R36SX_OSK_UNIT_2),
    R36SX_OSK_KEY(";", R36SX_SCREEN_KEY_OEM_1, 0),
    R36SX_OSK_KEY(":", R36SX_SCREEN_KEY_OEM_1, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("'", R36SX_SCREEN_KEY_OEM_7, 0),
    R36SX_OSK_KEY("\"", R36SX_SCREEN_KEY_OEM_7, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY(",", R36SX_SCREEN_KEY_OEM_COMMA, 0),
    R36SX_OSK_KEY(".", R36SX_SCREEN_KEY_OEM_PERIOD, 0),
    R36SX_OSK_KEY("`", R36SX_SCREEN_KEY_OEM_3, 0),
    R36SX_OSK_KEY("~", R36SX_SCREEN_KEY_OEM_3, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("#", '3', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("$", '4', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_WIDE("ENT", R36SX_SCREEN_KEY_RETURN, 0, R36SX_OSK_UNIT_2)
};

static const struct r36sx_osk_key g_osk_symbol_row4[] = {
    R36SX_OSK_WIDE("SHIFT", R36SX_SCREEN_KEY_SHIFT,
                   R36SX_OSK_FLAG_SHIFT_MOD, R36SX_OSK_UNIT_2_5),
    R36SX_OSK_KEY("%", '5', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("^", '6', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("&", '7', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("*", '8', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("(", '9', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY(")", '0', R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("<", R36SX_SCREEN_KEY_OEM_COMMA,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY(">", R36SX_SCREEN_KEY_OEM_PERIOD,
                  R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("?", R36SX_SCREEN_KEY_OEM_2, R36SX_OSK_FLAG_SHIFTED),
    R36SX_OSK_KEY("/", R36SX_SCREEN_KEY_OEM_2, 0),
    R36SX_OSK_WIDE("SHIFT", R36SX_SCREEN_KEY_SHIFT,
                   R36SX_OSK_FLAG_SHIFT_MOD, R36SX_OSK_UNIT_2_5)
};

static const struct r36sx_osk_key g_osk_symbol_row5[] = {
    R36SX_OSK_WIDE("CTRL", R36SX_SCREEN_KEY_CONTROL,
                   R36SX_OSK_FLAG_CTRL_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_KEY("WIN", R36SX_SCREEN_KEY_LWIN, 0),
    R36SX_OSK_WIDE("ALT", R36SX_SCREEN_KEY_MENU,
                   R36SX_OSK_FLAG_ALT_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("SPC", R36SX_SCREEN_KEY_SPACE, 0,
                   R36SX_OSK_UNIT_6_5),
    R36SX_OSK_WIDE("ALT", R36SX_SCREEN_KEY_MENU,
                   R36SX_OSK_FLAG_ALT_MOD, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("MNU", R36SX_SCREEN_KEY_APPS, 0, R36SX_OSK_UNIT_1_5),
    R36SX_OSK_WIDE("CTRL", R36SX_SCREEN_KEY_CONTROL,
                   R36SX_OSK_FLAG_CTRL_MOD, R36SX_OSK_UNIT_1_5)
};

static const struct r36sx_osk_key g_osk_cursor_keys[] = {
    R36SX_OSK_KEY("PRT", R36SX_SCREEN_KEY_PRINT, 0),
    R36SX_OSK_KEY("SCR", R36SX_SCREEN_KEY_SCROLL, 0),
    R36SX_OSK_KEY("PAU", R36SX_SCREEN_KEY_PAUSE, 0),
    R36SX_OSK_KEY("INS", R36SX_SCREEN_KEY_INSERT, 0),
    R36SX_OSK_KEY("HOM", R36SX_SCREEN_KEY_HOME, 0),
    R36SX_OSK_KEY("PUP", R36SX_SCREEN_KEY_PRIOR, 0),
    R36SX_OSK_KEY("DEL", R36SX_SCREEN_KEY_DELETE, 0),
    R36SX_OSK_KEY("END", R36SX_SCREEN_KEY_END, 0),
    R36SX_OSK_KEY("PDN", R36SX_SCREEN_KEY_NEXT, 0),
    R36SX_OSK_KEY(R36SX_OSK_LABEL_UP, R36SX_SCREEN_KEY_UP, 0),
    R36SX_OSK_KEY(R36SX_OSK_LABEL_LEFT, R36SX_SCREEN_KEY_LEFT, 0),
    R36SX_OSK_KEY(R36SX_OSK_LABEL_DOWN, R36SX_SCREEN_KEY_DOWN, 0),
    R36SX_OSK_KEY(R36SX_OSK_LABEL_RIGHT, R36SX_SCREEN_KEY_RIGHT, 0),
};

static const int8_t g_osk_cursor_grid
    [R36SX_OSK_CURSOR_BLOCK_ROWS][R36SX_OSK_CURSOR_BLOCK_COLS] = {
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { R36SX_OSK_NO_CURSOR_KEY, R36SX_OSK_NO_CURSOR_KEY,
      R36SX_OSK_NO_CURSOR_KEY },
    { R36SX_OSK_NO_CURSOR_KEY, 9, R36SX_OSK_NO_CURSOR_KEY },
    { 10, 11, 12 },
};

static const struct r36sx_osk_key *const g_osk_rows[] = {
    g_osk_row0, g_osk_row1, g_osk_row2, g_osk_row3, g_osk_row4, g_osk_row5
};

static const struct r36sx_osk_key *const g_osk_symbol_rows[] = {
    g_osk_symbol_row0, g_osk_symbol_row1, g_osk_symbol_row2,
    g_osk_symbol_row3, g_osk_symbol_row4, g_osk_symbol_row5
};

static const uint8_t g_osk_row_counts[] = {
    R36SX_OSK_ARRAY_COUNT(g_osk_row0),
    R36SX_OSK_ARRAY_COUNT(g_osk_row1),
    R36SX_OSK_ARRAY_COUNT(g_osk_row2),
    R36SX_OSK_ARRAY_COUNT(g_osk_row3),
    R36SX_OSK_ARRAY_COUNT(g_osk_row4),
    R36SX_OSK_ARRAY_COUNT(g_osk_row5)
};

static const uint8_t g_osk_symbol_row_counts[] = {
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row0),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row1),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row2),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row3),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row4),
    R36SX_OSK_ARRAY_COUNT(g_osk_symbol_row5)
};

static struct r36sx_osk_font_state g_osk_font;

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

static void draw_caps_lock_led(uint16_t *frame, int width, int height,
                               int stride, int cx, int cy, int enabled)
{
    const int radius = R36SX_OSK_CAPS_LED_RADIUS;
    const int outer_radius = R36SX_OSK_CAPS_LED_OUTER_RADIUS;
    const uint16_t bright_green = rgb565(64, 255, 106);
    const uint16_t dim_green = rgb565(18, 72, 34);
    const uint16_t dark_green = rgb565(8, 34, 18);
    const uint16_t outline = rgb565(0, 0, 0);

    for (int y = -outer_radius; y <= outer_radius; y++) {
        int py = cy + y;
        if (py < 0 || py >= height) {
            continue;
        }
        for (int x = -outer_radius; x <= outer_radius; x++) {
            int px = cx + x;
            int dist2 = x * x + y * y;
            if (px < 0 || px >= width) {
                continue;
            }
            if (dist2 <= radius * radius) {
                uint16_t color = enabled ? bright_green : dark_green;
                if (enabled && dist2 > (radius - 1) * (radius - 1)) {
                    color = dim_green;
                }
                frame[(size_t)py * (size_t)stride + (size_t)px] = color;
            } else if (dist2 <= outer_radius * outer_radius) {
                frame[(size_t)py * (size_t)stride + (size_t)px] = outline;
            }
        }
    }
}

static void put_pixel_alpha(uint16_t *frame, int width, int height, int stride,
                            int x, int y, uint16_t color, unsigned alpha)
{
    if (!frame || x < 0 || y < 0 || x >= width || y >= height ||
        alpha == 0) {
        return;
    }
    if (alpha >= 255u) {
        frame[(size_t)y * (size_t)stride + (size_t)x] = color;
        return;
    }

    uint16_t dst = frame[(size_t)y * (size_t)stride + (size_t)x];
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

    frame[(size_t)y * (size_t)stride + (size_t)x] =
        (uint16_t)((r << 11) | (g << 5) | b);
}

static int osk_font_px_for_scale(int scale)
{
    return scale > 1 ? R36SX_OSK_FONT_PX : R36SX_OSK_FONT_SMALL_PX;
}

static void osk_font_cache_clear(void)
{
    for (int i = 0; i < R36SX_OSK_FONT_CACHE_SLOTS; i++) {
        free(g_osk_font.cache[i].buffer);
        memset(&g_osk_font.cache[i], 0, sizeof(g_osk_font.cache[i]));
    }
}

static void osk_font_close(void)
{
    osk_font_cache_clear();
    if (g_osk_font.face && g_osk_font.done_face) {
        g_osk_font.done_face(g_osk_font.face);
    }
    if (g_osk_font.library && g_osk_font.done_free_type) {
        g_osk_font.done_free_type(g_osk_font.library);
    }
    if (g_osk_font.handle) {
        dlclose(g_osk_font.handle);
    }
    memset(&g_osk_font, 0, sizeof(g_osk_font));
    g_osk_font.attempted = 1;
}

static int osk_font_bind_symbols(void)
{
    g_osk_font.init_free_type =
        (r36sx_ft_init_free_type_fn)dlsym(g_osk_font.handle,
                                          "FT_Init_FreeType");
    g_osk_font.new_face =
        (r36sx_ft_new_face_fn)dlsym(g_osk_font.handle, "FT_New_Face");
    g_osk_font.done_face =
        (r36sx_ft_done_face_fn)dlsym(g_osk_font.handle, "FT_Done_Face");
    g_osk_font.done_free_type =
        (r36sx_ft_done_free_type_fn)dlsym(g_osk_font.handle,
                                          "FT_Done_FreeType");
    g_osk_font.select_charmap =
        (r36sx_ft_select_charmap_fn)dlsym(g_osk_font.handle,
                                          "FT_Select_Charmap");
    g_osk_font.set_pixel_sizes =
        (r36sx_ft_set_pixel_sizes_fn)dlsym(g_osk_font.handle,
                                           "FT_Set_Pixel_Sizes");
    g_osk_font.load_char =
        (r36sx_ft_load_char_fn)dlsym(g_osk_font.handle, "FT_Load_Char");
    return g_osk_font.init_free_type && g_osk_font.new_face &&
           g_osk_font.done_face && g_osk_font.done_free_type &&
           g_osk_font.set_pixel_sizes && g_osk_font.load_char;
}

static int osk_font_open(void)
{
    static const char *library_paths[] = {
        R36SX_CUBEGM_DIR "/lib/libfreetype.so.6",
        R36SX_CUBEGM_DIR "/usr/lib/libfreetype.so.6",
        R36SX_CUBEGM_DIR "/lib/libfreetype.so",
        "libfreetype.so.6"
    };
    static const char *font_paths[] = {
        R36SX_CUBEGM_DIR "/Arial_en.ttf",
        R36SX_CUBEGM_DIR "/Arial_kr.ttf",
        R36SX_CUBEGM_DIR "/font.ttf",
        R36SX_CUBEGM_DIR "/Tahoma.ttf"
    };

    if (g_osk_font.active) {
        return 0;
    }
    if (g_osk_font.attempted) {
        return -1;
    }

    memset(&g_osk_font, 0, sizeof(g_osk_font));
    g_osk_font.attempted = 1;
    for (size_t i = 0; i < R36SX_OSK_ARRAY_COUNT(library_paths); i++) {
        g_osk_font.handle = dlopen(library_paths[i], RTLD_NOW);
        if (g_osk_font.handle) {
            break;
        }
    }
    if (!g_osk_font.handle || !osk_font_bind_symbols()) {
        osk_font_close();
        return -1;
    }
    if (g_osk_font.init_free_type(&g_osk_font.library) != 0) {
        osk_font_close();
        return -1;
    }
    for (size_t i = 0; i < R36SX_OSK_ARRAY_COUNT(font_paths); i++) {
        if (access(font_paths[i], R_OK) != 0) {
            continue;
        }
        if (g_osk_font.new_face(
                g_osk_font.library, font_paths[i], 0,
                &g_osk_font.face) == 0) {
            break;
        }
    }
    if (!g_osk_font.face) {
        osk_font_close();
        return -1;
    }
    if (g_osk_font.select_charmap) {
        g_osk_font.select_charmap(g_osk_font.face, FT_ENCODING_UNICODE);
    }

    g_osk_font.active = 1;
    return 0;
}

static uint32_t osk_next_codepoint(const unsigned char **text)
{
    uint8_t c = *(*text)++;

    switch (c) {
    case 0x11: return 0x2190u;
    case 0x12: return 0x2191u;
    case 0x13: return 0x2192u;
    case 0x14: return 0x2193u;
    default: break;
    }
    if (c < 0x80u) {
        return c;
    }
    if ((c & 0xe0u) == 0xc0u && ((*text)[0] & 0xc0u) == 0x80u) {
        uint32_t cp = ((uint32_t)(c & 0x1fu) << 6) |
                      (uint32_t)((*text)[0] & 0x3fu);
        (*text)++;
        return cp;
    }
    if ((c & 0xf0u) == 0xe0u &&
        ((*text)[0] & 0xc0u) == 0x80u &&
        ((*text)[1] & 0xc0u) == 0x80u) {
        uint32_t cp = ((uint32_t)(c & 0x0fu) << 12) |
                      ((uint32_t)((*text)[0] & 0x3fu) << 6) |
                      (uint32_t)((*text)[1] & 0x3fu);
        *text += 2;
        return cp;
    }
    return '?';
}

static struct r36sx_osk_glyph_cache_entry *osk_font_cache_lookup(
    uint32_t codepoint,
    int pixel_height)
{
    for (int i = 0; i < R36SX_OSK_FONT_CACHE_SLOTS; i++) {
        if (g_osk_font.cache[i].valid &&
            g_osk_font.cache[i].codepoint == codepoint &&
            g_osk_font.cache[i].pixel_height == (uint16_t)pixel_height) {
            g_osk_font.cache[i].age = ++g_osk_font.cache_age;
            return &g_osk_font.cache[i];
        }
    }
    return NULL;
}

static struct r36sx_osk_glyph_cache_entry *osk_font_cache_alloc_slot(void)
{
    int slot = 0;
    uint32_t oldest = UINT32_MAX;

    for (int i = 0; i < R36SX_OSK_FONT_CACHE_SLOTS; i++) {
        if (!g_osk_font.cache[i].valid) {
            slot = i;
            break;
        }
        if (g_osk_font.cache[i].age < oldest) {
            oldest = g_osk_font.cache[i].age;
            slot = i;
        }
    }

    free(g_osk_font.cache[slot].buffer);
    memset(&g_osk_font.cache[slot], 0, sizeof(g_osk_font.cache[slot]));
    return &g_osk_font.cache[slot];
}

static struct r36sx_osk_glyph_cache_entry *osk_font_load_glyph(
    uint32_t codepoint,
    int pixel_height)
{
    struct r36sx_osk_glyph_cache_entry *entry =
        osk_font_cache_lookup(codepoint, pixel_height);
    if (entry) {
        return entry;
    }
    if (!g_osk_font.active || !g_osk_font.face) {
        return NULL;
    }
    if (g_osk_font.set_pixel_sizes(
            g_osk_font.face, 0, (FT_UInt)pixel_height) != 0) {
        return NULL;
    }
    if (g_osk_font.load_char(g_osk_font.face, (FT_ULong)codepoint,
                             FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT) != 0) {
        if (codepoint != '?') {
            return osk_font_load_glyph('?', pixel_height);
        }
        return NULL;
    }

    FT_GlyphSlot slot = g_osk_font.face->glyph;
    FT_Bitmap *bitmap = &slot->bitmap;
    if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) {
        return NULL;
    }

    entry = osk_font_cache_alloc_slot();
    entry->codepoint = codepoint;
    entry->pixel_height = (uint16_t)pixel_height;
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
                : bitmap->buffer +
                  (size_t)(entry->rows - 1 - row) * (size_t)pitch;
            memcpy(entry->buffer + (size_t)row * (size_t)entry->pitch,
                   src, (size_t)entry->width);
        }
    }

    entry->valid = 1;
    entry->age = ++g_osk_font.cache_age;
    return entry;
}

static int osk_font_text_width(const char *text, int scale)
{
    if (!text || osk_font_open() != 0) {
        return -1;
    }

    const unsigned char *p = (const unsigned char *)text;
    int pixel_height = osk_font_px_for_scale(scale);
    int width = 0;
    while (*p != '\0') {
        uint32_t cp = osk_next_codepoint(&p);
        struct r36sx_osk_glyph_cache_entry *glyph =
            osk_font_load_glyph(cp, pixel_height);
        if (!glyph) {
            return -1;
        }
        width += glyph->advance;
    }
    return width;
}

static void osk_font_draw_glyph(
    uint16_t *frame,
    int width,
    int height,
    int stride,
    const struct r36sx_osk_glyph_cache_entry *glyph,
    int x,
    int y,
    uint16_t color)
{
    for (int row = 0; row < glyph->rows; row++) {
        for (int col = 0; col < glyph->width; col++) {
            unsigned alpha =
                glyph->buffer[(size_t)row * (size_t)glyph->pitch +
                              (size_t)col];
            put_pixel_alpha(frame, width, height, stride,
                            x + col, y + row, color, alpha);
        }
    }
}

static int osk_font_draw_text(uint16_t *frame, int width, int height,
                              int stride, int x, int y, const char *text,
                              uint16_t color, int scale)
{
    if (!text || osk_font_open() != 0) {
        return 0;
    }

    const unsigned char *p = (const unsigned char *)text;
    int pixel_height = osk_font_px_for_scale(scale);
    int baseline = y + pixel_height - 2;
    while (*p != '\0') {
        uint32_t cp = osk_next_codepoint(&p);
        struct r36sx_osk_glyph_cache_entry *glyph =
            osk_font_load_glyph(cp, pixel_height);
        if (!glyph) {
            return 0;
        }
        if (glyph->buffer) {
            int gx = x + glyph->bitmap_left;
            int gy = baseline - glyph->bitmap_top;
            osk_font_draw_glyph(frame, width, height, stride, glyph,
                                gx, gy, color);
        }
        x += glyph->advance;
    }
    return 1;
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
    static const uint8_t glyph_la[7] = {0, 0, 14, 1, 15, 17, 15};
    static const uint8_t glyph_lb[7] = {16, 16, 22, 25, 17, 17, 30};
    static const uint8_t glyph_lc[7] = {0, 0, 14, 16, 16, 17, 14};
    static const uint8_t glyph_ld[7] = {1, 1, 13, 19, 17, 17, 15};
    static const uint8_t glyph_le[7] = {0, 0, 14, 17, 31, 16, 14};
    static const uint8_t glyph_lf[7] = {6, 8, 8, 30, 8, 8, 8};
    static const uint8_t glyph_lg[7] = {0, 0, 15, 17, 15, 1, 14};
    static const uint8_t glyph_lh[7] = {16, 16, 22, 25, 17, 17, 17};
    static const uint8_t glyph_li[7] = {4, 0, 12, 4, 4, 4, 14};
    static const uint8_t glyph_lj[7] = {2, 0, 6, 2, 2, 18, 12};
    static const uint8_t glyph_lk[7] = {16, 16, 18, 20, 24, 20, 18};
    static const uint8_t glyph_ll[7] = {12, 4, 4, 4, 4, 4, 14};
    static const uint8_t glyph_lm[7] = {0, 0, 26, 21, 21, 21, 21};
    static const uint8_t glyph_ln[7] = {0, 0, 22, 25, 17, 17, 17};
    static const uint8_t glyph_lo[7] = {0, 0, 14, 17, 17, 17, 14};
    static const uint8_t glyph_lp[7] = {0, 0, 30, 17, 30, 16, 16};
    static const uint8_t glyph_lq[7] = {0, 0, 15, 17, 15, 1, 1};
    static const uint8_t glyph_lr[7] = {0, 0, 22, 25, 16, 16, 16};
    static const uint8_t glyph_ls[7] = {0, 0, 15, 16, 14, 1, 30};
    static const uint8_t glyph_lt[7] = {8, 8, 30, 8, 8, 9, 6};
    static const uint8_t glyph_lu[7] = {0, 0, 17, 17, 17, 19, 13};
    static const uint8_t glyph_lv[7] = {0, 0, 17, 17, 17, 10, 4};
    static const uint8_t glyph_lw[7] = {0, 0, 17, 17, 21, 21, 10};
    static const uint8_t glyph_lx[7] = {0, 0, 17, 10, 4, 10, 17};
    static const uint8_t glyph_ly[7] = {0, 0, 17, 17, 15, 1, 14};
    static const uint8_t glyph_lz[7] = {0, 0, 31, 2, 4, 8, 31};
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
    static const uint8_t glyph_semicolon[7] = {0, 4, 4, 0, 4, 4, 8};
    static const uint8_t glyph_quote[7] = {4, 4, 8, 0, 0, 0, 0};
    static const uint8_t glyph_dquote[7] = {10, 10, 10, 0, 0, 0, 0};
    static const uint8_t glyph_comma[7] = {0, 0, 0, 0, 4, 4, 8};
    static const uint8_t glyph_period[7] = {0, 0, 0, 0, 0, 12, 12};
    static const uint8_t glyph_slash[7] = {1, 1, 2, 4, 8, 16, 16};
    static const uint8_t glyph_backslash[7] = {16, 16, 8, 4, 2, 1, 1};
    static const uint8_t glyph_lbracket[7] = {14, 8, 8, 8, 8, 8, 14};
    static const uint8_t glyph_rbracket[7] = {14, 2, 2, 2, 2, 2, 14};
    static const uint8_t glyph_grave[7] = {8, 4, 2, 0, 0, 0, 0};
    static const uint8_t glyph_exclaim[7] = {4, 4, 4, 4, 4, 0, 4};
    static const uint8_t glyph_at[7] = {14, 17, 23, 21, 23, 16, 14};
    static const uint8_t glyph_hash[7] = {10, 10, 31, 10, 31, 10, 10};
    static const uint8_t glyph_dollar[7] = {4, 15, 20, 14, 5, 30, 4};
    static const uint8_t glyph_percent[7] = {24, 25, 2, 4, 8, 19, 3};
    static const uint8_t glyph_caret[7] = {4, 10, 17, 0, 0, 0, 0};
    static const uint8_t glyph_amp[7] = {12, 18, 20, 8, 21, 18, 13};
    static const uint8_t glyph_star[7] = {0, 21, 14, 31, 14, 21, 0};
    static const uint8_t glyph_lparen[7] = {2, 4, 8, 8, 8, 4, 2};
    static const uint8_t glyph_rparen[7] = {8, 4, 2, 2, 2, 4, 8};
    static const uint8_t glyph_underscore[7] = {0, 0, 0, 0, 0, 0, 31};
    static const uint8_t glyph_plus[7] = {0, 4, 4, 31, 4, 4, 0};
    static const uint8_t glyph_less[7] = {2, 4, 8, 16, 8, 4, 2};
    static const uint8_t glyph_greater[7] = {8, 4, 2, 1, 2, 4, 8};
    static const uint8_t glyph_question[7] = {14, 17, 1, 2, 4, 0, 4};
    static const uint8_t glyph_pipe[7] = {4, 4, 4, 4, 4, 4, 4};
    static const uint8_t glyph_lbrace[7] = {6, 8, 8, 16, 8, 8, 6};
    static const uint8_t glyph_rbrace[7] = {12, 2, 2, 1, 2, 2, 12};
    static const uint8_t glyph_left[7] = {0, 4, 8, 31, 8, 4, 0};
    static const uint8_t glyph_up[7] = {0, 4, 14, 21, 4, 4, 0};
    static const uint8_t glyph_right[7] = {0, 4, 2, 31, 2, 4, 0};
    static const uint8_t glyph_down[7] = {0, 4, 4, 21, 14, 4, 0};
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
    case 'a': glyph = glyph_la; break;
    case 'b': glyph = glyph_lb; break;
    case 'c': glyph = glyph_lc; break;
    case 'd': glyph = glyph_ld; break;
    case 'e': glyph = glyph_le; break;
    case 'f': glyph = glyph_lf; break;
    case 'g': glyph = glyph_lg; break;
    case 'h': glyph = glyph_lh; break;
    case 'i': glyph = glyph_li; break;
    case 'j': glyph = glyph_lj; break;
    case 'k': glyph = glyph_lk; break;
    case 'l': glyph = glyph_ll; break;
    case 'm': glyph = glyph_lm; break;
    case 'n': glyph = glyph_ln; break;
    case 'o': glyph = glyph_lo; break;
    case 'p': glyph = glyph_lp; break;
    case 'q': glyph = glyph_lq; break;
    case 'r': glyph = glyph_lr; break;
    case 's': glyph = glyph_ls; break;
    case 't': glyph = glyph_lt; break;
    case 'u': glyph = glyph_lu; break;
    case 'v': glyph = glyph_lv; break;
    case 'w': glyph = glyph_lw; break;
    case 'x': glyph = glyph_lx; break;
    case 'y': glyph = glyph_ly; break;
    case 'z': glyph = glyph_lz; break;
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
    case ';': glyph = glyph_semicolon; break;
    case '\'': glyph = glyph_quote; break;
    case '"': glyph = glyph_dquote; break;
    case ',': glyph = glyph_comma; break;
    case '.': glyph = glyph_period; break;
    case '/': glyph = glyph_slash; break;
    case '\\': glyph = glyph_backslash; break;
    case '[': glyph = glyph_lbracket; break;
    case ']': glyph = glyph_rbracket; break;
    case '`': glyph = glyph_grave; break;
    case '!': glyph = glyph_exclaim; break;
    case '@': glyph = glyph_at; break;
    case '#': glyph = glyph_hash; break;
    case '$': glyph = glyph_dollar; break;
    case '%': glyph = glyph_percent; break;
    case '^': glyph = glyph_caret; break;
    case '&': glyph = glyph_amp; break;
    case '*': glyph = glyph_star; break;
    case '(': glyph = glyph_lparen; break;
    case ')': glyph = glyph_rparen; break;
    case '_': glyph = glyph_underscore; break;
    case '+': glyph = glyph_plus; break;
    case '<': glyph = glyph_less; break;
    case '>': glyph = glyph_greater; break;
    case '?': glyph = glyph_question; break;
    case '|': glyph = glyph_pipe; break;
    case '{': glyph = glyph_lbrace; break;
    case '}': glyph = glyph_rbrace; break;
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
    int ft_width = osk_font_text_width(text, scale);
    if (ft_width >= 0) {
        return ft_width;
    }

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
    if (osk_font_draw_text(frame, width, height, stride, x, y, text, color,
                           scale)) {
        return;
    }

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
    if (keycode == R36SX_SCREEN_KEY_CAPITAL) {
        keyboard->caps_lock ^= 1u;
    }
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

static int keyboard_row_count(void)
{
    return (int)R36SX_OSK_ARRAY_COUNT(g_osk_rows);
}

static int keyboard_rows_h(void)
{
    int rows = keyboard_row_count();
    return rows * R36SX_OSK_KEY_H + (rows - 1) * R36SX_OSK_KEY_GAP;
}

static int keyboard_fit_panel_h(void)
{
    return 2 + R36SX_OSK_HEADER_H + 2 * R36SX_OSK_INNER_PAD +
           keyboard_rows_h();
}

static int keyboard_view_h_for_panel(int panel_h)
{
    int view_h = panel_h - 2 - R36SX_OSK_HEADER_H -
                 2 * R36SX_OSK_INNER_PAD;
    return view_h > 0 ? view_h : 0;
}

static int keyboard_view_h_for_state(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->expanded ?
        keyboard_view_h_for_panel(keyboard_fit_panel_h()) :
        keyboard_view_h_for_panel(R36SX_SCREEN_KEYBOARD_PANEL_H);
}

static int keyboard_scroll_max_for_view(int view_h)
{
    int max_scroll = keyboard_rows_h() - view_h;
    return max_scroll > 0 ? max_scroll : 0;
}

static int keyboard_scroll_max_for_state(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard_scroll_max_for_view(keyboard_view_h_for_state(keyboard));
}

static int key_units(const struct r36sx_osk_key *key)
{
    return key && key->units != 0 ? key->units : R36SX_OSK_KEY_DEFAULT_UNITS;
}

static int row_extra_gap_units(int row, int col)
{
    if (row != 0) {
        return 0;
    }

    /* Keep the function-key row close to a real PC keyboard grouping. */
    if (col == 0) {
        return R36SX_OSK_UNIT_1;
    }
    if (col == 4 || col == 8) {
        return R36SX_OSK_UNIT_1 / 2;
    }
    return 0;
}

static int row_units(const struct r36sx_osk_key *const *rows,
                     const uint8_t *counts,
                     int row)
{
    int units = 0;
    for (int col = 0; col < counts[row]; col++) {
        units += key_units(&rows[row][col]);
        if (col + 1 < counts[row]) {
            units += row_extra_gap_units(row, col);
        }
    }
    return units;
}

static int max_main_row_units(const struct r36sx_osk_key *const *rows,
                              const uint8_t *counts,
                              int *max_gaps)
{
    int max_units = 1;
    int gaps = 0;
    for (int row = 0; row < keyboard_row_count(); row++) {
        int units = row_units(rows, counts, row);
        int row_gaps = counts[row] > 0 ? (int)counts[row] - 1 : 0;
        if (units > max_units) {
            max_units = units;
        }
        if (row_gaps > gaps) {
            gaps = row_gaps;
        }
    }
    if (max_gaps) {
        *max_gaps = gaps;
    }
    return max_units;
}

static int main_unit_w(const struct r36sx_osk_key *const *rows,
                       const uint8_t *counts,
                       int main_w)
{
    int max_gaps = 0;
    int max_units = max_main_row_units(rows, counts, &max_gaps);
    int unit_w = (main_w - max_gaps * R36SX_OSK_KEY_GAP) / max_units;
    return unit_w > R36SX_OSK_MIN_UNIT_W ? unit_w : R36SX_OSK_MIN_UNIT_W;
}

static int key_pixel_w(const struct r36sx_osk_key *key, int unit_w)
{
    return key_units(key) * unit_w;
}

static int row_extra_gap_w(int row, int col, int unit_w)
{
    int gap = row_extra_gap_units(row, col) * unit_w;

    /*
     * Nudge the late function-key group so the F10/F11 split lines up with
     * the =/Backspace split in the number row.  The wide Backspace key on a
     * real keyboard visually consumes that internal gap.
     */
    if (row == 0 && col == 0) {
        gap += 1;
    }
    if (row == 0 && col == 4) {
        gap -= 1;
    }
    if (row == 0 && col == 8) {
        gap += 3;
    }
    return gap;
}

static int row_pixel_w(const struct r36sx_osk_key *const *rows,
                       const uint8_t *counts,
                       int row,
                       int unit_w)
{
    int width = 0;

    for (int col = 0; col < counts[row]; col++) {
        width += key_pixel_w(&rows[row][col], unit_w);
        if (col + 1 < counts[row]) {
            width += R36SX_OSK_KEY_GAP +
                     row_extra_gap_w(row, col, unit_w);
        }
    }
    return width;
}

static int max_main_row_pixel_w(const struct r36sx_osk_key *const *rows,
                                const uint8_t *counts,
                                int unit_w)
{
    int max_width = 1;

    for (int row = 0; row < keyboard_row_count(); row++) {
        int width = row_pixel_w(rows, counts, row, unit_w);
        if (width > max_width) {
            max_width = width;
        }
    }
    return max_width;
}

static int row_tail_extra_col(int row, int count)
{
    /*
     * Keep the bottom-row gaps visually identical to the other rows by letting
     * the spacebar absorb the extra width instead of widening right Ctrl.
     */
    if (row == 5 && count > 3) {
        return 3;
    }
    return count - 1;
}

static int key_text_scale(const char *label, int key_w)
{
    if (text_width(label, R36SX_OSK_TEXT_SCALE) <= key_w - 4 &&
        osk_font_px_for_scale(R36SX_OSK_TEXT_SCALE) <=
            R36SX_OSK_KEY_H - 2) {
        return R36SX_OSK_TEXT_SCALE;
    }
    return 1;
}

static void update_scroll_for_selection(struct r36sx_screen_keyboard *keyboard)
{
    int selected_row;
    int row_y;
    int view_h = keyboard_view_h_for_state(keyboard);
    int max_scroll = keyboard_scroll_max_for_state(keyboard);
    int scroll;

    if (!keyboard) {
        return;
    }
    selected_row = keyboard->zone == R36SX_OSK_ZONE_CURSOR ?
        (int)keyboard->row : (int)keyboard->row;
    if (selected_row < 0) {
        selected_row = 0;
    } else if (selected_row >= keyboard_row_count()) {
        selected_row = keyboard_row_count() - 1;
    }

    row_y = selected_row * R36SX_OSK_ROW_STEP;
    scroll = keyboard->scroll_y;
    if (row_y < scroll) {
        scroll = row_y;
    } else if (row_y + R36SX_OSK_KEY_H > scroll + view_h) {
        scroll = row_y + R36SX_OSK_KEY_H - view_h;
    }
    if (scroll < 0) {
        scroll = 0;
    } else if (scroll > max_scroll) {
        scroll = max_scroll;
    }
    keyboard->scroll_y = (uint8_t)scroll;
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

static int nearest_cursor_col(int row, int preferred_col)
{
    int best_col = R36SX_OSK_NO_CURSOR_KEY;
    int best_distance = R36SX_OSK_CURSOR_BLOCK_COLS + 1;

    if (row < 0 || row >= R36SX_OSK_CURSOR_BLOCK_ROWS) {
        return R36SX_OSK_NO_CURSOR_KEY;
    }
    for (int col = 0; col < R36SX_OSK_CURSOR_BLOCK_COLS; col++) {
        int distance;
        if (!cursor_key_at(row, col)) {
            continue;
        }
        distance = preferred_col > col ? preferred_col - col :
            col - preferred_col;
        if (distance < best_distance) {
            best_distance = distance;
            best_col = col;
        }
    }
    return best_col;
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
        keyboard->col = 0;
    }
    if (!cursor_key_at(keyboard->row, keyboard->col)) {
        int col = nearest_cursor_col(keyboard->row, keyboard->col);
        if (col < 0) {
            keyboard->row = 0;
            col = nearest_cursor_col(0, 0);
        }
        keyboard->col = (uint8_t)(col < 0 ? 0 : col);
    }
}

static int cursor_row_for_main_row(int row)
{
    if (row <= 2) {
        return row;
    }
    if (row >= keyboard_row_count() - 1) {
        return R36SX_OSK_CURSOR_BLOCK_ROWS - 1;
    }
    return R36SX_OSK_CURSOR_BLOCK_ROWS - 2;
}

static void enter_cursor_block(struct r36sx_screen_keyboard *keyboard)
{
    int row = cursor_row_for_main_row(keyboard->row);
    int col = nearest_cursor_col(row, 0);
    keyboard->zone = R36SX_OSK_ZONE_CURSOR;
    keyboard->row = (uint8_t)row;
    keyboard->col = (uint8_t)(col < 0 ? 0 : col);
    update_scroll_for_selection(keyboard);
}

static void leave_cursor_block(struct r36sx_screen_keyboard *keyboard)
{
    const uint8_t *counts = active_row_counts(keyboard);
    int row = (int)keyboard->row;
    int row_count = keyboard_row_count();

    if (row >= row_count) {
        row = row_count - 1;
    }
    keyboard->zone = R36SX_OSK_ZONE_MAIN;
    keyboard->row = (uint8_t)row;
    keyboard->col = (uint8_t)(counts[row] - 1);
    update_scroll_for_selection(keyboard);
}

static void move_cursor_selection(struct r36sx_screen_keyboard *keyboard,
                                  int dx, int dy)
{
    if (dy != 0) {
        int col = (int)keyboard->col;
        int row = (int)keyboard->row + dy;
        while (row >= 0 && row < R36SX_OSK_CURSOR_BLOCK_ROWS) {
            if (cursor_key_at(row, col)) {
                keyboard->row = (uint8_t)row;
                keyboard->col = (uint8_t)col;
                return;
            }
            row += dy;
        }
        return;
    }

    if (dx < 0) {
        int col = (int)keyboard->col - 1;
        while (col >= 0 && !cursor_key_at(keyboard->row, col)) {
            col--;
        }
        if (col >= 0) {
            keyboard->col = (uint8_t)col;
        } else {
            leave_cursor_block(keyboard);
        }
    } else if (dx > 0) {
        int col = (int)keyboard->col + 1;
        while (col < R36SX_OSK_CURSOR_BLOCK_COLS &&
               !cursor_key_at(keyboard->row, col)) {
            col++;
        }
        if (col < R36SX_OSK_CURSOR_BLOCK_COLS) {
            keyboard->col = (uint8_t)col;
        }
    }
}

static void move_main_selection(struct r36sx_screen_keyboard *keyboard, int dx,
                                int dy)
{
    const uint8_t *counts = active_row_counts(keyboard);
    int row_count = keyboard_row_count();
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
    update_scroll_for_selection(keyboard);
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

    if (!keyboard || repeat_button == 0 || keycode == 0 ||
        keycode == R36SX_SCREEN_KEY_CAPITAL) {
        return;
    }

    keyboard->key_repeat_button = repeat_button;
    keyboard->key_repeat_keycode = keycode;
    keyboard->key_repeat_force_shift = (uint8_t)(force_shift != 0);
    keyboard->key_repeat_next_us = now_us() + R36SX_OSK_KEY_REPEAT_DELAY_US;
}

static void handle_physical_modifiers(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t held,
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user)
{
    uint8_t shift_down = (held & R36SX_RKGAME_KEY_L) != 0;
    uint8_t ctrl_down = (held & R36SX_RKGAME_KEY_R) != 0;

    if (!keyboard || !emit) {
        return;
    }
    if (shift_down != keyboard->physical_shift) {
        emit(emit_user, R36SX_SCREEN_KEY_SHIFT, shift_down);
        keyboard->physical_shift = shift_down;
    }
    if (ctrl_down != keyboard->physical_ctrl) {
        emit(emit_user, R36SX_SCREEN_KEY_CONTROL, ctrl_down);
        keyboard->physical_ctrl = ctrl_down;
    }
}

static void release_physical_modifiers(
    struct r36sx_screen_keyboard *keyboard,
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user)
{
    if (!keyboard || !emit) {
        return;
    }
    if (keyboard->physical_shift) {
        emit(emit_user, R36SX_SCREEN_KEY_SHIFT, 0);
        keyboard->physical_shift = 0;
    }
    if (keyboard->physical_ctrl) {
        emit(emit_user, R36SX_SCREEN_KEY_CONTROL, 0);
        keyboard->physical_ctrl = 0;
    }
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
    if (keyboard->row >= keyboard_row_count()) {
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

    for (size_t row = 0; row < (size_t)keyboard_row_count(); row++) {
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
        release_physical_modifiers(keyboard, emit, emit_user);
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

static const char *shifted_label_for_key(uint16_t keycode)
{
    switch (keycode) {
    case '1': return "!";
    case '2': return "@";
    case '3': return "#";
    case '4': return "$";
    case '5': return "%";
    case '6': return "^";
    case '7': return "&";
    case '8': return "*";
    case '9': return "(";
    case '0': return ")";
    case R36SX_SCREEN_KEY_OEM_3: return "~";
    case R36SX_SCREEN_KEY_OEM_MINUS: return "_";
    case R36SX_SCREEN_KEY_OEM_PLUS: return "+";
    case R36SX_SCREEN_KEY_OEM_4: return "{";
    case R36SX_SCREEN_KEY_OEM_6: return "}";
    case R36SX_SCREEN_KEY_OEM_5: return "|";
    case R36SX_SCREEN_KEY_OEM_1: return ":";
    case R36SX_SCREEN_KEY_OEM_7: return "\"";
    case R36SX_SCREEN_KEY_OEM_COMMA: return "<";
    case R36SX_SCREEN_KEY_OEM_PERIOD: return ">";
    case R36SX_SCREEN_KEY_OEM_2: return "?";
    default: return NULL;
    }
}

static const char *display_label_for_key(
    const struct r36sx_screen_keyboard *keyboard,
    const struct r36sx_osk_key *key,
    char *scratch,
    size_t scratch_size)
{
    if (!keyboard || !key) {
        return "";
    }
    if ((key->flags & R36SX_OSK_FLAG_SHIFTED) == 0 &&
        key->keycode >= 'A' && key->keycode <= 'Z' && scratch_size >= 2) {
        int upper = (keyboard->shift || keyboard->physical_shift) ^
                    keyboard->caps_lock;
        scratch[0] = (char)((upper ? 'A' : 'a') +
                            (key->keycode - 'A'));
        scratch[1] = '\0';
        return scratch;
    }
    if ((keyboard->shift || keyboard->physical_shift) &&
        (key->flags & R36SX_OSK_FLAG_SHIFTED) == 0) {
        const char *shifted = shifted_label_for_key(key->keycode);
        if (shifted) {
            return shifted;
        }
    }
    return key->label;
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
        ((key->flags & R36SX_OSK_FLAG_SHIFT_MOD) &&
         (keyboard->shift || keyboard->physical_shift)) ||
        ((key->flags & R36SX_OSK_FLAG_CTRL_MOD) &&
         (keyboard->ctrl || keyboard->physical_ctrl)) ||
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
        char label_buf[2];
        const char *label =
            display_label_for_key(keyboard, key, label_buf,
                                  sizeof(label_buf));
        int scale = key_text_scale(label, key_w);
        int text_w = text_width(label, scale);
        int text_h = osk_font_open() == 0 ?
            osk_font_px_for_scale(scale) : 7 * scale;
        int text_x = x + (key_w - text_w) / 2;
        int text_y = y + (R36SX_OSK_KEY_H - text_h) / 2;
        if (key->keycode == R36SX_SCREEN_KEY_CAPITAL) {
            int led_outer = R36SX_OSK_CAPS_LED_OUTER_RADIUS;
            int led_pad = 4;
            int label_w = key_w - (led_outer * 2 + 1) - led_pad - 4;
            if (label_w < 1) {
                label_w = 1;
            }
            scale = key_text_scale(label, label_w);
            text_w = text_width(label, scale);
            text_h = osk_font_open() == 0 ?
                osk_font_px_for_scale(scale) : 7 * scale;
            text_x = x + 4;
            text_y = y + (R36SX_OSK_KEY_H - text_h) / 2;
            draw_caps_lock_led(frame, width, height, stride,
                               x + key_w - led_outer - led_pad,
                               y + R36SX_OSK_KEY_H / 2,
                               keyboard->caps_lock != 0);
        }
        draw_text(frame, width, height, stride, text_x, text_y, label, fg,
                  scale);
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
    keyboard->physical_shift = 0;
    keyboard->physical_ctrl = 0;
    keyboard->caps_lock = 0;
    keyboard->expanded = 0;
    keyboard->symbol_mode = 0;
    keyboard->cursor_block = 0;
    keyboard->scroll_y = 0;
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
        keyboard->row < keyboard_row_count()) {
        counts = active_row_counts(keyboard);
        if (keyboard->col >= counts[keyboard->row]) {
            keyboard->col = (uint8_t)(counts[keyboard->row] - 1);
        }
    }
    update_scroll_for_selection(keyboard);
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
        keyboard->physical_shift = 0;
        keyboard->physical_ctrl = 0;
        keyboard->symbol_mode = 0;
        keyboard->scroll_y = 0;
        keyboard->expanded = 0;
        keyboard->press_buttons = 0;
        reset_nav_repeat(keyboard);
        reset_key_repeat(keyboard);
    } else {
        update_scroll_for_selection(keyboard);
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
    update_scroll_for_selection(keyboard);
}

int r36sx_screen_keyboard_cursor_block_enabled(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->cursor_block != 0;
}

void r36sx_screen_keyboard_set_expanded(
    struct r36sx_screen_keyboard *keyboard, int expanded)
{
    if (!keyboard) {
        return;
    }
    keyboard->expanded = (uint8_t)(expanded != 0);
    keyboard->press_buttons = 0;
    reset_nav_repeat(keyboard);
    reset_key_repeat(keyboard);
    update_scroll_for_selection(keyboard);
}

int r36sx_screen_keyboard_is_expanded(
    const struct r36sx_screen_keyboard *keyboard)
{
    return keyboard && keyboard->expanded != 0;
}

int r36sx_screen_keyboard_fit_panel_height(int framebuffer_height)
{
    int panel_h = keyboard_fit_panel_h();
    if (framebuffer_height <= 0) {
        return 0;
    }
    if (panel_h > framebuffer_height) {
        panel_h = framebuffer_height;
    }
    return panel_h;
}

int r36sx_screen_keyboard_panel_height(
    const struct r36sx_screen_keyboard *keyboard, int framebuffer_height)
{
    if (framebuffer_height <= 0) {
        return 0;
    }
    if (r36sx_screen_keyboard_is_visible(keyboard) &&
        r36sx_screen_keyboard_is_expanded(keyboard)) {
        return r36sx_screen_keyboard_fit_panel_height(framebuffer_height);
    }
    return framebuffer_height > R36SX_SCREEN_KEYBOARD_PANEL_H ?
        R36SX_SCREEN_KEYBOARD_PANEL_H : framebuffer_height;
}

int r36sx_screen_keyboard_panel_y(int framebuffer_height)
{
    return framebuffer_height > R36SX_SCREEN_KEYBOARD_PANEL_H ?
        framebuffer_height - R36SX_SCREEN_KEYBOARD_PANEL_H : 0;
}

int r36sx_screen_keyboard_panel_y_for(
    const struct r36sx_screen_keyboard *keyboard, int framebuffer_height)
{
    int panel_h = r36sx_screen_keyboard_panel_height(keyboard,
                                                    framebuffer_height);
    return framebuffer_height > panel_h ? framebuffer_height - panel_h : 0;
}

int r36sx_screen_keyboard_content_height(
    const struct r36sx_screen_keyboard *keyboard, int framebuffer_height)
{
    return r36sx_screen_keyboard_is_visible(keyboard) ?
        r36sx_screen_keyboard_panel_y_for(keyboard, framebuffer_height) :
        framebuffer_height;
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
    handle_physical_modifiers(keyboard, held, emit, emit_user);
    nav_buttons = nav_buttons_with_repeat(keyboard, pressed, held);
    handle_navigation(keyboard, nav_buttons);
    if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
        r36sx_screen_keyboard_set_expanded(keyboard, !keyboard->expanded);
        result |= R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
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
    if ((pressed & R36SX_RKGAME_KEY_L) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_SHIFT,
                                      pressed & R36SX_RKGAME_KEY_L);
        if (keycode) {
            *keycode = R36SX_SCREEN_KEY_SHIFT;
        }
        return R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
    }
    if ((pressed & R36SX_RKGAME_KEY_R) != 0) {
        start_keycode_press_animation(keyboard, R36SX_SCREEN_KEY_CONTROL,
                                      pressed & R36SX_RKGAME_KEY_R);
        if (keycode) {
            *keycode = R36SX_SCREEN_KEY_CONTROL;
        }
        return R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED;
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
    const int panel_x = 0;
    const int panel_w = width;
    const int panel_h = r36sx_screen_keyboard_panel_height(keyboard, height);
    const int panel_y = r36sx_screen_keyboard_panel_y_for(keyboard, height);
    const int content_x = panel_x + 1 + R36SX_OSK_INNER_PAD;
    const int content_w = panel_w - 2 * (1 + R36SX_OSK_INNER_PAD);
    const int compact = r36sx_screen_keyboard_cursor_block_enabled(keyboard);
    const int side_w = compact ? R36SX_OSK_CURSOR_BLOCK_W : 0;
    const int side_gap = compact ? R36SX_OSK_CURSOR_BLOCK_GAP : 0;
    const int main_w = compact ?
        content_w - side_w - side_gap : content_w;
    const int unit_w = main_unit_w(rows, counts, main_w);
    const int row_target_w = max_main_row_pixel_w(rows, counts, unit_w);
    const int view_y = panel_y + 1 + R36SX_OSK_HEADER_H +
        R36SX_OSK_INNER_PAD;
    const int view_h = keyboard_view_h_for_panel(panel_h);
    const int view_bottom = view_y + view_h;
    const int keys_y = view_y - (int)keyboard->scroll_y;
    const int cursor_x = content_x + main_w + side_gap;
    const int max_scroll = keyboard_scroll_max_for_view(view_h);
    const uint16_t panel = rgb565(12, 18, 24);
    const uint16_t header = rgb565(24, 54, 70);
    const uint16_t border = rgb565(160, 192, 204);
    const uint16_t text = rgb565(228, 236, 224);

    if (!r36sx_screen_keyboard_is_visible(keyboard) || !frame || width <= 0 ||
        height <= 0 || stride_pixels <= 0 || content_w <= 0) {
        return;
    }

    fill_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
              panel_h, panel);
    stroke_rect(frame, width, height, stride_pixels, panel_x, panel_y, panel_w,
                panel_h, border);
    fill_rect(frame, width, height, stride_pixels, panel_x + 1, panel_y + 1,
              panel_w - 2, R36SX_OSK_HEADER_H, header);
    draw_text(frame, width, height, stride_pixels, panel_x + 5, panel_y + 3,
              keyboard->symbol_mode ?
                  (keyboard->expanded ?
                   "DOS KBD SYM  A/START=TYPE B=BACK Y=ENTER X=ESC L=SHIFT R=CTRL SEL=MIN" :
                   "DOS KBD SYM  A/START=TYPE B=BACK Y=ENTER X=ESC L=SHIFT R=CTRL SEL=FIT") :
                  (keyboard->expanded ?
                   "DOS KBD ABC  A/START=TYPE B=BACK Y=ENTER X=ESC L=SHIFT R=CTRL SEL=MIN" :
                   "DOS KBD ABC  A/START=TYPE B=BACK Y=ENTER X=ESC L=SHIFT R=CTRL SEL=FIT"),
              text, 1);

    for (size_t row = 0; row < (size_t)keyboard_row_count(); row++) {
        int count = counts[row];
        int x = content_x;
        int y = keys_y + (int)row * R36SX_OSK_ROW_STEP;
        int row_w = row_pixel_w(rows, counts, (int)row, unit_w);
        int tail_extra = row_target_w > row_w ? row_target_w - row_w : 0;
        int tail_col = row_tail_extra_col((int)row, count);
        if (y < view_y || y + R36SX_OSK_KEY_H > view_bottom) {
            continue;
        }
        for (int col = 0; col < count; col++) {
            int key_w = key_pixel_w(&rows[row][col], unit_w);
            if (col == tail_col) {
                key_w += tail_extra;
            }
            draw_key(keyboard, &rows[row][col], frame, width, height,
                     stride_pixels, x, y, key_w,
                     keyboard->zone == R36SX_OSK_ZONE_MAIN &&
                     row == keyboard->row && col == keyboard->col,
                     key_is_pressed(keyboard, R36SX_OSK_ZONE_MAIN,
                                    (uint8_t)row, (uint8_t)col));
            x += key_w + R36SX_OSK_KEY_GAP +
                 row_extra_gap_w((int)row, col, unit_w);
        }
    }

    if (compact) {
        for (int row = 0; row < R36SX_OSK_CURSOR_BLOCK_ROWS; row++) {
            int y = keys_y + row * R36SX_OSK_ROW_STEP;
            if (y < view_y || y + R36SX_OSK_KEY_H > view_bottom) {
                continue;
            }
            for (int col = 0; col < R36SX_OSK_CURSOR_BLOCK_COLS; col++) {
                const struct r36sx_osk_key *key = cursor_key_at(row, col);
                if (!key) {
                    continue;
                }
                draw_key(keyboard, key, frame, width, height, stride_pixels,
                         cursor_x + col * (R36SX_OSK_SIDE_KEY_W +
                                           R36SX_OSK_CURSOR_GAP),
                         y,
                         R36SX_OSK_SIDE_KEY_W,
                         keyboard->zone == R36SX_OSK_ZONE_CURSOR &&
                         row == keyboard->row && col == keyboard->col,
                         key_is_pressed(keyboard, R36SX_OSK_ZONE_CURSOR,
                                        (uint8_t)row, (uint8_t)col));
            }
        }
    }

    if (max_scroll > 0) {
        int track_x = panel_x + panel_w - 4;
        int track_y = view_y;
        int track_h = view_h;
        int thumb_h = (view_h * view_h) / keyboard_rows_h();
        int thumb_y;
        if (thumb_h < 8) {
            thumb_h = 8;
        }
        thumb_y = track_y +
            ((view_h - thumb_h) * (int)keyboard->scroll_y) / max_scroll;
        fill_rect(frame, width, height, stride_pixels, track_x, track_y, 2,
                  track_h, rgb565(40, 52, 62));
        fill_rect(frame, width, height, stride_pixels, track_x, thumb_y, 2,
                  thumb_h, rgb565(220, 230, 224));
    }
}
