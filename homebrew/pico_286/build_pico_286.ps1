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
    $ScreenTextLogger = @'
#if DEBUG
static int r36sx_pico286_make_cell_text_line(char *line, size_t line_size,
                                             const uint32_t *row, int cols)
{
    int out = 0;
    if (line_size == 0) {
        return 0;
    }
    for (int col = 0; col < cols && out < (int)line_size - 1; col++) {
        unsigned char ch = (unsigned char)(row[col * 2] & 0xffu);
        line[out++] = (ch >= 32 && ch <= 126) ? (char)ch : ' ';
    }
    while (out > 0 && line[out - 1] == ' ') {
        out--;
    }
    line[out] = 0;
    return out;
}

static int r36sx_pico286_make_byte_text_line(char *line, size_t line_size,
                                             const uint8_t *row, int cols)
{
    int out = 0;
    if (line_size == 0) {
        return 0;
    }
    for (int col = 0; col < cols && out < (int)line_size - 1; col++) {
        unsigned char ch = row[col * 2];
        line[out++] = (ch >= 32 && ch <= 126) ? (char)ch : ' ';
    }
    while (out > 0 && line[out - 1] == ' ') {
        out--;
    }
    line[out] = 0;
    return out;
}

static uint32_t r36sx_pico286_hash_cell_text(const uint32_t *base, int cols,
                                             int rows, int stride)
{
    uint32_t hash = 2166136261u;
    for (int row = 0; row < rows; row++) {
        const uint32_t *line = base + row * stride;
        for (int col = 0; col < cols; col++) {
            hash ^= (uint8_t)(line[col * 2] & 0xffu);
            hash *= 16777619u;
        }
        hash ^= 0xffu;
        hash *= 16777619u;
    }
    return hash;
}

static uint32_t r36sx_pico286_hash_byte_text(const uint8_t *base, int cols,
                                             int rows, int stride)
{
    uint32_t hash = 2166136261u;
    for (int row = 0; row < rows; row++) {
        const uint8_t *line = base + row * stride;
        for (int col = 0; col < cols; col++) {
            hash ^= line[col * 2];
            hash *= 16777619u;
        }
        hash ^= 0xffu;
        hash *= 16777619u;
    }
    return hash;
}

static void r36sx_pico286_log_cell_text(int slot, const char *tag,
                                        const uint32_t *base, int cols,
                                        int rows, int stride, uint8_t mode,
                                        uint32_t offset)
{
    static uint32_t last_hash[4] = {0, 0, 0, 0};
    static unsigned int change_count[4] = {0, 0, 0, 0};
    char line[161];
    int emitted = 0;
    uint32_t hash = r36sx_pico286_hash_cell_text(base, cols, rows, stride);

    slot &= 3;
    if (hash == last_hash[slot]) {
        return;
    }
    last_hash[slot] = hash;
    change_count[slot]++;
    if (change_count[slot] > 64u) {
        if (change_count[slot] == 65u) {
            r36sx_pico286_debug_log("screen_text:%s further changes suppressed",
                                    tag);
        }
        return;
    }

    r36sx_pico286_debug_log(
        "screen_text:%s change=%u mode=0x%02x offset=0x%04x cols=%d rows=%d",
        tag, change_count[slot], mode, offset, cols, rows);
    for (int row = 0; row < rows; row++) {
        if (r36sx_pico286_make_cell_text_line(
                line, sizeof(line), base + row * stride, cols) > 0) {
            r36sx_pico286_debug_log("screen_text:%s[%02d]: %s", tag, row,
                                    line);
            emitted = 1;
        }
    }
    if (!emitted) {
        r36sx_pico286_debug_log("screen_text:%s blank", tag);
    }
}

static void r36sx_pico286_log_byte_text(int slot, const char *tag,
                                        const uint8_t *base, int cols,
                                        int rows, int stride, uint8_t mode,
                                        uint32_t offset)
{
    static uint32_t last_hash[4] = {0, 0, 0, 0};
    static unsigned int change_count[4] = {0, 0, 0, 0};
    char line[161];
    int emitted = 0;
    uint32_t hash = r36sx_pico286_hash_byte_text(base, cols, rows, stride);

    slot &= 3;
    if (hash == last_hash[slot]) {
        return;
    }
    last_hash[slot] = hash;
    change_count[slot]++;
    if (change_count[slot] > 64u) {
        if (change_count[slot] == 65u) {
            r36sx_pico286_debug_log("screen_text:%s further changes suppressed",
                                    tag);
        }
        return;
    }

    r36sx_pico286_debug_log(
        "screen_text:%s change=%u mode=0x%02x offset=0x%04x cols=%d rows=%d",
        tag, change_count[slot], mode, offset, cols, rows);
    for (int row = 0; row < rows; row++) {
        if (r36sx_pico286_make_byte_text_line(
                line, sizeof(line), base + row * stride, cols) > 0) {
            r36sx_pico286_debug_log("screen_text:%s[%02d]: %s", tag, row,
                                    line);
            emitted = 1;
        }
    }
    if (!emitted) {
        r36sx_pico286_debug_log("screen_text:%s blank", tag);
    }
}

static void r36sx_pico286_log_visible_text(uint8_t mode, uint32_t offset,
                                           const uint8_t *renderer_bytes)
{
    uint32_t logical_offset =
        (0x8000u + (((uint32_t)offset & 0xffffu) << 1)) & 0xffffu;

    if (mode == 0x02 || mode == 0x03 || mode == 0x20 || mode == 0x30) {
        r36sx_pico286_log_cell_text(0, "logical-b800",
                                    &VIDEORAM[logical_offset], 80, 25, 160,
                                    mode, logical_offset);
        r36sx_pico286_log_byte_text(1, "renderer-byte-view", renderer_bytes,
                                    80, 25, 160, mode, logical_offset);
    } else if (mode == 0x77 || mode == 0x78) {
        int cols = mode == 0x78 ? 40 : 80;
        r36sx_pico286_log_cell_text(2, "logical-small-text",
                                    &VIDEORAM[0x8000], cols, 25, 160, mode,
                                    0x8000);
        r36sx_pico286_log_byte_text(
            3, "renderer-byte-small-text",
            (const uint8_t *)&VIDEORAM[0] + 0x8000,
            cols, 25, 160, mode, 0x8000);
    }
}
#endif
'@
    $ScreenTextLogger = ""
    $Text = $Text.Replace('static inline void renderer() {',
        $ScreenTextLogger + "`nstatic inline void renderer() {")
    $Text = $Text.Replace('    uint8_t cols = 80;
    for (int y = 0; y < 480; y++) {',
'    uint8_t cols = 80;
    for (int y = 0; y < 480; y++) {')
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
    $Text = $Text.Replace('                case 0x02:
                case 0x03: {',
'                case 0x02:
                case 0x03:
                case 0x20:
                case 0x30: {')
    $Text = $Text.Replace("VIDEORAM + ", "(uint8_t *)VIDEORAM + ")
    $Text = $Text.Replace("vram_offset + (uint8_t *)VIDEORAM", "(uint8_t *)VIDEORAM + vram_offset")
    $Text = $Text.Replace("tga_offset + (uint8_t *)VIDEORAM", "(uint8_t *)VIDEORAM + tga_offset")
    $Text = $Text.Replace('    uint8_t *vidramptr = (uint8_t *)VIDEORAM + 0x8000 + ((vram_offset & 0xffff) << 1);',
'    uint8_t *vidramptr = (uint8_t *)(VIDEORAM + 0x8000 + ((vram_offset & 0xffff) << 1));')
    $Text = $Text.Replace('                    uint8_t *text_buffer_line = vidramptr + y_div_16 * 80;

                    for (int column = 0; column < 40; column++) {
                        uint8_t glyph_pixels = font_8x8[*text_buffer_line++ * 8 + glyph_line]; // Glyph row from font
                        uint8_t color = *text_buffer_line++; // Color attribute',
'                    uint32_t *text_buffer_line =
                            &VIDEORAM[0x8000 + ((vram_offset & 0xffff) << 1) + y_div_16 * 80];

                    for (int column = 0; column < 40; column++) {
                        uint8_t charcode = (uint8_t)(*text_buffer_line++ & 0xffu);
                        uint8_t glyph_pixels = font_8x8[charcode * 8 + glyph_line]; // Glyph row from font
                        uint8_t color = (uint8_t)(*text_buffer_line++ & 0xffu); // Color attribute')
    $Text = $Text.Replace('                    uint8_t *text_row = vidramptr + y_div_16 * 160;
                    for (uint8_t column = 0; column < 80; column++) {
                        // Access vidram and font data once per character
                        uint8_t *charcode = text_row + column * 2; // Character code
                        uint8_t glyph_row = font_8x16[*charcode * 16 + glyph_line]; // Glyph row from font
                        uint8_t color = *++charcode; // Color attribute',
'                    uint32_t *text_row =
                            &VIDEORAM[0x8000 + ((vram_offset & 0xffff) << 1) + y_div_16 * 160];
                    for (uint8_t column = 0; column < 80; column++) {
                        // Access vidram and font data once per character
                        uint8_t charcode = (uint8_t)(text_row[column * 2] & 0xffu); // Character code
                        uint8_t glyph_row = font_8x16[charcode * 16 + glyph_line]; // Glyph row from font
                        uint8_t color = (uint8_t)(text_row[column * 2 + 1] & 0xffu); // Color attribute')
    $Text = [regex]::Replace($Text,
'(?s)        else \{\n            uint8_t ydebug = y - 400;.*?        \}\n    \}\n\}\n\nextern "C" void HandleInput',
'        else {
            for (int x = 0; x < 640; x++) {
                *pixels++ = 0;
            }
        }
    }
}

extern "C" void HandleInput')
    Set-Content -Path $Dest -Value $Text -NoNewline -Encoding ascii
    return $Dest
}

function New-Patched-Cpu {
    New-Patched-DisksWin32 | Out-Null
    $Source = Join-Path $PicoRoot "src\emulator\cpu.c"
    $Dest = Join-Path $ObjDir "r36sx_cpu.c"
    $Text = Get-Content -Raw -Path $Source
    $Text = $Text.Replace("`r`n", "`n")
    $Text = $Text.Replace("""../fdd0.img""", """fdd0.img""")
    $Text = $Text.Replace("""../fdd1.img""", """fdd1.img""")
    $Text = $Text.Replace("""../hdd.img""", """hdd.img""")
    $Text = $Text.Replace("""../hdd2.img""", """hdd2.img""")
    $VideoBiosPatch = @'
#define R36SX_BIOS_TEXT_BASE 0x8000u
#define R36SX_BIOS_TEXT_PAGE_CELLS 0x1000u
#define R36SX_BIOS_MAX_TEXT_COLS 80u
#define R36SX_BIOS_MAX_TEXT_ROWS 25u

static uint8_t r36sx_bios_active_page(void)
{
    return FIRST_RAM_PAGE[0x462] & 7u;
}

static uint8_t r36sx_bios_text_cols(void)
{
    uint8_t cols = FIRST_RAM_PAGE[0x44A];
    if (videomode == 0x00 || videomode == 0x01) {
        return 40;
    }
    if (cols == 0 || cols > R36SX_BIOS_MAX_TEXT_COLS) {
        return 80;
    }
    return cols;
}

static uint8_t r36sx_bios_text_rows(void)
{
    uint8_t rows = FIRST_RAM_PAGE[0x484] + 1u;
    if (rows == 0 || rows > R36SX_BIOS_MAX_TEXT_ROWS) {
        return 25;
    }
    return rows;
}

static void r36sx_bios_set_cursor(uint8_t page, uint8_t col, uint8_t row)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    page &= 7u;
    if (col >= cols) {
        col = cols - 1u;
    }
    if (row >= rows) {
        row = rows - 1u;
    }

    FIRST_RAM_PAGE[0x450 + page * 2u] = col;
    FIRST_RAM_PAGE[0x451 + page * 2u] = row;
    if (page == r36sx_bios_active_page()) {
        CURSOR_X = col;
        CURSOR_Y = row;
    }
}

static uint32_t r36sx_bios_text_index(uint8_t page, uint8_t col, uint8_t row)
{
    const uint32_t stride = (uint32_t)r36sx_bios_text_cols() * 2u;
    return R36SX_BIOS_TEXT_BASE +
           (uint32_t)(page & 7u) * R36SX_BIOS_TEXT_PAGE_CELLS +
           (uint32_t)row * stride +
           (uint32_t)col * 2u;
}

static uint8_t r36sx_bios_read_text_attr(uint8_t page, uint8_t col,
                                         uint8_t row)
{
    uint32_t index = r36sx_bios_text_index(page, col, row) + 1u;
    uint8_t attr = 0x07;
    if (index < VIDEORAM_SIZE) {
        attr = (uint8_t)(VIDEORAM[index] & 0xffu);
    }
    return attr ? attr : 0x07;
}

static void r36sx_bios_write_text_cell(uint8_t page, uint8_t col, uint8_t row,
                                       uint8_t ch, uint8_t attr)
{
    uint32_t index = r36sx_bios_text_index(page, col, row);
    if (index + 1u >= VIDEORAM_SIZE) {
        return;
    }
    VIDEORAM[index] = ch;
    VIDEORAM[index + 1u] = attr;
}

static void r36sx_bios_clear_text_window(uint8_t page, uint8_t top,
                                         uint8_t left, uint8_t bottom,
                                         uint8_t right, uint8_t attr)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    if (top >= rows) top = rows - 1u;
    if (bottom >= rows) bottom = rows - 1u;
    if (left >= cols) left = cols - 1u;
    if (right >= cols) right = cols - 1u;
    if (bottom < top || right < left) {
        return;
    }

    for (uint8_t row = top; row <= bottom; row++) {
        for (uint8_t col = left; col <= right; col++) {
            r36sx_bios_write_text_cell(page, col, row, ' ', attr);
        }
    }
}

static void r36sx_bios_scroll_text_window(uint8_t page, uint8_t top,
                                          uint8_t left, uint8_t bottom,
                                          uint8_t right, uint8_t lines,
                                          uint8_t attr, int direction)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();

    if (top >= rows) top = rows - 1u;
    if (bottom >= rows) bottom = rows - 1u;
    if (left >= cols) left = cols - 1u;
    if (right >= cols) right = cols - 1u;
    if (bottom < top || right < left) {
        return;
    }

    uint8_t height = (uint8_t)(bottom - top + 1u);
    if (lines == 0 || lines >= height) {
        r36sx_bios_clear_text_window(page, top, left, bottom, right, attr);
        return;
    }

    if (direction > 0) {
        for (uint8_t row = top; row <= (uint8_t)(bottom - lines); row++) {
            for (uint8_t col = left; col <= right; col++) {
                uint32_t src = r36sx_bios_text_index(page, col, row + lines);
                uint32_t dst = r36sx_bios_text_index(page, col, row);
                if (src + 1u < VIDEORAM_SIZE && dst + 1u < VIDEORAM_SIZE) {
                    VIDEORAM[dst] = VIDEORAM[src];
                    VIDEORAM[dst + 1u] = VIDEORAM[src + 1u];
                }
            }
        }
        r36sx_bios_clear_text_window(page, (uint8_t)(bottom - lines + 1u),
                                     left, bottom, right, attr);
    } else {
        for (int row = bottom; row >= (int)top + lines; row--) {
            for (uint8_t col = left; col <= right; col++) {
                uint32_t src = r36sx_bios_text_index(page, col,
                                                     (uint8_t)(row - lines));
                uint32_t dst = r36sx_bios_text_index(page, col, (uint8_t)row);
                if (src + 1u < VIDEORAM_SIZE && dst + 1u < VIDEORAM_SIZE) {
                    VIDEORAM[dst] = VIDEORAM[src];
                    VIDEORAM[dst + 1u] = VIDEORAM[src + 1u];
                }
            }
        }
        r36sx_bios_clear_text_window(page, top, left, (uint8_t)(top + lines - 1u),
                                     right, attr);
    }
}

static void r36sx_bios_teletype(uint8_t page, uint8_t ch, uint8_t attr)
{
    const uint8_t cols = r36sx_bios_text_cols();
    const uint8_t rows = r36sx_bios_text_rows();
    uint8_t col = FIRST_RAM_PAGE[0x450 + (page & 7u) * 2u];
    uint8_t row = FIRST_RAM_PAGE[0x451 + (page & 7u) * 2u];

    if (col >= cols) col = 0;
    if (row >= rows) row = rows - 1u;
    if (attr == 0) {
        attr = r36sx_bios_read_text_attr(page, col, row);
    }

    if (ch == '\a') {
        return;
    } else if (ch == '\b') {
        if (col > 0) {
            col--;
        }
    } else if (ch == '\r') {
        col = 0;
    } else if (ch == '\n') {
        row++;
    } else if (ch == '\t') {
        do {
            r36sx_bios_teletype(page, ' ', attr);
            col = FIRST_RAM_PAGE[0x450 + (page & 7u) * 2u];
        } while ((col & 7u) != 0);
        return;
    } else {
        r36sx_bios_write_text_cell(page, col, row, ch, attr);
        col++;
        if (col >= cols) {
            col = 0;
            row++;
        }
    }

    if (row >= rows) {
        r36sx_bios_scroll_text_window(page, 0, 0, rows - 1u, cols - 1u, 1,
                                      attr, 1);
        row = rows - 1u;
    }
    r36sx_bios_set_cursor(page, col, row);
}

'@
    $Text = $Text.Replace('void intcall86(uint8_t intnum) {',
        $VideoBiosPatch + 'void intcall86(uint8_t intnum) {')
    $Text = $Text.Replace('                case 0x09:
                case 0x0a:
                    if (videomode >= 8 && videomode <= 0x13) {
                        // TODO: char attr?
                        tga_draw_char(CPU_AL, CURSOR_X, CURSOR_Y, 9);
                        printf("%c", CPU_AL);
                        return;
                    }
                    break;
                case 0x0f:
                    if (videomode < 8) break;
                    CPU_AL = videomode;
                    CPU_AH = 80;
                    CPU_BH = 0;
                    return;
                case 0x00:',
'                case 0x01:
                    cursor_start = CPU_CH;
                    cursor_end = CPU_CL;
                    FIRST_RAM_PAGE[0x460] = CPU_CH;
                    FIRST_RAM_PAGE[0x461] = CPU_CL;
                    return;
                case 0x02:
                    r36sx_bios_set_cursor(CPU_BH, CPU_DL, CPU_DH);
                    return;
                case 0x03:
                    CPU_CH = cursor_start;
                    CPU_CL = cursor_end;
                    CPU_DH = FIRST_RAM_PAGE[0x451 + (CPU_BH & 7u) * 2u];
                    CPU_DL = FIRST_RAM_PAGE[0x450 + (CPU_BH & 7u) * 2u];
                    return;
                case 0x06:
                    r36sx_bios_scroll_text_window(r36sx_bios_active_page(),
                                                  CPU_CH, CPU_CL, CPU_DH,
                                                  CPU_DL, CPU_AL, CPU_BH, 1);
                    return;
                case 0x07:
                    r36sx_bios_scroll_text_window(r36sx_bios_active_page(),
                                                  CPU_CH, CPU_CL, CPU_DH,
                                                  CPU_DL, CPU_AL, CPU_BH, -1);
                    return;
                case 0x08: {
                    uint8_t page = CPU_BH & 7u;
                    uint8_t col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint32_t index = r36sx_bios_text_index(page, col, row);
                    CPU_AL = index < VIDEORAM_SIZE ? (uint8_t)(VIDEORAM[index] & 0xffu) : '' '';
                    CPU_AH = index + 1u < VIDEORAM_SIZE ? (uint8_t)(VIDEORAM[index + 1u] & 0xffu) : 0x07;
                    return;
                }
                case 0x09:
                case 0x0a: {
                    uint8_t page = CPU_BH & 7u;
                    uint8_t col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint8_t attr = CPU_AH == 0x09 ? CPU_BL :
                                   r36sx_bios_read_text_attr(page, col, row);
                    uint16_t count = CPU_CX ? CPU_CX : 1u;
                    for (uint16_t i = 0; i < count; i++) {
                        uint16_t pos = (uint16_t)col + i;
                        uint8_t write_col = (uint8_t)(pos % r36sx_bios_text_cols());
                        uint8_t write_row = (uint8_t)(row + pos / r36sx_bios_text_cols());
                        if (write_row < r36sx_bios_text_rows()) {
                            r36sx_bios_write_text_cell(page, write_col,
                                                       write_row, CPU_AL, attr);
                        }
                    }
                    if (videomode >= 8 && videomode <= 0x13) {
                        tga_draw_char(CPU_AL, CURSOR_X, CURSOR_Y, attr & 0x0f);
                    }
                    return;
                }
                case 0x0e:
                    if (videomode >= 8 && videomode <= 0x13) {
                        tga_draw_char(CPU_AL, CURSOR_X, CURSOR_Y, CPU_BL ? CPU_BL : 9);
                    }
                    r36sx_bios_teletype(CPU_BH, CPU_AL, CPU_BL);
                    return;
                case 0x0f:
                    CPU_AL = (uint8_t)videomode;
                    CPU_AH = r36sx_bios_text_cols();
                    CPU_BH = r36sx_bios_active_page();
                    return;
                case 0x13: {
                    uint8_t mode = CPU_AL;
                    uint8_t page = CPU_BH & 7u;
                    uint8_t attr = CPU_BL;
                    uint8_t old_col = FIRST_RAM_PAGE[0x450 + page * 2u];
                    uint8_t old_row = FIRST_RAM_PAGE[0x451 + page * 2u];
                    uint32_t memloc = CPU_ES * 16u + CPU_BP;

                    r36sx_bios_set_cursor(page, CPU_DL, CPU_DH);
                    for (uint16_t i = 0; i < CPU_CX; i++) {
                        uint8_t ch = read86(memloc++);
                        if (mode & 0x02u) {
                            attr = read86(memloc++);
                        }
                        r36sx_bios_teletype(page, ch, attr);
                    }
                    if ((mode & 0x01u) == 0) {
                        r36sx_bios_set_cursor(page, old_col, old_row);
                    }
                    return;
                }
                case 0x00:')
    $Text = $Text.Replace('FIRST_RAM_PAGE[0x44A] = videomode <= 2 || (videomode >= 0x8 && videomode <= 0xa) ? 40 : 80;',
        'FIRST_RAM_PAGE[0x44A] = videomode <= 1 || (videomode >= 0x8 && videomode <= 0xa) ? 40 : 80;')
    $Text = $Text.Replace('                    tga_offset = 0x8000;
                    break;',
'                    tga_offset = 0x8000;
                    FIRST_RAM_PAGE[0x462] = 0;
                    r36sx_bios_set_cursor(0, 0, 0);
                    break;')
    $Text = $Text.Replace('
                    break;
                }
                case 0x10:',
'
                    FIRST_RAM_PAGE[0x462] = CPU_AL & 7u;
                    r36sx_bios_set_cursor(FIRST_RAM_PAGE[0x462],
                                          FIRST_RAM_PAGE[0x450 + FIRST_RAM_PAGE[0x462] * 2u],
                                          FIRST_RAM_PAGE[0x451 + FIRST_RAM_PAGE[0x462] * 2u]);
                    return;
                }
                case 0x10:')
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

function New-Patched-DisksWin32 {
    $Source = Join-Path $PicoRoot "src\emulator\disks-win32.c.inl"
    $Dest = Join-Path $ObjDir "disks-win32.c.inl"
    $Text = Get-Content -Raw -Path $Source
    $Text = $Text.Replace("`r`n", "`n")
    $Text = $Text.Replace('#include "emulator.h"',
'#include "emulator.h"

extern void r36sx_pico286_disk_activity(void);')
    $Text = $Text.Replace('        // Read the sector into buffer
        if (fread(&sectorbuffer[0], 512, 1, disk[drivenum].diskfile) != 1) {',
'        r36sx_pico286_disk_activity();

        // Read the sector into buffer
        if (fread(&sectorbuffer[0], 512, 1, disk[drivenum].diskfile) != 1) {')
    $Text = $Text.Replace('        // Write the buffer to the file
        fwrite(sectorbuffer, 512, 1, disk[drivenum].diskfile);',
'        r36sx_pico286_disk_activity();

        // Write the buffer to the file
        fwrite(sectorbuffer, 512, 1, disk[drivenum].diskfile);')
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
