; Intel 80386 PRM Jcc, 9.1/9.8, 12.3; AMD APM v3 rev.3.19 pp.180-183.
; Standalone 64 KiB reset ROM, one of thirteen independent 8192-case phases.
; The truth table is calculated by NASM, not by guest Jcc/SETcc instructions.
cpu 386
bits 16
org 0
%ifndef JCC_PHASE
%define JCC_PHASE 0
%endif
%if JCC_PHASE < 0 || JCC_PHASE > 12
    %error Invalid JCC_PHASE
%endif
%define CASE_COUNT 8192
%assign DB_COUNT 16384
%assign FAULT_COUNT 0
%if JCC_PHASE = 4 || JCC_PHASE = 11
    %assign DB_COUNT 12288
    %assign FAULT_COUNT 2048
%elif JCC_PHASE = 6
    %assign DB_COUNT 8192
    %assign FAULT_COUNT 4096
%elif JCC_PHASE = 9 || JCC_PHASE = 10
    %assign DB_COUNT 12288
    %assign FAULT_COUNT 4096
%elif JCC_PHASE = 12
    %assign DB_COUNT 0
    %assign FAULT_COUNT 8192
%endif

%define ROM 0xf0000
%define PD 0x1000
%define PT 0x2000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define TSS 0x7000
%define KSTACK 0x9000
%define CODEBUF 0x40000
%define STACKBASE 0x20000
%define UESP 0x6100
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define USTACK (40|3)
%define TASK 48
%define CR2_SENTINEL 0x13579bdf
%define FLAGS_MASK 0x37fd7

%define ROW STATE
%define CODE32 STATE+4
%define OP32 STATE+8
%define ADDR32 STATE+12
%define FORM STATE+16 ; 0=short 70+cc, 1=near 0F 80+cc.
%define COND STATE+20
%define TAKEN STATE+24
%define INPUT_FLAGS STATE+28
%define CODE_SEL STATE+32
%define PREFIX_BYTES STATE+36
%define LENGTH STATE+40
%define ENTRY_IP STATE+44
%define NEXT_IP STATE+48
%define TARGET_IP STATE+52
%define CODE_LIMIT STATE+56
%define EXPECT_VEC STATE+60
%define EXPECT_ERR STATE+64
%define EXPECT_IP STATE+68
%define EXPECT_FLAGS STATE+72
%define EXPECT_CR2 STATE+76
%define MISSING_PAGE STATE+80
%define FETCH_FAULT STATE+84
%define STEP STATE+88
%define GOT_VEC STATE+92
%define CHECK_ID STATE+96
%define GOT STATE+100
%define WANT STATE+104
%define TRAPS STATE+108
%define FAULTS STATE+112

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1>>16)&255, %3, ((%2>>16)&15)|%4, (%1>>24)&255
%endmacro

start:
    cli
    cld
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
    xor eax, eax
    mov edi, STATE
    mov ecx, 32
    rep stosd
    mov word [0xb8000], 0x0743 ; C, without a dependency on the video BIOS.
    mov edi, PD
    mov ecx, 2048
    rep stosd
    mov dword [PD], PT|7
    mov edi, PT
    mov eax, 7
    mov ecx, 256
.page:
    stosd
    add eax, 0x1000
    loop .page
    mov dword [PT+8*4], 0x8003
    xor eax, eax
    mov edi, TSS
    mov ecx, 26
    rep stosd
    mov dword [TSS+4], KSTACK
    mov word [TSS+8], KDATA
    mov word [TSS+102], 104
    mov ax, TASK
    ltr ax
    mov edi, IDT
    mov ecx, 50
    mov eax, (unexpected-$$)|(KCODE<<16)
    mov edx, 0x8e00
.gate:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .gate
    mov word [IDT+8], db_handler
    mov word [IDT+13*8], gp_handler
    mov word [IDT+14*8], pf_handler
    mov byte [IDT+0x31*8+5], 0xee
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov edi, CODEBUF
    mov eax, 0x31cd31cd
    mov ecx, 0x20000/4
    rep stosd
    mov dx, 0x190
    mov al, 0xee ; Suppress per-instruction test386 tracing, not exceptions.
    out dx, al

case_begin:
    cld
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    ; Only the two potentially absent code pages need restoring between rows.
    mov dword [PT+0x40*4], 0x40007
    mov dword [PT+0x50*4], 0x50007
    mov eax, cr3
    mov cr3, eax
    mov edi, STACKBASE+UESP-16
    mov eax, 0xcccccccc
    mov ecx, 8
    rep stosd
    mov dword [STEP], 0
    mov dword [FETCH_FAULT], 0
    mov dword [MISSING_PAGE], 0
    mov dword [EXPECT_VEC], 1
    mov dword [EXPECT_ERR], 0
    mov dword [EXPECT_CR2], CR2_SENTINEL
    mov eax, [ROW]
    mov ecx, eax
    and ecx, 15
    mov [COND], ecx
    mov edx, eax
    shr edx, 4
    and edx, 31
    mov ebx, [cs:truth_table+edx*8]
    mov edx, [cs:truth_table+edx*8+4]
    shr edx, cl
    and edx, 1
    mov [TAKEN], edx
    mov edx, eax
    shr edx, 9
    and edx, 1
    mov [ADDR32], edx
    shl edx, 4 ; AF is irrelevant to every Jcc, but must be preserved.
    or ebx, edx
    mov edx, eax
    shr edx, 10
    and edx, 1
    mov [OP32], edx
    mov edx, eax
    shr edx, 11
    and edx, 1
    mov [CODE32], edx
    shl edx, 10 ; Exercise DF independently of the tested condition flags.
    or ebx, edx
    mov [INPUT_FLAGS], ebx
    shr eax, 12
    and eax, 1
    mov [FORM], eax
    ; Length is derived from encoding attributes before any target arithmetic.
    mov ecx, [CODE32]
    xor ecx, [OP32]
    mov edx, [CODE32]
    xor edx, [ADDR32]
    add ecx, edx
    mov [PREFIX_BYTES], ecx
    mov edx, 2
    test eax, eax
    jz .length
    mov edx, [OP32]
    lea edx, [edx*2+4] ; 0F cc + imm16/imm32.
.length:
    add edx, ecx
    mov [LENGTH], edx
    mov eax, 0x1000
    mov ebx, 0x1040
    mov ecx, 0x1ffff
%if JCC_PHASE = 1
    mov eax, 0x1040
    mov ebx, 0x1000
%elif JCC_PHASE = 2
    mov eax, 0x11000
    mov ebx, 0x11040
%elif JCC_PHASE = 3
    mov eax, 0xfff0
    mov ebx, 0x10030
%elif JCC_PHASE = 4
    mov eax, 0x20
    mov ebx, 0xfffffff0
%elif JCC_PHASE = 5
    mov ecx, 0x1040
%elif JCC_PHASE = 6
    mov ecx, 0x103f
%elif JCC_PHASE = 9
    mov eax, 0xffd0
    mov ebx, 0x10040
%elif JCC_PHASE = 10
    mov eax, 0x10000
    sub eax, edx ; Fall-through is on a different page from the whole Jcc.
    mov ebx, 0xffc0
%elif JCC_PHASE = 11
    mov eax, 0x11000
    mov ebx, 0x11040
    mov ecx, 0x1103f
%elif JCC_PHASE = 12
    mov eax, 0x10000
    sub eax, [PREFIX_BYTES]
    sub eax, [FORM]
    dec eax ; First immediate byte is at the first absent address, 10000h.
%endif
    mov [ENTRY_IP], eax
    add eax, edx
    mov [NEXT_IP], eax
%if JCC_PHASE = 7
    lea ebx, [eax+127]
%elif JCC_PHASE = 8
    lea ebx, [eax-128]
%endif
    mov [TARGET_IP], ebx
    mov [CODE_LIMIT], ecx
    mov eax, UCODE16
    cmp dword [CODE32], 0
    je .selector
    mov eax, UCODE32
.selector:
    mov [CODE_SEL], eax
    and eax, ~7
    mov [GDT+eax], cx
    shr ecx, 16
    and byte [GDT+eax+6], 0xf0
    or [GDT+eax+6], cl
    mov edi, [ENTRY_IP]
    add edi, CODEBUF
    mov eax, [OP32]
    cmp eax, [CODE32]
    je .addr_prefix
    mov al, 0x66
    stosb
.addr_prefix:
    mov eax, [ADDR32]
    cmp eax, [CODE32]
    je .opcode
    mov al, 0x67
    stosb
.opcode:
    mov al, 0x70
    cmp dword [FORM], 0
    je .condition
    mov al, 0x0f
    stosb
    mov al, 0x80
.condition:
    add al, [COND]
    stosb
    mov eax, [TARGET_IP]
    sub eax, [NEXT_IP]
    cmp dword [FORM], 0
    jne .near
    stosb
    jmp .encoded
.near:
    cmp dword [OP32], 0
    jne .dword
    stosw
    jmp .encoded
.dword:
    stosd
.encoded:
    ; Truncation and limit checks are conditional: never apply them to the
    ; not-taken sequential EIP, even with operand size 16 above 64 KiB.
    mov eax, [NEXT_IP]
    cmp dword [TAKEN], 0
    je .destination
    mov eax, [TARGET_IP]
    cmp dword [OP32], 0
    jne .limit
    and eax, 0xffff
.limit:
    cmp eax, [CODE_LIMIT]
    jbe .destination
    mov dword [EXPECT_VEC], 13
    mov eax, [ENTRY_IP]
.destination:
    mov [EXPECT_IP], eax
    cmp dword [EXPECT_VEC], 1
    jne .page_scenario
    mov byte [CODEBUF+eax], 0x90
.page_scenario:
%if JCC_PHASE = 9
    mov edx, [TARGET_IP]
    cmp dword [OP32], 0
    jne .missing_target
    and edx, 0xffff
.missing_target:
    add edx, CODEBUF
    and edx, ~0xfff
    mov [MISSING_PAGE], edx
    mov ecx, [TAKEN]
    mov [FETCH_FAULT], ecx
%elif JCC_PHASE = 10
    mov dword [MISSING_PAGE], CODEBUF+0x10000
    mov ecx, [TAKEN]
    xor ecx, 1
    mov [FETCH_FAULT], ecx
%elif JCC_PHASE = 12
    ; This overrides the provisional post-branch oracle: the immediate fetch
    ; fails before either a taken transfer or a not-taken fall-through.
    mov dword [MISSING_PAGE], CODEBUF+0x10000
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 4
    mov dword [EXPECT_CR2], CODEBUF+0x10000
    mov eax, [ENTRY_IP]
    mov [EXPECT_IP], eax
%endif
    mov eax, [MISSING_PAGE]
    test eax, eax
    jz .expected_flags
    shr eax, 12
    mov dword [PT+eax*4], 0
.expected_flags:
    mov eax, [INPUT_FLAGS]
    cmp dword [EXPECT_VEC], 1
    je .save_flags
    or eax, 0x10000
.save_flags:
    mov [EXPECT_FLAGS], eax
%ifdef JCC_BAD_ORACLE
    xor dword [EXPECT_IP], 1
%endif
    mov eax, cr3
    mov cr3, eax
    mov eax, CR2_SENTINEL
    mov cr2, eax
    xor eax, eax
    mov dr6, eax
    push dword USTACK
    push dword UESP
    push dword [INPUT_FLAGS]
    push dword [CODE_SEL]
    push dword [ENTRY_IP]
    xor eax, eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, 0x11223344
    mov ecx, 0x22334455
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    mov esi, 0x66778899
    mov edi, 0x778899aa
    iretd

db_handler:
    push dword 0
    mov dword [ss:GOT_VEC], 1
    jmp check_frame
gp_handler:
    mov dword [ss:GOT_VEC], 13
    jmp check_frame
pf_handler:
    mov dword [ss:GOT_VEC], 14
check_frame:
    pushad
    mov ebp, esp
    mov dword [ss:CHECK_ID], 1
    mov eax, [ss:GOT_VEC]
    mov edx, [ss:EXPECT_VEC]
    call equal
    mov eax, [ss:ebp+32]
    mov edx, [ss:EXPECT_ERR]
    call equal
    mov dword [ss:CHECK_ID], 2
    mov eax, [ss:ebp+36]
    mov edx, [ss:EXPECT_IP]
    call equal
    mov eax, [ss:ebp+40]
    mov edx, [ss:CODE_SEL]
    call equal
    mov dword [ss:CHECK_ID], 3
    mov eax, [ss:ebp+44]
    and eax, FLAGS_MASK
    mov edx, [ss:EXPECT_FLAGS]
    and edx, FLAGS_MASK
    call equal
    mov eax, cr2
    mov edx, [ss:EXPECT_CR2]
    call equal
    mov dword [ss:CHECK_ID], 4
    xor ecx, ecx
.gpr:
    cmp ecx, 3
    je .skip_sp
    mov eax, [ss:ebp+ecx*4]
    mov edx, [cs:init_gprs+ecx*4]
    call equal
.skip_sp:
    inc ecx
    cmp ecx, 8
    jb .gpr
    mov eax, [ss:ebp+48]
    mov edx, UESP
    call equal
    mov eax, [ss:ebp+52]
    mov edx, USTACK
    call equal
    mov dword [ss:CHECK_ID], 5
    xor eax, eax
    xor edx, edx
    mov ax, ds
    call equal
    mov ax, es
    call equal
    mov ax, fs
    call equal
    mov ax, gs
    call equal
    mov dword [ss:CHECK_ID], 6
    mov edi, STACKBASE+UESP-16
    mov ecx, 8
.stack:
    mov eax, [ss:edi]
    mov edx, 0xcccccccc
    call equal
    add edi, 4
    loop .stack
    cmp dword [ss:GOT_VEC], 1
    jne .fault_done
    inc dword [ss:TRAPS]
    mov eax, dr6
    and eax, 0x400f
    mov edx, 0x4000
    call equal
    inc dword [ss:STEP]
    cmp dword [ss:STEP], 1
    jne .next
    cmp dword [ss:FETCH_FAULT], 0
    jne .next_fetch_fault
    inc dword [ss:EXPECT_IP]
    jmp .resume
.next_fetch_fault:
    ; The branch already retired at the checked EIP. Now observe its fetch
    ; without TF, distinguishing this #PF from an early fault on Jcc itself.
    mov dword [ss:EXPECT_VEC], 14
    mov dword [ss:EXPECT_ERR], 4
    mov eax, [ss:EXPECT_IP]
    add eax, CODEBUF
    mov [ss:EXPECT_CR2], eax
    and dword [ss:ebp+44], ~0x100
    and dword [ss:EXPECT_FLAGS], ~0x100
    or dword [ss:EXPECT_FLAGS], 0x10000
.resume:
    popad
    add esp, 4
    iretd
.fault_done:
    inc dword [ss:FAULTS]
.next:
    mov esp, KSTACK
    inc dword [ss:ROW]
    cmp dword [ss:ROW], CASE_COUNT
    jb case_begin
    mov dword [ss:CHECK_ID], 7
    mov eax, [ss:TRAPS]
    mov edx, DB_COUNT
    call equal
    mov eax, [ss:FAULTS]
    mov edx, FAULT_COUNT
    call equal
    mov esi, pass_msg
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt
equal:
    cmp eax, edx
    jne failure
    ret
failure:
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    mov esi, fail_msg
    call print
    mov eax, [ss:ROW]
    call hex32
    mov esi, check_msg
    call print
    mov eax, [ss:CHECK_ID]
    call hex32
    mov esi, step_msg
    call print
    mov eax, [ss:STEP]
    call hex32
    mov esi, got_msg
    call print
    mov eax, [ss:GOT]
    call hex32
    mov esi, want_msg
    call print
    mov eax, [ss:WANT]
    call hex32
    mov dx, 0x191
    mov al, 10
    out dx, al
    mov al, 0xfe
    out 0x80, al
halt:
    cli
    hlt
    jmp halt
unexpected:
    mov dword [ss:CHECK_ID], 8
    mov eax, [ss:esp]
    xor edx, edx
    jmp failure
print:
    ; DF belongs to the tested state; logging must work with either value.
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
    mov al, [cs:digits+eax]
    mov dx, 0x191
    out dx, al
    loop .digit
    ret

align 4
init_gprs: dd 0x778899aa,0x66778899,0x55667788,UESP
           dd 0x44556677,0x33445566,0x22334455,0x11223344
truth_table:
; Bit positions in the packed test input are CF,PF,ZF,SF,OF.
; Each word of expected results corresponds to the Intel condition order.
%assign f 0
%rep 32
%assign c (f & 1)
%assign p ((f >> 1) & 1)
%assign z ((f >> 2) & 1)
%assign s ((f >> 3) & 1)
%assign o ((f >> 4) & 1)
%assign mask (o | ((o^1)<<1) | (c<<2) | ((c^1)<<3) | (z<<4) | ((z^1)<<5))
%assign mask (mask | ((c|z)<<6) | (((c|z)^1)<<7) | (s<<8) | ((s^1)<<9))
%assign mask (mask | (p<<10) | ((p^1)<<11) | ((s^o)<<12) | (((s^o)^1)<<13))
%assign mask (mask | ((z|(s^o))<<14) | (((z|(s^o))^1)<<15))
    dd 0x102 | c | (p<<2) | (z<<6) | (s<<7) | (o<<11), mask
%assign f f+1
%endrep
align 8
descriptors:
    dq 0
    DESC ROM,0xffff,0x9a,0x40
    DESC 0,0xfffff,0x92,0xc0
    DESC CODEBUF,0x1ffff,0xfa,0
    DESC CODEBUF,0x1ffff,0xfa,0x40
    DESC STACKBASE,0xfffff,0xf2,0xc0
    DESC TSS,103,0x89,0
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
         dd GDT
idt_ptr: dw 50*8-1
         dd IDT
digits: db '0123456789ABCDEF'
pass_msg: db 'CPU386 JCC PASS phase=', '0'+JCC_PHASE/10, '0'+(JCC_PHASE % 10), ' cases=8192',10,0
fail_msg: db 'CPU386 JCC FAIL phase=', '0'+JCC_PHASE/10, '0'+(JCC_PHASE % 10), ' case=',0
check_msg: db ' check=',0
step_msg: db ' step=',0
got_msg: db ' got=',0
want_msg: db ' want=',0
times 0xfff0-($-$$) db 0xff
bits 16
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
