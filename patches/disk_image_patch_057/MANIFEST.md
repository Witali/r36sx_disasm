# disk_image_patch_057: Tiny MC child-return fix

Purpose:

Fix the black-screen / no-menu-return symptom after exiting native Pong or
Button Demo back to Tiny MC.

Root cause hypothesis:

Tiny MC launches native apps like a small Linux shell: it closes its display and
input handles, `fork()`s, the child `exec()`s the selected program, and the
parent waits with `waitpid()`. Native Pong exits with `Select + Start`; when
Tiny MC resumes, `Start` may still be held or still present in the input queue.
Tiny MC uses `Start` as "run selected file", so it can immediately relaunch the
same app before the menu becomes visible.

Implementation:

- Tiny MC now reopens the display after child exit with up to five attempts.
- Tiny MC redraws the menu immediately after reopening.
- Tiny MC polls input after reopening and suppresses all post-launch input until
  every button has been released once.
- `handle_buttons()` returns immediately after launching a child, so stale
  pre-launch button state cannot overwrite the refreshed post-child state.

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
Tiny MC size: 55780 bytes
Tiny MC SHA256: B5E01B7DAFF20C0D82867702F6664A55E969EFDB60A13593F5962A49D14E40B0

Contains strings:
  suppressing post-launch buttons
  post-launch buttons released
  display reopened after child
  display reopen after child failed

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_057: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```
