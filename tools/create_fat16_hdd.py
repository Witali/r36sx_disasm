#!/usr/bin/env python3
"""Create a small CHS-aligned FAT16 hard disk image for Pico-286."""

from __future__ import annotations

import argparse
import pathlib
import struct
from dataclasses import dataclass


SECTOR_SIZE = 512
FAT_COUNT = 2
RESERVED_SECTORS = 1
ROOT_ENTRIES = 512
ROOT_DIR_SECTORS = (ROOT_ENTRIES * 32 + SECTOR_SIZE - 1) // SECTOR_SIZE
MEDIA_DESCRIPTOR = 0xF8
PARTITION_TYPE_FAT16 = 0x06


@dataclass
class Geometry:
    cylinders: int
    heads: int
    sectors: int

    @property
    def total_sectors(self) -> int:
        return self.cylinders * self.heads * self.sectors


@dataclass
class InputFile:
    dos_name: str
    host_path: pathlib.Path
    data: bytes


def parse_dos_name(name: str) -> bytes:
    name = name.strip().upper()
    if not name or any(ch in name for ch in '/\\:;,+[]="<>|'):
        raise ValueError(f"invalid DOS name: {name!r}")
    parts = name.split(".")
    if len(parts) > 2:
        raise ValueError(f"invalid 8.3 DOS name: {name!r}")
    stem = parts[0]
    ext = parts[1] if len(parts) == 2 else ""
    if not (1 <= len(stem) <= 8) or len(ext) > 3:
        raise ValueError(f"name is not 8.3: {name!r}")
    return stem.encode("ascii").ljust(8, b" ") + ext.encode("ascii").ljust(3, b" ")


def parse_geometry(text: str) -> Geometry:
    parts = [int(part, 0) for part in text.replace(":", ",").split(",")]
    if len(parts) != 3:
        raise ValueError("geometry must be CYLINDERS,HEADS,SECTORS")
    geometry = Geometry(parts[0], parts[1], parts[2])
    if not (1 <= geometry.cylinders <= 1024):
        raise ValueError("cylinders must be in 1..1024")
    if not (1 <= geometry.heads <= 255):
        raise ValueError("heads must be in 1..255")
    if not (1 <= geometry.sectors <= 63):
        raise ValueError("sectors must be in 1..63")
    return geometry


def chs_bytes(lba: int, geometry: Geometry) -> bytes:
    sector_index = lba % geometry.sectors
    temp = lba // geometry.sectors
    head = temp % geometry.heads
    cylinder = temp // geometry.heads
    if cylinder > 1023:
        cylinder = 1023
        head = min(geometry.heads - 1, 254)
        sector = 63
    else:
        sector = sector_index + 1
    return bytes((head & 0xFF, (sector & 0x3F) | ((cylinder >> 2) & 0xC0), cylinder & 0xFF))


def fat16_sectors(total_sectors: int, sectors_per_cluster: int) -> int:
    sectors = 1
    while True:
        data = total_sectors - RESERVED_SECTORS - ROOT_DIR_SECTORS - FAT_COUNT * sectors
        if data <= 0:
            raise ValueError("volume is too small")
        clusters = data // sectors_per_cluster
        needed = ((clusters + 2) * 2 + SECTOR_SIZE - 1) // SECTOR_SIZE
        if needed <= sectors:
            if clusters < 4085:
                raise ValueError("volume is too small for FAT16")
            return sectors
        sectors = needed


def write_mbr(image: bytearray, geometry: Geometry, start_lba: int, part_sectors: int) -> None:
    end_lba = start_lba + part_sectors - 1
    image[0:446] = b"\0" * 446
    entry = bytearray(16)
    entry[0] = 0x80
    entry[1:4] = chs_bytes(start_lba, geometry)
    entry[4] = PARTITION_TYPE_FAT16
    entry[5:8] = chs_bytes(end_lba, geometry)
    struct.pack_into("<I", entry, 8, start_lba)
    struct.pack_into("<I", entry, 12, part_sectors)
    image[446:462] = entry
    image[510:512] = b"\x55\xAA"


def write_boot_sector(
    image: bytearray,
    offset: int,
    geometry: Geometry,
    start_lba: int,
    total_sectors: int,
    sectors_per_cluster: int,
    fat_sectors: int,
    label: str,
) -> None:
    bs = bytearray(SECTOR_SIZE)
    bs[0:3] = b"\xEB\x3C\x90"
    bs[3:11] = b"MSDOS5.0"
    struct.pack_into("<H", bs, 11, SECTOR_SIZE)
    bs[13] = sectors_per_cluster
    struct.pack_into("<H", bs, 14, RESERVED_SECTORS)
    bs[16] = FAT_COUNT
    struct.pack_into("<H", bs, 17, ROOT_ENTRIES)
    struct.pack_into("<H", bs, 19, total_sectors if total_sectors <= 0xFFFF else 0)
    bs[21] = MEDIA_DESCRIPTOR
    struct.pack_into("<H", bs, 22, fat_sectors)
    struct.pack_into("<H", bs, 24, geometry.sectors)
    struct.pack_into("<H", bs, 26, geometry.heads)
    struct.pack_into("<I", bs, 28, start_lba)
    struct.pack_into("<I", bs, 32, total_sectors if total_sectors > 0xFFFF else 0)
    bs[36] = 0x80
    bs[38] = 0x29
    struct.pack_into("<I", bs, 39, 0x20260529)
    bs[43:54] = label.encode("ascii", "replace")[:11].upper().ljust(11, b" ")
    bs[54:62] = b"FAT16   "
    bs[510:512] = b"\x55\xAA"
    image[offset : offset + SECTOR_SIZE] = bs


def write_label_entry(root: bytearray, label: str) -> None:
    root[0:11] = label.encode("ascii", "replace")[:11].upper().ljust(11, b" ")
    root[11] = 0x08
    struct.pack_into("<H", root, 22, 0)
    struct.pack_into("<H", root, 24, ((2026 - 1980) << 9) | (5 << 5) | 29)


def write_file_entry(root: bytearray, index: int, item: InputFile, start_cluster: int) -> None:
    entry = index * 32
    root[entry : entry + 11] = parse_dos_name(item.dos_name)
    root[entry + 11] = 0x20
    dos_date = ((2026 - 1980) << 9) | (5 << 5) | 29
    struct.pack_into("<H", root, entry + 22, 0)
    struct.pack_into("<H", root, entry + 24, dos_date)
    struct.pack_into("<H", root, entry + 26, start_cluster)
    struct.pack_into("<I", root, entry + 28, len(item.data))


def build_image(geometry: Geometry, label: str, files: list[InputFile]) -> bytes:
    start_lba = geometry.sectors
    total_sectors = geometry.total_sectors
    part_sectors = total_sectors - start_lba
    sectors_per_cluster = 4
    fat_sectors = fat16_sectors(part_sectors, sectors_per_cluster)
    first_fat_sector = start_lba + RESERVED_SECTORS
    root_sector = first_fat_sector + FAT_COUNT * fat_sectors
    first_data_sector = root_sector + ROOT_DIR_SECTORS
    max_clusters = (part_sectors - RESERVED_SECTORS - FAT_COUNT * fat_sectors - ROOT_DIR_SECTORS) // sectors_per_cluster

    image = bytearray(total_sectors * SECTOR_SIZE)
    write_mbr(image, geometry, start_lba, part_sectors)
    write_boot_sector(
        image,
        start_lba * SECTOR_SIZE,
        geometry,
        start_lba,
        part_sectors,
        sectors_per_cluster,
        fat_sectors,
        label,
    )

    fat = bytearray(fat_sectors * SECTOR_SIZE)
    struct.pack_into("<HH", fat, 0, MEDIA_DESCRIPTOR | 0xFF00, 0xFFFF)
    root = bytearray(ROOT_DIR_SECTORS * SECTOR_SIZE)
    write_label_entry(root, label)

    next_cluster = 2
    for root_index, item in enumerate(files, start=1):
        clusters_needed = (len(item.data) + sectors_per_cluster * SECTOR_SIZE - 1) // (sectors_per_cluster * SECTOR_SIZE)
        if clusters_needed == 0:
            write_file_entry(root, root_index, item, 0)
            continue
        if next_cluster + clusters_needed - 2 > max_clusters:
            raise ValueError(f"not enough hard disk space for {item.dos_name}")

        first_cluster = next_cluster
        for i in range(clusters_needed):
            cluster = first_cluster + i
            next_value = 0xFFFF if i == clusters_needed - 1 else cluster + 1
            struct.pack_into("<H", fat, cluster * 2, next_value)
            data_offset = i * sectors_per_cluster * SECTOR_SIZE
            data = item.data[data_offset : data_offset + sectors_per_cluster * SECTOR_SIZE]
            sector = first_data_sector + (cluster - 2) * sectors_per_cluster
            start = sector * SECTOR_SIZE
            image[start : start + len(data)] = data

        write_file_entry(root, root_index, item, first_cluster)
        next_cluster += clusters_needed

    for copy in range(FAT_COUNT):
        start = (first_fat_sector + copy * fat_sectors) * SECTOR_SIZE
        image[start : start + len(fat)] = fat

    root_start = root_sector * SECTOR_SIZE
    image[root_start : root_start + len(root)] = root
    return bytes(image)


def load_file_mapping(mapping: str) -> InputFile:
    if "=" not in mapping:
        raise ValueError(f"expected DOSNAME=path mapping, got {mapping!r}")
    dos_name, path_text = mapping.split("=", 1)
    host_path = pathlib.Path(path_text)
    data = host_path.read_bytes()
    parse_dos_name(dos_name)
    return InputFile(dos_name=dos_name.upper(), host_path=host_path, data=data)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, type=pathlib.Path)
    parser.add_argument("--geometry", default="65,16,63")
    parser.add_argument("--label", default="R36SX HDD")
    parser.add_argument("--file", action="append", default=[], help="DOSNAME=host-path")
    args = parser.parse_args()

    geometry = parse_geometry(args.geometry)
    files = [load_file_mapping(mapping) for mapping in args.file]
    if len(files) + 1 > ROOT_ENTRIES:
        raise ValueError("too many root directory entries")
    image = build_image(geometry, args.label, files)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(image)
    print(f"wrote {args.output} ({len(image)} bytes), CHS={geometry.cylinders},{geometry.heads},{geometry.sectors}")
    for item in files:
        print(f"  {item.dos_name:12s} {len(item.data):8d} bytes <- {item.host_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
