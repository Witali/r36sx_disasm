param(
    [switch]$DebugLog,
    [switch]$DisableProfiling,
    [switch]$DisableComputedGoto,
    [switch]$DisableFastMemory,
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
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$ToolchainRoot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain"
$CppInclude = Join-Path $ToolchainRoot "mips-mti-linux-gnu\include\c++\6.3.0"
$CppTargetInclude = Join-Path $CppInclude "mips-mti-linux-gnu"
$GccLib = Join-Path $ToolchainRoot "lib\gcc\mips-mti-linux-gnu\6.3.0\mipsel-r2-hard\lib"
$TargetZlib = Join-Path $SysrootUsrLib "libz.so.1.2.11"
$Out = Join-Path $PSScriptRoot "pico_286"
$ObjDir = Join-Path $PSScriptRoot "obj"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"
$CrtBegin = Join-Path $GccLib "crtbegin.o"
$CrtEnd = Join-Path $GccLib "crtend.o"
$CompatHeader = Join-Path $PSScriptRoot "r36sx_pico286_compat.h"
$DebugValue = if ($DebugLog) { "1" } else { "0" }
$ProfilingValue = if ($DisableProfiling) { "0" } else { "1" }
$ComputedGotoValue = if ($DisableComputedGoto) { "0" } else { "1" }
$FastMemoryValue = if ($DisableFastMemory) { "0" } else { "1" }

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
    "-isystem$SysrootInclude"
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
    "-DR36SX_ENABLE_PROFILING=$ProfilingValue",
    "-DR36SX_CPU_COMPUTED_GOTO=$ComputedGotoValue",
    "-DR36SX_NATIVE_FAST_MEMORY=$FastMemoryValue",
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
$CFiles += Get-Item (Join-Path $Root "homebrew\common\inih\ini.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_disk_menu.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_key_presets.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_minifb.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_linux_audio.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_app_stats.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_bios_rom.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_host_disk_io.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_disk_config.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_profile.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_cpu.c")
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
    $TargetZlib `
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

Write-Host "Built $Out"
