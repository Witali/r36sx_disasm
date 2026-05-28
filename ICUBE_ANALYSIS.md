# iCube Analysis

Date: 2026-05-28

This note summarizes the original `cubegm/icube` launch structure as observed in
the firmware image and Ghidra exports.

## Files

```text
disk_image/cubegm/icube
disk_image/cubegm/icube_start.sh
disk_image/cubegm/icube.sh
disk_image/cubegm/icubemp_start.sh
disk_image/cubegm/rkgame
disk_image/cubegm/rkgame.stock
```

## Launch Chain

`hcprojector` calls:

```sh
/mnt/sdcard/cubegm/icube_start.sh &
```

The original-style `icube_start.sh` kills `hcprojector`, sets
`LD_LIBRARY_PATH`, and starts:

```sh
/mnt/sdcard/cubegm/icube &
```

`icube.sh` is a second helper that also starts `icube`, then calls `init -q`.
It does not appear to be the path called by the decompiled `hcprojector` game
transition.

`icubemp_start.sh` is the return-to-projector path:

```sh
killall rkgame rkgame.stock tiny_mc icube
/usr/bin/hcprojector &
```

`driver.so` contains a call to:

```sh
/mnt/sdcard/cubegm/icubemp_start.sh &
```

## iCube Binary

Ghidra export `ghidra_exports/icube/decompiled_all.c` shows `icube v1.3`.

The main loop:

```c
ShareMemCreat();
fork();
execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0);
sleep(6);

for (;;) {
    current = shm[1];
    shm[0] = 0;
    if (previous == current || current == 0) {
        system("killall rkgame");
        fork();
        execl("/mnt/sdcard/cubegm/rkgame", "rkgame", 0);
        current = 0;
        sleep(6);
    }
    sleep(1);
    previous = current;
}
```

`ShareMemCreat()` uses:

```c
shmget(0x4d2, 0x1c4, 0x3b6);
shmat(shmid, NULL, 0);
```

So `icube` is a small userspace supervisor for `cubegm/rkgame`. It does not
take arguments and does not choose an alternate executable.

## Current Direct Tiny MC Mode

The direct Tiny MC patches intentionally bypass `icube`:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

`disk_image_patch_037` additionally disables Tiny MC's Fn shortcut entirely.
The patched Tiny MC binary was verified not to contain these strings:

```text
/mnt/sdcard/cubegm/icube
FN
shmget
heartbeat
```

If the device still enters `icube` after applying `disk_image_patch_037`, the
likely explanations are:

- the device is still running an older `MIPS_NATIVE/tiny_mc/tiny_mc`;
- `cubegm/icube_start.sh` on the SD card was not overwritten by the direct
  Tiny MC version;
- another boot/menu path is launching `cubegm/icube` or `icube.sh` instead of
  the `icube_start.sh` file we patched.

## Shell Route Trace

`disk_image_patch_038` instruments these scripts:

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
```

They append route diagnostics to:

```text
/mnt/sdcard/cubegm/boot_route.log
/mnt/sdcard/boot_route.log
```

Each event records `$0`, `$$`, `$PPID`, current directory, and a `ps` snapshot.
This should show whether the device is entering through `icube_start.sh`,
`icube.sh`, or the return-to-projector `icubemp_start.sh` path.

## Observed Device Route

`E:/cubegm/boot_route.log` from the device showed:

```text
script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=start args=
script=/mnt/sdcard/cubegm/icube.sh ... event=launching /mnt/sdcard/cubegm/icube ...
script=/mnt/sdcard/cubegm/icube.sh ... event=started /mnt/sdcard/cubegm/icube child=556
script=/mnt/sdcard/cubegm/icube.sh ... event=calling init -q
```

The process snapshot contained:

```text
hcdaemon
{icube.sh} /bin/sh /mnt/sdcard/cubegm/icube.sh
/mnt/sdcard/cubegm/icube
rkgame
```

So the cold boot path is not `hcprojector -> icube_start.sh`. It is
`init/rcS -> S41hcdaemon -> hcdaemon -> /mnt/sdcard/cubegm/icube.sh`, with
the script process parented to `init` by the time it logs.

`disk_image_patch_039` therefore changes `icube.sh` itself to launch:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

instead of `/mnt/sdcard/cubegm/icube &`.
