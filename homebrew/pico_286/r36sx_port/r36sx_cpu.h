#ifndef R36SX_CPU_H
#define R36SX_CPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*r36sx_cpu_exec_fn)(uint32_t execloops);

#define R36SX_CPU_DEBUG_SEGMENT_COUNT 6u
#define R36SX_CPU_DEBUG_REGISTER_COUNT 8u

typedef struct r36sx_cpu_debug_segment_cache_s {
    uint16_t selector;
    uint32_t base;
    uint32_t limit;
    uint8_t access;
    uint8_t flags;
    uint8_t valid;
} r36sx_cpu_debug_segment_cache_t;

typedef struct r36sx_cpu_debug_snapshot_s {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
    uint32_t eflags;
    uint16_t es;
    uint16_t cs;
    uint16_t ss;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint32_t cr0;
    uint32_t cr2;
    uint32_t cr3;
    uint32_t debug_regs[R36SX_CPU_DEBUG_REGISTER_COUNT];
    uint32_t test_regs[R36SX_CPU_DEBUG_REGISTER_COUNT];
    uint32_t gdtr_base;
    uint32_t idtr_base;
    uint16_t gdtr_limit;
    uint16_t idtr_limit;
    uint16_t ldtr_selector;
    uint16_t tr_selector;
    uint32_t segment_values[R36SX_CPU_DEBUG_SEGMENT_COUNT];
    uint16_t segment_selectors[R36SX_CPU_DEBUG_SEGMENT_COUNT];
    uint32_t segment_bases[R36SX_CPU_DEBUG_SEGMENT_COUNT];
    r36sx_cpu_debug_segment_cache_t segment_cache[R36SX_CPU_DEBUG_SEGMENT_COUNT];
    r36sx_cpu_debug_segment_cache_t ldtr_cache;
    r36sx_cpu_debug_segment_cache_t tr_cache;
    uint8_t protected_mode;
    uint8_t native_protected_mode;
    uint8_t vm86_mode;
    uint8_t cpl;
    uint8_t iopl;
} r36sx_cpu_debug_snapshot_t;

r36sx_cpu_exec_fn r36sx_cpu_select_exec(void);
const char *r36sx_cpu_selected_exec_name(void);
void r36sx_cpu_debug_snapshot(r36sx_cpu_debug_snapshot_t *snapshot);

#ifdef __cplusplus
}
#endif

#endif
