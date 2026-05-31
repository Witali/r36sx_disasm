# disk_image_patch_boot_time

Copy the contents of this patch over the original Linux root filesystem.

This patch adds a minimal software-clock workaround for devices without a
real-time clock.  BusyBox init runs `/etc/init.d/rcS`, and `rcS` sources
`/etc/init.d/S??*.sh` scripts in order.  The new `S00settime.sh` runs early and
sets the Linux software clock from the last saved timestamp.

Installed files:

- `rootfs/etc/init.d/S00settime.sh`
- `rootfs/etc/last_time`
- `rootfs/usr/bin/save_last_time.sh`

Behavior:

- On boot, `S00settime.sh` reads `/etc/last_time` first.
- If `/mnt/sdcard/.last_time` is already available, it may be used as a
  fallback.
- If no saved timestamp exists, it falls back to `2026-05-31 00:00:00`.
- `save_last_time.sh` writes the current software clock to `/etc/last_time`
  and, when available, `/mnt/sdcard/.last_time`.

To update the saved time from an app or shutdown hook:

```sh
sh /usr/bin/save_last_time.sh
```

This does not recover real elapsed time while the console is powered off.  It
only prevents logs, screenshots, and file timestamps from falling back to
`1970-01-01` on every boot.
