; HOSTRPC - diagnostic program for the R36SX Pico-286 private host RPC port.
;
; This is not a DOS redirector.  It only verifies that guest DOS code can talk
; to the emulator-side host RPC device without using INT 2Fh.
;
; Build:
;   nasm -i. -f bin hostrpc_test.asm -o hostrpc.com

    org 100h
    bits 16
    cpu 8086

%include "hostrpc.inc"

ARG_PATH_MAX    equ 128
FILE_CHUNK_SIZE equ 512

start:
    cld
    push cs
    pop ds
    call parse_command_line

    call rpc_init_session
    jc rpc_missing

    cmp byte [transfer_mode], 0
    je run_self_test
    jmp transfer_file

run_self_test:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CREATE
    mov si, test_path
    mov di, request + REQ_PATH_PHYS
    call store_phys
    call execute_request
    jc rpc_request_failed
    call require_ok
    mov ax, [request + REQ_HANDLE]
    mov [open_handle], ax

    call clear_request
    mov word [request + REQ_COMMAND], CMD_WRITE
    mov ax, [open_handle]
    mov [request + REQ_HANDLE], ax
    mov word [request + REQ_DATA_LEN], test_text_end - test_text
    mov word [request + REQ_DATA_LEN + 2], 0
    mov si, test_text
    mov di, request + REQ_DATA_PHYS
    call store_phys
    call execute_request
    jc rpc_request_failed
    call require_ok

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [open_handle]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc rpc_request_failed
    call require_ok

    mov dx, msg_ok
    mov ah, 09h
    int 21h
    mov ax, 4C00h
    int 21h

transfer_file:
    mov word [dos_handle], 0FFFFh
    mov word [open_handle], 0FFFFh

    mov dx, arg_path
    mov ax, 3D00h
    int 21h
    jc dos_failed
    mov [dos_handle], ax

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CREATE
    mov si, arg_path
    mov di, request + REQ_PATH_PHYS
    call store_phys
    call execute_request
    jc rpc_transfer_failed
    call check_ok
    jc rpc_transfer_failed
    mov ax, [request + REQ_HANDLE]
    mov [open_handle], ax

    xor ax, ax
    mov [file_pos], ax
    mov [file_pos + 2], ax

.read_loop:
    mov bx, [dos_handle]
    mov dx, file_buffer
    mov cx, FILE_CHUNK_SIZE
    mov ah, 3Fh
    int 21h
    jc dos_failed
    or ax, ax
    jz .done_reading
    mov [read_count], ax

    call clear_request
    mov word [request + REQ_COMMAND], CMD_WRITE
    mov ax, [open_handle]
    mov [request + REQ_HANDLE], ax
    mov ax, [read_count]
    mov [request + REQ_DATA_LEN], ax
    mov word [request + REQ_DATA_LEN + 2], 0
    mov ax, [file_pos]
    mov [request + REQ_FILE_POS], ax
    mov ax, [file_pos + 2]
    mov [request + REQ_FILE_POS + 2], ax
    mov si, file_buffer
    mov di, request + REQ_DATA_PHYS
    call store_phys
    call execute_request
    jc rpc_transfer_failed
    call check_ok
    jc rpc_transfer_failed

    mov ax, [file_pos]
    add ax, [read_count]
    mov [file_pos], ax
    adc word [file_pos + 2], 0
    jmp .read_loop

.done_reading:
    call close_host_file
    jc rpc_transfer_failed
    call close_dos_file
    mov dx, msg_transfer_ok
    mov ah, 09h
    int 21h
    mov ax, 4C00h
    int 21h

rpc_missing:
    mov dx, msg_missing
    mov ah, 09h
    int 21h
    mov ax, 4C01h
    int 21h

require_ok:
    call check_ok
    jc rpc_request_failed
    ret

check_ok:
    cmp word [request + REQ_RESULT], 0
    jne .fail
    clc
    ret
.fail:
    stc
    ret

rpc_failed:
    mov dx, msg_failed
    mov ah, 09h
    int 21h
    mov ax, 4C02h
    int 21h

rpc_request_failed:
    cmp word [rpc_protocol_error], 0
    jne rpc_protocol_failed
    jmp rpc_failed

rpc_protocol_failed:
    mov ax, [rpc_protocol_error]
    cmp ax, PROTO_ERR_MISMATCH
    je .mismatch
    cmp ax, PROTO_ERR_TOO_LARGE
    je .too_large
    mov dx, msg_protocol_error
    jmp .print
.mismatch:
    mov dx, msg_protocol_mismatch
    jmp .print
.too_large:
    mov dx, msg_protocol_too_large
.print:
    mov ah, 09h
    int 21h
    mov ax, 4C04h
    int 21h

rpc_transfer_failed:
    call close_host_file
    call close_dos_file
    cmp word [rpc_protocol_error], 0
    jne rpc_protocol_failed
    mov dx, msg_failed
    mov ah, 09h
    int 21h
    mov ax, 4C02h
    int 21h

dos_failed:
    call close_host_file
    call close_dos_file
    mov dx, msg_dos_failed
    mov ah, 09h
    int 21h
    mov ax, 4C03h
    int 21h

parse_command_line:
    push ax
    push bx
    push cx
    push si
    push di
    mov byte [transfer_mode], 0
    mov byte [arg_path], 0
    mov si, 81h
    mov cl, [80h]
    xor ch, ch
.skip_space:
    cmp cx, 0
    je .none
    lodsb
    dec cx
    cmp al, ' '
    je .skip_space
    cmp al, 09h
    je .skip_space
    cmp al, 13
    je .none

    mov di, arg_path
    mov bx, ARG_PATH_MAX - 1
    cmp al, '"'
    je .copy_quoted_next

.copy_unquoted:
    cmp bx, 0
    je .skip_unquoted_tail
    stosb
    dec bx
.copy_unquoted_next:
    cmp cx, 0
    je .done
    lodsb
    dec cx
    cmp al, ' '
    je .done
    cmp al, 09h
    je .done
    cmp al, 13
    je .done
    jmp .copy_unquoted

.skip_unquoted_tail:
    cmp cx, 0
    je .done
    lodsb
    dec cx
    cmp al, ' '
    je .done
    cmp al, 09h
    je .done
    cmp al, 13
    je .done
    jmp .skip_unquoted_tail

.copy_quoted_next:
    cmp cx, 0
    je .done
    lodsb
    dec cx
    cmp al, '"'
    je .done
    cmp al, 13
    je .done
    cmp bx, 0
    je .copy_quoted_next
    stosb
    dec bx
    jmp .copy_quoted_next

.done:
    mov byte [di], 0
    cmp di, arg_path
    je .none
    mov byte [transfer_mode], 1
    clc
    jmp .exit
.none:
    stc
.exit:
    pop di
    pop si
    pop cx
    pop bx
    pop ax
    ret

close_host_file:
    push ax
    push bx
    mov ax, [open_handle]
    cmp ax, 0FFFFh
    je .ok
    mov bx, ax
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov [request + REQ_HANDLE], bx
    call execute_request
    jc .fail
    call check_ok
    jc .fail
    mov word [open_handle], 0FFFFh
.ok:
    clc
    jmp .done
.fail:
    stc
.done:
    pop bx
    pop ax
    ret

close_dos_file:
    push ax
    push bx
    mov bx, [dos_handle]
    cmp bx, 0FFFFh
    je .done
    mov ah, 3Eh
    int 21h
    mov word [dos_handle], 0FFFFh
.done:
    pop bx
    pop ax
    ret

clear_request:
    push ax
    push cx
    push di
    push es
    push ds
    pop es
    cld
    xor ax, ax
    mov di, request
    mov cx, REQ_SIZE / 2
    rep stosw
    mov word [request + REQ_MAGIC], RPC_MAGIC
    mov word [request + REQ_VERSION], RPC_VERSION
    pop es
    pop di
    pop cx
    pop ax
    ret

store_phys:
    ; Input: SI = near offset, DI = dword destination in the request block.
    push ax
    push bx
    push cx
    mov ax, cs
    mov bx, ax
    mov cl, 4
    shl ax, cl
    mov cl, 12
    shr bx, cl
    add ax, si
    adc bx, 0
    mov [di], ax
    mov [di + 2], bx
    pop cx
    pop bx
    pop ax
    ret

execute_request:
    ; Submit the request block through the HOSTRPC stream protocol.  The stream
    ; command is the RPC operation; the payload is this block's physical
    ; address encoded as three 15-bit frames.  The host writes fixed response
    ; fields back into this block.
    mov word [rpc_protocol_error], 0
    mov si, request
    mov di, phys_tmp
    call store_phys

    mov ax, [request + REQ_COMMAND]
    call rpc_send_command
    jc .done
    call rpc_check_protocol_error
    jc .done
    call rpc_send_phys_tmp
    jc .done
    call rpc_check_protocol_error
    jc .done
    clc
.done:
    ret

rpc_init_session:
    mov dx, PORT_DATA
    in ax, dx
    and ax, PROTO_CMD_FLAG
    mov [rpc_sync_bit], ax
    mov ax, PROTO_RESET
    call rpc_send_command
    jc .fail
    mov ax, PROTO_PING
    call rpc_send_command
    jc .fail
    and ax, PROTO_DATA_MASK
    cmp ax, RPC_VERSION
    jne .fail
    clc
    ret
.fail:
    stc
    ret

rpc_send_command:
    or ax, PROTO_CMD_FLAG
    jmp rpc_send_frame

rpc_send_data:
    and ax, PROTO_DATA_MASK

rpc_send_frame:
    push dx
    mov dx, PORT_DATA
    out dx, ax
    call rpc_wait_toggle
    pop dx
    ret

rpc_check_protocol_error:
    and ax, PROTO_DATA_MASK
    cmp ax, PROTO_ERR_TOO_LARGE
    jb .ok
    mov [rpc_protocol_error], ax
    stc
    ret
.ok:
    clc
    ret

rpc_wait_toggle:
    push bx
    push cx
    push dx
    mov bx, 0400h
.outer:
    mov cx, 0FFFFh
.poll:
    mov dx, PORT_DATA
    in ax, dx
    mov dx, ax
    xor dx, [rpc_sync_bit]
    test dx, PROTO_CMD_FLAG
    jnz .changed
    loop .poll
    dec bx
    jnz .outer
    stc
    jmp .done
.changed:
    mov dx, ax
    and dx, PROTO_CMD_FLAG
    mov [rpc_sync_bit], dx
    clc
.done:
    pop dx
    pop cx
    pop bx
    ret

rpc_send_phys_tmp:
    mov ax, [phys_tmp]
    and ax, PROTO_DATA_MASK
    call rpc_send_data
    jc .done
    call rpc_check_protocol_error
    jc .done

    mov ax, [phys_tmp]
    mov cl, 15
    shr ax, cl
    mov dx, [phys_tmp + 2]
    and dx, 03FFFh
    shl dx, 1
    or ax, dx
    call rpc_send_data
    jc .done
    call rpc_check_protocol_error
    jc .done

    mov ax, [phys_tmp + 2]
    mov cl, 14
    shr ax, cl
    call rpc_send_data
    jc .done
    call rpc_check_protocol_error
.done:
    ret

open_handle dw 0
dos_handle dw 0FFFFh
file_pos dd 0
read_count dw 0
phys_tmp dd 0
rpc_sync_bit dw 0
rpc_protocol_error dw 0
transfer_mode db 0

msg_ok      db 'HOSTRPC OK: wrote HOSTRPC.TXT in the host drive.',13,10,'$'
msg_transfer_ok db 'HOSTRPC OK: copied file to the host drive.',13,10,'$'
msg_missing db 'HOSTRPC stream device not found on port E360h.',13,10,'$'
msg_failed  db 'HOSTRPC request failed.',13,10,'$'
msg_protocol_mismatch db 'HOSTRPC protocol mismatch: command arrived before the fixed payload was complete.',13,10,'$'
msg_protocol_too_large db 'HOSTRPC protocol error: fixed-size request payload is too large.',13,10,'$'
msg_protocol_error db 'HOSTRPC protocol error.',13,10,'$'
msg_dos_failed db 'DOS file I/O failed.',13,10,'$'
test_path   db 'HOSTRPC.TXT',0
test_text   db 'Hello from DOS through R36SX HOSTRPC.',13,10
test_text_end:

arg_path:
    times ARG_PATH_MAX db 0

file_buffer:
    times FILE_CHUNK_SIZE db 0

request:
    times REQ_SIZE db 0
