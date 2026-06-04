#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "emulator.h"
#include "r36sx_debug_config.h"
#include "r36sx_disk_config.h"

#define CPU_ALLOW_ILLEGAL_OP_EXCEPTION
#define CPU_LIMIT_SHIFT_COUNT
#define CPU_NO_SALC
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
uint16_t segselector16[6];
uint32_t segbase32[6];
uint16_t useseg, oldsp;
uint32_t useseg_base;
uint32_t ip32;
uint8_t tempcf, oldcf, mode, reg, rm, sib;
x86_flags_t x86_flags;
bool operandSizeOverride = false;
bool addressSizeOverride = false;
static volatile uint8_t hltstate;
static uint8_t r36sx_cpu_maskable_interrupt_shadow;
static uint8_t r36sx_cpu_interpreter_protected;
static uint8_t r36sx_cpu_current_cpl;
static uint8_t r36sx_cpu_strict_8086_mode;

static inline uint8_t r36sx_cpu_x87_present(void)
{
#if PICO_ON_DEVICE
    return 1u;
#else
    return r36sx_pico286_x87_enabled() ? 1u : 0u;
#endif
}

#if R36SX_DEBUG_PM_DIAG
#define R36SX_PM_DIAG_LOG(...) r36sx_pico286_debug_log(__VA_ARGS__)
#else
#define R36SX_PM_DIAG_LOG(...) ((void)0)
#endif

static const uint8_t __not_in_flash("cpu.regt") byteregtable[8] = {
    regal, regcl, regdl, regbl, regah, regch, regdh, regbh
};

uint8_t nestlev;
uint16_t saveip, savecs, oper1, oper2, res16, temp16, dummy, stacksize;
uint32_t disp32;
#define disp16 (*(uint16_t*)&disp32)
uint32_t ea;

uint32_t dwordregs[8];

#if !PICO_ON_DEVICE
static uint8_t r36sx_bios_rtc_to_bcd(int value)
{
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

static int r36sx_bios_rtc_from_bcd(uint8_t value,
                                   int min_value,
                                   int max_value,
                                   int *decoded)
{
    int high = (value >> 4) & 0x0f;
    int low = value & 0x0f;
    int result;

    if (high > 9 || low > 9) {
        return 0;
    }

    result = high * 10 + low;
    if (result < min_value || result > max_value) {
        return 0;
    }

    *decoded = result;
    return 1;
}

static int r36sx_bios_rtc_get_time(struct tm *tm_value)
{
    time_t rtc_now;

    if (!r36sx_pico286_rtc_enabled()) {
        return 0;
    }

    rtc_now = (time_t)r36sx_pico286_rtc_current_time_unix();
    return localtime_r(&rtc_now, tm_value) != NULL;
}

static int r36sx_bios_rtc_set_time(struct tm *tm_value)
{
    int year = tm_value->tm_year;
    int month = tm_value->tm_mon;
    int day = tm_value->tm_mday;
    int hour = tm_value->tm_hour;
    int minute = tm_value->tm_min;
    int second = tm_value->tm_sec;
    time_t parsed;

    tm_value->tm_isdst = -1;
    parsed = mktime(tm_value);
    if (parsed == (time_t)-1 ||
        tm_value->tm_year != year ||
        tm_value->tm_mon != month ||
        tm_value->tm_mday != day ||
        tm_value->tm_hour != hour ||
        tm_value->tm_min != minute ||
        tm_value->tm_sec != second) {
        return 0;
    }

    return r36sx_pico286_rtc_set_time_unix((int64_t)parsed);
}

static void r36sx_bios_rtc_success(void)
{
    CPU_AH = 0;
    CPU_FL_CF = 0;
}

static void r36sx_bios_rtc_failure(void)
{
    CPU_AH = 0x80;
    CPU_FL_CF = 1;
}

static int r36sx_bios_rtc_int1a(void)
{
    struct tm tm_value;
    int hour;
    int minute;
    int second;
    int century;
    int year;
    int month;
    int day;
    int full_year;

    /*
     * IBM-compatible BIOS INT 1Ah RTC services exchange packed BCD values:
     * CH/CL/DH are hour/minute/second for time and century/year/month for date.
     */
    switch (CPU_AH) {
        case 0x02: /* Read Real-Time Clock time */
            if (!r36sx_bios_rtc_get_time(&tm_value)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            CPU_CH = r36sx_bios_rtc_to_bcd(tm_value.tm_hour);
            CPU_CL = r36sx_bios_rtc_to_bcd(tm_value.tm_min);
            CPU_DH = r36sx_bios_rtc_to_bcd(tm_value.tm_sec);
            CPU_DL = 0;
            r36sx_bios_rtc_success();
            return 1;

        case 0x03: /* Set Real-Time Clock time */
            if (!r36sx_bios_rtc_get_time(&tm_value) ||
                !r36sx_bios_rtc_from_bcd(CPU_CH, 0, 23, &hour) ||
                !r36sx_bios_rtc_from_bcd(CPU_CL, 0, 59, &minute) ||
                !r36sx_bios_rtc_from_bcd(CPU_DH, 0, 59, &second)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            tm_value.tm_hour = hour;
            tm_value.tm_min = minute;
            tm_value.tm_sec = second;
            if (!r36sx_bios_rtc_set_time(&tm_value)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            r36sx_bios_rtc_success();
            return 1;

        case 0x04: /* Read Real-Time Clock date */
            if (!r36sx_bios_rtc_get_time(&tm_value)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            full_year = tm_value.tm_year + 1900;
            CPU_CH = r36sx_bios_rtc_to_bcd(full_year / 100);
            CPU_CL = r36sx_bios_rtc_to_bcd(full_year % 100);
            CPU_DH = r36sx_bios_rtc_to_bcd(tm_value.tm_mon + 1);
            CPU_DL = r36sx_bios_rtc_to_bcd(tm_value.tm_mday);
            r36sx_bios_rtc_success();
            return 1;

        case 0x05: /* Set Real-Time Clock date */
            if (!r36sx_bios_rtc_get_time(&tm_value) ||
                !r36sx_bios_rtc_from_bcd(CPU_CH, 19, 20, &century) ||
                !r36sx_bios_rtc_from_bcd(CPU_CL, 0, 99, &year) ||
                !r36sx_bios_rtc_from_bcd(CPU_DH, 1, 12, &month) ||
                !r36sx_bios_rtc_from_bcd(CPU_DL, 1, 31, &day)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            full_year = century * 100 + year;
            if (full_year < 1980 || full_year > 2037) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            tm_value.tm_year = full_year - 1900;
            tm_value.tm_mon = month - 1;
            tm_value.tm_mday = day;
            if (!r36sx_bios_rtc_set_time(&tm_value)) {
                r36sx_bios_rtc_failure();
                return 1;
            }
            r36sx_bios_rtc_success();
            return 1;

        default:
            return 0;
    }
}
#endif

#define R36SX_CR0_PE 0x00000001u
#define R36SX_CR0_MP 0x00000002u
#define R36SX_CR0_EM 0x00000004u
#define R36SX_CR0_TS 0x00000008u
#define R36SX_CR0_ET 0x00000010u
#define R36SX_CR0_PG 0x80000000u
#define R36SX_CR0_386_WRITE_MASK \
    (R36SX_CR0_PE | R36SX_CR0_MP | R36SX_CR0_EM | R36SX_CR0_TS | \
     R36SX_CR0_ET | R36SX_CR0_PG)
#define R36SX_CR0_386_RESERVED_READ_MASK 0x7ffffff0u
#define R36SX_CR3_PAGE_DIRECTORY_MASK 0xfffff000u
#define R36SX_DR6_RESET 0xffff0ff0u
#define R36SX_DR6_B0_MASK 0x00000001u
#define R36SX_DR6_BS_MASK 0x00004000u
#define R36SX_DR7_RESET 0x00000400u
#define R36SX_DR7_ENABLE_MASK(slot) (0x03u << ((slot) * 2u))
#define R36SX_DR7_RW_FIELD(slot) (((slot) * 4u) + 16u)
#define R36SX_DR7_LEN_FIELD(slot) (((slot) * 4u) + 18u)
#define R36SX_DR7_RW_EXECUTE 0x00u
#define R36SX_DR7_RW_WRITE 0x01u
#define R36SX_DR7_RW_READWRITE 0x03u
#define R36SX_386_REGISTER_COUNT 8u
#define R36SX_386_TEST_REGISTER_FIRST 6u
#define R36SX_386_TEST_REGISTER_LAST 7u
#define R36SX_PAGE_PRESENT 0x00000001u
#define R36SX_PAGE_WRITABLE 0x00000002u
#define R36SX_PAGE_USER 0x00000004u
#define R36SX_PAGE_ACCESSED 0x00000020u
#define R36SX_PAGE_DIRTY 0x00000040u
#define R36SX_PAGE_FRAME_MASK 0xfffff000u
#define R36SX_DESCRIPTOR_PRESENT 0x80u
#define R36SX_DESCRIPTOR_CODE_DATA 0x10u
#define R36SX_DESCRIPTOR_EXECUTABLE 0x08u
#define R36SX_DESCRIPTOR_CONFORMING 0x04u
#define R36SX_DESCRIPTOR_EXPAND_DOWN 0x04u
#define R36SX_DESCRIPTOR_WRITABLE 0x02u
#define R36SX_DESCRIPTOR_READABLE 0x02u
#define R36SX_DESCRIPTOR_FLAG_DB 0x04u
#define R36SX_DESCRIPTOR_FLAG_GRANULAR 0x08u
#define R36SX_DESCRIPTOR_DPL_SHIFT 5u
#define R36SX_SELECTOR_TABLE_INDICATOR 0x0004u
#define R36SX_SELECTOR_INDEX_MASK 0xfff8u
/* ModR/M mod field value 11b: operand is a register, not memory. */
#define R36SX_MODRM_MOD_REGISTER 0x03u
#define R36SX_DESCRIPTOR_SYSTEM_TYPE_MASK 0x0fu
#define R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE 0x01u
#define R36SX_DESCRIPTOR_TYPE_LDT 0x02u
#define R36SX_DESCRIPTOR_TYPE_TSS16_BUSY 0x03u
#define R36SX_DESCRIPTOR_TYPE_CALL_GATE16 0x04u
#define R36SX_DESCRIPTOR_TYPE_TASK_GATE 0x05u
#define R36SX_DESCRIPTOR_TYPE_CALL_GATE32 0x0cu
#define R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE 0x09u
#define R36SX_DESCRIPTOR_TYPE_TSS32_BUSY 0x0bu
#define R36SX_TSS16_BACKLINK 0x00u
#define R36SX_TSS16_IP 0x0eu
#define R36SX_TSS16_FLAGS 0x10u
#define R36SX_TSS16_AX 0x12u
#define R36SX_TSS16_CX 0x14u
#define R36SX_TSS16_DX 0x16u
#define R36SX_TSS16_BX 0x18u
#define R36SX_TSS16_SP 0x1au
#define R36SX_TSS16_BP 0x1cu
#define R36SX_TSS16_SI 0x1eu
#define R36SX_TSS16_DI 0x20u
#define R36SX_TSS16_ES 0x22u
#define R36SX_TSS16_CS 0x24u
#define R36SX_TSS16_SS 0x26u
#define R36SX_TSS16_DS 0x28u
#define R36SX_TSS16_LDTR 0x2au
#define R36SX_TSS16_MIN_LIMIT 0x2bu
#define R36SX_TSS32_BACKLINK 0x00u
#define R36SX_TSS32_CR3 0x1cu
#define R36SX_TSS32_EIP 0x20u
#define R36SX_TSS32_EFLAGS 0x24u
#define R36SX_TSS32_EAX 0x28u
#define R36SX_TSS32_ECX 0x2cu
#define R36SX_TSS32_EDX 0x30u
#define R36SX_TSS32_EBX 0x34u
#define R36SX_TSS32_ESP 0x38u
#define R36SX_TSS32_EBP 0x3cu
#define R36SX_TSS32_ESI 0x40u
#define R36SX_TSS32_EDI 0x44u
#define R36SX_TSS32_ES 0x48u
#define R36SX_TSS32_CS 0x4cu
#define R36SX_TSS32_SS 0x50u
#define R36SX_TSS32_DS 0x54u
#define R36SX_TSS32_FS 0x58u
#define R36SX_TSS32_GS 0x5cu
#define R36SX_TSS32_LDTR 0x60u
#define R36SX_TSS32_IOMAP_BASE 0x66u
#define R36SX_TSS32_MIN_LIMIT 0x67u
#define R36SX_IO_PORT_COUNT 0x10000u
#define R36SX_IO_PERMISSION_BITS_PER_BYTE 8u
#define R36SX_EXCEPTION_DEBUG 1u
#define R36SX_EXCEPTION_INVALID_OPCODE 6u
#define R36SX_EXCEPTION_BOUND 5u
#define R36SX_EXCEPTION_DEVICE_NOT_AVAILABLE 7u
#define R36SX_EXCEPTION_INVALID_TSS 10u
#define R36SX_EXCEPTION_NOT_PRESENT 11u
#define R36SX_EXCEPTION_STACK 12u
#define R36SX_EXCEPTION_GP 13u
#define R36SX_EXCEPTION_PF 14u
#define R36SX_EXCEPTION_X87_ERROR 16u
#define R36SX_FLAGS_ALWAYS_ONE 0x0002u
#define R36SX_FLAGS_STATUS_MASK 0x0fd5u
#define R36SX_FLAGS_IF_MASK 0x0200u
#define R36SX_FLAGS_IOPL_MASK 0x3000u
#define R36SX_FLAGS_IOPL_SHIFT 12u
#define R36SX_FLAGS_NT_MASK 0x4000u
#define R36SX_FLAGS_286_CONTROL_MASK 0x7000u
#define R36SX_FLAGS_8086_FIXED_MASK 0xf000u
/* 80386-only EFLAGS bits kept by POPFD/IRETD and 32-bit task switches. */
#define R36SX_EFLAGS_RF_MASK 0x00010000u
#define R36SX_EFLAGS_VM_MASK 0x00020000u
#define R36SX_EFLAGS_386_MASK 0x00037fd5u

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
static uint32_t r36sx_dr[R36SX_386_REGISTER_COUNT];
static uint32_t r36sx_tr[R36SX_386_REGISTER_COUNT];
static uint16_t r36sx_debug_resume_cs;
static uint32_t r36sx_debug_resume_ip;
static uint32_t r36sx_debug_pending_dr6_hits;
static uint8_t r36sx_debug_resume_valid;
static uint8_t r36sx_debug_suppress_watchpoints;
static uint32_t r36sx_gdtr_base;
static uint32_t r36sx_idtr_base;
static uint16_t r36sx_gdtr_limit;
static uint16_t r36sx_idtr_limit = 0x03ffu;
static uint16_t r36sx_ldtr_selector;
static uint16_t r36sx_tr_selector;
static r36sx_segment_cache_t r36sx_ldtr_cache;
static r36sx_segment_cache_t r36sx_tr_cache;

typedef struct {
    uint32_t dwordregs[8];
    uint32_t ip32;
    uint32_t flags;
    uint32_t cr0;
    uint32_t cr2;
    uint32_t cr3;
    uint32_t debug_regs[R36SX_386_REGISTER_COUNT];
    uint32_t test_regs[R36SX_386_REGISTER_COUNT];
    uint32_t gdtr_base;
    uint32_t idtr_base;
    uint16_t gdtr_limit;
    uint16_t idtr_limit;
    uint16_t ldtr_selector;
    uint16_t tr_selector;
    r36sx_segment_cache_t seg_cache[6];
    r36sx_segment_cache_t ldtr_cache;
    r36sx_segment_cache_t tr_cache;
    uint32_t segment_values[6];
    uint16_t segselectors[6];
    uint32_t segbases[6];
    uint16_t useseg;
    uint16_t oldsp;
    uint32_t useseg_base;
    uint8_t segoverride;
    uint8_t reptype;
    uint8_t tempcf;
    uint8_t oldcf;
    uint8_t mode;
    uint8_t reg;
    uint8_t rm;
    uint8_t sib;
    uint8_t hltstate;
    uint8_t interpreter_protected;
    uint8_t current_cpl;
    uint8_t strict_8086_mode;
    uint8_t nestlev;
    uint16_t saveip;
    uint16_t savecs;
    uint16_t oper1;
    uint16_t oper2;
    uint16_t res16;
    uint16_t temp16;
    uint16_t dummy;
    uint16_t stacksize;
    uint32_t disp32;
    uint32_t ea;
    bool operand_size_override;
    bool address_size_override;
} r36sx_cpu_snapshot_t;

static inline uint8_t r36sx_cpu_v86_enabled(void)
{
#if R36SX_ENABLE_PROTECTED_MODE
    return r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) &&
           (r36sx_cr0 & R36SX_CR0_PE) != 0 &&
           (x86_flags.value & R36SX_EFLAGS_VM_MASK) != 0;
#else
    return 0;
#endif
}

static inline uint8_t r36sx_cpu_native_protected_enabled(void)
{
    /*
     * Descriptor-management instructions are protected-mode instructions, but
     * Intel still rejects them in virtual-8086 mode with #UD.
     */
#if R36SX_ENABLE_PROTECTED_MODE
    return (r36sx_cr0 & R36SX_CR0_PE) != 0 && !r36sx_cpu_v86_enabled();
#else
    return 0;
#endif
}

static uint8_t r36sx_cpu_protected_interrupt(uint8_t intnum,
                                             uint32_t error_code,
                                             uint8_t has_error_code,
                                             uint8_t software_int,
                                             uint32_t fault_ip);
static void r36sx_cpu_raise_exception(uint8_t intnum,
                                      uint32_t error_code,
                                      uint8_t has_error_code,
                                      uint32_t fault_ip);
static uint8_t r36sx_cpu_set_tss_busy(uint16_t selector, uint8_t busy);
void intcall86(uint8_t intnum);
static void r36sx_cpu_software_interrupt(uint8_t intnum, uint32_t fault_ip);
static void __not_in_flash() r36sx_cpu_exec86_real(uint32_t execloops);

/* 80286 protected-mode state, descriptors, and selector loading. */
#include "r36sx_cpu_80286.inl"

static inline uint8_t r36sx_cpu_iopl(void)
{
    return (uint8_t)((x86_flags.value & R36SX_FLAGS_IOPL_MASK) >>
                     R36SX_FLAGS_IOPL_SHIFT);
}

static uint8_t r36sx_cpu_require_cpl0(uint32_t fault_ip)
{
    if (r36sx_cpu_v86_enabled() ||
        (r36sx_cpu_protected_enabled() && r36sx_cpu_cpl() != 0u)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 0;
    }
    return 1;
}

static uint8_t r36sx_cpu_require_iopl(uint32_t fault_ip)
{
    if (r36sx_cpu_protected_enabled() &&
        r36sx_cpu_cpl() > r36sx_cpu_iopl()) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 0;
    }
    return 1;
}

static uint8_t r36sx_cpu_v86_iopl_sensitive_fault(uint32_t fault_ip)
{
    /*
     * Intel 80386 virtual-8086 mode makes LOCK, PUSHF, POPF, INT n, and
     * IRET sensitive to IOPL.  When IOPL is below 3, the protected monitor
     * gets #GP(0) and can emulate or reflect the operation.
     */
    if (r36sx_cpu_v86_enabled() && r36sx_cpu_iopl() < 3u) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 1;
    }
    return 0;
}

static inline uint8_t r36sx_cpu_x87_wait_available(uint32_t fault_ip)
{
    /*
     * WAIT/FWAIT synchronizes the CPU with the numeric processor extension.
     * EM traps both WAIT and ESC for software emulation; MP makes WAIT honor
     * TS for lazy x87 context switching.  A physically absent 80287 with EM
     * clear behaves like inactive BUSY/ERROR pins, so it is handled by the
     * caller as a no-op rather than as #NM.
     */
    if ((r36sx_cr0 & R36SX_CR0_EM) ||
        ((r36sx_cr0 & R36SX_CR0_MP) && (r36sx_cr0 & R36SX_CR0_TS))) {
        r36sx_cpu_raise_exception(
            R36SX_EXCEPTION_DEVICE_NOT_AVAILABLE, 0, 0, fault_ip);
        return 0;
    }
    return 1;
}

static inline uint8_t r36sx_cpu_x87_escape_available(uint32_t fault_ip)
{
    /*
     * ESC opcodes are x87 instructions.  EM requests an INT 7/#NM trap for an
     * emulator, and TS traps any x87 instruction until CLTS or a task switch
     * handler marks the coprocessor state usable again.
     */
    if (r36sx_cr0 & (R36SX_CR0_EM | R36SX_CR0_TS)) {
        r36sx_cpu_raise_exception(
            R36SX_EXCEPTION_DEVICE_NOT_AVAILABLE, 0, 0, fault_ip);
        return 0;
    }
    return 1;
}

static inline uint8_t r36sx_cpu_read_linear8(uint32_t linear);
static inline uint16_t r36sx_cpu_read_linear16(uint32_t linear);
static inline uint32_t r36sx_cpu_read_linear32(uint32_t linear);
static inline void r36sx_cpu_write_linear8(uint32_t linear, uint8_t value);
static inline void r36sx_cpu_write_linear16(uint32_t linear, uint16_t value);
static inline void r36sx_cpu_write_linear32(uint32_t linear, uint32_t value);

/*
 * getmem/putmem model guest segment-register accesses and must enforce
 * protected-mode descriptor limits and permissions.  read/write helpers stay
 * linear for descriptor tables, paging structures, and already-checked EAs.
 */
static inline uint8_t r36sx_cpu_pick_segment_cache(
    uint8_t segid,
    uint16_t selector,
    uint8_t *out_segid,
    const r36sx_segment_cache_t **out_cache)
{
    if (segid > reggs || segselector16[segid] != selector) {
        return 0;
    }

    *out_segid = segid;
    *out_cache = &r36sx_seg_cache[segid];
    return 1;
}

static uint8_t r36sx_cpu_find_segment_cache(
    uint16_t selector,
    uint8_t *segid,
    const r36sx_segment_cache_t **cache)
{
    if (r36sx_cpu_pick_segment_cache(regcs, selector, segid, cache) ||
        r36sx_cpu_pick_segment_cache(regss, selector, segid, cache) ||
        r36sx_cpu_pick_segment_cache(reges, selector, segid, cache) ||
        r36sx_cpu_pick_segment_cache(regds, selector, segid, cache) ||
        r36sx_cpu_pick_segment_cache(regfs, selector, segid, cache) ||
        r36sx_cpu_pick_segment_cache(reggs, selector, segid, cache)) {
        return 1;
    }

    for (uint8_t i = reges; i <= reggs; i++) {
        if (r36sx_seg_cache[i].selector == selector) {
            *segid = i;
            *cache = &r36sx_seg_cache[i];
            return 1;
        }
    }

    return 0;
}

static uint8_t r36sx_cpu_check_segment_cache_access(
    uint8_t segid,
    const r36sx_segment_cache_t *cache,
    uint32_t offset,
    uint32_t bytes,
    uint8_t write_access,
    uint8_t execute_access)
{
    if (!r36sx_cpu_protected_enabled() || bytes == 0u) {
        return 1;
    }

    if (!cache || !cache->valid) {
        r36sx_cpu_raise_exception(
            segid == regss ? R36SX_EXCEPTION_STACK : R36SX_EXCEPTION_GP,
            segid == regss ? 0u : (cache ? cache->selector & 0xfffcu : 0u),
            1, CPU_IP);
        return 0;
    }

    if (execute_access) {
        if (!r36sx_descriptor_is_code(cache)) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP,
                                      cache->selector & 0xfffcu,
                                      1, CPU_IP);
            return 0;
        }
    } else if (write_access) {
        if (!r36sx_descriptor_is_writable_data(cache)) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP,
                                      cache->selector & 0xfffcu,
                                      1, CPU_IP);
            return 0;
        }
    } else if (r36sx_descriptor_is_code(cache)) {
        if (!r36sx_descriptor_is_readable_code(cache)) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP,
                                      cache->selector & 0xfffcu,
                                      1, CPU_IP);
            return 0;
        }
    } else if (!r36sx_descriptor_is_data(cache)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP,
                                  cache->selector & 0xfffcu,
                                  1, CPU_IP);
        return 0;
    }

    uint32_t last = offset + bytes - 1u;
    uint8_t overflow = last < offset;
    uint8_t limit_fault;

    if (r36sx_descriptor_is_expand_down_data(cache)) {
        uint32_t upper = (cache->flags & R36SX_DESCRIPTOR_FLAG_DB) ?
                         0xffffffffu : 0xffffu;
        limit_fault = overflow || offset <= cache->limit || last > upper;
    } else {
        limit_fault = overflow || last > cache->limit;
    }

    if (limit_fault) {
        r36sx_cpu_raise_exception(
            segid == regss ? R36SX_EXCEPTION_STACK : R36SX_EXCEPTION_GP,
            segid == regss ? 0u : (cache->selector & 0xfffcu), 1, CPU_IP);
        return 0;
    }

    return 1;
}

static uint8_t r36sx_cpu_segment_linear_checked(
    uint16_t selector,
    uint32_t offset,
    uint32_t bytes,
    uint8_t write_access,
    uint8_t execute_access,
    uint32_t *linear)
{
    if (!r36sx_cpu_protected_enabled() || r36sx_cpu_v86_enabled()) {
        *linear = ((uint32_t)selector << 4) + offset;
        return 1;
    }

    uint8_t segid = 0xffu;
    const r36sx_segment_cache_t *cache = NULL;
    if (!r36sx_cpu_find_segment_cache(selector, &segid, &cache)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, selector & 0xfffcu,
                                  1, CPU_IP);
        return 0;
    }

    if (!r36sx_cpu_check_segment_cache_access(
            segid, cache, offset, bytes, write_access, execute_access)) {
        return 0;
    }

    if (offset > UINT32_MAX - cache->base) {
        r36sx_cpu_raise_exception(
            segid == regss ? R36SX_EXCEPTION_STACK : R36SX_EXCEPTION_GP,
            segid == regss ? 0u : (cache->selector & 0xfffcu), 1, CPU_IP);
        return 0;
    }

    *linear = cache->base + offset;
    return 1;
}

static void r36sx_cpu_raise_debug_exception(uint32_t fault_ip,
                                            uint32_t dr6_bits)
{
    r36sx_dr[6] |= R36SX_DR6_RESET | dr6_bits;
    r36sx_debug_suppress_watchpoints++;
    r36sx_cpu_raise_exception(R36SX_EXCEPTION_DEBUG, 0, 0, fault_ip);
    r36sx_debug_suppress_watchpoints--;
}

static inline uint32_t r36sx_cpu_debug_watchpoint_length(uint8_t slot)
{
    uint32_t len = (r36sx_dr[7] >> R36SX_DR7_LEN_FIELD(slot)) & 0x03u;
    switch (len) {
        case 0x00u:
            return 1u;
        case 0x01u:
            return 2u;
        case 0x03u:
            return 4u;
        default:
            return 1u;
    }
}

static inline uint8_t r36sx_cpu_debug_ranges_overlap(uint32_t a_start,
                                                     uint32_t a_len,
                                                     uint32_t b_start,
                                                     uint32_t b_len)
{
    uint32_t a_end = UINT32_MAX - a_start < a_len - 1u
                         ? UINT32_MAX
                         : a_start + a_len - 1u;
    uint32_t b_end = UINT32_MAX - b_start < b_len - 1u
                         ? UINT32_MAX
                         : b_start + b_len - 1u;
    return a_start <= b_end && b_start <= a_end;
}

static uint8_t r36sx_cpu_debug_check_execute_breakpoint(uint32_t fault_ip)
{
    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        return 0;
    }

    if (r36sx_debug_resume_valid &&
        r36sx_debug_resume_cs == CPU_CS &&
        r36sx_debug_resume_ip == fault_ip) {
        r36sx_debug_resume_valid = 0;
        return 0;
    }

    if (x86_flags.value & R36SX_EFLAGS_RF_MASK) {
        x86_flags.value &= ~R36SX_EFLAGS_RF_MASK;
        return 0;
    }

    uint32_t dr7 = r36sx_dr[7];
    if ((dr7 & 0xffu) == 0) {
        return 0;
    }

    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(CPU_CS, fault_ip, 1u, 0, 1,
                                          &linear)) {
        return 1;
    }

    uint32_t hits = 0;
    for (uint8_t slot = 0; slot < 4u; slot++) {
        uint32_t rw = (dr7 >> R36SX_DR7_RW_FIELD(slot)) & 0x03u;
        if ((dr7 & R36SX_DR7_ENABLE_MASK(slot)) &&
            rw == R36SX_DR7_RW_EXECUTE &&
            r36sx_dr[slot] == linear) {
            hits |= R36SX_DR6_B0_MASK << slot;
        }
    }

    if (!hits) {
        return 0;
    }

    /*
     * 386 execution breakpoints are debug faults before the instruction
     * executes.  Data read watchpoints need a fetch-aware read path, so they
     * stay tracked separately in TODO_386_INSTRUCTIONS.md.
     */
    r36sx_debug_resume_cs = CPU_CS;
    r36sx_debug_resume_ip = fault_ip;
    r36sx_debug_resume_valid = 1;
    r36sx_cpu_raise_debug_exception(fault_ip, hits);
    return 1;
}

static inline void r36sx_cpu_debug_note_data_write(uint32_t linear,
                                                   uint32_t bytes)
{
    if (r36sx_debug_suppress_watchpoints ||
        bytes == 0u ||
        !r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        return;
    }

    uint32_t dr7 = r36sx_dr[7];
    if ((dr7 & 0xffu) == 0) {
        return;
    }

    for (uint8_t slot = 0; slot < 4u; slot++) {
        uint32_t rw = (dr7 >> R36SX_DR7_RW_FIELD(slot)) & 0x03u;
        if ((dr7 & R36SX_DR7_ENABLE_MASK(slot)) &&
            (rw == R36SX_DR7_RW_WRITE || rw == R36SX_DR7_RW_READWRITE) &&
            r36sx_cpu_debug_ranges_overlap(
                linear, bytes, r36sx_dr[slot],
                r36sx_cpu_debug_watchpoint_length(slot))) {
            r36sx_debug_pending_dr6_hits |= R36SX_DR6_B0_MASK << slot;
        }
    }
}

static inline uint8_t r36sx_cpu_selector_is_current_cs(uint16_t selector)
{
    return r36sx_cpu_protected_enabled() && selector == CPU_CS;
}

static inline uint8_t r36sx_cpu_getmem8_checked(uint16_t selector,
                                                uint32_t offset)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(
            selector, offset, 1u, 0, r36sx_cpu_selector_is_current_cs(selector),
            &linear)) {
        return 0xffu;
    }
    return r36sx_cpu_read_linear8(linear);
}

static inline uint16_t r36sx_cpu_getmem16_checked(uint16_t selector,
                                                  uint32_t offset)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(
            selector, offset, 2u, 0, r36sx_cpu_selector_is_current_cs(selector),
            &linear)) {
        return 0xffffu;
    }
    return r36sx_cpu_read_linear16(linear);
}

static inline uint32_t r36sx_cpu_getmem32_checked(uint16_t selector,
                                                  uint32_t offset)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(
            selector, offset, 4u, 0, r36sx_cpu_selector_is_current_cs(selector),
            &linear)) {
        return 0xffffffffu;
    }
    return r36sx_cpu_read_linear32(linear);
}

static inline void r36sx_cpu_putmem8_checked(uint16_t selector,
                                             uint32_t offset,
                                             uint8_t value)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(selector, offset, 1u, 1, 0,
                                          &linear)) {
        return;
    }
    r36sx_cpu_write_linear8(linear, value);
}

static inline void r36sx_cpu_putmem16_checked(uint16_t selector,
                                              uint32_t offset,
                                              uint16_t value)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(selector, offset, 2u, 1, 0,
                                          &linear)) {
        return;
    }
    r36sx_cpu_write_linear16(linear, value);
}

static inline void r36sx_cpu_putmem32_checked(uint16_t selector,
                                              uint32_t offset,
                                              uint32_t value)
{
    uint32_t linear;
    if (!r36sx_cpu_segment_linear_checked(selector, offset, 4u, 1, 0,
                                          &linear)) {
        return;
    }
    r36sx_cpu_write_linear32(linear, value);
}

/* 80386 interpreter memory models: fast real mode and checked protected mode. */
#include "r36sx_cpu_80386_real.inl"
#include "r36sx_cpu_80386_protected.inl"

static inline uint8_t r36sx_cpu_interpreter_getmem8(uint16_t selector,
                                                    uint32_t offset)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_getmem8(selector, offset);
    }
    return r36sx_cpu_protected_getmem8(selector, offset);
}

static inline uint16_t r36sx_cpu_interpreter_getmem16(uint16_t selector,
                                                      uint32_t offset)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_getmem16(selector, offset);
    }
    return r36sx_cpu_protected_getmem16(selector, offset);
}

static inline uint32_t r36sx_cpu_interpreter_getmem32(uint16_t selector,
                                                      uint32_t offset)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_getmem32(selector, offset);
    }
    return r36sx_cpu_protected_getmem32(selector, offset);
}

static inline void r36sx_cpu_interpreter_putmem8(uint16_t selector,
                                                 uint32_t offset,
                                                 uint8_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_putmem8(selector, offset, value);
        return;
    }
    r36sx_cpu_protected_putmem8(selector, offset, value);
}

static inline void r36sx_cpu_interpreter_putmem16(uint16_t selector,
                                                  uint32_t offset,
                                                  uint16_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_putmem16(selector, offset, value);
        return;
    }
    r36sx_cpu_protected_putmem16(selector, offset, value);
}

static inline void r36sx_cpu_interpreter_putmem32(uint16_t selector,
                                                  uint32_t offset,
                                                  uint32_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_putmem32(selector, offset, value);
        return;
    }
    r36sx_cpu_protected_putmem32(selector, offset, value);
}

static inline uint8_t r36sx_cpu_interpreter_read8(uint32_t linear)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_read8(linear);
    }
    return r36sx_cpu_protected_read8(linear);
}

static inline uint16_t r36sx_cpu_interpreter_read16(uint32_t linear)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_read16(linear);
    }
    return r36sx_cpu_protected_read16(linear);
}

static inline uint32_t r36sx_cpu_interpreter_read32(uint32_t linear)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        return r36sx_cpu_real_read32(linear);
    }
    return r36sx_cpu_protected_read32(linear);
}

static inline void r36sx_cpu_interpreter_write8(uint32_t linear,
                                                uint8_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_write8(linear, value);
        return;
    }
    r36sx_cpu_protected_write8(linear, value);
}

static inline void r36sx_cpu_interpreter_write16(uint32_t linear,
                                                 uint16_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_write16(linear, value);
        return;
    }
    r36sx_cpu_protected_write16(linear, value);
}

static inline void r36sx_cpu_interpreter_write32(uint32_t linear,
                                                 uint32_t value)
{
    if (likely(!r36sx_cpu_interpreter_protected)) {
        r36sx_cpu_real_write32(linear, value);
        return;
    }
    r36sx_cpu_protected_write32(linear, value);
}

#undef getmem8
#undef getmem16
#undef getmem32
#undef putmem8
#undef putmem16
#undef putmem32
#define getmem8(x, y) \
    r36sx_cpu_interpreter_getmem8((uint16_t)(x), (uint32_t)(y))
#define getmem16(x, y) \
    r36sx_cpu_interpreter_getmem16((uint16_t)(x), (uint32_t)(y))
#define getmem32(x, y) \
    r36sx_cpu_interpreter_getmem32((uint16_t)(x), (uint32_t)(y))
#define putmem8(x, y, z) \
    r36sx_cpu_interpreter_putmem8((uint16_t)(x), (uint32_t)(y), (uint8_t)(z))
#define putmem16(x, y, z) \
    r36sx_cpu_interpreter_putmem16((uint16_t)(x), (uint32_t)(y), (uint16_t)(z))
#define putmem32(x, y, z) \
    r36sx_cpu_interpreter_putmem32((uint16_t)(x), (uint32_t)(y), (uint32_t)(z))

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

static inline uint32_t r36sx_loop_get_count(void)
{
    return addressSizeOverride ? CPU_ECX : CPU_CX;
}

static inline void r36sx_loop_set_count(uint32_t count)
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

#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
static inline int r36sx_rep_offset_span_no_wrap(uint32_t offset,
                                                uint32_t bytes,
                                                int index32)
{
    if (bytes == 0u) {
        return 1;
    }
    if (index32) {
        return offset <= UINT32_MAX - (bytes - 1u);
    }
    return bytes <= 0x10000u && offset <= 0x10000u - bytes;
}

static inline int r36sx_rep_ram_span(uint16_t segment,
                                     uint32_t offset,
                                     uint32_t bytes,
                                     uint32_t *linear,
                                     int index32,
                                     uint8_t write_access)
{
    uint32_t address;

    if (!r36sx_rep_offset_span_no_wrap(offset, bytes, index32)) {
        return 0;
    }
    if (!r36sx_cpu_segment_linear_checked(segment, offset, bytes,
                                          write_access, 0, &address)) {
        return 0;
    }
    if (!r36sx_memory_fast_ram_range(address, bytes)) {
        return 0;
    }
    *linear = address;
    return 1;
}

static inline int r36sx_rep_ranges_overlap(uint32_t a,
                                           uint32_t b,
                                           uint32_t bytes)
{
    return a < b + bytes && b < a + bytes;
}

static inline void r36sx_rep_movs_ram_forward(uint32_t src,
                                              uint32_t dst,
                                              uint32_t count,
                                              uint32_t unit_bytes)
{
    uint32_t bytes = count * unit_bytes;
    uint8_t *src_ptr = RAM + src;
    uint8_t *dst_ptr = RAM + dst;

    if (!r36sx_rep_ranges_overlap(src, dst, bytes)) {
        memcpy(dst_ptr, src_ptr, bytes);
        return;
    }

    /* x86 forward MOVS does not have memmove's reverse-copy overlap semantics. */
    for (uint32_t i = 0; i < count; i++) {
        uint8_t value0 = src_ptr[0];
        dst_ptr[0] = value0;
        if (unit_bytes >= 2u) {
            uint8_t value1 = src_ptr[1];
            dst_ptr[1] = value1;
        }
        if (unit_bytes == 4u) {
            uint8_t value2 = src_ptr[2];
            uint8_t value3 = src_ptr[3];
            dst_ptr[2] = value2;
            dst_ptr[3] = value3;
        }
        src_ptr += unit_bytes;
        dst_ptr += unit_bytes;
    }
}

static inline int r36sx_rep_try_movs_ram(uint32_t count,
                                         uint32_t unit_bytes,
                                         uint32_t si,
                                         uint32_t di,
                                         int index32)
{
    uint32_t src;
    uint32_t dst;
    uint32_t bytes = count * unit_bytes;

    if (df || count == 0u) {
        return 0;
    }
    if (!r36sx_rep_ram_span(useseg, si, bytes, &src, index32, 0) ||
        !r36sx_rep_ram_span(CPU_ES, di, bytes, &dst, index32, 1)) {
        return 0;
    }

    r36sx_rep_movs_ram_forward(src, dst, count, unit_bytes);
    if (index32) {
        r36sx_set_src_index(si + bytes);
        r36sx_set_dst_index(di + bytes);
    } else {
        CPU_SI = (uint16_t)(si + bytes);
        CPU_DI = (uint16_t)(di + bytes);
    }
    return 1;
}

static inline void r36sx_rep_stosw_ram(uint32_t dst,
                                       uint32_t count,
                                       uint16_t value)
{
    if ((((uintptr_t)(void *)(RAM + dst)) & 1u) == 0u) {
        uint16_t *ptr = (uint16_t *)(RAM + dst);
        while (count--) {
            *ptr++ = value;
        }
        return;
    }

    uint8_t *ptr = RAM + dst;
    while (count--) {
        ptr[0] = (uint8_t)value;
        ptr[1] = (uint8_t)(value >> 8);
        ptr += 2;
    }
}

static inline void r36sx_rep_stosd_ram(uint32_t dst,
                                       uint32_t count,
                                       uint32_t value)
{
    if ((((uintptr_t)(void *)(RAM + dst)) & 3u) == 0u) {
        uint32_t *ptr = (uint32_t *)(RAM + dst);
        while (count--) {
            *ptr++ = value;
        }
        return;
    }

    uint8_t *ptr = RAM + dst;
    while (count--) {
        ptr[0] = (uint8_t)value;
        ptr[1] = (uint8_t)(value >> 8);
        ptr[2] = (uint8_t)(value >> 16);
        ptr[3] = (uint8_t)(value >> 24);
        ptr += 4;
    }
}

static inline int r36sx_rep_try_stos_ram(uint32_t count,
                                         uint32_t unit_bytes,
                                         uint32_t di,
                                         int index32)
{
    uint32_t dst;
    uint32_t bytes = count * unit_bytes;

    if (df || count == 0u) {
        return 0;
    }
    if (!r36sx_rep_ram_span(CPU_ES, di, bytes, &dst, index32, 1)) {
        return 0;
    }

    if (unit_bytes == 1u) {
        memset(RAM + dst, CPU_AL, bytes);
    } else if (unit_bytes == 2u) {
        r36sx_rep_stosw_ram(dst, count, CPU_AX);
    } else {
        r36sx_rep_stosd_ram(dst, count, CPU_EAX);
    }
    if (index32) {
        r36sx_set_dst_index(di + bytes);
    } else {
        CPU_DI = (uint16_t)(di + bytes);
    }
    return 1;
}
#else
static inline int r36sx_rep_try_movs_ram(uint32_t count,
                                         uint32_t unit_bytes,
                                         uint32_t si,
                                         uint32_t di,
                                         int index32)
{
    (void)count;
    (void)unit_bytes;
    (void)si;
    (void)di;
    (void)index32;
    return 0;
}

static inline int r36sx_rep_try_stos_ram(uint32_t count,
                                         uint32_t unit_bytes,
                                         uint32_t di,
                                         int index32)
{
    (void)count;
    (void)unit_bytes;
    (void)di;
    (void)index32;
    return 0;
}
#endif

static inline void r36sx_rep_movsb(uint32_t count)
{
    uint32_t si = r36sx_src_index();
    uint32_t di = r36sx_dst_index();

    if (r36sx_rep_try_movs_ram(count, 1u, si, di, addressSizeOverride)) {
        return;
    }

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

    r36sx_set_src_index(si);
    r36sx_set_dst_index(di);
}

static inline void r36sx_rep_movsw(uint32_t count)
{
    uint32_t si = r36sx_src_index();
    uint32_t di = r36sx_dst_index();

    if (r36sx_rep_try_movs_ram(count, 2u, si, di, addressSizeOverride)) {
        return;
    }

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

    r36sx_set_src_index(si);
    r36sx_set_dst_index(di);
}

static inline void r36sx_rep_movsd(uint32_t count)
{
    uint32_t si = r36sx_src_index();
    uint32_t di = r36sx_dst_index();

    if (r36sx_rep_try_movs_ram(count, 4u, si, di, addressSizeOverride)) {
        return;
    }

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
    uint32_t di = r36sx_dst_index();
    uint8_t value = CPU_AL;

    if (r36sx_rep_try_stos_ram(count, 1u, di, addressSizeOverride)) {
        return;
    }

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

    r36sx_set_dst_index(di);
}

static inline void r36sx_rep_stosd(uint32_t count)
{
    uint32_t di = r36sx_dst_index();
    uint32_t value = CPU_EAX;

    if (r36sx_rep_try_stos_ram(count, 4u, di, addressSizeOverride)) {
        return;
    }

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
    uint32_t di = r36sx_dst_index();
    uint16_t value = CPU_AX;

    if (r36sx_rep_try_stos_ram(count, 2u, di, addressSizeOverride)) {
        return;
    }

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

    r36sx_set_dst_index(di);
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
        if (mode != R36SX_MODRM_MOD_REGISTER && rm == 4) {
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
                r36sx_cpu_use_segment(regss);
            }
            break;
        case 1:
            disp16 = signext(getmem8(CPU_CS, CPU_IP));
            StepIP(1);
            if (((rm == 2) || (rm == 3) || (rm == 6)) && !segoverride) {
                r36sx_cpu_use_segment(regss);
            }
            break;
        case 2:
            disp16 = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (((rm == 2) || (rm == 3) || (rm == 6)) && !segoverride) {
                r36sx_cpu_use_segment(regss);
            }
            break;
        default:
            disp16 = 0;
    }
}

static inline void r36sx_cpu_skip_x87_escape(void)
{
    /*
     * ESC D8..DF still has a ModR/M byte and optional displacement even when
     * no math coprocessor is present.  Decode those bytes so DOS x87 probes
     * continue at the correct IP, but do not touch memory or FPU state.
     */
    modregrm();
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
        r36sx_cpu_use_segment(regss);
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

static inline uint16_t r36sx_cpu_phys_read16(uint32_t address)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    return r36sx_readw86_fast(address);
#else
    return readw86_ob(address);
#endif
}

static inline uint32_t r36sx_cpu_phys_read32(uint32_t address)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    return r36sx_readdw86_fast(address);
#else
    return readdw86_ob(address);
#endif
}

static inline void r36sx_cpu_phys_write16(uint32_t address, uint16_t value)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    r36sx_writew86_fast(address, value);
#else
    writew86_ob(address, value);
#endif
}

static inline void r36sx_cpu_phys_write32(uint32_t address, uint32_t value)
{
#if R36SX_NATIVE_FAST_MEMORY && !PICO_ON_DEVICE
    r36sx_writedw86_fast(address, value);
#else
    write86_ob(address, (uint8_t)value);
    write86_ob(address + 1u, (uint8_t)(value >> 8));
    write86_ob(address + 2u, (uint8_t)(value >> 16));
    write86_ob(address + 3u, (uint8_t)(value >> 24));
#endif
}

static uint8_t r36sx_cpu_translate_linear(uint32_t linear,
                                          uint8_t write_access,
                                          uint32_t *physical)
{
    if ((r36sx_cr0 & R36SX_CR0_PG) == 0) {
        *physical = linear;
        return 1;
    }

    uint32_t pde_addr = (r36sx_cr3 & R36SX_CR3_PAGE_DIRECTORY_MASK) |
                        (((linear >> 22) & 0x3ffu) << 2);
    uint32_t pde = r36sx_cpu_phys_read32(pde_addr);
    uint8_t user_access = r36sx_cpu_cpl() == 3u;
    uint32_t error_code = (write_access ? 0x02u : 0u) |
                          (user_access ? 0x04u : 0u);

    if ((pde & R36SX_PAGE_PRESENT) == 0) {
        r36sx_cr2 = linear;
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_PF, error_code, 1, CPU_IP);
        return 0;
    }

    uint32_t pte_addr = (pde & R36SX_PAGE_FRAME_MASK) |
                        (((linear >> 12) & 0x3ffu) << 2);
    uint32_t pte = r36sx_cpu_phys_read32(pte_addr);
    if ((pte & R36SX_PAGE_PRESENT) == 0) {
        r36sx_cr2 = linear;
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_PF, error_code, 1, CPU_IP);
        return 0;
    }

    if (user_access) {
        if (((pde & R36SX_PAGE_USER) == 0) ||
            ((pte & R36SX_PAGE_USER) == 0) ||
            (write_access &&
             (((pde & R36SX_PAGE_WRITABLE) == 0) ||
              ((pte & R36SX_PAGE_WRITABLE) == 0)))) {
            r36sx_cr2 = linear;
            r36sx_cpu_raise_exception(
                R36SX_EXCEPTION_PF, error_code | 0x01u, 1, CPU_IP);
            return 0;
        }
    }

    if ((pde & R36SX_PAGE_ACCESSED) == 0) {
        r36sx_cpu_phys_write32(pde_addr, pde | R36SX_PAGE_ACCESSED);
        pde |= R36SX_PAGE_ACCESSED;
    }
    uint32_t pte_update = pte | R36SX_PAGE_ACCESSED |
                          (write_access ? R36SX_PAGE_DIRTY : 0u);
    if (pte_update != pte) {
        r36sx_cpu_phys_write32(pte_addr, pte_update);
        pte = pte_update;
    }

    *physical = (pte & R36SX_PAGE_FRAME_MASK) | (linear & 0x0fffu);
    return 1;
}

static inline uint8_t r36sx_cpu_read_linear8(uint32_t linear)
{
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 0, &physical)) {
        return 0xffu;
    }
    return read86_ob(physical);
}

static inline uint16_t r36sx_cpu_read_linear16(uint32_t linear)
{
    if ((linear & 0x0fffu) == 0x0fffu) {
        return (uint16_t)r36sx_cpu_read_linear8(linear) |
               ((uint16_t)r36sx_cpu_read_linear8(linear + 1u) << 8);
    }
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 0, &physical)) {
        return 0xffffu;
    }
    return r36sx_cpu_phys_read16(physical);
}

static inline uint32_t r36sx_cpu_read_linear32(uint32_t linear)
{
    if ((linear & 0x0fffu) > 0x0ffcu) {
        return (uint32_t)r36sx_cpu_read_linear8(linear) |
               ((uint32_t)r36sx_cpu_read_linear8(linear + 1u) << 8) |
               ((uint32_t)r36sx_cpu_read_linear8(linear + 2u) << 16) |
               ((uint32_t)r36sx_cpu_read_linear8(linear + 3u) << 24);
    }
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 0, &physical)) {
        return 0xffffffffu;
    }
    return r36sx_cpu_phys_read32(physical);
}

static inline void r36sx_cpu_write_linear8(uint32_t linear, uint8_t value)
{
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 1, &physical)) {
        return;
    }
    r36sx_cpu_debug_note_data_write(linear, 1u);
    write86_ob(physical, value);
}

static inline void r36sx_cpu_write_linear16(uint32_t linear, uint16_t value)
{
    if ((linear & 0x0fffu) == 0x0fffu) {
        r36sx_cpu_write_linear8(linear, (uint8_t)value);
        r36sx_cpu_write_linear8(linear + 1u, (uint8_t)(value >> 8));
        return;
    }
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 1, &physical)) {
        return;
    }
    r36sx_cpu_debug_note_data_write(linear, 2u);
    r36sx_cpu_phys_write16(physical, value);
}

static inline void r36sx_cpu_write_linear32(uint32_t linear, uint32_t value)
{
    if ((linear & 0x0fffu) > 0x0ffcu) {
        r36sx_cpu_write_linear8(linear, (uint8_t)value);
        r36sx_cpu_write_linear8(linear + 1u, (uint8_t)(value >> 8));
        r36sx_cpu_write_linear8(linear + 2u, (uint8_t)(value >> 16));
        r36sx_cpu_write_linear8(linear + 3u, (uint8_t)(value >> 24));
        return;
    }
    uint32_t physical;
    if (!r36sx_cpu_translate_linear(linear, 1, &physical)) {
        return;
    }
    r36sx_cpu_debug_note_data_write(linear, 4u);
    r36sx_cpu_phys_write32(physical, value);
}

#undef read86
#undef readw86
#undef readdw86
#undef write86
#undef writew86
#undef writedw86
#undef getmem8
#undef getmem16
#undef getmem32
#undef putmem8
#undef putmem16
#undef putmem32
#define read86(address) r36sx_cpu_read_linear8((uint32_t)(address))
#define readw86(address) r36sx_cpu_read_linear16((uint32_t)(address))
#define readdw86(address) r36sx_cpu_read_linear32((uint32_t)(address))
#define write86(address, value) \
    r36sx_cpu_write_linear8((uint32_t)(address), (uint8_t)(value))
#define writew86(address, value) \
    r36sx_cpu_write_linear16((uint32_t)(address), (uint16_t)(value))
#define writedw86(address, value) \
    r36sx_cpu_write_linear32((uint32_t)(address), (uint32_t)(value))
#define getmem8(x, y) \
    r36sx_cpu_interpreter_getmem8((uint16_t)(x), (uint32_t)(y))
#define getmem16(x, y) \
    r36sx_cpu_interpreter_getmem16((uint16_t)(x), (uint32_t)(y))
#define getmem32(x, y) \
    r36sx_cpu_interpreter_getmem32((uint16_t)(x), (uint32_t)(y))
#define putmem8(x, y, z) \
    r36sx_cpu_interpreter_putmem8((uint16_t)(x), (uint32_t)(y), (uint8_t)(z))
#define putmem16(x, y, z) \
    r36sx_cpu_interpreter_putmem16((uint16_t)(x), (uint32_t)(y), (uint16_t)(z))
#define putmem32(x, y, z) \
    r36sx_cpu_interpreter_putmem32((uint16_t)(x), (uint32_t)(y), (uint32_t)(z))

static uint8_t r36sx_cpu_check_segment_access(uint32_t offset,
                                              uint32_t bytes,
                                              uint8_t write_access)
{
    uint32_t linear;
    if (likely(!r36sx_cpu_interpreter_protected) || bytes == 0u) {
        return 1;
    }
    return r36sx_cpu_segment_linear_checked(useseg, offset, bytes,
                                            write_access, 0, &linear);
}

static inline void r36sx_cpu_delay_maskable_interrupts_one_instruction(void)
{
    /*
     * Intel documents a one-instruction interrupt shadow after STI, and after
     * loading SS with MOV/POP.  Without this, a pending PIT IRQ can fire before
     * the instruction immediately following STI; test386's "STI; PUSHF" then
     * vectors into the protected-mode default handler instead of testing IF.
     */
    /*
     * Use a two-phase counter because the common opcode epilogue runs after
     * the instruction that created the shadow.  2 -> 1 after STI/MOV SS/POP SS,
     * then 1 -> 0 after the following instruction has executed.
     */
    r36sx_cpu_maskable_interrupt_shadow = 2u;
}

static INLINE void push(uint16_t pushval) {
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP -= 2u;
        putmem16(CPU_SS, CPU_ESP, pushval);
    } else {
        CPU_SP = (uint16_t)(CPU_SP - 2u);
        putmem16(CPU_SS, CPU_SP, pushval);
    }
}

static INLINE uint16_t pop() {
    uint32_t sp = r36sx_cpu_stack_default32() ? CPU_ESP : CPU_SP;
    uint16_t tempval = getmem16(CPU_SS, sp);
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP += 2u;
    } else {
        CPU_SP = (uint16_t)(CPU_SP + 2u);
    }
    return tempval;
}

static INLINE void push32(uint32_t pushval) {
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP -= 4u;
        putmem32(CPU_SS, CPU_ESP, pushval);
    } else {
        CPU_SP = (uint16_t)(CPU_SP - 4u);
        putmem32(CPU_SS, CPU_SP, pushval);
    }
}

static INLINE uint32_t pop32(void) {
    uint32_t sp = r36sx_cpu_stack_default32() ? CPU_ESP : CPU_SP;
    uint32_t tempval = getmem32(CPU_SS, sp);
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP += 4u;
    } else {
        CPU_SP = (uint16_t)(CPU_SP + 4u);
    }
    return tempval;
}

static INLINE void r36sx_cpu_push_segment_selector(uint16_t selector)
{
    /*
     * PUSH Sreg is controlled by the current operand size.  On 80386 a
     * 32-bit operand-size segment push consumes a 32-bit stack slot even
     * though the selector itself is only 16 bits.
     */
    if (operandSizeOverride) {
        push32(selector);
    } else {
        push(selector);
    }
}

static INLINE uint16_t r36sx_cpu_pop_segment_selector(void)
{
    /*
     * POP Sreg with 32-bit operand size discards the high half of the stack
     * value after advancing SP/ESP by four bytes.
     */
    return operandSizeOverride ? (uint16_t)pop32() : pop();
}

static INLINE void r36sx_cpu_adjust_stack(uint32_t bytes)
{
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP += bytes;
    } else {
        CPU_SP = (uint16_t)(CPU_SP + bytes);
    }
}

static INLINE uint32_t r36sx_cpu_adjust_stack_value_for_cache(
    uint32_t sp,
    uint32_t bytes,
    const r36sx_segment_cache_t *stack_cache)
{
    /*
     * Used before committing an outer-privilege SS.  The stack width belongs
     * to the stack segment we are about to restore, not to the current SS.
     */
    if (r36sx_cpu_descriptor_uses_386_format() && stack_cache->valid &&
        (stack_cache->flags & R36SX_DESCRIPTOR_FLAG_DB)) {
        return sp + bytes;
    }
    return (uint16_t)(sp + bytes);
}

static INLINE void r36sx_cpu_set_stack_pointer(uint32_t value)
{
    if (r36sx_cpu_stack_default32()) {
        CPU_ESP = value;
    } else {
        CPU_SP = (uint16_t)value;
    }
}

#if R36SX_DEBUG_TEST386_CPU_TRACE
static uint16_t r36sx_test386_trace_port;
static uint8_t r36sx_test386_trace_code;
static uint32_t r36sx_test386_trace_remaining;

void r36sx_cpu_debug_test386_subpost(uint16_t portnum, uint8_t value)
{
    /*
     * Re-arm on every R36SX sub-POST.  test386 POST 09 contains several large
     * instruction groups; tracing a bounded window after each breadcrumb lets
     * us localize the failing group from the Windows host without growing the
     * ROM with many more OUT instructions.
     */
    if (r36sx_test386_trace_remaining != 0u) {
        r36sx_pico286_debug_log(
            "[T386] trace rearmed by post=%03x:%02x previous_left=%lu",
            (unsigned int)portnum, (unsigned int)value,
            (unsigned long)r36sx_test386_trace_remaining);
    }
    r36sx_test386_trace_port = portnum;
    r36sx_test386_trace_code = value;
    r36sx_test386_trace_remaining = R36SX_DEBUG_TEST386_CPU_TRACE_LIMIT;
    r36sx_pico286_debug_log(
        "[T386] trace armed post=%03x:%02x limit=%lu",
        (unsigned int)portnum, (unsigned int)value,
        (unsigned long)r36sx_test386_trace_remaining);
}

static void r36sx_cpu_debug_trace_test386_instruction(uint32_t firstip,
                                                      uint8_t opcode)
{
    if (r36sx_test386_trace_remaining == 0u) {
        return;
    }

    uint8_t bytes[8];
    for (uint8_t i = 0; i < sizeof(bytes); ++i) {
        bytes[i] = getmem8(CPU_CS, r36sx_cpu_mask_ip(firstip + i));
    }

    uint32_t index = R36SX_DEBUG_TEST386_CPU_TRACE_LIMIT -
                     r36sx_test386_trace_remaining;
    r36sx_pico286_debug_log(
        "[T386] #%03lu post=%03x:%02x cs:eip=%04X:%08lX op=%02X "
        "bytes=%02X %02X %02X %02X %02X %02X %02X %02X os=%u as=%u "
        "ss:sp=%04X:%04X esp=%08lX ds=%04X es=%04X fs=%04X gs=%04X "
        "eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX "
        "ebp=%08lX flags=%08lX cr0=%08lX",
        (unsigned long)index,
        (unsigned int)r36sx_test386_trace_port,
        (unsigned int)r36sx_test386_trace_code,
        CPU_CS, (unsigned long)firstip, opcode,
        bytes[0], bytes[1], bytes[2], bytes[3],
        bytes[4], bytes[5], bytes[6], bytes[7],
        operandSizeOverride ? 32u : 16u,
        addressSizeOverride ? 32u : 16u,
        CPU_SS, CPU_SP, (unsigned long)CPU_ESP,
        CPU_DS, CPU_ES, CPU_FS, CPU_GS,
        (unsigned long)CPU_EAX, (unsigned long)CPU_EBX,
        (unsigned long)CPU_ECX, (unsigned long)CPU_EDX,
        (unsigned long)CPU_ESI, (unsigned long)CPU_EDI,
        (unsigned long)CPU_EBP,
        (unsigned long)x86_flags.value,
        (unsigned long)r36sx_cr0);

    r36sx_test386_trace_remaining--;
    if (r36sx_test386_trace_remaining == 0u) {
        r36sx_pico286_debug_log("[T386] trace exhausted");
    }
}
#else
void r36sx_cpu_debug_test386_subpost(uint16_t portnum, uint8_t value)
{
    (void)portnum;
    (void)value;
}

static inline void r36sx_cpu_debug_trace_test386_instruction(uint32_t firstip,
                                                             uint8_t opcode)
{
    (void)firstip;
    (void)opcode;
}
#endif

typedef struct {
    uint16_t selector;
    uint32_t offset;
    uint8_t param_count;
    uint8_t type;
    uint8_t dpl;
    uint8_t present;
    uint8_t is_32;
} r36sx_far_gate_t;

static INLINE uint16_t r36sx_cpu_flags_word_variable_mask(void);
static INLINE uint16_t makeflagsword(void);
static INLINE uint32_t makeflagsdword(void);
static INLINE void decodeflagsword(uint16_t x);
static INLINE void decodeflagsdword(uint32_t x);
static uint8_t r36sx_cpu_validate_return_code(uint16_t selector,
                                              uint32_t offset,
                                              uint8_t new_cpl,
                                              r36sx_segment_cache_t *cache,
                                              uint32_t fault_ip);

static inline uint32_t r36sx_cpu_stack_pointer_value(void)
{
    return r36sx_cpu_stack_default32() ? CPU_ESP : CPU_SP;
}

static INLINE void r36sx_cpu_enter16(uint16_t frame_size,
                                     uint8_t nesting_level)
{
    uint16_t frame_ptr;

    nesting_level &= 0x1Fu;
    push(CPU_BP);
    frame_ptr = CPU_SP;

    /* ENTER copies saved frame-pointer values from the caller's frame chain.
       It must not push BP addresses; nested block-structured languages depend
       on the copied display words matching Intel ENTER imm16, imm8 semantics. */
    if (nesting_level != 0) {
        uint16_t source_bp = CPU_BP;

        for (uint8_t level = 1; level < nesting_level; level++) {
            source_bp = (uint16_t)(source_bp - 2u);
            push(getmem16(CPU_SS, source_bp));
        }
        push(frame_ptr);
    }

    CPU_BP = frame_ptr;
    CPU_SP = (uint16_t)(CPU_SP - frame_size);
}

static inline void r36sx_cpu_raise_selector_fault(uint8_t exception,
                                                  uint16_t selector)
{
    r36sx_cpu_raise_exception(exception, selector & 0xfffcu, 1, CPU_IP);
}

static inline void r36sx_cpu_raise_selector_fault_at(uint8_t exception,
                                                     uint16_t selector,
                                                     uint32_t fault_ip)
{
    r36sx_cpu_raise_exception(exception, selector & 0xfffcu, 1, fault_ip);
}

static uint8_t r36sx_cpu_descriptor_linear_address(uint16_t selector,
                                                   uint32_t *address)
{
    if ((selector & 0xfffcu) == 0) {
        return 0;
    }

    uint32_t table_base = r36sx_gdtr_base;
    uint32_t table_limit = r36sx_gdtr_limit;
    if (selector & R36SX_SELECTOR_TABLE_INDICATOR) {
        if (!r36sx_ldtr_cache.valid) {
            return 0;
        }
        table_base = r36sx_ldtr_cache.base;
        table_limit = r36sx_ldtr_cache.limit;
    }

    uint32_t descriptor_offset = selector & R36SX_SELECTOR_INDEX_MASK;
    if (descriptor_offset + 7u > table_limit) {
        return 0;
    }

    *address = table_base + descriptor_offset;
    return 1;
}

static uint8_t r36sx_cpu_set_tss_busy(uint16_t selector, uint8_t busy)
{
    uint32_t descriptor_address;
    if (!r36sx_cpu_descriptor_linear_address(selector, &descriptor_address)) {
        return 0;
    }

    uint32_t lo = readdw86(descriptor_address);
    uint32_t hi = readdw86(descriptor_address + 4u);
    r36sx_segment_cache_t cache;
    r36sx_cpu_fill_descriptor_cache(selector, lo, hi, &cache);
    uint8_t type = r36sx_descriptor_type(&cache);
    uint8_t new_type;

    if (type == R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE ||
        type == R36SX_DESCRIPTOR_TYPE_TSS16_BUSY) {
        new_type = busy ? R36SX_DESCRIPTOR_TYPE_TSS16_BUSY :
                          R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE;
    } else if (r36sx_cpu_descriptor_uses_386_format() &&
               (type == R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE ||
                type == R36SX_DESCRIPTOR_TYPE_TSS32_BUSY)) {
        new_type = busy ? R36SX_DESCRIPTOR_TYPE_TSS32_BUSY :
                          R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE;
    } else {
        return 0;
    }

    /*
     * The busy bit is encoded in the low system-type nibble of the descriptor
     * access byte.  Update the descriptor in-place so LAR/LSL and later task
     * switches observe the architectural B-bit state.
     */
    uint8_t new_access =
        (cache.access & (uint8_t)~R36SX_DESCRIPTOR_SYSTEM_TYPE_MASK) |
        new_type;
    write86(descriptor_address + 5u, new_access);

    if ((r36sx_tr_selector & 0xfffcu) == (selector & 0xfffcu)) {
        r36sx_tr_cache.access = new_access;
    }
    return 1;
}

static uint8_t r36sx_cpu_decode_descriptor_raw(uint16_t selector,
                                               uint32_t *lo,
                                               uint32_t *hi,
                                               r36sx_segment_cache_t *cache)
{
    if ((selector & 0xfffcu) == 0) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, CPU_IP);
        return 0;
    }

    uint32_t table_base = r36sx_gdtr_base;
    uint32_t table_limit = r36sx_gdtr_limit;
    if (selector & R36SX_SELECTOR_TABLE_INDICATOR) {
        if (!r36sx_ldtr_cache.valid) {
            r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
            return 0;
        }
        table_base = r36sx_ldtr_cache.base;
        table_limit = r36sx_ldtr_cache.limit;
    }

    uint32_t descriptor_offset = selector & R36SX_SELECTOR_INDEX_MASK;
    if (descriptor_offset + 7u > table_limit) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }

    uint32_t addr = table_base + descriptor_offset;
    *lo = readdw86(addr);
    *hi = readdw86(addr + 4u);
    r36sx_cpu_fill_descriptor_cache(selector, *lo, *hi, cache);
    return 1;
}

static uint8_t r36sx_cpu_decode_call_gate(uint16_t selector,
                                          r36sx_far_gate_t *gate)
{
    uint32_t lo;
    uint32_t hi;
    r36sx_segment_cache_t gate_cache;
    memset(&gate_cache, 0, sizeof(gate_cache));
    if (!r36sx_cpu_decode_descriptor_raw(selector, &lo, &hi, &gate_cache)) {
        return 0;
    }

    uint8_t type = r36sx_descriptor_type(&gate_cache);
    if (r36sx_descriptor_is_code_data(&gate_cache) ||
        !(type == R36SX_DESCRIPTOR_TYPE_CALL_GATE16 ||
          type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32)) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }
    if (type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32 &&
        !r36sx_cpu_descriptor_uses_386_format()) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }

    if (!gate_cache.valid) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_NOT_PRESENT, selector);
        return 0;
    }

    gate->selector = (uint16_t)(lo >> 16);
    gate->offset = (lo & 0xffffu) |
                   (type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32
                        ? (hi & 0xffff0000u) : 0u);
    gate->param_count = (uint8_t)(hi & 0x1fu);
    gate->type = type;
    gate->dpl = r36sx_descriptor_dpl(&gate_cache);
    gate->present = gate_cache.valid;
    gate->is_32 = type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32;
    return 1;
}

static uint8_t r36sx_cpu_code_offset_valid(
    uint16_t selector,
    const r36sx_segment_cache_t *cache,
    uint32_t offset,
    uint32_t fault_ip)
{
    if (offset > cache->limit) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    return 1;
}

static uint8_t r36sx_cpu_load_code_for_transfer(
    uint16_t selector,
    uint32_t offset,
    uint8_t gate_transfer,
    uint8_t jump_transfer,
    r36sx_segment_cache_t *target_cache,
    uint8_t *new_cpl,
    uint32_t fault_ip)
{
    if ((selector & 0xfffcu) == 0) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 0;
    }

    memset(target_cache, 0, sizeof(*target_cache));
    if (!r36sx_cpu_decode_descriptor(selector, target_cache)) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    if (!target_cache->valid) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_NOT_PRESENT,
                                          selector, fault_ip);
        return 0;
    }
    if (!r36sx_descriptor_is_code(target_cache)) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    if (!r36sx_cpu_code_offset_valid(selector, target_cache, offset,
                                     fault_ip)) {
        return 0;
    }

    uint8_t cpl = r36sx_cpu_cpl();
    uint8_t dpl = r36sx_descriptor_dpl(target_cache);
    uint8_t rpl = r36sx_selector_rpl(selector);

    if (r36sx_descriptor_is_conforming_code(target_cache)) {
        if (dpl > cpl) {
            r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                              fault_ip);
            return 0;
        }
        *new_cpl = cpl;
        return 1;
    }

    if (!gate_transfer) {
        if (dpl != cpl || rpl > cpl) {
            r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                              fault_ip);
            return 0;
        }
        *new_cpl = cpl;
        return 1;
    }

    if (jump_transfer) {
        if (dpl != cpl) {
            r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                              fault_ip);
            return 0;
        }
        *new_cpl = cpl;
        return 1;
    }

    if (dpl > cpl) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    *new_cpl = dpl;
    return 1;
}

static void r36sx_cpu_commit_code_transfer(uint16_t selector,
                                           const r36sx_segment_cache_t *cache,
                                           uint8_t cpl,
                                           uint32_t offset)
{
    uint16_t loaded_selector = (uint16_t)((selector & 0xfffcu) | (cpl & 3u));
    r36sx_segment_cache_t committed = *cache;
    committed.selector = loaded_selector;
    r36sx_cpu_commit_segment_cache(regcs, loaded_selector, &committed);
    r36sx_cpu_current_cpl = cpl & 3u;
    r36sx_cpu_set_ip(offset);
}

static uint8_t r36sx_cpu_decode_stack_segment_for_level(
    uint16_t selector,
    uint8_t cpl,
    r36sx_segment_cache_t *stack_cache,
    uint8_t invalid_exception)
{
    if ((selector & 0xfffcu) == 0) {
        r36sx_cpu_raise_exception(invalid_exception, 0, 1, CPU_IP);
        return 0;
    }

    memset(stack_cache, 0, sizeof(*stack_cache));
    if (!r36sx_cpu_decode_descriptor(selector, stack_cache)) {
        r36sx_cpu_raise_selector_fault(invalid_exception, selector);
        return 0;
    }
    if (!stack_cache->valid) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_STACK, selector);
        return 0;
    }
    if (!r36sx_descriptor_is_writable_data(stack_cache) ||
        r36sx_descriptor_dpl(stack_cache) != cpl ||
        r36sx_selector_rpl(selector) != cpl) {
        r36sx_cpu_raise_selector_fault(invalid_exception, selector);
        return 0;
    }
    return 1;
}

static void r36sx_cpu_commit_stack_segment(uint16_t selector,
                                           const r36sx_segment_cache_t *cache)
{
    r36sx_segment_cache_t committed = *cache;
    committed.selector = selector;
    r36sx_cpu_commit_segment_cache(regss, selector, &committed);
}

static uint8_t r36sx_cpu_tss_stack_for_level(uint8_t cpl,
                                             uint32_t *new_sp,
                                             uint16_t *new_ss)
{
    if (cpl > 2u || (r36sx_tr_selector & 0xfffcu) == 0 ||
        !r36sx_tr_cache.valid) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  r36sx_tr_selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }

    uint8_t type = r36sx_descriptor_type(&r36sx_tr_cache);
    uint32_t offset;
    uint32_t needed_last;

    if (type == R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE ||
        type == R36SX_DESCRIPTOR_TYPE_TSS32_BUSY) {
        if (!r36sx_cpu_descriptor_uses_386_format()) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                      r36sx_tr_selector & 0xfffcu, 1, CPU_IP);
            return 0;
        }
        offset = 4u + (uint32_t)cpl * 8u;
        needed_last = offset + 5u;
        if (needed_last > r36sx_tr_cache.limit) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                      r36sx_tr_selector & 0xfffcu, 1, CPU_IP);
            return 0;
        }
        *new_sp = readdw86(r36sx_tr_cache.base + offset);
        *new_ss = readw86(r36sx_tr_cache.base + offset + 4u);
        return 1;
    }

    if (type == R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE ||
        type == R36SX_DESCRIPTOR_TYPE_TSS16_BUSY) {
        offset = 2u + (uint32_t)cpl * 4u;
        needed_last = offset + 3u;
        if (needed_last > r36sx_tr_cache.limit) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                      r36sx_tr_selector & 0xfffcu, 1, CPU_IP);
            return 0;
        }
        *new_sp = readw86(r36sx_tr_cache.base + offset);
        *new_ss = readw86(r36sx_tr_cache.base + offset + 2u);
        return 1;
    }

    r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                              r36sx_tr_selector & 0xfffcu, 1, CPU_IP);
    return 0;
}

typedef enum {
    R36SX_TASK_SWITCH_JMP,
    R36SX_TASK_SWITCH_CALL,
    R36SX_TASK_SWITCH_INTERRUPT,
    R36SX_TASK_SWITCH_IRET
} r36sx_task_switch_reason_t;

static inline uint8_t r36sx_descriptor_is_task_gate(
    const r36sx_segment_cache_t *cache)
{
    return !r36sx_descriptor_is_code_data(cache) &&
           r36sx_descriptor_type(cache) == R36SX_DESCRIPTOR_TYPE_TASK_GATE;
}

static inline uint8_t r36sx_tss_type_is_16(uint8_t type)
{
    return type == R36SX_DESCRIPTOR_TYPE_TSS16_AVAILABLE ||
           type == R36SX_DESCRIPTOR_TYPE_TSS16_BUSY;
}

static inline uint8_t r36sx_tss_type_is_32(uint8_t type)
{
    return r36sx_cpu_descriptor_uses_386_format() &&
           (type == R36SX_DESCRIPTOR_TYPE_TSS32_AVAILABLE ||
            type == R36SX_DESCRIPTOR_TYPE_TSS32_BUSY);
}

static uint8_t r36sx_cpu_require_io_permission(uint16_t port,
                                               uint8_t bytes,
                                               uint32_t fault_ip)
{
    if (!r36sx_cpu_protected_enabled() ||
        (!r36sx_cpu_v86_enabled() && r36sx_cpu_cpl() <= r36sx_cpu_iopl())) {
        return 1;
    }

    /*
     * On 80386, I/O instructions below IOPL (and all VM86 I/O) are checked
     * against the current 32-bit TSS I/O permission bitmap.  A set bit denies
     * the port; an absent or too-short bitmap behaves as denied.
     */
    uint8_t tr_type = r36sx_descriptor_type(&r36sx_tr_cache);
    if (!r36sx_tr_cache.valid || !r36sx_tss_type_is_32(tr_type) ||
        r36sx_tr_cache.limit < R36SX_TSS32_IOMAP_BASE + 1u) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 0;
    }

    uint32_t end_port = (uint32_t)port + bytes;
    if (bytes == 0u || end_port > R36SX_IO_PORT_COUNT) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
        return 0;
    }

    uint32_t bitmap_base = readw86(r36sx_tr_cache.base +
                                   R36SX_TSS32_IOMAP_BASE);
    for (uint32_t checked_port = port; checked_port < end_port;
         checked_port++) {
        uint32_t bitmap_byte_offset =
            bitmap_base +
            checked_port / R36SX_IO_PERMISSION_BITS_PER_BYTE;
        if (bitmap_byte_offset > r36sx_tr_cache.limit) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
            return 0;
        }
        uint8_t bitmap =
            read86(r36sx_tr_cache.base + bitmap_byte_offset);
        uint8_t bit =
            (uint8_t)(checked_port % R36SX_IO_PERMISSION_BITS_PER_BYTE);
        if (bitmap & (uint8_t)(1u << bit)) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, fault_ip);
            return 0;
        }
    }

    return 1;
}

static uint8_t r36sx_cpu_decode_tss_descriptor(
    uint16_t selector,
    uint8_t allow_busy,
    r36sx_segment_cache_t *cache,
    uint8_t *is_32)
{
    if ((selector & 0xfffcu) == 0 ||
        (selector & R36SX_SELECTOR_TABLE_INDICATOR)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }

    uint32_t lo;
    uint32_t hi;
    memset(cache, 0, sizeof(*cache));
    if (!r36sx_cpu_decode_descriptor_raw(selector, &lo, &hi, cache)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }
    (void)lo;
    (void)hi;

    uint8_t type = r36sx_descriptor_type(cache);
    if (r36sx_tss_type_is_32(type)) {
        if (cache->limit < R36SX_TSS32_MIN_LIMIT) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                      selector & 0xfffcu, 1, CPU_IP);
            return 0;
        }
        *is_32 = 1;
    } else if (r36sx_tss_type_is_16(type)) {
        if (cache->limit < R36SX_TSS16_MIN_LIMIT) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                      selector & 0xfffcu, 1, CPU_IP);
            return 0;
        }
        *is_32 = 0;
    } else {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }

    if (!cache->valid) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_NOT_PRESENT,
                                  selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }

    uint8_t busy = type == R36SX_DESCRIPTOR_TYPE_TSS16_BUSY ||
                   type == R36SX_DESCRIPTOR_TYPE_TSS32_BUSY;
    if (busy && !allow_busy) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP,
                                  selector & 0xfffcu, 1, CPU_IP);
        return 0;
    }
    return 1;
}

static void r36sx_cpu_save_tss_state(const r36sx_segment_cache_t *cache,
                                     uint8_t is_32)
{
    uint32_t base = cache->base;
    if (is_32) {
        writedw86(base + R36SX_TSS32_CR3,
                  r36sx_cr3 & R36SX_CR3_PAGE_DIRECTORY_MASK);
        writedw86(base + R36SX_TSS32_EIP, CPU_IP);
        writedw86(base + R36SX_TSS32_EFLAGS, makeflagsdword());
        writedw86(base + R36SX_TSS32_EAX, CPU_EAX);
        writedw86(base + R36SX_TSS32_ECX, CPU_ECX);
        writedw86(base + R36SX_TSS32_EDX, CPU_EDX);
        writedw86(base + R36SX_TSS32_EBX, CPU_EBX);
        writedw86(base + R36SX_TSS32_ESP, CPU_ESP);
        writedw86(base + R36SX_TSS32_EBP, CPU_EBP);
        writedw86(base + R36SX_TSS32_ESI, CPU_ESI);
        writedw86(base + R36SX_TSS32_EDI, CPU_EDI);
        writew86(base + R36SX_TSS32_ES, CPU_ES);
        writew86(base + R36SX_TSS32_CS, CPU_CS);
        writew86(base + R36SX_TSS32_SS, CPU_SS);
        writew86(base + R36SX_TSS32_DS, CPU_DS);
        writew86(base + R36SX_TSS32_FS, CPU_FS);
        writew86(base + R36SX_TSS32_GS, CPU_GS);
        writew86(base + R36SX_TSS32_LDTR, r36sx_ldtr_selector);
        return;
    }

    writew86(base + R36SX_TSS16_IP, (uint16_t)CPU_IP);
    writew86(base + R36SX_TSS16_FLAGS, makeflagsword());
    writew86(base + R36SX_TSS16_AX, CPU_AX);
    writew86(base + R36SX_TSS16_CX, CPU_CX);
    writew86(base + R36SX_TSS16_DX, CPU_DX);
    writew86(base + R36SX_TSS16_BX, CPU_BX);
    writew86(base + R36SX_TSS16_SP, CPU_SP);
    writew86(base + R36SX_TSS16_BP, CPU_BP);
    writew86(base + R36SX_TSS16_SI, CPU_SI);
    writew86(base + R36SX_TSS16_DI, CPU_DI);
    writew86(base + R36SX_TSS16_ES, CPU_ES);
    writew86(base + R36SX_TSS16_CS, CPU_CS);
    writew86(base + R36SX_TSS16_SS, CPU_SS);
    writew86(base + R36SX_TSS16_DS, CPU_DS);
    writew86(base + R36SX_TSS16_LDTR, r36sx_ldtr_selector);
}

static void r36sx_cpu_load_task_flags(uint32_t flags, uint8_t is_32)
{
    /*
     * A hardware task switch loads FLAGS/EFLAGS from the TSS image; unlike
     * POPF/IRET within the same task, it is not constrained by current CPL.
     */
    if (is_32 && r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        x86_flags.value = R36SX_FLAGS_ALWAYS_ONE |
                          (flags & R36SX_EFLAGS_386_MASK);
    } else {
        x86_flags.value = R36SX_FLAGS_ALWAYS_ONE |
                          (flags & r36sx_cpu_flags_word_variable_mask());
    }
}

static uint8_t r36sx_cpu_load_task_state(uint16_t selector,
                                         const r36sx_segment_cache_t *cache,
                                         uint8_t is_32)
{
    uint32_t base = cache->base;
    uint32_t new_ip;
    uint32_t new_flags;
    uint32_t new_sp;
    uint16_t new_cs;
    uint16_t new_ss;
    uint16_t new_es;
    uint16_t new_ds;
    uint16_t new_fs = 0;
    uint16_t new_gs = 0;
    uint16_t new_ldtr;

    if (is_32) {
        r36sx_cr3 = readdw86(base + R36SX_TSS32_CR3) &
                    R36SX_CR3_PAGE_DIRECTORY_MASK;
        new_ip = readdw86(base + R36SX_TSS32_EIP);
        new_flags = readdw86(base + R36SX_TSS32_EFLAGS);
        CPU_EAX = readdw86(base + R36SX_TSS32_EAX);
        CPU_ECX = readdw86(base + R36SX_TSS32_ECX);
        CPU_EDX = readdw86(base + R36SX_TSS32_EDX);
        CPU_EBX = readdw86(base + R36SX_TSS32_EBX);
        CPU_ESP = readdw86(base + R36SX_TSS32_ESP);
        CPU_EBP = readdw86(base + R36SX_TSS32_EBP);
        CPU_ESI = readdw86(base + R36SX_TSS32_ESI);
        CPU_EDI = readdw86(base + R36SX_TSS32_EDI);
        new_es = readw86(base + R36SX_TSS32_ES);
        new_cs = readw86(base + R36SX_TSS32_CS);
        new_ss = readw86(base + R36SX_TSS32_SS);
        new_ds = readw86(base + R36SX_TSS32_DS);
        new_fs = readw86(base + R36SX_TSS32_FS);
        new_gs = readw86(base + R36SX_TSS32_GS);
        new_ldtr = readw86(base + R36SX_TSS32_LDTR);
        new_sp = CPU_ESP;
    } else {
        new_ip = readw86(base + R36SX_TSS16_IP);
        new_flags = readw86(base + R36SX_TSS16_FLAGS);
        CPU_EAX = readw86(base + R36SX_TSS16_AX);
        CPU_ECX = readw86(base + R36SX_TSS16_CX);
        CPU_EDX = readw86(base + R36SX_TSS16_DX);
        CPU_EBX = readw86(base + R36SX_TSS16_BX);
        CPU_ESP = readw86(base + R36SX_TSS16_SP);
        CPU_EBP = readw86(base + R36SX_TSS16_BP);
        CPU_ESI = readw86(base + R36SX_TSS16_SI);
        CPU_EDI = readw86(base + R36SX_TSS16_DI);
        new_es = readw86(base + R36SX_TSS16_ES);
        new_cs = readw86(base + R36SX_TSS16_CS);
        new_ss = readw86(base + R36SX_TSS16_SS);
        new_ds = readw86(base + R36SX_TSS16_DS);
        new_ldtr = readw86(base + R36SX_TSS16_LDTR);
        new_sp = CPU_ESP;
    }

    if (!r36sx_cpu_load_ldtr(new_ldtr, CPU_IP)) {
        return 0;
    }

    if (is_32 &&
        r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) &&
        (new_flags & R36SX_EFLAGS_VM_MASK)) {
        /*
         * VM86 task state is loaded by a protected-mode task switch, but the
         * visible segment registers are real-mode segment values.  Descriptor
         * validation has already happened for the TSS and LDTR above; CS/SS
         * and data segments must therefore be cached as selector << 4 bases.
         */
        r36sx_tr_selector = selector & 0xfffcu;
        r36sx_tr_cache = *cache;
        r36sx_cpu_load_task_flags(new_flags, is_32);
        putsegreg(regcs, new_cs);
        r36sx_cpu_real_cache_segment(regcs);
        putsegreg(regss, new_ss);
        r36sx_cpu_real_cache_segment(regss);
        putsegreg(reges, new_es);
        r36sx_cpu_real_cache_segment(reges);
        putsegreg(regds, new_ds);
        r36sx_cpu_real_cache_segment(regds);
        putsegreg(regfs, new_fs);
        r36sx_cpu_real_cache_segment(regfs);
        putsegreg(reggs, new_gs);
        r36sx_cpu_real_cache_segment(reggs);
        r36sx_cpu_set_ip(new_ip);
        r36sx_cpu_set_stack_pointer(new_sp);
        r36sx_cr0 |= R36SX_CR0_TS;
        return 1;
    }

    uint8_t new_cpl = r36sx_selector_rpl(new_cs);
    r36sx_segment_cache_t cs_cache;
    r36sx_segment_cache_t ss_cache;
    if (!r36sx_cpu_validate_return_code(new_cs, new_ip, new_cpl,
                                        &cs_cache, CPU_IP) ||
        !r36sx_cpu_decode_stack_segment_for_level(
            new_ss, new_cpl, &ss_cache, R36SX_EXCEPTION_INVALID_TSS)) {
        return 0;
    }

    r36sx_tr_selector = selector & 0xfffcu;
    r36sx_tr_cache = *cache;
    r36sx_cpu_commit_code_transfer(new_cs, &cs_cache, new_cpl, new_ip);
    r36sx_cpu_commit_stack_segment(new_ss, &ss_cache);
    r36sx_cpu_set_stack_pointer(new_sp);
    r36sx_cpu_load_task_flags(new_flags, is_32);

    if (!r36sx_cpu_load_segment(reges, new_es) ||
        !r36sx_cpu_load_segment(regds, new_ds)) {
        return 0;
    }
    if (is_32) {
        if (!r36sx_cpu_load_segment(regfs, new_fs) ||
            !r36sx_cpu_load_segment(reggs, new_gs)) {
            return 0;
        }
    } else {
        r36sx_cpu_clear_segment_cache(regfs, 0);
        r36sx_cpu_clear_segment_cache(reggs, 0);
    }

    r36sx_cr0 |= R36SX_CR0_TS;
    return 1;
}

static uint8_t r36sx_cpu_task_switch(uint16_t selector,
                                     r36sx_task_switch_reason_t reason)
{
    uint16_t target_selector = selector & 0xfffcu;
    if (reason == R36SX_TASK_SWITCH_IRET) {
        if ((r36sx_tr_selector & 0xfffcu) == 0 || !r36sx_tr_cache.valid) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS, 0, 1,
                                      CPU_IP);
            return 0;
        }
        target_selector = readw86(r36sx_tr_cache.base +
                                  (r36sx_descriptor_type(&r36sx_tr_cache) ==
                                           R36SX_DESCRIPTOR_TYPE_TSS32_BUSY
                                       ? R36SX_TSS32_BACKLINK
                                       : R36SX_TSS16_BACKLINK)) &
                          0xfffcu;
    }

    r36sx_segment_cache_t target_cache;
    uint8_t target_is_32;
    uint8_t allow_busy = reason == R36SX_TASK_SWITCH_IRET;
    if (!r36sx_cpu_decode_tss_descriptor(target_selector, allow_busy,
                                         &target_cache, &target_is_32)) {
        return 0;
    }

    uint16_t old_selector = r36sx_tr_selector & 0xfffcu;
    uint8_t old_is_32 = r36sx_tss_type_is_32(
        r36sx_descriptor_type(&r36sx_tr_cache));
    if (reason != R36SX_TASK_SWITCH_IRET) {
        if (old_selector == 0 || !r36sx_tr_cache.valid) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS, 0, 1,
                                      CPU_IP);
            return 0;
        }
        r36sx_cpu_save_tss_state(&r36sx_tr_cache, old_is_32);
    }

    if (reason == R36SX_TASK_SWITCH_CALL ||
        reason == R36SX_TASK_SWITCH_INTERRUPT) {
        writew86(target_cache.base + R36SX_TSS32_BACKLINK, old_selector);
    } else if (old_selector != 0 &&
               (reason == R36SX_TASK_SWITCH_JMP ||
                reason == R36SX_TASK_SWITCH_IRET)) {
        r36sx_cpu_set_tss_busy(old_selector, 0);
    }

    if (!r36sx_cpu_set_tss_busy(target_selector, 1)) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_TSS,
                                  target_selector, 1, CPU_IP);
        return 0;
    }
    target_cache.access =
        (target_cache.access & (uint8_t)~R36SX_DESCRIPTOR_SYSTEM_TYPE_MASK) |
        (target_is_32 ? R36SX_DESCRIPTOR_TYPE_TSS32_BUSY :
                        R36SX_DESCRIPTOR_TYPE_TSS16_BUSY);

    if (!r36sx_cpu_load_task_state(target_selector, &target_cache,
                                   target_is_32)) {
        return 0;
    }

    if (reason == R36SX_TASK_SWITCH_CALL ||
        reason == R36SX_TASK_SWITCH_INTERRUPT) {
        x86_flags.value |= R36SX_FLAGS_NT_MASK;
    }
    return 1;
}

static uint8_t r36sx_cpu_decode_task_gate_target(uint16_t selector,
                                                 uint16_t *target_selector,
                                                 uint8_t *gate_dpl)
{
    uint32_t lo;
    uint32_t hi;
    r36sx_segment_cache_t gate_cache;
    memset(&gate_cache, 0, sizeof(gate_cache));
    if (!r36sx_cpu_decode_descriptor_raw(selector, &lo, &hi, &gate_cache)) {
        return 0;
    }
    if (!r36sx_descriptor_is_task_gate(&gate_cache)) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }
    if (!gate_cache.valid) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_NOT_PRESENT, selector);
        return 0;
    }

    *target_selector = (uint16_t)(lo >> 16);
    *gate_dpl = r36sx_descriptor_dpl(&gate_cache);
    (void)hi;
    return 1;
}

static inline void r36sx_cpu_push_frame_value(uint32_t value, uint8_t wide)
{
    if (wide) {
        push32(value);
    } else {
        push((uint16_t)value);
    }
}

static inline uint32_t r36sx_cpu_pop_frame_value(uint8_t wide)
{
    return wide ? pop32() : pop();
}

static void r36sx_cpu_invalidate_data_segments_for_cpl(uint8_t cpl)
{
    for (uint8_t segid = reges; segid <= reggs; segid++) {
        if (segid == regcs || segid == regss ||
            (segselector16[segid] & 0xfffcu) == 0 ||
            !r36sx_seg_cache[segid].valid) {
            continue;
        }
        if (r36sx_descriptor_is_code(&r36sx_seg_cache[segid]) &&
            !r36sx_descriptor_is_readable_code(&r36sx_seg_cache[segid])) {
            /*
             * Intel specifies that an interlevel RET/IRET checks DS/ES/FS/GS
             * against the return CPL.  Selectors that are no longer usable are
             * replaced with the null selector, not merely left with an invalid
             * hidden descriptor cache.
             */
            r36sx_cpu_clear_segment_cache(segid, 0);
            continue;
        }
        if (r36sx_descriptor_is_conforming_code(&r36sx_seg_cache[segid])) {
            continue;
        }
        if (r36sx_descriptor_dpl(&r36sx_seg_cache[segid]) < cpl) {
            r36sx_cpu_clear_segment_cache(segid, 0);
        }
    }
}

static uint8_t r36sx_cpu_protected_far_call(uint16_t selector,
                                            uint32_t offset,
                                            uint8_t wide,
                                            uint32_t fault_ip)
{
    uint32_t raw_lo;
    uint32_t raw_hi;
    r36sx_segment_cache_t descriptor_cache;
    memset(&descriptor_cache, 0, sizeof(descriptor_cache));
    if (!r36sx_cpu_decode_descriptor_raw(selector, &raw_lo, &raw_hi,
                                         &descriptor_cache)) {
        return 0;
    }

    uint8_t descriptor_type = r36sx_descriptor_type(&descriptor_cache);
    if (r36sx_descriptor_is_code(&descriptor_cache)) {
        r36sx_segment_cache_t target_cache;
        uint8_t new_cpl;
        if (!r36sx_cpu_load_code_for_transfer(selector, offset, 0, 0,
                                              &target_cache, &new_cpl,
                                              fault_ip)) {
            return 0;
        }
        r36sx_cpu_push_frame_value(CPU_CS, wide);
        r36sx_cpu_push_frame_value(CPU_IP, wide);
        r36sx_cpu_commit_code_transfer(selector, &target_cache, new_cpl,
                                       offset);
        return 1;
    }

    if (!r36sx_descriptor_is_code_data(&descriptor_cache) &&
        (r36sx_tss_type_is_16(descriptor_type) ||
         r36sx_tss_type_is_32(descriptor_type))) {
        if (r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector)) >
            r36sx_descriptor_dpl(&descriptor_cache)) {
            r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
            return 0;
        }
        return r36sx_cpu_task_switch(selector, R36SX_TASK_SWITCH_CALL);
    }

    if (r36sx_descriptor_is_task_gate(&descriptor_cache)) {
        uint16_t tss_selector;
        uint8_t gate_dpl;
        if (!r36sx_cpu_decode_task_gate_target(selector, &tss_selector,
                                              &gate_dpl)) {
            return 0;
        }
        if (r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector)) >
            gate_dpl) {
            r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
            return 0;
        }
        return r36sx_cpu_task_switch(tss_selector, R36SX_TASK_SWITCH_CALL);
    }

    if (r36sx_descriptor_is_code_data(&descriptor_cache) ||
        !(descriptor_type == R36SX_DESCRIPTOR_TYPE_CALL_GATE16 ||
          descriptor_type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32)) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }

    r36sx_segment_cache_t target_cache;
    uint8_t new_cpl;
    uint8_t old_cpl = r36sx_cpu_cpl();

    r36sx_far_gate_t gate;
    if (!r36sx_cpu_decode_call_gate(selector, &gate)) {
        return 0;
    }

    if (r36sx_priv_max(old_cpl, r36sx_selector_rpl(selector)) > gate.dpl) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }

    if (!r36sx_cpu_load_code_for_transfer(gate.selector, gate.offset, 1, 0,
                                          &target_cache, &new_cpl,
                                          fault_ip)) {
        return 0;
    }

    /*
     * Intel protected-mode call gates define their own operand size.  A 16-bit
     * gate pushes 16-bit return/old-stack fields and copies word parameters
     * even when the caller executes the far CALL from a 32-bit code segment.
     */
    uint8_t frame_wide = gate.is_32;
    uint16_t old_cs = CPU_CS;
    uint32_t old_ip = CPU_IP;
    uint16_t old_ss = CPU_SS;
    uint32_t old_sp = r36sx_cpu_stack_pointer_value();

    if (new_cpl < old_cpl) {
        uint32_t params[32];
        uint8_t param_count = gate.param_count;
        uint8_t param_wide = gate.is_32;
        uint32_t param_bytes = param_wide ? 4u : 2u;
        uint32_t copied_param_span = (uint32_t)param_count * param_bytes;
        uint32_t copied_param_linear;
        if (copied_param_span &&
            !r36sx_cpu_segment_linear_checked(old_ss, old_sp,
                                              copied_param_span, 0, 0,
                                              &copied_param_linear)) {
            return 0;
        }
        for (uint8_t i = 0; i < param_count; i++) {
            uint32_t param_offset = old_sp + (uint32_t)i * param_bytes;
            params[i] = param_wide ? getmem32(old_ss, param_offset)
                                   : getmem16(old_ss, param_offset);
        }

        uint32_t new_sp;
        uint16_t new_ss;
        r36sx_segment_cache_t new_ss_cache;
        if (!r36sx_cpu_tss_stack_for_level(new_cpl, &new_sp, &new_ss) ||
            !r36sx_cpu_decode_stack_segment_for_level(
                new_ss, new_cpl, &new_ss_cache,
                R36SX_EXCEPTION_INVALID_TSS)) {
            return 0;
        }

        r36sx_cpu_commit_stack_segment(new_ss, &new_ss_cache);
        r36sx_cpu_set_stack_pointer(new_sp);
        r36sx_cpu_push_frame_value(old_ss, frame_wide);
        r36sx_cpu_push_frame_value(old_sp, frame_wide);
        for (uint8_t i = param_count; i > 0; i--) {
            r36sx_cpu_push_frame_value(params[i - 1u], param_wide);
        }
    }

    r36sx_cpu_push_frame_value(old_cs, frame_wide);
    r36sx_cpu_push_frame_value(old_ip, frame_wide);
    r36sx_cpu_commit_code_transfer(gate.selector, &target_cache, new_cpl,
                                   gate.offset);
    return 1;
}

static uint8_t r36sx_cpu_protected_far_jump(uint16_t selector,
                                            uint32_t offset,
                                            uint32_t fault_ip)
{
    uint32_t raw_lo;
    uint32_t raw_hi;
    r36sx_segment_cache_t descriptor_cache;
    memset(&descriptor_cache, 0, sizeof(descriptor_cache));
    if (!r36sx_cpu_decode_descriptor_raw(selector, &raw_lo, &raw_hi,
                                         &descriptor_cache)) {
        return 0;
    }

    uint8_t descriptor_type = r36sx_descriptor_type(&descriptor_cache);
    r36sx_segment_cache_t target_cache;
    uint8_t new_cpl;
    if (r36sx_descriptor_is_code(&descriptor_cache)) {
        if (!r36sx_cpu_load_code_for_transfer(selector, offset, 0, 1,
                                              &target_cache, &new_cpl,
                                              fault_ip)) {
            return 0;
        }
        r36sx_cpu_commit_code_transfer(selector, &target_cache, new_cpl,
                                       offset);
        return 1;
    }

    if (!r36sx_descriptor_is_code_data(&descriptor_cache) &&
        (r36sx_tss_type_is_16(descriptor_type) ||
         r36sx_tss_type_is_32(descriptor_type))) {
        if (r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector)) >
            r36sx_descriptor_dpl(&descriptor_cache)) {
            r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
            return 0;
        }
        return r36sx_cpu_task_switch(selector, R36SX_TASK_SWITCH_JMP);
    }

    if (r36sx_descriptor_is_task_gate(&descriptor_cache)) {
        uint16_t tss_selector;
        uint8_t gate_dpl;
        if (!r36sx_cpu_decode_task_gate_target(selector, &tss_selector,
                                              &gate_dpl)) {
            return 0;
        }
        if (r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector)) >
            gate_dpl) {
            r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
            return 0;
        }
        return r36sx_cpu_task_switch(tss_selector, R36SX_TASK_SWITCH_JMP);
    }

    if (r36sx_descriptor_is_code_data(&descriptor_cache) ||
        !(descriptor_type == R36SX_DESCRIPTOR_TYPE_CALL_GATE16 ||
          descriptor_type == R36SX_DESCRIPTOR_TYPE_CALL_GATE32)) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }

    r36sx_far_gate_t gate;
    if (!r36sx_cpu_decode_call_gate(selector, &gate)) {
        return 0;
    }
    if (r36sx_priv_max(r36sx_cpu_cpl(), r36sx_selector_rpl(selector)) >
        gate.dpl) {
        r36sx_cpu_raise_selector_fault(R36SX_EXCEPTION_GP, selector);
        return 0;
    }
    if (!r36sx_cpu_load_code_for_transfer(gate.selector, gate.offset, 1, 1,
                                          &target_cache, &new_cpl,
                                          fault_ip)) {
        return 0;
    }
    r36sx_cpu_commit_code_transfer(gate.selector, &target_cache, new_cpl,
                                   gate.offset);
    return 1;
}

static uint8_t r36sx_cpu_validate_return_code(uint16_t selector,
                                              uint32_t offset,
                                              uint8_t new_cpl,
                                              r36sx_segment_cache_t *cache,
                                              uint32_t fault_ip)
{
    memset(cache, 0, sizeof(*cache));
    if ((selector & 0xfffcu) == 0 ||
        !r36sx_cpu_decode_descriptor(selector, cache)) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    if (!cache->valid) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_NOT_PRESENT,
                                          selector, fault_ip);
        return 0;
    }
    if (!r36sx_descriptor_is_code(cache) ||
        !r36sx_cpu_code_offset_valid(selector, cache, offset, fault_ip)) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }

    uint8_t dpl = r36sx_descriptor_dpl(cache);
    if (r36sx_descriptor_is_conforming_code(cache)) {
        if (dpl > new_cpl) {
            r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                              fault_ip);
            return 0;
        }
    } else if (dpl != new_cpl) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }
    return 1;
}

static uint8_t r36sx_cpu_protected_retf(uint16_t adjust, uint8_t wide,
                                        uint32_t fault_ip)
{
    uint8_t old_cpl = r36sx_cpu_cpl();
    uint32_t target_ip = r36sx_cpu_pop_frame_value(wide);
    uint16_t target_cs = (uint16_t)r36sx_cpu_pop_frame_value(wide);
    uint8_t new_cpl = r36sx_selector_rpl(target_cs);
    r36sx_segment_cache_t target_cache;

    /*
     * A far return can only keep the same CPL or return to a less privileged
     * outer level.  Returning from user mode directly to a kernel selector is
     * a fault on the RETF itself, not a silent no-op that continues after it.
     */
    if (new_cpl < old_cpl) {
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, target_cs,
                                          fault_ip);
        return 0;
    }

    if (!r36sx_cpu_validate_return_code(target_cs, target_ip, new_cpl,
                                        &target_cache, fault_ip)) {
        return 0;
    }

    r36sx_cpu_adjust_stack(adjust);

    if (new_cpl == old_cpl) {
        r36sx_cpu_commit_code_transfer(target_cs, &target_cache, new_cpl,
                                       target_ip);
        return 1;
    }

    uint32_t new_sp = r36sx_cpu_pop_frame_value(wide);
    uint16_t new_ss = (uint16_t)r36sx_cpu_pop_frame_value(wide);
    r36sx_segment_cache_t new_ss_cache;
    if (!r36sx_cpu_decode_stack_segment_for_level(
            new_ss, new_cpl, &new_ss_cache, R36SX_EXCEPTION_GP)) {
        return 0;
    }

    /*
     * For an inter-privilege RETF imm16, Intel specifies that the immediate
     * count releases parameters from both the inner stack and the restored
     * outer stack.  The earlier adjust skipped copied parameters before
     * popping old SP/SS; this one skips the caller's original parameters.
     */
    new_sp = r36sx_cpu_adjust_stack_value_for_cache(new_sp, adjust,
                                                   &new_ss_cache);
    r36sx_cpu_commit_code_transfer(target_cs, &target_cache, new_cpl,
                                   target_ip);
    r36sx_cpu_commit_stack_segment(new_ss, &new_ss_cache);
    r36sx_cpu_set_stack_pointer(new_sp);
    r36sx_cpu_invalidate_data_segments_for_cpl(new_cpl);
    return 1;
}

static uint8_t r36sx_cpu_protected_iret(uint8_t wide)
{
    if (x86_flags.value & R36SX_FLAGS_NT_MASK) {
        return r36sx_cpu_task_switch(0, R36SX_TASK_SWITCH_IRET);
    }

    uint8_t old_cpl = r36sx_cpu_cpl();
    uint32_t target_ip = r36sx_cpu_pop_frame_value(wide);
    uint16_t target_cs = (uint16_t)r36sx_cpu_pop_frame_value(wide);
    uint32_t target_flags = r36sx_cpu_pop_frame_value(wide);
    uint8_t new_cpl = r36sx_selector_rpl(target_cs);
    r36sx_segment_cache_t target_cache;

    if (target_flags & 0x00004000u) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, CPU_IP);
        return 0;
    }

    if (wide && (target_flags & R36SX_EFLAGS_VM_MASK)) {
        if (old_cpl != 0u ||
            !r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, 0, 1, CPU_IP);
            return 0;
        }

        /*
         * IRETD to VM86 consumes the extended frame produced by an interrupt
         * from VM86: EIP, CS, EFLAGS, ESP, SS, ES, DS, FS, GS.  The visible
         * segment selectors are real-mode values, so no descriptor lookup is
         * performed when returning to the virtual-8086 task.
         */
        uint32_t new_sp = pop32();
        uint16_t new_ss = (uint16_t)pop32();
        uint16_t new_es = (uint16_t)pop32();
        uint16_t new_ds = (uint16_t)pop32();
        uint16_t new_fs = (uint16_t)pop32();
        uint16_t new_gs = (uint16_t)pop32();

        decodeflagsdword(target_flags);
        putsegreg(regcs, target_cs);
        r36sx_cpu_real_cache_segment(regcs);
        r36sx_cpu_set_ip(target_ip);
        CPU_ESP = new_sp;
        putsegreg(regss, new_ss);
        r36sx_cpu_real_cache_segment(regss);
        putsegreg(reges, new_es);
        r36sx_cpu_real_cache_segment(reges);
        putsegreg(regds, new_ds);
        r36sx_cpu_real_cache_segment(regds);
        putsegreg(regfs, new_fs);
        r36sx_cpu_real_cache_segment(regfs);
        putsegreg(reggs, new_gs);
        r36sx_cpu_real_cache_segment(reggs);
        return 1;
    }

    if (new_cpl < old_cpl ||
        !r36sx_cpu_validate_return_code(target_cs, target_ip, new_cpl,
                                        &target_cache, CPU_IP)) {
        return 0;
    }

    if (new_cpl == old_cpl) {
        r36sx_cpu_commit_code_transfer(target_cs, &target_cache, new_cpl,
                                       target_ip);
        if (wide) {
            decodeflagsdword(target_flags);
        } else {
            decodeflagsword((uint16_t)target_flags);
        }
        return 1;
    }

    uint32_t new_sp = r36sx_cpu_pop_frame_value(wide);
    uint16_t new_ss = (uint16_t)r36sx_cpu_pop_frame_value(wide);
    r36sx_segment_cache_t new_ss_cache;
    if (!r36sx_cpu_decode_stack_segment_for_level(
            new_ss, new_cpl, &new_ss_cache, R36SX_EXCEPTION_GP)) {
        return 0;
    }

    r36sx_cpu_commit_code_transfer(target_cs, &target_cache, new_cpl,
                                   target_ip);
    r36sx_cpu_commit_stack_segment(new_ss, &new_ss_cache);
    r36sx_cpu_set_stack_pointer(new_sp);
    r36sx_cpu_invalidate_data_segments_for_cpl(new_cpl);
    if (wide) {
        decodeflagsdword(target_flags);
    } else {
        decodeflagsword((uint16_t)target_flags);
    }
    return 1;
}

static INLINE uint32_t readrm32(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 4u, 0)) {
            return 0xffffffffu;
        }
        return r36sx_cpu_interpreter_read32(ea);
    }
    return getreg32(rmval);
}

static INLINE uint16_t readrm16(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 2u, 0)) {
            return 0xffffu;
        }
        return r36sx_cpu_interpreter_read16(ea);
    }
    return getreg16(rmval);
}

static INLINE uint8_t readrm8(uint8_t rmval) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 1u, 0)) {
            return 0xffu;
        }
        return r36sx_cpu_interpreter_read8(ea);
    }
    return getreg8(rmval);
}

static INLINE void writerm16(uint8_t rmval, uint16_t value) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 2u, 1)) {
            return;
        }
        r36sx_cpu_interpreter_write16(ea, value);
    } else {
        putreg16(rmval, value);
    }
}

static INLINE void writerm32(uint8_t rmval, uint32_t value) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 4u, 1)) {
            return;
        }
        r36sx_cpu_interpreter_write32(ea, value);
    } else {
        putreg32(rmval, value);
    }
}

static INLINE void writerm8(uint8_t rmval, uint8_t value) {
    if (mode < 3) {
        getea(rmval);
        if (!r36sx_cpu_check_segment_access(ea - useseg_base, 1u, 1)) {
            return;
        }
        r36sx_cpu_interpreter_write8(ea, value);
    } else {
        putreg8(rmval, value);
    }
}

static INLINE uint16_t r36sx_cpu_flags_word_variable_mask(void) {
    uint16_t mask = R36SX_FLAGS_STATUS_MASK;

    /*
     * 8086/80186 expose bits 12..15 as fixed ones.  80286 real mode exposes
     * them as fixed zeros; protected mode and 80386 keep IOPL/NT in FLAGS.
     */
    if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
        return mask;
    }
    if (r36sx_cpu_protected_enabled() ||
        r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        mask |= R36SX_FLAGS_286_CONTROL_MASK;
    }
    return mask;
}

static INLINE uint16_t r36sx_cpu_flags_word_fixed_bits(void) {
    uint16_t fixed = R36SX_FLAGS_ALWAYS_ONE;
    if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
        fixed |= R36SX_FLAGS_8086_FIXED_MASK;
    }
    return fixed;
}

static INLINE uint16_t makeflagsword(void) {
    return r36sx_cpu_flags_word_fixed_bits() |
           (uint16_t)(x86_flags.value & r36sx_cpu_flags_word_variable_mask());
}

static INLINE void decodeflagsword(uint16_t x) {
    uint32_t preserved = 0;
    if (r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        /* 16-bit POPF/IRET updates FLAGS, not the high EFLAGS-only bits. */
        preserved = x86_flags.value & (R36SX_EFLAGS_386_MASK & 0xffff0000u);
    }
    uint32_t next = x & r36sx_cpu_flags_word_variable_mask();

    if (r36sx_cpu_protected_enabled()) {
        uint8_t cpl = r36sx_cpu_cpl();
        /*
         * In protected mode, POPF/IRET cannot raise IOPL from non-ring-0
         * code, and IF is writable only when CPL <= current IOPL.
         */
        if (cpl != 0u) {
            next = (next & ~R36SX_FLAGS_IOPL_MASK) |
                   (x86_flags.value & R36SX_FLAGS_IOPL_MASK);
        }
        if (cpl > r36sx_cpu_iopl()) {
            next = (next & ~R36SX_FLAGS_IF_MASK) |
                   (x86_flags.value & R36SX_FLAGS_IF_MASK);
        }
    }

    x86_flags.value = R36SX_FLAGS_ALWAYS_ONE | preserved |
                      next;
}

static INLINE uint32_t makeflagsdword(void) {
    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        return makeflagsword();
    }
    /* 80386 EFLAGS: expose 386-era bits, but not 486+ AC or CPUID ID. */
    return R36SX_FLAGS_ALWAYS_ONE | (x86_flags.value & R36SX_EFLAGS_386_MASK);
}

static INLINE void decodeflagsdword(uint32_t x) {
    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
        decodeflagsword((uint16_t)x);
        return;
    }
    uint32_t next = x & R36SX_EFLAGS_386_MASK;

    if (r36sx_cpu_protected_enabled()) {
        uint8_t cpl = r36sx_cpu_cpl();
        if (cpl != 0u) {
            next = (next & ~R36SX_FLAGS_IOPL_MASK) |
                   (x86_flags.value & R36SX_FLAGS_IOPL_MASK);
        }
        if (cpl > r36sx_cpu_iopl()) {
            next = (next & ~R36SX_FLAGS_IF_MASK) |
                   (x86_flags.value & R36SX_FLAGS_IF_MASK);
        }
    }

    x86_flags.value = R36SX_FLAGS_ALWAYS_ONE | next;
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
#define R36SX_VBE_WINDOW_BYTES (R36SX_VBE_WINDOW_KB * 1024u)

static uint8_t r36sx_vbe_dac_width = 6;

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
    return vga_svga_mode_pitch(r36sx_vbe_mode_base(mode));
}

static uint32_t r36sx_vbe_mode_bytes(uint16_t mode)
{
    mode = r36sx_vbe_mode_base(mode);
    return (uint32_t)r36sx_vbe_bytes_per_scanline(mode) *
           vga_svga_mode_height(mode);
}

static uint8_t r36sx_vbe_mode_banks(uint16_t mode)
{
    uint32_t mode_bytes = r36sx_vbe_mode_bytes(mode);
    uint32_t banks = (mode_bytes + R36SX_VBE_WINDOW_BYTES - 1u) /
                     R36SX_VBE_WINDOW_BYTES;

    if (banks == 0) {
        banks = 1;
    } else if (banks > 0xFFu) {
        banks = 0xFFu;
    }
    return (uint8_t)banks;
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
    r36sx_vbe_write32(base, 0x0A, 0x00000001u);
    r36sx_vbe_write32(base, 0x0E, r36sx_vbe_far_ptr(modes));
    r36sx_vbe_write16(base, 0x12,
                      (uint16_t)((SVGA_VRAM_SIZE + 0xFFFFu) >> 16));

    r36sx_vbe_write16(modes, 0x00, VBE_MODE_640X480X8);
    r36sx_vbe_write16(modes, 0x02, VBE_MODE_640X480X16);
    r36sx_vbe_write16(modes, 0x04, VBE_MODE_800X600X8);
    r36sx_vbe_write16(modes, 0x06, VBE_MODE_800X600X16);
    r36sx_vbe_write16(modes, 0x08, 0xFFFFu);
    CPU_AX = R36SX_VBE_STATUS_OK;
}

static void r36sx_vbe_write_mode_info(uint16_t mode)
{
    uint32_t base = (uint32_t)CPU_ES * 16u + CPU_DI;
    uint16_t bytes_per_scanline;
    uint16_t width;
    uint16_t height;
    uint32_t mode_bytes;
    uint8_t bpp;
    uint8_t pages;

    mode = r36sx_vbe_mode_base(mode);
    if (!vga_svga_mode_supported(mode)) {
        CPU_AX = R36SX_VBE_STATUS_FAIL;
        return;
    }

    bytes_per_scanline = r36sx_vbe_bytes_per_scanline(mode);
    width = vga_svga_mode_width(mode);
    height = vga_svga_mode_height(mode);
    mode_bytes = (uint32_t)bytes_per_scanline * height;
    bpp = vga_svga_mode_bpp(mode);
    pages = (uint8_t)((SVGA_VRAM_SIZE / mode_bytes) > 0 ?
                      (SVGA_VRAM_SIZE / mode_bytes) - 1u : 0u);

    r36sx_vbe_clear(base, 256);
    r36sx_vbe_write16(base, 0x00, 0x005Bu);
    r36sx_vbe_write8(base, 0x02, 0x07u);
    r36sx_vbe_write8(base, 0x03, 0x07u);
    r36sx_vbe_write16(base, 0x04, R36SX_VBE_WINDOW_KB);
    r36sx_vbe_write16(base, 0x06, R36SX_VBE_WINDOW_KB);
    r36sx_vbe_write16(base, 0x08, 0xA000u);
    r36sx_vbe_write16(base, 0x0A, 0xA000u);
    r36sx_vbe_write32(base, 0x0C, 0u);
    r36sx_vbe_write16(base, 0x10, bytes_per_scanline);
    r36sx_vbe_write16(base, 0x12, width);
    r36sx_vbe_write16(base, 0x14, height);
    r36sx_vbe_write8(base, 0x16, 8u);
    r36sx_vbe_write8(base, 0x17, 16u);
    r36sx_vbe_write8(base, 0x18, 1u);
    r36sx_vbe_write8(base, 0x19, bpp);
    r36sx_vbe_write8(base, 0x1A, r36sx_vbe_mode_banks(mode));
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
    r36sx_vbe_write16(base, 0x32, bytes_per_scanline);
    r36sx_vbe_write8(base, 0x34, pages);
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

static void r36sx_vbe_scanline_length(void)
{
    if (!vga_svga_mode_active()) {
        CPU_AX = R36SX_VBE_STATUS_FAIL;
        return;
    }

    switch (CPU_BL) {
        case 0x00:
            if (!vga_svga_set_scanline_pixels(CPU_CX)) {
                CPU_AX = R36SX_VBE_STATUS_FAIL;
                return;
            }
            break;
        case 0x02:
            if (!vga_svga_set_scanline_bytes(CPU_CX)) {
                CPU_AX = R36SX_VBE_STATUS_FAIL;
                return;
            }
            break;
        case 0x01:
            break;
        case 0x03:
            CPU_BX = vga_svga_max_scanline_bytes();
            CPU_CX = vga_svga_max_scanline_pixels();
            CPU_DX = vga_svga_max_scanlines();
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        default:
            CPU_AX = R36SX_VBE_STATUS_FAIL;
            return;
    }

    CPU_BX = vga_svga_bytes_per_scanline();
    CPU_CX = vga_svga_pixels_per_scanline();
    CPU_DX = vga_svga_max_scanlines();
    CPU_AX = R36SX_VBE_STATUS_OK;
}

static void r36sx_vbe_dac_format(void)
{
    switch (CPU_BL) {
        case 0x00:
            if (CPU_BH != 6 && CPU_BH != 8) {
                CPU_AX = R36SX_VBE_STATUS_FAIL;
                return;
            }
            r36sx_vbe_dac_width = CPU_BH;
            CPU_BH = r36sx_vbe_dac_width;
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        case 0x01:
            CPU_BH = r36sx_vbe_dac_width;
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        default:
            CPU_AX = R36SX_VBE_STATUS_FAIL;
            return;
    }
}

static void r36sx_vbe_palette_data(void)
{
    uint8_t subfunction = CPU_BL & 0x7Fu;
    uint32_t memloc = (uint32_t)CPU_ES * 16u + CPU_DI;

    switch (subfunction) {
        case 0x00:
            for (uint16_t i = 0; i < CPU_CX; i++) {
                uint16_t color_index = CPU_DX + i;
                uint8_t blue = read86(memloc++);
                uint8_t green = read86(memloc++);
                uint8_t red = read86(memloc++);
                memloc++;

                if (color_index < 256u) {
                    if (r36sx_vbe_dac_width == 8) {
                        vga_set_dac_color8((uint8_t)color_index,
                                           red, green, blue);
                    } else {
                        vga_set_dac_color((uint8_t)color_index,
                                          red & 0x3Fu,
                                          green & 0x3Fu,
                                          blue & 0x3Fu);
                    }
                }
            }
            CPU_AX = R36SX_VBE_STATUS_OK;
            return;
        case 0x01:
            for (uint16_t i = 0; i < CPU_CX; i++) {
                uint16_t color_index = CPU_DX + i;
                uint8_t red = 0;
                uint8_t green = 0;
                uint8_t blue = 0;

                if (color_index < 256u) {
                    if (r36sx_vbe_dac_width == 8) {
                        vga_get_dac_color8((uint8_t)color_index,
                                           &red, &green, &blue);
                    } else {
                        vga_get_dac_color((uint8_t)color_index,
                                          &red, &green, &blue);
                    }
                }
                write86(memloc++, blue);
                write86(memloc++, green);
                write86(memloc++, red);
                write86(memloc++, 0);
            }
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
        case 0x06:
            r36sx_vbe_scanline_length();
            return;
        case 0x08:
            r36sx_vbe_dac_format();
            return;
        case 0x09:
            r36sx_vbe_palette_data();
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

/* 80286 protected-mode interrupt helpers. */
#include "r36sx_cpu_80286_interrupts.inl"

static inline uint8_t r36sx_cpu_at_class_memory_available(void)
{
    /*
     * XMS, BIOS INT 15h extended-memory services, and HMA/A20-visible memory
     * are AT/286-class facilities.  Hide them when the user selects the
     * documented 8086 compatibility model.
     */
    return r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80286);
}

#define R36SX_BIOS_INT15_STATUS_OK 0x00u
#define R36SX_BIOS_INT15_STATUS_INVALID_COMMAND 0x80u
#define R36SX_BIOS_INT15_STATUS_UNSUPPORTED 0x86u
#define R36SX_BIOS_E820_SIGNATURE 0x534D4150UL
#define R36SX_BIOS_E820_ENTRY_SIZE 20u
#define R36SX_BIOS_E820_TYPE_MEMORY 1UL
#define R36SX_BIOS_E820_TYPE_RESERVED 2UL
#define R36SX_BIOS_EXTENDED_BELOW_16M_KB 0x3C00u

static inline uint32_t r36sx_bios_real_ptr(uint16_t segment, uint16_t offset)
{
    return ((uint32_t)segment << 4) + (uint32_t)offset;
}

static inline void r36sx_bios_int15_fail(uint8_t status)
{
    CPU_AH = status;
    CPU_FL_CF = 1;
}

static inline void r36sx_bios_int15_success_status(void)
{
    CPU_AH = R36SX_BIOS_INT15_STATUS_OK;
    CPU_FL_CF = 0;
}

static inline uint32_t r36sx_bios_descriptor_base(uint32_t descriptor)
{
    /* INT 15h AH=87h uses 286-style descriptors with a 24-bit base. */
    return (uint32_t)read86_ob(descriptor + 2u) |
           ((uint32_t)read86_ob(descriptor + 3u) << 8) |
           ((uint32_t)read86_ob(descriptor + 4u) << 16);
}

static inline uint32_t r36sx_bios_descriptor_limit(uint32_t descriptor)
{
    /*
     * INT 15h AH=87h is an AT/286 BIOS service.  Its caller supplies 286-style
     * descriptors, where the limit is exactly the low 16-bit limit field; the
     * 386 limit-high/granularity byte is not part of the contract here.
     */
    return (uint32_t)read86_ob(descriptor) |
           ((uint32_t)read86_ob(descriptor + 1u) << 8);
}

static void r36sx_bios_int15_move_extended_block(void)
{
    if (!r36sx_cpu_at_class_memory_available()) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    uint32_t bytes = (uint32_t)CPU_CX * 2u;
    if (bytes == 0u) {
        r36sx_bios_int15_success_status();
        return;
    }

    uint32_t gdt = r36sx_bios_real_ptr(CPU_ES, CPU_SI);
    uint32_t source_descriptor = gdt + 0x10u;
    uint32_t destination_descriptor = gdt + 0x18u;
    uint32_t source = r36sx_bios_descriptor_base(source_descriptor);
    uint32_t destination = r36sx_bios_descriptor_base(destination_descriptor);
    uint32_t source_limit = r36sx_bios_descriptor_limit(source_descriptor);
    uint32_t destination_limit =
        r36sx_bios_descriptor_limit(destination_descriptor);

    if (source_limit < bytes - 1u || destination_limit < bytes - 1u) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_INVALID_COMMAND);
        return;
    }

    /*
     * Real BIOS implementations switch to protected mode for this copy.  Force
     * A20 open while copying so physical addresses above 1 MB do not wrap.
     */
    int saved_a20 = a20_enabled;
    a20_enabled = 1;
    if (destination > source && destination < source + bytes) {
        for (uint32_t i = bytes; i > 0u; i--) {
            write86_ob(destination + i - 1u, read86_ob(source + i - 1u));
        }
    } else {
        for (uint32_t i = 0; i < bytes; i++) {
            write86_ob(destination + i, read86_ob(source + i));
        }
    }
    a20_enabled = saved_a20;

    r36sx_bios_int15_success_status();
}

static void r36sx_bios_int15_extended_memory_size(void)
{
    if (!r36sx_cpu_at_class_memory_available()) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    uint32_t kb = r36sx_pico286_extended_memory_kb();
    if (kb > 0xFFFFu) {
        kb = 0xFFFFu;
    }
    CPU_AX = (uint16_t)kb;
    CPU_FL_CF = 0;
}

static void r36sx_bios_int15_e801_memory_size(void)
{
    if (!r36sx_cpu_at_class_memory_available()) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    uint32_t extended_kb = r36sx_pico286_extended_memory_kb();
    uint32_t below_16m_kb = extended_kb;
    uint32_t above_16m_blocks = 0;

    if (below_16m_kb > R36SX_BIOS_EXTENDED_BELOW_16M_KB) {
        below_16m_kb = R36SX_BIOS_EXTENDED_BELOW_16M_KB;
        above_16m_blocks =
            (extended_kb - R36SX_BIOS_EXTENDED_BELOW_16M_KB) / 64u;
    }

    CPU_AX = (uint16_t)below_16m_kb;
    CPU_BX = (uint16_t)above_16m_blocks;
    CPU_CX = (uint16_t)below_16m_kb;
    CPU_DX = (uint16_t)above_16m_blocks;
    CPU_FL_CF = 0;
}

static inline void r36sx_bios_write_e820_entry(uint32_t buffer,
                                               uint32_t base,
                                               uint32_t length,
                                               uint32_t type)
{
    writedw86(buffer + 0u, base);
    writedw86(buffer + 4u, 0u);
    writedw86(buffer + 8u, length);
    writedw86(buffer + 12u, 0u);
    writedw86(buffer + 16u, type);
}

static void r36sx_bios_int15_e820_memory_map(void)
{
    if (!r36sx_cpu_at_class_memory_available()) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }
    if (CPU_EDX != R36SX_BIOS_E820_SIGNATURE ||
        CPU_ECX < R36SX_BIOS_E820_ENTRY_SIZE) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    uint32_t conventional_kb = r36sx_pico286_conventional_memory_kb();
    if (conventional_kb > 640u) {
        conventional_kb = 640u;
    }
    uint32_t extended_kb = r36sx_pico286_extended_memory_kb();
    uint32_t entry_count = extended_kb ? 3u : 2u;
    uint32_t entry_index = CPU_EBX;

    if (entry_index >= entry_count) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    uint32_t buffer = r36sx_bios_real_ptr(CPU_ES, CPU_DI);
    switch (entry_index) {
        case 0:
            r36sx_bios_write_e820_entry(
                buffer, 0x00000000u, conventional_kb << 10,
                R36SX_BIOS_E820_TYPE_MEMORY);
            break;
        case 1:
            r36sx_bios_write_e820_entry(
                buffer, 0x000A0000u, 0x00060000u,
                R36SX_BIOS_E820_TYPE_RESERVED);
            break;
        default:
            r36sx_bios_write_e820_entry(
                buffer, 0x00100000u, extended_kb << 10,
                R36SX_BIOS_E820_TYPE_MEMORY);
            break;
    }

    CPU_EAX = R36SX_BIOS_E820_SIGNATURE;
    CPU_EBX = (entry_index + 1u < entry_count) ? entry_index + 1u : 0u;
    CPU_ECX = R36SX_BIOS_E820_ENTRY_SIZE;
    CPU_FL_CF = 0;
}

static void r36sx_bios_int15_a20_service(void)
{
    if (!r36sx_cpu_at_class_memory_available()) {
        r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
        return;
    }

    switch (CPU_AX) {
        case 0x2400:
            a20_enabled = 0;
            r36sx_bios_int15_success_status();
            return;
        case 0x2401:
            a20_enabled = 1;
            r36sx_bios_int15_success_status();
            return;
        case 0x2402:
            CPU_AL = a20_enabled ? 1u : 0u;
            CPU_AH = R36SX_BIOS_INT15_STATUS_OK;
            CPU_FL_CF = 0;
            return;
        case 0x2403:
            CPU_BX = 0x0003u; /* 8042 keyboard gate and fast port 92. */
            r36sx_bios_int15_success_status();
            return;
        default:
            r36sx_bios_int15_fail(R36SX_BIOS_INT15_STATUS_UNSUPPORTED);
            return;
    }
}

void intcall86(uint8_t intnum) {
    r36sx_pm_diag_log_interrupt(intnum);

    if (r36sx_cpu_protected_enabled() &&
        r36sx_cpu_protected_interrupt(intnum, 0, 0, 0, CPU_IP)) {
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
                    vga_set_standard_mode((uint8_t)videomode);

                    FIRST_RAM_PAGE[0x449] = CPU_AL;
                    FIRST_RAM_PAGE[0x44A] = videomode <= 1 || (videomode >= 0x8 && videomode <= 0xa) ? 40 : 80;
                    FIRST_RAM_PAGE[0x44B] = 0;
                    FIRST_RAM_PAGE[0x484] = (25 - 1);

                    if ((CPU_AL & 0x80) == 0x00) {
                        memset(VIDEORAM, 0x0, sizeof(VIDEORAM));
                    }
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
                                    r36sx_pico286_vga_palette565_set(color_index,
                                                                     color);
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
                                    r36sx_pico286_vga_palette565_set(color_index,
                                                                     color);
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
        case 0x15: /* System BIOS services. */
            switch (CPU_AH) {
                case 0x24:
                    /* AX=2400h..2403h: BIOS A20 gate disable/enable/query. */
                    r36sx_bios_int15_a20_service();
                    return;
                case 0x87:
                    /* AH=87h: copy words using source/destination GDT entries. */
                    r36sx_bios_int15_move_extended_block();
                    return;
                case 0x88:
                    /* AH=88h: legacy extended-memory size in KB above 1 MB. */
                    r36sx_bios_int15_extended_memory_size();
                    return;
                case 0xE8:
                    /* AX=E801h and E820h: later BIOS extended-memory reports. */
                    if (CPU_AX == 0xE801u) {
                        r36sx_bios_int15_e801_memory_size();
                        return;
                    }
                    if (CPU_EAX == 0x0000E820u) {
                        r36sx_bios_int15_e820_memory_map();
                        return;
                    }
                    break;
            }
            break;
        case 0x2F: /* DOS multiplex interrupt compatibility hook. */
            if (r36sx_pico286_int2f_enabled()) {
                switch (CPU_AX) {
                    case 0x4300:
                        /*
                         * XMS installation check.  This mirrors a tiny HIMEM
                         * manager shim so DOS programs can discover the
                         * already-built-in XMS handler at XMS_FN_CS:XMS_FN_IP.
                         */
                        CPU_AL = r36sx_cpu_at_class_memory_available()
                                     ? 0x80
                                     : 0x00;
                        return;
                    case 0x4310:
                        if (!r36sx_cpu_at_class_memory_available()) {
                            CPU_AL = 0x00;
                            return;
                        }
                        r36sx_cpu_load_segment(reges, XMS_FN_CS);
                        CPU_BX = XMS_FN_IP;
                        return;
                    default:
                        if (redirector_handler()) {
                            return;
                        }
                        break;
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
#if !PICO_ON_DEVICE
            if (r36sx_bios_rtc_int1a()) {
                return;
            }
#else
            switch (CPU_AH) {
                case 0x02: /* 02H: Read Time from Real-Time Clock */
                    CPU_CX = 0x2259;
                    CPU_DX = 0x0001;
                    CPU_FL_CF = 0;
                    return;
                case 0x04: /* 04H: Read Date from Real-Time Clock */
                    CPU_CX = 0x2026;
                    CPU_DX = 0x0603;
                    CPU_AH = 0;
                    CPU_FL_CF = 0;
                    return;
            }
#endif
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

static void r36sx_cpu_software_interrupt(uint8_t intnum, uint32_t fault_ip)
{
    if (r36sx_cpu_protected_enabled()) {
        r36sx_pm_diag_log_interrupt(intnum);
        (void)r36sx_cpu_protected_interrupt(intnum, 0, 0, 1, fault_ip);
        return;
    }

    intcall86(intnum);
}

/* 8086/80186-compatible arithmetic, flag, and group-op helpers. */
#include "r36sx_cpu_8086.inl"

/* 80386 operand-size, 32-bit, and extended-opcode helpers. */
#include "r36sx_cpu_80386.inl"







void reset86() {
#if !PICO_ON_DEVICE
    r36sx_cpu_strict_8086_mode =
        r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086;
#else
    r36sx_cpu_strict_8086_mode = 0;
#endif
    CPU_CS = 0xFFFF;
    CPU_SS = 0x0000;
    CPU_SP = 0x0000;
    hltstate = 0;
    r36sx_cpu_maskable_interrupt_shadow = 0;
    r36sx_cpu_current_cpl = 0;
    r36sx_cr0 = R36SX_CR0_ET;
    r36sx_cr2 = 0;
    r36sx_cr3 = 0;
    memset(r36sx_dr, 0, sizeof(r36sx_dr));
    memset(r36sx_tr, 0, sizeof(r36sx_tr));
    r36sx_dr[6] = R36SX_DR6_RESET;
    r36sx_dr[7] = R36SX_DR7_RESET;
    r36sx_debug_resume_valid = 0;
    r36sx_debug_resume_cs = 0;
    r36sx_debug_resume_ip = 0;
    r36sx_debug_pending_dr6_hits = 0;
    r36sx_debug_suppress_watchpoints = 0;
    r36sx_gdtr_base = 0;
    r36sx_gdtr_limit = 0;
    r36sx_idtr_base = 0;
    r36sx_idtr_limit = 0x03ffu;
    r36sx_ldtr_selector = 0;
    r36sx_tr_selector = 0;
#if R36SX_DEBUG_PM_DIAG
    r36sx_pm_diag_first_fault_logged = 0;
#endif
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

static void __not_in_flash() r36sx_cpu_exec86_core(uint32_t execloops) {
    static uint32_t firstip;
    static bool was_TF;
    uint32_t loopcount = 0;

    //counterticks = (uint64_t) ( (double) timerfreq / (double) 65536.0);
    //tickssource();
    for (loopcount = 0; loopcount < execloops; loopcount++) {
        uint8_t maskable_irq_shadowed = r36sx_cpu_maskable_interrupt_shadow;
        if (unlikely(hltstate)) {
            if (unlikely(ifl && !maskable_irq_shadowed &&
                         r36sx_cpu_pending_maskable_irq())) {
                hltstate = 0;
                intcall86(nextintr());
            } else {
                r36sx_app_stats_record_x86(loopcount);
                return;
            }
        } else if (unlikely(ifl && !maskable_irq_shadowed &&
                            r36sx_cpu_pending_maskable_irq())) {
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
        operandSizeOverride = r36sx_cpu_code_default32();
        addressSizeOverride = r36sx_cpu_code_default32();
        r36sx_cpu_use_segment(regds);
        uint8_t docontinue = 0;
        uint8_t prefix_exception = 0;
        firstip = CPU_IP;
        register uint8_t opcode;

        if (unlikely(r36sx_cpu_debug_check_execute_breakpoint(firstip))) {
            continue;
        }

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
                    r36sx_cpu_use_segment(regcs);
                    segoverride = 1;
                    break;

                case 0x3E: /* segment CPU_DS */
                    r36sx_cpu_use_segment(regds);
                    segoverride = 1;
                    break;

                case 0x26: /* segment CPU_ES */
                    r36sx_cpu_use_segment(reges);
                    segoverride = 1;
                    break;

                case 0x36: /* segment CPU_SS */
                    r36sx_cpu_use_segment(regss);
                    segoverride = 1;
                    break;

#if CPU_386_EXTENDED_OPS
                case 0x64: /* segment CPU_FS */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    r36sx_cpu_use_segment(regfs);
                    segoverride = 1;
                    break;

                case 0x65: /* segment CPU_GS */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    r36sx_cpu_use_segment(reggs);
                    segoverride = 1;
                    break;
#else
                case 0x64:
                case 0x65:
                    r36sx_cpu_set_ip(firstip);
                    intcall86(6);
                    prefix_exception = 1;
                    docontinue = 1;
                    break;
#endif

#if CPU_386_EXTENDED_OPS
                case 0x66: /* operand-size override */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    operandSizeOverride = !operandSizeOverride;
                    break;

                case 0x67: /* address-size override */
                    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386)) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    addressSizeOverride = !addressSizeOverride;
                    break;
#endif

                case 0xF0: /* LOCK */
                    if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                        prefix_exception = 1;
                        docontinue = 1;
                    }
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

        r36sx_cpu_debug_trace_test386_instruction(firstip, opcode);

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
            &&r36sx_opcode_60, &&r36sx_opcode_61, &&r36sx_opcode_62, &&r36sx_opcode_63, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_66, &&r36sx_opcode_67,
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
                r36sx_cpu_push_segment_selector(CPU_ES);
                break;

            case 0x7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_07: ;
#endif
                /* 07 POP CPU_ES */
                r36sx_cpu_load_segment_at(reges,
                                          r36sx_cpu_pop_segment_selector(),
                                          firstip);
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
                r36sx_cpu_push_segment_selector(CPU_CS);
                break;

            case 0xF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_0F: ;
#endif
                if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    /* 8086/8088 only: 0F POP CS. */
                    r36sx_cpu_load_segment(regcs,
                                           r36sx_cpu_pop_segment_selector());
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
                r36sx_cpu_push_segment_selector(CPU_SS);
                break;

            case 0x17:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_17: ;
#endif
                /* 17 POP CPU_SS */
                if (r36sx_cpu_load_segment_at(
                        regss, r36sx_cpu_pop_segment_selector(), firstip)) {
                    r36sx_cpu_delay_maskable_interrupts_one_instruction();
                }
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
                r36sx_cpu_push_segment_selector(CPU_DS);
                break;

            case 0x1F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_1F: ;
#endif
                /* 1F POP CPU_DS */
                r36sx_cpu_load_segment_at(regds,
                                          r36sx_cpu_pop_segment_selector(),
                                          firstip);
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
                    r36sx_cpu_adjust_stack(4u);
                    CPU_EBX = pop32();
                    CPU_EDX = pop32();
                    CPU_ECX = pop32();
                    CPU_EAX = pop32();
                    break;
                }
                CPU_DI = pop();
                CPU_SI = pop();
                CPU_BP = pop();
                r36sx_cpu_adjust_stack(2u);
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

                /* Intel BOUND requires a memory bounds table operand. */
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                getea(rm);
                if (operandSizeOverride) {
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, 8u, 0)) {
                        break;
                    }
                    int32_t value = (int32_t)getreg32(reg);
                    int32_t lower = (int32_t)readdw86(ea);
                    int32_t upper = (int32_t)readdw86(ea + 4u);
                    if (value < lower || value > upper) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(R36SX_EXCEPTION_BOUND);
                    }
                } else {
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, 4u, 0)) {
                        break;
                    }
                    int32_t value = signext32(getreg16(reg));
                    int32_t lower = signext32(readw86(ea));
                    int32_t upper = signext32(readw86(ea + 2u));
                    if (value < lower || value > upper) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(R36SX_EXCEPTION_BOUND);
                    }
                }
                break;
            case 0x63:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_63: ;
#endif
                /* 63 ARPL Ew,Gw (80286+ protected mode) */
                if (!r36sx_cpu_native_protected_enabled() ||
                    r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                {
                    uint16_t dest = readrm16(rm);
                    uint8_t src_rpl = getreg16(reg) & 3u;
                    if ((dest & 3u) < src_rpl) {
                        dest = (uint16_t)((dest & 0xfffcu) | src_rpl);
                        writerm16(rm, dest);
                        zf = 1;
                    } else {
                        zf = 0;
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
                if (!r36sx_cpu_require_io_permission(CPU_DX, 1u, firstip)) {
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

                r36sx_cpu_set_ip(firstip);
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
                if (!r36sx_cpu_require_io_permission(CPU_DX, 2u, firstip)) {
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

                r36sx_cpu_set_ip(firstip);
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
                if (!r36sx_cpu_require_io_permission(CPU_DX, 1u, firstip)) {
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

                r36sx_cpu_set_ip(firstip);
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
                if (!r36sx_cpu_require_io_permission(CPU_DX, 2u, firstip)) {
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

                r36sx_cpu_set_ip(firstip);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                    r36sx_cpu_add_ip((int16_t)temp16);
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
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* Intel LEA takes a memory addressing form, not a register. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                putreg16(reg, ea
                         -
                         useseg_base
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

                if (r36sx_cpu_load_segment_at(reg, readrm16(rm), firstip) &&
                    reg == regss) {
                    r36sx_cpu_delay_maskable_interrupts_one_instruction();
                }
                break;

            case 0x8F:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_8F: ;
#endif
                /* 8F POP Ev */
                modregrm();
                if (reg != 0) {
                    /* 8F is a group opcode; only /0 is documented as POP Ev. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

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
                if (r36sx_cpu_protected_enabled()) {
                    r36sx_cpu_protected_far_call(oper2, oper1, 0, firstip);
                    break;
                }
                push(CPU_CS);
                push(CPU_IP);
                r36sx_cpu_load_segment(regcs, oper2);
                r36sx_cpu_set_ip(oper1);
                break;

            case 0x9B:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9B: ;
#endif
                /* 9B WAIT */
                if (!r36sx_cpu_x87_wait_available(firstip)) {
                    break;
                }
                if (!r36sx_cpu_x87_present()) {
                    break;
                }
                if (OpFwait()) {
                    r36sx_cpu_raise_exception(
                        R36SX_EXCEPTION_X87_ERROR, 0, 0, firstip);
                }
                break;

            case 0x9C:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_9C: ;
#endif
                /* 9C PUSHF */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
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
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                if (operandSizeOverride) {
                    decodeflagsdword(pop32());
                    break;
                }
                decodeflagsword(pop());
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
                CPU_AL = getmem8(useseg, r36sx_read_moffs());
                break;

            case 0xA1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A1: ;
#endif
                /* A1 MOV eAX Ov */
                oper1 = getmem16(useseg, r36sx_read_moffs());
                CPU_AX = oper1;
                break;

            case 0xA2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A2: ;
#endif
                /* A2 MOV Ob CPU_AL */
                putmem8(useseg, r36sx_read_moffs(), CPU_AL);
                break;

            case 0xA3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A3: ;
#endif
                /* A3 MOV Ov eAX */
                putmem16(useseg, r36sx_read_moffs(), CPU_AX);
                break;

            case 0xA4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A4: ;
#endif
                /* A4 MOVSB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_movsb(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
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
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_movsw(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
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
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    uint32_t di = r36sx_dst_index();
                    oper1b = getmem8(useseg, si);
                    oper2b = getmem8(CPU_ES, di);
                    r36sx_set_src_index(df ? si - 1u : si + 1u);
                    r36sx_set_dst_index(df ? di - 1u : di + 1u);
                }

                flag_sub8(oper1b, oper2b
                );
                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
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

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xA7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_A7: ;
#endif
                /* A7 CMPSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    uint32_t di = r36sx_dst_index();
                    oper1 = getmem16(useseg, si);
                    oper2 = getmem16(CPU_ES, di);
                    r36sx_set_src_index(df ? si - 2u : si + 2u);
                    r36sx_set_dst_index(df ? di - 2u : di + 2u);
                }

                flag_sub16(oper1, oper2
                );
                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
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

                r36sx_cpu_set_ip(firstip);
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
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_stosb(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
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
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_stosw(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
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
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    CPU_AL = getmem8(useseg, si);
                    r36sx_set_src_index(df ? si - 1u : si + 1u);
                }

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AD: ;
#endif
                /* AD LODSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    oper1 = getmem16(useseg, si);
                    CPU_AX = oper1;
                    r36sx_set_src_index(df ? si - 2u : si + 2u);
                }

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                loopcount++;
                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AE: ;
#endif
                /* AE SCASB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                oper1b = CPU_AL;
                {
                    uint32_t di = r36sx_dst_index();
                    oper2b = getmem8(CPU_ES, di);
                    r36sx_set_dst_index(df ? di - 1u : di + 1u);
                }
                flag_sub8(oper1b, oper2b
                );

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
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

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_AF: ;
#endif
                /* AF SCASW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                oper1 = CPU_AX;
                {
                    uint32_t di = r36sx_dst_index();
                    oper2 = getmem16(CPU_ES, di);
                    r36sx_set_dst_index(df ? di - 2u : di + 2u);
                }
                flag_sub16(oper1, oper2
                );

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
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

                r36sx_cpu_set_ip(firstip);
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
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_adjust_stack(oper1);
                break;

            case 0xC3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C3: ;
#endif
                /* C3 RET */
                r36sx_cpu_set_ip(pop());
                break;

            case 0xC4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C4: ;
#endif
                /* C4 LES Gv Mp */
                modregrm();
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* LES loads a far pointer from memory. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                {
                    uint8_t pointer_size = operandSizeOverride ? 6u : 4u;
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, pointer_size, 0)) {
                        break;
                    }
                    uint32_t offset = operandSizeOverride ? readdw86(ea) :
                                                            readw86(ea);
                    uint16_t selector = readw86(ea + (operandSizeOverride ?
                                                      4u : 2u));
                    if (r36sx_cpu_load_segment(reges, selector)) {
                        if (operandSizeOverride) {
                            putreg32(reg, offset);
                        } else {
                            putreg16(reg, (uint16_t)offset);
                        }
                    }
                }
                break;

            case 0xC5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C5: ;
#endif
                /* C5 LDS Gv Mp */
                modregrm();
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* LDS loads a far pointer from memory. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                {
                    uint8_t pointer_size = operandSizeOverride ? 6u : 4u;
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, pointer_size, 0)) {
                        break;
                    }
                    uint32_t offset = operandSizeOverride ? readdw86(ea) :
                                                            readw86(ea);
                    uint16_t selector = readw86(ea + (operandSizeOverride ?
                                                      4u : 2u));
                    if (r36sx_cpu_load_segment(regds, selector)) {
                        if (operandSizeOverride) {
                            putreg32(reg, offset);
                        } else {
                            putreg16(reg, (uint16_t)offset);
                        }
                    }
                }
                break;

            case 0xC6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_C6: ;
#endif
                /* C6 MOV Eb Ib */
                modregrm();
                if (reg != 0) {
                    /* C6 is a group opcode; only /0 is documented as MOV Eb,Ib. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

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
                if (reg != 0) {
                    /* C7 is a group opcode; only /0 is documented as MOV Ev,Iv. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

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
                r36sx_cpu_enter16(stacksize, nestlev);

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
                r36sx_cpu_set_stack_pointer(CPU_BP);
                CPU_BP = pop();
                break;

            case 0xCA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CA: ;
#endif
                /* CA RETF Iw */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_retf(oper1, 0, firstip);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                r36sx_cpu_adjust_stack(oper1);
                break;

            case 0xCB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CB: ;
#endif
                /* CB RETF */
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_retf(0, 0, firstip);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                break;

            case 0xCC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CC: ;
#endif
                /* CC INT 3 */
                r36sx_cpu_software_interrupt(3, firstip);
                break;

            case 0xCD:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CD: ;
#endif
                /* CD INT Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                r36sx_cpu_software_interrupt(oper1b, firstip);
                break;

            case 0xCE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CE: ;
#endif
                /* CE INTO */
                if (of) {
                    r36sx_cpu_software_interrupt(4, firstip);
                }
                break;

            case 0xCF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_CF: ;
#endif
                /* CF IRET */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_iret(0);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                decodeflagsword(pop());
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
                    r36sx_cpu_divide_error(firstip);
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
                /*
                 * D6 is outside Intel's documented 8086/8088 instruction set.
                 * Some chips execute undocumented SALC here; the strict CPU
                 * model reports it through the same invalid-opcode path as
                 * other non-documented opcode bytes.
                 */
                r36sx_cpu_invalid_opcode(firstip);
                break;
#endif

            case 0xD7:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_D7: ;
#endif
                /* D7 XLAT */
                CPU_AL = getmem8(useseg,
                                  (addressSizeOverride ? CPU_EBX : CPU_BX) +
                                  CPU_AL);
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
                if (!r36sx_cpu_x87_escape_available(firstip)) {
                    break;
                }
                if (!r36sx_cpu_x87_present()) {
                    r36sx_cpu_skip_x87_escape();
                    break;
                }
                OpFpu(opcode);
                break;

            case 0xE0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E0: ;
#endif
                /* E0 LOOPNZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count && !zf) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE1:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E1: ;
#endif
                /* E1 LOOPZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count && (zf == 1)) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE2:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E2: ;
#endif
                /* E2 LOOP Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE3:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E3: ;
#endif
                /* E3 JCXZ/JECXZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!r36sx_loop_get_count()) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0xE4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E4: ;
#endif
                /* E4 IN CPU_AL Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 1u, firstip)) {
                    break;
                }
                CPU_AL = (uint8_t) portin(oper1b);
                break;

            case 0xE5:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E5: ;
#endif
                /* E5 IN eAX Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 2u, firstip)) {
                    break;
                }
                CPU_AX = portin16(oper1b);
                break;

            case 0xE6:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E6: ;
#endif
                /* E6 OUT Ib CPU_AL */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 1u, firstip)) {
                    break;
                }
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
                if (!r36sx_cpu_require_io_permission(oper1b, 2u, firstip)) {
                    break;
                }
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
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xE9:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_E9: ;
#endif
                /* E9 JMP Jv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xEA:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EA: ;
#endif
                /* EA JMP Ap */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                if (r36sx_cpu_protected_enabled()) {
                    r36sx_cpu_protected_far_jump(oper2, oper1, firstip);
                    break;
                }
                r36sx_cpu_load_segment(regcs, oper2);
                r36sx_cpu_set_ip(oper1);
                break;

            case 0xEB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EB: ;
#endif
                /* EB JMP Jb */
                oper1 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xEC:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EC: ;
#endif
                /* EC IN CPU_AL regdx */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 1u, firstip)) {
                    break;
                }
                CPU_AL = (uint8_t) portin(oper1);
                break;

            case 0xED:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_ED: ;
#endif
                /* ED IN eAX regdx */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 2u, firstip)) {
                    break;
                }
                CPU_AX = portin16(oper1);
                break;

            case 0xEE:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EE: ;
#endif
                /* EE OUT regdx CPU_AL */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 1u, firstip)) {
                    break;
                }
                portout(oper1, CPU_AL
                );
                break;

            case 0xEF:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_EF: ;
#endif
                /* EF OUT regdx eAX */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 2u, firstip)) {
                    break;
                }
                portout16(oper1, CPU_AX);
                break;

            case 0xF0:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F0: ;
#endif
                /* F0 LOCK */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                break;

            case 0xF4:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_F4: ;
#endif
                /* F4 HLT */
                if (!r36sx_cpu_require_cpl0(firstip)) {
                    break;
                }
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
                    case 0: /* TEST */
                        flag_log8(oper1b & getmem8(CPU_CS, CPU_IP));
                        StepIP(1);
                        break;

                    case 1:
                        /*
                         * Some 8086-class chips alias F6 /1 to TEST, but
                         * Intel documents only F6 /0.  Keep strict CPU-model
                         * filtering consistent with the official opcode map.
                         */
                        r36sx_cpu_invalid_opcode(firstip);
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
                        op_div8(CPU_AX, oper1b, firstip);
                        break;

                    case 7: /* IDIV */
                        op_idiv8(CPU_AX, oper1b, firstip);
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
                op_grp3_16(firstip);
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
                if (!r36sx_cpu_require_iopl(firstip)) {
                    break;
                }
                ifl = 0;
                break;

            case 0xFB:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_FB: ;
#endif
                /* FB STI */
                if (!r36sx_cpu_require_iopl(firstip)) {
                    break;
                }
                ifl = 1;
                r36sx_cpu_delay_maskable_interrupts_one_instruction();
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
                if (reg > 1) {
                    /* FE is a group opcode; only /0 INC and /1 DEC are valid. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
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
                if (reg == 7 ||
                    ((reg == 3 || reg == 5) &&
                     mode == R36SX_MODRM_MOD_REGISTER)) {
                    /*
                     * FF /7 is undefined, and far CALL/JMP (/3,/5) require
                     * a memory far pointer rather than a register operand.
                     */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                oper1 = readrm16(rm);
                op_grp5(firstip);
                break;

            default:
#if R36SX_CPU_COMPUTED_GOTO
            r36sx_opcode_default: ;
#endif
#ifdef CPU_ALLOW_ILLEGAL_OP_EXCEPTION
                r36sx_cpu_set_ip(firstip);
                intcall86(6); /* trip invalid opcode exception. this occurs on the 80186+, 8086/8088 CPUs treat them as NOPs. */
                /* technically they aren't exactly like NOPs in most cases, but for our pursoses, that's accurate enough. */
                r36sx_pico286_debug_log("[CPU] Invalid opcode 0x%02x exception at %04X:%04X",
                                        opcode, CPU_CS, firstip);
#endif
                break;
        }
r36sx_opcode_done:
        if (unlikely(r36sx_cpu_maskable_interrupt_shadow != 0u)) {
            r36sx_cpu_maskable_interrupt_shadow--;
        }
        if (unlikely(r36sx_debug_pending_dr6_hits || was_TF)) {
            uint32_t dr6_hits = r36sx_debug_pending_dr6_hits;
            r36sx_debug_pending_dr6_hits = 0;
            if (was_TF) {
                dr6_hits |= R36SX_DR6_BS_MASK;
            }
            was_TF = false;
            r36sx_cpu_raise_debug_exception(CPU_IP, dr6_hits);
        } else if (tf) {
            was_TF = true;
        }
    }
    r36sx_app_stats_record_x86(loopcount);
}

static void __not_in_flash() r36sx_cpu_exec86_real(uint32_t execloops)
{
    r36sx_cpu_interpreter_protected = 0;
    r36sx_cpu_exec86_core(execloops);
}

static void __not_in_flash() r36sx_cpu_exec86_protected(uint32_t execloops)
{
    r36sx_cpu_interpreter_protected = 1;
    r36sx_cpu_exec86_core(execloops);
}

void __not_in_flash() exec86(uint32_t execloops)
{
    if (unlikely(r36sx_cpu_protected_enabled())) {
        r36sx_cpu_exec86_protected(execloops);
        return;
    }
    r36sx_cpu_exec86_real(execloops);
}
