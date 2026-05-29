#ifndef R36SX_KEY_PRESETS_H
#define R36SX_KEY_PRESETS_H

#include <stdint.h>

#include "../common/r36sx_screen_keyboard.h"

#define R36SX_KEY_PRESETS_MAX 8
#define R36SX_KEY_PRESET_NAME_LEN 32
#define R36SX_KEY_PRESET_RESULT_CLOSED 0x01u

enum r36sx_key_preset_button {
    R36SX_KEY_PRESET_BUTTON_UP = 0,
    R36SX_KEY_PRESET_BUTTON_DOWN,
    R36SX_KEY_PRESET_BUTTON_LEFT,
    R36SX_KEY_PRESET_BUTTON_RIGHT,
    R36SX_KEY_PRESET_BUTTON_A,
    R36SX_KEY_PRESET_BUTTON_B,
    R36SX_KEY_PRESET_BUTTON_Y,
    R36SX_KEY_PRESET_BUTTON_X,
    R36SX_KEY_PRESET_BUTTON_START,
    R36SX_KEY_PRESET_BUTTON_SELECT,
    R36SX_KEY_PRESET_BUTTON_L,
    R36SX_KEY_PRESET_BUTTON_L2,
    R36SX_KEY_PRESET_BUTTON_R,
    R36SX_KEY_PRESET_BUTTON_R2,
    R36SX_KEY_PRESET_BUTTON_COUNT
};

struct r36sx_key_preset {
    char name[R36SX_KEY_PRESET_NAME_LEN];
    uint16_t keycodes[R36SX_KEY_PRESET_BUTTON_COUNT];
};

struct r36sx_key_presets {
    struct r36sx_key_preset presets[R36SX_KEY_PRESETS_MAX];
    struct r36sx_key_preset draft_presets[R36SX_KEY_PRESETS_MAX];
    struct r36sx_screen_keyboard picker_keyboard;
    uint8_t count;
    uint8_t active;
    uint8_t draft_count;
    uint8_t draft_active;
    uint8_t visible;
    uint8_t selected_row;
    uint8_t edit_mode;
    uint8_t picker_button;
    char config_path[192];
};

void r36sx_key_presets_init(struct r36sx_key_presets *state);
void r36sx_key_presets_load(struct r36sx_key_presets *state);
void r36sx_key_presets_save(struct r36sx_key_presets *state);
int r36sx_key_presets_is_visible(const struct r36sx_key_presets *state);
void r36sx_key_presets_set_visible(struct r36sx_key_presets *state,
                                   int visible);
uint16_t r36sx_key_presets_key_for_mask(
    const struct r36sx_key_presets *state, uint32_t raw_mask);
uint32_t r36sx_key_presets_handle_buttons(struct r36sx_key_presets *state,
                                          uint32_t pressed,
                                          uint32_t held);
void r36sx_key_presets_draw(const struct r36sx_key_presets *state,
                            uint16_t *frame,
                            int width,
                            int height,
                            int stride_pixels);

#endif
