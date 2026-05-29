#ifndef R36SX_DISK_CONFIG_H
#define R36SX_DISK_CONFIG_H

#include <stdint.h>

const char *r36sx_pico286_disk_path(uint8_t bios_drive, const char *fallback_path);

#endif
