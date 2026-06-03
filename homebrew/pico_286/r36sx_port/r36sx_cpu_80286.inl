/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

static inline uint8_t r36sx_cpu_protected_enabled(void)
{
#if R36SX_ENABLE_PROTECTED_MODE
    return (r36sx_cr0 & R36SX_CR0_PE) != 0;
#else
    return 0;
#endif
}

static inline void r36sx_cpu_real_cache_segment(uint8_t segid)
{
    uint16_t selector = getsegreg(segid);
    uint32_t base = (uint32_t)selector << 4;
    segselector16[segid] = selector;
    r36sx_seg_cache[segid].selector = selector;
    r36sx_seg_cache[segid].base = base;
    r36sx_seg_cache[segid].limit = 0xffffu;
    r36sx_seg_cache[segid].access =
        R36SX_DESCRIPTOR_PRESENT | R36SX_DESCRIPTOR_CODE_DATA |
        R36SX_DESCRIPTOR_READABLE | R36SX_DESCRIPTOR_WRITABLE;
    r36sx_seg_cache[segid].flags = 0;
    r36sx_seg_cache[segid].valid = 1;
    segbase32[segid] = base;
}

static inline void r36sx_cpu_real_cache_all_segments(void)
{
    for (uint8_t segid = reges; segid <= reggs; segid++) {
        r36sx_cpu_real_cache_segment(segid);
    }
}

static inline uint8_t r36sx_descriptor_type(
    const r36sx_segment_cache_t *cache)
{
    return cache->access & R36SX_DESCRIPTOR_SYSTEM_TYPE_MASK;
}

static inline uint8_t r36sx_descriptor_is_code_data(
    const r36sx_segment_cache_t *cache)
{
    return (cache->access & R36SX_DESCRIPTOR_CODE_DATA) != 0;
}

static inline uint8_t r36sx_descriptor_is_code(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_code_data(cache) &&
           (cache->access & R36SX_DESCRIPTOR_EXECUTABLE);
}

static inline uint8_t r36sx_descriptor_is_data(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_code_data(cache) &&
           ((cache->access & R36SX_DESCRIPTOR_EXECUTABLE) == 0);
}

static inline uint8_t r36sx_descriptor_is_readable_code(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_code(cache) &&
           (cache->access & R36SX_DESCRIPTOR_READABLE);
}

static inline uint8_t r36sx_descriptor_is_writable_data(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_data(cache) &&
           (cache->access & R36SX_DESCRIPTOR_WRITABLE);
}

static inline uint8_t r36sx_descriptor_is_usable_data_segment(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_data(cache) ||
           r36sx_descriptor_is_readable_code(cache);
}

static inline uint8_t r36sx_cpu_descriptor_uses_386_format(void)
{
    return r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386);
}

static void r36sx_cpu_fill_descriptor_cache(uint16_t selector,
                                            uint32_t lo,
                                            uint32_t hi,
                                            r36sx_segment_cache_t *cache)
{
    uint32_t limit = lo & 0xffffu;
    uint32_t base = ((lo >> 16) & 0xffffu) |
                    ((hi & 0x000000ffu) << 16);
    uint8_t flags = 0;

    /*
     * Intel 80286 descriptors have a 24-bit base and a 16-bit limit.  The
     * upper limit bits, D/B, granularity, and high base byte are 80386-era
     * extensions and must not leak into 286 mode.
     */
    if (r36sx_cpu_descriptor_uses_386_format()) {
        limit |= hi & 0x000f0000u;
        flags = (uint8_t)((hi >> 20) & 0x0fu);
        if (flags & R36SX_DESCRIPTOR_FLAG_GRANULAR) {
            limit = (limit << 12) | 0x0fffu;
        }
        base |= hi & 0xff000000u;
    }

    cache->selector = selector;
    cache->base = base;
    cache->limit = limit;
    cache->access = (uint8_t)((hi >> 8) & 0xffu);
    cache->flags = flags;
    cache->valid = (cache->access & R36SX_DESCRIPTOR_PRESENT) != 0;
}

static inline uint8_t r36sx_descriptor_is_tss(
    const r36sx_segment_cache_t *cache)
{
    uint8_t type = r36sx_descriptor_type(cache);
    if (r36sx_descriptor_is_code_data(cache)) {
        return 0;
    }
    if (type == R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE) {
        return 1;
    }
    return r36sx_cpu_descriptor_uses_386_format() &&
           type == R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE;
}

static inline uint8_t r36sx_descriptor_dpl(
    const r36sx_segment_cache_t *cache)
{
    return (cache->access >> 5) & 3u;
}

static inline uint8_t r36sx_selector_rpl(uint16_t selector)
{
    return selector & 3u;
}

static inline uint8_t r36sx_cpu_cpl(void)
{
    if (!r36sx_cpu_protected_enabled()) {
        return 0u;
    }
    /*
     * Virtual-8086 tasks run with protected mode enabled, but the processor
     * treats their current privilege level as 3 regardless of the low CS bits.
     */
    return r36sx_cpu_v86_enabled() ? 3u : (uint8_t)(CPU_CS & 3u);
}

static inline uint8_t r36sx_priv_max(uint8_t a, uint8_t b)
{
    return a > b ? a : b;
}

static inline uint8_t r36sx_descriptor_is_conforming_code(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_code(cache) &&
           (cache->access & R36SX_DESCRIPTOR_CONFORMING);
}

static inline uint8_t r36sx_descriptor_is_expand_down_data(
    const r36sx_segment_cache_t *cache)
{
    return r36sx_descriptor_is_data(cache) &&
           (cache->access & R36SX_DESCRIPTOR_EXPAND_DOWN);
}

static uint8_t r36sx_cpu_segment_privilege_ok(
    uint8_t segid,
    uint16_t selector,
    const r36sx_segment_cache_t *cache)
{
    uint8_t cpl = r36sx_cpu_cpl();
    uint8_t rpl = r36sx_selector_rpl(selector);
    uint8_t dpl = r36sx_descriptor_dpl(cache);

    if (segid == regcs) {
        if (!r36sx_descriptor_is_code(cache)) {
            return 0;
        }
        if (r36sx_descriptor_is_conforming_code(cache)) {
            return dpl <= cpl;
        }
        return dpl == cpl && rpl <= cpl;
    }

    if (segid == regss) {
        return r36sx_descriptor_is_writable_data(cache) &&
               dpl == cpl && rpl == cpl;
    }

    if (r36sx_descriptor_is_conforming_code(cache)) {
        return r36sx_descriptor_is_readable_code(cache);
    }

    if (!r36sx_descriptor_is_usable_data_segment(cache)) {
        return 0;
    }

    return dpl >= r36sx_priv_max(cpl, rpl);
}

static void r36sx_cpu_clear_segment_cache(uint8_t segid, uint16_t selector)
{
    putsegreg(segid, selector);
    segselector16[segid] = selector;
    r36sx_seg_cache[segid].selector = selector;
    r36sx_seg_cache[segid].base = 0;
    r36sx_seg_cache[segid].limit = 0;
    r36sx_seg_cache[segid].access = 0;
    r36sx_seg_cache[segid].flags = 0;
    r36sx_seg_cache[segid].valid = 0;
    segbase32[segid] = 0;
}

static void r36sx_cpu_commit_segment_cache(uint8_t segid,
                                           uint16_t selector,
                                           const r36sx_segment_cache_t *cache)
{
    putsegreg(segid, selector);
    r36sx_seg_cache[segid] = *cache;
    segselector16[segid] = selector;
    segbase32[segid] = cache->base;
}

uint32_t r36sx_cpu_segbase(uint16_t selector)
{
    if (!r36sx_cpu_protected_enabled()) {
        return (uint32_t)selector << 4;
    }

    for (uint8_t segid = reges; segid <= reggs; segid++) {
        if (r36sx_seg_cache[segid].valid &&
            r36sx_seg_cache[segid].selector == selector) {
            return segbase32[segid];
        }
    }

    return (uint32_t)selector << 4;
}

static inline void r36sx_cpu_use_segment(uint8_t segid)
{
    useseg = segselector16[segid];
    useseg_base = segbase32[segid];
}

static inline uint8_t r36sx_cpu_code_default32(void)
{
    return r36sx_cpu_descriptor_uses_386_format() &&
           r36sx_cpu_protected_enabled() &&
           r36sx_seg_cache[regcs].valid &&
           r36sx_descriptor_is_code(&r36sx_seg_cache[regcs]) &&
           (r36sx_seg_cache[regcs].flags & R36SX_DESCRIPTOR_FLAG_DB);
}

static inline uint8_t r36sx_cpu_stack_default32(void)
{
    return r36sx_cpu_descriptor_uses_386_format() &&
           r36sx_cpu_protected_enabled() &&
           r36sx_seg_cache[regss].valid &&
           (r36sx_seg_cache[regss].flags & R36SX_DESCRIPTOR_FLAG_DB);
}

static inline uint32_t r36sx_cpu_mask_ip(uint32_t value)
{
    return r36sx_cpu_code_default32() ? value : (uint16_t)value;
}

static inline void r36sx_cpu_set_ip(uint32_t value)
{
    ip32 = r36sx_cpu_mask_ip(value);
}

static inline void r36sx_cpu_add_ip(int32_t delta)
{
    r36sx_cpu_set_ip(ip32 + (uint32_t)delta);
}

#if R36SX_DEBUG_PM_DIAG
static uint8_t r36sx_pm_diag_first_fault_logged;
static uint32_t r36sx_pm_diag_int31_logs;
static uint32_t r36sx_pm_diag_int67_logs;

static void r36sx_pm_diag_log_state(const char *event)
{
    r36sx_pico286_debug_log(
        "[PM] %s cs:eip=%04X:%08lX ss:sp=%04X:%04X esp=%08lX "
        "cr0=%08lX cr2=%08lX cr3=%08lX gdtr=%08lX:%04X "
        "idtr=%08lX:%04X ldtr=%04X tr=%04X flags=%04X",
        event,
        CPU_CS, (unsigned long)CPU_IP,
        CPU_SS, CPU_SP, (unsigned long)CPU_ESP,
        (unsigned long)r36sx_cr0,
        (unsigned long)r36sx_cr2,
        (unsigned long)r36sx_cr3,
        (unsigned long)r36sx_gdtr_base, r36sx_gdtr_limit,
        (unsigned long)r36sx_idtr_base, r36sx_idtr_limit,
        r36sx_ldtr_selector, r36sx_tr_selector,
        (uint16_t)(2u | x86_flags.value));
}

static void r36sx_pm_diag_log_first_fault(const char *reason,
                                          uint32_t fault_ip)
{
    if (!r36sx_cpu_protected_enabled() || r36sx_pm_diag_first_fault_logged) {
        return;
    }
    r36sx_pm_diag_first_fault_logged = 1;
    r36sx_pico286_debug_log(
        "[PM] first fault reason=%s cs:eip=%04X:%08lX "
        "bytes=%02X %02X %02X %02X %02X %02X %02X %02X",
        reason, CPU_CS, (unsigned long)fault_ip,
        getmem8(CPU_CS, fault_ip),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 1u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 2u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 3u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 4u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 5u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 6u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 7u)));
    r36sx_pm_diag_log_state("first fault state");
}

static void r36sx_pm_diag_log_interrupt(uint8_t intnum)
{
    if (intnum == 0x2Fu && (CPU_AX == 0x1686u || CPU_AX == 0x1687u)) {
        r36sx_pico286_debug_log(
            "[PM] DPMI probe INT 2Fh AX=%04X protected=%u "
            "cs:eip=%04X:%08lX",
            CPU_AX, r36sx_cpu_protected_enabled(),
            CPU_CS, (unsigned long)CPU_IP);
        return;
    }

    if (intnum == 0x31u) {
        if (r36sx_pm_diag_int31_logs < 32u) {
            r36sx_pico286_debug_log(
                "[PM] DPMI service INT 31h AX=%04X BX=%04X CX=%04X "
                "DX=%04X protected=%u cs:eip=%04X:%08lX",
                CPU_AX, CPU_BX, CPU_CX, CPU_DX,
                r36sx_cpu_protected_enabled(),
                CPU_CS, (unsigned long)CPU_IP);
            r36sx_pm_diag_int31_logs++;
        }
        return;
    }

    if (intnum == 0x67u && (CPU_AX & 0xFF00u) == 0xDE00u) {
        if (r36sx_pm_diag_int67_logs < 32u) {
            r36sx_pico286_debug_log(
                "[PM] VCPI probe/service INT 67h AX=%04X BX=%04X CX=%04X "
                "DX=%04X protected=%u cs:eip=%04X:%08lX",
                CPU_AX, CPU_BX, CPU_CX, CPU_DX,
                r36sx_cpu_protected_enabled(),
                CPU_CS, (unsigned long)CPU_IP);
            r36sx_pm_diag_int67_logs++;
        }
    }
}
#else
static inline void r36sx_pm_diag_log_first_fault(const char *reason,
                                                 uint32_t fault_ip)
{
    (void)reason;
    (void)fault_ip;
}

static inline void r36sx_pm_diag_log_interrupt(uint8_t intnum)
{
    (void)intnum;
}
#endif

void r36sx_cpu_step_ip(uint32_t delta)
{
    r36sx_cpu_set_ip(ip32 + delta);
}

static inline uint32_t r36sx_cpu_linear_ea(uint32_t offset)
{
    return useseg_base + offset;
}

static uint8_t r36sx_cpu_decode_descriptor_from_table(
    uint16_t selector,
    uint32_t table_base,
    uint32_t table_limit,
    r36sx_segment_cache_t *cache,
    const char *table_name,
    uint8_t require_present)
{
    if ((selector & 0xfffcu) == 0) {
        cache->selector = selector;
        cache->base = 0;
        cache->limit = 0;
        cache->access = 0;
        cache->flags = 0;
        cache->valid = 0;
        return 1;
    }

    uint32_t descriptor_offset = selector & R36SX_SELECTOR_INDEX_MASK;
    if (descriptor_offset + 7u > table_limit) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode %s limit fault selector=%04x table=%08lx:%08lx",
            table_name, selector, (unsigned long)table_base,
            (unsigned long)table_limit);
#endif
        r36sx_pm_diag_log_first_fault("descriptor table limit", CPU_IP);
        return 0;
    }

    uint32_t addr = table_base + descriptor_offset;
    uint32_t lo = readdw86(addr);
    uint32_t hi = readdw86(addr + 4u);
    r36sx_cpu_fill_descriptor_cache(selector, lo, hi, cache);
    return require_present ? cache->valid : 1u;
}

static uint8_t r36sx_cpu_decode_descriptor(uint16_t selector,
                                           r36sx_segment_cache_t *cache)
{
    if (r36sx_dpmi_lookup_descriptor(selector, cache)) {
        return 1;
    }

    if ((selector & 0xfffcu) == 0) {
        return r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_gdtr_base, r36sx_gdtr_limit, cache, "GDT", 1);
    }

    if (selector & R36SX_SELECTOR_TABLE_INDICATOR) {
        if (!r36sx_ldtr_cache.valid) {
#if R36SX_DEBUG_PM_VERBOSE
            r36sx_pico286_debug_log(
                "[CPU] protected mode LDT selector without loaded LDTR selector=%04x",
                selector);
#endif
            return 0;
        }
        return r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_ldtr_cache.base, r36sx_ldtr_cache.limit, cache,
            "LDT", 1);
    }

    return r36sx_cpu_decode_descriptor_from_table(
        selector, r36sx_gdtr_base, r36sx_gdtr_limit, cache, "GDT", 1);
}

static uint8_t r36sx_cpu_decode_descriptor_any(uint16_t selector,
                                               r36sx_segment_cache_t *cache)
{
    if (r36sx_dpmi_lookup_descriptor(selector, cache)) {
        return 1;
    }

    if ((selector & 0xfffcu) == 0) {
        return r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_gdtr_base, r36sx_gdtr_limit, cache, "GDT", 0);
    }

    if (selector & R36SX_SELECTOR_TABLE_INDICATOR) {
        if (!r36sx_ldtr_cache.valid) {
            return 0;
        }
        return r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_ldtr_cache.base, r36sx_ldtr_cache.limit, cache,
            "LDT", 0);
    }

    return r36sx_cpu_decode_descriptor_from_table(
        selector, r36sx_gdtr_base, r36sx_gdtr_limit, cache, "GDT", 0);
}

static uint8_t r36sx_cpu_descriptor_visible_for_validation(
    uint16_t selector,
    const r36sx_segment_cache_t *cache)
{
    if ((selector & 0xfffcu) == 0) {
        return 0;
    }
    if (r36sx_descriptor_is_conforming_code(cache)) {
        return 1;
    }
    return r36sx_descriptor_dpl(cache) >=
           r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector));
}

static uint8_t r36sx_cpu_descriptor_type_valid_for_lar(
    const r36sx_segment_cache_t *cache)
{
    if (r36sx_descriptor_is_code_data(cache)) {
        return 1;
    }

    switch (r36sx_descriptor_type(cache)) {
        case R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE:
        case R36SX_DESCRIPTOR_TYPE_LDT:
        case R36SX_DESCRIPTOR_TYPE_TSS16_BUSY:
        case 0x04u: /* 80286 call gate */
        case R36SX_DESCRIPTOR_TYPE_TASK_GATE:
        case 0x06u: /* 80286 interrupt gate */
        case 0x07u: /* 80286 trap gate */
            return 1;
        case R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE:
        case R36SX_DESCRIPTOR_TYPE_TSS32_BUSY:
        case 0x0cu: /* 80386 call gate */
        case 0x0eu: /* 80386 interrupt gate */
        case 0x0fu: /* 80386 trap gate */
            return r36sx_cpu_descriptor_uses_386_format();
    }
    return 0;
}

static uint8_t r36sx_cpu_descriptor_type_valid_for_lsl(
    const r36sx_segment_cache_t *cache)
{
    if (r36sx_descriptor_is_code_data(cache)) {
        return 1;
    }

    switch (r36sx_descriptor_type(cache)) {
        case R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE:
        case R36SX_DESCRIPTOR_TYPE_LDT:
        case R36SX_DESCRIPTOR_TYPE_TSS16_BUSY:
            return 1;
        case R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE:
        case R36SX_DESCRIPTOR_TYPE_TSS32_BUSY:
            return r36sx_cpu_descriptor_uses_386_format();
    }
    return 0;
}

static uint32_t r36sx_cpu_descriptor_access_rights(
    const r36sx_segment_cache_t *cache)
{
    return (((uint32_t)cache->access) << 8) |
           (((uint32_t)(cache->flags & 0x0fu)) << 20);
}

static uint8_t r36sx_cpu_segment_valid_for_load(
    uint8_t segid,
    const r36sx_segment_cache_t *cache)
{
    if (!cache->valid) {
        return 0;
    }

    if (segid == regcs) {
        return r36sx_descriptor_is_code(cache);
    }
    if (segid == regss) {
        return r36sx_descriptor_is_writable_data(cache);
    }
    return r36sx_descriptor_is_usable_data_segment(cache);
}

static uint8_t r36sx_cpu_load_segment(uint8_t segid, uint16_t selector)
{
    if (!r36sx_cpu_protected_enabled()) {
        putsegreg(segid, selector);
        r36sx_cpu_real_cache_segment(segid);
        return 1;
    }

    if (r36sx_cpu_v86_enabled()) {
        putsegreg(segid, selector);
        r36sx_cpu_real_cache_segment(segid);
        return 1;
    }

    if ((selector & 0xfffcu) == 0) {
        if (segid == regcs || segid == regss) {
#if R36SX_DEBUG_PM_VERBOSE
            r36sx_pico286_debug_log(
                "[CPU] protected mode null selector rejected seg=%u selector=%04x",
                segid, selector);
#endif
            r36sx_pm_diag_log_first_fault("null CS/SS selector load", CPU_IP);
            return 0;
        }
        r36sx_cpu_clear_segment_cache(segid, selector);
        return 1;
    }

    r36sx_segment_cache_t cache;
    memset(&cache, 0, sizeof(cache));
    uint8_t decoded = r36sx_cpu_decode_descriptor(selector, &cache);
    if (!decoded || !cache.valid) {
        uint8_t exc = segid == regss ? R36SX_EXCEPTION_STACK :
                      cache.access ? R36SX_EXCEPTION_NOT_PRESENT :
                      R36SX_EXCEPTION_GP;
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode failed to load present segment seg=%u selector=%04x access=%02x flags=%02x exc=%u",
            segid, selector, cache.access, cache.flags, exc);
#endif
        r36sx_pm_diag_log_first_fault("segment not present/table fault",
                                      CPU_IP);
        r36sx_cpu_raise_exception(exc, selector & 0xfffcu,
                                  1, CPU_IP);
        return 0;
    }

    if (!r36sx_cpu_segment_valid_for_load(segid, &cache) ||
        !r36sx_cpu_segment_privilege_ok(segid, selector, &cache)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode failed to load segment seg=%u selector=%04x access=%02x flags=%02x cpl=%u dpl=%u rpl=%u",
            segid, selector, cache.access, cache.flags,
            r36sx_cpu_cpl(), r36sx_descriptor_dpl(&cache),
            r36sx_selector_rpl(selector));
#endif
        r36sx_pm_diag_log_first_fault("segment load failed", CPU_IP);
        r36sx_cpu_raise_exception(
            segid == regss ? R36SX_EXCEPTION_STACK : R36SX_EXCEPTION_GP,
            selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }

    r36sx_cpu_commit_segment_cache(segid, selector, &cache);
    return 1;
}

static uint8_t r36sx_cpu_load_ldtr(uint16_t selector, uint32_t fault_ip)
{
    if ((selector & 0xfffcu) == 0) {
        r36sx_ldtr_selector = selector;
        memset(&r36sx_ldtr_cache, 0, sizeof(r36sx_ldtr_cache));
        return 1;
    }

    if (selector & R36SX_SELECTOR_TABLE_INDICATOR) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode LLDT selector must be in GDT selector=%04x",
            selector);
#endif
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, selector & 0xfffcu,
                                  1, fault_ip);
        return 0;
    }

    r36sx_segment_cache_t cache;
    memset(&cache, 0, sizeof(cache));
    if (!r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_gdtr_base, r36sx_gdtr_limit, &cache, "GDT", 0) ||
        r36sx_descriptor_is_code_data(&cache) ||
        r36sx_descriptor_type(&cache) != R36SX_DESCRIPTOR_TYPE_LDT) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode LLDT rejected selector=%04x access=%02x flags=%02x",
            selector, cache.access, cache.flags);
#endif
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, selector & 0xfffcu,
                                  1, fault_ip);
        return 0;
    }
    if (!cache.valid) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_NOT_PRESENT,
                                  selector & 0xfffcu, 1, fault_ip);
        return 0;
    }

    r36sx_ldtr_selector = selector;
    r36sx_ldtr_cache = cache;
    return 1;
}

static uint8_t r36sx_cpu_load_tr(uint16_t selector, uint32_t fault_ip)
{
    if ((selector & 0xfffcu) == 0 ||
        (selector & R36SX_SELECTOR_TABLE_INDICATOR)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode LTR rejected selector=%04x", selector);
#endif
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, selector & 0xfffcu,
                                  1, fault_ip);
        return 0;
    }

    r36sx_segment_cache_t cache;
    memset(&cache, 0, sizeof(cache));
    if (!r36sx_cpu_decode_descriptor_from_table(
            selector, r36sx_gdtr_base, r36sx_gdtr_limit, &cache, "GDT", 0) ||
        !r36sx_descriptor_is_tss(&cache)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected mode LTR rejected selector=%04x access=%02x flags=%02x",
            selector, cache.access, cache.flags);
#endif
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, selector & 0xfffcu,
                                  1, fault_ip);
        return 0;
    }
    if (!cache.valid) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_NOT_PRESENT,
                                  selector & 0xfffcu, 1, fault_ip);
        return 0;
    }

    if (!r36sx_cpu_set_tss_busy(selector, 1)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  selector & 0xfffcu, 1, fault_ip);
        return 0;
    }
    cache.access = (cache.access & (uint8_t)~R36SX_DESCRIPTOR_SYSTEM_TYPE_MASK) |
                   (r36sx_descriptor_type(&cache) ==
                            R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE
                        ? R36SX_DESCRIPTOR_TYPE_TSS16_BUSY
                        : R36SX_DESCRIPTOR_TYPE_TSS32_BUSY);
    r36sx_tr_selector = selector;
    r36sx_tr_cache = cache;
    return 1;
}

static void r36sx_cpu_set_cr0(uint32_t value)
{
    uint8_t old_pe = r36sx_cpu_protected_enabled();
    if ((value & R36SX_CR0_PG) && !(value & R36SX_CR0_PE)) {
        R36SX_PM_DIAG_LOG(
            "[PM] CR0 write rejected PG without PE requested=%08lX "
            "cs:eip=%04X:%08lX",
            (unsigned long)value, CPU_CS, (unsigned long)CPU_IP);
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, CPU_IP);
        return;
    }
#if R36SX_ENABLE_PROTECTED_MODE
    uint32_t old_cr0 = r36sx_cr0;
    r36sx_cr0 = value | R36SX_CR0_ET;
#else
    uint32_t old_cr0 = r36sx_cr0;
    r36sx_cr0 = (value & ~R36SX_CR0_PE) | R36SX_CR0_ET;
#endif
    uint8_t new_pe = r36sx_cpu_protected_enabled();
    r36sx_cpu_interpreter_protected = new_pe;

    R36SX_PM_DIAG_LOG(
        "[PM] CR0 write old=%08lX requested=%08lX new=%08lX pe=%u->%u "
        "cs:eip=%04X:%08lX",
        (unsigned long)old_cr0, (unsigned long)value,
        (unsigned long)r36sx_cr0, old_pe, new_pe,
        CPU_CS, (unsigned long)CPU_IP);
    (void)old_cr0;

    if (old_pe != new_pe) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log("[CPU] protected mode %s CR0=%08lx",
                                new_pe ? "entered" : "left",
                                (unsigned long)r36sx_cr0);
#endif
        if (!new_pe) {
            r36sx_cpu_real_cache_all_segments();
        }
    }
}

static inline uint32_t r36sx_cpu_read_cr0(void)
{
    /*
     * DOSBox reports 386 CR0 reads with bits 4..30 set.  This matches the
     * 80386 reserved-bit behavior that some CPU probes expect.
     */
    return r36sx_cr0 | R36SX_CR0_386_RESERVED_READ_MASK;
}

static void r36sx_cpu_lmsw(uint16_t value)
{
    R36SX_PM_DIAG_LOG(
        "[PM] LMSW value=%04X old_cr0=%08lX cs:eip=%04X:%08lX",
        value, (unsigned long)r36sx_cr0, CPU_CS, (unsigned long)CPU_IP);
    uint32_t low = value & 0x000fu;
    if (r36sx_cr0 & R36SX_CR0_PE) {
        low |= R36SX_CR0_PE;
    }
    r36sx_cpu_set_cr0((r36sx_cr0 & ~0x000fu) | low);
}

static void r36sx_cpu_store_descriptor_table(uint32_t addr,
                                             uint16_t limit,
                                             uint32_t base)
{
    writew86(addr, limit);
    writedw86(addr + 2u, base);
}

static void r36sx_cpu_load_descriptor_table(uint32_t addr,
                                            uint16_t *limit,
                                            uint32_t *base,
                                            uint32_t base_mask)
{
    *limit = readw86(addr);
    *base = readdw86(addr + 2u) & base_mask;
}

