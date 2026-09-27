param(
    [string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-string-traps',
    [switch]$VerifyOracle
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$Build = Join-Path $Root 'homebrew/pico_286/build'
$Nasm = Join-Path $Root 'tools/nasm-3.01-win64/nasm-3.01/nasm.exe'
$Rom = Join-Path $Build 'cpu386_string_traps.bin'
if (!$Exe) { $Exe = Join-Path $Build 'pico_286_win.exe' }
New-Item -ItemType Directory -Force $Build | Out-Null

& $Nasm -f bin (Join-Path $PSScriptRoot 'cpu386_string_traps.asm') -o $Rom `
    -l (Join-Path $Build 'cpu386_string_traps.lst')
if ($LASTEXITCODE -ne 0) { throw "NASM failed: $LASTEXITCODE" }
if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
Get-FileHash -LiteralPath $Rom | Select-Object Hash
& (Join-Path $Root 'tools/scan-download.ps1') $Rom
& (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe -Tag $Tag `
    -Rom $Rom -Seconds 300 -SuccessMessage 'CPU386 STRING TRAPS PASS cases=1128'

if ($VerifyOracle) {
    $BadRom = Join-Path $Build 'cpu386_string_traps_bad_oracle.bin'
    & $Nasm -f bin -DSTRING_TRAPS_BAD_ORACLE=1 `
        (Join-Path $PSScriptRoot 'cpu386_string_traps.asm') -o $BadRom
    if ($LASTEXITCODE -ne 0) { throw "Negative-control NASM failed: $LASTEXITCODE" }
    if ((Get-Item -LiteralPath $BadRom).Length -ne 65536) { throw 'Invalid control ROM size' }
    & (Join-Path $Root 'tools/scan-download.ps1') $BadRom
    $Rejected = $false
    try {
        & (Join-Path $PSScriptRoot 'smoke_windows_build.ps1') -Exe $Exe `
            -Tag "$Tag-oracle" -Rom $BadRom -Seconds 30 `
            -SuccessMessage 'CPU386 STRING TRAPS PASS cases=1128'
    } catch {
        # Only this exact final-step EIP mismatch counts as oracle rejection.
        if ($_.Exception.Message -notlike '*Regression ROM reported failure*') { throw }
        $Log = Join-Path $Root "patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/diagnostics/compiler-$Tag-oracle/pico_286.log"
        $Expected = 'CPU386 STRING TRAPS FAIL case=00000002 check=00000002 step=00000001 got=00000002 want=00000000'
        if ((Get-Content -LiteralPath $Log -Raw) -notmatch [regex]::Escape($Expected)) {
            throw "Wrong negative-control failure; see $Log"
        }
        $Rejected = $true
    }
    if (!$Rejected) { throw 'Corrupted last-step EIP oracle was not rejected' }
    Write-Output "PASS $Tag-oracle`: deliberately wrong final EIP rejected at case 2."
}
