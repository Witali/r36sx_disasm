#if defined(__linux__) && !defined(_WIN32)
#define R36SX_PICO286_USE_EVDEV_INPUT 1
#else
#define R36SX_PICO286_USE_EVDEV_INPUT 0
#endif

#include <pthread.h>
#include <unistd.h>
#if R36SX_PICO286_USE_EVDEV_INPUT
#include <cerrno>
#endif
#include <cstring>
#if R36SX_PICO286_USE_EVDEV_INPUT
#include <fcntl.h>
#endif
#include <signal.h>
#if R36SX_PICO286_USE_EVDEV_INPUT
#include <sys/ioctl.h>
#endif
#include <sys/time.h>
#include <time.h>
#include <cstdio>
#if R36SX_PICO286_USE_EVDEV_INPUT
#include <linux/input.h>
#endif
#include "MiniFB.h"
#include "emulator/emulator.h"
#include "emulator/includes/font8x16.h"
#include "emulator/includes/font8x8.h"
#include "emu8950.h"
#include "linux-audio.h"
#include "r36sx_app_stats.h"
#include "r36sx_cpu.h"
#include "r36sx_debug_config.h"
#include "r36sx_disk_config.h"
#include "r36sx_mips_dsp.h"
#include "r36sx_profile.h"
#include "../../common/r36sx_screen_keyboard.h"

static uint16_t ALIGN(4, SCREEN[640 * 480]);
uint8_t ALIGN(4, DEBUG_VRAM[80 * 10]) = {0};

int cursor_blink_state = 0;
uint8_t log_debug = 0;

/*
 * Shadow Palette:
 * VGA/TGA/CGA keep their original RGB888 palette arrays for emulated hardware
 * reads.  The Linux/R36SX renderer consumes these RGB565 shadows, updated when
 * the original palette changes, so frame rendering does not reconvert colors.
 */
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
extern "C" void r36sx_pico286_post_reset(void);
extern "C" uint64_t sb_samplerate;

#define R36SX_AUDIO_DRIVER_RATE 44100u
#define R36SX_AUDIO_CHANNELS 2u
#define R36SX_AUDIO_BUFFER_MIN_FPS 10u
#define R36SX_AUDIO_BUFFER_MAX_FRAMES \
    (R36SX_AUDIO_DRIVER_RATE / R36SX_AUDIO_BUFFER_MIN_FPS)
#define R36SX_AUDIO_BUFFER_COUNT 4u
#define R36SX_TICKS_THREAD_SLEEP_US 1000u
#define R36SX_HLT_SLEEP_US 1000u
#define R36SX_MAIN_LOOP_DEFAULT_FPS 60u
#define R36SX_EXEC86_MIN_LOOPS 1000u
#define R36SX_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

#if R36SX_PICO286_USE_EVDEV_INPUT
enum {
    R36SX_EVDEV_MAX_FDS = 16,
    R36SX_EVDEV_RESCAN_USEC = 2000000
};

struct r36sx_physical_keyboard_state {
    int fds[R36SX_EVDEV_MAX_FDS];
    char paths[R36SX_EVDEV_MAX_FDS][32];
    uint64_t next_scan_us;
};

struct r36sx_physical_mouse_state {
    int fds[R36SX_EVDEV_MAX_FDS];
    char paths[R36SX_EVDEV_MAX_FDS][32];
    uint8_t buttons[R36SX_EVDEV_MAX_FDS];
    int pending_dx[R36SX_EVDEV_MAX_FDS];
    int pending_dy[R36SX_EVDEV_MAX_FDS];
    uint8_t pending_event[R36SX_EVDEV_MAX_FDS];
    uint64_t next_scan_us;
};

static struct r36sx_physical_keyboard_state g_physical_keyboard;
static struct r36sx_physical_mouse_state g_physical_mouse;
#endif

static int16_t audio_buffers[R36SX_AUDIO_BUFFER_COUNT]
                            [R36SX_AUDIO_BUFFER_MAX_FRAMES * 2u] = {};
static int16_t audio_playback_buffer[R36SX_AUDIO_BUFFER_MAX_FRAMES * 2u] = {};
static int sample_index = 0;
static uint32_t audio_write_buffer = 0;
static uint32_t audio_ready_head = 0;
static uint32_t audio_ready_count = 0;
static uint32_t audio_ready_frames[R36SX_AUDIO_BUFFER_COUNT] = {};
static volatile int soft_reset_requested = 0;
static volatile int soft_reset_in_progress = 0;
#if defined(R36SX_VIDEO_DIRTY_TRACKING) && R36SX_VIDEO_DIRTY_TRACKING
static volatile uint32_t g_video_dirty = 1;
#endif
static uint32_t g_main_loop_frame_us = 1000000u / R36SX_MAIN_LOOP_DEFAULT_FPS;
static uint32_t g_audio_packet_target_frames =
    R36SX_AUDIO_DRIVER_RATE / R36SX_MAIN_LOOP_DEFAULT_FPS;
static uint32_t g_audio_buffer_capacity_frames =
    R36SX_AUDIO_BUFFER_MAX_FRAMES;

static inline uint32_t vga_vram_cell(uint32_t index) {
    return VIDEORAM[index & 0xFFFFu];
}

static inline uint8_t vga_vram_byte(uint32_t index) {
    return (uint8_t)(vga_vram_cell(index) & 0xFFu);
}

static inline void put_pixel_repeat(uint16_t **pixels, uint16_t color,
                                    uint32_t count)
{
    while (count--) {
        *(*pixels)++ = color;
    }
}

extern "C" void adlib_getsample(int16_t *sndptr, intptr_t numsamples);

extern "C" void r36sx_pico286_request_soft_reset(void) {
    soft_reset_requested = 1;
}

#if defined(R36SX_VIDEO_DIRTY_TRACKING) && R36SX_VIDEO_DIRTY_TRACKING
extern "C" void r36sx_pico286_video_mark_dirty(void)
{
    __sync_lock_test_and_set(&g_video_dirty, 1u);
}

static int r36sx_pico286_video_take_dirty(void)
{
    return __sync_lock_test_and_set(&g_video_dirty, 0u) != 0u;
}
#else
static int r36sx_pico286_video_take_dirty(void)
{
    return 1;
}
#endif

static void r36sx_pico286_soft_reset(void);

static void r36sx_pico286_reset_pic(void)
{
    memset(&i8259_controller, 0, sizeof(i8259_controller));
    i8259_controller.interrupt_vector_offset = 8;
    i8259_controller.initialization_command_words[2] = 8;

    /*
     * The normal R36SX boot path uses BIOS services implemented in the emulator
     * instead of a full PC BIOS ROM, so keep the historical "ready PIC" state
     * for DOS compatibility.  A diagnostic BIOS ROM such as test386/test286 must not
     * receive synthetic timer IRQs before it has explicitly initialized the PIC.
     */
    if (r36sx_pico286_bios_mode() != R36SX_PICO286_BIOS_NORMAL) {
        i8259_controller.interrupt_mask_register = 0xFF;
        i8259_controller.controller_enabled = 0;
    } else {
        i8259_controller.interrupt_mask_register = 0x00;
        i8259_controller.controller_enabled = 1;
    }
    r36sx_pico286_debug_log("pic: reset bios=%s enabled=%u mask=%02x vector=%02x",
                            r36sx_pico286_bios_mode_name(),
                            (unsigned int)i8259_controller.controller_enabled,
                            (unsigned int)i8259_controller.interrupt_mask_register,
                            (unsigned int)i8259_controller.interrupt_vector_offset);
}

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

static uint32_t r36sx_pico286_audio_frames_for_elapsed(uint64_t elapsed_ticks,
                                                       uint32_t sample_rate,
                                                       uint64_t ticks_per_sec)
{
    if (sample_rate == 0 || ticks_per_sec == 0) {
        return 0;
    }

    uint64_t frames =
        (elapsed_ticks * (uint64_t)sample_rate + ticks_per_sec / 2u) /
        ticks_per_sec;
    if (frames > 0xffffffffull) {
        return 0xffffffffu;
    }
    return (uint32_t)frames;
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
    if (vga_svga_mode_active()) {
        return 480;
    }

    switch (videomode) {
        case 0x11:
        case 0x12:
            return 480;
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
#if R36SX_DEBUG_STDOUT_CAPTURE
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

extern "C" void r36sx_pico286_vga_palette565_set(uint8_t index,
                                                  uint32_t color)
{
    vga_palette565[index] = r36sx_mips_rgb888_to_rgb565(color);
}

extern "C" void r36sx_pico286_vga_palette565_set_all(const uint32_t *palette,
                                                      uint16_t count)
{
    if (count > 256) {
        count = 256;
    }
    r36sx_mips_dsp_rgb888_to_rgb565(vga_palette565, palette, count);
}

extern "C" void r36sx_pico286_tga_palette565_set(uint8_t index,
                                                  uint32_t color)
{
    if (index < 16) {
        tga_palette565[index] = r36sx_mips_rgb888_to_rgb565(color);
    }
}

static void init_palettes565_once(void)
{
    if (static_palettes565_ready) {
        return;
    }

    r36sx_mips_dsp_rgb888_to_rgb565(cga_palette565, cga_palette, 16);
    for (int p = 0; p < 3; p++) {
        r36sx_mips_dsp_rgb888_to_rgb565(cga_composite_palette565[p],
                                        cga_composite_palette[p], 16);
    }
    r36sx_mips_dsp_rgb888_to_rgb565(tga_palette565, tga_palette, 16);
    r36sx_mips_dsp_rgb888_to_rgb565(vga_palette565, vga_palette, 256);
    static_palettes565_ready = 1;
}

static inline void fill_black_row(uint16_t *pixels)
{
    for (int x = 0; x < 640; x++) {
        *pixels++ = 0;
    }
}

static inline uint16_t mda_text_color(uint8_t attr, int is_foreground)
{
    uint16_t fg = (attr & 0x08) ? 0xffff :
                  r36sx_mips_rgb888_to_rgb565(0xc4c4c4u);
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

static inline uint8_t vga_chain4_pixel(uint32_t byte_offset)
{
    uint32_t cell = vga_vram_cell(byte_offset >> 2);
    return (uint8_t)(cell >> ((byte_offset & 3u) << 3));
}

static inline uint16_t svga_read_rgb565(uint32_t offset)
{
    if (offset + 1u >= SVGA_VRAM_SIZE) {
        return 0;
    }
    return (uint16_t)SVGA_VRAM[offset] |
           ((uint16_t)SVGA_VRAM[offset + 1u] << 8);
}

static inline void svga_render_row(uint16_t *pixels, int y)
{
    uint32_t src_y;
    uint32_t row;

    if (!vga_svga_mode_active() || vga_svga_pitch == 0) {
        fill_black_row(pixels);
        return;
    }

    src_y = vga_svga_height == 480u ?
            (uint32_t)y :
            ((uint32_t)y * vga_svga_height + 240u) / 480u;
    if (src_y >= vga_svga_height) {
        fill_black_row(pixels);
        return;
    }
    row = src_y * vga_svga_pitch;

    if (vga_svga_bpp == 16) {
        for (uint32_t x = 0; x < 640u; x++) {
            uint32_t src_x = vga_svga_width == 640u ?
                             x :
                             (x * vga_svga_width + 320u) / 640u;
            *pixels++ = svga_read_rgb565(row + src_x * 2u);
        }
    } else {
        for (uint32_t x = 0; x < 640u; x++) {
            uint32_t src_x = vga_svga_width == 640u ?
                             x :
                             (x * vga_svga_width + 320u) / 640u;
            uint32_t offset = row + src_x;
            uint8_t color = offset < SVGA_VRAM_SIZE ? SVGA_VRAM[offset] : 0;
            *pixels++ = vga_palette565[color];
        }
    }
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
    static int v = -1;
    init_palettes565_once();

    if (v != videomode) {
        printf("videomode %x %x\n", videomode, v);
        v = videomode;
    }

    uint8_t cols = 80;
    for (int y = 0; y < 480; y++) {
        uint16_t *pixels = SCREEN + y * 640;

        if (vga_svga_mode_active()) {
            svga_render_row(pixels, y);
            continue;
        }

        if (y < 400)
            switch (videomode) {
                case 0x00:
                case 0x01: {
                    uint16_t y_div_16 = y / 16; // Precompute y / 16
                    uint8_t glyph_line = (y / 2) % 8; // Precompute y % 8 for font lookup
                    uint32_t text_base =
                        0x8000u + ((vram_offset & 0xffffu) << 1) +
                        (uint32_t)y_div_16 * 80u;

                    for (int column = 0; column < 40; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        uint8_t charcode = vga_vram_byte(cell);
                        uint8_t glyph_pixels = font_8x8[charcode * 8 + glyph_line]; // Glyph row from font
                        uint8_t color = vga_vram_byte(cell + 1u); // Color attribute

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
                            put_pixel_repeat(&pixels,
                                             cga_palette565[pixel_color], 2u);
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

                    uint32_t text_base =
                        0x8000u + ((vram_offset & 0xffffu) << 1) +
                        (uint32_t)y_div_16 * 160u;
                    for (uint8_t column = 0; column < 80; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        // Access vidram and font data once per character
                        uint8_t charcode = vga_vram_byte(cell); // Character code
                        uint8_t glyph_row = font_8x16[charcode * 16 + glyph_line]; // Glyph row from font
                        uint8_t color = vga_vram_byte(cell + 1u); // Color attribute

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
                    uint32_t cga_base =
                        cga_graphics_base() + cga_graphics_row_offset(y);
                    uint8_t *current_cga_palette = (uint8_t *) cga_gfxpal[cga_colorset][cga_intensity];

                    // Each byte containing 4 pixels
                    for (uint32_t x = 0; x < 320u / 4u; x++) {
                        uint8_t cga_byte = vga_vram_byte(cga_base + x);

                        // Extract all four 2-bit pixels from the CGA byte
                        // and write each pixel twice for horizontal scaling
                        put_pixel_repeat(&pixels,
                            cga_palette565[cga_byte >> 6 & 3
                                           ? current_cga_palette[cga_byte >> 6 & 3]
                                           : cga_foreground_color], 2u);
                        put_pixel_repeat(&pixels,
                            cga_palette565[cga_byte >> 4 & 3
                                           ? current_cga_palette[cga_byte >> 4 & 3]
                                           : cga_foreground_color], 2u);
                        put_pixel_repeat(&pixels,
                            cga_palette565[cga_byte >> 2 & 3
                                           ? current_cga_palette[cga_byte >> 2 & 3]
                                           : cga_foreground_color], 2u);
                        put_pixel_repeat(&pixels,
                            cga_palette565[cga_byte >> 0 & 3
                                           ? current_cga_palette[cga_byte >> 0 & 3]
                                           : cga_foreground_color], 2u);
                    }
                    break;
                }
                case 0x06: {
                    uint32_t cga_base =
                        cga_graphics_base() + cga_graphics_row_offset(y);

                    // Each byte containing 8 pixels
                    for (uint32_t x = 0; x < 640u / 8u; x++) {
                        uint8_t cga_byte = vga_vram_byte(cga_base + x);

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
                    uint32_t cga_base = vram_offset +
                        ((uint32_t)y & 3u) * 8192u +
                        ((uint32_t)y / 4u) * (uint32_t)cols;
                    // Each byte containing 8 pixels
                    for (uint32_t x = 0; x < 640u / 8u; x++) {
                        // The emulated video aperture is 64K; keep renderer
                        // reads wrapped exactly like the write path.
                        uint8_t cga_byte = vga_vram_byte(cga_base + x);

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
                    uint32_t text_base =
                        ((vram_offset & 0xffffu) << 1) +
                        (uint32_t)text_row_index * 160u;

                    for (uint8_t column = 0; column < 80; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        uint8_t charcode = vga_vram_byte(cell);
                        uint8_t attr = vga_vram_byte(cell + 1u);
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

                    uint32_t source_y = (uint32_t)y / 2u;
                    uint32_t cga_base = tga_offset +
                        (source_y >> 1) * 80u + (source_y & 1u) * 8192u;

                    // Each byte containing 8 pixels
                    for (uint32_t x = 0; x < 640u / 8u; x++) {
                        uint8_t cga_byte = vga_vram_byte(cga_base + x);
                        uint8_t color1 = cga_byte >> 4 & 15;
                        uint8_t color2 = cga_byte & 15;

                        if (!color1 && videomode == 0x8) color1 = cga_foreground_color;
                        if (!color2 && videomode == 0x8) color2 = cga_foreground_color;

                        put_pixel_repeat(&pixels, palette[color1], 4u);
                        put_pixel_repeat(&pixels, palette[color2], 4u);
                    }

                    break;
                }
                case 0x09: /* tandy 320x200 16 color */ {
                    uint32_t tga_base = tga_offset +
                        (((uint32_t)y / 2u) & 3u) * 8192u +
                        ((uint32_t)y / 8u) * 160u;

                    // Each byte containing 4 pixels
                    for (uint32_t x = 0; x < 320u / 2u; x++) {
                        uint8_t tga_byte = vga_vram_byte(tga_base + x);
                        put_pixel_repeat(&pixels,
                            tga_palette565[tga_palette_map[tga_byte >> 4 & 15]],
                            2u);
                        put_pixel_repeat(&pixels,
                            tga_palette565[tga_palette_map[tga_byte & 15]],
                            2u);
                    }
                    break;
                }
                case 0x0a: /* tandy 640x200 16 color */ {
                    uint32_t tga_base = ((uint32_t)y / 2u) * 320u;

                    // Each byte contains 2 pixels
                    for (uint32_t x = 0; x < 640u / 2u; x++) {
                        uint8_t tga_byte = vga_vram_byte(tga_base + x);
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
                    if (vga_mode13_unchained_planar_active()) {
                        const uint32_t visible_cells = 320u / 4u;
                        /*
                         * Mode X/Y: 256-color mode 13h with chain4 disabled.
                         * One addressable cell contains four adjacent pixels,
                         * one byte per VGA plane.  CRTC offset 28h therefore
                         * means 80 cells per 320-pixel scanline.
                         */
                        uint32_t stride = vga_crtc_planar_stride(visible_cells, 0);
                        uint32_t vram_base = vram_offset +
                                             ((uint32_t)y >> 1) * stride;
                        for (uint32_t x = 0; x < visible_cells; x++) {
                            uint32_t four_pixels = vga_vram_cell(vram_base + x);
                            put_pixel_repeat(&pixels,
                                             vga_palette565[four_pixels & 0xFFu],
                                             2u);
                            put_pixel_repeat(&pixels,
                                             vga_palette565[(four_pixels >> 8) & 0xFFu],
                                             2u);
                            put_pixel_repeat(&pixels,
                                             vga_palette565[(four_pixels >> 16) & 0xFFu],
                                             2u);
                            put_pixel_repeat(&pixels,
                                             vga_palette565[(four_pixels >> 24) & 0xFFu],
                                             2u);
                        }
                    } else {
                        uint32_t stride = vga_crtc_chunky_stride(320u);
                        uint32_t vram_base = vram_offset +
                                             ((uint32_t)y >> 1) * stride;
                        for (int x = 0; x < 320; x++) {
                            uint16_t color =
                                vga_palette565[vga_chain4_pixel(vram_base + (uint32_t)x)];
                            put_pixel_repeat(&pixels, color, 2u);
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
                    uint32_t text_base = 0x8000u + (uint32_t)y_div_4 * 160u;
                    for (uint8_t column = 0; column < cols; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        // Access vidram and font data once per character
                        uint8_t charcode = vga_vram_byte(cell);
                        uint8_t glyph_row = font_8x8[charcode * 8 + odd_even]; // Glyph row from font
                        uint8_t color = vga_vram_byte(cell + 1u);

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
                    uint32_t text_base = 0x8000u + (uint32_t)y_div_2 * 80u +
                        ((uint32_t)y_div_2 & (1u * 8192u));
                    for (int column = 0; column < 40; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        // Access vidram and font data once per character
                        uint8_t charcode = vga_vram_byte(cell);
                        uint8_t glyph_row = font_8x8[charcode * 8]; // Glyph row from font
                        uint8_t color = vga_vram_byte(cell + 1u);

#pragma GCC unroll(8)
                        for (int bit = 0; bit < 8; bit++) {
                            put_pixel_repeat(&pixels,
                                cga_palette565[glyph_row >> bit & 1
                                               ? color & 0x0f : color >> 4],
                                2u);
                        }
                    }
                    break;
                }
                case 0x87: {
                    /* 40x46 ??? */
                    int y_div_2 = y / 8; // Precompute y / 2
                    // Calculate screen position
                    uint32_t text_base = 0x8000u + (uint32_t)y_div_2 * 80u +
                        ((uint32_t)y_div_2 & (1u * 8192u));
                    for (int column = 0; column < 40; column++) {
                        uint32_t cell = text_base + (uint32_t)column * 2u;
                        // Access vidram and font data once per character
                        uint8_t charcode = vga_vram_byte(cell);
                        uint8_t glyph_row = font_8x8[charcode * 8 + (y_div_2 % 8)]; // Glyph row from font
                        uint8_t color = vga_vram_byte(cell + 1u);

#pragma GCC unroll(8)
                        for (int bit = 0; bit < 8; bit++) {
                            put_pixel_repeat(&pixels,
                                cga_palette565[glyph_row >> bit & 1
                                               ? color & 0x0f : color >> 4],
                                2u);
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
    int extended = 0;

    if (keycode == R36SX_SCREEN_KEY_PAUSE) {
        if (isKeyDown) {
            static const uint8_t pause_make[] = {
                0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5
            };
            for (size_t i = 0; i < sizeof(pause_make); i++) {
                r36sx_keyboard_enqueue_scancode(pause_make[i]);
            }
        }
        return;
    }

    if (keycode == R36SX_SCREEN_KEY_PRINT) {
        static const uint8_t print_make[] = {0xE0, 0x2A, 0xE0, 0x37};
        static const uint8_t print_break[] = {0xE0, 0xB7, 0xE0, 0xAA};
        const uint8_t *seq = isKeyDown ? print_make : print_break;
        for (size_t i = 0; i < sizeof(print_make); i++) {
            r36sx_keyboard_enqueue_scancode(seq[i]);
        }
        return;
    }

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
        case 37: scancode = 0x4B; extended = 1;
            break; // Left
        case 38: scancode = 0x48; extended = 1;
            break; // Up
        case 39: scancode = 0x4D; extended = 1;
            break; // Right
        case 40: scancode = 0x50; extended = 1;
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
        case R36SX_SCREEN_KEY_SHIFT:
        case R36SX_SCREEN_KEY_LSHIFT: scancode = 0x2A;
            break; // Left Shift
        case R36SX_SCREEN_KEY_RSHIFT: scancode = 0x36;
            break; // Right Shift
        case R36SX_SCREEN_KEY_CONTROL:
        case R36SX_SCREEN_KEY_LCONTROL: scancode = 0x1D;
            break; // Left Ctrl
        case R36SX_SCREEN_KEY_RCONTROL: scancode = 0x1D; extended = 1;
            break; // Right Ctrl
        case R36SX_SCREEN_KEY_MENU:
        case R36SX_SCREEN_KEY_LMENU: scancode = 0x38;
            break; // Left Alt
        case R36SX_SCREEN_KEY_RMENU: scancode = 0x38; extended = 1;
            break; // Right Alt
        case 20: scancode = 0x3A;
            break; // Caps Lock
        case R36SX_SCREEN_KEY_LWIN: scancode = 0x5B; extended = 1;
            break; // Left Windows
        case R36SX_SCREEN_KEY_RWIN: scancode = 0x5C; extended = 1;
            break; // Right Windows
        case R36SX_SCREEN_KEY_APPS: scancode = 0x5D; extended = 1;
            break; // Context menu
        case R36SX_SCREEN_KEY_NUMPAD0: scancode = 0x52;
            break; // Numpad 0
        case R36SX_SCREEN_KEY_NUMPAD1: scancode = 0x4F;
            break; // Numpad 1
        case R36SX_SCREEN_KEY_NUMPAD2: scancode = 0x50;
            break; // Numpad 2
        case R36SX_SCREEN_KEY_NUMPAD3: scancode = 0x51;
            break; // Numpad 3
        case R36SX_SCREEN_KEY_NUMPAD4: scancode = 0x4B;
            break; // Numpad 4
        case R36SX_SCREEN_KEY_NUMPAD5: scancode = 0x4C;
            break; // Numpad 5
        case R36SX_SCREEN_KEY_NUMPAD6: scancode = 0x4D;
            break; // Numpad 6
        case R36SX_SCREEN_KEY_NUMPAD7: scancode = 0x47;
            break; // Numpad 7
        case R36SX_SCREEN_KEY_NUMPAD8: scancode = 0x48;
            break; // Numpad 8
        case R36SX_SCREEN_KEY_NUMPAD9: scancode = 0x49;
            break; // Numpad 9
        case R36SX_SCREEN_KEY_MULTIPLY: scancode = 0x37;
            break; // Numpad *
        case R36SX_SCREEN_KEY_ADD:
        case R36SX_SCREEN_KEY_SEPARATOR: scancode = 0x4E;
            break; // Numpad +
        case R36SX_SCREEN_KEY_SUBTRACT: scancode = 0x4A;
            break; // Numpad -
        case R36SX_SCREEN_KEY_DECIMAL: scancode = 0x53;
            break; // Numpad .
        case R36SX_SCREEN_KEY_DIVIDE: scancode = 0x35; extended = 1;
            break; // Numpad /
        case R36SX_SCREEN_KEY_NUMLOCK: scancode = 0x45;
            break; // Num Lock
        case 33: scancode = 0x49; extended = 1;
            break; // Page Up
        case 34: scancode = 0x51; extended = 1;
            break; // Page Down
        case 35: scancode = 0x4F; extended = 1;
            break; // End
        case 36: scancode = 0x47; extended = 1;
            break; // Home
        case 45: scancode = 0x52; extended = 1;
            break; // Insert
        case 46: scancode = 0x53; extended = 1;
            break; // Delete
        case 145: scancode = 0x46;
            break; // Scroll Lock
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
        if (extended) {
            r36sx_keyboard_enqueue_scancode(0xE0);
        }
        r36sx_keyboard_enqueue_scancode((uint8_t)scancode);
    }
}

#if R36SX_PICO286_USE_EVDEV_INPUT
extern "C" void HandleMouseRelative(int dx, int dy, int buttons);

#define R36SX_EVDEV_BITS_PER_LONG ((int)(sizeof(unsigned long) * 8))
#define R36SX_EVDEV_BIT_WORD(nr) ((nr) / R36SX_EVDEV_BITS_PER_LONG)
#define R36SX_EVDEV_BIT_MASK(nr) (1ul << ((nr) % R36SX_EVDEV_BITS_PER_LONG))

static int r36sx_evdev_test_bit(int bit, const unsigned long *bits,
                                int word_count)
{
    int word = R36SX_EVDEV_BIT_WORD(bit);
    if (word < 0 || word >= word_count) {
        return 0;
    }
    return (bits[word] & R36SX_EVDEV_BIT_MASK(bit)) != 0;
}

static void r36sx_physical_keyboard_init(void)
{
    memset(&g_physical_keyboard, 0, sizeof(g_physical_keyboard));
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        g_physical_keyboard.fds[i] = -1;
    }
}

static void r36sx_physical_keyboard_close_index(int index)
{
    if (index < 0 || index >= R36SX_EVDEV_MAX_FDS) {
        return;
    }
    if (g_physical_keyboard.fds[index] >= 0) {
        close(g_physical_keyboard.fds[index]);
    }
    g_physical_keyboard.fds[index] = -1;
    g_physical_keyboard.paths[index][0] = '\0';
}

static void r36sx_physical_keyboard_close_all(void)
{
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        r36sx_physical_keyboard_close_index(i);
    }
}

static int r36sx_physical_keyboard_path_is_open(const char *path)
{
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        if (g_physical_keyboard.fds[i] >= 0 &&
            strcmp(g_physical_keyboard.paths[i], path) == 0) {
            return 1;
        }
    }
    return 0;
}

static int r36sx_physical_keyboard_is_keyboard_fd(int fd)
{
    unsigned long ev_bits[R36SX_EVDEV_BIT_WORD(EV_MAX) + 1];
    unsigned long key_bits[R36SX_EVDEV_BIT_WORD(KEY_MAX) + 1];

    memset(ev_bits, 0, sizeof(ev_bits));
    memset(key_bits, 0, sizeof(key_bits));
    if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0) {
        return 0;
    }
    if (!r36sx_evdev_test_bit(EV_KEY, ev_bits,
                              (int)R36SX_ARRAY_COUNT(ev_bits))) {
        return 0;
    }
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits) < 0) {
        return 0;
    }
    return r36sx_evdev_test_bit(KEY_A, key_bits,
                                (int)R36SX_ARRAY_COUNT(key_bits)) &&
           r36sx_evdev_test_bit(KEY_ENTER, key_bits,
                                (int)R36SX_ARRAY_COUNT(key_bits));
}

static void r36sx_physical_mouse_init(void)
{
    memset(&g_physical_mouse, 0, sizeof(g_physical_mouse));
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        g_physical_mouse.fds[i] = -1;
    }
}

static void r36sx_physical_mouse_close_index(int index)
{
    if (index < 0 || index >= R36SX_EVDEV_MAX_FDS) {
        return;
    }
    if (g_physical_mouse.fds[index] >= 0) {
        close(g_physical_mouse.fds[index]);
    }
    if (g_physical_mouse.buttons[index] != 0 ||
        g_physical_mouse.pending_event[index] != 0) {
        HandleMouseRelative(0, 0, 0);
    }
    g_physical_mouse.fds[index] = -1;
    g_physical_mouse.paths[index][0] = '\0';
    g_physical_mouse.buttons[index] = 0;
    g_physical_mouse.pending_dx[index] = 0;
    g_physical_mouse.pending_dy[index] = 0;
    g_physical_mouse.pending_event[index] = 0;
}

static void r36sx_physical_mouse_close_all(void)
{
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        r36sx_physical_mouse_close_index(i);
    }
}

static int r36sx_physical_mouse_path_is_open(const char *path)
{
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        if (g_physical_mouse.fds[i] >= 0 &&
            strcmp(g_physical_mouse.paths[i], path) == 0) {
            return 1;
        }
    }
    return 0;
}

static int r36sx_physical_mouse_is_mouse_fd(int fd)
{
    unsigned long ev_bits[R36SX_EVDEV_BIT_WORD(EV_MAX) + 1];
    unsigned long rel_bits[R36SX_EVDEV_BIT_WORD(REL_MAX) + 1];
    unsigned long key_bits[R36SX_EVDEV_BIT_WORD(KEY_MAX) + 1];

    memset(ev_bits, 0, sizeof(ev_bits));
    memset(rel_bits, 0, sizeof(rel_bits));
    memset(key_bits, 0, sizeof(key_bits));
    if (ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), ev_bits) < 0) {
        return 0;
    }
    if (!r36sx_evdev_test_bit(EV_REL, ev_bits,
                              (int)R36SX_ARRAY_COUNT(ev_bits))) {
        return 0;
    }
    if (ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bits)), rel_bits) < 0) {
        return 0;
    }
    if (!r36sx_evdev_test_bit(REL_X, rel_bits,
                              (int)R36SX_ARRAY_COUNT(rel_bits)) ||
        !r36sx_evdev_test_bit(REL_Y, rel_bits,
                              (int)R36SX_ARRAY_COUNT(rel_bits))) {
        return 0;
    }
    if (!r36sx_evdev_test_bit(EV_KEY, ev_bits,
                              (int)R36SX_ARRAY_COUNT(ev_bits))) {
        return 1;
    }
    if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits) < 0) {
        return 1;
    }
    return r36sx_evdev_test_bit(BTN_LEFT, key_bits,
                                (int)R36SX_ARRAY_COUNT(key_bits)) ||
           r36sx_evdev_test_bit(BTN_RIGHT, key_bits,
                                (int)R36SX_ARRAY_COUNT(key_bits));
}

static void r36sx_physical_keyboard_try_open(const char *path)
{
    if (r36sx_physical_keyboard_path_is_open(path)) {
        return;
    }

    int slot = -1;
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        if (g_physical_keyboard.fds[i] < 0) {
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
    if (!r36sx_physical_keyboard_is_keyboard_fd(fd)) {
        close(fd);
        return;
    }

    g_physical_keyboard.fds[slot] = fd;
    snprintf(g_physical_keyboard.paths[slot],
             sizeof(g_physical_keyboard.paths[slot]), "%s", path);
    r36sx_pico286_debug_log("keyboard: opened physical input %s", path);
}

static void r36sx_physical_mouse_try_open(const char *path)
{
    if (r36sx_physical_mouse_path_is_open(path)) {
        return;
    }

    int slot = -1;
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        if (g_physical_mouse.fds[i] < 0) {
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
    if (!r36sx_physical_mouse_is_mouse_fd(fd)) {
        close(fd);
        return;
    }

    g_physical_mouse.fds[slot] = fd;
    snprintf(g_physical_mouse.paths[slot],
             sizeof(g_physical_mouse.paths[slot]), "%s", path);
    r36sx_pico286_debug_log("mouse: opened physical input %s", path);
}

static void r36sx_physical_keyboard_scan(void)
{
    uint64_t now = r36sx_pico286_now_us();
    if (now < g_physical_keyboard.next_scan_us) {
        return;
    }
    g_physical_keyboard.next_scan_us = now + R36SX_EVDEV_RESCAN_USEC;

    for (int i = 0; i < 32; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        r36sx_physical_keyboard_try_open(path);
    }
}

static void r36sx_physical_mouse_scan(void)
{
    uint64_t now = r36sx_pico286_now_us();
    if (now < g_physical_mouse.next_scan_us) {
        return;
    }
    g_physical_mouse.next_scan_us = now + R36SX_EVDEV_RESCAN_USEC;

    for (int i = 0; i < 32; i++) {
        char path[32];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        r36sx_physical_mouse_try_open(path);
    }
}

static unsigned int r36sx_evdev_key_to_screen_key(unsigned int code)
{
    if (code >= KEY_1 && code <= KEY_9) {
        return '1' + (code - KEY_1);
    }
    if (code >= KEY_F1 && code <= KEY_F10) {
        return R36SX_SCREEN_KEY_F1 + (code - KEY_F1);
    }

    switch (code) {
        case KEY_0: return '0';
        case KEY_A: return 'A';
        case KEY_B: return 'B';
        case KEY_C: return 'C';
        case KEY_D: return 'D';
        case KEY_E: return 'E';
        case KEY_F: return 'F';
        case KEY_G: return 'G';
        case KEY_H: return 'H';
        case KEY_I: return 'I';
        case KEY_J: return 'J';
        case KEY_K: return 'K';
        case KEY_L: return 'L';
        case KEY_M: return 'M';
        case KEY_N: return 'N';
        case KEY_O: return 'O';
        case KEY_P: return 'P';
        case KEY_Q: return 'Q';
        case KEY_R: return 'R';
        case KEY_S: return 'S';
        case KEY_T: return 'T';
        case KEY_U: return 'U';
        case KEY_V: return 'V';
        case KEY_W: return 'W';
        case KEY_X: return 'X';
        case KEY_Y: return 'Y';
        case KEY_Z: return 'Z';
        case KEY_ESC: return R36SX_SCREEN_KEY_ESCAPE;
        case KEY_BACKSPACE: return R36SX_SCREEN_KEY_BACK;
        case KEY_TAB: return R36SX_SCREEN_KEY_TAB;
        case KEY_ENTER:
        case KEY_KPENTER: return R36SX_SCREEN_KEY_RETURN;
        case KEY_SPACE: return R36SX_SCREEN_KEY_SPACE;
        case KEY_LEFTSHIFT: return R36SX_SCREEN_KEY_LSHIFT;
        case KEY_RIGHTSHIFT: return R36SX_SCREEN_KEY_RSHIFT;
        case KEY_LEFTCTRL: return R36SX_SCREEN_KEY_LCONTROL;
        case KEY_RIGHTCTRL: return R36SX_SCREEN_KEY_RCONTROL;
        case KEY_LEFTALT: return R36SX_SCREEN_KEY_LMENU;
        case KEY_RIGHTALT: return R36SX_SCREEN_KEY_RMENU;
        case KEY_CAPSLOCK: return R36SX_SCREEN_KEY_CAPITAL;
        case KEY_HOME: return R36SX_SCREEN_KEY_HOME;
        case KEY_UP: return R36SX_SCREEN_KEY_UP;
        case KEY_PAGEUP: return R36SX_SCREEN_KEY_PRIOR;
        case KEY_LEFT: return R36SX_SCREEN_KEY_LEFT;
        case KEY_RIGHT: return R36SX_SCREEN_KEY_RIGHT;
        case KEY_END: return R36SX_SCREEN_KEY_END;
        case KEY_DOWN: return R36SX_SCREEN_KEY_DOWN;
        case KEY_PAGEDOWN: return R36SX_SCREEN_KEY_NEXT;
        case KEY_INSERT: return R36SX_SCREEN_KEY_INSERT;
        case KEY_DELETE: return R36SX_SCREEN_KEY_DELETE;
        case KEY_SYSRQ: return R36SX_SCREEN_KEY_PRINT;
        case KEY_PAUSE: return R36SX_SCREEN_KEY_PAUSE;
        case KEY_LEFTMETA: return R36SX_SCREEN_KEY_LWIN;
        case KEY_RIGHTMETA: return R36SX_SCREEN_KEY_RWIN;
        case KEY_MENU:
        case KEY_COMPOSE: return R36SX_SCREEN_KEY_APPS;
        case KEY_NUMLOCK: return R36SX_SCREEN_KEY_NUMLOCK;
        case KEY_F11: return R36SX_SCREEN_KEY_F1 + 10;
        case KEY_F12: return R36SX_SCREEN_KEY_F1 + 11;
        case KEY_SCROLLLOCK: return R36SX_SCREEN_KEY_SCROLL;
        case KEY_SEMICOLON: return R36SX_SCREEN_KEY_OEM_1;
        case KEY_EQUAL: return R36SX_SCREEN_KEY_OEM_PLUS;
        case KEY_COMMA: return R36SX_SCREEN_KEY_OEM_COMMA;
        case KEY_MINUS: return R36SX_SCREEN_KEY_OEM_MINUS;
        case KEY_DOT: return R36SX_SCREEN_KEY_OEM_PERIOD;
        case KEY_SLASH: return R36SX_SCREEN_KEY_OEM_2;
        case KEY_GRAVE: return R36SX_SCREEN_KEY_OEM_3;
        case KEY_LEFTBRACE: return R36SX_SCREEN_KEY_OEM_4;
        case KEY_BACKSLASH: return R36SX_SCREEN_KEY_OEM_5;
        case KEY_RIGHTBRACE: return R36SX_SCREEN_KEY_OEM_6;
        case KEY_APOSTROPHE: return R36SX_SCREEN_KEY_OEM_7;
        case KEY_KP0: return R36SX_SCREEN_KEY_NUMPAD0;
        case KEY_KP1: return R36SX_SCREEN_KEY_NUMPAD1;
        case KEY_KP2: return R36SX_SCREEN_KEY_NUMPAD2;
        case KEY_KP3: return R36SX_SCREEN_KEY_NUMPAD3;
        case KEY_KP4: return R36SX_SCREEN_KEY_NUMPAD4;
        case KEY_KP5: return R36SX_SCREEN_KEY_NUMPAD5;
        case KEY_KP6: return R36SX_SCREEN_KEY_NUMPAD6;
        case KEY_KP7: return R36SX_SCREEN_KEY_NUMPAD7;
        case KEY_KP8: return R36SX_SCREEN_KEY_NUMPAD8;
        case KEY_KP9: return R36SX_SCREEN_KEY_NUMPAD9;
        case KEY_KPASTERISK: return R36SX_SCREEN_KEY_MULTIPLY;
        case KEY_KPPLUS: return R36SX_SCREEN_KEY_ADD;
        case KEY_KPMINUS: return R36SX_SCREEN_KEY_SUBTRACT;
        case KEY_KPDOT: return R36SX_SCREEN_KEY_DECIMAL;
        case KEY_KPSLASH: return R36SX_SCREEN_KEY_DIVIDE;
        default: return 0;
    }
}

static void r36sx_physical_keyboard_handle_key(unsigned int code, int value)
{
    unsigned int keycode = r36sx_evdev_key_to_screen_key(code);
    if (keycode == 0) {
        return;
    }
    if (value == 0 || value == 1 || value == 2) {
        HandleInput(keycode, value != 0);
    }
}

static uint8_t r36sx_physical_mouse_button_bit(unsigned int code)
{
    switch (code) {
        case BTN_RIGHT:
            return 1;
        case BTN_LEFT:
            return 2;
        default:
            return 0;
    }
}

static void r36sx_physical_mouse_flush(int index)
{
    if (index < 0 || index >= R36SX_EVDEV_MAX_FDS ||
        !g_physical_mouse.pending_event[index]) {
        return;
    }

    HandleMouseRelative(g_physical_mouse.pending_dx[index],
                        g_physical_mouse.pending_dy[index],
                        g_physical_mouse.buttons[index]);
    g_physical_mouse.pending_dx[index] = 0;
    g_physical_mouse.pending_dy[index] = 0;
    g_physical_mouse.pending_event[index] = 0;
}

static void r36sx_physical_mouse_handle_event(int index,
                                              const struct input_event *ev)
{
    if (index < 0 || index >= R36SX_EVDEV_MAX_FDS || ev == NULL) {
        return;
    }

    if (ev->type == EV_REL) {
        if (ev->code == REL_X) {
            g_physical_mouse.pending_dx[index] += ev->value;
            g_physical_mouse.pending_event[index] = 1;
        } else if (ev->code == REL_Y) {
            g_physical_mouse.pending_dy[index] += ev->value;
            g_physical_mouse.pending_event[index] = 1;
        }
        return;
    }

    if (ev->type == EV_KEY) {
        const uint8_t bit = r36sx_physical_mouse_button_bit(ev->code);
        if (bit != 0 && (ev->value == 0 || ev->value == 1)) {
            if (ev->value != 0) {
                g_physical_mouse.buttons[index] |= bit;
            } else {
                g_physical_mouse.buttons[index] &= (uint8_t)~bit;
            }
            g_physical_mouse.pending_event[index] = 1;
        }
        return;
    }

    if (ev->type == EV_SYN && ev->code == SYN_REPORT) {
        r36sx_physical_mouse_flush(index);
    }
}

static void r36sx_physical_keyboard_poll(void)
{
    r36sx_physical_keyboard_scan();
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        int fd = g_physical_keyboard.fds[i];
        if (fd < 0) {
            continue;
        }
        for (;;) {
            struct input_event ev;
            ssize_t n = read(fd, &ev, sizeof(ev));
            if (n == (ssize_t)sizeof(ev)) {
                if (ev.type == EV_KEY) {
                    r36sx_physical_keyboard_handle_key(ev.code, ev.value);
                }
                continue;
            }
            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK ||
                    errno == EINTR) {
                    break;
                }
                if (errno == ENODEV || errno == ENXIO) {
                    r36sx_pico286_debug_log(
                        "keyboard: removed physical input %s",
                        g_physical_keyboard.paths[i]);
                    r36sx_physical_keyboard_close_index(i);
                }
                break;
            }
            if (n == 0) {
                r36sx_pico286_debug_log(
                    "keyboard: removed physical input %s",
                    g_physical_keyboard.paths[i]);
                r36sx_physical_keyboard_close_index(i);
            }
            break;
        }
    }
}

static void r36sx_physical_mouse_poll(void)
{
    r36sx_physical_mouse_scan();
    for (int i = 0; i < R36SX_EVDEV_MAX_FDS; i++) {
        int fd = g_physical_mouse.fds[i];
        if (fd < 0) {
            continue;
        }
        for (;;) {
            struct input_event ev;
            ssize_t n = read(fd, &ev, sizeof(ev));
            if (n == (ssize_t)sizeof(ev)) {
                r36sx_physical_mouse_handle_event(i, &ev);
                continue;
            }
            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK ||
                    errno == EINTR) {
                    r36sx_physical_mouse_flush(i);
                    break;
                }
                if (errno == ENODEV || errno == ENXIO) {
                    r36sx_pico286_debug_log(
                        "mouse: removed physical input %s",
                        g_physical_mouse.paths[i]);
                    r36sx_physical_mouse_close_index(i);
                }
                break;
            }
            if (n == 0) {
                r36sx_pico286_debug_log(
                    "mouse: removed physical input %s",
                    g_physical_mouse.paths[i]);
                r36sx_physical_mouse_close_index(i);
            }
            break;
        }
    }
}
#else
static void r36sx_physical_keyboard_init(void)
{
}

static void r36sx_physical_keyboard_close_all(void)
{
}

static void r36sx_physical_keyboard_poll(void)
{
}

static void r36sx_physical_mouse_init(void)
{
}

static void r36sx_physical_mouse_close_all(void)
{
}

static void r36sx_physical_mouse_poll(void)
{
}
#endif

static int r36sx_mouse_prev_x;
static int r36sx_mouse_prev_y;
static int r36sx_mouse_prev_buttons;
static int r36sx_mouse_initialized;
static int r36sx_mouse_absolute_initialized;

extern "C" void HandleMouseReset(void)
{
    r36sx_mouse_prev_x = 0;
    r36sx_mouse_prev_y = 0;
    r36sx_mouse_prev_buttons = 0;
    r36sx_mouse_initialized = 0;
    r36sx_mouse_absolute_initialized = 0;
}

static int r36sx_mouse_clamp_delta(int value)
{
    if (value > 63) {
        return 63;
    }
    if (value < -63) {
        return -63;
    }
    return value;
}

extern "C" void HandleMouseRelative(int dx, int dy, int buttons)
{
    int sent = 0;

    buttons &= 3;
    if (!r36sx_mouse_initialized) {
        r36sx_mouse_initialized = 1;
    }

    while (dx != 0 || dy != 0) {
        const int step_x = r36sx_mouse_clamp_delta(dx);
        const int step_y = r36sx_mouse_clamp_delta(dy);
        sermouseevent((uint8_t)buttons, (int8_t)step_x, (int8_t)step_y);
        dx -= step_x;
        dy -= step_y;
        sent = 1;
    }

    if (!sent && buttons != r36sx_mouse_prev_buttons) {
        sermouseevent((uint8_t)buttons, 0, 0);
    }
    r36sx_mouse_prev_buttons = buttons;
}

extern "C" void HandleMouse(int x, int y, int buttons)
{
    int dx;
    int dy;

    buttons &= 3;
    if (!r36sx_mouse_absolute_initialized) {
        r36sx_mouse_prev_x = x;
        r36sx_mouse_prev_y = y;
        r36sx_mouse_absolute_initialized = 1;
        HandleMouseRelative(0, 0, buttons);
        return;
    }

    dx = x - r36sx_mouse_prev_x;
    dy = y - r36sx_mouse_prev_y;
    r36sx_mouse_prev_x = x;
    r36sx_mouse_prev_y = y;

    HandleMouseRelative(dx, dy, buttons);
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
static volatile int vm_paused_by_menu = 0;

extern "C" int r36sx_emergency_dump_pending(void);
extern "C" int r36sx_memory_dump_pending(void);
extern "C" void r36sx_emergency_dump_write_and_clear(void);

static void r36sx_pico286_set_menu_pause(int paused)
{
    int new_paused = paused != 0;
    int old_paused = vm_paused_by_menu != 0;

    if (old_paused == new_paused) {
        return;
    }
    vm_paused_by_menu = new_paused;
    __sync_synchronize();
    r36sx_pico286_debug_log("main: VM %s by host menu",
                            new_paused ? "paused" : "resumed");
}

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

static void r36sx_pico286_audio_queue_current_locked(uint32_t frames)
{
    const uint32_t buffered_frames =
        (uint32_t)(sample_index / (int)R36SX_AUDIO_CHANNELS);
    uint32_t old_write_buffer = audio_write_buffer;
    uint32_t leftover_frames;

    if (frames == 0) {
        return;
    }
    if (frames > buffered_frames) {
        frames = buffered_frames;
    }
    if (frames > g_audio_buffer_capacity_frames) {
        frames = g_audio_buffer_capacity_frames;
    }
    if (frames == 0) {
        return;
    }
    leftover_frames = buffered_frames - frames;

    if (audio_ready_count >= R36SX_AUDIO_BUFFER_COUNT - 1u) {
        audio_ready_frames[audio_ready_head] = 0;
        audio_ready_head =
            (audio_ready_head + 1u) % R36SX_AUDIO_BUFFER_COUNT;
        audio_ready_count--;
    }

    audio_ready_frames[old_write_buffer] = frames;
    if (audio_ready_count == 0) {
        audio_ready_head = old_write_buffer;
    }
    audio_ready_count++;
    audio_write_buffer =
        (audio_write_buffer + 1u) % R36SX_AUDIO_BUFFER_COUNT;
    sample_index = 0;
    if (leftover_frames > 0) {
        r36sx_mips_dsp_copy_u16(
            (uint16_t *)audio_buffers[audio_write_buffer],
            (const uint16_t *)&audio_buffers[old_write_buffer]
                                            [frames * R36SX_AUDIO_CHANNELS],
            (size_t)leftover_frames * R36SX_AUDIO_CHANNELS);
        sample_index = (int)(leftover_frames * R36SX_AUDIO_CHANNELS);
    }
    update_ready = 1;
    pthread_cond_signal(&update_cond);
}

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
    r36sx_pico286_reset_pic();
    r36sx_pico286_post_reset();
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
    r36sx_pico286_video_mark_dirty();

    pthread_mutex_lock(&update_mutex);
    memset(audio_buffers, 0, sizeof(audio_buffers));
    memset(audio_playback_buffer, 0, sizeof(audio_playback_buffer));
    sample_index = 0;
    audio_write_buffer = 0;
    audio_ready_head = 0;
    audio_ready_count = 0;
    memset(audio_ready_frames, 0, sizeof(audio_ready_frames));
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
        uint32_t playback_frames = 0;
        pthread_mutex_lock(&update_mutex);
        while (audio_ready_count == 0 && running) {
            pthread_cond_wait(&update_cond, &update_mutex);
        }
        if (!running) {
            pthread_mutex_unlock(&update_mutex);
            break;
        }
        if (audio_ready_count == 0) {
            update_ready = 0;
            pthread_mutex_unlock(&update_mutex);
            continue;
        }
        playback_frames = audio_ready_frames[audio_ready_head];
        if (playback_frames > g_audio_buffer_capacity_frames) {
            playback_frames = g_audio_buffer_capacity_frames;
        }
        r36sx_mips_dsp_copy_u16((uint16_t *)audio_playback_buffer,
                                (const uint16_t *)
                                    audio_buffers[audio_ready_head],
                                (size_t)playback_frames *
                                    R36SX_AUDIO_CHANNELS);
        audio_ready_frames[audio_ready_head] = 0;
        audio_ready_head =
            (audio_ready_head + 1u) % R36SX_AUDIO_BUFFER_COUNT;
        audio_ready_count--;
        update_ready = audio_ready_count > 0 ? 1 : 0;
        pthread_mutex_unlock(&update_mutex);

        // Send audio buffer to Linux audio system
        if (++sound_loop_count <= 4u) {
            r36sx_pico286_debug_log("sound_thread: write #%u frames=%u",
                                    sound_loop_count, playback_frames);
        }
        R36SX_PROFILE_BEGIN(profile_audio_write);
        int audio_write_rc = linux_audio_write(audio_playback_buffer,
                                               playback_frames);
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
    uint64_t last_audio_packet_tick = 0;

    int16_t last_dss_sample = 0;
    int16_t last_sb_sample = 0;

    const uint64_t hostfreq = 1000000000; // nanoseconds
    const uint64_t dss_period = hostfreq / 7000;
    const uint32_t audio_sample_rate = r36sx_sound_frequency ?
        r36sx_sound_frequency : R36SX_AUDIO_DRIVER_RATE;
    const uint64_t sound_period = hostfreq / audio_sample_rate;
    const uint64_t blink_period = 333333333;
    const uint64_t frame_period = (uint64_t)g_main_loop_frame_us * 1000ull;
    const uint64_t audio_packet_period =
        g_audio_packet_target_frames > 0 ?
            ((uint64_t)g_audio_packet_target_frames * hostfreq) /
                (uint64_t)audio_sample_rate :
            frame_period;
    const unsigned int max_system_catchup = 8;
    const unsigned int max_dss_catchup = 700;
    const unsigned int max_sb_catchup = 2205;
    const unsigned int max_audio_catchup =
        audio_sample_rate / R36SX_AUDIO_BUFFER_MIN_FPS;
    const int dss_audio_enabled = r36sx_pico286_audio_disney_enabled();
    const int sb_audio_enabled = r36sx_pico286_audio_sound_blaster_enabled();
    r36sx_pico286_debug_log("ticks_thread: audio dss=%d sound_blaster=%d",
                            dss_audio_enabled, sb_audio_enabled);

    unsigned int ticks_loop_count = 0;
    while (running) {
        if (soft_reset_in_progress || vm_paused_by_menu) {
            clock_gettime(CLOCK_MONOTONIC, &current);
            if (vm_paused_by_menu) {
                uint64_t elapsedTime =
                    (current.tv_sec - start.tv_sec) * hostfreq +
                    (uint64_t)(current.tv_nsec - start.tv_nsec);
                elapsed_system_timer = elapsedTime;
                elapsed_blink_tics = elapsedTime;
                elapsed_frame_tics = elapsedTime;
                last_dss_tick = elapsedTime;
                last_sb_tick = elapsedTime;
                last_sound_tick = elapsedTime;
                last_audio_packet_tick = elapsedTime;
            }
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
            const int16_t sample = dss_sample();
            last_dss_sample = dss_audio_enabled ? sample : 0;
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
        const uint64_t sb_rate = sb_samplerate ? sb_samplerate : 22050ull;
        uint64_t sb_period = hostfreq / sb_rate;
        if (sb_period == 0) {
            sb_period = 1;
        }
        R36SX_PROFILE_BEGIN(profile_sb_sample);
        for (;
             elapsedTime - last_sb_tick >= sb_period;
             sb_catchup_count++) {
            const int16_t sample = blaster_sample();
            last_sb_sample = sb_audio_enabled ? sample : 0;
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
            get_sound_sample(last_dss_sample + last_sb_sample,
                             &audio_buffers[audio_write_buffer][sample_index]);
            sample_index += 2;

            if (sample_index >=
                (int)(g_audio_buffer_capacity_frames *
                      R36SX_AUDIO_CHANNELS)) {
                const uint32_t buffered_frames =
                    (uint32_t)(sample_index / (int)R36SX_AUDIO_CHANNELS);
                pthread_mutex_lock(&update_mutex);
                r36sx_pico286_audio_queue_current_locked(buffered_frames);
                pthread_mutex_unlock(&update_mutex);
                last_audio_packet_tick = elapsedTime;
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
        if (sample_index > 0 &&
            elapsedTime - last_audio_packet_tick >= audio_packet_period) {
            uint32_t buffered_frames =
                (uint32_t)(sample_index / (int)R36SX_AUDIO_CHANNELS);
            uint32_t elapsed_frames =
                r36sx_pico286_audio_frames_for_elapsed(
                    elapsedTime - last_audio_packet_tick,
                    audio_sample_rate,
                    hostfreq);

            if (elapsed_frames > g_audio_buffer_capacity_frames) {
                elapsed_frames = g_audio_buffer_capacity_frames;
            }
            if (buffered_frames > elapsed_frames && elapsed_frames > 0) {
                buffered_frames = elapsed_frames;
            }
            if (buffered_frames > 0) {
                pthread_mutex_lock(&update_mutex);
                r36sx_pico286_audio_queue_current_locked(buffered_frames);
                pthread_mutex_unlock(&update_mutex);
                last_audio_packet_tick = elapsedTime;
            }
        }

        // Cursor blink
        if (elapsedTime - elapsed_blink_tics >= blink_period) {
            // ~3Hz
            cursor_blink_state ^= 1;
            r36sx_pico286_video_mark_dirty();
            elapsed_blink_tics = elapsedTime;
        }

        // Frame rendering follows the configured main-loop frame budget.
        if (elapsedTime - elapsed_frame_tics >= frame_period) {
            if (r36sx_pico286_video_take_dirty()) {
                R36SX_PROFILE_BEGIN(profile_renderer);
                renderer();
                R36SX_PROFILE_END(R36SX_PROFILE_RENDERER, profile_renderer);
            }
            elapsed_frame_tics = elapsedTime;
        }

        // Generate missed ticks in batches, then yield the CPU executor.
        usleep(R36SX_TICKS_THREAD_SLEEP_US);
    }
    r36sx_pico286_debug_log("ticks_thread: exit loops=%u", ticks_loop_count);
    return NULL;
}

int main() {
    r36sx_pico286_debug_reset();
    r36sx_pico286_debug_log_build_info();
    r36sx_pico286_debug_log("main: start");
    r36sx_profile_init();
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSEGV, fatal_signal_handler);
    signal(SIGBUS, fatal_signal_handler);
    signal(SIGILL, fatal_signal_handler);
    signal(SIGABRT, fatal_signal_handler);
    r36sx_physical_keyboard_init();
    r36sx_physical_mouse_init();

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
    const uint32_t target_fps =
        r36sx_pico286_target_fps(R36SX_MAIN_LOOP_DEFAULT_FPS);
    const uint32_t main_loop_frame_us =
        r36sx_pico286_frame_us_from_fps(target_fps);
    g_main_loop_frame_us = main_loop_frame_us;

    r36sx_sound_frequency =
        r36sx_pico286_audio_sample_rate(R36SX_AUDIO_DRIVER_RATE);
    if (r36sx_sound_frequency != 22050u &&
        r36sx_sound_frequency != R36SX_AUDIO_DRIVER_RATE) {
        r36sx_sound_frequency = R36SX_AUDIO_DRIVER_RATE;
    }
    g_audio_packet_target_frames =
        target_fps ? (r36sx_sound_frequency + target_fps / 2u) / target_fps
                   : r36sx_sound_frequency / 60u;
    if (g_audio_packet_target_frames == 0) {
        g_audio_packet_target_frames = 1;
    }
    g_audio_buffer_capacity_frames = R36SX_AUDIO_BUFFER_MAX_FRAMES;
    if (g_audio_packet_target_frames > g_audio_buffer_capacity_frames) {
        g_audio_packet_target_frames = g_audio_buffer_capacity_frames;
    }
    r36sx_pico286_debug_log("main: audio sample_rate=%u target_fps=%u packet_frames=%u capacity_frames=%u",
                            r36sx_sound_frequency, target_fps,
                            g_audio_packet_target_frames,
                            g_audio_buffer_capacity_frames);
    emu8950_opl = OPL_new(3579552, r36sx_sound_frequency);
    r36sx_pico286_debug_log("main: OPL_new=%p rate=%u", emu8950_opl,
                            r36sx_sound_frequency);
    blaster_reset();
    r36sx_pico286_debug_log("main: blaster_reset done");
    sn76489_reset();
    r36sx_pico286_debug_log("main: sn76489_reset done");
    r36sx_pico286_reset_pic();
    r36sx_pico286_post_reset();
    reset86();
    r36sx_pico286_debug_log("main: reset86 done");

    // Initialize audio system
    if (linux_audio_init((int)r36sx_sound_frequency, 2,
                         (int)g_audio_buffer_capacity_frames) == 0) {
        if (linux_audio_start() == 0) {
            printf("Audio: %s backend started\n", linux_audio_get_backend_name());
        } else {
            printf("Audio: Failed to start, continuing without audio\n");
        }
    } else {
        printf("Audio: Failed to initialize, continuing without audio\n");
    }

    pthread_t sound_tid = 0, ticks_tid = 0;
    int sound_thread_rc = pthread_create(&sound_tid, NULL, sound_thread, NULL);
    int ticks_thread_rc = pthread_create(&ticks_tid, NULL, ticks_thread, NULL);
    int sound_thread_started = sound_thread_rc == 0;
    int ticks_thread_started = ticks_thread_rc == 0;
    r36sx_pico286_debug_log("main: pthread_create sound=%d ticks=%d",
                            sound_thread_rc, ticks_thread_rc);

    uint32_t cpu_exec_loops_per_ms =
        r36sx_pico286_cpu_exec_loops(32768u);
    uint32_t cpu_exec_loops_per_frame_max =
        r36sx_pico286_frame_exec_loops(cpu_exec_loops_per_ms,
                                       main_loop_frame_us);
    uint32_t cpu_exec_loops_per_frame = cpu_exec_loops_per_frame_max;
    r36sx_cpu_exec_fn cpu_exec = r36sx_cpu_select_exec();
    r36sx_pico286_debug_log(
        "main: cpu_model=%s cpu_exec=%s cpu_mode=%s x87=%s bios=%s cpu_exec_loops_per_ms=%u target_fps=%u frame_us=%u cpu_exec_loops_per_frame_max=%u",
                            r36sx_pico286_cpu_model_name(),
                            r36sx_cpu_selected_exec_name(),
                            r36sx_pico286_cpu_mode_name(),
                            r36sx_pico286_x87_enabled() ? "on" : "off",
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
        r36sx_app_stats_record_quantum();
        if (soft_reset_requested) {
            R36SX_PROFILE_BEGIN(profile_soft_reset);
            r36sx_pico286_soft_reset();
            cpu_exec = r36sx_cpu_select_exec();
            r36sx_pico286_debug_log("main: cpu_exec=%s after reset",
                                    r36sx_cpu_selected_exec_name());
            R36SX_PROFILE_END(R36SX_PROFILE_SOFT_RESET, profile_soft_reset);
        }
        {
            uint32_t updated_loops_per_ms =
                r36sx_pico286_cpu_exec_loops(32768u);
            uint32_t updated_frame_max =
                r36sx_pico286_frame_exec_loops(updated_loops_per_ms,
                                               main_loop_frame_us);

            if (updated_frame_max != cpu_exec_loops_per_frame_max) {
                int was_at_max =
                    cpu_exec_loops_per_frame >= cpu_exec_loops_per_frame_max;

                r36sx_pico286_debug_log(
                    "main: cpu timing changed loops_per_ms=%u->%u frame_max=%u->%u",
                    cpu_exec_loops_per_ms,
                    updated_loops_per_ms,
                    cpu_exec_loops_per_frame_max,
                    updated_frame_max);
                cpu_exec_loops_per_ms = updated_loops_per_ms;
                cpu_exec_loops_per_frame_max = updated_frame_max;
                if (was_at_max ||
                    cpu_exec_loops_per_frame > cpu_exec_loops_per_frame_max) {
                    cpu_exec_loops_per_frame = cpu_exec_loops_per_frame_max;
                }
            }
        }
        r36sx_pico286_set_menu_pause(mfb_vm_paused());
        if (vm_paused_by_menu) {
            if (main_loop_count <= 8u) {
                r36sx_pico286_debug_log("main: paused before mfb_update loop=%u",
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
            if (r36sx_emergency_dump_pending()) {
                r36sx_emergency_dump_write_and_clear();
                running = 0;
                break;
            }
            if (r36sx_memory_dump_pending()) {
                r36sx_emergency_dump_write_and_clear();
            }
            r36sx_pico286_set_menu_pause(mfb_vm_paused());
            r36sx_profile_maybe_log();
            r36sx_pico286_wait_for_next_main_frame(&next_main_loop_us,
                                                   main_loop_frame_us);
            continue;
        }
        R36SX_PROFILE_BEGIN(profile_keyboard_tick_1);
        r36sx_physical_keyboard_poll();
        r36sx_physical_mouse_poll();
        r36sx_keyboard_tick();
        R36SX_PROFILE_END(R36SX_PROFILE_KEYBOARD_TICK, profile_keyboard_tick_1);
        if (main_loop_count < 8u) {
            r36sx_pico286_debug_log("main: before exec loop=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        R36SX_PROFILE_BEGIN(profile_exec86);
        uint64_t exec_start_us = r36sx_pico286_now_us();
        cpu_exec(cpu_exec_loops_per_frame);
        exec_elapsed_us = r36sx_pico286_now_us() - exec_start_us;
        R36SX_PROFILE_END_UNITS(R36SX_PROFILE_EXEC86, profile_exec86,
                                cpu_exec_loops_per_frame);
        if (r36sx_emergency_dump_pending()) {
            r36sx_emergency_dump_write_and_clear();
            running = 0;
            break;
        }
        if (r36sx_memory_dump_pending()) {
            r36sx_emergency_dump_write_and_clear();
        }
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
        r36sx_pico286_set_menu_pause(mfb_vm_paused());
        if (vm_paused_by_menu) {
            if (r36sx_emergency_dump_pending()) {
                r36sx_emergency_dump_write_and_clear();
                running = 0;
                break;
            }
            if (r36sx_memory_dump_pending()) {
                r36sx_emergency_dump_write_and_clear();
            }
            r36sx_profile_maybe_log();
            r36sx_pico286_wait_for_next_main_frame(&next_main_loop_us,
                                                   main_loop_frame_us);
            continue;
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
    r36sx_physical_keyboard_close_all();
    r36sx_physical_mouse_close_all();

    mfb_close();
    r36sx_pico286_debug_log("main: exit 0");
    return 0;
}
