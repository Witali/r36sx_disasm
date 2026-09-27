; Intel REP fault restart: preserve completed indexes/count, restore the
; pre-instruction EFLAGS image for REPE/REPNE CMPS/SCAS (SDM vol.2B 4-551).
; Adapted from cpu386_rep_faults.asm: all operands are reads, not writes.
; The first handler leaves the mapping invalid and changes stacked flags;
; a second fault must preserve this NEW entry image before repair and retry.
; A separate TF run traps after one comparison: traps retain comparison flags,
; and the later memory fault must restore the post-IRET entry image instead.
cpu 386
bits 16
org 0

%define ROM 0xf0000
%define PD 0x1000
%define PT 0x2000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define TSS 0x7000
%define KSTACK 0x9000
%define USTACK 0xa000
%define CODEBUF 0x10000
%define SRC 0x20000
%define DST 0x40000
%define RAW_DST 0x50000 ; Alias to physical DST, not the mapped guest destination.
%define PHYS_SRC 0x60000
%define PHYS_DST 0x70000
%define DATA_BYTES 0x8000
%define BOUNDARY 0x4000
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define SRCSEG (40|3)
%define DSTSEG (48|3)
%define STACKSEG (56|3)
%define TASK 64
%define VALUE 0x89abcdef
%define FLAGS_TEST 0xad7
%define ARITH_FLAGS 0x8d5
%define FLAGS_MASK 0xfd5
%define CR2_SENTINEL 0x13579bdf

%define COUNT STATE
%define ROW STATE+4
%define SCENARIO STATE+8
%define PHASE STATE+12
%define CHECK_ID STATE+16
%define GOT STATE+20
%define WANT STATE+24
%define VECTOR STATE+28
%define COMPLETED STATE+32
%define BEFORE_FAULT STATE+36
%define TOTAL STATE+40
%define START_SRC STATE+44
%define START_DST STATE+48
%define START_COUNT STATE+52
%define TEST_FLAGS STATE+56
%define NEXT_IP STATE+60
%define ITERATION STATE+64
%define FAULTS STATE+68
%define RETRY_FLAGS STATE+72
%define DEBUG_TRAPS STATE+76
%define EXPECTED_REGS STATE+128

%define R_CS 0
%define R_WIDTH 4
%define R_ADDRESS 8
%define R_KIND 12 ; 0=CMPS/source fault, 1=CMPS/dest fault, 2=SCAS/dest fault.
%define R_STEP 16
%define R_FAULT 20 ; 0=segment limit, 1=page absent, 2=page access protection.
%define R_TARGET 24
%define R_OFFSET 28
%define R_PTE 32
%define R_GOOD_PTE 36
%define R_BAD_PTE 40
%define R_ERROR 44
%define R_PREFIX 48
%define ROW_BYTES 52
%define SCENARIOS 6
%define TOTAL_CASES 2592

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1 >> 16) & 255, %3, ((%2 >> 16) & 15) | %4, (%1 >> 24) & 255
%endmacro

start:
    cli
    cld
    mov al, 0xff
    out 0x21, al
    out 0xa1, al
    xor ax, ax
    mov ss, ax
    mov sp, KSTACK
    mov es, ax
    mov ax, 0xf000
    mov ds, ax
    mov si, descriptors
    mov di, GDT
    mov cx, descriptors_end-descriptors
    rep movsb
    lgdt [gdt_ptr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp KCODE:setup

bits 32
setup:
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, KSTACK
    mov word [0xb8000], 0x0752
    mov dword [COUNT], 0
    mov dword [SCENARIO], 0
    mov dword [ROW], cases
    mov edi, TSS
    xor eax, eax
    mov ecx, 26
    rep stosd
    mov dword [TSS+4], KSTACK
    mov word [TSS+8], KDATA
    mov word [TSS+102], 104
    mov ax, TASK
    ltr ax
    mov edi, IDT
    mov ecx, 49
    mov eax, (unexpected-$$) | (KCODE<<16)
    mov edx, 0x8e00
.gate:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .gate
    mov word [IDT+1*8], debug_trap
    mov word [IDT+13*8], gp
    mov word [IDT+14*8], pf
    mov word [IDT+0x30*8], complete
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    mov edi, PD
    xor eax, eax
    mov ecx, 1024
    rep stosd
    mov dword [PD], PT|7
    mov edi, PT
    mov ecx, 1024
    mov eax, 3 ; Identity-map supervisor RAM and ROM.
.pte:
    stosd
    add eax, 0x1000
    loop .pte
    or byte [PT+(CODEBUF>>12)*4], 4
    or byte [PT+((USTACK-1)>>12)*4], 4
    ; Entries are four bytes apart, physical pages are 4096 bytes apart.
    xor ebx, ebx
.aliases:
    mov eax, ebx
    shl eax, 12
    add eax, PHYS_SRC|7
    mov [PT+(SRC>>12)*4+ebx*4], eax
    add eax, PHYS_DST-PHYS_SRC
    mov [PT+(DST>>12)*4+ebx*4], eax
    mov eax, ebx
    shl eax, 12
    add eax, DST|3
    mov [PT+(RAW_DST>>12)*4+ebx*4], eax
    inc ebx
    cmp ebx, DATA_BYTES/4096
    jb .aliases
.source_init:
    xor ebx, ebx
.source:
    mov eax, ebx
    shr eax, 8
    xor eax, ebx
    xor al, 0x5a
    mov [PHYS_SRC+ebx], al
    mov byte [DST+ebx], 0xcc
    inc ebx
    cmp ebx, DATA_BYTES
    jb .source
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    jmp short .paged
.paged:
    mov dx, 0x190
    mov al, 0xee
    out dx, al

case_begin:
    cld
    mov esp, KSTACK
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov dword [CHECK_ID], 0
    mov dword [PHASE], 0
    mov dword [FAULTS], 0
    mov dword [DEBUG_TRAPS], 0
    mov esi, [ROW]
    ; Stop after 2, 0, or 1025 elements; test zero count and non-REP too.
    ; Scenario 5 takes #DB after one iteration, then faults after the second.
    mov eax, 2
    cmp dword [SCENARIO], 0
    je .progress
    cmp dword [SCENARIO], 5
    je .progress
    xor eax, eax
    cmp dword [SCENARIO], 2
    jne .progress
    mov eax, 1025
.progress:
    mov [BEFORE_FAULT], eax
    lea edx, [eax+3]
    cmp dword [SCENARIO], 3
    jne .nonrep
    xor edx, edx
.nonrep:
    cmp dword [SCENARIO], 4
    jne .total
    mov edx, 1
.total:
    mov [TOTAL], edx
    mov [START_COUNT], edx
    cmp dword [SCENARIO], 4
    jne .indices
    mov dword [START_COUNT], 7
.indices:
    imul eax, [cs:esi+R_STEP]
    mov edx, [cs:esi+R_OFFSET]
    sub edx, eax
    mov dword [START_SRC], 0x6000
    mov dword [START_DST], 0x6000
    cmp dword [cs:esi+R_KIND], 0
    jne .dst
    mov [START_SRC], edx
    jmp .upper
.dst:
    mov [START_DST], edx
.upper:
    cmp dword [cs:esi+R_ADDRESS], 16
    jne .flags
    or dword [START_SRC], 0x12340000
    or dword [START_DST], 0xabcd0000
    or dword [START_COUNT], 0x55aa0000
.flags:
    mov dword [TEST_FLAGS], FLAGS_TEST
    cmp dword [cs:esi+R_STEP], 0
    jg .clear
    or dword [TEST_FLAGS], 0x400
.clear:
    cmp dword [SCENARIO], 5
    jne .retry_flags
    or dword [TEST_FLAGS], 0x100 ; TF applies to the first comparison only.
.retry_flags:
    mov eax, [TEST_FLAGS]
    xor eax, ARITH_FLAGS
    mov [RETRY_FLAGS], eax
    mov dword [ITERATION], -1
.clear_element:
    call destination_offset
    call source_offset
    call memory_values
    xor ebx, ebx
.clear_byte:
    mov [PHYS_SRC+ebp+ebx], al
    mov [PHYS_DST+edi+ebx], dl
    shr eax, 8
    shr edx, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .clear_byte
    inc dword [ITERATION]
    mov eax, [ITERATION]
    cmp eax, [TOTAL]
    jle .clear_element
    ; Install the bad mapping only after all setup stores are complete.
    mov word [GDT+(SRCSEG&~7)], DATA_BYTES-1
    mov word [GDT+(DSTSEG&~7)], DATA_BYTES-1
    mov byte [GDT+(SRCSEG&~7)+5], 0xf2
    mov byte [GDT+(DSTSEG&~7)+5], 0xf2
    cmp dword [cs:esi+R_FAULT], 0
    jne .page_fault
    mov edi, GDT+(DSTSEG&~7)
    cmp dword [cs:esi+R_KIND], 0
    jne .limit
    mov edi, GDT+(SRCSEG&~7)
.limit:
    mov word [edi], BOUNDARY-1
    cmp dword [cs:esi+R_STEP], 0
    jg .code
    mov byte [edi+5], 0xf6 ; Expand-down: offsets <= limit must fail.
    jmp .code
.page_fault:
    mov edi, [cs:esi+R_PTE]
    mov eax, [cs:esi+R_BAD_PTE]
    mov [edi], eax
.code:
    mov eax, PD
    mov cr3, eax ; 386 flush, without a later-generation INVLPG.
    mov edi, CODEBUF
    mov eax, 16
    cmp dword [cs:esi+R_WIDTH], 4
    jne .operand
    mov eax, 32
.operand:
    mov edx, 16
    cmp dword [cs:esi+R_CS], UCODE16
    je .codebits
    mov edx, 32
.codebits:
    cmp eax, edx
    je .address
    mov al, 0x66
    stosb
.address:
    cmp edx, [cs:esi+R_ADDRESS]
    je .repeat
    mov al, 0x67
    stosb
.repeat:
    cmp dword [SCENARIO], 4
    je .opcode
    mov eax, [cs:esi+R_PREFIX]
    stosb
.opcode:
    mov al, 0xa6
    cmp dword [cs:esi+R_KIND], 2
    jne .size
    mov al, 0xae
.size:
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov ax, 0x30cd
    stosw
    mov eax, edi
    sub eax, CODEBUF
    mov [NEXT_IP], eax
    mov byte [edi], 0xf4
    mov eax, CR2_SENTINEL
    mov cr2, eax
    push dword STACKSEG
    push dword USTACK
    push dword [TEST_FLAGS]
    push dword [cs:esi+R_CS]
    push dword 0
    mov ecx, [START_COUNT]
    mov esi, [START_SRC]
    mov edi, [START_DST]
    mov ax, SRCSEG
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, DSTSEG
    mov es, ax
    mov eax, VALUE
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    iretd

destination_offset:
    mov edi, [ss:ITERATION]
    imul edi, [cs:esi+R_STEP]
    add edi, [ss:START_DST]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .done
    and edi, 0xffff
.done:
    ret

source_offset:
    mov ebp, [ss:ITERATION]
    imul ebp, [cs:esi+R_STEP]
    add ebp, [ss:START_SRC]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .done
    and ebp, 0xffff
.done:
    ret

; Equal REPE operands and unequal REPNE operands force repetition up to the
; fault or count exhaustion. EF-FF gives CF/PF/SF=1 for every tested width.
memory_values:
    mov eax, 0xcccccccc
    mov edx, eax
    cmp dword [ss:ITERATION], 0
    jl .done
    mov ecx, [ss:TOTAL]
    cmp [ss:ITERATION], ecx
    jae .done
    mov eax, VALUE
    mov edx, eax
    cmp dword [cs:esi+R_PREFIX], 0xf3
    je .done
    xor edx, 0x10
.done:
    ret

gp:
    pushad
    mov dword [ss:VECTOR], 13
    jmp check_frame
pf:
    pushad
    mov dword [ss:VECTOR], 14
    jmp check_frame
debug_trap:
    push dword 0
    pushad
    mov dword [ss:VECTOR], 1
    jmp check_frame
complete:
    push dword 0 ; Normalize INT and fault stack layouts.
    pushad
    mov dword [ss:VECTOR], 0
check_frame:
    mov esi, [ss:ROW]
    mov dword [ss:CHECK_ID], 1
    mov eax, [ss:BEFORE_FAULT]
    mov ecx, 13
    cmp dword [cs:esi+R_FAULT], 0
    je .stage
    mov ecx, 14
.stage:
    cmp dword [ss:SCENARIO], 5
    jne .fault_stage
    cmp dword [ss:DEBUG_TRAPS], 0
    jne .fault_stage
    mov eax, 1
    mov ecx, 1
    jmp .vector
.fault_stage:
    cmp dword [ss:PHASE], 2
    jae .finished
    cmp dword [ss:SCENARIO], 3
    jne .vector
.finished:
    mov eax, [ss:TOTAL]
    xor ecx, ecx
.vector:
    mov [ss:COMPLETED], eax
    mov [ss:WANT], ecx
    mov edx, [ss:VECTOR]
    mov [ss:GOT], edx
    cmp edx, ecx
    jne unexpected
    mov edx, [cs:esi+R_ERROR]
    cmp ecx, 1
    ja .error
    xor edx, edx
.error:
    cmp [ss:esp+32], edx
    jne unexpected
    ; Build expected GPRs from the number of successful elements, never from
    ; observed guest indexes/count. A faulting element contributes nothing.
    mov dword [ss:EXPECTED_REGS], VALUE
    mov edx, [ss:START_COUNT]
    cmp dword [ss:SCENARIO], 4
    je .count
    sub edx, eax
.count:
    mov [ss:EXPECTED_REGS+4], edx
    mov dword [ss:EXPECTED_REGS+8], 0x33445566
    mov dword [ss:EXPECTED_REGS+12], 0x44556677
    mov dword [ss:EXPECTED_REGS+16], USTACK
    mov dword [ss:EXPECTED_REGS+20], 0x55667788
    imul eax, [cs:esi+R_STEP]
    mov edx, [ss:START_SRC]
    cmp dword [cs:esi+R_KIND], 2
    je .source_end
    add edx, eax
.source_end:
    mov [ss:EXPECTED_REGS+24], edx
    add eax, [ss:START_DST]
    mov [ss:EXPECTED_REGS+28], eax
    mov dword [ss:CHECK_ID], 2
    xor edi, edi
.reg:
    mov edx, 7
    sub edx, edi
    mov eax, [ss:esp+edx*4]
    cmp edi, 4
    jne .compare
    mov eax, [ss:esp+48]
.compare:
    mov [ss:GOT], eax
    mov edx, [ss:EXPECTED_REGS+edi*4]
    mov [ss:WANT], edx
    cmp eax, edx
    jne unexpected
    inc edi
    cmp edi, 8
    jb .reg
    mov dword [ss:CHECK_ID], 3
    xor eax, eax
    cmp dword [ss:VECTOR], 0
    jne .ip
    mov eax, [ss:NEXT_IP]
.ip:
    cmp eax, [ss:esp+36]
    jne unexpected
    mov eax, [ss:esp+40]
    and eax, 0xffff
    cmp eax, [cs:esi+R_CS]
    jne unexpected
    cmp word [ss:esp+52], STACKSEG
    jne unexpected
    mov edx, [ss:TEST_FLAGS]
    cmp dword [ss:PHASE], 0
    je .flags_image
    mov edx, [ss:RETRY_FLAGS]
.flags_image:
    cmp dword [ss:VECTOR], 1
    ja .flags_compare
    cmp dword [ss:TOTAL], 0
    je .flags_compare
    ; Normal completion AND #DB traps retain the LAST comparison's flags.
    ; IF/DF stay unchanged and the trap frame must still have TF set.
    and edx, ~ARITH_FLAGS
    or edx, 0x44
    cmp dword [cs:esi+R_PREFIX], 0xf3
    je .flags_compare
    and edx, ~ARITH_FLAGS
    or edx, 0x85
.flags_compare:
    mov eax, [ss:esp+44]
    and eax, FLAGS_MASK
    and edx, FLAGS_MASK
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    cmp eax, edx
    jne unexpected
    mov ax, ds
    cmp ax, SRCSEG
    jne unexpected
    mov ax, es
    cmp ax, DSTSEG
    jne unexpected
    mov ax, fs
    cmp ax, SRCSEG
    jne unexpected
    mov ax, gs
    cmp ax, SRCSEG
    jne unexpected
    mov eax, CR2_SENTINEL
    cmp dword [ss:VECTOR], 1
    je .cr2
    cmp dword [cs:esi+R_FAULT], 0
    je .cr2
    cmp dword [ss:SCENARIO], 3
    je .cr2
    mov eax, [cs:esi+R_TARGET]
    add eax, [cs:esi+R_OFFSET]
.cr2:
    mov edx, cr2
    cmp eax, edx
    jne unexpected
    mov dword [ss:CHECK_ID], 4
    mov dword [ss:ITERATION], -1
.memory:
    call destination_offset
    call source_offset
    call memory_values
    mov ecx, eax
    xor ebx, ebx
.byte:
    movzx eax, byte [ss:PHYS_SRC+ebp+ebx]
    mov [ss:GOT], eax
    movzx eax, cl
    mov [ss:WANT], eax
    cmp al, [ss:PHYS_SRC+ebp+ebx]
    jne unexpected
    movzx eax, byte [ss:PHYS_DST+edi+ebx]
    mov [ss:GOT], eax
    movzx eax, dl
    mov [ss:WANT], eax
    cmp al, [ss:PHYS_DST+edi+ebx]
    jne unexpected
    cmp byte [ss:RAW_DST+edi+ebx], 0xcc
    jne unexpected
    shr edx, 8
    shr ecx, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ss:ITERATION]
    mov eax, [ss:ITERATION]
    cmp eax, [ss:TOTAL]
    jle .memory
    cmp dword [ss:VECTOR], 0
    je case_done
    cmp dword [ss:VECTOR], 1
    je .debug_resume
    mov dword [ss:CHECK_ID], 5
    inc dword [ss:FAULTS]
    cmp dword [ss:FAULTS], 2
    ja unexpected
    je .repair
    ; No repair yet: the immediate re-fault tests that the saved image does
    ; not leak across exception delivery or overwrite the handler's flags.
    mov eax, [ss:RETRY_FLAGS]
    mov [ss:esp+44], eax
    mov dword [ss:PHASE], 1
    jmp .retry
.repair:
    ; Repair the descriptor and PTE without resetting user GPRs. A real
    ; restart must use the count/index values saved by the faulting REP.
    call repair
    mov dword [ss:PHASE], 2
    jmp .retry
.debug_resume:
    ; A trap has no rollback. Replace its comparison flags with a different
    ; image for the restarted REP; the next operand fault must expose it.
    inc dword [ss:DEBUG_TRAPS]
    mov eax, dr6
    test eax, 0x4000 ; DR6.BS identifies the single-step event.
    jz unexpected
    mov eax, [ss:esp+44]
    and eax, ~0x10100 ; Clear TF and RF for this retry.
    xor eax, ARITH_FLAGS
    mov [ss:TEST_FLAGS], eax
    mov [ss:esp+44], eax
    xor eax, ARITH_FLAGS
    mov [ss:RETRY_FLAGS], eax
.retry:
    mov ax, SRCSEG
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, DSTSEG
    mov es, ax
    popad
    add esp, 4
    iretd

repair:
    mov word [ss:GDT+(SRCSEG&~7)], DATA_BYTES-1
    mov word [ss:GDT+(DSTSEG&~7)], DATA_BYTES-1
    mov byte [ss:GDT+(SRCSEG&~7)+5], 0xf2
    mov byte [ss:GDT+(DSTSEG&~7)+5], 0xf2
    mov edi, [cs:esi+R_PTE]
    mov eax, [cs:esi+R_GOOD_PTE]
    mov [ss:edi], eax
    mov eax, PD
    mov cr3, eax
    ret

case_done:
    mov dword [ss:CHECK_ID], 6
    mov eax, 2
    cmp dword [ss:SCENARIO], 3
    jne .faults
    xor eax, eax
.faults:
    cmp eax, [ss:FAULTS]
    jne unexpected
    xor eax, eax
    cmp dword [ss:SCENARIO], 5
    sete al
    cmp eax, [ss:DEBUG_TRAPS]
    jne unexpected
    call repair ; Count-zero cases leave the deliberately bad mapping intact.
    inc dword [ss:COUNT]
    inc dword [ss:SCENARIO]
    cmp dword [ss:SCENARIO], SCENARIOS
    jb case_begin
    mov dword [ss:SCENARIO], 0
    add dword [ss:ROW], ROW_BYTES
    cmp dword [ss:ROW], cases_end
    jb case_begin
    cmp dword [ss:COUNT], TOTAL_CASES
    jne unexpected
    mov esi, passed
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt

unexpected:
    mov esi, failed
    call print
    mov eax, [ss:COUNT]
    call hex32
    mov esi, check_text
    call print
    mov eax, [ss:CHECK_ID]
    call hex32
    mov esi, got_text
    call print
    mov eax, [ss:GOT]
    call hex32
    mov esi, want_text
    call print
    mov eax, [ss:WANT]
    call hex32
    mov al, 10
    mov dx, 0x191
    out dx, al
    mov al, 0xfe
    out 0x80, al
halt:
    cli
    hlt
    jmp halt
print:
    mov al, [cs:esi]
    inc esi
    test al, al
    jz .done
    mov dx, 0x191
    out dx, al
    jmp print
.done:
    ret
hex32:
    mov ebx, eax
    mov ecx, 8
.digit:
    rol ebx, 4
    mov eax, ebx
    and eax, 15
    mov al, [cs:hex_digits+eax]
    mov dx, 0x191
    out dx, al
    loop .digit
    ret
hex_digits: db '0123456789ABCDEF'
passed: db 'CPU386 COMPARE FAULTS PASS cases=2592',10,0
failed: db 'CPU386 COMPARE FAULTS FAIL case=',0
check_text: db ' check=',0
got_text: db ' got=',0
want_text: db ' want=',0

align 4
cases:
%assign codebits 16
%rep 2
%assign code UCODE16
%if codebits=32
%assign code UCODE32
%endif
%assign address 16
%rep 2
%assign width 1
%rep 3
%assign direction 0
%rep 2
%assign step width
%assign fault_offset BOUNDARY
%if direction
%assign step -width
%assign fault_offset BOUNDARY-width
%endif
%assign kind 0
%rep 3
%assign target DST
%assign physical PHYS_DST
%assign protection 3 ; Supervisor-only operand, user read must fail.
%assign pf_error 4
%if kind=0
%assign target SRC
%assign physical PHYS_SRC
%assign protection 3 ; Supervisor-only source, user read must fail.
%assign pf_error 4
%endif
%assign fault 0
%rep 3
%assign good_pte (physical+(fault_offset&~4095))|7
%assign bad_pte good_pte&~1
%assign error pf_error
%if fault=0
%assign error 0
%elif fault=2
%assign bad_pte (good_pte&~7)|protection
%assign error pf_error|1
%endif
%assign prefix 0xf3
%rep 2
    dd code,width,address,kind,step,fault,target,fault_offset
    dd PT+((target+fault_offset)>>12)*4,good_pte,bad_pte,error,prefix
%assign prefix 0xf2
%endrep
%assign fault fault+1
%endrep
%assign kind kind+1
%endrep
%assign direction 1
%endrep
%assign width width*2
%endrep
%assign address 32
%endrep
%assign codebits 32
%endrep
cases_end:
%if (cases_end-cases)/ROW_BYTES*SCENARIOS != TOTAL_CASES
%error "Update compare fault case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC SRC, DATA_BYTES-1, 0xf2, 0x40
    DESC DST, DATA_BYTES-1, 0xf2, 0x40
    DESC 0, 0xffff, 0xf2, 0x40
    DESC TSS, 103, 0x89, 0
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
    dd GDT
idt_ptr: dw 49*8-1
    dd IDT

bits 16
times 0xfff0-($-$$) db 0xff
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
