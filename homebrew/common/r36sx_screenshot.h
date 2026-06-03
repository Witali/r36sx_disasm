#ifndef R36SX_SCREENSHOT_H
#define R36SX_SCREENSHOT_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    R36SX_SCREENSHOT_FORMAT_BMP = 0,
    R36SX_SCREENSHOT_FORMAT_PNG = 1
} r36sx_screenshot_format_t;

typedef struct {
    const char *primary_dir;
    const char *fallback_dir;
    const char *prefix;
    const char *build_hash_sha256;
    int include_build_hash;
    int64_t unix_time;
    uint32_t sequence;
    r36sx_screenshot_format_t format;
} r36sx_screenshot_options_t;

// Save an RGB565 framebuffer to the configured screenshot directory.
int r36sx_screenshot_save_rgb565(const r36sx_screenshot_options_t *options,
                                 const uint16_t *pixels,
                                 int width,
                                 int height,
                                 char *saved_path,
                                 size_t saved_path_size);

// Write one RGB565 framebuffer file directly, without creating directories.
int r36sx_screenshot_write_rgb565_file(const char *path,
                                       const uint16_t *pixels,
                                       int width,
                                       int height,
                                       r36sx_screenshot_format_t format);

#endif
