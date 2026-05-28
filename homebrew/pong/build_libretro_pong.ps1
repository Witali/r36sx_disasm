$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$Out = Join-Path $PSScriptRoot "libemu_pong.so"
$Src = Join-Path $PSScriptRoot "pong.c"

$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

& $Zig cc `
    -target mipsel-linux-gnu `
    -march=mips32r2 `
    -O2 `
    -fno-sanitize=undefined `
    -fno-builtin `
    -fPIC `
    -Wall `
    -Wextra `
    -std=c99 `
    -DR36SX_PONG_TARGET=1 `
    -shared `
    -nostdlib `
    '-Wl,-soname,libemu_pong.so' `
    -o $Out `
    $Src
