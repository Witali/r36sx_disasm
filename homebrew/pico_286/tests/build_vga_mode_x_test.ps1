param(
    [string]$OutputName = "vga_test.com",
    [string]$HddImage,
    [switch]$NoCopy
)

$ErrorActionPreference = "Stop"

$TestsRoot = $PSScriptRoot
$PicoRoot = Resolve-Path (Join-Path $TestsRoot "..")
$RepoRoot = Resolve-Path (Join-Path $PicoRoot "..\..")
$SourceFile = Join-Path $TestsRoot "vga_mode_x_test.asm"
$BuildDir = Join-Path $RepoRoot "local_artifacts"
$OutputCom = Join-Path $BuildDir $OutputName
$OutputList = Join-Path $BuildDir ([IO.Path]::ChangeExtension($OutputName, ".lst"))
$Nasm = Join-Path $RepoRoot "tools\nasm-3.01-win64\nasm-3.01\nasm.exe"

if (!$HddImage) {
    $HddImage = Join-Path $RepoRoot "patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\images\hdd.hdd"
}

function ConvertTo-WslPath {
    param([string]$Path)

    $Full = [IO.Path]::GetFullPath($Path)
    if ($Full -match "^([A-Za-z]):\\(.*)$") {
        $Drive = $Matches[1].ToLowerInvariant()
        $Rest = $Matches[2] -replace "\\", "/"
        return "/mnt/$Drive/$Rest"
    }

    $WslPath = (& wsl wslpath -a $Full).Trim()
    if ($LASTEXITCODE -ne 0) {
        throw "wslpath failed for $Full"
    }
    return $WslPath
}

function Quote-ShSingle {
    param([string]$Text)
    return "'" + ($Text -replace "'", "'\''") + "'"
}

function Get-FirstPartitionOffsetBytes {
    param([string]$ImagePath)

    $Bytes = New-Object byte[] 512
    $Stream = [IO.File]::OpenRead($ImagePath)
    try {
        $Read = $Stream.Read($Bytes, 0, $Bytes.Length)
    } finally {
        $Stream.Dispose()
    }

    if ($Read -ne 512 -or $Bytes[510] -ne 0x55 -or $Bytes[511] -ne 0xAA) {
        return [uint64]0
    }

    $PartitionOffset = 0x1BE
    $PartitionType = $Bytes[$PartitionOffset + 4]
    $FirstLba = [BitConverter]::ToUInt32($Bytes, $PartitionOffset + 8)

    if ($PartitionType -eq 0 -or $FirstLba -eq 0) {
        return [uint64]0
    }

    return [uint64]$FirstLba * [uint64]512
}

if (!(Test-Path -LiteralPath $Nasm)) {
    throw "NASM was not found at $Nasm."
}
if (!(Test-Path -LiteralPath $SourceFile)) {
    throw "VGA Mode X test source was not found at $SourceFile."
}

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

& $Nasm -f bin $SourceFile -w-all -l $OutputList -o $OutputCom
if ($LASTEXITCODE -ne 0) {
    throw "NASM failed with exit code $LASTEXITCODE."
}

$Item = Get-Item -LiteralPath $OutputCom
$Hash = Get-FileHash -Algorithm SHA256 -LiteralPath $OutputCom
Write-Host "Built $OutputCom"
Write-Host "Size: $($Item.Length) bytes"
Write-Host "SHA256: $($Hash.Hash)"

if ($NoCopy) {
    return
}

if (!(Test-Path -LiteralPath $HddImage)) {
    throw "HDD image was not found at $HddImage."
}

$Offset = Get-FirstPartitionOffsetBytes -ImagePath $HddImage
$WslImage = ConvertTo-WslPath $HddImage
$WslCom = ConvertTo-WslPath $OutputCom
$ImageSpec = if ($Offset -gt 0) { "$WslImage@@$Offset" } else { $WslImage }
$DosName = $OutputName.ToUpperInvariant()

Write-Host "Copying $DosName to $HddImage"
Write-Host "mtools image spec: $ImageSpec"

$CopyCommand = "mcopy -o -i $(Quote-ShSingle $ImageSpec) $(Quote-ShSingle $WslCom) $(Quote-ShSingle `"::/$DosName`")"
& wsl sh -lc $CopyCommand
if ($LASTEXITCODE -ne 0) {
    throw "mcopy failed with exit code $LASTEXITCODE."
}

$DirCommand = "mdir -i $(Quote-ShSingle $ImageSpec) $(Quote-ShSingle `"::/$DosName`")"
& wsl sh -lc $DirCommand
if ($LASTEXITCODE -ne 0) {
    throw "mdir verification failed with exit code $LASTEXITCODE."
}
