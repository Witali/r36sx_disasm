$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$ZigInclude = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\lib\include"
$Out = Join-Path $PSScriptRoot "pong"
$Obj = Join-Path $PSScriptRoot "native_pong.o"
$Src = Join-Path $PSScriptRoot "native_pong.c"
$Sysroot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
$SysrootInclude = Join-Path $Sysroot "usr\include"
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"

$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

& $Zig clang `
    --target=mipsel-linux-gnu `
    -march=mips32r2 `
    -nostdlib `
    -nostdinc `
    "-isystem$ZigInclude" `
    "-isystem$SysrootInclude" `
    -O2 `
    -fno-pic `
    -fno-sanitize=undefined `
    -Wall `
    -Wextra `
    -std=c99 `
    -c `
    -o $Obj `
    $Src

& $Zig ld.lld `
    "--sysroot=$Sysroot" `
    -EL `
    -m `
    elf32ltsmip `
    "--dynamic-linker=/lib/ld.so.1" `
    "--hash-style=sysv" `
    "-L$SysrootLib" `
    "-L$SysrootUsrLib" `
    -o $Out `
    $Crt1 `
    $Crti `
    $Obj `
    "-ldl" `
    "-lc" `
    $Crtn
