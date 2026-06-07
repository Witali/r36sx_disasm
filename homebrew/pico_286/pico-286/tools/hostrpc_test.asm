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

start:
    call rpc_init_session
    jc rpc_missing

    call clear_request
    mov word [request + REQ_COMMAND], CMD_PING
    call execute_request
    jc rpc_missing
    call require_ok

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CREATE
    mov si, test_path
    mov di, request + REQ_PATH_PHYS
    call store_phys
    call execute_request
    jc rpc_failed
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
    jc rpc_failed
    call require_ok

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [open_handle]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc rpc_failed
    call require_ok

    mov dx, msg_ok
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
    cmp word [request + REQ_RESULT], 0
    jne rpc_failed
    ret

rpc_failed:
    mov dx, msg_failed
    mov ah, 09h
    int 21h
    mov ax, 4C02h
    int 21h

clear_request:
    push ax
    push cx
    push di
    xor ax, ax
    mov di, request
    mov cx, REQ_SIZE / 2
    rep stosw
    mov word [request + REQ_MAGIC], RPC_MAGIC
    mov word [request + REQ_VERSION], RPC_VERSION
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
    ; command carries a 32-bit physical pointer encoded as five 7-bit frames;
    ; the host still writes the fixed response fields back into this block.
    mov si, request
    mov di, phys_tmp
    call store_phys

    mov al, PROTO_CALL
    call rpc_send_command
    jc .done
    call rpc_send_phys_tmp
    jc .done
    and al, PROTO_DATA_MASK
    cmp al, PROTO_DATA_MASK
    jne .ok
    stc
    ret
.ok:
    clc
.done:
    ret

rpc_init_session:
    mov dx, PORT_DATA
    in al, dx
    and al, PROTO_CMD_FLAG
    mov [rpc_sync_bit], al
    mov al, PROTO_RESET
    call rpc_send_command
    jc .fail
    mov al, PROTO_PING
    call rpc_send_command
    jc .fail
    and al, PROTO_DATA_MASK
    cmp al, RPC_VERSION
    jne .fail
    clc
    ret
.fail:
    stc
    ret

rpc_send_command:
    or al, PROTO_CMD_FLAG
    jmp rpc_send_frame

rpc_send_data:
    and al, PROTO_DATA_MASK

rpc_send_frame:
    push dx
    mov dx, PORT_DATA
    out dx, al
    call rpc_wait_toggle
    pop dx
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
    in al, dx
    mov ah, al
    xor ah, [rpc_sync_bit]
    test ah, PROTO_CMD_FLAG
    jnz .changed
    loop .poll
    dec bx
    jnz .outer
    stc
    jmp .done
.changed:
    mov ah, al
    and ah, PROTO_CMD_FLAG
    mov [rpc_sync_bit], ah
    clc
.done:
    pop dx
    pop cx
    pop bx
    ret

rpc_send_phys_tmp:
    mov al, [phys_tmp]
    and al, 07Fh
    call rpc_send_data
    jc .done

    mov al, [phys_tmp]
    mov cl, 7
    shr al, cl
    mov ah, [phys_tmp + 1]
    and ah, 03Fh
    shl ah, 1
    or al, ah
    call rpc_send_data
    jc .done

    mov al, [phys_tmp + 1]
    mov cl, 6
    shr al, cl
    mov ah, [phys_tmp + 2]
    and ah, 01Fh
    mov cl, 2
    shl ah, cl
    or al, ah
    call rpc_send_data
    jc .done

    mov al, [phys_tmp + 2]
    mov cl, 5
    shr al, cl
    mov ah, [phys_tmp + 3]
    and ah, 00Fh
    mov cl, 3
    shl ah, cl
    or al, ah
    call rpc_send_data
    jc .done

    mov al, [phys_tmp + 3]
    mov cl, 4
    shr al, cl
    call rpc_send_data
.done:
    ret

open_handle dw 0
phys_tmp dd 0
rpc_sync_bit db 0

msg_ok      db 'HOSTRPC OK: wrote HOSTRPC.TXT in the host drive.',13,10,'$'
msg_missing db 'HOSTRPC stream device not found on port E360h.',13,10,'$'
msg_failed  db 'HOSTRPC request failed.',13,10,'$'
test_path   db 'HOSTRPC.TXT',0
test_text   db 'Hello from DOS through R36SX HOSTRPC.',13,10
test_text_end:

request:
    times REQ_SIZE db 0
