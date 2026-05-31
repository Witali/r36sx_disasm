;
; Tests conditional relative jumps.
; Uses: AX, ECX, Flags
;
; Opcodes tested, with positive and negative offsets:
;
; rel8  rel16/32 mnemonic condition
; 70    0F80     JO       OF=1
; 71    0F81     JNO      OF=0
; 72    0F82     JC       CF=1
; 73    0F83     JNC      CF=0
; 74    0F84     JZ       ZF=1
; 75    0F85     JNZ      ZF=0
; 76    0F86     JBE      CF=1 || ZF=1
; 77    0F87     JA       CF=0 && ZF=0
; 78    0F88     JS       SF=1
; 79    0F89     JNS      SF=0
; 7A    0F8A     JP       PF=1
; 7B    0F8B     JNP      PF=0
; 7C    0F8C     JL       SF!=OF
; 7D    0F8D     JNL      SF=OF
; 7E    0F8E     JLE      ZF=1 || SF!=OF
; 7F    0F8F     JNLE     ZF=0 && SF=OF
; E3             JCXZ     CX=0
; E3             JECXZ    ECX=0
;
%macro testJcc 1
%if %1==8
	VGA_TRACE 'T386 01 JCC8 START                  '
%else
	VGA_TRACE 'T386 01 JCC16 START                 '
%endif
	mov    ah, PS_CF
	sahf         ; don't use the stack (pushf/popf)
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNC CF=1 nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JNC CF=1 nojump       '
%endif
	jnc   %%err1 ; 73 / 0F83   JNC  CF=0
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JC CF=1 jump           '
%else
	VGA_TRACE 'T386 01 JCC16 JC CF=1 jump          '
%endif
	jc    %%jcok ; 72 / 0F82   JC   CF=1
	hlt
%%jz:
	mov    ah, PS_ZF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNZ ZF=1 nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JNZ ZF=1 nojump       '
%endif
	jnz   %%err1 ; 75 / 0F85   JNZ  ZF=0
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JZ ZF=1 jump           '
%else
	VGA_TRACE 'T386 01 JCC16 JZ ZF=1 jump          '
%endif
	jz    %%jzok ; 74 / 0F84   JZ   ZF=1
	hlt
%%jp:
	mov    ah, PS_PF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNP PF=1 nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JNP PF=1 nojump       '
%endif
	jnp   %%err1 ; 7B / 0F8B   JNP  PF=0
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JP PF=1 jump           '
%else
	VGA_TRACE 'T386 01 JCC16 JP PF=1 jump          '
%endif
	jp    %%jpok ; 7A / 0F8A   JP   PF=1
	hlt
%%js:
	mov    ah, PS_SF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNS SF=1 nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JNS SF=1 nojump       '
%endif
	jns   %%err1 ; 79 / 0F89   JNS  SF=0
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JS SF=1 jump           '
%else
	VGA_TRACE 'T386 01 JCC16 JS SF=1 jump          '
%endif
	js    %%jsok ; 78 / 0F88   JS   SF=1
	hlt
%%jna:
	mov    ah, PS_ZF|PS_CF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JA CF/ZF=1 nojump      '
%else
	VGA_TRACE 'T386 01 JCC16 JA CF/ZF=1 nojump     '
%endif
	ja    %%err1  ; 77 / 0F87   JA   CF=0 && ZF=0
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JBE CF/ZF=1 jump       '
%else
	VGA_TRACE 'T386 01 JCC16 JBE CF/ZF=1 jump      '
%endif
	jna   %%jnaok ; 76 / 0F86   JBE  CF=1 || ZF=1
%%next1:
	jmp %%jnc

%if %1==16
	times  128 hlt
%endif

%%err1:
	hlt

; test negative offsets
%%jcok:   jc   %%jz
%%jzok:   jz   %%jp
%%jpok:   jp   %%js
%%jsok:   js   %%jna
%%jnaok:  jna  %%next1


%%jnc:
	mov    ah, PS_SF|PS_ZF|PS_AF|PS_PF
	sahf
	mov    ax, 0
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNC CF=0 jump          '
%else
	VGA_TRACE 'T386 01 JCC16 JNC CF=0 jump         '
%endif
	jnc   %%jncok ; 73 / 0F83   JNC  CF=0
	hlt
%%jnz:
	mov    ah, PS_SF|PS_AF|PS_PF|PS_CF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNZ ZF=0 jump          '
%else
	VGA_TRACE 'T386 01 JCC16 JNZ ZF=0 jump         '
%endif
	jnz   %%jnzok ; 75 / 0F85   JNZ  ZF=0
	hlt
%%jnp:
	mov    ah, PS_SF|PS_ZF|PS_AF|PS_CF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNP PF=0 jump          '
%else
	VGA_TRACE 'T386 01 JCC16 JNP PF=0 jump         '
%endif
	jnp   %%jnpok ; 7B / 0F8B   JNP  PF=0
	hlt
%%jns:
	mov    ah, PS_ZF|PS_AF|PS_PF|PS_CF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNS SF=0 jump          '
%else
	VGA_TRACE 'T386 01 JCC16 JNS SF=0 jump         '
%endif
	jns   %%jnsok ; 79 /  0F89  JNS  SF=0
	hlt
%%ja:
	mov    ah, PS_SF|PS_AF|PS_PF
	sahf
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JA CF/ZF=0 jump        '
%else
	VGA_TRACE 'T386 01 JCC16 JA CF/ZF=0 jump       '
%endif
	ja    %%jaok  ; 77 / 0F87   JA   CF=0 && ZF=0
	hlt
%%next2:
	jmp   %%jo

%if %1==16
	times  128 hlt
%endif

; test negative offsets
%%jncok:  jnc  %%jnz
%%jnzok:  jnz  %%jnp
%%jnpok:  jnp  %%jns
%%jnsok:  jns  %%ja
%%jaok:   ja   %%next2

%%jo:
	mov    ah, 0
	sahf
	mov   al, 1000000b
	shl   al, 1    ; OF = high-order bit of AL <> (CF), ZF=0,SF=1,OF=1
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNO OF=1 nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JNO OF=1 nojump       '
%endif
	jno   %%err2
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JO OF=1 jump           '
%else
	VGA_TRACE 'T386 01 JCC16 JO OF=1 jump          '
%endif
	jo    %%jook
	hlt
%%jnl:
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JL SF=OF nojump        '
%else
	VGA_TRACE 'T386 01 JCC16 JL SF=OF nojump       '
%endif
	jl    %%err2   ; 7C / 0F8C   JL   SF!=OF
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JNL SF=OF jump         '
%else
	VGA_TRACE 'T386 01 JCC16 JNL SF=OF jump        '
%endif
	jnl   %%jnlok  ; 7D / 0F8D   JNL  SF=OF
	hlt
%%jnle:
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JLE ZF=0 SF=OF nojump  '
%else
	VGA_TRACE 'T386 01 JCC16 JLE ZF=0 SF=OF nojump '
%endif
	jle   %%err2   ; 7E / 0F8E   JLE  ZF=1 || SF!=OF
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JG ZF=0 SF=OF jump     '
%else
	VGA_TRACE 'T386 01 JCC16 JG ZF=0 SF=OF jump    '
%endif
	jnle  %%jnleok ; 7F / 0F8F   JNLE ZF=0 && SF=OF
	hlt
%%jl:
	mov ah, PS_ZF
	sahf           ; ZF=1,SF=0,OF=1
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JL SF!=OF jump         '
%else
	VGA_TRACE 'T386 01 JCC16 JL SF!=OF jump        '
%endif
	jl    %%jlok   ; 7C / 0F8C   JL   SF!=OF
	hlt
%%jle:
%if %1==8
	VGA_TRACE 'T386 01 JCC8 JLE ZF=1 jump          '
%else
	VGA_TRACE 'T386 01 JCC16 JLE ZF=1 jump         '
%endif
	jle   %%jleok  ; 7E / 0F8E   JLE  ZF=1 || SF!=OF
	hlt
%%jcxz:
%if %1==8
	mov ecx, 1
	VGA_TRACE 'T386 01 JCC8 JCXZ CX=1 nojump       '
	jcxz %%err2      ; E3   JCXZ  CX=0
	mov ecx, 0x10000
	VGA_TRACE 'T386 01 JCC8 JCXZ CX=0 jump         '
	jcxz %%jcxzok
%%jecxz:
	VGA_TRACE 'T386 01 JCC8 JECXZ ECX!=0 nojump    '
	jecxz %%err2
	mov ecx, 0
	VGA_TRACE 'T386 01 JCC8 JECXZ ECX=0 jump       '
	jecxz %%jecxzok ; E3   JECXZ   ECX=0
%%jecxze:
%endif
	jmp %%exit

%if %1==16
	times  128 hlt
%endif

%%err2:
	hlt

; test negative offsets
%%jook:   jo   %%jnl
%%jnlok:  jnl  %%jnle
%%jnleok: jnle %%jl
%%jlok:   jl   %%jle
%%jleok:  jle  %%jcxz
%if %1==8
%%jcxzok:  jcxz  %%jecxz
%%jecxzok: jecxz %%jecxze
%endif

%%exit:
%endmacro

