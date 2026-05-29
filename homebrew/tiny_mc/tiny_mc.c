/*
 * Tiny one-panel file manager / launcher for the R36SX/SF3000-like firmware.
 *
 * This direct-launch build runs as MIPS_NATIVE/tiny_mc/tiny_mc from
 * cubegm/icube_start.sh. It normally uses cubegm/driver.so for display setup,
 * matching rkgame's framebuffer/rotation path. Direct /dev/fb0 rendering
 * remains as fallback.
 */

#define _GNU_SOURCE
#define DEBUG 1
#define USE_ICUBE_HEARTBEAT 0
#define ENABLE_FN_ICUBE_SHORTCUT 1

#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../common/hardware.h"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    MAX_ENTRIES = 512,
    MAX_DIR_STATES = 64,
    MAX_NAME = 256,
    MAX_INPUT_FDS = 16,
    REFRESH_USEC = 50000,
    JS_DEADZONE = 16000,
    HEADER_H = 48,
    FOOTER_H = 40,
    DEFAULT_LIST_ROW_H = 22,
    LIST_TOP_OFFSET = 20,
    LIST_BOTTOM_PAD = 8,
    SCROLLBAR_WIDTH = 5,
    SCROLLBAR_RIGHT_PAD = 8,
    SCROLLBAR_MIN_THUMB = 14,
    DEFAULT_TINY_FT_SMALL_PX = 12,
    DEFAULT_TINY_FT_LARGE_PX = 19,
    TINY_FT_CACHE_SLOTS = 192,
    MAX_TEXT_EXTENSIONS = 128,
    TEXT_VIEWER_MAX_BYTES = 256 * 1024,
    TEXT_VIEWER_MAX_LINES = 4096
};

#define LOG_LINE_MAX 768
#define DEFAULT_TEXT_EXTENSIONS "txt,log,conf"
#define TINY_MC_CONFIG_PATH "/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc.conf"
#define TINY_MC_CONFIG_LOCAL_PATH "tiny_mc.conf"
#define TINY_MC_EXEC_PATH "/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc"

enum button_bits {
    BTN_UP_BIT = 1u << 0,
    BTN_DOWN_BIT = 1u << 1,
    BTN_LEFT_BIT = 1u << 2,
    BTN_RIGHT_BIT = 1u << 3,
    BTN_A_BIT = 1u << 4,
    BTN_B_BIT = 1u << 5,
    BTN_START_BIT = 1u << 6,
    BTN_SELECT_BIT = 1u << 7
#if ENABLE_FN_ICUBE_SHORTCUT
    ,
    BTN_FN_BIT = 1u << 8
#endif
};

struct fb_bitfield_local {
    uint32_t offset;
    uint32_t length;
    uint32_t msb_right;
};

struct fb_fix_screeninfo_local {
    char id[16];
    unsigned long smem_start;
    uint32_t smem_len;
    uint32_t type;
    uint32_t type_aux;
    uint32_t visual;
    uint16_t xpanstep;
    uint16_t ypanstep;
    uint16_t ywrapstep;
    uint32_t line_length;
    unsigned long mmio_start;
    uint32_t mmio_len;
    uint32_t accel;
    uint16_t capabilities;
    uint16_t reserved[2];
};

struct fb_var_screeninfo_local {
    uint32_t xres;
    uint32_t yres;
    uint32_t xres_virtual;
    uint32_t yres_virtual;
    uint32_t xoffset;
    uint32_t yoffset;
    uint32_t bits_per_pixel;
    uint32_t grayscale;
    struct fb_bitfield_local red;
    struct fb_bitfield_local green;
    struct fb_bitfield_local blue;
    struct fb_bitfield_local transp;
    uint32_t nonstd;
    uint32_t activate;
    uint32_t height;
    uint32_t width;
    uint32_t accel_flags;
    uint32_t pixclock;
    uint32_t left_margin;
    uint32_t right_margin;
    uint32_t upper_margin;
    uint32_t lower_margin;
    uint32_t hsync_len;
    uint32_t vsync_len;
    uint32_t sync;
    uint32_t vmode;
    uint32_t rotate;
    uint32_t colorspace;
    uint32_t reserved[4];
};

#define FBIOGET_VSCREENINFO_LOCAL 0x4600
#define FBIOGET_FSCREENINFO_LOCAL 0x4602
#define FBIOBLANK_LOCAL 0x4611

struct js_event_local {
    uint32_t time;
    int16_t value;
    uint8_t type;
    uint8_t number;
};

#define JS_EVENT_BUTTON_LOCAL 0x01
#define JS_EVENT_AXIS_LOCAL 0x02
#define JS_EVENT_INIT_LOCAL 0x80

struct input_event_local {
    int32_t tv_sec;
    int32_t tv_usec;
    uint16_t type;
    uint16_t code;
    int32_t value;
};

#define EV_KEY_LOCAL 0x01
#define EV_ABS_LOCAL 0x03
#define ABS_X_LOCAL 0x00
#define ABS_Y_LOCAL 0x01
#define KEY_ESC_LOCAL 1
#define KEY_ENTER_LOCAL 28
#define KEY_SPACE_LOCAL 57
#define KEY_LEFT_LOCAL 105
#define KEY_RIGHT_LOCAL 106
#define KEY_UP_LOCAL 103
#define KEY_DOWN_LOCAL 108
#define BTN_A_LOCAL 304
#define BTN_B_LOCAL 305
#define BTN_X_LOCAL 307
#define BTN_Y_LOCAL 308
#define BTN_SELECT_LOCAL 314
#define BTN_START_LOCAL 315
#define BTN_DPAD_UP_LOCAL 544
#define BTN_DPAD_DOWN_LOCAL 545
#define BTN_DPAD_LEFT_LOCAL 546
#define BTN_DPAD_RIGHT_LOCAL 547

struct fb_state {
    int fd;
    uint8_t *mem;
    size_t mem_len;
    int width;
    int height;
    int bpp;
    int stride;
};

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);
typedef void (*sound_driver_init_fn)(int, int, int);
typedef int (*sound_driver_playframe_fn)(const int16_t *, int);
typedef void (*sound_driver_flush_fn)(void);
typedef void (*sound_driver_deinit_fn)(void);
typedef FT_Error (*ft_init_free_type_fn)(FT_Library *);
typedef FT_Error (*ft_new_face_fn)(FT_Library, const char *, FT_Long, FT_Face *);
typedef FT_Error (*ft_done_face_fn)(FT_Face);
typedef FT_Error (*ft_done_free_type_fn)(FT_Library);
typedef FT_Error (*ft_select_charmap_fn)(FT_Face, FT_Encoding);
typedef FT_Error (*ft_set_pixel_sizes_fn)(FT_Face, FT_UInt, FT_UInt);
typedef FT_Error (*ft_load_char_fn)(FT_Face, FT_ULong, FT_Int32);

struct driver_display_state {
    void *handle;
    uint16_t *framebuf;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    sound_driver_init_fn sound_init;
    sound_driver_playframe_fn sound_playframe;
    sound_driver_flush_fn sound_flush;
    sound_driver_deinit_fn sound_deinit;
    int active;
    int sound_active;
};

struct ft_glyph_cache_entry {
    uint32_t codepoint;
    int pixel_height;
    int bitmap_left;
    int bitmap_top;
    int width;
    int rows;
    int pitch;
    int advance;
    uint8_t *buffer;
    uint32_t age;
    int valid;
};

struct font_state {
    void *handle;
    FT_Library library;
    FT_Face face;
    ft_init_free_type_fn init_free_type;
    ft_new_face_fn new_face;
    ft_done_face_fn done_face;
    ft_done_free_type_fn done_free_type;
    ft_select_charmap_fn select_charmap;
    ft_set_pixel_sizes_fn set_pixel_sizes;
    ft_load_char_fn load_char;
    struct ft_glyph_cache_entry cache[TINY_FT_CACHE_SLOTS];
    uint32_t cache_age;
    int active;
    char font_path[PATH_MAX];
};

#if USE_ICUBE_HEARTBEAT
struct heartbeat_state {
    int shmid;
    volatile uint32_t *shm;
    uint32_t counter;
    long next_log_ms;
};
#endif

struct input_state {
    int fds[MAX_INPUT_FDS];
    int fd_count;
    uint8_t is_js[MAX_INPUT_FDS];
    int js_axis[8];
    uint8_t js_button[32];
    uint8_t ev_key[768];
    int ev_abs_x;
    int ev_abs_y;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
    uint32_t cube_buttons;
    uint32_t cube_raw0;
    uint32_t cube_raw1;
    uint32_t cube_state;
};

struct entry {
    char name[MAX_NAME];
    int is_dir;
    int is_exec;
    int is_text;
    off_t size;
};

enum {
    TINY_CLICK_AUDIO_RATE = 44100,
    TINY_CLICK_AUDIO_CHANNELS = 2,
    TINY_CLICK_AUDIO_FRAMES =
        TINY_CLICK_AUDIO_RATE / (1000000 / REFRESH_USEC),
    TINY_CLICK_SAMPLES = 520
};

struct click_audio_state {
    int remaining;
    uint32_t phase;
    int16_t frame[TINY_CLICK_AUDIO_FRAMES * TINY_CLICK_AUDIO_CHANNELS];
};

struct dir_state {
    char path[PATH_MAX];
    char selected_name[MAX_NAME];
    int selected;
    int scroll;
};

struct app_config {
    char font_path[PATH_MAX];
    char text_extensions[MAX_TEXT_EXTENSIONS];
    int font_small_px;
    int font_large_px;
    int list_row_h;
};

struct text_viewer_state {
    int active;
    char path[PATH_MAX];
    char title[MAX_NAME];
    char *buffer;
    char **lines;
    size_t byte_count;
    int line_count;
    int scroll;
    int truncated;
};

static struct fb_state g_fb = {
    -1, NULL, 0, R36SX_SCREEN_WIDTH, R36SX_SCREEN_HEIGHT,
    R36SX_RGB565_BITS_PER_PIXEL, R36SX_RGB565_FRAME_STRIDE
};
static struct driver_display_state g_driver;
#if USE_ICUBE_HEARTBEAT
static struct heartbeat_state g_heartbeat = {-1, NULL, 0, 0};
#endif
static struct input_state g_input;
static struct entry g_entries[MAX_ENTRIES];
static struct dir_state g_dir_states[MAX_DIR_STATES];
static int g_dir_state_count;
static int g_dir_state_next;
static int g_entry_count;
static int g_selected;
static int g_scroll;
static char g_cwd[PATH_MAX] = "/mnt/sdcard";
static char g_status[256] = "A/Start runs files or opens text. Right/A enters dirs. Fn starts iCube.";
static uint32_t g_prev_buttons;
static uint32_t g_repeat_buttons;
static long g_next_repeat_ms;
static int g_wait_buttons_release;
static struct click_audio_state g_audio;
static struct font_state g_font;
static struct app_config g_config = {
    R36SX_DEFAULT_MONO_FONT_PATH,
    DEFAULT_TEXT_EXTENSIONS,
    DEFAULT_TINY_FT_SMALL_PX,
    DEFAULT_TINY_FT_LARGE_PX,
    DEFAULT_LIST_ROW_H
};
static struct text_viewer_state g_viewer;
#if ENABLE_FN_ICUBE_SHORTCUT
static int g_fn_shortcut_armed;
#endif
#if DEBUG
static int g_log_fd = -1;
static char g_log_path[PATH_MAX];
#endif

static long now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)tv.tv_sec * 1000L + (long)(tv.tv_usec / 1000);
}

static void set_close_on_exec(int fd)
{
    if (fd >= 0) {
        int flags = fcntl(fd, F_GETFD, 0);
        if (flags >= 0) {
            fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
        }
    }
}

static void log_msg(const char *fmt, ...);

#if DEBUG
static void log_open(void)
{
    static const char *paths[] = {
        "/mnt/sdcard/cubegm/tiny_mc.log",
        "/mnt/sdcard/tiny_mc.log",
        "tiny_mc.log"
    };

    for (size_t i = 0; i < ARRAY_COUNT(paths); i++) {
        int fd = open(paths[i], O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
        if (fd >= 0) {
            g_log_fd = fd;
            set_close_on_exec(g_log_fd);
            snprintf(g_log_path, sizeof(g_log_path), "%s", paths[i]);
            log_msg("log opened: %s", g_log_path);
            break;
        }
    }
}

static void log_close(void)
{
    if (g_log_fd >= 0) {
        close(g_log_fd);
        g_log_fd = -1;
    }
}

static void log_msg(const char *fmt, ...)
{
    char body[LOG_LINE_MAX / 2];
    char line[LOG_LINE_MAX];
    va_list ap;
    long ms;
    int n;

    if (g_log_fd < 0) {
        return;
    }

    va_start(ap, fmt);
    vsnprintf(body, sizeof(body), fmt, ap);
    va_end(ap);

    ms = now_ms();
    n = snprintf(line, sizeof(line), "[%ld.%03ld] %s\n", ms / 1000L, ms % 1000L, body);
    if (n > 0) {
        size_t len = (size_t)n;
        if (len >= sizeof(line)) {
            len = sizeof(line) - 1;
        }
        write(g_log_fd, line, len);
    }
}
#else
static void log_open(void)
{
}

static void log_close(void)
{
}

static void log_msg(const char *fmt, ...)
{
    (void)fmt;
}
#endif

static char *strip_space(char *s)
{
    char *end;

    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') {
        s++;
    }
    end = s + strlen(s);
    while (end > s &&
           (end[-1] == ' ' || end[-1] == '\t' ||
            end[-1] == '\r' || end[-1] == '\n')) {
        *--end = '\0';
    }
    return s;
}

static int parse_bounded_int(const char *text, int min_value, int max_value, int fallback)
{
    char *end = NULL;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);
    if (errno != 0 || end == text) {
        return fallback;
    }
    while (*end == ' ' || *end == '\t') {
        end++;
    }
    if (*end != '\0') {
        return fallback;
    }
    if (value < min_value) {
        value = min_value;
    }
    if (value > max_value) {
        value = max_value;
    }
    return (int)value;
}

static void config_load(void)
{
    static const char *paths[] = {
        TINY_MC_CONFIG_PATH,
        TINY_MC_CONFIG_LOCAL_PATH
    };
    FILE *fp = NULL;
    const char *loaded_path = NULL;
    char line[512];
    int row_h_set = 0;

    for (size_t i = 0; i < ARRAY_COUNT(paths); i++) {
        fp = fopen(paths[i], "r");
        if (fp) {
            loaded_path = paths[i];
            break;
        }
    }
    if (!fp) {
        log_msg("config not found; defaults font=%s text_ext=%s small=%d large=%d row=%d",
                g_config.font_path, g_config.text_extensions,
                g_config.font_small_px, g_config.font_large_px,
                g_config.list_row_h);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        char *hash = strchr(line, '#');
        char *eq;
        char *key;
        char *value;

        if (hash) {
            *hash = '\0';
        }
        key = strip_space(line);
        if (*key == '\0') {
            continue;
        }
        eq = strchr(key, '=');
        if (!eq) {
            continue;
        }
        *eq = '\0';
        value = strip_space(eq + 1);
        key = strip_space(key);

        if (strcmp(key, "font") == 0 || strcmp(key, "font_path") == 0) {
            if (*value != '\0' && strcmp(value, "default") != 0) {
                snprintf(g_config.font_path, sizeof(g_config.font_path), "%s", value);
            } else {
                snprintf(g_config.font_path, sizeof(g_config.font_path), "%s",
                         R36SX_DEFAULT_MONO_FONT_PATH);
            }
        } else if (strcmp(key, "text_extensions") == 0 ||
                   strcmp(key, "viewer_extensions") == 0 ||
                   strcmp(key, "text_ext") == 0) {
            if (*value != '\0' && strcmp(value, "default") != 0) {
                snprintf(g_config.text_extensions,
                         sizeof(g_config.text_extensions), "%s", value);
            } else {
                snprintf(g_config.text_extensions,
                         sizeof(g_config.text_extensions), "%s",
                         DEFAULT_TEXT_EXTENSIONS);
            }
        } else if (strcmp(key, "small_px") == 0 ||
                   strcmp(key, "font_small_px") == 0) {
            g_config.font_small_px =
                parse_bounded_int(value, 6, 32, g_config.font_small_px);
        } else if (strcmp(key, "large_px") == 0 ||
                   strcmp(key, "font_large_px") == 0 ||
                   strcmp(key, "font_size") == 0) {
            g_config.font_large_px =
                parse_bounded_int(value, 8, 48, g_config.font_large_px);
        } else if (strcmp(key, "list_row_h") == 0 ||
                   strcmp(key, "row_h") == 0) {
            g_config.list_row_h =
                parse_bounded_int(value, 10, 64, g_config.list_row_h);
            row_h_set = 1;
        }
    }
    fclose(fp);

    if (!row_h_set && g_config.list_row_h < g_config.font_large_px + 3) {
        g_config.list_row_h = g_config.font_large_px + 3;
    }

    log_msg("config loaded: %s font=%s text_ext=%s small=%d large=%d row=%d",
            loaded_path, g_config.font_path, g_config.text_extensions,
            g_config.font_small_px, g_config.font_large_px,
            g_config.list_row_h);
}

static void audio_driver_open(void)
{
    uint32_t saved_volume = 0;
    int have_saved_volume = 0;

    if (!g_driver.handle || g_driver.sound_active) {
        return;
    }

    g_driver.sound_init = (sound_driver_init_fn)dlsym(g_driver.handle, "sound_driver_init");
    g_driver.sound_playframe =
        (sound_driver_playframe_fn)dlsym(g_driver.handle, "sound_driver_playframe");
    g_driver.sound_flush = (sound_driver_flush_fn)dlsym(g_driver.handle, "sound_driver_flush");
    g_driver.sound_deinit = (sound_driver_deinit_fn)dlsym(g_driver.handle, "sound_driver_deinit");

    log_msg("driver.so sound init=%p playframe=%p flush=%p deinit=%p",
            g_driver.sound_init, g_driver.sound_playframe,
            g_driver.sound_flush, g_driver.sound_deinit);

    if (!g_driver.sound_init || !g_driver.sound_playframe || !g_driver.sound_deinit) {
        log_msg("driver.so sound symbols missing; click audio disabled");
        return;
    }

    if (g_driver.cube_ioctl &&
        g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_VOLUME, &saved_volume, 0, 0) == 0) {
        saved_volume &= 0xffu;
        have_saved_volume = 1;
        log_msg("preserving mixer volume before sound init: %u", saved_volume);
    }

    g_driver.sound_init(0, TINY_CLICK_AUDIO_RATE, TINY_CLICK_AUDIO_CHANNELS);
    g_driver.sound_active = 1;
    if (have_saved_volume) {
        g_driver.cube_ioctl(R36SX_CUBE_IOCTL_SET_VOLUME,
                            (uint32_t *)(uintptr_t)saved_volume, 0, 0);
        log_msg("restored mixer volume after sound init: %u", saved_volume);
    }
    log_msg("click audio initialized through driver.so");
}

static void audio_driver_close(void)
{
    if (!g_driver.sound_active) {
        return;
    }
    if (g_driver.sound_flush) {
        g_driver.sound_flush();
    }
    if (g_driver.sound_deinit) {
        g_driver.sound_deinit();
    }
    g_driver.sound_active = 0;
    g_audio.remaining = 0;
    log_msg("click audio closed");
}

static void audio_click(void)
{
    if (!g_driver.sound_active) {
        return;
    }
    g_audio.remaining = TINY_CLICK_SAMPLES;
    g_audio.phase = 0;
}

static void audio_update(void)
{
    const uint32_t step = (uint32_t)(((uint64_t)2200u << 16) / TINY_CLICK_AUDIO_RATE);
    int rc;

    if (!g_driver.sound_active || !g_driver.sound_playframe || g_audio.remaining <= 0) {
        return;
    }

    memset(g_audio.frame, 0, sizeof(g_audio.frame));
    for (int i = 0; i < TINY_CLICK_AUDIO_FRAMES && g_audio.remaining > 0; i++) {
        int32_t amp = (int32_t)(2600 * g_audio.remaining / TINY_CLICK_SAMPLES);
        int16_t sample;

        if (amp < 120) {
            amp = 120;
        }
        g_audio.phase += step;
        sample = (g_audio.phase & 0x8000u) ? (int16_t)amp : (int16_t)-amp;
        g_audio.frame[i * 2] = sample;
        g_audio.frame[i * 2 + 1] = sample;
        g_audio.remaining--;
    }

    rc = g_driver.sound_playframe(g_audio.frame, TINY_CLICK_AUDIO_FRAMES);
    if (rc != 0) {
        log_msg("sound_driver_playframe rc=%d", rc);
    }
}

#if USE_ICUBE_HEARTBEAT
static void heartbeat_tick(void)
{
    long ms;

    if (!g_heartbeat.shm) {
        return;
    }

    g_heartbeat.counter++;
    g_heartbeat.shm[R36SX_ICUBE_HEARTBEAT_ALIVE_WORD] = 1;
    g_heartbeat.shm[R36SX_ICUBE_HEARTBEAT_COUNTER_WORD] = g_heartbeat.counter;

    ms = now_ms();
    if (ms >= g_heartbeat.next_log_ms) {
        log_msg("icube heartbeat counter=%u", g_heartbeat.counter);
        g_heartbeat.next_log_ms = ms + 5000;
    }
}

static void heartbeat_open(void)
{
    void *mem;

    g_heartbeat.shmid = shmget(R36SX_ICUBE_HEARTBEAT_KEY, R36SX_ICUBE_HEARTBEAT_SIZE,
                               R36SX_ICUBE_HEARTBEAT_CREATE_FLAGS);
    if (g_heartbeat.shmid < 0) {
        log_msg("icube heartbeat shmget failed: %s", strerror(errno));
        return;
    }

    mem = shmat(g_heartbeat.shmid, NULL, 0);
    if (mem == (void *)-1) {
        log_msg("icube heartbeat shmat failed: %s", strerror(errno));
        g_heartbeat.shmid = -1;
        return;
    }

    g_heartbeat.shm = (volatile uint32_t *)mem;
    g_heartbeat.counter = 0;
    g_heartbeat.next_log_ms = now_ms() + 5000;
    heartbeat_tick();
    log_msg("icube heartbeat attached shmid=%d", g_heartbeat.shmid);
}

static void heartbeat_close(void)
{
    if (g_heartbeat.shm) {
        shmdt((void *)g_heartbeat.shm);
        g_heartbeat.shm = NULL;
    }
    g_heartbeat.shmid = -1;
}
#else
static void heartbeat_tick(void)
{
}

static void heartbeat_open(void)
{
    log_msg("direct launch mode");
}

static void heartbeat_close(void)
{
}
#endif

static uint16_t rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static uint32_t rgb888_from_565(uint16_t c)
{
    uint32_t r = (uint32_t)((c >> 11) & 0x1f);
    uint32_t g = (uint32_t)((c >> 5) & 0x3f);
    uint32_t b = (uint32_t)(c & 0x1f);
    r = (r << 3) | (r >> 2);
    g = (g << 2) | (g >> 4);
    b = (b << 3) | (b >> 2);
    return (r << 16) | (g << 8) | b;
}

static void driver_close_display(void)
{
    log_msg("driver display close active=%d handle=%p", g_driver.active, g_driver.handle);
    audio_driver_close();
    if (g_driver.active && g_driver.deinit) {
        g_driver.deinit();
    }
    if (g_driver.handle) {
        dlclose(g_driver.handle);
    }
    free(g_driver.framebuf);
    memset(&g_driver, 0, sizeof(g_driver));
    g_fb.fd = -1;
    g_fb.mem = NULL;
    g_fb.mem_len = 0;
    g_fb.width = R36SX_SCREEN_WIDTH;
    g_fb.height = R36SX_SCREEN_HEIGHT;
    g_fb.bpp = R36SX_RGB565_BITS_PER_PIXEL;
    g_fb.stride = R36SX_RGB565_FRAME_STRIDE;
}

static int driver_open_display(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };
    char last_dlerror[256] = "unknown dlopen error";

    if (g_driver.active) {
        return 0;
    }

    memset(&g_driver, 0, sizeof(g_driver));
    for (size_t i = 0; i < ARRAY_COUNT(paths); i++) {
        log_msg("trying display driver: %s", paths[i]);
        g_driver.handle = dlopen(paths[i], RTLD_NOW);
        if (g_driver.handle) {
            break;
        }
        const char *err = dlerror();
        snprintf(last_dlerror, sizeof(last_dlerror), "%s", err ? err : "unknown dlopen error");
        log_msg("dlopen failed for %s: %s", paths[i], last_dlerror);
    }
    if (!g_driver.handle) {
        snprintf(g_status, sizeof(g_status), "driver.so open failed: %s", last_dlerror);
        log_msg("%s", g_status);
        return -1;
    }
    log_msg("driver.so loaded");

    g_driver.setting = (video_driver_setting_fn)dlsym(g_driver.handle, "video_driver_setting");
    g_driver.init = (video_drivers_init_fn)dlsym(g_driver.handle, "video_drivers_init");
    g_driver.disp_frame = (video_driver_disp_frame_fn)dlsym(g_driver.handle, "video_driver_disp_frame");
    g_driver.deinit = (video_driver_deinit_fn)dlsym(g_driver.handle, "video_driver_deinit");
    g_driver.cube_ioctl = (cube_ioctl_fn)dlsym(g_driver.handle, "cube_ioctl");
    if (!g_driver.setting || !g_driver.init || !g_driver.disp_frame || !g_driver.deinit) {
        snprintf(g_status, sizeof(g_status), "driver.so display symbols missing");
        log_msg("%s setting=%p init=%p frame=%p deinit=%p", g_status,
                g_driver.setting, g_driver.init, g_driver.disp_frame, g_driver.deinit);
        driver_close_display();
        return -1;
    }
    log_msg("driver.so cube_ioctl=%p", g_driver.cube_ioctl);
    audio_driver_open();

    g_fb.width = R36SX_SCREEN_WIDTH;
    g_fb.height = R36SX_SCREEN_HEIGHT;
    g_fb.bpp = R36SX_RGB565_BITS_PER_PIXEL;
    g_fb.stride = R36SX_RGB565_FRAME_STRIDE;
    g_fb.mem_len = (size_t)g_fb.stride * (size_t)g_fb.height;
    g_driver.framebuf = (uint16_t *)calloc((size_t)R36SX_SCREEN_WIDTH * (size_t)R36SX_SCREEN_HEIGHT,
                                           sizeof(uint16_t));
    if (!g_driver.framebuf) {
        snprintf(g_status, sizeof(g_status), "Cannot allocate display buffer");
        driver_close_display();
        return -1;
    }
    g_fb.mem = (uint8_t *)g_driver.framebuf;

    {
        int cfg[5] = {
            R36SX_DRIVER_SETTING_0,
            R36SX_DRIVER_SETTING_1,
            R36SX_DRIVER_SETTING_2,
            R36SX_DRIVER_SETTING_WIDTH,
            R36SX_DRIVER_SETTING_HEIGHT
        };
        log_msg("video_driver_setting cfg={%d,%d,%d,%d,%d}",
                cfg[0], cfg[1], cfg[2], cfg[3], cfg[4]);
        g_driver.setting(cfg);
    }
    int init_rc = g_driver.init();
    log_msg("video_drivers_init returned %d", init_rc);
    if (init_rc < 0) {
        snprintf(g_status, sizeof(g_status), "video_drivers_init failed");
        log_msg("%s", g_status);
        driver_close_display();
        return -1;
    }

    g_driver.active = 1;
    snprintf(g_status, sizeof(g_status), "Display via driver.so %s", R36SX_SCREEN_MODE_TEXT);
    log_msg("%s", g_status);
    return 0;
}

static int fb_open_device(void)
{
    struct fb_fix_screeninfo_local fix;
    struct fb_var_screeninfo_local var;

    memset(&fix, 0, sizeof(fix));
    memset(&var, 0, sizeof(var));

    g_fb.fd = open(R36SX_FB_DEVICE_PATH, O_RDWR | O_CLOEXEC);
    if (g_fb.fd < 0) {
        snprintf(g_status, sizeof(g_status), "Cannot open %s: %s",
                 R36SX_FB_DEVICE_PATH, strerror(errno));
        log_msg("%s", g_status);
        return -1;
    }
    set_close_on_exec(g_fb.fd);

    if (ioctl(g_fb.fd, FBIOGET_FSCREENINFO_LOCAL, &fix) == 0 &&
        ioctl(g_fb.fd, FBIOGET_VSCREENINFO_LOCAL, &var) == 0) {
        g_fb.width = (int)var.xres;
        g_fb.height = (int)var.yres;
        g_fb.bpp = (int)var.bits_per_pixel;
        g_fb.stride = (int)fix.line_length;
        g_fb.mem_len = (size_t)fix.smem_len;
    } else {
        g_fb.width = R36SX_SCREEN_WIDTH;
        g_fb.height = R36SX_SCREEN_HEIGHT;
        g_fb.bpp = R36SX_RGB565_BITS_PER_PIXEL;
        g_fb.stride = g_fb.width * R36SX_RGB565_BYTES_PER_PIXEL;
        g_fb.mem_len = (size_t)(g_fb.stride * g_fb.height);
        log_msg("FBIOGET info failed, using fallback geometry: %s", strerror(errno));
    }

    if (g_fb.mem_len == 0 || g_fb.mem_len > 64u * 1024u * 1024u) {
        g_fb.mem_len = (size_t)(g_fb.stride * g_fb.height);
    }

    ioctl(g_fb.fd, FBIOBLANK_LOCAL, 0);

    g_fb.mem = mmap(NULL, g_fb.mem_len, PROT_READ | PROT_WRITE, MAP_SHARED, g_fb.fd, 0);
    if (g_fb.mem == MAP_FAILED) {
        g_fb.mem = NULL;
        snprintf(g_status, sizeof(g_status), "mmap %s failed: %s",
                 R36SX_FB_DEVICE_PATH, strerror(errno));
        log_msg("%s", g_status);
        close(g_fb.fd);
        g_fb.fd = -1;
        return -1;
    }

    snprintf(g_status, sizeof(g_status), "Display via direct %s fallback", R36SX_FB_DEVICE_PATH);
    log_msg("%s width=%d height=%d bpp=%d stride=%d mem_len=%lu",
            g_status, g_fb.width, g_fb.height, g_fb.bpp, g_fb.stride,
            (unsigned long)g_fb.mem_len);
    return 0;
}

static void fb_close_device(void)
{
    if (g_fb.mem) {
        munmap(g_fb.mem, g_fb.mem_len);
        g_fb.mem = NULL;
    }
    if (g_fb.fd >= 0) {
        close(g_fb.fd);
        g_fb.fd = -1;
    }
}

static int display_open(void)
{
    log_msg("display_open");
    if (driver_open_display() == 0) {
        return 0;
    }
    log_msg("driver display unavailable, trying %s fallback", R36SX_FB_DEVICE_PATH);
    return fb_open_device();
}

static void display_close(void)
{
    if (g_driver.active || g_driver.handle) {
        driver_close_display();
    } else {
        fb_close_device();
    }
}

static int display_reopen_after_child(void)
{
    enum {
        DISPLAY_REOPEN_ATTEMPTS = 5,
        DISPLAY_REOPEN_DELAY_USEC = 200000
    };

    for (int attempt = 1; attempt <= DISPLAY_REOPEN_ATTEMPTS; attempt++) {
        if (display_open() == 0) {
            if (attempt > 1) {
                log_msg("display reopened after child on attempt %d", attempt);
            }
            return 0;
        }
        log_msg("display reopen after child failed on attempt %d: %s",
                attempt, g_status);
        usleep(DISPLAY_REOPEN_DELAY_USEC);
    }
    return -1;
}

static void present_frame(void)
{
    if (g_driver.active && g_driver.disp_frame && g_driver.framebuf) {
        g_driver.disp_frame(g_driver.framebuf, g_fb.width, g_fb.height, g_fb.stride);
    }
}

static void put_pixel(int x, int y, uint16_t color)
{
    if (!g_fb.mem || x < 0 || y < 0 || x >= g_fb.width || y >= g_fb.height) {
        return;
    }

    uint8_t *row = g_fb.mem + (size_t)y * (size_t)g_fb.stride;
    if (g_fb.bpp == 16) {
        ((uint16_t *)row)[x] = color;
    } else if (g_fb.bpp == 32) {
        ((uint32_t *)row)[x] = 0xff000000u | rgb888_from_565(color);
    } else if (g_fb.bpp == 24) {
        uint32_t c = rgb888_from_565(color);
        uint8_t *p = row + (size_t)x * 3u;
        p[0] = (uint8_t)(c & 0xffu);
        p[1] = (uint8_t)((c >> 8) & 0xffu);
        p[2] = (uint8_t)((c >> 16) & 0xffu);
    }
}

static uint16_t get_pixel(int x, int y)
{
    if (!g_fb.mem || x < 0 || y < 0 || x >= g_fb.width || y >= g_fb.height) {
        return 0;
    }

    uint8_t *row = g_fb.mem + (size_t)y * (size_t)g_fb.stride;
    if (g_fb.bpp == 16) {
        return ((uint16_t *)row)[x];
    }
    if (g_fb.bpp == 32) {
        uint32_t c = ((uint32_t *)row)[x];
        return rgb565((c >> 16) & 0xffu, (c >> 8) & 0xffu, c & 0xffu);
    }
    if (g_fb.bpp == 24) {
        uint8_t *p = row + (size_t)x * 3u;
        return rgb565(p[2], p[1], p[0]);
    }
    return 0;
}

static uint16_t blend_rgb565(uint16_t dst, uint16_t src, unsigned alpha)
{
    unsigned inv = 255u - alpha;
    unsigned dr = ((dst >> 11) & 0x1fu) * 255u / 31u;
    unsigned dg = ((dst >> 5) & 0x3fu) * 255u / 63u;
    unsigned db = (dst & 0x1fu) * 255u / 31u;
    unsigned sr = ((src >> 11) & 0x1fu) * 255u / 31u;
    unsigned sg = ((src >> 5) & 0x3fu) * 255u / 63u;
    unsigned sb = (src & 0x1fu) * 255u / 31u;

    return rgb565((sr * alpha + dr * inv) / 255u,
                  (sg * alpha + dg * inv) / 255u,
                  (sb * alpha + db * inv) / 255u);
}

static void put_pixel_alpha(int x, int y, uint16_t color, unsigned alpha)
{
    if (alpha == 0) {
        return;
    }
    if (alpha >= 255u) {
        put_pixel(x, y, color);
    } else {
        put_pixel(x, y, blend_rgb565(get_pixel(x, y), color, alpha));
    }
}

static void fill_rect(int x, int y, int w, int h, uint16_t color)
{
    if (!g_fb.mem || w <= 0 || h <= 0) {
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
    if (x + w > g_fb.width) {
        w = g_fb.width - x;
    }
    if (y + h > g_fb.height) {
        h = g_fb.height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx < x + w; xx++) {
            put_pixel(xx, yy, color);
        }
    }
}

static void font_cache_clear(void)
{
    for (int i = 0; i < TINY_FT_CACHE_SLOTS; i++) {
        free(g_font.cache[i].buffer);
        memset(&g_font.cache[i], 0, sizeof(g_font.cache[i]));
    }
}

static void font_close(void)
{
    font_cache_clear();
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
}

static int font_bind_symbols(void)
{
    g_font.init_free_type = (ft_init_free_type_fn)dlsym(g_font.handle, "FT_Init_FreeType");
    g_font.new_face = (ft_new_face_fn)dlsym(g_font.handle, "FT_New_Face");
    g_font.done_face = (ft_done_face_fn)dlsym(g_font.handle, "FT_Done_Face");
    g_font.done_free_type = (ft_done_free_type_fn)dlsym(g_font.handle, "FT_Done_FreeType");
    g_font.select_charmap = (ft_select_charmap_fn)dlsym(g_font.handle, "FT_Select_Charmap");
    g_font.set_pixel_sizes = (ft_set_pixel_sizes_fn)dlsym(g_font.handle, "FT_Set_Pixel_Sizes");
    g_font.load_char = (ft_load_char_fn)dlsym(g_font.handle, "FT_Load_Char");
    return g_font.init_free_type && g_font.new_face && g_font.done_face &&
           g_font.done_free_type && g_font.set_pixel_sizes && g_font.load_char;
}

static int font_open(void)
{
    static const char *library_paths[] = {
        "/mnt/sdcard/cubegm/lib/libfreetype.so.6",
        "/mnt/sdcard/cubegm/lib/libfreetype.so",
        "libfreetype.so.6"
    };
    static const char *fallback_font_paths[] = {
        R36SX_DEFAULT_MONO_FONT_PATH,
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/LiberationMono-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/DejaVuSansMono.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/NotoSansMono-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/SourceCodePro-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/Hack-Regular.ttf",
        R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/RobotoMono-Regular.ttf",
        "/mnt/sdcard/MIPS_NATIVE/tiny_mc/fonts/JetBrainsMonoNL-Regular.ttf",
        "/mnt/sdcard/MIPS_NATIVE/tiny_mc/JetBrainsMonoNL-Regular.ttf",
        "fonts/JetBrainsMonoNL-Regular.ttf",
        "JetBrainsMonoNL-Regular.ttf",
        "/mnt/sdcard/cubegm/usr/share/directfb-1.7.7/decker.ttf",
        "/mnt/sdcard/cubegm/Tahoma.ttf",
        "/mnt/sdcard/cubegm/Arial_en.ttf"
    };

    if (g_font.active) {
        return 0;
    }

    memset(&g_font, 0, sizeof(g_font));
    for (size_t i = 0; i < ARRAY_COUNT(library_paths); i++) {
        g_font.handle = dlopen(library_paths[i], RTLD_NOW);
        if (g_font.handle) {
            log_msg("FreeType loaded: %s", library_paths[i]);
            break;
        }
        const char *err = dlerror();
        log_msg("FreeType dlopen failed for %s: %s", library_paths[i],
                err ? err : "unknown error");
    }
    if (!g_font.handle || !font_bind_symbols()) {
        log_msg("FreeType unavailable; using bitmap font fallback");
        font_close();
        return -1;
    }
    if (g_font.init_free_type(&g_font.library) != 0) {
        log_msg("FT_Init_FreeType failed; using bitmap font fallback");
        font_close();
        return -1;
    }

    if (g_config.font_path[0] != '\0' && access(g_config.font_path, R_OK) == 0) {
        if (g_font.new_face(g_font.library, g_config.font_path, 0, &g_font.face) == 0) {
            snprintf(g_font.font_path, sizeof(g_font.font_path), "%s", g_config.font_path);
        } else {
            log_msg("FT_New_Face failed for configured font %s", g_config.font_path);
        }
    } else if (g_config.font_path[0] != '\0') {
        log_msg("configured font is not readable: %s", g_config.font_path);
    }

    for (size_t i = 0; !g_font.face && i < ARRAY_COUNT(fallback_font_paths); i++) {
        if (access(fallback_font_paths[i], R_OK) != 0) {
            continue;
        }
        if (g_font.new_face(g_font.library, fallback_font_paths[i], 0, &g_font.face) == 0) {
            snprintf(g_font.font_path, sizeof(g_font.font_path), "%s", fallback_font_paths[i]);
            break;
        }
        log_msg("FT_New_Face failed for %s", fallback_font_paths[i]);
    }
    if (!g_font.face) {
        log_msg("No usable TrueType font found; using bitmap font fallback");
        font_close();
        return -1;
    }

    if (g_font.select_charmap) {
        g_font.select_charmap(g_font.face, FT_ENCODING_UNICODE);
    }
    g_font.active = 1;
    log_msg("FreeType font active: %s", g_font.font_path);
    return 0;
}

static struct ft_glyph_cache_entry *font_cache_lookup(uint32_t codepoint, int pixel_height)
{
    for (int i = 0; i < TINY_FT_CACHE_SLOTS; i++) {
        if (g_font.cache[i].valid &&
            g_font.cache[i].codepoint == codepoint &&
            g_font.cache[i].pixel_height == pixel_height) {
            g_font.cache[i].age = ++g_font.cache_age;
            return &g_font.cache[i];
        }
    }
    return NULL;
}

static struct ft_glyph_cache_entry *font_cache_alloc_slot(void)
{
    int slot = 0;
    uint32_t oldest = UINT32_MAX;

    for (int i = 0; i < TINY_FT_CACHE_SLOTS; i++) {
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

static struct ft_glyph_cache_entry *font_load_glyph(uint32_t codepoint, int pixel_height)
{
    struct ft_glyph_cache_entry *entry = font_cache_lookup(codepoint, pixel_height);
    if (entry) {
        return entry;
    }
    if (!g_font.active || !g_font.face) {
        return NULL;
    }
    if (g_font.set_pixel_sizes(g_font.face, 0, (FT_UInt)pixel_height) != 0) {
        return NULL;
    }
    if (g_font.load_char(g_font.face, (FT_ULong)codepoint,
                         FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT) != 0) {
        if (codepoint != '?') {
            return font_load_glyph('?', pixel_height);
        }
        return NULL;
    }

    FT_GlyphSlot slot = g_font.face->glyph;
    FT_Bitmap *bitmap = &slot->bitmap;
    if (bitmap->pixel_mode != FT_PIXEL_MODE_GRAY) {
        return NULL;
    }

    entry = font_cache_alloc_slot();
    entry->codepoint = codepoint;
    entry->pixel_height = pixel_height;
    entry->bitmap_left = slot->bitmap_left;
    entry->bitmap_top = slot->bitmap_top;
    entry->width = (int)bitmap->width;
    entry->rows = (int)bitmap->rows;
    entry->pitch = entry->width;
    entry->advance = (int)(slot->advance.x >> 6);
    if (entry->advance <= 0) {
        entry->advance = entry->width + 1;
    }

    if (entry->width > 0 && entry->rows > 0) {
        size_t bytes = (size_t)entry->width * (size_t)entry->rows;
        entry->buffer = (uint8_t *)malloc(bytes);
        if (!entry->buffer) {
            memset(entry, 0, sizeof(*entry));
            return NULL;
        }
        for (int row = 0; row < entry->rows; row++) {
            int pitch = bitmap->pitch < 0 ? -bitmap->pitch : bitmap->pitch;
            const uint8_t *src = bitmap->pitch >= 0
                ? bitmap->buffer + (size_t)row * (size_t)pitch
                : bitmap->buffer + (size_t)(entry->rows - 1 - row) * (size_t)pitch;
            memcpy(entry->buffer + (size_t)row * (size_t)entry->pitch,
                   src, (size_t)entry->width);
        }
    }

    entry->age = ++g_font.cache_age;
    entry->valid = 1;
    return entry;
}

static uint32_t utf8_next_codepoint(const char **text)
{
    const unsigned char *p = (const unsigned char *)*text;
    uint32_t cp;

    if (*p < 0x80u) {
        *text = (const char *)(p + 1);
        return *p;
    }
    if ((p[0] & 0xe0u) == 0xc0u && (p[1] & 0xc0u) == 0x80u) {
        cp = ((uint32_t)(p[0] & 0x1fu) << 6) | (uint32_t)(p[1] & 0x3fu);
        *text = (const char *)(p + 2);
        return cp;
    }
    if ((p[0] & 0xf0u) == 0xe0u &&
        (p[1] & 0xc0u) == 0x80u &&
        (p[2] & 0xc0u) == 0x80u) {
        cp = ((uint32_t)(p[0] & 0x0fu) << 12) |
             ((uint32_t)(p[1] & 0x3fu) << 6) |
             (uint32_t)(p[2] & 0x3fu);
        *text = (const char *)(p + 3);
        return cp;
    }
    if ((p[0] & 0xf8u) == 0xf0u &&
        (p[1] & 0xc0u) == 0x80u &&
        (p[2] & 0xc0u) == 0x80u &&
        (p[3] & 0xc0u) == 0x80u) {
        cp = ((uint32_t)(p[0] & 0x07u) << 18) |
             ((uint32_t)(p[1] & 0x3fu) << 12) |
             ((uint32_t)(p[2] & 0x3fu) << 6) |
             (uint32_t)(p[3] & 0x3fu);
        *text = (const char *)(p + 4);
        return cp;
    }

    *text = (const char *)(p + 1);
    return '?';
}

static void draw_freetype_glyph(const struct ft_glyph_cache_entry *glyph,
                                int x, int y, uint16_t color, unsigned alpha_mul)
{
    for (int row = 0; row < glyph->rows; row++) {
        for (int col = 0; col < glyph->width; col++) {
            unsigned alpha = glyph->buffer[(size_t)row * (size_t)glyph->pitch + (size_t)col];
            alpha = (alpha * alpha_mul) / 255u;
            put_pixel_alpha(x + col, y + row, color, alpha);
        }
    }
}

static int draw_text_freetype(int x, int y, const char *text,
                              uint16_t color, int scale, int max_px)
{
    if (!g_font.active || !text) {
        return 0;
    }

    int pixel_height = scale <= 1 ? g_config.font_small_px : g_config.font_large_px;
    int baseline = y + pixel_height - 1;
    int limit = max_px > 0 ? x + max_px : g_fb.width;
    const char *p = text;

    while (*p != '\0') {
        uint32_t cp = utf8_next_codepoint(&p);
        if (cp == '\n' || cp == '\r') {
            break;
        }
        if (cp == '\t') {
            cp = ' ';
        }

        struct ft_glyph_cache_entry *glyph = font_load_glyph(cp, pixel_height);
        if (!glyph) {
            return 0;
        }
        if (x + glyph->advance > limit) {
            break;
        }

        int gx = x + glyph->bitmap_left;
        int gy = baseline - glyph->bitmap_top;
        if (glyph->buffer) {
            draw_freetype_glyph(glyph, gx + 1, gy + 1, rgb565(0, 0, 0), 150);
            draw_freetype_glyph(glyph, gx, gy, color, 255);
        }
        x += glyph->advance;
    }
    return 1;
}

static uint8_t glyph_row(char c, int row)
{
    static const uint8_t blank[7] = {0, 0, 0, 0, 0, 0, 0};
    static const uint8_t g_a[7] = {14, 17, 17, 31, 17, 17, 17};
    static const uint8_t g_b[7] = {30, 17, 17, 30, 17, 17, 30};
    static const uint8_t g_c[7] = {14, 17, 16, 16, 16, 17, 14};
    static const uint8_t g_d[7] = {30, 17, 17, 17, 17, 17, 30};
    static const uint8_t g_e[7] = {31, 16, 16, 30, 16, 16, 31};
    static const uint8_t g_f[7] = {31, 16, 16, 30, 16, 16, 16};
    static const uint8_t g_g[7] = {14, 17, 16, 23, 17, 17, 15};
    static const uint8_t g_h[7] = {17, 17, 17, 31, 17, 17, 17};
    static const uint8_t g_i[7] = {14, 4, 4, 4, 4, 4, 14};
    static const uint8_t g_j[7] = {1, 1, 1, 1, 17, 17, 14};
    static const uint8_t g_k[7] = {17, 18, 20, 24, 20, 18, 17};
    static const uint8_t g_l[7] = {16, 16, 16, 16, 16, 16, 31};
    static const uint8_t g_m[7] = {17, 27, 21, 21, 17, 17, 17};
    static const uint8_t g_n[7] = {17, 25, 21, 19, 17, 17, 17};
    static const uint8_t g_o[7] = {14, 17, 17, 17, 17, 17, 14};
    static const uint8_t g_p[7] = {30, 17, 17, 30, 16, 16, 16};
    static const uint8_t g_q[7] = {14, 17, 17, 17, 21, 18, 13};
    static const uint8_t g_r[7] = {30, 17, 17, 30, 20, 18, 17};
    static const uint8_t g_s[7] = {15, 16, 16, 14, 1, 1, 30};
    static const uint8_t g_t[7] = {31, 4, 4, 4, 4, 4, 4};
    static const uint8_t g_u[7] = {17, 17, 17, 17, 17, 17, 14};
    static const uint8_t g_v[7] = {17, 17, 17, 17, 17, 10, 4};
    static const uint8_t g_w[7] = {17, 17, 17, 21, 21, 21, 10};
    static const uint8_t g_x[7] = {17, 17, 10, 4, 10, 17, 17};
    static const uint8_t g_y[7] = {17, 17, 10, 4, 4, 4, 4};
    static const uint8_t g_z[7] = {31, 1, 2, 4, 8, 16, 31};
    static const uint8_t g_0[7] = {14, 17, 19, 21, 25, 17, 14};
    static const uint8_t g_1[7] = {4, 12, 4, 4, 4, 4, 14};
    static const uint8_t g_2[7] = {14, 17, 1, 2, 4, 8, 31};
    static const uint8_t g_3[7] = {30, 1, 1, 14, 1, 1, 30};
    static const uint8_t g_4[7] = {2, 6, 10, 18, 31, 2, 2};
    static const uint8_t g_5[7] = {31, 16, 16, 30, 1, 1, 30};
    static const uint8_t g_6[7] = {14, 16, 16, 30, 17, 17, 14};
    static const uint8_t g_7[7] = {31, 1, 2, 4, 8, 8, 8};
    static const uint8_t g_8[7] = {14, 17, 17, 14, 17, 17, 14};
    static const uint8_t g_9[7] = {14, 17, 17, 15, 1, 1, 14};
    static const uint8_t g_dot[7] = {0, 0, 0, 0, 0, 12, 12};
    static const uint8_t g_dash[7] = {0, 0, 0, 31, 0, 0, 0};
    static const uint8_t g_slash[7] = {1, 1, 2, 4, 8, 16, 16};
    static const uint8_t g_us[7] = {0, 0, 0, 0, 0, 0, 31};
    static const uint8_t g_colon[7] = {0, 12, 12, 0, 12, 12, 0};
    static const uint8_t g_lt[7] = {2, 4, 8, 16, 8, 4, 2};
    static const uint8_t g_gt[7] = {8, 4, 2, 1, 2, 4, 8};
    static const uint8_t g_lbr[7] = {14, 8, 8, 8, 8, 8, 14};
    static const uint8_t g_rbr[7] = {14, 2, 2, 2, 2, 2, 14};
    const uint8_t *g = blank;

    if (c >= 'a' && c <= 'z') {
        c = (char)(c - 'a' + 'A');
    }

    switch (c) {
    case 'A': g = g_a; break;
    case 'B': g = g_b; break;
    case 'C': g = g_c; break;
    case 'D': g = g_d; break;
    case 'E': g = g_e; break;
    case 'F': g = g_f; break;
    case 'G': g = g_g; break;
    case 'H': g = g_h; break;
    case 'I': g = g_i; break;
    case 'J': g = g_j; break;
    case 'K': g = g_k; break;
    case 'L': g = g_l; break;
    case 'M': g = g_m; break;
    case 'N': g = g_n; break;
    case 'O': g = g_o; break;
    case 'P': g = g_p; break;
    case 'Q': g = g_q; break;
    case 'R': g = g_r; break;
    case 'S': g = g_s; break;
    case 'T': g = g_t; break;
    case 'U': g = g_u; break;
    case 'V': g = g_v; break;
    case 'W': g = g_w; break;
    case 'X': g = g_x; break;
    case 'Y': g = g_y; break;
    case 'Z': g = g_z; break;
    case '0': g = g_0; break;
    case '1': g = g_1; break;
    case '2': g = g_2; break;
    case '3': g = g_3; break;
    case '4': g = g_4; break;
    case '5': g = g_5; break;
    case '6': g = g_6; break;
    case '7': g = g_7; break;
    case '8': g = g_8; break;
    case '9': g = g_9; break;
    case '.': g = g_dot; break;
    case '-': g = g_dash; break;
    case '/': g = g_slash; break;
    case '_': g = g_us; break;
    case ':': g = g_colon; break;
    case '<': g = g_lt; break;
    case '>': g = g_gt; break;
    case '[': g = g_lbr; break;
    case ']': g = g_rbr; break;
    default: break;
    }

    return g[row];
}

static void draw_char(int x, int y, char c, uint16_t color, int scale)
{
    for (int row = 0; row < 7; row++) {
        uint8_t bits = glyph_row(c, row);
        for (int col = 0; col < 5; col++) {
            if ((bits & (uint8_t)(1u << (4 - col))) != 0) {
                fill_rect(x + col * scale, y + row * scale, scale, scale, color);
            }
        }
    }
}

static void draw_text_bitmap(int x, int y, const char *text, uint16_t color, int scale, int max_px)
{
    int advance = 6 * scale;
    int limit = max_px > 0 ? x + max_px : g_fb.width;

    for (const char *p = text; *p != '\0'; p++) {
        if (x + 5 * scale > limit) {
            break;
        }
        draw_char(x + scale, y + scale, *p, rgb565(0, 0, 0), scale);
        draw_char(x, y, *p, color, scale);
        x += advance;
    }
}

static void draw_text(int x, int y, const char *text, uint16_t color, int scale, int max_px)
{
    if (draw_text_freetype(x, y, text, color, scale, max_px)) {
        return;
    }
    draw_text_bitmap(x, y, text, color, scale, max_px);
}

static void join_path(char *out, size_t out_size, const char *dir, const char *name)
{
    if (strcmp(dir, "/") == 0) {
        snprintf(out, out_size, "/%s", name);
    } else {
        snprintf(out, out_size, "%s/%s", dir, name);
    }
}

static void parent_path(char *path)
{
    size_t len = strlen(path);
    if (len <= 1) {
        strcpy(path, "/");
        return;
    }
    while (len > 1 && path[len - 1] == '/') {
        path[--len] = '\0';
    }
    char *slash = strrchr(path, '/');
    if (!slash || slash == path) {
        strcpy(path, "/");
    } else {
        *slash = '\0';
    }
}

static int compare_entries(const void *a, const void *b)
{
    const struct entry *ea = (const struct entry *)a;
    const struct entry *eb = (const struct entry *)b;
    if (ea->is_dir != eb->is_dir) {
        return eb->is_dir - ea->is_dir;
    }
    return strcasecmp(ea->name, eb->name);
}

static int has_script_suffix(const char *name)
{
    size_t n = strlen(name);
    return n > 3 && strcasecmp(name + n - 3, ".sh") == 0;
}

static int is_extension_separator(char c)
{
    return c == ',' || c == ';' || c == ' ' || c == '\t';
}

static int has_text_suffix(const char *name)
{
    const char *dot = strrchr(name, '.');
    const char *list = g_config.text_extensions;
    size_t ext_len;

    if (!dot || dot[1] == '\0') {
        return 0;
    }
    dot++;
    ext_len = strlen(dot);

    while (*list != '\0') {
        const char *start;
        size_t len;

        while (is_extension_separator(*list)) {
            list++;
        }
        start = list;
        while (*list != '\0' && !is_extension_separator(*list)) {
            list++;
        }
        len = (size_t)(list - start);
        if (len > 0 && start[0] == '.') {
            start++;
            len--;
        }
        if (len == ext_len && strncasecmp(start, dot, len) == 0) {
            return 1;
        }
    }

    return 0;
}

static int find_dir_state(const char *path)
{
    for (int i = 0; i < g_dir_state_count; i++) {
        if (strcmp(g_dir_states[i].path, path) == 0) {
            return i;
        }
    }
    return -1;
}

static void save_dir_state(const char *path)
{
    int idx;
    struct dir_state *state;

    if (!path || path[0] == '\0') {
        return;
    }

    idx = find_dir_state(path);
    if (idx < 0) {
        if (g_dir_state_count < MAX_DIR_STATES) {
            idx = g_dir_state_count++;
        } else {
            idx = g_dir_state_next;
            g_dir_state_next = (g_dir_state_next + 1) % MAX_DIR_STATES;
        }
    }

    state = &g_dir_states[idx];
    snprintf(state->path, sizeof(state->path), "%s", path);
    state->selected = g_selected;
    state->scroll = g_scroll;
    state->selected_name[0] = '\0';
    if (g_selected >= 0 && g_selected < g_entry_count) {
        snprintf(state->selected_name, sizeof(state->selected_name), "%s",
                 g_entries[g_selected].name);
    }
}

static void restore_dir_state_after_scan(const char *path)
{
    int idx = find_dir_state(path);
    int selected = 0;
    int scroll = 0;

    if (idx >= 0) {
        struct dir_state *state = &g_dir_states[idx];
        selected = state->selected;
        scroll = state->scroll;
        if (state->selected_name[0] != '\0') {
            for (int i = 0; i < g_entry_count; i++) {
                if (strcmp(g_entries[i].name, state->selected_name) == 0) {
                    selected = i;
                    break;
                }
            }
        }
        log_msg("dir state restore: path=%s selected=%d scroll=%d name=%s",
                path, selected, scroll, state->selected_name);
    }

    g_selected = selected;
    g_scroll = scroll;
}

static void ensure_selection_visible(void);

static void scan_directory(void)
{
    DIR *dir;
    struct dirent *de;

    log_msg("scan_directory: %s", g_cwd);
    g_entry_count = 0;
    if (strcmp(g_cwd, "/") != 0) {
        strcpy(g_entries[g_entry_count].name, "..");
        g_entries[g_entry_count].is_dir = 1;
        g_entries[g_entry_count].is_exec = 0;
        g_entries[g_entry_count].is_text = 0;
        g_entries[g_entry_count].size = 0;
        g_entry_count++;
    }

    dir = opendir(g_cwd);
    if (!dir) {
        snprintf(g_status, sizeof(g_status), "Cannot open %s: %s", g_cwd, strerror(errno));
        log_msg("%s", g_status);
        parent_path(g_cwd);
        g_selected = 0;
        g_scroll = 0;
        return;
    }

    while ((de = readdir(dir)) != NULL && g_entry_count < MAX_ENTRIES) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }

        struct entry *e = &g_entries[g_entry_count];
        char full[PATH_MAX];
        struct stat st;
        memset(e, 0, sizeof(*e));
        snprintf(e->name, sizeof(e->name), "%s", de->d_name);
        join_path(full, sizeof(full), g_cwd, e->name);
        if (lstat(full, &st) == 0) {
            e->is_dir = S_ISDIR(st.st_mode);
            e->is_exec = ((st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0) || has_script_suffix(e->name);
            e->is_text = !e->is_dir && has_text_suffix(e->name);
            e->size = st.st_size;
        } else {
            e->is_dir = 0;
            e->is_exec = 0;
            e->is_text = 0;
            e->size = 0;
        }
        g_entry_count++;
    }
    closedir(dir);

    int start = (strcmp(g_cwd, "/") != 0) ? 1 : 0;
    if (g_entry_count - start > 1) {
        qsort(&g_entries[start], (size_t)(g_entry_count - start), sizeof(g_entries[0]), compare_entries);
    }

    if (g_selected >= g_entry_count) {
        g_selected = g_entry_count > 0 ? g_entry_count - 1 : 0;
    }
    if (g_selected < 0) {
        g_selected = 0;
    }
    if (g_scroll > g_selected) {
        g_scroll = g_selected;
    }
    restore_dir_state_after_scan(g_cwd);
    ensure_selection_visible();
    log_msg("scan_directory done entries=%d selected=%d scroll=%d", g_entry_count, g_selected, g_scroll);
}

static void input_open_devices(void)
{
    memset(&g_input, 0, sizeof(g_input));
    for (size_t i = 0; i < ARRAY_COUNT(g_input.fds); i++) {
        g_input.fds[i] = -1;
    }

    for (int i = 0; i < 4 && g_input.fd_count < MAX_INPUT_FDS; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/input/js%d", i);
        int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
        if (fd >= 0) {
            set_close_on_exec(fd);
            g_input.fds[g_input.fd_count] = fd;
            g_input.is_js[g_input.fd_count] = 1;
            g_input.fd_count++;
            log_msg("input opened: %s", path);
        }
    }

    for (int i = 0; i < 10 && g_input.fd_count < MAX_INPUT_FDS; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
        if (fd >= 0) {
            set_close_on_exec(fd);
            g_input.fds[g_input.fd_count] = fd;
            g_input.is_js[g_input.fd_count] = 0;
            g_input.fd_count++;
            log_msg("input opened: %s", path);
        }
    }

    snprintf(g_status, sizeof(g_status), "Input devices opened: %d", g_input.fd_count);
    log_msg("%s", g_status);
}

static void input_close_devices(void)
{
    log_msg("input_close_devices count=%d", g_input.fd_count);
    for (int i = 0; i < g_input.fd_count; i++) {
        if (g_input.fds[i] >= 0) {
            close(g_input.fds[i]);
            g_input.fds[i] = -1;
        }
    }
    g_input.fd_count = 0;
}

static uint32_t input_translate_rkgame_keys(uint32_t raw)
{
    uint32_t b = 0;

    if ((raw & R36SX_RKGAME_KEY_UP) != 0) {
        b |= BTN_UP_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_DOWN) != 0) {
        b |= BTN_DOWN_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_LEFT) != 0) {
        b |= BTN_LEFT_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_RIGHT) != 0) {
        b |= BTN_RIGHT_BIT;
    }
    if ((raw & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_X)) != 0) {
        b |= BTN_A_BIT;
    }
    if ((raw & (R36SX_RKGAME_KEY_B | R36SX_RKGAME_KEY_Y)) != 0) {
        b |= BTN_B_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_START) != 0) {
        b |= BTN_START_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_SELECT) != 0) {
        b |= BTN_SELECT_BIT;
    }
#if ENABLE_FN_ICUBE_SHORTCUT
    if ((raw & R36SX_RKGAME_KEY_FN) != 0) {
        b |= BTN_FN_BIT;
    }
#endif

    return b;
}

static void input_poll_cube(void)
{
    if (!g_driver.cube_ioctl) {
        return;
    }

    if (g_input.cube_key_addr == 0) {
        uint32_t addr = 0;
        int rc = g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_input.cube_key_addr = addr;
        g_input.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
        log_msg("cube_ioctl joy key shm rc=%d addr=0x%08x", rc, addr);
    }

    if (!g_input.cube_key_mem) {
        return;
    }

    uint32_t raw0 = g_input.cube_key_mem[0];
    uint32_t raw1 = g_input.cube_key_mem[1];
    uint32_t state = 0;
    g_driver.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);

    g_input.cube_buttons = input_translate_rkgame_keys(raw0) | input_translate_rkgame_keys(raw1);
    if (raw0 != g_input.cube_raw0 || raw1 != g_input.cube_raw1 || state != g_input.cube_state) {
        log_msg("cube input raw0=0x%08x raw1=0x%08x state=0x%08x buttons=0x%08x",
                raw0, raw1, state, g_input.cube_buttons);
        g_input.cube_raw0 = raw0;
        g_input.cube_raw1 = raw1;
        g_input.cube_state = state;
    }
}

static void input_poll_devices(void)
{
    input_poll_cube();

    for (int i = 0; i < g_input.fd_count; i++) {
        if (g_input.fds[i] < 0) {
            continue;
        }

        if (g_input.is_js[i]) {
            struct js_event_local ev;
            while (read(g_input.fds[i], &ev, sizeof(ev)) == (ssize_t)sizeof(ev)) {
                uint8_t type = ev.type & (uint8_t)~JS_EVENT_INIT_LOCAL;
                if (type == JS_EVENT_AXIS_LOCAL && ev.number < ARRAY_COUNT(g_input.js_axis)) {
                    g_input.js_axis[ev.number] = ev.value;
                } else if (type == JS_EVENT_BUTTON_LOCAL && ev.number < ARRAY_COUNT(g_input.js_button)) {
                    g_input.js_button[ev.number] = ev.value != 0;
                }
            }
        } else {
            struct input_event_local ev;
            while (read(g_input.fds[i], &ev, sizeof(ev)) == (ssize_t)sizeof(ev)) {
                if (ev.type == EV_KEY_LOCAL && ev.code < ARRAY_COUNT(g_input.ev_key)) {
                    g_input.ev_key[ev.code] = ev.value != 0;
                } else if (ev.type == EV_ABS_LOCAL) {
                    if (ev.code == ABS_X_LOCAL) {
                        g_input.ev_abs_x = ev.value;
                    } else if (ev.code == ABS_Y_LOCAL) {
                        g_input.ev_abs_y = ev.value;
                    }
                }
            }
        }
    }
}

static uint32_t input_buttons(void)
{
    uint32_t b = g_input.cube_buttons;

    if (g_input.js_axis[0] < -JS_DEADZONE) {
        b |= BTN_LEFT_BIT;
    } else if (g_input.js_axis[0] > JS_DEADZONE) {
        b |= BTN_RIGHT_BIT;
    }
    if (g_input.js_axis[1] < -JS_DEADZONE) {
        b |= BTN_UP_BIT;
    } else if (g_input.js_axis[1] > JS_DEADZONE) {
        b |= BTN_DOWN_BIT;
    }

    if (g_input.js_button[2] || g_input.js_button[1]) {
        b |= BTN_A_BIT;
    }
    if (g_input.js_button[0] || g_input.js_button[3]) {
        b |= BTN_B_BIT;
    }
    if (g_input.js_button[9] || g_input.js_button[3]) {
        b |= BTN_START_BIT;
    }
    if (g_input.js_button[8]) {
        b |= BTN_SELECT_BIT;
    }

    if (g_input.ev_key[KEY_UP_LOCAL] || g_input.ev_key[BTN_DPAD_UP_LOCAL]) {
        b |= BTN_UP_BIT;
    }
    if (g_input.ev_key[KEY_DOWN_LOCAL] || g_input.ev_key[BTN_DPAD_DOWN_LOCAL]) {
        b |= BTN_DOWN_BIT;
    }
    if (g_input.ev_key[KEY_LEFT_LOCAL] || g_input.ev_key[BTN_DPAD_LEFT_LOCAL]) {
        b |= BTN_LEFT_BIT;
    }
    if (g_input.ev_key[KEY_RIGHT_LOCAL] || g_input.ev_key[BTN_DPAD_RIGHT_LOCAL]) {
        b |= BTN_RIGHT_BIT;
    }
    if (g_input.ev_key[KEY_ENTER_LOCAL] || g_input.ev_key[KEY_SPACE_LOCAL] ||
        g_input.ev_key[BTN_A_LOCAL] || g_input.ev_key[BTN_B_LOCAL]) {
        b |= BTN_A_BIT;
    }
    if (g_input.ev_key[KEY_ESC_LOCAL] || g_input.ev_key[BTN_X_LOCAL] || g_input.ev_key[BTN_Y_LOCAL]) {
        b |= BTN_B_BIT;
    }
    if (g_input.ev_key[BTN_START_LOCAL]) {
        b |= BTN_START_BIT;
    }
    if (g_input.ev_key[BTN_SELECT_LOCAL]) {
        b |= BTN_SELECT_BIT;
    }

    return b;
}

static void input_reset_after_child(void)
{
    input_poll_devices();
    g_prev_buttons = input_buttons();
    g_repeat_buttons = 0;
    g_next_repeat_ms = 0;
    g_wait_buttons_release = g_prev_buttons != 0;
    if (g_wait_buttons_release) {
        log_msg("suppressing post-launch buttons until release: 0x%08x",
                g_prev_buttons);
    }
}

static int visible_rows(void)
{
    int top = HEADER_H + LIST_TOP_OFFSET;
    int bottom = g_fb.height - FOOTER_H - LIST_BOTTOM_PAD;
    int rows = (bottom - top) / g_config.list_row_h;
    return rows > 1 ? rows : 1;
}

static int list_row_extra_px(void)
{
    int extra = g_config.list_row_h - g_config.font_large_px;
    return extra > 0 ? extra : 0;
}

static int list_row_pad_top(void)
{
    return list_row_extra_px() / 2;
}

static int list_row_pad_bottom(void)
{
    int extra = list_row_extra_px();
    return extra - extra / 2;
}

static int list_row_text_y(int row_y)
{
    const int scale = 2;

    if (g_font.active) {
        static int cached_pixel_height = -1;
        static int cached_row_h = -1;
        static int cached_offset = 0;
        int pixel_height = g_config.font_large_px;

        if (cached_pixel_height != pixel_height ||
            cached_row_h != g_config.list_row_h) {
            const char *sample =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]";
            int visible_top = 10000;
            int visible_bottom = -10000;
            int found = 0;

            for (const char *p = sample; *p != '\0'; p++) {
                struct ft_glyph_cache_entry *glyph =
                    font_load_glyph((uint32_t)(unsigned char)*p, pixel_height);
                if (glyph && glyph->buffer && glyph->rows > 0) {
                    int glyph_top = pixel_height - 1 - glyph->bitmap_top;
                    int glyph_bottom = glyph_top + glyph->rows;
                    if (glyph_top < visible_top) {
                        visible_top = glyph_top;
                    }
                    if (glyph_bottom > visible_bottom) {
                        visible_bottom = glyph_bottom;
                    }
                    found = 1;
                }
            }

            if (found && visible_bottom > visible_top) {
                int visible_h = visible_bottom - visible_top;
                cached_offset =
                    (g_config.list_row_h - visible_h) / 2 - visible_top;
            } else {
                cached_offset =
                    (g_config.list_row_h - pixel_height) / 2;
            }
            cached_pixel_height = pixel_height;
            cached_row_h = g_config.list_row_h;
        }

        return row_y + cached_offset;
    }

    return row_y + (g_config.list_row_h - 7 * scale) / 2;
}

static void ensure_selection_visible(void)
{
    int rows = visible_rows();
    if (g_selected < 0) {
        g_selected = 0;
    }
    if (g_selected >= g_entry_count) {
        g_selected = g_entry_count > 0 ? g_entry_count - 1 : 0;
    }
    if (g_selected < g_scroll) {
        g_scroll = g_selected;
    }
    if (g_selected >= g_scroll + rows) {
        g_scroll = g_selected - rows + 1;
    }
    if (g_scroll < 0) {
        g_scroll = 0;
    }
    int max_scroll = g_entry_count - rows;
    if (max_scroll < 0) {
        max_scroll = 0;
    }
    if (g_scroll > max_scroll) {
        g_scroll = max_scroll;
    }
}

static void format_entry(char *out, size_t out_size, const struct entry *e)
{
    if (strcmp(e->name, "..") == 0) {
        snprintf(out, out_size, "[UP] ..");
    } else if (e->is_dir) {
        snprintf(out, out_size, "[DIR] %s", e->name);
    } else if (e->is_text) {
        snprintf(out, out_size, "[TXT] %s", e->name);
    } else if (e->is_exec || has_script_suffix(e->name)) {
        snprintf(out, out_size, "[RUN] %s", e->name);
    } else {
        snprintf(out, out_size, "      %s", e->name);
    }
}

static void draw_scrollbar_for(int top, int rows, int total, int scroll, int row_h)
{
    int track_h;
    int thumb_h;
    int max_scroll;
    int movable;
    int thumb_y;
    int x;

    if (total <= rows || rows <= 0) {
        return;
    }

    track_h = rows * row_h;
    x = g_fb.width - SCROLLBAR_RIGHT_PAD - SCROLLBAR_WIDTH;
    max_scroll = total - rows;
    thumb_h = (rows * track_h) / total;
    if (thumb_h < SCROLLBAR_MIN_THUMB) {
        thumb_h = SCROLLBAR_MIN_THUMB;
    }
    if (thumb_h > track_h) {
        thumb_h = track_h;
    }

    movable = track_h - thumb_h;
    thumb_y = top;
    if (max_scroll > 0 && movable > 0) {
        thumb_y += (scroll * movable + max_scroll / 2) / max_scroll;
    }
    if (thumb_y + thumb_h > top + track_h) {
        thumb_y = top + track_h - thumb_h;
    }

    fill_rect(x, top, SCROLLBAR_WIDTH, track_h, rgb565(32, 42, 48));
    fill_rect(x, thumb_y, SCROLLBAR_WIDTH, thumb_h, rgb565(105, 138, 146));
    fill_rect(x, thumb_y, SCROLLBAR_WIDTH, 1, rgb565(170, 205, 205));
}

static void draw_scrollbar(int top, int rows)
{
    draw_scrollbar_for(top, rows, g_entry_count, g_scroll,
                       g_config.list_row_h);
}

static int text_viewer_row_h(void)
{
    int row_h = g_config.font_small_px + 3;
    return row_h > 10 ? row_h : 10;
}

static int text_viewer_visible_rows(void)
{
    int top = HEADER_H + LIST_TOP_OFFSET;
    int bottom = g_fb.height - FOOTER_H - LIST_BOTTOM_PAD;
    int rows = (bottom - top) / text_viewer_row_h();
    return rows > 1 ? rows : 1;
}

static void text_viewer_release(void)
{
    free(g_viewer.lines);
    free(g_viewer.buffer);
    memset(&g_viewer, 0, sizeof(g_viewer));
}

static void text_viewer_close(void)
{
    if (g_viewer.active) {
        log_msg("text viewer close: %s", g_viewer.path);
    }
    text_viewer_release();
    snprintf(g_status, sizeof(g_status), "Text viewer closed");
}

static void text_viewer_clamp_scroll(void)
{
    int rows = text_viewer_visible_rows();
    int max_scroll = g_viewer.line_count - rows;

    if (max_scroll < 0) {
        max_scroll = 0;
    }
    if (g_viewer.scroll < 0) {
        g_viewer.scroll = 0;
    }
    if (g_viewer.scroll > max_scroll) {
        g_viewer.scroll = max_scroll;
    }
}

static int text_viewer_open(const char *path, const char *title)
{
    int fd;
    struct stat st;
    size_t wanted = TEXT_VIEWER_MAX_BYTES;
    size_t total = 0;
    char *buffer;
    char **lines;

    text_viewer_release();
    fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        snprintf(g_status, sizeof(g_status), "Cannot open text: %s", strerror(errno));
        log_msg("text viewer open failed: %s: %s", path, strerror(errno));
        return -1;
    }
    set_close_on_exec(fd);

    if (fstat(fd, &st) == 0 && st.st_size >= 0 &&
        (uint64_t)st.st_size < (uint64_t)wanted) {
        wanted = (size_t)st.st_size;
    }

    buffer = (char *)malloc(wanted + 1);
    lines = (char **)malloc(sizeof(char *) * TEXT_VIEWER_MAX_LINES);
    if (!buffer || !lines) {
        close(fd);
        free(lines);
        free(buffer);
        snprintf(g_status, sizeof(g_status), "Not enough memory for text viewer");
        log_msg("text viewer malloc failed for %s", path);
        return -1;
    }

    while (total < wanted) {
        ssize_t rc = read(fd, buffer + total, wanted - total);
        if (rc < 0) {
            if (errno == EINTR) {
                continue;
            }
            close(fd);
            free(lines);
            free(buffer);
            snprintf(g_status, sizeof(g_status), "Read failed: %s", strerror(errno));
            log_msg("text viewer read failed: %s: %s", path, strerror(errno));
            return -1;
        }
        if (rc == 0) {
            break;
        }
        total += (size_t)rc;
    }
    close(fd);
    buffer[total] = '\0';

    int line_count = 0;
    if (total > 0) {
        lines[line_count++] = buffer;
    }
    for (size_t i = 0; i < total && line_count < TEXT_VIEWER_MAX_LINES; i++) {
        unsigned char c = (unsigned char)buffer[i];
        if (c == '\r' || c == '\n') {
            buffer[i] = '\0';
            if (c == '\r' && i + 1 < total && buffer[i + 1] == '\n') {
                i++;
                buffer[i] = '\0';
            }
            if (i + 1 < total && line_count < TEXT_VIEWER_MAX_LINES) {
                lines[line_count++] = buffer + i + 1;
            }
        } else if ((c < 32 && c != '\t') || c == 127) {
            buffer[i] = ' ';
        }
    }

    g_viewer.active = 1;
    g_viewer.buffer = buffer;
    g_viewer.lines = lines;
    g_viewer.byte_count = total;
    g_viewer.line_count = line_count;
    g_viewer.scroll = 0;
    g_viewer.truncated =
        total == TEXT_VIEWER_MAX_BYTES ||
        (line_count >= TEXT_VIEWER_MAX_LINES && total > 0);
    snprintf(g_viewer.path, sizeof(g_viewer.path), "%s", path);
    snprintf(g_viewer.title, sizeof(g_viewer.title), "%s", title);
    snprintf(g_status, sizeof(g_status), "Viewing %s: %d lines%s",
             title, line_count, g_viewer.truncated ? " (truncated)" : "");
    log_msg("text viewer open: path=%s bytes=%lu lines=%d truncated=%d",
            path, (unsigned long)total, line_count, g_viewer.truncated);
    return 0;
}

static void draw_text_viewer_ui(void)
{
    uint16_t bg = rgb565(12, 16, 20);
    uint16_t band = rgb565(24, 38, 48);
    uint16_t accent = rgb565(55, 150, 120);
    uint16_t hi = rgb565(240, 170, 60);
    uint16_t text = rgb565(222, 235, 228);
    uint16_t muted = rgb565(145, 160, 165);
    uint16_t black = rgb565(0, 0, 0);
    int top = HEADER_H + LIST_TOP_OFFSET;
    int row_h = text_viewer_row_h();
    int rows = text_viewer_visible_rows();
    char header[360];
    char footer[192];

    text_viewer_clamp_scroll();
    fill_rect(0, 0, g_fb.width, g_fb.height, bg);
    fill_rect(0, 0, g_fb.width, HEADER_H, band);
    fill_rect(0, HEADER_H - 3, g_fb.width, 3, accent);
    fill_rect(0, g_fb.height - FOOTER_H, g_fb.width, FOOTER_H, band);
    fill_rect(0, g_fb.height - FOOTER_H, g_fb.width, 3, hi);

    snprintf(header, sizeof(header), "VIEW %s", g_viewer.title);
    draw_text(12, 12, header, rgb565(250, 250, 245), 2, g_fb.width - 24);
    draw_text(12, HEADER_H + 4, g_viewer.path, muted, 1, g_fb.width - 24);

    if (g_viewer.line_count == 0) {
        draw_text(12, top, "[EMPTY TEXT FILE]", muted, 1, g_fb.width - 24);
    } else {
        for (int row = 0; row < rows; row++) {
            int idx = g_viewer.scroll + row;
            if (idx >= g_viewer.line_count) {
                break;
            }
            draw_text(12, top + row * row_h, g_viewer.lines[idx],
                      text, 1, g_fb.width - 36);
        }
    }

    draw_scrollbar_for(top, rows, g_viewer.line_count, g_viewer.scroll, row_h);

    snprintf(footer, sizeof(footer),
             "UP/DOWN SCROLL  SELECT PGUP  A/START PGDN  LEFT/B BACK");
    draw_text(12, g_fb.height - FOOTER_H + 8, footer,
              rgb565(230, 240, 220), 1, g_fb.width - 24);
    snprintf(footer, sizeof(footer), "Line %d/%d%s",
             g_viewer.line_count > 0 ? g_viewer.scroll + 1 : 0,
             g_viewer.line_count,
             g_viewer.truncated ? "  TRUNCATED" : "");
    draw_text(12, g_fb.height - 17, footer,
              rgb565(220, 210, 180), 1, g_fb.width - 24);

    fill_rect(0, g_fb.height - 1, g_fb.width, 1, black);
    present_frame();
}

static void draw_ui(void)
{
    uint16_t bg = rgb565(14, 18, 24);
    uint16_t band = rgb565(24, 40, 56);
    uint16_t accent = rgb565(45, 160, 210);
    uint16_t hi = rgb565(240, 170, 60);
    uint16_t text = rgb565(220, 235, 230);
    uint16_t muted = rgb565(150, 165, 170);
    uint16_t black = rgb565(0, 0, 0);
    int top = HEADER_H + LIST_TOP_OFFSET;
    int rows = visible_rows();
    int has_scrollbar = g_entry_count > rows;
    int selection_w = g_fb.width - (has_scrollbar ? 34 : 16);
    int text_max_w = g_fb.width - (has_scrollbar ? 64 : 44);
    int row_pad_top = list_row_pad_top();
    int row_pad_bottom = list_row_pad_bottom();
    int row_marker_h = g_config.list_row_h - row_pad_top - row_pad_bottom;
    if (row_marker_h < 1) {
        row_marker_h = 1;
    }

    if (g_viewer.active) {
        draw_text_viewer_ui();
        return;
    }

    fill_rect(0, 0, g_fb.width, g_fb.height, bg);
    fill_rect(0, 0, g_fb.width, HEADER_H, band);
    fill_rect(0, HEADER_H - 3, g_fb.width, 3, accent);
    fill_rect(0, g_fb.height - FOOTER_H, g_fb.width, FOOTER_H, band);
    fill_rect(0, g_fb.height - FOOTER_H, g_fb.width, 3, hi);

    draw_text(12, 12, "R36SX TINY MC", rgb565(250, 250, 245), 2, g_fb.width - 24);
    draw_text(12, HEADER_H + 4, g_cwd, muted, 1, g_fb.width - 24);

    if (g_entry_count == 0) {
        draw_text(22, list_row_text_y(top), "[EMPTY]", muted, 2,
                  g_fb.width - 44);
    }

    for (int row = 0; row < rows; row++) {
        int idx = g_scroll + row;
        if (idx >= g_entry_count) {
            break;
        }
        int y = top + row * g_config.list_row_h;
        int text_y = list_row_text_y(y);
        char label[320];
        format_entry(label, sizeof(label), &g_entries[idx]);
        if (idx == g_selected) {
            fill_rect(8, y, selection_w, g_config.list_row_h, rgb565(50, 78, 92));
            fill_rect(10, text_y, 4, row_marker_h, hi);
            draw_text(22, text_y, label, rgb565(255, 246, 220), 2, text_max_w);
        } else {
            draw_text(22, text_y, label, text, 2, text_max_w);
        }
    }

    draw_scrollbar(top, rows);

    draw_text(12, g_fb.height - FOOTER_H + 8,
              "UP/DOWN SELECT  A/START OPEN  RIGHT ENTER  LEFT/B BACK  FN ICUBE",
              rgb565(230, 240, 220), 1, g_fb.width - 24);
    draw_text(12, g_fb.height - 17, g_status, rgb565(220, 210, 180), 1, g_fb.width - 24);

    fill_rect(0, g_fb.height - 1, g_fb.width, 1, black);
    present_frame();
}

static void change_dir_to_selected(void)
{
    if (g_entry_count <= 0) {
        return;
    }

    struct entry *e = &g_entries[g_selected];
    save_dir_state(g_cwd);
    if (strcmp(e->name, "..") == 0) {
        parent_path(g_cwd);
    } else if (e->is_dir) {
        char next[PATH_MAX];
        join_path(next, sizeof(next), g_cwd, e->name);
        snprintf(g_cwd, sizeof(g_cwd), "%s", next);
    } else {
        return;
    }

    scan_directory();
}

static void split_dir_base(const char *path, char *dir, size_t dir_size, const char **base)
{
    const char *slash = strrchr(path, '/');
    if (!slash) {
        snprintf(dir, dir_size, ".");
        *base = path;
    } else if (slash == path) {
        snprintf(dir, dir_size, "/");
        *base = slash + 1;
    } else {
        size_t n = (size_t)(slash - path);
        if (n >= dir_size) {
            n = dir_size - 1;
        }
        memcpy(dir, path, n);
        dir[n] = '\0';
        *base = slash + 1;
    }
}

static void finish_button_frame(uint32_t buttons)
{
    ensure_selection_visible();
    save_dir_state(g_cwd);
    g_prev_buttons = buttons;
}

static int restart_self_after_child(const char *start_dir)
{
    const char *dir = (start_dir && start_dir[0]) ? start_dir : R36SX_MIPS_NATIVE_DIR;

    log_msg("restarting tiny_mc after child: %s", dir);
    heartbeat_close();
    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);
    execl(TINY_MC_EXEC_PATH, "tiny_mc", dir, (char *)NULL);
    log_msg("tiny_mc self-exec failed: %s", strerror(errno));
    return -1;
}

static int launch_selected(void)
{
    if (g_entry_count <= 0) {
        return 0;
    }

    struct entry *e = &g_entries[g_selected];
    if (e->is_dir || strcmp(e->name, "..") == 0) {
        change_dir_to_selected();
        return 0;
    }
    save_dir_state(g_cwd);

    char path[PATH_MAX];
    char dir[PATH_MAX];
    const char *base;
    join_path(path, sizeof(path), g_cwd, e->name);

    if (e->is_text || has_text_suffix(e->name)) {
        save_dir_state(g_cwd);
        text_viewer_open(path, e->name);
        return 0;
    }

    split_dir_base(path, dir, sizeof(dir), &base);

    snprintf(g_status, sizeof(g_status), "Running %s", e->name);
    log_msg("launch requested: path=%s dir=%s base=%s", path, dir, base);
    draw_ui();
    display_close();
    input_close_devices();

    pid_t pid = fork();
    if (pid < 0) {
        snprintf(g_status, sizeof(g_status), "fork failed: %s", strerror(errno));
        log_msg("%s", g_status);
        display_reopen_after_child();
        input_open_devices();
        input_reset_after_child();
        draw_ui();
        return 1;
    }

    if (pid == 0) {
        chdir(dir);
        setenv("LD_LIBRARY_PATH",
               "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
               1);
        if (has_script_suffix(base)) {
            execl("/bin/sh", "sh", path, (char *)NULL);
        } else {
            execl(path, base, (char *)NULL);
        }
        log_msg("exec failed in child: %s: %s", path, strerror(errno));
        _exit(127);
    }

    log_msg("waiting child pid=%ld", (long)pid);
    int status = 0;
    int wait_error = 0;
    for (;;) {
        pid_t wait_rc = waitpid(pid, &status, WNOHANG);
        if (wait_rc == pid) {
            break;
        }
        if (wait_rc < 0 && errno != EINTR) {
            snprintf(g_status, sizeof(g_status), "waitpid failed: %s", strerror(errno));
            log_msg("%s", g_status);
            wait_error = errno;
            break;
        }
        heartbeat_tick();
        usleep(REFRESH_USEC);
    }

    char result[sizeof(g_status)];
    if (wait_error != 0) {
        snprintf(result, sizeof(result), "%s wait failed: %s", e->name, strerror(wait_error));
    } else if (WIFEXITED(status)) {
        snprintf(result, sizeof(result), "%s exited: %d", e->name, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        snprintf(result, sizeof(result), "%s signaled: %d", e->name, WTERMSIG(status));
    } else {
        snprintf(result, sizeof(result), "%s returned", e->name);
    }
    log_msg("launch result: %s raw_status=0x%x", result, status);
    if (restart_self_after_child(g_cwd) == 0) {
        return 1;
    }
    if (display_reopen_after_child() != 0) {
        log_msg("display reopen failed after child exit; continuing with fallback state");
    }
    input_open_devices();
    input_reset_after_child();
    scan_directory();
    snprintf(g_status, sizeof(g_status), "%s", result);
    draw_ui();
    return 1;
}

#if ENABLE_FN_ICUBE_SHORTCUT
static void launch_icube(void)
{
    int err;

    save_dir_state(g_cwd);
    snprintf(g_status, sizeof(g_status), "Starting iCube");
    log_msg("FN pressed: launching %s", R36SX_ICUBE_PATH);
    draw_ui();
    display_close();
    input_close_devices();
    heartbeat_close();

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);
    chdir(R36SX_CUBEGM_DIR);
    execl(R36SX_ICUBE_PATH, "icube", (char *)NULL);

    err = errno;
    snprintf(g_status, sizeof(g_status), "iCube failed: %s", strerror(err));
    log_msg("icube exec failed: %s", strerror(err));
    display_open();
    input_open_devices();
}
#endif

static void choose_start_dir(int argc, char **argv)
{
    if (argc > 1 && access(argv[1], R_OK | X_OK) == 0) {
        snprintf(g_cwd, sizeof(g_cwd), "%s", argv[1]);
    } else if (access(R36SX_MIPS_NATIVE_DIR, R_OK | X_OK) == 0) {
        snprintf(g_cwd, sizeof(g_cwd), "%s", R36SX_MIPS_NATIVE_DIR);
    } else if (access("/mnt/sdcard", R_OK | X_OK) == 0) {
        snprintf(g_cwd, sizeof(g_cwd), "%s", "/mnt/sdcard");
    } else if (access("/media", R_OK | X_OK) == 0) {
        snprintf(g_cwd, sizeof(g_cwd), "%s", "/media");
    } else if (getcwd(g_cwd, sizeof(g_cwd)) == NULL) {
        snprintf(g_cwd, sizeof(g_cwd), "%s", "/");
    }
    log_msg("start dir: %s", g_cwd);
}

static void handle_text_viewer_buttons(uint32_t buttons, uint32_t changed,
                                       uint32_t nav, long now)
{
    int rows = text_viewer_visible_rows();
    int page = rows > 1 ? rows - 1 : 1;

    if ((changed & (BTN_LEFT_BIT | BTN_B_BIT)) != 0) {
        text_viewer_close();
        g_repeat_buttons = 0;
        finish_button_frame(buttons);
        return;
    }

    if ((changed & BTN_SELECT_BIT) != 0) {
        g_viewer.scroll -= page;
        g_repeat_buttons = 0;
    } else if ((changed & (BTN_RIGHT_BIT | BTN_A_BIT | BTN_START_BIT)) != 0) {
        g_viewer.scroll += page;
        g_repeat_buttons = 0;
    } else if ((changed & BTN_UP_BIT) != 0 ||
               (nav == BTN_UP_BIT && g_repeat_buttons == BTN_UP_BIT &&
                now >= g_next_repeat_ms)) {
        g_viewer.scroll--;
        g_repeat_buttons = BTN_UP_BIT;
        g_next_repeat_ms = now + ((changed & BTN_UP_BIT) ? 360 : 80);
    } else if ((changed & BTN_DOWN_BIT) != 0 ||
               (nav == BTN_DOWN_BIT && g_repeat_buttons == BTN_DOWN_BIT &&
                now >= g_next_repeat_ms)) {
        g_viewer.scroll++;
        g_repeat_buttons = BTN_DOWN_BIT;
        g_next_repeat_ms = now + ((changed & BTN_DOWN_BIT) ? 360 : 80);
    } else if (nav == 0) {
        g_repeat_buttons = 0;
    }

    text_viewer_clamp_scroll();
    finish_button_frame(buttons);
}

static void handle_buttons(uint32_t buttons)
{
    if (g_wait_buttons_release) {
        if (buttons == 0) {
            log_msg("post-launch buttons released");
            g_wait_buttons_release = 0;
            g_prev_buttons = 0;
        } else {
            if (buttons != g_prev_buttons) {
                log_msg("post-launch buttons still held: 0x%08x", buttons);
            }
            g_prev_buttons = buttons;
        }
        g_repeat_buttons = 0;
        return;
    }

    uint32_t changed = buttons & ~g_prev_buttons;
    uint32_t nav = buttons & (BTN_UP_BIT | BTN_DOWN_BIT);
    long now = now_ms();

    if (buttons != g_prev_buttons) {
        log_msg("buttons state=0x%08x changed_down=0x%08x changed_any=0x%08x",
                buttons, changed, buttons ^ g_prev_buttons);
    }

#if ENABLE_FN_ICUBE_SHORTCUT
    if ((buttons & BTN_FN_BIT) == 0) {
        if (!g_fn_shortcut_armed) {
            log_msg("FN shortcut armed after release");
        }
        g_fn_shortcut_armed = 1;
    } else if ((changed & BTN_FN_BIT) != 0) {
        if (g_fn_shortcut_armed) {
            launch_icube();
            g_prev_buttons = buttons;
            return;
        }
        log_msg("FN startup state ignored until release");
    }
#endif

    {
        uint32_t click_buttons = changed;
#if ENABLE_FN_ICUBE_SHORTCUT
        if (!g_fn_shortcut_armed) {
            click_buttons &= ~BTN_FN_BIT;
        }
#endif
        if (click_buttons != 0) {
            audio_click();
            audio_update();
        }
    }

    if (g_viewer.active) {
        handle_text_viewer_buttons(buttons, changed, nav, now);
        return;
    }

    if ((changed & BTN_UP_BIT) != 0 || (nav == BTN_UP_BIT && g_repeat_buttons == BTN_UP_BIT && now >= g_next_repeat_ms)) {
        if (g_selected > 0) {
            g_selected--;
        }
        g_repeat_buttons = BTN_UP_BIT;
        g_next_repeat_ms = now + ((changed & BTN_UP_BIT) ? 360 : 110);
    } else if ((changed & BTN_DOWN_BIT) != 0 || (nav == BTN_DOWN_BIT && g_repeat_buttons == BTN_DOWN_BIT && now >= g_next_repeat_ms)) {
        if (g_selected + 1 < g_entry_count) {
            g_selected++;
        }
        g_repeat_buttons = BTN_DOWN_BIT;
        g_next_repeat_ms = now + ((changed & BTN_DOWN_BIT) ? 360 : 110);
    } else if (nav == 0) {
        g_repeat_buttons = 0;
    }

    if ((changed & (BTN_LEFT_BIT | BTN_B_BIT | BTN_SELECT_BIT)) != 0) {
        save_dir_state(g_cwd);
        parent_path(g_cwd);
        scan_directory();
        finish_button_frame(buttons);
        return;
    }
    if ((changed & BTN_RIGHT_BIT) != 0) {
        if (g_entry_count > 0 && (g_entries[g_selected].is_dir || strcmp(g_entries[g_selected].name, "..") == 0)) {
            change_dir_to_selected();
            finish_button_frame(buttons);
            return;
        }
    }
    if ((changed & (BTN_A_BIT | BTN_START_BIT)) != 0) {
        if (!launch_selected()) {
            finish_button_frame(buttons);
        }
        return;
    }

    finish_button_frame(buttons);
}

int main(int argc, char **argv)
{
    log_open();
    log_msg("tiny_mc start argc=%d", argc);
    config_load();
    heartbeat_open();
    choose_start_dir(argc, argv);
    scan_directory();

    if (display_open() != 0) {
        fprintf(stderr, "tiny_mc: %s\n", g_status);
        log_msg("fatal display_open failed: %s", g_status);
        heartbeat_close();
        log_close();
        return 1;
    }
    font_open();
    input_open_devices();

    for (;;) {
        input_poll_devices();
        uint32_t buttons = input_buttons();
        handle_buttons(buttons);
        audio_update();
        draw_ui();
        heartbeat_tick();
        usleep(REFRESH_USEC);
    }

    display_close();
    input_close_devices();
    font_close();
    log_msg("tiny_mc exit");
    heartbeat_close();
    log_close();
    return 0;
}
