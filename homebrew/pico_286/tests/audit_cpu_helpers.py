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


def between(text, start, end):
    begin = text.index(start)
    return text[begin:text.index(end, begin)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cc", default="gcc")
    parser.add_argument("--output-dir", type=Path, required=True)
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
static void r36sx_cpu_divide_error(uint32_t ip) { (void)ip; divide_fault++; }
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
    driver = r"""
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    for (unsigned i = 0; i < 256; i++) parity[i] = !__builtin_parity(i);
    if (!strcmp(argv[1], "idiv16_overflow")) {
        volatile uint32_t dividend = 0x80000000u;
        op_idiv16(dividend, 0xffffu, 0x100);
        return divide_fault != 1;
    }
    if (!strcmp(argv[1], "idiv32_overflow")) {
        volatile int64_t dividend = INT64_MIN;
        op_idiv32(dividend, UINT32_MAX, 0x100);
        return divide_fault != 1;
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
    names = ("conditions", "adc_sbb8", "idiv16_overflow", "idiv32_overflow",
             "bit_negative16", "bit_negative32", "xchg_address_alias",
             "rep16_index_wrap")
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
