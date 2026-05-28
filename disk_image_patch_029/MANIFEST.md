# disk_image_patch_029

Purpose:

Add a scrollbar to Tiny MC when the current directory contains more entries
than can fit on the screen.

Code change:

- Added list/scrollbar UI constants in `tiny_mc.c`.
- Added `draw_scrollbar()`.
- Scrollbar is shown only when `g_entry_count > visible_rows()`.
- Selection highlight and text width are reduced while the scrollbar is visible
  so long filenames do not draw under the scrollbar.
- `ensure_selection_visible()` now clamps `g_scroll` to the maximum valid scroll
  offset.

Files:

```text
cubegm/rkgame
```

Apply:

Copy this patch directory over the SD card/image root.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl
Size: 39636 bytes
SHA256: 3B50D99E3F450205D83015FF5D99D6F30D04207E8E641DF8D112940B81FE9150
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_029\cubegm\rkgame: found no threats
```
