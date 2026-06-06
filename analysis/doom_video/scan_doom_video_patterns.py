import argparse
from pathlib import Path


PATTERNS = {
    "int10": bytes([0xCD, 0x10]),
    "set13_mov_ax_int10": bytes([0xB8, 0x13, 0x00, 0xCD, 0x10]),
    "set13_ah_al_int10": bytes([0xB4, 0x00, 0xB0, 0x13, 0xCD, 0x10]),
    "set13_al_ah_int10": bytes([0xB0, 0x13, 0xB4, 0x00, 0xCD, 0x10]),
    "dpmi_int31": bytes([0xCD, 0x31]),
    "mov_dx_3c0": bytes([0xBA, 0xC0, 0x03]),
    "mov_dx_3c4": bytes([0xBA, 0xC4, 0x03]),
    "mov_dx_3c5": bytes([0xBA, 0xC5, 0x03]),
    "mov_dx_3ce": bytes([0xBA, 0xCE, 0x03]),
    "mov_dx_3cf": bytes([0xBA, 0xCF, 0x03]),
    "mov_dx_3d4": bytes([0xBA, 0xD4, 0x03]),
    "mov_dx_3d5": bytes([0xBA, 0xD5, 0x03]),
    "mov_dx_3da": bytes([0xBA, 0xDA, 0x03]),
    "out_dx_al": bytes([0xEE]),
    "out_dx_ax": bytes([0xEF]),
    "rep_movsd": bytes([0xF3, 0x66, 0xA5]),
    "rep_movsw": bytes([0xF3, 0xA5]),
}


def find_all(blob: bytes, needle: bytes, limit: int = 200):
    hits = []
    start = 0
    while len(hits) < limit:
        index = blob.find(needle, start)
        if index < 0:
            break
        hits.append(index)
        start = index + 1
    return hits


def hexdump(blob: bytes, offset: int, before: int = 24, after: int = 40):
    start = max(0, offset - before)
    end = min(len(blob), offset + after)
    chunk = blob[start:end]
    hex_bytes = " ".join(f"{b:02x}" for b in chunk)
    ascii_bytes = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
    return start, hex_bytes, ascii_bytes


def main():
    parser = argparse.ArgumentParser(
        description="Scan DOS executables for Doom-style VGA/DPMI byte patterns.")
    parser.add_argument(
        "files",
        nargs="*",
        type=Path,
        default=[Path(r"C:\Temp\DM.EXE"), Path(r"C:\Temp\DOOM.EXE")],
        help="EXE files to scan; defaults to C:\\Temp\\DM.EXE and DOOM.EXE.")
    parser.add_argument(
        "--out",
        type=Path,
        default=Path(__file__).resolve().with_name("doom_video_patterns.txt"),
        help="Output report path.")
    args = parser.parse_args()

    out_path = args.out
    out_path.parent.mkdir(parents=True, exist_ok=True)

    with out_path.open("w", encoding="utf-8") as out:
        for path in args.files:
            blob = path.read_bytes()
            out.write(f"FILE {path} length={len(blob)}\n")
            for name, pattern in PATTERNS.items():
                hits = find_all(blob, pattern)
                if not hits:
                    continue
                out.write(f"  {name}: count>={len(hits)} offsets="
                          f"{' '.join(hex(h) for h in hits[:40])}\n")
                if name in {
                    "int10",
                    "set13_mov_ax_int10",
                    "set13_ah_al_int10",
                    "set13_al_ah_int10",
                    "dpmi_int31",
                    "mov_dx_3c0",
                    "mov_dx_3c4",
                    "mov_dx_3ce",
                    "mov_dx_3d4",
                    "mov_dx_3da",
                }:
                    for hit in hits[:10]:
                        start, hex_bytes, ascii_bytes = hexdump(blob, hit)
                        out.write(f"    @{hit:#x} ctx_start={start:#x}\n")
                        out.write(f"      {hex_bytes}\n")
                        out.write(f"      {ascii_bytes}\n")
            out.write("\n")

    print(out_path)


if __name__ == "__main__":
    main()
