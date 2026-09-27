param(
    [string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-compare-strings',
    [switch]$VerifyOracle
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$Build = Join-Path $Root 'homebrew/pico_286/build'
$Nasm = Join-Path $Root 'tools/nasm-3.01-win64/nasm-3.01/nasm.exe'
$Rom = Join-Path $Build 'cpu386_compare_strings.bin'
if (!$Exe) { $Exe = Join-Path $Build 'pico_286_win.exe' }
New-Item -ItemType Directory -Force $Build | Out-Null

# Independent assembly-time flags and scalar memory checks; no user disks.
& $Nasm -f bin (Join-Path $PSScriptRoot 'cpu386_compare_strings.asm') -o $Rom `
    -l (Join-Path $Build 'cpu386_compare_strings.lst')
if ($LASTEXITCODE -ne 0) { throw "NASM failed: $LASTEXITCODE" }
if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
Get-FileHash -LiteralPath $Rom | Select-Object Hash
& (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe -Tag $Tag `
    -Rom $Rom -Seconds 300 -SuccessMessage 'CPU386 COMPARE STRINGS PASS cases=10752'

if ($VerifyOracle) {
    $BadRom = Join-Path $Build 'cpu386_compare_strings_bad_oracle.bin'
    & $Nasm -f bin -DCOMPARE_STRINGS_BAD_ORACLE=1 `
        (Join-Path $PSScriptRoot 'cpu386_compare_strings.asm') -o $BadRom
    if ($LASTEXITCODE -ne 0) { throw "Negative-control NASM failed: $LASTEXITCODE" }
    if ((Get-Item -LiteralPath $BadRom).Length -ne 65536) { throw 'Invalid control ROM size' }
    $Rejected = $false
    try {
        & (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe `
            -Tag "$Tag-oracle" -Rom $BadRom -Seconds 30 `
            -SuccessMessage 'CPU386 COMPARE STRINGS PASS cases=10752'
    } catch {
        # A crash, timeout, setup fault or stale log must not count as rejection.
        if ($_.Exception.Message -notlike '*Regression ROM reported failure*') { throw }
        $Log = Join-Path $Root "patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/diagnostics/compiler-$Tag-oracle/pico_286.log"
        $Expected = 'CPU386 COMPARE STRINGS FAIL case=00000000 check=00000002 got=00000044 want=00000045'
        if ((Get-Content -LiteralPath $Log -Raw) -notmatch [regex]::Escape($Expected)) {
            throw "Wrong negative-control failure; see $Log"
        }
        $Rejected = $true
    }
    if (!$Rejected) { throw 'Corrupted flags oracle was not rejected' }
    Write-Output "PASS $Tag-oracle`: deliberately incorrect CF rejected at case 0."
}
