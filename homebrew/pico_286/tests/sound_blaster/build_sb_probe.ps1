$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$PicoRoot = Resolve-Path (Join-Path $ScriptDir "..\..")
$RepoRoot = Resolve-Path (Join-Path $PicoRoot "..\..")
$Nasm = Join-Path $RepoRoot "tools\nasm-3.01-win64\nasm-3.01\nasm.exe"
$BuildDir = Join-Path $ScriptDir "build"
$Source = Join-Path $ScriptDir "sb_probe.asm"
$Output = Join-Path $BuildDir "SBPROBE.COM"
$Listing = Join-Path $BuildDir "sb_probe.lst"

if (!(Test-Path -LiteralPath $Nasm)) {
    throw "NASM not found: $Nasm"
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
& $Nasm -f bin $Source -o $Output -l $Listing -w-all
if ($LASTEXITCODE -ne 0) {
    throw "NASM failed with exit code $LASTEXITCODE"
}

Write-Host "Built $Output"
