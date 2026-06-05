0000E000  15204D            adc ax,0x4d20
0000E003  55                push bp
0000E004  52                push dx
0000E005  4D                dec bp
0000E006  55                push bp
0000E007  4C                dec sp
0000E008  41                inc cx
0000E009  54                push sp
0000E00A  4F                dec di
0000E00B  52                push dx
0000E00C  207C20            and [si+0x20],bh
0000E00F  3238              xor bh,[bx+si]
0000E011  36205043          and [ss:bx+si+0x43],dl
0000E015  20454D            and [di+0x4d],al
0000E018  55                push bp
0000E019  4C                dec sp
0000E01A  41                inc cx
0000E01B  54                push sp
0000E01C  4F                dec di
0000E01D  52                push dx
0000E01E  00C3              add bl,al
0000E020  204347            and [bp+di+0x47],al
0000E023  41                inc cx
0000E024  2F                das
0000E025  54                push sp
0000E026  47                inc di
0000E027  41                inc cx
0000E028  2F                das
0000E029  56                push si
0000E02A  47                inc di
0000E02B  41                inc cx
0000E02C  204772            and [bx+0x72],al
0000E02F  61                popa
0000E030  7068              jo 0xe09a
0000E032  69637300C3        imul sp,[bp+di+0x73],word 0xc300
0000E037  204C50            and [si+0x50],cl
0000E03A  54                push sp
0000E03B  20506F            and [bx+si+0x6f],dl
0000E03E  7274              jc 0xe0b4
0000E040  206174            and [bx+di+0x74],ah
0000E043  2000              and [bx+si],al
0000E045  C3                ret
0000E046  204761            and [bx+0x61],al
0000E049  6D                insw
0000E04A  6520506F          and [gs:bx+si+0x6f],dl
0000E04E  7274              jc 0xe0c4
0000E050  206174            and [bx+di+0x74],ah
0000E053  2032              and [bp+si],dh
0000E055  3031              xor [bx+di],dh
0000E057  680090            push word 0x9000
0000E05A  90                nop
0000E05B  FA                cli
0000E05C  FC                cld
0000E05D  B000              mov al,0x0
0000E05F  E6A0              out 0xa0,al
0000E061  BAD803            mov dx,0x3d8
0000E064  EE                out dx,al
0000E065  B2B8              mov dl,0xb8
0000E067  FEC0              inc al
0000E069  EE                out dx,al
0000E06A  B099              mov al,0x99
0000E06C  E663              out 0x63,al
0000E06E  B0A5              mov al,0xa5
0000E070  E661              out 0x61,al
0000E072  B054              mov al,0x54
0000E074  E643              out 0x43,al
0000E076  B012              mov al,0x12
0000E078  E641              out 0x41,al
0000E07A  B040              mov al,0x40
0000E07C  E643              out 0x43,al
0000E07E  B000              mov al,0x0
0000E080  E681              out 0x81,al
0000E082  E682              out 0x82,al
0000E084  E683              out 0x83,al
0000E086  E60D              out 0xd,al
0000E088  B058              mov al,0x58
0000E08A  E60B              out 0xb,al
0000E08C  B041              mov al,0x41
0000E08E  E60B              out 0xb,al
0000E090  B042              mov al,0x42
0000E092  E60B              out 0xb,al
0000E094  B043              mov al,0x43
0000E096  E60B              out 0xb,al
0000E098  B0FF              mov al,0xff
0000E09A  E601              out 0x1,al
0000E09C  E601              out 0x1,al
0000E09E  40                inc ax
0000E09F  E608              out 0x8,al
0000E0A1  E60A              out 0xa,al
0000E0A3  B036              mov al,0x36
0000E0A5  E643              out 0x43,al
0000E0A7  B000              mov al,0x0
0000E0A9  E640              out 0x40,al
0000E0AB  E640              out 0x40,al
0000E0AD  BA1302            mov dx,0x213
0000E0B0  40                inc ax
0000E0B1  EE                out dx,al
0000E0B2  B84000            mov ax,0x40
0000E0B5  8ED8              mov ds,ax
0000E0B7  8B367200          mov si,[0x72]
0000E0BB  33C0              xor ax,ax
0000E0BD  8BE8              mov bp,ax
0000E0BF  8BD8              mov bx,ax
0000E0C1  BAAA55            mov dx,0x55aa
0000E0C4  FC                cld
0000E0C5  33FF              xor di,di
0000E0C7  8EC3              mov es,bx
0000E0C9  268915            mov [es:di],dx
0000E0CC  263B15            cmp dx,[es:di]
0000E0CF  750D              jnz 0xe0de
0000E0D1  B90020            mov cx,0x2000
0000E0D4  F3AB              rep stosw
0000E0D6  80C704            add bh,0x4
0000E0D9  80FFA0            cmp bh,0xa0
0000E0DC  72E7              jc 0xe0c5
0000E0DE  33C0              xor ax,ax
0000E0E0  8EC0              mov es,ax
0000E0E2  8ED0              mov ss,ax
0000E0E4  BC0009            mov sp,0x900
0000E0E7  55                push bp
0000E0E8  53                push bx
0000E0E9  BD0200            mov bp,0x2
0000E0EC  E83C18            call 0xf92b
0000E0EF  89367200          mov [0x72],si
0000E0F3  58                pop ax
0000E0F4  B106              mov cl,0x6
0000E0F6  D3E8              shr ax,cl
0000E0F8  A31300            mov [0x13],ax
0000E0FB  58                pop ax
0000E0FC  7302              jnc 0xe100
0000E0FE  0C10              or al,0x10
0000E100  A21500            mov [0x15],al
0000E103  33C0              xor ax,ax
0000E105  50                push ax
0000E106  50                push ax
0000E107  50                push ax
0000E108  50                push ax
0000E109  50                push ax
0000E10A  B030              mov al,0x30
0000E10C  8ED0              mov ss,ax
0000E10E  BC0001            mov sp,0x100
0000E111  1E                push ds
0000E112  BB00E0            mov bx,0xe000
0000E115  0E                push cs
0000E116  1F                pop ds
0000E117  B401              mov ah,0x1
0000E119  E8A404            call 0xe5c0
0000E11C  1F                pop ds
0000E11D  7405              jz 0xe124
0000E11F  800E150001        or byte [0x15],0x1
0000E124  FA                cli
0000E125  B013              mov al,0x13
0000E127  E620              out 0x20,al
0000E129  B008              mov al,0x8
0000E12B  E621              out 0x21,al
0000E12D  B009              mov al,0x9
0000E12F  E621              out 0x21,al
0000E131  B0FF              mov al,0xff
0000E133  E621              out 0x21,al
0000E135  1E                push ds
0000E136  33C0              xor ax,ax
0000E138  8EC0              mov es,ax
0000E13A  0E                push cs
0000E13B  1F                pop ds
0000E13C  B90800            mov cx,0x8
0000E13F  33FF              xor di,di
0000E141  B823FF            mov ax,0xff23
0000E144  AB                stosw
0000E145  8CC8              mov ax,cs
0000E147  AB                stosw
0000E148  E2F7              loop 0xe141
0000E14A  BEF3FE            mov si,0xfef3
0000E14D  B118              mov cl,0x18
0000E14F  A5                movsw
0000E150  8CC8              mov ax,cs
0000E152  AB                stosw
0000E153  E2FA              loop 0xe14f
0000E155  B4F6              mov ah,0xf6
0000E157  8ED8              mov ds,ax
0000E159  33DB              xor bx,bx
0000E15B  B404              mov ah,0x4
0000E15D  8B17              mov dx,[bx]
0000E15F  3AD6              cmp dl,dh
0000E161  7418              jz 0xe17b
0000E163  81FA55AA          cmp dx,0xaa55
0000E167  7412              jz 0xe17b
0000E169  E85404            call 0xe5c0
0000E16C  750D              jnz 0xe17b
0000E16E  FECC              dec ah
0000E170  75EB              jnz 0xe15d
0000E172  BF6000            mov di,0x60
0000E175  33C0              xor ax,ax
0000E177  AB                stosw
0000E178  B4F6              mov ah,0xf6
0000E17A  AB                stosw
0000E17B  1F                pop ds
0000E17C  33D2              xor dx,dx
0000E17E  26C70608005FF8    mov word [es:0x8],0xf85f
0000E185  26C706140054FF    mov word [es:0x14],0xff54
0000E18C  2689167C00        mov [es:0x7c],dx
0000E191  2689167E00        mov [es:0x7e],dx
0000E196  B261              mov dl,0x61
0000E198  EC                in al,dx
0000E199  0C30              or al,0x30
0000E19B  EE                out dx,al
0000E19C  24CF              and al,0xcf
0000E19E  EE                out dx,al
0000E19F  B080              mov al,0x80
0000E1A1  E6A0              out 0xa0,al
0000E1A3  B83000            mov ax,0x30
0000E1A6  A31000            mov [0x10],ax
0000E1A9  CD10              int 0x10
0000E1AB  B82000            mov ax,0x20
0000E1AE  A31000            mov [0x10],ax
0000E1B1  CD10              int 0x10
0000E1B3  E462              in al,0x62
0000E1B5  240F              and al,0xf
0000E1B7  8AE0              mov ah,al
0000E1B9  B0AD              mov al,0xad
0000E1BB  EE                out dx,al
0000E1BC  B061              mov al,0x61
0000E1BE  B400              mov ah,0x0
0000E1C0  A31000            mov [0x10],ax
0000E1C3  2430              and al,0x30
0000E1C5  7509              jnz 0xe1d0
0000E1C7  B853FF            mov ax,0xff53
0000E1CA  26A34000          mov [es:0x40],ax
0000E1CE  EB03              jmp short 0xe1d3
0000E1D0  E8C015            call 0xf793
0000E1D3  B008              mov al,0x8
0000E1D5  EE                out dx,al
0000E1D6  B95629            mov cx,0x2956
0000E1D9  E2FE              loop 0xe1d9
0000E1DB  B0C8              mov al,0xc8
0000E1DD  EE                out dx,al
0000E1DE  3480              xor al,0x80
0000E1E0  EE                out dx,al
0000E1E1  B81E00            mov ax,0x1e
0000E1E4  A31A00            mov [0x1a],ax
0000E1E7  A31C00            mov [0x1c],ax
0000E1EA  A38000            mov [0x80],ax
0000E1ED  0420              add al,0x20
0000E1EF  A38200            mov [0x82],ax
0000E1F2  B81414            mov ax,0x1414
0000E1F5  A37800            mov [0x78],ax
0000E1F8  A37A00            mov [0x7a],ax
0000E1FB  B80101            mov ax,0x101
0000E1FE  A37C00            mov [0x7c],ax
0000E201  A37E00            mov [0x7e],ax
0000E204  C70608007803      mov word [0x8],0x378
0000E20A  C7060A007802      mov word [0xa],0x278
0000E210  C606110080        mov byte [0x11],0x80
0000E215  33FF              xor di,di
0000E217  BAFB03            mov dx,0x3fb
0000E21A  B01A              mov al,0x1a
0000E21C  EE                out dx,al
0000E21D  B0FF              mov al,0xff
0000E21F  E6C0              out 0xc0,al
0000E221  EC                in al,dx
0000E222  3C1A              cmp al,0x1a
0000E224  7506              jnz 0xe22c
0000E226  C705F803          mov word [di],0x3f8
0000E22A  47                inc di
0000E22B  47                inc di
0000E22C  BAFB02            mov dx,0x2fb
0000E22F  B01A              mov al,0x1a
0000E231  EE                out dx,al
0000E232  B0FF              mov al,0xff
0000E234  E6C0              out 0xc0,al
0000E236  EC                in al,dx
0000E237  3C1A              cmp al,0x1a
0000E239  7506              jnz 0xe241
0000E23B  C705F802          mov word [di],0x2f8
0000E23F  47                inc di
0000E240  47                inc di
0000E241  8BC7              mov ax,di
0000E243  08061100          or [0x11],al
0000E247  B164              mov cl,0x64
0000E249  BA0102            mov dx,0x201
0000E24C  EC                in al,dx
0000E24D  3CFF              cmp al,0xff
0000E24F  7505              jnz 0xe256
0000E251  49                dec cx
0000E252  E307              jcxz 0xe25b
0000E254  EBF6              jmp short 0xe24c
0000E256  800E110010        or byte [0x11],0x10
0000E25B  E82603            call 0xe584
0000E25E  8B1E7200          mov bx,[0x72]
0000E262  53                push bx
0000E263  1E                push ds
0000E264  E461              in al,0x61
0000E266  340C              xor al,0xc
0000E268  E661              out 0x61,al
0000E26A  BA00C0            mov dx,0xc000
0000E26D  8EDA              mov ds,dx
0000E26F  EB55              jmp short 0xe2c6
0000E271  33C0              xor ax,ax
0000E273  8ED8              mov ds,ax
0000E275  A37204            mov [0x472],ax
0000E278  E9E0FD            jmp 0xe05b
0000E27B  4E                dec si
0000E27C  6F                outsw
0000E27D  20524F            and [bp+si+0x4f],dl
0000E280  4D                dec bp
0000E281  204241            and [bp+si+0x41],al
0000E284  53                push bx
0000E285  49                dec cx
0000E286  43                inc bx
0000E287  2C20              sub al,0x20
0000E289  626F6F            bound bp,[bx+0x6f]
0000E28C  7469              jz 0xe2f7
0000E28E  6E                outsb
0000E28F  67206672          and [esi+0x72],ah
0000E293  6F                outsw
0000E294  6D                insw
0000E295  206469            and [si+0x69],ah
0000E298  736B              jnc 0xe305
0000E29A  2E2E2E00909090    add [cs:bx+si-0x6f70],dl
0000E2A1  90                nop
0000E2A2  90                nop
0000E2A3  90                nop
0000E2A4  90                nop
0000E2A5  90                nop
0000E2A6  90                nop
0000E2A7  90                nop
0000E2A8  90                nop
0000E2A9  90                nop
0000E2AA  90                nop
0000E2AB  90                nop
0000E2AC  90                nop
0000E2AD  90                nop
0000E2AE  90                nop
0000E2AF  90                nop
0000E2B0  90                nop
0000E2B1  90                nop
0000E2B2  90                nop
0000E2B3  90                nop
0000E2B4  90                nop
0000E2B5  90                nop
0000E2B6  90                nop
0000E2B7  90                nop
0000E2B8  90                nop
0000E2B9  90                nop
0000E2BA  90                nop
0000E2BB  90                nop
0000E2BC  90                nop
0000E2BD  90                nop
0000E2BE  90                nop
0000E2BF  90                nop
0000E2C0  90                nop
0000E2C1  90                nop
0000E2C2  90                nop
0000E2C3  E99915            jmp 0xf85f
0000E2C6  33DB              xor bx,bx
0000E2C8  813F55AA          cmp word [bx],0xaa55
0000E2CC  753A              jnz 0xe308
0000E2CE  B84000            mov ax,0x40
0000E2D1  8EC0              mov es,ax
0000E2D3  8A4702            mov al,[bx+0x2]
0000E2D6  B105              mov cl,0x5
0000E2D8  D3E0              shl ax,cl
0000E2DA  03D0              add dx,ax
0000E2DC  B104              mov cl,0x4
0000E2DE  D3E0              shl ax,cl
0000E2E0  8BC8              mov cx,ax
0000E2E2  E8DE02            call 0xe5c3
0000E2E5  751B              jnz 0xe302
0000E2E7  52                push dx
0000E2E8  26C606120001      mov byte [es:0x12],0x1
0000E2EE  26C70667000300    mov word [es:0x67],0x3
0000E2F5  268C1E6900        mov [es:0x69],ds
0000E2FA  26FF1E6700        call far [es:0x67]
0000E2FF  5A                pop dx
0000E300  EB0A              jmp short 0xe30c
0000E302  26800E150020      or byte [es:0x15],0x20
0000E308  81C28000          add dx,0x80
0000E30C  81FA00FE          cmp dx,0xfe00
0000E310  7D03              jnl 0xe315
0000E312  E958FF            jmp 0xe26d
0000E315  1F                pop ds
0000E316  5B                pop bx
0000E317  891E7200          mov [0x72],bx
0000E31B  E421              in al,0x21
0000E31D  24BC              and al,0xbc
0000E31F  E621              out 0x21,al
0000E321  33DB              xor bx,bx
0000E323  381E1200          cmp [0x12],bl
0000E327  7407              jz 0xe330
0000E329  8EC3              mov es,bx
0000E32B  B300              mov bl,0x0
0000E32D  E88316            call 0xf9b3
0000E330  B412              mov ah,0x12
0000E332  BB10FF            mov bx,0xff10
0000E335  CD10              int 0x10
0000E337  80FFFF            cmp bh,0xff
0000E33A  7407              jz 0xe343
0000E33C  80261000CF        and byte [0x10],0xcf
0000E341  EB09              jmp short 0xe34c
0000E343  A04900            mov al,[0x49]
0000E346  E8C70B            call 0xef10
0000E349  E84714            call 0xf793
0000E34C  B401              mov ah,0x1
0000E34E  B5F0              mov ch,0xf0
0000E350  CD10              int 0x10
0000E352  E87B16            call 0xf9d0
0000E355  C606960010        mov byte [0x96],0x10
0000E35A  813E72003412      cmp word [0x72],0x1234
0000E360  1E                push ds
0000E361  07                pop es
0000E362  0E                push cs
0000E363  1F                pop ds
0000E364  7505              jnz 0xe36b
0000E366  B7FD              mov bh,0xfd
0000E368  E93301            jmp 0xe49e
0000E36B  BE00E0            mov si,0xe000
0000E36E  E87916            call 0xf9ea
0000E371  BEFFEB            mov si,0xebff
0000E374  E86B01            call 0xe4e2
0000E377  26F6061500FF      test byte [es:0x15],0xff
0000E37D  743E              jz 0xe3bd
0000E37F  B80003            mov ax,0x300
0000E382  E86801            call 0xe4ed
0000E385  BE09F8            mov si,0xf809
0000E388  E85701            call 0xe4e2
0000E38B  26A01500          mov al,[es:0x15]
0000E38F  E86801            call 0xe4fa
0000E392  BE2FF8            mov si,0xf82f
0000E395  E84A01            call 0xe4e2
0000E398  B302              mov bl,0x2
0000E39A  E81214            call 0xf7af
0000E39D  E84E1B            call 0xfeee
0000E3A0  50                push ax
0000E3A1  E83301            call 0xe4d7
0000E3A4  58                pop ax
0000E3A5  3C59              cmp al,0x59
0000E3A7  7409              jz 0xe3b2
0000E3A9  3C79              cmp al,0x79
0000E3AB  7405              jz 0xe3b2
0000E3AD  EA71E200F0        jmp 0xf000:0xe271
0000E3B2  26C606150000      mov byte [es:0x15],0x0
0000E3B8  E81516            call 0xf9d0
0000E3BB  EBAE              jmp short 0xe36b
0000E3BD  B80003            mov ax,0x300
0000E3C0  E82A01            call 0xe4ed
0000E3C3  E8A001            call 0xe566
0000E3C6  BED3F7            mov si,0xf7d3
0000E3C9  B80704            mov ax,0x407
0000E3CC  E81E01            call 0xe4ed
0000E3CF  26A04900          mov al,[es:0x49]
0000E3D3  3C07              cmp al,0x7
0000E3D5  7410              jz 0xe3e7
0000E3D7  26A01000          mov al,[es:0x10]
0000E3DB  2430              and al,0x30
0000E3DD  7505              jnz 0xe3e4
0000E3DF  BE1FE0            mov si,0xe01f
0000E3E2  EB03              jmp short 0xe3e7
0000E3E4  BE1FE0            mov si,0xe01f
0000E3E7  E8F800            call 0xe4e2
0000E3EA  BB0705            mov bx,0x507
0000E3ED  26A01100          mov al,[es:0x11]
0000E3F1  50                push ax
0000E3F2  B106              mov cl,0x6
0000E3F4  D2C8              ror al,cl
0000E3F6  2403              and al,0x3
0000E3F8  7409              jz 0xe403
0000E3FA  BD0800            mov bp,0x8
0000E3FD  BE36E0            mov si,0xe036
0000E400  E81701            call 0xe51a
0000E403  58                pop ax
0000E404  50                push ax
0000E405  BEF9E7            mov si,0xe7f9
0000E408  D0C8              ror al,1
0000E40A  2403              and al,0x3
0000E40C  7405              jz 0xe413
0000E40E  33ED              xor bp,bp
0000E410  E80701            call 0xe51a
0000E413  58                pop ax
0000E414  BE45E0            mov si,0xe045
0000E417  A810              test al,0x10
0000E419  740A              jz 0xe425
0000E41B  8BC3              mov ax,bx
0000E41D  E8CD00            call 0xe4ed
0000E420  E8BF00            call 0xe4e2
0000E423  FEC7              inc bh
0000E425  E87B01            call 0xe5a3
0000E428  720D              jc 0xe437
0000E42A  8BC3              mov ax,bx
0000E42C  E8BE00            call 0xe4ed
0000E42F  FEC7              inc bh
0000E431  BEECF7            mov si,0xf7ec
0000E434  E8AB00            call 0xe4e2
0000E437  FECF              dec bh
0000E439  B307              mov bl,0x7
0000E43B  8BC3              mov ax,bx
0000E43D  E8AD00            call 0xe4ed
0000E440  BEA1F0            mov si,0xf0a1
0000E443  E89C00            call 0xe4e2
0000E446  FEC7              inc bh
0000E448  FEC7              inc bh
0000E44A  32DB              xor bl,bl
0000E44C  8BC3              mov ax,bx
0000E44E  E89C00            call 0xe4ed
0000E451  BEF4F7            mov si,0xf7f4
0000E454  E88B00            call 0xe4e2
0000E457  06                push es
0000E458  268B2E1300        mov bp,[es:0x13]
0000E45D  4D                dec bp
0000E45E  4D                dec bp
0000E45F  BE0200            mov si,0x2
0000E462  8BD6              mov dx,si
0000E464  B88000            mov ax,0x80
0000E467  8EC0              mov es,ax
0000E469  80C30D            add bl,0xd
0000E46C  53                push bx
0000E46D  58                pop ax
0000E46E  50                push ax
0000E46F  8CC1              mov cx,es
0000E471  83FD01            cmp bp,byte +0x1
0000E474  7409              jz 0xe47f
0000E476  F7C1FF01          test cx,0x1ff
0000E47A  7402              jz 0xe47e
0000E47C  32ED              xor ch,ch
0000E47E  4A                dec dx
0000E47F  E86B00            call 0xe4ed
0000E482  E8B700            call 0xe53c
0000E485  42                inc dx
0000E486  E8A214            call 0xf92b
0000E489  7238              jc 0xe4c3
0000E48B  4D                dec bp
0000E48C  75DF              jnz 0xe46d
0000E48E  5B                pop bx
0000E48F  07                pop es
0000E490  B81E00            mov ax,0x1e
0000E493  26A31A00          mov [es:0x1a],ax
0000E497  26A31C00          mov [es:0x1c],ax
0000E49B  E82F01            call 0xe5cd
0000E49E  B301              mov bl,0x1
0000E4A0  E80C13            call 0xf7af
0000E4A3  E82A15            call 0xf9d0
0000E4A6  33C0              xor ax,ax
0000E4A8  8ED8              mov ds,ax
0000E4AA  C70672043412      mov word [0x472],0x1234
0000E4B0  B401              mov ah,0x1
0000E4B2  B90C0B            mov cx,0xb0c
0000E4B5  803E490407        cmp byte [0x449],0x7
0000E4BA  7403              jz 0xe4bf
0000E4BC  B90706            mov cx,0x607
0000E4BF  CD10              int 0x10
0000E4C1  CD19              int 0x19
0000E4C3  4D                dec bp
0000E4C4  5B                pop bx
0000E4C5  07                pop es
0000E4C6  26800E150002      or byte [es:0x15],0x2
0000E4CC  FEC7              inc bh
0000E4CE  FEC7              inc bh
0000E4D0  32DB              xor bl,bl
0000E4D2  8BC3              mov ax,bx
0000E4D4  E9ABFE            jmp 0xe382
0000E4D7  53                push bx
0000E4D8  50                push ax
0000E4D9  B40E              mov ah,0xe
0000E4DB  B307              mov bl,0x7
0000E4DD  CD10              int 0x10
0000E4DF  58                pop ax
0000E4E0  5B                pop bx
0000E4E1  C3                ret
0000E4E2  AC                lodsb
0000E4E3  0AC0              or al,al
0000E4E5  7405              jz 0xe4ec
0000E4E7  E8EDFF            call 0xe4d7
0000E4EA  EBF6              jmp short 0xe4e2
0000E4EC  C3                ret
0000E4ED  52                push dx
0000E4EE  53                push bx
0000E4EF  8BD0              mov dx,ax
0000E4F1  B402              mov ah,0x2
0000E4F3  B700              mov bh,0x0
0000E4F5  CD10              int 0x10
0000E4F7  5B                pop bx
0000E4F8  5A                pop dx
0000E4F9  C3                ret
0000E4FA  50                push ax
0000E4FB  B104              mov cl,0x4
0000E4FD  D2E8              shr al,cl
0000E4FF  E8E001            call 0xe6e2
0000E502  58                pop ax
0000E503  E8DC01            call 0xe6e2
0000E506  C3                ret
0000E507  50                push ax
0000E508  8AC4              mov al,ah
0000E50A  E8D501            call 0xe6e2
0000E50D  58                pop ax
0000E50E  E8E9FF            call 0xe4fa
0000E511  C3                ret
0000E512  50                push ax
0000E513  8AC4              mov al,ah
0000E515  E8E2FF            call 0xe4fa
0000E518  EBF3              jmp short 0xe50d
0000E51A  8AD0              mov dl,al
0000E51C  8BC3              mov ax,bx
0000E51E  E8CCFF            call 0xe4ed
0000E521  56                push si
0000E522  E8BDFF            call 0xe4e2
0000E525  268B4600          mov ax,[es:bp+0x0]
0000E529  E8DBFF            call 0xe507
0000E52C  BEA3FE            mov si,0xfea3
0000E52F  E8B0FF            call 0xe4e2
0000E532  5E                pop si
0000E533  45                inc bp
0000E534  45                inc bp
0000E535  FEC7              inc bh
0000E537  FECA              dec dl
0000E539  75E1              jnz 0xe51c
0000E53B  C3                ret
0000E53C  F8                clc
0000E53D  8AC2              mov al,dl
0000E53F  FEC0              inc al
0000E541  27                daa
0000E542  8AD0              mov dl,al
0000E544  7307              jnc 0xe54d
0000E546  8AC6              mov al,dh
0000E548  1400              adc al,0x0
0000E54A  27                daa
0000E54B  8AF0              mov dh,al
0000E54D  80FD00            cmp ch,0x0
0000E550  7413              jz 0xe565
0000E552  8AC6              mov al,dh
0000E554  E88B01            call 0xe6e2
0000E557  8AC2              mov al,dl
0000E559  B104              mov cl,0x4
0000E55B  D2C8              ror al,cl
0000E55D  E88201            call 0xe6e2
0000E560  8AC2              mov al,dl
0000E562  E87D01            call 0xe6e2
0000E565  C3                ret
0000E566  BEC1FF            mov si,0xffc1
0000E569  E876FF            call 0xe4e2
0000E56C  E8AC13            call 0xf91b
0000E56F  E870FF            call 0xe4e2
0000E572  BE13F9            mov si,0xf913
0000E575  26F606100002      test byte [es:0x10],0x2
0000E57B  7403              jz 0xe580
0000E57D  BEE6FF            mov si,0xffe6
0000E580  E85FFF            call 0xe4e2
0000E583  C3                ret
0000E584  DBE3              fninit
0000E586  BE0002            mov si,0x200
0000E589  C6440100          mov byte [si+0x1],0x0
0000E58D  D93C              fnstcw [si]
0000E58F  8A6401            mov ah,[si+0x1]
0000E592  80FC03            cmp ah,0x3
0000E595  7506              jnz 0xe59d
0000E597  800E100002        or byte [0x10],0x2
0000E59C  C3                ret
0000E59D  80261000FD        and byte [0x10],0xfd
0000E5A2  C3                ret
0000E5A3  FA                cli
0000E5A4  BAC102            mov dx,0x2c1
0000E5A7  EC                in al,dx
0000E5A8  3C99              cmp al,0x99
0000E5AA  7611              jna 0xe5bd
0000E5AC  B241              mov dl,0x41
0000E5AE  EC                in al,dx
0000E5AF  3C99              cmp al,0x99
0000E5B1  760A              jna 0xe5bd
0000E5B3  B603              mov dh,0x3
0000E5B5  EC                in al,dx
0000E5B6  3C99              cmp al,0x99
0000E5B8  7603              jna 0xe5bd
0000E5BA  FB                sti
0000E5BB  F9                stc
0000E5BC  C3                ret
0000E5BD  FB                sti
0000E5BE  F8                clc
0000E5BF  C3                ret
0000E5C0  B90020            mov cx,0x2000
0000E5C3  B000              mov al,0x0
0000E5C5  0207              add al,[bx]
0000E5C7  43                inc bx
0000E5C8  E2FB              loop 0xe5c5
0000E5CA  0AC0              or al,al
0000E5CC  C3                ret
0000E5CD  33C9              xor cx,cx
0000E5CF  8EC1              mov es,cx
0000E5D1  268A2E6300        mov ch,[es:0x63]
0000E5D6  32DB              xor bl,bl
0000E5D8  80C703            add bh,0x3
0000E5DB  8BC3              mov ax,bx
0000E5DD  E80DFF            call 0xe4ed
0000E5E0  BE7BE2            mov si,0xe27b
0000E5E3  32D2              xor dl,dl
0000E5E5  80FDF6            cmp ch,0xf6
0000E5E8  7505              jnz 0xe5ef
0000E5EA  BE42EC            mov si,0xec42
0000E5ED  FEC2              inc dl
0000E5EF  E8F0FE            call 0xe4e2
0000E5F2  BB1200            mov bx,0x12
0000E5F5  E8BB13            call 0xf9b3
0000E5F8  3C20              cmp al,0x20
0000E5FA  7401              jz 0xe5fd
0000E5FC  C3                ret
0000E5FD  CD18              int 0x18
0000E5FF  90                nop
0000E600  FB                sti
0000E601  33C0              xor ax,ax
0000E603  8ED8              mov ds,ax
0000E605  C7067800C7EF      mov word [0x78],0xefc7
0000E60B  8C0E7A00          mov [0x7a],cs
0000E60F  B006              mov al,0x6
0000E611  50                push ax
0000E612  33D2              xor dx,dx
0000E614  3C02              cmp al,0x2
0000E616  7703              ja 0xe61b
0000E618  80CA80            or dl,0x80
0000E61B  52                push dx
0000E61C  B400              mov ah,0x0
0000E61E  CD13              int 0x13
0000E620  5A                pop dx
0000E621  7217              jc 0xe63a
0000E623  33C0              xor ax,ax
0000E625  8EC0              mov es,ax
0000E627  B80102            mov ax,0x201
0000E62A  BB007C            mov bx,0x7c00
0000E62D  B101              mov cl,0x1
0000E62F  B500              mov ch,0x0
0000E631  CD13              int 0x13
0000E633  7205              jc 0xe63a
0000E635  EA007C0000        jmp 0x0:0x7c00
0000E63A  58                pop ax
0000E63B  FEC8              dec al
0000E63D  75D2              jnz 0xe611
0000E63F  0AE4              or ah,ah
0000E641  7510              jnz 0xe653
0000E643  0E                push cs
0000E644  1F                pop ds
0000E645  BEF5E6            mov si,0xe6f5
0000E648  E897FE            call 0xe4e2
0000E64B  E8A018            call 0xfeee
0000E64E  B806FF            mov ax,0xff06
0000E651  EBBE              jmp short 0xe611
0000E653  33C0              xor ax,ax
0000E655  8ED8              mov ds,ax
0000E657  A06300            mov al,[0x63]
0000E65A  3CF6              cmp al,0xf6
0000E65C  75E5              jnz 0xe643
0000E65E  CD18              int 0x18
0000E660  3CE0              cmp al,0xe0
0000E662  750A              jnz 0xe66e
0000E664  80CB02            or bl,0x2
0000E667  59                pop cx
0000E668  BAB3E9            mov dx,0xe9b3
0000E66B  52                push dx
0000E66C  EB4E              jmp short 0xe6bc
0000E66E  3C57              cmp al,0x57
0000E670  744F              jz 0xe6c1
0000E672  3C58              cmp al,0x58
0000E674  744B              jz 0xe6c1
0000E676  F6C302            test bl,0x2
0000E679  7441              jz 0xe6bc
0000E67B  80E3FD            and bl,0xfd
0000E67E  3CAA              cmp al,0xaa
0000E680  7506              jnz 0xe688
0000E682  8AC7              mov al,bh
0000E684  0C80              or al,0x80
0000E686  8AE0              mov ah,al
0000E688  8AF8              mov bh,al
0000E68A  8AC8              mov cl,al
0000E68C  80E17F            and cl,0x7f
0000E68F  80F92A            cmp cl,0x2a
0000E692  74D3              jz 0xe667
0000E694  80F936            cmp cl,0x36
0000E697  74CE              jz 0xe667
0000E699  3C35              cmp al,0x35
0000E69B  741F              jz 0xe6bc
0000E69D  3C1C              cmp al,0x1c
0000E69F  741B              jz 0xe6bc
0000E6A1  3C1D              cmp al,0x1d
0000E6A3  7417              jz 0xe6bc
0000E6A5  3C38              cmp al,0x38
0000E6A7  7413              jz 0xe6bc
0000E6A9  3C46              cmp al,0x46
0000E6AB  740F              jz 0xe6bc
0000E6AD  E88C08            call 0xef3c
0000E6B0  E80205            call 0xebb5
0000E6B3  7207              jc 0xe6bc
0000E6B5  B0E0              mov al,0xe0
0000E6B7  59                pop cx
0000E6B8  BAA4EB            mov dx,0xeba4
0000E6BB  52                push dx
0000E6BC  891E9600          mov [0x96],bx
0000E6C0  C3                ret
0000E6C1  80C42E            add ah,0x2e
0000E6C4  A01700            mov al,[0x17]
0000E6C7  A808              test al,0x8
0000E6C9  750A              jnz 0xe6d5
0000E6CB  A804              test al,0x4
0000E6CD  7509              jnz 0xe6d8
0000E6CF  A803              test al,0x3
0000E6D1  7508              jnz 0xe6db
0000E6D3  EB09              jmp short 0xe6de
0000E6D5  80C402            add ah,0x2
0000E6D8  80C402            add ah,0x2
0000E6DB  80C402            add ah,0x2
0000E6DE  32C0              xor al,al
0000E6E0  EBD5              jmp short 0xe6b7
0000E6E2  50                push ax
0000E6E3  240F              and al,0xf
0000E6E5  3C09              cmp al,0x9
0000E6E7  7602              jna 0xe6eb
0000E6E9  0407              add al,0x7
0000E6EB  0430              add al,0x30
0000E6ED  E8E7FD            call 0xe4d7
0000E6F0  58                pop ax
0000E6F1  C3                ret
0000E6F2  E90BFF            jmp 0xe600
0000E6F5  49                dec cx
0000E6F6  6E                outsb
0000E6F7  7365              jnc 0xe75e
0000E6F9  7274              jc 0xe76f
0000E6FB  20424F            and [bp+si+0x4f],al
0000E6FE  4F                dec di
0000E6FF  54                push sp
0000E700  206469            and [si+0x69],ah
0000E703  736B              jnc 0xe770
0000E705  20696E            and [bx+di+0x6e],ch
0000E708  20413A            and [bx+di+0x3a],al
0000E70B  0D0A50            or ax,0x500a
0000E70E  7265              jc 0xe775
0000E710  7373              jnc 0xe785
0000E712  20616E            and [bx+di+0x6e],ah
0000E715  7920              jns 0xe737
0000E717  6B657920          imul sp,[di+0x79],byte +0x20
0000E71B  7768              ja 0xe785
0000E71D  656E              gs outsb
0000E71F  207265            and [bp+si+0x65],dh
0000E722  61                popa
0000E723  64790D            fs jns 0xe733
0000E726  0A0A              or cl,[bp+si]
0000E728  0017              add [bx],dl
0000E72A  0400              add al,0x0
0000E72C  038001C0          add ax,[bx+si-0x3fff]
0000E730  006000            add [bx+si+0x0],ah
0000E733  3000              xor [bx+si],al
0000E735  1800              sbb [bx+si],al
0000E737  0C00              or al,0x0
0000E739  FB                sti
0000E73A  1E                push ds
0000E73B  52                push dx
0000E73C  56                push si
0000E73D  57                push di
0000E73E  51                push cx
0000E73F  53                push bx
0000E740  BB4000            mov bx,0x40
0000E743  8EDB              mov ds,bx
0000E745  8BFA              mov di,dx
0000E747  8BDA              mov bx,dx
0000E749  D1E3              shl bx,1
0000E74B  8B17              mov dx,[bx]
0000E74D  0BD2              or dx,dx
0000E74F  7410              jz 0xe761
0000E751  0AE4              or ah,ah
0000E753  7413              jz 0xe768
0000E755  FECC              dec ah
0000E757  743A              jz 0xe793
0000E759  FECC              dec ah
0000E75B  7453              jz 0xe7b0
0000E75D  FECC              dec ah
0000E75F  7463              jz 0xe7c4
0000E761  5B                pop bx
0000E762  59                pop cx
0000E763  5F                pop di
0000E764  5E                pop si
0000E765  5A                pop dx
0000E766  1F                pop ds
0000E767  CF                iret
0000E768  50                push ax
0000E769  8AD8              mov bl,al
0000E76B  83C203            add dx,byte +0x3
0000E76E  B080              mov al,0x80
0000E770  EE                out dx,al
0000E771  B104              mov cl,0x4
0000E773  D2C3              rol bl,cl
0000E775  83E30E            and bx,byte +0xe
0000E778  2E8B8729E7        mov ax,[cs:bx-0x18d7]
0000E77D  83EA03            sub dx,byte +0x3
0000E780  EE                out dx,al
0000E781  42                inc dx
0000E782  8AC4              mov al,ah
0000E784  EE                out dx,al
0000E785  58                pop ax
0000E786  42                inc dx
0000E787  42                inc dx
0000E788  241F              and al,0x1f
0000E78A  EE                out dx,al
0000E78B  B000              mov al,0x0
0000E78D  4A                dec dx
0000E78E  4A                dec dx
0000E78F  EE                out dx,al
0000E790  4A                dec dx
0000E791  EB31              jmp short 0xe7c4
0000E793  50                push ax
0000E794  B003              mov al,0x3
0000E796  B730              mov bh,0x30
0000E798  B320              mov bl,0x20
0000E79A  E84800            call 0xe7e5
0000E79D  7509              jnz 0xe7a8
0000E79F  83EA05            sub dx,byte +0x5
0000E7A2  59                pop cx
0000E7A3  8AC1              mov al,cl
0000E7A5  EE                out dx,al
0000E7A6  EBB9              jmp short 0xe761
0000E7A8  59                pop cx
0000E7A9  8AC1              mov al,cl
0000E7AB  80CC80            or ah,0x80
0000E7AE  EBB1              jmp short 0xe761
0000E7B0  B001              mov al,0x1
0000E7B2  B720              mov bh,0x20
0000E7B4  B301              mov bl,0x1
0000E7B6  E82C00            call 0xe7e5
0000E7B9  75F0              jnz 0xe7ab
0000E7BB  80E41E            and ah,0x1e
0000E7BE  83EA05            sub dx,byte +0x5
0000E7C1  EC                in al,dx
0000E7C2  EB9D              jmp short 0xe761
0000E7C4  83C205            add dx,byte +0x5
0000E7C7  EC                in al,dx
0000E7C8  8AE0              mov ah,al
0000E7CA  42                inc dx
0000E7CB  EC                in al,dx
0000E7CC  EB93              jmp short 0xe761
0000E7CE  8A5D7C            mov bl,[di+0x7c]
0000E7D1  2BC9              sub cx,cx
0000E7D3  EC                in al,dx
0000E7D4  8AE0              mov ah,al
0000E7D6  22C7              and al,bh
0000E7D8  3AC7              cmp al,bh
0000E7DA  7408              jz 0xe7e4
0000E7DC  E2F5              loop 0xe7d3
0000E7DE  FECB              dec bl
0000E7E0  75EF              jnz 0xe7d1
0000E7E2  0AFF              or bh,bh
0000E7E4  C3                ret
0000E7E5  83C204            add dx,byte +0x4
0000E7E8  EE                out dx,al
0000E7E9  42                inc dx
0000E7EA  42                inc dx
0000E7EB  53                push bx
0000E7EC  E8DFFF            call 0xe7ce
0000E7EF  5B                pop bx
0000E7F0  7506              jnz 0xe7f8
0000E7F2  4A                dec dx
0000E7F3  8AFB              mov bh,bl
0000E7F5  E8D6FF            call 0xe7ce
0000E7F8  C3                ret
0000E7F9  C3                ret
0000E7FA  20434F            and [bp+di+0x4f],al
0000E7FD  4D                dec bp
0000E7FE  20506F            and [bx+si+0x6f],dl
0000E801  7274              jc 0xe877
0000E803  206174            and [bx+di+0x74],ah
0000E806  2000              and [bx+si],al
0000E808  FA                cli
0000E809  8B1E1A00          mov bx,[0x1a]
0000E80D  3B1E1C00          cmp bx,[0x1c]
0000E811  8B07              mov ax,[bx]
0000E813  FB                sti
0000E814  5B                pop bx
0000E815  1F                pop ds
0000E816  CA0200            retf 0x2
0000E819  A11700            mov ax,[0x17]
0000E81C  EB2E              jmp short 0xe84c
0000E81E  8BC1              mov ax,cx
0000E820  E8BA03            call 0xebdd
0000E823  B000              mov al,0x0
0000E825  7325              jnc 0xe84c
0000E827  FEC0              inc al
0000E829  EB21              jmp short 0xe84c
0000E82B  90                nop
0000E82C  90                nop
0000E82D  90                nop
0000E82E  FB                sti
0000E82F  1E                push ds
0000E830  53                push bx
0000E831  BB4000            mov bx,0x40
0000E834  8EDB              mov ds,bx
0000E836  80FC05            cmp ah,0x5
0000E839  74E3              jz 0xe81e
0000E83B  8BD8              mov bx,ax
0000E83D  80E40F            and ah,0xf
0000E840  0AE4              or ah,ah
0000E842  740B              jz 0xe84f
0000E844  FECC              dec ah
0000E846  74C0              jz 0xe808
0000E848  FECC              dec ah
0000E84A  74CD              jz 0xe819
0000E84C  5B                pop bx
0000E84D  1F                pop ds
0000E84E  CF                iret
0000E84F  FA                cli
0000E850  A11A00            mov ax,[0x1a]
0000E853  3B061C00          cmp ax,[0x1c]
0000E857  7503              jnz 0xe85c
0000E859  FB                sti
0000E85A  EBF3              jmp short 0xe84f
0000E85C  F6C710            test bh,0x10
0000E85F  9C                pushf
0000E860  93                xchg ax,bx
0000E861  8B07              mov ax,[bx]
0000E863  9D                popf
0000E864  7506              jnz 0xe86c
0000E866  3CE0              cmp al,0xe0
0000E868  7502              jnz 0xe86c
0000E86A  32C0              xor al,al
0000E86C  43                inc bx
0000E86D  43                inc bx
0000E86E  891E1A00          mov [0x1a],bx
0000E872  3B1E8200          cmp bx,[0x82]
0000E876  75D4              jnz 0xe84c
0000E878  8B1E8000          mov bx,[0x80]
0000E87C  891E1A00          mov [0x1a],bx
0000E880  EBCA              jmp short 0xe84c
0000E882  90                nop
0000E883  90                nop
0000E884  90                nop
0000E885  0037              add [bx],dh
0000E887  2E202F            and [cs:bx],ch
0000E88A  3031              xor [bx+di],dh
0000E88C  2132              and [bp+si],si
0000E88E  3334              xor si,[si]
0000E890  352236            xor ax,0x3622
0000E893  383E1117          cmp [0x1711],bh
0000E897  051214            add ax,0x1412
0000E89A  1915              sbb [di],dx
0000E89C  090F              or [bx],cx
0000E89E  1039              adc [bx+di],bh
0000E8A0  3A3B              cmp bh,[bp+di]
0000E8A2  8401              test [bx+di],al
0000E8A4  1304              adc ax,[si]
0000E8A6  06                push es
0000E8A7  07                pop es
0000E8A8  080A              or [bp+si],cl
0000E8AA  0B0C              or cx,[si]
0000E8AC  3F                aas
0000E8AD  40                inc ax
0000E8AE  41                inc cx
0000E8AF  82                db 0x82
0000E8B0  3C1A              cmp al,0x1a
0000E8B2  1803              sbb [bp+di],al
0000E8B4  16                push ss
0000E8B5  020E0D42          add cl,[0x420d]
0000E8B9  43                inc bx
0000E8BA  44                inc sp
0000E8BB  813D882D          cmp word [di],0x2d88
0000E8BF  C02324            shl byte [bp+di],byte 0x24
0000E8C2  252627            and ax,0x2726
0000E8C5  2829              sub [bx+di],ch
0000E8C7  2A2B              sub ch,[bp+di]
0000E8C9  2CA0              sub al,0xa0
0000E8CB  90                nop
0000E8CC  32362DBB          xor dh,[0xbb2d]
0000E8D0  BCBDBE            mov sp,0xbebd
0000E8D3  BFC0C1            mov di,0xc1c0
0000E8D6  C2C3C4            ret 0xc4c3
0000E8D9  2031              and [bx+di],dh
0000E8DB  3334              xor si,[si]
0000E8DD  353738            xor ax,0x3837
0000E8E0  3930              cmp [bx+si],si
0000E8E2  3D1B08            cmp ax,0x81b
0000E8E5  5B                pop bx
0000E8E6  5D                pop bp
0000E8E7  0D5C2A            or ax,0x2a5c
0000E8EA  093B              or [bp+di],di
0000E8EC  27                daa
0000E8ED  60                pusha
0000E8EE  2C2E              sub al,0x2e
0000E8F0  2F                das
0000E8F1  40                inc ax
0000E8F2  5E                pop si
0000E8F3  5F                pop di
0000E8F4  D4D5              aam 0xd5
0000E8F6  D6                salc
0000E8F7  D7                xlatb
0000E8F8  D8D9              fcomp st1
0000E8FA  DADB              fcmovu st3
0000E8FC  DC                db 0xdc
0000E8FD  DD20              frstor [bx+si]
0000E8FF  2123              and [bp+di],sp
0000E901  2425              and al,0x25
0000E903  262A28            sub ch,[es:bx+si]
0000E906  292B              sub [bp+di],bp
0000E908  1B08              sbb cx,[bx+si]
0000E90A  7B7D              jpo 0xe989
0000E90C  0D7C05            or ax,0x57c
0000E90F  8F                db 0x8f
0000E910  3A22              cmp ah,[bp+si]
0000E912  7E3C              jng 0xe950
0000E914  3E3F              ds aas
0000E916  031E1FDE          add bx,[0xde1f]
0000E91A  DFE0              fnstsw ax
0000E91C  E1E2              loope 0xe900
0000E91E  E3E4              jcxz 0xe904
0000E920  E5E6              in ax,0xe6
0000E922  E720              out 0x20,ax
0000E924  050505            add ax,0x505
0000E927  050505            add ax,0x505
0000E92A  050505            add ax,0x505
0000E92D  1B7F1B            sbb di,[bx+0x1b]
0000E930  1D0A1C            sbb ax,0x1c0a
0000E933  F2050505          repne add ax,0x505
0000E937  050505            add ax,0x505
0000E93A  05F9FD            add ax,0xfdf9
0000E93D  02E8              add ch,al
0000E93F  E9EAEB            jmp 0xd52c
0000E942  EC                in al,dx
0000E943  ED                in ax,dx
0000E944  EE                out dx,al
0000E945  EF                out dx,ax
0000E946  F0F1              lock int1
0000E948  20F8              and al,bh
0000E94A  FA                cli
0000E94B  FB                sti
0000E94C  FC                cld
0000E94D  FE                db 0xfe
0000E94E  FF00              inc word [bx+si]
0000E950  0103              add [bp+di],ax
0000E952  050505            add ax,0x505
0000E955  050505            add ax,0x505
0000E958  050505            add ax,0x505
0000E95B  050505            add ax,0x505
0000E95E  050537            add ax,0x3705
0000E961  3839              cmp [bx+di],bh
0000E963  2D3435            sub ax,0x3534
0000E966  362B31            sub si,[ss:bx+di]
0000E969  3233              xor dh,[bp+di]
0000E96B  302EF705          xor [0x5f7],ch
0000E96F  0405              add al,0x5
0000E971  F305F405          rep add ax,0x5f4
0000E975  F5                cmc
0000E976  05F605            add ax,0x5f6
0000E979  05C7C8            add ax,0xc8c7
0000E97C  C9                leave
0000E97D  2DCB05            sub ax,0x5cb
0000E980  CD2B              int 0x2b
0000E982  CF                iret
0000E983  D0D1              rcl cl,1
0000E985  D2D3              rcl bl,cl
0000E987  FB                sti
0000E988  50                push ax
0000E989  53                push bx
0000E98A  51                push cx
0000E98B  52                push dx
0000E98C  56                push si
0000E98D  57                push di
0000E98E  1E                push ds
0000E98F  FC                cld
0000E990  B84000            mov ax,0x40
0000E993  8ED8              mov ds,ax
0000E995  E460              in al,0x60
0000E997  50                push ax
0000E998  E461              in al,0x61
0000E99A  50                push ax
0000E99B  0C80              or al,0x80
0000E99D  E661              out 0x61,al
0000E99F  58                pop ax
0000E9A0  E661              out 0x61,al
0000E9A2  58                pop ax
0000E9A3  8AE0              mov ah,al
0000E9A5  8B1E9600          mov bx,[0x96]
0000E9A9  E8B4FC            call 0xe660
0000E9AC  3CFF              cmp al,0xff
0000E9AE  750F              jnz 0xe9bf
0000E9B0  E9F601            jmp 0xeba9
0000E9B3  B020              mov al,0x20
0000E9B5  E620              out 0x20,al
0000E9B7  1F                pop ds
0000E9B8  5F                pop di
0000E9B9  5E                pop si
0000E9BA  5A                pop dx
0000E9BB  59                pop cx
0000E9BC  5B                pop bx
0000E9BD  58                pop ax
0000E9BE  CF                iret
0000E9BF  247F              and al,0x7f
0000E9C1  3C46              cmp al,0x46
0000E9C3  7603              jna 0xe9c8
0000E9C5  E92201            jmp 0xeaea
0000E9C8  BB85E8            mov bx,0xe885
0000E9CB  2ED7              cs xlatb
0000E9CD  0AC0              or al,al
0000E9CF  7806              js 0xe9d7
0000E9D1  0AE4              or ah,ah
0000E9D3  78DE              js 0xe9b3
0000E9D5  EB4D              jmp short 0xea24
0000E9D7  247F              and al,0x7f
0000E9D9  0AE4              or ah,ah
0000E9DB  7821              js 0xe9fe
0000E9DD  3C10              cmp al,0x10
0000E9DF  7306              jnc 0xe9e7
0000E9E1  08061700          or [0x17],al
0000E9E5  EBCC              jmp short 0xe9b3
0000E9E7  F606170004        test byte [0x17],0x4
0000E9EC  7536              jnz 0xea24
0000E9EE  84061800          test [0x18],al
0000E9F2  75BF              jnz 0xe9b3
0000E9F4  08061800          or [0x18],al
0000E9F8  30061700          xor [0x17],al
0000E9FC  EBB5              jmp short 0xe9b3
0000E9FE  3C10              cmp al,0x10
0000EA00  731A              jnc 0xea1c
0000EA02  F6D0              not al
0000EA04  20061700          and [0x17],al
0000EA08  3CF7              cmp al,0xf7
0000EA0A  75A7              jnz 0xe9b3
0000EA0C  A01900            mov al,[0x19]
0000EA0F  B400              mov ah,0x0
0000EA11  88261900          mov [0x19],ah
0000EA15  3AC4              cmp al,ah
0000EA17  749A              jz 0xe9b3
0000EA19  E99501            jmp 0xebb1
0000EA1C  F6D0              not al
0000EA1E  20061800          and [0x18],al
0000EA22  EB8F              jmp short 0xe9b3
0000EA24  F606180008        test byte [0x18],0x8
0000EA29  740D              jz 0xea38
0000EA2B  80FC45            cmp ah,0x45
0000EA2E  7405              jz 0xea35
0000EA30  80261800F7        and byte [0x18],0xf7
0000EA35  E97BFF            jmp 0xe9b3
0000EA38  8A161700          mov dl,[0x17]
0000EA3C  F6C208            test dl,0x8
0000EA3F  751D              jnz 0xea5e
0000EA41  F6C204            test dl,0x4
0000EA44  752B              jnz 0xea71
0000EA46  F6C203            test dl,0x3
0000EA49  757E              jnz 0xeac9
0000EA4B  3C1A              cmp al,0x1a
0000EA4D  7705              ja 0xea54
0000EA4F  0460              add al,0x60
0000EA51  E91601            jmp 0xeb6a
0000EA54  BBCCE8            mov bx,0xe8cc
0000EA57  2C20              sub al,0x20
0000EA59  2ED7              cs xlatb
0000EA5B  E90C01            jmp 0xeb6a
0000EA5E  3C1A              cmp al,0x1a
0000EA60  7705              ja 0xea67
0000EA62  B000              mov al,0x0
0000EA64  E93D01            jmp 0xeba4
0000EA67  BB3BE9            mov bx,0xe93b
0000EA6A  2C20              sub al,0x20
0000EA6C  2ED7              cs xlatb
0000EA6E  E9F900            jmp 0xeb6a
0000EA71  80FC46            cmp ah,0x46
0000EA74  7515              jnz 0xea8b
0000EA76  C606710080        mov byte [0x71],0x80
0000EA7B  A18000            mov ax,[0x80]
0000EA7E  A31C00            mov [0x1c],ax
0000EA81  A31A00            mov [0x1a],ax
0000EA84  CD1B              int 0x1b
0000EA86  2BC0              sub ax,ax
0000EA88  E9F100            jmp 0xeb7c
0000EA8B  80FC45            cmp ah,0x45
0000EA8E  7521              jnz 0xeab1
0000EA90  800E180008        or byte [0x18],0x8
0000EA95  B020              mov al,0x20
0000EA97  E620              out 0x20,al
0000EA99  803E490007        cmp byte [0x49],0x7
0000EA9E  7407              jz 0xeaa7
0000EAA0  BAD803            mov dx,0x3d8
0000EAA3  A06500            mov al,[0x65]
0000EAA6  EE                out dx,al
0000EAA7  F606180008        test byte [0x18],0x8
0000EAAC  75F9              jnz 0xeaa7
0000EAAE  E906FF            jmp 0xe9b7
0000EAB1  80FC03            cmp ah,0x3
0000EAB4  7505              jnz 0xeabb
0000EAB6  B000              mov al,0x0
0000EAB8  E9E900            jmp 0xeba4
0000EABB  3C1A              cmp al,0x1a
0000EABD  76F9              jna 0xeab8
0000EABF  BB16E9            mov bx,0xe916
0000EAC2  2C20              sub al,0x20
0000EAC4  2ED7              cs xlatb
0000EAC6  E9A100            jmp 0xeb6a
0000EAC9  80FC37            cmp ah,0x37
0000EACC  7509              jnz 0xead7
0000EACE  B020              mov al,0x20
0000EAD0  E620              out 0x20,al
0000EAD2  CD05              int 0x5
0000EAD4  E9E0FE            jmp 0xe9b7
0000EAD7  3C1A              cmp al,0x1a
0000EAD9  7705              ja 0xeae0
0000EADB  0440              add al,0x40
0000EADD  E98A00            jmp 0xeb6a
0000EAE0  BBF1E8            mov bx,0xe8f1
0000EAE3  2C20              sub al,0x20
0000EAE5  2ED7              cs xlatb
0000EAE7  E98000            jmp 0xeb6a
0000EAEA  2C47              sub al,0x47
0000EAEC  8A1E1700          mov bl,[0x17]
0000EAF0  F6C308            test bl,0x8
0000EAF3  7518              jnz 0xeb0d
0000EAF5  F6C304            test bl,0x4
0000EAF8  7550              jnz 0xeb4a
0000EAFA  F6C320            test bl,0x20
0000EAFD  7407              jz 0xeb06
0000EAFF  F6C303            test bl,0x3
0000EB02  7551              jnz 0xeb55
0000EB04  EB5B              jmp short 0xeb61
0000EB06  F6C303            test bl,0x3
0000EB09  744A              jz 0xeb55
0000EB0B  EB54              jmp short 0xeb61
0000EB0D  0AE4              or ah,ah
0000EB0F  7836              js 0xeb47
0000EB11  F606170004        test byte [0x17],0x4
0000EB16  7416              jz 0xeb2e
0000EB18  80FC53            cmp ah,0x53
0000EB1B  7509              jnz 0xeb26
0000EB1D  C70672003412      mov word [0x72],0x1234
0000EB23  E935F5            jmp 0xe05b
0000EB26  80FC4A            cmp ah,0x4a
0000EB29  7503              jnz 0xeb2e
0000EB2B  E8530E            call 0xf981
0000EB2E  BB60E9            mov bx,0xe960
0000EB31  2ED7              cs xlatb
0000EB33  3C30              cmp al,0x30
0000EB35  7210              jc 0xeb47
0000EB37  2C30              sub al,0x30
0000EB39  8AD8              mov bl,al
0000EB3B  A01900            mov al,[0x19]
0000EB3E  B40A              mov ah,0xa
0000EB40  F6E4              mul ah
0000EB42  02C3              add al,bl
0000EB44  A21900            mov [0x19],al
0000EB47  E969FE            jmp 0xe9b3
0000EB4A  0AE4              or ah,ah
0000EB4C  78F9              js 0xeb47
0000EB4E  BB6DE9            mov bx,0xe96d
0000EB51  2ED7              cs xlatb
0000EB53  EB15              jmp short 0xeb6a
0000EB55  E85D00            call 0xebb5
0000EB58  72ED              jc 0xeb47
0000EB5A  BB7AE9            mov bx,0xe97a
0000EB5D  2ED7              cs xlatb
0000EB5F  EB09              jmp short 0xeb6a
0000EB61  0AE4              or ah,ah
0000EB63  78E2              js 0xeb47
0000EB65  BB60E9            mov bx,0xe960
0000EB68  2ED7              cs xlatb
0000EB6A  3C05              cmp al,0x5
0000EB6C  7440              jz 0xebae
0000EB6E  3C04              cmp al,0x4
0000EB70  7704              ja 0xeb76
0000EB72  0C80              or al,0x80
0000EB74  EB06              jmp short 0xeb7c
0000EB76  A880              test al,0x80
0000EB78  7406              jz 0xeb80
0000EB7A  247F              and al,0x7f
0000EB7C  8AE0              mov ah,al
0000EB7E  B000              mov al,0x0
0000EB80  F606170040        test byte [0x17],0x40
0000EB85  741D              jz 0xeba4
0000EB87  F606170003        test byte [0x17],0x3
0000EB8C  740C              jz 0xeb9a
0000EB8E  3C41              cmp al,0x41
0000EB90  7212              jc 0xeba4
0000EB92  3C5A              cmp al,0x5a
0000EB94  770E              ja 0xeba4
0000EB96  0420              add al,0x20
0000EB98  EB0A              jmp short 0xeba4
0000EB9A  3C61              cmp al,0x61
0000EB9C  7206              jc 0xeba4
0000EB9E  3C7A              cmp al,0x7a
0000EBA0  7702              ja 0xeba4
0000EBA2  2C20              sub al,0x20
0000EBA4  E83600            call 0xebdd
0000EBA7  7305              jnc 0xebae
0000EBA9  B301              mov bl,0x1
0000EBAB  E8010C            call 0xf7af
0000EBAE  E902FE            jmp 0xe9b3
0000EBB1  B438              mov ah,0x38
0000EBB3  EBEF              jmp short 0xeba4
0000EBB5  80FCD2            cmp ah,0xd2
0000EBB8  7507              jnz 0xebc1
0000EBBA  802618007F        and byte [0x18],0x7f
0000EBBF  F9                stc
0000EBC0  C3                ret
0000EBC1  0AE4              or ah,ah
0000EBC3  78FA              js 0xebbf
0000EBC5  80FC52            cmp ah,0x52
0000EBC8  7511              jnz 0xebdb
0000EBCA  F606180080        test byte [0x18],0x80
0000EBCF  750A              jnz 0xebdb
0000EBD1  8036170080        xor byte [0x17],0x80
0000EBD6  800E180080        or byte [0x18],0x80
0000EBDB  F8                clc
0000EBDC  C3                ret
0000EBDD  8B1E1C00          mov bx,[0x1c]
0000EBE1  8BFB              mov di,bx
0000EBE3  43                inc bx
0000EBE4  43                inc bx
0000EBE5  3B1E8200          cmp bx,[0x82]
0000EBE9  7504              jnz 0xebef
0000EBEB  8B1E8000          mov bx,[0x80]
0000EBEF  3B1E1A00          cmp bx,[0x1a]
0000EBF3  7502              jnz 0xebf7
0000EBF5  F9                stc
0000EBF6  C3                ret
0000EBF7  8905              mov [di],ax
0000EBF9  891E1C00          mov [0x1c],bx
0000EBFD  F8                clc
0000EBFE  C3                ret
0000EBFF  0D0A54            or ax,0x540a
0000EC02  7572              jnz 0xec76
0000EC04  626F20            bound bp,[bx+0x20]
0000EC07  58                pop ax
0000EC08  54                push sp
0000EC09  204249            and [bp+si+0x49],al
0000EC0C  4F                dec di
0000EC0D  53                push bx
0000EC0E  207633            and [bp+0x33],dh
0000EC11  2E312E2055        xor [cs:0x5520],bp
0000EC16  7067              jo 0xec7f
0000EC18  7261              jc 0xec7b
0000EC1A  64657320          gs jnc 0xec3e
0000EC1E  627920            bound di,[bx+di+0x20]
0000EC21  59                pop cx
0000EC22  61                popa
0000EC23  60                pusha
0000EC24  61                popa
0000EC25  6B6F7620          imul bp,[bx+0x76],byte +0x20
0000EC29  4D                dec bp
0000EC2A  696C657320        imul bp,[si+0x65],word 0x2073
0000EC2F  26204A6F          and [es:bp+si+0x6f],cl
0000EC33  6E                outsb
0000EC34  205065            and [bx+si+0x65],dl
0000EC37  7472              jz 0xecab
0000EC39  6F                outsw
0000EC3A  736B              jnc 0xeca7
0000EC3C  790D              jns 0xec4b
0000EC3E  0A00              or al,[bx+si]
0000EC40  0000              add [bx+si],al
0000EC42  53                push bx
0000EC43  50                push ax
0000EC44  41                inc cx
0000EC45  43                inc bx
0000EC46  45                inc bp
0000EC47  20746F            and [si+0x6f],dh
0000EC4A  207275            and [bp+si+0x75],dh
0000EC4D  6E                outsb
0000EC4E  204241            and [bp+si+0x41],al
0000EC51  53                push bx
0000EC52  49                dec cx
0000EC53  43                inc bx
0000EC54  00909090          add [bx+si-0x6f70],dl
0000EC58  90                nop
0000EC59  FB                sti
0000EC5A  55                push bp
0000EC5B  56                push si
0000EC5C  57                push di
0000EC5D  1E                push ds
0000EC5E  06                push es
0000EC5F  53                push bx
0000EC60  8BF8              mov di,ax
0000EC62  33DB              xor bx,bx
0000EC64  8EDB              mov ds,bx
0000EC66  C4367800          les si,[0x78]
0000EC6A  B340              mov bl,0x40
0000EC6C  8EDB              mov ds,bx
0000EC6E  B305              mov bl,0x5
0000EC70  268B00            mov ax,[es:bx+si]
0000EC73  50                push ax
0000EC74  4B                dec bx
0000EC75  4B                dec bx
0000EC76  268B00            mov ax,[es:bx+si]
0000EC79  50                push ax
0000EC7A  86F1              xchg dh,cl
0000EC7C  86CA              xchg cl,dl
0000EC7E  52                push dx
0000EC7F  51                push cx
0000EC80  57                push di
0000EC81  8BEC              mov bp,sp
0000EC83  E82200            call 0xeca8
0000EC86  268A6402          mov ah,[es:si+0x2]
0000EC8A  88264000          mov [0x40],ah
0000EC8E  8A264100          mov ah,[0x41]
0000EC92  80FC01            cmp ah,0x1
0000EC95  F5                cmc
0000EC96  5B                pop bx
0000EC97  59                pop cx
0000EC98  5A                pop dx
0000EC99  86CA              xchg cl,dl
0000EC9B  86F1              xchg dh,cl
0000EC9D  5B                pop bx
0000EC9E  5B                pop bx
0000EC9F  5B                pop bx
0000ECA0  07                pop es
0000ECA1  1F                pop ds
0000ECA2  5F                pop di
0000ECA3  5E                pop si
0000ECA4  5D                pop bp
0000ECA5  CA0200            retf 0x2
0000ECA8  8A4601            mov al,[bp+0x1]
0000ECAB  0AC0              or al,al
0000ECAD  741B              jz 0xecca
0000ECAF  FEC8              dec al
0000ECB1  7413              jz 0xecc6
0000ECB3  807E0203          cmp byte [bp+0x2],0x3
0000ECB7  7704              ja 0xecbd
0000ECB9  3C05              cmp al,0x5
0000ECBB  7606              jna 0xecc3
0000ECBD  C606410001        mov byte [0x41],0x1
0000ECC2  C3                ret
0000ECC3  E98100            jmp 0xed47
0000ECC6  A04100            mov al,[0x41]
0000ECC9  C3                ret
0000ECCA  BAF203            mov dx,0x3f2
0000ECCD  FA                cli
0000ECCE  80263F000F        and byte [0x3f],0xf
0000ECD3  A03F00            mov al,[0x3f]
0000ECD6  B104              mov cl,0x4
0000ECD8  D2E0              shl al,cl
0000ECDA  A820              test al,0x20
0000ECDC  750C              jnz 0xecea
0000ECDE  A840              test al,0x40
0000ECE0  7506              jnz 0xece8
0000ECE2  A880              test al,0x80
0000ECE4  7406              jz 0xecec
0000ECE6  FEC0              inc al
0000ECE8  FEC0              inc al
0000ECEA  FEC0              inc al
0000ECEC  C6063E0000        mov byte [0x3e],0x0
0000ECF1  C606410000        mov byte [0x41],0x0
0000ECF6  0C08              or al,0x8
0000ECF8  EE                out dx,al
0000ECF9  0C04              or al,0x4
0000ECFB  EE                out dx,al
0000ECFC  FB                sti
0000ECFD  E8B201            call 0xeeb2
0000ED00  E86702            call 0xef6a
0000ED03  A04200            mov al,[0x42]
0000ED06  3CC0              cmp al,0xc0
0000ED08  7407              jz 0xed11
0000ED0A  C606410020        mov byte [0x41],0x20
0000ED0F  EB12              jmp short 0xed23
0000ED11  B003              mov al,0x3
0000ED13  E87C02            call 0xef92
0000ED16  268A04            mov al,[es:si]
0000ED19  E87602            call 0xef92
0000ED1C  268A4401          mov al,[es:si+0x1]
0000ED20  E86F02            call 0xef92
0000ED23  C3                ret
0000ED24  0300              add ax,[bx+si]
0000ED26  E6C5              out 0xc5,al
0000ED28  E64D              out 0x4d,al
0000ED2A  0000              add [bx+si],al
0000ED2C  46                inc si
0000ED2D  4A                dec dx
0000ED2E  42                inc dx
0000ED2F  4A                dec dx
0000ED30  0000              add [bx+si],al
0000ED32  00800080          add [bx+si-0x8000],al
0000ED36  0102              add [bp+si],ax
0000ED38  0408              add al,0x8
0000ED3A  802010            and byte [bx+si],0x10
0000ED3D  0402              add al,0x2
0000ED3F  0104              add [si],ax
0000ED41  1008              adc [bx+si],cl
0000ED43  0403              add al,0x3
0000ED45  0220              add ah,[bx+si]
0000ED47  FA                cli
0000ED48  B400              mov ah,0x0
0000ED4A  8A4601            mov al,[bp+0x1]
0000ED4D  88264100          mov [0x41],ah
0000ED51  8BF8              mov di,ax
0000ED53  E60C              out 0xc,al
0000ED55  2E8A852AED        mov al,[cs:di-0x12d6]
0000ED5A  E60B              out 0xb,al
0000ED5C  8B460C            mov ax,[bp+0xc]
0000ED5F  B104              mov cl,0x4
0000ED61  D3C0              rol ax,cl
0000ED63  8AE8              mov ch,al
0000ED65  80E50F            and ch,0xf
0000ED68  24F0              and al,0xf0
0000ED6A  03460A            add ax,[bp+0xa]
0000ED6D  80D500            adc ch,0x0
0000ED70  8BD0              mov dx,ax
0000ED72  E604              out 0x4,al
0000ED74  8AC4              mov al,ah
0000ED76  E604              out 0x4,al
0000ED78  8AC5              mov al,ch
0000ED7A  E681              out 0x81,al
0000ED7C  8A6600            mov ah,[bp+0x0]
0000ED7F  B000              mov al,0x0
0000ED81  D1E8              shr ax,1
0000ED83  8A4E06            mov cl,[bp+0x6]
0000ED86  D3E0              shl ax,cl
0000ED88  48                dec ax
0000ED89  E605              out 0x5,al
0000ED8B  86E0              xchg ah,al
0000ED8D  E605              out 0x5,al
0000ED8F  86E0              xchg ah,al
0000ED91  03C2              add ax,dx
0000ED93  7309              jnc 0xed9e
0000ED95  FB                sti
0000ED96  C606410009        mov byte [0x41],0x9
0000ED9B  E91101            jmp 0xeeaf
0000ED9E  B002              mov al,0x2
0000EDA0  E60A              out 0xa,al
0000EDA2  C6064000FF        mov byte [0x40],0xff
0000EDA7  8A5E02            mov bl,[bp+0x2]
0000EDAA  B700              mov bh,0x0
0000EDAC  2E8A8736ED        mov al,[cs:bx-0x12ca]
0000EDB1  8AE8              mov ch,al
0000EDB3  B104              mov cl,0x4
0000EDB5  D2E0              shl al,cl
0000EDB7  0AC3              or al,bl
0000EDB9  0C0C              or al,0xc
0000EDBB  BAF203            mov dx,0x3f2
0000EDBE  EE                out dx,al
0000EDBF  FB                sti
0000EDC0  2E8A8530ED        mov al,[cs:di-0x12d0]
0000EDC5  08063F00          or [0x3f],al
0000EDC9  0AC0              or al,al
0000EDCB  7911              jns 0xedde
0000EDCD  268A640A          mov ah,[es:si+0xa]
0000EDD1  0AE4              or ah,ah
0000EDD3  7409              jz 0xedde
0000EDD5  842E3F00          test [0x3f],ch
0000EDD9  7503              jnz 0xedde
0000EDDB  E82701            call 0xef05
0000EDDE  082E3F00          or [0x3f],ch
0000EDE2  842E3E00          test [0x3e],ch
0000EDE6  7514              jnz 0xedfc
0000EDE8  082E3E00          or [0x3e],ch
0000EDEC  B007              mov al,0x7
0000EDEE  E8A101            call 0xef92
0000EDF1  8AC3              mov al,bl
0000EDF3  E89C01            call 0xef92
0000EDF6  E8B900            call 0xeeb2
0000EDF9  E8BD01            call 0xefb9
0000EDFC  B00F              mov al,0xf
0000EDFE  E89101            call 0xef92
0000EE01  8AC3              mov al,bl
0000EE03  E88C01            call 0xef92
0000EE06  8A4603            mov al,[bp+0x3]
0000EE09  E88601            call 0xef92
0000EE0C  E8A300            call 0xeeb2
0000EE0F  E85801            call 0xef6a
0000EE12  268A4409          mov al,[es:si+0x9]
0000EE16  0AC0              or al,al
0000EE18  7409              jz 0xee23
0000EE1A  B92602            mov cx,0x226
0000EE1D  E2FE              loop 0xee1d
0000EE1F  FEC8              dec al
0000EE21  75F7              jnz 0xee1a
0000EE23  2E8A8524ED        mov al,[cs:di-0x12dc]
0000EE28  E86701            call 0xef92
0000EE2B  8A4604            mov al,[bp+0x4]
0000EE2E  2401              and al,0x1
0000EE30  D0E0              shl al,1
0000EE32  D0E0              shl al,1
0000EE34  0AC3              or al,bl
0000EE36  E85901            call 0xef92
0000EE39  807E0105          cmp byte [bp+0x1],0x5
0000EE3D  751C              jnz 0xee5b
0000EE3F  8A4606            mov al,[bp+0x6]
0000EE42  E84D01            call 0xef92
0000EE45  8A4607            mov al,[bp+0x7]
0000EE48  E84701            call 0xef92
0000EE4B  268A4407          mov al,[es:si+0x7]
0000EE4F  E84001            call 0xef92
0000EE52  268A4408          mov al,[es:si+0x8]
0000EE56  E83901            call 0xef92
0000EE59  EB0E              jmp short 0xee69
0000EE5B  B90700            mov cx,0x7
0000EE5E  BF0300            mov di,0x3
0000EE61  8A03              mov al,[bp+di]
0000EE63  E82C01            call 0xef92
0000EE66  47                inc di
0000EE67  E2F8              loop 0xee61
0000EE69  E84600            call 0xeeb2
0000EE6C  E80001            call 0xef6f
0000EE6F  A04200            mov al,[0x42]
0000EE72  24C0              and al,0xc0
0000EE74  7425              jz 0xee9b
0000EE76  3C40              cmp al,0x40
0000EE78  7407              jz 0xee81
0000EE7A  C606410020        mov byte [0x41],0x20
0000EE7F  EB1A              jmp short 0xee9b
0000EE81  A04300            mov al,[0x43]
0000EE84  B90600            mov cx,0x6
0000EE87  33DB              xor bx,bx
0000EE89  2E84873AED        test [cs:bx-0x12c6],al
0000EE8E  7503              jnz 0xee93
0000EE90  43                inc bx
0000EE91  E2F6              loop 0xee89
0000EE93  2E8A8740ED        mov al,[cs:bx-0x12c0]
0000EE98  A24100            mov [0x41],al
0000EE9B  A04500            mov al,[0x45]
0000EE9E  3A4603            cmp al,[bp+0x3]
0000EEA1  A04700            mov al,[0x47]
0000EEA4  7405              jz 0xeeab
0000EEA6  8A4607            mov al,[bp+0x7]
0000EEA9  FEC0              inc al
0000EEAB  2A4605            sub al,[bp+0x5]
0000EEAE  C3                ret
0000EEAF  B000              mov al,0x0
0000EEB1  C3                ret
0000EEB2  FB                sti
0000EEB3  33C9              xor cx,cx
0000EEB5  B002              mov al,0x2
0000EEB7  F6063E0080        test byte [0x3e],0x80
0000EEBC  F8                clc
0000EEBD  7510              jnz 0xeecf
0000EEBF  E2F6              loop 0xeeb7
0000EEC1  FEC8              dec al
0000EEC3  75F2              jnz 0xeeb7
0000EEC5  C606410080        mov byte [0x41],0x80
0000EECA  58                pop ax
0000EECB  B000              mov al,0x0
0000EECD  F9                stc
0000EECE  C3                ret
0000EECF  80263E007F        and byte [0x3e],0x7f
0000EED4  C3                ret
0000EED5  51                push cx
0000EED6  33C9              xor cx,cx
0000EED8  BAF403            mov dx,0x3f4
0000EEDB  EC                in al,dx
0000EEDC  0AC0              or al,al
0000EEDE  7809              js 0xeee9
0000EEE0  E2F9              loop 0xeedb
0000EEE2  C606410080        mov byte [0x41],0x80
0000EEE7  EB09              jmp short 0xeef2
0000EEE9  A840              test al,0x40
0000EEEB  7508              jnz 0xeef5
0000EEED  C606410020        mov byte [0x41],0x20
0000EEF2  59                pop cx
0000EEF3  F9                stc
0000EEF4  C3                ret
0000EEF5  42                inc dx
0000EEF6  EC                in al,dx
0000EEF7  50                push ax
0000EEF8  B90A00            mov cx,0xa
0000EEFB  E2FE              loop 0xeefb
0000EEFD  4A                dec dx
0000EEFE  EC                in al,dx
0000EEFF  A810              test al,0x10
0000EF01  F8                clc
0000EF02  58                pop ax
0000EF03  59                pop cx
0000EF04  C3                ret
0000EF05  51                push cx
0000EF06  33C9              xor cx,cx
0000EF08  E2FE              loop 0xef08
0000EF0A  FECC              dec ah
0000EF0C  75F8              jnz 0xef06
0000EF0E  59                pop cx
0000EF0F  C3                ret
0000EF10  BD0400            mov bp,0x4
0000EF13  BB00B0            mov bx,0xb000
0000EF16  3C07              cmp al,0x7
0000EF18  7405              jz 0xef1f
0000EF1A  BD1000            mov bp,0x10
0000EF1D  B7B8              mov bh,0xb8
0000EF1F  53                push bx
0000EF20  07                pop es
0000EF21  A06500            mov al,[0x65]
0000EF24  24F7              and al,0xf7
0000EF26  8B166300          mov dx,[0x63]
0000EF2A  83C204            add dx,byte +0x4
0000EF2D  EE                out dx,al
0000EF2E  E8FA09            call 0xf92b
0000EF31  4D                dec bp
0000EF32  75FA              jnz 0xef2e
0000EF34  7305              jnc 0xef3b
0000EF36  800E150004        or byte [0x15],0x4
0000EF3B  C3                ret
0000EF3C  3C53              cmp al,0x53
0000EF3E  7511              jnz 0xef51
0000EF40  8A0E1700          mov cl,[0x17]
0000EF44  F6C104            test cl,0x4
0000EF47  7408              jz 0xef51
0000EF49  F6C108            test cl,0x8
0000EF4C  7403              jz 0xef51
0000EF4E  E9CCFB            jmp 0xeb1d
0000EF51  C3                ret
0000EF52  90                nop
0000EF53  90                nop
0000EF54  90                nop
0000EF55  90                nop
0000EF56  90                nop
0000EF57  FB                sti
0000EF58  1E                push ds
0000EF59  50                push ax
0000EF5A  33C0              xor ax,ax
0000EF5C  8ED8              mov ds,ax
0000EF5E  800E3E0480        or byte [0x43e],0x80
0000EF63  B020              mov al,0x20
0000EF65  E620              out 0x20,al
0000EF67  58                pop ax
0000EF68  1F                pop ds
0000EF69  CF                iret
0000EF6A  B008              mov al,0x8
0000EF6C  E82300            call 0xef92
0000EF6F  53                push bx
0000EF70  51                push cx
0000EF71  B90700            mov cx,0x7
0000EF74  33DB              xor bx,bx
0000EF76  E85CFF            call 0xeed5
0000EF79  720D              jc 0xef88
0000EF7B  884742            mov [bx+0x42],al
0000EF7E  740F              jz 0xef8f
0000EF80  43                inc bx
0000EF81  E2F3              loop 0xef76
0000EF83  C606410020        mov byte [0x41],0x20
0000EF88  F9                stc
0000EF89  59                pop cx
0000EF8A  5B                pop bx
0000EF8B  58                pop ax
0000EF8C  B000              mov al,0x0
0000EF8E  C3                ret
0000EF8F  59                pop cx
0000EF90  5B                pop bx
0000EF91  C3                ret
0000EF92  51                push cx
0000EF93  52                push dx
0000EF94  50                push ax
0000EF95  33C9              xor cx,cx
0000EF97  BAF403            mov dx,0x3f4
0000EF9A  EC                in al,dx
0000EF9B  0AC0              or al,al
0000EF9D  7809              js 0xefa8
0000EF9F  E2F9              loop 0xef9a
0000EFA1  C606410080        mov byte [0x41],0x80
0000EFA6  EB12              jmp short 0xefba
0000EFA8  A840              test al,0x40
0000EFAA  7407              jz 0xefb3
0000EFAC  C606410020        mov byte [0x41],0x20
0000EFB1  EB07              jmp short 0xefba
0000EFB3  42                inc dx
0000EFB4  58                pop ax
0000EFB5  EE                out dx,al
0000EFB6  F8                clc
0000EFB7  5A                pop dx
0000EFB8  59                pop cx
0000EFB9  C3                ret
0000EFBA  58                pop ax
0000EFBB  5A                pop dx
0000EFBC  59                pop cx
0000EFBD  58                pop ax
0000EFBE  B000              mov al,0x0
0000EFC0  F9                stc
0000EFC1  C3                ret
0000EFC2  90                nop
0000EFC3  90                nop
0000EFC4  90                nop
0000EFC5  90                nop
0000EFC6  90                nop
0000EFC7  CF                iret
0000EFC8  0225              add ah,[di]
0000EFCA  0208              add cl,[bx+si]
0000EFCC  2AFF              sub bh,bh
0000EFCE  50                push ax
0000EFCF  F619              neg byte [bx+di]
0000EFD1  04FB              add al,0xfb
0000EFD3  1E                push ds
0000EFD4  53                push bx
0000EFD5  51                push cx
0000EFD6  52                push dx
0000EFD7  BB4000            mov bx,0x40
0000EFDA  8EDB              mov ds,bx
0000EFDC  8BDA              mov bx,dx
0000EFDE  D1E3              shl bx,1
0000EFE0  8B5708            mov dx,[bx+0x8]
0000EFE3  0BD2              or dx,dx
0000EFE5  740C              jz 0xeff3
0000EFE7  0AE4              or ah,ah
0000EFE9  740D              jz 0xeff8
0000EFEB  FECC              dec ah
0000EFED  7439              jz 0xf028
0000EFEF  FECC              dec ah
0000EFF1  7429              jz 0xf01c
0000EFF3  5A                pop dx
0000EFF4  59                pop cx
0000EFF5  5B                pop bx
0000EFF6  1F                pop ds
0000EFF7  CF                iret
0000EFF8  EE                out dx,al
0000EFF9  42                inc dx
0000EFFA  8A7F78            mov bh,[bx+0x78]
0000EFFD  8AE0              mov ah,al
0000EFFF  33C9              xor cx,cx
0000F001  EC                in al,dx
0000F002  0AC0              or al,al
0000F004  780C              js 0xf012
0000F006  E2F9              loop 0xf001
0000F008  FECF              dec bh
0000F00A  75F3              jnz 0xefff
0000F00C  0C01              or al,0x1
0000F00E  24F9              and al,0xf9
0000F010  EB10              jmp short 0xf022
0000F012  42                inc dx
0000F013  B00D              mov al,0xd
0000F015  EE                out dx,al
0000F016  B00C              mov al,0xc
0000F018  EE                out dx,al
0000F019  4A                dec dx
0000F01A  EB03              jmp short 0xf01f
0000F01C  8AE0              mov ah,al
0000F01E  42                inc dx
0000F01F  EC                in al,dx
0000F020  24F8              and al,0xf8
0000F022  3448              xor al,0x48
0000F024  86E0              xchg ah,al
0000F026  EBCB              jmp short 0xeff3
0000F028  8AE0              mov ah,al
0000F02A  42                inc dx
0000F02B  42                inc dx
0000F02C  B008              mov al,0x8
0000F02E  EE                out dx,al
0000F02F  B9DC05            mov cx,0x5dc
0000F032  E2FE              loop 0xf032
0000F034  EBE0              jmp short 0xf016
0000F036  90                nop
0000F037  90                nop
0000F038  90                nop
0000F039  90                nop
0000F03A  90                nop
0000F03B  90                nop
0000F03C  90                nop
0000F03D  90                nop
0000F03E  90                nop
0000F03F  90                nop
0000F040  90                nop
0000F041  90                nop
0000F042  90                nop
0000F043  90                nop
0000F044  90                nop
0000F045  15F1BE            adc ax,0xbef1
0000F048  F1                int1
0000F049  CB                retf
0000F04A  F1                int1
0000F04B  DBF1              fcomi st1
0000F04D  F7F1              div cx
0000F04F  78F2              js 0xf043
0000F051  98                cbw
0000F052  F298              repne cbw
0000F054  F2E4F3            repne in al,0xf3
0000F057  E4F3              in al,0xf3
0000F059  E4F3              in al,0xf3
0000F05B  BEF5E9            mov si,0xe9f5
0000F05E  F5                cmc
0000F05F  2CF6              sub al,0xf6
0000F061  55                push bp
0000F062  F6D3              not bl
0000F064  F6FB              idiv bl
0000F066  FC                cld
0000F067  55                push bp
0000F068  06                push es
0000F069  1E                push ds
0000F06A  56                push si
0000F06B  57                push di
0000F06C  52                push dx
0000F06D  51                push cx
0000F06E  53                push bx
0000F06F  50                push ax
0000F070  BB4000            mov bx,0x40
0000F073  8EDB              mov ds,bx
0000F075  8A1E1000          mov bl,[0x10]
0000F079  80E330            and bl,0x30
0000F07C  80FB30            cmp bl,0x30
0000F07F  BB00B8            mov bx,0xb800
0000F082  7502              jnz 0xf086
0000F084  B7B0              mov bh,0xb0
0000F086  53                push bx
0000F087  8BEC              mov bp,sp
0000F089  E87800            call 0xf104
0000F08C  5E                pop si
0000F08D  58                pop ax
0000F08E  5B                pop bx
0000F08F  59                pop cx
0000F090  5A                pop dx
0000F091  5F                pop di
0000F092  5E                pop si
0000F093  1F                pop ds
0000F094  07                pop es
0000F095  5D                pop bp
0000F096  CF                iret
0000F097  52                push dx
0000F098  B400              mov ah,0x0
0000F09A  F7E3              mul bx
0000F09C  5A                pop dx
0000F09D  8B4E00            mov cx,[bp+0x0]
0000F0A0  C3                ret
0000F0A1  C00090            rol byte [bx+si],byte 0x90
0000F0A4  3828              cmp [bx+si],ch
0000F0A6  2D0A1F            sub ax,0x1f0a
0000F0A9  06                push es
0000F0AA  191C              sbb [si],bx
0000F0AC  0207              add al,[bx]
0000F0AE  06                push es
0000F0AF  07                pop es
0000F0B0  0000              add [bx+si],al
0000F0B2  0000              add [bx+si],al
0000F0B4  7150              jno 0xf106
0000F0B6  5A                pop dx
0000F0B7  0A1F              or bl,[bx]
0000F0B9  06                push es
0000F0BA  191C              sbb [si],bx
0000F0BC  0207              add al,[bx]
0000F0BE  06                push es
0000F0BF  07                pop es
0000F0C0  0000              add [bx+si],al
0000F0C2  0000              add [bx+si],al
0000F0C4  3828              cmp [bx+si],ch
0000F0C6  2D0A7F            sub ax,0x7f0a
0000F0C9  06                push es
0000F0CA  647002            fs jo 0xf0cf
0000F0CD  01060700          add [0x7],ax
0000F0D1  0000              add [bx+si],al
0000F0D3  006150            add [bx+di+0x50],ah
0000F0D6  52                push dx
0000F0D7  0F19061919        hint_nop8 word [0x1919]
0000F0DC  020D              add cl,[di]
0000F0DE  0B0C              or cx,[si]
0000F0E0  0000              add [bx+si],al
0000F0E2  0000              add [bx+si],al
0000F0E4  0008              add [bx+si],cl
0000F0E6  0010              add [bx+si],dl
0000F0E8  004000            add [bx+si+0x0],al
0000F0EB  40                inc ax
0000F0EC  2828              sub [bx+si],ch
0000F0EE  50                push ax
0000F0EF  50                push ax
0000F0F0  2828              sub [bx+si],ch
0000F0F2  50                push ax
0000F0F3  50                push ax
0000F0F4  2C28              sub al,0x28
0000F0F6  2D292A            sub ax,0x2a29
0000F0F9  2E1E              cs push ds
0000F0FB  2900              sub [bx+si],ax
0000F0FD  0010              add [bx+si],dl
0000F0FF  1020              adc [bx+si],ah
0000F101  2020              and [bx+si],ah
0000F103  3080FC0F          xor [bx+si+0xffc],al
0000F107  7601              jna 0xf10a
0000F109  C3                ret
0000F10A  D0E4              shl ah,1
0000F10C  8ADC              mov bl,ah
0000F10E  B700              mov bh,0x0
0000F110  2EFFA745F0        jmp [cs:bx-0xfbb]
0000F115  A01000            mov al,[0x10]
0000F118  BAB403            mov dx,0x3b4
0000F11B  2430              and al,0x30
0000F11D  3C30              cmp al,0x30
0000F11F  B001              mov al,0x1
0000F121  B307              mov bl,0x7
0000F123  740C              jz 0xf131
0000F125  8A5E02            mov bl,[bp+0x2]
0000F128  80FB07            cmp bl,0x7
0000F12B  77DC              ja 0xf109
0000F12D  B2D4              mov dl,0xd4
0000F12F  FEC8              dec al
0000F131  89166300          mov [0x63],dx
0000F135  80C204            add dl,0x4
0000F138  EE                out dx,al
0000F139  881E4900          mov [0x49],bl
0000F13D  1E                push ds
0000F13E  33C0              xor ax,ax
0000F140  8ED8              mov ds,ax
0000F142  C4367400          les si,[0x74]
0000F146  1F                pop ds
0000F147  B700              mov bh,0x0
0000F149  53                push bx
0000F14A  2E8A9FFCF0        mov bl,[cs:bx-0xf04]
0000F14F  03F3              add si,bx
0000F151  B91000            mov cx,0x10
0000F154  268A04            mov al,[es:si]
0000F157  E82A06            call 0xf784
0000F15A  FEC4              inc ah
0000F15C  46                inc si
0000F15D  E2F5              loop 0xf154
0000F15F  8B5E00            mov bx,[bp+0x0]
0000F162  8EC3              mov es,bx
0000F164  33FF              xor di,di
0000F166  E87D05            call 0xf6e6
0000F169  B520              mov ch,0x20
0000F16B  B80000            mov ax,0x0
0000F16E  7207              jc 0xf177
0000F170  7502              jnz 0xf174
0000F172  B508              mov ch,0x8
0000F174  B82007            mov ax,0x720
0000F177  F3AB              rep stosw
0000F179  8B166300          mov dx,[0x63]
0000F17D  80C204            add dl,0x4
0000F180  5B                pop bx
0000F181  2E8A87F4F0        mov al,[cs:bx-0xf0c]
0000F186  EE                out dx,al
0000F187  A26500            mov [0x65],al
0000F18A  42                inc dx
0000F18B  B030              mov al,0x30
0000F18D  80FB06            cmp bl,0x6
0000F190  7502              jnz 0xf194
0000F192  B03F              mov al,0x3f
0000F194  A26600            mov [0x66],al
0000F197  EE                out dx,al
0000F198  33C0              xor ax,ax
0000F19A  A34E00            mov [0x4e],ax
0000F19D  A26200            mov [0x62],al
0000F1A0  B108              mov cl,0x8
0000F1A2  BF5000            mov di,0x50
0000F1A5  8905              mov [di],ax
0000F1A7  47                inc di
0000F1A8  E2FB              loop 0xf1a5
0000F1AA  2E8A87ECF0        mov al,[cs:bx-0xf14]
0000F1AF  A34A00            mov [0x4a],ax
0000F1B2  80E3FE            and bl,0xfe
0000F1B5  2E8B87E4F0        mov ax,[cs:bx-0xf1c]
0000F1BA  A34C00            mov [0x4c],ax
0000F1BD  C3                ret
0000F1BE  8B4E06            mov cx,[bp+0x6]
0000F1C1  890E6000          mov [0x60],cx
0000F1C5  B40A              mov ah,0xa
0000F1C7  E8B105            call 0xf77b
0000F1CA  C3                ret
0000F1CB  8A5E05            mov bl,[bp+0x5]
0000F1CE  D0E3              shl bl,1
0000F1D0  B700              mov bh,0x0
0000F1D2  8B4608            mov ax,[bp+0x8]
0000F1D5  894750            mov [bx+0x50],ax
0000F1D8  E98B05            jmp 0xf766
0000F1DB  8A5E05            mov bl,[bp+0x5]
0000F1DE  D0E3              shl bl,1
0000F1E0  B700              mov bh,0x0
0000F1E2  8B4750            mov ax,[bx+0x50]
0000F1E5  894608            mov [bp+0x8],ax
0000F1E8  A16000            mov ax,[0x60]
0000F1EB  894606            mov [bp+0x6],ax
0000F1EE  C3                ret
0000F1EF  0303              add ax,[bp+di]
0000F1F1  050503            add ax,0x305
0000F1F4  0303              add ax,[bp+di]
0000F1F6  048B              add al,0x8b
0000F1F8  16                push ss
0000F1F9  6300              arpl [bx+si],ax
0000F1FB  80C206            add dl,0x6
0000F1FE  C6460300          mov byte [bp+0x3],0x0
0000F202  EC                in al,dx
0000F203  A804              test al,0x4
0000F205  7468              jz 0xf26f
0000F207  A802              test al,0x2
0000F209  7501              jnz 0xf20c
0000F20B  C3                ret
0000F20C  B410              mov ah,0x10
0000F20E  E81005            call 0xf721
0000F211  8A1E4900          mov bl,[0x49]
0000F215  8ACB              mov cl,bl
0000F217  B700              mov bh,0x0
0000F219  2E8A9FEFF1        mov bl,[cs:bx-0xe11]
0000F21E  2BCB              sub cx,bx
0000F220  7902              jns 0xf224
0000F222  33C0              xor ax,ax
0000F224  E8BF04            call 0xf6e6
0000F227  7325              jnc 0xf24e
0000F229  B528              mov ch,0x28
0000F22B  F6F2              div dl
0000F22D  8ADC              mov bl,ah
0000F22F  B700              mov bh,0x0
0000F231  B103              mov cl,0x3
0000F233  D3E3              shl bx,cl
0000F235  8AE8              mov ch,al
0000F237  D0E5              shl ch,1
0000F239  8AD4              mov dl,ah
0000F23B  8AF0              mov dh,al
0000F23D  D0EE              shr dh,1
0000F23F  D0EE              shr dh,1
0000F241  803E490006        cmp byte [0x49],0x6
0000F246  751A              jnz 0xf262
0000F248  D0E2              shl dl,1
0000F24A  D1E3              shl bx,1
0000F24C  EB14              jmp short 0xf262
0000F24E  F6364A00          div byte [0x4a]
0000F252  86E0              xchg ah,al
0000F254  8BD0              mov dx,ax
0000F256  B103              mov cl,0x3
0000F258  D2E4              shl ah,cl
0000F25A  8AEC              mov ch,ah
0000F25C  8AD8              mov bl,al
0000F25E  B700              mov bh,0x0
0000F260  D3E3              shl bx,cl
0000F262  C6460301          mov byte [bp+0x3],0x1
0000F266  895608            mov [bp+0x8],dx
0000F269  895E04            mov [bp+0x4],bx
0000F26C  886E07            mov [bp+0x7],ch
0000F26F  8B166300          mov dx,[0x63]
0000F273  83C207            add dx,byte +0x7
0000F276  EE                out dx,al
0000F277  C3                ret
0000F278  8A4602            mov al,[bp+0x2]
0000F27B  A26200            mov [0x62],al
0000F27E  B400              mov ah,0x0
0000F280  50                push ax
0000F281  8B1E4C00          mov bx,[0x4c]
0000F285  F7E3              mul bx
0000F287  A34E00            mov [0x4e],ax
0000F28A  D1E8              shr ax,1
0000F28C  8BC8              mov cx,ax
0000F28E  B40C              mov ah,0xc
0000F290  E8E804            call 0xf77b
0000F293  5B                pop bx
0000F294  E8D704            call 0xf76e
0000F297  C3                ret
0000F298  E84B04            call 0xf6e6
0000F29B  7303              jnc 0xf2a0
0000F29D  E99B00            jmp 0xf33b
0000F2A0  FC                cld
0000F2A1  803E490002        cmp byte [0x49],0x2
0000F2A6  7215              jc 0xf2bd
0000F2A8  803E490003        cmp byte [0x49],0x3
0000F2AD  770E              ja 0xf2bd
0000F2AF  BADA03            mov dx,0x3da
0000F2B2  EC                in al,dx
0000F2B3  A808              test al,0x8
0000F2B5  74FB              jz 0xf2b2
0000F2B7  BAD803            mov dx,0x3d8
0000F2BA  B025              mov al,0x25
0000F2BC  EE                out dx,al
0000F2BD  8B4608            mov ax,[bp+0x8]
0000F2C0  50                push ax
0000F2C1  807E0307          cmp byte [bp+0x3],0x7
0000F2C5  7403              jz 0xf2ca
0000F2C7  8B4606            mov ax,[bp+0x6]
0000F2CA  E87504            call 0xf742
0000F2CD  03064E00          add ax,[0x4e]
0000F2D1  8BF0              mov si,ax
0000F2D3  8BF8              mov di,ax
0000F2D5  5A                pop dx
0000F2D6  2B5606            sub dx,[bp+0x6]
0000F2D9  81C20101          add dx,0x101
0000F2DD  8B1E4A00          mov bx,[0x4a]
0000F2E1  D1E3              shl bx,1
0000F2E3  1E                push ds
0000F2E4  8A4602            mov al,[bp+0x2]
0000F2E7  E8ADFD            call 0xf097
0000F2EA  8EC1              mov es,cx
0000F2EC  8ED9              mov ds,cx
0000F2EE  807E0306          cmp byte [bp+0x3],0x6
0000F2F2  7405              jz 0xf2f9
0000F2F4  F7D8              neg ax
0000F2F6  F7DB              neg bx
0000F2F8  FD                std
0000F2F9  8A4E02            mov cl,[bp+0x2]
0000F2FC  0AC9              or cl,cl
0000F2FE  741A              jz 0xf31a
0000F300  03F0              add si,ax
0000F302  2A7602            sub dh,[bp+0x2]
0000F305  B500              mov ch,0x0
0000F307  8ACA              mov cl,dl
0000F309  57                push di
0000F30A  56                push si
0000F30B  F3A5              rep movsw
0000F30D  5E                pop si
0000F30E  5F                pop di
0000F30F  03F3              add si,bx
0000F311  03FB              add di,bx
0000F313  FECE              dec dh
0000F315  75EE              jnz 0xf305
0000F317  8A7602            mov dh,[bp+0x2]
0000F31A  B500              mov ch,0x0
0000F31C  8A6605            mov ah,[bp+0x5]
0000F31F  B020              mov al,0x20
0000F321  8ACA              mov cl,dl
0000F323  57                push di
0000F324  F3AB              rep stosw
0000F326  5F                pop di
0000F327  03FB              add di,bx
0000F329  FECE              dec dh
0000F32B  75F4              jnz 0xf321
0000F32D  1F                pop ds
0000F32E  E8B503            call 0xf6e6
0000F331  7407              jz 0xf33a
0000F333  A06500            mov al,[0x65]
0000F336  BAD803            mov dx,0x3d8
0000F339  EE                out dx,al
0000F33A  C3                ret
0000F33B  FC                cld
0000F33C  8B4608            mov ax,[bp+0x8]
0000F33F  50                push ax
0000F340  807E0307          cmp byte [bp+0x3],0x7
0000F344  7403              jz 0xf349
0000F346  8B4606            mov ax,[bp+0x6]
0000F349  E80704            call 0xf753
0000F34C  8BF8              mov di,ax
0000F34E  5A                pop dx
0000F34F  2B5606            sub dx,[bp+0x6]
0000F352  81C20101          add dx,0x101
0000F356  D0E6              shl dh,1
0000F358  D0E6              shl dh,1
0000F35A  8A4603            mov al,[bp+0x3]
0000F35D  803E490006        cmp byte [0x49],0x6
0000F362  7409              jz 0xf36d
0000F364  D0E2              shl dl,1
0000F366  D1E7              shl di,1
0000F368  3C07              cmp al,0x7
0000F36A  7501              jnz 0xf36d
0000F36C  47                inc di
0000F36D  3C07              cmp al,0x7
0000F36F  7504              jnz 0xf375
0000F371  81C7F000          add di,0xf0
0000F375  8A5E02            mov bl,[bp+0x2]
0000F378  D0E3              shl bl,1
0000F37A  D0E3              shl bl,1
0000F37C  53                push bx
0000F37D  2AF3              sub dh,bl
0000F37F  B050              mov al,0x50
0000F381  F6E3              mul bl
0000F383  BBB01F            mov bx,0x1fb0
0000F386  807E0306          cmp byte [bp+0x3],0x6
0000F38A  7406              jz 0xf392
0000F38C  F7D8              neg ax
0000F38E  BB5020            mov bx,0x2050
0000F391  FD                std
0000F392  8BF7              mov si,di
0000F394  03F0              add si,ax
0000F396  58                pop ax
0000F397  0AC0              or al,al
0000F399  8B4E00            mov cx,[bp+0x0]
0000F39C  8ED9              mov ds,cx
0000F39E  8EC1              mov es,cx
0000F3A0  7426              jz 0xf3c8
0000F3A2  50                push ax
0000F3A3  B500              mov ch,0x0
0000F3A5  8ACA              mov cl,dl
0000F3A7  56                push si
0000F3A8  57                push di
0000F3A9  F3A4              rep movsb
0000F3AB  5F                pop di
0000F3AC  5E                pop si
0000F3AD  81C60020          add si,0x2000
0000F3B1  81C70020          add di,0x2000
0000F3B5  8ACA              mov cl,dl
0000F3B7  56                push si
0000F3B8  57                push di
0000F3B9  F3A4              rep movsb
0000F3BB  5F                pop di
0000F3BC  5E                pop si
0000F3BD  2BF3              sub si,bx
0000F3BF  2BFB              sub di,bx
0000F3C1  FECE              dec dh
0000F3C3  75DE              jnz 0xf3a3
0000F3C5  58                pop ax
0000F3C6  8AF0              mov dh,al
0000F3C8  8A4605            mov al,[bp+0x5]
0000F3CB  B500              mov ch,0x0
0000F3CD  8ACA              mov cl,dl
0000F3CF  57                push di
0000F3D0  F3AA              rep stosb
0000F3D2  5F                pop di
0000F3D3  81C70020          add di,0x2000
0000F3D7  8ACA              mov cl,dl
0000F3D9  57                push di
0000F3DA  F3AA              rep stosb
0000F3DC  5F                pop di
0000F3DD  2BFB              sub di,bx
0000F3DF  FECE              dec dh
0000F3E1  75EA              jnz 0xf3cd
0000F3E3  C3                ret
0000F3E4  E8FF02            call 0xf6e6
0000F3E7  7270              jc 0xf459
0000F3E9  8A5E05            mov bl,[bp+0x5]
0000F3EC  B700              mov bh,0x0
0000F3EE  53                push bx
0000F3EF  E84903            call 0xf73b
0000F3F2  8BF8              mov di,ax
0000F3F4  58                pop ax
0000F3F5  F7264C00          mul word [0x4c]
0000F3F9  03F8              add di,ax
0000F3FB  8BF7              mov si,di
0000F3FD  8B166300          mov dx,[0x63]
0000F401  83C206            add dx,byte +0x6
0000F404  1E                push ds
0000F405  8B5E00            mov bx,[bp+0x0]
0000F408  8EDB              mov ds,bx
0000F40A  8EC3              mov es,bx
0000F40C  8A4603            mov al,[bp+0x3]
0000F40F  3C08              cmp al,0x8
0000F411  7514              jnz 0xf427
0000F413  EC                in al,dx
0000F414  A801              test al,0x1
0000F416  75FB              jnz 0xf413
0000F418  FA                cli
0000F419  EC                in al,dx
0000F41A  A801              test al,0x1
0000F41C  74FB              jz 0xf419
0000F41E  AD                lodsw
0000F41F  1F                pop ds
0000F420  884602            mov [bp+0x2],al
0000F423  886603            mov [bp+0x3],ah
0000F426  C3                ret
0000F427  8A5E02            mov bl,[bp+0x2]
0000F42A  8A7E04            mov bh,[bp+0x4]
0000F42D  8B4E06            mov cx,[bp+0x6]
0000F430  3C0A              cmp al,0xa
0000F432  7412              jz 0xf446
0000F434  EC                in al,dx
0000F435  A801              test al,0x1
0000F437  75FB              jnz 0xf434
0000F439  FA                cli
0000F43A  EC                in al,dx
0000F43B  A801              test al,0x1
0000F43D  74FB              jz 0xf43a
0000F43F  8BC3              mov ax,bx
0000F441  AB                stosw
0000F442  E2F0              loop 0xf434
0000F444  1F                pop ds
0000F445  C3                ret
0000F446  EC                in al,dx
0000F447  A801              test al,0x1
0000F449  75FB              jnz 0xf446
0000F44B  FA                cli
0000F44C  EC                in al,dx
0000F44D  A801              test al,0x1
0000F44F  74FB              jz 0xf44c
0000F451  8AC3              mov al,bl
0000F453  AA                stosb
0000F454  47                inc di
0000F455  E2EF              loop 0xf446
0000F457  1F                pop ds
0000F458  C3                ret
0000F459  807E0308          cmp byte [bp+0x3],0x8
0000F45D  7503              jnz 0xf462
0000F45F  E9B800            jmp 0xf51a
0000F462  A15000            mov ax,[0x50]
0000F465  E8EB02            call 0xf753
0000F468  8BF8              mov di,ax
0000F46A  1E                push ds
0000F46B  8A4602            mov al,[bp+0x2]
0000F46E  B400              mov ah,0x0
0000F470  0AC0              or al,al
0000F472  7807              js 0xf47b
0000F474  8CCA              mov dx,cs
0000F476  BE6EFA            mov si,0xfa6e
0000F479  EB0C              jmp short 0xf487
0000F47B  247F              and al,0x7f
0000F47D  33DB              xor bx,bx
0000F47F  8EDB              mov ds,bx
0000F481  C5367C00          lds si,[0x7c]
0000F485  8CDA              mov dx,ds
0000F487  1F                pop ds
0000F488  B103              mov cl,0x3
0000F48A  D3E0              shl ax,cl
0000F48C  03F0              add si,ax
0000F48E  8B4600            mov ax,[bp+0x0]
0000F491  8EC0              mov es,ax
0000F493  8B4E06            mov cx,[bp+0x6]
0000F496  803E490006        cmp byte [0x49],0x6
0000F49B  1E                push ds
0000F49C  8EDA              mov ds,dx
0000F49E  7451              jz 0xf4f1
0000F4A0  D1E7              shl di,1
0000F4A2  8A4604            mov al,[bp+0x4]
0000F4A5  83E003            and ax,byte +0x3
0000F4A8  BB5555            mov bx,0x5555
0000F4AB  F7E3              mul bx
0000F4AD  8BD0              mov dx,ax
0000F4AF  8A5E04            mov bl,[bp+0x4]
0000F4B2  B708              mov bh,0x8
0000F4B4  57                push di
0000F4B5  56                push si
0000F4B6  AC                lodsb
0000F4B7  51                push cx
0000F4B8  53                push bx
0000F4B9  33DB              xor bx,bx
0000F4BB  B90800            mov cx,0x8
0000F4BE  D0E8              shr al,1
0000F4C0  D1DB              rcr bx,1
0000F4C2  D1FB              sar bx,1
0000F4C4  E2F8              loop 0xf4be
0000F4C6  8BC3              mov ax,bx
0000F4C8  5B                pop bx
0000F4C9  59                pop cx
0000F4CA  23C2              and ax,dx
0000F4CC  86C4              xchg al,ah
0000F4CE  0ADB              or bl,bl
0000F4D0  7903              jns 0xf4d5
0000F4D2  263305            xor ax,[es:di]
0000F4D5  268905            mov [es:di],ax
0000F4D8  81F70020          xor di,0x2000
0000F4DC  F7C70020          test di,0x2000
0000F4E0  7503              jnz 0xf4e5
0000F4E2  83C750            add di,byte +0x50
0000F4E5  FECF              dec bh
0000F4E7  75CD              jnz 0xf4b6
0000F4E9  5E                pop si
0000F4EA  5F                pop di
0000F4EB  47                inc di
0000F4EC  47                inc di
0000F4ED  E2C3              loop 0xf4b2
0000F4EF  1F                pop ds
0000F4F0  C3                ret
0000F4F1  8A5E04            mov bl,[bp+0x4]
0000F4F4  BA0020            mov dx,0x2000
0000F4F7  B708              mov bh,0x8
0000F4F9  57                push di
0000F4FA  56                push si
0000F4FB  AC                lodsb
0000F4FC  0ADB              or bl,bl
0000F4FE  7903              jns 0xf503
0000F500  263205            xor al,[es:di]
0000F503  268805            mov [es:di],al
0000F506  33FA              xor di,dx
0000F508  85FA              test dx,di
0000F50A  7503              jnz 0xf50f
0000F50C  83C750            add di,byte +0x50
0000F50F  FECF              dec bh
0000F511  75E8              jnz 0xf4fb
0000F513  5E                pop si
0000F514  5F                pop di
0000F515  47                inc di
0000F516  E2DF              loop 0xf4f7
0000F518  1F                pop ds
0000F519  C3                ret
0000F51A  FC                cld
0000F51B  A15000            mov ax,[0x50]
0000F51E  E83202            call 0xf753
0000F521  8BF0              mov si,ax
0000F523  83EC08            sub sp,byte +0x8
0000F526  8BFC              mov di,sp
0000F528  803E490006        cmp byte [0x49],0x6
0000F52D  8B4600            mov ax,[bp+0x0]
0000F530  1E                push ds
0000F531  57                push di
0000F532  8ED8              mov ds,ax
0000F534  7431              jz 0xf567
0000F536  B608              mov dh,0x8
0000F538  D1E6              shl si,1
0000F53A  BB0020            mov bx,0x2000
0000F53D  8B04              mov ax,[si]
0000F53F  86C4              xchg al,ah
0000F541  B900C0            mov cx,0xc000
0000F544  B200              mov dl,0x0
0000F546  85C1              test cx,ax
0000F548  F8                clc
0000F549  7401              jz 0xf54c
0000F54B  F9                stc
0000F54C  D0D2              rcl dl,1
0000F54E  D1E9              shr cx,1
0000F550  D1E9              shr cx,1
0000F552  73F2              jnc 0xf546
0000F554  368815            mov [ss:di],dl
0000F557  47                inc di
0000F558  33F3              xor si,bx
0000F55A  85F3              test bx,si
0000F55C  7503              jnz 0xf561
0000F55E  83C650            add si,byte +0x50
0000F561  FECE              dec dh
0000F563  75D8              jnz 0xf53d
0000F565  EB17              jmp short 0xf57e
0000F567  B604              mov dh,0x4
0000F569  8A24              mov ah,[si]
0000F56B  368825            mov [ss:di],ah
0000F56E  47                inc di
0000F56F  8AA40020          mov ah,[si+0x2000]
0000F573  368825            mov [ss:di],ah
0000F576  47                inc di
0000F577  83C650            add si,byte +0x50
0000F57A  FECE              dec dh
0000F57C  75EB              jnz 0xf569
0000F57E  8CCA              mov dx,cs
0000F580  BF6EFA            mov di,0xfa6e
0000F583  8EC2              mov es,dx
0000F585  8CD2              mov dx,ss
0000F587  8EDA              mov ds,dx
0000F589  5E                pop si
0000F58A  B000              mov al,0x0
0000F58C  BA8000            mov dx,0x80
0000F58F  56                push si
0000F590  57                push di
0000F591  B90800            mov cx,0x8
0000F594  F3A6              repe cmpsb
0000F596  5F                pop di
0000F597  5E                pop si
0000F598  741C              jz 0xf5b6
0000F59A  FEC0              inc al
0000F59C  83C708            add di,byte +0x8
0000F59F  4A                dec dx
0000F5A0  75ED              jnz 0xf58f
0000F5A2  0AC0              or al,al
0000F5A4  7410              jz 0xf5b6
0000F5A6  33DB              xor bx,bx
0000F5A8  8EDB              mov ds,bx
0000F5AA  C43E7C00          les di,[0x7c]
0000F5AE  8CC3              mov bx,es
0000F5B0  0BDF              or bx,di
0000F5B2  7402              jz 0xf5b6
0000F5B4  EBD6              jmp short 0xf58c
0000F5B6  884602            mov [bp+0x2],al
0000F5B9  1F                pop ds
0000F5BA  83C408            add sp,byte +0x8
0000F5BD  C3                ret
0000F5BE  8B166300          mov dx,[0x63]
0000F5C2  83C205            add dx,byte +0x5
0000F5C5  A06600            mov al,[0x66]
0000F5C8  8A6605            mov ah,[bp+0x5]
0000F5CB  0AE4              or ah,ah
0000F5CD  8A6604            mov ah,[bp+0x4]
0000F5D0  7509              jnz 0xf5db
0000F5D2  24E0              and al,0xe0
0000F5D4  80E41F            and ah,0x1f
0000F5D7  0AC4              or al,ah
0000F5D9  EB09              jmp short 0xf5e4
0000F5DB  24DF              and al,0xdf
0000F5DD  F6C401            test ah,0x1
0000F5E0  7402              jz 0xf5e4
0000F5E2  0C20              or al,0x20
0000F5E4  A26600            mov [0x66],al
0000F5E7  EE                out dx,al
0000F5E8  C3                ret
0000F5E9  8B4600            mov ax,[bp+0x0]
0000F5EC  8EC0              mov es,ax
0000F5EE  8B5608            mov dx,[bp+0x8]
0000F5F1  8B4E06            mov cx,[bp+0x6]
0000F5F4  E80101            call 0xf6f8
0000F5F7  750D              jnz 0xf606
0000F5F9  8A4602            mov al,[bp+0x2]
0000F5FC  8AD8              mov bl,al
0000F5FE  2401              and al,0x1
0000F600  D0C8              ror al,1
0000F602  B47F              mov ah,0x7f
0000F604  EB0F              jmp short 0xf615
0000F606  D0E1              shl cl,1
0000F608  8A4602            mov al,[bp+0x2]
0000F60B  8AD8              mov bl,al
0000F60D  2403              and al,0x3
0000F60F  D0C8              ror al,1
0000F611  D0C8              ror al,1
0000F613  B43F              mov ah,0x3f
0000F615  D2CC              ror ah,cl
0000F617  D2E8              shr al,cl
0000F619  268A0C            mov cl,[es:si]
0000F61C  0ADB              or bl,bl
0000F61E  7904              jns 0xf624
0000F620  32C8              xor cl,al
0000F622  EB04              jmp short 0xf628
0000F624  22CC              and cl,ah
0000F626  0AC8              or cl,al
0000F628  26880C            mov [es:si],cl
0000F62B  C3                ret
0000F62C  8B4600            mov ax,[bp+0x0]
0000F62F  8EC0              mov es,ax
0000F631  8B5608            mov dx,[bp+0x8]
0000F634  8B4E06            mov cx,[bp+0x6]
0000F637  E8BE00            call 0xf6f8
0000F63A  268A04            mov al,[es:si]
0000F63D  7508              jnz 0xf647
0000F63F  D2E0              shl al,cl
0000F641  D0C0              rol al,1
0000F643  2401              and al,0x1
0000F645  EB0A              jmp short 0xf651
0000F647  D0E1              shl cl,1
0000F649  D2E0              shl al,cl
0000F64B  D0C0              rol al,1
0000F64D  D0C0              rol al,1
0000F64F  2403              and al,0x3
0000F651  884602            mov [bp+0x2],al
0000F654  C3                ret
0000F655  8A1E6200          mov bl,[0x62]
0000F659  D0E3              shl bl,1
0000F65B  B700              mov bh,0x0
0000F65D  8B5750            mov dx,[bx+0x50]
0000F660  8A4602            mov al,[bp+0x2]
0000F663  3C08              cmp al,0x8
0000F665  7422              jz 0xf689
0000F667  3C0A              cmp al,0xa
0000F669  743D              jz 0xf6a8
0000F66B  3C07              cmp al,0x7
0000F66D  7423              jz 0xf692
0000F66F  3C0D              cmp al,0xd
0000F671  7425              jz 0xf698
0000F673  8A5E04            mov bl,[bp+0x4]
0000F676  B40A              mov ah,0xa
0000F678  B90100            mov cx,0x1
0000F67B  CD10              int 0x10
0000F67D  FEC2              inc dl
0000F67F  3A164A00          cmp dl,[0x4a]
0000F683  7515              jnz 0xf69a
0000F685  B200              mov dl,0x0
0000F687  EB1F              jmp short 0xf6a8
0000F689  80FA00            cmp dl,0x0
0000F68C  740C              jz 0xf69a
0000F68E  FECA              dec dl
0000F690  EB08              jmp short 0xf69a
0000F692  B301              mov bl,0x1
0000F694  E81801            call 0xf7af
0000F697  C3                ret
0000F698  B200              mov dl,0x0
0000F69A  8A1E6200          mov bl,[0x62]
0000F69E  D0E3              shl bl,1
0000F6A0  B700              mov bh,0x0
0000F6A2  895750            mov [bx+0x50],dx
0000F6A5  E9BE00            jmp 0xf766
0000F6A8  80FE18            cmp dh,0x18
0000F6AB  7404              jz 0xf6b1
0000F6AD  FEC6              inc dh
0000F6AF  75E9              jnz 0xf69a
0000F6B1  B402              mov ah,0x2
0000F6B3  CD10              int 0x10
0000F6B5  E82E00            call 0xf6e6
0000F6B8  B700              mov bh,0x0
0000F6BA  7206              jc 0xf6c2
0000F6BC  B408              mov ah,0x8
0000F6BE  CD10              int 0x10
0000F6C0  8AFC              mov bh,ah
0000F6C2  B406              mov ah,0x6
0000F6C4  B001              mov al,0x1
0000F6C6  33C9              xor cx,cx
0000F6C8  B618              mov dh,0x18
0000F6CA  8A164A00          mov dl,[0x4a]
0000F6CE  FECA              dec dl
0000F6D0  CD10              int 0x10
0000F6D2  C3                ret
0000F6D3  A04A00            mov al,[0x4a]
0000F6D6  884603            mov [bp+0x3],al
0000F6D9  A04900            mov al,[0x49]
0000F6DC  884602            mov [bp+0x2],al
0000F6DF  A06200            mov al,[0x62]
0000F6E2  884605            mov [bp+0x5],al
0000F6E5  C3                ret
0000F6E6  50                push ax
0000F6E7  A04900            mov al,[0x49]
0000F6EA  3C07              cmp al,0x7
0000F6EC  7408              jz 0xf6f6
0000F6EE  3C04              cmp al,0x4
0000F6F0  F5                cmc
0000F6F1  7303              jnc 0xf6f6
0000F6F3  18C0              sbb al,al
0000F6F5  F9                stc
0000F6F6  58                pop ax
0000F6F7  C3                ret
0000F6F8  B050              mov al,0x50
0000F6FA  33F6              xor si,si
0000F6FC  D0EA              shr dl,1
0000F6FE  7303              jnc 0xf703
0000F700  BE0020            mov si,0x2000
0000F703  F6E2              mul dl
0000F705  03F0              add si,ax
0000F707  8BD1              mov dx,cx
0000F709  B90203            mov cx,0x302
0000F70C  803E490006        cmp byte [0x49],0x6
0000F711  9C                pushf
0000F712  7503              jnz 0xf717
0000F714  B90307            mov cx,0x703
0000F717  22EA              and ch,dl
0000F719  D3EA              shr dx,cl
0000F71B  03F2              add si,dx
0000F71D  86E9              xchg ch,cl
0000F71F  9D                popf
0000F720  C3                ret
0000F721  E80A00            call 0xf72e
0000F724  8AE8              mov ch,al
0000F726  FEC4              inc ah
0000F728  E80300            call 0xf72e
0000F72B  8AC8              mov cl,al
0000F72D  C3                ret
0000F72E  52                push dx
0000F72F  8B166300          mov dx,[0x63]
0000F733  86E0              xchg ah,al
0000F735  EE                out dx,al
0000F736  FEC2              inc dl
0000F738  EC                in al,dx
0000F739  5A                pop dx
0000F73A  C3                ret
0000F73B  B700              mov bh,0x0
0000F73D  D1E3              shl bx,1
0000F73F  8B4750            mov ax,[bx+0x50]
0000F742  53                push bx
0000F743  8AD8              mov bl,al
0000F745  8AC4              mov al,ah
0000F747  F6264A00          mul byte [0x4a]
0000F74B  B700              mov bh,0x0
0000F74D  03C3              add ax,bx
0000F74F  D1E0              shl ax,1
0000F751  5B                pop bx
0000F752  C3                ret
0000F753  53                push bx
0000F754  8AD8              mov bl,al
0000F756  8AC4              mov al,ah
0000F758  F6264A00          mul byte [0x4a]
0000F75C  D1E0              shl ax,1
0000F75E  D1E0              shl ax,1
0000F760  B700              mov bh,0x0
0000F762  03C3              add ax,bx
0000F764  5B                pop bx
0000F765  C3                ret
0000F766  D0EB              shr bl,1
0000F768  381E6200          cmp [0x62],bl
0000F76C  7524              jnz 0xf792
0000F76E  E8CAFF            call 0xf73b
0000F771  03064E00          add ax,[0x4e]
0000F775  D1E8              shr ax,1
0000F777  8BC8              mov cx,ax
0000F779  B40E              mov ah,0xe
0000F77B  8AC5              mov al,ch
0000F77D  E80400            call 0xf784
0000F780  FEC4              inc ah
0000F782  8AC1              mov al,cl
0000F784  52                push dx
0000F785  8B166300          mov dx,[0x63]
0000F789  86E0              xchg ah,al
0000F78B  EE                out dx,al
0000F78C  86E0              xchg ah,al
0000F78E  FEC2              inc dl
0000F790  EE                out dx,al
0000F791  5A                pop dx
0000F792  C3                ret
0000F793  8A261000          mov ah,[0x10]
0000F797  80E430            and ah,0x30
0000F79A  B000              mov al,0x0
0000F79C  80FC30            cmp ah,0x30
0000F79F  7409              jz 0xf7aa
0000F7A1  B001              mov al,0x1
0000F7A3  80FC10            cmp ah,0x10
0000F7A6  7402              jz 0xf7aa
0000F7A8  B003              mov al,0x3
0000F7AA  B400              mov ah,0x0
0000F7AC  CD10              int 0x10
0000F7AE  C3                ret
0000F7AF  50                push ax
0000F7B0  51                push cx
0000F7B1  B0B6              mov al,0xb6
0000F7B3  E643              out 0x43,al
0000F7B5  B82805            mov ax,0x528
0000F7B8  E642              out 0x42,al
0000F7BA  8AC4              mov al,ah
0000F7BC  E642              out 0x42,al
0000F7BE  E461              in al,0x61
0000F7C0  50                push ax
0000F7C1  0C03              or al,0x3
0000F7C3  E661              out 0x61,al
0000F7C5  33C9              xor cx,cx
0000F7C7  E2FE              loop 0xf7c7
0000F7C9  FECB              dec bl
0000F7CB  75FA              jnz 0xf7c7
0000F7CD  58                pop ax
0000F7CE  E661              out 0x61,al
0000F7D0  59                pop cx
0000F7D1  58                pop ax
0000F7D2  C3                ret
0000F7D3  C3                ret
0000F7D4  204D6F            and [di+0x6f],cl
0000F7D7  6E                outsb
0000F7D8  6F                outsw
0000F7D9  2F                das
0000F7DA  48                dec ax
0000F7DB  657263            gs jc 0xf841
0000F7DE  756C              jnz 0xf84c
0000F7E0  657320            gs jnc 0xf803
0000F7E3  47                inc di
0000F7E4  7261              jc 0xf847
0000F7E6  7068              jo 0xf850
0000F7E8  69637300C3        imul sp,[bp+di+0x73],word 0xc300
0000F7ED  20436C            and [bp+di+0x6c],al
0000F7F0  6F                outsw
0000F7F1  636B00            arpl [bp+di+0x0],bp
0000F7F4  54                push sp
0000F7F5  657374            gs jnc 0xf86c
0000F7F8  696E672052        imul bp,[bp+0x67],word 0x5220
0000F7FD  41                inc cx
0000F7FE  4D                dec bp
0000F7FF  3A20              cmp ah,[bx+si]
0000F801  2020              and [bx+si],ah
0000F803  204B20            and [bp+di+0x20],cl
0000F806  4F                dec di
0000F807  4B                dec bx
0000F808  005379            add [bp+di+0x79],dl
0000F80B  7374              jnc 0xf881
0000F80D  656D              gs insw
0000F80F  204572            and [di+0x72],al
0000F812  726F              jc 0xf883
0000F814  723A              jc 0xf850
0000F816  2000              and [bx+si],al
0000F818  50                push ax
0000F819  61                popa
0000F81A  7269              jc 0xf885
0000F81C  7479              jz 0xf897
0000F81E  206572            and [di+0x72],ah
0000F821  726F              jc 0xf892
0000F823  7220              jc 0xf845
0000F825  61                popa
0000F826  743A              jz 0xf862
0000F828  203F              and [bx],bh
0000F82A  3F                aas
0000F82B  3F                aas
0000F82C  3F                aas
0000F82D  3F                aas
0000F82E  000D              add [di],cl
0000F830  0A436F            or al,[bp+di+0x6f]
0000F833  6E                outsb
0000F834  7469              jz 0xf89f
0000F836  6E                outsb
0000F837  7565              jnz 0xf89e
0000F839  3F                aas
0000F83A  2000              and [bx+si],al
0000F83C  90                nop
0000F83D  90                nop
0000F83E  90                nop
0000F83F  90                nop
0000F840  90                nop
0000F841  FB                sti
0000F842  1E                push ds
0000F843  33C0              xor ax,ax
0000F845  8ED8              mov ds,ax
0000F847  A11304            mov ax,[0x413]
0000F84A  1F                pop ds
0000F84B  CF                iret
0000F84C  90                nop
0000F84D  FB                sti
0000F84E  1E                push ds
0000F84F  33C0              xor ax,ax
0000F851  8ED8              mov ds,ax
0000F853  A11004            mov ax,[0x410]
0000F856  1F                pop ds
0000F857  CF                iret
0000F858  90                nop
0000F859  F9                stc
0000F85A  B486              mov ah,0x86
0000F85C  CA0200            retf 0x2
0000F85F  50                push ax
0000F860  E462              in al,0x62
0000F862  A8C0              test al,0xc0
0000F864  7503              jnz 0xf869
0000F866  E9A800            jmp 0xf911
0000F869  53                push bx
0000F86A  51                push cx
0000F86B  52                push dx
0000F86C  56                push si
0000F86D  57                push di
0000F86E  55                push bp
0000F86F  1E                push ds
0000F870  06                push es
0000F871  B84000            mov ax,0x40
0000F874  8ED8              mov ds,ax
0000F876  E81AFF            call 0xf793
0000F879  1E                push ds
0000F87A  0E                push cs
0000F87B  1F                pop ds
0000F87C  BE18F8            mov si,0xf818
0000F87F  E860EC            call 0xe4e2
0000F882  1F                pop ds
0000F883  B81100            mov ax,0x11
0000F886  E864EC            call 0xe4ed
0000F889  B000              mov al,0x0
0000F88B  E6A0              out 0xa0,al
0000F88D  BA6100            mov dx,0x61
0000F890  EC                in al,dx
0000F891  0C30              or al,0x30
0000F893  EE                out dx,al
0000F894  24CF              and al,0xcf
0000F896  EE                out dx,al
0000F897  B106              mov cl,0x6
0000F899  8B1E1300          mov bx,[0x13]
0000F89D  D3E3              shl bx,cl
0000F89F  42                inc dx
0000F8A0  33C0              xor ax,ax
0000F8A2  8ED8              mov ds,ax
0000F8A4  B91000            mov cx,0x10
0000F8A7  33F6              xor si,si
0000F8A9  8A24              mov ah,[si]
0000F8AB  EC                in al,dx
0000F8AC  A8C0              test al,0xc0
0000F8AE  750E              jnz 0xf8be
0000F8B0  46                inc si
0000F8B1  E2F6              loop 0xf8a9
0000F8B3  8CD8              mov ax,ds
0000F8B5  40                inc ax
0000F8B6  8ED8              mov ds,ax
0000F8B8  3BC3              cmp ax,bx
0000F8BA  75E8              jnz 0xf8a4
0000F8BC  EB0C              jmp short 0xf8ca
0000F8BE  8824              mov [si],ah
0000F8C0  8CD8              mov ax,ds
0000F8C2  E84DEC            call 0xe512
0000F8C5  8BC6              mov ax,si
0000F8C7  E818EE            call 0xe6e2
0000F8CA  B81600            mov ax,0x16
0000F8CD  E81DEC            call 0xe4ed
0000F8D0  1E                push ds
0000F8D1  0E                push cs
0000F8D2  1F                pop ds
0000F8D3  BE2FF8            mov si,0xf82f
0000F8D6  E809EC            call 0xe4e2
0000F8D9  1F                pop ds
0000F8DA  E421              in al,0x21
0000F8DC  50                push ax
0000F8DD  B0FC              mov al,0xfc
0000F8DF  E621              out 0x21,al
0000F8E1  FB                sti
0000F8E2  E80906            call 0xfeee
0000F8E5  50                push ax
0000F8E6  E8EEEB            call 0xe4d7
0000F8E9  58                pop ax
0000F8EA  3C59              cmp al,0x59
0000F8EC  7407              jz 0xf8f5
0000F8EE  3C79              cmp al,0x79
0000F8F0  7403              jz 0xf8f5
0000F8F2  E97CE9            jmp 0xe271
0000F8F5  E8D800            call 0xf9d0
0000F8F8  58                pop ax
0000F8F9  E621              out 0x21,al
0000F8FB  BA6100            mov dx,0x61
0000F8FE  EC                in al,dx
0000F8FF  0C30              or al,0x30
0000F901  EE                out dx,al
0000F902  24CF              and al,0xcf
0000F904  EE                out dx,al
0000F905  B080              mov al,0x80
0000F907  E6A0              out 0xa0,al
0000F909  07                pop es
0000F90A  1F                pop ds
0000F90B  5D                pop bp
0000F90C  5F                pop di
0000F90D  5E                pop si
0000F90E  5A                pop dx
0000F90F  59                pop cx
0000F910  5B                pop bx
0000F911  58                pop ax
0000F912  CF                iret
0000F913  4E                dec si
0000F914  6F                outsw
0000F915  204650            and [bp+0x50],al
0000F918  55                push bp
0000F919  2900              sub [bx+si],ax
0000F91B  32C0              xor al,al
0000F91D  B040              mov al,0x40
0000F91F  F6E0              mul al
0000F921  7404              jz 0xf927
0000F923  BE40EC            mov si,0xec40
0000F926  C3                ret
0000F927  BEDCFF            mov si,0xffdc
0000F92A  C3                ret
0000F92B  BB0002            mov bx,0x200
0000F92E  B85555            mov ax,0x5555
0000F931  33FF              xor di,di
0000F933  8BCB              mov cx,bx
0000F935  F3AB              rep stosw
0000F937  33FF              xor di,di
0000F939  8BCB              mov cx,bx
0000F93B  F3AF              repe scasw
0000F93D  E302              jcxz 0xf941
0000F93F  F9                stc
0000F940  C3                ret
0000F941  33FF              xor di,di
0000F943  8BCB              mov cx,bx
0000F945  F7D0              not ax
0000F947  F3AB              rep stosw
0000F949  33FF              xor di,di
0000F94B  8BCB              mov cx,bx
0000F94D  F3AF              repe scasw
0000F94F  E302              jcxz 0xf953
0000F951  F9                stc
0000F952  C3                ret
0000F953  33FF              xor di,di
0000F955  8BCB              mov cx,bx
0000F957  33C0              xor ax,ax
0000F959  F7D0              not ax
0000F95B  F3AB              rep stosw
0000F95D  33FF              xor di,di
0000F95F  8BCB              mov cx,bx
0000F961  F3AF              repe scasw
0000F963  E302              jcxz 0xf967
0000F965  F9                stc
0000F966  C3                ret
0000F967  33FF              xor di,di
0000F969  8BCB              mov cx,bx
0000F96B  33C0              xor ax,ax
0000F96D  F3AB              rep stosw
0000F96F  33FF              xor di,di
0000F971  8BCB              mov cx,bx
0000F973  F3AF              repe scasw
0000F975  E302              jcxz 0xf979
0000F977  F9                stc
0000F978  C3                ret
0000F979  8CC0              mov ax,es
0000F97B  83C040            add ax,byte +0x40
0000F97E  8EC0              mov es,ax
0000F980  C3                ret
0000F981  50                push ax
0000F982  53                push bx
0000F983  51                push cx
0000F984  E461              in al,0x61
0000F986  340C              xor al,0xc
0000F988  E661              out 0x61,al
0000F98A  BB890F            mov bx,0xf89
0000F98D  2404              and al,0x4
0000F98F  7403              jz 0xf994
0000F991  BB2E05            mov bx,0x52e
0000F994  B0B6              mov al,0xb6
0000F996  E643              out 0x43,al
0000F998  8BC3              mov ax,bx
0000F99A  E642              out 0x42,al
0000F99C  8AC4              mov al,ah
0000F99E  E642              out 0x42,al
0000F9A0  E461              in al,0x61
0000F9A2  50                push ax
0000F9A3  0C03              or al,0x3
0000F9A5  E661              out 0x61,al
0000F9A7  B90020            mov cx,0x2000
0000F9AA  E2FE              loop 0xf9aa
0000F9AC  58                pop ax
0000F9AD  E661              out 0x61,al
0000F9AF  59                pop cx
0000F9B0  5B                pop bx
0000F9B1  58                pop ax
0000F9B2  C3                ret
0000F9B3  FB                sti
0000F9B4  26031E6C04        add bx,[es:0x46c]
0000F9B9  B401              mov ah,0x1
0000F9BB  CD16              int 0x16
0000F9BD  750B              jnz 0xf9ca
0000F9BF  268B0E6C04        mov cx,[es:0x46c]
0000F9C4  2BCB              sub cx,bx
0000F9C6  72F1              jc 0xf9b9
0000F9C8  FA                cli
0000F9C9  C3                ret
0000F9CA  32E4              xor ah,ah
0000F9CC  CD16              int 0x16
0000F9CE  EBF8              jmp short 0xf9c8
0000F9D0  BA4F18            mov dx,0x184f
0000F9D3  33C9              xor cx,cx
0000F9D5  B80006            mov ax,0x600
0000F9D8  B707              mov bh,0x7
0000F9DA  CD10              int 0x10
0000F9DC  B402              mov ah,0x2
0000F9DE  33D2              xor dx,dx
0000F9E0  B700              mov bh,0x0
0000F9E2  CD10              int 0x10
0000F9E4  B80005            mov ax,0x500
0000F9E7  CD10              int 0x10
0000F9E9  C3                ret
0000F9EA  33D2              xor dx,dx
0000F9EC  B90100            mov cx,0x1
0000F9EF  BB7000            mov bx,0x70
0000F9F2  26803E490007      cmp byte [es:0x49],0x7
0000F9F8  7402              jz 0xf9fc
0000F9FA  B3E0              mov bl,0xe0
0000F9FC  AC                lodsb
0000F9FD  0AC0              or al,al
0000F9FF  7407              jz 0xfa08
0000FA01  FEC2              inc dl
0000FA03  E82400            call 0xfa2a
0000FA06  EBF4              jmp short 0xf9fc
0000FA08  B109              mov cl,0x9
0000FA0A  02D1              add dl,cl
0000FA0C  E81B00            call 0xfa2a
0000FA0F  80C104            add cl,0x4
0000FA12  B3E8              mov bl,0xe8
0000FA14  7902              jns 0xfa18
0000FA16  B301              mov bl,0x1
0000FA18  B0B2              mov al,0xb2
0000FA1A  02D1              add dl,cl
0000FA1C  E80B00            call 0xfa2a
0000FA1F  B0B1              mov al,0xb1
0000FA21  02D1              add dl,cl
0000FA23  FEC1              inc cl
0000FA25  E80200            call 0xfa2a
0000FA28  B0B0              mov al,0xb0
0000FA2A  B409              mov ah,0x9
0000FA2C  CD10              int 0x10
0000FA2E  B402              mov ah,0x2
0000FA30  CD10              int 0x10
0000FA32  C3                ret
0000FA33  90                nop
0000FA34  90                nop
0000FA35  90                nop
0000FA36  90                nop
0000FA37  90                nop
0000FA38  90                nop
0000FA39  90                nop
0000FA3A  90                nop
0000FA3B  90                nop
0000FA3C  90                nop
0000FA3D  90                nop
0000FA3E  90                nop
0000FA3F  90                nop
0000FA40  90                nop
0000FA41  90                nop
0000FA42  90                nop
0000FA43  90                nop
0000FA44  90                nop
0000FA45  90                nop
0000FA46  90                nop
0000FA47  90                nop
0000FA48  90                nop
0000FA49  90                nop
0000FA4A  90                nop
0000FA4B  90                nop
0000FA4C  90                nop
0000FA4D  90                nop
0000FA4E  90                nop
0000FA4F  90                nop
0000FA50  90                nop
0000FA51  90                nop
0000FA52  90                nop
0000FA53  90                nop
0000FA54  90                nop
0000FA55  90                nop
0000FA56  90                nop
0000FA57  90                nop
0000FA58  90                nop
0000FA59  90                nop
0000FA5A  90                nop
0000FA5B  90                nop
0000FA5C  90                nop
0000FA5D  90                nop
0000FA5E  90                nop
0000FA5F  90                nop
0000FA60  90                nop
0000FA61  90                nop
0000FA62  90                nop
0000FA63  90                nop
0000FA64  90                nop
0000FA65  90                nop
0000FA66  90                nop
0000FA67  90                nop
0000FA68  90                nop
0000FA69  90                nop
0000FA6A  90                nop
0000FA6B  90                nop
0000FA6C  90                nop
0000FA6D  90                nop
0000FA6E  0000              add [bx+si],al
0000FA70  0000              add [bx+si],al
0000FA72  0000              add [bx+si],al
0000FA74  0000              add [bx+si],al
0000FA76  7E81              jng 0xf9f9
0000FA78  A5                movsw
0000FA79  81BD99817E7E      cmp word [di-0x7e67],0x7e7e
0000FA7F  FF                db 0xff
0000FA80  DB                db 0xdb
0000FA81  FFC3              inc bx
0000FA83  E7FF              out 0xff,ax
0000FA85  7E6C              jng 0xfaf3
0000FA87  FE                db 0xfe
0000FA88  FE                db 0xfe
0000FA89  FE                db 0xfe
0000FA8A  7C38              jl 0xfac4
0000FA8C  1000              adc [bx+si],al
0000FA8E  1038              adc [bx+si],bh
0000FA90  7CFE              jl 0xfa90
0000FA92  7C38              jl 0xfacc
0000FA94  1000              adc [bx+si],al
0000FA96  387C38            cmp [si+0x38],bh
0000FA99  FE                db 0xfe
0000FA9A  FE                db 0xfe
0000FA9B  7C38              jl 0xfad5
0000FA9D  7C10              jl 0xfaaf
0000FA9F  1038              adc [bx+si],bh
0000FAA1  7CFE              jl 0xfaa1
0000FAA3  7C38              jl 0xfadd
0000FAA5  7C00              jl 0xfaa7
0000FAA7  0018              add [bx+si],bl
0000FAA9  3C3C              cmp al,0x3c
0000FAAB  1800              sbb [bx+si],al
0000FAAD  00FF              add bh,bh
0000FAAF  FFE7              jmp di
0000FAB1  C3                ret
0000FAB2  C3                ret
0000FAB3  E7FF              out 0xff,ax
0000FAB5  FF00              inc word [bx+si]
0000FAB7  3C66              cmp al,0x66
0000FAB9  42                inc dx
0000FABA  42                inc dx
0000FABB  663C00            o32 cmp al,0x0
0000FABE  FFC3              inc bx
0000FAC0  99                cwd
0000FAC1  BDBD99            mov bp,0x99bd
0000FAC4  C3                ret
0000FAC5  FF0F              dec word [bx]
0000FAC7  07                pop es
0000FAC8  0F                db 0x0f
0000FAC9  7DCC              jnl 0xfa97
0000FACB  CC                int3
0000FACC  CC                int3
0000FACD  783C              js 0xfb0b
0000FACF  6666663C18        o32 cmp al,0x18
0000FAD4  7E18              jng 0xfaee
0000FAD6  3F                aas
0000FAD7  333F              xor di,[bx]
0000FAD9  3030              xor [bx+si],dh
0000FADB  70F0              jo 0xfacd
0000FADD  E07F              loopne 0xfb5e
0000FADF  637F63            arpl [bx+0x63],di
0000FAE2  6367E6            arpl [bx-0x1a],sp
0000FAE5  C0995A3CE7        rcr byte [bx+di+0x3c5a],byte 0xe7
0000FAEA  E73C              out 0x3c,ax
0000FAEC  5A                pop dx
0000FAED  99                cwd
0000FAEE  80E0F8            and al,0xf8
0000FAF1  FE                db 0xfe
0000FAF2  F8                clc
0000FAF3  E080              loopne 0xfa75
0000FAF5  0002              add [bp+si],al
0000FAF7  0E                push cs
0000FAF8  3E                ds
0000FAF9  FE                db 0xfe
0000FAFA  3E0E              ds push cs
0000FAFC  0200              add al,[bx+si]
0000FAFE  183C              sbb [si],bh
0000FB00  7E18              jng 0xfb1a
0000FB02  187E3C            sbb [bp+0x3c],bh
0000FB05  186666            sbb [bp+0x66],ah
0000FB08  666666006600      o32 add [bp+0x0],ah
0000FB0E  7FDB              jg 0xfaeb
0000FB10  DB7B1B            fstp tword [bp+di+0x1b]
0000FB13  1B1B              sbb bx,[bp+di]
0000FB15  003E6338          add [0x3863],bh
0000FB19  6C                insb
0000FB1A  6C                insb
0000FB1B  38CC              cmp ah,cl
0000FB1D  7800              js 0xfb1f
0000FB1F  0000              add [bx+si],al
0000FB21  007E7E            add [bp+0x7e],bh
0000FB24  7E00              jng 0xfb26
0000FB26  183C              sbb [si],bh
0000FB28  7E18              jng 0xfb42
0000FB2A  7E3C              jng 0xfb68
0000FB2C  18FF              sbb bh,bh
0000FB2E  183C              sbb [si],bh
0000FB30  7E18              jng 0xfb4a
0000FB32  1818              sbb [bx+si],bl
0000FB34  1800              sbb [bx+si],al
0000FB36  1818              sbb [bx+si],bl
0000FB38  1818              sbb [bx+si],bl
0000FB3A  7E3C              jng 0xfb78
0000FB3C  1800              sbb [bx+si],al
0000FB3E  0018              add [bx+si],bl
0000FB40  0CFE              or al,0xfe
0000FB42  0C18              or al,0x18
0000FB44  0000              add [bx+si],al
0000FB46  0030              add [bx+si],dh
0000FB48  60                pusha
0000FB49  FE                db 0xfe
0000FB4A  60                pusha
0000FB4B  3000              xor [bx+si],al
0000FB4D  0000              add [bx+si],al
0000FB4F  00C0              add al,al
0000FB51  C0C0FE            rol al,byte 0xfe
0000FB54  0000              add [bx+si],al
0000FB56  0024              add [si],ah
0000FB58  66FF6624          jmp dword [bp+0x24]
0000FB5C  0000              add [bx+si],al
0000FB5E  0018              add [bx+si],bl
0000FB60  3C7E              cmp al,0x7e
0000FB62  FF                db 0xff
0000FB63  FF00              inc word [bx+si]
0000FB65  0000              add [bx+si],al
0000FB67  FF                db 0xff
0000FB68  FF                db 0xff
0000FB69  7E3C              jng 0xfba7
0000FB6B  1800              sbb [bx+si],al
0000FB6D  0000              add [bx+si],al
0000FB6F  0000              add [bx+si],al
0000FB71  0000              add [bx+si],al
0000FB73  0000              add [bx+si],al
0000FB75  0030              add [bx+si],dh
0000FB77  7878              js 0xfbf1
0000FB79  3030              xor [bx+si],dh
0000FB7B  0030              add [bx+si],dh
0000FB7D  006C6C            add [si+0x6c],ch
0000FB80  6C                insb
0000FB81  0000              add [bx+si],al
0000FB83  0000              add [bx+si],al
0000FB85  006C6C            add [si+0x6c],ch
0000FB88  FE                db 0xfe
0000FB89  6C                insb
0000FB8A  FE                db 0xfe
0000FB8B  6C                insb
0000FB8C  6C                insb
0000FB8D  0030              add [bx+si],dh
0000FB8F  7CC0              jl 0xfb51
0000FB91  780C              js 0xfb9f
0000FB93  F8                clc
0000FB94  3000              xor [bx+si],al
0000FB96  00C6              add dh,al
0000FB98  CC                int3
0000FB99  1830              sbb [bx+si],dh
0000FB9B  66C60038          o32 mov byte [bx+si],0x38
0000FB9F  6C                insb
0000FBA0  3876DC            cmp [bp-0x24],dh
0000FBA3  CC                int3
0000FBA4  7600              jna 0xfba6
0000FBA6  60                pusha
0000FBA7  60                pusha
0000FBA8  C00000            rol byte [bx+si],byte 0x0
0000FBAB  0000              add [bx+si],al
0000FBAD  0018              add [bx+si],bl
0000FBAF  306060            xor [bx+si+0x60],ah
0000FBB2  60                pusha
0000FBB3  3018              xor [bx+si],bl
0000FBB5  006030            add [bx+si+0x30],ah
0000FBB8  1818              sbb [bx+si],bl
0000FBBA  1830              sbb [bx+si],dh
0000FBBC  60                pusha
0000FBBD  0000              add [bx+si],al
0000FBBF  663CFF            o32 cmp al,0xff
0000FBC2  3C66              cmp al,0x66
0000FBC4  0000              add [bx+si],al
0000FBC6  0030              add [bx+si],dh
0000FBC8  30FC              xor ah,bh
0000FBCA  3030              xor [bx+si],dh
0000FBCC  0000              add [bx+si],al
0000FBCE  0000              add [bx+si],al
0000FBD0  0000              add [bx+si],al
0000FBD2  0030              add [bx+si],dh
0000FBD4  306000            xor [bx+si+0x0],ah
0000FBD7  0000              add [bx+si],al
0000FBD9  FC                cld
0000FBDA  0000              add [bx+si],al
0000FBDC  0000              add [bx+si],al
0000FBDE  0000              add [bx+si],al
0000FBE0  0000              add [bx+si],al
0000FBE2  0030              add [bx+si],dh
0000FBE4  3000              xor [bx+si],al
0000FBE6  06                push es
0000FBE7  0C18              or al,0x18
0000FBE9  3060C0            xor [bx+si-0x40],ah
0000FBEC  80007C            add byte [bx+si],0x7c
0000FBEF  C6                db 0xc6
0000FBF0  CE                into
0000FBF1  DEF6              fdivrp st6
0000FBF3  E67C              out 0x7c,al
0000FBF5  0030              add [bx+si],dh
0000FBF7  7030              jo 0xfc29
0000FBF9  3030              xor [bx+si],dh
0000FBFB  30FC              xor ah,bh
0000FBFD  0078CC            add [bx+si-0x34],bh
0000FC00  0C38              or al,0x38
0000FC02  60                pusha
0000FC03  CC                int3
0000FC04  FC                cld
0000FC05  0078CC            add [bx+si-0x34],bh
0000FC08  0C38              or al,0x38
0000FC0A  0CCC              or al,0xcc
0000FC0C  7800              js 0xfc0e
0000FC0E  1C3C              sbb al,0x3c
0000FC10  6C                insb
0000FC11  CC                int3
0000FC12  FE0C              dec byte [si]
0000FC14  1E                push ds
0000FC15  00FC              add ah,bh
0000FC17  C0F80C            sar al,byte 0xc
0000FC1A  0CCC              or al,0xcc
0000FC1C  7800              js 0xfc1e
0000FC1E  3860C0            cmp [bx+si-0x40],ah
0000FC21  F8                clc
0000FC22  CC                int3
0000FC23  CC                int3
0000FC24  7800              js 0xfc26
0000FC26  FC                cld
0000FC27  CC                int3
0000FC28  0C18              or al,0x18
0000FC2A  3030              xor [bx+si],dh
0000FC2C  3000              xor [bx+si],al
0000FC2E  78CC              js 0xfbfc
0000FC30  CC                int3
0000FC31  78CC              js 0xfbff
0000FC33  CC                int3
0000FC34  7800              js 0xfc36
0000FC36  78CC              js 0xfc04
0000FC38  CC                int3
0000FC39  7C0C              jl 0xfc47
0000FC3B  187000            sbb [bx+si+0x0],dh
0000FC3E  0030              add [bx+si],dh
0000FC40  3000              xor [bx+si],al
0000FC42  0030              add [bx+si],dh
0000FC44  3000              xor [bx+si],al
0000FC46  0030              add [bx+si],dh
0000FC48  3000              xor [bx+si],al
0000FC4A  0030              add [bx+si],dh
0000FC4C  306018            xor [bx+si+0x18],ah
0000FC4F  3060C0            xor [bx+si-0x40],ah
0000FC52  60                pusha
0000FC53  3018              xor [bx+si],bl
0000FC55  0000              add [bx+si],al
0000FC57  00FC              add ah,bh
0000FC59  0000              add [bx+si],al
0000FC5B  FC                cld
0000FC5C  0000              add [bx+si],al
0000FC5E  60                pusha
0000FC5F  3018              xor [bx+si],bl
0000FC61  0C18              or al,0x18
0000FC63  306000            xor [bx+si+0x0],ah
0000FC66  78CC              js 0xfc34
0000FC68  0C18              or al,0x18
0000FC6A  3000              xor [bx+si],al
0000FC6C  3000              xor [bx+si],al
0000FC6E  7CC6              jl 0xfc36
0000FC70  DE                db 0xde
0000FC71  DE                db 0xde
0000FC72  DEC0              faddp st0
0000FC74  7800              js 0xfc76
0000FC76  3078CC            xor [bx+si-0x34],bh
0000FC79  CC                int3
0000FC7A  FC                cld
0000FC7B  CC                int3
0000FC7C  CC                int3
0000FC7D  00FC              add ah,bh
0000FC7F  66667C66          o32 jl 0xfce9
0000FC83  66FC              o32 cld
0000FC85  003C              add [si],bh
0000FC87  66C0C0C0          o32 rol al,byte 0xc0
0000FC8B  663C00            o32 cmp al,0x0
0000FC8E  F8                clc
0000FC8F  6C                insb
0000FC90  6666666C          o32 insb
0000FC94  F8                clc
0000FC95  00FE              add dh,bh
0000FC97  626878            bound bp,[bx+si+0x78]
0000FC9A  6862FE            push word 0xfe62
0000FC9D  00FE              add dh,bh
0000FC9F  626878            bound bp,[bx+si+0x78]
0000FCA2  6860F0            push word 0xf060
0000FCA5  003C              add [si],bh
0000FCA7  66C0C0CE          o32 rol al,byte 0xce
0000FCAB  663E00CC          ds o32 add ah,cl
0000FCAF  CC                int3
0000FCB0  CC                int3
0000FCB1  FC                cld
0000FCB2  CC                int3
0000FCB3  CC                int3
0000FCB4  CC                int3
0000FCB5  007830            add [bx+si+0x30],bh
0000FCB8  3030              xor [bx+si],dh
0000FCBA  3030              xor [bx+si],dh
0000FCBC  7800              js 0xfcbe
0000FCBE  1E                push ds
0000FCBF  0C0C              or al,0xc
0000FCC1  0CCC              or al,0xcc
0000FCC3  CC                int3
0000FCC4  7800              js 0xfcc6
0000FCC6  E666              out 0x66,al
0000FCC8  6C                insb
0000FCC9  786C              js 0xfd37
0000FCCB  66E600            o32 out 0x0,al
0000FCCE  F060              lock pusha
0000FCD0  60                pusha
0000FCD1  60                pusha
0000FCD2  6266FE            bound sp,[bp-0x2]
0000FCD5  00C6              add dh,al
0000FCD7  EE                out dx,al
0000FCD8  FE                db 0xfe
0000FCD9  FE                db 0xfe
0000FCDA  D6                salc
0000FCDB  C6C600            mov dh,0x0
0000FCDE  C6                db 0xc6
0000FCDF  E6F6              out 0xf6,al
0000FCE1  DECE              fmulp st6
0000FCE3  C6C600            mov dh,0x0
0000FCE6  386CC6            cmp [si-0x3a],ch
0000FCE9  C6C66C            mov dh,0x6c
0000FCEC  3800              cmp [bx+si],al
0000FCEE  FC                cld
0000FCEF  66667C60          o32 jl 0xfd53
0000FCF3  60                pusha
0000FCF4  F00078CC          lock add [bx+si-0x34],bh
0000FCF8  CC                int3
0000FCF9  CC                int3
0000FCFA  DC781C            fdivr qword [bx+si+0x1c]
0000FCFD  00FC              add ah,bh
0000FCFF  66667C6C          o32 jl 0xfd6f
0000FD03  66E600            o32 out 0x0,al
0000FD06  78CC              js 0xfcd4
0000FD08  E070              loopne 0xfd7a
0000FD0A  1CCC              sbb al,0xcc
0000FD0C  7800              js 0xfd0e
0000FD0E  FC                cld
0000FD0F  B430              mov ah,0x30
0000FD11  3030              xor [bx+si],dh
0000FD13  307800            xor [bx+si+0x0],bh
0000FD16  CC                int3
0000FD17  CC                int3
0000FD18  CC                int3
0000FD19  CC                int3
0000FD1A  CC                int3
0000FD1B  CC                int3
0000FD1C  FC                cld
0000FD1D  00CC              add ah,cl
0000FD1F  CC                int3
0000FD20  CC                int3
0000FD21  CC                int3
0000FD22  CC                int3
0000FD23  7830              js 0xfd55
0000FD25  00C6              add dh,al
0000FD27  C6C6D6            mov dh,0xd6
0000FD2A  FE                db 0xfe
0000FD2B  EE                out dx,al
0000FD2C  C600C6            mov byte [bx+si],0xc6
0000FD2F  C6                db 0xc6
0000FD30  6C                insb
0000FD31  3838              cmp [bx+si],bh
0000FD33  6C                insb
0000FD34  C600CC            mov byte [bx+si],0xcc
0000FD37  CC                int3
0000FD38  CC                int3
0000FD39  7830              js 0xfd6b
0000FD3B  307800            xor [bx+si+0x0],bh
0000FD3E  FEC6              inc dh
0000FD40  8C18              mov [bx+si],ds
0000FD42  3266FE            xor ah,[bp-0x2]
0000FD45  007860            add [bx+si+0x60],bh
0000FD48  60                pusha
0000FD49  60                pusha
0000FD4A  60                pusha
0000FD4B  60                pusha
0000FD4C  7800              js 0xfd4e
0000FD4E  C0603018          shl byte [bx+si+0x30],byte 0x18
0000FD52  0C06              or al,0x6
0000FD54  0200              add al,[bx+si]
0000FD56  7818              js 0xfd70
0000FD58  1818              sbb [bx+si],bl
0000FD5A  1818              sbb [bx+si],bl
0000FD5C  7800              js 0xfd5e
0000FD5E  1038              adc [bx+si],bh
0000FD60  6C                insb
0000FD61  C60000            mov byte [bx+si],0x0
0000FD64  0000              add [bx+si],al
0000FD66  0000              add [bx+si],al
0000FD68  0000              add [bx+si],al
0000FD6A  0000              add [bx+si],al
0000FD6C  00FF              add bh,bh
0000FD6E  3030              xor [bx+si],dh
0000FD70  1800              sbb [bx+si],al
0000FD72  0000              add [bx+si],al
0000FD74  0000              add [bx+si],al
0000FD76  0000              add [bx+si],al
0000FD78  780C              js 0xfd86
0000FD7A  7CCC              jl 0xfd48
0000FD7C  7600              jna 0xfd7e
0000FD7E  E060              loopne 0xfde0
0000FD80  60                pusha
0000FD81  7C66              jl 0xfde9
0000FD83  66DC00            o32 fadd qword [bx+si]
0000FD86  0000              add [bx+si],al
0000FD88  78CC              js 0xfd56
0000FD8A  C0CC78            ror ah,byte 0x78
0000FD8D  001C              add [si],bl
0000FD8F  0C0C              or al,0xc
0000FD91  7CCC              jl 0xfd5f
0000FD93  CC                int3
0000FD94  7600              jna 0xfd96
0000FD96  0000              add [bx+si],al
0000FD98  78CC              js 0xfd66
0000FD9A  FC                cld
0000FD9B  C0780038          sar byte [bx+si+0x0],byte 0x38
0000FD9F  6C                insb
0000FDA0  60                pusha
0000FDA1  F060              lock pusha
0000FDA3  60                pusha
0000FDA4  F00000            lock add [bx+si],al
0000FDA7  0076CC            add [bp-0x34],dh
0000FDAA  CC                int3
0000FDAB  7C0C              jl 0xfdb9
0000FDAD  F8                clc
0000FDAE  E060              loopne 0xfe10
0000FDB0  6C                insb
0000FDB1  7666              jna 0xfe19
0000FDB3  66E600            o32 out 0x0,al
0000FDB6  3000              xor [bx+si],al
0000FDB8  7030              jo 0xfdea
0000FDBA  3030              xor [bx+si],dh
0000FDBC  7800              js 0xfdbe
0000FDBE  0C00              or al,0x0
0000FDC0  0C0C              or al,0xc
0000FDC2  0CCC              or al,0xcc
0000FDC4  CC                int3
0000FDC5  78E0              js 0xfda7
0000FDC7  60                pusha
0000FDC8  666C              o32 insb
0000FDCA  786C              js 0xfe38
0000FDCC  E600              out 0x0,al
0000FDCE  7030              jo 0xfe00
0000FDD0  3030              xor [bx+si],dh
0000FDD2  3030              xor [bx+si],dh
0000FDD4  7800              js 0xfdd6
0000FDD6  0000              add [bx+si],al
0000FDD8  CC                int3
0000FDD9  FE                db 0xfe
0000FDDA  FE                db 0xfe
0000FDDB  D6                salc
0000FDDC  C60000            mov byte [bx+si],0x0
0000FDDF  00F8              add al,bh
0000FDE1  CC                int3
0000FDE2  CC                int3
0000FDE3  CC                int3
0000FDE4  CC                int3
0000FDE5  0000              add [bx+si],al
0000FDE7  0078CC            add [bx+si-0x34],bh
0000FDEA  CC                int3
0000FDEB  CC                int3
0000FDEC  7800              js 0xfdee
0000FDEE  0000              add [bx+si],al
0000FDF0  DC6666            fsub qword [bp+0x66]
0000FDF3  7C60              jl 0xfe55
0000FDF5  F00000            lock add [bx+si],al
0000FDF8  76CC              jna 0xfdc6
0000FDFA  CC                int3
0000FDFB  7C0C              jl 0xfe09
0000FDFD  1E                push ds
0000FDFE  0000              add [bx+si],al
0000FE00  DC7666            fdiv qword [bp+0x66]
0000FE03  60                pusha
0000FE04  F00000            lock add [bx+si],al
0000FE07  007CC0            add [si-0x40],bh
0000FE0A  780C              js 0xfe18
0000FE0C  F8                clc
0000FE0D  0010              add [bx+si],dl
0000FE0F  307C30            xor [si+0x30],bh
0000FE12  3034              xor [si],dh
0000FE14  1800              sbb [bx+si],al
0000FE16  0000              add [bx+si],al
0000FE18  CC                int3
0000FE19  CC                int3
0000FE1A  CC                int3
0000FE1B  CC                int3
0000FE1C  7600              jna 0xfe1e
0000FE1E  0000              add [bx+si],al
0000FE20  CC                int3
0000FE21  CC                int3
0000FE22  CC                int3
0000FE23  7830              js 0xfe55
0000FE25  0000              add [bx+si],al
0000FE27  00C6              add dh,al
0000FE29  D6                salc
0000FE2A  FE                db 0xfe
0000FE2B  FE                db 0xfe
0000FE2C  6C                insb
0000FE2D  0000              add [bx+si],al
0000FE2F  00C6              add dh,al
0000FE31  6C                insb
0000FE32  386CC6            cmp [si-0x3a],ch
0000FE35  0000              add [bx+si],al
0000FE37  00CC              add ah,cl
0000FE39  CC                int3
0000FE3A  CC                int3
0000FE3B  7C0C              jl 0xfe49
0000FE3D  F8                clc
0000FE3E  0000              add [bx+si],al
0000FE40  FC                cld
0000FE41  98                cbw
0000FE42  3064FC            xor [si-0x4],ah
0000FE45  001C              add [si],bl
0000FE47  3030              xor [bx+si],dh
0000FE49  E030              loopne 0xfe7b
0000FE4B  301C              xor [si],bl
0000FE4D  0018              add [bx+si],bl
0000FE4F  1818              sbb [bx+si],bl
0000FE51  0018              add [bx+si],bl
0000FE53  1818              sbb [bx+si],bl
0000FE55  00E0              add al,ah
0000FE57  3030              xor [bx+si],dh
0000FE59  1C30              sbb al,0x30
0000FE5B  30E0              xor al,ah
0000FE5D  0076DC            add [bp-0x24],dh
0000FE60  0000              add [bx+si],al
0000FE62  0000              add [bx+si],al
0000FE64  0000              add [bx+si],al
0000FE66  0010              add [bx+si],dl
0000FE68  386CC6            cmp [si-0x3a],ch
0000FE6B  C6                db 0xc6
0000FE6C  FE00              inc byte [bx+si]
0000FE6E  FB                sti
0000FE6F  1E                push ds
0000FE70  50                push ax
0000FE71  33C0              xor ax,ax
0000FE73  8ED8              mov ds,ax
0000FE75  58                pop ax
0000FE76  FA                cli
0000FE77  0AE4              or ah,ah
0000FE79  7412              jz 0xfe8d
0000FE7B  FECC              dec ah
0000FE7D  7519              jnz 0xfe98
0000FE7F  89166C04          mov [0x46c],dx
0000FE83  890E6E04          mov [0x46e],cx
0000FE87  88267004          mov [0x470],ah
0000FE8B  EB0B              jmp short 0xfe98
0000FE8D  8B0E6E04          mov cx,[0x46e]
0000FE91  8B166C04          mov dx,[0x46c]
0000FE95  E80300            call 0xfe9b
0000FE98  FB                sti
0000FE99  1F                pop ds
0000FE9A  CF                iret
0000FE9B  A07004            mov al,[0x470]
0000FE9E  30067004          xor [0x470],al
0000FEA2  C3                ret
0000FEA3  6800FB            push word 0xfb00
0000FEA6  1E                push ds
0000FEA7  52                push dx
0000FEA8  50                push ax
0000FEA9  33C0              xor ax,ax
0000FEAB  8ED8              mov ds,ax
0000FEAD  FE0E4004          dec byte [0x440]
0000FEB1  750B              jnz 0xfebe
0000FEB3  80263F04F0        and byte [0x43f],0xf0
0000FEB8  B00C              mov al,0xc
0000FEBA  BAF203            mov dx,0x3f2
0000FEBD  EE                out dx,al
0000FEBE  FF066C04          inc word [0x46c]
0000FEC2  7504              jnz 0xfec8
0000FEC4  FF066E04          inc word [0x46e]
0000FEC8  833E6E0418        cmp word [0x46e],byte +0x18
0000FECD  7515              jnz 0xfee4
0000FECF  813E6C04B000      cmp word [0x46c],0xb0
0000FED5  750D              jnz 0xfee4
0000FED7  33C0              xor ax,ax
0000FED9  A36E04            mov [0x46e],ax
0000FEDC  A36C04            mov [0x46c],ax
0000FEDF  C606700401        mov byte [0x470],0x1
0000FEE4  CD1C              int 0x1c
0000FEE6  B020              mov al,0x20
0000FEE8  E620              out 0x20,al
0000FEEA  58                pop ax
0000FEEB  5A                pop dx
0000FEEC  1F                pop ds
0000FEED  CF                iret
0000FEEE  B400              mov ah,0x0
0000FEF0  CD16              int 0x16
0000FEF2  C3                ret
0000FEF3  A5                movsw
0000FEF4  FE87E923          inc byte [bx+0x23e9]
0000FEF8  FF23              jmp [bp+di]
0000FEFA  FF23              jmp [bp+di]
0000FEFC  FF23              jmp [bp+di]
0000FEFE  FF57EF            call [bx-0x11]
0000FF01  23FF              and di,di
0000FF03  65F04D            gs lock dec bp
0000FF06  F8                clc
0000FF07  41                inc cx
0000FF08  F8                clc
0000FF09  59                pop cx
0000FF0A  EC                in al,dx
0000FF0B  39E7              cmp di,sp
0000FF0D  59                pop cx
0000FF0E  F8                clc
0000FF0F  2EE8D2EF          cs call 0xeee5
0000FF13  23FF              and di,di
0000FF15  F2E66E            repne out 0x6e,al
0000FF18  FE                db 0xfe
0000FF19  53                push bx
0000FF1A  FF53FF            call [bp+di-0x1]
0000FF1D  A4                movsb
0000FF1E  F0                lock
0000FF1F  C7                db 0xc7
0000FF20  EF                out dx,ax
0000FF21  0000              add [bx+si],al
0000FF23  1E                push ds
0000FF24  52                push dx
0000FF25  50                push ax
0000FF26  33C0              xor ax,ax
0000FF28  8ED8              mov ds,ax
0000FF2A  B00B              mov al,0xb
0000FF2C  E620              out 0x20,al
0000FF2E  90                nop
0000FF2F  E420              in al,0x20
0000FF31  8AE0              mov ah,al
0000FF33  0AC0              or al,al
0000FF35  7504              jnz 0xff3b
0000FF37  B0FF              mov al,0xff
0000FF39  EB0A              jmp short 0xff45
0000FF3B  E421              in al,0x21
0000FF3D  0AC4              or al,ah
0000FF3F  E621              out 0x21,al
0000FF41  B020              mov al,0x20
0000FF43  E620              out 0x20,al
0000FF45  88266B04          mov [0x46b],ah
0000FF49  58                pop ax
0000FF4A  5A                pop dx
0000FF4B  1F                pop ds
0000FF4C  CF                iret
0000FF4D  BC0378            mov sp,0x7803
0000FF50  037802            add di,[bx+si+0x2]
0000FF53  CF                iret
0000FF54  FB                sti
0000FF55  1E                push ds
0000FF56  50                push ax
0000FF57  53                push bx
0000FF58  51                push cx
0000FF59  52                push dx
0000FF5A  33C0              xor ax,ax
0000FF5C  8ED8              mov ds,ax
0000FF5E  803E000501        cmp byte [0x500],0x1
0000FF63  7456              jz 0xffbb
0000FF65  C606000501        mov byte [0x500],0x1
0000FF6A  E85E00            call 0xffcb
0000FF6D  B40F              mov ah,0xf
0000FF6F  CD10              int 0x10
0000FF71  50                push ax
0000FF72  B403              mov ah,0x3
0000FF74  CD10              int 0x10
0000FF76  58                pop ax
0000FF77  52                push dx
0000FF78  B519              mov ch,0x19
0000FF7A  8ACC              mov cl,ah
0000FF7C  33D2              xor dx,dx
0000FF7E  B402              mov ah,0x2
0000FF80  CD10              int 0x10
0000FF82  B408              mov ah,0x8
0000FF84  CD10              int 0x10
0000FF86  0AC0              or al,al
0000FF88  7502              jnz 0xff8c
0000FF8A  B020              mov al,0x20
0000FF8C  52                push dx
0000FF8D  33D2              xor dx,dx
0000FF8F  8AE2              mov ah,dl
0000FF91  CD17              int 0x17
0000FF93  5A                pop dx
0000FF94  F6C425            test ah,0x25
0000FF97  7407              jz 0xffa0
0000FF99  C6060005FF        mov byte [0x500],0xff
0000FF9E  EB16              jmp short 0xffb6
0000FFA0  FEC2              inc dl
0000FFA2  3ACA              cmp cl,dl
0000FFA4  75D8              jnz 0xff7e
0000FFA6  B200              mov dl,0x0
0000FFA8  E82000            call 0xffcb
0000FFAB  FEC6              inc dh
0000FFAD  3AF5              cmp dh,ch
0000FFAF  75CD              jnz 0xff7e
0000FFB1  C606000500        mov byte [0x500],0x0
0000FFB6  5A                pop dx
0000FFB7  B402              mov ah,0x2
0000FFB9  CD10              int 0x10
0000FFBB  5A                pop dx
0000FFBC  59                pop cx
0000FFBD  5B                pop bx
0000FFBE  58                pop ax
0000FFBF  1F                pop ds
0000FFC0  CF                iret
0000FFC1  53                push bx
0000FFC2  7973              jns 0x37
0000FFC4  7465              jz 0x2b
0000FFC6  6D                insw
0000FFC7  20C2              and dl,al
0000FFC9  2000              and [bx+si],al
0000FFCB  52                push dx
0000FFCC  33D2              xor dx,dx
0000FFCE  8AE2              mov ah,dl
0000FFD0  B00A              mov al,0xa
0000FFD2  CD17              int 0x17
0000FFD4  B400              mov ah,0x0
0000FFD6  B00D              mov al,0xd
0000FFD8  CD17              int 0x17
0000FFDA  5A                pop dx
0000FFDB  C3                ret
0000FFDC  3238              xor bh,[bx+si]
0000FFDE  36204350          and [ss:bp+di+0x50],al
0000FFE2  55                push bp
0000FFE3  2028              and [bx+si],ch
0000FFE5  0038              add [bx+si],bh
0000FFE7  3038              xor [bx+si],bh
0000FFE9  37                aaa
0000FFEA  204650            and [bp+0x50],al
0000FFED  55                push bp
0000FFEE  2900              sub [bx+si],ax
0000FFF0  EA5BE000F0        jmp 0xf000:0xe05b
0000FFF5  3132              xor [bp+si],si
0000FFF7  2F                das
0000FFF8  3132              xor [bp+si],si
0000FFFA  2F                das
0000FFFB  3233              xor dh,[bp+di]
0000FFFD  00FF              add bh,bh
0000FFFF  33                db 0x33
