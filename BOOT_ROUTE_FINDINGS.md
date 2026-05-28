# Boot Route Findings

Date: 2026-05-28

This note records the latest device-side boot route facts discovered from
`E:/cubegm/boot_route.log` after applying the shell tracing patch.

## Direct Evidence From Device Log

The real cold boot path entered:

```text
/mnt/sdcard/cubegm/icube.sh
```

not:

```text
/mnt/sdcard/cubegm/icube_start.sh
```

Important log lines:

```text
[Thu Jan  1 00:00:04 UTC 1970] script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=start args=
[Thu Jan  1 00:00:04 UTC 1970] script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=launching /mnt/sdcard/cubegm/icube LD_LIBRARY_PATH=:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib
[Thu Jan  1 00:00:04 UTC 1970] script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=started /mnt/sdcard/cubegm/icube child=556
[Thu Jan  1 00:00:04 UTC 1970] script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=calling init -q
[Thu Jan  1 00:00:04 UTC 1970] script=/mnt/sdcard/cubegm/icube.sh pid=548 ppid=1 event=init -q returned rc=0
```

The clock value is not meaningful; the device RTC/system time was still at the
Unix epoch during early boot.

The `ps` snapshot in the same log showed:

```text
528 root     hcdaemon
548 root     {icube.sh} /bin/sh /mnt/sdcard/cubegm/icube.sh
549 root     /bin/sh --
556 root     /mnt/sdcard/cubegm/icube
558 root     rkgame
```

This confirms that the observed script started the stock `icube`, and `icube`
then started `rkgame`.

## Rootfs Evidence

`disk_image/rootfs/etc/inittab` runs BusyBox init startup scripts through:

```text
::sysinit:/etc/init.d/rcS
```

`disk_image/rootfs/etc/init.d/rcS` runs every `/etc/init.d/S??*` script in
numeric order.

`disk_image/rootfs/etc/init.d/S41hcdaemon` contains only:

```sh
hcdaemon&
```

The boot log already shows `hcdaemon` running before and during the
`icube.sh` trace. The most likely cold boot route is therefore:

```text
init -> /etc/init.d/rcS -> /etc/init.d/S41hcdaemon -> hcdaemon -> /mnt/sdcard/cubegm/icube.sh
```

The `icube.sh` process has `ppid=1` in the log, so the exact parent process may
have exited or detached before the trace was written. The `hcdaemon` link is an
inference from rootfs startup scripts plus the live `ps` snapshot.

## Other Route References

Ghidra/decompiled evidence still shows:

```text
hcprojector -> system("/mnt/sdcard/cubegm/icube_start.sh &")
driver.so   -> system("/mnt/sdcard/cubegm/icubemp_start.sh &")
```

So `icube_start.sh` is still relevant for transitions from `hcprojector` into
the game frontend, and `icubemp_start.sh` is still relevant for returning from
the game/display driver path to `hcprojector`.

It is not the cold boot path observed in `boot_route.log`.

## Practical Result

Patching `icube_start.sh` alone does not affect cold boot on this device.

For replacing the stock frontend at boot, patch:

```text
cubegm/icube.sh
```

`disk_image_patch_039` applies this result: it changes `icube.sh` to launch
Tiny MC directly:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

instead of:

```sh
/mnt/sdcard/cubegm/icube &
```

The script keeps the stock `LD_LIBRARY_PATH` setup and `init -q` call shape.

## Next Verification

After copying `disk_image_patch_039` to the SD card, remove the old log file:

```text
/mnt/sdcard/cubegm/boot_route.log
```

On the next boot, the expected evidence is:

```text
script=/mnt/sdcard/cubegm/icube.sh ... event=launching /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc ...
```

If that line appears but Tiny MC still does not stay on screen, the next issue
is inside Tiny MC startup/display/input, not the boot shell route.
