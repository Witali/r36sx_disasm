#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "r36sx_screenshot_png.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void r36sx_png_put_be32(uint8_t *dst, uint32_t value)
{
    dst[0] = (uint8_t)((value >> 24) & 0xffu);
    dst[1] = (uint8_t)((value >> 16) & 0xffu);
    dst[2] = (uint8_t)((value >> 8) & 0xffu);
    dst[3] = (uint8_t)(value & 0xffu);
}

static uint8_t r36sx_png_rgb565_to_r8(uint16_t color)
{
    uint8_t r = (uint8_t)((color >> 11) & 0x1fu);
    return (uint8_t)((r << 3) | (r >> 2));
}

static uint8_t r36sx_png_rgb565_to_g8(uint16_t color)
{
    uint8_t g = (uint8_t)((color >> 5) & 0x3fu);
    return (uint8_t)((g << 2) | (g >> 4));
}

static uint8_t r36sx_png_rgb565_to_b8(uint16_t color)
{
    uint8_t b = (uint8_t)(color & 0x1fu);
    return (uint8_t)((b << 3) | (b >> 2));
}

static void r36sx_png_rgb565_to_rgb24(uint8_t *dst,
                                      const uint16_t *src,
                                      size_t count)
{
    for (size_t i = 0; i < count; i++) {
        uint16_t color = src[i];

        dst[i * 3u + 0u] = r36sx_png_rgb565_to_r8(color);
        dst[i * 3u + 1u] = r36sx_png_rgb565_to_g8(color);
        dst[i * 3u + 2u] = r36sx_png_rgb565_to_b8(color);
    }
}

static int r36sx_png_write_chunk(FILE *fp,
                                 const char type[4],
                                 const uint8_t *data,
                                 uint32_t length)
{
    uint8_t header[8];
    uint8_t crc_bytes[4];
    uLong crc;

    if (!fp || !type) {
        return -1;
    }

    r36sx_png_put_be32(&header[0], length);
    memcpy(&header[4], type, 4);
    if (fwrite(header, 1u, sizeof(header), fp) != sizeof(header)) {
        return -1;
    }
    if (length > 0u && (!data || fwrite(data, 1u, length, fp) != length)) {
        return -1;
    }

    crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, &header[4], 4);
    if (length > 0u) {
        crc = crc32(crc, data, length);
    }
    r36sx_png_put_be32(crc_bytes, (uint32_t)crc);
    return fwrite(crc_bytes, 1u, sizeof(crc_bytes), fp) == sizeof(crc_bytes) ?
        0 : -1;
}

int r36sx_screenshot_png_write_rgb565(const char *path,
                                      const uint16_t *pixels,
                                      int width,
                                      int height)
{
    FILE *fp;
    uint8_t ihdr[13];
    uint8_t *raw;
    uint8_t *compressed;
    uint32_t row_bytes;
    uint32_t raw_bytes;
    uLongf compressed_bytes;
    int rc = -1;
    static const uint8_t png_signature[8] = {
        0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'
    };

    if (!path || !pixels || width <= 0 || height <= 0) {
        return -1;
    }

    row_bytes = (uint32_t)width * 3u;
    raw_bytes = (row_bytes + 1u) * (uint32_t)height;
    if (row_bytes / 3u != (uint32_t)width ||
        raw_bytes / (row_bytes + 1u) != (uint32_t)height) {
        return -1;
    }

    raw = (uint8_t *)malloc(raw_bytes);
    if (!raw) {
        return -1;
    }

    for (int y = 0; y < height; y++) {
        uint8_t *row = raw + (size_t)y * (size_t)(row_bytes + 1u);
        const uint16_t *src = pixels + (size_t)y * (size_t)width;

        row[0] = 0; /* PNG filter type: none. */
        r36sx_png_rgb565_to_rgb24(&row[1], src, (size_t)width);
    }

    compressed_bytes = compressBound(raw_bytes);
    compressed = (uint8_t *)malloc(compressed_bytes);
    if (!compressed) {
        free(raw);
        return -1;
    }
    if (compress2(compressed, &compressed_bytes, raw, raw_bytes,
                  Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(compressed);
        free(raw);
        return -1;
    }

    fp = fopen(path, "wb");
    if (!fp) {
        free(compressed);
        free(raw);
        return -1;
    }

    r36sx_png_put_be32(&ihdr[0], (uint32_t)width);
    r36sx_png_put_be32(&ihdr[4], (uint32_t)height);
    ihdr[8] = 8;  /* bit depth */
    ihdr[9] = 2;  /* truecolor RGB */
    ihdr[10] = 0; /* deflate compression */
    ihdr[11] = 0; /* adaptive filtering */
    ihdr[12] = 0; /* no interlace */

    if (fwrite(png_signature, 1u, sizeof(png_signature), fp) ==
            sizeof(png_signature) &&
        r36sx_png_write_chunk(fp, "IHDR", ihdr, sizeof(ihdr)) == 0 &&
        r36sx_png_write_chunk(fp, "IDAT", compressed,
                              (uint32_t)compressed_bytes) == 0 &&
        r36sx_png_write_chunk(fp, "IEND", NULL, 0) == 0) {
        rc = 0;
    }

    if (fclose(fp) != 0) {
        rc = -1;
    }

    free(compressed);
    free(raw);
    return rc;
}
