param()

$ErrorActionPreference = "Stop"

$TestsRoot = $PSScriptRoot
$PicoRoot = Resolve-Path (Join-Path $TestsRoot "..")
$RepoRoot = Resolve-Path (Join-Path $PicoRoot "..\..")

& (Join-Path $TestsRoot "build_test386_r36sx.ps1")
if ($LASTEXITCODE -ne 0) {
    throw "test386 build failed with exit code $LASTEXITCODE."
}

$CreateFat = Join-Path $RepoRoot "tools\create_fat12_floppy.py"
$OutputImage = Join-Path $PicoRoot "cpu_tests.img"
$Test386Bin = Join-Path $TestsRoot "test386.asm\build\test386.bin"
$ExeSideTest386Bin = Join-Path $PicoRoot "test386.bin"
$Readme = Join-Path $PicoRoot "dos_files\cpu_tests_readme.txt"
$PcjsRoot = Join-Path $RepoRoot "internet_sources\pcjs_cpu_tests\software\pcx86\test\cpu"

if (!(Test-Path -LiteralPath $CreateFat)) {
    throw "FAT12 image helper was not found at $CreateFat."
}
if (!(Test-Path -LiteralPath $PcjsRoot)) {
    throw "PCjs CPU test sources were not found at $PcjsRoot."
}

python $CreateFat `
    --output $OutputImage `
    --label CPUTESTS `
    --file "ID.COM=$(Join-Path $PcjsRoot 'bin\id.com')" `
    --file "TEST386.COM=$(Join-Path $PcjsRoot 'bin\test386.com')" `
    --file "TEST386.BIN=$Test386Bin" `
    --file "CPUID.ASM=$(Join-Path $PcjsRoot 'cpuid.asm')" `
    --file "ID.ASM=$(Join-Path $PcjsRoot 'id.asm')" `
    --file "README.TXT=$Readme"

if ($LASTEXITCODE -ne 0) {
    throw "FAT12 image build failed with exit code $LASTEXITCODE."
}

Copy-Item -LiteralPath $Test386Bin -Destination $ExeSideTest386Bin -Force
Write-Host "Copied $ExeSideTest386Bin"

$Hash = Get-FileHash -Algorithm SHA256 -LiteralPath $OutputImage
$Item = Get-Item -LiteralPath $OutputImage
Write-Host "Built $OutputImage"
Write-Host "Size: $($Item.Length) bytes"
Write-Host "SHA256: $($Hash.Hash)"
