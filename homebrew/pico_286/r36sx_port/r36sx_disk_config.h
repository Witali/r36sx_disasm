#ifndef R36SX_DISK_CONFIG_H
#define R36SX_DISK_CONFIG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint16_t cyls;
    uint16_t heads;
    uint16_t sects;
} r36sx_pico286_chs_t;

// Return the configured host image path for a BIOS disk drive, or fallback_path.
const char *r36sx_pico286_disk_path(uint8_t bios_drive,
                                    const char *fallback_path);

// Return the configured CPU execution quantum derived from cpu_mhz.
uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops);

// Return non-zero when boot should stop at the ROM BIOS boot prompt.
int r36sx_pico286_boot_bios_prompt(void);

// Return non-zero when the on-screen keyboard should show cursor keys.
int r36sx_pico286_osk_cursor_keys(void);

// Return the configured boot drive order. Empty result means use ROM behavior.
uint8_t r36sx_pico286_boot_order(uint8_t *drives, uint8_t max_drives);

// Return non-zero when a configured hard-disk CHS geometry exists.
int r36sx_pico286_hdd_geometry(uint8_t bios_drive,
                               r36sx_pico286_chs_t *geometry);

#ifdef __cplusplus
}
#endif

#endif
