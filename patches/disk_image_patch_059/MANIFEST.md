# disk_image_patch_059: Tiny MC self-exec return from native apps

Purpose:

Fix the black-screen return from native Pong and Button Demo.

Device log finding:

`E:/cubegm/tiny_mc.log` showed that Pong exits and Tiny MC continues running:

```text
launch result: pong exited: 0 raw_status=0x0
display_open
video_drivers_init returned 36
Display via driver.so 640x480
scan_directory done ...
```

The LCD still stayed black with only the battery OSD visible. So the failure is
not `waitpid()` and not child termination. It is the repeated display-driver
reinitialization inside the already-running Tiny MC process.

Implementation:

- After a real child process exits, Tiny MC now executes a fresh copy of itself:

```text
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc <current-directory>
```

- This uses the same clean `driver.so`/framebuffer initialization path as the
normal boot route.
- The previous in-process display reopen path remains only as fallback if
  self-exec fails.

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
Tiny MC size: 56796 bytes
Tiny MC SHA256: 72DB5041C4496DA9489D18CB22E4D14F860274B37336CBEB7DCE64B86EAC427B

Contains strings:
  restarting tiny_mc after child
  /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc
  tiny_mc self-exec failed

Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_059: found no threats
Defender scan patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc:
found no threats
```
