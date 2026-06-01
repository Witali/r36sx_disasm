#ifndef R36SX_SCREEN_KEYBOARD_H
#define R36SX_SCREEN_KEYBOARD_H

#include <stdint.h>

#include "hardware.h"

#ifdef __cplusplus
extern "C" {
#endif

#define R36SX_SCREEN_KEYBOARD_PANEL_H 96
#define R36SX_SCREEN_KEYBOARD_RESULT_CLOSED 0x01u
#define R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED 0x02u

enum {
    R36SX_SCREEN_KEY_BACK = 8,
    R36SX_SCREEN_KEY_TAB = 9,
    R36SX_SCREEN_KEY_RETURN = 13,
    R36SX_SCREEN_KEY_SHIFT = 16,
    R36SX_SCREEN_KEY_CONTROL = 17,
    R36SX_SCREEN_KEY_MENU = 18,
    R36SX_SCREEN_KEY_PAUSE = 19,
    R36SX_SCREEN_KEY_CAPITAL = 20,
    R36SX_SCREEN_KEY_ESCAPE = 27,
    R36SX_SCREEN_KEY_SPACE = 32,
    R36SX_SCREEN_KEY_PRIOR = 33,
    R36SX_SCREEN_KEY_NEXT = 34,
    R36SX_SCREEN_KEY_END = 35,
    R36SX_SCREEN_KEY_HOME = 36,
    R36SX_SCREEN_KEY_LEFT = 37,
    R36SX_SCREEN_KEY_UP = 38,
    R36SX_SCREEN_KEY_RIGHT = 39,
    R36SX_SCREEN_KEY_DOWN = 40,
    R36SX_SCREEN_KEY_PRINT = 44,
    R36SX_SCREEN_KEY_INSERT = 45,
    R36SX_SCREEN_KEY_DELETE = 46,
    R36SX_SCREEN_KEY_LWIN = 91,
    R36SX_SCREEN_KEY_APPS = 93,
    R36SX_SCREEN_KEY_F1 = 112,
    R36SX_SCREEN_KEY_SCROLL = 145,
    R36SX_SCREEN_KEY_OEM_1 = 186,
    R36SX_SCREEN_KEY_OEM_PLUS = 187,
    R36SX_SCREEN_KEY_OEM_COMMA = 188,
    R36SX_SCREEN_KEY_OEM_MINUS = 189,
    R36SX_SCREEN_KEY_OEM_PERIOD = 190,
    R36SX_SCREEN_KEY_OEM_2 = 191,
    R36SX_SCREEN_KEY_OEM_3 = 192,
    R36SX_SCREEN_KEY_OEM_4 = 219,
    R36SX_SCREEN_KEY_OEM_5 = 220,
    R36SX_SCREEN_KEY_OEM_6 = 221,
    R36SX_SCREEN_KEY_OEM_7 = 222
};

struct r36sx_screen_keyboard {
    uint8_t visible;
    uint8_t zone;
    uint8_t row;
    uint8_t col;
    uint8_t shift;
    uint8_t ctrl;
    uint8_t alt;
    uint8_t physical_shift;
    uint8_t physical_ctrl;
    uint8_t caps_lock;
    uint8_t symbol_mode;
    uint8_t cursor_block;
    uint8_t scroll_y;
    uint8_t press_zone;
    uint8_t press_row;
    uint8_t press_col;
    uint32_t press_buttons;
    uint32_t nav_repeat_button;
    uint64_t nav_repeat_next_us;
    uint32_t key_repeat_button;
    uint16_t key_repeat_keycode;
    uint8_t key_repeat_force_shift;
    uint64_t key_repeat_next_us;
};

typedef void (*r36sx_screen_keyboard_emit_fn)(
    void *user, uint16_t keycode, int is_down);

void r36sx_screen_keyboard_init(struct r36sx_screen_keyboard *keyboard);
int r36sx_screen_keyboard_is_visible(
    const struct r36sx_screen_keyboard *keyboard);
void r36sx_screen_keyboard_set_visible(
    struct r36sx_screen_keyboard *keyboard, int visible);
void r36sx_screen_keyboard_set_cursor_block(
    struct r36sx_screen_keyboard *keyboard, int enabled);
int r36sx_screen_keyboard_cursor_block_enabled(
    const struct r36sx_screen_keyboard *keyboard);
int r36sx_screen_keyboard_panel_y(int framebuffer_height);
int r36sx_screen_keyboard_content_height(
    const struct r36sx_screen_keyboard *keyboard, int framebuffer_height);
uint16_t r36sx_screen_keyboard_current_keycode(
    struct r36sx_screen_keyboard *keyboard);
const char *r36sx_screen_keyboard_current_label(
    struct r36sx_screen_keyboard *keyboard);
uint32_t r36sx_screen_keyboard_handle_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
    uint32_t held,
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user);
uint32_t r36sx_screen_keyboard_handle_picker_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
    uint32_t held,
    uint16_t *keycode);
void r36sx_screen_keyboard_draw(
    const struct r36sx_screen_keyboard *keyboard,
    uint16_t *frame,
    int width,
    int height,
    int stride_pixels);

#ifdef __cplusplus
}
#endif

#endif
