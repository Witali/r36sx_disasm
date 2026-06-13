#pragma once

#include "emulator.h"
#include "r36sx_host_disk_io.h"

int hdcount = 0, fdcount = 0;

static uint8_t sectorbuffer[512];
typedef unsigned long DWORD;

#define R36SX_FLOPPY_IMAGE_MIN_BYTES (360UL * 1024UL)
#define R36SX_FLOPPY_IMAGE_MAX_BYTES (2880UL * 1024UL)
#define R36SX_HARD_IMAGE_MAX_BYTES ((size_t)0x7ffffe00UL)
#define R36SX_BIOS_MAX_CHS_CYLINDERS 1024u

struct struct_drive {
    FILE *diskfile;
    r36sx_host_disk_cache_t cache;
    size_t filesize;
    uint16_t cyls;
    uint16_t sects;
    uint16_t heads;
    uint8_t inserted;
    uint8_t readonly;
} disk[4];

static inline int normalize_disk_number(uint8_t *drivenum) {
    if (*drivenum & 0x80) *drivenum -= 126;
    return *drivenum < 4;
}

static inline void update_bios_disk_counts(void) {
    RAM[0x475] = (uint8_t)hdcount;  // BIOS Data Area: fixed disk count.
}

static inline void disk_choose_floppy_geometry(size_t size,
                                               uint16_t *cyls,
                                               uint16_t *heads,
                                               uint16_t *sects) {
    uint64_t total_sectors = (uint64_t)(size / 512UL);

    *cyls = 80;
    *heads = 2;
    *sects = 18;

    if (size <= 360UL * 1024UL) {
        *cyls = 40;
        *sects = 9;
    } else if (size <= 720UL * 1024UL) {
        *sects = 9;
    } else if (size <= 1200UL * 1024UL) {
        *sects = 15;
    } else if (size > 1440UL * 1024UL) {
        *sects = 36;
    }

    if ((uint64_t)*cyls * (uint64_t)*heads * (uint64_t)*sects !=
        total_sectors &&
        total_sectors % 160ULL == 0) {
        uint64_t auto_sects = total_sectors / 160ULL;
        if (auto_sects >= 1 && auto_sects <= 36) {
            *cyls = 80;
            *heads = 2;
            *sects = (uint16_t)auto_sects;
        }
    }
}

static inline void disk_choose_hdd_geometry(size_t size,
                                            uint16_t *cyls,
                                            uint16_t *heads,
                                            uint16_t *sects) {
    static const uint16_t preferred_heads[] = {
        16, 32, 64, 128, 255, 8, 4, 2, 1
    };
    static const uint16_t preferred_sects[] = {
        63, 32, 17, 16, 15, 9, 18, 36
    };
    uint64_t total_sectors = (uint64_t)(size / 512UL);
    uint16_t chosen_heads = 16;
    uint64_t chosen_cyls;

    for (size_t hi = 0; hi < sizeof(preferred_heads) / sizeof(preferred_heads[0]);
         hi++) {
        for (size_t si = 0;
             si < sizeof(preferred_sects) / sizeof(preferred_sects[0]);
             si++) {
            uint64_t track_sectors =
                (uint64_t)preferred_heads[hi] * (uint64_t)preferred_sects[si];
            uint64_t exact_cyls;

            if (track_sectors == 0 || total_sectors % track_sectors != 0) {
                continue;
            }
            exact_cyls = total_sectors / track_sectors;
            if (exact_cyls >= 1 &&
                exact_cyls <= R36SX_BIOS_MAX_CHS_CYLINDERS) {
                *cyls = (uint16_t)exact_cyls;
                *heads = preferred_heads[hi];
                *sects = preferred_sects[si];
                return;
            }
        }
    }

    *sects = 63;

    if (total_sectors <= R36SX_BIOS_MAX_CHS_CYLINDERS * 16ULL * 63ULL) {
        chosen_heads = 16;
    } else if (total_sectors <= R36SX_BIOS_MAX_CHS_CYLINDERS * 32ULL * 63ULL) {
        chosen_heads = 32;
    } else if (total_sectors <= R36SX_BIOS_MAX_CHS_CYLINDERS * 64ULL * 63ULL) {
        chosen_heads = 64;
    } else if (total_sectors <= R36SX_BIOS_MAX_CHS_CYLINDERS * 128ULL * 63ULL) {
        chosen_heads = 128;
    } else {
        chosen_heads = 255;
    }

    chosen_cyls = total_sectors / ((uint64_t)chosen_heads * (uint64_t)*sects);
    if (chosen_cyls == 0) {
        chosen_heads = 1;
        chosen_cyls = total_sectors / (uint64_t)*sects;
        if (chosen_cyls == 0) {
            *sects = total_sectors ? (uint16_t)total_sectors : 1;
            chosen_cyls = 1;
        }
    }
    if (chosen_cyls > R36SX_BIOS_MAX_CHS_CYLINDERS) {
        chosen_cyls = R36SX_BIOS_MAX_CHS_CYLINDERS;
    }

    *cyls = (uint16_t)chosen_cyls;
    *heads = chosen_heads;
}

static inline void ejectdisk(uint8_t drivenum) {
    if (!normalize_disk_number(&drivenum)) return;

    if (disk[drivenum].inserted) {
        r36sx_host_disk_close(&disk[drivenum].diskfile,
                              &disk[drivenum].cache, drivenum);
        disk[drivenum].inserted = 0;
        if (drivenum >= 2) {
            if (hdcount > 0) hdcount--;
        } else {
            if (fdcount > 0) fdcount--;
        }
        update_bios_disk_counts();
    }
}

uint8_t insertdisk(uint8_t drivenum, const char *pathname) {
    uint8_t bios_drive = drivenum;
    if (!normalize_disk_number(&drivenum)) {
        printf("DISK: ERROR: unsupported drive %02Xh\n", bios_drive);
        return 0;
    }

    r36sx_host_disk_cache_t cache;
    size_t size = 0;

    ejectdisk(drivenum);

    FILE *file = r36sx_host_disk_open(pathname, &cache, &size);
    if (!file) {
        printf( "DISK: ERROR: cannot open disk file %s for drive %02Xh\n", pathname, drivenum);
        return 0;
    }

    const size_t min_size = R36SX_FLOPPY_IMAGE_MIN_BYTES;
    const size_t max_size = drivenum >= 2 ?
        R36SX_HARD_IMAGE_MAX_BYTES : R36SX_FLOPPY_IMAGE_MAX_BYTES;

    // Validate size constraints
    if (size < min_size || size > max_size || (size & 511u)) {
        r36sx_pico286_debug_log(
            "disk: reject drive %02xh image '%s' size=%lu min=%lu max=%lu",
            bios_drive, pathname ? pathname : "<null>",
            (unsigned long)size, (unsigned long)min_size,
            (unsigned long)max_size);
        r36sx_host_disk_close(&file, &cache, drivenum);
        return 0;
    }

    // Determine geometry (cyls, heads, sects)
    uint16_t cyls = 0, heads = 0, sects = 0;

    if (drivenum >= 2) {  // Hard disk
        disk_choose_hdd_geometry(size, &cyls, &heads, &sects);
        r36sx_pico286_debug_log(
            "disk: drive %02xh auto CHS=%u,%u,%u size=%lu",
            bios_drive, cyls, heads, sects, (unsigned long)size);
    } else {  // Floppy disk
        disk_choose_floppy_geometry(size, &cyls, &heads, &sects);
        r36sx_pico286_debug_log(
            "disk: drive %02xh floppy CHS=%u,%u,%u size=%lu",
            bios_drive, cyls, heads, sects, (unsigned long)size);
    }

    // Validate geometry
    if (cyls > 1023 || cyls * heads * sects * 512 != size) {
//        fclose(file);
//        fprintf(stderr, "DISK: ERROR: Cannot determine correct CHS geometry for drive %02Xh\n", drivenum);
//        return 0;
    }

    disk[drivenum].diskfile = file;
    disk[drivenum].cache = cache;
    disk[drivenum].filesize = size;
    disk[drivenum].inserted = 1;  // Using 1 instead of true for consistency with uint8_t
    disk[drivenum].readonly = 0;  // Default to read-write
    disk[drivenum].cyls = cyls;
    disk[drivenum].heads = heads;
    disk[drivenum].sects = sects;

    // Update drive counts
    if (drivenum >= 2) {
        hdcount++;
    } else {
        fdcount++;
    }
    update_bios_disk_counts();

//    printf("DISK: Disk %02Xh attached from file %s, size=%luK, CHS=%d,%d,%d\n",
//           drivenum, pathname, (unsigned long) (size >> 10), cyls, heads, sects);

    return 1;
}

// Call this ONLY if all parameters are valid! There is no check here!
static inline size_t chs2ofs(int drivenum, int cyl, int head, int sect) {
    return (
                   ((size_t)cyl * (size_t)disk[drivenum].heads + (size_t)head) * (size_t)disk[drivenum].sects + (size_t) sect - 1
           ) * 512UL;
}

static inline int disk_transfer_is_inside_image(uint8_t drivenum,
                                                size_t fileoffset,
                                                uint16_t sectcount) {
    size_t bytecount = (size_t)sectcount * 512UL;

    return sectcount != 0 &&
           fileoffset < disk[drivenum].filesize &&
           bytecount <= disk[drivenum].filesize - fileoffset;
}

static inline int disk_memory_range_is_plain_ram(uint32_t address,
                                                 size_t bytecount) {
    return bytecount > 0 &&
           address < RAM_SIZE &&
           bytecount <= (size_t)(RAM_SIZE - address);
}

static inline uint32_t disk_real_mode_linear(uint16_t segment,
                                             uint16_t offset) {
    return ((uint32_t)segment << 4) + (uint32_t)offset;
}

static inline uint16_t disk_mem_read16(uint32_t address) {
    return (uint16_t)read86(address) |
           ((uint16_t)read86(address + 1u) << 8);
}

static inline uint32_t disk_mem_read32(uint32_t address) {
    return (uint32_t)disk_mem_read16(address) |
           ((uint32_t)disk_mem_read16(address + 2u) << 16);
}

static inline uint64_t disk_mem_read64(uint32_t address) {
    return (uint64_t)disk_mem_read32(address) |
           ((uint64_t)disk_mem_read32(address + 4u) << 32);
}

static inline void disk_mem_write16(uint32_t address, uint16_t value) {
    write86(address, (uint8_t)value);
    write86(address + 1u, (uint8_t)(value >> 8));
}

static inline void disk_mem_write32(uint32_t address, uint32_t value) {
    disk_mem_write16(address, (uint16_t)value);
    disk_mem_write16(address + 2u, (uint16_t)(value >> 16));
}

static inline void disk_mem_write64(uint32_t address, uint64_t value) {
    disk_mem_write32(address, (uint32_t)value);
    disk_mem_write32(address + 4u, (uint32_t)(value >> 32));
}

static inline uint64_t disk_total_sectors(uint8_t drivenum) {
    return (uint64_t)(disk[drivenum].filesize / 512UL);
}

static inline int disk_lba_to_fileoffset(uint8_t drivenum, uint64_t lba,
                                         uint16_t sectcount,
                                         size_t *fileoffset) {
    uint64_t total_sectors = disk_total_sectors(drivenum);
    uint64_t offset64 = lba * 512ULL;

    if (sectcount == 0) {
        *fileoffset = 0;
        return lba <= total_sectors;
    }
    if (lba >= total_sectors || (uint64_t)sectcount > total_sectors - lba) {
        return 0;
    }
    if (offset64 > (uint64_t)((size_t)-1)) {
        return 0;
    }

    *fileoffset = (size_t)offset64;
    return 1;
}

static int disk_flush_drive(uint8_t drivenum, const char *reason) {
    if (drivenum & 0x80) drivenum -= 126;
    if (drivenum >= 4 || !disk[drivenum].inserted) {
        return 0;
    }
    return r36sx_host_disk_flush(disk[drivenum].diskfile,
                                 &disk[drivenum].cache, drivenum, reason);
}

void r36sx_pico286_disk_flush_pending(void) {
    for (uint8_t drivenum = 0; drivenum < 4; drivenum++) {
        if (disk[drivenum].inserted) {
            r36sx_host_disk_flush_due(disk[drivenum].diskfile,
                                      &disk[drivenum].cache, drivenum);
        }
    }
}

void r36sx_pico286_disk_flush_all(void) {
    for (uint8_t drivenum = 0; drivenum < 4; drivenum++) {
        disk_flush_drive(drivenum, "flush-all");
    }
}

static void readdisk(uint8_t drivenum,
              uint16_t dstseg, uint16_t dstoff,
              uint16_t cyl, uint16_t sect, uint16_t head,
              uint16_t sectcount, int is_verify
) {
    uint32_t memdest = disk_real_mode_linear(dstseg, dstoff);
    uint32_t cursect = 0;

    // Check if disk is inserted
    if (!disk[drivenum].inserted) {
//        printf("no media %i\r\n", drivenum);
        r36sx_pico286_debug_log("disk: read fail drive=%u no media", drivenum);
        CPU_AH = 0x31;    // no media in drive
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Check if CHS parameters are valid
    if (sect == 0 || sect > disk[drivenum].sects || cyl >= disk[drivenum].cyls || head >= disk[drivenum].heads) {
//        printf("sector not found\r\n");
        r36sx_pico286_debug_log(
            "disk: read fail drive=%u invalid CHS c=%u h=%u s=%u count=%u geometry=%u,%u,%u",
            drivenum, cyl, head, sect, sectcount,
            disk[drivenum].cyls, disk[drivenum].heads, disk[drivenum].sects);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Convert CHS to file offset
    size_t fileoffset = chs2ofs(drivenum, cyl, head, sect);
    size_t bytecount = (size_t)sectcount * 512UL;

    // Check if fileoffset is valid
    if (!disk_transfer_is_inside_image(drivenum, fileoffset, sectcount)) {
//        printf("sector not found\r\n");
        r36sx_pico286_debug_log(
            "disk: read fail drive=%u out of range c=%u h=%u s=%u count=%u offset=%lu size=%lu",
            drivenum, cyl, head, sect, sectcount,
            (unsigned long)fileoffset, (unsigned long)disk[drivenum].filesize);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (!is_verify && disk_memory_range_is_plain_ram(memdest, bytecount)) {
        if (r36sx_host_disk_read_at(disk[drivenum].diskfile, fileoffset,
                                    &RAM[memdest], bytecount) != 0) {
            r36sx_pico286_debug_log(
                "disk: read fail drive=%u bulk offset=%lu bytes=%lu",
                drivenum, (unsigned long)fileoffset,
                (unsigned long)bytecount);
            CPU_AH = 0x04;    // sector not found
            CPU_AL = 0;
            CPU_FL_CF = 1;
            return;
        }
        CPU_AL = sectcount;
        CPU_FL_CF = 0;
        CPU_AH = 0;
        return;
    }

    // Process sectors
    for (cursect = 0; cursect < sectcount; cursect++) {
        // Read the sector into buffer
        if (r36sx_host_disk_read_at(disk[drivenum].diskfile, fileoffset,
                                    &sectorbuffer[0], 512) != 0) {
//            printf("Disk read error on drive %i\r\n", drivenum);
            r36sx_pico286_debug_log(
                "disk: read fail drive=%u fread sector_index=%u offset=%lu",
                drivenum, cursect,
                (unsigned long)(fileoffset + (size_t)cursect * 512UL));
            CPU_AH = 0x04;    // sector not found
            CPU_AL = cursect;
            CPU_FL_CF = 1;
            return;
        }

        if (is_verify) {
            for (int sectoffset = 0; sectoffset < 512; sectoffset++) {
                // Verify sector data
                if (read86(memdest++) != sectorbuffer[sectoffset]) {
                    // Sector verify failed
                    r36sx_pico286_debug_log(
                        "disk: verify fail drive=%u c=%u h=%u s=%u count=%u done=%u",
                        drivenum, cyl, head, sect, sectcount, cursect);
                    CPU_AL = cursect;
                    CPU_FL_CF = 1;
                    CPU_AH = 0xBB;    // sector verify failed error code
                    return;
                }
            }
        } else {
            for (int sectoffset = 0; sectoffset < 512; sectoffset++) {
                // Write sector data
                write86(memdest++, sectorbuffer[sectoffset]);
            }
        }

        // Update file offset for next sector
        fileoffset += 512;
    }

    // If no sectors could be read, handle the error
    if (cursect == 0) {
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Set success flags
    CPU_AL = cursect;
    CPU_FL_CF = 0;
    CPU_AH = 0;
}

static void writedisk(uint8_t drivenum,
               uint16_t dstseg, uint16_t dstoff,
               uint16_t cyl, uint16_t sect, uint16_t head,
               uint16_t sectcount
) {
    uint32_t memdest = disk_real_mode_linear(dstseg, dstoff);
    uint32_t cursect = 0;

    // Check if disk is inserted
    if (!disk[drivenum].inserted) {
        r36sx_pico286_debug_log("disk: write fail drive=%u no media", drivenum);
        CPU_AH = 0x31;    // no media in drive
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (sect == 0 || sect > disk[drivenum].sects ||
        cyl >= disk[drivenum].cyls || head >= disk[drivenum].heads) {
        r36sx_pico286_debug_log(
            "disk: write fail drive=%u invalid CHS c=%u h=%u s=%u count=%u geometry=%u,%u,%u",
            drivenum, cyl, head, sect, sectcount,
            disk[drivenum].cyls, disk[drivenum].heads, disk[drivenum].sects);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Convert CHS to file offset
    size_t fileoffset = chs2ofs(drivenum, cyl, head, sect);
    size_t bytecount = (size_t)sectcount * 512UL;

    if (!disk_transfer_is_inside_image(drivenum, fileoffset, sectcount)) {
        r36sx_pico286_debug_log(
            "disk: write fail drive=%u out of range c=%u h=%u s=%u count=%u offset=%lu size=%lu",
            drivenum, cyl, head, sect, sectcount,
            (unsigned long)fileoffset, (unsigned long)disk[drivenum].filesize);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Check if drive is read-only
    if (disk[drivenum].readonly) {
        r36sx_pico286_debug_log("disk: write fail drive=%u read only", drivenum);
        CPU_AH = 0x03;    // drive is read-only
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (disk_memory_range_is_plain_ram(memdest, bytecount)) {
        if (r36sx_host_disk_write_at(disk[drivenum].diskfile,
                                     &disk[drivenum].cache, drivenum,
                                     fileoffset, &RAM[memdest], bytecount,
                                     sectcount) != 0) {
            r36sx_pico286_debug_log(
                "disk: write fail drive=%u bulk offset=%lu bytes=%lu",
                drivenum, (unsigned long)fileoffset,
                (unsigned long)bytecount);
            CPU_AH = 0xCC;    // write fault
            CPU_AL = 0;
            CPU_FL_CF = 1;
            return;
        }
        CPU_AL = sectcount;
        CPU_FL_CF = 0;
        CPU_AH = 0;
        return;
    }

    // Write each sector
    for (cursect = 0; cursect < sectcount; cursect++) {
        // Read from memory and store in sector buffer
        for (int sectoffset = 0; sectoffset < 512; sectoffset++) {
            // FIXME: segment overflow condition?
            sectorbuffer[sectoffset] = read86(memdest++);
        }

        // Write the buffer to the file
        if (r36sx_host_disk_write_at(disk[drivenum].diskfile,
                                     &disk[drivenum].cache, drivenum,
                                     fileoffset + (size_t)cursect * 512UL,
                                     sectorbuffer, 512, 1) != 0) {
            r36sx_pico286_debug_log(
                "disk: write fail drive=%u fwrite sector_index=%u offset=%lu",
                drivenum, cursect,
                (unsigned long)(fileoffset + (size_t)cursect * 512UL));
            CPU_AH = 0xCC;    // write fault
            CPU_AL = cursect;
            CPU_FL_CF = 1;
            return;
        }
    }

    // Handle the case where no sectors were written
    if (sectcount && cursect == 0) {
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Set success flags
    CPU_AL = cursect;
    CPU_FL_CF = 0;
    CPU_AH = 0;
}

static void readdisk_lba(uint8_t drivenum,
                         uint64_t lba,
                         uint32_t memdest,
                         uint16_t sectcount) {
    size_t fileoffset = 0;
    size_t bytecount = (size_t)sectcount * 512UL;
    uint16_t cursect = 0;

    if (!disk[drivenum].inserted) {
        r36sx_pico286_debug_log("disk: lba read fail drive=%u no media",
                                drivenum);
        CPU_AH = 0x31;    // no media in drive
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (!disk_lba_to_fileoffset(drivenum, lba, sectcount, &fileoffset)) {
        r36sx_pico286_debug_log(
            "disk: lba read fail drive=%u lba=%lu count=%u size=%lu",
            drivenum, (unsigned long)lba, sectcount,
            (unsigned long)disk[drivenum].filesize);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (sectcount == 0) {
        CPU_AH = 0;
        CPU_AL = 0;
        CPU_FL_CF = 0;
        return;
    }

    if (disk_memory_range_is_plain_ram(memdest, bytecount)) {
        if (r36sx_host_disk_read_at(disk[drivenum].diskfile, fileoffset,
                                    &RAM[memdest], bytecount) != 0) {
            r36sx_pico286_debug_log(
                "disk: lba read fail drive=%u bulk lba=%lu bytes=%lu",
                drivenum, (unsigned long)lba, (unsigned long)bytecount);
            CPU_AH = 0x04;    // sector not found
            CPU_AL = 0;
            CPU_FL_CF = 1;
            return;
        }
        CPU_AH = 0;
        CPU_AL = (uint8_t)sectcount;
        CPU_FL_CF = 0;
        return;
    }

    for (cursect = 0; cursect < sectcount; cursect++) {
        size_t sector_offset = fileoffset + (size_t)cursect * 512UL;
        if (r36sx_host_disk_read_at(disk[drivenum].diskfile, sector_offset,
                                    &sectorbuffer[0], 512) != 0) {
            r36sx_pico286_debug_log(
                "disk: lba read fail drive=%u sector=%u offset=%lu",
                drivenum, cursect, (unsigned long)sector_offset);
            CPU_AH = 0x04;    // sector not found
            CPU_AL = (uint8_t)cursect;
            CPU_FL_CF = 1;
            return;
        }

        for (int sectoffset = 0; sectoffset < 512; sectoffset++) {
            write86(memdest++, sectorbuffer[sectoffset]);
        }
    }

    CPU_AH = 0;
    CPU_AL = (uint8_t)cursect;
    CPU_FL_CF = 0;
}

static void writedisk_lba(uint8_t drivenum,
                          uint64_t lba,
                          uint32_t memdest,
                          uint16_t sectcount) {
    size_t fileoffset = 0;
    size_t bytecount = (size_t)sectcount * 512UL;
    uint16_t cursect = 0;

    if (!disk[drivenum].inserted) {
        r36sx_pico286_debug_log("disk: lba write fail drive=%u no media",
                                drivenum);
        CPU_AH = 0x31;    // no media in drive
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (!disk_lba_to_fileoffset(drivenum, lba, sectcount, &fileoffset)) {
        r36sx_pico286_debug_log(
            "disk: lba write fail drive=%u lba=%lu count=%u size=%lu",
            drivenum, (unsigned long)lba, sectcount,
            (unsigned long)disk[drivenum].filesize);
        CPU_AH = 0x04;    // sector not found
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (sectcount == 0) {
        CPU_AH = 0;
        CPU_AL = 0;
        CPU_FL_CF = 0;
        return;
    }

    if (disk[drivenum].readonly) {
        r36sx_pico286_debug_log("disk: lba write fail drive=%u read only",
                                drivenum);
        CPU_AH = 0x03;    // drive is read-only
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    if (disk_memory_range_is_plain_ram(memdest, bytecount)) {
        if (r36sx_host_disk_write_at(disk[drivenum].diskfile,
                                     &disk[drivenum].cache, drivenum,
                                     fileoffset, &RAM[memdest], bytecount,
                                     sectcount) != 0) {
            r36sx_pico286_debug_log(
                "disk: lba write fail drive=%u bulk lba=%lu bytes=%lu",
                drivenum, (unsigned long)lba, (unsigned long)bytecount);
            CPU_AH = 0xCC;    // write fault
            CPU_AL = 0;
            CPU_FL_CF = 1;
            return;
        }
        CPU_AH = 0;
        CPU_AL = (uint8_t)sectcount;
        CPU_FL_CF = 0;
        return;
    }

    for (cursect = 0; cursect < sectcount; cursect++) {
        size_t sector_offset = fileoffset + (size_t)cursect * 512UL;
        for (int sectoffset = 0; sectoffset < 512; sectoffset++) {
            sectorbuffer[sectoffset] = read86(memdest++);
        }

        if (r36sx_host_disk_write_at(disk[drivenum].diskfile,
                                     &disk[drivenum].cache, drivenum,
                                     sector_offset, sectorbuffer, 512, 1) != 0) {
            r36sx_pico286_debug_log(
                "disk: lba write fail drive=%u sector=%u offset=%lu",
                drivenum, cursect, (unsigned long)sector_offset);
            CPU_AH = 0xCC;    // write fault
            CPU_AL = (uint8_t)cursect;
            CPU_FL_CF = 1;
            return;
        }
    }

    CPU_AH = 0;
    CPU_AL = (uint8_t)cursect;
    CPU_FL_CF = 0;
}

typedef struct disk_address_packet_s {
    uint16_t sector_count;
    uint32_t buffer;
    uint64_t lba;
} disk_address_packet_t;

static int disk_read_address_packet(uint32_t dap,
                                    disk_address_packet_t *packet) {
    uint8_t packet_size = read86(dap);
    uint8_t reserved = read86(dap + 1u);
    uint16_t sector_count = disk_mem_read16(dap + 2u);
    uint16_t buffer_offset = disk_mem_read16(dap + 4u);
    uint16_t buffer_segment = disk_mem_read16(dap + 6u);
    uint64_t lba = disk_mem_read64(dap + 8u);

    if (packet_size < 0x10u || reserved != 0 || sector_count > 127u) {
        return 0;
    }

    if (buffer_offset == 0xffffu && buffer_segment == 0xffffu) {
        uint64_t flat_buffer;
        if (packet_size < 0x18u) {
            return 0;
        }
        flat_buffer = disk_mem_read64(dap + 16u);
        if (flat_buffer > 0xffffffffULL) {
            return 0;
        }
        packet->buffer = (uint32_t)flat_buffer;
    } else {
        packet->buffer = disk_real_mode_linear(buffer_segment, buffer_offset);
    }

    packet->sector_count = sector_count;
    packet->lba = lba;
    return 1;
}

static inline void disk_set_extended_count(uint32_t dap, uint16_t count) {
    disk_mem_write16(dap + 2u, count);
}

static void disk_get_extended_parameters(uint8_t drivenum) {
    uint32_t result = disk_real_mode_linear(CPU_DS, CPU_SI);
    uint16_t requested_size = disk_mem_read16(result);
    uint16_t returned_size;
    uint64_t total_sectors;

    if (!disk[drivenum].inserted || drivenum < 2) {
        CPU_AH = 0x31;    // no media in drive
        CPU_FL_CF = 1;
        return;
    }
    if (requested_size < 26u) {
        CPU_AH = 0x01;    // invalid command or parameter
        CPU_FL_CF = 1;
        return;
    }

    returned_size = requested_size >= 30u ? 30u : 26u;
    total_sectors = disk_total_sectors(drivenum);

    disk_mem_write16(result + 0u, returned_size);
    disk_mem_write16(result + 2u, 0x0003u);  // DMA boundary handled; geometry valid.
    disk_mem_write32(result + 4u, disk[drivenum].cyls);
    disk_mem_write32(result + 8u, disk[drivenum].heads);
    disk_mem_write32(result + 12u, disk[drivenum].sects);
    disk_mem_write64(result + 16u, total_sectors);
    disk_mem_write16(result + 24u, 512u);
    if (returned_size >= 30u) {
        disk_mem_write32(result + 26u, 0xffffffffUL);
    }

    CPU_AH = 0;
    CPU_FL_CF = 0;
}


static INLINE void diskhandler() {
    static uint8_t lastdiskah[4] = { 0 }, lastdiskcf[4] = { 0 };
    uint8_t drivenum = CPU_DL;

    // Normalize drivenum for hard drives
    if (drivenum & 0x80) drivenum -= 126;
    if (drivenum >= 4) {
        r36sx_pico286_debug_log(
            "disk: int13 unsupported drive dl=0x%02x ah=0x%02x",
            CPU_DL, CPU_AH);
        CPU_AH = 0xAA;
        CPU_AL = 0;
        CPU_FL_CF = 1;
        return;
    }

    // Handle the interrupt service based on the function requested in AH
    switch (CPU_AH) {
        case 0x00:  // Reset disk system
            if (disk[drivenum].inserted) {
                disk_flush_drive(drivenum, "int13-reset");
                CPU_AH = 0;
                CPU_FL_CF = 0;  // Successful reset (no-op in emulator)
            } else {

                CPU_FL_CF = 1;  // Disk not inserted
            }
            break;

        case 0x01:  // Return last status
            CPU_AH = lastdiskah[drivenum];
            CPU_FL_CF = lastdiskcf[drivenum];
//            printf("disk not inserted %i", drivenum);
            return;

        case 0x02:  // Read sector(s) into memory
            readdisk(drivenum, CPU_ES, CPU_BX,
                     CPU_CH + (CPU_CL / 64) * 256,  // Cylinder
                     CPU_CL & 63,                    // Sector
                     CPU_DH,                         // Head
                     CPU_AL,                         // Sector count
                     0);                             // Read operation
            break;

        case 0x03:  // Write sector(s) from memory
            writedisk(drivenum, CPU_ES, CPU_BX,
                      CPU_CH + (CPU_CL / 64) * 256,  // Cylinder
                      CPU_CL & 63,                   // Sector
                      CPU_DH,                        // Head
                      CPU_AL);                       // Sector count
            break;

        case 0x04:  // Verify sectors
            readdisk(drivenum, CPU_ES, CPU_BX,
                     CPU_CH + (CPU_CL / 64) * 256,   // Cylinder
                     CPU_CL & 63,                    // Sector
                     CPU_DH,                         // Head
                     CPU_AL,                         // Sector count
                     1);                             // Verify operation
            break;

        case 0x05:  // Format track
            CPU_FL_CF = 0;  // Success (no-op for emulator)
            CPU_AH = 0;
            break;

        case 0x08:  // Get drive parameters
            if (disk[drivenum].inserted) {
                uint16_t max_cyl = disk[drivenum].cyls - 1u;
                CPU_FL_CF = 0;
                CPU_AH = 0;
                CPU_CH = (uint8_t)max_cyl;
                CPU_CL = (disk[drivenum].sects & 63) |
                         (uint8_t)((max_cyl >> 2) & 0xC0);
                CPU_DH = disk[drivenum].heads - 1;

                // Set DL and BL for floppy or hard drive
                if (CPU_DL < 2) {
                    CPU_BL = 4;  // Floppy
                    CPU_DL = 2;
                } else {
                    CPU_DL = hdcount;  // Hard disk
                }
            } else {
                CPU_FL_CF = 1;
                CPU_AH = 0xAA;  // Error code for no disk inserted
            }
            break;

        case 0x10:  // Check if drive is ready
        case 0x11:  // Recalibrate drive
            if (disk[drivenum].inserted) {
                CPU_FL_CF = 0;
                CPU_AH = 0;
            } else {
                CPU_FL_CF = 1;
                CPU_AH = 0xAA;
            }
            break;

        case 0x15:  // Get disk type
            if (disk[drivenum].inserted) {
                CPU_FL_CF = 0;
                if (drivenum >= 2) {
                    uint64_t total_sectors64 = disk_total_sectors(drivenum);
                    uint32_t total_sectors = total_sectors64 > 0xffffffffULL ?
                        0xffffffffUL : (uint32_t)total_sectors64;
                    CPU_AH = 0x03;  // Fixed disk.
                    CPU_CX = (uint16_t)(total_sectors >> 16);
                    CPU_DX = (uint16_t)(total_sectors & 0xFFFF);
                } else {
                    CPU_AH = 0x02;  // Diskette with change-line support.
                }
            } else {
                CPU_FL_CF = 1;
                CPU_AH = 0x00;
            }
            break;

        case 0x41:  // Check extensions present
            if (drivenum >= 2 && disk[drivenum].inserted && CPU_BX == 0x55aa) {
                CPU_AH = 0x30;      // EDD 3.0 style version reporting.
                CPU_AL = 0;
                CPU_BX = 0xaa55;
                CPU_CX = 0x0005;    // Packet access + enhanced drive parameters.
                CPU_FL_CF = 0;
            } else {
                CPU_AH = 0x01;      // Invalid command or unsupported drive.
                CPU_FL_CF = 1;
            }
            break;

        case 0x42:  // Extended read using a Disk Address Packet at DS:SI
        {
            uint32_t dap = disk_real_mode_linear(CPU_DS, CPU_SI);
            disk_address_packet_t packet;
            if (drivenum < 2 || !disk_read_address_packet(dap, &packet)) {
                CPU_AH = 0x01;      // Invalid command or parameter.
                CPU_AL = 0;
                CPU_FL_CF = 1;
                break;
            }
            readdisk_lba(drivenum, packet.lba, packet.buffer,
                         packet.sector_count);
            disk_set_extended_count(dap, CPU_FL_CF ? CPU_AL
                                                   : packet.sector_count);
            break;
        }

        case 0x43:  // Extended write using a Disk Address Packet at DS:SI
        {
            uint32_t dap = disk_real_mode_linear(CPU_DS, CPU_SI);
            disk_address_packet_t packet;
            if (drivenum < 2 || !disk_read_address_packet(dap, &packet)) {
                CPU_AH = 0x01;      // Invalid command or parameter.
                CPU_AL = 0;
                CPU_FL_CF = 1;
                break;
            }
            writedisk_lba(drivenum, packet.lba, packet.buffer,
                          packet.sector_count);
            disk_set_extended_count(dap, CPU_FL_CF ? CPU_AL
                                                   : packet.sector_count);
            break;
        }

        case 0x48:  // Get extended drive parameters
            disk_get_extended_parameters(drivenum);
            break;

        default:  // Unknown function requested
            CPU_FL_CF = 1;  // Error
            break;
    }

    // Update last disk status
    lastdiskah[drivenum] = CPU_AH;
    lastdiskcf[drivenum] = CPU_FL_CF;

    // Set the last status in BIOS Data Area (for hard drives)
    if (CPU_DL & 0x80) {
        RAM[0x474] = CPU_AH;
    }
}
