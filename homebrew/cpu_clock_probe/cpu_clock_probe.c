/* Read-only R36SX clock diagnostics. No clock/PLL/control-register writes. */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static FILE *logfile;

static void report(const char *fmt, ...)
{
    va_list a, b;
    va_start(a, fmt);
    va_copy(b, a);
    if (logfile) { vfprintf(logfile, fmt, b); fflush(logfile); }
    vprintf(fmt, a);
    fflush(stdout);
    va_end(b);
    va_end(a);
}

static void capture(const char *path)
{
    char buf[1025];
    size_t n, total = 0;
    FILE *f = fopen(path, "rb");
    report("\n--- %s ---\n", path);
    if (!f) { report("Unavailable: %s\n", strerror(errno)); return; }
    while ((n = fread(buf, 1, sizeof(buf)-1, f)) > 0) {
        buf[n] = 0;
        report("%s", buf);
        total += n;
        if (total >= 32768) { report("\n[output limited to 32 KiB]\n"); break; }
    }
    report("\n");
    fclose(f);
}

static int is_hc16xx(void)
{
    char buf[2048];
    FILE *f = fopen("/proc/cpuinfo", "r");
    size_t n;
    if (!f) return 0;
    n = fread(buf, 1, sizeof(buf)-1, f);
    buf[n] = 0;
    fclose(f);
    return strstr(buf, "Hichip") && (strstr(buf, "HC16") || strstr(buf, "hc16"));
}

static void clock_registers(void)
{
    /* DT cpu_clk: info=0x18800070, control=0x18800074, shift=8, width=3.
       HC16xx vendor SDK mcpu_clksel2clk() provides the fixed-source mapping. */
    static const unsigned mhz[] = {594, 396, 297, 198, 900, 1188, 24, 0};
    int fd;
    void *map;
    volatile const uint32_t *regs;
    uint32_t info, ctrl;
    unsigned status_sel, ctrl_sel;
    report("\n--- HC16xx clock-register snapshot (read only) ---\n");
    if (!is_hc16xx()) { report("Skipped: this is not an identified Hichip HC16xx device.\n"); return; }
    fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (fd < 0) { report("/dev/mem unavailable: %s\n", strerror(errno)); return; }
    map = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, (off_t)0x18800000);
    if (map == MAP_FAILED) { report("Read-only mapping failed: %s\n", strerror(errno)); close(fd); return; }
    regs = (volatile const uint32_t *)map;
    info = regs[0x70/4];
    ctrl = regs[0x74/4];
    status_sel = (info >> 8) & 7;
    ctrl_sel = (ctrl >> 8) & 7;
    report("0x18800070 info    = 0x%08x; selector=%u\n", info, status_sel);
    report("0x18800074 control = 0x%08x; selector=%u\n", ctrl, ctrl_sel);
    if (mhz[status_sel])
        report("Status selector maps to %u MHz in the vendor fixed-source table.\n", mhz[status_sel]);
    else
        report("Status selects a programmable PLL; no fixed MHz value inferred.\n");
    if (status_sel != ctrl_sel)
        report("Selectors differ; retain raw values for analysis.\n");
    report("This is register interpretation, not an independent frequency measurement.\n");
    munmap(map, 4096);
    close(fd);
}

#if defined(__mips__)
static inline uint32_t counter(void)
{
    uint32_t v;
    __asm__ volatile(".set push\n.set mips32r2\nrdhwr %0,$2\n.set pop" : "=r"(v) : : "memory");
    return v;
}
static inline uint32_t resolution(void)
{
    uint32_t v;
    __asm__ volatile(".set push\n.set mips32r2\nrdhwr %0,$3\n.set pop" : "=r"(v) : : "memory");
    return v;
}
#endif

#if defined(__mips__)
static double elapsed(struct timespec a, struct timespec b)
{
    return (double)(b.tv_sec-a.tv_sec) + (b.tv_nsec-a.tv_nsec)/1e9;
}
#endif

static void counter_measurement(void)
{
    report("\n--- RDHWR counter against Linux monotonic time ---\n");
#if defined(__mips__)
    struct timespec t0, t1;
    clockid_t clock_id = CLOCK_MONOTONIC_RAW;
    uint32_t res = resolution();
    int i;
    if (clock_gettime(clock_id, &t0)) clock_id = CLOCK_MONOTONIC;
    if (clock_gettime(clock_id, &t0)) { report("No usable clock: %s\n", strerror(errno)); return; }
    report("RDHWR CCRes=%u CPU cycles/count; clock=%s\n", res,
           clock_id == CLOCK_MONOTONIC_RAW ? "MONOTONIC_RAW" : "MONOTONIC");
    for (i = 0; i < 3; ++i) {
        uint32_t c0, c1, delta;
        double duration;
        unsigned spin;
        volatile unsigned keep_busy = 0;
        clock_gettime(clock_id, &t0);
        c0 = counter();
        do {
            for (spin = 0; spin < 8192; ++spin) keep_busy += spin;
            if (clock_gettime(clock_id, &t1)) return;
        } while (elapsed(t0, t1) < 0.4);
        c1 = counter();
        clock_gettime(clock_id, &t1);
        duration = elapsed(t0, t1);
        delta = c1-c0; /* One 32-bit wrap is handled by unsigned subtraction. */
        report("sample %d: seconds=%.6f counts=%u counter_MHz=%.3f", i+1,
               duration, delta, delta/duration/1e6);
        if (res >= 1 && res <= 16)
            report(" derived_CPU_MHz=%.3f\n", (delta/duration)*res/1e6);
        else report(" derived_CPU_MHz=unknown (invalid CCRes)\n");
    }
    report("Interpret together with clocksource and clock registers.\n");
    report("RDHWR may be virtualized by Linux. A timebase derived from the same\n");
    report("CPU counter is not an independent calibration of its physical clock.\n");
#else
    report("Skipped: this build is not MIPS.\n");
#endif
}

static void isolated(const char *name, void (*fn)(void))
{
    pid_t pid;
    int status = 0;
    fflush(NULL);
    pid = fork();
    if (pid == 0) {
        struct rlimit limit = {0,0};
        setrlimit(RLIMIT_CORE, &limit);
        alarm(8);
        fn();
        fflush(NULL);
        _exit(0);
    }
    if (pid < 0) { report("Cannot run %s: %s\n", name, strerror(errno)); return; }
    while (waitpid(pid, &status, 0) < 0) if (errno != EINTR) return;
    if (WIFSIGNALED(status)) report("%s unavailable/interrupted (signal %d); other evidence retained.\n", name, WTERMSIG(status));
}

int main(int argc, char **argv)
{
    char exe[1024], directory[1100], output[1200];
    ssize_t n;
    char *slash;
    int fd;
    const char *files[] = {
        "/proc/cpuinfo", "/proc/meminfo", "/sys/devices/system/cpu/online",
        "/sys/devices/system/cpu/present", "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq",
        "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq",
        "/sys/devices/system/clocksource/clocksource0/current_clocksource",
        "/sys/devices/system/clocksource/clocksource0/available_clocksource",
        "/sys/kernel/debug/clk/clk_summary", NULL
    };
    int i;
    /* A launcher can close stdout; diagnostics must still reach the SD card. */
    signal(SIGPIPE, SIG_IGN);
    n = readlink("/proc/self/exe", exe, sizeof(exe)-1);
    if (n < 0) snprintf(exe, sizeof(exe), "%s", argv[0]); else exe[n] = 0;
    slash = strrchr(exe, '/');
    if (slash) *slash = 0; else strcpy(exe, ".");
    snprintf(directory, sizeof(directory), "%s/diagnostics", exe);
    if (mkdir(directory, 0755) && errno != EEXIST) {
        fprintf(stderr, "Cannot create %s: %s\n", directory, strerror(errno)); return 1;
    }
    snprintf(output, sizeof(output), "%s/cpu_clock_report_XXXXXX.txt", directory);
    fd = mkstemps(output, 4);
    if (fd < 0 || !(logfile = fdopen(fd, "w"))) { perror("report file"); return 1; }
    report("R36SX CPU clock probe v2\nReport: %s\n", output);
    report("Read-only diagnostics; no clock, PLL, firmware or sysfs writes.\n");
    report("cpuinfo BogoMIPS is not a CPU MHz specification.\n");
    for (i = 0; files[i]; ++i) capture(files[i]);
    if (!(argc > 1 && !strcmp(argv[1], "--no-mem"))) isolated("Clock-register read", clock_registers);
    isolated("RDHWR counter test", counter_measurement);
    report("\nDone. Return this report for analysis: %s\n", output);
    fclose(logfile);
    return 0;
}
