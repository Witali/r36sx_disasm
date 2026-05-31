#include "r36sx_bios_rom.h"

#include <stdio.h>

#include "r36sx_disk_config.h"

#define R36SX_TEST386_BIOS_START 0xF0000u
#define R36SX_TEST386_BIOS_SIZE 0x10000u

static uint8_t test386_bios[R36SX_TEST386_BIOS_SIZE];
static int test386_bios_load_attempted;
static int test386_bios_loaded;

static int r36sx_load_test386_bios(void)
{
    const char *path;
    FILE *fp;
    size_t got;
    int extra;

    if (test386_bios_load_attempted) {
        return test386_bios_loaded;
    }
    test386_bios_load_attempted = 1;

    path = r36sx_pico286_test_bios_path();
    fp = fopen(path, "rb");
    if (!fp) {
        r36sx_pico286_debug_log("bios: test386 open failed path='%s'", path);
        return 0;
    }

    got = fread(test386_bios, 1, sizeof(test386_bios), fp);
    extra = fgetc(fp);
    fclose(fp);

    if (got != sizeof(test386_bios) || extra != EOF) {
        r36sx_pico286_debug_log(
            "bios: test386 invalid size path='%s' got=%lu expected=%lu extra=%d",
            path, (unsigned long)got,
            (unsigned long)sizeof(test386_bios), extra != EOF);
        return 0;
    }

    test386_bios_loaded = 1;
    r36sx_pico286_debug_log("bios: loaded test386 path='%s' size=%lu",
                            path, (unsigned long)got);
    return 1;
}

int r36sx_bios_rom_contains(uint32_t address, uint32_t bytes)
{
    if (r36sx_pico286_bios_mode() != R36SX_PICO286_BIOS_TEST386) {
        return 0;
    }
    return bytes != 0u &&
           address >= R36SX_TEST386_BIOS_START &&
           address < R36SX_TEST386_BIOS_START + R36SX_TEST386_BIOS_SIZE &&
           bytes <= R36SX_TEST386_BIOS_START + R36SX_TEST386_BIOS_SIZE - address;
}

int r36sx_bios_rom_read8(uint32_t address, uint8_t *value)
{
    if (!r36sx_bios_rom_contains(address, 1u)) {
        return 0;
    }

    if (!r36sx_load_test386_bios()) {
        *value = 0xffu;
        return 1;
    }

    *value = test386_bios[address - R36SX_TEST386_BIOS_START];
    return 1;
}
