
/* ===== FORCED FUNCTION forced_1000:0032 @ 1000:0032 ===== */

void forced_1000_0032(void)

{
  return;
}



/* ===== FORCED FUNCTION forced_1000:0000 @ 1000:0000 ===== */

void forced_1000_0000(void)

{
  char *pcVar1;
  code *pcVar2;
  char cVar3;
  int in_AX;
  undefined2 in_DX;
  int in_BX;
  int unaff_BP;
  int unaff_SI;
  int unaff_DI;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  
  pcVar1 = (char *)(in_BX + unaff_DI + 8);
  *pcVar1 = *pcVar1 + (char)in_AX;
  *(int *)(in_BX + unaff_SI) = *(int *)(in_BX + unaff_SI) + in_AX;
  cVar3 = (char)((uint)in_DX >> 8);
  *(undefined1 *)(in_BX + unaff_SI) = 0xff;
  pcVar1 = (char *)(in_BX + unaff_SI + -0x4ea9);
  *pcVar1 = *pcVar1 + cVar3;
  pcVar1 = (char *)(unaff_BP + unaff_SI + -0x6000);
  *pcVar1 = *pcVar1 + cVar3;
  pcVar1 = (char *)(in_BX + unaff_SI + 0xacc);
  *pcVar1 = *pcVar1 + cVar3;
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}



/* ===== FORCED FUNCTION forced_1000:0100 @ 1000:0100 ===== */

void forced_1000_0100(void)

{
  uint uVar1;
  code *pcVar2;
  uint uVar3;
  byte bVar4;
  byte bVar5;
  uint in_AX;
  int iVar6;
  uint uVar7;
  uint in_DX;
  uint in_BX;
  uint unaff_BP;
  uint unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  undefined1 in_CF;
  bool bVar8;
  bool bVar9;
  longdouble in_ST0;
  
  uVar3 = CONCAT11(in_CF,*(undefined1 *)(unaff_BP + unaff_DI));
  uVar3 = uVar3 << 5 | uVar3 >> 4;
  *(undefined1 *)(unaff_BP + unaff_DI) = (char)uVar3;
  uVar3 = (uint)(CARRY2(unaff_BP,in_DX) ||
                CARRY2(unaff_BP + in_DX,
                       (uint)(CARRY2(in_BX,(uint)&stack0x0000) ||
                             CARRY2((uint)(&stack0x0000 + in_BX),(uint)((uVar3 & 0x100) != 0)))));
  uVar7 = (uint)(CARRY2(unaff_DI,in_AX) || CARRY2(unaff_DI + in_AX,uVar3));
  uVar1 = in_AX + *(uint *)0x1414;
  bVar8 = CARRY2(in_AX,*(uint *)0x1414) || CARRY2(uVar1,uVar7);
  iVar6 = uVar1 + uVar7;
  bVar5 = (byte)iVar6;
  bVar4 = bVar5 + 0x22;
  bVar9 = 0xdd < bVar5 || CARRY1(bVar4,bVar8);
  bVar4 = bVar4 + bVar8;
  bVar5 = bVar4 + 0x5f;
  bVar8 = 0xa0 < bVar4 || CARRY1(bVar5,bVar9);
  bVar5 = bVar5 + bVar9;
  bVar4 = bVar5 + 0x93;
  bVar9 = 0x6c < bVar5 || CARRY1(bVar4,bVar8);
  bVar4 = bVar4 + bVar8;
  bVar5 = bVar4 + 0x9a;
  bVar8 = 0x65 < bVar4 || CARRY1(bVar5,bVar9);
  bVar5 = bVar5 + bVar9;
  bVar4 = bVar5 - 0x11;
  bVar9 = 0x10 < bVar5 || CARRY1(bVar4,bVar8);
  bVar4 = bVar4 + bVar8;
  bVar5 = bVar4 + 0x21;
  uVar7 = CONCAT11((char)((uint)iVar6 >> 8),bVar5 + bVar9) + 0x15a8 +
          (uint)(0xde < bVar4 || CARRY1(bVar5,bVar9));
  *(uint *)(unaff_DI + in_AX + uVar3) = uVar7;
  *(double *)0xc915 = (double)in_ST0;
  out(0x15,uVar7 + 0x2b88 + (uint)(0xea4a < uVar7));
  pcVar2 = (code *)swi(1);
  (*pcVar2)();
  return;
}


