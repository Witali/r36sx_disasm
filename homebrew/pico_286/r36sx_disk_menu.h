#ifndef R36SX_DISK_MENU_H
#define R36SX_DISK_MENU_H

#include <stdint.h>

#define R36SX_DISK_MENU_RESULT_CLOSED 0x01u
#define R36SX_DISK_MENU_RESULT_EXIT_APP 0x02u

#define R36SX_DISK_MENU_MAX_IMAGES 32
#define R36SX_DISK_MENU_IMAGE_NAME_LEN 96

struct r36sx_disk_menu {
    uint8_t visible;
    uint8_t selected_row;
    uint8_t image_count;
    uint8_t selected_image[4];
    char images[R36SX_DISK_MENU_MAX_IMAGES][R36SX_DISK_MENU_IMAGE_NAME_LEN];
    char message[96];
};

void r36sx_disk_menu_init(struct r36sx_disk_menu *menu);
int r36sx_disk_menu_is_visible(const struct r36sx_disk_menu *menu);
void r36sx_disk_menu_set_visible(struct r36sx_disk_menu *menu, int visible);
uint32_t r36sx_disk_menu_handle_buttons(struct r36sx_disk_menu *menu,
                                        uint32_t pressed);
void r36sx_disk_menu_draw(const struct r36sx_disk_menu *menu,
                          uint16_t *frame,
                          int width,
                          int height,
                          int stride_pixels);

#endif
