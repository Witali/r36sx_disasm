; vga_mode_x_test.asm
; Standalone DOS .COM smoke test for VGA mode 13h with chain-4 disabled.
;
; Build with NASM:
;   nasm -f bin vga_mode_x_test.asm -o vga_mode_x_test.com
;
; Expected display:
;   - 320x200 mode scaled by the emulator to the physical screen;
;   - repeating four-pixel color groups from VGA planes 0..3;
;   - a white 20-pixel grid and two diagonals drawn with single-pixel writes.
;
; This intentionally uses only real-mode DOS/BIOS services and direct VGA
; register/memory access.  It is a focused test for unchained planar 256-color
; addressing: byte offset = y * 80 + x / 4, plane mask = 1 << (x & 3).

bits 16
org 100h

VGA_SEG             equ 0A000h

SEQ_INDEX           equ 03C4h
SEQ_RESET           equ 00h
SEQ_MAP_MASK        equ 02h
SEQ_MEMORY_MODE     equ 04h

GC_INDEX            equ 03CEh
GC_MODE             equ 05h
GC_MISC             equ 06h
GC_BIT_MASK         equ 08h

CRTC_INDEX          equ 03D4h
CRTC_START_HIGH     equ 0Ch
CRTC_START_LOW      equ 0Dh
CRTC_OFFSET         equ 13h

SCREEN_W            equ 320
SCREEN_H            equ 200
MODE_X_ROW_BYTES    equ SCREEN_W / 4
MODE_X_PLANE_BYTES  equ MODE_X_ROW_BYTES * SCREEN_H

start:
    cld

    ; Start from BIOS mode 13h so DAC, attribute controller, graphics
    ; controller, and CRTC are already in the familiar 320x200x256 baseline.
    mov ax, 0013h
    int 10h

    call enable_mode_x_320x200

    mov ax, VGA_SEG
    mov es, ax

    call clear_mode_x
    call fill_plane_background
    call draw_grid
    call draw_diagonals

    ; Wait for a key, then let BIOS restore text mode.
    xor ah, ah
    int 16h

    mov ax, 0003h
    int 10h
    mov ax, 4C00h
    int 21h

enable_mode_x_320x200:
    ; Sequencer reset is the conventional safe wrapper for changing the
    ; sequencer Memory Mode register.
    mov al, SEQ_RESET
    mov ah, 01h
    call set_seq

    ; 06h = extended memory enabled, odd/even disabled, chain-4 disabled.
    ; This is the key Mode X switch after BIOS mode 13h.
    mov al, SEQ_MEMORY_MODE
    mov ah, 06h
    call set_seq

    mov al, SEQ_RESET
    mov ah, 03h
    call set_seq

    ; Keep mode-13h 256-color shift behavior and A0000h graphics aperture.
    mov al, GC_MODE
    mov ah, 40h
    call set_gc
    mov al, GC_MISC
    mov ah, 05h
    call set_gc
    mov al, GC_BIT_MASK
    mov ah, 0FFh
    call set_gc

    ; Display starts at offset 0000h and uses 80 addressable bytes per row:
    ; 320 visible pixels / 4 planes.
    mov al, CRTC_START_HIGH
    mov ah, 00h
    call set_crtc
    mov al, CRTC_START_LOW
    mov ah, 00h
    call set_crtc
    mov al, CRTC_OFFSET
    mov ah, 28h
    call set_crtc
    ret

clear_mode_x:
    ; With all four map-mask bits set, one byte write clears all planes at
    ; the addressed offset.
    mov al, SEQ_MAP_MASK
    mov ah, 0Fh
    call set_seq

    xor di, di
    mov cx, MODE_X_PLANE_BYTES
    xor al, al
    rep stosb
    ret

fill_plane_background:
    ; Fill each plane with a different palette index.  If plane addressing is
    ; correct, the background becomes repeating four-pixel color groups.
    mov al, SEQ_MAP_MASK
    mov ah, 01h
    call set_seq
    xor di, di
    mov cx, MODE_X_PLANE_BYTES
    mov al, 01h
    rep stosb

    mov al, SEQ_MAP_MASK
    mov ah, 02h
    call set_seq
    xor di, di
    mov cx, MODE_X_PLANE_BYTES
    mov al, 02h
    rep stosb

    mov al, SEQ_MAP_MASK
    mov ah, 04h
    call set_seq
    xor di, di
    mov cx, MODE_X_PLANE_BYTES
    mov al, 04h
    rep stosb

    mov al, SEQ_MAP_MASK
    mov ah, 08h
    call set_seq
    xor di, di
    mov cx, MODE_X_PLANE_BYTES
    mov al, 06h
    rep stosb
    ret

draw_grid:
    ; Horizontal grid lines every 20 pixels.
    xor bx, bx                  ; BX = y
.hline:
    xor cx, cx                  ; CX = x
.hline_pixel:
    mov ax, cx
    mov dl, 0Fh
    call put_mode_x_pixel
    inc cx
    cmp cx, SCREEN_W
    jb .hline_pixel
    add bx, 20
    cmp bx, SCREEN_H
    jb .hline

    ; Vertical grid lines every 20 pixels.
    xor si, si                  ; SI = x
.vline:
    xor bx, bx                  ; BX = y
.vline_pixel:
    mov ax, si
    mov dl, 0Fh
    call put_mode_x_pixel
    inc bx
    cmp bx, SCREEN_H
    jb .vline_pixel
    add si, 20
    cmp si, SCREEN_W
    jb .vline
    ret

draw_diagonals:
    ; Two single-pixel diagonals are useful for spotting wrong stride or plane
    ; selection.  They are deliberately drawn after the grid.
    xor bx, bx                  ; BX = y
.diag_loop:
    mov ax, bx
    mov dl, 0Ch
    call put_mode_x_pixel

    mov ax, SCREEN_W - 1
    sub ax, bx
    mov dl, 0Ah
    call put_mode_x_pixel

    inc bx
    cmp bx, SCREEN_H
    jb .diag_loop
    ret

put_mode_x_pixel:
    ; Input:
    ;   AX = x, 0..319
    ;   BX = y, 0..199
    ;   DL = 8-bit palette index
    ;
    ; Mode X byte offset = y * 80 + x / 4
    ; Mode X plane mask  = 1 << (x & 3)
    push ax
    push bx
    push cx
    push dx
    push di
    push si

    mov si, ax                  ; preserve x
    mov cx, ax
    and cl, 03h
    mov ah, 01h
    shl ah, cl
    mov al, SEQ_MAP_MASK
    call set_seq

    mov ax, bx
    mov di, ax
    shl ax, 6                   ; y * 64
    shl di, 4                   ; y * 16
    add di, ax                  ; y * 80

    mov ax, si
    shr ax, 1
    shr ax, 1                   ; x / 4
    add di, ax

    mov al, dl
    mov [es:di], al

    pop si
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret

set_seq:
    ; AL = sequencer register index, AH = value.
    push dx
    mov dx, SEQ_INDEX
    out dx, al
    inc dx
    mov al, ah
    out dx, al
    pop dx
    ret

set_gc:
    ; AL = graphics-controller register index, AH = value.
    push dx
    mov dx, GC_INDEX
    out dx, al
    inc dx
    mov al, ah
    out dx, al
    pop dx
    ret

set_crtc:
    ; AL = CRTC register index, AH = value.
    push dx
    mov dx, CRTC_INDEX
    out dx, al
    inc dx
    mov al, ah
    out dx, al
    pop dx
    ret
