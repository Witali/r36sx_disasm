# Build log: libemu_pong.so

Date: 2026-05-27

Workspace:

```text
C:\Work\r36sx_disasm
```

Goal:

Build a tiny Pong-style game module using the same integration approach as
`homebrew\libretro_button_demo`.

Source files:

```text
homebrew\libretro_pong\pong.c
homebrew\libretro_pong\Makefile
homebrew\libretro_pong\README.md
homebrew\libretro_pong\BUILD_LOG.md
```

Build command:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Notes:

- `-nostdlib` keeps the module independent from the device libc.
- `-fno-builtin` prevents compiler-generated libc calls such as `memset`.
- The game returns from `retro_run()` every frame after polling input, updating
  the game state, submitting video, and emitting any active sound.

Verification and patch details are appended after successful builds.

## 2026-05-27 initial Pong build

Implemented:

- Pong-style playfield with two paddles, center line, ball, and score digits.
- D-pad controls the left paddle.
- A/X and B/Y can manually control the right paddle.
- If the right paddle is not being controlled, a small AI follows the ball.
- Start toggles pause/resume.
- Select resets the match.
- Paddle hits, wall bounces, scores, pause, and reset play generated tones.

Verification:

```text
ELF32 little-endian DYN(shared object), machine=MIPS, flags=0x70001007
Size: 41496 bytes
SHA256: B4DFD05CAD16A365BBF062CBB124BC73BAC67CC0BCE1130C33C575D15A989EE0
No __ubsan/GLIBC/libc/ld-linux/memset dependency strings found.
Zig objdump search found no UND/undefined/NEEDED/memset/__ubsan/GLIBC lines.
Defender: found no threats.
```

Installed copy:

```text
disk_image\cubegm\cores\libemu_pong.so
```

Patch directory:

```text
disk_image_patch_016
```

## 2026-05-27 `.gb` launch compatibility rebuild

Reason:

The first device test of `disk_image_patch_016` briefly showed a black screen
and returned to the launcher. This matches an earlier Button Demo failure mode
where a custom extension appeared in the GB list but was not a reliable launch
route. Button Demo became reliable after adding a dummy file with a stock
`.gb` extension plus a `cores/filelist.xml` override.

Source change:

- `retro_get_system_info()` now reports `valid_extensions = "gb|pong"`.

Build command from repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Integration changes:

- Added `GB\pong.gb`.
- Added `<file name="GB/pong.gb" core="libemu_pong.so" />` to
  `cubegm\cores\filelist.xml`.
- Added `pong.gb,Pong Demo GB,Pong Demo GB` to `GB\filelist.csv`.
- Kept `GB\pong.pong` as a secondary diagnostic entry.

Verification:

```text
Size: 41512 bytes
SHA256: CDA9C881BA3A40AA3984B8B4274508022F64C743341C690EEAA61F2BCE63AC55
Required symbol strings include retro_run, retro_load_game,
retro_set_video_refresh, retro_set_audio_sample,
retro_set_audio_sample_batch, check_encrypty, CheckEncrypty.
No NEEDED/UND/__ubsan/GLIBC/ld-linux/libc.so/libm.so/memset strings found.
Defender scan of homebrew SO: found no threats.
Defender scan of disk_image_patch_017: found no threats.
```

Patch directory:

```text
disk_image_patch_017
```

## 2026-05-28 shared hardware header rebuild

Reason:

Move shared R36SX/SF3000-like hardware constants into
`homebrew/common/hardware.h`.

Source change:

- `pong.c` now includes `../common/hardware.h`.
- The 640x480 framebuffer size and 44.1 kHz / 60 Hz audio timing now come from
  the shared header.

Build command from repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Verification:

```text
ELF32 little-endian DYN(shared object), machine=MIPS.
Size: 41512 bytes
SHA256: A769CA8D5710D4863D0348E7ECC8F41C9CD28CC3F14505F4E934014AFC782EA4
No NEEDED/UND/__ubsan/GLIBC/ld-linux/libc.so/libm.so/memset strings found.
Defender scan homebrew\libretro_pong\libemu_pong.so: found no threats
Defender scan disk_image_patch_027\cubegm\cores\libemu_pong.so: found no threats
```

## 2026-05-28 libretro hardware header split

Reason:

Split the shared hardware constants into a native/common layer and a
libretro-specific layer.

Source change:

- Moved libretro-only audio/video cadence constants out of
  `homebrew/common/hardware.h`.
- Added `homebrew/common/libretro_hardware.h`, which includes `hardware.h`.
- `pong.c` now includes `../common/libretro_hardware.h`.
- Audio timing now uses the `R36SX_LIBRETRO_*` macro names.

Build command from repository root:

```powershell
$env:ZIG_GLOBAL_CACHE_DIR=(Resolve-Path .\tools\zig-global-cache).Path
$env:ZIG_LOCAL_CACHE_DIR=(Resolve-Path .\tools\zig-cache).Path
.\tools\zig-x86_64-windows-0.16.0\zig.exe cc -target mipsel-linux-gnu -march=mips32r2 -O2 -fno-sanitize=undefined -fno-builtin -fPIC -Wall -Wextra -std=c99 -shared -nostdlib '-Wl,-soname,libemu_pong.so' -o .\homebrew\libretro_pong\libemu_pong.so .\homebrew\libretro_pong\pong.c
```

Verification:

```text
Size: 41512 bytes
SHA256: A769CA8D5710D4863D0348E7ECC8F41C9CD28CC3F14505F4E934014AFC782EA4
Binary contents remained identical to the previous shared-header rebuild.
Defender scan homebrew\libretro_pong\libemu_pong.so: found no threats
```
