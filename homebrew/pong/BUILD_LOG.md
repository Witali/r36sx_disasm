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

## 2026-05-28 preserve mixer volume rebuild

Purpose:

Avoid audible volume jumps when native Pong initializes audio through
`driver.so`. The vendor `sound_driver_init()` path also writes the saved AV
volume to `/dev/sndC0i2so`, so native Pong now preserves and restores the
currently applied mixer volume around audio initialization.

Implementation:

- Reused `r36sx_driver_audio_bind_preserve_volume()` from
  `homebrew/common/driver_audio.h`.
- Native Pong passes its resolved `cube_ioctl` symbol to the audio helper.

Build command from repository root:

```powershell
.\homebrew\pong\build_native_pong.ps1
```

Patch directory:

```text
patches\disk_image_patch_056
```

Verification:

```text
Size: 19284 bytes
SHA256: D011576B1235901DF9E0BB844D3688967372CA77B923CA4235FC12F9DFE05642
Contains strings: sound_driver_init, sound_driver_playframe
Defender scan homebrew\pong\pong: found no threats
Defender scan disk_image\MIPS_NATIVE\pong\pong: found no threats
Defender scan patches\disk_image_patch_056: found no threats
```
