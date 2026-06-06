#include "r36sx_cpu.h"
#include "r36sx_cpu_8086.h"
#include "r36sx_cpu_80286.h"
#include "r36sx_cpu_80386.h"
#include "r36sx_disk_config.h"

static r36sx_cpu_exec_fn selected_exec = exec386;
static const char *selected_exec_name = "exec386";

r36sx_cpu_exec_fn r36sx_cpu_select_exec(void)
{
    switch (r36sx_pico286_cpu_model()) {
        case R36SX_PICO286_CPU_8086:
            selected_exec = exec86;
            selected_exec_name = "exec86";
            break;
        case R36SX_PICO286_CPU_80286:
            selected_exec = exec286;
            selected_exec_name = "exec286";
            break;
        case R36SX_PICO286_CPU_80386:
        default:
            selected_exec = exec386;
            selected_exec_name = "exec386";
            break;
    }
    return selected_exec;
}

const char *r36sx_cpu_selected_exec_name(void)
{
    return selected_exec_name;
}
