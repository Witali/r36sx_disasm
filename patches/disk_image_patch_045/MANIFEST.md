# Patch 045 - Tiny MC Button Click Audio

Date: 2026-05-28

## Purpose

Add short button-click sounds to the current Tiny MC launcher overlay.

This patch is a full overlay copy of the current `disk_image_patch_tiny_mc`
state, with the rebuilt Tiny MC executable and README. It can be copied over the
SD-card root like the consolidated Tiny MC patch.

## Files

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
cubegm/rkgame
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
```

## Behavior

- Boot route still enters Tiny MC directly through `cubegm/icube.sh`.
- `icube_start.sh` also launches Tiny MC when called by `hcprojector`.
- `icubemp_start.sh` returns to `hcprojector`.
- `cubegm/rkgame` is the stock frontend binary used after the Tiny MC Fn
  shortcut starts `/mnt/sdcard/cubegm/icube`.
- Tiny MC starts in `/mnt/sdcard/MIPS_NATIVE`.
- Tiny MC uses `driver.so` display/input as before.
- New button presses now trigger short generated clicks through `driver.so`
  `sound_driver_init()` / `sound_driver_playframe()`, matching the stock
  LibRetro audio route more closely than direct `/dev/auddec` writes.
- An initially raised Fn bit is still ignored until release, so startup Fn state
  does not create a click.

## Build And Update Commands

Run from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
New-Item -ItemType Directory -Force -Path disk_image\MIPS_NATIVE\tiny_mc,patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination disk_image\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath homebrew\tiny_mc\tiny_mc -Destination patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc -Force
Copy-Item -LiteralPath disk_image\MIPS_NATIVE\tiny_mc\README.txt -Destination patches\disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\README.txt -Force
New-Item -ItemType Directory -Force -Path patches\disk_image_patch_045
Copy-Item -LiteralPath patches\disk_image_patch_tiny_mc\cubegm -Destination patches\disk_image_patch_045\cubegm -Recurse -Force
Copy-Item -LiteralPath patches\disk_image_patch_tiny_mc\MIPS_NATIVE -Destination patches\disk_image_patch_045\MIPS_NATIVE -Recurse -Force
```

## Verification

```text
Tiny MC:
  Size: 41640 bytes
  SHA256: E43432F1800BDF7049E825CE471063E82694A1972523D50B799E0A3210B7E660
  ELF: class=1 (ELF32), data=1 (little-endian), machine=0x0008 (MIPS)
  Contains: /mnt/sdcard/cubegm/icube, click audio, driver.so sound,
            sound_driver_init, sound_driver_playframe, sound_driver_flush,
            sound_driver_deinit
  Does not contain: icube heartbeat, shmget

rkgame:
  Size: 1178732 bytes
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9

Scripts and README:
  cubegm/icube.sh SHA256:
    432B5388F3584B834372DA57318408E341422AA507B5F8FFD3DC5C87F5EC83AC
  cubegm/icube_start.sh SHA256:
    1C2944DED8E27CB52114CA5734CD2D5A64BFC2471677F7F1458008DA1EBF34CF
  cubegm/icubemp_start.sh SHA256:
    BE313D3917BA5E355ECFC1818A6F42A6644ECE5724A098C507DDE2C7149F0EE9
  MIPS_NATIVE/tiny_mc/README.txt SHA256:
    2E5D8855BD8AC8851C48C38DEA6FDE20330924805A9F07655F293B0837E22BAD

Defender scan:
  patches\disk_image_patch_045\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
  patches\disk_image_patch_045\cubegm\rkgame: found no threats
```
