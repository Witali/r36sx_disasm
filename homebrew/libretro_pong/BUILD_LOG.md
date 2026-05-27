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
