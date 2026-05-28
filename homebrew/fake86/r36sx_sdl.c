/*
 * Minimal SDL2 shim for Fake86 on the R36SX/SF3000-like firmware.
 *
 * The display path uses cubegm/driver.so just like rkgame.  Only the subset of
 * SDL that Fake86 touches is implemented here.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "SDL.h"

#include <dlfcn.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../common/hardware.h"

#define R36SX_FAKE86_LOG_PATH "/mnt/sdcard/MIPS_NATIVE/fake86/fake86.log"
#define R36SX_FAKE86_FALLBACK_LOG_PATH "/mnt/sdcard/fake86.log"
#define R36SX_FAKE86_FRAME_USEC 16666u

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);

struct SDL_Window {
    char title[128];
    int w;
    int h;
};

struct SDL_Renderer {
    SDL_Window *window;
    SDL_Texture *copy_texture;
    SDL_Rect copy_rect;
};

struct SDL_Texture {
    int w;
    int h;
    uint32_t *pixels;
};

struct SDL_mutex {
    pthread_mutex_t mutex;
};

struct SDL_Thread {
    pthread_t thread;
};

struct r36sx_display {
    void *handle;
    video_driver_setting_fn setting;
    video_drivers_init_fn init;
    video_driver_disp_frame_fn disp_frame;
    video_driver_deinit_fn deinit;
    cube_ioctl_fn cube_ioctl;
    uint16_t *frame;
    int active;
    uint64_t last_present_us;
    uint32_t cube_key_addr;
    volatile uint32_t *cube_key_mem;
};

struct r36sx_key_map {
    uint32_t raw_mask;
    SDL_Keycode key;
};

static char g_sdl_error[256] = "OK";
static struct r36sx_display g_display;
static uint32_t g_last_raw_keys = UINT32_MAX;
static uint32_t g_pending_changed;
static uint32_t g_pending_raw;
static int g_pending_down;
static int g_quit_sent;
static int g_relative_mouse;

static void r36sx_fake86_log(const char *format, ...)
{
    FILE *fp = fopen(R36SX_FAKE86_LOG_PATH, "a");
    if (!fp) {
        fp = fopen(R36SX_FAKE86_FALLBACK_LOG_PATH, "a");
    }
    if (fp) {
        struct timeval tv;
        va_list args;
        gettimeofday(&tv, NULL);
        fprintf(fp, "[%ld.%03ld] ", (long)tv.tv_sec,
                (long)(tv.tv_usec / 1000));
        va_start(args, format);
        vfprintf(fp, format, args);
        va_end(args);
        fputc('\n', fp);
        fclose(fp);
    }
}

void r36sx_fake86_debug_reset(void)
{
    unlink(R36SX_FAKE86_LOG_PATH);
    unlink(R36SX_FAKE86_FALLBACK_LOG_PATH);
    r36sx_fake86_log("log reset");
}

void r36sx_fake86_debug_log(const char *format, ...)
{
    FILE *fp = fopen(R36SX_FAKE86_LOG_PATH, "a");
    if (!fp) {
        fp = fopen(R36SX_FAKE86_FALLBACK_LOG_PATH, "a");
    }
    if (fp) {
        struct timeval tv;
        va_list args;
        gettimeofday(&tv, NULL);
        fprintf(fp, "[%ld.%03ld] ", (long)tv.tv_sec,
                (long)(tv.tv_usec / 1000));
        va_start(args, format);
        vfprintf(fp, format, args);
        va_end(args);
        fputc('\n', fp);
        fclose(fp);
    }
}

static void r36sx_set_error(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(g_sdl_error, sizeof(g_sdl_error), format, args);
    va_end(args);
    r36sx_fake86_log("SDL error: %s", g_sdl_error);
}

static uint64_t r36sx_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull +
           (uint64_t)ts.tv_nsec / 1000ull;
}

static uint16_t r36sx_argb8888_to_rgb565(uint32_t color)
{
    uint32_t r = (color >> 16) & 0xffu;
    uint32_t g = (color >> 8) & 0xffu;
    uint32_t b = color & 0xffu;
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static int r36sx_display_load(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
        g_display.handle = dlopen(paths[i], RTLD_NOW);
        if (g_display.handle) {
            r36sx_fake86_log("driver.so loaded: %s", paths[i]);
            break;
        }
        r36sx_fake86_log("driver.so failed: %s err=%s", paths[i], dlerror());
    }
    if (!g_display.handle) {
        r36sx_set_error("cannot load driver.so");
        return -1;
    }

    g_display.setting =
        (video_driver_setting_fn)dlsym(g_display.handle, "video_driver_setting");
    g_display.init =
        (video_drivers_init_fn)dlsym(g_display.handle, "video_drivers_init");
    g_display.disp_frame =
        (video_driver_disp_frame_fn)dlsym(g_display.handle,
                                          "video_driver_disp_frame");
    g_display.deinit =
        (video_driver_deinit_fn)dlsym(g_display.handle, "video_driver_deinit");
    g_display.cube_ioctl = (cube_ioctl_fn)dlsym(g_display.handle, "cube_ioctl");

    if (!g_display.setting || !g_display.init || !g_display.disp_frame ||
        !g_display.deinit) {
        r36sx_set_error("driver.so display symbols missing");
        return -1;
    }
    r36sx_fake86_log("driver symbols setting=%p init=%p disp=%p deinit=%p ioctl=%p",
                    g_display.setting, g_display.init, g_display.disp_frame,
                    g_display.deinit, g_display.cube_ioctl);
    return 0;
}

static int r36sx_display_open(void)
{
    if (g_display.active) {
        return 0;
    }
    memset(&g_display, 0, sizeof(g_display));
    g_display.frame = (uint16_t *)calloc(
        (size_t)R36SX_SCREEN_WIDTH * (size_t)R36SX_SCREEN_HEIGHT,
        sizeof(g_display.frame[0]));
    if (!g_display.frame) {
        r36sx_set_error("frame allocation failed");
        return -1;
    }
    if (r36sx_display_load() != 0) {
        return -1;
    }
    {
        int cfg[5] = {
            R36SX_DRIVER_SETTING_0,
            R36SX_DRIVER_SETTING_1,
            R36SX_DRIVER_SETTING_2,
            R36SX_DRIVER_SETTING_WIDTH,
            R36SX_DRIVER_SETTING_HEIGHT
        };
        g_display.setting(cfg);
    }
    {
        int rc = g_display.init();
        r36sx_fake86_log("video_drivers_init rc=%d", rc);
        if (rc < 0) {
            r36sx_set_error("video_drivers_init failed");
            return -1;
        }
    }
    if (g_display.cube_ioctl) {
        uint32_t addr = 0;
        int rc = g_display.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR,
                                      &addr, 0, 0);
        g_display.cube_key_addr = addr;
        g_display.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
        r36sx_fake86_log("joy shm rc=%d addr=0x%08x", rc, addr);
    }
    g_display.active = 1;
    return 0;
}

static uint32_t r36sx_read_raw_keys(void)
{
    uint32_t state = 0;

    if (!g_display.cube_ioctl) {
        return 0;
    }
    if (!g_display.cube_key_mem) {
        uint32_t addr = 0;
        g_display.cube_ioctl(R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR, &addr, 0, 0);
        g_display.cube_key_addr = addr;
        g_display.cube_key_mem = (volatile uint32_t *)(uintptr_t)addr;
    }
    if (!g_display.cube_key_mem) {
        return 0;
    }
    g_display.cube_ioctl(R36SX_CUBE_IOCTL_GET_GAME_STATUS, &state, 0, 0);
    (void)state;
    return g_display.cube_key_mem[0] | g_display.cube_key_mem[1];
}

static SDL_Keycode r36sx_key_for_raw(uint32_t raw_mask)
{
    static const struct r36sx_key_map map[] = {
        { R36SX_RKGAME_KEY_LEFT, SDLK_LEFT },
        { R36SX_RKGAME_KEY_RIGHT, SDLK_RIGHT },
        { R36SX_RKGAME_KEY_UP, SDLK_UP },
        { R36SX_RKGAME_KEY_DOWN, SDLK_DOWN },
        { R36SX_RKGAME_KEY_A, SDLK_RETURN },
        { R36SX_RKGAME_KEY_START, SDLK_RETURN },
        { R36SX_RKGAME_KEY_B, SDLK_ESCAPE },
        { R36SX_RKGAME_KEY_SELECT, SDLK_ESCAPE },
        { R36SX_RKGAME_KEY_X, SDLK_SPACE },
        { R36SX_RKGAME_KEY_Y, SDLK_LCTRL },
        { R36SX_RKGAME_KEY_L, SDLK_LALT },
        { R36SX_RKGAME_KEY_R, SDLK_LSHIFT },
        { R36SX_RKGAME_KEY_L2, SDLK_F1 },
        { R36SX_RKGAME_KEY_R2, SDLK_F2 },
        { R36SX_RKGAME_KEY_FN, SDLK_F3 }
    };

    for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (map[i].raw_mask == raw_mask) {
            return map[i].key;
        }
    }
    return 0;
}

static int r36sx_next_key_event(SDL_Event *event)
{
    uint32_t raw;

    if (g_pending_changed) {
        uint32_t bit = g_pending_changed & (~g_pending_changed + 1u);
        SDL_Keycode key = r36sx_key_for_raw(bit);
        g_pending_changed &= ~bit;
        if (!key) {
            return 0;
        }
        memset(event, 0, sizeof(*event));
        event->type = g_pending_down ? SDL_KEYDOWN : SDL_KEYUP;
        event->key.type = event->type;
        event->key.keysym.sym = key;
        return 1;
    }

    raw = r36sx_read_raw_keys();
    if ((raw & (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) ==
        (R36SX_RKGAME_KEY_SELECT | R36SX_RKGAME_KEY_START)) {
        if (!g_quit_sent) {
            g_quit_sent = 1;
            memset(event, 0, sizeof(*event));
            event->type = SDL_QUIT;
            r36sx_fake86_log("Select+Start -> SDL_QUIT");
            return 1;
        }
    } else {
        g_quit_sent = 0;
    }
    if (g_last_raw_keys == UINT32_MAX) {
        g_last_raw_keys = raw;
        return 0;
    }
    if (raw == g_last_raw_keys) {
        return 0;
    }

    g_pending_changed = raw ^ g_last_raw_keys;
    g_pending_raw = raw;
    g_pending_down = ((g_pending_changed & raw) != 0);
    if (!g_pending_down) {
        g_pending_changed &= g_last_raw_keys;
    } else {
        g_pending_changed &= raw;
    }
    g_last_raw_keys = raw;
    r36sx_fake86_log("raw keys=0x%08x", raw);
    return r36sx_next_key_event(event);
}

int SDL_Init(uint32_t flags)
{
    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);
    r36sx_fake86_log("SDL_Init flags=0x%08x", flags);
    return 0;
}

void SDL_Quit(void)
{
}

const char *SDL_GetError(void)
{
    return g_sdl_error;
}

void *SDL_malloc(size_t size)
{
    return malloc(size);
}

void SDL_free(void *ptr)
{
    free(ptr);
}

SDL_PixelFormat *SDL_AllocFormat(uint32_t format)
{
    SDL_PixelFormat *fmt = (SDL_PixelFormat *)calloc(1, sizeof(*fmt));
    if (fmt) {
        fmt->format = format;
        fmt->Rshift = 16;
        fmt->Gshift = 8;
        fmt->Bshift = 0;
        fmt->Ashift = 24;
    }
    return fmt;
}

uint32_t SDL_MapRGBA(SDL_PixelFormat *format, uint8_t r, uint8_t g,
                     uint8_t b, uint8_t a)
{
    (void)format;
    return ((uint32_t)a << 24) | ((uint32_t)r << 16) |
           ((uint32_t)g << 8) | (uint32_t)b;
}

char *SDL_GetBasePath(void)
{
    return strdup("/mnt/sdcard/MIPS_NATIVE/fake86/");
}

char *SDL_GetPrefPath(const char *org, const char *app)
{
    (void)org;
    (void)app;
    return strdup("/mnt/sdcard/MIPS_NATIVE/fake86/");
}

SDL_RWops *SDL_RWFromFile(const char *file, const char *mode)
{
    SDL_RWops *ops = (SDL_RWops *)calloc(1, sizeof(*ops));
    if (!ops) {
        r36sx_set_error("RWFromFile allocation failed");
        return NULL;
    }
    ops->fp = fopen(file, mode);
    if (!ops->fp) {
        r36sx_set_error("cannot open %s mode=%s", file, mode);
        free(ops);
        return NULL;
    }
    return ops;
}

size_t SDL_RWread(SDL_RWops *ctx, void *ptr, size_t size, size_t maxnum)
{
    return fread(ptr, size, maxnum, ctx->fp);
}

size_t SDL_RWwrite(SDL_RWops *ctx, const void *ptr, size_t size, size_t num)
{
    return fwrite(ptr, size, num, ctx->fp);
}

Sint64 SDL_RWsize(SDL_RWops *ctx)
{
    long cur = ftell(ctx->fp);
    long end;
    fseek(ctx->fp, 0, SEEK_END);
    end = ftell(ctx->fp);
    fseek(ctx->fp, cur, SEEK_SET);
    return (Sint64)end;
}

Sint64 SDL_RWseek(SDL_RWops *ctx, Sint64 offset, int whence)
{
    if (fseek(ctx->fp, (long)offset, whence) != 0) {
        return -1;
    }
    return (Sint64)ftell(ctx->fp);
}

Sint64 SDL_RWtell(SDL_RWops *ctx)
{
    return (Sint64)ftell(ctx->fp);
}

int SDL_RWclose(SDL_RWops *ctx)
{
    int rc = 0;
    if (!ctx) {
        return 0;
    }
    if (ctx->fp) {
        rc = fclose(ctx->fp);
    }
    free(ctx);
    return rc;
}

SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h,
                             uint32_t flags)
{
    SDL_Window *win;
    (void)x;
    (void)y;
    (void)flags;
    if (r36sx_display_open() != 0) {
        return NULL;
    }
    win = (SDL_Window *)calloc(1, sizeof(*win));
    if (!win) {
        r36sx_set_error("window allocation failed");
        return NULL;
    }
    snprintf(win->title, sizeof(win->title), "%s", title ? title : "");
    win->w = w;
    win->h = h;
    return win;
}

SDL_Renderer *SDL_CreateRenderer(SDL_Window *window, int index, uint32_t flags)
{
    SDL_Renderer *renderer;
    (void)index;
    (void)flags;
    renderer = (SDL_Renderer *)calloc(1, sizeof(*renderer));
    if (!renderer) {
        r36sx_set_error("renderer allocation failed");
        return NULL;
    }
    renderer->window = window;
    return renderer;
}

int SDL_RenderSetLogicalSize(SDL_Renderer *renderer, int w, int h)
{
    (void)renderer;
    (void)w;
    (void)h;
    return 0;
}

SDL_Texture *SDL_CreateTexture(SDL_Renderer *renderer, uint32_t format,
                               int access, int w, int h)
{
    SDL_Texture *tex;
    (void)renderer;
    (void)format;
    (void)access;
    tex = (SDL_Texture *)calloc(1, sizeof(*tex));
    if (!tex) {
        r36sx_set_error("texture allocation failed");
        return NULL;
    }
    tex->w = w;
    tex->h = h;
    tex->pixels = (uint32_t *)calloc((size_t)w * (size_t)h,
                                     sizeof(tex->pixels[0]));
    if (!tex->pixels) {
        free(tex);
        r36sx_set_error("texture pixels allocation failed");
        return NULL;
    }
    return tex;
}

int SDL_LockTexture(SDL_Texture *texture, const SDL_Rect *rect, void **pixels,
                    int *pitch)
{
    int x = rect ? rect->x : 0;
    int y = rect ? rect->y : 0;

    if (!texture || !texture->pixels || !pixels || !pitch) {
        r36sx_set_error("LockTexture bad argument");
        return -1;
    }
    *pixels = texture->pixels + (size_t)y * (size_t)texture->w + (size_t)x;
    *pitch = texture->w * (int)sizeof(texture->pixels[0]);
    return 0;
}

void SDL_UnlockTexture(SDL_Texture *texture)
{
    (void)texture;
}

int SDL_RenderClear(SDL_Renderer *renderer)
{
    (void)renderer;
    if (g_display.frame) {
        memset(g_display.frame, 0,
               (size_t)R36SX_SCREEN_WIDTH * (size_t)R36SX_SCREEN_HEIGHT *
                   sizeof(g_display.frame[0]));
    }
    return 0;
}

int SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture,
                   const SDL_Rect *src, const SDL_Rect *dst)
{
    (void)dst;
    if (!renderer || !texture) {
        return -1;
    }
    renderer->copy_texture = texture;
    if (src) {
        renderer->copy_rect = *src;
    } else {
        renderer->copy_rect.x = 0;
        renderer->copy_rect.y = 0;
        renderer->copy_rect.w = texture->w;
        renderer->copy_rect.h = texture->h;
    }
    return 0;
}

void SDL_RenderPresent(SDL_Renderer *renderer)
{
    SDL_Texture *tex;
    SDL_Rect src;
    int dst_w;
    int dst_h;
    int dst_x;
    int dst_y;
    uint64_t now;

    if (!renderer || !renderer->copy_texture || !g_display.active ||
        !g_display.disp_frame) {
        return;
    }
    tex = renderer->copy_texture;
    src = renderer->copy_rect;
    if (src.w <= 0 || src.h <= 0) {
        return;
    }

    dst_w = R36SX_SCREEN_WIDTH;
    dst_h = (src.h * dst_w) / src.w;
    if (dst_h > R36SX_SCREEN_HEIGHT) {
        dst_h = R36SX_SCREEN_HEIGHT;
        dst_w = (src.w * dst_h) / src.h;
    }
    dst_x = (R36SX_SCREEN_WIDTH - dst_w) / 2;
    dst_y = (R36SX_SCREEN_HEIGHT - dst_h) / 2;
    memset(g_display.frame, 0,
           (size_t)R36SX_SCREEN_WIDTH * (size_t)R36SX_SCREEN_HEIGHT *
               sizeof(g_display.frame[0]));

    for (int y = 0; y < dst_h; y++) {
        int sy = src.y + (y * src.h) / dst_h;
        uint16_t *dst_row = g_display.frame +
            (size_t)(dst_y + y) * (size_t)R36SX_SCREEN_WIDTH +
            (size_t)dst_x;
        for (int x = 0; x < dst_w; x++) {
            int sx = src.x + (x * src.w) / dst_w;
            dst_row[x] = r36sx_argb8888_to_rgb565(
                tex->pixels[(size_t)sy * (size_t)tex->w + (size_t)sx]);
        }
    }

    now = r36sx_now_us();
    if (g_display.last_present_us != 0 &&
        now - g_display.last_present_us < R36SX_FAKE86_FRAME_USEC) {
        usleep(1000);
    }
    g_display.disp_frame(g_display.frame, R36SX_SCREEN_WIDTH,
                         R36SX_SCREEN_HEIGHT, R36SX_RGB565_FRAME_STRIDE);
    g_display.last_present_us = now;
}

int SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect,
                      const void *pixels, int pitch)
{
    int x0 = rect ? rect->x : 0;
    int y0 = rect ? rect->y : 0;
    int w = rect ? rect->w : texture->w;
    int h = rect ? rect->h : texture->h;
    const uint8_t *src = (const uint8_t *)pixels;

    for (int y = 0; y < h; y++) {
        memcpy(texture->pixels + (size_t)(y0 + y) * texture->w + x0,
               src + (size_t)y * (size_t)pitch,
               (size_t)w * sizeof(uint32_t));
    }
    return 0;
}

int SDL_SetTextureBlendMode(SDL_Texture *texture, int blend_mode)
{
    (void)texture;
    (void)blend_mode;
    return 0;
}

int SDL_SetTextureAlphaMod(SDL_Texture *texture, uint8_t alpha)
{
    (void)texture;
    (void)alpha;
    return 0;
}

void SDL_SetWindowTitle(SDL_Window *window, const char *title)
{
    if (window && title) {
        snprintf(window->title, sizeof(window->title), "%s", title);
    }
}

SDL_mutex *SDL_CreateMutex(void)
{
    SDL_mutex *mutex = (SDL_mutex *)calloc(1, sizeof(*mutex));
    if (mutex) {
        pthread_mutex_init(&mutex->mutex, NULL);
    }
    return mutex;
}

int SDL_LockMutex(SDL_mutex *mutex)
{
    return mutex ? pthread_mutex_lock(&mutex->mutex) : -1;
}

int SDL_UnlockMutex(SDL_mutex *mutex)
{
    return mutex ? pthread_mutex_unlock(&mutex->mutex) : -1;
}

struct thread_start {
    int (*fn)(void *);
    void *data;
};

static void *r36sx_thread_entry(void *arg)
{
    struct thread_start *start = (struct thread_start *)arg;
    int (*fn)(void *) = start->fn;
    void *data = start->data;
    free(start);
    (void)fn(data);
    return NULL;
}

SDL_Thread *SDL_CreateThread(int (*fn)(void *), const char *name, void *data)
{
    SDL_Thread *thread;
    struct thread_start *start;
    (void)name;

    thread = (SDL_Thread *)calloc(1, sizeof(*thread));
    start = (struct thread_start *)calloc(1, sizeof(*start));
    if (!thread || !start) {
        free(thread);
        free(start);
        r36sx_set_error("thread allocation failed");
        return NULL;
    }
    start->fn = fn;
    start->data = data;
    if (pthread_create(&thread->thread, NULL, r36sx_thread_entry, start) != 0) {
        free(thread);
        free(start);
        r36sx_set_error("pthread_create failed");
        return NULL;
    }
    pthread_detach(thread->thread);
    return thread;
}

uint32_t SDL_GetTicks(void)
{
    return (uint32_t)(r36sx_now_us() / 1000u);
}

void SDL_Delay(uint32_t ms)
{
    usleep(ms * 1000u);
}

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
    if (obtained && desired) {
        *obtained = *desired;
    }
    r36sx_fake86_log("SDL_OpenAudio stub rate=%d channels=%u samples=%u",
                    desired ? desired->freq : 0,
                    desired ? desired->channels : 0,
                    desired ? desired->samples : 0);
    return -1;
}

void SDL_PauseAudio(int pause_on)
{
    (void)pause_on;
}

int SDL_PollEvent(SDL_Event *event)
{
    if (!event) {
        return 0;
    }
    return r36sx_next_key_event(event);
}

const char *SDL_GetKeyName(SDL_Keycode key)
{
    static char name[32];
    snprintf(name, sizeof(name), "key-%d", (int)key);
    return name;
}

int SDL_SetRelativeMouseMode(int enabled)
{
    g_relative_mouse = enabled != 0;
    return 0;
}

int SDL_GetRelativeMouseMode(void)
{
    return g_relative_mouse ? SDL_TRUE : SDL_FALSE;
}

uint32_t SDL_GetMouseState(int *x, int *y)
{
    if (x) {
        *x = 0;
    }
    if (y) {
        *y = 0;
    }
    return 0;
}

uint32_t SDL_GetRelativeMouseState(int *x, int *y)
{
    if (x) {
        *x = 0;
    }
    if (y) {
        *y = 0;
    }
    return 0;
}

int SDL_ShowCursor(int toggle)
{
    (void)toggle;
    return 0;
}
