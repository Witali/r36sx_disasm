param(
    [switch]$DebugLog,
    [switch]$DisableProfiling,
    [switch]$DisableComputedGoto,
    [switch]$DisableFastMemory,
    [switch]$DisableProtectedMode,
    [ValidateSet("O0", "O1", "O2", "O3", "Os", "Og")]
    [string]$OptLevel = "O2",
    [string]$Out,
    [string]$PatchDir,
    [switch]$NoPatchCopy
)

$ErrorActionPreference = "Stop"
if ($PSVersionTable.PSVersion.Major -ge 7) {
    $PSNativeCommandUseErrorActionPreference = $false
}

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$PicoRoot = Join-Path $PSScriptRoot "pico-286"
$PortRoot = Join-Path $PSScriptRoot "r36sx_port"
$WindowsRoot = Join-Path $PSScriptRoot "windows"
$BuildDir = Join-Path $PSScriptRoot "build"
$ObjDir = Join-Path $BuildDir "obj-windows"
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$CompatHeader = Join-Path $WindowsRoot "r36sx_pico286_windows_compat.h"
$PrintfRenameHeader = Join-Path $WindowsRoot "r36sx_windows_printf_rename.h"
$DefaultPatchDir = Join-Path $Root "patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286"

if (!$Out) {
    $Out = Join-Path $BuildDir "pico_286_win.exe"
}
if (!$PatchDir) {
    $PatchDir = $DefaultPatchDir
}

$DebugValue = if ($DebugLog) { "1" } else { "0" }
if ($DebugLog -and $OptLevel -ne "O2") {
    Write-Host "Debug build requested; forcing -O2 instead of -$OptLevel"
    $OptLevel = "O2"
}

$ProfilingValue = if ($DisableProfiling) { "0" } else { "1" }
$ComputedGotoValue = if ($DisableComputedGoto) { "0" } else { "1" }
$FastMemoryValue = if ($DisableFastMemory) { "0" } else { "1" }
$ProtectedModeValue = if ($DisableProtectedMode) { "0" } else { "1" }
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

function Invoke-Checked {
    param([scriptblock]$Command)
    & $Command
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code $LASTEXITCODE"
    }
}

if (!(Test-Path $Zig)) {
    throw "Missing Zig compiler: $Zig"
}
if (!(Test-Path $PicoRoot)) {
    throw "Missing Pico-286 source tree: $PicoRoot"
}
if (!(Test-Path $PortRoot)) {
    throw "Missing R36SX port source tree: $PortRoot"
}
if (!(Test-Path $WindowsRoot)) {
    throw "Missing Windows compatibility source tree: $WindowsRoot"
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
    $OldErrorActionPreference = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    & git -C $RootPath diff --quiet --ignore-submodules -- . 2>$null
    $GitDiffExit = $LASTEXITCODE
    & git -C $RootPath diff --cached --quiet --ignore-submodules -- . 2>$null
    $GitCachedDiffExit = $LASTEXITCODE
    $ErrorActionPreference = $OldErrorActionPreference
    if ($GitDiffExit -ne 0) {
        $BuildGitDirty = "1"
    }
    if ($GitCachedDiffExit -ne 0) {
        $BuildGitDirty = "1"
    }
}

$ObjDirFull = [IO.Path]::GetFullPath($ObjDir)
$ExpectedObjDir = [IO.Path]::GetFullPath((Join-Path $BuildDir "obj-windows"))
if ($ObjDirFull -ne $ExpectedObjDir) {
    throw "Refusing to clean unexpected object directory: $ObjDirFull"
}
if (Test-Path -LiteralPath $ObjDirFull) {
    Remove-Item -LiteralPath $ObjDirFull -Recurse -Force
}
New-Item -ItemType Directory -Force $ObjDirFull | Out-Null
New-Item -ItemType Directory -Force ([IO.Path]::GetDirectoryName([IO.Path]::GetFullPath($Out))) | Out-Null

$IncludeArgs = @(
    "-I$WindowsRoot",
    "-I$(Join-Path $Root "homebrew\common")",
    "-I$PortRoot",
    "-I$PSScriptRoot",
    "-I$(Join-Path $PicoRoot "src")",
    "-I$(Join-Path $PicoRoot "src\emulator")",
    "-I$(Join-Path $PicoRoot "src\emu8950")",
    "-I$(Join-Path $PicoRoot "src\printf")"
)

$CommonArgs = @(
    "-target", "x86_64-windows-gnu",
    "-D__USE_MINGW_ANSI_STDIO=1",
    "-DPICO_RP2040=0",
    "-DPICO_RP2350=0",
    "-DDEBUG=$DebugValue",
    ("-DR36SX_BUILD_GIT_COMMIT=\`"{0}\`"" -f (ConvertTo-CMacroString $BuildGitCommit)),
    ("-DR36SX_BUILD_GIT_COMMIT_SHORT=\`"{0}\`"" -f (ConvertTo-CMacroString $BuildGitCommitShort)),
    ("-DR36SX_BUILD_COMMIT_OBJECT_SHA256=\`"{0}\`"" -f (ConvertTo-CMacroString $BuildCommitObjectSha256)),
    "-DR36SX_BUILD_GIT_DIRTY=$BuildGitDirty",
    "-DR36SX_ENABLE_PROFILING=$ProfilingValue",
    "-DR36SX_CPU_COMPUTED_GOTO=$ComputedGotoValue",
    "-DR36SX_NATIVE_FAST_MEMORY=$FastMemoryValue",
    "-DR36SX_ENABLE_PROTECTED_MODE=$ProtectedModeValue",
    "-DR36SX_SEGMENT_BASE_CACHE=1",
    "-DCPU_386_EXTENDED_OPS=1",
    "-DR36SX_RUNTIME_SOUND_FREQUENCY=1",
    "-DR36SX_VIDEO_DIRTY_TRACKING=1",
    "-DR36SX_MIPS_DSP=0",
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
    "-include", $CompatHeader,
    "-$OptLevel",
    "-fms-extensions",
    "-fno-strict-aliasing",
    "-fno-builtin-memset",
    "-fno-builtin-memcpy",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-missing-field-initializers",
    "-Wno-ignored-attributes"
)

$Objects = New-Object System.Collections.Generic.List[string]

function Get-ObjectPath {
    param([string]$Source)
    $Full = [IO.Path]::GetFullPath($Source)
    $Rel = $Full.Substring($Root.Path.Length).TrimStart('\', '/')
    $Name = ($Rel -replace "[:\\/ ]", "_")
    return (Join-Path $ObjDirFull ([IO.Path]::ChangeExtension($Name, ".o")))
}

function Compile-C {
    param([string]$Source)
    $Obj = Get-ObjectPath -Source $Source
    $ExtraArgs = @()
    if ([IO.Path]::GetFileName($Source) -eq "printf.c") {
        $ExtraArgs = @("-include", $PrintfRenameHeader)
    }
    Invoke-Checked { & $Zig cc @CommonArgs @IncludeArgs @ExtraArgs "-std=gnu11" "-c" $Source "-o" $Obj }
    $Objects.Add($Obj) | Out-Null
}

function Compile-Cpp {
    param([string]$Source)
    $Obj = Get-ObjectPath -Source $Source
    Invoke-Checked { & $Zig c++ @CommonArgs @IncludeArgs "-std=gnu++14" "-fpermissive" "-fno-exceptions" "-fno-rtti" "-c" $Source "-o" $Obj }
    $Objects.Add($Obj) | Out-Null
}

$CFiles = @()
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emulator") -Recurse -File -Filter "*.c" |
    Where-Object { $_.Name -ne "cpu.c" -and $_.Name -ne "ports.c" }
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emu8950") -File -Filter "*.c"
$CFiles += Get-Item (Join-Path $PicoRoot "src\printf\printf.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\inih\ini.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\r36sx_screenshot.c")
$CFiles += Get-Item (Join-Path $Root "homebrew\common\r36sx_screen_keyboard.c")
$CFiles += Get-Item (Join-Path $WindowsRoot "r36sx_winminifb.c")
$CFiles += Get-Item (Join-Path $WindowsRoot "r36sx_windows_audio.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_disk_menu.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_key_presets.c")
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

Invoke-Checked { & $Zig c++ -target x86_64-windows-gnu @Objects "-o" $Out "-luser32" "-lgdi32" "-lshell32" }

foreach ($Asset in @("pico_286.conf", "keypresets.conf", "test386.bin")) {
    $Source = Join-Path $PSScriptRoot $Asset
    if (Test-Path $Source) {
        Copy-Item -LiteralPath $Source -Destination (Join-Path $BuildDir $Asset) -Force
    }
}

if (!(Test-Path (Join-Path $BuildDir "host"))) {
    New-Item -ItemType Directory -Force (Join-Path $BuildDir "host") | Out-Null
}

if (!$NoPatchCopy) {
    if (!(Test-Path $PatchDir)) {
        New-Item -ItemType Directory -Force $PatchDir | Out-Null
    }
    Copy-Item -LiteralPath $Out -Destination (Join-Path $PatchDir ([IO.Path]::GetFileName($Out))) -Force
    Write-Host "Copied Windows debug executable to $PatchDir"
}

Write-Host "Built $Out"
