#include "r36sx_debug_control.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulator/emulator.h"
#include "r36sx_cpu.h"
#include "r36sx_debug_config.h"
#include "r36sx_disk_config.h"

#ifndef R36SX_DEBUG_CONTROL_MAX_COMMAND
#define R36SX_DEBUG_CONTROL_MAX_COMMAND 4096u
#endif

#ifndef R36SX_DEBUG_CONTROL_MAX_INLINE_BYTES
#define R36SX_DEBUG_CONTROL_MAX_INLINE_BYTES 4096u
#endif

#ifdef _WIN32
#define R36SX_PATH_SEP '\\'
#else
#define R36SX_PATH_SEP '/'
#endif

extern void r36sx_keyboard_enqueue_scancode(uint8_t scancode);
extern void r36sx_memory_dump_request(uint8_t code, const char *reason);
extern void r36sx_emergency_dump_request(uint8_t code, const char *reason);
extern int a20_enabled;

static int g_debug_control_enabled = 0;
static char g_command_path[512];
static char g_response_path[512];
static char g_artifact_dir[512];
static const uint16_t *g_framebuffer;
static uint32_t g_framebuffer_width;
static uint32_t g_framebuffer_height;
static uint32_t g_framebuffer_stride;

static int debug_str_eq(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

static char *debug_trim(char *text)
{
    char *end;

    while (*text && isspace((unsigned char)*text)) {
        text++;
    }
    end = text + strlen(text);
    while (end > text && isspace((unsigned char)end[-1])) {
        *--end = '\0';
    }
    return text;
}

static char *debug_next_token(char **cursor)
{
    char *token;
    char *p = *cursor;

    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    if (*p == '\0') {
        *cursor = p;
        return NULL;
    }
    token = p;
    while (*p && !isspace((unsigned char)*p)) {
        p++;
    }
    if (*p) {
        *p++ = '\0';
    }
    *cursor = p;
    return token;
}

static int debug_path_is_absolute(const char *path)
{
    if (!path || !path[0]) {
        return 0;
    }
#ifdef _WIN32
    if ((isalpha((unsigned char)path[0]) && path[1] == ':') ||
        (path[0] == '\\' && path[1] == '\\')) {
        return 1;
    }
#endif
    return path[0] == '/' || path[0] == '\\';
}

static void debug_resolve_path(char *dest, size_t dest_size,
                               const char *value,
                               const char *fallback)
{
    const char *raw = (value && value[0]) ? value : fallback;
    const char *base = r36sx_pico286_config_dir();

    if (debug_path_is_absolute(raw) || !base || base[0] == '\0') {
        snprintf(dest, dest_size, "%s", raw);
        return;
    }
    snprintf(dest, dest_size, "%s%c%s", base, R36SX_PATH_SEP, raw);
}

static void debug_artifact_path(char *dest, size_t dest_size,
                                const char *name)
{
    if (debug_path_is_absolute(name)) {
        snprintf(dest, dest_size, "%s", name);
        return;
    }
    snprintf(dest, dest_size, "%s%c%s", g_artifact_dir, R36SX_PATH_SEP, name);
}

static int debug_parse_u32_default(const char *text, uint32_t *value,
                                   int default_base)
{
    char *end = NULL;
    char buffer[32];
    size_t len;
    size_t i;
    unsigned long parsed;
    int base = default_base;

    if (!text || text[0] == '\0') {
        return 0;
    }
    if (text[0] == '$') {
        text++;
        base = 16;
    } else if (text[0] == '0' &&
               (text[1] == 'x' || text[1] == 'X')) {
        base = 16;
    } else {
        for (i = 0; text[i] != '\0'; i++) {
            if ((text[i] >= 'a' && text[i] <= 'f') ||
                (text[i] >= 'A' && text[i] <= 'F')) {
                base = 16;
                break;
            }
        }
    }
    len = strlen(text);
    if (len >= 2u && (text[len - 1u] == 'h' || text[len - 1u] == 'H')) {
        if (len >= sizeof(buffer)) {
            return 0;
        }
        memcpy(buffer, text, len - 1u);
        buffer[len - 1u] = '\0';
        text = buffer;
        base = 16;
    }
    parsed = strtoul(text, &end, base);
    if (!end || *end != '\0' || parsed > 0xffffffffUL) {
        return 0;
    }
    *value = (uint32_t)parsed;
    return 1;
}

static int debug_parse_u32(const char *text, uint32_t *value)
{
    return debug_parse_u32_default(text, value, 10);
}

static int debug_parse_address(const char *text, uint32_t *address)
{
    const char *colon;
    uint32_t segment;
    uint32_t offset;

    if (!text || !address) {
        return 0;
    }
    colon = strchr(text, ':');
    if (!colon) {
        return debug_parse_u32(text, address);
    }
    {
        char left[32];
        char right[32];
        size_t left_len = (size_t)(colon - text);
        if (left_len >= sizeof(left) ||
            strlen(colon + 1) >= sizeof(right)) {
            return 0;
        }
        memcpy(left, text, left_len);
        left[left_len] = '\0';
        snprintf(right, sizeof(right), "%s", colon + 1);
        if (!debug_parse_u32_default(left, &segment, 16) ||
            !debug_parse_u32_default(right, &offset, 16) ||
            segment > 0xffffu || offset > 0xffffu) {
            return 0;
        }
    }
    *address = ((segment << 4) + offset) & 0xffffffffu;
    return 1;
}

static int debug_parse_breakpoint_address(const char *text,
                                          uint8_t *linear,
                                          uint16_t *cs,
                                          uint32_t *eip,
                                          uint32_t *linear_address)
{
    const char *colon;

    if (!text || !linear || !cs || !eip || !linear_address) {
        return 0;
    }
    colon = strchr(text, ':');
    if (!colon) {
        uint32_t parsed;
        if (!debug_parse_u32(text, &parsed)) {
            return 0;
        }
        *linear = 1u;
        *cs = 0;
        *eip = 0;
        *linear_address = parsed;
        return 1;
    }

    {
        char left[32];
        char right[32];
        uint32_t segment;
        uint32_t offset;
        size_t left_len = (size_t)(colon - text);
        if (left_len >= sizeof(left) ||
            strlen(colon + 1) >= sizeof(right)) {
            return 0;
        }
        memcpy(left, text, left_len);
        left[left_len] = '\0';
        snprintf(right, sizeof(right), "%s", colon + 1);
        if (!debug_parse_u32_default(left, &segment, 16) ||
            !debug_parse_u32_default(right, &offset, 16) ||
            segment > 0xffffu) {
            return 0;
        }
        *linear = 0u;
        *cs = (uint16_t)segment;
        *eip = offset;
        *linear_address = ((segment << 4) + offset) & 0xffffffffu;
        return 1;
    }
}

static int debug_parse_scan_byte(const char *text, uint8_t *value)
{
    uint32_t parsed;
    if (!debug_parse_u32(text, &parsed) || parsed > 0xffu) {
        return 0;
    }
    *value = (uint8_t)parsed;
    return 1;
}

static void debug_write_header(FILE *out)
{
    fprintf(out, "ok 1\n");
}

static void debug_write_error(FILE *out, const char *message)
{
    fprintf(out, "ok 0\nerror %s\n", message);
}

static void debug_write_errno_error(FILE *out, const char *message,
                                    const char *path)
{
    fprintf(out, "ok 0\nerror %s errno=%d path=%s\n",
            message, errno, path ? path : "");
}

static void debug_write_hex(FILE *out, uint32_t address, uint32_t length,
                            uint32_t base_address)
{
    uint32_t i;

    fprintf(out, "address=%08lX length=%lu\n",
            (unsigned long)base_address, (unsigned long)length);
    for (i = 0; i < length; i++) {
        if ((i & 15u) == 0) {
            fprintf(out, "%08lX:", (unsigned long)(base_address + i));
        }
        fprintf(out, " %02X", (unsigned)read86_ob(address + i));
        if ((i & 15u) == 15u || i + 1u == length) {
            fputc('\n', out);
        }
    }
}

static uint8_t debug_current_user_access(void)
{
    r36sx_cpu_debug_snapshot_t s;
    r36sx_cpu_debug_snapshot(&s);
    return s.cpl == 3u;
}

static int debug_translate_linear_as(uint32_t linear,
                                     uint8_t write_access,
                                     uint8_t user_access,
                                     uint32_t *physical,
                                     uint32_t *pde,
                                     uint32_t *pte)
{
    return r36sx_cpu_debug_translate_linear(
        linear, write_access, user_access, physical, pde, pte);
}

static void debug_write_virtual_hex(FILE *out, uint32_t linear,
                                    uint32_t length,
                                    uint8_t user_access)
{
    uint32_t i;

    fprintf(out, "linear=%08lX length=%lu\n",
            (unsigned long)linear, (unsigned long)length);
    for (i = 0; i < length; i++) {
        uint32_t physical;
        if (!debug_translate_linear_as(linear + i, 0, user_access,
                                       &physical, NULL, NULL)) {
            fprintf(out, "\ntranslation_failed linear=%08lX\n",
                    (unsigned long)(linear + i));
            return;
        }
        if ((i & 15u) == 0) {
            fprintf(out, "%08lX:", (unsigned long)(linear + i));
        }
        fprintf(out, " %02X", (unsigned)read86_ob(physical));
        if ((i & 15u) == 15u || i + 1u == length) {
            fputc('\n', out);
        }
    }
}

static int debug_dump_memory_file(uint32_t address, uint32_t length,
                                  const char *file_name,
                                  FILE *out)
{
    char path[512];
    FILE *fp;
    uint32_t i;

    debug_artifact_path(path, sizeof(path), file_name);
    fp = fopen(path, "wb");
    if (!fp) {
        debug_write_errno_error(out, "write file failed", path);
        return 0;
    }
    for (i = 0; i < length; i++) {
        fputc(read86_ob(address + i), fp);
    }
    if (fclose(fp) != 0) {
        debug_write_errno_error(out, "close file failed", path);
        return 0;
    }
    debug_write_header(out);
    fprintf(out, "file=%s bytes=%lu\n", path, (unsigned long)length);
    return 1;
}

static int debug_dump_virtual_memory_file(uint32_t linear, uint32_t length,
                                          const char *file_name,
                                          FILE *out)
{
    char path[512];
    FILE *fp;
    uint32_t i;
    uint8_t user_access = debug_current_user_access();

    debug_artifact_path(path, sizeof(path), file_name);
    fp = fopen(path, "wb");
    if (!fp) {
        debug_write_errno_error(out, "write file failed", path);
        return 0;
    }
    for (i = 0; i < length; i++) {
        uint32_t physical;
        if (!debug_translate_linear_as(linear + i, 0, user_access,
                                       &physical, NULL, NULL)) {
            fclose(fp);
            debug_write_error(out, "linear address not mapped");
            return 0;
        }
        fputc(read86_ob(physical), fp);
    }
    if (fclose(fp) != 0) {
        debug_write_errno_error(out, "close file failed", path);
        return 0;
    }
    debug_write_header(out);
    fprintf(out, "file=%s bytes=%lu linear=%08lX\n",
            path, (unsigned long)length, (unsigned long)linear);
    return 1;
}

static int debug_dump_vram_file(uint32_t offset, uint32_t length,
                                const char *file_name,
                                FILE *out)
{
    if (offset > VIDEORAM_SIZE || length > VIDEORAM_SIZE - offset) {
        debug_write_error(out, "vram range outside 64K window");
        return 0;
    }
    return debug_dump_memory_file(VIDEORAM_START + offset, length,
                                  file_name, out);
}

static int debug_dump_screen_file(const char *file_name, FILE *out)
{
    char path[512];
    FILE *fp;
    uint32_t y;

    if (!g_framebuffer || !g_framebuffer_width || !g_framebuffer_height ||
        !g_framebuffer_stride) {
        debug_write_error(out, "framebuffer not registered");
        return 0;
    }

    debug_artifact_path(path, sizeof(path), file_name);
    fp = fopen(path, "wb");
    if (!fp) {
        debug_write_errno_error(out, "write file failed", path);
        return 0;
    }
    for (y = 0; y < g_framebuffer_height; y++) {
        const uint16_t *row = g_framebuffer + y * g_framebuffer_stride;
        if (fwrite(row, sizeof(uint16_t), g_framebuffer_width, fp) !=
            g_framebuffer_width) {
            fclose(fp);
            debug_write_errno_error(out, "write file failed", path);
            return 0;
        }
    }
    if (fclose(fp) != 0) {
        debug_write_errno_error(out, "close file failed", path);
        return 0;
    }
    debug_write_header(out);
    fprintf(out, "file=%s format=rgb565 width=%lu height=%lu stride_pixels=%lu bytes=%lu\n",
            path,
            (unsigned long)g_framebuffer_width,
            (unsigned long)g_framebuffer_height,
            (unsigned long)g_framebuffer_stride,
            (unsigned long)(g_framebuffer_width * g_framebuffer_height * 2u));
    return 1;
}

static void debug_command_regs(FILE *out)
{
    r36sx_cpu_debug_snapshot_t s;
    r36sx_cpu_debug_snapshot(&s);

    fprintf(out,
            "eax=%08lX ebx=%08lX ecx=%08lX edx=%08lX esi=%08lX edi=%08lX ebp=%08lX esp=%08lX\n",
            (unsigned long)s.eax, (unsigned long)s.ebx,
            (unsigned long)s.ecx, (unsigned long)s.edx,
            (unsigned long)s.esi, (unsigned long)s.edi,
            (unsigned long)s.ebp, (unsigned long)s.esp);
    fprintf(out,
            "cs=%04X ip=%08lX ss=%04X sp=%08lX ds=%04X es=%04X fs=%04X gs=%04X flags=%08lX\n",
            (unsigned)s.cs, (unsigned long)s.eip,
            (unsigned)s.ss, (unsigned long)s.esp,
            (unsigned)s.ds, (unsigned)s.es,
            (unsigned)s.fs, (unsigned)s.gs,
            (unsigned long)s.eflags);
    fprintf(out,
            "cr0=%08lX cr2=%08lX cr3=%08lX pm=%u vm86=%u cpl=%u a20=%d videomode=%02X vram_offset=%08lX port60=%02X port64=%02X port3da=%02X\n",
            (unsigned long)s.cr0, (unsigned long)s.cr2,
            (unsigned long)s.cr3, (unsigned)s.protected_mode,
            (unsigned)s.vm86_mode, (unsigned)s.cpl, a20_enabled,
            (unsigned)videomode, (unsigned long)vram_offset,
            (unsigned)port60, (unsigned)port64, (unsigned)port3DA);
}

static void debug_command_mem(char *cursor, FILE *out)
{
    char *addr_token = debug_next_token(&cursor);
    char *len_token = debug_next_token(&cursor);
    char *file_token = debug_next_token(&cursor);
    uint32_t address;
    uint32_t length;

    if (!addr_token || !len_token ||
        !debug_parse_address(addr_token, &address) ||
        !debug_parse_u32(len_token, &length)) {
        debug_write_error(out, "usage: mem <address|seg:off> <length> [file]");
        return;
    }
    if (file_token) {
        debug_dump_memory_file(address, length, file_token, out);
        return;
    }
    if (length > R36SX_DEBUG_CONTROL_MAX_INLINE_BYTES) {
        debug_write_error(out, "inline memory response too large; pass file name");
        return;
    }
    debug_write_header(out);
    debug_write_hex(out, address, length, address);
}

static void debug_command_vmem(char *cursor, FILE *out)
{
    char *addr_token = debug_next_token(&cursor);
    char *len_token = debug_next_token(&cursor);
    char *file_token = debug_next_token(&cursor);
    uint32_t linear;
    uint32_t length;
    uint8_t user_access;

    if (!addr_token || !len_token ||
        !debug_parse_address(addr_token, &linear) ||
        !debug_parse_u32(len_token, &length)) {
        debug_write_error(out, "usage: vmem <linear|seg:off> <length> [file]");
        return;
    }
    if (file_token) {
        debug_dump_virtual_memory_file(linear, length, file_token, out);
        return;
    }
    if (length > R36SX_DEBUG_CONTROL_MAX_INLINE_BYTES) {
        debug_write_error(out, "inline virtual memory response too large; pass file name");
        return;
    }
    user_access = debug_current_user_access();
    debug_write_header(out);
    debug_write_virtual_hex(out, linear, length, user_access);
}

static void debug_command_pagemap(char *cursor, FILE *out)
{
    char *addr_token = debug_next_token(&cursor);
    uint32_t linear;
    uint32_t physical;
    uint32_t pde;
    uint32_t pte;
    uint8_t user_access;

    if (!addr_token || !debug_parse_address(addr_token, &linear)) {
        debug_write_error(out, "usage: pagemap <linear|seg:off>");
        return;
    }

    debug_write_header(out);
    user_access = debug_current_user_access();
    if (!debug_translate_linear_as(linear, 0, user_access,
                                   &physical, &pde, &pte)) {
        fprintf(out,
                "linear=%08lX mapped=0 pde=%08lX pte=%08lX\n",
                (unsigned long)linear, (unsigned long)pde,
                (unsigned long)pte);
        return;
    }
    fprintf(out,
            "linear=%08lX physical=%08lX pde=%08lX pte=%08lX\n",
            (unsigned long)linear, (unsigned long)physical,
            (unsigned long)pde, (unsigned long)pte);
}

static void debug_command_vram(char *cursor, FILE *out)
{
    char *offset_token = debug_next_token(&cursor);
    char *len_token = debug_next_token(&cursor);
    char *file_token = debug_next_token(&cursor);
    uint32_t offset = 0;
    uint32_t length = VIDEORAM_SIZE;

    if (offset_token && !debug_parse_u32(offset_token, &offset)) {
        debug_write_error(out, "usage: vram [offset] [length] [file]");
        return;
    }
    if (len_token && !debug_parse_u32(len_token, &length)) {
        debug_write_error(out, "usage: vram [offset] [length] [file]");
        return;
    }
    if (offset > VIDEORAM_SIZE || length > VIDEORAM_SIZE - offset) {
        debug_write_error(out, "vram range outside 64K window");
        return;
    }
    if (file_token || length > R36SX_DEBUG_CONTROL_MAX_INLINE_BYTES) {
        debug_dump_vram_file(offset, length,
                             file_token ? file_token : "debug_vram.bin",
                             out);
        return;
    }
    debug_write_header(out);
    debug_write_hex(out, VIDEORAM_START + offset, length,
                    VIDEORAM_START + offset);
}

static int debug_collect_scancodes(char *cursor, uint8_t *bytes,
                                   size_t *count, FILE *out)
{
    char *token;
    *count = 0;
    while ((token = debug_next_token(&cursor)) != NULL) {
        if (*count >= 16u) {
            debug_write_error(out, "too many scancode bytes");
            return 0;
        }
        if (!debug_parse_scan_byte(token, &bytes[*count])) {
            debug_write_error(out, "invalid scancode byte");
            return 0;
        }
        (*count)++;
    }
    if (*count == 0) {
        debug_write_error(out, "missing scancode byte");
        return 0;
    }
    return 1;
}

static void debug_enqueue_keyup(const uint8_t *bytes, size_t count)
{
    size_t i;
    for (i = 0; i < count; i++) {
        if (bytes[i] == 0xe0u || bytes[i] == 0xe1u) {
            r36sx_keyboard_enqueue_scancode(bytes[i]);
        } else {
            r36sx_keyboard_enqueue_scancode((uint8_t)(bytes[i] | 0x80u));
        }
    }
}

static void debug_command_key(char *cursor, const char *command, FILE *out)
{
    uint8_t bytes[16];
    size_t count = 0;
    size_t i;

    if (!debug_collect_scancodes(cursor, bytes, &count, out)) {
        return;
    }

    if (debug_str_eq(command, "keyup")) {
        debug_enqueue_keyup(bytes, count);
    } else {
        for (i = 0; i < count; i++) {
            r36sx_keyboard_enqueue_scancode(bytes[i]);
        }
        if (debug_str_eq(command, "key")) {
            debug_enqueue_keyup(bytes, count);
        }
    }
    debug_write_header(out);
    fprintf(out, "queued=%lu\n", (unsigned long)count);
}

static void debug_write_breakpoint(FILE *out,
                                   uint32_t slot,
                                   const r36sx_cpu_debug_host_breakpoint_t *bp)
{
    if (bp->linear) {
        fprintf(out,
                "slot=%lu enabled=1 type=linear linear=%08lX hits=%llu\n",
                (unsigned long)slot,
                (unsigned long)bp->linear_address,
                (unsigned long long)bp->hit_count);
    } else {
        fprintf(out,
                "slot=%lu enabled=1 type=csip cs:eip=%04X:%08lX linear_hint=%08lX hits=%llu\n",
                (unsigned long)slot, (unsigned)bp->cs,
                (unsigned long)bp->eip,
                (unsigned long)(((uint32_t)bp->cs << 4) + bp->eip),
                (unsigned long long)bp->hit_count);
    }
}

static void debug_command_bp(char *cursor, FILE *out)
{
    char *addr_token = debug_next_token(&cursor);
    uint8_t linear;
    uint16_t cs;
    uint32_t eip;
    uint32_t linear_address;
    uint32_t slot;

    if (!addr_token ||
        !debug_parse_breakpoint_address(addr_token, &linear, &cs, &eip,
                                        &linear_address)) {
        debug_write_error(out, "usage: bp <linear|cs:eip>");
        return;
    }
    if (r36sx_cpu_debug_host_breakpoint_add(linear, cs, eip, linear_address,
                                           &slot) != 0) {
        debug_write_error(out, "no free breakpoint slot");
        return;
    }
    debug_write_header(out);
    fprintf(out, "breakpoint_added=%lu\n", (unsigned long)slot);
}

static void debug_command_bpset(char *cursor, FILE *out)
{
    char *slot_token = debug_next_token(&cursor);
    char *addr_token = debug_next_token(&cursor);
    uint32_t slot;
    uint8_t linear;
    uint16_t cs;
    uint32_t eip;
    uint32_t linear_address;

    if (!slot_token || !addr_token ||
        !debug_parse_u32(slot_token, &slot) ||
        !debug_parse_breakpoint_address(addr_token, &linear, &cs, &eip,
                                        &linear_address)) {
        debug_write_error(out, "usage: bpset <slot> <linear|cs:eip>");
        return;
    }
    if (r36sx_cpu_debug_host_breakpoint_set(slot, linear, cs, eip,
                                           linear_address) != 0) {
        debug_write_error(out, "invalid breakpoint slot");
        return;
    }
    debug_write_header(out);
    fprintf(out, "breakpoint_set=%lu\n", (unsigned long)slot);
}

static void debug_command_bpdel(char *cursor, FILE *out)
{
    char *slot_token = debug_next_token(&cursor);
    uint32_t slot;

    if (!slot_token || !debug_parse_u32(slot_token, &slot)) {
        debug_write_error(out, "usage: bpdel <slot>");
        return;
    }
    if (r36sx_cpu_debug_host_breakpoint_remove(slot) != 0) {
        debug_write_error(out, "breakpoint slot is empty or invalid");
        return;
    }
    debug_write_header(out);
    fprintf(out, "breakpoint_deleted=%lu\n", (unsigned long)slot);
}

static void debug_command_bplist(FILE *out)
{
    debug_write_header(out);
    for (uint32_t slot = 0;
         slot < R36SX_CPU_DEBUG_HOST_BREAKPOINT_SLOTS;
         slot++) {
        r36sx_cpu_debug_host_breakpoint_t bp;
        if (r36sx_cpu_debug_host_breakpoint_get(slot, &bp) == 0) {
            debug_write_breakpoint(out, slot, &bp);
        }
    }
}

static void debug_command_bpstatus(FILE *out)
{
    r36sx_cpu_debug_host_breakpoint_status_t status;

    r36sx_cpu_debug_host_breakpoint_status(&status);
    debug_write_header(out);
    fprintf(out,
            "paused=%u active=%u hit_slot=%ld hit_type=%s hit_cs:eip=%04X:%08lX hit_linear=%08lX hit_count=%llu\n",
            (unsigned)status.paused,
            (unsigned)status.active_count,
            (long)status.hit_slot,
            status.hit_linear ? "linear" : "csip",
            (unsigned)status.hit_cs,
            (unsigned long)status.hit_ip,
            (unsigned long)status.hit_linear_address,
            (unsigned long long)status.hit_count);
}

static void debug_command_bpclear(FILE *out)
{
    r36sx_cpu_debug_host_breakpoint_clear_all();
    debug_write_header(out);
    fprintf(out, "breakpoints_cleared=1\n");
}

static void debug_command_continue(FILE *out)
{
    r36sx_cpu_debug_host_breakpoint_continue();
    debug_write_header(out);
    fprintf(out, "continued=1\n");
}

static void debug_command_help(FILE *out)
{
    fprintf(out,
            "commands:\n"
            "  ping\n"
            "  regs\n"
            "  bp <linear|cs:eip>\n"
            "  bpset <slot> <linear|cs:eip>\n"
            "  bplist\n"
            "  bpdel <slot>\n"
            "  bpclear\n"
            "  bpstatus\n"
            "  cont\n"
            "  mem <address|seg:off> <length> [file]\n"
            "  vmem <linear|seg:off> <length> [file]\n"
            "  pagemap <linear|seg:off>\n"
            "  vram [offset] [length] [file]\n"
            "  screen [file]\n"
            "  key <scan byte> [scan byte...]\n"
            "  keydown <scan byte> [scan byte...]\n"
            "  keyup <scan byte> [scan byte...]\n"
            "  dump\n"
            "  stopdump\n"
            "  help\n");
}

static void debug_execute_command(char *command_text, FILE *out)
{
    char *cursor = debug_trim(command_text);
    char *command = debug_next_token(&cursor);

    if (!command) {
        debug_write_error(out, "empty command");
        return;
    }
    if (debug_str_eq(command, "ping")) {
        debug_write_header(out);
        fprintf(out, "pong\n");
    } else if (debug_str_eq(command, "regs")) {
        debug_write_header(out);
        debug_command_regs(out);
    } else if (debug_str_eq(command, "bp") ||
               debug_str_eq(command, "break")) {
        debug_command_bp(cursor, out);
    } else if (debug_str_eq(command, "bpset")) {
        debug_command_bpset(cursor, out);
    } else if (debug_str_eq(command, "bplist")) {
        debug_command_bplist(out);
    } else if (debug_str_eq(command, "bpdel")) {
        debug_command_bpdel(cursor, out);
    } else if (debug_str_eq(command, "bpclear")) {
        debug_command_bpclear(out);
    } else if (debug_str_eq(command, "bpstatus")) {
        debug_command_bpstatus(out);
    } else if (debug_str_eq(command, "cont") ||
               debug_str_eq(command, "continue")) {
        debug_command_continue(out);
    } else if (debug_str_eq(command, "mem")) {
        debug_command_mem(cursor, out);
    } else if (debug_str_eq(command, "vmem")) {
        debug_command_vmem(cursor, out);
    } else if (debug_str_eq(command, "pagemap")) {
        debug_command_pagemap(cursor, out);
    } else if (debug_str_eq(command, "vram")) {
        debug_command_vram(cursor, out);
    } else if (debug_str_eq(command, "screen")) {
        char *file_token = debug_next_token(&cursor);
        debug_dump_screen_file(file_token ? file_token : "debug_screen_rgb565.bin",
                               out);
    } else if (debug_str_eq(command, "key") ||
               debug_str_eq(command, "keydown") ||
               debug_str_eq(command, "keyup")) {
        debug_command_key(cursor, command, out);
    } else if (debug_str_eq(command, "dump")) {
        r36sx_memory_dump_request(0xdbu, "debug-control");
        debug_write_header(out);
        fprintf(out, "memory_dump_requested=1\n");
    } else if (debug_str_eq(command, "stopdump")) {
        r36sx_emergency_dump_request(0xdcu, "debug-control");
        debug_write_header(out);
        fprintf(out, "emergency_dump_requested=1\n");
    } else if (debug_str_eq(command, "help")) {
        debug_write_header(out);
        debug_command_help(out);
    } else {
        debug_write_error(out, "unknown command");
    }
}

void r36sx_debug_control_init(void)
{
    g_debug_control_enabled = r36sx_pico286_debug_control_enabled();
    debug_resolve_path(g_command_path, sizeof(g_command_path),
                       r36sx_pico286_debug_control_command_path(),
                       "pico_286_debug.cmd");
    debug_resolve_path(g_response_path, sizeof(g_response_path),
                       r36sx_pico286_debug_control_response_path(),
                       "pico_286_debug.out");
    debug_resolve_path(g_artifact_dir, sizeof(g_artifact_dir),
                       r36sx_pico286_debug_control_artifact_dir(),
                       ".");

    if (g_debug_control_enabled) {
        r36sx_pico286_debug_log(
            "debugctl: enabled cmd='%s' out='%s' artifacts='%s'",
            g_command_path, g_response_path, g_artifact_dir);
    } else {
        r36sx_pico286_debug_log("debugctl: disabled");
    }
}

void r36sx_debug_control_set_framebuffer(const uint16_t *pixels,
                                         uint32_t width,
                                         uint32_t height,
                                         uint32_t stride_pixels)
{
    g_framebuffer = pixels;
    g_framebuffer_width = width;
    g_framebuffer_height = height;
    g_framebuffer_stride = stride_pixels;
}

void r36sx_debug_control_poll(void)
{
    char command[R36SX_DEBUG_CONTROL_MAX_COMMAND];
    char tmp_response[576];
    FILE *cmd;
    FILE *out;
    size_t nread;
    int command_too_large;

    if (!g_debug_control_enabled) {
        return;
    }

    cmd = fopen(g_command_path, "rb");
    if (!cmd) {
        return;
    }
    nread = fread(command, 1, sizeof(command) - 1u, cmd);
    command[nread] = '\0';
    command_too_large = !feof(cmd);
    fclose(cmd);
    remove(g_command_path);

    snprintf(tmp_response, sizeof(tmp_response), "%s.tmp", g_response_path);
    out = fopen(tmp_response, "wb");
    if (!out) {
        r36sx_pico286_debug_log("debugctl: response open failed path='%s' errno=%d",
                                tmp_response, errno);
        return;
    }
    if (command_too_large) {
        debug_write_error(out, "command file too large");
    } else {
        debug_execute_command(command, out);
    }
    fclose(out);
    remove(g_response_path);
    if (rename(tmp_response, g_response_path) != 0) {
        r36sx_pico286_debug_log("debugctl: response rename failed '%s' -> '%s' errno=%d",
                                tmp_response, g_response_path, errno);
    }
}
