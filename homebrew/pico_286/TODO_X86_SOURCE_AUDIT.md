# x86 Source Audit TODO

Audit date: 2026-09-05. Baseline: `28c46e23` on `single-loop`.

This is a separate backlog from [TODO_386_INSTRUCTIONS.md](TODO_386_INSTRUCTIONS.md).
An implemented opcode is not necessarily correct for every operand size,
address size, CPU generation, segment, privilege level, or exception path.
Previously checked boxes in that older list do not close the findings below.

No production CPU fixes, emulator rebuild, ROM run, or patch deployment were
performed for this audit. The user's patch configuration was left untouched.

## Scope and evidence

The review walked the integer primary-opcode dispatch, the 0F dispatch, 32-bit
operand handlers, and their shared memory, stack, flags, and task helpers.
It also inspected the specialized 8086/286 dispatch wiring and the x87 memory
interface. It is not an exhaustive execution-based conformance certification;
the full x87 arithmetic implementation and bus timing were not audited.

File abbreviations below are relative to `homebrew/pico_286/`:

| Name | File |
| --- | --- |
| CPU | `r36sx_port/r36sx_cpu.c` |
| CORE | `r36sx_port/r36sx_cpu_exec_core.inl` |
| I86 | `r36sx_port/r36sx_cpu_8086.inl` |
| I286 | `r36sx_port/r36sx_cpu_80286.inl` |
| I386 | `r36sx_port/r36sx_cpu_80386.inl` |
| IRQ | `r36sx_port/r36sx_cpu_80286_interrupts.inl` |
| REAL386 | `r36sx_port/r36sx_cpu_80386_real.inl` |
| FPU | `pico-286/src/emulator/fpu.c` |

Line numbers refer to the baseline; function names are the durable anchors.
`Probe` means reproduced with extracted production C and observable stubs.
`Source` means a concrete source-level mismatch, still needing an integrated
guest regression test. `Investigate` is deliberately not a confirmed defect.
P1 items risk host failure, guest state corruption, or broken exception delivery;
P2 items are narrower instruction/conformance gaps.

## Confirmed findings

- [ ] **X86-01 / P1 / Probe: IDIV16 can overflow the host division.**
  I86:551, `op_idiv16`: `INT32_MIN / -1` is evaluated before the guest quotient
  range check. GCC UBSan reports undefined behavior instead of a guest #DE.
  Check this case before C division and preserve guest results on failure.
  Test `DX:AX=8000:0000`, divisor `FFFFh`. [Intel IDIV][idiv].

- [ ] **X86-02 / P1 / Probe: IDIV32 has the same host-overflow problem.**
  I386:211, `op_idiv32`: `INT64_MIN / -1` reaches undefined C division.
  Also remove the negative signed left shift used to assemble EDX:EAX in
  `op_grp3_32` near line 288. Build the bit pattern with unsigned arithmetic.
  Test `EDX:EAX=80000000:00000000`, divisor `FFFFFFFFh`. [Intel IDIV][idiv].

- [ ] **X86-03 / P1 / Probe: XCHG recalculates its memory operand after changing a register.**
  CORE:2143/2157 and I386:505 change the register between `readrm*` and
  `writerm*`; CPU:4200 and following recalculate EA on each access.
  With EAX=1000h and [1000h]=2000h, `XCHG EAX,[EAX]` writes at 2000h.
  Resolve the original address once; cover byte/word address-register aliases
  as well as the dword case. [Intel XCHG][xchg].

- [ ] **X86-04 / P1 / Source: a fault does not reliably stop the current instruction.**
  CPU:1169/4200 checked reads return sentinel values after raising an exception.
  IRQ:527, `r36sx_cpu_raise_exception`, immediately enters the handler, but
  callers such as CORE:2204 and I386's MOV/ALU handlers continue assigning
  registers or advancing IP. CORE's epilogue has no general unwind/rollback.
  Introduce explicit instruction-abort semantics. Test a faulting MOV and an
  immediate-fetch #PF: neither may modify the new handler's registers/IP.
  [Intel exception classification][faults], [Intel MOV][mov].

- [ ] **X86-05 / P1 / Source: stack operations commit SP and results too early.**
  CPU:2504, `push`, `pop`, `push32`, `pop32`, update SP before a successful
  write or unconditionally after a potentially faulting read. The 286 helpers
  have the same pattern. PUSHA/POPA, ENTER/LEAVE, RETF and IRET compound it.
  Stage architectural updates and validate the required frame; test #SS/#PF
  on first and later frame slots, including expand-down stacks and SS.B=0/1.
  [Intel PUSH][push], [Intel POP][pop], [Intel ENTER][enter].

- [ ] **X86-06 / P1 / Probe: the generic REP batch does not wrap 16-bit indexes per element.**
  CPU:1666, `r36sx_rep_movsb` and adjacent MOVSW/STOS helpers keep SI/DI in
  uint32_t locals and truncate only when committing registers. Starting
  SI=FFFFh, two MOVSB iterations read FFFFh then 10000h instead of 0000h.
  Fix the 386 addr16 fallback for both DF directions and operand widths.
  The specialized 8086/286 loops already use uint16_t indexes; do not regress
  them. [Intel MOVS][movs], [Intel REP][rep].

- [ ] **X86-07 / P1 / Source: REP batches continue after a fault.**
  CPU:1666 and CPU:7574 loops lack per-element abort checks; CORE:2516 and
  neighboring string cases consume the planned batch count, not the number
  actually completed. A fault can therefore corrupt memory, CX/ECX, indexes,
  or the handler IP. Commit progress only for completed iterations and stop
  before the first failed element. Test a page/segment boundary mid-batch.
  [Intel REP restart rules][rep].

- [ ] **X86-08 / P1 / Source: segment cache lookup loses segment-register identity.**
  CPU:712, `r36sx_cpu_find_segment_cache`, selects by the visible selector;
  I286:246, `r36sx_cpu_segbase`, also searches selector matches.
  DS and SS can contain equal selectors yet have different hidden caches after
  a descriptor edit/reload. Lookup can use the wrong base/limit or report #SS
  for a DS access. Pass segment-register identity through memory helpers.
  Test equal selectors with distinct cached descriptors. [Intel 5.1.4][segments].

- [ ] **X86-09 / P1 / Source: a CS-selector data read is mistaken for instruction fetch.**
  CPU:1164, `r36sx_cpu_selector_is_current_cs`, classifies reads solely by
  selector equality. The 286 equivalent does likewise. Reading through an
  execute-only CS must not inherit execute permission just because the
  selector matches CS. Keep fetch and data-read access kinds distinct.
  Test a CS override against an execute-only descriptor. [Intel type checks][protection].

- [ ] **X86-10 / P1 / Source: real/v86 paths bypass architectural segment limits.**
  CPU:807 returns early for real mode or VM86; CPU:7071 and REAL386 have
  comparable fast-path omissions. With a normal 64 KB cache, addr32 offsets
  beyond FFFFh and multi-byte accesses crossing the limit must fault on the
  applicable 286/386 paths. Sequential fetch must not silently wrap either.
  Preserve the different 8086 rules and retained real-mode hidden-cache state.
  [Intel real-mode differences][real-differences], [Intel v86 addressing][v86].

- [ ] **X86-11 / P1 / Source: control-transfer width is conflated with CS.D.**
  I286:278, `r36sx_cpu_mask_ip`, chooses truncation from code-default size;
  callers use it for near CALL/JMP/Jcc targets. Operand-size overrides need
  their own target-width handling. The word group-5 helper also writes the
  low `ip` alias without necessarily clearing upper EIP.
  Test 66h near branches in both D=0 and D=1 code, including a D=0 descriptor
  whose limit permits targets above FFFFh. [Intel JMP][jmp], [Intel Jcc][jcc].

- [ ] **X86-12 / P1 / Source: near transfers lack target-limit validation before commit.**
  I386:920 pushes a return address before checking the destination; word
  group-5 and relative branch paths similarly commit the target directly.
  A later fetch fault is not equivalent to a fault on CALL/JMP/Jcc itself.
  Validate a taken target before changing EIP or the return stack.
  [Intel CALL][call], [Intel JMP][jmp].

- [ ] **X86-13 / P1 / Source: call-gate stack writes use the old CPL.**
  CPU:3794 installs the inner-level SS/SP and pushes the frame, but does not
  change CPL until `r36sx_cpu_commit_code_transfer` at line 3805.
  A ring-3 to ring-0 call can consequently fault on a valid supervisor stack.
  Use the destination privilege for the new stack accesses while retaining
  correct fault/restart state. Test paging with a supervisor-only ring-0 stack.
  [Intel CALL gates][call], [Intel page protection][paging].

- [ ] **X86-14 / P1 / Source: task-return IRET skips saving the outgoing TSS.**
  CPU:3562 explicitly skips `r36sx_cpu_save_tss_state` for
  `R36SX_TASK_SWITCH_IRET`. The outgoing task must still be saved, with the
  appropriate NT/busy changes. Test 16/32-bit nested tasks and reenter the
  returned-from task to check its saved registers and post-IRET IP.
  [Intel task switching][tasks], [Intel IRET][iret].

- [ ] **X86-15 / P1 / Source: incoming-task validation runs with inconsistent task context.**
  CPU:3392, `r36sx_cpu_load_task_state`, changes general registers/CR3 before
  validating incoming selectors, but installs TR later. It also calls normal
  LLDT validation for a TSS-provided LDT selector. Incoming-LDT faults can use
  the wrong exception type and task context. Separate outgoing-task checks
  from incoming-task faults; test invalid/not-present LDT, CS and SS, with
  paging enabled. [Intel 7.5 and Table 7-1][tasks].

- [ ] **X86-16 / P1 / Source: IRET to a more privileged RPL silently returns failure.**
  CPU:4162 short-circuits on `new_cpl < old_cpl` without raising #GP, after
  popping the frame. Raise the proper selector error and preserve restart
  state. Test ring-3 IRET with a ring-0 return selector. [Intel IRET][iret].

- [ ] **X86-17 / P2 / Source: IRET rejects NT in the saved flags image.**
  CPU:4115 unconditionally raises #GP for a stacked NT bit. Current NT
  selects task-return behavior; a popped NT bit is not this blanket rejection
  condition. Test current NT=0 with stacked NT=1. [Intel IRET][iret].

- [ ] **X86-18 / P2 / Source: outer-level IRET restores flags using the new CPL.**
  CPU:4187 commits the code transfer before `decodeflagsword/dword`, whose
  privilege tests use current CPL. Preserve the executing CPL for IF/IOPL
  restoration. Test a ring-0 return to ring 3 with changed IF/IOPL.
  [Intel IRET][iret].

- [ ] **X86-19 / P1 / Source: protected IDT gate validation is incomplete and misordered.**
  IRQ:218 uses only the low type nibble, without rejecting S=1. Presence
  checks also precede type and software-INT DPL checks. An invalid access
  byte such as 9Eh can pass as a gate; combined errors give the wrong fault.
  Validate the full descriptor class and Intel's check order. Include task
  gates and selector/IDT error-code bits in the regression matrix.
  [Intel INT][int], [Intel IDT descriptors][idt].

- [ ] **X86-20 / P1 / Source: interrupt/trap-gate entry does not clear NT.**
  IRQ:430 clears VM/RF and later TF/IF, but leaves NT. An interrupt of a nested
  task can then make the handler's IRET take an unintended task-return path.
  Preserve NT in the saved image and clear it in the running handler.
  [Intel INT][int].

- [ ] **X86-21 / P2 / Probe: BT/BTS/BTR/BTC use an unsigned memory bit index.**
  I386:979 takes uint32_t `bit_offset` and uses unsigned division for the
  displacement. A register index of -1 should select the preceding word/dword;
  the probes instead access 2FFEh/20000FFCh from base 1000h.
  Implement signed displacement and address-size handling without changing
  the immediate-index form. [Intel BT memory addressing][bt].

- [ ] **X86-22 / P2 / Source: byte/word INS/OUTS ignore address-size 32.**
  CORE:1679 and following use DI/SI/CX directly for INSB/INSW/OUTSB/OUTSW;
  the dword handlers use the proper index/count helpers.
  Test 67h with indexes above FFFFh and ECX=10000h, both REP and single forms.
  [Intel INS][ins], [Intel OUTS][outs].

- [ ] **X86-23 / P2 / Source: operand32 dispatch omits invalid-ModRM checks.**
  I386:527 accepts register-source LEA; line 534 accepts non-/0 POP; line 926
  and the FF /5 path accept register-mode far indirect CALL/JMP. The word
  dispatch has checks which the earlier operand32 path bypasses.
  Test `66 8D C0`, `66 8F C8`, `66 FF D8`, `66 FF E8` in a 16-bit code
  segment. These must not execute as valid forms. [Intel LEA][lea],
  [Intel POP][pop], [Intel CALL][call], [Intel JMP][jmp].

- [ ] **X86-24 / P2 / Source: descriptor-table operands bypass segment checks.**
  I386:1279 and following pass a linear EA to I286:795/803 table helpers
  without validating the six-byte segmented operand. SGDT/SIDT can write a
  read-only/short segment; LGDT/LIDT can read outside its limit. Loads also
  assign the limit before completing the base read. Validate access and stage
  the new table register. Test a six-byte operand across a limit/page boundary.
  [Intel SGDT/SIDT][sgdt], [Intel LGDT/LIDT][lgdt].

- [ ] **X86-25 / P2 / Source: LOCK group 1 incorrectly permits CMP.**
  CPU:6407 allows all memory group-1 forms, including /7. `LOCK CMP [mem],imm`
  must not pass the 386 LOCK filter. Restrict by group operation as already
  done for groups 3/5. Do not apply 386 restrictions blindly to 8086/286.
  [Intel LOCK][lock].

- [ ] **X86-26 / P2 / Source: XLAT addr16 fails to truncate BX+AL.**
  CORE:3362 passes the promoted sum into getmem without a uint16_t wrap.
  CPU:6595 also does not truncate that offset in the 8086 path.
  Test BX=FFFFh, AL=1 with different bytes at DS:0000h and base+10000h.
  [Intel XLAT][xlat].

- [ ] **X86-27 / P2 / Source: 8086 divide errors use the later-CPU return IP.**
  I86:93, `r36sx_cpu_divide_error`, always rewinds to the instruction start.
  8086/8088 divide interrupts save the following IP; 286/386 use faulting IP.
  Make this generation-specific; cover DIV, IDIV and AAM 0 with prefixes.
  [Intel 14.7, item 2][real-differences].

- [ ] **X86-28 / P2 / Source: the decoder has no total instruction-length limit.**
  CORE:118 loops through prefixes without accounting for the complete
  instruction length, including ModRM, displacement and immediate bytes.
  Implement the 386 15-byte limit with a restartable #GP(0). Confirm the
  exact 286 limit separately; leave 8086's different behavior intact.
  [Intel 14.7, item 6][real-differences].

- [ ] **X86-29 / P2 / Source: RF is cleared before successful instruction completion.**
  CPU:895 clears RF at execute-breakpoint checking time; a following fault
  loses restart suppression. Fault entry also needs the correct saved RF
  image. Test breakpoint suppression across a fault and retry, not merely a
  successful NOP. [Intel debug exceptions][debug].

- [ ] **X86-30 / P2 / Source: debug-register support is still partial.**
  CPU:928 and the existing TODO explicitly omit data-read watchpoints.
  MOV DR handlers also need GD handling; task switches need local-enable
  clearing and TSS trap-bit/DR6.BT handling. Keep these as individual fixes
  with fetch-vs-data and post-instruction trap tests. [Intel debug exceptions][debug].

- [ ] **X86-31 / P2 / Source: descriptor accessed-bit writeback is missing.**
  I286:235, `r36sx_cpu_commit_segment_cache`, updates only internal state.
  The reviewed load paths have no corresponding descriptor A-bit writeback;
  page-table accessed-bit handling is a separate mechanism.
  Test MOV/POP segment and far transfers with initially clear A bits, including
  descriptor-table paging faults. [Intel segment descriptors][segments].

- [ ] **X86-32 / P1 / Source: x87 operands bypass CPU memory protection/translation.**
  FPU:81/1161 uses getea followed by global readw86/readdw86/write pointers;
  these are physical backend pointers, not the CPU translation-unit's checked
  helpers. Connect FPU operands to segment/paging checks and fault-abort logic.
  Test FLD/FSTP under nonidentity paging, a read-only segment and a page fault.
  This finding does not certify the separate x87 arithmetic implementation.
  [Intel protection][protection], [Intel paging][paging].

- [ ] **X86-33 / P2 / Source: diagnostic instruction reads can raise guest faults.**
  I86:13/67 and I286:326 use architectural getmem in opcode/context dumps.
  Reading across an unmapped page or segment boundary while reporting a
  fault can mutate CPU state or cause another exception. Use side-effect-free
  diagnostic reads with explicit unreadable-byte markers; compare debug on/off.
  This is an emulator instrumentation defect, not a new guest instruction.

## Investigations not yet established as conformance bugs

- [ ] **CHECK-01:** CPU:2310/2325 writes cross-page operands byte by byte and
  continues after a later fault. X86-04 covers continuing in the handler;
  separately establish the 386 guarantees for partial memory side effects
  before choosing preflight versus rollback, especially for memory-mapped I/O.
- [ ] **CHECK-02:** I386:1076 LSS does not call the interrupt-shadow helper
  used by MOV SS/POP SS. Verify original 386 LSS and debug/NMI inhibition
  rules before changing it. Test pending IRQ and TF across SS loads.
- [ ] **CHECK-03:** IRQ's IDT error construction lacks external-origin EXT
  tracking. Verify nested exception origins and task-gate error-code width
  (TSS format versus SS.B) against Intel exception-delivery rules.
- [ ] **CHECK-04:** CPU:4143 truncates a v86 return EIP through `set_ip`.
  Verify oversized IRETD targets and the exact validation order; do not copy
  the inconsistent v86 pseudocode in the HTML transcription without checking
  the original manual/errata.
- [ ] **CHECK-05:** `r36sx_cpu_save_tss_state` also writes CR3 and LDTR.
  Check which TSS fields are load-only versus saved by real 286/386 hardware;
  preserve software changes to static TSS fields where required.
- [ ] **CHECK-06:** VERR/VERW reuse descriptor decoding with a present check.
  Verify P=0 behavior for the target generation instead of borrowing modern
  CPU rules. Add LAR/LSL/VERR/VERW privilege/type/presence matrices.
- [ ] **CHECK-07:** Duplicate 66h/67h prefixes toggle the effective size.
  Determine the target's documented/undefined-prefix policy before asserting
  last-prefix-wins or changing behavior. Include F7 /1 and other reserved
  encodings whose acceptance differs between word and dword handlers.
- [ ] **CHECK-08:** Early Intel documentation says 8086 IDIV faults on the
  smallest negative quotient. Confirm against an 8086 manual/hardware test;
  do not conflate this with the proven host-overflow bugs X86-01/02.
- [ ] **CHECK-09:** CPU segment checking rejects base+offset uint32 overflow.
  Verify linear-address wrap semantics separately from segment-offset limits.
- [ ] **CHECK-10:** Audit TF/`was_TF` ordering across external IRQs, MOV/POP SS,
  REP iterations and faults, plus NMI blocking until IRET.
- [ ] **CHECK-11:** Define the configured x87 generation. FPU dispatch accepts
  FCMOV/FCOMI and FISTTP, which are later than 80387; arithmetic uses host
  double precision rather than a complete 80-bit model. Do a dedicated x87
  opcode, precision, rounding, exception and 8087/287/387 compatibility audit.
- [ ] **CHECK-12:** Expand the host-language UB/alignment audit to signed
  shifts, overflow, pointer casts and register aliases beyond IDIV. Repeat
  under Windows and MIPS compilers; GCC host probes do not cover either ABI.
- [ ] **CHECK-13:** Run the integrated test286/test386 ROMs and fault-injection
  tests in real, protected and v86 modes. A source pass cannot establish
  correctness of every byte encoding, flag, fault priority or bus timing.

## Reviewed instruction inventory

This records which dispatch families were inspected, not a pass certificate.
Memory forms also inherit X86-04/08/09/10 even if their arithmetic is correct.

| Instructions/family | Result of this source pass |
| --- | --- |
| ADD, ADC, SUB, SBB, CMP, INC, DEC, NEG | Basic handlers inspected; ADC/SBB8 flags exhaustively probed; other widths still need equivalent tests |
| AND, OR, XOR, TEST, NOT | Basic handlers inspected; LOCK CMP gap X86-25; faulting memory paths remain open |
| AAA, AAS, DAA, DAS, AAM, AAD | Handlers inspected; generation-specific divide return X86-27; exhaustive decimal/undefined-input tests not run |
| MUL, IMUL, DIV, IDIV | Handlers inspected; IDIV host overflows X86-01/02 |
| ROL, ROR, RCL, RCR, SAL/SHL, SHR, SAR | Count/flag paths inspected, including generation masking; no exhaustive all-width execution test |
| SHLD, SHRD | Handlers inspected; 1986 manual leaves OF/AF undefined, so missing modern OF behavior was not filed as a proven 386 bug |
| BT, BTS, BTR, BTC, BSF, BSR | Signed memory bit-index bug X86-21; zero-input BSF/BSR destination is architecturally undefined |
| MOV, XCHG, LEA, XLAT, MOVSX, MOVZX, CBW/CWDE, CWD/CDQ | X86-03/23/26; checked memory/fault propagation remains a shared blocker |
| PUSH, POP, PUSHA/PUSHAD, POPA/POPAD, ENTER, LEAVE | Stack restartability X86-05; bad POP encoding X86-23 |
| PUSHF/PUSHFD, POPF/POPFD, LAHF, SAHF | Flag masks/privilege paths inspected; fault handling and IRET restore context still open |
| MOVS, STOS, LODS, CMPS, SCAS, REP/REPE/REPNE | X86-06/07; test all operand/address-size combinations after abort logic is fixed |
| IN, OUT, INS, OUTS | I/O permission paths inspected; byte/word string address-size gap X86-22 |
| Jcc, SETcc | All 512 condition/flag combinations pass; transfer target size/limit and memory-fault issues remain |
| LOOP, LOOPE/LOOPZ, LOOPNE/LOOPNZ, JCXZ/JECXZ | Counter-selection handlers inspected; target/fetch boundaries need integrated tests |
| CALL, JMP, RET/RETF, INT, INTO, IRET | X86-05/11..20; gate/task/exception combinations need broad regressions |
| LDS, LES, LSS, LFS, LGS; MOV/POP segment | Cache/protection gaps X86-08/09/31; LSS shadow CHECK-02 |
| BOUND, ARPL, LAR, LSL, VERR, VERW | Handlers inspected; selector matrix CHECK-06; LAR's old-386 gate types were not incorrectly rejected using modern rules |
| SGDT, SIDT, LGDT, LIDT, SLDT, LLDT, STR, LTR, SMSW, LMSW | X86-24; task-loaded selectors X86-15; real/protected restrictions need runtime matrices |
| MOV CRx/DRx/TRx, CLTS | Dispatch/privilege paths inspected; debug completion X86-29/30; paging/task integration still needs tests |
| CLC, STC, CMC, CLD, STD, CLI, STI, HLT, NOP | Basic and privilege paths inspected; interrupt timing is not certified |
| LOCK, segment overrides, 66h/67h, invalid encodings | X86-23/25/28; reserved/repeated-prefix policy CHECK-07 |
| WAIT/FWAIT and ESC D8..DF | CPU gate and FPU boundary inspected; X86-32 and dedicated x87 audit CHECK-11 |

## Regression probes and run record

Added [tests/audit_cpu_helpers.py](tests/audit_cpu_helpers.py). It extracts the
current production function bodies into a temporary C program; it does not
maintain independently copied opcode implementations. Memory/exception stubs
make addresses and faults observable, so results are component tests, not
complete emulator execution. Source-anchor changes deliberately fail extraction
instead of silently running stale code.

Run from repository root, using the already installed WSL Python/GCC:

```powershell
wsl --exec python3 /mnt/c/Work/r36sx_disasm/homebrew/pico_286/tests/audit_cpu_helpers.py --output-dir /mnt/c/Work/r36sx_disasm/patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/diagnostics/x86-audit
```

Compiler options: `-std=c11 -O2 -fsanitize=undefined -fno-sanitize-recover=undefined`.
On 2026-09-05, compilation succeeded and the suite exited 1, as expected for
unfixed regressions:

| Probe | Result |
| --- | --- |
| `conditions` | PASS, 512 condition/flag combinations |
| `adc_sbb8` | PASS, 262144 operand/carry/operation combinations |
| `idiv16_overflow` | FAIL, UBSan signed division overflow |
| `idiv32_overflow` | FAIL, UBSan signed division overflow |
| `bit_negative16` | FAIL, address 00002FFE, expected 00000FFE |
| `bit_negative32` | FAIL, address 20000FFC, expected 00000FFC |
| `xchg_address_alias` | FAIL, write address 00002000, expected 00001000 |
| `rep16_index_wrap` | FAIL, second source 00010000, expected 00000000 |

These six failing probes cover five numbered findings (X86-21 has two probes).
The temporary C and native executable are deleted by the script. No downloaded
executables were used; no distributable binary/hash was produced. No antivirus
scan of the ephemeral probe was performed; execution used only locally generated
source and the installed compiler. No disk images or patch EXEs were changed.

## Fix order and completion criteria

First fix X86-01/02 and X86-03 with their small probes. Then address the shared
instruction-abort, stack, REP and segment-identity contracts before changing
individual fault handlers. Follow with transfer widths, gates/tasks, remaining
instruction cases and debugger/FPU integration. Each successful fix should have
a focused commit and a regression test; do not close a source finding solely
because an application still boots.

## Primary documentation

The main reference is Intel's **80386 Programmer's Reference Manual (1986)**,
chapter 17 plus the architecture chapters linked below, hosted as a transcription
by MIT/other mirrors. These are Intel-authored documents, not emulator folklore.
The transcription contains apparent inconsistencies; consult an original scan
and generation-specific manuals/errata before resolving disputed behavior.
No AMD-specific compatibility claim is made by this pass.

- [Intel 80386 instruction index](https://pdos.csail.mit.edu/6.828/2005/readings/i386/c17.htm)
- [Original Intel manual scan](https://www.bitsavers.org/components/intel/80386/230985-001_80386_Programmers_Reference_Manual_1986.pdf)
- [Intel LAR, alternate transcription](https://www.ardent-tool.com/CPU/docs/Intel/386/manuals/prref386/LAR.htm)

[idiv]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/IDIV.htm
[xchg]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/XCHG.htm
[faults]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm
[mov]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOV.htm
[push]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/PUSH.htm
[pop]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/POP.htm
[enter]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/ENTER.htm
[movs]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOVS.htm
[rep]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm
[segments]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s05_01.htm
[protection]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s06_03.htm
[real-differences]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s14_07.htm
[v86]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s15_01.htm
[jmp]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/JMP.htm
[jcc]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/Jcc.htm
[call]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/CALL.htm
[paging]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s06_04.htm
[tasks]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s07_05.htm
[iret]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/IRET.htm
[int]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/INT.htm
[idt]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_05.htm
[bt]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/BT.htm
[ins]: https://www.ardent-tool.com/CPU/docs/Intel/386/manuals/prref386/INS.htm
[outs]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/OUTS.htm
[lea]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/LEA.htm
[sgdt]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/SGDT.htm
[lgdt]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/LGDT.htm
[lock]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/LOCK.htm
[xlat]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/XLAT.htm
[debug]: https://pdos.csail.mit.edu/6.828/2005/readings/i386/s12_03.htm
