#ifndef R36SX_BIOS_ROM_H
#define R36SX_BIOS_ROM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int r36sx_bios_rom_read8(uint32_t address, uint8_t *value);
int r36sx_bios_rom_contains(uint32_t address, uint32_t bytes);
int r36sx_bios_rom_write_protected(uint32_t address, uint32_t bytes);
int r36sx_bios_rom_fixed_disk_parameter_pointer(uint8_t index,
                                                uint16_t *segment,
                                                uint16_t *offset);
void r36sx_bios_rom_set_fixed_disk_parameter_table(uint8_t index,
                                                   const uint8_t *table);

#ifdef __cplusplus
}
#endif

#endif
