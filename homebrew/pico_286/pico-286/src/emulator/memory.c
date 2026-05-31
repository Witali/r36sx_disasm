#pragma GCC optimize("Ofast")

#include "includes/bios.h"
#include "emulator.h"
#include "r36sx_bios_rom.h"
#include "ems.c.inl"
#if PICO_ON_DEVICE
#include "psram_spi.h"
#include "swap.h"
#endif
uint32_t __attribute__((aligned (4)))  VIDEORAM[VIDEORAM_SIZE] = {0};
uint8_t PICO286_PSRAM_ATTR RAM[RAM_SIZE] = {0};
uint8_t PICO286_PSRAM_ATTR UMB[UMB_END - UMB_START] = {0};
uint8_t PICO286_PSRAM_ATTR HMA[HMA_END - HMA_START] = {0};
uint8_t __attribute__((aligned (4))) SRAM[SRAM_BLOCK_SIZE] = {0};

#define VIDEORAM_MASK 0xFFFF

static inline uint32_t videoram_index(const uint32_t address)
{
    return address & VIDEORAM_MASK;
}

static inline uint32_t a20_wrap_address(const uint32_t address)
{
    return address & (HMA_START - 1u);
}

static inline int memory_range_inside(const uint32_t address,
                                      const uint32_t start,
                                      const uint32_t end,
                                      const uint32_t bytes)
{
    return bytes != 0u && address >= start && address < end &&
           bytes <= end - address;
}

static inline int r36sx_external_bios_read16(uint32_t address,
                                             uint16_t *value)
{
    uint8_t lo;
    uint8_t hi;

    if (!r36sx_bios_rom_contains(address, 2u)) {
        return 0;
    }
    r36sx_bios_rom_read8(address, &lo);
    r36sx_bios_rom_read8(address + 1u, &hi);
    *value = (uint16_t)lo | ((uint16_t)hi << 8);
    return 1;
}

static inline int r36sx_external_bios_read32(uint32_t address,
                                             uint32_t *value)
{
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;

    if (!r36sx_bios_rom_contains(address, 4u)) {
        return 0;
    }
    r36sx_bios_rom_read8(address, &b0);
    r36sx_bios_rom_read8(address + 1u, &b1);
    r36sx_bios_rom_read8(address + 2u, &b2);
    r36sx_bios_rom_read8(address + 3u, &b3);
    *value = (uint32_t)b0 |
             ((uint32_t)b1 << 8) |
             ((uint32_t)b2 << 16) |
             ((uint32_t)b3 << 24);
    return 1;
}

static inline int videoram_uses_vga_path(void)
{
    return ega_vga_enabled && videomode >= 0x0D && videomode <= 0x13;
}

static inline void videoram_write8_raw(const uint32_t address, const uint8_t value)
{
    uint32_t *cell = &VIDEORAM[videoram_index(address)];
    if (*cell != value) {
        *cell = value;
        r36sx_pico286_video_mark_dirty();
    }
}

static inline uint8_t videoram_read8_raw(const uint32_t address)
{
    return (uint8_t)(VIDEORAM[videoram_index(address)] & 0xFFu);
}

static inline void videoram_write8(const uint32_t address, const uint8_t value)
{
    if (videoram_uses_vga_path()) {
        if (vga_memory_address_visible(address)) {
            vga_mem_write(vga_memory_address_offset(address), value);
        }
        return;
    }
    videoram_write8_raw(address, value);
}

static inline uint8_t videoram_read8(const uint32_t address)
{
    if (videoram_uses_vga_path()) {
        if (vga_memory_address_visible(address)) {
            return vga_mem_read(vga_memory_address_offset(address));
        }
        return 0xFFu;
    }
    return videoram_read8_raw(address);
}

static inline void videoram_write16(const uint32_t address, const uint16_t value)
{
    if (videoram_uses_vga_path()) {
        videoram_write8(address, (uint8_t)value);
        videoram_write8(address + 1u, (uint8_t)(value >> 8));
        return;
    }
    videoram_write8_raw(address, (uint8_t)value);
    videoram_write8_raw(address + 1u, (uint8_t)(value >> 8));
}

static inline uint16_t videoram_read16(const uint32_t address)
{
    if (videoram_uses_vga_path()) {
        return (uint16_t)videoram_read8(address) |
               ((uint16_t)videoram_read8(address + 1u) << 8);
    }
    return (uint16_t)videoram_read8_raw(address) |
           ((uint16_t)videoram_read8_raw(address + 1u) << 8);
}

static inline void videoram_write32(const uint32_t address, const uint32_t value)
{
    if (videoram_uses_vga_path()) {
        vga_mem_write(address, (uint8_t)value);
        vga_mem_write(address + 1u, (uint8_t)(value >> 8));
        vga_mem_write(address + 2u, (uint8_t)(value >> 16));
        vga_mem_write(address + 3u, (uint8_t)(value >> 24));
        return;
    }
    videoram_write8_raw(address, (uint8_t)value);
    videoram_write8_raw(address + 1u, (uint8_t)(value >> 8));
    videoram_write8_raw(address + 2u, (uint8_t)(value >> 16));
    videoram_write8_raw(address + 3u, (uint8_t)(value >> 24));
}

static inline uint32_t videoram_read32(const uint32_t address)
{
    if (videoram_uses_vga_path()) {
        return (uint32_t)vga_mem_read(address) |
               ((uint32_t)vga_mem_read(address + 1u) << 8) |
               ((uint32_t)vga_mem_read(address + 2u) << 16) |
               ((uint32_t)vga_mem_read(address + 3u) << 24);
    }
    return (uint32_t)videoram_read8_raw(address) |
           ((uint32_t)videoram_read8_raw(address + 1u) << 8) |
           ((uint32_t)videoram_read8_raw(address + 2u) << 16) |
           ((uint32_t)videoram_read8_raw(address + 3u) << 24);
}
read86_t read86;
read86w_t readw86;
read86dw_t readdw86;
write86_t write86;
write86w_t writew86;
write86dw_t writedw86;

// Writes a byte to the virtual memory
void write86_ob(const uint32_t address, const uint8_t value) {
    if (address < RAM_SIZE) {
        RAM[address] = value;
    } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        // printf("video write %x traddr %x =  %x\n", address, (address - VIDEORAM_START) & VIDEORAM_MASK, value);
        videoram_write8(address, value);
        // VIDEORAM[(address - VIDEORAM_START) & VIDEORAM_MASK] = value;
    } else if (address >= EMS_START && address < EMS_END) {
        ems_write(address - EMS_START, value);
    } else if (r36sx_bios_rom_contains(address, 1u)) {
        return;
    } else if (address >= UMB_START && address < UMB_END) {
        UMB[address - UMB_START] = value;
    } else if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            HMA[address - HMA_START] = value;
        } else {
            RAM[address - HMA_START] = value;
        }
    } else if (!a20_enabled && address >= HMA_END) {
        write86_ob(a20_wrap_address(address), value);
    }
}

// Writes a word to the virtual memory
void writew86_ob(const uint32_t address, const uint16_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
    } else {
        if (memory_range_inside(address, 0u, RAM_SIZE, 2u)) {
            *(uint16_t *) &RAM[address] = value;
        } else if (memory_range_inside(address, VIDEORAM_START, VIDEORAM_END, 2u)) {
            videoram_write16(address, value);
        } else if (memory_range_inside(address, EMS_START, EMS_END, 2u)) {
            ems_writew(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 2u)) {
            return;
        } else if (memory_range_inside(address, UMB_START, UMB_END, 2u)) {
            *(uint16_t *) &UMB[address - UMB_START] = value;
        } else if (memory_range_inside(address, HMA_START, HMA_END, 2u)) {
            if (a20_enabled) {
                *(uint16_t *) &HMA[address - HMA_START] = value;
            } else {
                *(uint16_t *) &RAM[address - HMA_START] = value;
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writew86_ob(a20_wrap_address(address), value);
        } else {
            write86(address, (uint8_t) (value & 0xFF));
            write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
        }
    }
}

void writedw86_ob(const uint32_t address, const uint32_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
        write86(address + 2, (uint8_t) ((value >> 16) & 0xFF));
        write86(address + 3, (uint8_t) ((value >> 24) & 0xFF));
    } else {
        if (memory_range_inside(address, 0u, RAM_SIZE, 4u)) {
            *(uint32_t *) &RAM[address] = value;
        } else if (memory_range_inside(address, VIDEORAM_START, VIDEORAM_END, 4u)) {
            videoram_write32(address, value);
        } else if (memory_range_inside(address, EMS_START, EMS_END, 4u)) {
            ems_writedw(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 4u)) {
            return;
        } else if (memory_range_inside(address, UMB_START, UMB_END, 4u)) {
            *(uint32_t *) &UMB[address - UMB_START] = value;
        } else if (memory_range_inside(address, HMA_START, HMA_END, 4u)) {
            if (a20_enabled) {
                *(uint32_t *) &HMA[address - HMA_START] = value;
            } else {
                *(uint32_t *) &RAM[address - HMA_START] = value;
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writedw86_ob(a20_wrap_address(address), value);
        } else {
            write86(address, (uint8_t) (value & 0xFF));
            write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
            write86(address + 2, (uint8_t) ((value >> 16) & 0xFF));
            write86(address + 3, (uint8_t) ((value >> 24) & 0xFF));
        }
    }
}

// Reads a byte from the virtual memory
uint8_t read86_ob(const uint32_t address) {
    uint8_t bios_value;

    if (address < RAM_SIZE) {
        return RAM[address];
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read8(address);
    }
     // if (address >= VBIOS_START && address < VBIOS_END) {
         // return VGABIOS[address - VBIOS_START];
     // }
    if (address >= EMS_START && address < EMS_END) {
        return ems_read(address - EMS_START);
    }
    if (r36sx_bios_rom_read8(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return UMB[address - UMB_START];
    }
    if (address == 0xFC000) {
        return 0x21;
    }
    if (address >= BIOS_START && address < HMA_START) {
        return BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return HMA[address - HMA_START];
        }
        return RAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return read86_ob(a20_wrap_address(address));
    }
    return 0xFF;
}

// Reads a word from the virtual memory
uint16_t readw86_ob(const uint32_t address) {
    uint16_t bios_value;

    if (address & 1) {
        return (uint16_t) read86(address) | ((uint16_t) read86(address + 1) << 8);
    }
    if (memory_range_inside(address, 0u, RAM_SIZE, 2u)) {
        return *(uint16_t *) &RAM[address];
    }
    if (memory_range_inside(address, VIDEORAM_START, VIDEORAM_END, 2u)) {
        return videoram_read16(address);
    }
    // if (address >= VBIOS_START && address < VBIOS_END) {
        // return *(uint16_t *) &VGABIOS[address - VBIOS_START];
    // }
    if (memory_range_inside(address, EMS_START, EMS_END, 2u)) {
        return ems_readw(address - EMS_START);
    }
    if (r36sx_external_bios_read16(address, &bios_value)) {
        return bios_value;
    }
    if (memory_range_inside(address, UMB_START, UMB_END, 2u)) {
        return *(uint16_t *) &UMB[address - UMB_START];
    }
    if (memory_range_inside(address, BIOS_START, HMA_START, 2u)) {
        return *(uint16_t *) &BIOS[address - BIOS_START];
    }
    if (memory_range_inside(address, HMA_START, HMA_END, 2u)) {
        if (a20_enabled) {
            return *(uint16_t *) &HMA[address - HMA_START];
        }
        return *(uint16_t *) &RAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return readw86_ob(a20_wrap_address(address));
    }
    return (uint16_t) read86(address) | ((uint16_t) read86(address + 1) << 8);
}

uint32_t readdw86_ob(const uint32_t address) {
    uint32_t bios_value;

    if (address & 3) {
        return (uint32_t) read86(address)
               | ((uint32_t) read86(address + 1) << 8)
               | ((uint32_t) read86(address + 2) << 16)
               | ((uint32_t) read86(address + 3) << 24);
    }
    if (memory_range_inside(address, 0u, RAM_SIZE, 4u)) {
        return *(uint32_t *) &RAM[address];
    }
    if (memory_range_inside(address, VIDEORAM_START, VIDEORAM_END, 4u)) {
        return (uint32_t) read86(address)
               | ((uint32_t) read86(address + 1) << 8)
               | ((uint32_t) read86(address + 2) << 16)
               | ((uint32_t) read86(address + 3) << 24);
    }
    if (memory_range_inside(address, EMS_START, EMS_END, 4u)) {
        return ems_readdw(address - EMS_START);
    }
    if (r36sx_external_bios_read32(address, &bios_value)) {
        return bios_value;
    }
    if (memory_range_inside(address, UMB_START, UMB_END, 4u)) {
        return *(uint32_t *) &UMB[address - UMB_START];
    }
    if (memory_range_inside(address, BIOS_START, HMA_START, 4u)) {
        return *(uint32_t *) &BIOS[address - BIOS_START];
    }
    if (memory_range_inside(address, HMA_START, HMA_END, 4u)) {
        if (a20_enabled) {
            return *(uint32_t *) &HMA[address - HMA_START];
        }
        return *(uint32_t *) &RAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return readdw86_ob(a20_wrap_address(address));
    }
    return (uint32_t) read86(address)
           | ((uint32_t) read86(address + 1) << 8)
           | ((uint32_t) read86(address + 2) << 16)
           | ((uint32_t) read86(address + 3) << 24);
}

#if PICO_ON_DEVICE
// using UMB as low-RAM, and psram start space as UMB instead
#define LO_MEM (SRAM_BLOCK_SIZE)
#define RAM (DO_NOT_USE_IN_THIS_BLOCK)
#define HMA (DO_NOT_USE_IN_THIS_BLOCK)
#define UMB (DO_NOT_USE_IN_THIS_BLOCK)

// Writes a byte to the virtual memory
void write86_mp(const uint32_t address, const uint8_t value) {
    if (address < LO_MEM) {
        SRAM[address] = value;
    } else if (address < VIDEORAM_START) {
        write8psram(address, value);
    } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        // printf("video write %x traddr %x =  %x\n", address, (address - VIDEORAM_START) & VIDEORAM_MASK, value);
        videoram_write8(address, value);
        // VIDEORAM[(address - VIDEORAM_START) & VIDEORAM_MASK] = value;
    } else if (address >= EMS_START && address < EMS_END) {
        ems_write(address - EMS_START, value);
    } else if (r36sx_bios_rom_contains(address, 1u)) {
        return;
    } else if (address >= UMB_START && address < UMB_END) {
        write8psram(address - UMB_START, value);
    } else if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            write8psram(address, value);
        } else {
            SRAM[address - HMA_START] = value;
        }
    } else if (!a20_enabled && address >= HMA_END) {
        write86(address - HMA_START, value);
    }
}

// Writes a word to the virtual memory
void writew86_mp(const uint32_t address, const uint16_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
    } else {
        if (address < LO_MEM) {
            *(uint16_t *) &SRAM[address] = value;
        } else if (address < VIDEORAM_START) {
            write16psram(address, value);
        } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
            videoram_write16(address, value);
        } else if (address >= EMS_START && address < EMS_END) {
            ems_writew(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 2u)) {
            return;
        } else if (address >= UMB_START && address < UMB_END) {
            write16psram(address - UMB_START, value);
        } else if (address >= HMA_START && address < HMA_END) {
            if (a20_enabled) {
                write16psram(address, value);
            } else {
                *(uint16_t *) &SRAM[address - HMA_START] = value;
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writew86(address - HMA_START, value);
        }
    }
}

void writedw86_mp(const uint32_t address, const uint32_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
        write86(address + 2, (uint8_t) ((value >> 16) & 0xFF));
        write86(address + 3, (uint8_t) ((value >> 24) & 0xFF));
    } else {
        if (address < LO_MEM) {
            *(uint32_t *) &SRAM[address] = value;
        } else if (address < VIDEORAM_START) {
            write32psram(address, value);
        } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
            videoram_write32(address, value);
        } else if (address >= EMS_START && address < EMS_END) {
            ems_writedw(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 4u)) {
            return;
        } else if (address >= UMB_START && address < UMB_END) {
            write32psram(address - UMB_START, value);
        } else if (address >= HMA_START && address < HMA_END) {
            if (a20_enabled) {
                write32psram(address, value);
            } else {
                *(uint32_t *) &SRAM[address - HMA_START] = value;
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writedw86(address - HMA_START, value);
        }
    }
}

// Reads a byte from the virtual memory
uint8_t read86_mp(const uint32_t address) {
    uint8_t bios_value;

    if (address < LO_MEM) {
        return SRAM[address];
    }
    if (address < VIDEORAM_START) {
        return read8psram(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read8(address);
    }
     // if (address >= VBIOS_START && address < VBIOS_END) {
         // return VGABIOS[address - VBIOS_START];
     // }
    if (address >= EMS_START && address < EMS_END) {
        return ems_read(address - EMS_START);
    }
    if (r36sx_bios_rom_read8(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return read8psram(address - UMB_START);
    }
    if (address == 0xFC000) {
        return 0x21;
    }
    if (address >= BIOS_START && address < HMA_START) {
        return BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return read8psram(address);
        }
        return SRAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return read86_mp(address - HMA_START);
    }
    return 0xFF;
}

// Reads a word from the virtual memory
uint16_t readw86_mp(const uint32_t address) {
    uint16_t bios_value;

    if (address & 1) {
        return (uint16_t) read86_mp(address) | ((uint16_t) read86_mp(address + 1) << 8);
    }
    if (address < LO_MEM) {
        return *(uint16_t *) &SRAM[address];
    }
    if (address < VIDEORAM_START) {
        return read16psram(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read16(address);
    }
    // if (address >= VBIOS_START && address < VBIOS_END) {
        // return *(uint16_t *) &VGABIOS[address - VBIOS_START];
    // }
    if (address >= EMS_START && address < EMS_END) {
        return ems_readw(address - EMS_START);
    }
    if (r36sx_external_bios_read16(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return read16psram(address - UMB_START);
    }
    if (address >= BIOS_START && address < HMA_START) {
        return *(uint16_t *) &BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return read16psram(address);
        }
        return *(uint16_t *) &SRAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return readw86_mp(address - HMA_START);
    }
    return 0xFFFF;
}

uint32_t readdw86_mp(const uint32_t address) {
    uint32_t bios_value;

    if (address & 3) {
        return (uint32_t) read86_mp(address)
               | ((uint32_t) read86_mp(address + 1) << 8)
               | ((uint32_t) read86_mp(address + 2) << 16)
               | ((uint32_t) read86_mp(address + 3) << 24);
    }
    if (address < LO_MEM) {
        return *(uint32_t *) &SRAM[address];
    }
    if (address < VIDEORAM_START) {
        return read32psram(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read32(address);
    }
    if (address >= EMS_START && address < EMS_END) {
        return ems_readdw(address - EMS_START);
    }
    if (r36sx_external_bios_read32(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return read32psram(address - UMB_START);
    }
    if (address >= BIOS_START && address < HMA_START) {
        return *(uint32_t *) &BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return read32psram(address);
        }
        return *(uint32_t *) &SRAM[address - HMA_START];
    }
    if (!a20_enabled && address >= HMA_END) {
        return readdw86_mp(address - HMA_START);
    }
    return 0xFFFFFFFF;
}



// Writes a byte to the virtual memory
void write86_sw(const uint32_t address, const uint8_t value) {
    if (address < VIDEORAM_START) {
        swap_write(address, value);
    } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        // printf("video write %x traddr %x =  %x\n", address, (address - VIDEORAM_START) & VIDEORAM_MASK, value);
        videoram_write8(address, value);
        // VIDEORAM[(address - VIDEORAM_START) & VIDEORAM_MASK] = value;
    } else if (address >= EMS_START && address < EMS_END) {
        ems_write(address - EMS_START, value);
    } else if (r36sx_bios_rom_contains(address, 1u)) {
        return;
    } else if (address >= UMB_START && address < UMB_END) {
        swap_write(address, value);
    } else if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            swap_write(address, value);
        } else {
            swap_write(address - HMA_START, value);
        }
    } else if (!a20_enabled && address >= HMA_END) {
        write86(address - HMA_START, value);
    }
}

// Writes a word to the virtual memory
void writew86_sw(const uint32_t address, const uint16_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
    } else {
        if (address < VIDEORAM_START) {
            swap_write16(address, value);
        } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
            videoram_write16(address, value);
        } else if (address >= EMS_START && address < EMS_END) {
            ems_writew(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 2u)) {
            return;
        } else if (address >= UMB_START && address < UMB_END) {
            swap_write16(address, value);
        } else if (address >= HMA_START && address < HMA_END) {
            if (a20_enabled) {
                swap_write16(address, value);
            } else {
                swap_write16(address - HMA_START, value);
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writew86(address - HMA_START, value);
        }
    }
}

void writedw86_sw(const uint32_t address, const uint32_t value) {
    if (address & 1) {
        write86(address, (uint8_t) (value & 0xFF));
        write86(address + 1, (uint8_t) ((value >> 8) & 0xFF));
        write86(address + 2, (uint8_t) ((value >> 16) & 0xFF));
        write86(address + 3, (uint8_t) ((value >> 24) & 0xFF));
    } else {
        if (address < VIDEORAM_START) {
            swap_write32(address, value);
        } else if (address >= VIDEORAM_START && address < VIDEORAM_END) {
            videoram_write32(address, value);
        } else if (address >= EMS_START && address < EMS_END) {
            ems_writedw(address - EMS_START, value);
        } else if (r36sx_bios_rom_contains(address, 4u)) {
            return;
        } else if (address >= UMB_START && address < UMB_END) {
            swap_write32(address, value);
        } else if (address >= HMA_START && address < HMA_END) {
            if (a20_enabled) {
                swap_write32(address, value);
            } else {
                swap_write32(address - HMA_START, value);
            }
        } else if (!a20_enabled && address >= HMA_END) {
            writedw86(address - HMA_START, value);
        }
    }
}

// Reads a byte from the virtual memory
uint8_t read86_sw(const uint32_t address) {
    uint8_t bios_value;

    if (address < VIDEORAM_START) {
        return swap_read(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read8(address);
    }
     // if (address >= VBIOS_START && address < VBIOS_END) {
         // return VGABIOS[address - VBIOS_START];
     // }
    if (address >= EMS_START && address < EMS_END) {
        return ems_read(address - EMS_START);
    }
    if (r36sx_bios_rom_read8(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return swap_read(address);
    }
    if (address == 0xFC000) {
        return 0x21;
    }
    if (address >= BIOS_START && address < HMA_START) {
        return BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return swap_read(address);
        }
        return swap_read(address - HMA_START);
    }
    if (!a20_enabled && address >= HMA_END) {
        return read86_sw(address - HMA_START);
    }
    return 0xFF;
}

// Reads a word from the virtual memory
uint16_t readw86_sw(const uint32_t address) {
    uint16_t bios_value;

    if (address & 1) {
        return (uint16_t) read86_sw(address) | ((uint16_t) read86_sw(address + 1) << 8);
    }
    if (address < VIDEORAM_START) {
        return swap_read16(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read16(address);
    }
    // if (address >= VBIOS_START && address < VBIOS_END) {
        // return *(uint16_t *) &VGABIOS[address - VBIOS_START];
    // }
    if (address >= EMS_START && address < EMS_END) {
        return ems_readw(address - EMS_START);
    }
    if (r36sx_external_bios_read16(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return swap_read16(address);
    }
    if (address >= BIOS_START && address < HMA_START) {
        return *(uint16_t *) &BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return swap_read16(address);
        }
        return swap_read16(address - HMA_START);
    }
    if (!a20_enabled && address >= HMA_END) {
        return readw86_sw(address - HMA_START);
    }
    return 0xFFFF;
}

uint32_t readdw86_sw(const uint32_t address) {
    uint32_t bios_value;

    if (address & 3) {
        return (uint32_t) read86_sw(address)
               | ((uint32_t) read86_sw(address + 1) << 8)
               | ((uint32_t) read86_sw(address + 2) << 16)
               | ((uint32_t) read86_sw(address + 3) << 24);
    }
    if (address < VIDEORAM_START) {
        return swap_read32(address);
    }
    if (address >= VIDEORAM_START && address < VIDEORAM_END) {
        return videoram_read32(address);
    }
    if (address >= EMS_START && address < EMS_END) {
        return ems_readdw(address - EMS_START);
    }
    if (r36sx_external_bios_read32(address, &bios_value)) {
        return bios_value;
    }
    if (address >= UMB_START && address < UMB_END) {
        return swap_read32(address);
    }
    if (address >= BIOS_START && address < HMA_START) {
        return *(uint32_t *) &BIOS[address - BIOS_START];
    }
    if (address >= HMA_START && address < HMA_END) {
        if (a20_enabled) {
            return swap_read32(address);
        }
        return swap_read32(address - HMA_START);
    }
    if (!a20_enabled && address >= HMA_END) {
        return readdw86_sw(address - HMA_START);
    }
    return 0xFFFFFFFF;
}
#endif
