/*
 * fb-test: direct framebuffer/GE probe for the R36SX stock Linux firmware.
 *
 * This program intentionally does not use driver.so. It repeats the safe,
 * observable parts of driver.so's display path:
 *
 *   /dev/dis probe -> /dev/ge probe -> /dev/fb0 mmap -> draw RGB test pattern
 *
 * It does not feed GE commands yet. The GE command queue format is still
 * vendor-specific, so this test only opens/maps/probes /dev/ge.
 */

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __mips__
#include <sys/cachectl.h>
#endif

#define FBTEST_DIS_DEVICE "/dev/dis"
#define FBTEST_GE_DEVICE "/dev/ge"
#define FBTEST_FB_DEVICE "/dev/fb0"

#define FBTEST_DIS_GET_SCREEN_INFO 0xc00c0e0cUL
#define FBTEST_GE_GET_CMDQ_INFO 0x20002305UL
#define FBTEST_FB_VENDOR_UPDATE 0x80044620UL

#define FBTEST_DEFAULT_HOLD_SECONDS 8
#define FBTEST_MAX_GE_CMDQ_SIZE (16U * 1024U * 1024U)

struct fbtest_options {
    int hold_seconds;
    int probe_ge;
    int use_next_page;
    int restore_old_page;
};

struct fbtest_ge_cmdq_info {
    uint32_t base;
    uint32_t size;
};

static void print_usage(const char *argv0)
{
    fprintf(stderr,
            "Usage: %s [--hold SECONDS] [--no-ge] [--no-pan] [--restore]\n"
            "\n"
            "Draw a direct /dev/fb0 RGB test pattern and print /dev/dis,\n"
            "/dev/ge, and framebuffer diagnostics.\n"
            "\n"
            "  --hold SECONDS  keep the pattern visible before exit, default %d\n"
            "  --no-ge         skip /dev/ge probe\n"
            "  --no-pan        draw into the currently visible page only\n"
            "  --restore       pan back to the old page before exit when panning\n",
            argv0, FBTEST_DEFAULT_HOLD_SECONDS);
}

static int parse_int_arg(const char *value, int min_value, int max_value)
{
    char *end = NULL;
    long parsed;

    errno = 0;
    parsed = strtol(value, &end, 10);
    if (errno != 0 || end == value || *end != '\0' ||
        parsed < min_value || parsed > max_value) {
        return -1;
    }
    return (int)parsed;
}

static int parse_options(int argc, char **argv, struct fbtest_options *opts)
{
    int i;

    opts->hold_seconds = FBTEST_DEFAULT_HOLD_SECONDS;
    opts->probe_ge = 1;
    opts->use_next_page = 1;
    opts->restore_old_page = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--hold") == 0) {
            int parsed;
            if (i + 1 >= argc) {
                print_usage(argv[0]);
                return -1;
            }
            parsed = parse_int_arg(argv[++i], 0, 3600);
            if (parsed < 0) {
                fprintf(stderr, "fb-test: invalid --hold value: %s\n", argv[i]);
                return -1;
            }
            opts->hold_seconds = parsed;
        } else if (strcmp(argv[i], "--no-ge") == 0) {
            opts->probe_ge = 0;
        } else if (strcmp(argv[i], "--no-pan") == 0) {
            opts->use_next_page = 0;
        } else if (strcmp(argv[i], "--restore") == 0) {
            opts->restore_old_page = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else {
            fprintf(stderr, "fb-test: unknown argument: %s\n", argv[i]);
            print_usage(argv[0]);
            return -1;
        }
    }

    return 0;
}

static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)((uint16_t)(r >> 3) << 11 |
                      (uint16_t)(g >> 2) << 5 |
                      (uint16_t)(b >> 3));
}

static void flush_cache_if_possible(void *addr, size_t len)
{
#ifdef __mips__
    if (addr != NULL && len != 0) {
        (void)cacheflush(addr, (int)len, BCACHE);
    }
#else
    (void)addr;
    (void)len;
#endif
}

static void sleep_seconds(int seconds)
{
    struct timespec req;

    if (seconds <= 0) {
        return;
    }
    req.tv_sec = seconds;
    req.tv_nsec = 0;
    while (nanosleep(&req, &req) != 0 && errno == EINTR) {
        ;
    }
}

static void probe_dis(void)
{
    int fd;
    uint32_t info[3] = { 1, 0, 0 };

    fd = open(FBTEST_DIS_DEVICE, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "fb-test: open %s failed: %s\n",
                FBTEST_DIS_DEVICE, strerror(errno));
        return;
    }

    if (ioctl(fd, FBTEST_DIS_GET_SCREEN_INFO, info) == 0) {
        uint32_t packed = info[2];
        unsigned width = packed & 0xffffU;
        unsigned height = (packed >> 16) & 0xffffU;
        printf("fb-test: /dev/dis screen info raw={0x%08x,0x%08x,0x%08x} "
               "screen=%ux%u\n",
               info[0], info[1], info[2], width, height);
    } else {
        fprintf(stderr, "fb-test: /dev/dis ioctl 0x%08lx failed: %s\n",
                FBTEST_DIS_GET_SCREEN_INFO, strerror(errno));
    }

    close(fd);
}

static void probe_ge(void)
{
    int fd;
    void *regs = MAP_FAILED;
    void *cmdq = MAP_FAILED;
    struct fbtest_ge_cmdq_info info;

    fd = open(FBTEST_GE_DEVICE, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "fb-test: open %s failed: %s\n",
                FBTEST_GE_DEVICE, strerror(errno));
        return;
    }

    regs = mmap(NULL, 0x2000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (regs == MAP_FAILED) {
        fprintf(stderr, "fb-test: mmap /dev/ge registers failed: %s\n",
                strerror(errno));
    } else {
        volatile uint32_t *words = (volatile uint32_t *)regs;
        printf("fb-test: /dev/ge regs mapped, word[0]=0x%08x word[0x2b]=0x%08x\n",
               words[0], words[0x2b]);
    }

    memset(&info, 0, sizeof(info));
    if (ioctl(fd, FBTEST_GE_GET_CMDQ_INFO, &info) == 0) {
        printf("fb-test: /dev/ge cmdq info base=0x%08x size=%u\n",
               info.base, info.size);
        if (info.size > 0 && info.size <= FBTEST_MAX_GE_CMDQ_SIZE) {
            cmdq = mmap(NULL, info.size, PROT_READ | PROT_WRITE,
                        MAP_SHARED, fd, 0x2000);
            if (cmdq == MAP_FAILED) {
                fprintf(stderr, "fb-test: mmap /dev/ge cmdq failed: %s\n",
                        strerror(errno));
            } else {
                printf("fb-test: /dev/ge cmdq mapped at %p\n", cmdq);
            }
        } else {
            printf("fb-test: /dev/ge cmdq size skipped as unsafe\n");
        }
    } else {
        fprintf(stderr, "fb-test: /dev/ge ioctl 0x%08lx failed: %s\n",
                FBTEST_GE_GET_CMDQ_INFO, strerror(errno));
    }

    if (cmdq != MAP_FAILED) {
        munmap(cmdq, info.size);
    }
    if (regs != MAP_FAILED) {
        munmap(regs, 0x2000);
    }
    close(fd);
}

static void put_pixel(uint8_t *row, unsigned x, unsigned bpp,
                      uint8_t r, uint8_t g, uint8_t b)
{
    if (bpp == 16) {
        uint16_t *dst = (uint16_t *)(row + x * 2U);
        *dst = rgb565(r, g, b);
    } else if (bpp == 24) {
        uint8_t *dst = row + x * 3U;
        dst[0] = b;
        dst[1] = g;
        dst[2] = r;
    } else if (bpp == 32) {
        uint32_t *dst = (uint32_t *)(row + x * 4U);
        *dst = 0xff000000U | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }
}

static void draw_pattern(uint8_t *base, size_t map_len,
                         const struct fb_fix_screeninfo *fix,
                         const struct fb_var_screeninfo *var,
                         unsigned yoffset)
{
    unsigned x;
    unsigned y;
    unsigned width = var->xres;
    unsigned height = var->yres;
    unsigned bpp = var->bits_per_pixel;
    size_t page_offset = (size_t)yoffset * fix->line_length +
                         (size_t)var->xoffset * ((size_t)bpp / 8U);
    size_t page_len = (size_t)fix->line_length * height;

    if (page_offset >= map_len || page_len > map_len - page_offset) {
        fprintf(stderr, "fb-test: visible page is outside mmap region\n");
        return;
    }

    memset(base + page_offset, 0, page_len);

    for (y = 0; y < height; y++) {
        uint8_t *row = base + page_offset + (size_t)y * fix->line_length;
        for (x = 0; x < width; x++) {
            uint8_t r;
            uint8_t g;
            uint8_t b;

            if (y < height / 8U) {
                r = 255;
                g = 255;
                b = 255;
            } else if (y < height / 4U) {
                r = 255;
                g = 0;
                b = 0;
            } else if (y < height * 3U / 8U) {
                r = 0;
                g = 255;
                b = 0;
            } else if (y < height / 2U) {
                r = 0;
                g = 0;
                b = 255;
            } else {
                r = (uint8_t)((x * 255U) / (width ? width : 1U));
                g = (uint8_t)((y * 255U) / (height ? height : 1U));
                b = (uint8_t)(((x + y) * 255U) /
                              ((width + height) ? (width + height) : 1U));
            }

            if (x < 4 || y < 4 || x + 4 >= width || y + 4 >= height) {
                r = 255;
                g = 255;
                b = 0;
            }

            put_pixel(row, x, bpp, r, g, b);
        }
    }

    flush_cache_if_possible(base + page_offset, page_len);
}

static int fb_draw_test(const struct fbtest_options *opts)
{
    int fd;
    void *map = MAP_FAILED;
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;
    struct fb_var_screeninfo old_var;
    size_t map_len;
    unsigned target_yoffset;
    uint32_t vsync_arg[2] = { 0, 0 };

    fd = open(FBTEST_FB_DEVICE, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "fb-test: open %s failed: %s\n",
                FBTEST_FB_DEVICE, strerror(errno));
        return 1;
    }

    memset(&fix, 0, sizeof(fix));
    memset(&var, 0, sizeof(var));
    if (ioctl(fd, FBIOGET_FSCREENINFO, &fix) != 0) {
        fprintf(stderr, "fb-test: FBIOGET_FSCREENINFO failed: %s\n",
                strerror(errno));
        close(fd);
        return 1;
    }
    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) != 0) {
        fprintf(stderr, "fb-test: FBIOGET_VSCREENINFO failed: %s\n",
                strerror(errno));
        close(fd);
        return 1;
    }
    old_var = var;

    printf("fb-test: fb id='%s' smem_len=%u line_length=%u\n",
           fix.id, fix.smem_len, fix.line_length);
    printf("fb-test: var xres=%u yres=%u xres_virtual=%u yres_virtual=%u "
           "xoffset=%u yoffset=%u bpp=%u\n",
           var.xres, var.yres, var.xres_virtual, var.yres_virtual,
           var.xoffset, var.yoffset, var.bits_per_pixel);

    if (var.bits_per_pixel != 16 && var.bits_per_pixel != 24 &&
        var.bits_per_pixel != 32) {
        fprintf(stderr, "fb-test: unsupported bpp=%u\n", var.bits_per_pixel);
        close(fd);
        return 1;
    }

    map_len = fix.smem_len;
    if (map_len == 0) {
        map_len = (size_t)fix.line_length * var.yres_virtual;
    }
    if (map_len == 0) {
        fprintf(stderr, "fb-test: framebuffer map length is zero\n");
        close(fd);
        return 1;
    }

    map = mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        fprintf(stderr, "fb-test: mmap %s failed: %s\n",
                FBTEST_FB_DEVICE, strerror(errno));
        close(fd);
        return 1;
    }

    target_yoffset = var.yoffset;
    if (opts->use_next_page && var.yres != 0 &&
        var.yres_virtual >= var.yres * 2U) {
        target_yoffset = (var.yoffset == 0) ? var.yres : 0;
    }

    printf("fb-test: drawing pattern at yoffset=%u\n", target_yoffset);
    draw_pattern((uint8_t *)map, map_len, &fix, &var, target_yoffset);
    msync(map, map_len, MS_SYNC);

    if (ioctl(fd, FBIOBLANK, FB_BLANK_UNBLANK) != 0) {
        fprintf(stderr, "fb-test: FBIOBLANK unblank failed: %s\n",
                strerror(errno));
    }

    if (target_yoffset != var.yoffset) {
        var.yoffset = target_yoffset;
        if (ioctl(fd, FBIOPAN_DISPLAY, &var) != 0) {
            fprintf(stderr, "fb-test: FBIOPAN_DISPLAY failed: %s\n",
                    strerror(errno));
        } else {
            printf("fb-test: panned display to yoffset=%u\n", var.yoffset);
        }
    } else {
        if (ioctl(fd, FBIOPAN_DISPLAY, &var) != 0) {
            fprintf(stderr, "fb-test: FBIOPAN_DISPLAY current page failed: %s\n",
                    strerror(errno));
        }
    }

    if (ioctl(fd, FBTEST_FB_VENDOR_UPDATE, vsync_arg) != 0) {
        fprintf(stderr, "fb-test: vendor update ioctl 0x%08lx failed: %s\n",
                FBTEST_FB_VENDOR_UPDATE, strerror(errno));
    }

    printf("fb-test: pattern visible for %d seconds\n", opts->hold_seconds);
    sleep_seconds(opts->hold_seconds);

    if (opts->restore_old_page && target_yoffset != old_var.yoffset) {
        if (ioctl(fd, FBIOPAN_DISPLAY, &old_var) != 0) {
            fprintf(stderr, "fb-test: restore FBIOPAN_DISPLAY failed: %s\n",
                    strerror(errno));
        } else {
            printf("fb-test: restored yoffset=%u\n", old_var.yoffset);
        }
    }

    munmap(map, map_len);
    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    struct fbtest_options opts;

    if (parse_options(argc, argv, &opts) != 0) {
        return 2;
    }

    printf("fb-test: direct framebuffer test starting\n");
    probe_dis();
    if (opts.probe_ge) {
        probe_ge();
    }

    return fb_draw_test(&opts);
}

