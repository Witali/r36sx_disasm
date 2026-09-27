; Standalone 64 KiB ROM at F0000h, Intel 80386 PRM CALL/JMP and 9.8.6.
; AMD APM vol.3 rev.3.19 CALL (Far)/JMP (Far) confirms register-form #UD.
; Execute same-ring far transfers at CPL3 with real segmentation and paging.
; Invalid FF /3,/5 encodings must fault before touching a pointer or stack.
cpu 386
bits 16
org 0

%define ROM 0xf0000
%define PD 0x1000
%define PT 0x2000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define USTACK_TOP 0x6100
%define TSS 0x7000
%define KSTACK 0x9000
%define POINTER 0xa010
%define CODEBUF 0x10000
%define TARGETBUF 0x12000
%define TARGET_IP 0x100
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define UDATA (40|3)
%define USTACK16 (48|3)
%define USTACK32 (56|3)
%define TASK 64
%define TARGET16 (72|3)
%define TARGET32 (80|3)
%define FLAGS_TEST 0x8d7
%define FLAGS_MASK 0xed5 ; Arithmetic flags, IF and DF; fault RF is separate.
%define CR2_SENTINEL 0x13579bdf

%define COUNT STATE
%define ROW STATE+4
%define CONTEXT STATE+8
%define EXPECT_VEC STATE+12
%define EXPECT_ERR STATE+16
%define EXPECT_CR2 STATE+20
%define EXPECT_ESP STATE+24
%define EXPECT_CS STATE+28
%define EXPECT_IP STATE+32
%define SOURCE_CS STATE+36
%define RETURN_IP STATE+40
%define EXPECT_DATA STATE+44
%define EXPECT_SS STATE+48
%define GOT_VEC STATE+52
%define CHECK_ID STATE+56
%define GOT_VALUE STATE+60
%define POINTER_SHADOW STATE+128

%define ROW_CODEBITS 0
%define ROW_OPBITS 4
%define ROW_ADDRBITS 8
%define ROW_GROUP 12 ; FF /3 = CALL, /5 = JMP.
%define ROW_FORM 16 ; 0..7: invalid r/m register, 8: DS, 9: FS, 10: SS, 11: immediate.
%define ROW_TARGET 20
%define ROW_BYTES 24
%define ROW_COUNT 192
%define CONTEXT_COUNT 6 ; SS.B=0/1 x accessible/null-data/missing-pointer-page.
%define TOTAL_CASES (ROW_COUNT*CONTEXT_COUNT)

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1 >> 16) & 255, %3, ((%2 >> 16) & 15) | %4, (%1 >> 24) & 255
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
    mov edi, STATE
    xor eax, eax
    mov ecx, 48
    rep stosd
    mov word [0xb8000], 0x0746 ; Visible F without a BIOS dependency.
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
    mov dword [PT+8*4], 0x8003 ; Supervisor-only exception stack.
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
    mov ecx, 50
    mov eax, (unexpected-$$) | (KCODE<<16)
    mov edx, 0x8e00
.gate:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .gate
    mov word [IDT+6*8], ud_handler
    mov word [IDT+13*8], gp_handler
    mov word [IDT+14*8], pf_handler
    mov word [IDT+0x30*8], success_handler
    mov byte [IDT+0x30*8+5], 0xee
    mov byte [IDT+0x31*8+5], 0xee ; Fall-through must not count as a transfer.
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov dword [TARGETBUF+TARGET_IP], 0xf4f430cd ; INT 30h; HLT; HLT (both widths).
    mov dx, 0x190
    mov al, 0xee ; Disable instruction trace, retaining precise fault messages.
    out dx, al
context_begin:
    mov dword [ss:ROW], cases
case_begin:
    cld
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    mov eax, [CONTEXT]
    and eax, 1
    shl eax, 3
    add eax, USTACK16
    mov [EXPECT_SS], eax
    mov dword [PT+10*4], 0xa007
    mov eax, cr3
    mov cr3, eax
    mov edi, USTACK_TOP-32
    mov ecx, 12
    mov eax, 0xcccccccc
    rep stosd
    mov edi, POINTER
    mov ecx, 4
    rep stosd
    mov dword [POINTER], TARGET_IP
    mov eax, [cs:esi+ROW_OPBITS]
    shr eax, 3
    mov edx, [cs:esi+ROW_TARGET]
    mov [POINTER+eax], dx
    push esi
    mov esi, POINTER
    mov edi, POINTER_SHADOW
    mov ecx, 4
    rep movsd
    pop esi

    mov eax, UCODE16
    cmp dword [cs:esi+ROW_CODEBITS], 16
    je .source_cs
    mov eax, UCODE32
.source_cs:
    mov [SOURCE_CS], eax
    mov edi, CODEBUF
    mov eax, [cs:esi+ROW_CODEBITS]
    cmp eax, [cs:esi+ROW_OPBITS]
    je .address_prefix
    mov al, 0x66
    stosb
.address_prefix:
    mov eax, [cs:esi+ROW_CODEBITS]
    cmp eax, [cs:esi+ROW_ADDRBITS]
    je .segment_prefix
    mov al, 0x67
    stosb
.segment_prefix:
    mov ebx, [cs:esi+ROW_FORM]
    cmp ebx, 9
    jne .ss_prefix
    mov al, 0x64
    stosb
.ss_prefix:
    cmp ebx, 10
    jne .opcode
    mov al, 0x36
    stosb
.opcode:
    cmp ebx, 11
    je .direct
    mov al, 0xff
    stosb
    mov eax, [cs:esi+ROW_GROUP]
    shl eax, 3
    cmp ebx, 8
    jae .memory
    or eax, ebx
    or al, 0xc0 ; mod=11 is forbidden for a far memory pointer.
    stosb
    jmp .stub_done
.memory:
    cmp dword [cs:esi+ROW_ADDRBITS], 32
    je .memory32
    or al, 6
    stosb
    mov ax, POINTER
    stosw
    jmp .stub_done
.memory32:
    or al, 5
    stosb
    mov eax, POINTER
    stosd
    jmp .stub_done
.direct:
    mov al, 0x9a
    cmp dword [cs:esi+ROW_GROUP], 3
    je .direct_opcode
    mov al, 0xea
.direct_opcode:
    stosb
    mov eax, TARGET_IP
    cmp dword [cs:esi+ROW_OPBITS], 32
    je .direct32
    stosw
    jmp .selector
.direct32:
    stosd
.selector:
    mov ax, [cs:esi+ROW_TARGET]
    stosw
.stub_done:
    mov eax, edi
    sub eax, CODEBUF
    mov [RETURN_IP], eax
    mov ax, 0x31cd ; Any fall-through is failure, not a completed far transfer.
    stosw
    mov byte [edi], 0xf4

    mov dword [EXPECT_VEC], 0x30
    mov dword [EXPECT_ERR], 0
    mov dword [EXPECT_CR2], CR2_SENTINEL
    mov dword [EXPECT_DATA], UDATA
    mov eax, [CONTEXT]
    shr eax, 1
    cmp eax, 1
    jne .missing_page
    mov dword [EXPECT_DATA], 0
    cmp ebx, 8
    jb .encoding
    cmp ebx, 9
    ja .encoding ; SS and immediate forms do not depend on DS/FS.
    mov dword [EXPECT_VEC], 13
    jmp .encoding
.missing_page:
    cmp eax, 2
    jne .encoding
    mov dword [PT+10*4], 0
    cmp ebx, 8
    jb .encoding
    cmp ebx, 10
    ja .encoding
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 4 ; User read from the missing pointer page.
    mov dword [EXPECT_CR2], POINTER
.encoding:
    cmp ebx, 8
    jae .expected_frame
    mov dword [EXPECT_VEC], 6 ; #UD regardless of data-selector/page state.
.expected_frame:
    mov dword [EXPECT_ESP], USTACK_TOP
    mov dword [EXPECT_IP], 0
    mov eax, [SOURCE_CS]
    mov [EXPECT_CS], eax
    cmp dword [EXPECT_VEC], 0x30
    jne .enter
    mov eax, [cs:esi+ROW_TARGET]
    mov [EXPECT_CS], eax
    mov dword [EXPECT_IP], TARGET_IP+2
    cmp dword [cs:esi+ROW_GROUP], 3
    jne .enter
    mov eax, [cs:esi+ROW_OPBITS]
    shr eax, 2 ; CALL pushes two operand-sized slots, irrespective of SS.B.
    sub [EXPECT_ESP], eax
.enter:
    mov eax, cr3
    mov cr3, eax
    mov eax, CR2_SENTINEL
    mov cr2, eax
    push dword [EXPECT_SS]
    push dword USTACK_TOP
    push dword FLAGS_TEST
    push dword [SOURCE_CS]
    push dword 0
    mov ax, [EXPECT_DATA]
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

success_handler:
    push dword 0
    mov dword [ss:GOT_VEC], 0x30
    jmp check_frame
ud_handler:
    push dword 0 ; #UD, unlike #GP and #PF, has no hardware error code.
    mov dword [ss:GOT_VEC], 6
    jmp check_frame
gp_handler:
    mov dword [ss:GOT_VEC], 13
    jmp check_frame
pf_handler:
    mov dword [ss:GOT_VEC], 14
check_frame:
    pushad
    mov dword [ss:CHECK_ID], 1
    mov eax, [ss:GOT_VEC]
    mov [ss:GOT_VALUE], eax
    cmp eax, [ss:EXPECT_VEC]
    jne unexpected
    mov eax, [ss:esp+32]
    cmp eax, [ss:EXPECT_ERR]
    jne unexpected
    mov dword [ss:CHECK_ID], 2
    xor edi, edi
.reg:
    mov eax, [cs:original_regs+edi*4]
    mov edx, 7
    sub edx, edi
    mov edx, [ss:esp+edx*4]
    cmp edi, 4
    jne .compare_reg
    mov eax, [ss:EXPECT_ESP]
    mov edx, [ss:esp+48]
.compare_reg:
    mov [ss:GOT_VALUE], edx
    cmp edx, eax
    jne unexpected
    inc edi
    cmp edi, 8
    jb .reg
    mov dword [ss:CHECK_ID], 3
    mov eax, [ss:esp+40]
    and eax, 0xffff
    cmp eax, [ss:EXPECT_CS]
    jne unexpected
    mov eax, [ss:esp+52]
    and eax, 0xffff
    cmp eax, [ss:EXPECT_SS]
    jne unexpected
    mov eax, [ss:esp+44]
    xor eax, FLAGS_TEST
    test eax, FLAGS_MASK
    jnz unexpected
    mov ax, ds
    cmp ax, [ss:EXPECT_DATA]
    jne unexpected
    mov ax, es
    cmp ax, [ss:EXPECT_DATA]
    jne unexpected
    mov ax, fs
    cmp ax, [ss:EXPECT_DATA]
    jne unexpected
    mov ax, gs
    cmp ax, [ss:EXPECT_DATA]
    jne unexpected
    mov dword [ss:CHECK_ID], 4
    mov eax, [ss:esp+36]
    mov [ss:GOT_VALUE], eax
    cmp eax, [ss:EXPECT_IP]
    jne unexpected
    mov dword [ss:CHECK_ID], 5
    mov eax, cr2
    cmp eax, [ss:EXPECT_CR2]
    jne unexpected

    ; Check the user stack byte-by-byte, ignoring only the unspecified upper
    ; half of a 32-bit CS slot. Faults/JMP may not leave any CALL frame behind.
    mov dword [ss:CHECK_ID], 6
    mov esi, [ss:ROW]
    mov edi, USTACK_TOP-32
.stack:
    mov al, 0xcc
    cmp dword [ss:EXPECT_VEC], 0x30
    jne .stack_compare
    cmp dword [cs:esi+ROW_GROUP], 3
    jne .stack_compare
    mov ecx, edi
    sub ecx, [ss:EXPECT_ESP]
    mov ebp, [cs:esi+ROW_OPBITS]
    shr ebp, 3
    cmp ecx, ebp
    jae .cs_slot
    mov eax, [ss:RETURN_IP]
    shl ecx, 3
    shr eax, cl
    jmp .stack_compare
.cs_slot:
    sub ecx, ebp
    cmp ecx, ebp
    jae .stack_compare
    cmp ecx, 2
    jae .stack_next
    mov eax, [ss:SOURCE_CS]
    shl ecx, 3
    shr eax, cl
.stack_compare:
    cmp [ss:edi], al
    jne unexpected
.stack_next:
    inc edi
    cmp edi, USTACK_TOP+16
    jb .stack
    mov dword [ss:CHECK_ID], 7
    mov dword [ss:PT+10*4], 0xa007
    mov eax, cr3
    mov cr3, eax
    xor edi, edi
.pointer:
    mov al, [ss:POINTER+edi]
    cmp al, [ss:POINTER_SHADOW+edi]
    jne unexpected
    inc edi
    cmp edi, 16
    jb .pointer
    mov esp, KSTACK
    inc dword [ss:COUNT]
    add dword [ss:ROW], ROW_BYTES
    cmp dword [ss:ROW], cases_end
    jb case_begin
    inc dword [ss:CONTEXT]
    cmp dword [ss:CONTEXT], CONTEXT_COUNT
    jb context_begin
    mov dword [ss:CHECK_ID], 8
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
    mov esi, value_text
    call print
    mov eax, [ss:GOT_VALUE]
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
passed: db 'CPU386 FAR PASS cases=1152',10,0
failed: db 'CPU386 FAR FAIL case=',0
check_text: db ' check=',0
value_text: db ' value=',0
original_regs:
    dd 0x11223344,0x22334455,0x33445566,0x44556677
    dd USTACK_TOP,0x55667788,0x66778899,0x778899aa

align 4
cases:
%assign codebits 16
%rep 2
%assign target TARGET32
%if codebits=32
%assign target TARGET16
%endif
%assign opbits 16
%rep 2
%assign addrbits 16
%rep 2
%assign group 3
%rep 2
%assign form 0
%rep 12
    dd codebits,opbits,addrbits,group,form,target
%assign form form+1
%endrep
%assign group 5
%endrep
%assign addrbits 32
%endrep
%assign opbits 32
%endrep
%assign codebits 32
%endrep
cases_end:
%if (cases_end-cases)/ROW_BYTES != ROW_COUNT
%error "Update FAR coverage count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC 0, 0xffff, 0xf2, 0
    DESC 0, 0xffff, 0xf2, 0
    DESC 0, 0xffff, 0xf2, 0x40
    DESC TSS, 103, 0x89, 0
    DESC TARGETBUF, 0xfff, 0xfa, 0
    DESC TARGETBUF, 0xfff, 0xfa, 0x40
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
    dd GDT
idt_ptr: dw 50*8-1
    dd IDT

bits 16
times 0xfff0-($-$$) db 0xff
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
