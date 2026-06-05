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
;   nasm -i. -f bin hostdrv.asm -o hostdrv.com

    org 100h
    bits 16
    cpu 8086

%include "hostrpc.inc"

; HOSTRPC currently exposes 16 file handles.  HOSTDRV mirrors handle ownership
; with a tiny SFT sidecar table so later callbacks can distinguish our SFTs
; from another redirector's SFT even if DOS rewrites the device-info word.
HOSTDRV_MAX_HANDLES equ HOSTRPC_MAX_HANDLES

; DOS error codes returned through redirected INT 2Fh callbacks.
DOS_ERR_INVALID_FUNCTION equ 1
DOS_ERR_FILE_NOT_FOUND   equ 2
DOS_ERR_INVALID_HANDLE   equ 6
DOS_ERR_NO_MORE_FILES    equ 18
DOS_ERR_FILE_EXISTS      equ 80

; Default mapping is H:, matching the project documentation.  DOS drive
; numbers are zero-based: A=0, B=1, ..., H=7.
DEFAULT_DRIVE_LETTER equ 'H'
DEFAULT_DRIVE_NUMBER equ 7

; Our handler keeps the original INT frame on the DOS stack.  The first
; redirector stack parameter therefore starts after old BP, IP, CS and FLAGS.
REDIR_STACK_PARAM1   equ 8

; Current Directory Structure (CDS) fields used to mark the drive as a
; network/physical redirector drive.  DOS then routes INT 21h file operations
; for that drive through INT 2Fh/AH=11h callbacks.
CDS_ENTRY_SIZE       equ 058h
CDS_OFF_FLAGS        equ 043h
CDSFLAG_NET_PHY      equ 0C000h

; SDA inline filename buffers for DOS 4+ redirector callbacks.  This matches
; the previous emulator-owned INT 2Fh redirector (`network-redirector.c.inl`).
; DOS 3.x uses slightly earlier fields (FN1 at 92h), but the Pico-286 FreeDOS
; image follows the DOS 4+ layout used here.
FIRST_FILENAME_OFF   equ 09Eh
SECOND_FILENAME_OFF  equ 016Ah
; INT 21h/AX=6C00h stores its extended-open parameters in the DOS 4+ SDA.
; DOS forwards them to redirectors through INT 2Fh/AX=112Eh.
SDA_EXT_OPEN_ACTION  equ 02DDh
SDA_EXT_OPEN_ATTR    equ 02DFh
SDA_EXT_OPEN_MODE    equ 02E1h

; INT 21h/AX=6C00h success status returned in CX.
EXT_STATUS_OPENED    equ 1
EXT_STATUS_CREATED   equ 2
EXT_STATUS_REPLACED  equ 3

; System File Table (SFT) fields that DOS passes in ES:DI for open files.
; HOSTDRV stores the host-side handle in SFT_FILE_HANDLE and keeps the DOS file
; position/size fields in sync after every read/write/seek.
SFT_TOTAL_HANDLES    equ 0
SFT_OPEN_MODE        equ 2
SFT_ATTRIBUTE        equ 4
SFT_DEVICE_INFO      equ 5
SFT_UNK0             equ 7
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
SFT_REFCOUNT_FREE    equ 0FFFFh

; Disk Transfer Area (DTA) / Search Data Block fields used by find-first and
; find-next.  The first 21 bytes are redirector-owned search state; DOS shells
; read the standard found-file data that starts at DTA_FOUND.
DTA_DRIVE            equ 0
DTA_FIND_HANDLE      equ 13
DTA_FIND_MAGIC       equ 15
DTA_FIND_MAGIC_VALUE equ 04852h
DTA_INVALID_HANDLE   equ 0FFFFh
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
    ; Stack after INT entry and BP setup:
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
    ; SHSUCDX restores the original register set when it chains a callback to
    ; the previous INT 2Fh handler.  HOSTDRV's ownership checks also touch
    ; general registers, so keep a private copy for redir_chain.  On handled
    ; callbacks redir_done discards these saved values and returns the handler's
    ; documented AX/BX/CX/DX/DI results instead.
    push ax
    push bx
    push cx
    push dx
    push di
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
    ; DOS termination cleanup callbacks.  HOSTDRV handles them only when its
    ; sidecar tables show open host resources, then chains onward so other
    ; redirectors still see the same notification.
    cmp ax, 111Dh
    je redir_process_cleanup
    cmp ax, 1122h
    je redir_process_cleanup
    cmp ax, 1120h
    je redir_success
    cmp ax, 1121h
    je redir_seek_end
    cmp ax, 112Eh
    je redir_ext_open_create

    jmp redir_chain

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
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov al, CMD_RMDIR
    call rpc_path_command
    jmp redir_from_rpc

redir_mkdir:
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov al, CMD_MKDIR
    call rpc_path_command
    jmp redir_from_rpc

redir_chdir:
    ; Keep current-directory state in the HOSTRPC device.  It then applies the
    ; mapped drive cwd to later relative paths before resolving them on host.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov al, CMD_CHDIR
    call rpc_path_command
    jmp redir_from_rpc

redir_delete:
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov al, CMD_DELETE
    call rpc_path_command
    jmp redir_from_rpc

redir_getattr:
    ; Attribute query returns DOS attributes in AX and file size in BX:DI.
    ; CX/DX are filled with stable placeholder time/date values for now.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov al, CMD_GETATTR
    call rpc_path_command
    jc redir_from_rpc
    ; Do not return stale attr/size from an earlier successful request.  DOS
    ; shells use GETATTR as the "does destination exist?" probe before copy.
    cmp word [request + REQ_RESULT], 0
    jne redir_from_rpc
    mov ax, [request + REQ_ATTR]
    mov bx, [request + REQ_FILE_SIZE + 2]
    mov di, [request + REQ_FILE_SIZE]
    mov cx, [request + REQ_DOS_TIME]
    cmp cx, 0
    jne .time_ready
    mov cx, 1000h
.time_ready:
    mov dx, [request + REQ_DOS_DATE]
    cmp dx, 0
    jne .date_ready
    mov dx, 1000h
.date_ready:
    jmp redir_success

redir_rename:
    ; Rename has two SDA filenames: source at FIRST_FILENAME_OFF and target at
    ; SECOND_FILENAME_OFF.  Both are copied into resident near buffers before
    ; their physical addresses are placed in the HOSTRPC request.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov ax, SECOND_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
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
    ; RBIL: AX=1116h receives the DOS access/share mode as a stack word.
    ; Access bits 0..2 match INT 21h/AH=3Dh: 0=read, 1=write, 2=read/write,
    ; 3=internal EXEC/case-sensitive open.  Only true write-capable opens use
    ; HOSTRPC OPEN_RW; EXEC/internal mode remains read-only.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov bx, [bp + REDIR_STACK_PARAM1]
    call select_open_command_from_mode
    call rpc_open_common
    jmp redir_from_rpc

redir_create:
    ; RBIL: AX=1117h receives a create-mode stack word.  The low byte is the
    ; file attributes; high byte 01h means "create new" and must fail if the
    ; file already exists.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov bx, [bp + REDIR_STACK_PARAM1]
    mov al, CMD_CREATE
    call rpc_open_common
    jmp redir_from_rpc

redir_ext_open_create:
    ; DOS 4+ extended open/create (INT 21h/AX=6C00h) reaches redirectors here.
    ; RBIL documents the action/mode fields in the SDA and the create
    ; attributes on the redirector stack.  The SDA attribute is preferred
    ; because some DOS versions pass a stale stack attribute for action 11h.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    mov ax, SDA_EXT_OPEN_ACTION
    call load_sda_word
    jc .bad_sda
    mov [ext_open_action], ax
    mov ax, SDA_EXT_OPEN_MODE
    call load_sda_word
    jc .bad_sda
    mov [ext_open_mode], ax
    mov ax, SDA_EXT_OPEN_ATTR
    call load_sda_word
    jnc .have_attr
    mov ax, [bp + REDIR_STACK_PARAM1]
.have_attr:
    ; INT 21h/6C create attributes use only bits 0..5.  Keep reserved bits out
    ; of HOSTRPC and the SFT so DOS does not see impossible file attributes.
    and ax, 003Fh
    mov [ext_open_attr], ax

    ; Probe existence first, then apply the extended action matrix:
    ; low nibble = if file exists, high nibble = if file does not exist.
    mov al, CMD_GETATTR
    call rpc_path_command
    jc redir_from_rpc
    cmp word [request + REQ_RESULT], 0
    jne .missing

.exists:
    mov ax, [ext_open_action]
    and al, 0Fh
    cmp al, 0
    je .exists_fail
    cmp al, 1
    je .exists_open
    cmp al, 2
    je .exists_replace
    jmp .bad_action

.exists_open:
    mov bx, [ext_open_mode]
    call select_open_command_from_mode
    mov word [ext_open_status], EXT_STATUS_OPENED
    call rpc_open_common
    mov cx, [ext_open_status]
    jmp redir_from_rpc

.exists_replace:
    mov bx, [ext_open_attr]
    mov al, CMD_CREATE
    mov word [ext_open_status], EXT_STATUS_REPLACED
    call rpc_open_common
    mov cx, [ext_open_status]
    jmp redir_from_rpc

.exists_fail:
    mov ax, DOS_ERR_FILE_EXISTS
    jmp redir_fail

.missing:
    mov ax, [request + REQ_DOS_ERROR]
    cmp ax, 0
    jne .store_missing_error
    mov ax, DOS_ERR_FILE_NOT_FOUND
.store_missing_error:
    mov [ext_open_missing_error], ax
    mov ax, [ext_open_action]
    and al, 0F0h
    cmp al, 0
    je .missing_fail
    cmp al, 10h
    je .missing_create
    jmp .bad_action

.missing_create:
    mov bx, [ext_open_attr]
    mov al, CMD_CREATE
    mov word [ext_open_status], EXT_STATUS_CREATED
    call rpc_open_common
    mov cx, [ext_open_status]
    jmp redir_from_rpc

.missing_fail:
    mov ax, [ext_open_missing_error]
    jmp redir_fail

.bad_sda:
    mov ax, DOS_ERR_INVALID_FUNCTION
    jmp redir_fail

.bad_action:
    mov ax, DOS_ERR_INVALID_FUNCTION
    jmp redir_fail

redir_process_cleanup:
    ; AX=1122h/111Dh are process-termination cleanup notifications.  During
    ; HOSTDRV installation DOS may call them before any host resource exists;
    ; in that case do not touch HOSTRPC.  If we do own resources, close them
    ; and clear sidecar state, then chain so older redirectors get notified.
    cmp word [host_file_count], 0
    jne .cleanup
    cmp word [host_find_count], 0
    jne .cleanup
    jmp redir_chain
.cleanup:
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es
    call close_all_host_resources
    call clear_host_tracking
    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    jmp redir_chain

redir_close:
    ; RBIL documents AX=1106h as a redirector close callback where the
    ; redirector must update the SFT open count.  DOS can have several JFT
    ; handles pointing at one SFT, so only the final reference releases the
    ; HOSTRPC file handle.  Earlier closes simply decrement the SFT refcount.
    call sft_is_ours
    jc redir_chain
    mov ax, [es:di + SFT_TOTAL_HANDLES]
    cmp ax, SFT_REFCOUNT_FREE
    je .invalid_sft
    cmp ax, 0
    je .invalid_sft
    cmp ax, 1
    je .last_reference
    dec ax
    mov [es:di + SFT_TOTAL_HANDLES], ax
    xor ax, ax
    jmp redir_success

.last_reference:
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc redir_from_rpc
    cmp word [request + REQ_RESULT], 0
    jne redir_from_rpc
    call track_close_sft_from_request
    mov word [es:di + SFT_TOTAL_HANDLES], SFT_REFCOUNT_FREE
    jmp redir_from_rpc

.invalid_sft:
    mov ax, DOS_ERR_INVALID_HANDLE
    jmp redir_fail

redir_commit:
    ; DOS commit/flush.  HOSTRPC maps this to fflush/fsync-style behavior on
    ; the host side when possible.
    call sft_is_ours
    jc redir_chain
    call clear_request
    mov word [request + REQ_COMMAND], CMD_COMMIT
    mov ax, [es:di + SFT_FILE_HANDLE]
    mov [request + REQ_HANDLE], ax
    call execute_request
    jmp redir_from_rpc

redir_read:
    call sft_is_ours
    jc redir_chain
    mov al, CMD_READ
    call rpc_io_common
    jmp redir_from_rpc

redir_write:
    call sft_is_ours
    jc redir_chain
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
    ; Directory enumeration is stateful on the host side.  DOS redirectors keep
    ; the opaque continuation state in the DTA/SDB, not in one process-global
    ; variable, so repeated or nested searches do not trample each other.
    mov ax, FIRST_FILENAME_OFF
    call sda_path_is_ours
    jc redir_chain
    call close_dta_find_state
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
    call write_dta_find_result
    call track_find_open
    jmp redir_from_rpc
.not_found:
    ; Leave the user-visible found-file area untouched, but invalidate our
    ; hidden SDB continuation state so a later FIND_NEXT cannot reuse a stale
    ; host handle after a failed FIND_FIRST.
    call invalidate_dta_find_state
    jmp redir_from_rpc

redir_find_next:
    ; Continue the enumeration identified by the current DTA/SDB.  This mirrors
    ; SHSUCDX-style redirectors where the DTA owns the find continuation state.
    call dta_find_state_is_ours
    jc redir_chain
    call load_dta_find_handle
    jc .invalid_dta
    call clear_request
    mov word [request + REQ_COMMAND], CMD_FIND_NEXT
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
    cmp word [request + REQ_DOS_ERROR], DOS_ERR_NO_MORE_FILES
    jne .keep_find_count
    call track_find_close
.keep_find_count:
    call invalidate_dta_find_state
    jmp redir_from_rpc
.invalid_dta:
    mov ax, DOS_ERR_NO_MORE_FILES
    jmp redir_fail

redir_seek_end:
    ; CX:DX is a signed offset from EOF.  HOSTRPC keeps size in the SFT, so
    ; avoid another host round-trip and clamp negative seeks to zero.
    call sft_is_ours
    jc redir_chain
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
    add sp, 10
    pop si
    pop es
    pop ds
    pop bp
    iret

redir_chain:
    ; This AH=11h callback is not for our mapped drive or SFT.  Restore the
    ; caller's saved registers and continue through the redirector chain.
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    pop si
    pop es
    pop ds
    pop bp
    jmp far [cs:old_2f]

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

select_open_command_from_mode:
    ; BX = DOS open mode, return AL = HOSTRPC open command.  Preserve BX so the
    ; caller can pass the exact documented mode word through REQ_MODE.
    push dx
    mov al, CMD_OPEN_RO
    mov dx, bx
    and dl, 07h
    cmp dl, 1
    je .write_open
    cmp dl, 2
    jne .done
.write_open:
    mov al, CMD_OPEN_RW
.done:
    pop dx
    ret

rpc_open_common:
    ; Common open/create path.  On success HOSTRPC returns an opaque handle and
    ; file size.  AL contains the HOSTRPC command and BX contains the DOS
    ; open/create mode word.  fill_sft_from_request publishes the state to DOS
    ; in ES:DI.
    push ax
    call clear_request
    pop ax
    push bx
    push dx
    xor ah, ah
    mov [request + REQ_COMMAND], ax
    mov [request + REQ_MODE], bx
    mov word [request + REQ_FLAGS], 0
    mov word [request + REQ_ATTR], 0
    ; MSCDEX/SHSUCDX complete redirected SFTs with mode 0002h.  Keep that
    ; DOS-facing marker stable, while passing the caller's real access/share
    ; or create-mode word to HOSTRPC through REQ_MODE/REQ_FLAGS.
    mov word [sft_open_mode], 0002h
    mov word [sft_file_attr], 0020h
    cmp word [request + REQ_COMMAND], CMD_CREATE
    jne .params_ready
    ; Create/truncate: low byte is DOS file attributes, high byte selects
    ; normal create/truncate (00h) vs create-new (01h).
    mov dx, bx
    xor bh, bh
    mov [request + REQ_ATTR], bx
    mov [sft_file_attr], bx
    cmp dh, 01h
    jne .params_ready
    mov word [request + REQ_FLAGS], REQ_FLAG_CREATE_NEW
.params_ready:
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
    call track_open_sft
    clc
.done:
    pop dx
    pop bx
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

sda_path_is_ours:
    ; AX = SDA-relative path offset.  If DOS supplied an explicit "X:" prefix,
    ; only handle the path when X matches our mapped drive.  Root-relative
    ; names are accepted because DOS may already have selected the target CDS
    ; before entering the redirector callback.
    push bx
    push es
    mov bx, [sda_seg]
    cmp bx, 0
    je .not_ours
    mov es, bx
    mov bx, [sda_off]
    add bx, ax
    mov al, [es:bx]
    call upper_al
    cmp al, 'A'
    jb .ours
    cmp al, 'Z'
    ja .ours
    cmp byte [es:bx + 1], ':'
    jne .ours
    cmp al, [drive_letter]
    jne .not_ours
.ours:
    clc
    jmp .done
.not_ours:
    stc
.done:
    pop es
    pop bx
    ret

sft_is_ours:
    ; HOSTDRV stamps SFT_DEVICE_INFO when opening a host-backed file, but DOS
    ; redirector fields are partly undocumented and can be rewritten by DOS.
    ; The sidecar table is authoritative: it maps the HOSTRPC handle stored in
    ; the SFT back to the exact ES:DI SFT address that HOSTDRV opened.
    push ax
    push bx
    push cx
    mov ax, [es:di + SFT_FILE_HANDLE]
    cmp ax, HOSTDRV_MAX_HANDLES
    jae .check_device_word
    mov bx, ax
    shl bx, 1
    mov ax, [host_sft_seg + bx]
    cmp ax, 0
    je .check_device_word
    mov cx, es
    cmp ax, cx
    jne .check_device_word
    mov ax, [host_sft_off + bx]
    cmp ax, di
    jne .check_device_word
    clc
    jmp .done
.check_device_word:
    ; Keep the device-info word as a compatibility fallback for older SFTs
    ; that were opened before sidecar tracking was introduced.
    mov ax, [es:di + SFT_DEVICE_INFO]
    cmp ax, [device_info]
    jne .not_ours
    clc
    jmp .done
.not_ours:
    stc
.done:
    pop cx
    pop bx
    pop ax
    ret

dta_find_state_is_ours:
    ; Find-next has no path parameter, so ownership comes from the hidden
    ; DTA/SDB fields written by write_dta_find_result after our find-first.
    push ax
    push di
    push es
    call load_dta_esdi
    jc .not_ours
    call dta_drive_marker
    cmp [es:di + DTA_DRIVE], al
    jne .not_ours
    cmp word [es:di + DTA_FIND_MAGIC], DTA_FIND_MAGIC_VALUE
    jne .not_ours
    clc
    jmp .done
.not_ours:
    stc
.done:
    pop es
    pop di
    pop ax
    ret

close_dta_find_state:
    ; Best-effort cleanup for the host find handle stored in the current DTA.
    ; Errors are deliberately ignored here because this runs before a new
    ; search reuses the same DTA/SDB.
    push ax
    call load_dta_find_handle
    jc .done
    call clear_request
    mov word [request + REQ_COMMAND], CMD_FIND_CLOSE
    mov [request + REQ_HANDLE], ax
    call execute_request
    jc .invalidate
    call track_find_close
.invalidate:
    call invalidate_dta_find_state
.done:
    pop ax
    ret

track_open_sft:
    ; Remember which live SFT owns each HOSTRPC file handle.  Process cleanup
    ; uses this sidecar state to decide whether AX=1122h/111Dh belongs to us,
    ; and sft_is_ours uses it to avoid stealing callbacks from another driver.
    push ax
    push bx
    mov ax, [es:di + SFT_FILE_HANDLE]
    cmp ax, HOSTDRV_MAX_HANDLES
    jae .done
    mov bx, ax
    shl bx, 1
    cmp word [host_sft_seg + bx], 0
    jne .store
    inc word [host_file_count]
.store:
    mov ax, es
    mov [host_sft_seg + bx], ax
    mov [host_sft_off + bx], di
.done:
    pop bx
    pop ax
    ret

track_close_sft_from_request:
    ; A successful HOSTRPC close means the host handle in REQ_HANDLE is no
    ; longer valid.  Clear only that slot so repeated close callbacks cannot
    ; underflow the resident resource counter.
    push ax
    push bx
    mov ax, [request + REQ_HANDLE]
    cmp ax, HOSTDRV_MAX_HANDLES
    jae .done
    mov bx, ax
    shl bx, 1
    cmp word [host_sft_seg + bx], 0
    je .done
    mov word [host_sft_seg + bx], 0
    mov word [host_sft_off + bx], 0
    cmp word [host_file_count], 0
    je .done
    dec word [host_file_count]
.done:
    pop bx
    pop ax
    ret

track_find_open:
    ; Find handles are stored in DTAs rather than SFTs.  A count is enough for
    ; safe process cleanup because HOSTRPC closes every live find handle on
    ; CMD_CLOSE_ALL.
    inc word [host_find_count]
    ret

track_find_close:
    cmp word [host_find_count], 0
    je .done
    dec word [host_find_count]
.done:
    ret

close_all_host_resources:
    ; Last-resort cleanup used from DOS process-termination notifications.
    ; The emulator owns the actual FILE*/find handles, so a single HOSTRPC
    ; command can close anything still associated with this HOSTDRV instance.
    call clear_request
    mov word [request + REQ_COMMAND], CMD_CLOSE_ALL
    call execute_request
    ret

clear_host_tracking:
    ; Clear the resident sidecar tables after HOSTRPC has closed every handle.
    ; ES is made CS-local for rep stosw and restored for the redirector chain.
    push ax
    push cx
    push di
    push es
    mov word [host_file_count], 0
    mov word [host_find_count], 0
    push cs
    pop es
    xor ax, ax
    mov di, host_sft_seg
    mov cx, HOSTDRV_MAX_HANDLES * 2
    rep stosw
    pop es
    pop di
    pop cx
    pop ax
    ret

load_dta_find_handle:
    ; Return AX = HOSTRPC find handle if the current DTA belongs to HOSTDRV.
    ; The first byte keeps the zero-based DOS drive number with the network
    ; search-state bits set, matching SHSUCDX's SDB.DriveLet format.
    push bx
    push di
    push es
    call load_dta_esdi
    jc .fail
    call dta_drive_marker
    cmp [es:di + DTA_DRIVE], al
    jne .fail
    cmp word [es:di + DTA_FIND_MAGIC], DTA_FIND_MAGIC_VALUE
    jne .fail
    mov ax, [es:di + DTA_FIND_HANDLE]
    cmp ax, DTA_INVALID_HANDLE
    je .fail
    clc
    jmp .done
.fail:
    stc
.done:
    pop es
    pop di
    pop bx
    ret

invalidate_dta_find_state:
    ; Clear only HOSTDRV's hidden continuation fields.  The visible
    ; found-file bytes at DTA+21 stay intact for DOS programs that inspect
    ; stale data after CF=1.
    push ax
    push di
    push es
    call load_dta_esdi
    jc .done
    mov word [es:di + DTA_FIND_HANDLE], DTA_INVALID_HANDLE
    mov word [es:di + DTA_FIND_MAGIC], 0
.done:
    pop es
    pop di
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
    ; INT 2Fh/1116h and 1117h require the redirector to fill the SFT except
    ; for the handle count; DOS owns SFT_TOTAL_HANDLES until close.
    mov ax, [sft_open_mode]
    mov [es:di + SFT_OPEN_MODE], ax
    mov ax, [request + REQ_ATTR]
    cmp al, 0
    jne .attr_ready
    mov ax, [sft_file_attr]
    cmp al, 0
    jne .attr_ready
    mov al, 20h
.attr_ready:
    mov [es:di + SFT_ATTRIBUTE], al
    mov ax, [device_info]
    mov [es:di + SFT_DEVICE_INFO], ax
    ; Match the old emulator-owned redirector's DOS 4+ SFT layout: bytes
    ; +7..+10 are an undocumented redirector field and must be cleared before
    ; DOS starts routing later read/write/close callbacks through this entry.
    mov word [es:di + SFT_UNK0], 0
    mov word [es:di + SFT_UNK0 + 2], 0
    mov ax, [request + REQ_HANDLE]
    mov [es:di + SFT_FILE_HANDLE], ax
    mov ax, [request + REQ_DOS_TIME]
    cmp ax, 0
    jne .time_ready
    mov ax, 1000h
.time_ready:
    mov word [es:di + SFT_FILE_TIME], ax
    mov ax, [request + REQ_DOS_DATE]
    cmp ax, 0
    jne .date_ready
    mov ax, 1000h
.date_ready:
    mov word [es:di + SFT_FILE_DATE], ax
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
    call dta_drive_marker
    mov [es:bx + DTA_DRIVE], al
    mov ax, [request + REQ_HANDLE]
    mov [es:bx + DTA_FIND_HANDLE], ax
    mov word [es:bx + DTA_FIND_MAGIC], DTA_FIND_MAGIC_VALUE
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

dta_drive_marker:
    ; SHSUCDX writes SDB.DriveLet as drive_number | C0h.  Keep the same shape:
    ; bit 7 marks a redirector search state and the low bits remain A=0..Z=25.
    mov al, [drive_number]
    or al, 0C0h
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

load_sda_word:
    ; AX = SDA-relative offset, returns AX = word from DOS' SDA.  ES is
    ; preserved because redirector callbacks keep the caller's SFT in ES:DI.
    push bx
    push es
    mov bx, [sda_seg]
    cmp bx, 0
    je .fail
    mov es, bx
    mov bx, [sda_off]
    add bx, ax
    mov ax, [es:bx]
    clc
    jmp .done
.fail:
    stc
.done:
    pop es
    pop bx
    ret

copy_sda_string:
    ; AX = SDA-relative inline filename offset, DI = destination in CS.
    ; The 127-byte limit keeps path_buf/path2_buf NUL-terminated and avoids
    ; scanning unbounded DOS memory if the SDA content is malformed.
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
    ; Source is ES:SI, but the destination buffer lives in our resident code
    ; segment (DS=CS).  STOSB would also write through ES and corrupt the SDA,
    ; which is why HOSTRPC used to receive guest='' for every path.
    mov [di], al
    inc di
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
    ; Reset the resident request block before each HOSTRPC operation.  DOS
    ; redirector callbacks often enter with ES pointing at caller-owned SFT/DTA
    ; data, so make ES explicitly match CS before using rep stosw through ES:DI.
    push ax
    push cx
    push di
    push es
    push cs
    pop es
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
device_info  dw 8048h
sft_open_mode dw 0FF02h
sft_file_attr dw 0020h
ext_open_action dw 0
ext_open_mode dw 0
ext_open_attr dw 0
ext_open_status dw 0
ext_open_missing_error dw DOS_ERR_FILE_NOT_FOUND
phys_tmp     dd 0
host_file_count dw 0
host_find_count dw 0
host_sft_seg times HOSTDRV_MAX_HANDLES dw 0
host_sft_off times HOSTDRV_MAX_HANDLES dw 0

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
