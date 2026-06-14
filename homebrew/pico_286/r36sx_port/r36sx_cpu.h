#ifndef R36SX_CPU_H
#define R36SX_CPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*r36sx_cpu_exec_fn)(uint32_t execloops);

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
    uint8_t protected_mode;
    uint8_t vm86_mode;
    uint8_t cpl;
} r36sx_cpu_debug_snapshot_t;

r36sx_cpu_exec_fn r36sx_cpu_select_exec(void);
const char *r36sx_cpu_selected_exec_name(void);
void r36sx_cpu_debug_snapshot(r36sx_cpu_debug_snapshot_t *snapshot);

#ifdef __cplusplus
}
#endif

#endif
