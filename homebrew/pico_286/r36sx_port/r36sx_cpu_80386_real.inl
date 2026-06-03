/*
 * Real-mode memory model for the R36SX x86 interpreter.
 *
 * This file is included by r36sx_cpu.c and intentionally has no include guard.
 * Real mode has no descriptor limits, privilege checks, or paging, so the hot
 * path can translate segment:offset as seg << 4 and access the physical memory
 * backend directly.
 */

static inline uint32_t r36sx_cpu_real_linear(uint16_t selector,
                                             uint32_t offset)
{
    return ((uint32_t)selector << 4) + offset;
}

static inline uint8_t r36sx_cpu_real_read8(uint32_t linear)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    return r36sx_read86_fast(linear);
#else
    return read86_ob(linear);
#endif
}

static inline uint16_t r36sx_cpu_real_read16(uint32_t linear)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    return r36sx_readw86_fast(linear);
#else
    return readw86_ob(linear);
#endif
}

static inline uint32_t r36sx_cpu_real_read32(uint32_t linear)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    return r36sx_readdw86_fast(linear);
#else
    return readdw86_ob(linear);
#endif
}

static inline void r36sx_cpu_real_write8(uint32_t linear, uint8_t value)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    r36sx_write86_fast(linear, value);
#else
    write86_ob(linear, value);
#endif
}

static inline void r36sx_cpu_real_write16(uint32_t linear, uint16_t value)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    r36sx_writew86_fast(linear, value);
#else
    writew86_ob(linear, value);
#endif
}

static inline void r36sx_cpu_real_write32(uint32_t linear, uint32_t value)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    r36sx_writedw86_fast(linear, value);
#else
    writedw86_ob(linear, value);
#endif
}

static inline uint8_t r36sx_cpu_real_getmem8(uint16_t selector,
                                             uint32_t offset)
{
    return r36sx_cpu_real_read8(r36sx_cpu_real_linear(selector, offset));
}

static inline uint16_t r36sx_cpu_real_getmem16(uint16_t selector,
                                               uint32_t offset)
{
    return r36sx_cpu_real_read16(r36sx_cpu_real_linear(selector, offset));
}

static inline uint32_t r36sx_cpu_real_getmem32(uint16_t selector,
                                               uint32_t offset)
{
    return r36sx_cpu_real_read32(r36sx_cpu_real_linear(selector, offset));
}

static inline void r36sx_cpu_real_putmem8(uint16_t selector,
                                          uint32_t offset,
                                          uint8_t value)
{
    r36sx_cpu_real_write8(r36sx_cpu_real_linear(selector, offset), value);
}

static inline void r36sx_cpu_real_putmem16(uint16_t selector,
                                           uint32_t offset,
                                           uint16_t value)
{
    r36sx_cpu_real_write16(r36sx_cpu_real_linear(selector, offset), value);
}

static inline void r36sx_cpu_real_putmem32(uint16_t selector,
                                           uint32_t offset,
                                           uint32_t value)
{
    r36sx_cpu_real_write32(r36sx_cpu_real_linear(selector, offset), value);
}
