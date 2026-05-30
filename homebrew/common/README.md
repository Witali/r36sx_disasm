# Shared Homebrew Helpers

This directory contains small reusable helpers for native and libretro-style
R36SX experiments.

## `r36sx_screen_keyboard`

`r36sx_screen_keyboard.h` / `r36sx_screen_keyboard.c` implement the compact
joystick-controlled on-screen keyboard first built for Pico-286.

The module owns only keyboard UI state, button navigation, key layout, and
RGB565 drawing.  The host application supplies:

- a `struct r36sx_screen_keyboard` state object;
- the pressed and currently held R36SX button bitmasks for navigation and
  press animation;
- an `emit` callback that receives Windows-like keycodes and key-down/key-up
  transitions;
- a RGB565 framebuffer pointer, dimensions, and stride for drawing.

This keeps emulator-specific behavior outside the module.  Pico-286 converts
the emitted keycodes into PC keyboard scancodes and feeds its emulated keyboard
controller FIFO; another project can map the same callbacks to its own input
model.

Basic integration shape:

```c
static struct r36sx_screen_keyboard keyboard;

static void emit_key(void *user, uint16_t keycode, int is_down)
{
    (void)user;
    /* Map keycode to the target app/emulator input system here. */
}

r36sx_screen_keyboard_init(&keyboard);
r36sx_screen_keyboard_set_cursor_block(&keyboard, 1);
r36sx_screen_keyboard_set_visible(&keyboard, 1);
r36sx_screen_keyboard_handle_buttons(&keyboard, pressed, held, emit_key, NULL);
r36sx_screen_keyboard_draw(&keyboard, frame, width, height, stride_pixels);
```

The same layout can also be used as a key picker instead of a typing device.
`r36sx_screen_keyboard_handle_picker_buttons()` moves the highlighted key and
returns `R36SX_SCREEN_KEYBOARD_RESULT_ACCEPTED` with the selected keycode when
A or Start is pressed; it does not call the emit callback or send scancodes.
Pico-286 uses this mode in the key preset editor for selecting bindings and
renaming presets.

The keyboard panel is `R36SX_SCREEN_KEYBOARD_PANEL_H` pixels tall.  Use
`r36sx_screen_keyboard_content_height()` when the underlying app screen should
be vertically compressed or clipped while the keyboard is visible.

`r36sx_screen_keyboard_set_cursor_block()` optionally adds a compact cursor-key
cluster to the right side of the keyboard.  The main keys are narrowed to keep
the whole keyboard inside the same panel, and the cursor keys use a physical
inverted-T layout with Up above Left/Down/Right.

Current physical button bindings while the keyboard is visible:

- D-pad moves the highlighted key.
- Holding a D-pad direction repeats movement after a short delay.
- L or R switches between the normal layout and the symbol layout.
- A or Start types the highlighted key.
- B sends Backspace immediately.
- Y sends Enter immediately.
- X sends Escape immediately.
- Select closes the keyboard.

Holding A, Start, B, X, or Y repeats the emitted key after a short delay.
Shift is still available as the on-screen `SHF` key.
