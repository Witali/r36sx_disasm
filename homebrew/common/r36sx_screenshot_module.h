#ifndef R36SX_SCREENSHOT_MODULE_H
#define R36SX_SCREENSHOT_MODULE_H

#include <stdint.h>

#define R36SX_SCREENSHOT_MODULE_WRITE_RGB565_SYMBOL \
    "r36sx_screenshot_write_rgb565"

typedef int (*r36sx_screenshot_module_write_rgb565_fn)(const char *path,
                                                       const uint16_t *pixels,
                                                       int width,
                                                       int height,
                                                       int format);

// Shared-object entry point used by r36sx_screenshot.c through dlsym().
int r36sx_screenshot_write_rgb565(const char *path,
                                  const uint16_t *pixels,
                                  int width,
                                  int height,
                                  int format);

#endif
