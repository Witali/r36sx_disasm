# Disk Image Patch 096

Copy the contents of this directory over the original SD-card/disk image root.

This patch updates Pico-286 with optional runtime profiling:

- `MIPS_NATIVE/pico_286/pico_286` rebuilt with profiling support compiled in.
- `MIPS_NATIVE/pico_286/pico_286.conf` adds:

```ini
[profiling]
profiling_enabled=0
profiling_log_ms=5000
```

Set `profiling_enabled=1` to write periodic performance summaries to
`pico_286.log`.
