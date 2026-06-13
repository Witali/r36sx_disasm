#!/usr/bin/env python3
"""Summarize key ELF header fields and flag likely non-MIPS files."""

from __future__ import annotations

import argparse
import hashlib
import struct
from pathlib import Path


E_MACHINE = {
    8: "MIPS",
}

E_TYPE = {
    0: "NONE",
    1: "REL",
    2: "EXEC",
    3: "DYN",
    4: "CORE",
}


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("elf", type=Path)
    args = parser.parse_args()

    data = args.elf.read_bytes()
    if len(data) < 52 or data[:4] != b"\x7fELF":
        print("elf=NO")
        return 2

    elf_class = data[4]
    endian_id = data[5]
    if elf_class not in (1, 2):
        print(f"elf=YES class=unknown({elf_class})")
        return 2
    if endian_id == 1:
        endian = "<"
        endian_name = "little"
    elif endian_id == 2:
        endian = ">"
        endian_name = "big"
    else:
        print(f"elf=YES endian=unknown({endian_id})")
        return 2

    if elf_class == 1:
        header = struct.unpack(endian + "HHIIIIIHHHHHH", data[16:52])
        entry = header[3]
        flags = header[6]
    else:
        if len(data) < 64:
            print("elf=YES truncated=YES")
            return 2
        header = struct.unpack(endian + "HHIQQQIHHHHHH", data[16:64])
        entry = header[3]
        flags = header[6]

    e_type, e_machine, e_version = header[0], header[1], header[2]
    print("elf=YES")
    print(f"path={args.elf}")
    print(f"size={len(data)}")
    print(f"sha256={sha256(data)}")
    print(f"class=ELF{32 if elf_class == 1 else 64}")
    print(f"endian={endian_name}")
    print(f"type={E_TYPE.get(e_type, f'UNKNOWN({e_type})')}")
    print(f"machine={E_MACHINE.get(e_machine, f'UNKNOWN({e_machine})')} ({e_machine})")
    print(f"version={e_version}")
    print(f"entry=0x{entry:X}")
    print(f"flags=0x{flags:X}")

    if e_machine != 8:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
