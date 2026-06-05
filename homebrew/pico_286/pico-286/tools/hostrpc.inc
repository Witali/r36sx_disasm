; hostrpc.inc - shared NASM definitions for the R36SX HOSTRPC guest ABI.
;
; Keep this file 8086-friendly: it is included by DOS .COM utilities that run
; before any protected-mode helper is available.

%ifndef R36SX_HOSTRPC_INC
%define R36SX_HOSTRPC_INC

; HOSTRPC I/O ports.  The emulator exposes a tiny device here:
;   E360/E361 return signature bytes 'R'/'H' and can be read as one word.
;   E362 executes the command currently stored in the request block.
;   E363 returns status, so command/status sit on one 16-bit I/O pair.
;   E364..E367 receive the physical request-block address, little-endian.
PORT_BASE       equ 0E360h
PORT_ID0        equ PORT_BASE + 0
PORT_ID1        equ PORT_BASE + 1
PORT_COMMAND    equ PORT_BASE + 2
PORT_STATUS     equ PORT_BASE + 3
PORT_ADDR0      equ PORT_BASE + 4
PORT_ADDR1      equ PORT_BASE + 5
PORT_ADDR2      equ PORT_BASE + 6
PORT_ADDR3      equ PORT_BASE + 7

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
