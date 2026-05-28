# disk_image_patch_036

Date: 2026-05-28

Purpose:

Fix Tiny MC immediately jumping into `icube` on startup after the Fn shortcut
was added.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

## Cause

Tiny MC direct launch was correct, but the Fn shortcut introduced in
`disk_image_patch_034` treated any initially raised Fn bit as a new button
press because `g_prev_buttons` starts at zero. On hardware where the Fn bit is
already set or noisy during startup, Tiny MC immediately ran:

```text
/mnt/sdcard/cubegm/icube
```

which made it look as if Tiny MC never started.

## Fix

Tiny MC now arms the Fn shortcut only after it has first observed Fn released.

Startup behavior:

```text
Fn high during first frame -> ignored
Fn remains high            -> ignored
Fn released                -> shortcut armed
Fn pressed again           -> launch /mnt/sdcard/cubegm/icube
```

`cubegm/icube_start.sh` still starts Tiny MC directly:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

`cubegm/rkgame` is restored from `cubegm/rkgame.stock`, so the Fn shortcut can
still enter the stock `icube -> rkgame` path after a real Fn press.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 40776 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains strings: /mnt/sdcard/cubegm/icube, FN startup state ignored until release, FN shortcut armed after release
  does not contain strings: shmget, heartbeat
  SHA256: 7E930951F3E14B60C030123234E249E9DCC3865B0F78EF7482EA3D2554FDBFFB
  Defender: found no threats

cubegm/rkgame
  size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9
  source: cubegm/rkgame.stock
  Defender: found no threats
```
