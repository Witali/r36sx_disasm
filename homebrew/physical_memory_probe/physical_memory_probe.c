/* HC16xx DDR-controller readout. No firmware, RAM-probe or MMIO writes. */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static FILE *logfile;

static void report(const char *format, ...)
{
    va_list args, copy;
    va_start(args, format);
    va_copy(copy, args);
    if (logfile) {
        vfprintf(logfile, format, copy);
        fflush(logfile);
    }
    va_end(copy);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

static int decode_ddr(uint32_t value)
{
    unsigned code, mib, type;
    report("DDR controller register: 0x%08" PRIx32 "\n", value);
    if (value == UINT32_MAX) {
        report("INVALID: all-one register read; capacity is unknown.\n");
        return 1;
    }
    /* Stock Stage 3 meminfo @ 0x89dabf90: size=16<<(reg&7), DDR=2+bit23. */
    code = value & 7u;
    mib = 16u << code;
    type = 2u + ((value >> 23) & 1u);
    report("DDR type: DDR%u\n", type);
    report("TOTAL DDR configured in hardware: %u MiB (%" PRIu64 " bytes)\n",
           mib, (uint64_t)mib * 1024u * 1024u);
    report("Source: DDR controller size code %u, decoded like stock meminfo.\n", code);
    report("This covers the controller's whole DDR configuration, including AVP.\n");
    report("It is independent of Linux MemTotal and the DTB Linux reservation.\n");
    report("It does not independently test every installed RAM cell.\n");
    return 0;
}

static void capture(const char *path)
{
    FILE *file = fopen(path, "rb");
    char buffer[513];
    size_t size, total = 0;
    report("\n--- %s (context only) ---\n", path);
    if (!file) {
        report("Unavailable: %s\n", strerror(errno));
        return;
    }
    while (total < 8192 && (size = fread(buffer, 1, sizeof(buffer)-1, file)) > 0) {
        buffer[size] = 0;
        report("%s", buffer);
        total += size;
    }
    report("\n");
    fclose(file);
}

#if defined(__mips__)
static int is_hc16xx(void)
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    char buffer[2048];
    size_t size;
    if (!file) return 0;
    size = fread(buffer, 1, sizeof(buffer)-1, file);
    fclose(file);
    buffer[size] = 0;
    return strstr(buffer, "Hichip") &&
           (strstr(buffer, "HC16") || strstr(buffer, "hc16"));
}
#endif

static int hardware_readout(void)
{
#if defined(__mips__)
    const off_t physical_base = 0x18800000;
    long page_size;
    size_t length;
    int fd;
    void *mapping;
    volatile const uint32_t *registers;
    uint32_t chip, first, second;
    if (!is_hc16xx()) {
        report("UNKNOWN: CPU info does not identify Hichip HC16xx; no MMIO read.\n");
        return 1;
    }
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0 || (uint64_t)physical_base % (unsigned long)page_size) {
        report("UNKNOWN: unsupported mapping alignment.\n");
        return 1;
    }
    length = (0x1004u + (size_t)page_size - 1) / (size_t)page_size * (size_t)page_size;
    fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (fd < 0) {
        report("UNKNOWN: cannot open /dev/mem read-only: %s\n", strerror(errno));
        return 1;
    }
    mapping = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, physical_base);
    if (mapping == MAP_FAILED) {
        report("UNKNOWN: read-only MMIO mapping failed: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    registers = (volatile const uint32_t *)mapping;
    chip = registers[0];
    report("Chip register 0x18800000: 0x%08" PRIx32 "\n", chip);
    if ((chip >> 24) != 0x16u) {
        report("UNKNOWN: hardware chip ID is outside the HC16xx family.\n");
        munmap(mapping, length);
        close(fd);
        return 1;
    }
    first = registers[0x1000/4];
    second = registers[0x1000/4];
    munmap(mapping, length);
    close(fd);
    report("Physical DDR register address: 0x18801000\n");
    if (first != second) {
        report("UNKNOWN: DDR reads were unstable: 0x%08" PRIx32 " / 0x%08" PRIx32 "\n",
               first, second);
        return 1;
    }
    return decode_ddr(first);
#else
    report("UNKNOWN: this host build is not MIPS; no MMIO access attempted.\n");
    return 1;
#endif
}

static void isolated_readout(void)
{
    pid_t child;
    int status;
    fflush(NULL);
    child = fork();
    if (child < 0) {
        report("UNKNOWN: cannot start isolated readout: %s\n", strerror(errno));
        return;
    }
    if (child == 0) {
        struct rlimit limit = {0, 0};
        setrlimit(RLIMIT_CORE, &limit);
        alarm(3);
        _exit(hardware_readout());
    }
    while (waitpid(child, &status, 0) < 0) {
        if (errno != EINTR) {
            report("UNKNOWN: waitpid failed: %s\n", strerror(errno));
            return;
        }
    }
    if (WIFSIGNALED(status))
        report("UNKNOWN: isolated register read stopped by signal %d.\n", WTERMSIG(status));
    else if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        report("Hardware capacity readout unavailable; no Linux/DTB value substituted.\n");
}

static int open_report(const char *argv0, char *output, size_t capacity)
{
    char executable[PATH_MAX], directory[PATH_MAX];
    char *slash;
    ssize_t size = readlink("/proc/self/exe", executable, sizeof(executable)-1);
    int fd, count;
    if (size < 0) {
        if (snprintf(executable, sizeof(executable), "%s", argv0) >= (int)sizeof(executable))
            return -1;
    } else executable[size] = 0;
    slash = strrchr(executable, '/');
    if (slash) *slash = 0; else strcpy(executable, ".");
    count = snprintf(directory, sizeof(directory), "%s/diagnostics", executable);
    if (count < 0 || count >= (int)sizeof(directory)) { errno = ENAMETOOLONG; return -1; }
    if (mkdir(directory, 0755) && errno != EEXIST) return -1;
    count = snprintf(output, capacity, "%s/physical_memory_XXXXXX.txt", directory);
    if (count < 0 || (size_t)count >= capacity) { errno = ENAMETOOLONG; return -1; }
    fd = mkstemps(output, 4);
    if (fd < 0) return -1;
    logfile = fdopen(fd, "w");
    if (!logfile) { close(fd); return -1; }
    return 0;
}

int main(int argc, char **argv)
{
    char output[PATH_MAX];
    signal(SIGPIPE, SIG_IGN);
    if (argc == 3 && !strcmp(argv[1], "--decode-register")) {
        char *end;
        unsigned long long value;
        errno = 0;
        value = strtoull(argv[2], &end, 0);
        if (errno || end == argv[2] || *end || argv[2][0] == '-' || value > UINT32_MAX) {
            fprintf(stderr, "Expected an unsigned 32-bit register value.\n");
            return 2;
        }
        report("OFFLINE DECODE ONLY: supplied value, not a hardware measurement.\n");
        return decode_ddr((uint32_t)value);
    }
    if (argc > 1 && !(argc == 2 && !strcmp(argv[1], "--no-hardware"))) {
        fprintf(stderr, "Usage: %s [--no-hardware | --decode-register VALUE]\n", argv[0]);
        return 2;
    }
    if (open_report(argv[0], output, sizeof(output))) {
        perror("Cannot create diagnostic report beside executable");
        return 1;
    }
    report("R36SX physical memory probe v1\nReport: %s\n", output);
    report("Reads the total DDR configuration from the hardware controller.\n");
    report("Only the stock-used chip-ID and DDR registers are read; no MMIO writes.\n");
    report("No RAM alias writes, allocation stress, firmware changes or MTD writes.\n\n");
    if (argc == 2) report("Hardware readout explicitly skipped; physical capacity UNKNOWN.\n");
    else isolated_readout();
    report("\n--- Linux reservation, separate from total DDR ---\n");
    capture("/proc/version");
    capture("/proc/meminfo");
    report("\nDone. Give this report back for interpretation: %s\n", output);
    {
        int failed = ferror(logfile);
        if (fclose(logfile)) failed = 1;
        if (failed) {
        fprintf(stderr, "Diagnostic report write failed.\n");
        return 1;
        }
    }
    return 0;
}
