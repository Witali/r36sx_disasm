/*
 * Shared Pong implementation for both libretro and native Tiny MC targets.
 *
 * The game state, framebuffer drawing, scoring, and button semantics live here.
 * Target-specific code is kept at the bottom: libretro callbacks for rkgame,
 * or a native main() using driver.so through Tiny MC.
 */

#define R36SX_PONG_TARGET_LIBRETRO 1
#define R36SX_PONG_TARGET_NATIVE 2

#ifndef R36SX_PONG_TARGET
#error "Define R36SX_PONG_TARGET when compiling homebrew/pong/pong.c"
#endif

#if R36SX_PONG_TARGET == R36SX_PONG_TARGET_LIBRETRO
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../common/libretro_hardware.h"
#elif R36SX_PONG_TARGET == R36SX_PONG_TARGET_NATIVE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/driver_audio.h"
#include "../common/hardware.h"
#else
#error "Unsupported R36SX_PONG_TARGET"
#endif

#define R36SX_PONG_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    R36SX_PONG_FB_WIDTH = R36SX_SCREEN_WIDTH,
    R36SX_PONG_FB_HEIGHT = R36SX_SCREEN_HEIGHT,
    R36SX_PONG_PLAY_TOP = 30,
    R36SX_PONG_PLAY_BOTTOM = 470,
    R36SX_PONG_PLAY_LEFT = 12,
    R36SX_PONG_PLAY_RIGHT = 628,
    R36SX_PONG_PADDLE_W = 12,
    R36SX_PONG_PADDLE_H = 76,
    R36SX_PONG_PADDLE_MARGIN = 34,
    R36SX_PONG_PADDLE_SPEED = 7,
    R36SX_PONG_BALL_SIZE = 10,
    R36SX_PONG_BALL_BASE_SPEED_X = 5,
    R36SX_PONG_BALL_BASE_SPEED_Y = 3,
    R36SX_PONG_SCORE_LIMIT = 9,
    R36SX_PONG_SERVE_FRAMES = 40,
    R36SX_PONG_BIG_FONT_SCALE = 13,
    R36SX_PONG_BIG_FONT_W = 5,
    R36SX_PONG_BIG_FONT_H = 5,
    R36SX_PONG_BIG_FONT_SPACING = 1,
    R36SX_PONG_BIG_FONT_SPACE_W = 3
};

enum r36sx_pong_button_bits {
    R36SX_PONG_BUTTON_UP = 1u << 0,
    R36SX_PONG_BUTTON_DOWN = 1u << 1,
    R36SX_PONG_BUTTON_A = 1u << 2,
    R36SX_PONG_BUTTON_B = 1u << 3,
    R36SX_PONG_BUTTON_X = 1u << 4,
    R36SX_PONG_BUTTON_Y = 1u << 5,
    R36SX_PONG_BUTTON_START = 1u << 6,
    R36SX_PONG_BUTTON_SELECT = 1u << 7,
    R36SX_PONG_BUTTON_FN = 1u << 8
};

enum r36sx_pong_audio_event {
    R36SX_PONG_AUDIO_NONE = 0,
    R36SX_PONG_AUDIO_BUTTON,
    R36SX_PONG_AUDIO_BOUNCE,
    R36SX_PONG_AUDIO_SCORE,
    R36SX_PONG_AUDIO_WIN,
    R36SX_PONG_AUDIO_LOSE
};

struct r36sx_pong_state {
    int left_paddle_y;
    int right_paddle_y;
    int ball_x;
    int ball_y;
    int ball_vx;
    int ball_vy;
    unsigned left_score;
    unsigned right_score;
    unsigned serve_wait;
    unsigned frame_counter;
    int paused;
    int game_over;
    int player_won;
    uint32_t prev_buttons;
    enum r36sx_pong_audio_event audio_event;
};

static uint16_t r36sx_pong_rgb565(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static int r36sx_pong_clamp_int(int value, int min_value, int max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

static void r36sx_pong_queue_audio(struct r36sx_pong_state *state,
                                   enum r36sx_pong_audio_event event)
{
    if (event != R36SX_PONG_AUDIO_NONE) {
        state->audio_event = event;
    }
}

static void r36sx_pong_reset_round(struct r36sx_pong_state *state,
                                   int direction)
{
    state->ball_x = (R36SX_PONG_FB_WIDTH - R36SX_PONG_BALL_SIZE) / 2;
    state->ball_y = (R36SX_PONG_PLAY_TOP + R36SX_PONG_PLAY_BOTTOM -
                     R36SX_PONG_BALL_SIZE) / 2;
    state->ball_vx = direction < 0 ? -R36SX_PONG_BALL_BASE_SPEED_X :
                                     R36SX_PONG_BALL_BASE_SPEED_X;
    state->ball_vy =
        ((state->frame_counter + state->left_score + state->right_score) &
         1u) != 0 ? -R36SX_PONG_BALL_BASE_SPEED_Y :
                    R36SX_PONG_BALL_BASE_SPEED_Y;
    state->serve_wait = R36SX_PONG_SERVE_FRAMES;
}

static void r36sx_pong_reset_game(struct r36sx_pong_state *state)
{
    state->left_paddle_y =
        (R36SX_PONG_PLAY_TOP + R36SX_PONG_PLAY_BOTTOM -
         R36SX_PONG_PADDLE_H) / 2;
    state->right_paddle_y = state->left_paddle_y;
    state->left_score = 0;
    state->right_score = 0;
    state->paused = 0;
    state->game_over = 0;
    state->player_won = 0;
    state->prev_buttons = 0;
    state->audio_event = R36SX_PONG_AUDIO_NONE;
    r36sx_pong_reset_round(state, 1);
}

static void r36sx_pong_move_paddles(struct r36sx_pong_state *state,
                                    uint32_t buttons)
{
    int manual_right_up;
    int manual_right_down;

    if ((buttons & R36SX_PONG_BUTTON_UP) != 0) {
        state->left_paddle_y -= R36SX_PONG_PADDLE_SPEED;
    }
    if ((buttons & R36SX_PONG_BUTTON_DOWN) != 0) {
        state->left_paddle_y += R36SX_PONG_PADDLE_SPEED;
    }

    manual_right_up = (buttons & (R36SX_PONG_BUTTON_A |
                                  R36SX_PONG_BUTTON_X)) != 0;
    manual_right_down = (buttons & (R36SX_PONG_BUTTON_B |
                                    R36SX_PONG_BUTTON_Y)) != 0;
    if (manual_right_up) {
        state->right_paddle_y -= R36SX_PONG_PADDLE_SPEED;
    }
    if (manual_right_down) {
        state->right_paddle_y += R36SX_PONG_PADDLE_SPEED;
    }

    if (!manual_right_up && !manual_right_down) {
        int target = state->ball_y + R36SX_PONG_BALL_SIZE / 2;
        int center = state->right_paddle_y + R36SX_PONG_PADDLE_H / 2;
        int ai_speed = 5;

        if (target < center - 8) {
            state->right_paddle_y -= ai_speed;
        } else if (target > center + 8) {
            state->right_paddle_y += ai_speed;
        }
    }

    state->left_paddle_y =
        r36sx_pong_clamp_int(state->left_paddle_y,
                             R36SX_PONG_PLAY_TOP,
                             R36SX_PONG_PLAY_BOTTOM -
                             R36SX_PONG_PADDLE_H);
    state->right_paddle_y =
        r36sx_pong_clamp_int(state->right_paddle_y,
                             R36SX_PONG_PLAY_TOP,
                             R36SX_PONG_PLAY_BOTTOM -
                             R36SX_PONG_PADDLE_H);
}

static int r36sx_pong_ball_overlaps_paddle(
    const struct r36sx_pong_state *state, int paddle_x, int paddle_y)
{
    return state->ball_x < paddle_x + R36SX_PONG_PADDLE_W &&
           state->ball_x + R36SX_PONG_BALL_SIZE > paddle_x &&
           state->ball_y < paddle_y + R36SX_PONG_PADDLE_H &&
           state->ball_y + R36SX_PONG_BALL_SIZE > paddle_y;
}

static void r36sx_pong_update_ball(struct r36sx_pong_state *state)
{
    int left_x;
    int right_x;

    if (state->serve_wait != 0) {
        state->serve_wait--;
        return;
    }

    state->ball_x += state->ball_vx;
    state->ball_y += state->ball_vy;

    if (state->ball_y <= R36SX_PONG_PLAY_TOP) {
        state->ball_y = R36SX_PONG_PLAY_TOP;
        state->ball_vy = -state->ball_vy;
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BOUNCE);
    } else if (state->ball_y + R36SX_PONG_BALL_SIZE >=
               R36SX_PONG_PLAY_BOTTOM) {
        state->ball_y = R36SX_PONG_PLAY_BOTTOM - R36SX_PONG_BALL_SIZE;
        state->ball_vy = -state->ball_vy;
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BOUNCE);
    }

    left_x = R36SX_PONG_PADDLE_MARGIN;
    right_x = R36SX_PONG_FB_WIDTH - R36SX_PONG_PADDLE_MARGIN -
              R36SX_PONG_PADDLE_W;
    if (state->ball_vx < 0 &&
        r36sx_pong_ball_overlaps_paddle(state, left_x,
                                        state->left_paddle_y)) {
        int paddle_center = state->left_paddle_y + R36SX_PONG_PADDLE_H / 2;
        int ball_center = state->ball_y + R36SX_PONG_BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        state->ball_x = left_x + R36SX_PONG_PADDLE_W;
        state->ball_vx = R36SX_PONG_BALL_BASE_SPEED_X +
                         (int)((state->left_score + state->right_score) / 4);
        state->ball_vy = r36sx_pong_clamp_int(state->ball_vy + offset,
                                              -7, 7);
        if (state->ball_vy == 0) {
            state->ball_vy = offset < 0 ? -2 : 2;
        }
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BOUNCE);
    } else if (state->ball_vx > 0 &&
               r36sx_pong_ball_overlaps_paddle(state, right_x,
                                               state->right_paddle_y)) {
        int paddle_center = state->right_paddle_y + R36SX_PONG_PADDLE_H / 2;
        int ball_center = state->ball_y + R36SX_PONG_BALL_SIZE / 2;
        int offset = (ball_center - paddle_center) / 10;

        state->ball_x = right_x - R36SX_PONG_BALL_SIZE;
        state->ball_vx = -(R36SX_PONG_BALL_BASE_SPEED_X +
                           (int)((state->left_score +
                                  state->right_score) / 4));
        state->ball_vy = r36sx_pong_clamp_int(state->ball_vy + offset,
                                              -7, 7);
        if (state->ball_vy == 0) {
            state->ball_vy = offset < 0 ? -2 : 2;
        }
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BOUNCE);
    }

    if (state->ball_x + R36SX_PONG_BALL_SIZE < R36SX_PONG_PLAY_LEFT) {
        if (state->right_score < R36SX_PONG_SCORE_LIMIT) {
            state->right_score++;
        }
        if (state->right_score >= R36SX_PONG_SCORE_LIMIT) {
            state->game_over = 1;
            state->player_won = 0;
            state->paused = 0;
            r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_LOSE);
            return;
        }
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_SCORE);
        r36sx_pong_reset_round(state, -1);
    } else if (state->ball_x > R36SX_PONG_PLAY_RIGHT) {
        if (state->left_score < R36SX_PONG_SCORE_LIMIT) {
            state->left_score++;
        }
        if (state->left_score >= R36SX_PONG_SCORE_LIMIT) {
            state->game_over = 1;
            state->player_won = 1;
            state->paused = 0;
            r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_WIN);
            return;
        }
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_SCORE);
        r36sx_pong_reset_round(state, 1);
    }
}

static void r36sx_pong_tick(struct r36sx_pong_state *state, uint32_t buttons)
{
    uint32_t pressed = buttons & ~state->prev_buttons;

    state->audio_event = R36SX_PONG_AUDIO_NONE;

    if ((pressed & R36SX_PONG_BUTTON_SELECT) != 0) {
        r36sx_pong_reset_game(state);
        state->prev_buttons = buttons;
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BUTTON);
        return;
    }

    if (state->game_over) {
        if ((pressed & (R36SX_PONG_BUTTON_A |
                        R36SX_PONG_BUTTON_START)) != 0) {
            r36sx_pong_reset_game(state);
            state->prev_buttons = buttons;
            r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BUTTON);
        }
        return;
    }

    if ((pressed & R36SX_PONG_BUTTON_START) != 0) {
        state->paused = !state->paused;
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BUTTON);
    }
    if ((pressed & R36SX_PONG_BUTTON_A) != 0 && state->paused) {
        r36sx_pong_reset_game(state);
        state->prev_buttons = buttons;
        r36sx_pong_queue_audio(state, R36SX_PONG_AUDIO_BUTTON);
        return;
    }

    if (!state->paused) {
        r36sx_pong_move_paddles(state, buttons);
        r36sx_pong_update_ball(state);
    }

    state->prev_buttons = buttons;
    state->frame_counter++;
}

static void r36sx_pong_draw_rect(uint16_t *frame, int x, int y, int w, int h,
                                 uint16_t color)
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
    if (x + w > R36SX_PONG_FB_WIDTH) {
        w = R36SX_PONG_FB_WIDTH - x;
    }
    if (y + h > R36SX_PONG_FB_HEIGHT) {
        h = R36SX_PONG_FB_HEIGHT - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int yy = y; yy < y + h; yy++) {
        uint16_t *row = &frame[(size_t)yy * R36SX_PONG_FB_WIDTH];
        for (int xx = x; xx < x + w; xx++) {
            row[xx] = color;
        }
    }
}

static void r36sx_pong_fill_frame(uint16_t *frame, uint16_t color)
{
    for (size_t i = 0;
         i < (size_t)R36SX_PONG_FB_WIDTH * (size_t)R36SX_PONG_FB_HEIGHT;
         i++) {
        frame[i] = color;
    }
}

static uint8_t r36sx_pong_digit_segments(unsigned digit)
{
    static const uint8_t segments[10] = {
        0x3f, 0x06, 0x5b, 0x4f, 0x66,
        0x6d, 0x7d, 0x07, 0x7f, 0x6f
    };
    return segments[digit % 10u];
}

static void r36sx_pong_draw_digit(uint16_t *frame, int x, int y,
                                  unsigned digit, uint16_t color)
{
    const int w = 46;
    const int h = 78;
    const int t = 8;
    uint8_t seg = r36sx_pong_digit_segments(digit);

    if ((seg & 0x01u) != 0) {
        r36sx_pong_draw_rect(frame, x + t, y, w - t * 2, t, color);
    }
    if ((seg & 0x02u) != 0) {
        r36sx_pong_draw_rect(frame, x + w - t, y + t, t,
                             h / 2 - t, color);
    }
    if ((seg & 0x04u) != 0) {
        r36sx_pong_draw_rect(frame, x + w - t, y + h / 2, t,
                             h / 2 - t, color);
    }
    if ((seg & 0x08u) != 0) {
        r36sx_pong_draw_rect(frame, x + t, y + h - t, w - t * 2, t,
                             color);
    }
    if ((seg & 0x10u) != 0) {
        r36sx_pong_draw_rect(frame, x, y + h / 2, t, h / 2 - t, color);
    }
    if ((seg & 0x20u) != 0) {
        r36sx_pong_draw_rect(frame, x, y + t, t, h / 2 - t, color);
    }
    if ((seg & 0x40u) != 0) {
        r36sx_pong_draw_rect(frame, x + t, y + h / 2 - t / 2,
                             w - t * 2, t, color);
    }
}

static void r36sx_pong_draw_center_line(uint16_t *frame, uint16_t color)
{
    for (int y = R36SX_PONG_PLAY_TOP; y < R36SX_PONG_PLAY_BOTTOM; y += 26) {
        r36sx_pong_draw_rect(frame, R36SX_PONG_FB_WIDTH / 2 - 2,
                             y + 4, 4, 14, color);
    }
}

static void r36sx_pong_draw_pause_icon(uint16_t *frame, uint16_t color)
{
    r36sx_pong_draw_rect(frame, R36SX_PONG_FB_WIDTH / 2 - 22,
                         R36SX_PONG_FB_HEIGHT / 2 - 28, 14, 56, color);
    r36sx_pong_draw_rect(frame, R36SX_PONG_FB_WIDTH / 2 + 8,
                         R36SX_PONG_FB_HEIGHT / 2 - 28, 14, 56, color);
}

static const uint8_t *r36sx_pong_big_glyph_rows(char ch, int *width)
{
    static const uint8_t glyph_y[R36SX_PONG_BIG_FONT_H] =
        { 0x11, 0x0a, 0x04, 0x04, 0x04 };
    static const uint8_t glyph_o[R36SX_PONG_BIG_FONT_H] =
        { 0x0e, 0x11, 0x11, 0x11, 0x0e };
    static const uint8_t glyph_u[R36SX_PONG_BIG_FONT_H] =
        { 0x11, 0x11, 0x11, 0x13, 0x0d };
    static const uint8_t glyph_w[R36SX_PONG_BIG_FONT_H] =
        { 0x11, 0x11, 0x15, 0x15, 0x0a };
    static const uint8_t glyph_i[R36SX_PONG_BIG_FONT_H] =
        { 0x1c, 0x08, 0x08, 0x08, 0x1c };
    static const uint8_t glyph_n[R36SX_PONG_BIG_FONT_H] =
        { 0x16, 0x19, 0x11, 0x11, 0x11 };
    static const uint8_t glyph_l[R36SX_PONG_BIG_FONT_H] =
        { 0x10, 0x10, 0x10, 0x10, 0x1f };
    static const uint8_t glyph_s[R36SX_PONG_BIG_FONT_H] =
        { 0x0f, 0x10, 0x0e, 0x01, 0x1e };
    static const uint8_t glyph_e[R36SX_PONG_BIG_FONT_H] =
        { 0x0e, 0x11, 0x1e, 0x10, 0x0f };

    *width = R36SX_PONG_BIG_FONT_W;
    switch (ch) {
    case 'Y':
    case 'y':
        return glyph_y;
    case 'O':
    case 'o':
        return glyph_o;
    case 'U':
    case 'u':
        return glyph_u;
    case 'W':
    case 'w':
        return glyph_w;
    case 'I':
    case 'i':
        *width = 3;
        return glyph_i;
    case 'N':
    case 'n':
        return glyph_n;
    case 'L':
    case 'l':
        return glyph_l;
    case 'S':
    case 's':
        return glyph_s;
    case 'E':
    case 'e':
        return glyph_e;
    default:
        return NULL;
    }
}

static int r36sx_pong_big_text_width(const char *text)
{
    int width = 0;

    while (*text) {
        int glyph_width = R36SX_PONG_BIG_FONT_W;

        if (*text == ' ') {
            glyph_width = R36SX_PONG_BIG_FONT_SPACE_W;
        } else {
            (void)r36sx_pong_big_glyph_rows(*text, &glyph_width);
        }

        width += glyph_width * R36SX_PONG_BIG_FONT_SCALE;
        text++;
        if (*text) {
            width += R36SX_PONG_BIG_FONT_SPACING *
                     R36SX_PONG_BIG_FONT_SCALE;
        }
    }

    return width;
}

static void r36sx_pong_draw_big_text(uint16_t *frame, int x, int y,
                                     const char *text, uint16_t color)
{
    while (*text) {
        int glyph_width = R36SX_PONG_BIG_FONT_W;
        const uint8_t *rows = r36sx_pong_big_glyph_rows(*text, &glyph_width);

        if (*text == ' ') {
            glyph_width = R36SX_PONG_BIG_FONT_SPACE_W;
        } else if (rows) {
            for (int row = 0; row < R36SX_PONG_BIG_FONT_H; row++) {
                for (int col = 0; col < glyph_width; col++) {
                    if ((rows[row] &
                         (1u << (R36SX_PONG_BIG_FONT_W - 1 - col))) != 0) {
                        r36sx_pong_draw_rect(frame,
                                             x + col *
                                             R36SX_PONG_BIG_FONT_SCALE,
                                             y + row *
                                             R36SX_PONG_BIG_FONT_SCALE,
                                             R36SX_PONG_BIG_FONT_SCALE,
                                             R36SX_PONG_BIG_FONT_SCALE,
                                             color);
                    }
                }
            }
        }

        x += (glyph_width + R36SX_PONG_BIG_FONT_SPACING) *
             R36SX_PONG_BIG_FONT_SCALE;
        text++;
    }
}

static void r36sx_pong_draw_centered_big_text(uint16_t *frame, int y,
                                              const char *text,
                                              uint16_t color)
{
    r36sx_pong_draw_big_text(frame,
                             (R36SX_PONG_FB_WIDTH -
                              r36sx_pong_big_text_width(text)) / 2,
                             y, text, color);
}

static void r36sx_pong_draw_game_over(const struct r36sx_pong_state *state,
                                      uint16_t *frame, uint16_t white,
                                      uint16_t accent)
{
    const char *message = state->player_won ? "You win" : "You lose";
    int text_h = R36SX_PONG_BIG_FONT_H * R36SX_PONG_BIG_FONT_SCALE;
    int box_w = r36sx_pong_big_text_width(message) + 48;
    int box_h = text_h + 56;
    int box_x = (R36SX_PONG_FB_WIDTH - box_w) / 2;
    int box_y = (R36SX_PONG_FB_HEIGHT - box_h) / 2;

    r36sx_pong_draw_rect(frame, box_x, box_y, box_w, box_h,
                         r36sx_pong_rgb565(0, 0, 0));
    r36sx_pong_draw_rect(frame, box_x, box_y, box_w, 4, accent);
    r36sx_pong_draw_rect(frame, box_x, box_y + box_h - 4, box_w, 4,
                         accent);
    r36sx_pong_draw_rect(frame, box_x, box_y, 4, box_h, accent);
    r36sx_pong_draw_rect(frame, box_x + box_w - 4, box_y, 4, box_h,
                         accent);
    r36sx_pong_draw_centered_big_text(frame, box_y + (box_h - text_h) / 2,
                                      message, white);
}

static void r36sx_pong_draw_frame(const struct r36sx_pong_state *state,
                                  uint16_t *frame)
{
    uint16_t black = r36sx_pong_rgb565(0, 0, 0);
    uint16_t dim = r36sx_pong_rgb565(45, 56, 64);
    uint16_t white = r36sx_pong_rgb565(232, 238, 232);
    uint16_t accent = r36sx_pong_rgb565(80, 170, 205);

    r36sx_pong_fill_frame(frame, black);
    r36sx_pong_draw_rect(frame, 0, R36SX_PONG_PLAY_TOP - 2,
                         R36SX_PONG_FB_WIDTH, 2, dim);
    r36sx_pong_draw_rect(frame, 0, R36SX_PONG_PLAY_BOTTOM,
                         R36SX_PONG_FB_WIDTH, 2, dim);
    r36sx_pong_draw_rect(frame, 0, 0, R36SX_PONG_FB_WIDTH, 1, white);
    r36sx_pong_draw_rect(frame, 0, R36SX_PONG_FB_HEIGHT - 1,
                         R36SX_PONG_FB_WIDTH, 1, white);
    r36sx_pong_draw_rect(frame, 0, 0, 1, R36SX_PONG_FB_HEIGHT, white);
    r36sx_pong_draw_rect(frame, R36SX_PONG_FB_WIDTH - 1, 0, 1,
                         R36SX_PONG_FB_HEIGHT, white);
    r36sx_pong_draw_center_line(frame, dim);

    r36sx_pong_draw_digit(frame, 238, 52, state->left_score, white);
    r36sx_pong_draw_digit(frame, 356, 52, state->right_score, white);

    r36sx_pong_draw_rect(frame, R36SX_PONG_PADDLE_MARGIN,
                         state->left_paddle_y,
                         R36SX_PONG_PADDLE_W, R36SX_PONG_PADDLE_H, white);
    r36sx_pong_draw_rect(frame, R36SX_PONG_FB_WIDTH -
                         R36SX_PONG_PADDLE_MARGIN -
                         R36SX_PONG_PADDLE_W,
                         state->right_paddle_y,
                         R36SX_PONG_PADDLE_W, R36SX_PONG_PADDLE_H, white);

    if ((state->serve_wait & 8u) == 0 || state->serve_wait == 0) {
        r36sx_pong_draw_rect(frame, state->ball_x, state->ball_y,
                             R36SX_PONG_BALL_SIZE, R36SX_PONG_BALL_SIZE,
                             white);
    }

    if (state->game_over) {
        r36sx_pong_draw_game_over(state, frame, white, accent);
    }

    if (state->paused) {
        r36sx_pong_draw_pause_icon(frame, accent);
    }
}

#if R36SX_PONG_TARGET == R36SX_PONG_TARGET_LIBRETRO

#define RETRO_API_VERSION 1

enum retro_device {
    RETRO_DEVICE_JOYPAD = 1
};

enum retro_device_id_joypad {
    RETRO_DEVICE_ID_JOYPAD_B = 0,
    RETRO_DEVICE_ID_JOYPAD_Y = 1,
    RETRO_DEVICE_ID_JOYPAD_SELECT = 2,
    RETRO_DEVICE_ID_JOYPAD_START = 3,
    RETRO_DEVICE_ID_JOYPAD_UP = 4,
    RETRO_DEVICE_ID_JOYPAD_DOWN = 5,
    RETRO_DEVICE_ID_JOYPAD_LEFT = 6,
    RETRO_DEVICE_ID_JOYPAD_RIGHT = 7,
    RETRO_DEVICE_ID_JOYPAD_A = 8,
    RETRO_DEVICE_ID_JOYPAD_X = 9,
    RETRO_DEVICE_ID_JOYPAD_L = 10,
    RETRO_DEVICE_ID_JOYPAD_R = 11
};

struct retro_game_info {
    const char *path;
    const void *data;
    size_t size;
    const char *meta;
};

struct retro_system_info {
    const char *library_name;
    const char *library_version;
    const char *valid_extensions;
    bool need_fullpath;
    bool block_extract;
};

struct retro_game_geometry {
    unsigned base_width;
    unsigned base_height;
    unsigned max_width;
    unsigned max_height;
    float aspect_ratio;
};

struct retro_system_timing {
    double fps;
    double sample_rate;
};

struct retro_system_av_info {
    struct retro_game_geometry geometry;
    struct retro_system_timing timing;
};

typedef bool (*retro_environment_t)(unsigned cmd, void *data);
typedef void (*retro_video_refresh_t)(const void *data, unsigned width,
                                      unsigned height, size_t pitch);
typedef void (*retro_audio_sample_t)(int16_t left, int16_t right);
typedef size_t (*retro_audio_sample_batch_t)(const int16_t *data,
                                             size_t frames);
typedef void (*retro_input_poll_t)(void);
typedef int16_t (*retro_input_state_t)(unsigned port, unsigned device,
                                       unsigned index, unsigned id);

enum {
    R36SX_PONG_AUDIO_RATE = R36SX_LIBRETRO_AUDIO_SAMPLE_RATE,
    R36SX_PONG_AUDIO_FRAMES_PER_VIDEO_FRAME =
        R36SX_LIBRETRO_AUDIO_FRAMES_PER_VIDEO_FRAME
};

static retro_environment_t environ_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_sample_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static struct r36sx_pong_state pong_state;
static uint16_t frame[R36SX_PONG_FB_WIDTH * R36SX_PONG_FB_HEIGHT];
static int16_t audio_frame[R36SX_PONG_AUDIO_FRAMES_PER_VIDEO_FRAME * 2];
static uint32_t tone_phase;
static uint32_t tone_step;
static uint32_t tone_remaining;
static uint32_t tone_total;

static void r36sx_pong_zero_memory(void *ptr, size_t size)
{
    uint8_t *bytes = (uint8_t *)ptr;

    for (size_t i = 0; i < size; i++) {
        bytes[i] = 0;
    }
}

static bool r36sx_pong_retro_pressed(unsigned id)
{
    if (!input_state_cb) {
        return false;
    }
    return input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, id) != 0;
}

static uint32_t r36sx_pong_retro_read_buttons(void)
{
    uint32_t buttons = 0;

    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_UP)) {
        buttons |= R36SX_PONG_BUTTON_UP;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_DOWN)) {
        buttons |= R36SX_PONG_BUTTON_DOWN;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_A)) {
        buttons |= R36SX_PONG_BUTTON_A;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_B)) {
        buttons |= R36SX_PONG_BUTTON_B;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_X)) {
        buttons |= R36SX_PONG_BUTTON_X;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_Y)) {
        buttons |= R36SX_PONG_BUTTON_Y;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_START)) {
        buttons |= R36SX_PONG_BUTTON_START;
    }
    if (r36sx_pong_retro_pressed(RETRO_DEVICE_ID_JOYPAD_SELECT)) {
        buttons |= R36SX_PONG_BUTTON_SELECT;
    }

    return buttons;
}

static void r36sx_pong_retro_trigger_tone(unsigned freq, unsigned samples)
{
    tone_phase = 0;
    tone_step = (freq << 16) / R36SX_PONG_AUDIO_RATE;
    tone_remaining = samples;
    tone_total = samples;
}

static void r36sx_pong_retro_play_event(
    enum r36sx_pong_audio_event event)
{
    switch (event) {
    case R36SX_PONG_AUDIO_BUTTON:
        r36sx_pong_retro_trigger_tone(520, 2200);
        break;
    case R36SX_PONG_AUDIO_BOUNCE:
        r36sx_pong_retro_trigger_tone(880, 1200);
        break;
    case R36SX_PONG_AUDIO_SCORE:
        r36sx_pong_retro_trigger_tone(220, 5000);
        break;
    case R36SX_PONG_AUDIO_WIN:
        r36sx_pong_retro_trigger_tone(1320, 5200);
        break;
    case R36SX_PONG_AUDIO_LOSE:
        r36sx_pong_retro_trigger_tone(180, 5200);
        break;
    case R36SX_PONG_AUDIO_NONE:
    default:
        break;
    }
}

static int16_t r36sx_pong_retro_synth_tone_sample(void)
{
    uint32_t envelope;
    uint32_t amp = 3600u;

    if (tone_remaining == 0) {
        return 0;
    }

    envelope = tone_remaining;
    if (tone_total != 0) {
        amp = 1200u + ((3600u * envelope) / tone_total);
    }

    tone_remaining--;
    tone_phase += tone_step;

    return (tone_phase & 0x8000u) ? (int16_t)amp : (int16_t)-(int32_t)amp;
}

static void r36sx_pong_retro_emit_audio(void)
{
    int had_sound = tone_remaining != 0;

    if (!audio_batch_cb && !audio_sample_cb) {
        return;
    }

    for (unsigned i = 0; i < R36SX_PONG_AUDIO_FRAMES_PER_VIDEO_FRAME; i++) {
        int16_t sample = r36sx_pong_retro_synth_tone_sample();
        audio_frame[i * 2] = sample;
        audio_frame[i * 2 + 1] = sample;
    }

    if (!had_sound) {
        return;
    }

    if (audio_batch_cb) {
        audio_batch_cb(audio_frame, R36SX_PONG_AUDIO_FRAMES_PER_VIDEO_FRAME);
        return;
    }

    for (unsigned i = 0; i < R36SX_PONG_AUDIO_FRAMES_PER_VIDEO_FRAME; i++) {
        audio_sample_cb(audio_frame[i * 2], audio_frame[i * 2 + 1]);
    }
}

unsigned retro_api_version(void)
{
    return RETRO_API_VERSION;
}

void retro_set_environment(retro_environment_t cb)
{
    environ_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
    audio_sample_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
    audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
    input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
    input_state_cb = cb;
}

void retro_get_system_info(struct retro_system_info *info)
{
    r36sx_pong_zero_memory(info, sizeof(*info));
    info->library_name = "Pong Demo";
    info->library_version = "0.2";
    info->valid_extensions = "gb|pong";
    info->need_fullpath = false;
    info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
    r36sx_pong_zero_memory(info, sizeof(*info));
    info->geometry.base_width = R36SX_PONG_FB_WIDTH;
    info->geometry.base_height = R36SX_PONG_FB_HEIGHT;
    info->geometry.max_width = R36SX_PONG_FB_WIDTH;
    info->geometry.max_height = R36SX_PONG_FB_HEIGHT;
    info->geometry.aspect_ratio = 4.0f / 3.0f;
    info->timing.fps = 60.0;
    info->timing.sample_rate = 44100.0;
}

void retro_init(void)
{
    pong_state.frame_counter = 0;
    r36sx_pong_reset_game(&pong_state);
}

void retro_deinit(void)
{
}

bool retro_load_game(const struct retro_game_info *game)
{
    (void)game;
    r36sx_pong_reset_game(&pong_state);
    return true;
}

void retro_unload_game(void)
{
}

void retro_reset(void)
{
    retro_init();
}

void retro_run(void)
{
    uint32_t buttons;

    if (input_poll_cb) {
        input_poll_cb();
    }

    buttons = r36sx_pong_retro_read_buttons();
    r36sx_pong_tick(&pong_state, buttons);
    r36sx_pong_retro_play_event(pong_state.audio_event);
    r36sx_pong_draw_frame(&pong_state, frame);

    if (video_cb) {
        video_cb(frame, R36SX_PONG_FB_WIDTH, R36SX_PONG_FB_HEIGHT,
                 R36SX_PONG_FB_WIDTH * sizeof(frame[0]));
    }

    r36sx_pong_retro_emit_audio();
}

bool retro_load_game_special(unsigned game_type,
                             const struct retro_game_info *info,
                             size_t num_info)
{
    (void)game_type;
    (void)info;
    (void)num_info;
    return false;
}

unsigned retro_get_region(void)
{
    return 0;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
    (void)port;
    (void)device;
}

size_t retro_serialize_size(void)
{
    return 0;
}

bool retro_serialize(void *data, size_t size)
{
    (void)data;
    (void)size;
    return false;
}

bool retro_unserialize(const void *data, size_t size)
{
    (void)data;
    (void)size;
    return false;
}

void *retro_get_memory_data(unsigned id)
{
    (void)id;
    return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
    (void)id;
    return 0;
}

void retro_cheat_reset(void)
{
}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
    (void)index;
    (void)enabled;
    (void)code;
}

void SetFrameSkip(int skip)
{
    (void)skip;
}

int check_encrypty(void)
{
    return 1;
}

int CheckEncrypty(const void *data)
{
    (void)data;
    return 1;
}

#elif R36SX_PONG_TARGET == R36SX_PONG_TARGET_NATIVE

enum {
    R36SX_PONG_FRAME_USEC = 16666,
    R36SX_PONG_NATIVE_EXIT_BUTTONS =
        R36SX_PONG_BUTTON_SELECT | R36SX_PONG_BUTTON_START
};

typedef int (*video_driver_setting_fn)(int *);
typedef int (*video_drivers_init_fn)(void);
typedef void (*video_driver_disp_frame_fn)(void *, int, int, int);
typedef void (*video_driver_deinit_fn)(void);
typedef int (*cube_ioctl_fn)(int, uint32_t *, uint32_t, uint32_t);

struct r36sx_pong_driver_state {
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

static struct r36sx_pong_driver_state g_driver;
static struct r36sx_pong_state pong_state;
static struct r36sx_driver_audio_state g_audio;
static uint16_t frame[R36SX_PONG_FB_WIDTH * R36SX_PONG_FB_HEIGHT];

static void r36sx_pong_display_close(void);

static int r36sx_pong_display_open(void)
{
    static const char *paths[] = {
        R36SX_DRIVER_SO_PATH,
        R36SX_DRIVER_SO_LOCAL_PATH,
        R36SX_DRIVER_SO_NAME
    };

    memset(&g_driver, 0, sizeof(g_driver));
    for (size_t i = 0; i < R36SX_PONG_ARRAY_COUNT(paths); i++) {
        g_driver.handle = dlopen(paths[i], RTLD_NOW);
        if (g_driver.handle) {
            break;
        }
    }
    if (!g_driver.handle) {
        fprintf(stderr, "native_pong: cannot open driver.so: %s\n", dlerror());
        return -1;
    }

    g_driver.setting =
        (video_driver_setting_fn)dlsym(g_driver.handle,
                                       "video_driver_setting");
    g_driver.init =
        (video_drivers_init_fn)dlsym(g_driver.handle, "video_drivers_init");
    g_driver.disp_frame =
        (video_driver_disp_frame_fn)dlsym(g_driver.handle,
                                          "video_driver_disp_frame");
    g_driver.deinit =
        (video_driver_deinit_fn)dlsym(g_driver.handle, "video_driver_deinit");
    g_driver.cube_ioctl =
        (cube_ioctl_fn)dlsym(g_driver.handle, "cube_ioctl");
    if (!g_driver.setting || !g_driver.init || !g_driver.disp_frame ||
        !g_driver.deinit) {
        fprintf(stderr, "native_pong: driver.so display symbols missing\n");
        r36sx_pong_display_close();
        return -1;
    }
    r36sx_driver_audio_init(&g_audio);
    (void)r36sx_driver_audio_bind(&g_audio, g_driver.handle);

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
        r36sx_pong_display_close();
        return -1;
    }

    g_driver.active = 1;
    return 0;
}

static void r36sx_pong_display_close(void)
{
    r36sx_driver_audio_close(&g_audio);
    if (g_driver.active && g_driver.deinit) {
        g_driver.deinit();
    }
    if (g_driver.handle) {
        dlclose(g_driver.handle);
    }
    memset(&g_driver, 0, sizeof(g_driver));
}

static void r36sx_pong_present_frame(void)
{
    if (g_driver.active && g_driver.disp_frame) {
        g_driver.disp_frame(frame, R36SX_PONG_FB_WIDTH,
                            R36SX_PONG_FB_HEIGHT,
                            R36SX_RGB565_FRAME_STRIDE);
    }
}

static uint32_t r36sx_pong_translate_rkgame_keys(uint32_t raw)
{
    uint32_t buttons = 0;

    if ((raw & R36SX_RKGAME_KEY_UP) != 0) {
        buttons |= R36SX_PONG_BUTTON_UP;
    }
    if ((raw & R36SX_RKGAME_KEY_DOWN) != 0) {
        buttons |= R36SX_PONG_BUTTON_DOWN;
    }
    if ((raw & R36SX_RKGAME_KEY_A) != 0) {
        buttons |= R36SX_PONG_BUTTON_A;
    }
    if ((raw & R36SX_RKGAME_KEY_B) != 0) {
        buttons |= R36SX_PONG_BUTTON_B;
    }
    if ((raw & R36SX_RKGAME_KEY_X) != 0) {
        buttons |= R36SX_PONG_BUTTON_X;
    }
    if ((raw & R36SX_RKGAME_KEY_Y) != 0) {
        buttons |= R36SX_PONG_BUTTON_Y;
    }
    if ((raw & R36SX_RKGAME_KEY_START) != 0) {
        buttons |= R36SX_PONG_BUTTON_START;
    }
    if ((raw & R36SX_RKGAME_KEY_SELECT) != 0) {
        buttons |= R36SX_PONG_BUTTON_SELECT;
    }
    if ((raw & R36SX_RKGAME_KEY_FN) != 0) {
        buttons |= R36SX_PONG_BUTTON_FN;
    }

    return buttons;
}

static uint32_t r36sx_pong_input_buttons(void)
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
    return r36sx_pong_translate_rkgame_keys(g_driver.cube_key_mem[0]) |
           r36sx_pong_translate_rkgame_keys(g_driver.cube_key_mem[1]);
}

static void r36sx_pong_native_play_event(enum r36sx_pong_audio_event event)
{
    switch (event) {
    case R36SX_PONG_AUDIO_BUTTON:
        r36sx_driver_audio_play_button(&g_audio);
        break;
    case R36SX_PONG_AUDIO_BOUNCE:
        r36sx_driver_audio_play_bounce(&g_audio);
        break;
    case R36SX_PONG_AUDIO_SCORE:
        r36sx_driver_audio_play_score(&g_audio);
        break;
    case R36SX_PONG_AUDIO_WIN:
        r36sx_driver_audio_play_win(&g_audio);
        break;
    case R36SX_PONG_AUDIO_LOSE:
        r36sx_driver_audio_play_lose(&g_audio);
        break;
    case R36SX_PONG_AUDIO_NONE:
    default:
        break;
    }
}

int main(void)
{
    int running = 1;

    setenv("LD_LIBRARY_PATH",
           "/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib:/lib:/usr/lib",
           1);

    if (r36sx_pong_display_open() != 0) {
        r36sx_pong_display_close();
        return 1;
    }

    r36sx_pong_reset_game(&pong_state);
    while (running) {
        uint32_t buttons = r36sx_pong_input_buttons();
        uint32_t pressed = buttons & ~pong_state.prev_buttons;

        if ((buttons & R36SX_PONG_NATIVE_EXIT_BUTTONS) ==
            R36SX_PONG_NATIVE_EXIT_BUTTONS) {
            running = 0;
        } else if ((pressed & R36SX_PONG_BUTTON_FN) != 0) {
            running = 0;
        } else {
            r36sx_pong_tick(&pong_state, buttons);
            r36sx_pong_native_play_event(pong_state.audio_event);
            r36sx_pong_draw_frame(&pong_state, frame);
            r36sx_pong_present_frame();
            r36sx_driver_audio_update(&g_audio);
            usleep(R36SX_PONG_FRAME_USEC);
        }
    }

    r36sx_pong_display_close();
    return 0;
}

#endif
