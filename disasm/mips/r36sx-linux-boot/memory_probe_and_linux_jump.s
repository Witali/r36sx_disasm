
### RANGE 89daa730 +124
89daa730:	e0 ff bd 27	addiu sp,sp,-0x20
89daa734:	1c 00 bf af	sw ra,0x1c(sp)
89daa738:	a3 e9 76 0e	jal 0x89dba68c
89daa73c:	00 00 00 00	_nop
89daa740:	eb 89 03 3c	lui v1,0x89eb
89daa744:	40 e3 62 ac	sw v0,-0x1cc0(v1)
89daa748:	bb aa 02 3c	lui v0,0xaabb
89daa74c:	00 a8 03 3c	lui v1,0xa800
89daa750:	dd cc 42 34	ori v0,v0,0xccdd
89daa754:	00 a0 04 3c	lui a0,0xa000
89daa758:	20 00 60 ac	sw zero,0x20(v1)
89daa75c:	20 00 82 ac	sw v0,0x20(a0)
89daa760:	20 00 63 8c	lw v1,0x20(v1)
89daa764:	04 00 62 14	bne v1,v0,0x89daa778
89daa768:	0b 00 02 24	_li v0,0xb
89daa76c:	ea 89 02 3c	lui v0,0x89ea
89daa770:	90 52 40 ac	sw zero,0x5290(v0)
89daa774:	0b 00 02 24	li v0,0xb
89daa778:	e8 89 05 3c	lui a1,0x89e8
89daa77c:	db 89 04 3c	lui a0,0x89db
89daa780:	25 38 00 00	or a3,zero,zero
89daa784:	00 10 06 24	li a2,0x1000
89daa788:	5c d7 a5 24	addiu a1,a1,-0x28a4
89daa78c:	cc 99 84 24	addiu a0,a0,-0x6634
89daa790:	14 00 a0 af	sw zero,0x14(sp)
89daa794:	3b f9 76 0e	jal 0x89dbe4ec
89daa798:	10 00 a2 af	_sw v0,0x10(sp)
89daa79c:	02 f9 76 0e	jal 0x89dbe408
89daa7a0:	00 00 00 00	_nop
89daa7a4:	0f 77 77 0e	jal 0x89dddc3c
89daa7a8:	00 00 00 00	_nop

### RANGE 89dae5ec +184
89dae5ec:	e0 ff bd 27	addiu sp,sp,-0x20
89dae5f0:	14 00 b1 af	sw s1,0x14(sp)
89dae5f4:	80 00 91 30	andi s1,a0,0x80
89dae5f8:	1c 00 bf af	sw ra,0x1c(sp)
89dae5fc:	18 00 b2 af	sw s2,0x18(sp)
89dae600:	1e 00 20 16	bne s1,zero,0x89dae67c
89dae604:	10 00 b0 af	_sw s0,0x10(sp)
89dae608:	40 01 82 30	andi v0,a0,0x140
89dae60c:	1d 00 40 14	bne v0,zero,0x89dae684
89dae610:	1c 00 bf 8f	_lw ra,0x1c(sp)
89dae614:	00 06 84 30	andi a0,a0,0x600
89dae618:	20 00 80 10	beq a0,zero,0x89dae69c
89dae61c:	e8 89 04 3c	_lui a0,0x89e8
89dae620:	60 00 f2 8c	lw s2,0x60(a3)
89dae624:	25 80 e0 00	or s0,a3,zero
89dae628:	25 28 40 02	or a1,s2,zero
89dae62c:	24 bf 76 0e	jal 0x89dafc90
89dae630:	5c ea 84 24	_addiu a0,a0,-0x15a4
89dae634:	70 00 02 8e	lw v0,0x70(s0)
89dae638:	12 00 40 10	beq v0,zero,0x89dae684
89dae63c:	1c 00 bf 8f	_lw ra,0x1c(sp)
89dae640:	ec 89 02 3c	lui v0,0x89ec
89dae644:	24 7b 42 8c	lw v0,0x7b24(v0)
89dae648:	03 00 40 14	bne v0,zero,0x89dae658
89dae64c:	00 00 00 00	_nop
89dae650:	28 f8 76 0e	jal 0x89dbe0a0
89dae654:	00 00 00 00	_nop
89dae658:	13 04 77 0e	jal 0x89dc104c
89dae65c:	00 00 00 00	_nop
89dae660:	d1 ea 76 0e	jal 0x89dbab44
89dae664:	00 00 00 00	_nop
89dae668:	6c 00 05 8e	lw a1,0x6c(s0)
89dae66c:	25 38 00 00	or a3,zero,zero
89dae670:	25 30 00 00	or a2,zero,zero
89dae674:	09 f8 40 02	jalr s2
89dae678:	fe ff 04 24	_li a0,-0x2
89dae67c:	ff ff 11 24	li s1,-0x1
89dae680:	1c 00 bf 8f	lw ra,0x1c(sp)
89dae684:	25 10 20 02	or v0,s1,zero
89dae688:	18 00 b2 8f	lw s2,0x18(sp)
89dae68c:	14 00 b1 8f	lw s1,0x14(sp)
89dae690:	10 00 b0 8f	lw s0,0x10(sp)
89dae694:	08 00 e0 03	jr ra
89dae698:	20 00 bd 27	_addiu sp,sp,0x20
89dae69c:	f8 ff 00 10	b 0x89dae680
89dae6a0:	01 00 11 24	_li s1,0x1
