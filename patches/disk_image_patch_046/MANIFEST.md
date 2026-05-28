# disk_image_patch_046

Purpose: update the Tiny MC native Button Demo and Pong executables so their
generated sounds go through `driver.so` `sound_driver_playframe`, matching the
stock `rkgame` LibRetro audio path instead of writing to `/dev/auddec`
directly.

Apply by copying this directory over the root of the SD card / disk image after
the Tiny MC native launcher patches are already installed.

## Files

```text
MIPS_NATIVE/button_demo/button_demo
MIPS_NATIVE/button_demo/README.txt
MIPS_NATIVE/pong/pong
MIPS_NATIVE/pong/README.txt
```

## Build Commands

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\native_button_demo\build_native_button_demo.ps1
powershell -ExecutionPolicy Bypass -File .\homebrew\native_pong\build_native_pong.ps1
```

The first Button Demo rebuild initially failed with `undefined symbol:
__udivdi3` because the new audio helper used a 64-bit divide. The helper was
changed back to the 32-bit phase-step calculation used by the older audio path,
which is safe for these generated tone frequencies and keeps the executable
free of extra libgcc helper dependencies.

## Verification

```text
button_demo size: 16956 bytes
button_demo SHA256: 3BEB30973118B04747D992C79FBFFA2E7555BE9D793B576013B2B3C30C596D1C

pong size: 18888 bytes
pong SHA256: CAB8C5F32DAFD3C53DA157518D43E9E181CED23431E135C435954A83658FC95D

Contains strings: sound_driver_init, sound_driver_playframe
Does not contain string in rebuilt binaries: /dev/auddec

Defender scan homebrew\native_button_demo\button_demo: found no threats
Defender scan homebrew\native_pong\pong: found no threats
Defender scan disk_image\MIPS_NATIVE\button_demo\button_demo: found no threats
Defender scan disk_image\MIPS_NATIVE\pong\pong: found no threats
Defender scan patches\disk_image_patch_046: found no threats
```
