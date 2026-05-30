# Disk Image Patch 103

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 with an optional on-screen app statistics overlay:

- `Fn` + D-pad `Down` shows or hides the overlay;
- it displays decoded x86 instruction loops per second, disk image read KB/s,
  disk image write KB/s, and presented FPS;
- `[stats] app_stats_enabled=0` disables the shortcut and overlay.

Updated files:

- `MIPS_NATIVE/pico_286/pico_286`
- `MIPS_NATIVE/pico_286/README.md`
- `MIPS_NATIVE/pico_286/pico_286.conf`

```text
pico_286 size: 993864 bytes
pico_286 SHA256: D35D17E92C3832F9EFFC88E3030E181A49F4667E6D06FD8E45FE928E6DBB1816
```
