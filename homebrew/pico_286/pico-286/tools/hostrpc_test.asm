; HOSTRPC - diagnostic program for the R36SX Pico-286 private host RPC port.
;
; This is not a DOS redirector.  It only verifies that guest DOS code can talk
; to the emulator-side host RPC device without using INT 2Fh.
;
; Build:
;   nasm -f bin hostrpc_test.asm -o hostrpc.com

    org 100h
    bits 16
    cpu 8086

PORT_BASE       equ 0E360h
PORT_ID0        equ PORT_BASE + 0
PORT_ID1        equ PORT_BASE + 1
PORT_COMMAND    equ PORT_BASE + 2
PORT_STATUS     equ PORT_BASE + 3
PORT_ADDR0      equ PORT_BASE + 4
PORT_ADDR1      equ PORT_BASE + 5
PORT_ADDR2      equ PORT_BASE + 6
PORT_ADDR3      equ PORT_BASE + 7

RPC_MAGIC       equ 05248h       ; "HR" little-endian
RPC_VERSION     equ 1
RPC_EXECUTE     equ 1

CMD_PING        equ 0
CMD_CREATE      equ 3
CMD_CLOSE       equ 4
CMD_WRITE       equ 6

REQ_MAGIC       equ 0
REQ_VERSION     equ 2
REQ_COMMAND     equ 4
REQ_FLAGS       equ 6
REQ_PATH_PHYS   equ 8
REQ_PATH2_PHYS  equ 12
REQ_DATA_PHYS   equ 16
REQ_DATA_LEN    equ 20
REQ_FILE_POS    equ 24
REQ_FILE_SIZE   equ 28
REQ_HANDLE      equ 32
REQ_MODE        equ 34
REQ_ATTR        equ 36
REQ_DOS_ERROR   equ 38
REQ_RESULT      equ 40
REQ_RESERVED    equ 42
REQ_BYTES_DONE  equ 44
REQ_SIZE        equ 48

start:
    mov dx, PORT_ID0
    in al, dx
    cmp al, 'R'
    jne rpc_missing
    mov dx, PORT_ID1
    in al, dx
    cmp al, 'H'
    jne rpc_missing

    call clear_request
    mov word [request + REQ_COMMAND], CMD_PING
    call execute_request
    call require_ok

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CREATE
    mov si, test_path
    mov di, request + REQ_PATH_PHYS
    call store_phys
    call execute_request
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
    call require_ok

    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [open_handle]
    mov [request + REQ_HANDLE], ax
    call execute_request
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
    ; Set request physical address ports.
    mov si, request
    mov di, phys_tmp
    call store_phys

    mov dx, PORT_ADDR0
    mov al, [phys_tmp]
    out dx, al
    mov dx, PORT_ADDR1
    mov al, [phys_tmp + 1]
    out dx, al
    mov dx, PORT_ADDR2
    mov al, [phys_tmp + 2]
    out dx, al
    mov dx, PORT_ADDR3
    mov al, [phys_tmp + 3]
    out dx, al

    mov dx, PORT_COMMAND
    mov al, RPC_EXECUTE
    out dx, al

    mov dx, PORT_STATUS
    in al, dx
    ret

open_handle dw 0
phys_tmp dd 0

msg_ok      db 'HOSTRPC OK: wrote HOSTRPC.TXT in the host drive.',13,10,'$'
msg_missing db 'HOSTRPC device not found on ports E360h..E36Fh.',13,10,'$'
msg_failed  db 'HOSTRPC request failed.',13,10,'$'
test_path   db 'HOSTRPC.TXT',0
test_text   db 'Hello from DOS through R36SX HOSTRPC.',13,10
test_text_end:

request:
    times REQ_SIZE db 0
