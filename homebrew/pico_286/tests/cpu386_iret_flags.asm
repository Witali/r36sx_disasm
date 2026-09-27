; Intel 80386 IRET, Intel SDM vol.2A RETURN-TO-OUTER-PRIVILEGE-LEVEL,
; AMD APM v3 IRETx: IF/IOPL write permission uses OLD CPL and OLD IOPL.
; RETF enters the source level so the test setup does not depend on IRET.
cpu 386
bits 16
org 0

%define ROM 0xf0000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define TSS 0x7000
%define KSTACK 0x9000
%define CODEBUF 0x10000
%define SOURCE_STACK 0x20000
%define TARGET_STACK 0x40000
%define START_SP 0x7000
%define TARGET_SP 0x9000
%define TARGET_IP 0x40
%define KCODE 8
%define KDATA 16
%define GLOBAL_DATA 24
%define SOURCE_CODE 32
%define DEST_CODE 40
%define SOURCE_SS 48
%define DEST_SS 56
%define TASK 64
%define FLAGS_MASK 0x3ed5
%define TOTAL_CASES 1280

%define COUNT STATE
%define ROW STATE+4
%define SCENARIO STATE+8
%define CHECK_ID STATE+12
%define GOT STATE+16
%define WANT STATE+20
%define OLD_FLAGS STATE+24
%define NEW_FLAGS STATE+28
%define EXPECTED_FLAGS STATE+32
%define EXPECTED_SP STATE+36
%define EXPECTED_SS STATE+40
%define EXPECTED_CS STATE+44
%define ENTRY_SS STATE+48
%define ENTRY_CS STATE+52
%define SAVED_FLAGS STATE+56
%define SAVED_SS STATE+60
%define SAVED_CS STATE+64
%define EXPECTED_REGS STATE+96
%define SAVED_REGS STATE+128
%define R_FROM 0
%define R_TO 4
%define R_WIDTH 8
%define ROW_BYTES 12
%define S_OLD_IOPL 0
%define S_NEW_IOPL 4
%define S_OLD_IF 8
%define S_NEW_IF 12
%define SCENARIO_BYTES 16

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
    mov word [0xb8000], 0x0749
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
    mov eax, (unexpected-$$)|(KCODE<<16)
    mov edx, 0x8e00
.idt:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .idt
    mov word [IDT+0x30*8], check_result
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    mov esi, target_code
    mov edi, CODEBUF+TARGET_IP
    mov ecx, target_code_end-target_code
    ; Source is ROM, destination is RAM, and the ranges do not overlap.
    cs rep movsb
    mov dword [ROW], rows
    mov dword [SCENARIO], scenarios
    mov dword [COUNT], 0
    mov dx, 0x190
    mov al, 0xee
    out dx, al

case_begin:
    cli
    cld
    mov ax, KDATA
    mov ss, ax
    mov esp, KSTACK
    mov ds, ax
    mov es, ax
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    mov ebp, [SCENARIO]
    mov eax, [cs:esi+R_FROM]
    mov edx, eax
    or eax, SOURCE_SS
    mov [ENTRY_SS], eax
    mov eax, edx
    or eax, SOURCE_CODE
    mov [ENTRY_CS], eax
    shl edx, 5
    mov eax, edx
    or al, 0x9a
    mov [GDT+SOURCE_CODE+5], al
    or dl, 0x92
    mov [GDT+SOURCE_SS+5], dl
    mov eax, [cs:esi+R_TO]
    mov edx, eax
    or eax, DEST_SS
    mov [EXPECTED_SS], eax
    mov eax, edx
    or eax, DEST_CODE
    mov [EXPECTED_CS], eax
    shl edx, 5
    mov eax, edx
    or al, 0x9a
    mov [GDT+DEST_CODE+5], al
    or dl, 0x92
    mov [GDT+DEST_SS+5], dl

    mov eax, [cs:ebp+S_OLD_IOPL]
    shl eax, 12
    mov edx, [cs:ebp+S_OLD_IF]
    shl edx, 9
    or eax, edx
    or eax, 0x802
    mov [OLD_FLAGS], eax
    mov eax, [cs:ebp+S_NEW_IOPL]
    shl eax, 12
    mov edx, [cs:ebp+S_NEW_IF]
    shl edx, 9
    or eax, edx
    or eax, 0x4d7 ; Different arithmetic flags/DF from the entry state.
    mov [NEW_FLAGS], eax
    ; Expected IF permission must not depend on the NEW privilege or IOPL.
    mov ecx, [cs:esi+R_FROM]
    cmp ecx, [cs:ebp+S_OLD_IOPL]
    jbe .iopl
    and eax, ~0x200
    mov edx, [OLD_FLAGS]
    and edx, 0x200
    or eax, edx
.iopl:
    test ecx, ecx
    jz .expected
    and eax, ~0x3000
    mov edx, [OLD_FLAGS]
    and edx, 0x3000
    or eax, edx
.expected:
    mov [EXPECTED_FLAGS], eax
    mov edi, SOURCE_STACK+START_SP
    mov eax, TARGET_IP
    call emit_frame_value
    mov eax, [EXPECTED_CS]
    call emit_frame_value
    mov eax, [NEW_FLAGS]
    call emit_frame_value
    mov eax, [cs:esi+R_FROM]
    cmp eax, [cs:esi+R_TO]
    je .same_level
    mov eax, TARGET_SP
    mov [EXPECTED_SP], eax
    call emit_frame_value
    mov eax, [EXPECTED_SS]
    call emit_frame_value
    jmp .opcode
.same_level:
    mov eax, edi
    sub eax, SOURCE_STACK
    mov [EXPECTED_SP], eax
    mov eax, [ENTRY_SS]
    mov [EXPECTED_SS], eax
.opcode:
    mov word [CODEBUF], 0xf4cf ; IRETD; forbidden fall-through HLT.
    cmp dword [cs:esi+R_WIDTH], 4
    je .registers
    mov word [CODEBUF], 0xcf66 ; IRET in a 32-bit code segment.
    mov byte [CODEBUF+2], 0xf4
.registers:
    mov dword [EXPECTED_REGS], 0x89abcdef
    mov dword [EXPECTED_REGS+4], 0x11223344
    mov dword [EXPECTED_REGS+8], 0x33445566
    mov dword [EXPECTED_REGS+12], 0x44556677
    mov eax, [EXPECTED_SP]
    mov [EXPECTED_REGS+16], eax
    mov dword [EXPECTED_REGS+20], 0x55667788
    mov dword [EXPECTED_REGS+24], 0x66778899
    mov dword [EXPECTED_REGS+28], 0x778899aa
    ; All levels can read the result area. RETF must preserve the flags set
    ; at CPL0, avoiding reliance on IRET to construct its own initial state.
    mov ax, GLOBAL_DATA|3
    mov ds, ax
    mov es, ax
    cmp dword [cs:esi+R_FROM], 0
    je .ring_zero
    push dword [ENTRY_SS]
    push dword START_SP
    jmp .entry_frame
.ring_zero:
    mov ax, SOURCE_SS
    mov ss, ax
    mov esp, START_SP
.entry_frame:
    push dword [ENTRY_CS]
    push dword 0
    push dword [OLD_FLAGS]
    popfd
    mov eax, 0x89abcdef
    mov ecx, 0x11223344
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    mov esi, 0x66778899
    mov edi, 0x778899aa
    retf

emit_frame_value:
    cmp dword [cs:esi+R_WIDTH], 4
    je .wide
    stosw
    ret
.wide:
    stosd
    ret

; Relocated to CODEBUF+40h. Save state before the INT gate clears IF or
; switches stacks. MOV, PUSHFD and POP m32 leave arithmetic flags unchanged.
target_code:
    mov [SAVED_REGS], eax
    mov [SAVED_REGS+4], ecx
    mov [SAVED_REGS+8], edx
    mov [SAVED_REGS+12], ebx
    mov [SAVED_REGS+16], esp
    mov [SAVED_REGS+20], ebp
    mov [SAVED_REGS+24], esi
    mov [SAVED_REGS+28], edi
    pushfd
    pop dword [SAVED_FLAGS]
    mov ax, ss
    mov [SAVED_SS], ax
    mov ax, cs
    mov [SAVED_CS], ax
    int 0x30
    hlt
target_code_end:

check_result:
    ; At CPL0 an INT does not switch stacks. Normalize SS explicitly before
    ; reading STATE; the guest context has already been saved by the stub.
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, KSTACK
    cld
    mov esi, [ROW]
    mov dword [CHECK_ID], 1
    mov eax, [SAVED_FLAGS]
    and eax, FLAGS_MASK
    mov edx, [EXPECTED_FLAGS]
    and edx, FLAGS_MASK
    call expect
    mov dword [CHECK_ID], 2
    xor edi, edi
.gpr:
    mov eax, [SAVED_REGS+edi*4]
    mov edx, [EXPECTED_REGS+edi*4]
    call expect
    inc edi
    cmp edi, 8
    jb .gpr
    mov dword [CHECK_ID], 3
    movzx eax, word [SAVED_SS]
    mov edx, [EXPECTED_SS]
    call expect
    movzx eax, word [SAVED_CS]
    mov edx, [EXPECTED_CS]
    call expect
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
expect:
    mov [GOT], eax
    mov [WANT], edx
    cmp eax, edx
    jne unexpected
    ret
unexpected:
    mov esi, failed
    call print
    mov eax, [COUNT]
    call hex32
    mov esi, check_text
    call print
    mov eax, [CHECK_ID]
    call hex32
    mov esi, got_text
    call print
    mov eax, [GOT]
    call hex32
    mov esi, want_text
    call print
    mov eax, [WANT]
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
passed: db 'CPU386 IRET FLAGS PASS cases=1280',10,0
failed: db 'CPU386 IRET FLAGS FAIL case=',0
check_text: db ' check=',0
got_text: db ' got=',0
want_text: db ' want=',0

align 4
rows:
%assign from 0
%rep 4
%assign to from
%rep (4-from)
    dd from,to,2
    dd from,to,4
%assign to to+1
%endrep
%assign from from+1
%endrep
rows_end:
scenarios:
%assign old_iopl 0
%rep 4
%assign new_iopl 0
%rep 4
%assign old_if 0
%rep 2
%assign new_if 0
%rep 2
    dd old_iopl,new_iopl,old_if,new_if
%assign new_if 1
%endrep
%assign old_if 1
%endrep
%assign new_iopl new_iopl+1
%endrep
%assign old_iopl old_iopl+1
%endrep
scenarios_end:
%if (rows_end-rows)/ROW_BYTES * (scenarios_end-scenarios)/SCENARIO_BYTES != TOTAL_CASES
%error "Update IRET case count and runner"
%endif
align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC 0, 0xfffff, 0xf2, 0xc0
    DESC CODEBUF, 0xfff, 0x9a, 0x40
    DESC CODEBUF, 0xfff, 0x9a, 0x40
    DESC SOURCE_STACK, 0xffff, 0x92, 0x40
    DESC TARGET_STACK, 0xffff, 0x92, 0x40
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
