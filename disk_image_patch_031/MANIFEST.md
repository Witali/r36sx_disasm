# disk_image_patch_031

Date: 2026-05-28

Purpose:

Move Tiny MC to the native application area and route the stock transition
script to it directly.

Apply this overlay to the SD/image root.

## Files

```text
MIPS_NATIVE/tiny_mc/tiny_mc
MIPS_NATIVE/tiny_mc/README.txt
cubegm/icube_start.sh
cubegm/rkgame
```

## Behavior

`cubegm/icube_start.sh` now does:

```sh
killall hcprojector
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/mnt/sdcard/cubegm/lib:/mnt/sdcard/cubegm/usr/lib;
if [ -f /mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc ]; then
	/mnt/sdcard/MIPS_NATIVE/tiny_mc/tiny_mc /mnt/sdcard/MIPS_NATIVE &
else
	/mnt/sdcard/cubegm/icube &
fi
```

So the transition is now:

```text
hcprojector
  -> cubegm/icube_start.sh
  -> MIPS_NATIVE/tiny_mc/tiny_mc
```

`cubegm/rkgame` in this patch is restored from the stock backup
`cubegm/rkgame.stock`; it is no longer Tiny MC.

## Verification

```text
MIPS_NATIVE/tiny_mc/tiny_mc
  size: 40768 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  SHA256: D6F2FAD29C234F200D7BE73825935BE49F29085ECE96F1B3A0EF7B70A6334949
  Defender: found no threats

cubegm/rkgame
  size: 1178732 bytes
  ELF: class=1, data=1, type=0x2, machine=0x8, interpreter=/lib/ld.so.1
  SHA256: 57D8B4FD85E0AAB44D17A51D209879C3F98130D066B622142736B42AD08DDCB9
  Defender: found no threats
```
