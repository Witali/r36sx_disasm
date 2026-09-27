; 64 KiB ROM at F0000h. Intel 80386 PRM MOVS/STOS/REP; AMD APM v3.
; Test indices/counts independently of operand size, including 16-bit wrap
; between elements. Every word/dword itself remains inside the segment.
; A byte-level reference buffer uses scalar MOV, not the tested string op.
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
%define SOURCE 0x20000
%define DESTINATION 0x40000
%define REFERENCE 0x60000
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define SRCSEG (40|3)
%define DSTSEG (48|3)
%define STACKSEG (56|3)
%define TASK 64
%define VALUE 0x89abcdef
%define FLAGS_TEST 0x8d7
%define FLAGS_MASK 0xed5
%define CR2_SENTINEL 0x13579bdf

%define COUNT STATE
%define ROW STATE+4
%define NEXT_IP STATE+8
%define CHECK_ID STATE+12
%define GOT_VALUE STATE+16
%define OFFSET STATE+20
%define ITERATION STATE+24
%define EXPECTED_REGS STATE+64

%define R_CS 0
%define R_WIDTH 4
%define R_ADDRESS 8
%define R_KIND 12 ; 0=MOVS, 1=STOS.
%define R_STEP 16
%define R_REPEAT 20
%define R_ECX 24
%define R_ESI 28
%define R_EDI 32
%define R_COUNT 36
%define R_SRC_END 40
%define R_DST_END 44
%define R_ITERATIONS 48
%define R_FLAGS 52
%define ROW_BYTES 56
%define TOTAL_CASES 576

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
    mov dword [COUNT], 0
    mov word [0xb8000], 0x0753 ; Visible S without a video BIOS.
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
    mov word [IDT+0x30*8], check_frame
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    ; Distinguish offsets on either side of 64 KiB. Initialization does not
    ; use the instruction under test, avoiding a matching setup/result bug.
    xor ebx, ebx
.source:
    mov eax, ebx
    shr eax, 8
    xor eax, ebx
    mov edx, ebx
    shr edx, 16
    xor eax, edx
    xor al, 0x5a
    mov [SOURCE+ebx], al
    inc ebx
    cmp ebx, 0x20000
    jb .source
    mov dx, 0x190
    mov al, 0xee
    out dx, al
    mov dword [ROW], cases

case_begin:
    cld
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    ; Initialize exactly the touched range plus two whole elements at either
    ; end. Wrapped addresses are handled by the scalar reference calculation.
    mov dword [ITERATION], -2
.clear:
    call destination_offset
    xor ebx, ebx
.clear_byte:
    mov byte [DESTINATION+edi+ebx], 0xcc
    mov byte [REFERENCE+edi+ebx], 0xcc
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .clear_byte
    inc dword [ITERATION]
    mov eax, [cs:esi+R_ITERATIONS]
    add eax, 2
    cmp [ITERATION], eax
    jl .clear
    mov dword [ITERATION], 0
.reference:
    mov eax, [ITERATION]
    cmp eax, [cs:esi+R_ITERATIONS]
    jae .code
    call destination_offset
    mov [OFFSET], edi
    mov eax, VALUE
    cmp dword [cs:esi+R_KIND], 1
    je .value
    mov eax, [ITERATION]
    imul eax, [cs:esi+R_STEP]
    add eax, [cs:esi+R_ESI]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .source_index
    and eax, 0xffff
.source_index:
    ; Extra bytes of this scalar read are ignored for a byte/word test.
    mov eax, [SOURCE+eax]
.value:
    mov edi, [OFFSET]
    xor ebx, ebx
.reference_byte:
    mov [REFERENCE+edi+ebx], al
    shr eax, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .reference_byte
    inc dword [ITERATION]
    jmp .reference
.code:
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
    cmp dword [cs:esi+R_REPEAT], 0
    je .opcode
    mov al, 0xf3
    stosb
.opcode:
    mov al, 0xa4
    cmp dword [cs:esi+R_KIND], 0
    je .size
    mov al, 0xaa
.size:
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov ax, 0x30cd
    stosw
    mov eax, edi
    sub eax, CODEBUF
    mov [NEXT_IP], eax
    mov byte [edi], 0xf4
    mov dword [EXPECTED_REGS], VALUE
    mov eax, [cs:esi+R_COUNT]
    mov [EXPECTED_REGS+4], eax
    mov dword [EXPECTED_REGS+8], 0x33445566
    mov dword [EXPECTED_REGS+12], 0x44556677
    mov dword [EXPECTED_REGS+16], USTACK
    mov dword [EXPECTED_REGS+20], 0x55667788
    mov eax, [cs:esi+R_SRC_END]
    mov [EXPECTED_REGS+24], eax
    mov eax, [cs:esi+R_DST_END]
    mov [EXPECTED_REGS+28], eax
    mov eax, CR2_SENTINEL
    mov cr2, eax
    push dword STACKSEG
    push dword USTACK
    push dword [cs:esi+R_FLAGS]
    push dword [cs:esi+R_CS]
    push dword 0
    mov ecx, [cs:esi+R_ECX]
    mov edi, [cs:esi+R_EDI]
    mov esi, [cs:esi+R_ESI]
    mov ax, SRCSEG
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, DSTSEG
    mov es, ax
    mov eax, VALUE
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    iretd

destination_offset:
    mov edi, [ss:ITERATION]
    imul edi, [cs:esi+R_STEP]
    add edi, [cs:esi+R_EDI]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .done
    and edi, 0xffff
.done:
    ret

check_frame:
    pushad
    mov dword [ss:CHECK_ID], 1
    xor edi, edi
.reg:
    mov edx, 7
    sub edx, edi
    mov eax, [ss:esp+edx*4]
    cmp edi, 4
    jne .compare
    mov eax, [ss:esp+44] ; Outer ESP after EIP,CS,EFLAGS in a 32-bit gate.
.compare:
    mov [ss:GOT_VALUE], eax
    cmp eax, [ss:EXPECTED_REGS+edi*4]
    jne unexpected
    inc edi
    cmp edi, 8
    jb .reg
    mov dword [ss:CHECK_ID], 2
    mov esi, [ss:ROW]
    mov eax, [ss:esp+32]
    cmp eax, [ss:NEXT_IP]
    jne unexpected
    mov eax, [ss:esp+36]
    and eax, 0xffff
    cmp eax, [cs:esi+R_CS]
    jne unexpected
    cmp word [ss:esp+48], STACKSEG
    jne unexpected
    mov eax, [ss:esp+40]
    xor eax, [cs:esi+R_FLAGS]
    test eax, FLAGS_MASK
    jnz unexpected
    mov ax, ds
    cmp ax, SRCSEG
    jne unexpected
    mov ax, es
    cmp ax, DSTSEG
    jne unexpected
    mov ax, fs
    cmp ax, SRCSEG
    jne unexpected
    mov ax, gs
    cmp ax, SRCSEG
    jne unexpected
    mov eax, cr2
    cmp eax, CR2_SENTINEL
    jne unexpected
    mov dword [ss:CHECK_ID], 3
    mov dword [ss:ITERATION], -2
.memory:
    call destination_offset
    xor ebx, ebx
.byte:
    mov al, [ss:DESTINATION+edi+ebx]
    cmp al, [ss:REFERENCE+edi+ebx]
    jne unexpected
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ss:ITERATION]
    mov eax, [cs:esi+R_ITERATIONS]
    add eax, 2
    cmp [ss:ITERATION], eax
    jl .memory
    mov esp, KSTACK
    inc dword [ss:COUNT]
    add dword [ss:ROW], ROW_BYTES
    cmp dword [ss:ROW], cases_end
    jb case_begin
    mov dword [ss:CHECK_ID], 4
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
passed: db 'CPU386 STRINGS PASS cases=576',10,0
failed: db 'CPU386 STRINGS FAIL case=',0
check_text: db ' check=',0
value_text: db ' value=',0

; Expectations use NASM integer arithmetic, not emulator indexing helpers.
align 4
cases:
%assign codebits 16
%rep 2
%assign code UCODE16
%if codebits=32
%assign code UCODE32
%endif
%assign addrbits 16
%rep 2
%assign width 1
%rep 3
%assign direction 0
%rep 2
%assign step width
%assign boundary 0x10000-width
%if direction
%assign step -width
%assign boundary 0
%if addrbits=32
%assign boundary 0x10000
%endif
%endif
%assign scenario 0
%rep 6
%assign repetitions 3
%if scenario=1
%assign repetitions 0
%elif scenario=2
%assign repetitions 1
%elif scenario=3
%assign repetitions 17
%elif scenario=4
%assign repetitions 2049
%endif
%assign iterations repetitions
%assign repeat 1
%if scenario=5
%assign iterations 1
%assign repeat 0
%endif
%assign pattern 0
%rep 4
%assign kind 0
%assign src boundary
%assign dst boundary
%if pattern=0
%assign dst 0x5000
%elif pattern=1
%assign src 0x5000
%elif pattern=3
%assign kind 1
%assign src 0x5000
%endif
%assign srcend src+step*iterations
%if kind=1
%assign srcend src
%endif
%assign dstend dst+step*iterations
%assign counter repetitions
%assign counterend repetitions-iterations
%if !repeat
%assign counterend repetitions
%endif
%if addrbits=16
%assign src src|0x12340000
%assign dst dst|0xabcd0000
%assign srcend (srcend&0xffff)|0x12340000
%assign dstend (dstend&0xffff)|0xabcd0000
%assign counter counter|0x55aa0000
%assign counterend counterend|0x55aa0000
%endif
    dd code,width,addrbits,kind,step,repeat,counter,src,dst
    dd counterend,srcend,dstend,iterations,FLAGS_TEST|(direction<<10)
%assign pattern pattern+1
%endrep
%assign scenario scenario+1
%endrep
%assign direction 1
%endrep
%assign width width*2
%endrep
%assign addrbits 32
%endrep
%assign codebits 32
%endrep
cases_end:
%if (cases_end-cases)/ROW_BYTES != TOTAL_CASES
%error "Update strings case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC SOURCE, 0x1ffff, 0xf2, 0x40
    DESC DESTINATION, 0x1ffff, 0xf2, 0x40
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
