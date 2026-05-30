#if PICO_ON_DEVICE
#include "pico/time.h"
#include "graphics.h"
#elif defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif
#include "emulator/emulator.h"

uint8_t cga_intensity = 0, cga_colorset = 0, cga_foreground_color = 15, cga_blinking = 0xFF, cga_blinking_lock = 0, cga_hires = 0;
static uint8_t hercules_mode = 0, hercules_enable = 0;
volatile uint8_t port3DA = 8;

const uint32_t cga_palette[16] = {
        //R, G, B
        0x000000, // 0 black
        0x0000C4, // 1 blue
        0x00C400, // 2 green
        0x00C4C4, // 3 cyan
        0xC40000, // 4 red
        0xC400C4, // 5 magenta
        0xC47E00, // 7 brown
        0xC4C4C4, // 8 light gray
        0x4E4E4E, // 9 dark gray
        0x4E4EDC, // 10 light blue
        0x4EDC4E, // 11 light green
        0x4EF3F3, // 12 light cyan
        0xDC4E4E, // 13 light red
        0xF34EF3, // 14 light magenta
        0xF3F34E, // 15 yellow
        0xFFFFFF //  16 white
};


const uint8_t cga_gfxpal[3][2][4] = {
        //palettes for 320x200 graphics mode
        {
                { 0, 2, 4, 6 }, //normal palettes
                { 0, 10, 12, 14 }, //intense palettes
        },
        {
                { 0, 3, 5, 7 },
                { 0, 11, 13, 15 },
        },
        {
                // the unofficial Mode 5 palette, accessed by disabling ColorBurst
                { 0, 3, 4, 7 },
                { 0, 11, 12, 15 },
        },
};

uint32_t cga_composite_palette[3][16] = {
        //R, G, B
        // 640x200 Color Composite
        {
                0x000000, // black
                0x007100, // d.green
                0x003fff, // d.blue
                0x00abff, // m.blue
                0xc10065, // red
                0x737373, // gray
                0xe639ff, // purple
                0x8ca8ff, // l.blue
                0x554600, // brown
                0x00cd00, // l.green
                0x00cd00, // gray 2
                0x00fc7e, // aqua
                0xff3900, // orange
                0xe4cc00, // yellow
                0xff7af2, // pink
                0xffffff //white
        },
        {
                // 320x200 Palette 0 High Intensity Composite Color
                0x000000, //
                0x00766d, //
                0x00316f, //
                0x7b3400, //
                0x39be42, //
                0x837649, //
                0x539b0e, //
                0xeb3207, //
                0xd2c499, //
                0xf87a9b, //
                0xd9a06b, //
                0xb34400, //
                0x8bd04a, //
                0xbe8550, //
                0x98ad14, //
                0x000000 // ???
        },
        {
                // 320x200 Palette 1 High Intensity Composite Color
                0x000000, //
                0x008bac, //
                0x0049ae, //
                0x009ee8, //
                0x581c00, //
                0x00bc9b, //
                0x64759f, //
                0x00cdd9, //
                0xc81b26, //
                0xb2c2ec, //
                0xdd7def, //
                0xbed3ff, //
                0xff4900, //
                0xf6edc0, //
                0xffa4c3, //
                0xffffff //
        }
};
uint8_t color_burst = 0;

#define CGA_STATUS_FRAME_NS 16666667ULL
#define CGA_STATUS_SCANLINES 262ULL
#define CGA_STATUS_LINE_NS (CGA_STATUS_FRAME_NS / CGA_STATUS_SCANLINES)
#define CGA_STATUS_VISIBLE_LINES 200ULL
#define CGA_STATUS_HORIZONTAL_ACTIVE_NS 52600ULL
#define CGA_STATUS_VRETRACE_START_LINE 224ULL
#define CGA_STATUS_VRETRACE_LINES 16ULL

static uint64_t cga_status_time_ns(void) {
#if PICO_ON_DEVICE
    return time_us_64() * 1000ULL;
#elif defined(_WIN32)
    static LARGE_INTEGER freq;
    LARGE_INTEGER counter;
    if (!freq.QuadPart) {
        QueryPerformanceFrequency(&freq);
    }
    QueryPerformanceCounter(&counter);
    return (uint64_t)((counter.QuadPart * 1000000000ULL) / freq.QuadPart);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#endif
}

static uint8_t cga_status_from_raster(void) {
    uint64_t frame_phase = cga_status_time_ns() % CGA_STATUS_FRAME_NS;
    uint64_t scanline = frame_phase / CGA_STATUS_LINE_NS;
    uint64_t line_phase = frame_phase - scanline * CGA_STATUS_LINE_NS;
    uint8_t status = 0;

    if (scanline >= CGA_STATUS_VISIBLE_LINES ||
        line_phase >= CGA_STATUS_HORIZONTAL_ACTIVE_NS) {
        status |= 0x01;
    }

    if (scanline >= CGA_STATUS_VRETRACE_START_LINE &&
        scanline < CGA_STATUS_VRETRACE_START_LINE + CGA_STATUS_VRETRACE_LINES) {
        status |= 0x08;
    }

    return status;
}

void cga_portout(uint16_t portnum, uint16_t value) {
    // https://www.youtube.com/watch?v=ttPhnUUxy94
    // https://www.youtube.com/watch?v=44eNkE1YoiI
    // https://nerdlypleasures.blogspot.com/2016/05/ibms-cga-hardware-explained.html
    // http://www.techhelpmanual.com/901-color_graphics_adapter_i_o_ports.html
    // https://www.seasip.info/VintagePC/cga.html
    switch (portnum) {
        case 0x3B8: {
            hercules_mode = ((hercules_enable & 1) & (value & 2)) != 0;
            if (hercules_mode && videomode != 0x7) videomode = 0x1e;
            break;
        }
        case 0x3BF: {
            hercules_enable = value & 3;
            break;
        }
        case 0x3D8: // Mode control register:
//         printf("3d8 %x\n", value);
        // 0x13 -- 640 dot mode -- 0b10011
            //if (videomode >= 0xd) return;
            cga_hires = (value >> 4) & 1;
            color_burst = (value >> 2) & 1;

            // the unofficial Mode 5 palette, accessed by disabling ColorBurst
            if ((videomode == 4 || videomode == 5) && color_burst) {
//                printf("colorburst!!\n");
                cga_colorset = 2;
            }
            if (videomode == 3 && value == 0x1b) {
                videomode = 0x0a;

            }

            if ((value & 0x0f) == 0b0001) {
                //printf("160x100x16\n");
                videomode =     0x77;
            }


            if ((value & 0x1f) == 0b11010) {
                //printf("160x100x16");
                videomode =     0x76;
            }
            if (value == 0x08) {
//                printf("80x100x16");
//                videomode = 0x76;
            }

            // TODO: Включение/выключение глобального композитного режима по хоткеямы
            if (videomode == 6 && (value & 0x0f) == 0b1010) {
                //printf("160x200x16 %i", videomode);
                videomode = 0x76;
            }
            if (!cga_blinking_lock) {
                cga_blinking = (value >> 5) & 1 ? 0x7F : 0xFF;
                //printf("[CGA] value 0x%02x\r\n", value);
            }
#if PICO_ON_DEVICE
            for (uint8_t i = 0; i < 4; i++) {
                graphics_set_palette(i, cga_palette[cga_gfxpal[cga_colorset][cga_intensity][i]]);
            }
#endif
            break;
        case 0x3D9: // Colour control register
            cga_foreground_color = value & 0b1111;
            if (!color_burst)
                cga_colorset = value >> 5 & 1;
            cga_intensity = value >> 4 & 1;
//            printf("cga_foreground_color %x cga_colorset %x cga_intensity %x\n", cga_foreground_color, cga_colorset, cga_intensity);
#if PICO_ON_DEVICE
            graphics_set_palette(0, cga_palette[cga_foreground_color]);
            for (uint8_t i = 1; i < 4; i++) {
                graphics_set_palette(i, cga_palette[cga_gfxpal[cga_colorset][cga_intensity][i]]);
            }
#endif
            break;
    }
}

uint16_t cga_portin(uint16_t portnum) {
     if (hercules_mode) {
         return 0xFF;
     }

     port3DA = cga_status_from_raster();
     return port3DA;
}
