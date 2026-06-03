/*
 * Protected-mode memory model for the R36SX x86 interpreter.
 *
 * This file is included by r36sx_cpu.c and intentionally has no include guard.
 * The helpers keep descriptor limit/access checks, VM86 handling, and paging
 * translation on the protected-mode path.
 */

static inline uint8_t r36sx_cpu_protected_getmem8(uint16_t selector,
                                                  uint32_t offset)
{
    return r36sx_cpu_getmem8_checked(selector, offset);
}

static inline uint16_t r36sx_cpu_protected_getmem16(uint16_t selector,
                                                    uint32_t offset)
{
    return r36sx_cpu_getmem16_checked(selector, offset);
}

static inline uint32_t r36sx_cpu_protected_getmem32(uint16_t selector,
                                                    uint32_t offset)
{
    return r36sx_cpu_getmem32_checked(selector, offset);
}

static inline void r36sx_cpu_protected_putmem8(uint16_t selector,
                                               uint32_t offset,
                                               uint8_t value)
{
    r36sx_cpu_putmem8_checked(selector, offset, value);
}

static inline void r36sx_cpu_protected_putmem16(uint16_t selector,
                                                uint32_t offset,
                                                uint16_t value)
{
    r36sx_cpu_putmem16_checked(selector, offset, value);
}

static inline void r36sx_cpu_protected_putmem32(uint16_t selector,
                                                uint32_t offset,
                                                uint32_t value)
{
    r36sx_cpu_putmem32_checked(selector, offset, value);
}

static inline uint8_t r36sx_cpu_protected_read8(uint32_t linear)
{
    return r36sx_cpu_read_linear8(linear);
}

static inline uint16_t r36sx_cpu_protected_read16(uint32_t linear)
{
    return r36sx_cpu_read_linear16(linear);
}

static inline uint32_t r36sx_cpu_protected_read32(uint32_t linear)
{
    return r36sx_cpu_read_linear32(linear);
}

static inline void r36sx_cpu_protected_write8(uint32_t linear, uint8_t value)
{
    r36sx_cpu_write_linear8(linear, value);
}

static inline void r36sx_cpu_protected_write16(uint32_t linear, uint16_t value)
{
    r36sx_cpu_write_linear16(linear, value);
}

static inline void r36sx_cpu_protected_write32(uint32_t linear, uint32_t value)
{
    r36sx_cpu_write_linear32(linear, value);
}
