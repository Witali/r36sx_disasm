#include "r36sx_cpu_80286.h"
#include "r36sx_cpu_internal.h"

void exec286(uint32_t execloops)
{
    if (r36sx_cpu_is_protected_enabled()) {
        r36sx_cpu_exec_protected(execloops);
        return;
    }
    r36sx_cpu_exec_real(execloops);
}
