# disk_image_patch_033

Date: 2026-05-28

Purpose:

Launch Tiny MC directly from `cubegm/icube_start.sh` and remove the Tiny MC
dependency on the `icube` heartbeat/watchdog contract.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

`cubegm/rkgame` in this patch is restored from the stock backup
`cubegm/rkgame.stock`. It is not the Tiny MC compatibility entrypoint from
`disk_image_patch_032`.

## Behavior

`cubegm/icube_start.sh` now does:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

The transition is:

```text
hcprojector
  -> cubegm/icube_start.sh
  -> MIPS_NATIVE/tiny_mc/tiny_mc
```

`cubegm/icube` is not started. Tiny MC is built with:

```c
#define USE_ICUBE_HEARTBEAT 0
```

so it does not attach or update the `icube` SysV shared memory heartbeat.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 39572 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains string: /mnt/sdcard/MIPS_NATIVE
  does not contain strings: shmget, heartbeat
  SHA256: F8DECFD3C397D1783FC64E505C6091D6013CA8514C03AB11ACAA73FBB52BB30A
  Defender: found no threats

cubegm/rkgame
  size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9
  source: cubegm/rkgame.stock
  Defender: found no threats
```
