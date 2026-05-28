# Tiny MC build commands

Run from the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\homebrew\tiny_mc\build_tiny_mc.ps1
```

The script uses the local Zig/Clang frontend and the downloaded Buildroot SDK
sysroot:

```text
tools\zig-x86_64-windows-0.16.0\zig.exe
tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot
```

The build is split into two steps:

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe clang `
    --target=mipsel-linux-gnu `
    -march=mips32r2 `
    -nostdlib `
    -nostdinc `
    -isystem<zig-lib-include> `
    -isystem<buildroot-sysroot>\usr\include `
    -O2 `
    -fno-pic `
    -fno-sanitize=undefined `
    -Wall `
    -Wextra `
    -std=c99 `
    -c `
    -o homebrew\tiny_mc\tiny_mc.o `
    homebrew\tiny_mc\tiny_mc.c
```

```powershell
.\tools\zig-x86_64-windows-0.16.0\zig.exe ld.lld `
    --sysroot=<buildroot-sysroot> `
    -EL `
    -m elf32ltsmip `
    --dynamic-linker=/lib/ld.so.1 `
    --hash-style=sysv `
    -L<buildroot-sysroot>\lib `
    -L<buildroot-sysroot>\usr\lib `
    -o homebrew\tiny_mc\tiny_mc `
    <buildroot-sysroot>\usr\lib\crt1.o `
    <buildroot-sysroot>\usr\lib\crti.o `
    homebrew\tiny_mc\tiny_mc.o `
    -ldl `
    -lc `
    <buildroot-sysroot>\usr\lib\crtn.o
```

`-ldl` is required because the current build uses `dlopen()`/`dlsym()` to load
the vendor `cubegm/driver.so` display path, matching stock `rkgame`.

The explicit SDK sysroot link is important. A simpler Zig glibc build produced
a binary requiring `GLIBC_2.34`, while the device rootfs libc only exposes
symbols up to `GLIBC_2.19`.

After building, scan the generated executable:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\homebrew\tiny_mc\tiny_mc
```

For the patch copy, scan the patch binary too:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\scan-download.ps1 .\disk_image_patch_022\cubegm\rkgame
```
