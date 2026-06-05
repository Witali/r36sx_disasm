; MapDrive - A utility to map a host drive in the Pico-286 emulator.
;
; This program interfaces with the emulator's built-in network redirector
; (INT 2Fh, Function 11h) to make a host directory available as a DOS drive.
; By default, it maps drive H: to the host's shared directory.
; A drive letter can be passed on the command line, for example:
;   MAPDRIVE H:
;   MAPDRIVE G
;
; This allows for seamless file access between the DOS environment and the
; host system, simplifying file transfers and development workflows.
;
; To assemble this file, use the Flat Assembler (FASM):
;   fasm mapdrive.asm mapdrive.com
;

    org 100h
    use16
    cpu 8086

    ; Constants
    DEFAULT_DRIVE_LETTER equ 'H'
    DEFAULT_DRIVE_NUMBER equ 7         ; H = 7
    CDS_ENTRY_SIZE       equ 058h      ; DOS 4+ CDS entry size
    CDS_OFF_FLAGS        equ 043h      ; offset of flags within CDS entry
    ; Flags
    CDSFLAG_PHY          equ 04000h
    CDSFLAG_NET          equ 08000h
    CDSFLAG_NET_PHY      equ 0C000h    ; NET|PHY, NASM/FASM-compatible

    start:
        call parse_drive_arg
        jnc .args_ok
        mov dx, err_usage
        mov ah, 09h
        int 21h
        jmp exit

    .args_ok:
        ; Get List of Lists: INT 21h, AH=52h -> ES:BX
        mov ah, 52h
        int 21h

        ; Get LASTDRIVE at ES:[BX+21h] (DOS 3.1+)
        mov si, 021h
        mov dl, byte [es:bx+si]    ; DL = lastdrive

        ; Get CDS base pointer (far) at ES:[BX+16h] (offset:segment)
        mov si, 016h
        les bx, [es:bx+si]         ; ES:BX = CDS base

        ; Some OS set CDS to FFFF:FFFF (invalid)
        cmp bx, 0FFFFh
        jne .cds_ok
        mov ax, es          ; move ES to a GP register first
        cmp ax, 0FFFFh
        je .error_cds

    .cds_ok:
        ; Check drive <= lastdrive
        mov al, [drive_number]
        cmp al, dl
        jg .error_lastdrive

    .drive_ok:
        ; DI = CDS entry = BX + drive * 58h (use MUL instead of many SHLs)
        mov di, bx                 ; DI = CDS base offset
        xor ax, ax
        mov al, [drive_number]     ; AL = drive (0..25)
        mov bl, CDS_ENTRY_SIZE     ; BL = 88 (0x58)
        mul bl                     ; AX = AL * BL
        add di, ax                 ; DI = CDS base + drive * 58h (entry address)

    .drive_free:
        ; Set flags = NET|PHY (C000h)
        mov word [es:di+CDS_OFF_FLAGS], CDSFLAG_NET_PHY

        ; Set current_path = "<drive>:\"
        mov al, [drive_letter]
        mov byte [es:di+0], al
        mov byte [es:di+1], ':'
        mov byte [es:di+2], '\'
        mov byte [es:di+3], 0

        ; Get SDA pointer: INT 21h, AX=5D06h -> DS:SI
        mov ax, 5D06h
        push ds
        push si
        int 21h
        ; DS:SI now points to SDA; save into BX:DX for INT 2Fh
        mov bx, ds
        mov dx, si
        pop si
        pop ds

        ; INT 2Fh, AX=1100h, BX:DX = SDA
        mov ax, 1100h
        int 2Fh

        ; Success message
        mov dx, msg_ok
        mov ah, 09h
        int 21h

        jmp exit

    .error_cds:
        mov dx, err_cds_fail
        mov ah, 09h
        int 21h
        jmp exit

    .error_lastdrive:
        mov dx, err_lastdrive
        mov ah, 09h
        int 21h

    exit:
        mov ax, 4C00h
        int 21h

    parse_drive_arg:
        mov byte [drive_letter], DEFAULT_DRIVE_LETTER
        mov byte [drive_number], DEFAULT_DRIVE_NUMBER

        ; DOS stores the .COM command tail in the PSP:
        ; byte 80h = length, bytes 81h.. = text ending before the CR.
        mov si, 081h
        xor cx, cx
        mov cl, byte [080h]

    .skip_leading:
        cmp cx, 0
        je .success
        mov al, byte [si]
        cmp al, ' '
        je .consume_leading
        cmp al, 09h
        jne .read_drive

    .consume_leading:
        inc si
        dec cx
        jmp .skip_leading

    .read_drive:
        mov al, byte [si]
        call uppercase_al
        cmp al, 'A'
        jb .fail
        cmp al, 'Z'
        ja .fail
        mov byte [drive_letter], al
        sub al, 'A'
        mov byte [drive_number], al
        inc si
        dec cx

        ; Accept either "H" or "H:".
        cmp cx, 0
        je .success
        cmp byte [si], ':'
        jne .skip_trailing
        inc si
        dec cx

    .skip_trailing:
        cmp cx, 0
        je .success
        mov al, byte [si]
        cmp al, ' '
        je .consume_trailing
        cmp al, 09h
        jne .fail

    .consume_trailing:
        inc si
        dec cx
        jmp .skip_trailing

    .success:
        mov al, byte [drive_letter]
        mov byte [err_cds_drive], al
        mov byte [err_lastdrive_drive], al
        mov byte [msg_ok_drive], al
        clc
        ret

    .fail:
        stc
        ret

    uppercase_al:
        cmp al, 'a'
        jb .done
        cmp al, 'z'
        ja .done
        sub al, 020h
    .done:
        ret

    ; Data
    drive_letter db DEFAULT_DRIVE_LETTER
    drive_number db DEFAULT_DRIVE_NUMBER

    err_usage     db 'Usage: MAPDRIVE [drive:]',13,10
                  db 'Example: MAPDRIVE H:',13,10,'$'
    err_cds_fail  db 'Error: Could not get CDS for drive '
    err_cds_drive db DEFAULT_DRIVE_LETTER
                  db ':.',13,10,'$'
    err_lastdrive db 'CONFIG.SYS must contain LASTDRIVE='
    err_lastdrive_drive db DEFAULT_DRIVE_LETTER
                  db ' or higher.',13,10,'$'
    msg_ok        db 'Drive '
    msg_ok_drive  db DEFAULT_DRIVE_LETTER
                  db ': successfully mapped as a host drive.',13,10,'$'
