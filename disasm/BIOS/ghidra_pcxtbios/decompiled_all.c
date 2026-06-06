
/* ===== FUNCTION FUN_f000_e271 @ f000:e271 ===== */

/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */
/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (ram,0x000fe1c7) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_f000_e271(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  code *pcVar3;
  char cVar4;
  char extraout_AH;
  int iVar5;
  undefined2 extraout_DX;
  undefined2 extraout_DX_00;
  byte bVar7;
  int *piVar6;
  undefined2 *puVar8;
  undefined2 *puVar9;
  undefined2 *puVar10;
  undefined2 *puVar11;
  undefined2 *puVar12;
  undefined2 unaff_CS;
  undefined2 uVar13;
  bool bVar14;
  
  _DAT_0000_0472 = 0;
  out(0xa0,0);
  out(0x3d8,0);
  out(0x3b8,1);
  out(99,0x99);
  out(0x61,0xa5);
  out(0x43,0x54);
  out(0x41,0x12);
  out(0x43,0x40);
  out(0x81,0);
  out(0x82,0);
  out(0x83,0);
  out(0xd,0);
  out(0xb,0x58);
  out(0xb,0x41);
  out(0xb,0x42);
  out(0xb,0x43);
  out(1,0xff);
  out(1,0xff);
  out(8,0);
  out(10,0);
  out(0x43,0x36);
  out(0x40,0);
  out(0x40,0);
  out(0x213,1);
  uRam000008fc = 0;
  do {
    puVar12 = (undefined2 *)0x0;
    *(undefined2 *)0x0 = 0x55aa;
    if (*(int *)0x0 != 0x55aa) break;
    for (iVar5 = 0x2000; iVar5 != 0; iVar5 = iVar5 + -1) {
      puVar1 = puVar12;
      puVar12 = puVar12 + 1;
      *puVar1 = 0;
    }
    bVar7 = (char)(uRam000008fc >> 8) + 4;
    uRam000008fc = (uint)bVar7 << 8;
  } while (bVar7 < 0xa0);
  uRam000008fe = 0;
  uRam000008fa = 0xe0ef;
  FUN_f000_f92b();
  _DAT_0000_0472 = 0;
  _DAT_0000_0413 = uRam000008fc >> 6;
  DAT_0000_0415 = (byte)uRam000008fe;
  if ((uRam000008fc >> 5 & 1) != 0) {
    DAT_0000_0415 = DAT_0000_0415 | 0x10;
  }
  bVar14 = true;
  uRam000008fe = 0;
  uRam000008fc = 0;
  uRam000008fa = 0;
  uRam000008f8 = 0;
  uRam000008f6 = 0;
  _DAT_0000_03fe = 0x40;
  puVar8 = (undefined2 *)0xfc;
  _DAT_0000_03fc = 0xe11c;
  FUN_f000_e5c0();
  uVar13 = *puVar8;
  if (!bVar14) {
    *(byte *)0x15 = *(byte *)0x15 | 1;
  }
  out(0x20,0x13);
  out(0x21,8);
  out(0x21,9);
  out(0x21,0xff);
  *puVar8 = uVar13;
  puVar8[-1] = unaff_CS;
  uVar13 = puVar8[-1];
  iVar5 = 8;
  puVar12 = (undefined2 *)0x0;
  do {
    puVar11 = puVar12 + 1;
    *puVar12 = 0xff23;
    puVar12 = puVar12 + 2;
    *puVar11 = unaff_CS;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  puVar11 = (undefined2 *)0xfef3;
  iVar5 = 0x18;
  do {
    puVar2 = puVar12 + 1;
    puVar1 = puVar11;
    puVar11 = puVar11 + 1;
    *puVar12 = *puVar1;
    puVar12 = puVar12 + 2;
    *puVar2 = unaff_CS;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  piVar6 = (int *)0x0;
  do {
    iVar5 = *piVar6;
    if (((char)iVar5 == (char)((uint)iVar5 >> 8)) || (bVar14 = iVar5 == -0x55ab, bVar14))
    goto LAB_f000_e17b;
    puVar9 = (undefined2 *)((int)puVar8 + -2);
    puVar8 = (undefined2 *)((int)puVar8 + -2);
    *puVar9 = 0xe16c;
    FUN_f000_e5c0();
    if (!bVar14) goto LAB_f000_e17b;
  } while (extraout_AH != '\x01');
  uRam00000060 = 0;
  uRam00000062 = 0xf600;
LAB_f000_e17b:
  uVar13 = *puVar8;
  puVar10 = puVar8 + 1;
  uRam00000008 = 0xf85f;
  uRam00000014 = 0xff54;
  uRam0000007c = 0;
  uRam0000007e = 0;
  bVar7 = in(0x61);
  out(0x61,bVar7 | 0x30);
  out(0x61,bVar7 & 0xcf);
  out(0xa0,0x80);
  *(undefined2 *)0x10 = 0x30;
  pcVar3 = (code *)swi(0x10);
  (*pcVar3)();
  *(undefined2 *)0x10 = 0x20;
  pcVar3 = (code *)swi(0x10);
  (*pcVar3)();
  in(0x62);
  out(extraout_DX,0xad);
  *(undefined2 *)0x10 = 0x61;
  *(undefined2 *)((int)puVar10 + -2) = 0xe1d3;
  FUN_f000_f793();
  out(extraout_DX_00,8);
  iVar5 = 0x2956;
  do {
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  out(extraout_DX_00,200);
  out(extraout_DX_00,0x48);
  *(undefined2 *)0x1a = 0x1e;
  *(undefined2 *)0x1c = 0x1e;
  *(undefined2 *)0x80 = 0x1e;
  *(undefined2 *)0x82 = 0x3e;
  *(undefined2 *)0x78 = 0x1414;
  *(undefined2 *)0x7a = 0x1414;
  *(undefined2 *)0x7c = 0x101;
  *(undefined2 *)0x7e = 0x101;
  *(undefined2 *)0x8 = 0x378;
  *(undefined2 *)0xa = 0x278;
  *(undefined1 *)0x11 = 0x80;
  puVar12 = (undefined2 *)0x0;
  out(0x3fb,0x1a);
  out(0xc0,0xff);
  cVar4 = in(0x3fb);
  if (cVar4 == '\x1a') {
    *(undefined2 *)0x0 = 0x3f8;
    puVar12 = (undefined2 *)0x2;
  }
  out(0x2fb,0x1a);
  out(0xc0,0xff);
  cVar4 = in(0x2fb);
  if (cVar4 == '\x1a') {
    *puVar12 = 0x2f8;
    puVar12 = puVar12 + 1;
  }
  *(byte *)0x11 = *(byte *)0x11 | (byte)puVar12;
  iVar5 = 100;
  do {
    cVar4 = in(0x201);
    if (cVar4 != -1) {
      *(byte *)0x11 = *(byte *)0x11 | 0x10;
      break;
    }
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  *(undefined2 *)((int)puVar10 + -2) = 0xe25e;
  FUN_f000_e584();
  *(undefined2 *)((int)puVar10 + -2) = *(undefined2 *)0x72;
  *(undefined2 *)((int)puVar10 + -4) = uVar13;
  bVar7 = in(0x61);
  out(0x61,bVar7 ^ 0xc);
  FUN_f000_e2c6();
  return;
}



/* ===== FUNCTION FUN_f000_e2c6 @ f000:e2c6 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_f000_e2c6(undefined2 param_1)

{
  code *pcVar1;
  byte bVar2;
  char cVar3;
  int in_DX;
  int extraout_DX;
  int iVar4;
  undefined2 unaff_DS;
  undefined1 uVar5;
  bool bVar6;
  undefined2 in_stack_00000000;
  
  if (*(int *)0x0 == -0x55ab) {
    bVar6 = ((uint)*(byte *)0x2 * 0x20 & 0xfff) == 0;
    FUN_f000_e5c3();
    if (bVar6) {
      DAT_0000_0412 = 1;
      _DAT_0000_0467 = 3;
      _DAT_0000_0469 = unaff_DS;
      (*(code *)*(undefined2 *)0x67)();
      iVar4 = extraout_DX;
      goto LAB_f000_e30c;
    }
    DAT_0000_0415 = DAT_0000_0415 | 0x20;
    in_DX = extraout_DX;
  }
  iVar4 = in_DX + 0x80;
LAB_f000_e30c:
  if (iVar4 < -0x200) {
    FUN_f000_e2c6();
    return;
  }
  *(undefined2 *)0x72 = param_1;
  bVar2 = in(0x21);
  out(0x21,bVar2 & 0xbc);
  if (*(char *)0x12 != '\0') {
    FUN_f000_f9b3();
  }
  cVar3 = -1;
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  if (cVar3 == -1) {
    FUN_f000_ef10();
    FUN_f000_f793();
  }
  else {
    *(byte *)0x10 = *(byte *)0x10 & 0xcf;
  }
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  FUN_f000_f9d0();
  *(undefined1 *)0x96 = 0x10;
  if (*(int *)0x72 != 0x1234) goto LAB_f000_e36b;
LAB_f000_e49e:
  FUN_f000_f7af();
  in_stack_00000000 = 0xe4a6;
  FUN_f000_f9d0();
  _DAT_0000_0472 = 0x1234;
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x19);
  (*pcVar1)();
  do {
    *(byte *)0x15 = *(byte *)0x15 | 2;
    do {
      FUN_f000_e4ed();
      FUN_f000_e4e2();
      FUN_f000_e4fa();
      FUN_f000_e4e2();
      FUN_f000_f7af();
      cVar3 = FUN_f000_feee();
      FUN_f000_e4d7();
      if ((cVar3 != 'Y') && (cVar3 != 'y')) {
        FUN_f000_e271();
        return;
      }
      *(undefined1 *)0x15 = 0;
      FUN_f000_f9d0();
LAB_f000_e36b:
      FUN_f000_f9ea();
      FUN_f000_e4e2();
    } while (*(char *)0x15 != '\0');
    FUN_f000_e4ed();
    FUN_f000_e566();
    FUN_f000_e4ed();
    FUN_f000_e4e2();
    bVar2 = *(byte *)0x11;
    if (bVar2 >> 6 != 0) {
      FUN_f000_e51a();
    }
    if ((bVar2 >> 1 & 3) != 0) {
      FUN_f000_e51a();
    }
    uVar5 = 0;
    if ((bVar2 & 0x10) != 0) {
      FUN_f000_e4ed();
      FUN_f000_e4e2();
    }
    FUN_f000_e5a3();
    if (!(bool)uVar5) {
      FUN_f000_e4ed();
      FUN_f000_e4e2();
    }
    FUN_f000_e4ed();
    FUN_f000_e4e2();
    FUN_f000_e4ed();
    FUN_f000_e4e2();
    iVar4 = *(int *)0x13 + -2;
    while( true ) {
      uVar5 = 0;
      FUN_f000_e4ed();
      FUN_f000_e53c();
      FUN_f000_f92b();
      if ((bool)uVar5) break;
      iVar4 = iVar4 + -1;
      if (iVar4 == 0) {
        *(undefined2 *)0x1a = 0x1e;
        *(undefined2 *)0x1c = 0x1e;
        FUN_f000_e5cd();
        goto LAB_f000_e49e;
      }
    }
  } while( true );
}



/* ===== FUNCTION FUN_f000_e4d7 @ f000:e4d7 ===== */

undefined2 __cdecl16near FUN_f000_e4d7(void)

{
  code *pcVar1;
  undefined2 in_stack_00000000;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return in_stack_00000000;
}



/* ===== FUNCTION FUN_f000_e4e2 @ f000:e4e2 ===== */

void __cdecl16near FUN_f000_e4e2(void)

{
  char *pcVar1;
  char *unaff_SI;
  undefined2 unaff_DS;
  
  while (pcVar1 = unaff_SI, unaff_SI = unaff_SI + 1, *pcVar1 != '\0') {
    FUN_f000_e4d7();
  }
  return;
}



/* ===== FUNCTION FUN_f000_e4ed @ f000:e4ed ===== */

void __cdecl16near FUN_f000_e4ed(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_f000_e4fa @ f000:e4fa ===== */

void __cdecl16near FUN_f000_e4fa(void)

{
  FUN_f000_e6e2();
  FUN_f000_e6e2();
  return;
}



/* ===== FUNCTION FUN_f000_e507 @ f000:e507 ===== */

void __cdecl16near FUN_f000_e507(void)

{
  FUN_f000_e6e2();
  FUN_f000_e4fa();
  return;
}



/* ===== FUNCTION FUN_f000_e512 @ f000:e512 ===== */

void __cdecl16near FUN_f000_e512(void)

{
  FUN_f000_e4fa();
  FUN_f000_e4fa();
  return;
}



/* ===== FUNCTION FUN_f000_e51a @ f000:e51a ===== */

void __cdecl16near FUN_f000_e51a(void)

{
  char extraout_DL;
  undefined2 unaff_SI;
  
  do {
    FUN_f000_e4ed();
    FUN_f000_e4e2(unaff_SI);
    FUN_f000_e507();
    FUN_f000_e4e2();
  } while (extraout_DL != '\x01');
  return;
}



/* ===== FUNCTION FUN_f000_e53c @ f000:e53c ===== */

void __cdecl16near FUN_f000_e53c(void)

{
  char in_CH;
  
  if (in_CH != '\0') {
    FUN_f000_e6e2();
    FUN_f000_e6e2();
    FUN_f000_e6e2();
  }
  return;
}



/* ===== FUNCTION FUN_f000_e566 @ f000:e566 ===== */

void __cdecl16near FUN_f000_e566(void)

{
  FUN_f000_e4e2();
  FUN_f000_f91b();
  FUN_f000_e4e2();
  FUN_f000_e4e2();
  return;
}



/* ===== FUNCTION FUN_f000_e584 @ f000:e584 ===== */

void __cdecl16near FUN_f000_e584(void)

{
  undefined2 unaff_DS;
  
  *(undefined1 *)0x201 = 0;
  *(undefined2 *)0x200 = 0x37f;
  if (*(char *)0x201 == '\x03') {
    *(byte *)0x10 = *(byte *)0x10 | 2;
    return;
  }
  *(byte *)0x10 = *(byte *)0x10 & 0xfd;
  return;
}



/* ===== FUNCTION FUN_f000_e5a3 @ f000:e5a3 ===== */

void __cdecl16near FUN_f000_e5a3(void)

{
  byte bVar1;
  
  bVar1 = in(0x2c1);
  if (((0x99 < bVar1) && (bVar1 = in(0x241), 0x99 < bVar1)) && (bVar1 = in(0x341), 0x99 < bVar1)) {
    return;
  }
  return;
}



/* ===== FUNCTION FUN_f000_e5c0 @ f000:e5c0 ===== */

char FUN_f000_e5c0(void)

{
  char cVar1;
  int iVar2;
  char *in_BX;
  undefined2 unaff_DS;
  
  iVar2 = 0x2000;
  cVar1 = '\0';
  do {
    cVar1 = cVar1 + *in_BX;
    in_BX = in_BX + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  return cVar1;
}



/* ===== FUNCTION FUN_f000_e5c3 @ f000:e5c3 ===== */

char __cdecl16near FUN_f000_e5c3(void)

{
  char cVar1;
  int in_CX;
  char *in_BX;
  undefined2 unaff_DS;
  
  cVar1 = '\0';
  do {
    cVar1 = cVar1 + *in_BX;
    in_BX = in_BX + 1;
    in_CX = in_CX + -1;
  } while (in_CX != 0);
  return cVar1;
}



/* ===== FUNCTION FUN_f000_e5cd @ f000:e5cd ===== */

/* WARNING: Control flow encountered bad instruction data */

byte __cdecl16near FUN_f000_e5cd(void)

{
  code *pcVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  uint uVar5;
  undefined2 unaff_CS;
  bool bVar6;
  undefined1 uVar7;
  
  FUN_f000_e4ed();
  FUN_f000_e4e2();
  uVar5 = 0x12;
  bVar2 = FUN_f000_f9b3();
  if (bVar2 != 0x20) {
    return bVar2;
  }
  pcVar1 = (code *)swi(0x18);
  (*pcVar1)();
  uRam00000078 = 0xefc7;
  cVar3 = '\x06';
  cVar4 = '\0';
  uRam0000007a = unaff_CS;
  while( true ) {
    do {
      bVar6 = false;
      pcVar1 = (code *)swi(0x13);
      (*pcVar1)();
      if (!bVar6) {
        bVar6 = false;
        uVar5 = 0x7c00;
        pcVar1 = (code *)swi(0x13);
        (*pcVar1)();
        if (!bVar6) {
                    /* WARNING: Bad instruction - Truncating control flow here */
          halt_baddata();
        }
      }
      cVar3 = cVar3 + -1;
    } while (cVar3 != '\0');
    if ((cVar4 != '\0') && (cRam00000063 == -10)) break;
    FUN_f000_e4e2();
    FUN_f000_feee();
    cVar3 = '\x06';
    cVar4 = -1;
  }
  pcVar1 = (code *)swi(0x18);
  bVar2 = (*pcVar1)();
  if (bVar2 == 0xe0) {
    uVar5 = uVar5 | 2;
  }
  else if ((bVar2 == 0x57) || (bVar2 == 0x58)) {
    bVar2 = 0;
  }
  else if ((uVar5 & 2) != 0) {
    if (bVar2 == 0xaa) {
      bVar2 = (byte)((uVar5 & 0xfffd) >> 8) | 0x80;
    }
    uVar5 = CONCAT11(bVar2,(char)(uVar5 & 0xfffd));
    if (((((bVar2 & 0x7f) != 0x2a) && ((bVar2 & 0x7f) != 0x36)) && (bVar2 != 0x35)) &&
       (((bVar2 != 0x1c && (bVar2 != 0x1d)) &&
        ((bVar2 != 0x38 && (uVar7 = bVar2 < 0x46, bVar2 != 0x46)))))) {
      FUN_f000_ef3c();
      bVar2 = FUN_f000_ebb5();
      if (!(bool)uVar7) {
        bVar2 = 0xe0;
      }
    }
  }
  uRam00000096 = uVar5;
  return bVar2;
}



/* ===== FUNCTION FUN_f000_e660 @ f000:e660 ===== */

void __cdecl16near FUN_f000_e660(void)

{
  byte in_AL;
  uint in_BX;
  undefined2 unaff_DS;
  
  if (in_AL == 0xe0) {
    in_BX = in_BX | 2;
  }
  else if (((in_AL != 0x57) && (in_AL != 0x58)) && ((in_BX & 2) != 0)) {
    if (in_AL == 0xaa) {
      in_AL = (byte)((in_BX & 0xfffd) >> 8) | 0x80;
    }
    in_BX = CONCAT11(in_AL,(char)(in_BX & 0xfffd));
    if ((((((in_AL & 0x7f) != 0x2a) && ((in_AL & 0x7f) != 0x36)) &&
         ((in_AL != 0x35 && ((in_AL != 0x1c && (in_AL != 0x1d)))))) && (in_AL != 0x38)) &&
       (in_AL != 0x46)) {
      FUN_f000_ef3c();
      FUN_f000_ebb5();
    }
  }
  *(uint *)0x96 = in_BX;
  return;
}



/* ===== FUNCTION FUN_f000_e6e2 @ f000:e6e2 ===== */

undefined2 __cdecl16near FUN_f000_e6e2(void)

{
  undefined2 in_AX;
  
  FUN_f000_e4d7();
  return in_AX;
}



/* ===== FUNCTION FUN_f000_e7ce @ f000:e7ce ===== */

void __cdecl16near FUN_f000_e7ce(void)

{
  byte bVar1;
  int iVar2;
  undefined2 in_DX;
  char cVar3;
  byte in_BH;
  int unaff_DI;
  undefined2 unaff_DS;
  
  cVar3 = *(char *)(unaff_DI + 0x7c);
  do {
    iVar2 = 0;
    do {
      bVar1 = in(in_DX);
      if ((bVar1 & in_BH) == in_BH) {
        return;
      }
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    cVar3 = cVar3 + -1;
  } while (cVar3 != '\0');
  return;
}



/* ===== FUNCTION FUN_f000_e7e5 @ f000:e7e5 ===== */

void __cdecl16near FUN_f000_e7e5(void)

{
  undefined1 in_AL;
  int in_DX;
  bool bVar1;
  
  out(in_DX + 4,in_AL);
  bVar1 = in_DX == -6;
  FUN_f000_e7ce();
  if (bVar1) {
    FUN_f000_e7ce();
  }
  return;
}



/* ===== FUNCTION FUN_f000_ebb5 @ f000:ebb5 ===== */

void __cdecl16near FUN_f000_ebb5(void)

{
  char in_AH;
  undefined2 unaff_DS;
  
  if (in_AH == -0x2e) {
    *(byte *)0x18 = *(byte *)0x18 & 0x7f;
  }
  else if (-1 < in_AH) {
    if ((in_AH == 'R') && ((*(byte *)0x18 & 0x80) == 0)) {
      *(byte *)0x17 = *(byte *)0x17 ^ 0x80;
      *(byte *)0x18 = *(byte *)0x18 | 0x80;
    }
    return;
  }
  return;
}



/* ===== FUNCTION FUN_f000_ebdd @ f000:ebdd ===== */

void __cdecl16near FUN_f000_ebdd(void)

{
  undefined2 in_AX;
  undefined2 *puVar1;
  undefined2 unaff_DS;
  
  puVar1 = (undefined2 *)*(int *)0x1c + 1;
  if (puVar1 == (undefined2 *)*(int *)0x82) {
    puVar1 = (undefined2 *)*(undefined2 *)0x80;
  }
  if (puVar1 == (undefined2 *)*(int *)0x1a) {
    return;
  }
  *(undefined2 *)*(int *)0x1c = in_AX;
  *(undefined2 *)0x1c = puVar1;
  return;
}



/* ===== FUNCTION FUN_f000_eca8 @ f000:eca8 ===== */

byte __cdecl16near FUN_f000_eca8(void)

{
  byte bVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  byte bVar6;
  int iVar5;
  int iVar7;
  byte *unaff_BP;
  int unaff_SI;
  undefined2 unaff_ES;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  
  if (unaff_BP[1] != 0) {
    bVar1 = unaff_BP[1] - 1;
    if (bVar1 == 0) {
      return *(byte *)0x41;
    }
    if ((3 < unaff_BP[2]) || (5 < bVar1)) {
      *(undefined1 *)0x41 = 1;
      return bVar1;
    }
    bVar1 = unaff_BP[1];
    *(undefined1 *)0x41 = 0;
    out(0xc,bVar1);
    out(0xb,*(undefined1 *)(bVar1 + 0xed2a));
    uVar4 = *(int *)(unaff_BP + 0xc) * 0x10;
    uVar3 = uVar4 + *(uint *)(unaff_BP + 10);
    out(4,(char)uVar3);
    out(4,(char)(uVar3 >> 8));
    out(0x81,(byte)((uint)*(int *)(unaff_BP + 0xc) >> 0xc) + CARRY2(uVar4,*(uint *)(unaff_BP + 10)))
    ;
    uVar4 = ((((uint)*unaff_BP << 8) >> 1) << (unaff_BP[6] & 0x1f)) - 1;
    out(5,(char)uVar4);
    out(5,(char)(uVar4 >> 8));
    if (CARRY2(uVar4,uVar3)) {
      *(undefined1 *)0x41 = 9;
      return 0;
    }
    out(10,2);
    *(undefined1 *)0x40 = 0xff;
    bVar6 = *(byte *)(unaff_BP[2] + 0xed36);
    out(0x3f2,bVar6 << 4 | unaff_BP[2] | 0xc);
    bVar1 = *(byte *)(bVar1 + 0xed30);
    *(byte *)0x3f = *(byte *)0x3f | bVar1;
    if ((((char)bVar1 < '\0') && (*(char *)(unaff_SI + 10) != '\0')) &&
       ((*(byte *)0x3f & bVar6) == 0)) {
      FUN_f000_ef05();
    }
    *(byte *)0x3f = *(byte *)0x3f | bVar6;
    if ((*(byte *)0x3e & bVar6) == 0) {
      *(byte *)0x3e = *(byte *)0x3e | bVar6;
      FUN_f000_ef92();
      FUN_f000_ef92();
      FUN_f000_eeb2();
      FUN_f000_efb9();
    }
    FUN_f000_ef92();
    FUN_f000_ef92();
    FUN_f000_ef92();
    FUN_f000_eeb2();
    FUN_f000_ef6a();
    for (cVar2 = *(char *)(unaff_SI + 9); cVar2 != '\0'; cVar2 = cVar2 + -1) {
      iVar5 = 0x226;
      do {
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    FUN_f000_ef92();
    FUN_f000_ef92();
    if (unaff_BP[1] == 5) {
      FUN_f000_ef92();
      FUN_f000_ef92();
      FUN_f000_ef92();
      FUN_f000_ef92();
    }
    else {
      iVar5 = 7;
      do {
        FUN_f000_ef92();
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    }
    FUN_f000_eeb2();
    FUN_f000_ef6f();
    if ((*(byte *)0x42 & 0xc0) != 0) {
      if ((*(byte *)0x42 & 0xc0) == 0x40) {
        iVar5 = 6;
        iVar7 = 0;
        do {
          if ((*(byte *)(iVar7 + -0x12c6) & *(byte *)0x43) != 0) break;
          iVar7 = iVar7 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
        *(undefined1 *)0x41 = *(undefined1 *)(iVar7 + -0x12c0);
      }
      else {
        *(undefined1 *)0x41 = 0x20;
      }
    }
    cVar2 = *(char *)0x47;
    if (*(byte *)0x45 != unaff_BP[3]) {
      cVar2 = unaff_BP[7] + 1;
    }
    return cVar2 - unaff_BP[5];
  }
  *(byte *)0x3f = *(byte *)0x3f & 0xf;
  bVar1 = *(char *)0x3f * '\x10';
  if ((bVar1 & 0x20) == 0) {
    if ((bVar1 & 0x40) == 0) {
      if ((bVar1 & 0x80) == 0) goto LAB_f000_ecec;
      bVar1 = bVar1 + 1;
    }
    bVar1 = bVar1 + 1;
  }
  bVar1 = bVar1 + 1;
LAB_f000_ecec:
  *(undefined1 *)0x3e = 0;
  *(undefined1 *)0x41 = 0;
  out(0x3f2,bVar1 | 8);
  out(0x3f2,bVar1 | 0xc);
  FUN_f000_eeb2();
  FUN_f000_ef6a();
  bVar1 = *(byte *)0x42;
  if (bVar1 == 0xc0) {
    FUN_f000_ef92();
    FUN_f000_ef92();
    bVar1 = FUN_f000_ef92();
  }
  else {
    *(undefined1 *)0x41 = 0x20;
  }
  return bVar1;
}



/* ===== FUNCTION FUN_f000_eeb2 @ f000:eeb2 ===== */

char __cdecl16near FUN_f000_eeb2(void)

{
  char cVar1;
  int iVar2;
  undefined2 unaff_DS;
  
  iVar2 = 0;
  cVar1 = '\x02';
  do {
    if ((*(byte *)0x3e & 0x80) != 0) {
      *(byte *)0x3e = *(byte *)0x3e & 0x7f;
      return cVar1;
    }
    iVar2 = iVar2 + -1;
  } while ((iVar2 != 0) || (cVar1 = cVar1 + -1, cVar1 != '\0'));
  *(undefined1 *)0x41 = 0x80;
  return '\0';
}



/* ===== FUNCTION FUN_f000_eed5 @ f000:eed5 ===== */

undefined4 __cdecl16near FUN_f000_eed5(void)

{
  byte bVar1;
  undefined1 uVar2;
  undefined2 in_AX;
  undefined1 uVar3;
  int iVar4;
  undefined2 unaff_DS;
  
  iVar4 = 0;
  do {
    uVar3 = (undefined1)((uint)in_AX >> 8);
    bVar1 = in(0x3f4);
    in_AX = CONCAT11(uVar3,bVar1);
    if ((char)bVar1 < '\0') {
      if ((bVar1 & 0x40) != 0) {
        uVar2 = in(0x3f5);
        iVar4 = 10;
        do {
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
        in(0x3f4);
        return CONCAT22(0x3f4,CONCAT11(uVar3,uVar2));
      }
      *(undefined1 *)0x41 = 0x20;
      goto LAB_f000_eef2;
    }
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  *(undefined1 *)0x41 = 0x80;
LAB_f000_eef2:
  return CONCAT22(0x3f4,in_AX);
}



/* ===== FUNCTION FUN_f000_ef05 @ f000:ef05 ===== */

void __cdecl16near FUN_f000_ef05(void)

{
  char in_AH;
  int iVar1;
  
  do {
    iVar1 = 0;
    do {
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    in_AH = in_AH + -1;
  } while (in_AH != '\0');
  return;
}



/* ===== FUNCTION FUN_f000_ef10 @ f000:ef10 ===== */

void __cdecl16near FUN_f000_ef10(void)

{
  char in_AL;
  int iVar1;
  undefined2 unaff_DS;
  undefined1 uVar2;
  
  iVar1 = 4;
  if (in_AL != '\a') {
    iVar1 = 0x10;
  }
  uVar2 = 0xfffb < *(uint *)0x63;
  out(*(uint *)0x63 + 4,*(byte *)0x65 & 0xf7);
  do {
    FUN_f000_f92b();
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  if ((bool)uVar2) {
    *(byte *)0x15 = *(byte *)0x15 | 4;
  }
  return;
}



/* ===== FUNCTION FUN_f000_ef3c @ f000:ef3c ===== */

/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */
/* WARNING: This function may have set the stack pointer */
/* WARNING: Removing unreachable block (ram,0x000fe1c7) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl16near FUN_f000_ef3c(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  code *pcVar3;
  char cVar4;
  char in_AL;
  char extraout_AH;
  int iVar5;
  undefined2 extraout_DX;
  undefined2 extraout_DX_00;
  byte bVar7;
  int *piVar6;
  undefined2 *puVar8;
  undefined2 *puVar9;
  undefined2 *puVar10;
  undefined2 *puVar11;
  undefined2 *puVar12;
  undefined2 unaff_CS;
  undefined2 uVar13;
  undefined2 unaff_DS;
  bool bVar14;
  
  if (((in_AL != 'S') || ((*(byte *)0x17 & 4) == 0)) || ((*(byte *)0x17 & 8) == 0)) {
    return;
  }
  *(undefined2 *)0x72 = 0x1234;
  uVar13 = _DAT_0000_0472;
  out(0xa0,0);
  out(0x3d8,0);
  out(0x3b8,1);
  out(99,0x99);
  out(0x61,0xa5);
  out(0x43,0x54);
  out(0x41,0x12);
  out(0x43,0x40);
  out(0x81,0);
  out(0x82,0);
  out(0x83,0);
  out(0xd,0);
  out(0xb,0x58);
  out(0xb,0x41);
  out(0xb,0x42);
  out(0xb,0x43);
  out(1,0xff);
  out(1,0xff);
  out(8,0);
  out(10,0);
  out(0x43,0x36);
  out(0x40,0);
  out(0x40,0);
  out(0x213,1);
  uRam000008fc = 0;
  do {
    puVar12 = (undefined2 *)0x0;
    *(undefined2 *)0x0 = 0x55aa;
    if (*(int *)0x0 != 0x55aa) break;
    for (iVar5 = 0x2000; iVar5 != 0; iVar5 = iVar5 + -1) {
      puVar1 = puVar12;
      puVar12 = puVar12 + 1;
      *puVar1 = 0;
    }
    bVar7 = (char)(uRam000008fc >> 8) + 4;
    uRam000008fc = (uint)bVar7 << 8;
  } while (bVar7 < 0xa0);
  uRam000008fe = 0;
  uRam000008fa = 0xe0ef;
  FUN_f000_f92b();
  _DAT_0000_0413 = uRam000008fc >> 6;
  DAT_0000_0415 = (byte)uRam000008fe;
  if ((uRam000008fc >> 5 & 1) != 0) {
    DAT_0000_0415 = DAT_0000_0415 | 0x10;
  }
  bVar14 = true;
  uRam000008fe = 0;
  uRam000008fc = 0;
  uRam000008fa = 0;
  uRam000008f8 = 0;
  uRam000008f6 = 0;
  _DAT_0000_03fe = 0x40;
  puVar8 = (undefined2 *)0xfc;
  _DAT_0000_03fc = 0xe11c;
  _DAT_0000_0472 = uVar13;
  FUN_f000_e5c0();
  uVar13 = *puVar8;
  if (!bVar14) {
    *(byte *)0x15 = *(byte *)0x15 | 1;
  }
  out(0x20,0x13);
  out(0x21,8);
  out(0x21,9);
  out(0x21,0xff);
  *puVar8 = uVar13;
  puVar8[-1] = unaff_CS;
  uVar13 = puVar8[-1];
  iVar5 = 8;
  puVar12 = (undefined2 *)0x0;
  do {
    puVar11 = puVar12 + 1;
    *puVar12 = 0xff23;
    puVar12 = puVar12 + 2;
    *puVar11 = unaff_CS;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  puVar11 = (undefined2 *)0xfef3;
  iVar5 = 0x18;
  do {
    puVar2 = puVar12 + 1;
    puVar1 = puVar11;
    puVar11 = puVar11 + 1;
    *puVar12 = *puVar1;
    puVar12 = puVar12 + 2;
    *puVar2 = unaff_CS;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  piVar6 = (int *)0x0;
  do {
    iVar5 = *piVar6;
    if (((char)iVar5 == (char)((uint)iVar5 >> 8)) || (bVar14 = iVar5 == -0x55ab, bVar14))
    goto LAB_f000_e17b;
    puVar9 = (undefined2 *)((int)puVar8 + -2);
    puVar8 = (undefined2 *)((int)puVar8 + -2);
    *puVar9 = 0xe16c;
    FUN_f000_e5c0();
    if (!bVar14) goto LAB_f000_e17b;
  } while (extraout_AH != '\x01');
  uRam00000060 = 0;
  uRam00000062 = 0xf600;
LAB_f000_e17b:
  uVar13 = *puVar8;
  puVar10 = puVar8 + 1;
  uRam00000008 = 0xf85f;
  uRam00000014 = 0xff54;
  uRam0000007c = 0;
  uRam0000007e = 0;
  bVar7 = in(0x61);
  out(0x61,bVar7 | 0x30);
  out(0x61,bVar7 & 0xcf);
  out(0xa0,0x80);
  *(undefined2 *)0x10 = 0x30;
  pcVar3 = (code *)swi(0x10);
  (*pcVar3)();
  *(undefined2 *)0x10 = 0x20;
  pcVar3 = (code *)swi(0x10);
  (*pcVar3)();
  in(0x62);
  out(extraout_DX,0xad);
  *(undefined2 *)0x10 = 0x61;
  *(undefined2 *)((int)puVar10 + -2) = 0xe1d3;
  FUN_f000_f793();
  out(extraout_DX_00,8);
  iVar5 = 0x2956;
  do {
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  out(extraout_DX_00,200);
  out(extraout_DX_00,0x48);
  *(undefined2 *)0x1a = 0x1e;
  *(undefined2 *)0x1c = 0x1e;
  *(undefined2 *)0x80 = 0x1e;
  *(undefined2 *)0x82 = 0x3e;
  *(undefined2 *)0x78 = 0x1414;
  *(undefined2 *)0x7a = 0x1414;
  *(undefined2 *)0x7c = 0x101;
  *(undefined2 *)0x7e = 0x101;
  *(undefined2 *)0x8 = 0x378;
  *(undefined2 *)0xa = 0x278;
  *(undefined1 *)0x11 = 0x80;
  puVar12 = (undefined2 *)0x0;
  out(0x3fb,0x1a);
  out(0xc0,0xff);
  cVar4 = in(0x3fb);
  if (cVar4 == '\x1a') {
    *(undefined2 *)0x0 = 0x3f8;
    puVar12 = (undefined2 *)0x2;
  }
  out(0x2fb,0x1a);
  out(0xc0,0xff);
  cVar4 = in(0x2fb);
  if (cVar4 == '\x1a') {
    *puVar12 = 0x2f8;
    puVar12 = puVar12 + 1;
  }
  *(byte *)0x11 = *(byte *)0x11 | (byte)puVar12;
  iVar5 = 100;
  do {
    cVar4 = in(0x201);
    if (cVar4 != -1) {
      *(byte *)0x11 = *(byte *)0x11 | 0x10;
      break;
    }
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  *(undefined2 *)((int)puVar10 + -2) = 0xe25e;
  FUN_f000_e584();
  *(undefined2 *)((int)puVar10 + -2) = *(undefined2 *)0x72;
  *(undefined2 *)((int)puVar10 + -4) = uVar13;
  bVar7 = in(0x61);
  out(0x61,bVar7 ^ 0xc);
  FUN_f000_e2c6();
  return;
}



/* ===== FUNCTION FUN_f000_ef6a @ f000:ef6a ===== */

uint FUN_f000_ef6a(void)

{
  uint uVar1;
  undefined2 in_CX;
  int iVar2;
  undefined2 in_BX;
  int iVar3;
  undefined2 unaff_DS;
  bool bVar4;
  bool bVar5;
  uint in_stack_00000000;
  
  FUN_f000_ef92();
  iVar2 = 7;
  iVar3 = 0;
  bVar5 = true;
  do {
    bVar4 = false;
    uVar1 = FUN_f000_eed5(in_CX,in_BX);
    if (bVar4) goto LAB_f000_ef88;
    *(undefined1 *)(iVar3 + 0x42) = (char)uVar1;
    if (bVar5) {
      return uVar1;
    }
    iVar3 = iVar3 + 1;
    bVar5 = iVar3 == 0;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(undefined1 *)0x41 = 0x20;
LAB_f000_ef88:
  return in_stack_00000000 & 0xff00;
}



/* ===== FUNCTION FUN_f000_ef6f @ f000:ef6f ===== */

uint __cdecl16near FUN_f000_ef6f(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined2 unaff_DS;
  bool bVar4;
  bool bVar5;
  uint in_stack_00000000;
  
  iVar2 = 7;
  iVar3 = 0;
  bVar5 = true;
  do {
    bVar4 = false;
    uVar1 = FUN_f000_eed5();
    if (bVar4) goto LAB_f000_ef88;
    *(undefined1 *)(iVar3 + 0x42) = (char)uVar1;
    if (bVar5) {
      return uVar1;
    }
    iVar3 = iVar3 + 1;
    bVar5 = iVar3 == 0;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(undefined1 *)0x41 = 0x20;
LAB_f000_ef88:
  return in_stack_00000000 & 0xff00;
}



/* ===== FUNCTION FUN_f000_ef92 @ f000:ef92 ===== */

undefined2 FUN_f000_ef92(void)

{
  byte bVar1;
  undefined2 in_AX;
  undefined2 uVar2;
  int iVar3;
  undefined2 unaff_DS;
  
  iVar3 = 0;
  do {
    bVar1 = in(0x3f4);
    if ((char)bVar1 < '\0') {
      if ((bVar1 & 0x40) != 0) {
        *(undefined1 *)0x41 = 0x20;
        uVar2 = FUN_f000_efba();
        return uVar2;
      }
      out(0x3f5,(char)in_AX);
      return in_AX;
    }
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  *(undefined1 *)0x41 = 0x80;
  uVar2 = FUN_f000_efba();
  return uVar2;
}



/* ===== FUNCTION FUN_f000_efb9 @ f000:efb9 ===== */

void __cdecl16near FUN_f000_efb9(void)

{
  return;
}



/* ===== FUNCTION FUN_f000_efba @ f000:efba ===== */

ulong __cdecl16near FUN_f000_efba(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  return CONCAT22(param_1,param_3) & 0xffffff00;
}



/* ===== FUNCTION FUN_f000_f097 @ f000:f097 ===== */

undefined4 __cdecl16near FUN_f000_f097(void)

{
  byte in_AL;
  undefined2 in_DX;
  int in_BX;
  
  return CONCAT22(in_DX,(uint)in_AL * in_BX);
}



/* ===== FUNCTION FUN_f000_f104 @ f000:f104 ===== */

uint __cdecl16near FUN_f000_f104(void)

{
  byte *pbVar1;
  char *pcVar2;
  undefined2 *puVar3;
  byte *pbVar4;
  char *pcVar5;
  undefined2 *puVar6;
  uint *puVar7;
  uint uVar8;
  code *pcVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  uint in_AX;
  char cVar18;
  undefined2 uVar14;
  int iVar15;
  undefined1 *puVar16;
  byte bVar19;
  uint uVar17;
  undefined2 in_CX;
  int iVar20;
  uint uVar21;
  int iVar22;
  byte extraout_DL;
  char extraout_DH;
  undefined2 extraout_DX;
  char cVar23;
  undefined2 *unaff_BP;
  byte *unaff_SI;
  undefined2 *puVar24;
  undefined2 *puVar25;
  undefined2 *puVar26;
  byte *pbVar27;
  uint *puVar28;
  uint *puVar29;
  byte *pbVar30;
  byte *pbVar31;
  byte *pbVar32;
  char *pcVar33;
  char *pcVar34;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  undefined2 uVar35;
  bool bVar36;
  undefined1 uVar37;
  bool bVar38;
  undefined4 uVar39;
  int in_stack_00000000;
  byte local_8 [2];
  undefined2 *puStack_6;
  undefined2 *puStack_4;
  
  bVar13 = (byte)(in_AX >> 8);
  if (0xf < bVar13) {
    return in_AX;
  }
  bVar36 = (int)in_AX < 0;
  cVar18 = bVar13 << 1;
  bVar38 = cVar18 == '\0';
  iVar15 = -0x1000;
  switch(bVar13) {
  case 0:
    uVar35 = 0x3b4;
    uVar37 = 1;
    bVar13 = 7;
    if ((*(byte *)0x10 & 0x30) != 0x30) {
      bVar13 = *(byte *)(unaff_BP + 1);
      if (7 < bVar13) {
        return 1;
      }
      uVar35 = 0x3d4;
      uVar37 = 0;
    }
    *(undefined2 *)0x63 = uVar35;
    out(CONCAT11((char)((uint)uVar35 >> 8),(char)uVar35 + '\x04'),uVar37);
    *(byte *)0x49 = bVar13;
    uVar17 = (uint)bVar13;
    iVar15 = 0x10;
    do {
      puStack_4 = (undefined2 *)0xf15a;
      FUN_f000_f784();
      iVar15 = iVar15 + -1;
    } while (iVar15 != 0);
    uVar35 = *unaff_BP;
    bVar36 = false;
    puVar25 = (undefined2 *)0x0;
    bVar38 = true;
    puStack_4 = (undefined2 *)0xf169;
    FUN_f000_f6e6();
    iVar20 = CONCAT11(0x20,(char)iVar15);
    uVar14 = 0;
    if (!bVar36) {
      if (bVar38) {
        iVar20 = CONCAT11(8,(char)iVar15);
      }
      uVar14 = 0x720;
    }
    for (; iVar20 != 0; iVar20 = iVar20 + -1) {
      puVar3 = puVar25;
      puVar25 = puVar25 + 1;
      *puVar3 = uVar14;
    }
    iVar15 = CONCAT11((char)((uint)*(undefined2 *)0x63 >> 8),(char)*(undefined2 *)0x63 + '\x04');
    out(iVar15,*(undefined1 *)(uVar17 - 0xf0c));
    *(undefined1 *)0x65 = *(undefined1 *)(uVar17 - 0xf0c);
    uVar37 = 0x30;
    if (bVar13 == 6) {
      uVar37 = 0x3f;
    }
    *(undefined1 *)0x66 = uVar37;
    out(iVar15 + 1,uVar37);
    *(undefined2 *)0x4e = 0;
    *(undefined1 *)0x62 = 0;
    iVar15 = 8;
    puVar25 = (undefined2 *)0x50;
    do {
      *puVar25 = 0;
      puVar25 = (undefined2 *)((int)puVar25 + 1);
      iVar15 = iVar15 + -1;
    } while (iVar15 != 0);
    *(uint *)0x4a = (uint)*(byte *)(uVar17 - 0xf14);
    uVar17 = *(uint *)((uVar17 & 0xfffe) - 0xf1c);
    *(uint *)0x4c = uVar17;
    return uVar17;
  case 1:
    *(undefined2 *)0x60 = unaff_BP[3];
    uVar17 = FUN_f000_f77b();
    return uVar17;
  case 2:
    *(undefined2 *)((byte)(*(char *)((int)unaff_BP + 5) << 1) + 0x50) = unaff_BP[4];
    uVar17 = FUN_f000_f766();
    return uVar17;
  case 3:
    unaff_BP[4] = *(undefined2 *)((byte)(*(char *)((int)unaff_BP + 5) << 1) + 0x50);
    uVar17 = *(uint *)0x60;
    unaff_BP[3] = uVar17;
    return uVar17;
  case 4:
    uVar35 = *(undefined2 *)0x63;
    *(undefined1 *)((int)unaff_BP + 3) = 0;
    bVar13 = in(CONCAT11((char)((uint)uVar35 >> 8),(char)uVar35 + '\x06'));
    uVar17 = CONCAT11(cVar18,bVar13);
    if ((bVar13 & 4) != 0) {
      if ((bVar13 & 2) == 0) {
        return uVar17;
      }
      FUN_f000_f721();
      uVar17 = CONCAT11((char)((uint)in_CX >> 8),*(byte *)0x49);
      uVar21 = (uint)*(byte *)(*(byte *)0x49 - 0xe11);
      bVar36 = -1 < (int)(uVar17 - uVar21) && uVar17 < uVar21;
      uVar39 = FUN_f000_f6e6();
      uVar17 = (uint)uVar39;
      if (bVar36) {
        uVar21 = (uint)((ulong)uVar39 >> 0x10) & 0xff;
        uVar8 = uVar17 % uVar21;
        bVar13 = (byte)(uVar17 / uVar21);
        cVar23 = (char)uVar8;
        uVar17 = CONCAT11(cVar23,bVar13);
        iVar15 = uVar8 << 3;
        cVar18 = bVar13 << 1;
        uVar35 = CONCAT11(bVar13 >> 2,cVar23);
        if (*(char *)0x49 == '\x06') {
          uVar35 = CONCAT11(bVar13 >> 2,cVar23 << 1);
          iVar15 = uVar8 << 4;
        }
      }
      else {
        uVar21 = uVar17 % (uint)*(byte *)0x4a;
        cVar18 = (char)(uVar17 / *(byte *)0x4a);
        uVar37 = (undefined1)uVar21;
        uVar35 = CONCAT11(cVar18,uVar37);
        cVar18 = cVar18 << 3;
        uVar17 = CONCAT11(cVar18,uVar37);
        iVar15 = uVar21 << 3;
      }
      *(undefined1 *)((int)unaff_BP + 3) = 1;
      unaff_BP[4] = uVar35;
      unaff_BP[2] = iVar15;
      *(char *)((int)unaff_BP + 7) = cVar18;
    }
    out(*(int *)0x63 + 7,(char)uVar17);
    return uVar17;
  case 5:
    bVar13 = *(byte *)(unaff_BP + 1);
    *(byte *)0x62 = bVar13;
    *(int *)0x4e = (uint)bVar13 * *(int *)0x4c;
    puStack_4 = (undefined2 *)0xf293;
    FUN_f000_f77b();
    uVar17 = FUN_f000_f76e();
    return uVar17;
  case 6:
  case 7:
    FUN_f000_f6e6();
    if (!bVar36) {
      if ((1 < *(byte *)0x49) && (*(byte *)0x49 < 4)) {
        do {
          bVar13 = in(0x3da);
        } while ((bVar13 & 8) == 0);
        out(0x3d8,0x25);
      }
      puStack_4 = (undefined2 *)0xf2cd;
      iVar15 = FUN_f000_f742();
      puVar25 = (undefined2 *)(iVar15 + *(int *)0x4e);
      iVar20 = *(int *)0x4a << 1;
      puStack_4 = (undefined2 *)0xf2ea;
      iVar15 = FUN_f000_f097();
      bVar36 = *(char *)((int)unaff_BP + 3) != '\x06';
      if (bVar36) {
        iVar15 = -iVar15;
        iVar20 = -iVar20;
      }
      cVar18 = extraout_DH;
      if (*(char *)(unaff_BP + 1) != '\0') {
        cVar18 = extraout_DH - *(char *)(unaff_BP + 1);
        puVar24 = (undefined2 *)((int)puVar25 + iVar15);
        do {
          puStack_6 = puVar24;
          puVar24 = puStack_6;
          puVar26 = puVar25;
          for (uVar17 = (uint)extraout_DL; uVar17 != 0; uVar17 = uVar17 - 1) {
            puVar6 = puVar26;
            puVar26 = puVar26 + (uint)bVar36 * -2 + 1;
            puVar3 = puVar24;
            puVar24 = puVar24 + (uint)bVar36 * -2 + 1;
            *puVar6 = *puVar3;
          }
          puVar25 = (undefined2 *)((int)puVar25 + iVar20);
          cVar18 = cVar18 + -1;
          puVar24 = (undefined2 *)((int)puStack_6 + iVar20);
        } while (cVar18 != '\0');
        cVar18 = *(char *)(unaff_BP + 1);
      }
      uVar37 = *(undefined1 *)((int)unaff_BP + 5);
      do {
        puStack_4 = puVar25;
        puVar25 = puStack_4;
        for (uVar17 = (uint)extraout_DL; uVar17 != 0; uVar17 = uVar17 - 1) {
          puVar3 = puVar25;
          puVar25 = puVar25 + (uint)bVar36 * -2 + 1;
          *puVar3 = CONCAT11(uVar37,0x20);
        }
        cVar18 = cVar18 + -1;
        bVar38 = cVar18 == '\0';
        puVar25 = (undefined2 *)((int)puStack_4 + iVar20);
      } while (!bVar38);
      uVar17 = FUN_f000_f6e6();
      if (!bVar38) {
        uVar17 = (uint)*(byte *)0x65;
        out(0x3d8,*(byte *)0x65);
      }
      return uVar17;
    }
    iVar15 = unaff_BP[4];
    puStack_4 = (undefined2 *)0xf34c;
    pbVar31 = (byte *)FUN_f000_f753();
    iVar15 = (iVar15 - unaff_BP[3]) + 0x101;
    bVar13 = (byte)iVar15;
    if (*(char *)0x49 != '\x06') {
      bVar13 = bVar13 << 1;
      pbVar31 = (byte *)((int)pbVar31 * 2);
      if (*(char *)((int)unaff_BP + 3) == '\a') {
        pbVar31 = pbVar31 + 1;
      }
    }
    if (*(char *)((int)unaff_BP + 3) == '\a') {
      pbVar31 = pbVar31 + 0xf0;
    }
    bVar12 = *(char *)(unaff_BP + 1) << 2;
    bVar11 = (char)((uint)iVar15 >> 8) * '\x04' + *(char *)(unaff_BP + 1) * -4;
    iVar15 = (uint)bVar12 * 0x50;
    iVar20 = 0x1fb0;
    bVar36 = *(char *)((int)unaff_BP + 3) != '\x06';
    if (bVar36) {
      iVar15 = (uint)bVar12 * -0x50;
      iVar20 = 0x2050;
    }
    pbVar32 = pbVar31 + iVar15;
    uVar35 = *unaff_BP;
    bVar19 = bVar11;
    bVar10 = bVar12;
    while (bVar10 != 0) {
      pbVar30 = pbVar32;
      pbVar27 = pbVar31;
      for (uVar17 = (uint)bVar13; uVar17 != 0; uVar17 = uVar17 - 1) {
        pbVar4 = pbVar27;
        pbVar27 = pbVar27 + (uint)bVar36 * -2 + 1;
        pbVar1 = pbVar30;
        pbVar30 = pbVar30 + (uint)bVar36 * -2 + 1;
        *pbVar4 = *pbVar1;
      }
      pbVar30 = pbVar32 + 0x2000;
      pbVar27 = pbVar31 + 0x2000;
      for (uVar17 = (uint)bVar13; uVar17 != 0; uVar17 = uVar17 - 1) {
        pbVar4 = pbVar27;
        pbVar27 = pbVar27 + (uint)bVar36 * -2 + 1;
        pbVar1 = pbVar30;
        pbVar30 = pbVar30 + (uint)bVar36 * -2 + 1;
        *pbVar4 = *pbVar1;
      }
      pbVar32 = pbVar32 + 0x2000 + -iVar20;
      pbVar31 = pbVar31 + 0x2000 + -iVar20;
      bVar11 = bVar11 - 1;
      bVar19 = bVar12;
      bVar10 = bVar11;
    }
    bVar11 = *(byte *)((int)unaff_BP + 5);
    do {
      pbVar32 = pbVar31;
      for (uVar17 = (uint)bVar13; uVar17 != 0; uVar17 = uVar17 - 1) {
        pbVar1 = pbVar32;
        pbVar32 = pbVar32 + (uint)bVar36 * -2 + 1;
        *pbVar1 = bVar11;
      }
      pbVar32 = pbVar31 + 0x2000;
      for (uVar17 = (uint)bVar13; uVar17 != 0; uVar17 = uVar17 - 1) {
        pbVar1 = pbVar32;
        pbVar32 = pbVar32 + (uint)bVar36 * -2 + 1;
        *pbVar1 = bVar11;
      }
      pbVar31 = pbVar31 + 0x2000 + -iVar20;
      bVar19 = bVar19 - 1;
    } while (bVar19 != 0);
    return (uint)bVar11;
  default:
    FUN_f000_f6e6();
    if (!bVar36) {
      puStack_4 = (undefined2 *)0xf3f2;
      iVar15 = FUN_f000_f73b();
      puVar28 = (uint *)(iVar15 + in_stack_00000000 * *(int *)0x4c);
      iVar15 = *(int *)0x63 + 6;
      uVar35 = *unaff_BP;
      if (*(char *)((int)unaff_BP + 3) == '\b') {
        do {
          bVar13 = in(iVar15);
        } while ((bVar13 & 1) != 0);
        do {
          bVar13 = in(iVar15);
        } while ((bVar13 & 1) == 0);
        uVar17 = *puVar28;
        *(char *)(unaff_BP + 1) = (char)uVar17;
        *(undefined1 *)((int)unaff_BP + 3) = (char)(uVar17 >> 8);
        return uVar17;
      }
      bVar13 = *(byte *)(unaff_BP + 1);
      uVar37 = *(undefined1 *)(unaff_BP + 2);
      iVar20 = unaff_BP[3];
      if (*(char *)((int)unaff_BP + 3) == '\n') {
        do {
          do {
            bVar11 = in(iVar15);
          } while ((bVar11 & 1) != 0);
          do {
            bVar11 = in(iVar15);
          } while ((bVar11 & 1) == 0);
          *(byte *)puVar28 = bVar13;
          puVar28 = puVar28 + 1;
          iVar20 = iVar20 + -1;
        } while (iVar20 != 0);
        return (uint)bVar13;
      }
      do {
        do {
          bVar11 = in(iVar15);
        } while ((bVar11 & 1) != 0);
        do {
          bVar11 = in(iVar15);
        } while ((bVar11 & 1) == 0);
        puVar7 = puVar28;
        puVar28 = puVar28 + 1;
        *puVar7 = CONCAT11(uVar37,bVar13);
        iVar20 = iVar20 + -1;
      } while (iVar20 != 0);
      return CONCAT11(uVar37,bVar13);
    }
    if (*(char *)((int)unaff_BP + 3) != '\b') {
      pbVar31 = (byte *)FUN_f000_f753();
      bVar13 = *(byte *)(unaff_BP + 1);
      if ((char)bVar13 < '\0') {
        bVar13 = bVar13 & 0x7f;
        iVar15 = (int)((ulong)pcRam0000007c >> 0x10);
        pcVar33 = (char *)pcRam0000007c;
      }
      else {
        pcVar33 = (char *)0xfa6e;
      }
      uVar35 = *unaff_BP;
      iVar20 = unaff_BP[3];
      if (*(char *)0x49 == '\x06') {
        cVar18 = *(char *)(unaff_BP + 2);
        do {
          cVar23 = '\b';
          pbVar32 = (byte *)(pcVar33 + (uint)bVar13 * 8);
          pbVar30 = pbVar31;
          do {
            pbVar1 = pbVar32;
            pbVar32 = pbVar32 + 1;
            bVar11 = *pbVar1;
            if (cVar18 < '\0') {
              bVar11 = bVar11 ^ *pbVar30;
            }
            *pbVar30 = bVar11;
            pbVar30 = (byte *)((uint)pbVar30 ^ 0x2000);
            if (((uint)pbVar30 & 0x2000) == 0) {
              pbVar30 = pbVar30 + 0x50;
            }
            cVar23 = cVar23 + -1;
          } while (cVar23 != '\0');
          pbVar31 = pbVar31 + 1;
          iVar20 = iVar20 + -1;
        } while (iVar20 != 0);
        return (uint)bVar11;
      }
      puVar28 = (uint *)((int)pbVar31 << 1);
      bVar11 = *(byte *)(unaff_BP + 2);
      cVar18 = *(char *)(unaff_BP + 2);
      do {
        cVar23 = '\b';
        pbVar31 = (byte *)(pcVar33 + (uint)bVar13 * 8);
        puVar29 = puVar28;
        do {
          pbVar1 = pbVar31;
          pbVar31 = pbVar31 + 1;
          bVar12 = *pbVar1;
          uVar17 = 0;
          iVar22 = 8;
          do {
            bVar19 = bVar12 & 1;
            bVar12 = bVar12 >> 1;
            uVar17 = (int)(uVar17 >> 1 | (uint)bVar19 << 0xf) >> 1;
            iVar22 = iVar22 + -1;
          } while (iVar22 != 0);
          uVar17 = uVar17 & (bVar11 & 3) * 0x5555;
          uVar17 = CONCAT11((char)uVar17,(char)(uVar17 >> 8));
          if (cVar18 < '\0') {
            uVar17 = uVar17 ^ *puVar29;
          }
          *puVar29 = uVar17;
          puVar29 = (uint *)((uint)puVar29 ^ 0x2000);
          if (((uint)puVar29 & 0x2000) == 0) {
            puVar29 = puVar29 + 0x28;
          }
          cVar23 = cVar23 + -1;
        } while (cVar23 != '\0');
        puVar28 = puVar28 + 1;
        iVar20 = iVar20 + -1;
      } while (iVar20 != 0);
      return uVar17;
    }
    puVar16 = (undefined1 *)FUN_f000_f753();
    pbVar31 = local_8;
    pbVar32 = local_8;
    uVar35 = *unaff_BP;
    if (*(char *)0x49 == '\x06') {
      cVar18 = '\x04';
      do {
        *pbVar32 = *puVar16;
        pbVar32[1] = puVar16[0x2000];
        pbVar32 = pbVar32 + 2;
        puVar16 = puVar16 + 0x50;
        cVar18 = cVar18 + -1;
        bVar36 = cVar18 == '\0';
      } while (!bVar36);
    }
    else {
      cVar18 = '\b';
      puVar25 = (undefined2 *)((int)puVar16 << 1);
      do {
        bVar13 = 0;
        uVar17 = 0xc000;
        do {
          bVar13 = bVar13 << 1 |
                   (uVar17 & CONCAT11((char)*puVar25,(char)((uint)*puVar25 >> 8))) != 0;
          uVar21 = uVar17 >> 1;
          uVar17 = uVar17 >> 2;
        } while ((uVar21 & 1) == 0);
        *pbVar31 = bVar13;
        pbVar31 = pbVar31 + 1;
        puVar25 = (undefined2 *)((uint)puVar25 ^ 0x2000);
        if (((uint)puVar25 & 0x2000) == 0) {
          puVar25 = puVar25 + 0x28;
        }
        cVar18 = cVar18 + -1;
      } while (cVar18 != '\0');
      bVar36 = true;
    }
    pcVar33 = (char *)0xfa6e;
    uVar17 = 0;
    uVar35 = unaff_SS;
    do {
      iVar20 = 0x80;
      do {
        iVar22 = 8;
        pbVar31 = local_8;
        pcVar34 = pcVar33;
        do {
          if (iVar22 == 0) break;
          iVar22 = iVar22 + -1;
          pcVar5 = pcVar34;
          pcVar34 = pcVar34 + 1;
          pcVar2 = (char *)pbVar31;
          pbVar31 = pbVar31 + 1;
          bVar36 = *pcVar2 == *pcVar5;
        } while (bVar36);
        if (bVar36) goto LAB_f000_f5b6;
        bVar13 = (char)uVar17 + 1;
        uVar17 = (uint)bVar13;
        pcVar33 = pcVar33 + 8;
        iVar20 = iVar20 + -1;
        bVar36 = iVar20 == 0;
      } while (!bVar36);
      uVar17 = (uint)bVar13;
      if (bVar13 == 0) {
LAB_f000_f5b6:
        *(char *)(unaff_BP + 1) = (char)uVar17;
        return uVar17;
      }
      uVar35 = 0;
      iVar15 = (int)((ulong)pcRam0000007c >> 0x10);
      pcVar33 = (char *)pcRam0000007c;
      if (iVar15 == 0 && pcVar33 == (char *)0x0) goto LAB_f000_f5b6;
      bVar36 = false;
    } while( true );
  case 0xb:
    iVar15 = *(int *)0x63;
    uVar17 = CONCAT11(*(byte *)(unaff_BP + 2),*(undefined1 *)0x66);
    if (*(char *)((int)unaff_BP + 5) == '\0') {
      uVar17 = uVar17 & 0x1fe0;
      bVar13 = (byte)(uVar17 >> 8);
      uVar17 = CONCAT11(bVar13,(byte)uVar17 | bVar13);
    }
    else {
      uVar17 = uVar17 & 0xffdf;
      if ((*(byte *)(unaff_BP + 2) & 1) != 0) {
        uVar17 = uVar17 | 0x20;
      }
    }
    *(undefined1 *)0x66 = (char)uVar17;
    out(iVar15 + 5,(char)uVar17);
    return uVar17;
  case 0xc:
    uVar35 = *unaff_BP;
    bVar13 = (byte)unaff_BP[3];
    FUN_f000_f6f8();
    if (bVar38) {
      bVar11 = *(byte *)(unaff_BP + 1);
      bVar12 = bVar11 << 7;
      bVar19 = 0x7f;
    }
    else {
      bVar13 = bVar13 << 1;
      bVar11 = *(byte *)(unaff_BP + 1);
      bVar12 = (byte)((bVar11 & 3) << 7) >> 1 | ((bVar11 & 3) >> 1) << 7;
      bVar19 = 0x3f;
    }
    bVar19 = bVar19 >> (bVar13 & 7) | bVar19 << 8 - (bVar13 & 7);
    bVar12 = bVar12 >> (bVar13 & 0x1f);
    if ((char)bVar11 < '\0') {
      bVar13 = *unaff_SI ^ bVar12;
    }
    else {
      bVar13 = *unaff_SI & bVar19 | bVar12;
    }
    *unaff_SI = bVar13;
    return CONCAT11(bVar19,bVar12);
  case 0xd:
    uVar35 = *unaff_BP;
    bVar13 = (byte)unaff_BP[3];
    FUN_f000_f6f8();
    if (bVar38) {
      uVar17 = (uint)((char)(*unaff_SI << (bVar13 & 0x1f)) < '\0');
    }
    else {
      cVar18 = *unaff_SI << ((bVar13 & 0xf) << 1);
      bVar13 = cVar18 << 1 | cVar18 < '\0';
      uVar17 = (byte)(bVar13 << 1 | (char)bVar13 < '\0') & 0xff03;
    }
    *(char *)(unaff_BP + 1) = (char)uVar17;
    return uVar17;
  case 0xe:
    break;
  case 0xf:
    *(undefined1 *)((int)unaff_BP + 3) = *(undefined1 *)0x4a;
    *(undefined1 *)(unaff_BP + 1) = *(undefined1 *)0x49;
    uVar37 = *(undefined1 *)0x62;
    *(undefined1 *)((int)unaff_BP + 5) = uVar37;
    return CONCAT11(cVar18,uVar37);
  }
  iVar15 = *(int *)((byte)(*(char *)0x62 << 1) + 0x50);
  cVar18 = *(char *)(unaff_BP + 1);
  bVar13 = (byte)((uint)iVar15 >> 8);
  if (cVar18 == '\b') {
    if ((char)iVar15 != '\0') {
      iVar15 = CONCAT11(bVar13,(char)iVar15 + -1);
    }
    goto LAB_f000_f69a;
  }
  if (cVar18 != '\n') {
    if (cVar18 == '\a') {
      uVar17 = FUN_f000_f7af();
      return uVar17;
    }
    if (cVar18 == '\r') {
      iVar15 = (uint)bVar13 << 8;
      goto LAB_f000_f69a;
    }
    pcVar9 = (code *)swi(0x10);
    (*pcVar9)();
    bVar13 = (byte)((uint)extraout_DX >> 8);
    cVar18 = (char)extraout_DX + '\x01';
    iVar15 = CONCAT11(bVar13,cVar18);
    if (cVar18 != *(char *)0x4a) goto LAB_f000_f69a;
    iVar15 = (uint)bVar13 << 8;
  }
  bVar13 = (byte)((uint)iVar15 >> 8);
  uVar37 = bVar13 < 0x18;
  if ((bVar13 == 0x18) || (iVar15 = CONCAT11(bVar13 + 1,(char)iVar15), (byte)(bVar13 + 1) == '\0'))
  {
    pcVar9 = (code *)swi(0x10);
    (*pcVar9)();
    FUN_f000_f6e6();
    if (!(bool)uVar37) {
      pcVar9 = (code *)swi(0x10);
      (*pcVar9)();
    }
    pcVar9 = (code *)swi(0x10);
    uVar17 = (*pcVar9)();
    return uVar17;
  }
LAB_f000_f69a:
  *(int *)((byte)(*(char *)0x62 << 1) + 0x50) = iVar15;
  uVar17 = FUN_f000_f766();
  return uVar17;
}



/* ===== FUNCTION FUN_f000_f6e6 @ f000:f6e6 ===== */

undefined2 __cdecl16near FUN_f000_f6e6(void)

{
  undefined2 in_AX;
  
  return in_AX;
}



/* ===== FUNCTION FUN_f000_f6f8 @ f000:f6f8 ===== */

undefined4 __cdecl16near FUN_f000_f6f8(void)

{
  sbyte sVar1;
  uint in_CX;
  byte in_DL;
  undefined2 unaff_DS;
  
  sVar1 = 2;
  if (*(char *)0x49 == '\x06') {
    sVar1 = 3;
  }
  return CONCAT22(in_CX >> sVar1,(uint)(in_DL >> 1) * 0x50);
}



/* ===== FUNCTION FUN_f000_f721 @ f000:f721 ===== */

void __cdecl16near FUN_f000_f721(void)

{
  FUN_f000_f72e();
  FUN_f000_f72e();
  return;
}



/* ===== FUNCTION FUN_f000_f72e @ f000:f72e ===== */

undefined1 __cdecl16near FUN_f000_f72e(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  undefined1 in_AH;
  undefined2 unaff_DS;
  
  uVar1 = *(undefined2 *)0x63;
  out(uVar1,in_AH);
  uVar2 = in(CONCAT11((char)((uint)uVar1 >> 8),(char)uVar1 + '\x01'));
  return uVar2;
}



/* ===== FUNCTION FUN_f000_f73b @ f000:f73b ===== */

int FUN_f000_f73b(void)

{
  uint uVar1;
  byte in_BL;
  undefined2 unaff_DS;
  
  uVar1 = *(uint *)((uint)in_BL * 2 + 0x50);
  return ((uVar1 >> 8) * (uint)*(byte *)0x4a + (uVar1 & 0xff)) * 2;
}



/* ===== FUNCTION FUN_f000_f742 @ f000:f742 ===== */

int __cdecl16near FUN_f000_f742(void)

{
  uint in_AX;
  undefined2 unaff_DS;
  
  return ((in_AX >> 8) * (uint)*(byte *)0x4a + (in_AX & 0xff)) * 2;
}



/* ===== FUNCTION FUN_f000_f753 @ f000:f753 ===== */

int __cdecl16near FUN_f000_f753(void)

{
  uint in_AX;
  undefined2 unaff_DS;
  
  return (in_AX >> 8) * (uint)*(byte *)0x4a * 4 + (in_AX & 0xff);
}



/* ===== FUNCTION FUN_f000_f766 @ f000:f766 ===== */

void __cdecl16near FUN_f000_f766(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  int iVar3;
  char extraout_AH;
  byte in_BL;
  undefined2 unaff_DS;
  
  if (*(byte *)0x62 == in_BL >> 1) {
    iVar3 = FUN_f000_f73b();
    uVar2 = (undefined1)((uint)(iVar3 + *(int *)0x4e) >> 1);
    FUN_f000_f784();
    uVar1 = *(undefined2 *)0x63;
    out(uVar1,extraout_AH + '\x01');
    out(CONCAT11((char)((uint)uVar1 >> 8),(char)uVar1 + '\x01'),uVar2);
  }
  return;
}



/* ===== FUNCTION FUN_f000_f76e @ f000:f76e ===== */

undefined4 FUN_f000_f76e(void)

{
  undefined2 uVar1;
  undefined1 uVar2;
  int iVar3;
  char extraout_AH;
  undefined2 extraout_DX;
  undefined2 unaff_DS;
  
  iVar3 = FUN_f000_f73b();
  uVar2 = (undefined1)((uint)(iVar3 + *(int *)0x4e) >> 1);
  FUN_f000_f784();
  uVar1 = *(undefined2 *)0x63;
  out(uVar1,extraout_AH + '\x01');
  out(CONCAT11((char)((uint)uVar1 >> 8),(char)uVar1 + '\x01'),uVar2);
  return CONCAT22(extraout_DX,CONCAT11(extraout_AH + '\x01',uVar2));
}



/* ===== FUNCTION FUN_f000_f77b @ f000:f77b ===== */

undefined4 FUN_f000_f77b(void)

{
  undefined2 uVar1;
  char extraout_AH;
  undefined1 in_CL;
  undefined2 extraout_DX;
  undefined2 unaff_DS;
  
  FUN_f000_f784();
  uVar1 = *(undefined2 *)0x63;
  out(uVar1,extraout_AH + '\x01');
  out(CONCAT11((char)((uint)uVar1 >> 8),(char)uVar1 + '\x01'),in_CL);
  return CONCAT22(extraout_DX,CONCAT11(extraout_AH + '\x01',in_CL));
}



/* ===== FUNCTION FUN_f000_f784 @ f000:f784 ===== */

void __cdecl16near FUN_f000_f784(void)

{
  undefined2 uVar1;
  undefined1 in_AL;
  undefined1 in_AH;
  undefined2 unaff_DS;
  
  uVar1 = *(undefined2 *)0x63;
  out(uVar1,in_AH);
  out(CONCAT11((char)((uint)uVar1 >> 8),(char)uVar1 + '\x01'),in_AL);
  return;
}



/* ===== FUNCTION FUN_f000_f793 @ f000:f793 ===== */

void __cdecl16near FUN_f000_f793(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_f000_f7af @ f000:f7af ===== */

undefined2 __cdecl16near FUN_f000_f7af(void)

{
  byte bVar1;
  undefined2 in_AX;
  int iVar2;
  char in_BL;
  
  out(0x43,0xb6);
  out(0x42,0x28);
  out(0x42,5);
  bVar1 = in(0x61);
  out(0x61,bVar1 | 3);
  iVar2 = 0;
  do {
    do {
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    in_BL = in_BL + -1;
  } while (in_BL != '\0');
  out(0x61,bVar1);
  return in_AX;
}



/* ===== FUNCTION FUN_f000_f85f @ f000:f85f ===== */

undefined2 FUN_f000_f85f(void)

{
  byte bVar1;
  undefined1 uVar2;
  char cVar3;
  undefined2 in_AX;
  undefined1 extraout_AH;
  int iVar4;
  undefined1 *puVar5;
  int iVar6;
  undefined2 uVar7;
  
  bVar1 = in(0x62);
  if ((bVar1 & 0xc0) != 0) {
    FUN_f000_f793();
    uVar7 = 0x40;
    FUN_f000_e4e2();
    FUN_f000_e4ed();
    out(0xa0,0);
    bVar1 = in(0x61);
    out(0x61,bVar1 | 0x30);
    out(0x61,bVar1 & 0xcf);
    iVar6 = 0;
    do {
      iVar4 = 0x10;
      puVar5 = (undefined1 *)0x0;
      do {
        bVar1 = in(0x62);
        if ((bVar1 & 0xc0) != 0) {
          *puVar5 = *puVar5;
          FUN_f000_e512();
          FUN_f000_e6e2();
          goto LAB_f000_f8ca;
        }
        puVar5 = puVar5 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      iVar6 = iVar6 + 1;
    } while (iVar6 != *(int *)0x13 << 6);
LAB_f000_f8ca:
    FUN_f000_e4ed();
    FUN_f000_e4e2(iVar6);
    uVar2 = in(0x21);
    uVar7 = CONCAT11(extraout_AH,uVar2);
    out(0x21,0xfc);
    cVar3 = FUN_f000_feee(uVar7);
    uVar2 = (undefined1)uVar7;
    FUN_f000_e4d7();
    if ((cVar3 != 'Y') && (cVar3 != 'y')) {
      uVar7 = FUN_f000_e271();
      return uVar7;
    }
    FUN_f000_f9d0();
    out(0x21,uVar2);
    bVar1 = in(0x61);
    out(0x61,bVar1 | 0x30);
    out(0x61,bVar1 & 0xcf);
    out(0xa0,0x80);
  }
  return in_AX;
}



/* ===== FUNCTION FUN_f000_f91b @ f000:f91b ===== */

/* WARNING: Removing unreachable block (ram,0x000ff923) */

undefined2 __cdecl16near FUN_f000_f91b(void)

{
  return 0x1000;
}



/* ===== FUNCTION FUN_f000_f92b @ f000:f92b ===== */

int __cdecl16near FUN_f000_f92b(void)

{
  undefined2 *puVar1;
  int *piVar2;
  int iVar3;
  undefined2 *puVar4;
  int *piVar5;
  int unaff_ES;
  
  puVar4 = (undefined2 *)0x0;
  for (iVar3 = 0x200; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar4;
    puVar4 = puVar4 + 1;
    *puVar1 = 0x5555;
  }
  piVar5 = (int *)0x0;
  iVar3 = 0x200;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    piVar2 = piVar5;
    piVar5 = piVar5 + 1;
  } while (*piVar2 == 0x5555);
  if (iVar3 != 0) {
    return 0x5555;
  }
  puVar4 = (undefined2 *)0x0;
  for (iVar3 = 0x200; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar4;
    puVar4 = puVar4 + 1;
    *puVar1 = 0xaaaa;
  }
  piVar5 = (int *)0x0;
  iVar3 = 0x200;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    piVar2 = piVar5;
    piVar5 = piVar5 + 1;
  } while (*piVar2 == -0x5556);
  if (iVar3 != 0) {
    return -0x5556;
  }
  puVar4 = (undefined2 *)0x0;
  for (iVar3 = 0x200; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar4;
    puVar4 = puVar4 + 1;
    *puVar1 = 0xffff;
  }
  piVar5 = (int *)0x0;
  iVar3 = 0x200;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    piVar2 = piVar5;
    piVar5 = piVar5 + 1;
  } while (*piVar2 == -1);
  if (iVar3 != 0) {
    return -1;
  }
  puVar4 = (undefined2 *)0x0;
  for (iVar3 = 0x200; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar1 = puVar4;
    puVar4 = puVar4 + 1;
    *puVar1 = 0;
  }
  piVar5 = (int *)0x0;
  iVar3 = 0x200;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    piVar2 = piVar5;
    piVar5 = piVar5 + 1;
  } while (*piVar2 == 0);
  if (iVar3 != 0) {
    return 0;
  }
  return unaff_ES + 0x40;
}



/* ===== FUNCTION FUN_f000_f981 @ f000:f981 ===== */

undefined2 __cdecl16near FUN_f000_f981(void)

{
  byte bVar1;
  undefined2 in_AX;
  int iVar2;
  undefined2 uVar3;
  
  bVar1 = in(0x61);
  out(0x61,bVar1 ^ 0xc);
  uVar3 = 0xf89;
  if (((bVar1 ^ 0xc) & 4) != 0) {
    uVar3 = 0x52e;
  }
  out(0x43,0xb6);
  out(0x42,(char)uVar3);
  out(0x42,(char)((uint)uVar3 >> 8));
  bVar1 = in(0x61);
  out(0x61,bVar1 | 3);
  iVar2 = 0x2000;
  do {
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  out(0x61,bVar1);
  return in_AX;
}



/* ===== FUNCTION FUN_f000_f9b3 @ f000:f9b3 ===== */

void __cdecl16near FUN_f000_f9b3(void)

{
  code *pcVar1;
  uint uVar2;
  int in_BX;
  uint uVar3;
  undefined2 unaff_ES;
  bool bVar4;
  
  uVar3 = in_BX + *(int *)0x46c;
  uVar2 = uVar3;
  do {
    bVar4 = uVar2 == 0;
    pcVar1 = (code *)swi(0x16);
    (*pcVar1)();
    if (!bVar4) {
      pcVar1 = (code *)swi(0x16);
      (*pcVar1)();
      return;
    }
    uVar2 = *(uint *)0x46c - uVar3;
  } while (*(uint *)0x46c < uVar3);
  return;
}



/* ===== FUNCTION FUN_f000_f9d0 @ f000:f9d0 ===== */

void __cdecl16near FUN_f000_f9d0(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_f000_f9ea @ f000:f9ea ===== */

void FUN_f000_f9ea(void)

{
  char *pcVar1;
  code *pcVar2;
  char *unaff_SI;
  undefined2 unaff_DS;
  
  while (pcVar1 = unaff_SI, unaff_SI = unaff_SI + 1, *pcVar1 != '\0') {
    FUN_f000_fa2a();
  }
  FUN_f000_fa2a();
  FUN_f000_fa2a();
  FUN_f000_fa2a();
  pcVar2 = (code *)swi(0x10);
  (*pcVar2)();
  pcVar2 = (code *)swi(0x10);
  (*pcVar2)();
  return;
}



/* ===== FUNCTION FUN_f000_fa2a @ f000:fa2a ===== */

void __cdecl16near FUN_f000_fa2a(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x10);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_f000_fe9b @ f000:fe9b ===== */

void __cdecl16near FUN_f000_fe9b(void)

{
  undefined2 unaff_DS;
  
  *(byte *)0x470 = *(byte *)0x470 ^ *(byte *)0x470;
  return;
}



/* ===== FUNCTION FUN_f000_feee @ f000:feee ===== */

void __cdecl16near FUN_f000_feee(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x16);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_f000_ffcb @ f000:ffcb ===== */

void __cdecl16near FUN_f000_ffcb(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x17);
  (*pcVar1)();
  pcVar1 = (code *)swi(0x17);
  (*pcVar1)();
  return;
}


