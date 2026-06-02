/*
 * This file is included by r36sx_cpu.c and intentionally has no include
 * guard: the CPU core keeps these helpers in one translation unit so the
 * compiler can still inline hot interpreter paths.
 */

static uint8_t r36sx_cpu_protected_interrupt(uint8_t intnum,
                                             uint32_t error_code,
                                             uint8_t has_error_code)
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

    if ((access & R36SX_DESCRIPTOR_PRESENT) == 0 ||
        !(type == 0x06u || type == 0x07u ||
          type == 0x0eu || type == 0x0fu)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt unsupported gate int=%02x access=%02x",
            intnum, access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt gate unsupported",
                                      CPU_IP);
        return 0;
    }

    uint16_t selector = (uint16_t)(lo >> 16);
    uint32_t offset = (lo & 0xffffu) |
                      ((type >= 0x0eu) ? (hi & 0xffff0000u) : 0u);

    r36sx_segment_cache_t target_cs;
    memset(&target_cs, 0, sizeof(target_cs));
    if (!r36sx_cpu_decode_descriptor(selector, &target_cs) ||
        !r36sx_descriptor_is_code(&target_cs)) {
#if R36SX_DEBUG_PM_VERBOSE
        r36sx_pico286_debug_log(
            "[CPU] protected interrupt invalid target CS int=%02x selector=%04x access=%02x",
            intnum, selector, target_cs.access);
#endif
        r36sx_pm_diag_log_first_fault("protected interrupt target CS invalid",
                                      CPU_IP);
        return 0;
    }

    if (type >= 0x0eu) {
        push32(makeflagsdword());
        push32(CPU_CS);
        push32(CPU_IP);
        if (has_error_code) {
            push32(error_code);
        }
    } else {
        push(makeflagsword());
        push(CPU_CS);
        push(CPU_IP);
        if (has_error_code) {
            push((uint16_t)error_code);
        }
    }

    r36sx_cpu_commit_segment_cache(regcs, selector, &target_cs);
    r36sx_cpu_set_ip(offset);
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
        if (!r36sx_cpu_protected_interrupt(
                intnum, error_code, has_error_code)) {
            r36sx_pm_diag_log_first_fault("protected exception delivery",
                                          fault_ip);
        }
        return;
    }

    intcall86(intnum);
}

static uint8_t r36sx_cpu_handle_vcpi(void)
{
    if ((CPU_AX & 0xff00u) != 0xde00u) {
        return 0;
    }

    /*
     * VCPI is supplied by an EMS/V86 control program such as EMM386.  Pico-286
     * currently boots real-mode DOS directly, so claiming a live VCPI server
     * would send DOS extenders into an interface we cannot yet back with V86,
     * paging, or a real protected-mode entry point.
     */
    switch (CPU_AL) {
        case 0x00: /* Installation check */
            R36SX_PM_DIAG_LOG(
                "[PM] VCPI DE00 installation check: not present");
            CPU_AH = 0x80u;
            CPU_BH = 0;
            CPU_BL = 0;
            return 1;
        case 0x01: /* Get protected-mode interface */
        case 0x02: /* Get maximum physical memory address */
        case 0x03: /* Get number of free 4K pages */
        case 0x04: /* Allocate a 4K page */
        case 0x05: /* Free a 4K page */
        case 0x06: /* Get physical address of page in first MB */
        case 0x07: /* Read CR0 */
        case 0x08: /* Read debug registers */
        case 0x09: /* Set debug registers */
        case 0x0A: /* Get 8259 interrupt vector mappings */
        case 0x0B: /* Set 8259 interrupt vector mappings */
        case 0x0C: /* Switch to protected mode */
            R36SX_PM_DIAG_LOG(
                "[PM] VCPI DE%02X requested while VCPI server absent",
                CPU_AL);
            CPU_AH = 0x80u;
            return 1;
        default:
            R36SX_PM_DIAG_LOG(
                "[PM] VCPI DE%02X unsupported subfunction", CPU_AL);
            CPU_AH = 0x8Fu;
            return 1;
    }
}
