#include "r36sx_disk_config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R36SX_PICO286_CONFIG_PATH "pico_286.conf"
#define R36SX_PICO286_ABS_CONFIG_PATH "/mnt/sdcard/MIPS_NATIVE/pico_286/pico_286.conf"
#define R36SX_PICO286_MAX_DISK_PATH 192
#define R36SX_PICO286_MIN_CPU_MHZ 0.100
#define R36SX_PICO286_MAX_CPU_MHZ 250.000

typedef struct {
    uint8_t bios_drive;
    const char *name;
    const char *alias;
    char path[R36SX_PICO286_MAX_DISK_PATH];
    int configured;
} r36sx_pico286_disk_entry_t;

static r36sx_pico286_disk_entry_t disk_entries[] = {
    { 0, "fdd0", "drive0", "", 0 },
    { 1, "fdd1", "drive1", "", 0 },
    { 128, "hdd0", "drive128", "", 0 },
    { 129, "hdd1", "drive129", "", 0 },
};

static int disk_config_loaded = 0;
static char disk_config_dir[R36SX_PICO286_MAX_DISK_PATH] = "";
static uint32_t cpu_exec_loops = 0;

static char *trim_space(char *text)
{
    char *end;

    while (*text && isspace((unsigned char)*text)) {
        text++;
    }

    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';

    return text;
}

static char *trim_quotes(char *text)
{
    size_t len = strlen(text);

    if (len >= 2 &&
        ((text[0] == '"' && text[len - 1] == '"') ||
         (text[0] == '\'' && text[len - 1] == '\''))) {
        text[len - 1] = '\0';
        return text + 1;
    }

    return text;
}

static int key_equals(const char *left, const char *right)
{
    while (*left && *right) {
        if (tolower((unsigned char)*left) != tolower((unsigned char)*right)) {
            return 0;
        }
        left++;
        right++;
    }

    return *left == '\0' && *right == '\0';
}

static r36sx_pico286_disk_entry_t *find_disk_entry(const char *key)
{
    size_t i;

    for (i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        if (key_equals(key, disk_entries[i].name) ||
            key_equals(key, disk_entries[i].alias)) {
            return &disk_entries[i];
        }
    }

    if (key_equals(key, "drive80h") || key_equals(key, "0x80")) {
        return &disk_entries[2];
    }
    if (key_equals(key, "drive81h") || key_equals(key, "0x81")) {
        return &disk_entries[3];
    }

    return NULL;
}

static int is_absolute_path(const char *path)
{
    return path[0] == '/' || path[0] == '\\' ||
           (isalpha((unsigned char)path[0]) && path[1] == ':');
}

static void set_config_dir(const char *config_path)
{
    const char *last_slash = strrchr(config_path, '/');
    const char *last_backslash = strrchr(config_path, '\\');
    const char *separator = last_slash;
    size_t len;

    if (last_backslash && (!separator || last_backslash > separator)) {
        separator = last_backslash;
    }
    if (!separator) {
        disk_config_dir[0] = '\0';
        return;
    }

    len = (size_t)(separator - config_path);
    if (len >= sizeof(disk_config_dir)) {
        len = sizeof(disk_config_dir) - 1;
    }
    memcpy(disk_config_dir, config_path, len);
    disk_config_dir[len] = '\0';
}

static int set_cpu_mhz(const char *value, int line_no)
{
    char *end = NULL;
    double mhz = strtod(value, &end);
    uint32_t loops;

    if (end) {
        end = trim_space(end);
    }
    if (value[0] == '\0' || (end && end[0] != '\0') ||
        mhz < R36SX_PICO286_MIN_CPU_MHZ ||
        mhz > R36SX_PICO286_MAX_CPU_MHZ) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring invalid cpu_mhz '%s' at line %d",
            value, line_no);
        return 0;
    }

    loops = (uint32_t)(mhz * 1000.0 + 0.5);
    if (loops == 0) {
        loops = 1;
    }
    cpu_exec_loops = loops;
    r36sx_pico286_debug_log("diskcfg: cpu_mhz=%.3f exec_loops=%u",
                            mhz, cpu_exec_loops);
    return 1;
}

static int set_config_value(const char *key, const char *value, int line_no)
{
    r36sx_pico286_disk_entry_t *entry = find_disk_entry(key);

    if (key_equals(key, "cpu_mhz") ||
        key_equals(key, "cpu_frequency_mhz")) {
        return set_cpu_mhz(value, line_no);
    }

    if (!entry) {
        r36sx_pico286_debug_log("diskcfg: ignoring unknown key '%s' at line %d",
                                key, line_no);
        return 0;
    }

    if (value[0] == '\0' || is_absolute_path(value) || disk_config_dir[0] == '\0') {
        snprintf(entry->path, sizeof(entry->path), "%s", value);
    } else {
        snprintf(entry->path, sizeof(entry->path), "%s/%s", disk_config_dir, value);
    }
    entry->configured = 1;
    r36sx_pico286_debug_log("diskcfg: %s drive=%u path='%s'",
                            entry->name, entry->bios_drive,
                            entry->path[0] ? entry->path : "<disabled>");
    return 1;
}

static void load_disk_config(void)
{
    FILE *fp;
    const char *loaded_path;
    char line[384];
    int line_no = 0;

    if (disk_config_loaded) {
        return;
    }
    disk_config_loaded = 1;

    fp = fopen(R36SX_PICO286_CONFIG_PATH, "r");
    loaded_path = R36SX_PICO286_CONFIG_PATH;
    if (!fp) {
        fp = fopen(R36SX_PICO286_ABS_CONFIG_PATH, "r");
        loaded_path = R36SX_PICO286_ABS_CONFIG_PATH;
    }
    if (!fp) {
        r36sx_pico286_debug_log("diskcfg: %s not found; using built-in defaults",
                                R36SX_PICO286_CONFIG_PATH);
        return;
    }

    r36sx_pico286_debug_log("diskcfg: loading %s", loaded_path);
    set_config_dir(loaded_path);

    while (fgets(line, sizeof(line), fp)) {
        char *equals;
        char *key;
        char *value;

        line_no++;
        key = trim_space(line);
        if (key[0] == '\0' || key[0] == '#' || key[0] == ';') {
            continue;
        }

        equals = strchr(key, '=');
        if (!equals) {
            r36sx_pico286_debug_log("diskcfg: ignoring malformed line %d", line_no);
            continue;
        }

        *equals = '\0';
        value = trim_quotes(trim_space(equals + 1));
        key = trim_space(key);
        set_config_value(key, value, line_no);
    }

    fclose(fp);
}

const char *r36sx_pico286_disk_path(uint8_t bios_drive, const char *fallback_path)
{
    size_t i;

    load_disk_config();

    for (i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        if (disk_entries[i].bios_drive == bios_drive) {
            return disk_entries[i].configured ? disk_entries[i].path : fallback_path;
        }
    }

    return fallback_path;
}

uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops)
{
    load_disk_config();

    return cpu_exec_loops ? cpu_exec_loops : fallback_loops;
}
