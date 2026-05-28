param(
    [switch]$TryStrip
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$Fake86Root = Join-Path $Root "internet_sources\fake86"
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$ZigInclude = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\lib\include"
$Sysroot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
$SysrootInclude = Join-Path $Sysroot "usr\include"
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$ToolchainRoot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain"
$GccLib = Join-Path $ToolchainRoot "lib\gcc\mips-mti-linux-gnu\6.3.0\mipsel-r2-hard\lib"
$Out = Join-Path $PSScriptRoot "fake86"
$ObjDir = Join-Path $PSScriptRoot "obj"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"
$CrtBegin = Join-Path $GccLib "crtbegin.o"
$CrtEnd = Join-Path $GccLib "crtend.o"
$CompatHeader = Join-Path $PSScriptRoot "r36sx_fake86_compat.h"

if (!(Test-Path $Fake86Root)) {
    throw "Missing internet_sources\fake86. Clone https://github.com/lgblgblgb/fake86 first."
}

New-Item -ItemType Directory -Force $ObjDir | Out-Null
$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

$IncludeArgs = @(
    "-I$PSScriptRoot",
    "-I$(Join-Path $Root "homebrew\common")",
    "-I$(Join-Path $ObjDir "src")",
    "-I$(Join-Path $Fake86Root "src")",
    "-isystem$ZigInclude",
    "-isystem$SysrootInclude"
)

$CommonArgs = @(
    "-target", "mipsel-linux-gnu",
    "-march=mips32r2",
    "--sysroot=$Sysroot",
    "-DPATH_DATAFILES=`"/mnt/sdcard/MIPS_NATIVE/fake86/data/`"",
    "-D_DEFAULT_SOURCE",
    "-D_GNU_SOURCE",
    "-include",
    $CompatHeader,
    "-O2",
    "-fPIC",
    "-fno-sanitize=undefined",
    "-fno-builtin-memset",
    "-fno-builtin-memcpy",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-missing-field-initializers",
    "-Wno-unused-variable"
)

$Objects = New-Object System.Collections.Generic.List[string]

function Add-Object {
    param([string]$Path)
    $Objects.Add($Path) | Out-Null
}

function Invoke-Checked {
    param([scriptblock]$Command)
    & $Command
    if ($LASTEXITCODE -ne 0) {
        throw "Command failed with exit code $LASTEXITCODE"
    }
}

function Write-AsciiFile {
    param([string]$Path, [string]$Text)
    Set-Content -Path $Path -Value $Text -NoNewline -Encoding ascii
}

function New-Patched-Sources {
    $SrcDir = Join-Path $Fake86Root "src"
    $DstDir = Join-Path $ObjDir "src"
    New-Item -ItemType Directory -Force $DstDir | Out-Null

    Get-ChildItem -Path $SrcDir -File -Filter "*.c" | ForEach-Object {
        $Text = Get-Content -Raw -Path $_.FullName
        $Text = $Text.Replace("`r`n", "`n")

        if ($_.Name -eq "config.h") {
            throw "unexpected config.h in C source loop"
        }

        if ($_.Name -eq "main.c") {
            $Text = $Text.Replace('int main ( int argc, char *argv[] )
{
	puts(FAKE86_BANNER_STRING);',
'extern void r36sx_fake86_debug_reset(void);
extern void r36sx_fake86_debug_log(const char *format, ...);

int main ( int argc, char *argv[] )
{
	r36sx_fake86_debug_reset();
	freopen("/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stdout.log", "w", stdout);
	freopen("/mnt/sdcard/MIPS_NATIVE/fake86/fake86.stderr.log", "w", stderr);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	r36sx_fake86_debug_log("main start argc=%d", argc);
	if (argc < 2) {
		static char *default_argv[] = {
			"fake86", "-boot", "rom", "-nosound",
			"-speed", "4770000", "-delay", "16", NULL
		};
		argc = 8;
		argv = default_argv;
		r36sx_fake86_debug_log("using default args: -boot rom -nosound -speed 4770000 -delay 16");
	}
	puts(FAKE86_BANNER_STRING);')
            $Text = $Text.Replace('	while (running) {
		handleinput();',
'	while (running) {
		handleinput();')
            $Text = $Text.Replace('	printf("\n%lu instructions executed in %lu seconds.\n", (long unsigned int)totalexec, (long unsigned int)endtick);',
'	r36sx_fake86_debug_log("main exit totalexec=%lu seconds=%lu", (long unsigned int)totalexec, (long unsigned int)endtick);
	printf("\n%lu instructions executed in %lu seconds.\n", (long unsigned int)totalexec, (long unsigned int)endtick);')
        }

        if ($_.Name -eq "render.c") {
            $Text = $Text.Replace("#ifdef USE_OSD", "#if 0 /* R36SX native build: no OSD */")
        }

        $Dest = Join-Path $DstDir $_.Name
        Write-AsciiFile $Dest $Text
    }

    $ConfigSource = Join-Path $SrcDir "config.h"
    $ConfigDest = Join-Path $DstDir "config.h"
    $Config = Get-Content -Raw -Path $ConfigSource
    $Config = $Config.Replace("`r`n", "`n")
    $Config = $Config.Replace("#define USE_KVM", "/* R36SX native build: #undef USE_KVM */")
    $Config = $Config.Replace("#define USE_OSD", "/* R36SX native build: #undef USE_OSD */")
    $Config = $Config.Replace("#define AUDIO_DEFAULT_SAMPLE_RATE 48000", "#define AUDIO_DEFAULT_SAMPLE_RATE 44100")
    Write-AsciiFile $ConfigDest $Config

    return $DstDir
}

function Compile-C {
    param([string]$Source)
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig cc @CommonArgs @IncludeArgs "-std=c11" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

$PatchedSrc = New-Patched-Sources

$Skip = @(
    "imagegen.c",
    "kvm.c",
    "netcard.c",
    "osd.c"
)

Get-ChildItem -Path $PatchedSrc -File -Filter "*.c" |
    Where-Object { $Skip -notcontains $_.Name } |
    Sort-Object Name |
    ForEach-Object { Compile-C $_.FullName }

Compile-C (Join-Path $PSScriptRoot "r36sx_sdl.c")

Invoke-Checked { & $Zig ld.lld `
    "--sysroot=$Sysroot" `
    -EL `
    -m `
    elf32ltsmip `
    "--dynamic-linker=/lib/ld.so.1" `
    "--hash-style=sysv" `
    "-L$SysrootLib" `
    "-L$SysrootUsrLib" `
    "-L$GccLib" `
    -o $Out `
    $Crt1 `
    $Crti `
    $CrtBegin `
    @Objects `
    "-lpthread" `
    "-ldl" `
    "-lm" `
    "-lgcc_s" `
    "-lc" `
    $CrtEnd `
    $Crtn }

if ($TryStrip) {
    $StrippedOut = Join-Path $ObjDir "fake86.stripped"
    & $Zig objcopy "--strip-all" $Out $StrippedOut
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force $StrippedOut $Out
    } else {
        Write-Warning "zig objcopy --strip-all failed; keeping unstripped fake86"
        if (Test-Path $StrippedOut) {
            Remove-Item -Force $StrippedOut
        }
        $global:LASTEXITCODE = 0
    }
}

Write-Host "Built $Out"
