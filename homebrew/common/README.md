# Shared Homebrew Helpers

This directory contains small reusable helpers for native and libretro-style
R36SX experiments.

## `inih`

`inih/ini.c` and `inih/ini.h` vendor `benhoyt/inih` release `r62`, a small
BSD-3-Clause callback-based C INI parser.  Pico-286 and Tiny MC use it for
runtime `.conf` file loading instead of local hand-written line parsers.

## `r36sx_screenshot`

`r36sx_screenshot.h` / `r36sx_screenshot.c` save RGB565 framebuffers to disk
with the shared filename and directory fallback logic used by native projects.
`r36sx_screenshot_module.c` builds the optional `screenshot.so` backend for
the actual file encoding work.

The caller supplies:

- primary and fallback output directories;
- filename prefix;
- RTC or host timestamp;
- sequence number;
- optional build SHA-256 string for the eight-character build hash suffix;
- output format, currently BMP or PNG.

The helper loads the encoder module lazily from:

```text
/mnt/sdcard/MIPS_NATIVE/common/screenshot.so
```

The `R36SX_SCREENSHOT_SO` environment variable can override that path, and the
helper also tries `./screenshot.so` for local tests.  The module receives the
requested format as a parameter and currently writes both BMP and PNG.  Keeping
the encoder in a separate shared object removes zlib from the main Pico-286
executable.  If the module is absent, BMP falls back to the small built-in
writer; PNG screenshot saves fail cleanly.

Build the module with:

```sh
bash homebrew/common/build_screenshot_so_wsl.sh --strip
```

For static linking, build the same encoder as an archive:

```sh
bash homebrew/common/build_screenshot_a_wsl.sh
```

`screenshot.a` exports the same `r36sx_screenshot_write_rgb565()` function as
`screenshot.so`.  It contains our encoder object only; applications that use
PNG through the static archive must also link target zlib.  The Pico-286 and
Shell WSL build scripts rebuild both `screenshot.so` and `screenshot.a`
automatically.

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

The keyboard opens in the fit-height panel by default.  That panel is just tall
enough to show every key row at once, with a one-pixel key-area gap at the top
and bottom.  Select collapses it to the compact
`R36SX_SCREEN_KEYBOARD_PANEL_H` panel, currently 80 pixels tall, and toggles it
back to fit-height.  Compact mode hides the header hints to preserve key space.
Use `r36sx_screen_keyboard_content_height()` when the underlying app screen
should be vertically compressed or clipped while the keyboard is visible.

`r36sx_screen_keyboard_set_cursor_block()` optionally adds a compact cursor-key
cluster to the right side of the keyboard.  The main keys are narrowed to keep
the whole keyboard inside the same panel, and the cursor keys use a physical
inverted-T layout with Up above Left/Down/Right.  Vertical navigation inside
this side cluster follows the same physical column and skips empty rows, so
moving up from Left Arrow selects `DEL` instead of the Up Arrow key.

The normal keyboard rows are left-aligned like a PC keyboard: `ESC`, `TAB`,
`CAPS`, left `SHIFT`, and left `CTRL` share the same edge.  The bottom row
includes `WIN`, an unlabeled stretched spacebar, and a context `MENU` key; host
applications can map these emitted Windows-like keycodes as needed.  The
on-screen left/right modifier keys emit distinct picker keycodes: `LSHIFT`,
`RSHIFT`, `LCTRL`, `RCTRL`, `LALT`, and `RALT`.  The last key in each row
absorbs the row-width remainder so `F12`, Backspace, `\`, Enter, right
`SHIFT`, and right `CTRL` share a clean visual right edge.  On the
bottom row, the spacebar absorbs that remainder instead, keeping the visible
gaps between `CTRL`, `WIN`, `ALT`, the spacebar, `MENU`, and right `CTRL`
consistent with the rows above.  The bottom-row remainder is split so `WIN`
ends under `A`, the spacebar ends under comma, and `MENU` starts under the
`=`/`F10` guide column.  The `CAPS` key includes a small green lock LED that
follows the keyboard module's Caps Lock state.

Current physical button bindings while the keyboard is visible:

- D-pad moves the highlighted key.
- Holding a D-pad direction repeats movement after a short delay.
- L holds Shift, R holds Ctrl, and R2 holds Alt while the physical trigger is
  held.
- A or Start types the highlighted key.
- B sends Backspace immediately.
- Y sends Enter immediately.
- X sends Escape immediately.
- Select toggles compact and fit-height keyboard panels.

Holding A, Start, B, X, or Y repeats the emitted key after a short delay;
Caps Lock is excluded from that repeat so one press toggles it once.  Shift is
still available as the on-screen `SHIFT` key.  Host applications should keep a
separate shortcut, such as Fn tap in Pico-286, for showing and hiding the
keyboard.  The fit-height keyboard header repeats the compact binding hints;
compact mode hides the header.
