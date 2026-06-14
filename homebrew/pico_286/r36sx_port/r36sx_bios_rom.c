#include "r36sx_bios_rom.h"

#include <stdio.h>
#include <string.h>

#include "r36sx_disk_config.h"

#define R36SX_SYSTEM_BIOS_START 0xF0000u
#define R36SX_SYSTEM_BIOS_SIZE 0x10000u
#define R36SX_TEST_BIOS_START R36SX_SYSTEM_BIOS_START
#define R36SX_TEST_BIOS_SIZE R36SX_SYSTEM_BIOS_SIZE
/*
 * Keep the fixed disk parameter tables in the reserved system BIOS area.
 * C0000h..EFFFFh may be exposed as UMB/RAM to DOS memory managers such as
 * JemmEx; placing ROM-like tables there creates a partial read-only overlap.
 */
#define R36SX_BIOS_FDPT_BASE 0xF0000u
#define R36SX_BIOS_FDPT_SEGMENT 0xF000u
#define R36SX_BIOS_FDPT_OFFSET 0x0000u
#define R36SX_BIOS_FDPT_COUNT 2u
#define R36SX_BIOS_FDPT_SIZE 16u

static uint8_t test_bios[R36SX_TEST_BIOS_SIZE];
static int test_bios_load_attempted;
static int test_bios_loaded;
static uint8_t fixed_disk_parameter_table[R36SX_BIOS_FDPT_COUNT][R36SX_BIOS_FDPT_SIZE];
static uint8_t fixed_disk_parameter_table_present[R36SX_BIOS_FDPT_COUNT];

static int r36sx_bios_rom_range_inside(uint32_t address,
                                       uint32_t bytes,
                                       uint32_t base,
                                       uint32_t size)
{
    return bytes != 0u &&
           address >= base &&
           address < base + size &&
           bytes <= base + size - address;
}

static int r36sx_bios_rom_fdpt_range(uint32_t address,
                                     uint32_t bytes,
                                     uint8_t *index,
                                     uint32_t *offset)
{
    if (bytes == 0u) {
        return 0;
    }

    for (uint8_t i = 0; i < R36SX_BIOS_FDPT_COUNT; i++) {
        uint32_t base = R36SX_BIOS_FDPT_BASE +
            (uint32_t)i * R36SX_BIOS_FDPT_SIZE;

        if (fixed_disk_parameter_table_present[i] &&
            address >= base &&
            address < base + R36SX_BIOS_FDPT_SIZE &&
            bytes <= base + R36SX_BIOS_FDPT_SIZE - address) {
            if (index) {
                *index = i;
            }
            if (offset) {
                *offset = address - base;
            }
            return 1;
        }
    }

    return 0;
}

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
    if (r36sx_bios_rom_fdpt_range(address, bytes, NULL, NULL)) {
        return 1;
    }
    if (!r36sx_bios_rom_uses_test_bios()) {
        return 0;
    }
    return r36sx_bios_rom_range_inside(address, bytes,
                                       R36SX_TEST_BIOS_START,
                                       R36SX_TEST_BIOS_SIZE);
}

int r36sx_bios_rom_write_protected(uint32_t address, uint32_t bytes)
{
    if (r36sx_bios_rom_range_inside(address, bytes,
                                    R36SX_SYSTEM_BIOS_START,
                                    R36SX_SYSTEM_BIOS_SIZE)) {
        return 1;
    }
    return r36sx_bios_rom_fdpt_range(address, bytes, NULL, NULL);
}

int r36sx_bios_rom_read8(uint32_t address, uint8_t *value)
{
    uint8_t fdpt_index;
    uint32_t fdpt_offset;

    if (r36sx_bios_rom_fdpt_range(address, 1u, &fdpt_index, &fdpt_offset)) {
        *value = fixed_disk_parameter_table[fdpt_index][fdpt_offset];
        return 1;
    }
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

int r36sx_bios_rom_fixed_disk_parameter_pointer(uint8_t index,
                                                uint16_t *segment,
                                                uint16_t *offset)
{
    if (index >= R36SX_BIOS_FDPT_COUNT || !segment || !offset) {
        return 0;
    }

    *segment = R36SX_BIOS_FDPT_SEGMENT;
    *offset = (uint16_t)(R36SX_BIOS_FDPT_OFFSET +
                         (uint16_t)index * R36SX_BIOS_FDPT_SIZE);
    return 1;
}

void r36sx_bios_rom_set_fixed_disk_parameter_table(uint8_t index,
                                                   const uint8_t *table)
{
    if (index >= R36SX_BIOS_FDPT_COUNT) {
        return;
    }

    if (!table) {
        memset(fixed_disk_parameter_table[index], 0,
               sizeof(fixed_disk_parameter_table[index]));
        fixed_disk_parameter_table_present[index] = 0;
        return;
    }

    memcpy(fixed_disk_parameter_table[index], table,
           sizeof(fixed_disk_parameter_table[index]));
    fixed_disk_parameter_table_present[index] = 1;
}
