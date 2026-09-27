param(
    [string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-pop'
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$Build = Join-Path $Root 'homebrew/pico_286/build'
$Nasm = Join-Path $Root 'tools/nasm-3.01-win64/nasm-3.01/nasm.exe'
$Rom = Join-Path $Build 'cpu386_pop.bin'
if (!$Exe) { $Exe = Join-Path $Build 'pico_286_win.exe' }
New-Item -ItemType Directory -Force $Build | Out-Null

# Exercise production code without DOS/disks; restore the build config afterward.
& $Nasm -f bin (Join-Path $PSScriptRoot 'cpu386_pop.asm') -o $Rom `
    -l (Join-Path $Build 'cpu386_pop.lst')
if ($LASTEXITCODE -ne 0) { throw "NASM failed: $LASTEXITCODE" }
if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
Get-FileHash -LiteralPath $Rom | Select-Object Hash
& (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe -Tag $Tag `
    -Rom $Rom -SuccessMessage 'CPU386 POP PASS cases=800'
