# disk_image_patch_023: Tiny MC logging and menu cleanup

Date: 2026-05-28

Purpose:

Add device-side debug logging to Tiny MC and remove known non-working duplicate
Game Boy menu entries for the Button and Pong demos.

## Files

```text
cubegm/rkgame
cubegm/cores/filelist.xml
GB/filelist.csv
```

## Menu cleanup

Removed non-working menu rows:

```text
button.demo,Button Demo,Button Demo
pong.pong,Pong Demo,Pong Demo
```

Kept the working stock-extension routes:

```text
button.gb,Button Demo GB,Button Demo GB
pong.gb,Pong Demo GB,Pong Demo GB
```

Also removed the matching per-file overrides:

```xml
<file name="GB/button.demo" core="libemu_buttondemo.so" />
<file name="GB/pong.pong" core="libemu_pong.so" />
```

## Fuse and Doom extension notes

Fuse already uses a proper Spectrum tape extension in the current image:

```text
ATARI/R36SX_ZX48.tap
```

and `cubegm/cores/filelist.xml` maps it explicitly to `libemu_fuse.so`.

Doom/PRBoom should use proper Doom content extensions, not `.gb` dummy names:

```text
doom1.wad
freedoom1.wad
doom.m3u
```

Those routes are prepared in the older `disk_image_patch_020`, but the current
local `disk_image` does not contain `libemu_prboom.so` or the WAD/M3U files.
This patch therefore does not add Doom menu rows, avoiding another set of dead
entries. When PRBoom files are copied again, use `.wad`/`.m3u` plus explicit
`filelist.xml` overrides.

## Tiny MC logging

Tiny MC now appends a debug log to the first writable path:

```text
/mnt/sdcard/cubegm/tiny_mc.log
/mnt/sdcard/tiny_mc.log
tiny_mc.log
```

Logged events include:

- startup and selected start directory;
- display driver load attempts and `video_drivers_init()` result;
- fallback `/dev/fb0` setup;
- input device opens/closes;
- directory scans;
- button state changes;
- launched executable path, child wait, and exit status.

The render loop does not log every frame.

## Build

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

## Verification

Tiny MC binary:

```text
ELF32 little-endian executable
Machine: MIPS
Interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains log path string: tiny_mc.log
Size: 36332 bytes
SHA256: BB7EC1F3D97A283DD8C1D80E078A20120A694D51E0A11F11C2E3536E8B076E9B
Defender scan: found no threats
```

Checks:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_023\cubegm\rkgame
Select-String -Path .\disk_image_patch_023\GB\filelist.csv,.\disk_image_patch_023\cubegm\cores\filelist.xml -Pattern "button\.demo|pong\.pong"
```

The final `Select-String` check returns no matches.
