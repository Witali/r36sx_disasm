#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "MiniFB.h"

extern void HandleInput(unsigned int keycode, int isKeyDown);
extern void HandleMouse(int x, int y, int buttons);

static HWND g_wnd;
static HDC g_hdc;
static BITMAPINFO g_bmi;
static uint32_t *g_frame32;
static int g_width;
static int g_height;
static int g_scale;
static int g_close_requested;
static char g_key_status[512];
static volatile uint32_t g_frame_generation;

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

void r36sx_mfb_mark_frame_ready(void)
{
    InterlockedIncrement((volatile LONG *)&g_frame_generation);
}

void r36sx_pico286_disk_activity(void)
{
    /* The PC debug host currently keeps overlays off and logs disk activity. */
}

void r36sx_pico286_post_code_out(uint16_t portnum, uint8_t value)
{
    (void)portnum;
    (void)value;
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
            HandleMouse((int)(short)LOWORD(l_param),
                        (int)(short)HIWORD(l_param), 0);
            return 0;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            int buttons = 0;
            if (w_param & MK_RBUTTON) {
                buttons |= 1;
            }
            if (w_param & MK_LBUTTON) {
                buttons |= 2;
            }
            HandleMouse((int)(short)LOWORD(l_param),
                        (int)(short)HIWORD(l_param), buttons);
            return 0;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            unsigned int key = r36sx_win_keycode(w_param, l_param);
            int is_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
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
    g_frame_generation = 0;

    rect.left = 0;
    rect.top = 0;
    rect.right = width * g_scale;
    rect.bottom = height * g_scale;
    AdjustWindowRect(&rect,
                     WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
                     FALSE);

    g_wnd = CreateWindowEx(0, wc.lpszClassName, title,
                           WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX &
                               ~WS_THICKFRAME,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           NULL, NULL, wc.hInstance, NULL);
    if (!g_wnd) {
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
    if (!g_frame32) {
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
    size_t pixels;
    (void)fps_limit;

    if (!g_wnd || !g_frame32 || !src || g_close_requested) {
        return -1;
    }

    pixels = (size_t)g_width * (size_t)g_height;
    for (size_t i = 0; i < pixels; ++i) {
        g_frame32[i] = r36sx_win_rgb565_to_rgb888(src[i]);
    }

    StretchDIBits(g_hdc, 0, 0, g_width * g_scale, g_height * g_scale,
                  0, 0, g_width, g_height,
                  g_frame32, &g_bmi, DIB_RGB_COLORS, SRCCOPY);

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
    if (g_hdc && g_wnd) {
        ReleaseDC(g_wnd, g_hdc);
    }
    g_hdc = NULL;
    free(g_frame32);
    g_frame32 = NULL;
    if (g_wnd) {
        DestroyWindow(g_wnd);
    }
    g_wnd = NULL;
}

char *mfb_keystatus(void)
{
    return g_key_status;
}
