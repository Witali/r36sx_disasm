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

#ifndef DEBUG
#define DEBUG 0
#endif

#define R36SX_PICO286_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_PICO286_FRAME_USEC 16666u

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
};

struct r36sx_key_map {
    uint32_t raw_mask;
    uint8_t keycode;
};

static struct r36sx_mfb_driver g_mfb;
static uint32_t g_palette[256];

extern void HandleInput(unsigned int keycode, int isKeyDown);

static void r36sx_mfb_log(const char *message)
{
#if DEBUG
    FILE *fp = fopen("/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.log", "a");
    if (fp) {
        fputs(message, fp);
        fputc('\n', fp);
        fclose(fp);
    }
#else
    (void)message;
#endif
}

static uint64_t r36sx_mfb_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static uint16_t r36sx_mfb_rgb888_to_rgb565(uint32_t color)
{
    uint32_t r = (color >> 16) & 0xffu;
    uint32_t g = (color >> 8) & 0xffu;
    uint32_t b = color & 0xffu;
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
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
            break;
        }
    }
    if (!g_mfb.handle) {
        r36sx_mfb_log("dlopen driver.so failed");
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
        r36sx_mfb_log("driver.so display symbols missing");
        return -1;
    }
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
        g_mfb.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_mfb.cube_key_addr = addr;
        g_mfb.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
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
    static const struct r36sx_key_map keys[] = {
        { R36SX_RKGAME_KEY_LEFT, 37 },
        { R36SX_RKGAME_KEY_UP, 38 },
        { R36SX_RKGAME_KEY_RIGHT, 39 },
        { R36SX_RKGAME_KEY_DOWN, 40 },
        { R36SX_RKGAME_KEY_A, 13 },
        { R36SX_RKGAME_KEY_B, 27 },
        { R36SX_RKGAME_KEY_X, 32 },
        { R36SX_RKGAME_KEY_Y, 17 },
        { R36SX_RKGAME_KEY_L, 18 },
        { R36SX_RKGAME_KEY_R, 16 },
        { R36SX_RKGAME_KEY_L2, 112 },
        { R36SX_RKGAME_KEY_R2, 113 },
        { R36SX_RKGAME_KEY_START, 13 },
        { R36SX_RKGAME_KEY_SELECT, 27 },
        { R36SX_RKGAME_KEY_FN, 114 }
    };
    uint8_t new_down[256];
    uint32_t raw = r36sx_mfb_read_raw_keys();

    if ((raw & (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) ==
        (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) {
        return -1;
    }

    memset(new_down, 0, sizeof(new_down));
    for (size_t i = 0; i < R36SX_PICO286_ARRAY_COUNT(keys); i++) {
        if ((raw & keys[i].raw_mask) != 0) {
            new_down[keys[i].keycode] = 1;
        }
    }

    for (size_t code = 0; code < sizeof(new_down); code++) {
        if (new_down[code] != g_mfb.key_down[code]) {
            HandleInput((unsigned int)code, new_down[code] != 0);
            g_mfb.key_down[code] = new_down[code];
        }
    }
    return 0;
}

int mfb_open(const char *name, int width, int height, int scale)
{
    (void)name;
    (void)scale;

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    memset(&g_mfb, 0, sizeof(g_mfb));
    g_mfb.width = width;
    g_mfb.height = height;
    g_mfb.stride = width * R36SX_RGB565_BYTES_PER_PIXEL;
    g_mfb.frame = (uint16_t *)calloc((size_t)width * (size_t)height,
                                     sizeof(g_mfb.frame[0]));
    if (!g_mfb.frame) {
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
        g_mfb.setting(cfg);
    }
    if (g_mfb.init() < 0) {
        r36sx_mfb_log("video_drivers_init failed");
        mfb_close();
        return 0;
    }

    g_mfb.active = 1;
    g_mfb.last_present_us = 0;
    r36sx_mfb_log("pico_286 MiniFB opened");
    return 1;
}

int mfb_update(void *buffer, int fps_limit)
{
    uint64_t now;
    uint32_t *src = (uint32_t *)buffer;
    (void)fps_limit;

    if (!g_mfb.active || !g_mfb.disp_frame || !src) {
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

    for (int y = 0; y < g_mfb.height; y++) {
        uint16_t *dst_row = g_mfb.frame + (size_t)y * (size_t)g_mfb.width;
        uint32_t *src_row = src + (size_t)y * (size_t)g_mfb.width;
        for (int x = 0; x < g_mfb.width; x++) {
            dst_row[x] = r36sx_mfb_rgb888_to_rgb565(src_row[x]);
        }
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
