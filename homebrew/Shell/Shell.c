/*
 * R36SX framebuffer shell.
 *
 * Standalone MIPS executable for the stock Hichip firmware. It presents a
 * small VT100-ish terminal on the vendor driver.so framebuffer path and feeds
 * the stock /bin/sh through a pseudo terminal. The on-screen keyboard is the
 * shared R36SX keyboard also used by Pico-286.
 */

#define _GNU_SOURCE
#define _XOPEN_SOURCE 600

#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../common/hardware.h"
#include "../common/r36sx_screen_keyboard.h"
#include "../pico_286/pico-286/src/emulator/includes/font8x16.h"

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define SHELL_SCREENSHOT_DIR R36SX_MIPS_NATIVE_DIR "/Shell/screenshots"
#define SHELL_SCREENSHOT_LOCAL_DIR "screenshots"
#define SHELL_SCREENSHOT_PATH_MAX 512

enum {
    FB_WIDTH = R36SX_SCREEN_WIDTH,
    FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    FONT_W = 8,
    FONT_H = 16,
    TERM_FONT_PX = 13,
    TERM_FONT_BASELINE = 13,
    TERM_COLS = FB_WIDTH / FONT_W,
    TERM_MAX_ROWS = FB_HEIGHT / FONT_H,
    TERM_FONT_CACHE_SLOTS = 512,
    EVDEV_MAX_FDS = 16,
    EVDEV_RESCAN_USEC = 2000000,
    FRAME_USEC = 16666,
    CSI_BUF_LEN = 48,
    PTY_READ_BUF = 512,
    PIPE_LINE_MAX = 1024
};

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);
typedef FT_Error (*term_ft_init_free_type_fn)(FT_Library *);
typedef FT_Error (*term_ft_new_face_fn)(
    FT_Library, const char *, FT_Long, FT_Face *);
typedef FT_Error (*term_ft_done_face_fn)(FT_Face);
typedef FT_Error (*term_ft_done_free_type_fn)(FT_Library);
typedef FT_Error (*term_ft_select_charmap_fn)(FT_Face, FT_Encoding);
typedef FT_Error (*term_ft_set_pixel_sizes_fn)(FT_Face, FT_UInt, FT_UInt);
typedef FT_Error (*term_ft_load_char_fn)(FT_Face, FT_ULong, FT_Int32);

struct driver_state {
    void *handle;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
    int active;
};

struct cell {
    uint32_t ch;
    uint8_t fg;
    uint8_t bg;
    uint8_t flags;
};

struct terminal_state {
    struct cell cells[TERM_MAX_ROWS][TERM_COLS];
    int rows;
    int cursor_x;
    int cursor_y;
    uint8_t fg;
    uint8_t bg;
    uint8_t bright;
    uint8_t reverse;
    uint8_t esc_state;
    uint8_t utf8_remaining;
    uint32_t utf8_codepoint;
    char csi[CSI_BUF_LEN];
    int csi_len;
};

struct pty_state {
    int master_fd;
    int read_fd;
    int write_fd;
    pid_t child_pid;
    int child_exited;
    int pipe_mode;
    int pipe_esc_state;
    char pipe_line[PIPE_LINE_MAX];
    size_t pipe_line_len;
};

struct key_state {
    int shift;
    int ctrl;
    int alt;
    int caps_lock;
};

struct evdev_keyboard_state {
    int fds[EVDEV_MAX_FDS];
    char paths[EVDEV_MAX_FDS][32];
    uint64_t next_scan_us;
    int shift;
    int ctrl;
    int alt;
    int caps_lock;
};

struct term_glyph_cache_entry {
    uint32_t codepoint;
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

struct term_font_state {
    void *handle;
    FT_Library library;
    FT_Face face;
    term_ft_init_free_type_fn init_free_type;
    term_ft_new_face_fn new_face;
    term_ft_done_face_fn done_face;
    term_ft_done_free_type_fn done_free_type;
    term_ft_select_charmap_fn select_charmap;
    term_ft_set_pixel_sizes_fn set_pixel_sizes;
    term_ft_load_char_fn load_char;
    struct term_glyph_cache_entry cache[TERM_FONT_CACHE_SLOTS];
    uint32_t cache_age;
    uint8_t attempted;
    uint8_t active;
};

static struct driver_state g_driver;
static struct terminal_state g_term;
static struct pty_state g_pty = { -1, -1, -1, -1, 0, 0, 0, { 0 }, 0 };
static struct key_state g_keys;
static struct evdev_keyboard_state g_evdev;
static struct term_font_state g_font;
static struct r36sx_screen_keyboard g_keyboard;
static uint16_t g_frame[FB_WIDTH * FB_HEIGHT];
static uint32_t g_prev_buttons;
static uint32_t g_screenshot_counter;
static uint8_t g_screenshot_requested;
static int g_running = 1;
static int g_last_winsize_rows;

static uint16_t rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static const uint16_t g_palette[16] = {
    0x0000, 0x7800, 0x03c0, 0x7bc0, 0x0018, 0x7818, 0x03d8, 0xbdf7,
    0x8410, 0xf800, 0x07e0, 0xffe0, 0x001f, 0xf81f, 0x07ff, 0xffff
};

static uint64_t monotonic_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull +
           (uint64_t)ts.tv_nsec / 1000ull;
}

static void fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0) {
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
    if (x + w > FB_WIDTH) {
        w = FB_WIDTH - x;
    }
    if (y + h > FB_HEIGHT) {
        h = FB_HEIGHT - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }
    for (int yy = y; yy < y + h; yy++) {
        uint16_t *row = &g_frame[(size_t)yy * FB_WIDTH + (size_t)x];
        for (int xx = 0; xx < w; xx++) {
            row[xx] = color;
        }
    }
}

static uint16_t blend_rgb565(uint16_t dst, uint16_t src, unsigned alpha)
{
    if (alpha >= 255u) {
        return src;
    }
    if (alpha == 0u) {
        return dst;
    }

    unsigned dr = ((dst >> 11) & 0x1fu) * 255u / 31u;
    unsigned dg = ((dst >> 5) & 0x3fu) * 255u / 63u;
    unsigned db = (dst & 0x1fu) * 255u / 31u;
    unsigned sr = ((src >> 11) & 0x1fu) * 255u / 31u;
    unsigned sg = ((src >> 5) & 0x3fu) * 255u / 63u;
    unsigned sb = (src & 0x1fu) * 255u / 31u;
    unsigned inv = 255u - alpha;

    return rgb565((sr * alpha + dr * inv) / 255u,
                  (sg * alpha + dg * inv) / 255u,
                  (sb * alpha + db * inv) / 255u);
}

static void put_pixel_alpha(int x, int y, uint16_t color, unsigned alpha)
{
    if (x < 0 || x >= FB_WIDTH || y < 0 || y >= FB_HEIGHT) {
        return;
    }
    uint16_t *dst = &g_frame[(size_t)y * FB_WIDTH + (size_t)x];
    *dst = blend_rgb565(*dst, color, alpha);
}

static void term_font_cache_clear(void)
{
    for (int i = 0; i < TERM_FONT_CACHE_SLOTS; i++) {
        free(g_font.cache[i].buffer);
        memset(&g_font.cache[i], 0, sizeof(g_font.cache[i]));
    }
}

static void term_font_close(void)
{
    term_font_cache_clear();
    if (g_font.face && g_font.done_face) {
        g_font.done_face(g_font.face);
    }
    if (g_font.library && g_font.done_free_type) {
        g_font.done_free_type(g_font.library);
    }
    if (g_font.handle) {
        dlclose(g_font.handle);
    }
    memset(&g_font, 0, sizeof(g_font));
    g_font.attempted = 1;
}

static int term_font_bind_symbols(void)
{
    g_font.init_free_type =
        (term_ft_init_free_type_fn)dlsym(g_font.handle, "FT_Init_FreeType");
    g_font.new_face =
        (term_ft_new_face_fn)dlsym(g_font.handle, "FT_New_Face");
    g_font.done_face =
        (term_ft_done_face_fn)dlsym(g_font.handle, "FT_Done_Face");
    g_font.done_free_type =
        (term_ft_done_free_type_fn)dlsym(g_font.handle, "FT_Done_FreeType");
    g_font.select_charmap =
        (term_ft_select_charmap_fn)dlsym(g_font.handle, "FT_Select_Charmap");
    g_font.set_pixel_sizes =
        (term_ft_set_pixel_sizes_fn)dlsym(g_font.handle, "FT_Set_Pixel_Sizes");
    g_font.load_char =
        (term_ft_load_char_fn)dlsym(g_font.handle, "FT_Load_Char");
    return g_font.init_free_type && g_font.new_face && g_font.done_face &&
           g_font.done_free_type && g_font.set_pixel_sizes &&
           g_font.load_char;
}

static int term_font_open(void)
{
    static const char *library_paths[] = {
        R36SX_CUBEGM_DIR "/lib/libfreetype.so.6",
        R36SX_CUBEGM_DIR "/usr/lib/libfreetype.so.6",
        R36SX_CUBEGM_DIR "/lib/libfreetype.so",
        "libfreetype.so.6"
    };
    static const char *font_paths[] = {
        R36SX_DEFAULT_MONO_FONT_PATH,
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/NotoSansMono-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/DejaVuSansMono.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/LiberationMono-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/Hack-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/SourceCodePro-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/RobotoMono-Regular.ttf",
        R36SX_CUBEGM_DIR "/Arial_en.ttf",
        R36SX_CUBEGM_DIR "/font.ttf",
        R36SX_CUBEGM_DIR "/Tahoma.ttf"
    };
    const char *env_font = getenv("R36SX_SHELL_FONT");

    if (g_font.active) {
        return 0;
    }
    if (g_font.attempted) {
        return -1;
    }

    memset(&g_font, 0, sizeof(g_font));
    g_font.attempted = 1;
    for (size_t i = 0; i < ARRAY_COUNT(library_paths); i++) {
        g_font.handle = dlopen(library_paths[i], RTLD_NOW);
        if (g_font.handle) {
            break;
        }
    }
    if (!g_font.handle || !term_font_bind_symbols()) {
        term_font_close();
        return -1;
    }
    if (g_font.init_free_type(&g_font.library) != 0) {
        term_font_close();
        return -1;
    }
    if (env_font && access(env_font, R_OK) == 0 &&
        g_font.new_face(g_font.library, env_font, 0, &g_font.face) != 0) {
        g_font.face = NULL;
    }
    for (size_t i = 0; !g_font.face && i < ARRAY_COUNT(font_paths); i++) {
        if (access(font_paths[i], R_OK) != 0) {
            continue;
        }
        if (g_font.new_face(g_font.library, font_paths[i], 0,
                            &g_font.face) == 0) {
            break;
        }
    }
    if (!g_font.face) {
        term_font_close();
        return -1;
    }
    if (g_font.select_charmap) {
        g_font.select_charmap(g_font.face, FT_ENCODING_UNICODE);
    }

    g_font.active = 1;
    return 0;
}

static struct term_glyph_cache_entry *term_font_cache_lookup(uint32_t codepoint)
{
    for (int i = 0; i < TERM_FONT_CACHE_SLOTS; i++) {
        if (g_font.cache[i].valid &&
            g_font.cache[i].codepoint == codepoint) {
            g_font.cache[i].age = ++g_font.cache_age;
            return &g_font.cache[i];
        }
    }
    return NULL;
}

static struct term_glyph_cache_entry *term_font_cache_alloc_slot(void)
{
    int slot = 0;
    uint32_t oldest = UINT32_MAX;

    for (int i = 0; i < TERM_FONT_CACHE_SLOTS; i++) {
        if (!g_font.cache[i].valid) {
            slot = i;
            break;
        }
        if (g_font.cache[i].age < oldest) {
            oldest = g_font.cache[i].age;
            slot = i;
        }
    }

    free(g_font.cache[slot].buffer);
    memset(&g_font.cache[slot], 0, sizeof(g_font.cache[slot]));
    return &g_font.cache[slot];
}

static struct term_glyph_cache_entry *term_font_load_glyph(uint32_t codepoint)
{
    struct term_glyph_cache_entry *entry =
        term_font_cache_lookup(codepoint);
    if (entry) {
        return entry;
    }
    if (term_font_open() != 0 || !g_font.face) {
        return NULL;
    }
    if (g_font.set_pixel_sizes(g_font.face, 0, TERM_FONT_PX) != 0) {
        return NULL;
    }
    if (g_font.load_char(g_font.face, (FT_ULong)codepoint,
                         FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT) != 0) {
        if (codepoint != '?') {
            return term_font_load_glyph('?');
        }
        return NULL;
    }

    FT_GlyphSlot slot_ft = g_font.face->glyph;
    FT_Bitmap *bitmap = &slot_ft->bitmap;
    if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) {
        return NULL;
    }

    entry = term_font_cache_alloc_slot();
    entry->codepoint = codepoint;
    entry->bitmap_left = slot_ft->bitmap_left;
    entry->bitmap_top = slot_ft->bitmap_top;
    entry->width = (int)bitmap->width;
    entry->rows = (int)bitmap->rows;
    entry->pitch = entry->width;
    entry->advance = (int)(slot_ft->advance.x >> 6);
    if (entry->advance <= 0) {
        entry->advance = FONT_W;
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
    entry->age = ++g_font.cache_age;
    return entry;
}

static void term_reset_attrs(void)
{
    g_term.fg = 15;
    g_term.bg = 0;
    g_term.bright = 0;
    g_term.reverse = 0;
}

static void term_clear_row(int row)
{
    if (row < 0 || row >= TERM_MAX_ROWS) {
        return;
    }
    for (int col = 0; col < TERM_COLS; col++) {
        g_term.cells[row][col].ch = ' ';
        g_term.cells[row][col].fg = g_term.fg;
        g_term.cells[row][col].bg = g_term.bg;
        g_term.cells[row][col].flags = 0;
    }
}

static void term_clear(void)
{
    for (int row = 0; row < TERM_MAX_ROWS; row++) {
        term_clear_row(row);
    }
    g_term.cursor_x = 0;
    g_term.cursor_y = 0;
}

static void term_init(void)
{
    memset(&g_term, 0, sizeof(g_term));
    g_term.rows = TERM_MAX_ROWS;
    term_reset_attrs();
    term_clear();
}

static void term_set_rows(int rows)
{
    if (rows < 1) {
        rows = 1;
    }
    if (rows > TERM_MAX_ROWS) {
        rows = TERM_MAX_ROWS;
    }
    if (g_term.rows == rows) {
        return;
    }
    if (rows > g_term.rows) {
        for (int row = g_term.rows; row < rows; row++) {
            term_clear_row(row);
        }
    }
    g_term.rows = rows;
    if (g_term.cursor_y >= g_term.rows) {
        g_term.cursor_y = g_term.rows - 1;
    }
}

static void term_scroll(void)
{
    memmove(g_term.cells[0], g_term.cells[1],
            (size_t)(g_term.rows - 1) * sizeof(g_term.cells[0]));
    term_clear_row(g_term.rows - 1);
    if (g_term.cursor_y > 0) {
        g_term.cursor_y--;
    }
}

static void term_newline(void)
{
    g_term.cursor_x = 0;
    g_term.cursor_y++;
    if (g_term.cursor_y >= g_term.rows) {
        term_scroll();
        g_term.cursor_y = g_term.rows - 1;
    }
}

static void term_put_printable(uint32_t ch)
{
    if (g_term.cursor_x >= TERM_COLS) {
        term_newline();
    }
    if (g_term.cursor_y < 0 || g_term.cursor_y >= g_term.rows) {
        return;
    }
    struct cell *cell = &g_term.cells[g_term.cursor_y][g_term.cursor_x];
    cell->ch = ch >= 32 ? ch : '?';
    cell->fg = g_term.fg;
    cell->bg = g_term.bg;
    cell->flags = g_term.reverse ? 1u : 0u;
    g_term.cursor_x++;
}

static void term_reset_utf8(void)
{
    g_term.utf8_remaining = 0;
    g_term.utf8_codepoint = 0;
}

static void term_process_printable_byte(uint8_t ch)
{
    if (ch < 0x80u) {
        term_reset_utf8();
        term_put_printable(ch);
        return;
    }

    if (g_term.utf8_remaining != 0) {
        if ((ch & 0xc0u) != 0x80u) {
            term_reset_utf8();
            term_put_printable('?');
            term_process_printable_byte(ch);
            return;
        }
        g_term.utf8_codepoint =
            (g_term.utf8_codepoint << 6) | (uint32_t)(ch & 0x3fu);
        g_term.utf8_remaining--;
        if (g_term.utf8_remaining == 0) {
            uint32_t cp = g_term.utf8_codepoint;
            term_reset_utf8();
            term_put_printable(cp);
        }
        return;
    }

    if ((ch & 0xe0u) == 0xc0u) {
        g_term.utf8_codepoint = (uint32_t)(ch & 0x1fu);
        g_term.utf8_remaining = 1;
    } else if ((ch & 0xf0u) == 0xe0u) {
        g_term.utf8_codepoint = (uint32_t)(ch & 0x0fu);
        g_term.utf8_remaining = 2;
    } else if ((ch & 0xf8u) == 0xf0u) {
        g_term.utf8_codepoint = (uint32_t)(ch & 0x07u);
        g_term.utf8_remaining = 3;
    } else {
        term_put_printable('?');
    }
}

static int parse_csi_params(int *params, int max_params, int *private_mode)
{
    int count = 0;
    int value = -1;
    *private_mode = 0;

    for (int i = 0; i < g_term.csi_len; i++) {
        char c = g_term.csi[i];
        if (c == '?') {
            *private_mode = 1;
            continue;
        }
        if (c >= '0' && c <= '9') {
            if (value < 0) {
                value = 0;
            }
            value = value * 10 + (c - '0');
            continue;
        }
        if (c == ';') {
            if (count < max_params) {
                params[count++] = value < 0 ? 0 : value;
            }
            value = -1;
        }
    }
    if (count < max_params) {
        params[count++] = value < 0 ? 0 : value;
    }
    return count;
}

static void term_clear_from_cursor(void)
{
    if (g_term.cursor_y < 0 || g_term.cursor_y >= g_term.rows) {
        return;
    }
    for (int col = g_term.cursor_x; col < TERM_COLS; col++) {
        g_term.cells[g_term.cursor_y][col].ch = ' ';
        g_term.cells[g_term.cursor_y][col].fg = g_term.fg;
        g_term.cells[g_term.cursor_y][col].bg = g_term.bg;
        g_term.cells[g_term.cursor_y][col].flags = 0;
    }
    for (int row = g_term.cursor_y + 1; row < g_term.rows; row++) {
        term_clear_row(row);
    }
}

static void term_apply_sgr(const int *params, int count)
{
    if (count <= 0) {
        term_reset_attrs();
        return;
    }
    for (int i = 0; i < count; i++) {
        int p = params[i];
        if (p == 0) {
            term_reset_attrs();
        } else if (p == 1) {
            g_term.bright = 1;
            if (g_term.fg < 8) {
                g_term.fg += 8;
            }
        } else if (p == 7) {
            g_term.reverse = 1;
        } else if (p == 22) {
            g_term.bright = 0;
            if (g_term.fg >= 8) {
                g_term.fg -= 8;
            }
        } else if (p == 27) {
            g_term.reverse = 0;
        } else if (p >= 30 && p <= 37) {
            g_term.fg = (uint8_t)(p - 30 + (g_term.bright ? 8 : 0));
        } else if (p >= 40 && p <= 47) {
            g_term.bg = (uint8_t)(p - 40);
        } else if (p >= 90 && p <= 97) {
            g_term.fg = (uint8_t)(p - 90 + 8);
        } else if (p >= 100 && p <= 107) {
            g_term.bg = (uint8_t)(p - 100 + 8);
        }
    }
}

static void term_handle_csi(char final)
{
    int params[6] = {0, 0, 0, 0, 0, 0};
    int private_mode = 0;
    int count = parse_csi_params(params, (int)ARRAY_COUNT(params), &private_mode);
    int n = params[0] <= 0 ? 1 : params[0];
    (void)private_mode;

    switch (final) {
    case 'A':
        g_term.cursor_y -= n;
        if (g_term.cursor_y < 0) {
            g_term.cursor_y = 0;
        }
        break;
    case 'B':
        g_term.cursor_y += n;
        if (g_term.cursor_y >= g_term.rows) {
            g_term.cursor_y = g_term.rows - 1;
        }
        break;
    case 'C':
        g_term.cursor_x += n;
        if (g_term.cursor_x >= TERM_COLS) {
            g_term.cursor_x = TERM_COLS - 1;
        }
        break;
    case 'D':
        g_term.cursor_x -= n;
        if (g_term.cursor_x < 0) {
            g_term.cursor_x = 0;
        }
        break;
    case 'G':
        g_term.cursor_x = n - 1;
        if (g_term.cursor_x < 0) {
            g_term.cursor_x = 0;
        }
        if (g_term.cursor_x >= TERM_COLS) {
            g_term.cursor_x = TERM_COLS - 1;
        }
        break;
    case 'H':
    case 'f': {
        int row = count >= 1 && params[0] > 0 ? params[0] - 1 : 0;
        int col = count >= 2 && params[1] > 0 ? params[1] - 1 : 0;
        if (row < 0) row = 0;
        if (col < 0) col = 0;
        if (row >= g_term.rows) row = g_term.rows - 1;
        if (col >= TERM_COLS) col = TERM_COLS - 1;
        g_term.cursor_y = row;
        g_term.cursor_x = col;
        break;
    }
    case 'J':
        if (params[0] == 2) {
            term_clear();
        } else {
            term_clear_from_cursor();
        }
        break;
    case 'K':
        for (int col = params[0] == 1 ? 0 : g_term.cursor_x;
             col < TERM_COLS; col++) {
            g_term.cells[g_term.cursor_y][col].ch = ' ';
            g_term.cells[g_term.cursor_y][col].fg = g_term.fg;
            g_term.cells[g_term.cursor_y][col].bg = g_term.bg;
            g_term.cells[g_term.cursor_y][col].flags = 0;
        }
        break;
    case 'm':
        term_apply_sgr(params, count);
        break;
    default:
        break;
    }
}

static void term_process_byte(uint8_t ch)
{
    if (g_term.esc_state == 1) {
        term_reset_utf8();
        if (ch == '[') {
            g_term.esc_state = 2;
            g_term.csi_len = 0;
        } else if (ch == ']') {
            g_term.esc_state = 3;
        } else {
            g_term.esc_state = 0;
        }
        return;
    }
    if (g_term.esc_state == 2) {
        term_reset_utf8();
        if ((ch >= '0' && ch <= '9') || ch == ';' || ch == '?') {
            if (g_term.csi_len + 1 < CSI_BUF_LEN) {
                g_term.csi[g_term.csi_len++] = (char)ch;
                g_term.csi[g_term.csi_len] = '\0';
            }
            return;
        }
        term_handle_csi((char)ch);
        g_term.esc_state = 0;
        return;
    }
    if (g_term.esc_state == 3) {
        term_reset_utf8();
        if (ch == 7) {
            g_term.esc_state = 0;
        }
        return;
    }

    switch (ch) {
    case 7:
        term_reset_utf8();
        break;
    case 8:
        term_reset_utf8();
        if (g_term.cursor_x > 0) {
            g_term.cursor_x--;
        }
        break;
    case 9:
        term_reset_utf8();
        do {
            term_put_printable(' ');
        } while ((g_term.cursor_x & 7) != 0);
        break;
    case 10:
        term_reset_utf8();
        term_newline();
        break;
    case 13:
        term_reset_utf8();
        g_term.cursor_x = 0;
        break;
    case 27:
        term_reset_utf8();
        g_term.esc_state = 1;
        break;
    default:
        if (ch >= 32) {
            term_process_printable_byte(ch);
        }
        break;
    }
}

static void term_write_text(const char *text)
{
    while (text && *text) {
        term_process_byte((uint8_t)*text++);
    }
}

static void shell_put_le16(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)((value >> 8) & 0xffu);
}

static void shell_put_le32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)((value >> 8) & 0xffu);
    dst[2] = (uint8_t)((value >> 16) & 0xffu);
    dst[3] = (uint8_t)((value >> 24) & 0xffu);
}

static void shell_rgb565_to_bgr24(uint8_t *dst, const uint16_t *src,
                                  int pixels)
{
    for (int i = 0; i < pixels; i++) {
        uint16_t px = src[i];
        unsigned r5 = (px >> 11) & 0x1fu;
        unsigned g6 = (px >> 5) & 0x3fu;
        unsigned b5 = px & 0x1fu;
        dst[(size_t)i * 3u + 0u] = (uint8_t)((b5 * 255u) / 31u);
        dst[(size_t)i * 3u + 1u] = (uint8_t)((g6 * 255u) / 63u);
        dst[(size_t)i * 3u + 2u] = (uint8_t)((r5 * 255u) / 31u);
    }
}

static int shell_write_bmp24(const char *path, const uint16_t *pixels,
                             int width, int height)
{
    uint8_t header[54];
    FILE *fp;
    uint8_t *row;
    size_t row_bytes;
    size_t padded_row_bytes;
    size_t pixel_bytes;
    int ok;

    if (!path || !pixels || width <= 0 || height <= 0) {
        return -1;
    }

    row_bytes = (size_t)width * 3u;
    padded_row_bytes = (row_bytes + 3u) & ~(size_t)3u;
    pixel_bytes = padded_row_bytes * (size_t)height;
    if (row_bytes / 3u != (size_t)width ||
        padded_row_bytes < row_bytes ||
        pixel_bytes / padded_row_bytes != (size_t)height ||
        pixel_bytes + sizeof(header) > UINT32_MAX) {
        return -1;
    }

    fp = fopen(path, "wb");
    if (!fp) {
        return -1;
    }

    memset(header, 0, sizeof(header));
    header[0] = 'B';
    header[1] = 'M';
    shell_put_le32(&header[2], (uint32_t)(sizeof(header) + pixel_bytes));
    shell_put_le32(&header[10], (uint32_t)sizeof(header));
    shell_put_le32(&header[14], 40u);
    shell_put_le32(&header[18], (uint32_t)width);
    shell_put_le32(&header[22], (uint32_t)height);
    shell_put_le16(&header[26], 1u);
    shell_put_le16(&header[28], 24u);
    shell_put_le32(&header[34], (uint32_t)pixel_bytes);

    row = (uint8_t *)calloc(1u, padded_row_bytes);
    if (!row) {
        fclose(fp);
        return -1;
    }

    ok = fwrite(header, 1u, sizeof(header), fp) == sizeof(header);
    for (int y = height - 1; ok && y >= 0; y--) {
        memset(row, 0, padded_row_bytes);
        shell_rgb565_to_bgr24(row,
                              pixels + (size_t)y * (size_t)width,
                              width);
        ok = fwrite(row, 1u, padded_row_bytes, fp) == padded_row_bytes;
    }

    free(row);
    if (fclose(fp) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}

static int shell_save_screenshot_to_dir(const char *dir,
                                        const uint16_t *pixels,
                                        char *saved_path,
                                        size_t saved_path_size)
{
    time_t now;
    struct tm tm_now;
    char stamp[32];
    char path[SHELL_SCREENSHOT_PATH_MAX];
    uint32_t seq;
    int written;

    if (!dir || !pixels) {
        return -1;
    }

    (void)mkdir(dir, 0755);
    now = time(NULL);
    if (now != (time_t)-1 && localtime_r(&now, &tm_now)) {
        strftime(stamp, sizeof(stamp), "%Y%m%d_%H%M%S", &tm_now);
    } else {
        snprintf(stamp, sizeof(stamp), "unknown_time");
    }

    seq = g_screenshot_counter++;
    written = snprintf(path, sizeof(path), "%s/Shell_%s_%03u.bmp",
                       dir, stamp, (unsigned)(seq % 1000u));
    if (written < 0 || (size_t)written >= sizeof(path)) {
        return -1;
    }

    if (shell_write_bmp24(path, pixels, FB_WIDTH, FB_HEIGHT) != 0) {
        return -1;
    }
    if (saved_path && saved_path_size > 0) {
        snprintf(saved_path, saved_path_size, "%s", path);
    }
    return 0;
}

static int shell_save_screenshot(const uint16_t *pixels, char *saved_path,
                                 size_t saved_path_size)
{
    if (shell_save_screenshot_to_dir(SHELL_SCREENSHOT_DIR, pixels,
                                     saved_path, saved_path_size) == 0 ||
        shell_save_screenshot_to_dir(SHELL_SCREENSHOT_LOCAL_DIR, pixels,
                                     saved_path, saved_path_size) == 0) {
        return 1;
    }
    return 0;
}

static void shell_request_screenshot(void)
{
    g_screenshot_requested = 1;
}

static void shell_finish_screenshot(void)
{
    char path[SHELL_SCREENSHOT_PATH_MAX] = { 0 };
    int saved = shell_save_screenshot(g_frame, path, sizeof(path));

    g_screenshot_requested = 0;
    if (saved) {
        term_write_text("\r\n[screenshot saved: ");
        term_write_text(path);
        term_write_text("]\r\n");
    } else {
        term_write_text("\r\n[screenshot failed]\r\n");
    }
}

static int display_open(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    memset(&g_driver, 0, sizeof(g_driver));
    for (size_t i = 0; i < ARRAY_COUNT(paths); i++) {
        g_driver.handle = dlopen(paths[i], RTLD_NOW);
        if (g_driver.handle) {
            break;
        }
    }
    if (!g_driver.handle) {
        fprintf(stderr, "Shell: cannot open driver.so: %s\n", dlerror());
        return -1;
    }

    g_driver.setting = (video_driver_setting_fn)dlsym(g_driver.handle, "video_driver_setting");
    g_driver.init = (video_drivers_init_fn)dlsym(g_driver.handle, "video_drivers_init");
    g_driver.disp_frame = (video_driver_disp_frame_fn)dlsym(g_driver.handle, "video_driver_disp_frame");
    g_driver.deinit = (video_driver_deinit_fn)dlsym(g_driver.handle, "video_driver_deinit");
    g_driver.cube_ioctl = (cube_ioctl_fn)dlsym(g_driver.handle, "cube_ioctl");
    if (!g_driver.setting || !g_driver.init || !g_driver.disp_frame ||
        !g_driver.deinit || !g_driver.cube_ioctl) {
        fprintf(stderr, "Shell: driver.so symbols missing\n");
        return -1;
    }

    int cfg[5] = {
        R36SX_DRIVER_SETTING_0,
        R36SX_DRIVER_SETTING_1,
        R36SX_DRIVER_SETTING_2,
        R36SX_DRIVER_SETTING_WIDTH,
        R36SX_DRIVER_SETTING_HEIGHT
    };
    g_driver.setting(cfg);
    if (g_driver.init() < 0) {
        fprintf(stderr, "Shell: video_drivers_init failed\n");
        return -1;
    }
    g_driver.active = 1;
    return 0;
}

static void display_close(void)
{
    if (g_driver.active && g_driver.deinit) {
        g_driver.deinit();
    }
    if (g_driver.handle) {
        dlclose(g_driver.handle);
    }
    memset(&g_driver, 0, sizeof(g_driver));
}

static void present_frame(void)
{
    if (g_driver.active && g_driver.disp_frame) {
        g_driver.disp_frame(g_frame, FB_WIDTH, FB_HEIGHT,
                            R36SX_RGB565_FRAME_STRIDE);
    }
}

static uint32_t input_buttons(void)
{
    uint32_t state = 0;

    if (!g_driver.cube_ioctl) {
        return 0;
    }
    if (g_driver.cube_key_addr == 0) {
        uint32_t addr = 0;
        g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_driver.cube_key_addr = addr;
        g_driver.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
    }
    if (!g_driver.cube_key_mem) {
        return 0;
    }
    g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);
    (void)state;
    return g_driver.cube_key_mem[0] | g_driver.cube_key_mem[1];
}

static void term_write_errno_message(const char *prefix, int error_number)
{
    term_write_text(prefix);
    term_write_text(": ");
    term_write_text(strerror(error_number));
    term_write_text("\r\n");
}

static void shell_child_set_env(void)
{
    setenv("TERM", "vt100", 1);
    setenv("PS1", "r36sx:\\w# ", 1);
    unsetenv("LC_ALL");
    setenv("LANG", "C.UTF-8", 1);
    setenv("LC_CTYPE", "C.UTF-8", 1);
    setenv("PATH",
           "/bin:/sbin:/usr/bin:/usr/sbin:/mnt/sdcard/cubegm:/mnt/sdcard/MIPS_NATIVE",
           1);
}

static void pty_prepare_devpts(void)
{
    struct stat st;

    (void)mkdir("/dev", 0755);
    (void)mkdir("/dev/pts", 0755);

    if (stat("/dev/ptmx", &st) != 0 && errno == ENOENT) {
        if (mknod("/dev/ptmx", S_IFCHR | 0666, makedev(5, 2)) != 0 &&
            errno != EEXIST) {
            term_write_errno_message("mknod /dev/ptmx failed", errno);
        }
    }

    if (mount("devpts", "/dev/pts", "devpts", 0, "mode=620") != 0 &&
        errno != EBUSY) {
        term_write_errno_message("mount devpts failed", errno);
    }
}

static void pty_close(void)
{
    int read_fd = g_pty.read_fd;
    int write_fd = g_pty.write_fd;
    int master_fd = g_pty.master_fd;

    if (read_fd >= 0) {
        close(read_fd);
    }
    if (write_fd >= 0 && write_fd != read_fd) {
        close(write_fd);
    }
    if (master_fd >= 0 && master_fd != read_fd && master_fd != write_fd) {
        close(master_fd);
    }
    g_pty.master_fd = -1;
    g_pty.read_fd = -1;
    g_pty.write_fd = -1;
    g_pty.pipe_mode = 0;
    g_pty.pipe_esc_state = 0;
    g_pty.pipe_line_len = 0;
}

static void pty_set_winsize(void)
{
    if (g_pty.master_fd < 0 || g_pty.pipe_mode ||
        g_term.rows == g_last_winsize_rows) {
        return;
    }
    struct winsize ws;
    memset(&ws, 0, sizeof(ws));
    ws.ws_row = (unsigned short)g_term.rows;
    ws.ws_col = TERM_COLS;
    ws.ws_xpixel = FB_WIDTH;
    ws.ws_ypixel = (unsigned short)(g_term.rows * FONT_H);
    ioctl(g_pty.master_fd, TIOCSWINSZ, &ws);
    g_last_winsize_rows = g_term.rows;
}

static int pty_open_master(int *out_master, char **out_slave_name)
{
    int master = posix_openpt(O_RDWR | O_NOCTTY);
    if (master < 0) {
        term_write_errno_message("posix_openpt failed", errno);
        return -1;
    }
    if (grantpt(master) != 0 || unlockpt(master) != 0) {
        term_write_errno_message("grantpt/unlockpt failed", errno);
        close(master);
        return -1;
    }
    char *slave_name = ptsname(master);
    if (!slave_name) {
        term_write_errno_message("ptsname failed", errno);
        close(master);
        return -1;
    }
    *out_master = master;
    *out_slave_name = slave_name;
    return 0;
}

static void pty_write_raw_fd(int fd, const char *bytes, size_t len)
{
    while (fd >= 0 && len > 0) {
        ssize_t n = write(fd, bytes, len);
        if (n > 0) {
            bytes += n;
            len -= (size_t)n;
        } else if (errno != EINTR && errno != EAGAIN) {
            break;
        }
    }
}

static int pipe_spawn_shell(void)
{
    int in_pipe[2] = { -1, -1 };
    int out_pipe[2] = { -1, -1 };

    if (pipe(in_pipe) != 0 || pipe(out_pipe) != 0) {
        term_write_errno_message("pipe fallback failed", errno);
        if (in_pipe[0] >= 0) close(in_pipe[0]);
        if (in_pipe[1] >= 0) close(in_pipe[1]);
        if (out_pipe[0] >= 0) close(out_pipe[0]);
        if (out_pipe[1] >= 0) close(out_pipe[1]);
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        term_write_errno_message("fork failed", errno);
        close(in_pipe[0]);
        close(in_pipe[1]);
        close(out_pipe[0]);
        close(out_pipe[1]);
        return -1;
    }
    if (pid == 0) {
        close(in_pipe[1]);
        close(out_pipe[0]);
        dup2(in_pipe[0], STDIN_FILENO);
        dup2(out_pipe[1], STDOUT_FILENO);
        dup2(out_pipe[1], STDERR_FILENO);
        if (in_pipe[0] > STDERR_FILENO) {
            close(in_pipe[0]);
        }
        if (out_pipe[1] > STDERR_FILENO) {
            close(out_pipe[1]);
        }
        shell_child_set_env();
        execl("/bin/sh", "sh", "-i", (char *)NULL);
        _exit(127);
    }

    close(in_pipe[0]);
    close(out_pipe[1]);
    fcntl(out_pipe[0], F_SETFL, fcntl(out_pipe[0], F_GETFL, 0) | O_NONBLOCK);
    fcntl(in_pipe[1], F_SETFL, fcntl(in_pipe[1], F_GETFL, 0) | O_NONBLOCK);

    g_pty.master_fd = -1;
    g_pty.read_fd = out_pipe[0];
    g_pty.write_fd = in_pipe[1];
    g_pty.child_pid = pid;
    g_pty.child_exited = 0;
    g_pty.pipe_mode = 1;
    g_pty.pipe_esc_state = 0;
    g_pty.pipe_line_len = 0;
    term_write_text("[PTY unavailable; using pipe shell fallback]\r\n");
    term_write_text("[line editing is local; job control is unavailable]\r\n");
    return 0;
}

static int pty_spawn_shell(void)
{
    int master = -1;
    char *slave_name = NULL;

    if (pty_open_master(&master, &slave_name) != 0) {
        term_write_text("[trying /dev/pts setup]\r\n");
        pty_prepare_devpts();
        if (pty_open_master(&master, &slave_name) != 0) {
            return pipe_spawn_shell();
        }
        term_write_text("[PTY recovered after /dev/pts setup]\r\n");
    }

    pid_t pid = fork();
    if (pid < 0) {
        term_write_errno_message("fork failed", errno);
        close(master);
        return pipe_spawn_shell();
    }
    if (pid == 0) {
        setsid();
        int slave = open(slave_name, O_RDWR | O_NOCTTY);
        if (slave < 0) {
            _exit(127);
        }
        ioctl(slave, TIOCSCTTY, 0);
        dup2(slave, STDIN_FILENO);
        dup2(slave, STDOUT_FILENO);
        dup2(slave, STDERR_FILENO);
        if (slave > STDERR_FILENO) {
            close(slave);
        }
        close(master);
        shell_child_set_env();
        execl("/bin/sh", "sh", "-i", (char *)NULL);
        _exit(127);
    }

    g_pty.master_fd = master;
    g_pty.read_fd = master;
    g_pty.write_fd = master;
    g_pty.child_pid = pid;
    g_pty.child_exited = 0;
    g_pty.pipe_mode = 0;
    g_pty.pipe_esc_state = 0;
    g_pty.pipe_line_len = 0;
    fcntl(master, F_SETFL, fcntl(master, F_GETFL, 0) | O_NONBLOCK);
    g_last_winsize_rows = 0;
    pty_set_winsize();
    return 0;
}

static void pty_check_child(void)
{
    if (g_pty.child_pid <= 0 || g_pty.child_exited) {
        return;
    }
    int status = 0;
    pid_t rc = waitpid(g_pty.child_pid, &status, WNOHANG);
    if (rc == g_pty.child_pid) {
        g_pty.child_exited = 1;
        pty_close();
        term_write_text("\r\n[shell exited; press START with keyboard hidden to restart]\r\n");
    }
}

static void pty_read_available(void)
{
    if (g_pty.read_fd < 0) {
        return;
    }
    uint8_t buf[PTY_READ_BUF];
    for (;;) {
        ssize_t n = read(g_pty.read_fd, buf, sizeof(buf));
        if (n > 0) {
            for (ssize_t i = 0; i < n; i++) {
                term_process_byte(buf[i]);
            }
            continue;
        }
        if (n == 0 || (n < 0 && errno == EIO)) {
            pty_check_child();
        }
        break;
    }
}

static void pipe_echo_backspace(void)
{
    term_process_byte(8);
    term_process_byte(' ');
    term_process_byte(8);
}

static void pipe_submit_line(void)
{
    if (g_pty.pipe_line_len > 0) {
        pty_write_raw_fd(g_pty.write_fd, g_pty.pipe_line,
                         g_pty.pipe_line_len);
    }
    pty_write_raw_fd(g_pty.write_fd, "\n", 1);
    g_pty.pipe_line_len = 0;
    term_process_byte('\r');
    term_process_byte('\n');
}

static void pipe_write_interactive_bytes(const char *bytes, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)bytes[i];

        if (g_pty.pipe_esc_state == 1) {
            g_pty.pipe_esc_state = (ch == '[') ? 2 : 0;
            continue;
        }
        if (g_pty.pipe_esc_state == 2) {
            if (ch >= 0x40 && ch <= 0x7e) {
                g_pty.pipe_esc_state = 0;
            }
            continue;
        }

        if (ch == 27) {
            g_pty.pipe_esc_state = 1;
            continue;
        }
        if (ch == '\r' || ch == '\n') {
            pipe_submit_line();
            continue;
        }
        if (ch == 8 || ch == 127) {
            if (g_pty.pipe_line_len > 0) {
                g_pty.pipe_line_len--;
                pipe_echo_backspace();
            }
            continue;
        }
        if (ch == 3) {
            g_pty.pipe_line_len = 0;
            term_write_text("^C\r\n");
            continue;
        }
        if (ch == 4 && g_pty.pipe_line_len == 0) {
            close(g_pty.write_fd);
            g_pty.write_fd = -1;
            term_write_text("^D\r\n");
            continue;
        }
        if ((ch >= 32 || ch == '\t') &&
            g_pty.pipe_line_len + 1 < sizeof(g_pty.pipe_line)) {
            g_pty.pipe_line[g_pty.pipe_line_len++] = (char)ch;
            term_process_byte(ch);
        }
    }
}

static void pty_write_bytes(const char *bytes, size_t len)
{
    if (g_pty.write_fd < 0 || !bytes || len == 0) {
        return;
    }
    if (g_pty.pipe_mode) {
        pipe_write_interactive_bytes(bytes, len);
        return;
    }
    pty_write_raw_fd(g_pty.write_fd, bytes, len);
}

static void pty_write_str(const char *s)
{
    pty_write_bytes(s, strlen(s));
}

#define EVDEV_BITS_PER_LONG ((int)(sizeof(unsigned long) * 8))
#define EVDEV_BIT_WORD(nr) ((nr) / EVDEV_BITS_PER_LONG)
#define EVDEV_BIT_MASK(nr) (1ul << ((nr) % EVDEV_BITS_PER_LONG))

static int evdev_test_bit(int bit, const unsigned long *bits, int word_count)
{
    int word = EVDEV_BIT_WORD(bit);
    if (word < 0 || word >= word_count) {
        return 0;
    }
    return (bits[word] & EVDEV_BIT_MASK(bit)) != 0;
}

static void evdev_init(void)
{
    memset(&g_evdev, 0, sizeof(g_evdev));
    for (int i = 0; i < EVDEV_MAX_FDS; i++) {
        g_evdev.fds[i] = -1;
    }
}

static void evdev_close_index(int index)
{
    if (index < 0 || index >= EVDEV_MAX_FDS) {
        return;
    }
    if (g_evdev.fds[index] >= 0) {
        close(g_evdev.fds[index]);
    }
    g_evdev.fds[index] = -1;
    g_evdev.paths[index][0] = '\0';
}

static void evdev_close_all(void)
{
    for (int i = 0; i < EVDEV_MAX_FDS; i++) {
        evdev_close_index(i);
    }
}

static int evdev_path_is_open(const char *path)
{
    for (int i = 0; i < EVDEV_MAX_FDS; i++) {
        if (g_evdev.fds[i] >= 0 &&
            strcmp(g_evdev.paths[i], path) == 0) {
            return 1;
        }
    }
    return 0;
}

static int evdev_is_keyboard_fd(int fd)
{
    unsigned long ev_bits[EVDEV_BIT_WORD(EV_MAX) + 1];
    unsigned long key_bits[EVDEV_BIT_WORD(KEY_MAX) + 1];

    memset(ev_bits, 0, sizeof(ev_bits));
    memset(key_bits, 0, sizeof(key_bits));
    if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0) {
        return 0;
    }
    if (!evdev_test_bit(EV_KEY, ev_bits, ARRAY_COUNT(ev_bits))) {
        return 0;
    }
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits) < 0) {
        return 0;
    }
    return evdev_test_bit(KEY_A, key_bits, ARRAY_COUNT(key_bits)) &&
           evdev_test_bit(KEY_ENTER, key_bits, ARRAY_COUNT(key_bits));
}

static void evdev_try_open(const char *path)
{
    if (evdev_path_is_open(path)) {
        return;
    }

    int slot = -1;
    for (int i = 0; i < EVDEV_MAX_FDS; i++) {
        if (g_evdev.fds[i] < 0) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        return;
    }

    int fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        return;
    }
    if (!evdev_is_keyboard_fd(fd)) {
        close(fd);
        return;
    }

    g_evdev.fds[slot] = fd;
    snprintf(g_evdev.paths[slot], sizeof(g_evdev.paths[slot]), "%s", path);
    term_write_text("[usb keyboard: ");
    term_write_text(path);
    term_write_text("]\r\n");
}

static void evdev_scan_keyboards(void)
{
    uint64_t now = monotonic_us();
    if (now < g_evdev.next_scan_us) {
        return;
    }
    g_evdev.next_scan_us = now + EVDEV_RESCAN_USEC;

    for (int i = 0; i < 32; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        evdev_try_open(path);
    }
}

static char evdev_shifted_digit(int code)
{
    static const char normal[] = "1234567890";
    static const char shifted[] = "!@#$%^&*()";
    if (code >= KEY_1 && code <= KEY_9) {
        return g_evdev.shift ? shifted[code - KEY_1] : normal[code - KEY_1];
    }
    if (code == KEY_0) {
        return g_evdev.shift ? ')' : '0';
    }
    return 0;
}

static char evdev_key_to_char(int code)
{
    char digit = evdev_shifted_digit(code);
    if (digit) {
        return digit;
    }
    if (code >= KEY_A && code <= KEY_Z) {
        char base = (char)('a' + code - KEY_A);
        if (g_evdev.ctrl) {
            return (char)(base - 'a' + 1);
        }
        if (g_evdev.shift ^ g_evdev.caps_lock) {
            base = (char)(base - 'a' + 'A');
        }
        return base;
    }
    if (code >= KEY_KP0 && code <= KEY_KP9) {
        return (char)('0' + code - KEY_KP0);
    }

    switch (code) {
    case KEY_SPACE: return ' ';
    case KEY_GRAVE: return g_evdev.shift ? '~' : '`';
    case KEY_MINUS: return g_evdev.shift ? '_' : '-';
    case KEY_EQUAL: return g_evdev.shift ? '+' : '=';
    case KEY_LEFTBRACE: return g_evdev.shift ? '{' : '[';
    case KEY_RIGHTBRACE: return g_evdev.shift ? '}' : ']';
    case KEY_BACKSLASH: return g_evdev.shift ? '|' : '\\';
    case KEY_SEMICOLON: return g_evdev.shift ? ':' : ';';
    case KEY_APOSTROPHE: return g_evdev.shift ? '"' : '\'';
    case KEY_COMMA: return g_evdev.shift ? '<' : ',';
    case KEY_DOT: return g_evdev.shift ? '>' : '.';
    case KEY_SLASH: return g_evdev.shift ? '?' : '/';
    case KEY_KPASTERISK: return '*';
    case KEY_KPMINUS: return '-';
    case KEY_KPPLUS: return '+';
    case KEY_KPDOT: return '.';
    case KEY_KPSLASH: return '/';
    default: return 0;
    }
}

static void evdev_write_str(const char *s)
{
    if (g_evdev.alt) {
        pty_write_str("\033");
    }
    pty_write_str(s);
}

static void evdev_write_char(char c)
{
    if (g_evdev.alt) {
        pty_write_str("\033");
    }
    pty_write_bytes(&c, 1);
}

static void evdev_handle_key(int code, int value)
{
    int is_down = value != 0;
    int is_press_or_repeat = value == 1 || value == 2;

    switch (code) {
    case KEY_LEFTSHIFT:
    case KEY_RIGHTSHIFT:
        g_evdev.shift = is_down;
        return;
    case KEY_LEFTCTRL:
    case KEY_RIGHTCTRL:
        g_evdev.ctrl = is_down;
        return;
    case KEY_LEFTALT:
    case KEY_RIGHTALT:
        g_evdev.alt = is_down;
        return;
    case KEY_CAPSLOCK:
        if (value == 1) {
            g_evdev.caps_lock = !g_evdev.caps_lock;
        }
        return;
    default:
        break;
    }

    if (!is_press_or_repeat) {
        return;
    }

    switch (code) {
    case KEY_ENTER:
    case KEY_KPENTER: evdev_write_str("\r"); return;
    case KEY_BACKSPACE: evdev_write_str("\177"); return;
    case KEY_TAB: evdev_write_str("\t"); return;
    case KEY_ESC: evdev_write_str("\033"); return;
    case KEY_LEFT: evdev_write_str("\033[D"); return;
    case KEY_RIGHT: evdev_write_str("\033[C"); return;
    case KEY_UP: evdev_write_str("\033[A"); return;
    case KEY_DOWN: evdev_write_str("\033[B"); return;
    case KEY_HOME: evdev_write_str("\033[H"); return;
    case KEY_END: evdev_write_str("\033[F"); return;
    case KEY_PAGEUP: evdev_write_str("\033[5~"); return;
    case KEY_PAGEDOWN: evdev_write_str("\033[6~"); return;
    case KEY_INSERT: evdev_write_str("\033[2~"); return;
    case KEY_DELETE: evdev_write_str("\033[3~"); return;
    default:
        break;
    }

    if (code >= KEY_F1 && code <= KEY_F12) {
        static const char *fn_seq[] = {
            "\033OP", "\033OQ", "\033OR", "\033OS", "\033[15~", "\033[17~",
            "\033[18~", "\033[19~", "\033[20~", "\033[21~", "\033[23~",
            "\033[24~"
        };
        evdev_write_str(fn_seq[code - KEY_F1]);
        return;
    }

    char c = evdev_key_to_char(code);
    if (c != 0) {
        evdev_write_char(c);
    }
}

static void evdev_poll_keyboards(void)
{
    evdev_scan_keyboards();
    for (int i = 0; i < EVDEV_MAX_FDS; i++) {
        int fd = g_evdev.fds[i];
        if (fd < 0) {
            continue;
        }
        for (;;) {
            struct input_event ev;
            ssize_t n = read(fd, &ev, sizeof(ev));
            if (n == (ssize_t)sizeof(ev)) {
                if (ev.type == EV_KEY) {
                    evdev_handle_key(ev.code, ev.value);
                }
                continue;
            }
            if (n < 0 && (errno == EAGAIN || errno == EINTR)) {
                break;
            }
            if (n == 0 || errno == ENODEV || errno == ENXIO) {
                term_write_text("[usb keyboard removed: ");
                term_write_text(g_evdev.paths[i]);
                term_write_text("]\r\n");
                evdev_close_index(i);
            }
            break;
        }
    }
}

static char shifted_digit(char c)
{
    static const char shifted[] = ")!@#$%^&*(";
    if (c >= '0' && c <= '9') {
        return shifted[c - '0'];
    }
    return c;
}

static char keycode_to_char(uint16_t keycode)
{
    int shift = g_keys.shift;

    if (keycode >= 'A' && keycode <= 'Z') {
        int upper = shift ^ g_keys.caps_lock;
        return (char)(upper ? keycode : (keycode - 'A' + 'a'));
    }
    if (keycode >= '0' && keycode <= '9') {
        return shift ? shifted_digit((char)keycode) : (char)keycode;
    }

    switch (keycode) {
    case R36SX_SCREEN_KEY_SPACE: return ' ';
    case R36SX_SCREEN_KEY_OEM_3: return shift ? '~' : '`';
    case R36SX_SCREEN_KEY_OEM_MINUS: return shift ? '_' : '-';
    case R36SX_SCREEN_KEY_OEM_PLUS: return shift ? '+' : '=';
    case R36SX_SCREEN_KEY_OEM_4: return shift ? '{' : '[';
    case R36SX_SCREEN_KEY_OEM_6: return shift ? '}' : ']';
    case R36SX_SCREEN_KEY_OEM_5: return shift ? '|' : '\\';
    case R36SX_SCREEN_KEY_OEM_1: return shift ? ':' : ';';
    case R36SX_SCREEN_KEY_OEM_7: return shift ? '"' : '\'';
    case R36SX_SCREEN_KEY_OEM_COMMA: return shift ? '<' : ',';
    case R36SX_SCREEN_KEY_OEM_PERIOD: return shift ? '>' : '.';
    case R36SX_SCREEN_KEY_OEM_2: return shift ? '?' : '/';
    default: return 0;
    }
}

static void emit_keycode(uint16_t keycode, int is_down)
{
    switch (keycode) {
    case R36SX_SCREEN_KEY_SHIFT:
    case R36SX_SCREEN_KEY_LSHIFT:
    case R36SX_SCREEN_KEY_RSHIFT:
        g_keys.shift = is_down;
        return;
    case R36SX_SCREEN_KEY_CONTROL:
    case R36SX_SCREEN_KEY_LCONTROL:
    case R36SX_SCREEN_KEY_RCONTROL:
        g_keys.ctrl = is_down;
        return;
    case R36SX_SCREEN_KEY_MENU:
    case R36SX_SCREEN_KEY_LMENU:
    case R36SX_SCREEN_KEY_RMENU:
        g_keys.alt = is_down;
        return;
    case R36SX_SCREEN_KEY_CAPITAL:
        if (is_down) {
            g_keys.caps_lock = !g_keys.caps_lock;
        }
        return;
    default:
        break;
    }

    if (!is_down) {
        return;
    }
    if (g_keys.alt) {
        pty_write_str("\033");
    }
    if (g_keys.ctrl && keycode >= 'A' && keycode <= 'Z') {
        char c = (char)(keycode - 'A' + 1);
        pty_write_bytes(&c, 1);
        return;
    }

    switch (keycode) {
    case R36SX_SCREEN_KEY_RETURN: pty_write_str("\r"); return;
    case R36SX_SCREEN_KEY_BACK: pty_write_str("\177"); return;
    case R36SX_SCREEN_KEY_TAB: pty_write_str("\t"); return;
    case R36SX_SCREEN_KEY_ESCAPE: pty_write_str("\033"); return;
    case R36SX_SCREEN_KEY_LEFT: pty_write_str("\033[D"); return;
    case R36SX_SCREEN_KEY_RIGHT: pty_write_str("\033[C"); return;
    case R36SX_SCREEN_KEY_UP: pty_write_str("\033[A"); return;
    case R36SX_SCREEN_KEY_DOWN: pty_write_str("\033[B"); return;
    case R36SX_SCREEN_KEY_HOME: pty_write_str("\033[H"); return;
    case R36SX_SCREEN_KEY_END: pty_write_str("\033[F"); return;
    case R36SX_SCREEN_KEY_PRIOR: pty_write_str("\033[5~"); return;
    case R36SX_SCREEN_KEY_NEXT: pty_write_str("\033[6~"); return;
    case R36SX_SCREEN_KEY_INSERT: pty_write_str("\033[2~"); return;
    case R36SX_SCREEN_KEY_DELETE: pty_write_str("\033[3~"); return;
    default:
        break;
    }
    if (keycode >= R36SX_SCREEN_KEY_F1 &&
        keycode < R36SX_SCREEN_KEY_F1 + 12) {
        static const char *fn_seq[] = {
            "\033OP", "\033OQ", "\033OR", "\033OS", "\033[15~", "\033[17~",
            "\033[18~", "\033[19~", "\033[20~", "\033[21~", "\033[23~",
            "\033[24~"
        };
        pty_write_str(fn_seq[keycode - R36SX_SCREEN_KEY_F1]);
        return;
    }

    char c = keycode_to_char(keycode);
    if (c != 0) {
        pty_write_bytes(&c, 1);
    }
}

static void osk_emit(void *user, uint16_t keycode, int is_down)
{
    (void)user;
    emit_keycode(keycode, is_down);
}

static void handle_hidden_buttons(uint32_t pressed)
{
    if ((pressed & R36SX_RKGAME_KEY_SELECT) != 0) {
        r36sx_screen_keyboard_set_visible(&g_keyboard, 1);
        return;
    }
    if (g_pty.child_exited && (pressed & R36SX_RKGAME_KEY_START) != 0) {
        if (pty_spawn_shell() == 0) {
            term_write_text("[new shell]\r\n");
        }
        return;
    }
    if ((pressed & R36SX_RKGAME_KEY_UP) != 0) pty_write_str("\033[A");
    if ((pressed & R36SX_RKGAME_KEY_DOWN) != 0) pty_write_str("\033[B");
    if ((pressed & R36SX_RKGAME_KEY_LEFT) != 0) pty_write_str("\033[D");
    if ((pressed & R36SX_RKGAME_KEY_RIGHT) != 0) pty_write_str("\033[C");
    if ((pressed & R36SX_RKGAME_KEY_A) != 0) pty_write_str("\r");
    if ((pressed & R36SX_RKGAME_KEY_B) != 0) pty_write_str("\177");
    if ((pressed & R36SX_RKGAME_KEY_X) != 0) pty_write_str("\t");
    if ((pressed & R36SX_RKGAME_KEY_Y) != 0) pty_write_str(" ");
}

static void draw_bitmap_fallback(int px, int py, uint32_t codepoint,
                                 uint16_t fg)
{
    uint8_t ch = (codepoint > 0 && codepoint < 256u) ?
        (uint8_t)codepoint : (uint8_t)'?';
    const unsigned char *glyph = &font_8x16[(size_t)ch * FONT_H];

    for (int gy = 0; gy < FONT_H; gy++) {
        uint8_t bits = glyph[gy];
        uint16_t *dst = &g_frame[(size_t)(py + gy) * FB_WIDTH + (size_t)px];
        for (int gx = 0; gx < FONT_W; gx++) {
            if ((bits & (uint8_t)(0x80u >> gx)) != 0) {
                dst[gx] = fg;
            }
        }
    }
}

static void draw_freetype_glyph(int px, int py, uint32_t codepoint,
                                uint16_t fg)
{
    struct term_glyph_cache_entry *glyph =
        term_font_load_glyph(codepoint ? codepoint : ' ');
    if (!glyph) {
        draw_bitmap_fallback(px, py, codepoint, fg);
        return;
    }
    if (!glyph->buffer) {
        return;
    }

    int pen_x = px + (FONT_W - glyph->advance) / 2;
    int gx0 = pen_x + glyph->bitmap_left;
    int gy0 = py + TERM_FONT_BASELINE - glyph->bitmap_top;

    for (int row = 0; row < glyph->rows; row++) {
        int y = gy0 + row;
        if (y < py || y >= py + FONT_H) {
            continue;
        }
        for (int col = 0; col < glyph->width; col++) {
            int x = gx0 + col;
            if (x < px || x >= px + FONT_W) {
                continue;
            }
            unsigned alpha =
                glyph->buffer[(size_t)row * (size_t)glyph->pitch +
                              (size_t)col];
            put_pixel_alpha(x, y, fg, alpha);
        }
    }
}

static void draw_cell(int col, int row, const struct cell *cell, int cursor)
{
    uint8_t fg_idx = cell->fg & 0x0f;
    uint8_t bg_idx = cell->bg & 0x0f;
    if ((cell->flags & 1u) != 0 || cursor) {
        uint8_t t = fg_idx;
        fg_idx = bg_idx;
        bg_idx = t;
        if (cursor && fg_idx == bg_idx) {
            fg_idx = 0;
            bg_idx = 15;
        }
    }
    uint16_t fg = g_palette[fg_idx];
    uint16_t bg = g_palette[bg_idx];
    int px = col * FONT_W;
    int py = row * FONT_H;
    fill_rect(px, py, FONT_W, FONT_H, bg);

    draw_freetype_glyph(px, py, cell->ch ? cell->ch : ' ', fg);
}

static void draw_frame(void)
{
    int content_h = r36sx_screen_keyboard_content_height(&g_keyboard, FB_HEIGHT);
    int rows = content_h / FONT_H;
    term_set_rows(rows);
    pty_set_winsize();

    fill_rect(0, 0, FB_WIDTH, FB_HEIGHT, g_palette[0]);
    for (int row = 0; row < g_term.rows; row++) {
        for (int col = 0; col < TERM_COLS; col++) {
            int cursor = (row == g_term.cursor_y && col == g_term.cursor_x &&
                          !g_pty.child_exited);
            draw_cell(col, row, &g_term.cells[row][col], cursor);
        }
    }
    if (r36sx_screen_keyboard_is_visible(&g_keyboard)) {
        r36sx_screen_keyboard_draw(&g_keyboard, g_frame, FB_WIDTH, FB_HEIGHT,
                                   FB_WIDTH);
    }
}

int main(void)
{
    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGPIPE, SIG_IGN);

    term_init();
    evdev_init();
    r36sx_screen_keyboard_init(&g_keyboard);
    r36sx_screen_keyboard_set_visible(&g_keyboard, 1);
    r36sx_screen_keyboard_set_cursor_block(&g_keyboard, 1);
    r36sx_screen_keyboard_set_expanded(&g_keyboard, 0);

    if (display_open() != 0) {
        display_close();
        return 1;
    }
    term_write_text("Shell. SELECT shows keyboard, FN toggles it, FN+UP saves screenshot, FN+X exits.\r\n");
    term_write_text("USB keyboards are accepted from /dev/input/event*.\r\n");
    term_write_text("Redirection works normally, e.g. ls / > /mnt/sdcard/root.txt\r\n\r\n");
    if (pty_spawn_shell() != 0) {
        term_write_text("Could not start /bin/sh.\r\n");
    }

    while (g_running) {
        uint32_t buttons = input_buttons();
        uint32_t pressed = buttons & ~g_prev_buttons;

        if ((buttons & R36SX_RKGAME_KEY_FN) != 0 &&
            (pressed & R36SX_RKGAME_KEY_X) != 0) {
            g_running = 0;
        } else if ((buttons & R36SX_RKGAME_KEY_FN) != 0 &&
                   (pressed & R36SX_RKGAME_KEY_UP) != 0) {
            shell_request_screenshot();
        } else if ((pressed & R36SX_RKGAME_KEY_FN) != 0) {
            r36sx_screen_keyboard_set_visible(
                &g_keyboard, !r36sx_screen_keyboard_is_visible(&g_keyboard));
        } else if (r36sx_screen_keyboard_is_visible(&g_keyboard)) {
            r36sx_screen_keyboard_handle_buttons(
                &g_keyboard,
                pressed & ~R36SX_RKGAME_KEY_FN,
                buttons & ~R36SX_RKGAME_KEY_FN,
                osk_emit,
                NULL);
        } else {
            handle_hidden_buttons(pressed);
        }

        pty_read_available();
        pty_check_child();
        evdev_poll_keyboards();
        draw_frame();
        if (g_screenshot_requested) {
            shell_finish_screenshot();
        }
        present_frame();
        g_prev_buttons = buttons;
        usleep(FRAME_USEC);
    }

    if (g_pty.child_pid > 0 && !g_pty.child_exited) {
        kill(g_pty.child_pid, SIGHUP);
        waitpid(g_pty.child_pid, NULL, 0);
    }
    pty_close();
    evdev_close_all();
    display_close();
    term_font_close();
    return 0;
}
