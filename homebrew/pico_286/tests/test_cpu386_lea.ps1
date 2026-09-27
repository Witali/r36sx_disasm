param(
    [string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-lea'
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$Build = Join-Path $Root 'homebrew/pico_286/build'
$Nasm = Join-Path $Root 'tools/nasm-3.01-win64/nasm-3.01/nasm.exe'
$Rom = Join-Path $Build 'cpu386_lea.bin'
if (!$Exe) { $Exe = Join-Path $Build 'pico_286_win.exe' }
New-Item -ItemType Directory -Force $Build | Out-Null

# The large matrix executes only this ROM, with no DOS or attached user disks.
& $Nasm -f bin (Join-Path $PSScriptRoot 'cpu386_lea.asm') -o $Rom `
    -l (Join-Path $Build 'cpu386_lea.lst')
if ($LASTEXITCODE -ne 0) { throw "NASM failed: $LASTEXITCODE" }
if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
Get-FileHash -LiteralPath $Rom | Select-Object Hash
& (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe -Tag $Tag `
    -Rom $Rom -Seconds 300 -SuccessMessage 'CPU386 LEA PASS cases=26528'
