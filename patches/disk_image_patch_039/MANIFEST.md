# Patch 039: launch Tiny MC from the real boot route

Date: 2026-05-28

## Finding

`boot_route.log` from the real device shows that cold boot enters:

```text
/mnt/sdcard/cubegm/icube.sh
```

with parent process `init`. The patched `icube_start.sh` is not used during
this boot path.

The observed sequence was:

```text
script=/mnt/sdcard/cubegm/icube.sh ... event=start args=
script=/mnt/sdcard/cubegm/icube.sh ... event=launching /mnt/sdcard/cubegm/icube ...
script=/mnt/sdcard/cubegm/icube.sh ... event=started /mnt/sdcard/cubegm/icube child=...
script=/mnt/sdcard/cubegm/icube.sh ... event=calling init -q
```

The process snapshot also showed:

```text
hcdaemon
{icube.sh} /bin/sh /mnt/sdcard/cubegm/icube.sh
/mnt/sdcard/cubegm/icube
rkgame
```

## Change

`cubegm/icube.sh` now starts Tiny MC directly:

```sh
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

instead of:

```sh
/mnt/sdcard/cubegm/icube &
```

The script still exports the stock library path and still calls `init -q`,
matching the original script shape.

## Files

Copy these files over the matching paths on the SD card:

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
```

`icube_start.sh` and `icubemp_start.sh` are included unchanged from patch 038
so route tracing remains available.

## Logs

The scripts continue to append diagnostics to:

```text
/mnt/sdcard/cubegm/boot_route.log
/mnt/sdcard/boot_route.log
```

After applying this patch, the expected line is:

```text
script=/mnt/sdcard/cubegm/icube.sh ... event=launching /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc ...
```
