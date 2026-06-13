#!/usr/bin/env python3
"""Summarize byte-level differences between two binary files."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def changed_ranges(a: bytes, b: bytes) -> list[tuple[int, int]]:
    ranges: list[tuple[int, int]] = []
    max_len = max(len(a), len(b))
    start: int | None = None
    for i in range(max_len):
        av = a[i] if i < len(a) else None
        bv = b[i] if i < len(b) else None
        if av != bv:
            if start is None:
                start = i
        elif start is not None:
            ranges.append((start, i))
            start = None
    if start is not None:
        ranges.append((start, max_len))
    return ranges


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("old", type=Path)
    parser.add_argument("new", type=Path)
    parser.add_argument("--limit", type=int, default=32)
    args = parser.parse_args()

    old = args.old.read_bytes()
    new = args.new.read_bytes()
    ranges = changed_ranges(old, new)

    print(f"old_size={len(old)} old_sha256={sha256(old)}")
    print(f"new_size={len(new)} new_sha256={sha256(new)}")
    print(f"changed_ranges={len(ranges)}")
    for start, end in ranges[: args.limit]:
        print(f"0x{start:08X}..0x{end - 1:08X} ({end - start} bytes)")
    if len(ranges) > args.limit:
        print(f"... {len(ranges) - args.limit} more ranges")
    return 0 if not ranges else 1


if __name__ == "__main__":
    raise SystemExit(main())
