; Standalone 64 KiB ROM: Intel 80386 POP, PRM 9.1/9.8, AMD APM v3
; rev.3.19 pp.246-247. Intel SDM POP clarifies non-wrapping ESP-based EA.
; Test POP r16/r32 and r/m16/r/m32 through real paging/segmentation at CPL3.
; Segment POP and POPF have different commit rules and need separate coverage.
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
%define EXPECT_CR2 STATE+32
%define EXPECT_ESP STATE+36
%define GOT_ESP STATE+40

%define ROW_IP 0
%define ROW_NEXT 4
%define ROW_CS 8
%define ROW_SIZE 12
%define ROW_REG 16
%define ROW_DEST 20
%define ROW_VALUE 24
%define ROW_BYTES 28
%define MEMORY_DEST -1
%define CONTEXT_COUNT 10

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
    mov word [0xb8000], 0x074f ; Visible 'O'.
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
    mov dword [PT+8*4], 0x8003 ; Supervisor-only exception stack.
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
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov dx, 0x190
    mov al, 0xee ; Stop verbose per-instruction tracing, retain fault logs.
    out dx, al

context_begin:
    mov dword [ROW], cases
case_begin:
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    mov eax, [CONTEXT]
    and eax, 1
    shl eax, 3
    add eax, USTACK16
    mov [EXPECT_SS], eax
    mov word [GDT+48], 0xffff
    mov word [GDT+56], 0xffff
    mov dword [PT+6*4], 0x6007
    mov dword [PT+10*4], 0xa007
    mov eax, cr3
    mov cr3, eax
    mov edi, USTACK_TOP
    mov ecx, 4
    mov eax, CANARY
    rep stosd
    mov edi, 0xa010
    mov ecx, 20
    rep stosd
    mov eax, [cs:esi+ROW_VALUE]
    mov [USTACK_TOP], eax
    mov dword [EXPECT_VEC], 0x30
    mov dword [EXPECT_ERR], 0
    mov dword [EXPECT_CR2], 0
    mov eax, USTACK_TOP
    add eax, [cs:esi+ROW_SIZE]
    cmp dword [cs:esi+ROW_REG], 4
    jne .not_pop_sp
    ; POP SP/ESP writes its operand after the implicit increment.
    mov eax, [cs:esi+ROW_VALUE]
.not_pop_sp:
    mov [EXPECT_ESP], eax
    mov eax, [CONTEXT]
    shr eax, 1
    test eax, eax
    jz .ready
    cmp eax, 1
    jne .source_page
    ; Source starts within the segment but its final byte exceeds the limit.
    mov eax, USTACK_TOP-2
    add eax, [cs:esi+ROW_SIZE]
    mov [GDT+48], ax
    mov [GDT+56], ax
    mov dword [EXPECT_VEC], 12
    jmp .fault
.source_page:
    cmp eax, 2
    jne .destination
    mov dword [PT+6*4], 0
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 4 ; User read from non-present source.
    mov dword [EXPECT_CR2], USTACK_TOP
    jmp .fault
.destination:
    ; Register destinations cannot fault after reading the source. These rows
    ; remain successful controls in the two destination-page contexts.
    cmp dword [cs:esi+ROW_REG], MEMORY_DEST
    jne .ready
    mov edx, [cs:esi+ROW_DEST]
    mov [EXPECT_CR2], edx
    shr edx, 12
    mov dword [EXPECT_VEC], 14
    mov dword [EXPECT_ERR], 7 ; User write, present read-only destination.
    cmp eax, 3
    jne .absent_destination
    and dword [PT+edx*4], ~2
    jmp .fault
.absent_destination:
    mov dword [PT+edx*4], 0
    mov dword [EXPECT_ERR], 6
    cmp edx, USTACK_TOP>>12
    jne .fault
    ; POP [ESP] aliases the source page: making it absent faults on the read,
    ; unlike making that page read-only, which specifically faults on write.
    mov dword [EXPECT_ERR], 4
    mov dword [EXPECT_CR2], USTACK_TOP
.fault:
    mov dword [EXPECT_ESP], USTACK_TOP
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
    push dword 0 ; Normalize the error-code layout for a successful INT.
    mov dword [es:GOT_VEC], 0x30
    jmp check_frame
ss_handler:
    mov dword [es:GOT_VEC], 12
    jmp check_frame
pf_handler:
    mov dword [es:GOT_VEC], 14
check_frame:
    pushad
    mov eax, [ss:esp+48]
    mov [es:GOT_ESP], eax
    mov dword [es:CHECK_ID], 1
    mov eax, [es:GOT_VEC]
    cmp eax, [es:EXPECT_VEC]
    jne unexpected
    mov eax, [ss:esp+32]
    cmp eax, [es:EXPECT_ERR]
    jne unexpected
    mov esi, [es:ROW]
    mov dword [es:CHECK_ID], 2
    xor edi, edi
.reg:
    cmp edi, 4
    je .next_reg ; User ESP is in the privilege frame, not PUSHAD's ESP slot.
    mov eax, [cs:original_regs+edi*4]
    cmp dword [es:EXPECT_VEC], 0x30
    jne .compare_reg
    cmp edi, [cs:esi+ROW_REG]
    jne .compare_reg
    cmp dword [cs:esi+ROW_SIZE], 4
    jne .word_reg
    mov eax, [cs:esi+ROW_VALUE]
    jmp .compare_reg
.word_reg:
    mov ax, [cs:esi+ROW_VALUE]
.compare_reg:
    mov edx, 7
    sub edx, edi
    cmp [ss:esp+edx*4], eax
    jne unexpected
.next_reg:
    inc edi
    cmp edi, 8
    jb .reg
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
    cmp dword [es:EXPECT_VEC], 0x30
    jne .fault_frame
    mov eax, [cs:esi+ROW_NEXT]
.fault_frame:
    cmp [ss:esp+36], eax
    jne unexpected
    mov eax, [es:EXPECT_ESP]
    cmp [ss:esp+48], eax
    jne unexpected
    mov dword [es:CHECK_ID], 5
    cmp dword [es:EXPECT_VEC], 14
    jne .memory
    mov eax, cr2
    cmp eax, [es:EXPECT_CR2]
    jne unexpected
.memory:
    mov dword [es:PT+6*4], 0x6007
    mov dword [es:PT+10*4], 0xa007
    mov eax, cr3
    mov cr3, eax
    mov dword [es:CHECK_ID], 6
    mov edi, USTACK_TOP
    mov ebp, USTACK_TOP+16
    call check_memory
    mov edi, 0xa010
    mov ebp, 0xa060
    call check_memory
    mov esp, KSTACK_TOP
    inc dword [COUNT]
    add dword [ROW], ROW_BYTES
    cmp dword [ROW], cases_end
    jb case_begin
    inc dword [CONTEXT]
    cmp dword [CONTEXT], CONTEXT_COUNT
    jb context_begin
    mov dword [CHECK_ID], 7
    cmp dword [COUNT], CONTEXT_COUNT*(cases_end-cases)/ROW_BYTES
    jne unexpected
    mov esi, passed
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt

; Independent byte oracle: initialize canaries + source, then overlay only a
; successful memory destination. This also verifies POP [ESP] overlap and
; adjacent bytes, rather than checking only the reported popped value.
check_memory:
    mov al, 0xcc
    mov ecx, edi
    sub ecx, USTACK_TOP
    cmp ecx, 4
    jae .destination
    mov eax, [cs:esi+ROW_VALUE]
    shl ecx, 3
    shr eax, cl
.destination:
    cmp dword [es:EXPECT_VEC], 0x30
    jne .compare
    cmp dword [cs:esi+ROW_REG], MEMORY_DEST
    jne .compare
    mov ecx, edi
    sub ecx, [cs:esi+ROW_DEST]
    cmp ecx, [cs:esi+ROW_SIZE]
    jae .compare
    mov eax, [cs:esi+ROW_VALUE]
    shl ecx, 3
    shr eax, cl
.compare:
    cmp [es:edi], al
    jne unexpected
    inc edi
    cmp edi, ebp
    jb check_memory
    ret

unexpected:
    mov esi, failed
    call print
    mov eax, [es:COUNT]
    call hex32
    mov esi, check_text
    call print
    mov eax, [es:CHECK_ID]
    call hex32
    mov esi, esp_text
    call print
    mov eax, [es:GOT_ESP]
    call hex32
    mov esi, expected_text
    call print
    mov eax, [es:EXPECT_ESP]
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
passed: db 'CPU386 POP PASS cases=800',10,0
failed: db 'CPU386 POP FAIL case=',0
check_text: db ' check=',0
esp_text: db ' esp=',0
expected_text: db ' expected=',0
original_regs:
    dd 0x11223344,0x22334455,0x33445566,0xa020
    dd USTACK_TOP,0x55667788,0xa040,0x778899aa

; Raw bytes include both register encodings and address-size overrides.
; Expected destination for ESP-based memory uses the post-increment pointer.
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
    hlt
%endif
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
%assign r 0
%rep 8
%assign val 0x89abcdef
%if r = 4
%assign val 0x6780 ; Keep ESP's high half zero; gate high-ESP rules are separate.
%endif
    CASE reg%+r, r, 0, val, 0x58+r, ''
    CASE rm%+r, r, 0, val, 0x8f, 0xc0+r
%assign r r+1
%endrep
%if codebits = 16
    CASE mem16, MEMORY_DEST, 0xa020, 0x89abcdef, 0x8f, 0x07 ; [BX]
    CASE mem32, MEMORY_DEST, 0xa040, 0x89abcdef, 0x67, 0x8f,0x06 ; [ESI]
    CASE esp, MEMORY_DEST, USTACK_TOP+opbits/8, 0x89abcdef, 0x67, 0x8f,0x04,0x24
    CASE espdisp, MEMORY_DEST, 0xa020+opbits/8, 0x89abcdef, 0x67, 0x8f,0x84,0x24,0x20,0x3f,0,0
%else
    CASE mem16, MEMORY_DEST, 0xa020, 0x89abcdef, 0x67, 0x8f,0x07
    CASE mem32, MEMORY_DEST, 0xa040, 0x89abcdef, 0x8f, 0x06
    CASE esp, MEMORY_DEST, USTACK_TOP+opbits/8, 0x89abcdef, 0x8f, 0x04,0x24
    CASE espdisp, MEMORY_DEST, 0xa020+opbits/8, 0x89abcdef, 0x8f, 0x84,0x24,0x20,0x3f,0,0
%endif
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
%if (cases_end-cases)/ROW_BYTES != 80
%error "Update runner/pass count when adding POP cases"
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
