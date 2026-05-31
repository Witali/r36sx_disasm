param(
    [switch]$DebugLog,
    [switch]$DisableProfiling,
    [switch]$DisableComputedGoto,
    [switch]$DisableFastMemory,
    [ValidateSet("O0", "O1", "O2", "O3", "Os", "Og", "Ofast")]
    [string]$OptLevel = "O2",
    [switch]$EnableMipsDsp,
    [switch]$Strip,
    [string]$Out
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$Script = Join-Path $PSScriptRoot "build_pico_286_wsl.sh"
$RootWin = $Root.Path -replace "\\", "/"
$ScriptWin = ([IO.Path]::GetFullPath($Script)) -replace "\\", "/"
$WslRoot = (& wsl wslpath -a $RootWin).Trim()
$WslScript = (& wsl wslpath -a $ScriptWin).Trim()

$ArgsList = @()
if ($DebugLog) { $ArgsList += "--debug-log" }
if ($DisableProfiling) { $ArgsList += "--disable-profiling" }
if ($DisableComputedGoto) { $ArgsList += "--disable-computed-goto" }
if ($DisableFastMemory) { $ArgsList += "--disable-fast-memory" }
if ($OptLevel) {
    $ArgsList += "--opt-level"
    $ArgsList += $OptLevel
}
if ($EnableMipsDsp) { $ArgsList += "--enable-mips-dsp" }
if ($Strip) { $ArgsList += "--strip" }
if ($Out) {
    $ArgsList += "--out"
    $OutWin = ([IO.Path]::GetFullPath($Out)) -replace "\\", "/"
    $ArgsList += (& wsl wslpath -a $OutWin).Trim()
}

$QuotedArgs = $ArgsList | ForEach-Object {
    "'" + ($_ -replace "'", "'\\''") + "'"
}
$Command = "cd '$WslRoot' && bash '$WslScript'"
if ($QuotedArgs.Count -gt 0) {
    $Command += " " + ($QuotedArgs -join " ")
}

& wsl bash -lc $Command
if ($LASTEXITCODE -ne 0) {
    throw "WSL Pico-286 GCC build failed with exit code $LASTEXITCODE"
}
