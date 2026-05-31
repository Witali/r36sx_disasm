param()

$ErrorActionPreference = "Stop"

$TestsRoot = $PSScriptRoot
$PicoRoot = Resolve-Path (Join-Path $TestsRoot "..")
$RepoRoot = Resolve-Path (Join-Path $PicoRoot "..\..")
$SourceRoot = Join-Path $TestsRoot "test386.asm"
$SourceFile = Join-Path $SourceRoot "src\test386.asm"
$IncludeDir = Join-Path $SourceRoot "src"
$BuildDir = Join-Path $SourceRoot "build"
$OutputBin = Join-Path $BuildDir "test386-r36sx.bin"
$OutputList = Join-Path $BuildDir "test386-r36sx.lst"
$Nasm = Join-Path $RepoRoot "tools\nasm-3.01-win64\nasm-3.01\nasm.exe"

if (!(Test-Path -LiteralPath $Nasm)) {
    throw "NASM was not found at $Nasm. Download nasm-3.01-win64.zip from https://www.nasm.us/ and extract it under tools first."
}
if (!(Test-Path -LiteralPath $SourceFile)) {
    throw "test386.asm source was not found at $SourceFile."
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

& $Nasm "-i$IncludeDir\" -f bin $SourceFile -w-all -l $OutputList -o $OutputBin
if ($LASTEXITCODE -ne 0) {
    throw "NASM failed with exit code $LASTEXITCODE."
}

$Hash = Get-FileHash -Algorithm SHA256 -LiteralPath $OutputBin
$Item = Get-Item -LiteralPath $OutputBin
Write-Host "Built $OutputBin"
Write-Host "Size: $($Item.Length) bytes"
Write-Host "SHA256: $($Hash.Hash)"
