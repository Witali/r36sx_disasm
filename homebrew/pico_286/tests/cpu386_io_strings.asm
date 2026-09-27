; Intel 80386 PRM INS/OUTS/REP, 8.3.2 and 12.3; AMD APM vol.3 INSx/OUTSx.
; CPL3 string I/O, with a CPL0 checker independent of the tested operation.
; DMA channels stay masked. Ports 0..3 are a four-byte observable fixture:
; reset the shared flip-flop, then access low/high/low/high register bytes.
; This uses Pico's adjacent-byte I/O model, not a claim about ISA bus timing.
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
%define SRC 0x20000
%define DST 0x40000
%define FSBASE 0x60000
%define GSBASE 0x80000
%define STACKBASE 0xa000
%define KCODE 8
%define KDATA 16
%define UCODE16 (24|3)
%define UCODE32 (32|3)
%define SRCSEG (40|3)
%define DSTSEG (48|3)
%define STACKSEG (56|3)
%define FSSEG (64|3)
%define GSSEG (72|3)
%define TASK 80
%define FLAGS_TEST 0xbd7
%define FLAGS_MASK 0x37fd7
%define PORT_FILL 0xdeadbeef
%define DMA_CLEAR_FF 0x0c

%define COUNT STATE
%define ROW STATE+4
%define SCENARIO STATE+8
%define CHECK_ID STATE+12
%define GOT STATE+16
%define WANT STATE+20
%define TRAPS STATE+24
%define COMPLETED STATE+28
%define TOTAL STATE+32
%define FINISH_AFTER STATE+36
%define EXPECT_GP STATE+40
%define START_COUNT STATE+44
%define START_SRC STATE+48
%define START_DST STATE+52
%define TEST_FLAGS STATE+56
%define NEXT_IP STATE+60
%define ITERATION STATE+64
%define EVENT STATE+68 ; 0=#DB, 1=#GP, 2=INT 30h completion.
%define LIVE_FLAGS STATE+72
%define GP_SEEN STATE+76
%define EXPECTED_REGS STATE+128

%define R_CS 0
%define R_WIDTH 4
%define R_ADDRESS 8
%define R_KIND 12 ; 0=INS, 1=OUTS.
%define R_STEP 16
%define R_PREFIX 20
%define R_BASE 24
%define ROW_BYTES 28
%define SCENARIOS 13
%define TOTAL_CASES 2496
; Reordering only helps reproduce the independent final-REP-IP bug on an
; old EXE before reaching its addr32 truncation bug; coverage is unchanged.
%ifndef IO_STRINGS_FIRST_ADDRESS
%define IO_STRINGS_FIRST_ADDRESS 32
%endif
%if IO_STRINGS_FIRST_ADDRESS != 16 && IO_STRINGS_FIRST_ADDRESS != 32
%error "IO_STRINGS_FIRST_ADDRESS must be 16 or 32"
%endif

; All handlers normalize an error-code slot before PUSHAD.
%define F_ERROR 32
%define F_IP 36
%define F_CS 40
%define F_FLAGS 44
%define F_SP 48
%define F_SS 52

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
    mov al, 15
    out 0x0f, al ; Mask every DMA channel: fixture accesses cannot transfer RAM.
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
    mov dword [COUNT], 0
    mov dword [SCENARIO], 0
    mov dword [ROW], cases
    mov edi, TSS
    xor eax, eax
    mov ecx, 27
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
    mov word [IDT+8], debug_trap
    mov word [IDT+13*8], gp_fault
    mov word [IDT+0x30*8], complete
    mov byte [IDT+0x30*8+5], 0xee
    lidt [cs:idt_ptr]
    xor eax, eax
    mov dr7, eax
    mov dx, 0x190
    mov al, 0xed
    out dx, al

case_begin:
    cld
    mov esp, KSTACK
    mov ax, KDATA
    mov ds, ax
    mov es, ax
    mov esi, [ROW]
    mov dword [CHECK_ID], 0
    mov dword [TRAPS], 0
    mov dword [GP_SEEN], 0
    mov dword [EXPECT_GP], 0
    mov word [TSS+102], 104
    mov word [TSS+104], 0xff00 ; Ports 0..7 allowed, following ports denied.
    mov eax, 3
    cmp dword [SCENARIO], 0
    jne .zero
    mov eax, 1
.zero:
    cmp dword [SCENARIO], 1
    je .zero_count
    cmp dword [SCENARIO], 11
    jne .one
.zero_count:
    xor eax, eax
.one:
    cmp dword [SCENARIO], 2
    jne .many
    mov eax, 1
.many:
    cmp dword [SCENARIO], 4
    jne .count
    mov eax, 17
.count:
    mov [TOTAL], eax
    mov [FINISH_AFTER], eax
    mov [START_COUNT], eax
    cmp dword [SCENARIO], 0
    jne .indices
    mov dword [START_COUNT], 7
.indices:
    mov dword [START_SRC], 0x11000
    mov dword [START_DST], 0x12000
    cmp dword [SCENARIO], 5
    jne .address
    mov eax, 0x10000
    cmp dword [cs:esi+R_STEP], 0
    jl .wrap_start
    sub eax, [cs:esi+R_WIDTH]
.wrap_start:
    mov [START_SRC], eax
    mov [START_DST], eax
.address:
    cmp dword [cs:esi+R_ADDRESS], 32
    je .high_count
    and dword [START_SRC], 0xffff
    and dword [START_DST], 0xffff
    or dword [START_SRC], 0x12340000
    or dword [START_DST], 0xabcd0000
    or dword [START_COUNT], 0x55aa0000
.high_count:
    cmp dword [SCENARIO], 6
    jne .flags
    ; Observe exactly the first element of ECX=10000h, then the handler
    ; deliberately skips the remainder. Addr16 must execute zero elements.
    mov dword [START_COUNT], 0x10000
    mov dword [FINISH_AFTER], 1
    cmp dword [cs:esi+R_ADDRESS], 32
    je .flags
    mov dword [TOTAL], 0
    mov dword [FINISH_AFTER], 0
.flags:
    mov dword [TEST_FLAGS], FLAGS_TEST
    cmp dword [cs:esi+R_STEP], 0
    jg .permissions
    or dword [TEST_FLAGS], 0x400
.permissions:
    cmp dword [SCENARIO], 7
    jb .memory
    cmp dword [SCENARIO], 12
    je .late_deny
    mov byte [TSS+104], 0xff
    cmp dword [SCENARIO], 11
    je .memory ; Zero REP bypasses both null segments and denied ports.
    cmp dword [SCENARIO], 9
    jne .deny
    or dword [TEST_FLAGS], 0x3000 ; CPL=IOPL: bypass the denied bitmap.
    jmp .memory
.deny:
    mov dword [EXPECT_GP], 1
    mov dword [FINISH_AFTER], 0
    cmp dword [SCENARIO], 10
    jne .bitmap
    mov word [TSS+102], 106 ; Beyond TSS limit: no bitmap at all.
    jmp .memory
.bitmap:
    mov ecx, [cs:esi+R_WIDTH]
    dec ecx
    cmp dword [SCENARIO], 8
    je .last_bit
    xor ecx, ecx
.last_bit:
    mov eax, 1
    shl eax, cl
    mov [TSS+104], al ; Deny first or last byte of the entire I/O operand.
    jmp .memory
.late_deny:
    mov dword [EXPECT_GP], 1
    mov dword [FINISH_AFTER], 1
.memory:
    mov dword [ITERATION], -1
.element:
    call memory_offsets
    call element_value
    mov edx, 0xcccccccc
    xor ebx, ebx
.byte:
    mov [ebp+ebx], al
    mov [DST+edi+ebx], dl
    shr eax, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ITERATION]
    mov eax, [ITERATION]
    cmp eax, [TOTAL]
    jle .element
    mov dword [COMPLETED], 0
    call seed_ports
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
    je .address_prefix
    mov al, 0x66
    stosb
.address_prefix:
    cmp edx, [cs:esi+R_ADDRESS]
    je .segment
    mov al, 0x67
    stosb
.segment:
    mov eax, [cs:esi+R_PREFIX]
    test al, al
    jz .repeat
    stosb
.repeat:
    cmp dword [SCENARIO], 0
    je .opcode
    mov al, 0xf3
    stosb
.opcode:
    mov eax, [cs:esi+R_KIND]
    shl eax, 1
    add al, 0x6c
    cmp dword [cs:esi+R_WIDTH], 1
    je .emit
    inc al
.emit:
    stosb
    mov eax, edi
    sub eax, CODEBUF
    mov [NEXT_IP], eax
    mov ax, 0x30cd
    stosw
    mov byte [edi], 0xf4
    xor eax, eax
    mov dr6, eax
    push dword STACKSEG
    push dword USTACK
    push dword [TEST_FLAGS]
    push dword [cs:esi+R_CS]
    push dword 0
    mov ecx, [START_COUNT]
    mov esi, [START_SRC]
    mov edi, [START_DST]
    mov ax, FSSEG
    mov fs, ax
    mov ax, GSSEG
    mov gs, ax
    mov ax, SRCSEG
    mov ds, ax
    mov ax, DSTSEG
    mov es, ax
    cmp dword [ss:TOTAL], 0
    jne .registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
.registers:
    mov eax, 0x89abcdef
    mov edx, 0xa5a50000 ; Only DX selects the port, never the high EDX bits.
    mov ebx, 0x44556677
    mov ebp, 0x55667788
    iretd

memory_offsets:
    mov edi, [ss:ITERATION]
    imul edi, [cs:esi+R_STEP]
    mov ebp, edi
    add edi, [ss:START_DST]
    add ebp, [ss:START_SRC]
    cmp dword [cs:esi+R_ADDRESS], 32
    je .base
    and edi, 0xffff
    and ebp, 0xffff
.base:
    add ebp, [cs:esi+R_BASE]
    ret
element_value:
    mov eax, 0xcccccccc
    cmp dword [ss:ITERATION], 0
    jl .done
    mov ecx, [ss:ITERATION]
    cmp ecx, [ss:TOTAL]
    jae .done
    imul ecx, 0x01010101
    mov eax, 0x12345678
    xor eax, ecx
.done:
    ret
seed_ports:
    mov eax, [ss:COMPLETED]
    mov [ss:ITERATION], eax
    call element_value
    cmp dword [cs:esi+R_KIND], 0
    je .seed
    mov eax, PORT_FILL
.seed:
    mov ebx, eax
    out DMA_CLEAR_FF, al
    xor edx, edx
.byte:
    mov eax, ebx
    out dx, al
    shr ebx, 8
    inc edx
    cmp edx, 4
    jb .byte
    out DMA_CLEAR_FF, al
    ret

debug_trap:
    push dword 0
    pushad
    mov dword [ss:EVENT], 0
    jmp check_frame
gp_fault:
    pushad
    mov dword [ss:EVENT], 1
    jmp check_frame
complete:
    push dword 0
    pushad
    mov dword [ss:EVENT], 2
check_frame:
    pushfd
    pop eax
    mov [ss:LIVE_FLAGS], eax
    cld
    mov esi, [ss:ROW]
    mov dword [ss:CHECK_ID], 1
    cmp dword [ss:EVENT], 1
    je .fault
    cmp dword [ss:EVENT], 2
    je .finished
    inc dword [ss:TRAPS]
    mov eax, [ss:TRAPS]
    mov edx, [ss:FINISH_AFTER]
    test edx, edx
    jnz .trap_limit
    cmp dword [ss:EXPECT_GP], 0
    jne unexpected
    inc edx
.trap_limit:
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    cmp eax, edx
    ja unexpected
    jmp .progress
.fault:
    mov eax, [ss:EXPECT_GP]
    mov edx, 1
    call equal
    inc dword [ss:GP_SEEN]
    mov eax, [ss:GP_SEEN]
    call equal
    mov eax, [ss:esp+F_ERROR]
    xor edx, edx
    call equal
    mov eax, [ss:TRAPS]
    mov edx, [ss:FINISH_AFTER]
    call equal
    jmp .progress
.finished:
    mov eax, [ss:GP_SEEN]
    mov edx, [ss:EXPECT_GP]
    call equal
    mov edx, [ss:FINISH_AFTER]
    cmp dword [ss:TOTAL], 0
    jne .finished_traps
    inc edx
.finished_traps:
    mov eax, [ss:TRAPS]
    call equal
.progress:
    mov eax, [ss:TRAPS]
    cmp dword [ss:TOTAL], 0
    jne .save_progress
    xor eax, eax
.save_progress:
    mov [ss:COMPLETED], eax
    mov dword [ss:CHECK_ID], 2
    xor edx, edx
    cmp dword [ss:EVENT], 1
    je .ip
    cmp dword [ss:SCENARIO], 6
    jne .normal_ip
    cmp dword [cs:esi+R_ADDRESS], 32
    je .ip
.normal_ip:
    cmp eax, [ss:TOTAL]
    jb .ip
    mov edx, [ss:NEXT_IP]
.ip:
    cmp dword [ss:EVENT], 2
    jne .check_ip
    mov edx, [ss:NEXT_IP]
    add edx, 2
.check_ip:
    mov eax, [ss:esp+F_IP]
    call equal
    mov dword [ss:CHECK_ID], 3
    movzx eax, word [ss:esp+F_CS]
    mov edx, [cs:esi+R_CS]
    call equal
    mov dword [ss:CHECK_ID], 4
    mov edx, [ss:TEST_FLAGS]
    cmp dword [ss:EVENT], 0
    je .flags
    or edx, 0x10000 ; A fault frame has RF; a trap frame does not (386).
    cmp dword [ss:EVENT], 1
    je .flags
    and edx, ~0x10100
.flags:
    and edx, FLAGS_MASK
    mov eax, [ss:esp+F_FLAGS]
    and eax, FLAGS_MASK
    call equal
    mov eax, [ss:LIVE_FLAGS]
    and eax, 0x300
    xor edx, edx
    call equal
    mov dword [ss:CHECK_ID], 5
    movzx eax, word [ss:esp+F_SS]
    mov edx, STACKSEG
    call equal
    mov dword [ss:EXPECTED_REGS], 0x89abcdef
    mov edx, [ss:START_COUNT]
    cmp dword [ss:SCENARIO], 0
    je .count
    sub edx, [ss:COMPLETED]
.count:
    mov [ss:EXPECTED_REGS+4], edx
    mov dword [ss:EXPECTED_REGS+8], 0xa5a50000
    mov dword [ss:EXPECTED_REGS+12], 0x44556677
    mov dword [ss:EXPECTED_REGS+16], USTACK
    mov dword [ss:EXPECTED_REGS+20], 0x55667788
    mov eax, [ss:COMPLETED]
    imul eax, [cs:esi+R_STEP]
    mov edx, [ss:START_SRC]
    cmp dword [cs:esi+R_KIND], 0
    je .source
    call advance_index
.source:
    mov [ss:EXPECTED_REGS+24], edx
    mov edx, [ss:START_DST]
    cmp dword [cs:esi+R_KIND], 1
    je .destination
    call advance_index
.destination:
    mov [ss:EXPECTED_REGS+28], edx
    mov dword [ss:CHECK_ID], 6
    xor edi, edi
.register:
    mov edx, 7
    sub edx, edi
    mov eax, [ss:esp+edx*4]
    cmp edi, 4
    jne .compare
    mov eax, [ss:esp+F_SP]
.compare:
    mov edx, [ss:EXPECTED_REGS+edi*4]
    call equal
    inc edi
    cmp edi, 8
    jb .register
    mov dword [ss:CHECK_ID], 7
    ; Compare every source/destination element and both boundary guards.
    mov dword [ss:ITERATION], -1
.memory:
    call memory_offsets
    call element_value
    mov ecx, eax
    mov edx, 0xcccccccc
    cmp dword [cs:esi+R_KIND], 0
    jne .bytes
    mov ebx, [ss:ITERATION]
    cmp ebx, [ss:COMPLETED]
    jae .bytes
    mov edx, eax
.bytes:
    xor ebx, ebx
.byte:
    movzx eax, byte [ss:ebp+ebx]
    mov [ss:GOT], eax
    movzx eax, cl
    mov [ss:WANT], eax
    cmp al, [ss:ebp+ebx]
    jne unexpected
    movzx eax, byte [ss:DST+edi+ebx]
    mov [ss:GOT], eax
    movzx eax, dl
    mov [ss:WANT], eax
    cmp al, [ss:DST+edi+ebx]
    jne unexpected
    shr edx, 8
    shr ecx, 8
    inc ebx
    cmp ebx, [cs:esi+R_WIDTH]
    jb .byte
    inc dword [ss:ITERATION]
    mov eax, [ss:ITERATION]
    cmp eax, [ss:TOTAL]
    jle .memory
    ; Scalar byte IN checks all four fixture bytes. No INS/OUTS is used by
    ; the oracle, including for seeding, and each transfer has distinct data.
    mov dword [ss:CHECK_ID], 8
    cmp dword [ss:EVENT], 2
    je case_done ; No new operation since the last checked trap/fault.
    mov eax, [ss:COMPLETED]
    cmp dword [ss:EVENT], 1
    je .port_element
    test eax, eax
    jz .port_element
    dec eax
.port_element:
    mov [ss:ITERATION], eax
    call element_value
    mov ebx, eax
    cmp dword [cs:esi+R_KIND], 0
    je .port_ready
    mov ebx, PORT_FILL
    cmp dword [ss:EVENT], 1
    je .port_ready
    cmp dword [ss:COMPLETED], 0
    je .port_ready
    mov ecx, [cs:esi+R_WIDTH]
    shl ecx, 3
    mov edx, -1
    cmp ecx, 32
    je .port_mask
    shl edx, cl
    not edx
.port_mask:
    and eax, edx
    not edx
    and ebx, edx
    or ebx, eax
.port_ready:
%ifdef IO_STRINGS_BAD_ORACLE
    xor ebx, 1 ; Prove that a wrong data expectation is rejected.
%endif
    out DMA_CLEAR_FF, al
    xor edi, edi
.port_byte:
    mov edx, edi
    xor eax, eax
    in al, dx
    movzx edx, bl
    call equal
    shr ebx, 8
    inc edi
    cmp edi, 4
    jb .port_byte
    cmp dword [ss:EVENT], 1
    je .skip
    mov eax, [ss:COMPLETED]
    cmp eax, [ss:FINISH_AFTER]
    jb .resume
    cmp dword [ss:SCENARIO], 12
    jne .skip
    mov byte [ss:TSS+104], 0xff ; Deny only after one completed REP element.
.resume:
    call seed_ports
    xor eax, eax
    mov dr6, eax
    popad
    add esp, 4
    iretd
.skip:
    ; High-count and permission cases intentionally abandon REP after
    ; checking its exact restart state. Normal cases already point here.
    mov eax, [ss:NEXT_IP]
    mov [ss:esp+F_IP], eax
    and dword [ss:esp+F_FLAGS], ~0x100
    popad
    add esp, 4
    iretd

advance_index:
    cmp dword [cs:esi+R_ADDRESS], 32
    jne .word
    add edx, eax
    ret
.word:
    add dx, ax ; Preserve the high half even across a 16-bit index wrap.
    ret
equal:
    mov [ss:GOT], eax
    mov [ss:WANT], edx
    cmp eax, edx
    jne unexpected
    ret
case_done:
    inc dword [ss:COUNT]
    inc dword [ss:SCENARIO]
    cmp dword [ss:SCENARIO], SCENARIOS
    jb case_begin
    mov dword [ss:SCENARIO], 0
    add dword [ss:ROW], ROW_BYTES
    cmp dword [ss:ROW], cases_end
    jb case_begin
    mov dword [ss:CHECK_ID], 9
    mov eax, [ss:COUNT]
    mov edx, TOTAL_CASES
    call equal
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
    mov esi, step_text
    call print
    mov eax, [ss:TRAPS]
    call hex32
    mov esi, got_text
    call print
    mov eax, [ss:GOT]
    call hex32
    mov esi, want_text
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
passed: db 'CPU386 IO STRINGS PASS cases=2496',10,0
failed: db 'CPU386 IO STRINGS FAIL case=',0
check_text: db ' check=',0
step_text: db ' step=',0
got_text: db ' got=',0
want_text: db ' want=',0

align 4
cases:
%assign case_count 0
%assign codebits 16
%rep 2
%assign code UCODE16
%if codebits=32
%assign code UCODE32
%endif
%assign address IO_STRINGS_FIRST_ADDRESS
%rep 2
%assign width 1
%rep 3
%assign direction 0
%rep 2
%assign step width
%if direction
%assign step -width
%endif
%assign kind 0
%rep 2
    dd code,width,address,kind,step,0,SRC
    dd code,width,address,kind,step,0x64,FSBASE
    dd code,width,address,kind,step,0x65,GSBASE
    dd code,width,address,kind,step,0x36,STACKBASE
%assign case_count case_count+4*SCENARIOS
%assign kind kind+1
%endrep
%assign direction 1
%endrep
%assign width width*2
%endrep
%assign address 48-address
%endrep
%assign codebits 32
%endrep
cases_end:
%if case_count != TOTAL_CASES
%error "Update I/O string case count and runner"
%endif

align 8
descriptors:
    dq 0
    DESC ROM, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC CODEBUF, 0xfff, 0xfa, 0
    DESC CODEBUF, 0xfff, 0xfa, 0x40
    DESC SRC, 0x1ffff, 0xf2, 0x40
    DESC DST, 0x1ffff, 0xf2, 0x40
    DESC STACKBASE, 0x1ffff, 0xf2, 0x40
    DESC FSBASE, 0x1ffff, 0xf2, 0x40
    DESC GSBASE, 0x1ffff, 0xf2, 0x40
    DESC TSS, 105, 0x89, 0
descriptors_end:
gdt_ptr: dw descriptors_end-descriptors-1
    dd GDT
idt_ptr: dw 49*8-1
    dd IDT

bits 16
times 0xfff0-($-$$) db 0xff
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
