# disk_image_patch_044

Restore sound in the standalone Tiny MC native Pong and Button Demo programs.

## Background

The old libretro Button Demo and Pong sounds went through `rkgame` audio
callbacks. The standalone `MIPS_NATIVE` executables are launched directly by
Tiny MC, so they do not receive libretro audio callbacks.

Ghidra review:

- `ghidra_exports/icube/decompiled_all.c`: `icube` is only a supervisor for
  `/mnt/sdcard/cubegm/rkgame`; it does not play sounds.
- `ghidra_exports/hcprojector/decompiled_all.c`: `hcprojector` plays UI WAV
  effects by opening `/dev/auddec`, calling ioctl `0x82780301`, calling ioctl
  `0x20000304`, and writing `AvPktHd` + PCM payload packets.

The new native builds reuse that `/dev/auddec` packet model and generate small
PCM effects internally.

## Files

Copy this patch directory over the original SD-card root:

```text
MIPS_NATIVE/button_demo/button_demo
MIPS_NATIVE/button_demo/README.txt
MIPS_NATIVE/pong/pong
MIPS_NATIVE/pong/README.txt
```

## Behavior

- Button Demo: any newly pressed button plays a short two-part generated sound.
- Pong: wall/paddle bounces, score changes, pause/reset, and win/lose states
  play generated sounds.

No external WAV files are required.

## Build Commands

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

## Verification

```text
button_demo:
  ELF class=1, data=1, type=2, machine=8, interpreter /lib/ld.so.1
  Size: 17304 bytes
  SHA256: CF8F8C91432DA770FBCA6BFC9C7C957C9820D351F033E38FA7BB38263CA52FA2
  Strings include: BUTTON DEMO, /mnt/sdcard/cubegm/driver.so, /dev/auddec, cube_ioctl
  Strings do not include: retro_run
  Defender scan: found no threats
  disk_image copy SHA256 matches and Defender scan found no threats

pong:
  ELF class=1, data=1, type=2, machine=8, interpreter /lib/ld.so.1
  Size: 19256 bytes
  SHA256: 8C9E69A38B24F8B9CF054B31A80065A952E9B497A00E810EEBBE1D62948514B0
  Strings include: YOU WIN, YOU LOSE, /mnt/sdcard/cubegm/driver.so, /dev/auddec, cube_ioctl
  Strings do not include: retro_run
  Defender scan: found no threats
  disk_image copy SHA256 matches and Defender scan found no threats
```
