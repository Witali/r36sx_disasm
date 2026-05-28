# Patch 042: Tiny MC Fn starts stock iCube

Date: 2026-05-28

## Purpose

Restore the Tiny MC Fn shortcut for starting the stock `icube` supervisor after
the confirmed boot route through `icube.sh` started Tiny MC correctly.

## Files

Copy these files over the matching paths on the SD card:

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
```

## Behavior

- Fn handling is enabled with `ENABLE_FN_ICUBE_SHORTCUT 1`.
- Tiny MC ignores a raised Fn bit during startup until Fn has been released
  once.
- A later Fn press closes Tiny MC display/input handles and executes:

```text
/mnt/sdcard/cubegm/icube
```

directly.

It intentionally does not start `icube.sh`, because the confirmed boot version
of `icube.sh` now launches Tiny MC and would loop back into Tiny MC.

## Build

Built from the repository root with:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

## Verification

```text
Contains strings: /mnt/sdcard/cubegm/icube, FN shortcut armed after release,
FN startup state ignored until release, FN pressed: launching %s
Does not contain strings: icube heartbeat, shmget
Size: 40776 bytes
SHA256: 1C01D7B90C58FBB3ADCDA99AC18C6BB5D8AA666AFEA34D09FC8F2B1360A21B5F
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_042\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
```
