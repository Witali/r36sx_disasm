#include "r36sx_disk_menu.h"

#include <ctype.h>
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../common/hardware.h"
#include "r36sx_port/r36sx_disk_config.h"

#define R36SX_DISK_MENU_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))
#define R36SX_DISK_MENU_DRIVE_COUNT 4
#define R36SX_DISK_MENU_ROW_BOOT_ORDER 4
#define R36SX_DISK_MENU_ROW_BIOS 5
#define R36SX_DISK_MENU_ROW_SAVE 6
#define R36SX_DISK_MENU_ROW_EXIT 7
#define R36SX_DISK_MENU_ROW_CANCEL 8
#define R36SX_DISK_MENU_ROW_COUNT 9

#define R36SX_DISK_BOOT_ORDER_AC 0u
#define R36SX_DISK_BOOT_ORDER_CA 1u
#define R36SX_DISK_BOOT_ORDER_ROM 2u

#define R36SX_DISK_BIOS_NORMAL 0u
#define R36SX_DISK_BIOS_TEST386 1u

extern uint8_t insertdisk(uint8_t drivenum, const char *pathname);

struct r36sx_disk_drive_info {
    uint8_t bios_drive;
    const char *config_key;
    const char *label;
    const char *fallback;
};

static const struct r36sx_disk_drive_info g_drives[] = {
    { 0, "fdd0", "FDD0 A:", "FreeDOS1.img" },
    { 1, "fdd1", "FDD1 B:", "sopwith.img" },
    { 128, "hdd0", "HDD0 C:", "hdd.img" },
    { 129, "hdd1", "HDD1 D:", "hdd2.img" },
};

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((uint16_t)(r & 0xf8u) << 8) |
                      ((uint16_t)(g & 0xfcu) << 3) |
                      ((uint16_t)b >> 3));
}

static void fill_rect(uint16_t *frame, int width, int height, int stride,
                      int x, int y, int w, int h, uint16_t color)
{
    if (w <= 0 || h <= 0 || !frame) {
        return;
    }
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > width) {
        w = width - x;
    }
    if (y + h > height) {
        h = height - y;
    }
    if (w <= 0 || h <= 0) {
        return;
    }

    for (int row = 0; row < h; row++) {
        uint16_t *dst = frame + (size_t)(y + row) * (size_t)stride +
                        (size_t)x;
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}

static void stroke_rect(uint16_t *frame, int width, int height, int stride,
                        int x, int y, int w, int h, uint16_t color)
{
    fill_rect(frame, width, height, stride, x, y, w, 1, color);
    fill_rect(frame, width, height, stride, x, y + h - 1, w, 1, color);
    fill_rect(frame, width, height, stride, x, y, 1, h, color);
    fill_rect(frame, width, height, stride, x + w - 1, y, 1, h, color);
}

static uint8_t glyph_row(char c, int row)
{
    static const uint8_t blank[7] = {0, 0, 0, 0, 0, 0, 0};
    static const uint8_t glyph_a[7] = {14, 17, 17, 31, 17, 17, 17};
    static const uint8_t glyph_b[7] = {30, 17, 17, 30, 17, 17, 30};
    static const uint8_t glyph_c[7] = {14, 17, 16, 16, 16, 17, 14};
    static const uint8_t glyph_d[7] = {30, 17, 17, 17, 17, 17, 30};
    static const uint8_t glyph_e[7] = {31, 16, 16, 30, 16, 16, 31};
    static const uint8_t glyph_f[7] = {31, 16, 16, 30, 16, 16, 16};
    static const uint8_t glyph_g[7] = {14, 17, 16, 23, 17, 17, 15};
    static const uint8_t glyph_h[7] = {17, 17, 17, 31, 17, 17, 17};
    static const uint8_t glyph_i[7] = {14, 4, 4, 4, 4, 4, 14};
    static const uint8_t glyph_j[7] = {1, 1, 1, 1, 17, 17, 14};
    static const uint8_t glyph_k[7] = {17, 18, 20, 24, 20, 18, 17};
    static const uint8_t glyph_l[7] = {16, 16, 16, 16, 16, 16, 31};
    static const uint8_t glyph_m[7] = {17, 27, 21, 21, 17, 17, 17};
    static const uint8_t glyph_n[7] = {17, 25, 21, 19, 17, 17, 17};
    static const uint8_t glyph_o[7] = {14, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_p[7] = {30, 17, 17, 30, 16, 16, 16};
    static const uint8_t glyph_q[7] = {14, 17, 17, 17, 21, 18, 13};
    static const uint8_t glyph_r[7] = {30, 17, 17, 30, 20, 18, 17};
    static const uint8_t glyph_s[7] = {15, 16, 16, 14, 1, 1, 30};
    static const uint8_t glyph_t[7] = {31, 4, 4, 4, 4, 4, 4};
    static const uint8_t glyph_u[7] = {17, 17, 17, 17, 17, 17, 14};
    static const uint8_t glyph_v[7] = {17, 17, 17, 17, 17, 10, 4};
    static const uint8_t glyph_w[7] = {17, 17, 17, 21, 21, 21, 10};
    static const uint8_t glyph_x[7] = {17, 17, 10, 4, 10, 17, 17};
    static const uint8_t glyph_y[7] = {17, 17, 10, 4, 4, 4, 4};
    static const uint8_t glyph_z[7] = {31, 1, 2, 4, 8, 16, 31};
    static const uint8_t glyph_0[7] = {14, 17, 19, 21, 25, 17, 14};
    static const uint8_t glyph_1[7] = {4, 12, 4, 4, 4, 4, 14};
    static const uint8_t glyph_2[7] = {14, 17, 1, 2, 4, 8, 31};
    static const uint8_t glyph_3[7] = {30, 1, 1, 14, 1, 1, 30};
    static const uint8_t glyph_4[7] = {2, 6, 10, 18, 31, 2, 2};
    static const uint8_t glyph_5[7] = {31, 16, 16, 30, 1, 1, 30};
    static const uint8_t glyph_6[7] = {14, 16, 16, 30, 17, 17, 14};
    static const uint8_t glyph_7[7] = {31, 1, 2, 4, 8, 8, 8};
    static const uint8_t glyph_8[7] = {14, 17, 17, 14, 17, 17, 14};
    static const uint8_t glyph_9[7] = {14, 17, 17, 15, 1, 1, 14};
    static const uint8_t glyph_colon[7] = {0, 4, 4, 0, 4, 4, 0};
    static const uint8_t glyph_period[7] = {0, 0, 0, 0, 0, 12, 12};
    static const uint8_t glyph_slash[7] = {1, 1, 2, 4, 8, 16, 16};
    static const uint8_t glyph_plus[7] = {0, 4, 4, 31, 4, 4, 0};
    static const uint8_t glyph_minus[7] = {0, 0, 0, 31, 0, 0, 0};
    const uint8_t *glyph = blank;

    if (c >= 'a' && c <= 'z') {
        c = (char)(c - ('a' - 'A'));
    }

    switch (c) {
    case 'A': glyph = glyph_a; break;
    case 'B': glyph = glyph_b; break;
    case 'C': glyph = glyph_c; break;
    case 'D': glyph = glyph_d; break;
    case 'E': glyph = glyph_e; break;
    case 'F': glyph = glyph_f; break;
    case 'G': glyph = glyph_g; break;
    case 'H': glyph = glyph_h; break;
    case 'I': glyph = glyph_i; break;
    case 'J': glyph = glyph_j; break;
    case 'K': glyph = glyph_k; break;
    case 'L': glyph = glyph_l; break;
    case 'M': glyph = glyph_m; break;
    case 'N': glyph = glyph_n; break;
    case 'O': glyph = glyph_o; break;
    case 'P': glyph = glyph_p; break;
    case 'Q': glyph = glyph_q; break;
    case 'R': glyph = glyph_r; break;
    case 'S': glyph = glyph_s; break;
    case 'T': glyph = glyph_t; break;
    case 'U': glyph = glyph_u; break;
    case 'V': glyph = glyph_v; break;
    case 'W': glyph = glyph_w; break;
    case 'X': glyph = glyph_x; break;
    case 'Y': glyph = glyph_y; break;
    case 'Z': glyph = glyph_z; break;
    case '0': glyph = glyph_0; break;
    case '1': glyph = glyph_1; break;
    case '2': glyph = glyph_2; break;
    case '3': glyph = glyph_3; break;
    case '4': glyph = glyph_4; break;
    case '5': glyph = glyph_5; break;
    case '6': glyph = glyph_6; break;
    case '7': glyph = glyph_7; break;
    case '8': glyph = glyph_8; break;
    case '9': glyph = glyph_9; break;
    case ':': glyph = glyph_colon; break;
    case '.': glyph = glyph_period; break;
    case '/': glyph = glyph_slash; break;
    case '+': glyph = glyph_plus; break;
    case '-': glyph = glyph_minus; break;
    default: break;
    }
    return glyph[row];
}

static int text_width(const char *text, int scale)
{
    int len = 0;
    while (text && text[len] != '\0') {
        len++;
    }
    return len > 0 ? (len * 6 - 1) * scale : 0;
}

static void draw_char(uint16_t *frame, int width, int height, int stride,
                      int x, int y, char c, uint16_t color, int scale)
{
    for (int row = 0; row < 7; row++) {
        uint8_t bits = glyph_row(c, row);
        for (int col = 0; col < 5; col++) {
            if ((bits & (uint8_t)(1u << (4 - col))) != 0) {
                fill_rect(frame, width, height, stride,
                          x + col * scale, y + row * scale,
                          scale, scale, color);
            }
        }
    }
}

static void draw_text(uint16_t *frame, int width, int height, int stride,
                      int x, int y, const char *text, uint16_t color,
                      int scale)
{
    for (int i = 0; text && text[i] != '\0'; i++) {
        draw_char(frame, width, height, stride, x + i * 6 * scale, y,
                  text[i], color, scale);
    }
}

static int ends_with_img(const char *name)
{
    size_t len = strlen(name);

    if (len < 5) {
        return 0;
    }
    name += len - 4;
    return tolower((unsigned char)name[0]) == '.' &&
           tolower((unsigned char)name[1]) == 'i' &&
           tolower((unsigned char)name[2]) == 'm' &&
           tolower((unsigned char)name[3]) == 'g';
}

static int image_index(const struct r36sx_disk_menu *menu, const char *name)
{
    for (int i = 0; i < menu->image_count; i++) {
        if (strcmp(menu->images[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static int add_image(struct r36sx_disk_menu *menu, const char *name)
{
    int pos;

    if (!name || !name[0]) {
        return -1;
    }
    pos = image_index(menu, name);
    if (pos >= 0) {
        return pos;
    }
    if (menu->image_count >= R36SX_DISK_MENU_MAX_IMAGES) {
        return -1;
    }
    snprintf(menu->images[menu->image_count],
             sizeof(menu->images[menu->image_count]), "%s", name);
    return menu->image_count++;
}

static void scan_image_dir(struct r36sx_disk_menu *menu, const char *dir,
                           const char *value_prefix)
{
    DIR *dp;

    dp = opendir(dir);
    if (!dp) {
        r36sx_pico286_debug_log("diskmenu: opendir failed '%s'", dir);
        return;
    }

    for (;;) {
        struct dirent *entry = readdir(dp);
        if (!entry) {
            break;
        }
        if (entry->d_name[0] == '.') {
            continue;
        }
        if (ends_with_img(entry->d_name)) {
            char value[R36SX_DISK_MENU_IMAGE_NAME_LEN];
            snprintf(value, sizeof(value), "%s%s",
                     value_prefix ? value_prefix : "", entry->d_name);
            add_image(menu, value);
        }
    }
    closedir(dp);
}

static void scan_images(struct r36sx_disk_menu *menu)
{
    const char *images_dir = r36sx_pico286_image_dir_path();

    for (size_t i = 0; i < R36SX_DISK_MENU_ARRAY_COUNT(g_drives); i++) {
        add_image(menu, r36sx_pico286_disk_value(
                       g_drives[i].bios_drive, g_drives[i].fallback));
    }

    if (images_dir && images_dir[0]) {
        scan_image_dir(menu, images_dir, "");
    }
}

static void refresh_menu(struct r36sx_disk_menu *menu)
{
    uint8_t order[4];
    uint8_t count;

    memset(menu->images, 0, sizeof(menu->images));
    menu->image_count = 0;
    scan_images(menu);

    for (size_t i = 0; i < R36SX_DISK_MENU_ARRAY_COUNT(g_drives); i++) {
        const char *current = r36sx_pico286_disk_value(
            g_drives[i].bios_drive, g_drives[i].fallback);
        int index = image_index(menu, current);
        if (index < 0) {
            index = add_image(menu, current);
        }
        menu->selected_image[i] = (uint8_t)(index >= 0 ? index : 0);
    }

    if (menu->image_count == 0) {
        add_image(menu, "hdd.img");
    }

    count = r36sx_pico286_boot_order(order, (uint8_t)sizeof(order));
    if (count == 0) {
        menu->boot_order_choice = R36SX_DISK_BOOT_ORDER_ROM;
    } else if (count >= 2 && order[0] == 128 && order[1] == 0) {
        menu->boot_order_choice = R36SX_DISK_BOOT_ORDER_CA;
    } else {
        menu->boot_order_choice = R36SX_DISK_BOOT_ORDER_AC;
    }
    menu->boot_order_changed = 0;

    menu->bios_choice =
        r36sx_pico286_bios_mode() == R36SX_PICO286_BIOS_TEST386 ?
        R36SX_DISK_BIOS_TEST386 : R36SX_DISK_BIOS_NORMAL;
    menu->bios_changed = 0;
}

static void cycle_image(struct r36sx_disk_menu *menu, int drive, int direction)
{
    int next;

    if (drive < 0 || drive >= R36SX_DISK_MENU_DRIVE_COUNT ||
        menu->image_count == 0) {
        return;
    }

    next = (int)menu->selected_image[drive] + direction;
    if (next < 0) {
        next = (int)menu->image_count - 1;
    } else if (next >= menu->image_count) {
        next = 0;
    }
    menu->selected_image[drive] = (uint8_t)next;
}

static void cycle_boot_order(struct r36sx_disk_menu *menu, int direction)
{
    if (!menu) {
        return;
    }

    if (menu->boot_order_choice == R36SX_DISK_BOOT_ORDER_ROM) {
        menu->boot_order_choice = direction < 0 ?
            R36SX_DISK_BOOT_ORDER_CA : R36SX_DISK_BOOT_ORDER_AC;
    } else {
        menu->boot_order_choice =
            menu->boot_order_choice == R36SX_DISK_BOOT_ORDER_AC ?
            R36SX_DISK_BOOT_ORDER_CA : R36SX_DISK_BOOT_ORDER_AC;
    }
    menu->boot_order_changed = 1;
}

static const char *boot_order_label(uint8_t choice)
{
    if (choice == R36SX_DISK_BOOT_ORDER_CA) {
        return "C,A";
    }
    if (choice == R36SX_DISK_BOOT_ORDER_ROM) {
        return "ROM";
    }
    return "A,C";
}

static const char *boot_order_config_value(uint8_t choice)
{
    return choice == R36SX_DISK_BOOT_ORDER_CA ? "hdd0,fdd0" : "fdd0,hdd0";
}

static void cycle_bios(struct r36sx_disk_menu *menu)
{
    if (!menu) {
        return;
    }
    menu->bios_choice = menu->bios_choice == R36SX_DISK_BIOS_TEST386 ?
        R36SX_DISK_BIOS_NORMAL : R36SX_DISK_BIOS_TEST386;
    menu->bios_changed = 1;
}

static const char *bios_label(uint8_t choice)
{
    return choice == R36SX_DISK_BIOS_TEST386 ? "TEST386" : "NORMAL";
}

static r36sx_pico286_bios_mode_t bios_config_value(uint8_t choice)
{
    return choice == R36SX_DISK_BIOS_TEST386 ?
        R36SX_PICO286_BIOS_TEST386 : R36SX_PICO286_BIOS_NORMAL;
}

static uint32_t apply_disk_bindings(struct r36sx_disk_menu *menu)
{
    int failures = 0;
    int bios_changed = menu->bios_changed;

    for (size_t i = 0; i < R36SX_DISK_MENU_ARRAY_COUNT(g_drives); i++) {
        const char *image = menu->images[menu->selected_image[i]];
        r36sx_pico286_set_disk_value(g_drives[i].bios_drive, image);
    }
    if (menu->boot_order_changed) {
        r36sx_pico286_set_boot_order_value(
            boot_order_config_value(menu->boot_order_choice));
    }
    if (bios_changed) {
        r36sx_pico286_set_bios_mode(bios_config_value(menu->bios_choice));
    }
    if (!r36sx_pico286_save_config()) {
        snprintf(menu->message, sizeof(menu->message), "SAVE FAILED");
        return 0;
    }

    for (size_t i = 0; i < R36SX_DISK_MENU_ARRAY_COUNT(g_drives); i++) {
        const char *path = r36sx_pico286_disk_path(g_drives[i].bios_drive, "");
        if (!path[0] || !insertdisk(g_drives[i].bios_drive, path)) {
            failures++;
        }
    }

    if (failures) {
        snprintf(menu->message, sizeof(menu->message),
                 "SAVED WITH %d ATTACH FAILS", failures);
    } else {
        snprintf(menu->message, sizeof(menu->message),
                 bios_changed ? "SAVED BIOS RESET" : "SAVED AND APPLIED");
    }
    menu->boot_order_changed = 0;
    menu->bios_changed = 0;
    return bios_changed ? R36SX_DISK_MENU_RESULT_RESET_PC : 0;
}

void r36sx_disk_menu_init(struct r36sx_disk_menu *menu)
{
    if (!menu) {
        return;
    }
    memset(menu, 0, sizeof(*menu));
    refresh_menu(menu);
}

int r36sx_disk_menu_is_visible(const struct r36sx_disk_menu *menu)
{
    return menu && menu->visible != 0;
}

void r36sx_disk_menu_set_visible(struct r36sx_disk_menu *menu, int visible)
{
    if (!menu) {
        return;
    }
    menu->visible = (uint8_t)(visible != 0);
    if (menu->visible) {
        menu->selected_row = 0;
        menu->message[0] = '\0';
        refresh_menu(menu);
    }
}

uint32_t r36sx_disk_menu_handle_buttons(struct r36sx_disk_menu *menu,
                                        uint32_t pressed)
{
    if (!menu || !menu->visible) {
        return 0;
    }

    if ((pressed & (R36SX_RKGAME_KEY_B | R36SX_RKGAME_KEY_X)) != 0) {
        r36sx_disk_menu_set_visible(menu, 0);
        return R36SX_DISK_MENU_RESULT_CLOSED;
    }
    if ((pressed & R36SX_RKGAME_KEY_UP) != 0) {
        menu->selected_row = menu->selected_row == 0 ?
            R36SX_DISK_MENU_ROW_COUNT - 1 : menu->selected_row - 1;
    }
    if ((pressed & R36SX_RKGAME_KEY_DOWN) != 0) {
        menu->selected_row = (uint8_t)((menu->selected_row + 1) %
                                       R36SX_DISK_MENU_ROW_COUNT);
    }
    if (menu->selected_row < R36SX_DISK_MENU_DRIVE_COUNT &&
        (pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
        cycle_image(menu, menu->selected_row, -1);
    } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BOOT_ORDER &&
               (pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
        cycle_boot_order(menu, -1);
    } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BIOS &&
               (pressed & R36SX_RKGAME_KEY_LEFT) != 0) {
        cycle_bios(menu);
    }
    if (menu->selected_row < R36SX_DISK_MENU_DRIVE_COUNT &&
        (pressed & R36SX_RKGAME_KEY_RIGHT) != 0) {
        cycle_image(menu, menu->selected_row, 1);
    } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BOOT_ORDER &&
               (pressed & R36SX_RKGAME_KEY_RIGHT) != 0) {
        cycle_boot_order(menu, 1);
    } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BIOS &&
               (pressed & R36SX_RKGAME_KEY_RIGHT) != 0) {
        cycle_bios(menu);
    }
    if ((pressed & (R36SX_RKGAME_KEY_A | R36SX_RKGAME_KEY_Y)) != 0) {
        if (menu->selected_row < R36SX_DISK_MENU_DRIVE_COUNT) {
            cycle_image(menu, menu->selected_row, 1);
        } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BOOT_ORDER) {
            cycle_boot_order(menu, 1);
        } else if (menu->selected_row == R36SX_DISK_MENU_ROW_BIOS) {
            cycle_bios(menu);
        } else if (menu->selected_row == R36SX_DISK_MENU_ROW_SAVE) {
            return apply_disk_bindings(menu);
        } else if (menu->selected_row == R36SX_DISK_MENU_ROW_EXIT) {
            return R36SX_DISK_MENU_RESULT_EXIT_APP;
        } else if (menu->selected_row == R36SX_DISK_MENU_ROW_CANCEL) {
            r36sx_disk_menu_set_visible(menu, 0);
            return R36SX_DISK_MENU_RESULT_CLOSED;
        }
    }
    return 0;
}

static void draw_row(const struct r36sx_disk_menu *menu,
                     uint16_t *frame,
                     int width,
                     int height,
                     int stride,
                     int row,
                     int x,
                     int y,
                     int w,
                     int h,
                     const char *text)
{
    int selected = row == menu->selected_row;
    uint16_t bg = selected ? rgb565(232, 174, 66) : rgb565(28, 34, 42);
    uint16_t fg = selected ? rgb565(15, 18, 20) : rgb565(232, 238, 226);
    uint16_t border = selected ? rgb565(255, 239, 158) : rgb565(74, 91, 108);

    fill_rect(frame, width, height, stride, x, y, w, h, bg);
    stroke_rect(frame, width, height, stride, x, y, w, h, border);
    draw_text(frame, width, height, stride, x + 10, y + 7, text, fg, 2);
}

void r36sx_disk_menu_draw(const struct r36sx_disk_menu *menu,
                          uint16_t *frame,
                          int width,
                          int height,
                          int stride_pixels)
{
    char line[160];
    int x = 36;
    int y = 88;
    int full_w = width - x * 2;
    const int row_h = 30;
    const int gap = 6;

    if (!r36sx_disk_menu_is_visible(menu) || !frame) {
        return;
    }

    fill_rect(frame, width, height, stride_pixels, 0, 0, width, height,
              rgb565(10, 13, 18));
    stroke_rect(frame, width, height, stride_pixels, 12, 12, width - 24,
                height - 24, rgb565(94, 118, 134));
    draw_text(frame, width, height, stride_pixels, 28, 28, "DISK MENU",
              rgb565(238, 236, 196), 3);
    draw_text(frame, width, height, stride_pixels, 28, 60,
              "LEFT/RIGHT CHANGE  A/Y CHANGE  X/B CANCEL  SAVE APPLIES",
              rgb565(180, 202, 208), 1);

    for (size_t i = 0; i < R36SX_DISK_MENU_ARRAY_COUNT(g_drives); i++) {
        const char *image = menu->image_count ?
            menu->images[menu->selected_image[i]] : "<NONE>";
        snprintf(line, sizeof(line), "%s  %s", g_drives[i].label, image);
        draw_row(menu, frame, width, height, stride_pixels, (int)i,
                 x, y, full_w, row_h, line);
        y += row_h + gap;
    }

    y += 10;
    snprintf(line, sizeof(line), "BOOT ORDER  %s",
             boot_order_label(menu->boot_order_choice));
    draw_row(menu, frame, width, height, stride_pixels,
             R36SX_DISK_MENU_ROW_BOOT_ORDER, x, y, full_w, row_h, line);
    y += row_h + gap;
    snprintf(line, sizeof(line), "BIOS  %s",
             bios_label(menu->bios_choice));
    draw_row(menu, frame, width, height, stride_pixels,
             R36SX_DISK_MENU_ROW_BIOS, x, y, full_w, row_h, line);
    y += row_h + gap;
    draw_row(menu, frame, width, height, stride_pixels,
             R36SX_DISK_MENU_ROW_SAVE, x, y, full_w, row_h, "SAVE/APPLY");
    y += row_h + gap;
    draw_row(menu, frame, width, height, stride_pixels,
             R36SX_DISK_MENU_ROW_EXIT, x, y, full_w, row_h, "EXIT APP");
    y += row_h + gap;
    draw_row(menu, frame, width, height, stride_pixels,
             R36SX_DISK_MENU_ROW_CANCEL, x, y, full_w, row_h, "CANCEL");

    if (menu->message[0]) {
        int msg_w = text_width(menu->message, 1);
        draw_text(frame, width, height, stride_pixels,
                  (width - msg_w) / 2, height - 42,
                  menu->message, rgb565(238, 236, 196), 1);
    } else {
        draw_text(frame, width, height, stride_pixels, 32, height - 42,
                  "FN+SELECT OPENS THIS MENU  FN+START OPENS KEY PRESETS",
                  rgb565(180, 202, 208), 1);
    }
}
