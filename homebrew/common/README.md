# Shared Homebrew Helpers

This directory contains small reusable helpers for native and libretro-style
R36SX experiments.

## `r36sx_screen_keyboard`

`r36sx_screen_keyboard.h` / `r36sx_screen_keyboard.c` implement the compact
joystick-controlled on-screen keyboard first built for Pico-286.

The module owns only keyboard UI state, button navigation, key layout, and
RGB565 drawing.  The host application supplies:

- a `struct r36sx_screen_keyboard` state object;
- the pressed R36SX button bitmask for navigation;
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
r36sx_screen_keyboard_set_visible(&keyboard, 1);
r36sx_screen_keyboard_handle_buttons(&keyboard, pressed, emit_key, NULL);
r36sx_screen_keyboard_draw(&keyboard, frame, width, height, stride_pixels);
```

The keyboard panel is `R36SX_SCREEN_KEYBOARD_PANEL_H` pixels tall.  Use
`r36sx_screen_keyboard_content_height()` when the underlying app screen should
be vertically compressed or clipped while the keyboard is visible.

Current physical button bindings while the keyboard is visible:

- D-pad moves the highlighted key.
- A or Start types the highlighted key.
- B sends Backspace immediately.
- Y sends Enter immediately.
- X sends Escape immediately.
- Select closes the keyboard.

Shift is still available as the on-screen `SHF` key.
