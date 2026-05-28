# Linux kernel build / porting notes for R36SX

Research date: 2026-05-28.

## Short answer

Yes, we can probably build a Linux kernel image for this console class, but a
drop-in replacement is not just "compile Linux for MIPS". The current firmware
uses a vendor Hichip HC16xx platform kernel and device tree, so the practical
path is to rebuild or adapt a Hichip-compatible BSP kernel first.

The safest project direction is still:

1. Keep the stock kernel, DTB, AVP image, and rootfs unchanged while developing
   userspace modules.
2. Try kernel module builds against the exact vendor kernel before replacing
   the whole kernel image.
3. Only after matching the vendor config, DTB, load addresses, and boot format,
   test a replacement `vmlinux.uImage`.

## Facts from our firmware

- CPU/device-tree string: `MIPS 74Kc`.
- Userspace ABI: ELF32 little-endian MIPS, O32.
- Root filesystem base: Buildroot `2021.05-rc2`.
- Kernel image: U-Boot legacy `uImage` named `vmlinux`.
- Kernel version string extracted from the decompressed image:

```text
Linux version 4.4.186-release (linsen.chen@hichip01) (gcc version 6.3.0 (Codescape GNU Tools 2018.09-02 for MIPS MTI Linux) ) #21 PREEMPT Thu Dec 18 18:13:45 CST 2025
```

- Existing kernel module vermagic:

```text
4.4.186-release preempt MIPS32_R2 32BIT
```

- Device tree / kernel strings identify Hichip HC16xx hardware:

```text
Hichip hc16xx
Hichip,1600
hichip,hc16xx
hc16xx-fb
hc16xx-ge
hc16xx-spi-sf
hc16xx-spi
hc16xx-nfc
hc16xx-musb
hc16xx-irc
hc16xx-key-adc
hc16xx-check-adc
```

The important consequence is that a generic MIPS kernel is not enough. Display,
input buttons, USB, storage, and possibly audio depend on Hichip-specific
drivers and the matching DTB.

## Relevant local sources and tools

### HCRTOS SDK

Local path:

```text
internet_sources\hcrtos
```

Remote:

```text
https://git.maschath.de/ignatz/hcrtos.git
```

This is the closest local source tree found so far. It is a custom firmware /
SDK tree for DataFrog SF2000-class Hichip devices, not a proven drop-in R36SX
kernel source. Still, it contains many HC16xx defconfigs and board files, for
example:

```text
configs\hichip_hc16xx_cb_d3100_v10_projector_c1_ak02_defconfig
configs\hichip_hc16xx_cb_d3101_v20_projector_c1_cj01_defconfig
board\hc16xx\common\dts\*.dts
```

This tree is useful for:

- comparing our `dtb.bin` against HC16xx DTS files;
- finding Hichip driver names and board configuration patterns;
- learning the expected build output format for `uImage`, DTB, and firmware
  images.

### MIPS toolchain

Local archive:

```text
tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

Documented in:

```text
tools\SF3000_TOOLCHAIN.md
```

Expected compiler prefix:

```text
mips-mti-linux-gnu-
```

The archive contains Linux x86_64 toolchain binaries, so it should be used from
WSL or Linux, not directly from native Windows PowerShell. For userspace work we
have also used Zig successfully, but for kernel and kernel modules a real Linux
MIPS cross-toolchain is the better target.

## Can mainline Linux be used?

Mainline/stable Linux has MIPS support, and the Linux/MIPS tree is public.
However, mainline alone is unlikely to boot this console with working display
and input because the required Hichip HC16xx board support and drivers are
vendor-specific.

Mainline is useful as:

- a reference for generic MIPS architecture code;
- a base if we later decide to forward-port the Hichip platform;
- a source for understanding kernel APIs.

It is not currently the fastest path to a usable replacement firmware.

## Package repositories for MIPS

There are public MIPS package ecosystems, but none is a direct package feed for
this stock firmware:

- Buildroot is the native model for this firmware family. It normally builds
  a full rootfs and target packages from source instead of installing binary
  packages on-device.
- OpenWrt publishes `mipsel_24kc` packages, but those packages target OpenWrt's
  libc/userspace/kernel assumptions and are not safe to install blindly into
  this Buildroot rootfs.
- Debian has MIPS ports, but Debian packages are also not ABI/layout compatible
  enough to treat as an on-device package repository here.

For our project, the right answer is to build packages from source against the
same sysroot/toolchain, or add recipes to a Buildroot/HCRTOS-style build tree.
Kernel modules must be built against the exact kernel source/config/symbols that
produce `4.4.186-release preempt MIPS32_R2 32BIT`.

## Practical kernel-build plan

1. Decode the stock DTB:

```sh
dtc -I dtb -O dts -o stock.dts disk_image/cubegm/dtb.bin
```

2. Compare `stock.dts` with HCRTOS HC16xx DTS files under:

```text
internet_sources\hcrtos\board\hc16xx
```

3. Identify the closest HCRTOS defconfig by device tree contents, LCD timing,
   framebuffer, input ADC keys, storage, and USB gadget/host settings.

4. In WSL/Linux, try an HCRTOS build with the closest `hichip_hc16xx_*`
   defconfig and inspect generated images before flashing anything.

5. Extract and record the stock U-Boot uImage header values: image type,
   compression, load address, entry address, payload size, and CRC behavior.

6. Build a tiny external `.ko` first. A loadable module is a much safer
   compatibility test than replacing `vmlinux.uImage`.

7. Only after the module/toolchain/config match, build and test a replacement
   kernel image on removable media with a full backup of the original files.

## Risk assessment

Low risk:

- userspace `.so` modules launched through existing emulator/frontend paths;
- extra files in FAT/rootfs areas without touching boot artifacts.

Medium risk:

- external kernel modules built for the exact stock kernel;
- replacing non-critical userspace services.

High risk:

- replacing `vmlinux.uImage` / `advapi32.dll`;
- replacing `dtb.bin` / `Bubbles.scr`;
- replacing AVP-related images.

## Source links

- HCRTOS SDK: <https://git.maschath.de/ignatz/hcrtos>
- HCRTOS HC16xx Linux bootloader defconfig example:
  <https://git.maschath.de/ignatz/hcrtos/-/blob/7356e416d0d07a6f844faf89f433fb106677e9d8/configs/hichip_hc16xx_linux_bl_tiny_defconfig>
- Linux/MIPS tree mirror: <https://kernel.googlesource.com/pub/scm/linux/kernel/git/mips/linux/>
- Linux stable tree: <https://kernel.googlesource.com/pub/scm/linux/kernel/git/stable/linux-stable.git>
- Buildroot manual: <https://buildroot.org/downloads/manual/manual.html>
- OpenWrt MIPS little-endian package index example:
  <https://downloads.openwrt.org/releases/24.10.2/packages/mipsel_24kc/base/>
- Debian MIPS port information: <https://www.debian.org/ports/mips/>
