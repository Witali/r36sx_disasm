; Intel 80386 PRM 9.1, 12.3.1.4, REP and LODS; AMD APM vol.2 13.1.3/13.1.4.
; Single-step every string element, including the last count/ZF termination.
; A CPL3 -> CPL0 #DB frame must describe completed work, not an extra empty
; REP iteration. Expected progress comes from a separate step counter.
cpu 386
bits 16
org 0

%define ROM 0xf0000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define TSS 0x7000
%define KSTACK 0x9000
%define USTACK 0xa000
%define CODEBUF 0x10000
%define SRC 0x20000
%define DST 0x40000
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define SRCSEG (40|3)
%define DSTSEG (48|3)
%define STACKSEG (56|3)
%define TASK 64
%define VALUE 0x89abcdef
%define FLAGS_TEST 0xbd7 ; TF/IF set, DF added per row, arithmetic sentinels.
%define ARITH_FLAGS 0x8d5
%define FLAGS_MASK 0x37fd7 ; Defined 386 bits, including RF/VM/NT/IOPL.

%define COUNT STATE
%define ROW STATE+4
%define SCENARIO STATE+8
%define CHECK_ID STATE+12
%define GOT STATE+16
%define WANT STATE+20
%define TRAPS STATE+24
%define COMPLETED STATE+28
%define TOTAL STATE+32
%define FINISH_AFTER STATE+36
%define STOP_AFTER STATE+40
%define START_COUNT STATE+44
%define START_SRC STATE+48
%define START_DST STATE+52
%define TEST_FLAGS STATE+56
%define NEXT_IP STATE+60
%define ITERATION STATE+64
%define IS_COMPLETE STATE+68
%define LIVE_FLAGS STATE+72
%define EXPECTED_REGS STATE+128

; Rows contain independent code/address sizes and a signed element stride.
%define R_CS 0
%define R_WIDTH 4
%define R_ADDRESS 8
%define R_KIND 12 ; 0 MOVS, 1 STOS, 2 LODS, 3 CMPS, 4 SCAS.
%define R_STEP 16
%define R_PREFIX 20
%define R_SCENARIOS 24
%define ROW_BYTES 28
%define TOTAL_CASES 1128

; Interrupt gates have no error code: pushad precedes the interlevel frame.
%define F_IP 32
%define F_CS 36
%define F_FLAGS 40
%define F_SP 44
%define F_SS 48

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
    mov word [0xb8000], 0x0754
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
    mov word [IDT+8], debug_trap
    mov word [IDT+0x30*8], complete
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    xor eax, eax
    mov dr7, eax
    mov dx, 0x190
    mov al, 0xef
    out dx, al

case_begin:
    cld
    mov esp, KSTACK
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov esi, [ROW]
    mov dword [CHECK_ID], 0
    mov dword [TRAPS], 0
    mov dword [STOP_AFTER], 0
    ; 0=non-REP; 1=zero; 2=one; 3=three; 4=17 elements. Comparison-only
    ; scenarios 5/6/7 stop on ZF at element 1/2/3, starting with count 3.
    mov eax, 1
    cmp dword [SCENARIO], 0
    je .total
    mov eax, [SCENARIO]
    dec eax
    cmp eax, 1
    jbe .total
    mov eax, 3
    cmp dword [SCENARIO], 4
    jne .stop
    mov eax, 17
.stop:
    mov edx, [SCENARIO]
    sub edx, 4
    jbe .total
    mov [STOP_AFTER], edx
.total:
    mov [TOTAL], eax
    mov [FINISH_AFTER], eax
    cmp dword [STOP_AFTER], 0
    je .counter
    mov edx, [STOP_AFTER]
    mov [FINISH_AFTER], edx
.counter:
    mov [START_COUNT], eax
    cmp dword [SCENARIO], 0
    jne .indices
    mov dword [START_COUNT], 7 ; Non-REP must leave the count untouched.
.indices:
    ; Addr32 must actually use the high index bits, not merely preserve them.
    mov dword [START_SRC], 0x11000
    mov dword [START_DST], 0x12000
    cmp dword [cs:esi+R_ADDRESS], 32
    je .flags
    mov dword [START_SRC], 0x12341000
    mov dword [START_DST], 0xabcd2000
    or dword [START_COUNT], 0x55aa0000
.flags:
    mov dword [TEST_FLAGS], FLAGS_TEST
    cmp dword [cs:esi+R_STEP], 0
    jg .memory
    or dword [TEST_FLAGS], 0x400
.memory:
    mov dword [ITERATION], -1
.element:
    call memory_offsets
    call initial_values
    xor ebx, ebx
.byte:
    mov [SRC+ebp+ebx], al
    mov [DST+edi+ebx], dl
    shr eax, 8
    shr edx, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ITERATION]
    mov eax, [ITERATION]
    cmp eax, [TOTAL]
    jle .element
    ; Emit only the instruction under test followed by INT 30h. Prefixes
    ; count toward NEXT_IP and must be included in the intermediate retry.
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
    cmp dword [SCENARIO], 0
    je .opcode
    mov eax, [cs:esi+R_PREFIX]
    stosb
.opcode:
    mov eax, [cs:esi+R_KIND]
    mov al, [cs:opcodes+eax]
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov eax, edi
    sub eax, CODEBUF
    mov [NEXT_IP], eax
    mov ax, 0x30cd
    stosw
    mov byte [edi], 0xf4
    xor eax, eax
    mov dr6, eax
    push dword STACKSEG
    push dword USTACK
    push dword [TEST_FLAGS]
    push dword [cs:esi+R_CS]
    push dword 0
    mov ecx, [START_COUNT]
    mov esi, [START_SRC]
    mov edi, [START_DST]
    mov ax, SRCSEG
    cmp dword [ss:SCENARIO], 1
    jne .load_source
    xor ax, ax ; Zero REP must not even read through a null data selector.
.load_source:
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, DSTSEG
    cmp dword [ss:SCENARIO], 1
    jne .load_destination
    xor ax, ax
.load_destination:
    mov es, ax
    mov eax, VALUE
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    iretd

; These scalar helpers never execute the string instruction being tested.
memory_offsets:
    mov edi, [ss:ITERATION]
    imul edi, [cs:esi+R_STEP]
    mov ebp, edi
    add edi, [ss:START_DST]
    add ebp, [ss:START_SRC]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .done
    and edi, 0xffff
    and ebp, 0xffff
.done:
    ret

initial_values:
    mov eax, 0xcccccccc
    mov edx, eax
    cmp dword [ss:ITERATION], 0
    jl .done
    mov ecx, [ss:ITERATION]
    cmp ecx, [ss:TOTAL]
    jae .done
    cmp dword [cs:esi+R_KIND], 3
    jae .comparison
    imul ecx, 0x01010101
    mov eax, 0x12345678
    xor eax, ecx ; LODS must return the current element, not the first/last.
    ret
.comparison:
    mov eax, VALUE
    xor ecx, ecx
    cmp dword [cs:esi+R_PREFIX], 0xf2
    sete cl
    mov edx, [ss:ITERATION]
    inc edx
    cmp edx, [ss:STOP_AFTER]
    jne .different
    xor ecx, 1
.different:
    shl ecx, 4
    mov edx, VALUE
    xor edx, ecx ; EF-FF = -10h: CF/PF/SF set, AF/OF/ZF clear.
.done:
    ret

debug_trap:
    pushad
    mov dword [ss:IS_COMPLETE], 0
    jmp check_frame
complete:
    pushad
    mov dword [ss:IS_COMPLETE], 1
check_frame:
    pushfd
    pop eax
    mov [ss:LIVE_FLAGS], eax
    cld
    mov esi, [ss:ROW]
    mov dword [ss:CHECK_ID], 1
    mov edx, [ss:FINISH_AFTER]
    test edx, edx
    jnz .trap_limit
    inc edx ; REP with count zero still retires and single-steps once.
.trap_limit:
    cmp dword [ss:IS_COMPLETE], 0
    jne .all_traps
    inc dword [ss:TRAPS]
    mov eax, [ss:TRAPS]
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    cmp eax, edx
    ja unexpected
    jmp .progress
.all_traps:
    mov eax, [ss:TRAPS]
    call equal
.progress:
    mov eax, [ss:TRAPS]
    cmp dword [ss:TOTAL], 0
    jne .save_progress
    xor eax, eax
.save_progress:
    mov [ss:COMPLETED], eax
    mov dword [ss:CHECK_ID], 2
    xor edx, edx
    cmp eax, [ss:FINISH_AFTER]
    jb .ip
    mov edx, [ss:NEXT_IP]
%ifdef STRING_TRAPS_BAD_ORACLE
    ; Deliberately demand the old, wrong last-iteration REP address.
    cmp dword [ss:SCENARIO], 2
    jne .oracle_done
    xor edx, edx
.oracle_done:
%endif
.ip:
    cmp dword [ss:IS_COMPLETE], 0
    je .check_ip
    add edx, 2 ; INT 30h frame points beyond the software interrupt.
.check_ip:
    mov eax, [ss:esp+F_IP]
    call equal
    mov dword [ss:CHECK_ID], 3
    movzx eax, word [ss:esp+F_CS]
    mov edx, [cs:esi+R_CS]
    call equal
    mov dword [ss:CHECK_ID], 4
    mov edx, [ss:TEST_FLAGS]
    cmp dword [ss:IS_COMPLETE], 0
    je .arith
    and edx, ~0x100 ; The final #DB handler clears TF before INT 30h.
.arith:
    cmp dword [cs:esi+R_KIND], 3
    jb .flags
    cmp dword [ss:COMPLETED], 0
    je .flags
    mov eax, [ss:COMPLETED]
    dec eax
    mov [ss:ITERATION], eax
    push edx
    call initial_values
    xor eax, edx
    pop edx
    and edx, ~ARITH_FLAGS
    test eax, eax
    jnz .unequal
    or edx, 0x44
    jmp .flags
.unequal:
    or edx, 0x85
.flags:
    and edx, FLAGS_MASK
    mov eax, [ss:esp+F_FLAGS]
    and eax, FLAGS_MASK
    call equal
    mov dword [ss:CHECK_ID], 5
    mov eax, [ss:LIVE_FLAGS]
    and eax, 0x300 ; Interrupt gate clears TF/IF in the handler, not frame.
    xor edx, edx
    call equal
    mov dword [ss:CHECK_ID], 6
    movzx eax, word [ss:esp+F_SS]
    mov edx, STACKSEG
    call equal

    mov dword [ss:EXPECTED_REGS], VALUE
    cmp dword [cs:esi+R_KIND], 2
    jne .count
    cmp dword [ss:COMPLETED], 0
    je .count
    mov eax, [ss:COMPLETED]
    dec eax
    mov [ss:ITERATION], eax
    call initial_values
    mov edx, VALUE
    cmp dword [cs:esi+R_WIDTH], 1
    jne .word
    mov dl, al
    jmp .accumulator
.word:
    cmp dword [cs:esi+R_WIDTH], 2
    jne .dword
    mov dx, ax
    jmp .accumulator
.dword:
    mov edx, eax
.accumulator:
    mov [ss:EXPECTED_REGS], edx
.count:
    mov edx, [ss:START_COUNT]
    cmp dword [ss:SCENARIO], 0
    je .save_count
    sub edx, [ss:COMPLETED]
.save_count:
    mov [ss:EXPECTED_REGS+4], edx
    mov dword [ss:EXPECTED_REGS+8], 0x33445566
    mov dword [ss:EXPECTED_REGS+12], 0x44556677
    mov dword [ss:EXPECTED_REGS+16], USTACK
    mov dword [ss:EXPECTED_REGS+20], 0x55667788
    mov eax, [ss:COMPLETED]
    imul eax, [cs:esi+R_STEP]
    mov edx, [ss:START_SRC]
    cmp dword [cs:esi+R_KIND], 1
    je .save_src
    cmp dword [cs:esi+R_KIND], 4
    je .save_src
    add edx, eax
.save_src:
    mov [ss:EXPECTED_REGS+24], edx
    mov edx, [ss:START_DST]
    cmp dword [cs:esi+R_KIND], 2
    je .save_dst
    add edx, eax
.save_dst:
    mov [ss:EXPECTED_REGS+28], edx
    mov dword [ss:CHECK_ID], 7
    xor edi, edi
.reg:
    mov edx, 7
    sub edx, edi
    mov eax, [ss:esp+edx*4]
    cmp edi, 4
    jne .compare
    mov eax, [ss:esp+F_SP]
.compare:
    mov edx, [ss:EXPECTED_REGS+edi*4]
    call equal
    inc edi
    cmp edi, 8
    jb .reg
    mov dword [ss:CHECK_ID], 8
    xor eax, eax
    mov ax, ds
    mov edx, SRCSEG
    cmp dword [ss:SCENARIO], 1
    jne .source_selector
    xor edx, edx
.source_selector:
    call equal
    mov ax, fs
    call equal
    mov ax, gs
    call equal
    mov ax, es
    mov edx, DSTSEG
    cmp dword [ss:SCENARIO], 1
    jne .destination_selector
    xor edx, edx
.destination_selector:
    call equal
    mov dword [ss:CHECK_ID], 9
    mov eax, dr6
    and eax, 0xe00f ; Defined 386 cause bits only; BS must be the sole cause.
    mov edx, 0x4000
    call equal
    mov dword [ss:CHECK_ID], 10
    mov dword [ss:ITERATION], -1
.memory:
    call memory_offsets
    call initial_values
    mov ecx, eax
    ; Only MOVS and STOS change destination bytes, and only completed ones.
    cmp dword [ss:ITERATION], 0
    jl .bytes
    mov ebx, [ss:ITERATION]
    cmp ebx, [ss:COMPLETED]
    jae .bytes
    cmp dword [cs:esi+R_KIND], 0
    jne .store
    mov edx, eax
    jmp .bytes
.store:
    cmp dword [cs:esi+R_KIND], 1
    jne .bytes
    mov edx, VALUE
.bytes:
    xor ebx, ebx
.byte:
    movzx eax, byte [ss:SRC+ebp+ebx]
    mov [ss:GOT], eax
    movzx eax, cl
    mov [ss:WANT], eax
    cmp al, [ss:SRC+ebp+ebx]
    jne unexpected
    movzx eax, byte [ss:DST+edi+ebx]
    mov [ss:GOT], eax
    movzx eax, dl
    mov [ss:WANT], eax
    cmp al, [ss:DST+edi+ebx]
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
    cmp dword [ss:IS_COMPLETE], 0
    jne case_done
    mov eax, [ss:COMPLETED]
    cmp eax, [ss:FINISH_AFTER]
    jb .resume
    and dword [ss:esp+F_FLAGS], ~0x100
.resume:
    ; Clear sticky BS only between steps, so every new trap must set it anew.
    mov eax, [ss:COMPLETED]
    cmp eax, [ss:FINISH_AFTER]
    jae .return
    xor eax, eax
    mov dr6, eax
.return:
    popad
    iretd

equal:
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    cmp eax, edx
    jne unexpected
    ret

case_done:
    inc dword [ss:COUNT]
    inc dword [ss:SCENARIO]
    mov eax, [ss:SCENARIO]
    cmp eax, [cs:esi+R_SCENARIOS]
    jb case_begin
    mov dword [ss:SCENARIO], 0
    add dword [ss:ROW], ROW_BYTES
    cmp dword [ss:ROW], cases_end
    jb case_begin
    mov dword [ss:CHECK_ID], 11
    mov eax, [ss:COUNT]
    mov edx, TOTAL_CASES
    call equal
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
    mov esi, step_text
    call print
    mov eax, [ss:TRAPS]
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
opcodes: db 0xa4,0xaa,0xac,0xa6,0xae
passed: db 'CPU386 STRING TRAPS PASS cases=1128',10,0
failed: db 'CPU386 STRING TRAPS FAIL case=',0
check_text: db ' check=',0
step_text: db ' step=',0
got_text: db ' got=',0
want_text: db ' want=',0

align 4
cases:
%assign case_count 0
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
%if direction
%assign step -width
%endif
%assign kind 0
%rep 5
%assign scenarios 5
%assign prefixes 1
%if kind>=3
%assign scenarios 8
%assign prefixes 2
%endif
%assign prefix 0xf3
%rep prefixes
    dd code,width,address,kind,step,prefix,scenarios
%assign case_count case_count+scenarios
%assign prefix 0xf2
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
%if case_count != TOTAL_CASES
%error "Update string trap case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC SRC, 0x1ffff, 0xf2, 0x40
    DESC DST, 0x1ffff, 0xf2, 0x40
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
