; HOSTDRV - early DOS-side network redirector for R36SX Pico-286.
;
; This resident .COM owns INT 2Fh/AH=11h and translates DOS redirector
; callbacks into the private HOSTRPC I/O-port protocol at E360h..E36Fh.
; It is intentionally separate from MAPDRIVE.COM while the port-backed
; redirector is still being brought up.
;
; DOS side:
;   - The program patches the Current Directory Structure (CDS) entry for the
;     requested drive and then stays resident as an INT 2Fh redirector.
;   - DOS passes file operation state through SDA/SFT/DTA structures.  The
;     offsets below are the small subset used by this driver.
;
; Emulator side:
;   - The resident driver never touches host files directly.  It fills the
;     request block below, writes its physical address to HOSTRPC ports, then
;     waits for the emulator to execute the command and update the block.
;
; Keep this file 8086-compatible.  It is meant to run inside plain DOS before
; any 286/386 extender or protected-mode helper is available.
;
; Build:
;   nasm -f bin hostdrv.asm -o hostdrv.com

    org 100h
    bits 16
    cpu 8086

; HOSTRPC I/O ports.  The emulator exposes a tiny device here:
;   E360/E361 return signature bytes 'R'/'H'.
;   E364..E367 receive the physical address of the request block.
;   E368 executes the command currently stored in that request block.
PORT_BASE       equ 0E360h
PORT_ID0        equ PORT_BASE + 0
PORT_ID1        equ PORT_BASE + 1
PORT_STATUS     equ PORT_BASE + 3
PORT_ADDR0      equ PORT_BASE + 4
PORT_ADDR1      equ PORT_BASE + 5
PORT_ADDR2      equ PORT_BASE + 6
PORT_ADDR3      equ PORT_BASE + 7
PORT_COMMAND    equ PORT_BASE + 8

; Request block versioning.  RPC_MAGIC is little-endian "RH".
RPC_MAGIC       equ 05248h
RPC_VERSION     equ 1
RPC_EXECUTE     equ 1

; HOSTRPC command identifiers understood by r36sx_host_rpc.c.inl.
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

; HOSTRPC request block layout.  All pointers are real-mode physical
; addresses, not segment:offset pairs, so the emulator can read guest memory
; without knowing the caller's current segment registers.
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

; Default mapping is H:, matching the project documentation.  DOS drive
; numbers are zero-based: A=0, B=1, ..., H=7.
DEFAULT_DRIVE_LETTER equ 'H'
DEFAULT_DRIVE_NUMBER equ 7

; Current Directory Structure (CDS) fields used to mark the drive as a
; network/physical redirector drive.  DOS then routes INT 21h file operations
; for that drive through INT 2Fh/AH=11h callbacks.
CDS_ENTRY_SIZE       equ 058h
CDS_OFF_FLAGS        equ 043h
CDSFLAG_NET_PHY      equ 0C000h

; SDA filename locations for DOS 3.1+ redirector callbacks.
; RBIL names these as the "SDA first/second filename pointer".  The fields at
; 27Ch/27Eh are WORD offsets in DOS DS, not inline strings.  The buffers at
; 92h/112h are kept as fallbacks for DOS variants that do not fill the pointer
; fields.  Reading from the old inline offsets made HOSTRPC receive guest=''.
FIRST_FILENAME_OFF   equ 027Ch
SECOND_FILENAME_OFF  equ 027Eh
FIRST_FILENAME_BUF   equ 092h
SECOND_FILENAME_BUF  equ 0112h

; System File Table (SFT) fields that DOS passes in ES:DI for open files.
; HOSTDRV stores the host-side handle in SFT_FILE_HANDLE and keeps the DOS file
; position/size fields in sync after every read/write/seek.
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

; Disk Transfer Area (DTA) / Search Data Block fields used by find-first and
; find-next.  DOS shells read this buffer after successful directory searches.
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

    ; HOSTDRV is a .COM program, so DS initially points at our PSP.  Switch it
    ; to CS and parse the optional command-line drive letter from PSP:80h.
    call parse_drive_arg
    jnc .args_ok
    mov dx, err_usage
    mov ah, 09h
    int 21h
    jmp exit_error

.args_ok:
    ; Refuse to install if the emulator-side HOSTRPC device is absent.  This
    ; avoids leaving a broken redirector resident on plain DOS/other emulators.
    call probe_rpc
    jc rpc_missing

    ; Mark the selected drive in DOS CDS before hooking INT 2Fh.  Once this
    ; succeeds, DOS will call our redirector for file operations on that drive.
    call install_cds_mapping
    jc exit_error

    ; FreeDOS/MS-DOS expose the SDA pointer through INT 21h AX=5D06h.
    ; The SDA contains the redirector filenames and the current DTA pointer.
    mov ax, 5D06h
    int 21h
    mov ax, ds
    mov bx, si
    push cs
    pop ds
    mov [sda_seg], ax
    mov [sda_off], bx

    ; Chain the previous INT 2Fh handler.  Only AH=11h redirector callbacks are
    ; consumed here; everything else is forwarded unchanged.
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

    ; Stay resident.  DX is rounded up to paragraphs from PSP start, as DOS
    ; expects for INT 21h/AH=31h.
    mov dx, resident_end
    add dx, 15
    mov cl, 4
    shr dx, cl
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
    ; DOS multiplex interrupt.  Redirector calls use AH=11h; other multiplex
    ; users must continue down the old INT 2Fh chain.
    cmp ah, 11h
    je .ours
    jmp far [cs:old_2f]

.ours:
    ; Stack after INT entry and pushes:
    ;   [BP+0] old BP
    ;   [BP+2] return IP
    ;   [BP+4] return CS
    ;   [BP+6] saved FLAGS
    ; redir_success/redir_fail edit the carry bit in saved FLAGS before IRET.
    push bp
    mov bp, sp
    push ds
    push es
    push si
    push cs
    pop ds

    ; INT 2Fh/AH=11h redirector subfunctions used by DOS.  Unsupported calls
    ; are chained so another redirector may handle them.
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
    ; Installation check.  Some DOS versions also pass a refreshed SDA pointer
    ; in BX:DX, so keep it if it is non-zero.
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
    ; Attribute query returns DOS attributes in AX and file size in BX:DI.
    ; CX/DX are filled with stable placeholder time/date values for now.
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
    ; Rename has two SDA filenames: source at FIRST_FILENAME_OFF and target at
    ; SECOND_FILENAME_OFF.  Both are copied into resident near buffers before
    ; their physical addresses are placed in the HOSTRPC request.
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
    ; Close releases the host-side file handle and marks the DOS SFT slot as
    ; closed.  The host RPC layer owns the actual FILE*/descriptor lifetime.
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc redir_from_rpc
    mov word [es:di + SFT_TOTAL_HANDLES], 0FFFFh
    jmp redir_from_rpc

redir_commit:
    ; DOS commit/flush.  HOSTRPC maps this to fflush/fsync-style behavior on
    ; the host side when possible.
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
    ; Return conservative fake free-space geometry.  DOS only needs plausible
    ; non-zero values for many shell operations; real host free-space reporting
    ; can be added later through HOSTRPC if needed.
    mov ah, 2
    mov al, 255
    mov bx, 4096
    mov cx, 4096
    mov dx, 4096
    jmp redir_success

redir_find_first:
    ; Directory enumeration is stateful on the host side.  Always close any
    ; previous active find before starting a new search; shells often probe
    ; existence with find-first before copy/create operations.
    call close_active_find
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
    cmp word [request + REQ_RESULT], 0
    jne .not_found
    mov ax, [request + REQ_HANDLE]
    mov [active_find], ax
    call write_dta_find_result
    jmp redir_from_rpc
.not_found:
    ; Do not touch the DOS DTA on failed search.  Some shells inspect stale
    ; search data even after CF=1 and may report a phantom "file exists".
    mov word [active_find], 0FFFFh
    jmp redir_from_rpc

redir_find_next:
    ; Continue the active enumeration.  active_find is the opaque host handle
    ; returned by FIND_FIRST.
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
    cmp word [request + REQ_RESULT], 0
    jne .not_found
    call write_dta_find_result
    jmp redir_from_rpc
.not_found:
    ; HOSTRPC closes the host find handle when enumeration is exhausted.
    mov word [active_find], 0FFFFh
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
    ; Translate HOSTRPC completion into the DOS redirector convention.  A
    ; transport error (CF from execute_request) and a host-side DOS error in the
    ; request block both become CF=1 for the original INT 2Fh caller.
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
    ; Clear carry in the saved FLAGS image.  We cannot simply CLC before IRET
    ; because IRET restores FLAGS from the interrupt frame.
    and word [bp + 6], 0FFFEh
    jmp redir_done

redir_fail:
    ; Set carry in the saved FLAGS image and return AX as DOS error code.
    or word [bp + 6], 0001h
    jmp redir_done

redir_done:
    pop si
    pop es
    pop ds
    pop bp
    iret

rpc_path_command:
    ; Common helper for one-path operations: delete, mkdir, rmdir, getattr.
    ; AL contains the HOSTRPC command id.  DOS has already placed the path in
    ; the SDA filename buffer; HOSTRPC receives a physical pointer to path_buf.
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
    ; Common open/create path.  On success HOSTRPC returns an opaque handle and
    ; file size.  fill_sft_from_request publishes that state to DOS in ES:DI.
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
    ; Common read/write path.  DOS passes byte count in CX and the current SFT
    ; in ES:DI.  The actual transfer buffer is the current DTA pointer stored in
    ; the SDA, so store_dta_phys converts that far pointer for HOSTRPC.
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

close_active_find:
    ; Best-effort cleanup for an outstanding host find handle.  Errors are
    ; deliberately ignored here because this is called before starting another
    ; search and during recovery from failed enumeration.
    push ax
    mov ax, [active_find]
    cmp ax, 0FFFFh
    je .done
    call clear_request
    mov word [request + REQ_COMMAND], CMD_FIND_CLOSE
    mov [request + REQ_HANDLE], ax
    call execute_request
    mov word [active_find], 0FFFFh
.done:
    pop ax
    ret

fill_sft_from_request:
    ; Populate the DOS System File Table entry for a file opened by HOSTRPC.
    ; The exact SFT layout varies slightly between DOS versions, but these
    ; fields are stable enough for FreeDOS/MS-DOS style redirector callbacks.
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
    ; Copy the compact 20-byte HOSTRPC find result into the DOS DTA/SDB format.
    ; find_buf layout:
    ;   00..10 = 8.3 filename, 11 = attr, 12..15 = time/date,
    ;   16..19 = little-endian file size.
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
    ; Load ES:DI with the caller's current DTA pointer from the SDA.  DOS uses
    ; this same pointer for file transfer buffers in redirector read/write
    ; calls and for search results in find-first/find-next.
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
    ; Convert the DTA far pointer to a physical address and store it in the
    ; request block as DATA_PHYS.
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
    ; AX = SDA-relative offset of a WORD filename pointer, DI = destination.
    ; The pointer value is an offset in DOS DS, the same segment that contains
    ; the SDA.  DOS redirector docs describe this as "SDA first filename
    ; pointer", so the string is not located at SDA+AX directly.
    ;
    ; If a DOS variant leaves that pointer empty, fall back to the known inline
    ; filename buffers in the SDA.  This keeps HOSTDRV usable on DOS 3.x-like
    ; layouts while still following the documented redirector convention.
    ;
    ; The 127-byte limit keeps path_buf/path2_buf NUL-terminated and avoids
    ; scanning unbounded DOS memory if the SDA content is malformed.
    push ax
    push bx
    push cx
    push dx
    push si
    push es
    mov bx, [sda_seg]
    cmp bx, 0
    je .empty
    mov es, bx
    mov si, [sda_off]
    add si, ax
    mov dx, ax
    mov si, [es:si]
    cmp si, 0
    je .fallback
    cmp si, 0FFFFh
    jne .copy_ready
.fallback:
    mov si, [sda_off]
    cmp dx, FIRST_FILENAME_OFF
    jne .second_name
    add si, FIRST_FILENAME_BUF
    jmp .copy_ready
.second_name:
    add si, SECOND_FILENAME_BUF
.copy_ready:
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
    pop dx
    pop cx
    pop bx
    pop ax
    ret

clear_request:
    ; Reset the request block before each HOSTRPC operation.  This prevents
    ; stale path/data/handle fields from leaking between redirector callbacks.
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
    ; Physical = segment * 16 + offset.  The split shift keeps the code valid
    ; on 8086, where 32-bit arithmetic and 80186+ shifts are not available.
    push ax
    push bx
    push cx
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
    ; Submit the current request block to the emulator.  The protocol is
    ; synchronous: after OUT PORT_COMMAND the emulator has already updated the
    ; request block and status port by the time we read PORT_STATUS.
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
    ; Verify that the emulator exposes HOSTRPC and that the request/response
    ; path is usable before installing a resident redirector.
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
    ; INT 21h/AH=52h returns the DOS List of Lists.  From there we locate the
    ; CDS array and patch the selected drive entry to mark it as a redirected
    ; network/physical drive.  LASTDRIVE must include the requested letter.
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
    ; Parse optional "X:" from the .COM command tail at PSP:80h.  No DOS 2+
    ; argument parser is used, keeping the resident loader tiny and portable.
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
