# Build log: libemu_buttondemo.so

Date: 2026-05-23

Workspace:

```text
C:\Work\r36sx_disasm
```

Output:

```text
homebrew\libretro_button_demo\libemu_buttondemo.so
```

SHA256:

```text
7E11D6F6566A244DEF38F93EB6F620DED7CE7F239A0AE9E4C2F885BDDC51430C
```

Latest rebuilt SHA256 after the 2026-05-24 compatibility fix:

```text
57EDDED0F60023B0473E7E5CC36AD5E635A1CF95A8C7EE4AC2EC2035126FAA91
```

## Goal

Build a minimal MIPS32 little-endian libretro core for the `cubegm/rkgame` launcher.

The module draws a 320x240 RGB565 framebuffer and reacts to joypad buttons:

- D-pad: move square
- A/B/X/Y: change square color
- Start: toggle checker background
- Select: reset square position

## Source files created

```text
homebrew\libretro_button_demo\button_demo.c
homebrew\libretro_button_demo\Makefile
homebrew\libretro_button_demo\README.md
homebrew\libretro_button_demo\BUILD_LOG.md
```

## Security rule added

Files:

```text
tools\SECURITY_RULES.md
tools\scan-download.ps1
```

Rule: every downloaded tool/archive and extracted tool directory must be scanned before use.

Scan command:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 PATH
```

## Downloaded tools

### SF3000 SDK archive

Downloaded:

```text
tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

URL:

```text
https://github.com/game-de-it/sf3000/releases/download/sf3000_toolchain_v0.1/mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

Command:

```powershell
curl.exe -L --fail --retry 3 --output .\tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz https://github.com/game-de-it/sf3000/releases/download/sf3000_toolchain_v0.1/mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

Defender scan:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz
```

Result:

```text
found no threats
```

Attempted extraction:

```powershell
tar -xzf .\tools\mipsel-buildroot-linux-gnu_sdk-buildroot.tar.gz -C .\tools
```

Result:

Windows `tar` partially extracted the SDK but failed on Linux symlinks/hardlinks. The compiler binaries inside the SDK are Linux x86_64 ELF programs requiring:

```text
/lib64/ld-linux-x86-64.so.2
```

Direct execution from Windows PowerShell failed:

```powershell
.\tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain\bin\mips-mti-linux-gnu-gcc --version
```

Result:

```text
Access is denied
```

Conclusion: this SDK is the best target match, but it needs WSL/Linux. It was not used for the Windows build.

The partially extracted SDK directory was also scanned:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\tools\mipsel-buildroot-linux-gnu_sdk-buildroot
```

Result:

```text
found no threats
```

### Zig 0.16.0 for Windows

Downloaded:

```text
tools\zig-x86_64-windows-0.16.0.zip
```

URL:

```text
https://ziglang.org/download/0.16.0/zig-x86_64-windows-0.16.0.zip
```

Command:

```powershell
curl.exe -L --fail --retry 3 --output .\tools\zig-x86_64-windows-0.16.0.zip https://ziglang.org/download/0.16.0/zig-x86_64-windows-0.16.0.zip
```

Defender scan of archive:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\tools\zig-x86_64-windows-0.16.0.zip
```

Result:

```text
found no threats
```

Extraction:

```powershell
tar -xf .\tools\zig-x86_64-windows-0.16.0.zip -C .\tools
```

Defender scan of extracted directory:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\tools\zig-x86_64-windows-0.16.0
```

Result:

```text
found no threats
```

Version check:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe version
```

Result:

```text
0.16.0
```

## Build commands

Zig initially failed because it tried to use the default global cache under the user profile:

```text
C:\Users\rudol\AppData\Local\zig
```

That location was not writable from this sandbox. Local cache directories were created:

```powershell
New-Item -ItemType Directory -Force .\tools\zig-cache | Out-Null
New-Item -ItemType Directory -Force .\tools\zig-global-cache | Out-Null
```

Environment variables used:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
```

Final build command:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

Why `-nostdlib`:

The demo core does not need libc and exports only libretro callbacks. Avoiding libc keeps the output small and avoids dependency mismatch with the device rootfs.

During the first Zig attempt, `button_demo.c` included `<string.h>` for `memset`. With `-nostdlib`, this failed:

```text
fatal error: 'string.h' file not found
```

Fix applied:

- removed `#include <string.h>`;
- replaced `memset` with local `zero_memory()`.

## Verification commands

Check output file:

```powershell
Get-Item .\homebrew\libretro_button_demo\libemu_buttondemo.so | Select-Object FullName,Length,LastWriteTime
```

Result:

```text
Length: 28584
```

Check ELF header:

```powershell
$b=[System.IO.File]::ReadAllBytes((Resolve-Path .\homebrew\libretro_button_demo\libemu_buttondemo.so))
"ELF magic: {0:X2} {1:X2} {2:X2} {3:X2}; class={4}; endian={5}; type=0x{6:X2}{7:X2}; machine=0x{8:X2}{9:X2}" -f $b[0],$b[1],$b[2],$b[3],$b[4],$b[5],$b[17],$b[16],$b[19],$b[18]
```

Result:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008
```

Meaning:

```text
class=1   -> ELF32
endian=1  -> little-endian
type=0003 -> shared object
machine=8 -> MIPS
```

Check exported/embedded symbol names:

```powershell
rg -a -o "retro_[A-Za-z0-9_]+|SetFrameSkip|check_encrypty|libemu_buttondemo" .\homebrew\libretro_button_demo\libemu_buttondemo.so | Sort-Object -Unique
```

Important symbols found:

```text
retro_api_version
retro_cheat_reset
retro_cheat_set
retro_deinit
retro_get_memory_data
retro_get_memory_size
retro_get_region
retro_get_system_av_info
retro_get_system_info
retro_init
retro_load_game
retro_load_game_special
retro_reset
retro_run
retro_serialize
retro_serialize_size
retro_set_audio_sample
retro_set_audio_sample_batch
retro_set_controller_port_device
retro_set_environment
retro_set_input_poll
retro_set_input_state
retro_set_video_refresh
retro_unload_game
retro_unserialize
SetFrameSkip
check_encrypty
CheckEncrypty
```

SHA256:

```powershell
Get-FileHash .\homebrew\libretro_button_demo\libemu_buttondemo.so -Algorithm SHA256
```

Result:

```text
7E11D6F6566A244DEF38F93EB6F620DED7CE7F239A0AE9E4C2F885BDDC51430C
```

## 2026-05-24 compatibility rebuild

Reason:

Ghidra export of `disk_image\cubegm\cores\libemu_tgbdual.so` showed that stock `retro_load_game()` continues only when `check_encrypty()` returns non-zero. The first demo build returned `0`, which could make the launcher reject or fail to load the module if it probes the same symbol.

Source changes:

- `check_encrypty()` now returns `1`.
- added `CheckEncrypty(const void *)`, returning `1`, to match the upper-case stock export.
- `retro_set_audio_sample_batch()` now stores its callback.
- `retro_run()` sends a silent 735-frame stereo audio batch when the callback exists.
- `retro_get_system_av_info()` reports `44100.0` Hz sample rate.

Build command:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -shared -fPIC -nostdlib '-Wl,-soname,libemu_buttondemo.so' -o .\homebrew\libretro_button_demo\libemu_buttondemo.so .\homebrew\libretro_button_demo\button_demo.c
```

Verification:

```text
ELF magic: 7F 45 4C 46; class=1; endian=1; type=0x0003; machine=0x0008
SHA256: 57EDDED0F60023B0473E7E5CC36AD5E635A1CF95A8C7EE4AC2EC2035126FAA91
Defender: found no threats
```

Installed copies:

```text
disk_image\cubegm\cores\libemu_buttondemo.so
disk_image_patch_002\cubegm\cores\libemu_buttondemo.so
```

Defender scan of built module:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\libretro_button_demo\libemu_buttondemo.so
```

Result:

```text
found no threats
```

## Install notes

Copy built module to the SD/image:

```text
cubegm\cores\libemu_buttondemo.so
```

Add to `cubegm\cores\config.xml`:

```xml
<core>
<emucore name="Button Demo" file="libemu_buttondemo.so" />
<supported_extensions>DEMO</supported_extensions>
</core>
```

Create a dummy file:

```text
DEMO\button.demo
```

If the launcher does not show new platforms from arbitrary root folders, reuse an existing platform folder for testing and add a `filelist.xml` override for one dummy file.
