/*
 * Parameterized x86 interpreter loop.
 *
 * r36sx_cpu.c includes this file with R36SX_CPU_EXEC_CORE_NAME and
 * per-model helper macros configured for the target CPU model.
 */

#ifndef R36SX_CPU_EXEC_CORE_NAME
#error R36SX_CPU_EXEC_CORE_NAME must be defined before including r36sx_cpu_exec_core.inl.
#endif

#ifndef R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
#define R36SX_CPU_CORE_HAS_386_EXTENDED_OPS CPU_386_EXTENDED_OPS
#endif

#ifndef R36SX_CPU_CORE_8086_ONLY
#define R36SX_CPU_CORE_8086_ONLY 0
#endif

#ifndef R36SX_CPU_CORE_FIXED_16BIT
#define R36SX_CPU_CORE_FIXED_16BIT R36SX_CPU_CORE_8086_ONLY
#endif

#ifndef R36SX_CPU_CORE_HAS_386_DEBUG_REGS
#define R36SX_CPU_CORE_HAS_386_DEBUG_REGS (!R36SX_CPU_CORE_FIXED_16BIT)
#endif

#ifndef R36SX_CPU_CORE_COMPUTED_GOTO
#define R36SX_CPU_CORE_COMPUTED_GOTO R36SX_CPU_COMPUTED_GOTO
#endif

#ifndef R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE
#define R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE operandSizeOverride
#endif

#ifndef R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE
#define R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE addressSizeOverride
#endif

#ifndef R36SX_CPU_CORE_LIMIT_SHIFT_COUNT
#define R36SX_CPU_CORE_LIMIT_SHIFT_COUNT (!R36SX_CPU_CORE_8086_ONLY)
#define R36SX_CPU_CORE_LIMIT_SHIFT_COUNT_DEFAULTED 1
#endif

#if R36SX_CPU_CORE_LIMIT_SHIFT_COUNT
#define R36SX_CPU_CORE_SHIFT_COUNT(count) ((uint8_t)((count) & 0x1fu))
#else
#define R36SX_CPU_CORE_SHIFT_COUNT(count) ((uint8_t)(count))
#endif

static void __not_in_flash() R36SX_CPU_EXEC_CORE_NAME(uint32_t execloops) {
    static uint32_t firstip;
    static bool was_TF;
    uint32_t loopcount = 0;

    //counterticks = (uint64_t) ( (double) timerfreq / (double) 65536.0);
    //tickssource();
    for (loopcount = 0; loopcount < execloops; loopcount++) {
        uint8_t maskable_irq_shadowed = r36sx_cpu_maskable_interrupt_shadow;
        if (unlikely(hltstate)) {
            if (unlikely(ifl && !maskable_irq_shadowed &&
                         r36sx_cpu_pending_maskable_irq())) {
                hltstate = 0;
                intcall86(nextintr());
            } else {
                r36sx_app_stats_record_x86(loopcount);
                return;
            }
        } else if (unlikely(ifl && !maskable_irq_shadowed &&
                            r36sx_cpu_pending_maskable_irq())) {
            intcall86(nextintr()); // get next interrupt from the i8259, if any d
        }
#if PICO_ON_DEVICE
        if (ask_to_blast) {
            ask_to_blast = false;
            last_sb_sample = blaster_sample();
        }
#endif
        reptype = 0;
        segoverride = 0;
        lockPrefix = 0;
#if R36SX_CPU_CORE_FIXED_16BIT
        operandSizeOverride = false;
        addressSizeOverride = false;
#else
        R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE = r36sx_cpu_code_default32();
        R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE = r36sx_cpu_code_default32();
#endif
        r36sx_cpu_use_segment(regds);
        uint8_t docontinue = 0;
        uint8_t prefix_exception = 0;
        firstip = CPU_IP;
        r36sx_cpu_fault_ip_context = firstip;
        r36sx_cpu_exception_pending = 0u;
        register uint8_t opcode;

#if R36SX_CPU_CORE_HAS_386_DEBUG_REGS
        if (unlikely(r36sx_cpu_debug_check_execute_breakpoint(firstip))) {
            continue;
        }
#endif

        while (!docontinue) {
            ///         CPU_CS &= 0xFFFF;
            ///         CPU_IP &= 0xFFFF;
            //            savecs = CPU_CS;
            //            saveip = ip;
            // W/A-hack: last byte of interrupts table (actually should not be ever used as CS:IP)
            if (unlikely(CPU_CS == XMS_FN_CS && ip == XMS_FN_IP)) {
                // hook for XMS
                opcode = xms_handler(); // always returns RET TODO: far/short ret?
            } else {
                opcode = getmem8(CPU_CS, CPU_IP);
            }

            StepIP(1);

            switch (opcode) {
                /* segment prefix check */
                case 0x2E: /* segment CPU_CS */
                    r36sx_cpu_use_segment(regcs);
                    segoverride = 1;
                    break;

                case 0x3E: /* segment CPU_DS */
                    r36sx_cpu_use_segment(regds);
                    segoverride = 1;
                    break;

                case 0x26: /* segment CPU_ES */
                    r36sx_cpu_use_segment(reges);
                    segoverride = 1;
                    break;

                case 0x36: /* segment CPU_SS */
                    r36sx_cpu_use_segment(regss);
                    segoverride = 1;
                    break;

#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
                case 0x64: /* segment CPU_FS */
                    if (!r36sx_cpu_has_80386_features()) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    r36sx_cpu_use_segment(regfs);
                    segoverride = 1;
                    break;

                case 0x65: /* segment CPU_GS */
                    if (!r36sx_cpu_has_80386_features()) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    r36sx_cpu_use_segment(reggs);
                    segoverride = 1;
                    break;
#else
                case 0x64:
                case 0x65:
                    r36sx_cpu_set_ip(firstip);
                    intcall86(6);
                    prefix_exception = 1;
                    docontinue = 1;
                    break;
#endif

#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
                case 0x66: /* operand-size override */
                    if (!r36sx_cpu_has_80386_features()) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE = !R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE;
                    break;

                case 0x67: /* address-size override */
                    if (!r36sx_cpu_has_80386_features()) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(6);
                        prefix_exception = 1;
                        docontinue = 1;
                        break;
                    }
                    R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE = !R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE;
                    break;
#endif

                case 0xF0: /* LOCK */
                    if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                        prefix_exception = 1;
                        docontinue = 1;
                    }
                    lockPrefix = 1;
                    break;

                case 0xF2: /* REPNE/REPNZ */
                    reptype = 2;
                    break;

                /* repetition prefix check */
                case 0xF3: /* REP/REPE/REPZ */
                    reptype = 1;
                    break;

                default:
                    docontinue = 1;
                    break;
            }
        }
        if (prefix_exception) {
            continue;
        }
        if (lockPrefix && !r36sx_cpu_lock_prefix_allowed(opcode)) {
            r36sx_cpu_invalid_opcode(firstip);
            continue;
        }

        r36sx_cpu_debug_trace_test386_instruction(firstip, opcode);

        register uint32_t res32;
        register uint8_t res8;
        register uint8_t oper1b;
        register uint8_t oper2b;
#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
        if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE &&
            r36sx_cpu_exec_operand32_opcode(opcode, firstip, execloops,
                                            &loopcount, tf || was_TF)) {
            goto r36sx_opcode_done;
        }
#endif
#if R36SX_CPU_CORE_COMPUTED_GOTO
        /* GNU labels-as-values remove the large opcode switch from the hot path. */
        static void *const r36sx_opcode_dispatch[256] = {
            &&r36sx_opcode_00, &&r36sx_opcode_01, &&r36sx_opcode_02, &&r36sx_opcode_03, &&r36sx_opcode_04, &&r36sx_opcode_05, &&r36sx_opcode_06, &&r36sx_opcode_07,
            &&r36sx_opcode_08, &&r36sx_opcode_09, &&r36sx_opcode_0A, &&r36sx_opcode_0B, &&r36sx_opcode_0C, &&r36sx_opcode_0D, &&r36sx_opcode_0E, &&r36sx_opcode_0F,
            &&r36sx_opcode_10, &&r36sx_opcode_11, &&r36sx_opcode_12, &&r36sx_opcode_13, &&r36sx_opcode_14, &&r36sx_opcode_15, &&r36sx_opcode_16, &&r36sx_opcode_17,
            &&r36sx_opcode_18, &&r36sx_opcode_19, &&r36sx_opcode_1A, &&r36sx_opcode_1B, &&r36sx_opcode_1C, &&r36sx_opcode_1D, &&r36sx_opcode_1E, &&r36sx_opcode_1F,
            &&r36sx_opcode_20, &&r36sx_opcode_21, &&r36sx_opcode_22, &&r36sx_opcode_23, &&r36sx_opcode_24, &&r36sx_opcode_25, &&r36sx_opcode_default, &&r36sx_opcode_27,
            &&r36sx_opcode_28, &&r36sx_opcode_29, &&r36sx_opcode_2A, &&r36sx_opcode_2B, &&r36sx_opcode_2C, &&r36sx_opcode_2D, &&r36sx_opcode_default, &&r36sx_opcode_2F,
            &&r36sx_opcode_30, &&r36sx_opcode_31, &&r36sx_opcode_32, &&r36sx_opcode_33, &&r36sx_opcode_34, &&r36sx_opcode_35, &&r36sx_opcode_default, &&r36sx_opcode_37,
            &&r36sx_opcode_38, &&r36sx_opcode_39, &&r36sx_opcode_3A, &&r36sx_opcode_3B, &&r36sx_opcode_3C, &&r36sx_opcode_3D, &&r36sx_opcode_default, &&r36sx_opcode_3F,
            &&r36sx_opcode_40, &&r36sx_opcode_41, &&r36sx_opcode_42, &&r36sx_opcode_43, &&r36sx_opcode_44, &&r36sx_opcode_45, &&r36sx_opcode_46, &&r36sx_opcode_47,
            &&r36sx_opcode_48, &&r36sx_opcode_49, &&r36sx_opcode_4A, &&r36sx_opcode_4B, &&r36sx_opcode_4C, &&r36sx_opcode_4D, &&r36sx_opcode_4E, &&r36sx_opcode_4F,
            &&r36sx_opcode_50, &&r36sx_opcode_51, &&r36sx_opcode_52, &&r36sx_opcode_53, &&r36sx_opcode_54, &&r36sx_opcode_55, &&r36sx_opcode_56, &&r36sx_opcode_57,
            &&r36sx_opcode_58, &&r36sx_opcode_59, &&r36sx_opcode_5A, &&r36sx_opcode_5B, &&r36sx_opcode_5C, &&r36sx_opcode_5D, &&r36sx_opcode_5E, &&r36sx_opcode_5F,
#if R36SX_CPU_CORE_8086_ONLY
            &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default,
            &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default,
#else
            &&r36sx_opcode_60, &&r36sx_opcode_61, &&r36sx_opcode_62, &&r36sx_opcode_63, &&r36sx_opcode_default, &&r36sx_opcode_default,
#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
            &&r36sx_opcode_66, &&r36sx_opcode_67,
#else
            &&r36sx_opcode_default, &&r36sx_opcode_default,
#endif
            &&r36sx_opcode_68, &&r36sx_opcode_69, &&r36sx_opcode_6A, &&r36sx_opcode_6B, &&r36sx_opcode_6C, &&r36sx_opcode_6D, &&r36sx_opcode_6E, &&r36sx_opcode_6F,
#endif
            &&r36sx_opcode_70, &&r36sx_opcode_71, &&r36sx_opcode_72, &&r36sx_opcode_73, &&r36sx_opcode_74, &&r36sx_opcode_75, &&r36sx_opcode_76, &&r36sx_opcode_77,
            &&r36sx_opcode_78, &&r36sx_opcode_79, &&r36sx_opcode_7A, &&r36sx_opcode_7B, &&r36sx_opcode_7C, &&r36sx_opcode_7D, &&r36sx_opcode_7E, &&r36sx_opcode_7F,
            &&r36sx_opcode_80, &&r36sx_opcode_81, &&r36sx_opcode_82, &&r36sx_opcode_83, &&r36sx_opcode_84, &&r36sx_opcode_85, &&r36sx_opcode_86, &&r36sx_opcode_87,
            &&r36sx_opcode_88, &&r36sx_opcode_89, &&r36sx_opcode_8A, &&r36sx_opcode_8B, &&r36sx_opcode_8C, &&r36sx_opcode_8D, &&r36sx_opcode_8E, &&r36sx_opcode_8F,
            &&r36sx_opcode_90, &&r36sx_opcode_91, &&r36sx_opcode_92, &&r36sx_opcode_93, &&r36sx_opcode_94, &&r36sx_opcode_95, &&r36sx_opcode_96, &&r36sx_opcode_97,
            &&r36sx_opcode_98, &&r36sx_opcode_99, &&r36sx_opcode_9A, &&r36sx_opcode_9B, &&r36sx_opcode_9C, &&r36sx_opcode_9D, &&r36sx_opcode_9E, &&r36sx_opcode_9F,
            &&r36sx_opcode_A0, &&r36sx_opcode_A1, &&r36sx_opcode_A2, &&r36sx_opcode_A3, &&r36sx_opcode_A4, &&r36sx_opcode_A5, &&r36sx_opcode_A6, &&r36sx_opcode_A7,
            &&r36sx_opcode_A8, &&r36sx_opcode_A9, &&r36sx_opcode_AA, &&r36sx_opcode_AB, &&r36sx_opcode_AC, &&r36sx_opcode_AD, &&r36sx_opcode_AE, &&r36sx_opcode_AF,
            &&r36sx_opcode_B0, &&r36sx_opcode_B1, &&r36sx_opcode_B2, &&r36sx_opcode_B3, &&r36sx_opcode_B4, &&r36sx_opcode_B5, &&r36sx_opcode_B6, &&r36sx_opcode_B7,
            &&r36sx_opcode_B8, &&r36sx_opcode_B9, &&r36sx_opcode_BA, &&r36sx_opcode_BB, &&r36sx_opcode_BC, &&r36sx_opcode_BD, &&r36sx_opcode_BE, &&r36sx_opcode_BF,
            &&r36sx_opcode_C0, &&r36sx_opcode_C1, &&r36sx_opcode_C2, &&r36sx_opcode_C3, &&r36sx_opcode_C4, &&r36sx_opcode_C5, &&r36sx_opcode_C6, &&r36sx_opcode_C7,
            &&r36sx_opcode_C8, &&r36sx_opcode_C9, &&r36sx_opcode_CA, &&r36sx_opcode_CB, &&r36sx_opcode_CC, &&r36sx_opcode_CD, &&r36sx_opcode_CE, &&r36sx_opcode_CF,
            &&r36sx_opcode_D0, &&r36sx_opcode_D1, &&r36sx_opcode_D2, &&r36sx_opcode_D3, &&r36sx_opcode_D4, &&r36sx_opcode_D5, &&r36sx_opcode_D6, &&r36sx_opcode_D7,
            &&r36sx_opcode_D8, &&r36sx_opcode_D9, &&r36sx_opcode_DA, &&r36sx_opcode_DB, &&r36sx_opcode_DC, &&r36sx_opcode_DD, &&r36sx_opcode_DE, &&r36sx_opcode_DF,
            &&r36sx_opcode_E0, &&r36sx_opcode_E1, &&r36sx_opcode_E2, &&r36sx_opcode_E3, &&r36sx_opcode_E4, &&r36sx_opcode_E5, &&r36sx_opcode_E6, &&r36sx_opcode_E7,
            &&r36sx_opcode_E8, &&r36sx_opcode_E9, &&r36sx_opcode_EA, &&r36sx_opcode_EB, &&r36sx_opcode_EC, &&r36sx_opcode_ED, &&r36sx_opcode_EE, &&r36sx_opcode_EF,
            &&r36sx_opcode_F0, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_default, &&r36sx_opcode_F4, &&r36sx_opcode_F5, &&r36sx_opcode_F6, &&r36sx_opcode_F7,
            &&r36sx_opcode_F8, &&r36sx_opcode_F9, &&r36sx_opcode_FA, &&r36sx_opcode_FB, &&r36sx_opcode_FC, &&r36sx_opcode_FD, &&r36sx_opcode_FE, &&r36sx_opcode_FF,
        };
        goto *r36sx_opcode_dispatch[opcode];
#endif
        switch (opcode) {
            case 0x0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_00: ;
#endif
                /* 00 ADD Eb Gb */
                modregrm();
                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_add8();
                writerm8(rm, res8);
                break;

            case 0x1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_01: ;
#endif
                /* 01 ADD Ev Gv */
                modregrm();
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    register uint32_t oper1 = readrm32(rm);
                    register uint32_t oper2 = getreg32(reg);
                    op_add32();
                    writerm32(rm, res32);
                } else {
                    register uint32_t oper1 = readrm16(rm);
                    register uint32_t oper2 = getreg16(reg);
                    op_add16();
                    writerm16(rm, res16);
                }
                break;

            case 0x2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_02: ;
#endif
                /* 02 ADD Gb Eb */
                modregrm();
                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_add8();
                putreg8(reg, res8);
                break;

            case 0x3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_03: ;
#endif
                {
                /* 03 ADD Gv Ev */
                modregrm();
                register uint32_t oper1 = getreg16(reg);
                register uint32_t oper2 = readrm16(rm);
                op_add16();
                putreg16(reg, res16);
                break;
            }
            case 0x4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_04: ;
#endif
                /* 04 ADD CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_add8();
                CPU_AL = res8;
                break;

            case 0x5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_05: ;
#endif
                {
                /* 05 ADD eAX Iv */
                register uint32_t oper1 = CPU_AX;
                register uint32_t oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_add16();
                CPU_AX = res16;
                break;
            }
            case 0x6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_06: ;
#endif
                /* 06 PUSH CPU_ES */
                r36sx_cpu_push_segment_selector(CPU_ES);
                break;

            case 0x7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_07: ;
#endif
                /* 07 POP CPU_ES */
                r36sx_cpu_load_segment_at(reges,
                                          r36sx_cpu_pop_segment_selector(),
                                          firstip);
                break;

            case 0x8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_08: ;
#endif
                /* 08 OR Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_or8();
                writerm8(rm, res8
                );
                break;

            case 0x9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_09: ;
#endif
                /* 09 OR Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_or16();
                writerm16(rm, res16
                );
                break;

            case 0xA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0A: ;
#endif
                /* 0A OR Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_or8();
                putreg8(reg, res8
                );
                break;

            case 0xB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0B: ;
#endif
                /* 0B OR Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_or16();
                /*                if ((oper1 == 0xF802) && (oper2 == 0xF802)) {
                                    sf = 0;    *//* cheap hack to make Wolf 3D think we're a 286 so it plays */ /*
                }*/

                putreg16(reg, res16);
                break;

            case 0xC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0C: ;
#endif
                /* 0C OR CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_or8();
                CPU_AL = res8;
                break;

            case 0xD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0D: ;
#endif
                /* 0D OR eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_or16();
                CPU_AX = res16;
                break;

            case 0xE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0E: ;
#endif
                /* 0E PUSH CPU_CS */
                r36sx_cpu_push_segment_selector(CPU_CS);
                break;

            case 0xF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_0F: ;
#endif
                if (r36sx_cpu_strict_8086_mode) {
                    /* 8086/8088 only: 0F POP CS. */
                    r36sx_cpu_load_segment(regcs,
                                           r36sx_cpu_pop_segment_selector());
                } else {
                    r36sx_cpu_exec_0f(firstip);
                }
                break;

            case 0x10:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_10: ;
#endif
                /* 10 ADC Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_adc8();
                writerm8(rm, res8);
                break;

            case 0x11:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_11: ;
#endif
                /* 11 ADC Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_adc16();
                writerm16(rm, res16);
                break;

            case 0x12:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_12: ;
#endif
                /* 12 ADC Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_adc8();
                putreg8(reg, res8);
                break;

            case 0x13:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_13: ;
#endif
                /* 13 ADC Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_adc16();
                putreg16(reg, res16
                );
                break;

            case 0x14:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_14: ;
#endif
                /* 14 ADC CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_adc8();
                CPU_AL = res8;
                break;

            case 0x15:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_15: ;
#endif
                /* 15 ADC eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_adc16();
                CPU_AX = res16;
                break;

            case 0x16:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_16: ;
#endif
                /* 16 PUSH CPU_SS */
                r36sx_cpu_push_segment_selector(CPU_SS);
                break;

            case 0x17:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_17: ;
#endif
                /* 17 POP CPU_SS */
                if (r36sx_cpu_load_segment_at(
                        regss, r36sx_cpu_pop_segment_selector(), firstip)) {
                    r36sx_cpu_delay_maskable_interrupts_one_instruction();
                }
                break;

            case 0x18:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_18: ;
#endif
                /* 18 SBB Eb Gb */
                modregrm();
                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_sbb8();
                writerm8(rm, res8);
                break;

            case 0x19:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_19: ;
#endif
                /* 19 SBB Ev Gv */
                modregrm();
                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_sbb16();
                writerm16(rm, res16);
                break;

            case 0x1A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1A: ;
#endif
                /* 1A SBB Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_sbb8();
                putreg8(reg, res8
                );
                break;

            case 0x1B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1B: ;
#endif
                /* 1B SBB Gv Ev */
                modregrm();
                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_sbb16();
                putreg16(reg, res16);
                break;

            case 0x1C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1C: ;
#endif
                /* 1C SBB CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_sbb8();
                CPU_AL = res8;
                break;

            case 0x1D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1D: ;
#endif
                /* 1D SBB eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_sbb16();
                CPU_AX = res16;
                break;

            case 0x1E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1E: ;
#endif
                /* 1E PUSH CPU_DS */
                r36sx_cpu_push_segment_selector(CPU_DS);
                break;

            case 0x1F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_1F: ;
#endif
                /* 1F POP CPU_DS */
                r36sx_cpu_load_segment_at(regds,
                                          r36sx_cpu_pop_segment_selector(),
                                          firstip);
                break;

            case 0x20:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_20: ;
#endif
                /* 20 AND Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_and8();
                writerm8(rm, res8);
                break;

            case 0x21:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_21: ;
#endif
                /* 21 AND Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_and16();
                writerm16(rm, res16
                );
                break;

            case 0x22:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_22: ;
#endif
                /* 22 AND Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_and8();
                putreg8(reg, res8
                );
                break;

            case 0x23:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_23: ;
#endif
                /* 23 AND Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_and16();
                putreg16(reg, res16
                );
                break;

            case 0x24:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_24: ;
#endif
                /* 24 AND CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_and8();
                CPU_AL = res8;
                break;

            case 0x25:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_25: ;
#endif
                /* 25 AND eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_and16();
                CPU_AX = res16;
                break;

            case 0x27:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_27: ;
#endif
                /* 27 DAA */
            {
                const uint8_t old_al = CPU_AL;
                const uint8_t old_cf = cf;

                /*
                 * Intel DAA decides the high-digit correction from the input
                 * AL/CF state.  The low correction sets AF but must not clear a
                 * carry that was already pending before the instruction.
                 */
                if (((old_al & 0x0F) > 9) || af) {
                    CPU_AL = (uint8_t)(CPU_AL + 0x06);
                    af = 1;
                } else {
                    af = 0;
                }
                if ((old_al > 0x99) || old_cf) {
                    CPU_AL = (uint8_t)(CPU_AL + 0x60);
                    cf = 1;
                } else {
                    cf = 0;
                }
                flag_szp8(CPU_AL);
                break;
            }

            case 0x28:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_28: ;
#endif
                /* 28 SUB Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_sub8();
                writerm8(rm, res8
                );
                break;

            case 0x29:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_29: ;
#endif
                {
                /* 29 SUB Ev Gv */
                modregrm();
                register uint32_t oper1 = readrm16(rm);
                register uint32_t oper2 = getreg16(reg);
                register uint32_t dst = oper1 - oper2;
                flag_szp16((uint16_t) dst);
                cf = (dst & 0xFFFF0000) != 0;
                of = ((dst ^ oper1) & (oper1 ^ oper2) & 0x8000) != 0;
                af = ((oper1 ^ oper2 ^ dst) & 0x10) != 0;
                writerm16(rm, (uint16_t) dst);
                break;
            }
            case 0x2A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_2A: ;
#endif
                /* 2A SUB Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_sub8();
                putreg8(reg, res8
                );
                break;

            case 0x2B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_2B: ;
#endif
                /* 2B SUB Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_sub16();
                putreg16(reg, res16
                );
                break;

            case 0x2C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_2C: ;
#endif
                /* 2C SUB CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_sub8();
                CPU_AL = res8;
                break;

            case 0x2D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_2D: ;
#endif
                /* 2D SUB eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_sub16();
                CPU_AX = res16;
                break;

            case 0x2F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_2F: ;
#endif
                /* 2F DAS */
            {
                const uint8_t old_al = CPU_AL;
                const uint8_t old_cf = cf;
                const uint8_t low_adjust = ((old_al & 0x0F) > 9) || af;

                /*
                 * DAS mirrors DAA: high correction is based on input AL/CF.
                 * If only the low correction borrows from AL, CF remains set.
                 */
                if (low_adjust) {
                    CPU_AL = (uint8_t)(CPU_AL - 0x06);
                    af = 1;
                } else {
                    af = 0;
                }
                if ((old_al > 0x99) || old_cf) {
                    CPU_AL = (uint8_t)(CPU_AL - 0x60);
                    cf = 1;
                } else {
                    cf = low_adjust && (old_al < 0x06);
                }
                flag_szp8(CPU_AL);
                break;
            }

            case 0x30:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_30: ;
#endif
                /* 30 XOR Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                op_xor8();
                writerm8(rm, res8
                );
                break;

            case 0x31:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_31: ;
#endif
                /* 31 XOR Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                op_xor16();
                writerm16(rm, res16
                );
                break;

            case 0x32:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_32: ;
#endif
                /* 32 XOR Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                op_xor8();
                putreg8(reg, res8
                );
                break;

            case 0x33:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_33: ;
#endif
                /* 33 XOR Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                op_xor16();
                putreg16(reg, res16
                );
                break;

            case 0x34:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_34: ;
#endif
                /* 34 XOR CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                op_xor8();
                CPU_AL = res8;
                break;

            case 0x35:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_35: ;
#endif
                /* 35 XOR eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                op_xor16();
                CPU_AX = res16;
                break;

            case 0x37:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_37: ;
#endif
                /* 37 AAA ASCII */
                if (((CPU_AL & 0xF) > 9) || (af == 1)) {
                    CPU_AX = CPU_AX + 0x106;
                    x86_flags.value |= FLAG_CF_AF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_AF_MASK;
                }

                CPU_AL = CPU_AL & 0xF;
                break;

            case 0x38:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_38: ;
#endif
                /* 38 CMP Eb Gb */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getreg8(reg);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x39:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_39: ;
#endif
                /* 39 CMP Ev Gv */
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getreg16(reg);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_3A: ;
#endif
                /* 3A CMP Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x3B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_3B: ;
#endif
                /* 3B CMP Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_3C: ;
#endif
                /* 3C CMP CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                flag_sub8(oper1b, oper2b
                );
                break;

            case 0x3D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_3D: ;
#endif
                /* 3D CMP eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                flag_sub16(oper1, oper2
                );
                break;

            case 0x3F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_3F: ;
#endif
                /* 3F AAS ASCII */
                if (((CPU_AL & 0xF) > 9) || (af == 1)) {
                    CPU_AX = CPU_AX - 6;
                    CPU_AH = CPU_AH - 1;
                    x86_flags.value |= FLAG_CF_AF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_AF_MASK;
                }

                CPU_AL = CPU_AL & 0xF;
                break;

            case 0x40:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_40: ;
#endif
                {
                /* 40 INC eAX */
                register uint32_t oper1 = CPU_AX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_AX = (uint16_t) dst;
                break;
            }
            case 0x41:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_41: ;
#endif
                {
                /* 41 INC eCX */
                register uint32_t oper1 = CPU_CX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_CX = (uint16_t) dst;
                break;
            }
            case 0x42:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_42: ;
#endif
                {
                /* 42 INC eDX */
                register uint32_t oper1 = CPU_DX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_DX = (uint16_t) dst;
                break;
            }
            case 0x43:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_43: ;
#endif
                {
                /* 43 INC eBX */
                register uint32_t oper1 = CPU_BX;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_BX = (uint16_t) dst;
                break;
            }
            case 0x44:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_44: ;
#endif
                {
                /* 44 INC eSP */
                register uint32_t oper1 = CPU_SP;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_SP = (uint16_t) dst;
                break;
            }
            case 0x45:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_45: ;
#endif
                {
                /* 45 INC eBP */
                register uint32_t oper1 = CPU_BP;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_BP = (uint16_t) dst;
                break;
            }
            case 0x46:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_46: ;
#endif
                {
                /* 46 INC eSI */
                register uint32_t oper1 = CPU_SI;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_SI = (uint16_t) dst;
                break;
            }
            case 0x47:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_47: ;
#endif
                {
                /* 47 INC eDI */
                register uint32_t oper1 = CPU_DI;
                register uint32_t dst = oper1 + 1;
                flag_szp16(dst);
                of = (((dst ^ oper1) & (dst ^ 1) & 0x8000) != 0);
                af = (((oper1 ^ 1 ^ dst) & 0x10) != 0);
                CPU_DI = (uint16_t) dst;
                break;
            }
            case 0x48:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_48: ;
#endif
                /* 48 DEC eAX */
                oldcf = cf;
                oper1 = CPU_AX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_AX = res16;
                break;

            case 0x49:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_49: ;
#endif
                /* 49 DEC eCX */
                oldcf = cf;
                oper1 = CPU_CX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_CX = res16;
                break;

            case 0x4A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4A: ;
#endif
                /* 4A DEC eDX */
                oldcf = cf;
                oper1 = CPU_DX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_DX = res16;
                break;

            case 0x4B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4B: ;
#endif
                /* 4B DEC eBX */
                oldcf = cf;
                oper1 = CPU_BX;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_BX = res16;
                break;

            case 0x4C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4C: ;
#endif
                /* 4C DEC eSP */
                oldcf = cf;
                oper1 = CPU_SP;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_SP = res16;
                break;

            case 0x4D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4D: ;
#endif
                /* 4D DEC eBP */
                oldcf = cf;
                oper1 = CPU_BP;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_BP = res16;
                break;

            case 0x4E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4E: ;
#endif
                /* 4E DEC eSI */
                oldcf = cf;
                oper1 = CPU_SI;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_SI = res16;
                break;

            case 0x4F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_4F: ;
#endif
                /* 4F DEC eDI */
                oldcf = cf;
                oper1 = CPU_DI;
                oper2 = 1;
                op_sub16();
                cf = oldcf;
                CPU_DI = res16;
                break;

            case 0x50:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_50: ;
#endif
                /* 50 PUSH eAX */
                push(CPU_AX);
                break;

            case 0x51:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_51: ;
#endif
                /* 51 PUSH eCX */
                push(CPU_CX);
                break;

            case 0x52:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_52: ;
#endif
                /* 52 PUSH eDX */
                push(CPU_DX);
                break;

            case 0x53:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_53: ;
#endif
                /* 53 PUSH eBX */
                push(CPU_BX);
                break;

            case 0x54:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_54: ;
#endif
                /* 54 PUSH eSP */
                if (r36sx_cpu_strict_8086_mode) {
                    push(CPU_SP - 2);
                } else {
                    push(CPU_SP);
                }
                break;

            case 0x55:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_55: ;
#endif
                /* 55 PUSH eBP */
                push(CPU_BP);
                break;

            case 0x56:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_56: ;
#endif
                /* 56 PUSH eSI */
                push(CPU_SI);
                break;

            case 0x57:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_57: ;
#endif
                /* 57 PUSH eDI */
                push(CPU_DI);
                break;

            case 0x58:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_58: ;
#endif
                /* 58 POP eAX */
                CPU_AX = pop();
                break;

            case 0x59:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_59: ;
#endif
                /* 59 POP eCX */
                CPU_CX = pop();
                break;

            case 0x5A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5A: ;
#endif
                /* 5A POP eDX */
                CPU_DX = pop();
                break;

            case 0x5B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5B: ;
#endif
                /* 5B POP eBX */
                CPU_BX = pop();
                break;

            case 0x5C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5C: ;
#endif
                /* 5C POP eSP */
                CPU_SP = pop();
                break;

            case 0x5D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5D: ;
#endif
                /* 5D POP eBP */
                CPU_BP = pop();
                break;

            case 0x5E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5E: ;
#endif
                /* 5E POP eSI */
                CPU_SI = pop();
                break;

            case 0x5F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_5F: ;
#endif
                /* 5F POP eDI */
                CPU_DI = pop();
                break;

#ifndef CPU_8086
            case 0x60:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_60: ;
#endif
                /* 60 PUSHA (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    uint32_t oldesp = CPU_ESP;
                    push32(CPU_EAX);
                    push32(CPU_ECX);
                    push32(CPU_EDX);
                    push32(CPU_EBX);
                    push32(oldesp);
                    push32(CPU_EBP);
                    push32(CPU_ESI);
                    push32(CPU_EDI);
                    break;
                }
                oldsp = CPU_SP;
                push(CPU_AX);
                push(CPU_CX);
                push(CPU_DX);
                push(CPU_BX);
                push(oldsp);
                push(CPU_BP);
                push(CPU_SI);
                push(CPU_DI);
                break;

            case 0x61:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_61: ;
#endif
                /* 61 POPA (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    CPU_EDI = pop32();
                    CPU_ESI = pop32();
                    CPU_EBP = pop32();
                    r36sx_cpu_adjust_stack(4u);
                    CPU_EBX = pop32();
                    CPU_EDX = pop32();
                    CPU_ECX = pop32();
                    CPU_EAX = pop32();
                    break;
                }
                CPU_DI = pop();
                CPU_SI = pop();
                CPU_BP = pop();
                r36sx_cpu_adjust_stack(2u);
                CPU_BX = pop();
                CPU_DX = pop();
                CPU_CX = pop();
                CPU_AX = pop();
                break;

            case 0x62:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_62: ;
#endif
                /* 62 BOUND Gv, Ev (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                /* Intel BOUND requires a memory bounds table operand. */
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                getea(rm);
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, 8u, 0)) {
                        break;
                    }
                    int32_t value = (int32_t)getreg32(reg);
                    int32_t lower = (int32_t)readdw86(ea);
                    int32_t upper = (int32_t)readdw86(ea + 4u);
                    if (value < lower || value > upper) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(R36SX_EXCEPTION_BOUND);
                    }
                } else {
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, 4u, 0)) {
                        break;
                    }
                    int32_t value = signext32(getreg16(reg));
                    int32_t lower = signext32(readw86(ea));
                    int32_t upper = signext32(readw86(ea + 2u));
                    if (value < lower || value > upper) {
                        r36sx_cpu_set_ip(firstip);
                        intcall86(R36SX_EXCEPTION_BOUND);
                    }
                }
                break;
            case 0x63:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_63: ;
#endif
                /* 63 ARPL Ew,Gw (80286+ protected mode) */
                if (!r36sx_cpu_native_protected_enabled() ||
                    r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                {
                    uint16_t dest = readrm16(rm);
                    uint8_t src_rpl = getreg16(reg) & 3u;
                    if ((dest & 3u) < src_rpl) {
                        dest = (uint16_t)((dest & 0xfffcu) | src_rpl);
                        writerm16(rm, dest);
                        zf = 1;
                    } else {
                        zf = 0;
                    }
                }
                break;
#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
            case 0x66:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_66: ;
#endif
                /* Operand-Size Override (???????? ?????? ?????????: 16 ? 32 ???) */
                if (!r36sx_cpu_has_80386_features()) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE = true;
                break;
            case 0x67:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_67: ;
#endif
                /* Address-Size Override (???????? ?????? ??????: 16 ? 32 ???) */
                if (!r36sx_cpu_has_80386_features()) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE = true;
                break;
#endif
            case 0x68:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_68: ;
#endif
                /* 68 PUSH Iv (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                push(getmem16(CPU_CS, CPU_IP)
                );
                StepIP(2);
                break;

            case 0x69:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_69: ;
#endif
                {
                /* 69 IMUL Gv Ev Iv (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                register int32_t temp1 = (int32_t)(int16_t)readrm16(rm);
                register int32_t temp2 = (int32_t)(int16_t)getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                temp1 *= temp2;
                putreg16(reg, (int16_t)temp1);
                if (temp1 != (int32_t)(int16_t)temp1) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
                break;
            }
            case 0x6A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6A: ;
#endif
                /* 6A PUSH Ib (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                push((uint16_t) signext(getmem8(CPU_CS, CPU_IP)));
                StepIP(1);
                break;

            case 0x6B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6B: ;
#endif
                {
                /* 6B IMUL Gv Eb Ib (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();
                register int32_t temp1 = (int32_t)(int16_t)readrm16(rm);
                register int32_t temp2 = (int32_t)(int16_t)signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                temp1 *= temp2;
				putreg16(reg, (int16_t)temp1);
                if (temp1 != (int32_t)(int16_t)temp1) {
                    x86_flags.value |= FLAG_CF_OF_MASK;
                } else {
                    x86_flags.value &= ~FLAG_CF_OF_MASK;
                }
                break;
            }
            case 0x6C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6C: ;
#endif
                /* 6C INSB */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }
                if (!r36sx_cpu_require_io_permission(CPU_DX, 1u, firstip)) {
                    break;
                }

                putmem8(CPU_ES, CPU_DI, portin(CPU_DX));
                if (df) {
                    CPU_DI = CPU_DI - 1;
                } else {
                    CPU_DI = CPU_DI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0x6D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6D: ;
#endif
                /* 6D INSW */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }
                if (!r36sx_cpu_require_io_permission(CPU_DX, 2u, firstip)) {
                    break;
                }

                putmem16(CPU_ES, CPU_DI, portin16(CPU_DX));
                if (df) {
                    CPU_DI = CPU_DI - 2;
                } else {
                    CPU_DI = CPU_DI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0x6E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6E: ;
#endif
                /* 6E OUTSB */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }
                if (!r36sx_cpu_require_io_permission(CPU_DX, 1u, firstip)) {
                    break;
                }

                portout(CPU_DX, getmem8(useseg, CPU_SI));
                if (df) {
                    CPU_SI = CPU_SI - 1;
                } else {
                    CPU_SI = CPU_SI + 1;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0x6F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_6F: ;
#endif
                /* 6F OUTSW */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                if (reptype && (CPU_CX == 0)) {
                    break;
                }
                if (!r36sx_cpu_require_io_permission(CPU_DX, 2u, firstip)) {
                    break;
                }

                portout16(CPU_DX, getmem16(useseg, CPU_SI));
                if (df) {
                    CPU_SI = CPU_SI - 2;
                } else {
                    CPU_SI = CPU_SI + 2;
                }

                if (reptype) {
                    CPU_CX = CPU_CX - 1;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;
#endif

            case 0x70:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_70: ;
#endif
                /* 70 JO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (of) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x71:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_71: ;
#endif
                /* 71 JNO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!of) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x72:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_72: ;
#endif
                /* 72 JB Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (cf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x73:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_73: ;
#endif
                /* 73 JNB Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!cf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x74:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_74: ;
#endif
                /* 74 JZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (zf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x75:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_75: ;
#endif
                /* 75 JNZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!zf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x76:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_76: ;
#endif
                /* 76 JBE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (cf || zf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x77:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_77: ;
#endif
                /* 77 JA Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!cf && !zf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x78:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_78: ;
#endif
                /* 78 JS Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x79:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_79: ;
#endif
                /* 79 JNS Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!sf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7A: ;
#endif
                /* 7A JPE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (pf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7B: ;
#endif
                /* 7B JPO Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!pf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7C: ;
#endif
                /* 7C JL Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf != of) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7D: ;
#endif
                /* 7D JGE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (sf == of) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7E: ;
#endif
                /* 7E JLE Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if ((sf != of) || zf) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x7F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_7F: ;
#endif
                /* 7F JG Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!
                    zf && (sf
                           == of)) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0x80:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_80: ;
#endif
            case 0x82:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_82: ;
#endif
                /* 80/82 GRP1 Eb Ib */
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                switch (reg) {
                    case 0:
                        op_add8();
                        break;
                    case 1:
                        op_or8();
                        break;
                    case 2:
                        op_adc8();
                        break;
                    case 3:
                        op_sbb8();
                        break;
                    case 4:
                        op_and8();
                        break;
                    case 5:
                        op_sub8();
                        break;
                    case 6:
                        op_xor8();
                        break;
                    case 7:
                        flag_sub8(oper1b, oper2b
                        );
                        break;
                    default:
                        break; /* to avoid compiler warnings */
                }

                if (reg < 7) {
                    writerm8(rm, res8
                    );
                }
                break;

            case 0x81:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_81: ;
#endif
                /* 81 GRP1 Ev Iv */
            case 0x83:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_83: ;
#endif
                /* 83 GRP1 Ev Ib */
                modregrm();

                oper1 = readrm16(rm);
                if (opcode == 0x81) {
                    oper2 = getmem16(CPU_CS, CPU_IP);
                    StepIP(2);
                } else {
                    oper2 = signext(getmem8(CPU_CS, CPU_IP));
                    StepIP(1);
                }

                switch (reg) {
                    case 0:
                        op_add16();
                        break;
                    case 1:
                        op_or16();
                        break;
                    case 2:
                        op_adc16();
                        break;
                    case 3:
                        op_sbb16();
                        break;
                    case 4:
                        op_and16();
                        break;
                    case 5:
                        op_sub16();
                        break;
                    case 6:
                        op_xor16();
                        break;
                    case 7:
                        flag_sub16(oper1, oper2
                        );
                        break;
                    default:
                        break; /* to avoid compiler warnings */
                }

                if (reg < 7) {
                    writerm16(rm, res16
                    );
                }
                break;

            case 0x84:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_84: ;
#endif
                /* 84 TEST Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                oper2b = readrm8(rm);
                flag_log8(oper1b
                          & oper2b);
                break;

            case 0x85:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_85: ;
#endif
                /* 85 TEST Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                oper2 = readrm16(rm);
                flag_log16(oper1
                           & oper2);
                break;

            case 0x86:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_86: ;
#endif
                /* 86 XCHG Gb Eb */
                modregrm();

                oper1b = getreg8(reg);
                putreg8(reg, readrm8(rm)
                );
                writerm8(rm, oper1b
                );
                break;

            case 0x87:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_87: ;
#endif
                /* 87 XCHG Gv Ev */
                modregrm();

                oper1 = getreg16(reg);
                putreg16(reg, readrm16(rm)
                );
                writerm16(rm, oper1
                );
                break;

            case 0x88:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_88: ;
#endif
                /* 88 MOV Eb Gb */
                modregrm();

                writerm8(rm, getreg8(reg)
                );
                break;

            case 0x89:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_89: ;
#endif
                /* 89 MOV Ev Gv */
                modregrm();

                writerm16(rm, getreg16(reg)
                );
                break;

            case 0x8A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8A: ;
#endif
                /* 8A MOV Gb Eb */
                modregrm();

                putreg8(reg, readrm8(rm)
                );
                break;

            case 0x8B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8B: ;
#endif
                /* 8B MOV Gv Ev */
                modregrm();

                putreg16(reg, readrm16(rm)
                );
                break;

            case 0x8C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8C: ;
#endif
                /* 8C MOV Ew Sw */
                modregrm();
                if ((!r36sx_cpu_has_80386_features() && reg > regds) ||
                    reg > reggs) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                writerm16(rm, getsegreg(reg)
                );
                break;

            case 0x8D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8D: ;
#endif
                /* 8D LEA Gv M */
                modregrm();
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* Intel LEA takes a memory addressing form, not a register. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                putreg16(reg, ea
                         -
                         useseg_base
                );
                break;

            case 0x8E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8E: ;
#endif
                /* 8E MOV Sw Ew */
                modregrm();
                if (reg == regcs ||
                    (!r36sx_cpu_has_80386_features() && reg > regds) ||
                    reg > reggs) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                if (r36sx_cpu_load_segment_at(reg, readrm16(rm), firstip) &&
                    reg == regss) {
                    r36sx_cpu_delay_maskable_interrupts_one_instruction();
                }
                break;

            case 0x8F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_8F: ;
#endif
                /* 8F POP Ev */
                modregrm();
                if (reg != 0) {
                    /* 8F is a group opcode; only /0 is documented as POP Ev. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                writerm16(rm, pop()
                );
                break;

            case 0x90:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_90: ;
#endif
                /* 90 NOP */
                break;

            case 0x91:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_91: ;
#endif
                /* 91 XCHG eCX eAX */
                oper1 = CPU_CX;
                CPU_CX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x92:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_92: ;
#endif
                /* 92 XCHG eDX eAX */
                oper1 = CPU_DX;
                CPU_DX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x93:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_93: ;
#endif
                /* 93 XCHG eBX eAX */
                oper1 = CPU_BX;
                CPU_BX = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x94:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_94: ;
#endif
                /* 94 XCHG eSP eAX */
                oper1 = CPU_SP;
                CPU_SP = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x95:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_95: ;
#endif
                /* 95 XCHG eBP eAX */
                oper1 = CPU_BP;
                CPU_BP = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x96:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_96: ;
#endif
                /* 96 XCHG eSI eAX */
                oper1 = CPU_SI;
                CPU_SI = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x97:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_97: ;
#endif
                /* 97 XCHG eDI eAX */
                oper1 = CPU_DI;
                CPU_DI = CPU_AX;
                CPU_AX = oper1;
                break;

            case 0x98:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_98: ;
#endif
                /* 98 CBW */
                if ((CPU_AL & 0x80) == 0x80) {
                    CPU_AH = 0xFF;
                } else {
                    CPU_AH = 0;
                }
                break;

            case 0x99:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_99: ;
#endif
                /* 99 CWD */
                if ((CPU_AH & 0x80) == 0x80) {
                    CPU_DX = 0xFFFF;
                } else {
                    CPU_DX = 0;
                }
                break;

            case 0x9A:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9A: ;
#endif
                /* 9A CALL Ap */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                if (r36sx_cpu_protected_enabled()) {
                    r36sx_cpu_protected_far_call(oper2, oper1, 0, firstip);
                    break;
                }
                push(CPU_CS);
                push(CPU_IP);
                r36sx_cpu_load_segment(regcs, oper2);
                r36sx_cpu_set_ip(oper1);
                break;

            case 0x9B:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9B: ;
#endif
                /* 9B WAIT */
                if (!r36sx_cpu_x87_wait_available(firstip)) {
                    break;
                }
                if (!r36sx_cpu_x87_present()) {
                    break;
                }
                if (OpFwait()) {
                    r36sx_cpu_raise_exception(
                        R36SX_EXCEPTION_X87_ERROR, 0, 0, firstip);
                }
                break;

            case 0x9C:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9C: ;
#endif
                /* 9C PUSHF */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    push32(makeflagsdword());
                    break;
                }
                push(makeflagsword());
                break;

            case 0x9D:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9D: ;
#endif
                /* 9D POPF */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    decodeflagsdword_popfd(pop32());
                    break;
                }
                decodeflagsword(pop());
                break;

            case 0x9E:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9E: ;
#endif
                /* 9E SAHF */
                decodeflagsword((makeflagsword() & 0xFF00) | CPU_AH);
                break;

            case 0x9F:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_9F: ;
#endif
                /* 9F LAHF */
                CPU_AH = makeflagsword() & 0xFF;
                break;

            case 0xA0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A0: ;
#endif
                /* A0 MOV CPU_AL Ob */
                CPU_AL = getmem8(useseg, r36sx_read_moffs());
                break;

            case 0xA1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A1: ;
#endif
                /* A1 MOV eAX Ov */
                oper1 = getmem16(useseg, r36sx_read_moffs());
                CPU_AX = oper1;
                break;

            case 0xA2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A2: ;
#endif
                /* A2 MOV Ob CPU_AL */
                putmem8(useseg, r36sx_read_moffs(), CPU_AL);
                break;

            case 0xA3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A3: ;
#endif
                /* A3 MOV Ov eAX */
                putmem16(useseg, r36sx_read_moffs(), CPU_AX);
                break;

            case 0xA4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A4: ;
#endif
                /* A4 MOVSB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_movsb(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch - 1u;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
                    }
                    break;
                }

                r36sx_rep_movsb(1);
                break;

            case 0xA5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A5: ;
#endif
                /* A5 MOVSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_movsw(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch - 1u;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
                    }
                    break;
                }

                r36sx_rep_movsw(1);
                break;

            case 0xA6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A6: ;
#endif
                /* A6 CMPSB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    uint32_t di = r36sx_dst_index();
                    oper1b = getmem8(useseg, si);
                    oper2b = getmem8(CPU_ES, di);
                    r36sx_set_src_index(df ? si - 1u : si + 1u);
                    r36sx_set_dst_index(df ? di - 1u : di + 1u);
                }

                flag_sub8(oper1b, oper2b
                );
                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xA7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A7: ;
#endif
                /* A7 CMPSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    uint32_t di = r36sx_dst_index();
                    oper1 = getmem16(useseg, si);
                    oper2 = getmem16(CPU_ES, di);
                    r36sx_set_src_index(df ? si - 2u : si + 2u);
                    r36sx_set_dst_index(df ? di - 2u : di + 2u);
                }

                flag_sub16(oper1, oper2
                );
                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if ((reptype == 1) && !zf) {
                    break;
                }

                if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xA8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A8: ;
#endif
                /* A8 TEST CPU_AL Ib */
                oper1b = CPU_AL;
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                flag_log8(oper1b
                          & oper2b);
                break;

            case 0xA9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_A9: ;
#endif
                /* A9 TEST eAX Iv */
                oper1 = CPU_AX;
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                flag_log16(oper1
                           & oper2);
                break;

            case 0xAA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AA: ;
#endif
                /* AA STOSB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_stosb(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch - 1u;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
                    }
                    break;
                }

                r36sx_rep_stosb(1);
                break;

            case 0xAB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AB: ;
#endif
                /* AB STOSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                if (reptype) {
                    uint32_t batch =
                        (tf || was_TF)
                            ? 1u
                            : r36sx_rep_batch_count(r36sx_rep_get_count(),
                                                    loopcount, execloops);
                    r36sx_rep_stosw(batch);
                    r36sx_rep_set_count(r36sx_rep_get_count() - batch);
                    loopcount += batch - 1u;
                    if (r36sx_rep_get_count() != 0) {
                        r36sx_cpu_set_ip(firstip);
                    }
                    break;
                }

                r36sx_rep_stosw(1);
                break;

            case 0xAC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AC: ;
#endif
                /* AC LODSB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    CPU_AL = getmem8(useseg, si);
                    r36sx_set_src_index(df ? si - 1u : si + 1u);
                }

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AD: ;
#endif
                /* AD LODSW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                {
                    uint32_t si = r36sx_src_index();
                    oper1 = getmem16(useseg, si);
                    CPU_AX = oper1;
                    r36sx_set_src_index(df ? si - 2u : si + 2u);
                }

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AE: ;
#endif
                /* AE SCASB */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                oper1b = CPU_AL;
                {
                    uint32_t di = r36sx_dst_index();
                    oper2b = getmem8(CPU_ES, di);
                    r36sx_set_dst_index(df ? di - 1u : di + 1u);
                }
                flag_sub8(oper1b, oper2b
                );

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    break;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xAF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_AF: ;
#endif
                /* AF SCASW */
                if (reptype && r36sx_rep_get_count() == 0) {
                    break;
                }

                oper1 = CPU_AX;
                {
                    uint32_t di = r36sx_dst_index();
                    oper2 = getmem16(CPU_ES, di);
                    r36sx_set_dst_index(df ? di - 2u : di + 2u);
                }
                flag_sub16(oper1, oper2
                );

                if (reptype) {
                    r36sx_rep_set_count(r36sx_rep_get_count() - 1);
                }

                if ((reptype == 1) && !zf) {
                    break;
                } else if ((reptype == 2) && (zf == 1)) {
                    //did i fix a typo bug? this used to be & instead of &&
                    break;
                }

                if (!reptype) {
                    break;
                }

                r36sx_cpu_set_ip(firstip);
                break;

            case 0xB0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B0: ;
#endif
                /* B0 MOV CPU_AL Ib */
                CPU_AL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B1: ;
#endif
                /* B1 MOV CPU_CL Ib */
                CPU_CL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B2: ;
#endif
                /* B2 MOV CPU_DL Ib */
                CPU_DL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B3: ;
#endif
                /* B3 MOV CPU_BL Ib */
                CPU_BL = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B4: ;
#endif
                /* B4 MOV CPU_AH Ib */
                CPU_AH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B5: ;
#endif
                /* B5 MOV CPU_CH Ib */
                CPU_CH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B6: ;
#endif
                /* B6 MOV CPU_DH Ib */
                CPU_DH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B7: ;
#endif
                /* B7 MOV CPU_BH Ib */
                CPU_BH = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                break;

            case 0xB8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B8: ;
#endif
                /* B8 MOV eAX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_AX = oper1;
                break;

            case 0xB9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_B9: ;
#endif
                /* B9 MOV eCX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_CX = oper1;
                break;

            case 0xBA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BA: ;
#endif
                /* BA MOV eDX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_DX = oper1;
                break;

            case 0xBB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BB: ;
#endif
                /* BB MOV eBX Iv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                CPU_BX = oper1;
                break;

            case 0xBC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BC: ;
#endif
                /* BC MOV eSP Iv */
                CPU_SP = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BD: ;
#endif
                /* BD MOV eBP Iv */
                CPU_BP = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BE: ;
#endif
                /* BE MOV eSI Iv */
                CPU_SI = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xBF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_BF: ;
#endif
                /* BF MOV eDI Iv */
                CPU_DI = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                break;

            case 0xC0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C0: ;
#endif
                /* C0 GRP2 byte imm8 (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                oper1b = readrm8(rm);
                oper2b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm8(rm, op_grp2_8(
                    R36SX_CPU_CORE_SHIFT_COUNT(oper2b), oper1b));
                break;

            case 0xC1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C1: ;
#endif
                /* C1 GRP2 word imm8 (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                modregrm();

                oper1 = readrm16(rm);
                oper2 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                writerm16(rm, op_grp2_16(
                    R36SX_CPU_CORE_SHIFT_COUNT((uint8_t)oper2))
                );
                break;

            case 0xC2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C2: ;
#endif
                /* C2 RET Iw */
                oper1 = getmem16(CPU_CS, CPU_IP);
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_adjust_stack(oper1);
                break;

            case 0xC3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C3: ;
#endif
                /* C3 RET */
                r36sx_cpu_set_ip(pop());
                break;

            case 0xC4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C4: ;
#endif
                /* C4 LES Gv Mp */
                modregrm();
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* LES loads a far pointer from memory. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                {
                    uint8_t pointer_size = R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ? 6u : 4u;
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, pointer_size, 0)) {
                        break;
                    }
                    uint32_t offset = R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ? readdw86(ea) :
                                                            readw86(ea);
                    uint16_t selector = readw86(ea + (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ?
                                                      4u : 2u));
                    if (r36sx_cpu_load_segment(reges, selector)) {
                        if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                            putreg32(reg, offset);
                        } else {
                            putreg16(reg, (uint16_t)offset);
                        }
                    }
                }
                break;

            case 0xC5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C5: ;
#endif
                /* C5 LDS Gv Mp */
                modregrm();
                if (mode == R36SX_MODRM_MOD_REGISTER) {
                    /* LDS loads a far pointer from memory. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                getea(rm);
                {
                    uint8_t pointer_size = R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ? 6u : 4u;
                    if (!r36sx_cpu_check_segment_access(
                            ea - useseg_base, pointer_size, 0)) {
                        break;
                    }
                    uint32_t offset = R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ? readdw86(ea) :
                                                            readw86(ea);
                    uint16_t selector = readw86(ea + (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE ?
                                                      4u : 2u));
                    if (r36sx_cpu_load_segment(regds, selector)) {
                        if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                            putreg32(reg, offset);
                        } else {
                            putreg16(reg, (uint16_t)offset);
                        }
                    }
                }
                break;

            case 0xC6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C6: ;
#endif
                /* C6 MOV Eb Ib */
                modregrm();
                if (reg != 0) {
                    /* C6 is a group opcode; only /0 is documented as MOV Eb,Ib. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                writerm8(rm, getmem8(CPU_CS, CPU_IP)
                );
                StepIP(1);
                break;

            case 0xC7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C7: ;
#endif
                /* C7 MOV Ev Iv */
                modregrm();
                if (reg != 0) {
                    /* C7 is a group opcode; only /0 is documented as MOV Ev,Iv. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                writerm16(rm, getmem16(CPU_CS, CPU_IP)
                );
                StepIP(2);
                break;

            case 0xC8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C8: ;
#endif
                /* C8 ENTER (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                stacksize = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                nestlev = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    r36sx_cpu_enter32(stacksize, nestlev);
                } else {
                    r36sx_cpu_enter16(stacksize, nestlev);
                }

                break;

            case 0xC9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_C9: ;
#endif
                /* C9 LEAVE (80186+) */
                if (r36sx_cpu_strict_8086_mode) {
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                /*
                 * LEAVE uses the stack-address size for SP/ESP restoration
                 * and the operand size for the BP/EBP pop width.
                 */
                r36sx_cpu_set_stack_pointer(CPU_EBP);
                if (R36SX_CPU_CORE_OPERAND_SIZE_OVERRIDE) {
                    CPU_EBP = pop32();
                } else {
                    CPU_BP = pop();
                }
                break;

            case 0xCA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CA: ;
#endif
                /* CA RETF Iw */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_retf(oper1, 0, firstip);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                r36sx_cpu_adjust_stack(oper1);
                break;

            case 0xCB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CB: ;
#endif
                /* CB RETF */
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_retf(0, 0, firstip);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                break;

            case 0xCC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CC: ;
#endif
                /* CC INT 3 */
                /*
                 * Intel documents INT3 as not IOPL-sensitive in V86 mode; it
                 * generates interrupt 3 even when INT imm8 would #GP(0).
                 */
                r36sx_cpu_software_interrupt(3, firstip);
                break;

            case 0xCD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CD: ;
#endif
                /* CD INT Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                r36sx_cpu_software_interrupt(oper1b, firstip);
                break;

            case 0xCE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CE: ;
#endif
                /* CE INTO */
                if (of) {
                    /*
                     * INTO follows INT3 here: in V86 mode it is not
                     * IOPL-sensitive and generates interrupt 4 only when OF=1.
                     */
                    r36sx_cpu_software_interrupt(4, firstip);
                }
                break;

            case 0xCF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_CF: ;
#endif
                /* CF IRET */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                if (r36sx_cpu_native_protected_enabled()) {
                    r36sx_cpu_protected_iret(0);
                    break;
                }
                r36sx_cpu_set_ip(pop());
                r36sx_cpu_load_segment(regcs, pop());
                decodeflagsword(pop());
                /*
                 * if (net.enabled) net.canrecv = 1;
                 */
                break;

            case 0xD0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D0: ;
#endif
                /* D0 GRP2 Eb 1 */
                modregrm();

                oper1b = readrm8(rm);
                writerm8(rm, op_grp2_8(R36SX_CPU_CORE_SHIFT_COUNT(1),
                                        oper1b));
                break;

            case 0xD1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D1: ;
#endif
                /* D1 GRP2 Ev 1 */
                modregrm();

                oper1 = readrm16(rm);
                writerm16(rm, op_grp2_16(R36SX_CPU_CORE_SHIFT_COUNT(1)));
                break;

            case 0xD2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D2: ;
#endif
                /* D2 GRP2 Eb CPU_CL */
                modregrm();

                oper1b = readrm8(rm);
                writerm8(rm, op_grp2_8(R36SX_CPU_CORE_SHIFT_COUNT(CPU_CL),
                                        oper1b));
                break;

            case 0xD3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D3: ;
#endif
                /* D3 GRP2 Ev CPU_CL */
                modregrm();

                oper1 = readrm16(rm);
                writerm16(rm, op_grp2_16(
                    R36SX_CPU_CORE_SHIFT_COUNT(CPU_CL))
                );
                break;

            case 0xD4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D4: ;
#endif
                /* D4 AAM I0 */
                oper1 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!oper1) {
                    r36sx_cpu_divide_error(firstip);
                    break;
                } /* division by zero */

                CPU_AH = (CPU_AL / oper1) & 255;
                CPU_AL = (CPU_AL % oper1) & 255;
                flag_szp8(CPU_AL);
                break;

            case 0xD5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D5: ;
#endif
                /* D5 AAD I0 */
                oper1 = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                CPU_AL = (CPU_AH * oper1 + CPU_AL) & 255;
                CPU_AH = 0;
                flag_szp8(CPU_AL);
                break;

            case 0xD6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D6: ;
#endif
                /* D6 XLAT on V20/V30, SALC on 8086/8088 */
#ifndef CPU_NO_SALC
                CPU_AL = CPU_FL_CF ? 0xFF : 0x00;
                break;
#else
                /*
                 * D6 is outside Intel's documented 8086/8088 instruction set.
                 * Some chips execute undocumented SALC here; the strict CPU
                 * model reports it through the same invalid-opcode path as
                 * other non-documented opcode bytes.
                 */
                r36sx_cpu_invalid_opcode(firstip);
                break;
#endif

            case 0xD7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D7: ;
#endif
                /* D7 XLAT */
                CPU_AL = getmem8(useseg,
                                  (R36SX_CPU_CORE_ADDRESS_SIZE_OVERRIDE ? CPU_EBX : CPU_BX) +
                                  CPU_AL);
                break;

            case 0xD8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D8: ;
#endif
            case 0xD9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_D9: ;
#endif
            case 0xDA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DA: ;
#endif
            case 0xDB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DB: ;
#endif
            case 0xDC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DC: ;
#endif
            case 0xDE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DE: ;
#endif
            case 0xDD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DD: ;
#endif
            case 0xDF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_DF: ;
#endif
                /* escape to x87 FPU */
                if (!r36sx_cpu_x87_escape_available(firstip)) {
                    break;
                }
                if (!r36sx_cpu_x87_present()) {
                    r36sx_cpu_skip_x87_escape();
                    break;
                }
                OpFpu(opcode);
                break;

            case 0xE0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E0: ;
#endif
                /* E0 LOOPNZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count && !zf) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE1:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E1: ;
#endif
                /* E1 LOOPZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count && (zf == 1)) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE2:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E2: ;
#endif
                /* E2 LOOP Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                {
                    uint32_t count = r36sx_loop_get_count() - 1u;
                    r36sx_loop_set_count(count);
                    if (count) {
                        r36sx_cpu_add_ip((int16_t)temp16);
                    }
                }
                break;

            case 0xE3:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E3: ;
#endif
                /* E3 JCXZ/JECXZ Jb */
                temp16 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                if (!r36sx_loop_get_count()) {
                    r36sx_cpu_add_ip((int16_t)temp16);
                }
                break;

            case 0xE4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E4: ;
#endif
                /* E4 IN CPU_AL Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 1u, firstip)) {
                    break;
                }
                CPU_AL = (uint8_t) portin(oper1b);
                break;

            case 0xE5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E5: ;
#endif
                /* E5 IN eAX Ib */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 2u, firstip)) {
                    break;
                }
                CPU_AX = portin16(oper1b);
                break;

            case 0xE6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E6: ;
#endif
                /* E6 OUT Ib CPU_AL */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 1u, firstip)) {
                    break;
                }
                portout(oper1b, CPU_AL
                );
                break;

            case 0xE7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E7: ;
#endif
                /* E7 OUT Ib eAX */
                oper1b = getmem8(CPU_CS, CPU_IP);
                StepIP(1);
                if (!r36sx_cpu_require_io_permission(oper1b, 2u, firstip)) {
                    break;
                }
                portout16(oper1b, CPU_AX
                );
                break;

            case 0xE8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E8: ;
#endif
                /* E8 CALL Jv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                push(CPU_IP);
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xE9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_E9: ;
#endif
                /* E9 JMP Jv */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xEA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_EA: ;
#endif
                /* EA JMP Ap */
                oper1 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                oper2 = getmem16(CPU_CS, CPU_IP);
                StepIP(2);
                if (r36sx_cpu_protected_enabled()) {
                    r36sx_cpu_protected_far_jump(oper2, oper1, firstip);
                    break;
                }
                r36sx_cpu_load_segment(regcs, oper2);
                r36sx_cpu_set_ip(oper1);
                break;

            case 0xEB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_EB: ;
#endif
                /* EB JMP Jb */
                oper1 = signext(getmem8(CPU_CS, CPU_IP));
                StepIP(1);
                r36sx_cpu_add_ip((int16_t)oper1);
                break;

            case 0xEC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_EC: ;
#endif
                /* EC IN CPU_AL regdx */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 1u, firstip)) {
                    break;
                }
                CPU_AL = (uint8_t) portin(oper1);
                break;

            case 0xED:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_ED: ;
#endif
                /* ED IN eAX regdx */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 2u, firstip)) {
                    break;
                }
                CPU_AX = portin16(oper1);
                break;

            case 0xEE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_EE: ;
#endif
                /* EE OUT regdx CPU_AL */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 1u, firstip)) {
                    break;
                }
                portout(oper1, CPU_AL
                );
                break;

            case 0xEF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_EF: ;
#endif
                /* EF OUT regdx eAX */
                oper1 = CPU_DX;
                if (!r36sx_cpu_require_io_permission(oper1, 2u, firstip)) {
                    break;
                }
                portout16(oper1, CPU_AX);
                break;

            case 0xF0:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F0: ;
#endif
                /* F0 LOCK */
                if (r36sx_cpu_v86_iopl_sensitive_fault(firstip)) {
                    break;
                }
                break;

            case 0xF4:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F4: ;
#endif
                /* F4 HLT */
                if (!r36sx_cpu_require_cpl0(firstip)) {
                    break;
                }
                hltstate = 1;
                return;

            case 0xF5:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F5: ;
#endif
                /* F5 CMC */
                if (!cf) {
                    cf = 1;
                } else {
                    cf = 0;
                }
                break;

            case 0xF6:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F6: ;
#endif
                /* F6 GRP3a Eb */
                modregrm();
                oper1b = readrm8(rm);
                oper1 = signext(oper1b);
                switch (reg) {
                    case 0: /* TEST */
                        flag_log8(oper1b & getmem8(CPU_CS, CPU_IP));
                        StepIP(1);
                        break;

                    case 1:
                        /*
                         * Some 8086-class chips alias F6 /1 to TEST, but
                         * Intel documents only F6 /0.  Keep strict CPU-model
                         * filtering consistent with the official opcode map.
                         */
                        r36sx_cpu_invalid_opcode(firstip);
                        break;

                    case 2: /* NOT */
                        res8 = ~oper1b;
                        break;

                    case 3: /* NEG */
                        res8 = (~oper1b) + 1;
                        flag_sub8(0, oper1b);
                        if (res8 == 0) {
                            cf = 0;
                        } else {
                            cf = 1;
                        }
                        break;

                    case 4: {
                        /* MUL */
                        register uint32_t temp1 = (uint32_t) oper1b * (uint32_t) CPU_AL;
                        CPU_AX = temp1 & 0xFFFF;
                        flag_szp8((uint8_t) temp1);
                        if (CPU_AH) {
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
                        oper1 = signext(oper1b);
                        register int32_t temp1 = (int32_t)(int8_t)signext(CPU_AL);
                        register int32_t temp2 = (int32_t)(int8_t)oper1;
						temp1 *= temp2;
						int16_t result = (int16_t)temp1;
						int8_t truncated = (int8_t)result;
						if (result != (int16_t)truncated) {
							x86_flags.value |= FLAG_CF_OF_MASK; // CF=OF=1
						} else {
							x86_flags.value &= ~FLAG_CF_OF_MASK; // CF=OF=0
						}
						CPU_AL = truncated;
						CPU_AH = (uint8_t)(result >> 8);
#ifdef CPU_CLEAR_ZF_ON_MUL
                        zf = 0;
#endif
                        break;
                    }
                    case 6: /* DIV */
                        op_div8(CPU_AX, oper1b, firstip);
                        break;

                    case 7: /* IDIV */
                        op_idiv8(CPU_AX, oper1b, firstip);
                        break;
                }

                if ((reg > 1) && (reg < 4)) {
                    writerm8(rm, res8
                    );
                }
                break;

            case 0xF7:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F7: ;
#endif
                /* F7 GRP3b Ev */
                modregrm();

                oper1 = readrm16(rm);
                op_grp3_16(firstip);
                if ((reg > 1) && (reg < 4)) {
                    writerm16(rm, res16
                    );
                }
                break;

            case 0xF8:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F8: ;
#endif
                /* F8 CLC */
                cf = 0;
                break;

            case 0xF9:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_F9: ;
#endif
                /* F9 STC */
                cf = 1;
                break;

            case 0xFA:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FA: ;
#endif
                /* FA CLI */
                if (!r36sx_cpu_require_iopl(firstip)) {
                    break;
                }
                ifl = 0;
                break;

            case 0xFB:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FB: ;
#endif
                /* FB STI */
                if (!r36sx_cpu_require_iopl(firstip)) {
                    break;
                }
                ifl = 1;
                r36sx_cpu_delay_maskable_interrupts_one_instruction();
                break;

            case 0xFC:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FC: ;
#endif
                /* FC CLD */
                df = 0;
                break;

            case 0xFD:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FD: ;
#endif
                /* FD STD */
                df = 1;
                break;

            case 0xFE:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FE: ;
#endif
                /* FE GRP4 Eb */
                modregrm();
                if (reg > 1) {
                    /* FE is a group opcode; only /0 INC and /1 DEC are valid. */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }
                oper1b = readrm8(rm);
                oper2b = 1;
                if (!reg) {
                    tempcf = cf;
                    op_add8();
                    cf = tempcf;
                    writerm8(rm, res8);
                } else {
                    tempcf = cf;
                    res8 = oper1b - oper2b;
                    flag_sub8(oper1b, oper2b);
                    cf = tempcf;
                    writerm8(rm, res8);
                }
                break;

            case 0xFF:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_FF: ;
#endif
                /* FF GRP5 Ev */
                modregrm();
                if (reg == 7 ||
                    ((reg == 3 || reg == 5) &&
                     mode == R36SX_MODRM_MOD_REGISTER)) {
                    /*
                     * FF /7 is undefined, and far CALL/JMP (/3,/5) require
                     * a memory far pointer rather than a register operand.
                     */
                    r36sx_cpu_invalid_opcode(firstip);
                    break;
                }

                oper1 = readrm16(rm);
                op_grp5(firstip);
                break;

            default:
#if R36SX_CPU_CORE_COMPUTED_GOTO
            r36sx_opcode_default: ;
#endif
#ifdef CPU_ALLOW_ILLEGAL_OP_EXCEPTION
                r36sx_cpu_invalid_opcode(firstip);
                if (!r36sx_cpu_invalid_opcode_nop_like()) {
                    r36sx_pico286_debug_log(
                        "[CPU] Invalid opcode 0x%02x exception at %04X:%04X",
                        opcode, CPU_CS, firstip);
                }
#endif
                break;
        }
#if R36SX_CPU_CORE_HAS_386_EXTENDED_OPS
r36sx_opcode_done:
#endif
        if (unlikely(r36sx_cpu_maskable_interrupt_shadow != 0u)) {
            r36sx_cpu_maskable_interrupt_shadow--;
        }
#if !R36SX_CPU_CORE_HAS_386_DEBUG_REGS
        if (unlikely(was_TF)) {
            was_TF = false;
            intcall86(1);
        } else if (tf) {
            was_TF = true;
        }
#else
        if (unlikely(r36sx_debug_pending_dr6_hits || was_TF)) {
            uint32_t dr6_hits = r36sx_debug_pending_dr6_hits;
            r36sx_debug_pending_dr6_hits = 0;
            if (was_TF) {
                dr6_hits |= R36SX_DR6_BS_MASK;
            }
            was_TF = false;
            r36sx_cpu_raise_debug_exception(CPU_IP, dr6_hits);
        } else if (tf) {
            was_TF = true;
        }
#endif
    }
    r36sx_app_stats_record_x86(loopcount);
}

#undef R36SX_CPU_CORE_SHIFT_COUNT
#ifdef R36SX_CPU_CORE_LIMIT_SHIFT_COUNT_DEFAULTED
#undef R36SX_CPU_CORE_LIMIT_SHIFT_COUNT_DEFAULTED
#undef R36SX_CPU_CORE_LIMIT_SHIFT_COUNT
#endif
