param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Path
)

$ErrorActionPreference = "Stop"

$resolved = Resolve-Path -LiteralPath $Path
$candidates = @(
    "C:\Program Files\Windows Defender\MpCmdRun.exe"
)

$platformRoot = "C:\ProgramData\Microsoft\Windows Defender\Platform"
if (Test-Path -LiteralPath $platformRoot) {
    $platformBins = Get-ChildItem -LiteralPath $platformRoot -Filter MpCmdRun.exe -Recurse -ErrorAction SilentlyContinue |
        Sort-Object FullName -Descending |
        Select-Object -ExpandProperty FullName
    $candidates = @($platformBins) + $candidates
}

$mpcmd = $candidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (-not $mpcmd) {
    throw "MpCmdRun.exe was not found. Install/enable Microsoft Defender or scan '$resolved' with another antivirus."
}

Write-Host "Scanning: $resolved"
Write-Host "Scanner:  $mpcmd"

& $mpcmd -Scan -ScanType 3 -File $resolved
$exitCode = $LASTEXITCODE

if ($exitCode -ne 0) {
    throw "Antivirus scan failed or found a problem. MpCmdRun exit code: $exitCode"
}

Write-Host "Scan completed successfully."
