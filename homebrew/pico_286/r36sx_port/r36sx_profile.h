#ifndef R36SX_PROFILE_H
#define R36SX_PROFILE_H

#include <stdint.h>

#ifndef R36SX_ENABLE_PROFILING
#define R36SX_ENABLE_PROFILING 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    R36SX_PROFILE_EXEC86 = 0,
    R36SX_PROFILE_DISK_FLUSH,
    R36SX_PROFILE_MFB_UPDATE,
    R36SX_PROFILE_KEYBOARD_TICK,
    R36SX_PROFILE_TIMER_IRQ,
    R36SX_PROFILE_DSS_SAMPLE,
    R36SX_PROFILE_SB_SAMPLE,
    R36SX_PROFILE_AUDIO_SAMPLE,
    R36SX_PROFILE_AUDIO_WRITE,
    R36SX_PROFILE_RENDERER,
    R36SX_PROFILE_SOFT_RESET,
    R36SX_PROFILE_COUNT
} r36sx_profile_id_t;

void r36sx_profile_init(void);
int r36sx_profile_is_enabled(void);
uint64_t r36sx_profile_now_us(void);
void r36sx_profile_record(r36sx_profile_id_t id, uint64_t elapsed_us,
                          uint32_t units);
void r36sx_profile_maybe_log(void);

#ifdef __cplusplus
}
#endif

#if R36SX_ENABLE_PROFILING

#define R36SX_PROFILE_BEGIN(var_name) \
    uint64_t var_name = r36sx_profile_is_enabled() ? \
        r36sx_profile_now_us() : 0u

#define R36SX_PROFILE_END(id, var_name) \
    do { \
        if ((var_name) != 0u) { \
            r36sx_profile_record((id), \
                                 r36sx_profile_now_us() - (var_name), 1u); \
        } \
    } while (0)

#define R36SX_PROFILE_END_UNITS(id, var_name, unit_count) \
    do { \
        if ((var_name) != 0u && (unit_count) != 0u) { \
            r36sx_profile_record((id), \
                                 r36sx_profile_now_us() - (var_name), \
                                 (uint32_t)(unit_count)); \
        } \
    } while (0)

#else

#define R36SX_PROFILE_BEGIN(var_name) \
    do { } while (0)

#define R36SX_PROFILE_END(id, var_name) \
    do { (void)(id); } while (0)

#define R36SX_PROFILE_END_UNITS(id, var_name, unit_count) \
    do { (void)(id); (void)(unit_count); } while (0)

#endif

#endif
