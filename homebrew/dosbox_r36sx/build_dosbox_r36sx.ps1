param(
    [switch]$TryStrip
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$DosboxRoot = Join-Path $PSScriptRoot "dosbox-0.74-3"
$PortRoot = Join-Path $PSScriptRoot "r36sx_port"
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$ZigInclude = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\lib\include"
$Sysroot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
$SysrootInclude = Join-Path $Sysroot "usr\include"
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$ToolchainRoot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain"
$CppInclude = Join-Path $ToolchainRoot "mips-mti-linux-gnu\include\c++\6.3.0"
$CppTargetInclude = Join-Path $CppInclude "mips-mti-linux-gnu\mipsel-r2-hard\lib"
$GccLib = Join-Path $ToolchainRoot "lib\gcc\mips-mti-linux-gnu\6.3.0\mipsel-r2-hard\lib"
$TargetCppLib = Join-Path $ToolchainRoot "mips-mti-linux-gnu\lib\mipsel-r2-hard\lib"
$Out = Join-Path $PSScriptRoot "dosbox_r36sx"
$ObjDir = Join-Path $PSScriptRoot "obj"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"
$CrtBegin = Join-Path $GccLib "crtbegin.o"
$CrtEnd = Join-Path $GccLib "crtend.o"
$SdlLib = Join-Path $SysrootUsrLib "libSDL-1.2.so.0.11.4"

if (!(Test-Path $DosboxRoot)) {
    throw "Missing homebrew\dosbox_r36sx\dosbox-0.74-3 source tree."
}
if (!(Test-Path (Join-Path $PortRoot "config.h"))) {
    throw "Missing R36SX DOSBox config.h."
}
if (!(Test-Path $SdlLib)) {
    throw "Missing target SDL 1.2 library: $SdlLib"
}

$ObjDirFull = [IO.Path]::GetFullPath($ObjDir)
$ExpectedObjDir = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "obj"))
if ($ObjDirFull -ne $ExpectedObjDir) {
    throw "Refusing to clean unexpected object directory: $ObjDirFull"
}
if (Test-Path -LiteralPath $ObjDirFull) {
    Remove-Item -LiteralPath $ObjDirFull -Recurse -Force
}
New-Item -ItemType Directory -Force $ObjDir | Out-Null

$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

$IncludeArgs = @(
    "-I$PortRoot",
    "-I$(Join-Path $DosboxRoot "include")",
    "-I$(Join-Path $DosboxRoot "src")",
    "-I$(Join-Path $DosboxRoot "src\cpu")",
    "-I$(Join-Path $DosboxRoot "src\debug")",
    "-I$(Join-Path $DosboxRoot "src\dos")",
    "-I$(Join-Path $DosboxRoot "src\fpu")",
    "-I$(Join-Path $DosboxRoot "src\gui")",
    "-I$(Join-Path $DosboxRoot "src\hardware")",
    "-I$(Join-Path $DosboxRoot "src\hardware\mame")",
    "-I$(Join-Path $DosboxRoot "src\hardware\serialport")",
    "-I$(Join-Path $DosboxRoot "src\ints")",
    "-I$(Join-Path $DosboxRoot "src\libs\gui_tk")",
    "-I$(Join-Path $DosboxRoot "src\misc")",
    "-I$(Join-Path $DosboxRoot "src\shell")",
    "-isystem$ZigInclude",
    "-isystem$SysrootInclude",
    "-isystem$(Join-Path $SysrootInclude "SDL")"
)

$CppIncludeArgs = @(
    "-isystem$CppInclude",
    "-isystem$CppTargetInclude"
) + $IncludeArgs

$CommonArgs = @(
    "-target", "mipsel-linux-gnu",
    "-march=mips32r2",
    "--sysroot=$Sysroot",
    "-D_GNU_SOURCE",
    "-DHAVE_CONFIG_H",
    "-USDL_VIDEO_DRIVER_X11",
    "-O2",
    "-fPIC",
    "-fno-sanitize=undefined",
    "-fno-strict-aliasing",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-missing-field-initializers",
    "-Wno-unused-variable",
    "-Wno-sign-compare",
    "-Wno-deprecated-declarations",
    "-Wno-null-pointer-subtraction"
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

function Compile-Cpp {
    param([string]$Source, [string[]]$ExtraArgs = @())
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig c++ @CommonArgs @CppIncludeArgs @ExtraArgs "-nostdinc++" "-std=gnu++03" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

$Sources = @(
    "src\dosbox.cpp",
    "src\cpu\callback.cpp",
    "src\cpu\cpu.cpp",
    "src\cpu\flags.cpp",
    "src\cpu\modrm.cpp",
    "src\cpu\paging.cpp",
    "src\cpu\core_full.cpp",
    "src\cpu\core_normal.cpp",
    "src\cpu\core_simple.cpp",
    "src\cpu\core_prefetch.cpp",
    "src\cpu\core_dyn_x86.cpp",
    "src\cpu\core_dynrec.cpp",
    "src\debug\debug.cpp",
    "src\debug\debug_gui.cpp",
    "src\debug\debug_disasm.cpp",
    "src\debug\debug_win32.cpp",
    "src\dos\dos.cpp",
    "src\dos\dos_devices.cpp",
    "src\dos\dos_execute.cpp",
    "src\dos\dos_files.cpp",
    "src\dos\dos_ioctl.cpp",
    "src\dos\dos_memory.cpp",
    "src\dos\dos_misc.cpp",
    "src\dos\dos_classes.cpp",
    "src\dos\dos_programs.cpp",
    "src\dos\dos_tables.cpp",
    "src\dos\drives.cpp",
    "src\dos\drive_virtual.cpp",
    "src\dos\drive_local.cpp",
    "src\dos\drive_cache.cpp",
    "src\dos\drive_fat.cpp",
    "src\dos\drive_iso.cpp",
    "src\dos\dos_mscdex.cpp",
    "src\dos\dos_keyboard_layout.cpp",
    "src\dos\cdrom.cpp",
    "src\dos\cdrom_ioctl_win32.cpp",
    "src\dos\cdrom_aspi_win32.cpp",
    "src\dos\cdrom_ioctl_linux.cpp",
    "src\dos\cdrom_image.cpp",
    "src\dos\cdrom_ioctl_os2.cpp",
    "src\fpu\fpu.cpp",
    "src\gui\sdlmain.cpp",
    "src\gui\sdl_mapper.cpp",
    "src\gui\render.cpp",
    "src\gui\render_scalers.cpp",
    "src\gui\midi.cpp",
    "src\gui\sdl_gui.cpp",
    "src\hardware\adlib.cpp",
    "src\hardware\dma.cpp",
    "src\hardware\gameblaster.cpp",
    "src\hardware\hardware.cpp",
    "src\hardware\iohandler.cpp",
    "src\hardware\joystick.cpp",
    "src\hardware\keyboard.cpp",
    "src\hardware\memory.cpp",
    "src\hardware\mixer.cpp",
    "src\hardware\pcspeaker.cpp",
    "src\hardware\pic.cpp",
    "src\hardware\sblaster.cpp",
    "src\hardware\tandy_sound.cpp",
    "src\hardware\timer.cpp",
    "src\hardware\vga.cpp",
    "src\hardware\vga_attr.cpp",
    "src\hardware\vga_crtc.cpp",
    "src\hardware\vga_dac.cpp",
    "src\hardware\vga_draw.cpp",
    "src\hardware\vga_gfx.cpp",
    "src\hardware\vga_other.cpp",
    "src\hardware\vga_memory.cpp",
    "src\hardware\vga_misc.cpp",
    "src\hardware\vga_seq.cpp",
    "src\hardware\vga_xga.cpp",
    "src\hardware\vga_s3.cpp",
    "src\hardware\vga_tseng.cpp",
    "src\hardware\vga_paradise.cpp",
    "src\hardware\cmos.cpp",
    "src\hardware\disney.cpp",
    "src\hardware\gus.cpp",
    "src\hardware\mpu401.cpp",
    "src\hardware\ipx.cpp",
    "src\hardware\ipxserver.cpp",
    "src\hardware\dbopl.cpp",
    "src\hardware\mame\saa1099.cpp",
    "src\hardware\mame\sn76496.cpp",
    "src\hardware\serialport\directserial.cpp",
    "src\hardware\serialport\libserial.cpp",
    "src\hardware\serialport\serialdummy.cpp",
    "src\hardware\serialport\serialport.cpp",
    "src\hardware\serialport\softmodem.cpp",
    "src\hardware\serialport\misc_util.cpp",
    "src\hardware\serialport\nullmodem.cpp",
    "src\ints\mouse.cpp",
    "src\ints\xms.cpp",
    "src\ints\ems.cpp",
    "src\ints\int10.cpp",
    "src\ints\int10_char.cpp",
    "src\ints\int10_memory.cpp",
    "src\ints\int10_misc.cpp",
    "src\ints\int10_modes.cpp",
    "src\ints\int10_vesa.cpp",
    "src\ints\int10_pal.cpp",
    "src\ints\int10_put_pixel.cpp",
    "src\ints\int10_video_state.cpp",
    "src\ints\int10_vptable.cpp",
    "src\ints\bios.cpp",
    "src\ints\bios_disk.cpp",
    "src\ints\bios_keyboard.cpp",
    "src\libs\gui_tk\gui_tk.cpp",
    "src\misc\cross.cpp",
    "src\misc\messages.cpp",
    "src\misc\programs.cpp",
    "src\misc\setup.cpp",
    "src\misc\support.cpp",
    "src\shell\shell.cpp",
    "src\shell\shell_batch.cpp",
    "src\shell\shell_cmds.cpp",
    "src\shell\shell_misc.cpp"
)

foreach ($Rel in $Sources) {
    if ($Rel -eq "src\gui\sdlmain.cpp") {
        Compile-Cpp (Join-Path $DosboxRoot $Rel) @("-Dmain=dosbox_main")
    } else {
        Compile-Cpp (Join-Path $DosboxRoot $Rel)
    }
}

Compile-Cpp (Join-Path $PortRoot "r36sx_main.cpp")

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
    "-L$TargetCppLib" `
    -o $Out `
    $Crt1 `
    $Crti `
    $CrtBegin `
    @Objects `
    $SdlLib `
    "-lpthread" `
    "-ldl" `
    "-lm" `
    "-lstdc++" `
    "-lgcc_s" `
    "-lc" `
    $CrtEnd `
    $Crtn }

if ($TryStrip) {
    $StrippedOut = Join-Path $ObjDir "dosbox_r36sx.stripped"
    & $Zig objcopy "--strip-all" $Out $StrippedOut
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force $StrippedOut $Out
    } else {
        Write-Warning "zig objcopy --strip-all failed; keeping unstripped dosbox_r36sx"
        if (Test-Path $StrippedOut) {
            Remove-Item -Force $StrippedOut
        }
        $global:LASTEXITCODE = 0
    }
}

Write-Host "Built $Out"
