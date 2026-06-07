bits 16
org 100h

SB_BASE             equ 0220h
DSP_RESET           equ SB_BASE + 06h
DSP_READ            equ SB_BASE + 0Ah
DSP_WRITE           equ SB_BASE + 0Ch
DSP_WRITE_STATUS    equ SB_BASE + 0Ch
DSP_READ_STATUS     equ SB_BASE + 0Eh

DMA_CH1_ADDR        equ 02h
DMA_CH1_COUNT       equ 03h
DMA_CH1_PAGE        equ 83h
DMA_MASK            equ 0Ah
DMA_MODE            equ 0Bh
DMA_CLEAR_FF        equ 0Ch
DMA_CH1_MASK        equ 05h
DMA_CH1_UNMASK      equ 01h
DMA_CH1_WRITE_MODE  equ 45h
DMA_CH1_READ_MODE   equ 49h

IRQ7_VECTOR         equ 0Fh
PIC_MASK            equ 21h
PIC_EOI             equ 20h

start:
    push cs
    pop ds

    mov dx, msg_title
    call print

    call reset_dsp
    jc fail_reset
    mov dx, msg_reset
    call pass

    call test_version
    jc fail_version
    mov dx, msg_version
    call pass

    call test_ident
    jc fail_ident
    mov dx, msg_ident
    call pass

    call test_speaker
    jc fail_speaker
    mov dx, msg_speaker
    call pass

    call test_e2_dma
    jc fail_e2
    mov dx, msg_e2
    call pass

    call test_playback_irq
    jc fail_irq
    mov dx, msg_irq
    call pass

    call restore_irq7
    mov dx, msg_all_ok
    call print
    mov ax, 4C00h
    int 21h

fail_reset:
    mov dx, msg_reset
    jmp fail
fail_version:
    mov dx, msg_version
    jmp fail
fail_ident:
    mov dx, msg_ident
    jmp fail
fail_speaker:
    mov dx, msg_speaker
    jmp fail
fail_e2:
    mov dx, msg_e2
    jmp fail
fail_irq:
    mov dx, msg_irq

fail:
    push dx
    call restore_irq7
    mov dx, msg_fail
    call print
    pop dx
    call print
    mov dx, msg_crlf
    call print
    mov ax, 4C01h
    int 21h

print:
    mov ah, 09h
    int 21h
    ret

pass:
    push dx
    mov dx, msg_pass
    call print
    pop dx
    call print
    mov dx, msg_crlf
    call print
    ret

short_delay:
    push cx
    mov cx, 200
.loop:
    loop .loop
    pop cx
    ret

wait_write_ready:
    push cx
    push dx
    mov dx, DSP_WRITE_STATUS
    mov cx, 0FFFFh
.loop:
    in al, dx
    test al, 80h
    jz .ready
    loop .loop
    stc
    jmp .done
.ready:
    clc
.done:
    pop dx
    pop cx
    ret

wait_read_ready:
    push cx
    push dx
    mov dx, DSP_READ_STATUS
    mov cx, 0FFFFh
.loop:
    in al, dx
    test al, 80h
    jnz .ready
    loop .loop
    stc
    jmp .done
.ready:
    clc
.done:
    pop dx
    pop cx
    ret

write_dsp:
    push bx
    push dx
    mov bl, al
    call wait_write_ready
    jc .fail
    mov dx, DSP_WRITE
    mov al, bl
    out dx, al
    clc
    jmp .done
.fail:
    stc
.done:
    pop dx
    pop bx
    ret

read_dsp:
    push dx
    call wait_read_ready
    jc .fail
    mov dx, DSP_READ
    in al, dx
    clc
    jmp .done
.fail:
    stc
.done:
    pop dx
    ret

reset_dsp:
    mov dx, DSP_RESET
    mov al, 1
    out dx, al
    call short_delay
    xor al, al
    out dx, al
    call read_dsp
    jc .fail
    cmp al, 0AAh
    jne .fail
    clc
    ret
.fail:
    stc
    ret

test_version:
    mov al, 0E1h
    call write_dsp
    jc .fail
    call read_dsp
    jc .fail
    cmp al, 02h
    jne .fail
    call read_dsp
    jc .fail
    cmp al, 01h
    jne .fail
    clc
    ret
.fail:
    stc
    ret

test_ident:
    mov al, 0E0h
    call write_dsp
    jc .fail
    mov al, 055h
    call write_dsp
    jc .fail
    call read_dsp
    jc .fail
    cmp al, 0AAh
    jne .fail
    clc
    ret
.fail:
    stc
    ret

test_speaker:
    mov al, 0D1h
    call write_dsp
    jc .fail
    mov al, 0D8h
    call write_dsp
    jc .fail
    call read_dsp
    jc .fail
    cmp al, 0FFh
    jne .fail

    mov al, 0D3h
    call write_dsp
    jc .fail
    mov al, 0D8h
    call write_dsp
    jc .fail
    call read_dsp
    jc .fail
    cmp al, 00h
    jne .fail
    clc
    ret
.fail:
    stc
    ret

setup_dma_channel1:
    mov [dma_mode_value], al
    push ax
    push bx
    push cx
    push dx
    push si

    mov dx, DMA_MASK
    mov al, DMA_CH1_MASK
    out dx, al

    mov dx, DMA_CLEAR_FF
    xor al, al
    out dx, al

    mov ax, ds
    mov dx, ax
    mov cl, 12
    shr dx, cl
    mov cl, 4
    shl ax, cl
    add ax, bx
    adc dx, 0
    mov si, dx
    mov bx, ax

    mov dx, DMA_CH1_ADDR
    mov al, bl
    out dx, al
    mov al, bh
    out dx, al

    mov dx, DMA_CH1_PAGE
    mov ax, si
    out dx, al

    mov dx, DMA_CH1_COUNT
    xor al, al
    out dx, al
    out dx, al

    mov dx, DMA_MODE
    mov al, [dma_mode_value]
    out dx, al

    mov dx, DMA_MASK
    mov al, DMA_CH1_UNMASK
    out dx, al

    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret

test_e2_dma:
    mov byte [dma_byte], 0
    mov bx, dma_byte
    mov al, DMA_CH1_WRITE_MODE
    call setup_dma_channel1

    mov al, 0E2h
    call write_dsp
    jc .fail
    xor al, al
    call write_dsp
    jc .fail
    call short_delay
    cmp byte [dma_byte], 040h
    jne .fail
    clc
    ret
.fail:
    stc
    ret

setup_irq7:
    mov ax, 3500h + IRQ7_VECTOR
    int 21h
    mov [old_irq7_off], bx
    mov [old_irq7_seg], es

    mov dx, irq7_handler
    mov ax, 2500h + IRQ7_VECTOR
    int 21h

    in al, PIC_MASK
    mov [old_pic_mask], al
    and al, 07Fh
    out PIC_MASK, al
    mov byte [irq_installed], 1
    sti
    ret

restore_irq7:
    cmp byte [irq_installed], 0
    je .done
    cli
    mov al, [old_pic_mask]
    out PIC_MASK, al
    push ds
    mov dx, [old_irq7_off]
    mov ax, [old_irq7_seg]
    mov ds, ax
    mov ax, 2500h + IRQ7_VECTOR
    int 21h
    pop ds
    mov byte [irq_installed], 0
    sti
.done:
    ret

irq7_handler:
    push ax
    push dx
    push ds
    push cs
    pop ds
    mov dx, DSP_READ_STATUS
    in al, dx
    mov al, PIC_EOI
    out PIC_EOI, al
    mov byte [irq_seen], 1
    pop ds
    pop dx
    pop ax
    iret

test_playback_irq:
    call setup_irq7
    mov byte [irq_seen], 0
    mov byte [dma_byte], 080h
    mov bx, dma_byte
    mov al, DMA_CH1_READ_MODE
    call setup_dma_channel1

    mov al, 0D1h
    call write_dsp
    jc .fail
    mov al, 014h
    call write_dsp
    jc .fail
    xor al, al
    call write_dsp
    jc .fail
    xor al, al
    call write_dsp
    jc .fail

    mov cx, 0FFFFh
.wait_irq:
    cmp byte [irq_seen], 1
    je .ok
    call short_delay
    loop .wait_irq
.fail:
    stc
    ret
.ok:
    call restore_irq7
    clc
    ret

msg_title   db 'SBPROBE Sound Blaster DSP test', 13, 10, '$'
msg_pass    db 'OK   $'
msg_fail    db 'FAIL $'
msg_crlf    db 13, 10, '$'
msg_all_ok  db 'All Sound Blaster probe tests passed', 13, 10, '$'
msg_reset   db 'DSP reset', '$'
msg_version db 'DSP version E1h', '$'
msg_ident   db 'DSP identification E0h', '$'
msg_speaker db 'speaker status D8h', '$'
msg_e2      db 'DMA identification E2h', '$'
msg_irq     db 'DMA playback IRQ ack', '$'

old_irq7_off    dw 0
old_irq7_seg    dw 0
old_pic_mask    db 0
irq_installed   db 0
irq_seen        db 0
dma_mode_value  db 0
dma_byte        db 080h
