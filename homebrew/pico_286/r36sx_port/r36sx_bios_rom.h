#ifndef R36SX_BIOS_ROM_H
#define R36SX_BIOS_ROM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int r36sx_bios_rom_read8(uint32_t address, uint8_t *value);
int r36sx_bios_rom_contains(uint32_t address, uint32_t bytes);

#ifdef __cplusplus
}
#endif

#endif
