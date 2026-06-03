
### FUNCTION entry @ 1cfb:0010
1cfb:0010:	8c c0	MOV AX,ES
1cfb:0012:	05 10 00	ADD AX,0x10
1cfb:0015:	0e	PUSH CS
1cfb:0016:	1f	POP DS
1cfb:0017:	a3 04 00	MOV [0x4],AX
1cfb:001a:	03 06 0c 00	ADD AX,word ptr [0xc]
1cfb:001e:	8e c0	MOV ES,AX
1cfb:0020:	8b 0e 06 00	MOV CX,word ptr [0x6]
1cfb:0024:	8b f9	MOV DI,CX
1cfb:0026:	4f	DEC DI
1cfb:0027:	8b f7	MOV SI,DI
1cfb:0029:	fd	STD
1cfb:002a:	f3 a4	MOVSB.REP ES:DI,SI
1cfb:002c:	50	PUSH AX
1cfb:002d:	b8 32 00	MOV AX,0x32
1cfb:0030:	50	PUSH AX
1cfb:0031:	cb	RETF
