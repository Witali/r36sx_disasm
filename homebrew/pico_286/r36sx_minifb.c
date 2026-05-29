/*
 * MiniFB-compatible backend for running xrip/pico-286 as a native R36SX app.
 *
 * The upstream Linux backend uses X11.  On the console we keep the same tiny
 * MiniFB API surface, but present RGB888 pixels through the stock driver.so
 * RGB565 framebuffer path and translate console buttons to PC key codes.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "MiniFB.h"
#include "../common/hardware.h"
#include "../common/r36sx_screen_keyboard.h"
#include "r36sx_disk_menu.h"
#include "r36sx_key_presets.h"

#define R36SX_PICO286_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_PICO286_FRAME_USEC 16666u
#define R36SX_PICO286_DISK_LED_HOLD_MS 350u
#define R36SX_PICO286_DISK_LED_BLINK_MS 120u
#define R36SX_PICO286_DISK_LED_RADIUS 8
#define R36SX_PICO286_FN_HOLD_EXIT_USEC 3000000ull

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);

struct r36sx_mfb_driver {
    void *handle;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    uint16_t *frame;
    int width;
    int height;
    int stride;
    int active;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
    uint8_t key_down[256];
    uint64_t last_present_us;
    uint32_t last_raw_keys;
    struct r36sx_screen_keyboard osk;
    struct r36sx_disk_menu disk_menu;
    struct r36sx_key_presets key_presets;
    uint8_t input_release_guard;
    uint8_t fn_down;
    uint8_t fn_chord_used;
    uint8_t fn_hold_exit_fired;
    uint64_t fn_down_since_us;
    uint8_t exit_requested;
};

static struct r36sx_mfb_driver g_mfb;
static uint32_t g_palette[256];
static volatile uint32_t g_disk_activity_until_ms;

extern void HandleInput(unsigned int keycode, int isKeyDown);

static uint64_t r36sx_mfb_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static uint32_t r36sx_mfb_now_ms32(void)
{
    return (uint32_t)(r36sx_mfb_now_us() / 1000ull);
}

void r36sx_pico286_disk_activity(void)
{
    g_disk_activity_until_ms =
        r36sx_mfb_now_ms32() + R36SX_PICO286_DISK_LED_HOLD_MS;
}

static uint16_t r36sx_mfb_rgb888_to_rgb565(uint32_t color)
{
    uint32_t r = (color >> 16) & 0xffu;
    uint32_t g = (color >> 8) & 0xffu;
    uint32_t b = color & 0xffu;
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static int r36sx_osk_panel_y(void)
{
    return r36sx_screen_keyboard_panel_y(g_mfb.height);
}

static uint16_t r36sx_mfb_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r & 0xf8u) << 8) |
                      ((uint16_t)(g & 0xfcu) << 3) |
                      ((uint16_t)b >> 3));
}

static void r36sx_mfb_fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0 || !g_mfb.frame) {
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
    if (x + w > g_mfb.width) {
        w = g_mfb.width - x;
    }
    if (y + h > g_mfb.height) {
        h = g_mfb.height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = g_mfb.frame + (size_t)(y + row) * (size_t)g_mfb.width +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void r36sx_mfb_release_all_keys(void)
{
    for (size_t code = 0; code < sizeof(g_mfb.key_down); code++) {
        if (g_mfb.key_down[code]) {
            HandleInput((unsigned int)code, 0);
            g_mfb.key_down[code] = 0;
        }
    }
}

static void r36sx_mfb_emit_osk_key(void *user, uint16_t keycode, int is_down)
{
    (void)user;
    HandleInput(keycode, is_down);
}

static void r36sx_osk_set_visible(int visible)
{
    int old_visible = r36sx_screen_keyboard_is_visible(&g_mfb.osk);
    visible = visible != 0;
    if (old_visible == visible) {
        return;
    }
    r36sx_mfb_release_all_keys();
    r36sx_screen_keyboard_set_visible(&g_mfb.osk, visible);
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: osk %s", visible ? "open" : "close");
}

static void r36sx_osk_handle_buttons(uint32_t pressed)
{
    uint32_t result = r36sx_screen_keyboard_handle_buttons(
        &g_mfb.osk, pressed, r36sx_mfb_emit_osk_key, NULL);

    if ((result & R36SX_SCREEN_KEYBOARD_RESULT_CLOSED) != 0) {
        g_mfb.input_release_guard = 1;
        r36sx_pico286_debug_log("minifb: osk close");
    }
}

static void r36sx_osk_draw(void)
{
    r36sx_screen_keyboard_draw(&g_mfb.osk, g_mfb.frame, g_mfb.width,
                               g_mfb.height, g_mfb.width);
}

static void r36sx_mfb_disk_menu_set_visible(int visible)
{
    int old_visible = r36sx_disk_menu_is_visible(&g_mfb.disk_menu);
    visible = visible != 0;
    if (old_visible == visible) {
        return;
    }
    r36sx_mfb_release_all_keys();
    r36sx_osk_set_visible(0);
    r36sx_key_presets_set_visible(&g_mfb.key_presets, 0);
    r36sx_disk_menu_set_visible(&g_mfb.disk_menu, visible);
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: disk menu %s",
                            visible ? "open" : "close");
}

static uint32_t r36sx_disk_menu_handle(uint32_t pressed)
{
    uint32_t result = r36sx_disk_menu_handle_buttons(&g_mfb.disk_menu,
                                                     pressed);
    if ((result & R36SX_DISK_MENU_RESULT_EXIT_APP) != 0) {
        r36sx_pico286_debug_log("minifb: disk menu exit requested");
        g_mfb.exit_requested = 1;
    }
    if ((result & (R36SX_DISK_MENU_RESULT_CLOSED |
                   R36SX_DISK_MENU_RESULT_EXIT_APP)) != 0) {
        g_mfb.input_release_guard = 1;
        r36sx_pico286_debug_log("minifb: disk menu close");
    }
    return result;
}

static void r36sx_disk_menu_draw_overlay(void)
{
    r36sx_disk_menu_draw(&g_mfb.disk_menu, g_mfb.frame, g_mfb.width,
                         g_mfb.height, g_mfb.width);
}

static void r36sx_presets_set_visible(int visible)
{
    int old_visible = r36sx_key_presets_is_visible(&g_mfb.key_presets);
    visible = visible != 0;
    if (old_visible == visible) {
        return;
    }
    r36sx_mfb_release_all_keys();
    r36sx_osk_set_visible(0);
    r36sx_disk_menu_set_visible(&g_mfb.disk_menu, 0);
    r36sx_key_presets_set_visible(&g_mfb.key_presets, visible);
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: key presets %s",
                            visible ? "open" : "close");
}

static void r36sx_presets_handle_buttons(uint32_t pressed)
{
    uint32_t result = r36sx_key_presets_handle_buttons(&g_mfb.key_presets,
                                                       pressed);
    if ((result & R36SX_KEY_PRESET_RESULT_CLOSED) != 0) {
        r36sx_mfb_release_all_keys();
        g_mfb.input_release_guard = 1;
        r36sx_pico286_debug_log("minifb: key presets close");
    }
}

static void r36sx_presets_draw(void)
{
    r36sx_key_presets_draw(&g_mfb.key_presets, g_mfb.frame, g_mfb.width,
                           g_mfb.height, g_mfb.width);
}

static void r36sx_mfb_draw_disk_led(uint32_t now_ms)
{
    int cx;
    int cy;
    const int radius = R36SX_PICO286_DISK_LED_RADIUS;
    const int outer_radius = radius + 2;
    const uint16_t red = 0xf800u;
    const uint16_t dark_red = 0x6000u;
    const uint16_t outline = 0x0000u;

    if ((int32_t)(g_disk_activity_until_ms - now_ms) <= 0) {
        return;
    }
    if (((now_ms / R36SX_PICO286_DISK_LED_BLINK_MS) & 1u) == 0u) {
        return;
    }

    cx = g_mfb.width - radius - 12;
    cy = (r36sx_screen_keyboard_is_visible(&g_mfb.osk) ?
          r36sx_osk_panel_y() : g_mfb.height) - radius - 12;
    for (int y = -outer_radius; y <= outer_radius; y++) {
        int py = cy + y;
        if (py < 0 || py >= g_mfb.height) {
            continue;
        }
        for (int x = -outer_radius; x <= outer_radius; x++) {
            int px = cx + x;
            int dist2 = x * x + y * y;
            if (px < 0 || px >= g_mfb.width) {
                continue;
            }
            if (dist2 <= radius * radius) {
                g_mfb.frame[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    dist2 <= (radius - 3) * (radius - 3) ? red : dark_red;
            } else if (dist2 <= outer_radius * outer_radius) {
                g_mfb.frame[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    outline;
            }
        }
    }
}

static int r36sx_mfb_load_driver(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    for (size_t i = 0; i < R36SX_PICO286_ARRAY_COUNT(paths); i++) {
        g_mfb.handle = dlopen(paths[i], RTLD_NOW);
        if (g_mfb.handle) {
            r36sx_pico286_debug_log("minifb: dlopen ok path=%s handle=%p",
                                    paths[i], g_mfb.handle);
            break;
        }
        r36sx_pico286_debug_log("minifb: dlopen failed path=%s err=%s",
                                paths[i], dlerror());
    }
    if (!g_mfb.handle) {
        r36sx_pico286_debug_log("minifb: all driver.so dlopen attempts failed");
        return -1;
    }

    g_mfb.setting =
        (video_driver_setting_fn)dlsym(g_mfb.handle, "video_driver_setting");
    g_mfb.init =
        (video_drivers_init_fn)dlsym(g_mfb.handle, "video_drivers_init");
    g_mfb.disp_frame =
        (video_driver_disp_frame_fn)dlsym(g_mfb.handle,
                                          "video_driver_disp_frame");
    g_mfb.deinit =
        (video_driver_deinit_fn)dlsym(g_mfb.handle, "video_driver_deinit");
    g_mfb.cube_ioctl = (cube_ioctl_fn)dlsym(g_mfb.handle, "cube_ioctl");

    if (!g_mfb.setting || !g_mfb.init || !g_mfb.disp_frame ||
        !g_mfb.deinit) {
        r36sx_pico286_debug_log(
            "minifb: driver symbols missing setting=%p init=%p disp=%p deinit=%p ioctl=%p",
            g_mfb.setting, g_mfb.init, g_mfb.disp_frame, g_mfb.deinit,
            g_mfb.cube_ioctl);
        return -1;
    }
    r36sx_pico286_debug_log(
        "minifb: driver symbols ok setting=%p init=%p disp=%p deinit=%p ioctl=%p",
        g_mfb.setting, g_mfb.init, g_mfb.disp_frame, g_mfb.deinit,
        g_mfb.cube_ioctl);
    return 0;
}

static uint32_t r36sx_mfb_read_raw_keys(void)
{
    uint32_t state = 0;

    if (!g_mfb.cube_ioctl) {
        return 0;
    }
    if (g_mfb.cube_key_addr == 0) {
        uint32_t addr = 0;
        int rc = g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_mfb.cube_key_addr = addr;
        g_mfb.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
        r36sx_pico286_debug_log("minifb: joy shm rc=%d addr=0x%08x", rc, addr);
    }
    if (!g_mfb.cube_key_mem) {
        return 0;
    }

    g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);
    (void)state;
    return g_mfb.cube_key_mem[0] | g_mfb.cube_key_mem[1];
}

static int r36sx_mfb_poll_input(void)
{
    static const uint32_t preset_masks[] = {
        R36SX_RKGAME_KEY_UP,
        R36SX_RKGAME_KEY_DOWN,
        R36SX_RKGAME_KEY_LEFT,
        R36SX_RKGAME_KEY_RIGHT,
        R36SX_RKGAME_KEY_A,
        R36SX_RKGAME_KEY_B,
        R36SX_RKGAME_KEY_Y,
        R36SX_RKGAME_KEY_X,
        R36SX_RKGAME_KEY_START,
        R36SX_RKGAME_KEY_L,
        R36SX_RKGAME_KEY_L2,
        R36SX_RKGAME_KEY_R,
        R36SX_RKGAME_KEY_R2
    };
    uint8_t new_down[256];
    uint32_t raw = r36sx_mfb_read_raw_keys();
    uint32_t pressed = raw & ~g_mfb.last_raw_keys;
    uint32_t released = ~raw & g_mfb.last_raw_keys;

    if (raw != g_mfb.last_raw_keys) {
        r36sx_pico286_debug_log("minifb: raw keys=0x%08x", raw);
    }

    if (g_mfb.input_release_guard) {
        if (raw == 0) {
            g_mfb.input_release_guard = 0;
        }
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if (r36sx_disk_menu_is_visible(&g_mfb.disk_menu)) {
        r36sx_disk_menu_handle(pressed);
        g_mfb.last_raw_keys = raw;
        return g_mfb.exit_requested ? -1 : 0;
    }

    if (r36sx_key_presets_is_visible(&g_mfb.key_presets)) {
        r36sx_presets_handle_buttons(pressed);
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if ((pressed & R36SX_RKGAME_KEY_FN) != 0) {
        g_mfb.fn_down = 1;
        g_mfb.fn_chord_used = 0;
        g_mfb.fn_hold_exit_fired = 0;
        g_mfb.fn_down_since_us = r36sx_mfb_now_us();
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if ((raw & R36SX_RKGAME_KEY_FN) != 0) {
        uint32_t fn_chord_pressed = pressed & ~R36SX_RKGAME_KEY_FN;
        if (fn_chord_pressed != 0) {
            g_mfb.fn_chord_used = 1;
        }
        if ((pressed & R36SX_RKGAME_KEY_X) != 0) {
            r36sx_pico286_debug_log("minifb: Fn+X exit requested");
            return -1;
        }
        if (g_mfb.fn_down && !g_mfb.fn_chord_used &&
            g_mfb.fn_down_since_us != 0 &&
            r36sx_mfb_now_us() - g_mfb.fn_down_since_us >=
                R36SX_PICO286_FN_HOLD_EXIT_USEC) {
            g_mfb.fn_hold_exit_fired = 1;
            r36sx_pico286_debug_log("minifb: Fn hold exit requested");
            return -1;
        }
        if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
            r36sx_mfb_disk_menu_set_visible(1);
            g_mfb.last_raw_keys = raw;
            return 0;
        }
        if ((pressed & R36SX_RKGAME_KEY_START) != 0) {
            r36sx_presets_set_visible(1);
            g_mfb.last_raw_keys = raw;
            return 0;
        }
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if ((released & R36SX_RKGAME_KEY_FN) != 0) {
        if (g_mfb.fn_down && !g_mfb.fn_chord_used &&
            !g_mfb.fn_hold_exit_fired) {
            r36sx_osk_set_visible(!r36sx_screen_keyboard_is_visible(&g_mfb.osk));
        }
        g_mfb.fn_down = 0;
        g_mfb.fn_chord_used = 0;
        g_mfb.fn_hold_exit_fired = 0;
        g_mfb.fn_down_since_us = 0;
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    if (r36sx_screen_keyboard_is_visible(&g_mfb.osk)) {
        r36sx_osk_handle_buttons(pressed);
        g_mfb.last_raw_keys = raw;
        return 0;
    }

    memset(new_down, 0, sizeof(new_down));
    for (size_t i = 0; i < R36SX_PICO286_ARRAY_COUNT(preset_masks); i++) {
        if ((raw & preset_masks[i]) != 0) {
            uint16_t keycode =
                r36sx_key_presets_key_for_mask(&g_mfb.key_presets,
                                                preset_masks[i]);
            if (keycode > 0 && keycode < sizeof(new_down)) {
                new_down[keycode] = 1;
            }
        }
    }

    for (size_t code = 0; code < sizeof(new_down); code++) {
        if (new_down[code] != g_mfb.key_down[code]) {
            HandleInput((unsigned int)code, new_down[code] != 0);
            g_mfb.key_down[code] = new_down[code];
        }
    }
    g_mfb.last_raw_keys = raw;
    return 0;
}

int mfb_open(const char *name, int width, int height, int scale)
{
    (void)scale;
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "?");
    }
    r36sx_pico286_debug_log("minifb: open name=%s size=%dx%d scale=%d cwd=%s",
                            name ? name : "(null)", width, height, scale, cwd);

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    memset(&g_mfb, 0, sizeof(g_mfb));
    r36sx_screen_keyboard_init(&g_mfb.osk);
    r36sx_screen_keyboard_set_cursor_block(&g_mfb.osk, 0);
    r36sx_disk_menu_init(&g_mfb.disk_menu);
    r36sx_key_presets_load(&g_mfb.key_presets);
    g_mfb.width = width;
    g_mfb.height = height;
    g_mfb.stride = width * R36SX_RGB565_BYTES_PER_PIXEL;
    g_mfb.frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                     sizeof(g_mfb.frame[0]));
    if (!g_mfb.frame) {
        r36sx_pico286_debug_log("minifb: framebuffer allocation failed");
        return 0;
    }
    if (r36sx_mfb_load_driver() != 0) {
        mfb_close();
        return 0;
    }

    {
        int cfg[5] = {
            R36SX_DRIVER_SETTING_0,
            R36SX_DRIVER_SETTING_1,
            R36SX_DRIVER_SETTING_2,
            R36SX_DRIVER_SETTING_WIDTH,
            R36SX_DRIVER_SETTING_HEIGHT
        };
        r36sx_pico286_debug_log("minifb: video_driver_setting cfg={%d,%d,%d,%d,%d}",
                                cfg[0], cfg[1], cfg[2], cfg[3], cfg[4]);
        g_mfb.setting(cfg);
    }
    {
        int rc = g_mfb.init();
        r36sx_pico286_debug_log("minifb: video_drivers_init rc=%d", rc);
        if (rc < 0) {
            r36sx_pico286_debug_log("minifb: video_drivers_init failed");
            mfb_close();
            return 0;
        }
    }

    if (g_mfb.cube_ioctl) {
        uint32_t joy = 0;
        int rc = g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &joy, 0, 0);
        g_mfb.cube_key_addr = joy;
        g_mfb.cube_key_mem = (volatile uint32_t *)(uintptr_t)joy;
        r36sx_pico286_debug_log("minifb: initial joy shm rc=%d addr=0x%08x", rc, joy);
    } else {
        r36sx_pico286_debug_log("minifb: cube_ioctl unavailable, input disabled");
    }

    if (!g_mfb.disp_frame) {
        r36sx_pico286_debug_log("minifb: no display frame function after init");
        mfb_close();
        return 0;
    }

    g_mfb.active = 1;
    g_mfb.last_present_us = 0;
    r36sx_pico286_debug_log("minifb: opened");
    return 1;
}

int mfb_update(void *buffer, int fps_limit)
{
    uint64_t now;
    uint32_t *src = (uint32_t *)buffer;
    int content_h;
    (void)fps_limit;

    if (!g_mfb.active || !g_mfb.disp_frame || !src) {
        r36sx_pico286_debug_log("minifb: update rejected active=%d disp=%p src=%p",
                                g_mfb.active, g_mfb.disp_frame, src);
        return -1;
    }
    if (r36sx_mfb_poll_input() != 0) {
        return -1;
    }

    now = r36sx_mfb_now_us();
    if (g_mfb.last_present_us != 0 &&
        now - g_mfb.last_present_us < R36SX_PICO286_FRAME_USEC) {
        usleep(1000);
        return 0;
    }

    content_h = r36sx_screen_keyboard_content_height(&g_mfb.osk, g_mfb.height);
    for (int y = 0; y < content_h; y++) {
        uint16_t *dst_row = g_mfb.frame + (size_t)y * (size_t)g_mfb.width;

        if (r36sx_screen_keyboard_is_visible(&g_mfb.osk)) {
            int start = y * g_mfb.height;
            int end = (y + 1) * g_mfb.height;
            int sy0 = start / content_h;
            int sy1 = (end - 1) / content_h;
            int weight0;
            int weight1;
            const uint32_t *src_row0;
            const uint32_t *src_row1;

            if (sy1 >= g_mfb.height) {
                sy1 = g_mfb.height - 1;
            }
            weight0 = (sy0 + 1) * content_h - start;
            if (weight0 > g_mfb.height) {
                weight0 = g_mfb.height;
            }
            if (sy0 == sy1) {
                weight1 = 0;
                weight0 = g_mfb.height;
            } else {
                weight1 = g_mfb.height - weight0;
            }

            src_row0 = src + (size_t)sy0 * (size_t)g_mfb.width;
            src_row1 = src + (size_t)sy1 * (size_t)g_mfb.width;
            if (g_mfb.height == 480 && content_h == 384) {
                uint32_t w0 = (uint32_t)(weight0 / 96);
                uint32_t w1 = (uint32_t)(weight1 / 96);
                for (int x = 0; x < g_mfb.width; x++) {
                    uint32_t c0 = src_row0[x];
                    uint32_t c1 = src_row1[x];
                    uint32_t r = (((c0 >> 16) & 0xffu) * w0 +
                                  ((c1 >> 16) & 0xffu) * w1 + 2u) / 5u;
                    uint32_t g = (((c0 >> 8) & 0xffu) * w0 +
                                  ((c1 >> 8) & 0xffu) * w1 + 2u) / 5u;
                    uint32_t b = ((c0 & 0xffu) * w0 +
                                  (c1 & 0xffu) * w1 + 2u) / 5u;
                    dst_row[x] = (uint16_t)(((r & 0xf8u) << 8) |
                                            ((g & 0xfcu) << 3) | (b >> 3));
                }
            } else {
                for (int x = 0; x < g_mfb.width; x++) {
                    uint32_t c0 = src_row0[x];
                    uint32_t c1 = src_row1[x];
                    uint32_t r = (((c0 >> 16) & 0xffu) *
                                  (uint32_t)weight0 +
                                  ((c1 >> 16) & 0xffu) *
                                  (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    uint32_t g = (((c0 >> 8) & 0xffu) *
                                  (uint32_t)weight0 +
                                  ((c1 >> 8) & 0xffu) *
                                  (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    uint32_t b = ((c0 & 0xffu) * (uint32_t)weight0 +
                                  (c1 & 0xffu) * (uint32_t)weight1) /
                                 (uint32_t)g_mfb.height;
                    dst_row[x] = (uint16_t)(((r & 0xf8u) << 8) |
                                            ((g & 0xfcu) << 3) | (b >> 3));
                }
            }
        } else {
            uint32_t *src_row = src + (size_t)y * (size_t)g_mfb.width;
            for (int x = 0; x < g_mfb.width; x++) {
                dst_row[x] = r36sx_mfb_rgb888_to_rgb565(src_row[x]);
            }
        }
    }
    if (content_h < g_mfb.height) {
        r36sx_mfb_fill_rect(0, content_h, g_mfb.width,
                            g_mfb.height - content_h,
                            r36sx_mfb_rgb565(0, 0, 0));
    }
    r36sx_mfb_draw_disk_led((uint32_t)(now / 1000ull));
    if (r36sx_disk_menu_is_visible(&g_mfb.disk_menu)) {
        r36sx_disk_menu_draw_overlay();
    } else if (r36sx_key_presets_is_visible(&g_mfb.key_presets)) {
        r36sx_presets_draw();
    } else {
        r36sx_osk_draw();
    }
    g_mfb.disp_frame(g_mfb.frame, g_mfb.width, g_mfb.height, g_mfb.stride);
    g_mfb.last_present_us = now;
    return 0;
}

void mfb_set_pallete_array(const uint32_t *new_palette, uint8_t start,
                           uint8_t count)
{
    if (!new_palette) {
        return;
    }
    for (uint8_t i = 0; i < count; i++) {
        g_palette[(uint8_t)(start + i)] = new_palette[i];
    }
}

void mfb_set_pallete(const uint8_t color_index, const uint32_t color)
{
    g_palette[color_index] = color;
}

void mfb_close(void)
{
    r36sx_pico286_debug_log("minifb: close active=%d handle=%p frame=%p",
                            g_mfb.active, g_mfb.handle, g_mfb.frame);
    if (g_mfb.active && g_mfb.deinit) {
        g_mfb.deinit();
    }
    if (g_mfb.frame) {
        free(g_mfb.frame);
    }
    if (g_mfb.handle) {
        dlclose(g_mfb.handle);
    }
    memset(&g_mfb, 0, sizeof(g_mfb));
}

char *mfb_keystatus(void)
{
    return (char *)g_mfb.key_down;
}
