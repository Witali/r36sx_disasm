/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

#if R36SX_DEBUG_INVALID_OPCODE_DUMP
#define R36SX_INVALID_OPCODE_DUMP_BYTES 256u
#define R36SX_INVALID_OPCODE_DUMP_HALF (R36SX_INVALID_OPCODE_DUMP_BYTES / 2u)

static inline void r36sx_cpu_log_invalid_opcode_dump(uint32_t fault_ip)
{
    const uint32_t center_ip = r36sx_cpu_mask_ip(fault_ip);
    const uint32_t start_ip = r36sx_cpu_mask_ip(fault_ip - R36SX_INVALID_OPCODE_DUMP_HALF);

    r36sx_pico286_debug_log(
        "[CPU] INT6 context dump cs=%04X center=%08lX start=%08lX bytes=%lu",
        CPU_CS,
        (unsigned long)center_ip,
        (unsigned long)start_ip,
        (unsigned long)R36SX_INVALID_OPCODE_DUMP_BYTES);

    for (uint32_t row = 0; row < R36SX_INVALID_OPCODE_DUMP_BYTES; row += 16u) {
        uint8_t bytes[16];
        char marks[17];

        for (uint32_t col = 0; col < 16u; ++col) {
            const uint32_t code_ip = r36sx_cpu_mask_ip(start_ip + row + col);
            bytes[col] = getmem8(CPU_CS, code_ip);
            marks[col] = (code_ip == center_ip) ? '^' : ' ';
        }
        marks[16] = '\0';

        r36sx_pico286_debug_log(
            "[CPU] %04X:%08lX  "
            "%02X %02X %02X %02X %02X %02X %02X %02X  "
            "%02X %02X %02X %02X %02X %02X %02X %02X  |%s|",
            CPU_CS,
            (unsigned long)r36sx_cpu_mask_ip(start_ip + row),
            bytes[0], bytes[1], bytes[2], bytes[3],
            bytes[4], bytes[5], bytes[6], bytes[7],
            bytes[8], bytes[9], bytes[10], bytes[11],
            bytes[12], bytes[13], bytes[14], bytes[15],
            marks);
    }
}
#endif

static inline uint8_t r36sx_cpu_invalid_opcode_nop_like(void)
{
    return r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086;
}

static inline void r36sx_cpu_invalid_opcode(uint32_t fault_ip)
{
    if (r36sx_cpu_invalid_opcode_nop_like()) {
        (void)fault_ip;
        return;
    }

    r36sx_pm_diag_log_first_fault("invalid opcode", fault_ip);
#if R36SX_DEBUG_INVALID_OPCODE_DUMP
    r36sx_pico286_debug_log(
        "[CPU] INT6 invalid opcode at %04X:%08lX bytes=%02X %02X %02X %02X %02X %02X %02X %02X flags=%04X ax=%04X bx=%04X cx=%04X dx=%04X si=%04X di=%04X bp=%04X sp=%04X ds=%04X es=%04X ss=%04X",
        CPU_CS, (unsigned long)fault_ip,
        getmem8(CPU_CS, fault_ip),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 1u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 2u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 3u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 4u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 5u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 6u)),
        getmem8(CPU_CS, r36sx_cpu_mask_ip(fault_ip + 7u)),
        (uint16_t)(2u | x86_flags.value),
        CPU_AX, CPU_BX, CPU_CX, CPU_DX,
        CPU_SI, CPU_DI, CPU_BP, CPU_SP,
        CPU_DS, CPU_ES, CPU_SS);
    r36sx_cpu_log_invalid_opcode_dump(fault_ip);
#endif
    r36sx_cpu_set_ip(fault_ip);
    if (r36sx_cpu_protected_enabled()) {
        /*
         * #UD is a processor exception in protected mode, so it is delivered
         * through the protected IDT and pushes the faulting instruction IP.
         */
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_INVALID_OPCODE, 0, 0,
                                  fault_ip);
        return;
    }
    intcall86(6);
}

static inline void r36sx_cpu_divide_error(uint32_t fault_ip)
{
    /*
     * #DE pushes the address of the faulting instruction. DIV/IDIV and AAM
     * detect the fault after operands were fetched, so restore IP first.
     */
    r36sx_cpu_set_ip(fault_ip);
    intcall86(0);
}

static inline void flag_szp8(uint8_t value) {
    zf = value == 0;
    sf = value >> 7;
    pf = parity[value];
}

static inline void flag_szp16(uint16_t value) {
    zf = value == 0;
    sf = value >> 15;
    pf = parity[value & 255];
}

static inline void flag_szp32(uint32_t value) {
    zf = value == 0;
    sf = value >> 31;
    pf = parity[value & 255];
}

static inline void flag_log8(uint8_t value) {
    flag_szp8(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_log16(uint16_t value) {
    flag_szp16(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_log32(uint32_t value) {
    flag_szp32(value);
    x86_flags.value &= ~FLAG_CF_OF_MASK;
}

static inline void flag_adc8(uint8_t v1, uint8_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    uint32_t dst = (uint32_t) v1 + (uint32_t) v2 + (uint32_t) v3;
    flag_szp8((uint8_t) dst);
    of = ((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2) & 0x80) != 0;
    cf = (dst & 0xFF00) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_adc16(uint16_t v1, uint16_t v2, uint16_t v3) {
    register uint32_t dst = (uint32_t) v1 + (uint32_t) v2 + (uint32_t) v3;
    flag_szp16((uint16_t) dst);
    of = (((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2)) & 0x8000) != 0;
    cf = (dst & 0xFFFF0000) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_adc32(uint32_t v1, uint32_t v2, uint8_t v3) {
    uint64_t dst64 = (uint64_t)v1 + (uint64_t)v2 + (uint64_t)v3;
    uint32_t dst = (uint32_t)dst64;
    flag_szp32(dst);
    of = ((dst ^ v1) & (dst ^ v2) & 0x80000000u) != 0;
    cf = (dst64 >> 32) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

static inline void flag_add8(uint8_t v1, uint8_t v2) {
    /* v1 = destination operand, v2 = source operand */
    register uint32_t dst = (uint32_t) v1 + (uint32_t) v2;
    flag_szp8((uint8_t) dst);
    cf = (dst & 0xFF00) != 0;
    of = ((dst ^ (uint32_t)v1) & (dst ^ (uint32_t)v2) & 0x80) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_add32(uint32_t v1, uint32_t v2, uint32_t res32) {
    /* v1 = destination operand, v2 = source operand */
    flag_szp32(res32);
    cf = (((uint64_t) v1 + (uint64_t) v2) >> 32) != 0;
    of = ((res32 ^ v1) & (res32 ^ v2) & 0x80000000u) != 0;
    af = ((v1 ^ v2 ^ res32) & 0x10) != 0;
}

static inline uint8_t sbb8(uint8_t v1, uint8_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint32_t dst = (uint32_t)v1 - (uint32_t)v2 - (uint32_t)v3;
    flag_szp8((uint8_t) dst);
    cf = ((dst >> 8) & 1) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint8_t)dst;
}

static inline uint16_t sbb16(uint16_t v1, uint16_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint32_t dst = (uint32_t)v1 - (uint32_t)v2 - (uint32_t)v3;
    flag_szp16((uint16_t) dst);
    cf = ((dst >> 16) & 1) != 0;
    of = ((dst ^ (uint32_t)v1) & (v1 ^ (uint32_t)v2) & 0x8000) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint16_t)dst;
}

static inline uint32_t sbb32(uint32_t v1, uint32_t v2, uint8_t v3) {
    /* v1 = destination operand, v2 = source operand, v3 = carry flag */
    register uint64_t dst = (uint64_t)v1 - (uint64_t)v2 - (uint64_t)v3;
    flag_szp32((uint32_t) dst);
    cf = ((dst >> 32) & 1) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80000000) != 0;
    af = ((v1 ^ v2 ^ dst ^ v3) & 0x10) != 0;
    return (uint32_t)dst;
}

static inline void flag_sub8(uint8_t v1, uint8_t v2) {
    /* v1 = destination operand, v2 = source operand */
    uint32_t dst = (uint32_t) v1 - (uint32_t) v2;
    flag_szp8((uint8_t) dst);
    cf = (dst & 0xFF00) != 0;
    of = ((dst ^ (uint32_t)v1) & (v1 ^ v2) & 0x80) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

static inline void flag_sub16(uint16_t v1, uint16_t v2) {
    /* v1 = destination operand, v2 = source operand */
    register uint32_t dst = (uint32_t) v1 - (uint32_t) v2;
    flag_szp16((uint16_t) dst);
    cf = (dst & 0xFFFF0000) != 0;
    of = ((dst ^ (uint32_t)v1) & ((uint32_t)v1 ^ (uint32_t)v2) & 0x8000) != 0;
    af = (((uint32_t)v1 ^ (uint32_t)v2 ^ dst) & 0x10) != 0;
}

static inline void flag_sub32(uint32_t v1, uint32_t v2) {
    uint64_t dst64 = (uint64_t)v1 - (uint64_t)v2;
    uint32_t dst = (uint32_t)dst64;
    flag_szp32(dst);
    cf = (dst64 >> 32) != 0;
    of = ((dst ^ v1) & (v1 ^ v2) & 0x80000000u) != 0;
    af = ((v1 ^ v2 ^ dst) & 0x10) != 0;
}

#define op_adc8() { res8 = oper1b + oper2b + cf; flag_adc8(oper1b, oper2b, cf); }
#define op_adc16() { res16 = oper1 + oper2 + cf; flag_adc16(oper1, oper2, cf); }
#define op_adc32() { res32 = oper1 + oper2 + cf; flag_adc32(oper1, oper2, cf); }
#define op_add8() { \
    register uint32_t dst = (uint32_t)oper1b + (uint32_t)oper2b; \
    res8 = dst; \
    flag_szp8(res8); \
    cf = (dst & 0xFF00) != 0; \
    of = ((dst ^ (uint32_t)oper1b) & (dst ^ (uint32_t)oper2b) & 0x80) != 0; \
    af = ((oper1b ^ oper2b ^ dst) & 0x10) != 0; \
}
#define op_add16() { \
    register uint32_t dst = (uint32_t)oper1 + (uint32_t)oper2; \
    res16 = dst; \
    flag_szp16(dst); \
    cf = (dst & 0xFFFF0000) != 0; \
    of = (((dst ^ (uint32_t)oper1) & (dst ^ (uint32_t)oper2) & 0x8000) != 0); \
    af = (((oper1 ^ oper2 ^ dst) & 0x10) != 0); \
}
#define op_add32() { res32 = oper1 + oper2; flag_add32(oper1, oper2, res32); }
#define op_and8() { res8 = oper1b & oper2b; flag_log8(res8); }
#define op_and16() { res16 = oper1 & oper2; flag_log16(res16); }
#define op_and32() { res32 = oper1 & oper2; flag_log32(res32); }
#define op_or8() { res8 = oper1b | oper2b; flag_log8(res8); }
#define op_or16() { res16 = oper1 | oper2; flag_log16(res16); }
#define op_or32() { res32 = oper1 | oper2; flag_log32(res32); }
#define op_xor8() { res8 = oper1b ^ oper2b; flag_log8(res8); }
#define op_xor16() { res16 = oper1 ^ oper2; flag_log16(res16); }
#define op_xor32() { res32 = oper1 ^ oper2; flag_log32(res32); }
#define op_sub8() { res8 = oper1b - oper2b; flag_sub8(oper1b, oper2b); }
#define op_sub16() { \
    register uint32_t dst = (uint32_t) oper1 - (uint32_t) oper2; \
    flag_szp16((uint16_t) dst); \
    cf = (dst & 0xFFFF0000) != 0; \
    of = ((dst ^ (uint32_t)oper1) & (oper1 ^ oper2) & 0x8000) != 0; \
    af = ((oper1 ^ oper2 ^ dst) & 0x10) != 0; \
    res16 = (uint16_t) dst; \
}
#define op_sub32() { res32 = oper1 - oper2; flag_sub32(oper1, oper2); }
#define op_sbb8() { res8 = sbb8(oper1b, oper2b, cf); }
#define op_sbb16() { res16 = sbb16(oper1, oper2, cf); }
#define op_sbb32() { res32 = sbb32(oper1, oper2, cf); }

static __not_in_flash() uint8_t op_grp2_8(uint8_t cnt, uint8_t oper1b) {
    register uint16_t s = oper1b;
    /*
     * Intel rotate/shift instructions with an effective count of zero are
     * no-ops: the destination and all FLAGS bits must remain unchanged.
     */
    if (cnt == 0) {
        return (uint8_t)s;
    }
    switch (reg) {
        case 0: /* ROL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | cf;
            }

            /*
             * OF is architecturally defined only for count=1.  DOSBox and
             * test386 both model the stable hardware result for larger counts.
             */
            of = cf ^ ((s & 0x80u) ? 1u : 0u);
            break;

        case 1: /* ROR r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = (s >> 1) | (cf << 7);
            }

            of = ((s >> 7) ^ (s >> 6)) & 1u;
            break;

        case 2: /* RCL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                register bool oldcf = cf;
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | oldcf;
            }

            of = cf ^ ((s & 0x80u) ? 1u : 0u);
            break;

        case 3: /* RCR r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                register uint8_t oldcf = cf;
                cf = s & 1;
                s = (s >> 1) | (oldcf << 7);
            }

            of = ((s >> 7) ^ (s >> 6)) & 1u;
            break;

        case 4:
        case 6: /* SHL r/m8 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x80) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = (s << 1) & 0xFF;
            }

            if ((cnt == 1) && (cf == (s >> 7))) {
                of = 0;
            } else {
                of = 1;
            }

            flag_szp8((uint8_t) s);
            break;

        case 5: /* SHR r/m8 */
            if ((cnt == 1) && (s & 0x80)) {
                of = 1;
            } else {
                of = 0;
            }

            for (int a = 1; a <= cnt; a++) {
                cf = s & 1;
                s = s >> 1;
            }

            flag_szp8((uint8_t) s);
            break;

        case 7: /* SAR r/m8 */
            for (int a = 1; a <= cnt; a++) {
                unsigned int msb = s & 0x80;
                cf = s & 1;
                s = (s >> 1) | msb;
            }

            of = 0;
            flag_szp8((uint8_t) s);
            break;
    }

    return s & 0xFF;
}

static __not_in_flash() uint16_t op_grp2_16(uint8_t cnt) {
    register uint32_t s = oper1;
    /*
     * Intel rotate/shift instructions with an effective count of zero are
     * no-ops: the destination and all FLAGS bits must remain unchanged.
     */
    if (cnt == 0) {
        return (uint16_t)s;
    }
    switch (reg) {
        case 0: /* ROL r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | cf;
            }

            /*
             * OF is architecturally defined only for count=1.  DOSBox and
             * test386 both model the stable hardware result for larger counts.
             */
            of = cf ^ ((s & 0x8000u) ? 1u : 0u);
            break;

        case 1: /* ROR r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = (s >> 1) | (cf << 15);
            }

            of = ((s >> 15) ^ (s >> 14)) & 1u;
            break;

        case 2: /* RCL r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                register bool oldcf = cf;
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = s << 1;
                s = s | oldcf;
            }

            of = cf ^ ((s & 0x8000u) ? 1u : 0u);
            break;

        case 3: /* RCR r/m16 */
            for (int shift = 1; shift <= cnt; shift++) {
                register uint32_t oldcf = cf;
                cf = s & 1;
                s = (s >> 1) | (oldcf << 15);
            }

            of = ((s >> 15) ^ (s >> 14)) & 1u;
            break;

        case 4:
        case 6: /* SHL r/m16 */
            for (unsigned int shift = 1; shift <= cnt; shift++) {
                if (s & 0x8000) {
                    cf = 1;
                } else {
                    cf = 0;
                }

                s = (s << 1) & 0xFFFF;
            }

            if ((cnt == 1) && (cf == (s >> 15))) {
                of = 0;
            } else {
                of = 1;
            }

            flag_szp16((uint16_t) s);
            break;

        case 5: /* SHR r/m16 */
            if ((cnt == 1) && (s & 0x8000)) {
                of = 1;
            } else {
                of = 0;
            }

            for (int shift = 1; shift <= cnt; shift++) {
                cf = s & 1;
                s = s >> 1;
            }

            flag_szp16((uint16_t) s);
            break;

        case 7: /* SAR r/m16 */
            for (int shift = 1, msb; shift <= cnt; shift++) {
                msb = s & 0x8000;
                cf = s & 1;
                s = (s >> 1) | msb;
            }

            of = 0;
            flag_szp16((uint16_t) s);
            break;
    }

    return (uint16_t) s & 0xFFFF;
}

static inline void op_div8(uint16_t valdiv, uint8_t divisor, uint32_t fault_ip) {
    if (divisor == 0 || valdiv / divisor > 0xFF) {
        /* Intel DIV raises #DE for divide-by-zero or quotient overflow. */
        r36sx_cpu_divide_error(fault_ip);
        return;
    }

    CPU_AH = (uint8_t) (valdiv % divisor);
    CPU_AL = (uint8_t) (valdiv / divisor);
}

static inline void op_idiv8(uint16_t valdiv, int8_t divisor, uint32_t fault_ip) {
    if (divisor == 0) {
        /* Intel IDIV reports divide-by-zero through #DE, not stdout. */
        r36sx_cpu_divide_error(fault_ip);
        return;
    }
    int16_t dividend = (int16_t) valdiv;
    int16_t quotient  = dividend / divisor;
    int16_t remainder = dividend % divisor;
    if (quotient < -128 || quotient > 127) {
        /* Quotient overflow is the same #DE fault as divide-by-zero. */
        r36sx_cpu_divide_error(fault_ip);
        return;
    }
    CPU_AL = (uint8_t)quotient;
    CPU_AH = (uint8_t)remainder;
}

static inline void op_div16(uint32_t valdiv, uint16_t divisor, uint32_t fault_ip) {
    if (divisor == 0 || valdiv / divisor > 0xFFFF) {
//        CPU_DX = 0;
//        CPU_AX = 0xFFFF;
//        printf("[op_div16] %d / %d\n", valdiv, divisor);
        r36sx_cpu_divide_error(fault_ip);
        return;
    }

    CPU_DX = (uint16_t) (valdiv % divisor);
    CPU_AX = (uint16_t) (valdiv / divisor);
}

static inline void op_idiv16(uint32_t valdiv, uint16_t divisor, uint32_t fault_ip) {
    int32_t dividend = (int32_t)valdiv;
    int16_t divisor_signed = (int16_t)divisor;
    if (divisor_signed == 0) {
        /* Intel IDIV reports divide-by-zero through #DE, not stdout. */
        r36sx_cpu_divide_error(fault_ip);
        return;
    }
    int32_t quotient  = dividend / divisor_signed;
    int32_t remainder = dividend % divisor_signed;
    if (quotient < -32768 || quotient > 32767) {
        /* Quotient overflow is the same #DE fault as divide-by-zero. */
        r36sx_cpu_divide_error(fault_ip);
        return;
    }
    CPU_AX = (uint16_t)quotient;
    CPU_DX = (uint16_t)remainder;
}

static __not_in_flash() void op_grp3_16(uint32_t fault_ip) {
    switch (reg) {
        case 0: /* TEST */
            flag_log16(oper1 & getmem16(CPU_CS, CPU_IP));
            StepIP(2);
            break;

        case 1:
            /*
             * F7 /1 is an undocumented TEST alias on some 8086-family chips.
             * Treat it as invalid for the documented CPU compatibility mode.
             */
            r36sx_cpu_invalid_opcode(fault_ip);
            break;

        case 2: /* NOT */
            res16 = ~oper1;
            break;

        case 3: /* NEG */
            res16 = (~oper1) + 1;
            flag_sub16(0, oper1);
            if (res16) {
                cf = 1;
            } else {
                cf = 0;
            }
            break;

        case 4: {
            /* MUL */
            register uint32_t temp1 = (uint32_t) oper1 * (uint32_t) CPU_AX;
            CPU_AX = temp1 & 0xFFFF;
            CPU_DX = temp1 >> 16;
            flag_szp16((uint16_t) temp1);
            if (CPU_DX) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }
        case 5: {
            /* IMUL */
            register int32_t temp1 = (int32_t)(int16_t)CPU_AX * (int32_t)(int16_t)oper1;
			int16_t truncated = (int16_t)temp1;
            CPU_AX = truncated; /* into register ax */
            CPU_DX = (uint16_t)(temp1 >> 16); /* into register dx */
            if (temp1 != (int32_t)truncated) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }
        case 6: /* DIV */
            op_div16(((uint32_t) CPU_DX << 16) + CPU_AX, oper1, fault_ip);
            break;

        case 7: /* IDIV */
            op_idiv16(((uint32_t) CPU_DX << 16) + CPU_AX, oper1, fault_ip);
            break;
    }
}

static __not_in_flash() void op_grp5(uint32_t fault_ip) {
    switch (reg) {
        case 0: /* INC Ev */
            oper2 = 1;
            tempcf = cf;
            op_add16();
            cf = tempcf;
            writerm16(rm, res16);
            break;

        case 1: /* DEC Ev */
            oper2 = 1;
            tempcf = cf;
            op_sub16();
            cf = tempcf;
            writerm16(rm, res16);
            break;

        case 2: /* CALL Ev */
            push(ip);
            ip = oper1;
            break;

        case 3: /* CALL Mp */
            getea(rm);
            if (!r36sx_cpu_check_segment_access(ea - useseg_base, 4u, 0)) {
                break;
            }
            oper1 = readw86(ea);
            oper2 = readw86(ea + 2u);
            if (r36sx_cpu_protected_enabled()) {
                r36sx_cpu_protected_far_call(oper2, oper1, 0, fault_ip);
                break;
            }
            push(CPU_CS);
            push(ip);
            r36sx_cpu_load_segment(regcs, oper2);
            r36sx_cpu_set_ip(oper1);
            break;

        case 4: /* JMP Ev */
            ip = oper1;
            break;

        case 5: /* JMP Mp */
            getea(rm);
            if (!r36sx_cpu_check_segment_access(ea - useseg_base, 4u, 0)) {
                break;
            }
            oper1 = readw86(ea);
            oper2 = readw86(ea + 2u);
            if (r36sx_cpu_protected_enabled()) {
                r36sx_cpu_protected_far_jump(oper2, oper1, fault_ip);
                break;
            }
            r36sx_cpu_load_segment(regcs, oper2);
            r36sx_cpu_set_ip(oper1);
            break;

        case 6: /* PUSH Ev */
            push(oper1);
            break;
    }
}
