# disk_image_patch_037

Date: 2026-05-28

Purpose:

Temporarily disable Tiny MC Fn handling completely. This is a stronger fix than
`disk_image_patch_036`: Tiny MC no longer contains the Fn shortcut or the
`/mnt/sdcard/cubegm/icube` path at all.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

## Behavior

`cubegm/icube_start.sh` still starts Tiny MC directly:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

Tiny MC is built with:

```c
#define USE_ICUBE_HEARTBEAT 0
#define ENABLE_FN_ICUBE_SHORTCUT 0
```

So it does not attach the `icube` heartbeat and does not handle Fn as a jump to
`icube`.

`cubegm/rkgame` is restored from `cubegm/rkgame.stock`.

## iCube Structure Check

The original `icube` binary is a small userspace supervisor:

```text
ShareMemCreat()
fork()
execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0)
sleep(6)
repeat once per second:
  compare shm[1]
  if unchanged or zero:
    system("killall rkgame")
    fork + execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0)
```

`hcprojector` calls `/mnt/sdcard/cubegm/icube_start.sh &`. `driver.so` contains
the return path `/mnt/sdcard/cubegm/icubemp_start.sh &`.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 39572 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  does not contain strings: /mnt/sdcard/cubegm/icube, FN, shmget, heartbeat
  SHA256: F8DECFD3C397D1783FC64E505C6091D6013CA8514C03AB11ACAA73FBB52BB30A
  Defender: found no threats

cubegm/rkgame
  size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9
  source: cubegm/rkgame.stock
  Defender: found no threats
```
