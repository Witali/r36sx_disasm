/*
 * Standalone native Pong for Tiny MC.
 *
 * This is a normal MIPS executable, not a libretro core. Tiny MC can launch it
 * directly with fork() + execl(). Display and built-in controls use the same
 * cubegm/driver.so path as Tiny MC and stock rkgame.
 */

#define _GNU_SOURCE

#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/hardware.h"
#include "../common/native_audio.h"

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    FB_WIDTH = R36SX_SCREEN_WIDTH,
    FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    PLAY_TOP = 30,
    PLAY_BOTTOM = 470,
    PLAY_LEFT = 12,
    PLAY_RIGHT = 628,
    PADDLE_W = 12,
    PADDLE_H = 76,
    PADDLE_MARGIN = 34,
    PADDLE_SPEED = 7,
    BALL_SIZE = 10,
    BALL_BASE_SPEED_X = 5,
    BALL_BASE_SPEED_Y = 3,
    SCORE_LIMIT = 9,
    SERVE_FRAMES = 40,
    FRAME_USEC = 16666,
    FONT_SCALE = 5,
    FONT_W = 5,
    FONT_H = 7,
    FONT_SPACING = 2
};

enum button_bits {
    BTN_UP_BIT = 1u << 0,
    BTN_DOWN_BIT = 1u << 1,
    BTN_A_BIT = 1u << 2,
    BTN_B_BIT = 1u << 3,
    BTN_X_BIT = 1u << 4,
    BTN_Y_BIT = 1u << 5,
    BTN_START_BIT = 1u << 6,
    BTN_SELECT_BIT = 1u << 7,
    BTN_FN_BIT = 1u << 8
};

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);

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

static struct driver_state g_driver;
static uint16_t g_frame[FB_WIDTH * FB_HEIGHT];
static int g_left_paddle_y;
static int g_right_paddle_y;
static int g_ball_x;
static int g_ball_y;
static int g_ball_vx;
static int g_ball_vy;
static unsigned g_left_score;
static unsigned g_right_score;
static unsigned g_serve_wait;
static unsigned g_frame_counter;
static int g_paused;
static int g_game_over;
static int g_player_won;
static uint32_t g_prev_buttons;
static struct r36sx_audio_state g_audio;

static void display_close(void);

static uint16_t rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static int clamp_int(int value, int min_value, int max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static void draw_rect(int x, int y, int w, int h, uint16_t color)
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
        uint16_t *row = &g_frame[(size_t)yy * FB_WIDTH];
        for (int xx = x; xx < x + w; xx++) {
            row[xx] = color;
        }
    }
}

static void fill_frame(uint16_t color)
{
    for (size_t i = 0; i < ARRAY_COUNT(g_frame); i++) {
        g_frame[i] = color;
    }
}

static uint8_t digit_segments(unsigned digit)
{
    static const uint8_t segments[10] = {
        0x3f, 0x06, 0x5b, 0x4f, 0x66,
        0x6d, 0x7d, 0x07, 0x7f, 0x6f
    };
    return segments[digit % 10u];
}

static void draw_digit(int x, int y, unsigned digit, uint16_t color)
{
    const int w = 46;
    const int h = 78;
    const int t = 8;
    uint8_t seg = digit_segments(digit);

    if ((seg & 0x01u) != 0) {
        draw_rect(x + t, y, w - t * 2, t, color);
    }
    if ((seg & 0x02u) != 0) {
        draw_rect(x + w - t, y + t, t, h / 2 - t, color);
    }
    if ((seg & 0x04u) != 0) {
        draw_rect(x + w - t, y + h / 2, t, h / 2 - t, color);
    }
    if ((seg & 0x08u) != 0) {
        draw_rect(x + t, y + h - t, w - t * 2, t, color);
    }
    if ((seg & 0x10u) != 0) {
        draw_rect(x, y + h / 2, t, h / 2 - t, color);
    }
    if ((seg & 0x20u) != 0) {
        draw_rect(x, y + t, t, h / 2 - t, color);
    }
    if ((seg & 0x40u) != 0) {
        draw_rect(x + t, y + h / 2 - t / 2, w - t * 2, t, color);
    }
}

static void draw_center_line(uint16_t color)
{
    for (int y = PLAY_TOP; y < PLAY_BOTTOM; y += 26) {
        draw_rect(FB_WIDTH / 2 - 2, y + 4, 4, 14, color);
    }
}

static void draw_pause_icon(uint16_t color)
{
    draw_rect(FB_WIDTH / 2 - 22, FB_HEIGHT / 2 - 28, 14, 56, color);
    draw_rect(FB_WIDTH / 2 + 8, FB_HEIGHT / 2 - 28, 14, 56, color);
}

static const uint8_t *glyph_rows(char ch)
{
    static const uint8_t glyph_e[FONT_H] = { 0x1f, 0x10, 0x10, 0x1e, 0x10, 0x10, 0x1f };
    static const uint8_t glyph_i[FONT_H] = { 0x1f, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1f };
    static const uint8_t glyph_l[FONT_H] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1f };
    static const uint8_t glyph_n[FONT_H] = { 0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11 };
    static const uint8_t glyph_o[FONT_H] = { 0x0e, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e };
    static const uint8_t glyph_s[FONT_H] = { 0x0f, 0x10, 0x10, 0x0e, 0x01, 0x01, 0x1e };
    static const uint8_t glyph_u[FONT_H] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e };
    static const uint8_t glyph_w[FONT_H] = { 0x11, 0x11, 0x11, 0x15, 0x15, 0x15, 0x0a };
    static const uint8_t glyph_y[FONT_H] = { 0x11, 0x11, 0x0a, 0x04, 0x04, 0x04, 0x04 };

    switch (ch) {
    case 'E':
        return glyph_e;
    case 'I':
        return glyph_i;
    case 'L':
        return glyph_l;
    case 'N':
        return glyph_n;
    case 'O':
        return glyph_o;
    case 'S':
        return glyph_s;
    case 'U':
        return glyph_u;
    case 'W':
        return glyph_w;
    case 'Y':
        return glyph_y;
    default:
        return NULL;
    }
}

static int text_width(const char *text, int scale)
{
    int width = 0;

    while (*text) {
        width += (*text == ' ') ? FONT_W * scale : FONT_W * scale;
        text++;
        if (*text) {
            width += FONT_SPACING * scale;
        }
    }

    return width;
}

static void draw_text(int x, int y, const char *text, int scale, uint16_t color)
{
    while (*text) {
        const uint8_t *rows = glyph_rows(*text);

        if (rows) {
            for (int row = 0; row < FONT_H; row++) {
                for (int col = 0; col < FONT_W; col++) {
                    if ((rows[row] & (1u << (FONT_W - 1 - col))) != 0) {
                        draw_rect(x + col * scale, y + row * scale, scale, scale, color);
                    }
                }
            }
        }

        x += (FONT_W + FONT_SPACING) * scale;
        text++;
    }
}

static void draw_centered_text(int y, const char *text, int scale, uint16_t color)
{
    draw_text((FB_WIDTH - text_width(text, scale)) / 2, y, text, scale, color);
}

static void draw_game_over(uint16_t white, uint16_t accent)
{
    const char *message = g_player_won ? "YOU WIN" : "YOU LOSE";
    int box_w = 330;
    int box_h = 104;
    int box_x = (FB_WIDTH - box_w) / 2;
    int box_y = (FB_HEIGHT - box_h) / 2;

    draw_rect(box_x, box_y, box_w, box_h, rgb565(0, 0, 0));
    draw_rect(box_x, box_y, box_w, 4, accent);
    draw_rect(box_x, box_y + box_h - 4, box_w, 4, accent);
    draw_rect(box_x, box_y, 4, box_h, accent);
    draw_rect(box_x + box_w - 4, box_y, 4, box_h, accent);
    draw_centered_text(box_y + 34, message, FONT_SCALE, white);
}

static void draw_frame(void)
{
    uint16_t black = rgb565(0, 0, 0);
    uint16_t dim = rgb565(45, 56, 64);
    uint16_t white = rgb565(232, 238, 232);
    uint16_t accent = rgb565(80, 170, 205);

    fill_frame(black);
    draw_rect(0, PLAY_TOP - 2, FB_WIDTH, 2, dim);
    draw_rect(0, PLAY_BOTTOM, FB_WIDTH, 2, dim);
    draw_rect(0, 0, FB_WIDTH, 1, white);
    draw_rect(0, FB_HEIGHT - 1, FB_WIDTH, 1, white);
    draw_rect(0, 0, 1, FB_HEIGHT, white);
    draw_rect(FB_WIDTH - 1, 0, 1, FB_HEIGHT, white);
    draw_center_line(dim);

    draw_digit(238, 52, g_left_score, white);
    draw_digit(356, 52, g_right_score, white);

    draw_rect(PADDLE_MARGIN, g_left_paddle_y, PADDLE_W, PADDLE_H, white);
    draw_rect(FB_WIDTH - PADDLE_MARGIN - PADDLE_W, g_right_paddle_y,
              PADDLE_W, PADDLE_H, white);

    if ((g_serve_wait & 8u) == 0 || g_serve_wait == 0) {
        draw_rect(g_ball_x, g_ball_y, BALL_SIZE, BALL_SIZE, white);
    }

    if (g_game_over) {
        draw_game_over(white, accent);
    }

    if (g_paused) {
        draw_pause_icon(accent);
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
        fprintf(stderr, "native_pong: cannot open driver.so: %s\n", dlerror());
        return -1;
    }

    g_driver.setting = (video_driver_setting_fn)dlsym(g_driver.handle, "video_driver_setting");
    g_driver.init = (video_drivers_init_fn)dlsym(g_driver.handle, "video_drivers_init");
    g_driver.disp_frame = (video_driver_disp_frame_fn)dlsym(g_driver.handle, "video_driver_disp_frame");
    g_driver.deinit = (video_driver_deinit_fn)dlsym(g_driver.handle, "video_driver_deinit");
    g_driver.cube_ioctl = (cube_ioctl_fn)dlsym(g_driver.handle, "cube_ioctl");
    if (!g_driver.setting || !g_driver.init || !g_driver.disp_frame || !g_driver.deinit) {
        fprintf(stderr, "native_pong: driver.so display symbols missing\n");
        display_close();
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
        g_driver.setting(cfg);
    }
    if (g_driver.init() < 0) {
        fprintf(stderr, "native_pong: video_drivers_init failed\n");
        display_close();
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
        g_driver.disp_frame(g_frame, FB_WIDTH, FB_HEIGHT, R36SX_RGB565_FRAME_STRIDE);
    }
}

static uint32_t translate_rkgame_keys(uint32_t raw)
{
    uint32_t buttons = 0;

    if ((raw & R36SX_RKGAME_KEY_UP) != 0) {
        buttons |= BTN_UP_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_DOWN) != 0) {
        buttons |= BTN_DOWN_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_A) != 0) {
        buttons |= BTN_A_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_B) != 0) {
        buttons |= BTN_B_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_X) != 0) {
        buttons |= BTN_X_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_Y) != 0) {
        buttons |= BTN_Y_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_START) != 0) {
        buttons |= BTN_START_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_SELECT) != 0) {
        buttons |= BTN_SELECT_BIT;
    }
    if ((raw & R36SX_RKGAME_KEY_FN) != 0) {
        buttons |= BTN_FN_BIT;
    }

    return buttons;
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
    return translate_rkgame_keys(g_driver.cube_key_mem[0]) |
           translate_rkgame_keys(g_driver.cube_key_mem[1]);
}

static void reset_round(int direction)
{
    g_ball_x = (FB_WIDTH - BALL_SIZE) / 2;
    g_ball_y = (PLAY_TOP + PLAY_BOTTOM - BALL_SIZE) / 2;
    g_ball_vx = direction < 0 ? -BALL_BASE_SPEED_X : BALL_BASE_SPEED_X;
    g_ball_vy = ((g_frame_counter + g_left_score + g_right_score) & 1u) != 0 ?
                -BALL_BASE_SPEED_Y : BALL_BASE_SPEED_Y;
    g_serve_wait = SERVE_FRAMES;
}

static void reset_game(void)
{
    g_left_paddle_y = (PLAY_TOP + PLAY_BOTTOM - PADDLE_H) / 2;
    g_right_paddle_y = g_left_paddle_y;
    g_left_score = 0;
    g_right_score = 0;
    g_paused = 0;
    g_game_over = 0;
    g_player_won = 0;
    g_prev_buttons = 0;
    reset_round(1);
}

static void move_paddles(uint32_t buttons)
{
    int manual_right_up;
    int manual_right_down;

    if ((buttons & BTN_UP_BIT) != 0) {
        g_left_paddle_y -= PADDLE_SPEED;
    }
    if ((buttons & BTN_DOWN_BIT) != 0) {
        g_left_paddle_y += PADDLE_SPEED;
    }

    manual_right_up = (buttons & (BTN_A_BIT | BTN_X_BIT)) != 0;
    manual_right_down = (buttons & (BTN_B_BIT | BTN_Y_BIT)) != 0;
    if (manual_right_up) {
        g_right_paddle_y -= PADDLE_SPEED;
    }
    if (manual_right_down) {
        g_right_paddle_y += PADDLE_SPEED;
    }

    if (!manual_right_up && !manual_right_down) {
        int target = g_ball_y + BALL_SIZE / 2;
        int center = g_right_paddle_y + PADDLE_H / 2;
        int ai_speed = 5;

        if (target < center - 8) {
            g_right_paddle_y -= ai_speed;
        } else if (target > center + 8) {
            g_right_paddle_y += ai_speed;
        }
    }

    g_left_paddle_y = clamp_int(g_left_paddle_y, PLAY_TOP, PLAY_BOTTOM - PADDLE_H);
    g_right_paddle_y = clamp_int(g_right_paddle_y, PLAY_TOP, PLAY_BOTTOM - PADDLE_H);
}

static int ball_overlaps_paddle(int paddle_x, int paddle_y)
{
    return g_ball_x < paddle_x + PADDLE_W &&
           g_ball_x + BALL_SIZE > paddle_x &&
           g_ball_y < paddle_y + PADDLE_H &&
           g_ball_y + BALL_SIZE > paddle_y;
}

static void update_ball(void)
{
    int left_x;
    int right_x;

    if (g_serve_wait != 0) {
        g_serve_wait--;
        return;
    }

    g_ball_x += g_ball_vx;
    g_ball_y += g_ball_vy;

    if (g_ball_y <= PLAY_TOP) {
        g_ball_y = PLAY_TOP;
        g_ball_vy = -g_ball_vy;
        r36sx_audio_play_bounce(&g_audio);
    } else if (g_ball_y + BALL_SIZE >= PLAY_BOTTOM) {
        g_ball_y = PLAY_BOTTOM - BALL_SIZE;
        g_ball_vy = -g_ball_vy;
        r36sx_audio_play_bounce(&g_audio);
    }

    left_x = PADDLE_MARGIN;
    right_x = FB_WIDTH - PADDLE_MARGIN - PADDLE_W;
    if (g_ball_vx < 0 && ball_overlaps_paddle(left_x, g_left_paddle_y)) {
        int paddle_center = g_left_paddle_y + PADDLE_H / 2;
        int ball_center = g_ball_y + BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        g_ball_x = left_x + PADDLE_W;
        g_ball_vx = BALL_BASE_SPEED_X + (int)((g_left_score + g_right_score) / 4);
        g_ball_vy = clamp_int(g_ball_vy + offset, -7, 7);
        if (g_ball_vy == 0) {
            g_ball_vy = offset < 0 ? -2 : 2;
        }
        r36sx_audio_play_bounce(&g_audio);
    } else if (g_ball_vx > 0 && ball_overlaps_paddle(right_x, g_right_paddle_y)) {
        int paddle_center = g_right_paddle_y + PADDLE_H / 2;
        int ball_center = g_ball_y + BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        g_ball_x = right_x - BALL_SIZE;
        g_ball_vx = -(BALL_BASE_SPEED_X + (int)((g_left_score + g_right_score) / 4));
        g_ball_vy = clamp_int(g_ball_vy + offset, -7, 7);
        if (g_ball_vy == 0) {
            g_ball_vy = offset < 0 ? -2 : 2;
        }
        r36sx_audio_play_bounce(&g_audio);
    }

    if (g_ball_x + BALL_SIZE < PLAY_LEFT) {
        if (g_right_score < SCORE_LIMIT) {
            g_right_score++;
        }
        if (g_right_score >= SCORE_LIMIT) {
            g_game_over = 1;
            g_player_won = 0;
            g_paused = 0;
            r36sx_audio_play_lose(&g_audio);
            return;
        }
        r36sx_audio_play_score(&g_audio);
        reset_round(-1);
    } else if (g_ball_x > PLAY_RIGHT) {
        if (g_left_score < SCORE_LIMIT) {
            g_left_score++;
        }
        if (g_left_score >= SCORE_LIMIT) {
            g_game_over = 1;
            g_player_won = 1;
            g_paused = 0;
            r36sx_audio_play_win(&g_audio);
            return;
        }
        r36sx_audio_play_score(&g_audio);
        reset_round(1);
    }
}

static void tick_game(uint32_t buttons)
{
    uint32_t pressed = buttons & ~g_prev_buttons;

    if (g_game_over) {
        if ((pressed & (BTN_A_BIT | BTN_START_BIT)) != 0) {
            reset_game();
            g_prev_buttons = buttons;
        }
        return;
    }

    if ((pressed & BTN_START_BIT) != 0) {
        g_paused = !g_paused;
        r36sx_audio_play_button(&g_audio);
    }
    if ((pressed & BTN_A_BIT) != 0 && g_paused) {
        r36sx_audio_play_button(&g_audio);
        reset_game();
    }

    if (!g_paused) {
        move_paddles(buttons);
        update_ball();
    }

    g_prev_buttons = buttons;
    g_frame_counter++;
}

int main(void)
{
    int running = 1;

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    if (display_open() != 0) {
        display_close();
        return 1;
    }
    r36sx_audio_init(&g_audio);

    reset_game();
    while (running) {
        uint32_t buttons = input_buttons();
        uint32_t pressed = buttons & ~g_prev_buttons;

        if ((pressed & (BTN_SELECT_BIT | BTN_FN_BIT)) != 0) {
            running = 0;
        } else {
            tick_game(buttons);
            draw_frame();
            present_frame();
            r36sx_audio_update(&g_audio);
            usleep(FRAME_USEC);
        }
    }

    r36sx_audio_close(&g_audio);
    display_close();
    return 0;
}
