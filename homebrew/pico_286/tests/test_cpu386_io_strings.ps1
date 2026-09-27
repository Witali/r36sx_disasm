param(
    [string]$Exe = (Join-Path $PSScriptRoot '..\build\pico_286_win.exe'),
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'cpu386-io-strings',
    [switch]$VerifyOracle
)
$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$Build = Join-Path $Root 'homebrew\pico_286\build'
$Nasm = Join-Path $Root 'tools\nasm-3.01-win64\nasm-3.01\nasm.exe'
$Source = Join-Path $PSScriptRoot 'cpu386_io_strings.asm'
$Rom = Join-Path $Build 'cpu386_io_strings.bin'
$Scan = Join-Path $Root 'tools\scan-download.ps1'
$Smoke = Join-Path $PSScriptRoot 'smoke_windows_build.ps1'
& $Nasm -f bin $Source -o $Rom -l (Join-Path $Build 'cpu386_io_strings.lst')
if ($LASTEXITCODE -ne 0) { throw 'NASM failed' }
& $Scan $Rom
& $Smoke -Exe $Exe -Tag $Tag -Rom $Rom -Seconds 180 `
    -SuccessMessage 'CPU386 IO STRINGS PASS cases=2496'
if ($VerifyOracle) {
    $BadRom = Join-Path $Build 'cpu386_io_strings_bad_oracle.bin'
    & $Nasm -f bin -DIO_STRINGS_BAD_ORACLE=1 $Source -o $BadRom
    if ($LASTEXITCODE -ne 0) { throw 'Negative-oracle assembly failed' }
    & $Scan $BadRom
    $Rejected = $false
    try {
        & $Smoke -Exe $Exe -Tag "$Tag-oracle" -Rom $BadRom -Seconds 20 `
            -SuccessMessage 'CPU386 IO STRINGS PASS cases=2496'
    } catch {
        $Log = Join-Path $Root "patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\diagnostics\compiler-$Tag-oracle\pico_286.log"
        # An unrelated crash/timeout must not count as rejection of the oracle.
        $Text = Get-Content -LiteralPath $Log -Raw
        $Rejected = $Text -match 'CPU386 IO STRINGS FAIL case=00000000 check=00000008 step=00000001 got=00000078 want=00000079'
        if (!$Rejected) { throw }
    }
    if (!$Rejected) { throw 'Wrong I/O data oracle unexpectedly passed' }
    Write-Output 'PASS negative oracle: the deliberately wrong port byte was rejected.'
}
