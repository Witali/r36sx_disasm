# Pico-286 Test Payloads

This directory stores CPU and platform test sources that are useful for the
R36SX Pico-286 port.

## Instruction fault regression ROM

`cpu386_faults.asm` is a standalone 64 KiB BIOS ROM testing real production
opcode decode, segmentation, paging and exception delivery, without DOS or
disk images. Build a Windows debug EXE first, then run:

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_faults.ps1
# Also exercise the GCC/computed-goto core:
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_faults.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-faults-mingw
```

The runner assembles with the local NASM, temporarily changes only the build
config, scans the ROM, checks POST `80:FF`, the exact `CPU386 FAULTS PASS cases=42` message,
the debug mailbox and framebuffer, then restores the config. POST `80:FE`
means failure. ROM/listing artifacts are in `build/`; diagnostics are in the
patch `diagnostics/compiler-<Tag>/` directory. Do not run multiple instances
of these runners concurrently: they share the build config.
The common `smoke_windows_build.ps1` also accepts `-CpuModel 80286` and
`-AllowBlankFrame` for port-only ROMs such as `test286`; the latter still
checks framebuffer dimensions and the requested POST/text completion markers.

Coverage (42 cases):

- 24 memory MOV loads: byte/word/dword operands, 16/32-bit addresses, CS.D=0/1,
  with null DS (#GP) and a non-present data page (#PF).
- Six immediate MOV fetches crossing a page boundary, covering byte/word/
  dword immediates in both code sizes, including operand-size prefixes.
- One opcode fetch from a non-present page.
- One valid INC at the last byte of a mapped page. It must complete before
  the next opcode fetch faults; diagnostic lookahead must not cause #PF.
- One fault while TF is set: no stale single-step trap over the fault handler.
- One #GP followed by a not-present #GP gate, producing #DF(0), followed by
  another ordinary #GP to check delivery-state cleanup.
- Five #UD diagnostic boundaries: an instruction in a two-byte code segment,
  and instructions next to absent preceding/following pages with CS.D=0/1.
  The opcode bytes themselves are valid memory; only the diagnostic peek
  would exceed the segment/page boundary. Assert #UD, exact saved CS:EIP,
  no hardware error code, preserved EAX/flags and unchanged CR2.
- Two software INT 13 controls (CS.D=0/1): unlike a processor #GP, the same
  vector saves the following EIP without an error code or forcing RF.

Checks include preserved EAX/defined arithmetic flags, handler entry, saved
CS:EIP, error code, RF=1 for #UD/#GP/#PF, and CR2. Data-load CR2 is checked exactly; instruction-fetch
CR2 is checked at page precision. Intel 9.8.14 specifies the faulting access
address but not an instruction-fetch granule/order within a split immediate;
this suite does not establish bus-level fetch precision. Saved fault EIP is
still exact. #DF restart EIP is deliberately not asserted.
This covers instruction abortion, not rollback of earlier writes in compound
instructions, REP progress, task-switch restart state, or every MOV encoding.
Those remain separate items in `TODO_X86_SOURCE_AUDIT.md`.
RF lifetime, execution-breakpoint #DB faults and task-gate saved state remain
separate checks; this ROM only verifies the non-debug fault gate images.

Specifications: [Intel 80386 PRM 9.1](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm),
[9.8](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm),
[MOV](https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOV.htm), and
[AMD APM vol. 3 rev. 3.19, MOV pp. 213-215](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf).
Only legacy behavior is used; original 80386 rules take precedence over later
CPU extensions.
For RF, use [Intel 80386 PRM 12.3.1.1](https://www.scs.stanford.edu/05au-cs240c/lab/i386/s12_03.htm)
and [AMD APM vol.2 rev.3.25 section 8.2.2](https://kib.kiev.ua/x86docs/AMD/AMD64/24593_APM_v2-r3.25.pdf).
The modern AMD #DB RF wording differs from the original 386 rule; do not use
it to redefine the target's fault/trap distinction.

## PUSH/PUSHF regression ROM

`cpu386_push.asm` runs 864 checked cases through the production interpreter:
27 instruction forms x two operand widths x two code widths x two stack
widths x four stack conditions. The 27 forms are all eight general registers
in both `50+r` and `FF /6` encodings, signed imm8, full-width immediate,
16/32-bit memory addresses, six segment registers and PUSHF/PUSHFD.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_push.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_push.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-push-mingw
```

The four stack conditions are a successful writable stack, expand-down #SS,
non-present #PF and read-only user-page #PF. Snippets run at CPL3; the handlers
use a supervisor-only CPL0 stack from a 32-bit TSS. Checks include saved
CS:EIP/SS:ESP, error code, exact data CR2, all general/data-segment registers,
arithmetic flags, the pushed value and adjacent memory canaries. Rejected
writes must not change the stack contents or SP/ESP. The successful PUSH SP/
ESP cases require the original pointer value, unlike the 8086 behavior.
Only the low selector word is asserted for 32-bit PUSH Sreg; the slot width
and pointer movement are checked independently.

POST `80:FF` plus `CPU386 PUSH PASS cases=864` means success; failure prints
the zero-based case and check IDs before POST `80:FE`. The case is
`context * 108 + row`, with table order recorded in `build/cpu386_push.lst`.
Contexts 0/1 are successful SS.B=0/1, 2/3 expand-down, 4/5 absent page, 6/7
read-only page. Checks 1..7 are vector/error, GPRs, selectors/flags, saved
EIP/ESP, CR2, memory, and final case count respectively. The runner uses the
same no-disk/restored-config workflow as the fault ROM above.

Coverage is not yet complete PUSH conformance: split-page stores, source
faults/aliasing, high ESP bits with SS.B=0, pointer wrap, real/v86 mode and
every immediate value are not established by these cases. Compound stack
operations and the specialized 286 PUSH helper remain separate audit items.
Specifications: [Intel 80386 PUSH](https://pdos.csail.mit.edu/6.828/2005/readings/i386/PUSH.htm),
[PUSHF](https://pdos.csail.mit.edu/6.828/2005/readings/i386/PUSHF.htm),
[fault/restart rules](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm),
and [AMD APM vol. 3 rev. 3.19, PUSH/PUSHF pp. 258-262](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf).
The primary target is original 80386; later AMD64/VME behavior is excluded.

## POP register/memory regression ROM

`cpu386_pop.asm` checks 1640 cases of POP r16/r32 and r/m16/r/m32 through the
production decoder (800 valid and 840 invalid encodings). It uses the same
ring-3 to supervisor-stack fixture as the PUSH ROM, with an independent byte
oracle for source/destination memory.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_pop.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_pop.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-pop-mingw
```

The 20 forms are eight GPRs in both `58+r` and `8F /0` encodings, `[BX]`,
`[ESI]`, `[ESP]` and `[ESP+disp32]`. The matrix varies operand size, CS.D,
SS.B and five memory conditions: success, source segment-limit #SS, absent
source #PF, read-only destination #PF and absent destination #PF. Register
rows remain successful controls in destination-fault contexts. For `[ESP]`
the absent destination shares the source page, so that case correctly expects
a read fault; its read-only case still isolates a destination write fault.

Each code/operand-width combination also includes all seven undefined
`8F /1..7` group selectors in register, addr16 `[BX]` and addr32 `[ESI]`
forms. These 21 additional rows must raise #UD without a hardware error code
or any register/stack/destination update, even in contexts with an unavailable
stack or unwritable destination. This checks that encoding validation happens
before operand accesses, not just that a later exception occurs. Original
386 encoding rules apply; later AMD XOP escapes are not supported here.

Checks include all GPRs/data selectors, arithmetic flags, saved CS:EIP/SS:ESP,
error code, exact CR2, and every byte in 16-byte stack and 80-byte destination
windows. POP SP/ESP must end with the popped value. ESP-based memory operands
must use the incremented address, yet a failed write must preserve the old
architectural SP/ESP. The current 386 helpers stage the source, resolve that
destination, restore the old pointer before faultable checks/writes, and
commit the pointer only on success. Lower-model interpreters are unchanged.

Pass requires POST `80:FF` and `CPU386 POP PASS cases=1640`. Failure reports
zero-based `case`, `check`, saved `esp` and `expected` values. The case is
`context * 164 + row`; contexts 0/1 are success SS.B=0/1, 2/3 source #SS,
4/5 absent source, 6/7 read-only destination, and 8/9 absent destination.
Check IDs match the PUSH fixture: vector/error, GPRs, selectors/flags, saved
EIP/ESP, CR2, memory, final count. See `build/cpu386_pop.lst` for row order.

This does not yet cover segment POP, POPF,
split-page memory accesses, all addressing combinations, high ESP bits on
16-bit stacks, pointer wrap, or real/v86 mode. Original Intel 80386 PRM
[POP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/POP.htm) and
[9.1](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm) define
the fault/restart contract; [9.8.6](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm)
specifies invalid-opcode faults. Cross-checks use
[AMD APM vol. 3 rev. 3.19, POP pp. 246-247](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf)
and [Intel SDM vol. 2, POP](https://cdrdv2-public.intel.com/835757/325383-sdm-vol-2abcd.pdf)
for non-wrapping post-increment ESP addressing. The latter explicitly leaves
16-bit-stack wrap behavior processor-family-specific; no such case is asserted.

## LEA addressing and encoding regression ROM

`cpu386_lea.asm` executes 26,528 cases through the production 386 decoder:
829 addressing/encoding rows x four CS.D/operand-size combinations x eight
destination registers. It is a standalone 64 KiB ROM with paging and CPL3
execution, using a TSS to deliver completion/#UD onto a supervisor stack.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_lea.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_lea.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-lea-mingw
```

The table contains 24 legal addr16 forms (all r/m values for mod=00/01/10),
789 legal addr32 forms (21 non-SIB plus all 256 SIB bytes for each memory
mod), and 16 invalid register-source rows (all eight r/m values at both
address sizes). Thus 26,016 cases are valid and 512 require #UD. Nonzero
displacements include negative disp8 and full-width values; initial register
values exercise high halves, signed-looking values, aliasing with destinations
and address overflow. Expected offsets are generated from Intel's addressing
tables in NASM, not by importing emulator helper code.

The fixture copies a single instruction into a RAM code buffer, sets its
prefixes and destination field, then transfers to it with IRETD. All GPRs,
arithmetic flags/DF/IF, data selectors, saved CS:EIP/SS:ESP and unchanged CR2
are checked. LEA r16 must preserve the upper half; addr16 into r32 must zero
extend. Effective offsets must not include segment bases. Data segments
alternate between null and nonzero-base/short-limit descriptors; SS has an
unmapped nonzero base. No source-memory access or segment-limit fault is
permitted. Six segment overrides and no override rotate across cases; they
are not a separate exhaustive Cartesian dimension. Flag patterns similarly
alternate between clear and set arithmetic flags/DF.

Pass requires POST `80:FF` and `CPU386 LEA PASS cases=26528`. Failure prints
`case`, `check` and `value`. Case ID = `row * 32 + widths * 8 + destination`;
widths bit 1 selects CS.D, bit 0 selects operand32. Checks are vector/frame
layout, all GPRs, selectors/flags, saved IP, unchanged CR2 and final count.
See `build/cpu386_lea.lst` for table order. The runner restores the build
configuration and uses no disks. Real/v86 mode, instruction-fetch faults,
LOCK/repeated-prefix policy and exhaustive displacement/input values remain
outside this matrix; it is not complete 386 conformance.

Specifications: [Intel 80386 LEA](https://pdos.csail.mit.edu/6.828/2005/readings/i386/LEA.htm),
[Intel 80386 PRM 17.2, Tables 17-2/3/4](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s17_02.htm),
and [AMD APM vol.3 rev.3.19, LEA pp.195-196](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf).
Only original 386/legacy rules are used, not 64-bit addressing extensions.

## Far CALL/JMP encoding and pointer regression ROM

`cpu386_far.asm` runs 1,152 cases at CPL3 with paging. A table of 192 rows
varies CS.D, operand size, address size, CALL/JMP and twelve operand forms:
all eight invalid register sources, absolute memory through DS/FS/SS, and
an immediate far pointer. Each row runs with SS.B=0/1 in three environments:
accessible data, null data selectors, and a missing pointer page. Valid
transfers enter a different code segment with the opposite CS.D; operands
and return stack slots retain the calling instruction's operand size.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_far.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_far.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-far-mingw
```

Expected outcomes: 768 #UD, 64 #GP(0), 96 #PF(user read, non-present), and
224 successful transfers. The test checks all GPRs, arithmetic flags/IF/DF,
selectors, saved CS:EIP/SS:ESP, error-code layout and CR2. The user stack is
checked byte-by-byte for exactly the CALL return frame or unchanged canaries;
the high half of a dword CS slot is not asserted. Pointer memory must remain
unchanged even when an access fails. Fall-through cannot count as success.
No user disks are attached, and the build config is restored after execution.

The pre-fix binary fails case 48 (`66 FF D8`) with #GP rather than #UD.
Pass requires POST `80:FF` plus `CPU386 FAR PASS cases=1152`. A failure prints
case/check/value. Case = context * 192 + row; context = data scenario * 2 +
SS.B. Row ordering is CS.D, operand width, address width, CALL/JMP, operand
form. Check IDs are vector/error frame, GPRs/ESP, selectors/flags, EIP,
CR2, stack bytes, pointer bytes and total case count.

This is not full far-transfer conformance: real/v86, call/task gates, target
descriptor faults, split pointer reads, call-stack faults and targets above
FFFFh require additional matrices. Those remain separate audit work.

Specifications: [Intel 80386 CALL](https://pdos.csail.mit.edu/6.828/2005/readings/i386/CALL.htm),
[JMP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/JMP.htm),
[PRM 9.8.6, invalid operand type and #UD](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm),
and [AMD APM vol.3 rev.3.19, CALL (Far) pp.124-130 / JMP (Far) pp.187-191](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf).
AMD64-only exceptions and instructions are not imported into 386 behavior.

## MOVS/STOS address-size and index-wrap regression ROM

`cpu386_strings.asm` executes 576 cases through the production 386 decoder:
2 code defaults * 2 address sizes * 3 element widths * 2 DF directions *
6 repeat scenarios * 4 source/destination patterns. The tested opcodes are
MOVSB/MOVSW/MOVSD and STOSB/STOSW/STOSD, with REP counts 0, 1, 3, 17 and
2049, plus a non-REP instruction that must leave the count unchanged.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_strings.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_strings.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-strings-mingw
```

Each case runs at CPL3 with paging disabled and distinct source/destination
segments. Addr16 cases cross the 64 KiB index boundary in both directions;
addr32 controls cross the same boundary without truncation. Words/dwords do
not straddle the segment limit. Three MOVS patterns wrap the source, the
destination, or both; STOS wraps the destination and preserves ESI. The
longest REP exceeds the current 1024-element batch cap.

An independent scalar-byte reference verifies every destination byte and two
guard elements at each end, with source data differing across the 64 KiB
boundary. The ROM also checks all GPRs, preserved upper halves of SI/DI/CX,
arithmetic flags/IF/DF, CS:EIP, SS:ESP, data selectors and unchanged CR2.
The pre-fix MSVC binary fails case 0's memory comparison after MOVSB reads
10000h instead of 0000h. Success requires both POST `80:FF` and the message
`CPU386 STRINGS PASS cases=576`. Case order is the product order above; see
`build/cpu386_strings.lst`. Check IDs: 0 unexpected exception, 1 GPRs,
2 flags/frame/selectors/CR2, 3 memory, 4 total case count.

The runner restores its temporary build config and never attaches user disks.
This matrix covers index wrapping, not all string semantics: segment/page
fault restart, overlap, debug traps, segment overrides, real/v86 limits,
and the remaining LODS/INS/OUTS families need separate tests. Normal
CMPS/SCAS completion is covered by the comparison ROM below; its fault
restart behavior is not yet covered.

Specifications: Intel 80386 PRM chapter 17
[MOVS](https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOVS.htm),
[STOS](https://pdos.csail.mit.edu/6.828/2005/readings/i386/STOS.htm),
[REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm);
[AMD APM vol.3 rev.3.19](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf),
table 1-4 (address-size register selection), section 1.2.6 (REP),
MOVS pp.228-229 and STOS pp.301-302. AMD64-only rules are not used.

## REP exception progress and restart regression ROM

`cpu386_rep_faults.asm` runs 1080 cases at CPL3 with paging enabled. A table
of 216 rows varies CS.D, address size, element width, DF, fault location
(MOVS source, MOVS destination, STOS destination) and fault type. Each row
runs five scenarios: REP faults after 2, 0 and 1025 successful elements,
count-zero REP with an inaccessible operand, and a non-REP fault/retry.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_rep_faults.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_rep_faults.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-rep-faults-mingw
```

Fault types are #GP(0) for segment limits, #PF for a non-present page, and
#PF for page access protection (supervisor-only source or read-only
destination). DF=0 uses a normal segment's upper limit; DF=1 uses the lower
limit of an expand-down segment. Page-aligned boundaries keep each individual
word/dword within one page. Source/destination linear ranges map to different
physical RAM, exposing a raw-copy path that mistakes linear for physical.

The handler checks vector, error code, CR2, saved CS:EIP/SS:ESP, all GPRs,
data selectors and preserved flags. Scalar byte checks verify completed
writes, untouched future elements/guards, and an alias to the physical RAM
that an erroneous untranslated copy would overwrite. It then repairs the
descriptor/PTE, reloads the data selectors/CR3 and uses IRETD without resetting
the user's indexes or count. The second check verifies completion and exactly
one fault, or no fault for count-zero. There are 864 fault/retry cases (288
per fault type) and 216 zero-count cases.

Baseline `860e32a` fails case 0: ECX is 55AA0005h instead of 55AA0003h after
two elements should have completed. Pass requires both POST `80:FF` and
`CPU386 REP FAULTS PASS cases=1080`. Failure reports case/check/got/want;
case = row * 5 + scenario. Check IDs: 1 vector/error, 2 GPRs, 3 frame/flags/
selectors/CR2, 4 memory, 5 repair fault count, 6 completion fault/total count.
No disks are attached, and the build config is restored.

The 386 helpers now commit progress after each completed element. Their raw
RAM bulk path is reserved for real mode until a non-faulting page-aware probe
is available. The dedicated 8086/286 helpers retain their existing contract.
This is not complete REP coverage: split-element faults, SS overrides,
real/v86 checks, debug/IRQ interruptions and the remaining string families
still require tests. Separate matrices below cover RAM element ordering and
CMPS/SCAS fault-time flags.

References: [Intel 80386 REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm),
[PRM 9.8.13/14, #GP/#PF](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_08.htm),
[Intel SDM 325383-060US vol.2B p.4-551, REP restart](https://kib.kiev.ua/x86docs/Intel/SDMs/325383-060.pdf),
and [AMD APM vol.3 rev.3.19](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf),
section 1.2.6, MOVS/STOS exception tables and legacy IRETD. These are
vendor-authored manuals hosted on mirrors; AMD64-only behavior is not used.

## MOVS overlapping RAM regression ROM

`cpu386_movs_overlap.asm` executes 10368 cases: 3 modes (real, protected CS.D=0,
protected CS.D=1) * 2 address sizes * 3 element widths * 2 DF directions *
2 destination segment aliases * 2 source alignments * 12 displacements *
6 repeat scenarios. It exercises MOVSB/MOVSW/MOVSD with REP counts 0, 1, 2,
17 and 1025, plus single MOVS with an unchanged count of 7. Displacements
are -33, -4, -3, -2, -1, 0, +1, +2, +3, +4, +33 and +1200h. The latter
is a disjoint-copy control even for the longest dword sequence; the segment
alias shifts ES.base by 16 bytes while preserving the physical destination.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_movs_overlap.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_movs_overlap.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag cpu386-movs-overlap-mingw
```

The independent reference reads one whole element into EAX, then stores its
bytes using ordinary MOV instructions. Later iterations read the modified
reference memory: neither a byte-at-a-time copy nor whole-range memmove is
an acceptable substitute. Checks cover the complete source/destination union
and four-byte guards, all GPRs (including addr16 upper halves), arithmetic
flags/IF/DF and DS/ES/FS/GS. The ROM masks IRQs, uses CPL0 and no paging;
v86, fault restart, source segment overrides and device memory are outside
this matrix. No disks are attached and the runner restores its build config.

Baseline fails case 613 (`265h`): real-mode addr16 REP MOVSW, DF=0, count=1,
aligned source, destination=source+1. Byte 3002h becomes 6Ah instead of 6Bh
because the optimized helper writes byte 0 before reading source byte 1.
Pass requires POST `80:FF` and `CPU386 MOVS OVERLAP PASS cases=10368`.
Failure IDs: 1 GPRs, 2 flags/selectors, 3 memory, 4 final case count.
Case order is the product order above; NASM also writes
`build/cpu386_movs_overlap.lst` for decoding a failing row.

References: Intel 80386 PRM chapter 17
[MOVS](https://pdos.csail.mit.edu/6.828/2005/readings/i386/MOVS.htm) (byte,
word or dword assignment before index changes) and
[REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm) (individual
string operation per iteration); [AMD APM vol.3 rev.3.19](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf),
MOVS pp.228-229 and section 1.2.6. These are vendor-authored manuals on
mirrors; only 386-applicable legacy rules are used.

## CMPS/SCAS normal-completion regression ROM

`cpu386_compare_strings.asm` checks 10752 CPL3 protected-mode cases through
the actual decoder. The matrix combines CS.D=0/1, address16/32, byte/word/dword
CMPS and SCAS, both DF values, and no/FS/GS/SS override. Each combination has
56 scenarios: two opposite initial arithmetic-flag patterns (including both
ZF and IF values), twelve single-comparison operand pairs, and eight repeat
profiles for each of REPE and REPNE. Counts are 0, 1, 5 and 1025, with stops
at the first, middle or last comparison, or by exhausting the count.

The NASM `PAIR` macro computes CF/PF/AF/ZF/SF/OF from scalar bit formulas,
independently of guest CMP/SUB. Operand pairs include zero/equality, unsigned
borrow, signed overflow, auxiliary carry and low-byte parity boundaries.
REP cases use equal/unequal patterns and verify the flags of the final
comparison, not the entry ZF. A zero count preserves flags and performs no
operand read even with null data selectors. SCAS uses null DS/FS/GS selectors
to detect an erroneous source access and must ignore source overrides.

Indexes cross the 64 KiB boundary without splitting individual elements.
The checker verifies SI/DI wrapping and high-half preservation for addr16,
full ESI/EDI progress for addr32, CX/ECX consumption, all other GPRs, IF/DF,
CS:EIP, SS:ESP and data selectors. Source regions, the ES region and two guard
elements at each end must remain unchanged. SCAS must leave ESI unchanged.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_compare_strings.ps1 -VerifyOracle
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_compare_strings.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag compare-strings-mingw -VerifyOracle
```

The runner assembles a 64 KiB F0000h ROM and requires POST `80:FF` plus
`CPU386 COMPARE STRINGS PASS cases=10752`. Checks 1/2/3/4 identify GPRs,
flags/frame/selectors, memory and final count. `-VerifyOracle` also builds a
negative-control ROM with deliberately inverted expected CF; it must fail
at case 0, check 2, observed `44h` versus expected `45h`. A crash, timeout,
wrong failure or unexpected success does not pass that control.

No disk images are attached; the runner restores the build config. MSVC and
GCC pass both the normal and negative-control runs after the separate IRET
flags fix. This is not complete CMPS/SCAS or REP coverage: real/v86 modes,
split elements and all segment-prefix encodings remain separate work. The
following fault matrix covers a subset of restart and debug interruption.

References: Intel 80386 PRM chapter 17
[CMPS](https://pdos.csail.mit.edu/6.828/2005/readings/i386/CMPS.htm),
[SCAS](https://pdos.csail.mit.edu/6.828/2005/readings/i386/SCAS.htm),
[REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm), and
[AMD APM vol.3 rev.3.19](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf)
CMPS pp.144-145, SCAS pp.285-286, table 1-4 and section 1.2.6. These are
vendor-authored manuals on mirrors. The Intel REP HTML pseudocode has
transposed ZF stop conditions; its prose and AMD agree that REPE stops on
ZF=0 and REPNE on ZF=1, after executing a comparison when count is nonzero.

## CMPS/SCAS fault flags and debug interruption ROM

`cpu386_compare_faults.asm` is a 64 KiB reset ROM with 2592 CPL3 cases:
CS.D=0/1, byte/word/dword operands, address16/32, both DF directions,
CMPS source/destination faults or SCAS destination faults, REPE/REPNE,
and segment-limit, absent-PTE or supervisor-PTE failures. Source/destination
pages use nonidentity mappings, with independent physical aliases for checks.
Descending segment faults use expand-down descriptors.

Each row runs six scenarios: fault after 2, 0 or 1025 completed elements;
zero-count REP with an inaccessible operand; non-REP fault/retry; and #DB
after one comparison followed by an operand fault after the second.
The handler checks independent expected GPRs, CS/EIP/SS/ESP, arithmetic
flags/IF/DF/TF, vector/error code, CR2, unchanged buffers and guard elements.
The first fault leaves the operand inaccessible and changes stacked EFLAGS;
the immediate re-fault must retain this new entry image. Only then does the
handler repair the descriptor/PTE and resume to full completion via IRETD.
The TF handler separately verifies comparison flags and DR6.BS, clears TF,
and supplies another entry image for the resumed REP.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_compare_faults.ps1 -Tag compare-faults-msvc
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_compare_faults.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag compare-faults-mingw
```

The unmodified GCC EXE at `02a5c13` fails case 0/check 3: saved flags are
`244h` (last equal comparison) instead of `AD5h` (masked REP entry image).
Pass requires POST `80:FF` AND `CPU386 COMPARE FAULTS PASS cases=2592`.
Failure reports case/check/got/want; case = row * 6 + scenario. Check IDs
1..6 have the same meanings as the MOVS/STOS fault ROM above. No disks are
attached; the runner restores the build config.

The 386 interpreter saves entry flags across per-element redecodes and host
quanta, restores them before operand-fault delivery, and discards the context
on instruction completion, reset, interrupt or exception entry. #DB traps
must retain the latest comparison result. The 8086/286 hot loops do not
capture this context. The runner requires the live binary's logged
`micro_exec` budget to be below 1025 (currently 100), so the long REP must
span CPU calls and retain its entry image across those boundaries.
Remaining work includes real/v86, SS-override faults, split operands and
IRQ/NMI. The per-iteration and final #DB boundaries are tested below.

References: [Intel 80386 PRM REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm),
[PRM 9.1 exception classes](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm),
[PRM 12.3 debug exceptions](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s12_03.htm),
and [Intel SDM 325383-060US vol.2B p.4-551](https://kib.kiev.ua/x86docs/Intel/SDMs/325383-060.pdf).
The explicit REPE/REPNE CMPS/SCAS fault-time EFLAGS restoration rule comes
from the latter Intel manual, not an inferred arithmetic rule.
[AMD APM vol.3 rev.3.19](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf)
section 1.2.6 and CMPS/SCAS entries corroborate count, ZF termination and
operand/flag rules; that text does not explicitly specify the fault-time
flags rollback. These are vendor-authored manuals hosted on mirrors, not
physical Intel/AMD 386 validation results.

## String single-step and final-iteration regression ROM

`cpu386_string_traps.asm` checks 1128 CPL3 cases (4440 single-step traps):
MOVS/STOS/LODS/CMPS/SCAS byte/word/dword forms, CS.D=0/1, address16/32,
both DF directions, REP and comparison REPE/REPNE. Counts are 0/1/3/17,
with non-REP controls that preserve a count sentinel. CMPS/SCAS additionally
stop on ZF at the first, middle or last of three elements. Zero-count REP
uses null DS/ES/FS/GS selectors: any attempted operand access must fail.
Addr32 operands lie above offset 64 KiB; addr16 has nonzero high-half
sentinels that must be preserved but ignored for addressing/count selection.

An interrupt-gate #DB handler checks every completed element before IRETD:
GPRs (including untouched high halves), count, indexes, partial AL/AX loads,
defined 386 flags, DR6.BS, CS/EIP/SS/ESP, data selectors, source contents,
destination progress and guard elements. The expected state comes from the
case table and an independent trap counter, not observed guest registers.
The last trap clears TF and returns to INT 30h, which verifies completion
and the exact trap count. Intermediate traps must return to the first prefix;
the last trap must return past the instruction on either count or ZF exit.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_string_traps.ps1 -Tag string-traps-msvc -VerifyOracle
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_string_traps.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag string-traps-mingw -VerifyOracle
```

The baseline GCC build at `abe8eda` fails case 12/check 2/step 1: REP LODSB
with count 1 saves EIP=0 instead of 2. The byte/word and dword CMPS/SCAS/LODS
handlers now rewind only when a REP iteration remains. Both switch/MSVC and
computed-goto/GCC builds pass the matrix. `-VerifyOracle` demands a specific
failure at case 2/check 2 when a control ROM incorrectly expects EIP=0 after
the last MOVSB; crashes, timeouts and other failures do not count as success.
The runner assembles/scans a 64 KiB ROM, attaches no disks and restores the
build config. Check IDs: 1 trap count, 2 EIP, 3 CS, 4 saved flags, 5 live
handler TF/IF, 6 SS, 7 GPRs, 8 data selectors, 9 DR6, 10 memory, 11 case count.

References are vendor-authored manuals on mirrors:
[Intel 80386 PRM 12.3.1.4](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s12_03.htm),
[9.1](https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_01.htm),
[REP](https://pdos.csail.mit.edu/6.828/2005/readings/i386/REP.htm) and
[LODS](https://www.scs.stanford.edu/05au-cs240c/lab/i386/LODS.htm).
[Intel B1 stepping information, 1987-09-01](https://docs.pcjs.org/manuals/intel/80386/80386_B1-1987-09-01.pdf),
erratum 5, explicitly distinguishes intended per-element stepping from the
early chip's two-element REP MOVS behavior; this test targets architectural
behavior, not that erratum. [AMD APM vol.2 rev.3.25](https://kib.kiev.ua/x86docs/AMD/AMD64/24593_APM_v2-r3.25.pdf)
sections 3.1 (TF), 13.1.3.2 and 13.1.4 corroborate trap frames, partial REP
resumption and BS/TF handling. The old 13.1.4 sentence excluding the following
instruction contradicts its own section 3.1; Intel's generation-specific TF
rule is authoritative here. No modern RF, BTF or fast-string rules are imported.
This does not certify real/v86 stepping, SS-override/split-operand faults,
hardware data breakpoints, INS/OUTS, IRQ/NMI or a physical Intel/AMD chip.

## IRET privilege and flags regression ROM

`cpu386_iret_flags.asm` checks 1280 normal protected-mode IRET/IRETD returns:
all ten source/destination CPL pairs with destination CPL >= source CPL,
16/32-bit operands, old/new IOPL 0..3, and all old/new IF combinations.
The code and stack descriptors have D/B=1; the word form uses prefix 66h.
RETF enters the source level after CPL0 sets the initial flags, so setup
does not depend on the IRET behavior being tested.

The target captures flags and all eight GPRs before entering the checker.
Checks include CF/PF/AF/ZF/SF/OF/DF, IF/IOPL, CS, SS and ESP, covering both
same-level stack consumption and outer-level stack replacement. IF write
permission depends on the executing CPL and old IOPL, and IOPL is writable
only from CPL0. Independent loops vary both the old and requested values.
The pre-fix EXE fails case 129 (`81h`), CPL0 -> CPL1 IRET with old IOPL=0:
flags check 1 reports `4D5h` instead of `6D5h` because IF is lost.

```powershell
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_iret_flags.ps1
powershell -ExecutionPolicy Bypass -File homebrew/pico_286/tests/test_cpu386_iret_flags.ps1 -Exe homebrew/pico_286/build/pico_286_win_mingw.exe -Tag iret-flags-mingw
```

The runner assembles a 64 KiB F0000h ROM and requires POST `80:FF` plus
`CPU386 IRET FLAGS PASS cases=1280`. Failure prints case/check/got/want;
checks 1/2/3/4 identify flags, GPRs, selectors and final case count.
It attaches no disk images and restores the build config in `finally`.
This does not cover fault rollback, invalid return selectors, task return,
NT/TF/RF/VM, v86, paging, or 16-bit code/stack defaults. Those remain separate
conformance work rather than being certified by this flags matrix.

References: [Intel 80386 PRM IRET](https://pdos.csail.mit.edu/6.828/2005/readings/i386/IRET.htm),
[Intel SDM 325383-060US vol.2A p.3-479](https://kib.kiev.ua/x86docs/Intel/SDMs/325383-060.pdf)
RETURN-TO-OUTER-PRIVILEGE-LEVEL (IF/IOPL before CPL update), and
[AMD APM vol.3 rev.3.19 p.339](https://kib.kiev.ua/x86docs/AMD/AMD64/24594_APM_v3-r3.19.pdf)
IRETx (`old_CPL`/`old_RFLAGS.IOPL`). These are vendor-authored manuals hosted
on mirrors; later-generation flags and modes are not applied to the 386.

## test386.asm

`test386.asm` is vendored from:

https://github.com/barotto/test386.asm

The R36SX copy is configured in `test386.asm/src/configuration.asm` with:

- `POST_PORT equ 0x80`
- `OUT_PORT equ 0x191`
- `DEBUG equ 1`
- `VGA_DEBUG equ 1`

Pico-286 captures standard POST writes to `80h`, keeps legacy support for the
older R36SX `190h` test port, and logs output-port text from `191h` as
`test386:` lines in `pico_286.log`.

With `VGA_DEBUG` enabled, the ROM also writes short breadcrumbs directly to
VGA text memory at `B800:0000` during the early `POST 01` branch/loop tests:
`JCC8`, `JCC16`, `LOOP`, `LOOPZ`, and `LOOPNZ`.

Build the ROM payload with:

```powershell
.\homebrew\pico_286\tests\build_test386_r36sx.ps1
```

The script uses the local NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\tests\test386.asm\src\ -f bin .\homebrew\pico_286\tests\test386.asm\src\test386.asm -w-all -l .\homebrew\pico_286\tests\test386.asm\build\test386.lst -o .\homebrew\pico_286\tests\test386.asm\build\test386.bin
```

Rebuild `cpu_tests.img` with:

```powershell
.\homebrew\pico_286\tests\rebuild_cpu_tests_disk.ps1
```

The script writes the floppy image to
`homebrew/pico_286/images/cpu_tests.img`.

The generated `test386.bin` is a 64 KB BIOS replacement ROM. It is not a
DOS `.COM` program, so it cannot be launched from the DOS prompt. The test disk
stores it as `TEST386.BIN` for reference and for future emulator BIOS-loading
work.  `rebuild_cpu_tests_disk.ps1` also copies the same ROM to
`homebrew/pico_286/test386.bin`, which is the default `test_bios_rom` used by
the native executable.

## test286.asm

`test286.asm` is a small R36SX-specific NASM BIOS replacement ROM for 80286
smoke testing. Like `test386.asm`, it does not prove complete instruction
conformance; it focuses on compact POST-driven coverage for 286 behavior:

- real-mode `PUSH SP`, `PUSHA`/`POPA`, 5-bit shift-count masking, `IMUL`,
  `BOUND`, `SGDT`, `SIDT`, and `SMSW`;
- raw protected-mode entry through `LMSW` and a far jump;
- protected-mode `LSL`, `LAR`, `VERR`, `VERW`, and `ARPL` descriptor checks.

Build the ROM payload with:

```powershell
.\homebrew\pico_286\tests\build_test286_r36sx.ps1
```

The script uses the local NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -i.\homebrew\pico_286\tests\test286.asm\src\ -f bin .\homebrew\pico_286\tests\test286.asm\src\test286.asm -w-all -l .\homebrew\pico_286\tests\test286.asm\build\test286.lst -o .\homebrew\pico_286\tests\test286.asm\build\test286.bin
```

The generated `test286.bin` is also 64 KB and is copied to
`homebrew/pico_286/test286.bin`, so it can be selected with:

```ini
bios=test286
test_bios_rom=test286.bin
cpu_model=80286
```

## pcxtbios

`pcxtbios/pcxtbios_from_ghidra_bytes.asm` is a byte-preserving NASM rebuild
source for the embedded 8 KB PC/XT-style BIOS ROM used by Pico-286.  It was
generated from `BIOS/pcxtbios_ghidra_full.s` and emits the original ROM bytes
with `db` directives while keeping Ghidra/ndisasm context as comments.

This source is intentionally conservative: it rebuilds byte-identically to
`BIOS/pcxtbios.bin` before we start replacing understood ranges with symbolic
labels and real instructions.

Build it with WSL NASM or any NASM-compatible binary:

```sh
nasm -f bin homebrew/pico_286/tests/pcxtbios/pcxtbios_from_ghidra_bytes.asm \
  -o BIOS/nasm_attempt/pcxtbios_from_ghidra_bytes.bin
```

Expected SHA-256 for the rebuilt ROM:

```text
468396458c74542e6fdf675fa53e9552b3037a5d6119e3232f486e8543922b96
```

## MAPDRIVE.COM

`homebrew/pico_286/pico-286/tools/mapdrive.asm` is the standalone DOS utility
source for registering a host-backed network drive.  It defaults to `H:` and
also accepts a drive parameter such as `MAPDRIVE G:` or `MAPDRIVE G`.  The
R36SX native executable no longer embeds or launches a MAPDRIVE trampoline, so
the patch folder keeps a real `.COM` copy for DOS-side mapping and testing.

Build it with the same NASM 3.01 executable:

```powershell
.\tools\nasm-3.01-win64\nasm-3.01\nasm.exe -f bin .\homebrew\pico_286\pico-286\tools\mapdrive.asm -o .\patches\disk_image_patch_pico_286\MIPS_NATIVE\pico_286\tools\MAPDRIVE.COM
```

Generated DOS `.COM` files for the patch are kept under
`patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/`.

## SBPROBE.COM

`sound_blaster/sb_probe.asm` is a DOS `.COM` probe for the Sound Blaster DSP
emulation.  It checks reset/version/identity commands, speaker status, DMA
identification, and a single-cycle DMA playback IRQ acknowledge path.

Build it with:

```powershell
.\homebrew\pico_286\tests\sound_blaster\build_sb_probe.ps1
```

The script also mirrors the generated DOS test to
`patches/disk_image_patch_pico_286/MIPS_NATIVE/pico_286/tools/SBPROBE.COM`.
