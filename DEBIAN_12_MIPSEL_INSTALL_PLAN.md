# Debian 12 mipsel Installation Plan for R36SX / HC16xx

Date: 2026-06-03.

This plan targets Debian 12 Bookworm `mipsel` on the HC16xx-based device while
preserving the stock MTD boot chain. The goal is to get a Debian root filesystem
running from an ext4 SD-card partition first, then only later attempt a direct
kernel root handoff if the stock kernel proves it can mount that partition early.

## Current Facts

- CPU: `MIPS 74Kc V4.12`, MIPS32r2, little-endian, O32.
- FPU is present according to captured `/proc/cpuinfo`: `FPU V0.0`.
- Userspace ABI used by stock binaries and our native builds: `mipsel`,
  hard-float, dynamic linker `/lib/ld.so.1`.
- Stock kernel: Linux `4.4.186-release`, vendor Hichip HC16xx BSP.
- Linux-visible RAM from `dtb.bin`: `0x0af91e50`, about 175.6 MiB.
- Current kernel command line:
  `root=/dev/ram0 rootfstype=ramfs rw init=/linuxrc console=tty1 ...`
- Current runtime mounts show `/dev/mmcblk0p1` as VFAT on `/mnt/sdcard`,
  `/media/mmc`, `/bin`, `/sbin`, `/etc`, `/lib`, and `/usr`.
- MTD disassembly proves only the early NOR -> stage2 -> `0x89da0000` chain.
  The Linux kernel, DTB, AVP firmware, and boot logo are SD/FAT files listed by
  the DTS `external_files` block.
- SD boot files expected by the stock loader:
  - `cubegm/dtb.bin`
  - `cubegm/avp.uImage`
  - `cubegm/vmlinux.uImage`
  - `cubegm/xgame-logo.bmp`
- `cubegm/vmlinux.uImage` is a U-Boot legacy Linux/MIPS image with load address
  `0x80000000` and entry point `0x803337c0`.

## Debian Version Choice

Use Debian 12 Bookworm `mipsel` as the first Debian target.

Reasons:

- Debian 12 still has official `mipsel` release/install documentation.
- Debian 13 Trixie removed `mipsel` and `mips64el`.
- The device ABI matches Debian `mipsel` much better than generic soft-float or
  big-endian MIPS ports.
- The stock kernel is old but still new enough for a minimal Debian 12 userspace.

Fallback options:

- Debian 11 Bullseye `mipsel` if Bookworm userspace is too heavy.
- Debian 10 Buster `mipsel` only as an archive fallback.
- Buildroot if a small embedded rootfs is more important than Debian packaging.

Official references:

- <https://www.debian.org/releases/bookworm/mipsel/>
- <https://www.debian.org/releases/bookworm/debian-installer/>
- <https://www.debian.org/releases/trixie/release-notes.en.pdf>

## Safety Principle

Do not modify MTD for Debian bring-up.

The safe path is:

```text
stock MTD loader
  -> stock SD/FAT boot files in cubegm/
  -> stock Linux kernel
  -> mount Debian rootfs from SD ext4 partition
```

This gives a simple rollback path: restore the original SD files or swap back to
the original SD card.

## Stage 0: Backups and Baseline Logs

Before changing boot files:

1. Keep the existing MTD dumps from `hardware_info/mtd-dump/`.
2. Copy the stock SD/FAT files to a host-side backup:
   - `cubegm/dtb.bin`
   - `cubegm/dtb.dts`
   - `cubegm/avp.uImage`
   - `cubegm/vmlinux.uImage`
   - `cubegm/Bubbles.scr`
   - `cubegm/icube.sh`
   - `cubegm/icube_start.sh`
   - `cubegm/icubemp_start.sh`
3. On the device, capture fresh logs from `shell`:

```sh
cat /proc/cpuinfo > /mnt/sdcard/debian_cpuinfo.txt
cat /proc/cmdline > /mnt/sdcard/debian_cmdline_stock.txt
cat /proc/filesystems > /mnt/sdcard/debian_filesystems.txt
cat /proc/mounts > /mnt/sdcard/debian_mounts_stock.txt
dmesg > /mnt/sdcard/debian_dmesg_stock.txt
ls -l /dev/mmcblk* > /mnt/sdcard/debian_mmc_devices.txt
```

Important check:

```sh
grep -w ext4 /proc/filesystems
```

If `ext4` is missing, direct ext4-root boot will not work with the stock kernel
unless the ext4 module can be loaded before root handoff.

## Stage 1: Prepare a Two-Partition SD Card

Target layout:

| Partition | Type | Size | Purpose |
| --- | --- | ---: | --- |
| `p1` | FAT32 | 512 MiB to 2 GiB | Stock boot/userspace files, `cubegm/`, `MIPS_NATIVE/`, rescue tools. |
| `p2` | ext4 | Remaining space | Debian 12 `mipsel` rootfs. |

The stock loader is expected to read from the FAT partition, so keep all boot
files on `p1`.

In Ubuntu/WSL, if tools are missing, ask the user to install:

```sh
sudo apt install debootstrap qemu-user-static binfmt-support parted dosfstools e2fsprogs rsync u-boot-tools device-tree-compiler
```

Example host-side formatting flow, replacing `/dev/sdX` with the real SD device:

```sh
lsblk
sudo umount /dev/sdX1 /dev/sdX2 2>/dev/null || true
sudo parted -s /dev/sdX mklabel msdos
sudo parted -s /dev/sdX mkpart primary fat32 1MiB 1025MiB
sudo parted -s /dev/sdX set 1 boot on
sudo parted -s /dev/sdX mkpart primary ext4 1025MiB 100%
sudo mkfs.vfat -F 32 -n R36SXBOOT /dev/sdX1
sudo mkfs.ext4 -L R36SXROOT /dev/sdX2
```

Mount and copy the current SD tree:

```sh
sudo mkdir -p /mnt/r36sx-boot /mnt/r36sx-root
sudo mount /dev/sdX1 /mnt/r36sx-boot
sudo mount /dev/sdX2 /mnt/r36sx-root
sudo rsync -a --info=progress2 /mnt/c/Work/r36sx_disasm/disk_image/ /mnt/r36sx-boot/
sync
```

## Stage 2: Create Debian 12 mipsel Rootfs

Create a minimal Debian rootfs on the ext4 partition:

```sh
sudo debootstrap --arch=mipsel --foreign --variant=minbase bookworm /mnt/r36sx-root http://deb.debian.org/debian
sudo cp /usr/bin/qemu-mipsel-static /mnt/r36sx-root/usr/bin/
sudo chroot /mnt/r36sx-root /debootstrap/debootstrap --second-stage
```

If Bookworm moves from mirrors for `mipsel`, use the archive mirror:

```sh
http://archive.debian.org/debian
```

Set `/etc/apt/sources.list` inside the rootfs:

```text
deb http://deb.debian.org/debian bookworm main
deb http://deb.debian.org/debian bookworm-updates main
deb http://security.debian.org/debian-security bookworm-security main
```

For archive fallback:

```text
deb http://archive.debian.org/debian bookworm main
```

Then enter the rootfs and install a small package set:

```sh
sudo mount -t proc proc /mnt/r36sx-root/proc
sudo mount -t sysfs sysfs /mnt/r36sx-root/sys
sudo mount --bind /dev /mnt/r36sx-root/dev
sudo chroot /mnt/r36sx-root /bin/sh

apt-get update
apt-get install --no-install-recommends \
  sysvinit-core \
  busybox-static \
  udev \
  ifupdown \
  netbase \
  iproute2 \
  procps \
  kmod \
  mount \
  e2fsprogs \
  ca-certificates \
  dropbear \
  nano
```

Prefer `sysvinit-core` over systemd for first boot attempts because RAM is
limited and the stock kernel/userspace setup is simple.

## Stage 3: Configure Debian Rootfs

Inside `/mnt/r36sx-root`, set basic files.

`/etc/fstab`:

```text
proc            /proc       proc    defaults                    0 0
sysfs           /sys        sysfs   defaults                    0 0
devtmpfs        /dev        devtmpfs mode=0755                  0 0
devpts          /dev/pts    devpts  gid=5,mode=620,ptmxmode=0666 0 0
tmpfs           /run        tmpfs   mode=0755,nosuid,nodev      0 0
/dev/mmcblk0p2  /           ext4    defaults,noatime            0 1
/dev/mmcblk0p1  /mnt/sdcard vfat    defaults,utf8              0 0
```

Create mount points:

```sh
mkdir -p /mnt/sdcard /dev/pts /proc /sys /run
```

Set hostname:

```sh
echo r36sx-debian > /etc/hostname
```

Set root password or add a local user:

```sh
passwd root
adduser r36sx
```

Make sure `/dev/ptmx` resolves correctly:

```sh
ln -sf /dev/pts/ptmx /dev/ptmx
```

Keep the rootfs lean:

```sh
apt-get clean
rm -rf /var/lib/apt/lists/*
```

## Stage 4: First Device Test - Manual Mount and Chroot

Boot the device with the stock SD boot files. Start our `shell`, then test:

```sh
ls /dev/mmcblk*
mkdir -p /mnt/debian
mount -t ext4 /dev/mmcblk0p2 /mnt/debian
mount -t proc proc /mnt/debian/proc
mount -t sysfs sysfs /mnt/debian/sys
mount --bind /dev /mnt/debian/dev
mount -t devpts devpts /mnt/debian/dev/pts
chroot /mnt/debian /bin/sh
```

Inside Debian:

```sh
uname -a
cat /etc/debian_version
dpkg --print-architecture
ldd --version
mount
ls /mnt/sdcard
```

Success criteria:

- `mount -t ext4 /dev/mmcblk0p2 /mnt/debian` works.
- `chroot /mnt/debian /bin/sh` works.
- Debian reports architecture `mipsel`.
- Commands can read and write files on ext4.
- `/proc`, `/sys`, `/dev`, and `/dev/pts` are usable.

This milestone proves Debian userspace compatibility without changing kernel
bootargs.

## Stage 5: Add a FAT-Side Debian Entry Script

Add a rescue-friendly script on FAT, for example:

```text
/mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh
```

Script shape:

```sh
#!/bin/sh
set -eu

ROOT=/mnt/debian
mkdir -p "$ROOT"

mountpoint -q "$ROOT" || mount -t ext4 /dev/mmcblk0p2 "$ROOT"
mountpoint -q "$ROOT/proc" || mount -t proc proc "$ROOT/proc"
mountpoint -q "$ROOT/sys" || mount -t sysfs sysfs "$ROOT/sys"
mountpoint -q "$ROOT/dev" || mount --bind /dev "$ROOT/dev"
mkdir -p "$ROOT/dev/pts"
mountpoint -q "$ROOT/dev/pts" || mount -t devpts devpts "$ROOT/dev/pts"

exec chroot "$ROOT" /bin/sh
```

This can be launched from our native `shell` or from a patched frontend script.
It is not yet a full Debian boot because Debian init is not PID 1.

## Stage 6: Optional Autostart from Stock Boot

Once manual chroot works, add a guarded autostart to `cubegm/icube.sh` on the
FAT partition:

```sh
if [ -x /mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh ]; then
    /mnt/sdcard/MIPS_NATIVE/debian/enter_debian.sh >/mnt/sdcard/debian_boot.log 2>&1 &
fi
```

Keep the stock frontend fallback in the same script. A failed Debian mount must
not prevent `icube` or `shell` from starting.

This stage is useful for services and experiments, but it still runs Debian
under the stock init process.

## Stage 7: Direct ext4 Root Boot Test

Only attempt this after Stage 4 succeeds and `/proc/filesystems` confirms ext4
support.

Create a modified DTS from `disk_image/cubegm/dtb.dts` and change `bootargs`:

```text
root=/dev/mmcblk0p2 rootfstype=ext4 rootwait rw init=/sbin/init console=tty1 earlycon= no_console_suspend noirqdebug
```

For the first direct test, a safer init can be:

```text
init=/bin/sh
```

Compile:

```sh
dtc -I dts -O dtb -o dtb.debian-ext4.bin dtb.debian-ext4.dts
```

Deploy on the FAT partition as a reversible swap:

```text
cubegm/dtb.bin.stock
cubegm/dtb.bin
```

Success criteria:

- Kernel waits for `/dev/mmcblk0p2`.
- ext4 root mounts.
- `/sbin/init` or `/bin/sh` starts from Debian.

Failure symptoms:

- Kernel panic: root device unavailable.
- Kernel panic: cannot mount root filesystem.
- Blank screen or no log due to console/input limitations.

Recovery:

- Restore stock `cubegm/dtb.bin` from the FAT partition backup.
- If needed, boot the original SD card.

## Stage 8: Better PID1 Without Direct Root

If direct root fails but the stock initramfs can be unpacked and repacked, add a
tiny early `/linuxrc` path:

```sh
mount -t devtmpfs devtmpfs /dev
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t ext4 /dev/mmcblk0p2 /newroot
exec switch_root /newroot /sbin/init
```

This is cleaner than launching Debian init from `icube.sh`, because `switch_root`
hands PID 1 to Debian. It requires knowing where the stock ramfs is packed and
being able to rebuild the stock `vmlinux.uImage` safely, so it is a later-stage
task.

## Stage 9: New Kernel Is a Separate Project

A replacement kernel is higher risk than a Debian rootfs:

- HC16xx display, buttons, USB, MMC, audio, and AVP integration are vendor BSP
  dependent.
- The stock `vmlinux.uImage` has known load/entry values and works with the
  stock boot chain.
- The current MTD analysis does not yet prove all constraints of the third-stage
  SD/FAT loader.

Before replacing the kernel:

1. Build and load a tiny `.ko` against `4.4.186-release preempt MIPS32_R2 32BIT`.
2. Identify the closest HCRTOS/HC16xx kernel config.
3. Match the stock uImage header format, compression, load address, and entry.
4. Test only by replacing `cubegm/vmlinux.uImage` on a backup SD card.

## Main Risks

- Debian 12 may be memory-heavy for 175 MiB RAM, especially with systemd.
- Direct ext4 root requires MMC and ext4 available before root mount.
- The stock boot path may only know the first FAT partition, but that is fine as
  long as the kernel image and DTB remain on FAT.
- PTY/devpts setup was weak in the stock firmware; Debian must mount devpts
  explicitly.
- Console/input during early direct boot may be poor; keep file logs and a
  stock fallback.

## Recommended First Implementation Order

1. Build two-partition SD.
2. Copy stock `disk_image` to FAT `p1`.
3. Create Debian 12 `mipsel` minbase rootfs on ext4 `p2`.
4. Boot stock kernel and manually mount/chroot into Debian.
5. Add `enter_debian.sh` on FAT.
6. Add guarded autostart from `icube.sh` only after manual chroot works.
7. Try modified `dtb.bin` with `root=/dev/mmcblk0p2 rootwait` only after ext4 is
   proven on the stock kernel.
8. Decode/import the `0x89da0000` third stage if bootloader behavior becomes a
   blocker.
