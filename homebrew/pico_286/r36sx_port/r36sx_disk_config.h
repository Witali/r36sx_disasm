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

typedef enum {
    R36SX_PICO286_SCREENSHOT_FORMAT_PNG = 0,
    R36SX_PICO286_SCREENSHOT_FORMAT_BMP = 1,
} r36sx_pico286_screenshot_format_t;

// Return the configured host image path for a BIOS disk drive, or fallback_path.
const char *r36sx_pico286_disk_path(uint8_t bios_drive,
                                    const char *fallback_path);

// Return the configured image value as written to pico_286.conf.
const char *r36sx_pico286_disk_value(uint8_t bios_drive,
                                     const char *fallback_value);

// Update the configured image value in memory. Call save_config to persist it.
void r36sx_pico286_set_disk_value(uint8_t bios_drive, const char *value);

// Return the directory that contains pico_286.conf, or an empty string.
const char *r36sx_pico286_config_dir(void);

// Write the current Pico-286 configuration back to pico_286.conf.
int r36sx_pico286_save_config(void);

// Return the configured CPU execution quantum derived from cpu_mhz.
uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops);

// Return non-zero when boot should stop at the ROM BIOS boot prompt.
int r36sx_pico286_boot_bios_prompt(void);

// Return the configured boot drive order. Empty result means use ROM behavior.
uint8_t r36sx_pico286_boot_order(uint8_t *drives, uint8_t max_drives);

// Update the configured boot order text in memory. Call save_config to persist.
int r36sx_pico286_set_boot_order_value(const char *value);

// Return non-zero when a configured hard-disk CHS geometry exists.
int r36sx_pico286_hdd_geometry(uint8_t bios_drive,
                               r36sx_pico286_chs_t *geometry);

// Return the stdio buffer size to use for each host disk image file.
uint32_t r36sx_pico286_disk_cache_buffer_bytes(void);

// Return the number of dirty 512-byte sectors that triggers fflush().
uint32_t r36sx_pico286_disk_cache_flush_sectors(void);

// Return the dirty-cache age in milliseconds that triggers fflush().
uint32_t r36sx_pico286_disk_cache_flush_ms(void);

// Return non-zero when runtime profiling log output is enabled.
int r36sx_pico286_profiling_enabled(void);

// Return how often profiling summaries should be written to the log.
uint32_t r36sx_pico286_profiling_log_ms(void);

// Return the configured screenshot file format.
r36sx_pico286_screenshot_format_t r36sx_pico286_screenshot_format(void);

// Return the configured screenshot format text used when rewriting config.
const char *r36sx_pico286_screenshot_format_name(void);

#ifdef __cplusplus
}
#endif

#endif
