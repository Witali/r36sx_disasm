#ifndef R36SX_CPU_H
#define R36SX_CPU_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*r36sx_cpu_exec_fn)(uint32_t execloops);

r36sx_cpu_exec_fn r36sx_cpu_select_exec(void);
const char *r36sx_cpu_selected_exec_name(void);

#ifdef __cplusplus
}
#endif

#endif
