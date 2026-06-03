
/* ===== FUNCTION FUN_0000_9314 @ 0000:9314 ===== */

void __cdecl16near FUN_0000_9314(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x1a);
  DAT_0000_0470 = (*pcVar1)();
  return;
}



/* ===== FUNCTION FUN_0000_9464 @ 0000:9464 ===== */

undefined4 __cdecl16near FUN_0000_9464(undefined2 param_1)

{
  code *pcVar1;
  undefined2 in_CX;
  
  pcVar1 = (code *)swi(0x33);
  (*pcVar1)();
  return CONCAT22(param_1,in_CX);
}



/* ===== FUNCTION FUN_0000_94a2 @ 0000:94a2 ===== */

void __cdecl16near FUN_0000_94a2(void)

{
  code *pcVar1;
  undefined2 unaff_ES;
  
  if (*(char *)0x119 != '\0') {
    pcVar1 = (code *)swi(0x33);
    (*pcVar1)();
  }
  return;
}



/* ===== FUNCTION FUN_0000_94b6 @ 0000:94b6 ===== */

ulong __cdecl16near FUN_0000_94b6(void)

{
  code *pcVar1;
  uint uVar2;
  undefined2 in_DX;
  byte bVar3;
  uint in_BX;
  undefined2 unaff_ES;
  
  uVar2 = 0;
  if (*(char *)0x119 != '\0') {
    pcVar1 = (code *)swi(0x33);
    uVar2 = in_BX;
    (*pcVar1)();
    if (*(char *)0xff != '\0') {
      bVar3 = (byte)uVar2;
      uVar2 = CONCAT11(bVar3 << 1,bVar3 >> 1) & 0x201;
      uVar2 = (uint)(byte)(bVar3 & 4 | (byte)uVar2 | (byte)(uVar2 >> 8));
    }
  }
  return CONCAT22(in_DX,uVar2) & 0xffff0007;
}



/* ===== FUNCTION FUN_0000_9573 @ 0000:9573 ===== */

void __cdecl16near FUN_0000_9573(void)

{
  return;
}



/* ===== FUNCTION FUN_0000_959d @ 0000:959d ===== */

undefined2 __cdecl16near FUN_0000_959d(void)

{
  undefined2 in_AX;
  
  return in_AX;
}



/* ===== FUNCTION FUN_0000_9657 @ 0000:9657 ===== */

undefined4 __cdecl16near FUN_0000_9657(void)

{
  char cVar1;
  undefined2 *puVar2;
  undefined2 *puVar3;
  undefined2 uVar4;
  code *pcVar5;
  byte bVar6;
  undefined2 in_AX;
  uint uVar7;
  undefined2 in_DX;
  uint extraout_DX;
  byte in_BL;
  undefined2 *unaff_DI;
  undefined2 *puVar8;
  undefined2 unaff_ES;
  
  FUN_0000_94a2();
  uVar4 = *(undefined2 *)0xac9;
  FUN_0000_959d();
  puVar8 = (undefined2 *)((int)unaff_DI + *(int *)0xac7 + -0x241a);
  cVar1 = *(char *)0xa82;
  if ((char)(extraout_DX >> 8) == '\x01') {
    uVar7 = extraout_DX & 0xff;
    do {
      if (cVar1 == '\0') {
        do {
          bVar6 = in(0x3da);
        } while ((bVar6 & 1) != 0);
        do {
          bVar6 = in(0x3da);
        } while ((bVar6 & 1) == 0);
      }
      puVar3 = puVar8;
      puVar8 = puVar8 + 1;
      puVar2 = unaff_DI;
      unaff_DI = unaff_DI + 1;
      *puVar3 = *puVar2;
      uVar7 = uVar7 - 1;
    } while (uVar7 != 0);
  }
  else {
    FUN_0000_96f8(extraout_DX,puVar8 + -(uint)in_BL);
  }
  if (*(char *)0xa83 != '\0') {
    pcVar5 = (code *)swi(0x10);
    (*pcVar5)();
  }
  FUN_0000_94a2();
  return CONCAT22(in_DX,in_AX);
}



/* ===== FUNCTION FUN_0000_96f8 @ 0000:96f8 ===== */

void __cdecl16near FUN_0000_96f8(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  char in_AL;
  byte bVar3;
  int iVar4;
  int iVar5;
  undefined2 in_DX;
  uint uVar6;
  char cVar7;
  undefined2 *unaff_SI;
  undefined2 *unaff_DI;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  
  uVar6 = CONCAT11((char)((uint)in_DX >> 8),7);
  while( true ) {
    bVar3 = (byte)(uVar6 >> 8);
    if (bVar3 < (byte)uVar6) {
      uVar6 = CONCAT11(bVar3,bVar3);
    }
    iVar4 = (uVar6 & 0xff) * 0x50;
    if (in_AL == '\0') break;
code_r0x00009724:
    for (; iVar4 != 0; iVar4 = iVar4 + -1) {
      puVar2 = unaff_DI;
      unaff_DI = unaff_DI + 1;
      puVar1 = unaff_SI;
      unaff_SI = unaff_SI + 1;
      *puVar2 = *puVar1;
    }
    cVar7 = (char)(uVar6 >> 8) - (char)uVar6;
    uVar6 = CONCAT11(cVar7,(char)uVar6);
    if (cVar7 == '\0') {
      return;
    }
  }
  do {
    bVar3 = in(0x3da);
  } while ((bVar3 & 1) != 0);
  do {
    iVar5 = 10;
    while (bVar3 = in(0x3da), (bVar3 & 1) != 0) {
      iVar5 = iVar5 + -1;
      if (iVar5 == 0) goto code_r0x00009724;
    }
  } while( true );
}



/* ===== FUNCTION FUN_0000_a28f @ 0000:a28f ===== */

void __cdecl16near FUN_0000_a28f(void)

{
  FUN_0000_a298();
  return;
}



/* ===== FUNCTION FUN_0000_a298 @ 0000:a298 ===== */

undefined2 __cdecl16near FUN_0000_a298(void)

{
  undefined2 in_AX;
  
  return in_AX;
}



/* ===== FUNCTION FUN_0000_a2d8 @ 0000:a2d8 ===== */

void __cdecl16near FUN_0000_a2d8(void)

{
  undefined1 *puVar1;
  undefined2 *puVar2;
  undefined2 in_AX;
  int in_CX;
  undefined1 *unaff_SI;
  undefined2 *unaff_DI;
  undefined2 unaff_ES;
  
  do {
    puVar1 = unaff_SI;
    unaff_SI = unaff_SI + 1;
    in_AX = CONCAT11((char)((uint)in_AX >> 8),*puVar1);
    puVar2 = unaff_DI;
    unaff_DI = unaff_DI + 1;
    *puVar2 = in_AX;
    in_CX = in_CX + -1;
  } while (in_CX != 0);
  return;
}



/* ===== FUNCTION FUN_0000_a700 @ 0000:a700 ===== */

undefined2 __cdecl16near FUN_0000_a700(void)

{
  code *pcVar1;
  undefined2 in_AX;
  undefined2 unaff_ES;
  undefined2 in_stack_00000000;
  
  if (*(char *)0xf2 == '\0') {
    *(undefined1 *)0xa88 = 0xff;
    return in_AX;
  }
  pcVar1 = (code *)swi(0x16);
  (*pcVar1)();
  FUN_0000_a733();
  return in_stack_00000000;
}



/* ===== FUNCTION FUN_0000_a733 @ 0000:a733 ===== */

undefined4 __cdecl16near FUN_0000_a733(void)

{
  undefined1 uVar1;
  byte bVar2;
  byte bVar3;
  undefined2 in_AX;
  int iVar4;
  undefined2 uVar5;
  undefined2 in_DX;
  undefined2 extraout_DX;
  undefined2 *unaff_DI;
  undefined2 *puVar6;
  undefined2 unaff_ES;
  
  bVar2 = (char)in_AX << 2 | (byte)((uint)in_AX >> 8);
  bVar3 = 0;
  if (bVar2 < 0x3c) {
    bVar3 = ((undefined *)&DAT_0000_e1f9)[bVar2];
  }
  if (bVar3 != *(byte *)0xa88) {
    *(byte *)0xa88 = bVar3;
    iVar4 = (uint)bVar3 * (CONCAT11(10,bVar3) >> 8);
    FUN_0000_959d();
    FUN_0000_a28f();
    uVar5 = FUN_0000_a28f();
    bVar3 = 0x31;
    do {
      puVar6 = unaff_DI;
      uVar5 = CONCAT11((char)((uint)uVar5 >> 8),bVar3);
      while( true ) {
        *puVar6 = uVar5;
        FUN_0000_a2d8(iVar4);
        iVar4 = iVar4 + 1;
        uVar1 = (undefined1)((uint)extraout_DX >> 8);
        uVar5 = CONCAT11(uVar1,0x20);
        unaff_DI = puVar6 + 2;
        puVar6[1] = uVar5;
        bVar3 = (char)extraout_DX + 1;
        if (bVar3 < 0x3a) break;
        if (0x3a < bVar3) {
          FUN_0000_9657();
          goto LAB_0000_a7b0;
        }
        puVar6 = puVar6 + 3;
        *unaff_DI = CONCAT11(uVar1,0x31);
        uVar5 = CONCAT11(uVar1,0x30);
      }
    } while( true );
  }
LAB_0000_a7b0:
  return CONCAT22(in_DX,in_AX);
}



/* ===== FUNCTION FUN_0000_ae8c @ 0000:ae8c ===== */

int __cdecl16near FUN_0000_ae8c(void)

{
  byte in_AL;
  
  return CONCAT11(in_AL % 10,in_AL / 10) + 0x3030;
}



/* ===== FUNCTION FUN_0000_b131 @ 0000:b131 ===== */

/* WARNING: Removing unreachable block (ram,0x0000b38f) */
/* WARNING: Removing unreachable block (ram,0x0000b397) */
/* WARNING: Removing unreachable block (ram,0x0000b3da) */
/* WARNING: Removing unreachable block (ram,0x0000b3e6) */
/* WARNING: Removing unreachable block (ram,0x0000b3ed) */
/* WARNING: Removing unreachable block (ram,0x0000b3f7) */

undefined4 __cdecl16near FUN_0000_b131(void)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  code *pcVar3;
  uint uVar4;
  uint uVar5;
  undefined1 uVar6;
  byte bVar7;
  undefined2 uVar8;
  undefined1 extraout_AH;
  byte bVar9;
  uint in_CX;
  int iVar10;
  byte extraout_DL;
  uint extraout_DX;
  uint extraout_DX_00;
  uint extraout_DX_01;
  uint uVar11;
  uint extraout_DX_02;
  uint extraout_DX_03;
  undefined2 extraout_DX_04;
  uint extraout_DX_05;
  uint extraout_DX_06;
  uint uVar12;
  uint extraout_DX_07;
  char cVar13;
  uint in_BX;
  undefined1 uVar14;
  undefined2 unaff_SI;
  undefined2 *puVar15;
  undefined2 *puVar16;
  undefined2 unaff_ES;
  undefined2 unaff_SS;
  bool bVar17;
  ulong uVar18;
  uint local_20;
  uint local_1e;
  undefined2 local_1a;
  undefined2 local_18 [6];
  char local_c;
  char local_a;
  byte local_8;
  char local_6;
  
  bVar17 = &stack0x0000 == (undefined1 *)0x20;
  pcVar3 = (code *)swi(0x16);
  (*pcVar3)();
  if (bVar17) goto LAB_0000_b14f;
  pcVar3 = (code *)swi(0x16);
  uVar8 = (*pcVar3)();
LAB_0000_b34c:
  do {
    *(undefined2 *)0xac3 = 0xffff;
    if (((char)((uint)uVar8 >> 8) != -4) || (*(char *)0xf2 == '\0')) {
LAB_0000_b3d3:
      return CONCAT22(unaff_SI,uVar8);
    }
    uVar6 = FUN_0000_94b6();
    uVar8 = CONCAT11(0xfc,uVar6);
    if ((char)((char)(in_CX >> 8) + '\x01') != *(char *)0xa81) goto LAB_0000_b3d3;
    in_BX = in_CX & 0xf8;
    while( true ) {
      uVar6 = (undefined1)in_BX;
      bVar7 = FUN_0000_9573(in_BX);
      in_BX = CONCAT11(bVar7,uVar6);
      if ((((bVar7 & 2) == 0) && ((bVar7 & 4) == 0)) && ((bVar7 & 1) == 0)) break;
      FUN_0000_a733();
      FUN_0000_94b6();
      uVar6 = FUN_0000_959d();
      if (*(char *)0x0 == ' ') {
        uVar8 = CONCAT11(0xfc,uVar6);
        goto LAB_0000_b3d3;
      }
    }
LAB_0000_b14f:
    bVar17 = true;
    uVar6 = FUN_0000_94b6();
    uVar8 = CONCAT11(0xfc,uVar6);
    if (bVar17) {
      FUN_0000_9314();
      puVar15 = (undefined2 *)0x24ae;
      puVar16 = local_18;
      for (iVar10 = 6; iVar10 != 0; iVar10 = iVar10 + -1) {
        puVar2 = puVar16;
        puVar16 = puVar16 + 1;
        puVar1 = puVar15;
        puVar15 = puVar15 + 1;
        *puVar2 = *puVar1;
      }
      pcVar3 = (code *)swi(0x16);
      local_8 = (*pcVar3)();
      local_a = '\0';
      uVar12 = extraout_DX_00;
      local_20 = extraout_DX;
      local_1e = in_CX;
      do {
        local_c = '\0';
        local_6 = '\0';
LAB_0000_b183:
        in_CX = 0xffff;
        local_1a = in_CX;
        uVar11 = uVar12;
        if (*(char *)0xfb != '\0') {
          pcVar3 = (code *)swi(0x21);
          (*pcVar3)();
          cVar13 = (char)(in_CX >> 8);
          bVar7 = (char)in_CX + *(char *)0xfb;
          in_CX = CONCAT11(cVar13,bVar7);
          local_1a = in_CX;
          uVar11 = extraout_DX_01;
          if (0x3b < bVar7) {
            in_CX = CONCAT11(cVar13 + '\x01',bVar7 - 0x3c);
            local_1a = in_CX;
          }
        }
        while( true ) {
          FUN_0000_a700();
          FUN_0000_b40a();
          if (*(char *)0xfb != '\0') break;
LAB_0000_b1ea:
          uVar6 = (undefined1)(in_BX >> 8);
          if (*(char *)0xf7 != '\0') {
            puVar16 = (undefined2 *)0x24b0;
            pcVar3 = (code *)swi(0x21);
            (*pcVar3)();
            if (*(char *)0xbc0 == '\0') {
              puVar16 = (undefined2 *)0x24ae;
              uVar6 = 0x61;
              if ((byte)((byte)(in_CX >> 8) / 0xc) != 0) {
                uVar6 = 0x70;
              }
            }
            cVar13 = ' ';
            if (0x31 < extraout_DL) {
              cVar13 = *(char *)0xbbc;
            }
            in_BX = CONCAT11(uVar6,cVar13);
            if (cVar13 != local_c) {
              local_c = cVar13;
              FUN_0000_ae8c();
              in_CX = FUN_0000_ae8c();
              FUN_0000_a28f();
              cVar13 = (char)extraout_DX_04;
              if (cVar13 == '0') {
                cVar13 = ' ';
              }
              *puVar16 = CONCAT11(extraout_AH,cVar13);
              puVar16[1] = CONCAT11(extraout_AH,(char)((uint)extraout_DX_04 >> 8));
              puVar16[2] = CONCAT11(extraout_AH,(char)in_BX);
              puVar16[3] = CONCAT11(extraout_AH,(char)in_CX);
              puVar16[4] = CONCAT11(extraout_AH,(char)(in_CX >> 8));
              if (*(char *)0xbc0 == '\0') {
                puVar16[5] = CONCAT11(extraout_AH,(char)(in_BX >> 8));
              }
              in_BX = 0;
              FUN_0000_9657();
            }
          }
          uVar6 = *(int *)0xac3 == -1;
          if (!(bool)uVar6) {
            pcVar3 = (code *)swi(0x21);
            (*pcVar3)();
            uVar8 = 0xfe00;
            uVar12 = *(uint *)0xac3;
            uVar6 = in_CX == uVar12;
            in_BX = extraout_DX_05;
            if ((in_CX < uVar12) ||
               ((in_CX <= uVar12 &&
                (uVar6 = extraout_DX_05 == *(uint *)0xac5, extraout_DX_05 < *(uint *)0xac5))))
            goto LAB_0000_b286;
LAB_0000_b32d:
            if (*(char *)0xf7 != '\0') {
              puVar16 = local_18;
              puVar15 = (undefined2 *)0x24ae;
              for (in_CX = 6; in_CX != 0; in_CX = in_CX - 1) {
                puVar2 = puVar15;
                puVar15 = puVar15 + 1;
                puVar1 = puVar16;
                puVar16 = puVar16 + 1;
                *puVar2 = *puVar1;
              }
              uVar8 = FUN_0000_9657();
            }
            goto LAB_0000_b34c;
          }
LAB_0000_b286:
          uVar14 = (undefined1)(in_BX >> 8);
          FUN_0000_b5ca();
          pcVar3 = (code *)swi(0x16);
          (*pcVar3)();
          if (!(bool)uVar6) {
            pcVar3 = (code *)swi(0x16);
            uVar8 = (*pcVar3)();
            goto LAB_0000_b32d;
          }
          bVar17 = true;
          uVar6 = FUN_0000_94b6();
          uVar8 = CONCAT11(0xfc,uVar6);
          if (!bVar17) goto LAB_0000_b32d;
          pcVar3 = (code *)swi(0x16);
          uVar18 = (*pcVar3)();
          uVar12 = (uint)(uVar18 >> 0x10);
          bVar7 = (byte)uVar18;
          LOCK();
          in_BX = CONCAT11(uVar14,local_8);
          UNLOCK();
          uVar4 = local_20;
          uVar5 = local_1e;
          local_8 = bVar7;
          if ((*(char *)0xa84 == '\0') && (*(char *)0x102 != '\0')) {
            FUN_0000_9314();
            LOCK();
            UNLOCK();
            LOCK();
            UNLOCK();
            uVar12 = local_20;
            uVar4 = extraout_DX_06;
            uVar5 = in_CX;
            if (((in_CX - local_1e) - (uint)(extraout_DX_06 < local_20)) * 2 +
                (uint)(2 < extraout_DX_06 - local_20) != 0) goto LAB_0000_b2ef;
            bVar9 = (byte)in_BX ^ bVar7;
            uVar4 = extraout_DX_06;
            uVar5 = in_CX;
            if ((bVar9 == 0) || (uVar4 = extraout_DX_06, uVar5 = in_CX, bVar9 != 8))
            goto LAB_0000_b2ef;
            in_BX = CONCAT11(1,(byte)in_BX);
            local_20 = extraout_DX_06;
            uVar4 = in_CX;
            if ((uVar18 & 8) == 0) {
              uVar4 = extraout_DX_06;
              uVar5 = in_CX;
              if (local_a != '\0') {
                uVar8 = 0xfd00;
                in_CX = local_1e;
                goto LAB_0000_b32d;
              }
              goto LAB_0000_b2ef;
            }
LAB_0000_b327:
            local_1e = uVar4;
            local_a = (char)(in_BX >> 8);
            goto LAB_0000_b183;
          }
LAB_0000_b2ef:
          local_1e = uVar5;
          local_20 = uVar4;
          in_BX = in_BX & 0xfff0;
          bVar7 = bVar7 ^ (byte)in_BX;
          if (bVar7 != 0) {
            if ((bVar7 & 0xf7) != 0) {
              in_BX = 0;
              uVar4 = local_1e;
              goto LAB_0000_b327;
            }
            goto LAB_0000_b183;
          }
          uVar6 = 0;
          in_CX = 0;
          FUN_0000_94b6();
          FUN_0000_9573();
          if ((bool)uVar6) {
            local_6 = '\x01';
            bVar17 = true;
          }
          else {
            bVar17 = false;
            if (local_6 != '\0') goto LAB_0000_b1d3;
          }
          in_BX = (uint)(byte)(*(char *)0xa81 - 1) << 8;
          FUN_0000_9573();
          uVar12 = extraout_DX_07;
          if (!bVar17) goto LAB_0000_b183;
        }
        pcVar3 = (code *)swi(0x21);
        (*pcVar3)();
        if (((char)(in_CX >> 8) == '\0') && (0x17 < local_1a._1_1_)) {
          local_1a = local_1a & 0xff;
        }
        if ((in_CX < local_1a) || ((in_CX <= local_1a && (extraout_DX_02 < uVar11))))
        goto LAB_0000_b1ea;
LAB_0000_b1d3:
        FUN_0000_b438();
        in_BX = 0;
        FUN_0000_9657();
        do {
          bVar17 = true;
          FUN_0000_94b6();
          uVar12 = extraout_DX_03;
        } while (!bVar17);
      } while( true );
    }
  } while( true );
}



/* ===== FUNCTION FUN_0000_b40a @ 0000:b40a ===== */

void __cdecl16near FUN_0000_b40a(void)

{
  code *pcVar1;
  char cVar2;
  undefined2 unaff_DS;
  
  pcVar1 = (code *)swi(0x28);
  (*pcVar1)();
  if (*(char *)0xa85 == '\0') {
    pcVar1 = (code *)swi(0x2f);
    cVar2 = (*pcVar1)();
    if (cVar2 == -0x80) {
      *(undefined1 *)0xa85 = 1;
    }
  }
  if (*(char *)0xa84 != '\0') {
    pcVar1 = (code *)swi(0x15);
    (*pcVar1)();
    pcVar1 = (code *)swi(0x15);
    (*pcVar1)();
    pcVar1 = (code *)swi(0x15);
    (*pcVar1)();
  }
  return;
}



/* ===== FUNCTION FUN_0000_b438 @ 0000:b438 ===== */

undefined4 __cdecl16near FUN_0000_b438(void)

{
  int *piVar1;
  int *piVar2;
  undefined2 *puVar3;
  code *pcVar4;
  byte bVar5;
  byte bVar6;
  uint uVar7;
  undefined2 in_CX;
  int iVar8;
  uint uVar9;
  int iVar10;
  int iVar11;
  int extraout_DX;
  char cVar12;
  int *piVar13;
  int *piVar14;
  undefined2 *puVar15;
  int iVar16;
  undefined2 unaff_ES;
  undefined2 unaff_DS;
  undefined1 uVar17;
  bool bVar18;
  undefined4 uVar19;
  undefined2 in_stack_00000012;
  undefined2 in_stack_00000018;
  
  FUN_0000_94a2();
  pcVar4 = (code *)swi(0x10);
  (*pcVar4)();
  pcVar4 = (code *)swi(0x10);
  (*pcVar4)();
  pcVar4 = (code *)swi(0x10);
  (*pcVar4)(in_CX);
  FUN_0000_a28f();
  pcVar4 = (code *)swi(0x10);
  (*pcVar4)();
  puVar15 = (undefined2 *)0xe7b;
  for (iVar8 = 0x40; iVar8 != 0; iVar8 = iVar8 + -1) {
    puVar3 = puVar15;
    puVar15 = puVar15 + 1;
    *puVar3 = 0;
  }
  iVar8 = 0;
  FUN_0000_94b6();
  pcVar4 = (code *)swi(0x16);
  bVar5 = (*pcVar4)();
  uVar9 = (uint)bVar5;
  iVar16 = 0;
LAB_0000_b484:
  FUN_0000_b5ca();
  FUN_0000_b5ca();
  uVar7 = FUN_0000_b5ca();
  bVar5 = (byte)(uVar7 >> 3);
  if (99 < bVar5) {
    uVar19 = FUN_0000_b5ca();
    iVar11 = (int)((ulong)uVar19 >> 0x10);
    bVar6 = (byte)uVar19 >> 5 | (byte)uVar19 << 3;
    if (0x31 < bVar6) {
      piVar14 = (int *)0xe7b;
      iVar10 = 0x20;
      piVar13 = (int *)0x0;
      do {
        piVar1 = piVar14;
        piVar2 = piVar14 + 1;
        piVar14 = piVar14 + 2;
        if (*piVar1 == 0) {
          if (piVar13 == (int *)0x0) {
            piVar13 = piVar14;
          }
        }
        else if (*piVar2 == iVar11) goto LAB_0000_b4fe;
        iVar10 = iVar10 + -1;
      } while (iVar10 != 0);
      if (piVar13 != (int *)0x0) {
        piVar13[-2] = CONCAT11(bVar6 | (byte)((ulong)uVar19 >> 8),bVar5) & 0x1ff;
        piVar13[-1] = iVar11;
        pcVar4 = (code *)swi(0x10);
        (*pcVar4)();
        pcVar4 = (code *)swi(0x10);
        (*pcVar4)();
        FUN_0000_a28f();
        pcVar4 = (code *)swi(0x10);
        (*pcVar4)();
      }
    }
  }
LAB_0000_b4fe:
  iVar11 = 0x20;
  piVar14 = (int *)0xe7b;
  do {
    uVar17 = *piVar14 == 0;
    if (!(bool)uVar17) {
      iVar10 = *piVar14 + -1;
      cVar12 = (char)iVar10;
      *(char *)piVar14 = cVar12;
      uVar17 = cVar12 == '\f';
      if (!(bool)uVar17) {
        if (cVar12 == '\t') {
          uVar17 = true;
          if ((char)((uint)iVar10 >> 8) == '\0') goto LAB_0000_b54b;
          iVar10 = 0;
        }
        cVar12 = (char)iVar10;
        uVar17 = cVar12 == '\x06';
        if ((!(bool)uVar17) && (uVar17 = cVar12 == '\x03', !(bool)uVar17)) {
          uVar17 = cVar12 == '\0';
          if (!(bool)uVar17) goto LAB_0000_b563;
          uVar17 = true;
          *piVar14 = 0;
          piVar14[1] = 0;
        }
      }
LAB_0000_b54b:
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
      FUN_0000_a28f();
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
    }
LAB_0000_b563:
    iVar11 = iVar11 + -1;
    piVar14 = piVar14 + 2;
  } while (iVar11 != 0);
  do {
    bVar5 = (byte)uVar9;
    pcVar4 = (code *)swi(0x16);
    (*pcVar4)();
    if (!(bool)uVar17) {
      pcVar4 = (code *)swi(0x16);
      (*pcVar4)();
LAB_0000_b5af:
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
      pcVar4 = (code *)swi(0x10);
      (*pcVar4)();
      FUN_0000_94a2();
      return CONCAT22(in_stack_00000018,in_stack_00000012);
    }
    pcVar4 = (code *)swi(0x16);
    bVar6 = (*pcVar4)();
    uVar9 = (uint)bVar6;
    if ((bVar5 & 0xf0) != bVar6) goto LAB_0000_b5af;
    uVar17 = 0;
    iVar11 = 0;
    bVar18 = true;
    FUN_0000_94b6();
    if (!bVar18) goto LAB_0000_b5af;
    FUN_0000_9573();
    if (!(bool)uVar17) {
      iVar8 = iVar11;
    }
    if (iVar11 != iVar8) goto LAB_0000_b5af;
    FUN_0000_9314();
    uVar17 = iVar16 == extraout_DX;
    iVar16 = extraout_DX;
    if (!(bool)uVar17) goto LAB_0000_b484;
    FUN_0000_b40a();
  } while( true );
}



/* ===== FUNCTION FUN_0000_b5ca @ 0000:b5ca ===== */

void __cdecl16near FUN_0000_b5ca(void)

{
  int iVar1;
  int iVar2;
  undefined2 unaff_ES;
  
  iVar1 = *(int *)0xab7;
  iVar2 = *(int *)0xab9;
  *(int *)0xab7 = iVar2;
  *(int *)0xab9 = iVar2 * 3 + iVar1 * 3 + 0x5ba1;
  return;
}


