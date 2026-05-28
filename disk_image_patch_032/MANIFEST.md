# disk_image_patch_032

Date: 2026-05-28

Purpose:

Restore the stock `icube` launch chain and keep Tiny MC opening the root-level
native program directory.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

## iCube Contract

Ghidra export `ghidra_exports/icube/decompiled_all.c` shows that `icube`:

```text
creates SysV shared memory key 0x4d2, size 0x1c4
forks and execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0)
waits 6 seconds
checks shm[1] once per second
runs system("killall rkgame") and restarts rkgame when shm[1] stops changing
```

So `cubegm/icube_start.sh` should start `icube`, and Tiny MC should be present
as `cubegm/rkgame`.

## Behavior

`cubegm/icube_start.sh` now does:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/cubegm/icube &
```

The transition is:

```text
hcprojector
  -> cubegm/icube_start.sh
  -> cubegm/icube
  -> cubegm/rkgame
```

`cubegm/rkgame` in this patch is Tiny MC rebuilt with a no-argument default
start directory of `/mnt/sdcard/MIPS_NATIVE`. The same binary is also copied to
`MIPS_NATIVE/tiny_mc/tiny_mc`.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 40940 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains string: /mnt/sdcard/MIPS_NATIVE
  SHA256: F7D7E26864F247B01EC1660AE1DF91F12E49E3A41F21F7ABDB6B473602ECA0E8
  Defender: found no threats

cubegm/rkgame
  size: 40940 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  contains string: /mnt/sdcard/MIPS_NATIVE
  SHA256: F7D7E26864F247B01EC1660AE1DF91F12E49E3A41F21F7ABDB6B473602ECA0E8
  Defender: found no threats
```
