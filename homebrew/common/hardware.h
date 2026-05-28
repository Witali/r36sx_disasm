/*
 * Shared R36SX/SF3000-like hardware and firmware constants.
 *
 * Keep device-specific facts here so small homebrew programs do not each grow
 * their own slightly different copy of the same magic numbers.
 */

#ifndef R36SX_HARDWARE_H
#define R36SX_HARDWARE_H

/* Native framebuffer/video shape used by our tested homebrew modules. */
#define R36SX_SCREEN_WIDTH 640
#define R36SX_SCREEN_HEIGHT 480
#define R36SX_SCREEN_MODE_TEXT "640x480"
#define R36SX_RGB565_BITS_PER_PIXEL 16
#define R36SX_RGB565_BYTES_PER_PIXEL 2
#define R36SX_RGB565_FRAME_STRIDE \
    (R36SX_SCREEN_WIDTH * R36SX_RGB565_BYTES_PER_PIXEL)

/* Common runtime paths on the SD-card firmware. */
#define R36SX_CUBEGM_DIR "/mnt/sdcard/cubegm"
#define R36SX_MIPS_NATIVE_DIR "/mnt/sdcard/MIPS_NATIVE"
#define R36SX_MIPS_NATIVE_COMMON_DIR "/mnt/sdcard/MIPS_NATIVE/common"
#define R36SX_MIPS_NATIVE_COMMON_FONTS_DIR "/mnt/sdcard/MIPS_NATIVE/common/fonts"
#define R36SX_DEFAULT_MONO_FONT_PATH \
    "/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf"
#define R36SX_DRIVER_SO_PATH "/mnt/sdcard/cubegm/driver.so"
#define R36SX_DRIVER_SO_LOCAL_PATH "./driver.so"
#define R36SX_DRIVER_SO_NAME "driver.so"
#define R36SX_RKGAME_PATH "/mnt/sdcard/cubegm/rkgame"
#define R36SX_RKGAME_STOCK_PATH "/mnt/sdcard/cubegm/rkgame.stock"
#define R36SX_FB_DEVICE_PATH "/dev/fb0"
#define R36SX_DIS_DEVICE_PATH "/dev/dis"
#define R36SX_JOY_KEY_SHM_PATH "/tmp/joy_key"
#define R36SX_GAME_STATUS_SHM_PATH "/tmp/cubegame_status"
#define R36SX_ICUBE_PATH "/mnt/sdcard/cubegm/icube"

/*
 * Stock rkgame calls video_driver_setting() with these five words before
 * video_drivers_init(). Their exact semantic names are still vendor-specific,
 * but values 3 and 4 are the observed 0x356 x 0x1e0 display setup.
 */
#define R36SX_DRIVER_SETTING_0 1
#define R36SX_DRIVER_SETTING_1 1
#define R36SX_DRIVER_SETTING_2 1
#define R36SX_DRIVER_SETTING_WIDTH 0x356
#define R36SX_DRIVER_SETTING_HEIGHT 0x1e0

/* Known driver.so cube_ioctl commands from rkgame/driver.so analysis. */
#define R36SX_CUBE_IOCTL_SET_BACKLIGHT 0x20010201u
#define R36SX_CUBE_IOCTL_SET_VOLUME 0x20010203u
#define R36SX_CUBE_IOCTL_GET_VOLUME 0x40010204u
#define R36SX_CUBE_IOCTL_SET_ASPECT_RATIO 0x20010205u
#define R36SX_CUBE_IOCTL_GET_BACKLIGHT 0x40050202u
#define R36SX_CUBE_IOCTL_GET_JOY_KEY 0x40050207u
#define R36SX_CUBE_IOCTL_GET_GAME_STATUS 0x40050208u
#define R36SX_CUBE_IOCTL_GET_JOY_KEY_PTR 0x40050209u

/* Raw joy_key bit masks used by stock rkgame. */
#define R36SX_RKGAME_KEY_SELECT 0x00000001u
#define R36SX_RKGAME_KEY_L3 0x00000002u
#define R36SX_RKGAME_KEY_R3 0x00000004u
#define R36SX_RKGAME_KEY_START 0x00000008u
#define R36SX_RKGAME_KEY_UP 0x00000010u
#define R36SX_RKGAME_KEY_RIGHT 0x00000020u
#define R36SX_RKGAME_KEY_DOWN 0x00000040u
#define R36SX_RKGAME_KEY_LEFT 0x00000080u
#define R36SX_RKGAME_KEY_L2 0x00000100u
#define R36SX_RKGAME_KEY_R2 0x00000200u
#define R36SX_RKGAME_KEY_L 0x00000400u
#define R36SX_RKGAME_KEY_R 0x00000800u
#define R36SX_RKGAME_KEY_X 0x00001000u
#define R36SX_RKGAME_KEY_A 0x00002000u
#define R36SX_RKGAME_KEY_B 0x00004000u
#define R36SX_RKGAME_KEY_Y 0x00008000u
#define R36SX_RKGAME_KEY_FN 0x00010000u

/*
 * icube supervises /mnt/sdcard/cubegm/rkgame through this SysV shared-memory
 * heartbeat. Linux IPC_CREAT is 01000; using the literal keeps this header
 * usable in tiny no-libc libretro cores that should not include <sys/ipc.h>.
 */
#define R36SX_SYSV_IPC_CREAT 01000
#define R36SX_ICUBE_HEARTBEAT_KEY 0x4d2
#define R36SX_ICUBE_HEARTBEAT_SIZE 0x1c4
#define R36SX_ICUBE_HEARTBEAT_PERMISSIONS 0666
#define R36SX_ICUBE_HEARTBEAT_CREATE_FLAGS \
    (R36SX_SYSV_IPC_CREAT | R36SX_ICUBE_HEARTBEAT_PERMISSIONS)
#define R36SX_ICUBE_HEARTBEAT_ALIVE_WORD 0
#define R36SX_ICUBE_HEARTBEAT_COUNTER_WORD 1
#define R36SX_ICUBE_INITIAL_GRACE_SECONDS 6
#define R36SX_ICUBE_CHECK_INTERVAL_SECONDS 1
#define R36SX_RKGAME_HEARTBEAT_USEC 20000

#endif
