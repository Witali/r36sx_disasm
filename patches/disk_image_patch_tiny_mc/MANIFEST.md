# Tiny MC Install Patch

Date: 2026-05-28

## Purpose

This named patch is the consolidated overlay needed to boot into Tiny MC on the
device and still allow returning to the stock `icube`/`rkgame` frontend. Tiny
MC also plays short button-click sounds through the same `driver.so` sound path
used by stock LibRetro cores and renders text through the stock FreeType
library when available.

It combines the current working Tiny MC route from the numbered patch history:

- boot enters `cubegm/icube.sh`;
- `icube.sh` launches Tiny MC directly;
- `icube_start.sh` also launches Tiny MC when called from `hcprojector`;
- `icubemp_start.sh` keeps the stock return-to-projector route;
- Tiny MC's Fn shortcut starts the stock `/mnt/sdcard/cubegm/icube` binary
  directly.

## Install

Copy the contents of this folder to the SD-card root, preserving directories.

Example target paths:

```text
E:/cubegm/icube.sh
E:/cubegm/icube_start.sh
E:/cubegm/icubemp_start.sh
E:/cubegm/rkgame
E:/MIPS_NATIVE/tiny_mc/tiny_mc
E:/MIPS_NATIVE/tiny_mc/tiny_mc.conf
E:/MIPS_NATIVE/tiny_mc/README.txt
E:/MIPS_NATIVE/common/fonts/
```

## Files

```text
cubegm/icube.sh
cubegm/icube_start.sh
cubegm/icubemp_start.sh
cubegm/rkgame
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/tiny_mc.conf
MIPS_NATIVE/tiny_mc/README.txt
MIPS_NATIVE/common/fonts/
```

## Behavior

`cubegm/icube.sh`:

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
init -q
```

`cubegm/icube_start.sh`:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
```

`cubegm/icubemp_start.sh`:

```sh
killall rkgame rkgame.stock tiny_mc icube
/usr/bin/hcprojector &
```

Tiny MC:

- starts in `/mnt/sdcard/MIPS_NATIVE`;
- launches normal MIPS executables with `fork()` + `execl()`;
- uses `/mnt/sdcard/cubegm/driver.so` for display and built-in controls;
- uses `/mnt/sdcard/cubegm/driver.so` `sound_driver_init()` and
  `sound_driver_playframe()` for short button-click sounds;
- preserves the currently applied mixer volume around `sound_driver_init()`,
  because vendor `driver.so` reapplies its saved AV volume during audio init;
- tries `/mnt/sdcard/cubegm/lib/libfreetype.so.6` for TrueType text rendering;
- uses `/mnt/sdcard/MIPS_NATIVE/common/fonts/JetBrainsMonoNL-Regular.ttf`
  as the preferred monospaced font, with other common mono fonts and the
  built-in 5x7 bitmap font as fallbacks;
- reads `/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc.conf` for the runtime font
  path and text-size settings;
- has `DEBUG=1`, so it writes `tiny_mc.log` to the first writable path:
  `/mnt/sdcard/cubegm/tiny_mc.log`, `/mnt/sdcard/tiny_mc.log`, or
  `tiny_mc.log`;
- has `USE_ICUBE_HEARTBEAT=0`, so it does not attach to the old `icube`
  SysV heartbeat;
- after a launched native app exits, reopens/redraws the display and ignores
  held post-launch buttons until all buttons are released once;
- normal directory navigation updates its previous-button state before
  returning, so a held `A` cannot immediately activate `..`;
- has Fn shortcut enabled. It ignores an initially raised Fn bit until Fn is
  released once, then a later Fn press executes `/mnt/sdcard/cubegm/icube`
  directly.

`cubegm/rkgame` is included as the stock game frontend binary so the Fn-started
stock `icube` supervisor has the expected target to launch.

## Verification

```text
Tiny MC:
  Size: 56420 bytes
  SHA256: FA262555F116BE3E5C37E386B519D5C70EDDF7D1619766BEED71B9BBBF153CC0
  Contains: /mnt/sdcard/cubegm/icube, FN shortcut armed after release,
            FN startup state ignored until release, click audio,
            sound_driver_init, sound_driver_playframe, libfreetype.so.6,
            FT_Init_FreeType, MIPS_NATIVE/common/fonts,
            JetBrainsMonoNL-Regular.ttf, LiberationMono-Regular.ttf,
            DejaVuSansMono.ttf, NotoSansMono-Regular.ttf,
            SourceCodePro-Regular.ttf, Hack-Regular.ttf,
            RobotoMono-Regular.ttf, tiny_mc.conf, config loaded,
            preserving mixer volume, restored mixer volume,
            suppressing post-launch buttons, display reopened after child
  Does not contain: icube heartbeat, shmget

Tiny MC config:
  tiny_mc.conf size: 452 bytes
  tiny_mc.conf SHA256:
    06F53A12D7B4EBC88B587FABF0CB531FB601041E78D025AC0F58CECF443F5D70

Tiny MC common fonts:
  JetBrainsMonoNL-Regular.ttf size: 208576 bytes
  JetBrainsMonoNL-Regular.ttf SHA256:
    FB3B2575D7B0657359707993288F12A7360344D39387BB26050E276D61F6BD2A
  LiberationMono-Regular.ttf SHA256:
    F2B83C763E8AFD21709333370BED4774337FAE82267937E2B5AEA7E2FBD922C1
  DejaVuSansMono.ttf SHA256:
    B4A6C3E4FAAB8773F4FF761D56451646409F29ABEDD68F05D38C2DF667D3C582
  NotoSansMono-Regular.ttf SHA256:
    D9E2B23D19F8230BE7146F409A52B1D23117E635E28F2E2892CF91B7382F325B
  SourceCodePro-Regular.ttf SHA256:
    74BD80D3E42A08517CD7E1108BA3D86F2DA29AC0F3065BE95E0357956AB9DB37
  Hack-Regular.ttf SHA256:
    15F55CC0C85A2988D2B4B3A8CDB5D77FDFBAF319E1BB5309D725DB9818FB7125
  RobotoMono-Regular.ttf SHA256:
    AF0BFF7599C3DF3831755C16E39B3C496DF74B8C8D8A1161B14DC8461BE17CB4

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
    24FC02117F37648231C9F6BE829BB5BC335915E5B61A2799363BED1253980395
  MIPS_NATIVE/common/fonts/README.md SHA256:
    6F1042BB337FB27FEF841F5DB370B6878AB24B0139C53BC497DA6C008DE79ACB

Defender scan:
  disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc\tiny_mc: found no threats
  disk_image_patch_tiny_mc\MIPS_NATIVE\common\fonts: found no threats
  disk_image_patch_tiny_mc\MIPS_NATIVE\tiny_mc: found no threats
  disk_image_patch_tiny_mc\cubegm\rkgame: found no threats
```

The temporary `boot_route.log` tracing from the discovery patches is not
included.
