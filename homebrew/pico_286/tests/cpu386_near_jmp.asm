; Intel 80386 PRM JMP, 9.1/9.8 and 12.3; AMD APM v3 rev.3.19 pp.185-186.
; A raw 64 KiB reset ROM: near-JMP width, target limits and fault provenance.
; CS.D sets defaults, not the width of EIP. Operand size truncates the target;
; address size only selects the indirect operand's addressing form.
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
%define POINTER 0xb010
%define CODEBUF 0x40000
%define STACKBASE 0x20000
%define UESP 0x6100
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define UDATA (40|3)
%define UFS (48|3)
%define UGS (56|3)
%define USTACK (64|3)
%define TASK 72
%define FLAGS_MASK 0x37fd7
%define CR2_SENTINEL 0x13579bdf

%define COUNT STATE
%define ROW STATE+4
%define CONTEXT STATE+8
%define ENTRY_IP STATE+12
%define TARGET_IP STATE+16
%define CODE_LIMIT STATE+20
%define CODE_SEL STATE+24
%define EXPECT_VEC STATE+28
%define EXPECT_ERR STATE+32
%define EXPECT_CR2 STATE+36
%define EXPECT_IP STATE+40
%define EXPECT_FLAGS STATE+44
%define INPUT_FLAGS STATE+48
%define EXPECT_DS STATE+52
%define EXPECT_FS STATE+56
%define EXPECT_GS STATE+60
%define PTR_LINEAR STATE+64
%define TRAP_STEP STATE+68
%define GOT_VEC STATE+72
%define CHECK_ID STATE+76
%define GOT STATE+80
%define WANT STATE+84
%define TRAP_COUNT STATE+88
%define FAULT_COUNT STATE+92
%define GPRS STATE+128 ; PUSHAD order, except slot 3 is the guest ESP.

%define R_CODE 0
%define R_OPERAND 4
%define R_ADDRESS 8
%define R_FORM 12 ; 0=rel8, 1=rel16/32, 2..9=GPR, 10..13=DS/FS/GS/SS.
%define R_SIZE 16
%define ROW_COUNT 112
%define CONTEXT_COUNT 28 ; Fourteen scenarios, each with two distinct flags images.
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
    xor eax, eax
    mov edi, STATE
    mov ecx, 64
    rep stosd
    mov word [0xb8000], 0x074a ; Visible J without invoking a BIOS.
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
    mov eax, (unexpected-$$) | (KCODE<<16)
    mov edx, 0x8e00
.gate:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .gate
    mov word [IDT+1*8], db_handler
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
    mov eax, 0x31cd31cd ; Unexpected execution must fail, never silently pass.
    mov ecx, 0x20000/4
    rep stosd
    mov dx, 0x190
    mov al, 0xee
    out dx, al
context_begin:
    mov dword [ss:ROW], cases
case_begin:
    cld
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov esi, [ROW]
    mov dword [TRAP_STEP], 0
    ; Restore all pages removed by the previous scenario before fixture writes.
    mov edi, PT
    mov eax, 7
    mov ecx, 256
.restore_pages:
    stosd
    add eax, 0x1000
    loop .restore_pages
    mov dword [PT+8*4], 0x8003
    mov eax, cr3
    mov cr3, eax
    mov edi, STACKBASE+UESP-16
    mov eax, 0xcccccccc
    mov ecx, 8
    rep stosd
    mov edi, GPRS
    push esi
    mov esi, init_gprs
    mov ecx, 8
    cs rep movsd
    pop esi
    mov dword [EXPECT_DS], UDATA
    mov dword [EXPECT_FS], UFS
    mov dword [EXPECT_GS], UGS
    mov dword [EXPECT_VEC], 1
    mov dword [EXPECT_ERR], 0
    mov dword [EXPECT_CR2], CR2_SENTINEL
    mov dword [ENTRY_IP], 0x1000
    mov dword [TARGET_IP], 0x1040
    mov dword [CODE_LIMIT], 0x1ffff
    mov eax, [CONTEXT]
    and eax, 1
    mov edx, 0x102 ; TF, reserved bit 1; IF remains clear throughout the test.
    test eax, eax
    jz .flags
    mov edx, 0xdd7 ; Arithmetic flags and DF set, TF set, IF clear.
.flags:
    mov [INPUT_FLAGS], edx
    mov eax, [CONTEXT]
    shr eax, 1
    cmp eax, 1
    jne .high
    mov dword [ENTRY_IP], 0x1040
    mov dword [TARGET_IP], 0x1000
.high:
    cmp eax, 2
    jne .cross
    mov dword [ENTRY_IP], 0x11000
    mov dword [TARGET_IP], 0x11040
.cross:
    cmp eax, 3
    jne .underflow
    mov dword [ENTRY_IP], 0xfff0
    mov dword [TARGET_IP], 0x10030
.underflow:
    cmp eax, 4
    jne .equal
    mov dword [ENTRY_IP], 0x20
    mov dword [TARGET_IP], 0xfffffff0
.equal:
    cmp eax, 5
    jne .beyond
    mov dword [CODE_LIMIT], 0x1040
.beyond:
    cmp eax, 6
    jne .target_page
    mov dword [CODE_LIMIT], 0x103f
.target_page:
    cmp eax, 9
    jne .prefixes
    ; Different source/target pages, even after 16-bit target truncation.
    mov dword [ENTRY_IP], 0xffd0
    mov dword [TARGET_IP], 0x10040
    and dword [INPUT_FLAGS], ~0x100 ; Observe target-fetch #PF, not a prior #DB.
.prefixes:
    cmp eax, 12
    jne .last16
    mov dword [ENTRY_IP], 0x11000
    mov dword [TARGET_IP], 0x11040
    mov dword [CODE_LIMIT], 0x1103f ; Fault IP itself is above 64 KiB.
.last16:
    cmp eax, 13
    jne .code_width
    mov dword [ENTRY_IP], 0xffb0
    mov dword [TARGET_IP], 0xffff ; NOP advancement must not wrap at 64 KiB.
.code_width:
    mov eax, UCODE16
    cmp dword [cs:esi+R_CODE], 16
    je .selector
    mov eax, UCODE32
.selector:
    mov [CODE_SEL], eax
    mov ebx, [CODE_LIMIT]
    and eax, ~7
    mov [GDT+eax], bx
    shr ebx, 16
    and byte [GDT+eax+6], 0xf0
    or [GDT+eax+6], bl
    mov edi, [ENTRY_IP]
    add edi, CODEBUF
    mov eax, [cs:esi+R_CODE]
    cmp eax, [cs:esi+R_OPERAND]
    je .address
    mov al, 0x66
    stosb
.address:
    mov eax, [cs:esi+R_CODE]
    cmp eax, [cs:esi+R_ADDRESS]
    je .segment
    mov al, 0x67
    stosb
.segment:
    mov ebx, [cs:esi+R_FORM]
    mov dword [PTR_LINEAR], POINTER
    cmp ebx, 11
    jne .gs
    mov al, 0x64
    stosb
    add dword [PTR_LINEAR], 0x10000
.gs:
    cmp ebx, 12
    jne .ss
    mov al, 0x65
    stosb
    add dword [PTR_LINEAR], 0x30000
.ss:
    cmp ebx, 13
    jne .encode
    mov al, 0x36
    stosb
    add dword [PTR_LINEAR], STACKBASE
.encode:
    cmp ebx, 2
    jae .indirect
    mov al, 0xeb
    cmp ebx, 0
    je .rel_opcode
    mov al, 0xe9
.rel_opcode:
    stosb
    mov ecx, 1
    cmp ebx, 0
    je .displacement
    mov ecx, [cs:esi+R_OPERAND]
    shr ecx, 3
.displacement:
    ; These two contexts explicitly exercise both signed rel8 endpoints.
    mov edx, edi
    sub edx, CODEBUF
    add edx, ecx
    mov eax, [CONTEXT]
    shr eax, 1
    cmp eax, 7
    jne .minus128
    lea eax, [edx+127]
    mov [TARGET_IP], eax
.minus128:
    cmp dword [CONTEXT], 16
    jb .rel_value
    cmp dword [CONTEXT], 17
    ja .rel_value
    lea eax, [edx-128]
    mov [TARGET_IP], eax
.rel_value:
    mov eax, [TARGET_IP]
    sub eax, edx
    cmp ecx, 1
    jne .rel_word
    stosb
    jmp .encoded
.rel_word:
    cmp ecx, 2
    jne .rel_dword
    stosw
    jmp .encoded
.rel_dword:
    stosd
    jmp .encoded
.indirect:
    mov al, 0xff
    stosb
    cmp ebx, 10
    jae .memory
    lea eax, [ebx-2+0xe0] ; FF /4, mod=11 and each general register.
    stosb
    mov eax, [TARGET_IP]
    cmp dword [cs:esi+R_OPERAND], 32
    je .reg_value
    and eax, 0xffff
    or eax, 0x76540000 ; JMP r16 must not preserve these upper target bits.
.reg_value:
    mov ecx, 9
    sub ecx, ebx ; Convert ModRM register order to PUSHAD order.
    mov [GPRS+ecx*4], eax
    jmp .encoded
.memory:
    cmp dword [cs:esi+R_ADDRESS], 32
    je .memory32
    mov al, 0x26
    stosb
    mov ax, POINTER
    stosw
    jmp .encoded
.memory32:
    mov al, 0x25
    stosb
    mov eax, POINTER
    stosd
.encoded:
    mov word [edi], 0x31cd ; No fall-through can count as success.
    mov eax, [TARGET_IP]
    cmp dword [cs:esi+R_OPERAND], 32
    je .target_width
    and eax, 0xffff
.target_width:
    mov [TARGET_IP], eax
    ; Poison every unselected segment's pointer, rather than leaving a previous
    ; row's identical target there and accidentally accepting an ignored prefix.
    mov dword [POINTER], 0xbad0abcd
    mov dword [POINTER+0x10000], 0xbad1abcd
    mov dword [POINTER+0x20000], 0xbad2abcd
    mov dword [POINTER+0x30000], 0xbad3abcd
    mov edx, [PTR_LINEAR]
    mov [edx], eax
    mov dword [edx-4], 0x13572468
    mov dword [edx+4], 0xabcdef01
    mov [EXPECT_IP], eax
    cmp eax, [CODE_LIMIT]
    jbe .nop
    mov dword [EXPECT_VEC], 13
    jmp .scenarios
.nop:
    mov byte [CODEBUF+eax], 0x90 ; Check execution/advancement at the exact target.
.scenarios:
    mov eax, [CONTEXT]
    shr eax, 1
    cmp eax, 9
    jne .pointer_fault
    mov eax, [TARGET_IP]
    add eax, CODEBUF
    mov [EXPECT_CR2], eax
    shr eax, 12
    mov dword [PT+eax*4], 0
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 4
    jmp .expected
.pointer_fault:
    cmp eax, 10
    jne .null_data
    cmp ebx, 10
    jb .expected
    mov eax, [PTR_LINEAR]
    mov [EXPECT_CR2], eax
    shr eax, 12
    mov dword [PT+eax*4], 0
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 4
    jmp .fault_ip
.null_data:
    cmp eax, 11
    jne .expected
    mov dword [EXPECT_DS], 0
    mov dword [EXPECT_FS], 0
    mov dword [EXPECT_GS], 0
    cmp ebx, 10
    jb .expected
    cmp ebx, 13
    je .expected ; Explicit SS still works with null DS/FS/GS.
    mov dword [EXPECT_VEC], 13
.expected:
    cmp dword [EXPECT_VEC], 13
    jne .flags_expected
.fault_ip:
    mov eax, [ENTRY_IP]
    mov [EXPECT_IP], eax
.flags_expected:
    mov eax, [INPUT_FLAGS]
    cmp dword [EXPECT_VEC], 1
    je .save_flags
    or eax, 0x10000 ; Intel fault frame RF, not a single-step trap.
.save_flags:
    mov [EXPECT_FLAGS], eax
%ifdef NEAR_JMP_BAD_ORACLE
    xor dword [EXPECT_IP], 1
%endif
    mov eax, cr3
    mov cr3, eax
    mov eax, CR2_SENTINEL
    mov cr2, eax
    xor eax, eax
    mov dr6, eax
    push dword USTACK
    push dword [GPRS+12]
    push dword [INPUT_FLAGS]
    push dword [CODE_SEL]
    push dword [ENTRY_IP]
    mov ax, [EXPECT_DS]
    mov ds, ax
    mov es, ax
    mov ax, [ss:EXPECT_FS]
    mov fs, ax
    mov ax, [ss:EXPECT_GS]
    mov gs, ax
    mov edi, [ss:GPRS]
    mov esi, [ss:GPRS+4]
    mov ebp, [ss:GPRS+8]
    mov ebx, [ss:GPRS+16]
    mov edx, [ss:GPRS+20]
    mov ecx, [ss:GPRS+24]
    mov eax, [ss:GPRS+28]
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
    ; SS stays flat at ring 0 even when the guest has null data selectors.
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
    mov edx, [ss:GPRS+ecx*4]
    call equal
.skip_sp:
    inc ecx
    cmp ecx, 8
    jb .gpr
    mov eax, [ss:ebp+48]
    mov edx, [ss:GPRS+12]
    call equal
    mov eax, [ss:ebp+52]
    mov edx, USTACK
    call equal
    mov dword [ss:CHECK_ID], 5
    xor eax, eax
    mov ax, ds
    mov edx, [ss:EXPECT_DS]
    call equal
    mov ax, es
    call equal
    mov ax, fs
    mov edx, [ss:EXPECT_FS]
    call equal
    mov ax, gs
    mov edx, [ss:EXPECT_GS]
    call equal
    ; Restore a removed pointer page before checking that JMP never wrote it.
    mov edi, [ss:PTR_LINEAR]
    mov eax, edi
    and eax, ~0xfff
    mov edx, eax
    shr edx, 12
    or eax, 7
    mov [ss:PT+edx*4], eax
    mov eax, cr3
    mov cr3, eax
    mov dword [ss:CHECK_ID], 6
    mov eax, [ss:edi]
    mov edx, [ss:TARGET_IP]
    call equal
    mov eax, [ss:edi-4]
    mov edx, 0x13572468
    call equal
    mov eax, [ss:edi+4]
    mov edx, 0xabcdef01
    call equal
    mov edi, STACKBASE+UESP-16
    mov ecx, 8
.stack:
    mov eax, [ss:edi]
    mov edx, 0xcccccccc
    call equal
    add edi, 4
    loop .stack
    cmp dword [ss:GOT_VEC], 1
    jne .fault_complete
    inc dword [ss:TRAP_COUNT]
    mov eax, dr6
    and eax, 0x400f
    mov edx, 0x4000
    call equal
    inc dword [ss:TRAP_STEP]
    cmp dword [ss:TRAP_STEP], 1
    jne .next
    ; Resume once at the destination. A second #DB must follow its NOP,
    ; including CS.D=0 targets above 64 KiB and an instruction at CS.limit.
    inc dword [ss:EXPECT_IP]
    mov ax, [ss:EXPECT_DS]
    mov es, ax
    popad
    add esp, 4
    iretd
.fault_complete:
    inc dword [ss:FAULT_COUNT]
.next:
    mov esp, KSTACK
    inc dword [ss:COUNT]
    add dword [ss:ROW], R_SIZE
    cmp dword [ss:ROW], cases_end
    jb case_begin
    inc dword [ss:CONTEXT]
    cmp dword [ss:CONTEXT], CONTEXT_COUNT
    jb context_begin
    mov dword [ss:CHECK_ID], 7
    mov eax, [ss:COUNT]
    mov edx, TOTAL_CASES
    call equal
    mov eax, [ss:TRAP_COUNT]
    mov edx, 4704
    call equal
    mov eax, [ss:FAULT_COUNT]
    mov edx, 784
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
    cld
    mov esi, fail_msg
    call print
    mov eax, [ss:COUNT]
    call hex32
    mov esi, check_msg
    call print
    mov eax, [ss:CHECK_ID]
    call hex32
    mov esi, step_msg
    call print
    mov eax, [ss:TRAP_STEP]
    call hex32
    mov esi, got_msg
    call print
    mov eax, [ss:GOT]
    call hex32
    mov esi, want_msg
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
unexpected:
    mov dword [ss:CHECK_ID], 8
    mov eax, [ss:esp]
    xor edx, edx
    jmp failure
print:
    ; Handler entry preserves guest DF. Logging must not depend on its value.
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
cases:
%assign cb 16
%rep 2
%assign ob 16
%rep 2
%assign ab 16
%rep 2
%assign form 0
%rep 14
    dd cb, ob, ab, form
%assign form form+1
%endrep
%assign ab ab+16
%endrep
%assign ob ob+16
%endrep
%assign cb cb+16
%endrep
cases_end:
%if (cases_end-cases) != ROW_COUNT*R_SIZE
    %error Bad row count
%endif
align 8
descriptors:
    dq 0
    DESC ROM,0xffff,0x9a,0x40
    DESC 0,0xfffff,0x92,0xc0
    DESC CODEBUF,0x1ffff,0xfa,0
    DESC CODEBUF,0x1ffff,0xfa,0x40
    DESC 0,0xfffff,0xf2,0xc0
    DESC 0x10000,0xfffff,0xf2,0xc0
    DESC 0x30000,0xfffff,0xf2,0xc0
    DESC STACKBASE,0xfffff,0xf2,0xc0
    DESC TSS,103,0x89,0
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
         dd GDT
idt_ptr: dw 50*8-1
         dd IDT
digits: db '0123456789ABCDEF'
pass_msg: db 'CPU386 NEAR JMP PASS cases=3136',10,0
fail_msg: db 'CPU386 NEAR JMP FAIL case=',0
check_msg: db ' check=',0
step_msg: db ' step=',0
got_msg: db ' got=',0
want_msg: db ' want=',0
times 0xfff0-($-$$) db 0xff
bits 16
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
