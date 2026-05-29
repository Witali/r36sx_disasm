#!/usr/bin/env python3
"""Create a small FAT12 1.44 MB floppy image from explicit file mappings."""

from __future__ import annotations

import argparse
import pathlib
import struct
from dataclasses import dataclass


SECTOR_SIZE = 512
TOTAL_SECTORS = 2880
SECTORS_PER_CLUSTER = 1
RESERVED_SECTORS = 1
FAT_COUNT = 2
ROOT_ENTRIES = 224
FAT_SECTORS = 9
MEDIA_DESCRIPTOR = 0xF0
ROOT_DIR_SECTORS = (ROOT_ENTRIES * 32 + SECTOR_SIZE - 1) // SECTOR_SIZE
FIRST_DATA_SECTOR = RESERVED_SECTORS + FAT_COUNT * FAT_SECTORS + ROOT_DIR_SECTORS
MAX_CLUSTERS = TOTAL_SECTORS - FIRST_DATA_SECTOR


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


def set_fat12_entry(fat: bytearray, cluster: int, value: int) -> None:
    offset = cluster + cluster // 2
    value &= 0xFFF
    if cluster & 1:
        fat[offset] = (fat[offset] & 0x0F) | ((value << 4) & 0xF0)
        fat[offset + 1] = (value >> 4) & 0xFF
    else:
        fat[offset] = value & 0xFF
        fat[offset + 1] = (fat[offset + 1] & 0xF0) | ((value >> 8) & 0x0F)


def write_boot_sector(image: bytearray, label: str) -> None:
    bs = bytearray(SECTOR_SIZE)
    bs[0:3] = b"\xEB\x3C\x90"
    bs[3:11] = b"R36SX286"
    struct.pack_into("<H", bs, 11, SECTOR_SIZE)
    bs[13] = SECTORS_PER_CLUSTER
    struct.pack_into("<H", bs, 14, RESERVED_SECTORS)
    bs[16] = FAT_COUNT
    struct.pack_into("<H", bs, 17, ROOT_ENTRIES)
    struct.pack_into("<H", bs, 19, TOTAL_SECTORS)
    bs[21] = MEDIA_DESCRIPTOR
    struct.pack_into("<H", bs, 22, FAT_SECTORS)
    struct.pack_into("<H", bs, 24, 18)
    struct.pack_into("<H", bs, 26, 2)
    bs[36] = 0
    bs[38] = 0x29
    struct.pack_into("<I", bs, 39, 0x20260529)
    bs[43:54] = label.encode("ascii", "replace")[:11].upper().ljust(11, b" ")
    bs[54:62] = b"FAT12   "
    bs[510:512] = b"\x55\xAA"
    image[0:SECTOR_SIZE] = bs


def write_label_entry(root: bytearray, label: str) -> None:
    root[0:11] = label.encode("ascii", "replace")[:11].upper().ljust(11, b" ")
    root[11] = 0x08
    struct.pack_into("<H", root, 22, 0)
    struct.pack_into("<H", root, 24, ((2000 - 1980) << 9) | (1 << 5) | 1)


def write_file_entry(root: bytearray, index: int, item: InputFile, start_cluster: int) -> None:
    entry = index * 32
    root[entry : entry + 11] = parse_dos_name(item.dos_name)
    root[entry + 11] = 0x20
    dos_time = 0
    dos_date = ((2000 - 1980) << 9) | (1 << 5) | 1
    struct.pack_into("<H", root, entry + 22, dos_time)
    struct.pack_into("<H", root, entry + 24, dos_date)
    struct.pack_into("<H", root, entry + 26, start_cluster)
    struct.pack_into("<I", root, entry + 28, len(item.data))


def build_image(files: list[InputFile], label: str) -> bytes:
    image = bytearray(TOTAL_SECTORS * SECTOR_SIZE)
    write_boot_sector(image, label)

    fat = bytearray(FAT_SECTORS * SECTOR_SIZE)
    fat[0:3] = bytes((MEDIA_DESCRIPTOR, 0xFF, 0xFF))

    root = bytearray(ROOT_DIR_SECTORS * SECTOR_SIZE)
    write_label_entry(root, label)

    next_cluster = 2
    for root_index, item in enumerate(files, start=1):
        clusters_needed = (len(item.data) + SECTOR_SIZE - 1) // SECTOR_SIZE
        if clusters_needed == 0:
            write_file_entry(root, root_index, item, 0)
            continue
        if next_cluster + clusters_needed - 2 > MAX_CLUSTERS:
            raise ValueError(f"not enough floppy space for {item.dos_name}")

        first_cluster = next_cluster
        for i in range(clusters_needed):
            cluster = first_cluster + i
            next_value = 0xFFF if i == clusters_needed - 1 else cluster + 1
            set_fat12_entry(fat, cluster, next_value)

            data_offset = i * SECTOR_SIZE
            data = item.data[data_offset : data_offset + SECTOR_SIZE]
            sector = FIRST_DATA_SECTOR + (cluster - 2) * SECTORS_PER_CLUSTER
            image[sector * SECTOR_SIZE : sector * SECTOR_SIZE + len(data)] = data

        write_file_entry(root, root_index, item, first_cluster)
        next_cluster += clusters_needed

    fat_start = RESERVED_SECTORS * SECTOR_SIZE
    for copy in range(FAT_COUNT):
        offset = fat_start + copy * FAT_SECTORS * SECTOR_SIZE
        image[offset : offset + len(fat)] = fat

    root_start = (RESERVED_SECTORS + FAT_COUNT * FAT_SECTORS) * SECTOR_SIZE
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
    parser.add_argument("--label", default="R36SX286")
    parser.add_argument("--file", action="append", default=[], help="DOSNAME=host-path")
    args = parser.parse_args()

    files = [load_file_mapping(mapping) for mapping in args.file]
    if len(files) + 1 > ROOT_ENTRIES:
        raise ValueError("too many root directory entries")

    image = build_image(files, args.label)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(image)
    print(f"wrote {args.output} ({len(image)} bytes)")
    for item in files:
        print(f"  {item.dos_name:12s} {len(item.data):8d} bytes <- {item.host_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
