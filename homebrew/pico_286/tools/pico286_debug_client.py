#!/usr/bin/env python3
"""Send one command to Pico-286's file-mailbox debug control interface."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import time


def atomic_write(path: Path, text: str) -> None:
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(text, encoding="utf-8")
    os.replace(tmp, path)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("command", nargs="+", help="debug command and args")
    parser.add_argument(
        "--dir",
        default="patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286",
        help="directory containing pico_286_debug.cmd/out",
    )
    parser.add_argument("--timeout", type=float, default=3.0)
    args = parser.parse_args()

    base = Path(args.dir)
    cmd_path = base / "pico_286_debug.cmd"
    out_path = base / "pico_286_debug.out"
    command = " ".join(args.command)

    try:
        out_path.unlink()
    except FileNotFoundError:
        pass

    atomic_write(cmd_path, command + "\n")
    deadline = time.monotonic() + args.timeout
    while time.monotonic() < deadline:
        if out_path.exists():
            print(out_path.read_text(encoding="utf-8", errors="replace"), end="")
            return 0
        time.sleep(0.02)

    raise SystemExit(f"timeout waiting for {out_path}")


if __name__ == "__main__":
    raise SystemExit(main())
