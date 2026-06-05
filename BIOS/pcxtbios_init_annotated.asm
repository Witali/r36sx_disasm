; Hand-annotated reset/init map for BIOS/pcxtbios.bin.
;
; ROM mapping:
;   BIOS image base/start: physical FE000h
;   file offset 0000h -> F000:E000 / physical FE000h
;   file offset 1FF0h -> F000:FFF0 / physical FFFF0h reset vector
;
; Reset vector:
;   F000:FFF0  ea 5b e0 00 f0    jmp far F000:E05B
;   F000:E05B == physical FE05Bh, the first executed init instruction.
;
; Use BIOS/pcxtbios_ghidra_full.s for the full machine-generated listing.

; ---------------------------------------------------------------------------
; F000:E000..E059 - POST banner strings
; ---------------------------------------------------------------------------
; These bytes are data, not startup code. The first strings are:
;   " MURMULATOR | 286 PC EMULATOR"
;   " CGA/TGA/VGA Graphics"
;   " LPT Port at "
;   " Game Port at 201h"

; ---------------------------------------------------------------------------
; F000:E05B - reset entry
; ---------------------------------------------------------------------------
F000:E05B  cli                         ; Disable IRQs during hardware setup.
F000:E05C  cld                         ; String ops advance upward.

; Display/control ports are quieted before memory and BDA setup.
F000:E05D  mov al,00h
F000:E05F  out 0A0h,al                 ; Murmulator/XT-specific control port.
F000:E061  mov dx,03D8h
F000:E064  out dx,al                   ; CGA mode control: display off.
F000:E065  mov dl,0B8h
F000:E067  inc al
F000:E069  out dx,al                   ; MDA/Hercules-style control port.

; Basic PPI/PIT setup.
F000:E06A  mov al,99h
F000:E06C  out 63h,al                  ; Platform PPI/control register.
F000:E06E  mov al,0A5h
F000:E070  out 61h,al                  ; PC speaker / keyboard gate port.
F000:E072  mov al,54h
F000:E074  out 43h,al                  ; PIT control: channel 1 setup.
F000:E076  mov al,12h
F000:E078  out 41h,al                  ; PIT channel 1 count byte.
F000:E07A  mov al,40h
F000:E07C  out 43h,al                  ; PIT latch/control update.

; DMA controller reset and channel mode setup.
F000:E07E  mov al,00h
F000:E080  out 81h,al                  ; DMA page registers.
F000:E082  out 82h,al
F000:E084  out 83h,al
F000:E086  out 0Dh,al                  ; 8237 master clear.
F000:E088  mov al,58h
F000:E08A  out 0Bh,al                  ; DMA channel mode programming.
F000:E08C  mov al,41h
F000:E08E  out 0Bh,al
F000:E090  mov al,42h
F000:E092  out 0Bh,al
F000:E094  mov al,43h
F000:E096  out 0Bh,al
F000:E098  mov al,0FFh
F000:E09A  out 01h,al                  ; DMA mask/base register writes.
F000:E09C  out 01h,al
F000:E09E  inc ax
F000:E09F  out 08h,al                  ; DMA command/status area.
F000:E0A1  out 0Ah,al                  ; DMA mask register.

; Program PIT channel 0 to the standard 18.2 Hz BIOS tick source.
F000:E0A3  mov al,36h
F000:E0A5  out 43h,al                  ; PIT ch0, lobyte/hibyte, mode 3.
F000:E0A7  mov al,00h
F000:E0A9  out 40h,al
F000:E0AB  out 40h,al                  ; Count 0000h means 65536.

; Platform-specific game/Murmulator control.
F000:E0AD  mov dx,0213h
F000:E0B0  inc ax
F000:E0B1  out dx,al

; ---------------------------------------------------------------------------
; F000:E0B2 - conventional RAM probe and clear
; ---------------------------------------------------------------------------
; DS is pointed at the BIOS Data Area. Then ES walks low memory in 16 KiB
; steps and writes 55AAh to verify RAM. Valid RAM is cleared with REP STOSW.
; The loop stops before video memory at A0000h.
F000:E0B2  mov ax,0040h
F000:E0B5  mov ds,ax                   ; DS = BIOS Data Area segment.
F000:E0B7  mov si,[0072h]              ; Warm/cold boot flag from BDA.
F000:E0C1  mov dx,55AAh                ; RAM test pattern.
F000:E0C5  xor di,di
F000:E0C7  mov es,bx                   ; ES = tested memory segment.
F000:E0C9  mov es:[di],dx
F000:E0CC  cmp dx,es:[di]
F000:E0CF  jnz F000:E0DE              ; Stop at first failing block.
F000:E0D1  mov cx,2000h
F000:E0D4  rep stosw                   ; Clear 16 KiB block.
F000:E0D6  add bh,04h                  ; Next 16 KiB segment.
F000:E0D9  cmp bh,0A0h                 ; Stop at A000:0000 video memory.
F000:E0DC  jc F000:E0C5

; ---------------------------------------------------------------------------
; F000:E0DE - initial stack and BDA memory/equipment values
; ---------------------------------------------------------------------------
F000:E0DE  xor ax,ax
F000:E0E0  mov es,ax
F000:E0E2  mov ss,ax
F000:E0E4  mov sp,0900h                ; Temporary real-mode stack.
F000:E0E9  mov bp,0002h
F000:E0EC  call F000:F92B              ; Memory test helper.
F000:E0EF  mov [0072h],si              ; Preserve warm-boot flag.
F000:E0F4  mov cl,06h
F000:E0F6  shr ax,cl
F000:E0F8  mov [0013h],ax              ; BDA: conventional memory size in KB.
F000:E100  mov [0015h],al              ; BDA: POST/error status bits.

; ---------------------------------------------------------------------------
; F000:E124 - PIC and interrupt vector table setup
; ---------------------------------------------------------------------------
F000:E124  cli
F000:E125  mov al,13h
F000:E127  out 20h,al                  ; 8259 PIC ICW1.
F000:E129  mov al,08h
F000:E12B  out 21h,al                  ; ICW2: IRQs start at INT 08h.
F000:E12D  mov al,09h
F000:E12F  out 21h,al                  ; ICW4 / 8086 mode.
F000:E131  mov al,0FFh
F000:E133  out 21h,al                  ; Mask all IRQs while vectors change.

; Vectors 00h..07h get the same default handler F000:FF23.
F000:E13C  mov cx,0008h
F000:E141  mov ax,0FF23h
F000:E144  stosw
F000:E145  mov ax,cs
F000:E147  stosw

; Vectors 08h..1Fh are copied from the ROM table at F000:FEF3.
; See BIOS/README.md for the decoded table.
F000:E14A  mov si,0FEF3h
F000:E14D  mov cl,18h
F000:E14F  movsw
F000:E150  mov ax,cs
F000:E152  stosw

; ---------------------------------------------------------------------------
; F000:E196 - keyboard gate / video defaults / BDA device state
; ---------------------------------------------------------------------------
F000:E196  mov dl,61h
F000:E198  in al,dx
F000:E199  or al,30h
F000:E19B  out dx,al
F000:E19C  and al,0CFh
F000:E19E  out dx,al                   ; Toggle keyboard/speaker gate bits.

F000:E1A3  mov ax,0030h
F000:E1A6  mov [0010h],ax              ; BDA: equipment flags candidate.
F000:E1A9  int 10h                     ; Set/test initial video mode.
F000:E1AB  mov ax,0020h
F000:E1AE  mov [0010h],ax
F000:E1B1  int 10h                     ; Set/test alternate video mode.

; Keyboard buffer setup in BDA.
F000:E1E1  mov ax,001Eh
F000:E1E4  mov [001Ah],ax              ; Keyboard buffer head offset.
F000:E1E7  mov [001Ch],ax              ; Keyboard buffer tail offset.
F000:E1EA  mov [0080h],ax              ; Keyboard buffer start.
F000:E1EF  mov [0082h],ax              ; Keyboard buffer end.

; LPT defaults.
F000:E204  mov word ptr [0008h],0378h  ; LPT1 base.
F000:E20A  mov word ptr [000Ah],0278h  ; LPT2 base.

; Serial/game-port detection follows.
F000:E217  mov dx,03FBh                ; Probe COM1 line-control register.
F000:E226  mov word ptr [di],03F8h     ; Save COM1 base if detected.
F000:E22C  mov dx,02FBh                ; Probe COM2 line-control register.
F000:E23B  mov word ptr [di],02F8h     ; Save COM2 base if detected.
F000:E249  mov dx,0201h                ; Game port probe.

; ---------------------------------------------------------------------------
; F000:E25B - x87 probe, option ROM scan, and POST display path
; ---------------------------------------------------------------------------
F000:E25B  call F000:E584              ; 8087/80287 presence test via FNINIT.
F000:E26A  mov dx,0C000h
F000:E26D  mov ds,dx                   ; Start scanning option ROM at C000h.
F000:E2C6  xor bx,bx
F000:E2C8  cmp word ptr [bx],0AA55h    ; Option ROM signature.
F000:E2E2  call F000:E5C3              ; ROM checksum helper.
F000:E2FA  call far [0067h]            ; Invoke accepted expansion ROM entry.
F000:E312  jmp F000:E26D              ; Continue option ROM scan.

; After option ROM scanning the BIOS initializes visible text mode, prints
; status/banner strings, handles POST errors, and reaches INT 19h.
F000:E330  mov ah,12h
F000:E335  int 10h                     ; Video feature/query call.
F000:E350  int 10h                     ; Cursor shape setup.
F000:E352  call F000:F9D0              ; Clear screen and home cursor.

; ---------------------------------------------------------------------------
; F000:E4BF - final screen setup and boot
; ---------------------------------------------------------------------------
F000:E4BF  int 10h                     ; Restore final cursor/video state.
F000:E4C1  int 19h                     ; Bootstrap loader.

; ---------------------------------------------------------------------------
; Key helper routines
; ---------------------------------------------------------------------------
F000:E584  fninit                      ; x87 probe starts here.
F000:E58D  fnstcw [si]                 ; Reads control word to detect FPU.

F000:F793                              ; Select video mode from equipment bits.
F000:F7AF                              ; PC speaker beep routine using PIT ch2.
F000:F92B                              ; RAM test helper: write/read 5555/AAAA.
F000:F9B3                              ; Wait for keyboard or BIOS tick timeout.
F000:F9D0                              ; Clear screen, set cursor, select page 0.
F000:FE6E                              ; INT 1Ah time-of-day services.
F000:FEA5                              ; INT 08h timer tick handler.
F000:FF23                              ; Default interrupt stub.
F000:FF54                              ; Print-screen style handler.
