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
#define R36SX_PICO286_MIN_CACHE_BUFFER_KB 0UL
#define R36SX_PICO286_MAX_CACHE_BUFFER_KB 1024UL
#define R36SX_PICO286_MIN_CACHE_FLUSH_SECTORS 1UL
#define R36SX_PICO286_MAX_CACHE_FLUSH_SECTORS 1024UL
#define R36SX_PICO286_MIN_CACHE_FLUSH_MS 100UL
#define R36SX_PICO286_MAX_CACHE_FLUSH_MS 60000UL
#define R36SX_PICO286_MIN_PROFILE_LOG_MS 500UL
#define R36SX_PICO286_MAX_PROFILE_LOG_MS 60000UL

typedef struct {
    uint8_t bios_drive;
    const char *name;
    const char *alias;
    char value[R36SX_PICO286_MAX_DISK_PATH];
    char path[R36SX_PICO286_MAX_DISK_PATH];
    int configured;
} r36sx_pico286_disk_entry_t;

static r36sx_pico286_disk_entry_t disk_entries[] = {
    { 0, "fdd0", "drive0", "FreeDOS1.img", "FreeDOS1.img", 1 },
    { 1, "fdd1", "drive1", "sopwith.img", "sopwith.img", 1 },
    { 128, "hdd0", "drive128", "hdd.img", "hdd.img", 1 },
    { 129, "hdd1", "drive129", "hdd2.img", "hdd2.img", 1 },
};

static int disk_config_loaded = 0;
static char disk_config_dir[R36SX_PICO286_MAX_DISK_PATH] = "";
static char disk_config_path[R36SX_PICO286_MAX_DISK_PATH] =
    R36SX_PICO286_CONFIG_PATH;
static char cpu_mhz_text[32] = "32.768";
static char boot_mode_text[32] = "normal";
static char boot_order_text[64] = "fdd0,hdd0";
static char hdd_geometry_text[2][32] = { "65,16,63", "65,16,63" };
static char disk_cache_buffer_kb_text[16] = "64";
static char disk_cache_flush_sectors_text[16] = "4";
static char disk_cache_flush_ms_text[16] = "2000";
static char profiling_enabled_text[8] = "0";
static char profiling_log_ms_text[16] = "5000";
static uint32_t cpu_exec_loops = 0;
static int boot_bios_prompt = 0;
static uint32_t disk_cache_buffer_bytes = 64u * 1024u;
static uint32_t disk_cache_flush_sectors = 4u;
static uint32_t disk_cache_flush_ms = 2000u;
static int profiling_enabled = 0;
static uint32_t profiling_log_ms = 5000u;
static uint8_t boot_order[4] = { 0, 128, 0, 0 };
static uint8_t boot_order_count = 0;
static int boot_order_configured = 0;
static r36sx_pico286_chs_t hdd_geometries[2];
static int hdd_geometry_configured[2] = { 0, 0 };

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

static int drive_token_to_bios_drive(const char *token, uint8_t *bios_drive)
{
    if (key_equals(token, "fdd0") || key_equals(token, "a") ||
        key_equals(token, "a:") || key_equals(token, "0") ||
        key_equals(token, "00h") || key_equals(token, "0x00")) {
        *bios_drive = 0;
        return 1;
    }
    if (key_equals(token, "fdd1") || key_equals(token, "b") ||
        key_equals(token, "b:") || key_equals(token, "1") ||
        key_equals(token, "01h") || key_equals(token, "0x01")) {
        *bios_drive = 1;
        return 1;
    }
    if (key_equals(token, "hdd0") || key_equals(token, "c") ||
        key_equals(token, "c:") || key_equals(token, "80h") ||
        key_equals(token, "0x80") || key_equals(token, "drive80h")) {
        *bios_drive = 128;
        return 1;
    }
    if (key_equals(token, "hdd1") || key_equals(token, "d") ||
        key_equals(token, "d:") || key_equals(token, "81h") ||
        key_equals(token, "0x81") || key_equals(token, "drive81h")) {
        *bios_drive = 129;
        return 1;
    }

    return 0;
}

static int geometry_key_to_index(const char *key, int *index)
{
    if (key_equals(key, "hdd0_geometry") ||
        key_equals(key, "drive80h_geometry") ||
        key_equals(key, "0x80_geometry")) {
        *index = 0;
        return 1;
    }
    if (key_equals(key, "hdd1_geometry") ||
        key_equals(key, "drive81h_geometry") ||
        key_equals(key, "0x81_geometry")) {
        *index = 1;
        return 1;
    }

    return 0;
}

static int is_absolute_path(const char *path)
{
    return path[0] == '/' || path[0] == '\\' ||
           (isalpha((unsigned char)path[0]) && path[1] == ':');
}

static void set_disk_entry_value(r36sx_pico286_disk_entry_t *entry,
                                 const char *value)
{
    if (!entry) {
        return;
    }

    snprintf(entry->value, sizeof(entry->value), "%s", value ? value : "");
    if (entry->value[0] == '\0' || is_absolute_path(entry->value) ||
        disk_config_dir[0] == '\0') {
        snprintf(entry->path, sizeof(entry->path), "%s", entry->value);
    } else {
        snprintf(entry->path, sizeof(entry->path), "%s/%s", disk_config_dir,
                 entry->value);
    }
    entry->configured = 1;
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

    for (size_t i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        set_disk_entry_value(&disk_entries[i], disk_entries[i].value);
    }
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
    snprintf(cpu_mhz_text, sizeof(cpu_mhz_text), "%s", value);
    r36sx_pico286_debug_log("diskcfg: cpu_mhz=%.3f exec_loops=%u",
                            mhz, cpu_exec_loops);
    return 1;
}

static int set_boot_mode(const char *value, int line_no)
{
    if (key_equals(value, "normal") ||
        key_equals(value, "disk") ||
        key_equals(value, "auto")) {
        boot_bios_prompt = 0;
        snprintf(boot_mode_text, sizeof(boot_mode_text), "normal");
        r36sx_pico286_debug_log("diskcfg: boot_mode=normal");
        return 1;
    }

    if (key_equals(value, "bios") ||
        key_equals(value, "bios_prompt") ||
        key_equals(value, "prompt")) {
        boot_bios_prompt = 1;
        snprintf(boot_mode_text, sizeof(boot_mode_text), "bios_prompt");
        r36sx_pico286_debug_log("diskcfg: boot_mode=bios_prompt");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid boot_mode '%s' at line %d",
        value, line_no);
    return 0;
}

static int set_disk_cache_uint(const char *key, const char *value,
                               unsigned long min_value,
                               unsigned long max_value,
                               uint32_t *target,
                               char *target_text,
                               size_t target_text_size,
                               unsigned long multiplier,
                               int line_no)
{
    char *end = NULL;
    unsigned long parsed = strtoul(value, &end, 10);

    if (end) {
        end = trim_space(end);
    }
    if (value[0] == '\0' || (end && end[0] != '\0') ||
        parsed < min_value || parsed > max_value) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring invalid %s '%s' at line %d",
            key, value, line_no);
        return 1;
    }

    *target = (uint32_t)(parsed * multiplier);
    snprintf(target_text, target_text_size, "%lu", parsed);
    r36sx_pico286_debug_log("diskcfg: %s=%lu", key, parsed);
    return 1;
}

static int set_disk_cache_value(const char *key, const char *value,
                                int line_no)
{
    if (key_equals(key, "disk_cache_buffer_kb") ||
        key_equals(key, "cache_buffer_kb") ||
        key_equals(key, "image_cache_buffer_kb")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_CACHE_BUFFER_KB,
            R36SX_PICO286_MAX_CACHE_BUFFER_KB,
            &disk_cache_buffer_bytes,
            disk_cache_buffer_kb_text,
            sizeof(disk_cache_buffer_kb_text),
            1024UL,
            line_no);
    }

    if (key_equals(key, "disk_cache_flush_sectors") ||
        key_equals(key, "cache_flush_sectors") ||
        key_equals(key, "image_cache_flush_sectors")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_CACHE_FLUSH_SECTORS,
            R36SX_PICO286_MAX_CACHE_FLUSH_SECTORS,
            &disk_cache_flush_sectors,
            disk_cache_flush_sectors_text,
            sizeof(disk_cache_flush_sectors_text),
            1UL,
            line_no);
    }

    if (key_equals(key, "disk_cache_flush_ms") ||
        key_equals(key, "cache_flush_ms") ||
        key_equals(key, "image_cache_flush_ms")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_CACHE_FLUSH_MS,
            R36SX_PICO286_MAX_CACHE_FLUSH_MS,
            &disk_cache_flush_ms,
            disk_cache_flush_ms_text,
            sizeof(disk_cache_flush_ms_text),
            1UL,
            line_no);
    }

    return 0;
}

static int parse_bool_value(const char *value, int *enabled)
{
    if (key_equals(value, "1") ||
        key_equals(value, "yes") ||
        key_equals(value, "true") ||
        key_equals(value, "on") ||
        key_equals(value, "enabled")) {
        *enabled = 1;
        return 1;
    }
    if (key_equals(value, "0") ||
        key_equals(value, "no") ||
        key_equals(value, "false") ||
        key_equals(value, "off") ||
        key_equals(value, "disabled")) {
        *enabled = 0;
        return 1;
    }

    return 0;
}

static int set_profiling_value(const char *key, const char *value,
                               int line_no)
{
    if (key_equals(key, "profiling") ||
        key_equals(key, "profile") ||
        key_equals(key, "profiling_enabled") ||
        key_equals(key, "profile_enabled")) {
        int enabled;

        if (!parse_bool_value(value, &enabled)) {
            r36sx_pico286_debug_log(
                "diskcfg: ignoring invalid %s '%s' at line %d",
                key, value, line_no);
            return 1;
        }

        profiling_enabled = enabled;
        snprintf(profiling_enabled_text, sizeof(profiling_enabled_text),
                 "%d", enabled ? 1 : 0);
        r36sx_pico286_debug_log("diskcfg: profiling_enabled=%d",
                                profiling_enabled);
        return 1;
    }

    if (key_equals(key, "profiling_log_ms") ||
        key_equals(key, "profile_log_ms")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_PROFILE_LOG_MS,
            R36SX_PICO286_MAX_PROFILE_LOG_MS,
            &profiling_log_ms,
            profiling_log_ms_text,
            sizeof(profiling_log_ms_text),
            1UL,
            line_no);
    }

    return 0;
}

static int set_boot_order(char *value, int line_no)
{
    uint8_t parsed[4] = { 0, 0, 0, 0 };
    uint8_t count = 0;
    char *token = value;
    char original[sizeof(boot_order_text)];

    snprintf(original, sizeof(original), "%s", value);

    if (key_equals(value, "rom") || key_equals(value, "bios")) {
        boot_order_count = 0;
        boot_order_configured = 1;
        snprintf(boot_order_text, sizeof(boot_order_text), "rom");
        r36sx_pico286_debug_log("diskcfg: boot_order=rom");
        return 1;
    }

    while (*token) {
        char *end;
        uint8_t drive;

        while (*token == ',' || *token == ';' ||
               isspace((unsigned char)*token)) {
            token++;
        }
        if (!*token) {
            break;
        }

        end = token;
        while (*end && *end != ',' && *end != ';' &&
               !isspace((unsigned char)*end)) {
            end++;
        }
        if (*end) {
            *end++ = '\0';
        }

        if (count >= sizeof(parsed) ||
            !drive_token_to_bios_drive(token, &drive)) {
            r36sx_pico286_debug_log(
                "diskcfg: ignoring invalid boot_order '%s' at line %d",
                value, line_no);
            return 0;
        }

        parsed[count++] = drive;
        token = end;
    }

    if (count == 0) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring empty boot_order at line %d", line_no);
        return 0;
    }

    memcpy(boot_order, parsed, count);
    boot_order_count = count;
    boot_order_configured = 1;
    snprintf(boot_order_text, sizeof(boot_order_text), "%s", original);
    r36sx_pico286_debug_log(
        "diskcfg: boot_order count=%u first=0x%02x",
        (unsigned int)boot_order_count, boot_order[0]);
    return 1;
}

static int set_hdd_geometry(const char *key, const char *value, int line_no)
{
    const char *cursor = value;
    unsigned long parsed[3];
    r36sx_pico286_chs_t geometry;
    int index;

    if (!geometry_key_to_index(key, &index)) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        char *end = NULL;

        while (*cursor == ',' || *cursor == ';' || *cursor == ':' ||
               *cursor == '/' || *cursor == 'x' || *cursor == 'X' ||
               isspace((unsigned char)*cursor)) {
            cursor++;
        }
        if (!isdigit((unsigned char)*cursor)) {
            r36sx_pico286_debug_log(
                "diskcfg: ignoring invalid %s '%s' at line %d",
                key, value, line_no);
            return 1;
        }

        parsed[i] = strtoul(cursor, &end, 10);
        cursor = end;
    }

    while (*cursor && isspace((unsigned char)*cursor)) {
        cursor++;
    }
    if (*cursor) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring invalid %s '%s' at line %d",
            key, value, line_no);
        return 1;
    }

    if (parsed[0] < 1 || parsed[0] > 1023 ||
        parsed[1] < 1 || parsed[1] > 255 ||
        parsed[2] < 1 || parsed[2] > 63) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring out-of-range %s '%s' at line %d",
            key, value, line_no);
        return 1;
    }

    geometry.cyls = (uint16_t)parsed[0];
    geometry.heads = (uint16_t)parsed[1];
    geometry.sects = (uint16_t)parsed[2];
    hdd_geometries[index] = geometry;
    hdd_geometry_configured[index] = 1;
    snprintf(hdd_geometry_text[index], sizeof(hdd_geometry_text[index]),
             "%u,%u,%u", geometry.cyls, geometry.heads, geometry.sects);
    r36sx_pico286_debug_log("diskcfg: %s=%u,%u,%u",
                            key, geometry.cyls, geometry.heads,
                            geometry.sects);
    return 1;
}

static int set_config_value(const char *key, const char *value, int line_no)
{
    r36sx_pico286_disk_entry_t *entry = find_disk_entry(key);
    char mutable_value[384];

    if (key_equals(key, "cpu_mhz") ||
        key_equals(key, "cpu_frequency_mhz")) {
        return set_cpu_mhz(value, line_no);
    }
    if (key_equals(key, "boot_mode")) {
        return set_boot_mode(value, line_no);
    }
    if (set_disk_cache_value(key, value, line_no)) {
        return 1;
    }
    if (set_profiling_value(key, value, line_no)) {
        return 1;
    }
    if (key_equals(key, "osk_cursor_keys") ||
        key_equals(key, "keyboard_cursor_keys") ||
        key_equals(key, "screen_keyboard_cursor_keys")) {
        return 1;
    }
    if (key[0] == '[') {
        return 1;
    }
    if (key_equals(key, "boot_order")) {
        snprintf(mutable_value, sizeof(mutable_value), "%s", value);
        return set_boot_order(mutable_value, line_no);
    }
    if (set_hdd_geometry(key, value, line_no)) {
        return 1;
    }

    if (!entry) {
        r36sx_pico286_debug_log("diskcfg: ignoring unknown key '%s' at line %d",
                                key, line_no);
        return 0;
    }

    set_disk_entry_value(entry, value);
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
    snprintf(disk_config_path, sizeof(disk_config_path), "%s", loaded_path);
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

const char *r36sx_pico286_disk_value(uint8_t bios_drive,
                                     const char *fallback_value)
{
    size_t i;

    load_disk_config();

    for (i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        if (disk_entries[i].bios_drive == bios_drive) {
            return disk_entries[i].configured ?
                disk_entries[i].value : fallback_value;
        }
    }

    return fallback_value;
}

void r36sx_pico286_set_disk_value(uint8_t bios_drive, const char *value)
{
    size_t i;

    load_disk_config();

    for (i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        if (disk_entries[i].bios_drive == bios_drive) {
            set_disk_entry_value(&disk_entries[i], value);
            r36sx_pico286_debug_log("diskcfg: set %s='%s'",
                                    disk_entries[i].name,
                                    disk_entries[i].value);
            return;
        }
    }
}

const char *r36sx_pico286_config_dir(void)
{
    load_disk_config();

    return disk_config_dir;
}

int r36sx_pico286_save_config(void)
{
    FILE *fp;

    load_disk_config();

    fp = fopen(disk_config_path, "w");
    if (!fp) {
        r36sx_pico286_debug_log("diskcfg: save failed path='%s'",
                                disk_config_path);
        return 0;
    }

    fprintf(fp, "# Pico-286 disk image bindings for the R36SX native port.\n");
    fprintf(fp, "# Paths are relative to this directory unless an absolute path is used.\n\n");
    fprintf(fp, "cpu_mhz=%s\n", cpu_mhz_text);
    fprintf(fp, "boot_mode=%s\n", boot_mode_text);
    fprintf(fp, "boot_order=%s\n\n", boot_order_text);
    fprintf(fp, "# Disk image I/O cache.\n");
    fprintf(fp, "# disk_cache_buffer_kb is the stdio buffer allocated per image file.\n");
    fprintf(fp, "# fflush runs after disk_cache_flush_sectors dirty sectors or after\n");
    fprintf(fp, "# disk_cache_flush_ms milliseconds since the latest cached write.\n");
    fprintf(fp, "[disk_cache]\n");
    fprintf(fp, "disk_cache_buffer_kb=%s\n", disk_cache_buffer_kb_text);
    fprintf(fp, "disk_cache_flush_sectors=%s\n", disk_cache_flush_sectors_text);
    fprintf(fp, "disk_cache_flush_ms=%s\n\n", disk_cache_flush_ms_text);
    fprintf(fp, "# Optional runtime profiling written to pico_286.log.\n");
    fprintf(fp, "[profiling]\n");
    fprintf(fp, "profiling_enabled=%s\n", profiling_enabled_text);
    fprintf(fp, "profiling_log_ms=%s\n\n", profiling_log_ms_text);
    fprintf(fp, "fdd0=%s\n", disk_entries[0].value);
    fprintf(fp, "fdd1=%s\n", disk_entries[1].value);
    fprintf(fp, "hdd0=%s\n", disk_entries[2].value);
    fprintf(fp, "hdd0_geometry=%s\n", hdd_geometry_text[0]);
    fprintf(fp, "hdd1=%s\n", disk_entries[3].value);
    fprintf(fp, "hdd1_geometry=%s\n", hdd_geometry_text[1]);

    if (fclose(fp) != 0) {
        r36sx_pico286_debug_log("diskcfg: save close failed path='%s'",
                                disk_config_path);
        return 0;
    }

    r36sx_pico286_debug_log("diskcfg: saved %s", disk_config_path);
    return 1;
}

uint32_t r36sx_pico286_cpu_exec_loops(uint32_t fallback_loops)
{
    load_disk_config();

    return cpu_exec_loops ? cpu_exec_loops : fallback_loops;
}

int r36sx_pico286_boot_bios_prompt(void)
{
    load_disk_config();

    return boot_bios_prompt;
}

uint8_t r36sx_pico286_boot_order(uint8_t *drives, uint8_t max_drives)
{
    uint8_t count;

    load_disk_config();

    if (!boot_order_configured) {
        static const uint8_t default_order[] = { 0, 128 };
        count = max_drives < sizeof(default_order) ?
                max_drives : (uint8_t)sizeof(default_order);
        memcpy(drives, default_order, count);
        return count;
    }

    count = max_drives < boot_order_count ? max_drives : boot_order_count;
    if (count) {
        memcpy(drives, boot_order, count);
    }
    return count;
}

int r36sx_pico286_set_boot_order_value(const char *value)
{
    char mutable_value[sizeof(boot_order_text)];

    load_disk_config();
    snprintf(mutable_value, sizeof(mutable_value), "%s", value ? value : "");
    return set_boot_order(mutable_value, 0);
}

int r36sx_pico286_hdd_geometry(uint8_t bios_drive,
                               r36sx_pico286_chs_t *geometry)
{
    int index;

    load_disk_config();

    if (bios_drive == 128) {
        index = 0;
    } else if (bios_drive == 129) {
        index = 1;
    } else {
        return 0;
    }

    if (!hdd_geometry_configured[index]) {
        return 0;
    }

    *geometry = hdd_geometries[index];
    return 1;
}

uint32_t r36sx_pico286_disk_cache_buffer_bytes(void)
{
    load_disk_config();

    return disk_cache_buffer_bytes;
}

uint32_t r36sx_pico286_disk_cache_flush_sectors(void)
{
    load_disk_config();

    return disk_cache_flush_sectors;
}

uint32_t r36sx_pico286_disk_cache_flush_ms(void)
{
    load_disk_config();

    return disk_cache_flush_ms;
}

int r36sx_pico286_profiling_enabled(void)
{
    load_disk_config();

    return profiling_enabled;
}

uint32_t r36sx_pico286_profiling_log_ms(void)
{
    load_disk_config();

    return profiling_log_ms;
}
