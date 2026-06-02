# Bubbles.scr vs dtb.dts

`Bubbles.scr` was decompiled with the local WSL `dtc`:

```sh
./tools/mipsel-buildroot-linux-gnu_sdk-buildroot/bin/linux-dtc \
  -I dtb -O dts \
  -o build-own-linux-feature/Bubbles.dts \
  build-own-linux-feature/Bubbles.scr
```

For the structural comparison, `dtb.bin` was decompiled again into a temporary
raw DTS, because `build-own-linux-feature/dtb.dts` contains research comments.
The resulting machine diff is saved as:

```text
build-own-linux-feature/Bubbles_vs_dtb.diff
```

## Result

The two trees are almost identical. They have the same node layout, bootargs,
external file list, storage partitions, display timing, panel init sequence,
GPIO/key map, USB/MMC/SPI/UART nodes, and hcrtos service nodes.

The only structural differences are memory address values. `Bubbles.scr` moves
the Linux-visible memory limit and the hcRTOS/framebuffer memory regions down
by `0x08000000` bytes compared with `dtb.bin`.

| Node/property | `dtb.bin` / `dtb.dts` | `Bubbles.scr` / `Bubbles.dts` |
| --- | ---: | ---: |
| `/soc/fb0@18808000/buffer-phy-static[0]` | `0x0af91e50` | `0x02f91e50` |
| `/hcrtos/memory-mapping/bootmem/reg[0]` | `0x09da0000` | `0x01da0000` |
| `/hcrtos/memory-mapping/sysmem/reg[0]` | `0x0bda2e50` | `0x03da2e50` |
| `/hcrtos/memory-mapping/mmz0/reg[0]` | `0x0cda2e50` | `0x04da2e50` |
| `/hcrtos/memory-mapping/mmz1/reg[0]` | `0x0c8f6450` | `0x048f6450` |
| `/hcrtos/fb0@18808000/buffer-phy-static[0]` | `0x0af91e50` | `0x02f91e50` |
| `/memory/reg[1]` | `0x0af91e50` | `0x02f91e50` |

Sizes are unchanged:

- framebuffer static buffer size: `0x0e11000`
- `bootmem` size: `0x02000000`
- `sysmem` size: `0x00b53600`
- `mmz0` size: `0x0325d1b0`
- `mmz1` size: `0x004aca00`

## Interpretation

`Bubbles.scr` appears to be a device tree variant for the same hardware and
software stack, but with a smaller Linux memory map. The address delta is
consistent across the visible memory size, static framebuffer buffer, and
hcRTOS memory regions.

This likely means `Bubbles.scr` reserves or assumes a different high-memory
layout than `dtb.bin`. For a custom Linux build, `dtb.bin` is the better base if
we want to preserve the firmware's larger `memory` range. `Bubbles.scr` is still
useful as a reference variant showing that the vendor boot path can swap DTB
memory layout without changing the rest of the board description.

## dtc warnings

`Bubbles.scr` emits the same SPI warnings as `dtb.bin`:

```text
Warning (reg_format): "reg" property in /hcrtos/sfspi/spi_nor_flash has invalid length (4 bytes) (#address-cells == 2, #size-cells == 1)
Warning (reg_format): "reg" property in /hcrtos/sfspi/spidev@0 has invalid length (4 bytes) (#address-cells == 2, #size-cells == 1)
Warning (avoid_default_addr_size): Relying on default #address-cells value for /hcrtos/sfspi/spi_nor_flash
Warning (avoid_default_addr_size): Relying on default #size-cells value for /hcrtos/sfspi/spi_nor_flash
Warning (avoid_default_addr_size): Relying on default #address-cells value for /hcrtos/sfspi/spidev@0
Warning (avoid_default_addr_size): Relying on default #size-cells value for /hcrtos/sfspi/spidev@0
```
