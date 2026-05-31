/*
 * Tiny MIPS DSP probe object.
 *
 * The normal build compiles this function as a plain move.  The experimental
 * DSP build compiles the same symbol with `addu.ph` and `addu.qb`, making it
 * easy to verify DSP ASE Rev2 code generation with objdump without depending
 * only on inlined C.
 */

    .text
    .align 2
    .set nomips16
    .set noreorder
    .globl r36sx_mips_dsp_probe
    .ent r36sx_mips_dsp_probe
r36sx_mips_dsp_probe:
#if R36SX_MIPS_DSP && defined(__mips_dspr2)
    addu.ph $v0,$a0,$zero
    addu.qb $v0,$v0,$zero
#else
    move $v0,$a0
#endif
    jr $ra
    nop
    .end r36sx_mips_dsp_probe
