; Intel 80386 MOVS/REP and AMD APM v3 MOVS: read the complete element before
; writing it, then update indexes. Overlap propagates in DF order, not with
; memmove semantics. Scalar reference code models that order independently.
; 64 KiB BIOS ROM, no DOS/disks. Runs real, PM16 and PM32 code on a 386.
cpu 386
bits 16
org 0

%define ROM 0xf0000
%define GDT 0x3000
%define IDT 0x4000
%define STATE 0x5000
%define STACK 0x9000
%define CODEBUF 0x10000
%define DATA 0x20000
%define REFERENCE 0x40000
%define KCODE 8
%define KDATA 16
%define SRCSEG 24
%define DST0 32
%define DST16 40
%define UCODE16 48
%define UCODE32 56
%define KCODE16 64
%define VALUE 0x89abcdef
%define FLAGS_TEST 0x8d7
%define FLAGS_MASK 0xed5
%define TOTAL_CASES 10368

%define COUNT STATE
%define ROW STATE+4
%define DELTA_INDEX STATE+8
%define SCENARIO STATE+12
%define CHECK_ID STATE+16
%define GOT STATE+20
%define WANT STATE+24
%define ITERATIONS STATE+28
%define SRC_OFF STATE+32
%define DST_OFF STATE+36
%define LOW_OFF STATE+40
%define HIGH_OFF STATE+44
%define START_SRC STATE+48
%define START_DST STATE+52
%define START_COUNT STATE+56
%define TEST_FLAGS STATE+60
%define EXPECTED_DS STATE+64
%define EXPECTED_ES STATE+68
%define SAVED_DS STATE+72
%define SAVED_ES STATE+76
%define SAVED_FS STATE+80
%define SAVED_GS STATE+84
%define JUMP_PTR STATE+88
%define EXPECTED_REGS STATE+128

%define R_MODE 0 ; 0=real, 1=protected CS.D=0, 2=protected CS.D=1.
%define R_ADDRESS 4
%define R_WIDTH 8
%define R_STEP 12
%define R_ALIAS 16
%define R_SOURCE 20
%define ROW_BYTES 24
%define DELTAS 12
%define SCENARIOS 6

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1 >> 16) & 255, %3, ((%2 >> 16) & 15) | %4, (%1 >> 24) & 255
%endmacro

start:
    cli
    cld
    mov al, 0xff ; This is not an IRQ timing test.
    out 0x21, al
    out 0xa1, al
    xor ax, ax
    mov ss, ax
    mov sp, STACK
    mov es, ax
    xor di, di
    mov cx, 256
.ivt:
    mov ax, unexpected_real
    stosw
    mov ax, 0xf000
    stosw
    loop .ivt
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
    jmp dword KCODE:setup

bits 32
setup:
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, STACK
    mov word [0xb8000], 0x074f
    mov dword [COUNT], 0
    mov dword [ROW], cases
    mov dword [SCENARIO], 0
    mov dword [DELTA_INDEX], 0
    mov edi, IDT
    mov ecx, 32
    mov eax, (unexpected-$$) | (KCODE<<16)
    mov edx, 0x8e00
.idt:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .idt
    lidt [cs:pm_idt_ptr]
    mov dx, 0x190
    mov al, 0xee
    out dx, al

case_begin:
    cld
    mov esp, STACK
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov dword [CHECK_ID], 0
    mov esi, [ROW]
    mov eax, [SCENARIO]
    mov eax, [cs:counts+eax*4]
    mov [ITERATIONS], eax
    mov [START_COUNT], eax
    cmp dword [SCENARIO], SCENARIOS-1
    jne .indexes
    mov dword [START_COUNT], 7 ; Single MOVS must not decrement ECX.
.indexes:
    mov eax, [cs:esi+R_SOURCE]
    mov [SRC_OFF], eax
    mov [START_SRC], eax
    mov ebx, [DELTA_INDEX]
    add eax, [cs:deltas+ebx*4]
    mov [DST_OFF], eax
    sub eax, [cs:esi+R_ALIAS]
    mov [START_DST], eax
    cmp dword [cs:esi+R_ADDRESS], 32
    je .flags
    or dword [START_SRC], 0x12340000
    or dword [START_DST], 0xabcd0000
    or dword [START_COUNT], 0x55aa0000
.flags:
    mov dword [TEST_FLAGS], FLAGS_TEST
    cmp dword [cs:esi+R_STEP], 0
    jg .range
    or dword [TEST_FLAGS], 0x400
.range:
    mov eax, [SRC_OFF]
    mov edx, [DST_OFF]
    cmp eax, edx
    jbe .ordered
    xchg eax, edx
.ordered:
    mov ecx, [ITERATIONS]
    test ecx, ecx
    jz .span
    dec ecx
.span:
    imul ecx, [cs:esi+R_WIDTH]
    cmp dword [cs:esi+R_STEP], 0
    jg .forward
    sub eax, ecx
    jmp .guards
.forward:
    add edx, ecx
.guards:
    sub eax, 4
    add edx, [cs:esi+R_WIDTH]
    add edx, 4
    mov [LOW_OFF], eax
    mov [HIGH_OFF], edx
    mov edi, eax
.init:
    mov eax, edi
    shr eax, 8
    xor eax, edi
    xor al, 0x5a
    mov [DATA+edi], al
    mov [REFERENCE+edi], al
    inc edi
    cmp edi, [HIGH_OFF]
    jb .init
    ; Snapshot a whole element into EAX before writing its bytes. Subsequent
    ; iterations read the modified reference, preserving overlap propagation.
    mov ebx, [SRC_OFF]
    mov edi, [DST_OFF]
    mov ecx, [ITERATIONS]
.reference:
    test ecx, ecx
    jz .code
    mov eax, [REFERENCE+ebx]
    xor edx, edx
.reference_byte:
    mov [REFERENCE+edi+edx], al
    shr eax, 8
    inc edx
    cmp edx, [cs:esi+R_WIDTH]
    jb .reference_byte
    add ebx, [cs:esi+R_STEP]
    add edi, [cs:esi+R_STEP]
    dec ecx
    jmp .reference
.code:
    mov edi, CODEBUF
    mov eax, 16
    cmp dword [cs:esi+R_WIDTH], 4
    jne .operand
    mov eax, 32
.operand:
    mov edx, 16
    cmp dword [cs:esi+R_MODE], 2
    jne .codebits
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
    cmp dword [SCENARIO], SCENARIOS-1
    je .opcode
    mov al, 0xf3
    stosb
.opcode:
    mov al, 0xa4
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov al, 0xea ; A far JMP preserves tested flags and all GPRs.
    stosb
    mov eax, pm_return
    cmp dword [cs:esi+R_MODE], 0
    jne .return_offset
    mov eax, rm_return
.return_offset:
    cmp edx, 32
    je .offset32
    stosw
    jmp .selector
.offset32:
    stosd
.selector:
    mov ax, KCODE
    cmp dword [cs:esi+R_MODE], 0
    jne .selector_emit
    mov ax, 0xf000
.selector_emit:
    stosw
    mov dword [EXPECTED_DS], SRCSEG
    mov eax, DST0
    cmp dword [cs:esi+R_ALIAS], 0
    je .es
    mov eax, DST16
.es:
    mov [EXPECTED_ES], eax
    cmp dword [cs:esi+R_MODE], 0
    jne .protected
    mov dword [EXPECTED_DS], DATA>>4
    mov eax, [cs:esi+R_ALIAS]
    shr eax, 4
    add eax, DATA>>4
    mov [EXPECTED_ES], eax
    jmp KCODE16:leave_pm
.protected:
    mov dword [JUMP_PTR], 0
    mov word [JUMP_PTR+4], UCODE16
    cmp dword [cs:esi+R_MODE], 2
    jne .registers
    mov word [JUMP_PTR+4], UCODE32
.registers:
    push dword [TEST_FLAGS]
    popfd
    mov ax, [ss:EXPECTED_DS]
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, [ss:EXPECTED_ES]
    mov es, ax
    mov ecx, [ss:START_COUNT]
    mov esi, [ss:START_SRC]
    mov edi, [ss:START_DST]
    mov eax, VALUE
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    jmp far [ss:JUMP_PTR]

bits 16
leave_pm:
    cli
    mov eax, cr0
    and al, 0xfe
    mov cr0, eax
    jmp 0xf000:rm_enter
rm_enter:
    lidt [cs:rm_idt_ptr]
    xor ax, ax
    mov ss, ax
    mov esp, STACK
    push dword [ss:TEST_FLAGS]
    popfd
    mov ax, [ss:EXPECTED_DS]
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ax, [ss:EXPECTED_ES]
    mov es, ax
    mov ecx, [ss:START_COUNT]
    mov esi, [ss:START_SRC]
    mov edi, [ss:START_DST]
    mov eax, VALUE
    mov edx, 0x33445566
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    jmp 0x1000:0
rm_return:
    pushfd
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
    cli
    lidt [cs:pm_idt_ptr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp dword KCODE:check_frame
unexpected_real:
    mov dx, 0x191
    mov si, real_exception
.text:
    mov al, [cs:si]
    inc si
    test al, al
    jz .fail
    out dx, al
    jmp .text
.fail:
    mov al, 0xfe
    out 0x80, al
.halt:
    cli
    hlt
    jmp .halt

bits 32
pm_return:
    pushfd
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
check_frame:
    mov ax, KDATA
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov esi, [ROW]
    mov dword [CHECK_ID], 1
    mov dword [EXPECTED_REGS], VALUE
    mov eax, [START_COUNT]
    cmp dword [SCENARIO], SCENARIOS-1
    je .count
    sub eax, [ITERATIONS]
.count:
    mov [EXPECTED_REGS+4], eax
    mov dword [EXPECTED_REGS+8], 0x33445566
    mov dword [EXPECTED_REGS+12], 0x44556677
    mov dword [EXPECTED_REGS+16], STACK
    mov dword [EXPECTED_REGS+20], 0x55667788
    mov eax, [ITERATIONS]
    imul eax, [cs:esi+R_STEP]
    mov edx, eax
    add eax, [START_SRC]
    add edx, [START_DST]
    mov [EXPECTED_REGS+24], eax
    mov [EXPECTED_REGS+28], edx
    xor edi, edi
.reg:
    mov edx, 7
    sub edx, edi
    mov eax, [esp+edx*4]
    cmp edi, 4
    jne .compare
    add eax, 4 ; PUSHAD saved the ESP after PUSHFD.
.compare:
    mov [GOT], eax
    mov edx, [EXPECTED_REGS+edi*4]
    mov [WANT], edx
    cmp eax, edx
    jne unexpected
    inc edi
    cmp edi, 8
    jb .reg
    mov dword [CHECK_ID], 2
    mov eax, [esp+32]
    xor eax, [TEST_FLAGS]
    test eax, FLAGS_MASK
    jnz unexpected
    mov ax, [SAVED_DS]
    cmp ax, [EXPECTED_DS]
    jne unexpected
    mov ax, [SAVED_ES]
    cmp ax, [EXPECTED_ES]
    jne unexpected
    mov ax, [SAVED_FS]
    cmp ax, [EXPECTED_DS]
    jne unexpected
    mov ax, [SAVED_GS]
    cmp ax, [EXPECTED_DS]
    jne unexpected
    mov dword [CHECK_ID], 3
    mov edi, [LOW_OFF]
.memory:
    movzx eax, byte [DATA+edi]
    mov [GOT], eax
    movzx edx, byte [REFERENCE+edi]
    mov [WANT], edx
    cmp eax, edx
    jne unexpected
    inc edi
    cmp edi, [HIGH_OFF]
    jb .memory
    inc dword [COUNT]
    inc dword [SCENARIO]
    cmp dword [SCENARIO], SCENARIOS
    jb case_begin
    mov dword [SCENARIO], 0
    inc dword [DELTA_INDEX]
    cmp dword [DELTA_INDEX], DELTAS
    jb case_begin
    mov dword [DELTA_INDEX], 0
    add dword [ROW], ROW_BYTES
    cmp dword [ROW], cases_end
    jb case_begin
    mov dword [CHECK_ID], 4
    cmp dword [COUNT], TOTAL_CASES
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
    mov esi, got_text
    call print
    mov eax, [ss:GOT]
    call hex32
    mov esi, want_text
    call print
    mov eax, [ss:WANT]
    call hex32
    mov esi, offset_text
    call print
    mov eax, edi
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
passed: db 'CPU386 MOVS OVERLAP PASS cases=10368',10,0
failed: db 'CPU386 MOVS OVERLAP FAIL case=',0
check_text: db ' check=',0
got_text: db ' got=',0
want_text: db ' want=',0
offset_text: db ' offset=',0
real_exception: db 'CPU386 MOVS OVERLAP unexpected real exception',10,0

align 4
deltas: dd -33,-4,-3,-2,-1,0,1,2,3,4,33,0x1200
counts: dd 0,1,2,17,1025,1
cases:
%assign mode 0
%rep 3
%assign address 16
%rep 2
%assign width 1
%rep 3
%assign direction 0
%rep 2
%assign step width
%if direction
%assign step -width
%endif
%assign alias 0
%rep 2
%assign alignment 0
%rep 2
    dd mode,address,width,step,alias,0x3000+alignment
%assign alignment 1
%endrep
%assign alias 16
%endrep
%assign direction 1
%endrep
%assign width width*2
%endrep
%assign address 32
%endrep
%assign mode mode+1
%endrep
cases_end:
%if (cases_end-cases)/ROW_BYTES*DELTAS*SCENARIOS != TOTAL_CASES
%error "Update overlap case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC DATA, 0xffff, 0x92, 0x40
    DESC DATA, 0xffff, 0x92, 0x40
    DESC DATA+16, 0xffff, 0x92, 0x40
    DESC CODEBUF, 0xfff, 0x9a, 0
    DESC CODEBUF, 0xfff, 0x9a, 0x40
    DESC ROM, 0xffff, 0x9a, 0
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
    dd GDT
pm_idt_ptr: dw 32*8-1
    dd IDT
rm_idt_ptr: dw 0x3ff
    dd 0

bits 16
times 0xfff0-($-$$) db 0xff
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
