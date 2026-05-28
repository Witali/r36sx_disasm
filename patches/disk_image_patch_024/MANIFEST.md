# disk_image_patch_024: Tiny MC rkgame-style button input

Date: 2026-05-28

Purpose:

Fix Tiny MC controls after device testing showed that the UI draws correctly but
buttons do not respond. Decompilation showed that stock `rkgame` does not rely
on Linux joystick/evdev for the built-in controls; it reads `/tmp/joy_key`
through `driver.so` `cube_ioctl`.

## Files

```text
cubegm/rkgame
```

## What changed

Tiny MC now resolves `cube_ioctl` from `cubegm/driver.so` and polls the same
shared input state as `rkgame`:

```text
cube_ioctl(0x40050209, &joy_key_shm_addr)  -> /tmp/joy_key shared memory
cube_ioctl(0x40050208, &cube_state_reg)    -> game/status register
```

It reads two 32-bit words from `/tmp/joy_key`, translates the stock `rkgame`
bitmasks, and ORs that result with the older `/dev/input/js*` and evdev
fallback paths.

Known `rkgame` key masks from `joykey_explaned()`:

```text
SELECT 0x00000001
START  0x00000008
UP     0x00000010
RIGHT  0x00000020
DOWN   0x00000040
LEFT   0x00000080
X      0x00001000
A      0x00002000
B      0x00004000
Y      0x00008000
FN     0x00010000
```

Tiny MC maps:

```text
UP/DOWN/LEFT/RIGHT -> navigation
A or X             -> confirm/run
B or Y             -> back
START              -> run
SELECT             -> parent directory
```

## Debug log

`tiny_mc.log` now also records raw rkgame input changes:

```text
cube_ioctl joy key shm rc=... addr=...
cube input raw0=0x... raw1=0x... state=0x... buttons=0x...
```

## Build

Build command from repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

## Verification

Tiny MC binary:

```text
ELF32 little-endian executable
Machine: MIPS
Interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: cube_ioctl, cube input raw, joy key shm, tiny_mc.log
Size: 37040 bytes
SHA256: 91F96858C58AA6D2E4DFBA7539A0C01061DE39F195174C9EB80AA6541E9D1037
Defender scan: found no threats
```

Checks run:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_024\cubegm\rkgame
rg -a -o "cube_ioctl|cube input raw|joy key shm|tiny_mc\.log|ld\.so\.1|libc\.so\.6|libdl\.so\.[0-9]+|GLIBC_[0-9]+\.[0-9]+" .\disk_image_patch_024\cubegm\rkgame
Get-FileHash -Algorithm SHA256 .\disk_image_patch_024\cubegm\rkgame
```
