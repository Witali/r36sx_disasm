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
#endif

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
