param()

$ErrorActionPreference = "Stop"

$TestsRoot = $PSScriptRoot
$PicoRoot = Resolve-Path (Join-Path $TestsRoot "..")
$RepoRoot = Resolve-Path (Join-Path $PicoRoot "..\..")
$SourceRoot = Join-Path $TestsRoot "test286.asm"
$SourceFile = Join-Path $SourceRoot "src\test286.asm"
$IncludeDir = Join-Path $SourceRoot "src"
$BuildDir = Join-Path $SourceRoot "build"
$OutputBin = Join-Path $BuildDir "test286.bin"
$OutputList = Join-Path $BuildDir "test286.lst"
$ExeSideBin = Join-Path $PicoRoot "test286.bin"
$Nasm = Join-Path $RepoRoot "tools\nasm-3.01-win64\nasm-3.01\nasm.exe"

if (!(Test-Path -LiteralPath $Nasm)) {
    throw "NASM was not found at $Nasm."
}
if (!(Test-Path -LiteralPath $SourceFile)) {
    throw "test286.asm source was not found at $SourceFile."
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

& $Nasm "-i$IncludeDir\" -f bin $SourceFile -w-all -l $OutputList -o $OutputBin
if ($LASTEXITCODE -ne 0) {
    throw "NASM failed with exit code $LASTEXITCODE."
}

$Item = Get-Item -LiteralPath $OutputBin
if ($Item.Length -ne 65536) {
    throw "Unexpected test286.bin size: $($Item.Length), expected 65536."
}

Copy-Item -LiteralPath $OutputBin -Destination $ExeSideBin -Force
Write-Host "Copied $ExeSideBin"

$Hash = Get-FileHash -Algorithm SHA256 -LiteralPath $OutputBin
Write-Host "Built $OutputBin"
Write-Host "Size: $($Item.Length) bytes"
Write-Host "SHA256: $($Hash.Hash)"
