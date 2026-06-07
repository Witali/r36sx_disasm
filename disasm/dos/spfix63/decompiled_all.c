
/* ===== FUNCTION FUN_1000_04b7 @ 1000:04b7 ===== */

void __cdecl16near FUN_1000_04b7(void)

{
  code *pcVar1;
  char *unaff_SI;
  undefined2 unaff_DS;
  
  while (*unaff_SI != '\0') {
    unaff_SI = unaff_SI + 1;
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  return;
}



/* ===== FUNCTION FUN_1000_04c6 @ 1000:04c6 ===== */

uint __cdecl16near FUN_1000_04c6(void)

{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined2 unaff_DS;
  bool bVar6;
  int in_stack_00000008;
  
  FUN_1000_0634();
  FUN_1000_087e();
  FUN_1000_6cd2();
  FUN_1000_66d5();
  FUN_1000_083d();
  FUN_1000_1dd4();
  pcVar2 = (code *)swi(0x21);
  iVar3 = (*pcVar2)();
  piVar5 = (int *)0x96cc;
  iVar4 = 10;
  while (*piVar5 != iVar3) {
    piVar5 = piVar5 + 1;
    iVar4 = iVar4 + -1;
    if (iVar4 == 0) {
      return 0;
    }
  }
  bVar6 = false;
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  if (bVar6) {
    return 0;
  }
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  if (bVar6) {
    return 0;
  }
  iVar3 = (in_stack_00000008 + -10) * -4;
  if (*(int *)(iVar3 + -0x691e) != *(int *)0x76a) {
    return 0;
  }
  uVar1 = *(uint *)(iVar3 + -0x6920);
  if (uVar1 != *(uint *)0x768) {
    return 0;
  }
  return uVar1 & 0xff;
}



/* ===== FUNCTION FUN_1000_0634 @ 1000:0634 ===== */

void __cdecl16near FUN_1000_0634(void)

{
  code *pcVar1;
  undefined2 unaff_DS;
  
  do {
    do {
    } while (*(char *)0x168a != '\0');
  } while (((((*(char *)0x1697 != '\0') || (*(char *)0x16a3 != '\0')) || (*(char *)0x16a5 != '\0'))
           || ((*(char *)0x16a7 != '\0' || (*(char *)0x16b2 != '\0')))) ||
          ((*(char *)0x16b3 != '\0' || ((*(char *)0x16bf != '\0' || (*(char *)0x16c1 != '\0'))))));
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_1000_083d @ 1000:083d ===== */

undefined2 __cdecl16near FUN_1000_083d(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  out(0x43,0x36);
  out(0x40,0xff);
  out(0x40,0xff);
  return 0xff;
}



/* ===== FUNCTION FUN_1000_087e @ 1000:087e ===== */

void __cdecl16near FUN_1000_087e(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_1000_09f6 @ 1000:09f6 ===== */

undefined1 __cdecl16near FUN_1000_09f6(void)

{
  out(0x3f2,0xf);
  return 0xf;
}



/* ===== FUNCTION FUN_1000_0bb9 @ 1000:0bb9 ===== */

void __cdecl16near FUN_1000_0bb9(void)

{
  int iVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  undefined1 *local_6;
  undefined1 *local_4;
  
  out(0x3ce,5);
  out(0x3cf,1);
  out(0x3c4,2);
  out(0x3c5,0xf);
  out(0x3ce,8);
  out(0x3cf,0);
  local_4 = (undefined1 *)0x5a23;
  local_6 = (undefined1 *)0x5a24;
  iVar2 = 0xf;
  do {
    iVar1 = 2;
    do {
      FUN_1000_6803();
      FUN_1000_684b();
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    iVar1 = 0x94;
    puVar3 = local_4 + -1;
    puVar4 = local_4 + -2;
    do {
      *puVar4 = *puVar3;
      puVar4[1] = puVar3[1];
      puVar3 = puVar3 + 0x7a;
      puVar4 = puVar4 + 0x7a;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    iVar1 = 0x94;
    puVar4 = local_4 + 0x28;
    puVar3 = local_4;
    do {
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 0x7a;
      puVar3 = puVar3 + 0x7a;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    iVar1 = 0x94;
    puVar3 = local_6 + 0x4689;
    puVar4 = local_6 + 0x468a;
    do {
      *puVar4 = *puVar3;
      puVar4[-1] = puVar3[-1];
      puVar3 = puVar3 + -0x7a;
      puVar4 = puVar4 + -0x7a;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    iVar1 = 0x94;
    puVar4 = local_6 + 0x28;
    puVar3 = local_6;
    do {
      *puVar3 = *puVar4;
      puVar4 = puVar4 + 0x7a;
      puVar3 = puVar3 + 0x7a;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    local_4 = local_4 + -1;
    local_6 = local_6 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  out(0x3ce,5);
  out(0x3cf,1);
  iVar2 = 1;
  do {
    FUN_1000_6803();
    FUN_1000_684b();
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(undefined2 *)0x1657 = 0x4dd4;
  out(0x3d4,0xd);
  out(0x3d5,0xd4);
  out(0x3d4,0xc);
  out(0x3d5,0x4d);
  FUN_1000_6ae4();
  return;
}



/* ===== FUNCTION FUN_1000_0ddb @ 1000:0ddb ===== */

undefined4 __cdecl16near FUN_1000_0ddb(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  code *pcVar3;
  uint uVar4;
  undefined1 extraout_AH;
  undefined2 uVar5;
  undefined1 extraout_AH_00;
  undefined1 uVar6;
  int iVar7;
  undefined2 *puVar8;
  undefined2 *puVar9;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  undefined1 uVar10;
  byte bVar11;
  bool bVar12;
  undefined4 uVar13;
  uint local_6;
  byte local_3;
  
  bVar12 = false;
  uVar10 = (undefined1 *)0x5 < &stack0xfffe;
  while( true ) {
    pcVar3 = (code *)swi(0x21);
    uVar4 = (*pcVar3)();
    if (!(bool)uVar10) break;
    bVar11 = uVar4 < 2;
    if (uVar4 != 2) {
LAB_1000_0e04:
      uVar13 = FUN_1000_04c6();
      return uVar13;
    }
    FUN_1000_132d();
    uVar6 = extraout_AH;
    if (*(char *)0x9876 == -1) goto LAB_1000_0ed7;
    bVar12 = bVar12 != false;
    if ((bVar11 & 1) != 0) goto LAB_1000_0e04;
    uVar10 = false;
  }
  *(uint *)0x766 = uVar4;
  out(0x3ce,5);
  out(0x3cf,0);
  out(0x3ce,8);
  out(0x3cf,0xff);
  out(0x3ce,1);
  out(0x3cf,0);
  local_6 = 0;
  do {
    local_3 = 0;
    do {
      pcVar3 = (code *)swi(0x21);
      (*pcVar3)();
      if ((bool)uVar10) {
        uVar13 = FUN_1000_04c6();
        return uVar13;
      }
      puVar8 = (undefined2 *)0x768;
      for (puVar9 = (undefined2 *)(local_6 * 0x7a + 0xb72); (undefined2 *)0x4d33 < puVar9;
          puVar9 = puVar9 + -0x2686) {
      }
      out(0x3c4,2);
      out(0x3c5,'\x01' << (local_3 & 0x1f));
      for (iVar7 = 0x14; iVar7 != 0; iVar7 = iVar7 + -1) {
        puVar2 = puVar9;
        puVar9 = puVar9 + (uint)bVar12 * -2 + 1;
        puVar1 = puVar8;
        puVar8 = puVar8 + (uint)bVar12 * -2 + 1;
        *puVar2 = *puVar1;
      }
      local_3 = local_3 + 1;
      uVar10 = local_3 < 4;
    } while ((char)local_3 < '\x04');
    local_6 = local_6 + 1;
    uVar10 = local_6 < 0x1d0;
  } while ((int)local_6 < 0x1d0);
  pcVar3 = (code *)swi(0x21);
  (*pcVar3)();
  if ((bool)uVar10) {
    uVar13 = FUN_1000_04c6();
    return uVar13;
  }
  pcVar3 = (code *)swi(0x21);
  uVar5 = (*pcVar3)();
  if ((bool)uVar10) {
    uVar13 = FUN_1000_04c6();
    return uVar13;
  }
  *(undefined2 *)0x766 = uVar5;
  pcVar3 = (code *)swi(0x21);
  (*pcVar3)();
  if ((bool)uVar10) {
    uVar13 = FUN_1000_04c6();
    return uVar13;
  }
  pcVar3 = (code *)swi(0x21);
  (*pcVar3)();
  uVar6 = extraout_AH_00;
  if ((bool)uVar10) {
    uVar13 = FUN_1000_04c6();
    return uVar13;
  }
LAB_1000_0ed7:
  out(0x3c4,2);
  out(0x3c5,0xff);
  return CONCAT22(0x3c5,CONCAT11(uVar6,0xff));
}



/* ===== FUNCTION FUN_1000_128c @ 1000:128c ===== */

void __cdecl16near FUN_1000_128c(void)

{
  int in_BX;
  int iVar1;
  undefined2 unaff_DS;
  int iStack_2;
  
  *(undefined1 *)0x9876 = 0;
  do {
    if (*(char *)0x166e != '\0') {
      *(undefined1 *)0x9876 = 0xff;
    }
  } while (*(char *)0x16f9 != '\0');
  do {
    FUN_1000_6743();
    iVar1 = 0;
  } while (in_BX != 0);
  do {
    FUN_1000_224d();
  } while ('\x04' < *(char *)0x631);
  iStack_2 = 0x1068;
  do {
    iStack_2 = iStack_2 + -1;
    FUN_1000_6803();
    FUN_1000_684b();
    if (iStack_2 == 0) goto LAB_1000_12e5;
    FUN_1000_6743();
    if (iVar1 != 0) goto LAB_1000_12f6;
    if (*(char *)0x16f9 != '\0') goto LAB_1000_1300;
    FUN_1000_224d();
  } while (*(char *)0x631 < '\x05');
  do {
    if (*(char *)0x166e != '\0') {
      *(undefined1 *)0x9876 = 0xff;
    }
    FUN_1000_224d();
  } while ('\x04' < *(char *)0x631);
LAB_1000_12e5:
  *(undefined2 *)0xd92 = 0;
  return;
LAB_1000_12f6:
  do {
    FUN_1000_6743();
  } while (iVar1 != 0);
  goto LAB_1000_12e5;
LAB_1000_1300:
  do {
    if (*(char *)0x166e != '\0') {
      *(undefined1 *)0x9876 = 0xff;
    }
  } while (*(char *)0x16f9 != '\0');
  goto LAB_1000_12e5;
}



/* ===== FUNCTION FUN_1000_132d @ 1000:132d ===== */

void __cdecl16near FUN_1000_132d(void)

{
  undefined2 unaff_DS;
  byte bVar1;
  byte bVar2;
  byte in_AF;
  byte bVar3;
  byte bVar4;
  byte in_TF;
  byte in_IF;
  byte bVar5;
  byte in_NT;
  
  FUN_1000_09f6();
  FUN_1000_6c2a();
  FUN_1000_5ae3();
  bVar1 = *(char *)0x3700 == '\0';
  if (*(char *)0x3700 == '\x01') {
    *(undefined1 *)0x3700 = 2;
  }
  else {
    *(undefined1 *)0x3700 = 1;
  }
  FUN_1000_51e4();
  FUN_1000_6c2a();
  out(0x3d4,0xd);
  out(0x3d5,0x84);
  out(0x3d4,0xc);
  bVar5 = 0;
  bVar4 = 0;
  bVar3 = 0;
  bVar2 = 0;
  out(0x3d5,0x4d);
  FUN_1000_128c();
  FUN_1000_6c2a((uint)(in_NT & 1) * 0x4000 | (uint)(bVar5 & 1) * 0x800 | (uint)(in_IF & 1) * 0x200 |
                (uint)(in_TF & 1) * 0x100 | (uint)(bVar4 & 1) * 0x80 | (uint)(bVar3 & 1) * 0x40 |
                (uint)(in_AF & 1) * 0x10 | (uint)(bVar2 & 1) * 4 | (uint)(bVar1 & 1));
  return;
}



/* ===== FUNCTION FUN_1000_1dd4 @ 1000:1dd4 ===== */

void __cdecl16near FUN_1000_1dd4(void)

{
  char *pcVar1;
  code *pcVar2;
  int in_AX;
  int iVar3;
  char *pcVar4;
  
  pcVar4 = (char *)0x0;
  do {
    iVar3 = 1000;
    do {
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      pcVar1 = pcVar4;
      pcVar4 = pcVar4 + 1;
    } while (*pcVar1 != '\0');
    in_AX = in_AX + -1;
    if (in_AX == 0) {
      while (*pcVar4 != '\0') {
        pcVar4 = pcVar4 + 1;
        pcVar2 = (code *)swi(0x10);
        (*pcVar2)();
      }
      pcVar2 = (code *)swi(0x10);
      (*pcVar2)();
      pcVar2 = (code *)swi(0x10);
      (*pcVar2)();
      return;
    }
  } while( true );
}



/* ===== FUNCTION FUN_1000_224d @ 1000:224d ===== */

void __cdecl16near FUN_1000_224d(void)

{
  byte bVar1;
  uint uVar2;
  char cVar3;
  undefined2 unaff_DS;
  bool bVar4;
  
  cVar3 = '\0';
  if (*(char *)0x630 != '\0') {
    *(undefined2 *)0x632 = 0;
    *(undefined2 *)0x634 = 0;
    *(undefined2 *)0x63f = 0;
    *(undefined2 *)0x641 = 0;
    *(undefined1 *)0x643 = 0;
    *(undefined1 *)0x644 = 0;
    bVar4 = false;
    if ((*(char *)0x636 != '\0') && (uVar2 = FUN_1000_33e1(), !bVar4)) {
      *(uint *)0x632 = uVar2;
      uVar2 = (int)((ulong)uVar2 * (ulong)*(uint *)0x63b >> 0x10) +
              (uint)((int)((ulong)uVar2 * (ulong)*(uint *)0x63b) < 0);
      if (0x10 < uVar2) {
        uVar2 = 0x10;
      }
      if (*(char *)0x16e7 == '\0') {
        if (*(char *)0x16e6 != '\0') {
          uVar2 = 0x10;
        }
      }
      else {
        uVar2 = 1;
      }
      bVar4 = false;
      *(uint *)0x63f = uVar2;
      uVar2 = FUN_1000_33e1();
      if (!bVar4) {
        *(uint *)0x634 = uVar2;
        uVar2 = (int)((ulong)uVar2 * (ulong)*(uint *)0x63d >> 0x10) +
                (uint)((int)((ulong)uVar2 * (ulong)*(uint *)0x63d) < 0);
        if (0x10 < uVar2) {
          uVar2 = 0x10;
        }
        if (*(char *)0x16e9 == '\0') {
          if (*(char *)0x16e8 != '\0') {
            uVar2 = 0x10;
          }
        }
        else {
          uVar2 = 1;
        }
        *(uint *)0x641 = uVar2;
        cVar3 = *(char *)(*(int *)0x63f + *(int *)0x641 * 0x11 + 0x645);
        if (cVar3 == '\0') {
          bVar1 = in(0x201);
          if ((bVar1 & 0x10) == 0) {
            cVar3 = '\t';
          }
          else if ((bVar1 & 0x20) == 0) {
            cVar3 = '\t';
          }
          else if (*(char *)0x16e5 == '\0') {
            if (*(char *)0x16e4 != '\0') {
              cVar3 = '\t';
            }
          }
          else {
            cVar3 = '\t';
          }
        }
        else {
          bVar1 = in(0x201);
          if ((bVar1 & 0x10) == 0) {
            cVar3 = cVar3 + '\x04';
          }
          else if ((bVar1 & 0x20) == 0) {
            cVar3 = cVar3 + '\x04';
          }
          else if (*(char *)0x16e5 == '\0') {
            if (*(char *)0x16e4 != '\0') {
              cVar3 = cVar3 + '\x04';
            }
          }
          else {
            cVar3 = cVar3 + '\x04';
          }
        }
      }
    }
  }
  *(char *)0x631 = cVar3;
  return;
}



/* ===== FUNCTION FUN_1000_33e1 @ 1000:33e1 ===== */

/* WARNING: Removing unreachable block (ram,0x00013401) */

char __cdecl16near FUN_1000_33e1(void)

{
  char cVar1;
  undefined2 uVar2;
  int in_BX;
  int unaff_SI;
  undefined2 unaff_DS;
  bool bVar3;
  
  if (*(char *)0x630 != '\0') {
    uVar2 = 0x201;
    bVar3 = false;
    while( true ) {
      cVar1 = in(uVar2);
      if (!bVar3) break;
      uVar2 = CONCAT11((char)((uint)uVar2 >> 8),0x90);
      bVar3 = cVar1 == '\0' && *(char *)(in_BX + unaff_SI) == '\0';
      unaff_SI = (int)*(undefined4 *)(unaff_SI + 5);
    }
    *(int *)(unaff_SI + 1) = *(int *)(unaff_SI + 1) - unaff_SI;
    return cVar1;
  }
  return '\0';
}



/* ===== FUNCTION FUN_1000_4c47 @ 1000:4c47 ===== */

void __cdecl16near FUN_1000_4c47(void)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  out(0x3ce,5);
  out(0x3cf,1);
  puVar2 = (undefined1 *)(*(int *)0x8173 * 0x7a + 0x4d5c + (*(uint *)0x816f >> 3));
  *(undefined2 *)0x816b = puVar2;
  puVar3 = (undefined1 *)0x4d34;
  iVar1 = 0x10;
  do {
    *puVar3 = *puVar2;
    puVar3[1] = puVar2[1];
    puVar2 = puVar2 + 0x7a;
    puVar3 = puVar3 + 0x7a;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



/* ===== FUNCTION FUN_1000_4c84 @ 1000:4c84 ===== */

void __cdecl16near FUN_1000_4c84(void)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  out(0x3ce,5);
  out(0x3cf,1);
  puVar2 = (undefined1 *)0x4d34;
  puVar3 = (undefined1 *)*(undefined2 *)0x816b;
  iVar1 = 0x10;
  do {
    *puVar3 = *puVar2;
    puVar3[1] = puVar2[1];
    puVar2 = puVar2 + 0x7a;
    puVar3 = puVar3 + 0x7a;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return;
}



/* ===== FUNCTION FUN_1000_4ca9 @ 1000:4ca9 ===== */

undefined4 __cdecl16near FUN_1000_4ca9(void)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  byte bVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte *pbVar8;
  byte *pbVar9;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  out(0x3ce,5);
  out(0x3cf,8);
  out(0x3ce,2);
  out(0x3cf,1);
  out(0x3ce,7);
  out(0x3cf,0xf);
  out(0x3ce,0);
  out(0x3cf,1);
  out(0x3ce,1);
  out(0x3cf,0xf);
  pbVar7 = (byte *)*(undefined2 *)((*(uint *)0x164d >> 2 & 0xe) + 0x1520);
  pbVar9 = (byte *)*(undefined2 *)0x816b;
  bVar4 = (byte)*(undefined2 *)0x816f;
  bVar5 = 8 - (bVar4 & 7);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar3 = 8;
  pbVar6 = pbVar7;
  pbVar8 = pbVar9;
  do {
    bVar2 = *pbVar6;
    bVar1 = bVar2 >> (bVar4 & 7);
    out(0x3cf,bVar1);
    *pbVar8 = *pbVar8 ^ bVar1;
    bVar2 = bVar2 << (bVar5 & 0x1f);
    out(0x3cf,bVar2);
    pbVar8[1] = pbVar8[1] ^ bVar2;
    pbVar6 = pbVar6 + 0x7a;
    pbVar8 = pbVar8 + 0x7a;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  out(0x3ce,2);
  out(0x3cf,5);
  out(0x3ce,0);
  out(0x3cf,5);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar3 = 8;
  pbVar6 = pbVar7;
  pbVar8 = pbVar9;
  do {
    bVar2 = *pbVar6;
    bVar1 = bVar2 >> (bVar4 & 7);
    out(0x3cf,bVar1);
    *pbVar8 = *pbVar8 ^ bVar1;
    bVar2 = bVar2 << (bVar5 & 0x1f);
    out(0x3cf,bVar2);
    pbVar8[1] = pbVar8[1] ^ bVar2;
    pbVar6 = pbVar6 + 0x7a;
    pbVar8 = pbVar8 + 0x7a;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  out(0x3ce,2);
  out(0x3cf,8);
  out(0x3ce,0);
  out(0x3cf,8);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar3 = 8;
  pbVar6 = pbVar7;
  pbVar8 = pbVar9;
  do {
    bVar2 = *pbVar6;
    bVar1 = bVar2 >> (bVar4 & 7);
    out(0x3cf,bVar1);
    *pbVar8 = *pbVar8 ^ bVar1;
    bVar2 = bVar2 << (bVar5 & 0x1f);
    out(0x3cf,bVar2);
    pbVar8[1] = pbVar8[1] ^ bVar2;
    pbVar6 = pbVar6 + 0x7a;
    pbVar8 = pbVar8 + 0x7a;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  out(0x3ce,2);
  out(0x3cf,9);
  out(0x3ce,0);
  out(0x3cf,9);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar3 = 8;
  pbVar6 = pbVar7;
  pbVar8 = pbVar9;
  do {
    bVar2 = *pbVar6;
    bVar1 = bVar2 >> (bVar4 & 7);
    out(0x3cf,bVar1);
    *pbVar8 = *pbVar8 ^ bVar1;
    bVar2 = bVar2 << (bVar5 & 0x1f);
    out(0x3cf,bVar2);
    pbVar8[1] = pbVar8[1] ^ bVar2;
    pbVar6 = pbVar6 + 0x7a;
    pbVar8 = pbVar8 + 0x7a;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  out(0x3ce,2);
  out(0x3cf,0xf);
  out(0x3ce,0);
  out(0x3cf,0xf);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar3 = 8;
  do {
    bVar2 = *pbVar7;
    bVar1 = bVar2 >> (bVar4 & 7);
    out(0x3cf,bVar1);
    *pbVar9 = *pbVar9 ^ bVar1;
    bVar1 = bVar2 << (bVar5 & 0x1f);
    out(0x3cf,bVar1);
    pbVar9[1] = pbVar9[1] ^ bVar1;
    pbVar7 = pbVar7 + 0x7a;
    pbVar9 = pbVar9 + 0x7a;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  out(0x3ce,8);
  out(0x3cf,0xff);
  return CONCAT22(0x3cf,CONCAT11(bVar2,0xff));
}



/* ===== FUNCTION FUN_1000_51e4 @ 1000:51e4 ===== */

void __cdecl16near FUN_1000_51e4(void)

{
  char cVar1;
  byte bVar2;
  byte in_AH;
  byte bVar3;
  int in_CX;
  int in_BX;
  uint uVar4;
  char *unaff_SI;
  byte *unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  *(int *)(unaff_SI + in_BX + 0x5926) = *(int *)(unaff_SI + in_BX + 0x5926) + in_CX;
  out(0x3ce,5);
  out(0x3cf,0);
  out(0x3ce,1);
  out(0x3cf,0xf);
  out(0x3ce,8);
  out(0x3cf,in_AH);
  bVar2 = 0;
  while ((bVar3 = bVar2, cVar1 = *unaff_SI, cVar1 != '\0' && (cVar1 != '\n'))) {
    unaff_SI = unaff_SI + 1;
    uVar4 = (uint)(byte)(cVar1 - 0x20);
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,in_AH);
    bVar2 = *(byte *)(uVar4 + 0x5b15) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    *unaff_DI = *unaff_DI | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5b15) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[1] = unaff_DI[1] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5b55) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x7a] = unaff_DI[0x7a] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5b55) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x7b] = unaff_DI[0x7b] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5b95) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0xf4] = unaff_DI[0xf4] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5b95) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0xf5] = unaff_DI[0xf5] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5bd5) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x16e] = unaff_DI[0x16e] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5bd5) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x16f] = unaff_DI[0x16f] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5c15) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x1e8] = unaff_DI[0x1e8] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5c15) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x1e9] = unaff_DI[0x1e9] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5c55) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x262] = unaff_DI[0x262] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5c55) << (8 - bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x263] = unaff_DI[0x263] | bVar2;
    bVar2 = *(byte *)(uVar4 + 0x5c95) >> (bVar3 & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x2dc] = unaff_DI[0x2dc] | bVar2;
    in_AH = 8 - bVar3;
    bVar2 = *(byte *)(uVar4 + 0x5c95) << (in_AH & 0x1f);
    out(0x3cf,bVar2);
    unaff_DI[0x2dd] = unaff_DI[0x2dd] | bVar2;
    bVar2 = bVar3 + 6;
    if ('\a' < (char)(bVar3 + 6)) {
      unaff_DI = unaff_DI + 1;
      bVar2 = bVar3 - 2;
    }
  }
  return;
}



/* ===== FUNCTION FUN_1000_5a6f @ 1000:5a6f ===== */

undefined4 __cdecl16near FUN_1000_5a6f(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  undefined2 in_AX;
  byte bVar3;
  int iVar4;
  int iVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined2 unaff_ES;
  
  puVar6 = (undefined1 *)0x0;
  puVar7 = (undefined1 *)0x4d84;
  out(0x3ce,5);
  out(0x3cf,0);
  out(0x3ce,1);
  out(0x3cf,0);
  out(0x3ce,8);
  out(0x3cf,0xff);
  iVar4 = 200;
  do {
    bVar3 = 1;
    do {
      out(0x3c4,2);
      out(0x3c5,bVar3);
      puVar8 = puVar7;
      for (iVar5 = 0x28; iVar5 != 0; iVar5 = iVar5 + -1) {
        puVar2 = puVar8;
        puVar8 = puVar8 + 1;
        puVar1 = puVar6;
        puVar6 = puVar6 + 1;
        *puVar2 = *puVar1;
      }
      puVar7 = puVar8 + -0x28;
      bVar3 = bVar3 << 1;
    } while ((bVar3 & 0xf) != 0);
    puVar7 = puVar8 + 0x52;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  out(0x3c4,2);
  out(0x3c5,0xf);
  out(0x3ce,8);
  out(0x3cf,0xff);
  out(0x3ce,1);
  out(0x3cf,0xf);
  return CONCAT22(0x3cf,CONCAT11(bVar3,0xf));
}



/* ===== FUNCTION FUN_1000_5ae3 @ 1000:5ae3 ===== */

void FUN_1000_5ae3(void)

{
  FUN_1000_5a6f();
  return;
}



/* ===== FUNCTION FUN_1000_61fc @ 1000:61fc ===== */

void __cdecl16near FUN_1000_61fc(void)

{
  undefined1 *puVar1;
  int in_CX;
  int iVar2;
  int in_DX;
  undefined1 *unaff_SI;
  undefined1 *puVar3;
  undefined1 *puVar4;
  undefined2 unaff_ES;
  
  FUN_1000_4c84();
  out(0x3ce,5);
  out(0x3cf,8);
  out(0x3ce,2);
  out(0x3cf,0xf);
  out(0x3ce,7);
  out(0x3cf,0xf);
  out(0x3ce,0);
  out(0x3cf,1);
  out(0x3ce,1);
  out(0x3cf,0xf);
  out(0x3ce,8);
  out(0x3cf,0);
  iVar2 = in_CX;
  puVar1 = unaff_SI;
  do {
    do {
      puVar4 = puVar1;
      puVar3 = unaff_SI;
      out(0x3cf,*puVar3);
      *puVar4 = *puVar3;
      iVar2 = iVar2 + -1;
      unaff_SI = puVar3 + 1;
      puVar1 = puVar4 + 1;
    } while (iVar2 != 0);
    in_DX = in_DX + -1;
    iVar2 = in_CX;
    unaff_SI = puVar3 + (0x7b - in_CX);
    puVar1 = puVar4 + (0x7b - in_CX);
  } while (in_DX != 0);
  out(0x3ce,8);
  out(0x3cf,0xff);
  out(0x3ce,5);
  out(0x3cf,1);
  FUN_1000_4c47();
  FUN_1000_4ca9();
  return;
}



/* ===== FUNCTION FUN_1000_6287 @ 1000:6287 ===== */

void __cdecl16near FUN_1000_6287(void)

{
  undefined1 *puVar1;
  int in_CX;
  int iVar2;
  int in_DX;
  undefined1 *unaff_SI;
  undefined1 *puVar3;
  undefined1 *puVar4;
  undefined2 unaff_ES;
  
  FUN_1000_4c84();
  out(0x3ce,5);
  out(0x3cf,8);
  out(0x3ce,2);
  out(0x3cf,1);
  out(0x3ce,7);
  out(0x3cf,0xf);
  out(0x3ce,0);
  out(0x3cf,0xf);
  out(0x3ce,1);
  out(0x3cf,0xf);
  out(0x3ce,8);
  out(0x3cf,0);
  iVar2 = in_CX;
  puVar1 = unaff_SI;
  do {
    do {
      puVar4 = puVar1;
      puVar3 = unaff_SI;
      out(0x3cf,*puVar3);
      *puVar4 = *puVar3;
      iVar2 = iVar2 + -1;
      unaff_SI = puVar3 + 1;
      puVar1 = puVar4 + 1;
    } while (iVar2 != 0);
    in_DX = in_DX + -1;
    iVar2 = in_CX;
    unaff_SI = puVar3 + (0x7b - in_CX);
    puVar1 = puVar4 + (0x7b - in_CX);
  } while (in_DX != 0);
  FUN_1000_4c47();
  FUN_1000_4ca9();
  return;
}



/* ===== FUNCTION FUN_1000_66d5 @ 1000:66d5 ===== */

void __cdecl16near FUN_1000_66d5(void)

{
  code *pcVar1;
  char in_AL;
  char *in_BX;
  int unaff_ES;
  undefined2 unaff_DS;
  
  *(char *)0x35b4 = *(char *)0x35b4 + in_AL;
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  if ((unaff_ES != 0 || in_BX != (char *)0x0) && (*in_BX != -0x31)) {
    pcVar1 = (code *)swi(0x33);
    (*pcVar1)();
    if ((in_BX == (char *)0x2) || (in_BX == (char *)0x3)) {
      *(undefined1 *)0x8177 = 1;
      pcVar1 = (code *)swi(0x33);
      (*pcVar1)();
      pcVar1 = (code *)swi(0x33);
      (*pcVar1)();
      pcVar1 = (code *)swi(0x33);
      (*pcVar1)();
      pcVar1 = (code *)swi(0x33);
      (*pcVar1)();
    }
  }
  *(undefined2 *)0x8171 = 0xa0;
  *(undefined2 *)0x8175 = 100;
  *(undefined2 *)0x8178 = 1;
  return;
}



/* ===== FUNCTION FUN_1000_6743 @ 1000:6743 ===== */

void __cdecl16near FUN_1000_6743(void)

{
  return;
}



/* ===== FUNCTION FUN_1000_67f4 @ 1000:67f4 ===== */

void __cdecl16near FUN_1000_67f4(void)

{
  int in_AX;
  int iVar1;
  int in_CX;
  int iVar2;
  undefined2 unaff_DS;
  bool in_ZF;
  
  if (!in_ZF) {
    if (*(char *)0x16bd == '\x01') {
      *(int *)0x8175 = *(int *)0x8175 + in_AX;
      in_CX = 0;
    }
    if (*(char *)0x16b8 == '\x01') {
      *(int *)0x8171 = *(int *)0x8171 - in_AX;
      in_CX = 0;
    }
    if (*(char *)0x16ba == '\x01') {
      *(int *)0x8171 = *(int *)0x8171 + in_AX;
      in_CX = 0;
    }
    if (in_CX == 0) {
      *(int *)0x8178 = *(int *)0x8178 + 1;
    }
    else {
      *(undefined2 *)0x8178 = 1;
    }
    if (0x40 < *(uint *)0x8178) {
      *(undefined2 *)0x8178 = 0x40;
    }
    iVar1 = *(int *)0x8171;
    iVar2 = *(int *)0x8175;
    if (iVar1 < 0x11) {
      iVar1 = 0x10;
      *(undefined2 *)0x8171 = 0x10;
    }
    if (0x12f < iVar1) {
      *(undefined2 *)0x8171 = 0x130;
    }
    if (iVar2 < 9) {
      iVar2 = 8;
      *(undefined2 *)0x8175 = 8;
    }
    if (0xbf < iVar2) {
      *(undefined2 *)0x8175 = 0xc0;
    }
  }
  return;
}



/* ===== FUNCTION FUN_1000_6803 @ 1000:6803 ===== */

/* WARNING: Instruction at (ram,0x00016813) overlaps instruction at (ram,0x00016811)
    */

undefined4 __cdecl16near FUN_1000_6803(undefined2 param_1)

{
  byte bVar1;
  undefined2 uVar2;
  undefined2 in_DX;
  undefined2 uVar3;
  byte bVar4;
  int in_BX;
  int unaff_DI;
  undefined2 unaff_DS;
  undefined2 in_stack_00000000;
  
  bVar1 = in(in_DX);
  uVar3 = 0x3c0;
  out(0x3c0,0x33);
  uVar2 = CONCAT11(bVar1,*(undefined1 *)0xd96);
  out(0x3c0,*(undefined1 *)0xd96);
  if ((bVar1 & 8) == 0) {
    while( true ) {
      *(undefined2 *)(in_BX + unaff_DI) = uVar2;
      bVar4 = (byte)((uint)uVar3 >> 8);
      uVar3 = CONCAT11(bVar4,0x90);
      bVar1 = in(uVar3);
      uVar2 = CONCAT11((char)((uint)uVar2 >> 8),bVar1);
      if ((bVar1 & 8) != 0) break;
      *(byte *)(unaff_DI + 0x25) = *(byte *)(unaff_DI + 0x25) | bVar4;
    }
    uVar2 = in_stack_00000000;
    uVar3 = param_1;
    if (*(char *)0x9857 != '\0') {
      out(0x3c0,0x33);
      out(0x3c0,*(undefined1 *)0xd96);
    }
  }
  return CONCAT22(uVar3,uVar2);
}



/* ===== FUNCTION FUN_1000_683c @ 1000:683c ===== */

undefined4 __cdecl16near FUN_1000_683c(void)

{
  byte bVar1;
  undefined2 in_AX;
  undefined2 in_DX;
  
  do {
    bVar1 = in(0x3da);
  } while ((bVar1 & 8) != 0);
  return CONCAT22(in_DX,in_AX);
}



/* ===== FUNCTION FUN_1000_684b @ 1000:684b ===== */

undefined1 __cdecl16near FUN_1000_684b(void)

{
  undefined1 uVar1;
  int in_DX;
  undefined2 unaff_DS;
  
  out(in_DX + 1,0xf);
  out(0x3ce,5);
  out(0x3cf,1);
  out(0x3c4,2);
  out(0x3c5,0xf);
  out(0x3ce,8);
  out(0x3cf,0xff);
  out(0x3d4,0x13);
  out(0x3d5,0x3d);
  if (*(char *)0x3814 == '\x01') {
    out(0x3d4,0x18);
    out(0x3d5,0x5f);
    out(0x3d4,7);
    out(0x3d5,0x3f);
    out(0x3d4,9);
    out(0x3d5,0x80);
    FUN_1000_67f4();
    in(0x3da);
    out(0x3c0,0x30);
    uVar1 = 0x21;
    out(0x3c0,0x21);
  }
  else {
    out(0x3d4,0x18);
    out(0x3d5,0xb0);
    out(0x3d4,7);
    uVar1 = 1;
    out(0x3d5,1);
  }
  return uVar1;
}



/* ===== FUNCTION FUN_1000_6ad5 @ 1000:6ad5 ===== */

void __cdecl16near FUN_1000_6ad5(void)

{
  undefined2 uVar1;
  char *pcVar2;
  char *pcVar3;
  undefined1 uVar4;
  int iVar5;
  char *unaff_SI;
  byte *pbVar6;
  char *pcVar7;
  undefined1 *puVar8;
  byte *pbVar9;
  undefined2 unaff_DS;
  int local_6;
  int local_4;
  
  if (*(char *)0x3814 != '\x02') {
    uVar1 = *(undefined2 *)0xd92;
    *(undefined2 *)0xd92 = 0;
    iVar5 = 0x40;
    pcVar7 = (char *)0x768;
    do {
      pcVar2 = unaff_SI;
      unaff_SI = unaff_SI + 1;
      pcVar3 = pcVar7;
      pcVar7 = pcVar7 + 1;
      *pcVar3 = *pcVar2 << 2;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    out(0x3c7,0);
    puVar8 = (undefined1 *)0x6115;
    iVar5 = 0x10;
    do {
      uVar4 = in(0x3c9);
      *puVar8 = uVar4;
      uVar4 = in(0x3c9);
      puVar8[1] = uVar4;
      uVar4 = in(0x3c9);
      puVar8[2] = uVar4;
      puVar8 = puVar8 + 4;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    local_4 = 0;
    local_6 = 0x3f;
    do {
      out(0x3c8,0);
      iVar5 = 0x10;
      pbVar6 = (byte *)0x768;
      pbVar9 = (byte *)0x6115;
      do {
        out(0x3c9,(char)(((uint)*pbVar9 * local_6 >> 6) + ((uint)*pbVar6 * local_4 >> 6)));
        out(0x3c9,(char)(((uint)pbVar9[1] * local_6 >> 6) + ((uint)pbVar6[1] * local_4 >> 6)));
        out(0x3c9,(char)((uint)pbVar9[2] * local_6 >> 6) + (char)((uint)pbVar6[2] * local_4 >> 6));
        pbVar6 = pbVar6 + 4;
        pbVar9 = pbVar9 + 4;
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
      if (*(char *)0x9fe6 != '\x01') {
        FUN_1000_67f4();
        FUN_1000_683c();
      }
      local_6 = local_6 + -1;
      local_4 = local_4 + 1;
    } while (local_4 < 0x40);
    FUN_1000_6c1b();
    *(undefined2 *)0xd92 = uVar1;
    return;
  }
  FUN_1000_6c1b();
  return;
}



/* ===== FUNCTION FUN_1000_6ae4 @ 1000:6ae4 ===== */

uint __cdecl16near FUN_1000_6ae4(void)

{
  uint in_AX;
  
  return in_AX ^ 0x701;
}



/* ===== FUNCTION FUN_1000_6c1b @ 1000:6c1b ===== */

void FUN_1000_6c1b(void)

{
  undefined2 uVar1;
  byte *pbVar2;
  code *pcVar3;
  int iVar4;
  char *unaff_SI;
  byte *pbVar5;
  undefined2 unaff_DS;
  
  uVar1 = *(undefined2 *)0xd92;
  *(undefined2 *)0xd92 = 0;
  if (*(char *)0x3814 == '\x01') {
    iVar4 = 0x10;
    out(0x3c8,0);
    do {
      out(0x3c9,*unaff_SI << 2);
      out(0x3c9,unaff_SI[1] << 2);
      out(0x3c9,unaff_SI[2] << 2);
      unaff_SI = unaff_SI + 4;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  else {
    iVar4 = 0x10;
    pbVar5 = (byte *)0x611;
    do {
      pbVar2 = (byte *)(unaff_SI + 3);
      unaff_SI = unaff_SI + 4;
      *pbVar5 = *pbVar2 & 7 | (*pbVar2 & 8) << 1;
      pbVar5 = pbVar5 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
    pcVar3 = (code *)swi(0x10);
    (*pcVar3)();
  }
  *(undefined2 *)0xd92 = uVar1;
  return;
}



/* ===== FUNCTION FUN_1000_6c2a @ 1000:6c2a ===== */

void __cdecl16near FUN_1000_6c2a(void)

{
  byte *pbVar1;
  code *pcVar2;
  int iVar3;
  int unaff_BP;
  char *unaff_SI;
  byte *pbVar4;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  
  if (*(char *)0x3814 == '\x01') {
    iVar3 = 0x10;
    out(0x3c8,0);
    do {
      out(0x3c9,*unaff_SI << 2);
      out(0x3c9,unaff_SI[1] << 2);
      out(0x3c9,unaff_SI[2] << 2);
      unaff_SI = unaff_SI + 4;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  else {
    iVar3 = 0x10;
    pbVar4 = (byte *)0x611;
    do {
      pbVar1 = (byte *)(unaff_SI + 3);
      unaff_SI = unaff_SI + 4;
      *pbVar4 = *pbVar1 & 7 | (*pbVar1 & 8) << 1;
      pbVar4 = pbVar4 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    pcVar2 = (code *)swi(0x10);
    (*pcVar2)();
  }
  *(undefined2 *)0xd92 = *(undefined2 *)(unaff_BP + -2);
  return;
}



/* ===== FUNCTION FUN_1000_6cd2 @ 1000:6cd2 ===== */

void __cdecl16near FUN_1000_6cd2(void)

{
  byte *pbVar1;
  code *pcVar2;
  int in_AX;
  int in_CX;
  undefined2 in_DX;
  byte bVar3;
  byte in_BH;
  byte unaff_BP;
  int unaff_SI;
  byte *unaff_DI;
  undefined2 unaff_SS;
  undefined2 unaff_DS;
  
  bVar3 = (char)((uint)in_DX >> 8) + unaff_BP;
  if ((int)((uint)bVar3 << 8) < 0) goto LAB_1000_6c6d;
  if ((byte)(bVar3 + unaff_BP) != '\0') {
    FUN_1000_6e96();
    FUN_1000_6e2c();
    *(undefined1 *)0x9577 = 1;
    *(undefined1 *)0x9578 = 1;
    *(undefined1 *)0x9574 = 1;
    FUN_1000_6ef0();
    *(undefined1 *)0x9579 = 0;
    *(undefined1 *)0x957a = 100;
    *(undefined1 *)0x957b = 0;
    *(undefined1 *)0x957c = 0;
    return;
  }
  while (in_CX = in_CX + -1, in_CX != 0) {
    pbVar1 = (byte *)(unaff_SI + 3);
    unaff_SI = unaff_SI + 4;
    in_BH = *pbVar1 & 7;
    unaff_BP = (*pbVar1 & 8) << 1;
LAB_1000_6c6d:
    *unaff_DI = in_BH | unaff_BP;
    unaff_DI = unaff_DI + 1;
  }
  pcVar2 = (code *)swi(0x10);
  (*pcVar2)();
  *(undefined2 *)0xd92 = *(undefined2 *)(in_AX + -2);
  return;
}



/* ===== FUNCTION FUN_1000_6e2c @ 1000:6e2c ===== */

void __cdecl16near FUN_1000_6e2c(void)

{
  code *pcVar1;
  undefined2 uVar2;
  undefined2 unaff_DS;
  undefined1 in_CF;
  
  pcVar1 = (code *)swi(0x21);
  uVar2 = (*pcVar1)();
  if ((bool)in_CF) {
    FUN_1000_04b7();
    return;
  }
  *(undefined2 *)0x766 = uVar2;
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  if ((bool)in_CF) {
    FUN_1000_04b7();
    return;
  }
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  if ((bool)in_CF) {
    FUN_1000_04b7();
    return;
  }
  return;
}



/* ===== FUNCTION FUN_1000_6e96 @ 1000:6e96 ===== */

void __cdecl16near FUN_1000_6e96(void)

{
  code *pcVar1;
  byte bVar2;
  undefined2 unaff_DS;
  
  *(undefined1 *)0x9574 = 0;
  if (*(char *)0x9577 == '\x01') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
    bVar2 = in(0x61);
    out(0x61,bVar2 & 0xfc);
  }
  else if (*(char *)0x9577 == '\x03') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
  }
  else if (*(char *)0x9577 == '\x05') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
  }
  *(undefined1 *)0x9577 = 0;
  if (*(char *)0x9578 == '\x02') {
    pcVar1 = (code *)swi(0x81);
    (*pcVar1)();
  }
  else if (*(char *)0x9578 == '\x04') {
    pcVar1 = (code *)swi(0x81);
    (*pcVar1)();
  }
  *(undefined1 *)0x9578 = 0;
  return;
}



/* ===== FUNCTION FUN_1000_6ef0 @ 1000:6ef0 ===== */

void __cdecl16near FUN_1000_6ef0(void)

{
  code *pcVar1;
  undefined2 unaff_DS;
  
  if (*(char *)0x9576 != '\x01') {
    return;
  }
  if (*(char *)0x9577 == '\x01') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
    *(undefined1 *)0x9574 = 1;
  }
  else if (*(char *)0x9577 == '\x03') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
    *(undefined1 *)0x9574 = 1;
  }
  else if (*(char *)0x9577 == '\x05') {
    pcVar1 = (code *)swi(0x80);
    (*pcVar1)();
    *(undefined1 *)0x9574 = 1;
  }
  return;
}



/* ===== FUNCTION FUN_1000_91e2 @ 1000:91e2 ===== */

void __cdecl16near FUN_1000_91e2(void)

{
  char cVar1;
  char cVar2;
  undefined2 unaff_DS;
  
  cVar1 = *(char *)0xda0;
  cVar2 = *(char *)0xda7;
  if (cVar2 != cVar1) {
    *(char *)0xda7 = cVar1;
    func_0x00015332();
    FUN_1000_94d8();
  }
  if (*(char *)0xda8 != *(char *)0xda1) {
    *(char *)0xda8 = *(char *)0xda1;
    func_0x00015332();
    FUN_1000_94d8();
  }
  if (*(char *)0xda9 != *(char *)0xda2) {
    *(char *)0xda9 = *(char *)0xda2;
    func_0x00015332();
    FUN_1000_94d8();
  }
  if ((*(char *)0x3814 == '\x02') && (cVar2 != cVar1)) {
    FUN_1000_9529();
  }
  return;
}



/* ===== FUNCTION FUN_1000_9281 @ 1000:9281 ===== */

undefined1 __cdecl16near FUN_1000_9281(void)

{
  byte *pbVar1;
  char cVar2;
  undefined1 in_AH;
  uint uVar3;
  char *unaff_SI;
  byte *unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  *(undefined1 *)0x1659 = in_AH;
  out(0x3ce,5);
  out(0x3cf,0);
  out(0x3ce,1);
  out(0x3cf,0xf);
  out(0x3ce,8);
  out(0x3cf,in_AH);
  while ((cVar2 = *unaff_SI, cVar2 != '\0' && (cVar2 != '\n'))) {
    unaff_SI = unaff_SI + 1;
    uVar3 = (uint)(byte)(cVar2 - 0x20);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    *unaff_DI = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5d15));
    *unaff_DI = *unaff_DI | *(byte *)(uVar3 + 0x5d15);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0x7a] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5d55));
    pbVar1 = unaff_DI + 0x7a;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5d55);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0xf4] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5d95));
    pbVar1 = unaff_DI + 0xf4;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5d95);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0x16e] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5dd5));
    pbVar1 = unaff_DI + 0x16e;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5dd5);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0x1e8] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5e15));
    pbVar1 = unaff_DI + 0x1e8;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5e15);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0x262] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5e55));
    pbVar1 = unaff_DI + 0x262;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5e55);
    out(0x3ce,0);
    out(0x3cf,0);
    out(0x3ce,8);
    out(0x3cf,0xff);
    unaff_DI[0x2dc] = 0xff;
    out(0x3ce,0);
    out(0x3cf,*(undefined1 *)0x1659);
    out(0x3ce,8);
    out(0x3cf,*(byte *)(uVar3 + 0x5e95));
    pbVar1 = unaff_DI + 0x2dc;
    *pbVar1 = *pbVar1 | *(byte *)(uVar3 + 0x5e95);
    unaff_DI = unaff_DI + 1;
  }
  out(0x3ce,5);
  out(0x3cf,1);
  out(0x3ce,8);
  out(0x3cf,0xff);
  return 0xff;
}



/* ===== FUNCTION FUN_1000_945c @ 1000:945c ===== */

void __cdecl16near FUN_1000_945c(void)

{
  char cVar1;
  uint uVar2;
  char *unaff_SI;
  undefined1 *unaff_DI;
  undefined2 unaff_DS;
  
  for (; (cVar1 = *unaff_SI, cVar1 != '\0' && (cVar1 != '\n')); unaff_SI = unaff_SI + 1) {
    uVar2 = (uint)(byte)(cVar1 - 0x20);
    *unaff_DI = *(undefined1 *)(uVar2 + 0x5d15);
    unaff_DI[0xa8] = *(undefined1 *)(uVar2 + 0x5d55);
    unaff_DI[0x150] = *(undefined1 *)(uVar2 + 0x5d95);
    unaff_DI[0x1f8] = *(undefined1 *)(uVar2 + 0x5dd5);
    unaff_DI[0x2a0] = *(undefined1 *)(uVar2 + 0x5e15);
    unaff_DI[0x348] = *(undefined1 *)(uVar2 + 0x5e55);
    unaff_DI[0x3f0] = *(undefined1 *)(uVar2 + 0x5e95);
    unaff_DI = unaff_DI + 1;
  }
  return;
}



/* ===== FUNCTION FUN_1000_94d8 @ 1000:94d8 ===== */

void __cdecl16near FUN_1000_94d8(void)

{
  undefined2 unaff_DS;
  
  if (*(char *)0x3814 == '\x01') {
    FUN_1000_9281();
  }
  else {
    FUN_1000_945c();
  }
  return;
}



/* ===== FUNCTION FUN_1000_9529 @ 1000:9529 ===== */

void __cdecl16near FUN_1000_9529(void)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  byte *pbVar6;
  int iVar7;
  int iVar8;
  
  pbVar5 = (byte *)0x20b;
  iVar7 = 0x11cb;
  iVar4 = 6;
  do {
    iVar3 = 0x1c;
    do {
      iVar8 = iVar7;
      pbVar6 = pbVar5;
      bVar1 = pbVar6[1];
      *(byte *)(iVar8 + 1) = bVar1 >> 1 | ((*pbVar6 & 1) != 0) << 7;
      bVar2 = pbVar6[2];
      *(byte *)(iVar8 + 2) = bVar2 >> 1 | ((bVar1 & 1) != 0) << 7;
      bVar1 = pbVar6[3];
      *(byte *)(iVar8 + 3) = bVar1 >> 1 | ((bVar2 & 1) != 0) << 7;
      bVar2 = pbVar6[4];
      *(byte *)(iVar8 + 4) = bVar2 >> 1 | ((bVar1 & 1) != 0) << 7;
      bVar1 = pbVar6[5];
      *(byte *)(iVar8 + 5) = bVar1 >> 1 | ((bVar2 & 1) != 0) << 7;
      bVar2 = pbVar6[6];
      *(byte *)(iVar8 + 6) = bVar2 >> 1 | ((bVar1 & 1) != 0) << 7;
      bVar1 = pbVar6[7];
      *(byte *)(iVar8 + 7) = bVar1 >> 1 | ((bVar2 & 1) != 0) << 7;
      bVar2 = pbVar6[8];
      *(byte *)(iVar8 + 8) = bVar2 >> 1 | ((bVar1 & 1) != 0) << 7;
      *(byte *)(iVar8 + 9) = pbVar6[9] >> 1 | ((bVar2 & 1) != 0) << 7;
      iVar3 = iVar3 + -1;
      pbVar5 = pbVar6 + 0x2a;
      iVar7 = iVar8 + 0x2a;
    } while (iVar3 != 0);
    pbVar5 = pbVar6 + 0xb52;
    iVar7 = iVar8 + 0xb52;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  return;
}



/* ===== FUNCTION entry @ 1c1b:0010 ===== */

void __cdecl16far entry(void)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  int unaff_ES;
  
  DAT_1c1b_0004 = unaff_ES + 0x10;
  DAT_6000_5c9e = DAT_1c1b_0004 + DAT_1c1b_000c;
  puVar4 = (undefined1 *)(DAT_1c1b_0006 + -1);
  puVar5 = puVar4;
  for (iVar3 = DAT_1c1b_0006; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar2 = puVar5;
    puVar5 = puVar5 + -1;
    puVar1 = puVar4;
    puVar4 = puVar4 + -1;
    *puVar2 = *puVar1;
  }
  DAT_6000_5c9c = 0x34;
  return;
}


