# Patch 038: iCube shell route tracing

Date: 2026-05-28

## Purpose

Add simple boot/menu route logging to the shell scripts around `icube`,
Tiny MC, and `hcprojector`. This is intended to identify which script is
actually being executed on the device when the menu returns to `hcprojector`
or starts `icube`.

## Files

Copy these files over the matching paths on the SD card:

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
```

## Log File

The scripts append trace output to the first writable path:

```text
/mnt/sdcard/cubegm/boot_route.log
/mnt/sdcard/boot_route.log
```

Each trace event includes:

- script path from `$0`;
- shell process id `$$`;
- parent process id `$PPID`;
- current working directory;
- event text;
- a `ps` snapshot.

## Expected Clues

If `icube_start.sh` is used, the log should contain events like:

```text
script=/mnt/sdcard/cubegm/icube_start.sh ... event=start args=
script=/mnt/sdcard/cubegm/icube_start.sh ... event=launching /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc ...
```

If `icube.sh` is used instead, the log should contain:

```text
script=/mnt/sdcard/cubegm/icube.sh ... event=launching /mnt/sdcard/cubegm/icube ...
script=/mnt/sdcard/cubegm/icube.sh ... event=calling init -q
```

If the return-to-projector path is used, the log should contain:

```text
script=/mnt/sdcard/cubegm/icubemp_start.sh ... event=launching /usr/bin/hcprojector
script=/mnt/sdcard/cubegm/icubemp_start.sh ... event=started /usr/bin/hcprojector child=...
```

## Notes

This patch only instruments shell scripts. It does not change the Tiny MC
binary and does not alter `rkgame`, `icube`, `driver.so`, or `hcprojector`.
