/*
 * Private host filesystem RPC device for the R36SX Pico-286 port.
 *
 * DOS-side redirectors/TSRs should own INT 2Fh.  This emulated "hardware"
 * device gives those guest programs a narrow host-file bridge without making
 * the emulator intercept DOS multiplex interrupts directly.
 */
#pragma once

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "findfirst.h"

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#ifndef mkdir
#define mkdir(path, mode) _mkdir(path)
#endif
#ifndef rmdir
#define rmdir(path) _rmdir(path)
#endif
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#ifndef S_IFDIR
#ifdef _S_IFDIR
#define S_IFDIR _S_IFDIR
#else
#define S_IFDIR 0040000
#endif
#endif

#define R36SX_HOST_RPC_PORT_BASE 0xE360u
#define R36SX_HOST_RPC_STREAM_PORT R36SX_HOST_RPC_PORT_BASE
#define R36SX_HOST_RPC_PORT_LAST R36SX_HOST_RPC_STREAM_PORT

#define R36SX_HOST_RPC_MAGIC 0x5248u /* "HR" little-endian in guest RAM. */
#define R36SX_HOST_RPC_VERSION 5u
#define R36SX_HOST_RPC_MAX_FILES 32u
#define R36SX_HOST_RPC_MAX_FINDS 16u
#define R36SX_HOST_RPC_MAX_PATH 260u
#define R36SX_HOST_RPC_MAX_HOST_PATH 512u
#define R36SX_HOST_RPC_FIND_RESULT_SIZE 20u
#define R36SX_HOST_RPC_FLAG_CREATE_NEW 0x0001u

#define R36SX_HOST_RPC_PROTO_CMD_FLAG 0x8000u
#define R36SX_HOST_RPC_PROTO_DATA_MASK 0x7fffu
#define R36SX_HOST_RPC_PROTO_ERR 0x7fffu
#define R36SX_HOST_RPC_PROTO_ERR_MISMATCH 0x7ffeu
#define R36SX_HOST_RPC_PROTO_ERR_TOO_LARGE 0x7ffdu
#define R36SX_HOST_RPC_FRAME_DATA_BITS 15u
#define R36SX_HOST_RPC_REQUEST_ADDR_CHUNKS 3u
#define R36SX_HOST_RPC_SERVICE_COMMANDS 10u

#if R36SX_DEBUG_HOSTRPC_TRACE
extern void r36sx_pico286_debug_log(const char *format, ...);
#define R36SX_HOSTRPC_LOG(...) r36sx_pico286_debug_log(__VA_ARGS__)
#else
#define R36SX_HOSTRPC_LOG(...) ((void)0)
#endif

#define R36SX_HOST_RPC_STATUS_IDLE 0x00u
#define R36SX_HOST_RPC_STATUS_DONE 0x01u
#define R36SX_HOST_RPC_STATUS_BAD_REQUEST 0x80u

typedef enum {
    R36SX_HOST_RPC_RESET = 0,
    R36SX_HOST_RPC_PING = 1,
    R36SX_HOST_RPC_CONTINUE = 2,
    R36SX_HOST_RPC_ABORT = 3,
    R36SX_HOST_RPC_END = 4,
    R36SX_HOST_RPC_CMD_OPEN_RO = R36SX_HOST_RPC_SERVICE_COMMANDS,
    R36SX_HOST_RPC_CMD_OPEN_RW = 11,
    R36SX_HOST_RPC_CMD_CREATE = 12,
    R36SX_HOST_RPC_CMD_CLOSE = 13,
    R36SX_HOST_RPC_CMD_READ = 14,
    R36SX_HOST_RPC_CMD_WRITE = 15,
    R36SX_HOST_RPC_CMD_DELETE = 16,
    R36SX_HOST_RPC_CMD_MKDIR = 17,
    R36SX_HOST_RPC_CMD_RMDIR = 18,
    R36SX_HOST_RPC_CMD_GETATTR = 19,
    R36SX_HOST_RPC_CMD_RENAME = 20,
    R36SX_HOST_RPC_CMD_COMMIT = 21,
    R36SX_HOST_RPC_CMD_FIND_FIRST = 22,
    R36SX_HOST_RPC_CMD_FIND_NEXT = 23,
    R36SX_HOST_RPC_CMD_FIND_CLOSE = 24,
    R36SX_HOST_RPC_CMD_CLOSE_ALL = 25,
    R36SX_HOST_RPC_CMD_CHDIR = 26,
} r36sx_host_rpc_command_t;

static const char *r36sx_host_rpc_command_name(uint16_t command)
{
    switch (command) {
        case R36SX_HOST_RPC_RESET: return "RESET";
        case R36SX_HOST_RPC_PING: return "PING";
        case R36SX_HOST_RPC_CONTINUE: return "CONTINUE";
        case R36SX_HOST_RPC_ABORT: return "ABORT";
        case R36SX_HOST_RPC_END: return "END";
        case R36SX_HOST_RPC_CMD_OPEN_RO: return "OPEN_RO";
        case R36SX_HOST_RPC_CMD_OPEN_RW: return "OPEN_RW";
        case R36SX_HOST_RPC_CMD_CREATE: return "CREATE";
        case R36SX_HOST_RPC_CMD_CLOSE: return "CLOSE";
        case R36SX_HOST_RPC_CMD_READ: return "READ";
        case R36SX_HOST_RPC_CMD_WRITE: return "WRITE";
        case R36SX_HOST_RPC_CMD_DELETE: return "DELETE";
        case R36SX_HOST_RPC_CMD_MKDIR: return "MKDIR";
        case R36SX_HOST_RPC_CMD_RMDIR: return "RMDIR";
        case R36SX_HOST_RPC_CMD_GETATTR: return "GETATTR";
        case R36SX_HOST_RPC_CMD_RENAME: return "RENAME";
        case R36SX_HOST_RPC_CMD_COMMIT: return "COMMIT";
        case R36SX_HOST_RPC_CMD_FIND_FIRST: return "FIND_FIRST";
        case R36SX_HOST_RPC_CMD_FIND_NEXT: return "FIND_NEXT";
        case R36SX_HOST_RPC_CMD_FIND_CLOSE: return "FIND_CLOSE";
        case R36SX_HOST_RPC_CMD_CLOSE_ALL: return "CLOSE_ALL";
        case R36SX_HOST_RPC_CMD_CHDIR: return "CHDIR";
        default: return "UNKNOWN";
    }
}

static int r36sx_host_rpc_is_request_command(uint16_t command)
{
    return command >= R36SX_HOST_RPC_CMD_OPEN_RO &&
           command <= R36SX_HOST_RPC_CMD_CHDIR;
}

typedef enum {
    R36SX_HOST_RPC_OK = 0,
    R36SX_HOST_RPC_ERR_BAD_REQUEST = 1,
    R36SX_HOST_RPC_ERR_BAD_ADDRESS = 2,
    R36SX_HOST_RPC_ERR_BAD_PATH = 3,
    R36SX_HOST_RPC_ERR_NO_FREE_HANDLE = 4,
    R36SX_HOST_RPC_ERR_BAD_HANDLE = 5,
    R36SX_HOST_RPC_ERR_HOST_IO = 6,
} r36sx_host_rpc_result_t;

typedef struct {
    uint16_t magic;
    uint16_t version;
    uint16_t command;
    uint16_t flags;
    uint32_t path_phys;
    uint32_t path2_phys;
    uint32_t data_phys;
    uint32_t data_len;
    uint32_t file_pos;
    uint32_t file_size;
    uint16_t handle;
    uint16_t mode;
    uint16_t attr;
    uint16_t dos_error;
    uint16_t result;
    uint16_t reserved;
    uint32_t bytes_done;
} r36sx_host_rpc_request_t;

static uint32_t r36sx_host_rpc_request_addr;
static uint8_t r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_IDLE;
static uint16_t r36sx_host_rpc_last_result = R36SX_HOST_RPC_OK;
static uint16_t r36sx_host_rpc_stream_sync_bit;
static uint16_t r36sx_host_rpc_stream_value = 'R';
static uint16_t r36sx_host_rpc_stream_command;
static uint8_t r36sx_host_rpc_stream_addr_chunks;
static uint8_t r36sx_host_rpc_stream_addr_bits;
static uint8_t r36sx_host_rpc_stream_fixed_request_done;
static uint32_t r36sx_host_rpc_stream_addr;
static FILE *r36sx_host_rpc_files[R36SX_HOST_RPC_MAX_FILES];
static intptr_t r36sx_host_rpc_finds[R36SX_HOST_RPC_MAX_FINDS];
static uint8_t r36sx_host_rpc_find_active[R36SX_HOST_RPC_MAX_FINDS];
static char r36sx_host_rpc_cwd[R36SX_HOST_RPC_MAX_PATH];

static inline int r36sx_host_rpc_ram_range_ok(uint32_t address, size_t bytes)
{
    return bytes == 0 ? address <= RAM_SIZE :
           address < RAM_SIZE && bytes <= (size_t)(RAM_SIZE - address);
}

static int r36sx_host_rpc_resize_file(FILE *fp, uint32_t size)
{
    if (!fp) {
        errno = EINVAL;
        return -1;
    }
    if (fflush(fp) != 0) {
        return -1;
    }
#ifdef _WIN32
    return _chsize_s(_fileno(fp), size) == 0 ? 0 : -1;
#else
    return ftruncate(fileno(fp), (off_t)size);
#endif
}

static inline uint16_t r36sx_host_rpc_read_u16(uint32_t address)
{
    return (uint16_t)RAM[address] |
           ((uint16_t)RAM[address + 1u] << 8);
}

static inline uint32_t r36sx_host_rpc_read_u32(uint32_t address)
{
    return (uint32_t)RAM[address] |
           ((uint32_t)RAM[address + 1u] << 8) |
           ((uint32_t)RAM[address + 2u] << 16) |
           ((uint32_t)RAM[address + 3u] << 24);
}

static inline void r36sx_host_rpc_write_u16(uint32_t address, uint16_t value)
{
    RAM[address] = (uint8_t)value;
    RAM[address + 1u] = (uint8_t)(value >> 8);
}

static inline void r36sx_host_rpc_write_u32(uint32_t address, uint32_t value)
{
    RAM[address] = (uint8_t)value;
    RAM[address + 1u] = (uint8_t)(value >> 8);
    RAM[address + 2u] = (uint8_t)(value >> 16);
    RAM[address + 3u] = (uint8_t)(value >> 24);
}

static int r36sx_host_rpc_load_request(uint32_t address,
                                       r36sx_host_rpc_request_t *req)
{
    if (!req || !r36sx_host_rpc_ram_range_ok(address, sizeof(*req))) {
        return 0;
    }
    req->magic = r36sx_host_rpc_read_u16(address + 0u);
    req->version = r36sx_host_rpc_read_u16(address + 2u);
    req->command = r36sx_host_rpc_read_u16(address + 4u);
    req->flags = r36sx_host_rpc_read_u16(address + 6u);
    req->path_phys = r36sx_host_rpc_read_u32(address + 8u);
    req->path2_phys = r36sx_host_rpc_read_u32(address + 12u);
    req->data_phys = r36sx_host_rpc_read_u32(address + 16u);
    req->data_len = r36sx_host_rpc_read_u32(address + 20u);
    req->file_pos = r36sx_host_rpc_read_u32(address + 24u);
    req->file_size = r36sx_host_rpc_read_u32(address + 28u);
    req->handle = r36sx_host_rpc_read_u16(address + 32u);
    req->mode = r36sx_host_rpc_read_u16(address + 34u);
    req->attr = r36sx_host_rpc_read_u16(address + 36u);
    req->dos_error = r36sx_host_rpc_read_u16(address + 38u);
    req->result = r36sx_host_rpc_read_u16(address + 40u);
    req->reserved = r36sx_host_rpc_read_u16(address + 42u);
    req->bytes_done = r36sx_host_rpc_read_u32(address + 44u);
    return 1;
}

static void r36sx_host_rpc_store_response(uint32_t address,
                                          const r36sx_host_rpc_request_t *req)
{
    if (!req || !r36sx_host_rpc_ram_range_ok(address, sizeof(*req))) {
        return;
    }
    r36sx_host_rpc_write_u32(address + 20u, req->data_len);
    r36sx_host_rpc_write_u32(address + 24u, req->file_pos);
    r36sx_host_rpc_write_u32(address + 28u, req->file_size);
    r36sx_host_rpc_write_u16(address + 32u, req->handle);
    r36sx_host_rpc_write_u16(address + 36u, req->attr);
    r36sx_host_rpc_write_u16(address + 38u, req->dos_error);
    r36sx_host_rpc_write_u16(address + 40u, req->result);
    r36sx_host_rpc_write_u16(address + 42u, req->reserved);
    r36sx_host_rpc_write_u32(address + 44u, req->bytes_done);
}

static uint16_t r36sx_host_rpc_dos_error_from_errno(int err, int writing)
{
    switch (err) {
        case 0:
            return writing ? 0x1D : 0x1E; /* write/read fault */
        case ENOENT:
            return 2; /* file not found */
        case ENOTDIR:
            return 3; /* path not found */
        case EMFILE:
        case ENFILE:
            return 4; /* too many open files */
        case EACCES:
        case EEXIST:
        case EISDIR:
#ifdef ENOTEMPTY
        case ENOTEMPTY:
#endif
        case EPERM:
            return 5; /* access denied */
        case EBADF:
            return 6; /* invalid handle */
        case EINVAL:
            return 13; /* invalid data */
        case EROFS:
            return 19; /* write protected */
        case ENOSPC:
            return 0x27; /* disk full */
        case EIO:
        default:
            return writing ? 0x1D : 0x1E;
    }
}

static int r36sx_host_rpc_guest_string(uint32_t address, char *out,
                                       size_t out_size)
{
    size_t i;
    if (!out || out_size == 0 ||
        !r36sx_host_rpc_ram_range_ok(address, 1)) {
        return 0;
    }
    for (i = 0; i + 1 < out_size; ++i) {
        if (!r36sx_host_rpc_ram_range_ok(address + (uint32_t)i, 1)) {
            return 0;
        }
        out[i] = (char)RAM[address + (uint32_t)i];
        if (out[i] == '\0') {
            return 1;
        }
    }
    out[out_size - 1] = '\0';
    return 1;
}

static int r36sx_host_rpc_append_segment(char *tail, size_t tail_size,
                                         const char *segment)
{
    size_t len;
    size_t segment_len;
    if (!tail || !segment || !segment[0] || tail_size == 0) {
        return 0;
    }
    len = strlen(tail);
    segment_len = strlen(segment);
    if (len > 0) {
        if (len + 1u + segment_len >= tail_size) {
            return 0;
        }
        tail[len++] = '\\';
    } else if (segment_len >= tail_size) {
        return 0;
    }
    memcpy(tail + len, segment, segment_len + 1u);
    return 1;
}

static int r36sx_host_rpc_pop_segment(char *tail)
{
    char *slash;
    if (!tail || tail[0] == '\0') {
        return 0;
    }
    slash = strrchr(tail, '\\');
    if (!slash) {
        tail[0] = '\0';
        return 1;
    }
    *slash = '\0';
    return 1;
}

static int r36sx_host_rpc_dir_exists(const char *path)
{
    struct stat st;

    return path && path[0] && stat(path, &st) == 0 && (st.st_mode & S_IFDIR);
}

static void r36sx_host_rpc_normalize_host_separators(char *path)
{
    if (!path) {
        return;
    }
    for (char *p = path; *p; ++p) {
#ifdef _WIN32
        if (*p == '/') {
            *p = '\\';
        }
#else
        if (*p == '\\') {
            *p = '/';
        }
#endif
    }
}

static void r36sx_host_rpc_trim_trailing_separator(char *path)
{
    size_t len;
    if (!path) {
        return;
    }
    len = strlen(path);
    while (len > 1u && (path[len - 1u] == '\\' || path[len - 1u] == '/')) {
#ifdef _WIN32
        if (len == 3u && path[1] == ':') {
            break;
        }
#endif
        path[--len] = '\0';
    }
}

static int r36sx_host_rpc_absolute_path(const char *path, char *out,
                                        size_t out_size)
{
    if (!path || !path[0] || !out || out_size == 0) {
        return 0;
    }
#ifdef _WIN32
    if (!_fullpath(out, path, out_size)) {
        return 0;
    }
#else
    if (!realpath(path, out)) {
        return 0;
    }
#endif
    out[out_size - 1u] = '\0';
    r36sx_host_rpc_normalize_host_separators(out);
    r36sx_host_rpc_trim_trailing_separator(out);
    return out[0] != '\0';
}

static int r36sx_host_rpc_path_has_root_prefix(const char *root,
                                               const char *path)
{
    size_t root_len;
    size_t path_len;
    if (!root || !path) {
        return 0;
    }
    root_len = strlen(root);
    path_len = strlen(path);
    if (root_len == 0 || path_len < root_len) {
        return 0;
    }
    for (size_t i = 0; i < root_len; ++i) {
        unsigned char a = (unsigned char)root[i];
        unsigned char b = (unsigned char)path[i];
#ifdef _WIN32
        a = (unsigned char)tolower(a);
        b = (unsigned char)tolower(b);
#endif
        if (a != b) {
            return 0;
        }
    }
    return path[root_len] == '\0' ||
           path[root_len] == '\\' ||
           path[root_len] == '/';
}

static int r36sx_host_rpc_ensure_base_dir(const char *path)
{
    char temp[R36SX_HOST_RPC_MAX_HOST_PATH];
    size_t len;

    if (!path || !path[0]) {
        return 0;
    }
    snprintf(temp, sizeof(temp), "%s", path);
    temp[sizeof(temp) - 1u] = '\0';

    len = strlen(temp);
    while (len > 1u && (temp[len - 1u] == '\\' || temp[len - 1u] == '/')) {
        temp[--len] = '\0';
    }
    if (r36sx_host_rpc_dir_exists(temp)) {
        return 1;
    }

    /*
     * Create only the configured host-drive root automatically.  Guest-visible
     * subdirectories are still created by explicit DOS MKDIR requests.
     */
    for (size_t i = 0; i < len; ++i) {
        char saved;

        if (temp[i] != '\\' && temp[i] != '/') {
            continue;
        }
        if (i == 0u || (i == 2u && temp[1] == ':')) {
            continue;
        }

        saved = temp[i];
        temp[i] = '\0';
        if (!r36sx_host_rpc_dir_exists(temp) &&
            mkdir(temp, 0777) != 0 && errno != EEXIST) {
            temp[i] = saved;
            return 0;
        }
        temp[i] = saved;
    }

    if (!r36sx_host_rpc_dir_exists(temp) &&
        mkdir(temp, 0777) != 0 && errno != EEXIST) {
        return 0;
    }
    return r36sx_host_rpc_dir_exists(temp);
}

static int r36sx_host_rpc_build_host_path_ex(const char *guest_path,
                                             char *host_path,
                                             size_t host_path_size,
                                             char *normalized_tail,
                                             size_t normalized_tail_size)
{
    const char *base = r36sx_pico286_host_drive_path();
#if R36SX_DEBUG_HOSTRPC_TRACE
    const char *guest_path_start = guest_path;
#endif
    const char *path;
    char base_abs[R36SX_HOST_RPC_MAX_HOST_PATH];
    char tail[R36SX_HOST_RPC_MAX_PATH];
    char segment[R36SX_HOST_RPC_MAX_PATH];
    size_t seg_len = 0;
    int written;
    int absolute = 0;
    char sep =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

    if (!guest_path || !host_path || host_path_size == 0) {
        return 0;
    }
    if (!base || !base[0]) {
        base = "host";
    }
    if (!r36sx_host_rpc_ensure_base_dir(base)) {
        return 0;
    }
    if (!r36sx_host_rpc_absolute_path(base, base_abs, sizeof(base_abs))) {
        return 0;
    }

    /*
     * Drop an optional DOS drive prefix.  "X:\foo" is absolute on the mapped
     * drive, while "X:foo" follows that drive's current directory.
     */
    path = guest_path;
    if (isalpha((unsigned char)path[0]) && path[1] == ':') {
        path += 2;
    }
    if (*path == '\\' || *path == '/') {
        absolute = 1;
    }
    while (*path == '\\' || *path == '/') {
        ++path;
    }

    if (!absolute && r36sx_host_rpc_cwd[0]) {
        snprintf(tail, sizeof(tail), "%s", r36sx_host_rpc_cwd);
        tail[sizeof(tail) - 1u] = '\0';
    } else {
        tail[0] = '\0';
    }
    while (1) {
        char ch = *path++;
        if (ch == '\\' || ch == '/' || ch == '\0') {
            segment[seg_len] = '\0';
            if (seg_len == 0 || strcmp(segment, ".") == 0) {
                /* Ignore empty and current-directory segments. */
            } else if (strcmp(segment, "..") == 0) {
                /*
                 * DOS-style parent traversal is allowed inside the mapped root,
                 * but trying to climb above host_drive_path is a bad path, not
                 * a silent clamp to root.
                 */
                if (!r36sx_host_rpc_pop_segment(tail)) {
                    R36SX_HOSTRPC_LOG(
                        "hostrpc: reject path above root guest='%s'",
                        guest_path_start);
                    return 0;
                }
            } else {
                if (strchr(segment, ':') ||
                    !r36sx_host_rpc_append_segment(tail, sizeof(tail),
                                                   segment)) {
                    return 0;
                }
            }
            seg_len = 0;
            if (ch == '\0') {
                break;
            }
            continue;
        }
        if (seg_len + 1 >= sizeof(segment)) {
            return 0;
        }
        segment[seg_len++] = ch;
    }

    if (normalized_tail && normalized_tail_size > 0) {
        snprintf(normalized_tail, normalized_tail_size, "%s", tail);
        normalized_tail[normalized_tail_size - 1u] = '\0';
    }

    if (tail[0] == '\0') {
        written = snprintf(host_path, host_path_size, "%s", base_abs);
    } else {
        size_t base_len = strlen(base_abs);
        if (base_len > 0 &&
            (base_abs[base_len - 1] == '\\' ||
             base_abs[base_len - 1] == '/')) {
            written = snprintf(host_path, host_path_size, "%s%s", base_abs,
                               tail);
        } else {
            written = snprintf(host_path, host_path_size, "%s%c%s", base_abs,
                               sep, tail);
        }
    }
    if (written < 0 || (size_t)written >= host_path_size) {
        return 0;
    }
    host_path[host_path_size - 1] = '\0';
    r36sx_host_rpc_normalize_host_separators(host_path);
    if (!r36sx_host_rpc_path_has_root_prefix(base_abs, host_path)) {
        R36SX_HOSTRPC_LOG("hostrpc: reject escaped host path root='%s' path='%s'",
                          base_abs,
                          host_path);
        return 0;
    }
    return 1;
}

static int r36sx_host_rpc_build_host_path(const char *guest_path,
                                          char *host_path,
                                          size_t host_path_size)
{
    return r36sx_host_rpc_build_host_path_ex(guest_path, host_path,
                                             host_path_size, NULL, 0);
}

static int r36sx_host_rpc_path_has_wildcard(const char *path)
{
    return path && (strchr(path, '*') || strchr(path, '?'));
}

static int r36sx_host_rpc_make_find_pattern(char *host_path,
                                            size_t host_path_size)
{
    struct stat st;
    size_t len;
    char sep =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

    if (!host_path || host_path_size == 0 ||
        r36sx_host_rpc_path_has_wildcard(host_path)) {
        return 1;
    }

    /*
     * DOS find-first on a directory means "enumerate this directory".
     * Host CRT _findfirst("host") instead returns the directory entry named
     * HOST, which looked like a self-recursive HOST\HOST\HOST path in VC.
     */
    if (stat(host_path, &st) != 0 || !(st.st_mode & S_IFDIR)) {
        return 1;
    }

    len = strlen(host_path);
    if (len + 2u >= host_path_size) {
        return 0;
    }
    if (len > 0 && host_path[len - 1u] != '\\' && host_path[len - 1u] != '/') {
        host_path[len++] = sep;
    }
    host_path[len++] = '*';
    host_path[len] = '\0';
    return 1;
}

static int r36sx_host_rpc_free_handle(void)
{
    for (unsigned i = 0; i < R36SX_HOST_RPC_MAX_FILES; ++i) {
        if (!r36sx_host_rpc_files[i]) {
            return (int)i;
        }
    }
    return -1;
}

static int r36sx_host_rpc_free_find_handle(void)
{
    for (unsigned i = 0; i < R36SX_HOST_RPC_MAX_FINDS; ++i) {
        if (!r36sx_host_rpc_find_active[i]) {
            return (int)i;
        }
    }
    return -1;
}

static void r36sx_host_rpc_close_find_handle(uint16_t handle)
{
    if (handle >= R36SX_HOST_RPC_MAX_FINDS ||
        !r36sx_host_rpc_find_active[handle]) {
        return;
    }
    _findclose(r36sx_host_rpc_finds[handle]);
    r36sx_host_rpc_finds[handle] = 0;
    r36sx_host_rpc_find_active[handle] = 0;
}

static uint32_t r36sx_host_rpc_close_all_handles(void)
{
    uint32_t closed = 0;

    /*
     * DOS redirector callback 111Dh is an abort-style cleanup request.  Since
     * the current HOSTRPC protocol does not carry PSP ownership, close every
     * host-side resource owned by this single-drive redirector and leave no
     * stale FILE pointers or find handles behind after interrupted copies.
     */
    for (unsigned i = 0; i < R36SX_HOST_RPC_MAX_FILES; ++i) {
        if (!r36sx_host_rpc_files[i]) {
            continue;
        }
        errno = 0;
        if (fclose(r36sx_host_rpc_files[i]) != 0) {
            R36SX_HOSTRPC_LOG("hostrpc: close_all file=%u fclose errno=%d",
                              i, errno ? errno : EIO);
        }
        r36sx_host_rpc_files[i] = NULL;
        ++closed;
    }

    for (unsigned i = 0; i < R36SX_HOST_RPC_MAX_FINDS; ++i) {
        if (!r36sx_host_rpc_find_active[i]) {
            continue;
        }
        r36sx_host_rpc_close_find_handle((uint16_t)i);
        ++closed;
    }
    return closed;
}

static void r36sx_host_rpc_to_dos_name(const char *input, uint8_t *output)
{
    int i;
    int j;

    if (!input || !output) {
        return;
    }
    memset(output, ' ', 11);
    if (strcmp(input, ".") == 0) {
        output[0] = '.';
        return;
    }
    if (strcmp(input, "..") == 0) {
        output[0] = '.';
        output[1] = '.';
        return;
    }

    for (i = 0, j = 0; input[i] && input[i] != '.' && j < 8; ++i) {
        if (input[i] != ' ') {
            output[j++] = (uint8_t)toupper((unsigned char)input[i]);
        }
    }
    while (input[i] && input[i] != '.') {
        ++i;
    }
    if (input[i] == '.') {
        ++i;
        for (j = 8; input[i] && j < 11; ++i) {
            if (input[i] != ' ') {
                output[j++] = (uint8_t)toupper((unsigned char)input[i]);
            }
        }
    }
}

static void r36sx_host_rpc_pack_dos_timestamp(time_t timestamp,
                                              uint16_t *dos_time,
                                              uint16_t *dos_date)
{
    struct tm value;
    struct tm *tm_value;
    int year;

    if (!dos_time || !dos_date) {
        return;
    }

#ifdef _WIN32
    tm_value = localtime(&timestamp);
    if (tm_value) {
        value = *tm_value;
    }
#else
    tm_value = localtime_r(&timestamp, &value);
#endif
    if (!tm_value) {
        value.tm_year = 80;
        value.tm_mon = 0;
        value.tm_mday = 1;
        value.tm_hour = 0;
        value.tm_min = 0;
        value.tm_sec = 0;
    }

    year = value.tm_year + 1900;
    if (year < 1980) {
        year = 1980;
    } else if (year > 2107) {
        year = 2107;
    }

    /*
     * DOS packs local time as hhhhh mmmmmm sssss, with seconds counted in
     * two-second units, and dates as yyyyyyy mmmm ddddd from year 1980.
     */
    *dos_time = (uint16_t)(((value.tm_hour & 0x1F) << 11) |
                           ((value.tm_min & 0x3F) << 5) |
                           ((value.tm_sec / 2) & 0x1F));
    *dos_date = (uint16_t)((((year - 1980) & 0x7F) << 9) |
                           (((value.tm_mon + 1) & 0x0F) << 5) |
                           (value.tm_mday & 0x1F));
}

static uint16_t r36sx_host_rpc_attr_from_stat(const struct stat *st)
{
    uint16_t attr;

    if (!st) {
        return 0x20u;
    }

    attr = (st->st_mode & S_IFDIR) ? 0x10u : 0x20u;
#ifdef S_IWUSR
    if (!(st->st_mode & S_IWUSR)) {
        attr |= 0x01u;
    }
#endif
    return attr;
}

static void r36sx_host_rpc_apply_stat_metadata(r36sx_host_rpc_request_t *req,
                                               const struct stat *st)
{
    uint16_t dos_time;
    uint16_t dos_date;

    if (!req || !st) {
        return;
    }
    req->file_size = (uint32_t)st->st_size;
    req->attr = r36sx_host_rpc_attr_from_stat(st);
    r36sx_host_rpc_pack_dos_timestamp(st->st_mtime, &dos_time, &dos_date);
    req->reserved = dos_time;
    req->bytes_done = dos_date;
}

static int r36sx_host_rpc_store_find_result(uint32_t address,
                                            uint32_t bytes,
                                            const struct _finddata_t *fileinfo)
{
    uint16_t attr;
    uint16_t dos_time;
    uint16_t dos_date;
    uint32_t size;

    if (!fileinfo || bytes < R36SX_HOST_RPC_FIND_RESULT_SIZE ||
        !r36sx_host_rpc_ram_range_ok(address,
                                     R36SX_HOST_RPC_FIND_RESULT_SIZE)) {
        return 0;
    }

    /*
     * The guest redirector wants the same compact data that DOS puts into
     * the DTA: an 11-byte 8.3 name, attribute byte, time, date, and size.
     */
    r36sx_host_rpc_to_dos_name(fileinfo->name, &RAM[address]);
    attr = (uint16_t)(fileinfo->attrib & 0xffu);
    size = (uint32_t)fileinfo->size;
    r36sx_host_rpc_pack_dos_timestamp(fileinfo->time_write, &dos_time,
                                      &dos_date);
    RAM[address + 11u] = (uint8_t)attr;
    r36sx_host_rpc_write_u16(address + 12u, dos_time);
    r36sx_host_rpc_write_u16(address + 14u, dos_date);
    r36sx_host_rpc_write_u32(address + 16u, size);
    return 1;
}

static void r36sx_host_rpc_trace_buffer(const char *label,
                                        uint32_t address,
                                        uint32_t bytes)
{
#if R36SX_DEBUG_HOSTRPC_TRACE
    char hex[16u * 3u + 1u];
    uint32_t preview = bytes < 16u ? bytes : 16u;
    uint32_t pos = 0;

    if (!label || !r36sx_host_rpc_ram_range_ok(address, preview)) {
        R36SX_HOSTRPC_LOG("hostrpc: %s data addr=%05lx len=%lu unavailable",
                          label ? label : "buffer",
                          (unsigned long)address,
                          (unsigned long)bytes);
        return;
    }

    for (uint32_t i = 0; i < preview && pos + 3u < sizeof(hex); ++i) {
        static const char digits[] = "0123456789ABCDEF";
        uint8_t value = RAM[address + i];
        hex[pos++] = digits[value >> 4];
        hex[pos++] = digits[value & 0x0Fu];
        hex[pos++] = ' ';
    }
    if (pos > 0) {
        hex[pos - 1u] = '\0';
    } else {
        hex[0] = '\0';
    }
    R36SX_HOSTRPC_LOG("hostrpc: %s data addr=%05lx len=%lu preview=%s%s",
                      label,
                      (unsigned long)address,
                      (unsigned long)bytes,
                      hex,
                      bytes > preview ? " ..." : "");
#else
    (void)label;
    (void)address;
    (void)bytes;
#endif
}

static void r36sx_host_rpc_finish(r36sx_host_rpc_request_t *req,
                                  uint16_t result, uint16_t dos_error)
{
    req->result = result;
    req->dos_error = dos_error;
    r36sx_host_rpc_last_result = result;
}

static void r36sx_host_rpc_execute_request(uint16_t command)
{
    r36sx_host_rpc_request_t req;
    char guest_path[R36SX_HOST_RPC_MAX_PATH];
    char host_path[R36SX_HOST_RPC_MAX_HOST_PATH];
    int err = 0;

    r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_BAD_REQUEST;
    r36sx_host_rpc_last_result = R36SX_HOST_RPC_ERR_BAD_REQUEST;

    if (!r36sx_host_rpc_load_request(r36sx_host_rpc_request_addr, &req) ||
        req.magic != R36SX_HOST_RPC_MAGIC ||
        req.version != R36SX_HOST_RPC_VERSION) {
        R36SX_HOSTRPC_LOG("hostrpc: bad request addr=%05lx",
                          (unsigned long)r36sx_host_rpc_request_addr);
        return;
    }

    req.result = R36SX_HOST_RPC_OK;
    req.dos_error = 0;
    req.bytes_done = 0;
    req.command = command;

    R36SX_HOSTRPC_LOG(
        "hostrpc: req cmd=%s(%u) addr=%05lx path=%05lx path2=%05lx data=%05lx len=%lu pos=%lu size=%lu handle=%u mode=%u attr=%04x flags=%04x",
        r36sx_host_rpc_command_name(req.command),
        (unsigned)req.command,
        (unsigned long)r36sx_host_rpc_request_addr,
        (unsigned long)req.path_phys,
        (unsigned long)req.path2_phys,
        (unsigned long)req.data_phys,
        (unsigned long)req.data_len,
        (unsigned long)req.file_pos,
        (unsigned long)req.file_size,
        (unsigned)req.handle,
        (unsigned)req.mode,
        (unsigned)req.attr,
        (unsigned)req.flags);

    switch (req.command) {
        case R36SX_HOST_RPC_CMD_OPEN_RO:
        case R36SX_HOST_RPC_CMD_OPEN_RW:
        case R36SX_HOST_RPC_CMD_CREATE: {
            const char *mode;
            int handle;
            FILE *fp;
            struct stat st;
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_build_host_path(guest_path, host_path,
                                                sizeof(host_path))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            R36SX_HOSTRPC_LOG("hostrpc: path cmd=%s guest='%s' host='%s'",
                              r36sx_host_rpc_command_name(req.command),
                              guest_path,
                              host_path);
            handle = r36sx_host_rpc_free_handle();
            if (handle < 0) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_NO_FREE_HANDLE,
                                      4);
                break;
            }
            if (req.command == R36SX_HOST_RPC_CMD_CREATE &&
                (req.flags & R36SX_HOST_RPC_FLAG_CREATE_NEW) &&
                stat(host_path, &st) == 0) {
                R36SX_HOSTRPC_LOG(
                    "hostrpc: create-new exists host='%s' attr=%04x",
                    host_path,
                    (unsigned)req.attr);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      0x50u); /* file already exists */
                break;
            }
            mode = req.command == R36SX_HOST_RPC_CMD_OPEN_RO ? "rb" :
                   req.command == R36SX_HOST_RPC_CMD_OPEN_RW ? "rb+" : "wb+";
            errno = 0;
            fp = fopen(host_path, mode);
            if (!fp) {
                err = errno ? errno : EIO;
                R36SX_HOSTRPC_LOG(
                    "hostrpc: open/create failed cmd=%s host='%s' errno=%d",
                    r36sx_host_rpc_command_name(req.command),
                    host_path,
                    err);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err, 0));
                break;
            }
            r36sx_host_rpc_files[handle] = fp;
            req.handle = (uint16_t)handle;
            if (fseek(fp, 0, SEEK_END) == 0) {
                long size = ftell(fp);
                req.file_size = size > 0 ? (uint32_t)size : 0u;
                fseek(fp, 0, SEEK_SET);
            }
            if (stat(host_path, &st) == 0) {
                r36sx_host_rpc_apply_stat_metadata(&req, &st);
            } else if (req.command == R36SX_HOST_RPC_CMD_CREATE) {
                req.attr = 0x20u;
            }
            R36SX_HOSTRPC_LOG(
                "hostrpc: open/create ok cmd=%s host='%s' handle=%u size=%lu attr=%04x time=%04x date=%04lx mode=%04x flags=%04x",
                r36sx_host_rpc_command_name(req.command),
                host_path,
                (unsigned)req.handle,
                (unsigned long)req.file_size,
                (unsigned)req.attr,
                (unsigned)req.reserved,
                (unsigned long)(req.bytes_done & 0xffffu),
                (unsigned)req.mode,
                (unsigned)req.flags);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_CLOSE:
            if (req.handle >= R36SX_HOST_RPC_MAX_FILES ||
                !r36sx_host_rpc_files[req.handle]) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_HANDLE, 6);
                break;
            }
            errno = 0;
            if (fclose(r36sx_host_rpc_files[req.handle]) != 0) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_files[req.handle] = NULL;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err, 1));
                break;
            }
            r36sx_host_rpc_files[req.handle] = NULL;
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;

        case R36SX_HOST_RPC_CMD_CLOSE_ALL:
            req.bytes_done = r36sx_host_rpc_close_all_handles();
            R36SX_HOSTRPC_LOG("hostrpc: close_all closed=%lu",
                              (unsigned long)req.bytes_done);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;

        case R36SX_HOST_RPC_CMD_COMMIT:
            if (req.handle >= R36SX_HOST_RPC_MAX_FILES ||
                !r36sx_host_rpc_files[req.handle]) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_HANDLE, 6);
                break;
            }
            errno = 0;
            if (fflush(r36sx_host_rpc_files[req.handle]) != 0) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err,
                                                                          1));
                break;
            }
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;

        case R36SX_HOST_RPC_CMD_READ:
        case R36SX_HOST_RPC_CMD_WRITE: {
            FILE *fp;
            size_t done;
            int writing = req.command == R36SX_HOST_RPC_CMD_WRITE;
            if (req.handle >= R36SX_HOST_RPC_MAX_FILES ||
                !r36sx_host_rpc_files[req.handle]) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_HANDLE, 6);
                break;
            }
            if (req.data_len != 0 &&
                !r36sx_host_rpc_ram_range_ok(req.data_phys, req.data_len)) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_ADDRESS, 8);
                break;
            }
            if (writing && req.data_len != 0) {
                r36sx_host_rpc_trace_buffer("write-in", req.data_phys,
                                            req.data_len);
            }
            fp = r36sx_host_rpc_files[req.handle];
            errno = 0;
            clearerr(fp);
            if (fseek(fp, (long)req.file_pos, SEEK_SET) != 0) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err,
                                                                          writing));
                break;
            }
            if (writing && req.data_len == 0) {
                if (r36sx_host_rpc_resize_file(fp, req.file_pos) != 0) {
                    err = errno ? errno : EIO;
                    r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                          r36sx_host_rpc_dos_error_from_errno(
                                              err, 1));
                    break;
                }
                done = 0;
                req.file_size = req.file_pos;
            } else if (writing) {
                done = fwrite(&RAM[req.data_phys], 1, req.data_len, fp);
                fflush(fp);
            } else {
                done = fread(&RAM[req.data_phys], 1, req.data_len, fp);
            }
            if (!writing && req.data_len != 0) {
                r36sx_host_rpc_trace_buffer("read-out", req.data_phys,
                                            (uint32_t)done);
            }
            req.bytes_done = (uint32_t)done;
            req.file_pos += (uint32_t)done;
            if (writing && req.file_pos > req.file_size) {
                req.file_size = req.file_pos;
            }
            if ((uint32_t)done != req.data_len && ferror(fp)) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err,
                                                                          writing));
                break;
            }
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_RENAME: {
            char guest_path2[R36SX_HOST_RPC_MAX_PATH];
            char host_path2[R36SX_HOST_RPC_MAX_HOST_PATH];
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_guest_string(req.path2_phys, guest_path2,
                                             sizeof(guest_path2)) ||
                !r36sx_host_rpc_build_host_path(guest_path, host_path,
                                                sizeof(host_path)) ||
                !r36sx_host_rpc_build_host_path(guest_path2, host_path2,
                                                sizeof(host_path2))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            R36SX_HOSTRPC_LOG(
                "hostrpc: rename guest='%s' -> '%s' host='%s' -> '%s'",
                guest_path,
                guest_path2,
                host_path,
                host_path2);
            errno = 0;
            if (rename(host_path, host_path2) != 0) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err,
                                                                          1));
                break;
            }
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_CHDIR: {
            struct stat st;
            char normalized_tail[R36SX_HOST_RPC_MAX_PATH];
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_build_host_path_ex(guest_path, host_path,
                                                   sizeof(host_path),
                                                   normalized_tail,
                                                   sizeof(normalized_tail))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            R36SX_HOSTRPC_LOG("hostrpc: chdir guest='%s' host='%s' tail='%s'",
                              guest_path,
                              host_path,
                              normalized_tail);
            errno = 0;
            if (stat(host_path, &st) != 0 || !(st.st_mode & S_IFDIR)) {
                err = errno ? errno : ENOTDIR;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(err,
                                                                          0));
                break;
            }
            snprintf(r36sx_host_rpc_cwd, sizeof(r36sx_host_rpc_cwd), "%s",
                     normalized_tail);
            r36sx_host_rpc_cwd[sizeof(r36sx_host_rpc_cwd) - 1u] = '\0';
            r36sx_host_rpc_apply_stat_metadata(&req, &st);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_DELETE:
        case R36SX_HOST_RPC_CMD_MKDIR:
        case R36SX_HOST_RPC_CMD_RMDIR:
        case R36SX_HOST_RPC_CMD_GETATTR: {
            struct stat st;
            int rc = 0;
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_build_host_path(guest_path, host_path,
                                                sizeof(host_path))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            R36SX_HOSTRPC_LOG("hostrpc: path cmd=%s guest='%s' host='%s'",
                              r36sx_host_rpc_command_name(req.command),
                              guest_path,
                              host_path);
            errno = 0;
            if (req.command == R36SX_HOST_RPC_CMD_DELETE) {
                rc = remove(host_path);
            } else if (req.command == R36SX_HOST_RPC_CMD_MKDIR) {
                rc = mkdir(host_path, 0777);
            } else if (req.command == R36SX_HOST_RPC_CMD_RMDIR) {
                rc = rmdir(host_path);
            } else {
                rc = stat(host_path, &st);
                if (rc == 0) {
                    r36sx_host_rpc_apply_stat_metadata(&req, &st);
                }
            }
            if (rc != 0) {
                err = errno ? errno : EIO;
                R36SX_HOSTRPC_LOG(
                    "hostrpc: path cmd=%s failed host='%s' errno=%d",
                    r36sx_host_rpc_command_name(req.command),
                    host_path,
                    err);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(
                                          err, req.command !=
                                                   R36SX_HOST_RPC_CMD_GETATTR));
                break;
            }
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_FIND_FIRST: {
            struct _finddata_t fileinfo;
            intptr_t host_find;
            int handle;
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_build_host_path(guest_path, host_path,
                                                sizeof(host_path)) ||
                !r36sx_host_rpc_make_find_pattern(host_path,
                                                  sizeof(host_path))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            R36SX_HOSTRPC_LOG("hostrpc: find_first guest='%s' pattern='%s'",
                              guest_path,
                              host_path);
            handle = r36sx_host_rpc_free_find_handle();
            if (handle < 0) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_NO_FREE_HANDLE,
                                      4);
                break;
            }
            errno = 0;
            host_find = _findfirst(host_path, &fileinfo);
            if (host_find == (intptr_t)-1) {
                R36SX_HOSTRPC_LOG("hostrpc: find_first no_match pattern='%s'",
                                  host_path);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO, 18);
                break;
            }
            if (!r36sx_host_rpc_store_find_result(req.data_phys, req.data_len,
                                                  &fileinfo)) {
                _findclose(host_find);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_ADDRESS, 8);
                break;
            }
            r36sx_host_rpc_finds[handle] = host_find;
            r36sx_host_rpc_find_active[handle] = 1;
            req.handle = (uint16_t)handle;
            req.attr = (uint16_t)(fileinfo.attrib & 0xffu);
            req.file_size = (uint32_t)fileinfo.size;
            req.bytes_done = R36SX_HOST_RPC_FIND_RESULT_SIZE;
            R36SX_HOSTRPC_LOG(
                "hostrpc: find_first ok handle=%u name='%s' attr=%02x size=%lu",
                (unsigned)req.handle,
                fileinfo.name,
                (unsigned)req.attr,
                (unsigned long)req.file_size);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_FIND_NEXT: {
            struct _finddata_t fileinfo;
            if (req.handle >= R36SX_HOST_RPC_MAX_FINDS ||
                !r36sx_host_rpc_find_active[req.handle]) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_HANDLE, 6);
                break;
            }
            errno = 0;
            if (_findnext(r36sx_host_rpc_finds[req.handle], &fileinfo) != 0) {
                r36sx_host_rpc_close_find_handle(req.handle);
                R36SX_HOSTRPC_LOG("hostrpc: find_next done handle=%u",
                                  (unsigned)req.handle);
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO, 18);
                break;
            }
            if (!r36sx_host_rpc_store_find_result(req.data_phys, req.data_len,
                                                  &fileinfo)) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_ADDRESS, 8);
                break;
            }
            req.attr = (uint16_t)(fileinfo.attrib & 0xffu);
            req.file_size = (uint32_t)fileinfo.size;
            req.bytes_done = R36SX_HOST_RPC_FIND_RESULT_SIZE;
            R36SX_HOSTRPC_LOG(
                "hostrpc: find_next ok handle=%u name='%s' attr=%02x size=%lu",
                (unsigned)req.handle,
                fileinfo.name,
                (unsigned)req.attr,
                (unsigned long)req.file_size);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        case R36SX_HOST_RPC_CMD_FIND_CLOSE:
            if (req.handle >= R36SX_HOST_RPC_MAX_FINDS ||
                !r36sx_host_rpc_find_active[req.handle]) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_HANDLE, 6);
                break;
            }
            r36sx_host_rpc_close_find_handle(req.handle);
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;

        default:
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_REQUEST, 1);
            break;
    }

    R36SX_HOSTRPC_LOG(
        "hostrpc: done cmd=%s result=%u dos=%u bytes=%lu pos=%lu size=%lu handle=%u attr=%04x",
        r36sx_host_rpc_command_name(req.command),
        (unsigned)req.result,
        (unsigned)req.dos_error,
        (unsigned long)req.bytes_done,
        (unsigned long)req.file_pos,
        (unsigned long)req.file_size,
        (unsigned)req.handle,
        (unsigned)req.attr);
    r36sx_host_rpc_store_response(r36sx_host_rpc_request_addr, &req);
    r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_DONE;
}

static void r36sx_host_rpc_stream_reply(uint16_t payload)
{
    r36sx_host_rpc_stream_sync_bit ^= R36SX_HOST_RPC_PROTO_CMD_FLAG;
    r36sx_host_rpc_stream_value =
        r36sx_host_rpc_stream_sync_bit |
        (uint16_t)(payload & R36SX_HOST_RPC_PROTO_DATA_MASK);
}

static void r36sx_host_rpc_stream_abort_transfer(void)
{
    r36sx_host_rpc_stream_command = 0;
    r36sx_host_rpc_stream_addr = 0;
    r36sx_host_rpc_stream_addr_bits = 0;
    r36sx_host_rpc_stream_addr_chunks = 0;
    r36sx_host_rpc_stream_fixed_request_done = 0;
}

static void r36sx_host_rpc_stream_bad_request(const char *reason,
                                             uint16_t proto_error)
{
    R36SX_HOSTRPC_LOG("hostrpc: stream bad request: %s", reason);
    r36sx_host_rpc_stream_abort_transfer();
    r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_BAD_REQUEST;
    r36sx_host_rpc_last_result = R36SX_HOST_RPC_ERR_BAD_REQUEST;
    r36sx_host_rpc_stream_reply(proto_error);
}

static void r36sx_host_rpc_stream_reset_session(void)
{
    r36sx_host_rpc_stream_abort_transfer();
    r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_IDLE;
    r36sx_host_rpc_last_result = R36SX_HOST_RPC_OK;
    r36sx_host_rpc_request_addr = 0;
    r36sx_host_rpc_cwd[0] = '\0';
    (void)r36sx_host_rpc_close_all_handles();
}

static void r36sx_host_rpc_stream_command_frame(uint16_t command)
{
    if (r36sx_host_rpc_is_request_command(r36sx_host_rpc_stream_command)) {
        if (command == R36SX_HOST_RPC_RESET) {
            R36SX_HOSTRPC_LOG("hostrpc: stream reset during request");
            r36sx_host_rpc_stream_reset_session();
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_VERSION);
            return;
        }
        if (command == R36SX_HOST_RPC_ABORT) {
            R36SX_HOSTRPC_LOG("hostrpc: stream abort during request");
            r36sx_host_rpc_stream_abort_transfer();
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
            return;
        }
        R36SX_HOSTRPC_LOG("hostrpc: stream short request command=%u chunks=%u expected=%u next=%u",
                          (unsigned)r36sx_host_rpc_stream_command,
                          (unsigned)r36sx_host_rpc_stream_addr_chunks,
                          (unsigned)R36SX_HOST_RPC_REQUEST_ADDR_CHUNKS,
                          (unsigned)command);
        r36sx_host_rpc_stream_bad_request(
            "protocol mismatch: command before fixed request payload complete",
            R36SX_HOST_RPC_PROTO_ERR_MISMATCH);
        return;
    }

    r36sx_host_rpc_stream_fixed_request_done = 0;

    if (r36sx_host_rpc_is_request_command(command)) {
        R36SX_HOSTRPC_LOG("hostrpc: stream command %s(%u) expects request address",
                          r36sx_host_rpc_command_name(command),
                          (unsigned)command);
        r36sx_host_rpc_stream_command = command;
        r36sx_host_rpc_stream_addr = 0;
        r36sx_host_rpc_stream_addr_bits = 0;
        r36sx_host_rpc_stream_addr_chunks = 0;
        r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
        return;
    }

    switch (command) {
        case R36SX_HOST_RPC_RESET:
            R36SX_HOSTRPC_LOG("hostrpc: stream reset");
            r36sx_host_rpc_stream_reset_session();
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_VERSION);
            break;
        case R36SX_HOST_RPC_PING:
            R36SX_HOSTRPC_LOG("hostrpc: stream ping");
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_VERSION);
            break;
        case R36SX_HOST_RPC_ABORT:
            R36SX_HOSTRPC_LOG("hostrpc: stream abort");
            r36sx_host_rpc_stream_abort_transfer();
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
            break;
        case R36SX_HOST_RPC_END:
            R36SX_HOSTRPC_LOG("hostrpc: stream end");
            r36sx_host_rpc_stream_abort_transfer();
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
            break;
        case R36SX_HOST_RPC_CONTINUE:
            R36SX_HOSTRPC_LOG("hostrpc: stream continue");
            r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
            break;
        default:
            R36SX_HOSTRPC_LOG("hostrpc: stream bad command %u",
                              (unsigned)command);
            r36sx_host_rpc_stream_bad_request(
                "protocol mismatch: unknown command",
                R36SX_HOST_RPC_PROTO_ERR_MISMATCH);
            break;
    }
}

static void r36sx_host_rpc_stream_data_frame(uint16_t payload)
{
    const uint16_t frame_payload =
        (uint16_t)(payload & R36SX_HOST_RPC_PROTO_DATA_MASK);

    if (!r36sx_host_rpc_is_request_command(r36sx_host_rpc_stream_command)) {
        if (r36sx_host_rpc_stream_fixed_request_done) {
            R36SX_HOSTRPC_LOG("hostrpc: stream extra data after fixed request payload=%u",
                              (unsigned)frame_payload);
            r36sx_host_rpc_stream_bad_request(
                "fixed request payload too large",
                R36SX_HOST_RPC_PROTO_ERR_TOO_LARGE);
            return;
        }
        R36SX_HOSTRPC_LOG("hostrpc: stream data without request command payload=%u",
                          (unsigned)frame_payload);
        r36sx_host_rpc_stream_bad_request(
            "protocol mismatch: data without request command",
            R36SX_HOST_RPC_PROTO_ERR_MISMATCH);
        return;
    }

    if (r36sx_host_rpc_stream_addr_chunks >=
        R36SX_HOST_RPC_REQUEST_ADDR_CHUNKS) {
        R36SX_HOSTRPC_LOG("hostrpc: stream oversized request command=%u chunks=%u",
                          (unsigned)r36sx_host_rpc_stream_command,
                          (unsigned)r36sx_host_rpc_stream_addr_chunks);
        r36sx_host_rpc_stream_bad_request("fixed request payload too large",
                                          R36SX_HOST_RPC_PROTO_ERR_TOO_LARGE);
        return;
    }

    if (r36sx_host_rpc_stream_addr_bits < 32u) {
        const uint8_t remaining =
            (uint8_t)(32u - r36sx_host_rpc_stream_addr_bits);
        if (remaining < R36SX_HOST_RPC_FRAME_DATA_BITS &&
            (frame_payload >> remaining) != 0u) {
            R36SX_HOSTRPC_LOG("hostrpc: stream address payload too large command=%u chunks=%u payload=%u",
                              (unsigned)r36sx_host_rpc_stream_command,
                              (unsigned)r36sx_host_rpc_stream_addr_chunks,
                              (unsigned)frame_payload);
            r36sx_host_rpc_stream_bad_request(
                "fixed request payload too large",
                R36SX_HOST_RPC_PROTO_ERR_TOO_LARGE);
            return;
        }
    }

    r36sx_host_rpc_stream_addr |=
        ((uint32_t)frame_payload
         << r36sx_host_rpc_stream_addr_bits);
    r36sx_host_rpc_stream_addr_bits =
        (uint8_t)(r36sx_host_rpc_stream_addr_bits +
                  R36SX_HOST_RPC_FRAME_DATA_BITS);
    r36sx_host_rpc_stream_addr_chunks++;
    if (r36sx_host_rpc_stream_addr_chunks <
        R36SX_HOST_RPC_REQUEST_ADDR_CHUNKS) {
        r36sx_host_rpc_stream_reply(R36SX_HOST_RPC_OK);
        return;
    }

    r36sx_host_rpc_request_addr = r36sx_host_rpc_stream_addr;
    R36SX_HOSTRPC_LOG("hostrpc: stream request addr=%05lx command=%u",
                      (unsigned long)r36sx_host_rpc_request_addr,
                      (unsigned)r36sx_host_rpc_stream_command);
    r36sx_host_rpc_execute_request(r36sx_host_rpc_stream_command);
    r36sx_host_rpc_stream_abort_transfer();
    r36sx_host_rpc_stream_fixed_request_done = 1;
    r36sx_host_rpc_stream_reply(
        r36sx_host_rpc_status == R36SX_HOST_RPC_STATUS_DONE ?
            r36sx_host_rpc_last_result :
            R36SX_HOST_RPC_PROTO_ERR);
}

static void r36sx_host_rpc_stream_portout16(uint16_t value)
{
    if (value & R36SX_HOST_RPC_PROTO_CMD_FLAG) {
        r36sx_host_rpc_stream_command_frame(
            (uint16_t)(value & R36SX_HOST_RPC_PROTO_DATA_MASK));
    } else {
        r36sx_host_rpc_stream_data_frame(value);
    }
}

static void r36sx_host_rpc_portout16(uint16_t portnum, uint16_t value)
{
    if (portnum == R36SX_HOST_RPC_STREAM_PORT) {
        r36sx_host_rpc_stream_portout16(value);
    }
}

static uint16_t r36sx_host_rpc_portin16(uint16_t portnum)
{
    return portnum == R36SX_HOST_RPC_STREAM_PORT ?
        r36sx_host_rpc_stream_value : 0xFFFFu;
}
