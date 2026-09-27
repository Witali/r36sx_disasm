; Standalone 64 KiB ROM at F0000h. No DOS, BIOS services or disk images.
; Intel 80386 PRM 9.1, 9.8.13-14 and MOV: a fault stops the instruction,
; saves its starting CS:EIP, and must not commit a failed read's destination.
; AMD APM vol.3 rev.3.19 MOV pp.213-215 agrees for legacy operand sizes.
cpu 386
bits 16
org 0

%define ROM_BASE 0xf0000
%define CODE16 8
%define CODE32 16
%define DATA 24
%define STACK 32
%define IDT 0x4000
%define PD 0x1000
%define PT 0x2000
%define STATE 0x5000
%define EXPECT_IP STATE
%define RESUME_IP STATE+4
%define EXPECT_CS STATE+8
%define EXPECT_VECTOR STATE+12
%define EXPECT_ERROR STATE+16
%define EXPECT_CR2 STATE+20
%define HITS STATE+24
%define COUNT STATE+28
%define EXPECT_FLAGS STATE+32
%define SENTINEL 0x12345678

%macro DESC 4
    dw %2 & 0xffff, %1 & 0xffff
    db (%1 >> 16) & 0xff, %3, ((%2 >> 16) & 15) | %4, (%1 >> 24) & 0xff
%endmacro

start:
    cli
    cld
    xor ax, ax
    mov ss, ax
    mov sp, 0x9000
    mov ax, 0xf000
    mov ds, ax
    lgdt [gdt_ptr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE32:setup

bits 32
setup:
    mov ax, DATA
    mov ds, ax
    mov es, ax
    mov ax, STACK
    mov ss, ax
    mov esp, 0x9000
    mov dword [COUNT], 0
    mov word [0xb8000], 0x0746 ; Visible 'F' for the framebuffer smoke check.

    ; Supervisor identity map of the first MiB, including ROM and test stack.
    mov edi, PD
    xor eax, eax
    mov ecx, 2048
    rep stosd
    mov dword [PD], PT | 3
    mov edi, PT
    mov eax, 3
    mov ecx, 256
.page:
    stosd
    add eax, 0x1000
    loop .page
    mov dword [PT+6*4], 0 ; Data fault at 6000h.
%assign page 0xf3
%rep 6
    mov dword [PT+page*4], 0 ; Immediate straddles a mapped/unmapped ROM page.
%assign page page+2
%endrep
    mov dword [PT+0xfe*4], 0 ; Opcode fetch fault at FE000h.
    mov dword [PT+0xff*4], 0 ; Next fetch after a valid instruction at FEFFFh.

    ; All unexpected exceptions fail. Expected #GP/#PF use 32-bit gates,
    ; including when the interrupted code segment has D=0.
    mov edi, IDT
    mov ecx, 32
    mov eax, (unexpected-$$) | (CODE32 << 16)
    mov edx, 0x00008e00
.gate:
    stosd
    xchg eax, edx
    stosd
    xchg eax, edx
    loop .gate
    mov word [IDT+13*8], gp_handler
    mov word [IDT+14*8], pf_handler
    mov word [IDT+8*8], df_handler
    lidt [cs:idt_ptr]
    mov eax, PD
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    jmp CODE16:tests16

; Set expectations before each fault. EAX and defined arithmetic flags must
; survive. The handler replaces only return EIP so the next case can execute.
%macro ARM 5-6 0x46
    mov dword [es:EXPECT_IP], %1
    mov dword [es:RESUME_IP], %2
    mov word [es:EXPECT_CS], %3
    mov byte [es:EXPECT_VECTOR], %4
    mov dword [es:EXPECT_ERROR], 0
    mov dword [es:EXPECT_CR2], %5
    mov dword [es:HITS], 0
    mov dword [es:EXPECT_FLAGS], %6 & 0x9d5
    mov al, 0x40
    out 0x80, al
    mov dx, 0x190
    mov al, [es:COUNT] ; Zero-based case number remains visible on failure.
    out dx, al
    mov eax, SENTINEL
    push dword %6
    popfd
%endmacro

%macro CHECK_RETURN 0
    cmp eax, SENTINEL
    jne fail_current
    cmp dword [es:HITS], 1
    jne fail_current
    inc dword [es:COUNT]
%endmacro

; All 8/16/32-bit MOV loads, both address sizes, in D=0 and D=1 code.
%macro MEMORY_CASES 1
%assign vector 13
%rep 2
    mov ax, DATA
%if vector = 13
    xor ax, ax ; Null DS yields #GP(0), not a fabricated all-ones read.
%endif
    mov ds, ax
    mov bx, 0x6000
    mov esi, 0x6000
%assign width 8
%rep 3
%assign addr 16
%rep 2
    ARM %%fault%+vector%+width%+addr, %%resume%+vector%+width%+addr, %1, vector, 0x6000
%%fault%+vector%+width%+addr:
%if width = 8
%define dst al
%elif width = 16
%define dst ax
%else
%define dst eax
%endif
%if addr = 16
    mov dst, [bx]
%else
    mov dst, [esi]
%endif
    jmp fail_current ; Fault is mandatory, even if the value happened to match.
%%resume%+vector%+width%+addr:
    CHECK_RETURN
%assign addr 32
%endrep
%assign width width*2
%endrep
%assign vector 14
%endrep
%endmacro

bits 16
%define fail_current fail16
tests16:
    MEMORY_CASES CODE16
    ARM imm8_16, after_imm8_16, CODE16, 14, 0xf3000
    jmp imm8_16
after_imm8_16:
    CHECK_RETURN
    ARM imm16_16, after_imm16_16, CODE16, 14, 0xf5000
    jmp imm16_16
after_imm16_16:
    CHECK_RETURN
    ARM imm32_16, after_imm32_16, CODE16, 14, 0xf7000
    jmp imm32_16
after_imm32_16:
    CHECK_RETURN
    jmp CODE32:tests32
fail16:
    jmp CODE32:unexpected

bits 32
%define fail_current unexpected
tests32:
    MEMORY_CASES CODE32
    ARM imm8_32, after_imm8_32, CODE32, 14, 0xf9000
    jmp imm8_32
after_imm8_32:
    CHECK_RETURN
    ARM imm16_32, after_imm16_32, CODE32, 14, 0xfb000
    jmp imm16_32
after_imm16_32:
    CHECK_RETURN
    ARM imm32_32, after_imm32_32, CODE32, 14, 0xfd000
    jmp imm32_32
after_imm32_32:
    CHECK_RETURN
    ARM 0xe000, after_opcode, CODE32, 14, 0xfe000
    jmp 0xe000
after_opcode:
    CHECK_RETURN

    ; The preceding case deliberately unmapped FE000h. Restore that page and
    ; flush the TLB before testing its last byte; only FF000h stays absent.
    mov dword [es:PT+0xfe*4], 0xfe003
    mov eax, cr3
    mov cr3, eax
    ; Logging must not manufacture #PF while peeking past a valid instruction.
    ; INC EDI at EFFFh completes; only the following fetch at F000h faults.
    ARM 0xf000, after_trace_boundary, CODE32, 14, 0xff000
    mov dword [es:EXPECT_FLAGS], 0
    mov edi, 0x3333
    jmp trace_boundary
after_trace_boundary:
    CHECK_RETURN
    cmp edi, 0x3334
    jne unexpected

    ; A faulting instruction must not run the stale single-step epilogue and
    ; deliver #DB over the #GP handler. The handler checks and then clears TF.
    xor eax, eax
    mov ds, ax
    ARM tf_fault, after_tf, CODE32, 13, 0, 0x146
tf_fault:
    mov eax, [esi]
    jmp unexpected
after_tf:
    CHECK_RETURN

    ; #GP followed by a non-present #GP gate is a contributory pair: #DF(0).
    ; Once #DF is delivered, no older delivery frame may keep changing state.
    mov byte [es:IDT+13*8+5], 0x0e
    ARM nested_fault, after_nested, CODE32, 8, 0
nested_fault:
    mov eax, [esi]
    jmp unexpected
after_nested:
    CHECK_RETURN
    mov byte [es:IDT+13*8+5], 0x8e
    ; Check that delivery bookkeeping is reusable after the nested exception.
    ARM final_fault, after_final, CODE32, 13, 0
final_fault:
    mov ax, [esi]
    jmp unexpected
after_final:
    CHECK_RETURN
    cmp dword [es:COUNT], 35
    jne unexpected
    mov esi, passed
    call print
    mov al, 0xff
    out 0x80, al
    jmp halt

; First instruction must execute at the unmodified handler EIP. Continuing
; StepIP() from an interrupted fetch used to skip bytes in this instruction.
gp_handler:
    inc dword [es:HITS]
    pushad
    mov bl, 13
    jmp check_frame
df_handler:
    inc dword [es:HITS]
    pushad
    mov bl, 8
    jmp check_frame
pf_handler:
    inc dword [es:HITS]
    pushad
    mov bl, 14
check_frame:
    cmp bl, [es:EXPECT_VECTOR]
    jne unexpected
    cmp dword [ss:esp+28], SENTINEL ; EAX saved by PUSHAD.
    jne unexpected
    mov eax, [ss:esp+32] ; Error code precedes the normal IRETD frame.
    cmp eax, [es:EXPECT_ERROR]
    jne unexpected
    cmp bl, 8
    je .skip_fault_ip ; #DF is an abort: Intel does not promise restart EIP.
    mov eax, [ss:esp+36]
    cmp eax, [es:EXPECT_IP]
    jne unexpected
.skip_fault_ip:
    mov ax, [ss:esp+40]
    cmp ax, [es:EXPECT_CS]
    jne unexpected
    mov eax, [ss:esp+44]
    and eax, 0x9d5 ; Defined arithmetic flags plus TF.
    cmp eax, [es:EXPECT_FLAGS]
    jne unexpected
    cmp bl, 14
    jne .resume
    mov eax, cr2
    mov edx, [es:EXPECT_CR2]
    cmp edx, ROM_BASE
    jb .data_address
    ; Intel 9.8.14 specifies the faulting linear address, not a fetch granule
    ; or byte order inside a multi-byte immediate. Require the missing fetch
    ; page plus exact saved EIP; data loads below still require exact CR2.
    and eax, 0xfffff000
    and edx, 0xfffff000
.data_address:
    cmp eax, edx
    jne unexpected
.resume:
    and dword [ss:esp+44], ~0x100 ; Only the faulting instruction is stepped.
    mov eax, [es:RESUME_IP]
    mov [ss:esp+36], eax
    popad
    add esp, 4
    iretd

unexpected:
    mov esi, failed
    call print
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
passed: db 'CPU386 FAULTS PASS cases=35',10,0
failed: db 'CPU386 FAULTS FAIL',10,0

align 8
gdt:
    dq 0
    DESC ROM_BASE, 0xffff, 0x9a, 0
    DESC ROM_BASE, 0xffff, 0x9a, 0x40
    DESC 0, 0xfffff, 0x92, 0xc0
    DESC 0, 0xfffff, 0x92, 0xc0
gdt_end:
gdt_ptr: dw gdt_end-gdt-1
    dd ROM_BASE+gdt
idt_ptr: dw 32*8-1
    dd IDT

; Force a byte of each immediate onto a non-present page. Prefixes are part
; of saved fault EIP. These locations are outside the normal code/data pages.
bits 16
times 0x2fff-($-$$) db 0x90
imm8_16: mov al, 0x5a
times 0x4ffe-($-$$) db 0x90
imm16_16: mov ax, 0x55aa
times 0x6ffc-($-$$) db 0x90
imm32_16: mov eax, 0x55aa55aa
bits 32
times 0x8fff-($-$$) db 0x90
imm8_32: mov al, 0x5a
times 0xaffe-($-$$) db 0x90
imm16_32: mov ax, 0x55aa
times 0xcffc-($-$$) db 0x90
imm32_32: mov eax, 0x55aa55aa
times 0xefff-($-$$) db 0x90
trace_boundary: inc edi

bits 16
times 0xfff0-($-$$) db 0xff
    jmp 0xf000:start
times 0x10000-($-$$) db 0xff
