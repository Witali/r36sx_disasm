#!/usr/bin/env python3
"""Print size and SHA256 for one or more files."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path


def file_sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("paths", nargs="+", type=Path)
    args = parser.parse_args()

    for path in args.paths:
        print(f"{path}\tsize={path.stat().st_size}\tsha256={file_sha256(path)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
