#include "r36sx_disk_config.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
#define R36SX_PICO286_MIN_TARGET_FPS 1UL
#define R36SX_PICO286_MAX_TARGET_FPS 240UL
#define R36SX_PICO286_MIN_CONVENTIONAL_KB 64UL
#define R36SX_PICO286_MAX_CONVENTIONAL_KB 640UL
#define R36SX_PICO286_MIN_UPPER_KB 0UL
#define R36SX_PICO286_MAX_UPPER_KB 176UL
#define R36SX_PICO286_MIN_EXTENDED_KB 0UL
#define R36SX_PICO286_MAX_EXTENDED_KB 4096UL
#define R36SX_PICO286_MIN_XMS_KB 0UL
#define R36SX_PICO286_MAX_XMS_KB 4096UL

typedef struct {
    uint8_t bios_drive;
    const char *name;
    const char *alias;
    char value[R36SX_PICO286_MAX_DISK_PATH];
    char path[R36SX_PICO286_MAX_DISK_PATH];
    int configured;
} r36sx_pico286_disk_entry_t;

static r36sx_pico286_disk_entry_t disk_entries[] = {
    { 0, "fdd0", "drive0", "FreeDOS1.img",
      R36SX_PICO286_IMAGES_DIR "/FreeDOS1.img", 1 },
    { 1, "fdd1", "drive1", "sopwith.img",
      R36SX_PICO286_IMAGES_DIR "/sopwith.img", 1 },
    { 128, "hdd0", "drive128", "hdd.img",
      R36SX_PICO286_IMAGES_DIR "/hdd.img", 1 },
    { 129, "hdd1", "drive129", "hdd2.img",
      R36SX_PICO286_IMAGES_DIR "/hdd2.img", 1 },
};

static int disk_config_loaded = 0;
static char disk_config_dir[R36SX_PICO286_MAX_DISK_PATH] = "";
static char disk_config_path[R36SX_PICO286_MAX_DISK_PATH] =
    R36SX_PICO286_CONFIG_PATH;
static char image_dir_value[R36SX_PICO286_MAX_DISK_PATH] =
    R36SX_PICO286_IMAGES_DIR;
static char image_dir_path[R36SX_PICO286_MAX_DISK_PATH] =
    R36SX_PICO286_IMAGES_DIR;
static char cpu_model_text[16] = "80386";
static char cpu_mode_text[16] = "real";
static char cpu_mhz_text[32] = "32.768";
static char bios_mode_text[16] = "normal";
static char test_bios_value[R36SX_PICO286_MAX_DISK_PATH] = "test386.bin";
static char test_bios_path[R36SX_PICO286_MAX_DISK_PATH] = "test386.bin";
static char boot_mode_text[32] = "normal";
static char boot_order_text[64] = "fdd0,hdd0";
static char host_drive_value[R36SX_PICO286_MAX_DISK_PATH] = "host";
static char host_drive_path[R36SX_PICO286_MAX_DISK_PATH] = "host";
static char hdd_geometry_text[2][32] = { "65,16,63", "65,16,63" };
static char disk_cache_buffer_kb_text[16] = "64";
static char disk_cache_flush_sectors_text[16] = "4";
static char disk_cache_flush_ms_text[16] = "2000";
static char profiling_enabled_text[8] = "0";
static char profiling_log_ms_text[16] = "5000";
static char app_stats_enabled_text[8] = "1";
static char target_fps_text[16] = "60";
static char screenshot_format_text[8] = "png";
static char scaling_filter_text[16] = "nearest";
static char keyboard_mode_text[16] = "normal";
static char audio_adlib_enabled_text[8] = "1";
static char audio_sound_blaster_enabled_text[8] = "1";
static char audio_cms_enabled_text[8] = "1";
static char audio_sn76489_enabled_text[8] = "1";
static char audio_mpu401_enabled_text[8] = "1";
static char audio_disney_enabled_text[8] = "1";
static char audio_covox_enabled_text[8] = "1";
static char audio_sample_rate_text[16] = "44100";
static char conventional_memory_kb_text[16] = "640";
static char upper_memory_kb_text[16] = "176";
static char extended_memory_kb_text[16] = "64";
static char xms_memory_kb_text[16] = "4096";
static uint32_t cpu_exec_loops = 0;
static r36sx_pico286_cpu_model_t cpu_model = R36SX_PICO286_CPU_80386;
static r36sx_pico286_cpu_mode_t cpu_mode = R36SX_PICO286_CPU_MODE_REAL;
static r36sx_pico286_bios_mode_t bios_mode = R36SX_PICO286_BIOS_NORMAL;
static int boot_bios_prompt = 0;
static uint32_t disk_cache_buffer_bytes = 64u * 1024u;
static uint32_t disk_cache_flush_sectors = 4u;
static uint32_t disk_cache_flush_ms = 2000u;
static int profiling_enabled = 0;
static uint32_t profiling_log_ms = 5000u;
static int app_stats_enabled = 1;
static uint32_t target_fps = 60u;
static int audio_adlib_enabled = 1;
static int audio_sound_blaster_enabled = 1;
static int audio_cms_enabled = 1;
static int audio_sn76489_enabled = 1;
static int audio_mpu401_enabled = 1;
static int audio_disney_enabled = 1;
static int audio_covox_enabled = 1;
static uint32_t audio_sample_rate = 44100u;
static uint32_t conventional_memory_kb = 640u;
static uint32_t upper_memory_kb = 176u;
static uint32_t extended_memory_kb = 64u;
static uint32_t xms_memory_kb = 4096u;
static r36sx_pico286_screenshot_format_t screenshot_format =
    R36SX_PICO286_SCREENSHOT_FORMAT_PNG;
static r36sx_pico286_scaling_filter_t scaling_filter =
    R36SX_PICO286_SCALING_NEAREST;
static r36sx_pico286_keyboard_mode_t keyboard_mode =
    R36SX_PICO286_KEYBOARD_NORMAL;
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

static int path_has_separator(const char *path)
{
    return path && (strchr(path, '/') || strchr(path, '\\'));
}

static void resolve_config_relative_path(char *dest, size_t dest_size,
                                         const char *value)
{
    if (!dest || dest_size == 0) {
        return;
    }

    if (!value || value[0] == '\0' || is_absolute_path(value) ||
        disk_config_dir[0] == '\0') {
        snprintf(dest, dest_size, "%s", value ? value : "");
    } else {
        snprintf(dest, dest_size, "%s/%s", disk_config_dir, value);
    }
}

static void resolve_image_dir_file_path(char *dest, size_t dest_size,
                                        const char *filename)
{
    if (!dest || dest_size == 0) {
        return;
    }
    if (!filename || filename[0] == '\0') {
        dest[0] = '\0';
        return;
    }
    if (image_dir_path[0]) {
        snprintf(dest, dest_size, "%s/%s", image_dir_path, filename);
    } else {
        resolve_config_relative_path(dest, dest_size, filename);
    }
}

static void set_disk_entry_value(r36sx_pico286_disk_entry_t *entry,
                                 const char *value)
{
    if (!entry) {
        return;
    }

    snprintf(entry->value, sizeof(entry->value), "%s", value ? value : "");
    if (entry->value[0] &&
        !is_absolute_path(entry->value) &&
        !path_has_separator(entry->value)) {
        resolve_image_dir_file_path(entry->path, sizeof(entry->path),
                                    entry->value);
    } else {
        resolve_config_relative_path(entry->path, sizeof(entry->path),
                                     entry->value);
    }
    entry->configured = 1;
}

static void set_image_dir_value(const char *value)
{
    snprintf(image_dir_value, sizeof(image_dir_value), "%s",
             value && value[0] ? value : R36SX_PICO286_IMAGES_DIR);
    resolve_config_relative_path(image_dir_path, sizeof(image_dir_path),
                                 image_dir_value);
    for (size_t i = 0; i < sizeof(disk_entries) / sizeof(disk_entries[0]); i++) {
        set_disk_entry_value(&disk_entries[i], disk_entries[i].value);
    }
}

static void set_host_drive_value(const char *value)
{
    snprintf(host_drive_value, sizeof(host_drive_value), "%s",
             value ? value : "");
    resolve_config_relative_path(host_drive_path, sizeof(host_drive_path),
                                 host_drive_value);
}

static void set_test_bios_value(const char *value)
{
    snprintf(test_bios_value, sizeof(test_bios_value), "%s",
             value && value[0] ? value : "test386.bin");
    resolve_config_relative_path(test_bios_path, sizeof(test_bios_path),
                                 test_bios_value);
}

static void ensure_host_drive_dir(void)
{
    if (host_drive_path[0] == '\0') {
        return;
    }

    if (mkdir(host_drive_path, 0777) == 0 || errno == EEXIST) {
        return;
    }

    r36sx_pico286_debug_log("diskcfg: host drive mkdir failed path='%s' errno=%d",
                            host_drive_path, errno);
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
    set_host_drive_value(host_drive_value);
    set_test_bios_value(test_bios_value);
    set_image_dir_value(image_dir_value);
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

static int set_cpu_model(const char *value, int line_no)
{
    if (key_equals(value, "8086") ||
        key_equals(value, "i8086") ||
        key_equals(value, "86") ||
        key_equals(value, "xt")) {
        cpu_model = R36SX_PICO286_CPU_8086;
        snprintf(cpu_model_text, sizeof(cpu_model_text), "8086");
        r36sx_pico286_debug_log("diskcfg: cpu_model=8086");
        return 1;
    }

    if (key_equals(value, "80286") ||
        key_equals(value, "286") ||
        key_equals(value, "i286") ||
        key_equals(value, "at")) {
        cpu_model = R36SX_PICO286_CPU_80286;
        snprintf(cpu_model_text, sizeof(cpu_model_text), "80286");
        r36sx_pico286_debug_log("diskcfg: cpu_model=80286");
        return 1;
    }

    if (key_equals(value, "80386") ||
        key_equals(value, "386") ||
        key_equals(value, "i386")) {
        cpu_model = R36SX_PICO286_CPU_80386;
        snprintf(cpu_model_text, sizeof(cpu_model_text), "80386");
        r36sx_pico286_debug_log("diskcfg: cpu_model=80386");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid cpu_model '%s' at line %d",
        value, line_no);
    return 0;
}

static int set_cpu_mode(const char *value, int line_no)
{
    if (key_equals(value, "real") ||
        key_equals(value, "real_mode") ||
        key_equals(value, "rm")) {
        cpu_mode = R36SX_PICO286_CPU_MODE_REAL;
        snprintf(cpu_mode_text, sizeof(cpu_mode_text), "real");
        r36sx_pico286_debug_log("diskcfg: cpu_mode=real");
        return 1;
    }

    if (key_equals(value, "protected") ||
        key_equals(value, "protected_mode") ||
        key_equals(value, "pm")) {
        cpu_mode = R36SX_PICO286_CPU_MODE_PROTECTED;
        snprintf(cpu_mode_text, sizeof(cpu_mode_text), "protected");
        r36sx_pico286_debug_log("diskcfg: cpu_mode=protected");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid cpu_mode '%s' at line %d",
        value, line_no);
    return 0;
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

static int set_bios_mode_value(const char *value, int line_no)
{
    if (key_equals(value, "normal") ||
        key_equals(value, "builtin") ||
        key_equals(value, "default")) {
        bios_mode = R36SX_PICO286_BIOS_NORMAL;
        snprintf(bios_mode_text, sizeof(bios_mode_text), "normal");
        r36sx_pico286_debug_log("diskcfg: bios=normal");
        return 1;
    }

    if (key_equals(value, "test386") ||
        key_equals(value, "test") ||
        key_equals(value, "test_bios")) {
        bios_mode = R36SX_PICO286_BIOS_TEST386;
        snprintf(bios_mode_text, sizeof(bios_mode_text), "test386");
        r36sx_pico286_debug_log("diskcfg: bios=test386");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid bios '%s' at line %d",
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

static int set_screenshot_format(const char *key, const char *value,
                                 int line_no)
{
    if (!(key_equals(key, "screenshot_format") ||
          key_equals(key, "screenshotformat") ||
          key_equals(key, "screenshot_file_format"))) {
        return 0;
    }

    if (key_equals(value, "png")) {
        screenshot_format = R36SX_PICO286_SCREENSHOT_FORMAT_PNG;
        snprintf(screenshot_format_text, sizeof(screenshot_format_text),
                 "png");
        r36sx_pico286_debug_log("diskcfg: screenshot_format=png");
        return 1;
    }

    if (key_equals(value, "bmp")) {
        screenshot_format = R36SX_PICO286_SCREENSHOT_FORMAT_BMP;
        snprintf(screenshot_format_text, sizeof(screenshot_format_text),
                 "bmp");
        r36sx_pico286_debug_log("diskcfg: screenshot_format=bmp");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid %s '%s' at line %d",
        key, value, line_no);
    return 1;
}

static int set_scaling_filter(const char *key, const char *value,
                              int line_no)
{
    if (!(key_equals(key, "scaling_filter") ||
          key_equals(key, "scale_filter") ||
          key_equals(key, "video_scaling_filter") ||
          key_equals(key, "screen_scaling_filter"))) {
        return 0;
    }

    if (key_equals(value, "nearest") ||
        key_equals(value, "nearest_pixel") ||
        key_equals(value, "pixel")) {
        scaling_filter = R36SX_PICO286_SCALING_NEAREST;
        snprintf(scaling_filter_text, sizeof(scaling_filter_text),
                 "nearest");
        r36sx_pico286_debug_log("diskcfg: scaling_filter=nearest");
        return 1;
    }

    if (key_equals(value, "bilinear") ||
        key_equals(value, "linear")) {
        scaling_filter = R36SX_PICO286_SCALING_BILINEAR;
        snprintf(scaling_filter_text, sizeof(scaling_filter_text),
                 "bilinear");
        r36sx_pico286_debug_log("diskcfg: scaling_filter=bilinear");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid %s '%s' at line %d",
        key, value, line_no);
    return 1;
}

static int set_keyboard_mode(const char *key, const char *value,
                             int line_no)
{
    if (!(key_equals(key, "keyboard_mode") ||
          key_equals(key, "osk_mode") ||
          key_equals(key, "screen_keyboard_mode"))) {
        return 0;
    }

    if (key_equals(value, "normal") ||
        key_equals(value, "resize") ||
        key_equals(value, "scaled")) {
        keyboard_mode = R36SX_PICO286_KEYBOARD_NORMAL;
        snprintf(keyboard_mode_text, sizeof(keyboard_mode_text), "normal");
        r36sx_pico286_debug_log("diskcfg: keyboard_mode=normal");
        return 1;
    }

    if (key_equals(value, "overlay") ||
        key_equals(value, "overlap") ||
        key_equals(value, "over")) {
        keyboard_mode = R36SX_PICO286_KEYBOARD_OVERLAY;
        snprintf(keyboard_mode_text, sizeof(keyboard_mode_text), "overlay");
        r36sx_pico286_debug_log("diskcfg: keyboard_mode=overlay");
        return 1;
    }

    r36sx_pico286_debug_log(
        "diskcfg: ignoring invalid %s '%s' at line %d",
        key, value, line_no);
    return 1;
}

static int set_app_stats_value(const char *key, const char *value,
                               int line_no)
{
    if (key_equals(key, "app_stats_enabled") ||
        key_equals(key, "stats_enabled") ||
        key_equals(key, "statistics_enabled") ||
        key_equals(key, "stats_overlay_enabled")) {
        int enabled;

        if (!parse_bool_value(value, &enabled)) {
            r36sx_pico286_debug_log(
                "diskcfg: ignoring invalid %s '%s' at line %d",
                key, value, line_no);
            return 1;
        }

        app_stats_enabled = enabled;
        snprintf(app_stats_enabled_text, sizeof(app_stats_enabled_text),
                 "%d", enabled ? 1 : 0);
        r36sx_pico286_debug_log("diskcfg: app_stats_enabled=%d",
                                app_stats_enabled);
        return 1;
    }

    return 0;
}

static int set_audio_bool_value(const char *key, const char *value,
                                int line_no, const char *canonical_key,
                                int *target, char *text, size_t text_size)
{
    int enabled;

    if (!parse_bool_value(value, &enabled)) {
        r36sx_pico286_debug_log(
            "diskcfg: ignoring invalid %s '%s' at line %d",
            key, value, line_no);
        return 1;
    }

    *target = enabled;
    snprintf(text, text_size, "%d", enabled ? 1 : 0);
    r36sx_pico286_debug_log("diskcfg: %s=%d", canonical_key, enabled);
    return 1;
}

static int set_audio_value(const char *key, const char *value, int line_no)
{
    if (key_equals(key, "audio_sample_rate") ||
        key_equals(key, "audio_rate") ||
        key_equals(key, "sample_rate") ||
        key_equals(key, "sound_frequency") ||
        key_equals(key, "sound_rate")) {
        if (key_equals(value, "44100") ||
            key_equals(value, "44") ||
            key_equals(value, "44k") ||
            key_equals(value, "44khz")) {
            audio_sample_rate = 44100u;
            snprintf(audio_sample_rate_text, sizeof(audio_sample_rate_text),
                     "44100");
            r36sx_pico286_debug_log("diskcfg: audio_sample_rate=44100");
            return 1;
        }
        if (key_equals(value, "22050") ||
            key_equals(value, "22") ||
            key_equals(value, "22k") ||
            key_equals(value, "22khz")) {
            audio_sample_rate = 22050u;
            snprintf(audio_sample_rate_text, sizeof(audio_sample_rate_text),
                     "22050");
            r36sx_pico286_debug_log("diskcfg: audio_sample_rate=22050");
            return 1;
        }

        r36sx_pico286_debug_log(
            "diskcfg: ignoring invalid %s '%s' at line %d",
            key, value, line_no);
        return 1;
    }

    if (key_equals(key, "audio_adlib") ||
        key_equals(key, "audio_opl2") ||
        key_equals(key, "adlib") ||
        key_equals(key, "opl2")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_adlib",
            &audio_adlib_enabled,
            audio_adlib_enabled_text,
            sizeof(audio_adlib_enabled_text));
    }

    if (key_equals(key, "audio_sound_blaster") ||
        key_equals(key, "audio_soundblaster") ||
        key_equals(key, "audio_sb") ||
        key_equals(key, "sound_blaster") ||
        key_equals(key, "soundblaster") ||
        key_equals(key, "sb")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_sound_blaster",
            &audio_sound_blaster_enabled,
            audio_sound_blaster_enabled_text,
            sizeof(audio_sound_blaster_enabled_text));
    }

    if (key_equals(key, "audio_cms") ||
        key_equals(key, "audio_game_blaster") ||
        key_equals(key, "audio_gameblaster") ||
        key_equals(key, "cms") ||
        key_equals(key, "game_blaster") ||
        key_equals(key, "gameblaster")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_cms",
            &audio_cms_enabled,
            audio_cms_enabled_text,
            sizeof(audio_cms_enabled_text));
    }

    if (key_equals(key, "audio_sn76489") ||
        key_equals(key, "audio_tandy") ||
        key_equals(key, "audio_tandy_psg") ||
        key_equals(key, "sn76489") ||
        key_equals(key, "tandy_sound") ||
        key_equals(key, "tandy_psg")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_sn76489",
            &audio_sn76489_enabled,
            audio_sn76489_enabled_text,
            sizeof(audio_sn76489_enabled_text));
    }

    if (key_equals(key, "audio_mpu401") ||
        key_equals(key, "audio_midi") ||
        key_equals(key, "mpu401") ||
        key_equals(key, "midi")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_mpu401",
            &audio_mpu401_enabled,
            audio_mpu401_enabled_text,
            sizeof(audio_mpu401_enabled_text));
    }

    if (key_equals(key, "audio_disney") ||
        key_equals(key, "audio_disney_sound_source") ||
        key_equals(key, "audio_dss") ||
        key_equals(key, "disney") ||
        key_equals(key, "disney_sound_source") ||
        key_equals(key, "dss")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_disney",
            &audio_disney_enabled,
            audio_disney_enabled_text,
            sizeof(audio_disney_enabled_text));
    }

    if (key_equals(key, "audio_covox") ||
        key_equals(key, "covox") ||
        key_equals(key, "speech_thing") ||
        key_equals(key, "covox_speech_thing")) {
        return set_audio_bool_value(
            key, value, line_no, "audio_covox",
            &audio_covox_enabled,
            audio_covox_enabled_text,
            sizeof(audio_covox_enabled_text));
    }

    return 0;
}

static int set_timing_value(const char *key, const char *value, int line_no)
{
    if (key_equals(key, "target_fps") ||
        key_equals(key, "main_loop_fps") ||
        key_equals(key, "frame_rate") ||
        key_equals(key, "fps")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_TARGET_FPS,
            R36SX_PICO286_MAX_TARGET_FPS,
            &target_fps,
            target_fps_text,
            sizeof(target_fps_text),
            1UL,
            line_no);
    }

    return 0;
}

static int set_memory_value(const char *key, const char *value, int line_no)
{
    if (key_equals(key, "conventional_kb") ||
        key_equals(key, "conventional_memory_kb") ||
        key_equals(key, "conventional_ram_kb")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_CONVENTIONAL_KB,
            R36SX_PICO286_MAX_CONVENTIONAL_KB,
            &conventional_memory_kb,
            conventional_memory_kb_text,
            sizeof(conventional_memory_kb_text),
            1UL,
            line_no);
    }

    if (key_equals(key, "upper_kb") ||
        key_equals(key, "upper_memory_kb") ||
        key_equals(key, "umb_kb")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_UPPER_KB,
            R36SX_PICO286_MAX_UPPER_KB,
            &upper_memory_kb,
            upper_memory_kb_text,
            sizeof(upper_memory_kb_text),
            1UL,
            line_no);
    }

    if (key_equals(key, "extended_kb") ||
        key_equals(key, "extended_memory_kb") ||
        key_equals(key, "int15_extended_kb")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_EXTENDED_KB,
            R36SX_PICO286_MAX_EXTENDED_KB,
            &extended_memory_kb,
            extended_memory_kb_text,
            sizeof(extended_memory_kb_text),
            1UL,
            line_no);
    }

    if (key_equals(key, "xms_kb") ||
        key_equals(key, "xms_memory_kb")) {
        return set_disk_cache_uint(
            key, value,
            R36SX_PICO286_MIN_XMS_KB,
            R36SX_PICO286_MAX_XMS_KB,
            &xms_memory_kb,
            xms_memory_kb_text,
            sizeof(xms_memory_kb_text),
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
    if (key_equals(key, "cpu_model") ||
        key_equals(key, "processor") ||
        key_equals(key, "processor_model")) {
        return set_cpu_model(value, line_no);
    }
    if (key_equals(key, "cpu_mode") ||
        key_equals(key, "processor_mode")) {
        return set_cpu_mode(value, line_no);
    }
    if (key_equals(key, "boot_mode")) {
        return set_boot_mode(value, line_no);
    }
    if (key_equals(key, "bios") ||
        key_equals(key, "bios_mode") ||
        key_equals(key, "bios_rom")) {
        return set_bios_mode_value(value, line_no);
    }
    if (key_equals(key, "test_bios_rom") ||
        key_equals(key, "test_bios_path") ||
        key_equals(key, "test386_bios_rom") ||
        key_equals(key, "test386_bios_path") ||
        key_equals(key, "bios_path")) {
        set_test_bios_value(value);
        r36sx_pico286_debug_log("diskcfg: test_bios_rom='%s'",
                                test_bios_path);
        return 1;
    }
    if (key_equals(key, "host_drive_path") ||
        key_equals(key, "host_path") ||
        key_equals(key, "host_dir") ||
        key_equals(key, "shared_dir") ||
        key_equals(key, "drive_h") ||
        key_equals(key, "drive_h_path")) {
        set_host_drive_value(value);
        r36sx_pico286_debug_log("diskcfg: host_drive_path='%s'",
                                host_drive_path);
        return 1;
    }
    if (key_equals(key, "image_dir") ||
        key_equals(key, "images_dir") ||
        key_equals(key, "image_path") ||
        key_equals(key, "images_path") ||
        key_equals(key, "disk_image_dir") ||
        key_equals(key, "disk_images_dir")) {
        set_image_dir_value(value);
        r36sx_pico286_debug_log("diskcfg: image_dir='%s'",
                                image_dir_path);
        return 1;
    }
    if (set_disk_cache_value(key, value, line_no)) {
        return 1;
    }
    if (set_profiling_value(key, value, line_no)) {
        return 1;
    }
    if (set_app_stats_value(key, value, line_no)) {
        return 1;
    }
    if (set_audio_value(key, value, line_no)) {
        return 1;
    }
    if (set_timing_value(key, value, line_no)) {
        return 1;
    }
    if (set_memory_value(key, value, line_no)) {
        return 1;
    }
    if (set_screenshot_format(key, value, line_no)) {
        return 1;
    }
    if (set_scaling_filter(key, value, line_no)) {
        return 1;
    }
    if (set_keyboard_mode(key, value, line_no)) {
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
        ensure_host_drive_dir();
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
    ensure_host_drive_dir();
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

const char *r36sx_pico286_image_dir_path(void)
{
    load_disk_config();

    return image_dir_path;
}

const char *r36sx_pico286_image_dir_value(void)
{
    load_disk_config();

    return image_dir_value;
}

const char *r36sx_pico286_config_dir(void)
{
    load_disk_config();

    return disk_config_dir;
}

const char *r36sx_pico286_host_drive_path(void)
{
    load_disk_config();

    return host_drive_path;
}

const char *r36sx_pico286_host_drive_value(void)
{
    load_disk_config();

    return host_drive_value;
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

    fprintf(fp, "# Pico-286 configuration for the R36SX native port.\n");
    fprintf(fp, "# Paths are relative to this directory unless an absolute path is used.\n");
    fprintf(fp, "# Disk drive values are file names loaded from image_dir.\n\n");

    fprintf(fp, "# CPU compatibility model: 8086, 80286, or 80386.\n");
    fprintf(fp, "# Higher-generation instructions are accepted only when the selected\n");
    fprintf(fp, "# model supports them; unsupported opcodes raise INT 6.\n");
    fprintf(fp, "# cpu_mode=real is the normal PC boot mode. cpu_mode=protected is\n");
    fprintf(fp, "# parsed for experiments; full protected-mode execution is still WIP.\n");
    fprintf(fp, "# CPU speed knob maps to exec86 loops per host scheduler slice.\n");
    fprintf(fp, "[cpu]\n");
    fprintf(fp, "cpu_model=%s\n", cpu_model_text);
    fprintf(fp, "cpu_mode=%s\n", cpu_mode_text);
    fprintf(fp, "cpu_mhz=%s\n\n", cpu_mhz_text);

    fprintf(fp, "# Main loop timing. target_fps sets the frame budget used by\n");
    fprintf(fp, "# adaptive exec86 quantum control and display pacing.\n");
    fprintf(fp, "[timing]\n");
    fprintf(fp, "target_fps=%s\n\n", target_fps_text);

    fprintf(fp, "# Scaling filter used when the DOS image is resized.\n");
    fprintf(fp, "# Supported scaling_filter values: nearest, bilinear.\n");
    fprintf(fp, "# keyboard_mode=normal resizes the DOS image above the keyboard.\n");
    fprintf(fp, "# keyboard_mode=overlay draws the keyboard over the DOS image.\n");
    fprintf(fp, "[video]\n");
    fprintf(fp, "scaling_filter=%s\n", scaling_filter_text);
    fprintf(fp, "keyboard_mode=%s\n\n", keyboard_mode_text);

    fprintf(fp, "# Emulated audio devices mixed into the output stream.\n");
    fprintf(fp, "# The built-in PC speaker/beeper is always enabled.\n");
    fprintf(fp, "# audio_sample_rate supports 44100 and 22050 Hz.\n");
    fprintf(fp, "[audio]\n");
    fprintf(fp, "audio_sample_rate=%s\n", audio_sample_rate_text);
    fprintf(fp, "audio_adlib=%s\n", audio_adlib_enabled_text);
    fprintf(fp, "audio_sound_blaster=%s\n", audio_sound_blaster_enabled_text);
    fprintf(fp, "audio_cms=%s\n", audio_cms_enabled_text);
    fprintf(fp, "audio_sn76489=%s\n", audio_sn76489_enabled_text);
    fprintf(fp, "audio_mpu401=%s\n", audio_mpu401_enabled_text);
    fprintf(fp, "audio_disney=%s\n", audio_disney_enabled_text);
    fprintf(fp, "audio_covox=%s\n\n", audio_covox_enabled_text);

    fprintf(fp, "# Boot behavior and boot-sector probe order.\n");
    fprintf(fp, "[boot]\n");
    fprintf(fp, "boot_mode=%s\n", boot_mode_text);
    fprintf(fp, "boot_order=%s\n\n", boot_order_text);

    fprintf(fp, "# BIOS ROM provider: normal or test386.\n");
    fprintf(fp, "# test_bios_rom is resolved relative to this config file.\n");
    fprintf(fp, "[bios]\n");
    fprintf(fp, "bios=%s\n", bios_mode_text);
    fprintf(fp, "test_bios_rom=%s\n\n", test_bios_value);

    fprintf(fp, "# Emulated memory sizes in KB, capped by the compiled buffers.\n");
    fprintf(fp, "# Allowed ranges:\n");
    fprintf(fp, "# conventional_kb: 64..640, reported through the BIOS Data Area.\n");
    fprintf(fp, "# upper_kb: 0..176, limits XMS UMB allocations from D000:0000 upward.\n");
    fprintf(fp, "# extended_kb: 0..4096, reported by INT 15h AH=88h.\n");
    fprintf(fp, "# xms_kb: 0..4096, limits the built-in XMS handler.\n");
    fprintf(fp, "[memory]\n");
    fprintf(fp, "conventional_kb=%s\n", conventional_memory_kb_text);
    fprintf(fp, "upper_kb=%s\n", upper_memory_kb_text);
    fprintf(fp, "extended_kb=%s\n", extended_memory_kb_text);
    fprintf(fp, "xms_kb=%s\n\n", xms_memory_kb_text);

    fprintf(fp, "# Screenshot output format: png or bmp.\n");
    fprintf(fp, "[screenshot]\n");
    fprintf(fp, "screenshot_format=%s\n\n", screenshot_format_text);

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

    fprintf(fp, "# On-screen runtime statistics, toggled with Fn+D-pad Down.\n");
    fprintf(fp, "[stats]\n");
    fprintf(fp, "app_stats_enabled=%s\n\n", app_stats_enabled_text);

    fprintf(fp, "# Host filesystem directory exposed to DOS as network drive H:.\n");
    fprintf(fp, "# Relative paths are resolved next to pico_286.conf.\n");
    fprintf(fp, "# DOS must run MAPDRIVE.COM after boot; CONFIG.SYS needs LASTDRIVE=H or higher.\n");
    fprintf(fp, "[host_drive]\n");
    fprintf(fp, "host_drive_path=%s\n\n", host_drive_value);

    fprintf(fp, "# Directory that contains .img files used by short file names below.\n");
    fprintf(fp, "# Relative paths are resolved next to pico_286.conf.\n");
    fprintf(fp, "[disk_images]\n");
    fprintf(fp, "image_dir=%s\n\n", image_dir_value);

    fprintf(fp, "# BIOS floppy drives 00h and 01h, DOS A: and B:.\n");
    fprintf(fp, "[floppy_drives]\n");
    fprintf(fp, "fdd0=%s\n", disk_entries[0].value);
    fprintf(fp, "fdd1=%s\n\n", disk_entries[1].value);

    fprintf(fp, "# BIOS hard drives 80h and 81h, DOS C: and D:.\n");
    fprintf(fp, "[hard_drives]\n");
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

uint32_t r36sx_pico286_target_fps(uint32_t fallback_fps)
{
    load_disk_config();

    return target_fps ? target_fps : fallback_fps;
}

r36sx_pico286_cpu_model_t r36sx_pico286_cpu_model(void)
{
    load_disk_config();

    return cpu_model;
}

int r36sx_pico286_cpu_model_at_least(r36sx_pico286_cpu_model_t model)
{
    load_disk_config();

    return cpu_model >= model;
}

const char *r36sx_pico286_cpu_model_name(void)
{
    load_disk_config();

    return cpu_model_text;
}

r36sx_pico286_cpu_mode_t r36sx_pico286_cpu_mode(void)
{
    load_disk_config();

    return cpu_mode;
}

const char *r36sx_pico286_cpu_mode_name(void)
{
    load_disk_config();

    return cpu_mode_text;
}

int r36sx_pico286_boot_bios_prompt(void)
{
    load_disk_config();

    return boot_bios_prompt;
}

r36sx_pico286_bios_mode_t r36sx_pico286_bios_mode(void)
{
    load_disk_config();

    return bios_mode;
}

const char *r36sx_pico286_bios_mode_name(void)
{
    load_disk_config();

    return bios_mode_text;
}

int r36sx_pico286_set_bios_mode(r36sx_pico286_bios_mode_t mode)
{
    load_disk_config();

    if (mode == R36SX_PICO286_BIOS_TEST386) {
        return set_bios_mode_value("test386", 0);
    }
    return set_bios_mode_value("normal", 0);
}

const char *r36sx_pico286_test_bios_path(void)
{
    load_disk_config();

    return test_bios_path;
}

const char *r36sx_pico286_test_bios_value(void)
{
    load_disk_config();

    return test_bios_value;
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

int r36sx_pico286_app_stats_enabled(void)
{
    load_disk_config();

    return app_stats_enabled;
}

int r36sx_pico286_audio_adlib_enabled(void)
{
    load_disk_config();

    return audio_adlib_enabled;
}

int r36sx_pico286_audio_sound_blaster_enabled(void)
{
    load_disk_config();

    return audio_sound_blaster_enabled;
}

int r36sx_pico286_audio_cms_enabled(void)
{
    load_disk_config();

    return audio_cms_enabled;
}

int r36sx_pico286_audio_sn76489_enabled(void)
{
    load_disk_config();

    return audio_sn76489_enabled;
}

int r36sx_pico286_audio_mpu401_enabled(void)
{
    load_disk_config();

    return audio_mpu401_enabled;
}

int r36sx_pico286_audio_disney_enabled(void)
{
    load_disk_config();

    return audio_disney_enabled;
}

int r36sx_pico286_audio_covox_enabled(void)
{
    load_disk_config();

    return audio_covox_enabled;
}

uint32_t r36sx_pico286_audio_sample_rate(uint32_t fallback_rate)
{
    load_disk_config();

    return audio_sample_rate ? audio_sample_rate : fallback_rate;
}

uint32_t r36sx_pico286_conventional_memory_kb(void)
{
    load_disk_config();

    return conventional_memory_kb;
}

uint32_t r36sx_pico286_upper_memory_kb(void)
{
    load_disk_config();

    return upper_memory_kb;
}

uint32_t r36sx_pico286_extended_memory_kb(void)
{
    load_disk_config();

    return extended_memory_kb;
}

uint32_t r36sx_pico286_xms_memory_kb(void)
{
    load_disk_config();

    return xms_memory_kb;
}

r36sx_pico286_screenshot_format_t r36sx_pico286_screenshot_format(void)
{
    load_disk_config();

    return screenshot_format;
}

const char *r36sx_pico286_screenshot_format_name(void)
{
    load_disk_config();

    return screenshot_format_text;
}

r36sx_pico286_scaling_filter_t r36sx_pico286_scaling_filter(void)
{
    load_disk_config();

    return scaling_filter;
}

const char *r36sx_pico286_scaling_filter_name(void)
{
    load_disk_config();

    return scaling_filter_text;
}

r36sx_pico286_keyboard_mode_t r36sx_pico286_keyboard_mode(void)
{
    load_disk_config();

    return keyboard_mode;
}

const char *r36sx_pico286_keyboard_mode_name(void)
{
    load_disk_config();

    return keyboard_mode_text;
}
