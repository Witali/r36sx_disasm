#!/usr/bin/env python3
"""Create and print a normalized disasm output directory."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def clean_component(value: str) -> str:
    value = value.strip().replace("\\", "/").split("/")[-1]
    value = re.sub(r"[^A-Za-z0-9._-]+", "_", value)
    value = value.strip("._")
    if not value:
        raise ValueError("empty path component")
    return value


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("category", help="disasm category, e.g. dos, BIOS, mtd")
    parser.add_argument("name", help="target name")
    parser.add_argument("--root", default="disasm")
    args = parser.parse_args()

    root = Path(args.root)
    category = clean_component(args.category)
    name = clean_component(args.name)
    out_dir = root / category / name
    out_dir.mkdir(parents=True, exist_ok=True)
    print(out_dir)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
