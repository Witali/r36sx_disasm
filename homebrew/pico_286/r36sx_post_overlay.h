#ifndef R36SX_POST_OVERLAY_H
#define R36SX_POST_OVERLAY_H

#include <stdint.h>

#define R36SX_POST_OVERLAY_CODE_PORT 0x080u
#define R36SX_POST_OVERLAY_SUBCODE_PORT 0x190u

void r36sx_post_overlay_rect(int frame_width,
                             int frame_height,
                             int *x,
                             int *y,
                             int *w,
                             int *h);

void r36sx_post_overlay_draw(uint16_t *target,
                             int frame_width,
                             int frame_height,
                             int stride_pixels,
                             uint8_t code_valid,
                             uint8_t code_value,
                             uint8_t subcode_valid,
                             uint8_t subcode_value);

#endif
