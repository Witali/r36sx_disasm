; hostrpc.inc - shared NASM definitions for the R36SX HOSTRPC guest ABI.
;
; Keep this file 8086-friendly: it is included by DOS .COM utilities that run
; before any protected-mode helper is available.

%ifndef R36SX_HOSTRPC_INC
%define R36SX_HOSTRPC_INC

; HOSTRPC stream protocol port.  Guest code writes command/data frames to the
; same 8-bit port and waits for the host to toggle bit 7 in the readable latch.
PORT_BASE       equ 0E360h
PORT_DATA       equ PORT_BASE + 0

PROTO_CMD_FLAG  equ 080h
PROTO_DATA_MASK equ 07Fh
PROTO_RESET     equ 0
PROTO_PING      equ 1
PROTO_CONTINUE  equ 2
PROTO_ABORT     equ 3
PROTO_END       equ 4
PROTO_CALL      equ 5

; Request block versioning.  RPC_MAGIC is little-endian "RH".
RPC_MAGIC       equ 05248h
RPC_VERSION     equ 1
RPC_EXECUTE     equ 1

; HOSTRPC command identifiers understood by r36sx_host_rpc.c.inl.
CMD_PING        equ 0
CMD_OPEN_RO     equ 1
CMD_OPEN_RW     equ 2
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
CMD_CLOSE_ALL   equ 16
CMD_CHDIR       equ 17

; HOSTRPC currently exposes 16 file handles.  Guest drivers may mirror handle
; ownership with sidecar tables, but the handle namespace itself belongs to
; the emulator-side HOSTRPC device.
HOSTRPC_MAX_HANDLES equ 16

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

; Metadata response aliases.  GETATTR/open store DOS packed time in the old
; reserved word and DOS packed date in the low word of bytes_done.  Read/write
; still use the full bytes_done dword for transfer counts.
REQ_DOS_TIME    equ REQ_RESERVED
REQ_DOS_DATE    equ REQ_BYTES_DONE

; HOSTRPC request flags.
REQ_FLAG_CREATE_NEW equ 0001h

%endif
