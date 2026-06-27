#!/usr/bin/env python3
"""Check that a rebuilt binary keeps the expected size."""

from __future__ import annotations

import argparse
from pathlib import Path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("reference", type=Path)
    parser.add_argument("candidate", type=Path)
    parser.add_argument("--expected-size", type=int)
    args = parser.parse_args()

    reference_size = args.reference.stat().st_size
    candidate_size = args.candidate.stat().st_size
    expected = args.expected_size if args.expected_size is not None else reference_size

    print(f"reference_size={reference_size}")
    print(f"candidate_size={candidate_size}")
    print(f"expected_size={expected}")
    if candidate_size != expected:
        print("size_check=FAIL")
        return 1
    print("size_check=OK")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
