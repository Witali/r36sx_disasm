; Standalone 64 KiB ROM at F0000h. Intel 80386 PRM LEA and 17.2 tables;
; AMD APM vol.3 rev.3.19 pp.195-196 (legacy modes, not AMD64).
; A generated addressing table is independent of the emulator's decoder.
; Each case copies one LEA into RAM, selects operand/code width and ModRM.reg,
; then executes at CPL3. Only the register destination may change; no operand
; memory reference is permitted, including through null or short segments.
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
%define CODEBUF 0x10000
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define UDATA (40|3)
%define USTACK (48|3)
%define TASK 56
%define CR2_SENTINEL 0x13579bdf
%define FLAGS_SET 0x0cd7
%define FLAGS_MASK 0x0ed5 ; Arithmetic flags, DF and IF; ignore fault RF.

%define COUNT STATE
%define CURRENT_ROW STATE+4
%define WIDTHS STATE+8
%define DEST STATE+12
%define PREFIX_INDEX STATE+16
%define NEXT_IP STATE+20
%define EXPECT_CS STATE+24
%define EXPECT_DATA STATE+28
%define EXPECT_FLAGS STATE+32
%define EXPECT_VALUE STATE+36
%define GOT_VEC STATE+40
%define CHECK_ID STATE+44
%define EXPECT_VEC STATE+48
%define GOT_VALUE STATE+52

%define ROW_EA 0
%define ROW_LENGTH 4
%define ROW_ADDRBITS 5
%define ROW_BAD 6
%define ROW_CODE 8
%define ROW_BYTES 16
%define ROW_COUNT 829 ; 24 addr16 + 789 addr32 + 16 invalid register forms.
%define TOTAL_CASES (ROW_COUNT*4*8)

; Distinct values exercise high halves, 16/32-bit overflow and signed disp8.
; The user SS base is deliberately unmapped; interrupt entry uses TSS ESP0.
%define R0 0xfedc7654
%define R1 0x87654321
%define R2 0x80010080
%define R3 0x10203040
%define R4 0x6100
%define R5 0xfffff800
%define R6 0x7fff9000
%define R7 0x456789ab

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
    mov ecx, 16
    rep stosd
    mov word [0xb8000], 0x074c ; Visible L, without a video BIOS dependency.
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
    mov dword [PT+8*4], 0x8003 ; Only CPL0 can access the handler stack.
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
    mov word [IDT+6*8], ud_handler
    mov word [IDT+0x30*8], success_handler
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov dx, 0x190
    mov al, 0xee ; Disable verbose per-instruction tracing for the large matrix.
    out dx, al
    mov dword [CURRENT_ROW], cases

case_begin:
    cld
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov dword [CHECK_ID], 0
    mov esi, [CURRENT_ROW]
    mov eax, [WIDTHS]
    shr eax, 1 ; bit1 chooses CS.D; bit0 chooses operand size.
    shl eax, 3
    add eax, UCODE16
    mov [EXPECT_CS], eax
    mov edi, CODEBUF
    mov eax, [WIDTHS]
    shr eax, 1
    xor eax, [WIDTHS]
    test eax, 1
    jz .address_prefix
    mov al, 0x66
    stosb
.address_prefix:
    mov eax, [WIDTHS]
    shr eax, 1
    shl eax, 4
    add eax, 16
    cmp al, [cs:esi+ROW_ADDRBITS]
    je .segment_prefix
    mov al, 0x67
    stosb
.segment_prefix:
    ; Rotate all six overrides plus no override. They affect neither the
    ; offset nor access checks. CS/SS have nonzero bases; data is null/short.
    mov ebx, [PREFIX_INDEX]
    mov al, [cs:prefixes+ebx]
    test al, al
    jz .opcode
    stosb
.opcode:
    mov ebp, edi
    movzx ecx, byte [cs:esi+ROW_LENGTH]
    add esi, ROM+ROW_CODE
    rep movsb
    mov eax, [DEST]
    shl eax, 3
    or [ebp+1], al ; Patch only ModRM.reg, leaving the EA form intact.
    mov ax, 0x30cd
    stosw
    mov eax, edi
    sub eax, CODEBUF
    mov [NEXT_IP], eax
    mov byte [edi], 0xf4

    mov esi, [CURRENT_ROW]
    mov dword [EXPECT_VEC], 0x30
    cmp byte [cs:esi+ROW_BAD], 0
    je .valid
    mov dword [EXPECT_VEC], 6
.valid:
    mov eax, [cs:esi+ROW_EA]
    test dword [WIDTHS], 1
    jnz .value_ready
    and eax, 0xffff
    mov ebx, [DEST]
    mov edx, [cs:original_regs+ebx*4]
    and edx, 0xffff0000
    or eax, edx ; LEA r16 preserves the destination's upper half.
.value_ready:
    mov [EXPECT_VALUE], eax
    mov eax, [COUNT]
    and eax, 2
    jz .null_data
    mov eax, UDATA
.null_data:
    mov [EXPECT_DATA], eax
    mov eax, 2
    test dword [COUNT], 1
    jnz .flags_ready
    mov eax, FLAGS_SET
.flags_ready:
    mov [EXPECT_FLAGS], eax
    mov eax, CR2_SENTINEL
    mov cr2, eax
    push dword USTACK
    push dword R4
    push dword [EXPECT_FLAGS]
    push dword [EXPECT_CS]
    push dword 0
    mov ax, [EXPECT_DATA]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, R0
    mov ecx, R1
    mov edx, R2
    mov ebx, R3
    mov ebp, R5
    mov esi, R6
    mov edi, R7
    iretd

success_handler:
    push dword 0
    mov dword [ss:GOT_VEC], 0x30
    jmp check_frame
ud_handler:
    push dword 0 ; #UD has no error code.
    mov dword [ss:GOT_VEC], 6
check_frame:
    pushad
    mov dword [ss:CHECK_ID], 1
    mov eax, [ss:GOT_VEC]
    cmp eax, [ss:EXPECT_VEC]
    jne unexpected
    cmp dword [ss:esp+32], 0
    jne unexpected
    mov dword [ss:CHECK_ID], 2
    xor edi, edi
.reg:
    mov eax, [cs:original_regs+edi*4]
    cmp dword [ss:EXPECT_VEC], 0x30
    jne .compare_reg
    cmp edi, [ss:DEST]
    jne .compare_reg
    mov eax, [ss:EXPECT_VALUE]
.compare_reg:
    mov edx, 7
    sub edx, edi
    mov edx, [ss:esp+edx*4]
    cmp edi, 4
    jne .not_esp
    mov edx, [ss:esp+48] ; User ESP saved by the CPL3 -> CPL0 transition.
.not_esp:
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
    cmp word [ss:esp+52], USTACK
    jne unexpected
    mov eax, [ss:esp+44]
    xor eax, [ss:EXPECT_FLAGS]
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
    xor eax, eax
    cmp dword [ss:EXPECT_VEC], 6
    je .ip
    mov eax, [ss:NEXT_IP]
.ip:
    cmp [ss:esp+36], eax
    jne unexpected
    mov dword [ss:CHECK_ID], 5
    mov eax, cr2
    cmp eax, CR2_SENTINEL
    jne unexpected
    mov esp, KSTACK
    inc dword [ss:COUNT]
    inc dword [ss:PREFIX_INDEX]
    cmp dword [ss:PREFIX_INDEX], 7
    jb .prefix_ready
    mov dword [ss:PREFIX_INDEX], 0
.prefix_ready:
    inc dword [ss:DEST]
    cmp dword [ss:DEST], 8
    jb case_begin
    mov dword [ss:DEST], 0
    inc dword [ss:WIDTHS]
    cmp dword [ss:WIDTHS], 4
    jb case_begin
    mov dword [ss:WIDTHS], 0
    add dword [ss:CURRENT_ROW], ROW_BYTES
    cmp dword [ss:CURRENT_ROW], cases_end
    jb case_begin
    mov dword [ss:CHECK_ID], 6
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
prefixes: db 0,0x26,0x2e,0x36,0x3e,0x64,0x65
passed: db 'CPU386 LEA PASS cases=26528',10,0
failed: db 'CPU386 LEA FAIL case=',0
check_text: db ' check=',0
value_text: db ' value=',0
original_regs: dd R0,R1,R2,R3,R4,R5,R6,R7

; Table entry: expected offset, length/address width/invalid flag, 8 code bytes.
; ModRM.reg starts at zero and is varied independently at runtime.
%macro ROW 7
    dd (%1) & 0xffffffff
    db 2+(%4!=-1)+%5, %2, %7, 0
    db 0x8d, %3
%if %4!=-1
    db %4
%endif
%if %5=1
    db %6 & 255
%elif %5=2
    dw %6 & 0xffff
%elif %5=4
    dd %6 & 0xffffffff
%endif
    times 8-(2+(%4!=-1)+%5) db 0
%endmacro

align 4
cases:
; Invalid register sources first, so an old decoder fails without a long run.
%assign addr 16
%rep 2
%assign rm 0
%rep 8
    ROW 0, addr, 0xc0|rm, -1, 0, 0, 1
%assign rm rm+1
%endrep
%assign addr 32
%endrep

; Intel Table 17-2: the eight 16-bit EA combinations, all three memory mods.
%define EA0 (R3+R6)
%define EA1 (R3+R7)
%define EA2 (R5+R6)
%define EA3 (R5+R7)
%define EA4 R6
%define EA5 R7
%define EA6 R5
%define EA7 R3
%assign mod 0
%rep 3
%assign rm 0
%rep 8
%assign value EA%+rm
%assign dsiz 0
%assign disp 0
%if mod=0 && rm=6
%assign value 0
%assign dsiz 2
%assign disp 0xfedc
%elif mod=1
%assign dsiz 1
%assign disp -128
%elif mod=2
%assign dsiz 2
%assign disp 0xfedc
%endif
    ROW (value+disp)&0xffff, 16, (mod<<6)|rm, -1, dsiz, disp, 0
%assign rm rm+1
%endrep
%assign mod mod+1
%endrep

; Intel Tables 17-3/17-4: every non-SIB r/m and every SIB byte for each mod.
; No-index (index=4) ignores scale; no-base (base=5, mod=0) uses disp32.
%assign mod 0
%rep 3
%assign rm 0
%rep 8
%assign sib 0
%assign nsib 1
%if rm=4
%assign nsib 256
%endif
%rep nsib
%assign raw_sib -1
%assign base rm
%assign index_value 0
%if rm=4
%assign raw_sib sib
%assign base sib&7
%assign idx (sib>>3)&7
%if idx!=4
%assign index_value R%+idx << (sib>>6)
%endif
%endif
%assign value R%+base
%assign dsiz 0
%assign disp 0
%if mod=0 && base=5
%assign value 0
%assign dsiz 4
%assign disp 0x87654321
%elif mod=1
%assign dsiz 1
%assign disp -128
%elif mod=2
%assign dsiz 4
%assign disp 0x87654321
%endif
    ROW value+index_value+disp, 32, (mod<<6)|rm, raw_sib, dsiz, disp, 0
%assign sib sib+1
%endrep
%assign rm rm+1
%endrep
%assign mod mod+1
%endrep
cases_end:
%if (cases_end-cases)/ROW_BYTES != ROW_COUNT
%error "Update matrix and runner when changing LEA coverage"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC 0x12345000, 0x1f, 0xf2, 0x40
    DESC 0x23456000, 0xffff, 0xf2, 0x40
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
