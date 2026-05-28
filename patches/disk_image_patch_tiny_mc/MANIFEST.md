# Tiny MC Install Patch

Date: 2026-05-28

## Purpose

This named patch is the consolidated overlay needed to boot into Tiny MC on the
device and still allow returning to the stock `icube`/`rkgame` frontend.

It combines the current working Tiny MC route from the numbered patch history:

- boot enters `cubegm/icube.sh`;
- `icube.sh` launches Tiny MC directly;
- `icube_start.sh` also launches Tiny MC when called from `hcprojector`;
- `icubemp_start.sh` keeps the stock return-to-projector route;
- Tiny MC's Fn shortcut starts the stock `/mnt/sdcard/cubegm/icube` binary
  directly.

## Install

Copy the contents of this folder to the SD-card root, preserving directories.

Example target paths:

```text
E:/cubegm/icube.sh
E:/cubegm/icube_start.sh
E:/cubegm/icubemp_start.sh
E:/cubegm/rkgame
E:/MIPS_NATIVE/tiny_mc/tiny_mc
E:/MIPS_NATIVE/tiny_mc/README.txt
```

## Files

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
cubegm/rkgame
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
```

## Behavior

`cubegm/icube.sh`:

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
init -q
```

`cubegm/icube_start.sh`:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

`cubegm/icubemp_start.sh`:

```sh
killall rkgame rkgame.stock tiny_mc icube
/usr/bin/hcprojector &
```

Tiny MC:

- starts in `/mnt/sdcard/MIPS_NATIVE`;
- launches normal MIPS executables with `fork()` + `execl()`;
- uses `/mnt/sdcard/cubegm/driver.so` for display and built-in controls;
- has `DEBUG=1`, so it writes `tiny_mc.log` to the first writable path:
  `/mnt/sdcard/cubegm/tiny_mc.log`, `/mnt/sdcard/tiny_mc.log`, or
  `tiny_mc.log`;
- has `USE_ICUBE_HEARTBEAT=0`, so it does not attach to the old `icube`
  SysV heartbeat;
- has Fn shortcut enabled. It ignores an initially raised Fn bit until Fn is
  released once, then a later Fn press executes `/mnt/sdcard/cubegm/icube`
  directly.

`cubegm/rkgame` is included as the stock game frontend binary so the Fn-started
stock `icube` supervisor has the expected target to launch.

## Verification

```text
Tiny MC:
  Size: 40776 bytes
  SHA256: 1C01D7B90C58FBB3ADCDA99AC18C6BB5D8AA666AFEA34D09FC8F2B1360A21B5F
  Contains: /mnt/sdcard/cubegm/icube, FN shortcut armed after release,
            FN startup state ignored until release
  Does not contain: icube heartbeat, shmget

rkgame:
  Size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Scripts and README:
  cubegm/icube.sh SHA256:
    432B5388F3584B834372DA57318408E341422AA507B5F8FFD3DC5C87F5EC83AC
  cubegm/icube_start.sh SHA256:
    1C2944DED8E27CB52114CA5734CD2D5A64BFC2471677F7F1458008DA1EBF34CF
  cubegm/icubemp_start.sh SHA256:
    BE313D3917BA5E355ECFC1818A6F42A6644ECE5724A098C507DDE2C7149F0EE9
  MIPS_NATIVE/tiny_mc/README.txt SHA256:
    93D344A5CC0E5B4FD2E48D376AC6DA52C0D74E47D9CF7B1FD8E2E8562F1367EA

Defender scan:
  disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
  disk_image_patch_tiny_mc\cubegm\rkgame: found no threats
```

The temporary `boot_route.log` tracing from the discovery patches is not
included.
