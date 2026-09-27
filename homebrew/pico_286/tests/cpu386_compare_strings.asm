; Intel 80386 PRM ch.17 CMPS/SCAS/REP, AMD APM v3 CMPS/SCAS and 1.2.6.
; 64 KiB ROM at F0000h. Compare/scan completion, flags and index/count sizes.
; Expected subtraction flags are calculated by NASM, not by the guest CMP.
; Fault restart and interrupt-time EFLAGS restoration are separate tests.
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
%define DSBASE 0x20000
%define ESBASE 0x40000
%define FSBASE 0x60000
%define GSBASE 0x80000
%define KCODE 8
%define KDATA 16
%define CODE16 (24|3)
%define CODE32 (32|3)
%define DATASEG (40|3)
%define EXTRA (48|3)
%define FSEG (56|3)
%define GSEG (64|3)
%define STACKSEG (72|3)
%define TASK 80
%define ARITH_FLAGS 0x8d5
%define CHECK_FLAGS 0xed5
%define TOTAL_CASES 10752
%define PAIR_BYTES 12
%define PAIRS_PER_WIDTH 12
%define SCENARIO_BYTES 20
%define ROW_BYTES 28

%define COUNT STATE
%define ROW STATE+4
%define SCENARIO STATE+8
%define CHECK_ID STATE+12
%define GOT STATE+16
%define WANT STATE+20
%define NEXT_IP STATE+24
%define N_ELEMENTS STATE+28
%define N_EXECUTED STATE+32
%define START_INDEX STATE+36
%define START_FLAGS STATE+40
%define END_FLAGS STATE+44
%define LEFT_VALUE STATE+48
%define RIGHT_VALUE STATE+52
%define PAIR_FLAGS STATE+56
%define ITERATION STATE+60
%define MEM_SOURCE STATE+64
%define MEM_OTHER STATE+68
%define MEM_DEST STATE+72
%define SAVED_DS STATE+76
%define SAVED_ES STATE+80
%define SAVED_FS STATE+84
%define SAVED_GS STATE+88
%define TEST_DS STATE+92
%define TEST_ES STATE+96
%define TEST_FS STATE+100
%define TEST_GS STATE+104
%define INITIAL_ECX STATE+108
%define INITIAL_ESI STATE+112
%define INITIAL_EDI STATE+116
%define EXPECTED_REGS STATE+128

%define R_CS 0
%define R_ADDRESS 4
%define R_WIDTH 8
%define R_KIND 12 ; 0=CMPS, 1=SCAS.
%define R_STEP 16
%define R_PREFIX 20 ; No override, FS, GS or SS. SCAS must ignore all of them.
%define R_BASE 24
%define S_PREFIX 0 ; 0=single, F3=REPE, F2=REPNE.
%define S_COUNT 4
%define S_STOP 8 ; One-based terminating comparison, or -1 for count exhaustion.
%define S_PAIR 12
%define S_FLAGS 16

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
    mov word [0xb8000], 0x0743
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
.idt:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .idt
    mov word [IDT+0x30*8], check_frame
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    mov dword [COUNT], 0
    mov dword [ROW], rows
    mov dword [SCENARIO], scenarios
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
    mov esi, [ROW]
    mov ebp, [SCENARIO]
    mov ecx, [cs:ebp+S_COUNT]
    mov [INITIAL_ECX], ecx
    cmp dword [cs:ebp+S_PREFIX], 0
    jne .repeat_count
    mov ecx, 1
.repeat_count:
    mov [N_ELEMENTS], ecx
    mov eax, [cs:ebp+S_STOP]
    cmp eax, ecx
    jae .iterations
    mov ecx, eax
.iterations:
    mov [N_EXECUTED], ecx
    ; Start adjacent to the wrap boundary without splitting any element.
    mov eax, 0x10000
    cmp dword [cs:esi+R_STEP], 0
    jl .index
    sub eax, [cs:esi+R_WIDTH]
.index:
    cmp dword [cs:esi+R_ADDRESS], 32
    je .index_done
    and eax, 0xffff
    or dword [INITIAL_ECX], 0x55aa0000
.index_done:
    mov [START_INDEX], eax
    mov [INITIAL_ESI], eax
    mov [INITIAL_EDI], eax
    cmp dword [cs:esi+R_ADDRESS], 32
    je .pair
    or dword [INITIAL_ESI], 0x12340000
    or dword [INITIAL_EDI], 0xabcd0000
.pair:
    mov eax, [cs:esi+R_WIDTH]
    shr eax, 1 ; Widths 1/2/4 select tables 0/1/2.
    imul eax, PAIRS_PER_WIDTH
    add eax, [cs:ebp+S_PAIR]
    imul eax, PAIR_BYTES
    mov edx, [cs:pairs+eax]
    mov [LEFT_VALUE], edx
    mov edx, [cs:pairs+eax+4]
    mov [RIGHT_VALUE], edx
    mov edx, [cs:pairs+eax+8]
    mov [PAIR_FLAGS], edx
    mov eax, [cs:ebp+S_FLAGS]
    cmp dword [cs:esi+R_STEP], 0
    jg .flags
    or eax, 0x400
.flags:
    mov [START_FLAGS], eax
    mov [END_FLAGS], eax
    cmp dword [N_EXECUTED], 0
    je .memory
    cmp dword [cs:ebp+S_PREFIX], 0
    je .result_flags
    ; REP scenarios use pair 0-1 (non-equal). Choose the expected LAST result.
    mov ebx, [cs:ebp+S_PREFIX]
    cmp dword [cs:ebp+S_STOP], -1
    je .final_relation
    xor ebx, 1 ; Break reverses the relation required by the prefix.
.final_relation:
    cmp ebx, 0xf3
    jne .result_flags
    mov edx, 0x44 ; Any equal comparison sets PF/ZF, clears CF/AF/SF/OF.
.result_flags:
    and eax, ~ARITH_FLAGS
    or eax, edx
    mov [END_FLAGS], eax
.memory:
    mov dword [ITERATION], -2
.init:
    call memory_values
    xor ecx, ecx
.init_byte:
    mov eax, [MEM_OTHER]
    mov [DSBASE+edi+ecx], al
    mov [FSBASE+edi+ecx], al
    mov [GSBASE+edi+ecx], al
    mov ebx, [cs:esi+R_BASE]
    mov eax, [MEM_SOURCE]
    add ebx, edi
    mov [ebx+ecx], al
    mov eax, [MEM_DEST]
    mov [ESBASE+edi+ecx], al
    shr dword [MEM_OTHER], 8
    shr dword [MEM_SOURCE], 8
    shr dword [MEM_DEST], 8
    inc ecx
    cmp ecx, [cs:esi+R_WIDTH]
    jb .init_byte
    inc dword [ITERATION]
    mov eax, [N_ELEMENTS]
    add eax, 2
    cmp [ITERATION], eax
    jl .init

    mov edi, CODEBUF
    mov eax, [cs:esi+R_PREFIX]
    test eax, eax
    jz .operand
    stosb
.operand:
    mov edx, 16
    cmp dword [cs:esi+R_CS], CODE16
    je .operand_bits
    mov edx, 32
.operand_bits:
    mov eax, 16
    cmp dword [cs:esi+R_WIDTH], 4
    jne .operand_prefix
    mov eax, 32
.operand_prefix:
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
    mov eax, [cs:ebp+S_PREFIX]
    test eax, eax
    jz .opcode
    stosb
.opcode:
    mov al, 0xa6
    cmp dword [cs:esi+R_KIND], 0
    je .width
    mov al, 0xae
.width:
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov ax, 0x30cd
    stosw
    sub edi, CODEBUF
    mov [NEXT_IP], edi

    mov eax, [LEFT_VALUE]
    cmp dword [cs:esi+R_WIDTH], 4
    je .eax
    or eax, 0x98760000
    cmp dword [cs:esi+R_WIDTH], 1
    jne .eax
    or eax, 0x5500
.eax:
    mov [EXPECTED_REGS], eax
    mov eax, [INITIAL_ECX]
    cmp dword [cs:ebp+S_PREFIX], 0
    je .ecx
    sub eax, [N_EXECUTED]
.ecx:
    mov [EXPECTED_REGS+4], eax
    mov dword [EXPECTED_REGS+8], 0x33445566
    mov dword [EXPECTED_REGS+12], 0x44556677
    mov dword [EXPECTED_REGS+16], USTACK
    mov dword [EXPECTED_REGS+20], 0x55667788
    mov eax, [N_EXECUTED]
    imul eax, [cs:esi+R_STEP]
    add eax, [START_INDEX]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .end_index
    and eax, 0xffff
.end_index:
    mov edx, eax
    cmp dword [cs:esi+R_ADDRESS], 32
    je .si
    or eax, 0x12340000
    or edx, 0xabcd0000
.si:
    cmp dword [cs:esi+R_KIND], 0
    je .save_si
    mov eax, [INITIAL_ESI] ; SCAS must never change the source index.
.save_si:
    mov [EXPECTED_REGS+24], eax
    mov [EXPECTED_REGS+28], edx
    mov dword [TEST_DS], DATASEG
    mov dword [TEST_ES], EXTRA
    mov dword [TEST_FS], FSEG
    mov dword [TEST_GS], GSEG
    ; Null selectors detect accidental source reads by SCAS, or any read by
    ; a zero-count REP. SS remains valid for the CPL3 stack and SS overrides.
    cmp dword [N_EXECUTED], 0
    jne .source_selectors
    mov dword [TEST_ES], 0
.source_selectors:
    cmp dword [cs:esi+R_KIND], 1
    je .null_source
    cmp dword [N_EXECUTED], 0
    jne .enter
.null_source:
    mov dword [TEST_DS], 0
    mov dword [TEST_FS], 0
    mov dword [TEST_GS], 0
.enter:
    push dword STACKSEG
    push dword USTACK
    push dword [START_FLAGS]
    push dword [cs:esi+R_CS]
    push dword 0
    mov ecx, [INITIAL_ECX]
    mov edi, [INITIAL_EDI]
    mov esi, [INITIAL_ESI]
    mov ax, [TEST_DS]
    mov ds, ax
    mov ax, [ss:TEST_ES]
    mov es, ax
    mov ax, [ss:TEST_FS]
    mov fs, ax
    mov ax, [ss:TEST_GS]
    mov gs, ax
    mov eax, [ss:EXPECTED_REGS]
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    iretd

; Return offset in EDI and the initialized values for each memory region.
; Guard elements are all CCh. Unselected source segments differ by bit 0,
; exposing a decoder which ignores a source override or uses it for SCAS.
memory_values:
    mov edi, [ITERATION]
    imul edi, [cs:esi+R_STEP]
    add edi, [START_INDEX]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .offset
    and edi, 0xffff
.offset:
    mov dword [MEM_SOURCE], 0xcccccccc
    mov dword [MEM_OTHER], 0xcccccccc
    mov dword [MEM_DEST], 0xcccccccc
    mov eax, [ITERATION]
    cmp eax, [N_ELEMENTS]
    jae .done
    mov eax, [LEFT_VALUE]
    mov [MEM_SOURCE], eax
    xor eax, 1
    mov [MEM_OTHER], eax
    mov eax, [RIGHT_VALUE]
    cmp dword [cs:ebp+S_PREFIX], 0
    je .dest
    mov ebx, [cs:ebp+S_PREFIX]
    mov edx, [ITERATION]
    inc edx
    cmp edx, [cs:ebp+S_STOP]
    jne .relation
    xor ebx, 1
.relation:
    cmp ebx, 0xf3
    jne .dest
    mov eax, [LEFT_VALUE]
.dest:
    mov [MEM_DEST], eax
.done:
    ret

check_frame:
    pushad
    cld
    mov ax, ds
    mov [ss:SAVED_DS], ax
    mov ax, es
    mov [ss:SAVED_ES], ax
    mov ax, fs
    mov [ss:SAVED_FS], ax
    mov ax, gs
    mov [ss:SAVED_GS], ax
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov esi, [ROW]
    mov ebp, [SCENARIO]
    mov dword [CHECK_ID], 1
    xor edi, edi
.gpr:
    mov edx, 7
    sub edx, edi
    mov eax, [esp+edx*4]
    cmp edi, 4
    jne .reg
    mov eax, [esp+44] ; Old ESP in the CPL3 -> CPL0 interrupt frame.
.reg:
    mov edx, [EXPECTED_REGS+edi*4]
    call expect
    inc edi
    cmp edi, 8
    jb .gpr
    mov dword [CHECK_ID], 2
    mov eax, [esp+40]
    and eax, CHECK_FLAGS
    mov edx, [END_FLAGS]
    and edx, CHECK_FLAGS
    call expect
    mov eax, [esp+32]
    mov edx, [NEXT_IP]
    call expect
    mov eax, [esp+36]
    mov edx, [cs:esi+R_CS]
    call expect
    mov eax, [esp+48]
    mov edx, STACKSEG
    call expect
    movzx eax, word [SAVED_DS]
    mov edx, [TEST_DS]
    call expect
    movzx eax, word [SAVED_ES]
    mov edx, [TEST_ES]
    call expect
    movzx eax, word [SAVED_FS]
    mov edx, [TEST_FS]
    call expect
    movzx eax, word [SAVED_GS]
    mov edx, [TEST_GS]
    call expect
    mov dword [CHECK_ID], 3
    mov dword [ITERATION], -2
.memory:
    call memory_values
    xor ecx, ecx
.byte:
    ; Compare/scan must not write either memory operand or adjacent guards.
    mov ebx, DSBASE
    call source_byte
    mov ebx, FSBASE
    call source_byte
    mov ebx, GSBASE
    call source_byte
    movzx eax, byte [ESBASE+edi+ecx]
    movzx edx, byte [MEM_DEST]
    call expect
    shr dword [MEM_SOURCE], 8
    shr dword [MEM_OTHER], 8
    shr dword [MEM_DEST], 8
    inc ecx
    cmp ecx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ITERATION]
    mov eax, [N_ELEMENTS]
    add eax, 2
    cmp [ITERATION], eax
    jl .memory
    inc dword [COUNT]
    add dword [SCENARIO], SCENARIO_BYTES
    cmp dword [SCENARIO], scenarios_end
    jb case_begin
    mov dword [SCENARIO], scenarios
    add dword [ROW], ROW_BYTES
    cmp dword [ROW], rows_end
    jb case_begin
    mov dword [CHECK_ID], 4
    mov eax, [COUNT]
    mov edx, TOTAL_CASES
    call expect
    mov esi, passed
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt
source_byte:
    movzx edx, byte [MEM_OTHER]
    cmp ebx, [cs:esi+R_BASE]
    jne .read
    movzx edx, byte [MEM_SOURCE]
.read:
    add ebx, edi
    movzx eax, byte [ebx+ecx]
    jmp expect
expect:
    mov [GOT], eax
    mov [WANT], edx
    cmp eax, edx
    jne unexpected
    ret
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
passed: db 'CPU386 COMPARE STRINGS PASS cases=10752',10,0
failed: db 'CPU386 COMPARE STRINGS FAIL case=',0
check_text: db ' check=',0
got_text: db ' got=',0
want_text: db ' want=',0

; NASM computes CF/PF/AF/ZF/SF/OF independently of the emulator's flag helpers.
%macro PAIR 3
%assign mask ((1 << (%1*8))-1)
%assign sign (1 << (%1*8-1))
%assign left ((%2)&mask)
%assign right ((%3)&mask)
%assign result ((left-right)&mask)
%assign flags 0
%if left < right
%assign flags flags|1
%endif
%assign parity result&255
%assign parity parity^(parity>>4)
%assign parity parity^(parity>>2)
%assign parity parity^(parity>>1)
%if (parity&1) = 0
%assign flags flags|4
%endif
%if (left^right^result)&0x10
%assign flags flags|0x10
%endif
%if result = 0
%assign flags flags|0x40
%endif
%if result&sign
%assign flags flags|0x80
%endif
%if (left^right)&(left^result)&sign
%assign flags flags|0x800
%endif
; An optional negative-control ROM must fail case 0's flags check. It verifies
; that the comparison oracle and failure path are live, not merely the loop.
%ifdef COMPARE_STRINGS_BAD_ORACLE
%assign flags flags^1
%endif
    dd left,right,flags
%endmacro
align 4
pairs:
%assign width 1
%rep 3
%assign signbit 1 << (width*8-1)
%assign allbits (1 << (width*8))-1
    PAIR width, 0, 0
    PAIR width, 0, 1
    PAIR width, 1, 0
    PAIR width, 0x10, 1
    PAIR width, 0xf, 0x10
    PAIR width, signbit, 1
    PAIR width, signbit-1, allbits
    PAIR width, allbits, allbits
    PAIR width, allbits, signbit
    PAIR width, signbit, allbits
    PAIR width, 0x55, 0xaa
    PAIR width, 0xaa, 0x55
%assign width width*2
%endrep

scenarios:
%assign initial 0
%rep 2
%assign initial_flags 0x897
%if initial
%assign initial_flags 0x242 ; Opposite arithmetic flags, ZF=1 and IF=1.
%endif
%assign pair 0
%rep 12
    dd 0,7,-1,pair,initial_flags
%assign pair pair+1
%endrep
%assign prefix 0xf3
%rep 2
    dd prefix,0,-1,1,initial_flags
    dd prefix,1,-1,1,initial_flags
    dd prefix,1,1,1,initial_flags
    dd prefix,5,1,1,initial_flags
    dd prefix,5,3,1,initial_flags
    dd prefix,5,-1,1,initial_flags
    dd prefix,5,5,1,initial_flags
    dd prefix,1025,-1,1,initial_flags
%assign prefix 0xf2
%endrep
%assign initial 1
%endrep
scenarios_end:

rows:
%assign code CODE16
%rep 2
%assign address 16
%rep 2
%assign width 1
%rep 3
%assign kind 0
%rep 2
%assign direction 0
%rep 2
%assign step width
%if direction
%assign step -width
%endif
    dd code,address,width,kind,step,0,DSBASE
    dd code,address,width,kind,step,0x64,FSBASE
    dd code,address,width,kind,step,0x65,GSBASE
    dd code,address,width,kind,step,0x36,FSBASE
%assign direction 1
%endrep
%assign kind 1
%endrep
%assign width width*2
%endrep
%assign address 32
%endrep
%assign code CODE32
%endrep
rows_end:
%if (rows_end-rows)/ROW_BYTES * (scenarios_end-scenarios)/SCENARIO_BYTES != TOTAL_CASES
%error "Update comparison case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC DSBASE, 0x1ffff, 0xf2, 0x40
    DESC ESBASE, 0x1ffff, 0xf2, 0x40
    DESC FSBASE, 0x1ffff, 0xf2, 0x40
    DESC GSBASE, 0x1ffff, 0xf2, 0x40
    DESC FSBASE, 0x1ffff, 0xf2, 0x40
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
