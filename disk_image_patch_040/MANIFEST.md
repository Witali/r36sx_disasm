# Patch 040: remove boot route tracing

Date: 2026-05-28

## Purpose

Remove the temporary `boot_route.log` diagnostics added in patches 038 and 039
now that the device confirmed Tiny MC starts through the real boot route.

## Files

Copy these files over the matching paths on the SD card:

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
```

## Behavior Kept

`cubegm/icube.sh` still launches Tiny MC directly:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

and still calls:

```sh
init -q
```

`cubegm/icube_start.sh` also launches Tiny MC directly after killing
`hcprojector`.

`cubegm/icubemp_start.sh` keeps the stock-style return path:

```sh
killall rkgame rkgame.stock tiny_mc icube
/usr/bin/hcprojector &
```

## Removed

The scripts no longer write:

```text
/mnt/sdcard/cubegm/boot_route.log
/mnt/sdcard/boot_route.log
```

and no longer run `ps` snapshots during boot.
