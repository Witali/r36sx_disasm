#ifndef R36SX_DISK_CONFIG_H
#define R36SX_DISK_CONFIG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Return the configured host image path for a BIOS disk drive, or fallback_path.
const char *r36sx_pico286_disk_path(uint8_t bios_drive,
                                    const char *fallback_path);

// Return the configured CPU execution quantum derived from cpu_mhz.
uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops);

#ifdef __cplusplus
}
#endif

#endif
