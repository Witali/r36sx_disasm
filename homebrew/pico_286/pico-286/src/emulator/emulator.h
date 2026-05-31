#pragma once
#if PICO_ON_DEVICE
#include "printf/printf.h"
#include <pico.h>
#else
#endif
#include <stdint.h>
#include "cpu.h"

#if PICO_ON_DEVICE
#define PICO286_PSRAM_ATTR __attribute__((aligned (4), section(".psram")))
#else
#define PICO286_PSRAM_ATTR __attribute__((aligned (4)))
#endif

#define SVGA_WIDTH 800
#define SVGA_HEIGHT 600
#define SVGA_MAX_BPP 16
#define SVGA_VRAM_SIZE (SVGA_WIDTH * SVGA_HEIGHT * (SVGA_MAX_BPP / 8))
#define VBE_MODE_800X600X8 0x0103
#define VBE_MODE_800X600X16 0x0114

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PICO_ON_DEVICE
extern uint32_t butter_psram_size;
extern bool PSRAM_AVAILABLE;
#include "psram_spi.h"
#define VIDEORAM_SIZE (64 << 10)
#if PICO_RP2350

#define RAM_SIZE (640 << 10)

#else
//#define RAM_SIZE (146 << 10)

#ifdef TOTAL_VIRTUAL_MEMORY_KBS
#define RAM_SIZE (72 << 10)
#else
#define RAM_SIZE (112 << 10)
#endif

#endif
#else
#include "printf/printf.h"
#define VIDEORAM_SIZE (64 << 10)
#define RAM_SIZE (640 << 10)
#define butter_psram_size 1
#define PSRAM_AVAILABLE 1
#endif
#if defined(R36SX_RUNTIME_SOUND_FREQUENCY) && R36SX_RUNTIME_SOUND_FREQUENCY
extern uint32_t r36sx_sound_frequency;
#define SOUND_FREQUENCY (r36sx_sound_frequency)
#elif defined(HARDWARE_SOUND)
#define SOUND_FREQUENCY (44100)
#else
    #if PICO_RP2040
#define SOUND_FREQUENCY (22050)
#else
#define SOUND_FREQUENCY (44100)
#endif
#endif

#if defined(R36SX_VIDEO_DIRTY_TRACKING) && R36SX_VIDEO_DIRTY_TRACKING
void r36sx_pico286_video_mark_dirty(void);
#else
static inline void r36sx_pico286_video_mark_dirty(void) {}
#endif
#define rgb(r, g, b) (((r)<<16) | ((g) << 8 ) | (b) )

#define VIDEORAM_START (0xA0000)
#define VIDEORAM_END (0xC0000)

#define VBIOS_START (0xC0000)
#define VBIOS_END (0xC8000)

#define EMS_START (0xC0000)
#define EMS_END   (0xD0000)

#define UMB_START (0xD0000)
#define UMB_END (0xFC000)

#define HMA_START (0x100000)
#define HMA_END (0x110000-16)

#define BIOS_START (0xFE000)

#define EMS_MEMORY_SIZE (2048 << 10) // 2 MB
#define XMS_MEMORY_KB 15568u // Keeps total usable RAM at 16 MB.
#define XMS_MEMORY_SIZE (XMS_MEMORY_KB << 10)
#define EXTENDED_MEMORY_START HMA_START
#define EXTENDED_MEMORY_END (EXTENDED_MEMORY_START + XMS_MEMORY_SIZE)

#define BIOS_MEMORY_SIZE                0x413
#define BIOS_TRUE_MEMORY_SIZE           0x415
#define BIOS_CRTCPU_PAGE        0x48A
extern uint8_t log_debug;


extern uint8_t RAM[RAM_SIZE];
extern uint32_t VIDEORAM[VIDEORAM_SIZE];
extern uint8_t SVGA_VRAM[SVGA_VRAM_SIZE];
extern uint8_t UMB[UMB_END - UMB_START];
extern uint8_t HMA[HMA_END - HMA_START];
extern uint8_t XMS[XMS_MEMORY_SIZE];
uint32_t xms_configured_memory_bytes(void);
// for non-butter-psram modes
#define SRAM_BLOCK_SIZE 0x2C000
extern uint8_t SRAM[SRAM_BLOCK_SIZE];
#define FIRST_RAM_PAGE (butter_psram_size ? RAM : SRAM)

extern uint32_t dwordregs[8];
#define byteregs ((uint8_t*)dwordregs)
#define wordregs ((uint16_t*)dwordregs)

typedef union {
    uint32_t value;
    struct {
        unsigned CF : 1;  // 0 bit of value
        unsigned _1 : 1;  // 1
        unsigned PF : 1;  // 2
        unsigned _3 : 1;  // 3
        unsigned AF : 1;  // 4
        unsigned _5 : 1;  // 5
        unsigned ZF : 1;  // 6
        unsigned SF : 1;  // 7
        unsigned TF : 1;  // 8
        unsigned IF : 1;  // 9
        unsigned DF : 1;  // 10
        unsigned OF : 1;  // 11
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned _16 : 1;
        unsigned _17 : 1;
        unsigned AC : 1; // 18 (Alignment Check)	Проверка выравнивания (включается в CPL=3 при CR0.AM=1)
                         // (Alignment Check Exception) — INT 17 (11h)
        unsigned VIF : 1; // 19 (Virtual Interrupt Flag)	Виртуальный IF для виртуализации (введён в 486, но зарезервирован с 386)
        unsigned VIP : 1; // 20 (Virtual Interrupt Pending)	Виртуальное прерывание ожидает (аналогично — введён в 486)
        unsigned ID : 1; // 21 (ID Flag)	Позволяет проверить поддержку CPUID инструкцией
    } bits;
} x86_flags_t;

extern x86_flags_t x86_flags;
extern uint32_t segregs32[6];

#ifndef R36SX_SEGMENT_BASE_CACHE
#define R36SX_SEGMENT_BASE_CACHE 0
#endif

#if R36SX_SEGMENT_BASE_CACHE
extern uint16_t segselector16[6];
extern uint32_t segbase32[6];

static inline uint32_t r36sx_cpu_segbase_cached(uint16_t selector)
{
    if (selector == segselector16[reges]) {
        return segbase32[reges];
    }
    if (selector == segselector16[regcs]) {
        return segbase32[regcs];
    }
    if (selector == segselector16[regss]) {
        return segbase32[regss];
    }
    if (selector == segselector16[regds]) {
        return segbase32[regds];
    }
    if (selector == segselector16[regfs]) {
        return segbase32[regfs];
    }
    if (selector == segselector16[reggs]) {
        return segbase32[reggs];
    }
    return r36sx_cpu_segbase(selector);
}

#define CPU_ES_BASE segbase32[reges]
#define CPU_CS_BASE segbase32[regcs]
#define CPU_SS_BASE segbase32[regss]
#define CPU_DS_BASE segbase32[regds]
#define CPU_FS_BASE segbase32[regfs]
#define CPU_GS_BASE segbase32[reggs]

#undef segbase
#define segbase(x) r36sx_cpu_segbase_cached((uint16_t)(x))
#endif

// i8259
extern struct i8259_s {
    uint8_t interrupt_mask_register; //mask register
    uint8_t interrupt_request_register; //request register
    uint8_t in_service_register; //service register
    uint8_t initialization_command_word_step; //used during initialization to keep track of which ICW we're at
    uint8_t initialization_command_words[5];
    uint8_t interrupt_vector_offset; //interrupt vector offset
    uint8_t priority_level; //which IRQ has highest priority
    uint8_t automatic_end_of_interrupt; //automatic EOI mode
    uint8_t register_read_mode; //remember what to return on read register from OCW3
    uint8_t controller_enabled;
} i8259_controller;

#define doirq(irqnum) (i8259_controller.interrupt_request_register |= (1 << (irqnum)) & (~i8259_controller.interrupt_mask_register))

static inline uint8_t nextintr() {
    uint8_t tmpirr = i8259_controller.interrupt_request_register & (~i8259_controller.interrupt_mask_register); //XOR request register with inverted mask register
    for (uint8_t i = 0; i < 8; i++)
        if ((tmpirr >> i) & 1) {
            i8259_controller.interrupt_request_register &= ~(1 << i);
            i8259_controller.in_service_register |= (1 << i);
            return (i8259_controller.initialization_command_words[2] + i);
        }
    return 0;
}

void out8259(uint16_t portnum, uint8_t value);

uint8_t in8259(uint16_t portnum);

// Video
extern int videomode;
#define CURSOR_X FIRST_RAM_PAGE[0x450]
#define CURSOR_Y FIRST_RAM_PAGE[0x451]
extern uint8_t cursor_start, cursor_end;
extern uint32_t vga_palette[256];
uint8_t vga_dac_6_to_8(uint8_t value);
uint8_t vga_dac_8_to_6(uint8_t value);
uint32_t vga_dac_color(uint8_t red6, uint8_t green6, uint8_t blue6);
void vga_set_dac_color(uint8_t index, uint8_t red6, uint8_t green6, uint8_t blue6);
void vga_get_dac_color(uint8_t index, uint8_t *red6, uint8_t *green6, uint8_t *blue6);

/*
 * Shadow Palette hooks for the Linux/R36SX renderer.
 * The RGB888 arrays above remain the emulated hardware state and are used for
 * DAC readback.  The renderer keeps RGB565 shadow copies in sync so drawing
 * does not reconvert palettes every frame.
 */
#if !PICO_ON_DEVICE
void r36sx_pico286_vga_palette565_set(uint8_t index, uint32_t color);
void r36sx_pico286_vga_palette565_set_all(const uint32_t *palette, uint16_t count);
#else
static inline void r36sx_pico286_vga_palette565_set(uint8_t index,
                                                    uint32_t color)
{
    (void)index;
    (void)color;
}

static inline void r36sx_pico286_vga_palette565_set_all(
    const uint32_t *palette, uint16_t count)
{
    (void)palette;
    (void)count;
}
#endif

// TGA
extern uint32_t tga_palette[16];
extern uint8_t tga_palette_map[16];
#if !PICO_ON_DEVICE
void r36sx_pico286_tga_palette565_set(uint8_t index, uint32_t color);
#else
static inline void r36sx_pico286_tga_palette565_set(uint8_t index,
                                                    uint32_t color)
{
    (void)index;
    (void)color;
}
#endif

extern void tga_portout(uint16_t portnum, uint16_t value);

extern void tga_draw_char(uint8_t ch, int x, int y, uint8_t color);

extern void tga_draw_pixel(int x, int y, uint8_t color);

// CGA
extern const uint32_t cga_palette[16];
extern const uint8_t cga_gfxpal[3][2][4];
extern uint32_t cga_composite_palette[3][16];
extern uint8_t cga_intensity, cga_colorset, cga_foreground_color, cga_blinking, cga_blinking_lock, cga_hires;

void cga_portout(uint16_t portnum, uint16_t value);

uint16_t cga_portin(uint16_t portnum);

// EGA/VGA
#define vga_plane_size (16000)
extern uint32_t vga_plane_offset;
extern uint8_t vga_planar_mode;
extern uint32_t vga_svga_bank;
extern uint16_t vga_svga_width;
extern uint16_t vga_svga_height;
extern uint8_t vga_svga_bpp;
int vga_svga_mode_active(void);
int vga_svga_mode_supported(uint16_t mode);
int vga_svga_set_mode(uint16_t mode, int clear_memory);
void vga_svga_disable(void);
void vga_svga_set_bank(uint16_t bank);
uint16_t vga_svga_get_bank(void);
void vga_set_standard_mode(uint8_t mode);

#if PICO_ON_DEVICE
    extern bool ega_vga_enabled;
#else
#define ega_vga_enabled (1)
#endif

void OpFpu(uint8_t opcode);

// Memory
typedef void (*write86_t)(uint32_t address, uint8_t value);
typedef void (*write86w_t)(uint32_t address, uint16_t value);
typedef void (*write86dw_t)(uint32_t address, uint32_t value);
typedef uint8_t (*read86_t)(uint32_t address);
typedef uint16_t (*read86w_t)(uint32_t address);
typedef uint32_t (*read86dw_t)(uint32_t address);
extern read86_t read86;
extern read86w_t readw86;
extern read86dw_t readdw86;
extern write86_t write86;
extern write86w_t writew86;
extern write86dw_t writedw86;
// on-board (butter) psram
void write86_ob(const uint32_t address, const uint8_t value);
void writew86_ob(uint32_t address, uint16_t value);
void writedw86_ob(uint32_t address, uint32_t value);
uint8_t read86_ob(uint32_t address);
uint16_t readw86_ob(uint32_t address);
uint32_t readdw86_ob(uint32_t address);
// murmulator-psram
void write86_mp(const uint32_t address, const uint8_t value);
void writew86_mp(uint32_t address, uint16_t value);
void writedw86_mp(uint32_t address, uint32_t value);
uint8_t read86_mp(uint32_t address);
uint16_t readw86_mp(uint32_t address);
uint32_t readdw86_mp(uint32_t address);
// swap
void write86_sw(const uint32_t address, const uint8_t value);
void writew86_sw(uint32_t address, uint16_t value);
void writedw86_sw(uint32_t address, uint32_t value);
uint8_t read86_sw(uint32_t address);
uint16_t readw86_sw(uint32_t address);
uint32_t readdw86_sw(uint32_t address);

#ifndef R36SX_NATIVE_FAST_MEMORY
#define R36SX_NATIVE_FAST_MEMORY 0
#endif

#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
static inline int r36sx_memory_fast_ram_range(uint32_t address,
                                              uint32_t bytes)
{
    return bytes != 0u && address < RAM_SIZE && bytes <= RAM_SIZE - address;
}

static inline uint8_t r36sx_read86_fast(uint32_t address)
{
    if (address < RAM_SIZE) {
        return RAM[address];
    }
    return read86_ob(address);
}

static inline uint16_t r36sx_readw86_fast(uint32_t address)
{
    if (address & 1u) {
        return (uint16_t)r36sx_read86_fast(address) |
               ((uint16_t)r36sx_read86_fast(address + 1u) << 8);
    }
    if (r36sx_memory_fast_ram_range(address, 2u)) {
        return *(uint16_t *)&RAM[address];
    }
    return readw86_ob(address);
}

static inline uint32_t r36sx_readdw86_fast(uint32_t address)
{
    if (address & 3u) {
        return (uint32_t)r36sx_read86_fast(address) |
               ((uint32_t)r36sx_read86_fast(address + 1u) << 8) |
               ((uint32_t)r36sx_read86_fast(address + 2u) << 16) |
               ((uint32_t)r36sx_read86_fast(address + 3u) << 24);
    }
    if (r36sx_memory_fast_ram_range(address, 4u)) {
        return *(uint32_t *)&RAM[address];
    }
    return readdw86_ob(address);
}

static inline void r36sx_write86_fast(uint32_t address, uint8_t value)
{
    if (address < RAM_SIZE) {
        RAM[address] = value;
        return;
    }
    write86_ob(address, value);
}

static inline void r36sx_writew86_fast(uint32_t address, uint16_t value)
{
    if (address & 1u) {
        r36sx_write86_fast(address, (uint8_t)value);
        r36sx_write86_fast(address + 1u, (uint8_t)(value >> 8));
        return;
    }
    if (r36sx_memory_fast_ram_range(address, 2u)) {
        *(uint16_t *)&RAM[address] = value;
        return;
    }
    writew86_ob(address, value);
}

static inline void r36sx_writedw86_fast(uint32_t address, uint32_t value)
{
    if (address & 3u) {
        r36sx_write86_fast(address, (uint8_t)value);
        r36sx_write86_fast(address + 1u, (uint8_t)(value >> 8));
        r36sx_write86_fast(address + 2u, (uint8_t)(value >> 16));
        r36sx_write86_fast(address + 3u, (uint8_t)(value >> 24));
        return;
    }
    if (r36sx_memory_fast_ram_range(address, 4u)) {
        *(uint32_t *)&RAM[address] = value;
        return;
    }
    writedw86_ob(address, value);
}

#undef getmem8
#undef getmem16
#undef getmem32
#undef putmem8
#undef putmem16
#undef putmem32
#define getmem8(x, y) r36sx_read86_fast(segbase(x) + (uint32_t)(y))
#define getmem16(x, y) r36sx_readw86_fast(segbase(x) + (uint32_t)(y))
#define getmem32(x, y) r36sx_readdw86_fast(segbase(x) + (uint32_t)(y))
#define putmem8(x, y, z) r36sx_write86_fast(segbase(x) + (uint32_t)(y), (z))
#define putmem16(x, y, z) r36sx_writew86_fast(segbase(x) + (uint32_t)(y), (z))
#define putmem32(x, y, z) r36sx_writedw86_fast(segbase(x) + (uint32_t)(y), (z))
#endif

// Ports
void vga_portout(uint16_t portnum, uint16_t value);
uint16_t vga_portin(uint16_t portnum);
void vga_attribute_reset_flipflop(void);
int vga_memory_address_visible(uint32_t address);
uint32_t vga_memory_address_offset(uint32_t address);
extern void portout(uint16_t portnum, uint16_t value);
extern void portout16(uint16_t portnum, uint16_t value);
extern uint16_t portin(uint16_t portnum);
extern uint16_t portin16(uint16_t portnum);

extern uint8_t port60, port61, port64;
extern volatile uint8_t port3DA;
extern uint8_t crt_controller_idx, crt_controller[32];
extern uint32_t vram_offset;
extern uint32_t tga_offset;

// CPU
extern void exec86(uint32_t execloops);
extern uint8_t r36sx_cpu_waiting_for_interrupt(void);

extern void reset86();
void r36sx_keyboard_reset(void);

// i8253
    extern struct i8253_s {
        uint16_t channel_reload_value[3];     // chandata -> channel reload values (what gets loaded into counters)
        uint8_t channel_access_mode[3];       // accessmode -> how each channel is accessed (lobyte/hibyte/toggle)
        uint8_t channel_byte_toggle[3];       // bytetoggle -> tracks which byte to read/write in toggle mode
        uint32_t channel_effective_count[3];  // effectivedata -> actual count value used by channel
        float channel_frequency[3];           // chanfreq -> calculated frequency for each channel
        uint8_t channel_active[3];            // active -> whether channel is actively counting
        uint16_t channel_current_count[3];    // counter -> current counter value for each channel
    } i8253_controller;

void out8253(uint16_t portnum, uint8_t value);

uint8_t in8253(uint16_t portnum);

extern int timer_period;
extern int speakerenabled;

// Mouse
extern void sermouseevent(uint8_t buttons, int8_t xrel, int8_t yrel);

extern uint8_t mouse_portin(uint16_t portnum);

extern void mouse_portout(uint16_t portnum, uint8_t value);

extern void tandy_write(uint16_t reg, uint8_t value);

extern void adlib_write_d(uint16_t reg, uint8_t value);

extern void cms_write(uint16_t reg, uint8_t value);

int16_t dss_sample();

extern void sn76489_reset();

// static int16_t sn76489_sample();

// extern void cms_samples(int16_t *output);

#define XMS_FN_CS 0x0000
#define XMS_FN_IP 0x03FF

extern uint8_t xms_handler();

//void i8237_writeport(uint16_t portnum, uint8_t value);
//void i8237_writepage(uint16_t portnum, uint8_t value);

//uint8_t i8237_readport( uint16_t portnum);
//uint8_t i8237_readpage( uint16_t portnum);
uint8_t i8237_read(uint8_t channel);

void i8237_write(uint8_t channel, uint8_t value);

void i8237_reset();

void blaster_reset();

// uint8_t blaster_read(uint16_t portnum);
// void blaster_write(uint16_t portnum, uint8_t value);
int16_t blaster_sample();

void outadlib(uint16_t portnum, uint8_t value);

uint8_t inadlib(uint16_t portnum);

int16_t adlibgensample();

extern void out_ems(uint16_t port, uint8_t data);

extern int16_t covox_sample;

#if !PICO_ON_DEVICE
#define __fast_mul(x,y) (x*y)
#define __not_in_flash(x)
#define __time_critical_func(x)

#endif

#ifndef INLINE
#if defined(_MSC_VER)
#define likely(x)       (x)
#define unlikely(x)     (x)
#define INLINE __inline
#define ALIGN(x, y) __declspec(align(x)) y
#elif defined(__GNUC__)
#define INLINE inline
///__not_in_flash("was_inline")
#if PICO_ON_DEVICE
#define ALIGN(x, y) y __attribute__((aligned(x)))
#else
#define ALIGN(x, y) y __attribute__((aligned(x)))
#endif
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define INLINE inline
#define ALING(x, y) y __attribute__((aligned(x)))
#endif
#endif
static INLINE int16_t speaker_sample() {
    if (!speakerenabled) return 0;
    static uint32_t speakerfullstep, speakerhalfstep, speakercurstep = 0;
    int16_t speakervalue;
    speakerfullstep = SOUND_FREQUENCY / i8253_controller.channel_frequency[2];
    if (speakerfullstep < 2)
        speakerfullstep = 2;
    speakerhalfstep = speakerfullstep >> 1;
    if (speakercurstep < speakerhalfstep) {
        speakervalue = 4096;
    } else {
        speakervalue = -4096;
    }
    speakercurstep = (speakercurstep + 1) % speakerfullstep;
    return speakervalue;
}

extern void get_sound_sample(int16_t other_sample, int16_t *samples);
#ifdef __cplusplus
}
#endif


#ifdef TOTAL_VIRTUAL_MEMORY_KBS
#include "swap.h"
static INLINE void write8psram(uint32_t address, uint8_t value) {
    swap_write(address, value);
}
static INLINE void write16psram(uint32_t address, uint16_t value) {
    swap_write16(address, value);
}
static INLINE void write32psram(uint32_t address, uint32_t value) {
    swap_write32(address, value);
}
static INLINE uint8_t read8psram(uint32_t address) {
    return swap_read(address);
}
static INLINE uint16_t read16psram(uint32_t address) {
    return swap_read16(address);
}
static INLINE uint32_t read32psram(uint32_t address) {
    return swap_read32(address);
}
#endif

void vga_init(void);
void vga_mem_write(uint32_t address, uint8_t cpu_data);
void vga_mem_write16(uint32_t address, uint16_t cpu_data_x2);
uint8_t vga_mem_read(uint32_t address);
uint16_t vga_mem_read16(uint32_t address);
