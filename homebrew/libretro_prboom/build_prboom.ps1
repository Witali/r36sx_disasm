param(
    [string]$SourceDir = ".\internet_sources\libretro-prboom",
    [string]$OutFile = ".\homebrew\libretro_prboom\prboom_libretro.so",
    [string]$ObjDir = ".\homebrew\libretro_prboom\obj",
    [string]$Zig = ".\tools\zig-x86_64-windows-0.16.0\zig.exe",
    [string]$Sysroot = ".\tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
)

$ErrorActionPreference = "Stop"

$repo = (Resolve-Path ".").Path
$root = (Resolve-Path $SourceDir).Path
$out = Join-Path $repo $OutFile
$objdirPath = Join-Path $repo $ObjDir
$cc = (Resolve-Path $Zig).Path
$sysrootPath = (Resolve-Path $Sysroot).Path

$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path ".\tools\zig-global-cache").Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path ".\tools\zig-cache").Path

New-Item -ItemType Directory -Force $objdirPath | Out-Null

$mk = Get-Content -Raw (Join-Path $root "Makefile.common")
$mk = $mk -replace "\\\r?\n", " "
$fluidIdx = $mk.IndexOf("ifeq (`$(WANT_FLUIDSYNTH), 1)")
if ($fluidIdx -gt 0) {
    $mk = $mk.Substring(0, $fluidIdx)
}

$srcs = @()
foreach ($m in [regex]::Matches($mk, "\$\((LIBRETRO_DIR|LIBRETRO_COMM_DIR|DEPS_DIR|CORE_DIR)\)/[^\s]+?\.c")) {
    $p = $m.Value.
        Replace("`$(LIBRETRO_DIR)", "libretro").
        Replace("`$(LIBRETRO_COMM_DIR)", "libretro/libretro-common").
        Replace("`$(DEPS_DIR)", "deps").
        Replace("`$(CORE_DIR)", "src")
    $srcs += (Join-Path $root ($p -replace "/", "\"))
}
$srcs = $srcs | Select-Object -Unique

$cflags = @(
    "cc",
    # Compile with mipsel-linux-gnu so Zig does not stamp newer GLIBC symbol
    # versions into object references. Link with gnueabihf below for the
    # hard-float MIPS runtime.
    "-target", "mipsel-linux-gnu",
    "--sysroot", $sysrootPath,
    "-isystem", (Join-Path $sysrootPath "usr\include"),
    "-march=mips32r2",
    "-O2",
    "-DNDEBUG",
    "-DFPM_DEFAULT",
    "-DINLINE=inline",
    "-DHAVE_LIBMAD",
    "-DMUSIC_SUPPORT",
    "-D_POSIX_C_SOURCE=199309L",
    "-std=c99",
    "-ffast-math",
    "-fomit-frame-pointer",
    "-fPIC",
    "-Wall",
    "-W",
    "-Wno-unused-parameter",
    "-I$root",
    "-I$(Join-Path $root "src")",
    "-I$(Join-Path $root "deps\libmad")",
    "-I$(Join-Path $root "libretro\libretro-common\include")",
    "-include", "z_zone.h"
)

$objects = @()
Write-Host "Compiling $($srcs.Count) C files"
foreach ($src in $srcs) {
    $rel = [IO.Path]::GetRelativePath($root, $src)
    $objName = ($rel -replace "[\\/]", "_") + ".o"
    $obj = Join-Path $objdirPath $objName
    Write-Host "  $rel"
    & $cc @cflags -c $src -o $obj
    if ($LASTEXITCODE -ne 0) {
        throw "compile failed: $rel"
    }
    $objects += $obj
}

Set-Content -Encoding ASCII -Path (Join-Path $objdirPath "objects.txt") -Value $objects

Push-Location $root
try {
    & $cc cc `
        -target mipsel-linux-gnueabihf `
        --sysroot $sysrootPath `
        -nostdlib `
        -march=mips32r2 `
        -shared `
        -fPIC `
        "-Wl,--version-script=libretro/link.T" `
        "-Wl,--no-undefined" `
        "-Wl,--as-needed" `
        "-Wl,-s" `
        -o $out `
        @objects `
        -L/lib `
        -L/usr/lib `
        -lm `
        -lc `
        -lgcc_s
    if ($LASTEXITCODE -ne 0) {
        throw "link failed"
    }
}
finally {
    Pop-Location
}

Get-Item $out | Select-Object FullName, Length, LastWriteTime
Get-FileHash $out -Algorithm SHA256
