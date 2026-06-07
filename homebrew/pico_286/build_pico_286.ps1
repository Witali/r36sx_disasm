param(
    [switch]$DebugLog,
    [switch]$DisableProfiling,
    [switch]$DisableComputedGoto,
    [switch]$DisableFastMemory,
    [switch]$DisableProtectedMode,
    [switch]$DisableProtectedModeDebug,
    [switch]$RedirectorTrace,
    [switch]$TryStrip
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$PicoRoot = Join-Path $PSScriptRoot "pico-286"
$PortRoot = Join-Path $PSScriptRoot "r36sx_port"
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$ZigInclude = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\lib\include"
$Sysroot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
$SysrootInclude = Join-Path $Sysroot "usr\include"
$SysrootFreetypeInclude = Join-Path $SysrootInclude "freetype2"
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$ToolchainRoot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain"
$CppInclude = Join-Path $ToolchainRoot "mips-mti-linux-gnu\include\c++\6.3.0"
$CppTargetInclude = Join-Path $CppInclude "mips-mti-linux-gnu"
$GccLib = Join-Path $ToolchainRoot "lib\gcc\mips-mti-linux-gnu\6.3.0\mipsel-r2-hard\lib"
$TargetZlib = Join-Path $SysrootUsrLib "libz.so.1.2.11"
$ScreenshotSoOut = Join-Path $Root "homebrew\common\screenshot.so"
$ScreenshotAOut = Join-Path $Root "homebrew\common\screenshot.a"
$Out = Join-Path $PSScriptRoot "pico_286"
$ObjDir = Join-Path $PSScriptRoot "obj"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"
$CrtBegin = Join-Path $GccLib "crtbegin.o"
$CrtEnd = Join-Path $GccLib "crtend.o"
$CompatHeader = Join-Path $PSScriptRoot "r36sx_pico286_compat.h"
$DebugValue = if ($DebugLog -or $RedirectorTrace) { "1" } else { "0" }
$ProfilingValue = if ($DisableProfiling) { "0" } else { "1" }
$ComputedGotoValue = if ($DisableComputedGoto) { "0" } else { "1" }
$FastMemoryValue = if ($DisableFastMemory) { "0" } else { "1" }
$ProtectedModeValue = if ($DisableProtectedMode) { "0" } else { "1" }
$ProtectedModeDebugValue = if ($DisableProtectedModeDebug) { "0" } else { "1" }
$RedirectorTraceValue = if ($RedirectorTrace) { "1" } else { "0" }
$RootPath = $Root.Path

function ConvertTo-CMacroString {
    param([string]$Value)
    return (($Value -replace "\\", "\\") -replace '"', '\"')
}

function Get-GitText {
    param([string[]]$Arguments)
    $Output = & git @Arguments 2>$null
    if ($LASTEXITCODE -ne 0) {
        return $null
    }
    return (($Output -join "`n").TrimEnd("`r", "`n"))
}

$BuildGitCommit = "unknown"
$BuildGitCommitShort = "unknown"
$BuildCommitObjectSha256 = "unknown"
$BuildGitDirty = "1"

$InsideWorkTree = Get-GitText -Arguments @("-C", $RootPath, "rev-parse", "--is-inside-work-tree")
if ($InsideWorkTree -eq "true") {
    $BuildGitCommit = Get-GitText -Arguments @("-C", $RootPath, "rev-parse", "HEAD")
    $BuildGitCommitShort = Get-GitText -Arguments @("-C", $RootPath, "rev-parse", "--short=12", "HEAD")

    $CommitObjectLines = & git -C $RootPath cat-file commit HEAD 2>$null
    if ($LASTEXITCODE -eq 0) {
        $CommitObjectText = ($CommitObjectLines -join "`n") + "`n"
        $Sha256 = [System.Security.Cryptography.SHA256]::Create()
        $HashBytes = $Sha256.ComputeHash([System.Text.Encoding]::UTF8.GetBytes($CommitObjectText))
        $BuildCommitObjectSha256 = -join ($HashBytes | ForEach-Object { $_.ToString("x2") })
    }

    $BuildGitDirty = "0"
    & git -C $RootPath diff --quiet --ignore-submodules -- . 2>$null
    if ($LASTEXITCODE -ne 0) {
        $BuildGitDirty = "1"
    }
    & git -C $RootPath diff --cached --quiet --ignore-submodules -- . 2>$null
    if ($LASTEXITCODE -ne 0) {
        $BuildGitDirty = "1"
    }
}

if (!(Test-Path $PicoRoot)) {
    throw "Missing homebrew\pico_286\pico-286 source tree."
}
if (!(Test-Path $PortRoot)) {
    throw "Missing homebrew\pico_286\r36sx_port source tree."
}
if (!(Test-Path $TargetZlib)) {
    throw "Missing target zlib library: $TargetZlib"
}

$ObjDirFull = [IO.Path]::GetFullPath($ObjDir)
$ScriptRootFull = [IO.Path]::GetFullPath($PSScriptRoot)
$ExpectedObjDir = [IO.Path]::GetFullPath((Join-Path $ScriptRootFull "obj"))
if ($ObjDirFull -ne $ExpectedObjDir) {
    throw "Refusing to clean unexpected object directory: $ObjDirFull"
}
if (Test-Path -LiteralPath $ObjDirFull) {
    Remove-Item -LiteralPath $ObjDirFull -Recurse -Force
}
New-Item -ItemType Directory -Force $ObjDir | Out-Null
$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

$IncludeArgs = @(
    "-I$PortRoot",
    "-I$PSScriptRoot",
    "-I$(Join-Path $Root "homebrew\common")",
    "-I$(Join-Path $PicoRoot "src")",
    "-I$(Join-Path $PicoRoot "src\emulator")",
    "-I$(Join-Path $PicoRoot "src\emu8950")",
    "-I$(Join-Path $PicoRoot "src\printf")",
    "-I$(Join-Path $PicoRoot "findfirst")",
    "-isystem$ZigInclude",
    "-isystem$SysrootInclude",
    "-isystem$SysrootFreetypeInclude"
)
$CppIncludeArgs = $IncludeArgs + @(
    "-isystem$CppInclude",
    "-isystem$CppTargetInclude"
)
$CommonArgs = @(
    "-target", "mipsel-linux-gnu",
    "-march=mips32r2",
    "--sysroot=$Sysroot",
    "-DPICO_RP2040=0",
    "-DPICO_RP2350=0",
    "-DDEBUG=$DebugValue",
    ("-DR36SX_BUILD_GIT_COMMIT=""{0}""" -f (ConvertTo-CMacroString $BuildGitCommit)),
    ("-DR36SX_BUILD_GIT_COMMIT_SHORT=""{0}""" -f (ConvertTo-CMacroString $BuildGitCommitShort)),
    ("-DR36SX_BUILD_COMMIT_OBJECT_SHA256=""{0}""" -f (ConvertTo-CMacroString $BuildCommitObjectSha256)),
    "-DR36SX_BUILD_GIT_DIRTY=$BuildGitDirty",
    "-DR36SX_ENABLE_PROFILING=$ProfilingValue",
    "-DR36SX_CPU_COMPUTED_GOTO=$ComputedGotoValue",
    "-DR36SX_NATIVE_FAST_MEMORY=$FastMemoryValue",
    "-DR36SX_ENABLE_PROTECTED_MODE=$ProtectedModeValue",
    "-DR36SX_DEBUG_386_PROTECTED_MODE=$ProtectedModeDebugValue",
    "-DR36SX_DEBUG_REDIRECTOR_TRACE=$RedirectorTraceValue",
    "-DR36SX_SEGMENT_BASE_CACHE=1",
    "-DCPU_386_EXTENDED_OPS=1",
    "-DR36SX_RUNTIME_SOUND_FREQUENCY=1",
    "-DR36SX_VIDEO_DIRTY_TRACKING=1",
    "-DINI_HANDLER_LINENO=1",
    "-DINI_MAX_LINE=512",
    "-DINI_ALLOW_MULTILINE=0",
    "-DUSE_EMU8950_OPL",
    "-DEMU8950_SLOT_RENDER=1",
    "-DEMU8950_ASM=0",
    "-DEMU8950_NO_RATECONV=1",
    "-DEMU8950_NO_WAVE_TABLE_MAP=1",
    "-DEMU8950_NO_TLL=1",
    "-DEMU8950_NO_FLOAT=1",
    "-DEMU8950_NO_TIMER=1",
    "-DEMU8950_NO_TEST_FLAG=1",
    "-DEMU8950_SIMPLER_NOISE=1",
    "-DEMU8950_SHORT_NOISE_UPDATE_CHECK=1",
    "-DEMU8950_LINEAR_SKIP=1",
    "-DEMU8950_LINEAR_END_OF_NOTE_OPTIMIZATION",
    "-DEMU8950_NO_PERCUSSION_MODE=1",
    "-DEMU8950_LINEAR=1",
    "-include",
    $CompatHeader,
    "-O2",
    "-fPIC",
    "-fms-extensions",
    "-fno-sanitize=undefined",
    "-fno-strict-aliasing",
    "-fno-builtin-memset",
    "-fno-builtin-memcpy",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-missing-field-initializers"
)

$Objects = New-Object System.Collections.Generic.List[string]

function Add-Object {
    param([string]$Path)
    $Objects.Add($Path) | Out-Null
}

function Invoke-Checked {
    param([scriptblock]$Command)
    & $Command
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code $LASTEXITCODE"
    }
}

function Compile-C {
    param([string]$Source)
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig cc @CommonArgs @IncludeArgs "-std=c17" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

function Compile-Cpp {
    param([string]$Source)
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig c++ @CommonArgs @CppIncludeArgs "-std=c++20" "-fpermissive" "-fno-exceptions" "-fno-rtti" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

$CFiles = @()
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emulator") -Recurse -File -Filter "*.c" |
    Where-Object { $_.Name -ne "cpu.c" -and $_.Name -ne "ports.c" }
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emu8950") -File -Filter "*.c"
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "findfirst") -File -Filter "*.c"
$CFiles += Get-Item (Join-Path $PicoRoot "src\printf\printf.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\r36sx_screen_keyboard.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\r36sx_screenshot.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\inih\ini.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_disk_menu.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_key_presets.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_post_overlay.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_minifb.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_linux_audio.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_app_stats.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_bios_rom.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_host_disk_io.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_disk_config.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_profile.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu_dispatch.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu_8086.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu_80286.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu_80386.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_ports.c")

foreach ($File in $CFiles) {
    Compile-C $File.FullName
}

Compile-Cpp (Join-Path $PicoRoot "src\emu8950\slot_render.cpp")
Compile-Cpp (Join-Path $PortRoot "r36sx_linux-main.cpp")

Invoke-Checked { & $Zig ld.lld `
    "--sysroot=$Sysroot" `
    -EL `
    -m `
    elf32ltsmip `
    "--dynamic-linker=/lib/ld.so.1" `
    "--hash-style=sysv" `
    "-L$SysrootLib" `
    "-L$SysrootUsrLib" `
    "-L$GccLib" `
    -o $Out `
    $Crt1 `
    $Crti `
    $CrtBegin `
    @Objects `
    "-lpthread" `
    "-ldl" `
    "-lm" `
    "-lstdc++" `
    "-lgcc_s" `
    "-lc" `
    $CrtEnd `
    $Crtn }

if ($TryStrip) {
    $StrippedOut = Join-Path $ObjDir "pico_286.stripped"
    & $Zig objcopy "--strip-all" $Out $StrippedOut
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force $StrippedOut $Out
    } else {
        Write-Warning "zig objcopy --strip-all failed; keeping unstripped pico_286"
        if (Test-Path $StrippedOut) {
            Remove-Item -Force $StrippedOut
        }
        $global:LASTEXITCODE = 0
    }
}

Invoke-Checked { & $Zig cc `
    "-target" `
    "mipsel-linux-gnu" `
    "-march=mips32r2" `
    "--sysroot=$Sysroot" `
    "-shared" `
    "-fPIC" `
    "-O2" `
    "-I$(Join-Path $Root "homebrew\common")" `
    "-isystem$SysrootInclude" `
    "-Wl,--hash-style=sysv" `
    "-Wl,-soname,screenshot.so" `
    (Join-Path $Root "homebrew\common\r36sx_screenshot_module.c") `
    $TargetZlib `
    "-o" `
    $ScreenshotSoOut }

$ScreenshotObj = Join-Path $ObjDir "r36sx_screenshot_module-static.o"
Invoke-Checked { & $Zig cc `
    "-target" `
    "mipsel-linux-gnu" `
    "-march=mips32r2" `
    "--sysroot=$Sysroot" `
    "-O2" `
    "-fno-pic" `
    "-I$(Join-Path $Root "homebrew\common")" `
    "-isystem$SysrootInclude" `
    "-c" `
    (Join-Path $Root "homebrew\common\r36sx_screenshot_module.c") `
    "-o" `
    $ScreenshotObj }
if (Test-Path -LiteralPath $ScreenshotAOut) {
    Remove-Item -LiteralPath $ScreenshotAOut -Force
}
Invoke-Checked { & $Zig ar rcs $ScreenshotAOut $ScreenshotObj }

Write-Host "Built $Out"
Write-Host "Built $ScreenshotSoOut"
Write-Host "Built $ScreenshotAOut"
