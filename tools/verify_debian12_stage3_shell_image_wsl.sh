#!/usr/bin/env bash
set -euo pipefail

IMG=${1:-/mnt/c/Work/r36sx_disasm/local_artifacts/r36sx_debian12_stage3_shell.img}

if [[ $(id -u) -ne 0 ]]; then
    echo "Run as root in WSL" >&2
    exit 1
fi

if [[ ! -e "$IMG" ]]; then
    echo "Image not found: $IMG" >&2
    exit 1
fi

for cmd in losetup parted lsblk mount umount sha256sum dtc grep cat test chroot qemu-mipsel-static; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "Missing required command: $cmd" >&2
        exit 1
    }
done

LOOP=
BOOT_MNT=/mnt/r36sx-debian-stage3-verify-boot
ROOT_MNT=/mnt/r36sx-debian-stage3-verify-root
DTS=/tmp/r36sx-debian-stage3-verify.dts

cleanup() {
    set +e
    umount "$ROOT_MNT" 2>/dev/null || true
    umount "$BOOT_MNT" 2>/dev/null || true
    [[ -n "$LOOP" ]] && losetup -d "$LOOP" 2>/dev/null || true
}
trap cleanup EXIT

LOOP=$(losetup --find --show -P "$IMG")
mkdir -p "$BOOT_MNT" "$ROOT_MNT"

echo "image=$IMG"
echo "loop=$LOOP"
lsblk -o NAME,SIZE,TYPE,FSTYPE,LABEL "$LOOP"
parted -s "$LOOP" unit MiB print

mount -o ro "${LOOP}p1" "$BOOT_MNT"
mount -o ro "${LOOP}p2" "$ROOT_MNT"

echo
echo "stage3_files:"
for file in dtb.bin avp.uImage vmlinux.uImage xgame-logo.bmp; do
    test -f "$BOOT_MNT/cubegm/$file"
    sha256sum "$BOOT_MNT/cubegm/$file"
done

echo
echo "launcher_files:"
test -x "$BOOT_MNT/MIPS_NATIVE/shell/shell"
test -x "$BOOT_MNT/MIPS_NATIVE/debian/start_debian_shell.sh"
test -x "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian_shell.sh"
grep -n "R36SX_SHELL_COMMAND" "$BOOT_MNT/MIPS_NATIVE/debian/start_debian_shell.sh"
grep -n "start_debian_shell" "$BOOT_MNT/cubegm/icube.sh"
grep -n "create_block_nodes_from_proc" "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian_shell.sh"
grep -n "waiting for second partition" "$BOOT_MNT/MIPS_NATIVE/debian/enter_debian_shell.sh"

echo
echo "debian:"
cat "$ROOT_MNT/etc/debian_version"
grep -E '^(PRETTY_NAME|VERSION_CODENAME)=' "$ROOT_MNT/etc/os-release"
test -x "$ROOT_MNT/bin/bash"
test -x "$ROOT_MNT/bin/busybox"
chroot "$ROOT_MNT" /usr/bin/qemu-mipsel-static /bin/bash -lc \
    'echo debian-bash-ok; /bin/busybox echo busybox-ok; dpkg --print-architecture'

dtc -I dtb -O dts -o "$DTS" "$BOOT_MNT/cubegm/dtb.bin" 2>/dev/null
echo
echo "dtb_bootargs:"
grep -E "bootargs =" "$DTS"
