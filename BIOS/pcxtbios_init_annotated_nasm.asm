; NASM rebuild source generated from BIOS/pcxtbios_init_annotated.asm.
; Address prefixes are converted to loc_XXXX labels; unlisted gaps are
; byte-preserved from BIOS/pcxtbios.bin so the output can still rebuild
; the complete 8 KiB ROM while selected init code is assembled as text.
;
; Build: nasm -f bin BIOS/pcxtbios_init_annotated_nasm.asm -o BIOS/pcxtbios_init_annotated.bin
bits 16
org 0xe000

%define ROM_BASE 0xe000
%macro gap_to 1
    %assign __target (%1 - ROM_BASE)
    %assign __here ($ - $$)
    %if __target < __here
        %error "assembled past annotated BIOS address"
    %elif __target > __here
        incbin "BIOS/pcxtbios.bin", __here, __target - __here
    %endif
%endmacro

loc_E5C3 equ 0xE5C3 ; target inside byte-preserved ROM gap

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
gap_to 0xE05B
loc_E05B:
    cli ; Disable IRQs during hardware setup.
gap_to 0xE05C
loc_E05C:
    cld ; String ops advance upward.

; Display/control ports are quieted before memory and BDA setup.
gap_to 0xE05D
loc_E05D:
    mov al,00h
gap_to 0xE05F
loc_E05F:
    out 0A0h,al ; Murmulator/XT-specific control port.
gap_to 0xE061
loc_E061:
    mov dx,03D8h
gap_to 0xE064
loc_E064:
    out dx,al ; CGA mode control: display off.
gap_to 0xE065
loc_E065:
    mov dl,0B8h
gap_to 0xE067
loc_E067:
    inc al
gap_to 0xE069
loc_E069:
    out dx,al ; MDA/Hercules-style control port.

; Basic PPI/PIT setup.
gap_to 0xE06A
loc_E06A:
    mov al,99h
gap_to 0xE06C
loc_E06C:
    out 63h,al ; Platform PPI/control register.
gap_to 0xE06E
loc_E06E:
    mov al,0A5h
gap_to 0xE070
loc_E070:
    out 61h,al ; PC speaker / keyboard gate port.
gap_to 0xE072
loc_E072:
    mov al,54h
gap_to 0xE074
loc_E074:
    out 43h,al ; PIT control: channel 1 setup.
gap_to 0xE076
loc_E076:
    mov al,12h
gap_to 0xE078
loc_E078:
    out 41h,al ; PIT channel 1 count byte.
gap_to 0xE07A
loc_E07A:
    mov al,40h
gap_to 0xE07C
loc_E07C:
    out 43h,al ; PIT latch/control update.

; DMA controller reset and channel mode setup.
gap_to 0xE07E
loc_E07E:
    mov al,00h
gap_to 0xE080
loc_E080:
    out 81h,al ; DMA page registers.
gap_to 0xE082
loc_E082:
    out 82h,al
gap_to 0xE084
loc_E084:
    out 83h,al
gap_to 0xE086
loc_E086:
    out 0Dh,al ; 8237 master clear.
gap_to 0xE088
loc_E088:
    mov al,58h
gap_to 0xE08A
loc_E08A:
    out 0Bh,al ; DMA channel mode programming.
gap_to 0xE08C
loc_E08C:
    mov al,41h
gap_to 0xE08E
loc_E08E:
    out 0Bh,al
gap_to 0xE090
loc_E090:
    mov al,42h
gap_to 0xE092
loc_E092:
    out 0Bh,al
gap_to 0xE094
loc_E094:
    mov al,43h
gap_to 0xE096
loc_E096:
    out 0Bh,al
gap_to 0xE098
loc_E098:
    mov al,0FFh
gap_to 0xE09A
loc_E09A:
    out 01h,al ; DMA mask/base register writes.
gap_to 0xE09C
loc_E09C:
    out 01h,al
gap_to 0xE09E
loc_E09E:
    inc ax
gap_to 0xE09F
loc_E09F:
    out 08h,al ; DMA command/status area.
gap_to 0xE0A1
loc_E0A1:
    out 0Ah,al ; DMA mask register.

; Program PIT channel 0 to the standard 18.2 Hz BIOS tick source.
gap_to 0xE0A3
loc_E0A3:
    mov al,36h
gap_to 0xE0A5
loc_E0A5:
    out 43h,al ; PIT ch0, lobyte/hibyte, mode 3.
gap_to 0xE0A7
loc_E0A7:
    mov al,00h
gap_to 0xE0A9
loc_E0A9:
    out 40h,al
gap_to 0xE0AB
loc_E0AB:
    out 40h,al ; Count 0000h means 65536.

; Platform-specific game/Murmulator control.
gap_to 0xE0AD
loc_E0AD:
    mov dx,0213h
gap_to 0xE0B0
loc_E0B0:
    inc ax
gap_to 0xE0B1
loc_E0B1:
    out dx,al

; ---------------------------------------------------------------------------
; F000:E0B2 - conventional RAM probe and clear
; ---------------------------------------------------------------------------
; DS is pointed at the BIOS Data Area. Then ES walks low memory in 16 KiB
; steps and writes 55AAh to verify RAM. Valid RAM is cleared with REP STOSW.
; The loop stops before video memory at A0000h.
gap_to 0xE0B2
loc_E0B2:
    mov ax,0040h
gap_to 0xE0B5
loc_E0B5:
    mov ds,ax ; DS = BIOS Data Area segment.
gap_to 0xE0B7
loc_E0B7:
    mov si,[0072h] ; Warm/cold boot flag from BDA.
gap_to 0xE0C1
loc_E0C1:
    mov dx,55AAh ; RAM test pattern.
gap_to 0xE0C5
loc_E0C5:
    db 033h,0FFh ; xor di,di
gap_to 0xE0C7
loc_E0C7:
    mov es,bx ; ES = tested memory segment.
gap_to 0xE0C9
loc_E0C9:
    mov [es:di],dx
gap_to 0xE0CC
loc_E0CC:
    cmp dx,[es:di]
gap_to 0xE0CF
loc_E0CF:
    jnz loc_E0DE ; Stop at first failing block.
gap_to 0xE0D1
loc_E0D1:
    mov cx,2000h
gap_to 0xE0D4
loc_E0D4:
    rep stosw ; Clear 16 KiB block.
gap_to 0xE0D6
loc_E0D6:
    add bh,04h ; Next 16 KiB segment.
gap_to 0xE0D9
loc_E0D9:
    cmp bh,0A0h ; Stop at A000:0000 video memory.
gap_to 0xE0DC
loc_E0DC:
    jc loc_E0C5

; ---------------------------------------------------------------------------
; F000:E0DE - initial stack and BDA memory/equipment values
; ---------------------------------------------------------------------------
gap_to 0xE0DE
loc_E0DE:
    db 033h,0C0h ; xor ax,ax
gap_to 0xE0E0
loc_E0E0:
    mov es,ax
gap_to 0xE0E2
loc_E0E2:
    mov ss,ax
gap_to 0xE0E4
loc_E0E4:
    mov sp,0900h ; Temporary real-mode stack.
gap_to 0xE0E9
loc_E0E9:
    mov bp,0002h
gap_to 0xE0EC
loc_E0EC:
    call loc_F92B ; Memory test helper.
gap_to 0xE0EF
loc_E0EF:
    mov [0072h],si ; Preserve warm-boot flag.
gap_to 0xE0F4
loc_E0F4:
    mov cl,06h
gap_to 0xE0F6
loc_E0F6:
    shr ax,cl
gap_to 0xE0F8
loc_E0F8:
    mov [0013h],ax ; BDA: conventional memory size in KB.
gap_to 0xE100
loc_E100:
    mov [0015h],al ; BDA: POST/error status bits.

; ---------------------------------------------------------------------------
; F000:E124 - PIC and interrupt vector table setup
; ---------------------------------------------------------------------------
gap_to 0xE124
loc_E124:
    cli
gap_to 0xE125
loc_E125:
    mov al,13h
gap_to 0xE127
loc_E127:
    out 20h,al ; 8259 PIC ICW1.
gap_to 0xE129
loc_E129:
    mov al,08h
gap_to 0xE12B
loc_E12B:
    out 21h,al ; ICW2: IRQs start at INT 08h.
gap_to 0xE12D
loc_E12D:
    mov al,09h
gap_to 0xE12F
loc_E12F:
    out 21h,al ; ICW4 / 8086 mode.
gap_to 0xE131
loc_E131:
    mov al,0FFh
gap_to 0xE133
loc_E133:
    out 21h,al ; Mask all IRQs while vectors change.

; Vectors 00h..07h get the same default handler F000:FF23.
gap_to 0xE13C
loc_E13C:
    mov cx,0008h
gap_to 0xE141
loc_E141:
    mov ax,0FF23h
gap_to 0xE144
loc_E144:
    stosw
gap_to 0xE145
loc_E145:
    mov ax,cs
gap_to 0xE147
loc_E147:
    stosw

; Vectors 08h..1Fh are copied from the ROM table at F000:FEF3.
; See BIOS/README.md for the decoded table.
gap_to 0xE14A
loc_E14A:
    mov si,0FEF3h
gap_to 0xE14D
loc_E14D:
    mov cl,18h
gap_to 0xE14F
loc_E14F:
    movsw
gap_to 0xE150
loc_E150:
    mov ax,cs
gap_to 0xE152
loc_E152:
    stosw

; ---------------------------------------------------------------------------
; F000:E196 - keyboard gate / video defaults / BDA device state
; ---------------------------------------------------------------------------
gap_to 0xE196
loc_E196:
    mov dl,61h
gap_to 0xE198
loc_E198:
    in al,dx
gap_to 0xE199
loc_E199:
    or al,30h
gap_to 0xE19B
loc_E19B:
    out dx,al
gap_to 0xE19C
loc_E19C:
    and al,0CFh
gap_to 0xE19E
loc_E19E:
    out dx,al ; Toggle keyboard/speaker gate bits.

gap_to 0xE1A3
loc_E1A3:
    mov ax,0030h
gap_to 0xE1A6
loc_E1A6:
    mov [0010h],ax ; BDA: equipment flags candidate.
gap_to 0xE1A9
loc_E1A9:
    int 10h ; Set/test initial video mode.
gap_to 0xE1AB
loc_E1AB:
    mov ax,0020h
gap_to 0xE1AE
loc_E1AE:
    mov [0010h],ax
gap_to 0xE1B1
loc_E1B1:
    int 10h ; Set/test alternate video mode.

; Keyboard buffer setup in BDA.
gap_to 0xE1E1
loc_E1E1:
    mov ax,001Eh
gap_to 0xE1E4
loc_E1E4:
    mov [001Ah],ax ; Keyboard buffer head offset.
gap_to 0xE1E7
loc_E1E7:
    mov [001Ch],ax ; Keyboard buffer tail offset.
gap_to 0xE1EA
loc_E1EA:
    mov [0080h],ax ; Keyboard buffer start.
gap_to 0xE1EF
loc_E1EF:
    mov [0082h],ax ; Keyboard buffer end.

; LPT defaults.
gap_to 0xE204
loc_E204:
    mov word [0008h],0378h ; LPT1 base.
gap_to 0xE20A
loc_E20A:
    mov word [000Ah],0278h ; LPT2 base.

; Serial/game-port detection follows.
gap_to 0xE217
loc_E217:
    mov dx,03FBh ; Probe COM1 line-control register.
gap_to 0xE226
loc_E226:
    mov word [di],03F8h ; Save COM1 base if detected.
gap_to 0xE22C
loc_E22C:
    mov dx,02FBh ; Probe COM2 line-control register.
gap_to 0xE23B
loc_E23B:
    mov word [di],02F8h ; Save COM2 base if detected.
gap_to 0xE249
loc_E249:
    mov dx,0201h ; Game port probe.

; ---------------------------------------------------------------------------
; F000:E25B - x87 probe, option ROM scan, and POST display path
; ---------------------------------------------------------------------------
gap_to 0xE25B
loc_E25B:
    call loc_E584 ; 8087/80287 presence test via FNINIT.
gap_to 0xE26A
loc_E26A:
    mov dx,0C000h
gap_to 0xE26D
loc_E26D:
    mov ds,dx ; Start scanning option ROM at C000h.
gap_to 0xE2C6
loc_E2C6:
    db 033h,0DBh ; xor bx,bx
gap_to 0xE2C8
loc_E2C8:
    cmp word [bx],0AA55h ; Option ROM signature.
gap_to 0xE2E2
loc_E2E2:
    call loc_E5C3 ; ROM checksum helper.
gap_to 0xE2FA
loc_E2FA:
    call far [es:0067h] ; Invoke accepted expansion ROM entry.
gap_to 0xE312
loc_E312:
    jmp loc_E26D ; Continue option ROM scan.

; After option ROM scanning the BIOS initializes visible text mode, prints
; status/banner strings, handles POST errors, and reaches INT 19h.
gap_to 0xE330
loc_E330:
    mov ah,12h
gap_to 0xE335
loc_E335:
    int 10h ; Video feature/query call.
gap_to 0xE350
loc_E350:
    int 10h ; Cursor shape setup.
gap_to 0xE352
loc_E352:
    call loc_F9D0 ; Clear screen and home cursor.

; ---------------------------------------------------------------------------
; F000:E4BF - final screen setup and boot
; ---------------------------------------------------------------------------
gap_to 0xE4BF
loc_E4BF:
    int 10h ; Restore final cursor/video state.
gap_to 0xE4C1
loc_E4C1:
    int 19h ; Bootstrap loader.

; ---------------------------------------------------------------------------
; Key helper routines
; ---------------------------------------------------------------------------
gap_to 0xE584
loc_E584:
    fninit ; x87 probe starts here.
gap_to 0xE58D
loc_E58D:
    fnstcw [si] ; Reads control word to detect FPU.

gap_to 0xF793
loc_F793: ; Select video mode from equipment bits.
gap_to 0xF7AF
loc_F7AF: ; PC speaker beep routine using PIT ch2.
gap_to 0xF92B
loc_F92B: ; RAM test helper: write/read 5555/AAAA.
gap_to 0xF9B3
loc_F9B3: ; Wait for keyboard or BIOS tick timeout.
gap_to 0xF9D0
loc_F9D0: ; Clear screen, set cursor, select page 0.
gap_to 0xFE6E
loc_FE6E: ; INT 1Ah time-of-day services.
gap_to 0xFEA5
loc_FEA5: ; INT 08h timer tick handler.
gap_to 0xFF23
loc_FF23: ; Default interrupt stub.
gap_to 0xFF54
loc_FF54: ; Print-screen style handler.

; Preserve the rest of the ROM image through F000:FFFF.
gap_to 0x10000
