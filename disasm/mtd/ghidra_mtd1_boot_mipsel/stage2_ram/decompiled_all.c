
/* ===== FUNCTION stage2_entry_cp0_cache_and_handoff @ 89eb0000 =====
 * High confidence: RAM-resident stage2 entry. Saves boot arguments, edits CP0 config registers, clears a small BSS-like range, sets stack/return trampoline, and jumps onward.
 */

/* WARNING: This function may have set the stack pointer */
/* High confidence: RAM-resident stage2 entry. Saves boot arguments, edits CP0 config registers,
   clears a small BSS-like range, sets stack/return trampoline, and jumps onward. */

void stage2_entry_cp0_cache_and_handoff(void)

{
  bool bVar1;
  undefined4 *puVar2;
  
  setCopReg(0,Config,Config & 0xf1ff7ff8 | 0x6230003);
  setCopReg(0,cop0_reg22,cop0_reg22 | 1);
  stage2_jump_to_cached_alias(0);
  stage2_jump_to_cached_alias(1);
  puVar2 = (undefined4 *)&DAT_89f15da0;
  do {
    *puVar2 = 0;
    bVar1 = puVar2 != (undefined4 *)0x89f15dac;
    puVar2 = puVar2 + 1;
  } while (bVar1);
  stage2_main_handoff_wrapper(0x89f15dac);
  return;
}



/* ===== FUNCTION sub_89eb00d8 @ 89eb00d8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb00d8(void)

{
  stage2_main_handoff_wrapper();
  return;
}



/* ===== FUNCTION stage2_jump_to_cached_alias @ 89eb0118 =====
 * Medium confidence: builds a KSEG0/cached alias using 0xa0000000 masking and jumps through it.
 */

/* Medium confidence: builds a KSEG0/cached alias using 0xa0000000 masking and jumps through it. */

void stage2_jump_to_cached_alias(void)

{
  (*(code *)&LAB_a9eb0130)();
  return;
}



/* ===== FUNCTION stage2_cache_probe_and_invalidate @ 89eb0130 =====
 * Medium confidence: reads CP0 Config1, derives cache geometry, and uses MIPS cache operations over the cache ways/sets.
 */

/* Medium confidence: reads CP0 Config1, derives cache geometry, and uses MIPS cache operations over
   the cache ways/sets. */

void stage2_cache_probe_and_invalidate(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  
  if ((param_1 == 0) && (uVar1 = (uint)(Config1 << 10) >> 0x1d, uVar1 != 0)) {
    iVar3 = (0x40 << ((uint)(Config1 << 7) >> 0x1d)) * (((uint)(Config1 << 0xd) >> 0x1d) + 1);
    iVar2 = -0x80000000;
    setCopReg(0,TagLo,0);
    setCopReg(0,TagHi,0);
    do {
      cacheOp(8,iVar2);
      iVar3 = iVar3 + -1;
      iVar2 = iVar2 + (2 << uVar1);
    } while (iVar3 != 0);
  }
  if ((param_1 == 1) && (uVar1 = (uint)(Config1 << 0x13) >> 0x1d, uVar1 != 0)) {
    iVar3 = (0x40 << ((uint)(Config1 << 0x10) >> 0x1d)) * (((uint)(Config1 << 0x16) >> 0x1d) + 1);
    iVar2 = -0x80000000;
    setCopReg(0,TagLo,0);
    setCopReg(0,TagHi,0);
    setCopReg(0,TagLo_2,0);
    setCopReg(0,TagHi_2,0);
    do {
      cacheOp(9,iVar2);
      iVar3 = iVar3 + -1;
      iVar2 = iVar2 + (2 << uVar1);
    } while (iVar3 != 0);
  }
  return;
}



/* ===== FUNCTION FUN_89eb0298 @ 89eb0298 =====
 * Touches CP0/cache state.
 */

void FUN_89eb0298(uint param_1,int param_2,int param_3)

{
  bool bVar1;
  uint uVar2;
  
  if (0 < param_2) {
    uVar2 = param_1 & 0xffffffe0;
    do {
      cacheOp(0x19,uVar2);
      bVar1 = uVar2 != ((param_1 + param_2) - 1 & 0xffffffe0);
      uVar2 = uVar2 + param_3;
    } while (bVar1);
    SYNC(0);
    SYNC(0);
  }
  return;
}



/* ===== FUNCTION sub_89eb02f8 @ 89eb02f8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb02f8(void)

{
  return;
}



/* ===== FUNCTION sub_89eb02fc @ 89eb02fc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb02fc(void)

{
  return;
}



/* ===== FUNCTION sub_89eb0308 @ 89eb0308 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb0308(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION sub_89eb0310 @ 89eb0310 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb0310(void)

{
  return;
}



/* ===== FUNCTION sub_89eb0318 @ 89eb0318 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_89eb0318(int param_1)

{
  _DAT_89f15da8 = _DAT_89f15da8 + param_1;
  return;
}



/* ===== FUNCTION stage2_main_handoff_wrapper @ 89eb032c =====
 * Medium confidence: main wrapper after cache setup. Prepares stack locals and calls deeper loader/decompressor routines.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Medium confidence: main wrapper after cache setup. Prepares stack locals and calls deeper
   loader/decompressor routines. */

void stage2_main_handoff_wrapper(int param_1)

{
  undefined4 uVar1;
  undefined4 auStack_10 [3];
  
  _DAT_89f15da4 = param_1 + 0x400000;
  auStack_10[0] = 0;
  _DAT_89f15da8 = param_1;
  FUN_89eb2dcc(0x89eb2e48,0x62f4f,0,0,&LAB_89da0000,0,0,auStack_10,0x89eb0300);
  uVar1 = 0x10;
  if (((_DAT_b8800000 & 0xffff0000) != 0x15120000) && ((_DAT_b8800000 & 0xffff0000) == 0x16000000))
  {
    uVar1 = 0x20;
  }
  FUN_89eb0298(&LAB_89da0000,auStack_10[0],uVar1);
  return;
}



/* ===== FUNCTION sub_89eb0348 @ 89eb0348 =====
 * References 0x89da..., likely handoff or external image entry/data.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_89eb0348(void)

{
  int in_v0;
  undefined4 uVar1;
  undefined4 auStack_10 [3];
  
  auStack_10[0] = 0;
  FUN_89eb2dcc(0x89eb2e48,0x62f4f,0,0,&LAB_89da0000,0,0,auStack_10,in_v0 + 0x300);
  uVar1 = 0x10;
  if (((_DAT_b8800000 & 0xffff0000) != 0x15120000) && ((_DAT_b8800000 & 0xffff0000) == 0x16000000))
  {
    uVar1 = 0x20;
  }
  FUN_89eb0298(&LAB_89da0000,auStack_10[0],uVar1);
  return;
}



/* ===== FUNCTION sub_89eb03dc @ 89eb03dc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb03dc(void)

{
  return 0;
}



/* ===== FUNCTION sub_89eb03e0 @ 89eb03e0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb03e0(void)

{
  return 0;
}



/* ===== FUNCTION FUN_89eb03e8 @ 89eb03e8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined1 * FUN_89eb03e8(undefined1 *param_1,undefined1 *param_2,int param_3)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  puVar2 = param_2 + param_3;
  puVar1 = param_1;
  for (; param_2 != puVar2; param_2 = param_2 + 1) {
    *puVar1 = *param_2;
    puVar1 = puVar1 + 1;
  }
  return param_1;
}



/* ===== FUNCTION sub_89eb0404 @ 89eb0404 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb0404(undefined4 param_1,undefined1 *param_2,undefined1 *param_3)

{
  undefined1 uVar1;
  int in_v1;
  
  do {
    uVar1 = *param_2;
    param_2 = param_2 + 1;
    *(undefined1 *)(in_v1 + -1) = uVar1;
    in_v1 = in_v1 + 1;
  } while (param_2 != param_3);
  return;
}



/* ===== FUNCTION FUN_89eb0420 @ 89eb0420 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_89eb0420(undefined4 param_1,undefined1 param_2,undefined1 *param_3)

{
  undefined1 *in_v1;
  
  if (in_v1 == param_3) {
    return;
  }
  *in_v1 = param_2;
  FUN_89eb0420();
  return;
}



/* ===== FUNCTION sub_89eb0430 @ 89eb0430 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb0430(undefined4 param_1,undefined1 param_2)

{
  undefined1 *in_v1;
  
  *in_v1 = param_2;
  FUN_89eb0420();
  return;
}



/* ===== FUNCTION FUN_89eb043c @ 89eb043c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_89eb043c(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  undefined1 *puVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  
  uVar1 = *(uint *)(param_1 + 0x48);
  if (uVar1 - 1 < 0x111) {
    iVar3 = *(int *)(param_1 + 0x24);
    iVar6 = *(int *)(param_1 + 0x14);
    iVar8 = *(int *)(param_1 + 0x28);
    uVar7 = *(uint *)(param_1 + 0x38);
    uVar4 = param_2 - iVar3;
    if (uVar1 < (uint)(param_2 - iVar3)) {
      uVar4 = uVar1;
    }
    if ((*(int *)(param_1 + 0x30) == 0) &&
       ((uint)(*(int *)(param_1 + 0xc) - *(int *)(param_1 + 0x2c)) <= uVar4)) {
      *(int *)(param_1 + 0x30) = *(int *)(param_1 + 0xc);
    }
    *(uint *)(param_1 + 0x2c) = uVar4 + *(int *)(param_1 + 0x2c);
    *(uint *)(param_1 + 0x48) = uVar1 - uVar4;
    puVar5 = (undefined1 *)(iVar6 + iVar3);
    uVar1 = uVar4;
    while (uVar1 != 0) {
      uVar1 = uVar1 - 1;
      iVar2 = iVar8;
      if (uVar7 <= (uint)((int)puVar5 - iVar6)) {
        iVar2 = 0;
      }
      *puVar5 = puVar5[iVar2 - uVar7];
      puVar5 = puVar5 + 1;
    }
    *(uint *)(param_1 + 0x24) = iVar3 + uVar4;
  }
  return;
}



/* ===== FUNCTION sub_89eb04c8 @ 89eb04c8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb04c8(int param_1,int param_2,undefined1 *param_3,int param_4)

{
  int iVar1;
  int in_v1;
  int in_t0;
  uint in_t1;
  uint in_t2;
  int in_t3;
  int in_t4;
  
  do {
    in_t0 = in_t0 + -1;
    iVar1 = in_t3;
    if (in_t2 <= in_t1) {
      iVar1 = 0;
    }
    *param_3 = param_3[(param_4 + iVar1) - in_t4];
    in_t1 = (int)(param_3 + 1) - param_4;
    param_3 = param_3 + 1;
  } while (in_t0 != 0);
  *(int *)(param_1 + 0x24) = in_v1 + param_2;
  return;
}



/* ===== FUNCTION FUN_89eb04f8 @ 89eb04f8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 FUN_89eb04f8(uint *param_1,uint param_2,byte *param_3)

{
  ushort uVar1;
  bool bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  byte *pbVar6;
  char cVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  int iVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  ushort *puVar25;
  uint uVar26;
  undefined1 *puVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint uVar31;
  ushort *puVar32;
  uint uVar33;
  uint local_54;
  uint local_50;
  
  do {
    uVar3 = param_1[0xc];
    uVar21 = param_1[0xb];
    uVar16 = param_1[9];
    local_50 = param_2;
    if ((uVar3 == 0) && (local_50 = param_2, param_1[3] - uVar21 < param_2 - uVar16)) {
      local_50 = (param_1[3] - uVar21) + uVar16;
    }
    uVar8 = param_1[2];
    local_54 = param_1[0x11];
    uVar9 = param_1[1];
    uVar22 = param_1[4];
    uVar4 = *param_1;
    iVar10 = uVar22 + 0x644;
    uVar26 = param_1[10];
    uVar17 = param_1[0xd];
    uVar23 = param_1[0xe];
    uVar31 = param_1[0xf];
    uVar28 = param_1[0x10];
    uVar29 = param_1[5];
    pbVar6 = (byte *)param_1[6];
    uVar20 = param_1[7];
    uVar5 = param_1[8];
    uVar15 = 0;
LAB_89eb0608:
    uVar18 = ~(-1 << (uVar8 & 0x1f)) & uVar21;
    puVar32 = (ushort *)(uVar22 + (uVar18 + uVar17 * 0x10) * 2);
    uVar1 = *puVar32;
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
    uVar20 = uVar20 - uVar11;
    if (uVar5 < uVar11) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
      iVar19 = uVar22 + 0xe6c;
      if (uVar3 != 0 || uVar21 != 0) {
        uVar20 = uVar16;
        if (uVar16 == 0) {
          uVar20 = uVar26;
        }
        iVar19 = iVar19 + (((int)(uint)*(byte *)(uVar29 + (uVar20 - 1)) >> (8 - uVar4 & 0x1f)) +
                          ((uVar21 & ~(-1 << (uVar9 & 0x1f))) << (uVar4 & 0x1f))) * 0x600;
      }
      uVar21 = uVar21 + 1;
      if (uVar17 < 7) {
        uVar18 = uVar17;
        if (3 < uVar17) {
          uVar18 = 3;
        }
        uVar1 = *(ushort *)(iVar19 + 2);
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 * 0x100;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar20) {
          *(ushort *)(iVar19 + 2) = uVar1 + (short)(0x800 - uVar1 >> 5);
          iVar12 = 2;
        }
        else {
          uVar5 = uVar5 - uVar20;
          *(ushort *)(iVar19 + 2) = uVar1 - (uVar1 >> 5);
          uVar20 = uVar11 - uVar20;
          iVar12 = 3;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar11) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar11;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar11 = uVar20 - uVar11;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar20) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar20;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar20 = uVar11 - uVar20;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar11) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar11;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar11 = uVar20 - uVar11;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar20) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar20;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar20 = uVar11 - uVar20;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar11) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar11;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar11 = uVar20 - uVar11;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar1 = *puVar32;
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar33 = (uVar11 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar33) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        }
        else {
          uVar5 = uVar5 - uVar33;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar33 = uVar11 - uVar33;
        }
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(iVar19 + iVar12);
        uVar11 = (uint)*puVar32;
        if (uVar33 < 0x1000000) {
          uVar33 = uVar33 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar33 >> 0xb) * uVar11;
      }
      else {
        uVar20 = 0;
        if (uVar16 < uVar23) {
          uVar20 = uVar26;
        }
        uVar20 = (uint)*(byte *)(uVar29 + (uVar16 - uVar23) + uVar20);
        uVar18 = 6;
        if (uVar17 < 10) {
          uVar18 = 3;
        }
        uVar30 = uVar20 << 1 & 0x100;
        puVar32 = (ushort *)(iVar19 + (uVar30 + 0x101) * 2);
        uVar1 = *puVar32;
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 * 0x100;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar33 = (uVar11 >> 0xb) * (uint)uVar1;
        if (uVar5 < uVar33) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar30 = ~(uVar20 << 1) & 0x100;
          iVar12 = 2;
        }
        else {
          uVar5 = uVar5 - uVar33;
          *puVar32 = uVar1 - (uVar1 >> 5);
          uVar33 = uVar11 - uVar33;
          iVar12 = 3;
        }
        uVar11 = uVar20 << 2 & uVar30;
        puVar32 = (ushort *)(iVar19 + (uVar30 + iVar12 + uVar11) * 2);
        uVar1 = *puVar32;
        if (uVar33 < 0x1000000) {
          uVar33 = uVar33 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar24 = (uVar33 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar24) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar11 = ~uVar11 & uVar30;
        }
        else {
          uVar5 = uVar5 - uVar24;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar24 = uVar33 - uVar24;
        }
        uVar30 = uVar20 << 3 & uVar11;
        puVar32 = (ushort *)(iVar19 + (uVar11 + iVar12 + uVar30) * 2);
        uVar1 = *puVar32;
        if (uVar24 < 0x1000000) {
          uVar24 = uVar24 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar33 = (uVar24 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar33) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar30 = ~uVar30 & uVar11;
        }
        else {
          uVar5 = uVar5 - uVar33;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar33 = uVar24 - uVar33;
        }
        uVar11 = uVar20 << 4 & uVar30;
        puVar32 = (ushort *)(iVar19 + (uVar30 + iVar12 + uVar11) * 2);
        uVar1 = *puVar32;
        if (uVar33 < 0x1000000) {
          uVar33 = uVar33 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar24 = (uVar33 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar24) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar11 = ~uVar11 & uVar30;
        }
        else {
          uVar5 = uVar5 - uVar24;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar24 = uVar33 - uVar24;
        }
        uVar30 = uVar20 << 5 & uVar11;
        puVar32 = (ushort *)(iVar19 + (uVar11 + iVar12 + uVar30) * 2);
        uVar1 = *puVar32;
        if (uVar24 < 0x1000000) {
          uVar24 = uVar24 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar33 = (uVar24 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar33) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar30 = ~uVar30 & uVar11;
        }
        else {
          uVar5 = uVar5 - uVar33;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar33 = uVar24 - uVar33;
        }
        uVar11 = uVar20 << 6 & uVar30;
        puVar32 = (ushort *)(iVar19 + (uVar30 + iVar12 + uVar11) * 2);
        uVar1 = *puVar32;
        if (uVar33 < 0x1000000) {
          uVar33 = uVar33 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar24 = (uVar33 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar24) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar11 = ~uVar11 & uVar30;
        }
        else {
          uVar5 = uVar5 - uVar24;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar24 = uVar33 - uVar24;
        }
        uVar30 = uVar20 << 7 & uVar11;
        puVar32 = (ushort *)(iVar19 + (uVar11 + iVar12 + uVar30) * 2);
        uVar1 = *puVar32;
        if (uVar24 < 0x1000000) {
          uVar24 = uVar24 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar33 = (uVar24 >> 0xb) * (uint)uVar1;
        iVar12 = iVar12 * 2;
        if (uVar5 < uVar33) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar30 = ~uVar30 & uVar11;
        }
        else {
          uVar5 = uVar5 - uVar33;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar33 = uVar24 - uVar33;
        }
        puVar32 = (ushort *)(iVar19 + ((uVar20 << 8 & uVar30) + uVar30 + iVar12) * 2);
        uVar11 = (uint)*puVar32;
        if (uVar33 < 0x1000000) {
          uVar33 = uVar33 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar33 >> 0xb) * uVar11;
        iVar12 = iVar12 << 1;
      }
      uVar17 = uVar17 - uVar18;
      cVar7 = (char)iVar12;
      if (uVar5 < uVar20) {
        *puVar32 = (short)uVar11 + (short)(0x800 - uVar11 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar20;
        *puVar32 = (short)uVar11 - (short)(uVar11 >> 5);
        cVar7 = cVar7 + '\x01';
        uVar20 = uVar33 - uVar20;
      }
      *(char *)(uVar29 + uVar16) = cVar7;
      uVar16 = uVar16 + 1;
      goto LAB_89eb0964;
    }
    uVar5 = uVar5 - uVar11;
    *puVar32 = uVar1 - (uVar1 >> 5);
    iVar19 = (uVar17 + 0xc0) * 2;
    puVar32 = (ushort *)(uVar22 + iVar19);
    uVar1 = *puVar32;
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 * 0x100;
      uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
      pbVar6 = pbVar6 + 1;
    }
    uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
    uVar20 = uVar20 - uVar11;
    uVar30 = uVar31;
    uVar33 = uVar28;
    if (uVar5 < uVar11) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
      uVar17 = uVar17 + 0xc;
      puVar32 = (ushort *)(uVar22 + 0x664);
    }
    else {
      uVar5 = uVar5 - uVar11;
      *puVar32 = uVar1 - (uVar1 >> 5);
      if (uVar3 == 0 && uVar21 == 0) {
        return 1;
      }
      puVar32 = (ushort *)(uVar22 + iVar19 + 0x18);
      uVar1 = *puVar32;
      if (uVar20 < 0x1000000) {
        uVar20 = uVar20 * 0x100;
        uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
        pbVar6 = pbVar6 + 1;
      }
      uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
      uVar20 = uVar20 - uVar11;
      if (uVar5 < uVar11) {
        *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
        puVar32 = (ushort *)(uVar22 + (uVar17 * 0x10 + 0xf0 + uVar18) * 2);
        uVar1 = *puVar32;
        if (uVar11 < 0x1000000) {
          uVar11 = uVar11 * 0x100;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
        uVar11 = uVar11 - uVar20;
        if (uVar5 < uVar20) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar18 = 0;
          if (uVar16 < uVar23) {
            uVar18 = uVar26;
          }
          uVar21 = uVar21 + 1;
          *(undefined1 *)(uVar29 + uVar16) = *(undefined1 *)(uVar29 + (uVar16 - uVar23) + uVar18);
          bVar2 = uVar17 < 7;
          uVar16 = uVar16 + 1;
          uVar17 = 0xb;
          if (bVar2) {
            uVar17 = 9;
          }
          goto LAB_89eb0964;
        }
        uVar5 = uVar5 - uVar20;
        *puVar32 = uVar1 - (uVar1 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar11;
        *puVar32 = uVar1 - (uVar1 >> 5);
        puVar32 = (ushort *)(uVar22 + iVar19 + 0x30);
        uVar1 = *puVar32;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 * 0x100;
          uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
          pbVar6 = pbVar6 + 1;
        }
        uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
        uVar20 = uVar20 - uVar11;
        uVar30 = uVar23;
        if (uVar5 < uVar11) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar23 = uVar31;
        }
        else {
          uVar5 = uVar5 - uVar11;
          *puVar32 = uVar1 - (uVar1 >> 5);
          puVar32 = (ushort *)(uVar22 + iVar19 + 0x48);
          uVar1 = *puVar32;
          if (uVar20 < 0x1000000) {
            uVar20 = uVar20 * 0x100;
            uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
            pbVar6 = pbVar6 + 1;
          }
          uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
          uVar33 = uVar31;
          if (uVar5 < uVar11) {
            *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
            uVar23 = uVar28;
          }
          else {
            uVar5 = uVar5 - uVar11;
            *puVar32 = uVar1 - (uVar1 >> 5);
            uVar11 = uVar20 - uVar11;
            uVar23 = local_54;
            local_54 = uVar28;
          }
        }
      }
      bVar2 = uVar17 < 7;
      uVar17 = 0xb;
      puVar32 = (ushort *)(uVar22 + 0xa68);
      if (bVar2) {
        uVar17 = 8;
      }
    }
    uVar1 = *puVar32;
    if (uVar11 < 0x1000000) {
      uVar11 = uVar11 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
    uVar11 = uVar11 - uVar20;
    uVar31 = uVar23;
    uVar28 = uVar33;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
      uVar1 = puVar32[uVar18 * 8 + 3];
      if (uVar20 < 0x1000000) {
        uVar20 = uVar20 * 0x100;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar15 = (uVar20 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar15) {
        puVar32[uVar18 * 8 + 3] = uVar1 + (short)(0x800 - uVar1 >> 5);
        iVar19 = 2;
      }
      else {
        uVar5 = uVar5 - uVar15;
        puVar32[uVar18 * 8 + 3] = uVar1 - (uVar1 >> 5);
        uVar15 = uVar20 - uVar15;
        iVar19 = 3;
      }
      iVar12 = iVar19 * 2;
      puVar25 = puVar32 + uVar18 * 8 + iVar19 + 2;
      uVar1 = *puVar25;
      if (uVar15 < 0x1000000) {
        uVar15 = uVar15 << 8;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar11 = (uVar15 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar11) {
        *puVar25 = uVar1 + (short)(0x800 - uVar1 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar11;
        *puVar25 = uVar1 - (uVar1 >> 5);
        iVar12 = iVar12 + 1;
        uVar11 = uVar15 - uVar11;
      }
      iVar19 = iVar12 * 2;
      puVar32 = puVar32 + uVar18 * 8 + iVar12 + 2;
      uVar1 = *puVar32;
      if (uVar11 < 0x1000000) {
        uVar11 = uVar11 << 8;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar20) {
        *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar20;
        *puVar32 = uVar1 - (uVar1 >> 5);
        iVar19 = iVar19 + 1;
        uVar20 = uVar11 - uVar20;
      }
      uVar15 = iVar19 - 8;
      if (0xb < uVar17) {
        iVar19 = 0x4e0;
        if (uVar15 < 4) {
          iVar19 = uVar15 * 0x80 + 0x360;
        }
        iVar19 = uVar22 + iVar19;
        goto LAB_89eb1578;
      }
LAB_89eb1018:
      uVar23 = local_50 - uVar16;
      uVar15 = uVar15 + 2;
      if (uVar23 == 0) {
LAB_89eb1814:
        param_1[9] = uVar16;
        return 1;
      }
      uVar18 = uVar15;
      if (uVar23 < uVar15) {
        uVar18 = uVar23;
      }
      uVar11 = 0;
      if (uVar16 < uVar31) {
        uVar11 = uVar26;
      }
      uVar11 = (uVar16 - uVar31) + uVar11;
      uVar21 = uVar21 + uVar18;
      uVar15 = uVar15 - uVar18;
      puVar13 = (undefined1 *)(uVar29 + uVar16);
      uVar33 = uVar18;
      uVar23 = uVar31;
      uVar31 = uVar30;
      if (uVar26 - uVar11 < uVar18) {
        do {
          puVar27 = (undefined1 *)(uVar29 + uVar11);
          uVar11 = uVar11 + 1;
          *puVar13 = *puVar27;
          if (uVar26 == uVar11) {
            uVar11 = 0;
          }
          uVar33 = uVar33 - 1;
          puVar13 = puVar13 + 1;
        } while (uVar33 != 0);
        uVar16 = uVar16 + uVar18;
      }
      else {
        iVar19 = uVar11 - uVar16;
        uVar16 = uVar16 + uVar18;
        puVar27 = puVar13;
        do {
          puVar14 = puVar27 + 1;
          *puVar27 = puVar27[iVar19];
          puVar27 = puVar14;
        } while (puVar13 + uVar18 != puVar14);
      }
LAB_89eb0964:
      if ((local_50 <= uVar16) || (param_3 <= pbVar6)) goto LAB_89eb1a84;
      goto LAB_89eb0608;
    }
    *puVar32 = uVar1 - (uVar1 >> 5);
    uVar5 = uVar5 - uVar20;
    uVar1 = puVar32[1];
    if (uVar11 < 0x1000000) {
      uVar11 = uVar11 * 0x100;
      uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
      pbVar6 = pbVar6 + 1;
    }
    uVar15 = (uVar11 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar15) {
      puVar32[1] = uVar1 + (short)(0x800 - uVar1 >> 5);
      uVar1 = puVar32[uVar18 * 8 + 0x83];
      if (uVar15 < 0x1000000) {
        uVar15 = uVar15 * 0x100;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar20 = (uVar15 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar20) {
        puVar32[uVar18 * 8 + 0x83] = uVar1 + (short)(0x800 - uVar1 >> 5);
        iVar19 = 2;
      }
      else {
        uVar5 = uVar5 - uVar20;
        puVar32[uVar18 * 8 + 0x83] = uVar1 - (uVar1 >> 5);
        uVar20 = uVar15 - uVar20;
        iVar19 = 3;
      }
      iVar12 = iVar19 * 2;
      puVar25 = puVar32 + uVar18 * 8 + iVar19 + 0x82;
      uVar1 = *puVar25;
      if (uVar20 < 0x1000000) {
        uVar20 = uVar20 << 8;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar11 = (uVar20 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar11) {
        *puVar25 = uVar1 + (short)(0x800 - uVar1 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar11;
        *puVar25 = uVar1 - (uVar1 >> 5);
        iVar12 = iVar12 + 1;
        uVar11 = uVar20 - uVar11;
      }
      uVar15 = iVar12 * 2;
      puVar32 = puVar32 + uVar18 * 8 + iVar12 + 0x82;
      uVar1 = *puVar32;
      if (uVar11 < 0x1000000) {
        uVar11 = uVar11 << 8;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar20 = (uVar11 >> 0xb) * (uint)uVar1;
      if (uVar5 < uVar20) {
        *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
      }
      else {
        uVar5 = uVar5 - uVar20;
        *puVar32 = uVar1 - (uVar1 >> 5);
        uVar15 = uVar15 + 1;
        uVar20 = uVar11 - uVar20;
      }
    }
    else {
      uVar20 = uVar11 - uVar15;
      uVar5 = uVar5 - uVar15;
      puVar32[1] = uVar1 - (uVar1 >> 5);
      uVar18 = 1;
      do {
        uVar15 = uVar18 * 2;
        puVar25 = puVar32 + uVar18 + 0x102;
        uVar1 = *puVar25;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar18 = (uVar20 >> 0xb) * (uint)uVar1;
        uVar20 = uVar20 - uVar18;
        if (uVar5 < uVar18) {
          *puVar25 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar20 = uVar18;
        }
        else {
          uVar5 = uVar5 - uVar18;
          *puVar25 = uVar1 - (uVar1 >> 5);
          uVar15 = uVar15 + 1;
        }
        uVar18 = uVar15;
      } while (uVar15 < 0x100);
      uVar15 = uVar15 - 0xf0;
    }
    if (uVar17 < 0xc) goto LAB_89eb1018;
    iVar19 = uVar22 + 0x4e0;
LAB_89eb1578:
    uVar1 = *(ushort *)(iVar19 + 2);
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar31 = (uVar20 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar31) {
      *(ushort *)(iVar19 + 2) = uVar1 + (short)(0x800 - uVar1 >> 5);
      iVar12 = 2;
    }
    else {
      uVar5 = uVar5 - uVar31;
      *(ushort *)(iVar19 + 2) = uVar1 - (uVar1 >> 5);
      uVar31 = uVar20 - uVar31;
      iVar12 = 3;
    }
    iVar12 = iVar12 * 2;
    puVar32 = (ushort *)(iVar19 + iVar12);
    uVar1 = *puVar32;
    if (uVar31 < 0x1000000) {
      uVar31 = uVar31 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar31 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar20;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar12 = iVar12 + 1;
      uVar20 = uVar31 - uVar20;
    }
    iVar12 = iVar12 * 2;
    puVar32 = (ushort *)(iVar19 + iVar12);
    uVar1 = *puVar32;
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar31 = (uVar20 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar31) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar31;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar12 = iVar12 + 1;
      uVar31 = uVar20 - uVar31;
    }
    iVar12 = iVar12 * 2;
    puVar32 = (ushort *)(iVar19 + iVar12);
    uVar1 = *puVar32;
    if (uVar31 < 0x1000000) {
      uVar31 = uVar31 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar31 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar20;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar12 = iVar12 + 1;
      uVar20 = uVar31 - uVar20;
    }
    iVar12 = iVar12 * 2;
    puVar32 = (ushort *)(iVar19 + iVar12);
    uVar1 = *puVar32;
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar31 = (uVar20 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar31) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar31;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar12 = iVar12 + 1;
      uVar31 = uVar20 - uVar31;
    }
    iVar12 = iVar12 * 2;
    puVar32 = (ushort *)(iVar19 + iVar12);
    uVar1 = *puVar32;
    if (uVar31 < 0x1000000) {
      uVar31 = uVar31 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar31 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar20;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar12 = iVar12 + 1;
      uVar20 = uVar31 - uVar20;
    }
    uVar18 = iVar12 - 0x40;
    if (uVar18 < 4) {
LAB_89eb1800:
      uVar28 = uVar3;
      if (uVar3 == 0) {
        uVar28 = uVar21;
      }
      if (uVar28 <= uVar18) goto LAB_89eb1814;
      uVar31 = uVar18 + 1;
      uVar28 = uVar30;
      local_54 = uVar33;
      if (uVar17 < 0x13) {
        uVar17 = 7;
        uVar30 = uVar23;
      }
      else {
        uVar17 = 10;
        uVar30 = uVar23;
      }
      goto LAB_89eb1018;
    }
    uVar31 = uVar18 & 1 | 2;
    if (uVar18 < 0xe) {
      uVar28 = (uVar18 >> 1) - 1;
      uVar31 = uVar31 << (uVar28 & 0x1f);
      iVar19 = 0x2af - uVar18;
      iVar12 = 1;
      uVar11 = 1;
      uVar18 = uVar31;
      do {
        iVar12 = iVar12 * 2;
        puVar32 = (ushort *)(uVar22 + (iVar19 + uVar31) * 2 + iVar12);
        uVar1 = *puVar32;
        if (uVar20 < 0x1000000) {
          uVar20 = uVar20 << 8;
          uVar5 = (uint)*pbVar6 | uVar5 << 8;
          pbVar6 = pbVar6 + 1;
        }
        uVar24 = (uVar20 >> 0xb) * (uint)uVar1;
        uVar20 = uVar20 - uVar24;
        if (uVar5 < uVar24) {
          *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
          uVar20 = uVar24;
        }
        else {
          uVar5 = uVar5 - uVar24;
          *puVar32 = uVar1 - (uVar1 >> 5);
          iVar12 = iVar12 + 1;
          uVar18 = uVar18 | uVar11;
        }
        uVar28 = uVar28 - 1;
        uVar11 = uVar11 << 1;
      } while (uVar28 != 0);
      goto LAB_89eb1800;
    }
    iVar19 = (uVar18 >> 1) - 5;
    do {
      if (uVar20 < 0x1000000) {
        uVar20 = uVar20 << 8;
        uVar5 = (uint)*pbVar6 | uVar5 << 8;
        pbVar6 = pbVar6 + 1;
      }
      uVar20 = uVar20 >> 1;
      uVar18 = (int)(uVar5 - uVar20) >> 0x1f;
      uVar31 = uVar18 + uVar31 * 2 + 1;
      iVar19 = iVar19 + -1;
      uVar5 = (uVar20 & uVar18) + (uVar5 - uVar20);
    } while (iVar19 != 0);
    uVar18 = uVar31 * 0x10;
    uVar1 = *(ushort *)(uVar22 + 0x646);
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 * 0x100;
      pbVar6 = pbVar6 + 1;
    }
    uVar31 = (uVar20 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar31) {
      *(ushort *)(uVar22 + 0x646) = uVar1 + (short)(0x800 - uVar1 >> 5);
      iVar19 = 2;
    }
    else {
      uVar5 = uVar5 - uVar31;
      *(ushort *)(uVar22 + 0x646) = uVar1 - (uVar1 >> 5);
      uVar18 = uVar18 | 1;
      uVar31 = uVar20 - uVar31;
      iVar19 = 3;
    }
    iVar19 = iVar19 * 2;
    puVar32 = (ushort *)(iVar10 + iVar19);
    uVar1 = *puVar32;
    if (uVar31 < 0x1000000) {
      uVar31 = uVar31 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar31 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar20;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar19 = iVar19 + 1;
      uVar18 = uVar18 | 2;
      uVar20 = uVar31 - uVar20;
    }
    iVar19 = iVar19 * 2;
    puVar32 = (ushort *)(iVar10 + iVar19);
    uVar1 = *puVar32;
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar31 = (uVar20 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar31) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar31;
      *puVar32 = uVar1 - (uVar1 >> 5);
      iVar19 = iVar19 + 1;
      uVar18 = uVar18 | 4;
      uVar31 = uVar20 - uVar31;
    }
    puVar32 = (ushort *)(iVar10 + iVar19 * 2);
    uVar1 = *puVar32;
    if (uVar31 < 0x1000000) {
      uVar31 = uVar31 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    uVar20 = (uVar31 >> 0xb) * (uint)uVar1;
    if (uVar5 < uVar20) {
      *puVar32 = uVar1 + (short)(0x800 - uVar1 >> 5);
    }
    else {
      uVar5 = uVar5 - uVar20;
      *puVar32 = uVar1 - (uVar1 >> 5);
      uVar18 = uVar18 | 8;
      uVar20 = uVar31 - uVar20;
    }
    if (uVar18 != 0xffffffff) goto LAB_89eb1800;
    uVar15 = uVar15 + 0x112;
    uVar17 = uVar17 - 0xc;
    uVar31 = uVar30;
LAB_89eb1a84:
    if (uVar20 < 0x1000000) {
      uVar20 = uVar20 << 8;
      uVar5 = (uint)*pbVar6 | uVar5 << 8;
      pbVar6 = pbVar6 + 1;
    }
    param_1[8] = uVar5;
    param_1[6] = (uint)pbVar6;
    param_1[7] = uVar20;
    param_1[0x11] = local_54;
    param_1[0x12] = uVar15;
    param_1[9] = uVar16;
    param_1[0xb] = uVar21;
    param_1[0xe] = uVar23;
    param_1[0xf] = uVar31;
    param_1[0x10] = uVar28;
    param_1[0xd] = uVar17;
    if ((param_1[0xc] == 0) && (param_1[3] <= uVar21)) {
      param_1[0xc] = param_1[3];
    }
    FUN_89eb043c(param_1,param_2);
    if (((param_2 <= param_1[9]) || (param_3 <= (byte *)param_1[6])) || (0x111 < param_1[0x12])) {
      if (0x112 < param_1[0x12]) {
        param_1[0x12] = 0x112;
      }
      return 0;
    }
  } while( true );
}



/* ===== FUNCTION sub_89eb1b68 @ 89eb1b68 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb1b68(uint param_1,uint param_2,uint param_3,int param_4)

{
  ushort uVar1;
  bool bVar2;
  uint in_v0;
  byte *in_v1;
  char cVar3;
  uint uVar4;
  int iVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  uint in_t0;
  uint in_t1;
  uint uVar8;
  uint in_t2;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint in_t3;
  uint in_t4;
  uint in_t5;
  uint *in_t6;
  uint in_t7;
  uint uVar12;
  ushort *puVar13;
  uint unaff_s1;
  ushort *puVar14;
  undefined1 *puVar15;
  uint unaff_s3;
  uint unaff_s4;
  uint uVar16;
  uint in_t8;
  uint uVar17;
  uint uVar18;
  uint in_t9;
  uint in_stack_00000078;
  uint in_stack_0000007c;
  uint in_stack_00000080;
  int in_stack_00000084;
  uint in_stack_00000088;
  uint in_stack_0000008c;
  uint in_stack_00000090;
  ushort *in_stack_00000094;
  ushort *in_stack_00000098;
  uint in_stack_0000009c;
  int in_stack_000000a0;
  uint in_stack_000000d4;
  byte *in_stack_000000d8;
  
sub_89eb1b6c:
  in_v0 = in_v0 - in_t9;
  *(short *)(in_t5 + 0x646) = (short)in_t2 - (short)(in_t2 >> 5);
  param_1 = param_1 | 1;
  in_t9 = in_t3 - in_t9;
  iVar9 = 3;
LAB_89eb196c:
  iVar9 = iVar9 * 2;
  puVar14 = (ushort *)(in_stack_00000084 + iVar9);
  uVar1 = *puVar14;
  if (in_t9 < param_3) {
    in_t9 = in_t9 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar11 = (in_t9 >> 0xb) * (uint)uVar1;
  if (in_v0 < uVar11) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - uVar11;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = iVar9 + 1;
    param_1 = param_1 | 2;
    uVar11 = in_t9 - uVar11;
  }
  iVar9 = iVar9 * 2;
  puVar14 = (ushort *)(in_stack_00000084 + iVar9);
  uVar1 = *puVar14;
  if (uVar11 < param_3) {
    uVar11 = uVar11 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
  if (in_v0 < uVar18) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - uVar18;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = iVar9 + 1;
    param_1 = param_1 | 4;
    uVar18 = uVar11 - uVar18;
  }
  puVar14 = (ushort *)(in_stack_00000084 + iVar9 * 2);
  uVar1 = *puVar14;
  if (uVar18 < param_3) {
    uVar18 = uVar18 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
  if (in_v0 < in_t3) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - in_t3;
    *puVar14 = uVar1 - (uVar1 >> 5);
    param_1 = param_1 | 8;
    in_t3 = uVar18 - in_t3;
  }
  if (param_1 != 0xffffffff) goto LAB_89eb1800;
  param_2 = param_2 + 0x112;
  in_t1 = in_t1 - 0xc;
  uVar11 = unaff_s3;
  uVar18 = in_t8;
LAB_89eb1a84:
  if (in_t3 < 0x1000000) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  in_t6[8] = in_v0;
  in_t6[6] = (uint)in_v1;
  in_t6[7] = in_t3;
  in_t6[0x11] = in_stack_0000007c;
  in_t6[0x12] = param_2;
  in_t6[9] = in_t0;
  in_t6[0xb] = in_t4;
  in_t6[0xe] = in_t7;
  in_t6[0xf] = uVar18;
  in_t6[0x10] = uVar11;
  in_t6[0xd] = in_t1;
  if ((in_t6[0xc] == 0) && (in_t6[3] <= in_t4)) {
    in_t6[0xc] = in_t6[3];
  }
  FUN_89eb043c(in_t6,in_stack_000000d4);
  if (((in_stack_000000d4 <= in_t6[9]) || (in_stack_000000d8 <= (byte *)in_t6[6])) ||
     (0x111 < in_t6[0x12])) {
    if (0x112 < in_t6[0x12]) {
      in_t6[0x12] = 0x112;
    }
    return 0;
  }
  in_stack_00000078 = in_t6[0xc];
  in_t4 = in_t6[0xb];
  in_t0 = in_t6[9];
  in_stack_00000080 = in_stack_000000d4;
  if ((in_stack_00000078 == 0) && (in_t6[3] - in_t4 < in_stack_000000d4 - in_t0)) {
    in_stack_00000080 = (in_t6[3] - in_t4) + in_t0;
  }
  in_stack_0000007c = in_t6[0x11];
  in_stack_0000008c = ~(-1 << (in_t6[2] & 0x1f));
  in_t5 = in_t6[4];
  in_stack_00000088 = *in_t6;
  in_stack_00000090 = ~(-1 << (in_t6[1] & 0x1f));
  in_stack_00000084 = in_t5 + 0x644;
  in_stack_00000094 = (ushort *)(in_t5 + 0xa68);
  unaff_s1 = in_t6[10];
  in_stack_00000098 = (ushort *)(in_t5 + 0x664);
  in_stack_0000009c = 8 - in_stack_00000088;
  in_t1 = in_t6[0xd];
  in_t7 = in_t6[0xe];
  uVar18 = in_t6[0xf];
  uVar11 = in_t6[0x10];
  unaff_s4 = in_t6[5];
  in_v1 = (byte *)in_t6[6];
  in_t3 = in_t6[7];
  in_v0 = in_t6[8];
  in_stack_000000a0 = unaff_s1 - 1;
  param_2 = 0;
  param_3 = 0x1000000;
  param_4 = 0x800;
LAB_89eb0608:
  uVar8 = in_stack_0000008c & in_t4;
  puVar14 = (ushort *)(in_t5 + (uVar8 + in_t1 * 0x10) * 2);
  uVar1 = *puVar14;
  if (in_t3 < param_3) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar4 = (in_t3 >> 0xb) * (uint)uVar1;
  uVar10 = in_t3 - uVar4;
  if (in_v0 < uVar4) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
    iVar9 = in_t5 + 0xe6c;
    if (in_stack_00000078 != 0 || in_t4 != 0) {
      iVar5 = in_t0 - 1;
      if (in_t0 == 0) {
        iVar5 = in_stack_000000a0;
      }
      iVar9 = iVar9 + (((int)(uint)*(byte *)(unaff_s4 + iVar5) >> (in_stack_0000009c & 0x1f)) +
                      ((in_t4 & in_stack_00000090) << (in_stack_00000088 & 0x1f))) * 0x600;
    }
    in_t4 = in_t4 + 1;
    if (in_t1 < 7) {
      uVar8 = in_t1;
      if (3 < in_t1) {
        uVar8 = 3;
      }
      uVar1 = *(ushort *)(iVar9 + 2);
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *(ushort *)(iVar9 + 2) = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        iVar5 = 2;
      }
      else {
        in_v0 = in_v0 - uVar10;
        *(ushort *)(iVar9 + 2) = uVar1 - (uVar1 >> 5);
        uVar10 = uVar4 - uVar10;
        iVar5 = 3;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar10;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar10 = uVar4 - uVar10;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar10;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar10 = uVar4 - uVar10;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar4 - uVar17;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar4 = (uint)*puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar17 >> 0xb) * uVar4;
    }
    else {
      uVar8 = 0;
      if (in_t0 < in_t7) {
        uVar8 = unaff_s1;
      }
      uVar10 = (uint)*(byte *)(unaff_s4 + (in_t0 - in_t7) + uVar8);
      uVar8 = 6;
      if (in_t1 < 10) {
        uVar8 = 3;
      }
      uVar16 = uVar10 << 1 & 0x100;
      puVar14 = (ushort *)(iVar9 + (uVar16 + 0x101) * 2);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~(uVar10 << 1) & 0x100;
        iVar5 = 2;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        uVar17 = uVar4 - uVar17;
        iVar5 = 3;
      }
      uVar4 = uVar10 << 2 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 3 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      uVar4 = uVar10 << 4 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 5 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      uVar4 = uVar10 << 6 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 7 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      puVar14 = (ushort *)(iVar9 + ((uVar10 << 8 & uVar16) + uVar16 + iVar5) * 2);
      uVar4 = (uint)*puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar17 >> 0xb) * uVar4;
      iVar5 = iVar5 << 1;
    }
    in_t1 = in_t1 - uVar8;
    cVar3 = (char)iVar5;
    if (in_v0 < in_t3) {
      *puVar14 = (short)uVar4 + (short)(param_4 - uVar4 >> 5);
    }
    else {
      in_v0 = in_v0 - in_t3;
      *puVar14 = (short)uVar4 - (short)(uVar4 >> 5);
      cVar3 = cVar3 + '\x01';
      in_t3 = uVar17 - in_t3;
    }
    *(char *)(unaff_s4 + in_t0) = cVar3;
    in_t0 = in_t0 + 1;
  }
  else {
    in_v0 = in_v0 - uVar4;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = (in_t1 + 0xc0) * 2;
    puVar14 = (ushort *)(in_t5 + iVar9);
    uVar1 = *puVar14;
    if (uVar10 < param_3) {
      uVar10 = uVar10 * 0x100;
      in_v0 = (uint)*in_v1 | in_v0 * 0x100;
      in_v1 = in_v1 + 1;
    }
    uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
    uVar10 = uVar10 - uVar4;
    in_t8 = uVar18;
    unaff_s3 = uVar11;
    if (in_v0 < uVar4) {
      *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      in_t1 = in_t1 + 0xc;
      puVar14 = in_stack_00000098;
    }
    else {
      in_v0 = in_v0 - uVar4;
      *puVar14 = uVar1 - (uVar1 >> 5);
      if (in_stack_00000078 == 0 && in_t4 == 0) {
        return 1;
      }
      puVar14 = (ushort *)(in_t5 + iVar9 + 0x18);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 * 0x100;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      uVar10 = uVar10 - uVar4;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        puVar14 = (ushort *)(in_t5 + (in_t1 * 0x10 + 0xf0 + uVar8) * 2);
        uVar1 = *puVar14;
        if (uVar4 < param_3) {
          uVar4 = uVar4 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar4 >> 0xb) * (uint)uVar1;
        uVar4 = uVar4 - in_t3;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          uVar8 = 0;
          if (in_t0 < in_t7) {
            uVar8 = unaff_s1;
          }
          in_t4 = in_t4 + 1;
          *(undefined1 *)(unaff_s4 + in_t0) = *(undefined1 *)(unaff_s4 + (in_t0 - in_t7) + uVar8);
          bVar2 = in_t1 < 7;
          in_t0 = in_t0 + 1;
          in_t1 = 0xb;
          if (bVar2) {
            in_t1 = 9;
          }
          goto LAB_89eb0964;
        }
        in_v0 = in_v0 - in_t3;
        *puVar14 = uVar1 - (uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        puVar14 = (ushort *)(in_t5 + iVar9 + 0x30);
        uVar1 = *puVar14;
        if (uVar10 < param_3) {
          uVar10 = uVar10 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 * 0x100;
          in_v1 = in_v1 + 1;
        }
        uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
        uVar10 = uVar10 - uVar4;
        in_t8 = in_t7;
        if (in_v0 < uVar4) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          in_t7 = uVar18;
        }
        else {
          in_v0 = in_v0 - uVar4;
          *puVar14 = uVar1 - (uVar1 >> 5);
          puVar14 = (ushort *)(in_t5 + iVar9 + 0x48);
          uVar1 = *puVar14;
          if (uVar10 < param_3) {
            uVar10 = uVar10 * 0x100;
            in_v0 = (uint)*in_v1 | in_v0 * 0x100;
            in_v1 = in_v1 + 1;
          }
          uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
          unaff_s3 = uVar18;
          if (in_v0 < uVar4) {
            *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
            in_t7 = uVar11;
          }
          else {
            in_v0 = in_v0 - uVar4;
            *puVar14 = uVar1 - (uVar1 >> 5);
            uVar4 = uVar10 - uVar4;
            in_t7 = in_stack_0000007c;
            in_stack_0000007c = uVar11;
          }
        }
      }
      bVar2 = in_t1 < 7;
      in_t1 = 0xb;
      puVar14 = in_stack_00000094;
      if (bVar2) {
        in_t1 = 8;
      }
    }
    uVar1 = *puVar14;
    if (uVar4 < param_3) {
      uVar4 = uVar4 << 8;
      in_v0 = (uint)*in_v1 | in_v0 << 8;
      in_v1 = in_v1 + 1;
    }
    uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
    uVar4 = uVar4 - uVar18;
    uVar10 = in_t7;
    uVar11 = unaff_s3;
    if (in_v0 < uVar18) {
      *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      uVar1 = puVar14[uVar8 * 8 + 3];
      if (uVar18 < param_3) {
        uVar18 = uVar18 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar18 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        puVar14[uVar8 * 8 + 3] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        iVar9 = 2;
      }
      else {
        in_v0 = in_v0 - uVar4;
        puVar14[uVar8 * 8 + 3] = uVar1 - (uVar1 >> 5);
        uVar4 = uVar18 - uVar4;
        iVar9 = 3;
      }
      iVar5 = iVar9 * 2;
      puVar13 = puVar14 + uVar8 * 8 + iVar9 + 2;
      uVar1 = *puVar13;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar18) {
        *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar18;
        *puVar13 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar18 = uVar4 - uVar18;
      }
      iVar9 = iVar5 * 2;
      puVar14 = puVar14 + uVar8 * 8 + iVar5 + 2;
      uVar1 = *puVar14;
      if (uVar18 < param_3) {
        uVar18 = uVar18 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
      if (in_v0 < in_t3) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - in_t3;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar9 = iVar9 + 1;
        in_t3 = uVar18 - in_t3;
      }
      param_2 = iVar9 - 8;
      if (0xb < in_t1) {
        iVar9 = 0x4e0;
        if (param_2 < 4) {
          iVar9 = param_2 * 0x80 + 0x360;
        }
        iVar9 = in_t5 + iVar9;
LAB_89eb1578:
        uVar1 = *(ushort *)(iVar9 + 2);
        if (in_t3 < param_3) {
          in_t3 = in_t3 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (in_t3 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *(ushort *)(iVar9 + 2) = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          iVar5 = 2;
        }
        else {
          in_v0 = in_v0 - uVar11;
          *(ushort *)(iVar9 + 2) = uVar1 - (uVar1 >> 5);
          uVar11 = in_t3 - uVar11;
          iVar5 = 3;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar11 - uVar18;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar11;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar11 = uVar18 - uVar11;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar11 - uVar18;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar11;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar11 = uVar18 - uVar11;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - in_t3;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          in_t3 = uVar11 - in_t3;
        }
        uVar11 = iVar5 - 0x40;
        param_1 = uVar11;
        if (3 < uVar11) {
          param_1 = uVar11 & 1 | 2;
          if (0xd < uVar11) goto LAB_89eb18d0;
          uVar18 = (uVar11 >> 1) - 1;
          param_1 = param_1 << (uVar18 & 0x1f);
          iVar9 = (0x2af - uVar11) + param_1;
          iVar5 = 1;
          uVar11 = 1;
          do {
            iVar5 = iVar5 * 2;
            puVar14 = (ushort *)(in_t5 + iVar9 * 2 + iVar5);
            uVar1 = *puVar14;
            if (in_t3 < param_3) {
              in_t3 = in_t3 << 8;
              in_v0 = (uint)*in_v1 | in_v0 << 8;
              in_v1 = in_v1 + 1;
            }
            uVar8 = (in_t3 >> 0xb) * (uint)uVar1;
            in_t3 = in_t3 - uVar8;
            if (in_v0 < uVar8) {
              *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
              in_t3 = uVar8;
            }
            else {
              in_v0 = in_v0 - uVar8;
              *puVar14 = uVar1 - (uVar1 >> 5);
              iVar5 = iVar5 + 1;
              param_1 = param_1 | uVar11;
            }
            uVar18 = uVar18 - 1;
            uVar11 = uVar11 << 1;
          } while (uVar18 != 0);
        }
LAB_89eb1800:
        uVar11 = in_stack_00000078;
        if (in_stack_00000078 == 0) {
          uVar11 = in_t4;
        }
        if (uVar11 <= param_1) goto LAB_89eb1814;
        uVar10 = param_1 + 1;
        uVar11 = in_t8;
        in_stack_0000007c = unaff_s3;
        if (in_t1 < 0x13) {
          in_t1 = 7;
          in_t8 = in_t7;
        }
        else {
          in_t1 = 10;
          in_t8 = in_t7;
        }
      }
    }
    else {
      *puVar14 = uVar1 - (uVar1 >> 5);
      in_v0 = in_v0 - uVar18;
      uVar1 = puVar14[1];
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 * 0x100;
        in_v1 = in_v1 + 1;
      }
      uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar18) {
        puVar14[1] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar1 = puVar14[uVar8 * 8 + 0x83];
        if (uVar18 < param_3) {
          uVar18 = uVar18 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar4 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar4) {
          puVar14[uVar8 * 8 + 0x83] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          iVar9 = 2;
        }
        else {
          in_v0 = in_v0 - uVar4;
          puVar14[uVar8 * 8 + 0x83] = uVar1 - (uVar1 >> 5);
          uVar4 = uVar18 - uVar4;
          iVar9 = 3;
        }
        iVar5 = iVar9 * 2;
        puVar13 = puVar14 + uVar8 * 8 + iVar9 + 0x82;
        uVar1 = *puVar13;
        if (uVar4 < param_3) {
          uVar4 = uVar4 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar13 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar4 - uVar18;
        }
        param_2 = iVar5 * 2;
        puVar14 = puVar14 + uVar8 * 8 + iVar5 + 0x82;
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - in_t3;
          *puVar14 = uVar1 - (uVar1 >> 5);
          param_2 = param_2 + 1;
          in_t3 = uVar18 - in_t3;
        }
      }
      else {
        in_t3 = uVar4 - uVar18;
        in_v0 = in_v0 - uVar18;
        puVar14[1] = uVar1 - (uVar1 >> 5);
        uVar18 = 1;
        do {
          param_2 = uVar18 * 2;
          puVar13 = puVar14 + uVar18 + 0x102;
          uVar1 = *puVar13;
          if (in_t3 < param_3) {
            in_t3 = in_t3 << 8;
            in_v0 = (uint)*in_v1 | in_v0 << 8;
            in_v1 = in_v1 + 1;
          }
          uVar18 = (in_t3 >> 0xb) * (uint)uVar1;
          in_t3 = in_t3 - uVar18;
          if (in_v0 < uVar18) {
            *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
            in_t3 = uVar18;
          }
          else {
            in_v0 = in_v0 - uVar18;
            *puVar13 = uVar1 - (uVar1 >> 5);
            param_2 = param_2 + 1;
          }
          uVar18 = param_2;
        } while (param_2 < 0x100);
        param_2 = param_2 - 0xf0;
      }
      if (0xb < in_t1) {
        iVar9 = in_t5 + 0x4e0;
        goto LAB_89eb1578;
      }
    }
    uVar18 = in_stack_00000080 - in_t0;
    param_2 = param_2 + 2;
    if (uVar18 == 0) {
LAB_89eb1814:
      in_t6[9] = in_t0;
      return 1;
    }
    uVar8 = param_2;
    if (uVar18 < param_2) {
      uVar8 = uVar18;
    }
    uVar4 = 0;
    if (in_t0 < uVar10) {
      uVar4 = unaff_s1;
    }
    uVar4 = (in_t0 - uVar10) + uVar4;
    in_t4 = in_t4 + uVar8;
    param_2 = param_2 - uVar8;
    puVar6 = (undefined1 *)(unaff_s4 + in_t0);
    uVar16 = uVar8;
    in_t7 = uVar10;
    uVar18 = in_t8;
    if (unaff_s1 - uVar4 < uVar8) {
      do {
        puVar15 = (undefined1 *)(unaff_s4 + uVar4);
        uVar4 = uVar4 + 1;
        *puVar6 = *puVar15;
        if (unaff_s1 == uVar4) {
          uVar4 = 0;
        }
        uVar16 = uVar16 - 1;
        puVar6 = puVar6 + 1;
      } while (uVar16 != 0);
      in_t0 = in_t0 + uVar8;
    }
    else {
      iVar9 = uVar4 - in_t0;
      in_t0 = in_t0 + uVar8;
      puVar15 = puVar6;
      do {
        puVar7 = puVar15 + 1;
        *puVar15 = puVar15[iVar9];
        puVar15 = puVar7;
      } while (puVar6 + uVar8 != puVar7);
    }
  }
LAB_89eb0964:
  if ((in_stack_00000080 <= in_t0) || (in_stack_000000d8 <= in_v1)) goto LAB_89eb1a84;
  goto LAB_89eb0608;
LAB_89eb18d0:
  iVar9 = (uVar11 >> 1) - 5;
  do {
    if (in_t3 < param_3) {
      in_t3 = in_t3 << 8;
      in_v0 = (uint)*in_v1 | in_v0 << 8;
      in_v1 = in_v1 + 1;
    }
    in_t3 = in_t3 >> 1;
    uVar11 = (int)(in_v0 - in_t3) >> 0x1f;
    param_1 = uVar11 + param_1 * 2 + 1;
    iVar9 = iVar9 + -1;
    in_v0 = (in_t3 & uVar11) + (in_v0 - in_t3);
  } while (iVar9 != 0);
  param_1 = param_1 * 0x10;
  in_t2 = (uint)*(ushort *)(in_t5 + 0x646);
  if (in_t3 < param_3) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 * 0x100;
    in_v1 = in_v1 + 1;
  }
  in_t9 = (in_t3 >> 0xb) * in_t2;
  if (in_t9 <= in_v0) goto sub_89eb1b6c;
  *(ushort *)(in_t5 + 0x646) = *(ushort *)(in_t5 + 0x646) + (short)(param_4 - in_t2 >> 5);
  iVar9 = 2;
  goto LAB_89eb196c;
}



/* ===== FUNCTION sub_89eb1b6c @ 89eb1b6c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4
sub_89eb1b6c(uint param_1,uint param_2,uint param_3,int param_4,uint param_5,uint param_6,
            uint param_7,int param_8,uint param_9,uint param_10,uint param_11,ushort *param_12,
            ushort *param_13,uint param_14,int param_15)

{
  ushort uVar1;
  bool bVar2;
  uint in_v0;
  byte *in_v1;
  char cVar3;
  uint uVar4;
  int iVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  uint in_t0;
  uint in_t1;
  uint uVar8;
  uint in_t2;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint in_t3;
  uint in_t4;
  uint in_t5;
  uint *in_t6;
  uint in_t7;
  uint uVar12;
  ushort *puVar13;
  uint unaff_s1;
  ushort *puVar14;
  undefined1 *puVar15;
  uint unaff_s3;
  uint unaff_s4;
  uint uVar16;
  uint in_t8;
  uint uVar17;
  uint uVar18;
  uint in_t9;
  uint in_stack_0000006c;
  byte *in_stack_00000070;
  
code_r0x89eb1b6c:
  in_v0 = in_v0 - in_t9;
  *(short *)(in_t5 + 0x646) = (short)in_t2 - (short)(in_t2 >> 5);
  param_1 = param_1 | 1;
  in_t9 = in_t3 - in_t9;
  iVar9 = 3;
LAB_89eb196c:
  iVar9 = iVar9 * 2;
  puVar14 = (ushort *)(param_8 + iVar9);
  uVar1 = *puVar14;
  if (in_t9 < param_3) {
    in_t9 = in_t9 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar11 = (in_t9 >> 0xb) * (uint)uVar1;
  if (in_v0 < uVar11) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - uVar11;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = iVar9 + 1;
    param_1 = param_1 | 2;
    uVar11 = in_t9 - uVar11;
  }
  iVar9 = iVar9 * 2;
  puVar14 = (ushort *)(param_8 + iVar9);
  uVar1 = *puVar14;
  if (uVar11 < param_3) {
    uVar11 = uVar11 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
  if (in_v0 < uVar18) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - uVar18;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = iVar9 + 1;
    param_1 = param_1 | 4;
    uVar18 = uVar11 - uVar18;
  }
  puVar14 = (ushort *)(param_8 + iVar9 * 2);
  uVar1 = *puVar14;
  if (uVar18 < param_3) {
    uVar18 = uVar18 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
  if (in_v0 < in_t3) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
  }
  else {
    in_v0 = in_v0 - in_t3;
    *puVar14 = uVar1 - (uVar1 >> 5);
    param_1 = param_1 | 8;
    in_t3 = uVar18 - in_t3;
  }
  if (param_1 != 0xffffffff) goto LAB_89eb1800;
  param_2 = param_2 + 0x112;
  in_t1 = in_t1 - 0xc;
  uVar11 = unaff_s3;
  uVar18 = in_t8;
LAB_89eb1a84:
  if (in_t3 < 0x1000000) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  in_t6[8] = in_v0;
  in_t6[6] = (uint)in_v1;
  in_t6[7] = in_t3;
  in_t6[0x11] = param_6;
  in_t6[0x12] = param_2;
  in_t6[9] = in_t0;
  in_t6[0xb] = in_t4;
  in_t6[0xe] = in_t7;
  in_t6[0xf] = uVar18;
  in_t6[0x10] = uVar11;
  in_t6[0xd] = in_t1;
  if ((in_t6[0xc] == 0) && (in_t6[3] <= in_t4)) {
    in_t6[0xc] = in_t6[3];
  }
  FUN_89eb043c(in_t6,in_stack_0000006c);
  if (((in_stack_0000006c <= in_t6[9]) || (in_stack_00000070 <= (byte *)in_t6[6])) ||
     (0x111 < in_t6[0x12])) {
    if (0x112 < in_t6[0x12]) {
      in_t6[0x12] = 0x112;
    }
    return 0;
  }
  param_5 = in_t6[0xc];
  in_t4 = in_t6[0xb];
  in_t0 = in_t6[9];
  param_7 = in_stack_0000006c;
  if ((param_5 == 0) && (in_t6[3] - in_t4 < in_stack_0000006c - in_t0)) {
    param_7 = (in_t6[3] - in_t4) + in_t0;
  }
  param_6 = in_t6[0x11];
  param_10 = ~(-1 << (in_t6[2] & 0x1f));
  in_t5 = in_t6[4];
  param_9 = *in_t6;
  param_11 = ~(-1 << (in_t6[1] & 0x1f));
  param_8 = in_t5 + 0x644;
  param_12 = (ushort *)(in_t5 + 0xa68);
  unaff_s1 = in_t6[10];
  param_13 = (ushort *)(in_t5 + 0x664);
  param_14 = 8 - param_9;
  in_t1 = in_t6[0xd];
  in_t7 = in_t6[0xe];
  uVar18 = in_t6[0xf];
  uVar11 = in_t6[0x10];
  unaff_s4 = in_t6[5];
  in_v1 = (byte *)in_t6[6];
  in_t3 = in_t6[7];
  in_v0 = in_t6[8];
  param_15 = unaff_s1 - 1;
  param_2 = 0;
  param_3 = 0x1000000;
  param_4 = 0x800;
LAB_89eb0608:
  uVar8 = param_10 & in_t4;
  puVar14 = (ushort *)(in_t5 + (uVar8 + in_t1 * 0x10) * 2);
  uVar1 = *puVar14;
  if (in_t3 < param_3) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 << 8;
    in_v1 = in_v1 + 1;
  }
  uVar4 = (in_t3 >> 0xb) * (uint)uVar1;
  uVar10 = in_t3 - uVar4;
  if (in_v0 < uVar4) {
    *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
    iVar9 = in_t5 + 0xe6c;
    if (param_5 != 0 || in_t4 != 0) {
      iVar5 = in_t0 - 1;
      if (in_t0 == 0) {
        iVar5 = param_15;
      }
      iVar9 = iVar9 + (((int)(uint)*(byte *)(unaff_s4 + iVar5) >> (param_14 & 0x1f)) +
                      ((in_t4 & param_11) << (param_9 & 0x1f))) * 0x600;
    }
    in_t4 = in_t4 + 1;
    if (in_t1 < 7) {
      uVar8 = in_t1;
      if (3 < in_t1) {
        uVar8 = 3;
      }
      uVar1 = *(ushort *)(iVar9 + 2);
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *(ushort *)(iVar9 + 2) = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        iVar5 = 2;
      }
      else {
        in_v0 = in_v0 - uVar10;
        *(ushort *)(iVar9 + 2) = uVar1 - (uVar1 >> 5);
        uVar10 = uVar4 - uVar10;
        iVar5 = 3;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar10;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar10 = uVar4 - uVar10;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar10 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar10) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar10;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar10 = uVar4 - uVar10;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar4 = uVar10 - uVar4;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar4 - uVar17;
      }
      iVar5 = iVar5 * 2;
      puVar14 = (ushort *)(iVar9 + iVar5);
      uVar4 = (uint)*puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar17 >> 0xb) * uVar4;
    }
    else {
      uVar8 = 0;
      if (in_t0 < in_t7) {
        uVar8 = unaff_s1;
      }
      uVar10 = (uint)*(byte *)(unaff_s4 + (in_t0 - in_t7) + uVar8);
      uVar8 = 6;
      if (in_t1 < 10) {
        uVar8 = 3;
      }
      uVar16 = uVar10 << 1 & 0x100;
      puVar14 = (ushort *)(iVar9 + (uVar16 + 0x101) * 2);
      uVar1 = *puVar14;
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~(uVar10 << 1) & 0x100;
        iVar5 = 2;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        uVar17 = uVar4 - uVar17;
        iVar5 = 3;
      }
      uVar4 = uVar10 << 2 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 3 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      uVar4 = uVar10 << 4 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 5 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      uVar4 = uVar10 << 6 & uVar16;
      puVar14 = (ushort *)(iVar9 + (uVar16 + iVar5 + uVar4) * 2);
      uVar1 = *puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar12 = (uVar17 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar12) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar4 = ~uVar4 & uVar16;
      }
      else {
        in_v0 = in_v0 - uVar12;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar12 = uVar17 - uVar12;
      }
      uVar16 = uVar10 << 7 & uVar4;
      puVar14 = (ushort *)(iVar9 + (uVar4 + iVar5 + uVar16) * 2);
      uVar1 = *puVar14;
      if (uVar12 < param_3) {
        uVar12 = uVar12 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar17 = (uVar12 >> 0xb) * (uint)uVar1;
      iVar5 = iVar5 * 2;
      if (in_v0 < uVar17) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar16 = ~uVar16 & uVar4;
      }
      else {
        in_v0 = in_v0 - uVar17;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar17 = uVar12 - uVar17;
      }
      puVar14 = (ushort *)(iVar9 + ((uVar10 << 8 & uVar16) + uVar16 + iVar5) * 2);
      uVar4 = (uint)*puVar14;
      if (uVar17 < param_3) {
        uVar17 = uVar17 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar17 >> 0xb) * uVar4;
      iVar5 = iVar5 << 1;
    }
    in_t1 = in_t1 - uVar8;
    cVar3 = (char)iVar5;
    if (in_v0 < in_t3) {
      *puVar14 = (short)uVar4 + (short)(param_4 - uVar4 >> 5);
    }
    else {
      in_v0 = in_v0 - in_t3;
      *puVar14 = (short)uVar4 - (short)(uVar4 >> 5);
      cVar3 = cVar3 + '\x01';
      in_t3 = uVar17 - in_t3;
    }
    *(char *)(unaff_s4 + in_t0) = cVar3;
    in_t0 = in_t0 + 1;
  }
  else {
    in_v0 = in_v0 - uVar4;
    *puVar14 = uVar1 - (uVar1 >> 5);
    iVar9 = (in_t1 + 0xc0) * 2;
    puVar14 = (ushort *)(in_t5 + iVar9);
    uVar1 = *puVar14;
    if (uVar10 < param_3) {
      uVar10 = uVar10 * 0x100;
      in_v0 = (uint)*in_v1 | in_v0 * 0x100;
      in_v1 = in_v1 + 1;
    }
    uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
    uVar10 = uVar10 - uVar4;
    in_t8 = uVar18;
    unaff_s3 = uVar11;
    if (in_v0 < uVar4) {
      *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      in_t1 = in_t1 + 0xc;
      puVar14 = param_13;
    }
    else {
      in_v0 = in_v0 - uVar4;
      *puVar14 = uVar1 - (uVar1 >> 5);
      if (param_5 == 0 && in_t4 == 0) {
        return 1;
      }
      puVar14 = (ushort *)(in_t5 + iVar9 + 0x18);
      uVar1 = *puVar14;
      if (uVar10 < param_3) {
        uVar10 = uVar10 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 * 0x100;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
      uVar10 = uVar10 - uVar4;
      if (in_v0 < uVar4) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        puVar14 = (ushort *)(in_t5 + (in_t1 * 0x10 + 0xf0 + uVar8) * 2);
        uVar1 = *puVar14;
        if (uVar4 < param_3) {
          uVar4 = uVar4 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar4 >> 0xb) * (uint)uVar1;
        uVar4 = uVar4 - in_t3;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          uVar8 = 0;
          if (in_t0 < in_t7) {
            uVar8 = unaff_s1;
          }
          in_t4 = in_t4 + 1;
          *(undefined1 *)(unaff_s4 + in_t0) = *(undefined1 *)(unaff_s4 + (in_t0 - in_t7) + uVar8);
          bVar2 = in_t1 < 7;
          in_t0 = in_t0 + 1;
          in_t1 = 0xb;
          if (bVar2) {
            in_t1 = 9;
          }
          goto LAB_89eb0964;
        }
        in_v0 = in_v0 - in_t3;
        *puVar14 = uVar1 - (uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar4;
        *puVar14 = uVar1 - (uVar1 >> 5);
        puVar14 = (ushort *)(in_t5 + iVar9 + 0x30);
        uVar1 = *puVar14;
        if (uVar10 < param_3) {
          uVar10 = uVar10 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 * 0x100;
          in_v1 = in_v1 + 1;
        }
        uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
        uVar10 = uVar10 - uVar4;
        in_t8 = in_t7;
        if (in_v0 < uVar4) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          in_t7 = uVar18;
        }
        else {
          in_v0 = in_v0 - uVar4;
          *puVar14 = uVar1 - (uVar1 >> 5);
          puVar14 = (ushort *)(in_t5 + iVar9 + 0x48);
          uVar1 = *puVar14;
          if (uVar10 < param_3) {
            uVar10 = uVar10 * 0x100;
            in_v0 = (uint)*in_v1 | in_v0 * 0x100;
            in_v1 = in_v1 + 1;
          }
          uVar4 = (uVar10 >> 0xb) * (uint)uVar1;
          unaff_s3 = uVar18;
          if (in_v0 < uVar4) {
            *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
            in_t7 = uVar11;
          }
          else {
            in_v0 = in_v0 - uVar4;
            *puVar14 = uVar1 - (uVar1 >> 5);
            uVar4 = uVar10 - uVar4;
            in_t7 = param_6;
            param_6 = uVar11;
          }
        }
      }
      bVar2 = in_t1 < 7;
      in_t1 = 0xb;
      puVar14 = param_12;
      if (bVar2) {
        in_t1 = 8;
      }
    }
    uVar1 = *puVar14;
    if (uVar4 < param_3) {
      uVar4 = uVar4 << 8;
      in_v0 = (uint)*in_v1 | in_v0 << 8;
      in_v1 = in_v1 + 1;
    }
    uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
    uVar4 = uVar4 - uVar18;
    uVar10 = in_t7;
    uVar11 = unaff_s3;
    if (in_v0 < uVar18) {
      *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      uVar1 = puVar14[uVar8 * 8 + 3];
      if (uVar18 < param_3) {
        uVar18 = uVar18 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar4 = (uVar18 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar4) {
        puVar14[uVar8 * 8 + 3] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        iVar9 = 2;
      }
      else {
        in_v0 = in_v0 - uVar4;
        puVar14[uVar8 * 8 + 3] = uVar1 - (uVar1 >> 5);
        uVar4 = uVar18 - uVar4;
        iVar9 = 3;
      }
      iVar5 = iVar9 * 2;
      puVar13 = puVar14 + uVar8 * 8 + iVar9 + 2;
      uVar1 = *puVar13;
      if (uVar4 < param_3) {
        uVar4 = uVar4 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar18) {
        *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - uVar18;
        *puVar13 = uVar1 - (uVar1 >> 5);
        iVar5 = iVar5 + 1;
        uVar18 = uVar4 - uVar18;
      }
      iVar9 = iVar5 * 2;
      puVar14 = puVar14 + uVar8 * 8 + iVar5 + 2;
      uVar1 = *puVar14;
      if (uVar18 < param_3) {
        uVar18 = uVar18 << 8;
        in_v0 = (uint)*in_v1 | in_v0 << 8;
        in_v1 = in_v1 + 1;
      }
      in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
      if (in_v0 < in_t3) {
        *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
      }
      else {
        in_v0 = in_v0 - in_t3;
        *puVar14 = uVar1 - (uVar1 >> 5);
        iVar9 = iVar9 + 1;
        in_t3 = uVar18 - in_t3;
      }
      param_2 = iVar9 - 8;
      if (0xb < in_t1) {
        iVar9 = 0x4e0;
        if (param_2 < 4) {
          iVar9 = param_2 * 0x80 + 0x360;
        }
        iVar9 = in_t5 + iVar9;
LAB_89eb1578:
        uVar1 = *(ushort *)(iVar9 + 2);
        if (in_t3 < param_3) {
          in_t3 = in_t3 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (in_t3 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *(ushort *)(iVar9 + 2) = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          iVar5 = 2;
        }
        else {
          in_v0 = in_v0 - uVar11;
          *(ushort *)(iVar9 + 2) = uVar1 - (uVar1 >> 5);
          uVar11 = in_t3 - uVar11;
          iVar5 = 3;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar11 - uVar18;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar11;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar11 = uVar18 - uVar11;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar11 - uVar18;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar11 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar11) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar11;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar11 = uVar18 - uVar11;
        }
        iVar5 = iVar5 * 2;
        puVar14 = (ushort *)(iVar9 + iVar5);
        uVar1 = *puVar14;
        if (uVar11 < param_3) {
          uVar11 = uVar11 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar11 >> 0xb) * (uint)uVar1;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - in_t3;
          *puVar14 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          in_t3 = uVar11 - in_t3;
        }
        uVar11 = iVar5 - 0x40;
        param_1 = uVar11;
        if (3 < uVar11) {
          param_1 = uVar11 & 1 | 2;
          if (0xd < uVar11) goto LAB_89eb18d0;
          uVar18 = (uVar11 >> 1) - 1;
          param_1 = param_1 << (uVar18 & 0x1f);
          iVar9 = (0x2af - uVar11) + param_1;
          iVar5 = 1;
          uVar11 = 1;
          do {
            iVar5 = iVar5 * 2;
            puVar14 = (ushort *)(in_t5 + iVar9 * 2 + iVar5);
            uVar1 = *puVar14;
            if (in_t3 < param_3) {
              in_t3 = in_t3 << 8;
              in_v0 = (uint)*in_v1 | in_v0 << 8;
              in_v1 = in_v1 + 1;
            }
            uVar8 = (in_t3 >> 0xb) * (uint)uVar1;
            in_t3 = in_t3 - uVar8;
            if (in_v0 < uVar8) {
              *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
              in_t3 = uVar8;
            }
            else {
              in_v0 = in_v0 - uVar8;
              *puVar14 = uVar1 - (uVar1 >> 5);
              iVar5 = iVar5 + 1;
              param_1 = param_1 | uVar11;
            }
            uVar18 = uVar18 - 1;
            uVar11 = uVar11 << 1;
          } while (uVar18 != 0);
        }
LAB_89eb1800:
        uVar11 = param_5;
        if (param_5 == 0) {
          uVar11 = in_t4;
        }
        if (uVar11 <= param_1) goto LAB_89eb1814;
        uVar10 = param_1 + 1;
        uVar11 = in_t8;
        param_6 = unaff_s3;
        if (in_t1 < 0x13) {
          in_t1 = 7;
          in_t8 = in_t7;
        }
        else {
          in_t1 = 10;
          in_t8 = in_t7;
        }
      }
    }
    else {
      *puVar14 = uVar1 - (uVar1 >> 5);
      in_v0 = in_v0 - uVar18;
      uVar1 = puVar14[1];
      if (uVar4 < param_3) {
        uVar4 = uVar4 * 0x100;
        in_v0 = (uint)*in_v1 | in_v0 * 0x100;
        in_v1 = in_v1 + 1;
      }
      uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
      if (in_v0 < uVar18) {
        puVar14[1] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        uVar1 = puVar14[uVar8 * 8 + 0x83];
        if (uVar18 < param_3) {
          uVar18 = uVar18 * 0x100;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar4 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar4) {
          puVar14[uVar8 * 8 + 0x83] = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
          iVar9 = 2;
        }
        else {
          in_v0 = in_v0 - uVar4;
          puVar14[uVar8 * 8 + 0x83] = uVar1 - (uVar1 >> 5);
          uVar4 = uVar18 - uVar4;
          iVar9 = 3;
        }
        iVar5 = iVar9 * 2;
        puVar13 = puVar14 + uVar8 * 8 + iVar9 + 0x82;
        uVar1 = *puVar13;
        if (uVar4 < param_3) {
          uVar4 = uVar4 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        uVar18 = (uVar4 >> 0xb) * (uint)uVar1;
        if (in_v0 < uVar18) {
          *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - uVar18;
          *puVar13 = uVar1 - (uVar1 >> 5);
          iVar5 = iVar5 + 1;
          uVar18 = uVar4 - uVar18;
        }
        param_2 = iVar5 * 2;
        puVar14 = puVar14 + uVar8 * 8 + iVar5 + 0x82;
        uVar1 = *puVar14;
        if (uVar18 < param_3) {
          uVar18 = uVar18 << 8;
          in_v0 = (uint)*in_v1 | in_v0 << 8;
          in_v1 = in_v1 + 1;
        }
        in_t3 = (uVar18 >> 0xb) * (uint)uVar1;
        if (in_v0 < in_t3) {
          *puVar14 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
        }
        else {
          in_v0 = in_v0 - in_t3;
          *puVar14 = uVar1 - (uVar1 >> 5);
          param_2 = param_2 + 1;
          in_t3 = uVar18 - in_t3;
        }
      }
      else {
        in_t3 = uVar4 - uVar18;
        in_v0 = in_v0 - uVar18;
        puVar14[1] = uVar1 - (uVar1 >> 5);
        uVar18 = 1;
        do {
          param_2 = uVar18 * 2;
          puVar13 = puVar14 + uVar18 + 0x102;
          uVar1 = *puVar13;
          if (in_t3 < param_3) {
            in_t3 = in_t3 << 8;
            in_v0 = (uint)*in_v1 | in_v0 << 8;
            in_v1 = in_v1 + 1;
          }
          uVar18 = (in_t3 >> 0xb) * (uint)uVar1;
          in_t3 = in_t3 - uVar18;
          if (in_v0 < uVar18) {
            *puVar13 = uVar1 + (short)(param_4 - (uint)uVar1 >> 5);
            in_t3 = uVar18;
          }
          else {
            in_v0 = in_v0 - uVar18;
            *puVar13 = uVar1 - (uVar1 >> 5);
            param_2 = param_2 + 1;
          }
          uVar18 = param_2;
        } while (param_2 < 0x100);
        param_2 = param_2 - 0xf0;
      }
      if (0xb < in_t1) {
        iVar9 = in_t5 + 0x4e0;
        goto LAB_89eb1578;
      }
    }
    uVar18 = param_7 - in_t0;
    param_2 = param_2 + 2;
    if (uVar18 == 0) {
LAB_89eb1814:
      in_t6[9] = in_t0;
      return 1;
    }
    uVar8 = param_2;
    if (uVar18 < param_2) {
      uVar8 = uVar18;
    }
    uVar4 = 0;
    if (in_t0 < uVar10) {
      uVar4 = unaff_s1;
    }
    uVar4 = (in_t0 - uVar10) + uVar4;
    in_t4 = in_t4 + uVar8;
    param_2 = param_2 - uVar8;
    puVar6 = (undefined1 *)(unaff_s4 + in_t0);
    uVar16 = uVar8;
    in_t7 = uVar10;
    uVar18 = in_t8;
    if (unaff_s1 - uVar4 < uVar8) {
      do {
        puVar15 = (undefined1 *)(unaff_s4 + uVar4);
        uVar4 = uVar4 + 1;
        *puVar6 = *puVar15;
        if (unaff_s1 == uVar4) {
          uVar4 = 0;
        }
        uVar16 = uVar16 - 1;
        puVar6 = puVar6 + 1;
      } while (uVar16 != 0);
      in_t0 = in_t0 + uVar8;
    }
    else {
      iVar9 = uVar4 - in_t0;
      in_t0 = in_t0 + uVar8;
      puVar15 = puVar6;
      do {
        puVar7 = puVar15 + 1;
        *puVar15 = puVar15[iVar9];
        puVar15 = puVar7;
      } while (puVar6 + uVar8 != puVar7);
    }
  }
LAB_89eb0964:
  if ((param_7 <= in_t0) || (in_stack_00000070 <= in_v1)) goto LAB_89eb1a84;
  goto LAB_89eb0608;
LAB_89eb18d0:
  iVar9 = (uVar11 >> 1) - 5;
  do {
    if (in_t3 < param_3) {
      in_t3 = in_t3 << 8;
      in_v0 = (uint)*in_v1 | in_v0 << 8;
      in_v1 = in_v1 + 1;
    }
    in_t3 = in_t3 >> 1;
    uVar11 = (int)(in_v0 - in_t3) >> 0x1f;
    param_1 = uVar11 + param_1 * 2 + 1;
    iVar9 = iVar9 + -1;
    in_v0 = (in_t3 & uVar11) + (in_v0 - in_t3);
  } while (iVar9 != 0);
  param_1 = param_1 * 0x10;
  in_t2 = (uint)*(ushort *)(in_t5 + 0x646);
  if (in_t3 < param_3) {
    in_t3 = in_t3 << 8;
    in_v0 = (uint)*in_v1 | in_v0 * 0x100;
    in_v1 = in_v1 + 1;
  }
  in_t9 = (in_t3 >> 0xb) * in_t2;
  if (in_t9 <= in_v0) goto code_r0x89eb1b6c;
  *(ushort *)(in_t5 + 0x646) = *(ushort *)(in_t5 + 0x646) + (short)(param_4 - in_t2 >> 5);
  iVar9 = 2;
  goto LAB_89eb196c;
}



/* ===== FUNCTION FUN_89eb1c40 @ 89eb1c40 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 FUN_89eb1c40(uint *param_1,byte *param_2,int param_3)

{
  bool bVar1;
  undefined4 uVar2;
  uint uVar3;
  byte *pbVar4;
  uint uVar5;
  ushort *puVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  
  uVar12 = param_1[0xd];
  uVar14 = param_1[0xb];
  uVar8 = param_1[4];
  uVar9 = (1 << (param_1[2] & 0x1f)) - 1U & uVar14;
  iVar13 = uVar12 * 0x10 + uVar9;
  uVar7 = param_1[7];
  uVar3 = param_1[8];
  pbVar4 = param_2 + param_3;
  if (uVar7 < 0x1000000) {
    if (pbVar4 <= param_2) {
      return 0;
    }
    uVar7 = uVar7 << 8;
    uVar3 = (uint)*param_2 | uVar3 << 8;
    param_2 = param_2 + 1;
  }
  uVar5 = (uVar7 >> 0xb) * (uint)*(ushort *)(uVar8 + iVar13 * 2);
  if (uVar3 < uVar5) {
    iVar13 = uVar8 + 0xe6c;
    if ((param_1[0xc] != 0) || (uVar14 != 0)) {
      uVar7 = param_1[9];
      if (uVar7 == 0) {
        uVar7 = param_1[10];
      }
      iVar13 = iVar13 + (((int)(uint)*(byte *)(param_1[5] + (uVar7 - 1)) >> (8 - *param_1 & 0x1f)) +
                        (((1 << (param_1[1] & 0x1f)) - 1U & uVar14) << (*param_1 & 0x1f))) * 0x600;
    }
    if (uVar12 < 7) {
      uVar7 = 2;
      do {
        uVar8 = uVar5;
        if (uVar5 < 0x1000000) {
          if (pbVar4 <= param_2) {
            return 0;
          }
          uVar8 = uVar5 << 8;
          uVar3 = (uint)*param_2 | uVar3 << 8;
          param_2 = param_2 + 1;
        }
        uVar5 = (uVar8 >> 0xb) * (uint)*(ushort *)(iVar13 + uVar7);
        if (uVar5 <= uVar3) {
          uVar3 = uVar3 - uVar5;
          uVar7 = uVar7 + 1;
          uVar5 = uVar8 - uVar5;
        }
        bVar1 = uVar7 < 0x100;
        uVar7 = uVar7 << 1;
      } while (bVar1);
      uVar2 = 1;
    }
    else {
      uVar7 = 0;
      if (param_1[9] < param_1[0xe]) {
        uVar7 = param_1[10];
      }
      uVar9 = (uint)*(byte *)(param_1[5] + (param_1[9] - param_1[0xe]) + uVar7);
      uVar8 = 1;
      uVar7 = 0x100;
      do {
        uVar9 = uVar9 << 1;
        uVar12 = uVar7 & uVar9;
        if (uVar5 < 0x1000000) {
          if (pbVar4 <= param_2) {
            return 0;
          }
          uVar5 = uVar5 << 8;
          uVar3 = (uint)*param_2 | uVar3 << 8;
          param_2 = param_2 + 1;
        }
        uVar14 = (uVar5 >> 0xb) * (uint)*(ushort *)(iVar13 + (uVar7 + uVar8 + uVar12) * 2);
        uVar8 = uVar8 * 2;
        if (uVar3 < uVar14) {
          uVar12 = uVar7 & ~uVar12;
          uVar5 = uVar14;
        }
        else {
          uVar5 = uVar5 - uVar14;
          uVar3 = uVar3 - uVar14;
          uVar8 = uVar8 + 1;
        }
        uVar2 = 1;
        uVar7 = uVar12;
      } while (uVar8 < 0x100);
    }
    goto joined_r0x89eb21ac;
  }
  uVar7 = uVar7 - uVar5;
  uVar3 = uVar3 - uVar5;
  puVar6 = (ushort *)(uVar8 + (uVar12 + 0xc0) * 2);
  if (uVar7 < 0x1000000) {
    if (pbVar4 <= param_2) {
      return 0;
    }
    uVar7 = uVar7 * 0x100;
    uVar3 = (uint)*param_2 | uVar3 * 0x100;
    param_2 = param_2 + 1;
  }
  uVar12 = (uVar7 >> 0xb) * (uint)*puVar6;
  uVar7 = uVar7 - uVar12;
  if (uVar3 < uVar12) {
    puVar6 = (ushort *)(uVar8 + 0x664);
    uVar2 = 2;
    uVar7 = 0;
  }
  else {
    uVar12 = uVar3 - uVar12;
    if (uVar7 < 0x1000000) {
      if (pbVar4 <= param_2) {
        return 0;
      }
      uVar7 = uVar7 * 0x100;
      uVar12 = (uint)*param_2 | uVar12 * 0x100;
      param_2 = param_2 + 1;
    }
    uVar14 = (uVar7 >> 0xb) * (uint)puVar6[0xc];
    if (uVar12 < uVar14) {
      if (uVar14 < 0x1000000) {
        if (pbVar4 <= param_2) {
          return 0;
        }
        uVar14 = uVar14 * 0x100;
        uVar12 = (uint)*param_2 | uVar12 << 8;
        param_2 = param_2 + 1;
      }
      uVar5 = (uVar14 >> 0xb) * (uint)*(ushort *)(uVar8 + (iVar13 + 0xf0) * 2);
      uVar3 = uVar12 - uVar5;
      if (uVar12 < uVar5) {
        uVar2 = 3;
        goto joined_r0x89eb21ac;
      }
      uVar12 = uVar14 - uVar5;
    }
    else {
      uVar7 = uVar7 - uVar14;
      uVar3 = uVar12 - uVar14;
      if (uVar7 < 0x1000000) {
        if (pbVar4 <= param_2) {
          return 0;
        }
        uVar7 = uVar7 * 0x100;
        uVar3 = (uint)*param_2 | uVar3 * 0x100;
        param_2 = param_2 + 1;
      }
      uVar12 = (uVar7 >> 0xb) * (uint)puVar6[0x18];
      if (uVar12 <= uVar3) {
        uVar7 = uVar7 - uVar12;
        uVar3 = uVar3 - uVar12;
        if (uVar7 < 0x1000000) {
          if (pbVar4 <= param_2) {
            return 0;
          }
          uVar7 = uVar7 * 0x100;
          uVar3 = (uint)*param_2 | uVar3 * 0x100;
          param_2 = param_2 + 1;
        }
        uVar12 = (uVar7 >> 0xb) * (uint)puVar6[0x24];
        if (uVar12 <= uVar3) {
          uVar3 = uVar3 - uVar12;
          uVar12 = uVar7 - uVar12;
        }
      }
    }
    puVar6 = (ushort *)(uVar8 + 0xa68);
    uVar2 = 3;
    uVar7 = 0xc;
  }
  if (uVar12 < 0x1000000) {
    if (pbVar4 <= param_2) {
      return 0;
    }
    uVar12 = uVar12 << 8;
    uVar3 = (uint)*param_2 | uVar3 << 8;
    param_2 = param_2 + 1;
  }
  uVar5 = (uVar12 >> 0xb) * (uint)*puVar6;
  uVar12 = uVar12 - uVar5;
  if (uVar3 < uVar5) {
    iVar10 = uVar9 * 0x10 + 4;
    iVar13 = 0;
LAB_89eb1f50:
    puVar6 = (ushort *)((int)puVar6 + iVar10);
    uVar14 = 8;
  }
  else {
    uVar3 = uVar3 - uVar5;
    if (uVar12 < 0x1000000) {
      if (pbVar4 <= param_2) {
        return 0;
      }
      uVar12 = uVar12 * 0x100;
      uVar3 = (uint)*param_2 | uVar3 * 0x100;
      param_2 = param_2 + 1;
    }
    uVar5 = (uVar12 >> 0xb) * (uint)puVar6[1];
    uVar14 = 0x100;
    if (uVar3 < uVar5) {
      iVar10 = uVar9 * 0x10 + 0x104;
      iVar13 = 8;
      goto LAB_89eb1f50;
    }
    uVar3 = uVar3 - uVar5;
    puVar6 = puVar6 + 0x102;
    uVar5 = uVar12 - uVar5;
    iVar13 = 0x10;
  }
  uVar9 = 1;
  do {
    uVar11 = uVar9 * 2;
    uVar12 = uVar5;
    if (uVar5 < 0x1000000) {
      if (pbVar4 <= param_2) {
        return 0;
      }
      uVar12 = uVar5 << 8;
      uVar3 = (uint)*param_2 | uVar3 << 8;
      param_2 = param_2 + 1;
    }
    uVar5 = (uVar12 >> 0xb) * (uint)puVar6[uVar9];
    if (uVar5 <= uVar3) {
      uVar3 = uVar3 - uVar5;
      uVar11 = uVar11 + 1;
      uVar5 = uVar12 - uVar5;
    }
    uVar9 = uVar11;
  } while (uVar11 < uVar14);
  if (uVar7 < 4) {
    uVar11 = (iVar13 - uVar14) + uVar11;
    iVar13 = 0x4e0;
    if (uVar11 < 4) {
      iVar13 = uVar11 * 0x80 + 0x360;
    }
    uVar7 = 1;
    do {
      uVar7 = uVar7 * 2;
      uVar9 = uVar5;
      if (uVar5 < 0x1000000) {
        if (pbVar4 <= param_2) {
          return 0;
        }
        uVar9 = uVar5 << 8;
        uVar3 = (uint)*param_2 | uVar3 << 8;
        param_2 = param_2 + 1;
      }
      uVar5 = (uVar9 >> 0xb) * (uint)*(ushort *)(uVar8 + uVar7 + iVar13);
      if (uVar5 <= uVar3) {
        uVar3 = uVar3 - uVar5;
        uVar7 = uVar7 + 1;
        uVar5 = uVar9 - uVar5;
      }
    } while (uVar7 < 0x40);
    uVar7 = uVar7 - 0x40;
    if (3 < uVar7) {
      if (uVar7 < 0xe) {
        uVar9 = (uVar7 >> 1) - 1;
        iVar13 = uVar8 + ((((uVar7 & 1 | 2) << (uVar9 & 0x1f)) - uVar7) + 0x2af) * 2;
      }
      else {
        iVar13 = (uVar7 >> 1) - 5;
        do {
          if (uVar5 < 0x1000000) {
            if (pbVar4 <= param_2) {
              return 0;
            }
            uVar5 = uVar5 << 8;
            uVar3 = (uint)*param_2 | uVar3 << 8;
            param_2 = param_2 + 1;
          }
          uVar5 = uVar5 >> 1;
          iVar13 = iVar13 + -1;
          uVar3 = uVar3 - (0xffffffffU - ((int)(uVar3 - uVar5) >> 0x1f) & uVar5);
        } while (iVar13 != 0);
        iVar13 = uVar8 + 0x644;
        uVar9 = 4;
      }
      iVar10 = 1;
      do {
        iVar10 = iVar10 * 2;
        uVar7 = uVar5;
        if (uVar5 < 0x1000000) {
          if (pbVar4 <= param_2) {
            return 0;
          }
          uVar7 = uVar5 << 8;
          uVar3 = (uint)*param_2 | uVar3 << 8;
          param_2 = param_2 + 1;
        }
        uVar5 = (uVar7 >> 0xb) * (uint)*(ushort *)(iVar13 + iVar10);
        if (uVar5 <= uVar3) {
          uVar3 = uVar3 - uVar5;
          iVar10 = iVar10 + 1;
          uVar5 = uVar7 - uVar5;
        }
        uVar9 = uVar9 - 1;
      } while (uVar9 != 0);
    }
  }
joined_r0x89eb21ac:
  if ((uVar5 < 0x1000000) && (pbVar4 <= param_2)) {
    return 0;
  }
  return uVar2;
}



/* ===== FUNCTION sub_89eb1dc4 @ 89eb1dc4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb1dc4(undefined4 param_1,byte *param_2,byte *param_3)

{
  bool bVar1;
  uint in_v0;
  uint in_v1;
  uint uVar2;
  uint in_t0;
  int in_t1;
  uint in_lo;
  
  while (bVar1 = in_v0 < 0x100, in_v0 = in_v0 * 2, bVar1) {
    uVar2 = in_lo;
    if (in_lo < in_t0) {
      if (param_3 <= param_2) {
        return 0;
      }
      uVar2 = in_lo << 8;
      in_v1 = (uint)*param_2 | in_v1 << 8;
      param_2 = param_2 + 1;
    }
    in_lo = (uVar2 >> 0xb) * (uint)*(ushort *)(in_t1 + in_v0);
    if (in_lo <= in_v1) {
      in_v1 = in_v1 - in_lo;
      in_v0 = in_v0 + 1;
      in_lo = uVar2 - in_lo;
    }
  }
  if ((in_lo < 0x1000000) && (param_3 <= param_2)) {
    return 0;
  }
  return 1;
}



/* ===== FUNCTION sub_89eb21c8 @ 89eb21c8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0x89eb1fcc) */
/* WARNING: Removing unreachable block (ram,0x89eb1fdc) */
/* WARNING: Removing unreachable block (ram,0x89eb1fe4) */
/* WARNING: Removing unreachable block (ram,0x89eb1fec) */
/* WARNING: Removing unreachable block (ram,0x89eb2008) */
/* WARNING: Removing unreachable block (ram,0x89eb2014) */
/* WARNING: Removing unreachable block (ram,0x89eb2028) */
/* WARNING: Removing unreachable block (ram,0x89eb2300) */
/* WARNING: Removing unreachable block (ram,0x89eb203c) */
/* WARNING: Removing unreachable block (ram,0x89eb204c) */
/* WARNING: Removing unreachable block (ram,0x89eb2054) */
/* WARNING: Removing unreachable block (ram,0x89eb2064) */
/* WARNING: Removing unreachable block (ram,0x89eb230c) */
/* WARNING: Removing unreachable block (ram,0x89eb2314) */
/* WARNING: Removing unreachable block (ram,0x89eb2320) */
/* WARNING: Removing unreachable block (ram,0x89eb2328) */
/* WARNING: Removing unreachable block (ram,0x89eb2338) */
/* WARNING: Removing unreachable block (ram,0x89eb2358) */
/* WARNING: Removing unreachable block (ram,0x89eb2070) */
/* WARNING: Removing unreachable block (ram,0x89eb2090) */
/* WARNING: Removing unreachable block (ram,0x89eb2098) */
/* WARNING: Removing unreachable block (ram,0x89eb20b0) */
/* WARNING: Removing unreachable block (ram,0x89eb20bc) */
/* WARNING: Removing unreachable block (ram,0x89eb20d0) */
/* WARNING: Removing unreachable block (ram,0x89eb2364) */
/* WARNING: Removing unreachable block (ram,0x89eb20e4) */
/* WARNING: Removing unreachable block (ram,0x89eb20f4) */
/* WARNING: Removing unreachable block (ram,0x89eb20fc) */

undefined4 sub_89eb21c8(int param_1,byte *param_2,byte *param_3)

{
  int in_v0;
  uint in_v1;
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int in_t1;
  int iVar4;
  int in_t2;
  uint uVar5;
  
  uVar1 = param_1 - in_v0;
  if (uVar1 < 0x1000000) {
    if (param_3 <= param_2) {
      return 0;
    }
    uVar1 = uVar1 * 0x100;
    in_v1 = (uint)*param_2 | in_v1 << 8;
    param_2 = param_2 + 1;
  }
  uVar3 = (uVar1 >> 0xb) * (uint)*(ushort *)(in_t1 + 0xa68);
  uVar1 = uVar1 - uVar3;
  if (in_v1 < uVar3) {
    iVar4 = in_t2 * 0x10 + 4;
  }
  else {
    in_v1 = in_v1 - uVar3;
    if (uVar1 < 0x1000000) {
      if (param_3 <= param_2) {
        return 0;
      }
      uVar1 = uVar1 * 0x100;
      in_v1 = (uint)*param_2 | in_v1 * 0x100;
      param_2 = param_2 + 1;
    }
    uVar3 = (uVar1 >> 0xb) * (uint)*(ushort *)(in_t1 + 0xa6a);
    uVar5 = 0x100;
    if (uVar3 <= in_v1) {
      in_v1 = in_v1 - uVar3;
      iVar4 = in_t1 + 0xc6c;
      uVar3 = uVar1 - uVar3;
      goto LAB_89eb1f54;
    }
    iVar4 = in_t2 * 0x10 + 0x104;
  }
  iVar4 = in_t1 + 0xa68 + iVar4;
  uVar5 = 8;
LAB_89eb1f54:
  uVar1 = 1;
  do {
    uVar1 = uVar1 * 2;
    uVar2 = uVar3;
    if (uVar3 < 0x1000000) {
      if (param_3 <= param_2) {
        return 0;
      }
      uVar2 = uVar3 << 8;
      in_v1 = (uint)*param_2 | in_v1 << 8;
      param_2 = param_2 + 1;
    }
    uVar3 = (uVar2 >> 0xb) * (uint)*(ushort *)(iVar4 + uVar1);
    if (uVar3 <= in_v1) {
      in_v1 = in_v1 - uVar3;
      uVar1 = uVar1 + 1;
      uVar3 = uVar2 - uVar3;
    }
  } while (uVar1 < uVar5);
  if ((uVar3 < 0x1000000) && (param_3 <= param_2)) {
    return 0;
  }
  return 3;
}



/* ===== FUNCTION stage2_stream_context_prepare @ 89eb23a4 =====
 * Low/medium confidence: prepares a stream/context object used by the packed payload processing code.
 */

/* Low/medium confidence: prepares a stream/context object used by the packed payload processing
   code. */

void stage2_stream_context_prepare(int param_1)

{
  *(undefined4 *)(param_1 + 0x24) = 0;
  *(undefined4 *)(param_1 + 0x4c) = 1;
  *(undefined4 *)(param_1 + 0x48) = 0;
  *(undefined4 *)(param_1 + 0x58) = 0;
  *(undefined4 *)(param_1 + 0x2c) = 0;
  *(undefined4 *)(param_1 + 0x30) = 0;
  *(undefined4 *)(param_1 + 0x50) = 1;
  return;
}



/* ===== FUNCTION stage2_stream_copy_or_decode_loop @ 89eb23c8 =====
 * Low/medium confidence: loop over input/output counters; appears to move or decode packed payload data.
 */

/* Low/medium confidence: loop over input/output counters; appears to move or decode packed payload
   data. */

bool stage2_stream_copy_or_decode_loop
               (int *param_1,uint param_2,undefined1 *param_3,uint *param_4,int param_5,
               undefined4 *param_6)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  undefined2 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  
  uVar10 = *param_4;
  *param_4 = 0;
  piVar4 = param_1;
  FUN_89eb043c();
  piVar4 = piVar4 + 0x17;
  *param_6 = 0;
  do {
    iVar6 = param_1[0x12];
    if (iVar6 == 0x112) {
      iVar6 = param_1[8];
      if (iVar6 == 0) {
        *param_6 = 1;
      }
      return iVar6 != 0;
    }
    puVar9 = param_3;
    if (param_1[0x13] != 0) {
      while( true ) {
        uVar1 = param_1[0x16];
        if (uVar10 == 0) break;
        if (4 < uVar1) goto LAB_89eb249c;
        param_1[0x16] = uVar1 + 1;
        uVar10 = uVar10 - 1;
        *(undefined1 *)((int)param_1 + uVar1 + 0x5c) = *puVar9;
        *param_4 = *param_4 + 1;
        puVar9 = puVar9 + 1;
      }
      if (4 < uVar1) {
LAB_89eb249c:
        if ((char)param_1[0x17] != '\0') {
          return true;
        }
        param_1[0x13] = 0;
        param_1[0x16] = 0;
        param_1[8] = (uint)*(byte *)((int)param_1 + 0x5d) << 0x18 |
                     (uint)*(byte *)((int)param_1 + 0x5e) << 0x10 | (uint)*(byte *)(param_1 + 0x18)
                     | (uint)*(byte *)((int)param_1 + 0x5f) << 8;
        param_1[7] = -1;
        goto LAB_89eb24ec;
      }
LAB_89eb2464:
      uVar2 = 3;
LAB_89eb2514:
      *param_6 = uVar2;
      return false;
    }
LAB_89eb24ec:
    iVar12 = 0;
    if (param_2 <= (uint)param_1[9]) {
      if (iVar6 == 0) {
        if (param_1[8] == 0) {
          uVar2 = 4;
          goto LAB_89eb2514;
        }
        iVar12 = 1;
        if (param_5 != 0) goto LAB_89eb255c;
      }
      else if (param_5 != 0) goto LAB_89eb2754;
      uVar2 = 2;
      goto LAB_89eb2514;
    }
LAB_89eb255c:
    if (param_1[0x14] != 0) {
      iVar6 = *param_1;
      iVar5 = param_1[1];
      puVar7 = (undefined2 *)param_1[4];
      for (uVar1 = 0; uVar1 < (0x300 << (iVar6 + iVar5 & 0x1fU)) + 0x736U; uVar1 = uVar1 + 1) {
        *puVar7 = 0x400;
        puVar7 = puVar7 + 1;
      }
      param_1[0x11] = 1;
      param_1[0x10] = 1;
      param_1[0xf] = 1;
      param_1[0xe] = 1;
      param_1[0xd] = 0;
      param_1[0x14] = 0;
    }
    uVar3 = param_1[0x16];
    uVar1 = uVar3;
    if (uVar3 == 0) {
      if ((uVar10 < 0x14) || (iVar12 != 0)) {
        iVar6 = FUN_89eb1c40(param_1,puVar9,uVar10);
        if (iVar6 == 0) {
          FUN_89eb03e8(param_1 + 0x17,puVar9,uVar10);
          uVar1 = *param_4;
          param_1[0x16] = uVar10;
          *param_4 = uVar1 + uVar10;
          goto LAB_89eb2464;
        }
        puVar8 = puVar9;
        if (iVar12 == 0) goto LAB_89eb2638;
        if (iVar6 != 2) goto LAB_89eb2754;
        param_1[6] = (int)puVar9;
      }
      else {
        puVar8 = puVar9 + (uVar10 - 0x14);
LAB_89eb2638:
        param_1[6] = (int)puVar9;
      }
      iVar6 = FUN_89eb04f8(param_1,param_2,puVar8);
      if (iVar6 != 0) {
        return true;
      }
      param_3 = (undefined1 *)param_1[6];
      iVar6 = (int)param_3 - (int)puVar9;
      *param_4 = *param_4 + iVar6;
    }
    else {
      while( true ) {
        uVar11 = uVar1 - uVar3;
        if ((uVar1 >= 0x14) || (uVar10 <= uVar11)) break;
        *(undefined1 *)((int)param_1 + uVar1 + 0x5c) = puVar9[((uVar1 + 1) - uVar3) + -1];
        uVar1 = uVar1 + 1;
      }
      param_1[0x16] = uVar1;
      if ((uVar1 < 0x14) || (iVar12 != 0)) {
        iVar6 = FUN_89eb1c40(param_1,piVar4,uVar1);
        if (iVar6 == 0) {
          *param_4 = *param_4 + uVar11;
          goto LAB_89eb2464;
        }
        if ((iVar12 != 0) && (iVar6 != 2)) {
LAB_89eb2754:
          *param_6 = 2;
          return true;
        }
      }
      param_1[6] = (int)piVar4;
      iVar6 = FUN_89eb04f8(param_1,param_2,piVar4);
      if (iVar6 != 0) {
        return true;
      }
      uVar3 = uVar1 - (param_1[6] - (int)piVar4);
      if ((uVar1 < (uint)(param_1[6] - (int)piVar4)) || (iVar6 = uVar11 - uVar3, uVar11 < uVar3)) {
        return (bool)0xb;
      }
      *param_4 = *param_4 + iVar6;
      param_3 = puVar9 + iVar6;
      param_1[0x16] = 0;
    }
    uVar10 = uVar10 - iVar6;
  } while( true );
}



/* ===== FUNCTION sub_89eb2548 @ 89eb2548 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool sub_89eb2548(void)

{
  int in_v0;
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined2 *puVar6;
  undefined1 *puVar7;
  int *unaff_s0;
  undefined1 *unaff_s1;
  undefined1 *puVar8;
  undefined4 *unaff_s2;
  int unaff_s3;
  uint unaff_s4;
  uint unaff_s5;
  int *unaff_s6;
  uint uVar9;
  int iVar10;
  int in_stack_00000080;
  
  do {
    iVar10 = 1;
    puVar8 = unaff_s1;
    if (in_v0 == 0) {
LAB_89eb2554:
      uVar1 = 2;
      goto LAB_89eb2514;
    }
    do {
      if (unaff_s0[0x14] != 0) {
        iVar2 = *unaff_s0;
        iVar4 = unaff_s0[1];
        puVar6 = (undefined2 *)unaff_s0[4];
        for (uVar5 = 0; uVar5 < (0x300 << (iVar2 + iVar4 & 0x1fU)) + 0x736U; uVar5 = uVar5 + 1) {
          *puVar6 = 0x400;
          puVar6 = puVar6 + 1;
        }
        unaff_s0[0x11] = 1;
        unaff_s0[0x10] = 1;
        unaff_s0[0xf] = 1;
        unaff_s0[0xe] = 1;
        unaff_s0[0xd] = 0;
        unaff_s0[0x14] = 0;
      }
      uVar3 = unaff_s0[0x16];
      uVar5 = uVar3;
      if (uVar3 == 0) {
        if ((0x13 < unaff_s5) && (iVar10 == 0)) {
          puVar7 = puVar8 + (unaff_s5 - 0x14);
LAB_89eb2638:
          unaff_s0[6] = (int)puVar8;
LAB_89eb263c:
          iVar10 = FUN_89eb04f8(unaff_s0,unaff_s4,puVar7);
          if (iVar10 != 0) {
            return true;
          }
          unaff_s1 = (undefined1 *)unaff_s0[6];
          iVar10 = (int)unaff_s1 - (int)puVar8;
          *unaff_s6 = *unaff_s6 + iVar10;
          goto LAB_89eb2668;
        }
        iVar2 = FUN_89eb1c40(unaff_s0,puVar8,unaff_s5);
        if (iVar2 != 0) {
          puVar7 = puVar8;
          if (iVar10 == 0) goto LAB_89eb2638;
          if (iVar2 == 2) {
            unaff_s0[6] = (int)puVar8;
            goto LAB_89eb263c;
          }
          goto LAB_89eb2754;
        }
        FUN_89eb03e8(unaff_s0 + 0x17,puVar8,unaff_s5);
        iVar10 = *unaff_s6;
        unaff_s0[0x16] = unaff_s5;
        *unaff_s6 = iVar10 + unaff_s5;
LAB_89eb2464:
        uVar1 = 3;
        goto LAB_89eb2514;
      }
      while( true ) {
        uVar9 = uVar5 - uVar3;
        if ((0x13 < uVar5) || (unaff_s5 <= uVar9)) break;
        *(undefined1 *)((int)unaff_s0 + uVar5 + 0x5c) = puVar8[((uVar5 + 1) - uVar3) + -1];
        uVar5 = uVar5 + 1;
      }
      unaff_s0[0x16] = uVar5;
      if ((0x13 >= uVar5) || (iVar10 != 0)) {
        iVar2 = FUN_89eb1c40(unaff_s0,unaff_s3,uVar5);
        if (iVar2 == 0) {
          *unaff_s6 = *unaff_s6 + uVar9;
          goto LAB_89eb2464;
        }
        if ((iVar10 != 0) && (iVar2 != 2)) goto LAB_89eb2754;
      }
      unaff_s0[6] = unaff_s3;
      iVar10 = FUN_89eb04f8(unaff_s0,unaff_s4,unaff_s3);
      if (iVar10 != 0) {
        return true;
      }
      uVar3 = uVar5 - (unaff_s0[6] - unaff_s3);
      if ((uVar5 < (uint)(unaff_s0[6] - unaff_s3)) || (iVar10 = uVar9 - uVar3, uVar9 < uVar3)) {
        return (bool)0xb;
      }
      *unaff_s6 = *unaff_s6 + iVar10;
      unaff_s1 = puVar8 + iVar10;
      unaff_s0[0x16] = 0;
LAB_89eb2668:
      unaff_s5 = unaff_s5 - iVar10;
      iVar2 = unaff_s0[0x12];
      if (iVar2 == 0x112) {
        iVar10 = unaff_s0[8];
        if (iVar10 == 0) {
          *unaff_s2 = 1;
        }
        return iVar10 != 0;
      }
      if (unaff_s0[0x13] != 0) {
        while( true ) {
          uVar5 = unaff_s0[0x16];
          if (unaff_s5 == 0) break;
          if (uVar5 >= 5) goto LAB_89eb249c;
          unaff_s0[0x16] = uVar5 + 1;
          unaff_s5 = unaff_s5 - 1;
          *(undefined1 *)((int)unaff_s0 + uVar5 + 0x5c) = *unaff_s1;
          *unaff_s6 = *unaff_s6 + 1;
          unaff_s1 = unaff_s1 + 1;
        }
        if (uVar5 < 5) goto LAB_89eb2464;
LAB_89eb249c:
        if ((char)unaff_s0[0x17] != '\0') {
          return true;
        }
        unaff_s0[0x13] = 0;
        unaff_s0[0x16] = 0;
        unaff_s0[8] = (uint)*(byte *)((int)unaff_s0 + 0x5d) << 0x18 |
                      (uint)*(byte *)((int)unaff_s0 + 0x5e) << 0x10 |
                      (uint)*(byte *)(unaff_s0 + 0x18) | (uint)*(byte *)((int)unaff_s0 + 0x5f) << 8;
        unaff_s0[7] = -1;
      }
      iVar10 = 0;
      puVar8 = unaff_s1;
    } while ((uint)unaff_s0[9] < unaff_s4);
    if (iVar2 != 0) {
      if (in_stack_00000080 != 0) {
LAB_89eb2754:
        *unaff_s2 = 2;
        return true;
      }
      goto LAB_89eb2554;
    }
    in_v0 = in_stack_00000080;
  } while (unaff_s0[8] != 0);
  uVar1 = 4;
LAB_89eb2514:
  *unaff_s2 = uVar1;
  return false;
}



/* ===== FUNCTION sub_89eb254c @ 89eb254c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool sub_89eb254c(void)

{
  int in_v0;
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  undefined2 *puVar6;
  undefined1 *puVar7;
  int *unaff_s0;
  undefined1 *unaff_s1;
  undefined1 *puVar8;
  undefined4 *unaff_s2;
  int unaff_s3;
  uint unaff_s4;
  uint unaff_s5;
  int *unaff_s6;
  uint uVar9;
  int iVar10;
  int in_stack_00000048;
  
  do {
    iVar10 = 1;
    puVar8 = unaff_s1;
    if (in_v0 == 0) {
LAB_89eb2554:
      uVar1 = 2;
      goto LAB_89eb2514;
    }
    do {
      if (unaff_s0[0x14] != 0) {
        iVar2 = *unaff_s0;
        iVar4 = unaff_s0[1];
        puVar6 = (undefined2 *)unaff_s0[4];
        for (uVar5 = 0; uVar5 < (0x300 << (iVar2 + iVar4 & 0x1fU)) + 0x736U; uVar5 = uVar5 + 1) {
          *puVar6 = 0x400;
          puVar6 = puVar6 + 1;
        }
        unaff_s0[0x11] = 1;
        unaff_s0[0x10] = 1;
        unaff_s0[0xf] = 1;
        unaff_s0[0xe] = 1;
        unaff_s0[0xd] = 0;
        unaff_s0[0x14] = 0;
      }
      uVar3 = unaff_s0[0x16];
      uVar5 = uVar3;
      if (uVar3 == 0) {
        if ((0x13 < unaff_s5) && (iVar10 == 0)) {
          puVar7 = puVar8 + (unaff_s5 - 0x14);
LAB_89eb2638:
          unaff_s0[6] = (int)puVar8;
LAB_89eb263c:
          iVar10 = FUN_89eb04f8(unaff_s0,unaff_s4,puVar7);
          if (iVar10 != 0) {
            return true;
          }
          unaff_s1 = (undefined1 *)unaff_s0[6];
          iVar10 = (int)unaff_s1 - (int)puVar8;
          *unaff_s6 = *unaff_s6 + iVar10;
          goto LAB_89eb2668;
        }
        iVar2 = FUN_89eb1c40(unaff_s0,puVar8,unaff_s5);
        if (iVar2 != 0) {
          puVar7 = puVar8;
          if (iVar10 == 0) goto LAB_89eb2638;
          if (iVar2 == 2) {
            unaff_s0[6] = (int)puVar8;
            goto LAB_89eb263c;
          }
          goto LAB_89eb2754;
        }
        FUN_89eb03e8(unaff_s0 + 0x17,puVar8,unaff_s5);
        iVar10 = *unaff_s6;
        unaff_s0[0x16] = unaff_s5;
        *unaff_s6 = iVar10 + unaff_s5;
LAB_89eb2464:
        uVar1 = 3;
        goto LAB_89eb2514;
      }
      while( true ) {
        uVar9 = uVar5 - uVar3;
        if ((0x13 < uVar5) || (unaff_s5 <= uVar9)) break;
        *(undefined1 *)((int)unaff_s0 + uVar5 + 0x5c) = puVar8[((uVar5 + 1) - uVar3) + -1];
        uVar5 = uVar5 + 1;
      }
      unaff_s0[0x16] = uVar5;
      if ((0x13 >= uVar5) || (iVar10 != 0)) {
        iVar2 = FUN_89eb1c40(unaff_s0,unaff_s3,uVar5);
        if (iVar2 == 0) {
          *unaff_s6 = *unaff_s6 + uVar9;
          goto LAB_89eb2464;
        }
        if ((iVar10 != 0) && (iVar2 != 2)) goto LAB_89eb2754;
      }
      unaff_s0[6] = unaff_s3;
      iVar10 = FUN_89eb04f8(unaff_s0,unaff_s4,unaff_s3);
      if (iVar10 != 0) {
        return true;
      }
      uVar3 = uVar5 - (unaff_s0[6] - unaff_s3);
      if ((uVar5 < (uint)(unaff_s0[6] - unaff_s3)) || (iVar10 = uVar9 - uVar3, uVar9 < uVar3)) {
        return (bool)0xb;
      }
      *unaff_s6 = *unaff_s6 + iVar10;
      unaff_s1 = puVar8 + iVar10;
      unaff_s0[0x16] = 0;
LAB_89eb2668:
      unaff_s5 = unaff_s5 - iVar10;
      iVar2 = unaff_s0[0x12];
      if (iVar2 == 0x112) {
        iVar10 = unaff_s0[8];
        if (iVar10 == 0) {
          *unaff_s2 = 1;
        }
        return iVar10 != 0;
      }
      if (unaff_s0[0x13] != 0) {
        while( true ) {
          uVar5 = unaff_s0[0x16];
          if (unaff_s5 == 0) break;
          if (uVar5 >= 5) goto LAB_89eb249c;
          unaff_s0[0x16] = uVar5 + 1;
          unaff_s5 = unaff_s5 - 1;
          *(undefined1 *)((int)unaff_s0 + uVar5 + 0x5c) = *unaff_s1;
          *unaff_s6 = *unaff_s6 + 1;
          unaff_s1 = unaff_s1 + 1;
        }
        if (uVar5 < 5) goto LAB_89eb2464;
LAB_89eb249c:
        if ((char)unaff_s0[0x17] != '\0') {
          return true;
        }
        unaff_s0[0x13] = 0;
        unaff_s0[0x16] = 0;
        unaff_s0[8] = (uint)*(byte *)((int)unaff_s0 + 0x5d) << 0x18 |
                      (uint)*(byte *)((int)unaff_s0 + 0x5e) << 0x10 |
                      (uint)*(byte *)(unaff_s0 + 0x18) | (uint)*(byte *)((int)unaff_s0 + 0x5f) << 8;
        unaff_s0[7] = -1;
      }
      iVar10 = 0;
      puVar8 = unaff_s1;
    } while ((uint)unaff_s0[9] < unaff_s4);
    if (iVar2 != 0) {
      if (in_stack_00000048 != 0) {
LAB_89eb2754:
        *unaff_s2 = 2;
        return true;
      }
      goto LAB_89eb2554;
    }
    in_v0 = in_stack_00000048;
  } while (unaff_s0[8] != 0);
  uVar1 = 4;
LAB_89eb2514:
  *unaff_s2 = uVar1;
  return false;
}



/* ===== FUNCTION sub_89eb275c @ 89eb275c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

int sub_89eb275c(int param_1,int param_2,uint *param_3,int param_4,int *param_5,undefined4 param_6,
                undefined4 param_7)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int local_38 [2];
  int local_30;
  
  uVar4 = *param_3;
  iVar6 = *param_5;
  *param_3 = 0;
  *param_5 = 0;
  do {
    iVar2 = *(int *)(param_1 + 0x28);
    if (*(int *)(param_1 + 0x24) == iVar2) {
      *(undefined4 *)(param_1 + 0x24) = 0;
    }
    iVar5 = *(int *)(param_1 + 0x24);
    uVar1 = 0;
    if (uVar4 <= (uint)(iVar2 - iVar5)) {
      iVar2 = uVar4 + iVar5;
      uVar1 = param_6;
    }
    local_38[0] = iVar6;
    local_30 = stage2_stream_copy_or_decode_loop(param_1,iVar2,param_4,local_38,uVar1,param_7);
    iVar2 = *(int *)(param_1 + 0x14);
    *param_5 = *param_5 + local_38[0];
    param_4 = param_4 + local_38[0];
    iVar3 = *(int *)(param_1 + 0x24) - iVar5;
    iVar6 = iVar6 - local_38[0];
    FUN_89eb03e8(param_2,iVar2 + iVar5,iVar3);
    param_2 = param_2 + iVar3;
    uVar4 = uVar4 - iVar3;
    *param_3 = *param_3 + iVar3;
  } while (((local_30 == 0) && (iVar3 != 0)) && (uVar4 != 0));
  return local_30;
}



/* ===== FUNCTION sub_89eb288c @ 89eb288c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb288c(int param_1,int param_2)

{
  (**(code **)(param_2 + 4))(param_2,*(undefined4 *)(param_1 + 0x10));
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



/* ===== FUNCTION stage2_stream_context_release @ 89eb2890 =====
 * Low/medium confidence: calls a function pointer from a context and clears a stream buffer pointer.
 */

/* Low/medium confidence: calls a function pointer from a context and clears a stream buffer
   pointer. */

void stage2_stream_context_release(int param_1,int param_2)

{
  (**(code **)(param_2 + 4))(param_2,*(undefined4 *)(param_1 + 0x10));
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



/* ===== FUNCTION sub_89eb28c4 @ 89eb28c4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb28c4(int param_1,int *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (0x300 << (*param_2 + param_2[1] & 0x1fU)) + 0x736;
  if ((*(int *)(param_1 + 0x10) == 0) || (*(int *)(param_1 + 0x54) != iVar2)) {
    stage2_stream_context_release(param_1,param_3);
    iVar1 = (*(code *)*param_3)(param_3,iVar2 * 2);
    *(int *)(param_1 + 0x10) = iVar1;
    *(int *)(param_1 + 0x54) = iVar2;
    if (iVar1 == 0) {
      return 2;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_89eb28c8 @ 89eb28c8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 FUN_89eb28c8(int param_1,int *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (0x300 << (*param_2 + param_2[1] & 0x1fU)) + 0x736;
  if ((*(int *)(param_1 + 0x10) == 0) || (*(int *)(param_1 + 0x54) != iVar2)) {
    stage2_stream_context_release(param_1,param_3);
    iVar1 = (*(code *)*param_3)(param_3,iVar2 * 2);
    *(int *)(param_1 + 0x10) = iVar1;
    *(int *)(param_1 + 0x54) = iVar2;
    if (iVar1 == 0) {
      return 2;
    }
  }
  return 0;
}



/* ===== FUNCTION sub_89eb2954 @ 89eb2954 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb2954(int param_1,int param_2)

{
  stage2_stream_context_release();
  (**(code **)(param_2 + 4))(param_2,*(undefined4 *)(param_1 + 0x14));
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}



/* ===== FUNCTION FUN_89eb2958 @ 89eb2958 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_89eb2958(int param_1,int param_2)

{
  stage2_stream_context_release();
  (**(code **)(param_2 + 4))(param_2,*(undefined4 *)(param_1 + 0x14));
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}



/* ===== FUNCTION sub_89eb2998 @ 89eb2998 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0x89eb2a00) */
/* WARNING: Removing unreachable block (ram,0x89eb29e0) */
/* WARNING: Removing unreachable block (ram,0x89eb2a14) */

undefined4 sub_89eb2998(uint *param_1,byte *param_2,uint param_3)

{
  uint uVar1;
  
  if (4 < param_3) {
    uVar1 = *(uint *)(param_2 + 1);
    if (uVar1 < 0x1000) {
      uVar1 = 0x1000;
    }
    param_1[3] = uVar1;
    uVar1 = (uint)*param_2;
    if (uVar1 < 0xe1) {
      *param_1 = uVar1 % 9;
      param_1[2] = uVar1 / 0x2d;
      param_1[1] = (uVar1 / 9) % 5;
      return 0;
    }
  }
  return 4;
}



/* ===== FUNCTION FUN_89eb299c @ 89eb299c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0x89eb2a00) */
/* WARNING: Removing unreachable block (ram,0x89eb29e0) */
/* WARNING: Removing unreachable block (ram,0x89eb2a14) */

undefined4 FUN_89eb299c(uint *param_1,byte *param_2,uint param_3)

{
  uint uVar1;
  
  if (4 < param_3) {
    uVar1 = *(uint *)(param_2 + 1);
    if (uVar1 < 0x1000) {
      uVar1 = 0x1000;
    }
    param_1[3] = uVar1;
    uVar1 = (uint)*param_2;
    if (uVar1 < 0xe1) {
      *param_1 = uVar1 % 9;
      param_1[2] = uVar1 / 0x2d;
      param_1[1] = (uVar1 / 9) % 5;
      return 0;
    }
  }
  return 4;
}



/* ===== FUNCTION sub_89eb29b0 @ 89eb29b0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0x89eb2a00) */
/* WARNING: Removing unreachable block (ram,0x89eb29e0) */
/* WARNING: Removing unreachable block (ram,0x89eb2a14) */

undefined4 sub_89eb29b0(uint *param_1,byte *param_2)

{
  uint uVar1;
  
  uVar1 = *(uint *)(param_2 + 1);
  if (uVar1 < 0x1000) {
    uVar1 = 0x1000;
  }
  param_1[3] = uVar1;
  uVar1 = (uint)*param_2;
  if (0xe0 < uVar1) {
    return 4;
  }
  *param_1 = uVar1 % 9;
  param_1[2] = uVar1 / 0x2d;
  param_1[1] = (uVar1 / 9) % 5;
  return 0;
}



/* ===== FUNCTION stage2_payload_header_parse @ 89eb2a28 =====
 * Medium confidence: parses a small packed-payload header into size/type fields and validates ranges.
 */

/* Medium confidence: parses a small packed-payload header into size/type fields and validates
   ranges. */

int stage2_payload_header_parse
              (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined1 auStack_20 [20];
  
  iVar1 = FUN_89eb299c(auStack_20);
  if ((iVar1 == 0) && (iVar1 = FUN_89eb28c8(param_1,auStack_20,param_4), iVar1 == 0)) {
    FUN_89eb03e8(param_1,auStack_20,0x10);
  }
  return iVar1;
}



/* ===== FUNCTION sub_89eb2a88 @ 89eb2a88 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

int sub_89eb2a88(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  code *pcVar4;
  uint in_stack_0000001c;
  
  iVar1 = FUN_89eb299c(&stack0x00000010);
  if (iVar1 != 0) {
    return iVar1;
  }
  iVar1 = FUN_89eb28c8(param_1,&stack0x00000010,param_4);
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = 0x3f0000;
  if (in_stack_0000001c < 0x40000000) {
    uVar2 = 0xfff;
    if (in_stack_0000001c < 0x400000) goto LAB_89eb2af8;
    uVar2 = 0xf0000;
  }
  uVar2 = uVar2 | 0xffff;
LAB_89eb2af8:
  uVar3 = uVar2 + in_stack_0000001c & ~uVar2;
  uVar2 = in_stack_0000001c;
  if (in_stack_0000001c < uVar3) {
    uVar2 = uVar3;
  }
  if ((*(int *)(param_1 + 0x14) == 0) || (*(uint *)(param_1 + 0x28) != uVar2)) {
    (*(code *)param_4[1])(param_4);
    pcVar4 = (code *)*param_4;
    *(undefined4 *)(param_1 + 0x14) = 0;
    iVar1 = (*pcVar4)(param_4,uVar2);
    *(int *)(param_1 + 0x14) = iVar1;
    if (iVar1 == 0) {
      stage2_stream_context_release(param_1,param_4);
      return 2;
    }
  }
  *(uint *)(param_1 + 0x28) = uVar2;
  FUN_89eb03e8(param_1,&stack0x00000010,0x10);
  return 0;
}



/* ===== FUNCTION FUN_89eb2a8c @ 89eb2a8c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

int FUN_89eb2a8c(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  code *pcVar4;
  undefined1 auStack_20 [12];
  uint local_14;
  
  iVar1 = FUN_89eb299c(auStack_20);
  if (iVar1 != 0) {
    return iVar1;
  }
  iVar1 = FUN_89eb28c8(param_1,auStack_20,param_4);
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = 0x3f0000;
  if (local_14 < 0x40000000) {
    uVar2 = 0xfff;
    if (local_14 < 0x400000) goto LAB_89eb2af8;
    uVar2 = 0xf0000;
  }
  uVar2 = uVar2 | 0xffff;
LAB_89eb2af8:
  uVar3 = uVar2 + local_14 & ~uVar2;
  uVar2 = local_14;
  if (local_14 < uVar3) {
    uVar2 = uVar3;
  }
  if ((*(int *)(param_1 + 0x14) == 0) || (*(uint *)(param_1 + 0x28) != uVar2)) {
    (*(code *)param_4[1])(param_4);
    pcVar4 = (code *)*param_4;
    *(undefined4 *)(param_1 + 0x14) = 0;
    iVar1 = (*pcVar4)(param_4,uVar2);
    *(int *)(param_1 + 0x14) = iVar1;
    if (iVar1 == 0) {
      stage2_stream_context_release(param_1,param_4);
      return 2;
    }
  }
  *(uint *)(param_1 + 0x28) = uVar2;
  FUN_89eb03e8(param_1,auStack_20,0x10);
  return 0;
}



/* ===== FUNCTION sub_89eb2b78 @ 89eb2b78 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb2b78(void)

{
  undefined4 unaff_s0;
  int unaff_s1;
  
  *(undefined4 *)(unaff_s1 + 0x28) = unaff_s0;
  FUN_89eb03e8(unaff_s1,&stack0x00000040,0x10);
  return 0;
}



/* ===== FUNCTION sub_89eb2b7c @ 89eb2b7c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb2b7c(void)

{
  undefined4 unaff_s0;
  int unaff_s1;
  
  *(undefined4 *)(unaff_s1 + 0x28) = unaff_s0;
  FUN_89eb03e8(unaff_s1,&stack0x00000010,0x10);
  return 0;
}



/* ===== FUNCTION stage2_payload_decode_dispatch @ 89eb2b98 =====
 * Low/medium confidence: higher-level packed-payload decode dispatcher; checks mode/status values and invokes decode helpers.
 */

/* Low/medium confidence: higher-level packed-payload decode dispatcher; checks mode/status values
   and invokes decode helpers. */

int stage2_payload_decode_dispatch
              (undefined4 param_1,undefined4 *param_2,undefined4 param_3,uint *param_4,
              undefined4 param_5,undefined4 param_6,undefined4 param_7,int *param_8,
              undefined4 param_9)

{
  undefined4 uVar1;
  uint uVar2;
  undefined1 auStack_98 [16];
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_74;
  undefined4 local_70;
  int local_28;
  
  uVar2 = *param_4;
  uVar1 = *param_2;
  local_28 = 6;
  *param_4 = 0;
  *param_2 = 0;
  *param_8 = 0;
  if (4 < uVar2) {
    local_84 = 0;
    local_88 = 0;
    local_28 = stage2_payload_header_parse(auStack_98,param_5,param_6,param_9);
    if (local_28 == 0) {
      local_84 = param_1;
      local_70 = uVar1;
      stage2_stream_context_prepare(auStack_98);
      *param_4 = uVar2;
      local_28 = stage2_stream_copy_or_decode_loop(auStack_98,uVar1,param_3,param_4,param_7,param_8)
      ;
      *param_2 = local_74;
      if ((local_28 == 0) && (*param_8 == 3)) {
        local_28 = 6;
      }
      stage2_stream_context_release(auStack_98,param_9);
    }
  }
  return local_28;
}



/* ===== FUNCTION sub_89eb2ca0 @ 89eb2ca0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb2ca0(void)

{
  return;
}



/* ===== FUNCTION sub_89eb2ca4 @ 89eb2ca4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_89eb2ca4(void)

{
  return;
}



/* ===== FUNCTION stage2_payload_size_probe @ 89eb2cb4 =====
 * Low/medium confidence: helper used to probe or return decoded payload size/status.
 */

/* Low/medium confidence: helper used to probe or return decoded payload size/status. */

undefined4 stage2_payload_size_probe(undefined4 param_1,uint *param_2,int param_3,int param_4)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined1 *local_20;
  code *local_1c;
  undefined4 local_18;
  uint local_14;
  int local_10 [2];
  
  local_20 = &LAB_89eb2cac;
  local_10[0] = param_4 + -5;
  local_1c = sub_89eb2ca4;
  local_18 = 0;
  iVar5 = 0;
  uVar3 = 0;
  uVar1 = 0;
  iVar4 = param_3;
  do {
    iVar4 = (uint)*(byte *)(iVar4 + 5) << ((uVar1 & 3) << 3);
    if ((int)uVar1 < 4) {
      uVar3 = uVar3 + iVar4;
    }
    else {
      iVar5 = iVar5 + iVar4;
    }
    uVar1 = uVar1 + 1;
    iVar4 = param_3 + uVar1;
  } while (uVar1 != 8);
  if (iVar5 == 0) {
    if ((uVar3 != 0xffffffff) && (*param_2 < uVar3)) {
      return 7;
    }
  }
  else {
    if (iVar5 != -1) {
      return 1;
    }
    if (uVar3 != 0xffffffff) {
      return 1;
    }
  }
  local_14 = *param_2;
  if (uVar3 < *param_2) {
    local_14 = uVar3;
  }
  uVar2 = stage2_payload_decode_dispatch
                    (param_1,&local_14,param_3 + 0xd,local_10,param_3,5,1,&local_18,&local_20);
  *param_2 = local_14;
  return uVar2;
}



/* ===== FUNCTION sub_89eb2cb8 @ 89eb2cb8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb2cb8(undefined4 param_1,uint *param_2,int param_3,int param_4)

{
  int in_v0;
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int local_20;
  code *local_1c;
  undefined4 local_18;
  uint local_14;
  int local_10 [2];
  
  local_20 = in_v0 + 0x2cac;
  local_10[0] = param_4 + -5;
  local_1c = sub_89eb2ca4;
  local_18 = 0;
  iVar5 = 0;
  uVar3 = 0;
  uVar1 = 0;
  iVar4 = param_3;
  do {
    iVar4 = (uint)*(byte *)(iVar4 + 5) << ((uVar1 & 3) << 3);
    if ((int)uVar1 < 4) {
      uVar3 = uVar3 + iVar4;
    }
    else {
      iVar5 = iVar5 + iVar4;
    }
    uVar1 = uVar1 + 1;
    iVar4 = param_3 + uVar1;
  } while (uVar1 != 8);
  if (iVar5 == 0) {
    if ((uVar3 != 0xffffffff) && (*param_2 < uVar3)) {
      return 7;
    }
  }
  else {
    if (iVar5 != -1) {
      return 1;
    }
    if (uVar3 != 0xffffffff) {
      return 1;
    }
  }
  local_14 = *param_2;
  if (uVar3 < *param_2) {
    local_14 = uVar3;
  }
  uVar2 = stage2_payload_decode_dispatch
                    (param_1,&local_14,param_3 + 0xd,local_10,param_3,5,1,&local_18,&local_20);
  *param_2 = local_14;
  return uVar2;
}



/* ===== FUNCTION sub_89eb2dc8 @ 89eb2dc8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_89eb2dc8(undefined4 param_1,undefined4 param_2)

{
  int iStack00000030;
  undefined4 in_stack_00000058;
  int *in_stack_00000064;
  
  iStack00000030 = 0x10000000;
  stage2_payload_size_probe(in_stack_00000058,&stack0x00000030,param_1,param_2);
  if ((iStack00000030 != 0x10000000) && (in_stack_00000064 != (int *)0x0)) {
    *in_stack_00000064 = iStack00000030;
  }
  return 0;
}



/* ===== FUNCTION FUN_89eb2dcc @ 89eb2dcc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4
FUN_89eb2dcc(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7,int *param_8)

{
  int local_18 [3];
  
  local_18[0] = 0x10000000;
  stage2_payload_size_probe(param_5,local_18,param_1,param_2);
  if ((local_18[0] != 0x10000000) && (param_8 != (int *)0x0)) {
    *param_8 = local_18[0];
  }
  return 0;
}



/* ===== FUNCTION sub_89eb2e28 @ 89eb2e28 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Control flow encountered bad instruction data */

void sub_89eb2e28(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION sub_89eb2e2c @ 89eb2e2c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Control flow encountered bad instruction data */

void sub_89eb2e2c(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


