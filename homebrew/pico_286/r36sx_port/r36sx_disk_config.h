#ifndef R36SX_DISK_CONFIG_H
#define R36SX_DISK_CONFIG_H

#include <stdint.h>

#define R36SX_PICO286_IMAGES_DIR "images"

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

typedef enum {
    R36SX_PICO286_SCALING_NEAREST = 0,
    R36SX_PICO286_SCALING_BILINEAR = 1,
} r36sx_pico286_scaling_filter_t;

typedef enum {
    R36SX_PICO286_KEYBOARD_NORMAL = 0,
    R36SX_PICO286_KEYBOARD_OVERLAY = 1,
} r36sx_pico286_keyboard_mode_t;

typedef enum {
    R36SX_PICO286_CPU_8086 = 0,
    R36SX_PICO286_CPU_80286 = 1,
    R36SX_PICO286_CPU_80386 = 2,
} r36sx_pico286_cpu_model_t;

typedef enum {
    R36SX_PICO286_CPU_MODE_REAL = 0,
    R36SX_PICO286_CPU_MODE_PROTECTED = 1,
} r36sx_pico286_cpu_mode_t;

typedef enum {
    R36SX_PICO286_BIOS_NORMAL = 0,
    R36SX_PICO286_BIOS_TEST386 = 1,
} r36sx_pico286_bios_mode_t;

// Return the configured host image path for a BIOS disk drive, or fallback_path.
const char *r36sx_pico286_disk_path(uint8_t bios_drive,
                                    const char *fallback_path);

// Return the configured image value as written to pico_286.conf.
const char *r36sx_pico286_disk_value(uint8_t bios_drive,
                                     const char *fallback_value);

// Update the configured image value in memory. Call save_config to persist it.
void r36sx_pico286_set_disk_value(uint8_t bios_drive, const char *value);

// Return the directory that contains short-named disk image files.
const char *r36sx_pico286_image_dir_path(void);

// Return the image directory value as written to pico_286.conf.
const char *r36sx_pico286_image_dir_value(void);

// Return the directory that contains pico_286.conf, or an empty string.
const char *r36sx_pico286_config_dir(void);

// Return the host filesystem directory exposed through DOS network drive H:.
const char *r36sx_pico286_host_drive_path(void);

// Return the configured host drive value as written to pico_286.conf.
const char *r36sx_pico286_host_drive_value(void);

// Write the current Pico-286 configuration back to pico_286.conf.
int r36sx_pico286_save_config(void);

// Return the configured CPU execution quantum derived from cpu_mhz.
uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops);

// Return the configured main loop target FPS.
uint32_t r36sx_pico286_target_fps(uint32_t fallback_fps);

// Return the configured CPU compatibility model.
r36sx_pico286_cpu_model_t r36sx_pico286_cpu_model(void);

// Return non-zero when the configured CPU model is at least the requested one.
int r36sx_pico286_cpu_model_at_least(r36sx_pico286_cpu_model_t model);

// Return the configured CPU model text used when rewriting config.
const char *r36sx_pico286_cpu_model_name(void);

// Return the configured initial CPU execution mode.
r36sx_pico286_cpu_mode_t r36sx_pico286_cpu_mode(void);

// Return the configured CPU execution mode text used when rewriting config.
const char *r36sx_pico286_cpu_mode_name(void);

// Return non-zero when boot should stop at the ROM BIOS boot prompt.
int r36sx_pico286_boot_bios_prompt(void);

// Return the selected BIOS ROM provider.
r36sx_pico286_bios_mode_t r36sx_pico286_bios_mode(void);

// Return the selected BIOS ROM provider text used when rewriting config.
const char *r36sx_pico286_bios_mode_name(void);

// Update the selected BIOS ROM provider. Call save_config to persist it.
int r36sx_pico286_set_bios_mode(r36sx_pico286_bios_mode_t mode);

// Return the configured test BIOS path resolved relative to pico_286.conf.
const char *r36sx_pico286_test_bios_path(void);

// Return the test BIOS path value as written to pico_286.conf.
const char *r36sx_pico286_test_bios_value(void);

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

// Return non-zero when the on-screen app statistics overlay can be toggled.
int r36sx_pico286_app_stats_enabled(void);

// Return non-zero when AdLib/OPL2 audio is mixed into the output stream.
int r36sx_pico286_audio_adlib_enabled(void);

// Return non-zero when Sound Blaster audio is mixed into the output stream.
int r36sx_pico286_audio_sound_blaster_enabled(void);

// Return non-zero when CMS/GameBlaster audio is mixed into the output stream.
int r36sx_pico286_audio_cms_enabled(void);

// Return non-zero when Tandy SN76489 PSG audio is mixed into the output stream.
int r36sx_pico286_audio_sn76489_enabled(void);

// Return non-zero when MPU-401/MIDI audio is mixed into the output stream.
int r36sx_pico286_audio_mpu401_enabled(void);

// Return non-zero when Disney Sound Source audio is mixed into the stream.
int r36sx_pico286_audio_disney_enabled(void);

// Return non-zero when Covox Speech Thing audio is mixed into the stream.
int r36sx_pico286_audio_covox_enabled(void);

// Return the configured internal audio mixer rate, normally 44100 or 22050 Hz.
uint32_t r36sx_pico286_audio_sample_rate(uint32_t fallback_rate);

// Return conventional DOS memory reported through the BIOS Data Area.
uint32_t r36sx_pico286_conventional_memory_kb(void);

// Return upper memory available to the XMS UMB allocator.
uint32_t r36sx_pico286_upper_memory_kb(void);

// Return extended memory reported by INT 15h AH=88h.
uint32_t r36sx_pico286_extended_memory_kb(void);

// Return XMS memory exposed by the built-in XMS handler.
uint32_t r36sx_pico286_xms_memory_kb(void);

// Return the configured screenshot file format.
r36sx_pico286_screenshot_format_t r36sx_pico286_screenshot_format(void);

// Return the configured screenshot format text used when rewriting config.
const char *r36sx_pico286_screenshot_format_name(void);

// Return the filter used when the DOS image must be scaled.
r36sx_pico286_scaling_filter_t r36sx_pico286_scaling_filter(void);

// Return the configured scaling filter text used when rewriting config.
const char *r36sx_pico286_scaling_filter_name(void);

// Return how the on-screen keyboard affects the DOS image.
r36sx_pico286_keyboard_mode_t r36sx_pico286_keyboard_mode(void);

// Return the configured keyboard mode text used when rewriting config.
const char *r36sx_pico286_keyboard_mode_name(void);

#ifdef __cplusplus
}
#endif

#endif
