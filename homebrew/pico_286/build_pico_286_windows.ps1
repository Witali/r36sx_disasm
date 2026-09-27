param(
    [ValidateSet("MSVC", "Zig", "MinGW")]
    [string]$Compiler = "MSVC",
    [string]$CygwinRoot,
    [switch]$DebugLog,
    [switch]$DisableProfiling,
    [switch]$DisableComputedGoto,
    [switch]$DisableFastMemory,
    [switch]$DisableProtectedMode,
    [switch]$DisableProtectedModeDebug,
    [switch]$RedirectorTrace,
    [switch]$HostRpcTrace,
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
$ObjDirName = "obj-windows-$($Compiler.ToLowerInvariant())"
$ObjDir = Join-Path $BuildDir $ObjDirName
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

$DebugValue = if ($DebugLog -or $RedirectorTrace -or $HostRpcTrace) { "1" } else { "0" }
if (($DebugLog -or $RedirectorTrace -or $HostRpcTrace) -and $OptLevel -ne "O2") {
    Write-Host "Debug build requested; forcing -O2 instead of -$OptLevel"
    $OptLevel = "O2"
}

$ProfilingValue = if ($DisableProfiling) { "0" } else { "1" }
# MSVC does not implement GNU labels-as-values; use the shared switch core.
$ComputedGotoValue = if ($Compiler -eq "MSVC" -or $DisableComputedGoto) { "0" } else { "1" }
$FastMemoryValue = if ($DisableFastMemory) { "0" } else { "1" }
$ProtectedModeValue = if ($DisableProtectedMode) { "0" } else { "1" }
$ProtectedModeDebugValue = if ($DisableProtectedModeDebug) { "0" } else { "1" }
$RedirectorTraceValue = if ($RedirectorTrace) { "1" } else { "0" }
$HostRpcTraceValue = if ($HostRpcTrace) { "1" } else { "0" }
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

$CygwinPaths = @{}
function Invoke-MinGW {
    param([string]$Tool, [string[]]$Arguments, [string]$ResponseFile)
    # Cygwin and Win32 have different argv quote parsing. A GCC response file
    # preserves paths with spaces and C string macro quotes through both layers.
    $Lines = foreach ($Argument in $Arguments) {
        if ($Argument.StartsWith('-D')) {
            $Argument = $Argument.Replace('\"', '"')
        } elseif ($Argument -match '^(-I)?([A-Za-z]:[\\/].*)$') {
            $Prefix = $Matches[1]
            $NativePath = $Matches[2]
            if (!$CygwinPaths.ContainsKey($NativePath)) {
                $Converted = & (Join-Path $CygwinBin 'cygpath.exe') -u $NativePath
                if ($LASTEXITCODE -ne 0) { throw "cygpath failed: $NativePath" }
                $CygwinPaths[$NativePath] = $Converted
            }
            $Argument = $Prefix + $CygwinPaths[$NativePath]
        } else {
            $Argument = $Argument.Replace('\', '/')
        }
        '"' + $Argument.Replace('\', '\\').Replace('"', '\"') + '"'
    }
    [IO.File]::WriteAllLines($ResponseFile, [string[]]$Lines, (New-Object Text.UTF8Encoding($false)))
    Invoke-Checked { & $Tool "@$ResponseFile" }
}

if ($Compiler -eq "MSVC") {
    $VsWhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (!(Test-Path $VsWhere)) {
        throw "Install Visual Studio C++ desktop tools and a Windows SDK (vswhere.exe missing)."
    }
    $VsRoot = & $VsWhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (!$VsRoot) {
        throw "No Visual Studio installation with MSVC x64 tools was found."
    }
    & (Join-Path $VsRoot "Common7\Tools\Launch-VsDevShell.ps1") -Arch amd64 -HostArch amd64 -SkipAutomaticLocation | Out-Null
    $Cl = (Get-Command cl.exe -ErrorAction Stop).Source
    $Linker = (Get-Command link.exe -ErrorAction Stop).Source
    Write-Host "Using MSVC: $Cl (x64, switch CPU dispatch)"
} elseif ($Compiler -eq "Zig") {
    if (!(Test-Path $Zig)) {
        throw "Missing Zig compiler: $Zig"
    }
    # Keep compiler caches writable without changing the user's global cache.
    if (!$env:ZIG_GLOBAL_CACHE_DIR) {
        $env:ZIG_GLOBAL_CACHE_DIR = Join-Path $BuildDir "zig-cache"
    }
    Write-Host "Using Zig: $Zig (x64, computed goto=$ComputedGotoValue)"
} else {
    if (!$CygwinRoot) {
        $CygwinRoot = @((Join-Path $Root "tools\cygwin64"), "C:\cygwin64") |
            Where-Object { Test-Path (Join-Path $_ "bin\x86_64-w64-mingw32-gcc.exe") } |
            Select-Object -First 1
    }
    if (!$CygwinRoot) {
        throw "Install Cygwin packages mingw64-x86_64-gcc-core and mingw64-x86_64-gcc-g++, or pass -CygwinRoot."
    }
    $CygwinBin = Join-Path ([IO.Path]::GetFullPath($CygwinRoot)) "bin"
    $Gcc = Join-Path $CygwinBin "x86_64-w64-mingw32-gcc.exe"
    $Gxx = Join-Path $CygwinBin "x86_64-w64-mingw32-g++.exe"
    if (!(Test-Path $Gcc) -or !(Test-Path $Gxx)) {
        throw "Missing MinGW-w64 GCC/G++ in $CygwinBin"
    }
    # Only the compiler runs under Cygwin. The emulator EXE uses the Windows CRT.
    $env:PATH = "$CygwinBin;$env:PATH"
    $GccTarget = & $Gcc -dumpmachine
    if ($LASTEXITCODE -ne 0 -or $GccTarget -ne "x86_64-w64-mingw32") {
        throw "Unexpected GCC target '$GccTarget'; refusing a Cygwin-dependent EXE."
    }
    Write-Host "Using MinGW-w64 GCC: $Gcc (computed goto=$ComputedGotoValue)"
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
$ExpectedObjDir = [IO.Path]::GetFullPath((Join-Path $BuildDir $ObjDirName))
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
    "-DR36SX_DEBUG_386_PROTECTED_MODE=$ProtectedModeDebugValue",
    "-DR36SX_DEBUG_REDIRECTOR_TRACE=$RedirectorTraceValue",
    "-DR36SX_DEBUG_HOSTRPC_TRACE=$HostRpcTraceValue",
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
    "-DEMU8950_LINEAR=1"
)
if ($Compiler -eq "MSVC") {
    # MSVC has no /O3 or /Og; retain the script's shared optimization vocabulary.
    $MsvcOpt = switch ($OptLevel) {
        { $_ -in "O0", "Og" } { "/Od"; break }
        { $_ -in "O1", "Os" } { "/O1"; break }
        default { "/O2" }
    }
    $CommonArgs += @(
        "/nologo", "/MT", "/Zi", "/W3", "/utf-8", $MsvcOpt,
        "/Fd$(Join-Path $ObjDirFull 'compiler.pdb')", "/FI$CompatHeader",
        "/D_CRT_SECURE_NO_WARNINGS", "/D_CRT_NONSTDC_NO_WARNINGS"
    )
    Write-Host "Optimization: $MsvcOpt, static CRT, PDB symbols"
} else {
    $CommonArgs += @(
        "-D__USE_MINGW_ANSI_STDIO=1",
        "-include", $CompatHeader, "-$OptLevel", "-fms-extensions",
        "-fno-strict-aliasing", "-fno-builtin-memset", "-fno-builtin-memcpy",
        "-Wall", "-Wextra", "-Wno-unused-parameter", "-Wno-unused-function",
        "-Wno-missing-field-initializers", "-Wno-ignored-attributes"
    )
    if ($Compiler -eq "Zig") {
        $CommonArgs += @("-target", "x86_64-windows-gnu")
    } else {
        $CommonArgs += "-g"
    }
}

$Objects = New-Object System.Collections.Generic.List[string]

function Get-ObjectPath {
    param([string]$Source)
    $Full = [IO.Path]::GetFullPath($Source)
    $Rel = $Full.Substring($Root.Path.Length).TrimStart('\', '/')
    $Name = ($Rel -replace "[:\\/ ]", "_")
    return (Join-Path $ObjDirFull ([IO.Path]::ChangeExtension($Name, ".obj")))
}

function Compile-C {
    param([string]$Source)
    $Obj = Get-ObjectPath -Source $Source
    [string[]]$ExtraArgs = @()
    if ([IO.Path]::GetFileName($Source) -eq "printf.c") {
        $ExtraArgs = if ($Compiler -eq "MSVC") { @("/FI$PrintfRenameHeader") } else { @("-include", $PrintfRenameHeader) }
    }
    if ($Compiler -eq "MSVC") {
        Invoke-Checked { & $Cl @CommonArgs @IncludeArgs @ExtraArgs /std:c11 /TC /c $Source "/Fo$Obj" }
    } elseif ($Compiler -eq "Zig") {
        Invoke-Checked { & $Zig cc @CommonArgs @IncludeArgs @ExtraArgs -std=gnu11 -c $Source -o $Obj }
    } else {
        Invoke-MinGW $Gcc ($CommonArgs + $IncludeArgs + $ExtraArgs + @('-std=gnu11', '-c', $Source, '-o', $Obj)) "$Obj.rsp"
    }
    $Objects.Add($Obj) | Out-Null
}

function Compile-Cpp {
    param([string]$Source)
    $Obj = Get-ObjectPath -Source $Source
    if ($Compiler -eq "MSVC") {
        Invoke-Checked { & $Cl @CommonArgs @IncludeArgs /std:c++14 /TP /GR- /c $Source "/Fo$Obj" }
    } elseif ($Compiler -eq "Zig") {
        Invoke-Checked { & $Zig c++ @CommonArgs @IncludeArgs -std=gnu++14 -fpermissive -fno-exceptions -fno-rtti -c $Source -o $Obj }
    } else {
        Invoke-MinGW $Gxx ($CommonArgs + $IncludeArgs + @('-std=gnu++14', '-fpermissive', '-fno-exceptions', '-fno-rtti', '-c', $Source, '-o', $Obj)) "$Obj.rsp"
    }
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
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_post_overlay.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_app_stats.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_bios_rom.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_host_disk_io.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_disk_config.c")
$CFiles += Get-Item (Join-Path $PortRoot "r36sx_debug_control.c")
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

$PdbOut = [IO.Path]::ChangeExtension([IO.Path]::GetFullPath($Out), ".pdb")
if ($Compiler -eq "MSVC") {
    Invoke-Checked { & $Linker /NOLOGO @Objects "/OUT:$Out" "/PDB:$PdbOut" /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF /SUBSYSTEM:CONSOLE user32.lib gdi32.lib shell32.lib winmm.lib dbghelp.lib }
} elseif ($Compiler -eq "Zig") {
    Invoke-Checked { & $Zig c++ -target x86_64-windows-gnu @Objects -o $Out -luser32 -lgdi32 -lshell32 -lwinmm -ldbghelp }
} else {
    # Include GCC/C++ support statically so the patch needs no MinGW DLLs.
    Invoke-MinGW $Gxx ($Objects.ToArray() + @('-static', '-o', $Out, '-luser32', '-lgdi32', '-lshell32', '-lwinmm', '-ldbghelp')) (Join-Path $ObjDirFull 'link.rsp')
}

foreach ($Asset in @("pico_286.conf", "keypresets.conf", "test386.bin", "test286.bin")) {
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
    foreach ($Artifact in $(if ($Compiler -eq "MSVC") { @($PdbOut) } else { @() })) {
        if (Test-Path $Artifact) {
            Copy-Item -LiteralPath $Artifact -Destination (Join-Path $PatchDir ([IO.Path]::GetFileName($Artifact))) -Force
        }
    }
    Write-Host "Copied Windows $Compiler executable to $PatchDir"
}

Write-Host "Built $Out"
