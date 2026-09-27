; 64 KiB reset ROM. Intel 80386 PRM PUSH, PUSHF, 9.1 and 9.8.12/14;
; AMD APM vol.3 rev.3.19 PUSH/PUSHF pp.258-262 (legacy behavior only).
; Each snippet runs at CPL3. A CPL0 handler checks the original registers,
; the privilege-transition frame and the stack contents, then starts a new
; case. Faults must leave SP/ESP unchanged so the instruction is restartable.
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
%define KSTACK_TOP 0x9000
%define USTACK_TOP 0x6100
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define UDATA (40|3)
%define USTACK16 (48|3)
%define USTACK32 (56|3)
%define TASK 64
%define FLAGS_TEST 0x8d7
%define ARITH_FLAGS 0x8d5
%define CANARY 0xcccccccc

%define COUNT STATE
%define CONTEXT STATE+4
%define ROW STATE+8
%define EXPECT_VEC STATE+12
%define EXPECT_ERR STATE+16
%define EXPECT_SS STATE+20
%define GOT_VEC STATE+24
%define CHECK_ID STATE+28

; Table fields. The instruction bytes are emitted separately from the table.
%define ROW_IP 0
%define ROW_NEXT 4
%define ROW_CS 8
%define ROW_SIZE 12
%define ROW_VALUE 16
%define ROW_MASK 20
%define ROW_KIND 24
%define ROW_BYTES 28
%define VALUE_LITERAL 0
%define VALUE_CS 1
%define VALUE_SS 2

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1 >> 16) & 0xff, %3, ((%2 >> 16) & 15) | %4, (%1 >> 24) & 0xff
%endmacro

start:
    cli
    cld
    xor ax, ax
    mov ss, ax
    mov sp, KSTACK_TOP
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
    mov esp, KSTACK_TOP
    mov dword [COUNT], 0
    mov dword [CONTEXT], 0
    mov word [0xb8000], 0x0750 ; 'P' proves a nonblank framebuffer.

    mov edi, PD
    xor eax, eax
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
    ; The handler's stack is supervisor-only. Interrupt frame writes must use
    ; the destination CPL, even when they originated from a user-mode fault.
    mov dword [PT+8*4], 0x8003
    mov edi, TSS
    xor eax, eax
    mov ecx, 26
    rep stosd
    mov dword [TSS+4], KSTACK_TOP
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
    mov word [IDT+12*8], ss_handler
    mov word [IDT+14*8], pf_handler
    mov word [IDT+0x30*8], success_handler
    mov byte [IDT+0x30*8+5], 0xee ; User-callable completion interrupt.
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ; Avoid rearming the verbose instruction trace for hundreds of cases.
    ; On failure the counter/check ID below identifies the exact table entry.
    mov dx, 0x190
    mov al, 0xee
    out dx, al

context_begin:
    mov dword [ROW], cases
case_begin:
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    ; Even contexts use SS.B=0, odd ones SS.B=1. Use the same low ESP in
    ; both: upper ESP preservation across gates is a separate test contract.
    mov eax, [CONTEXT]
    and eax, 1
    shl eax, 3
    add eax, USTACK16
    mov [EXPECT_SS], eax
    mov word [GDT+48], 0xffff
    mov word [GDT+56], 0xffff
    mov byte [GDT+48+5], 0xf2
    mov byte [GDT+56+5], 0xf2
    mov dword [PT+6*4], 0x6007
    mov eax, cr3
    mov cr3, eax
    mov dword [USTACK_TOP-4], CANARY
    mov dword [USTACK_TOP], CANARY
    mov dword [0xa020], 0x89abcdef
    mov dword [0xa040], 0x76543210
    mov dword [EXPECT_VEC], 0x30
    mov dword [EXPECT_ERR], 0
    mov eax, [CONTEXT]
    shr eax, 1
    test eax, eax
    jz .ready
    cmp eax, 1
    jne .page_fault
    ; Expand-down lower bound is exclusive: SP=6100 is valid, but a push
    ; reaches 60FE/60FC below the first permitted byte and must raise #SS(0).
    mov word [GDT+48], USTACK_TOP-1
    mov word [GDT+56], USTACK_TOP-1
    mov byte [GDT+48+5], 0xf6
    mov byte [GDT+56+5], 0xf6
    mov dword [EXPECT_VEC], 12
    jmp .ready
.page_fault:
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 6 ; User write, non-present page.
    mov dword [PT+6*4], 0
    cmp eax, 2
    je .ready
    mov dword [EXPECT_ERR], 7 ; User write to a present read-only page.
    mov dword [PT+6*4], 0x6005
.ready:
    mov eax, cr3
    mov cr3, eax
    push dword [EXPECT_SS]
    push dword USTACK_TOP
    push dword FLAGS_TEST
    push dword [cs:esi+ROW_CS]
    push dword [cs:esi+ROW_IP]
    mov ax, UDATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, 0x11223344
    mov ecx, 0x22334455
    mov edx, 0x33445566
    mov ebx, 0xa020
    mov ebp, 0x55667788
    mov esi, 0xa040
    mov edi, 0x778899aa
    iretd

success_handler:
    push dword 0 ; Match the hardware error-code layout used by #SS/#PF.
    mov dword [es:GOT_VEC], 0x30
    jmp check_frame
ss_handler:
    mov dword [es:GOT_VEC], 12
    jmp check_frame
pf_handler:
    mov dword [es:GOT_VEC], 14
check_frame:
    pushad
    mov dword [es:CHECK_ID], 1
    mov eax, [es:GOT_VEC]
    cmp eax, [es:EXPECT_VEC]
    jne unexpected
    mov eax, [ss:esp+32]
    cmp eax, [es:EXPECT_ERR]
    jne unexpected
    mov dword [es:CHECK_ID], 2
    cmp dword [ss:esp+28], 0x11223344
    jne unexpected
    cmp dword [ss:esp+24], 0x22334455
    jne unexpected
    cmp dword [ss:esp+20], 0x33445566
    jne unexpected
    cmp dword [ss:esp+16], 0xa020
    jne unexpected
    cmp dword [ss:esp+8], 0x55667788
    jne unexpected
    cmp dword [ss:esp+4], 0xa040
    jne unexpected
    cmp dword [ss:esp], 0x778899aa
    jne unexpected
    mov esi, [es:ROW]
    mov dword [es:CHECK_ID], 3
    mov eax, [ss:esp+40]
    and eax, 0xffff
    cmp eax, [cs:esi+ROW_CS]
    jne unexpected
    mov eax, [ss:esp+52]
    and eax, 0xffff
    cmp eax, [es:EXPECT_SS]
    jne unexpected
    mov eax, [ss:esp+44]
    and eax, ARITH_FLAGS
    cmp eax, FLAGS_TEST & ARITH_FLAGS
    jne unexpected
    mov ax, ds
    cmp ax, UDATA
    jne unexpected
    mov ax, es
    cmp ax, UDATA
    jne unexpected
    mov ax, fs
    cmp ax, UDATA
    jne unexpected
    mov ax, gs
    cmp ax, UDATA
    jne unexpected
    mov dword [es:CHECK_ID], 4
    mov eax, [cs:esi+ROW_IP]
    mov edx, USTACK_TOP
    cmp dword [es:EXPECT_VEC], 0x30
    jne .fault_frame
    mov eax, [cs:esi+ROW_NEXT]
    sub edx, [cs:esi+ROW_SIZE]
.fault_frame:
    cmp [ss:esp+36], eax
    jne unexpected
    cmp [ss:esp+48], edx ; Faulting PUSH must preserve the pre-instruction SP.
    jne unexpected
    mov dword [es:CHECK_ID], 5
    cmp dword [es:EXPECT_VEC], 14
    jne .memory
    mov eax, cr2
    mov edx, USTACK_TOP
    sub edx, [cs:esi+ROW_SIZE]
    cmp eax, edx
    jne unexpected
.memory:
    ; Make the backing page readable again to check that a rejected write did
    ; not touch data. Page-table accessed/dirty bits are not part of this test.
    mov dword [es:PT+6*4], 0x6007
    mov eax, cr3
    mov cr3, eax
    mov dword [es:CHECK_ID], 6
    cmp dword [es:USTACK_TOP], CANARY
    jne unexpected
    cmp dword [es:EXPECT_VEC], 0x30
    je .written
    cmp dword [es:USTACK_TOP-4], CANARY
    jne unexpected
    jmp .next
.written:
    mov eax, [cs:esi+ROW_VALUE]
    cmp dword [cs:esi+ROW_KIND], VALUE_CS
    jne .not_cs
    mov eax, [cs:esi+ROW_CS]
.not_cs:
    cmp dword [cs:esi+ROW_KIND], VALUE_SS
    jne .not_ss
    mov eax, [es:EXPECT_SS]
.not_ss:
    mov edx, USTACK_TOP
    sub edx, [cs:esi+ROW_SIZE]
    mov ebx, [es:edx]
    and ebx, [cs:esi+ROW_MASK]
    and eax, [cs:esi+ROW_MASK]
    cmp ebx, eax
    jne unexpected
    cmp dword [cs:esi+ROW_SIZE], 2
    jne .next
    cmp word [es:USTACK_TOP-4], CANARY & 0xffff
    jne unexpected
.next:
    ; Each case has an independent ring transition. Do not return to the user
    ; snippet after a fault or depend on fixing its descriptor for IRETD.
    mov esp, KSTACK_TOP
    inc dword [COUNT]
    add dword [ROW], ROW_BYTES
    cmp dword [ROW], cases_end
    jb case_begin
    inc dword [CONTEXT]
    cmp dword [CONTEXT], 8
    jb context_begin
    mov dword [CHECK_ID], 7
    cmp dword [COUNT], 8*(cases_end-cases)/ROW_BYTES
    jne unexpected
    mov esi, passed
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt

unexpected:
    mov esi, failed
    call print
    mov eax, [es:COUNT]
    call hex32
    mov esi, check_text
    call print
    mov eax, [es:CHECK_ID]
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
passed: db 'CPU386 PUSH PASS cases=864',10,0
failed: db 'CPU386 PUSH FAIL case=',0
check_text: db ' check=',0

; Generate both the snippets and their independent value/width expectations.
; Raw ModRM encodings deliberately test both register forms of PUSH, including
; FF /6 with mod=11 and PUSH ESP before its implicit stack decrement.
%macro CASE 6+
%if EMIT_TABLE
    dd code%+codebits%+_%1_%+opbits, done%+codebits%+_%1_%+opbits
    dd user_cs, opbits/8, %2, %3, %4
%else
code%+codebits%+_%1_%+opbits:
%if opbits != codebits
    db 0x66
%endif
    db %5, %6
    int 0x30
done%+codebits%+_%1_%+opbits:
    hlt ; Returning here is a harness failure, never an intended test step.
%endif
%endmacro

%macro SINGLE 5
    ; CASE accepts a byte list. Single-byte instructions use a zero-length
    ; string as the second item, which NASM emits as no additional bytes.
    CASE %1, %2, %3, %4, %5, ''
%endmacro

%macro VARIANTS 0
%assign codebits 16
%rep 2
%assign user_cs UCODE16
%if codebits = 32
%assign user_cs UCODE32
%endif
bits codebits
%assign opbits 16
%rep 2
%assign mask 0xffff
%if opbits = 32
%assign mask 0xffffffff
%endif
%assign r 0
%rep 8
%assign val 0x11223344
%if r = 1
%assign val 0x22334455
%elif r = 2
%assign val 0x33445566
%elif r = 3
%assign val 0xa020
%elif r = 4
%assign val USTACK_TOP
%elif r = 5
%assign val 0x55667788
%elif r = 6
%assign val 0xa040
%elif r = 7
%assign val 0x778899aa
%endif
    SINGLE reg%+r, val, mask, VALUE_LITERAL, 0x50+r
    CASE rm%+r, val, mask, VALUE_LITERAL, 0xff, 0xf0+r
%assign r r+1
%endrep
    CASE imm8, -7, mask, VALUE_LITERAL, 0x6a, 0xf9
%if opbits = 16
    CASE imm, 0x5678, mask, VALUE_LITERAL, 0x68, 0x78,0x56
%else
    CASE imm, 0x12345678, mask, VALUE_LITERAL, 0x68, 0x78,0x56,0x34,0x12
%endif
%if codebits = 16
    CASE mem16, 0x89abcdef, mask, VALUE_LITERAL, 0xff, 0x37 ; [BX]
    CASE mem32, 0x76543210, mask, VALUE_LITERAL, 0x67, 0xff,0x36 ; [ESI]
%else
    CASE mem16, 0x89abcdef, mask, VALUE_LITERAL, 0x67, 0xff,0x37
    CASE mem32, 0x76543210, mask, VALUE_LITERAL, 0xff, 0x36
%endif
    ; Only the selector's defined low 16 bits are asserted for PUSH Sreg.
    SINGLE es, UDATA, 0xffff, VALUE_LITERAL, 0x06
    SINGLE cs, 0, 0xffff, VALUE_CS, 0x0e
    SINGLE ss, 0, 0xffff, VALUE_SS, 0x16
    SINGLE ds, UDATA, 0xffff, VALUE_LITERAL, 0x1e
    CASE fs, UDATA, 0xffff, VALUE_LITERAL, 0x0f, 0xa0
    CASE gs, UDATA, 0xffff, VALUE_LITERAL, 0x0f, 0xa8
    SINGLE flags, FLAGS_TEST, mask, VALUE_LITERAL, 0x9c
%assign opbits 32
%endrep
%assign codebits 32
%endrep
%endmacro

%define EMIT_TABLE 0
VARIANTS
%define EMIT_TABLE 1
align 4
cases:
VARIANTS
cases_end:
%if (cases_end-cases)/ROW_BYTES != 108
%error "Update the runner/pass count when adding PUSH cases"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC ROM, 0xffff, 0xfa, 0
    DESC ROM, 0xffff, 0xfa, 0x40
    DESC 0, 0xfffff, 0xf2, 0xc0
    DESC 0, 0xffff, 0xf2, 0
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
