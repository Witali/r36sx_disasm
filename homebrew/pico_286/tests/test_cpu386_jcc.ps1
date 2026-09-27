param(
    [string]$Exe = (Join-Path $PSScriptRoot '..\build\pico_286_win.exe'),
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-jcc',
    [ValidateRange(0, 12)][int[]]$Phase = (0..12),
    [switch]$VerifyOracle
)
$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$Build = Join-Path $Root 'homebrew\pico_286\build'
$Nasm = Join-Path $Root 'tools\nasm-3.01-win64\nasm-3.01\nasm.exe'
$Source = Join-Path $PSScriptRoot 'cpu386_jcc.asm'
$Scan = Join-Path $Root 'tools\scan-download.ps1'
$Smoke = Join-Path $PSScriptRoot 'smoke_windows_build.ps1'
$Completed = 0
foreach ($Index in $Phase) {
    # Separate runs keep diagnostics below the normal log cap and localize
    # failures. No phase is considered complete without its exact ROM marker.
    $Number = '{0:D2}' -f $Index
    $Rom = Join-Path $Build "cpu386_jcc_$Number.bin"
    & $Nasm -f bin "-DJCC_PHASE=$Index" $Source -o $Rom `
        -l (Join-Path $Build "cpu386_jcc_$Number.lst")
    if ($LASTEXITCODE -ne 0) { throw "NASM failed for phase $Number" }
    if ((Get-Item -LiteralPath $Rom).Length -ne 65536) { throw 'Invalid ROM size' }
    & $Scan $Rom
    & $Smoke -Exe $Exe -Tag "$Tag-$Number" -Rom $Rom -Seconds 180 `
        -SuccessMessage "CPU386 JCC PASS phase=$Number cases=8192"
    $Completed++
}
if ($VerifyOracle) {
    $BadRom = Join-Path $Build 'cpu386_jcc_bad_oracle.bin'
    & $Nasm -f bin -DJCC_PHASE=0 -DJCC_BAD_ORACLE=1 $Source -o $BadRom
    if ($LASTEXITCODE -ne 0) { throw 'Negative-oracle assembly failed' }
    & $Scan $BadRom
    $Rejected = $false
    try {
        & $Smoke -Exe $Exe -Tag "$Tag-oracle" -Rom $BadRom -Seconds 20 `
            -SuccessMessage 'CPU386 JCC PASS phase=00 cases=8192'
    } catch {
        $Log = Join-Path $Root "patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\diagnostics\compiler-$Tag-oracle\pico_286.log"
        $Text = Get-Content -LiteralPath $Log -Raw
        $Rejected = $Text -match 'CPU386 JCC FAIL phase=00 case=00000000 check=00000002 step=00000000 got=00001002 want=00001003'
        if (!$Rejected) { throw }
    }
    if (!$Rejected) { throw 'Wrong Jcc EIP oracle unexpectedly passed' }
    Write-Output 'PASS negative oracle: the deliberately wrong not-taken EIP was rejected.'
}
Write-Output "PASS Jcc phases=$Completed cases=$($Completed * 8192)"
