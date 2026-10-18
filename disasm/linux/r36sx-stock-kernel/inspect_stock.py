"""Read-only stock firmware inspection; write only derived analysis artifacts."""
import gzip
import hashlib
import json
from pathlib import Path
import re
import struct
import zlib

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
CACHE = ROOT / "local_artifacts/r36sx-linux-feasibility"
CACHE.mkdir(parents=True, exist_ok=True)


def fingerprint(path):
    data = path.read_bytes()
    return {"path": path.relative_to(ROOT).as_posix(), "size": len(data),
            "sha256": hashlib.sha256(data).hexdigest()}, data


def uimage(path):
    info, data = fingerprint(path)
    magic, hcrc, timestamp, size, load, entry, dcrc, osid, arch, kind, comp, name = (
        struct.unpack(">7I4B32s", data[:64]))
    header = bytearray(data[:64])
    header[4:8] = b"\0" * 4
    payload = data[64:64 + size]
    assert magic == 0x27051956
    assert len(payload) == size
    assert zlib.crc32(header) == hcrc
    assert zlib.crc32(payload) == dcrc
    info.update(name=name.rstrip(b"\0").decode(), load=hex(load), entry=hex(entry),
                payload_size=size, os_id=osid, arch_id=arch, type_id=kind,
                compression_id=comp, header_crc_ok=True, data_crc_ok=True,
                trailing_bytes=len(data) - 64 - size)
    assert comp == 1, "This inspector expects gzip stock images"
    raw = gzip.decompress(payload)
    info.update(unpacked_size=len(raw), unpacked_sha256=hashlib.sha256(raw).hexdigest())
    (CACHE / (path.stem + ".raw")).write_bytes(raw)
    return info, raw


def dtb(path):
    info, data = fingerprint(path)
    magic, total, off_struct, off_strings = struct.unpack_from(">4I", data)
    assert magic == 0xd00dfeed and total == len(data)
    stack, props, pos = [], {}, off_struct
    while True:
        token, = struct.unpack_from(">I", data, pos)
        pos += 4
        if token == 1:
            end = data.index(0, pos)
            stack.append(data[pos:end].decode())
            pos = (end + 4) & ~3
        elif token == 2:
            stack.pop()
        elif token == 3:
            size, nameoff = struct.unpack_from(">2I", data, pos)
            pos += 8
            start = off_strings + nameoff
            name = data[start:data.index(0, start)].decode()
            value = data[pos:pos + size]
            key = "/" + "/".join(stack[1:]) + ":" + name
            if key in {"/memory:reg", "/chosen:bootargs", "/board:label",
                       "/hcrtos/external_files:path-prefix"} or name == "part3-filename":
                props[key] = (value.rstrip(b"\0").decode() if name != "reg" else
                              [hex(n) for n in struct.unpack(">" + "I" * (size // 4), value)])
            pos = (pos + size + 3) & ~3
        elif token == 9:
            break
        elif token != 4:
            raise ValueError(f"Unexpected FDT token {token}")
    info["properties"] = props
    return info


def cpio_entries(raw):
    """Locate valid newc archives without extracting firmware executables."""
    archives, search = [], 0
    while (start := raw.find(b"070701", search)) >= 0:
        pos, entries = start, []
        try:
            while raw[pos:pos + 6] in (b"070701", b"070702"):
                values = [int(raw[pos + 6 + n * 8:pos + 14 + n * 8], 16) for n in range(13)]
                size, namesize = values[6], values[11]
                assert 0 < namesize < 4096
                name = raw[pos + 110:pos + 110 + namesize]
                assert name[-1:] == b"\0"
                name = name[:-1].decode()
                body = (pos + 110 + namesize + 3) & ~3
                assert body + size <= len(raw)
                if name == "TRAILER!!!":
                    archives.append({"offset": hex(start), "entries": entries})
                    break
                entry = {"name": name, "mode": oct(values[1]), "size": size}
                content = raw[body:body + size]
                if values[1] & 0o170000 == 0o120000:
                    entry["target"] = content.decode(errors="replace")
                if name in ("init", "linuxrc", "./init", "./linuxrc"):
                    entry["sha256"] = hashlib.sha256(content).hexdigest()
                    if content.startswith(b"#!"):
                        entry["script"] = content.decode(errors="replace")
                if name in ("etc/inittab", "etc/fstab") or name.startswith("etc/init.d/"):
                    if b"\0" not in content:
                        entry["text"] = content.decode(errors="replace")
                entries.append(entry)
                pos = (body + size + 3) & ~3
            search = max(pos, start + 6)
        except (ValueError, AssertionError, UnicodeError):
            search = start + 6
    return archives


report = {}
for name in ("vmlinux.uImage", "avp.uImage", "advapi32.dll", "ApplicationFrame.dll"):
    info, raw = uimage(ROOT / "disk_image/cubegm" / name)
    if name in ("vmlinux.uImage", "advapi32.dll"):
        prefix = "" if name == "vmlinux.uImage" else "advapi32_"
        strings = [s.decode("ascii") for s in re.findall(rb"[\x20-\x7e]{8,}", raw)]
        info["linux_version"] = next(s for s in strings if s.startswith("Linux version "))
        selected = [s for s in strings if any(term in s for term in
                    ("Linux version ", "hc16", "hichip", "amprpc", "avp-proxy",
                     "initramfs", "EXT4-fs", "drm", "snd_pcm", "linuxrc"))]
        (OUT / (prefix + "kernel_selected_strings.txt")).write_text("\n".join(selected) + "\n", encoding="utf-8")
        info["ikconfig_marker_offset"] = raw.find(b"IKCFG_ST")
        if info["ikconfig_marker_offset"] >= 0:
            begin = info["ikconfig_marker_offset"] + 8
            end = raw.find(b"IKCFG_ED", begin)
            config = gzip.decompress(raw[begin:end])
            (OUT / (prefix + "kernel.config")).write_bytes(config)
        info["newc_archives"] = cpio_entries(raw)
        boot_text, inventory = [], ["archive_offset\tname\tmode\tsize\ttarget"]
        for archive in info["newc_archives"]:
            entries = archive.pop("entries")
            archive["entry_count"] = len(entries)
            archive["boot_files"] = []
            for entry in entries:
                inventory.append("\t".join(str(x) for x in
                    (archive["offset"], entry["name"], entry["mode"], entry["size"], entry.get("target", "-"))))
                if "script" in entry or "text" in entry:
                    boot_text.append("=== " + entry["name"] + " ===\n" + entry.get("script", entry.get("text", "")))
                if entry["name"] in ("init", "linuxrc", "etc/inittab", "etc/fstab") or entry["name"].startswith("etc/init.d/"):
                    archive["boot_files"].append(entry)
        (OUT / (prefix + "initramfs_inventory.tsv")).write_text("\n".join(inventory) + "\n", encoding="utf-8")
        (OUT / (prefix + "initramfs_boot.txt")).write_text("\n\n".join(boot_text).rstrip() + "\n", encoding="utf-8")
    report[name] = info
for name in ("dtb.bin", "Bubbles.scr"):
    report[name] = dtb(ROOT / "disk_image/cubegm" / name)
for name in ("disk_image/rootfs/linuxrc", "disk_image/rootfs/bin/busybox",
             "disk_image/cubegm/driver.so", "disasm/mtd/stage3_89da0000.bin"):
    report[name] = fingerprint(ROOT / name)[0]
stage3 = (ROOT / "disasm/mtd/stage3_89da0000.bin").read_bytes()
base = 0x89da0000
aliases = {}
for offset in range(0x1051d0, 0x105200, 8):
    names = []
    for address in struct.unpack_from("<2I", stage3, offset):
        begin = address - base
        names.append(stage3[begin:stage3.index(0, begin)].decode())
    aliases[names[0]] = names[1]
report["stage3_filename_aliases"] = {"table_address": "0x89ea51d0", "mapping": aliases,
                                      "selector_address": "0x89ea5290",
                                      "initial_selector_value": struct.unpack_from("<I", stage3, 0x105290)[0]}
(OUT / "stock_inspection.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
for name, info in report.items():
    if "sha256" not in info:
        print(name, info)
        continue
    print(name, info["sha256"], info.get("load", ""), info.get("properties", ""))
    if "newc_archives" in info:
        print("initramfs archives:", len(info["newc_archives"]),
              "IKCONFIG offset:", info["ikconfig_marker_offset"])
