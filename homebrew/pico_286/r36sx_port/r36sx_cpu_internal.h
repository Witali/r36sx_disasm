#ifndef R36SX_CPU_INTERNAL_H
#define R36SX_CPU_INTERNAL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t r36sx_cpu_is_protected_enabled(void);
void r36sx_cpu_exec_8086(uint32_t execloops);
void r36sx_cpu_exec_286(uint32_t execloops);
void r36sx_cpu_exec_real(uint32_t execloops);
void r36sx_cpu_exec_protected(uint32_t execloops);

#ifdef __cplusplus
}
#endif

#endif
