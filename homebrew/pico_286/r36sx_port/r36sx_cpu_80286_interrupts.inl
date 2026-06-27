/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

#if R36SX_DEBUG_PM_DIAG
static uint8_t r36sx_pm_diag_idt_vector_is_interesting(uint8_t intnum)
{
    return intnum == 0x10u || intnum == 0x21u ||
           intnum == 0x31u || intnum == R36SX_EXCEPTION_GP;
}

static void r36sx_pm_diag_log_idt_gate_if_interesting(uint8_t intnum,
                                                      uint8_t software_int,
                                                      uint32_t fault_ip)
{
    enum { R36SX_PM_DIAG_IDT_SLOT_COUNT = 4 };
    typedef struct {
        uint8_t valid;
        uint8_t intnum;
        uint32_t idtr_base;
        uint16_t idtr_limit;
        uint32_t lo;
        uint32_t hi;
    } r36sx_pm_diag_idt_slot_t;

    static r36sx_pm_diag_idt_slot_t slots[R36SX_PM_DIAG_IDT_SLOT_COUNT];

    if (!r36sx_pm_diag_idt_vector_is_interesting(intnum)) {
        return;
    }

    uint32_t gate_offset = (uint32_t)intnum * 8u;
    if (gate_offset + 7u > r36sx_idtr_limit) {
        r36sx_pico286_debug_log(
            "[PM] IDT[%02X] unavailable reason=%s idtr=%08lX:%04X "
            "gate_offset=%04lX fault=%04X:%08lX",
            intnum, software_int ? "soft" : "cpu",
            (unsigned long)r36sx_idtr_base, r36sx_idtr_limit,
            (unsigned long)gate_offset, CPU_CS, (unsigned long)fault_ip);
        return;
    }

    uint32_t addr = r36sx_idtr_base + gate_offset;
    uint32_t lo = r36sx_cpu_system_read_linear32(addr);
    uint32_t hi = r36sx_cpu_system_read_linear32(addr + 4u);
    uint32_t slot_index = intnum == 0x10u ? 0u :
                          intnum == 0x21u ? 1u :
                          intnum == 0x31u ? 2u : 3u;
    r36sx_pm_diag_idt_slot_t *slot = &slots[slot_index];

    /*
     * Log only when the effective gate changes.  DOS extenders call BIOS and
     * DOS services thousands of times, but the IDT gate itself usually changes
     * only during real/protected-mode bridge setup.
     */
    if (slot->valid &&
        slot->intnum == intnum &&
        slot->idtr_base == r36sx_idtr_base &&
        slot->idtr_limit == r36sx_idtr_limit &&
        slot->lo == lo &&
        slot->hi == hi) {
        return;
    }

    slot->valid = 1u;
    slot->intnum = intnum;
    slot->idtr_base = r36sx_idtr_base;
    slot->idtr_limit = r36sx_idtr_limit;
    slot->lo = lo;
    slot->hi = hi;

    uint8_t access = (uint8_t)((hi >> 8) & 0xffu);
    uint8_t type = access & 0x0fu;
    uint8_t dpl = (access >> 5) & 3u;
    uint8_t present = (access & R36SX_DESCRIPTOR_PRESENT) != 0u;
    uint16_t selector = (uint16_t)(lo >> 16);
    uint32_t offset = (lo & 0xffffu) |
                      ((type == 0x0eu || type == 0x0fu)
                           ? (hi & 0xffff0000u)
                           : 0u);

    r36sx_pico286_debug_log(
        "[PM] IDT[%02X] reason=%s idtr=%08lX:%04X gate=%08lX:%08lX "
        "type=%02X dpl=%u p=%u sel=%04X off=%08lX fault=%04X:%08lX",
        intnum, software_int ? "soft" : "cpu",
        (unsigned long)r36sx_idtr_base, r36sx_idtr_limit,
        (unsigned long)hi, (unsigned long)lo,
        type, dpl, present, selector, (unsigned long)offset,
        CPU_CS, (unsigned long)fault_ip);

    if ((selector & 0xfffcu) != 0u) {
        r36sx_segment_cache_t target;
        memset(&target, 0, sizeof(target));
        if (r36sx_cpu_decode_descriptor_any(selector, &target)) {
            r36sx_pico286_debug_log(
                "[PM] IDT[%02X] target sel=%04X base=%08lX limit=%08lX "
                "access=%02X flags=%02X valid=%u dpl=%u",
                intnum, selector, (unsigned long)target.base,
                (unsigned long)target.limit, target.access, target.flags,
                target.valid, r36sx_descriptor_dpl(&target));
        } else {
            r36sx_pico286_debug_log(
                "[PM] IDT[%02X] target sel=%04X unavailable "
                "gdtr=%08lX:%04X ldtr=%04X base=%08lX limit=%08lX valid=%u",
                intnum, selector, (unsigned long)r36sx_gdtr_base,
                r36sx_gdtr_limit, r36sx_ldtr_selector,
                (unsigned long)r36sx_ldtr_cache.base,
                (unsigned long)r36sx_ldtr_cache.limit,
                r36sx_ldtr_cache.valid);
        }
    }
}
#else
static inline void r36sx_pm_diag_log_idt_gate_if_interesting(
    uint8_t intnum, uint8_t software_int, uint32_t fault_ip)
{
    (void)intnum;
    (void)software_int;
    (void)fault_ip;
}
#endif

typedef enum {
    R36SX_CPU_DF_CLASS_BENIGN = 0,
    R36SX_CPU_DF_CLASS_CONTRIBUTORY,
    R36SX_CPU_DF_CLASS_PAGE_FAULT
} r36sx_cpu_df_class_t;

static r36sx_cpu_df_class_t r36sx_cpu_exception_df_class(uint8_t intnum)
{
    /*
     * Intel 80386 PRM, Table 9-3/9-4.  #DF is raised only for selected
     * exception pairs; most nested exceptions are delivered serially.
     */
    switch (intnum) {
    case R36SX_EXCEPTION_PF:
        return R36SX_CPU_DF_CLASS_PAGE_FAULT;

    case 0u:
    case 9u:
    case R36SX_EXCEPTION_INVALID_TSS:
    case R36SX_EXCEPTION_NOT_PRESENT:
    case R36SX_EXCEPTION_STACK:
    case R36SX_EXCEPTION_GP:
        return R36SX_CPU_DF_CLASS_CONTRIBUTORY;

    default:
        return R36SX_CPU_DF_CLASS_BENIGN;
    }
}

static uint8_t r36sx_cpu_exception_pair_causes_df(uint8_t first,
                                                  uint8_t second)
{
    r36sx_cpu_df_class_t first_class =
        r36sx_cpu_exception_df_class(first);
    r36sx_cpu_df_class_t second_class =
        r36sx_cpu_exception_df_class(second);

    if (first_class == R36SX_CPU_DF_CLASS_CONTRIBUTORY) {
        return second_class == R36SX_CPU_DF_CLASS_CONTRIBUTORY;
    }
    if (first_class == R36SX_CPU_DF_CLASS_PAGE_FAULT) {
        return second_class == R36SX_CPU_DF_CLASS_CONTRIBUTORY ||
               second_class == R36SX_CPU_DF_CLASS_PAGE_FAULT;
    }
    return 0;
}

static uint8_t r36sx_cpu_protected_interrupt(uint8_t intnum,
                                             uint32_t error_code,
                                             uint8_t has_error_code,
                                             uint8_t software_int,
                                             uint32_t fault_ip,
                                             uint8_t raise_delivery_faults)
{
    uint32_t gate_offset = (uint32_t)intnum * 8u;
    /* IDT selector-style error code: vector index plus the IDT source bit. */
    uint32_t gate_error = ((uint32_t)intnum << 3) | 0x02u;
    r36sx_pm_diag_log_idt_gate_if_interesting(
        intnum, software_int, fault_ip);
    if (r36sx_cpu_exception_delivery_should_abort()) {
        return 0;
    }

    if (gate_offset + 7u > r36sx_idtr_limit) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt IDT limit fault int=%02x idtr=%08lx:%04x",
            intnum, (unsigned long)r36sx_idtr_base, r36sx_idtr_limit);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt IDT limit", CPU_IP);
        /*
         * Intel raises #GP(IDT selector error-code) when an interrupt vector
         * is outside the IDT limit.  When this routine is already trying to
         * deliver another exception, leave double-fault/triple-fault handling
         * to the caller instead of recursively raising #GP forever.
         */
        if (raise_delivery_faults) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1,
                                      fault_ip);
        }
        return 0;
    }

    uint32_t addr = r36sx_idtr_base + gate_offset;
    uint32_t lo = r36sx_cpu_system_read_linear32(addr);
    if (r36sx_cpu_exception_delivery_should_abort()) {
        return 0;
    }
    uint32_t hi = r36sx_cpu_system_read_linear32(addr + 4u);
    if (r36sx_cpu_exception_delivery_should_abort()) {
        return 0;
    }
    uint8_t access = (uint8_t)((hi >> 8) & 0xffu);
    uint8_t type = access & 0x0fu;
    uint8_t gate16 = type == 0x06u || type == 0x07u;
    uint8_t gate32 = type == 0x0eu || type == 0x0fu;
    uint8_t task_gate = type == R36SX_DESCRIPTOR_TYPE_TASK_GATE;
    uint8_t gate_dpl = (access >> 5) & 3u;
    if (task_gate) {
        if ((access & R36SX_DESCRIPTOR_PRESENT) == 0) {
#if R36SX_DEBUG_PM_VERBOSE
            r36sx_pico286_debug_log(
                "[CPU] protected interrupt task gate not present int=%02x",
                intnum);
#endif
            r36sx_pm_diag_log_first_fault("protected task gate not present",
                                          CPU_IP);
            if (raise_delivery_faults) {
                r36sx_cpu_raise_exception(R36SX_EXCEPTION_NOT_PRESENT,
                                          gate_error, 1, fault_ip);
            }
            return 0;
        }
        if (software_int && r36sx_cpu_cpl() > gate_dpl) {
            /*
             * A denied software INT is a #GP fault.  Intel fault semantics
             * return to the INT opcode itself, while a successful INT pushes
             * the next EIP.  CPU_IP is already past the opcode here, so use
             * the decoder-supplied fault_ip for the exception path.
             */
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1,
                                      fault_ip);
            return 0;
        }
        uint8_t switched = r36sx_cpu_task_switch(
            (uint16_t)(lo >> 16), R36SX_TASK_SWITCH_INTERRUPT);
        if (switched && has_error_code) {
            /*
             * Intel pushes an exception error code on the new task stack after
             * a task-gate switch.  DOS extenders such as DOS/16M may install
             * task-gate exception handlers and expect this word/dword there.
             */
            if (r36sx_cpu_stack_default32()) {
                push32(error_code);
            } else {
                push((uint16_t)error_code);
            }
        }
        return switched;
    }

    if ((access & R36SX_DESCRIPTOR_PRESENT) == 0) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt gate not present int=%02x access=%02x",
            intnum, access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt gate not present",
                                      CPU_IP);
        if (raise_delivery_faults) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_NOT_PRESENT,
                                      gate_error, 1, fault_ip);
        }
        return 0;
    }

    if (!(gate16 ||
          (gate32 && r36sx_cpu_descriptor_uses_386_format()))) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt unsupported gate int=%02x access=%02x",
            intnum, access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt gate unsupported",
                                      CPU_IP);
        if (raise_delivery_faults) {
            r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1,
                                      fault_ip);
        }
        return 0;
    }

    if (software_int && r36sx_cpu_cpl() > gate_dpl) {
        r36sx_cpu_raise_exception(R36SX_EXCEPTION_GP, gate_error, 1,
                                  fault_ip);
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
                                          &target_cs, &new_cpl,
                                          fault_ip)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt invalid target CS int=%02x selector=%04x access=%02x",
            intnum, selector, target_cs.access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt target CS invalid",
                                      CPU_IP);
        return 0;
    }

    if (old_vm86 &&
        (r36sx_descriptor_is_conforming_code(&target_cs) || new_cpl != 0u)) {
        /*
         * Intel 80386 leaves VM86 mode only through an interrupt/trap gate
         * targeting a privilege-level-0 protected handler.  A conforming code
         * segment or any non-zero target CPL is a #GP on the gate's CS
         * selector, not a TSS stack fault.
         */
        r36sx_cpu_raise_selector_fault_at(R36SX_EXCEPTION_GP, selector,
                                          fault_ip);
        return 0;
    }

    uint16_t old_ss = CPU_SS;
    uint32_t old_sp = r36sx_cpu_stack_pointer_value();
    uint16_t old_es = CPU_ES;
    uint16_t old_ds = CPU_DS;
    uint16_t old_fs = CPU_FS;
    uint16_t old_gs = CPU_GS;
    uint32_t old_flags_dword = makeflagsdword();
    uint16_t old_flags_word = makeflagsword();

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
        /*
         * Intel privilege transitions load the inner stack before building the
         * interrupt frame.  The frame writes are performed with the target CPL:
         * otherwise a ring-3 -> ring-0 interrupt into a supervisor page would
         * incorrectly look like a user-mode write and raise #PF(P/W/U).
         */
        r36sx_cpu_current_cpl = new_cpl & 3u;
    }

    if (old_vm86) {
        /*
         * VM86 exception delivery switches to a protected-mode handler stack.
         * Keep the VM bit only in the saved EFLAGS image; stack frame writes
         * themselves must use the target protected SS descriptor, not VM86's
         * real-mode segment translation.
         */
        x86_flags.value &= ~(R36SX_EFLAGS_VM_MASK | R36SX_EFLAGS_RF_MASK);
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
        push32(old_flags_dword);
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
        push(old_flags_word);
        push(CPU_CS);
        push(CPU_IP);
        if (has_error_code) {
            push((uint16_t)error_code);
        }
    }

#if R36SX_DEBUG_PM_DIAG
    if (old_vm86 && intnum == R36SX_EXCEPTION_GP) {
        static uint32_t vm86_gp_frames;
        if (vm86_gp_frames < 16u) {
            vm86_gp_frames++;
            r36sx_pico286_debug_log(
                "[PM] VM86 #GP frame fault=%04X:%08lX old_sp=%04X:%08lX "
                "old_flags=%08lX oldsegs=%04X/%04X/%04X/%04X "
                "new_ss=%04X sp_after=%08lX err=%08lX gate32=%u",
                CPU_CS, (unsigned long)fault_ip, old_ss,
                (unsigned long)old_sp, (unsigned long)old_flags_dword,
                old_es, old_ds, old_fs, old_gs, CPU_SS,
                (unsigned long)r36sx_cpu_stack_pointer_value(),
                (unsigned long)error_code, gate32);
        }
    }
#endif

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

#if R36SX_DEBUG_PM_DIAG
static uint8_t r36sx_pm_diag_raw_code_byte(uint32_t offset)
{
    /*
     * Diagnostic only: read instruction bytes through the current CS hidden
     * base without using getmem(), so a fault report cannot recursively raise
     * another segment exception while we are already reporting one.
     */
    uint32_t base = r36sx_seg_cache[regcs].valid ?
                    r36sx_seg_cache[regcs].base :
                    ((uint32_t)CPU_CS << 4);
    return read86_ob(base + offset);
}

static void r36sx_pm_diag_log_segment_cache(const char *name,
                                            const r36sx_segment_cache_t *cache)
{
    r36sx_pico286_debug_log(
        "[PM] seg %s sel=%04X base=%08lX limit=%08lX access=%02X flags=%02X valid=%u",
        name, cache->selector, (unsigned long)cache->base,
        (unsigned long)cache->limit, cache->access, cache->flags,
        cache->valid);
}

static void r36sx_pm_diag_log_exception_context(uint8_t intnum,
                                                uint32_t error_code,
                                                uint8_t has_error_code,
                                                uint32_t fault_ip)
{
    if (intnum != R36SX_EXCEPTION_GP &&
        intnum != R36SX_EXCEPTION_STACK &&
        intnum != R36SX_EXCEPTION_NOT_PRESENT &&
        intnum != R36SX_EXCEPTION_INVALID_TSS) {
        return;
    }

    uint8_t b0 = r36sx_pm_diag_raw_code_byte(fault_ip + 0u);
    uint8_t b1 = r36sx_pm_diag_raw_code_byte(fault_ip + 1u);
    uint8_t b2 = r36sx_pm_diag_raw_code_byte(fault_ip + 2u);
    uint8_t b3 = r36sx_pm_diag_raw_code_byte(fault_ip + 3u);
    uint8_t b4 = r36sx_pm_diag_raw_code_byte(fault_ip + 4u);
    uint8_t b5 = r36sx_pm_diag_raw_code_byte(fault_ip + 5u);
    uint8_t b6 = r36sx_pm_diag_raw_code_byte(fault_ip + 6u);
    uint8_t b7 = r36sx_pm_diag_raw_code_byte(fault_ip + 7u);

    r36sx_pico286_debug_log(
        "[PM] exception ctx int=%02X err=%08lX cs:eip=%04X:%08lX bytes=%02X %02X %02X %02X %02X %02X %02X %02X",
        intnum, (unsigned long)error_code, CPU_CS, (unsigned long)fault_ip,
        b0, b1, b2, b3, b4, b5, b6, b7);
    r36sx_pico286_debug_log(
        "[PM] regs ax=%04X bx=%04X cx=%04X dx=%04X si=%04X di=%04X "
        "bp=%04X sp=%04X flags=%04X eflags=%08lX cpl=%u vm=%u iopl=%u",
        CPU_AX, CPU_BX, CPU_CX, CPU_DX, CPU_SI, CPU_DI, CPU_BP, CPU_SP,
        makeflagsword(), (unsigned long)makeflagsdword(), r36sx_cpu_cpl(),
        r36sx_cpu_v86_enabled(), r36sx_cpu_iopl());
    r36sx_pm_diag_log_segment_cache("cs", &r36sx_seg_cache[regcs]);
    r36sx_pm_diag_log_segment_cache("ss", &r36sx_seg_cache[regss]);
    r36sx_pm_diag_log_segment_cache("ds", &r36sx_seg_cache[regds]);
    r36sx_pm_diag_log_segment_cache("es", &r36sx_seg_cache[reges]);

    if (has_error_code && (error_code & 0xfffcu) != 0u &&
        (error_code & 0x02u) == 0u) {
        uint16_t selector = (uint16_t)(error_code & 0xfffcu);
        r36sx_segment_cache_t descriptor;
        memset(&descriptor, 0, sizeof(descriptor));
        if (r36sx_cpu_decode_descriptor_any(selector, &descriptor)) {
            r36sx_pico286_debug_log(
                "[PM] err selector desc sel=%04X base=%08lX limit=%08lX access=%02X flags=%02X valid=%u",
                selector, (unsigned long)descriptor.base,
                (unsigned long)descriptor.limit, descriptor.access,
                descriptor.flags, descriptor.valid);
        } else {
            r36sx_pico286_debug_log(
                "[PM] err selector desc sel=%04X unavailable gdtr=%08lX:%04X ldtr=%04X base=%08lX limit=%08lX valid=%u",
                selector, (unsigned long)r36sx_gdtr_base, r36sx_gdtr_limit,
                r36sx_ldtr_selector, (unsigned long)r36sx_ldtr_cache.base,
                (unsigned long)r36sx_ldtr_cache.limit, r36sx_ldtr_cache.valid);
        }
    }
}
#endif

static void r36sx_cpu_raise_exception(uint8_t intnum,
                                      uint32_t error_code,
                                      uint8_t has_error_code,
                                      uint32_t fault_ip)
{
    if (r36sx_cpu_triple_fault_latched) {
        r36sx_cpu_exception_pending = 1u;
        return;
    }

    if (r36sx_cpu_protected_enabled() &&
        r36sx_cpu_exception_delivery_depth != 0u) {
        uint8_t active = r36sx_cpu_exception_delivery_vector;
        if (active == R36SX_EXCEPTION_DOUBLE_FAULT ||
            intnum == R36SX_EXCEPTION_DOUBLE_FAULT) {
            r36sx_cpu_latch_triple_fault(
                intnum, error_code, has_error_code, fault_ip);
            return;
        }

        r36sx_cpu_exception_abort_delivery_depth =
            r36sx_cpu_exception_delivery_depth;
        if (r36sx_cpu_exception_pair_causes_df(active, intnum)) {
            r36sx_pico286_debug_log(
                "[PM] exception delivery fault active=%02X nested=%02X "
                "err=%08lX -> #DF",
                active, intnum, (unsigned long)error_code);
            intnum = R36SX_EXCEPTION_DOUBLE_FAULT;
            error_code = 0;
            has_error_code = 1u;
        } else {
            r36sx_pico286_debug_log(
                "[PM] exception delivery fault active=%02X nested=%02X "
                "err=%08lX -> serial",
                active, intnum, (unsigned long)error_code);
        }
    }

    r36sx_cpu_exception_pending = 1u;
    r36sx_cpu_set_ip(fault_ip);
#if R36SX_DEBUG_PM_DIAG
    static uint8_t last_intnum;
    static uint32_t last_error_code;
    static uint8_t last_has_error_code;
    static uint16_t last_cs;
    static uint32_t last_ip;
    static uint32_t repeat_count;
    uint8_t repeated = repeat_count &&
                       last_intnum == intnum &&
                       last_error_code == error_code &&
                       last_has_error_code == has_error_code &&
                       last_cs == CPU_CS &&
                       last_ip == CPU_IP;

    if (repeated) {
        repeat_count++;
    } else {
        if (repeat_count > 8u) {
            r36sx_pico286_debug_log(
                "[PM] exception repeat suppressed count=%lu",
                (unsigned long)(repeat_count - 8u));
        }
        last_intnum = intnum;
        last_error_code = error_code;
        last_has_error_code = has_error_code;
        last_cs = CPU_CS;
        last_ip = CPU_IP;
        repeat_count = 1u;
    }

    /*
     * Keep only the first few identical exception records.  A broken protected
     * exception path can otherwise fill the 2 MB log before the useful first
     * fault and descriptor diagnostics survive.
     */
    if (repeat_count <= 8u) {
        r36sx_pico286_debug_log(
            "[PM] exception int=%02X err=%08lX has_err=%u cs:eip=%04X:%08lX",
            intnum, (unsigned long)error_code, has_error_code,
            CPU_CS, (unsigned long)CPU_IP);
        if (repeat_count == 1u) {
            r36sx_pm_diag_log_exception_context(
                intnum, error_code, has_error_code, fault_ip);
        }
    }
#endif

    if (r36sx_cpu_protected_enabled()) {
        uint8_t saved_depth = r36sx_cpu_exception_delivery_depth;
        uint8_t saved_vector = r36sx_cpu_exception_delivery_vector;
        uint8_t handled;
        uint8_t aborted;

        r36sx_cpu_exception_delivery_depth = saved_depth + 1u;
        r36sx_cpu_exception_delivery_vector = intnum;
        handled = r36sx_cpu_protected_interrupt(
            intnum, error_code, has_error_code, 0, fault_ip, 1);
        aborted = r36sx_cpu_exception_abort_delivery_depth != 0u;
        r36sx_cpu_exception_delivery_vector = saved_vector;
        r36sx_cpu_exception_delivery_depth = saved_depth;

        if (!handled) {
            r36sx_pm_diag_log_first_fault("protected exception delivery",
                                          fault_ip);
            if (intnum == R36SX_EXCEPTION_DOUBLE_FAULT) {
                r36sx_cpu_latch_triple_fault(
                    intnum, error_code, has_error_code, fault_ip);
            } else if (saved_depth == 0u && !aborted &&
                       !r36sx_cpu_triple_fault_latched) {
                r36sx_pico286_debug_log(
                    "[PM] exception delivery failed int=%02X -> #DF",
                    intnum);
                r36sx_cpu_raise_exception(
                    R36SX_EXCEPTION_DOUBLE_FAULT, 0, 1, fault_ip);
            }
        }
        if (saved_depth == 0u) {
            r36sx_cpu_exception_abort_delivery_depth = 0;
        }
        return;
    }

    intcall86(intnum);
}

