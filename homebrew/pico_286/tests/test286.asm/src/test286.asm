; test286.asm
;
; Small R36SX/Pico-286 diagnostic BIOS ROM for checking Intel 80286-specific
; real-mode instructions and the basic raw protected-mode transition.
;
; It is intentionally much smaller than test386.asm: POST 80h gives the major
; stage, POST 190h gives the sub-stage, and text goes to port 191h.

cpu 286
bits 16
org 0

%define C_SEG_REAL 0xf000
%define S_SEG_REAL 0x1000
%define ESP_REAL   0xfffe

%define POST_PORT    0x80
%define SUBPOST_PORT 0x190
%define OUT_PORT     0x191

%define TMP_GDTR 0x0700
%define TMP_IDTR 0x0708
%define TMP_MSW  0x0710

%define CODE_SEL     0x08
%define DATA_SEL     0x10
%define STACK_SEL    0x18

%macro POST 1
    mov al, %1
    out POST_PORT, al
%endmacro

%macro SUBPOST 1
    mov dx, SUBPOST_PORT
    mov al, %1
    out dx, al
%endmacro

; Intel 80286 segment descriptor: limit16, base24, access byte, reserved word.
%macro DESC286 3
    dw (%2) & 0xffff
    dw (%1) & 0xffff
    db ((%1) >> 16) & 0xff
    db (%3) & 0xff
    dw 0
%endmacro

cpuTest:
    cli
    mov ax, C_SEG_REAL
    mov ds, ax
    mov es, ax
    mov ax, S_SEG_REAL
    mov ss, ax
    mov sp, ESP_REAL

    POST 0x00
    mov si, banner
    call print_string

    call real_mode_tests
    call enter_protected_mode

test_passed:
    mov si, passed
    call print_string_cs
    POST 0xff
halt_loop:
    cli
    hlt
    jmp halt_loop

test_failed:
    mov si, failed
    call print_string_cs
    POST 0xfe
    jmp halt_loop

print_string:
    lodsb
    test al, al
    jz .done
    mov dx, OUT_PORT
    out dx, al
    jmp print_string
.done:
    ret

print_string_cs:
    push ds
    push cs
    pop ds
    call print_string
    pop ds
    ret

real_mode_tests:
    POST 0x01

    ; 80286 PUSH SP pushes the original SP value.  8086 pushes the decremented
    ; value, so this catches accidental 8086 semantics in 286 mode.
    SUBPOST 0x10
    mov bp, sp
    push sp
    pop ax
    cmp ax, bp
    jne test_failed

    ; PUSHA/POPA are 80186+ instructions and must exist on a 286.
    SUBPOST 0x11
    mov ax, 0x1111
    mov bx, 0x2222
    mov cx, 0x3333
    mov dx, 0x4444
    mov si, 0x5555
    mov di, 0x6666
    pusha
    xor ax, ax
    xor bx, bx
    xor cx, cx
    xor dx, dx
    xor si, si
    xor di, di
    popa
    cmp ax, 0x1111
    jne test_failed
    cmp bx, 0x2222
    jne test_failed
    cmp cx, 0x3333
    jne test_failed
    cmp dx, 0x4444
    jne test_failed
    cmp si, 0x5555
    jne test_failed
    cmp di, 0x6666
    jne test_failed

    ; 286 masks variable shift counts to 5 bits, unlike the 8086.
    SUBPOST 0x12
    mov ax, 1
    mov cl, 33
    shl ax, cl
    cmp ax, 2
    jne test_failed

    ; IMUL r16,r/m16,imm8 and BOUND are available on 80186/80286.
    SUBPOST 0x13
    mov bx, -7
    imul ax, bx, byte 3
    cmp ax, -21
    jne test_failed
    mov ax, 5
    bound ax, [cs:bounds]

    ; SGDT/SIDT/SMSW are 286 system instructions.  In real mode we only verify
    ; that they execute and write to memory without corrupting DS.
    SUBPOST 0x14
    push ds
    xor ax, ax
    mov ds, ax
    sgdt [TMP_GDTR]
    sidt [TMP_IDTR]
    smsw [TMP_MSW]
    pop ds

    ret

enter_protected_mode:
    POST 0x08
    SUBPOST 0x80
    lgdt [gdt_ptr]

    smsw ax
    or ax, 1
    lmsw ax

    ; Intel requires a far control transfer after setting PE so CS is loaded
    ; from a protected-mode descriptor.
    jmp CODE_SEL:protected_entry

protected_entry:
    POST 0x09
    SUBPOST 0x90

    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov ax, STACK_SEL
    mov ss, ax
    mov sp, ESP_REAL

    smsw ax
    test ax, 1
    jz test_failed_pm

    ; LSL/LAR/VERR/VERW/ARPL are selector-aware 286 instructions.  They are the
    ; smallest useful smoke-test for descriptor visibility and access rights.
    SUBPOST 0x91
    mov ax, CODE_SEL
    lsl bx, ax
    jnz test_failed_pm
    cmp bx, 0xffff
    jne test_failed_pm

    SUBPOST 0x92
    mov ax, CODE_SEL
    lar bx, ax
    jnz test_failed_pm

    SUBPOST 0x93
    mov ax, CODE_SEL
    verr ax
    jnz test_failed_pm
    verw ax
    jz test_failed_pm

    SUBPOST 0x94
    mov ax, DATA_SEL
    verw ax
    jnz test_failed_pm

    SUBPOST 0x95
    mov ax, DATA_SEL
    mov bx, 3
    arpl ax, bx
    jnz test_failed_pm
    cmp ax, DATA_SEL | 3
    jne test_failed_pm

    jmp test_passed_pm

test_passed_pm:
    mov si, passed
    call print_string_cs
    POST 0xff
    jmp halt_loop_pm

test_failed_pm:
    mov si, failed
    call print_string_cs
    POST 0xfe

halt_loop_pm:
    cli
    hlt
    jmp halt_loop_pm

align 2
bounds:
    dw 0, 10

gdt_start:
    dq 0
    DESC286 0xf0000, 0xffff, 0x9a ; readable code, base F0000h
    DESC286 0x00000, 0xffff, 0x92 ; writable data, base 00000h
    DESC286 0x10000, 0xffff, 0x92 ; writable stack, base 10000h
gdt_end:

gdt_ptr:
    dw gdt_end - gdt_start - 1
    dd 0xf0000 + gdt_start

banner:
    db 'test286 R36SX', 13, 10, 0
passed:
    db 'PASS', 13, 10, 0
failed:
    db 'FAIL', 13, 10, 0

times 0xfff0-($-$$) nop

reset_vector:
    jmp C_SEG_REAL:cpuTest

release_date:
    db '06/05/26'
model_byte:
    db 0xfc
checksum:
    db 0

times 0x10000-($-$$) db 0
