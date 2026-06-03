
/* ===== FORCED FUNCTION forced_1cfb:0032 @ 1cfb:0032 ===== */

/* WARNING: Control flow encountered bad instruction data */

void forced_1cfb_0032(void)

{
  uint *puVar1;
  byte *pbVar2;
  int *piVar3;
  char *pcVar4;
  byte *pbVar5;
  undefined1 uVar6;
  code *pcVar7;
  byte bVar8;
  int iVar9;
  undefined1 *puVar10;
  byte bVar11;
  int iVar12;
  int iVar13;
  byte *pbVar14;
  int *piVar15;
  char *pcVar16;
  byte *pbVar17;
  byte *pbVar18;
  int unaff_ES;
  undefined2 unaff_SS;
  int unaff_DS;
  undefined2 in_FS;
  undefined2 in_GS;
  byte bVar19;
  int in_stack_00000008;
  uint in_stack_0000000a;
  int in_stack_0000000e;
  undefined2 in_stack_00000010;
  undefined2 in_stack_00000012;
  undefined2 in_stack_00000014;
  
  iVar9 = unaff_DS + -1;
  pcVar16 = (char *)0xf;
  iVar12 = 0x10;
  do {
    if (iVar12 == 0) break;
    iVar12 = iVar12 + -1;
    pcVar4 = pcVar16;
    pcVar16 = pcVar16 + 1;
  } while (*pcVar4 == -1);
  pbVar17 = (byte *)(pcVar16 + 1);
  iVar12 = unaff_ES + -1;
  pbVar18 = (byte *)0xf;
  do {
    pbVar14 = pbVar17;
    if (~(uint)pbVar17 >> 4 != 0) {
      iVar9 = iVar9 - (~(uint)pbVar17 >> 4);
      pbVar14 = (byte *)((uint)pbVar17 | 0xfff0);
    }
    if (~(uint)pbVar18 >> 4 != 0) {
      iVar12 = iVar12 - (~(uint)pbVar18 >> 4);
      pbVar18 = (byte *)((uint)pbVar18 | 0xfff0);
    }
    bVar11 = *pbVar14;
    iVar13 = *(int *)pbVar14;
    pbVar17 = pbVar14 + 3;
    bVar8 = bVar11 & 0xfe;
    if (bVar8 == 0xb0) {
      pbVar2 = pbVar17;
      pbVar17 = pbVar14 + 4;
      bVar8 = *pbVar2;
      for (; iVar13 != 0; iVar13 = iVar13 + -1) {
        pbVar2 = pbVar18;
        pbVar18 = pbVar18 + 1;
        *pbVar2 = bVar8;
      }
    }
    else {
      bVar19 = bVar8 < 0xb2;
      if (bVar8 != 0xb2) {
        pcVar7 = (code *)swi(0x21);
        (*pcVar7)();
        pcVar7 = (code *)swi(0x21);
        puVar10 = (undefined1 *)(*pcVar7)();
        puVar1 = (uint *)(puVar10 + in_stack_0000000a + 0x65);
        *puVar1 = *puVar1 + (uint)bVar19 * ((in_stack_0000000a & 3) - (*puVar1 & 3));
        bVar11 = (byte)((uint)in_stack_00000014 >> 8);
        *(byte *)(in_stack_0000000a + 0x69) = *(byte *)(in_stack_0000000a + 0x69) & bVar11;
        uVar6 = in(in_stack_00000010);
        *puVar10 = uVar6;
        puVar10[in_stack_0000000e + 0x74] =
             puVar10[in_stack_0000000e + 0x74] & (byte)((uint)in_stack_00000012 >> 8);
        pbVar2 = puVar10 + in_stack_0000000a + 0x70;
        *pbVar2 = *pbVar2 & bVar11;
        if (*pbVar2 == 0) {
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_00000014;
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_00000014;
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_0000000e;
          *(char *)(in_stack_0000000a - 0x39) =
               *(char *)(in_stack_0000000a - 0x39) + (char)((uint)in_stack_00000010 >> 8);
                    /* WARNING: Bad instruction - Truncating control flow here */
          halt_baddata();
        }
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      for (; iVar13 != 0; iVar13 = iVar13 + -1) {
        pbVar5 = pbVar18;
        pbVar18 = pbVar18 + 1;
        pbVar2 = pbVar17;
        pbVar17 = pbVar17 + 1;
        *pbVar5 = *pbVar2;
      }
    }
    iVar13 = DAT_1cfb_0004;
  } while ((bVar11 & 1) == 0);
  piVar15 = (int *)0x112;
  iVar9 = 0;
  while( true ) {
    piVar3 = piVar15;
    piVar15 = piVar15 + 1;
    iVar12 = *piVar3;
    if (iVar12 != 0) {
      do {
        piVar3 = piVar15;
        piVar15 = piVar15 + 1;
        *(int *)*piVar3 = *(int *)*piVar3 + iVar13;
        iVar12 = iVar12 + -1;
      } while (iVar12 != 0);
    }
    if (iVar9 == -0x1000) break;
    iVar9 = iVar9 + 0x1000;
  }
  iRam0001cfb2 = iRam0001cfb2 + iVar13;
                    /* WARNING: Could not recover jumptable at 0x0001d093. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(ulong)uRam00010000)();
  return;
}



/* ===== FORCED FUNCTION forced_1cfb:0000 @ 1cfb:0000 ===== */

/* WARNING: Control flow encountered bad instruction data */

void forced_1cfb_0000(void)

{
  uint *puVar1;
  char *pcVar2;
  byte *pbVar3;
  undefined1 *puVar4;
  int *piVar5;
  undefined1 *puVar6;
  byte *pbVar7;
  undefined1 uVar8;
  code *pcVar9;
  byte bVar10;
  int in_AX;
  int iVar11;
  byte bVar12;
  int iVar13;
  int iVar14;
  undefined2 in_DX;
  int in_BX;
  int unaff_BP;
  int unaff_SI;
  byte *pbVar15;
  int *piVar16;
  int unaff_DI;
  undefined1 *puVar17;
  undefined1 *puVar18;
  byte *pbVar19;
  byte *pbVar20;
  int unaff_ES;
  undefined2 unaff_SS;
  int unaff_DS;
  undefined2 in_FS;
  undefined2 in_GS;
  byte bVar21;
  int in_stack_00000008;
  uint in_stack_0000000a;
  int in_stack_0000000e;
  undefined2 in_stack_00000010;
  undefined2 in_stack_00000012;
  undefined2 in_stack_00000014;
  
  pbVar3 = (byte *)(in_BX + unaff_SI);
  bVar12 = *pbVar3;
  bVar10 = (byte)in_AX;
  *pbVar3 = *pbVar3 + bVar10;
  if (CARRY1(bVar12,bVar10) || *pbVar3 == 0) {
    *(char *)(in_BX + unaff_SI) = *(char *)(in_BX + unaff_SI) + bVar10;
    *(int *)(in_BX + unaff_SI) = *(int *)(in_BX + unaff_SI) + in_AX;
    *(char *)(in_BX + unaff_SI) = *(char *)(in_BX + unaff_SI) + bVar10;
    pcVar2 = (char *)(unaff_BP + unaff_DI + 0x5245);
    *pcVar2 = *pcVar2 + (char)in_BX;
    DAT_1cfb_0004 = unaff_ES + 0x10;
    iVar11 = DAT_1cfb_0004 + DAT_1cfb_000c;
    puVar17 = (undefined1 *)(DAT_1cfb_0006 + -1);
    puVar18 = puVar17;
    for (iVar13 = DAT_1cfb_0006; iVar13 != 0; iVar13 = iVar13 + -1) {
      puVar6 = puVar18;
      puVar18 = puVar18 + -1;
      puVar4 = puVar17;
      puVar17 = puVar17 + -1;
      *puVar6 = *puVar4;
    }
    return;
  }
  pcVar2 = (char *)(in_BX + unaff_SI + -0xc01);
  *pcVar2 = *pcVar2 + (char)((uint)in_DX >> 8);
  pbVar19 = (byte *)(unaff_DI + 2);
  iVar13 = in_BX + -1;
  pbVar20 = (byte *)0xf;
  do {
    pbVar15 = pbVar19;
    if (~(uint)pbVar19 >> 4 != 0) {
      unaff_DS = unaff_DS - (~(uint)pbVar19 >> 4);
      pbVar15 = (byte *)((uint)pbVar19 | 0xfff0);
    }
    if (~(uint)pbVar20 >> 4 != 0) {
      iVar13 = iVar13 - (~(uint)pbVar20 >> 4);
      pbVar20 = (byte *)((uint)pbVar20 | 0xfff0);
    }
    bVar12 = *pbVar15;
    iVar11 = *(int *)pbVar15;
    pbVar19 = pbVar15 + 3;
    bVar10 = bVar12 & 0xfe;
    if (bVar10 == 0xb0) {
      pbVar3 = pbVar19;
      pbVar19 = pbVar15 + 4;
      bVar10 = *pbVar3;
      for (; iVar11 != 0; iVar11 = iVar11 + -1) {
        pbVar3 = pbVar20;
        pbVar20 = pbVar20 + 1;
        *pbVar3 = bVar10;
      }
    }
    else {
      bVar21 = bVar10 < 0xb2;
      if (bVar10 != 0xb2) {
        pcVar9 = (code *)swi(0x21);
        (*pcVar9)();
        pcVar9 = (code *)swi(0x21);
        puVar18 = (undefined1 *)(*pcVar9)();
        puVar1 = (uint *)(puVar18 + in_stack_0000000a + 0x65);
        *puVar1 = *puVar1 + (uint)bVar21 * ((in_stack_0000000a & 3) - (*puVar1 & 3));
        bVar12 = (byte)((uint)in_stack_00000014 >> 8);
        *(byte *)(in_stack_0000000a + 0x69) = *(byte *)(in_stack_0000000a + 0x69) & bVar12;
        uVar8 = in(in_stack_00000010);
        *puVar18 = uVar8;
        puVar18[in_stack_0000000e + 0x74] =
             puVar18[in_stack_0000000e + 0x74] & (byte)((uint)in_stack_00000012 >> 8);
        pbVar3 = puVar18 + in_stack_0000000a + 0x70;
        *pbVar3 = *pbVar3 & bVar12;
        if (*pbVar3 == 0) {
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_00000014;
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_00000014;
          *(char *)(in_stack_0000000e + in_stack_00000008) =
               *(char *)(in_stack_0000000e + in_stack_00000008) + (char)in_stack_0000000e;
          *(char *)(in_stack_0000000a - 0x39) =
               *(char *)(in_stack_0000000a - 0x39) + (char)((uint)in_stack_00000010 >> 8);
                    /* WARNING: Bad instruction - Truncating control flow here */
          halt_baddata();
        }
                    /* WARNING: Bad instruction - Truncating control flow here */
        halt_baddata();
      }
      for (; iVar11 != 0; iVar11 = iVar11 + -1) {
        pbVar7 = pbVar20;
        pbVar20 = pbVar20 + 1;
        pbVar3 = pbVar19;
        pbVar19 = pbVar19 + 1;
        *pbVar7 = *pbVar3;
      }
    }
    iVar11 = DAT_1cfb_0004;
  } while ((bVar12 & 1) == 0);
  piVar16 = (int *)0x112;
  iVar13 = 0;
  while( true ) {
    piVar5 = piVar16;
    piVar16 = piVar16 + 1;
    iVar14 = *piVar5;
    if (iVar14 != 0) {
      do {
        piVar5 = piVar16;
        piVar16 = piVar16 + 1;
        *(int *)*piVar5 = *(int *)*piVar5 + iVar11;
        iVar14 = iVar14 + -1;
      } while (iVar14 != 0);
    }
    if (iVar13 == -0x1000) break;
    iVar13 = iVar13 + 0x1000;
  }
  iRam0001cfb2 = iRam0001cfb2 + iVar11;
                    /* WARNING: Could not recover jumptable at 0x0001d093. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(ulong)uRam00010000)();
  return;
}


