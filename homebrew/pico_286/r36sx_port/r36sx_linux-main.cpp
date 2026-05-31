#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <cstdio>
#include "MiniFB.h"
#include "emulator/emulator.h"
#include "emulator/includes/font8x16.h"
#include "emulator/includes/font8x8.h"
#include "emu8950.h"
#include "linux-audio.h"
#include "r36sx_disk_config.h"
#include "r36sx_profile.h"

static uint16_t ALIGN(4, SCREEN[640 * 480]);
uint8_t ALIGN(4, DEBUG_VRAM[80 * 10]) = {0};

int cursor_blink_state = 0;
uint8_t log_debug = 0;

static uint16_t cga_palette565[16];
static uint16_t cga_composite_palette565[3][16];
static uint16_t tga_palette565[16];
static uint16_t vga_palette565[256];
static int static_palettes565_ready = 0;

extern OPL *emu8950_opl;
extern "C" void r36sx_keyboard_enqueue_scancode(uint8_t scancode);
extern "C" void r36sx_keyboard_tick(void);
extern "C" void r36sx_mfb_mark_frame_ready(void);
extern "C" void r36sx_pico286_disk_flush_pending(void);
extern "C" void r36sx_pico286_disk_flush_all(void);

#define AUDIO_BUFFER_LENGTH ((SOUND_FREQUENCY / 10))
#define R36SX_TICKS_THREAD_SLEEP_US 1000u
#define R36SX_HLT_SLEEP_US 1000u
#define R36SX_MAIN_LOOP_DEFAULT_FPS 60u
#define R36SX_EXEC86_MIN_LOOPS 1000u
static int16_t audio_buffer[AUDIO_BUFFER_LENGTH * 2] = {};
static int sample_index = 0;
static volatile int soft_reset_requested = 0;
static volatile int soft_reset_in_progress = 0;
static uint32_t g_main_loop_frame_us = 1000000u / R36SX_MAIN_LOOP_DEFAULT_FPS;

static inline uint32_t vga_vram_cell(uint32_t index) {
    return VIDEORAM[index & 0xFFFFu];
}

extern "C" void adlib_getsample(int16_t *sndptr, intptr_t numsamples);

extern "C" void r36sx_pico286_request_soft_reset(void) {
    soft_reset_requested = 1;
}

static void r36sx_pico286_soft_reset(void);

static uint64_t r36sx_pico286_now_us(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static uint32_t r36sx_pico286_frame_us_from_fps(uint32_t target_fps)
{
    if (target_fps == 0) {
        target_fps = R36SX_MAIN_LOOP_DEFAULT_FPS;
    }

    uint32_t frame_us = (uint32_t)((1000000ull + target_fps / 2u) /
                                   target_fps);
    return frame_us ? frame_us : 1u;
}

static uint32_t r36sx_pico286_frame_exec_loops(uint32_t loops_per_ms,
                                               uint32_t frame_us)
{
    uint64_t loops = ((uint64_t)loops_per_ms * frame_us + 500ull) / 1000ull;

    if (loops < R36SX_EXEC86_MIN_LOOPS) {
        return R36SX_EXEC86_MIN_LOOPS;
    }
    if (loops > 0xffffffffull) {
        return 0xffffffffu;
    }
    return (uint32_t)loops;
}

static uint32_t r36sx_pico286_adjust_exec_loops(uint32_t current_loops,
                                                uint32_t max_loops,
                                                uint64_t exec_us,
                                                uint32_t target_us)
{
    if (max_loops < R36SX_EXEC86_MIN_LOOPS) {
        max_loops = R36SX_EXEC86_MIN_LOOPS;
    }
    if (current_loops < R36SX_EXEC86_MIN_LOOPS) {
        current_loops = R36SX_EXEC86_MIN_LOOPS;
    }
    if (current_loops > max_loops) {
        current_loops = max_loops;
    }
    if (target_us == 0) {
        return current_loops;
    }

    if (exec_us > target_us) {
        uint64_t scaled = (uint64_t)current_loops * target_us / exec_us;
        uint32_t target_loops;
        uint32_t step;
        uint32_t stepped;

        if (scaled < R36SX_EXEC86_MIN_LOOPS) {
            target_loops = R36SX_EXEC86_MIN_LOOPS;
        } else if (scaled > max_loops) {
            target_loops = max_loops;
        } else {
            target_loops = (uint32_t)scaled;
        }

        step = current_loops / 4u;
        if (step == 0) {
            step = 1u;
        }
        stepped = current_loops > step ? current_loops - step
                                       : R36SX_EXEC86_MIN_LOOPS;
        return stepped < target_loops ? target_loops : stepped;
    }

    if (current_loops < max_loops) {
        uint32_t step = current_loops / 4u;
        uint64_t grown;

        if (step == 0) {
            step = 1u;
        }
        grown = (uint64_t)current_loops + step;
        return grown > max_loops ? max_loops : (uint32_t)grown;
    }

    return current_loops;
}

static void r36sx_pico286_wait_for_next_main_frame(uint64_t *next_frame_us,
                                                   uint32_t frame_us)
{
    uint64_t now_us = r36sx_pico286_now_us();

    if (now_us < *next_frame_us) {
        uint64_t sleep_us = *next_frame_us - now_us;
        if (sleep_us > 1000000ull) {
            sleep_us = 1000000ull;
        }
        usleep((unsigned int)sleep_us);
        *next_frame_us += frame_us;
        return;
    }

    *next_frame_us = now_us + frame_us;
    if (r36sx_cpu_waiting_for_interrupt()) {
        usleep(R36SX_HLT_SLEEP_US);
    }
}

extern "C" int r36sx_pico286_video_active_height(void) {
    /*
     * Mirrors the current renderer below: most DOS modes are drawn into the
     * first 400 rows of the 640x480 buffer, with the rest kept black.
     */
    switch (videomode) {
        case 0x07:
        case 0x0f:
        case 0x10:
            return 350;
        case 0x1e:
            return 348;
        default:
            return 400;
    }
}

extern "C" void _putchar(char character) {
    putchar(character);
#if DEBUG
    static char log_line[160];
    static int log_pos = 0;
    if (character == '\n' || character == '\r') {
        if (log_pos > 0) {
            log_line[log_pos] = 0;
            r36sx_pico286_debug_log("stdout: %s", log_line);
            log_pos = 0;
        }
    } else if ((unsigned char)character >= 32 &&
               log_pos < (int)sizeof(log_line) - 1) {
        log_line[log_pos++] = character;
    }
#endif
    static int x = 0, y = 0;

    if (y == 10) {
        y = 9;
        memmove(DEBUG_VRAM, DEBUG_VRAM + 80, 80 * 9);
        memset(DEBUG_VRAM + 80 * 9, 0, 80);
    }
    uint8_t *vidramptr = DEBUG_VRAM + y * 80 + x;

    if ((unsigned) character >= 32) {
        if (character >= 96) character -= 32;
        *vidramptr = ((character - 32) & 63) | 0 << 6;
        if (x == 80) {
            x = 0;
            y++;
        } else
            x++;
    } else if (character == '\n') {
        x = 0;
        y++;
    } else if (character == '\r') {
        x = 0;
    } else if (character == 8 && x > 0) {
        x--;
        *vidramptr = 0;
    }
}

static inline uint16_t rgb888_to_rgb565(uint32_t color)
{
    uint32_t r = (color >> 16) & 0xffu;
    uint32_t g = (color >> 8) & 0xffu;
    uint32_t b = color & 0xffu;
    return (uint16_t)(((r & 0xf8u) << 8) | ((g & 0xfcu) << 3) | (b >> 3));
}

static void refresh_palettes565(void)
{
    if (!static_palettes565_ready) {
        for (int i = 0; i < 16; i++) {
            cga_palette565[i] = rgb888_to_rgb565(cga_palette[i]);
            for (int p = 0; p < 3; p++) {
                cga_composite_palette565[p][i] =
                    rgb888_to_rgb565(cga_composite_palette[p][i]);
            }
        }
        static_palettes565_ready = 1;
    }

    for (int i = 0; i < 16; i++) {
        tga_palette565[i] = rgb888_to_rgb565(tga_palette[i]);
    }
    for (int i = 0; i < 256; i++) {
        vga_palette565[i] = rgb888_to_rgb565(vga_palette[i]);
    }
}

static inline void fill_black_row(uint16_t *pixels)
{
    for (int x = 0; x < 640; x++) {
        *pixels++ = 0;
    }
}

static inline uint16_t mda_text_color(uint8_t attr, int is_foreground)
{
    uint16_t fg = (attr & 0x08) ? 0xffff : rgb888_to_rgb565(0xc4c4c4u);
    uint16_t bg = 0x0000;

    if ((attr & 0x70) != 0) {
        bg = fg;
        fg = 0x000000;
    }
    if ((attr & 0x80) && !cursor_blink_state) {
        fg = bg;
    }
    return is_foreground ? fg : bg;
}

static inline uint16_t ega_mono_pixel(uint32_t plane_bits, int bit)
{
    uint32_t mask = (1u << bit) | (1u << (bit + 8)) |
                    (1u << (bit + 16)) | (1u << (bit + 24));
    return (plane_bits & mask) ? 0xffff : 0x0000;
}

static inline uint32_t vga_crtc_offset_register(void)
{
    return (uint32_t)crt_controller[0x13];
}

static inline uint32_t vga_crtc_planar_stride(uint32_t fallback_stride,
                                              uint8_t half_clock_320)
{
    uint32_t offset = vga_crtc_offset_register();

    if (offset == 0) {
        return fallback_stride;
    }

    /*
     * CRTC offset is normally in word units. EGA 320-wide mode 0Dh uses a
     * half-clocked 40-byte visible row, but tweaked modes (for example
     * Supaplex/SPFIX with offset 3Dh) expect the word interpretation.
     */
    if (half_clock_320 && offset <= fallback_stride) {
        return offset;
    }

    return offset << 1;
}

static inline uint32_t vga_crtc_chunky_stride(uint32_t fallback_stride)
{
    uint32_t offset = vga_crtc_offset_register();

    if (offset == 0) {
        return fallback_stride;
    }

    return offset << 3;
}

static inline void vga_render_planar_4bpp(uint16_t **out_pixels,
                                          uint32_t vram_base,
                                          uint32_t cells,
                                          uint8_t x_scale)
{
    uint16_t *pixels = *out_pixels;

    for (uint32_t i = 0; i < cells; ++i) {
        uint32_t eight_pixels = vga_vram_cell(vram_base + i);
        uint8_t plane0 =  eight_pixels        & 0xFF;
        uint8_t plane1 = (eight_pixels >> 8)  & 0xFF;
        uint8_t plane2 = (eight_pixels >> 16) & 0xFF;
        uint8_t plane3 = (eight_pixels >> 24) & 0xFF;

        for (int bit = 7; bit >= 0; --bit) {
            uint8_t color_index = ((plane0 >> bit) & 1)
                                | (((plane1 >> bit) & 1) << 1)
                                | (((plane2 >> bit) & 1) << 2)
                                | (((plane3 >> bit) & 1) << 3);
            uint16_t color = vga_palette565[color_index];
            *pixels++ = color;
            if (x_scale == 2) {
                *pixels++ = color;
            }
        }
    }

    *out_pixels = pixels;
}

static inline uint32_t cga_graphics_base(void)
{
    return 0x8000u + ((uint32_t)(vram_offset & 0xffffu) << 1);
}

static inline uint32_t cga_graphics_row_offset(int screen_y)
{
    uint32_t source_y = (uint32_t)screen_y >> 1;
    return (source_y >> 1) * 80u + (source_y & 1u) * 8192u;
}

static inline void renderer() {
    static uint8_t v = 0;
    refresh_palettes565();

    if (v != videomode) {
        printf("videomode %x %x\n", videomode, v);
        v = videomode;
    }

    uint8_t cols = 80;
    for (int y = 0; y < 480; y++) {
        uint16_t *pixels = SCREEN + y * 640;

        if (y < 400)
            switch (videomode) {
                case 0x00:
                case 0x01: {
                    uint16_t y_div_16 = y / 16; // Precompute y / 16
                    uint8_t glyph_line = (y / 2) % 8; // Precompute y % 8 for font lookup
                    // Calculate screen position
                    uint32_t *text_buffer_line =
                            &VIDEORAM[0x8000 + ((vram_offset & 0xffff) << 1) + y_div_16 * 80];

                    for (int column = 0; column < 40; column++) {
                        uint8_t charcode = (uint8_t)(*text_buffer_line++ & 0xffu);
                        uint8_t glyph_pixels = font_8x8[charcode * 8 + glyph_line]; // Glyph row from font
                        uint8_t color = (uint8_t)(*text_buffer_line++ & 0xffu); // Color attribute

                        // Cursor blinking check
                        uint8_t cursor_active = cursor_blink_state &&
                                                y_div_16 == CURSOR_Y && column == CURSOR_X &&
                                                glyph_line >= cursor_start && glyph_line <= cursor_end;

                        for (uint8_t bit = 0; bit < 8; bit++) {
                            uint8_t pixel_color;
                            if (cursor_active) {
                                pixel_color = color & 0x0F; // Cursor foreground color
                            } else if (cga_blinking && color >> 7 & 1) {
                                pixel_color = cursor_blink_state ? color >> 4 & 0x7 : color & 0x7; // Blinking background color
                            } else {
                                pixel_color = glyph_pixels >> bit & 1 ? color & 0x0f : color >> 4;
                                // Foreground or background color
                            }

                            // Write the pixel twice (horizontal scaling)
                            *pixels++ = *pixels++ = cga_palette565[pixel_color];
                        }
                    }


                    break;
                }
                case 0x02:
                case 0x03:
                case 0x20:
                case 0x30: {
                    uint16_t y_div_16 = y / 16; // Precompute y / 16
                    uint8_t glyph_line = y % 16; // Precompute y % 8 for font lookup

                    // Calculate screen position
                    uint32_t *text_row =
                            &VIDEORAM[0x8000 + ((vram_offset & 0xffff) << 1) + y_div_16 * 160];
                    for (uint8_t column = 0; column < 80; column++) {
                        // Access vidram and font data once per character
                        uint8_t charcode = (uint8_t)(text_row[column * 2] & 0xffu); // Character code
                        uint8_t glyph_row = font_8x16[charcode * 16 + glyph_line]; // Glyph row from font
                        uint8_t color = (uint8_t)(text_row[column * 2 + 1] & 0xffu); // Color attribute

                        // Cursor blinking check
                        uint8_t cursor_active =
                                cursor_blink_state && y_div_16 == CURSOR_Y && column == CURSOR_X &&
                                (cursor_start > cursor_end
                                     ? !(glyph_line >= cursor_end << 1 &&
                                         glyph_line <= cursor_start << 1)
                                     : glyph_line >= cursor_start << 1 && glyph_line <= cursor_end << 1);

                        // Unrolled bit loop: Write 8 pixels with scaling (2x horizontally)
                        for (int bit = 0; bit < 8; bit++) {
                            uint8_t pixel_color;
                            if (cursor_active) {
                                pixel_color = color & 0x0F; // Cursor foreground color
                            } else if (cga_blinking && color >> 7 & 1) {
                                if (cursor_blink_state) {
                                    pixel_color = color >> 4 & 0x7; // Blinking background color
                                } else {
                                    pixel_color = glyph_row >> bit & 1 ? color & 0x0f : (color >> 4 & 0x7);
                                }
                            } else {
                                // Foreground or background color
                                pixel_color = glyph_row >> bit & 1 ? color & 0x0f : color >> 4;
                            }

                            *pixels++ = cga_palette565[pixel_color];
                        }
                    }
                    break;
                }
                case 0x04:
                case 0x05: {
                    uint32_t *cga_row = &VIDEORAM[cga_graphics_base() + cga_graphics_row_offset(y)];
                    uint8_t *current_cga_palette = (uint8_t *) cga_gfxpal[cga_colorset][cga_intensity];

                    // Each byte containing 4 pixels
                    for (int x = 320 / 4; x--;) {
                        uint8_t cga_byte = (uint8_t)(*cga_row++ & 0xffu);

                        // Extract all four 2-bit pixels from the CGA byte
                        // and write each pixel twice for horizontal scaling
                        *pixels++ = *pixels++ = cga_palette565[cga_byte >> 6 & 3
                                                                ? current_cga_palette[cga_byte >> 6 & 3]
                                                                : cga_foreground_color];
                        *pixels++ = *pixels++ = cga_palette565[cga_byte >> 4 & 3
                                                                ? current_cga_palette[cga_byte >> 4 & 3]
                                                                : cga_foreground_color];
                        *pixels++ = *pixels++ = cga_palette565[cga_byte >> 2 & 3
                                                                ? current_cga_palette[cga_byte >> 2 & 3]
                                                                : cga_foreground_color];
                        *pixels++ = *pixels++ = cga_palette565[cga_byte >> 0 & 3
                                                                ? current_cga_palette[cga_byte >> 0 & 3]
                                                                : cga_foreground_color];
                    }
                    break;
                }
                case 0x06: {
                    uint32_t *cga_row = &VIDEORAM[cga_graphics_base() + cga_graphics_row_offset(y)];

                    // Each byte containing 8 pixels
                    for (int x = 640 / 8; x--;) {
                        uint8_t cga_byte = (uint8_t)(*cga_row++ & 0xffu);

                        *pixels++ = cga_palette565[(cga_byte >> 7 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 6 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 5 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 4 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 3 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 2 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 1 & 1) * cga_foreground_color];
                        *pixels++ = cga_palette565[(cga_byte >> 0 & 1) * cga_foreground_color];
                    }

                    break;
                }
                case 0x1e: {
                    cols = 90;
                    vram_offset = 5;
                    if (y >= 348) {
                        fill_black_row(pixels);
                        break;
                    }
                    uint8_t *cga_row = (uint8_t *)VIDEORAM + vram_offset + (y & 3) * 8192 + y / 4 * cols;
                    // Each byte containing 8 pixels
                    for (int x = 640 / 8; x--;) {
                        uint8_t cga_byte = *cga_row++;

                        *pixels++ = cga_palette565[(cga_byte >> 7 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 6 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 5 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 4 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 3 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 2 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 1 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 0 & 1) * 15];
                    }

                    break;
                }
                case 0x7: {
                    if (y >= 350) {
                        fill_black_row(pixels);
                        break;
                    }

                    uint8_t glyph_line = (uint8_t)(y % 14);
                    uint8_t font_line = glyph_line + 1;
                    uint16_t text_row_index = (uint16_t)(y / 14);
                    uint32_t *text_row = &VIDEORAM[((vram_offset & 0xffff) << 1) +
                                                   text_row_index * 160];

                    for (uint8_t column = 0; column < 80; column++) {
                        uint8_t charcode =
                            (uint8_t)(text_row[column * 2] & 0xffu);
                        uint8_t attr =
                            (uint8_t)(text_row[column * 2 + 1] & 0xffu);
                        uint8_t glyph_row =
                            font_8x16[charcode * 16 + font_line];
                        uint8_t cursor_active =
                            cursor_blink_state && text_row_index == CURSOR_Y &&
                            column == CURSOR_X &&
                            glyph_line >= cursor_start &&
                            glyph_line <= cursor_end;

                        if ((attr & 0x07) == 0x01 && glyph_line == 13) {
                            glyph_row = 0xff;
                        }

                        for (int bit = 0; bit < 8; bit++) {
                            int draw_pixel = (glyph_row >> bit) & 1;
                            if (cursor_active) {
                                draw_pixel = !draw_pixel;
                            }
                            *pixels++ = mda_text_color(attr, draw_pixel);
                        }
                    }

                    break;
                }

                case 0x8:
                case 0x74: /* 160x200x16    */
                case 0x76: /* cga composite / tandy */ {
                    const uint16_t *palette = tga_palette565;
                    switch (videomode) {
                        case 0x08:
                            palette = tga_palette565;
                            break;
                        case 0x74:
                            palette = cga_composite_palette565[cga_intensity << 1];
                            break;
                        case 0x76:
                            palette = cga_composite_palette565[0];
                            break;
                    }

                    uint8_t *cga_row = (uint8_t *)VIDEORAM + tga_offset + (y / 2 >> 1) * 80 + (y / 2 & 1) * 8192; // Precompute row start

                    // Each byte containing 8 pixels
                    for (int x = 640 / 8; x--;) {
                        uint8_t cga_byte = *cga_row++; // Fetch 8 pixels from TGA memory
                        uint8_t color1 = cga_byte >> 4 & 15;
                        uint8_t color2 = cga_byte & 15;

                        if (!color1 && videomode == 0x8) color1 = cga_foreground_color;
                        if (!color2 && videomode == 0x8) color2 = cga_foreground_color;

                        *pixels++ = *pixels++ = *pixels++ = *pixels++ = palette[color1];
                        *pixels++ = *pixels++ = *pixels++ = *pixels++ = palette[color2];
                    }

                    break;
                }
                case 0x09: /* tandy 320x200 16 color */ {
                    uint8_t *tga_row = (uint8_t *)VIDEORAM + tga_offset + (y / 2 & 3) * 8192 + y / 8 * 160;
                    //                  uint8_t *tga_row = &VIDEORAM[tga_offset+(((y / 2) & 3) * 8192) + ((y / 8) * 160)];

                    // Each byte containing 4 pixels
                    for (int x = 320 / 2; x--;) {
                        uint8_t tga_byte = *tga_row++;
                        *pixels++ = *pixels++ = tga_palette565[tga_palette_map[tga_byte >> 4 & 15]];
                        *pixels++ = *pixels++ = tga_palette565[tga_palette_map[tga_byte & 15]];
                    }
                    break;
                }
                case 0x0a: /* tandy 640x200 16 color */ {
                    uint8_t *tga_row = (uint8_t *)VIDEORAM + y / 2 * 320;

                    // Each byte contains 2 pixels
                    for (int x = 640 / 2; x--;) {
                        uint8_t tga_byte = *tga_row++;
                        *pixels++ = tga_palette565[tga_palette_map[tga_byte >> 4 & 15]];
                        *pixels++ = tga_palette565[tga_palette_map[tga_byte & 15]];
                    }
                    break;
                }
                case 0x0D: /* EGA 320x200 16-color */ {
                    if (y >= 400) break;
                    const uint32_t visible_cells = 320u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 1);
                    uint32_t vram_base = vram_offset +
                                         (uint32_t)(y / 2) * stride;
                    vga_render_planar_4bpp(&pixels, vram_base,
                                            visible_cells, 2);
                    break;
                }
                case 0x0E: /* EGA 640x200 16-color */ {
                    if (y >= 400) break;
                    const uint32_t visible_cells = 640u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                    uint32_t vram_base = vram_offset +
                                         (uint32_t)(y / 2) * stride;
                    vga_render_planar_4bpp(&pixels, vram_base,
                                            visible_cells, 1);
                    break;
                }
                case 0x0F: /* EGA 640x350 monochrome */ {
                    if (y >= 350) {
                        fill_black_row(pixels);
                        break;
                    }
                    const uint32_t visible_cells = 640u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                    uint32_t vram_base = vram_offset + (uint32_t)y * stride;
                    for (uint32_t i = 0; i < visible_cells; ++i) {
                        uint32_t eight_pixels = vga_vram_cell(vram_base + i);
                        for (int bit = 7; bit >= 0; --bit) {
                            *pixels++ = ega_mono_pixel(eight_pixels, bit);
                        }
                    }
                    break;
                }
                case 0x10: /* EGA 640x350 16-color */ {
                    if (y >= 350) break;
                    const uint32_t visible_cells = 640u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                    uint32_t vram_base = vram_offset + (uint32_t)y * stride;
                    vga_render_planar_4bpp(&pixels, vram_base,
                                            visible_cells, 1);
                    break;
                }
                case 0x11: /* VGA 640x480 2-color */ {
                    // Each byte containing 8 pixels
                    const uint32_t visible_cells = 640u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                    uint32_t vram_base = vram_offset + (uint32_t)y * stride;
                    for (uint32_t i = 0; i < visible_cells; ++i) {
                        uint8_t cga_byte = (uint8_t)(vga_vram_cell(vram_base + i) & 0xFFu);

                        *pixels++ = cga_palette565[(cga_byte >> 7 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 6 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 5 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 4 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 3 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 2 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 1 & 1) * 15];
                        *pixels++ = cga_palette565[(cga_byte >> 0 & 1) * 15];
                    }

                    break;
                }
                case 0x12: /* VGA 640x480 16-color */ {
                    if (y >= 480) break;
                    const uint32_t visible_cells = 640u / 8u;
                    uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                    uint32_t vram_base = vram_offset + (uint32_t)y * stride;
                    vga_render_planar_4bpp(&pixels, vram_base,
                                            visible_cells, 1);
                    break;
                }
                case 0x13: {
                    if (vga_planar_mode) {
                        const uint32_t visible_cells = 320u / 4u;
                        uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                        uint32_t vram_base = vram_offset +
                                             ((uint32_t)y >> 1) * stride;
                        for (uint32_t x = 0; x < visible_cells; x++) {
                            uint32_t four_pixels = vga_vram_cell(vram_base + x);
                            *pixels++ = *pixels++ = vga_palette565[four_pixels & 0xFFu];
                            *pixels++ = *pixels++ = vga_palette565[(four_pixels >> 8) & 0xFFu];
                            *pixels++ = *pixels++ = vga_palette565[(four_pixels >> 16) & 0xFFu];
                            *pixels++ = *pixels++ = vga_palette565[(four_pixels >> 24) & 0xFFu];
                        }
                    } else {
                        uint32_t stride = vga_crtc_chunky_stride(320u);
                        uint32_t vram_base = vram_offset +
                                             ((uint32_t)y >> 1) * stride;
                        for (int x = 0; x < 320; x++) {
                            uint16_t color = vga_palette565[vga_vram_cell(vram_base + x) & 0xFFu];
                            *pixels++ = *pixels++ = color;
                        }
                    }

                    break;
                }
                case 0x78: /* 80x100x16 textmode */
                    cols = 40;
                case 0x77: /* 160x100x16 textmode */ {
                    uint16_t y_div_4 = y / 4; // Precompute y / 4
                    uint8_t odd_even = y / 2 & 1;
                    // Calculate screen position
                    uint8_t *cga_row = (uint8_t *)VIDEORAM + 0x8000 + y_div_4 * 160;
                    for (uint8_t column = 0; column < cols; column++) {
                        // Access vidram and font data once per character
                        uint8_t *charcode = cga_row + column * 2; // Character code
                        uint8_t glyph_row = font_8x8[*charcode * 8 + odd_even]; // Glyph row from font
                        uint8_t color = *++charcode;

#pragma GCC unroll(8)
                        for (uint8_t bit = 0; bit < 8; bit++) {
                            *pixels++ = cga_palette565[glyph_row >> bit & 1 ? color & 0x0f : color >> 4];
                        }
                    }
                    break;
                }
                case 0x79: /* 80x200x16 textmode */ {
                    int y_div_2 = y / 2; // Precompute y / 2
                    // Calculate screen position
                    uint8_t *cga_row = (uint8_t *)VIDEORAM + 0x8000 + y_div_2 * 80 + (y_div_2 & 1 * 8192);
                    for (int column = 0; column < 40; column++) {
                        // Access vidram and font data once per character
                        uint8_t *charcode = cga_row + column * 2; // Character code
                        uint8_t glyph_row = font_8x8[*charcode * 8]; // Glyph row from font
                        uint8_t color = *++charcode;

#pragma GCC unroll(8)
                        for (int bit = 0; bit < 8; bit++) {
                            *pixels++ = *pixels++ = cga_palette565[glyph_row >> bit & 1 ? color & 0x0f : color >> 4];
                        }
                    }
                    break;
                }
                case 0x87: {
                    /* 40x46 ??? */
                    int y_div_2 = y / 8; // Precompute y / 2
                    // Calculate screen position
                    uint8_t *cga_row = (uint8_t *)VIDEORAM + 0x8000 + y_div_2 * 80 + (y_div_2 & 1 * 8192);
                    for (int column = 0; column < 40; column++) {
                        // Access vidram and font data once per character
                        uint8_t *charcode = cga_row + column * 2; // Character code
                        uint8_t glyph_row = font_8x8[*charcode * 8 + (y_div_2 % 8)]; // Glyph row from font
                        uint8_t color = *++charcode;

#pragma GCC unroll(8)
                        for (int bit = 0; bit < 8; bit++) {
                            *pixels++ = *pixels++ = cga_palette565[glyph_row >> bit & 1 ? color & 0x0f : color >> 4];
                        }
                    }
                    break;
                }
                default:
                    printf("Unsupported videomode %x\n", videomode);
                    break;
            }
        else {
            fill_black_row(pixels);
        }
    }
    r36sx_mfb_mark_frame_ready();
}

extern "C" void HandleInput(unsigned int keycode, int isKeyDown) {
    // Convert X11 keycode to PC scancode
    unsigned char scancode = 0;

    switch (keycode) {
        case 27: scancode = 0x01;
            break; // Escape
        case 49: scancode = 0x02;
            break; // 1
        case 50: scancode = 0x03;
            break; // 2
        case 51: scancode = 0x04;
            break; // 3
        case 52: scancode = 0x05;
            break; // 4
        case 53: scancode = 0x06;
            break; // 5
        case 54: scancode = 0x07;
            break; // 6
        case 55: scancode = 0x08;
            break; // 7
        case 56: scancode = 0x09;
            break; // 8
        case 57: scancode = 0x0A;
            break; // 9
        case 48: scancode = 0x0B;
            break; // 0
        case 65: scancode = 0x1E;
            break; // A
        case 66: scancode = 0x30;
            break; // B
        case 67: scancode = 0x2E;
            break; // C
        case 68: scancode = 0x20;
            break; // D
        case 69: scancode = 0x12;
            break; // E
        case 70: scancode = 0x21;
            break; // F
        case 71: scancode = 0x22;
            break; // G
        case 72: scancode = 0x23;
            break; // H
        case 73: scancode = 0x17;
            break; // I
        case 74: scancode = 0x24;
            break; // J
        case 75: scancode = 0x25;
            break; // K
        case 76: scancode = 0x26;
            break; // L
        case 77: scancode = 0x32;
            break; // M
        case 78: scancode = 0x31;
            break; // N
        case 79: scancode = 0x18;
            break; // O
        case 80: scancode = 0x19;
            break; // P
        case 81: scancode = 0x10;
            break; // Q
        case 82: scancode = 0x13;
            break; // R
        case 83: scancode = 0x1F;
            break; // S
        case 84: scancode = 0x14;
            break; // T
        case 85: scancode = 0x16;
            break; // U
        case 86: scancode = 0x2F;
            break; // V
        case 87: scancode = 0x11;
            break; // W
        case 88: scancode = 0x2D;
            break; // X
        case 89: scancode = 0x15;
            break; // Y
        case 90: scancode = 0x2C;
            break; // Z
        case 32: scancode = 0x39;
            break; // Space
        case 13: scancode = 0x1C;
            break; // Enter
        case 8: scancode = 0x0E;
            break; // Backspace
        case 9: scancode = 0x0F;
            break; // Tab
        case 37: scancode = 0x4B;
            break; // Left
        case 38: scancode = 0x48;
            break; // Up
        case 39: scancode = 0x4D;
            break; // Right
        case 40: scancode = 0x50;
            break; // Down
        case 112: scancode = 0x3B;
            break; // F1
        case 113: scancode = 0x3C;
            break; // F2
        case 114: scancode = 0x3D;
            break; // F3
        case 115: scancode = 0x3E;
            break; // F4
        case 116: scancode = 0x3F;
            break; // F5
        case 117: scancode = 0x40;
            break; // F6
        case 118: scancode = 0x41;
            break; // F7
        case 119: scancode = 0x42;
            break; // F8
        case 120: scancode = 0x43;
            break; // F9
        case 121: scancode = 0x44;
            break; // F10
        case 122: scancode = 0x57;
            break; // F11
        case 123: scancode = 0x58;
            break; // F12
        case 16: scancode = 0x2A;
            break; // Shift
        case 17: scancode = 0x1D;
            break; // Ctrl
        case 18: scancode = 0x38;
            break; // Alt
        case 46: scancode = 0x53;
            break; // Delete
        case 186: scancode = 0x27;
            break; // ; / :
        case 187: scancode = 0x0D;
            break; // = / +
        case 188: scancode = 0x33;
            break; // ,
        case 189: scancode = 0x0C;
            break; // - / _
        case 190: scancode = 0x34;
            break; // .
        case 191: scancode = 0x35;
            break; // / / ?
        case 192: scancode = 0x29;
            break; // ` / ~
        case 219: scancode = 0x1A;
            break; // [ / {
        case 220: scancode = 0x2B;
            break; // backslash / |
        case 221: scancode = 0x1B;
            break; // ] / }
        case 222: scancode = 0x28;
            break; // apostrophe / quote
        default: scancode = 0;
            break;
    }

    if (!isKeyDown && scancode != 0) {
        scancode |= 0x80;
    }

    if (scancode != 0) {
        r36sx_keyboard_enqueue_scancode((uint8_t)scancode);
    }
}

extern "C" void HandleMouse(int x, int y, int buttons) {
    static int prev_x = 0, prev_y = 0;
    sermouseevent(buttons, x - prev_x, y - prev_y);
    prev_y = y;
    prev_x = x;
}

extern "C" int HanldeMenu(int menu_id, int checked) {
    switch (menu_id) {
        case 1:
            return !checked;
        case 2:
            return !checked;
        default:
            return 0;
    }
}

static volatile int running = 1;

void signal_handler(int sig) {
    r36sx_pico286_debug_log("main: signal %d, stopping", sig);
    running = 0;
}

void fatal_signal_handler(int sig) {
    r36sx_pico286_debug_log("main: fatal signal %d", sig);
    signal(sig, SIG_DFL);
    raise(sig);
}

pthread_mutex_t update_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t update_cond = PTHREAD_COND_INITIALIZER;
volatile int update_ready = 0;

static void r36sx_pico286_soft_reset(void) {
    r36sx_pico286_debug_log("main: soft reset begin");
    soft_reset_requested = 0;
    soft_reset_in_progress = 1;
    __sync_synchronize();
    r36sx_pico286_disk_flush_all();

    r36sx_keyboard_reset();
    port60 = 0;
    port61 = 0;
    port64 = 0;
    memset(&i8259_controller, 0, sizeof(i8259_controller));
    i8259_controller.interrupt_vector_offset = 8;
    memset(&i8253_controller, 0, sizeof(i8253_controller));
    timer_period = 54925;
    speakerenabled = 0;
    covox_sample = 0;

    blaster_reset();
    sn76489_reset();
    if (emu8950_opl) {
        OPL_reset(emu8950_opl);
    }
    reset86();

    pthread_mutex_lock(&update_mutex);
    memset(audio_buffer, 0, sizeof(audio_buffer));
    sample_index = 0;
    update_ready = 0;
    pthread_mutex_unlock(&update_mutex);

    r36sx_mfb_mark_frame_ready();
    __sync_synchronize();
    soft_reset_in_progress = 0;
    r36sx_pico286_debug_log("main: soft reset end");
}

void *sound_thread(void *arg) {
    r36sx_pico286_debug_log("sound_thread: start arg=%p", arg);
    unsigned int sound_loop_count = 0;
    while (running) {
        pthread_mutex_lock(&update_mutex);
        while (!update_ready && running) {
            pthread_cond_wait(&update_cond, &update_mutex);
        }
        if (!running) {
            pthread_mutex_unlock(&update_mutex);
            break;
        }
        update_ready = 0;
        pthread_mutex_unlock(&update_mutex);

        // Send audio buffer to Linux audio system
        if (++sound_loop_count <= 4u) {
            r36sx_pico286_debug_log("sound_thread: write #%u sample_index=%d",
                                    sound_loop_count, sample_index);
        }
        R36SX_PROFILE_BEGIN(profile_audio_write);
        int audio_write_rc = linux_audio_write(audio_buffer, AUDIO_BUFFER_LENGTH);
        R36SX_PROFILE_END(R36SX_PROFILE_AUDIO_WRITE, profile_audio_write);
        if (audio_write_rc != 0) {
            // Audio write failed, but continue running
            // printf("Audio write failed!\n");
            // usleep(1000); // 1ms delay
        }
    }
    r36sx_pico286_debug_log("sound_thread: exit loops=%u", sound_loop_count);
    return NULL;
}

void *ticks_thread(void *arg) {
    r36sx_pico286_debug_log("ticks_thread: start arg=%p timer_period=%d",
                            arg, timer_period);
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    uint64_t elapsed_system_timer = 0;
    uint64_t elapsed_blink_tics = 0;
    uint64_t elapsed_frame_tics = 0;
    uint64_t last_dss_tick = 0;
    uint64_t last_sb_tick = 0;
    uint64_t last_sound_tick = 0;

    int16_t last_dss_sample = 0;
    int16_t last_sb_sample = 0;

    const uint64_t hostfreq = 1000000000; // nanoseconds
    const uint64_t dss_period = hostfreq / 7000;
    const uint64_t sb_period = hostfreq / 22050;
    const uint64_t sound_period = hostfreq / SOUND_FREQUENCY;
    const uint64_t blink_period = 333333333;
    const uint64_t frame_period = (uint64_t)g_main_loop_frame_us * 1000ull;
    const unsigned int max_system_catchup = 8;
    const unsigned int max_dss_catchup = 700;
    const unsigned int max_sb_catchup = 2205;
    const unsigned int max_audio_catchup = SOUND_FREQUENCY / 20;

    unsigned int ticks_loop_count = 0;
    while (running) {
        if (soft_reset_in_progress) {
            usleep(R36SX_TICKS_THREAD_SLEEP_US);
            continue;
        }
        clock_gettime(CLOCK_MONOTONIC, &current);
        if (++ticks_loop_count <= 4u) {
            r36sx_pico286_debug_log("ticks_thread: loop #%u timer_period=%d sample_index=%d",
                                    ticks_loop_count, timer_period, sample_index);
        }

        uint64_t elapsedTime = (current.tv_sec - start.tv_sec) * hostfreq + (current.tv_nsec - start.tv_nsec);
        uint64_t system_period = timer_period > 0 ? hostfreq / (uint64_t)timer_period : hostfreq / 18;

        // Timer interrupt (~18.2 Hz)
        unsigned int system_catchup_count = 0;
        R36SX_PROFILE_BEGIN(profile_timer_irq);
        for (;
             elapsedTime - elapsed_system_timer >= system_period;
             system_catchup_count++) {
            doirq(0);
            elapsed_system_timer += system_period;
            if (system_catchup_count >= max_system_catchup) {
                elapsed_system_timer = elapsedTime;
                break;
            }
        }
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_TIMER_IRQ, profile_timer_irq,
                                system_catchup_count);

        // Disney Sound Source frequency ~7KHz
        unsigned int dss_catchup_count = 0;
        R36SX_PROFILE_BEGIN(profile_dss_sample);
        for (;
             elapsedTime - last_dss_tick >= dss_period;
             dss_catchup_count++) {
            last_dss_sample = dss_sample();
            last_dss_tick += dss_period;
            if (dss_catchup_count >= max_dss_catchup) {
                last_dss_tick = elapsedTime;
                break;
            }
        }
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_DSS_SAMPLE, profile_dss_sample,
                                dss_catchup_count);

        // Sound Blaster
        unsigned int sb_catchup_count = 0;
        R36SX_PROFILE_BEGIN(profile_sb_sample);
        for (;
             elapsedTime - last_sb_tick >= sb_period;
             sb_catchup_count++) {
            last_sb_sample = blaster_sample();
            last_sb_tick += sb_period;
            if (sb_catchup_count >= max_sb_catchup) {
                last_sb_tick = elapsedTime;
                break;
            }
        }
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_SB_SAMPLE, profile_sb_sample,
                                sb_catchup_count);

        // Audio samples
        unsigned int audio_catchup_count = 0;
        R36SX_PROFILE_BEGIN(profile_audio_sample);
        for (;
             elapsedTime - last_sound_tick >= sound_period;
             audio_catchup_count++) {
            get_sound_sample(last_dss_sample + last_sb_sample, &audio_buffer[sample_index]);
            sample_index += 2;

            if (sample_index >= AUDIO_BUFFER_LENGTH * 2) {
                pthread_mutex_lock(&update_mutex);
                update_ready = 1;
                pthread_cond_signal(&update_cond);
                pthread_mutex_unlock(&update_mutex);
                sample_index = 0;
            }

            last_sound_tick += sound_period;
            if (audio_catchup_count >= max_audio_catchup) {
                last_sound_tick = elapsedTime;
                break;
            }
        }
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_AUDIO_SAMPLE,
                                profile_audio_sample,
                                audio_catchup_count);

        // Cursor blink
        if (elapsedTime - elapsed_blink_tics >= blink_period) {
            // ~3Hz
            cursor_blink_state ^= 1;
            elapsed_blink_tics = elapsedTime;
        }

        // Frame rendering follows the configured main-loop frame budget.
        if (elapsedTime - elapsed_frame_tics >= frame_period) {
            R36SX_PROFILE_BEGIN(profile_renderer);
            renderer();
            R36SX_PROFILE_END(R36SX_PROFILE_RENDERER, profile_renderer);
            elapsed_frame_tics = elapsedTime;
        }

        // Generate missed ticks in batches, then yield the CPU to exec86().
        usleep(R36SX_TICKS_THREAD_SLEEP_US);
    }
    r36sx_pico286_debug_log("ticks_thread: exit loops=%u", ticks_loop_count);
    return NULL;
}

int main() {
    r36sx_pico286_debug_reset();
    r36sx_pico286_debug_log("main: start");
    r36sx_profile_init();
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSEGV, fatal_signal_handler);
    signal(SIGBUS, fatal_signal_handler);
    signal(SIGILL, fatal_signal_handler);
    signal(SIGABRT, fatal_signal_handler);

    r36sx_pico286_debug_log("main: opening MiniFB");
    if (!mfb_open("Pico-286 Emulator", 640, 480, 1)) {
        r36sx_pico286_debug_log("main: mfb_open failed");
        printf("Failed to open window\n");
        return -1;
    }
    r36sx_pico286_debug_log("main: MiniFB opened");

    write86 = write86_ob;
    writew86 = writew86_ob;
    writedw86 = writedw86_ob;
    read86 = read86_ob;
    readw86 = readw86_ob;
    readdw86 = readdw86_ob;
    r36sx_pico286_debug_log("main: memory backend read=%p write=%p",
                            read86, write86);
    memset(SCREEN, 0, sizeof(SCREEN));
    r36sx_mfb_mark_frame_ready();
    r36sx_pico286_debug_log("main: screen cleared");
    emu8950_opl = OPL_new(3579552, SOUND_FREQUENCY);
    r36sx_pico286_debug_log("main: OPL_new=%p", emu8950_opl);
    blaster_reset();
    r36sx_pico286_debug_log("main: blaster_reset done");
    sn76489_reset();
    r36sx_pico286_debug_log("main: sn76489_reset done");
    reset86();
    r36sx_pico286_debug_log("main: reset86 done");

    // Initialize audio system
    if (linux_audio_init(SOUND_FREQUENCY, 2, AUDIO_BUFFER_LENGTH) == 0) {
        if (linux_audio_start() == 0) {
            printf("Audio: %s backend started\n", linux_audio_get_backend_name());
        } else {
            printf("Audio: Failed to start, continuing without audio\n");
        }
    } else {
        printf("Audio: Failed to initialize, continuing without audio\n");
    }

    const uint32_t target_fps =
        r36sx_pico286_target_fps(R36SX_MAIN_LOOP_DEFAULT_FPS);
    const uint32_t main_loop_frame_us =
        r36sx_pico286_frame_us_from_fps(target_fps);
    g_main_loop_frame_us = main_loop_frame_us;

    pthread_t sound_tid = 0, ticks_tid = 0;
    int sound_thread_rc = pthread_create(&sound_tid, NULL, sound_thread, NULL);
    int ticks_thread_rc = pthread_create(&ticks_tid, NULL, ticks_thread, NULL);
    int sound_thread_started = sound_thread_rc == 0;
    int ticks_thread_started = ticks_thread_rc == 0;
    r36sx_pico286_debug_log("main: pthread_create sound=%d ticks=%d",
                            sound_thread_rc, ticks_thread_rc);

    const uint32_t cpu_exec_loops_per_ms =
        r36sx_pico286_cpu_exec_loops(32768u);
    const uint32_t cpu_exec_loops_per_frame_max =
        r36sx_pico286_frame_exec_loops(cpu_exec_loops_per_ms,
                                       main_loop_frame_us);
    uint32_t cpu_exec_loops_per_frame = cpu_exec_loops_per_frame_max;
    r36sx_pico286_debug_log(
        "main: cpu_model=%s cpu_mode=%s bios=%s cpu_exec_loops_per_ms=%u target_fps=%u frame_us=%u cpu_exec_loops_per_frame_max=%u",
                            r36sx_pico286_cpu_model_name(),
                            r36sx_pico286_cpu_mode_name(),
                            r36sx_pico286_bios_mode_name(),
                            cpu_exec_loops_per_ms,
                            target_fps,
                            main_loop_frame_us,
                            cpu_exec_loops_per_frame_max);
    if (r36sx_pico286_cpu_mode() == R36SX_PICO286_CPU_MODE_PROTECTED) {
        r36sx_pico286_debug_log(
            "main: cpu_mode=protected requested; guest still boots in real mode and may enter protected mode through CR0/LMSW");
    }

    unsigned int main_loop_count = 0;
    uint64_t next_main_loop_us =
        r36sx_pico286_now_us() + main_loop_frame_us;
    while (running) {
        uint64_t exec_elapsed_us;
        if (soft_reset_requested) {
            R36SX_PROFILE_BEGIN(profile_soft_reset);
            r36sx_pico286_soft_reset();
            R36SX_PROFILE_END(R36SX_PROFILE_SOFT_RESET, profile_soft_reset);
        }
        R36SX_PROFILE_BEGIN(profile_keyboard_tick_1);
        r36sx_keyboard_tick();
        R36SX_PROFILE_END(R36SX_PROFILE_KEYBOARD_TICK, profile_keyboard_tick_1);
        if (main_loop_count < 8u) {
            r36sx_pico286_debug_log("main: before exec loop=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        R36SX_PROFILE_BEGIN(profile_exec86);
        uint64_t exec_start_us = r36sx_pico286_now_us();
        exec86(cpu_exec_loops_per_frame);
        exec_elapsed_us = r36sx_pico286_now_us() - exec_start_us;
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_EXEC86, profile_exec86,
                                cpu_exec_loops_per_frame);
        R36SX_PROFILE_BEGIN(profile_disk_flush);
        r36sx_pico286_disk_flush_pending();
        R36SX_PROFILE_END(R36SX_PROFILE_DISK_FLUSH, profile_disk_flush);
        R36SX_PROFILE_BEGIN(profile_keyboard_tick_2);
        r36sx_keyboard_tick();
        R36SX_PROFILE_END(R36SX_PROFILE_KEYBOARD_TICK, profile_keyboard_tick_2);
        if (main_loop_count < 8u) {
            r36sx_pico286_debug_log("main: after exec loop=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        if ((++main_loop_count % 300u) == 0u) {
            r36sx_pico286_debug_log("main: alive loops=%u videomode=0x%x exec_loops=%u",
                                    main_loop_count, videomode,
                                    cpu_exec_loops_per_frame);
        }
        if (main_loop_count <= 8u) {
            r36sx_pico286_debug_log("main: before mfb_update loop=%u",
                                    main_loop_count);
        }
        R36SX_PROFILE_BEGIN(profile_mfb_update);
        int mfb_update_rc = mfb_update(SCREEN, 0);
        R36SX_PROFILE_END(R36SX_PROFILE_MFB_UPDATE, profile_mfb_update);
        if (mfb_update_rc < 0) {
            r36sx_pico286_debug_log("main: mfb_update requested stop");
            running = 0;
            break;
        }
        R36SX_PROFILE_BEGIN(profile_keyboard_tick_3);
        r36sx_keyboard_tick();
        R36SX_PROFILE_END(R36SX_PROFILE_KEYBOARD_TICK, profile_keyboard_tick_3);
        {
            uint32_t adjusted_exec_loops =
                r36sx_pico286_adjust_exec_loops(
                    cpu_exec_loops_per_frame,
                    cpu_exec_loops_per_frame_max,
                    exec_elapsed_us,
                    main_loop_frame_us);

            if (adjusted_exec_loops != cpu_exec_loops_per_frame) {
                if (main_loop_count <= 8u || (main_loop_count % 300u) == 0u) {
                    r36sx_pico286_debug_log(
                        "main: adjust exec_loops %u->%u exec_us=%llu target_us=%u",
                        cpu_exec_loops_per_frame,
                        adjusted_exec_loops,
                        (unsigned long long)exec_elapsed_us,
                        main_loop_frame_us);
                }
                cpu_exec_loops_per_frame = adjusted_exec_loops;
            }
        }
        if (main_loop_count <= 8u) {
            r36sx_pico286_debug_log("main: after mfb_update loop=%u",
                                    main_loop_count);
        }
        r36sx_profile_maybe_log();
        r36sx_pico286_wait_for_next_main_frame(&next_main_loop_us,
                                               main_loop_frame_us);
    }
    r36sx_pico286_debug_log("main: leaving loop loops=%u", main_loop_count);

    if (sound_thread_started) {
        r36sx_pico286_debug_log("main: cancel sound thread");
        pthread_cancel(sound_tid);
        pthread_join(sound_tid, NULL);
    }
    if (ticks_thread_started) {
        r36sx_pico286_debug_log("main: cancel ticks thread");
        pthread_cancel(ticks_tid);
        pthread_join(ticks_tid, NULL);
    }

    // Clean up audio
    r36sx_pico286_debug_log("main: cleanup begin");
    r36sx_pico286_disk_flush_all();
    linux_audio_close();

    mfb_close();
    r36sx_pico286_debug_log("main: exit 0");
    return 0;
}
