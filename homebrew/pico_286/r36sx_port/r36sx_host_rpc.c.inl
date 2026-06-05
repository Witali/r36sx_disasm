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
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#ifndef mkdir
#define mkdir(path, mode) _mkdir(path)
#endif
#ifndef rmdir
#define rmdir(path) _rmdir(path)
#endif
#else
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
#define R36SX_HOST_RPC_PORT_LAST 0xE36Fu

#define R36SX_HOST_RPC_MAGIC 0x5248u /* "HR" little-endian in guest RAM. */
#define R36SX_HOST_RPC_VERSION 1u
#define R36SX_HOST_RPC_MAX_FILES 32u
#define R36SX_HOST_RPC_MAX_PATH 260u
#define R36SX_HOST_RPC_MAX_HOST_PATH 512u

#define R36SX_HOST_RPC_STATUS_IDLE 0x00u
#define R36SX_HOST_RPC_STATUS_DONE 0x01u
#define R36SX_HOST_RPC_STATUS_BAD_REQUEST 0x80u

typedef enum {
    R36SX_HOST_RPC_CMD_PING = 0,
    R36SX_HOST_RPC_CMD_OPEN_RO = 1,
    R36SX_HOST_RPC_CMD_OPEN_RW = 2,
    R36SX_HOST_RPC_CMD_CREATE = 3,
    R36SX_HOST_RPC_CMD_CLOSE = 4,
    R36SX_HOST_RPC_CMD_READ = 5,
    R36SX_HOST_RPC_CMD_WRITE = 6,
    R36SX_HOST_RPC_CMD_DELETE = 7,
    R36SX_HOST_RPC_CMD_MKDIR = 8,
    R36SX_HOST_RPC_CMD_RMDIR = 9,
    R36SX_HOST_RPC_CMD_GETATTR = 10,
} r36sx_host_rpc_command_t;

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
static FILE *r36sx_host_rpc_files[R36SX_HOST_RPC_MAX_FILES];

static inline int r36sx_host_rpc_ram_range_ok(uint32_t address, size_t bytes)
{
    return bytes == 0 ? address <= RAM_SIZE :
           address < RAM_SIZE && bytes <= (size_t)(RAM_SIZE - address);
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
        case EPERM:
            return 5; /* access denied */
        case EBADF:
            return 6; /* invalid handle */
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

static void r36sx_host_rpc_append_segment(char *tail, size_t tail_size,
                                          const char *segment)
{
    size_t len;
    if (!tail || !segment || !segment[0] || tail_size == 0) {
        return;
    }
    len = strlen(tail);
    if (len > 0 && len + 1 < tail_size) {
        tail[len++] = '\\';
        tail[len] = '\0';
    }
    snprintf(tail + len, tail_size - len, "%s", segment);
    tail[tail_size - 1] = '\0';
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

static int r36sx_host_rpc_build_host_path(const char *guest_path,
                                          char *host_path,
                                          size_t host_path_size)
{
    const char *base = r36sx_pico286_host_drive_path();
    char tail[R36SX_HOST_RPC_MAX_PATH];
    char segment[R36SX_HOST_RPC_MAX_PATH];
    size_t seg_len = 0;
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

    /* Drop an optional DOS drive prefix and root slashes. */
    if (isalpha((unsigned char)guest_path[0]) && guest_path[1] == ':') {
        guest_path += 2;
    }
    while (*guest_path == '\\' || *guest_path == '/') {
        ++guest_path;
    }

    tail[0] = '\0';
    while (1) {
        char ch = *guest_path++;
        if (ch == '\\' || ch == '/' || ch == '\0') {
            segment[seg_len] = '\0';
            if (seg_len == 0 || strcmp(segment, ".") == 0) {
                /* Ignore empty and current-directory segments. */
            } else if (strcmp(segment, "..") == 0) {
                /*
                 * Allow DOS-style parent traversal inside the mapped root, but
                 * never let the resolved host path escape above host_drive_path.
                 */
                r36sx_host_rpc_pop_segment(tail);
            } else {
                r36sx_host_rpc_append_segment(tail, sizeof(tail), segment);
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

    if (tail[0] == '\0') {
        snprintf(host_path, host_path_size, "%s", base);
    } else {
        size_t base_len = strlen(base);
        if (base_len > 0 &&
            (base[base_len - 1] == '\\' || base[base_len - 1] == '/')) {
            snprintf(host_path, host_path_size, "%s%s", base, tail);
        } else {
            snprintf(host_path, host_path_size, "%s%c%s", base, sep, tail);
        }
    }
    host_path[host_path_size - 1] = '\0';
#ifndef _WIN32
    for (char *p = host_path; *p; ++p) {
        if (*p == '\\') {
            *p = '/';
        }
    }
#endif
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

static void r36sx_host_rpc_finish(r36sx_host_rpc_request_t *req,
                                  uint16_t result, uint16_t dos_error)
{
    req->result = result;
    req->dos_error = dos_error;
    r36sx_host_rpc_last_result = result;
}

static void r36sx_host_rpc_execute_request(void)
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
        return;
    }

    req.result = R36SX_HOST_RPC_OK;
    req.dos_error = 0;
    req.bytes_done = 0;

    switch (req.command) {
        case R36SX_HOST_RPC_CMD_PING:
            req.file_size = R36SX_HOST_RPC_VERSION;
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;

        case R36SX_HOST_RPC_CMD_OPEN_RO:
        case R36SX_HOST_RPC_CMD_OPEN_RW:
        case R36SX_HOST_RPC_CMD_CREATE: {
            const char *mode;
            int handle;
            FILE *fp;
            if (!r36sx_host_rpc_guest_string(req.path_phys, guest_path,
                                             sizeof(guest_path)) ||
                !r36sx_host_rpc_build_host_path(guest_path, host_path,
                                                sizeof(host_path))) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_PATH, 3);
                break;
            }
            handle = r36sx_host_rpc_free_handle();
            if (handle < 0) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_NO_FREE_HANDLE,
                                      4);
                break;
            }
            mode = req.command == R36SX_HOST_RPC_CMD_OPEN_RO ? "rb" :
                   req.command == R36SX_HOST_RPC_CMD_OPEN_RW ? "rb+" : "wb+";
            errno = 0;
            fp = fopen(host_path, mode);
            if (!fp && req.command == R36SX_HOST_RPC_CMD_OPEN_RW) {
                fp = fopen(host_path, "wb+");
            }
            if (!fp) {
                err = errno ? errno : EIO;
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
            if (!r36sx_host_rpc_ram_range_ok(req.data_phys, req.data_len)) {
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_ADDRESS, 8);
                break;
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
            if (writing) {
                done = fwrite(&RAM[req.data_phys], 1, req.data_len, fp);
                fflush(fp);
            } else {
                done = fread(&RAM[req.data_phys], 1, req.data_len, fp);
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
                    req.file_size = (uint32_t)st.st_size;
                    req.attr = (st.st_mode & S_IFDIR) ? 0x10u : 0x00u;
                }
            }
            if (rc != 0) {
                err = errno ? errno : EIO;
                r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_HOST_IO,
                                      r36sx_host_rpc_dos_error_from_errno(
                                          err, req.command !=
                                                   R36SX_HOST_RPC_CMD_GETATTR));
                break;
            }
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_OK, 0);
            break;
        }

        default:
            r36sx_host_rpc_finish(&req, R36SX_HOST_RPC_ERR_BAD_REQUEST, 1);
            break;
    }

    r36sx_host_rpc_store_response(r36sx_host_rpc_request_addr, &req);
    r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_DONE;
}

static void r36sx_host_rpc_portout(uint16_t portnum, uint8_t value)
{
    switch (portnum - R36SX_HOST_RPC_PORT_BASE) {
        case 0x04:
            r36sx_host_rpc_request_addr =
                (r36sx_host_rpc_request_addr & 0xFFFFFF00u) | value;
            break;
        case 0x05:
            r36sx_host_rpc_request_addr =
                (r36sx_host_rpc_request_addr & 0xFFFF00FFu) |
                ((uint32_t)value << 8);
            break;
        case 0x06:
            r36sx_host_rpc_request_addr =
                (r36sx_host_rpc_request_addr & 0xFF00FFFFu) |
                ((uint32_t)value << 16);
            break;
        case 0x07:
            r36sx_host_rpc_request_addr =
                (r36sx_host_rpc_request_addr & 0x00FFFFFFu) |
                ((uint32_t)value << 24);
            break;
        case 0x08:
            if (value == 1u) {
                r36sx_host_rpc_execute_request();
            } else {
                r36sx_host_rpc_status = R36SX_HOST_RPC_STATUS_BAD_REQUEST;
                r36sx_host_rpc_last_result = R36SX_HOST_RPC_ERR_BAD_REQUEST;
            }
            break;
        default:
            break;
    }
}

static uint8_t r36sx_host_rpc_portin(uint16_t portnum)
{
    switch (portnum - R36SX_HOST_RPC_PORT_BASE) {
        case 0x00:
            return 'R';
        case 0x01:
            return 'H';
        case 0x02:
            return R36SX_HOST_RPC_VERSION;
        case 0x03:
            return r36sx_host_rpc_status;
        case 0x04:
            return (uint8_t)r36sx_host_rpc_request_addr;
        case 0x05:
            return (uint8_t)(r36sx_host_rpc_request_addr >> 8);
        case 0x06:
            return (uint8_t)(r36sx_host_rpc_request_addr >> 16);
        case 0x07:
            return (uint8_t)(r36sx_host_rpc_request_addr >> 24);
        case 0x09:
            return (uint8_t)r36sx_host_rpc_last_result;
        case 0x0A:
            return (uint8_t)(r36sx_host_rpc_last_result >> 8);
        default:
            return 0xFFu;
    }
}
