# Stock kernel inspection for custom Linux feasibility

Date: 2026-10-18. Inputs remain under `disk_image/cubegm/`,
`disk_image/rootfs/`, and `disasm/mtd/`.

Run `inspect_stock.py` from any directory with Python 3.8+:

```powershell
& 'C:/Users/rudol/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' `
  disasm/linux/r36sx-stock-kernel/inspect_stock.py
```

The script reads local firmware as data, validates all four legacy uImage
headers and payload CRC32 values, decompresses gzip, reads selected DTB
properties, parses embedded `newc` archives, and records SHA256 fingerprints.
It executes no firmware code and writes no input files.

Generated text:

- `stock_inspection.json`: full input fingerprints, load/entry addresses,
  kernel versions, selected DTB properties, boot files, filename alias table.
- `kernel_selected_strings.txt`, `advapi32_kernel_selected_strings.txt`:
  selected strings from the two decompressed kernels.
- `initramfs_boot.txt`, `advapi32_initramfs_boot.txt`: extracted startup text.
- `initramfs_inventory.tsv`, `advapi32_initramfs_inventory.tsv`: archive names,
  modes, sizes, symlink targets. No arbitrary paths are extracted.

Decompressed binary caches are under ignored
`local_artifacts/r36sx-linux-feasibility/`. The only executed new file is our
local Python analysis script; no third-party software was downloaded.

## Findings

| Input | Kernel version / image address |
| --- | --- |
| `vmlinux.uImage` | Linux 4.4.186-release **#7**, load `0x80000000`, entry `0x803337c0` |
| `advapi32.dll` | Linux 4.4.186-release **#21**, same load/entry |
| `avp.uImage` | load/entry `0x8bda4000` |
| `ApplicationFrame.dll` | load/entry `0x83da4000` |
| `dtb.bin` | Linux region `0x0af91e50` = 175.57 MiB |
| `Bubbles.scr` | Linux region `0x02f91e50` = 47.57 MiB |

Both kernels decompress to 8,709,472 bytes. Both contain a valid uncompressed
`newc` rootfs at offset `0x4a1ca8` with 380 entries and a separate empty archive
at `0x84d788`. The extracted boot text is identical between the two kernels.
No `IKCFG_ST` marker was found; an exact embedded kernel `.config` was not
recovered by this method. Absence of a string is not a substitute for checking
kernel features on the running device.

`disk_image/rootfs/linuxrc` is byte-identical to its `bin/busybox` (867,948
bytes), not a standalone init script. Inside the kernel archive, `linuxrc` is
a symlink to `bin/busybox`, and `/init` is a shell script starting `/sbin/init`.

The actual embedded userspace sequence is:

```text
/init -> /sbin/init -> /etc/inittab -> /etc/init.d/rcS
  -> S10mdev
  -> S41hcdaemon (background service)
  -> S99app
       wait for /media/<device>/cubegm/icube
       bind SD to /mnt/sdcard
       bind rootfs/{lib,usr,bin,sbin,etc,hcdemo_files} when present
       optionally create/enable 128 MiB pagefile.sys when MemTotal <= 128000 KiB
       launch /mnt/sdcard/cubegm/icube.sh
```

This directly establishes `S99app` as the caller of `icube.sh`; the older
inference that `hcdaemon` launches it is unnecessary for this image. A
replacement shell at the `icube.sh` level must retain the `cubegm/icube`
sentinel expected by embedded `S99app`, unless initramfs is also changed.

The saved hardware `uname` matches the disguised kernel's **#21**, while the
saved `MemTotal: 43164 kB` is consistent with `Bubbles.scr`. See the accompanying
[bootloader analysis](../../mips/r36sx-linux-boot/README.md) for the RAM-probe
controlled filename substitutions. This narrows the likely historical variant;
it does not certify the current live device's memory size.

Official reference: [Linux v4.4 ramfs/rootfs/initramfs documentation](https://raw.githubusercontent.com/torvalds/linux/v4.4/Documentation/filesystems/ramfs-rootfs-initramfs.txt),
sections on embedded CPIO, `/init`, and `switch_root`.
