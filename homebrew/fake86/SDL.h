/*
 * Minimal SDL2 compatibility header for the R36SX Fake86 native build.
 *
 * It only declares the SDL subset used by Fake86.  The implementation lives in
 * r36sx_sdl.c and routes display/input to cubegm/driver.so.
 */

#ifndef R36SX_FAKE86_SDL_H
#define R36SX_FAKE86_SDL_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t SDL_Keycode;
typedef int64_t Sint64;

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_PixelFormat {
    uint32_t format;
    uint8_t Rshift;
    uint8_t Gshift;
    uint8_t Bshift;
    uint8_t Ashift;
} SDL_PixelFormat;
typedef struct SDL_mutex SDL_mutex;
typedef struct SDL_Thread SDL_Thread;

typedef struct SDL_Rect {
    int x;
    int y;
    int w;
    int h;
} SDL_Rect;

typedef struct SDL_AudioSpec {
    int freq;
    uint16_t format;
    uint8_t channels;
    uint16_t samples;
    void (*callback)(void *userdata, uint8_t *stream, int len);
    void *userdata;
} SDL_AudioSpec;

typedef struct SDL_RWops {
    FILE *fp;
} SDL_RWops;

typedef struct SDL_Keysym {
    SDL_Keycode sym;
} SDL_Keysym;

typedef struct SDL_KeyboardEvent {
    uint32_t type;
    struct {
        SDL_Keycode sym;
    } keysym;
} SDL_KeyboardEvent;

typedef struct SDL_TextEditingEvent {
    uint32_t type;
    char text[32];
} SDL_TextEditingEvent;

typedef struct SDL_TextInputEvent {
    uint32_t type;
    char text[32];
} SDL_TextInputEvent;

typedef struct SDL_Event {
    uint32_t type;
    SDL_KeyboardEvent key;
    SDL_TextEditingEvent edit;
    SDL_TextInputEvent text;
} SDL_Event;

#define SDL_INIT_TIMER 0x00000001u
#define SDL_INIT_AUDIO 0x00000010u
#define SDL_INIT_VIDEO 0x00000020u

#define SDL_WINDOWPOS_UNDEFINED 0
#define SDL_WINDOW_SHOWN 0x00000004u
#define SDL_WINDOW_RESIZABLE 0x00000020u
#define SDL_RENDERER_ACCELERATED 0x00000002u
#define SDL_TEXTUREACCESS_STREAMING 1
#define SDL_PIXELFORMAT_ARGB8888 0x16362004u

#define SDL_FALSE 0
#define SDL_TRUE 1
#define SDL_DISABLE 0
#define SDL_ENABLE 1
#define SDL_GRAB_OFF 0
#define SDL_GRAB_ON 1
#define SDL_HWSURFACE 0
#define SDL_BLENDMODE_BLEND 1

#define AUDIO_U8 0x0008u

#define SDL_TEXTEDITING 0x0302u
#define SDL_TEXTINPUT 0x0303u
#define SDL_KEYDOWN 0x0300u
#define SDL_KEYUP 0x0301u
#define SDL_MOUSEMOTION 0x0400u
#define SDL_MOUSEBUTTONDOWN 0x0401u
#define SDL_MOUSEBUTTONUP 0x0402u
#define SDL_QUIT 0x0100u

#define RW_SEEK_SET SEEK_SET
#define RW_SEEK_CUR SEEK_CUR
#define RW_SEEK_END SEEK_END

#define SDLK_BACKSPACE 8
#define SDLK_TAB 9
#define SDLK_RETURN 13
#define SDLK_ESCAPE 27
#define SDLK_SPACE 32
#define SDLK_DELETE 127

#define SDLK_LCTRL 0x400000e0
#define SDLK_LSHIFT 0x400000e1
#define SDLK_LALT 0x400000e2
#define SDLK_RCTRL 0x400000e4
#define SDLK_RSHIFT 0x400000e5
#define SDLK_RALT 0x400000e6
#define SDLK_CAPSLOCK 0x40000039
#define SDLK_PRINTSCREEN 0x40000046
#define SDLK_SCROLLLOCK 0x40000047
#define SDLK_NUMLOCKCLEAR 0x40000053

#define SDLK_F1 0x4000003a
#define SDLK_F2 0x4000003b
#define SDLK_F3 0x4000003c
#define SDLK_F4 0x4000003d
#define SDLK_F5 0x4000003e
#define SDLK_F6 0x4000003f
#define SDLK_F7 0x40000040
#define SDLK_F8 0x40000041
#define SDLK_F9 0x40000042
#define SDLK_F10 0x40000043

#define SDLK_HOME 0x4000004a
#define SDLK_PAGEUP 0x4000004b
#define SDLK_END 0x4000004d
#define SDLK_PAGEDOWN 0x4000004e
#define SDLK_RIGHT 0x4000004f
#define SDLK_LEFT 0x40000050
#define SDLK_DOWN 0x40000051
#define SDLK_UP 0x40000052
#define SDLK_INSERT 0x40000049

#define SDLK_KP_ENTER 0x40000058
#define SDLK_RETURN2 0x4000009e
#define SDLK_KP_0 0x40000062
#define SDLK_KP_1 0x40000059
#define SDLK_KP_2 0x4000005a
#define SDLK_KP_3 0x4000005b
#define SDLK_KP_4 0x4000005c
#define SDLK_KP_5 0x4000005d
#define SDLK_KP_6 0x4000005e
#define SDLK_KP_7 0x4000005f
#define SDLK_KP_8 0x40000060
#define SDLK_KP_9 0x40000061
#define SDLK_KP_PERIOD 0x40000063
#define SDLK_KP_MINUS 0x40000056
#define SDLK_KP_PLUS 0x40000057

int SDL_Init(uint32_t flags);
void SDL_Quit(void);
const char *SDL_GetError(void);

void *SDL_malloc(size_t size);
void SDL_free(void *ptr);
SDL_PixelFormat *SDL_AllocFormat(uint32_t format);
uint32_t SDL_MapRGBA(SDL_PixelFormat *format, uint8_t r, uint8_t g,
                     uint8_t b, uint8_t a);

char *SDL_GetBasePath(void);
char *SDL_GetPrefPath(const char *org, const char *app);

SDL_RWops *SDL_RWFromFile(const char *file, const char *mode);
size_t SDL_RWread(SDL_RWops *ctx, void *ptr, size_t size, size_t maxnum);
size_t SDL_RWwrite(SDL_RWops *ctx, const void *ptr, size_t size, size_t num);
Sint64 SDL_RWsize(SDL_RWops *ctx);
Sint64 SDL_RWseek(SDL_RWops *ctx, Sint64 offset, int whence);
Sint64 SDL_RWtell(SDL_RWops *ctx);
int SDL_RWclose(SDL_RWops *ctx);

SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h,
                             uint32_t flags);
SDL_Renderer *SDL_CreateRenderer(SDL_Window *window, int index,
                                 uint32_t flags);
int SDL_RenderSetLogicalSize(SDL_Renderer *renderer, int w, int h);
SDL_Texture *SDL_CreateTexture(SDL_Renderer *renderer, uint32_t format,
                               int access, int w, int h);
int SDL_LockTexture(SDL_Texture *texture, const SDL_Rect *rect, void **pixels,
                    int *pitch);
void SDL_UnlockTexture(SDL_Texture *texture);
int SDL_RenderClear(SDL_Renderer *renderer);
int SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture,
                   const SDL_Rect *src, const SDL_Rect *dst);
void SDL_RenderPresent(SDL_Renderer *renderer);
int SDL_UpdateTexture(SDL_Texture *texture, const SDL_Rect *rect,
                      const void *pixels, int pitch);
int SDL_SetTextureBlendMode(SDL_Texture *texture, int blend_mode);
int SDL_SetTextureAlphaMod(SDL_Texture *texture, uint8_t alpha);
void SDL_SetWindowTitle(SDL_Window *window, const char *title);

SDL_mutex *SDL_CreateMutex(void);
int SDL_LockMutex(SDL_mutex *mutex);
int SDL_UnlockMutex(SDL_mutex *mutex);

SDL_Thread *SDL_CreateThread(int (*fn)(void *), const char *name, void *data);
uint32_t SDL_GetTicks(void);
void SDL_Delay(uint32_t ms);

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
void SDL_PauseAudio(int pause_on);

int SDL_PollEvent(SDL_Event *event);
const char *SDL_GetKeyName(SDL_Keycode key);
int SDL_SetRelativeMouseMode(int enabled);
int SDL_GetRelativeMouseMode(void);
uint32_t SDL_GetMouseState(int *x, int *y);
uint32_t SDL_GetRelativeMouseState(int *x, int *y);
int SDL_ShowCursor(int toggle);

#endif
