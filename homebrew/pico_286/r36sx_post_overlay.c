#include "r36sx_post_overlay.h"

#include <stdio.h>

#include "emulator/includes/font8x8.h"

#define R36SX_POST_OVERLAY_FONT_SCALE 2
#define R36SX_POST_OVERLAY_PAD 6
#define R36SX_POST_OVERLAY_MARGIN 8
#define R36SX_POST_OVERLAY_MAX_CHARS 6
#define R36SX_POST_OVERLAY_LINES 3

static uint16_t r36sx_post_rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)((uint16_t)(r >> 3) << 11 |
                      (uint16_t)(g >> 2) << 5 |
                      (uint16_t)(b >> 3));
}

static void r36sx_post_fill_rect(uint16_t *target,
                                 int frame_width,
                                 int frame_height,
                                 int stride_pixels,
                                 int x,
                                 int y,
                                 int w,
                                 int h,
                                 uint16_t color)
{
    if (!target || frame_width <= 0 || frame_height <= 0 ||
        stride_pixels <= 0 || w <= 0 || h <= 0) {
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
    if (x + w > frame_width) {
        w = frame_width - x;
    }
    if (y + h > frame_height) {
        h = frame_height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = target + (size_t)(y + row) * (size_t)stride_pixels +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void r36sx_post_stroke_rect(uint16_t *target,
                                   int frame_width,
                                   int frame_height,
                                   int stride_pixels,
                                   int x,
                                   int y,
                                   int w,
                                   int h,
                                   uint16_t color)
{
    r36sx_post_fill_rect(target, frame_width, frame_height, stride_pixels,
                         x, y, w, 1, color);
    r36sx_post_fill_rect(target, frame_width, frame_height, stride_pixels,
                         x, y + h - 1, w, 1, color);
    r36sx_post_fill_rect(target, frame_width, frame_height, stride_pixels,
                         x, y, 1, h, color);
    r36sx_post_fill_rect(target, frame_width, frame_height, stride_pixels,
                         x + w - 1, y, 1, h, color);
}

static void r36sx_post_draw_text8_scaled(uint16_t *target,
                                         int frame_width,
                                         int frame_height,
                                         int stride_pixels,
                                         int x,
                                         int y,
                                         const char *text,
                                         uint16_t color)
{
    const int scale = R36SX_POST_OVERLAY_FONT_SCALE;

    for (int i = 0; text && text[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)text[i];
        const unsigned char *glyph = &font_8x8[(size_t)ch * 8u];
        int char_x = x + i * 8 * scale;

        for (int row = 0; row < 8; row++) {
            unsigned char bits = glyph[row];
            for (int col = 0; col < 8; col++) {
                if ((bits & (unsigned char)(1u << col)) == 0) {
                    continue;
                }
                r36sx_post_fill_rect(target, frame_width, frame_height,
                                     stride_pixels,
                                     char_x + col * scale,
                                     y + row * scale,
                                     scale,
                                     scale,
                                     color);
            }
        }
    }
}

void r36sx_post_overlay_rect(int frame_width,
                             int frame_height,
                             int *x,
                             int *y,
                             int *w,
                             int *h)
{
    int box_w = R36SX_POST_OVERLAY_MAX_CHARS * 8 *
                    R36SX_POST_OVERLAY_FONT_SCALE +
                R36SX_POST_OVERLAY_PAD * 2;
    int box_h = R36SX_POST_OVERLAY_LINES * 8 *
                    R36SX_POST_OVERLAY_FONT_SCALE +
                R36SX_POST_OVERLAY_PAD * 2;
    int box_x = R36SX_POST_OVERLAY_MARGIN;
    int box_y = frame_height - box_h - R36SX_POST_OVERLAY_MARGIN;

    if (box_x + box_w > frame_width) {
        box_x = frame_width - box_w;
    }
    if (box_x < 0) {
        box_x = 0;
    }
    if (box_y < 0) {
        box_y = 0;
    }

    if (x) {
        *x = box_x;
    }
    if (y) {
        *y = box_y;
    }
    if (w) {
        *w = box_w;
    }
    if (h) {
        *h = box_h;
    }
}

void r36sx_post_overlay_draw(uint16_t *target,
                             int frame_width,
                             int frame_height,
                             int stride_pixels,
                             uint8_t code_valid,
                             uint8_t code_value,
                             uint8_t subcode_valid,
                             uint8_t subcode_value)
{
    char line1[16];
    char line2[16];
    int x;
    int y;
    int w;
    int h;
    uint16_t bg = r36sx_post_rgb565(8, 12, 18);
    uint16_t border = r36sx_post_rgb565(96, 148, 170);
    uint16_t text = r36sx_post_rgb565(255, 228, 120);

    if (!target) {
        return;
    }

    snprintf(line1, sizeof(line1), "%03X:%02X",
             (unsigned int)R36SX_POST_OVERLAY_CODE_PORT,
             code_valid ? (unsigned int)code_value : 0u);
    snprintf(line2, sizeof(line2), "%03X:%02X",
             (unsigned int)R36SX_POST_OVERLAY_SUBCODE_PORT,
             subcode_valid ? (unsigned int)subcode_value : 0u);

    r36sx_post_overlay_rect(frame_width, frame_height, &x, &y, &w, &h);
    r36sx_post_fill_rect(target, frame_width, frame_height, stride_pixels,
                         x, y, w, h, bg);
    r36sx_post_stroke_rect(target, frame_width, frame_height, stride_pixels,
                           x, y, w, h, border);
    r36sx_post_draw_text8_scaled(target, frame_width, frame_height,
                                 stride_pixels,
                                 x + R36SX_POST_OVERLAY_PAD,
                                 y + R36SX_POST_OVERLAY_PAD,
                                 "Post",
                                 text);
    r36sx_post_draw_text8_scaled(target, frame_width, frame_height,
                                 stride_pixels,
                                 x + R36SX_POST_OVERLAY_PAD,
                                 y + R36SX_POST_OVERLAY_PAD +
                                     8 * R36SX_POST_OVERLAY_FONT_SCALE,
                                 line1,
                                 text);
    r36sx_post_draw_text8_scaled(target, frame_width, frame_height,
                                 stride_pixels,
                                 x + R36SX_POST_OVERLAY_PAD,
                                 y + R36SX_POST_OVERLAY_PAD +
                                     16 * R36SX_POST_OVERLAY_FONT_SCALE,
                                 line2,
                                 text);
}
