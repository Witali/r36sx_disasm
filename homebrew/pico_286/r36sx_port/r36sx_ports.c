#pragma GCC optimize("Ofast")
#include <errno.h>
#include <stdio.h>
#include <string.h>
#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include <time.h>
#include "emulator.h"
#include "r36sx_cpu.h"
#include "r36sx_debug_config.h"
#include "r36sx_disk_config.h"
#if PICO_ON_DEVICE
#include <74hc595.h>
#include <hardware/pwm.h>
extern int16_t keyboard_send(uint8_t data);
#include "nespad.h"
#endif

#include <emu8950.h>
OPL *emu8950_opl;
#include "audio/sn76489.c.inl"
#include "audio/cms.c.inl"
#include "audio/dss.c.inl"
#include "audio/mpu401.c.inl"
#include "audio/sound_blaster.c.inl"
#include "i8237.c.inl"
#include "r36sx_host_rpc.c.inl"

#define R36SX_HOSTDRV_TRACE_PORT (R36SX_HOST_RPC_PORT_BASE + 0x0Au)
#define R36SX_EMERGENCY_DUMP_PORT (R36SX_HOST_RPC_PORT_BASE + 0x0Bu)

uint8_t crt_controller_idx, crt_controller[32];
uint8_t port60, port61, port64;
uint8_t cursor_start = 12, cursor_end = 13;
uint32_t vram_offset = 0x0;

int sound_chips_clock = 0;
uint32_t r36sx_sound_frequency = 44100u;

static int audio_flags_loaded;
static int audio_adlib_enabled = 1;
static int audio_cms_enabled = 1;
static int audio_sn76489_enabled = 1;
static int audio_mpu401_enabled = 1;
static int audio_covox_enabled = 1;

static volatile int r36sx_emergency_dump_requested;
static volatile int r36sx_emergency_stop_requested;
static uint8_t r36sx_emergency_dump_code;
static char r36sx_emergency_dump_reason[64];
static unsigned r36sx_emergency_dump_sequence;

extern uint8_t r36sx_cpu_is_protected_enabled(void);

static const char *r36sx_debug_segment_name(unsigned index)
{
    static const char *names[R36SX_CPU_DEBUG_SEGMENT_COUNT] = {
        "es", "cs", "ss", "ds", "fs", "gs"
    };

    return index < R36SX_CPU_DEBUG_SEGMENT_COUNT ? names[index] : "unknown";
}

static void r36sx_emergency_dump_descriptor_cache(
    FILE *fp,
    const char *name,
    const r36sx_cpu_debug_segment_cache_t *cache)
{
    uint8_t type = cache->access & 0x0fu;
    uint8_t system = (cache->access & 0x10u) ? 1u : 0u;
    uint8_t dpl = (cache->access >> 5) & 3u;
    uint8_t present = (cache->access >> 7) & 1u;
    uint8_t avl = cache->flags & 1u;
    uint8_t long_mode = (cache->flags >> 1) & 1u;
    uint8_t db = (cache->flags >> 2) & 1u;
    uint8_t granular = (cache->flags >> 3) & 1u;

    fprintf(fp,
            "%s selector=%04x base=%08lx limit=%08lx access=%02x flags=%x "
            "valid=%u type=%x s=%u dpl=%u p=%u avl=%u l=%u db=%u g=%u\n",
            name,
            (unsigned)cache->selector,
            (unsigned long)cache->base,
            (unsigned long)cache->limit,
            (unsigned)cache->access,
            (unsigned)cache->flags,
            (unsigned)cache->valid,
            (unsigned)type,
            (unsigned)system,
            (unsigned)dpl,
            (unsigned)present,
            (unsigned)avl,
            (unsigned)long_mode,
            (unsigned)db,
            (unsigned)granular);
}

static int r36sx_emergency_mkdir(const char *path)
{
#if defined(_WIN32)
    if (_mkdir(path) == 0 || errno == EEXIST) {
        return 1;
    }
#else
    if (mkdir(path, 0777) == 0 || errno == EEXIST) {
        return 1;
    }
#endif
    return 0;
}

static int r36sx_emergency_write_file(const char *path,
                                      const void *data,
                                      size_t bytes)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        r36sx_pico286_debug_log("emergency_dump: open failed path='%s' errno=%d",
                                path, errno);
        return 0;
    }
    if (bytes != 0 && fwrite(data, 1, bytes, fp) != bytes) {
        r36sx_pico286_debug_log("emergency_dump: write failed path='%s' errno=%d",
                                path, errno);
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

static uint8_t r36sx_videoram_byte(uint32_t address)
{
    return (uint8_t)(VIDEORAM[(address - VIDEORAM_START) & 0xFFFFu] & 0xFFu);
}

static void r36sx_emergency_dump_text_screen(const char *path)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        r36sx_pico286_debug_log("emergency_dump: open failed path='%s' errno=%d",
                                path, errno);
        return;
    }
    for (unsigned row = 0; row < 25u; row++) {
        for (unsigned col = 0; col < 80u; col++) {
            uint32_t addr = 0xB8000u + ((row * 80u + col) * 2u);
            uint8_t ch = r36sx_videoram_byte(addr);
            if (ch < 0x20u || ch > 0x7Eu) {
                ch = ' ';
            }
            fputc((int)ch, fp);
        }
        fputc('\n', fp);
    }
    fclose(fp);
}

static void r36sx_emergency_dump_registers(const char *path)
{
    FILE *fp = fopen(path, "wb");
    r36sx_cpu_debug_snapshot_t cpu;
    if (!fp) {
        r36sx_pico286_debug_log("emergency_dump: open failed path='%s' errno=%d",
                                path, errno);
        return;
    }

    r36sx_cpu_debug_snapshot(&cpu);
    fprintf(fp, "reason=%s\n", r36sx_emergency_dump_reason);
    fprintf(fp, "code=%u\n", (unsigned)r36sx_emergency_dump_code);
    fprintf(fp, "cpu_protected=%u\n",
            (unsigned)r36sx_cpu_is_protected_enabled());
    fprintf(fp, "videomode=0x%02x\n", (unsigned)videomode);
    fprintf(fp, "ip=%04x ip32=%08lx\n",
            (unsigned)CPU_IP,
            (unsigned long)ip32);
    fprintf(fp, "flags=%08lx CF=%u PF=%u AF=%u ZF=%u SF=%u TF=%u IF=%u DF=%u OF=%u\n",
            (unsigned long)x86_flags.value,
            (unsigned)CPU_FL_CF,
            (unsigned)CPU_FL_PF,
            (unsigned)CPU_FL_AF,
            (unsigned)CPU_FL_ZF,
            (unsigned)CPU_FL_SF,
            (unsigned)CPU_FL_TF,
            (unsigned)CPU_FL_IFL,
            (unsigned)CPU_FL_DF,
            (unsigned)CPU_FL_OF);
    fprintf(fp, "eax=%08lx ebx=%08lx ecx=%08lx edx=%08lx\n",
            (unsigned long)CPU_EAX,
            (unsigned long)CPU_EBX,
            (unsigned long)CPU_ECX,
            (unsigned long)CPU_EDX);
    fprintf(fp, "esi=%08lx edi=%08lx ebp=%08lx esp=%08lx\n",
            (unsigned long)CPU_ESI,
            (unsigned long)CPU_EDI,
            (unsigned long)CPU_EBP,
            (unsigned long)CPU_ESP);
    fprintf(fp, "ax=%04x bx=%04x cx=%04x dx=%04x si=%04x di=%04x bp=%04x sp=%04x\n",
            (unsigned)CPU_AX,
            (unsigned)CPU_BX,
            (unsigned)CPU_CX,
            (unsigned)CPU_DX,
            (unsigned)CPU_SI,
            (unsigned)CPU_DI,
            (unsigned)CPU_BP,
            (unsigned)CPU_SP);
    fprintf(fp, "cs=%04x ds=%04x es=%04x ss=%04x fs=%04x gs=%04x\n",
            (unsigned)CPU_CS,
            (unsigned)CPU_DS,
            (unsigned)CPU_ES,
            (unsigned)CPU_SS,
            (unsigned)CPU_FS,
            (unsigned)CPU_GS);
    fprintf(fp, "a20=%d xms_bytes=%lu hma_bytes=%lu\n",
            a20_enabled,
            (unsigned long)xms_configured_memory_bytes(),
            (unsigned long)HMA_SIZE);
    fprintf(fp, "linear_cs_ip=%05lx linear_ss_sp=%05lx\n",
            (unsigned long)(((uint32_t)CPU_CS << 4) + (uint16_t)CPU_IP),
            (unsigned long)(((uint32_t)CPU_SS << 4) + CPU_SP));
    fprintf(fp, "cpu_modes protected=%u native_protected=%u vm86=%u cpl=%u iopl=%u\n",
            (unsigned)cpu.protected_mode,
            (unsigned)cpu.native_protected_mode,
            (unsigned)cpu.vm86_mode,
            (unsigned)cpu.cpl,
            (unsigned)cpu.iopl);
    fprintf(fp, "control_registers cr0=%08lx cr2=%08lx cr3=%08lx\n",
            (unsigned long)cpu.cr0,
            (unsigned long)cpu.cr2,
            (unsigned long)cpu.cr3);
    fprintf(fp,
            "debug_registers dr0=%08lx dr1=%08lx dr2=%08lx dr3=%08lx "
            "dr4=%08lx dr5=%08lx dr6=%08lx dr7=%08lx\n",
            (unsigned long)cpu.debug_regs[0],
            (unsigned long)cpu.debug_regs[1],
            (unsigned long)cpu.debug_regs[2],
            (unsigned long)cpu.debug_regs[3],
            (unsigned long)cpu.debug_regs[4],
            (unsigned long)cpu.debug_regs[5],
            (unsigned long)cpu.debug_regs[6],
            (unsigned long)cpu.debug_regs[7]);
    fprintf(fp,
            "test_registers tr0=%08lx tr1=%08lx tr2=%08lx tr3=%08lx "
            "tr4=%08lx tr5=%08lx tr6=%08lx tr7=%08lx\n",
            (unsigned long)cpu.test_regs[0],
            (unsigned long)cpu.test_regs[1],
            (unsigned long)cpu.test_regs[2],
            (unsigned long)cpu.test_regs[3],
            (unsigned long)cpu.test_regs[4],
            (unsigned long)cpu.test_regs[5],
            (unsigned long)cpu.test_regs[6],
            (unsigned long)cpu.test_regs[7]);
    fprintf(fp,
            "descriptor_tables gdtr_base=%08lx gdtr_limit=%04x "
            "idtr_base=%08lx idtr_limit=%04x ldtr=%04x tr=%04x\n",
            (unsigned long)cpu.gdtr_base,
            (unsigned)cpu.gdtr_limit,
            (unsigned long)cpu.idtr_base,
            (unsigned)cpu.idtr_limit,
            (unsigned)cpu.ldtr_selector,
            (unsigned)cpu.tr_selector);
    for (unsigned i = 0; i < R36SX_CPU_DEBUG_SEGMENT_COUNT; i++) {
        char label[32];
        const char *name = r36sx_debug_segment_name(i);
        fprintf(fp,
                "segment_%s value=%08lx selector_cache=%04x base_cache=%08lx\n",
                name,
                (unsigned long)cpu.segment_values[i],
                (unsigned)cpu.segment_selectors[i],
                (unsigned long)cpu.segment_bases[i]);
        snprintf(label, sizeof(label), "descriptor_cache_%s", name);
        r36sx_emergency_dump_descriptor_cache(
            fp, label, &cpu.segment_cache[i]);
    }
    r36sx_emergency_dump_descriptor_cache(
        fp, "descriptor_cache_ldtr", &cpu.ldtr_cache);
    r36sx_emergency_dump_descriptor_cache(
        fp, "descriptor_cache_tr", &cpu.tr_cache);
    fclose(fp);
}

static void r36sx_emergency_dump_request_common(uint8_t code,
                                                const char *reason,
                                                int stop)
{
    r36sx_emergency_dump_code = code;
    if (!reason || !reason[0]) {
        reason = "guest";
    }
    snprintf(r36sx_emergency_dump_reason,
             sizeof(r36sx_emergency_dump_reason),
             "%s", reason);
    r36sx_emergency_dump_requested = 1;
    if (stop) {
        r36sx_emergency_stop_requested = 1;
    }
    r36sx_pico286_debug_log("memory_dump: requested code=%u stop=%d reason='%s'",
                            (unsigned)code,
                            stop,
                            r36sx_emergency_dump_reason);
}

void r36sx_emergency_dump_request(uint8_t code, const char *reason)
{
    r36sx_emergency_dump_request_common(code, reason, 1);
}

void r36sx_memory_dump_request(uint8_t code, const char *reason)
{
    r36sx_emergency_dump_request_common(code, reason, 0);
}

int r36sx_emergency_dump_pending(void)
{
    return r36sx_emergency_stop_requested != 0;
}

int r36sx_memory_dump_pending(void)
{
    return r36sx_emergency_dump_requested != 0;
}

void r36sx_emergency_dump_write_and_clear(void)
{
    char dir[96];
    char path[160];
    int stop = r36sx_emergency_stop_requested != 0;
    const char *prefix = stop ? "emergency_dump" : "memory_dump";

    if (!r36sx_emergency_dump_requested) {
        return;
    }
    r36sx_emergency_dump_requested = 0;
    r36sx_emergency_dump_sequence++;
    snprintf(dir, sizeof(dir), "%s_%03u", prefix,
             r36sx_emergency_dump_sequence);
    if (!r36sx_emergency_mkdir(dir)) {
        r36sx_pico286_debug_log("memory_dump: mkdir failed path='%s' errno=%d",
                                dir, errno);
        return;
    }

    snprintf(path, sizeof(path), "%s/registers.txt", dir);
    r36sx_emergency_dump_registers(path);
    snprintf(path, sizeof(path), "%s/ram.bin", dir);
    r36sx_emergency_write_file(path, RAM, RAM_SIZE);
    {
        size_t xms_bytes = xms_configured_memory_bytes();
        size_t hma_bytes = HMA_SIZE;
        if (xms_bytes > XMS_MEMORY_SIZE) {
            xms_bytes = XMS_MEMORY_SIZE;
        }
        if (hma_bytes > xms_bytes) {
            hma_bytes = xms_bytes;
        }
        snprintf(path, sizeof(path), "%s/xms.bin", dir);
        r36sx_emergency_write_file(path, XMS, xms_bytes);
        snprintf(path, sizeof(path), "%s/hma.bin", dir);
        r36sx_emergency_write_file(path, XMS, hma_bytes);
    }
    snprintf(path, sizeof(path), "%s/videoram.bin", dir);
    r36sx_emergency_write_file(path, VIDEORAM,
                               sizeof(VIDEORAM[0]) * (size_t)VIDEORAM_SIZE);
    snprintf(path, sizeof(path), "%s/svga_vram.bin", dir);
    r36sx_emergency_write_file(path, SVGA_VRAM, SVGA_VRAM_SIZE);
    snprintf(path, sizeof(path), "%s/text_b800.txt", dir);
    r36sx_emergency_dump_text_screen(path);

    r36sx_pico286_debug_log("memory_dump: wrote %s", dir);
}

#define R36SX_KEYBOARD_QUEUE_CAPACITY 8u
#define R36SX_PS2_MOUSE_QUEUE_CAPACITY 48u
#define R36SX_KEYBOARD_BYTE_DELAY_US 1000ull
#define R36SX_KBD_STATUS_OUTPUT_FULL 0x01u
#define R36SX_KBD_STATUS_INPUT_FULL 0x02u
#define R36SX_KBD_STATUS_MOUSE_OUTPUT_FULL 0x20u
#define R36SX_KBD_CMD_READ_CONFIG 0x20u
#define R36SX_KBD_CMD_WRITE_CONFIG 0x60u
#define R36SX_KBD_CMD_DISABLE_AUX 0xA7u
#define R36SX_KBD_CMD_ENABLE_AUX 0xA8u
#define R36SX_KBD_CMD_TEST_AUX 0xA9u
#define R36SX_KBD_CMD_SELF_TEST 0xAAu
#define R36SX_KBD_CMD_TEST_KEYBOARD 0xABu
#define R36SX_KBD_CMD_DISABLE_KEYBOARD 0xADu
#define R36SX_KBD_CMD_ENABLE_KEYBOARD 0xAEu
#define R36SX_KBD_CMD_READ_OUTPUT_PORT 0xD0u
#define R36SX_KBD_CMD_WRITE_OUTPUT_PORT 0xD1u
#define R36SX_KBD_CMD_WRITE_AUX_OUTPUT 0xD3u
#define R36SX_KBD_CMD_WRITE_AUX_INPUT 0xD4u
#define R36SX_KBD_CONFIG_KEYBOARD_IRQ 0x01u
#define R36SX_KBD_CONFIG_AUX_IRQ 0x02u
#define R36SX_KBD_CONFIG_SYSTEM_FLAG 0x04u
#define R36SX_KBD_CONFIG_KEYBOARD_CLOCK_DISABLED 0x10u
#define R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED 0x20u
#define R36SX_KBD_CONFIG_TRANSLATION 0x40u
#define R36SX_FAST_A20_ENABLE_BIT 0x02u
#define R36SX_PS2_ACK 0xFAu
#define R36SX_PS2_RESEND 0xFEu
#define R36SX_PS2_MOUSE_PENDING_NONE 0u
#define R36SX_PS2_MOUSE_PENDING_RESOLUTION 1u
#define R36SX_PS2_MOUSE_PENDING_SAMPLE_RATE 2u
#define R36SX_PC_POST_PORT 0x80u
#define R36SX_TEST386_SUBPOST_PORT 0x190u
#define R36SX_TEST386_ASCII_PORT 0x191u
/*
 * test386's printChar uses OUT imm8,AL for OUT_PORT.  Per Intel 80386, the
 * immediate port id is only 8 bits and is zero-extended to 16 bits, so a source
 * EQU of 0x191 is observed by hardware as port 0x91.
 */
#define R36SX_TEST386_ASCII_IMM8_PORT 0x91u

extern void r36sx_pico286_post_code_out(uint16_t portnum, uint8_t value);
extern void r36sx_pico286_post_code_reset(void);
extern void r36sx_cpu_debug_test386_subpost(uint16_t portnum, uint8_t value);
#if !PICO_ON_DEVICE
extern uint8_t r36sx_ata_portin8(uint16_t portnum);
extern uint16_t r36sx_ata_portin16(uint16_t portnum);
extern void r36sx_ata_portout8(uint16_t portnum, uint8_t value);
extern void r36sx_ata_portout16(uint16_t portnum, uint16_t value);
#endif

#if R36SX_DEBUG_TEST_BIOS_TRACE
#define R36SX_TEST_BIOS_LOG(...) r36sx_pico286_debug_log(__VA_ARGS__)
#else
#define R36SX_TEST_BIOS_LOG(...) ((void)0)
#endif

#if R36SX_DEBUG_KEYBOARD_TRACE
#define R36SX_KBD_LOG(...) r36sx_pico286_debug_log(__VA_ARGS__)
#else
#define R36SX_KBD_LOG(...) ((void)0)
#endif

static uint8_t keyboard_queue[R36SX_KEYBOARD_QUEUE_CAPACITY];
static uint8_t keyboard_queue_head;
static uint8_t keyboard_queue_count;
static uint8_t keyboard_output_full;
static uint64_t keyboard_next_ready_us;
static uint8_t keyboard_controller_response_ready;
static uint8_t keyboard_controller_write_config_byte;
static uint8_t keyboard_controller_write_output_port;
static uint8_t keyboard_controller_write_aux_output;
static uint8_t keyboard_controller_write_aux_input;
static uint8_t keyboard_controller_config_byte;
static uint8_t keyboard_controller_output_port;
static uint8_t ps2_mouse_queue[R36SX_PS2_MOUSE_QUEUE_CAPACITY];
static uint8_t ps2_mouse_queue_head;
static uint8_t ps2_mouse_queue_count;
static uint8_t ps2_mouse_output_full;
static uint64_t ps2_mouse_next_ready_us;
static uint8_t ps2_mouse_reporting_enabled;
static uint8_t ps2_mouse_stream_mode;
static uint8_t ps2_mouse_scaling_2_1;
static uint8_t ps2_mouse_resolution;
static uint8_t ps2_mouse_sample_rate;
static uint8_t ps2_mouse_pending_param;
static uint8_t ps2_mouse_buttons;
static uint8_t r36sx_test386_current_post;

void r36sx_pico286_post_reset(void)
{
    r36sx_test386_current_post = 0;
    r36sx_pico286_post_code_reset();
}

#if R36SX_DEBUG_TEST_BIOS_TRACE
static FILE *r36sx_test386_ee_output_fp;
static uint8_t r36sx_test386_ee_output_open_attempted;

static void r36sx_test386_ee_output_line(const char *line)
{
    if (!r36sx_test386_ee_output_fp &&
        !r36sx_test386_ee_output_open_attempted) {
        /*
         * Keep the huge POST EE arithmetic/logic reference stream out of the
         * capped main debug log.  The file is intentionally overwritten for
         * each emulator run so it can be diffed directly against upstream.
         */
        r36sx_test386_ee_output_open_attempted = 1;
        r36sx_test386_ee_output_fp = fopen("test386-ee-output.txt", "w");
        if (r36sx_test386_ee_output_fp) {
            r36sx_pico286_debug_log(
                "test386: writing POST EE output to test386-ee-output.txt");
        } else {
            r36sx_pico286_debug_log(
                "test386: failed to open test386-ee-output.txt");
        }
    }

    if (r36sx_test386_ee_output_fp) {
        fputs(line, r36sx_test386_ee_output_fp);
        fputc('\n', r36sx_test386_ee_output_fp);
        fflush(r36sx_test386_ee_output_fp);
    }
}
#endif

static void r36sx_test386_ascii_out(uint8_t value) {
    static char line[192];
    static uint8_t line_pos;

    if (value == '\r') {
        return;
    }
    if (value == '\n') {
        if (line_pos > 0) {
            line[line_pos] = 0;
            if (r36sx_test386_current_post == 0xEEu) {
#if R36SX_DEBUG_TEST_BIOS_TRACE
                r36sx_test386_ee_output_line(line);
#endif
            } else {
                R36SX_TEST_BIOS_LOG("test386: %s", line);
            }
            line_pos = 0;
        }
        return;
    }
    if (value >= 32 && value < 127 && line_pos < sizeof(line) - 1u) {
        line[line_pos++] = (char)value;
    }
}

static void r36sx_test386_subpost_out(uint8_t value) {
    r36sx_pico286_post_code_out(R36SX_TEST386_SUBPOST_PORT, value);
    R36SX_TEST_BIOS_LOG("test386: SUBPOST=0x%02x", value);
    r36sx_cpu_debug_test386_subpost(R36SX_TEST386_SUBPOST_PORT, value);
}

static void r36sx_audio_ensure_flags(void)
{
    if (audio_flags_loaded) {
        return;
    }

    audio_adlib_enabled = r36sx_pico286_audio_adlib_enabled();
    audio_cms_enabled = r36sx_pico286_audio_cms_enabled();
    audio_sn76489_enabled = r36sx_pico286_audio_sn76489_enabled();
    audio_mpu401_enabled = r36sx_pico286_audio_mpu401_enabled();
    audio_covox_enabled = r36sx_pico286_audio_covox_enabled();
    audio_flags_loaded = 1;
    r36sx_pico286_debug_log(
        "audio devices: adlib=%d cms=%d sn76489=%d mpu401=%d covox=%d",
        audio_adlib_enabled, audio_cms_enabled, audio_sn76489_enabled,
        audio_mpu401_enabled, audio_covox_enabled);
}

static INLINE uint64_t r36sx_keyboard_now_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static INLINE int r36sx_ps2_mouse_config_enabled(void)
{
    return r36sx_pico286_mouse_type() == R36SX_PICO286_MOUSE_PS2;
}

static INLINE int r36sx_ps2_mouse_aux_enabled(void)
{
    return r36sx_ps2_mouse_config_enabled() &&
           (keyboard_controller_config_byte &
            R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED) == 0;
}

static INLINE int r36sx_ps2_mouse_irq_enabled(void)
{
    return r36sx_ps2_mouse_aux_enabled() &&
           (keyboard_controller_config_byte & R36SX_KBD_CONFIG_AUX_IRQ) != 0;
}

static INLINE void r36sx_ps2_mouse_set_defaults(void)
{
    ps2_mouse_reporting_enabled = 0;
    ps2_mouse_stream_mode = 1;
    ps2_mouse_scaling_2_1 = 0;
    ps2_mouse_resolution = 2;
    ps2_mouse_sample_rate = 100;
    ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_NONE;
}

static INLINE void r36sx_keyboard_refresh_status(void) {
    port64 &= (uint8_t)~(R36SX_KBD_STATUS_OUTPUT_FULL |
                         R36SX_KBD_STATUS_INPUT_FULL |
                         R36SX_KBD_STATUS_MOUSE_OUTPUT_FULL);

    if (keyboard_controller_response_ready) {
        port64 |= R36SX_KBD_STATUS_OUTPUT_FULL;
    } else if (ps2_mouse_output_full && ps2_mouse_queue_count > 0) {
        port60 = ps2_mouse_queue[ps2_mouse_queue_head];
        port64 |= R36SX_KBD_STATUS_OUTPUT_FULL |
                  R36SX_KBD_STATUS_MOUSE_OUTPUT_FULL;
    } else if (keyboard_output_full && keyboard_queue_count > 0) {
        port60 = keyboard_queue[keyboard_queue_head];
        port64 |= R36SX_KBD_STATUS_OUTPUT_FULL;
    }
}

static void r36sx_ps2_mouse_enqueue(uint8_t data)
{
    uint8_t was_idle = ps2_mouse_queue_count == 0 && !ps2_mouse_output_full;

    if (!r36sx_ps2_mouse_config_enabled()) {
        return;
    }
    if (ps2_mouse_queue_count >= R36SX_PS2_MOUSE_QUEUE_CAPACITY) {
        R36SX_KBD_LOG("ps2mouse: queue full, drop data=0x%02x", data);
        return;
    }

    ps2_mouse_queue[(ps2_mouse_queue_head + ps2_mouse_queue_count) %
                    R36SX_PS2_MOUSE_QUEUE_CAPACITY] = data;
    ps2_mouse_queue_count++;
    r36sx_keyboard_refresh_status();

    if (was_idle) {
        ps2_mouse_next_ready_us = r36sx_keyboard_now_us();
    }
}

static INLINE void r36sx_ps2_mouse_tick(uint64_t now)
{
    if (ps2_mouse_queue_count == 0 || ps2_mouse_output_full ||
        keyboard_controller_response_ready || keyboard_output_full ||
        !r36sx_ps2_mouse_aux_enabled()) {
        return;
    }

    if (ps2_mouse_next_ready_us == 0) {
        ps2_mouse_next_ready_us = now + R36SX_KEYBOARD_BYTE_DELAY_US;
        return;
    }
    if ((int64_t)(now - ps2_mouse_next_ready_us) < 0) {
        return;
    }

    ps2_mouse_output_full = 1;
    ps2_mouse_next_ready_us = 0;
    r36sx_keyboard_refresh_status();
    R36SX_KBD_LOG("ps2mouse: ready data=0x%02x count=%u",
                  port60, (unsigned int)ps2_mouse_queue_count);
    if (r36sx_ps2_mouse_irq_enabled()) {
        doirq(12);
    }
}

void r36sx_keyboard_tick(void) {
    uint64_t now;

    now = r36sx_keyboard_now_us();
    r36sx_ps2_mouse_tick(now);

    if (keyboard_controller_response_ready || ps2_mouse_output_full ||
        keyboard_queue_count == 0 || keyboard_output_full) {
        r36sx_keyboard_refresh_status();
        return;
    }

    if (keyboard_next_ready_us == 0) {
        keyboard_next_ready_us = now + R36SX_KEYBOARD_BYTE_DELAY_US;
        r36sx_keyboard_refresh_status();
        return;
    }
    if ((int64_t)(now - keyboard_next_ready_us) < 0) {
        r36sx_keyboard_refresh_status();
        return;
    }

    keyboard_output_full = 1;
    keyboard_next_ready_us = 0;
    r36sx_keyboard_refresh_status();
    R36SX_KBD_LOG("kbd: ready scancode=0x%02x count=%u",
                  port60, (unsigned int)keyboard_queue_count);
    doirq(1);
}

void r36sx_keyboard_enqueue_scancode(uint8_t scancode) {
    uint8_t was_idle = keyboard_queue_count == 0 && !keyboard_output_full;

    if (keyboard_queue_count >= R36SX_KEYBOARD_QUEUE_CAPACITY) {
        R36SX_KBD_LOG("kbd: queue full, drop scancode=0x%02x", scancode);
        return;
    }

    keyboard_queue[(keyboard_queue_head + keyboard_queue_count) %
                   R36SX_KEYBOARD_QUEUE_CAPACITY] = scancode;
    keyboard_queue_count++;
    r36sx_keyboard_refresh_status();
    R36SX_KBD_LOG("kbd: enqueue scancode=0x%02x count=%u",
                  scancode, (unsigned int)keyboard_queue_count);

    if (was_idle) {
        keyboard_next_ready_us =
            r36sx_keyboard_now_us() + R36SX_KEYBOARD_BYTE_DELAY_US;
    }
}

void r36sx_keyboard_reset(void) {
    memset(keyboard_queue, 0, sizeof(keyboard_queue));
    memset(ps2_mouse_queue, 0, sizeof(ps2_mouse_queue));
    keyboard_queue_head = 0;
    keyboard_queue_count = 0;
    keyboard_output_full = 0;
    keyboard_next_ready_us = 0;
    keyboard_controller_response_ready = 0;
    keyboard_controller_write_config_byte = 0;
    keyboard_controller_write_output_port = 0;
    keyboard_controller_write_aux_output = 0;
    keyboard_controller_write_aux_input = 0;
    keyboard_controller_config_byte =
        R36SX_KBD_CONFIG_SYSTEM_FLAG |
        R36SX_KBD_CONFIG_KEYBOARD_IRQ |
        R36SX_KBD_CONFIG_TRANSLATION;
    if (r36sx_ps2_mouse_config_enabled()) {
        keyboard_controller_config_byte |= R36SX_KBD_CONFIG_AUX_IRQ;
    } else {
        keyboard_controller_config_byte |=
            R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED;
    }
    /* Reset must not preserve a memory-manager-enabled A20 gate across Ctrl+R. */
    a20_enabled = 0;
    keyboard_controller_output_port = 0;
    ps2_mouse_queue_head = 0;
    ps2_mouse_queue_count = 0;
    ps2_mouse_output_full = 0;
    ps2_mouse_next_ready_us = 0;
    ps2_mouse_buttons = 0;
    r36sx_ps2_mouse_set_defaults();
    port60 = 0;
    r36sx_keyboard_refresh_status();
}

static INLINE uint8_t r36sx_keyboard_read_data(void) {
    uint8_t data = port60;

    if (keyboard_controller_response_ready) {
        keyboard_controller_response_ready = 0;
        r36sx_keyboard_refresh_status();
        R36SX_KBD_LOG("kbd: read controller response=0x%02x", data);
        return data;
    }

    r36sx_keyboard_tick();
    if (ps2_mouse_output_full && ps2_mouse_queue_count > 0) {
        data = ps2_mouse_queue[ps2_mouse_queue_head];
        ps2_mouse_queue_head =
            (ps2_mouse_queue_head + 1u) % R36SX_PS2_MOUSE_QUEUE_CAPACITY;
        ps2_mouse_queue_count--;
        ps2_mouse_output_full = 0;
        ps2_mouse_next_ready_us = ps2_mouse_queue_count > 0 ?
            r36sx_keyboard_now_us() + R36SX_KEYBOARD_BYTE_DELAY_US : 0;
        r36sx_keyboard_refresh_status();
        R36SX_KBD_LOG("ps2mouse: read data=0x%02x remaining=%u",
                      data, (unsigned int)ps2_mouse_queue_count);
        return data;
    }
    if (keyboard_output_full && keyboard_queue_count > 0) {
        data = keyboard_queue[keyboard_queue_head];
        keyboard_queue_head =
            (keyboard_queue_head + 1u) % R36SX_KEYBOARD_QUEUE_CAPACITY;
        keyboard_queue_count--;
        keyboard_output_full = 0;
        keyboard_next_ready_us = keyboard_queue_count > 0 ?
            r36sx_keyboard_now_us() + R36SX_KEYBOARD_BYTE_DELAY_US : 0;
        r36sx_keyboard_refresh_status();
        R36SX_KBD_LOG("kbd: read scancode=0x%02x remaining=%u",
                      data, (unsigned int)keyboard_queue_count);
    }

    return data;
}

static INLINE uint8_t r36sx_ps2_mouse_packet_buttons(uint8_t buttons)
{
    uint8_t packet_buttons = 0;

    if (buttons & 0x02u) {
        packet_buttons |= 0x01u;
    }
    if (buttons & 0x01u) {
        packet_buttons |= 0x02u;
    }
    if (buttons & 0x04u) {
        packet_buttons |= 0x04u;
    }
    return packet_buttons;
}

static void r36sx_ps2_mouse_enqueue_packet(uint8_t buttons, int xrel,
                                           int yrel)
{
    const int packet_y = -yrel;
    uint8_t packet0 = (uint8_t)(0x08u |
                                r36sx_ps2_mouse_packet_buttons(buttons));

    if (xrel < 0) {
        packet0 |= 0x10u;
    }
    if (packet_y < 0) {
        packet0 |= 0x20u;
    }

    r36sx_ps2_mouse_enqueue(packet0);
    r36sx_ps2_mouse_enqueue((uint8_t)(xrel & 0xff));
    r36sx_ps2_mouse_enqueue((uint8_t)(packet_y & 0xff));
}

static void r36sx_ps2_mouse_status(void)
{
    uint8_t status = r36sx_ps2_mouse_packet_buttons(ps2_mouse_buttons);

    if (!ps2_mouse_stream_mode) {
        status |= 0x40u;
    }
    if (ps2_mouse_reporting_enabled) {
        status |= 0x20u;
    }
    if (ps2_mouse_scaling_2_1) {
        status |= 0x10u;
    }

    r36sx_ps2_mouse_enqueue(status);
    r36sx_ps2_mouse_enqueue(ps2_mouse_resolution);
    r36sx_ps2_mouse_enqueue(ps2_mouse_sample_rate);
}

static void r36sx_ps2_mouse_write(uint8_t value)
{
    if (!r36sx_ps2_mouse_aux_enabled()) {
        return;
    }

    if (ps2_mouse_pending_param == R36SX_PS2_MOUSE_PENDING_RESOLUTION) {
        ps2_mouse_resolution = (uint8_t)(value & 0x03u);
        ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_NONE;
        r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
        R36SX_KBD_LOG("ps2mouse: set resolution=%u",
                      (unsigned int)ps2_mouse_resolution);
        return;
    }
    if (ps2_mouse_pending_param == R36SX_PS2_MOUSE_PENDING_SAMPLE_RATE) {
        ps2_mouse_sample_rate = value;
        ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_NONE;
        r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
        R36SX_KBD_LOG("ps2mouse: set sample_rate=%u",
                      (unsigned int)ps2_mouse_sample_rate);
        return;
    }

    switch (value) {
        case 0xE6:
            ps2_mouse_scaling_2_1 = 0;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xE7:
            ps2_mouse_scaling_2_1 = 1;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xE8:
            ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_RESOLUTION;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xE9:
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            r36sx_ps2_mouse_status();
            return;
        case 0xEA:
            ps2_mouse_stream_mode = 1;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xEB:
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            r36sx_ps2_mouse_enqueue_packet(ps2_mouse_buttons, 0, 0);
            return;
        case 0xF0:
            ps2_mouse_stream_mode = 0;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xF2:
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            r36sx_ps2_mouse_enqueue(0x00u);
            return;
        case 0xF3:
            ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_SAMPLE_RATE;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xF4:
            ps2_mouse_reporting_enabled = 1;
            ps2_mouse_stream_mode = 1;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xF5:
            ps2_mouse_reporting_enabled = 0;
            ps2_mouse_pending_param = R36SX_PS2_MOUSE_PENDING_NONE;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xF6:
            r36sx_ps2_mouse_set_defaults();
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xFE:
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            return;
        case 0xFF:
            r36sx_ps2_mouse_set_defaults();
            ps2_mouse_buttons = 0;
            r36sx_ps2_mouse_enqueue(R36SX_PS2_ACK);
            r36sx_ps2_mouse_enqueue(0xAAu);
            r36sx_ps2_mouse_enqueue(0x00u);
            return;
        default:
            r36sx_ps2_mouse_enqueue(R36SX_PS2_RESEND);
            R36SX_KBD_LOG("ps2mouse: unsupported command=0x%02x", value);
            return;
    }
}

static INLINE int r36sx_mouse_clamp_delta(int value, int limit)
{
    if (value > limit) {
        return limit;
    }
    if (value < -limit) {
        return -limit;
    }
    return value;
}

static void r36sx_ps2_mouse_event(uint8_t buttons, int xrel, int yrel)
{
    ps2_mouse_buttons = buttons;
    if (!r36sx_ps2_mouse_aux_enabled() || !ps2_mouse_reporting_enabled ||
        !ps2_mouse_stream_mode) {
        return;
    }
    r36sx_ps2_mouse_enqueue_packet(buttons, xrel, yrel);
}

void r36sx_mouse_event(uint8_t buttons, int xrel, int yrel)
{
    const int ps2_mode = r36sx_pico286_mouse_type() == R36SX_PICO286_MOUSE_PS2;
    const int limit = ps2_mode ? 255 : 63;
    int sent = 0;

    buttons &= 0x07u;
    while (xrel != 0 || yrel != 0) {
        const int step_x = r36sx_mouse_clamp_delta(xrel, limit);
        const int step_y = r36sx_mouse_clamp_delta(yrel, limit);
        if (ps2_mode) {
            r36sx_ps2_mouse_event(buttons, step_x, step_y);
        } else {
            sermouseevent(buttons, (int8_t)step_x, (int8_t)step_y);
        }
        xrel -= step_x;
        yrel -= step_y;
        sent = 1;
    }

    if (!sent) {
        if (ps2_mode) {
            r36sx_ps2_mouse_event(buttons, 0, 0);
        } else {
            sermouseevent(buttons, 0, 0);
        }
    }
}

static INLINE void r36sx_keyboard_output_port_write(uint8_t value) {
    keyboard_controller_output_port = value;
    a20_enabled = (value & R36SX_FAST_A20_ENABLE_BIT) != 0;
    r36sx_pico286_debug_log(
        "kbd: output port write value=%02x fast_a20=%d",
        (unsigned)value, a20_enabled);
}

void r36sx_keyboard_controller_set_a20(int enabled)
{
    /*
     * INT 15h AX=2400h/2401h is a BIOS-level A20 service.  Expose the same
     * state through the 8042 output-port read command so probes that mix BIOS
     * and keyboard-controller methods do not observe stale A20 state.
     */
    if (enabled) {
        keyboard_controller_output_port |= R36SX_FAST_A20_ENABLE_BIT;
        a20_enabled = 1;
    } else {
        keyboard_controller_output_port &=
            (uint8_t)~R36SX_FAST_A20_ENABLE_BIT;
        a20_enabled = 0;
    }
}

static uint16_t adlibregmem[5], adlib_register = 0;
static uint8_t adlibstatus = 0;

static int8_t joystick_tick;
static INLINE void joystick_out() {
#if PICO_ON_DEVICE
    joystick_tick = -127;
#endif
}

static INLINE uint8_t joystick_in() {
    uint8_t data = 0xF0;
#if PICO_ON_DEVICE
    nespad_read();
    int8_t axis_x = nespad_state & DPAD_LEFT ? -127 : (nespad_state & DPAD_RIGHT) ? 127 : 0;
    int8_t axis_y = nespad_state & DPAD_UP ? -127 : (nespad_state & DPAD_DOWN) ? 127 : 0;
    joystick_tick++;

    if (joystick_tick < axis_x) data |= 1;
    if (joystick_tick < axis_y) data |= 2;
    if (nespad_state & DPAD_A) data ^= 0x10;
    if (nespad_state & DPAD_B) data ^= 0x20;
#endif
    return data;
}


#define R36SX_CMOS_REG_SECONDS 0x00u
#define R36SX_CMOS_REG_SECONDS_ALARM 0x01u
#define R36SX_CMOS_REG_MINUTES 0x02u
#define R36SX_CMOS_REG_MINUTES_ALARM 0x03u
#define R36SX_CMOS_REG_HOURS 0x04u
#define R36SX_CMOS_REG_HOURS_ALARM 0x05u
#define R36SX_CMOS_REG_DAY_OF_WEEK 0x06u
#define R36SX_CMOS_REG_DAY_OF_MONTH 0x07u
#define R36SX_CMOS_REG_MONTH 0x08u
#define R36SX_CMOS_REG_YEAR 0x09u
#define R36SX_CMOS_REG_STATUS_A 0x0Au
#define R36SX_CMOS_REG_STATUS_B 0x0Bu
#define R36SX_CMOS_REG_STATUS_C 0x0Cu
#define R36SX_CMOS_REG_STATUS_D 0x0Du
#define R36SX_CMOS_REG_CENTURY 0x32u
#define R36SX_CMOS_STATUS_B_24H 0x02u
#define R36SX_CMOS_STATUS_B_BINARY 0x04u
#define R36SX_CMOS_STATUS_B_SET 0x80u

static uint8_t cmos_index;
static uint8_t cmos_nmi_disabled;
static uint8_t cmos_ram[128];
static int cmos_initialized;
static time_t rtc_host_start;
static time_t rtc_start;

static uint8_t r36sx_rtc_to_bcd(uint8_t value)
{
    return (uint8_t)(((value / 10u) << 4) | (value % 10u));
}

static int r36sx_rtc_from_bcd(uint8_t value, uint8_t *decoded)
{
    uint8_t high = (value >> 4) & 0x0fu;
    uint8_t low = value & 0x0fu;

    if (high > 9u || low > 9u) {
        return 0;
    }

    *decoded = (uint8_t)(high * 10u + low);
    return 1;
}

static uint8_t r36sx_cmos_encode_value(uint8_t value)
{
    if (cmos_ram[R36SX_CMOS_REG_STATUS_B] & R36SX_CMOS_STATUS_B_BINARY) {
        return value;
    }
    return r36sx_rtc_to_bcd(value);
}

static int r36sx_cmos_decode_value(uint8_t raw, uint8_t *value)
{
    if (cmos_ram[R36SX_CMOS_REG_STATUS_B] & R36SX_CMOS_STATUS_B_BINARY) {
        *value = raw;
        return 1;
    }
    return r36sx_rtc_from_bcd(raw, value);
}

static uint8_t r36sx_cmos_encode_hour(uint8_t hour)
{
    if (cmos_ram[R36SX_CMOS_REG_STATUS_B] & R36SX_CMOS_STATUS_B_24H) {
        return r36sx_cmos_encode_value(hour);
    }

    uint8_t pm = hour >= 12u ? 0x80u : 0x00u;
    uint8_t hour12 = (uint8_t)(hour % 12u);
    if (hour12 == 0) {
        hour12 = 12u;
    }
    return (uint8_t)(r36sx_cmos_encode_value(hour12) | pm);
}

static int r36sx_cmos_decode_hour(uint8_t raw, uint8_t *hour)
{
    uint8_t decoded;

    if (cmos_ram[R36SX_CMOS_REG_STATUS_B] & R36SX_CMOS_STATUS_B_24H) {
        if (!r36sx_cmos_decode_value(raw, &decoded) || decoded > 23u) {
            return 0;
        }
        *hour = decoded;
        return 1;
    }

    uint8_t pm = raw & 0x80u;
    raw &= 0x7fu;
    if (!r36sx_cmos_decode_value(raw, &decoded) ||
        decoded < 1u || decoded > 12u) {
        return 0;
    }
    if (decoded == 12u) {
        decoded = 0u;
    }
    *hour = (uint8_t)(decoded + (pm ? 12u : 0u));
    return 1;
}

static time_t r36sx_rtc_current_time_raw(void)
{
    time_t host_now;

    if (time(&host_now) == (time_t)-1) {
        return rtc_start;
    }

    return rtc_start + (host_now - rtc_host_start);
}

static int r36sx_cmos_time_register(uint8_t reg)
{
    return reg == R36SX_CMOS_REG_SECONDS ||
           reg == R36SX_CMOS_REG_MINUTES ||
           reg == R36SX_CMOS_REG_HOURS ||
           reg == R36SX_CMOS_REG_DAY_OF_WEEK ||
           reg == R36SX_CMOS_REG_DAY_OF_MONTH ||
           reg == R36SX_CMOS_REG_MONTH ||
           reg == R36SX_CMOS_REG_YEAR ||
           reg == R36SX_CMOS_REG_CENTURY;
}

static int r36sx_rtc_update_in_progress(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        return 0;
    }

    return ts.tv_nsec >= 999000000L;
}

static void r36sx_cmos_latch_time_regs(void)
{
    time_t rtc_now = r36sx_rtc_current_time_raw();
    struct tm tm_buf;
    struct tm *t = localtime_r(&rtc_now, &tm_buf);
    int full_year;

    if (!t) {
        return;
    }

    full_year = t->tm_year + 1900;
    cmos_ram[R36SX_CMOS_REG_SECONDS] =
        r36sx_cmos_encode_value((uint8_t)t->tm_sec);
    cmos_ram[R36SX_CMOS_REG_MINUTES] =
        r36sx_cmos_encode_value((uint8_t)t->tm_min);
    cmos_ram[R36SX_CMOS_REG_HOURS] =
        r36sx_cmos_encode_hour((uint8_t)t->tm_hour);
    cmos_ram[R36SX_CMOS_REG_DAY_OF_WEEK] =
        r36sx_cmos_encode_value((uint8_t)(t->tm_wday + 1));
    cmos_ram[R36SX_CMOS_REG_DAY_OF_MONTH] =
        r36sx_cmos_encode_value((uint8_t)t->tm_mday);
    cmos_ram[R36SX_CMOS_REG_MONTH] =
        r36sx_cmos_encode_value((uint8_t)(t->tm_mon + 1));
    cmos_ram[R36SX_CMOS_REG_YEAR] =
        r36sx_cmos_encode_value((uint8_t)(full_year % 100));
    cmos_ram[R36SX_CMOS_REG_CENTURY] =
        r36sx_cmos_encode_value((uint8_t)(full_year / 100));
}

static void r36sx_cmos_init_once(void)
{
    if (cmos_initialized) {
        return;
    }

    memset(cmos_ram, 0, sizeof(cmos_ram));
    cmos_ram[R36SX_CMOS_REG_STATUS_A] = 0x26u;
    cmos_ram[R36SX_CMOS_REG_STATUS_B] = R36SX_CMOS_STATUS_B_24H;
    cmos_ram[R36SX_CMOS_REG_STATUS_D] = 0x80u;

    if (time(&rtc_host_start) == (time_t)-1) {
        rtc_host_start = 0;
    }
    rtc_start = (time_t)r36sx_pico286_rtc_start_time_unix();
    cmos_initialized = 1;
    r36sx_cmos_latch_time_regs();
    r36sx_pico286_debug_log("rtc: start unix=%ld", (long)rtc_start);
}

int64_t r36sx_pico286_rtc_current_time_unix(void)
{
    r36sx_cmos_init_once();
    return (int64_t)r36sx_rtc_current_time_raw();
}

int r36sx_pico286_rtc_set_time_unix(int64_t unix_time)
{
    time_t parsed = (time_t)unix_time;
    time_t host_now;

    r36sx_cmos_init_once();

    if (parsed < 0 || (int64_t)parsed != unix_time) {
        r36sx_pico286_debug_log("rtc: rejecting out-of-range unix=%lld",
                                (long long)unix_time);
        return 0;
    }

    if (time(&host_now) == (time_t)-1) {
        host_now = rtc_host_start;
    }
    rtc_host_start = host_now;
    rtc_start = parsed;
    r36sx_cmos_latch_time_regs();
    r36sx_pico286_debug_log("rtc: set unix=%ld", (long)rtc_start);
    return 1;
}

static void r36sx_cmos_commit_time_regs(void)
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t century = 0;
    int full_year;
    struct tm tm_value;
    time_t parsed;

    if (!r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_SECONDS], &second) ||
        !r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_MINUTES], &minute) ||
        !r36sx_cmos_decode_hour(cmos_ram[R36SX_CMOS_REG_HOURS], &hour) ||
        !r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_DAY_OF_MONTH], &day) ||
        !r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_MONTH], &month) ||
        !r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_YEAR], &year)) {
        r36sx_pico286_debug_log("rtc: ignoring invalid CMOS time write");
        return;
    }

    if (!r36sx_cmos_decode_value(cmos_ram[R36SX_CMOS_REG_CENTURY],
                                 &century) ||
        century < 19u || century > 20u) {
        full_year = year >= 80u ? 1900 + year : 2000 + year;
    } else {
        full_year = century * 100 + year;
    }

    if (full_year < 1980 || full_year > 2037 ||
        month < 1u || month > 12u ||
        day < 1u || day > 31u ||
        hour > 23u || minute > 59u || second > 59u) {
        r36sx_pico286_debug_log("rtc: ignoring out-of-range CMOS time write");
        return;
    }

    memset(&tm_value, 0, sizeof(tm_value));
    tm_value.tm_year = full_year - 1900;
    tm_value.tm_mon = month - 1;
    tm_value.tm_mday = day;
    tm_value.tm_hour = hour;
    tm_value.tm_min = minute;
    tm_value.tm_sec = second;
    tm_value.tm_isdst = -1;

    parsed = mktime(&tm_value);
    if (parsed == (time_t)-1 ||
        tm_value.tm_year != full_year - 1900 ||
        tm_value.tm_mon != month - 1 ||
        tm_value.tm_mday != day ||
        tm_value.tm_hour != hour ||
        tm_value.tm_min != minute ||
        tm_value.tm_sec != second) {
        r36sx_pico286_debug_log("rtc: ignoring normalized CMOS time write");
        return;
    }

    r36sx_pico286_rtc_set_time_unix((int64_t)parsed);
}

static uint8_t r36sx_cmos_read(uint8_t reg)
{
    r36sx_cmos_init_once();
    reg &= 0x7fu;

    if (r36sx_cmos_time_register(reg) &&
        !(cmos_ram[R36SX_CMOS_REG_STATUS_B] & R36SX_CMOS_STATUS_B_SET)) {
        r36sx_cmos_latch_time_regs();
    }

    switch (reg) {
        case R36SX_CMOS_REG_STATUS_A:
            return (uint8_t)((cmos_ram[R36SX_CMOS_REG_STATUS_A] & 0x7fu) |
                             (!(cmos_ram[R36SX_CMOS_REG_STATUS_B] &
                                R36SX_CMOS_STATUS_B_SET) &&
                              r36sx_rtc_update_in_progress() ? 0x80u : 0u));
        case R36SX_CMOS_REG_STATUS_C: {
            uint8_t value = cmos_ram[R36SX_CMOS_REG_STATUS_C];
            cmos_ram[R36SX_CMOS_REG_STATUS_C] = 0;
            return value;
        }
        case R36SX_CMOS_REG_STATUS_D:
            return 0x80u;
        default:
            return cmos_ram[reg];
    }
}

static void r36sx_cmos_write(uint8_t reg, uint8_t value)
{
    r36sx_cmos_init_once();
    reg &= 0x7fu;

    switch (reg) {
        case R36SX_CMOS_REG_STATUS_A:
            cmos_ram[R36SX_CMOS_REG_STATUS_A] = value & 0x7fu;
            return;
        case R36SX_CMOS_REG_STATUS_B: {
            uint8_t old = cmos_ram[R36SX_CMOS_REG_STATUS_B];

            if (!(old & R36SX_CMOS_STATUS_B_SET) &&
                (value & R36SX_CMOS_STATUS_B_SET)) {
                r36sx_cmos_latch_time_regs();
            }

            cmos_ram[R36SX_CMOS_REG_STATUS_B] = value;
            if ((old & R36SX_CMOS_STATUS_B_SET) &&
                !(value & R36SX_CMOS_STATUS_B_SET)) {
                r36sx_cmos_commit_time_regs();
            } else if (((old ^ value) &
                        (R36SX_CMOS_STATUS_B_BINARY |
                         R36SX_CMOS_STATUS_B_24H)) &&
                       !(value & R36SX_CMOS_STATUS_B_SET)) {
                r36sx_cmos_latch_time_regs();
            }
            return;
        }
        case R36SX_CMOS_REG_STATUS_C:
        case R36SX_CMOS_REG_STATUS_D:
            return;
        default:
            if (r36sx_cmos_time_register(reg) &&
                !(cmos_ram[R36SX_CMOS_REG_STATUS_B] &
                  R36SX_CMOS_STATUS_B_SET)) {
                r36sx_cmos_latch_time_regs();
                cmos_ram[reg] = value;
                r36sx_cmos_commit_time_regs();
            } else {
                cmos_ram[reg] = value;
            }
            return;
    }
}

static uint8_t r36sx_xt_rtc_read(uint16_t addr)
{
    uint8_t ret = 0xFF;
    time_t rtc_now;
    struct tm tm_buf;
    struct tm *t;

    r36sx_cmos_init_once();
    rtc_now = r36sx_rtc_current_time_raw();
    t = localtime_r(&rtc_now, &tm_buf);
    if (!t) {
        return ret;
    }

    switch (addr & 0x1Fu) {
        case 1:
            ret = 0;
            break;
        case 2:
            ret = (uint8_t)t->tm_sec;
            break;
        case 3:
            ret = (uint8_t)t->tm_min;
            break;
        case 4:
            ret = (uint8_t)t->tm_hour;
            break;
        case 5:
            ret = (uint8_t)t->tm_wday;
            break;
        case 6:
            ret = (uint8_t)t->tm_mday;
            break;
        case 7:
            ret = (uint8_t)t->tm_mon + 1;
            break;
        case 9:
            ret = (uint8_t)t->tm_year % 100;
            break;
    }

    return ret == 0xFFu ? ret : r36sx_rtc_to_bcd(ret);
}

static void r36sx_xt_rtc_write(uint16_t addr, uint8_t value)
{
    uint8_t reg;

    switch (addr & 0x1fu) {
        case 2:
            reg = R36SX_CMOS_REG_SECONDS;
            break;
        case 3:
            reg = R36SX_CMOS_REG_MINUTES;
            break;
        case 4:
            reg = R36SX_CMOS_REG_HOURS;
            break;
        case 5:
            reg = R36SX_CMOS_REG_DAY_OF_WEEK;
            break;
        case 6:
            reg = R36SX_CMOS_REG_DAY_OF_MONTH;
            break;
        case 7:
            reg = R36SX_CMOS_REG_MONTH;
            break;
        case 9:
            reg = R36SX_CMOS_REG_YEAR;
            break;
        default:
            return;
    }

    r36sx_cmos_write(reg, value);
}

static INLINE uint8_t rtc_read(uint16_t addr) {
    return r36sx_xt_rtc_read(addr);
}

void portout(uint16_t portnum, uint16_t value) {
#if R36SX_DEBUG_HOSTRPC_TRACE
    static char hostdrv_trace_text[128];
    static uint8_t hostdrv_trace_text_len;
    static uint8_t hostdrv_trace_collecting;
#endif

    if (portnum == R36SX_HOSTDRV_TRACE_PORT) {
#if R36SX_DEBUG_HOSTRPC_TRACE
        uint8_t code = (uint8_t)value;
        if (code == 0xD0u) {
            hostdrv_trace_collecting = 1u;
            hostdrv_trace_text_len = 0u;
            hostdrv_trace_text[0] = '\0';
            return;
        }
        if (code == 0xD1u) {
            hostdrv_trace_text[hostdrv_trace_text_len] = '\0';
            r36sx_pico286_debug_log("hostdrv: 1123 source='%s'",
                                    hostdrv_trace_text);
            hostdrv_trace_collecting = 0u;
            return;
        }
        if (hostdrv_trace_collecting) {
            if (hostdrv_trace_text_len < sizeof(hostdrv_trace_text) - 1u) {
                hostdrv_trace_text[hostdrv_trace_text_len++] =
                    (code >= 0x20u && code <= 0x7Eu) ? (char)code : '.';
            }
            return;
        }
        switch (code) {
            case 0xF0u:
                r36sx_pico286_debug_log("hostdrv: success");
                break;
            case 0xF1u:
                r36sx_pico286_debug_log("hostdrv: fail");
                break;
            case 0xF2u:
                r36sx_pico286_debug_log("hostdrv: chain");
                break;
            case 0xF3u:
                r36sx_pico286_debug_log("hostdrv: sft ours");
                break;
            case 0xF4u:
                r36sx_pico286_debug_log("hostdrv: sft not ours");
                break;
            default:
                r36sx_pico286_debug_log("hostdrv: int2f ax=11%02x", code);
                break;
        }
#endif
        return;
    }
    if (portnum == R36SX_EMERGENCY_DUMP_PORT) {
#if R36SX_DEBUG_HOSTRPC_TRACE || DEBUG
        r36sx_emergency_dump_request((uint8_t)value, "guest-port");
        r36sx_emergency_dump_write_and_clear();
#endif
        return;
    }

    switch (portnum) {
        case R36SX_PC_POST_PORT:
            r36sx_test386_current_post = (uint8_t)value;
            r36sx_pico286_post_code_out(portnum, (uint8_t)value);
            /*
             * Main POSTs are frequent during early ROM self-tests.  Keep the
             * compact POST log for all values, but only arm the expensive
             * instruction trace for the protected-mode blocks we are debugging.
             */
            if ((uint8_t)value >= 0x20u) {
                r36sx_cpu_debug_test386_subpost(portnum, (uint8_t)value);
            }
            return;
        case R36SX_TEST386_SUBPOST_PORT:
            r36sx_test386_subpost_out((uint8_t)value);
            return;
        case R36SX_TEST386_ASCII_IMM8_PORT:
        case R36SX_TEST386_ASCII_PORT:
            r36sx_test386_ascii_out((uint8_t)value);
            return;
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0a:
        case 0x0b:
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
// i8237 DMA
            return i8237_writeport(portnum, value);
        case 0x20:
        case 0x21: // i8259 PIC
            return out8259(portnum, value);
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43: // i8253 PIT
            return out8253(portnum, value);
        case 0x60: // Keyboard Controller data port
            if (keyboard_controller_write_config_byte) {
                keyboard_controller_write_config_byte = 0;
                keyboard_controller_config_byte =
                    (uint8_t)(value & (R36SX_KBD_CONFIG_KEYBOARD_IRQ |
                                       R36SX_KBD_CONFIG_AUX_IRQ |
                                       R36SX_KBD_CONFIG_SYSTEM_FLAG |
                                       R36SX_KBD_CONFIG_KEYBOARD_CLOCK_DISABLED |
                                       R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED |
                                       R36SX_KBD_CONFIG_TRANSLATION));
                if (!r36sx_ps2_mouse_config_enabled()) {
                    keyboard_controller_config_byte |=
                        R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED;
                    keyboard_controller_config_byte &=
                        (uint8_t)~R36SX_KBD_CONFIG_AUX_IRQ;
                }
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: config byte write value=0x%02x",
                              keyboard_controller_config_byte);
                return;
            }
            if (keyboard_controller_write_output_port) {
                keyboard_controller_write_output_port = 0;
                r36sx_keyboard_output_port_write((uint8_t)value);
                r36sx_keyboard_refresh_status();
                return;
            }
            if (keyboard_controller_write_aux_output) {
                keyboard_controller_write_aux_output = 0;
                if (r36sx_ps2_mouse_config_enabled()) {
                    r36sx_ps2_mouse_enqueue((uint8_t)value);
                }
                r36sx_keyboard_refresh_status();
                return;
            }
            if (keyboard_controller_write_aux_input) {
                keyboard_controller_write_aux_input = 0;
                r36sx_ps2_mouse_write((uint8_t)value);
                r36sx_keyboard_refresh_status();
                return;
            }
#if PICO_ON_DEVICE
            keyboard_send(value);
#endif
            port60 = (uint8_t)value;
            r36sx_keyboard_refresh_status();
            return;
        case 0x61: // PC Speaker
            port61 = value;
            if ((value & 3) == 3) {
#if I2S_SOUND || HARDWARE_SOUND || !PICO_ON_DEVICE
                speakerenabled = 1;
#else
                pwm_set_gpio_level(PWM_BEEPER, 127);
#endif
            } else {
#if I2S_SOUND || HARDWARE_SOUND || !PICO_ON_DEVICE
                speakerenabled = 0;
#else
                pwm_set_gpio_level(PWM_BEEPER, 0);
#endif
            }

            break;
        case 0x64: // Keyboard Controller
            if ((uint8_t)value == R36SX_KBD_CMD_READ_CONFIG) {
                port60 = keyboard_controller_config_byte;
                keyboard_controller_response_ready = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command read config");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_WRITE_CONFIG) {
                keyboard_controller_write_config_byte = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command write config");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_DISABLE_AUX) {
                keyboard_controller_config_byte |=
                    R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command disable aux");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_ENABLE_AUX) {
                if (r36sx_ps2_mouse_config_enabled()) {
                    keyboard_controller_config_byte &=
                        (uint8_t)~R36SX_KBD_CONFIG_AUX_CLOCK_DISABLED;
                }
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command enable aux");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_TEST_AUX) {
                port60 = r36sx_ps2_mouse_config_enabled() ? 0x00u : 0x01u;
                keyboard_controller_response_ready = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command test aux result=0x%02x", port60);
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_SELF_TEST) {
                port60 = 0x55u;
                keyboard_controller_response_ready = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command self-test");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_TEST_KEYBOARD) {
                port60 = 0x00u;
                keyboard_controller_response_ready = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command test keyboard");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_DISABLE_KEYBOARD) {
                keyboard_controller_config_byte |=
                    R36SX_KBD_CONFIG_KEYBOARD_CLOCK_DISABLED;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command disable keyboard");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_ENABLE_KEYBOARD) {
                keyboard_controller_config_byte &=
                    (uint8_t)~R36SX_KBD_CONFIG_KEYBOARD_CLOCK_DISABLED;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command enable keyboard");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_READ_OUTPUT_PORT) {
                port60 = (uint8_t)((keyboard_controller_output_port &
                                    (uint8_t)~R36SX_FAST_A20_ENABLE_BIT) |
                                   (a20_enabled ? R36SX_FAST_A20_ENABLE_BIT :
                                                  0x00u));
                keyboard_controller_response_ready = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command read output port");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_WRITE_OUTPUT_PORT) {
                keyboard_controller_write_output_port = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command write output port");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_WRITE_AUX_OUTPUT) {
                keyboard_controller_write_aux_output = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command write aux output");
                return;
            }
            if ((uint8_t)value == R36SX_KBD_CMD_WRITE_AUX_INPUT) {
                keyboard_controller_write_aux_input = 1;
                r36sx_keyboard_refresh_status();
                R36SX_KBD_LOG("kbd: command write aux input");
                return;
            }
#if PICO_ON_DEVICE
            keyboard_send(value);
#endif
            port64 = (uint8_t)value;
            r36sx_keyboard_refresh_status();
            break;
        case 0x70: // AT CMOS index / NMI mask
            if (r36sx_pico286_rtc_at_enabled()) {
                cmos_index = (uint8_t)(value & 0x7fu);
                cmos_nmi_disabled = (uint8_t)((value & 0x80u) != 0);
            }
            return;
        case 0x71: // AT CMOS data
            if (r36sx_pico286_rtc_at_enabled()) {
                r36sx_cmos_write(cmos_index, (uint8_t)value);
            }
            return;
// i8237 DMA
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x87:
// i8237 DMA Page Registers
            return i8237_writepage(portnum, value);

// A20 Gate
        case 0x92:
            /*
             * PS/2 system control port A uses bit 1 for Fast A20 Gate.
             * Bit 0 is CPU reset, so treating it as A20 makes DOS/16M and
             * HIMEM-style probes believe extended memory is inaccessible.
             */
            a20_enabled = (value & R36SX_FAST_A20_ENABLE_BIT) != 0;
            keyboard_controller_output_port =
                (uint8_t)((keyboard_controller_output_port &
                           (uint8_t)~R36SX_FAST_A20_ENABLE_BIT) |
                          (a20_enabled ? R36SX_FAST_A20_ENABLE_BIT : 0x00u));
            r36sx_pico286_debug_log("ports: fast A20 write value=%02x enabled=%d",
                                    (unsigned)value, a20_enabled);
            return;
// Tandy 3-Voice Sound
        case 0x1E0:
        case 0x2C0:
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5:
        case 0xC6:
        case 0xC7:
#if HARDWARE_SOUND
        if (!sound_chips_clock) {
            clock_init(CLOCK_PIN, CLOCK_FREQUENCY);
            sound_chips_clock = 1;
        }
        return SN76489_write(value);
#else
        return sn76489_out(value);
#endif
// Joystick
        case 0x201:
            return joystick_out();
// Creative Music System / GameBlaster
        case 0x220:
        case 0x221:
        case 0x222:
        case 0x223:
#if HARDWARE_SOUND
if (sound_chips_clock) {
    clock_init(CLOCK_PIN, CLOCK_FREQUENCY * 2);
    sound_chips_clock = 0;
}
        switch (portnum & 3) {
            case 0:
                SAA1099_write(0, 0, value);
            break;
            case 1:
                SAA1099_write(1, 0, value);
            break;
            case 2:
                SAA1099_write(0, 1, value);
            break;
            case 3:
                SAA1099_write(1, 1, value);
            break;
        }
#else
        cms_out(portnum, value);
#endif
        return;
        case 0x224:
        case 0x225:
        case 0x226:
        case 0x227:
        case 0x228:
        case 0x229:
        case 0x22a:
        case 0x22b:
        case 0x22c:
        case 0x22d:
        case 0x22e:
        case 0x22f:
// Sound Blaster
#if !PICO_RP2040
            blaster_write(portnum, value);
#endif
        return;
        case 0x240:
        case 0x241:
        case 0x242:
        case 0x243:
        case 0x244:
        case 0x245:
        case 0x246:
        case 0x247:
        case 0x248:
        case 0x249:
        case 0x24A:
        case 0x24B:
        case 0x24C:
        case 0x24D:
        case 0x24E:
        case 0x24F:
            if (r36sx_pico286_rtc_xt_enabled()) {
                r36sx_xt_rtc_write(portnum, (uint8_t)value);
                return;
            }
#if !PICO_RP2040
            blaster_write(portnum, value);
#endif
            return;
        case 0x250:
        case 0x251:
        case 0x252:
        case 0x253:
        case 0x254:
        case 0x255:
        case 0x256:
        case 0x257:
            if (r36sx_pico286_rtc_xt_enabled()) {
                r36sx_xt_rtc_write(portnum, (uint8_t)value);
            }
            return;
        case 0x260:
        case 0x261:
        case 0x262:
        case 0x263:
// EMS
            return out_ems(portnum, value);

#if !PICO_ON_DEVICE
        case 0x170:
        case 0x171:
        case 0x172:
        case 0x173:
        case 0x174:
        case 0x175:
        case 0x176:
        case 0x177:
        case 0x1F0:
        case 0x1F1:
        case 0x1F2:
        case 0x1F3:
        case 0x1F4:
        case 0x1F5:
        case 0x1F6:
        case 0x1F7:
        case 0x376:
        case 0x3F6:
// PATA/IDE task-file registers.
            r36sx_ata_portout8(portnum, (uint8_t)value);
            return;
#endif

        case 0x278:
// Covox Speech Thing
            covox_sample = (int16_t)((value - 128) << 6);
            return;
        case 0x330:
        case 0x331:
// MPU-401
            return mpu401_write(portnum, value);
        case 0x378:
        case 0x37A:
// Disney Sound Source
            return dss_out(portnum, value);
// AdLib / OPL2
        case 0x388:
            adlib_register = value;
            break;
        case 0x389:
            if (adlib_register <= 4) {
                adlibregmem[adlib_register] = value;

                if (adlib_register == 4 && value & 0x80) {
                    adlibstatus = 0;
                    adlibregmem[4] = 0;
                }
            }
#if HARDWARE_SOUND
        if (!sound_chips_clock) {
            clock_init(CLOCK_PIN, CLOCK_FREQUENCY);
            sound_chips_clock = 1;
        }
            OPL2_write_byte(0, 0, adlib_register & 0xff);
            OPL2_write_byte(1, 0, value & 0xff);
        return;
#else
            return OPL_writeReg(emu8950_opl, adlib_register, value);
#endif
// EGA/VGA
        case 0x3C4:
        case 0x3CE:
        case 0x3C0:
        case 0x3C2:
        case 0x3C5:
        case 0x3C6:
        case 0x3C7:
        case 0x3C8:
        case 0x3C9:
        case 0x3CF:
            return vga_portout(portnum, value);
// https://stanislavs.org/helppc/6845.html
// https://bitsavers.trailing-edge.com/components/motorola/_dataSheets/6845.pdf
// https://www.theoddys.com/acorn/the_6845_crtc/the_6845_crtc.html
// MC6845 CRTC
        case 0x3B0:
        case 0x3B2:
        case 0x3B4:
        case 0x3B6:
        case 0x3D0:
        case 0x3D2:
        case 0x3D4:
        case 0x3D6:
            crt_controller_idx = value & 31;
            break;
        case 0x3B1:
        case 0x3B3:
        case 0x3B5:
        case 0x3B7:
        case 0x3D1:
        case 0x3D3:
        case 0x3D5:
        case 0x3D7: {
            uint8_t old_value = crt_controller[crt_controller_idx];
            switch (crt_controller_idx) {
                case 0x4: {
                    if (value == 0x3e/* && videomode == 1*/) {
                        videomode = 0x79;

                    }
                    break;
                }

                case 0x6:
//                    printf("!!! Y = %i\n", value);
                    // 160x100x16 or 160x200x16 mode TODO: Add more checks
                    if (value == 0x64 && (videomode <= 3)) {
                        videomode = cga_hires ? 0x76 : 0x77;
                    }

                    // 160x46x16 mode TODO: Add more checks
                    if (value == 0x2e/* && (videomode <= 3))*/) {
                        videomode = 0x87;
                    }
                    break;

                case 0x8:
                    break;
// Cursor pos
                case 0x0A:
                    cursor_start = value;
                    //cursor_visible = !(value & 0x20) && (cursor_start < 8);
                    break;
                case 0x0B:
                    cursor_end = value;
                    break;

// Screen offset
                case 0x0C: // Start address (MSB)
                    vram_offset = (vram_offset & 0x00ffu) |
                                  ((uint32_t)(value & 0xffu) << 8);
                    // printf("vram offset %04X\n", vram_offset);
                    break;
                case 0x0D: // Start address (LSB)
                    vram_offset = (vram_offset & 0xff00u) |
                                  (uint32_t)(value & 0xffu);
                    // printf("vram offset %04X\n", vram_offset);
                    break;
            }

//            if ((crt_controller_idx != 0x03) && ((crt_controller_idx != 0x0E) && (crt_controller_idx != 0x0F) && (crt_controller_idx != 0x0c) && (crt_controller_idx != 0x0d)))
//                printf("CRT %x %x\n", crt_controller_idx, value);

            crt_controller[crt_controller_idx] = value;
            if (old_value != (uint8_t)value) {
                r36sx_pico286_video_mark_dirty();
            }

            break;
        }
        case 0x3B8:
        case 0x3BF:
        case 0x3D8:
        case 0x3D9:
// CGA
            return cga_portout(portnum, value);
        case 0x3DA:
        case 0x3DE:
        case 0x3DF:
// TGA
            return tga_portout(portnum, value);
        case 0x3F8:
        case 0x3F9:
        case 0x3FA:
        case 0x3FB:
        case 0x3FC:
        case 0x3FD:
        case 0x3FE:
        case 0x3FF:
// Serial Port (Mouse)
            return mouse_portout(portnum, value);
    }
}

uint16_t portin(uint16_t portnum) {
    switch (portnum) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0a:
        case 0x0b:
        case 0x0c:
        case 0x0d:
        case 0x0e:
        case 0x0f:
// i8237 DMA
            return i8237_readport(portnum);
        case 0x20:
        case 0x21: // i8259 PIC
            return in8259(portnum);
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43: // i8253 PIT
            return in8253(portnum);

// Keyboard
        case 0x60:
            return r36sx_keyboard_read_data();
        case 0x61:
            return port61;
        case 0x64:
            r36sx_keyboard_tick();
            return port64;
        case 0x70:
            if (r36sx_pico286_rtc_at_enabled()) {
                return (uint8_t)(cmos_index |
                                 (cmos_nmi_disabled ? 0x80u : 0u));
            }
            return 0xFF;
        case 0x71:
            if (r36sx_pico286_rtc_at_enabled()) {
                return r36sx_cmos_read(cmos_index);
            }
            return 0xFF;
// i8237 DMA Page Registers
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x87:
            return i8237_readpage(portnum);
// A20 Gate
        case 0x92:
            r36sx_pico286_debug_log("ports: fast A20 read enabled=%d",
                                    a20_enabled);
            return a20_enabled ? R36SX_FAST_A20_ENABLE_BIT : 0x00u;
        case 0x201:
// Joystick
            return joystick_in();

        case 0x220:
        case 0x221:
        case 0x222:
        case 0x223:
// Creative Music System / GameBlaster
#if !PICO_RP2040
            return audio_cms_enabled ? cms_in(portnum) : 0xFF;
#else
            return cms_in(portnum);
#endif
        case 0x224:
        case 0x225:
        case 0x226:
        case 0x227:
        case 0x228:
        case 0x229:
        case 0x22a:
        case 0x22b:
        case 0x22c:
        case 0x22d:
        case 0x22e:
        case 0x22f:
// Sound Blaster DSP
#if !PICO_RP2040
            return blaster_read(portnum);
#else
            return cms_in(portnum);
#endif
// RTC
        case 0x240:
        case 0x241:
        case 0x242:
        case 0x243:
        case 0x244:
        case 0x245:
        case 0x246:
        case 0x247:
        case 0x248:
        case 0x249:
        case 0x24A:
        case 0x24B:
        case 0x24C:
        case 0x24D:
        case 0x24E:
        case 0x24F:
            if (r36sx_pico286_rtc_xt_enabled()) {
                return rtc_read(portnum);
            }
#if !PICO_RP2040
            return blaster_read(portnum);
#else
            return 0xFF;
#endif
        case 0x250:
        case 0x251:
        case 0x252:
        case 0x253:
        case 0x254:
        case 0x255:
        case 0x256:
        case 0x257:
            return r36sx_pico286_rtc_xt_enabled() ? rtc_read(portnum) : 0xFF;
        case 0x27A: // Covox Speech Thing
            return 0;
#if !PICO_ON_DEVICE
        case 0x170:
        case 0x171:
        case 0x172:
        case 0x173:
        case 0x174:
        case 0x175:
        case 0x176:
        case 0x177:
        case 0x1F0:
        case 0x1F1:
        case 0x1F2:
        case 0x1F3:
        case 0x1F4:
        case 0x1F5:
        case 0x1F6:
        case 0x1F7:
        case 0x376:
        case 0x3F6:
// PATA/IDE task-file registers.
            return r36sx_ata_portin8(portnum);
#endif
        case 0x330:
        case 0x331:
// MPU-401
            return mpu401_read(portnum);
        case 0x378:
        case 0x379:
// Disney Sound Source
            return dss_in(portnum);
        case 0x37A:
            return 0;
// AdLib
        case 0x388:
        case 0x389:
            if (!adlibregmem[4])
                adlibstatus = 0;
            else
                adlibstatus = 0x80;

            adlibstatus = adlibstatus + (adlibregmem[4] & 1) * 0x40 + (adlibregmem[4] & 2) * 0x10;
            return adlibstatus;
        case 0x3C1:
        case 0x3C2:
        case 0x3C5:
        case 0x3C6:
        case 0x3C7:
        case 0x3C8:
        case 0x3C9:
        case 0x3CC:
        case 0x3CF:
// VGA
            return vga_portin(portnum);

        case 0x3D4:
// CRTC
            return crt_controller_idx;
        case 0x3D5:
// CRTC
            return crt_controller[crt_controller_idx];
        case 0x3DA:
// CGA
            vga_attribute_reset_flipflop();
            return cga_portin(portnum);
        case 0x3F8:
        case 0x3F9:
        case 0x3FA:
        case 0x3FB:
        case 0x3FC:
        case 0x3FD:
        case 0x3FE:
        case 0x3FF:
// Serial Port (Mouse)
            return mouse_portin(portnum);
        default:
            return 0xFF;
    }
}


void portout16(uint16_t portnum, uint16_t value) {
    if (portnum >= R36SX_HOST_RPC_PORT_BASE &&
        portnum <= R36SX_HOST_RPC_PORT_LAST) {
        r36sx_host_rpc_portout16(portnum, value);
        return;
    }

#if !PICO_ON_DEVICE
    if (portnum == 0x1F0u || portnum == 0x170u) {
        r36sx_ata_portout16(portnum, value);
        return;
    }
#endif

    portout(portnum, (uint8_t) value);
    portout(portnum + 1, (uint8_t) (value >> 8));
}

uint16_t portin16(uint16_t portnum) {
    if (portnum >= R36SX_HOST_RPC_PORT_BASE &&
        portnum <= R36SX_HOST_RPC_PORT_LAST) {
        return r36sx_host_rpc_portin16(portnum);
    }

#if !PICO_ON_DEVICE
    if (portnum == 0x1F0u || portnum == 0x170u) {
        return r36sx_ata_portin16(portnum);
    }
#endif

    return portin(portnum) | portin(portnum + 1) << 8;
}


static int16_t r36sx_clamp_i16(int32_t sample)
{
    if (sample > 32767) {
        return 32767;
    }
    if (sample < -32768) {
        return -32768;
    }
    return (int16_t)sample;
}

void get_sound_sample(const int16_t other_sample, int16_t *samples) {
    r36sx_audio_ensure_flags();

#if HARDWARE_SOUND
    const int32_t sample = speaker_sample() + other_sample +
        (audio_covox_enabled ? covox_sample : 0) +
        (audio_mpu401_enabled ? midi_sample() : 0);
    pwm_set_gpio_level(PCM_PIN, (uint16_t) ((int32_t) sample + 0x8000L) >> 4);
#else
    int32_t opl_sample[1] = {0};
    int16_t cms_mix[2] = {0, 0};
    int32_t mixed;

    if (audio_adlib_enabled && emu8950_opl) {
        OPL_calc_buffer_linear(emu8950_opl, opl_sample, 1);
    }

    mixed = opl_sample[0] + (int32_t)(speaker_sample() + other_sample +
            (audio_covox_enabled ? covox_sample : 0) +
            (audio_sn76489_enabled ? sn76489_sample() : 0) +
            (audio_mpu401_enabled ? midi_sample() : 0));
    cms_mix[0] = r36sx_clamp_i16(mixed);
    cms_mix[1] = cms_mix[0];
    if (audio_cms_enabled) {
        cms_samples(cms_mix);
    }
    samples[0] = cms_mix[0];
    samples[1] = cms_mix[1];
#endif

}
