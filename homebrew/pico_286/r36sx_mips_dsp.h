#ifndef R36SX_MIPS_DSP_H
#define R36SX_MIPS_DSP_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifndef R36SX_MIPS_DSP
#define R36SX_MIPS_DSP 0
#endif

#if R36SX_MIPS_DSP && defined(__mips_dspr2)
#define R36SX_MIPS_DSP_ENABLED 1
#else
#define R36SX_MIPS_DSP_ENABLED 0
#endif

#ifdef __cplusplus
extern "C" {
#endif
uint32_t r36sx_mips_dsp_probe(uint32_t value);
#ifdef __cplusplus
}
#endif

#if R36SX_MIPS_DSP_ENABLED
static inline uint32_t r36sx_mips_dsp_addu_ph_passthrough(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("addu.ph %0,%1,$zero"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shrl_ph_2(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shrl.ph %0,%1,2"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shrl_ph_3(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shrl.ph %0,%1,3"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shrl_ph_8(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shrl.ph %0,%1,8"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shrl_ph_9(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shrl.ph %0,%1,9"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shrl_ph_13(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shrl.ph %0,%1,13"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}

static inline uint32_t r36sx_mips_dsp_shll_ph_3(uint32_t packed)
{
    uint32_t result;

    __asm__ volatile ("shll.ph %0,%1,3"
                      : "=r"(result)
                      : "r"(packed));
    return result;
}
#endif

static inline uint8_t r36sx_mips_rgb565_to_r8(uint16_t color)
{
    uint8_t r = (uint8_t)((color >> 11) & 0x1fu);
    return (uint8_t)((r << 3) | (r >> 2));
}

static inline uint8_t r36sx_mips_rgb565_to_g8(uint16_t color)
{
    uint8_t g = (uint8_t)((color >> 5) & 0x3fu);
    return (uint8_t)((g << 2) | (g >> 4));
}

static inline uint8_t r36sx_mips_rgb565_to_b8(uint16_t color)
{
    uint8_t b = (uint8_t)(color & 0x1fu);
    return (uint8_t)((b << 3) | (b >> 2));
}

static inline uint16_t r36sx_mips_rgb888_to_rgb565(uint32_t color)
{
    return (uint16_t)(((color >> 8) & 0xf800u) |
                      ((color >> 5) & 0x07e0u) |
                      ((color >> 3) & 0x001fu));
}

#if R36SX_MIPS_DSP_ENABLED
static inline uint32_t r36sx_mips_dsp_rgb888_pair_to_rgb565(uint32_t color0,
                                                            uint32_t color1)
{
    uint32_t rg_pair = ((color0 >> 8) & 0x0000ffffu) |
                       ((color1 << 8) & 0xffff0000u);
    uint32_t b_pair = (color0 & 0x000000ffu) |
                      ((color1 & 0x000000ffu) << 16);
    uint32_t r = rg_pair & 0xf800f800u;
    uint32_t g = r36sx_mips_dsp_shll_ph_3(rg_pair) & 0x07e007e0u;
    uint32_t b = r36sx_mips_dsp_shrl_ph_3(b_pair) & 0x001f001fu;

    return r | g | b;
}

static inline void r36sx_mips_dsp_rgb565_pair_to_rgb24(uint8_t *dst,
                                                       uint32_t packed)
{
    uint32_t r = (r36sx_mips_dsp_shrl_ph_8(packed) & 0x00f800f8u) |
                 (r36sx_mips_dsp_shrl_ph_13(packed) & 0x00070007u);
    uint32_t g = (r36sx_mips_dsp_shrl_ph_3(packed) & 0x00fc00fcu) |
                 (r36sx_mips_dsp_shrl_ph_9(packed) & 0x00030003u);
    uint32_t b = (r36sx_mips_dsp_shll_ph_3(packed) & 0x00f800f8u) |
                 (r36sx_mips_dsp_shrl_ph_2(packed) & 0x00070007u);

    dst[0] = (uint8_t)r;
    dst[1] = (uint8_t)g;
    dst[2] = (uint8_t)b;
    dst[3] = (uint8_t)(r >> 16);
    dst[4] = (uint8_t)(g >> 16);
    dst[5] = (uint8_t)(b >> 16);
}

static inline void r36sx_mips_dsp_rgb565_pair_to_bgr24(uint8_t *dst,
                                                       uint32_t packed)
{
    uint32_t r = (r36sx_mips_dsp_shrl_ph_8(packed) & 0x00f800f8u) |
                 (r36sx_mips_dsp_shrl_ph_13(packed) & 0x00070007u);
    uint32_t g = (r36sx_mips_dsp_shrl_ph_3(packed) & 0x00fc00fcu) |
                 (r36sx_mips_dsp_shrl_ph_9(packed) & 0x00030003u);
    uint32_t b = (r36sx_mips_dsp_shll_ph_3(packed) & 0x00f800f8u) |
                 (r36sx_mips_dsp_shrl_ph_2(packed) & 0x00070007u);

    dst[0] = (uint8_t)b;
    dst[1] = (uint8_t)g;
    dst[2] = (uint8_t)r;
    dst[3] = (uint8_t)(b >> 16);
    dst[4] = (uint8_t)(g >> 16);
    dst[5] = (uint8_t)(r >> 16);
}
#endif

static inline void r36sx_mips_dsp_rgb888_to_rgb565(uint16_t *dst,
                                                   const uint32_t *src,
                                                   size_t count)
{
    if (!dst || !src || count == 0) {
        return;
    }

#if R36SX_MIPS_DSP_ENABLED
    while (count > 0 && (((uintptr_t)dst) & (uintptr_t)3u) != 0) {
        *dst++ = r36sx_mips_rgb888_to_rgb565(*src++);
        count--;
    }

    while (count >= 2) {
        *(uint32_t *)dst =
            r36sx_mips_dsp_rgb888_pair_to_rgb565(src[0], src[1]);
        dst += 2;
        src += 2;
        count -= 2;
    }
#endif

    while (count > 0) {
        *dst++ = r36sx_mips_rgb888_to_rgb565(*src++);
        count--;
    }
}

static inline void r36sx_mips_dsp_rgb565_to_rgb24(uint8_t *dst,
                                                  const uint16_t *src,
                                                  size_t count)
{
    if (!dst || !src || count == 0) {
        return;
    }

#if R36SX_MIPS_DSP_ENABLED
    while (count > 0 && (((uintptr_t)src) & (uintptr_t)3u) != 0) {
        uint16_t color = *src++;

        dst[0] = r36sx_mips_rgb565_to_r8(color);
        dst[1] = r36sx_mips_rgb565_to_g8(color);
        dst[2] = r36sx_mips_rgb565_to_b8(color);
        dst += 3;
        count--;
    }

    while (count >= 2) {
        r36sx_mips_dsp_rgb565_pair_to_rgb24(
            dst, *(const uint32_t *)src);
        src += 2;
        dst += 6;
        count -= 2;
    }
#endif

    while (count > 0) {
        uint16_t color = *src++;

        dst[0] = r36sx_mips_rgb565_to_r8(color);
        dst[1] = r36sx_mips_rgb565_to_g8(color);
        dst[2] = r36sx_mips_rgb565_to_b8(color);
        dst += 3;
        count--;
    }
}

static inline void r36sx_mips_dsp_rgb565_to_bgr24(uint8_t *dst,
                                                  const uint16_t *src,
                                                  size_t count)
{
    if (!dst || !src || count == 0) {
        return;
    }

#if R36SX_MIPS_DSP_ENABLED
    while (count > 0 && (((uintptr_t)src) & (uintptr_t)3u) != 0) {
        uint16_t color = *src++;

        dst[0] = r36sx_mips_rgb565_to_b8(color);
        dst[1] = r36sx_mips_rgb565_to_g8(color);
        dst[2] = r36sx_mips_rgb565_to_r8(color);
        dst += 3;
        count--;
    }

    while (count >= 2) {
        r36sx_mips_dsp_rgb565_pair_to_bgr24(
            dst, *(const uint32_t *)src);
        src += 2;
        dst += 6;
        count -= 2;
    }
#endif

    while (count > 0) {
        uint16_t color = *src++;

        dst[0] = r36sx_mips_rgb565_to_b8(color);
        dst[1] = r36sx_mips_rgb565_to_g8(color);
        dst[2] = r36sx_mips_rgb565_to_r8(color);
        dst += 3;
        count--;
    }
}

static inline void r36sx_mips_dsp_copy_u16(uint16_t *dst,
                                           const uint16_t *src,
                                           size_t count)
{
    if (!dst || !src || count == 0) {
        return;
    }

#if R36SX_MIPS_DSP_ENABLED
    while (count > 0 &&
           (((uintptr_t)dst | (uintptr_t)src) & (uintptr_t)3u) != 0) {
        *dst++ = *src++;
        count--;
    }

    if (count >= 2) {
        uint32_t *dst32 = (uint32_t *)dst;
        const uint32_t *src32 = (const uint32_t *)src;
        size_t pairs = count >> 1;

        for (size_t i = 0; i < pairs; i++) {
            dst32[i] = r36sx_mips_dsp_addu_ph_passthrough(src32[i]);
        }

        dst += pairs << 1;
        src += pairs << 1;
        count &= 1u;
    }

    if (count != 0) {
        *dst = *src;
    }
#else
    memcpy(dst, src, count * sizeof(dst[0]));
#endif
}

static inline void r36sx_mips_dsp_fill_u16(uint16_t *dst, uint16_t value,
                                           size_t count)
{
    if (!dst || count == 0) {
        return;
    }

#if R36SX_MIPS_DSP_ENABLED
    while (count > 0 && (((uintptr_t)dst) & (uintptr_t)3u) != 0) {
        *dst++ = value;
        count--;
    }

    if (count >= 2) {
        uint32_t packed =
            r36sx_mips_dsp_addu_ph_passthrough(
                ((uint32_t)value << 16) | (uint32_t)value);
        uint32_t *dst32 = (uint32_t *)dst;
        size_t pairs = count >> 1;

        for (size_t i = 0; i < pairs; i++) {
            dst32[i] = packed;
        }

        dst += pairs << 1;
        count &= 1u;
    }

    if (count != 0) {
        *dst = value;
    }
#else
    for (size_t i = 0; i < count; i++) {
        dst[i] = value;
    }
#endif
}

static inline void r36sx_mips_dsp_zero_u16(uint16_t *dst, size_t count)
{
    r36sx_mips_dsp_fill_u16(dst, 0, count);
}

#endif
