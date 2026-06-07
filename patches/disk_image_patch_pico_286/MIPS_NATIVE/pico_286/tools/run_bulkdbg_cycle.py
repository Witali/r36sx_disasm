from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
import time
from pathlib import Path


PICO_DIR = Path(__file__).resolve().parents[1]
EXE = PICO_DIR / "pico_286_win.exe"


def collect_stats(host_root: Path) -> tuple[int, int, int]:
    if not host_root.exists():
        return (0, 0, 0)
    file_count = 0
    dir_count = 0
    byte_count = 0
    for path in host_root.rglob("*"):
        if path.is_dir():
            dir_count += 1
        elif path.is_file():
            file_count += 1
            byte_count += path.stat().st_size
    return (file_count, dir_count, byte_count)


def clean_host_output(host_root: Path) -> None:
    resolved = host_root.resolve()
    pico_resolved = PICO_DIR.resolve()
    if pico_resolved not in resolved.parents:
        raise RuntimeError(f"refusing to remove outside pico dir: {resolved}")
    if host_root.exists():
        try:
            shutil.rmtree(host_root)
        except PermissionError:
            if any(host_root.iterdir()):
                raise


def start_emulator() -> subprocess.Popen[bytes]:
    creationflags = 0
    if sys.platform == "win32":
        creationflags = subprocess.CREATE_NO_WINDOW
    return subprocess.Popen(
        [str(EXE)],
        cwd=str(PICO_DIR),
        stdin=subprocess.DEVNULL,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        creationflags=creationflags,
    )


def stop_emulator(proc: subprocess.Popen[bytes]) -> None:
    if proc.poll() is not None:
        return
    proc.terminate()
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait(timeout=5)


def run_once(iteration: int, args: argparse.Namespace) -> tuple[int, int, int]:
    host_root = PICO_DIR / "host" / args.host_dir
    if args.clean:
        clean_host_output(host_root)
    proc = start_emulator()
    last_stats = collect_stats(host_root)
    last_change = time.monotonic()
    started = time.monotonic()
    print(f"run={iteration} pid={proc.pid} files={last_stats[0]} dirs={last_stats[1]} bytes={last_stats[2]}")
    try:
        while True:
            time.sleep(args.poll)
            stats = collect_stats(host_root)
            now = time.monotonic()
            if stats != last_stats:
                print(f"run={iteration} t={now - started:.1f}s files={stats[0]} dirs={stats[1]} bytes={stats[2]}")
                last_stats = stats
                last_change = now
            if proc.poll() is not None:
                print(f"run={iteration} emulator_exit={proc.returncode}")
                return last_stats
            if args.timeout and now - started >= args.timeout:
                print(f"run={iteration} stop=timeout")
                return last_stats
            if args.stall and now - last_change >= args.stall:
                print(f"run={iteration} stop=stall")
                return last_stats
    finally:
        stop_emulator(proc)


def main() -> int:
    parser = argparse.ArgumentParser(description="Run the Pico-286 HOSTDRV bulk copy debug cycle.")
    parser.add_argument("--repeat", type=int, default=1)
    parser.add_argument("--poll", type=float, default=2.0)
    parser.add_argument("--timeout", type=float, default=240.0)
    parser.add_argument("--stall", type=float, default=30.0)
    parser.add_argument("--clean", action="store_true")
    parser.add_argument("--host-dir", default="BULKDBG")
    args = parser.parse_args()

    if not EXE.exists():
        print(f"missing emulator: {EXE}", file=sys.stderr)
        return 2

    final_stats = (0, 0, 0)
    for index in range(1, args.repeat + 1):
        final_stats = run_once(index, args)
    print(f"final files={final_stats[0]} dirs={final_stats[1]} bytes={final_stats[2]}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
