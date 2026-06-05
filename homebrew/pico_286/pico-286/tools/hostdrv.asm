; HOSTDRV - early DOS-side network redirector for R36SX Pico-286.
;
; This resident .COM owns INT 2Fh/AH=11h and translates DOS redirector
; callbacks into the private HOSTRPC I/O-port protocol at E360h..E36Fh.
; It is intentionally separate from MAPDRIVE.COM while the port-backed
; redirector is still being brought up.
;
; Build:
;   nasm -f bin hostdrv.asm -o hostdrv.com

    org 100h
    bits 16

PORT_BASE       equ 0E360h
PORT_ID0        equ PORT_BASE + 0
PORT_ID1        equ PORT_BASE + 1
PORT_STATUS     equ PORT_BASE + 3
PORT_ADDR0      equ PORT_BASE + 4
PORT_ADDR1      equ PORT_BASE + 5
PORT_ADDR2      equ PORT_BASE + 6
PORT_ADDR3      equ PORT_BASE + 7
PORT_COMMAND    equ PORT_BASE + 8

RPC_MAGIC       equ 05248h
RPC_VERSION     equ 1
RPC_EXECUTE     equ 1

CMD_PING        equ 0
CMD_OPEN_RO     equ 1
CMD_CREATE      equ 3
CMD_CLOSE       equ 4
CMD_READ        equ 5
CMD_WRITE       equ 6
CMD_DELETE      equ 7
CMD_MKDIR       equ 8
CMD_RMDIR       equ 9
CMD_GETATTR     equ 10
CMD_RENAME      equ 11
CMD_COMMIT      equ 12
CMD_FIND_FIRST  equ 13
CMD_FIND_NEXT   equ 14
CMD_FIND_CLOSE  equ 15

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

DEFAULT_DRIVE_LETTER equ 'H'
DEFAULT_DRIVE_NUMBER equ 7
CDS_ENTRY_SIZE       equ 058h
CDS_OFF_FLAGS        equ 043h
CDSFLAG_NET_PHY      equ 0C000h
FIRST_FILENAME_OFF   equ 09Eh
SECOND_FILENAME_OFF  equ 016Ah

SFT_TOTAL_HANDLES    equ 0
SFT_OPEN_MODE        equ 2
SFT_ATTRIBUTE        equ 4
SFT_DEVICE_INFO      equ 5
SFT_FILE_HANDLE      equ 11
SFT_FILE_TIME        equ 13
SFT_FILE_DATE        equ 15
SFT_FILE_SIZE        equ 17
SFT_FILE_POSITION    equ 21
SFT_UNK1             equ 25
SFT_UNK2             equ 27
SFT_UNK3             equ 29
SFT_UNK4             equ 31
SFT_FILE_NAME        equ 32

DTA_DRIVE            equ 0
DTA_FOUND            equ 21
FOUND_NAME           equ DTA_FOUND + 0
FOUND_ATTR           equ DTA_FOUND + 11
FOUND_TIME           equ DTA_FOUND + 22
FOUND_DATE           equ DTA_FOUND + 24
FOUND_CLUSTER        equ DTA_FOUND + 26
FOUND_SIZE           equ DTA_FOUND + 28

start:
    push cs
    pop ds

    call parse_drive_arg
    jnc .args_ok
    mov dx, err_usage
    mov ah, 09h
    int 21h
    jmp exit_error

.args_ok:
    call probe_rpc
    jc rpc_missing

    call install_cds_mapping
    jc exit_error

    ; FreeDOS/MS-DOS expose the SDA pointer through INT 21h AX=5D06h.
    mov ax, 5D06h
    int 21h
    mov ax, ds
    mov bx, si
    push cs
    pop ds
    mov [sda_seg], ax
    mov [sda_off], bx

    mov ax, 352Fh
    int 21h
    mov [old_2f], bx
    mov [old_2f + 2], es

    push cs
    pop ds
    mov dx, int2f_handler
    mov ax, 252Fh
    int 21h

    mov dx, msg_ok
    mov ah, 09h
    int 21h

    mov dx, resident_end
    add dx, 15
    shr dx, 4
    mov ax, 3100h
    int 21h

rpc_missing:
    mov dx, msg_missing
    mov ah, 09h
    int 21h

exit_error:
    mov ax, 4C01h
    int 21h

int2f_handler:
    cmp ah, 11h
    je .ours
    jmp far [cs:old_2f]

.ours:
    push bp
    mov bp, sp
    push ds
    push es
    push si
    push cs
    pop ds

    cmp ax, 1100h
    je redir_install_check
    cmp ax, 1101h
    je redir_rmdir
    cmp ax, 1103h
    je redir_mkdir
    cmp ax, 1105h
    je redir_chdir
    cmp ax, 1106h
    je redir_close
    cmp ax, 1107h
    je redir_commit
    cmp ax, 1108h
    je redir_read
    cmp ax, 1109h
    je redir_write
    cmp ax, 110Ah
    je redir_success
    cmp ax, 110Ch
    je redir_disk_info
    cmp ax, 110Eh
    je redir_success
    cmp ax, 110Fh
    je redir_getattr
    cmp ax, 1111h
    je redir_rename
    cmp ax, 1113h
    je redir_delete
    cmp ax, 1116h
    je redir_open
    cmp ax, 1117h
    je redir_create
    cmp ax, 111Bh
    je redir_find_first
    cmp ax, 111Ch
    je redir_find_next
    cmp ax, 1120h
    je redir_success
    cmp ax, 1121h
    je redir_seek_end

    pop si
    pop es
    pop ds
    pop bp
    jmp far [cs:old_2f]

redir_install_check:
    cmp bx, 0
    jne .store_sda
    cmp dx, 0
    je .done
.store_sda:
    mov [sda_seg], bx
    mov [sda_off], dx
.done:
    mov ax, 50FFh
    jmp redir_success

redir_rmdir:
    mov al, CMD_RMDIR
    call rpc_path_command
    jmp redir_from_rpc

redir_mkdir:
    mov al, CMD_MKDIR
    call rpc_path_command
    jmp redir_from_rpc

redir_chdir:
    ; DOS keeps the selected path in the CDS. HOSTRPC path resolution is
    ; root-based for now, so accepting the callback is enough for this stage.
    jmp redir_success

redir_delete:
    mov al, CMD_DELETE
    call rpc_path_command
    jmp redir_from_rpc

redir_getattr:
    mov al, CMD_GETATTR
    call rpc_path_command
    jc redir_from_rpc
    mov ax, [request + REQ_ATTR]
    mov bx, [request + REQ_FILE_SIZE + 2]
    mov di, [request + REQ_FILE_SIZE]
    mov cx, 1000h
    mov dx, 1000h
    jmp redir_success

redir_rename:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_RENAME
    mov ax, FIRST_FILENAME_OFF
    mov di, path_buf
    call copy_sda_string
    mov si, path_buf
    mov di, request + REQ_PATH_PHYS
    call store_near_phys
    mov ax, SECOND_FILENAME_OFF
    mov di, path2_buf
    call copy_sda_string
    mov si, path2_buf
    mov di, request + REQ_PATH2_PHYS
    call store_near_phys
    call execute_request
    jmp redir_from_rpc

redir_open:
    ; The current HOSTRPC open command for redirector reads existing files.
    ; Create/truncate uses AX=1117h below.
    mov al, CMD_OPEN_RO
    call rpc_open_common
    jmp redir_from_rpc

redir_create:
    mov al, CMD_CREATE
    call rpc_open_common
    jmp redir_from_rpc

redir_close:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc redir_from_rpc
    mov word [es:di + SFT_TOTAL_HANDLES], 0FFFFh
    jmp redir_from_rpc

redir_commit:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_COMMIT
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jmp redir_from_rpc

redir_read:
    mov al, CMD_READ
    call rpc_io_common
    jmp redir_from_rpc

redir_write:
    mov al, CMD_WRITE
    call rpc_io_common
    jmp redir_from_rpc

redir_disk_info:
    mov ah, 2
    mov al, 255
    mov bx, 4096
    mov cx, 4096
    mov dx, 4096
    jmp redir_success

redir_find_first:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_FIND_FIRST
    mov ax, FIRST_FILENAME_OFF
    mov di, path_buf
    call copy_sda_string
    mov si, path_buf
    mov di, request + REQ_PATH_PHYS
    call store_near_phys
    mov si, find_buf
    mov di, request + REQ_DATA_PHYS
    call store_near_phys
    mov word [request + REQ_DATA_LEN], 20
    call execute_request
    jc redir_from_rpc
    mov ax, [request + REQ_HANDLE]
    mov [active_find], ax
    call write_dta_find_result
    jmp redir_from_rpc

redir_find_next:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_FIND_NEXT
    mov ax, [active_find]
    mov [request + REQ_HANDLE], ax
    mov si, find_buf
    mov di, request + REQ_DATA_PHYS
    call store_near_phys
    mov word [request + REQ_DATA_LEN], 20
    call execute_request
    jc redir_from_rpc
    call write_dta_find_result
    jmp redir_from_rpc

redir_seek_end:
    ; CX:DX is a signed offset from EOF.  HOSTRPC keeps size in the SFT, so
    ; avoid another host round-trip and clamp negative seeks to zero.
    mov ax, [es:di + SFT_FILE_SIZE]
    mov bx, [es:di + SFT_FILE_SIZE + 2]
    add ax, dx
    adc bx, cx
    test bx, 8000h
    jz .store
    xor ax, ax
    xor bx, bx
.store:
    mov [es:di + SFT_FILE_POSITION], ax
    mov [es:di + SFT_FILE_POSITION + 2], bx
    mov dx, bx
    jmp redir_success

redir_from_rpc:
    jc redir_fail
    cmp word [request + REQ_RESULT], 0
    jne .rpc_error
    xor ax, ax
    jmp redir_success
.rpc_error:
    mov ax, [request + REQ_DOS_ERROR]
    cmp ax, 0
    jne redir_fail
    mov ax, 1
    jmp redir_fail

redir_success:
    and word [bp + 6], 0FFFEh
    jmp redir_done

redir_fail:
    or word [bp + 6], 0001h
    jmp redir_done

redir_done:
    pop si
    pop es
    pop ds
    pop bp
    iret

rpc_path_command:
    call clear_request
    xor ah, ah
    mov [request + REQ_COMMAND], ax
    mov ax, FIRST_FILENAME_OFF
    mov di, path_buf
    call copy_sda_string
    mov si, path_buf
    mov di, request + REQ_PATH_PHYS
    call store_near_phys
    call execute_request
    ret

rpc_open_common:
    push ax
    call clear_request
    pop ax
    xor ah, ah
    mov [request + REQ_COMMAND], ax
    mov ax, FIRST_FILENAME_OFF
    mov di, path_buf
    call copy_sda_string
    mov si, path_buf
    mov di, request + REQ_PATH_PHYS
    call store_near_phys
    call execute_request
    jc .done
    cmp word [request + REQ_RESULT], 0
    jne .done
    call fill_sft_from_request
.done:
    ret

rpc_io_common:
    push ax
    call clear_request
    pop ax
    xor ah, ah
    mov [request + REQ_COMMAND], ax
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    mov ax, cx
    mov [request + REQ_DATA_LEN], ax
    mov word [request + REQ_DATA_LEN + 2], 0
    mov ax, [es:di + SFT_FILE_POSITION]
    mov [request + REQ_FILE_POS], ax
    mov ax, [es:di + SFT_FILE_POSITION + 2]
    mov [request + REQ_FILE_POS + 2], ax
    call store_dta_phys
    call execute_request
    jc .done
    cmp word [request + REQ_RESULT], 0
    jne .done
    mov ax, [request + REQ_FILE_POS]
    mov [es:di + SFT_FILE_POSITION], ax
    mov ax, [request + REQ_FILE_POS + 2]
    mov [es:di + SFT_FILE_POSITION + 2], ax
    mov ax, [request + REQ_FILE_SIZE]
    mov [es:di + SFT_FILE_SIZE], ax
    mov ax, [request + REQ_FILE_SIZE + 2]
    mov [es:di + SFT_FILE_SIZE + 2], ax
    mov cx, [request + REQ_BYTES_DONE]
.done:
    ret

fill_sft_from_request:
    push ax
    push bx
    push cx
    push si
    push di
    mov word [es:di + SFT_TOTAL_HANDLES], 1
    mov word [es:di + SFT_OPEN_MODE], 0FF02h
    mov byte [es:di + SFT_ATTRIBUTE], 08h
    mov ax, [device_info]
    mov [es:di + SFT_DEVICE_INFO], ax
    mov ax, [request + REQ_HANDLE]
    mov [es:di + SFT_FILE_HANDLE], ax
    mov word [es:di + SFT_FILE_TIME], 1000h
    mov word [es:di + SFT_FILE_DATE], 1000h
    mov ax, [request + REQ_FILE_SIZE]
    mov [es:di + SFT_FILE_SIZE], ax
    mov ax, [request + REQ_FILE_SIZE + 2]
    mov [es:di + SFT_FILE_SIZE + 2], ax
    mov word [es:di + SFT_FILE_POSITION], 0
    mov word [es:di + SFT_FILE_POSITION + 2], 0
    mov word [es:di + SFT_UNK1], 0FFFFh
    mov word [es:di + SFT_UNK2], 0FFFFh
    mov word [es:di + SFT_UNK3], 0
    mov byte [es:di + SFT_UNK4], 0FFh
    lea si, [path_buf]
    add di, SFT_FILE_NAME
    call path_to_dos_name_esdi
    pop di
    pop si
    pop cx
    pop bx
    pop ax
    ret

write_dta_find_result:
    push ax
    push bx
    push cx
    push si
    push di
    push es
    call load_dta_esdi
    jc .done
    mov bx, di
    mov al, [drive_letter]
    or al, 80h
    mov [es:bx + DTA_DRIVE], al
    mov si, find_buf
    lea di, [bx + FOUND_NAME]
    mov cx, 11
    rep movsb
    mov al, [find_buf + 11]
    mov [es:bx + FOUND_ATTR], al
    mov ax, [find_buf + 12]
    mov [es:bx + FOUND_TIME], ax
    mov ax, [find_buf + 14]
    mov [es:bx + FOUND_DATE], ax
    mov word [es:bx + FOUND_CLUSTER], 0
    mov ax, [find_buf + 16]
    mov [es:bx + FOUND_SIZE], ax
    mov ax, [find_buf + 18]
    mov [es:bx + FOUND_SIZE + 2], ax
.done:
    pop es
    pop di
    pop si
    pop cx
    pop bx
    pop ax
    ret

path_to_dos_name_esdi:
    ; SI points to a NUL path, ES:DI receives an 11-byte padded DOS name.
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    mov cx, 11
    mov al, ' '
    rep stosb
    pop di
    pop si
    mov bx, si
.scan:
    lodsb
    cmp al, 0
    je .base_found
    cmp al, '\'
    je .mark
    cmp al, '/'
    jne .scan
.mark:
    mov bx, si
    jmp .scan
.base_found:
    mov si, bx
    xor cx, cx
.name:
    lodsb
    cmp al, 0
    je .done
    cmp al, '.'
    je .ext
    cmp cx, 8
    jae .name
    call upper_al
    mov [es:di], al
    inc di
    inc cx
    jmp .name
.ext:
    sub di, cx
    add di, 8
    mov cx, 0
.ext_loop:
    lodsb
    cmp al, 0
    je .done
    cmp cx, 3
    jae .ext_loop
    call upper_al
    mov [es:di], al
    inc di
    inc cx
    jmp .ext_loop
.done:
    pop dx
    pop cx
    pop bx
    pop ax
    ret

load_dta_esdi:
    mov ax, [sda_seg]
    cmp ax, 0
    je .fail
    mov es, ax
    mov di, [sda_off]
    mov ax, [es:di + 12]
    mov bx, [es:di + 14]
    mov di, ax
    mov es, bx
    clc
    ret
.fail:
    stc
    ret

store_dta_phys:
    push ax
    push bx
    push si
    push es
    call load_dta_esdi
    jc .done
    mov ax, es
    mov si, di
    mov di, request + REQ_DATA_PHYS
    call store_far_phys
.done:
    pop es
    pop si
    pop bx
    pop ax
    ret

copy_sda_string:
    ; AX = SDA-relative offset, DI = near destination in CS.
    push ax
    push bx
    push cx
    push si
    push es
    mov bx, [sda_seg]
    cmp bx, 0
    je .empty
    mov es, bx
    mov si, [sda_off]
    add si, ax
    mov cx, 127
.copy:
    mov al, [es:si]
    inc si
    stosb
    cmp al, 0
    je .done
    loop .copy
.empty:
    mov byte [di], 0
.done:
    pop es
    pop si
    pop cx
    pop bx
    pop ax
    ret

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

store_near_phys:
    ; SI = near offset in CS, DI = dword destination in request.
    push ax
    push si
    mov ax, cs
    call store_far_phys
    pop si
    pop ax
    ret

store_far_phys:
    ; AX = segment, SI = offset, DI = dword destination in request.
    push ax
    push bx
    mov bx, ax
    shl ax, 4
    shr bx, 12
    add ax, si
    adc bx, 0
    mov [di], ax
    mov [di + 2], bx
    pop bx
    pop ax
    ret

execute_request:
    push ax
    push dx
    push si
    push di
    mov si, request
    mov di, phys_tmp
    call store_near_phys
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
    cmp al, 01h
    clc
    je .done
    stc
.done:
    pop di
    pop si
    pop dx
    pop ax
    ret

probe_rpc:
    mov dx, PORT_ID0
    in al, dx
    cmp al, 'R'
    jne .fail
    mov dx, PORT_ID1
    in al, dx
    cmp al, 'H'
    jne .fail
    call clear_request
    mov word [request + REQ_COMMAND], CMD_PING
    call execute_request
    jc .fail
    cmp word [request + REQ_RESULT], 0
    jne .fail
    clc
    ret
.fail:
    stc
    ret

install_cds_mapping:
    mov ah, 52h
    int 21h
    mov si, 021h
    mov dl, [es:bx + si]
    mov si, 016h
    les bx, [es:bx + si]
    cmp bx, 0FFFFh
    jne .cds_ok
    mov ax, es
    cmp ax, 0FFFFh
    je .error_cds
.cds_ok:
    mov al, [drive_number]
    cmp al, dl
    jg .error_lastdrive
    mov di, bx
    xor ax, ax
    mov al, [drive_number]
    mov bl, CDS_ENTRY_SIZE
    mul bl
    add di, ax
    mov word [es:di + CDS_OFF_FLAGS], CDSFLAG_NET_PHY
    mov al, [drive_letter]
    mov [es:di + 0], al
    mov byte [es:di + 1], ':'
    mov byte [es:di + 2], '\'
    mov byte [es:di + 3], 0
    clc
    ret
.error_cds:
    mov dx, err_cds_fail
    mov ah, 09h
    int 21h
    stc
    ret
.error_lastdrive:
    mov dx, err_lastdrive
    mov ah, 09h
    int 21h
    stc
    ret

parse_drive_arg:
    mov byte [drive_letter], DEFAULT_DRIVE_LETTER
    mov byte [drive_number], DEFAULT_DRIVE_NUMBER
    mov si, 081h
    xor cx, cx
    mov cl, [080h]
.skip_leading:
    cmp cx, 0
    je .success
    mov al, [si]
    cmp al, ' '
    je .consume_leading
    cmp al, 09h
    jne .read_drive
.consume_leading:
    inc si
    dec cx
    jmp .skip_leading
.read_drive:
    mov al, [si]
    call upper_al
    cmp al, 'A'
    jb .fail
    cmp al, 'Z'
    ja .fail
    mov [drive_letter], al
    sub al, 'A'
    mov [drive_number], al
    inc si
    dec cx
    cmp cx, 0
    je .success
    cmp byte [si], ':'
    jne .skip_trailing
    inc si
    dec cx
.skip_trailing:
    cmp cx, 0
    je .success
    mov al, [si]
    cmp al, ' '
    je .consume_trailing
    cmp al, 09h
    jne .fail
.consume_trailing:
    inc si
    dec cx
    jmp .skip_trailing
.success:
    mov al, [drive_letter]
    mov [msg_ok_drive], al
    mov [err_lastdrive_drive], al
    mov ah, 80h
    or al, 40h
    mov [device_info], ax
    clc
    ret
.fail:
    stc
    ret

upper_al:
    cmp al, 'a'
    jb .done
    cmp al, 'z'
    ja .done
    sub al, 20h
.done:
    ret

old_2f       dd 0
sda_seg      dw 0
sda_off      dw 0
active_find  dw 0FFFFh
device_info  dw 8048h
phys_tmp     dd 0

drive_letter db DEFAULT_DRIVE_LETTER
drive_number db DEFAULT_DRIVE_NUMBER

err_usage     db 'Usage: HOSTDRV [drive:]',13,10
              db 'Example: HOSTDRV H:',13,10,'$'
err_cds_fail  db 'HOSTDRV: could not get CDS.',13,10,'$'
err_lastdrive db 'CONFIG.SYS must contain LASTDRIVE='
err_lastdrive_drive db DEFAULT_DRIVE_LETTER
              db ' or higher.',13,10,'$'
msg_missing   db 'HOSTDRV: HOSTRPC ports E360h..E36Fh not found.',13,10,'$'
msg_ok        db 'HOSTDRV: drive '
msg_ok_drive  db DEFAULT_DRIVE_LETTER
              db ': mapped through HOSTRPC ports.',13,10,'$'

path_buf      times 128 db 0
path2_buf     times 128 db 0
find_buf      times 20 db 0
request       times REQ_SIZE db 0

resident_end:
