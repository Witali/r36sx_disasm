
/* ===== FUNCTION first_stage_entry_copy_and_init @ bfe00800 =====
 * High confidence: NOR entry. Checks a boot magic at 0xbfe00014; if missing, copies code/data from the flash alias around 0xafc02000 to 0xbfe02000, then calls low-level init routines.
 */

/* High confidence: NOR entry. Checks a boot magic at 0xbfe00014; if missing, copies code/data from
   the flash alias around 0xafc02000 to 0xbfe02000, then calls low-level init routines. */

void first_stage_entry_copy_and_init(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  
  if (DAT_bfe00014 != -0x15511552) {
    puVar1 = (undefined4 *)&DAT_afc02000;
    puVar2 = (undefined4 *)0xbfe02000;
    do {
      *puVar2 = *puVar1;
      puVar1 = puVar1 + 1;
      puVar2 = puVar2 + 1;
    } while (puVar2 < (undefined4 *)0xbfe04000);
  }
  FUN_bfe02c40();
  FUN_bfe0103c();
  FUN_bfe013e8();
  FUN_bfe02434();
  FUN_bfe00db4();
  FUN_bfe011a4();
  FUN_bfe024f4();
  FUN_bfe00e2c();
  return;
}



/* ===== FUNCTION sub_bfe0089c @ bfe0089c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe0089c(void)

{
  return;
}



/* ===== FUNCTION sub_bfe008a8 @ bfe008a8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Control flow encountered bad instruction data */

void sub_bfe008a8(uint param_1)

{
  uint uVar1;
  uint *puVar2;
  int in_v1;
  
  uVar1 = param_1 & 3;
  *(uint *)(param_1 - uVar1) =
       *(uint *)(param_1 - uVar1) & 0xffffffffU >> (4 - uVar1) * 8 | 0 << uVar1 * 8;
  uVar1 = param_1 + 0x1000 & 3;
  puVar2 = (uint *)((param_1 + 0x1000) - uVar1);
  *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar1) * 8 | 0 << uVar1 * 8;
  uVar1 = param_1 - 0x2000 & 3;
  puVar2 = (uint *)((param_1 - 0x2000) - uVar1);
  *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar1) * 8 | in_v1 << uVar1 * 8;
  uVar1 = param_1 + 0x1800 & 3;
  puVar2 = (uint *)((param_1 + 0x1800) - uVar1);
  *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar1) * 8 | 0 << uVar1 * 8;
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION FUN_bfe00db4 @ bfe00db4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe00db4(void)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = 0;
  while ((uVar2 < DAT_bfe008c0 >> 2 &&
         (iVar1 = FUN_bfe00e6c(&DAT_bfe008c4 + uVar2 * 0x10), iVar1 == 0))) {
    uVar2 = uVar2 + 1;
  }
  FUN_bfe00e3c(&DAT_a0000000,0,0x20);
  return;
}



/* ===== FUNCTION sub_bfe00e20 @ bfe00e20 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe00e20(void)

{
  int iVar1;
  uint unaff_s0;
  int unaff_s1;
  int unaff_s2;
  
  do {
    unaff_s0 = unaff_s0 + 1;
    if (*(uint *)(unaff_s2 + 0x8c0) >> 2 <= unaff_s0) break;
    iVar1 = FUN_bfe00e6c(unaff_s1 + unaff_s0 * 0x10);
  } while (iVar1 == 0);
  FUN_bfe00e3c(&DAT_a0000000,0,0x20);
  return;
}



/* ===== FUNCTION FUN_bfe00e2c @ bfe00e2c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe00e2c(void)

{
                    /* WARNING: Could not recover jumptable at 0xbfe00e34. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_bfe02e90)();
  return;
}



/* ===== FUNCTION FUN_bfe00e3c @ bfe00e3c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined1 * FUN_bfe00e3c(undefined1 *param_1,undefined1 param_2,int param_3)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  puVar1 = param_1;
  puVar2 = param_1 + param_3;
  for (; param_1 != puVar2; param_1 = param_1 + 1) {
    *param_1 = param_2;
  }
  return puVar1;
}



/* ===== FUNCTION sub_bfe00e58 @ bfe00e58 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe00e58(undefined4 param_1,undefined1 param_2,undefined1 *param_3)

{
  undefined1 *in_v1;
  
  do {
    *in_v1 = param_2;
    in_v1 = in_v1 + 1;
  } while (in_v1 != param_3);
  return;
}



/* ===== FUNCTION FUN_bfe00e64 @ bfe00e64 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe00e64(void)

{
  return;
}



/* ===== FUNCTION FUN_bfe00e6c @ bfe00e6c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool FUN_bfe00e6c(int *param_1)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  
  puVar2 = (uint *)param_1[1];
  if ((uint)puVar2 >> 0x10 < 4) {
    puVar2 = (uint *)((uint)puVar2 & 0xffff | *(uint *)(((uint)puVar2 >> 0x10) * 4 + -0x401ff750));
  }
  iVar1 = *param_1;
  if (iVar1 == 1) {
    for (uVar3 = 0; uVar3 < (uint)param_1[1]; uVar3 = uVar3 + 1) {
    }
  }
  else if (iVar1 == 8) {
    *(char *)puVar2 = (char)param_1[2];
  }
  else if (iVar1 == 0x16) {
    *(short *)puVar2 = (short)param_1[2];
  }
  else {
    if (iVar1 == 0x32) {
      uVar3 = param_1[2];
    }
    else if (iVar1 == 0x132) {
      uVar3 = (*puVar2 ^ param_1[2]) & param_1[3] ^ *puVar2;
    }
    else {
      if (iVar1 == 0x232) {
        *puVar2 = param_1[2];
        do {
        } while (*puVar2 != param_1[2]);
        return false;
      }
      if (iVar1 == 0x320000) {
        return false;
      }
      if (iVar1 == 0x1320000) {
        do {
        } while ((*puVar2 & param_1[3]) != param_1[2]);
        return false;
      }
      if (iVar1 == 0x2320000) {
        do {
        } while ((*puVar2 & param_1[3]) == param_1[2]);
        return false;
      }
      if (iVar1 == 0x432) {
        uVar3 = (param_1[2] << (param_1[3] & 0x1fU)) + *puVar2;
      }
      else {
        if (iVar1 != 0x332) {
          if (iVar1 == -0x55443323) {
            FUN_bfe00e64();
            return false;
          }
          return iVar1 == -0x522133f0;
        }
        uVar3 = *puVar2 - (param_1[2] << (param_1[3] & 0x1fU));
      }
    }
    *puVar2 = uVar3;
  }
  return false;
}



/* ===== FUNCTION sub_bfe00ed0 @ bfe00ed0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool sub_bfe00ed0(int param_1,int param_2)

{
  int in_v0;
  uint uVar1;
  uint *in_v1;
  
  if (in_v0 == param_2) {
    *(undefined1 *)in_v1 = *(undefined1 *)(param_1 + 8);
  }
  else if (in_v0 == 0x16) {
    *(undefined2 *)in_v1 = *(undefined2 *)(param_1 + 8);
  }
  else {
    if (in_v0 == 0x32) {
      uVar1 = *(uint *)(param_1 + 8);
    }
    else if (in_v0 == 0x132) {
      uVar1 = (*in_v1 ^ *(uint *)(param_1 + 8)) & *(uint *)(param_1 + 0xc) ^ *in_v1;
    }
    else {
      if (in_v0 == 0x232) {
        *in_v1 = *(uint *)(param_1 + 8);
        do {
        } while (*in_v1 != *(uint *)(param_1 + 8));
        return false;
      }
      if (in_v0 == 0x320000) {
        return false;
      }
      if (in_v0 == 0x1320000) {
        do {
        } while ((*in_v1 & *(uint *)(param_1 + 0xc)) != *(uint *)(param_1 + 8));
        return false;
      }
      if (in_v0 == 0x2320000) {
        do {
        } while ((*in_v1 & *(uint *)(param_1 + 0xc)) == *(uint *)(param_1 + 8));
        return false;
      }
      if (in_v0 == 0x432) {
        uVar1 = (*(int *)(param_1 + 8) << (*(uint *)(param_1 + 0xc) & 0x1f)) + *in_v1;
      }
      else {
        if (in_v0 != 0x332) {
          if (in_v0 == -0x55443323) {
            FUN_bfe00e64();
            return false;
          }
          return in_v0 == -0x522133f0;
        }
        uVar1 = *in_v1 - (*(int *)(param_1 + 8) << (*(uint *)(param_1 + 0xc) & 0x1f));
      }
    }
    *in_v1 = uVar1;
  }
  return false;
}



/* ===== FUNCTION sub_bfe0100c @ bfe0100c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_bfe0100c(void)

{
  FUN_bfe00e64();
  return 0;
}



/* ===== FUNCTION sub_bfe01028 @ bfe01028 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool sub_bfe01028(void)

{
  int in_v0;
  int in_v1;
  
  return in_v0 + in_v1 + 0x33f0 == 0;
}



/* ===== FUNCTION sub_bfe0102c @ bfe0102c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

bool sub_bfe0102c(void)

{
  int in_v0;
  int in_v1;
  
  return in_v0 + in_v1 + 0x33f0 == 0;
}



/* ===== FUNCTION FUN_bfe0103c @ bfe0103c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe0103c(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  _DAT_b8800060 = _DAT_b8800060 & 0x7fffffff;
  uVar3 = _DAT_b8818a70 & 0xff;
  if (DAT_bfe00030 == '\x01') {
    _DAT_b8818a00 = _DAT_b8818a00 | 0x40000000;
  }
  else {
    _DAT_b8818a00 = _DAT_b8818a00 & 0xbfffffff;
  }
  if (DAT_bfe00031 == '\x01') {
    uVar1 = _DAT_b8818a00 | 0x80000000;
  }
  else {
    uVar1 = _DAT_b8818a00 & 0x7fffffff;
  }
  _DAT_b8818a00 = uVar1 & 0xfc00ffff;
  if (((uVar3 == 0xa5) || (uVar3 == 0x5a)) || (iVar2 = 0, uVar3 == 0x55)) {
    iVar2 = _DAT_b8818a0c + _DAT_b8818a10;
    uVar1 = (uVar1 >> 0x10) + (_DAT_b8818a08 >> 0x10) & 0x3ff;
    if (999 < uVar1) {
      iVar2 = iVar2 + 1;
      uVar1 = uVar1 - 1000;
    }
    _DAT_b8818a00 = _DAT_b8818a00 | uVar1 << 0x10;
  }
  else {
    uVar3 = 0x55;
  }
  _DAT_b8800080 = _DAT_b8800080 & 0xffbfffff;
  _DAT_b8818a70 = uVar3 | _DAT_b8818a70 & 0xffffff00;
  _DAT_b8818a10 = iVar2;
  _DAT_b8818a08 = 4;
  return;
}



/* ===== FUNCTION sub_bfe01160 @ bfe01160 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe01160(int param_1)

{
  uint in_v0;
  uint in_v1;
  uint uVar1;
  int iVar2;
  uint in_t0;
  uint in_t1;
  
  uVar1 = in_v1 & (in_v0 | 0xffff);
  if (*(char *)(param_1 + 0x31) == '\x01') {
    uVar1 = uVar1 | 0x80000000;
  }
  else {
    uVar1 = uVar1 & 0x7fffffff;
  }
  _DAT_b8818a00 = uVar1 & 0xfc00ffff;
  if (((in_t0 == 0xa5) || (in_t0 == 0x5a)) || (iVar2 = 0, in_t0 == 0x55)) {
    iVar2 = _DAT_b8818a0c + _DAT_b8818a10;
    uVar1 = (uVar1 >> 0x10) + (_DAT_b8818a08 >> 0x10) & 0x3ff;
    if (999 < uVar1) {
      iVar2 = iVar2 + 1;
      uVar1 = uVar1 - 1000;
    }
    _DAT_b8818a00 = _DAT_b8818a00 | uVar1 << 0x10;
  }
  else {
    in_t0 = 0x55;
  }
  _DAT_b8800080 = _DAT_b8800080 & 0xffbfffff;
  _DAT_b8818a70 = in_t0 | in_t1 & 0xffffff00;
  _DAT_b8818a10 = iVar2;
  _DAT_b8818a08 = 4;
  return;
}



/* ===== FUNCTION FUN_bfe01180 @ bfe01180 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_bfe01180(void)

{
  return _DAT_b8818a0c * 1000 + (uint)_DAT_b8818a0a;
}



/* ===== FUNCTION FUN_bfe011a4 @ bfe011a4 =====
 * References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe011a4(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  uint uVar4;
  short sVar5;
  short sVar6;
  undefined1 *puVar7;
  uint uVar8;
  uint uVar9;
  undefined1 local_28 [4];
  undefined1 local_24;
  
  sVar5 = FUN_bfe01180();
  uVar4 = DAT_bfe00040;
  uVar9 = DAT_bfe00028;
  uVar3 = DAT_bfe00024;
  uVar2 = DAT_bfe00020;
  iVar1 = DAT_bfe00014;
  DAT_bfe02e90 = DAT_bfe0002c;
  first_stage_clock_or_pll_select(DAT_bfe00044);
  if (iVar1 == -0x15511552) {
    FUN_bfe029c8(uVar2,uVar3,uVar9,uVar4 & 0xff);
  }
  else {
    uVar9 = uVar9 & 0x3fffff;
    _DAT_b882e098 = CONCAT11(1,DAT_b882e098);
    local_28[1] = (char)(uVar9 >> 0x10);
    local_28[2] = (char)(uVar9 >> 8);
    local_24 = 0xff;
    local_28[3] = (char)uVar9;
    if (uVar4 == 1) {
      local_28[0] = 3;
      uVar9 = 4;
    }
    else {
      local_28[0] = 0x3b;
      uVar9 = 5;
    }
    _DAT_b882e0c8 = _DAT_b882e0c8 | 0x7000000;
    uVar8 = 0;
    puVar7 = local_28;
    while (uVar8 < uVar9) {
      DAT_afc00000 = *puVar7;
      puVar7 = puVar7 + 1;
      uVar8 = (int)puVar7 - (int)local_28;
    }
    if (uVar4 == 1) {
      _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
    }
    else {
      _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff | 0x10000;
    }
    first_stage_sfspi_transfer_setup(uVar3,uVar2);
    _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf9ffffff;
  }
  sVar6 = FUN_bfe01180();
  _DAT_a0000006 = sVar6 - sVar5;
  return;
}



/* ===== FUNCTION sub_bfe01274 @ bfe01274 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe01274(void)

{
  short sVar1;
  uint in_v0;
  uint *puVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  uint unaff_s0;
  short unaff_s1;
  int unaff_s2;
  undefined4 unaff_s3;
  undefined4 unaff_s4;
  undefined1 uStack00000048;
  undefined1 uStack00000049;
  undefined1 uStack0000004a;
  undefined1 uStack0000004b;
  undefined1 uStack0000004c;
  
  puVar2 = (uint *)(in_v0 | 0xe0c8);
  *puVar2 = *puVar2 | 0x1000000;
  uVar5 = unaff_s0 & 0x3fffff;
  *(undefined1 *)((int)puVar2 - 0x2f) = 1;
  uStack00000049 = (char)(uVar5 >> 0x10);
  uStack0000004a = (char)(uVar5 >> 8);
  uStack0000004c = 0xff;
  uStack0000004b = (char)uVar5;
  if (unaff_s2 == 1) {
    uStack00000048 = 3;
    uVar5 = 4;
  }
  else {
    uStack00000048 = 0x3b;
    uVar5 = 5;
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x6000000;
  uVar4 = 0;
  puVar3 = &stack0x00000048;
  while (uVar4 < uVar5) {
    DAT_afc00000 = *puVar3;
    puVar3 = puVar3 + 1;
    uVar4 = (int)puVar3 - (int)&stack0x00000048;
  }
  if (unaff_s2 == 1) {
    _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  }
  else {
    _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff | 0x10000;
  }
  first_stage_sfspi_transfer_setup(unaff_s3,unaff_s4);
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf9ffffff;
  sVar1 = FUN_bfe01180();
  _DAT_a0000006 = sVar1 - unaff_s1;
  return;
}



/* ===== FUNCTION first_stage_spi_nor_command_path @ bfe01278 =====
 * Medium confidence: talks to the SPI/SF controller registers around 0xb882e0c8 and prepares small command buffers on the stack.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Medium confidence: talks to the SPI/SF controller registers around 0xb882e0c8 and prepares small
   command buffers on the stack. */

void first_stage_spi_nor_command_path(void)

{
  short sVar1;
  uint in_v0;
  uint *puVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  uint unaff_s0;
  short unaff_s1;
  int unaff_s2;
  undefined4 unaff_s3;
  undefined4 unaff_s4;
  undefined1 uStack00000010;
  undefined1 uStack00000011;
  undefined1 uStack00000012;
  undefined1 uStack00000013;
  undefined1 uStack00000014;
  
  puVar2 = (uint *)(in_v0 | 0xe0c8);
  *puVar2 = *puVar2 | 0x1000000;
  uVar5 = unaff_s0 & 0x3fffff;
  *(undefined1 *)((int)puVar2 - 0x2f) = 1;
  uStack00000011 = (char)(uVar5 >> 0x10);
  uStack00000012 = (char)(uVar5 >> 8);
  uStack00000014 = 0xff;
  uStack00000013 = (char)uVar5;
  if (unaff_s2 == 1) {
    uStack00000010 = 3;
    uVar5 = 4;
  }
  else {
    uStack00000010 = 0x3b;
    uVar5 = 5;
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x6000000;
  uVar4 = 0;
  puVar3 = &stack0x00000010;
  while (uVar4 < uVar5) {
    DAT_afc00000 = *puVar3;
    puVar3 = puVar3 + 1;
    uVar4 = (int)puVar3 - (int)&stack0x00000010;
  }
  if (unaff_s2 == 1) {
    _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  }
  else {
    _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff | 0x10000;
  }
  first_stage_sfspi_transfer_setup(unaff_s3,unaff_s4);
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf9ffffff;
  sVar1 = FUN_bfe01180();
  _DAT_a0000006 = sVar1 - unaff_s1;
  return;
}



/* ===== FUNCTION FUN_bfe013a4 @ bfe013a4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0xbfe013c0) */

void FUN_bfe013a4(int param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  iVar1 = FUN_bfe01180();
  uVar3 = iVar1 + (param_1 + 999U) / 1000;
  do {
    uVar2 = FUN_bfe01180();
  } while (uVar2 < uVar3);
  return;
}



/* ===== FUNCTION sub_bfe013e4 @ bfe013e4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe013e4(void)

{
  return;
}



/* ===== FUNCTION FUN_bfe013e8 @ bfe013e8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe013e8(void)

{
  return;
}



/* ===== FUNCTION FUN_bfe013f0 @ bfe013f0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 FUN_bfe013f0(int param_1,uint param_2,undefined4 *param_3,uint *param_4)

{
  int iVar1;
  uint uVar2;
  undefined1 *puVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined1 *puVar7;
  
  iVar1 = 0x102;
  if (param_2 != 0) {
    iVar1 = param_2 + 0x106;
  }
  if ((*(ushort *)(iVar1 + param_1) & 1) == 0) {
    *param_4 = 0;
    return 0xffffffff;
  }
  uVar4 = (uint)*(ushort *)(param_1 + 0x108 + param_2);
  uVar2 = *param_4;
  if (uVar4 < *param_4) {
    uVar2 = uVar4;
  }
  *param_4 = uVar2;
  puVar6 = (undefined4 *)((param_2 >> 2) + param_1 + 0x20);
  while( true ) {
    puVar5 = param_3;
    if (((uint)param_3 & 3) == 0) {
      for (; (undefined1 *)0x3 < (undefined1 *)((int)param_3 + (uVar2 - (int)puVar5));
          puVar5 = puVar5 + 1) {
        *puVar5 = *puVar6;
      }
      puVar7 = (undefined1 *)((int)param_3 + (uVar2 & 0xfffffffc));
      puVar3 = puVar7 + (uVar2 - (uVar2 & 0xfffffffc));
      for (; puVar7 != puVar3; puVar7 = puVar7 + 1) {
        *puVar7 = *(undefined1 *)puVar6;
      }
      return 0;
    }
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    *(undefined1 *)param_3 = *(undefined1 *)puVar6;
    param_3 = (undefined4 *)((int)param_3 + 1);
  }
  return 0;
}



/* ===== FUNCTION sub_bfe01418 @ bfe01418 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_bfe01418(int param_1,uint param_2,undefined4 *param_3,uint *param_4)

{
  uint uVar1;
  undefined1 *puVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined1 *puVar6;
  
  uVar3 = (uint)*(ushort *)(param_1 + 0x108 + param_2);
  uVar1 = *param_4;
  if (uVar3 < *param_4) {
    uVar1 = uVar3;
  }
  *param_4 = uVar1;
  puVar5 = (undefined4 *)((param_2 >> 2) + param_1 + 0x20);
  while( true ) {
    puVar4 = param_3;
    if (((uint)param_3 & 3) == 0) {
      for (; (undefined1 *)0x3 < (undefined1 *)((int)param_3 + (uVar1 - (int)puVar4));
          puVar4 = puVar4 + 1) {
        *puVar4 = *puVar5;
      }
      puVar6 = (undefined1 *)((int)param_3 + (uVar1 & 0xfffffffc));
      puVar2 = puVar6 + (uVar1 - (uVar1 & 0xfffffffc));
      for (; puVar6 != puVar2; puVar6 = puVar6 + 1) {
        *puVar6 = *(undefined1 *)puVar5;
      }
      return 0;
    }
    if (uVar1 == 0) break;
    uVar1 = uVar1 - 1;
    *(undefined1 *)param_3 = *(undefined1 *)puVar5;
    param_3 = (undefined4 *)((int)param_3 + 1);
  }
  return 0;
}



/* ===== FUNCTION sub_bfe01468 @ bfe01468 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_bfe01468(undefined4 param_1,undefined4 *param_2,undefined4 *param_3)

{
  uint in_v0;
  undefined1 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  
  do {
    puVar3 = (undefined4 *)((int)param_3 + 1);
    in_v0 = in_v0 - 1;
    *(undefined1 *)param_3 = *(undefined1 *)param_2;
    puVar2 = puVar3;
    if (((uint)puVar3 & 3) == 0) {
      for (; (undefined1 *)0x3 < (undefined1 *)((int)puVar3 + (in_v0 - (int)puVar2));
          puVar2 = puVar2 + 1) {
        *puVar2 = *param_2;
      }
      puVar4 = (undefined1 *)((int)puVar3 + (in_v0 & 0xfffffffc));
      puVar1 = puVar4 + (in_v0 - (in_v0 & 0xfffffffc));
      for (; puVar4 != puVar1; puVar4 = puVar4 + 1) {
        *puVar4 = *(undefined1 *)param_2;
      }
      return 0;
    }
    param_3 = puVar3;
  } while (in_v0 != 0);
  return 0;
}



/* ===== FUNCTION FUN_bfe014d0 @ bfe014d0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 FUN_bfe014d0(int param_1,uint param_2,undefined4 *param_3,uint param_4)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  
  if (((param_2 == 0) + 1U & *(byte *)(param_1 + 0x102 + param_2)) != 0) {
    return 0xffffffff;
  }
  puVar2 = (undefined4 *)((param_2 >> 2) + param_1 + 0x20);
  while( true ) {
    puVar1 = param_3;
    if (((uint)param_3 & 3) == 0) {
      for (; (undefined1 *)0x3 < (undefined1 *)((int)param_3 + (param_4 - (int)puVar1));
          puVar1 = puVar1 + 1) {
        *puVar2 = *puVar1;
      }
      puVar3 = (undefined1 *)((int)param_3 + (param_4 & 0xfffffffc));
      puVar4 = puVar3;
      while (puVar4 != puVar3 + (param_4 - (param_4 & 0xfffffffc))) {
        *(undefined1 *)puVar2 = *puVar4;
        puVar4 = puVar4 + 1;
      }
      return 0;
    }
    if (param_4 == 0) break;
    param_4 = param_4 - 1;
    *(undefined1 *)puVar2 = *(undefined1 *)param_3;
    param_3 = (undefined4 *)((int)param_3 + 1);
  }
  return 0;
}



/* ===== FUNCTION sub_bfe01518 @ bfe01518 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

undefined4 sub_bfe01518(undefined4 param_1,undefined4 *param_2,undefined4 *param_3,uint param_4)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  do {
    param_4 = param_4 - 1;
    *(undefined1 *)param_2 = *(undefined1 *)((int)param_3 + -1);
    puVar1 = param_3;
    if (((uint)param_3 & 3) == 0) {
      for (; 3 < (int)param_3 + (param_4 - (int)puVar1); puVar1 = puVar1 + 1) {
        *param_2 = *puVar1;
      }
      puVar2 = (undefined1 *)((int)param_3 + (param_4 & 0xfffffffc));
      puVar3 = puVar2;
      while (puVar3 != puVar2 + (param_4 - (param_4 & 0xfffffffc))) {
        *(undefined1 *)param_2 = *puVar3;
        puVar3 = puVar3 + 1;
      }
      return 0;
    }
    param_3 = (undefined4 *)((int)param_3 + 1);
  } while (param_4 != 0);
  return 0;
}



/* ===== FUNCTION FUN_bfe01588 @ bfe01588 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe01588(void)

{
  _DAT_b8800084 = _DAT_b8800084 | 0x20000000;
  FUN_bfe013a4(900);
  _DAT_b8800084 = _DAT_b8800084 & 0xdfffffff;
  return;
}



/* ===== FUNCTION sub_bfe015d0 @ bfe015d0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe015d0(int param_1)

{
  if (param_1 == -0x477bc000) {
    _DAT_b8800080 = _DAT_b8800080 | 0x10000000;
    FUN_bfe013a4(900);
    _DAT_b8800080 = _DAT_b8800080 & 0xefffffff;
    return;
  }
  if (param_1 == -0x477b0000) {
    FUN_bfe01588();
    return;
  }
  return;
}



/* ===== FUNCTION first_stage_peripheral_gate_for_usb_blocks @ bfe015d4 =====
 * Medium confidence: compares the argument with 0xb8844000 and 0xb8850000, matching the DTS USB host controller MMIO windows, then toggles bits in 0xb8800080.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Medium confidence: compares the argument with 0xb8844000 and 0xb8850000, matching the DTS USB
   host controller MMIO windows, then toggles bits in 0xb8800080. */

void first_stage_peripheral_gate_for_usb_blocks(int param_1)

{
  if (param_1 == -0x477bc000) {
    _DAT_b8800080 = _DAT_b8800080 | 0x10000000;
    FUN_bfe013a4(900);
    _DAT_b8800080 = _DAT_b8800080 & 0xefffffff;
    return;
  }
  if (param_1 == -0x477b0000) {
    FUN_bfe01588();
    return;
  }
  return;
}



/* ===== FUNCTION sub_bfe015e4 @ bfe015e4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe015e4(void)

{
  _DAT_b8800080 = _DAT_b8800080 | 0x10000000;
  FUN_bfe013a4(900);
  _DAT_b8800080 = _DAT_b8800080 & 0xefffffff;
  return;
}



/* ===== FUNCTION sub_bfe0162c @ bfe0162c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe0162c(int param_1)

{
  int in_v0;
  
  if (param_1 == in_v0) {
    FUN_bfe01588();
    return;
  }
  return;
}



/* ===== FUNCTION sub_bfe01630 @ bfe01630 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01630(int param_1)

{
  int in_v0;
  
  if (param_1 == in_v0) {
    FUN_bfe01588();
    return;
  }
  return;
}



/* ===== FUNCTION first_stage_peripheral_reset_sequence @ bfe01648 =====
 * Medium confidence: writes several control/status registers in the selected MMIO window after gating the peripheral clock/reset.
 */

/* Medium confidence: writes several control/status registers in the selected MMIO window after
   gating the peripheral clock/reset. */

void first_stage_peripheral_reset_sequence(int param_1)

{
  *(ushort *)(param_1 + 0x1020) = *(ushort *)(param_1 + 0x1020) | 0x10c0;
  *(uint *)(param_1 + 0x380) = *(uint *)(param_1 + 0x380) & 0xffffff6f | 0x11;
  FUN_bfe013a4(900);
  *(uint *)(param_1 + 0x380) = *(uint *)(param_1 + 0x380) & 0xfffffffe;
  return;
}



/* ===== FUNCTION sub_bfe01650 @ bfe01650 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01650(int param_1)

{
  ushort in_v0;
  uint in_v1;
  
  *(ushort *)(param_1 + 0x1020) = in_v0 | 0x10c0;
  *(uint *)(param_1 + 0x380) = in_v1 & 0xffffff6f | 0x11;
  FUN_bfe013a4(900);
  *(uint *)(param_1 + 0x380) = *(uint *)(param_1 + 0x380) & 0xfffffffe;
  return;
}



/* ===== FUNCTION sub_bfe0169c @ bfe0169c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe0169c(int *param_1)

{
  uint uVar1;
  
  if ((((_DAT_b8818a00 & 0x40000000) != 0) || (param_1[1] != -0x477bc000)) &&
     (((int)_DAT_b8818a00 < 0 || (param_1[1] != -0x477b0000)))) {
    param_1[8] = 1;
    param_1[9] = 0x40;
    param_1[0xb] = 0xff;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[0xc] = 0;
    param_1[0xe] = 0;
    param_1[0xf] = 0;
    param_1[7] = 0;
    param_1[10] = 0;
    if (*param_1 == 0) {
      uVar1 = 0xfeff0000;
    }
    else {
      uVar1 = 0xfdff0000;
    }
    _DAT_b8800064 = _DAT_b8800064 & (uVar1 | 0xffff);
    first_stage_peripheral_reset_sequence(param_1[1]);
    first_stage_peripheral_gate_for_usb_blocks(param_1[1]);
    first_stage_peripheral_reset_sequence(param_1[1]);
    *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) & 0xbf;
    FUN_bfe013a4(1000);
    *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) | 0x40;
    return;
  }
  return;
}



/* ===== FUNCTION FUN_bfe016a0 @ bfe016a0 =====
 * References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe016a0(int *param_1)

{
  uint uVar1;
  
  if ((((_DAT_b8818a00 & 0x40000000) != 0) || (param_1[1] != -0x477bc000)) &&
     (((int)_DAT_b8818a00 < 0 || (param_1[1] != -0x477b0000)))) {
    param_1[8] = 1;
    param_1[9] = 0x40;
    param_1[0xb] = 0xff;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[0xc] = 0;
    param_1[0xe] = 0;
    param_1[0xf] = 0;
    param_1[7] = 0;
    param_1[10] = 0;
    if (*param_1 == 0) {
      uVar1 = 0xfeff0000;
    }
    else {
      uVar1 = 0xfdff0000;
    }
    _DAT_b8800064 = _DAT_b8800064 & (uVar1 | 0xffff);
    first_stage_peripheral_reset_sequence(param_1[1]);
    first_stage_peripheral_gate_for_usb_blocks(param_1[1]);
    first_stage_peripheral_reset_sequence(param_1[1]);
    *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) & 0xbf;
    FUN_bfe013a4(1000);
    *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) | 0x40;
    return;
  }
  return;
}



/* ===== FUNCTION sub_bfe016f0 @ bfe016f0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe016f0(int *param_1)

{
  uint uVar1;
  
  param_1[8] = 1;
  param_1[9] = 0x40;
  param_1[0xb] = 0xff;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[0xc] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[7] = 0;
  param_1[10] = 0;
  if (*param_1 == 0) {
    uVar1 = 0xfeff0000;
  }
  else {
    uVar1 = 0xfdff0000;
  }
  _DAT_b8800064 = _DAT_b8800064 & (uVar1 | 0xffff);
  first_stage_peripheral_reset_sequence(param_1[1]);
  first_stage_peripheral_gate_for_usb_blocks(param_1[1]);
  first_stage_peripheral_reset_sequence(param_1[1]);
  *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) & 0xbf;
  FUN_bfe013a4(1000);
  *(byte *)(param_1[1] + 1) = *(byte *)(param_1[1] + 1) | 0x40;
  return;
}



/* ===== FUNCTION sub_bfe017a8 @ bfe017a8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe017a8(int param_1,int param_2)

{
  *(uint *)(param_2 + 100) = *(uint *)(param_2 + 100) & 0xfdffffff;
  first_stage_peripheral_reset_sequence(*(undefined4 *)(param_1 + 4));
  first_stage_peripheral_gate_for_usb_blocks(*(undefined4 *)(param_1 + 4));
  first_stage_peripheral_reset_sequence(*(undefined4 *)(param_1 + 4));
  *(byte *)(*(int *)(param_1 + 4) + 1) = *(byte *)(*(int *)(param_1 + 4) + 1) & 0xbf;
  FUN_bfe013a4(1000);
  *(byte *)(*(int *)(param_1 + 4) + 1) = *(byte *)(*(int *)(param_1 + 4) + 1) | 0x40;
  return;
}



/* ===== FUNCTION sub_bfe017ac @ bfe017ac =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe017ac(int param_1,int param_2)

{
  *(uint *)(param_2 + 100) = *(uint *)(param_2 + 100) & 0xfdffffff;
  first_stage_peripheral_reset_sequence(*(undefined4 *)(param_1 + 4));
  first_stage_peripheral_gate_for_usb_blocks(*(undefined4 *)(param_1 + 4));
  first_stage_peripheral_reset_sequence(*(undefined4 *)(param_1 + 4));
  *(byte *)(*(int *)(param_1 + 4) + 1) = *(byte *)(*(int *)(param_1 + 4) + 1) & 0xbf;
  FUN_bfe013a4(1000);
  *(byte *)(*(int *)(param_1 + 4) + 1) = *(byte *)(*(int *)(param_1 + 4) + 1) | 0x40;
  return;
}



/* ===== FUNCTION FUN_bfe017c0 @ bfe017c0 =====
 * References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
 */

void FUN_bfe017c0(void)

{
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_d8;
  undefined4 local_d4;
  
  local_1a4 = 0xb8844000;
  local_d4 = 0xb8850000;
  local_d8 = 1;
  local_1a8 = 0;
  FUN_bfe016a0(&local_1a8);
  FUN_bfe016a0(&local_d8);
  return;
}



/* ===== FUNCTION sub_bfe017c4 @ bfe017c4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe017c4(void)

{
  int in_v0;
  undefined4 local_1a8;
  int local_1a4;
  undefined4 local_d8;
  undefined4 local_d4;
  
  local_1a4 = in_v0 + 0x4000;
  local_d4 = 0xb8850000;
  local_d8 = 1;
  local_1a8 = 0;
  FUN_bfe016a0(&local_1a8);
  FUN_bfe016a0(&local_d8);
  return;
}



/* ===== FUNCTION sub_bfe01800 @ bfe01800 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe01800(int param_1)

{
  if ((((_DAT_b8818a00 & 0x40000000) != 0) || (*(int *)(param_1 + 4) != -0x477bc000)) &&
     (((int)_DAT_b8818a00 < 0 || (*(int *)(param_1 + 4) != -0x477b0000)))) {
    *(undefined4 *)(param_1 + 0x20) = 1;
    *(undefined4 *)(param_1 + 0x24) = 0x40;
    *(undefined4 *)(param_1 + 0x2c) = 0xff;
    *(undefined4 *)(param_1 + 0xc) = 0;
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
    first_stage_peripheral_gate_for_usb_blocks();
    FUN_bfe02ba4(param_1,0);
    FUN_bfe02ba4(param_1,1);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_bfe01804 @ bfe01804 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe01804(int param_1)

{
  if ((((_DAT_b8818a00 & 0x40000000) != 0) || (*(int *)(param_1 + 4) != -0x477bc000)) &&
     (((int)_DAT_b8818a00 < 0 || (*(int *)(param_1 + 4) != -0x477b0000)))) {
    *(undefined4 *)(param_1 + 0x20) = 1;
    *(undefined4 *)(param_1 + 0x24) = 0x40;
    *(undefined4 *)(param_1 + 0x2c) = 0xff;
    *(undefined4 *)(param_1 + 0xc) = 0;
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
    first_stage_peripheral_gate_for_usb_blocks();
    FUN_bfe02ba4(param_1,0);
    FUN_bfe02ba4(param_1,1);
    return;
  }
  return;
}



/* ===== FUNCTION sub_bfe01814 @ bfe01814 =====
 * References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe01814(int param_1)

{
  uint in_v0;
  uint in_v1;
  
  if ((((in_v0 & in_v1) != 0) || (*(int *)(param_1 + 4) != -0x477bc000)) &&
     ((_DAT_b8818a00 < 0 || (*(int *)(param_1 + 4) != -0x477b0000)))) {
    *(undefined4 *)(param_1 + 0x20) = 1;
    *(undefined4 *)(param_1 + 0x24) = 0x40;
    *(undefined4 *)(param_1 + 0x2c) = 0xff;
    *(undefined4 *)(param_1 + 0xc) = 0;
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x38) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x28) = 0;
    first_stage_peripheral_gate_for_usb_blocks();
    FUN_bfe02ba4(param_1,0);
    FUN_bfe02ba4(param_1,1);
    return;
  }
  return;
}



/* ===== FUNCTION sub_bfe018b8 @ bfe018b8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe018b8(void)

{
  return;
}



/* ===== FUNCTION sub_bfe018c4 @ bfe018c4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe018c4(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined1 uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar5 = *(uint *)(param_1 + 0xb8);
  *(undefined4 *)(param_1 + 0x38) = 1;
  uVar4 = 0x40;
  if (uVar5 < 0x41) {
    uVar4 = uVar5;
  }
  *(uint *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - uVar4;
  *(uint *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + uVar4;
  FUN_bfe014d0(*(undefined4 *)(param_1 + 4),0);
  iVar2 = *(int *)(param_1 + 4);
  if (uVar5 < 0x40) {
    *(undefined4 *)(param_1 + 0x38) = 4;
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 10;
  }
  else {
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 2;
  }
  if ((bVar1 & 2) == 0) {
    *(undefined1 *)(iVar2 + 0x102) = uVar3;
  }
  return;
}



/* ===== FUNCTION FUN_bfe018c8 @ bfe018c8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe018c8(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined1 uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar5 = *(uint *)(param_1 + 0xb8);
  *(undefined4 *)(param_1 + 0x38) = 1;
  uVar4 = 0x40;
  if (uVar5 < 0x41) {
    uVar4 = uVar5;
  }
  *(uint *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - uVar4;
  *(uint *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + uVar4;
  FUN_bfe014d0(*(undefined4 *)(param_1 + 4),0);
  iVar2 = *(int *)(param_1 + 4);
  if (uVar5 < 0x40) {
    *(undefined4 *)(param_1 + 0x38) = 4;
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 10;
  }
  else {
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 2;
  }
  if ((bVar1 & 2) == 0) {
    *(undefined1 *)(iVar2 + 0x102) = uVar3;
  }
  return;
}



/* ===== FUNCTION sub_bfe018cc @ bfe018cc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe018cc(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined1 uVar3;
  uint uVar4;
  uint in_t1;
  
  *(undefined4 *)(param_1 + 0x38) = 1;
  uVar4 = 0x40;
  if (in_t1 < 0x41) {
    uVar4 = in_t1;
  }
  *(uint *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - uVar4;
  *(uint *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + uVar4;
  FUN_bfe014d0(*(undefined4 *)(param_1 + 4),0);
  iVar2 = *(int *)(param_1 + 4);
  if (in_t1 < 0x40) {
    *(undefined4 *)(param_1 + 0x38) = 4;
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 10;
  }
  else {
    bVar1 = *(byte *)(iVar2 + 0x102);
    uVar3 = 2;
  }
  if ((bVar1 & 2) == 0) {
    *(undefined1 *)(iVar2 + 0x102) = uVar3;
  }
  return;
}



/* ===== FUNCTION sub_bfe0193c @ bfe0193c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe0193c(void)

{
  int in_v0;
  int in_t0;
  
  *(undefined4 *)(in_t0 + 0x38) = 4;
  if ((*(byte *)(in_v0 + 0x102) & 2) == 0) {
    *(undefined1 *)(in_v0 + 0x102) = 10;
  }
  return;
}



/* ===== FUNCTION sub_bfe01940 @ bfe01940 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01940(void)

{
  int in_v0;
  int in_t0;
  
  *(undefined4 *)(in_t0 + 0x38) = 4;
  if ((*(byte *)(in_v0 + 0x102) & 2) == 0) {
    *(undefined1 *)(in_v0 + 0x102) = 10;
  }
  return;
}



/* ===== FUNCTION FUN_bfe01960 @ bfe01960 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe01960(int param_1)

{
  if ((*(int *)(param_1 + 0x38) == 0) && (*(int *)(param_1 + 0x30) != 1)) {
    if (*(int *)(param_1 + 0x20) == 1) {
      *(undefined4 *)(param_1 + 0x38) = 1;
      *(undefined4 *)(param_1 + 0xb4) = *(undefined4 *)(param_1 + 0x10);
      *(undefined4 *)(param_1 + 0xb8) = *(undefined4 *)(param_1 + 0x14);
      FUN_bfe018c8();
      return;
    }
    if (*(int *)(param_1 + 0x20) == 0) {
      *(undefined4 *)(param_1 + 0x38) = 3;
      *(undefined4 *)(param_1 + 0xb4) = *(undefined4 *)(param_1 + 0x10);
      *(undefined4 *)(param_1 + 0xb8) = *(undefined4 *)(param_1 + 0x14);
    }
  }
  return;
}



/* ===== FUNCTION sub_bfe019c4 @ bfe019c4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe019c4(int param_1,int param_2)

{
  if ((*(int *)(param_2 + 0x14) == 1) && (*(int *)(param_1 + 0x30) != 1)) {
    *(undefined4 *)(param_1 + 0x30) = 1;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x20) = 0;
    *(code **)(param_1 + 0x34) = sub_bfe019c4;
    FUN_bfe01960();
    return;
  }
  return;
}



/* ===== FUNCTION FUN_bfe01a18 @ bfe01a18 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe01a18(int param_1)

{
  undefined1 uVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  
  cVar2 = DAT_bfe00273;
  uVar1 = DAT_bfe00272;
  if ((DAT_bfe00270 & 0x60) != 0) {
    return;
  }
  if (DAT_bfe00271 == 5) {
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x30) = 1;
    *(undefined4 *)(param_1 + 0x20) = 1;
    *(code **)(param_1 + 0x34) = sub_bfe019c4;
    *(uint *)(param_1 + 0xc) = (uint)CONCAT11(cVar2,uVar1);
    *(uint *)(param_1 + 0xb0) = (uint)CONCAT11(cVar2,uVar1);
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    FUN_bfe01960();
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 0xb0) | 0x80000000;
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    *(undefined4 *)(param_1 + 0x38) = 4;
    return;
  }
  if (DAT_bfe00271 < 6) {
    if (DAT_bfe00271 != 0) {
      return;
    }
    *(undefined2 *)(param_1 + 0xbc) = 0;
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(int *)(param_1 + 0x10) = param_1 + 0xbc;
    *(undefined4 *)(param_1 + 0x14) = 2;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  else {
    if (DAT_bfe00271 != 6) {
      uVar4 = 1;
      if (DAT_bfe00271 != 9) {
        return;
      }
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined4 *)(param_1 + 0x10) = 0;
      *(undefined4 *)(param_1 + 0x14) = 0;
      *(undefined4 *)(param_1 + 0x30) = 1;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
      iVar3 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar3 + 0x110) = 0x200;
      *(undefined1 *)(iVar3 + 0x113) = 0;
      *(undefined1 *)(iVar3 + 0x112) = 0x40;
      iVar3 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar3 + 0x124) = 0x200;
      *(undefined1 *)(iVar3 + 0x127) = 0;
      *(undefined1 *)(iVar3 + 0x126) = 0x80;
      iVar3 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar3 + 0xe);
      *(undefined1 *)(iVar3 + 0xe) = 1;
      *(undefined1 *)(iVar3 + 0x62) = 6;
      *(undefined1 *)(iVar3 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar3 + 0xe);
      *(undefined1 *)(iVar3 + 0xe) = 1;
      *(undefined2 *)(iVar3 + 100) = 8;
      *(undefined1 *)(iVar3 + 0xe) = uVar1;
      iVar3 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar3 + 0xe);
      *(undefined1 *)(iVar3 + 0xe) = 2;
      *(undefined1 *)(iVar3 + 99) = 6;
      *(undefined1 *)(iVar3 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar3 + 0xe);
      *(undefined1 *)(iVar3 + 0xe) = 2;
      *(undefined2 *)(iVar3 + 0x66) = 0x48;
      *(undefined1 *)(iVar3 + 0xe) = uVar1;
      FUN_bfe01960();
      *(undefined4 *)(param_1 + 0xa0) = 0x30000001;
      *(undefined4 *)(param_1 + 0x90) = 2;
      *(undefined4 *)(param_1 + 0x5c) = uVar4;
      return;
    }
    if (DAT_bfe00273 != '\x01') {
      if (DAT_bfe00273 != '\x02') {
        return;
      }
      *(undefined4 *)(param_1 + 0x30) = 0;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined **)(param_1 + 0x10) = &DAT_bfe002b0;
      *(undefined4 *)(param_1 + 0x14) = 0x20;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 1;
      goto LAB_bfe01b9c;
    }
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002d0;
    *(undefined4 *)(param_1 + 0x14) = 0x12;
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  *(undefined4 *)(param_1 + 0x20) = 1;
  *(code **)(param_1 + 0x34) = sub_bfe019c4;
LAB_bfe01b9c:
  FUN_bfe01960();
  return;
}



/* ===== FUNCTION sub_bfe01a30 @ bfe01a30 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01a30(int param_1)

{
  byte bVar1;
  undefined1 uVar2;
  ushort uVar3;
  int in_v0;
  int iVar4;
  undefined4 uVar5;
  
  bVar1 = *(byte *)(in_v0 + 0x271);
  if (bVar1 == 5) {
    uVar3 = *(ushort *)(in_v0 + 0x272);
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x30) = 1;
    *(undefined4 *)(param_1 + 0x20) = 1;
    *(code **)(param_1 + 0x34) = sub_bfe019c4;
    *(uint *)(param_1 + 0xc) = (uint)uVar3;
    *(uint *)(param_1 + 0xb0) = (uint)uVar3;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    FUN_bfe01960();
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 0xb0) | 0x80000000;
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    *(undefined4 *)(param_1 + 0x38) = 4;
    return;
  }
  if (bVar1 < 6) {
    if (bVar1 != 0) {
      return;
    }
    *(undefined2 *)(param_1 + 0xbc) = 0;
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(int *)(param_1 + 0x10) = param_1 + 0xbc;
    *(undefined4 *)(param_1 + 0x14) = 2;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  else {
    if (bVar1 != 6) {
      uVar5 = 1;
      if (bVar1 != 9) {
        return;
      }
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined4 *)(param_1 + 0x10) = 0;
      *(undefined4 *)(param_1 + 0x14) = 0;
      *(undefined4 *)(param_1 + 0x30) = 1;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
      iVar4 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar4 + 0x110) = 0x200;
      *(undefined1 *)(iVar4 + 0x113) = 0;
      *(undefined1 *)(iVar4 + 0x112) = 0x40;
      iVar4 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar4 + 0x124) = 0x200;
      *(undefined1 *)(iVar4 + 0x127) = 0;
      *(undefined1 *)(iVar4 + 0x126) = 0x80;
      iVar4 = *(int *)(param_1 + 4);
      uVar2 = *(undefined1 *)(iVar4 + 0xe);
      *(undefined1 *)(iVar4 + 0xe) = 1;
      *(undefined1 *)(iVar4 + 0x62) = 6;
      *(undefined1 *)(iVar4 + 0xe) = uVar2;
      uVar2 = *(undefined1 *)(iVar4 + 0xe);
      *(undefined1 *)(iVar4 + 0xe) = 1;
      *(undefined2 *)(iVar4 + 100) = 8;
      *(undefined1 *)(iVar4 + 0xe) = uVar2;
      iVar4 = *(int *)(param_1 + 4);
      uVar2 = *(undefined1 *)(iVar4 + 0xe);
      *(undefined1 *)(iVar4 + 0xe) = 2;
      *(undefined1 *)(iVar4 + 99) = 6;
      *(undefined1 *)(iVar4 + 0xe) = uVar2;
      uVar2 = *(undefined1 *)(iVar4 + 0xe);
      *(undefined1 *)(iVar4 + 0xe) = 2;
      *(undefined2 *)(iVar4 + 0x66) = 0x48;
      *(undefined1 *)(iVar4 + 0xe) = uVar2;
      FUN_bfe01960();
      *(undefined4 *)(param_1 + 0xa0) = 0x30000001;
      *(undefined4 *)(param_1 + 0x90) = 2;
      *(undefined4 *)(param_1 + 0x5c) = uVar5;
      return;
    }
    if (*(char *)(in_v0 + 0x273) != '\x01') {
      if (*(char *)(in_v0 + 0x273) != '\x02') {
        return;
      }
      *(undefined4 *)(param_1 + 0x30) = 0;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined **)(param_1 + 0x10) = &DAT_bfe002b0;
      *(undefined4 *)(param_1 + 0x14) = 0x20;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 1;
      goto LAB_bfe01b9c;
    }
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002d0;
    *(undefined4 *)(param_1 + 0x14) = 0x12;
    *(undefined4 *)(param_1 + 0x1c) = 0;
  }
  *(undefined4 *)(param_1 + 0x20) = 1;
  *(code **)(param_1 + 0x34) = sub_bfe019c4;
LAB_bfe01b9c:
  FUN_bfe01960();
  return;
}



/* ===== FUNCTION sub_bfe01a64 @ bfe01a64 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01a64(int param_1)

{
  undefined1 uVar1;
  int in_v0;
  int iVar2;
  int in_v1;
  int in_t2;
  undefined4 uVar3;
  
  if (in_v1 != 6) {
    uVar3 = 1;
    if (in_v1 == 9) {
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined4 *)(param_1 + 0x10) = 0;
      *(undefined4 *)(param_1 + 0x14) = 0;
      *(undefined4 *)(param_1 + 0x30) = 1;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
      iVar2 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar2 + 0x110) = 0x200;
      *(undefined1 *)(iVar2 + 0x113) = 0;
      *(undefined1 *)(iVar2 + 0x112) = 0x40;
      iVar2 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar2 + 0x124) = 0x200;
      *(undefined1 *)(iVar2 + 0x127) = 0;
      *(undefined1 *)(iVar2 + 0x126) = 0x80;
      iVar2 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 1;
      *(undefined1 *)(iVar2 + 0x62) = 6;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 1;
      *(undefined2 *)(iVar2 + 100) = 8;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      iVar2 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 2;
      *(undefined1 *)(iVar2 + 99) = 6;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 2;
      *(undefined2 *)(iVar2 + 0x66) = 0x48;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      FUN_bfe01960();
      *(undefined4 *)(in_t2 + 0xa0) = 0x30000001;
      *(undefined4 *)(in_t2 + 0x90) = 2;
      *(undefined4 *)(in_t2 + 0x5c) = uVar3;
    }
    return;
  }
  if (*(char *)(in_v0 + 0x273) == '\x01') {
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002d0;
    *(undefined4 *)(param_1 + 0x14) = 0x12;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(in_t2 + 0x20) = 1;
    *(code **)(in_t2 + 0x34) = sub_bfe019c4;
  }
  else {
    if (*(char *)(in_v0 + 0x273) != '\x02') {
      return;
    }
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002b0;
    *(undefined4 *)(param_1 + 0x14) = 0x20;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x20) = 1;
  }
  FUN_bfe01960();
  return;
}



/* ===== FUNCTION sub_bfe01a68 @ bfe01a68 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01a68(int param_1)

{
  undefined1 uVar1;
  int in_v0;
  int iVar2;
  int in_v1;
  int in_t2;
  undefined4 uVar3;
  
  if (in_v1 != 6) {
    uVar3 = 1;
    if (in_v1 == 9) {
      *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
      *(undefined4 *)(param_1 + 0x10) = 0;
      *(undefined4 *)(param_1 + 0x14) = 0;
      *(undefined4 *)(param_1 + 0x30) = 1;
      *(undefined4 *)(param_1 + 0x3c) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 0;
      *(undefined4 *)(param_1 + 0x20) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
      iVar2 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar2 + 0x110) = 0x200;
      *(undefined1 *)(iVar2 + 0x113) = 0;
      *(undefined1 *)(iVar2 + 0x112) = 0x40;
      iVar2 = *(int *)(param_1 + 4);
      *(undefined2 *)(iVar2 + 0x124) = 0x200;
      *(undefined1 *)(iVar2 + 0x127) = 0;
      *(undefined1 *)(iVar2 + 0x126) = 0x80;
      iVar2 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 1;
      *(undefined1 *)(iVar2 + 0x62) = 6;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 1;
      *(undefined2 *)(iVar2 + 100) = 8;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      iVar2 = *(int *)(param_1 + 4);
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 2;
      *(undefined1 *)(iVar2 + 99) = 6;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      uVar1 = *(undefined1 *)(iVar2 + 0xe);
      *(undefined1 *)(iVar2 + 0xe) = 2;
      *(undefined2 *)(iVar2 + 0x66) = 0x48;
      *(undefined1 *)(iVar2 + 0xe) = uVar1;
      FUN_bfe01960();
      *(undefined4 *)(in_t2 + 0xa0) = 0x30000001;
      *(undefined4 *)(in_t2 + 0x90) = 2;
      *(undefined4 *)(in_t2 + 0x5c) = uVar3;
    }
    return;
  }
  if (*(char *)(in_v0 + 0x273) == '\x01') {
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002d0;
    *(undefined4 *)(param_1 + 0x14) = 0x12;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(in_t2 + 0x20) = 1;
    *(code **)(in_t2 + 0x34) = sub_bfe019c4;
  }
  else {
    if (*(char *)(in_v0 + 0x273) != '\x02') {
      return;
    }
    *(undefined4 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined **)(param_1 + 0x10) = &DAT_bfe002b0;
    *(undefined4 *)(param_1 + 0x14) = 0x20;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x20) = 1;
  }
  FUN_bfe01960();
  return;
}



/* ===== FUNCTION sub_bfe01ba4 @ bfe01ba4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01ba4(int param_1)

{
  ushort uVar1;
  int in_v0;
  int in_t2;
  
  uVar1 = *(ushort *)(in_v0 + 0x272);
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  *(undefined4 *)(param_1 + 0x30) = 1;
  *(undefined4 *)(param_1 + 0x20) = 1;
  *(code **)(param_1 + 0x34) = sub_bfe019c4;
  *(uint *)(param_1 + 0xc) = (uint)uVar1;
  *(uint *)(param_1 + 0xb0) = (uint)uVar1;
  *(undefined4 *)(param_1 + 0x3c) = 0;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  FUN_bfe01960();
  *(uint *)(in_t2 + 8) = *(uint *)(in_t2 + 0xb0) | 0x80000000;
  *(undefined1 *)(*(int *)(in_t2 + 4) + 0x102) = 0x48;
  *(undefined4 *)(in_t2 + 0x38) = 4;
  return;
}



/* ===== FUNCTION FUN_bfe01c90 @ bfe01c90 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe01c90(int param_1)

{
  _DAT_b8818500 = -((uint)(param_1 * 0x1b) >> 7);
  _DAT_b8818504 = 0x26;
  return;
}



/* ===== FUNCTION FUN_bfe01cc4 @ bfe01cc4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe01cc4(void)

{
  _DAT_b8818504 = 0;
  return;
}



/* ===== FUNCTION FUN_bfe01cd8 @ bfe01cd8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe01cd8(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6)

{
  byte bVar1;
  byte *pbVar2;
  
  *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_1 + 0xb0);
  *(undefined4 *)(param_1 + 0x6c) = 1;
  *(undefined4 *)(param_1 + 0x54) = param_4;
  *(undefined4 *)(param_1 + 0x50) = param_6;
  *(int *)(param_1 + 0x5c) = param_2;
  *(undefined4 *)(param_1 + 0x44) = param_3;
  *(undefined4 *)(param_1 + 0x48) = param_5;
  *(undefined4 *)(param_1 + 100) = 0;
  *(undefined4 *)(param_1 + 0x70) = 0;
  param_2 = param_2 << 4;
  FUN_bfe014d0(*(undefined4 *)(param_1 + 4),param_2,param_3,param_5);
  pbVar2 = (byte *)(param_2 + 0x102 + *(int *)(param_1 + 4));
  bVar1 = (param_2 == 0) + 1;
  if ((bVar1 & *pbVar2) == 0) {
    *pbVar2 = bVar1;
  }
  return;
}



/* ===== FUNCTION sub_bfe01cdc @ bfe01cdc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe01cdc(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6)

{
  byte bVar1;
  undefined4 in_v1;
  byte *pbVar2;
  
  *(undefined4 *)(param_1 + 0x40) = in_v1;
  *(undefined4 *)(param_1 + 0x6c) = 1;
  *(undefined4 *)(param_1 + 0x54) = param_4;
  *(undefined4 *)(param_1 + 0x50) = param_6;
  *(int *)(param_1 + 0x5c) = param_2;
  *(undefined4 *)(param_1 + 0x44) = param_3;
  *(undefined4 *)(param_1 + 0x48) = param_5;
  *(undefined4 *)(param_1 + 100) = 0;
  *(undefined4 *)(param_1 + 0x70) = 0;
  param_2 = param_2 << 4;
  FUN_bfe014d0(*(undefined4 *)(param_1 + 4),param_2,param_3,param_5);
  pbVar2 = (byte *)(param_2 + 0x102 + *(int *)(param_1 + 4));
  bVar1 = (param_2 == 0) + 1;
  if ((bVar1 & *pbVar2) == 0) {
    *pbVar2 = bVar1;
  }
  return;
}



/* ===== FUNCTION sub_bfe01d5c @ bfe01d5c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe01d5c(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  int iVar7;
  
  if ((*(uint *)(param_1 + 0xa8) & 4) != 0) {
    return 0;
  }
  if ((*(uint *)(param_1 + 0xa8) & 0x20) != 0) {
    *(undefined4 *)(param_1 + 0x38) = 0;
    return 0;
  }
  if ((*(uint *)(param_1 + 0xac) & 1) == 0) goto switchD_bfe01ddc_caseD_2;
  iVar5 = *(int *)(param_1 + 4);
  uVar1 = *(ushort *)(iVar5 + 0x102);
  switch(*(undefined4 *)(param_1 + 0x38)) {
  case 0:
    if ((uVar1 & 1) != 0) {
LAB_bfe01e04:
      iVar7 = 0x40;
      FUN_bfe013f0(iVar5,0,&DAT_bfe00270,&stack0x00000000);
      if (iVar7 != 0) {
        FUN_bfe01a18(param_1);
      }
    }
    break;
  case 1:
    FUN_bfe018c8(param_1);
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 3:
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      iVar7 = *(int *)(param_1 + 0xb8);
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    else {
      iVar7 = 0x40;
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    FUN_bfe013f0(iVar5,0,uVar2,&stack0x00000000);
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      (**(code **)(param_1 + 0x34))();
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    }
    else {
      (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    }
    *(int *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + iVar7;
    *(int *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - iVar7;
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 4:
    (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    *(undefined4 *)(param_1 + 0x38) = 0;
    if ((int)*(uint *)(param_1 + 8) < 0) {
      uVar3 = *(uint *)(param_1 + 8) & 0x7fffffff;
      *(uint *)(param_1 + 8) = uVar3;
      **(undefined1 **)(param_1 + 4) = (char)uVar3;
    }
    if ((uVar1 & 1) != 0) {
      iVar5 = *(int *)(param_1 + 4);
      goto LAB_bfe01e04;
    }
    break;
  case 5:
    if ((uVar1 & 4) != 0) {
      *(undefined4 *)(param_1 + 0x38) = 0;
    }
  }
switchD_bfe01ddc_caseD_2:
  uVar3 = *(uint *)(param_1 + 0xac);
LAB_bfe01e24:
  if ((uVar3 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe;
  if ((uVar3 & 0xfffe) != 0) {
    iVar5 = *(int *)(param_1 + 0x6c);
    if (iVar5 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar5 < 0x10000005) {
      if (iVar5 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar5 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar5 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar5 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe0000;
  if ((uVar3 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar5 = *(int *)(param_1 + 0x90) << 4;
  if (*(int *)(param_1 + 0xa0) != 0x30000001) {
    if (*(int *)(param_1 + 0xa0) != 0x30000002) {
      return 0;
    }
    iVar7 = 0x200;
    FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,*(undefined4 *)(param_1 + 0x78),&stack0x00000000
                );
    if (iVar5 == 0) {
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
      *pbVar6 = *pbVar6 & 0xfe;
    }
    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + iVar7;
    uVar3 = iVar7 + *(int *)(param_1 + 0xa4);
    *(uint *)(param_1 + 0xa4) = uVar3;
    uVar4 = 0;
    if (uVar3 < *(uint *)(param_1 + 0x7c)) {
      *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar3 = 0;
    for (; *(uint *)(param_1 + 200) >> 2 != uVar4; uVar4 = uVar4 + 1) {
      uVar3 = uVar3 ^ *(uint *)(uVar4 * 4 + *(int *)(param_1 + 0xc4));
    }
    if (*(uint *)(param_1 + 0xcc) != uVar3) {
      *(uint *)(param_1 + 0xcc) = uVar3;
    }
    iVar5 = param_1 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(param_1 + 0x78) = param_1 + 0xc0;
  iVar7 = 0x200;
  FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,param_1 + 0xc0,&stack0x00000000);
  if (iVar5 == 0) {
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  else {
    pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
    *pbVar6 = *pbVar6 & 0xfe;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  if (iVar5 == 0x1991a0a1) {
    *(undefined4 *)(param_1 + 0x7c) = *(undefined4 *)(param_1 + 200);
    *(undefined4 *)(param_1 + 0x84) = 2;
    *(undefined4 *)(param_1 + 0x90) = 2;
    *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
    *(undefined4 *)(param_1 + 0x74) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined4 *)(param_1 + 0x78) = *(undefined4 *)(param_1 + 0xc4);
    *(undefined4 *)(param_1 + 0x98) = 0;
    *(undefined4 *)(param_1 + 0xa4) = 0;
    *(undefined4 *)(param_1 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
  else if (iVar5 == 0x1992b0b1) {
    FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),*(undefined4 *)(param_1 + 0xc4),1,
                 *(undefined4 *)(param_1 + 200),2,iVar7);
    *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(param_1 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(param_1 + 0xc4))();
  iVar5 = param_1 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),iVar5,1,4,2,iVar7);
  *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
  *(undefined4 *)(param_1 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION FUN_bfe01d60 @ bfe01d60 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_bfe01d60(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  int aiStack_18 [3];
  
  if ((*(uint *)(param_1 + 0xa8) & 4) != 0) {
    return 0;
  }
  if ((*(uint *)(param_1 + 0xa8) & 0x20) != 0) {
    *(undefined4 *)(param_1 + 0x38) = 0;
    return 0;
  }
  if ((*(uint *)(param_1 + 0xac) & 1) == 0) goto switchD_bfe01ddc_caseD_2;
  iVar5 = *(int *)(param_1 + 4);
  uVar1 = *(ushort *)(iVar5 + 0x102);
  switch(*(undefined4 *)(param_1 + 0x38)) {
  case 0:
    if ((uVar1 & 1) != 0) {
LAB_bfe01e04:
      aiStack_18[0] = 0x40;
      FUN_bfe013f0(iVar5,0,&DAT_bfe00270,aiStack_18);
      if (aiStack_18[0] != 0) {
        FUN_bfe01a18(param_1);
      }
    }
    break;
  case 1:
    FUN_bfe018c8(param_1);
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 3:
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      aiStack_18[0] = *(int *)(param_1 + 0xb8);
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    else {
      aiStack_18[0] = 0x40;
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    FUN_bfe013f0(iVar5,0,uVar2,aiStack_18);
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      (**(code **)(param_1 + 0x34))();
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    }
    else {
      (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    }
    *(int *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + aiStack_18[0];
    *(int *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - aiStack_18[0];
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 4:
    (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    *(undefined4 *)(param_1 + 0x38) = 0;
    if ((int)*(uint *)(param_1 + 8) < 0) {
      uVar3 = *(uint *)(param_1 + 8) & 0x7fffffff;
      *(uint *)(param_1 + 8) = uVar3;
      **(undefined1 **)(param_1 + 4) = (char)uVar3;
    }
    if ((uVar1 & 1) != 0) {
      iVar5 = *(int *)(param_1 + 4);
      goto LAB_bfe01e04;
    }
    break;
  case 5:
    if ((uVar1 & 4) != 0) {
      *(undefined4 *)(param_1 + 0x38) = 0;
    }
  }
switchD_bfe01ddc_caseD_2:
  uVar3 = *(uint *)(param_1 + 0xac);
LAB_bfe01e24:
  if ((uVar3 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe;
  if ((uVar3 & 0xfffe) != 0) {
    iVar5 = *(int *)(param_1 + 0x6c);
    if (iVar5 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar5 < 0x10000005) {
      if (iVar5 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar5 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar5 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar5 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe0000;
  if ((uVar3 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar5 = *(int *)(param_1 + 0x90) << 4;
  if (*(int *)(param_1 + 0xa0) != 0x30000001) {
    if (*(int *)(param_1 + 0xa0) != 0x30000002) {
      return 0;
    }
    aiStack_18[0] = 0x200;
    FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,*(undefined4 *)(param_1 + 0x78),aiStack_18);
    if (iVar5 == 0) {
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
      *pbVar6 = *pbVar6 & 0xfe;
    }
    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + aiStack_18[0];
    uVar3 = aiStack_18[0] + *(int *)(param_1 + 0xa4);
    *(uint *)(param_1 + 0xa4) = uVar3;
    uVar4 = 0;
    if (uVar3 < *(uint *)(param_1 + 0x7c)) {
      *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar3 = 0;
    for (; *(uint *)(param_1 + 200) >> 2 != uVar4; uVar4 = uVar4 + 1) {
      uVar3 = uVar3 ^ *(uint *)(uVar4 * 4 + *(int *)(param_1 + 0xc4));
    }
    if (*(uint *)(param_1 + 0xcc) != uVar3) {
      *(uint *)(param_1 + 0xcc) = uVar3;
    }
    iVar5 = param_1 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(param_1 + 0x78) = param_1 + 0xc0;
  aiStack_18[0] = 0x200;
  FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,param_1 + 0xc0,aiStack_18);
  if (iVar5 == 0) {
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  else {
    pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
    *pbVar6 = *pbVar6 & 0xfe;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  if (iVar5 == 0x1991a0a1) {
    *(undefined4 *)(param_1 + 0x7c) = *(undefined4 *)(param_1 + 200);
    *(undefined4 *)(param_1 + 0x84) = 2;
    *(undefined4 *)(param_1 + 0x90) = 2;
    *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
    *(undefined4 *)(param_1 + 0x74) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined4 *)(param_1 + 0x78) = *(undefined4 *)(param_1 + 0xc4);
    *(undefined4 *)(param_1 + 0x98) = 0;
    *(undefined4 *)(param_1 + 0xa4) = 0;
    *(undefined4 *)(param_1 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
  else if (iVar5 == 0x1992b0b1) {
    FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),*(undefined4 *)(param_1 + 0xc4),1,
                 *(undefined4 *)(param_1 + 200),2);
    *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(param_1 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(param_1 + 0xc4))();
  iVar5 = param_1 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),iVar5,1,4,2);
  *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
  *(undefined4 *)(param_1 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION FUN_bfe01d88 @ bfe01d88 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_bfe01d88(int param_1)

{
  ushort uVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  int local_18 [3];
  
  if ((*(uint *)(param_1 + 0xac) & 1) == 0) goto switchD_bfe01ddc_caseD_2;
  iVar5 = *(int *)(param_1 + 4);
  uVar1 = *(ushort *)(iVar5 + 0x102);
  switch(*(undefined4 *)(param_1 + 0x38)) {
  case 0:
    if ((uVar1 & 1) != 0) {
LAB_bfe01e04:
      local_18[0] = 0x40;
      FUN_bfe013f0(iVar5,0,&DAT_bfe00270,local_18);
      if (local_18[0] != 0) {
        FUN_bfe01a18(param_1);
      }
    }
    break;
  case 1:
    FUN_bfe018c8(param_1);
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 3:
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      local_18[0] = *(int *)(param_1 + 0xb8);
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    else {
      local_18[0] = 0x40;
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    FUN_bfe013f0(iVar5,0,uVar2,local_18);
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      (**(code **)(param_1 + 0x34))();
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    }
    else {
      (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    }
    *(int *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + local_18[0];
    *(int *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - local_18[0];
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 4:
    (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    *(undefined4 *)(param_1 + 0x38) = 0;
    if ((int)*(uint *)(param_1 + 8) < 0) {
      uVar3 = *(uint *)(param_1 + 8) & 0x7fffffff;
      *(uint *)(param_1 + 8) = uVar3;
      **(undefined1 **)(param_1 + 4) = (char)uVar3;
    }
    if ((uVar1 & 1) != 0) {
      iVar5 = *(int *)(param_1 + 4);
      goto LAB_bfe01e04;
    }
    break;
  case 5:
    if ((uVar1 & 4) != 0) {
      *(undefined4 *)(param_1 + 0x38) = 0;
    }
  }
switchD_bfe01ddc_caseD_2:
  uVar3 = *(uint *)(param_1 + 0xac);
LAB_bfe01e24:
  if ((uVar3 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe;
  if ((uVar3 & 0xfffe) != 0) {
    iVar5 = *(int *)(param_1 + 0x6c);
    if (iVar5 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar5 < 0x10000005) {
      if (iVar5 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar5 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar5 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar5 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe0000;
  if ((uVar3 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar5 = *(int *)(param_1 + 0x90) << 4;
  if (*(int *)(param_1 + 0xa0) != 0x30000001) {
    if (*(int *)(param_1 + 0xa0) != 0x30000002) {
      return 0;
    }
    local_18[0] = 0x200;
    FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,*(undefined4 *)(param_1 + 0x78),local_18);
    if (iVar5 == 0) {
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
      *pbVar6 = *pbVar6 & 0xfe;
    }
    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + local_18[0];
    uVar3 = local_18[0] + *(int *)(param_1 + 0xa4);
    *(uint *)(param_1 + 0xa4) = uVar3;
    uVar4 = 0;
    if (uVar3 < *(uint *)(param_1 + 0x7c)) {
      *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar3 = 0;
    for (; *(uint *)(param_1 + 200) >> 2 != uVar4; uVar4 = uVar4 + 1) {
      uVar3 = uVar3 ^ *(uint *)(uVar4 * 4 + *(int *)(param_1 + 0xc4));
    }
    if (*(uint *)(param_1 + 0xcc) != uVar3) {
      *(uint *)(param_1 + 0xcc) = uVar3;
    }
    iVar5 = param_1 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(param_1 + 0x78) = param_1 + 0xc0;
  local_18[0] = 0x200;
  FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,param_1 + 0xc0,local_18);
  if (iVar5 == 0) {
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  else {
    pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
    *pbVar6 = *pbVar6 & 0xfe;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  if (iVar5 == 0x1991a0a1) {
    *(undefined4 *)(param_1 + 0x7c) = *(undefined4 *)(param_1 + 200);
    *(undefined4 *)(param_1 + 0x84) = 2;
    *(undefined4 *)(param_1 + 0x90) = 2;
    *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
    *(undefined4 *)(param_1 + 0x74) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined4 *)(param_1 + 0x78) = *(undefined4 *)(param_1 + 0xc4);
    *(undefined4 *)(param_1 + 0x98) = 0;
    *(undefined4 *)(param_1 + 0xa4) = 0;
    *(undefined4 *)(param_1 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
  else if (iVar5 == 0x1992b0b1) {
    FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),*(undefined4 *)(param_1 + 0xc4),1,
                 *(undefined4 *)(param_1 + 200),2);
    *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(param_1 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(param_1 + 0xc4))();
  iVar5 = param_1 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),iVar5,1,4,2);
  *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
  *(undefined4 *)(param_1 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION sub_bfe01d8c @ bfe01d8c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe01d8c(int param_1)

{
  ushort uVar1;
  uint in_v0;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  byte *pbVar6;
  int local_18 [3];
  
  if ((in_v0 & 1) == 0) goto switchD_bfe01ddc_caseD_2;
  iVar5 = *(int *)(param_1 + 4);
  uVar1 = *(ushort *)(iVar5 + 0x102);
  switch(*(undefined4 *)(param_1 + 0x38)) {
  case 0:
    if ((uVar1 & 1) != 0) {
LAB_bfe01e04:
      local_18[0] = 0x40;
      FUN_bfe013f0(iVar5,0,&DAT_bfe00270,local_18);
      if (local_18[0] != 0) {
        FUN_bfe01a18(param_1);
      }
    }
    break;
  case 1:
    FUN_bfe018c8(param_1);
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 3:
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      local_18[0] = *(int *)(param_1 + 0xb8);
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    else {
      local_18[0] = 0x40;
      uVar2 = *(undefined4 *)(param_1 + 0xb4);
    }
    FUN_bfe013f0(iVar5,0,uVar2,local_18);
    if (*(uint *)(param_1 + 0xb8) < 0x41) {
      (**(code **)(param_1 + 0x34))();
      *(undefined4 *)(param_1 + 0x38) = 0;
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x48;
    }
    else {
      (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    }
    *(int *)(param_1 + 0xb4) = *(int *)(param_1 + 0xb4) + local_18[0];
    *(int *)(param_1 + 0xb8) = *(int *)(param_1 + 0xb8) - local_18[0];
    uVar3 = *(uint *)(param_1 + 0xac);
    goto LAB_bfe01e24;
  case 4:
    (**(code **)(param_1 + 0x34))(param_1,param_1 + 0xc);
    *(undefined4 *)(param_1 + 0x38) = 0;
    if ((int)*(uint *)(param_1 + 8) < 0) {
      uVar3 = *(uint *)(param_1 + 8) & 0x7fffffff;
      *(uint *)(param_1 + 8) = uVar3;
      **(undefined1 **)(param_1 + 4) = (char)uVar3;
    }
    if ((uVar1 & 1) != 0) {
      iVar5 = *(int *)(param_1 + 4);
      goto LAB_bfe01e04;
    }
    break;
  case 5:
    if ((uVar1 & 4) != 0) {
      *(undefined4 *)(param_1 + 0x38) = 0;
    }
  }
switchD_bfe01ddc_caseD_2:
  uVar3 = *(uint *)(param_1 + 0xac);
LAB_bfe01e24:
  if ((uVar3 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe;
  if ((uVar3 & 0xfffe) != 0) {
    iVar5 = *(int *)(param_1 + 0x6c);
    if (iVar5 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar5 < 0x10000005) {
      if (iVar5 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar5 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar5 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar5 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(param_1 + 0xac) = uVar3 & 0xfffe0000;
  if ((uVar3 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar5 = *(int *)(param_1 + 0x90) << 4;
  if (*(int *)(param_1 + 0xa0) != 0x30000001) {
    if (*(int *)(param_1 + 0xa0) != 0x30000002) {
      return 0;
    }
    local_18[0] = 0x200;
    FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,*(undefined4 *)(param_1 + 0x78),local_18);
    if (iVar5 == 0) {
      *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
      *pbVar6 = *pbVar6 & 0xfe;
    }
    *(int *)(param_1 + 0x78) = *(int *)(param_1 + 0x78) + local_18[0];
    uVar3 = local_18[0] + *(int *)(param_1 + 0xa4);
    *(uint *)(param_1 + 0xa4) = uVar3;
    uVar4 = 0;
    if (uVar3 < *(uint *)(param_1 + 0x7c)) {
      *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar3 = 0;
    for (; *(uint *)(param_1 + 200) >> 2 != uVar4; uVar4 = uVar4 + 1) {
      uVar3 = uVar3 ^ *(uint *)(uVar4 * 4 + *(int *)(param_1 + 0xc4));
    }
    if (*(uint *)(param_1 + 0xcc) != uVar3) {
      *(uint *)(param_1 + 0xcc) = uVar3;
    }
    iVar5 = param_1 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(param_1 + 0x78) = param_1 + 0xc0;
  local_18[0] = 0x200;
  FUN_bfe013f0(*(undefined4 *)(param_1 + 4),iVar5,param_1 + 0xc0,local_18);
  if (iVar5 == 0) {
    *(undefined1 *)(*(int *)(param_1 + 4) + 0x102) = 0x40;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  else {
    pbVar6 = (byte *)(*(int *)(param_1 + 4) + 0x106 + iVar5);
    *pbVar6 = *pbVar6 & 0xfe;
    iVar5 = *(int *)(param_1 + 0xc0);
  }
  if (iVar5 == 0x1991a0a1) {
    *(undefined4 *)(param_1 + 0x7c) = *(undefined4 *)(param_1 + 200);
    *(undefined4 *)(param_1 + 0x84) = 2;
    *(undefined4 *)(param_1 + 0x90) = 2;
    *(undefined4 *)(param_1 + 0x6c) = 0x10000010;
    *(undefined4 *)(param_1 + 0x74) = *(undefined4 *)(param_1 + 0xb0);
    *(undefined4 *)(param_1 + 0x78) = *(undefined4 *)(param_1 + 0xc4);
    *(undefined4 *)(param_1 + 0x98) = 0;
    *(undefined4 *)(param_1 + 0xa4) = 0;
    *(undefined4 *)(param_1 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(param_1 + 0xa0) = uVar2;
  }
  else if (iVar5 == 0x1992b0b1) {
    FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),*(undefined4 *)(param_1 + 0xc4),1,
                 *(undefined4 *)(param_1 + 200),2);
    *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(param_1 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(param_1 + 0xc4))();
  iVar5 = param_1 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(param_1,*(undefined4 *)(param_1 + 0x5c),iVar5,1,4,2);
  *(undefined4 *)(param_1 + 0x6c) = 0x40000001;
  *(undefined4 *)(param_1 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION sub_bfe01ebc @ bfe01ebc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe01ebc(void)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  byte *pbVar5;
  int unaff_s0;
  uint unaff_s1;
  int in_stack_00000048;
  
  (**(code **)(unaff_s0 + 0x34))(unaff_s0,unaff_s0 + 0xc);
  *(undefined4 *)(unaff_s0 + 0x38) = 0;
  if ((int)*(uint *)(unaff_s0 + 8) < 0) {
    uVar1 = *(uint *)(unaff_s0 + 8) & 0x7fffffff;
    *(uint *)(unaff_s0 + 8) = uVar1;
    **(undefined1 **)(unaff_s0 + 4) = (char)uVar1;
  }
  if ((unaff_s1 & 1) != 0) {
    in_stack_00000048 = 0x40;
    FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),0,&DAT_bfe00270,&stack0x00000048);
    if (in_stack_00000048 != 0) {
      FUN_bfe01a18(unaff_s0);
    }
  }
  uVar1 = *(uint *)(unaff_s0 + 0xac);
  if ((uVar1 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(unaff_s0 + 0xac) = uVar1 & 0xfffe;
  if ((uVar1 & 0xfffe) != 0) {
    iVar4 = *(int *)(unaff_s0 + 0x6c);
    if (iVar4 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar4 < 0x10000005) {
      if (iVar4 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar4 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar4 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar4 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(unaff_s0 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(unaff_s0 + 0xac) = uVar1 & 0xfffe0000;
  if ((uVar1 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar4 = *(int *)(unaff_s0 + 0x90) << 4;
  if (*(int *)(unaff_s0 + 0xa0) != 0x30000001) {
    if (*(int *)(unaff_s0 + 0xa0) != 0x30000002) {
      return 0;
    }
    in_stack_00000048 = 0x200;
    FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),iVar4,*(undefined4 *)(unaff_s0 + 0x78),
                 &stack0x00000048);
    if (iVar4 == 0) {
      *(undefined1 *)(*(int *)(unaff_s0 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar5 = (byte *)(*(int *)(unaff_s0 + 4) + 0x106 + iVar4);
      *pbVar5 = *pbVar5 & 0xfe;
    }
    *(int *)(unaff_s0 + 0x78) = *(int *)(unaff_s0 + 0x78) + in_stack_00000048;
    uVar1 = in_stack_00000048 + *(int *)(unaff_s0 + 0xa4);
    *(uint *)(unaff_s0 + 0xa4) = uVar1;
    uVar3 = 0;
    if (uVar1 < *(uint *)(unaff_s0 + 0x7c)) {
      *(undefined4 *)(unaff_s0 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar1 = 0;
    for (; *(uint *)(unaff_s0 + 200) >> 2 != uVar3; uVar3 = uVar3 + 1) {
      uVar1 = uVar1 ^ *(uint *)(uVar3 * 4 + *(int *)(unaff_s0 + 0xc4));
    }
    if (*(uint *)(unaff_s0 + 0xcc) != uVar1) {
      *(uint *)(unaff_s0 + 0xcc) = uVar1;
    }
    iVar4 = unaff_s0 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(unaff_s0 + 0x78) = unaff_s0 + 0xc0;
  in_stack_00000048 = 0x200;
  FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),iVar4,unaff_s0 + 0xc0,&stack0x00000048);
  if (iVar4 == 0) {
    *(undefined1 *)(*(int *)(unaff_s0 + 4) + 0x102) = 0x40;
    iVar4 = *(int *)(unaff_s0 + 0xc0);
  }
  else {
    pbVar5 = (byte *)(*(int *)(unaff_s0 + 4) + 0x106 + iVar4);
    *pbVar5 = *pbVar5 & 0xfe;
    iVar4 = *(int *)(unaff_s0 + 0xc0);
  }
  if (iVar4 == 0x1991a0a1) {
    *(undefined4 *)(unaff_s0 + 0x7c) = *(undefined4 *)(unaff_s0 + 200);
    *(undefined4 *)(unaff_s0 + 0x84) = 2;
    *(undefined4 *)(unaff_s0 + 0x90) = 2;
    *(undefined4 *)(unaff_s0 + 0x6c) = 0x10000010;
    *(undefined4 *)(unaff_s0 + 0x74) = *(undefined4 *)(unaff_s0 + 0xb0);
    *(undefined4 *)(unaff_s0 + 0x78) = *(undefined4 *)(unaff_s0 + 0xc4);
    *(undefined4 *)(unaff_s0 + 0x98) = 0;
    *(undefined4 *)(unaff_s0 + 0xa4) = 0;
    *(undefined4 *)(unaff_s0 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(unaff_s0 + 0xa0) = uVar2;
  }
  else if (iVar4 == 0x1992b0b1) {
    FUN_bfe01cd8(unaff_s0,*(undefined4 *)(unaff_s0 + 0x5c),*(undefined4 *)(unaff_s0 + 0xc4),1);
    *(undefined4 *)(unaff_s0 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(unaff_s0 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(unaff_s0 + 0xc4))();
  iVar4 = unaff_s0 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(unaff_s0,*(undefined4 *)(unaff_s0 + 0x5c),iVar4,1);
  *(undefined4 *)(unaff_s0 + 0x6c) = 0x40000001;
  *(undefined4 *)(unaff_s0 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION sub_bfe01ec0 @ bfe01ec0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe01ec0(void)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  byte *pbVar5;
  int unaff_s0;
  uint unaff_s1;
  int in_stack_00000018;
  
  (**(code **)(unaff_s0 + 0x34))(unaff_s0,unaff_s0 + 0xc);
  *(undefined4 *)(unaff_s0 + 0x38) = 0;
  if ((int)*(uint *)(unaff_s0 + 8) < 0) {
    uVar1 = *(uint *)(unaff_s0 + 8) & 0x7fffffff;
    *(uint *)(unaff_s0 + 8) = uVar1;
    **(undefined1 **)(unaff_s0 + 4) = (char)uVar1;
  }
  if ((unaff_s1 & 1) != 0) {
    in_stack_00000018 = 0x40;
    FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),0,&DAT_bfe00270,&stack0x00000018);
    if (in_stack_00000018 != 0) {
      FUN_bfe01a18(unaff_s0);
    }
  }
  uVar1 = *(uint *)(unaff_s0 + 0xac);
  if ((uVar1 & 0xfffefffe) == 0) {
    return 0;
  }
  *(uint *)(unaff_s0 + 0xac) = uVar1 & 0xfffe;
  if ((uVar1 & 0xfffe) != 0) {
    iVar4 = *(int *)(unaff_s0 + 0x6c);
    if (iVar4 == 0x10000004) {
      uVar2 = 0x20000020;
    }
    else if (iVar4 < 0x10000005) {
      if (iVar4 == 0x10000001) {
        uVar2 = 0x20000004;
      }
      else {
        if (iVar4 != 0x10000002) goto LAB_bfe01e74;
        uVar2 = 0x20000010;
      }
    }
    else if (iVar4 == 0x10000008) {
      uVar2 = 0x20000080;
    }
    else {
      if (iVar4 != 0x40000001) goto LAB_bfe01e74;
      uVar2 = 0x30000001;
    }
    *(undefined4 *)(unaff_s0 + 0xa0) = uVar2;
  }
LAB_bfe01e74:
  *(uint *)(unaff_s0 + 0xac) = uVar1 & 0xfffe0000;
  if ((uVar1 & 0xfffe0000) == 0) {
    return 0;
  }
  iVar4 = *(int *)(unaff_s0 + 0x90) << 4;
  if (*(int *)(unaff_s0 + 0xa0) != 0x30000001) {
    if (*(int *)(unaff_s0 + 0xa0) != 0x30000002) {
      return 0;
    }
    in_stack_00000018 = 0x200;
    FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),iVar4,*(undefined4 *)(unaff_s0 + 0x78),
                 &stack0x00000018);
    if (iVar4 == 0) {
      *(undefined1 *)(*(int *)(unaff_s0 + 4) + 0x102) = 0x40;
    }
    else {
      pbVar5 = (byte *)(*(int *)(unaff_s0 + 4) + 0x106 + iVar4);
      *pbVar5 = *pbVar5 & 0xfe;
    }
    *(int *)(unaff_s0 + 0x78) = *(int *)(unaff_s0 + 0x78) + in_stack_00000018;
    uVar1 = in_stack_00000018 + *(int *)(unaff_s0 + 0xa4);
    *(uint *)(unaff_s0 + 0xa4) = uVar1;
    uVar3 = 0;
    if (uVar1 < *(uint *)(unaff_s0 + 0x7c)) {
      *(undefined4 *)(unaff_s0 + 0x6c) = 0x10000010;
      return 0;
    }
    uVar1 = 0;
    for (; *(uint *)(unaff_s0 + 200) >> 2 != uVar3; uVar3 = uVar3 + 1) {
      uVar1 = uVar1 ^ *(uint *)(uVar3 * 4 + *(int *)(unaff_s0 + 0xc4));
    }
    if (*(uint *)(unaff_s0 + 0xcc) != uVar1) {
      *(uint *)(unaff_s0 + 0xcc) = uVar1;
    }
    iVar4 = unaff_s0 + 0xcc;
    goto LAB_bfe020fc;
  }
  *(int *)(unaff_s0 + 0x78) = unaff_s0 + 0xc0;
  in_stack_00000018 = 0x200;
  FUN_bfe013f0(*(undefined4 *)(unaff_s0 + 4),iVar4,unaff_s0 + 0xc0,&stack0x00000018);
  if (iVar4 == 0) {
    *(undefined1 *)(*(int *)(unaff_s0 + 4) + 0x102) = 0x40;
    iVar4 = *(int *)(unaff_s0 + 0xc0);
  }
  else {
    pbVar5 = (byte *)(*(int *)(unaff_s0 + 4) + 0x106 + iVar4);
    *pbVar5 = *pbVar5 & 0xfe;
    iVar4 = *(int *)(unaff_s0 + 0xc0);
  }
  if (iVar4 == 0x1991a0a1) {
    *(undefined4 *)(unaff_s0 + 0x7c) = *(undefined4 *)(unaff_s0 + 200);
    *(undefined4 *)(unaff_s0 + 0x84) = 2;
    *(undefined4 *)(unaff_s0 + 0x90) = 2;
    *(undefined4 *)(unaff_s0 + 0x6c) = 0x10000010;
    *(undefined4 *)(unaff_s0 + 0x74) = *(undefined4 *)(unaff_s0 + 0xb0);
    *(undefined4 *)(unaff_s0 + 0x78) = *(undefined4 *)(unaff_s0 + 0xc4);
    *(undefined4 *)(unaff_s0 + 0x98) = 0;
    *(undefined4 *)(unaff_s0 + 0xa4) = 0;
    *(undefined4 *)(unaff_s0 + 0x88) = 0;
    uVar2 = 0x30000002;
LAB_bfe020b4:
    *(undefined4 *)(unaff_s0 + 0xa0) = uVar2;
  }
  else if (iVar4 == 0x1992b0b1) {
    FUN_bfe01cd8(unaff_s0,*(undefined4 *)(unaff_s0 + 0x5c),*(undefined4 *)(unaff_s0 + 0xc4),1);
    *(undefined4 *)(unaff_s0 + 0x6c) = 0x40000001;
    uVar2 = 0x20000040;
    goto LAB_bfe020b4;
  }
  if (*(int *)(unaff_s0 + 0xc0) != 0x1993c0c1) {
    return 0;
  }
  _DAT_b8818a00 = 0xe66a;
  (**(code **)(unaff_s0 + 0xc4))();
  iVar4 = unaff_s0 + 0xc4;
LAB_bfe020fc:
  FUN_bfe01cd8(unaff_s0,*(undefined4 *)(unaff_s0 + 0x5c),iVar4,1);
  *(undefined4 *)(unaff_s0 + 0x6c) = 0x40000001;
  *(undefined4 *)(unaff_s0 + 0xa0) = 0x20000040;
  return 0;
}



/* ===== FUNCTION FUN_bfe02258 @ bfe02258 =====
 * References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe02258(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 local_1b8;
  int local_1b4;
  uint local_110;
  int local_10c;
  undefined4 local_e8;
  int local_e4;
  uint local_40;
  int local_3c;
  
  _DAT_b8818504 = 0;
  local_1b4 = -0x477bc000;
  local_e4 = -0x477b0000;
  local_1b8 = 0;
  local_e8 = 1;
  _DAT_b8818a20 = 0;
  iVar1 = param_1;
LAB_bfe022b8:
  do {
    local_10c = *(int *)(local_1b4 + 2);
    local_110 = (uint)*(byte *)(local_1b4 + 10);
    local_3c = *(int *)(local_e4 + 2);
    local_40 = (uint)*(byte *)(local_e4 + 10);
    if ((local_10c == 0) && (local_3c == 0)) {
      if (iVar1 != 0) goto LAB_bfe02310;
    }
    else if (iVar1 != 0) {
      _DAT_b8818a00 = CONCAT22(_DAT_b8818a02,0xe66a);
      FUN_bfe01804(&local_1b8);
      FUN_bfe01804(&local_e8);
      _DAT_b8818a20 = FUN_bfe01180();
      iVar1 = 0;
      goto LAB_bfe022b8;
    }
    FUN_bfe01d60(&local_1b8);
    FUN_bfe01d60(&local_e8);
    if ((((param_1 != 0) && (DAT_b8818a02 != -0x5b)) && (DAT_b8818a02 != 'Z')) &&
       (iVar1 = FUN_bfe01180(), *(uint *)PTR_DAT_bfe02e88 < (uint)(iVar1 - _DAT_b8818a20))) {
      _DAT_b8818a00 = _DAT_b8818a00 | 0x20000000;
      return;
    }
    iVar1 = 0;
    if ((DAT_b8818a02 != -0x5b) && (DAT_b8818a02 != 'Z')) {
      iVar2 = FUN_bfe01180();
      iVar1 = 0;
      if (5000 < (uint)(iVar2 - _DAT_b8818a20)) {
LAB_bfe02310:
        _DAT_b8818a00 = _DAT_b8818a00 & 0xdfffffff;
        return;
      }
    }
  } while( true );
}



/* ===== FUNCTION sub_bfe02344 @ bfe02344 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe02344(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  char *unaff_s0;
  int unaff_s1;
  uint unaff_s2;
  int unaff_s3;
  int in_stack_000001dc;
  int in_stack_000002ac;
  
  do {
    if (param_1 == 0) goto LAB_bfe0237c;
    *(undefined2 *)(unaff_s2 | 0x8a00) = 0xe66a;
    FUN_bfe01804(&stack0x000001d8);
    FUN_bfe01804(&stack0x000002a8);
    uVar1 = FUN_bfe01180();
    *(undefined4 *)((undefined2 *)(unaff_s2 | 0x8a00) + 0x10) = uVar1;
    while( true ) {
      param_1 = 0;
      if ((*(int *)(in_stack_000001dc + 2) != 0) || (*(int *)(in_stack_000002ac + 2) != 0)) break;
LAB_bfe0237c:
      FUN_bfe01d60(&stack0x000001d8);
      FUN_bfe01d60(&stack0x000002a8);
      if ((unaff_s1 != 0) &&
         (((*unaff_s0 != -0x5b && (*unaff_s0 != 'Z')) &&
          (iVar2 = FUN_bfe01180(),
          **(uint **)(unaff_s3 + 0x2e88) < (uint)(iVar2 - *(int *)(unaff_s0 + 0x1e)))))) {
        *(uint *)(unaff_s0 + -2) = *(uint *)(unaff_s0 + -2) | 0x20000000;
        return;
      }
      if (((*unaff_s0 != -0x5b) && (*unaff_s0 != 'Z')) &&
         (iVar2 = FUN_bfe01180(), 5000 < (uint)(iVar2 - *(int *)(unaff_s0 + 0x1e)))) {
        _DAT_b8818a00 = _DAT_b8818a00 & 0xdfffffff;
        return;
      }
    }
  } while( true );
}



/* ===== FUNCTION sub_bfe02348 @ bfe02348 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe02348(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  char *unaff_s0;
  int unaff_s1;
  uint unaff_s2;
  int unaff_s3;
  int in_stack_00000014;
  uint in_stack_000000b8;
  int in_stack_000000bc;
  int in_stack_000000e4;
  uint in_stack_00000188;
  int in_stack_0000018c;
  
  do {
    if (param_1 == 0) goto LAB_bfe0237c;
    *(undefined2 *)(unaff_s2 | 0x8a00) = 0xe66a;
    FUN_bfe01804(&stack0x00000010);
    FUN_bfe01804(&stack0x000000e0);
    uVar1 = FUN_bfe01180();
    *(undefined4 *)((undefined2 *)(unaff_s2 | 0x8a00) + 0x10) = uVar1;
    while( true ) {
      param_1 = 0;
      in_stack_000000bc = *(int *)(in_stack_00000014 + 2);
      in_stack_000000b8 = (uint)*(byte *)(in_stack_00000014 + 10);
      in_stack_0000018c = *(int *)(in_stack_000000e4 + 2);
      in_stack_00000188 = (uint)*(byte *)(in_stack_000000e4 + 10);
      if ((in_stack_000000bc != 0) || (in_stack_0000018c != 0)) break;
LAB_bfe0237c:
      FUN_bfe01d60(&stack0x00000010);
      FUN_bfe01d60(&stack0x000000e0);
      if ((unaff_s1 != 0) &&
         (((*unaff_s0 != -0x5b && (*unaff_s0 != 'Z')) &&
          (iVar2 = FUN_bfe01180(),
          **(uint **)(unaff_s3 + 0x2e88) < (uint)(iVar2 - *(int *)(unaff_s0 + 0x1e)))))) {
        *(uint *)(unaff_s0 + -2) = *(uint *)(unaff_s0 + -2) | 0x20000000;
        return;
      }
      if (((*unaff_s0 != -0x5b) && (*unaff_s0 != 'Z')) &&
         (iVar2 = FUN_bfe01180(), 5000 < (uint)(iVar2 - *(int *)(unaff_s0 + 0x1e)))) {
        _DAT_b8818a00 = _DAT_b8818a00 & 0xdfffffff;
        return;
      }
    }
  } while( true );
}



/* ===== FUNCTION FUN_bfe02434 @ bfe02434 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe02434(void)

{
  if ((_DAT_b8818a00 & 0xc0000000) == 0) {
    if (_DAT_b8818a00 != 0x5991) {
      return;
    }
    _DAT_b8818a00 = _DAT_b8818a00 | 0xc0000000;
  }
  _DAT_b8818504 = 0;
  if ((_DAT_b8818a00 == 0x1995) || (_DAT_b8818a00 == 0x5991)) {
    _DAT_b8818a00 = CONCAT22(_DAT_b8818a02,0xe66a);
    FUN_bfe017c0();
    FUN_bfe02258(0);
  }
  FUN_bfe017c0();
  _DAT_b8818a00 = CONCAT22(_DAT_b8818a02,0x1995);
  FUN_bfe01c90(3000000);
  return;
}



/* ===== FUNCTION sub_bfe02470 @ bfe02470 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe02470(void)

{
  uint in_v0;
  undefined4 *puVar1;
  
  puVar1 = (undefined4 *)(in_v0 | 0x8504);
  *puVar1 = 0;
  if ((*(short *)(puVar1 + 0x13f) == 0x1995) || (*(short *)(puVar1 + 0x13f) == 0x5991)) {
    _DAT_b8818a00 = 0xe66a;
    FUN_bfe017c0();
    FUN_bfe02258(0);
  }
  FUN_bfe017c0();
  _DAT_b8818a00 = 0x1995;
  FUN_bfe01c90(3000000);
  return;
}



/* ===== FUNCTION sub_bfe024e8 @ bfe024e8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe024e8(void)

{
  return;
}



/* ===== FUNCTION FUN_bfe024f4 @ bfe024f4 =====
 * References 0xb884... registers, likely USB/MMC-side peripheral MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe024f4(void)

{
  uint uVar1;
  
  if ((_DAT_b8818a00 & 0xc0000000) != 0) {
    FUN_bfe02258(1);
    uVar1 = FUN_bfe01180();
    if (*(uint *)PTR_DAT_bfe02e8c < uVar1) {
      first_stage_peripheral_gate_for_usb_blocks(0xb8844000);
      FUN_bfe01588();
      _DAT_b8800064 = _DAT_b8800064 | 0x3000000;
      _DAT_b8818a00 = CONCAT22(_DAT_b8818a02,0xe66a);
    }
    else if ((_DAT_b8818a00 & 0x20000000) == 0) {
      FUN_bfe01c90(3000000);
    }
    DAT_a0000000 = DAT_a0000000 & 0xfc | (byte)(_DAT_b8818a00 >> 0x1e) & 1 |
                   (byte)((_DAT_b8818a00 >> 0x1f) << 1);
    _DAT_a0000002 = (undefined2)*(undefined4 *)PTR_DAT_bfe02e8c;
    _DAT_a0000004 = (undefined2)*(undefined4 *)PTR_DAT_bfe02e88;
  }
  return;
}



/* ===== FUNCTION sub_bfe025dc @ bfe025dc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe025dc(void)

{
  int unaff_s0;
  uint *unaff_s1;
  
  if ((*unaff_s1 & 0x20000000) == 0) {
    FUN_bfe01c90(3000000);
  }
  DAT_a0000000 = DAT_a0000000 & 0xfc | (byte)(_DAT_b8818a00 >> 0x1e) & 1 |
                 (byte)((_DAT_b8818a00 >> 0x1f) << 1);
  _DAT_a0000002 = (short)**(undefined4 **)(unaff_s0 + 0x2e8c);
  _DAT_a0000004 = (short)*(undefined4 *)PTR_DAT_bfe02e88;
  return;
}



/* ===== FUNCTION sub_bfe025e0 @ bfe025e0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe025e0(void)

{
  int unaff_s0;
  uint *unaff_s1;
  
  if ((*unaff_s1 & 0x20000000) == 0) {
    FUN_bfe01c90(3000000);
  }
  DAT_a0000000 = DAT_a0000000 & 0xfc | (byte)(_DAT_b8818a00 >> 0x1e) & 1 |
                 (byte)((_DAT_b8818a00 >> 0x1f) << 1);
  _DAT_a0000002 = (short)**(undefined4 **)(unaff_s0 + 0x2e8c);
  _DAT_a0000004 = (short)*(undefined4 *)PTR_DAT_bfe02e88;
  return;
}



/* ===== FUNCTION FUN_bfe02604 @ bfe02604 =====
 * References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_bfe02604(undefined1 *param_1)

{
  uint uVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined1 local_10 [4];
  undefined1 local_c;
  
  _DAT_b882e098 = CONCAT11(1,DAT_b882e098);
  iVar2 = *(int *)(param_1 + 4);
  local_10[0] = *param_1;
  if (iVar2 != 0) {
    local_10[1] = param_1[8];
    if (iVar2 == 3) {
      local_10[2] = param_1[9];
      local_10[3] = param_1[10];
    }
    else if (iVar2 == 4) {
      local_10[2] = param_1[9];
      local_10[3] = param_1[10];
      local_c = param_1[0xb];
    }
    else if (iVar2 == 2) {
      local_10[2] = param_1[9];
    }
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x7000000;
  _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  iVar2 = *(int *)(param_1 + 4);
  for (puVar3 = local_10; (uint)((int)puVar3 - (int)local_10) < iVar2 + 1U; puVar3 = puVar3 + 1) {
    DAT_afc00000 = *puVar3;
    iVar2 = *(int *)(param_1 + 4);
  }
  if (*(int *)(param_1 + 0xc) != 0) {
    DAT_afc00000 = 0xff;
  }
  if (param_1[0x21] == '\x02') {
    _DAT_b882e098 = _DAT_b882e098 | 0x10000;
  }
  uVar1 = 0;
  if (*(int *)(param_1 + 0x18) == 1) {
    *(undefined1 *)*(undefined4 **)(param_1 + 0x1c) = DAT_afc00000;
  }
  else {
    puVar5 = *(undefined4 **)(param_1 + 0x1c);
    if (param_1[0x20] == '\0') {
      while (uVar1 < *(uint *)(param_1 + 0x18)) {
        puVar4 = (undefined4 *)(&DAT_afc00000 + uVar1);
        if ((((uint)puVar5 | *(uint *)(param_1 + 0x18) - uVar1) & 3) == 0) {
          uVar1 = uVar1 + 4;
          *puVar5 = *puVar4;
          puVar5 = puVar5 + 1;
        }
        else {
          puVar3 = (undefined1 *)((int)puVar5 + uVar1);
          uVar1 = uVar1 + 1;
          *puVar3 = *(undefined1 *)puVar4;
        }
      }
    }
    else {
      first_stage_sfspi_transfer_setup(*(undefined4 *)(param_1 + 0x1c));
    }
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf8ffffff;
  _DAT_b882e098 = (ushort)DAT_b882e098;
  DAT_afc00000 = 0;
  return 0;
}



/* ===== FUNCTION sub_bfe02614 @ bfe02614 =====
 * References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe02614(undefined1 *param_1,uint param_2)

{
  uint *in_v0;
  uint uVar1;
  uint in_v1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined1 local_10 [4];
  undefined1 uStack_c;
  
  puVar5 = *(undefined4 **)(param_1 + 0x1c);
  *in_v0 = in_v1 | param_2;
  *(undefined1 *)((int)in_v0 + -0x2f) = 1;
  iVar2 = *(int *)(param_1 + 4);
  local_10[0] = *param_1;
  if (iVar2 != 0) {
    local_10[1] = param_1[8];
    if (iVar2 == 3) {
      local_10[2] = param_1[9];
      local_10[3] = param_1[10];
    }
    else if (iVar2 == 4) {
      local_10[2] = param_1[9];
      local_10[3] = param_1[10];
      uStack_c = param_1[0xb];
    }
    else if (iVar2 == 2) {
      local_10[2] = param_1[9];
    }
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x6000000;
  _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  iVar2 = *(int *)(param_1 + 4);
  for (puVar3 = local_10; (uint)((int)puVar3 - (int)local_10) < iVar2 + 1U; puVar3 = puVar3 + 1) {
    DAT_afc00000 = *puVar3;
    iVar2 = *(int *)(param_1 + 4);
  }
  if (*(int *)(param_1 + 0xc) != 0) {
    DAT_afc00000 = 0xff;
  }
  if (param_1[0x21] == '\x02') {
    _DAT_b882e098 = _DAT_b882e098 | 0x10000;
  }
  uVar1 = 0;
  if (*(int *)(param_1 + 0x18) == 1) {
    *(undefined1 *)puVar5 = DAT_afc00000;
  }
  else if (param_1[0x20] == '\0') {
    while (uVar1 < *(uint *)(param_1 + 0x18)) {
      puVar4 = (undefined4 *)(&DAT_afc00000 + uVar1);
      if ((((uint)puVar5 | *(uint *)(param_1 + 0x18) - uVar1) & 3) == 0) {
        uVar1 = uVar1 + 4;
        *puVar5 = *puVar4;
        puVar5 = puVar5 + 1;
      }
      else {
        puVar3 = (undefined1 *)((int)puVar5 + uVar1);
        uVar1 = uVar1 + 1;
        *puVar3 = *(undefined1 *)puVar4;
      }
    }
  }
  else {
    first_stage_sfspi_transfer_setup(*(undefined4 *)(param_1 + 0x1c));
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf8ffffff;
  _DAT_b882e098 = (ushort)DAT_b882e098;
  DAT_afc00000 = 0;
  return 0;
}



/* ===== FUNCTION sub_bfe02780 @ bfe02780 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe02780(int param_1,undefined4 param_2,undefined4 *param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined1 uStack00000032;
  undefined1 uStack00000033;
  
  uStack00000032 = *(undefined1 *)(param_1 + 9);
  uStack00000033 = *(undefined1 *)(param_1 + 10);
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x6000000;
  _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  iVar3 = *(int *)(param_1 + 4);
  for (puVar2 = &stack0x00000030; (uint)((int)puVar2 - (int)&stack0x00000030) < iVar3 + 1U;
      puVar2 = puVar2 + 1) {
    DAT_afc00000 = *puVar2;
    iVar3 = *(int *)(param_1 + 4);
  }
  if (*(int *)(param_1 + 0xc) != 0) {
    DAT_afc00000 = 0xff;
  }
  if (*(char *)(param_1 + 0x21) == '\x02') {
    _DAT_b882e098 = _DAT_b882e098 | 0x10000;
  }
  uVar1 = 0;
  if (*(int *)(param_1 + 0x18) == 1) {
    *(undefined1 *)param_3 = DAT_afc00000;
  }
  else if (*(char *)(param_1 + 0x20) == '\0') {
    while (uVar1 < *(uint *)(param_1 + 0x18)) {
      puVar4 = (undefined4 *)(&DAT_afc00000 + uVar1);
      if ((((uint)param_3 | *(uint *)(param_1 + 0x18) - uVar1) & 3) == 0) {
        uVar1 = uVar1 + 4;
        *param_3 = *puVar4;
        param_3 = param_3 + 1;
      }
      else {
        puVar2 = (undefined1 *)((int)param_3 + uVar1);
        uVar1 = uVar1 + 1;
        *puVar2 = *(undefined1 *)puVar4;
      }
    }
  }
  else {
    first_stage_sfspi_transfer_setup(*(undefined4 *)(param_1 + 0x1c));
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf8ffffff;
  _DAT_b882e098 = (ushort)DAT_b882e098;
  DAT_afc00000 = 0;
  return 0;
}



/* ===== FUNCTION sub_bfe02784 @ bfe02784 =====
 * References 0xb882... registers, likely SPI/SF or clock-controller MMIO.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 sub_bfe02784(int param_1,undefined4 param_2,undefined4 *param_3)

{
  uint uVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined1 uStack00000012;
  undefined1 uStack00000013;
  
  uStack00000012 = *(undefined1 *)(param_1 + 9);
  uStack00000013 = *(undefined1 *)(param_1 + 10);
  _DAT_b882e0c8 = _DAT_b882e0c8 | 0x6000000;
  _DAT_b882e098 = _DAT_b882e098 & 0xfff8ffff;
  iVar3 = *(int *)(param_1 + 4);
  for (puVar2 = &stack0x00000010; (uint)((int)puVar2 - (int)&stack0x00000010) < iVar3 + 1U;
      puVar2 = puVar2 + 1) {
    DAT_afc00000 = *puVar2;
    iVar3 = *(int *)(param_1 + 4);
  }
  if (*(int *)(param_1 + 0xc) != 0) {
    DAT_afc00000 = 0xff;
  }
  if (*(char *)(param_1 + 0x21) == '\x02') {
    _DAT_b882e098 = _DAT_b882e098 | 0x10000;
  }
  uVar1 = 0;
  if (*(int *)(param_1 + 0x18) == 1) {
    *(undefined1 *)param_3 = DAT_afc00000;
  }
  else if (*(char *)(param_1 + 0x20) == '\0') {
    while (uVar1 < *(uint *)(param_1 + 0x18)) {
      puVar4 = (undefined4 *)(&DAT_afc00000 + uVar1);
      if ((((uint)param_3 | *(uint *)(param_1 + 0x18) - uVar1) & 3) == 0) {
        uVar1 = uVar1 + 4;
        *param_3 = *puVar4;
        param_3 = param_3 + 1;
      }
      else {
        puVar2 = (undefined1 *)((int)param_3 + uVar1);
        uVar1 = uVar1 + 1;
        *puVar2 = *(undefined1 *)puVar4;
      }
    }
  }
  else {
    first_stage_sfspi_transfer_setup(*(undefined4 *)(param_1 + 0x1c));
  }
  _DAT_b882e0c8 = _DAT_b882e0c8 & 0xf8ffffff;
  _DAT_b882e098 = (ushort)DAT_b882e098;
  DAT_afc00000 = 0;
  return 0;
}



/* ===== FUNCTION FUN_bfe0284c @ bfe0284c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

int FUN_bfe0284c(undefined4 param_1)

{
  int iVar1;
  undefined1 local_30 [4];
  undefined4 local_2c;
  undefined1 local_28;
  undefined4 local_18;
  undefined4 local_14;
  undefined1 local_f;
  
  FUN_bfe00e3c(local_30,0,0x24);
  local_30[0] = 0xf;
  local_2c = 1;
  local_28 = 0xc0;
  local_18 = 1;
  local_f = 1;
  local_14 = param_1;
  iVar1 = FUN_bfe02604(local_30);
  if (iVar1 < 0) {
    iVar1 = -1;
  }
  return iVar1;
}



/* ===== FUNCTION sub_bfe028ac @ bfe028ac =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe028ac(void)

{
  char cStack00000019;
  byte bStack0000001c;
  char in_stack_00000054;
  undefined1 auStack_8 [4];
  undefined4 uStack_4;
  
  bStack0000001c = 0;
  FUN_bfe00e3c(auStack_8,0,0x24);
  auStack_8[0] = 0x13;
  uStack_4 = 3;
  cStack00000019 = 1;
  FUN_bfe02604(auStack_8);
  do {
    FUN_bfe0284c(&stack0x0000001c);
  } while ((bStack0000001c & 1) != 0);
  FUN_bfe00e3c(auStack_8,0,0x24);
  auStack_8[0] = 3;
  if (in_stack_00000054 == '\x02') {
    auStack_8[0] = 0x3b;
  }
  uStack_4 = 2;
  cStack00000019 = in_stack_00000054;
  FUN_bfe02604(auStack_8);
  return;
}



/* ===== FUNCTION FUN_bfe028b0 @ bfe028b0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe028b0(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined1 param_5,char param_6)

{
  undefined1 local_48 [4];
  undefined4 local_44;
  byte local_40;
  undefined1 local_3f;
  undefined1 local_3e;
  undefined4 local_3c;
  undefined4 local_30;
  undefined4 local_2c;
  undefined1 local_28;
  char local_27;
  byte local_24 [8];
  
  local_24[0] = 0;
  FUN_bfe00e3c(local_48,0,0x24);
  local_48[0] = 0x13;
  local_44 = 3;
  local_3f = (undefined1)(param_1 >> 8);
  local_3e = (undefined1)param_1;
  local_28 = param_5;
  local_27 = 1;
  FUN_bfe02604(local_48);
  do {
    FUN_bfe0284c(local_24);
  } while ((local_24[0] & 1) != 0);
  FUN_bfe00e3c(local_48,0,0x24);
  local_48[0] = 3;
  if (param_6 == '\x02') {
    local_48[0] = 0x3b;
  }
  local_44 = 2;
  local_40 = (byte)((uint)param_2 >> 8) | (byte)(param_1 >> 2) & 0x10;
  local_3f = (undefined1)param_2;
  local_28 = param_5;
  local_27 = param_6;
  local_3c = 1;
  local_30 = param_3;
  local_2c = param_4;
  FUN_bfe02604(local_48);
  return;
}



/* ===== FUNCTION sub_bfe029c4 @ bfe029c4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe029c4(uint param_1,int param_2,uint param_3,undefined4 param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  byte in_stack_00000020;
  uint in_stack_00000028;
  uint uStack00000058;
  int iStack0000005c;
  char cStack_20;
  char cStack_1f;
  
  uVar1 = DAT_bfe00018;
  uVar6 = DAT_bfe0001c / DAT_bfe00018;
  if (DAT_bfe00018 == 0) {
    trap(7);
  }
  uStack00000058 = param_1;
  iStack0000005c = param_2;
  FUN_bfe00e3c(&cStack_20,0,0x24);
  cStack_20 = 'f';
  in_stack_00000020 = 0;
  FUN_bfe02604(&cStack_20);
  do {
    FUN_bfe0284c(&stack0x00000020);
  } while ((in_stack_00000020 & 1) != 0);
  cStack_20 = -0x67;
  FUN_bfe02604(&cStack_20);
  do {
    FUN_bfe0284c(&stack0x00000020);
  } while ((in_stack_00000020 & 1) != 0);
  uVar7 = (param_3 & 0x3fffff) / uVar1;
  if (uVar1 == 0) {
    trap(7);
  }
  iVar5 = 0x10;
  uVar3 = 0;
  uVar4 = 0;
  do {
    while( true ) {
      if (uStack00000058 <= uVar3) {
        return;
      }
      if (uVar6 == 0) {
        trap(7);
      }
      if ((int)uVar4 % (int)uVar6 == 0) break;
LAB_bfe02b60:
      if (uVar7 == 0) {
        iVar2 = iStack0000005c + uVar3;
        uVar3 = uVar3 + uVar1;
        in_stack_00000028 = uVar4 + 1;
        FUN_bfe028b0(uVar4 & 0xffff,0,uVar1 & 0xffff,iVar2,1,param_4);
        uVar4 = in_stack_00000028;
      }
      else {
        uVar7 = uVar7 - 1;
        uVar4 = uVar4 + 1;
      }
    }
    FUN_bfe00e3c(&cStack_20,0,0x40);
    FUN_bfe028b0(uVar4 & 0xffff,uVar1 & 0xffff,0x40,&cStack_20,0,param_4);
    if ((cStack_20 == -1) && (cStack_1f == -1)) goto LAB_bfe02b60;
    iVar5 = iVar5 + -1;
    uVar4 = uVar4 + uVar6;
    if (iVar5 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  } while( true );
}



/* ===== FUNCTION FUN_bfe029c8 @ bfe029c8 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe029c8(uint param_1,int param_2,uint param_3,undefined4 param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char local_78;
  char local_77;
  undefined1 local_57;
  byte local_38 [8];
  uint local_30;
  
  uVar1 = DAT_bfe00018;
  uVar6 = DAT_bfe0001c / DAT_bfe00018;
  if (DAT_bfe00018 == 0) {
    trap(7);
  }
  FUN_bfe00e3c(&local_78,0,0x24);
  local_78 = 'f';
  local_38[0] = 0;
  local_57 = 1;
  FUN_bfe02604(&local_78);
  do {
    FUN_bfe0284c(local_38);
  } while ((local_38[0] & 1) != 0);
  local_78 = -0x67;
  FUN_bfe02604(&local_78);
  do {
    FUN_bfe0284c(local_38);
  } while ((local_38[0] & 1) != 0);
  uVar7 = (param_3 & 0x3fffff) / uVar1;
  if (uVar1 == 0) {
    trap(7);
  }
  iVar5 = 0x10;
  uVar3 = 0;
  uVar4 = 0;
  do {
    while( true ) {
      if (param_1 <= uVar3) {
        return;
      }
      if (uVar6 == 0) {
        trap(7);
      }
      if ((int)uVar4 % (int)uVar6 == 0) break;
LAB_bfe02b60:
      if (uVar7 == 0) {
        iVar2 = param_2 + uVar3;
        uVar3 = uVar3 + uVar1;
        local_30 = uVar4 + 1;
        FUN_bfe028b0(uVar4 & 0xffff,0,uVar1 & 0xffff,iVar2,1,param_4);
        uVar4 = local_30;
      }
      else {
        uVar7 = uVar7 - 1;
        uVar4 = uVar4 + 1;
      }
    }
    FUN_bfe00e3c(&local_78,0,0x40);
    FUN_bfe028b0(uVar4 & 0xffff,uVar1 & 0xffff,0x40,&local_78,0,param_4);
    if ((local_78 == -1) && (local_77 == -1)) goto LAB_bfe02b60;
    iVar5 = iVar5 + -1;
    uVar4 = uVar4 + uVar6;
    if (iVar5 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  } while( true );
}



/* ===== FUNCTION sub_bfe029cc @ bfe029cc =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe029cc(uint param_1,int param_2,uint param_3,undefined4 param_4)

{
  int in_v0;
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  char local_78;
  char cStack_77;
  undefined1 local_57;
  byte local_38 [8];
  uint uStack_30;
  
  uVar2 = *(uint *)(in_v0 + 0x18);
  uVar6 = *(uint *)(in_v0 + 0x1c) / uVar2;
  if (uVar2 == 0) {
    trap(7);
  }
  FUN_bfe00e3c(&local_78,0,0x24);
  local_78 = 'f';
  local_38[0] = 0;
  local_57 = 1;
  FUN_bfe02604(&local_78);
  do {
    FUN_bfe0284c(local_38);
  } while ((local_38[0] & 1) != 0);
  local_78 = -0x67;
  FUN_bfe02604(&local_78);
  do {
    FUN_bfe0284c(local_38);
  } while ((local_38[0] & 1) != 0);
  uVar7 = (param_3 & 0x3fffff) / uVar2;
  if (uVar2 == 0) {
    trap(7);
  }
  iVar5 = 0x10;
  uVar3 = 0;
  uVar4 = 0;
  do {
    while( true ) {
      if (param_1 <= uVar3) {
        return;
      }
      if (uVar6 == 0) {
        trap(7);
      }
      if ((int)uVar4 % (int)uVar6 == 0) break;
LAB_bfe02b60:
      if (uVar7 == 0) {
        iVar1 = param_2 + uVar3;
        uVar3 = uVar3 + uVar2;
        uStack_30 = uVar4 + 1;
        FUN_bfe028b0(uVar4 & 0xffff,0,uVar2 & 0xffff,iVar1,1,param_4);
        uVar4 = uStack_30;
      }
      else {
        uVar7 = uVar7 - 1;
        uVar4 = uVar4 + 1;
      }
    }
    FUN_bfe00e3c(&local_78,0,0x40);
    FUN_bfe028b0(uVar4 & 0xffff,uVar2 & 0xffff,0x40,&local_78,0,param_4);
    if ((local_78 == -1) && (cStack_77 == -1)) goto LAB_bfe02b60;
    iVar5 = iVar5 + -1;
    uVar4 = uVar4 + uVar6;
    if (iVar5 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
  } while( true );
}



/* ===== FUNCTION sub_bfe02af0 @ bfe02af0 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe02af0(void)

{
  int iVar1;
  int unaff_s0;
  int unaff_s1;
  uint unaff_s3;
  uint unaff_s4;
  int unaff_s5;
  undefined4 unaff_s6;
  uint unaff_s7;
  int unaff_s8;
  byte bStack000000a8;
  byte bStack000000a9;
  uint in_stack_000000f0;
  uint in_stack_00000120;
  int in_stack_00000124;
  
  do {
    if (unaff_s5 == 0) {
      trap(7);
    }
    if ((int)unaff_s4 % unaff_s5 == 0) {
      FUN_bfe00e3c(&stack0x000000a8,0,0x40);
      FUN_bfe028b0(unaff_s4 & 0xffff,unaff_s6,0x40,&stack0x000000a8);
      if ((bStack000000a8 == unaff_s7) && (bStack000000a9 == unaff_s7)) goto LAB_bfe02b60;
      unaff_s8 = unaff_s8 + -1;
      unaff_s4 = unaff_s4 + unaff_s5;
      if (unaff_s8 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
    }
    else {
LAB_bfe02b60:
      if (unaff_s0 == 0) {
        iVar1 = in_stack_00000124 + unaff_s3;
        unaff_s3 = unaff_s3 + unaff_s1;
        in_stack_000000f0 = unaff_s4 + 1;
        FUN_bfe028b0(unaff_s4 & 0xffff,0,unaff_s6,iVar1);
        unaff_s4 = in_stack_000000f0;
      }
      else {
        unaff_s0 = unaff_s0 + -1;
        unaff_s4 = unaff_s4 + 1;
      }
    }
    if (in_stack_00000120 <= unaff_s3) {
      return;
    }
  } while( true );
}



/* ===== FUNCTION sub_bfe02af4 @ bfe02af4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe02af4(void)

{
  int iVar1;
  int unaff_s0;
  int unaff_s1;
  uint unaff_s3;
  uint unaff_s4;
  int unaff_s5;
  undefined4 unaff_s6;
  uint unaff_s7;
  int unaff_s8;
  byte bStack00000018;
  byte bStack00000019;
  uint in_stack_00000060;
  uint in_stack_00000090;
  int in_stack_00000094;
  
  do {
    if (unaff_s5 == 0) {
      trap(7);
    }
    if ((int)unaff_s4 % unaff_s5 == 0) {
      FUN_bfe00e3c(&stack0x00000018,0,0x40);
      FUN_bfe028b0(unaff_s4 & 0xffff,unaff_s6,0x40,&stack0x00000018);
      if ((bStack00000018 == unaff_s7) && (bStack00000019 == unaff_s7)) goto LAB_bfe02b60;
      unaff_s8 = unaff_s8 + -1;
      unaff_s4 = unaff_s4 + unaff_s5;
      if (unaff_s8 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
    }
    else {
LAB_bfe02b60:
      if (unaff_s0 == 0) {
        iVar1 = in_stack_00000094 + unaff_s3;
        unaff_s3 = unaff_s3 + unaff_s1;
        in_stack_00000060 = unaff_s4 + 1;
        FUN_bfe028b0(unaff_s4 & 0xffff,0,unaff_s6,iVar1);
        unaff_s4 = in_stack_00000060;
      }
      else {
        unaff_s0 = unaff_s0 + -1;
        unaff_s4 = unaff_s4 + 1;
      }
    }
    if (in_stack_00000090 <= unaff_s3) {
      return;
    }
  } while( true );
}



/* ===== FUNCTION FUN_bfe02ba4 @ bfe02ba4 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void FUN_bfe02ba4(int param_1,int param_2)

{
  byte bVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 4);
  if (param_2 == 0) {
    *(undefined1 *)(iVar2 + 0x380) = 0x80;
    FUN_bfe013a4(900);
    bVar1 = *(byte *)(iVar2 + 0x380) & 0xef | 0x8c;
  }
  else {
    if (param_2 != 1) goto LAB_bfe02bd8;
    *(undefined1 *)(iVar2 + 0x380) = 0x90;
    FUN_bfe013a4(900);
    bVar1 = *(byte *)(iVar2 + 0x380) | 0x9c;
  }
  *(byte *)(iVar2 + 0x380) = bVar1;
LAB_bfe02bd8:
  *(byte *)(iVar2 + 0x60) = *(byte *)(iVar2 + 0x60) & 0xfe;
  FUN_bfe013a4(900);
  *(byte *)(iVar2 + 0x60) = *(byte *)(iVar2 + 0x60) | 1;
  *(byte *)(iVar2 + 1) = *(byte *)(iVar2 + 1) | 0x40;
  FUN_bfe013a4(900);
  return;
}



/* ===== FUNCTION sub_bfe02c18 @ bfe02c18 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe02c18(undefined4 param_1,int param_2)

{
  int unaff_s0;
  
  if (param_2 == 1) {
    *(undefined1 *)(unaff_s0 + 0x380) = 0x90;
    FUN_bfe013a4(900);
    *(byte *)(unaff_s0 + 0x380) = *(byte *)(unaff_s0 + 0x380) | 0x9c;
  }
  *(byte *)(unaff_s0 + 0x60) = *(byte *)(unaff_s0 + 0x60) & 0xfe;
  FUN_bfe013a4(900);
  *(byte *)(unaff_s0 + 0x60) = *(byte *)(unaff_s0 + 0x60) | 1;
  *(byte *)(unaff_s0 + 1) = *(byte *)(unaff_s0 + 1) | 0x40;
  FUN_bfe013a4(900);
  return;
}



/* ===== FUNCTION FUN_bfe02c40 @ bfe02c40 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0xbfe02c9c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_bfe02c40(void)

{
  ushort uVar1;
  ushort uVar2;
  
  uVar1 = CONCAT11(DAT_bfe00049,DAT_bfe00048);
  uVar2 = CONCAT11(DAT_bfe0004b,DAT_bfe0004a);
  FUN_bfe01c90(3000000);
  _DAT_b8800380 = ((uint)uVar2 * 10 - 0x18) / 0x18 << 0x10 | _DAT_b8800380 & 0xffff | 0x80000000;
  if (uVar1 == 7) {
    _DAT_b880007c = _DAT_b880007c | 0x80;
  }
  _DAT_b8800074 = (uVar1 & 7) << 8 | _DAT_b8800074 & 0xfffff8ff | 0x400000;
  FUN_bfe01cc4();
  return;
}



/* ===== FUNCTION sub_bfe02c48 @ bfe02c48 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Removing unreachable block (ram,0xbfe02c9c) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe02c48(void)

{
  ushort uVar1;
  ushort uVar2;
  int in_v1;
  
  uVar1 = *(ushort *)(in_v1 + 0x48);
  uVar2 = *(ushort *)(in_v1 + 0x4a);
  FUN_bfe01c90(3000000);
  _DAT_b8800380 = ((uint)uVar2 * 10 - 0x18) / 0x18 << 0x10 | _DAT_b8800380 & 0xffff | 0x80000000;
  if (uVar1 == 7) {
    _DAT_b880007c = _DAT_b880007c | 0x80;
  }
  _DAT_b8800074 = (uVar1 & 7) << 8 | _DAT_b8800074 & 0xfffff8ff | 0x400000;
  FUN_bfe01cc4();
  return;
}



/* ===== FUNCTION sub_bfe02d1c @ bfe02d1c =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sub_bfe02d1c(uint param_1)

{
  _DAT_b882e004 = _DAT_b882e004 & 0xf0ffffff;
  if (param_1 < 98000000) {
    if (param_1 < 54000000) {
      if (param_1 < 27000000) {
        return;
      }
      _DAT_b882e098 = _DAT_b882e098 & 0xf0ffffff | 0x1000000;
      return;
    }
  }
  else {
    _DAT_b880007c = _DAT_b880007c & 0xffff3fff | 0x8000;
  }
  _DAT_b882e098 = _DAT_b882e098 & 0xf0ffffff;
  return;
}



/* ===== FUNCTION first_stage_clock_or_pll_select @ bfe02d20 =====
 * Medium confidence: adjusts 0xb882e004/0xb882e094 and 0xb880007c based on a frequency-like argument.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Medium confidence: adjusts 0xb882e004/0xb882e094 and 0xb880007c based on a frequency-like
   argument. */

void first_stage_clock_or_pll_select(uint param_1)

{
  _DAT_b882e004 = _DAT_b882e004 & 0xf0ffffff;
  if (param_1 < 98000000) {
    if (param_1 < 54000000) {
      if (param_1 < 27000000) {
        return;
      }
      _DAT_b882e098 = _DAT_b882e098 & 0xf0ffffff | 0x1000000;
      return;
    }
  }
  else {
    _DAT_b880007c = _DAT_b880007c & 0xffff3fff | 0x8000;
  }
  _DAT_b882e098 = _DAT_b882e098 & 0xf0ffffff;
  return;
}



/* ===== FUNCTION sub_bfe02d80 @ bfe02d80 =====
 * No strong automatic classification; name and body are generated from raw MIPS control flow.
 */

void sub_bfe02d80(uint param_1,uint param_2)

{
  int in_v0;
  uint in_v1;
  uint uVar1;
  
  if (param_1 < (param_2 | 0xf980)) {
    if (param_1 < 27000000) {
      return;
    }
    uVar1 = *(uint *)(in_v0 + 0x94) & in_v1 | 0x1000000;
  }
  else {
    uVar1 = *(uint *)(in_v0 + 0x94) & in_v1;
  }
  *(uint *)(in_v0 + 0x94) = uVar1;
  return;
}



/* ===== FUNCTION first_stage_sfspi_transfer_setup @ bfe02db4 =====
 * Medium confidence: programs the SPI/SF controller registers around 0xb882e058 and starts a transfer/status wait loop.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Medium confidence: programs the SPI/SF controller registers around 0xb882e058 and starts a
   transfer/status wait loop. */

void first_stage_sfspi_transfer_setup(uint param_1,undefined4 param_2)

{
  _DAT_b882e058 = param_1 & 0xfffffff;
  _DAT_b882e060 = param_2;
  _DAT_b882e064 = _DAT_b882e064 | 0x1e0020;
  _DAT_b882e0a0 = _DAT_b882e0a0 | 1;
  return;
}


