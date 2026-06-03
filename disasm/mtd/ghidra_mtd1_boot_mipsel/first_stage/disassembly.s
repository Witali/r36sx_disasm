
### FUNCTION first_stage_entry_copy_and_init @ bfe00800
### NOTE High confidence: NOR entry. Checks a boot magic at 0xbfe00014; if missing, copies code/data from the flash alias around 0xafc02000 to 0xbfe02000, then calls low-level init routines.
bfe00800:	e8 ff bd 27	addiu sp,sp,-0x18
bfe00804:	14 00 bf af	sw ra,0x14(sp)
bfe00808:	e0 bf 08 3c	lui t0,0xbfe0
bfe0080c:	14 00 08 35	ori t0,t0,0x14
bfe00810:	00 00 09 8d	lw t1,0x0(t0)
bfe00814:	ae ea 0a 3c	lui t2,0xeaae
bfe00818:	ae ea 4a 35	ori t2,t2,0xeaae
bfe0081c:	0e 00 2a 11	beq t1,t2,0xbfe00858
bfe00820:	00 00 00 00	_nop
bfe00824:	c0 af 08 3c	lui t0,0xafc0
bfe00828:	00 20 08 35	ori t0,t0,0x2000
bfe0082c:	e0 bf 09 3c	lui t1,0xbfe0
bfe00830:	00 20 29 35	ori t1,t1,0x2000
bfe00834:	e0 bf 0a 3c	lui t2,0xbfe0
bfe00838:	00 40 4a 35	ori t2,t2,0x4000
bfe0083c:	00 00 02 8d	lw v0,0x0(t0)
bfe00840:	00 00 22 ad	sw v0,0x0(t1)
bfe00844:	04 00 08 25	addiu t0,t0,0x4
bfe00848:	04 00 29 25	addiu t1,t1,0x4
bfe0084c:	2b 08 2a 01	sltu at,t1,t2
bfe00850:	fa ff 20 14	bne at,zero,0xbfe0083c
bfe00854:	00 00 00 00	_nop
bfe00858:	10 0b f8 0f	jal 0xbfe02c40
bfe0085c:	00 00 00 00	_nop
bfe00860:	0f 04 f8 0f	jal 0xbfe0103c
bfe00864:	00 00 00 00	_nop
bfe00868:	fa 04 f8 0f	jal 0xbfe013e8
bfe0086c:	00 00 00 00	_nop
bfe00870:	0d 09 f8 0f	jal 0xbfe02434
bfe00874:	00 00 00 00	_nop
bfe00878:	6d 03 f8 0f	jal 0xbfe00db4
bfe0087c:	00 00 00 00	_nop
bfe00880:	69 04 f8 0f	jal 0xbfe011a4
bfe00884:	00 00 00 00	_nop
bfe00888:	3d 09 f8 0f	jal 0xbfe024f4
bfe0088c:	00 00 00 00	_nop
bfe00890:	8b 03 f8 0f	jal 0xbfe00e2c
bfe00894:	00 00 00 00	_nop
bfe00898:	14 00 bf 8f	lw ra,0x14(sp)

### FUNCTION sub_bfe0089c @ bfe0089c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0089c:	18 00 bd 27	addiu sp,sp,0x18
bfe008a0:	08 00 e0 03	jr ra
bfe008a4:	00 00 00 00	_nop

### FUNCTION sub_bfe008a8 @ bfe008a8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe008a8:	00 00 00 00	nop
bfe008ac:	00 00 00 00	nop
bfe008b0:	00 00 80 b8	swr zero,0x0(a0)
bfe008b4:	00 10 80 b8	swr zero,0x1000(a0)
bfe008b8:	00 e0 83 b8	swr v1,-0x2000(a0)
bfe008bc:	00 18 80 b8	swr zero,0x1800(a0)

### FUNCTION FUN_bfe00db4 @ bfe00db4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00db4:	e0 ff bd 27	addiu sp,sp,-0x20
bfe00db8:	14 00 b1 af	sw s1,0x14(sp)
bfe00dbc:	e0 bf 11 3c	lui s1,0xbfe0
bfe00dc0:	18 00 b2 af	sw s2,0x18(sp)
bfe00dc4:	10 00 b0 af	sw s0,0x10(sp)
bfe00dc8:	1c 00 bf af	sw ra,0x1c(sp)
bfe00dcc:	25 80 00 00	or s0,zero,zero
bfe00dd0:	c4 08 31 26	addiu s1,s1,0x8c4
bfe00dd4:	e0 bf 12 3c	lui s2,0xbfe0
bfe00dd8:	c0 08 42 8e	lw v0,0x8c0(s2)
bfe00ddc:	00 21 10 00	sll a0,s0,0x4
bfe00de0:	82 10 02 00	srl v0,v0,0x2
bfe00de4:	2b 10 02 02	sltu v0,s0,v0
bfe00de8:	05 00 40 10	beq v0,zero,0xbfe00e00
bfe00dec:	21 20 24 02	_addu a0,s1,a0
bfe00df0:	9b 03 f8 0f	jal 0xbfe00e6c
bfe00df4:	00 00 00 00	_nop
bfe00df8:	0a 00 40 10	beq v0,zero,0xbfe00e24
bfe00dfc:	00 00 00 00	_nop
bfe00e00:	1c 00 bf 8f	lw ra,0x1c(sp)
bfe00e04:	18 00 b2 8f	lw s2,0x18(sp)
bfe00e08:	14 00 b1 8f	lw s1,0x14(sp)
bfe00e0c:	10 00 b0 8f	lw s0,0x10(sp)
bfe00e10:	20 00 06 24	li a2,0x20
bfe00e14:	25 28 00 00	or a1,zero,zero
bfe00e18:	00 a0 04 3c	lui a0,0xa000
bfe00e1c:	8f 03 f8 0b	j 0xbfe00e3c

### FUNCTION sub_bfe00e20 @ bfe00e20
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e20:	20 00 bd 27	_addiu sp,sp,0x20
bfe00e24:	ec ff 00 10	b 0xbfe00dd8
bfe00e28:	01 00 10 26	_addiu s0,s0,0x1

### FUNCTION FUN_bfe00e2c @ bfe00e2c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e2c:	e0 bf 02 3c	lui v0,0xbfe0
bfe00e30:	90 2e 59 8c	lw t9,0x2e90(v0)
bfe00e34:	08 00 20 03	jr t9
bfe00e38:	00 00 00 00	_nop

### FUNCTION FUN_bfe00e3c @ bfe00e3c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e3c:	25 10 80 00	or v0,a0,zero
bfe00e40:	25 18 80 00	or v1,a0,zero
bfe00e44:	21 30 86 00	addu a2,a0,a2
bfe00e48:	03 00 66 14	bne v1,a2,0xbfe00e58
bfe00e4c:	00 00 00 00	_nop
bfe00e50:	08 00 e0 03	jr ra
bfe00e54:	00 00 00 00	_nop

### FUNCTION sub_bfe00e58 @ bfe00e58
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e58:	00 00 65 a0	sb a1,0x0(v1)
bfe00e5c:	fa ff 00 10	b 0xbfe00e48
bfe00e60:	01 00 63 24	_addiu v1,v1,0x1

### FUNCTION FUN_bfe00e64 @ bfe00e64
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e64:	08 00 e0 03	jr ra
bfe00e68:	00 00 00 00	_nop

### FUNCTION FUN_bfe00e6c @ bfe00e6c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00e6c:	04 00 83 8c	lw v1,0x4(a0)
bfe00e70:	02 14 03 00	srl v0,v1,0x10
bfe00e74:	04 00 45 2c	sltiu a1,v0,0x4
bfe00e78:	07 00 a0 10	beq a1,zero,0xbfe00e98
bfe00e7c:	e0 bf 05 3c	_lui a1,0xbfe0
bfe00e80:	80 10 02 00	sll v0,v0,0x2
bfe00e84:	b0 08 a5 24	addiu a1,a1,0x8b0
bfe00e88:	21 10 45 00	addu v0,v0,a1
bfe00e8c:	00 00 42 8c	lw v0,0x0(v0)
bfe00e90:	ff ff 63 30	andi v1,v1,0xffff
bfe00e94:	25 18 62 00	or v1,v1,v0
bfe00e98:	00 00 82 8c	lw v0,0x0(a0)
bfe00e9c:	01 00 05 24	li a1,0x1
bfe00ea0:	0b 00 45 14	bne v0,a1,0xbfe00ed0
bfe00ea4:	08 00 05 24	_li a1,0x8
bfe00ea8:	03 00 00 10	b 0xbfe00eb8
bfe00eac:	25 18 00 00	_or v1,zero,zero
bfe00eb0:	00 00 00 00	nop
bfe00eb4:	01 00 63 24	addiu v1,v1,0x1
bfe00eb8:	04 00 82 8c	lw v0,0x4(a0)
bfe00ebc:	2b 10 62 00	sltu v0,v1,v0
bfe00ec0:	fb ff 40 14	bne v0,zero,0xbfe00eb0
bfe00ec4:	00 00 00 00	_nop
bfe00ec8:	08 00 e0 03	jr ra
bfe00ecc:	25 10 00 00	_or v0,zero,zero

### FUNCTION sub_bfe00ed0 @ bfe00ed0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe00ed0:	04 00 45 14	bne v0,a1,0xbfe00ee4
bfe00ed4:	16 00 05 24	_li a1,0x16
bfe00ed8:	08 00 82 90	lbu v0,0x8(a0)
bfe00edc:	fa ff 00 10	b 0xbfe00ec8
bfe00ee0:	00 00 62 a0	_sb v0,0x0(v1)
bfe00ee4:	04 00 45 14	bne v0,a1,0xbfe00ef8
bfe00ee8:	32 00 05 24	_li a1,0x32
bfe00eec:	08 00 82 94	lhu v0,0x8(a0)
bfe00ef0:	f5 ff 00 10	b 0xbfe00ec8
bfe00ef4:	00 00 62 a4	_sh v0,0x0(v1)
bfe00ef8:	04 00 45 14	bne v0,a1,0xbfe00f0c
bfe00efc:	32 01 05 24	_li a1,0x132
bfe00f00:	08 00 82 8c	lw v0,0x8(a0)
bfe00f04:	f0 ff 00 10	b 0xbfe00ec8
bfe00f08:	00 00 62 ac	_sw v0,0x0(v1)
bfe00f0c:	08 00 45 14	bne v0,a1,0xbfe00f30
bfe00f10:	32 02 05 24	_li a1,0x232
bfe00f14:	00 00 65 8c	lw a1,0x0(v1)
bfe00f18:	08 00 82 8c	lw v0,0x8(a0)
bfe00f1c:	0c 00 84 8c	lw a0,0xc(a0)
bfe00f20:	26 10 a2 00	xor v0,a1,v0
bfe00f24:	24 10 44 00	and v0,v0,a0
bfe00f28:	f6 ff 00 10	b 0xbfe00f04
bfe00f2c:	26 10 45 00	_xor v0,v0,a1
bfe00f30:	09 00 45 14	bne v0,a1,0xbfe00f58
bfe00f34:	32 00 05 3c	_lui a1,0x32
bfe00f38:	08 00 82 8c	lw v0,0x8(a0)
bfe00f3c:	00 00 62 ac	sw v0,0x0(v1)
bfe00f40:	08 00 82 8c	lw v0,0x8(a0)
bfe00f44:	00 00 64 8c	lw a0,0x0(v1)
bfe00f48:	fe ff 82 14	bne a0,v0,0xbfe00f44
bfe00f4c:	00 00 00 00	_nop
bfe00f50:	dd ff 00 10	b 0xbfe00ec8
bfe00f54:	00 00 00 00	_nop
bfe00f58:	04 00 45 14	bne v0,a1,0xbfe00f6c
bfe00f5c:	32 01 05 3c	_lui a1,0x132
bfe00f60:	00 00 62 8c	lw v0,0x0(v1)
bfe00f64:	d8 ff 00 10	b 0xbfe00ec8
bfe00f68:	00 00 00 00	_nop
bfe00f6c:	09 00 45 14	bne v0,a1,0xbfe00f94
bfe00f70:	32 02 05 3c	_lui a1,0x232
bfe00f74:	0c 00 85 8c	lw a1,0xc(a0)
bfe00f78:	08 00 84 8c	lw a0,0x8(a0)
bfe00f7c:	00 00 62 8c	lw v0,0x0(v1)
bfe00f80:	24 10 45 00	and v0,v0,a1
bfe00f84:	fd ff 44 14	bne v0,a0,0xbfe00f7c
bfe00f88:	00 00 00 00	_nop
bfe00f8c:	ce ff 00 10	b 0xbfe00ec8
bfe00f90:	00 00 00 00	_nop
bfe00f94:	09 00 45 14	bne v0,a1,0xbfe00fbc
bfe00f98:	32 04 05 24	_li a1,0x432
bfe00f9c:	0c 00 85 8c	lw a1,0xc(a0)
bfe00fa0:	08 00 84 8c	lw a0,0x8(a0)
bfe00fa4:	00 00 62 8c	lw v0,0x0(v1)
bfe00fa8:	24 10 45 00	and v0,v0,a1
bfe00fac:	fd ff 44 10	beq v0,a0,0xbfe00fa4
bfe00fb0:	00 00 00 00	_nop
bfe00fb4:	c4 ff 00 10	b 0xbfe00ec8
bfe00fb8:	00 00 00 00	_nop
bfe00fbc:	07 00 45 14	bne v0,a1,0xbfe00fdc
bfe00fc0:	32 03 05 24	_li a1,0x332
bfe00fc4:	08 00 82 8c	lw v0,0x8(a0)
bfe00fc8:	0c 00 84 8c	lw a0,0xc(a0)
bfe00fcc:	00 00 65 8c	lw a1,0x0(v1)
bfe00fd0:	04 10 82 00	sllv v0,v0,a0
bfe00fd4:	cb ff 00 10	b 0xbfe00f04
bfe00fd8:	21 10 45 00	_addu v0,v0,a1
bfe00fdc:	07 00 45 14	bne v0,a1,0xbfe00ffc
bfe00fe0:	00 00 00 00	_nop
bfe00fe4:	08 00 85 8c	lw a1,0x8(a0)
bfe00fe8:	0c 00 84 8c	lw a0,0xc(a0)
bfe00fec:	00 00 62 8c	lw v0,0x0(v1)
bfe00ff0:	04 20 85 00	sllv a0,a1,a0
bfe00ff4:	c3 ff 00 10	b 0xbfe00f04
bfe00ff8:	23 10 44 00	_subu v0,v0,a0
bfe00ffc:	bb aa 03 3c	lui v1,0xaabb
bfe01000:	dd cc 63 34	ori v1,v1,0xccdd
bfe01004:	09 00 43 14	bne v0,v1,0xbfe0102c
bfe01008:	21 52 03 3c	_lui v1,0x5221

### FUNCTION sub_bfe0100c @ bfe0100c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0100c:	e8 ff bd 27	addiu sp,sp,-0x18
bfe01010:	14 00 bf af	sw ra,0x14(sp)
bfe01014:	99 03 f8 0f	jal 0xbfe00e64
bfe01018:	00 00 00 00	_nop
bfe0101c:	14 00 bf 8f	lw ra,0x14(sp)
bfe01020:	25 10 00 00	or v0,zero,zero
bfe01024:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01028 @ bfe01028
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01028:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION sub_bfe0102c @ bfe0102c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0102c:	f0 33 63 24	addiu v1,v1,0x33f0
bfe01030:	21 10 43 00	addu v0,v0,v1
bfe01034:	08 00 e0 03	jr ra
bfe01038:	01 00 42 2c	_sltiu v0,v0,0x1

### FUNCTION FUN_bfe0103c @ bfe0103c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0103c:	80 b8 04 3c	lui a0,0xb880
bfe01040:	60 00 82 8c	lw v0,0x60(a0)
bfe01044:	ff 7f 03 3c	lui v1,0x7fff
bfe01048:	ff ff 63 34	ori v1,v1,0xffff
bfe0104c:	24 10 43 00	and v0,v0,v1
bfe01050:	60 00 82 ac	sw v0,0x60(a0)
bfe01054:	81 b8 03 3c	lui v1,0xb881
bfe01058:	e0 bf 04 3c	lui a0,0xbfe0
bfe0105c:	70 8a 63 34	ori v1,v1,0x8a70
bfe01060:	00 00 84 24	addiu a0,a0,0x0
bfe01064:	00 00 69 8c	lw t1,0x0(v1)
bfe01068:	30 00 85 90	lbu a1,0x30(a0)
bfe0106c:	01 00 02 24	li v0,0x1
bfe01070:	ff 00 28 31	andi t0,t1,0xff
bfe01074:	90 ff 63 8c	lw v1,-0x70(v1)
bfe01078:	39 00 a2 14	bne a1,v0,0xbfe01160
bfe0107c:	ff bf 02 3c	_lui v0,0xbfff
bfe01080:	00 40 02 3c	lui v0,0x4000
bfe01084:	25 18 62 00	or v1,v1,v0
bfe01088:	31 00 84 90	lbu a0,0x31(a0)
bfe0108c:	01 00 02 24	li v0,0x1
bfe01090:	36 00 82 14	bne a0,v0,0xbfe0116c
bfe01094:	ff 7f 02 3c	_lui v0,0x7fff
bfe01098:	00 80 02 3c	lui v0,0x8000
bfe0109c:	25 18 62 00	or v1,v1,v0
bfe010a0:	00 fc 04 3c	lui a0,0xfc00
bfe010a4:	ff ff 84 34	ori a0,a0,0xffff
bfe010a8:	a5 00 02 24	li v0,0xa5
bfe010ac:	06 00 02 11	beq t0,v0,0xbfe010c8
bfe010b0:	24 20 64 00	_and a0,v1,a0
bfe010b4:	5a 00 02 24	li v0,0x5a
bfe010b8:	03 00 02 11	beq t0,v0,0xbfe010c8
bfe010bc:	55 00 02 24	_li v0,0x55
bfe010c0:	2d 00 02 15	bne t0,v0,0xbfe01178
bfe010c4:	25 28 00 00	_or a1,zero,zero
bfe010c8:	81 b8 06 3c	lui a2,0xb881
bfe010cc:	0c 8a c6 34	ori a2,a2,0x8a0c
bfe010d0:	00 00 c5 8c	lw a1,0x0(a2)
bfe010d4:	04 00 c2 8c	lw v0,0x4(a2)
bfe010d8:	fe ff c6 94	lhu a2,-0x2(a2)
bfe010dc:	21 28 a2 00	addu a1,a1,v0
bfe010e0:	02 14 03 00	srl v0,v1,0x10
bfe010e4:	21 10 46 00	addu v0,v0,a2
bfe010e8:	ff 03 42 30	andi v0,v0,0x3ff
bfe010ec:	e8 03 43 2c	sltiu v1,v0,0x3e8
bfe010f0:	03 00 60 14	bne v1,zero,0xbfe01100
bfe010f4:	00 00 00 00	_nop
bfe010f8:	01 00 a5 24	addiu a1,a1,0x1
bfe010fc:	18 fc 42 24	addiu v0,v0,-0x3e8
bfe01100:	00 14 02 00	sll v0,v0,0x10
bfe01104:	25 20 82 00	or a0,a0,v0
bfe01108:	80 b8 03 3c	lui v1,0xb880
bfe0110c:	80 00 66 8c	lw a2,0x80(v1)
bfe01110:	40 00 02 3c	lui v0,0x40
bfe01114:	bf ff 07 3c	lui a3,0xffbf
bfe01118:	25 30 c2 00	or a2,a2,v0
bfe0111c:	80 00 66 ac	sw a2,0x80(v1)
bfe01120:	80 00 66 8c	lw a2,0x80(v1)
bfe01124:	ff ff e7 34	ori a3,a3,0xffff
bfe01128:	00 ff 02 24	li v0,-0x100
bfe0112c:	24 30 c7 00	and a2,a2,a3
bfe01130:	80 00 66 ac	sw a2,0x80(v1)
bfe01134:	24 10 22 01	and v0,t1,v0
bfe01138:	81 b8 03 3c	lui v1,0xb881
bfe0113c:	70 8a 63 34	ori v1,v1,0x8a70
bfe01140:	25 10 02 01	or v0,t0,v0
bfe01144:	00 00 62 ac	sw v0,0x0(v1)
bfe01148:	04 00 02 24	li v0,0x4
bfe0114c:	90 ff 64 ac	sw a0,-0x70(v1)
bfe01150:	a0 ff 65 ac	sw a1,-0x60(v1)
bfe01154:	98 ff 62 ac	sw v0,-0x68(v1)
bfe01158:	08 00 e0 03	jr ra
bfe0115c:	00 00 00 00	_nop

### FUNCTION sub_bfe01160 @ bfe01160
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01160:	ff ff 42 34	ori v0,v0,0xffff
bfe01164:	c8 ff 00 10	b 0xbfe01088
bfe01168:	24 18 62 00	_and v1,v1,v0
bfe0116c:	ff ff 42 34	ori v0,v0,0xffff
bfe01170:	cb ff 00 10	b 0xbfe010a0
bfe01174:	24 18 62 00	_and v1,v1,v0
bfe01178:	e3 ff 00 10	b 0xbfe01108
bfe0117c:	55 00 08 24	_li t0,0x55

### FUNCTION FUN_bfe01180 @ bfe01180
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01180:	81 b8 03 3c	lui v1,0xb881
bfe01184:	0c 8a 63 34	ori v1,v1,0x8a0c
bfe01188:	00 00 64 8c	lw a0,0x0(v1)
bfe0118c:	fe ff 62 94	lhu v0,-0x2(v1)
bfe01190:	e8 03 03 24	li v1,0x3e8
bfe01194:	18 00 83 00	mult a0,v1
bfe01198:	12 20 00 00	mflo a0
bfe0119c:	08 00 e0 03	jr ra
bfe011a0:	21 10 82 00	_addu v0,a0,v0

### FUNCTION FUN_bfe011a4 @ bfe011a4
### NOTE References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
bfe011a4:	c8 ff bd 27	addiu sp,sp,-0x38
bfe011a8:	34 00 bf af	sw ra,0x34(sp)
bfe011ac:	30 00 b5 af	sw s5,0x30(sp)
bfe011b0:	2c 00 b4 af	sw s4,0x2c(sp)
bfe011b4:	28 00 b3 af	sw s3,0x28(sp)
bfe011b8:	24 00 b2 af	sw s2,0x24(sp)
bfe011bc:	20 00 b1 af	sw s1,0x20(sp)
bfe011c0:	60 04 f8 0f	jal 0xbfe01180
bfe011c4:	1c 00 b0 af	_sw s0,0x1c(sp)
bfe011c8:	e0 bf 03 3c	lui v1,0xbfe0
bfe011cc:	00 00 63 24	addiu v1,v1,0x0
bfe011d0:	25 88 40 00	or s1,v0,zero
bfe011d4:	2f 00 62 88	lwl v0,0x2f(v1)
bfe011d8:	e0 bf 04 3c	lui a0,0xbfe0
bfe011dc:	2b 00 66 88	lwl a2,0x2b(v1)
bfe011e0:	2c 00 62 98	lwr v0,0x2c(v1)
bfe011e4:	17 00 75 88	lwl s5,0x17(v1)
bfe011e8:	23 00 74 88	lwl s4,0x23(v1)
bfe011ec:	90 2e 82 ac	sw v0,0x2e90(a0)
bfe011f0:	47 00 64 88	lwl a0,0x47(v1)
bfe011f4:	27 00 73 88	lwl s3,0x27(v1)
bfe011f8:	28 00 66 98	lwr a2,0x28(v1)
bfe011fc:	43 00 72 88	lwl s2,0x43(v1)
bfe01200:	44 00 64 98	lwr a0,0x44(v1)
bfe01204:	14 00 75 98	lwr s5,0x14(v1)
bfe01208:	20 00 74 98	lwr s4,0x20(v1)
bfe0120c:	24 00 73 98	lwr s3,0x24(v1)
bfe01210:	25 80 c0 00	or s0,a2,zero
bfe01214:	48 0b f8 0f	jal 0xbfe02d20
bfe01218:	40 00 72 98	_lwr s2,0x40(v1)
bfe0121c:	ae ea 02 3c	lui v0,0xeaae
bfe01220:	ae ea 42 34	ori v0,v0,0xeaae
bfe01224:	14 00 a2 16	bne s5,v0,0xbfe01278
bfe01228:	82 b8 02 3c	_lui v0,0xb882
bfe0122c:	ff 00 47 32	andi a3,s2,0xff
bfe01230:	25 30 00 02	or a2,s0,zero
bfe01234:	25 28 60 02	or a1,s3,zero
bfe01238:	72 0a f8 0f	jal 0xbfe029c8
bfe0123c:	25 20 80 02	_or a0,s4,zero
bfe01240:	60 04 f8 0f	jal 0xbfe01180
bfe01244:	00 00 00 00	_nop
bfe01248:	34 00 bf 8f	lw ra,0x34(sp)
bfe0124c:	23 10 51 00	subu v0,v0,s1
bfe01250:	00 a0 03 3c	lui v1,0xa000
bfe01254:	30 00 b5 8f	lw s5,0x30(sp)
bfe01258:	2c 00 b4 8f	lw s4,0x2c(sp)
bfe0125c:	28 00 b3 8f	lw s3,0x28(sp)
bfe01260:	24 00 b2 8f	lw s2,0x24(sp)
bfe01264:	20 00 b1 8f	lw s1,0x20(sp)
bfe01268:	1c 00 b0 8f	lw s0,0x1c(sp)
bfe0126c:	06 00 62 a4	sh v0,0x6(v1)
bfe01270:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01274 @ bfe01274
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01274:	38 00 bd 27	_addiu sp,sp,0x38

### FUNCTION first_stage_spi_nor_command_path @ bfe01278
### NOTE Medium confidence: talks to the SPI/SF controller registers around 0xb882e0c8 and prepares small command buffers on the stack.
bfe01278:	c8 e0 42 34	ori v0,v0,0xe0c8
bfe0127c:	00 00 43 8c	lw v1,0x0(v0)
bfe01280:	3f 00 06 3c	lui a2,0x3f
bfe01284:	00 01 04 3c	lui a0,0x100
bfe01288:	25 18 64 00	or v1,v1,a0
bfe0128c:	ff ff c6 34	ori a2,a2,0xffff
bfe01290:	00 00 43 ac	sw v1,0x0(v0)
bfe01294:	24 30 06 02	and a2,s0,a2
bfe01298:	01 00 03 24	li v1,0x1
bfe0129c:	d1 ff 43 a0	sb v1,-0x2f(v0)
bfe012a0:	02 14 06 00	srl v0,a2,0x10
bfe012a4:	11 00 a2 a3	sb v0,0x11(sp)
bfe012a8:	02 12 06 00	srl v0,a2,0x8
bfe012ac:	12 00 a2 a3	sb v0,0x12(sp)
bfe012b0:	ff ff 02 24	li v0,-0x1
bfe012b4:	14 00 a2 a3	sb v0,0x14(sp)
bfe012b8:	01 00 02 24	li v0,0x1
bfe012bc:	29 00 42 16	bne s2,v0,0xbfe01364
bfe012c0:	13 00 a6 a3	_sb a2,0x13(sp)
bfe012c4:	03 00 02 24	li v0,0x3
bfe012c8:	10 00 a2 a3	sb v0,0x10(sp)
bfe012cc:	04 00 05 24	li a1,0x4
bfe012d0:	82 b8 03 3c	lui v1,0xb882
bfe012d4:	c8 e0 63 34	ori v1,v1,0xe0c8
bfe012d8:	00 00 64 8c	lw a0,0x0(v1)
bfe012dc:	00 06 06 3c	lui a2,0x600
bfe012e0:	25 20 86 00	or a0,a0,a2
bfe012e4:	00 00 64 ac	sw a0,0x0(v1)
bfe012e8:	00 00 62 8c	lw v0,0x0(v1)
bfe012ec:	10 00 a4 27	addiu a0,sp,0x10
bfe012f0:	25 10 46 00	or v0,v0,a2
bfe012f4:	00 00 62 ac	sw v0,0x0(v1)
bfe012f8:	c0 af 06 3c	lui a2,0xafc0
bfe012fc:	25 10 80 00	or v0,a0,zero
bfe01300:	23 18 44 00	subu v1,v0,a0
bfe01304:	2b 18 65 00	sltu v1,v1,a1
bfe01308:	1a 00 60 14	bne v1,zero,0xbfe01374
bfe0130c:	00 00 00 00	_nop
bfe01310:	82 b8 03 3c	lui v1,0xb882
bfe01314:	01 00 02 24	li v0,0x1
bfe01318:	1b 00 42 16	bne s2,v0,0xbfe01388
bfe0131c:	98 e0 63 34	_ori v1,v1,0xe098
bfe01320:	00 00 62 8c	lw v0,0x0(v1)
bfe01324:	f8 ff 04 3c	lui a0,0xfff8
bfe01328:	ff ff 84 34	ori a0,a0,0xffff
bfe0132c:	24 10 44 00	and v0,v0,a0
bfe01330:	00 00 62 ac	sw v0,0x0(v1)
bfe01334:	25 20 60 02	or a0,s3,zero
bfe01338:	6d 0b f8 0f	jal 0xbfe02db4
bfe0133c:	25 28 80 02	_or a1,s4,zero
bfe01340:	82 b8 02 3c	lui v0,0xb882
bfe01344:	c8 e0 42 34	ori v0,v0,0xe0c8
bfe01348:	00 00 43 8c	lw v1,0x0(v0)
bfe0134c:	ff f9 04 3c	lui a0,0xf9ff
bfe01350:	ff ff 84 34	ori a0,a0,0xffff
bfe01354:	24 18 64 00	and v1,v1,a0
bfe01358:	00 00 43 ac	sw v1,0x0(v0)
bfe0135c:	b8 ff 00 10	b 0xbfe01240
bfe01360:	00 00 00 00	_nop
bfe01364:	3b 00 02 24	li v0,0x3b
bfe01368:	10 00 a2 a3	sb v0,0x10(sp)
bfe0136c:	d8 ff 00 10	b 0xbfe012d0
bfe01370:	05 00 05 24	_li a1,0x5
bfe01374:	00 00 43 90	lbu v1,0x0(v0)
bfe01378:	01 00 42 24	addiu v0,v0,0x1
bfe0137c:	00 00 c3 a0	sb v1,0x0(a2)
bfe01380:	e0 ff 00 10	b 0xbfe01304
bfe01384:	23 18 44 00	_subu v1,v0,a0
bfe01388:	00 00 62 8c	lw v0,0x0(v1)
bfe0138c:	f8 ff 04 3c	lui a0,0xfff8
bfe01390:	ff ff 84 34	ori a0,a0,0xffff
bfe01394:	24 10 44 00	and v0,v0,a0
bfe01398:	01 00 04 3c	lui a0,0x1
bfe0139c:	e4 ff 00 10	b 0xbfe01330
bfe013a0:	25 10 44 00	_or v0,v0,a0

### FUNCTION FUN_bfe013a4 @ bfe013a4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe013a4:	e8 ff bd 27	addiu sp,sp,-0x18
bfe013a8:	14 00 bf af	sw ra,0x14(sp)
bfe013ac:	60 04 f8 0f	jal 0xbfe01180
bfe013b0:	25 28 80 00	_or a1,a0,zero
bfe013b4:	e7 03 a4 24	addiu a0,a1,0x3e7
bfe013b8:	e8 03 05 24	li a1,0x3e8
bfe013bc:	1b 00 85 00	divu a0,a1
bfe013c0:	f4 01 a0 00	teq a1,zero
bfe013c4:	12 20 00 00	mflo a0
bfe013c8:	21 28 44 00	addu a1,v0,a0
bfe013cc:	60 04 f8 0f	jal 0xbfe01180
bfe013d0:	00 00 00 00	_nop
bfe013d4:	2b 10 45 00	sltu v0,v0,a1
bfe013d8:	fc ff 40 14	bne v0,zero,0xbfe013cc
bfe013dc:	14 00 bf 8f	_lw ra,0x14(sp)
bfe013e0:	08 00 e0 03	jr ra

### FUNCTION sub_bfe013e4 @ bfe013e4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe013e4:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION FUN_bfe013e8 @ bfe013e8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe013e8:	08 00 e0 03	jr ra
bfe013ec:	00 00 00 00	_nop

### FUNCTION FUN_bfe013f0 @ bfe013f0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe013f0:	02 00 a0 10	beq a1,zero,0xbfe013fc
bfe013f4:	02 01 02 24	_li v0,0x102
bfe013f8:	06 01 a2 24	addiu v0,a1,0x106
bfe013fc:	21 10 44 00	addu v0,v0,a0
bfe01400:	00 00 42 94	lhu v0,0x0(v0)
bfe01404:	01 00 42 30	andi v0,v0,0x1
bfe01408:	03 00 40 14	bne v0,zero,0xbfe01418
bfe0140c:	ff ff 02 24	_li v0,-0x1
bfe01410:	08 00 e0 03	jr ra
bfe01414:	00 00 e0 ac	_sw zero,0x0(a3)

### FUNCTION sub_bfe01418 @ bfe01418
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01418:	08 01 82 24	addiu v0,a0,0x108
bfe0141c:	21 10 45 00	addu v0,v0,a1
bfe01420:	00 00 43 94	lhu v1,0x0(v0)
bfe01424:	00 00 e2 8c	lw v0,0x0(a3)
bfe01428:	ff ff 63 30	andi v1,v1,0xffff
bfe0142c:	2b 40 62 00	sltu t0,v1,v0
bfe01430:	02 00 00 11	beq t0,zero,0xbfe0143c
bfe01434:	00 00 00 00	_nop
bfe01438:	25 10 60 00	or v0,v1,zero
bfe0143c:	82 28 05 00	srl a1,a1,0x2
bfe01440:	20 00 84 24	addiu a0,a0,0x20
bfe01444:	00 00 e2 ac	sw v0,0x0(a3)
bfe01448:	21 28 a4 00	addu a1,a1,a0
bfe0144c:	03 00 c3 30	andi v1,a2,0x3
bfe01450:	1c 00 60 10	beq v1,zero,0xbfe014c4
bfe01454:	00 00 00 00	_nop
bfe01458:	03 00 40 14	bne v0,zero,0xbfe01468
bfe0145c:	00 00 00 00	_nop
bfe01460:	08 00 e0 03	jr ra
bfe01464:	25 10 00 00	_or v0,zero,zero

### FUNCTION sub_bfe01468 @ bfe01468
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01468:	00 00 a3 90	lbu v1,0x0(a1)
bfe0146c:	01 00 c6 24	addiu a2,a2,0x1
bfe01470:	ff ff 42 24	addiu v0,v0,-0x1
bfe01474:	f5 ff 00 10	b 0xbfe0144c
bfe01478:	ff ff c3 a0	_sb v1,-0x1(a2)
bfe0147c:	00 00 a4 8c	lw a0,0x0(a1)
bfe01480:	04 00 63 24	addiu v1,v1,0x4
bfe01484:	fc ff 64 ac	sw a0,-0x4(v1)
bfe01488:	23 20 e3 00	subu a0,a3,v1
bfe0148c:	04 00 84 2c	sltiu a0,a0,0x4
bfe01490:	fa ff 80 10	beq a0,zero,0xbfe0147c
bfe01494:	00 00 00 00	_nop
bfe01498:	82 18 02 00	srl v1,v0,0x2
bfe0149c:	80 20 03 00	sll a0,v1,0x2
bfe014a0:	21 30 c4 00	addu a2,a2,a0
bfe014a4:	23 10 44 00	subu v0,v0,a0
bfe014a8:	21 10 c2 00	addu v0,a2,v0
bfe014ac:	ec ff c2 10	beq a2,v0,0xbfe01460
bfe014b0:	00 00 00 00	_nop
bfe014b4:	00 00 a3 90	lbu v1,0x0(a1)
bfe014b8:	01 00 c6 24	addiu a2,a2,0x1
bfe014bc:	fb ff 00 10	b 0xbfe014ac
bfe014c0:	ff ff c3 a0	_sb v1,-0x1(a2)
bfe014c4:	25 18 c0 00	or v1,a2,zero
bfe014c8:	ef ff 00 10	b 0xbfe01488
bfe014cc:	21 38 c2 00	_addu a3,a2,v0

### FUNCTION FUN_bfe014d0 @ bfe014d0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe014d0:	02 01 83 24	addiu v1,a0,0x102
bfe014d4:	21 18 65 00	addu v1,v1,a1
bfe014d8:	00 00 63 90	lbu v1,0x0(v1)
bfe014dc:	01 00 a2 2c	sltiu v0,a1,0x1
bfe014e0:	01 00 42 24	addiu v0,v0,0x1
bfe014e4:	24 18 43 00	and v1,v0,v1
bfe014e8:	25 00 60 14	bne v1,zero,0xbfe01580
bfe014ec:	ff ff 02 24	_li v0,-0x1
bfe014f0:	82 28 05 00	srl a1,a1,0x2
bfe014f4:	20 00 84 24	addiu a0,a0,0x20
bfe014f8:	21 28 a4 00	addu a1,a1,a0
bfe014fc:	03 00 c2 30	andi v0,a2,0x3
bfe01500:	1c 00 40 10	beq v0,zero,0xbfe01574
bfe01504:	00 00 00 00	_nop
bfe01508:	03 00 e0 14	bne a3,zero,0xbfe01518
bfe0150c:	01 00 c6 24	_addiu a2,a2,0x1
bfe01510:	08 00 e0 03	jr ra
bfe01514:	25 10 00 00	_or v0,zero,zero

### FUNCTION sub_bfe01518 @ bfe01518
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01518:	ff ff c2 90	lbu v0,-0x1(a2)
bfe0151c:	ff ff e7 24	addiu a3,a3,-0x1
bfe01520:	00 00 a2 a0	sb v0,0x0(a1)
bfe01524:	f6 ff 00 10	b 0xbfe01500
bfe01528:	03 00 c2 30	_andi v0,a2,0x3
bfe0152c:	00 00 43 8c	lw v1,0x0(v0)
bfe01530:	04 00 42 24	addiu v0,v0,0x4
bfe01534:	00 00 a3 ac	sw v1,0x0(a1)
bfe01538:	23 18 82 00	subu v1,a0,v0
bfe0153c:	04 00 63 2c	sltiu v1,v1,0x4
bfe01540:	fa ff 60 10	beq v1,zero,0xbfe0152c
bfe01544:	00 00 00 00	_nop
bfe01548:	82 10 07 00	srl v0,a3,0x2
bfe0154c:	80 18 02 00	sll v1,v0,0x2
bfe01550:	21 30 c3 00	addu a2,a2,v1
bfe01554:	23 38 e3 00	subu a3,a3,v1
bfe01558:	21 38 c7 00	addu a3,a2,a3
bfe0155c:	ec ff c7 10	beq a2,a3,0xbfe01510
bfe01560:	01 00 c6 24	_addiu a2,a2,0x1
bfe01564:	ff ff c2 90	lbu v0,-0x1(a2)
bfe01568:	00 00 a2 a0	sb v0,0x0(a1)
bfe0156c:	fb ff 00 10	b 0xbfe0155c
bfe01570:	00 00 00 00	_nop
bfe01574:	25 10 c0 00	or v0,a2,zero
bfe01578:	ef ff 00 10	b 0xbfe01538
bfe0157c:	21 20 c7 00	_addu a0,a2,a3

### IN FUNCTION FUN_bfe014d0
bfe01580:	08 00 e0 03	jr ra
bfe01584:	00 00 00 00	_nop

### FUNCTION FUN_bfe01588 @ bfe01588
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01588:	e8 ff bd 27	addiu sp,sp,-0x18
bfe0158c:	10 00 b0 af	sw s0,0x10(sp)
bfe01590:	80 b8 10 3c	lui s0,0xb880
bfe01594:	84 00 02 8e	lw v0,0x84(s0)
bfe01598:	00 20 03 3c	lui v1,0x2000
bfe0159c:	14 00 bf af	sw ra,0x14(sp)
bfe015a0:	25 10 43 00	or v0,v0,v1
bfe015a4:	84 00 02 ae	sw v0,0x84(s0)
bfe015a8:	e9 04 f8 0f	jal 0xbfe013a4
bfe015ac:	84 03 04 24	_li a0,0x384
bfe015b0:	84 00 02 8e	lw v0,0x84(s0)
bfe015b4:	14 00 bf 8f	lw ra,0x14(sp)
bfe015b8:	ff df 03 3c	lui v1,0xdfff
bfe015bc:	ff ff 63 34	ori v1,v1,0xffff
bfe015c0:	24 10 43 00	and v0,v0,v1
bfe015c4:	84 00 02 ae	sw v0,0x84(s0)
bfe015c8:	10 00 b0 8f	lw s0,0x10(sp)
bfe015cc:	08 00 e0 03	jr ra

### FUNCTION sub_bfe015d0 @ bfe015d0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe015d0:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION first_stage_peripheral_gate_for_usb_blocks @ bfe015d4
### NOTE Medium confidence: compares the argument with 0xb8844000 and 0xb8850000, matching the DTS USB host controller MMIO windows, then toggles bits in 0xb8800080.
bfe015d4:	84 b8 02 3c	lui v0,0xb884
bfe015d8:	00 40 42 24	addiu v0,v0,0x4000
bfe015dc:	14 00 82 14	bne a0,v0,0xbfe01630
bfe015e0:	85 b8 02 3c	_lui v0,0xb885

### FUNCTION sub_bfe015e4 @ bfe015e4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe015e4:	e8 ff bd 27	addiu sp,sp,-0x18
bfe015e8:	10 00 b0 af	sw s0,0x10(sp)
bfe015ec:	80 b8 10 3c	lui s0,0xb880
bfe015f0:	80 00 02 8e	lw v0,0x80(s0)
bfe015f4:	00 10 03 3c	lui v1,0x1000
bfe015f8:	14 00 bf af	sw ra,0x14(sp)
bfe015fc:	25 10 43 00	or v0,v0,v1
bfe01600:	80 00 02 ae	sw v0,0x80(s0)
bfe01604:	e9 04 f8 0f	jal 0xbfe013a4
bfe01608:	84 03 04 24	_li a0,0x384
bfe0160c:	80 00 02 8e	lw v0,0x80(s0)
bfe01610:	14 00 bf 8f	lw ra,0x14(sp)
bfe01614:	ff ef 03 3c	lui v1,0xefff
bfe01618:	ff ff 63 34	ori v1,v1,0xffff
bfe0161c:	24 10 43 00	and v0,v0,v1
bfe01620:	80 00 02 ae	sw v0,0x80(s0)
bfe01624:	10 00 b0 8f	lw s0,0x10(sp)
bfe01628:	08 00 e0 03	jr ra

### FUNCTION sub_bfe0162c @ bfe0162c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0162c:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION sub_bfe01630 @ bfe01630
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01630:	03 00 82 14	bne a0,v0,0xbfe01640
bfe01634:	00 00 00 00	_nop
bfe01638:	62 05 f8 0b	j 0xbfe01588
bfe0163c:	00 00 00 00	_nop
bfe01640:	08 00 e0 03	jr ra
bfe01644:	00 00 00 00	_nop

### FUNCTION first_stage_peripheral_reset_sequence @ bfe01648
### NOTE Medium confidence: writes several control/status registers in the selected MMIO window after gating the peripheral clock/reset.
bfe01648:	20 10 82 94	lhu v0,0x1020(a0)
bfe0164c:	80 03 83 8c	lw v1,0x380(a0)

### FUNCTION sub_bfe01650 @ bfe01650
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01650:	e8 ff bd 27	addiu sp,sp,-0x18
bfe01654:	c0 10 42 34	ori v0,v0,0x10c0
bfe01658:	10 00 b0 af	sw s0,0x10(sp)
bfe0165c:	14 00 bf af	sw ra,0x14(sp)
bfe01660:	20 10 82 a4	sh v0,0x1020(a0)
bfe01664:	6f ff 02 24	li v0,-0x91
bfe01668:	24 10 43 00	and v0,v0,v1
bfe0166c:	11 00 42 34	ori v0,v0,0x11
bfe01670:	25 80 80 00	or s0,a0,zero
bfe01674:	80 03 82 ac	sw v0,0x380(a0)
bfe01678:	e9 04 f8 0f	jal 0xbfe013a4
bfe0167c:	84 03 04 24	_li a0,0x384
bfe01680:	80 03 02 8e	lw v0,0x380(s0)
bfe01684:	14 00 bf 8f	lw ra,0x14(sp)
bfe01688:	fe ff 03 24	li v1,-0x2
bfe0168c:	24 10 43 00	and v0,v0,v1
bfe01690:	80 03 02 ae	sw v0,0x380(s0)
bfe01694:	10 00 b0 8f	lw s0,0x10(sp)
bfe01698:	08 00 e0 03	jr ra

### FUNCTION sub_bfe0169c @ bfe0169c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0169c:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION FUN_bfe016a0 @ bfe016a0
### NOTE References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
bfe016a0:	81 b8 02 3c	lui v0,0xb881
bfe016a4:	00 8a 42 34	ori v0,v0,0x8a00
bfe016a8:	00 00 42 8c	lw v0,0x0(v0)
bfe016ac:	00 40 03 3c	lui v1,0x4000
bfe016b0:	24 10 43 00	and v0,v0,v1
bfe016b4:	07 00 40 14	bne v0,zero,0xbfe016d4
bfe016b8:	81 b8 02 3c	_lui v0,0xb881
bfe016bc:	04 00 83 8c	lw v1,0x4(a0)
bfe016c0:	84 b8 02 3c	lui v0,0xb884
bfe016c4:	00 40 42 24	addiu v0,v0,0x4000
bfe016c8:	3b 00 62 10	beq v1,v0,0xbfe017b8
bfe016cc:	00 00 00 00	_nop
bfe016d0:	81 b8 02 3c	lui v0,0xb881
bfe016d4:	00 8a 42 34	ori v0,v0,0x8a00
bfe016d8:	00 00 42 8c	lw v0,0x0(v0)
bfe016dc:	04 00 40 04	bltz v0,0xbfe016f0
bfe016e0:	85 b8 02 3c	_lui v0,0xb885
bfe016e4:	04 00 83 8c	lw v1,0x4(a0)
bfe016e8:	33 00 62 10	beq v1,v0,0xbfe017b8
bfe016ec:	00 00 00 00	_nop

### FUNCTION sub_bfe016f0 @ bfe016f0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe016f0:	e8 ff bd 27	addiu sp,sp,-0x18
bfe016f4:	01 00 02 24	li v0,0x1
bfe016f8:	14 00 bf af	sw ra,0x14(sp)
bfe016fc:	10 00 b0 af	sw s0,0x10(sp)
bfe01700:	20 00 82 ac	sw v0,0x20(a0)
bfe01704:	40 00 02 24	li v0,0x40
bfe01708:	24 00 82 ac	sw v0,0x24(a0)
bfe0170c:	ff 00 02 24	li v0,0xff
bfe01710:	2c 00 82 ac	sw v0,0x2c(a0)
bfe01714:	00 00 82 8c	lw v0,0x0(a0)
bfe01718:	0c 00 80 ac	sw zero,0xc(a0)
bfe0171c:	10 00 80 ac	sw zero,0x10(a0)
bfe01720:	14 00 80 ac	sw zero,0x14(a0)
bfe01724:	30 00 80 ac	sw zero,0x30(a0)
bfe01728:	38 00 80 ac	sw zero,0x38(a0)
bfe0172c:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01730:	1c 00 80 ac	sw zero,0x1c(a0)
bfe01734:	28 00 80 ac	sw zero,0x28(a0)
bfe01738:	1c 00 40 14	bne v0,zero,0xbfe017ac
bfe0173c:	80 b8 05 3c	_lui a1,0xb880
bfe01740:	64 00 a2 8c	lw v0,0x64(a1)
bfe01744:	ff fe 03 3c	lui v1,0xfeff
bfe01748:	ff ff 63 34	ori v1,v1,0xffff
bfe0174c:	24 10 43 00	and v0,v0,v1
bfe01750:	64 00 a2 ac	sw v0,0x64(a1)
bfe01754:	25 80 80 00	or s0,a0,zero
bfe01758:	92 05 f8 0f	jal 0xbfe01648
bfe0175c:	04 00 84 8c	_lw a0,0x4(a0)
bfe01760:	75 05 f8 0f	jal 0xbfe015d4
bfe01764:	04 00 04 8e	_lw a0,0x4(s0)
bfe01768:	92 05 f8 0f	jal 0xbfe01648
bfe0176c:	04 00 04 8e	_lw a0,0x4(s0)
bfe01770:	04 00 03 8e	lw v1,0x4(s0)
bfe01774:	e8 03 04 24	li a0,0x3e8
bfe01778:	01 00 62 90	lbu v0,0x1(v1)
bfe0177c:	bf 00 42 30	andi v0,v0,0xbf
bfe01780:	01 00 62 a0	sb v0,0x1(v1)
bfe01784:	e9 04 f8 0f	jal 0xbfe013a4
bfe01788:	00 00 00 00	_nop
bfe0178c:	04 00 03 8e	lw v1,0x4(s0)
bfe01790:	01 00 62 90	lbu v0,0x1(v1)
bfe01794:	40 00 42 34	ori v0,v0,0x40
bfe01798:	01 00 62 a0	sb v0,0x1(v1)
bfe0179c:	14 00 bf 8f	lw ra,0x14(sp)
bfe017a0:	10 00 b0 8f	lw s0,0x10(sp)
bfe017a4:	08 00 e0 03	jr ra

### FUNCTION sub_bfe017a8 @ bfe017a8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe017a8:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION sub_bfe017ac @ bfe017ac
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe017ac:	64 00 a2 8c	lw v0,0x64(a1)
bfe017b0:	e5 ff 00 10	b 0xbfe01748
bfe017b4:	ff fd 03 3c	_lui v1,0xfdff

### IN FUNCTION FUN_bfe016a0
bfe017b8:	08 00 e0 03	jr ra
bfe017bc:	00 00 00 00	_nop

### FUNCTION FUN_bfe017c0 @ bfe017c0
### NOTE References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
bfe017c0:	84 b8 02 3c	lui v0,0xb884

### FUNCTION sub_bfe017c4 @ bfe017c4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe017c4:	48 fe bd 27	addiu sp,sp,-0x1b8
bfe017c8:	00 40 42 24	addiu v0,v0,0x4000
bfe017cc:	14 00 a2 af	sw v0,0x14(sp)
bfe017d0:	85 b8 02 3c	lui v0,0xb885
bfe017d4:	e4 00 a2 af	sw v0,0xe4(sp)
bfe017d8:	10 00 a4 27	addiu a0,sp,0x10
bfe017dc:	01 00 02 24	li v0,0x1
bfe017e0:	b4 01 bf af	sw ra,0x1b4(sp)
bfe017e4:	e0 00 a2 af	sw v0,0xe0(sp)
bfe017e8:	a8 05 f8 0f	jal 0xbfe016a0
bfe017ec:	10 00 a0 af	_sw zero,0x10(sp)
bfe017f0:	a8 05 f8 0f	jal 0xbfe016a0
bfe017f4:	e0 00 a4 27	_addiu a0,sp,0xe0
bfe017f8:	b4 01 bf 8f	lw ra,0x1b4(sp)
bfe017fc:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01800 @ bfe01800
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01800:	b8 01 bd 27	_addiu sp,sp,0x1b8

### FUNCTION FUN_bfe01804 @ bfe01804
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01804:	81 b8 02 3c	lui v0,0xb881
bfe01808:	00 8a 42 34	ori v0,v0,0x8a00
bfe0180c:	00 00 42 8c	lw v0,0x0(v0)
bfe01810:	00 40 03 3c	lui v1,0x4000

### FUNCTION sub_bfe01814 @ bfe01814
### NOTE References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
bfe01814:	e8 ff bd 27	addiu sp,sp,-0x18
bfe01818:	24 10 43 00	and v0,v0,v1
bfe0181c:	10 00 b0 af	sw s0,0x10(sp)
bfe01820:	14 00 bf af	sw ra,0x14(sp)
bfe01824:	25 80 80 00	or s0,a0,zero
bfe01828:	05 00 40 14	bne v0,zero,0xbfe01840
bfe0182c:	04 00 84 8c	_lw a0,0x4(a0)
bfe01830:	84 b8 02 3c	lui v0,0xb884
bfe01834:	00 40 42 24	addiu v0,v0,0x4000
bfe01838:	20 00 82 10	beq a0,v0,0xbfe018bc
bfe0183c:	14 00 bf 8f	_lw ra,0x14(sp)
bfe01840:	81 b8 02 3c	lui v0,0xb881
bfe01844:	00 8a 42 34	ori v0,v0,0x8a00
bfe01848:	00 00 42 8c	lw v0,0x0(v0)
bfe0184c:	03 00 40 04	bltz v0,0xbfe0185c
bfe01850:	85 b8 02 3c	_lui v0,0xb885
bfe01854:	19 00 82 10	beq a0,v0,0xbfe018bc
bfe01858:	14 00 bf 8f	_lw ra,0x14(sp)
bfe0185c:	01 00 02 24	li v0,0x1
bfe01860:	20 00 02 ae	sw v0,0x20(s0)
bfe01864:	40 00 02 24	li v0,0x40
bfe01868:	24 00 02 ae	sw v0,0x24(s0)
bfe0186c:	ff 00 02 24	li v0,0xff
bfe01870:	2c 00 02 ae	sw v0,0x2c(s0)
bfe01874:	0c 00 00 ae	sw zero,0xc(s0)
bfe01878:	10 00 00 ae	sw zero,0x10(s0)
bfe0187c:	14 00 00 ae	sw zero,0x14(s0)
bfe01880:	30 00 00 ae	sw zero,0x30(s0)
bfe01884:	38 00 00 ae	sw zero,0x38(s0)
bfe01888:	3c 00 00 ae	sw zero,0x3c(s0)
bfe0188c:	1c 00 00 ae	sw zero,0x1c(s0)
bfe01890:	75 05 f8 0f	jal 0xbfe015d4
bfe01894:	28 00 00 ae	_sw zero,0x28(s0)
bfe01898:	25 20 00 02	or a0,s0,zero
bfe0189c:	e9 0a f8 0f	jal 0xbfe02ba4
bfe018a0:	25 28 00 00	_or a1,zero,zero
bfe018a4:	14 00 bf 8f	lw ra,0x14(sp)
bfe018a8:	25 20 00 02	or a0,s0,zero
bfe018ac:	10 00 b0 8f	lw s0,0x10(sp)
bfe018b0:	01 00 05 24	li a1,0x1
bfe018b4:	e9 0a f8 0b	j 0xbfe02ba4

### FUNCTION sub_bfe018b8 @ bfe018b8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe018b8:	18 00 bd 27	_addiu sp,sp,0x18
bfe018bc:	10 00 b0 8f	lw s0,0x10(sp)
bfe018c0:	08 00 e0 03	jr ra

### FUNCTION sub_bfe018c4 @ bfe018c4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe018c4:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION FUN_bfe018c8 @ bfe018c8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe018c8:	b8 00 89 8c	lw t1,0xb8(a0)

### FUNCTION sub_bfe018cc @ bfe018cc
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe018cc:	e8 ff bd 27	addiu sp,sp,-0x18
bfe018d0:	01 00 02 24	li v0,0x1
bfe018d4:	14 00 bf af	sw ra,0x14(sp)
bfe018d8:	38 00 82 ac	sw v0,0x38(a0)
bfe018dc:	41 00 22 2d	sltiu v0,t1,0x41
bfe018e0:	25 40 80 00	or t0,a0,zero
bfe018e4:	02 00 40 10	beq v0,zero,0xbfe018f0
bfe018e8:	40 00 07 24	_li a3,0x40
bfe018ec:	25 38 20 01	or a3,t1,zero
bfe018f0:	b8 00 02 8d	lw v0,0xb8(t0)
bfe018f4:	b4 00 06 8d	lw a2,0xb4(t0)
bfe018f8:	04 00 04 8d	lw a0,0x4(t0)
bfe018fc:	23 10 47 00	subu v0,v0,a3
bfe01900:	b8 00 02 ad	sw v0,0xb8(t0)
bfe01904:	21 10 c7 00	addu v0,a2,a3
bfe01908:	b4 00 02 ad	sw v0,0xb4(t0)
bfe0190c:	34 05 f8 0f	jal 0xbfe014d0
bfe01910:	25 28 00 00	_or a1,zero,zero
bfe01914:	40 00 29 2d	sltiu t1,t1,0x40
bfe01918:	09 00 20 15	bne t1,zero,0xbfe01940
bfe0191c:	04 00 02 8d	_lw v0,0x4(t0)
bfe01920:	02 01 43 90	lbu v1,0x102(v0)
bfe01924:	02 00 63 30	andi v1,v1,0x2
bfe01928:	02 00 60 14	bne v1,zero,0xbfe01934
bfe0192c:	02 00 03 24	_li v1,0x2
bfe01930:	02 01 43 a0	sb v1,0x102(v0)
bfe01934:	14 00 bf 8f	lw ra,0x14(sp)
bfe01938:	08 00 e0 03	jr ra

### FUNCTION sub_bfe0193c @ bfe0193c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0193c:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION sub_bfe01940 @ bfe01940
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01940:	04 00 03 24	li v1,0x4
bfe01944:	38 00 03 ad	sw v1,0x38(t0)
bfe01948:	02 01 43 90	lbu v1,0x102(v0)
bfe0194c:	02 00 63 30	andi v1,v1,0x2
bfe01950:	f8 ff 60 14	bne v1,zero,0xbfe01934
bfe01954:	0a 00 03 24	_li v1,0xa
bfe01958:	f5 ff 00 10	b 0xbfe01930
bfe0195c:	00 00 00 00	_nop

### FUNCTION FUN_bfe01960 @ bfe01960
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01960:	38 00 83 8c	lw v1,0x38(a0)
bfe01964:	15 00 60 14	bne v1,zero,0xbfe019bc
bfe01968:	01 00 05 24	_li a1,0x1
bfe0196c:	30 00 83 8c	lw v1,0x30(a0)
bfe01970:	12 00 65 10	beq v1,a1,0xbfe019bc
bfe01974:	00 00 00 00	_nop
bfe01978:	20 00 83 8c	lw v1,0x20(a0)
bfe0197c:	08 00 65 14	bne v1,a1,0xbfe019a0
bfe01980:	00 00 00 00	_nop
bfe01984:	38 00 83 ac	sw v1,0x38(a0)
bfe01988:	10 00 83 8c	lw v1,0x10(a0)
bfe0198c:	b4 00 83 ac	sw v1,0xb4(a0)
bfe01990:	14 00 83 8c	lw v1,0x14(a0)
bfe01994:	b8 00 83 ac	sw v1,0xb8(a0)
bfe01998:	32 06 f8 0b	j 0xbfe018c8
bfe0199c:	00 00 00 00	_nop
bfe019a0:	06 00 60 14	bne v1,zero,0xbfe019bc
bfe019a4:	03 00 03 24	_li v1,0x3
bfe019a8:	38 00 83 ac	sw v1,0x38(a0)
bfe019ac:	10 00 83 8c	lw v1,0x10(a0)
bfe019b0:	b4 00 83 ac	sw v1,0xb4(a0)
bfe019b4:	14 00 83 8c	lw v1,0x14(a0)
bfe019b8:	b8 00 83 ac	sw v1,0xb8(a0)
bfe019bc:	08 00 e0 03	jr ra
bfe019c0:	00 00 00 00	_nop

### FUNCTION sub_bfe019c4 @ bfe019c4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe019c4:	14 00 a3 8c	lw v1,0x14(a1)
bfe019c8:	01 00 05 24	li a1,0x1
bfe019cc:	10 00 65 14	bne v1,a1,0xbfe01a10
bfe019d0:	00 00 00 00	_nop
bfe019d4:	30 00 85 8c	lw a1,0x30(a0)
bfe019d8:	0d 00 a3 10	beq a1,v1,0xbfe01a10
bfe019dc:	00 00 00 00	_nop
bfe019e0:	b0 00 85 8c	lw a1,0xb0(a0)
bfe019e4:	30 00 83 ac	sw v1,0x30(a0)
bfe019e8:	e0 bf 03 3c	lui v1,0xbfe0
bfe019ec:	c4 19 63 24	addiu v1,v1,0x19c4
bfe019f0:	0c 00 85 ac	sw a1,0xc(a0)
bfe019f4:	10 00 80 ac	sw zero,0x10(a0)
bfe019f8:	14 00 80 ac	sw zero,0x14(a0)
bfe019fc:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01a00:	1c 00 80 ac	sw zero,0x1c(a0)
bfe01a04:	20 00 80 ac	sw zero,0x20(a0)
bfe01a08:	58 06 f8 0b	j 0xbfe01960
bfe01a0c:	34 00 83 ac	_sw v1,0x34(a0)
bfe01a10:	08 00 e0 03	jr ra
bfe01a14:	00 00 00 00	_nop

### FUNCTION FUN_bfe01a18 @ bfe01a18
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01a18:	e0 bf 02 3c	lui v0,0xbfe0
bfe01a1c:	00 00 42 24	addiu v0,v0,0x0
bfe01a20:	70 02 43 90	lbu v1,0x270(v0)
bfe01a24:	60 00 63 30	andi v1,v1,0x60
bfe01a28:	97 00 60 14	bne v1,zero,0xbfe01c88
bfe01a2c:	00 00 00 00	_nop

### FUNCTION sub_bfe01a30 @ bfe01a30
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01a30:	e8 ff bd 27	addiu sp,sp,-0x18
bfe01a34:	14 00 bf af	sw ra,0x14(sp)
bfe01a38:	71 02 43 90	lbu v1,0x271(v0)
bfe01a3c:	05 00 05 24	li a1,0x5
bfe01a40:	59 00 65 10	beq v1,a1,0xbfe01ba8
bfe01a44:	25 50 80 00	_or t2,a0,zero
bfe01a48:	06 00 65 2c	sltiu a1,v1,0x6
bfe01a4c:	06 00 a0 10	beq a1,zero,0xbfe01a68
bfe01a50:	00 00 00 00	_nop
bfe01a54:	42 00 60 10	beq v1,zero,0xbfe01b60
bfe01a58:	00 00 00 00	_nop
bfe01a5c:	14 00 bf 8f	lw ra,0x14(sp)
bfe01a60:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01a64 @ bfe01a64
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01a64:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION sub_bfe01a68 @ bfe01a68
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01a68:	06 00 05 24	li a1,0x6
bfe01a6c:	69 00 65 10	beq v1,a1,0xbfe01c14
bfe01a70:	00 00 00 00	_nop
bfe01a74:	09 00 02 24	li v0,0x9
bfe01a78:	f8 ff 62 14	bne v1,v0,0xbfe01a5c
bfe01a7c:	01 00 0b 24	_li t3,0x1
bfe01a80:	b0 00 82 8c	lw v0,0xb0(a0)
bfe01a84:	48 00 03 24	li v1,0x48
bfe01a88:	0c 00 82 ac	sw v0,0xc(a0)
bfe01a8c:	04 00 82 8c	lw v0,0x4(a0)
bfe01a90:	10 00 80 ac	sw zero,0x10(a0)
bfe01a94:	14 00 80 ac	sw zero,0x14(a0)
bfe01a98:	30 00 8b ac	sw t3,0x30(a0)
bfe01a9c:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01aa0:	1c 00 80 ac	sw zero,0x1c(a0)
bfe01aa4:	20 00 80 ac	sw zero,0x20(a0)
bfe01aa8:	02 01 43 a0	sb v1,0x102(v0)
bfe01aac:	04 00 82 8c	lw v0,0x4(a0)
bfe01ab0:	00 02 03 24	li v1,0x200
bfe01ab4:	40 00 06 24	li a2,0x40
bfe01ab8:	10 01 43 a4	sh v1,0x110(v0)
bfe01abc:	13 01 40 a0	sb zero,0x113(v0)
bfe01ac0:	12 01 46 a0	sb a2,0x112(v0)
bfe01ac4:	04 00 82 8c	lw v0,0x4(a0)
bfe01ac8:	08 00 06 24	li a2,0x8
bfe01acc:	24 01 43 a4	sh v1,0x124(v0)
bfe01ad0:	80 ff 03 24	li v1,-0x80
bfe01ad4:	27 01 40 a0	sb zero,0x127(v0)
bfe01ad8:	26 01 43 a0	sb v1,0x126(v0)
bfe01adc:	04 00 82 8c	lw v0,0x4(a0)
bfe01ae0:	0e 00 43 90	lbu v1,0xe(v0)
bfe01ae4:	0e 00 4b a0	sb t3,0xe(v0)
bfe01ae8:	62 00 45 a0	sb a1,0x62(v0)
bfe01aec:	ff 00 63 30	andi v1,v1,0xff
bfe01af0:	0e 00 43 a0	sb v1,0xe(v0)
bfe01af4:	0e 00 43 90	lbu v1,0xe(v0)
bfe01af8:	0e 00 4b a0	sb t3,0xe(v0)
bfe01afc:	64 00 46 a4	sh a2,0x64(v0)
bfe01b00:	ff 00 63 30	andi v1,v1,0xff
bfe01b04:	0e 00 43 a0	sb v1,0xe(v0)
bfe01b08:	04 00 82 8c	lw v0,0x4(a0)
bfe01b0c:	02 00 06 24	li a2,0x2
bfe01b10:	0e 00 43 90	lbu v1,0xe(v0)
bfe01b14:	0e 00 46 a0	sb a2,0xe(v0)
bfe01b18:	63 00 45 a0	sb a1,0x63(v0)
bfe01b1c:	ff 00 63 30	andi v1,v1,0xff
bfe01b20:	0e 00 43 a0	sb v1,0xe(v0)
bfe01b24:	0e 00 43 90	lbu v1,0xe(v0)
bfe01b28:	48 00 05 24	li a1,0x48
bfe01b2c:	0e 00 46 a0	sb a2,0xe(v0)
bfe01b30:	ff 00 63 30	andi v1,v1,0xff
bfe01b34:	66 00 45 a4	sh a1,0x66(v0)
bfe01b38:	0e 00 43 a0	sb v1,0xe(v0)
bfe01b3c:	58 06 f8 0f	jal 0xbfe01960
bfe01b40:	00 00 00 00	_nop
bfe01b44:	00 30 02 3c	lui v0,0x3000
bfe01b48:	01 00 42 24	addiu v0,v0,0x1
bfe01b4c:	a0 00 42 ad	sw v0,0xa0(t2)
bfe01b50:	02 00 02 24	li v0,0x2
bfe01b54:	90 00 42 ad	sw v0,0x90(t2)
bfe01b58:	c0 ff 00 10	b 0xbfe01a5c
bfe01b5c:	5c 00 4b ad	_sw t3,0x5c(t2)

### NO FUNCTION
bfe01b60:	b0 00 82 8c	lw v0,0xb0(a0)
bfe01b64:	bc 00 80 a4	sh zero,0xbc(a0)
bfe01b68:	30 00 80 ac	sw zero,0x30(a0)
bfe01b6c:	0c 00 82 ac	sw v0,0xc(a0)
bfe01b70:	bc 00 82 24	addiu v0,a0,0xbc
bfe01b74:	10 00 82 ac	sw v0,0x10(a0)
bfe01b78:	02 00 02 24	li v0,0x2
bfe01b7c:	14 00 82 ac	sw v0,0x14(a0)
bfe01b80:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01b84:	1c 00 80 ac	sw zero,0x1c(a0)
bfe01b88:	01 00 02 24	li v0,0x1

### IN FUNCTION sub_bfe01a68
bfe01b8c:	20 00 42 ad	sw v0,0x20(t2)
bfe01b90:	e0 bf 02 3c	lui v0,0xbfe0
bfe01b94:	c4 19 42 24	addiu v0,v0,0x19c4
bfe01b98:	34 00 42 ad	sw v0,0x34(t2)
bfe01b9c:	14 00 bf 8f	lw ra,0x14(sp)
bfe01ba0:	58 06 f8 0b	j 0xbfe01960

### FUNCTION sub_bfe01ba4 @ bfe01ba4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01ba4:	18 00 bd 27	_addiu sp,sp,0x18
bfe01ba8:	72 02 43 90	lbu v1,0x272(v0)
bfe01bac:	73 02 42 90	lbu v0,0x273(v0)
bfe01bb0:	10 00 80 ac	sw zero,0x10(a0)
bfe01bb4:	14 00 80 ac	sw zero,0x14(a0)
bfe01bb8:	00 12 02 00	sll v0,v0,0x8
bfe01bbc:	25 10 43 00	or v0,v0,v1
bfe01bc0:	01 00 03 24	li v1,0x1
bfe01bc4:	30 00 83 ac	sw v1,0x30(a0)
bfe01bc8:	20 00 83 ac	sw v1,0x20(a0)
bfe01bcc:	e0 bf 03 3c	lui v1,0xbfe0
bfe01bd0:	c4 19 63 24	addiu v1,v1,0x19c4
bfe01bd4:	34 00 83 ac	sw v1,0x34(a0)
bfe01bd8:	0c 00 82 ac	sw v0,0xc(a0)
bfe01bdc:	b0 00 82 ac	sw v0,0xb0(a0)
bfe01be0:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01be4:	58 06 f8 0f	jal 0xbfe01960
bfe01be8:	1c 00 80 ac	_sw zero,0x1c(a0)
bfe01bec:	b0 00 42 8d	lw v0,0xb0(t2)
bfe01bf0:	00 80 03 3c	lui v1,0x8000
bfe01bf4:	25 10 43 00	or v0,v0,v1
bfe01bf8:	08 00 42 ad	sw v0,0x8(t2)
bfe01bfc:	04 00 42 8d	lw v0,0x4(t2)
bfe01c00:	48 00 03 24	li v1,0x48
bfe01c04:	02 01 43 a0	sb v1,0x102(v0)
bfe01c08:	04 00 02 24	li v0,0x4
bfe01c0c:	93 ff 00 10	b 0xbfe01a5c
bfe01c10:	38 00 42 ad	_sw v0,0x38(t2)

### IN FUNCTION sub_bfe01a68
bfe01c14:	73 02 42 90	lbu v0,0x273(v0)
bfe01c18:	01 00 03 24	li v1,0x1
bfe01c1c:	0f 00 43 10	beq v0,v1,0xbfe01c5c
bfe01c20:	02 00 05 24	_li a1,0x2
bfe01c24:	8e ff 45 14	bne v0,a1,0xbfe01a60
bfe01c28:	14 00 bf 8f	_lw ra,0x14(sp)
bfe01c2c:	b0 00 82 8c	lw v0,0xb0(a0)
bfe01c30:	30 00 80 ac	sw zero,0x30(a0)
bfe01c34:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01c38:	0c 00 82 ac	sw v0,0xc(a0)
bfe01c3c:	e0 bf 02 3c	lui v0,0xbfe0
bfe01c40:	b0 02 42 24	addiu v0,v0,0x2b0
bfe01c44:	10 00 82 ac	sw v0,0x10(a0)
bfe01c48:	20 00 02 24	li v0,0x20
bfe01c4c:	14 00 82 ac	sw v0,0x14(a0)
bfe01c50:	1c 00 80 ac	sw zero,0x1c(a0)
bfe01c54:	d1 ff 00 10	b 0xbfe01b9c
bfe01c58:	20 00 83 ac	_sw v1,0x20(a0)
bfe01c5c:	b0 00 83 8c	lw v1,0xb0(a0)
bfe01c60:	30 00 80 ac	sw zero,0x30(a0)
bfe01c64:	3c 00 80 ac	sw zero,0x3c(a0)
bfe01c68:	0c 00 83 ac	sw v1,0xc(a0)
bfe01c6c:	e0 bf 03 3c	lui v1,0xbfe0
bfe01c70:	d0 02 63 24	addiu v1,v1,0x2d0
bfe01c74:	10 00 83 ac	sw v1,0x10(a0)
bfe01c78:	12 00 03 24	li v1,0x12
bfe01c7c:	14 00 83 ac	sw v1,0x14(a0)
bfe01c80:	c2 ff 00 10	b 0xbfe01b8c
bfe01c84:	1c 00 80 ac	_sw zero,0x1c(a0)

### IN FUNCTION FUN_bfe01a18
bfe01c88:	08 00 e0 03	jr ra
bfe01c8c:	00 00 00 00	_nop

### FUNCTION FUN_bfe01c90 @ bfe01c90
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01c90:	1b 00 02 24	li v0,0x1b
bfe01c94:	18 00 82 00	mult a0,v0
bfe01c98:	81 b8 03 3c	lui v1,0xb881
bfe01c9c:	04 85 63 34	ori v1,v1,0x8504
bfe01ca0:	26 00 02 24	li v0,0x26
bfe01ca4:	00 00 60 ac	sw zero,0x0(v1)
bfe01ca8:	12 20 00 00	mflo a0
bfe01cac:	c2 21 04 00	srl a0,a0,0x7
bfe01cb0:	23 20 04 00	subu a0,zero,a0
bfe01cb4:	fc ff 64 ac	sw a0,-0x4(v1)
bfe01cb8:	00 00 62 ac	sw v0,0x0(v1)
bfe01cbc:	08 00 e0 03	jr ra
bfe01cc0:	00 00 00 00	_nop

### FUNCTION FUN_bfe01cc4 @ bfe01cc4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01cc4:	81 b8 02 3c	lui v0,0xb881
bfe01cc8:	04 85 42 34	ori v0,v0,0x8504
bfe01ccc:	00 00 40 ac	sw zero,0x0(v0)
bfe01cd0:	08 00 e0 03	jr ra
bfe01cd4:	00 00 00 00	_nop

### FUNCTION FUN_bfe01cd8 @ bfe01cd8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01cd8:	b0 00 83 8c	lw v1,0xb0(a0)

### FUNCTION sub_bfe01cdc @ bfe01cdc
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01cdc:	e8 ff bd 27	addiu sp,sp,-0x18
bfe01ce0:	14 00 bf af	sw ra,0x14(sp)
bfe01ce4:	40 00 83 ac	sw v1,0x40(a0)
bfe01ce8:	01 00 03 24	li v1,0x1
bfe01cec:	28 00 a2 8f	lw v0,0x28(sp)
bfe01cf0:	6c 00 83 ac	sw v1,0x6c(a0)
bfe01cf4:	2c 00 a3 8f	lw v1,0x2c(sp)
bfe01cf8:	25 40 80 00	or t0,a0,zero
bfe01cfc:	54 00 87 ac	sw a3,0x54(a0)
bfe01d00:	50 00 83 ac	sw v1,0x50(a0)
bfe01d04:	5c 00 85 ac	sw a1,0x5c(a0)
bfe01d08:	44 00 86 ac	sw a2,0x44(a0)
bfe01d0c:	48 00 82 ac	sw v0,0x48(a0)
bfe01d10:	64 00 80 ac	sw zero,0x64(a0)
bfe01d14:	70 00 80 ac	sw zero,0x70(a0)
bfe01d18:	04 00 84 8c	lw a0,0x4(a0)
bfe01d1c:	00 49 05 00	sll t1,a1,0x4
bfe01d20:	25 28 20 01	or a1,t1,zero
bfe01d24:	34 05 f8 0f	jal 0xbfe014d0
bfe01d28:	25 38 40 00	_or a3,v0,zero
bfe01d2c:	04 00 05 8d	lw a1,0x4(t0)
bfe01d30:	01 00 22 2d	sltiu v0,t1,0x1
bfe01d34:	02 01 29 25	addiu t1,t1,0x102
bfe01d38:	21 48 25 01	addu t1,t1,a1
bfe01d3c:	00 00 23 91	lbu v1,0x0(t1)
bfe01d40:	01 00 42 24	addiu v0,v0,0x1
bfe01d44:	24 18 43 00	and v1,v0,v1
bfe01d48:	02 00 60 14	bne v1,zero,0xbfe01d54
bfe01d4c:	ff 00 42 30	_andi v0,v0,0xff
bfe01d50:	00 00 22 a1	sb v0,0x0(t1)
bfe01d54:	14 00 bf 8f	lw ra,0x14(sp)
bfe01d58:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01d5c @ bfe01d5c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01d5c:	18 00 bd 27	_addiu sp,sp,0x18

### FUNCTION FUN_bfe01d60 @ bfe01d60
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01d60:	a8 00 82 8c	lw v0,0xa8(a0)
bfe01d64:	04 00 43 30	andi v1,v0,0x4
bfe01d68:	05 00 60 14	bne v1,zero,0xbfe01d80
bfe01d6c:	00 00 00 00	_nop
bfe01d70:	20 00 42 30	andi v0,v0,0x20
bfe01d74:	04 00 40 10	beq v0,zero,0xbfe01d88
bfe01d78:	00 00 00 00	_nop
bfe01d7c:	38 00 80 ac	sw zero,0x38(a0)
bfe01d80:	08 00 e0 03	jr ra
bfe01d84:	25 10 00 00	_or v0,zero,zero

### FUNCTION FUN_bfe01d88 @ bfe01d88
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01d88:	ac 00 82 8c	lw v0,0xac(a0)

### FUNCTION sub_bfe01d8c @ bfe01d8c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01d8c:	d0 ff bd 27	addiu sp,sp,-0x30
bfe01d90:	24 00 b0 af	sw s0,0x24(sp)
bfe01d94:	01 00 42 30	andi v0,v0,0x1
bfe01d98:	2c 00 bf af	sw ra,0x2c(sp)
bfe01d9c:	28 00 b1 af	sw s1,0x28(sp)
bfe01da0:	1f 00 40 10	beq v0,zero,0xbfe01e20
bfe01da4:	25 80 80 00	_or s0,a0,zero
bfe01da8:	04 00 84 8c	lw a0,0x4(a0)
bfe01dac:	02 01 82 94	lhu v0,0x102(a0)
bfe01db0:	06 01 91 94	lhu s1,0x106(a0)
bfe01db4:	00 8c 11 00	sll s1,s1,0x10
bfe01db8:	25 88 22 02	or s1,s1,v0
bfe01dbc:	38 00 02 8e	lw v0,0x38(s0)
bfe01dc0:	06 00 43 2c	sltiu v1,v0,0x6
bfe01dc4:	16 00 60 10	beq v1,zero,0xbfe01e20
bfe01dc8:	80 18 02 00	_sll v1,v0,0x2
bfe01dcc:	e0 bf 02 3c	lui v0,0xbfe0
bfe01dd0:	70 2e 42 24	addiu v0,v0,0x2e70
bfe01dd4:	21 10 43 00	addu v0,v0,v1
bfe01dd8:	00 00 42 8c	lw v0,0x0(v0)
bfe01ddc:	08 00 40 00	jr v0
bfe01de0:	00 00 00 00	_nop
bfe01de4:	01 00 31 32	andi s1,s1,0x1
bfe01de8:	0d 00 20 12	beq s1,zero,0xbfe01e20
bfe01dec:	40 00 02 24	_li v0,0x40
bfe01df0:	e0 bf 06 3c	lui a2,0xbfe0
bfe01df4:	18 00 a2 af	sw v0,0x18(sp)
bfe01df8:	18 00 a7 27	addiu a3,sp,0x18
bfe01dfc:	70 02 c6 24	addiu a2,a2,0x270
bfe01e00:	25 28 00 00	or a1,zero,zero
bfe01e04:	fc 04 f8 0f	jal 0xbfe013f0
bfe01e08:	00 00 00 00	_nop
bfe01e0c:	18 00 a2 8f	lw v0,0x18(sp)
bfe01e10:	03 00 40 10	beq v0,zero,0xbfe01e20
bfe01e14:	00 00 00 00	_nop
bfe01e18:	86 06 f8 0f	jal 0xbfe01a18
bfe01e1c:	25 20 00 02	_or a0,s0,zero
bfe01e20:	ac 00 03 8e	lw v1,0xac(s0)
bfe01e24:	fe ff 02 3c	lui v0,0xfffe
bfe01e28:	fe ff 42 34	ori v0,v0,0xfffe
bfe01e2c:	24 10 62 00	and v0,v1,v0
bfe01e30:	1d 00 40 10	beq v0,zero,0xbfe01ea8
bfe01e34:	fe ff 62 30	_andi v0,v1,0xfffe
bfe01e38:	0e 00 40 10	beq v0,zero,0xbfe01e74
bfe01e3c:	ac 00 02 ae	_sw v0,0xac(s0)
bfe01e40:	6c 00 04 8e	lw a0,0x6c(s0)
bfe01e44:	00 10 02 3c	lui v0,0x1000
bfe01e48:	04 00 45 24	addiu a1,v0,0x4
bfe01e4c:	72 00 85 10	beq a0,a1,0xbfe02018
bfe01e50:	05 00 45 24	_addiu a1,v0,0x5
bfe01e54:	2a 28 85 00	slt a1,a0,a1
bfe01e58:	60 00 a0 10	beq a1,zero,0xbfe01fdc
bfe01e5c:	01 00 45 24	_addiu a1,v0,0x1
bfe01e60:	68 00 85 10	beq a0,a1,0xbfe02004
bfe01e64:	00 00 00 00	_nop
bfe01e68:	02 00 42 24	addiu v0,v0,0x2
bfe01e6c:	68 00 82 10	beq a0,v0,0xbfe02010
bfe01e70:	00 20 02 3c	_lui v0,0x2000
bfe01e74:	fe ff 02 3c	lui v0,0xfffe
bfe01e78:	24 18 62 00	and v1,v1,v0
bfe01e7c:	0a 00 60 10	beq v1,zero,0xbfe01ea8
bfe01e80:	ac 00 03 ae	_sw v1,0xac(s0)
bfe01e84:	90 00 09 8e	lw t1,0x90(s0)
bfe01e88:	a0 00 03 8e	lw v1,0xa0(s0)
bfe01e8c:	00 30 02 3c	lui v0,0x3000
bfe01e90:	01 00 44 24	addiu a0,v0,0x1
bfe01e94:	65 00 64 10	beq v1,a0,0xbfe0202c
bfe01e98:	00 49 09 00	_sll t1,t1,0x4
bfe01e9c:	02 00 42 24	addiu v0,v0,0x2
bfe01ea0:	b8 00 62 10	beq v1,v0,0xbfe02184
bfe01ea4:	00 02 02 24	_li v0,0x200
bfe01ea8:	2c 00 bf 8f	lw ra,0x2c(sp)
bfe01eac:	28 00 b1 8f	lw s1,0x28(sp)
bfe01eb0:	24 00 b0 8f	lw s0,0x24(sp)
bfe01eb4:	25 10 00 00	or v0,zero,zero
bfe01eb8:	08 00 e0 03	jr ra

### FUNCTION sub_bfe01ebc @ bfe01ebc
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01ebc:	30 00 bd 27	_addiu sp,sp,0x30

### FUNCTION sub_bfe01ec0 @ bfe01ec0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe01ec0:	34 00 02 8e	lw v0,0x34(s0)
bfe01ec4:	0c 00 05 26	addiu a1,s0,0xc
bfe01ec8:	09 f8 40 00	jalr v0
bfe01ecc:	25 20 00 02	_or a0,s0,zero
bfe01ed0:	08 00 02 8e	lw v0,0x8(s0)
bfe01ed4:	08 00 41 04	bgez v0,0xbfe01ef8
bfe01ed8:	38 00 00 ae	_sw zero,0x38(s0)
bfe01edc:	ff 7f 03 3c	lui v1,0x7fff
bfe01ee0:	ff ff 63 34	ori v1,v1,0xffff
bfe01ee4:	24 10 43 00	and v0,v0,v1
bfe01ee8:	04 00 03 8e	lw v1,0x4(s0)
bfe01eec:	08 00 02 ae	sw v0,0x8(s0)
bfe01ef0:	ff 00 42 30	andi v0,v0,0xff
bfe01ef4:	00 00 62 a0	sb v0,0x0(v1)
bfe01ef8:	01 00 31 32	andi s1,s1,0x1
bfe01efc:	c8 ff 20 12	beq s1,zero,0xbfe01e20
bfe01f00:	40 00 02 24	_li v0,0x40
bfe01f04:	e0 bf 06 3c	lui a2,0xbfe0
bfe01f08:	18 00 a2 af	sw v0,0x18(sp)
bfe01f0c:	18 00 a7 27	addiu a3,sp,0x18
bfe01f10:	70 02 c6 24	addiu a2,a2,0x270
bfe01f14:	25 28 00 00	or a1,zero,zero
bfe01f18:	ba ff 00 10	b 0xbfe01e04
bfe01f1c:	04 00 04 8e	_lw a0,0x4(s0)

### NO FUNCTION
bfe01f20:	32 06 f8 0f	jal 0xbfe018c8
bfe01f24:	25 20 00 02	_or a0,s0,zero
bfe01f28:	be ff 00 10	b 0xbfe01e24
bfe01f2c:	ac 00 03 8e	_lw v1,0xac(s0)
bfe01f30:	b8 00 02 8e	lw v0,0xb8(s0)
bfe01f34:	41 00 42 2c	sltiu v0,v0,0x41
bfe01f38:	1c 00 40 14	bne v0,zero,0xbfe01fac
bfe01f3c:	00 00 00 00	_nop
bfe01f40:	40 00 02 24	li v0,0x40
bfe01f44:	b4 00 06 8e	lw a2,0xb4(s0)
bfe01f48:	25 28 00 00	or a1,zero,zero
bfe01f4c:	18 00 a7 27	addiu a3,sp,0x18
bfe01f50:	fc 04 f8 0f	jal 0xbfe013f0
bfe01f54:	18 00 a2 af	_sw v0,0x18(sp)
bfe01f58:	b8 00 02 8e	lw v0,0xb8(s0)
bfe01f5c:	0c 00 05 26	addiu a1,s0,0xc
bfe01f60:	25 20 00 02	or a0,s0,zero
bfe01f64:	41 00 42 2c	sltiu v0,v0,0x41
bfe01f68:	13 00 40 10	beq v0,zero,0xbfe01fb8
bfe01f6c:	34 00 03 8e	_lw v1,0x34(s0)
bfe01f70:	09 f8 60 00	jalr v1
bfe01f74:	00 00 00 00	_nop
bfe01f78:	04 00 02 8e	lw v0,0x4(s0)
bfe01f7c:	48 00 03 24	li v1,0x48
bfe01f80:	38 00 00 ae	sw zero,0x38(s0)
bfe01f84:	02 01 43 a0	sb v1,0x102(v0)
bfe01f88:	18 00 a4 8f	lw a0,0x18(sp)
bfe01f8c:	b4 00 03 8e	lw v1,0xb4(s0)
bfe01f90:	b8 00 02 8e	lw v0,0xb8(s0)
bfe01f94:	21 18 64 00	addu v1,v1,a0
bfe01f98:	23 10 44 00	subu v0,v0,a0
bfe01f9c:	b4 00 03 ae	sw v1,0xb4(s0)
bfe01fa0:	b8 00 02 ae	sw v0,0xb8(s0)
bfe01fa4:	9f ff 00 10	b 0xbfe01e24
bfe01fa8:	ac 00 03 8e	_lw v1,0xac(s0)
bfe01fac:	b8 00 02 8e	lw v0,0xb8(s0)
bfe01fb0:	e5 ff 00 10	b 0xbfe01f48
bfe01fb4:	b4 00 06 8e	_lw a2,0xb4(s0)
bfe01fb8:	09 f8 60 00	jalr v1
bfe01fbc:	00 00 00 00	_nop
bfe01fc0:	f2 ff 00 10	b 0xbfe01f8c
bfe01fc4:	18 00 a4 8f	_lw a0,0x18(sp)
bfe01fc8:	04 00 31 32	andi s1,s1,0x4
bfe01fcc:	94 ff 20 12	beq s1,zero,0xbfe01e20
bfe01fd0:	00 00 00 00	_nop
bfe01fd4:	92 ff 00 10	b 0xbfe01e20
bfe01fd8:	38 00 00 ae	_sw zero,0x38(s0)

### IN FUNCTION sub_bfe01ec0
bfe01fdc:	08 00 42 24	addiu v0,v0,0x8
bfe01fe0:	10 00 82 10	beq a0,v0,0xbfe02024
bfe01fe4:	00 20 02 3c	_lui v0,0x2000
bfe01fe8:	00 40 02 3c	lui v0,0x4000
bfe01fec:	01 00 42 24	addiu v0,v0,0x1
bfe01ff0:	a0 ff 82 14	bne a0,v0,0xbfe01e74
bfe01ff4:	00 30 02 3c	_lui v0,0x3000
bfe01ff8:	01 00 42 24	addiu v0,v0,0x1
bfe01ffc:	9d ff 00 10	b 0xbfe01e74
bfe02000:	a0 00 02 ae	_sw v0,0xa0(s0)
bfe02004:	00 20 02 3c	lui v0,0x2000
bfe02008:	fc ff 00 10	b 0xbfe01ffc
bfe0200c:	04 00 42 24	_addiu v0,v0,0x4
bfe02010:	fa ff 00 10	b 0xbfe01ffc
bfe02014:	10 00 42 24	_addiu v0,v0,0x10
bfe02018:	00 20 02 3c	lui v0,0x2000
bfe0201c:	f7 ff 00 10	b 0xbfe01ffc
bfe02020:	20 00 42 24	_addiu v0,v0,0x20
bfe02024:	f5 ff 00 10	b 0xbfe01ffc
bfe02028:	80 00 42 24	_addiu v0,v0,0x80
bfe0202c:	04 00 04 8e	lw a0,0x4(s0)
bfe02030:	c0 00 06 26	addiu a2,s0,0xc0
bfe02034:	00 02 02 24	li v0,0x200
bfe02038:	78 00 06 ae	sw a2,0x78(s0)
bfe0203c:	18 00 a7 27	addiu a3,sp,0x18
bfe02040:	25 28 20 01	or a1,t1,zero
bfe02044:	fc 04 f8 0f	jal 0xbfe013f0
bfe02048:	18 00 a2 af	_sw v0,0x18(sp)
bfe0204c:	35 00 20 15	bne t1,zero,0xbfe02124
bfe02050:	04 00 02 8e	_lw v0,0x4(s0)
bfe02054:	40 00 03 24	li v1,0x40
bfe02058:	02 01 43 a0	sb v1,0x102(v0)
bfe0205c:	c0 00 03 8e	lw v1,0xc0(s0)
bfe02060:	91 19 02 3c	lui v0,0x1991
bfe02064:	a1 a0 42 34	ori v0,v0,0xa0a1
bfe02068:	35 00 62 14	bne v1,v0,0xbfe02140
bfe0206c:	92 19 02 3c	_lui v0,0x1992
bfe02070:	c8 00 02 8e	lw v0,0xc8(s0)
bfe02074:	c4 00 03 8e	lw v1,0xc4(s0)
bfe02078:	b0 00 04 8e	lw a0,0xb0(s0)
bfe0207c:	7c 00 02 ae	sw v0,0x7c(s0)
bfe02080:	02 00 02 24	li v0,0x2
bfe02084:	84 00 02 ae	sw v0,0x84(s0)
bfe02088:	90 00 02 ae	sw v0,0x90(s0)
bfe0208c:	00 10 02 3c	lui v0,0x1000
bfe02090:	10 00 42 24	addiu v0,v0,0x10
bfe02094:	6c 00 02 ae	sw v0,0x6c(s0)
bfe02098:	00 30 02 3c	lui v0,0x3000
bfe0209c:	74 00 04 ae	sw a0,0x74(s0)
bfe020a0:	78 00 03 ae	sw v1,0x78(s0)
bfe020a4:	98 00 00 ae	sw zero,0x98(s0)
bfe020a8:	a4 00 00 ae	sw zero,0xa4(s0)
bfe020ac:	88 00 00 ae	sw zero,0x88(s0)
bfe020b0:	02 00 42 24	addiu v0,v0,0x2
bfe020b4:	a0 00 02 ae	sw v0,0xa0(s0)
bfe020b8:	c0 00 03 8e	lw v1,0xc0(s0)
bfe020bc:	93 19 02 3c	lui v0,0x1993
bfe020c0:	c1 c0 42 34	ori v0,v0,0xc0c1
bfe020c4:	78 ff 62 14	bne v1,v0,0xbfe01ea8
bfe020c8:	81 b8 02 3c	_lui v0,0xb881
bfe020cc:	c4 00 03 8e	lw v1,0xc4(s0)
bfe020d0:	00 8a 42 34	ori v0,v0,0x8a00
bfe020d4:	6a e6 04 24	li a0,-0x1996
bfe020d8:	00 00 44 a4	sh a0,0x0(v0)
bfe020dc:	09 f8 60 00	jalr v1
bfe020e0:	00 00 00 00	_nop
bfe020e4:	02 00 02 24	li v0,0x2
bfe020e8:	14 00 a2 af	sw v0,0x14(sp)
bfe020ec:	04 00 02 24	li v0,0x4
bfe020f0:	10 00 a2 af	sw v0,0x10(sp)
bfe020f4:	01 00 07 24	li a3,0x1
bfe020f8:	c4 00 06 26	addiu a2,s0,0xc4
bfe020fc:	5c 00 05 8e	lw a1,0x5c(s0)
bfe02100:	36 07 f8 0f	jal 0xbfe01cd8
bfe02104:	25 20 00 02	_or a0,s0,zero
bfe02108:	00 40 02 3c	lui v0,0x4000
bfe0210c:	01 00 42 24	addiu v0,v0,0x1
bfe02110:	6c 00 02 ae	sw v0,0x6c(s0)
bfe02114:	00 20 02 3c	lui v0,0x2000
bfe02118:	40 00 42 24	addiu v0,v0,0x40
bfe0211c:	62 ff 00 10	b 0xbfe01ea8
bfe02120:	a0 00 02 ae	_sw v0,0xa0(s0)
bfe02124:	06 01 42 24	addiu v0,v0,0x106
bfe02128:	21 48 49 00	addu t1,v0,t1
bfe0212c:	00 00 22 91	lbu v0,0x0(t1)
bfe02130:	fe 00 42 30	andi v0,v0,0xfe
bfe02134:	00 00 22 a1	sb v0,0x0(t1)
bfe02138:	c9 ff 00 10	b 0xbfe02060
bfe0213c:	c0 00 03 8e	_lw v1,0xc0(s0)
bfe02140:	b1 b0 42 34	ori v0,v0,0xb0b1
bfe02144:	dc ff 62 14	bne v1,v0,0xbfe020b8
bfe02148:	02 00 02 24	_li v0,0x2
bfe0214c:	14 00 a2 af	sw v0,0x14(sp)
bfe02150:	c8 00 02 8e	lw v0,0xc8(s0)
bfe02154:	01 00 07 24	li a3,0x1
bfe02158:	25 20 00 02	or a0,s0,zero
bfe0215c:	10 00 a2 af	sw v0,0x10(sp)
bfe02160:	c4 00 06 8e	lw a2,0xc4(s0)
bfe02164:	36 07 f8 0f	jal 0xbfe01cd8
bfe02168:	5c 00 05 8e	_lw a1,0x5c(s0)
bfe0216c:	00 40 02 3c	lui v0,0x4000
bfe02170:	01 00 42 24	addiu v0,v0,0x1
bfe02174:	6c 00 02 ae	sw v0,0x6c(s0)
bfe02178:	00 20 02 3c	lui v0,0x2000
bfe0217c:	cd ff 00 10	b 0xbfe020b4
bfe02180:	40 00 42 24	_addiu v0,v0,0x40
bfe02184:	78 00 06 8e	lw a2,0x78(s0)
bfe02188:	04 00 04 8e	lw a0,0x4(s0)
bfe0218c:	18 00 a7 27	addiu a3,sp,0x18
bfe02190:	25 28 20 01	or a1,t1,zero
bfe02194:	fc 04 f8 0f	jal 0xbfe013f0
bfe02198:	18 00 a2 af	_sw v0,0x18(sp)
bfe0219c:	1e 00 20 15	bne t1,zero,0xbfe02218
bfe021a0:	04 00 02 8e	_lw v0,0x4(s0)
bfe021a4:	40 00 03 24	li v1,0x40
bfe021a8:	02 01 43 a0	sb v1,0x102(v0)
bfe021ac:	18 00 a4 8f	lw a0,0x18(sp)
bfe021b0:	78 00 03 8e	lw v1,0x78(s0)
bfe021b4:	a4 00 02 8e	lw v0,0xa4(s0)
bfe021b8:	21 18 64 00	addu v1,v1,a0
bfe021bc:	78 00 03 ae	sw v1,0x78(s0)
bfe021c0:	7c 00 03 8e	lw v1,0x7c(s0)
bfe021c4:	21 10 82 00	addu v0,a0,v0
bfe021c8:	a4 00 02 ae	sw v0,0xa4(s0)
bfe021cc:	2b 10 43 00	sltu v0,v0,v1
bfe021d0:	1d 00 40 14	bne v0,zero,0xbfe02248
bfe021d4:	25 18 00 00	_or v1,zero,zero
bfe021d8:	c8 00 04 8e	lw a0,0xc8(s0)
bfe021dc:	c4 00 06 8e	lw a2,0xc4(s0)
bfe021e0:	82 20 04 00	srl a0,a0,0x2
bfe021e4:	13 00 83 14	bne a0,v1,0xbfe02234
bfe021e8:	80 28 03 00	_sll a1,v1,0x2
bfe021ec:	cc 00 03 8e	lw v1,0xcc(s0)
bfe021f0:	02 00 62 10	beq v1,v0,0xbfe021fc
bfe021f4:	00 00 00 00	_nop
bfe021f8:	cc 00 02 ae	sw v0,0xcc(s0)
bfe021fc:	02 00 02 24	li v0,0x2
bfe02200:	14 00 a2 af	sw v0,0x14(sp)
bfe02204:	04 00 02 24	li v0,0x4
bfe02208:	10 00 a2 af	sw v0,0x10(sp)
bfe0220c:	01 00 07 24	li a3,0x1
bfe02210:	ba ff 00 10	b 0xbfe020fc
bfe02214:	cc 00 06 26	_addiu a2,s0,0xcc
bfe02218:	06 01 42 24	addiu v0,v0,0x106
bfe0221c:	21 48 49 00	addu t1,v0,t1
bfe02220:	00 00 22 91	lbu v0,0x0(t1)
bfe02224:	fe 00 42 30	andi v0,v0,0xfe
bfe02228:	00 00 22 a1	sb v0,0x0(t1)
bfe0222c:	e0 ff 00 10	b 0xbfe021b0
bfe02230:	18 00 a4 8f	_lw a0,0x18(sp)
bfe02234:	21 28 a6 00	addu a1,a1,a2
bfe02238:	00 00 a5 8c	lw a1,0x0(a1)
bfe0223c:	01 00 63 24	addiu v1,v1,0x1
bfe02240:	e8 ff 00 10	b 0xbfe021e4
bfe02244:	26 10 45 00	_xor v0,v0,a1
bfe02248:	00 10 02 3c	lui v0,0x1000
bfe0224c:	10 00 42 24	addiu v0,v0,0x10
bfe02250:	15 ff 00 10	b 0xbfe01ea8
bfe02254:	6c 00 02 ae	_sw v0,0x6c(s0)

### FUNCTION FUN_bfe02258 @ bfe02258
### NOTE References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
bfe02258:	38 fe bd 27	addiu sp,sp,-0x1c8
bfe0225c:	84 b8 02 3c	lui v0,0xb884
bfe02260:	b0 01 b0 af	sw s0,0x1b0(sp)
bfe02264:	81 b8 10 3c	lui s0,0xb881
bfe02268:	04 85 10 36	ori s0,s0,0x8504
bfe0226c:	00 40 42 24	addiu v0,v0,0x4000
bfe02270:	bc 01 b3 af	sw s3,0x1bc(sp)
bfe02274:	b8 01 b2 af	sw s2,0x1b8(sp)
bfe02278:	b4 01 b1 af	sw s1,0x1b4(sp)
bfe0227c:	c4 01 bf af	sw ra,0x1c4(sp)
bfe02280:	c0 01 b4 af	sw s4,0x1c0(sp)
bfe02284:	00 00 00 ae	sw zero,0x0(s0)
bfe02288:	14 00 a2 af	sw v0,0x14(sp)
bfe0228c:	85 b8 02 3c	lui v0,0xb885
bfe02290:	e4 00 a2 af	sw v0,0xe4(sp)
bfe02294:	01 00 02 24	li v0,0x1
bfe02298:	10 00 a0 af	sw zero,0x10(sp)
bfe0229c:	e0 00 a2 af	sw v0,0xe0(sp)
bfe022a0:	25 88 80 00	or s1,a0,zero
bfe022a4:	1c 05 00 ae	sw zero,0x51c(s0)
bfe022a8:	81 b8 12 3c	lui s2,0xb881
bfe022ac:	fe 04 10 26	addiu s0,s0,0x4fe
bfe022b0:	e0 bf 13 3c	lui s3,0xbfe0
bfe022b4:	14 00 a2 8f	lw v0,0x14(sp)
bfe022b8:	b8 00 a0 af	sw zero,0xb8(sp)
bfe022bc:	02 00 45 94	lhu a1,0x2(v0)
bfe022c0:	04 00 43 94	lhu v1,0x4(v0)
bfe022c4:	00 1c 03 00	sll v1,v1,0x10
bfe022c8:	25 18 65 00	or v1,v1,a1
bfe022cc:	bc 00 a3 af	sw v1,0xbc(sp)
bfe022d0:	0a 00 42 90	lbu v0,0xa(v0)
bfe022d4:	e4 00 a5 8f	lw a1,0xe4(sp)
bfe022d8:	88 01 a0 af	sw zero,0x188(sp)
bfe022dc:	b8 00 a2 af	sw v0,0xb8(sp)
bfe022e0:	02 00 a6 94	lhu a2,0x2(a1)
bfe022e4:	04 00 a2 94	lhu v0,0x4(a1)
bfe022e8:	00 14 02 00	sll v0,v0,0x10
bfe022ec:	25 10 46 00	or v0,v0,a2
bfe022f0:	8c 01 a2 af	sw v0,0x18c(sp)
bfe022f4:	0a 00 a5 90	lbu a1,0xa(a1)
bfe022f8:	13 00 60 14	bne v1,zero,0xbfe02348
bfe022fc:	88 01 a5 af	_sw a1,0x188(sp)
bfe02300:	11 00 40 14	bne v0,zero,0xbfe02348
bfe02304:	00 00 00 00	_nop
bfe02308:	1c 00 80 10	beq a0,zero,0xbfe0237c
bfe0230c:	81 b8 02 3c	_lui v0,0xb881
bfe02310:	00 8a 42 34	ori v0,v0,0x8a00
bfe02314:	00 00 43 8c	lw v1,0x0(v0)
bfe02318:	ff df 04 3c	lui a0,0xdfff
bfe0231c:	ff ff 84 34	ori a0,a0,0xffff
bfe02320:	24 18 64 00	and v1,v1,a0
bfe02324:	00 00 43 ac	sw v1,0x0(v0)
bfe02328:	c4 01 bf 8f	lw ra,0x1c4(sp)
bfe0232c:	c0 01 b4 8f	lw s4,0x1c0(sp)
bfe02330:	bc 01 b3 8f	lw s3,0x1bc(sp)
bfe02334:	b8 01 b2 8f	lw s2,0x1b8(sp)
bfe02338:	b4 01 b1 8f	lw s1,0x1b4(sp)
bfe0233c:	b0 01 b0 8f	lw s0,0x1b0(sp)
bfe02340:	08 00 e0 03	jr ra

### FUNCTION sub_bfe02344 @ bfe02344
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02344:	c8 01 bd 27	_addiu sp,sp,0x1c8

### FUNCTION sub_bfe02348 @ bfe02348
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02348:	0c 00 80 10	beq a0,zero,0xbfe0237c
bfe0234c:	6a e6 02 24	_li v0,-0x1996
bfe02350:	00 8a 54 36	ori s4,s2,0x8a00
bfe02354:	00 00 82 a6	sh v0,0x0(s4)
bfe02358:	01 06 f8 0f	jal 0xbfe01804
bfe0235c:	10 00 a4 27	_addiu a0,sp,0x10
bfe02360:	01 06 f8 0f	jal 0xbfe01804
bfe02364:	e0 00 a4 27	_addiu a0,sp,0xe0
bfe02368:	60 04 f8 0f	jal 0xbfe01180
bfe0236c:	00 00 00 00	_nop
bfe02370:	20 00 82 ae	sw v0,0x20(s4)
bfe02374:	cf ff 00 10	b 0xbfe022b4
bfe02378:	25 20 00 00	_or a0,zero,zero
bfe0237c:	58 07 f8 0f	jal 0xbfe01d60
bfe02380:	10 00 a4 27	_addiu a0,sp,0x10
bfe02384:	58 07 f8 0f	jal 0xbfe01d60
bfe02388:	e0 00 a4 27	_addiu a0,sp,0xe0
bfe0238c:	18 00 20 12	beq s1,zero,0xbfe023f0
bfe02390:	00 00 00 00	_nop
bfe02394:	00 00 02 92	lbu v0,0x0(s0)
bfe02398:	a5 00 03 24	li v1,0xa5
bfe0239c:	14 00 62 10	beq v1,v0,0xbfe023f0
bfe023a0:	00 00 00 00	_nop
bfe023a4:	00 00 02 92	lbu v0,0x0(s0)
bfe023a8:	5a 00 03 24	li v1,0x5a
bfe023ac:	10 00 62 10	beq v1,v0,0xbfe023f0
bfe023b0:	00 00 00 00	_nop
bfe023b4:	60 04 f8 0f	jal 0xbfe01180
bfe023b8:	00 00 00 00	_nop
bfe023bc:	1e 00 03 8e	lw v1,0x1e(s0)
bfe023c0:	23 10 43 00	subu v0,v0,v1
bfe023c4:	88 2e 63 8e	lw v1,0x2e88(s3)
bfe023c8:	00 00 63 8c	lw v1,0x0(v1)
bfe023cc:	2b 10 62 00	sltu v0,v1,v0
bfe023d0:	07 00 40 10	beq v0,zero,0xbfe023f0
bfe023d4:	00 00 00 00	_nop
bfe023d8:	fe ff 02 8e	lw v0,-0x2(s0)
bfe023dc:	00 20 03 3c	lui v1,0x2000
bfe023e0:	25 10 43 00	or v0,v0,v1
bfe023e4:	fe ff 02 ae	sw v0,-0x2(s0)
bfe023e8:	d0 ff 00 10	b 0xbfe0232c
bfe023ec:	c4 01 bf 8f	_lw ra,0x1c4(sp)
bfe023f0:	00 00 02 92	lbu v0,0x0(s0)
bfe023f4:	a5 00 03 24	li v1,0xa5
bfe023f8:	ae ff 62 10	beq v1,v0,0xbfe022b4
bfe023fc:	25 20 00 00	_or a0,zero,zero
bfe02400:	00 00 02 92	lbu v0,0x0(s0)
bfe02404:	5a 00 03 24	li v1,0x5a
bfe02408:	ab ff 62 10	beq v1,v0,0xbfe022b8
bfe0240c:	14 00 a2 8f	_lw v0,0x14(sp)
bfe02410:	60 04 f8 0f	jal 0xbfe01180
bfe02414:	00 00 00 00	_nop
bfe02418:	1e 00 03 8e	lw v1,0x1e(s0)
bfe0241c:	23 10 43 00	subu v0,v0,v1
bfe02420:	89 13 42 2c	sltiu v0,v0,0x1389
bfe02424:	a3 ff 40 14	bne v0,zero,0xbfe022b4
bfe02428:	25 20 00 00	_or a0,zero,zero
bfe0242c:	b8 ff 00 10	b 0xbfe02310
bfe02430:	81 b8 02 3c	_lui v0,0xb881

### FUNCTION FUN_bfe02434 @ bfe02434
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02434:	81 b8 02 3c	lui v0,0xb881
bfe02438:	00 8a 42 34	ori v0,v0,0x8a00
bfe0243c:	00 00 44 8c	lw a0,0x0(v0)
bfe02440:	00 c0 05 3c	lui a1,0xc000
bfe02444:	24 20 85 00	and a0,a0,a1
bfe02448:	08 00 80 14	bne a0,zero,0xbfe0246c
bfe0244c:	00 00 00 00	_nop
bfe02450:	00 00 43 94	lhu v1,0x0(v0)
bfe02454:	91 59 04 24	li a0,0x5991
bfe02458:	24 00 83 14	bne a0,v1,0xbfe024ec
bfe0245c:	00 00 00 00	_nop
bfe02460:	00 00 43 8c	lw v1,0x0(v0)
bfe02464:	25 18 65 00	or v1,v1,a1
bfe02468:	00 00 43 ac	sw v1,0x0(v0)
bfe0246c:	81 b8 02 3c	lui v0,0xb881

### FUNCTION sub_bfe02470 @ bfe02470
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02470:	e8 ff bd 27	addiu sp,sp,-0x18
bfe02474:	04 85 42 34	ori v0,v0,0x8504
bfe02478:	14 00 bf af	sw ra,0x14(sp)
bfe0247c:	00 00 40 ac	sw zero,0x0(v0)
bfe02480:	fc 04 43 94	lhu v1,0x4fc(v0)
bfe02484:	95 19 04 24	li a0,0x1995
bfe02488:	05 00 83 10	beq a0,v1,0xbfe024a0
bfe0248c:	00 00 00 00	_nop
bfe02490:	fc 04 42 94	lhu v0,0x4fc(v0)
bfe02494:	91 59 03 24	li v1,0x5991
bfe02498:	09 00 62 14	bne v1,v0,0xbfe024c0
bfe0249c:	00 00 00 00	_nop
bfe024a0:	81 b8 02 3c	lui v0,0xb881
bfe024a4:	00 8a 42 34	ori v0,v0,0x8a00
bfe024a8:	6a e6 03 24	li v1,-0x1996
bfe024ac:	00 00 43 a4	sh v1,0x0(v0)
bfe024b0:	f0 05 f8 0f	jal 0xbfe017c0
bfe024b4:	00 00 00 00	_nop
bfe024b8:	96 08 f8 0f	jal 0xbfe02258
bfe024bc:	25 20 00 00	_or a0,zero,zero
bfe024c0:	f0 05 f8 0f	jal 0xbfe017c0
bfe024c4:	00 00 00 00	_nop
bfe024c8:	14 00 bf 8f	lw ra,0x14(sp)
bfe024cc:	81 b8 02 3c	lui v0,0xb881
bfe024d0:	00 8a 42 34	ori v0,v0,0x8a00
bfe024d4:	95 19 03 24	li v1,0x1995
bfe024d8:	2d 00 04 3c	lui a0,0x2d
bfe024dc:	00 00 43 a4	sh v1,0x0(v0)
bfe024e0:	c0 c6 84 34	ori a0,a0,0xc6c0
bfe024e4:	24 07 f8 0b	j 0xbfe01c90

### FUNCTION sub_bfe024e8 @ bfe024e8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe024e8:	18 00 bd 27	_addiu sp,sp,0x18
bfe024ec:	08 00 e0 03	jr ra
bfe024f0:	00 00 00 00	_nop

### FUNCTION FUN_bfe024f4 @ bfe024f4
### NOTE References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
bfe024f4:	e0 ff bd 27	addiu sp,sp,-0x20
bfe024f8:	18 00 b1 af	sw s1,0x18(sp)
bfe024fc:	81 b8 11 3c	lui s1,0xb881
bfe02500:	00 8a 31 36	ori s1,s1,0x8a00
bfe02504:	00 00 22 8e	lw v0,0x0(s1)
bfe02508:	00 c0 03 3c	lui v1,0xc000
bfe0250c:	1c 00 bf af	sw ra,0x1c(sp)
bfe02510:	24 10 43 00	and v0,v0,v1
bfe02514:	2d 00 40 10	beq v0,zero,0xbfe025cc
bfe02518:	14 00 b0 af	_sw s0,0x14(sp)
bfe0251c:	01 00 04 24	li a0,0x1
bfe02520:	96 08 f8 0f	jal 0xbfe02258
bfe02524:	e0 bf 10 3c	_lui s0,0xbfe0
bfe02528:	60 04 f8 0f	jal 0xbfe01180
bfe0252c:	00 00 00 00	_nop
bfe02530:	8c 2e 03 8e	lw v1,0x2e8c(s0)
bfe02534:	00 00 63 8c	lw v1,0x0(v1)
bfe02538:	2b 10 62 00	sltu v0,v1,v0
bfe0253c:	28 00 40 10	beq v0,zero,0xbfe025e0
bfe02540:	84 b8 04 3c	_lui a0,0xb884
bfe02544:	75 05 f8 0f	jal 0xbfe015d4
bfe02548:	00 40 84 24	_addiu a0,a0,0x4000
bfe0254c:	62 05 f8 0f	jal 0xbfe01588
bfe02550:	00 00 00 00	_nop
bfe02554:	80 b8 03 3c	lui v1,0xb880
bfe02558:	64 00 62 8c	lw v0,0x64(v1)
bfe0255c:	00 03 04 3c	lui a0,0x300
bfe02560:	25 10 44 00	or v0,v0,a0
bfe02564:	64 00 62 ac	sw v0,0x64(v1)
bfe02568:	6a e6 02 24	li v0,-0x1996
bfe0256c:	00 00 22 a6	sh v0,0x0(s1)
bfe02570:	81 b8 05 3c	lui a1,0xb881
bfe02574:	00 8a a5 34	ori a1,a1,0x8a00
bfe02578:	00 00 a2 8c	lw v0,0x0(a1)
bfe0257c:	00 a0 03 3c	lui v1,0xa000
bfe02580:	82 17 02 00	srl v0,v0,0x1e
bfe02584:	01 00 44 30	andi a0,v0,0x1
bfe02588:	00 00 62 90	lbu v0,0x0(v1)
bfe0258c:	fe 00 42 30	andi v0,v0,0xfe
bfe02590:	25 10 44 00	or v0,v0,a0
bfe02594:	00 00 a4 8c	lw a0,0x0(a1)
bfe02598:	00 00 62 a0	sb v0,0x0(v1)
bfe0259c:	fd 00 42 30	andi v0,v0,0xfd
bfe025a0:	c2 27 04 00	srl a0,a0,0x1f
bfe025a4:	40 20 04 00	sll a0,a0,0x1
bfe025a8:	25 10 44 00	or v0,v0,a0
bfe025ac:	00 00 62 a0	sb v0,0x0(v1)
bfe025b0:	8c 2e 02 8e	lw v0,0x2e8c(s0)
bfe025b4:	00 00 42 8c	lw v0,0x0(v0)
bfe025b8:	02 00 62 a4	sh v0,0x2(v1)
bfe025bc:	e0 bf 02 3c	lui v0,0xbfe0
bfe025c0:	88 2e 42 8c	lw v0,0x2e88(v0)
bfe025c4:	00 00 42 8c	lw v0,0x0(v0)
bfe025c8:	04 00 62 a4	sh v0,0x4(v1)
bfe025cc:	1c 00 bf 8f	lw ra,0x1c(sp)
bfe025d0:	18 00 b1 8f	lw s1,0x18(sp)
bfe025d4:	14 00 b0 8f	lw s0,0x14(sp)
bfe025d8:	08 00 e0 03	jr ra

### FUNCTION sub_bfe025dc @ bfe025dc
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe025dc:	20 00 bd 27	_addiu sp,sp,0x20

### FUNCTION sub_bfe025e0 @ bfe025e0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe025e0:	00 00 22 8e	lw v0,0x0(s1)
bfe025e4:	00 20 03 3c	lui v1,0x2000
bfe025e8:	24 10 43 00	and v0,v0,v1
bfe025ec:	e0 ff 40 14	bne v0,zero,0xbfe02570
bfe025f0:	2d 00 04 3c	_lui a0,0x2d
bfe025f4:	24 07 f8 0f	jal 0xbfe01c90
bfe025f8:	c0 c6 84 34	_ori a0,a0,0xc6c0
bfe025fc:	dd ff 00 10	b 0xbfe02574
bfe02600:	81 b8 05 3c	_lui a1,0xb881

### FUNCTION FUN_bfe02604 @ bfe02604
### NOTE References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
bfe02604:	82 b8 02 3c	lui v0,0xb882
bfe02608:	c8 e0 42 34	ori v0,v0,0xe0c8
bfe0260c:	00 00 43 8c	lw v1,0x0(v0)
bfe02610:	00 01 05 3c	lui a1,0x100

### FUNCTION sub_bfe02614 @ bfe02614
### NOTE References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
bfe02614:	e0 ff bd 27	addiu sp,sp,-0x20
bfe02618:	25 18 65 00	or v1,v1,a1
bfe0261c:	1c 00 86 8c	lw a2,0x1c(a0)
bfe02620:	1c 00 bf af	sw ra,0x1c(sp)
bfe02624:	00 00 43 ac	sw v1,0x0(v0)
bfe02628:	01 00 03 24	li v1,0x1
bfe0262c:	d1 ff 43 a0	sb v1,-0x2f(v0)
bfe02630:	00 00 82 90	lbu v0,0x0(a0)
bfe02634:	04 00 83 8c	lw v1,0x4(a0)
bfe02638:	0b 00 60 10	beq v1,zero,0xbfe02668
bfe0263c:	10 00 a2 a3	_sb v0,0x10(sp)
bfe02640:	03 00 05 24	li a1,0x3
bfe02644:	4f 00 65 10	beq v1,a1,0xbfe02784
bfe02648:	08 00 82 90	_lbu v0,0x8(a0)
bfe0264c:	04 00 05 24	li a1,0x4
bfe02650:	52 00 65 10	beq v1,a1,0xbfe0279c
bfe02654:	02 00 05 24	_li a1,0x2
bfe02658:	03 00 65 14	bne v1,a1,0xbfe02668
bfe0265c:	11 00 a2 a3	_sb v0,0x11(sp)
bfe02660:	09 00 82 90	lbu v0,0x9(a0)
bfe02664:	12 00 a2 a3	sb v0,0x12(sp)
bfe02668:	82 b8 02 3c	lui v0,0xb882
bfe0266c:	c8 e0 42 34	ori v0,v0,0xe0c8
bfe02670:	00 00 45 8c	lw a1,0x0(v0)
bfe02674:	00 06 07 3c	lui a3,0x600
bfe02678:	c0 af 08 3c	lui t0,0xafc0
bfe0267c:	25 28 a7 00	or a1,a1,a3
bfe02680:	00 00 45 ac	sw a1,0x0(v0)
bfe02684:	00 00 43 8c	lw v1,0x0(v0)
bfe02688:	f8 ff 05 3c	lui a1,0xfff8
bfe0268c:	ff ff a5 34	ori a1,a1,0xffff
bfe02690:	25 18 67 00	or v1,v1,a3
bfe02694:	00 00 43 ac	sw v1,0x0(v0)
bfe02698:	d0 ff 43 8c	lw v1,-0x30(v0)
bfe0269c:	10 00 a7 27	addiu a3,sp,0x10
bfe026a0:	24 18 65 00	and v1,v1,a1
bfe026a4:	d0 ff 43 ac	sw v1,-0x30(v0)
bfe026a8:	25 18 e0 00	or v1,a3,zero
bfe026ac:	04 00 85 8c	lw a1,0x4(a0)
bfe026b0:	23 10 67 00	subu v0,v1,a3
bfe026b4:	01 00 a5 24	addiu a1,a1,0x1
bfe026b8:	2b 10 45 00	sltu v0,v0,a1
bfe026bc:	3f 00 40 14	bne v0,zero,0xbfe027bc
bfe026c0:	00 00 00 00	_nop
bfe026c4:	0c 00 82 8c	lw v0,0xc(a0)
bfe026c8:	03 00 40 10	beq v0,zero,0xbfe026d8
bfe026cc:	ff ff 03 24	_li v1,-0x1
bfe026d0:	c0 af 02 3c	lui v0,0xafc0
bfe026d4:	00 00 43 a0	sb v1,0x0(v0)
bfe026d8:	21 00 85 90	lbu a1,0x21(a0)
bfe026dc:	82 b8 02 3c	lui v0,0xb882
bfe026e0:	02 00 03 24	li v1,0x2
bfe026e4:	3a 00 a3 14	bne a1,v1,0xbfe027d0
bfe026e8:	98 e0 42 34	_ori v0,v0,0xe098
bfe026ec:	00 00 43 8c	lw v1,0x0(v0)
bfe026f0:	f8 ff 05 3c	lui a1,0xfff8
bfe026f4:	ff ff a5 34	ori a1,a1,0xffff
bfe026f8:	24 18 65 00	and v1,v1,a1
bfe026fc:	01 00 05 3c	lui a1,0x1
bfe02700:	25 18 65 00	or v1,v1,a1
bfe02704:	00 00 43 ac	sw v1,0x0(v0)
bfe02708:	18 00 85 8c	lw a1,0x18(a0)
bfe0270c:	01 00 02 24	li v0,0x1
bfe02710:	34 00 a2 14	bne a1,v0,0xbfe027e4
bfe02714:	25 10 00 00	_or v0,zero,zero
bfe02718:	c0 af 02 3c	lui v0,0xafc0
bfe0271c:	00 00 42 90	lbu v0,0x0(v0)
bfe02720:	00 00 c2 a0	sb v0,0x0(a2)
bfe02724:	82 b8 02 3c	lui v0,0xb882
bfe02728:	c8 e0 42 34	ori v0,v0,0xe0c8
bfe0272c:	00 00 43 8c	lw v1,0x0(v0)
bfe02730:	ff fe 04 3c	lui a0,0xfeff
bfe02734:	ff ff 84 34	ori a0,a0,0xffff
bfe02738:	24 18 64 00	and v1,v1,a0
bfe0273c:	00 00 43 ac	sw v1,0x0(v0)
bfe02740:	00 00 43 8c	lw v1,0x0(v0)
bfe02744:	ff f9 04 3c	lui a0,0xf9ff
bfe02748:	ff ff 84 34	ori a0,a0,0xffff
bfe0274c:	24 18 64 00	and v1,v1,a0
bfe02750:	00 00 43 ac	sw v1,0x0(v0)
bfe02754:	d1 ff 40 a0	sb zero,-0x2f(v0)
bfe02758:	d1 ff 42 24	addiu v0,v0,-0x2f
bfe0275c:	00 00 43 90	lbu v1,0x0(v0)
bfe02760:	ff 00 63 30	andi v1,v1,0xff
bfe02764:	fd ff 60 14	bne v1,zero,0xbfe0275c
bfe02768:	00 00 00 00	_nop
bfe0276c:	c0 af 02 3c	lui v0,0xafc0
bfe02770:	00 00 40 a0	sb zero,0x0(v0)
bfe02774:	1c 00 bf 8f	lw ra,0x1c(sp)
bfe02778:	25 10 00 00	or v0,zero,zero
bfe0277c:	08 00 e0 03	jr ra

### FUNCTION sub_bfe02780 @ bfe02780
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02780:	20 00 bd 27	_addiu sp,sp,0x20

### FUNCTION sub_bfe02784 @ bfe02784
### NOTE References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
bfe02784:	11 00 a2 a3	sb v0,0x11(sp)
bfe02788:	09 00 82 90	lbu v0,0x9(a0)
bfe0278c:	12 00 a2 a3	sb v0,0x12(sp)
bfe02790:	0a 00 82 90	lbu v0,0xa(a0)
bfe02794:	b4 ff 00 10	b 0xbfe02668
bfe02798:	13 00 a2 a3	_sb v0,0x13(sp)

### NO FUNCTION
bfe0279c:	11 00 a2 a3	sb v0,0x11(sp)
bfe027a0:	09 00 82 90	lbu v0,0x9(a0)
bfe027a4:	12 00 a2 a3	sb v0,0x12(sp)
bfe027a8:	0a 00 82 90	lbu v0,0xa(a0)
bfe027ac:	13 00 a2 a3	sb v0,0x13(sp)
bfe027b0:	0b 00 82 90	lbu v0,0xb(a0)
bfe027b4:	ac ff 00 10	b 0xbfe02668
bfe027b8:	14 00 a2 a3	_sb v0,0x14(sp)

### IN FUNCTION sub_bfe02784
bfe027bc:	00 00 62 90	lbu v0,0x0(v1)
bfe027c0:	01 00 63 24	addiu v1,v1,0x1
bfe027c4:	00 00 02 a1	sb v0,0x0(t0)
bfe027c8:	b9 ff 00 10	b 0xbfe026b0
bfe027cc:	04 00 85 8c	_lw a1,0x4(a0)
bfe027d0:	00 00 43 8c	lw v1,0x0(v0)
bfe027d4:	f8 ff 05 3c	lui a1,0xfff8
bfe027d8:	ff ff a5 34	ori a1,a1,0xffff
bfe027dc:	c9 ff 00 10	b 0xbfe02704
bfe027e0:	24 18 65 00	_and v1,v1,a1
bfe027e4:	20 00 83 90	lbu v1,0x20(a0)
bfe027e8:	0d 00 60 10	beq v1,zero,0xbfe02820
bfe027ec:	c0 af 07 3c	_lui a3,0xafc0
bfe027f0:	6d 0b f8 0f	jal 0xbfe02db4
bfe027f4:	1c 00 84 8c	_lw a0,0x1c(a0)
bfe027f8:	cb ff 00 10	b 0xbfe02728
bfe027fc:	82 b8 02 3c	_lui v0,0xb882
bfe02800:	25 18 c3 00	or v1,a2,v1
bfe02804:	03 00 63 30	andi v1,v1,0x3
bfe02808:	0b 00 60 14	bne v1,zero,0xbfe02838
bfe0280c:	21 28 47 00	_addu a1,v0,a3
bfe02810:	00 00 a3 8c	lw v1,0x0(a1)
bfe02814:	04 00 c6 24	addiu a2,a2,0x4
bfe02818:	04 00 42 24	addiu v0,v0,0x4
bfe0281c:	fc ff c3 ac	sw v1,-0x4(a2)
bfe02820:	18 00 83 8c	lw v1,0x18(a0)
bfe02824:	2b 28 43 00	sltu a1,v0,v1
bfe02828:	f5 ff a0 14	bne a1,zero,0xbfe02800
bfe0282c:	23 18 62 00	_subu v1,v1,v0
bfe02830:	bd ff 00 10	b 0xbfe02728
bfe02834:	82 b8 02 3c	_lui v0,0xb882
bfe02838:	00 00 a5 90	lbu a1,0x0(a1)
bfe0283c:	21 18 c2 00	addu v1,a2,v0
bfe02840:	01 00 42 24	addiu v0,v0,0x1
bfe02844:	f6 ff 00 10	b 0xbfe02820
bfe02848:	00 00 65 a0	_sb a1,0x0(v1)

### FUNCTION FUN_bfe0284c @ bfe0284c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe0284c:	c0 ff bd 27	addiu sp,sp,-0x40
bfe02850:	24 00 06 24	li a2,0x24
bfe02854:	25 28 00 00	or a1,zero,zero
bfe02858:	38 00 b0 af	sw s0,0x38(sp)
bfe0285c:	25 80 80 00	or s0,a0,zero
bfe02860:	3c 00 bf af	sw ra,0x3c(sp)
bfe02864:	8f 03 f8 0f	jal 0xbfe00e3c
bfe02868:	10 00 a4 27	_addiu a0,sp,0x10
bfe0286c:	0f 00 02 24	li v0,0xf
bfe02870:	10 00 a2 a3	sb v0,0x10(sp)
bfe02874:	c0 ff 03 24	li v1,-0x40
bfe02878:	01 00 02 24	li v0,0x1
bfe0287c:	10 00 a4 27	addiu a0,sp,0x10
bfe02880:	14 00 a2 af	sw v0,0x14(sp)
bfe02884:	18 00 a3 a3	sb v1,0x18(sp)
bfe02888:	28 00 a2 af	sw v0,0x28(sp)
bfe0288c:	2c 00 b0 af	sw s0,0x2c(sp)
bfe02890:	81 09 f8 0f	jal 0xbfe02604
bfe02894:	31 00 a2 a3	_sb v0,0x31(sp)
bfe02898:	02 00 41 04	bgez v0,0xbfe028a4
bfe0289c:	3c 00 bf 8f	_lw ra,0x3c(sp)
bfe028a0:	ff ff 02 24	li v0,-0x1
bfe028a4:	38 00 b0 8f	lw s0,0x38(sp)
bfe028a8:	08 00 e0 03	jr ra

### FUNCTION sub_bfe028ac @ bfe028ac
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe028ac:	40 00 bd 27	_addiu sp,sp,0x40

### FUNCTION FUN_bfe028b0 @ bfe028b0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe028b0:	a8 ff bd 27	addiu sp,sp,-0x58
bfe028b4:	50 00 b5 af	sw s5,0x50(sp)
bfe028b8:	48 00 b3 af	sw s3,0x48(sp)
bfe028bc:	3c 00 b0 af	sw s0,0x3c(sp)
bfe028c0:	25 98 a0 00	or s3,a1,zero
bfe028c4:	25 80 80 00	or s0,a0,zero
bfe028c8:	25 a8 c0 00	or s5,a2,zero
bfe028cc:	10 00 a4 27	addiu a0,sp,0x10
bfe028d0:	24 00 06 24	li a2,0x24
bfe028d4:	25 28 00 00	or a1,zero,zero
bfe028d8:	4c 00 b4 af	sw s4,0x4c(sp)
bfe028dc:	44 00 b2 af	sw s2,0x44(sp)
bfe028e0:	40 00 b1 af	sw s1,0x40(sp)
bfe028e4:	68 00 b2 93	lbu s2,0x68(sp)
bfe028e8:	54 00 bf af	sw ra,0x54(sp)
bfe028ec:	25 a0 e0 00	or s4,a3,zero
bfe028f0:	6c 00 b1 93	lbu s1,0x6c(sp)
bfe028f4:	8f 03 f8 0f	jal 0xbfe00e3c
bfe028f8:	34 00 a0 a3	_sb zero,0x34(sp)
bfe028fc:	13 00 02 24	li v0,0x13
bfe02900:	10 00 a2 a3	sb v0,0x10(sp)
bfe02904:	03 00 02 24	li v0,0x3
bfe02908:	14 00 a2 af	sw v0,0x14(sp)
bfe0290c:	02 12 10 00	srl v0,s0,0x8
bfe02910:	19 00 a2 a3	sb v0,0x19(sp)
bfe02914:	10 00 a4 27	addiu a0,sp,0x10
bfe02918:	01 00 02 24	li v0,0x1
bfe0291c:	1a 00 b0 a3	sb s0,0x1a(sp)
bfe02920:	30 00 b2 a3	sb s2,0x30(sp)
bfe02924:	81 09 f8 0f	jal 0xbfe02604
bfe02928:	31 00 a2 a3	_sb v0,0x31(sp)
bfe0292c:	13 0a f8 0f	jal 0xbfe0284c
bfe02930:	34 00 a4 27	_addiu a0,sp,0x34
bfe02934:	34 00 a2 93	lbu v0,0x34(sp)
bfe02938:	01 00 42 30	andi v0,v0,0x1
bfe0293c:	fb ff 40 14	bne v0,zero,0xbfe0292c
bfe02940:	24 00 06 24	_li a2,0x24
bfe02944:	25 28 00 00	or a1,zero,zero
bfe02948:	8f 03 f8 0f	jal 0xbfe00e3c
bfe0294c:	10 00 a4 27	_addiu a0,sp,0x10
bfe02950:	02 00 02 24	li v0,0x2
bfe02954:	02 00 22 16	bne s1,v0,0xbfe02960
bfe02958:	03 00 02 24	_li v0,0x3
bfe0295c:	3b 00 02 24	li v0,0x3b
bfe02960:	10 00 a2 a3	sb v0,0x10(sp)
bfe02964:	82 80 10 00	srl s0,s0,0x2
bfe02968:	02 00 02 24	li v0,0x2
bfe0296c:	14 00 a2 af	sw v0,0x14(sp)
bfe02970:	10 00 10 32	andi s0,s0,0x10
bfe02974:	02 12 13 00	srl v0,s3,0x8
bfe02978:	25 80 50 00	or s0,v0,s0
bfe0297c:	10 00 a4 27	addiu a0,sp,0x10
bfe02980:	01 00 02 24	li v0,0x1
bfe02984:	18 00 b0 a3	sb s0,0x18(sp)
bfe02988:	19 00 b3 a3	sb s3,0x19(sp)
bfe0298c:	28 00 b5 af	sw s5,0x28(sp)
bfe02990:	2c 00 b4 af	sw s4,0x2c(sp)
bfe02994:	30 00 b2 a3	sb s2,0x30(sp)
bfe02998:	31 00 b1 a3	sb s1,0x31(sp)
bfe0299c:	81 09 f8 0f	jal 0xbfe02604
bfe029a0:	1c 00 a2 af	_sw v0,0x1c(sp)
bfe029a4:	54 00 bf 8f	lw ra,0x54(sp)
bfe029a8:	50 00 b5 8f	lw s5,0x50(sp)
bfe029ac:	4c 00 b4 8f	lw s4,0x4c(sp)
bfe029b0:	48 00 b3 8f	lw s3,0x48(sp)
bfe029b4:	44 00 b2 8f	lw s2,0x44(sp)
bfe029b8:	40 00 b1 8f	lw s1,0x40(sp)
bfe029bc:	3c 00 b0 8f	lw s0,0x3c(sp)
bfe029c0:	08 00 e0 03	jr ra

### FUNCTION sub_bfe029c4 @ bfe029c4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe029c4:	58 00 bd 27	_addiu sp,sp,0x58

### FUNCTION FUN_bfe029c8 @ bfe029c8
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe029c8:	e0 bf 02 3c	lui v0,0xbfe0

### FUNCTION sub_bfe029cc @ bfe029cc
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe029cc:	70 ff bd 27	addiu sp,sp,-0x90
bfe029d0:	00 00 42 24	addiu v0,v0,0x0
bfe029d4:	7c 00 b5 af	sw s5,0x7c(sp)
bfe029d8:	6c 00 b1 af	sw s1,0x6c(sp)
bfe029dc:	1f 00 55 88	lwl s5,0x1f(v0)
bfe029e0:	1b 00 51 88	lwl s1,0x1b(v0)
bfe029e4:	68 00 b0 af	sw s0,0x68(sp)
bfe029e8:	1c 00 55 98	lwr s5,0x1c(v0)
bfe029ec:	18 00 51 98	lwr s1,0x18(v0)
bfe029f0:	90 00 a4 af	sw a0,0x90(sp)
bfe029f4:	94 00 a5 af	sw a1,0x94(sp)
bfe029f8:	1b 00 b1 02	divu s5,s1
bfe029fc:	f4 01 20 02	teq s1,zero
bfe02a00:	18 00 a4 27	addiu a0,sp,0x18
bfe02a04:	25 80 c0 00	or s0,a2,zero
bfe02a08:	25 28 00 00	or a1,zero,zero
bfe02a0c:	24 00 06 24	li a2,0x24
bfe02a10:	70 00 b2 af	sw s2,0x70(sp)
bfe02a14:	8c 00 bf af	sw ra,0x8c(sp)
bfe02a18:	88 00 be af	sw s8,0x88(sp)
bfe02a1c:	84 00 b7 af	sw s7,0x84(sp)
bfe02a20:	80 00 b6 af	sw s6,0x80(sp)
bfe02a24:	78 00 b4 af	sw s4,0x78(sp)
bfe02a28:	74 00 b3 af	sw s3,0x74(sp)
bfe02a2c:	12 a8 00 00	mflo s5
bfe02a30:	8f 03 f8 0f	jal 0xbfe00e3c
bfe02a34:	25 90 e0 00	_or s2,a3,zero
bfe02a38:	66 00 02 24	li v0,0x66
bfe02a3c:	18 00 a2 a3	sb v0,0x18(sp)
bfe02a40:	18 00 a4 27	addiu a0,sp,0x18
bfe02a44:	01 00 02 24	li v0,0x1
bfe02a48:	58 00 a0 a3	sb zero,0x58(sp)
bfe02a4c:	81 09 f8 0f	jal 0xbfe02604
bfe02a50:	39 00 a2 a3	_sb v0,0x39(sp)
bfe02a54:	13 0a f8 0f	jal 0xbfe0284c
bfe02a58:	58 00 a4 27	_addiu a0,sp,0x58
bfe02a5c:	58 00 a2 93	lbu v0,0x58(sp)
bfe02a60:	01 00 42 30	andi v0,v0,0x1
bfe02a64:	fb ff 40 14	bne v0,zero,0xbfe02a54
bfe02a68:	99 ff 02 24	_li v0,-0x67
bfe02a6c:	18 00 a4 27	addiu a0,sp,0x18
bfe02a70:	81 09 f8 0f	jal 0xbfe02604
bfe02a74:	18 00 a2 a3	_sb v0,0x18(sp)
bfe02a78:	13 0a f8 0f	jal 0xbfe0284c
bfe02a7c:	58 00 a4 27	_addiu a0,sp,0x58
bfe02a80:	58 00 a2 93	lbu v0,0x58(sp)
bfe02a84:	01 00 42 30	andi v0,v0,0x1
bfe02a88:	fb ff 40 14	bne v0,zero,0xbfe02a78
bfe02a8c:	3f 00 02 3c	_lui v0,0x3f
bfe02a90:	ff ff 42 34	ori v0,v0,0xffff
bfe02a94:	24 80 02 02	and s0,s0,v0
bfe02a98:	1b 00 11 02	divu s0,s1
bfe02a9c:	f4 01 20 02	teq s1,zero
bfe02aa0:	10 00 1e 24	li s8,0x10
bfe02aa4:	25 a0 00 00	or s4,zero,zero
bfe02aa8:	25 98 00 00	or s3,zero,zero
bfe02aac:	ff ff 36 32	andi s6,s1,0xffff
bfe02ab0:	ff 00 17 24	li s7,0xff
bfe02ab4:	12 80 00 00	mflo s0
bfe02ab8:	90 00 a2 8f	lw v0,0x90(sp)
bfe02abc:	2b 10 62 02	sltu v0,s3,v0
bfe02ac0:	0c 00 40 14	bne v0,zero,0xbfe02af4
bfe02ac4:	8c 00 bf 8f	_lw ra,0x8c(sp)
bfe02ac8:	88 00 be 8f	lw s8,0x88(sp)
bfe02acc:	84 00 b7 8f	lw s7,0x84(sp)
bfe02ad0:	80 00 b6 8f	lw s6,0x80(sp)
bfe02ad4:	7c 00 b5 8f	lw s5,0x7c(sp)
bfe02ad8:	78 00 b4 8f	lw s4,0x78(sp)
bfe02adc:	74 00 b3 8f	lw s3,0x74(sp)
bfe02ae0:	70 00 b2 8f	lw s2,0x70(sp)
bfe02ae4:	6c 00 b1 8f	lw s1,0x6c(sp)
bfe02ae8:	68 00 b0 8f	lw s0,0x68(sp)
bfe02aec:	08 00 e0 03	jr ra

### FUNCTION sub_bfe02af0 @ bfe02af0
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02af0:	90 00 bd 27	_addiu sp,sp,0x90

### FUNCTION sub_bfe02af4 @ bfe02af4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02af4:	1a 00 95 02	div s4,s5
bfe02af8:	f4 01 a0 02	teq s5,zero
bfe02afc:	10 10 00 00	mfhi v0
bfe02b00:	17 00 40 14	bne v0,zero,0xbfe02b60
bfe02b04:	40 00 06 24	_li a2,0x40
bfe02b08:	25 28 00 00	or a1,zero,zero
bfe02b0c:	8f 03 f8 0f	jal 0xbfe00e3c
bfe02b10:	18 00 a4 27	_addiu a0,sp,0x18
bfe02b14:	14 00 b2 af	sw s2,0x14(sp)
bfe02b18:	10 00 a0 af	sw zero,0x10(sp)
bfe02b1c:	18 00 a7 27	addiu a3,sp,0x18
bfe02b20:	40 00 06 24	li a2,0x40
bfe02b24:	25 28 c0 02	or a1,s6,zero
bfe02b28:	2c 0a f8 0f	jal 0xbfe028b0
bfe02b2c:	ff ff 84 32	_andi a0,s4,0xffff
bfe02b30:	18 00 a2 93	lbu v0,0x18(sp)
bfe02b34:	03 00 57 14	bne v0,s7,0xbfe02b44
bfe02b38:	19 00 a2 93	_lbu v0,0x19(sp)
bfe02b3c:	08 00 57 10	beq v0,s7,0xbfe02b60
bfe02b40:	00 00 00 00	_nop
bfe02b44:	ff ff de 27	addiu s8,s8,-0x1
bfe02b48:	03 00 c0 17	bne s8,zero,0xbfe02b58
bfe02b4c:	21 50 95 02	_addu t2,s4,s5
bfe02b50:	ff ff 00 10	b 0xbfe02b50
bfe02b54:	00 00 00 00	_nop
bfe02b58:	d7 ff 00 10	b 0xbfe02ab8
bfe02b5c:	25 a0 40 01	_or s4,t2,zero
bfe02b60:	03 00 00 12	beq s0,zero,0xbfe02b70
bfe02b64:	01 00 8a 26	_addiu t2,s4,0x1
bfe02b68:	fb ff 00 10	b 0xbfe02b58
bfe02b6c:	ff ff 10 26	_addiu s0,s0,-0x1
bfe02b70:	01 00 02 24	li v0,0x1
bfe02b74:	10 00 a2 af	sw v0,0x10(sp)
bfe02b78:	94 00 a2 8f	lw v0,0x94(sp)
bfe02b7c:	14 00 b2 af	sw s2,0x14(sp)
bfe02b80:	25 30 c0 02	or a2,s6,zero
bfe02b84:	21 38 53 00	addu a3,v0,s3
bfe02b88:	25 28 00 00	or a1,zero,zero
bfe02b8c:	ff ff 84 32	andi a0,s4,0xffff
bfe02b90:	60 00 aa af	sw t2,0x60(sp)
bfe02b94:	2c 0a f8 0f	jal 0xbfe028b0
bfe02b98:	21 98 71 02	_addu s3,s3,s1
bfe02b9c:	ee ff 00 10	b 0xbfe02b58
bfe02ba0:	60 00 aa 8f	_lw t2,0x60(sp)

### FUNCTION FUN_bfe02ba4 @ bfe02ba4
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02ba4:	e8 ff bd 27	addiu sp,sp,-0x18
bfe02ba8:	10 00 b0 af	sw s0,0x10(sp)
bfe02bac:	14 00 bf af	sw ra,0x14(sp)
bfe02bb0:	1a 00 a0 14	bne a1,zero,0xbfe02c1c
bfe02bb4:	04 00 90 8c	_lw s0,0x4(a0)
bfe02bb8:	80 ff 02 24	li v0,-0x80
bfe02bbc:	80 03 02 a2	sb v0,0x380(s0)
bfe02bc0:	e9 04 f8 0f	jal 0xbfe013a4
bfe02bc4:	84 03 04 24	_li a0,0x384
bfe02bc8:	80 03 02 92	lbu v0,0x380(s0)
bfe02bcc:	ef 00 42 30	andi v0,v0,0xef
bfe02bd0:	8c 00 42 34	ori v0,v0,0x8c
bfe02bd4:	80 03 02 a2	sb v0,0x380(s0)
bfe02bd8:	60 00 02 92	lbu v0,0x60(s0)
bfe02bdc:	84 03 04 24	li a0,0x384
bfe02be0:	fe 00 42 30	andi v0,v0,0xfe
bfe02be4:	60 00 02 a2	sb v0,0x60(s0)
bfe02be8:	e9 04 f8 0f	jal 0xbfe013a4
bfe02bec:	00 00 00 00	_nop
bfe02bf0:	60 00 02 92	lbu v0,0x60(s0)
bfe02bf4:	84 03 04 24	li a0,0x384
bfe02bf8:	01 00 42 34	ori v0,v0,0x1
bfe02bfc:	60 00 02 a2	sb v0,0x60(s0)
bfe02c00:	01 00 02 92	lbu v0,0x1(s0)
bfe02c04:	40 00 42 34	ori v0,v0,0x40
bfe02c08:	01 00 02 a2	sb v0,0x1(s0)
bfe02c0c:	14 00 bf 8f	lw ra,0x14(sp)
bfe02c10:	10 00 b0 8f	lw s0,0x10(sp)
bfe02c14:	e9 04 f8 0b	j 0xbfe013a4

### FUNCTION sub_bfe02c18 @ bfe02c18
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02c18:	18 00 bd 27	_addiu sp,sp,0x18
bfe02c1c:	01 00 02 24	li v0,0x1
bfe02c20:	ed ff a2 14	bne a1,v0,0xbfe02bd8
bfe02c24:	90 ff 02 24	_li v0,-0x70
bfe02c28:	80 03 02 a2	sb v0,0x380(s0)
bfe02c2c:	e9 04 f8 0f	jal 0xbfe013a4
bfe02c30:	84 03 04 24	_li a0,0x384
bfe02c34:	80 03 02 92	lbu v0,0x380(s0)
bfe02c38:	e6 ff 00 10	b 0xbfe02bd4
bfe02c3c:	9c 00 42 34	_ori v0,v0,0x9c

### FUNCTION FUN_bfe02c40 @ bfe02c40
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02c40:	e0 bf 03 3c	lui v1,0xbfe0
bfe02c44:	00 00 63 24	addiu v1,v1,0x0

### FUNCTION sub_bfe02c48 @ bfe02c48
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02c48:	e0 ff bd 27	addiu sp,sp,-0x20
bfe02c4c:	14 00 b0 af	sw s0,0x14(sp)
bfe02c50:	49 00 70 90	lbu s0,0x49(v1)
bfe02c54:	48 00 62 90	lbu v0,0x48(v1)
bfe02c58:	18 00 b1 af	sw s1,0x18(sp)
bfe02c5c:	00 82 10 00	sll s0,s0,0x8
bfe02c60:	25 80 02 02	or s0,s0,v0
bfe02c64:	4b 00 62 90	lbu v0,0x4b(v1)
bfe02c68:	4a 00 71 90	lbu s1,0x4a(v1)
bfe02c6c:	2d 00 04 3c	lui a0,0x2d
bfe02c70:	00 12 02 00	sll v0,v0,0x8
bfe02c74:	25 88 51 00	or s1,v0,s1
bfe02c78:	1c 00 bf af	sw ra,0x1c(sp)
bfe02c7c:	24 07 f8 0f	jal 0xbfe01c90
bfe02c80:	c0 c6 84 34	_ori a0,a0,0xc6c0
bfe02c84:	80 10 11 00	sll v0,s1,0x2
bfe02c88:	21 10 51 00	addu v0,v0,s1
bfe02c8c:	40 10 02 00	sll v0,v0,0x1
bfe02c90:	e8 ff 42 24	addiu v0,v0,-0x18
bfe02c94:	18 00 05 24	li a1,0x18
bfe02c98:	1b 00 45 00	divu v0,a1
bfe02c9c:	f4 01 a0 00	teq a1,zero
bfe02ca0:	80 b8 03 3c	lui v1,0xb880
bfe02ca4:	80 03 64 8c	lw a0,0x380(v1)
bfe02ca8:	00 80 05 3c	lui a1,0x8000
bfe02cac:	ff ff 84 30	andi a0,a0,0xffff
bfe02cb0:	25 20 85 00	or a0,a0,a1
bfe02cb4:	ff f8 05 24	li a1,-0x701
bfe02cb8:	12 10 00 00	mflo v0
bfe02cbc:	00 14 02 00	sll v0,v0,0x10
bfe02cc0:	25 10 44 00	or v0,v0,a0
bfe02cc4:	80 03 62 ac	sw v0,0x380(v1)
bfe02cc8:	74 00 64 8c	lw a0,0x74(v1)
bfe02ccc:	00 12 10 00	sll v0,s0,0x8
bfe02cd0:	00 07 42 30	andi v0,v0,0x700
bfe02cd4:	24 20 85 00	and a0,a0,a1
bfe02cd8:	25 10 44 00	or v0,v0,a0
bfe02cdc:	74 00 62 ac	sw v0,0x74(v1)
bfe02ce0:	07 00 02 24	li v0,0x7
bfe02ce4:	04 00 02 16	bne s0,v0,0xbfe02cf8
bfe02ce8:	00 00 00 00	_nop
bfe02cec:	7c 00 62 8c	lw v0,0x7c(v1)
bfe02cf0:	80 00 42 34	ori v0,v0,0x80
bfe02cf4:	7c 00 62 ac	sw v0,0x7c(v1)
bfe02cf8:	80 b8 03 3c	lui v1,0xb880
bfe02cfc:	74 00 62 8c	lw v0,0x74(v1)
bfe02d00:	1c 00 bf 8f	lw ra,0x1c(sp)
bfe02d04:	18 00 b1 8f	lw s1,0x18(sp)
bfe02d08:	14 00 b0 8f	lw s0,0x14(sp)
bfe02d0c:	40 00 04 3c	lui a0,0x40
bfe02d10:	25 10 44 00	or v0,v0,a0
bfe02d14:	74 00 62 ac	sw v0,0x74(v1)
bfe02d18:	31 07 f8 0b	j 0xbfe01cc4

### FUNCTION sub_bfe02d1c @ bfe02d1c
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02d1c:	20 00 bd 27	_addiu sp,sp,0x20

### FUNCTION first_stage_clock_or_pll_select @ bfe02d20
### NOTE Medium confidence: adjusts 0xb882e004/0xb882e094 and 0xb880007c based on a frequency-like argument.
bfe02d20:	82 b8 02 3c	lui v0,0xb882
bfe02d24:	04 e0 42 34	ori v0,v0,0xe004
bfe02d28:	00 00 45 8c	lw a1,0x0(v0)
bfe02d2c:	ff f0 03 3c	lui v1,0xf0ff
bfe02d30:	ff ff 63 34	ori v1,v1,0xffff
bfe02d34:	24 28 a3 00	and a1,a1,v1
bfe02d38:	00 00 45 ac	sw a1,0x0(v0)
bfe02d3c:	d7 05 05 3c	lui a1,0x5d7
bfe02d40:	80 5c a5 24	addiu a1,a1,0x5c80
bfe02d44:	2b 28 85 00	sltu a1,a0,a1
bfe02d48:	0d 00 a0 14	bne a1,zero,0xbfe02d80
bfe02d4c:	37 03 05 3c	_lui a1,0x337
bfe02d50:	80 b8 06 3c	lui a2,0xb880
bfe02d54:	7c 00 c4 8c	lw a0,0x7c(a2)
bfe02d58:	ff ff 05 3c	lui a1,0xffff
bfe02d5c:	ff 3f a5 24	addiu a1,a1,0x3fff
bfe02d60:	24 20 85 00	and a0,a0,a1
bfe02d64:	00 80 84 34	ori a0,a0,0x8000
bfe02d68:	7c 00 c4 ac	sw a0,0x7c(a2)
bfe02d6c:	94 00 44 8c	lw a0,0x94(v0)
bfe02d70:	24 18 83 00	and v1,a0,v1
bfe02d74:	94 00 43 ac	sw v1,0x94(v0)
bfe02d78:	08 00 e0 03	jr ra
bfe02d7c:	00 00 00 00	_nop

### FUNCTION sub_bfe02d80 @ bfe02d80
### NOTE No strong automatic classification; name and body are generated from raw MIPS control flow.
bfe02d80:	80 f9 a5 34	ori a1,a1,0xf980
bfe02d84:	2b 28 85 00	sltu a1,a0,a1
bfe02d88:	f8 ff a0 10	beq a1,zero,0xbfe02d6c
bfe02d8c:	9b 01 05 3c	_lui a1,0x19b
bfe02d90:	c0 fc a5 34	ori a1,a1,0xfcc0
bfe02d94:	2b 20 85 00	sltu a0,a0,a1
bfe02d98:	f7 ff 80 14	bne a0,zero,0xbfe02d78
bfe02d9c:	00 00 00 00	_nop
bfe02da0:	94 00 44 8c	lw a0,0x94(v0)
bfe02da4:	24 18 83 00	and v1,a0,v1
bfe02da8:	00 01 04 3c	lui a0,0x100
bfe02dac:	f1 ff 00 10	b 0xbfe02d74
bfe02db0:	25 18 64 00	_or v1,v1,a0

### FUNCTION first_stage_sfspi_transfer_setup @ bfe02db4
### NOTE Medium confidence: programs the SPI/SF controller registers around 0xb882e058 and starts a transfer/status wait loop.
bfe02db4:	ff 0f 02 3c	lui v0,0xfff
bfe02db8:	ff ff 42 34	ori v0,v0,0xffff
bfe02dbc:	24 20 82 00	and a0,a0,v0
bfe02dc0:	82 b8 02 3c	lui v0,0xb882
bfe02dc4:	58 e0 42 34	ori v0,v0,0xe058
bfe02dc8:	00 00 44 ac	sw a0,0x0(v0)
bfe02dcc:	08 00 45 ac	sw a1,0x8(v0)
bfe02dd0:	0c 00 43 8c	lw v1,0xc(v0)
bfe02dd4:	10 00 04 3c	lui a0,0x10
bfe02dd8:	48 00 42 24	addiu v0,v0,0x48
bfe02ddc:	25 18 64 00	or v1,v1,a0
bfe02de0:	c4 ff 43 ac	sw v1,-0x3c(v0)
bfe02de4:	c4 ff 43 8c	lw v1,-0x3c(v0)
bfe02de8:	08 00 04 3c	lui a0,0x8
bfe02dec:	25 18 64 00	or v1,v1,a0
bfe02df0:	c4 ff 43 ac	sw v1,-0x3c(v0)
bfe02df4:	c4 ff 43 8c	lw v1,-0x3c(v0)
bfe02df8:	04 00 04 3c	lui a0,0x4
bfe02dfc:	25 18 64 00	or v1,v1,a0
bfe02e00:	c4 ff 43 ac	sw v1,-0x3c(v0)
bfe02e04:	c4 ff 43 8c	lw v1,-0x3c(v0)
bfe02e08:	02 00 04 3c	lui a0,0x2
bfe02e0c:	25 18 64 00	or v1,v1,a0
bfe02e10:	c4 ff 43 ac	sw v1,-0x3c(v0)
bfe02e14:	00 00 43 8c	lw v1,0x0(v0)
bfe02e18:	01 00 63 34	ori v1,v1,0x1
bfe02e1c:	00 00 43 ac	sw v1,0x0(v0)
bfe02e20:	c4 ff 43 8c	lw v1,-0x3c(v0)
bfe02e24:	20 00 63 34	ori v1,v1,0x20
bfe02e28:	c4 ff 43 ac	sw v1,-0x3c(v0)
bfe02e2c:	00 00 43 8c	lw v1,0x0(v0)
bfe02e30:	01 00 63 30	andi v1,v1,0x1
bfe02e34:	fd ff 60 10	beq v1,zero,0xbfe02e2c
bfe02e38:	00 00 00 00	_nop
bfe02e3c:	00 00 43 8c	lw v1,0x0(v0)
bfe02e40:	01 00 63 34	ori v1,v1,0x1
bfe02e44:	00 00 43 ac	sw v1,0x0(v0)
bfe02e48:	08 00 e0 03	jr ra
bfe02e4c:	00 00 00 00	_nop
