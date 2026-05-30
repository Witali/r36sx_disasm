#pragma once

#include "emulator.h"
#include "r36sx_host_disk_io.h"

extern void r36sx_pico286_disk_activity(void);

int hdcount = 0, fdcount = 0;

static uint8_t sectorbuffer[512];
typedef unsigned long DWORD;

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

static inline void update_bios_disk_counts(void) {
    RAM[0x475] = (uint8_t)hdcount;  // BIOS Data Area: fixed disk count.
}

static inline void ejectdisk(uint8_t drivenum) {
    if (drivenum & 0x80) drivenum -= 126;

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
    if (drivenum & 0x80) drivenum -= 126;  // Normalize hard drive numbers

    r36sx_host_disk_cache_t cache;
    size_t size = 0;
    FILE *file = r36sx_host_disk_open(pathname, &cache, &size);
    if (!file) {
        printf( "DISK: ERROR: cannot open disk file %s for drive %02Xh\n", pathname, drivenum);
        return 0;
    }

    // Validate size constraints
    if (size < 360 * 1024 || size > 0x1f782000UL || (size & 511)) {
        r36sx_host_disk_close(&file, &cache, drivenum);
//        fprintf(stderr, "DISK: ERROR: invalid disk size for drive %02Xh (%lu bytes)\n", drivenum, (unsigned long) size);
        return 0;
    }

    // Determine geometry (cyls, heads, sects)
    uint16_t cyls = 0, heads = 0, sects = 0;

    if (drivenum >= 2) {  // Hard disk
        r36sx_pico286_chs_t configured_geometry;

        if (r36sx_pico286_hdd_geometry(bios_drive, &configured_geometry)) {
            size_t configured_size =
                (size_t)configured_geometry.cyls *
                (size_t)configured_geometry.heads *
                (size_t)configured_geometry.sects * 512UL;
            if (configured_size <= size) {
                cyls = configured_geometry.cyls;
                heads = configured_geometry.heads;
                sects = configured_geometry.sects;
                r36sx_pico286_debug_log(
                    "disk: drive %02xh using configured CHS=%u,%u,%u",
                    bios_drive, cyls, heads, sects);
            } else {
                r36sx_pico286_debug_log(
                    "disk: drive %02xh ignores CHS=%u,%u,%u larger than image",
                    bios_drive, configured_geometry.cyls,
                    configured_geometry.heads, configured_geometry.sects);
            }
        }

        if (!cyls || !heads || !sects) {
            sects = 63;
            heads = 16;
            cyls = size / (sects * heads * 512);
        }
    } else {  // Floppy disk
        cyls = 80;
        sects = 18;
        heads = 2;

        if (size <= 368640) {  // 360 KB or lower
            cyls = 40;
            sects = 9;
            heads = 2;
        } else if (size <= 737280) {
            sects = 9;
        } else if (size <= 1228800) {
            sects = 15;
        }
    }

    // Validate geometry
    if (cyls > 1023 || cyls * heads * sects * 512 != size) {
//        fclose(file);
//        fprintf(stderr, "DISK: ERROR: Cannot determine correct CHS geometry for drive %02Xh\n", drivenum);
//        return 0;
    }

    // Eject any existing disk and insert the new one
    ejectdisk(drivenum);

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
    uint32_t memdest = ((uint32_t) dstseg << 4) + (uint32_t) dstoff;
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
        r36sx_pico286_disk_activity();
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
        r36sx_pico286_disk_activity();

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
    uint32_t memdest = ((uint32_t) dstseg << 4) + (uint32_t) dstoff;
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
        r36sx_pico286_disk_activity();
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

        r36sx_pico286_disk_activity();

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
                CPU_FL_CF = 0;
                CPU_AH = 0;
                CPU_CH = disk[drivenum].cyls - 1;
                CPU_CL = (disk[drivenum].sects & 63) + ((disk[drivenum].cyls / 256) * 64);
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
                    uint32_t total_sectors =
                        (uint32_t)disk[drivenum].cyls *
                        (uint32_t)disk[drivenum].heads *
                        (uint32_t)disk[drivenum].sects;
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
