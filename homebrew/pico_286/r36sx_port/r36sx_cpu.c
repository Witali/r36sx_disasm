#include <time.h>
#include <stdbool.h>
#include "emulator.h"

#define CPU_ALLOW_ILLEGAL_OP_EXCEPTION
#define CPU_LIMIT_SHIFT_COUNT
#define CPU_NO_SALC
//#define CPU_SET_HIGH_FLAGS
#define CPU_286_STYLE_PUSH_SP
#define R36SX_REP_BATCH_MAX 1024u
#ifndef R36SX_CPU_COMPUTED_GOTO
#define R36SX_CPU_COMPUTED_GOTO 0
#endif
#if R36SX_CPU_COMPUTED_GOTO && !defined(__GNUC__) && !defined(__clang__)
#error R36SX_CPU_COMPUTED_GOTO requires GNU labels-as-values support.
#endif
#if PICO_ON_DEVICE

#include "disks-rp2350.c.inl"
#include "network-redirector-rp2350.c.inl"
#include "graphics.h"
#include "psram_spi.h"
#include "swap.h"
static inline void r36sx_app_stats_record_x86(uint32_t instructions)
{
    (void)instructions;
}
#else

#include "r36sx_disk_config.h"
#include "r36sx_app_stats.h"
#include "disks-win32.c.inl"
#define R36SX_PICO286_HOST_DRIVE_CONFIG 1
#include "network-redirector.c.inl"

#endif

#ifdef TOTAL_VIRTUAL_MEMORY_KBS
#undef __not_in_flash
#define __not_in_flash(group)
#endif

int videomode = 3;
uint8_t segoverride, reptype;
uint32_t segregs32[6];
uint16_t useseg, oldsp;
uint32_t ip32;
uint8_t tempcf, oldcf, mode, reg, rm, sib;
x86_flags_t x86_flags;
bool operandSizeOverride = false;
bool addressSizeOverride = false;
static volatile uint8_t hltstate;

static const uint8_t __not_in_flash("cpu.regt") byteregtable[8] = {
    regal, regcl, regdl, regbl, regah, regch, regdh, regbh
};

uint8_t nestlev;
uint16_t saveip, savecs, oper1, oper2, res16, temp16, dummy, stacksize, frametemp;
uint32_t disp32;
#define disp16 (*(uint16_t*)&disp32)
uint32_t ea;

uint32_t dwordregs[8];

#define R36SX_CR0_PE 0x00000001u
#define R36SX_CR0_MP 0x00000002u
#define R36SX_CR0_EM 0x00000004u
#define R36SX_CR0_TS 0x00000008u
#define R36SX_CR0_ET 0x00000010u
#define R36SX_CR0_386_RESERVED_READ_MASK 0x7ffffff0u
#define R36SX_CR3_PAGE_DIRECTORY_MASK 0xfffff000u
#define R36SX_DESCRIPTOR_PRESENT 0x80u
#define R36SX_DESCRIPTOR_CODE_DATA 0x10u
#define R36SX_DESCRIPTOR_EXECUTABLE 0x08u
#define R36SX_DESCRIPTOR_WRITABLE 0x02u
#define R36SX_DESCRIPTOR_READABLE 0x02u
#define R36SX_DESCRIPTOR_FLAG_DB 0x04u
#define R36SX_DESCRIPTOR_FLAG_GRANULAR 0x08u

typedef struct {
    uint16_t selector;
    uint32_t base;
    uint32_t limit;
    uint8_t access;
    uint8_t flags;
    uint8_t valid;
} r36sx_segment_cache_t;

static r36sx_segment_cache_t r36sx_seg_cache[6];
static uint32_t r36sx_cr0 = R36SX_CR0_ET;
static uint32_t r36sx_cr2;
static uint32_t r36sx_cr3;
static uint32_t r36sx_gdtr_base;
static uint32_t r36sx_idtr_base;
static uint16_t r36sx_gdtr_limit;
static uint16_t r36sx_idtr_limit = 0x03ffu;
static uint16_t r36sx_ldtr_selector;
static uint16_t r36sx_tr_selector;

static inline uint8_t r36sx_cpu_protected_enabled(void)
{
    return (r36sx_cr0 & R36SX_CR0_PE) != 0;
}

static inline void r36sx_cpu_real_cache_segment(uint8_t segid)
{
    uint16_t selector = getsegreg(segid);
    r36sx_seg_cache[segid].selector = selector;
    r36sx_seg_cache[segid].base = (uint32_t)selector << 4;
    r36sx_seg_cache[segid].limit = 0xffffu;
    r36sx_seg_cache[segid].access =
        R36SX_DESCRIPTOR_PRESENT | R36SX_DESCRIPTOR_CODE_DATA |
        R36SX_DESCRIPTOR_READABLE | R36SX_DESCRIPTOR_WRITABLE;
    r36sx_seg_cache[segid].flags = 0;
    r36sx_seg_cache[segid].valid = 1;
}

static inline void r36sx_cpu_real_cache_all_segments(void)
{
    for (uint8_t segid = reges; segid <= reggs; segid++) {
        r36sx_cpu_real_cache_segment(segid);
    }
}

uint32_t r36sx_cpu_segbase(uint16_t selector)
{
    if (!r36sx_cpu_protected_enabled()) {
        return (uint32_t)selector << 4;
    }

    for (uint8_t segid = reges; segid <= reggs; segid++) {
        if (r36sx_seg_cache[segid].valid &&
            r36sx_seg_cache[segid].selector == selector) {
            return r36sx_seg_cache[segid].base;
        }
    }

    return (uint32_t)selector << 4;
}

void r36sx_cpu_step_ip(uint32_t delta)
{
    CPU_IP = (uint16_t)(CPU_IP + delta);
}

static inline uint32_t r36sx_cpu_linear_ea(uint32_t offset)
{
    return segbase(useseg) + offset;
}

static uint8_t r36sx_cpu_decode_descriptor(uint16_t selector,
                                           r36sx_segment_cache_t *cache)
{
    if ((selector & 0xfffcu) == 0) {
        cache->selector = selector;
        cache->base = 0;
        cache->limit = 0;
        cache->access = 0;
        cache->flags = 0;
        cache->valid = 1;
        return 1;
    }

    if (selector & 0x0004u) {
#if DEBUG && !PICO_ON_DEVICE
        r36sx_pico286_debug_log(
            "[CPU] protected mode LDT selector not implemented selector=%04x",
            selector);
#endif
        return 0;
    }

    uint32_t descriptor_offset = selector & 0xfff8u;
    if (descriptor_offset + 7u > r36sx_gdtr_limit) {
#if DEBUG && !PICO_ON_DEVICE
        r36sx_pico286_debug_log(
            "[CPU] protected mode GDT limit fault selector=%04x gdtr=%08lx:%04x",
            selector, (unsigned long)r36sx_gdtr_base, r36sx_gdtr_limit);
#endif
        return 0;
    }

    uint32_t addr = r36sx_gdtr_base + descriptor_offset;
    uint32_t lo = readdw86(addr);
    uint32_t hi = readdw86(addr + 4u);
    uint32_t limit = (lo & 0xffffu) | (hi & 0x000f0000u);
    uint8_t flags = (uint8_t)((hi >> 20) & 0x0fu);

    if (flags & R36SX_DESCRIPTOR_FLAG_GRANULAR) {
        limit = (limit << 12) | 0x0fffu;
    }

    cache->selector = selector;
    cache->base = ((lo >> 16) & 0xffffu) |
                  ((hi & 0x000000ffu) << 16) |
                  (hi & 0xff000000u);
    cache->limit = limit;
    cache->access = (uint8_t)((hi >> 8) & 0xffu);
    cache->flags = flags;
    cache->valid = (cache->access & R36SX_DESCRIPTOR_PRESENT) != 0;
    return cache->valid;
}

static uint8_t r36sx_cpu_load_segment(uint8_t segid, uint16_t selector)
{
    putsegreg(segid, selector);

    if (!r36sx_cpu_protected_enabled()) {
        r36sx_cpu_real_cache_segment(segid);
        return 1;
    }

    r36sx_segment_cache_t cache;
    if (!r36sx_cpu_decode_descriptor(selector, &cache)) {
#if DEBUG && !PICO_ON_DEVICE
        r36sx_pico286_debug_log(
            "[CPU] protected mode failed to load segment seg=%u selector=%04x",
            segid, selector);
#endif
        cache.selector = selector;
        cache.base = 0;
        cache.limit = 0;
        cache.access = 0;
        cache.flags = 0;
        cache.valid = 0;
    }

    r36sx_seg_cache[segid] = cache;
    return cache.valid;
}

static void r36sx_cpu_set_cr0(uint32_t value)
{
    uint8_t old_pe = r36sx_cpu_protected_enabled();
    r36sx_cr0 = value | R36SX_CR0_ET;
    uint8_t new_pe = r36sx_cpu_protected_enabled();

    if (old_pe != new_pe) {
#if DEBUG && !PICO_ON_DEVICE
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

static inline uint8_t r36sx_cpu_pending_maskable_irq(void)
{
    return i8259_controller.interrupt_request_register &
           (uint8_t)(~i8259_controller.interrupt_mask_register);
}

uint8_t r36sx_cpu_waiting_for_interrupt(void)
{
    return hltstate && !(ifl && r36sx_cpu_pending_maskable_irq());
}

static inline uint32_t r36sx_rep_batch_count(uint32_t count,
                                             uint32_t loopcount,
                                             uint32_t execloops)
{
    uint32_t batch = count;
    uint32_t budget = execloops > loopcount ? execloops - loopcount : 1u;

    if (batch > budget) {
        batch = budget;
    }
    if (batch > R36SX_REP_BATCH_MAX) {
        batch = R36SX_REP_BATCH_MAX;
    }
    return batch ? batch : 1u;
}

static inline uint32_t r36sx_rep_get_count(void)
{
    return addressSizeOverride ? CPU_ECX : CPU_CX;
}

static inline void r36sx_rep_set_count(uint32_t count)
{
    if (addressSizeOverride) {
        CPU_ECX = count;
    } else {
        CPU_CX = (uint16_t)count;
    }
}

static inline uint32_t r36sx_src_index(void)
{
    return addressSizeOverride ? CPU_ESI : CPU_SI;
}

static inline uint32_t r36sx_dst_index(void)
{
    return addressSizeOverride ? CPU_EDI : CPU_DI;
}

static inline void r36sx_set_src_index(uint32_t value)
{
    if (addressSizeOverride) {
        CPU_ESI = value;
    } else {
        CPU_SI = (uint16_t)value;
    }
}

static inline void r36sx_set_dst_index(uint32_t value)
{
    if (addressSizeOverride) {
        CPU_EDI = value;
    } else {
        CPU_DI = (uint16_t)value;
    }
}

static inline void r36sx_rep_movsb(uint32_t count)
{
    uint16_t si = CPU_SI;
    uint16_t di = CPU_DI;

    if (df) {
        while (count--) {
            putmem8(CPU_ES, di, getmem8(useseg, si));
            si--;
            di--;
        }
    } else {
        while (count--) {
            putmem8(CPU_ES, di, getmem8(useseg, si));
            si++;
            di++;
        }
    }

    CPU_SI = si;
    CPU_DI = di;
}

static inline void r36sx_rep_movsw(uint32_t count)
{
    uint16_t si = CPU_SI;
    uint16_t di = CPU_DI;

    if (df) {
        while (count--) {
            putmem16(CPU_ES, di, getmem16(useseg, si));
            si -= 2;
            di -= 2;
        }
    } else {
        while (count--) {
            putmem16(CPU_ES, di, getmem16(useseg, si));
            si += 2;
            di += 2;
        }
    }

    CPU_SI = si;
    CPU_DI = di;
}

static inline void r36sx_rep_movsd(uint32_t count)
{
    uint32_t si = r36sx_src_index();
    uint32_t di = r36sx_dst_index();

    if (df) {
        while (count--) {
            putmem32(CPU_ES, di, getmem32(useseg, si));
            si -= 4;
            di -= 4;
        }
    } else {
        while (count--) {
            putmem32(CPU_ES, di, getmem32(useseg, si));
            si += 4;
            di += 4;
        }
    }

    r36sx_set_src_index(si);
    r36sx_set_dst_index(di);
}

static inline void r36sx_rep_stosb(uint32_t count)
{
    uint16_t di = CPU_DI;
    uint8_t value = CPU_AL;

    if (df) {
        while (count--) {
            putmem8(CPU_ES, di, value);
            di--;
        }
    } else {
        while (count--) {
            putmem8(CPU_ES, di, value);
            di++;
        }
    }

    CPU_DI = di;
}

static inline void r36sx_rep_stosd(uint32_t count)
{
    uint32_t di = r36sx_dst_index();
    uint32_t value = CPU_EAX;

    if (df) {
        while (count--) {
            putmem32(CPU_ES, di, value);
            di -= 4;
        }
    } else {
        while (count--) {
            putmem32(CPU_ES, di, value);
            di += 4;
        }
    }

    r36sx_set_dst_index(di);
}

static inline void r36sx_rep_stosw(uint32_t count)
{
    uint16_t di = CPU_DI;
    uint16_t value = CPU_AX;

    if (df) {
        while (count--) {
            putmem16(CPU_ES, di, value);
            di -= 2;
        }
    } else {
        while (count--) {
            putmem16(CPU_ES, di, value);
            di += 2;
        }
    }

    CPU_DI = di;
}

static const bool __not_in_flash("cpu.pf") parity[0x100] = {
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1
};

__not_in_flash() void modregrm() {
    register uint8_t addrbyte = getmem8(CPU_CS, CPU_IP);
    StepIP(1);
    mode = addrbyte >> 6;
    reg = (addrbyte >> 3) & 7;
    rm = addrbyte & 7;
#ifdef CPU_386_EXTENDED_OPS
    if (addressSizeOverride) {
        // 32-?????? ?????
        if (mode != 3 && rm == 4) {
            sib = getmem8(CPU_CS, CPU_IP);
            StepIP(1);
        }
        switch (mode) {
            case 0:
                if (rm == 5 || (rm == 4 && (sib & 7u) == 5u)) {
                    disp32 = getmem32(CPU_CS, CPU_IP);
                    StepIP(4);
                } else {
                    disp32 = 0;
                }
                break;
            case 1:
                disp32 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                break;
            case 2:
                disp32 = getmem32(CPU_CS, CPU_IP);
                StepIP(4);
                break;
            default:
                disp32 = 0;
        }
        return;
    }
#endif
    switch (mode) {
        case 0:
            if (rm == 6) {
                disp16 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
            } else {
                disp16 = 0;
            }
            if (((rm == 2) || (rm == 3)) && !segoverride) {
                useseg = CPU_SS;
            }
            break;
        case 1:
            disp16 = signext(getmem8(CPU_CS, CPU_IP));
            StepIP(1);
            if (((rm == 2) || (rm == 3) || (rm == 6)) && !segoverride) {
                useseg = CPU_SS;
            }
            break;
        case 2:
            disp16 = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (((rm == 2) || (rm == 3) || (rm == 6)) && !segoverride) {
                useseg = CPU_SS;
            }
            break;
        default:
            disp16 = 0;
    }
}

#if 0
__not_in_flash() void r36sx_cpu_getea_legacy_unused(uint8_t rmval) {
    register uint32_t tempea = 0;
#ifdef CPU_386_EXTENDED_OPS
    if (addressSizeOverride) {
        addressSizeOverride = false;
        if (operandSizeOverride) {
            operandSizeOverride = false;
            // ???????? 32-?????? ?????????
            if (mode == 0 && rmval == 6) {
                tempea = disp32; // ?????????? 32-?????? displacement
            } else {
                // ???? Mode ?? 0 ??? RM ?? 6, ??????? ?? ??????? ????? ? SIB
                switch (mode) {
                    case 0:
                        switch (rmval) {
                            case 0: tempea = CPU_EBX + CPU_ESI;
                                break;
                            case 1: tempea = CPU_EBX + CPU_EDI;
                                break;
                            case 2: tempea = CPU_EBP + CPU_ESI;
                                break;
                            case 3: tempea = CPU_EBP + CPU_EDI;
                                break;
                            case 4: tempea = CPU_ESI;
                                break;
                            case 5: tempea = CPU_EDI;
                                break;
                            case 6: tempea = disp32;
                                break; // DISP32
                            case 7: tempea = CPU_EBX;
                                break;
                        }
                        break;
                    case 1:
                    case 2:
                        switch (rmval) {
                            case 0: tempea = CPU_EBX + CPU_ESI + disp32;
                                break;
                            case 1: tempea = CPU_EBX + CPU_EDI + disp32;
                                break;
                            case 2: tempea = CPU_EBP + CPU_ESI + disp32;
                                break;
                            case 3: tempea = CPU_EBP + CPU_EDI + disp32;
                                break;
                            case 4: tempea = CPU_ESI + disp32;
                                break;
                            case 5: tempea = CPU_EDI + disp32;
                                break;
                            case 6: tempea = CPU_EBP + disp32;
                                break;
                            case 7: tempea = CPU_EBX + disp32;
                                break;
                        }
                        break;
                }
            }
            // ????????? SIB, ???? ?????
            if (rmval == 4 && mode != 3) {
                // RM == 4 ????????? ?? SIB
                uint8_t sib_scale = sib >> 6;
                uint8_t sib_index = (sib >> 3) & 7;
                uint8_t sib_base = sib & 7;
                uint32_t sib_value = 0;
                // ????????? ??????? ???????
                switch (sib_base) {
                    case 0: sib_value = CPU_EBX;
                        break;
                    case 1: sib_value = CPU_ECX;
                        break;
                    case 2: sib_value = CPU_EDX;
                        break;
                    case 3: sib_value = CPU_EBX;
                        break;
                    case 4: sib_value = CPU_ESP;
                        break;
                    case 5: sib_value = CPU_EBP;
                        break;
                    case 6: sib_value = CPU_ESI;
                        break;
                    case 7: sib_value = CPU_EDI;
                        break;
                }
                // ????????? ?????? (???? ????)
                if (sib_index != 4) {
                    // ???? ?????? ?? ??????? (ESP, EBP ? ?.?.)
                    uint32_t index_value = getreg32(sib_index);
                    sib_value += (index_value << sib_scale); // ????????? ? ?????? ????????
                }
                tempea += sib_value;
            }
            return;
        }
        switch (mode) {
            case 0:
                switch (rmval) {
                    case 0: tempea = CPU_EBX + CPU_ESI;
                        break;
                    case 1: tempea = CPU_EBX + CPU_EDI;
                        break;
                    case 2: tempea = CPU_EBP + CPU_ESI;
                        break;
                    case 3: tempea = CPU_EBP + CPU_EDI;
                        break;
                    case 4: tempea = CPU_ESI;
                        break;
                    case 5: tempea = CPU_EDI;
                        break;
                    case 6: tempea = disp32;
                        break;
                    case 7: tempea = CPU_EBX;
                        break;
                }
                break;
            case 1:
            case 2:
                switch (rmval) {
                    case 0: tempea = CPU_EBX + CPU_ESI + disp32;
                        break;
                    case 1: tempea = CPU_EBX + CPU_EDI + disp32;
                        break;
                    case 2: tempea = CPU_EBP + CPU_ESI + disp32;
                        break;
                    case 3: tempea = CPU_EBP + CPU_EDI + disp32;
                        break;
                    case 4: tempea = CPU_ESI + disp32;
                        break;
                    case 5: tempea = CPU_EDI + disp32;
                        break;
                    case 6: tempea = CPU_EBP + disp32;
                        break;
                    case 7: tempea = CPU_EBX + disp32;
                        break;
                }
                break;
        }
        ea = (tempea & 0xFFFF) + (useseg << 4);
        return;
    }
    if (operandSizeOverride) {
        operandSizeOverride = false;
        // ???????? 32-?????? ?????????
        if (mode == 0 && rmval == 6) {
            tempea = disp32; // ?????????? 32-?????? displacement
        } else {
            // ???? Mode ?? 0 ??? RM ?? 6, ??????? ?? ??????? ????? ? SIB
            switch (mode) {
                case 0:
                    switch (rmval) {
                        case 0: tempea = CPU_BX + CPU_SI;
                            break;
                        case 1: tempea = CPU_BX + CPU_DI;
                            break;
                        case 2: tempea = CPU_BP + CPU_SI;
                            break;
                        case 3: tempea = CPU_BP + CPU_DI;
                            break;
                        case 4: tempea = CPU_SI;
                            break;
                        case 5: tempea = CPU_DI;
                            break;
                        case 6: tempea = disp32;
                            break; // DISP32
                        case 7: tempea = CPU_BX;
                            break;
                    }
                    break;
                case 1:
                case 2:
                    switch (rmval) {
                        case 0: tempea = CPU_BX + CPU_SI + disp32;
                            break;
                        case 1: tempea = CPU_BX + CPU_DI + disp32;
                            break;
                        case 2: tempea = CPU_BP + CPU_SI + disp32;
                            break;
                        case 3: tempea = CPU_BP + CPU_DI + disp32;
                            break;
                        case 4: tempea = CPU_SI + disp32;
                            break;
                        case 5: tempea = CPU_DI + disp32;
                            break;
                        case 6: tempea = CPU_BP + disp32;
                            break;
                        case 7: tempea = CPU_BX + disp32;
                            break;
                    }
                    break;
            }
        }
        // ????????? SIB, ???? ?????
        if (rmval == 4 && mode != 3) {
            // RM == 4 ????????? ?? SIB
            uint8_t sib_scale = sib >> 6;
            uint8_t sib_index = (sib >> 3) & 7;
            uint8_t sib_base = sib & 7;
            uint32_t sib_value = 0;
            // ????????? ??????? ???????
            switch (sib_base) {
                case 0: sib_value = CPU_BX;
                    break;
                case 1: sib_value = CPU_CX;
                    break;
                case 2: sib_value = CPU_DX;
                    break;
                case 3: sib_value = CPU_BX;
                    break;
                case 4: sib_value = CPU_SP;
                    break;
                case 5: sib_value = CPU_BP;
                    break;
                case 6: sib_value = CPU_SI;
                    break;
                case 7: sib_value = CPU_DI;
                    break;
            }
            // ????????? ?????? (???? ????)
            if (sib_index != 4) {
                // ???? ?????? ?? ??????? (ESP, EBP ? ?.?.)
                uint32_t index_value = getreg32(sib_index);
                sib_value += (index_value << sib_scale); // ????????? ? ?????? ????????
            }
            tempea += sib_value;
        }
    }
#endif
    switch (mode) {
        case 0:
            switch (rmval) {
                case 0: tempea = CPU_BX + CPU_SI;
                    break;
                case 1: tempea = CPU_BX + CPU_DI;
                    break;
                case 2: tempea = CPU_BP + CPU_SI;
                    break;
                case 3: tempea = CPU_BP + CPU_DI;
                    break;
                case 4: tempea = CPU_SI;
                    break;
                case 5: tempea = CPU_DI;
                    break;
                case 6: tempea = disp16;
                    break;
                case 7: tempea = CPU_BX;
                    break;
            }
            break;

        case 1:
        case 2:
            switch (rmval) {
                case 0: tempea = CPU_BX + CPU_SI + disp16;
                    break;
                case 1: tempea = CPU_BX + CPU_DI + disp16;
                    break;
                case 2: tempea = CPU_BP + CPU_SI + disp16;
                    break;
                case 3: tempea = CPU_BP + CPU_DI + disp16;
                    break;
                case 4: tempea = CPU_SI + disp16;
                    break;
                case 5: tempea = CPU_DI + disp16;
                    break;
                case 6: tempea = CPU_BP + disp16;
                    break;
                case 7: tempea = CPU_BX + disp16;
                    break;
            }
            break;
    }
    ea = (tempea & 0xFFFF) + (useseg << 4);
}

#endif

static inline uint32_t r36sx_cpu_get_sib_base(uint8_t sib_value,
                                              uint32_t displacement,
                                              uint8_t mode_value,
                                              uint8_t *base_uses_ss)
{
    uint8_t base = sib_value & 7u;

    if (base_uses_ss) {
        *base_uses_ss = (base == regsp || base == regbp) &&
                        !(mode_value == 0 && base == regbp);
    }

    if (mode_value == 0 && base == regbp) {
        return displacement;
    }

    return getreg32(base);
}

static inline uint32_t r36sx_cpu_get_sib_index(uint8_t sib_value)
{
    uint8_t scale = sib_value >> 6;
    uint8_t index = (sib_value >> 3) & 7u;

    if (index == regsp) {
        return 0;
    }

    return getreg32(index) << scale;
}

static inline uint32_t r36sx_cpu_ea32(uint8_t rmval)
{
    uint32_t tempea = 0;
    uint8_t base_uses_ss = 0;

    switch (rmval) {
        case regax:
        case regcx:
        case regdx:
        case regbx:
        case regsi:
        case regdi:
            tempea = getreg32(rmval);
            break;

        case regsp:
            tempea = r36sx_cpu_get_sib_base(sib, disp32, mode, &base_uses_ss) +
                     r36sx_cpu_get_sib_index(sib);
            break;

        case regbp:
            if (mode == 0) {
                tempea = disp32;
            } else {
                tempea = CPU_EBP;
                base_uses_ss = 1;
            }
            break;
    }

    if (mode == 1 || mode == 2) {
        tempea += disp32;
    }

    if (base_uses_ss && !segoverride) {
        useseg = CPU_SS;
    }

    return tempea;
}

__not_in_flash() void getea(uint8_t rmval) {
    register uint32_t tempea = 0;
#ifdef CPU_386_EXTENDED_OPS
    if (addressSizeOverride) {
        tempea = r36sx_cpu_ea32(rmval);
        ea = r36sx_cpu_linear_ea(tempea);
        return;
    }
#endif
    switch (mode) {
        case 0:
            switch (rmval) {
                case 0: tempea = CPU_BX + CPU_SI;
                    break;
                case 1: tempea = CPU_BX + CPU_DI;
                    break;
                case 2: tempea = CPU_BP + CPU_SI;
                    break;
                case 3: tempea = CPU_BP + CPU_DI;
                    break;
                case 4: tempea = CPU_SI;
                    break;
                case 5: tempea = CPU_DI;
                    break;
                case 6: tempea = disp16;
                    break;
                case 7: tempea = CPU_BX;
                    break;
            }
            break;

        case 1:
        case 2:
            switch (rmval) {
                case 0: tempea = CPU_BX + CPU_SI + disp16;
                    break;
                case 1: tempea = CPU_BX + CPU_DI + disp16;
                    break;
                case 2: tempea = CPU_BP + CPU_SI + disp16;
                    break;
                case 3: tempea = CPU_BP + CPU_DI + disp16;
                    break;
                case 4: tempea = CPU_SI + disp16;
                    break;
                case 5: tempea = CPU_DI + disp16;
                    break;
                case 6: tempea = CPU_BP + disp16;
                    break;
                case 7: tempea = CPU_BX + disp16;
                    break;
            }
            break;
    }
    ea = r36sx_cpu_linear_ea(tempea & 0xffffu);
}

static INLINE void push(uint16_t pushval) {
    CPU_SP = CPU_SP - 2;
    putmem16(CPU_SS, CPU_SP, pushval);
}

static INLINE uint16_t pop() {
    uint16_t tempval = getmem16(CPU_SS, CPU_SP);
    CPU_SP = CPU_SP + 2;
    return tempval;
}

static INLINE void push32(uint32_t pushval) {
    CPU_SP = CPU_SP - 4;
    putmem32(CPU_SS, CPU_SP, pushval);
}

static INLINE uint32_t pop32(void) {
    uint32_t tempval = getmem32(CPU_SS, CPU_SP);
    CPU_SP = CPU_SP + 4;
    return tempval;
}

static INLINE uint32_t readrm32(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        return readdw86(ea);
    }
    return getreg32(rmval);
}

static INLINE uint16_t readrm16(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        return readw86(ea);
    }
    return getreg16(rmval);
}

static INLINE uint8_t readrm8(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        return read86(ea);
    }
    return getreg8(rmval);
}

static INLINE void writerm16(uint8_t rmval, uint16_t value) {
    if (mode < 3) {
        getea(rmval);
        writew86(ea, value);
    } else {
        putreg16(rmval, value);
    }
}

static INLINE void writerm32(uint8_t rmval, uint32_t value) {
    if (mode < 3) {
        getea(rmval);
        writedw86(ea, value);
    } else {
        putreg32(rmval, value);
    }
}

static INLINE void writerm8(uint8_t rmval, uint8_t value) {
    if (mode < 3) {
        getea(rmval);
        write86(ea, value);
    } else {
        putreg8(rmval, value);
    }
}

static INLINE uint16_t makeflagsword(void) {
#if CPU_386_EXTENDED_OPS
    return (uint16_t)(2u | x86_flags.value);
#else
    return 2 | (x86_flags.value & 0b111111010101);
#endif
}

static INLINE void decodeflagsword(uint16_t x) {
    x86_flags.value = x;
}

static INLINE uint32_t makeflagsdword(void) {
    /* 80386 EFLAGS: expose 386-era status/control bits, but not 486+ AC or CPUID ID. */
    return 2u | (x86_flags.value & 0x00037FD7u);
}

static INLINE void decodeflagsdword(uint32_t x) {
    x86_flags.value = 2u | (x & 0x00037FD7u);
}

#define R36SX_BIOS_TEXT_BASE 0x8000u
#define R36SX_BIOS_TEXT_PAGE_CELLS 0x1000u
#define R36SX_BIOS_MAX_TEXT_COLS 80u
#define R36SX_BIOS_MAX_TEXT_ROWS 25u

static uint8_t r36sx_bios_active_page(void)
{
    return FIRST_RAM_PAGE[0x462] & 7u;
}

static uint32_t r36sx_bios_text_base(void)
{
    return videomode == 0x07 ? 0x0000u : R36SX_BIOS_TEXT_BASE;
}

static uint8_t r36sx_bios_text_cols(void)
{
    uint8_t cols = FIRST_RAM_PAGE[0x44A];
    if (videomode == 0x00 || videomode == 0x01) {
        return 40;
    }
    if (cols == 0 || cols > R36SX_BIOS_MAX_TEXT_COLS) {
        return 80;
    }
    return cols;
}

static uint8_t r36sx_bios_text_rows(void)
{
    uint8_t rows = FIRST_RAM_PAGE[0x484] + 1u;
    if (rows == 0 || rows > R36SX_BIOS_MAX_TEXT_ROWS) {
        return 25;
    }
    return rows;
}

static void r36sx_bios_set_cursor(uint8_t page, uint8_t col, uint8_t row)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    page &= 7u;
    if (col >= cols) {
        col = cols - 1u;
    }
    if (row >= rows) {
        row = rows - 1u;
    }

    FIRST_RAM_PAGE[0x450 + page * 2u] = col;
    FIRST_RAM_PAGE[0x451 + page * 2u] = row;
    if (page == r36sx_bios_active_page()) {
        uint8_t old_x = CURSOR_X;
        uint8_t old_y = CURSOR_Y;
        CURSOR_X = col;
        CURSOR_Y = row;
        if (old_x != CURSOR_X || old_y != CURSOR_Y) {
            r36sx_pico286_video_mark_dirty();
        }
    }
}

static uint32_t r36sx_bios_text_index(uint8_t page, uint8_t col, uint8_t row)
{
    const uint32_t stride = (uint32_t)r36sx_bios_text_cols() * 2u;
    return r36sx_bios_text_base() +
           (uint32_t)(page & 7u) * R36SX_BIOS_TEXT_PAGE_CELLS +
           (uint32_t)row * stride +
           (uint32_t)col * 2u;
}

static uint8_t r36sx_bios_read_text_attr(uint8_t page, uint8_t col,
                                         uint8_t row)
{
    uint32_t index = r36sx_bios_text_index(page, col, row) + 1u;
    uint8_t attr = 0x07;
    if (index < VIDEORAM_SIZE) {
        attr = (uint8_t)(VIDEORAM[index] & 0xffu);
    }
    return attr ? attr : 0x07;
}

static void r36sx_bios_write_text_cell(uint8_t page, uint8_t col, uint8_t row,
                                       uint8_t ch, uint8_t attr)
{
    uint32_t index = r36sx_bios_text_index(page, col, row);
    if (index + 1u >= VIDEORAM_SIZE) {
        return;
    }
    if (VIDEORAM[index] != ch || VIDEORAM[index + 1u] != attr) {
        VIDEORAM[index] = ch;
        VIDEORAM[index + 1u] = attr;
        r36sx_pico286_video_mark_dirty();
    }
}

static void r36sx_bios_clear_text_window(uint8_t page, uint8_t top,
                                         uint8_t left, uint8_t bottom,
                                         uint8_t right, uint8_t attr)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    if (top >= rows) top = rows - 1u;
    if (bottom >= rows) bottom = rows - 1u;
    if (left >= cols) left = cols - 1u;
    if (right >= cols) right = cols - 1u;
    if (bottom < top || right < left) {
        return;
    }

    for (uint8_t row = top; row <= bottom; row++) {
        for (uint8_t col = left; col <= right; col++) {
            r36sx_bios_write_text_cell(page, col, row, ' ', attr);
        }
    }
}

static void r36sx_bios_scroll_text_window(uint8_t page, uint8_t top,
                                          uint8_t left, uint8_t bottom,
                                          uint8_t right, uint8_t lines,
                                          uint8_t attr, int direction)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    if (top >= rows) top = rows - 1u;
    if (bottom >= rows) bottom = rows - 1u;
    if (left >= cols) left = cols - 1u;
    if (right >= cols) right = cols - 1u;
    if (bottom < top || right < left) {
        return;
    }

    int changed = 0;
    uint8_t height = (uint8_t)(bottom - top + 1u);
    if (lines == 0 || lines >= height) {
        r36sx_bios_clear_text_window(page, top, left, bottom, right, attr);
        return;
    }

    if (direction > 0) {
        for (uint8_t row = top; row <= (uint8_t)(bottom - lines); row++) {
            for (uint8_t col = left; col <= right; col++) {
                uint32_t src = r36sx_bios_text_index(page, col, row + lines);
                uint32_t dst = r36sx_bios_text_index(page, col, row);
                if (src + 1u < VIDEORAM_SIZE && dst + 1u < VIDEORAM_SIZE) {
                    changed |= VIDEORAM[dst] != VIDEORAM[src] ||
                               VIDEORAM[dst + 1u] != VIDEORAM[src + 1u];
                    VIDEORAM[dst] = VIDEORAM[src];
                    VIDEORAM[dst + 1u] = VIDEORAM[src + 1u];
                }
            }
        }
        r36sx_bios_clear_text_window(page, (uint8_t)(bottom - lines + 1u),
                                     left, bottom, right, attr);
    } else {
        for (int row = bottom; row >= (int)top + lines; row--) {
            for (uint8_t col = left; col <= right; col++) {
                uint32_t src = r36sx_bios_text_index(page, col,
                                                     (uint8_t)(row - lines));
                uint32_t dst = r36sx_bios_text_index(page, col, (uint8_t)row);
                if (src + 1u < VIDEORAM_SIZE && dst + 1u < VIDEORAM_SIZE) {
                    changed |= VIDEORAM[dst] != VIDEORAM[src] ||
                               VIDEORAM[dst + 1u] != VIDEORAM[src + 1u];
                    VIDEORAM[dst] = VIDEORAM[src];
                    VIDEORAM[dst + 1u] = VIDEORAM[src + 1u];
                }
            }
        }
        r36sx_bios_clear_text_window(page, top, left, (uint8_t)(top + lines - 1u),
                                     right, attr);
    }
    if (changed) {
        r36sx_pico286_video_mark_dirty();
    }
}

static void r36sx_bios_teletype(uint8_t page, uint8_t ch, uint8_t attr)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();
    uint8_t col = FIRST_RAM_PAGE[0x450 + (page & 7u) * 2u];
    uint8_t row = FIRST_RAM_PAGE[0x451 + (page & 7u) * 2u];

    if (col >= cols) col = 0;
    if (row >= rows) row = rows - 1u;
    if (attr == 0) {
        attr = r36sx_bios_read_text_attr(page, col, row);
    }

    if (ch == '\a') {
        return;
    } else if (ch == '\b') {
        if (col > 0) {
            col--;
        }
    } else if (ch == '\r') {
        col = 0;
    } else if (ch == '\n') {
        row++;
    } else if (ch == '\t') {
        do {
            r36sx_bios_teletype(page, ' ', attr);
            col = FIRST_RAM_PAGE[0x450 + (page & 7u) * 2u];
        } while ((col & 7u) != 0);
        return;
    } else {
        r36sx_bios_write_text_cell(page, col, row, ch, attr);
        col++;
        if (col >= cols) {
            col = 0;
            row++;
        }
    }

    if (row >= rows) {
        r36sx_bios_scroll_text_window(page, 0, 0, rows - 1u, cols - 1u, 1,
                                      attr, 1);
        row = rows - 1u;
    }
    r36sx_bios_set_cursor(page, col, row);
}

#define R36SX_VBE_STATUS_OK 0x004Fu
#define R36SX_VBE_STATUS_FAIL 0x014Fu
#define R36SX_VBE_WINDOW_KB 64u

static void r36sx_vbe_write8(uint32_t base, uint16_t offset, uint8_t value)
{
    write86(base + offset, value);
}

static void r36sx_vbe_write16(uint32_t base, uint16_t offset, uint16_t value)
{
    writew86(base + offset, value);
}

static void r36sx_vbe_write32(uint32_t base, uint16_t offset, uint32_t value)
{
    write86(base + offset, (uint8_t)(value & 0xFFu));
    write86(base + offset + 1u, (uint8_t)((value >> 8) & 0xFFu));
    write86(base + offset + 2u, (uint8_t)((value >> 16) & 0xFFu));
    write86(base + offset + 3u, (uint8_t)((value >> 24) & 0xFFu));
}

static void r36sx_vbe_clear(uint32_t base, uint16_t bytes)
{
    for (uint16_t i = 0; i < bytes; i++) {
        write86(base + i, 0);
    }
}

static uint32_t r36sx_vbe_far_ptr(uint32_t linear)
{
    return ((linear >> 4) << 16) | (linear & 0x0Fu);
}

static uint16_t r36sx_vbe_mode_base(uint16_t mode)
{
    return mode & 0x01FFu;
}

static uint16_t r36sx_vbe_bytes_per_scanline(uint16_t mode)
{
    return (uint16_t)(SVGA_WIDTH *
        (r36sx_vbe_mode_base(mode) == VBE_MODE_800X600X16 ? 2u : 1u));
}

static void r36sx_vbe_write_info_block(void)
{
    uint32_t base = (uint32_t)CPU_ES * 16u + CPU_DI;
    uint32_t modes = base + 0x100u;

    r36sx_vbe_clear(base, 512);
    r36sx_vbe_write8(base, 0x00, 'V');
    r36sx_vbe_write8(base, 0x01, 'E');
    r36sx_vbe_write8(base, 0x02, 'S');
    r36sx_vbe_write8(base, 0x03, 'A');
    r36sx_vbe_write16(base, 0x04, 0x0200u);
    r36sx_vbe_write32(base, 0x0A, 0u);
    r36sx_vbe_write32(base, 0x0E, r36sx_vbe_far_ptr(modes));
    r36sx_vbe_write16(base, 0x12,
                      (uint16_t)((SVGA_VRAM_SIZE + 0xFFFFu) >> 16));

    r36sx_vbe_write16(modes, 0x00, VBE_MODE_800X600X8);
    r36sx_vbe_write16(modes, 0x02, VBE_MODE_800X600X16);
    r36sx_vbe_write16(modes, 0x04, 0xFFFFu);
    CPU_AX = R36SX_VBE_STATUS_OK;
}

static void r36sx_vbe_write_mode_info(uint16_t mode)
{
    uint32_t base = (uint32_t)CPU_ES * 16u + CPU_DI;
    uint16_t bytes_per_scanline;
    uint32_t mode_bytes;
    uint8_t bpp;
    uint8_t pages;

    mode = r36sx_vbe_mode_base(mode);
    if (!vga_svga_mode_supported(mode)) {
        CPU_AX = R36SX_VBE_STATUS_FAIL;
        return;
    }

    bytes_per_scanline = r36sx_vbe_bytes_per_scanline(mode);
    mode_bytes = (uint32_t)bytes_per_scanline * SVGA_HEIGHT;
    bpp = mode == VBE_MODE_800X600X16 ? 16 : 8;
    pages = (uint8_t)((SVGA_VRAM_SIZE / mode_bytes) > 0 ?
                      (SVGA_VRAM_SIZE / mode_bytes) - 1u : 0u);

    r36sx_vbe_clear(base, 256);
    r36sx_vbe_write16(base, 0x00, 0x0019u);
    r36sx_vbe_write8(base, 0x02, 0x07u);
    r36sx_vbe_write8(base, 0x03, 0x07u);
    r36sx_vbe_write16(base, 0x04, R36SX_VBE_WINDOW_KB);
    r36sx_vbe_write16(base, 0x06, R36SX_VBE_WINDOW_KB);
    r36sx_vbe_write16(base, 0x08, 0xA000u);
    r36sx_vbe_write16(base, 0x0A, 0xA000u);
    r36sx_vbe_write32(base, 0x0C, 0u);
    r36sx_vbe_write16(base, 0x10, bytes_per_scanline);
    r36sx_vbe_write16(base, 0x12, SVGA_WIDTH);
    r36sx_vbe_write16(base, 0x14, SVGA_HEIGHT);
    r36sx_vbe_write8(base, 0x16, 8u);
    r36sx_vbe_write8(base, 0x17, 16u);
    r36sx_vbe_write8(base, 0x18, 1u);
    r36sx_vbe_write8(base, 0x19, bpp);
    r36sx_vbe_write8(base, 0x1A, (uint8_t)((mode_bytes + 0xFFFFu) >> 16));
    r36sx_vbe_write8(base, 0x1B, bpp == 16 ? 6u : 4u);
    r36sx_vbe_write8(base, 0x1C, R36SX_VBE_WINDOW_KB);
    r36sx_vbe_write8(base, 0x1D, pages);
    if (bpp == 16) {
        r36sx_vbe_write8(base, 0x1F, 5u);
        r36sx_vbe_write8(base, 0x20, 11u);
        r36sx_vbe_write8(base, 0x21, 6u);
        r36sx_vbe_write8(base, 0x22, 5u);
        r36sx_vbe_write8(base, 0x23, 5u);
        r36sx_vbe_write8(base, 0x24, 0u);
        r36sx_vbe_write8(base, 0x25, 0u);
        r36sx_vbe_write8(base, 0x26, 0u);
    }
    CPU_AX = R36SX_VBE_STATUS_OK;
}

static void r36sx_vbe_set_mode(uint16_t mode)
{
    uint16_t base_mode = r36sx_vbe_mode_base(mode);
    int clear_memory = (mode & 0x8000u) == 0;

    if ((mode & 0x4000u) != 0) {
        CPU_AX = R36SX_VBE_STATUS_FAIL;
        return;
    }
    if (!vga_svga_set_mode(base_mode, clear_memory)) {
        CPU_AX = R36SX_VBE_STATUS_FAIL;
        return;
    }

    videomode = base_mode;
    vga_planar_mode = 0;
    vga_plane_offset = 0;
    vram_offset = 0;
    FIRST_RAM_PAGE[0x449] = 0x7Fu;
    FIRST_RAM_PAGE[0x44A] = 80;
    FIRST_RAM_PAGE[0x44B] = 0;
    FIRST_RAM_PAGE[0x484] = 24;
    FIRST_RAM_PAGE[0x462] = 0;
    r36sx_bios_set_cursor(0, 0, 0);
    r36sx_pico286_video_mark_dirty();
    CPU_AX = R36SX_VBE_STATUS_OK;
}

static void r36sx_vbe_banked_window(void)
{
    switch (CPU_BH) {
        case 0x00:
            if ((CPU_BL & 1u) == 0) {
                vga_svga_set_bank(CPU_DX);
                CPU_AX = R36SX_VBE_STATUS_OK;
            } else {
                CPU_AX = R36SX_VBE_STATUS_FAIL;
            }
            return;
        case 0x01:
            CPU_DX = vga_svga_get_bank();
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        default:
            CPU_AX = R36SX_VBE_STATUS_FAIL;
            return;
    }
}

static void r36sx_bios_vesa(void)
{
    switch (CPU_AL) {
        case 0x00:
            r36sx_vbe_write_info_block();
            return;
        case 0x01:
            r36sx_vbe_write_mode_info(CPU_CX);
            return;
        case 0x02:
            r36sx_vbe_set_mode(CPU_BX);
            return;
        case 0x03:
            CPU_BX = vga_svga_mode_active() ? (uint16_t)videomode
                                            : (uint16_t)(videomode & 0xFFu);
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        case 0x05:
            r36sx_vbe_banked_window();
            return;
        default:
            CPU_AX = R36SX_VBE_STATUS_FAIL;
            return;
    }
}

#if !PICO_ON_DEVICE
static uint8_t r36sx_bios_drive_to_slot(uint8_t bios_drive)
{
    return (bios_drive & 0x80) ? (uint8_t)(bios_drive - 126u) : bios_drive;
}

static void r36sx_bios_attach_configured_disks(void)
{
    const char *fdd0_path = r36sx_pico286_disk_path(0, "fdd0.img");
    const char *fdd1_path = r36sx_pico286_disk_path(1, "fdd1.img");
    const char *hdd0_path = r36sx_pico286_disk_path(128, "hdd.img");
    const char *hdd1_path = r36sx_pico286_disk_path(129, "hdd2.img");
    uint8_t fdd0_ok = fdd0_path[0] ? insertdisk(0, fdd0_path) : 0;
    uint8_t fdd1_ok = fdd1_path[0] ? insertdisk(1, fdd1_path) : 0;
    uint8_t hdd0_ok = hdd0_path[0] ? insertdisk(128, hdd0_path) : 0;
    uint8_t hdd1_ok = hdd1_path[0] ? insertdisk(129, hdd1_path) : 0;

    r36sx_pico286_debug_log(
        "cpu: int19 disk attach fdd0=%u '%s' fdd1=%u '%s' hdd0=%u '%s' hdd1=%u '%s'",
        fdd0_ok, fdd0_path[0] ? fdd0_path : "<disabled>",
        fdd1_ok, fdd1_path[0] ? fdd1_path : "<disabled>",
        hdd0_ok, hdd0_path[0] ? hdd0_path : "<disabled>",
        hdd1_ok, hdd1_path[0] ? hdd1_path : "<disabled>");
}

static int r36sx_bios_try_boot_drive(uint8_t bios_drive)
{
    uint8_t slot = r36sx_bios_drive_to_slot(bios_drive);
    uint8_t boot_sector[512];

    if (slot >= 4 || !disk[slot].inserted || !disk[slot].diskfile) {
        r36sx_pico286_debug_log(
            "cpu: boot_order skip drive 0x%02x not inserted", bios_drive);
        return 0;
    }

    disk_flush_drive(slot, "boot-sector-read");
    if (r36sx_host_disk_read_at(disk[slot].diskfile, 0, boot_sector,
                                sizeof(boot_sector)) != 0) {
        r36sx_pico286_debug_log(
            "cpu: boot_order failed reading drive 0x%02x", bios_drive);
        return 0;
    }

    if (boot_sector[510] != 0x55 || boot_sector[511] != 0xaa) {
        r36sx_pico286_debug_log(
            "cpu: boot_order drive 0x%02x has no boot signature", bios_drive);
        return 0;
    }

    for (uint16_t i = 0; i < sizeof(boot_sector); i++) {
        write86(0x7c00u + i, boot_sector[i]);
    }

    r36sx_cpu_load_segment(regcs, 0x0000);
    CPU_IP = 0x7c00;
    CPU_DL = bios_drive;
    CPU_FL_CF = 0;
    r36sx_pico286_debug_log(
        "cpu: boot_order booting drive 0x%02x at 0000:7c00", bios_drive);
    return 1;
}

static int r36sx_bios_boot_configured_order(void)
{
    uint8_t order[4];
    uint8_t count = r36sx_pico286_boot_order(order, (uint8_t)sizeof(order));

    for (uint8_t i = 0; i < count; i++) {
        if (r36sx_bios_try_boot_drive(order[i])) {
            return 1;
        }
    }

    return 0;
}
#endif

static uint8_t r36sx_cpu_protected_interrupt(uint8_t intnum)
{
    uint32_t gate_offset = (uint32_t)intnum * 8u;
    if (gate_offset + 7u > r36sx_idtr_limit) {
#if DEBUG && !PICO_ON_DEVICE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt IDT limit fault int=%02x idtr=%08lx:%04x",
            intnum, (unsigned long)r36sx_idtr_base, r36sx_idtr_limit);
#endif
        return 0;
    }

    uint32_t addr = r36sx_idtr_base + gate_offset;
    uint32_t lo = readdw86(addr);
    uint32_t hi = readdw86(addr + 4u);
    uint8_t access = (uint8_t)((hi >> 8) & 0xffu);
    uint8_t type = access & 0x0fu;

    if ((access & R36SX_DESCRIPTOR_PRESENT) == 0 ||
        !(type == 0x06u || type == 0x07u ||
          type == 0x0eu || type == 0x0fu)) {
#if DEBUG && !PICO_ON_DEVICE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt unsupported gate int=%02x access=%02x",
            intnum, access);
#endif
        return 0;
    }

    uint16_t selector = (uint16_t)(lo >> 16);
    uint32_t offset = (lo & 0xffffu) |
                      ((type >= 0x0eu) ? (hi & 0xffff0000u) : 0u);

    if (type >= 0x0eu) {
        push32(makeflagsdword());
        push32(CPU_CS);
        push32(CPU_IP);
    } else {
        push(makeflagsword());
        push(CPU_CS);
        push(CPU_IP);
    }

    r36sx_cpu_load_segment(regcs, selector);
    CPU_IP = (uint16_t)offset;
    if (type == 0x06u || type == 0x0eu) {
        ifl = 0;
    }
    tf = 0;
    return 1;
}

void intcall86(uint8_t intnum) {
    if (r36sx_cpu_protected_enabled() &&
        r36sx_cpu_protected_interrupt(intnum)) {
        return;
    }

    switch (intnum) {
        case 0x10: {
            switch (CPU_AH) {
                case 0x4F:
                    r36sx_bios_vesa();
                    return;
                case 0x01:
                    if (cursor_start != CPU_CH || cursor_end != CPU_CL) {
                        cursor_start = CPU_CH;
                        cursor_end = CPU_CL;
                        r36sx_pico286_video_mark_dirty();
                    }
                    FIRST_RAM_PAGE[0x460] = CPU_CH;
                    FIRST_RAM_PAGE[0x461] = CPU_CL;
                    return;
                case 0x02:
                    r36sx_bios_set_cursor(CPU_BH, CPU_DL, CPU_DH);
                    return;
                case 0x03:
                    CPU_CH = cursor_start;
                    CPU_CL = cursor_end;
                    CPU_DH = FIRST_RAM_PAGE[0x451 + (CPU_BH & 7u) * 2u];
                    CPU_DL = FIRST_RAM_PAGE[0x450 + (CPU_BH & 7u) * 2u];
                    return;
                case 0x06:
                    r36sx_bios_scroll_text_window(r36sx_bios_active_page(),
                                                  CPU_CH, CPU_CL, CPU_DH,
                                                  CPU_DL, CPU_AL, CPU_BH, 1);
                    return;
                case 0x07:
                    r36sx_bios_scroll_text_window(r36sx_bios_active_page(),
                                                  CPU_CH, CPU_CL, CPU_DH,
                                                  CPU_DL, CPU_AL, CPU_BH, -1);
                    return;
                case 0x08: {
                    uint8_t page = CPU_BH & 7u;
                    uint8_t col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint32_t index = r36sx_bios_text_index(page, col, row);
                    CPU_AL = index < VIDEORAM_SIZE ? (uint8_t)(VIDEORAM[index] & 0xffu) : ' ';
                    CPU_AH = index + 1u < VIDEORAM_SIZE ? (uint8_t)(VIDEORAM[index + 1u] & 0xffu) : 0x07;
                    return;
                }
                case 0x09:
                case 0x0a: {
                    uint8_t page = CPU_BH & 7u;
                    uint8_t col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint8_t attr = CPU_AH == 0x09 ? CPU_BL :
                                   r36sx_bios_read_text_attr(page, col, row);
                    uint16_t count = CPU_CX ? CPU_CX : 1u;
                    for (uint16_t i = 0; i < count; i++) {
                        uint16_t pos = (uint16_t)col + i;
                        uint8_t write_col = (uint8_t)(pos % r36sx_bios_text_cols());
                        uint8_t write_row = (uint8_t)(row + pos / r36sx_bios_text_cols());
                        if (write_row < r36sx_bios_text_rows()) {
                            r36sx_bios_write_text_cell(page, write_col,
                                                       write_row, CPU_AL, attr);
                        }
                    }
                    if (videomode >= 8 && videomode <= 0x13) {
                        tga_draw_char(CPU_AL, CURSOR_X, CURSOR_Y, attr & 0x0f);
                    }
                    return;
                }
                case 0x0e:
                    if (videomode >= 8 && videomode <= 0x13) {
                        tga_draw_char(CPU_AL, CURSOR_X, CURSOR_Y, CPU_BL ? CPU_BL : 9);
                    }
                    r36sx_bios_teletype(CPU_BH, CPU_AL, CPU_BL);
                    return;
                case 0x0f:
                    CPU_AL = (uint8_t)videomode;
                    CPU_AH = r36sx_bios_text_cols();
                    CPU_BH = r36sx_bios_active_page();
                    return;
                case 0x13: {
                    uint8_t mode = CPU_AL;
                    uint8_t page = CPU_BH & 7u;
                    uint8_t attr = CPU_BL;
                    uint8_t old_col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t old_row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint32_t memloc = CPU_ES * 16u + CPU_BP;

                    r36sx_bios_set_cursor(page, CPU_DL, CPU_DH);
                    for (uint16_t i = 0; i < CPU_CX; i++) {
                        uint8_t ch = read86(memloc++);
                        if (mode & 0x02u) {
                            attr = read86(memloc++);
                        }
                        r36sx_bios_teletype(page, ch, attr);
                    }
                    if ((mode & 0x01u) == 0) {
                        r36sx_bios_set_cursor(page, old_col, old_row);
                    }
                    return;
                }
                case 0x00:
                    // http://www.techhelpmanual.com/114-video_modes.html
                    // http://www.techhelpmanual.com/89-video_memory_layouts.html

                    videomode = CPU_AL & 0x7F;

                    FIRST_RAM_PAGE[0x449] = CPU_AL;
                    FIRST_RAM_PAGE[0x44A] = videomode <= 1 || (videomode >= 0x8 && videomode <= 0xa) ? 40 : 80;
                    FIRST_RAM_PAGE[0x44B] = 0;
                    FIRST_RAM_PAGE[0x484] = (25 - 1);

                    if ((CPU_AL & 0x80) == 0x00) {
                        memset(VIDEORAM, 0x0, sizeof(VIDEORAM));
                    }
                    vga_plane_offset = 0;
                    vga_planar_mode = 0;
                    vga_svga_disable();
                    tga_offset = 0x8000;
                    FIRST_RAM_PAGE[0x462] = 0;
                    r36sx_bios_set_cursor(0, 0, 0);
                    r36sx_pico286_video_mark_dirty();
                    break;
                case 0x05: /* Select Active Page */ {
                    if (CPU_AL >= 0x80) {
                        uint8_t CRTCPU = FIRST_RAM_PAGE[BIOS_CRTCPU_PAGE];
                        switch (CPU_AL) {
                            case 0x80: /* read CRT/CPU page registers */
                                CPU_BH = CRTCPU & 7;
                                CPU_BL = (CRTCPU >> 3) & 7;
                                break;
                            case 0x81: /* set CPU page register to value in BL */
                                CRTCPU = (CRTCPU & 0xc7) | ((CPU_BL & 7) << 3);
                                break;
                            case 0x82: /* set CRT page register to value in BH */
                                CRTCPU = (CRTCPU & 0xf8) | (CPU_BH & 7);
                                break;
                            case 0x83: /* set CRT and CPU page registers in BH and BL */
                                CRTCPU = (CRTCPU & 0xc0) | (CPU_BH & 7) | ((CPU_BL & 7) << 3);
                                break;
                        }
                        tga_portout(0x3df, CRTCPU);
                        FIRST_RAM_PAGE[BIOS_CRTCPU_PAGE] = CRTCPU;
                        return;
                    }

                    FIRST_RAM_PAGE[0x462] = CPU_AL & 7u;
                    r36sx_bios_set_cursor(FIRST_RAM_PAGE[0x462],
                                          FIRST_RAM_PAGE[0x450 + FIRST_RAM_PAGE[0x462] * 2u],
                                          FIRST_RAM_PAGE[0x451 + FIRST_RAM_PAGE[0x462] * 2u]);
                    r36sx_pico286_video_mark_dirty();
                    return;
                }
                case 0x10:
                    switch (CPU_AL) {
                        case 0x00: {
                            uint8_t color_index = CPU_BL & 0xF;
                            uint8_t color_byte = CPU_BH;

                            const uint16_t r = (((color_byte >> 2) & 1) << 1) + (color_byte >> 5 & 1);
                            const uint16_t g = (((color_byte >> 1) & 1) << 1) + (color_byte >> 4 & 1);
                            const uint16_t b = (((color_byte >> 0) & 1) << 1) + (color_byte >> 3 & 1);

                            if (videomode <= 0xa) {
                                if (tga_palette_map[color_index] != color_byte) {
                                    tga_palette_map[color_index] = color_byte;
                                    r36sx_pico286_video_mark_dirty();
                                }
                            } else {
                                uint32_t color = rgb((r * 85), (g * 85), (b * 85));
                                if (vga_palette[color_index] != color) {
                                    vga_palette[color_index] = color;
                                    r36sx_pico286_video_mark_dirty();
                                }
#if PICO_ON_DEVICE
                                graphics_set_palette(color_index, vga_palette[color_index]);
#endif
                            }
                            return;
                        }
                        case 0x02: {
                            uint32_t memloc = CPU_ES * 16 + CPU_DX;
                            for (int color_index = 0; color_index < 16; color_index++) {
                                uint8_t color_byte = read86(memloc++);
                                const uint8_t r = (((color_byte >> 2) & 1) << 1) + (color_byte >> 5 & 1);
                                const uint8_t g = (((color_byte >> 1) & 1) << 1) + (color_byte >> 4 & 1);
                                const uint8_t b = (((color_byte >> 0) & 1) << 1) + (color_byte >> 3 & 1);

                                uint32_t color = rgb((r * 85), (g * 85), (b * 85));
                                if (vga_palette[color_index] != color) {
                                    vga_palette[color_index] = color;
                                    r36sx_pico286_video_mark_dirty();
                                }
#if PICO_ON_DEVICE
                                graphics_set_palette(color_index, vga_palette[color_index]);
#endif
                            }
                            // TODO: Overscan/Border 17th color
                            return;
                        }
                        case 0x03: {
                            uint8_t old_blinking = cga_blinking;
                            cga_blinking = CPU_BL ? 0x7F : 0xFF;
                            cga_blinking_lock = !CPU_BL;
                            if (old_blinking != cga_blinking) {
                                r36sx_pico286_video_mark_dirty();
                            }
                            //printf("[CPU] INT BL 0x%02x\r\n", CPU_BL);
                            return;
                        }
                        case 0x10: {
                            // Set One DAC Color Register
                            vga_set_dac_color(CPU_BL, CPU_DH, CPU_CH, CPU_CL);
                            return;
                        }
                        case 0x12: {
                            // set block of DAC color registers               VGA
                            uint32_t memloc = CPU_ES * 16 + CPU_DX;
                            for (uint16_t i = 0; i < CPU_CX; i++) {
                                uint8_t color_index = (uint8_t)(CPU_BX + i);
                                uint8_t red = read86(memloc++);
                                uint8_t green = read86(memloc++);
                                uint8_t blue = read86(memloc++);
                                vga_set_dac_color(color_index, red, green, blue);
                            }
                            return;
                        }
                        case 0x15: {
                            // Read One DAC Color Register
                            const uint8_t color_index = CPU_BX & 0xFF;
                            vga_get_dac_color(color_index, &CPU_DH, &CPU_CH, &CPU_CL);
                            return;
                        }
                        case 0x17: {
                            // Read a Block of DAC Color Registers
                            uint32_t memloc = CPU_ES * 16 + CPU_DX;
                            for (uint16_t i = 0; i < CPU_CX; i++) {
                                uint8_t color_index = (uint8_t)(CPU_BX + i);
                                uint8_t red, green, blue;
                                vga_get_dac_color(color_index, &red, &green, &blue);
                                write86(memloc++, red);
                                write86(memloc++, green);
                                write86(memloc++, blue);
                            }
                            return;
                        }
                    }
                    //printf("Unhandled 10h CPU_AL: 0x%x\r\n", CPU_AL);
                    break;
                case 0x1A: //get display combination code (ps, vga/mcga)
                    CPU_AL = 0x1A;
                    if (ega_vga_enabled) {
                        CPU_BL = 0x08;
                    } else {
                        CPU_BL = 0x05; // MCGA
                    }
                    return;
            }
            break;
        }
        case 0x13:
            return diskhandler();
        case 0x15: /* XMS */
            switch (CPU_AH) {
                case 0x87: {
                    //https://github.com/neozeed/himem.sys-2.06/blob/5761f4fc182543b3964fd0d3a236d04bac7bfb50/oemsrc/himem.asm#L690
                    //                    printf("mem move?! %x %x:%x\n", CPU_CX, CPU_ES, CPU_SI);
                    CPU_AX = 0;
                    return;
                }
                    return;
                case 0x88: {
                    CPU_AX = (uint16_t)r36sx_pico286_extended_memory_kb();
                    return;
                }
            }
            break;
        /**/
        case 0x19:
#if PICO_ON_DEVICE
            insertdisk(0, "\\XT\\fdd0.img");
            insertdisk(1, "\\XT\\fdd1.img");
            insertdisk(128, "\\XT\\hdd.img");
            insertdisk(129, "\\XT\\hdd2.img");
#else
            {
                if (r36sx_pico286_boot_bios_prompt()) {
                    r36sx_pico286_debug_log(
                        "cpu: int19 boot_mode=bios_prompt; disks left detached");
                } else {
                    r36sx_bios_attach_configured_disks();
                }
            }
#endif
            if (1) {
                /* PCjr reserves the top of its internal 128KB of RAM for video RAM.  * Sidecars can extend it past 128KB but it
                 * requires DOS drivers or TSRs to modify the MCB chain so that it a) marks the video memory as reserved and b)
                 * creates a new free region above the video RAM region.
                 *
                 * Therefore, only subtract 16KB if 128KB or less is configured for this machine.
                 *
                 * Note this is not speculation, it's there in the PCjr BIOS source code:
                 * [http://hackipedia.org/browse.cgi/Computer/Platform/PC%2c%20IBM%20compatible/Video/PCjr/IBM%20Personal%20Computer%20PCjr%20Hardware%20Reference%20Library%20Technical%20Reference%20%281983%2d11%29%20First%20Edition%20Revised%2epdf]
                 * ROM BIOS source code page A-16 */

                {
                    uint32_t conventional_kb =
                        r36sx_pico286_conventional_memory_kb();
                    if (conventional_kb > 640u) {
                        conventional_kb = 640u;
                    }
                    writew86(BIOS_MEMORY_SIZE, (uint16_t)conventional_kb);
                    writew86(BIOS_TRUE_MEMORY_SIZE,
                             (uint16_t)(conventional_kb > 16u ?
                                            conventional_kb - 16u :
                                            conventional_kb));
                }
#if !PICO_ON_DEVICE
                time_t uts = time(NULL);
                struct tm *t = localtime(&uts);

                writew86(0x46E, t->tm_hour); // Hour bcd
                writew86(0x46C, t->tm_min * 1092 + t->tm_sec * 18); // minute + second
#endif
            }
#if !PICO_ON_DEVICE
            if (!r36sx_pico286_boot_bios_prompt() &&
                r36sx_bios_boot_configured_order()) {
                return;
            }
#endif
            break;
        case 0x1A: /* Timer I/O RTC */
            switch (CPU_AH) {
                case 0x02: /* 02H: Read Time from Real-Time Clock */
                    CPU_CX = 0x2259;
                    CPU_DX = 0x0001;
                    CPU_FL_CF = 0;
                    return;
                case 0x04: /* 04H: Read Date from Real-Time Clock */
                    CPU_CX = 0x2024;
                    CPU_DX = 0x1024;
                    CPU_AH = 0;
                    CPU_FL_CF = 0;
                    return;
            }
            break;
        case 0x2F: /* Multiplex Interrupt */
            switch (CPU_AX) {
                /* XMS */
                case 0x4300:
                    CPU_AL = 0x80;
                    return;
                case 0x4310: {
                    r36sx_cpu_load_segment(reges, XMS_FN_CS); // to be handled by DOS memory manager using
                    CPU_BX = XMS_FN_IP; // CALL FAR ES:BX
                    return;
                default:
                    if (redirector_handler()) {
                        return;
                    }
                }
            }
            break;
    }

    push(makeflagsword());
    push(CPU_CS);
    push(ip);
    r36sx_cpu_load_segment(regcs, getmem16(0, (uint16_t) intnum * 4 + 2));
    ip = getmem16(0, (uint16_t) intnum * 4);
    ifl = 0;
    tf = 0;
}

static inline void r36sx_cpu_invalid_opcode(uint16_t fault_ip)
{
#if DEBUG
    r36sx_pico286_debug_log(
        "[CPU] INT6 invalid opcode at %04X:%04X bytes=%02X %02X %02X %02X %02X %02X %02X %02X flags=%04X ax=%04X bx=%04X cx=%04X dx=%04X si=%04X di=%04X bp=%04X sp=%04X ds=%04X es=%04X ss=%04X",
        CPU_CS, fault_ip,
        getmem8(CPU_CS, fault_ip),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 1u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 2u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 3u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 4u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 5u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 6u)),
        getmem8(CPU_CS, (uint16_t)(fault_ip + 7u)),
        (uint16_t)(2u | x86_flags.value),
        CPU_AX, CPU_BX, CPU_CX, CPU_DX,
        CPU_SI, CPU_DI, CPU_BP, CPU_SP,
        CPU_DS, CPU_ES, CPU_SS);
#endif
    CPU_IP = fault_ip;
    intcall86(6);
}

static inline void flag_szp8(uint8_t value) {
    zf = value == 0;
    sf = value >> 7;
    pf = parity[value];
}

static inline void flag_szp16(uint16_t value) {
    zf = value == 0;
    sf = value >> 15;
    pf = parity[value & 255];
}

static inline void flag_szp32(uint32_t value) {
    zf = value == 0;
    sf = value >> 31;
    pf = parity[value & 255];
}

static inline void flag_log8(uint8_t value) {
    flag_szp8(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_log16(uint16_t value) {
    flag_szp16(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_log32(uint32_t value) {
    flag_szp32(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_adc8(uint8_t v1, uint8_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    uint32_t dst = (uint32_t) v1 + (uint32_t) v2 + (uint32_t) v3;
    flag_szp8((uint8_t) dst);
    of = ((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2) & 0x80) != 0;
    cf = (dst & 0xFF00) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_adc16(uint16_t v1, uint16_t v2, uint16_t v3) {
    register uint32_t dst = (uint32_t) v1 + (uint32_t) v2 + (uint32_t) v3;
    flag_szp16((uint16_t) dst);
    of = (((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2)) & 0x8000) != 0;
    cf = (dst & 0xFFFF0000) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_adc32(uint32_t v1, uint32_t v2, uint8_t v3) {
    uint64_t dst64 = (uint64_t)v1 + (uint64_t)v2 + (uint64_t)v3;
    uint32_t dst = (uint32_t)dst64;
    flag_szp32(dst);
    of = ((dst ^ v1) & (dst ^ v2) & 0x80000000u) != 0;
    cf = (dst64 >> 32) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

static inline void flag_add8(uint8_t v1, uint8_t v2) {
    /* v1 = destination operand, v2 = source operand */
    register uint32_t dst = (uint32_t) v1 + (uint32_t) v2;
    flag_szp8((uint8_t) dst);
    cf = (dst & 0xFF00) != 0;
    of = ((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2) & 0x80) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_add32(uint32_t v1, uint32_t v2, uint32_t res32) {
    /* v1 = destination operand, v2 = source operand */
    flag_szp32(res32);
    cf = (((uint64_t) v1 + (uint64_t) v2) >> 32) != 0;
    of = ((res32 ^ v1) & (res32 ^ v2) & 0x80000000u) != 0;
    af = ((v1 ^ v2 ^ res32) & 0x10) != 0;
}

static inline uint8_t sbb8(uint8_t v1, uint8_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint32_t dst = (uint32_t)v1 - (uint32_t)v2 - (uint32_t)v3;
    flag_szp8((uint8_t) dst);
    cf = ((dst >> 8) & 1) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint8_t)dst;
}

static inline uint16_t sbb16(uint16_t v1, uint16_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint32_t dst = (uint32_t)v1 - (uint32_t)v2 - (uint32_t)v3;
    flag_szp16((uint16_t) dst);
    cf = ((dst >> 16) & 1) != 0;
    of = ((dst ^ (uint32_t)v1) & (v1 ^ (uint32_t)v2) & 0x8000) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint16_t)dst;
}

static inline uint32_t sbb32(uint32_t v1, uint32_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint64_t dst = (uint64_t)v1 - (uint64_t)v2 - (uint64_t)v3;
    flag_szp32((uint32_t) dst);
    cf = ((dst >> 32) & 1) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80000000) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint32_t)dst;
}

static inline void flag_sub8(uint8_t v1, uint8_t v2) {
    /* v1 = destination operand, v2 = source operand */
    uint32_t dst = (uint32_t) v1 - (uint32_t) v2;
    flag_szp8((uint8_t) dst);
    cf = (dst & 0xFF00) != 0;
    of = ((dst ^ (uint32_t)v1) & (v1 ^ v2) & 0x80) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

static inline void flag_sub16(uint16_t v1, uint16_t v2) {
    /* v1 = destination operand, v2 = source operand */
    register uint32_t dst = (uint32_t) v1 - (uint32_t) v2;
    flag_szp16((uint16_t) dst);
    cf = (dst & 0xFFFF0000) != 0;
    of = ((dst ^ (uint32_t)v1) & ((uint32_t)v1 ^ (uint32_t)v2) & 0x8000) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_sub32(uint32_t v1, uint32_t v2) {
    uint64_t dst64 = (uint64_t)v1 - (uint64_t)v2;
    uint32_t dst = (uint32_t)dst64;
    flag_szp32(dst);
    cf = (dst64 >> 32) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80000000u) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

#define op_adc8() { res8 = oper1b + oper2b + cf; flag_adc8(oper1b, oper2b, cf); }
#define op_adc16() { res16 = oper1 + oper2 + cf; flag_adc16(oper1, oper2, cf); }
#define op_adc32() { res32 = oper1 + oper2 + cf; flag_adc32(oper1, oper2, cf); }
#define op_add8() { \
    register uint32_t dst = (uint32_t)oper1b + (uint32_t)oper2b; \
    res8 = dst; \
    flag_szp8(res8); \
    cf = (dst & 0xFF00) != 0; \
    of = ((dst ^ (uint32_t)oper1b) & (dst ^ (uint32_t)oper2b) & 0x80) != 0; \
    af = ((oper1b ^ oper2b ^ dst) & 0x10) != 0; \
}
#define op_add16() { \
    register uint32_t dst = (uint32_t)oper1 + (uint32_t)oper2; \
    res16 = dst; \
    flag_szp16(dst); \
    cf = (dst & 0xFFFF0000) != 0; \
    of = (((dst ^ (uint32_t)oper1) & (dst ^ (uint32_t)oper2) & 0x8000) != 0); \
    af = (((oper1 ^ oper2 ^ dst) & 0x10) != 0); \
}
#define op_add32() { res32 = oper1 + oper2; flag_add32(oper1, oper2, res32); }
#define op_and8() { res8 = oper1b & oper2b; flag_log8(res8); }
#define op_and16() { res16 = oper1 & oper2; flag_log16(res16); }
#define op_and32() { res32 = oper1 & oper2; flag_log32(res32); }
#define op_or8() { res8 = oper1b | oper2b; flag_log8(res8); }
#define op_or16() { res16 = oper1 | oper2; flag_log16(res16); }
#define op_or32() { res32 = oper1 | oper2; flag_log32(res32); }
#define op_xor8() { res8 = oper1b ^ oper2b; flag_log8(res8); }
#define op_xor16() { res16 = oper1 ^ oper2; flag_log16(res16); }
#define op_xor32() { res32 = oper1 ^ oper2; flag_log32(res32); }
#define op_sub8() { res8 = oper1b - oper2b; flag_sub8(oper1b, oper2b); }
#define op_sub16() { \
    register uint32_t dst = (uint32_t) oper1 - (uint32_t) oper2; \
    flag_szp16((uint16_t) dst); \
    cf = (dst & 0xFFFF0000) != 0; \
    of = ((dst ^ (uint32_t)oper1) & (oper1 ^ oper2) & 0x8000) != 0; \
    af = ((oper1 ^ oper2 ^ dst) & 0x10) != 0; \
    res16 = (uint16_t) dst; \
}
#define op_sub32() { res32 = oper1 - oper2; flag_sub32(oper1, oper2); }
#define op_sbb8() { res8 = sbb8(oper1b, oper2b, cf); }
#define op_sbb16() { res16 = sbb16(oper1, oper2, cf); }
#define op_sbb32() { res32 = sbb32(oper1, oper2, cf); }

static inline uint32_t r36sx_cpu_alu32(uint8_t aluop, uint32_t dst, uint32_t src)
{
    uint32_t result;
    uint8_t carry;

    switch (aluop) {
        case 0: /* ADD */
            result = dst + src;
            flag_add32(dst, src, result);
            return result;
        case 1: /* OR */
            result = dst | src;
            flag_log32(result);
            return result;
        case 2: /* ADC */
            carry = cf;
            result = dst + src + carry;
            flag_adc32(dst, src, carry);
            return result;
        case 3: /* SBB */
            carry = cf;
            return sbb32(dst, src, carry);
        case 4: /* AND */
            result = dst & src;
            flag_log32(result);
            return result;
        case 5: /* SUB */
            result = dst - src;
            flag_sub32(dst, src);
            return result;
        case 6: /* XOR */
            result = dst ^ src;
            flag_log32(result);
            return result;
        case 7: /* CMP */
            flag_sub32(dst, src);
            return dst;
    }

    return dst;
}

static inline void r36sx_cpu_alu_rm_reg32(uint8_t aluop, uint8_t rmval,
                                          uint8_t regid)
{
    uint32_t result = r36sx_cpu_alu32(aluop, readrm32(rmval),
                                      getreg32(regid));
    if (aluop != 7) {
        writerm32(rmval, result);
    }
}

static inline void r36sx_cpu_alu_reg_rm32(uint8_t aluop, uint8_t regid,
                                          uint8_t rmval)
{
    uint32_t result = r36sx_cpu_alu32(aluop, getreg32(regid),
                                      readrm32(rmval));
    if (aluop != 7) {
        putreg32(regid, result);
    }
}

static inline void r36sx_cpu_alu_eax_imm32(uint8_t aluop)
{
    uint32_t imm = getmem32(CPU_CS, CPU_IP);
    StepIP(4);
    uint32_t result = r36sx_cpu_alu32(aluop, CPU_EAX, imm);
    if (aluop != 7) {
        CPU_EAX = result;
    }
}

static inline void r36sx_cpu_alu_rm_imm32(uint8_t aluop, uint8_t rmval,
                                          uint32_t imm)
{
    uint32_t result = r36sx_cpu_alu32(aluop, readrm32(rmval), imm);
    if (aluop != 7) {
        writerm32(rmval, result);
    }
}

static inline uint8_t r36sx_cpu_condition(uint8_t condition)
{
    switch (condition & 0x0F) {
        case 0x0: return of;
        case 0x1: return !of;
        case 0x2: return cf;
        case 0x3: return !cf;
        case 0x4: return zf;
        case 0x5: return !zf;
        case 0x6: return cf || zf;
        case 0x7: return !cf && !zf;
        case 0x8: return sf;
        case 0x9: return !sf;
        case 0xA: return pf;
        case 0xB: return !pf;
        case 0xC: return sf != of;
        case 0xD: return sf == of;
        case 0xE: return zf || (sf != of);
        case 0xF: return !zf && (sf == of);
    }
    return 0;
}

static __not_in_flash() uint8_t op_grp2_8(uint8_t cnt, uint8_t oper1b) {
    uint16_t s = oper1b;
#ifdef CPU_LIMIT_SHIFT_COUNT
    cnt &= 0x1F;
#endif
    switch (reg) {
        case 0: /* ROL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | cf;
            }

            if (cnt == 1) {
                // of = cf ^ ( (s >> 7) & 1);
                if ((s & 0x80) && cf)
                    of = 1;
                else
                    of = 0;
            } else
                of = 0;
            break;

        case 1: /* ROR r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = (s >> 1) | (cf << 7);
            }

            if (cnt == 1) {
                of = (s >> 7) ^ ((s >> 6) & 1);
            }
            break;

        case 2: /* RCL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                register bool oldcf = cf;
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | oldcf;
            }

            if (cnt == 1) {
                of = cf ^ ((s >> 7) & 1);
            }
            break;

        case 3: /* RCR r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                register uint8_t oldcf = cf;
                cf = s & 1;
                s = (s >> 1) | (oldcf << 7);
            }

            if (cnt == 1) {
                of = (s >> 7) ^ ((s >> 6) & 1);
            }
            break;

        case 4:
        case 6: /* SHL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = (s << 1) & 0xFF;
            }

            if ((cnt == 1) && (cf == (s >> 7))) {
                of = 0;
            } else {
                of = 1;
            }

            flag_szp8((uint8_t) s);
            break;

        case 5: /* SHR r/m8 */
            if ((cnt == 1) && (s & 0x80)) {
                of = 1;
            } else {
                of = 0;
            }

            for (int a = 1; a <= cnt; a++) {
                cf = s & 1;
                s = s >> 1;
            }

            flag_szp8((uint8_t) s);
            break;

        case 7: /* SAR r/m8 */
            for (int a = 1; a <= cnt; a++) {
                unsigned int msb = s & 0x80;
                cf = s & 1;
                s = (s >> 1) | msb;
            }

            of = 0;
            flag_szp8((uint8_t) s);
            break;
    }

    return s & 0xFF;
}

static __not_in_flash() uint16_t op_grp2_16(uint8_t cnt) {
    register uint32_t s = oper1;
#ifdef CPU_LIMIT_SHIFT_COUNT
    cnt &= 0x1F;
#endif
    switch (reg) {
        case 0: /* ROL r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | cf;
            }

            if (cnt == 1) {
                of = cf ^ ((s >> 15) & 1);
            }
            break;

        case 1: /* ROR r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = (s >> 1) | (cf << 15);
            }

            if (cnt == 1) {
                of = (s >> 15) ^ ((s >> 14) & 1);
            }
            break;

        case 2: /* RCL r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                register bool oldcf = cf;
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | oldcf;
            }

            if (cnt == 1) {
                of = cf ^ ((s >> 15) & 1);
            }
            break;

        case 3: /* RCR r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                register uint32_t oldcf = cf;
                cf = s & 1;
                s = (s >> 1) | (oldcf << 15);
            }

            if (cnt == 1) {
                of = (s >> 15) ^ ((s >> 14) & 1);
            }
            break;

        case 4:
        case 6: /* SHL r/m16 */
            for (unsigned int shift = 1; shift <= cnt; shift++) {
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = (s << 1) & 0xFFFF;
            }

            if ((cnt == 1) && (cf == (s >> 15))) {
                of = 0;
            } else {
                of = 1;
            }

            flag_szp16((uint16_t) s);
            break;

        case 5: /* SHR r/m16 */
            if ((cnt == 1) && (s & 0x8000)) {
                of = 1;
            } else {
                of = 0;
            }

            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = s >> 1;
            }

            flag_szp16((uint16_t) s);
            break;

        case 7: /* SAR r/m16 */
            for (int shift = 1, msb; shift <= cnt; shift++) {
                msb = s & 0x8000;
                cf = s & 1;
                s = (s >> 1) | msb;
            }

            of = 0;
            flag_szp16((uint16_t) s);
            break;
    }

    return (uint16_t) s & 0xFFFF;
}

static __not_in_flash() uint32_t op_grp2_32(uint8_t cnt, uint32_t value) {
    uint32_t s = value;
#ifdef CPU_LIMIT_SHIFT_COUNT
    cnt &= 0x1F;
#endif
    if (!cnt) {
        return s;
    }

    switch (reg) {
        case 0: /* ROL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = (s >> 31) & 1u;
                s = (s << 1) | cf;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            break;

        case 1: /* ROR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s = (s >> 1) | ((uint32_t)cf << 31);
            }
            if (cnt == 1) {
                of = ((s >> 31) ^ (s >> 30)) & 1u;
            }
            break;

        case 2: /* RCL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                uint8_t oldcf = cf;
                cf = (s >> 31) & 1u;
                s = (s << 1) | oldcf;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            break;

        case 3: /* RCR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                uint8_t oldcf = cf;
                cf = s & 1u;
                s = (s >> 1) | ((uint32_t)oldcf << 31);
            }
            if (cnt == 1) {
                of = ((s >> 31) ^ (s >> 30)) & 1u;
            }
            break;

        case 4:
        case 6: /* SHL/SAL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = (s >> 31) & 1u;
                s <<= 1;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            flag_szp32(s);
            break;

        case 5: { /* SHR r/m32 */
            uint8_t oldmsb = (s >> 31) & 1u;
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s >>= 1;
            }
            if (cnt == 1) {
                of = oldmsb;
            }
            flag_szp32(s);
            break;
        }

        case 7: /* SAR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s = (uint32_t)((int32_t)s >> 1);
            }
            of = 0;
            flag_szp32(s);
            break;
    }

    return s;
}

static inline void op_div8(uint16_t valdiv, uint8_t divisor) {
    if (divisor == 0 || valdiv / divisor > 0xFF) {
        printf("[op_div8] %d / %d\n", valdiv, divisor);
        intcall86(0);
        return;
    }

    CPU_AH = (uint8_t) (valdiv % divisor);
    CPU_AL = (uint8_t) (valdiv / divisor);
}

static inline void op_idiv8(uint16_t valdiv, int8_t divisor) {
    if (divisor == 0) {
        printf("[op_idiv8] %d / 0\n", valdiv);
        intcall86(0);
        return;
    }
    int16_t dividend = (int16_t) valdiv;
    int16_t quotient  = dividend / divisor;
    int16_t remainder = dividend % divisor;
    if (quotient < -128 || quotient > 127) {
        printf("[op_idiv8] %d / %d overflow\n", dividend, divisor);
        intcall86(0);
        return;
    }
    CPU_AL = (uint8_t)quotient;
    CPU_AH = (uint8_t)remainder;
}

static inline void op_div16(uint32_t valdiv, uint16_t divisor) {
    if (divisor == 0 || valdiv / divisor > 0xFFFF) {
//        CPU_DX = 0;
//        CPU_AX = 0xFFFF;
//        printf("[op_div16] %d / %d\n", valdiv, divisor);
        intcall86(0);
        return;
    }

    CPU_DX = (uint16_t) (valdiv % divisor);
    CPU_AX = (uint16_t) (valdiv / divisor);
}

static inline void op_idiv16(uint32_t valdiv, uint16_t divisor) {
    int32_t dividend = (int32_t)valdiv;
    int16_t divisor_signed = (int16_t)divisor;
    if (divisor_signed == 0) {
        printf("[op_idiv16] %d / 0\n", dividend);
        intcall86(0);
        return;
    }
    int32_t quotient  = dividend / divisor_signed;
    int32_t remainder = dividend % divisor_signed;
    if (quotient < -32768 || quotient > 32767) {
        printf("[op_idiv16] %d / %d overflow\n", dividend, divisor_signed);
        intcall86(0);
        return;
    }
    CPU_AX = (uint16_t)quotient;
    CPU_DX = (uint16_t)remainder;
}

static inline void op_div32(uint64_t valdiv, uint32_t divisor) {
    if (divisor == 0 || valdiv / divisor > 0xFFFFFFFFull) {
        intcall86(0);
        return;
    }

    CPU_EDX = (uint32_t)(valdiv % divisor);
    CPU_EAX = (uint32_t)(valdiv / divisor);
}

static inline void op_idiv32(int64_t dividend, uint32_t divisor) {
    int32_t divisor_signed = (int32_t)divisor;
    if (divisor_signed == 0) {
        intcall86(0);
        return;
    }

    int64_t quotient = dividend / divisor_signed;
    int64_t remainder = dividend % divisor_signed;
    if (quotient < INT32_MIN || quotient > INT32_MAX) {
        intcall86(0);
        return;
    }

    CPU_EAX = (uint32_t)(int32_t)quotient;
    CPU_EDX = (uint32_t)(int32_t)remainder;
}

static __not_in_flash() void op_grp3_32(uint8_t rmval) {
    uint32_t value = readrm32(rmval);
    switch (reg) {
        case 0:
        case 1: { /* TEST */
            uint32_t imm = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            flag_log32(value & imm);
            break;
        }

        case 2: /* NOT */
            writerm32(rmval, ~value);
            break;

        case 3: { /* NEG */
            uint32_t result = (uint32_t)(0u - value);
            flag_sub32(0, value);
            cf = result != 0;
            writerm32(rmval, result);
            break;
        }

        case 4: { /* MUL */
            uint64_t result = (uint64_t)value * (uint64_t)CPU_EAX;
            CPU_EAX = (uint32_t)result;
            CPU_EDX = (uint32_t)(result >> 32);
            if (CPU_EDX) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }

        case 5: { /* IMUL */
            int64_t result = (int64_t)(int32_t)CPU_EAX *
                             (int64_t)(int32_t)value;
            CPU_EAX = (uint32_t)result;
            CPU_EDX = (uint32_t)(result >> 32);
            if (result != (int64_t)(int32_t)CPU_EAX) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }

        case 6: /* DIV */
            op_div32(((uint64_t)CPU_EDX << 32) | CPU_EAX, value);
            break;

        case 7: { /* IDIV */
            int64_t dividend = ((int64_t)(int32_t)CPU_EDX << 32) | CPU_EAX;
            op_idiv32(dividend, value);
            break;
        }
    }
}


static __not_in_flash() void op_grp3_16() {
    switch (reg) {
        case 0:
        case 1: /* TEST */
            flag_log16(oper1 & getmem16(CPU_CS, CPU_IP));
            StepIP(2);
            break;

        case 2: /* NOT */
            res16 = ~oper1;
            break;

        case 3: /* NEG */
            res16 = (~oper1) + 1;
            flag_sub16(0, oper1);
            if (res16) {
                cf = 1;
            } else {
                cf = 0;
            }
            break;

        case 4: {
            /* MUL */
            register uint32_t temp1 = (uint32_t) oper1 * (uint32_t) CPU_AX;
            CPU_AX = temp1 & 0xFFFF;
            CPU_DX = temp1 >> 16;
            flag_szp16((uint16_t) temp1);
            if (CPU_DX) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }
        case 5: {
            /* IMUL */
            register int32_t temp1 = (int32_t)(int16_t)CPU_AX * (int32_t)(int16_t)oper1;
			int16_t truncated = (int16_t)temp1;
            CPU_AX = truncated; /* into register ax */
            CPU_DX = (uint16_t)(temp1 >> 16); /* into register dx */
            if (temp1 != (int32_t)truncated) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }
        case 6: /* DIV */
            op_div16(((uint32_t) CPU_DX << 16) + CPU_AX, oper1);
            break;

        case 7: /* DIV */
            op_idiv16(((uint32_t) CPU_DX << 16) + CPU_AX, oper1);
            break;
    }
}

static __not_in_flash() void op_grp5() {
    switch (reg) {
        case 0: /* INC Ev */
            oper2 = 1;
            tempcf = cf;
            op_add16();
            cf = tempcf;
            writerm16(rm, res16);
            break;

        case 1: /* DEC Ev */
            oper2 = 1;
            tempcf = cf;
            op_sub16();
            cf = tempcf;
            writerm16(rm, res16);
            break;

        case 2: /* CALL Ev */
            push(ip);
            ip = oper1;
            break;

        case 3: /* CALL Mp */
            push(CPU_CS);
            push(ip);
            getea(rm);
            ip = (uint16_t) read86(ea) + (uint16_t) read86(ea + 1) * 256;
            r36sx_cpu_load_segment(
                regcs,
                (uint16_t) read86(ea + 2) + (uint16_t) read86(ea + 3) * 256);
            break;

        case 4: /* JMP Ev */
            ip = oper1;
            break;

        case 5: /* JMP Mp */
            getea(rm);
            ip = (uint16_t) read86(ea) + (uint16_t) read86(ea + 1) * 256;
            r36sx_cpu_load_segment(
                regcs,
                (uint16_t) read86(ea + 2) + (uint16_t) read86(ea + 3) * 256);
            break;

        case 6: /* PUSH Ev */
            push(oper1);
            break;
    }
}

static inline uint32_t r36sx_read_moffs(void)
{
    if (addressSizeOverride) {
        uint32_t offset = getmem32(CPU_CS, CPU_IP);
        StepIP(4);
        return offset;
    }

    uint16_t offset = getmem16(CPU_CS, CPU_IP);
    StepIP(2);
    return offset;
}

static __not_in_flash() bool r36sx_cpu_exec_operand32_opcode(uint8_t opcode,
                                                             uint16_t fault_ip,
                                                             uint32_t execloops,
                                                             uint32_t *loopcount,
                                                             bool trace_active)
{
    uint8_t aluop = opcode >> 3;

    switch (opcode) {
        case 0x01: case 0x09: case 0x11: case 0x19:
        case 0x21: case 0x29: case 0x31: case 0x39:
            modregrm();
            r36sx_cpu_alu_rm_reg32(aluop, rm, reg);
            return true;

        case 0x03: case 0x0B: case 0x13: case 0x1B:
        case 0x23: case 0x2B: case 0x33: case 0x3B:
            modregrm();
            r36sx_cpu_alu_reg_rm32(aluop, reg, rm);
            return true;

        case 0x05: case 0x0D: case 0x15: case 0x1D:
        case 0x25: case 0x2D: case 0x35: case 0x3D:
            r36sx_cpu_alu_eax_imm32(aluop);
            return true;

        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45: case 0x46: case 0x47: {
            uint8_t regid = opcode & 7u;
            uint8_t saved_cf = cf;
            uint32_t value = getreg32(regid);
            uint32_t result = value + 1u;
            flag_add32(value, 1u, result);
            cf = saved_cf;
            putreg32(regid, result);
            return true;
        }

        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D: case 0x4E: case 0x4F: {
            uint8_t regid = opcode & 7u;
            uint8_t saved_cf = cf;
            uint32_t value = getreg32(regid);
            uint32_t result = value - 1u;
            flag_sub32(value, 1u);
            cf = saved_cf;
            putreg32(regid, result);
            return true;
        }

        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55: case 0x56: case 0x57:
            push32(getreg32(opcode & 7u));
            return true;

        case 0x58: case 0x59: case 0x5A: case 0x5B:
        case 0x5C: case 0x5D: case 0x5E: case 0x5F:
            putreg32(opcode & 7u, pop32());
            return true;

        case 0x68:
            push32(getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        case 0x69: {
            modregrm();
            int64_t result = (int64_t)(int32_t)readrm32(rm) *
                             (int64_t)(int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            putreg32(reg, (uint32_t)result);
            if (result != (int64_t)(int32_t)(uint32_t)result) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
            return true;
        }

        case 0x6A:
            push32((uint32_t)(int32_t)(int8_t)getmem8(CPU_CS, CPU_IP));
            StepIP(1);
            return true;

        case 0x6B: {
            modregrm();
            int64_t result = (int64_t)(int32_t)readrm32(rm) *
                             (int64_t)(int8_t)getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            putreg32(reg, (uint32_t)result);
            if (result != (int64_t)(int32_t)(uint32_t)result) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
            return true;
        }

        case 0x81:
        case 0x83: {
            modregrm();
            uint32_t imm;
            if (opcode == 0x81) {
                imm = getmem32(CPU_CS, CPU_IP);
                StepIP(4);
            } else {
                imm = (uint32_t)(int32_t)(int8_t)getmem8(CPU_CS, CPU_IP);
                StepIP(1);
            }
            r36sx_cpu_alu_rm_imm32(reg, rm, imm);
            return true;
        }

        case 0x85:
            modregrm();
            flag_log32(getreg32(reg) & readrm32(rm));
            return true;

        case 0x87: {
            modregrm();
            uint32_t tmp = getreg32(reg);
            putreg32(reg, readrm32(rm));
            writerm32(rm, tmp);
            return true;
        }

        case 0x89:
            modregrm();
            writerm32(rm, getreg32(reg));
            return true;

        case 0x8B:
            modregrm();
            putreg32(reg, readrm32(rm));
            return true;

        case 0x8D:
            modregrm();
            getea(rm);
            putreg32(reg, ea - segbase(useseg));
            return true;

        case 0x8F:
            modregrm();
            writerm32(rm, pop32());
            return true;

        case 0x9A: {
            uint32_t target_ip = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            uint16_t target_cs = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            push(CPU_CS);
            push32(CPU_IP);
            CPU_IP = (uint16_t)target_ip;
            r36sx_cpu_load_segment(regcs, target_cs);
            return true;
        }

        case 0x90:
            return true;

        case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97: {
            uint8_t regid = opcode & 7u;
            uint32_t tmp = CPU_EAX;
            CPU_EAX = getreg32(regid);
            putreg32(regid, tmp);
            return true;
        }

        case 0x98:
            CPU_EAX = (uint32_t)(int32_t)(int16_t)CPU_AX;
            return true;

        case 0x99:
            CPU_EDX = ((int32_t)CPU_EAX < 0) ? 0xFFFFFFFFu : 0u;
            return true;

        case 0xA1:
            CPU_EAX = getmem32(useseg, r36sx_read_moffs());
            return true;

        case 0xA3:
            putmem32(useseg, r36sx_read_moffs(), CPU_EAX);
            return true;

        case 0xA5:
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (reptype) {
                uint32_t batch = trace_active
                    ? 1u
                    : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                            *loopcount, execloops);
                r36sx_rep_movsd(batch);
                r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                *loopcount += batch;
                if (r36sx_rep_get_count() != 0) {
                    CPU_IP = fault_ip;
                }
                return true;
            }
            r36sx_rep_movsd(1);
            (*loopcount)++;
            return true;

        case 0xA9:
            flag_log32(CPU_EAX & getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        case 0xAB:
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (reptype) {
                uint32_t batch = trace_active
                    ? 1u
                    : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                            *loopcount, execloops);
                r36sx_rep_stosd(batch);
                r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                *loopcount += batch;
                if (r36sx_rep_get_count() != 0) {
                    CPU_IP = fault_ip;
                }
                return true;
            }
            r36sx_rep_stosd(1);
            (*loopcount)++;
            return true;

        case 0xAD: {
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            uint32_t si = r36sx_src_index();
            CPU_EAX = getmem32(useseg, si);
            r36sx_set_src_index(df ? si - 4 : si + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            (*loopcount)++;
            if (reptype) {
                CPU_IP = fault_ip;
            }
            return true;
        }

        case 0xAF: {
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            uint32_t di = r36sx_dst_index();
            flag_sub32(CPU_EAX, getmem32(CPU_ES, di));
            r36sx_set_dst_index(df ? di - 4 : di + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            if ((reptype == 1) && !zf) {
                return true;
            }
            if ((reptype == 2) && zf) {
                return true;
            }
            (*loopcount)++;
            if (reptype) {
                CPU_IP = fault_ip;
            }
            return true;
        }

        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD: case 0xBE: case 0xBF:
            putreg32(opcode & 7u, getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        case 0xC1:
            modregrm();
            {
                uint32_t value = readrm32(rm);
                uint8_t count = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm32(rm, op_grp2_32(count, value));
            }
            return true;

        case 0xC2: {
            uint16_t bytes = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            CPU_IP = (uint16_t)pop32();
            CPU_SP = (uint16_t)(CPU_SP + bytes);
            return true;
        }

        case 0xC3:
            CPU_IP = (uint16_t)pop32();
            return true;

        case 0xC7:
            modregrm();
            if (reg != 0) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return true;
            }
            writerm32(rm, getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        case 0xC9:
            CPU_SP = CPU_BP;
            CPU_EBP = pop32();
            return true;

        case 0xCA: {
            uint16_t bytes = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            CPU_IP = (uint16_t)pop32();
            r36sx_cpu_load_segment(regcs, pop());
            CPU_SP = (uint16_t)(CPU_SP + bytes);
            return true;
        }

        case 0xCB:
            CPU_IP = (uint16_t)pop32();
            r36sx_cpu_load_segment(regcs, pop());
            return true;

        case 0xCF:
            CPU_IP = (uint16_t)pop32();
            r36sx_cpu_load_segment(regcs, (uint16_t)pop32());
            decodeflagsdword(pop32());
            return true;

        case 0xD1:
            modregrm();
            writerm32(rm, op_grp2_32(1, readrm32(rm)));
            return true;

        case 0xD3:
            modregrm();
            writerm32(rm, op_grp2_32(CPU_CL, readrm32(rm)));
            return true;

        case 0xE8: {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            push32(CPU_IP);
            CPU_IP = (uint16_t)(CPU_IP + rel);
            return true;
        }

        case 0xE9: {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            CPU_IP = (uint16_t)(CPU_IP + rel);
            return true;
        }

        case 0xEA: {
            uint32_t target_ip = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            uint16_t target_cs = getmem16(CPU_CS, CPU_IP);
            CPU_IP = (uint16_t)target_ip;
            r36sx_cpu_load_segment(regcs, target_cs);
            return true;
        }

        case 0xF7:
            modregrm();
            op_grp3_32(rm);
            return true;

        case 0xFF:
            modregrm();
            switch (reg) {
                case 0: { /* INC Ev */
                    uint8_t saved_cf = cf;
                    uint32_t value = readrm32(rm);
                    uint32_t result = value + 1u;
                    flag_add32(value, 1u, result);
                    cf = saved_cf;
                    writerm32(rm, result);
                    return true;
                }
                case 1: { /* DEC Ev */
                    uint8_t saved_cf = cf;
                    uint32_t value = readrm32(rm);
                    uint32_t result = value - 1u;
                    flag_sub32(value, 1u);
                    cf = saved_cf;
                    writerm32(rm, result);
                    return true;
                }
                case 2: { /* CALL Ev */
                    uint32_t target = readrm32(rm);
                    push32(CPU_IP);
                    CPU_IP = (uint16_t)target;
                    return true;
                }
                case 3: { /* CALL Mp */
                    getea(rm);
                    uint32_t target_ip = readdw86(ea);
                    uint16_t target_cs = readw86(ea + 4);
                    push(CPU_CS);
                    push32(CPU_IP);
                    CPU_IP = (uint16_t)target_ip;
                    r36sx_cpu_load_segment(regcs, target_cs);
                    return true;
                }
                case 4: /* JMP Ev */
                    CPU_IP = (uint16_t)readrm32(rm);
                    return true;
                case 5: { /* JMP Mp */
                    getea(rm);
                    CPU_IP = (uint16_t)readdw86(ea);
                    r36sx_cpu_load_segment(regcs, readw86(ea + 4));
                    return true;
                }
                case 6: /* PUSH Ev */
                    push32(readrm32(rm));
                    return true;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return true;
    }

    return false;
}

static __not_in_flash() void r36sx_cpu_exec_bit_test(uint8_t operation,
                                                     uint32_t bit_offset,
                                                     uint8_t register_offset)
{
    uint8_t width = operandSizeOverride ? 32u : 16u;
    uint8_t bit = (uint8_t)(bit_offset & (uint32_t)(width - 1u));
    uint32_t mask = 1u << bit;
    uint32_t value;

    if (mode < 3) {
        getea(rm);
        if (register_offset) {
            ea += (bit_offset / width) * (uint32_t)(width / 8u);
        }
        value = operandSizeOverride ? readdw86(ea) : readw86(ea);
    } else {
        value = operandSizeOverride ? getreg32(rm) : getreg16(rm);
    }

    cf = (value & mask) != 0;
    switch (operation) {
        case 1: /* BTS */
            value |= mask;
            break;
        case 2: /* BTR */
            value &= ~mask;
            break;
        case 3: /* BTC */
            value ^= mask;
            break;
        default: /* BT */
            return;
    }

    if (mode < 3) {
        if (operandSizeOverride) {
            writedw86(ea, value);
        } else {
            writew86(ea, (uint16_t)value);
        }
    } else if (operandSizeOverride) {
        putreg32(rm, value);
    } else {
        putreg16(rm, (uint16_t)value);
    }
}

static __not_in_flash() void r36sx_cpu_exec_0f(uint16_t fault_ip)
{
    uint8_t op2 = getmem8(CPU_CS, CPU_IP);
    StepIP(1);

    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) &&
        op2 != 0x00 && op2 != 0x01 && op2 != 0x06) {
        r36sx_cpu_invalid_opcode(fault_ip);
        return;
    }

    if (op2 >= 0x80 && op2 <= 0x8F) {
        uint8_t take = r36sx_cpu_condition(op2);
        if (operandSizeOverride) {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            if (take) {
                CPU_IP = (uint16_t)(CPU_IP + rel);
            }
        } else {
            int16_t rel = (int16_t)getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (take) {
                CPU_IP = (uint16_t)(CPU_IP + rel);
            }
        }
        return;
    }

    if (op2 >= 0x90 && op2 <= 0x9F) {
        modregrm();
        writerm8(rm, r36sx_cpu_condition(op2) ? 1u : 0u);
        return;
    }

    switch (op2) {
        case 0x00: { /* SLDT/STR/LLDT/LTR/VERR/VERW */
            if (!r36sx_cpu_protected_enabled()) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            modregrm();
            switch (reg) {
                case 0: /* SLDT Ew */
                    writerm16(rm, r36sx_ldtr_selector);
                    return;
                case 1: /* STR Ew */
                    writerm16(rm, r36sx_tr_selector);
                    return;
                case 2: /* LLDT Ew */
                    r36sx_ldtr_selector = readrm16(rm);
                    return;
                case 3: /* LTR Ew */
                    r36sx_tr_selector = readrm16(rm);
                    return;
                case 4: /* VERR Ew */
                case 5: { /* VERW Ew */
                    r36sx_segment_cache_t cache;
                    uint16_t selector = readrm16(rm);
                    uint8_t ok = r36sx_cpu_decode_descriptor(selector, &cache) &&
                                 (cache.access & R36SX_DESCRIPTOR_CODE_DATA);
                    if (ok && reg == 4) {
                        ok = ((cache.access & R36SX_DESCRIPTOR_EXECUTABLE) == 0) ||
                             (cache.access & R36SX_DESCRIPTOR_READABLE);
                    } else if (ok) {
                        ok = ((cache.access & R36SX_DESCRIPTOR_EXECUTABLE) == 0) &&
                             (cache.access & R36SX_DESCRIPTOR_WRITABLE);
                    }
                    zf = ok ? 1u : 0u;
                    return;
                }
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x01: { /* SGDT/SIDT/LGDT/LIDT/SMSW/LMSW */
            modregrm();
            switch (reg) {
                case 0: /* SGDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_store_descriptor_table(
                        ea, r36sx_gdtr_limit, r36sx_gdtr_base);
                    return;
                case 1: /* SIDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_store_descriptor_table(
                        ea, r36sx_idtr_limit, r36sx_idtr_base);
                    return;
                case 2: /* LGDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_load_descriptor_table(
                        ea, &r36sx_gdtr_limit, &r36sx_gdtr_base,
                        operandSizeOverride ? 0xffffffffu : 0x00ffffffu);
                    return;
                case 3: /* LIDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_load_descriptor_table(
                        ea, &r36sx_idtr_limit, &r36sx_idtr_base,
                        operandSizeOverride ? 0xffffffffu : 0x00ffffffu);
                    return;
                case 4: /* SMSW Ew */
                    writerm16(rm, (uint16_t)r36sx_cr0);
                    return;
                case 6: /* LMSW Ew */
                    r36sx_cpu_lmsw(readrm16(rm));
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x06: /* CLTS */
            r36sx_cr0 &= ~R36SX_CR0_TS;
            return;

        case 0x20: { /* MOV Rd,Cd */
            modregrm();
            if (mode != 3) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            switch (reg) {
                case 0:
                    putreg32(rm, r36sx_cpu_read_cr0());
                    return;
                case 2:
                    putreg32(rm, r36sx_cr2);
                    return;
                case 3:
                    putreg32(rm, r36sx_cr3 & R36SX_CR3_PAGE_DIRECTORY_MASK);
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x22: { /* MOV Cd,Rd */
            modregrm();
            if (mode != 3) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            switch (reg) {
                case 0:
                    r36sx_cpu_set_cr0(getreg32(rm));
                    return;
                case 2:
                    r36sx_cr2 = getreg32(rm);
                    return;
                case 3:
                    r36sx_cr3 = getreg32(rm) & R36SX_CR3_PAGE_DIRECTORY_MASK;
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0xA0:
            if (operandSizeOverride) {
                push32(CPU_FS);
            } else {
                push(CPU_FS);
            }
            return;

        case 0xA1:
            r36sx_cpu_load_segment(
                regfs, operandSizeOverride ? (uint16_t)pop32() : pop());
            return;

        case 0xA8:
            if (operandSizeOverride) {
                push32(CPU_GS);
            } else {
                push(CPU_GS);
            }
            return;

        case 0xA9:
            r36sx_cpu_load_segment(
                reggs, operandSizeOverride ? (uint16_t)pop32() : pop());
            return;

        case 0xA3: /* BT Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                0, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xAB: /* BTS Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                1, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xAF: {
            modregrm();
            if (operandSizeOverride) {
                int64_t result = (int64_t)(int32_t)getreg32(reg) *
                                 (int64_t)(int32_t)readrm32(rm);
                putreg32(reg, (uint32_t)result);
                if (result != (int64_t)(int32_t)(uint32_t)result) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
            } else {
                int32_t result = (int32_t)(int16_t)getreg16(reg) *
                                 (int32_t)(int16_t)readrm16(rm);
                putreg16(reg, (uint16_t)result);
                if (result != (int32_t)(int16_t)(uint16_t)result) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
            }
            return;
        }

        case 0xB3: /* BTR Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                2, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xB6:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, readrm8(rm));
            } else {
                putreg16(reg, readrm8(rm));
            }
            return;

        case 0xB7:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, readrm16(rm));
            } else {
                putreg16(reg, readrm16(rm));
            }
            return;

        case 0xBA: { /* BT/BTS/BTR/BTC Ev,Ib */
            modregrm();
            if (reg < 4 || reg > 7) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            uint8_t bit = getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            r36sx_cpu_exec_bit_test((uint8_t)(reg - 4u), bit, 0);
            return;
        }

        case 0xBB: /* BTC Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                3, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xBC: {
            modregrm();
            uint32_t value = operandSizeOverride ? readrm32(rm) : readrm16(rm);
            if (!value) {
                zf = 1;
                return;
            }
            zf = 0;
            uint8_t bit = 0;
            while (((value >> bit) & 1u) == 0) {
                bit++;
            }
            if (operandSizeOverride) {
                putreg32(reg, bit);
            } else {
                putreg16(reg, bit);
            }
            return;
        }

        case 0xBD: {
            modregrm();
            uint32_t value = operandSizeOverride ? readrm32(rm) : readrm16(rm);
            if (!value) {
                zf = 1;
                return;
            }
            zf = 0;
            uint8_t bit = operandSizeOverride ? 31u : 15u;
            while (((value >> bit) & 1u) == 0) {
                bit--;
            }
            if (operandSizeOverride) {
                putreg32(reg, bit);
            } else {
                putreg16(reg, bit);
            }
            return;
        }

        case 0xBE:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, (uint32_t)(int32_t)(int8_t)readrm8(rm));
            } else {
                putreg16(reg, (uint16_t)(int16_t)(int8_t)readrm8(rm));
            }
            return;

        case 0xBF:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, (uint32_t)(int32_t)(int16_t)readrm16(rm));
            } else {
                putreg16(reg, readrm16(rm));
            }
            return;
    }

    r36sx_cpu_invalid_opcode(fault_ip);
}

void reset86() {
    CPU_CS = 0xFFFF;
    CPU_SS = 0x0000;
    CPU_SP = 0x0000;
    hltstate = 0;
    r36sx_cr0 = R36SX_CR0_ET;
    r36sx_cr2 = 0;
    r36sx_cr3 = 0;
    r36sx_gdtr_base = 0;
    r36sx_gdtr_limit = 0;
    r36sx_idtr_base = 0;
    r36sx_idtr_limit = 0x03ffu;
    r36sx_ldtr_selector = 0;
    r36sx_tr_selector = 0;
    r36sx_cpu_real_cache_all_segments();

    memset(VIDEORAM, 0x00, sizeof(VIDEORAM));
    r36sx_pico286_video_mark_dirty();
    if (butter_psram_size) {
        memset(RAM, 0, sizeof(RAM));
        memset(UMB, 0, sizeof(UMB));
        memset(HMA, 0, sizeof(HMA));
    } else {
        memset(SRAM, 0, sizeof(SRAM));
        if (PSRAM_AVAILABLE) {
            for (uint32_t a = HMA_START; a < HMA_END; a += 4) write32psram(a, 0);
        } else {
            for (uint32_t a = HMA_START; a < HMA_END; a += 4) swap_write32(a, 0);
        }
    }
    init_umb();
    ip = 0x0000;
    i8237_reset();
    vga_init();
}

/// @brief  W/A for SWAP mode (avoid using core#1)
extern volatile int16_t last_sb_sample;
extern volatile bool ask_to_blast;

void __not_in_flash() exec86(uint32_t execloops) {
    static uint16_t firstip;
    static bool was_TF;
    uint32_t loopcount = 0;

    //counterticks = (uint64_t) ( (double) timerfreq / (double) 65536.0);
    //tickssource();
    for (loopcount = 0; loopcount < execloops; loopcount++) {
        if (unlikely(hltstate)) {
            if (unlikely(ifl && r36sx_cpu_pending_maskable_irq())) {
                hltstate = 0;
                intcall86(nextintr());
            } else {
                r36sx_app_stats_record_x86(loopcount);
                return;
            }
        } else if (unlikely(ifl && r36sx_cpu_pending_maskable_irq())) {
            intcall86(nextintr()); // get next interrupt from the i8259, if any d
        }
#if PICO_ON_DEVICE
        if (ask_to_blast) {
            ask_to_blast = false;
            last_sb_sample = blaster_sample();
        }
#endif
        reptype = 0;
        segoverride = 0;
        operandSizeOverride = false;
        addressSizeOverride = false;
        useseg = CPU_DS;
        uint8_t docontinue = 0;
        uint8_t prefix_exception = 0;
        firstip = CPU_IP;
        register uint8_t opcode;

        while (!docontinue) {
            ///         CPU_CS &= 0xFFFF;
            ///         CPU_IP &= 0xFFFF;
            //            savecs = CPU_CS;
            //            saveip = ip;
            // W/A-hack: last byte of interrupts table (actually should not be ever used as CS:IP)
            if (unlikely(CPU_CS == XMS_FN_CS && ip == XMS_FN_IP)) {
                // hook for XMS
                opcode = xms_handler(); // always returns RET TODO: far/short ret?
            } else {
                opcode = getmem8(CPU_CS, CPU_IP);
            }

            StepIP(1);

            switch (opcode) {
                /* segment prefix check */
                case 0x2E: /* segment CPU_CS */
                    useseg = CPU_CS;
                    segoverride = 1;
                    break;

                case 0x3E: /* segment CPU_DS */
                    useseg = CPU_DS;
                    segoverride = 1;
                    break;

                case 0x26: /* segment CPU_ES */
                    useseg = CPU_ES;
                    segoverride = 1;
                    break;

                case 0x36: /* segment CPU_SS */
                    useseg = CPU_SS;
                    segoverride = 1;
                    break;

#if CPU_386_EXTENDED_OPS
                case 0x64: /* segment CPU_FS */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        CPU_IP = firstip;
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    useseg = CPU_FS;
                    segoverride = 1;
                    break;

                case 0x65: /* segment CPU_GS */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        CPU_IP = firstip;
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    useseg = CPU_GS;
                    segoverride = 1;
                    break;
#else
                case 0x64:
                case 0x65:
                    CPU_IP = firstip;
                    intcall86(6);
                    prefix_exception = 1;
                    docontinue = 1;
                    break;
#endif

#if CPU_386_EXTENDED_OPS
                case 0x66: /* operand-size override */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        CPU_IP = firstip;
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    operandSizeOverride = true;
                    break;

                case 0x67: /* address-size override */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        CPU_IP = firstip;
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    addressSizeOverride = true;
                    break;
#endif

                case 0xF0: /* LOCK (?????????? ????, ??? ????????? ????????) */
                    /// TODO:
                    break;

                case 0xF2: /* REPNE/REPNZ */
                    reptype = 2;
                    break;

                /* repetition prefix check */
                case 0xF3: /* REP/REPE/REPZ */
                    reptype = 1;
                    break;

                default:
                    docontinue = 1;
                    break;
            }
        }
        if (prefix_exception) {
            continue;
        }

        register uint32_t res32;
        register uint8_t res8;
        register uint8_t oper1b;
        register uint8_t oper2b;
#if CPU_386_EXTENDED_OPS
        if (operandSizeOverride &&
            r36sx_cpu_exec_operand32_opcode(opcode, firstip, execloops,
                                            &loopcount, tf || was_TF)) {
            goto r36sx_opcode_done;
        }
#endif
#if R36SX_CPU_COMPUTED_GOTO
        /* GNU labels-as-values remove the large opcode switch from the hot path. */
        static void *const r36sx_opcode_dispatch[256] = {
            &&r36sx_opcode_00, &&r36sx_opcode_01, &&r36sx_opcode_02, &&r36sx_opcode_03, &&r36sx_opcode_04, &&r36sx_opcode_05, &&r36sx_opcode_06, &&r36sx_opcode_07,
            &&r36sx_opcode_08, &&r36sx_opcode_09, &&r36sx_opcode_0A, &&r36sx_opcode_0B, &&r36sx_opcode_0C, &&r36sx_opcode_0D, &&r36sx_opcode_0E, &&r36sx_opcode_0F,
            &&r36sx_opcode_10, &&r36sx_opcode_11, &&r36sx_opcode_12, &&r36sx_opcode_13, &&r36sx_opcode_14, &&r36sx_opcode_15, &&r36sx_opcode_16, &&r36sx_opcode_17,
            &&r36sx_opcode_18, &&r36sx_opcode_19, &&r36sx_opcode_1A, &&r36sx_opcode_1B, &&r36sx_opcode_1C, &&r36sx_opcode_1D, &&r36sx_opcode_1E, &&r36sx_opcode_1F,
            &&r36sx_opcode_20, &&r36sx_opcode_21, &&r36sx_opcode_22, &&r36sx_opcode_23, &&r36sx_opcode_24, &&r36sx_opcode_25, &&r36sx_opcode_default, &&r36sx_opcode_27,
            &&r36sx_opcode_28, &&r36sx_opcode_29, &&r36sx_opcode_2A, &&r36sx_opcode_2B, &&r36sx_opcode_2C, &&r36sx_opcode_2D, &&r36sx_opcode_default, &&r36sx_opcode_2F,
            &&r36sx_opcode_30, &&r36sx_opcode_31, &&r36sx_opcode_32, &&r36sx_opcode_33, &&r36sx_opcode_34, &&r36sx_opcode_35, &&r36sx_opcode_default, &&r36sx_opcode_37,
            &&r36sx_opcode_38, &&r36sx_opcode_39, &&r36sx_opcode_3A, &&r36sx_opcode_3B, &&r36sx_opcode_3C, &&r36sx_opcode_3D, &&r36sx_opcode_default, &&r36sx_opcode_3F,
            &&r36sx_opcode_40, &&r36sx_opcode_41, &&r36sx_opcode_42, &&r36sx_opcode_43, &&r36sx_opcode_44, &&r36sx_opcode_45, &&r36sx_opcode_46, &&r36sx_opcode_47,
            &&r36sx_opcode_48, &&r36sx_opcode_49, &&r36sx_opcode_4A, &&r36sx_opcode_4B, &&r36sx_opcode_4C, &&r36sx_opcode_4D, &&r36sx_opcode_4E, &&r36sx_opcode_4F,
            &&r36sx_opcode_50, &&r36sx_opcode_51, &&r36sx_opcode_52, &&r36sx_opcode_53, &&r36sx_opcode_54, &&r36sx_opcode_55, &&r36sx_opcode_56, &&r36sx_opcode_57,
            &&r36sx_opcode_58, &&r36sx_opcode_59, &&r36sx_opcode_5A, &&r36sx_opcode_5B, &&r36sx_opcode_5C, &&r36sx_opcode_5D, &&r36sx_opcode_5E, &&r36sx_opcode_5F,
            &&r36sx_opcode_60, &&r36sx_opcode_61, &&r36sx_opcode_62, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_66, &&r36sx_opcode_67,
            &&r36sx_opcode_68, &&r36sx_opcode_69, &&r36sx_opcode_6A, &&r36sx_opcode_6B, &&r36sx_opcode_6C, &&r36sx_opcode_6D, &&r36sx_opcode_6E, &&r36sx_opcode_6F,
            &&r36sx_opcode_70, &&r36sx_opcode_71, &&r36sx_opcode_72, &&r36sx_opcode_73, &&r36sx_opcode_74, &&r36sx_opcode_75, &&r36sx_opcode_76, &&r36sx_opcode_77,
            &&r36sx_opcode_78, &&r36sx_opcode_79, &&r36sx_opcode_7A, &&r36sx_opcode_7B, &&r36sx_opcode_7C, &&r36sx_opcode_7D, &&r36sx_opcode_7E, &&r36sx_opcode_7F,
            &&r36sx_opcode_80, &&r36sx_opcode_81, &&r36sx_opcode_82, &&r36sx_opcode_83, &&r36sx_opcode_84, &&r36sx_opcode_85, &&r36sx_opcode_86, &&r36sx_opcode_87,
            &&r36sx_opcode_88, &&r36sx_opcode_89, &&r36sx_opcode_8A, &&r36sx_opcode_8B, &&r36sx_opcode_8C, &&r36sx_opcode_8D, &&r36sx_opcode_8E, &&r36sx_opcode_8F,
            &&r36sx_opcode_90, &&r36sx_opcode_91, &&r36sx_opcode_92, &&r36sx_opcode_93, &&r36sx_opcode_94, &&r36sx_opcode_95, &&r36sx_opcode_96, &&r36sx_opcode_97,
            &&r36sx_opcode_98, &&r36sx_opcode_99, &&r36sx_opcode_9A, &&r36sx_opcode_9B, &&r36sx_opcode_9C, &&r36sx_opcode_9D, &&r36sx_opcode_9E, &&r36sx_opcode_9F,
            &&r36sx_opcode_A0, &&r36sx_opcode_A1, &&r36sx_opcode_A2, &&r36sx_opcode_A3, &&r36sx_opcode_A4, &&r36sx_opcode_A5, &&r36sx_opcode_A6, &&r36sx_opcode_A7,
            &&r36sx_opcode_A8, &&r36sx_opcode_A9, &&r36sx_opcode_AA, &&r36sx_opcode_AB, &&r36sx_opcode_AC, &&r36sx_opcode_AD, &&r36sx_opcode_AE, &&r36sx_opcode_AF,
            &&r36sx_opcode_B0, &&r36sx_opcode_B1, &&r36sx_opcode_B2, &&r36sx_opcode_B3, &&r36sx_opcode_B4, &&r36sx_opcode_B5, &&r36sx_opcode_B6, &&r36sx_opcode_B7,
            &&r36sx_opcode_B8, &&r36sx_opcode_B9, &&r36sx_opcode_BA, &&r36sx_opcode_BB, &&r36sx_opcode_BC, &&r36sx_opcode_BD, &&r36sx_opcode_BE, &&r36sx_opcode_BF,
            &&r36sx_opcode_C0, &&r36sx_opcode_C1, &&r36sx_opcode_C2, &&r36sx_opcode_C3, &&r36sx_opcode_C4, &&r36sx_opcode_C5, &&r36sx_opcode_C6, &&r36sx_opcode_C7,
            &&r36sx_opcode_C8, &&r36sx_opcode_C9, &&r36sx_opcode_CA, &&r36sx_opcode_CB, &&r36sx_opcode_CC, &&r36sx_opcode_CD, &&r36sx_opcode_CE, &&r36sx_opcode_CF,
            &&r36sx_opcode_D0, &&r36sx_opcode_D1, &&r36sx_opcode_D2, &&r36sx_opcode_D3, &&r36sx_opcode_D4, &&r36sx_opcode_D5, &&r36sx_opcode_D6, &&r36sx_opcode_D7,
            &&r36sx_opcode_D8, &&r36sx_opcode_D9, &&r36sx_opcode_DA, &&r36sx_opcode_DB, &&r36sx_opcode_DC, &&r36sx_opcode_DD, &&r36sx_opcode_DE, &&r36sx_opcode_DF,
            &&r36sx_opcode_E0, &&r36sx_opcode_E1, &&r36sx_opcode_E2, &&r36sx_opcode_E3, &&r36sx_opcode_E4, &&r36sx_opcode_E5, &&r36sx_opcode_E6, &&r36sx_opcode_E7,
            &&r36sx_opcode_E8, &&r36sx_opcode_E9, &&r36sx_opcode_EA, &&r36sx_opcode_EB, &&r36sx_opcode_EC, &&r36sx_opcode_ED, &&r36sx_opcode_EE, &&r36sx_opcode_EF,
            &&r36sx_opcode_F0, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_F4, &&r36sx_opcode_F5, &&r36sx_opcode_F6, &&r36sx_opcode_F7,
            &&r36sx_opcode_F8, &&r36sx_opcode_F9, &&r36sx_opcode_FA, &&r36sx_opcode_FB, &&r36sx_opcode_FC, &&r36sx_opcode_FD, &&r36sx_opcode_FE, &&r36sx_opcode_FF,
        };
        goto *r36sx_opcode_dispatch[opcode];
#endif
        switch (opcode) {
            case 0x0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_00: ;
#endif
                /* 00 ADD Eb Gb */
                modregrm();
                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_add8();
                writerm8(rm, res8);
                break;

            case 0x1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_01: ;
#endif
                /* 01 ADD Ev Gv */
                modregrm();
                if (operandSizeOverride) {
                    register uint32_t oper1 = readrm32(rm);
                    register uint32_t oper2 = getreg32(reg);
                    op_add32();
                    writerm32(rm, res32);
                } else {
                    register uint32_t oper1 = readrm16(rm);
                    register uint32_t oper2 = getreg16(reg);
                    op_add16();
                    writerm16(rm, res16);
                }
                break;

            case 0x2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_02: ;
#endif
                /* 02 ADD Gb Eb */
                modregrm();
                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_add8();
                putreg8(reg, res8);
                break;

            case 0x3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_03: ;
#endif
                {
                /* 03 ADD Gv Ev */
                modregrm();
                register uint32_t oper1 = getreg16(reg);
                register uint32_t oper2 = readrm16(rm);
                op_add16();
                putreg16(reg, res16);
                break;
            }
            case 0x4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_04: ;
#endif
                /* 04 ADD CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_add8();
                CPU_AL = res8;
                break;

            case 0x5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_05: ;
#endif
                {
                /* 05 ADD eAX Iv */
                register uint32_t oper1 = CPU_AX;
                register uint32_t oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_add16();
                CPU_AX = res16;
                break;
            }
            case 0x6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_06: ;
#endif
                /* 06 PUSH CPU_ES */
                push(CPU_ES);
                break;

            case 0x7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_07: ;
#endif
                /* 07 POP CPU_ES */
                r36sx_cpu_load_segment(reges, pop());
                break;

            case 0x8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_08: ;
#endif
                /* 08 OR Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_or8();
                writerm8(rm, res8
                );
                break;

            case 0x9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_09: ;
#endif
                /* 09 OR Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_or16();
                writerm16(rm, res16
                );
                break;

            case 0xA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0A: ;
#endif
                /* 0A OR Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_or8();
                putreg8(reg, res8
                );
                break;

            case 0xB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0B: ;
#endif
                /* 0B OR Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_or16();
                /*                if ((oper1 == 0xF802) && (oper2 == 0xF802)) {
                                    sf = 0;    *//* cheap hack to make Wolf 3D think we're a 286 so it plays */ /*
                }*/

                putreg16(reg, res16);
                break;

            case 0xC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0C: ;
#endif
                /* 0C OR CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_or8();
                CPU_AL = res8;
                break;

            case 0xD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0D: ;
#endif
                /* 0D OR eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_or16();
                CPU_AX = res16;
                break;

            case 0xE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0E: ;
#endif
                /* 0E PUSH CPU_CS */
                push(CPU_CS);
                break;

            case 0xF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0F: ;
#endif
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    /* 8086/8088 only: 0F POP CS. */
                    r36sx_cpu_load_segment(regcs, pop());
                } else {
                    r36sx_cpu_exec_0f(firstip);
                }
                break;

            case 0x10:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_10: ;
#endif
                /* 10 ADC Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_adc8();
                writerm8(rm, res8);
                break;

            case 0x11:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_11: ;
#endif
                /* 11 ADC Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_adc16();
                writerm16(rm, res16);
                break;

            case 0x12:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_12: ;
#endif
                /* 12 ADC Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_adc8();
                putreg8(reg, res8);
                break;

            case 0x13:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_13: ;
#endif
                /* 13 ADC Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_adc16();
                putreg16(reg, res16
                );
                break;

            case 0x14:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_14: ;
#endif
                /* 14 ADC CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_adc8();
                CPU_AL = res8;
                break;

            case 0x15:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_15: ;
#endif
                /* 15 ADC eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_adc16();
                CPU_AX = res16;
                break;

            case 0x16:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_16: ;
#endif
                /* 16 PUSH CPU_SS */
                push(CPU_SS);
                break;

            case 0x17:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_17: ;
#endif
                /* 17 POP CPU_SS */
                r36sx_cpu_load_segment(regss, pop());
                break;

            case 0x18:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_18: ;
#endif
                /* 18 SBB Eb Gb */
                modregrm();
                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_sbb8();
                writerm8(rm, res8);
                break;

            case 0x19:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_19: ;
#endif
                /* 19 SBB Ev Gv */
                modregrm();
                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_sbb16();
                writerm16(rm, res16);
                break;

            case 0x1A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1A: ;
#endif
                /* 1A SBB Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_sbb8();
                putreg8(reg, res8
                );
                break;

            case 0x1B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1B: ;
#endif
                /* 1B SBB Gv Ev */
                modregrm();
                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_sbb16();
                putreg16(reg, res16);
                break;

            case 0x1C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1C: ;
#endif
                /* 1C SBB CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_sbb8();
                CPU_AL = res8;
                break;

            case 0x1D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1D: ;
#endif
                /* 1D SBB eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_sbb16();
                CPU_AX = res16;
                break;

            case 0x1E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1E: ;
#endif
                /* 1E PUSH CPU_DS */
                push(CPU_DS);
                break;

            case 0x1F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1F: ;
#endif
                /* 1F POP CPU_DS */
                r36sx_cpu_load_segment(regds, pop());
                break;

            case 0x20:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_20: ;
#endif
                /* 20 AND Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_and8();
                writerm8(rm, res8);
                break;

            case 0x21:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_21: ;
#endif
                /* 21 AND Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_and16();
                writerm16(rm, res16
                );
                break;

            case 0x22:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_22: ;
#endif
                /* 22 AND Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_and8();
                putreg8(reg, res8
                );
                break;

            case 0x23:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_23: ;
#endif
                /* 23 AND Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_and16();
                putreg16(reg, res16
                );
                break;

            case 0x24:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_24: ;
#endif
                /* 24 AND CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_and8();
                CPU_AL = res8;
                break;

            case 0x25:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_25: ;
#endif
                /* 25 AND eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_and16();
                CPU_AX = res16;
                break;

            case 0x27:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_27: ;
#endif
                /* 27 DAA */
            {
                uint8_t old_al;
                old_al = CPU_AL;
                if (((CPU_AL & 0x0F) > 9) || af) {
                    oper1 = (uint16_t) CPU_AL + 0x06;
                    CPU_AL = oper1 & 0xFF;
                    if (oper1 & 0xFF00)
                        cf = 1;
                    if ((oper1 & 0x000F) < (old_al & 0x0F))
                        af = 1;
                }
                if (((CPU_AL & 0xF0) > 0x90) || cf) {
                    oper1 = (uint16_t) CPU_AL + 0x60;
                    CPU_AL = oper1 & 0xFF;
                    if (oper1 & 0xFF00)
                        cf = 1;
                    else
                        cf = 0;
                }
                flag_szp8(CPU_AL);
                break;
            }

            case 0x28:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_28: ;
#endif
                /* 28 SUB Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_sub8();
                writerm8(rm, res8
                );
                break;

            case 0x29:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_29: ;
#endif
                {
                /* 29 SUB Ev Gv */
                modregrm();
                register uint32_t oper1 = readrm16(rm);
                register uint32_t oper2 = getreg16(reg);
                register uint32_t dst = oper1 - oper2;
                flag_szp16((uint16_t) dst);
                cf = (dst & 0xFFFF0000) != 0;
                of = ((dst ^ oper1) & (oper1 ^ oper2) & 0x8000) != 0;
                af = ((oper1 ^ oper2 ^ dst) & 0x10) != 0;
                writerm16(rm, (uint16_t) dst);
                break;
            }
            case 0x2A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_2A: ;
#endif
                /* 2A SUB Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_sub8();
                putreg8(reg, res8
                );
                break;

            case 0x2B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_2B: ;
#endif
                /* 2B SUB Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_sub16();
                putreg16(reg, res16
                );
                break;

            case 0x2C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_2C: ;
#endif
                /* 2C SUB CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_sub8();
                CPU_AL = res8;
                break;

            case 0x2D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_2D: ;
#endif
                /* 2D SUB eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_sub16();
                CPU_AX = res16;
                break;

            case 0x2F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_2F: ;
#endif
                /* 2F DAS */
            {
                uint8_t old_al;
                old_al = CPU_AL;
                if (((CPU_AL & 0x0F) > 9) || af) {
                    oper1 = (uint16_t) CPU_AL - 0x06;
                    CPU_AL = oper1 & 0xFF;
                    if (oper1 & 0xFF00)
                        cf = 1;
                    if ((oper1 & 0x000F) >= (old_al & 0x0F))
                        af = 1;
                }
                if (((CPU_AL & 0xF0) > 0x90) || cf) {
                    oper1 = (uint16_t) CPU_AL - 0x60;
                    CPU_AL = oper1 & 0xFF;
                    if (oper1 & 0xFF00)
                        cf = 1;
                    else
                        cf = 0;
                }
                flag_szp8(CPU_AL);
                break;
            }

            case 0x30:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_30: ;
#endif
                /* 30 XOR Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_xor8();
                writerm8(rm, res8
                );
                break;

            case 0x31:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_31: ;
#endif
                /* 31 XOR Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_xor16();
                writerm16(rm, res16
                );
                break;

            case 0x32:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_32: ;
#endif
                /* 32 XOR Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_xor8();
                putreg8(reg, res8
                );
                break;

            case 0x33:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_33: ;
#endif
                /* 33 XOR Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_xor16();
                putreg16(reg, res16
                );
                break;

            case 0x34:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_34: ;
#endif
                /* 34 XOR CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_xor8();
                CPU_AL = res8;
                break;

            case 0x35:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_35: ;
#endif
                /* 35 XOR eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_xor16();
                CPU_AX = res16;
                break;

            case 0x37:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_37: ;
#endif
                /* 37 AAA ASCII */
                if (((CPU_AL & 0xF) > 9) || (af == 1)) {
                    CPU_AX = CPU_AX + 0x106;
                    x86_flags.value |= FLAG_CF_AF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_AF_MASK;
                }

                CPU_AL = CPU_AL & 0xF;
                break;

            case 0x38:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_38: ;
#endif
                /* 38 CMP Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x39:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_39: ;
#endif
                /* 39 CMP Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_3A: ;
#endif
                /* 3A CMP Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x3B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_3B: ;
#endif
                /* 3B CMP Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_3C: ;
#endif
                /* 3C CMP CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x3D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_3D: ;
#endif
                /* 3D CMP eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_3F: ;
#endif
                /* 3F AAS ASCII */
                if (((CPU_AL & 0xF) > 9) || (af == 1)) {
                    CPU_AX = CPU_AX - 6;
                    CPU_AH = CPU_AH - 1;
                    x86_flags.value |= FLAG_CF_AF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_AF_MASK;
                }

                CPU_AL = CPU_AL & 0xF;
                break;

            case 0x40:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_40: ;
#endif
                {
                /* 40 INC eAX */
                register uint32_t oper1 = CPU_AX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_AX = (uint16_t) dst;
                break;
            }
            case 0x41:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_41: ;
#endif
                {
                /* 41 INC eCX */
                register uint32_t oper1 = CPU_CX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_CX = (uint16_t) dst;
                break;
            }
            case 0x42:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_42: ;
#endif
                {
                /* 42 INC eDX */
                register uint32_t oper1 = CPU_DX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_DX = (uint16_t) dst;
                break;
            }
            case 0x43:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_43: ;
#endif
                {
                /* 43 INC eBX */
                register uint32_t oper1 = CPU_BX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_BX = (uint16_t) dst;
                break;
            }
            case 0x44:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_44: ;
#endif
                {
                /* 44 INC eSP */
                register uint32_t oper1 = CPU_SP;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_SP = (uint16_t) dst;
                break;
            }
            case 0x45:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_45: ;
#endif
                {
                /* 45 INC eBP */
                register uint32_t oper1 = CPU_BP;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_BP = (uint16_t) dst;
                break;
            }
            case 0x46:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_46: ;
#endif
                {
                /* 46 INC eSI */
                register uint32_t oper1 = CPU_SI;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_SI = (uint16_t) dst;
                break;
            }
            case 0x47:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_47: ;
#endif
                {
                /* 47 INC eDI */
                register uint32_t oper1 = CPU_DI;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_DI = (uint16_t) dst;
                break;
            }
            case 0x48:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_48: ;
#endif
                /* 48 DEC eAX */
                oldcf = cf;
                oper1 = CPU_AX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_AX = res16;
                break;

            case 0x49:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_49: ;
#endif
                /* 49 DEC eCX */
                oldcf = cf;
                oper1 = CPU_CX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_CX = res16;
                break;

            case 0x4A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4A: ;
#endif
                /* 4A DEC eDX */
                oldcf = cf;
                oper1 = CPU_DX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_DX = res16;
                break;

            case 0x4B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4B: ;
#endif
                /* 4B DEC eBX */
                oldcf = cf;
                oper1 = CPU_BX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_BX = res16;
                break;

            case 0x4C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4C: ;
#endif
                /* 4C DEC eSP */
                oldcf = cf;
                oper1 = CPU_SP;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_SP = res16;
                break;

            case 0x4D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4D: ;
#endif
                /* 4D DEC eBP */
                oldcf = cf;
                oper1 = CPU_BP;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_BP = res16;
                break;

            case 0x4E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4E: ;
#endif
                /* 4E DEC eSI */
                oldcf = cf;
                oper1 = CPU_SI;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_SI = res16;
                break;

            case 0x4F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_4F: ;
#endif
                /* 4F DEC eDI */
                oldcf = cf;
                oper1 = CPU_DI;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_DI = res16;
                break;

            case 0x50:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_50: ;
#endif
                /* 50 PUSH eAX */
                push(CPU_AX);
                break;

            case 0x51:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_51: ;
#endif
                /* 51 PUSH eCX */
                push(CPU_CX);
                break;

            case 0x52:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_52: ;
#endif
                /* 52 PUSH eDX */
                push(CPU_DX);
                break;

            case 0x53:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_53: ;
#endif
                /* 53 PUSH eBX */
                push(CPU_BX);
                break;

            case 0x54:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_54: ;
#endif
                /* 54 PUSH eSP */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    push(CPU_SP - 2);
                } else {
                    push(CPU_SP);
                }
                break;

            case 0x55:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_55: ;
#endif
                /* 55 PUSH eBP */
                push(CPU_BP);
                break;

            case 0x56:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_56: ;
#endif
                /* 56 PUSH eSI */
                push(CPU_SI);
                break;

            case 0x57:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_57: ;
#endif
                /* 57 PUSH eDI */
                push(CPU_DI);
                break;

            case 0x58:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_58: ;
#endif
                /* 58 POP eAX */
                CPU_AX = pop();
                break;

            case 0x59:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_59: ;
#endif
                /* 59 POP eCX */
                CPU_CX = pop();
                break;

            case 0x5A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5A: ;
#endif
                /* 5A POP eDX */
                CPU_DX = pop();
                break;

            case 0x5B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5B: ;
#endif
                /* 5B POP eBX */
                CPU_BX = pop();
                break;

            case 0x5C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5C: ;
#endif
                /* 5C POP eSP */
                CPU_SP = pop();
                break;

            case 0x5D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5D: ;
#endif
                /* 5D POP eBP */
                CPU_BP = pop();
                break;

            case 0x5E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5E: ;
#endif
                /* 5E POP eSI */
                CPU_SI = pop();
                break;

            case 0x5F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_5F: ;
#endif
                /* 5F POP eDI */
                CPU_DI = pop();
                break;

#ifndef CPU_8086
            case 0x60:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_60: ;
#endif
                /* 60 PUSHA (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (operandSizeOverride) {
                    uint32_t oldesp = CPU_ESP;
                    push32(CPU_EAX);
                    push32(CPU_ECX);
                    push32(CPU_EDX);
                    push32(CPU_EBX);
                    push32(oldesp);
                    push32(CPU_EBP);
                    push32(CPU_ESI);
                    push32(CPU_EDI);
                    break;
                }
                oldsp = CPU_SP;
                push(CPU_AX);
                push(CPU_CX);
                push(CPU_DX);
                push(CPU_BX);
                push(oldsp);
                push(CPU_BP);
                push(CPU_SI);
                push(CPU_DI);
                break;

            case 0x61:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_61: ;
#endif
                /* 61 POPA (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (operandSizeOverride) {
                    CPU_EDI = pop32();
                    CPU_ESI = pop32();
                    CPU_EBP = pop32();
                    CPU_SP += 4;
                    CPU_EBX = pop32();
                    CPU_EDX = pop32();
                    CPU_ECX = pop32();
                    CPU_EAX = pop32();
                    break;
                }
                CPU_DI = pop();
                CPU_SI = pop();
                CPU_BP = pop();
                CPU_SP += 2;
                CPU_BX = pop();
                CPU_DX = pop();
                CPU_CX = pop();
                CPU_AX = pop();
                break;

            case 0x62:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_62: ;
#endif
                /* 62 BOUND Gv, Ev (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                getea(rm);
                if (
                    signext32(getreg16(reg)
                    ) <
                    signext32(getmem16(ea >> 4, ea & 15)
                    )) {
                    intcall86(5); //bounds check exception
                } else {
                    ea += 2;
                    if (
                        signext32(getreg16(reg)
                        ) >
                        signext32(getmem16(ea >> 4, ea & 15)
                        )) {
                        intcall86(5); //bounds check exception
                    }
                }
                break;
#if CPU_386_EXTENDED_OPS
            case 0x66:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_66: ;
#endif
                /* Operand-Size Override (???????? ?????? ?????????: 16 ? 32 ???) */
                if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                operandSizeOverride = true;
                break;
            case 0x67:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_67: ;
#endif
                /* Address-Size Override (???????? ?????? ??????: 16 ? 32 ???) */
                if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                addressSizeOverride = true;
                break;
#endif
            case 0x68:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_68: ;
#endif
                /* 68 PUSH Iv (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                push(getmem16(CPU_CS, CPU_IP)
                );
                StepIP(2);
                break;

            case 0x69:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_69: ;
#endif
                {
                /* 69 IMUL Gv Ev Iv (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                register int32_t temp1 = (int32_t)(int16_t)readrm16(rm);
                register int32_t temp2 = (int32_t)(int16_t)getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                temp1 *= temp2;
                putreg16(reg, (int16_t)temp1);
                if (temp1 != (int32_t)(int16_t)temp1) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
                break;
            }
            case 0x6A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6A: ;
#endif
                /* 6A PUSH Ib (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                push((uint16_t) signext(getmem8(CPU_CS, CPU_IP)));
                StepIP(1);
                break;

            case 0x6B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6B: ;
#endif
                {
                /* 6B IMUL Gv Eb Ib (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                register int32_t temp1 = (int32_t)(int16_t)readrm16(rm);
                register int32_t temp2 = (int32_t)(int16_t)signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                temp1 *= temp2;
				putreg16(reg, (int16_t)temp1);
                if (temp1 != (int32_t)(int16_t)temp1) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
                break;
            }
            case 0x6C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6C: ;
#endif
                /* 6C INSB */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }

                putmem8(CPU_ES, CPU_DI, portin(CPU_DX));
                if (df) {
                    CPU_DI = CPU_DI - 1;
                } else {
                    CPU_DI = CPU_DI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0x6D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6D: ;
#endif
                /* 6D INSW */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }

                putmem16(CPU_ES, CPU_DI, portin16(CPU_DX));
                if (df) {
                    CPU_DI = CPU_DI - 2;
                } else {
                    CPU_DI = CPU_DI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0x6E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6E: ;
#endif
                /* 6E OUTSB */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }

                portout(CPU_DX, getmem8(useseg, CPU_SI));
                if (df) {
                    CPU_SI = CPU_SI - 1;
                } else {
                    CPU_SI = CPU_SI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0x6F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_6F: ;
#endif
                /* 6F OUTSW */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }

                portout16(CPU_DX, getmem16(useseg, CPU_SI));
                if (df) {
                    CPU_SI = CPU_SI - 2;
                } else {
                    CPU_SI = CPU_SI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;
#endif

            case 0x70:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_70: ;
#endif
                /* 70 JO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (of) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x71:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_71: ;
#endif
                /* 71 JNO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!of) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x72:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_72: ;
#endif
                /* 72 JB Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (cf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x73:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_73: ;
#endif
                /* 73 JNB Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!cf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x74:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_74: ;
#endif
                /* 74 JZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x75:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_75: ;
#endif
                /* 75 JNZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x76:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_76: ;
#endif
                /* 76 JBE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (cf || zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x77:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_77: ;
#endif
                /* 77 JA Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!cf && !zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x78:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_78: ;
#endif
                /* 78 JS Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x79:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_79: ;
#endif
                /* 79 JNS Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!sf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7A: ;
#endif
                /* 7A JPE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (pf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7B: ;
#endif
                /* 7B JPO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!pf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7C: ;
#endif
                /* 7C JL Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf != of) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7D: ;
#endif
                /* 7D JGE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf == of) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7E: ;
#endif
                /* 7E JLE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if ((sf != of) || zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x7F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_7F: ;
#endif
                /* 7F JG Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!
                    zf && (sf
                           == of)) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0x80:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_80: ;
#endif
            case 0x82:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_82: ;
#endif
                /* 80/82 GRP1 Eb Ib */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                switch (reg) {
                    case 0:
                        op_add8();
                        break;
                    case 1:
                        op_or8();
                        break;
                    case 2:
                        op_adc8();
                        break;
                    case 3:
                        op_sbb8();
                        break;
                    case 4:
                        op_and8();
                        break;
                    case 5:
                        op_sub8();
                        break;
                    case 6:
                        op_xor8();
                        break;
                    case 7:
                        flag_sub8(oper1b, oper2b
                        );
                        break;
                    default:
                        break; /* to avoid compiler warnings */
                }

                if (reg < 7) {
                    writerm8(rm, res8
                    );
                }
                break;

            case 0x81:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_81: ;
#endif
                /* 81 GRP1 Ev Iv */
            case 0x83:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_83: ;
#endif
                /* 83 GRP1 Ev Ib */
                modregrm();

                oper1 = readrm16(rm);
                if (opcode == 0x81) {
                    oper2 = getmem16(CPU_CS, CPU_IP);
                    StepIP(2);
                } else {
                    oper2 = signext(getmem8(CPU_CS, CPU_IP));
                    StepIP(1);
                }

                switch (reg) {
                    case 0:
                        op_add16();
                        break;
                    case 1:
                        op_or16();
                        break;
                    case 2:
                        op_adc16();
                        break;
                    case 3:
                        op_sbb16();
                        break;
                    case 4:
                        op_and16();
                        break;
                    case 5:
                        op_sub16();
                        break;
                    case 6:
                        op_xor16();
                        break;
                    case 7:
                        flag_sub16(oper1, oper2
                        );
                        break;
                    default:
                        break; /* to avoid compiler warnings */
                }

                if (reg < 7) {
                    writerm16(rm, res16
                    );
                }
                break;

            case 0x84:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_84: ;
#endif
                /* 84 TEST Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                flag_log8(oper1b
                          & oper2b);
                break;

            case 0x85:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_85: ;
#endif
                /* 85 TEST Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                flag_log16(oper1
                           & oper2);
                break;

            case 0x86:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_86: ;
#endif
                /* 86 XCHG Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                putreg8(reg, readrm8(rm)
                );
                writerm8(rm, oper1b
                );
                break;

            case 0x87:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_87: ;
#endif
                /* 87 XCHG Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                putreg16(reg, readrm16(rm)
                );
                writerm16(rm, oper1
                );
                break;

            case 0x88:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_88: ;
#endif
                /* 88 MOV Eb Gb */
                modregrm();

                writerm8(rm, getreg8(reg)
                );
                break;

            case 0x89:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_89: ;
#endif
                /* 89 MOV Ev Gv */
                modregrm();

                writerm16(rm, getreg16(reg)
                );
                break;

            case 0x8A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8A: ;
#endif
                /* 8A MOV Gb Eb */
                modregrm();

                putreg8(reg, readrm8(rm)
                );
                break;

            case 0x8B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8B: ;
#endif
                /* 8B MOV Gv Ev */
                modregrm();

                putreg16(reg, readrm16(rm)
                );
                break;

            case 0x8C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8C: ;
#endif
                /* 8C MOV Ew Sw */
                modregrm();
                if ((!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) && reg > regds) ||
                    reg > reggs) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                writerm16(rm, getsegreg(reg)
                );
                break;

            case 0x8D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8D: ;
#endif
                /* 8D LEA Gv M */
                modregrm();

                getea(rm);
                putreg16(reg, ea
                         -
                         segbase(useseg)
                );
                break;

            case 0x8E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8E: ;
#endif
                /* 8E MOV Sw Ew */
                modregrm();
                if (reg == regcs ||
                    (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) && reg > regds) ||
                    reg > reggs) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                r36sx_cpu_load_segment(reg, readrm16(rm)
                );
                break;

            case 0x8F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8F: ;
#endif
                /* 8F POP Ev */
                modregrm();

                writerm16(rm, pop()
                );
                break;

            case 0x90:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_90: ;
#endif
                /* 90 NOP */
                break;

            case 0x91:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_91: ;
#endif
                /* 91 XCHG eCX eAX */
                oper1 = CPU_CX;
                CPU_CX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x92:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_92: ;
#endif
                /* 92 XCHG eDX eAX */
                oper1 = CPU_DX;
                CPU_DX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x93:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_93: ;
#endif
                /* 93 XCHG eBX eAX */
                oper1 = CPU_BX;
                CPU_BX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x94:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_94: ;
#endif
                /* 94 XCHG eSP eAX */
                oper1 = CPU_SP;
                CPU_SP = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x95:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_95: ;
#endif
                /* 95 XCHG eBP eAX */
                oper1 = CPU_BP;
                CPU_BP = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x96:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_96: ;
#endif
                /* 96 XCHG eSI eAX */
                oper1 = CPU_SI;
                CPU_SI = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x97:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_97: ;
#endif
                /* 97 XCHG eDI eAX */
                oper1 = CPU_DI;
                CPU_DI = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x98:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_98: ;
#endif
                /* 98 CBW */
                if ((CPU_AL & 0x80) == 0x80) {
                    CPU_AH = 0xFF;
                } else {
                    CPU_AH = 0;
                }
                break;

            case 0x99:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_99: ;
#endif
                /* 99 CWD */
                if ((CPU_AH & 0x80) == 0x80) {
                    CPU_DX = 0xFFFF;
                } else {
                    CPU_DX = 0;
                }
                break;

            case 0x9A:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9A: ;
#endif
                /* 9A CALL Ap */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                push(CPU_CS);
                push(CPU_IP);
                CPU_IP = oper1;
                r36sx_cpu_load_segment(regcs, oper2);
                break;

            case 0x9B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9B: ;
#endif
                /* 9B WAIT */
                /// TODO:
                break;

            case 0x9C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9C: ;
#endif
                /* 9C PUSHF */
                if (operandSizeOverride) {
                    push32(makeflagsdword());
                    break;
                }
                push(makeflagsword());
                break;

            case 0x9D:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9D: ;
#endif
                /* 9D POPF */
                if (operandSizeOverride) {
                    decodeflagsdword(pop32());
                    break;
                }
#ifdef CPU_SET_HIGH_FLAGS
                decodeflagsword(pop() | 0xF800);
#else
                decodeflagsword(pop() & 0x0FFF);
#endif
                break;

            case 0x9E:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9E: ;
#endif
                /* 9E SAHF */
                decodeflagsword((makeflagsword() & 0xFF00) | CPU_AH);
                break;

            case 0x9F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9F: ;
#endif
                /* 9F LAHF */
                CPU_AH = makeflagsword() & 0xFF;
                break;

            case 0xA0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A0: ;
#endif
                /* A0 MOV CPU_AL Ob */
                CPU_AL = getmem8(useseg, getmem16(CPU_CS, CPU_IP));
                StepIP(2);
                break;

            case 0xA1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A1: ;
#endif
                /* A1 MOV eAX Ov */
                oper1 = getmem16(useseg, getmem16(CPU_CS, CPU_IP));
                StepIP(2);
                CPU_AX = oper1;
                break;

            case 0xA2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A2: ;
#endif
                /* A2 MOV Ob CPU_AL */
                putmem8(useseg, getmem16(CPU_CS, CPU_IP), CPU_AL);
                StepIP(2);
                break;

            case 0xA3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A3: ;
#endif
                /* A3 MOV Ov eAX */
                putmem16(useseg, getmem16(CPU_CS, CPU_IP), CPU_AX);
                StepIP(2);
                break;

            case 0xA4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A4: ;
#endif
                /* A4 MOVSB */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(CPU_CX, loopcount,
                                                    execloops);
                    r36sx_rep_movsb(batch);
                    CPU_CX = (uint16_t)(CPU_CX - batch);
                    loopcount += batch;
                    if (CPU_CX != 0) {
                        CPU_IP = firstip;
                    }
                    break;
                }

                r36sx_rep_movsb(1);
                loopcount++;
                break;

            case 0xA5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A5: ;
#endif
                /* A5 MOVSW */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(CPU_CX, loopcount,
                                                    execloops);
                    r36sx_rep_movsw(batch);
                    CPU_CX = (uint16_t)(CPU_CX - batch);
                    loopcount += batch;
                    if (CPU_CX != 0) {
                        CPU_IP = firstip;
                    }
                    break;
                }

                r36sx_rep_movsw(1);
                loopcount++;
                break;

            case 0xA6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A6: ;
#endif
                /* A6 CMPSB */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                oper1b = getmem8(useseg, CPU_SI);
                oper2b = getmem8(CPU_ES, CPU_DI);
                if (df) {
                    CPU_SI = CPU_SI - 1;
                    CPU_DI = CPU_DI - 1;
                } else {
                    CPU_SI = CPU_SI + 1;
                    CPU_DI = CPU_DI + 1;
                }

                flag_sub8(oper1b, oper2b
                );
                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xA7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A7: ;
#endif
                /* A7 CMPSW */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                oper1 = getmem16(useseg, CPU_SI);
                oper2 = getmem16(CPU_ES, CPU_DI);
                if (df) {
                    CPU_SI = CPU_SI - 2;
                    CPU_DI = CPU_DI - 2;
                } else {
                    CPU_SI = CPU_SI + 2;
                    CPU_DI = CPU_DI + 2;
                }

                flag_sub16(oper1, oper2
                );
                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if ((reptype == 1) && !zf) {
                    break;
                }

                if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xA8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A8: ;
#endif
                /* A8 TEST CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                flag_log8(oper1b
                          & oper2b);
                break;

            case 0xA9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A9: ;
#endif
                /* A9 TEST eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                flag_log16(oper1
                           & oper2);
                break;

            case 0xAA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AA: ;
#endif
                /* AA STOSB */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(CPU_CX, loopcount,
                                                    execloops);
                    r36sx_rep_stosb(batch);
                    CPU_CX = (uint16_t)(CPU_CX - batch);
                    loopcount += batch;
                    if (CPU_CX != 0) {
                        CPU_IP = firstip;
                    }
                    break;
                }

                r36sx_rep_stosb(1);
                loopcount++;
                break;

            case 0xAB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AB: ;
#endif
                /* AB STOSW */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(CPU_CX, loopcount,
                                                    execloops);
                    r36sx_rep_stosw(batch);
                    CPU_CX = (uint16_t)(CPU_CX - batch);
                    loopcount += batch;
                    if (CPU_CX != 0) {
                        CPU_IP = firstip;
                    }
                    break;
                }

                r36sx_rep_stosw(1);
                loopcount++;
                break;

            case 0xAC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AC: ;
#endif
                /* AC LODSB */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                CPU_AL = getmem8(useseg, CPU_SI);
                if (df) {
                    CPU_SI = CPU_SI - 1;
                } else {
                    CPU_SI = CPU_SI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xAD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AD: ;
#endif
                /* AD LODSW */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                oper1 = getmem16(useseg, CPU_SI);
                CPU_AX = oper1;
                if (df) {
                    CPU_SI = CPU_SI - 2;
                } else {
                    CPU_SI = CPU_SI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xAE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AE: ;
#endif
                /* AE SCASB */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                oper1b = CPU_AL;
                oper2b = getmem8(CPU_ES, CPU_DI);
                flag_sub8(oper1b, oper2b
                );
                if (df) {
                    CPU_DI = CPU_DI - 1;
                } else {
                    CPU_DI = CPU_DI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xAF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AF: ;
#endif
                /* AF SCASW */
                if (
                    reptype && (CPU_CX
                                == 0)) {
                    break;
                }

                oper1 = CPU_AX;
                oper2 = getmem16(CPU_ES, CPU_DI);
                flag_sub16(oper1, oper2
                );
                if (df) {
                    CPU_DI = CPU_DI - 2;
                } else {
                    CPU_DI = CPU_DI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    //did i fix a typo bug? this used to be & instead of &&
                    break;
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                CPU_IP = firstip;
                break;

            case 0xB0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B0: ;
#endif
                /* B0 MOV CPU_AL Ib */
                CPU_AL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B1: ;
#endif
                /* B1 MOV CPU_CL Ib */
                CPU_CL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B2: ;
#endif
                /* B2 MOV CPU_DL Ib */
                CPU_DL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B3: ;
#endif
                /* B3 MOV CPU_BL Ib */
                CPU_BL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B4: ;
#endif
                /* B4 MOV CPU_AH Ib */
                CPU_AH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B5: ;
#endif
                /* B5 MOV CPU_CH Ib */
                CPU_CH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B6: ;
#endif
                /* B6 MOV CPU_DH Ib */
                CPU_DH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B7: ;
#endif
                /* B7 MOV CPU_BH Ib */
                CPU_BH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B8: ;
#endif
                /* B8 MOV eAX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_AX = oper1;
                break;

            case 0xB9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_B9: ;
#endif
                /* B9 MOV eCX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_CX = oper1;
                break;

            case 0xBA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BA: ;
#endif
                /* BA MOV eDX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_DX = oper1;
                break;

            case 0xBB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BB: ;
#endif
                /* BB MOV eBX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_BX = oper1;
                break;

            case 0xBC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BC: ;
#endif
                /* BC MOV eSP Iv */
                CPU_SP = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BD: ;
#endif
                /* BD MOV eBP Iv */
                CPU_BP = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BE: ;
#endif
                /* BE MOV eSI Iv */
                CPU_SI = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_BF: ;
#endif
                /* BF MOV eDI Iv */
                CPU_DI = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xC0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C0: ;
#endif
                /* C0 GRP2 byte imm8 (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm8(rm, op_grp2_8(oper2b, oper1b));
                break;

            case 0xC1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C1: ;
#endif
                /* C1 GRP2 word imm8 (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm16(rm, op_grp2_16((uint8_t) oper2)
                );
                break;

            case 0xC2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C2: ;
#endif
                /* C2 RET Iw */
                oper1 = getmem16(CPU_CS, CPU_IP);
                CPU_IP = pop();
                CPU_SP = CPU_SP + oper1;
                break;

            case 0xC3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C3: ;
#endif
                /* C3 RET */
                CPU_IP = pop();
                break;

            case 0xC4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C4: ;
#endif
                /* C4 LES Gv Mp */
                modregrm();

                getea(rm);
                putreg16(reg, read86(ea) + read86(ea + 1) * 256);
                r36sx_cpu_load_segment(
                    reges, read86(ea + 2) + read86(ea + 3) * 256);
                break;

            case 0xC5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C5: ;
#endif
                /* C5 LDS Gv Mp */
                modregrm();

                getea(rm);
                putreg16(reg, read86(ea) + read86(ea + 1) * 256);
                r36sx_cpu_load_segment(
                    regds, read86(ea + 2) + read86(ea + 3) * 256);
                break;

            case 0xC6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C6: ;
#endif
                /* C6 MOV Eb Ib */
                modregrm();

                writerm8(rm, getmem8(CPU_CS, CPU_IP)
                );
                StepIP(1);
                break;

            case 0xC7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C7: ;
#endif
                /* C7 MOV Ev Iv */
                modregrm();

                writerm16(rm, getmem16(CPU_CS, CPU_IP)
                );
                StepIP(2);
                break;

            case 0xC8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C8: ;
#endif
                /* C8 ENTER (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                stacksize = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                nestlev = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                push(CPU_BP);
                frametemp = CPU_SP;
                if (nestlev) {
                    for (
                        temp16 = 1;
                        temp16 < nestlev;
                        ++temp16) {
                        CPU_BP = CPU_BP - 2;
                        push(CPU_BP);
                    }

                    push(frametemp); //CPU_SP);
                }

                CPU_BP = frametemp;
                CPU_SP = CPU_BP - stacksize;

                break;

            case 0xC9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C9: ;
#endif
                /* C9 LEAVE (80186+) */
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                CPU_SP = CPU_BP;
                CPU_BP = pop();
                break;

            case 0xCA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CA: ;
#endif
                /* CA RETF Iw */
                oper1 = getmem16(CPU_CS, CPU_IP);
                CPU_IP = pop();
                r36sx_cpu_load_segment(regcs, pop());
                CPU_SP = CPU_SP + oper1;
                break;

            case 0xCB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CB: ;
#endif
                /* CB RETF */
                CPU_IP = pop();
                r36sx_cpu_load_segment(regcs, pop());
                break;

            case 0xCC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CC: ;
#endif
                /* CC INT 3 */
                intcall86(3);
                break;

            case 0xCD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CD: ;
#endif
                /* CD INT Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                intcall86(oper1b);
                break;

            case 0xCE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CE: ;
#endif
                /* CE INTO */
                if (of) {
                    intcall86(4);
                }
                break;

            case 0xCF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CF: ;
#endif
                /* CF IRET */
                CPU_IP = pop();
                r36sx_cpu_load_segment(regcs, pop());
#ifdef CPU_SET_HIGH_FLAGS
                decodeflagsword(pop() | 0xF000);
#else
                decodeflagsword(pop() & 0x0FFF);
#endif


                /*
                 * if (net.enabled) net.canrecv = 1;
                 */
                break;

            case 0xD0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D0: ;
#endif
                /* D0 GRP2 Eb 1 */
                modregrm();

                oper1b = readrm8(rm);
                writerm8(rm, op_grp2_8(1, oper1b));
                break;

            case 0xD1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D1: ;
#endif
                /* D1 GRP2 Ev 1 */
                modregrm();

                oper1 = readrm16(rm);
                writerm16(rm, op_grp2_16(1));
                break;

            case 0xD2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D2: ;
#endif
                /* D2 GRP2 Eb CPU_CL */
                modregrm();

                oper1b = readrm8(rm);
                writerm8(rm, op_grp2_8(CPU_CL, oper1b));
                break;

            case 0xD3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D3: ;
#endif
                /* D3 GRP2 Ev CPU_CL */
                modregrm();

                oper1 = readrm16(rm);
                writerm16(rm, op_grp2_16(CPU_CL)
                );
                break;

            case 0xD4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D4: ;
#endif
                /* D4 AAM I0 */
                oper1 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!oper1) {
                    intcall86(0);
                    break;
                } /* division by zero */

                CPU_AH = (CPU_AL / oper1) & 255;
                CPU_AL = (CPU_AL % oper1) & 255;
                flag_szp16(CPU_AX);
                break;

            case 0xD5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D5: ;
#endif
                /* D5 AAD I0 */
                oper1 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                CPU_AL = (CPU_AH * oper1 + CPU_AL) & 255;
                CPU_AH = 0;
                flag_szp16(CPU_AH
                           * oper1 + CPU_AL);
                sf = 0;
                break;

            case 0xD6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D6: ;
#endif
                /* D6 XLAT on V20/V30, SALC on 8086/8088 */
#ifndef CPU_NO_SALC
                CPU_AL = CPU_FL_CF ? 0xFF : 0x00;
                break;
#else
                /* Undefined on 80286; documented as a no-op compatibility hole. */
                break;
#endif

            case 0xD7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D7: ;
#endif
                /* D7 XLAT */
                CPU_AL = getmem8(useseg, (uint16_t)(CPU_BX + CPU_AL));
                break;

            case 0xD8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D8: ;
#endif
            case 0xD9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D9: ;
#endif
            case 0xDA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DA: ;
#endif
            case 0xDB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DB: ;
#endif
            case 0xDC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DC: ;
#endif
            case 0xDE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DE: ;
#endif
            case 0xDD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DD: ;
#endif
            case 0xDF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_DF: ;
#endif
                /* escape to x87 FPU */
                OpFpu(opcode);
                break;

            case 0xE0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E0: ;
#endif
                /* E0 LOOPNZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                CPU_CX = CPU_CX - 1;
                if ((CPU_CX) && !zf) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0xE1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E1: ;
#endif
                /* E1 LOOPZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                CPU_CX = CPU_CX - 1;
                if (CPU_CX && (zf == 1)) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0xE2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E2: ;
#endif
                /* E2 LOOP Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                CPU_CX = CPU_CX - 1;
                if (CPU_CX) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0xE3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E3: ;
#endif
                /* E3 JCXZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!CPU_CX) {
                    CPU_IP = CPU_IP + temp16;
                }
                break;

            case 0xE4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E4: ;
#endif
                /* E4 IN CPU_AL Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                CPU_AL = (uint8_t) portin(oper1b);
                break;

            case 0xE5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E5: ;
#endif
                /* E5 IN eAX Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                CPU_AX = portin16(oper1b);
                break;

            case 0xE6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E6: ;
#endif
                /* E6 OUT Ib CPU_AL */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                portout(oper1b, CPU_AL
                );
                break;

            case 0xE7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E7: ;
#endif
                /* E7 OUT Ib eAX */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                portout16(oper1b, CPU_AX
                );
                break;

            case 0xE8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E8: ;
#endif
                /* E8 CALL Jv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                push(CPU_IP);
                CPU_IP = CPU_IP + oper1;
                break;

            case 0xE9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E9: ;
#endif
                /* E9 JMP Jv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_IP = CPU_IP + oper1;
                break;

            case 0xEA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EA: ;
#endif
                /* EA JMP Ap */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                oper2 = getmem16(CPU_CS, CPU_IP);
                CPU_IP = oper1;
                r36sx_cpu_load_segment(regcs, oper2);
                break;

            case 0xEB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EB: ;
#endif
                /* EB JMP Jb */
                oper1 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                CPU_IP = CPU_IP + oper1;
                break;

            case 0xEC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EC: ;
#endif
                /* EC IN CPU_AL regdx */
                oper1 = CPU_DX;
                CPU_AL = (uint8_t) portin(oper1);
                break;

            case 0xED:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_ED: ;
#endif
                /* ED IN eAX regdx */
                oper1 = CPU_DX;
                CPU_AX = portin16(oper1);
                break;

            case 0xEE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EE: ;
#endif
                /* EE OUT regdx CPU_AL */
                oper1 = CPU_DX;
                portout(oper1, CPU_AL
                );
                break;

            case 0xEF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EF: ;
#endif
                /* EF OUT regdx eAX */
                oper1 = CPU_DX;
                portout16(oper1, CPU_AX);
                break;

            case 0xF0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F0: ;
#endif
                /* F0 LOCK */
                break;

            case 0xF4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F4: ;
#endif
                /* F4 HLT */
                hltstate = 1;
                return;

            case 0xF5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F5: ;
#endif
                /* F5 CMC */
                if (!cf) {
                    cf = 1;
                } else {
                    cf = 0;
                }
                break;

            case 0xF6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F6: ;
#endif
                /* F6 GRP3a Eb */
                modregrm();
                oper1b = readrm8(rm);
                oper1 = signext(oper1b);
                switch (reg) {
                    case 0:
                    case 1: /* TEST */
                        flag_log8(oper1b & getmem8(CPU_CS, CPU_IP));
                        StepIP(1);
                        break;

                    case 2: /* NOT */
                        res8 = ~oper1b;
                        break;

                    case 3: /* NEG */
                        res8 = (~oper1b) + 1;
                        flag_sub8(0, oper1b);
                        if (res8 == 0) {
                            cf = 0;
                        } else {
                            cf = 1;
                        }
                        break;

                    case 4: {
                        /* MUL */
                        register uint32_t temp1 = (uint32_t) oper1b * (uint32_t) CPU_AL;
                        CPU_AX = temp1 & 0xFFFF;
                        flag_szp8((uint8_t) temp1);
                        if (CPU_AH) {
                            x86_flags.value |= FLAG_CF_OF_MASK;
                        } else {
                            x86_flags.value &= ~FLAG_CF_OF_MASK;
                        }
#ifdef CPU_CLEAR_ZF_ON_MUL
                        zf = 0;
#endif
                        break;
                    }
                    case 5: {
                        /* IMUL */
                        oper1 = signext(oper1b);
                        register int32_t temp1 = (int32_t)(int8_t)signext(CPU_AL);
                        register int32_t temp2 = (int32_t)(int8_t)oper1;
						temp1 *= temp2;
						int16_t result = (int16_t)temp1;
						int8_t truncated = (int8_t)result;
						if (result != (int16_t)truncated) {
							x86_flags.value |= FLAG_CF_OF_MASK; // CF=OF=1
						} else {
							x86_flags.value &= ~FLAG_CF_OF_MASK; // CF=OF=0
						}
						CPU_AL = truncated;
						CPU_AH = (uint8_t)(result >> 8);
#ifdef CPU_CLEAR_ZF_ON_MUL
                        zf = 0;
#endif
                        break;
                    }
                    case 6: /* DIV */
                        op_div8(CPU_AX, oper1b);
                        break;

                    case 7: /* IDIV */
                        op_idiv8(CPU_AX, oper1b);
                        break;
                }

                if ((reg > 1) && (reg < 4)) {
                    writerm8(rm, res8
                    );
                }
                break;

            case 0xF7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F7: ;
#endif
                /* F7 GRP3b Ev */
                modregrm();

                oper1 = readrm16(rm);
                op_grp3_16();
                if ((reg > 1) && (reg < 4)) {
                    writerm16(rm, res16
                    );
                }
                break;

            case 0xF8:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F8: ;
#endif
                /* F8 CLC */
                cf = 0;
                break;

            case 0xF9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F9: ;
#endif
                /* F9 STC */
                cf = 1;
                break;

            case 0xFA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FA: ;
#endif
                /* FA CLI */
                ifl = 0;
                break;

            case 0xFB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FB: ;
#endif
                /* FB STI */
                ifl = 1;
                break;

            case 0xFC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FC: ;
#endif
                /* FC CLD */
                df = 0;
                break;

            case 0xFD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FD: ;
#endif
                /* FD STD */
                df = 1;
                break;

            case 0xFE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FE: ;
#endif
                /* FE GRP4 Eb */
                modregrm();
                oper1b = readrm8(rm);
                oper2b = 1;
                if (!reg) {
                    tempcf = cf;
                    op_add8();
                    cf = tempcf;
                    writerm8(rm, res8);
                } else {
                    tempcf = cf;
                    res8 = oper1b - oper2b;
                    flag_sub8(oper1b, oper2b);
                    cf = tempcf;
                    writerm8(rm, res8);
                }
                break;

            case 0xFF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FF: ;
#endif
                /* FF GRP5 Ev */
                modregrm();

                oper1 = readrm16(rm);
                op_grp5();
                break;

            default:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_default: ;
#endif
#ifdef CPU_ALLOW_ILLEGAL_OP_EXCEPTION
                CPU_IP = firstip;
                intcall86(6); /* trip invalid opcode exception. this occurs on the 80186+, 8086/8088 CPUs treat them as NOPs. */
                /* technically they aren't exactly like NOPs in most cases, but for our pursoses, that's accurate enough. */
                r36sx_pico286_debug_log("[CPU] Invalid opcode 0x%02x exception at %04X:%04X",
                                        opcode, CPU_CS, firstip);
#endif
                break;
        }
r36sx_opcode_done:
        if (was_TF) {
            was_TF = false;
            intcall86(1);
        }
        if (tf) {
            was_TF = true;
        }
    }
    r36sx_app_stats_record_x86(loopcount);
}
