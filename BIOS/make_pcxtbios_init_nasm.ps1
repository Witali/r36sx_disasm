param(
    [string]$InputPath = ".\BIOS\pcxtbios_init_annotated.asm",
    [string]$OutputPath = ".\BIOS\pcxtbios_init_annotated_nasm.asm"
)

$ErrorActionPreference = "Stop"

function Convert-InstructionLine {
    param([string]$Text)

    $Code = $Text
    $Comment = ""
    $CommentIndex = $Text.IndexOf(";")
    if ($CommentIndex -ge 0) {
        $Code = $Text.Substring(0, $CommentIndex)
        $Comment = $Text.Substring($CommentIndex)
    }

    # Ghidra/Intel listings use "word ptr"; NASM wants just "word".
    $Code = [regex]::Replace(
        $Code,
        "\b(byte|word|dword|qword)\s+ptr\b",
        { param($m) $m.Groups[1].Value.ToLowerInvariant() },
        [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)

    # Convert Intel segment override form "es:[di]" to NASM "[es:di]".
    $Code = [regex]::Replace(
        $Code,
        "\b(es|cs|ds|ss|fs|gs):\[([^\]]+)\]",
        { param($m) "[" + $m.Groups[1].Value.ToLowerInvariant() + ":" + $m.Groups[2].Value + "]" },
        [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)

    # Same-segment ROM references become labels. Comments keep the original
    # addresses so the annotated source remains easy to compare with Ghidra.
    $Code = [regex]::Replace(
        $Code,
        "\b[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4})\b",
        { param($m) "loc_" + $m.Groups[1].Value.ToUpperInvariant() })

    $Code = $Code.Trim()
    if ($Code.Length -eq 0) {
        return $Comment
    }
    $XorMatch = [regex]::Match(
        $Code,
        "^xor\s+(ax|cx|dx|bx|sp|bp|si|di)\s*,\s*\1$",
        [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)
    if ($XorMatch.Success) {
        $ModRmByRegister = @{
            "ax" = "0C0h"
            "cx" = "0C9h"
            "dx" = "0D2h"
            "bx" = "0DBh"
            "sp" = "0E4h"
            "bp" = "0EDh"
            "si" = "0F6h"
            "di" = "0FFh"
        }
        $Reg = $XorMatch.Groups[1].Value.ToLowerInvariant()
        $Line = "    db 033h," + $ModRmByRegister[$Reg] + " ; " + $Code
        if ($Comment.Length -gt 0) {
            $Line += " " + $Comment
        }
        return $Line
    }
    if ($Comment.Length -gt 0) {
        return "    " + $Code + " " + $Comment
    }
    return "    " + $Code
}

$InputLines = @(Get-Content -LiteralPath $InputPath)
$DefinedOffsets = New-Object System.Collections.Generic.HashSet[string]
$ReferencedOffsets = New-Object System.Collections.Generic.HashSet[string]

foreach ($Line in $InputLines) {
    $Match = [regex]::Match($Line, "^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\s*(.*)$")
    if (!$Match.Success) {
        continue
    }

    [void]$DefinedOffsets.Add($Match.Groups[2].Value.ToUpperInvariant())

    $Rest = $Match.Groups[3].Value
    $CommentIndex = $Rest.IndexOf(";")
    if ($CommentIndex -ge 0) {
        $Rest = $Rest.Substring(0, $CommentIndex)
    }
    foreach ($Reference in [regex]::Matches($Rest, "\b[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4})\b")) {
        [void]$ReferencedOffsets.Add($Reference.Groups[1].Value.ToUpperInvariant())
    }
}

$Output = New-Object System.Collections.Generic.List[string]
$Output.Add("; NASM rebuild source generated from BIOS/pcxtbios_init_annotated.asm.") | Out-Null
$Output.Add("; Address prefixes are converted to loc_XXXX labels; unlisted gaps are") | Out-Null
$Output.Add("; byte-preserved from BIOS/pcxtbios.bin so the output can still rebuild") | Out-Null
$Output.Add("; the complete 8 KiB ROM while selected init code is assembled as text.") | Out-Null
$Output.Add(";") | Out-Null
$Output.Add("; Build: nasm -f bin BIOS/pcxtbios_init_annotated_nasm.asm -o BIOS/pcxtbios_init_annotated.bin") | Out-Null
$Output.Add("bits 16") | Out-Null
$Output.Add("org 0xe000") | Out-Null
$Output.Add("") | Out-Null
$Output.Add("%define ROM_BASE 0xe000") | Out-Null
$Output.Add("%macro gap_to 1") | Out-Null
$Output.Add("    %assign __target (%1 - ROM_BASE)") | Out-Null
$Output.Add("    %assign __here (`$ - `$$)") | Out-Null
$Output.Add("    %if __target < __here") | Out-Null
$Output.Add("        %error `"assembled past annotated BIOS address`"") | Out-Null
$Output.Add("    %elif __target > __here") | Out-Null
$Output.Add("        incbin `"BIOS/pcxtbios.bin`", __here, __target - __here") | Out-Null
$Output.Add("    %endif") | Out-Null
$Output.Add("%endmacro") | Out-Null
$Output.Add("") | Out-Null

foreach ($Offset in ($ReferencedOffsets | Sort-Object)) {
    if (!$DefinedOffsets.Contains($Offset)) {
        $Output.Add("loc_$Offset equ 0x$Offset ; target inside byte-preserved ROM gap") | Out-Null
    }
}
if ($ReferencedOffsets.Count -gt 0) {
    $Output.Add("") | Out-Null
}

foreach ($Line in $InputLines) {
    $Match = [regex]::Match($Line, "^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\s*(.*)$")
    if (!$Match.Success) {
        $Output.Add($Line) | Out-Null
        continue
    }

    $Offset = $Match.Groups[2].Value.ToUpperInvariant()
    $Rest = $Match.Groups[3].Value
    $Output.Add("gap_to 0x$Offset") | Out-Null
    $Converted = Convert-InstructionLine $Rest
    if ($Converted.Trim().Length -eq 0) {
        $Output.Add("loc_${Offset}:") | Out-Null
    } elseif ($Converted.TrimStart().StartsWith(";")) {
        $Output.Add("loc_${Offset}: $($Converted.TrimStart())") | Out-Null
    } else {
        $Output.Add("loc_${Offset}:") | Out-Null
        $Output.Add($Converted) | Out-Null
    }
}

$Output.Add("") | Out-Null
$Output.Add("; Preserve the rest of the ROM image through F000:FFFF.") | Out-Null
$Output.Add("gap_to 0x10000") | Out-Null

$OutputDir = Split-Path -Parent $OutputPath
if ($OutputDir -and !(Test-Path -LiteralPath $OutputDir)) {
    New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
}
$Output | Set-Content -LiteralPath $OutputPath -Encoding ASCII
Write-Host "Generated $OutputPath"
