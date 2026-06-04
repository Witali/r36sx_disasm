/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

static inline uint32_t r36sx_cpu_alu32(uint8_t aluop, uint32_t dst, uint32_t src)
{
    uint32_t result;
    uint8_t carry;

    switch (aluop) {
        case 0: /* ADD */
            result = dst + src;
            flag_add32(dst, src, result);
            return result;
        case 1: /* OR */
            result = dst | src;
            flag_log32(result);
            return result;
        case 2: /* ADC */
            carry = cf;
            result = dst + src + carry;
            flag_adc32(dst, src, carry);
            return result;
        case 3: /* SBB */
            carry = cf;
            return sbb32(dst, src, carry);
        case 4: /* AND */
            result = dst & src;
            flag_log32(result);
            return result;
        case 5: /* SUB */
            result = dst - src;
            flag_sub32(dst, src);
            return result;
        case 6: /* XOR */
            result = dst ^ src;
            flag_log32(result);
            return result;
        case 7: /* CMP */
            flag_sub32(dst, src);
            return dst;
    }

    return dst;
}

static inline void r36sx_cpu_alu_rm_reg32(uint8_t aluop, uint8_t rmval,
                                          uint8_t regid)
{
    uint32_t result = r36sx_cpu_alu32(aluop, readrm32(rmval),
                                      getreg32(regid));
    if (aluop != 7) {
        writerm32(rmval, result);
    }
}

static inline void r36sx_cpu_alu_reg_rm32(uint8_t aluop, uint8_t regid,
                                          uint8_t rmval)
{
    uint32_t result = r36sx_cpu_alu32(aluop, getreg32(regid),
                                      readrm32(rmval));
    if (aluop != 7) {
        putreg32(regid, result);
    }
}

static inline void r36sx_cpu_alu_eax_imm32(uint8_t aluop)
{
    uint32_t imm = getmem32(CPU_CS, CPU_IP);
    StepIP(4);
    uint32_t result = r36sx_cpu_alu32(aluop, CPU_EAX, imm);
    if (aluop != 7) {
        CPU_EAX = result;
    }
}

static inline void r36sx_cpu_alu_rm_imm32(uint8_t aluop, uint8_t rmval,
                                          uint32_t imm)
{
    uint32_t result = r36sx_cpu_alu32(aluop, readrm32(rmval), imm);
    if (aluop != 7) {
        writerm32(rmval, result);
    }
}

static inline uint8_t r36sx_cpu_condition(uint8_t condition)
{
    uint8_t value;

    /*
     * x86 condition codes are paired: the even opcode tests the base
     * predicate, and the following odd opcode tests its inverse.
     */
    switch (condition & 0x0E) {
        case 0x0: value = of != 0; break;          /* O / NO */
        case 0x2: value = cf != 0; break;          /* B/C/NAE / NB/NC/AE */
        case 0x4: value = zf != 0; break;          /* Z/E / NZ/NE */
        case 0x6: value = (cf || zf) != 0; break;  /* BE/NA / NBE/A */
        case 0x8: value = sf != 0; break;          /* S / NS */
        case 0xA: value = pf != 0; break;          /* P/PE / NP/PO */
        case 0xC: value = (sf != of); break;       /* L/NGE / NL/GE */
        case 0xE: value = (zf || (sf != of)) != 0; break; /* LE/NG / NLE/G */
        default: value = 0; break;
    }

    return (uint8_t)(value ^ (condition & 1u));
}

static __not_in_flash() uint32_t op_grp2_32(uint8_t cnt, uint32_t value) {
    uint32_t s = value;
#ifdef CPU_LIMIT_SHIFT_COUNT
    cnt &= 0x1F;
#endif
    if (!cnt) {
        return s;
    }

    switch (reg) {
        case 0: /* ROL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = (s >> 31) & 1u;
                s = (s << 1) | cf;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            break;

        case 1: /* ROR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s = (s >> 1) | ((uint32_t)cf << 31);
            }
            if (cnt == 1) {
                of = ((s >> 31) ^ (s >> 30)) & 1u;
            }
            break;

        case 2: /* RCL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                uint8_t oldcf = cf;
                cf = (s >> 31) & 1u;
                s = (s << 1) | oldcf;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            break;

        case 3: /* RCR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                uint8_t oldcf = cf;
                cf = s & 1u;
                s = (s >> 1) | ((uint32_t)oldcf << 31);
            }
            if (cnt == 1) {
                of = ((s >> 31) ^ (s >> 30)) & 1u;
            }
            break;

        case 4:
        case 6: /* SHL/SAL r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = (s >> 31) & 1u;
                s <<= 1;
            }
            if (cnt == 1) {
                of = cf ^ ((s >> 31) & 1u);
            }
            flag_szp32(s);
            break;

        case 5: { /* SHR r/m32 */
            uint8_t oldmsb = (s >> 31) & 1u;
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s >>= 1;
            }
            if (cnt == 1) {
                of = oldmsb;
            }
            flag_szp32(s);
            break;
        }

        case 7: /* SAR r/m32 */
            for (uint8_t shift = 0; shift < cnt; shift++) {
                cf = s & 1u;
                s = (uint32_t)((int32_t)s >> 1);
            }
            of = 0;
            flag_szp32(s);
            break;
    }

    return s;
}

static inline void op_div32(uint64_t valdiv, uint32_t divisor, uint32_t fault_ip) {
    if (divisor == 0 || valdiv / divisor > 0xFFFFFFFFull) {
        r36sx_cpu_divide_error(fault_ip);
        return;
    }

    CPU_EDX = (uint32_t)(valdiv % divisor);
    CPU_EAX = (uint32_t)(valdiv / divisor);
}

static inline void op_idiv32(int64_t dividend, uint32_t divisor, uint32_t fault_ip) {
    int32_t divisor_signed = (int32_t)divisor;
    if (divisor_signed == 0) {
        r36sx_cpu_divide_error(fault_ip);
        return;
    }

    int64_t quotient = dividend / divisor_signed;
    int64_t remainder = dividend % divisor_signed;
    if (quotient < INT32_MIN || quotient > INT32_MAX) {
        r36sx_cpu_divide_error(fault_ip);
        return;
    }

    CPU_EAX = (uint32_t)(int32_t)quotient;
    CPU_EDX = (uint32_t)(int32_t)remainder;
}

static __not_in_flash() void op_grp3_32(uint8_t rmval, uint32_t fault_ip) {
    uint32_t value = readrm32(rmval);
    switch (reg) {
        case 0:
        case 1: { /* TEST */
            uint32_t imm = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            flag_log32(value & imm);
            break;
        }

        case 2: /* NOT */
            writerm32(rmval, ~value);
            break;

        case 3: { /* NEG */
            uint32_t result = (uint32_t)(0u - value);
            flag_sub32(0, value);
            cf = result != 0;
            writerm32(rmval, result);
            break;
        }

        case 4: { /* MUL */
            uint64_t result = (uint64_t)value * (uint64_t)CPU_EAX;
            CPU_EAX = (uint32_t)result;
            CPU_EDX = (uint32_t)(result >> 32);
            if (CPU_EDX) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
#ifdef CPU_CLEAR_ZF_ON_MUL
            zf = 0;
#endif
            break;
        }

        case 5: { /* IMUL */
            int64_t result = (int64_t)(int32_t)CPU_EAX *
                             (int64_t)(int32_t)value;
            CPU_EAX = (uint32_t)result;
            CPU_EDX = (uint32_t)(result >> 32);
            if (result != (int64_t)(int32_t)CPU_EAX) {
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
            op_div32(((uint64_t)CPU_EDX << 32) | CPU_EAX, value, fault_ip);
            break;

        case 7: { /* IDIV */
            int64_t dividend = ((int64_t)(int32_t)CPU_EDX << 32) | CPU_EAX;
            op_idiv32(dividend, value, fault_ip);
            break;
        }
    }
}


static inline uint32_t r36sx_read_moffs(void)
{
    if (addressSizeOverride) {
        uint32_t offset = getmem32(CPU_CS, CPU_IP);
        StepIP(4);
        return offset;
    }

    uint16_t offset = getmem16(CPU_CS, CPU_IP);
    StepIP(2);
    return offset;
}

static __not_in_flash() bool r36sx_cpu_exec_operand32_opcode(uint8_t opcode,
                                                             uint32_t fault_ip,
                                                             uint32_t execloops,
                                                             uint32_t *loopcount,
                                                             bool trace_active)
{
    uint8_t aluop = opcode >> 3;

    switch (opcode) {
        /* INSD */
        case 0x6D: {
            if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return true;
            }
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (!r36sx_cpu_require_io_permission(CPU_DX, 4u, fault_ip)) {
                return true;
            }
            uint32_t di = r36sx_dst_index();
            uint32_t value = (uint32_t)portin16(CPU_DX) |
                             ((uint32_t)portin16((uint16_t)(CPU_DX + 2u))
                              << 16);
            putmem32(CPU_ES, di, value);
            r36sx_set_dst_index(df ? di - 4 : di + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            (*loopcount)++;
            if (reptype) {
                r36sx_cpu_set_ip(fault_ip);
            }
            return true;
        }

        /* OUTSD */
        case 0x6F: {
            if (r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return true;
            }
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (!r36sx_cpu_require_io_permission(CPU_DX, 4u, fault_ip)) {
                return true;
            }
            uint32_t si = r36sx_src_index();
            uint32_t value = getmem32(useseg, si);
            portout16(CPU_DX, (uint16_t)value);
            portout16((uint16_t)(CPU_DX + 2u), (uint16_t)(value >> 16));
            r36sx_set_src_index(df ? si - 4 : si + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            (*loopcount)++;
            if (reptype) {
                r36sx_cpu_set_ip(fault_ip);
            }
            return true;
        }

        /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m32, r32 */
        case 0x01: case 0x09: case 0x11: case 0x19:
        case 0x21: case 0x29: case 0x31: case 0x39:
            modregrm();
            r36sx_cpu_alu_rm_reg32(aluop, rm, reg);
            return true;

        /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r32, r/m32 */
        case 0x03: case 0x0B: case 0x13: case 0x1B:
        case 0x23: case 0x2B: case 0x33: case 0x3B:
            modregrm();
            r36sx_cpu_alu_reg_rm32(aluop, reg, rm);
            return true;

        /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP EAX, imm32 */
        case 0x05: case 0x0D: case 0x15: case 0x1D:
        case 0x25: case 0x2D: case 0x35: case 0x3D:
            r36sx_cpu_alu_eax_imm32(aluop);
            return true;

        /* INC r32 */
        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45: case 0x46: case 0x47: {
            uint8_t regid = opcode & 7u;
            uint8_t saved_cf = cf;
            uint32_t value = getreg32(regid);
            uint32_t result = value + 1u;
            flag_add32(value, 1u, result);
            cf = saved_cf;
            putreg32(regid, result);
            return true;
        }

        /* DEC r32 */
        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D: case 0x4E: case 0x4F: {
            uint8_t regid = opcode & 7u;
            uint8_t saved_cf = cf;
            uint32_t value = getreg32(regid);
            uint32_t result = value - 1u;
            flag_sub32(value, 1u);
            cf = saved_cf;
            putreg32(regid, result);
            return true;
        }

        /* PUSH r32 */
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55: case 0x56: case 0x57:
            push32(getreg32(opcode & 7u));
            return true;

        /* POP r32 */
        case 0x58: case 0x59: case 0x5A: case 0x5B:
        case 0x5C: case 0x5D: case 0x5E: case 0x5F:
            putreg32(opcode & 7u, pop32());
            return true;

        /* PUSH imm32 */
        case 0x68:
            push32(getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        /* IMUL r32, r/m32, imm32 */
        case 0x69: {
            modregrm();
            int64_t result = (int64_t)(int32_t)readrm32(rm) *
                             (int64_t)(int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            putreg32(reg, (uint32_t)result);
            if (result != (int64_t)(int32_t)(uint32_t)result) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
            return true;
        }

        /* PUSH imm8 */
        case 0x6A:
            push32((uint32_t)(int32_t)(int8_t)getmem8(CPU_CS, CPU_IP));
            StepIP(1);
            return true;

        /* IMUL r32, r/m32, imm8 */
        case 0x6B: {
            modregrm();
            int64_t result = (int64_t)(int32_t)readrm32(rm) *
                             (int64_t)(int8_t)getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            putreg32(reg, (uint32_t)result);
            if (result != (int64_t)(int32_t)(uint32_t)result) {
                x86_flags.value |= FLAG_CF_OF_MASK;
            } else {
                x86_flags.value &= ~FLAG_CF_OF_MASK;
            }
            return true;
        }

        /* ADD/OR/ADC/SBB/AND/SUB/XOR/CMP r/m32, imm32/imm8 */
        case 0x81:
        case 0x83: {
            modregrm();
            uint32_t imm;
            if (opcode == 0x81) {
                imm = getmem32(CPU_CS, CPU_IP);
                StepIP(4);
            } else {
                imm = (uint32_t)(int32_t)(int8_t)getmem8(CPU_CS, CPU_IP);
                StepIP(1);
            }
            r36sx_cpu_alu_rm_imm32(reg, rm, imm);
            return true;
        }

        /* TEST r/m32, r32 */
        case 0x85:
            modregrm();
            flag_log32(getreg32(reg) & readrm32(rm));
            return true;

        /* XCHG r/m32, r32 */
        case 0x87: {
            modregrm();
            uint32_t tmp = getreg32(reg);
            putreg32(reg, readrm32(rm));
            writerm32(rm, tmp);
            return true;
        }

        /* MOV r/m32, r32 */
        case 0x89:
            modregrm();
            writerm32(rm, getreg32(reg));
            return true;

        /* MOV r32, r/m32 */
        case 0x8B:
            modregrm();
            putreg32(reg, readrm32(rm));
            return true;

        /* LEA r32, m */
        case 0x8D:
            modregrm();
            getea(rm);
            putreg32(reg, ea - useseg_base);
            return true;

        /* POP r/m32 */
        case 0x8F:
            modregrm();
            writerm32(rm, pop32());
            return true;

        /* CALL ptr16:32 */
        case 0x9A: {
            uint32_t target_ip = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            uint16_t target_cs = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (r36sx_cpu_protected_enabled()) {
                r36sx_cpu_protected_far_call(target_cs, target_ip, 1,
                                             fault_ip);
                return true;
            }
            /*
             * Intel far CALL with operand-size 32 stores both return fields in
             * 32-bit stack slots: EIP and a zero-extended CS selector.  The
             * selector itself is still 16 bits, but RETF32 consumes 8 bytes.
             */
            push32(CPU_CS);
            push32(CPU_IP);
            r36sx_cpu_load_segment(regcs, target_cs);
            r36sx_cpu_set_ip(target_ip);
            return true;
        }

        /* NOP */
        case 0x90:
            return true;

        /* XCHG EAX, r32 */
        case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95: case 0x96: case 0x97: {
            uint8_t regid = opcode & 7u;
            uint32_t tmp = CPU_EAX;
            CPU_EAX = getreg32(regid);
            putreg32(regid, tmp);
            return true;
        }

        /* CWDE */
        case 0x98:
            CPU_EAX = (uint32_t)(int32_t)(int16_t)CPU_AX;
            return true;

        /* CDQ */
        case 0x99:
            CPU_EDX = ((int32_t)CPU_EAX < 0) ? 0xFFFFFFFFu : 0u;
            return true;

        /* MOV EAX, moffs32 */
        case 0xA1:
            CPU_EAX = getmem32(useseg, r36sx_read_moffs());
            return true;

        /* MOV moffs32, EAX */
        case 0xA3:
            putmem32(useseg, r36sx_read_moffs(), CPU_EAX);
            return true;

        /* MOVSD */
        case 0xA5:
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (reptype) {
                uint32_t batch = trace_active
                    ? 1u
                    : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                            *loopcount, execloops);
                r36sx_rep_movsd(batch);
                r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                *loopcount += batch;
                if (r36sx_rep_get_count() != 0) {
                    r36sx_cpu_set_ip(fault_ip);
                }
                return true;
            }
            r36sx_rep_movsd(1);
            (*loopcount)++;
            return true;

        /* CMPSD */
        case 0xA7: {
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            uint32_t si = r36sx_src_index();
            uint32_t di = r36sx_dst_index();
            flag_sub32(getmem32(useseg, si), getmem32(CPU_ES, di));
            r36sx_set_src_index(df ? si - 4 : si + 4);
            r36sx_set_dst_index(df ? di - 4 : di + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            if ((reptype == 1) && !zf) {
                return true;
            }
            if ((reptype == 2) && zf) {
                return true;
            }
            (*loopcount)++;
            if (reptype) {
                r36sx_cpu_set_ip(fault_ip);
            }
            return true;
        }

        /* TEST EAX, imm32 */
        case 0xA9:
            flag_log32(CPU_EAX & getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        /* STOSD */
        case 0xAB:
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            if (reptype) {
                uint32_t batch = trace_active
                    ? 1u
                    : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                            *loopcount, execloops);
                r36sx_rep_stosd(batch);
                r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                *loopcount += batch;
                if (r36sx_rep_get_count() != 0) {
                    r36sx_cpu_set_ip(fault_ip);
                }
                return true;
            }
            r36sx_rep_stosd(1);
            (*loopcount)++;
            return true;

        /* LODSD */
        case 0xAD: {
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            uint32_t si = r36sx_src_index();
            CPU_EAX = getmem32(useseg, si);
            r36sx_set_src_index(df ? si - 4 : si + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            (*loopcount)++;
            if (reptype) {
                r36sx_cpu_set_ip(fault_ip);
            }
            return true;
        }

        /* SCASD */
        case 0xAF: {
            if (reptype && r36sx_rep_get_count() == 0) {
                return true;
            }
            uint32_t di = r36sx_dst_index();
            flag_sub32(CPU_EAX, getmem32(CPU_ES, di));
            r36sx_set_dst_index(df ? di - 4 : di + 4);
            if (reptype) {
                r36sx_rep_set_count(r36sx_rep_get_count() - 1);
            }
            if ((reptype == 1) && !zf) {
                return true;
            }
            if ((reptype == 2) && zf) {
                return true;
            }
            (*loopcount)++;
            if (reptype) {
                r36sx_cpu_set_ip(fault_ip);
            }
            return true;
        }

        /* MOV r32, imm32 */
        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD: case 0xBE: case 0xBF:
            putreg32(opcode & 7u, getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        /* SHL/SHR/SAR/ROL/ROR/RCL/RCR r/m32, imm8 */
        case 0xC1:
            modregrm();
            {
                uint32_t value = readrm32(rm);
                uint8_t count = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm32(rm, op_grp2_32(count, value));
            }
            return true;

        /* RET imm16 */
        case 0xC2: {
            uint16_t bytes = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            r36sx_cpu_set_ip(pop32());
            r36sx_cpu_adjust_stack(bytes);
            return true;
        }

        /* RET */
        case 0xC3:
            r36sx_cpu_set_ip(pop32());
            return true;

        /* MOV r/m32, imm32 */
        case 0xC7:
            modregrm();
            if (reg != 0) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return true;
            }
            writerm32(rm, getmem32(CPU_CS, CPU_IP));
            StepIP(4);
            return true;

        /* LEAVE */
        case 0xC9:
            r36sx_cpu_set_stack_pointer(CPU_EBP);
            CPU_EBP = pop32();
            return true;

        /* RETF imm16 */
        case 0xCA: {
            uint16_t bytes = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (r36sx_cpu_native_protected_enabled()) {
                r36sx_cpu_protected_retf(bytes, 1, fault_ip);
                return true;
            }
            r36sx_cpu_set_ip(pop32());
            r36sx_cpu_load_segment(regcs, (uint16_t)pop32());
            r36sx_cpu_adjust_stack(bytes);
            return true;
        }

        /* RETF */
        case 0xCB:
            if (r36sx_cpu_native_protected_enabled()) {
                r36sx_cpu_protected_retf(0, 1, fault_ip);
                return true;
            }
            r36sx_cpu_set_ip(pop32());
            r36sx_cpu_load_segment(regcs, (uint16_t)pop32());
            return true;

        /* IRETD */
        case 0xCF:
            if (r36sx_cpu_v86_iopl_sensitive_fault(fault_ip)) {
                return true;
            }
            if (r36sx_cpu_native_protected_enabled()) {
                r36sx_cpu_protected_iret(1);
                return true;
            }
            r36sx_cpu_set_ip(pop32());
            r36sx_cpu_load_segment(regcs, (uint16_t)pop32());
            decodeflagsdword(pop32());
            return true;

        /* SHL/SHR/SAR/ROL/ROR/RCL/RCR r/m32, 1 */
        case 0xD1:
            modregrm();
            writerm32(rm, op_grp2_32(1, readrm32(rm)));
            return true;

        /* SHL/SHR/SAR/ROL/ROR/RCL/RCR r/m32, CL */
        case 0xD3:
            modregrm();
            writerm32(rm, op_grp2_32(CPU_CL, readrm32(rm)));
            return true;

        /* CALL rel32 */
        case 0xE8: {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            push32(CPU_IP);
            r36sx_cpu_add_ip(rel);
            return true;
        }

        /* JMP rel32 */
        case 0xE9: {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            r36sx_cpu_add_ip(rel);
            return true;
        }

        /* JMP ptr16:32 */
        case 0xEA: {
            uint32_t target_ip = getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            uint16_t target_cs = getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (r36sx_cpu_protected_enabled()) {
                r36sx_cpu_protected_far_jump(target_cs, target_ip, fault_ip);
                return true;
            }
            r36sx_cpu_load_segment(regcs, target_cs);
            r36sx_cpu_set_ip(target_ip);
            return true;
        }

        /* TEST/NOT/NEG/MUL/IMUL/DIV/IDIV r/m32 */
        case 0xF7:
            modregrm();
            op_grp3_32(rm, fault_ip);
            return true;

        /* INC/DEC/CALL/JMP/PUSH r/m32 */
        case 0xFF:
            modregrm();
            switch (reg) {
                case 0: { /* INC Ev */
                    uint8_t saved_cf = cf;
                    uint32_t value = readrm32(rm);
                    uint32_t result = value + 1u;
                    flag_add32(value, 1u, result);
                    cf = saved_cf;
                    writerm32(rm, result);
                    return true;
                }
                case 1: { /* DEC Ev */
                    uint8_t saved_cf = cf;
                    uint32_t value = readrm32(rm);
                    uint32_t result = value - 1u;
                    flag_sub32(value, 1u);
                    cf = saved_cf;
                    writerm32(rm, result);
                    return true;
                }
                case 2: { /* CALL Ev */
                    uint32_t target = readrm32(rm);
                    push32(CPU_IP);
                    r36sx_cpu_set_ip(target);
                    return true;
                }
                case 3: { /* CALL Mp */
                    getea(rm);
                    if (!r36sx_cpu_check_segment_access(ea - useseg_base, 6u, 0)) {
                        return true;
                    }
                    uint32_t target_ip = readdw86(ea);
                    uint16_t target_cs = readw86(ea + 4);
                    if (r36sx_cpu_protected_enabled()) {
                        r36sx_cpu_protected_far_call(target_cs, target_ip, 1,
                                                     fault_ip);
                        return true;
                    }
                    /*
                     * CALL m16:32 follows the same return-stack layout as
                     * direct ptr16:32: RETF32 later pops EIP and a 32-bit CS
                     * slot with the selector in the low 16 bits.
                     */
                    push32(CPU_CS);
                    push32(CPU_IP);
                    r36sx_cpu_load_segment(regcs, target_cs);
                    r36sx_cpu_set_ip(target_ip);
                    return true;
                }
                case 4: /* JMP Ev */
                    r36sx_cpu_set_ip(readrm32(rm));
                    return true;
                case 5: { /* JMP Mp */
                    getea(rm);
                    if (!r36sx_cpu_check_segment_access(ea - useseg_base, 6u, 0)) {
                        return true;
                    }
                    uint32_t target_ip = readdw86(ea);
                    uint16_t target_cs = readw86(ea + 4);
                    if (r36sx_cpu_protected_enabled()) {
                        r36sx_cpu_protected_far_jump(target_cs, target_ip,
                                                     fault_ip);
                        return true;
                    }
                    r36sx_cpu_load_segment(regcs, target_cs);
                    r36sx_cpu_set_ip(target_ip);
                    return true;
                }
                case 6: /* PUSH Ev */
                    push32(readrm32(rm));
                    return true;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return true;
    }

    return false;
}

static __not_in_flash() void r36sx_cpu_exec_bit_test(uint8_t operation,
                                                     uint32_t bit_offset,
                                                     uint8_t register_offset)
{
    uint8_t width = operandSizeOverride ? 32u : 16u;
    uint8_t bit = (uint8_t)(bit_offset & (uint32_t)(width - 1u));
    uint32_t mask = 1u << bit;
    uint32_t value;

    if (mode < 3) {
        getea(rm);
        if (register_offset) {
            ea += (bit_offset / width) * (uint32_t)(width / 8u);
        }
        if (!r36sx_cpu_check_segment_access(
                ea - useseg_base, (uint32_t)(width / 8u),
                operation != 0u)) {
            return;
        }
        value = operandSizeOverride ? readdw86(ea) : readw86(ea);
    } else {
        value = operandSizeOverride ? getreg32(rm) : getreg16(rm);
    }

    cf = (value & mask) != 0;
    switch (operation) {
        case 1: /* BTS */
            value |= mask;
            break;
        case 2: /* BTR */
            value &= ~mask;
            break;
        case 3: /* BTC */
            value ^= mask;
            break;
        default: /* BT */
            return;
    }

    if (mode < 3) {
        if (operandSizeOverride) {
            writedw86(ea, value);
        } else {
            writew86(ea, (uint16_t)value);
        }
    } else if (operandSizeOverride) {
        putreg32(rm, value);
    } else {
        putreg16(rm, (uint16_t)value);
    }
}

static __not_in_flash() void r36sx_cpu_exec_double_shift(uint8_t shift_right,
                                                         uint8_t count)
{
    uint8_t width = operandSizeOverride ? 32u : 16u;

    count &= 0x1fu;
    if (count == 0) {
        return;
    }

    uint32_t dest = operandSizeOverride ? readrm32(rm) : readrm16(rm);
    uint32_t source = operandSizeOverride ? getreg32(reg) : getreg16(reg);
    uint32_t result;

    /*
     * Intel documents SHLD/SHRD counts greater than or equal to operand width
     * as undefined. Keep the emulator deterministic and avoid C undefined
     * shifts; normal software should use the count range below.
     */
    if (count >= width) {
        uint8_t over = (uint8_t)(count - width);
        if (shift_right) {
            cf = (dest >> (width - 1u)) & 1u;
            result = source >> over;
        } else {
            cf = dest & 1u;
            result = source << over;
        }
    } else if (shift_right) {
        cf = (dest >> (count - 1u)) & 1u;
        result = (dest >> count) | (source << (width - count));
    } else {
        cf = (dest >> (width - count)) & 1u;
        result = (dest << count) | (source >> (width - count));
    }

    if (operandSizeOverride) {
        writerm32(rm, result);
        flag_szp32(result);
    } else {
        writerm16(rm, (uint16_t)result);
        flag_szp16((uint16_t)result);
    }
}

static __not_in_flash() void r36sx_cpu_load_far_data_pointer(uint8_t segid,
                                                             uint32_t fault_ip)
{
    if (mode == R36SX_MODRM_MOD_REGISTER) {
        /* LSS/LFS/LGS load a far pointer from memory, never from a register. */
        r36sx_cpu_invalid_opcode(fault_ip);
        return;
    }

    getea(rm);
    uint8_t pointer_size = operandSizeOverride ? 6u : 4u;
    if (!r36sx_cpu_check_segment_access(ea - useseg_base, pointer_size, 0)) {
        return;
    }

    uint32_t offset = operandSizeOverride ? readdw86(ea) : readw86(ea);
    uint16_t selector = readw86(ea + (operandSizeOverride ? 4u : 2u));
    if (!r36sx_cpu_load_segment(segid, selector)) {
        return;
    }

    if (operandSizeOverride) {
        putreg32(reg, offset);
    } else {
        putreg16(reg, (uint16_t)offset);
    }
}

static inline uint8_t r36sx_cpu_debug_register_valid(uint8_t index)
{
    return index < R36SX_386_REGISTER_COUNT && index != 4u && index != 5u;
}

static inline uint8_t r36sx_cpu_test_register_valid(uint8_t index)
{
    return index >= R36SX_386_TEST_REGISTER_FIRST &&
           index <= R36SX_386_TEST_REGISTER_LAST;
}

static inline uint32_t r36sx_cpu_read_debug_register(uint8_t index)
{
    /*
     * Keep 386 reserved bits visible as ones where DOS extenders and
     * diagnostic tools commonly probe them; breakpoint side effects are still
     * tracked as a separate TODO.
     */
    if (index == 6u) {
        return r36sx_dr[6] | R36SX_DR6_RESET;
    }
    if (index == 7u) {
        return r36sx_dr[7] | R36SX_DR7_RESET;
    }
    return r36sx_dr[index];
}

static __not_in_flash() void r36sx_cpu_mov_debug_register(uint8_t to_debug,
                                                          uint32_t fault_ip)
{
    modregrm();
    if (mode != R36SX_MODRM_MOD_REGISTER ||
        !r36sx_cpu_debug_register_valid(reg)) {
        r36sx_cpu_invalid_opcode(fault_ip);
        return;
    }
    if (!r36sx_cpu_require_cpl0(fault_ip)) {
        return;
    }

    if (to_debug) {
        r36sx_dr[reg] = getreg32(rm);
    } else {
        putreg32(rm, r36sx_cpu_read_debug_register(reg));
    }
}

static __not_in_flash() void r36sx_cpu_mov_test_register(uint8_t to_test,
                                                         uint32_t fault_ip)
{
    modregrm();
    if (mode != R36SX_MODRM_MOD_REGISTER ||
        !r36sx_cpu_test_register_valid(reg)) {
        r36sx_cpu_invalid_opcode(fault_ip);
        return;
    }
    if (!r36sx_cpu_require_cpl0(fault_ip)) {
        return;
    }

    if (to_test) {
        r36sx_tr[reg] = getreg32(rm);
    } else {
        putreg32(rm, r36sx_tr[reg]);
    }
}

static __not_in_flash() void r36sx_cpu_exec_0f(uint32_t fault_ip)
{
    uint8_t op2 = getmem8(CPU_CS, CPU_IP);
    StepIP(1);

    if (!r36sx_pico286_cpu_model_at_least(R36SX_PICO286_CPU_80386) &&
        op2 != 0x00 && op2 != 0x01 && op2 != 0x02 &&
        op2 != 0x03 && op2 != 0x06) {
        r36sx_cpu_invalid_opcode(fault_ip);
        return;
    }

    if (op2 >= 0x80 && op2 <= 0x8F) {
        uint8_t take = r36sx_cpu_condition(op2);
        if (operandSizeOverride) {
            int32_t rel = (int32_t)getmem32(CPU_CS, CPU_IP);
            StepIP(4);
            if (take) {
                r36sx_cpu_add_ip(rel);
            }
        } else {
            int16_t rel = (int16_t)getmem16(CPU_CS, CPU_IP);
            StepIP(2);
            if (take) {
                r36sx_cpu_add_ip(rel);
            }
        }
        return;
    }

    if (op2 >= 0x90 && op2 <= 0x9F) {
        modregrm();
        writerm8(rm, r36sx_cpu_condition(op2) ? 1u : 0u);
        return;
    }

    switch (op2) {
        case 0x00: { /* SLDT/STR/LLDT/LTR/VERR/VERW */
            if (!r36sx_cpu_native_protected_enabled()) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            modregrm();
            switch (reg) {
                case 0: /* SLDT Ew */
                    writerm16(rm, r36sx_ldtr_selector);
                    return;
                case 1: /* STR Ew */
                    writerm16(rm, r36sx_tr_selector);
                    return;
                case 2: { /* LLDT Ew */
                    if (!r36sx_cpu_require_cpl0(fault_ip)) {
                        return;
                    }
                    uint16_t selector = readrm16(rm);
                    R36SX_PM_DIAG_LOG(
                        "[PM] LLDT selector=%04X cs:eip=%04X:%08lX",
                        selector, CPU_CS, (unsigned long)CPU_IP);
                    if (!r36sx_cpu_load_ldtr(selector, fault_ip)) {
                        r36sx_pm_diag_log_first_fault("LLDT failed", fault_ip);
                    }
                    return;
                }
                case 3: { /* LTR Ew */
                    if (!r36sx_cpu_require_cpl0(fault_ip)) {
                        return;
                    }
                    uint16_t selector = readrm16(rm);
                    R36SX_PM_DIAG_LOG(
                        "[PM] LTR selector=%04X cs:eip=%04X:%08lX",
                        selector, CPU_CS, (unsigned long)CPU_IP);
                    if (!r36sx_cpu_load_tr(selector, fault_ip)) {
                        r36sx_pm_diag_log_first_fault("LTR failed", fault_ip);
                    }
                    return;
                }
                case 4: /* VERR Ew */
                case 5: { /* VERW Ew */
                    r36sx_segment_cache_t cache;
                    uint16_t selector = readrm16(rm);
                    uint8_t ok = r36sx_cpu_decode_descriptor(selector, &cache) &&
                                 r36sx_cpu_descriptor_visible_for_validation(
                                     selector, &cache) &&
                                 (cache.access & R36SX_DESCRIPTOR_CODE_DATA);
                    if (ok && reg == 4) {
                        ok = ((cache.access & R36SX_DESCRIPTOR_EXECUTABLE) == 0) ||
                             (cache.access & R36SX_DESCRIPTOR_READABLE);
                    } else if (ok) {
                        ok = ((cache.access & R36SX_DESCRIPTOR_EXECUTABLE) == 0) &&
                             (cache.access & R36SX_DESCRIPTOR_WRITABLE);
                    }
                    zf = ok ? 1u : 0u;
                    return;
                }
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x01: { /* SGDT/SIDT/LGDT/LIDT/SMSW/LMSW */
            modregrm();
            switch (reg) {
                case 0: /* SGDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_store_descriptor_table(
                        ea, r36sx_gdtr_limit, r36sx_gdtr_base);
                    return;
                case 1: /* SIDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_store_descriptor_table(
                        ea, r36sx_idtr_limit, r36sx_idtr_base);
                    return;
                case 2: /* LGDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    if (!r36sx_cpu_require_cpl0(fault_ip)) {
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_load_descriptor_table(
                        ea, &r36sx_gdtr_limit, &r36sx_gdtr_base,
                        operandSizeOverride ? 0xffffffffu : 0x00ffffffu);
                    R36SX_PM_DIAG_LOG(
                        "[PM] LGDT ea=%08lX gdtr=%08lX:%04X op32=%u "
                        "cs:eip=%04X:%08lX",
                        (unsigned long)ea,
                        (unsigned long)r36sx_gdtr_base, r36sx_gdtr_limit,
                        operandSizeOverride, CPU_CS, (unsigned long)CPU_IP);
                    return;
                case 3: /* LIDT Ms */
                    if (mode == 3) {
                        r36sx_cpu_invalid_opcode(fault_ip);
                        return;
                    }
                    if (!r36sx_cpu_require_cpl0(fault_ip)) {
                        return;
                    }
                    getea(rm);
                    r36sx_cpu_load_descriptor_table(
                        ea, &r36sx_idtr_limit, &r36sx_idtr_base,
                        operandSizeOverride ? 0xffffffffu : 0x00ffffffu);
                    R36SX_PM_DIAG_LOG(
                        "[PM] LIDT ea=%08lX idtr=%08lX:%04X op32=%u "
                        "cs:eip=%04X:%08lX",
                        (unsigned long)ea,
                        (unsigned long)r36sx_idtr_base, r36sx_idtr_limit,
                        operandSizeOverride, CPU_CS, (unsigned long)CPU_IP);
                    return;
                case 4: /* SMSW Ew */
                    writerm16(rm, (uint16_t)r36sx_cr0);
                    return;
                case 6: /* LMSW Ew */
                    if (!r36sx_cpu_require_cpl0(fault_ip)) {
                        return;
                    }
                    r36sx_cpu_lmsw(readrm16(rm));
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x02: /* LAR Gv,Ew */
        case 0x03: { /* LSL Gv,Ew */
            if (!r36sx_cpu_native_protected_enabled() ||
                r36sx_pico286_cpu_model() == R36SX_PICO286_CPU_8086) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            modregrm();
            r36sx_segment_cache_t cache;
            uint16_t selector = readrm16(rm);
            uint8_t ok = r36sx_cpu_decode_descriptor_any(selector, &cache) &&
                         r36sx_cpu_descriptor_visible_for_validation(
                             selector, &cache);
            if (ok && op2 == 0x02u) {
                ok = r36sx_cpu_descriptor_type_valid_for_lar(&cache);
            } else if (ok) {
                ok = r36sx_cpu_descriptor_type_valid_for_lsl(&cache);
            }

            if (!ok) {
                zf = 0;
                return;
            }

            uint32_t value = op2 == 0x02u
                ? r36sx_cpu_descriptor_access_rights(&cache)
                : cache.limit;
            if (operandSizeOverride) {
                putreg32(reg, value);
            } else {
                putreg16(reg, (uint16_t)value);
            }
            zf = 1;
            return;
        }

        case 0x06: /* CLTS */
            if (!r36sx_cpu_require_cpl0(fault_ip)) {
                return;
            }
            r36sx_cr0 &= ~R36SX_CR0_TS;
            return;

        case 0x20: { /* MOV Rd,Cd */
            modregrm();
            if (mode != R36SX_MODRM_MOD_REGISTER) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            if (!r36sx_cpu_require_cpl0(fault_ip)) {
                return;
            }
            switch (reg) {
                case 0:
                    putreg32(rm, r36sx_cpu_read_cr0());
                    return;
                case 2:
                    putreg32(rm, r36sx_cr2);
                    return;
                case 3:
                    putreg32(rm, r36sx_cr3 & R36SX_CR3_PAGE_DIRECTORY_MASK);
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x21: /* MOV Rd,Dd */
            r36sx_cpu_mov_debug_register(0, fault_ip);
            return;

        case 0x22: { /* MOV Cd,Rd */
            modregrm();
            if (mode != R36SX_MODRM_MOD_REGISTER) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            if (!r36sx_cpu_require_cpl0(fault_ip)) {
                return;
            }
            switch (reg) {
                case 0:
                    r36sx_cpu_set_cr0(getreg32(rm));
                    return;
                case 2:
                    r36sx_cr2 = getreg32(rm);
                    return;
                case 3:
                    r36sx_cr3 = getreg32(rm) & R36SX_CR3_PAGE_DIRECTORY_MASK;
                    return;
            }
            r36sx_cpu_invalid_opcode(fault_ip);
            return;
        }

        case 0x23: /* MOV Dd,Rd */
            r36sx_cpu_mov_debug_register(1, fault_ip);
            return;

        case 0x24: /* MOV Rd,Td */
            r36sx_cpu_mov_test_register(0, fault_ip);
            return;

        case 0x26: /* MOV Td,Rd */
            r36sx_cpu_mov_test_register(1, fault_ip);
            return;

        case 0xA0:
            r36sx_cpu_push_segment_selector(CPU_FS);
            return;

        case 0xA1:
            r36sx_cpu_load_segment(regfs,
                                   r36sx_cpu_pop_segment_selector());
            return;

        case 0xA8:
            r36sx_cpu_push_segment_selector(CPU_GS);
            return;

        case 0xA9:
            r36sx_cpu_load_segment(reggs,
                                   r36sx_cpu_pop_segment_selector());
            return;

        case 0xA3: /* BT Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                0, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xA4: { /* SHLD Ev,Gv,Ib */
            modregrm();
            uint8_t count = getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            r36sx_cpu_exec_double_shift(0, count);
            return;
        }

        case 0xA5: /* SHLD Ev,Gv,CL */
            modregrm();
            r36sx_cpu_exec_double_shift(0, CPU_CL);
            return;

        case 0xAB: /* BTS Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                1, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xAC: { /* SHRD Ev,Gv,Ib */
            modregrm();
            uint8_t count = getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            r36sx_cpu_exec_double_shift(1, count);
            return;
        }

        case 0xAD: /* SHRD Ev,Gv,CL */
            modregrm();
            r36sx_cpu_exec_double_shift(1, CPU_CL);
            return;

        case 0xAF: {
            modregrm();
            if (operandSizeOverride) {
                int64_t result = (int64_t)(int32_t)getreg32(reg) *
                                 (int64_t)(int32_t)readrm32(rm);
                putreg32(reg, (uint32_t)result);
                if (result != (int64_t)(int32_t)(uint32_t)result) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
            } else {
                int32_t result = (int32_t)(int16_t)getreg16(reg) *
                                 (int32_t)(int16_t)readrm16(rm);
                putreg16(reg, (uint16_t)result);
                if (result != (int32_t)(int16_t)(uint16_t)result) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
            }
            return;
        }

        case 0xB2: /* LSS Gv,Mp */
            modregrm();
            r36sx_cpu_load_far_data_pointer(regss, fault_ip);
            return;

        case 0xB4: /* LFS Gv,Mp */
            modregrm();
            r36sx_cpu_load_far_data_pointer(regfs, fault_ip);
            return;

        case 0xB5: /* LGS Gv,Mp */
            modregrm();
            r36sx_cpu_load_far_data_pointer(reggs, fault_ip);
            return;

        case 0xB3: /* BTR Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                2, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xB6:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, readrm8(rm));
            } else {
                putreg16(reg, readrm8(rm));
            }
            return;

        case 0xB7:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, readrm16(rm));
            } else {
                putreg16(reg, readrm16(rm));
            }
            return;

        case 0xBA: { /* BT/BTS/BTR/BTC Ev,Ib */
            modregrm();
            if (reg < 4 || reg > 7) {
                r36sx_cpu_invalid_opcode(fault_ip);
                return;
            }
            uint8_t bit = getmem8(CPU_CS, CPU_IP);
            StepIP(1);
            r36sx_cpu_exec_bit_test((uint8_t)(reg - 4u), bit, 0);
            return;
        }

        case 0xBB: /* BTC Ev,Gv */
            modregrm();
            r36sx_cpu_exec_bit_test(
                3, operandSizeOverride ? getreg32(reg) : getreg16(reg), 1);
            return;

        case 0xBC: {
            modregrm();
            uint32_t value = operandSizeOverride ? readrm32(rm) : readrm16(rm);
            if (!value) {
                zf = 1;
                return;
            }
            zf = 0;
            uint8_t bit = 0;
            while (((value >> bit) & 1u) == 0) {
                bit++;
            }
            if (operandSizeOverride) {
                putreg32(reg, bit);
            } else {
                putreg16(reg, bit);
            }
            return;
        }

        case 0xBD: {
            modregrm();
            uint32_t value = operandSizeOverride ? readrm32(rm) : readrm16(rm);
            if (!value) {
                zf = 1;
                return;
            }
            zf = 0;
            uint8_t bit = operandSizeOverride ? 31u : 15u;
            while (((value >> bit) & 1u) == 0) {
                bit--;
            }
            if (operandSizeOverride) {
                putreg32(reg, bit);
            } else {
                putreg16(reg, bit);
            }
            return;
        }

        case 0xBE:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, (uint32_t)(int32_t)(int8_t)readrm8(rm));
            } else {
                putreg16(reg, (uint16_t)(int16_t)(int8_t)readrm8(rm));
            }
            return;

        case 0xBF:
            modregrm();
            if (operandSizeOverride) {
                putreg32(reg, (uint32_t)(int32_t)(int16_t)readrm16(rm));
            } else {
                putreg16(reg, readrm16(rm));
            }
            return;
    }

    r36sx_cpu_invalid_opcode(fault_ip);
}
