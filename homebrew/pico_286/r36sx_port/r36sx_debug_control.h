#ifndef R36SX_DEBUG_CONTROL_H
#define R36SX_DEBUG_CONTROL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void r36sx_debug_control_init(void);
void r36sx_debug_control_poll(void);
void r36sx_debug_control_set_framebuffer(const uint16_t *pixels,
                                         uint32_t width,
                                         uint32_t height,
                                         uint32_t stride_pixels);

#ifdef __cplusplus
}
#endif

#endif
