# Most Similar Repository Found

## Best Match: `goph-R/SF3000-RE`

- URL: https://github.com/goph-R/SF3000-RE
- Local copy: `internet_sources\SF3000-RE`
- Type: reverse-engineering notes / practical notes for running custom code, not `rkgame` source code.

Why this is the closest repository to our `disk_image`:

- Same platform: MIPS32 little-endian / MIPS32r2.
- Same userspace: Buildroot `2021.05-rc2`.
- Same SD/rootfs layout: `rootfs/` + `cubegm/`.
- Same game environment startup through `/mnt/sdcard/cubegm/icube`.
- Same emulator core model: MIPS32 `.so` files in `cubegm/cores/`, with `cores/config.xml` and `cores/filelist.xml`.
- Similar graphics stack in principle: stock system without SDL/EGL/GLES, with DirectFB in `cubegm`.
- The repository describes a working method for running a custom MIPS32 static binary by replacing `cubegm/icube`.

Our local confirmations:

- `disk_image\rootfs\etc\os-release`: `PRETTY_NAME="Buildroot 2021.05-rc2"`.
- `disk_image\cubegm\rkgame`: 1,178,732 bytes.
- `disk_image\cubegm\icube`: 11,976 bytes.
- `disk_image\cubegm\driver.so`: 98,216 bytes.

Conclusion:

`goph-R/SF3000-RE` currently looks like the most similar public reverse-engineering base for this branch of SF3000 / GB350 / R36SX-like MIPS + iCube/cubegm consoles. It is the best starting point for standalone homebrew and custom launcher work.

## Second Place: `LiamJ74/R36S-V2.6_Wiki`

- URL: https://github.com/LiamJ74/R36S-V2.6_Wiki
- Local copy: `internet_sources\R36S-V2.6_Wiki`
- Type: wiki/tools for SD card structure, ROM lists, covers, and UI resources.

Why it matters:

- It directly describes `iCube/cubegm`.
- It gives the same chain: `icube_start.sh` kills `hcprojector`, starts `cubegm/icube`, then `icube` starts `cubegm/rkgame`.
- It lists key files: `rkgame`, `icube`, `driver.so`, `UI_Res.cpd`, `resource.cpd`, `setting.xml`, `allfiles.lst`, `cores/config.xml`.
- It is useful for SD card and resource modding, but not for compiling custom firmware.

## Close Firmware Backup / Toolchain: `game-de-it/testrepo` + `game-de-it/sf3000`

- Stock OS release: https://github.com/game-de-it/testrepo/releases/tag/SF3000_StockOS
- Toolchain repo: https://github.com/game-de-it/sf3000
- Local copies:
  - `internet_sources\sf3000_stockos_repo`
  - `internet_sources\sf3000_toolchain`

Why it matters:

- The `SF3000_StockOS` release is described as internal SF3000 SD card files without ROM files.
- `game-de-it/sf3000` is almost empty in the git tree, but GitHub has a release named `sf3000_toolchain_v0.1`.

## Not Found

Public source code for `cubegm/rkgame`, `icube`, or `driver.so` has not been found so far. The most likely strategy is not to decompile everything back into exact original source code, but to:

1. use `goph-R/SF3000-RE` as the practical path for running our own MIPS32 code;
2. use the HCRTOS/Hichip SDK to understand the HC16xx/rootfs/kernel/userspace platform;
3. keep the stock kernel/rootfs/driver.so, while replacing or bypassing `icube`/`rkgame` with our own frontend;
4. for full firmware replacement, first build a minimal MIPS32 static userspace launcher and test output through `/dev/fb0` or DirectFB.
