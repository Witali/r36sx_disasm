# disk_image_patch_058: Tiny MC directory-enter debounce fix

Purpose:

Fix a regression where pressing `A` to enter a directory could briefly enter it
and immediately return to the parent directory.

Root cause:

`launch_selected()` is used for both executable launch and directory enter. The
previous child-return fix added an early `return` after `launch_selected()` so a
real child-process return would not overwrite the refreshed post-child input
state. For directory navigation, though, no child process is launched, so that
early return skipped the normal `g_prev_buttons` update. A still-held `A` was
seen as a new press on the next frame, often activating `..`.

Implementation:

- Added a shared `finish_button_frame()` helper for normal Tiny MC navigation.
- `launch_selected()` now returns whether it actually launched/recovered from a
  child process.
- Directory enter/back actions update the previous-button state before
  returning from `handle_buttons()`.
- The child-process return path still keeps the post-child input suppression
  added in `disk_image_patch_057`.

Files to copy over the original disk image:

```text
MIPS_NATIVE/tiny_mc/tiny_mc
```

Build command run from repository root:

```powershell
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Verification:

```text
Tiny MC size: 56420 bytes
Tiny MC SHA256: FA262555F116BE3E5C37E386B519D5C70EDDF7D1619766BEED71B9BBBF153CC0

Contains strings:
  suppressing post-launch buttons
  post-launch buttons released
  display reopened after child

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_058: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```
