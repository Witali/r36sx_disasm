
/* ===== FORCED FUNCTION forced_1000:3d90 @ 1000:3d90 ===== */

void forced_1000_3d90(void)

{
  byte *pbVar1;
  undefined2 *puVar2;
  byte *pbVar3;
  undefined2 *puVar4;
  code *pcVar5;
  undefined2 uVar6;
  undefined2 in_CX;
  int iVar7;
  undefined2 in_DX;
  undefined2 in_BX;
  undefined2 *puVar8;
  undefined2 *puVar9;
  byte *pbVar10;
  undefined2 unaff_DS;
  bool bVar11;
  
  *(undefined1 *)0x6f01 = (char)((uint)in_DX >> 8);
  *(undefined1 *)0x6cf2 = 0;
  *(undefined1 *)0x6cf2 = 1;
  func_0x00017a1a();
  *(undefined2 *)0x1d70 = in_BX;
  *(undefined2 *)0x1d72 = in_CX;
  pcVar5 = (code *)swi(0x10);
  (*pcVar5)();
  func_0x00013fc5();
  puVar9 = (undefined2 *)0xb6f4;
  puVar8 = (undefined2 *)0xa5b2;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar4 = puVar9;
    puVar9 = puVar9 + 1;
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar4 = *puVar2;
  }
  puVar8 = puVar8 + 0xd2;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar4 = puVar9;
    puVar9 = puVar9 + 1;
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar4 = *puVar2;
  }
  puVar8 = puVar8 + 0xf;
  for (iVar7 = 0xd; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar4 = puVar9;
    puVar9 = puVar9 + 1;
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar4 = *puVar2;
  }
  puVar8 = puVar8 + 0xb4;
  for (iVar7 = 7; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar4 = puVar9;
    puVar9 = puVar9 + 1;
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar4 = *puVar2;
  }
  func_0x00013f06();
  func_0x00013f06();
  func_0x00013f06();
  uVar6 = func_0x00013f06();
  if (*(char *)0xb6f2 == '\0') {
    iVar7 = 0x300;
    if (*(int *)0xa5a0 != 0) {
      pbVar10 = (byte *)(*(int *)0xa5a0 + -0x5a6a);
      do {
        pbVar1 = pbVar10;
        pbVar3 = pbVar10;
        pbVar10 = pbVar10 + 1;
        *pbVar3 = *pbVar1 >> 2;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
  }
  iVar7 = iRam0000046c + 0x5b;
  bVar11 = iVar7 == 0;
  while( true ) {
    pcVar5 = (code *)swi(0x16);
    (*pcVar5)();
    if (!bVar11) break;
    if (iVar7 == iRam0000046c) goto LAB_1000_3e46;
    bVar11 = false;
  }
  pcVar5 = (code *)swi(0x16);
  (*pcVar5)();
LAB_1000_3e46:
  puVar8 = (undefined2 *)0x0;
  for (iVar7 = -1; iVar7 != 0; iVar7 = iVar7 + -1) {
    puVar2 = puVar8;
    puVar8 = puVar8 + 1;
    *puVar2 = 0;
  }
  func_0x00013fc5();
  func_0x000169d6();
  func_0x000140f7();
  func_0x000144e7();
  *(undefined1 *)0x6f02 = 0;
  if (*(char *)0x6f01 != '\0') {
    func_0x000178e5();
    *(undefined1 *)0xb43f = 0;
    func_0x00016652();
    do {
      func_0x0001793f();
      if (*(char *)0xb598 == '1') {
        *(undefined1 *)0x6f02 = 1;
        func_0x000178e5();
        goto LAB_1000_3ea8;
      }
    } while (*(char *)0xb598 != '2');
    *(undefined1 *)0x6f02 = 2;
LAB_1000_3ea8:
    *(char *)0xb597 = *(char *)0x6f02 + '0';
    func_0x00016652();
    *(undefined2 *)0x1d74 = 100;
    do {
    } while (*(int *)0x1d74 != 0);
  }
  func_0x000141f7();
  func_0x00017b3e();
  *(byte *)0x6cf0 = *(byte *)0x6cf0 | 0x40;
  *(undefined2 *)0xb677 = 0;
  *(undefined2 *)0xb679 = 0;
  *(undefined2 *)0x9110 = 0;
  *(undefined1 *)0x6ced = 2;
  do {
    func_0x000194ac();
    func_0x0001b89e();
    func_0x0001c0ad();
    func_0x000165f0();
    func_0x00015d1a();
    *(undefined2 *)0x9110 = 0;
    func_0x000192ba();
    func_0x0001466b();
  } while( true );
}


/* FAILED TO CREATE FUNCTION @ 1000:3f58 */
/* FAILED TO CREATE FUNCTION @ 1000:3fd0 */

/* ===== FORCED FUNCTION forced_1000:40cc @ 1000:40cc ===== */

undefined4
forced_1000_40cc(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
                undefined2 param_5,undefined2 param_6)

{
  byte *pbVar1;
  byte *pbVar2;
  code *pcVar3;
  int iVar4;
  byte *in_stack_00000000;
  
  iVar4 = 0x300;
  do {
    pbVar1 = in_stack_00000000;
    pbVar2 = in_stack_00000000;
    in_stack_00000000 = in_stack_00000000 + 1;
    *pbVar2 = *pbVar1 >> 2;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  pcVar3 = (code *)swi(0x10);
  (*pcVar3)();
  return CONCAT22(param_6,param_3);
}



/* ===== FORCED FUNCTION forced_1000:44c0 @ 1000:44c0 ===== */

undefined4 forced_1000_44c0(void)

{
  undefined2 uVar1;
  code *pcVar2;
  byte bVar3;
  int iVar4;
  byte bVar5;
  char *in_BX;
  int unaff_ES;
  undefined2 in_stack_0000002e;
  undefined2 in_stack_00000030;
  undefined2 in_stack_00000036;
  
  uVar1 = *(undefined2 *)0x1bce;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  out(0x43,0x34);
  out(0x40,0xff);
  out(0x40,0xff);
  pcVar2 = (code *)swi(0x10);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(undefined1 *)0x6cf0 = 0x80;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)(in_BX);
  if ((unaff_ES != 0 || in_BX != (char *)0x0) && (*in_BX != -0x31)) {
    pcVar2 = (code *)swi(0x33);
    iVar4 = (*pcVar2)();
    if (iVar4 == -1) {
      pcVar2 = (code *)swi(0x33);
      (*pcVar2)();
      pcVar2 = (code *)swi(0x33);
      (*pcVar2)();
      in_BX = (char *)0x0;
      unaff_ES = 0x1000;
      pcVar2 = (code *)swi(0x33);
      (*pcVar2)();
      *(undefined2 *)0x1d6c = 0xa0;
      *(undefined2 *)0x1d6e = 100;
      pcVar2 = (code *)swi(0x33);
      (*pcVar2)();
      *(undefined1 *)0x6cf0 = 0;
      *(byte *)0x6cf0 = *(byte *)0x6cf0 & 0x7f;
      *(undefined1 *)0x6cf1 = 0xff;
      func_0x00017b09();
    }
  }
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bc2 = unaff_ES;
  *(undefined2 *)0x1bc0 = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bc6 = unaff_ES;
  *(undefined2 *)0x1bc4 = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bb2 = unaff_ES;
  *(undefined2 *)0x1bb0 = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bba = unaff_ES;
  *(undefined2 *)0x1bb8 = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bbe = unaff_ES;
  *(undefined2 *)0x1bbc = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bca = unaff_ES;
  *(undefined2 *)0x1bc8 = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  *(int *)0x1bce = unaff_ES;
  *(undefined2 *)0x1bcc = in_BX;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  if (*(char *)0x6f01 != '\0') {
    iVar4 = 0x3f8;
    if (*(char *)0x6f01 != '\x01') {
      iVar4 = 0x2f8;
    }
    pcVar2 = (code *)swi(0x21);
    (*pcVar2)();
    *(int *)0x6eff = iVar4;
    out(iVar4 + 3,0x80);
    out(iVar4 + 1,0);
    out(iVar4,0x60);
    out(iVar4 + 3,0x1b);
    iVar4 = *(int *)0x6eff;
    out(iVar4 + 1,7);
    out(iVar4 + 4,0xb);
    in(iVar4 + 5);
    in(iVar4 + 6);
    in(iVar4);
    in(iVar4 + 2);
    *(undefined1 *)0x6cf9 = 0x10;
    bVar5 = 0xef;
    if (*(int *)0x6eff != 0x3f8) {
      bVar5 = 0xf7;
    }
    bVar3 = in(0x21);
    out(0x21,bVar3 & bVar5);
  }
  out(0x43,0x34);
  out(0x40,0xa9);
  out(0x40,4);
  return CONCAT22(in_stack_00000036,in_stack_00000030);
}



/* ===== FORCED FUNCTION forced_1000:69d8 @ 1000:69d8 ===== */

undefined4 forced_1000_69d8(void)

{
  code *pcVar1;
  undefined2 in_AX;
  undefined2 in_DX;
  undefined2 unaff_DS;
  
  if (*(int *)0xa5a0 != 0) {
    pcVar1 = (code *)swi(0x10);
    (*pcVar1)();
  }
  return CONCAT22(in_DX,in_AX);
}


