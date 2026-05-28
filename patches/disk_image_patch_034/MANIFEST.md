# disk_image_patch_034

Date: 2026-05-28

Purpose:

Keep Tiny MC in direct-launch mode, but add an Fn shortcut that exits Tiny MC
and starts the stock `icube` supervisor.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

`cubegm/rkgame` in this patch is restored from `cubegm/rkgame.stock`, so when
Tiny MC starts `icube`, `icube` can launch the stock game frontend.

## Behavior

`cubegm/icube_start.sh` still starts Tiny MC directly:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

When Fn is pressed in Tiny MC:

```text
Tiny MC
  -> close display/input
  -> chdir /mnt/sdcard/cubegm
  -> execl /mnt/sdcard/cubegm/icube
  -> icube launches /mnt/sdcard/cubegm/rkgame
```

Tiny MC remains built with:

```c
#define USE_ICUBE_HEARTBEAT 0
```

so it does not attach or update the `icube` SysV shared memory heartbeat.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 40512 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains strings: /mnt/sdcard/MIPS_NATIVE, /mnt/sdcard/cubegm/icube, FN iCube
  does not contain strings: shmget, heartbeat
  SHA256: 3AE2ED9DD32E1053310F2E10612A474568E4E92092B8D4C86D283A3C490582B5
  Defender: found no threats

cubegm/rkgame
  size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9
  source: cubegm/rkgame.stock
  Defender: found no threats
```
