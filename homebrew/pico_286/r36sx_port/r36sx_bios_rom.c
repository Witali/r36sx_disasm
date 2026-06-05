#include "r36sx_bios_rom.h"

#include <stdio.h>

#include "r36sx_disk_config.h"

#define R36SX_TEST_BIOS_START 0xF0000u
#define R36SX_TEST_BIOS_SIZE 0x10000u

static uint8_t test_bios[R36SX_TEST_BIOS_SIZE];
static int test_bios_load_attempted;
static int test_bios_loaded;

static int r36sx_load_test_bios(void)
{
    const char *path;
    FILE *fp;
    size_t got;
    int extra;

    if (test_bios_load_attempted) {
        return test_bios_loaded;
    }
    test_bios_load_attempted = 1;

    path = r36sx_pico286_test_bios_path();
    fp = fopen(path, "rb");
    if (!fp) {
        r36sx_pico286_debug_log("bios: test rom open failed path='%s'", path);
        return 0;
    }

    got = fread(test_bios, 1, sizeof(test_bios), fp);
    extra = fgetc(fp);
    fclose(fp);

    if (got != sizeof(test_bios) || extra != EOF) {
        r36sx_pico286_debug_log(
            "bios: test rom invalid size path='%s' got=%lu expected=%lu extra=%d",
            path, (unsigned long)got,
            (unsigned long)sizeof(test_bios), extra != EOF);
        return 0;
    }

    test_bios_loaded = 1;
    r36sx_pico286_debug_log("bios: loaded test rom path='%s' size=%lu",
                            path, (unsigned long)got);
    return 1;
}

static int r36sx_bios_rom_uses_test_bios(void)
{
    r36sx_pico286_bios_mode_t mode = r36sx_pico286_bios_mode();

    return mode == R36SX_PICO286_BIOS_TEST386 ||
           mode == R36SX_PICO286_BIOS_TEST286;
}

int r36sx_bios_rom_contains(uint32_t address, uint32_t bytes)
{
    if (!r36sx_bios_rom_uses_test_bios()) {
        return 0;
    }
    return bytes != 0u &&
           address >= R36SX_TEST_BIOS_START &&
           address < R36SX_TEST_BIOS_START + R36SX_TEST_BIOS_SIZE &&
           bytes <= R36SX_TEST_BIOS_START + R36SX_TEST_BIOS_SIZE - address;
}

int r36sx_bios_rom_read8(uint32_t address, uint8_t *value)
{
    if (!r36sx_bios_rom_contains(address, 1u)) {
        return 0;
    }

    if (!r36sx_load_test_bios()) {
        *value = 0xffu;
        return 1;
    }

    *value = test_bios[address - R36SX_TEST_BIOS_START];
    return 1;
}
