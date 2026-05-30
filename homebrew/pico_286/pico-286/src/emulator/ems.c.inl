// The Lo-tech EMS board driver is hardcoded to 2MB.
#pragma once
#if PICO_ON_DEVICE
#include "psram_spi.h"
#include "swap.h"
#endif
#define EMS_PSRAM_OFFSET (2048 << 10)
#define EMS_PAGE_SIZE 0x4000u
#define EMS_PAGE_MASK (EMS_PAGE_SIZE - 1u)
#define EMS_PAGE_COUNT (EMS_MEMORY_SIZE / EMS_PAGE_SIZE)

static uint8_t ems_pages[4] = {0};
uint8_t PICO286_PSRAM_ATTR EMS[EMS_MEMORY_SIZE] = {0};

inline void out_ems(const uint16_t port, const uint8_t data) {
    ems_pages[port & 3] = data;
}

static INLINE uint32_t physical_address(const uint32_t address) {
    const uint32_t page_addr = address & EMS_PAGE_MASK;
    const uint8_t selector = ems_pages[(address >> 14) & 3];
    return ((uint32_t)(selector % EMS_PAGE_COUNT) * EMS_PAGE_SIZE) + page_addr;
}

static INLINE uint8_t ems_read(const uint32_t address) {
    const uint32_t phys_addr = physical_address(address);
    return butter_psram_size ? EMS[phys_addr] :
     (PSRAM_AVAILABLE ? read8psram(phys_addr + EMS_PSRAM_OFFSET) : swap_read(phys_addr + EMS_PSRAM_OFFSET));
}

static INLINE uint16_t ems_readw(const uint32_t address) {
    return (uint16_t)ems_read(address) |
           ((uint16_t)ems_read(address + 1u) << 8);
}

static INLINE uint32_t ems_readdw(const uint32_t address) {
    return (uint32_t)ems_read(address) |
           ((uint32_t)ems_read(address + 1u) << 8) |
           ((uint32_t)ems_read(address + 2u) << 16) |
           ((uint32_t)ems_read(address + 3u) << 24);
}

static INLINE void ems_write(const uint32_t address, const uint8_t data) {
    const uint32_t phys_addr = physical_address(address);
    if (butter_psram_size)
        EMS[phys_addr] = data;
    else if (PSRAM_AVAILABLE)
        write8psram(phys_addr + EMS_PSRAM_OFFSET, data);
    else
        swap_write(phys_addr + EMS_PSRAM_OFFSET, data);
}


static INLINE void ems_writew(const uint32_t address, const uint16_t data) {
    ems_write(address, (uint8_t)data);
    ems_write(address + 1u, (uint8_t)(data >> 8));
}

static INLINE void ems_writedw(const uint32_t address, const uint32_t data) {
    ems_write(address, (uint8_t)data);
    ems_write(address + 1u, (uint8_t)(data >> 8));
    ems_write(address + 2u, (uint8_t)(data >> 16));
    ems_write(address + 3u, (uint8_t)(data >> 24));
}
