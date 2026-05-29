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
    R36SX_SCREEN_KEY_ESCAPE = 27,
    R36SX_SCREEN_KEY_SPACE = 32,
    R36SX_SCREEN_KEY_LEFT = 37,
    R36SX_SCREEN_KEY_UP = 38,
    R36SX_SCREEN_KEY_RIGHT = 39,
    R36SX_SCREEN_KEY_DOWN = 40,
    R36SX_SCREEN_KEY_DELETE = 46,
    R36SX_SCREEN_KEY_F1 = 112,
    R36SX_SCREEN_KEY_OEM_1 = 186,
    R36SX_SCREEN_KEY_OEM_PLUS = 187,
    R36SX_SCREEN_KEY_OEM_COMMA = 188,
    R36SX_SCREEN_KEY_OEM_MINUS = 189,
    R36SX_SCREEN_KEY_OEM_PERIOD = 190,
    R36SX_SCREEN_KEY_OEM_2 = 191,
    R36SX_SCREEN_KEY_OEM_5 = 220,
    R36SX_SCREEN_KEY_OEM_7 = 222
};

struct r36sx_screen_keyboard {
    uint8_t visible;
    uint8_t row;
    uint8_t col;
    uint8_t shift;
    uint8_t ctrl;
    uint8_t alt;
};

typedef void (*r36sx_screen_keyboard_emit_fn)(
    void *user, uint16_t keycode, int is_down);

void r36sx_screen_keyboard_init(struct r36sx_screen_keyboard *keyboard);
int r36sx_screen_keyboard_is_visible(
    const struct r36sx_screen_keyboard *keyboard);
void r36sx_screen_keyboard_set_visible(
    struct r36sx_screen_keyboard *keyboard, int visible);
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
    r36sx_screen_keyboard_emit_fn emit,
    void *emit_user);
uint32_t r36sx_screen_keyboard_handle_picker_buttons(
    struct r36sx_screen_keyboard *keyboard,
    uint32_t pressed,
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
