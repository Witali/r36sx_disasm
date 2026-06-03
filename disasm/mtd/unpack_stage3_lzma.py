#!/usr/bin/env python3
"""Extract and unpack the R36SX MTD1 third-stage boot payload.

The stage2 loader calls the LZMA-Alone stream at mtd1_boot.bin offset 0x5e48
and unpacks it to runtime address 0x89da0000.
"""

from __future__ import annotations

import argparse
import hashlib
import lzma
from pathlib import Path


DEFAULT_OFFSET = 0x5E48
DEFAULT_LENGTH = 0x62F4F


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--mtd1",
        type=Path,
        default=Path("hardware_info/mtd-dump/mtd1_boot.bin"),
        help="Path to mtd1_boot.bin",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=Path("disasm/mtd"),
        help="Output directory",
    )
    parser.add_argument("--offset", type=lambda s: int(s, 0), default=DEFAULT_OFFSET)
    parser.add_argument("--length", type=lambda s: int(s, 0), default=DEFAULT_LENGTH)
    args = parser.parse_args()

    mtd1 = args.mtd1
    out_dir = args.out_dir
    out_dir.mkdir(parents=True, exist_ok=True)

    data = mtd1.read_bytes()
    payload = data[args.offset : args.offset + args.length]
    if len(payload) != args.length:
        raise SystemExit(f"short read: wanted 0x{args.length:x}, got 0x{len(payload):x}")

    decoded = lzma.decompress(payload, format=lzma.FORMAT_ALONE)

    payload_path = out_dir / "stage3_89da0000.lzma"
    decoded_path = out_dir / "stage3_89da0000.bin"
    hash_path = out_dir / "stage3_89da0000.sha256sum.txt"

    payload_path.write_bytes(payload)
    decoded_path.write_bytes(decoded)
    hash_path.write_text(
        "\n".join(
            [
                f"{sha256(payload)}  {payload_path.as_posix()}  size=0x{len(payload):x} ({len(payload)})",
                f"{sha256(decoded)}  {decoded_path.as_posix()}  size=0x{len(decoded):x} ({len(decoded)})",
            ]
        )
        + "\n",
        encoding="ascii",
    )

    print(f"packed:  {payload_path}  size=0x{len(payload):x} sha256={sha256(payload)}")
    print(f"decoded: {decoded_path}  size=0x{len(decoded):x} sha256={sha256(decoded)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
