#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "r36sx_screenshot.h"
#include "hardware.h"
#include "r36sx_screenshot_png.h"

#include <ctype.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static void r36sx_screenshot_put_le16(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)((value >> 8) & 0xffu);
}

static void r36sx_screenshot_put_le32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)(value & 0xffu);
    dst[1] = (uint8_t)((value >> 8) & 0xffu);
    dst[2] = (uint8_t)((value >> 16) & 0xffu);
    dst[3] = (uint8_t)((value >> 24) & 0xffu);
}

static uint8_t r36sx_screenshot_rgb565_to_r8(uint16_t color)
{
    uint8_t r = (uint8_t)((color >> 11) & 0x1fu);
    return (uint8_t)((r << 3) | (r >> 2));
}

static uint8_t r36sx_screenshot_rgb565_to_g8(uint16_t color)
{
    uint8_t g = (uint8_t)((color >> 5) & 0x3fu);
    return (uint8_t)((g << 2) | (g >> 4));
}

static uint8_t r36sx_screenshot_rgb565_to_b8(uint16_t color)
{
    uint8_t b = (uint8_t)(color & 0x1fu);
    return (uint8_t)((b << 3) | (b >> 2));
}

static void r36sx_screenshot_rgb565_to_bgr24(uint8_t *dst,
                                             const uint16_t *src,
                                             size_t count)
{
    for (size_t i = 0; i < count; i++) {
        uint16_t color = src[i];

        dst[i * 3u + 0u] = r36sx_screenshot_rgb565_to_b8(color);
        dst[i * 3u + 1u] = r36sx_screenshot_rgb565_to_g8(color);
        dst[i * 3u + 2u] = r36sx_screenshot_rgb565_to_r8(color);
    }
}

static int r36sx_screenshot_write_bmp24(const char *path,
                                        const uint16_t *pixels,
                                        int width,
                                        int height)
{
    FILE *fp;
    uint8_t header[54];
    uint8_t *row;
    size_t row_bytes;
    size_t padded_row_bytes;
    size_t pixel_bytes;
    int ok;

    if (!path || !pixels || width <= 0 || height <= 0) {
        return -1;
    }

    row_bytes = (size_t)width * 3u;
    padded_row_bytes = (row_bytes + 3u) & ~(size_t)3u;
    pixel_bytes = padded_row_bytes * (size_t)height;
    if (row_bytes / 3u != (size_t)width ||
        padded_row_bytes < row_bytes ||
        pixel_bytes / padded_row_bytes != (size_t)height ||
        pixel_bytes + sizeof(header) > UINT32_MAX) {
        return -1;
    }

    fp = fopen(path, "wb");
    if (!fp) {
        return -1;
    }

    memset(header, 0, sizeof(header));
    header[0] = 'B';
    header[1] = 'M';
    r36sx_screenshot_put_le32(&header[2],
                              (uint32_t)(sizeof(header) + pixel_bytes));
    r36sx_screenshot_put_le32(&header[10], (uint32_t)sizeof(header));
    r36sx_screenshot_put_le32(&header[14], 40u);
    r36sx_screenshot_put_le32(&header[18], (uint32_t)width);
    r36sx_screenshot_put_le32(&header[22], (uint32_t)height);
    r36sx_screenshot_put_le16(&header[26], 1u);
    r36sx_screenshot_put_le16(&header[28], 24u);
    r36sx_screenshot_put_le32(&header[34], (uint32_t)pixel_bytes);

    row = (uint8_t *)calloc(1u, padded_row_bytes);
    if (!row) {
        fclose(fp);
        return -1;
    }

    ok = fwrite(header, 1u, sizeof(header), fp) == sizeof(header);
    for (int y = height - 1; ok && y >= 0; y--) {
        const uint16_t *src = pixels + (size_t)y * (size_t)width;

        memset(row, 0, padded_row_bytes);
        r36sx_screenshot_rgb565_to_bgr24(row, src, (size_t)width);
        ok = fwrite(row, 1u, padded_row_bytes, fp) == padded_row_bytes;
    }

    free(row);
    if (fclose(fp) != 0) {
        ok = 0;
    }
    return ok ? 0 : -1;
}

static r36sx_screenshot_png_write_rgb565_fn r36sx_screenshot_load_png_writer(
    void)
{
    static int attempted;
    static void *module;
    static r36sx_screenshot_png_write_rgb565_fn write_png;
    const char *env_path;
    const char *paths[4];

    if (attempted) {
        return write_png;
    }
    attempted = 1;

    env_path = getenv("R36SX_SCREENSHOT_PNG_SO");
    if (env_path && env_path[0]) {
        module = dlopen(env_path, RTLD_NOW);
        if (module) {
            write_png = (r36sx_screenshot_png_write_rgb565_fn)dlsym(
                module, R36SX_SCREENSHOT_PNG_WRITE_RGB565_SYMBOL);
            if (write_png) {
                return write_png;
            }
            dlclose(module);
            module = NULL;
        }
    }

    paths[0] = R36SX_SCREENSHOT_PNG_SO_PATH;
    paths[1] = R36SX_SCREENSHOT_PNG_SO_LOCAL_PATH;
    paths[2] = R36SX_SCREENSHOT_PNG_SO_NAME;
    paths[3] = NULL;

    for (size_t i = 0; paths[i]; i++) {
        const char *path = paths[i];

        module = dlopen(path, RTLD_NOW);
        if (!module) {
            continue;
        }
        write_png = (r36sx_screenshot_png_write_rgb565_fn)dlsym(
            module, R36SX_SCREENSHOT_PNG_WRITE_RGB565_SYMBOL);
        if (write_png) {
            return write_png;
        }

        dlclose(module);
        module = NULL;
    }

    return NULL;
}

static int r36sx_screenshot_write_png_file(const char *path,
                                           const uint16_t *pixels,
                                           int width,
                                           int height)
{
    r36sx_screenshot_png_write_rgb565_fn write_png =
        r36sx_screenshot_load_png_writer();

    if (!write_png) {
        return -1;
    }
    return write_png(path, pixels, width, height);
}

int r36sx_screenshot_write_rgb565_file(const char *path,
                                       const uint16_t *pixels,
                                       int width,
                                       int height,
                                       r36sx_screenshot_format_t format)
{
    if (format == R36SX_SCREENSHOT_FORMAT_PNG) {
        return r36sx_screenshot_write_png_file(path, pixels, width, height);
    }
    return r36sx_screenshot_write_bmp24(path, pixels, width, height);
}

static int r36sx_screenshot_build_hash8(const char *hash,
                                        int include_build_hash,
                                        char *dst,
                                        size_t dst_size)
{
    if (!include_build_hash || !hash || !dst || dst_size < 9u ||
        !hash[0] || strcmp(hash, "unknown") == 0) {
        return 0;
    }

    for (size_t i = 0; i < 8u; i++) {
        unsigned char ch = (unsigned char)hash[i];

        if (!isxdigit(ch)) {
            return 0;
        }
        dst[i] = (char)tolower(ch);
    }
    dst[8] = '\0';
    return 1;
}

static int r36sx_screenshot_save_to_dir(
    const r36sx_screenshot_options_t *options,
    const char *dir,
    const uint16_t *pixels,
    int width,
    int height,
    char *saved_path,
    size_t saved_path_size)
{
    struct tm tm_value;
    time_t timestamp;
    char stamp[32];
    char build_hash[9];
    char path[512];
    const char *prefix = options->prefix ? options->prefix : "screenshot";
    const char *ext = options->format == R36SX_SCREENSHOT_FORMAT_PNG ?
        "png" : "bmp";
    int include_hash;
    int written;

    if (!dir || !dir[0] || !pixels) {
        return -1;
    }

    (void)mkdir(dir, 0755);
    timestamp = (time_t)options->unix_time;
    if (timestamp != (time_t)-1 &&
        localtime_r(&timestamp, &tm_value) != NULL) {
        strftime(stamp, sizeof(stamp), "%Y%m%d_%H%M%S", &tm_value);
    } else {
        snprintf(stamp, sizeof(stamp), "unknown_time");
    }

    include_hash = r36sx_screenshot_build_hash8(
        options->build_hash_sha256, options->include_build_hash,
        build_hash, sizeof(build_hash));
    if (include_hash) {
        written = snprintf(path, sizeof(path), "%s/%s_%s_%s_%03u.%s",
                           dir, prefix, stamp, build_hash,
                           (unsigned)(options->sequence % 1000u), ext);
    } else {
        written = snprintf(path, sizeof(path), "%s/%s_%s_%03u.%s",
                           dir, prefix, stamp,
                           (unsigned)(options->sequence % 1000u), ext);
    }
    if (written < 0 || (size_t)written >= sizeof(path)) {
        return -1;
    }

    if (r36sx_screenshot_write_rgb565_file(path, pixels, width, height,
                                           options->format) != 0) {
        return -1;
    }

    if (saved_path && saved_path_size > 0u) {
        snprintf(saved_path, saved_path_size, "%s", path);
    }
    return 0;
}

int r36sx_screenshot_save_rgb565(const r36sx_screenshot_options_t *options,
                                 const uint16_t *pixels,
                                 int width,
                                 int height,
                                 char *saved_path,
                                 size_t saved_path_size)
{
    if (!options || !pixels || width <= 0 || height <= 0) {
        return -1;
    }

    if (r36sx_screenshot_save_to_dir(options, options->primary_dir, pixels,
                                     width, height, saved_path,
                                     saved_path_size) == 0) {
        return 0;
    }
    if (options->fallback_dir &&
        r36sx_screenshot_save_to_dir(options, options->fallback_dir, pixels,
                                     width, height, saved_path,
                                     saved_path_size) == 0) {
        return 0;
    }

    return -1;
}
