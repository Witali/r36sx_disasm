param(
    [string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-compare-faults'
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$Build = Join-Path $Root 'homebrew/pico_286/build'
$Nasm = Join-Path $Root 'tools/nasm-3.01-win64/nasm-3.01/nasm.exe'
$Rom = Join-Path $Build 'cpu386_compare_faults.bin'
if (!$Exe) { $Exe = Join-Path $Build 'pico_286_win.exe' }
New-Item -ItemType Directory -Force $Build | Out-Null

# Actual CPL3 fault delivery, repeated fault, repair and IRETD retry; no disks.
& $Nasm -f bin (Join-Path $PSScriptRoot 'cpu386_compare_faults.asm') -o $Rom `
    -l (Join-Path $Build 'cpu386_compare_faults.lst')
if ($LASTEXITCODE -ne 0) { throw "NASM failed: $LASTEXITCODE" }
if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
Get-FileHash -LiteralPath $Rom | Select-Object Hash
& (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe -Tag $Tag `
    -Rom $Rom -Seconds 240 -SuccessMessage 'CPU386 COMPARE FAULTS PASS cases=2592'

# The 1025-iteration case must span CPU calls, not just decoder iterations.
# Check the live binary's reported budget rather than assuming the default.
$Log = Join-Path $Root "patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/diagnostics/compiler-$Tag/pico_286.log"
$Budget = [regex]::Match((Get-Content -LiteralPath $Log -Raw), 'main:.*\bmicro_exec=(\d+)')
if (!$Budget.Success -or [uint32]$Budget.Groups[1].Value -eq 0 -or
    [uint32]$Budget.Groups[1].Value -ge 1025) {
    throw 'Cannot prove that the long REP crosses interpreter quanta'
}
Write-Output "PASS $Tag`: 1025 comparisons cross CPU call budget $($Budget.Groups[1].Value)."
