"""Run selected production CPU helpers with observable memory/exception stubs.

This is a source-level regression probe, not a full emulator conformance test.
Run under WSL with Python 3 and GCC. The generated C uses verbatim source slices
so a probe cannot silently pass against a separately copied implementation.
"""

import argparse
from pathlib import Path
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]
PORT = ROOT / "r36sx_port"
PROBES = ("conditions", "adc_sbb8", "idiv16_overflow", "idiv16_boundaries",
          "idiv32_overflow", "idiv32_boundaries", "idiv32_dividend_assembly",
          "bit_negative16", "bit_negative32",
          "xchg_address_alias", "rep16_index_wrap")


def between(text, start, end):
    begin = text.index(start)
    return text[begin:text.index(end, begin)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cc", default="gcc")
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--case", action="append", choices=PROBES,
                        help="Run only this probe (repeatable); default runs all.")
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    common = (PORT / "r36sx_cpu.c").read_text(encoding="utf-8-sig")
    cpu86 = (PORT / "r36sx_cpu_8086.inl").read_text(encoding="utf-8-sig")
    cpu386 = (PORT / "r36sx_cpu_80386.inl").read_text(encoding="utf-8-sig")

    # Stubs record accesses; they deliberately do not add missing CPU checks.
    preamble = r"""
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#define __not_in_flash()
static uint8_t cf, pf, af, zf, sf, of;
static uint16_t CPU_AX, CPU_DX;
static uint32_t CPU_EAX, CPU_EDX;
static int divide_fault;
static uint32_t divide_fault_ip;
static void r36sx_cpu_divide_error(uint32_t ip) {
    divide_fault_ip = ip; divide_fault++;
}
static bool parity[256];
static uint8_t mode, rm, reg, df;
static bool operandSizeOverride, addressSizeOverride;
static uint32_t ea, useseg_base, last_address, source_index, dest_index;
static uint16_t CPU_ES, useseg;
static uint32_t registers[8];
#define getreg32(i) registers[i]
#define getreg16(i) ((uint16_t)registers[i])
#define putreg32(i, v) (registers[i] = (v))
#define putreg16(i, v) (registers[i] = (uint16_t)(v))
static void getea(uint8_t i) { ea = registers[i]; }
static void modregrm(void) { mode = 0; reg = 0; rm = 0; }
static uint8_t r36sx_cpu_check_segment_access(uint32_t a, uint32_t n,
                                             uint8_t w) {
    (void)a; (void)n; (void)w; return 1;
}
static uint32_t readdw86(uint32_t a) { last_address = a; return 0x2000; }
static uint16_t readw86(uint32_t a) { last_address = a; return 0x2000; }
static void writedw86(uint32_t a, uint32_t v) { (void)v; last_address = a; }
static void writew86(uint32_t a, uint16_t v) { (void)v; last_address = a; }
static uint32_t readrm32(uint8_t i) { getea(i); return readdw86(ea); }
static void writerm32(uint8_t i, uint32_t v) { getea(i); writedw86(ea, v); }
static uint32_t r36sx_src_index(void) { return source_index; }
static uint32_t r36sx_dst_index(void) { return dest_index; }
static void r36sx_set_src_index(uint32_t v) {
    source_index = addressSizeOverride ? v : (uint16_t)v;
}
static void r36sx_set_dst_index(uint32_t v) {
    dest_index = addressSizeOverride ? v : (uint16_t)v;
}
static int r36sx_rep_try_movs_ram(uint32_t n, uint32_t w, uint32_t si,
                                 uint32_t di, bool a32) {
    (void)n; (void)w; (void)si; (void)di; (void)a32; return 0;
}
static uint8_t getmem8(uint16_t s, uint32_t o) {
    (void)s; last_address = o; return 0;
}
static void putmem8(uint16_t s, uint32_t o, uint8_t v) {
    (void)s; (void)o; (void)v;
}
"""
    slices = [
        between(cpu86, "static inline void flag_szp8(", "static inline void flag_szp16("),
        between(cpu86, "static inline void flag_adc8(", "static inline void flag_adc16("),
        between(cpu86, "static inline uint8_t sbb8(", "static inline uint16_t sbb16("),
        between(cpu86, "static inline void op_idiv16(", "static __not_in_flash() void op_grp3_16("),
        between(cpu386, "static inline void op_idiv32(", "static __not_in_flash() void op_grp3_32("),
        between(cpu386, "static inline uint8_t r36sx_cpu_condition(", "static __not_in_flash() uint32_t op_grp2_32("),
        between(cpu386, "static __not_in_flash() void r36sx_cpu_exec_bit_test(", "static __not_in_flash() void r36sx_cpu_exec_double_shift("),
        between(common, "static inline void r36sx_rep_movsb(", "static inline void r36sx_rep_movsw("),
    ]
    xchg_case = between(cpu386, "        /* XCHG r/m32, r32 */", "        /* MOV r/m32, r32 */")
    slices.append("static bool xchg_probe(void) { switch (0x87) {\n" +
                  xchg_case + "} return false; }\n")
    group3 = between(cpu386, "static __not_in_flash() void op_grp3_32(",
                     "static inline uint32_t r36sx_read_moffs(")
    idiv_case = between(group3, "        case 7: { /* IDIV */", "\n    }\n}")
    slices.append("static void idiv32_opcode_probe(uint32_t value, "
                  "uint32_t fault_ip) { switch (7) {\n" + idiv_case + "\n} }\n")
    driver = r"""
static int check_idiv16(int32_t dividend, int32_t divisor) {
    /* Widen the oracle: even INT32_MIN / -1 is representable here. */
    int64_t quotient = divisor ? (int64_t)dividend / divisor : 0;
    int64_t remainder = divisor ? (int64_t)dividend % divisor : 0;
    bool fault = !divisor || quotient < INT16_MIN || quotient > INT16_MAX;
    CPU_AX = (uint16_t)dividend;
    CPU_DX = (uint16_t)((uint32_t)dividend >> 16);
    uint16_t old_ax = CPU_AX, old_dx = CPU_DX;
    divide_fault = 0;
    divide_fault_ip = 0;
    op_idiv16((uint32_t)dividend, (uint16_t)divisor, 0x12345);
    if (divide_fault != fault ||
        (fault && (divide_fault_ip != 0x12345 ||
                   CPU_AX != old_ax || CPU_DX != old_dx)) ||
        (!fault && (CPU_AX != (uint16_t)quotient ||
                    CPU_DX != (uint16_t)remainder))) {
        printf("IDIV16 mismatch: dividend=%ld divisor=%ld faults=%d\n",
               (long)dividend, (long)divisor, divide_fault);
        return 1;
    }
    return 0;
}

static int check_idiv32(int64_t dividend, int32_t divisor, bool decoded) {
    /* Host-only oracle: 128-bit division also represents INT64_MIN / -1. */
    __int128 quotient = divisor ? (__int128)dividend / divisor : 0;
    __int128 remainder = divisor ? (__int128)dividend % divisor : 0;
    bool fault = !divisor || quotient < INT32_MIN || quotient > INT32_MAX;
    CPU_EAX = (uint32_t)dividend;
    CPU_EDX = (uint32_t)((uint64_t)dividend >> 32);
    uint32_t old_eax = CPU_EAX, old_edx = CPU_EDX;
    divide_fault = 0;
    divide_fault_ip = 0;
    if (decoded) idiv32_opcode_probe((uint32_t)divisor, 0x87654321);
    else op_idiv32(dividend, (uint32_t)divisor, 0x87654321);
    if (divide_fault != fault ||
        (fault && (divide_fault_ip != 0x87654321 ||
                   CPU_EAX != old_eax || CPU_EDX != old_edx)) ||
        (!fault && (CPU_EAX != (uint32_t)quotient ||
                    CPU_EDX != (uint32_t)remainder))) {
        printf("IDIV32 mismatch: dividend=%lld divisor=%ld decoded=%u faults=%d\n",
               (long long)dividend, (long)divisor, decoded, divide_fault);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) return 2;
    for (unsigned i = 0; i < 256; i++) parity[i] = !__builtin_parity(i);
    if (!strcmp(argv[1], "idiv16_overflow")) {
        volatile uint32_t dividend = 0x80000000u;
        op_idiv16(dividend, 0xffffu, 0x100);
        return divide_fault != 1;
    }
    if (!strcmp(argv[1], "idiv16_boundaries")) {
        const int32_t dividends[] = {
            INT32_MIN, INT32_MIN + 1, -1073741824, -65537, -65536,
            -32769, -32768, -32767, -7, -1, 0, 1, 7, 32766, 32767,
            32768, 32769, 65535, 65536, 1073741824, INT32_MAX
        };
        unsigned cases = 0;
        for (unsigned i = 0; i < sizeof(dividends) / sizeof(dividends[0]); i++)
        for (int32_t divisor = INT16_MIN; divisor <= INT16_MAX; divisor++) {
            if (check_idiv16(dividends[i], divisor)) return 1;
            cases++;
        }
        printf("%u IDIV16 boundary/divisor cases passed\n", cases);
        return 0;
    }
    if (!strcmp(argv[1], "idiv32_overflow")) {
        volatile int64_t dividend = INT64_MIN;
        op_idiv32(dividend, UINT32_MAX, 0x100);
        return divide_fault != 1;
    }
    if (!strcmp(argv[1], "idiv32_dividend_assembly")) {
        return check_idiv32(-7, 3, true);
    }
    if (!strcmp(argv[1], "idiv32_boundaries")) {
        const int64_t dividends[] = {
            INT64_MIN, INT64_MIN + 1, -INT64_C(4611686018427387904),
            -INT64_C(4294967297), -INT64_C(4294967296), -INT64_C(2147483649),
            INT32_MIN, INT32_MIN + 1, -7, -1, 0, 1, 7, INT32_MAX,
            INT64_C(2147483648), INT64_C(2147483649), INT64_C(4294967295),
            INT64_C(4294967296), INT64_C(4611686018427387904), INT64_MAX
        };
        const int32_t divisors[] = {
            INT32_MIN, INT32_MIN + 1, -65536, -32768, -7, -3, -2, -1,
            0, 1, 2, 3, 7, 32767, 65535, INT32_MAX - 1, INT32_MAX
        };
        unsigned cases = 0;
        for (unsigned i = 0; i < sizeof(dividends) / sizeof(dividends[0]); i++)
        for (unsigned j = 0; j < sizeof(divisors) / sizeof(divisors[0]); j++)
        for (unsigned decoded = 0; decoded < 2; decoded++) {
            if (check_idiv32(dividends[i], divisors[j], decoded)) return 1;
            cases++;
        }
        /* Fixed-seed mixed-sign dividends, including nonzero high halves. */
        uint64_t bits = UINT64_C(0xabcdef1234567890);
        for (unsigned i = 0; i < 50000; i++) {
            bits ^= bits << 13; bits ^= bits >> 7; bits ^= bits << 17;
            int64_t dividend = (int64_t)bits;
            int32_t divisor = (int32_t)(bits >> 17);
            for (unsigned decoded = 0; decoded < 2; decoded++) {
                if (check_idiv32(dividend, divisor, decoded)) return 1;
                cases++;
            }
        }
        printf("%u IDIV32 helper/opcode cases passed\n", cases);
        return 0;
    }
    if (!strcmp(argv[1], "bit_negative16") ||
        !strcmp(argv[1], "bit_negative32")) {
        operandSizeOverride = !strcmp(argv[1], "bit_negative32");
        registers[0] = 0x1000;
        r36sx_cpu_exec_bit_test(0, operandSizeOverride ? UINT32_MAX : 0xffff, 1);
        uint32_t expected = operandSizeOverride ? 0xffc : 0xffe;
        printf("address=%08x expected=%08x\n", last_address, expected);
        return last_address != expected;
    }
    if (!strcmp(argv[1], "xchg_address_alias")) {
        registers[0] = 0x1000;
        xchg_probe();
        printf("write address=%08x expected=00001000\n", last_address);
        return last_address != 0x1000;
    }
    if (!strcmp(argv[1], "rep16_index_wrap")) {
        source_index = 0xffff;
        r36sx_rep_movsb(2);
        printf("second source offset=%08x expected=00000000\n", last_address);
        return last_address != 0;
    }
    if (!strcmp(argv[1], "conditions")) {
        for (unsigned f = 0; f < 32; f++) {
            of = (f >> 4) & 1; sf = (f >> 3) & 1;
            zf = (f >> 2) & 1; cf = (f >> 1) & 1; pf = f & 1;
            const bool expected[16] = {
                of, !of, cf, !cf, zf, !zf, cf || zf, !cf && !zf,
                sf, !sf, pf, !pf, sf != of, sf == of,
                zf || sf != of, !zf && sf == of
            };
            for (unsigned c = 0; c < 16; c++)
                if (r36sx_cpu_condition(c) != expected[c]) return 1;
        }
        puts("512 condition/flag combinations passed");
        return 0;
    }
    if (!strcmp(argv[1], "adc_sbb8")) {
        for (unsigned a = 0; a < 256; a++)
        for (unsigned b = 0; b < 256; b++)
        for (unsigned carry = 0; carry < 2; carry++)
        for (unsigned sub = 0; sub < 2; sub++) {
            int signed_result = sub ? (int)(int8_t)a - (int)(int8_t)b - (int)carry
                                    : (int)(int8_t)a + (int)(int8_t)b + (int)carry;
            unsigned result = sub ? a - b - carry : a + b + carry;
            unsigned byte = result & 255;
            if (sub) {
                if (sbb8(a, b, carry) != byte) return 1;
            } else flag_adc8(a, b, carry);
            bool carry_expected = sub ? a < b + carry : result > 255;
            bool af_expected = sub ? (a & 15) < (b & 15) + carry
                                   : (a & 15) + (b & 15) + carry > 15;
            if (cf != carry_expected || af != af_expected || zf != (byte == 0)
                || sf != (byte >> 7) || pf != !__builtin_parity(byte)
                || of != (signed_result < -128 || signed_result > 127)) return 1;
        }
        puts("262144 ADC/SBB operand/carry cases passed");
        return 0;
    }
    return 2;
}
"""
    names = args.case or PROBES
    failed = 0
    with tempfile.TemporaryDirectory(prefix="cpu-audit-", dir=args.output_dir) as work:
        source = Path(work) / "probe.c"
        binary = Path(work) / "probe"
        source.write_text(preamble + "\n".join(slices) + driver, encoding="ascii")
        subprocess.run([args.cc, "-std=c11", "-O2", "-fsanitize=undefined",
                        "-fno-sanitize-recover=undefined", str(source), "-o",
                        str(binary)], check=True)
        for name in names:
            result = subprocess.run([str(binary), name], text=True,
                                    capture_output=True, timeout=10)
            failed += result.returncode != 0
            print(f"{'FAIL' if result.returncode else 'PASS'} {name}")
            for output in (result.stdout, result.stderr):
                if output.strip():
                    print(output.strip())
    print(f"{len(names) - failed} passed, {failed} failed")
    return bool(failed)


if __name__ == "__main__":
    raise SystemExit(main())
