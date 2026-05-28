# disk_image_patch_056: preserve native audio volume

Purpose:

Prevent native applications from changing the currently selected mixer volume
when they initialize sound through `driver.so`.

Root cause:

`driver.so` `sound_driver_init()` opens the audio decoder and also reapplies the
saved AV volume to `/dev/sndC0i2so`. Stock `rkgame` does this once at frontend
startup, but native Tiny MC, Button Demo, and Pong initialize audio per process,
so the volume can jump while switching between programs.

Implementation:

- `homebrew/common/driver_audio.h` now exposes
  `r36sx_driver_audio_bind_preserve_volume()`.
- Native Button Demo and native Pong call that helper with their resolved
  `cube_ioctl` symbol.
- Tiny MC preserves/restores volume in its own `audio_driver_open()` path and
  logs the saved value when `DEBUG=1`.

Files to copy over the original disk image:

```text
MIPS_NATIVE/button_demo/button_demo
MIPS_NATIVE/pong/pong
MIPS_NATIVE/tiny_mc/tiny_mc
```

Build commands run from repository root:

```powershell
.\homebrew\native_button_demo\build_native_button_demo.ps1
.\homebrew\pong\build_native_pong.ps1
.\homebrew\tiny_mc\build_tiny_mc.ps1
```

Verification:

```text
Button Demo size: 19928 bytes
Button Demo SHA256: 348B37F0F4ACCC0CEC54F396EA5B94EE5E655181DA419CD8D7D01A0299E9D59F

Pong size: 19284 bytes
Pong SHA256: D011576B1235901DF9E0BB844D3688967372CA77B923CA4235FC12F9DFE05642

Tiny MC size: 54516 bytes
Tiny MC SHA256: 144F89FA72FE1372B7E3F7E522BFD40C4F66243B149B4963E8D877AB51177B94

Contains strings: sound_driver_init, sound_driver_playframe,
preserving mixer volume, restored mixer volume

Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan homebrew\pong\pong: found no threats
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\pong\pong: found no threats
Defender scan disk_image\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
Defender scan patches\disk_image_patch_056: found no threats
```
