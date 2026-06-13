#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "MiniFB.h"
#include "hardware.h"
#include "r36sx_app_stats.h"
#include "r36sx_disk_config.h"
#include "r36sx_disk_menu.h"
#include "r36sx_key_presets.h"
#include "r36sx_post_overlay.h"
#include "r36sx_screenshot.h"

extern void HandleInput(unsigned int keycode, int isKeyDown);
extern void HandleMouse(int x, int y, int buttons);
extern void HandleMouseReset(void);
extern void r36sx_pico286_request_soft_reset(void);
extern void r36sx_memory_dump_request(uint8_t code, const char *reason);

#define R36SX_WIN_MENU_DISK 40001
#define R36SX_WIN_MENU_PRESETS 40002
#define R36SX_WIN_MENU_SCREENSHOT 40003
#define R36SX_WIN_MENU_STATS 40004
#define R36SX_WIN_MENU_POST_CODES 40005
#define R36SX_WIN_MENU_RESET 40006
#define R36SX_WIN_MENU_EXIT 40007
#define R36SX_WIN_MENU_MEMORY_DUMP 40008

#define R36SX_WIN_SCREENSHOT_DIR "screenshots"
#define R36SX_WIN_STATS_FONT_W 3
#define R36SX_WIN_STATS_FONT_H 5
#define R36SX_WIN_STATS_FONT_SCALE 2
#define R36SX_WIN_STATS_CHAR_ADVANCE \
    ((R36SX_WIN_STATS_FONT_W + 1) * R36SX_WIN_STATS_FONT_SCALE)
#define R36SX_WIN_STATS_ROWS 5
#define R36SX_WIN_DISK_LED_HOLD_MS 350u
#define R36SX_WIN_DISK_LED_BLINK_MS 120u
#define R36SX_WIN_DISK_LED_RADIUS 8
#define R36SX_WIN_DISK_LED_OUTER_RADIUS (R36SX_WIN_DISK_LED_RADIUS + 2)

static HWND g_wnd;
static HDC g_hdc;
static BITMAPINFO g_bmi;
static uint32_t *g_frame32;
static uint16_t *g_overlay_frame;
static int g_width;
static int g_height;
static int g_scale;
static int g_close_requested;
static uint8_t g_screenshot_requested;
static char g_key_status[512];
static volatile uint32_t g_frame_generation;
static uint32_t g_screenshot_counter;
static struct r36sx_disk_menu g_disk_menu;
static struct r36sx_key_presets g_key_presets;
static uint32_t g_menu_held_buttons;
static uint8_t g_post_codes_visible;
static uint8_t g_mouse_leave_tracking;
static volatile LONG g_post_code_generation;
static volatile LONG g_post_code_port;
static volatile LONG g_post_code_value;
static volatile LONG g_post_code_valid;
static volatile LONG g_post_subcode_port;
static volatile LONG g_post_subcode_value;
static volatile LONG g_post_subcode_valid;
static volatile LONG g_disk_activity_until_ms;
static volatile LONG g_disk_activity_depth;
static volatile LONG g_disk_activity_generation;
static LONG g_disk_led_presented_generation;

static uint32_t r36sx_win_rgb565_to_rgb888(uint16_t color)
{
    uint32_t r = (uint32_t)((color >> 11) & 0x1fu);
    uint32_t g = (uint32_t)((color >> 5) & 0x3fu);
    uint32_t b = (uint32_t)(color & 0x1fu);
    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);
    return (r << 16) | (g << 8) | b;
}

static uint16_t r36sx_win_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r >> 3) << 11) |
                      ((uint16_t)(g >> 2) << 5) |
                      (uint16_t)(b >> 3));
}

static void r36sx_win_fill_rect(uint16_t *target, int x, int y,
                                int w, int h, uint16_t color)
{
    if (!target || w <= 0 || h <= 0) {
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
    if (x + w > g_width) {
        w = g_width - x;
    }
    if (y + h > g_height) {
        h = g_height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = target + (size_t)(y + row) * (size_t)g_width +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void r36sx_win_stroke_rect(uint16_t *target, int x, int y,
                                  int w, int h, uint16_t color)
{
    r36sx_win_fill_rect(target, x, y, w, 1, color);
    r36sx_win_fill_rect(target, x, y + h - 1, w, 1, color);
    r36sx_win_fill_rect(target, x, y, 1, h, color);
    r36sx_win_fill_rect(target, x + w - 1, y, 1, h, color);
}

static int r36sx_win_disk_led_visible(uint32_t now_ms)
{
    /*
     * GetTickCount wraps, so keep the same signed-delta comparison used by the
     * device MiniFB backend.  Real disk I/O uses the busy-depth path; the
     * timed branch is only a fallback for any older event-style callers.
     */
    if (InterlockedCompareExchange(&g_disk_activity_depth, 0, 0) > 0) {
        return 1;
    }
    if ((int32_t)((uint32_t)g_disk_activity_until_ms - now_ms) <= 0) {
        return 0;
    }
    return ((now_ms / R36SX_WIN_DISK_LED_BLINK_MS) & 1u) != 0u;
}

static void r36sx_win_draw_disk_led(uint16_t *target, uint32_t now_ms,
                                    int force_visible)
{
    const int radius = R36SX_WIN_DISK_LED_RADIUS;
    const int outer_radius = R36SX_WIN_DISK_LED_OUTER_RADIUS;
    const int cx = g_width - radius - 12;
    const int cy = g_height - radius - 12;
    const uint16_t red = 0xf800u;
    const uint16_t dark_red = 0x6000u;
    const uint16_t outline = 0x0000u;

    if (!target ||
        (!force_visible && !r36sx_win_disk_led_visible(now_ms))) {
        return;
    }

    for (int y = -outer_radius; y <= outer_radius; y++) {
        int py = cy + y;
        if (py < 0 || py >= g_height) {
            continue;
        }
        for (int x = -outer_radius; x <= outer_radius; x++) {
            int px = cx + x;
            int dist2 = x * x + y * y;
            if (px < 0 || px >= g_width) {
                continue;
            }
            if (dist2 <= radius * radius) {
                target[(size_t)py * (size_t)g_width + (size_t)px] =
                    dist2 <= (radius - 3) * (radius - 3) ? red : dark_red;
            } else if (dist2 <= outer_radius * outer_radius) {
                target[(size_t)py * (size_t)g_width + (size_t)px] = outline;
            }
        }
    }
}

static const uint8_t *r36sx_win_stats_glyph(char ch)
{
    static const uint8_t blank[5] = { 0, 0, 0, 0, 0 };
    static const uint8_t digits[10][5] = {
        { 7, 5, 5, 5, 7 }, { 2, 6, 2, 2, 7 },
        { 7, 1, 7, 4, 7 }, { 7, 1, 7, 1, 7 },
        { 5, 5, 7, 1, 1 }, { 7, 4, 7, 1, 7 },
        { 7, 4, 7, 5, 7 }, { 7, 1, 2, 2, 2 },
        { 7, 5, 7, 5, 7 }, { 7, 5, 7, 1, 7 }
    };
    static const uint8_t slash[5] = { 1, 1, 2, 4, 4 };
    static const uint8_t glyph_a[5] = { 7, 5, 7, 5, 5 };
    static const uint8_t glyph_d[5] = { 6, 5, 5, 5, 6 };
    static const uint8_t glyph_e[5] = { 7, 4, 6, 4, 7 };
    static const uint8_t glyph_f[5] = { 7, 4, 6, 4, 4 };
    static const uint8_t glyph_i[5] = { 7, 2, 2, 2, 7 };
    static const uint8_t glyph_k[5] = { 5, 5, 6, 5, 5 };
    static const uint8_t glyph_o[5] = { 7, 5, 5, 5, 7 };
    static const uint8_t glyph_p[5] = { 6, 5, 6, 4, 4 };
    static const uint8_t glyph_q[5] = { 7, 5, 5, 7, 1 };
    static const uint8_t glyph_r[5] = { 6, 5, 6, 5, 5 };
    static const uint8_t glyph_s[5] = { 7, 4, 7, 1, 7 };
    static const uint8_t glyph_t[5] = { 7, 2, 2, 2, 2 };
    static const uint8_t glyph_w[5] = { 5, 5, 5, 7, 5 };
    static const uint8_t glyph_x[5] = { 5, 5, 2, 5, 5 };
    static const uint8_t colon[5] = { 0, 2, 0, 2, 0 };
    static const uint8_t minus[5] = { 0, 0, 7, 0, 0 };

    if (ch >= '0' && ch <= '9') {
        return digits[ch - '0'];
    }
    if (ch >= 'a' && ch <= 'z') {
        ch = (char)(ch - 'a' + 'A');
    }

    switch (ch) {
        case '/': return slash;
        case ':': return colon;
        case '-': return minus;
        case 'A': return glyph_a;
        case 'D': return glyph_d;
        case 'E': return glyph_e;
        case 'F': return glyph_f;
        case 'I': return glyph_i;
        case 'K': return glyph_k;
        case 'O': return glyph_o;
        case 'P': return glyph_p;
        case 'Q': return glyph_q;
        case 'R': return glyph_r;
        case 'S': return glyph_s;
        case 'T': return glyph_t;
        case 'W': return glyph_w;
        case 'X': return glyph_x;
        default: return blank;
    }
}

static int r36sx_win_stats_text_width(const char *text)
{
    int len = text ? (int)strlen(text) : 0;

    if (len <= 0) {
        return 0;
    }
    return len * R36SX_WIN_STATS_CHAR_ADVANCE -
           R36SX_WIN_STATS_FONT_SCALE;
}

static void r36sx_win_draw_stats_text(uint16_t *target, int x, int y,
                                      const char *text, uint16_t color)
{
    for (int i = 0; text && text[i] != '\0'; i++) {
        const uint8_t *glyph = r36sx_win_stats_glyph(text[i]);
        int char_x = x + i * R36SX_WIN_STATS_CHAR_ADVANCE;

        for (int row = 0; row < R36SX_WIN_STATS_FONT_H; row++) {
            uint8_t bits = glyph[row];
            for (int col = 0; col < R36SX_WIN_STATS_FONT_W; col++) {
                if ((bits & (uint8_t)(1u << (R36SX_WIN_STATS_FONT_W -
                                             1 - col))) == 0) {
                    continue;
                }
                r36sx_win_fill_rect(
                    target,
                    char_x + col * R36SX_WIN_STATS_FONT_SCALE,
                    y + row * R36SX_WIN_STATS_FONT_SCALE,
                    R36SX_WIN_STATS_FONT_SCALE,
                    R36SX_WIN_STATS_FONT_SCALE,
                    color);
            }
        }
    }
}

static void r36sx_win_draw_post_codes_overlay(uint16_t *target)
{
    LONG generation = g_post_code_generation;
    LONG value = g_post_code_value;
    LONG code_valid = g_post_code_valid;
    LONG sub_value = g_post_subcode_value;
    LONG sub_valid = g_post_subcode_valid;

    if (!target || !g_post_codes_visible) {
        return;
    }

    r36sx_post_overlay_draw(target, g_width, g_height, g_width,
                            (uint8_t)(generation != 0 && code_valid),
                            (uint8_t)(value & 0xff),
                            (uint8_t)sub_valid,
                            (uint8_t)(sub_value & 0xff));
}

static void r36sx_win_draw_stats_overlay(uint16_t *target)
{
    r36sx_app_stats_snapshot_t stats;
    static const char *labels[R36SX_WIN_STATS_ROWS] = {
        "X86", "QPS", "READ", "WRITE", "FPS"
    };
    char values[R36SX_WIN_STATS_ROWS][16];
    int label_w = r36sx_win_stats_text_width("WRITE");
    int value_w = 0;
    const int pad = 4;
    const int gap = 7;
    const int row_h = 12;
    int box_w;
    int box_h;
    int x;
    int y;

    if (!target || !r36sx_app_stats_is_visible()) {
        return;
    }

    r36sx_app_stats_snapshot(&stats);
    snprintf(values[0], sizeof(values[0]), "%luK/S",
             (unsigned long)(stats.x86_per_sec / 1000u));
    snprintf(values[1], sizeof(values[1]), "%lu",
             (unsigned long)stats.quanta_per_sec);
    snprintf(values[2], sizeof(values[2]), "%luK/S",
             (unsigned long)stats.disk_read_kb_per_sec);
    snprintf(values[3], sizeof(values[3]), "%luK/S",
             (unsigned long)stats.disk_write_kb_per_sec);
    snprintf(values[4], sizeof(values[4]), "%lu",
             (unsigned long)stats.fps);

    for (int i = 0; i < R36SX_WIN_STATS_ROWS; i++) {
        int width = r36sx_win_stats_text_width(values[i]);
        if (width > value_w) {
            value_w = width;
        }
    }

    box_w = pad * 2 + label_w + gap + value_w;
    box_h = pad * 2 + R36SX_WIN_STATS_ROWS * row_h -
            (row_h - R36SX_WIN_STATS_FONT_H *
                     R36SX_WIN_STATS_FONT_SCALE);
    x = g_width - box_w - 8;
    y = g_height - box_h - 8;
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }

    r36sx_win_fill_rect(target, x, y, box_w, box_h,
                        r36sx_win_rgb565(5, 8, 12));
    r36sx_win_stroke_rect(target, x, y, box_w, box_h,
                          r36sx_win_rgb565(70, 96, 112));
    for (int i = 0; i < R36SX_WIN_STATS_ROWS; i++) {
        int row_y = y + pad + i * row_h;
        r36sx_win_draw_stats_text(target, x + pad, row_y, labels[i],
                                  r36sx_win_rgb565(176, 202, 214));
        r36sx_win_draw_stats_text(target, x + pad + label_w + gap,
                                  row_y, values[i],
                                  r36sx_win_rgb565(236, 242, 220));
    }
}

void r36sx_mfb_mark_frame_ready(void)
{
    InterlockedIncrement((volatile LONG *)&g_frame_generation);
}

static void r36sx_win_extend_disk_activity(void)
{
    InterlockedExchange(&g_disk_activity_until_ms,
                        (LONG)(GetTickCount() +
                               R36SX_WIN_DISK_LED_HOLD_MS));
    InterlockedIncrement(&g_disk_activity_generation);
}

void r36sx_pico286_disk_activity(void)
{
    r36sx_win_extend_disk_activity();
}

void r36sx_pico286_disk_activity_begin(void)
{
    r36sx_win_extend_disk_activity();
    InterlockedIncrement(&g_disk_activity_depth);
}

void r36sx_pico286_disk_activity_end(void)
{
    LONG depth = InterlockedDecrement(&g_disk_activity_depth);

    if (depth < 0) {
        InterlockedExchange(&g_disk_activity_depth, 0);
        return;
    }
    r36sx_win_extend_disk_activity();
}

void r36sx_pico286_post_code_out(uint16_t portnum, uint8_t value)
{
    /*
     * Keep the standard POST stage and R36SX test386 sub-step separately, so
     * a 190h breadcrumb does not hide the major 80h stage on the overlay.
     */
    if (portnum == R36SX_POST_OVERLAY_SUBCODE_PORT) {
        g_post_subcode_port = (LONG)portnum;
        g_post_subcode_value = (LONG)value;
        g_post_subcode_valid = 1;
    } else {
        g_post_code_port = (LONG)portnum;
        g_post_code_value = (LONG)value;
        g_post_code_valid = 1;
    }
    InterlockedIncrement(&g_post_code_generation);
    r36sx_pico286_debug_log("post: port=0x%03x code=0x%02x",
                            portnum, value);
}

void r36sx_pico286_post_code_reset(void)
{
    InterlockedExchange(&g_post_code_port, 0);
    InterlockedExchange(&g_post_code_value, 0);
    InterlockedExchange(&g_post_code_valid, 0);
    InterlockedExchange(&g_post_subcode_port, 0);
    InterlockedExchange(&g_post_subcode_value, 0);
    InterlockedExchange(&g_post_subcode_valid, 0);
    InterlockedIncrement(&g_post_code_generation);
    r36sx_pico286_debug_log("post: reset");
}

static int r36sx_win_menu_visible(void)
{
    return r36sx_disk_menu_is_visible(&g_disk_menu) ||
           r36sx_key_presets_is_visible(&g_key_presets);
}

int mfb_vm_paused(void)
{
    return r36sx_win_menu_visible();
}

static void r36sx_win_update_stats_menu_check(void)
{
    HMENU menu = g_wnd ? GetMenu(g_wnd) : NULL;

    if (!menu) {
        return;
    }
    CheckMenuItem(menu, R36SX_WIN_MENU_STATS,
                  MF_BYCOMMAND |
                  (r36sx_app_stats_is_visible() ? MF_CHECKED :
                                                  MF_UNCHECKED));
}

static void r36sx_win_update_post_menu_check(void)
{
    HMENU menu = g_wnd ? GetMenu(g_wnd) : NULL;

    if (!menu) {
        return;
    }
    CheckMenuItem(menu, R36SX_WIN_MENU_POST_CODES,
                  MF_BYCOMMAND |
                  (g_post_codes_visible ? MF_CHECKED : MF_UNCHECKED));
}

static void r36sx_win_open_disk_menu(void)
{
    r36sx_key_presets_set_visible(&g_key_presets, 0);
    r36sx_disk_menu_set_visible(&g_disk_menu,
                                !r36sx_disk_menu_is_visible(&g_disk_menu));
    g_menu_held_buttons = 0;
}

static void r36sx_win_open_key_presets(void)
{
    r36sx_disk_menu_set_visible(&g_disk_menu, 0);
    r36sx_key_presets_set_visible(
        &g_key_presets,
        !r36sx_key_presets_is_visible(&g_key_presets));
    g_menu_held_buttons = 0;
}

static void r36sx_win_request_screenshot(void)
{
    g_screenshot_requested = 1;
    r36sx_pico286_debug_log("winminifb: F12 screenshot requested");
}

static void r36sx_win_request_memory_dump(void)
{
    r36sx_memory_dump_request(0, "windows-ui");
    r36sx_pico286_debug_log("winminifb: memory dump requested");
}

static void r36sx_win_toggle_post_codes(void)
{
    g_post_codes_visible = !g_post_codes_visible;
    r36sx_win_update_post_menu_check();
    r36sx_pico286_debug_log("winminifb: POST codes %s",
                            g_post_codes_visible ? "on" : "off");
}

static void r36sx_win_save_screenshot(const uint16_t *pixels)
{
    r36sx_screenshot_options_t options;
    char path[512];
    r36sx_pico286_screenshot_format_t format;

    if (!pixels) {
        g_screenshot_requested = 0;
        return;
    }

    format = r36sx_pico286_screenshot_format();
    memset(&options, 0, sizeof(options));
    options.primary_dir = R36SX_WIN_SCREENSHOT_DIR;
    options.prefix = "pico_286_win";
    options.unix_time = r36sx_pico286_rtc_current_time_unix();
    options.sequence = g_screenshot_counter++;
    options.format = format == R36SX_PICO286_SCREENSHOT_FORMAT_BMP ?
        R36SX_SCREENSHOT_FORMAT_BMP : R36SX_SCREENSHOT_FORMAT_PNG;
    options.include_build_hash =
        r36sx_pico286_screenshot_build_hash_enabled();
    options.build_hash_sha256 = R36SX_BUILD_COMMIT_OBJECT_SHA256;

    if (r36sx_screenshot_save_rgb565(&options, pixels, g_width, g_height,
                                     path, sizeof(path)) == 0) {
        r36sx_pico286_debug_log("winminifb: screenshot saved %s", path);
        g_screenshot_requested = 0;
        return;
    }

    /*
     * Windows debug builds may not have a loadable screenshot.so for PNG.
     * Keep F12 useful by falling back to the built-in BMP writer.
     */
    if (options.format == R36SX_SCREENSHOT_FORMAT_PNG) {
        options.format = R36SX_SCREENSHOT_FORMAT_BMP;
        if (r36sx_screenshot_save_rgb565(&options, pixels, g_width,
                                         g_height, path, sizeof(path)) == 0) {
            r36sx_pico286_debug_log("winminifb: screenshot saved %s", path);
            g_screenshot_requested = 0;
            return;
        }
    }

    r36sx_pico286_debug_log("winminifb: screenshot save failed");
    g_screenshot_requested = 0;
}

static uint32_t r36sx_win_button_from_key(unsigned int key)
{
    switch (key) {
        case VK_UP: return R36SX_RKGAME_KEY_UP;
        case VK_DOWN: return R36SX_RKGAME_KEY_DOWN;
        case VK_LEFT: return R36SX_RKGAME_KEY_LEFT;
        case VK_RIGHT: return R36SX_RKGAME_KEY_RIGHT;
        case VK_RETURN: return R36SX_RKGAME_KEY_A;
        case VK_SPACE: return R36SX_RKGAME_KEY_A;
        case VK_ESCAPE: return R36SX_RKGAME_KEY_B;
        case VK_BACK: return R36SX_RKGAME_KEY_B;
        case VK_TAB: return R36SX_RKGAME_KEY_SELECT;
        case VK_PRIOR: return R36SX_RKGAME_KEY_L2;
        case VK_NEXT: return R36SX_RKGAME_KEY_R2;
        case 'A': return R36SX_RKGAME_KEY_A;
        case 'B': return R36SX_RKGAME_KEY_B;
        case 'X': return R36SX_RKGAME_KEY_X;
        case 'Y': return R36SX_RKGAME_KEY_Y;
        case 'L': return R36SX_RKGAME_KEY_L;
        case 'R': return R36SX_RKGAME_KEY_R;
        case 'S': return R36SX_RKGAME_KEY_START;
        default: return 0;
    }
}

static void r36sx_win_handle_disk_menu_buttons(uint32_t pressed,
                                               uint32_t held)
{
    uint64_t now_us = (uint64_t)GetTickCount() * 1000ull;
    uint32_t result = r36sx_disk_menu_handle_buttons(&g_disk_menu,
                                                     pressed,
                                                     held,
                                                     now_us);
    if ((result & R36SX_DISK_MENU_RESULT_EXIT_APP) != 0) {
        g_close_requested = 1;
    }
    if ((result & R36SX_DISK_MENU_RESULT_RESET_PC) != 0) {
        r36sx_pico286_request_soft_reset();
    }
    if ((result & R36SX_DISK_MENU_RESULT_MEMORY_DUMP) != 0) {
        r36sx_win_request_memory_dump();
    }
}

static void r36sx_win_handle_key_preset_buttons(uint32_t pressed,
                                                uint32_t held)
{
    uint32_t result =
        r36sx_key_presets_handle_buttons(&g_key_presets, pressed, held);
    if ((result & R36SX_KEY_PRESET_RESULT_CLOSED) != 0) {
        r36sx_key_presets_set_visible(&g_key_presets, 0);
    }
}

static int r36sx_win_handle_menu_key(unsigned int key, int is_down)
{
    uint32_t button = r36sx_win_button_from_key(key);
    uint32_t pressed = 0;

    if (!button || !r36sx_win_menu_visible()) {
        return 0;
    }

    if (is_down) {
        pressed = button & ~g_menu_held_buttons;
        g_menu_held_buttons |= button;
    } else {
        g_menu_held_buttons &= ~button;
    }

    if (pressed) {
        if (r36sx_disk_menu_is_visible(&g_disk_menu)) {
            r36sx_win_handle_disk_menu_buttons(pressed,
                                               g_menu_held_buttons);
        } else if (r36sx_key_presets_is_visible(&g_key_presets)) {
            r36sx_win_handle_key_preset_buttons(pressed,
                                                g_menu_held_buttons);
        }
    } else if (r36sx_disk_menu_is_visible(&g_disk_menu)) {
        r36sx_win_handle_disk_menu_buttons(0, g_menu_held_buttons);
    } else if (r36sx_key_presets_is_visible(&g_key_presets)) {
        /*
         * The key-preset picker uses held state for virtual-keyboard modifier
         * keys, so feed key releases even when there is no new press edge.
         */
        r36sx_win_handle_key_preset_buttons(0, g_menu_held_buttons);
    }

    return 1;
}

static void r36sx_win_handle_command(WORD command)
{
    switch (command) {
        case R36SX_WIN_MENU_DISK:
            r36sx_win_open_disk_menu();
            break;
        case R36SX_WIN_MENU_PRESETS:
            r36sx_win_open_key_presets();
            break;
        case R36SX_WIN_MENU_SCREENSHOT:
            r36sx_win_request_screenshot();
            break;
        case R36SX_WIN_MENU_MEMORY_DUMP:
            r36sx_win_request_memory_dump();
            break;
        case R36SX_WIN_MENU_STATS:
            r36sx_app_stats_toggle_visible();
            r36sx_win_update_stats_menu_check();
            break;
        case R36SX_WIN_MENU_POST_CODES:
            r36sx_win_toggle_post_codes();
            break;
        case R36SX_WIN_MENU_RESET:
            r36sx_pico286_request_soft_reset();
            break;
        case R36SX_WIN_MENU_EXIT:
            g_close_requested = 1;
            break;
        default:
            break;
    }
}

static unsigned int r36sx_win_keycode(WPARAM w_param, LPARAM l_param)
{
    unsigned int key = (unsigned int)w_param;
    unsigned int scan = (unsigned int)((l_param >> 16) & 0xffu);

    if (w_param == VK_SHIFT) {
        key = (unsigned int)MapVirtualKey(scan, MAPVK_VSC_TO_VK_EX);
    } else if (w_param == VK_CONTROL) {
        key = (l_param & (1L << 24)) ? VK_RCONTROL : VK_LCONTROL;
    } else if (w_param == VK_MENU) {
        key = (l_param & (1L << 24)) ? VK_RMENU : VK_LMENU;
    }

    return key;
}

static int r36sx_win_mouse_buttons(WPARAM w_param)
{
    int buttons = 0;
    if ((w_param & MK_RBUTTON) != 0) {
        buttons |= 1;
    }
    if ((w_param & MK_LBUTTON) != 0) {
        buttons |= 2;
    }
    return buttons;
}

static int r36sx_win_mouse_coord_to_frame(int value, int limit)
{
    const int scale = g_scale > 0 ? g_scale : 1;
    value /= scale;
    if (value < 0) {
        return 0;
    }
    if (value >= limit) {
        return limit > 0 ? limit - 1 : 0;
    }
    return value;
}

static void r36sx_win_track_mouse_leave(HWND wnd)
{
    TRACKMOUSEEVENT tme;

    if (g_mouse_leave_tracking) {
        return;
    }

    memset(&tme, 0, sizeof(tme));
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = wnd;
    if (TrackMouseEvent(&tme)) {
        g_mouse_leave_tracking = 1;
    }
}

static void r36sx_win_handle_mouse(HWND wnd, UINT msg, WPARAM w_param,
                                   LPARAM l_param)
{
    int x;
    int y;
    int buttons;

    if (r36sx_win_menu_visible()) {
        HandleMouseReset();
        return;
    }

    buttons = r36sx_win_mouse_buttons(w_param);
    if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN) {
        SetCapture(wnd);
    } else if ((msg == WM_LBUTTONUP || msg == WM_RBUTTONUP) &&
               buttons == 0 && GetCapture() == wnd) {
        ReleaseCapture();
    }

    r36sx_win_track_mouse_leave(wnd);
    x = r36sx_win_mouse_coord_to_frame((int)(short)LOWORD(l_param), g_width);
    y = r36sx_win_mouse_coord_to_frame((int)(short)HIWORD(l_param), g_height);
    HandleMouse(x, y, buttons);
}

static LRESULT CALLBACK r36sx_win_wndproc(HWND wnd, UINT msg,
                                          WPARAM w_param, LPARAM l_param)
{
    switch (msg) {
        case WM_CLOSE:
            g_close_requested = 1;
            return 0;

        case WM_DESTROY:
            g_close_requested = 1;
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            r36sx_win_handle_command(LOWORD(w_param));
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(wnd, &ps);
            if (g_frame32) {
                StretchDIBits(ps.hdc,
                              0, 0, g_width * g_scale, g_height * g_scale,
                              0, 0, g_width, g_height,
                              g_frame32, &g_bmi, DIB_RGB_COLORS, SRCCOPY);
            }
            EndPaint(wnd, &ps);
            return 0;
        }

        case WM_MOUSEMOVE:
            r36sx_win_handle_mouse(wnd, msg, w_param, l_param);
            return 0;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            r36sx_win_handle_mouse(wnd, msg, w_param, l_param);
            return 0;

        case WM_MOUSELEAVE:
            g_mouse_leave_tracking = 0;
            HandleMouseReset();
            return 0;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            unsigned int key = r36sx_win_keycode(w_param, l_param);
            int is_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            int first_down = is_down && ((l_param & (1L << 30)) == 0);

            if (first_down && key == VK_F11) {
                r36sx_win_open_disk_menu();
                return 0;
            }
            if (first_down && key == VK_F12) {
                r36sx_win_request_screenshot();
                return 0;
            }
            if (first_down && key == 'R' && (GetKeyState(VK_CONTROL) < 0)) {
                r36sx_pico286_request_soft_reset();
                return 0;
            }
            if (r36sx_win_handle_menu_key(key, is_down)) {
                return 0;
            }
            if (key < sizeof(g_key_status)) {
                g_key_status[key] = (char)is_down;
            }
            HandleInput(key, is_down);
            return 0;
        }
    }

    return DefWindowProc(wnd, msg, w_param, l_param);
}

int mfb_open(const char *name, int width, int height, int scale)
{
    WNDCLASS wc;
    RECT rect;
    HMENU menu;
    HMENU host_menu;
    const char *title = name ? name : "Pico-286";

    memset(&wc, 0, sizeof(wc));
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = r36sx_win_wndproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "R36SX_Pico286_Win";
    RegisterClass(&wc);

    g_width = width;
    g_height = height;
    g_scale = scale > 0 ? scale : 1;
    g_close_requested = 0;
    g_screenshot_requested = 0;
    g_frame_generation = 0;
    g_screenshot_counter = 0;
    g_menu_held_buttons = 0;
    g_post_codes_visible = 0;
    g_mouse_leave_tracking = 0;
    HandleMouseReset();
    g_post_code_generation = 0;
    g_post_code_port = 0;
    g_post_code_value = 0;
    g_post_code_valid = 0;
    g_post_subcode_port = 0;
    g_post_subcode_value = 0;
    g_post_subcode_valid = 0;
    g_disk_activity_until_ms = 0;
    g_disk_activity_depth = 0;
    g_disk_activity_generation = 0;
    g_disk_led_presented_generation = 0;
    r36sx_app_stats_init();
    r36sx_disk_menu_init(&g_disk_menu);
    r36sx_key_presets_load(&g_key_presets);

    menu = CreateMenu();
    host_menu = CreatePopupMenu();
    if (menu && host_menu) {
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_DISK,
                   "Settings\tF11");
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_PRESETS,
                   "Key presets");
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_SCREENSHOT,
                   "Screenshot\tF12");
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_MEMORY_DUMP,
                   "Dump memory");
        AppendMenu(host_menu, MF_STRING | MF_UNCHECKED,
                   R36SX_WIN_MENU_STATS, "Show statistics");
        AppendMenu(host_menu, MF_STRING | MF_UNCHECKED,
                   R36SX_WIN_MENU_POST_CODES, "Show POST codes");
        AppendMenu(host_menu, MF_SEPARATOR, 0, NULL);
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_RESET,
                   "Soft reset\tCtrl+R");
        AppendMenu(host_menu, MF_STRING, R36SX_WIN_MENU_EXIT, "Exit");
        AppendMenu(menu, MF_POPUP, (UINT_PTR)host_menu, "Host");
    } else if (host_menu) {
        DestroyMenu(host_menu);
        host_menu = NULL;
    }

    rect.left = 0;
    rect.top = 0;
    rect.right = width * g_scale;
    rect.bottom = height * g_scale;
    AdjustWindowRect(&rect,
                     WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
                     menu != NULL);

    g_wnd = CreateWindowEx(0, wc.lpszClassName, title,
                           WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX &
                               ~WS_THICKFRAME,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           NULL, menu, wc.hInstance, NULL);
    if (!g_wnd) {
        if (menu) {
            DestroyMenu(menu);
        }
        return 0;
    }

    memset(&g_bmi, 0, sizeof(g_bmi));
    g_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    g_bmi.bmiHeader.biWidth = width;
    g_bmi.bmiHeader.biHeight = -height;
    g_bmi.bmiHeader.biPlanes = 1;
    g_bmi.bmiHeader.biBitCount = 32;
    g_bmi.bmiHeader.biCompression = BI_RGB;

    g_frame32 = (uint32_t *)calloc((size_t)width * (size_t)height,
                                   sizeof(g_frame32[0]));
    g_overlay_frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                         sizeof(g_overlay_frame[0]));
    if (!g_frame32 || !g_overlay_frame) {
        free(g_frame32);
        free(g_overlay_frame);
        g_frame32 = NULL;
        g_overlay_frame = NULL;
        DestroyWindow(g_wnd);
        g_wnd = NULL;
        return 0;
    }

    g_hdc = GetDC(g_wnd);
    ShowWindow(g_wnd, SW_SHOW);
    UpdateWindow(g_wnd);
    return 1;
}

int mfb_update(void *buffer, int fps_limit)
{
    MSG msg;
    const uint16_t *src = (const uint16_t *)buffer;
    const uint16_t *present_src;
    size_t pixels;
    uint32_t now_ms;
    LONG disk_activity_generation;
    int disk_led_pending;
    int disk_led_visible;
    (void)fps_limit;

    if (!g_wnd || !g_frame32 || !g_overlay_frame || !src ||
        g_close_requested) {
        return -1;
    }

    pixels = (size_t)g_width * (size_t)g_height;
    now_ms = GetTickCount();
    disk_activity_generation =
        InterlockedCompareExchange(&g_disk_activity_generation, 0, 0);
    disk_led_pending =
        disk_activity_generation != g_disk_led_presented_generation;
    disk_led_visible = disk_led_pending || r36sx_win_disk_led_visible(now_ms);
    present_src = src;
    if (r36sx_win_menu_visible() || r36sx_app_stats_is_visible() ||
        g_post_codes_visible || disk_led_visible) {
        /*
         * The Windows host draws debug overlays on a temporary RGB565 copy so
         * the emulated PC framebuffer stays untouched.
         */
        if (r36sx_disk_menu_is_visible(&g_disk_menu)) {
            r36sx_win_handle_disk_menu_buttons(0, g_menu_held_buttons);
        } else if (r36sx_key_presets_is_visible(&g_key_presets)) {
            r36sx_win_handle_key_preset_buttons(0, g_menu_held_buttons);
        }
        memcpy(g_overlay_frame, src, pixels * sizeof(g_overlay_frame[0]));
        if (r36sx_disk_menu_is_visible(&g_disk_menu)) {
            r36sx_disk_menu_draw(&g_disk_menu, g_overlay_frame, g_width,
                                 g_height, g_width);
        } else if (r36sx_key_presets_is_visible(&g_key_presets)) {
            r36sx_key_presets_draw(&g_key_presets, g_overlay_frame, g_width,
                                   g_height, g_width);
        } else {
            r36sx_win_draw_stats_overlay(g_overlay_frame);
            r36sx_win_draw_post_codes_overlay(g_overlay_frame);
        }
        r36sx_win_draw_disk_led(g_overlay_frame, now_ms, disk_led_pending);
        present_src = g_overlay_frame;
    }
    if (g_screenshot_requested) {
        r36sx_win_save_screenshot(present_src);
    }
    for (size_t i = 0; i < pixels; ++i) {
        g_frame32[i] = r36sx_win_rgb565_to_rgb888(present_src[i]);
    }

    StretchDIBits(g_hdc, 0, 0, g_width * g_scale, g_height * g_scale,
                  0, 0, g_width, g_height,
                  g_frame32, &g_bmi, DIB_RGB_COLORS, SRCCOPY);
    if (disk_led_pending) {
        g_disk_led_presented_generation = disk_activity_generation;
    }
    r36sx_app_stats_record_frame();

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            g_close_requested = 1;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return g_close_requested ? -1 : 0;
}

void mfb_set_pallete_array(const uint32_t *new_palette, uint8_t start,
                           uint8_t count)
{
    (void)new_palette;
    (void)start;
    (void)count;
}

void mfb_set_pallete(const uint8_t color_index, const uint32_t color)
{
    (void)color_index;
    (void)color;
}

void mfb_close(void)
{
    g_mouse_leave_tracking = 0;
    HandleMouseReset();
    if (g_hdc && g_wnd) {
        ReleaseDC(g_wnd, g_hdc);
    }
    g_hdc = NULL;
    free(g_frame32);
    g_frame32 = NULL;
    free(g_overlay_frame);
    g_overlay_frame = NULL;
    if (g_wnd) {
        DestroyWindow(g_wnd);
    }
    g_wnd = NULL;
}

char *mfb_keystatus(void)
{
    return g_key_status;
}
