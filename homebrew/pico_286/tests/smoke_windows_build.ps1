param(
    [Parameter(Mandatory = $true)][string]$Exe,
    [ValidatePattern('^[a-zA-Z0-9_-]+$')][string]$Tag = 'windows',
    [ValidateRange(5, 300)][int]$Seconds = 60
)

$ErrorActionPreference = 'Stop'
$Root = (Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
$Build = Join-Path $Root 'homebrew\pico_286\build'
$Exe = (Resolve-Path $Exe).Path
$Config = Join-Path $Build 'pico_286.conf'
$SavedConfig = [IO.File]::ReadAllBytes($Config)
$Diag = Join-Path $Root "patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\diagnostics\compiler-$Tag"
New-Item -ItemType Directory -Force $Diag | Out-Null
$Command = Join-Path $Diag 'command.txt'
$Response = Join-Path $Diag 'response.txt'
$Frame = Join-Path $Diag 'screen.bin'
$Log = Join-Path $Diag 'pico_286.log'
$Process = $null

function Read-LiveLog {
    $Stream = [IO.File]::Open($Log, [IO.FileMode]::Open, [IO.FileAccess]::Read,
                             [IO.FileShare]::ReadWrite)
    $Reader = New-Object IO.StreamReader($Stream)
    try { return $Reader.ReadToEnd() } finally { $Reader.Dispose() }
}

function Send-DebugCommand([string]$Text) {
    if (Test-Path -LiteralPath $Response) { Remove-Item -LiteralPath $Response }
    [IO.File]::WriteAllText("$Command.tmp", $Text, [Text.Encoding]::ASCII)
    Move-Item -LiteralPath "$Command.tmp" -Destination $Command -Force
    $Deadline = (Get-Date).AddSeconds(10)
    while (!(Test-Path -LiteralPath $Response)) {
        if ($Process.HasExited) { throw "Emulator exited: $($Process.ExitCode)" }
        if ((Get-Date) -gt $Deadline) { throw "Debug command timed out: $Text" }
        Start-Sleep -Milliseconds 100
    }
    $Reply = [IO.File]::ReadAllText($Response)
    if ($Reply -match 'error=') { throw $Reply }
    return $Reply
}

try {
    # Never attach user disks or touch the active patch config in a compiler test.
    $Rom = Join-Path $Build 'test386.bin'
    [IO.File]::WriteAllText($Config, @"
[cpu]
cpu_model=80386
cpu_mhz=20
[bios]
bios=test386
test_bios_rom=$Rom
[boot]
boot_mode=bios_prompt
[debug]
diagnostics_dir=$Diag
log_truncate_on_start=1
log_max_bytes=10485760
debug_control_enabled=1
debug_control_command_path=$Command
debug_control_response_path=$Response
debug_control_artifact_dir=.
"@, [Text.Encoding]::ASCII)
    # Do not mistake a previous run's completion marker for this process.
    [IO.File]::WriteAllText($Log, '')
    $Process = Start-Process -FilePath $Exe -WorkingDirectory $Build -WindowStyle Hidden -PassThru `
        -RedirectStandardOutput (Join-Path $Diag 'stdout.log') `
        -RedirectStandardError (Join-Path $Diag 'stderr.log')
    $Deadline = (Get-Date).AddSeconds($Seconds)
    $Finished = $false
    do {
        Start-Sleep -Milliseconds 500
        if ($Process.HasExited) { throw "Emulator exited: $($Process.ExitCode)" }
        try { $Finished = (Read-LiveLog) -match 'post: port=0x080 code=0xff' }
        catch [IO.IOException] { continue } # Retry a transient CRT sharing race.
    } while (!$Finished -and (Get-Date) -lt $Deadline)
    if (!$Finished) { throw "test386 did not reach POST 80:FF within $Seconds seconds" }
    $Reply = Send-DebugCommand 'regs'
    [IO.File]::WriteAllText((Join-Path $Diag 'regs.txt'), $Reply)
    Write-Output $Reply
    Write-Output (Send-DebugCommand "screen $Frame")
    $Pixels = [IO.File]::ReadAllBytes($Frame)
    if ($Pixels.Length -ne 640 * 480 * 2) { throw 'Unexpected framebuffer size' }
    if (($Pixels | Where-Object { $_ -ne 0 } | Select-Object -First 1).Count -eq 0) {
        throw 'Blank framebuffer'
    }
} finally {
    if ($Process -and !$Process.HasExited) {
        # Close only the process created here and allow the normal flush path.
        $null = $Process.CloseMainWindow()
        if (!$Process.WaitForExit(5000)) { Stop-Process -Id $Process.Id }
    }
    [IO.File]::WriteAllBytes($Config, $SavedConfig)
}
# MSVC's CRT can keep the result file open until shutdown.
Get-FileHash (Join-Path $Diag 'test386-ee-output.txt') | Select-Object Hash
Write-Output "PASS $Tag`: POST 80:FF, debug mailbox responsive, nonblank 640x480 RGB565 frame."
Write-Output 'This is a compiler smoke test, not a full CPU conformance test.'
