#pragma GCC optimize("Ofast")
#include "emulator.h"
#include <string.h>
#if !PICO_ON_DEVICE
#include "r36sx_disk_config.h"
#endif
// https://www.phatcode.net/res/218/files/limems40.txt
// https://www.phatcode.net/res/219/files/xms20.txt
// http://www.techhelpmanual.com/944-xms_functions.html
// http://www.techhelpmanual.com/651-emm_functions.html
// http://www.techhelpmanual.com/650-expanded_memory_specification__ems_.html
// http://www.techhelpmanual.com/943-extended_memory_specification__xms_.html
// http://www.techhelpmanual.com/698-int_2fh_43xxh__himem_sys__extended_memory_manager__services.html

//#define DEBUG_XMS
#if defined(DEBUG_XMS)
#define debug_log(...) printf(__VA_ARGS__)
#else
#define debug_log(...) ((void)0)
#endif

#define XMS_VERSION 0x00
#define REQUEST_HMA 0x01
#define RELEASE_HMA 0x02
#define GLOBAL_ENABLE_A20 0x03
#define GLOBAL_DISABLE_A20 0x04
#define LOCAL_ENABLE_A20 0x05
#define LOCAL_DISABLE_A20 0x06
#define QUERY_A20 0x07

#define QUERY_EMB 0x08
#define ALLOCATE_EMB 0x09
#define RELEASE_EMB 0x0A
#define MOVE_EMB 0x0B

#define LOCK_EMB 0x0C
#define UNLOCK_EMB 0x0D
#define EMB_HANDLE_INFO 0x0E
#define REALLOCATE_EMB 0x0F

#define REQUEST_UMB 0x10
#define RELEASE_UMB 0x11

#define XMS_HANDLES 64
#define HMA_SIZE_BYTES (HMA_END - HMA_START)
#define HMA_RESERVED_KB 64u

#define XMS_ERR_HMA_NOT_EXIST 0x90
#define XMS_ERR_HMA_IN_USE 0x91
#define XMS_ERR_HMA_NOT_ALLOCATED 0x93

// FIXME: Calculate with EMS offset
#define XMS_PSRAM_OFFSET (4096*1024)

//typedef struct __attribute__((packed, aligned)) {
typedef struct __attribute__((packed, aligned)) {
    uint32_t length;
    uint16_t source_handle;
    uint32_t source_offset;
    uint16_t destination_handle;
    uint32_t destination_offset;
} move_data_t;

typedef struct umb {
    uint16_t segment;
    uint16_t size; // paragraphs
    int allocated_paragraphs; // -1 for a chain
} umb_t;

/*
 * XMS UMB allocation exposes C0000h-EFFFFh.  Keep F0000h-FFFFFh reserved for
 * BIOS/test ROM code and A0000h-BFFFFh reserved for VGA memory.
 */
#define UMB_BLOCK_PARAGRAPHS 0x0080u
#define UMB_BLOCK_BYTES (UMB_BLOCK_PARAGRAPHS << 4)
#define UMB_BLOCKS_COUNT ((UMB_END - UMB_START) / UMB_BLOCK_BYTES)

static umb_t umb_blocks[UMB_BLOCKS_COUNT];

static int umb_blocks_allocated = 0;

uint32_t xms_available = XMS_MEMORY_SIZE;
uint8_t xms_handles = 0;
static uint16_t xms_handle_kb[XMS_HANDLES] = {0};
static uint32_t xms_allocated_kb = 0;
static int hma_allocated = 0;

int a20_enabled = 0;

uint8_t PICO286_PSRAM_ATTR XMS[XMS_MEMORY_SIZE] = {0};

static uint32_t configured_upper_memory_kb(void)
{
#if PICO_ON_DEVICE
    return (UMB_END - UMB_START) >> 10;
#else
    return r36sx_pico286_upper_memory_kb();
#endif
}

static uint32_t configured_xms_memory_kb(void)
{
    uint32_t kb;

#if PICO_ON_DEVICE
    kb = XMS_MEMORY_SIZE >> 10;
#else
    kb = r36sx_pico286_xms_memory_kb();
#endif
    if (kb > (XMS_MEMORY_SIZE >> 10)) {
        kb = XMS_MEMORY_SIZE >> 10;
    }
    return kb;
}

uint32_t xms_configured_memory_bytes(void)
{
    return configured_xms_memory_kb() << 10;
}

static uint32_t xms_free_kb(void)
{
    uint32_t limit_kb = configured_xms_memory_kb();
    uint32_t reserved_kb = hma_allocated ? HMA_RESERVED_KB : 0u;
    uint32_t used_kb = xms_allocated_kb + reserved_kb;

    return used_kb < limit_kb ? limit_kb - used_kb : 0;
}

static int hma_available(void)
{
    return xms_configured_memory_bytes() >= HMA_SIZE_BYTES;
}

static int xms_range_valid(uint32_t offset, uint32_t length)
{
    uint32_t limit = xms_configured_memory_bytes();

    return offset <= limit && length <= limit - offset;
}

static void reset_xms_allocations(void)
{
    memset(xms_handle_kb, 0, sizeof(xms_handle_kb));
    xms_allocated_kb = 0;
    xms_available = xms_configured_memory_bytes();
    xms_handles = 0;
    hma_allocated = 0;
}

void init_umb() {
    uint32_t upper_paragraphs = configured_upper_memory_kb() * 64u;

    if (upper_paragraphs > ((UMB_END - UMB_START) >> 4)) {
        upper_paragraphs = (UMB_END - UMB_START) >> 4;
    }
    umb_blocks_allocated = 0;
    for (int i = 0; i < (int)UMB_BLOCKS_COUNT; ++i) {
        umb_blocks[i].segment =
            (uint16_t)((UMB_START >> 4) +
                       (uint32_t)i * UMB_BLOCK_PARAGRAPHS);
        umb_blocks[i].size = UMB_BLOCK_PARAGRAPHS;
        if (upper_paragraphs >= umb_blocks[i].size) {
            umb_blocks[i].allocated_paragraphs = 0;
            upper_paragraphs -= umb_blocks[i].size;
        } else if (upper_paragraphs > 0) {
            umb_blocks[i].size = (uint16_t)upper_paragraphs;
            umb_blocks[i].allocated_paragraphs = 0;
            upper_paragraphs = 0;
        } else {
            umb_blocks[i].allocated_paragraphs = -2;
        }
    }
    reset_xms_allocations();
}

const umb_t *get_largest_free_umb_block(uint16_t *psz) {
    const umb_t *best = NULL;
    int best_length = 0;
    int i = 0;
    while (i < (int)UMB_BLOCKS_COUNT) {
        if (0 == umb_blocks[i].allocated_paragraphs) {
            int j = i;
            int length = 0;
            while (j < (int)UMB_BLOCKS_COUNT &&
                   umb_blocks[j].allocated_paragraphs == 0) {
                if (j > i) {
                    const uint16_t expected_segment = umb_blocks[j - 1].segment + umb_blocks[j - 1].size;
                    if (umb_blocks[j].segment != expected_segment)
                        break;
                }
                length += umb_blocks[j].size;
                j++;
            }
            if (length > best_length) {
                best = &umb_blocks[i];
                best_length = length;
            }
            i = j;
        } else {
            i++;
        }
    }
    *psz = best_length;
    return best;
}

umb_t *get_free_umb_block(const uint16_t size) {
    umb_t *best = NULL;
    int best_size = 0;
    int i = 0;
    while (i < (int)UMB_BLOCKS_COUNT) {
        if (umb_blocks[i].allocated_paragraphs != 0) {
            i++;
            continue;
        }
        uint16_t total_size = 0;
        int j = i;
        while (j < (int)UMB_BLOCKS_COUNT &&
               umb_blocks[j].allocated_paragraphs == 0) {
            if (j > i) {
                const uint16_t expected_segment = umb_blocks[j - 1].segment + umb_blocks[j - 1].size;
                if (umb_blocks[j].segment != expected_segment)
                    break;
            }
            total_size += umb_blocks[j].size;
            j++;
        }
        if (total_size >= size) {
            if (best == NULL || total_size < best_size) {
                best = &umb_blocks[i];
                best_size = total_size;
            }
        }
        i = j; // skip tested block
    }
    return best;
}
#if PICO_ON_DEVICE
#include "swap.h"
#include "psram_spi.h"
extern uint32_t butter_psram_size;
#endif
static INLINE void xms_move_to(const register uint32_t destination, register uint32_t source, register uint32_t length) {
    if (butter_psram_size) {
        register uint16_t *dest_ptr = (uint16_t *) &XMS[destination];
        length /= 2;
        while (length--) {
            *dest_ptr++ = readw86(source);
            source += 2;
        }
    } else {
        uint32_t dest = XMS_PSRAM_OFFSET + destination;
        length /= 2;
        if (PSRAM_AVAILABLE) {
            while (length--) {
                write16psram(dest, readw86(source));
                dest += 2;
                source += 2;
            }
        } else {
            while (length--) {
                swap_write16(dest, readw86(source));
                dest += 2;
                source += 2;
            }
        }
    }
}

static INLINE void xms_move_from(const uint32_t source, register uint32_t destination, register uint32_t length) {
    if (butter_psram_size) {
        const register uint16_t *source_ptr = (uint16_t *) &XMS[source];
        length /= 2;
        while (length--) {
            writew86(destination, *source_ptr++);
            destination += 2;
        }
    } else {
        uint32_t s = source + XMS_PSRAM_OFFSET;
        length /= 2;
        if (PSRAM_AVAILABLE) {
            while (length--) {
                writew86(destination, read16psram(s));
                destination += 2;
                s += 2;
            }
        } else {
            while (length--) {
                writew86(destination, swap_read16(s));
                destination += 2;
                s += 2;
            }
        }
    }
}

#define to_physical_offset(offset) (((uint16_t)(((offset) >> 16) & 0xFFFF) << 4) + (uint16_t)((offset) & 0xFFFF))

uint8_t __not_in_flash() xms_handler() {
    switch (CPU_AH) {
        case XMS_VERSION: {
            // Get XMS Version
            CPU_AX = 0x0200; // We are himem 2.06
            CPU_BX = 0x0206; // driver version
            CPU_DX = hma_available() ? 0x0001 : 0x0000; // HMA exists
            break;
        }
        case REQUEST_HMA: {
            if (!hma_available()) {
                CPU_AX = 0;
                CPU_BL = XMS_ERR_HMA_NOT_EXIST;
                break;
            }
            if (hma_allocated) {
                CPU_AX = 0;
                CPU_BL = XMS_ERR_HMA_IN_USE;
                break;
            }
            hma_allocated = 1;
            a20_enabled = 1;
            CPU_AX = 1;
            CPU_BL = 0;
            break;
        }
        case RELEASE_HMA: {
            if (!hma_allocated) {
                CPU_AX = 0;
                CPU_BL = XMS_ERR_HMA_NOT_ALLOCATED;
                break;
            }
            hma_allocated = 0;
            CPU_AX = 1;
            CPU_BL = 0;
            break;
        }
        case GLOBAL_ENABLE_A20:
        case LOCAL_ENABLE_A20: {
            // Local Enable A20
            CPU_AX = 1; // Success
            CPU_BL = 0;
            a20_enabled = 1;
            break;
        }
        case GLOBAL_DISABLE_A20:
        case LOCAL_DISABLE_A20: {
            // Local Disable A20
            CPU_AX = 1; // Success
            CPU_BL = 0;
            a20_enabled = 0;
            break;
        }
        case QUERY_A20: {
            // Query A20 (Function 07h):
            CPU_AX = a20_enabled; // Success
            break;
        }

        case QUERY_EMB: {
            // 08h
            uint32_t free_kb = xms_free_kb();

            if (free_kb > 0xffffu) {
                free_kb = 0xffffu;
            }
            debug_log("[XMS] Query free\r\n");
            CPU_AX = (uint16_t)free_kb;
            CPU_DX = (uint16_t)free_kb;
            CPU_BL = 0;
            break;
        }
        case ALLOCATE_EMB: {
            // Allocate Extended Memory Block (Function 09h):
            uint16_t requested_kb = CPU_DX;
            uint8_t handle = 0;

            debug_log("[XMS] Allocate %dKb\n", CPU_DX);
            for (uint8_t i = 1; i < XMS_HANDLES; i++) {
                if (xms_handle_kb[i] == 0) {
                    handle = i;
                    break;
                }
            }
            if (requested_kb > 0 && handle && requested_kb <= xms_free_kb()) {
                xms_handle_kb[handle] = requested_kb;
                xms_allocated_kb += requested_kb;
                xms_handles++;
                CPU_DX = handle;
                CPU_AX = 1;
                CPU_BL = 0;
                break;
            }
            CPU_AX = 0;
            CPU_BL = handle ? 0xA0 : 0xA1;
            break;
        }
        case RELEASE_EMB: {
            debug_log("[XMS] Free handle %d\n", CPU_DX);
            if (CPU_DX > 0 && CPU_DX < XMS_HANDLES &&
                xms_handle_kb[CPU_DX] != 0) {
                xms_allocated_kb -= xms_handle_kb[CPU_DX];
                xms_handle_kb[CPU_DX] = 0;
                xms_handles--;
                CPU_AX = 1;
                CPU_BL = 0;
                break;
            }
            CPU_AX = 0;
            CPU_BL = 0xA2;
            break;
        }

        case MOVE_EMB: {
            // Move Extended Memory Block (Function 0Bh)
            move_data_t move_data;
            uint32_t struct_offset = ((uint32_t) CPU_DS << 4) + CPU_SI;
            uint16_t *move_data_ptr = (uint16_t *) &move_data;

            for (int i = sizeof(move_data_t) / 2; i--;) {
                *move_data_ptr++ = readw86(struct_offset++);
                struct_offset++;
            }

            // TODO: Add mem<>mem and xms<>xms
            if ((move_data.source_handle != 0 &&
                 !xms_range_valid(move_data.source_offset,
                                  move_data.length)) ||
                (move_data.destination_handle != 0 &&
                 !xms_range_valid(move_data.destination_offset,
                                  move_data.length))) {
                CPU_AX = 0;
                CPU_BL = 0xA4;
                break;
            }
            if (!move_data.source_handle) {
                move_data.source_offset = to_physical_offset(move_data.source_offset);
                xms_move_to(move_data.destination_offset, move_data.source_offset, move_data.length);
            } else if (!move_data.destination_handle) {
                move_data.destination_offset = to_physical_offset(move_data.destination_offset);
                xms_move_from(move_data.source_offset, move_data.destination_offset, move_data.length);
            }

            debug_log(
                "[XMS] Move EMB 0x%06X\r\n\t length 0x%08X \r\n\t src_handle 0x%04X \r\n\t src_offset 0x%08X \r\n\t dest_handle 0x%04X \r\n\t dest_offset 0x%08X \r\n",
                struct_offset,
                move_data.length,
                move_data.source_handle,
                move_data.source_offset,
                move_data.destination_handle,
                move_data.destination_offset
            );
            CPU_AX = 1;
            CPU_BL = 0;
            break;
        }
        case REQUEST_UMB: {
            // Request Upper Memory Block (Function 10h):
            if (CPU_DX == 0xFFFF) {
                // Query largest available block
                if (umb_blocks_allocated < (int)UMB_BLOCKS_COUNT) {
                    uint16_t sz = 0;
                    const umb_t *umb_block = get_largest_free_umb_block(&sz);
                    if (umb_block != NULL) {
                        CPU_AX = 1;
                        CPU_BX = umb_block->segment;
                        CPU_DX = sz;
                        CPU_BL = 0;
                        break;
                    }
                }
            } else {
                const uint16_t requested_size = CPU_DX;
                umb_t *umb_block = get_free_umb_block(requested_size);
                if (umb_block != NULL) {
                    int unmarked_size = requested_size;
                    CPU_BX = umb_block->segment;
                    CPU_AX = 0x0001;
                    umb_t *ub = umb_block;
                    int total_allocated = 0;
                    while (unmarked_size > 0) {
                        total_allocated += umb_block->size;
                        umb_block->allocated_paragraphs = -1;
                        umb_blocks_allocated++;
                        unmarked_size -= umb_block->size;
                        umb_block++;
                    }
                    ub->allocated_paragraphs = total_allocated;
                    CPU_DX = total_allocated;
                    break;
                }
            }

            uint16_t sz = 0;
            get_largest_free_umb_block(&sz);
            CPU_AX = 0x0000;
            CPU_DX = sz;
            CPU_BL =
                umb_blocks_allocated >= (int)UMB_BLOCKS_COUNT ? 0xB1 : 0xB0;
            break;
        }
        case RELEASE_UMB: {
            // Release Upper Memory Block (Function 11h)
            // Stub: Release Upper Memory Block
            for (int i = 0; i < (int)UMB_BLOCKS_COUNT; ++i)
                if (umb_blocks[i].segment == CPU_BX && umb_blocks[i].allocated_paragraphs > 0) {
                    int par = umb_blocks[i].allocated_paragraphs;
                    while (par > 0 && i < (int)UMB_BLOCKS_COUNT) {
                        umb_blocks[i].allocated_paragraphs = 0;
                        par -= umb_blocks[i++].size;
                        umb_blocks_allocated--;
                    }
                    CPU_AX = 0x0001; // Success
                    CPU_BL = 0;
                    return 0xCB; // Early return to avoid fall-through
                }

            CPU_AX = 0x0000; // Failure
            CPU_DX = 0x0000;
            CPU_BL = 0xB2; // Error code
            break;
        }
        default: {
            if (CPU_AH > 0x7 && CPU_AH < 0x10) {
                debug_log("[XMS] %02X\n", CPU_AH);
            }
            // Unhandled function
            CPU_AX = 0x0000; // Function not supported
            CPU_BL = 0x80; // Function not implemented

            break;
        }
    }
    return 0xCB; // RETF opcode
}
