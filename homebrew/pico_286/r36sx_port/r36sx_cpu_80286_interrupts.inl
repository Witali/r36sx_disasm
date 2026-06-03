/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

static uint8_t r36sx_cpu_protected_interrupt(uint8_t intnum,
                                             uint32_t error_code,
                                             uint8_t has_error_code,
                                             uint8_t software_int)
{
    uint32_t gate_offset = (uint32_t)intnum * 8u;
    if (gate_offset + 7u > r36sx_idtr_limit) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt IDT limit fault int=%02x idtr=%08lx:%04x",
            intnum, (unsigned long)r36sx_idtr_base, r36sx_idtr_limit);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt IDT limit", CPU_IP);
        return 0;
    }

    uint32_t addr = r36sx_idtr_base + gate_offset;
    uint32_t lo = readdw86(addr);
    uint32_t hi = readdw86(addr + 4u);
    uint8_t access = (uint8_t)((hi >> 8) & 0xffu);
    uint8_t type = access & 0x0fu;
    uint8_t gate16 = type == 0x06u || type == 0x07u;
    uint8_t gate32 = type == 0x0eu || type == 0x0fu;
    uint8_t task_gate = type == R36SX_DESCRIPTOR_TYPE_TASK_GATE;
    uint8_t gate_dpl = (access >> 5) & 3u;
    /* IDT selector-style error code: vector index plus the IDT source bit. */
    uint32_t gate_error = ((uint32_t)intnum << 3) | 0x02u;

    if (task_gate) {
        if ((access & R36SX_DESCRIPTOR_PRESENT) == 0) {
#if R36SX_DEBUG_PM_VERBOSE
            r36sx_pico286_debug_log(
                "[CPU] protected interrupt task gate not present int=%02x",
                intnum);
#endif
            r36sx_pm_diag_log_first_fault("protected task gate not present",
                                          CPU_IP);
            return 0;
        }
        if (software_int && r36sx_cpu_cpl() > gate_dpl) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1,
                                      CPU_IP);
            return 0;
        }
        return r36sx_cpu_task_switch((uint16_t)(lo >> 16),
                                     R36SX_TASK_SWITCH_INTERRUPT);
    }

    if ((access & R36SX_DESCRIPTOR_PRESENT) == 0 ||
        !(gate16 ||
          (gate32 && r36sx_cpu_descriptor_uses_386_format()))) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt unsupported gate int=%02x access=%02x",
            intnum, access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt gate unsupported",
                                      CPU_IP);
        return 0;
    }

    if (software_int && r36sx_cpu_cpl() > gate_dpl) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1, CPU_IP);
        return 0;
    }

    uint16_t selector = (uint16_t)(lo >> 16);
    uint32_t offset = (lo & 0xffffu) |
                      (gate32 ? (hi & 0xffff0000u) : 0u);

    r36sx_segment_cache_t target_cs;
    uint8_t old_cpl = r36sx_cpu_cpl();
    uint8_t old_vm86 = r36sx_cpu_v86_enabled();
    uint8_t new_cpl;
    if (!r36sx_cpu_load_code_for_transfer(selector, offset, 1, 0,
                                          &target_cs, &new_cpl)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt invalid target CS int=%02x selector=%04x access=%02x",
            intnum, selector, target_cs.access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt target CS invalid",
                                      CPU_IP);
        return 0;
    }

    uint16_t old_ss = CPU_SS;
    uint32_t old_sp = r36sx_cpu_stack_pointer_value();
    uint16_t old_es = CPU_ES;
    uint16_t old_ds = CPU_DS;
    uint16_t old_fs = CPU_FS;
    uint16_t old_gs = CPU_GS;

    if (old_vm86 || new_cpl < old_cpl) {
        uint32_t new_sp;
        uint16_t new_ss;
        r36sx_segment_cache_t new_ss_cache;
        if (!r36sx_cpu_tss_stack_for_level(new_cpl, &new_sp, &new_ss) ||
            !r36sx_cpu_decode_stack_segment_for_level(
                new_ss, new_cpl, &new_ss_cache,
                R36SX_EXCEPTION_INVALID_TSS)) {
            return 0;
        }
        r36sx_cpu_commit_stack_segment(new_ss, &new_ss_cache);
        r36sx_cpu_set_stack_pointer(new_sp);
    }

    if (gate32) {
        /*
         * A 386 interrupt from VM86 first saves the VM86 data segments, then
         * the outer VM86 SS:ESP, before the normal EFLAGS/CS/EIP frame.
         * IRETD with VM=1 consumes the same frame in the reverse order.
         */
        if (old_vm86) {
            push32(old_gs);
            push32(old_fs);
            push32(old_ds);
            push32(old_es);
        }
        if (old_vm86 || new_cpl < old_cpl) {
            push32(old_ss);
            push32(old_sp);
        }
        push32(makeflagsdword());
        push32(CPU_CS);
        push32(CPU_IP);
        if (has_error_code) {
            push32(error_code);
        }
    } else {
        if (new_cpl < old_cpl) {
            push(old_ss);
            push((uint16_t)old_sp);
        }
        push(makeflagsword());
        push(CPU_CS);
        push(CPU_IP);
        if (has_error_code) {
            push((uint16_t)error_code);
        }
    }

    r36sx_cpu_commit_code_transfer(selector, &target_cs, new_cpl, offset);
    x86_flags.value &= ~(R36SX_EFLAGS_VM_MASK | R36SX_EFLAGS_RF_MASK);
    if (old_vm86) {
        r36sx_cpu_clear_segment_cache(reges, 0);
        r36sx_cpu_clear_segment_cache(regds, 0);
        r36sx_cpu_clear_segment_cache(regfs, 0);
        r36sx_cpu_clear_segment_cache(reggs, 0);
    }
    if (type == 0x06u || type == 0x0eu) {
        ifl = 0;
    }
    tf = 0;
    return 1;
}

static void r36sx_cpu_raise_exception(uint8_t intnum,
                                      uint32_t error_code,
                                      uint8_t has_error_code,
                                      uint32_t fault_ip)
{
    r36sx_cpu_set_ip(fault_ip);
    R36SX_PM_DIAG_LOG(
        "[PM] exception int=%02X err=%08lX has_err=%u cs:eip=%04X:%08lX",
        intnum, (unsigned long)error_code, has_error_code,
        CPU_CS, (unsigned long)CPU_IP);

    if (r36sx_cpu_protected_enabled()) {
        if (r36sx_dpmi_dispatch_pm_exception(intnum, error_code,
                                             has_error_code)) {
            return;
        }
        if (!r36sx_cpu_protected_interrupt(
                intnum, error_code, has_error_code, 0)) {
            r36sx_pm_diag_log_first_fault("protected exception delivery",
                                          fault_ip);
        }
        return;
    }

    intcall86(intnum);
}

