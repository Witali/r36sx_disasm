/*
 * MiniFB-compatible backend for running xrip/pico-286 as a native R36SX app.
 *
 * The upstream Linux backend uses X11.  On the console we keep the same tiny
 * MiniFB API surface, but present RGB565 pixels directly through the stock
 * driver.so framebuffer path and translate console buttons to PC key codes.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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
#define R36SX_PICO286_DISK_LED_OUTER_RADIUS \
    (R36SX_PICO286_DISK_LED_RADIUS + 2)
#define R36SX_PICO286_DISK_LED_DIAMETER \
    (R36SX_PICO286_DISK_LED_OUTER_RADIUS * 2 + 1)
#define R36SX_PICO286_FN_HOLD_EXIT_USEC 3000000ull
#define R36SX_PICO286_SCREENSHOT_DIR \
    "/mnt/sdcard/MIPS_NATIVE/pico_286/screenshots"
#define R36SX_PICO286_SCREENSHOT_LOCAL_DIR "screenshots"

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
    uint16_t *base_frame;
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
    uint8_t base_frame_valid;
    uint8_t force_present;
    uint8_t disk_led_was_active;
    uint8_t screenshot_requested;
    int base_content_h;
    int base_source_h;
    uint32_t base_generation;
    uint32_t presented_generation;
    uint32_t screenshot_counter;
};

struct r36sx_mfb_saved_rect {
    int x;
    int y;
    int w;
    int h;
    uint16_t pixels[R36SX_PICO286_DISK_LED_DIAMETER *
                    R36SX_PICO286_DISK_LED_DIAMETER];
};

static struct r36sx_mfb_driver g_mfb;
static uint16_t g_palette[256];
static volatile uint32_t g_frame_generation;
static volatile uint32_t g_disk_activity_until_ms;

extern void HandleInput(unsigned int keycode, int isKeyDown);
extern int r36sx_pico286_video_active_height(void);
extern void r36sx_pico286_request_soft_reset(void);

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

void r36sx_mfb_mark_frame_ready(void)
{
    __sync_add_and_fetch(&g_frame_generation, 1u);
}

static uint32_t r36sx_mfb_frame_generation(void)
{
    return __sync_add_and_fetch(&g_frame_generation, 0u);
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

static void r36sx_mfb_put_le16(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)(value >> 8);
}

static void r36sx_mfb_put_le32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)((value >> 8) & 0xffu);
    dst[2] = (uint8_t)((value >> 16) & 0xffu);
    dst[3] = (uint8_t)((value >> 24) & 0xffu);
}

static uint8_t r36sx_mfb_rgb565_to_r8(uint16_t color)
{
    uint8_t r = (uint8_t)((color >> 11) & 0x1fu);
    return (uint8_t)((r << 3) | (r >> 2));
}

static uint8_t r36sx_mfb_rgb565_to_g8(uint16_t color)
{
    uint8_t g = (uint8_t)((color >> 5) & 0x3fu);
    return (uint8_t)((g << 2) | (g >> 4));
}

static uint8_t r36sx_mfb_rgb565_to_b8(uint16_t color)
{
    uint8_t b = (uint8_t)(color & 0x1fu);
    return (uint8_t)((b << 3) | (b >> 2));
}

static int r36sx_mfb_write_bmp24(const char *path, const uint16_t *pixels,
                                 int width, int height)
{
    FILE *fp;
    uint8_t header[54];
    uint8_t *row;
    uint32_t row_bytes;
    uint32_t pixel_bytes;
    uint32_t file_bytes;

    if (!path || !pixels || width <= 0 || height <= 0) {
        return -1;
    }

    row_bytes = (uint32_t)width * 3u;
    pixel_bytes = row_bytes * (uint32_t)height;
    file_bytes = 54u + pixel_bytes;
    row = (uint8_t *)malloc(row_bytes);
    if (!row) {
        return -1;
    }

    memset(header, 0, sizeof(header));
    header[0] = 'B';
    header[1] = 'M';
    r36sx_mfb_put_le32(&header[2], file_bytes);
    r36sx_mfb_put_le32(&header[10], 54u);
    r36sx_mfb_put_le32(&header[14], 40u);
    r36sx_mfb_put_le32(&header[18], (uint32_t)width);
    r36sx_mfb_put_le32(&header[22], (uint32_t)height);
    r36sx_mfb_put_le16(&header[26], 1u);
    r36sx_mfb_put_le16(&header[28], 24u);
    r36sx_mfb_put_le32(&header[34], pixel_bytes);

    fp = fopen(path, "wb");
    if (!fp) {
        free(row);
        return -1;
    }

    if (fwrite(header, 1, sizeof(header), fp) != sizeof(header)) {
        fclose(fp);
        free(row);
        return -1;
    }

    for (int y = height - 1; y >= 0; y--) {
        const uint16_t *src = pixels + (size_t)y * (size_t)width;
        for (int x = 0; x < width; x++) {
            uint16_t c = src[x];
            row[(size_t)x * 3u + 0u] = r36sx_mfb_rgb565_to_b8(c);
            row[(size_t)x * 3u + 1u] = r36sx_mfb_rgb565_to_g8(c);
            row[(size_t)x * 3u + 2u] = r36sx_mfb_rgb565_to_r8(c);
        }
        if (fwrite(row, 1, row_bytes, fp) != row_bytes) {
            fclose(fp);
            free(row);
            return -1;
        }
    }

    if (fclose(fp) != 0) {
        free(row);
        return -1;
    }

    free(row);
    return 0;
}

static int r36sx_mfb_save_screenshot_to_dir(const char *dir,
                                            const uint16_t *pixels,
                                            char *saved_path,
                                            size_t saved_path_size)
{
    time_t now;
    struct tm tm_now;
    char stamp[32];
    char path[512];
    uint32_t seq;

    if (!dir || !pixels) {
        return -1;
    }

    mkdir(dir, 0755);
    now = time(NULL);
    if (localtime_r(&now, &tm_now) == NULL) {
        memset(&tm_now, 0, sizeof(tm_now));
    }
    strftime(stamp, sizeof(stamp), "%Y%m%d_%H%M%S", &tm_now);
    seq = g_mfb.screenshot_counter++;

    snprintf(path, sizeof(path), "%s/pico_286_%s_%03u.bmp",
             dir, stamp, (unsigned)(seq % 1000u));
    if (r36sx_mfb_write_bmp24(path, pixels, g_mfb.width, g_mfb.height) != 0) {
        return -1;
    }

    if (saved_path && saved_path_size > 0) {
        snprintf(saved_path, saved_path_size, "%s", path);
    }
    return 0;
}

static void r36sx_mfb_save_screenshot(const uint16_t *pixels)
{
    char path[512];

    if (r36sx_mfb_save_screenshot_to_dir(R36SX_PICO286_SCREENSHOT_DIR,
                                         pixels, path, sizeof(path)) == 0 ||
        r36sx_mfb_save_screenshot_to_dir(R36SX_PICO286_SCREENSHOT_LOCAL_DIR,
                                         pixels, path, sizeof(path)) == 0) {
        r36sx_pico286_debug_log("minifb: screenshot saved %s", path);
    } else {
        r36sx_pico286_debug_log("minifb: screenshot save failed");
    }
}

static uint16_t r36sx_mfb_blend_rgb565(uint16_t c0, uint16_t c1,
                                       uint32_t w0, uint32_t w1,
                                       uint32_t denominator)
{
    uint32_t r = (((c0 >> 11) & 0x1fu) * w0 +
                  ((c1 >> 11) & 0x1fu) * w1 + denominator / 2u) /
                 denominator;
    uint32_t g = (((c0 >> 5) & 0x3fu) * w0 +
                  ((c1 >> 5) & 0x3fu) * w1 + denominator / 2u) /
                 denominator;
    uint32_t b = ((c0 & 0x1fu) * w0 +
                  (c1 & 0x1fu) * w1 + denominator / 2u) /
                 denominator;

    return (uint16_t)((r << 11) | (g << 5) | b);
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
    g_mfb.base_frame_valid = 0;
    g_mfb.force_present = 1;
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: osk %s", visible ? "open" : "close");
}

static void r36sx_osk_handle_buttons(uint32_t pressed, uint32_t held)
{
    uint32_t result = r36sx_screen_keyboard_handle_buttons(
        &g_mfb.osk, pressed, held, r36sx_mfb_emit_osk_key, NULL);

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
    g_mfb.force_present = 1;
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
    g_mfb.force_present = 1;
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: key presets %s",
                            visible ? "open" : "close");
}

static void r36sx_presets_handle_buttons(uint32_t pressed, uint32_t held)
{
    uint32_t result = r36sx_key_presets_handle_buttons(&g_mfb.key_presets,
                                                        pressed, held);
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

static void r36sx_mfb_request_soft_reset(void)
{
    r36sx_mfb_release_all_keys();
    r36sx_screen_keyboard_set_visible(&g_mfb.osk, 0);
    r36sx_disk_menu_set_visible(&g_mfb.disk_menu, 0);
    r36sx_key_presets_set_visible(&g_mfb.key_presets, 0);
    r36sx_pico286_request_soft_reset();
    g_mfb.base_frame_valid = 0;
    g_mfb.force_present = 1;
    g_mfb.input_release_guard = 1;
    r36sx_pico286_debug_log("minifb: Fn+B soft reset requested");
}

static void r36sx_mfb_request_screenshot(void)
{
    g_mfb.screenshot_requested = 1;
    g_mfb.force_present = 1;
    r36sx_pico286_debug_log("minifb: Fn+Up screenshot requested");
}

static int r36sx_mfb_disk_led_active(uint32_t now_ms)
{
    return (int32_t)(g_disk_activity_until_ms - now_ms) > 0;
}

static int r36sx_mfb_disk_led_visible(uint32_t now_ms)
{
    return r36sx_mfb_disk_led_active(now_ms) &&
           ((now_ms / R36SX_PICO286_DISK_LED_BLINK_MS) & 1u) != 0u;
}

static void r36sx_mfb_disk_led_center(int *cx, int *cy)
{
    const int radius = R36SX_PICO286_DISK_LED_RADIUS;

    *cx = g_mfb.width - radius - 12;
    *cy = (r36sx_screen_keyboard_is_visible(&g_mfb.osk) ?
           r36sx_osk_panel_y() : g_mfb.height) - radius - 12;
}

static void r36sx_mfb_draw_disk_led_on(uint16_t *target, uint32_t now_ms)
{
    int cx;
    int cy;
    const int radius = R36SX_PICO286_DISK_LED_RADIUS;
    const int outer_radius = R36SX_PICO286_DISK_LED_OUTER_RADIUS;
    const uint16_t red = 0xf800u;
    const uint16_t dark_red = 0x6000u;
    const uint16_t outline = 0x0000u;

    if (!target || !r36sx_mfb_disk_led_visible(now_ms)) {
        return;
    }

    r36sx_mfb_disk_led_center(&cx, &cy);
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
                target[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    dist2 <= (radius - 3) * (radius - 3) ? red : dark_red;
            } else if (dist2 <= outer_radius * outer_radius) {
                target[(size_t)py * (size_t)g_mfb.width + (size_t)px] =
                    outline;
            }
        }
    }
}

static int r36sx_mfb_draw_disk_led_saved(uint16_t *target, uint32_t now_ms,
                                         struct r36sx_mfb_saved_rect *saved)
{
    int cx;
    int cy;
    int x0;
    int y0;
    int x1;
    int y1;
    const int outer_radius = R36SX_PICO286_DISK_LED_OUTER_RADIUS;

    if (!target || !saved || !r36sx_mfb_disk_led_visible(now_ms)) {
        return 0;
    }

    r36sx_mfb_disk_led_center(&cx, &cy);
    x0 = cx - outer_radius;
    y0 = cy - outer_radius;
    x1 = cx + outer_radius;
    y1 = cy + outer_radius;
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (x1 >= g_mfb.width) {
        x1 = g_mfb.width - 1;
    }
    if (y1 >= g_mfb.height) {
        y1 = g_mfb.height - 1;
    }
    if (x1 < x0 || y1 < y0) {
        return 0;
    }

    saved->x = x0;
    saved->y = y0;
    saved->w = x1 - x0 + 1;
    saved->h = y1 - y0 + 1;
    for (int row = 0; row < saved->h; row++) {
        memcpy(&saved->pixels[(size_t)row * (size_t)saved->w],
               &target[(size_t)(saved->y + row) * (size_t)g_mfb.width +
                       (size_t)saved->x],
               (size_t)saved->w * sizeof(saved->pixels[0]));
    }

    r36sx_mfb_draw_disk_led_on(target, now_ms);
    return 1;
}

static void r36sx_mfb_restore_saved_rect(
    uint16_t *target, const struct r36sx_mfb_saved_rect *saved)
{
    if (!target || !saved || saved->w <= 0 || saved->h <= 0) {
        return;
    }

    for (int row = 0; row < saved->h; row++) {
        memcpy(&target[(size_t)(saved->y + row) * (size_t)g_mfb.width +
                       (size_t)saved->x],
               &saved->pixels[(size_t)row * (size_t)saved->w],
               (size_t)saved->w * sizeof(saved->pixels[0]));
    }
}

static int r36sx_mfb_video_source_height(void)
{
    int source_h = g_mfb.height;

    if (r36sx_screen_keyboard_is_visible(&g_mfb.osk)) {
        source_h = r36sx_pico286_video_active_height();
        if (source_h <= 0 || source_h > g_mfb.height) {
            source_h = g_mfb.height;
        }
    }
    return source_h;
}

static int r36sx_mfb_large_overlay_active(void)
{
    return r36sx_screen_keyboard_is_visible(&g_mfb.osk) ||
           r36sx_disk_menu_is_visible(&g_mfb.disk_menu) ||
           r36sx_key_presets_is_visible(&g_mfb.key_presets);
}

static void r36sx_mfb_copy_source(uint16_t *src, int content_h,
                                  int source_h)
{
    int keyboard_visible = r36sx_screen_keyboard_is_visible(&g_mfb.osk);
    int rows_to_copy = content_h;

    if (!keyboard_visible) {
        rows_to_copy = r36sx_pico286_video_active_height();
        if (rows_to_copy <= 0 || rows_to_copy > g_mfb.height) {
            rows_to_copy = g_mfb.height;
        }
    }

    for (int y = 0; y < rows_to_copy; y++) {
        uint16_t *dst_row =
            g_mfb.base_frame + (size_t)y * (size_t)g_mfb.width;

        if (keyboard_visible) {
            int start = y * source_h;
            int end = (y + 1) * source_h;
            int sy0 = start / content_h;
            int sy1 = (end - 1) / content_h;
            int weight0;
            int weight1;
            const uint16_t *src_row0;
            const uint16_t *src_row1;

            if (sy1 >= source_h) {
                sy1 = source_h - 1;
            }
            weight0 = (sy0 + 1) * content_h - start;
            if (weight0 > source_h) {
                weight0 = source_h;
            }
            if (sy0 == sy1) {
                weight1 = 0;
                weight0 = source_h;
            } else {
                weight1 = source_h - weight0;
            }

            src_row0 = src + (size_t)sy0 * (size_t)g_mfb.width;
            src_row1 = src + (size_t)sy1 * (size_t)g_mfb.width;
            if (source_h == 480 && content_h == 384) {
                uint32_t w0 = (uint32_t)(weight0 / 96);
                uint32_t w1 = (uint32_t)(weight1 / 96);
                for (int x = 0; x < g_mfb.width; x++) {
                    dst_row[x] = r36sx_mfb_blend_rgb565(src_row0[x],
                                                        src_row1[x],
                                                        w0, w1, 5u);
                }
            } else {
                for (int x = 0; x < g_mfb.width; x++) {
                    dst_row[x] = r36sx_mfb_blend_rgb565(
                        src_row0[x], src_row1[x], (uint32_t)weight0,
                        (uint32_t)weight1, (uint32_t)source_h);
                }
            }
        } else {
            const uint16_t *src_row = src + (size_t)y * (size_t)g_mfb.width;
            memcpy(dst_row, src_row, (size_t)g_mfb.width * sizeof(dst_row[0]));
        }
    }

    if (rows_to_copy < g_mfb.height) {
        memset(g_mfb.base_frame + (size_t)rows_to_copy * (size_t)g_mfb.width,
               0,
               (size_t)(g_mfb.height - rows_to_copy) *
                   (size_t)g_mfb.width * sizeof(g_mfb.base_frame[0]));
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
        R36SX_RKGAME_KEY_SELECT,
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
        r36sx_presets_handle_buttons(pressed, raw);
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
        if ((pressed & R36SX_RKGAME_KEY_B) != 0) {
            r36sx_mfb_request_soft_reset();
            g_mfb.last_raw_keys = raw;
            return 0;
        }
        if ((pressed & R36SX_RKGAME_KEY_UP) != 0) {
            r36sx_mfb_request_screenshot();
            g_mfb.last_raw_keys = raw;
            return 0;
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
        r36sx_osk_handle_buttons(pressed, raw);
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
    g_frame_generation = 0;
    r36sx_screen_keyboard_init(&g_mfb.osk);
    r36sx_screen_keyboard_set_cursor_block(&g_mfb.osk, 0);
    r36sx_disk_menu_init(&g_mfb.disk_menu);
    r36sx_key_presets_load(&g_mfb.key_presets);
    g_mfb.width = width;
    g_mfb.height = height;
    g_mfb.stride = width * R36SX_RGB565_BYTES_PER_PIXEL;
    g_mfb.frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                     sizeof(g_mfb.frame[0]));
    g_mfb.base_frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                          sizeof(g_mfb.base_frame[0]));
    if (!g_mfb.frame || !g_mfb.base_frame) {
        r36sx_pico286_debug_log("minifb: framebuffer allocation failed");
        mfb_close();
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
    uint32_t now_ms;
    uint16_t *src = (uint16_t *)buffer;
    uint32_t frame_generation;
    int content_h;
    int source_h;
    int source_dirty;
    int source_changed;
    int large_overlay_active;
    int disk_led_active;
    int need_present;
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

    now_ms = (uint32_t)(now / 1000ull);
    frame_generation = r36sx_mfb_frame_generation();
    content_h = r36sx_screen_keyboard_content_height(&g_mfb.osk, g_mfb.height);
    source_h = r36sx_mfb_video_source_height();
    source_changed = frame_generation != g_mfb.presented_generation;
    source_dirty =
        !g_mfb.base_frame_valid ||
        frame_generation != g_mfb.base_generation ||
        content_h != g_mfb.base_content_h ||
        source_h != g_mfb.base_source_h;
    large_overlay_active = r36sx_mfb_large_overlay_active();
    disk_led_active = r36sx_mfb_disk_led_active(now_ms);
    need_present =
        source_changed ||
        large_overlay_active ||
        disk_led_active ||
        g_mfb.disk_led_was_active ||
        g_mfb.force_present ||
        g_mfb.screenshot_requested;

    if (!need_present) {
        usleep(1000);
        return 0;
    }

    if (large_overlay_active) {
        if (source_dirty) {
            r36sx_mfb_copy_source(src, content_h, source_h);
            g_mfb.base_generation = frame_generation;
            g_mfb.base_content_h = content_h;
            g_mfb.base_source_h = source_h;
            g_mfb.base_frame_valid = 1;
        }
        memcpy(g_mfb.frame, g_mfb.base_frame,
               (size_t)g_mfb.width * (size_t)g_mfb.height *
                   sizeof(g_mfb.frame[0]));
        r36sx_mfb_draw_disk_led_on(g_mfb.frame, now_ms);
        if (r36sx_disk_menu_is_visible(&g_mfb.disk_menu)) {
            r36sx_disk_menu_draw_overlay();
        } else if (r36sx_key_presets_is_visible(&g_mfb.key_presets)) {
            r36sx_presets_draw();
        } else {
            r36sx_osk_draw();
        }
        if (g_mfb.screenshot_requested) {
            r36sx_mfb_save_screenshot(g_mfb.frame);
            g_mfb.screenshot_requested = 0;
        }
        g_mfb.disp_frame(g_mfb.frame, g_mfb.width, g_mfb.height, g_mfb.stride);
    } else {
        struct r36sx_mfb_saved_rect saved_led;
        int led_saved =
            r36sx_mfb_draw_disk_led_saved(src, now_ms, &saved_led);

        if (g_mfb.screenshot_requested) {
            r36sx_mfb_save_screenshot(src);
            g_mfb.screenshot_requested = 0;
        }
        g_mfb.disp_frame(src, g_mfb.width, g_mfb.height, g_mfb.stride);
        if (led_saved) {
            r36sx_mfb_restore_saved_rect(src, &saved_led);
        }
    }

    g_mfb.disk_led_was_active = disk_led_active;
    g_mfb.force_present = 0;
    g_mfb.presented_generation = frame_generation;
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
        g_palette[(uint8_t)(start + i)] =
            r36sx_mfb_rgb888_to_rgb565(new_palette[i]);
    }
}

void mfb_set_pallete(const uint8_t color_index, const uint32_t color)
{
    g_palette[color_index] = r36sx_mfb_rgb888_to_rgb565(color);
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
    if (g_mfb.base_frame) {
        free(g_mfb.base_frame);
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
