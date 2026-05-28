# Build log: unified Pong

## 2026-05-28 remove old Pong target folders

Purpose:

Remove the old `homebrew\libretro_pong` and `homebrew\native_pong` directories
now that both targets build from the single source file:

```text
homebrew\pong\pong.c
```

Current target selection:

```text
R36SX_PONG_TARGET=1  libretro core for rkgame
R36SX_PONG_TARGET=2  native Tiny MC executable using driver.so
```

Current build scripts:

```text
homebrew\pong\build_libretro_pong.ps1
homebrew\pong\build_native_pong.ps1
```

Current outputs:

```text
homebrew\pong\libemu_pong.so
homebrew\pong\pong
```

Commands run:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\pong\build_libretro_pong.ps1
powershell -ExecutionPolicy Bypass -File .\homebrew\pong\build_native_pong.ps1
.\tools\scan-download.ps1 .\homebrew\pong\libemu_pong.so
.\tools\scan-download.ps1 .\homebrew\pong\pong
git rm -r homebrew/libretro_pong homebrew/native_pong
```

Outputs:

```text
homebrew\pong\libemu_pong.so
  size: 39500 bytes
  sha256: C56340485602A7AF5538095EA48C1DB5C078EA088006DC41A21D3CCE37C1A69E

homebrew\pong\pong
  size: 19188 bytes
  sha256: A84DA563B8DE044F0CB686CC11FE7983985B232733AC9A72BA80A15872E1B2DF
```

Scan results:

```text
Defender scan homebrew\pong\libemu_pong.so: found no threats
Defender scan homebrew\pong\pong: found no threats
```
