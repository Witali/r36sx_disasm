# disk_image_patch_011

Experimental ZX Spectrum 48K emulator patch using `libretro/fuse-libretro`.

## Changed/added files

- `cubegm/cores/libemu_fuse.so`
- `cubegm/cores/libemu_fuse.so.cfg`
- `cubegm/cores/config.xml`
- `cubegm/cores/filelist.xml`
- `ATARI/filelist.csv`
- `ATARI/R36SX_ZX48.tap`

## What this patch does

- Adds a new libretro core entry named `Fuse ZX Spectrum 48K`.
- Adds file extensions: `TAP`, `TZX`, `Z80`, `RZX`, `SCL`, `TRD`, `DSK`, `SNA`, `SZX`.
- Installs `libemu_fuse.so`, built from `libretro/fuse-libretro`.
- Adds `libemu_fuse.so.cfg`, pinning the core to `Spectrum 48K`.
- Adds a tiny generated test tape `R36SX_ZX48.tap`.
- Adds the test tape to the existing `ATARI` menu as a temporary launch point.
- Adds a `cores/filelist.xml` override so `ATARI/R36SX_ZX48.tap` launches with `libemu_fuse.so` instead of the Atari core.

## Source and build

- Source repo: `https://github.com/libretro/fuse-libretro.git`
- Source commit: `bce196f`
- Local source path: `internet_sources/fuse-libretro`
- Build output path: `homebrew/fuse_zx48/libemu_fuse.so`

Build notes are documented in:

```text
homebrew/fuse_zx48/BUILD_LOG.md
```

## Verification

- `libemu_fuse.so` is ELF32 little-endian MIPS.
- ELF flags: `0x70001007`.
- Exported symbols include `retro_get_system_av_info`, `retro_load_game`, `retro_run`, `retro_set_environment`, `retro_set_video_refresh`, `check_encrypty`, and `CheckEncrypty`.
- Microsoft Defender scan: no threats found.
- `libemu_fuse.so` SHA256: `BEBCFF5B456A10C6888ED20F5AACE173CC60B8668D037F98E2FD9411EAFB62A5`
- `R36SX_ZX48.tap` SHA256: `A5E66AFAA7B619D1784B3499A45E3153AB614BB9B7464B260B05D38C3D10C15C`

## Risk notes

This is an experimental first build.

The core is linked with `-nostdlib`, so it has no `NEEDED` entries and expects libc/libm symbols to resolve from the already-running `rkgame` process. If the device returns to the launcher immediately, the next step is to build a variant with explicit `NEEDED` dependencies against the MIPS sysroot.

The temporary menu location is `ATARI`, not a real top-level ZX Spectrum platform. This avoids patching the hardcoded main menu while testing whether the core runs.
