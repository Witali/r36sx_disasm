# disk_image_patch_026

Purpose:

Fix Tiny MC being restarted by the stock `icube` supervisor about every
5-6 seconds.

Finding:

Ghidra analysis of `cubegm/icube` showed that it starts
`/mnt/sdcard/cubegm/rkgame`, sleeps 6 seconds, then checks SysV shared memory
key `0x4d2`. If `shm[1]` does not change, or is zero, it runs:

```text
killall rkgame
```

and starts `rkgame` again.

Stock `rkgame` creates/attaches the same shared memory segment:

```text
shmget(0x4d2, 0x1c4, IPC_CREAT | 0666)
```

and its `XintiaoThread` writes a heartbeat every 20 ms:

```text
shm[0] = 1
shm[1]++
```

Code change:

Tiny MC now attaches the same shared memory segment and updates the heartbeat
from the main loop. It also continues ticking while waiting for a launched child
process, so `icube` should not restart Tiny MC during long child execution.

Files:

```text
cubegm/rkgame
```

Apply:

Copy `cubegm/rkgame` from this patch over the SD card/image `cubegm/rkgame`.
Keep the stock launcher backup as `cubegm/rkgame.stock`.

Build command:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

Analysis command:

```powershell
.\ghidra_12.0.4_PUBLIC\support\analyzeHeadless.bat .\ghidra_projects icube_supervisor -import .\disk_image\cubegm\icube -scriptPath .\ghidra_scripts -postScript ExportDisasmAndDecompile.java .\ghidra_exports\icube -deleteProject
```

Verification:

```text
ELF32 little-endian executable, machine=MIPS.
Program interpreter string: /lib/ld.so.1
Dynamic dependency strings: libc.so.6, libdl.so.2, GLIBC_2.0, GLIBC_2.2
Contains strings: icube heartbeat, tiny_mc.log, cube_ioctl
Size: 38676 bytes
SHA256: AD81FE4356DE2B49FF505256FB233614557AD1EA518447C84821BCC005925BC0
Defender scan homebrew\tiny_mc\tiny_mc: found no threats
Defender scan disk_image_patch_026\cubegm\rkgame: found no threats
```
