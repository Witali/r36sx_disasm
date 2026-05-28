# Unified Pong Source

`pong.c` is the single source file for both Pong builds:

- `R36SX_PONG_TARGET=1`: libretro core for `rkgame`.
- `R36SX_PONG_TARGET=2`: native Tiny MC executable using `driver.so`.

The shared section owns the game state, scoring, button semantics, RGB565
framebuffer drawing, and audio-event decisions. Target sections only translate
input, present video, and route audio to the correct platform API.

Native Pong follows the native-app exit rule: hold `Select + Start` together to
leave the program. `Fn` is still accepted as a secondary hardware escape.

## Outputs

Current build outputs live beside the shared source:

```text
homebrew\pong\libemu_pong.so
homebrew\pong\pong
```

`homebrew\libretro_pong` and `homebrew\native_pong` were removed after the two
targets were unified into this directory.

## Build libretro core

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pong\build_libretro_pong.ps1
```

Install output to:

```text
cubegm\cores\libemu_pong.so
```

## Build native executable

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pong\build_native_pong.ps1
```

Install output to:

```text
MIPS_NATIVE\pong\pong
```
