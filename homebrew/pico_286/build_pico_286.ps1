param(
    [switch]$TryStrip
)

$ErrorActionPreference = "Stop"

$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$PicoRoot = Join-Path $Root "internet_sources\pico-286"
$Zig = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\zig.exe"
$ZigInclude = Join-Path $Root "tools\zig-x86_64-windows-0.16.0\lib\include"
$Sysroot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\mipsel-buildroot-linux-gnu\sysroot"
$SysrootInclude = Join-Path $Sysroot "usr\include"
$SysrootLib = Join-Path $Sysroot "lib"
$SysrootUsrLib = Join-Path $Sysroot "usr\lib"
$ToolchainRoot = Join-Path $Root "tools\mipsel-buildroot-linux-gnu_sdk-buildroot\opt\ext-toolchain"
$CppInclude = Join-Path $ToolchainRoot "mips-mti-linux-gnu\include\c++\6.3.0"
$CppTargetInclude = Join-Path $CppInclude "mips-mti-linux-gnu"
$GccLib = Join-Path $ToolchainRoot "lib\gcc\mips-mti-linux-gnu\6.3.0\mipsel-r2-hard\lib"
$Out = Join-Path $PSScriptRoot "pico_286"
$ObjDir = Join-Path $PSScriptRoot "obj"
$Crt1 = Join-Path $Sysroot "usr\lib\crt1.o"
$Crti = Join-Path $Sysroot "usr\lib\crti.o"
$Crtn = Join-Path $Sysroot "usr\lib\crtn.o"
$CrtBegin = Join-Path $GccLib "crtbegin.o"
$CrtEnd = Join-Path $GccLib "crtend.o"
$CompatHeader = Join-Path $PSScriptRoot "r36sx_pico286_compat.h"

if (!(Test-Path $PicoRoot)) {
    throw "Missing internet_sources\pico-286. Clone https://github.com/xrip/pico-286 first."
}

New-Item -ItemType Directory -Force $ObjDir | Out-Null
$env:ZIG_GLOBAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-global-cache")).Path
$env:ZIG_LOCAL_CACHE_DIR = (Resolve-Path (Join-Path $Root "tools\zig-cache")).Path

$IncludeArgs = @(
    "-I$PSScriptRoot",
    "-I$(Join-Path $Root "homebrew\common")",
    "-I$(Join-Path $PicoRoot "src")",
    "-I$(Join-Path $PicoRoot "src\emulator")",
    "-I$(Join-Path $PicoRoot "src\emu8950")",
    "-I$(Join-Path $PicoRoot "src\printf")",
    "-I$(Join-Path $PicoRoot "findfirst")",
    "-isystem$ZigInclude",
    "-isystem$SysrootInclude"
)
$CppIncludeArgs = $IncludeArgs + @(
    "-isystem$CppInclude",
    "-isystem$CppTargetInclude"
)
$CommonArgs = @(
    "-target", "mipsel-linux-gnu",
    "-march=mips32r2",
    "--sysroot=$Sysroot",
    "-DPICO_RP2040=0",
    "-DPICO_RP2350=0",
    "-DDEBUG=1",
    "-DUSE_EMU8950_OPL",
    "-DEMU8950_SLOT_RENDER=1",
    "-DEMU8950_ASM=0",
    "-DEMU8950_NO_RATECONV=1",
    "-DEMU8950_NO_WAVE_TABLE_MAP=1",
    "-DEMU8950_NO_TLL=1",
    "-DEMU8950_NO_FLOAT=1",
    "-DEMU8950_NO_TIMER=1",
    "-DEMU8950_NO_TEST_FLAG=1",
    "-DEMU8950_SIMPLER_NOISE=1",
    "-DEMU8950_SHORT_NOISE_UPDATE_CHECK=1",
    "-DEMU8950_LINEAR_SKIP=1",
    "-DEMU8950_LINEAR_END_OF_NOTE_OPTIMIZATION",
    "-DEMU8950_NO_PERCUSSION_MODE=1",
    "-DEMU8950_LINEAR=1",
    "-include",
    $CompatHeader,
    "-O2",
    "-fPIC",
    "-fms-extensions",
    "-fno-sanitize=undefined",
    "-fno-strict-aliasing",
    "-fno-builtin-memset",
    "-fno-builtin-memcpy",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-Wno-unused-function",
    "-Wno-missing-field-initializers"
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

function Compile-C {
    param([string]$Source)
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig cc @CommonArgs @IncludeArgs "-std=c17" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

function Compile-Cpp {
    param([string]$Source)
    $Name = [IO.Path]::GetFileNameWithoutExtension($Source)
    $Hash = [Math]::Abs($Source.GetHashCode())
    $Obj = Join-Path $ObjDir "$Name-$Hash.o"
    Invoke-Checked { & $Zig c++ @CommonArgs @CppIncludeArgs "-std=c++20" "-fpermissive" "-fno-exceptions" "-fno-rtti" "-c" $Source "-o" $Obj }
    Add-Object $Obj
}

function New-Patched-LinuxMain {
    $Source = Join-Path $PicoRoot "src\linux-main.cpp"
    $Dest = Join-Path $ObjDir "r36sx_linux-main.cpp"
    $Text = Get-Content -Raw -Path $Source
    $Text = $Text.Replace("`r`n", "`n")
    $Text = $Text.Replace('extern "C" void _putchar(char character) {
    putchar(character);
    static int x = 0, y = 0;',
'extern "C" void _putchar(char character) {
    putchar(character);
#if DEBUG
    static char log_line[160];
    static int log_pos = 0;
    if (character == ''\n'' || character == ''\r'') {
        if (log_pos > 0) {
            log_line[log_pos] = 0;
            r36sx_pico286_debug_log("stdout: %s", log_line);
            log_pos = 0;
        }
    } else if ((unsigned char)character >= 32 &&
               log_pos < (int)sizeof(log_line) - 1) {
        log_line[log_pos++] = character;
    }
#endif
    static int x = 0, y = 0;')
    $Text = $Text.Replace('void signal_handler(int sig) {
    running = 0;
}',
'void signal_handler(int sig) {
    r36sx_pico286_debug_log("main: signal %d, stopping", sig);
    running = 0;
}

void fatal_signal_handler(int sig) {
    r36sx_pico286_debug_log("main: fatal signal %d", sig);
    signal(sig, SIG_DFL);
    raise(sig);
}')
    $Text = $Text.Replace('int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);',
'int main() {
    r36sx_pico286_debug_reset();
    r36sx_pico286_debug_log("main: start");
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSEGV, fatal_signal_handler);
    signal(SIGBUS, fatal_signal_handler);
    signal(SIGILL, fatal_signal_handler);
    signal(SIGABRT, fatal_signal_handler);')
    $Text = $Text.Replace('    if (!mfb_open("Pico-286 Emulator", 640, 480, 1)) {
        printf("Failed to open window\n");
        return -1;
    }',
'    r36sx_pico286_debug_log("main: opening MiniFB");
    if (!mfb_open("Pico-286 Emulator", 640, 480, 1)) {
        r36sx_pico286_debug_log("main: mfb_open failed");
        printf("Failed to open window\n");
        return -1;
    }
    r36sx_pico286_debug_log("main: MiniFB opened");')
    $Text = $Text.Replace('    memset(SCREEN, 0, sizeof(SCREEN));
    emu8950_opl = OPL_new(3579552, SOUND_FREQUENCY);
    blaster_reset();
    sn76489_reset();
    reset86();',
'    write86 = write86_ob;
    writew86 = writew86_ob;
    writedw86 = writedw86_ob;
    read86 = read86_ob;
    readw86 = readw86_ob;
    readdw86 = readdw86_ob;
    r36sx_pico286_debug_log("main: memory backend read=%p write=%p",
                            read86, write86);
    memset(SCREEN, 0, sizeof(SCREEN));
    r36sx_pico286_debug_log("main: screen cleared");
    emu8950_opl = OPL_new(3579552, SOUND_FREQUENCY);
    r36sx_pico286_debug_log("main: OPL_new=%p", emu8950_opl);
    blaster_reset();
    r36sx_pico286_debug_log("main: blaster_reset done");
    sn76489_reset();
    r36sx_pico286_debug_log("main: sn76489_reset done");
    reset86();
    r36sx_pico286_debug_log("main: reset86 done");')
    $Text = $Text.Replace('void *sound_thread(void *arg) {
    while (running) {',
'void *sound_thread(void *arg) {
    r36sx_pico286_debug_log("sound_thread: start arg=%p", arg);
    unsigned int sound_loop_count = 0;
    while (running) {')
    $Text = $Text.Replace('        // Send audio buffer to Linux audio system
        if (linux_audio_write(audio_buffer, AUDIO_BUFFER_LENGTH) != 0) {',
'        // Send audio buffer to Linux audio system
        if (++sound_loop_count <= 4u) {
            r36sx_pico286_debug_log("sound_thread: write #%u sample_index=%d",
                                    sound_loop_count, sample_index);
        }
        if (linux_audio_write(audio_buffer, AUDIO_BUFFER_LENGTH) != 0) {')
    $Text = $Text.Replace('    return NULL;
}

void *ticks_thread(void *arg) {
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);',
'    r36sx_pico286_debug_log("sound_thread: exit loops=%u", sound_loop_count);
    return NULL;
}

void *ticks_thread(void *arg) {
    r36sx_pico286_debug_log("ticks_thread: start arg=%p timer_period=%d",
                            arg, timer_period);
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);')
    $Text = $Text.Replace('    while (running) {
        clock_gettime(CLOCK_MONOTONIC, &current);',
'    unsigned int ticks_loop_count = 0;
    while (running) {
        clock_gettime(CLOCK_MONOTONIC, &current);
        if (++ticks_loop_count <= 4u) {
            r36sx_pico286_debug_log("ticks_thread: loop #%u timer_period=%d sample_index=%d",
                                    ticks_loop_count, timer_period, sample_index);
        }')
    $Text = $Text.Replace('    return NULL;
}

int main() {',
'    r36sx_pico286_debug_log("ticks_thread: exit loops=%u", ticks_loop_count);
    return NULL;
}

int main() {')
    $Text = $Text.Replace('    pthread_t sound_tid, ticks_tid;
    pthread_create(&sound_tid, NULL, sound_thread, NULL);
    pthread_create(&ticks_tid, NULL, ticks_thread, NULL);',
'    pthread_t sound_tid = 0, ticks_tid = 0;
    int sound_thread_rc = pthread_create(&sound_tid, NULL, sound_thread, NULL);
    int ticks_thread_rc = pthread_create(&ticks_tid, NULL, ticks_thread, NULL);
    int sound_thread_started = sound_thread_rc == 0;
    int ticks_thread_started = ticks_thread_rc == 0;
    r36sx_pico286_debug_log("main: pthread_create sound=%d ticks=%d",
                            sound_thread_rc, ticks_thread_rc);')
    $Text = $Text.Replace('    while (running) {
        exec86(32768);  // Reduced from 32768 to allow more frequent audio updates
        if (mfb_update(SCREEN, 0) < 0) {
            running = 0;
            break;
        }
    }',
'    unsigned int main_loop_count = 0;
    while (running) {
        if (main_loop_count < 8u) {
            r36sx_pico286_debug_log("main: before exec loop=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        exec86(32768);  // Reduced from 32768 to allow more frequent audio updates
        if (main_loop_count < 8u) {
            r36sx_pico286_debug_log("main: after exec loop=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        if ((++main_loop_count % 300u) == 0u) {
            r36sx_pico286_debug_log("main: alive loops=%u videomode=0x%x",
                                    main_loop_count, videomode);
        }
        if (main_loop_count <= 8u) {
            r36sx_pico286_debug_log("main: before mfb_update loop=%u",
                                    main_loop_count);
        }
        if (mfb_update(SCREEN, 0) < 0) {
            r36sx_pico286_debug_log("main: mfb_update requested stop");
            running = 0;
            break;
        }
        if (main_loop_count <= 8u) {
            r36sx_pico286_debug_log("main: after mfb_update loop=%u",
                                    main_loop_count);
        }
    }
    r36sx_pico286_debug_log("main: leaving loop loops=%u", main_loop_count);')
    $Text = $Text.Replace('    pthread_cancel(sound_tid);
    pthread_cancel(ticks_tid);
    pthread_join(sound_tid, NULL);
    pthread_join(ticks_tid, NULL);',
'    if (sound_thread_started) {
        r36sx_pico286_debug_log("main: cancel sound thread");
        pthread_cancel(sound_tid);
        pthread_join(sound_tid, NULL);
    }
    if (ticks_thread_started) {
        r36sx_pico286_debug_log("main: cancel ticks thread");
        pthread_cancel(ticks_tid);
        pthread_join(ticks_tid, NULL);
    }')
    $Text = $Text.Replace('    // Clean up audio
    linux_audio_close();

    mfb_close();
    return 0;',
'    // Clean up audio
    r36sx_pico286_debug_log("main: cleanup begin");
    linux_audio_close();

    mfb_close();
    r36sx_pico286_debug_log("main: exit 0");
    return 0;')
    $Text = $Text.Replace("VIDEORAM + ", "(uint8_t *)VIDEORAM + ")
    $Text = $Text.Replace("vram_offset + (uint8_t *)VIDEORAM", "(uint8_t *)VIDEORAM + vram_offset")
    $Text = $Text.Replace("tga_offset + (uint8_t *)VIDEORAM", "(uint8_t *)VIDEORAM + tga_offset")
    Set-Content -Path $Dest -Value $Text -NoNewline -Encoding ascii
    return $Dest
}

function New-Patched-Cpu {
    $Source = Join-Path $PicoRoot "src\emulator\cpu.c"
    $Dest = Join-Path $ObjDir "r36sx_cpu.c"
    $Text = Get-Content -Raw -Path $Source
    $Text = $Text.Replace("`r`n", "`n")
    $Text = $Text.Replace("""../fdd0.img""", """fdd0.img""")
    $Text = $Text.Replace("""../fdd1.img""", """fdd1.img""")
    $Text = $Text.Replace("""../hdd.img""", """hdd.img""")
    $Text = $Text.Replace("""../hdd2.img""", """hdd2.img""")
    $Text = $Text.Replace('            insertdisk(0, "fdd0.img");
            insertdisk(1, "fdd1.img");
            insertdisk(128, "hdd.img");
            insertdisk(129, "hdd2.img");',
'            {
                uint8_t fdd0_ok = insertdisk(0, "fdd0.img");
                uint8_t fdd1_ok = insertdisk(1, "fdd1.img");
                uint8_t hdd0_ok = insertdisk(128, "hdd.img");
                uint8_t hdd1_ok = insertdisk(129, "hdd2.img");
                r36sx_pico286_debug_log("cpu: int19 disk attach fdd0=%u fdd1=%u hdd=%u hdd2=%u",
                                        fdd0_ok, fdd1_ok, hdd0_ok, hdd1_ok);
            }')
    Set-Content -Path $Dest -Value $Text -NoNewline -Encoding ascii
    return $Dest
}

function New-Patched-Ports {
    $Source = Join-Path $PicoRoot "src\emulator\ports.c"
    $Dest = Join-Path $ObjDir "r36sx_ports.c"
    $Text = Get-Content -Raw -Path $Source
    $Text = $Text.Replace("`r`n", "`n")
    $Text = $Text.Replace('void get_sound_sample(const int16_t other_sample, int16_t *samples) {
#if HARDWARE_SOUND
    const int32_t sample = (speaker_sample() + other_sample + covox_sample + midi_sample());
    pwm_set_gpio_level(PCM_PIN, (uint16_t) ((int32_t) sample + 0x8000L) >> 4);
#else
    OPL_calc_buffer_linear(emu8950_opl, (int32_t *)samples, 1);

    samples[1] = samples[0] += (int32_t)(speaker_sample() + other_sample + covox_sample + sn76489_sample() + midi_sample());
    cms_samples(samples);
#endif

}',
'static int16_t r36sx_clamp_i16(int32_t sample)
{
    if (sample > 32767) {
        return 32767;
    }
    if (sample < -32768) {
        return -32768;
    }
    return (int16_t)sample;
}

void get_sound_sample(const int16_t other_sample, int16_t *samples) {
#if HARDWARE_SOUND
    const int32_t sample = (speaker_sample() + other_sample + covox_sample + midi_sample());
    pwm_set_gpio_level(PCM_PIN, (uint16_t) ((int32_t) sample + 0x8000L) >> 4);
#else
    int32_t opl_sample[1] = {0};
    int16_t cms_mix[2] = {0, 0};
    int32_t mixed;

    if (emu8950_opl) {
        OPL_calc_buffer_linear(emu8950_opl, opl_sample, 1);
    }

    mixed = opl_sample[0] + (int32_t)(speaker_sample() + other_sample +
            covox_sample + sn76489_sample() + midi_sample());
    cms_mix[0] = r36sx_clamp_i16(mixed);
    cms_mix[1] = cms_mix[0];
    cms_samples(cms_mix);
    samples[0] = cms_mix[0];
    samples[1] = cms_mix[1];
#endif

}')
    Set-Content -Path $Dest -Value $Text -NoNewline -Encoding ascii
    return $Dest
}

$CFiles = @()
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emulator") -Recurse -File -Filter "*.c" |
    Where-Object { $_.Name -ne "cpu.c" -and $_.Name -ne "ports.c" }
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "src\emu8950") -File -Filter "*.c"
$CFiles += Get-ChildItem -Path (Join-Path $PicoRoot "findfirst") -File -Filter "*.c"
$CFiles += Get-Item (Join-Path $PicoRoot "src\printf\printf.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_minifb.c")
$CFiles += Get-Item (Join-Path $PSScriptRoot "r36sx_linux_audio.c")
$CFiles += Get-Item (New-Patched-Cpu)
$CFiles += Get-Item (New-Patched-Ports)

foreach ($File in $CFiles) {
    Compile-C $File.FullName
}

Compile-Cpp (Join-Path $PicoRoot "src\emu8950\slot_render.cpp")
Compile-Cpp (New-Patched-LinuxMain)

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
    "-lstdc++" `
    "-lgcc_s" `
    "-lc" `
    $CrtEnd `
    $Crtn }

if ($TryStrip) {
    $StrippedOut = Join-Path $ObjDir "pico_286.stripped"
    & $Zig objcopy "--strip-all" $Out $StrippedOut
    if ($LASTEXITCODE -eq 0) {
        Move-Item -Force $StrippedOut $Out
    } else {
        Write-Warning "zig objcopy --strip-all failed; keeping unstripped pico_286"
        if (Test-Path $StrippedOut) {
            Remove-Item -Force $StrippedOut
        }
        $global:LASTEXITCODE = 0
    }
}

Write-Host "Built $Out"
