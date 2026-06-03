
/* ===== FUNCTION _init @ 00013480 ===== */

int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  __gmon_start__();
  (*(code *)0x36c4)();
  iVar1 = __do_global_ctors_aux();
  return iVar1;
}



/* ===== FUNCTION _GLOBAL__sub_I_libretro.cpp @ 00013510 ===== */

void processEntry _GLOBAL__sub_I_libretro_cpp(void)

{
  my_av_info = malloc(0x28);
  return;
}



/* ===== FUNCTION deregister_tm_clones @ 00013550 ===== */

/* WARNING: Removing unreachable block (ram,0x0001357c) */
/* WARNING: Removing unreachable block (ram,0x00013584) */

void deregister_tm_clones(void)

{
  return;
}



/* ===== FUNCTION register_tm_clones @ 00013594 ===== */

/* WARNING: Removing unreachable block (ram,0x000135c8) */
/* WARNING: Removing unreachable block (ram,0x000135d0) */

void register_tm_clones(void)

{
  return;
}



/* ===== FUNCTION __do_global_dtors_aux @ 000135e0 ===== */

/* WARNING: Removing unreachable block (ram,0x00013664) */
/* WARNING: Removing unreachable block (ram,0x00013690) */

void __do_global_dtors_aux(void)

{
  if (completed_6286 == '\0') {
    __cxa_finalize(__dso_handle);
    deregister_tm_clones();
    completed_6286 = '\x01';
  }
  return;
}



/* ===== FUNCTION frame_dummy @ 000136c4 ===== */

void frame_dummy(void)

{
  if (__JCR_END__ != 0) {
    _Jv_RegisterClasses();
  }
  register_tm_clones();
  return;
}



/* ===== FUNCTION ~apu @ 00013720 ===== */

/* apu::~apu() */

void __thiscall apu::~apu(apu *this)

{
  return;
}



/* ===== FUNCTION read @ 00013728 ===== */

/* apu::read(unsigned short) */

byte __thiscall apu::read(apu *this,ushort param_1)

{
  byte bVar1;
  int iVar2;
  byte bVar3;
  undefined2 in_register_00000016;
  byte bVar4;
  
  if (CONCAT22(in_register_00000016,param_1) != 0xff26) {
    return *(byte *)(*(int *)(this + 4) + CONCAT22(in_register_00000016,param_1) + 0x703c6);
  }
  iVar2 = *(int *)(this + 4);
  if (*(int *)(iVar2 + 0xb0) == 0) {
    return 0;
  }
  if (*(char *)(iVar2 + 4) == '\0') {
    bVar4 = 0x80;
    if (*(char *)(iVar2 + 0x3c) != '\0') {
      if (*(int *)(iVar2 + 0x6c) != 0) {
        bVar4 = 0x82;
      }
      goto LAB_00013778;
    }
  }
  else {
    bVar4 = 0x81;
    if (*(int *)(iVar2 + 0x6c) != 0) {
      if (*(char *)(iVar2 + 0x3c) != '\0') {
        bVar4 = 0x83;
      }
      goto LAB_00013778;
    }
  }
  bVar4 = 0x80;
LAB_00013778:
  bVar3 = 0;
  if ((*(int *)(iVar2 + 0xec) != 0) && (*(char *)(iVar2 + 0x68) != '\0')) {
    bVar3 = (*(int *)(iVar2 + 0x6c) != 0) << 2;
  }
  if (*(char *)(iVar2 + 0x84) == '\0') {
    bVar1 = 0;
  }
  else {
    bVar1 = (*(int *)(iVar2 + 0x90) != 0) << 3;
  }
  return bVar1 | bVar3 | bVar4;
}



/* ===== FUNCTION update @ 00013804 ===== */

/* apu::update() */

void apu::update(void)

{
  return;
}



/* ===== FUNCTION get_stat @ 0001380c ===== */

/* apu::get_stat() */

int __thiscall apu::get_stat(apu *this)

{
  return *(int *)(this + 4) + 4;
}



/* ===== FUNCTION get_stat_cpy @ 00013818 ===== */

/* apu::get_stat_cpy() */

int __thiscall apu::get_stat_cpy(apu *this)

{
  return *(int *)(this + 4) + 0xf0;
}



/* ===== FUNCTION get_mem @ 00013824 ===== */

/* apu::get_mem() */

int __thiscall apu::get_mem(apu *this)

{
  return *(int *)(this + 4) + 0x802d6;
}



/* ===== FUNCTION apu_snd @ 00013838 ===== */

/* apu_snd::apu_snd(apu*) */

void __thiscall apu_snd::apu_snd(apu_snd *this,apu *param_1)

{
  *(undefined ***)this = &PTR_render_000481b8;
  this[0x803d9] = (apu_snd)0x1;
  this[0x803d8] = (apu_snd)0x1;
  this[0x803d7] = (apu_snd)0x1;
  this[0x803d6] = (apu_snd)0x1;
  this[0x802d5] = (apu_snd)0x0;
  this[0x802d4] = (apu_snd)0x0;
  *(apu **)(this + 0x802d0) = param_1;
  return;
}



/* ===== FUNCTION ~apu_snd @ 0001387c ===== */

/* apu_snd::~apu_snd() */

void __thiscall apu_snd::~apu_snd(apu_snd *this)

{
  return;
}



/* ===== FUNCTION reset @ 00013884 ===== */

/* apu_snd::reset() */

void __thiscall apu_snd::reset(apu_snd *this)

{
  uint uVar1;
  uint uVar2;
  apu_snd *paVar3;
  apu_snd *paVar4;
  int iVar5;
  undefined4 uVar6;
  apu_snd *paVar7;
  apu_snd *paVar8;
  undefined4 uVar9;
  uint uVar10;
  
  *(undefined4 *)(this + 0x802c8) = 0;
  *(undefined4 *)(this + 0x802cc) = 0;
  memset(this + 4,0,0xec);
  this[4] = (apu_snd)0x0;
  this[0x3c] = (apu_snd)0x0;
  this[0x68] = (apu_snd)0x0;
  this[0x84] = (apu_snd)0x0;
  *(undefined4 *)(this + 0xb0) = 1;
  *(undefined4 *)(this + 0xb4) = 1;
  *(undefined4 *)(this + 0xb8) = 1;
  *(undefined4 *)(this + 0xbc) = 1;
  *(undefined4 *)(this + 0xc0) = 1;
  *(undefined4 *)(this + 0xc4) = 1;
  *(undefined4 *)(this + 200) = 1;
  *(undefined4 *)(this + 0xcc) = 1;
  *(undefined4 *)(this + 0xd0) = 1;
  *(undefined4 *)(this + 0xd4) = 7;
  *(undefined4 *)(this + 0xd8) = 7;
  *(undefined4 *)(this + 0xdc) = 1;
  *(undefined4 *)(this + 0xe0) = 1;
  *(undefined4 *)(this + 0xe4) = 1;
  *(undefined4 *)(this + 0xe8) = 1;
  paVar3 = this + 0xf0;
  paVar8 = this + 4;
  do {
    paVar7 = paVar8;
    paVar4 = paVar3;
    uVar10 = *(uint *)(paVar7 + 4);
    uVar9 = *(undefined4 *)(paVar7 + 8);
    uVar6 = *(undefined4 *)(paVar7 + 0xc);
    paVar8 = paVar7 + 0x10;
    *(undefined4 *)paVar4 = *(undefined4 *)paVar7;
    *(uint *)(paVar4 + 4) = uVar10;
    *(undefined4 *)(paVar4 + 8) = uVar9;
    *(undefined4 *)(paVar4 + 0xc) = uVar6;
    paVar3 = paVar4 + 0x10;
  } while (paVar8 != this + 0xe4);
  uVar1 = (uint)(paVar7 + 0x13) & 3;
  uVar2 = (uint)paVar8 & 3;
  uVar9 = *(undefined4 *)(paVar7 + 0x14);
  uVar6 = *(undefined4 *)(paVar7 + 0x18);
  *(uint *)(paVar4 + 0x10) =
       (*(int *)(paVar7 + 0x13 + -uVar1) << (3 - uVar1) * 8 |
       uVar10 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar2) * 8 |
       *(uint *)(paVar8 + -uVar2) >> uVar2 * 8;
  *(undefined4 *)(paVar4 + 0x14) = uVar9;
  *(undefined4 *)(paVar4 + 0x18) = uVar6;
  iVar5 = *(int *)(*(int *)(**(int **)(this + 0x802d0) + 0xc) + 0x20);
  if (iVar5 != 1) {
    if (2 < iVar5) {
      this[0x802ea] = (apu_snd)0x0;
      this[0x802ec] = (apu_snd)0x0;
      this[0x802eb] = (apu_snd)0xff;
      this[0x802ed] = (apu_snd)0xff;
      this[0x802ee] = (apu_snd)0x0;
      this[0x802ef] = (apu_snd)0xff;
      this[0x802f0] = (apu_snd)0x0;
      this[0x802f1] = (apu_snd)0xff;
      this[0x802f2] = (apu_snd)0x0;
      this[0x802f3] = (apu_snd)0xff;
      this[0x802f4] = (apu_snd)0x0;
      this[0x802f5] = (apu_snd)0xff;
      this[0x802f6] = (apu_snd)0x0;
      this[0x802f7] = (apu_snd)0xff;
      this[0x802f8] = (apu_snd)0x0;
      this[0x802f9] = (apu_snd)0xff;
    }
    return;
  }
  this[0x802ee] = (apu_snd)0x0;
  this[0x802f2] = (apu_snd)0x0;
  this[0x802ea] = (apu_snd)0x6;
  this[0x802f4] = (apu_snd)0x0;
  this[0x802eb] = (apu_snd)0xfe;
  this[0x802ec] = (apu_snd)0xe;
  this[0x802ed] = (apu_snd)0x7f;
  this[0x802ef] = (apu_snd)0xff;
  this[0x802f0] = (apu_snd)0x58;
  this[0x802f1] = (apu_snd)0xdf;
  this[0x802f3] = (apu_snd)0xec;
  this[0x802f5] = (apu_snd)0xbf;
  this[0x802f6] = (apu_snd)0xc;
  this[0x802f7] = (apu_snd)0xed;
  this[0x802f8] = (apu_snd)0x3;
  this[0x802f9] = (apu_snd)0xf7;
  return;
}



/* ===== FUNCTION reset @ 00013ab4 ===== */

/* apu::reset() */

void __thiscall apu::reset(apu *this)

{
  uint *puVar1;
  uint *puVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  
  iVar8 = *(int *)(this + 4);
  *(undefined4 *)(iVar8 + 0x802c8) = 0;
  *(undefined4 *)(iVar8 + 0x802cc) = 0;
  memset((uint *)(iVar8 + 4),0,0xec);
  *(undefined1 *)(iVar8 + 4) = 0;
  *(undefined1 *)(iVar8 + 0x3c) = 0;
  *(undefined1 *)(iVar8 + 0x68) = 0;
  *(undefined1 *)(iVar8 + 0x84) = 0;
  *(undefined4 *)(iVar8 + 0xb0) = 1;
  *(undefined4 *)(iVar8 + 0xb4) = 1;
  *(undefined4 *)(iVar8 + 0xb8) = 1;
  *(undefined4 *)(iVar8 + 0xbc) = 1;
  *(undefined4 *)(iVar8 + 0xc0) = 1;
  *(undefined4 *)(iVar8 + 0xc4) = 1;
  *(undefined4 *)(iVar8 + 200) = 1;
  *(undefined4 *)(iVar8 + 0xcc) = 1;
  *(undefined4 *)(iVar8 + 0xd0) = 1;
  *(undefined4 *)(iVar8 + 0xd4) = 7;
  *(undefined4 *)(iVar8 + 0xd8) = 7;
  *(undefined4 *)(iVar8 + 0xdc) = 1;
  *(undefined4 *)(iVar8 + 0xe0) = 1;
  *(undefined4 *)(iVar8 + 0xe4) = 1;
  *(undefined4 *)(iVar8 + 0xe8) = 1;
  puVar1 = (uint *)(iVar8 + 0xf0);
  puVar7 = (uint *)(iVar8 + 4);
  do {
    puVar6 = puVar7;
    puVar2 = puVar1;
    uVar11 = puVar6[1];
    uVar9 = puVar6[2];
    uVar4 = puVar6[3];
    puVar7 = puVar6 + 4;
    *puVar2 = *puVar6;
    puVar2[1] = uVar11;
    puVar2[2] = uVar9;
    puVar2[3] = uVar4;
    puVar1 = puVar2 + 4;
  } while (puVar7 != (uint *)(iVar8 + 0xe4U));
  uVar4 = (int)puVar6 + 0x13U & 3;
  uVar9 = (uint)puVar7 & 3;
  uVar10 = puVar6[5];
  uVar5 = puVar6[6];
  puVar2[4] = (*(int *)(((int)puVar6 + 0x13U) - uVar4) << (3 - uVar4) * 8 |
              uVar11 & 0xffffffffU >> (uVar4 + 1) * 8) & -1 << (4 - uVar9) * 8 |
              *(uint *)((int)puVar7 - uVar9) >> uVar9 * 8;
  puVar2[5] = uVar10;
  puVar2[6] = uVar5;
  iVar3 = *(int *)(*(int *)(**(int **)(iVar8 + 0x802d0) + 0xc) + 0x20);
  if (iVar3 == 1) {
    *(undefined1 *)(iVar8 + 0x802ee) = 0;
    *(undefined1 *)(iVar8 + 0x802f2) = 0;
    *(undefined1 *)(iVar8 + 0x802ea) = 6;
    *(undefined1 *)(iVar8 + 0x802f4) = 0;
    *(undefined1 *)(iVar8 + 0x802eb) = 0xfe;
    *(undefined1 *)(iVar8 + 0x802ec) = 0xe;
    *(undefined1 *)(iVar8 + 0x802ed) = 0x7f;
    *(undefined1 *)(iVar8 + 0x802ef) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f0) = 0x58;
    *(undefined1 *)(iVar8 + 0x802f1) = 0xdf;
    *(undefined1 *)(iVar8 + 0x802f3) = 0xec;
    *(undefined1 *)(iVar8 + 0x802f5) = 0xbf;
    *(undefined1 *)(iVar8 + 0x802f6) = 0xc;
    *(undefined1 *)(iVar8 + 0x802f7) = 0xed;
    *(undefined1 *)(iVar8 + 0x802f8) = 3;
    *(undefined1 *)(iVar8 + 0x802f9) = 0xf7;
    return;
  }
  if (2 < iVar3) {
    *(undefined1 *)(iVar8 + 0x802ea) = 0;
    *(undefined1 *)(iVar8 + 0x802ec) = 0;
    *(undefined1 *)(iVar8 + 0x802eb) = 0xff;
    *(undefined1 *)(iVar8 + 0x802ed) = 0xff;
    *(undefined1 *)(iVar8 + 0x802ee) = 0;
    *(undefined1 *)(iVar8 + 0x802ef) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f0) = 0;
    *(undefined1 *)(iVar8 + 0x802f1) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f2) = 0;
    *(undefined1 *)(iVar8 + 0x802f3) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f4) = 0;
    *(undefined1 *)(iVar8 + 0x802f5) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f6) = 0;
    *(undefined1 *)(iVar8 + 0x802f7) = 0xff;
    *(undefined1 *)(iVar8 + 0x802f8) = 0;
    *(undefined1 *)(iVar8 + 0x802f9) = 0xff;
  }
  return;
}



/* ===== FUNCTION apu @ 00013acc ===== */

/* apu::apu(gb*) */

void __thiscall apu::apu(apu *this,gb *param_1)

{
  apu_snd *this_00;
  
  *(gb **)this = param_1;
  this_00 = operator_new(0x803dc);
                    /* try { // try from 00013b14 to 00013b1b has its CatchHandler @ 00013b40 */
  apu_snd::apu_snd(this_00,this);
  *(apu_snd **)(this + 4) = this_00;
                    /* WARNING: Could not recover jumptable at 0x00013b38. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  reset(this);
  return;
}



/* ===== FUNCTION set_enable @ 00013b64 ===== */

/* apu_snd::set_enable(int, bool) */

void __thiscall apu_snd::set_enable(apu_snd *this,int param_1,bool param_2)

{
  this[param_1 + 0x803d6] = (apu_snd)param_2;
  return;
}



/* ===== FUNCTION get_enable @ 00013b78 ===== */

/* apu_snd::get_enable(int) */

apu_snd __thiscall apu_snd::get_enable(apu_snd *this,int param_1)

{
  return this[param_1 + 0x803d6];
}



/* ===== FUNCTION process @ 00013b8c ===== */

/* apu_snd::process(unsigned short, unsigned char) */

void __thiscall apu_snd::process(apu_snd *this,ushort param_1,uchar param_2)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  undefined2 in_register_00000016;
  undefined3 in_register_00000019;
  uint *puVar5;
  int iVar6;
  uint uVar7;
  undefined4 uVar8;
  uint uVar9;
  uint uVar10;
  uint local_90 [16];
  int local_50 [20];
  
  uVar4 = CONCAT31(in_register_00000019,param_2);
  local_50[0] = 1;
  local_50[1] = 1;
  puVar3 = &DAT_00033a00;
  local_50[9] = 1;
  local_50[10] = 1;
  local_50[0xb] = 1;
  local_50[0xc] = 1;
  local_50[0xd] = 1;
  local_50[0xe] = 1;
  local_50[0xf] = 1;
  local_50[0x13] = 1;
  local_50[2] = 2;
  local_50[3] = 3;
  local_50[4] = 4;
  local_50[5] = 5;
  local_50[8] = 2;
  local_50[6] = 6;
  local_50[0x10] = 0;
  local_50[0x11] = 4;
  local_50[0x12] = 2;
  local_50[7] = 7;
  puVar5 = local_90;
  do {
    uVar10 = *puVar3;
    uVar9 = puVar3[1];
    uVar7 = puVar3[2];
    uVar1 = puVar3[3];
    puVar3 = puVar3 + 4;
    *puVar5 = uVar10;
    puVar5[1] = uVar9;
    puVar5[2] = uVar7;
    puVar5[3] = uVar1;
    puVar5 = puVar5 + 4;
  } while (puVar3 != &switchD_00013c84::switchdataD_00033a40);
  uVar1 = CONCAT22(in_register_00000016,param_1) + 0xf0U & 0xffff;
  this[CONCAT22(in_register_00000016,param_1) + 0x703c6] = (apu_snd)param_2;
  if (uVar1 < 0x17) {
    uVar7 = (int)uVar4 >> 3;
    uVar10 = (int)uVar4 >> 4;
    uVar9 = (int)uVar4 >> 6;
    switch(uVar1) {
    case 0:
      *(uint *)(this + 8) = uVar10 & 7;
      *(uint *)(this + 0xc) = uVar7 & 1;
      *(uint *)(this + 0x10) = param_2 & 7;
      break;
    case 1:
      iVar2 = 0x40 - (param_2 & 0x3f);
      *(int *)(this + 0x14) = iVar2;
      *(int *)(this + 0x18) = iVar2;
      *(uint *)(this + 0x1c) = uVar4 >> 6;
      break;
    case 2:
      *(uint *)(this + 0x20) = uVar4 >> 4;
      *(uint *)(this + 0x24) = uVar4 >> 4;
      *(uint *)(this + 0x28) = uVar7 & 1;
      *(uint *)(this + 0x2c) = param_2 & 7;
      break;
    case 3:
      uVar4 = *(uint *)(this + 0x34) & 0x700 | uVar4;
      *(uint *)(this + 0x30) = uVar4;
      *(uint *)(this + 0x34) = uVar4;
      break;
    case 4:
      uVar4 = (uint)(byte)this[0x34] | (param_2 & 7) << 8;
      *(uint *)(this + 0x30) = uVar4;
      *(uint *)(this + 0x34) = uVar4;
      *(uint *)(this + 0x38) = uVar9 & 1;
      if ((char)param_2 < '\0') {
        this[4] = (apu_snd)0x1;
        *(undefined4 *)(this + 0x14) = *(undefined4 *)(this + 0x18);
        *(int *)(this + 0x20) = *(int *)(this + 0x24);
        if (*(int *)(this + 0x24) == 0) {
          sq1_cur_pos = 0;
        }
      }
      break;
    case 6:
      iVar2 = 0x40 - (param_2 & 0x3f);
      *(int *)(this + 0x40) = iVar2;
      *(int *)(this + 0x44) = iVar2;
      *(uint *)(this + 0x48) = uVar4 >> 6;
      break;
    case 7:
      *(uint *)(this + 0x4c) = uVar4 >> 4;
      *(uint *)(this + 0x50) = uVar4 >> 4;
      *(uint *)(this + 0x54) = uVar7 & 1;
      *(uint *)(this + 0x58) = param_2 & 7;
      break;
    case 8:
      uVar4 = *(uint *)(this + 0x60) & 0x700 | uVar4;
      *(uint *)(this + 0x5c) = uVar4;
      *(uint *)(this + 0x60) = uVar4;
      break;
    case 9:
      uVar4 = (uint)(byte)this[0x60] | (param_2 & 7) << 8;
      *(uint *)(this + 0x5c) = uVar4;
      *(uint *)(this + 0x60) = uVar4;
      *(uint *)(this + 100) = uVar9 & 1;
      if ((char)param_2 < '\0') {
        if ((this[0x3c] == (apu_snd)0x0) || (*(int *)(this + 0x4c) == 0)) {
          sq2_cur_pos = 0;
        }
        this[0x3c] = (apu_snd)0x1;
        *(undefined4 *)(this + 0x40) = *(undefined4 *)(this + 0x44);
        *(undefined4 *)(this + 0x4c) = *(undefined4 *)(this + 0x50);
      }
      break;
    case 10:
      *(uint *)(this + 0xec) = uVar4 >> 7;
      break;
    case 0xb:
      *(uint *)(this + 0x78) = 0x100 - uVar4;
      *(uint *)(this + 0x7c) = 0x100 - uVar4;
      if ((uVar4 == 0) && (*(int *)(this + 0x80) != 0)) {
        this[0x68] = (apu_snd)0x0;
      }
      else {
        this[0x68] = (apu_snd)0x1;
      }
      break;
    case 0xc:
      *(undefined4 *)(this + 0x6c) = *(undefined4 *)((int)local_50 + ((param_2 & 0x60) >> 3) + 0x40)
      ;
      break;
    case 0xd:
      *(uint *)(this + 0x70) = *(uint *)(this + 0x70) & 0x700 | uVar4;
      break;
    case 0xe:
      *(uint *)(this + 0x70) = (uint)(byte)this[0x70] | (param_2 & 7) << 8;
      *(uint *)(this + 0x80) = uVar9 & 1;
      if ((char)param_2 < '\0') {
        this[0x68] = (apu_snd)0x1;
        *(undefined4 *)(this + 0x7c) = *(undefined4 *)(this + 0x78);
      }
      break;
    case 0x10:
      iVar2 = 0x40 - (param_2 & 0x3f);
      *(int *)(this + 0x88) = iVar2;
      *(int *)(this + 0x8c) = iVar2;
      break;
    case 0x11:
      uVar4 = uVar4 >> 4;
      *(uint *)(this + 0x90) = uVar4;
      *(uint *)(this + 0x94) = uVar4;
      *(uint *)(this + 0x98) = uVar7 & 1;
      *(uint *)(this + 0x9c) = param_2 & 7;
      if (uVar4 == 0) {
        this[0x84] = (apu_snd)0x0;
      }
      break;
    case 0x12:
      uVar8 = 0xf;
      iVar2 = local_50[(param_2 & 7) + 8];
      iVar6 = local_50[param_2 & 7];
      uVar4 = local_90[uVar4 >> 4];
      if ((param_2 & 8) != 0) {
        uVar8 = 7;
      }
      if (iVar6 == 0) {
        trap(7);
      }
      *(undefined4 *)(this + 0xac) = uVar8;
      if (uVar4 == 0) {
        trap(7);
      }
      iVar2 = ((iVar2 << 0x16) / iVar6) / (int)uVar4;
      if (iVar2 < 0) {
        iVar2 = iVar2 + 7;
      }
      *(int *)(this + 0xa0) = iVar2 >> 3;
      *(int *)(this + 0xa4) = iVar2 >> 3;
      if (uVar9 == 3) {
        this[0x84] = (apu_snd)0x0;
      }
      break;
    case 0x13:
      *(uint *)(this + 0xa8) = uVar9 & 1;
      if ((char)param_2 < '\0') {
        this[0x84] = (apu_snd)0x1;
        *(undefined4 *)(this + 0x88) = *(undefined4 *)(this + 0x8c);
        *(int *)(this + 0x90) = *(int *)(this + 0x94);
        if (*(int *)(this + 0x94) == 0) {
          noi_cur_pos = 0;
        }
      }
      break;
    case 0x14:
      *(uint *)(this + 0xd4) = param_2 & 7;
      *(uint *)(this + 0xd8) = uVar10 & 7;
      break;
    case 0x15:
      *(uint *)(this + 0xb4) = param_2 & 1;
      *(uint *)(this + 0xb8) = uVar10 & 1;
      *(uint *)(this + 0xbc) = (int)uVar4 >> 1 & 1;
      *(uint *)(this + 0xc4) = (int)uVar4 >> 2 & 1;
      *(uint *)(this + 200) = uVar9 & 1;
      *(uint *)(this + 0xcc) = uVar7 & 1;
      *(uint *)(this + 0xd0) = uVar4 >> 7;
      *(uint *)(this + 0xc0) = (int)uVar4 >> 5 & 1;
      break;
    case 0x16:
      *(uint *)(this + 0xb0) = uVar4 >> 7;
      *(uint *)(this + 0xdc) = param_2 & 1;
      *(uint *)(this + 0xe0) = (int)uVar4 >> 1 & 1;
      *(uint *)(this + 0xe4) = (int)uVar4 >> 2 & 1;
      *(uint *)(this + 0xe8) = uVar7 & 1;
    }
  }
  return;
}



/* ===== FUNCTION update @ 00014088 ===== */

/* apu_snd::update() */

void __thiscall apu_snd::update(apu_snd *this)

{
  apu_snd aVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  if (this[4] == (apu_snd)0x0) {
    if ((this[0x3c] == (apu_snd)0x0) || (*(int *)(this + 0xb0) == 0)) goto LAB_000140b4;
LAB_000140f8:
    iVar2 = *(int *)(this + 0x58) << 2;
    if (*(int *)(this + 0x58) != 0) {
      if (iVar2 == 0) {
        trap(7);
      }
      iVar4 = 1;
      if (update()::counter % iVar2 == 0) {
        if (*(int *)(this + 0x54) == 0) {
          iVar4 = -1;
        }
        iVar4 = iVar4 + *(int *)(this + 0x4c);
        if (iVar4 < 0) {
          *(undefined4 *)(this + 0x4c) = 0;
        }
        else {
          if (0xf < iVar4) {
            iVar4 = 0xf;
          }
          *(int *)(this + 0x4c) = iVar4;
        }
      }
    }
    if (((*(int *)(this + 100) != 0) &&
        (iVar2 = *(int *)(this + 0x40) + -1, *(int *)(this + 0x40) != 0)) &&
       (*(int *)(this + 0x40) = iVar2, iVar2 < 1)) {
      this[0x3c] = (apu_snd)0x0;
    }
    if (this[0x68] != (apu_snd)0x0) goto LAB_00014270;
LAB_00014174:
    if ((this[0x84] == (apu_snd)0x0) || (*(int *)(this + 0xb0) == 0)) goto LAB_000140d0;
  }
  else {
    if (*(int *)(this + 0xb0) == 0) {
LAB_000140b4:
      aVar1 = this[0x68];
    }
    else {
      if (*(int *)(this + 0x2c) != 0) {
        iVar2 = *(int *)(this + 0x2c) << 2;
        if (iVar2 == 0) {
          trap(7);
        }
        iVar4 = 1;
        if (update()::counter % iVar2 == 0) {
          if (*(int *)(this + 0x28) == 0) {
            iVar4 = -1;
          }
          iVar4 = iVar4 + *(int *)(this + 0x20);
          if (iVar4 < 0) {
            *(undefined4 *)(this + 0x20) = 0;
          }
          else {
            if (0xf < iVar4) {
              iVar4 = 0xf;
            }
            *(int *)(this + 0x20) = iVar4;
          }
        }
      }
      if (*(int *)(this + 8) != 0) {
        uVar3 = *(uint *)(this + 0x10);
        iVar2 = *(int *)(this + 8) << 1;
        if (uVar3 != 0) {
          if (iVar2 == 0) {
            trap(7);
          }
          if (update()::counter % iVar2 == 0) {
            if (*(int *)(this + 0xc) == 0) {
              *(int *)(this + 0x30) =
                   (*(int *)(this + 0x30) >> (uVar3 & 0x1f)) + *(int *)(this + 0x30);
            }
            else {
              *(int *)(this + 0x30) =
                   *(int *)(this + 0x30) - (*(int *)(this + 0x30) >> (uVar3 & 0x1f));
            }
          }
        }
      }
      if (((*(int *)(this + 0x38) != 0) &&
          (iVar2 = *(int *)(this + 0x14) + -1, *(int *)(this + 0x14) != 0)) &&
         (*(int *)(this + 0x14) = iVar2, iVar2 < 1)) {
        this[4] = (apu_snd)0x0;
      }
      if (this[0x3c] != (apu_snd)0x0) goto LAB_000140f8;
      aVar1 = this[0x68];
    }
    if (aVar1 == (apu_snd)0x0) goto LAB_00014174;
    if (*(int *)(this + 0xb0) == 0) goto LAB_000140d0;
LAB_00014270:
    if (((*(int *)(this + 0x80) != 0) &&
        (iVar2 = *(int *)(this + 0x7c) + -1, *(int *)(this + 0x7c) != 0)) &&
       (*(int *)(this + 0x7c) = iVar2, iVar2 < 1)) {
      this[0x68] = (apu_snd)0x0;
    }
    if (this[0x84] == (apu_snd)0x0) goto LAB_000140d0;
  }
  iVar2 = *(int *)(this + 0x9c) << 2;
  if (*(int *)(this + 0x9c) != 0) {
    if (iVar2 == 0) {
      trap(7);
    }
    iVar4 = 1;
    if (update()::counter % iVar2 == 0) {
      if (*(int *)(this + 0x98) == 0) {
        iVar4 = -1;
      }
      iVar4 = iVar4 + *(int *)(this + 0x90);
      if (iVar4 < 0) {
        *(undefined4 *)(this + 0x90) = 0;
      }
      else {
        if (0xf < iVar4) {
          iVar4 = 0xf;
        }
        *(int *)(this + 0x90) = iVar4;
      }
    }
  }
  if (((*(int *)(this + 0xa8) != 0) &&
      (iVar2 = *(int *)(this + 0x88) + -1, *(int *)(this + 0x88) != 0)) &&
     (*(int *)(this + 0x88) = iVar2, iVar2 < 1)) {
    this[0x84] = (apu_snd)0x0;
  }
LAB_000140d0:
  update()::counter = update()::counter + 1;
  return;
}



/* ===== FUNCTION write @ 0001434c ===== */

/* apu::write(unsigned short, unsigned char, int) */

void __thiscall apu::write(apu *this,ushort param_1,uchar param_2,int param_3)

{
  char cVar1;
  int iVar2;
  apu_snd *this_00;
  undefined2 in_register_00000016;
  
  SYNC(0);
  if ((write(unsigned_short,unsigned_char,int)::bef_clock == '\0') &&
     (iVar2 = __cxa_guard_acquire(&write(unsigned_short,unsigned_char,int)::bef_clock), iVar2 != 0))
  {
    write(unsigned_short,unsigned_char,int)::bef_clock = param_3;
    __cxa_guard_release(&write(unsigned_short,unsigned_char,int)::bef_clock);
  }
  this_00 = *(apu_snd **)(this + 4);
  this_00[CONCAT22(in_register_00000016,param_1) + 0x703c6] = (apu_snd)param_2;
  iVar2 = *(int *)(this_00 + 0x802c8);
  *(ushort *)(this_00 + iVar2 * 8 + 0x2c8) = param_1;
  this_00[iVar2 * 8 + 0x2ca] = (apu_snd)param_2;
  *(int *)(this_00 + 0x802c8) = iVar2 + 1;
  *(int *)(this_00 + iVar2 * 8 + 0x2cc) = param_3;
  if (0xffff < iVar2 + 1) {
    *(undefined4 *)(this_00 + 0x802c8) = 0xffff;
  }
  apu_snd::process(this_00,param_1,param_2);
  if (param_3 < write(unsigned_short,unsigned_char,int)::bef_clock) {
    iVar2 = 0;
    write(unsigned_short,unsigned_char,int)::bef_clock = param_3;
  }
  else {
    iVar2 = param_3 - write(unsigned_short,unsigned_char,int)::bef_clock;
  }
  write(unsigned_short,unsigned_char,int)::clocks =
       iVar2 + write(unsigned_short,unsigned_char,int)::clocks;
  cVar1 = *(char *)(**(int **)this + 0xc77d);
  while( true ) {
    iVar2 = 0x8000;
    if (cVar1 == '\0') {
      iVar2 = 0x4000;
    }
    if (write(unsigned_short,unsigned_char,int)::clocks <= iVar2) break;
    apu_snd::update(*(apu_snd **)(this + 4));
    iVar2 = 0x4000;
    cVar1 = *(char *)(**(int **)this + 0xc77d);
    if (cVar1 != '\0') {
      iVar2 = 0x8000;
    }
    write(unsigned_short,unsigned_char,int)::clocks =
         write(unsigned_short,unsigned_char,int)::clocks - iVar2;
  }
  write(unsigned_short,unsigned_char,int)::bef_clock = param_3;
  return;
}



/* ===== FUNCTION render @ 00014510 ===== */

/* apu_snd::render(short*, int) */

void __thiscall apu_snd::render(apu_snd *this,short *param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  apu_snd *paVar8;
  apu_snd *paVar9;
  apu_snd *paVar10;
  int iVar11;
  apu_snd *paVar12;
  undefined4 uVar13;
  int iVar14;
  uint uVar15;
  undefined4 uVar16;
  int iVar17;
  uint uVar18;
  uint uVar19;
  int iVar20;
  undefined4 uVar21;
  uint uVar22;
  int iVar23;
  uint uVar24;
  uint uVar25;
  int iVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  int iVar30;
  apu_snd *paVar31;
  int iVar32;
  int local_68;
  
  paVar9 = this + 4;
  paVar31 = this + 0x1dc;
  paVar10 = paVar9;
  do {
    paVar12 = paVar10;
    paVar8 = paVar31;
    uVar19 = *(uint *)(paVar12 + 4);
    uVar16 = *(undefined4 *)(paVar12 + 8);
    uVar13 = *(undefined4 *)(paVar12 + 0xc);
    paVar10 = paVar12 + 0x10;
    *(undefined4 *)paVar8 = *(undefined4 *)paVar12;
    *(uint *)(paVar8 + 4) = uVar19;
    *(undefined4 *)(paVar8 + 8) = uVar16;
    *(undefined4 *)(paVar8 + 0xc) = uVar13;
    paVar31 = paVar8 + 0x10;
  } while (paVar10 != this + 0xe4);
  uVar28 = (uint)(paVar12 + 0x13) & 3;
  uVar29 = (uint)paVar10 & 3;
  uVar16 = *(undefined4 *)(paVar12 + 0x14);
  uVar13 = *(undefined4 *)(paVar12 + 0x18);
  *(uint *)(paVar8 + 0x10) =
       (*(int *)(paVar12 + 0x13 + -uVar28) << (3 - uVar28) * 8 |
       uVar19 & 0xffffffffU >> (uVar28 + 1) * 8) & -1 << (4 - uVar29) * 8 |
       *(uint *)(paVar10 + -uVar29) >> uVar29 * 8;
  *(undefined4 *)(paVar8 + 0x14) = uVar16;
  *(undefined4 *)(paVar8 + 0x18) = uVar13;
  paVar31 = paVar9;
  paVar10 = this + 0xf0;
  do {
    paVar12 = paVar10;
    paVar8 = paVar31;
    uVar16 = *(undefined4 *)(paVar12 + 4);
    uVar19 = *(uint *)(paVar12 + 8);
    uVar13 = *(undefined4 *)(paVar12 + 0xc);
    paVar10 = paVar12 + 0x10;
    *(undefined4 *)paVar8 = *(undefined4 *)paVar12;
    *(undefined4 *)(paVar8 + 4) = uVar16;
    *(uint *)(paVar8 + 8) = uVar19;
    *(undefined4 *)(paVar8 + 0xc) = uVar13;
    paVar31 = paVar8 + 0x10;
  } while (paVar10 != this + 0x1d0);
  uVar28 = (uint)(paVar12 + 0x13) & 3;
  uVar29 = (uint)(paVar12 + 0x17) & 3;
  iVar7 = *(int *)(paVar12 + 0x17 + -uVar29);
  uVar27 = (uint)paVar10 & 3;
  uVar24 = (uint)(paVar12 + 0x14) & 3;
  uVar18 = *(uint *)(paVar12 + 0x14 + -uVar24);
  uVar13 = *(undefined4 *)(paVar12 + 0x18);
  *(uint *)(paVar8 + 0x10) =
       (*(int *)(paVar12 + 0x13 + -uVar28) << (3 - uVar28) * 8 |
       (uint)(this + 0x1d0) & 0xffffffffU >> (uVar28 + 1) * 8) & -1 << (4 - uVar27) * 8 |
       *(uint *)(paVar10 + -uVar27) >> uVar27 * 8;
  *(uint *)(paVar8 + 0x14) =
       (iVar7 << (3 - uVar29) * 8 | uVar19 & 0xffffffffU >> (uVar29 + 1) * 8) &
       -1 << (4 - uVar24) * 8 | uVar18 >> uVar24 * 8;
  *(undefined4 *)(paVar8 + 0x18) = uVar13;
  iVar7 = *(int *)(*(int *)**(undefined4 **)(this + 0x802d0) + 0xc768);
  memset(param_1,0,param_2 << 2);
  if (param_2 < 1) {
    local_68 = 0;
  }
  else {
    iVar30 = 0;
    iVar20 = *(int *)(this + 0x802cc);
    iVar32 = 0;
    local_68 = 0;
    do {
      if (param_2 == 0) {
        trap(7);
      }
      render(short*,int)::now_time = ((iVar7 - iVar20) * iVar30) / param_2 + iVar20;
      if (local_68 == 0x10000) {
LAB_00014d2c:
        local_68 = 0x10000;
      }
      else if ((*(int *)(this + local_68 * 8 + 0x2cc) < render(short*,int)::now_time) &&
              (*(int *)(this + 0x802c8) != 0)) {
        iVar20 = local_68 + 1;
        process(this,*(ushort *)(this + local_68 * 8 + 0x2c8),(uchar)this[local_68 * 8 + 0x2ca]);
        local_68 = iVar20;
        if (*(int *)(this + 0x802c8) <= iVar20) goto LAB_00014d2c;
      }
      iVar11 = 0;
      iVar20 = 0;
      if (*(int *)(this + 0xb0) != 0) {
        iVar20 = 0;
        if ((this[0x803d6] != (apu_snd)0x0) && (iVar20 = 0, this[4] != (apu_snd)0x0)) {
          iVar20 = 0x800 - (*(uint *)(this + 0x30) & 0x7ff);
          if (iVar20 == 0) {
            trap(7);
          }
          iVar20 = 0x20000 / iVar20;
          if (iVar20 < 0xfde9) {
            iVar11 = (*(int *)(sq_wav_dat +
                              ((*(uint *)(this + 0x1c) & 3) * 8 + sq1_produce(int)::cur_sample) * 4)
                      * 20000 + -10000) * 0x10000 >> 0x10;
            if (iVar20 << 3 < 0x10001) {
              uVar19 = (uint)(iVar20 << 0x13) / 0xac44;
            }
            else {
              uVar19 = 0x17c6e;
            }
            sq1_cur_pos = uVar19 + sq1_cur_pos;
            if ((sq1_cur_pos & 0xffff0000) != 0) {
              uVar19 = sq1_cur_pos >> 0x10;
              sq1_cur_pos = sq1_cur_pos & 0xffff;
              sq1_produce(int)::cur_sample = uVar19 + sq1_produce(int)::cur_sample & 7;
            }
          }
          else {
            iVar11 = 15000;
          }
          iVar17 = (iVar11 * *(int *)(this + 0x20)) / 0x14;
          iVar20 = 0;
          if (*(int *)(this + 0xb4) != 0) {
            iVar20 = iVar17 * *(int *)(this + 0xd4);
            if (iVar20 < 0) {
              iVar20 = iVar20 + 7;
            }
            iVar20 = iVar20 >> 3;
          }
          iVar11 = 0;
          if (*(int *)(this + 0xb8) != 0) {
            iVar17 = iVar17 * *(int *)(this + 0xd8);
            if (iVar17 < 0) {
              iVar17 = iVar17 + 7;
            }
            iVar11 = iVar17 >> 3;
          }
        }
        if ((this[0x803d7] != (apu_snd)0x0) && (this[0x3c] != (apu_snd)0x0)) {
          iVar17 = 0x800 - (*(uint *)(this + 0x5c) & 0x7ff);
          if (iVar17 == 0) {
            trap(7);
          }
          iVar17 = 0x20000 / iVar17;
          if (iVar17 < 0xfde9) {
            iVar14 = (*(int *)(sq_wav_dat +
                              ((*(uint *)(this + 0x48) & 3) * 8 + sq2_produce(int)::cur_sample) * 4)
                      * 20000 + -10000) * 0x10000 >> 0x10;
            if (iVar17 << 3 < 0x10001) {
              uVar19 = (uint)(iVar17 << 0x13) / 0xac44;
            }
            else {
              uVar19 = 0x17c6e;
            }
            sq2_cur_pos = uVar19 + sq2_cur_pos;
            if ((sq2_cur_pos & 0xffff0000) != 0) {
              uVar19 = sq2_cur_pos >> 0x10;
              sq2_cur_pos = sq2_cur_pos & 0xffff;
              sq2_produce(int)::cur_sample = uVar19 + sq2_produce(int)::cur_sample & 7;
            }
          }
          else {
            iVar14 = 15000;
          }
          iVar17 = (iVar14 * *(int *)(this + 0x4c)) / 0x14;
          if (*(int *)(this + 0xbc) != 0) {
            iVar14 = iVar17 * *(int *)(this + 0xd4);
            if (iVar14 < 0) {
              iVar14 = iVar14 + 7;
            }
            iVar20 = iVar20 + (iVar14 >> 3);
          }
          if (*(int *)(this + 0xc0) != 0) {
            iVar17 = iVar17 * *(int *)(this + 0xd8);
            if (iVar17 < 0) {
              iVar17 = iVar17 + 7;
            }
            iVar11 = iVar11 + (iVar17 >> 3);
          }
        }
        if ((this[0x803d8] != (apu_snd)0x0) && (this[0x68] != (apu_snd)0x0)) {
          iVar17 = 0x800 - (*(uint *)(this + 0x70) & 0x7ff);
          if (iVar17 == 0) {
            trap(7);
          }
          iVar17 = 0x10000 / iVar17;
          if (iVar17 << 5 < 0xfde9) {
            wav_cur_pos = (uint)(iVar17 << 0x15) / 0xac44 + wav_cur_pos;
            iVar17 = (int)(((uint)wav_produce(int,bool)::cur_sample * 0x9c4 + -15000) * 0x10000) >>
                     0x10;
            if ((wav_cur_pos & 0xffff0000) != 0) {
              wav_produce(int,bool)::bef_sample = wav_produce(int,bool)::cur_sample;
              uVar19 = (wav_cur_pos >> 0x10) + wav_produce(int,bool)::cur_pos2;
              wav_produce(int,bool)::cur_pos2 = uVar19 & 0x1f;
              if ((uVar19 & 1) == 0) {
                wav_produce(int,bool)::cur_sample =
                     (byte)((int)(uint)(byte)this[(wav_produce(int,bool)::cur_pos2 >> 1) + 0x802f6]
                           >> 4);
              }
              else {
                wav_produce(int,bool)::cur_sample =
                     (byte)this[(wav_produce(int,bool)::cur_pos2 >> 1) + 0x802f6] & 0xf;
              }
              wav_cur_pos = wav_cur_pos & 0xffff;
            }
          }
          else {
            iVar17 = (((int)(uint)(byte)this[0x802f6] >> 4) * 4000 + -30000) * 0x10000 >> 0x10;
          }
          iVar17 = ((iVar17 * *(int *)(this + 0x6c)) / 10) * *(int *)(this + 0xec);
          if (*(int *)(this + 0xc4) != 0) {
            iVar14 = iVar17 * *(int *)(this + 0xd4);
            if (iVar14 < 0) {
              iVar14 = iVar14 + 7;
            }
            iVar20 = iVar20 + (iVar14 >> 3);
          }
          if (*(int *)(this + 200) != 0) {
            iVar17 = iVar17 * *(int *)(this + 0xd8);
            if (iVar17 < 0) {
              iVar17 = iVar17 + 7;
            }
            iVar11 = iVar11 + (iVar17 >> 3);
          }
        }
        if ((this[0x803d9] != (apu_snd)0x0) && (this[0x84] != (apu_snd)0x0)) {
          iVar17 = 0;
          if (*(int *)(this + 0xa0) != 0) {
            noi_cur_pos = *(int *)(this + 0xa0) + noi_cur_pos;
            iVar17 = (int)(short)noi_produce(int)::cur_sample;
            if (0xac44 < noi_cur_pos) {
              uVar29 = *(uint *)(this + 0xac);
              uVar28 = 0x7f;
              iVar14 = 0;
              uVar19 = 0x8000;
              if ((uVar29 ^ 7) == 0) {
                uVar19 = 0x80;
              }
              else {
                uVar28 = 0x7fff;
              }
              uVar27 = (uint)((uVar29 ^ 7) != 0);
              do {
                if (iVar14 == 0) {
                  uVar24 = _mrand(unsigned_long)::shift_reg;
                  if (uVar27 == _mrand(unsigned_long)::bef_degree) {
LAB_000150e8:
                    uVar18 = uVar24;
                    uVar15 = uVar18 & 3;
                    if (uVar15 != 0) goto LAB_00014fd8;
LAB_000150f4:
                    uVar24 = 0xffffff7f;
                    if (uVar29 != 7) {
                      uVar24 = 0xffff7fff;
                    }
                    uVar18 = uVar24 & uVar18;
                  }
                  else {
                    uVar15 = uVar28 & 3;
                    uVar18 = uVar28;
                    uVar24 = _mrand(unsigned_long)::shift_reg & uVar28;
                    if ((_mrand(unsigned_long)::shift_reg & uVar28) != 0) goto LAB_000150e8;
LAB_00014fd8:
                    uVar24 = 0;
                    do {
                      uVar22 = uVar24;
                      uVar25 = uVar15 & 1;
                      uVar15 = (int)uVar15 >> 1;
                      uVar24 = uVar25 ^ uVar22;
                    } while (uVar15 != 0);
                    if (uVar25 == uVar22) goto LAB_000150f4;
                    uVar18 = uVar19 | uVar18;
                  }
                  _mrand(unsigned_long)::shift_reg = (int)uVar18 >> 1;
                  noi_produce(int)::cur_sample = -10000;
                  if ((_mrand(unsigned_long)::shift_reg & 1) != 0) {
                    noi_produce(int)::cur_sample = 12000;
                  }
                }
                else {
                  uVar24 = _mrand(unsigned_long)::shift_reg;
                  if (uVar27 == _mrand(unsigned_long)::bef_degree) {
LAB_000150c8:
                    uVar18 = uVar24;
                    uVar15 = uVar18 & 3;
                    if (uVar15 != 0) goto LAB_00015084;
LAB_000150d4:
                    uVar24 = 0xffffff7f;
                    if (uVar29 != 7) {
                      uVar24 = 0xffff7fff;
                    }
                    uVar18 = uVar24 & uVar18;
                  }
                  else {
                    uVar15 = uVar28 & 3;
                    uVar18 = uVar28;
                    uVar24 = _mrand(unsigned_long)::shift_reg & uVar28;
                    if ((_mrand(unsigned_long)::shift_reg & uVar28) != 0) goto LAB_000150c8;
LAB_00015084:
                    uVar24 = 0;
                    do {
                      uVar22 = uVar24;
                      uVar25 = uVar15 & 1;
                      uVar15 = (int)uVar15 >> 1;
                      uVar24 = uVar25 ^ uVar22;
                    } while (uVar15 != 0);
                    if (uVar25 == uVar22) goto LAB_000150d4;
                    uVar18 = uVar18 | uVar19;
                  }
                  _mrand(unsigned_long)::shift_reg = (int)uVar18 >> 1;
                  iVar23 = -10000;
                  if ((_mrand(unsigned_long)::shift_reg & 1) != 0) {
                    iVar23 = 12000;
                  }
                  noi_produce(int)::cur_sample = noi_produce(int)::cur_sample + iVar23;
                }
                noi_cur_pos = noi_cur_pos - 0xac44;
                iVar14 = iVar14 + 1;
                _mrand(unsigned_long)::bef_degree = uVar27;
              } while (0xac44 < noi_cur_pos);
              if (0 < iVar14) {
                if (iVar14 == 0) {
                  trap(7);
                }
                noi_produce(int)::cur_sample = noi_produce(int)::cur_sample / iVar14;
              }
            }
          }
          iVar17 = (iVar17 * *(int *)(this + 0x90)) / 0x14;
          if (*(int *)(this + 0xcc) != 0) {
            iVar14 = iVar17 * *(int *)(this + 0xd4);
            if (iVar14 < 0) {
              iVar14 = iVar14 + 7;
            }
            iVar20 = iVar20 + (iVar14 >> 3);
          }
          if (*(int *)(this + 0xd0) != 0) {
            iVar17 = iVar17 * *(int *)(this + 0xd8);
            if (iVar17 < 0) {
              iVar17 = iVar17 + 7;
            }
            iVar11 = iVar11 + (iVar17 >> 3);
          }
        }
      }
      if (this[0x802d4] != (apu_snd)0x0) {
        iVar17 = render(short*,int)::counter * 4;
        render(short*,int)::counter = render(short*,int)::counter + 1;
        if (1999 < render(short*,int)::counter) {
          render(short*,int)::counter = 0;
        }
        iVar20 = (*(short *)(render(short*,int)::filter + iVar17) * 2 + iVar20 * 5) / 5;
        *(short *)(render(short*,int)::filter + iVar17) = (short)iVar20;
        iVar11 = (*(short *)(render(short*,int)::filter + iVar17 + 2) * 2 + iVar11 * 5) / 5;
        *(short *)(render(short*,int)::filter + iVar17 + 2) = (short)iVar11;
      }
      iVar6 = DAT_00048570;
      iVar5 = DAT_0004856c;
      iVar4 = DAT_00048568;
      iVar3 = DAT_0004855c;
      iVar2 = DAT_00048558;
      iVar23 = DAT_00048554;
      iVar17 = iVar20;
      iVar14 = iVar11;
      if (this[0x802d5] != (apu_snd)0x0) {
        iVar17 = DAT_00048568 * 8;
        iVar26 = DAT_0004856c * 2 + DAT_00048570;
        DAT_00048554 = render(short*,int)::bef_sample_r;
        iVar14 = DAT_00048558 * 2 + DAT_0004855c;
        DAT_00048558 = iVar23;
        DAT_0004855c = iVar2;
        DAT_00048560 = iVar3;
        DAT_00048568 = render(short*,int)::bef_sample_l;
        DAT_0004856c = iVar4;
        DAT_00048570 = iVar5;
        DAT_00048574 = iVar6;
        iVar17 = (iVar26 + iVar17 + render(short*,int)::bef_sample_l * 2 + iVar20) / 0xe;
        iVar14 = (iVar14 + iVar23 * 8 + render(short*,int)::bef_sample_r * 2 + iVar11) / 0xe;
        render(short*,int)::bef_sample_r = iVar11;
        render(short*,int)::bef_sample_l = iVar20;
      }
      if (0x7fff < iVar17) {
        iVar17 = 0x7fff;
      }
      if (0x7fff < iVar14) {
        iVar14 = 0x7fff;
      }
      if (iVar17 < -0x7fff) {
        iVar17 = -0x7fff;
      }
      if (iVar14 < -0x7fff) {
        iVar14 = -0x7fff;
      }
      render(short*,int)::tmp_sample = render(short*,int)::tmp_sample + 1;
      param_1[1] = (short)iVar17;
      *param_1 = (short)iVar14;
      iVar11 = iVar32 << 0xe;
      while( true ) {
        iVar20 = *(int *)(this + 0x802cc);
        if (render(short*,int)::now_time - iVar20 <=
            (int)(((*(char *)(*(int *)**(undefined4 **)(this + 0x802d0) + 0xc77d) != '\0') + 1) *
                 iVar11)) break;
        iVar32 = iVar32 + 1;
        update(this);
        iVar11 = iVar11 + 0x4000;
      }
      iVar30 = iVar30 + 1;
      param_1 = param_1 + 2;
    } while (param_2 != iVar30);
  }
  if (local_68 < *(int *)(this + 0x802c8)) {
    paVar31 = this + (local_68 + 0x59) * 8;
    do {
      paVar10 = paVar31 + 2;
      local_68 = local_68 + 1;
      uVar1 = *(ushort *)paVar31;
      paVar31 = paVar31 + 8;
      process(this,uVar1,(uchar)*paVar10);
    } while (local_68 < *(int *)(this + 0x802c8));
  }
  *(undefined4 *)(this + 0x802c8) = 0;
  *(int *)(this + 0x802cc) = iVar7;
  paVar31 = this + 0xf0;
  paVar10 = paVar9;
  do {
    paVar12 = paVar10;
    paVar8 = paVar31;
    uVar21 = *(undefined4 *)(paVar12 + 4);
    uVar16 = *(undefined4 *)(paVar12 + 8);
    uVar13 = *(undefined4 *)(paVar12 + 0xc);
    paVar10 = paVar12 + 0x10;
    *(undefined4 *)paVar8 = *(undefined4 *)paVar12;
    *(undefined4 *)(paVar8 + 4) = uVar21;
    *(undefined4 *)(paVar8 + 8) = uVar16;
    *(undefined4 *)(paVar8 + 0xc) = uVar13;
    paVar31 = paVar8 + 0x10;
  } while (paVar10 != this + 0xe4);
  uVar16 = *(undefined4 *)(paVar12 + 0x14);
  uVar13 = *(undefined4 *)(paVar12 + 0x18);
  *(undefined4 *)(paVar8 + 0x10) = *(undefined4 *)paVar10;
  *(undefined4 *)(paVar8 + 0x14) = uVar16;
  *(undefined4 *)(paVar8 + 0x18) = uVar13;
  paVar31 = this + 0x1dc;
  do {
    paVar8 = paVar31;
    paVar10 = paVar9;
    uVar21 = *(undefined4 *)(paVar8 + 4);
    uVar16 = *(undefined4 *)(paVar8 + 8);
    uVar13 = *(undefined4 *)(paVar8 + 0xc);
    paVar31 = paVar8 + 0x10;
    *(undefined4 *)paVar10 = *(undefined4 *)paVar8;
    *(undefined4 *)(paVar10 + 4) = uVar21;
    *(undefined4 *)(paVar10 + 8) = uVar16;
    *(undefined4 *)(paVar10 + 0xc) = uVar13;
    paVar9 = paVar10 + 0x10;
  } while (paVar31 != this + 700);
  uVar16 = *(undefined4 *)(paVar8 + 0x14);
  uVar13 = *(undefined4 *)(paVar8 + 0x18);
  *(undefined4 *)(paVar10 + 0x10) = *(undefined4 *)paVar31;
  *(undefined4 *)(paVar10 + 0x14) = uVar16;
  *(undefined4 *)(paVar10 + 0x18) = uVar13;
  return;
}



/* ===== FUNCTION serialize @ 00015288 ===== */

/* apu_snd::serialize(serializer&) */

void __thiscall apu_snd::serialize(apu_snd *this,serializer *param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined4 uVar6;
  apu_snd *paVar7;
  apu_snd *paVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  code *pcVar11;
  
  if (4 < *(uint *)param_1) {
    puts("ERROR: invalid serializer!");
    goto LAB_000152f4;
  }
  paVar7 = this + 4;
  switch(*(uint *)param_1) {
  case 0:
    piVar5 = *(int **)(param_1 + 4);
    iVar1 = *piVar5;
    *piVar5 = iVar1 + 0xec;
    *piVar5 = iVar1 + 0x1d8;
    *piVar5 = iVar1 + 0x2d8;
    *piVar5 = iVar1 + 0x2dc;
    *piVar5 = iVar1 + 0x2dd;
    *piVar5 = iVar1 + 0x2de;
    return;
  case 1:
    puVar4 = *(undefined4 **)(param_1 + 4);
    do {
      paVar8 = paVar7;
      puVar2 = puVar4;
      uVar10 = *(undefined4 *)(paVar8 + 4);
      uVar9 = *(undefined4 *)(paVar8 + 8);
      uVar6 = *(undefined4 *)(paVar8 + 0xc);
      paVar7 = paVar8 + 0x10;
      *puVar2 = *(undefined4 *)paVar8;
      puVar2[1] = uVar10;
      puVar2[2] = uVar9;
      puVar2[3] = uVar6;
      puVar4 = puVar2 + 4;
    } while (paVar7 != this + 0xe4);
    uVar9 = *(undefined4 *)(paVar8 + 0x14);
    uVar6 = *(undefined4 *)(paVar8 + 0x18);
    puVar2[4] = *(undefined4 *)paVar7;
    puVar2[5] = uVar9;
    puVar2[6] = uVar6;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    goto LAB_000152f4;
  case 2:
    puVar2 = *(undefined4 **)(param_1 + 4);
    puVar4 = puVar2;
    do {
      paVar8 = paVar7;
      puVar3 = puVar4;
      uVar10 = puVar3[1];
      uVar9 = puVar3[2];
      uVar6 = puVar3[3];
      puVar4 = puVar3 + 4;
      *(undefined4 *)paVar8 = *puVar3;
      *(undefined4 *)(paVar8 + 4) = uVar10;
      *(undefined4 *)(paVar8 + 8) = uVar9;
      *(undefined4 *)(paVar8 + 0xc) = uVar6;
      paVar7 = paVar8 + 0x10;
    } while (puVar4 != puVar2 + 0x38);
    uVar9 = puVar3[5];
    uVar6 = puVar3[6];
    *(undefined4 *)(paVar8 + 0x10) = *puVar4;
    *(undefined4 *)(paVar8 + 0x14) = uVar9;
    *(undefined4 *)(paVar8 + 0x18) = uVar6;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    goto LAB_000152f4;
  case 3:
    pcVar11 = fwrite;
    break;
  case 4:
    pcVar11 = fread;
  }
  (*pcVar11)(paVar7,1,0xec,*(undefined4 *)(param_1 + 4));
LAB_000152f4:
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001531c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(render(short*,int)::filter + *(int *)(&DAT_00033ab0 + *(uint *)param_1 * 4) + 0x7ccc)
    )(this + 0xf0);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001536c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(render(short*,int)::filter + *(int *)(&DAT_00033ac4 + *(uint *)param_1 * 4) + 0x7ccc)
    )(this + 0x802d6);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x000153bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(render(short*,int)::filter + *(int *)(&DAT_00033ad8 + *(uint *)param_1 * 4) + 0x7ccc)
    )(this + 0x802cc);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001540c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(render(short*,int)::filter + *(int *)(&DAT_00033aec + *(uint *)param_1 * 4) + 0x7ccc)
    )(this + 0x802d4);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001545c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(render(short*,int)::filter + *(int *)(&DAT_00033b00 + *(uint *)param_1 * 4) + 0x7ccc)
    )(this + 0x802d5);
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x00015564. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  puts("ERROR: invalid serializer!");
  return;
}



/* ===== FUNCTION serialize @ 00015a60 ===== */

/* apu::serialize(serializer&) */

void __thiscall apu::serialize(apu *this,serializer *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int *piVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  code *pcVar11;
  
  iVar8 = *(int *)(this + 4);
  if (4 < *(uint *)param_1) {
    puts("ERROR: invalid serializer!");
    goto LAB_000152f4;
  }
  puVar6 = (undefined4 *)(iVar8 + 4);
  switch(*(uint *)param_1) {
  case 0:
    piVar4 = *(int **)(param_1 + 4);
    iVar8 = *piVar4;
    *piVar4 = iVar8 + 0xec;
    *piVar4 = iVar8 + 0x1d8;
    *piVar4 = iVar8 + 0x2d8;
    *piVar4 = iVar8 + 0x2dc;
    *piVar4 = iVar8 + 0x2dd;
    *piVar4 = iVar8 + 0x2de;
    return;
  case 1:
    puVar3 = *(undefined4 **)(param_1 + 4);
    do {
      puVar2 = puVar6;
      puVar1 = puVar3;
      uVar10 = puVar2[1];
      uVar9 = puVar2[2];
      uVar5 = puVar2[3];
      puVar6 = puVar2 + 4;
      *puVar1 = *puVar2;
      puVar1[1] = uVar10;
      puVar1[2] = uVar9;
      puVar1[3] = uVar5;
      puVar3 = puVar1 + 4;
    } while (puVar6 != (undefined4 *)(iVar8 + 0xe4));
    uVar9 = puVar2[5];
    uVar5 = puVar2[6];
    puVar1[4] = *puVar6;
    puVar1[5] = uVar9;
    puVar1[6] = uVar5;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    goto LAB_000152f4;
  case 2:
    puVar1 = *(undefined4 **)(param_1 + 4);
    puVar3 = puVar1;
    do {
      puVar7 = puVar6;
      puVar2 = puVar3;
      uVar10 = puVar2[1];
      uVar9 = puVar2[2];
      uVar5 = puVar2[3];
      puVar3 = puVar2 + 4;
      *puVar7 = *puVar2;
      puVar7[1] = uVar10;
      puVar7[2] = uVar9;
      puVar7[3] = uVar5;
      puVar6 = puVar7 + 4;
    } while (puVar3 != puVar1 + 0x38);
    uVar9 = puVar2[5];
    uVar5 = puVar2[6];
    puVar7[4] = *puVar3;
    puVar7[5] = uVar9;
    puVar7[6] = uVar5;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    goto LAB_000152f4;
  case 3:
    pcVar11 = fwrite;
    break;
  case 4:
    pcVar11 = fread;
  }
  (*pcVar11)(puVar6,1,0xec,*(undefined4 *)(param_1 + 4));
LAB_000152f4:
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001531c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00033ab0 + *(uint *)param_1 * 4) + 0x7ccc))(iVar8 + 0xf0);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0001536c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00033ac4 + *(uint *)param_1 * 4) + 0x7ccc))(iVar8 + 0x802d6);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x000153bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00033ad8 + *(uint *)param_1 * 4) + 0x7ccc))(iVar8 + 0x802cc);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (4 < *(uint *)param_1) {
    puts("ERROR: invalid serializer!");
    if (4 < *(uint *)param_1) {
                    /* WARNING: Could not recover jumptable at 0x00015564. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      puts("ERROR: invalid serializer!");
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x0001545c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00033b00 + *(uint *)param_1 * 4) + 0x7ccc))(iVar8 + 0x802d5);
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0001540c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(apu_snd::render(short*,int)::filter +
            *(int *)(&DAT_00033aec + *(uint *)param_1 * 4) + 0x7ccc))(iVar8 + 0x802d4);
  return;
}



/* ===== FUNCTION ~cheat @ 00015a80 ===== */

/* cheat::~cheat() */

void __thiscall cheat::~cheat(cheat *this)

{
  cheat *pcVar1;
  cheat *pcVar2;
  
  pcVar1 = *(cheat **)this;
  while (this != pcVar1) {
    pcVar2 = *(cheat **)pcVar1;
    operator_delete(pcVar1);
    pcVar1 = pcVar2;
  }
  return;
}



/* ===== FUNCTION find_cheat @ 00015adc ===== */

/* cheat::find_cheat(char*) */

char * cheat::find_cheat(char *param_1)

{
  int iVar1;
  int *in_a1;
  char *in_a2;
  int *piVar2;
  
  piVar2 = (int *)*in_a1;
  *(int **)param_1 = piVar2;
  while ((piVar2 != in_a1 && (iVar1 = strcmp((char *)((int)piVar2 + 0xd),in_a2), iVar1 != 0))) {
    piVar2 = (int *)*piVar2;
    *(int **)param_1 = piVar2;
  }
  return param_1;
}



/* ===== FUNCTION create_unique_name @ 00015b64 ===== */

/* cheat::create_unique_name(char*) */

void __thiscall cheat::create_unique_name(cheat *this,char *param_1)

{
  bool bVar1;
  int iVar2;
  cheat *pcVar3;
  int iVar4;
  char acStack_30 [16];
  
  iVar4 = 0;
  do {
    sprintf(acStack_30,"cheat_%03d",iVar4);
    pcVar3 = *(cheat **)this;
    if (pcVar3 == this) break;
    bVar1 = true;
    do {
      iVar2 = strcmp((char *)(pcVar3 + 0xd),acStack_30);
      pcVar3 = *(cheat **)pcVar3;
      if (iVar2 == 0) {
        bVar1 = false;
      }
    } while (this != pcVar3);
    iVar4 = iVar4 + 1;
  } while (!bVar1);
  strcpy(param_1,acStack_30);
  return;
}



/* ===== FUNCTION create_cheat_map @ 00015c34 ===== */

/* cheat::create_cheat_map() */

void __thiscall cheat::create_cheat_map(cheat *this)

{
  cheat cVar1;
  ushort uVar2;
  cheat *pcVar3;
  uint uVar4;
  cheat *pcVar5;
  cheat *pcVar6;
  
  memset(this + 0xc,0,0x40000);
  pcVar6 = *(cheat **)this;
  do {
    if (pcVar6 == this) {
      return;
    }
    pcVar3 = pcVar6 + 8;
    do {
      cVar1 = pcVar3[1];
      pcVar5 = pcVar3;
      if ((byte)cVar1 < 0x98) {
        if (((byte)cVar1 < 0x90) && (cVar1 != (cheat)0x1)) {
          if (cVar1 == (cheat)0x10) {
            cVar1 = pcVar3[4];
            pcVar5 = *(cheat **)(pcVar3 + 0x104);
            if ((byte)cVar1 != 0) {
              uVar2 = *(ushort *)(pcVar3 + 2);
              uVar4 = 0;
              pcVar3 = this + (uint)*(ushort *)(pcVar5 + 2) * 4;
              do {
                uVar4 = uVar4 + 1;
                *(undefined4 *)(pcVar3 + 0xc) = 1;
                pcVar3 = pcVar3 + (uVar2 + 1) * 4;
              } while (uVar4 != (byte)cVar1);
            }
          }
        }
        else {
LAB_00015d18:
          *(undefined4 *)(this + (*(ushort *)(pcVar3 + 2) + 2) * 4 + 4) = 1;
        }
      }
      else if (cVar1 == (cheat)0xa1) goto LAB_00015d18;
      pcVar3 = *(cheat **)(pcVar5 + 0x104);
    } while (pcVar3 != (cheat *)0x0);
    pcVar6 = *(cheat **)pcVar6;
  } while( true );
}



/* ===== FUNCTION cheat @ 00015d30 ===== */

/* cheat::cheat(gb*) */

void __thiscall cheat::cheat(cheat *this,gb *param_1)

{
  *(cheat **)this = this;
  *(cheat **)(this + 4) = this;
  *(undefined4 *)(this + 8) = 0;
                    /* try { // try from 00015d70 to 00015d77 has its CatchHandler @ 00015d90 */
  *(gb **)(this + 0x4000c) = param_1;
  create_cheat_map(this);
  return;
}



/* ===== FUNCTION clear @ 00015dc8 ===== */

/* cheat::clear() */

void __thiscall cheat::clear(cheat *this)

{
  cheat cVar1;
  ushort uVar2;
  uint uVar3;
  cheat *pcVar4;
  cheat *pcVar5;
  cheat *pcVar6;
  
  pcVar5 = *(cheat **)this;
  while (pcVar5 != this) {
    pcVar6 = *(cheat **)pcVar5;
    operator_delete(pcVar5);
    pcVar5 = pcVar6;
  }
  *(cheat **)this = this;
  *(cheat **)(this + 4) = this;
  *(undefined4 *)(this + 8) = 0;
  memset(this + 0xc,0,0x40000);
  pcVar5 = *(cheat **)this;
  do {
    if (pcVar5 == this) {
      return;
    }
    pcVar6 = pcVar5 + 8;
    do {
      cVar1 = pcVar6[1];
      pcVar4 = pcVar6;
      if ((byte)cVar1 < 0x98) {
        if (((byte)cVar1 < 0x90) && (cVar1 != (cheat)0x1)) {
          if (cVar1 == (cheat)0x10) {
            cVar1 = pcVar6[4];
            pcVar4 = *(cheat **)(pcVar6 + 0x104);
            if ((byte)cVar1 != 0) {
              uVar2 = *(ushort *)(pcVar6 + 2);
              uVar3 = 0;
              pcVar6 = this + (uint)*(ushort *)(pcVar4 + 2) * 4;
              do {
                uVar3 = uVar3 + 1;
                *(undefined4 *)(pcVar6 + 0xc) = 1;
                pcVar6 = pcVar6 + (uVar2 + 1) * 4;
              } while (uVar3 != (byte)cVar1);
            }
          }
        }
        else {
LAB_00015d18:
          *(undefined4 *)(this + (*(ushort *)(pcVar6 + 2) + 2) * 4 + 4) = 1;
        }
      }
      else if (cVar1 == (cheat)0xa1) goto LAB_00015d18;
      pcVar6 = *(cheat **)(pcVar4 + 0x104);
    } while (pcVar6 != (cheat *)0x0);
    pcVar5 = *(cheat **)pcVar5;
  } while( true );
}



/* ===== FUNCTION delete_cheat @ 00015e38 ===== */

/* cheat::delete_cheat(char*) */

void __thiscall cheat::delete_cheat(cheat *this,char *param_1)

{
  cheat cVar1;
  ushort uVar2;
  cheat *pcVar3;
  int iVar4;
  uint uVar5;
  cheat *pcVar6;
  cheat *pcVar7;
  
  pcVar7 = *(cheat **)this;
  do {
    if (pcVar7 == this) {
cheat_create_cheat_map:
      memset(this + 0xc,0,0x40000);
      pcVar7 = *(cheat **)this;
      do {
        if (pcVar7 == this) {
          return;
        }
        pcVar3 = pcVar7 + 8;
        do {
          cVar1 = pcVar3[1];
          pcVar6 = pcVar3;
          if ((byte)cVar1 < 0x98) {
            if (((byte)cVar1 < 0x90) && (cVar1 != (cheat)0x1)) {
              if (cVar1 == (cheat)0x10) {
                cVar1 = pcVar3[4];
                pcVar6 = *(cheat **)(pcVar3 + 0x104);
                if ((byte)cVar1 != 0) {
                  uVar2 = *(ushort *)(pcVar3 + 2);
                  uVar5 = 0;
                  pcVar3 = this + (uint)*(ushort *)(pcVar6 + 2) * 4;
                  do {
                    uVar5 = uVar5 + 1;
                    *(undefined4 *)(pcVar3 + 0xc) = 1;
                    pcVar3 = pcVar3 + (uVar2 + 1) * 4;
                  } while (uVar5 != (byte)cVar1);
                }
              }
            }
            else {
LAB_00015d18:
              *(undefined4 *)(this + (*(ushort *)(pcVar3 + 2) + 2) * 4 + 4) = 1;
            }
          }
          else if (cVar1 == (cheat)0xa1) goto LAB_00015d18;
          pcVar3 = *(cheat **)(pcVar6 + 0x104);
        } while (pcVar3 != (cheat *)0x0);
        pcVar7 = *(cheat **)pcVar7;
      } while( true );
    }
    iVar4 = strcmp((char *)(pcVar7 + 0xd),param_1);
    if (iVar4 == 0) {
      *(int *)(this + 8) = *(int *)(this + 8) + -1;
      std::__detail::_List_node_base::_M_unhook();
      operator_delete(pcVar7);
      goto cheat_create_cheat_map;
    }
    pcVar7 = *(cheat **)pcVar7;
  } while( true );
}



/* ===== FUNCTION add_cheat @ 00015ee0 ===== */

/* cheat::add_cheat(cheat_dat*) */

void __thiscall cheat::add_cheat(cheat *this,cheat_dat *param_1)

{
  cheat_dat cVar1;
  ushort uVar2;
  _List_node_base *p_Var3;
  byte bVar4;
  undefined2 uVar5;
  _List_node_base *p_Var6;
  _List_node_base *p_Var7;
  uint uVar8;
  ushort *puVar9;
  int *piVar10;
  mbc *this_00;
  undefined2 *puVar11;
  cheat_dat *pcVar12;
  cheat_dat *pcVar13;
  uint uVar14;
  int iVar15;
  undefined4 uVar16;
  uint uVar17;
  int iVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  
  if (param_1[1] != (cheat_dat)0x0) {
    p_Var6 = operator_new(0x110);
    p_Var3 = p_Var6 + 8;
    pcVar13 = param_1;
    do {
      pcVar12 = pcVar13;
      p_Var7 = p_Var3;
      uVar20 = *(undefined4 *)(pcVar12 + 4);
      uVar19 = *(undefined4 *)(pcVar12 + 8);
      uVar16 = *(undefined4 *)(pcVar12 + 0xc);
      pcVar13 = pcVar12 + 0x10;
      *(undefined4 *)p_Var7 = *(undefined4 *)pcVar12;
      *(undefined4 *)(p_Var7 + 4) = uVar20;
      *(undefined4 *)(p_Var7 + 8) = uVar19;
      *(undefined4 *)(p_Var7 + 0xc) = uVar16;
      p_Var3 = p_Var7 + 0x10;
    } while (pcVar13 != param_1 + 0x100);
    uVar16 = *(undefined4 *)(pcVar12 + 0x14);
    *(undefined4 *)(p_Var7 + 0x10) = *(undefined4 *)pcVar13;
    *(undefined4 *)(p_Var7 + 0x14) = uVar16;
    std::__detail::_List_node_base::_M_hook(p_Var6);
    *(int *)(this + 8) = *(int *)(this + 8) + 1;
                    /* WARNING: Could not recover jumptable at 0x00015f9c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    create_cheat_map(this);
    return;
  }
  cVar1 = param_1[4];
  uVar17 = (uint)(byte)cVar1;
  uVar2 = *(ushort *)(param_1 + 2);
  uVar14 = (uint)uVar2;
  piVar10 = (int *)**(undefined4 **)(this + 0x4000c);
  uVar8 = (int)uVar14 >> 0xd;
  if (uVar8 == 5) {
    this_00 = *(mbc **)(*piVar10 + 0x10);
    if (this_00[0x11] == (mbc)0x0) {
      mbc::ext_write(this_00,uVar2,(uchar)cVar1);
      return;
    }
    *(cheat_dat *)(*(int *)(this_00 + 4) + (uVar14 & 0x1fff)) = cVar1;
    return;
  }
  if (uVar8 < 6) {
    if (uVar8 == 4) {
      *(cheat_dat *)(piVar10[0x3057] + (uVar14 & 0x1fff)) = cVar1;
      return;
    }
LAB_000172ac:
    mbc::write((ushort)*(undefined4 *)(*piVar10 + 0x10),(uchar)uVar2);
    return;
  }
  if (uVar8 == 6) {
LAB_0001727c:
    if ((uVar2 & 0x1000) == 0) {
      *(cheat_dat *)((int)piVar10 + (uVar14 & 0xfff) + 0x12) = cVar1;
      return;
    }
    *(cheat_dat *)(piVar10[0x3058] + (uVar14 & 0xfff)) = cVar1;
    return;
  }
  if (uVar8 != 7) goto LAB_000172ac;
  if (uVar14 < 0xfe00) goto LAB_0001727c;
  if (uVar14 < 0xfea0) {
    *(cheat_dat *)((int)piVar10 + (uVar14 - 0x3d6e)) = cVar1;
    return;
  }
  if (uVar14 < 0xff00) {
    *(cheat_dat *)((int)piVar10 + (((int)(uVar14 - 0xffa0) >> 5) << 3 | uVar14 & 7) + 0xc132) =
         cVar1;
    return;
  }
  if ((0xff7f < uVar14) && (uVar14 != 0xffff)) {
    *(cheat_dat *)((int)piVar10 + (uVar14 - 0x3f6e)) = cVar1;
    return;
  }
  switch(uVar14 + 0x100 & 0xffff) {
  case 0:
    *(cheat_dat *)(*piVar10 + 0x20) = cVar1;
    break;
  case 1:
    *(cheat_dat *)(*piVar10 + 0x21) = cVar1;
    break;
  case 2:
    iVar15 = *piVar10;
    if (*(int *)(*(int *)(iVar15 + 0xc) + 0x20) == 1) {
      *(byte *)(iVar15 + 0x22) = (byte)cVar1 & 0x81;
      if (((char)cVar1 < '\0') && (((byte)cVar1 & 1) != 0)) {
        piVar10[0x31dd] = piVar10[0x31da] + 0x200;
      }
    }
    else {
      *(byte *)(iVar15 + 0x22) = (byte)cVar1 & 0x83;
      if (((char)cVar1 < '\0') && (((byte)cVar1 & 1) != 0)) {
        if (((byte)cVar1 & 2) == 0) {
          piVar10[0x31dd] = piVar10[0x31da] + 0x1000;
        }
        else {
          piVar10[0x31dd] = piVar10[0x31da] + 0x80;
        }
      }
    }
    break;
  default:
    if ((uVar14 + 0xf0 & 0xffff) < 0x30) {
      apu::write(*(apu **)(*piVar10 + 8),uVar2,(uchar)cVar1,piVar10[0x31da]);
      return;
    }
    if ((uVar14 + 0x8f & 0xffff) < 0xf) {
      *(cheat_dat *)((int)piVar10 + (uVar14 - 0x3e27)) = cVar1;
    }
    break;
  case 4:
    *(undefined1 *)(*piVar10 + 0x23) = 0;
    break;
  case 5:
    *(cheat_dat *)(*piVar10 + 0x24) = cVar1;
    break;
  case 6:
    *(cheat_dat *)(*piVar10 + 0x25) = cVar1;
    break;
  case 7:
    if (((byte)cVar1 & 4) == 0) {
      iVar15 = *piVar10;
    }
    else {
      iVar15 = *piVar10;
      if ((*(byte *)(iVar15 + 0x26) & 4) == 0) {
        piVar10[0x31dc] = 0;
        *(cheat_dat *)(iVar15 + 0x26) = cVar1;
        return;
      }
    }
    *(cheat_dat *)(iVar15 + 0x26) = cVar1;
    break;
  case 0xf:
    *(cheat_dat *)(*piVar10 + 0x27) = cVar1;
    break;
  case 0x40:
    if ((char)cVar1 < '\0') {
      iVar15 = *piVar10;
      if (-1 < *(char *)(iVar15 + 0x28)) {
        *(undefined1 *)(iVar15 + 0x2c) = 0;
        *(undefined4 *)(*(int *)(iVar15 + 4) + 0x39c) = 9;
        *(cheat_dat *)(iVar15 + 0x28) = cVar1;
        return;
      }
    }
    else {
      iVar15 = *piVar10;
    }
    *(cheat_dat *)(iVar15 + 0x28) = cVar1;
    break;
  case 0x41:
    iVar15 = *piVar10;
    if (*(int *)(*(int *)(iVar15 + 0xc) + 0x20) == 1) {
      bVar4 = *(byte *)(iVar15 + 0x29);
      if ((bVar4 & 2) == 0) {
        *(byte *)(iVar15 + 0x27) = *(byte *)(iVar15 + 0x27) | 2;
      }
    }
    else {
      bVar4 = *(byte *)(iVar15 + 0x29);
    }
    *(byte *)(iVar15 + 0x29) = bVar4 & 7 | (byte)cVar1 & 0x78;
    break;
  case 0x42:
    *(cheat_dat *)(*piVar10 + 0x2a) = cVar1;
    break;
  case 0x43:
    *(cheat_dat *)(*piVar10 + 0x2b) = cVar1;
    break;
  case 0x44:
    *(undefined4 *)(*(int *)(*piVar10 + 4) + 0x39c) = 9;
    break;
  case 0x45:
    *(cheat_dat *)(*piVar10 + 0x2d) = cVar1;
    break;
  case 0x46:
                    /* WARNING: Could not recover jumptable at 0x00024f80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_000357b0 + ((int)uVar17 >> 5) * 4) + 0x7ccc))();
    return;
  case 0x47:
    *(cheat_dat *)(*piVar10 + 0x2f) = cVar1;
    break;
  case 0x48:
    *(cheat_dat *)(*piVar10 + 0x30) = cVar1;
    break;
  case 0x49:
    *(cheat_dat *)(*piVar10 + 0x31) = cVar1;
    break;
  case 0x4a:
    *(cheat_dat *)(*piVar10 + 0x32) = cVar1;
    break;
  case 0x4b:
    *(cheat_dat *)(*piVar10 + 0x33) = cVar1;
    break;
  case 0x4d:
    *(byte *)(*piVar10 + 0x35) = (byte)cVar1 & 1;
    *(byte *)((int)piVar10 + 0xc77e) = (byte)cVar1 & 1;
    break;
  case 0x4f:
    if (*(char *)((int)piVar10 + 0xc77f) == '\0') {
      piVar10[0x3057] = (int)piVar10 + (uVar17 & 1) * 0x2000 + 0x8012;
      *(cheat_dat *)(*piVar10 + 0x36) = cVar1;
    }
    break;
  case 0x51:
    piVar10[0x31e1] = piVar10[0x31e1] & 0xf0U | uVar17 << 8;
    break;
  case 0x52:
    piVar10[0x31e1] = piVar10[0x31e1] & 0xff00U | uVar17 & 0xf0;
    break;
  case 0x53:
    piVar10[0x31e2] = piVar10[0x31e2] & 0xf0U | uVar17 << 8;
    break;
  case 0x54:
    piVar10[0x31e2] = piVar10[0x31e2] & 0xff00U | uVar17 & 0xf0;
    break;
  case 0x55:
    uVar8 = piVar10[0x31e1];
    if ((uVar8 - 0x8000 < 0x2000) || (0xdfff < (int)uVar8)) {
      *(undefined1 *)(*piVar10 + 0x3b) = 0;
      return;
    }
    if ((char)cVar1 < '\0') {
      if (*(char *)((int)piVar10 + 0xc77f) == '\0') {
        *(undefined1 *)((int)piVar10 + 0xc77f) = 1;
        piVar10[0x31e3] = (uVar17 & 0x7f) + 1;
        *(undefined1 *)(piVar10 + 0x31e5) = 1;
        *(undefined1 *)(*piVar10 + 0x3b) = 0;
        return;
      }
LAB_00025760:
      *(undefined1 *)((int)piVar10 + 0xc77f) = 0;
      piVar10[0x31e3] = 0;
      *(undefined1 *)(*piVar10 + 0x3b) = 0xff;
      return;
    }
    if (*(char *)((int)piVar10 + 0xc77f) != '\0') goto LAB_00025760;
    iVar15 = *piVar10;
    uVar14 = piVar10[0x31e2];
    piVar10[0x31e3] = 0;
    *(undefined1 *)(iVar15 + 0x3b) = 0xff;
    switch((int)uVar8 >> 0xd) {
    case 0:
    case 1:
      iVar15 = *(int *)(*(int *)(iVar15 + 0xc) + 0x2c);
      goto LAB_000255e0;
    case 2:
    case 3:
      iVar15 = **(int **)(iVar15 + 0x10);
LAB_000255e0:
      memcpy((void *)(piVar10[0x3057] + (uVar14 & 0x1ff0)),(void *)(iVar15 + uVar8),
             (uVar17 + 1) * 0x10);
      break;
    default:
      break;
    case 5:
      iVar18 = *(int *)(*(int *)(iVar15 + 0x10) + 4);
      uVar8 = uVar8 & 0x1fff;
      iVar15 = piVar10[0x3057];
LAB_00025530:
      memcpy((void *)(iVar15 + (uVar14 & 0x1ff0)),(void *)(iVar18 + uVar8),(uVar17 + 1) * 0x10);
      break;
    case 6:
      if ((uVar8 & 0x1000) == 0) {
        iVar18 = (int)piVar10 + 0x12;
        uVar8 = uVar8 & 0xfff;
        iVar15 = piVar10[0x3057];
        goto LAB_00025530;
      }
      memcpy((void *)(piVar10[0x3057] + (uVar14 & 0x1ff0)),
             (void *)(piVar10[0x3058] + (uVar8 & 0xfff)),(uVar17 + 1) * 0x10);
    }
    iVar15 = (uVar17 + 1) * 0x10;
    piVar10[0x31e1] = piVar10[0x31e1] + iVar15;
    piVar10[0x31e2] = piVar10[0x31e2] + iVar15;
    piVar10[0x31e4] = ((*(char *)((int)piVar10 + 0xc77d) != '\0') + 1) * (uVar17 + 1) * 0x20 + 0x390
    ;
    break;
  case 0x56:
    iVar18 = piVar10[0x31d9];
    iVar15 = piVar10[0x31db];
    piVar10[0x31d9] = iVar18 + 1;
    piVar10[iVar18 + 0x30da] = 0;
    piVar10[iVar18 + 0x30d9] = (uint)CONCAT12(cVar1,(short)iVar15);
    *(cheat_dat *)(*piVar10 + 0x3c) = cVar1;
    break;
  case 0x68:
    *(cheat_dat *)(*piVar10 + 0x3d) = cVar1;
    break;
  case 0x69:
    iVar15 = *piVar10;
    bVar4 = *(byte *)(iVar15 + 0x3d);
    iVar18 = *(int *)(iVar15 + 4);
    uVar8 = (int)(uint)bVar4 >> 3;
    uVar14 = bVar4 & 6;
    if ((bVar4 & 1) == 0) {
      puVar9 = (ushort *)(iVar18 + ((uVar8 & 7) + 3) * 8 + uVar14);
      *puVar9 = *puVar9 & 0xff00 | (ushort)(byte)cVar1;
    }
    else {
      puVar11 = (undefined2 *)(iVar18 + ((uVar8 & 7) + 3) * 8 + uVar14);
      *puVar11 = CONCAT11(cVar1,*(undefined1 *)puVar11);
    }
    uVar5 = (**(code **)(**(int **)(iVar15 + 0x14) + 0x10))(*(int **)(iVar15 + 0x14));
    iVar15 = *piVar10;
    *(undefined2 *)(iVar18 + ((uVar8 & 7) + 0x13) * 8 + uVar14) = uVar5;
    *(cheat_dat *)(iVar15 + 0x3e) = cVar1;
    if (*(char *)(iVar15 + 0x3d) < '\0') {
      *(byte *)(iVar15 + 0x3d) = *(char *)(iVar15 + 0x3d) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6a:
    *(cheat_dat *)(*piVar10 + 0x3f) = cVar1;
    break;
  case 0x6b:
    iVar15 = *piVar10;
    uVar14 = (uint)*(byte *)(iVar15 + 0x3f);
    uVar8 = (int)uVar14 >> 3;
    if ((*(byte *)(iVar15 + 0x3f) & 1) == 0) {
      iVar18 = *(int *)(iVar15 + 4);
      puVar9 = (ushort *)(iVar18 + ((uVar8 & 7) + 0xb) * 8 + (uVar14 & 6));
      *puVar9 = *puVar9 & 0xff00 | (ushort)(byte)cVar1;
    }
    else {
      iVar18 = *(int *)(iVar15 + 4);
      puVar11 = (undefined2 *)(iVar18 + ((uVar8 & 7) + 0xb) * 8 + (uVar14 & 6));
      *puVar11 = CONCAT11(cVar1,*(undefined1 *)puVar11);
    }
    uVar5 = (**(code **)(**(int **)(iVar15 + 0x14) + 0x10))(*(int **)(iVar15 + 0x14));
    iVar15 = *piVar10;
    *(undefined2 *)(iVar18 + ((uVar8 & 7) + 0x1b) * 8 + (uVar14 & 6)) = uVar5;
    *(cheat_dat *)(iVar15 + 0x40) = cVar1;
    if (*(char *)(iVar15 + 0x3f) < '\0') {
      *(byte *)(iVar15 + 0x3f) = *(char *)(iVar15 + 0x3f) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6c:
    *(byte *)(piVar10 + 0x31ea) = (byte)cVar1 & 1;
    break;
  case 0x70:
    uVar17 = uVar17 & 7;
    iVar15 = 0x1000;
    if (((byte)cVar1 & 7) == 0) {
      uVar17 = 1;
    }
    else {
      iVar15 = uVar17 << 0xc;
    }
    *(char *)(*piVar10 + 0x41) = (char)uVar17;
    piVar10[0x3058] = (int)piVar10 + iVar15 + 0x12;
    break;
  case 0x72:
    *(cheat_dat *)((int)piVar10 + 0xc7a9) = cVar1;
    break;
  case 0x73:
    *(cheat_dat *)((int)piVar10 + 0xc7aa) = cVar1;
    break;
  case 0x74:
    *(cheat_dat *)((int)piVar10 + 0xc7ab) = cVar1;
    break;
  case 0x75:
    *(byte *)(piVar10 + 0x31eb) = (byte)cVar1 & 0x70;
    break;
  case 0xff:
    *(cheat_dat *)(*piVar10 + 0x34) = cVar1;
  }
  return;
}



/* ===== FUNCTION cheat_read @ 00015fd4 ===== */

/* cheat::cheat_read(unsigned short) */

uint __thiscall cheat::cheat_read(cheat *this,ushort param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined2 in_register_00000016;
  cheat *pcVar5;
  cheat *pcVar6;
  
  uVar3 = CONCAT22(in_register_00000016,param_1);
  pcVar6 = *(cheat **)this;
  if (pcVar6 != this) {
    do {
      if (pcVar6[8] != (cheat)0x0) {
        uVar1 = (uint)(byte)pcVar6[9];
        pcVar5 = pcVar6 + 8;
LAB_0001604c:
        do {
LAB_00016068:
          do {
            if (uVar1 == 0x21) {
              iVar2 = cpu::read_direct((cpu *)**(undefined4 **)(this + 0x4000c),
                                       *(ushort *)(pcVar5 + 2));
              if ((int)(uint)(byte)pcVar5[4] <= iVar2) {
                pcVar6 = *(cheat **)pcVar6;
                goto LAB_000160a0;
              }
LAB_00016190:
              pcVar5 = *(cheat **)(pcVar5 + 0x104);
              if (pcVar5 == (cheat *)0x0) goto LAB_0001609c;
              uVar1 = (uint)(byte)pcVar5[1];
              goto LAB_0001604c;
            }
            if (uVar1 < 0x22) {
              if (uVar1 == 0x10) {
                uVar1 = (uint)*(ushort *)(*(int *)(pcVar5 + 0x104) + 2);
                iVar2 = uVar3 - uVar1;
                if (((int)uVar1 <= (int)uVar3) &&
                   (iVar4 = *(ushort *)(pcVar5 + 2) + 1,
                   iVar2 < (int)((uint)(byte)pcVar5[4] * iVar4))) {
                  if (iVar4 == 0) {
                    trap(7);
                  }
                  if (iVar2 % iVar4 == 0) {
                    return (uint)*(byte *)(*(int *)(pcVar5 + 0x104) + 4);
                  }
                }
                goto LAB_0001609c;
              }
              if (uVar1 == 0x20) {
                uVar1 = cpu::read_direct((cpu *)**(undefined4 **)(this + 0x4000c),
                                         *(ushort *)(pcVar5 + 2));
                if ((byte)pcVar5[4] == uVar1) goto LAB_00016190;
                pcVar6 = *(cheat **)pcVar6;
                goto LAB_000160a0;
              }
              if (uVar1 == 1) {
                if (*(ushort *)(pcVar5 + 2) == uVar3) goto LAB_00016140;
                goto LAB_0001609c;
              }
              goto LAB_00016068;
            }
            if (uVar1 == 0x22) {
              iVar2 = cpu::read_direct((cpu *)**(undefined4 **)(this + 0x4000c),
                                       *(ushort *)(pcVar5 + 2));
              if ((int)(uint)(byte)pcVar5[4] < iVar2) goto LAB_00016190;
              goto LAB_0001609c;
            }
          } while (7 < uVar1 - 0x90);
        } while (*(ushort *)(pcVar5 + 2) != uVar3);
        if (0xfff < (uVar3 + 0x3000 & 0xffff)) {
LAB_00016140:
          return (uint)(byte)pcVar5[4];
        }
        iVar2 = *(int *)(**(int **)(this + 0x4000c) + 0xc160) - (**(int **)(this + 0x4000c) + 0x12);
        if (iVar2 < 0) {
          iVar2 = iVar2 + 0xfff;
        }
        if (iVar2 >> 0xc == uVar1 - 0x90) goto LAB_00016140;
      }
LAB_0001609c:
      pcVar6 = *(cheat **)pcVar6;
LAB_000160a0:
    } while (pcVar6 != this);
  }
                    /* WARNING: Could not recover jumptable at 0x00016b20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*(code *)(apu_snd::render(short*,int)::filter +
                    *(int *)(&DAT_00033c00 + ((int)uVar3 >> 0xd) * 4) + 0x7ccc))
                    (**(undefined4 **)(this + 0x4000c));
  return uVar3;
}



/* ===== FUNCTION cheat_write @ 00016260 ===== */

/* cheat::cheat_write(unsigned short, unsigned char) */

void __thiscall cheat::cheat_write(cheat *this,ushort param_1,uchar param_2)

{
  return;
}



/* ===== FUNCTION save @ 00016268 ===== */

/* cheat::save(_IO_FILE*) */

void __thiscall cheat::save(cheat *this,_IO_FILE *param_1)

{
  cheat *pcVar1;
  cheat *pcVar2;
  
  for (pcVar2 = *(cheat **)this; pcVar2 != this; pcVar2 = *(cheat **)pcVar2) {
    fprintf(param_1,"%s\n",pcVar2 + 0xd);
    pcVar1 = pcVar2 + 8;
    do {
      fprintf(param_1,"%02X%02X%02X%02X\n",(uint)(byte)pcVar1[1],(uint)(byte)pcVar1[4],
              *(ushort *)(pcVar1 + 2) & 0xff,(int)(uint)*(ushort *)(pcVar1 + 2) >> 8);
      pcVar1 = *(cheat **)(pcVar1 + 0x104);
    } while (pcVar1 != (cheat *)0x0);
    fputc(10,param_1);
  }
  return;
}



/* ===== FUNCTION load @ 0001634c ===== */

/* cheat::load(_IO_FILE*) */

void __thiscall cheat::load(cheat *this,_IO_FILE *param_1)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  short sVar5;
  short sVar6;
  cheat_dat **ppcVar7;
  cheat *pcVar8;
  char cVar9;
  short sVar10;
  short sVar11;
  int iVar12;
  char *pcVar13;
  cheat_dat *pcVar14;
  uint uVar15;
  cheat *pcVar16;
  cheat_dat *pcVar17;
  char cVar18;
  cheat_dat local_238 [264];
  undefined4 local_130;
  char local_12c;
  char local_12b;
  char local_12a;
  char local_129;
  cheat_dat *local_30 [2];
  
  pcVar8 = *(cheat **)this;
  pcVar16 = this;
  while (this != pcVar8) {
    pcVar16 = *(cheat **)pcVar8;
    operator_delete(pcVar8);
    pcVar8 = pcVar16;
  }
  pcVar17 = local_238;
  *(cheat **)this = pcVar16;
  bVar2 = false;
  *(cheat **)(this + 4) = pcVar16;
  *(undefined4 *)(this + 8) = 0;
  bVar1 = true;
  local_30[0] = pcVar17;
LAB_000163e0:
  do {
    bVar3 = false;
LAB_000163e4:
    iVar12 = feof(param_1);
    pcVar14 = local_30[0];
    while( true ) {
      local_30[0] = pcVar14;
      if (iVar12 != 0) {
        if (bVar2) {
          *(undefined4 *)(pcVar17 + 0x104) = 0;
          local_238[0] = (cheat_dat)0x1;
          add_cheat(this,pcVar14);
        }
        return;
      }
      pcVar13 = fgets((char *)&local_130,0x100,param_1);
      if ((pcVar13 == (char *)0x0) || (iVar12 = (int)(char)local_130, iVar12 == 10))
      goto LAB_0001661c;
      pcVar13 = (char *)&local_130;
      if (bVar1) break;
      while( true ) {
        if (iVar12 == 10) {
          *pcVar13 = '\0';
        }
        else {
          iVar12 = toupper(iVar12);
          *pcVar13 = (char)iVar12;
        }
        if ((cheat_dat **)(pcVar13 + 1) == local_30) break;
        iVar12 = (int)pcVar13[1];
        pcVar13 = pcVar13 + 1;
      }
      pcVar14 = pcVar17;
      if (bVar3) {
        pcVar14 = operator_new(0x108);
        *(cheat_dat **)(pcVar17 + 0x104) = pcVar14;
      }
      cVar18 = (char)local_130;
      uVar15 = (uint)(char)local_130;
      iVar12 = isalpha(uVar15);
      cVar9 = local_130._1_1_;
      if (iVar12 == 0) {
        cVar18 = (char)((uVar15 & 0xf) << 4);
        iVar12 = isalpha((int)local_130._1_1_);
        cVar4 = local_12c;
      }
      else {
        cVar18 = (cVar18 + -0x37) * '\x10';
        iVar12 = isalpha((int)local_130._1_1_);
        cVar4 = local_12c;
      }
      if (iVar12 == 0) {
        cVar9 = cVar9 + -0x30;
      }
      else {
        cVar9 = cVar9 + -0x37;
      }
      local_12c = cVar4;
      pcVar14[1] = (cheat_dat)(cVar18 + cVar9);
      iVar12 = isalpha((int)cVar4);
      cVar18 = local_12b;
      if (iVar12 == 0) {
        sVar5 = cVar4 + -0x30;
      }
      else {
        sVar5 = cVar4 + -0x37;
      }
      iVar12 = isalpha((int)local_12b);
      cVar9 = local_12a;
      if (iVar12 == 0) {
        sVar10 = cVar18 + -0x30;
      }
      else {
        sVar10 = cVar18 + -0x37;
      }
      uVar15 = (uint)local_12a;
      iVar12 = isalpha(uVar15);
      cVar18 = local_129;
      if (iVar12 == 0) {
        sVar11 = (short)((uVar15 & 0xf) << 0xc);
      }
      else {
        sVar11 = (cVar9 + -0x37) * 0x1000;
      }
      iVar12 = isalpha((int)local_129);
      cVar9 = local_130._2_1_;
      if (iVar12 == 0) {
        sVar6 = cVar18 + -0x30;
      }
      else {
        sVar6 = cVar18 + -0x37;
      }
      uVar15 = (uint)local_130._2_1_;
      *(short *)(pcVar14 + 2) = sVar6 * 0x100 + sVar11 + sVar5 * 0x10 + sVar10;
      iVar12 = isalpha(uVar15);
      cVar18 = local_130._3_1_;
      if (iVar12 == 0) {
        cVar9 = (char)((uVar15 & 0xf) << 4);
      }
      else {
        cVar9 = (cVar9 + -0x37) * '\x10';
      }
      iVar12 = isalpha((int)local_130._3_1_);
      if (iVar12 == 0) {
        cVar4 = -0x30;
      }
      else {
        cVar4 = -0x37;
      }
      pcVar14[4] = (cheat_dat)(cVar9 + cVar18 + cVar4);
      bVar2 = true;
      iVar12 = feof(param_1);
      bVar3 = true;
      pcVar17 = pcVar14;
      pcVar14 = local_30[0];
    }
    ppcVar7 = (cheat_dat **)&local_130;
    while (ppcVar7 = (cheat_dat **)((int)ppcVar7 + 1), local_30 != ppcVar7) {
      if (*(char *)ppcVar7 == '\n') {
        *(char *)ppcVar7 = '\0';
      }
    }
    strcpy((char *)(pcVar17 + 5),(char *)&local_130);
    bVar1 = false;
  } while( true );
LAB_0001661c:
  if (bVar2) goto code_r0x00016624;
  goto LAB_000163e4;
code_r0x00016624:
  *(undefined4 *)(pcVar17 + 0x104) = 0;
  pcVar17 = local_30[0];
  local_238[0] = (cheat_dat)0x1;
  bVar2 = false;
  add_cheat(this,local_30[0]);
  bVar1 = true;
  goto LAB_000163e0;
}



/* ===== FUNCTION ~cpu @ 00016740 ===== */

/* cpu::~cpu() */

void __thiscall cpu::~cpu(cpu *this)

{
  return;
}



/* ===== FUNCTION reset @ 00016748 ===== */

/* cpu::reset() */

void __thiscall cpu::reset(cpu *this)

{
  undefined2 uVar1;
  int iVar2;
  
  iVar2 = *(int *)(*(int *)(*(int *)this + 0xc) + 0x20);
  if (iVar2 < 3) {
    uVar1 = 0x13;
    *(undefined2 *)(this + 4) = 0x1b0;
  }
  else {
    uVar1 = 0x13;
    if (iVar2 != 3) {
      uVar1 = 0x113;
    }
    *(undefined2 *)(this + 4) = 0x11b0;
  }
  *(undefined2 *)(this + 6) = uVar1;
  this[0x10] = (cpu)0x0;
  *(undefined2 *)(this + 8) = 0xd8;
  *(undefined2 *)(this + 10) = 0x14d;
  *(undefined2 *)(this + 0xc) = 0xfffe;
  *(undefined2 *)(this + 0xe) = 0x100;
  *(cpu **)(this + 0xc160) = this + 0x1012;
  this[0xc79c] = (cpu)0x0;
  *(undefined4 *)(this + 0xc798) = 0;
  *(cpu **)(this + 0xc15c) = this + 0x8012;
  this[0xc794] = (cpu)0x0;
  *(undefined4 *)(this + 0xc790) = 0;
  this[0xc77f] = (cpu)0x0;
  this[0xc77e] = (cpu)0x0;
  this[0xc77d] = (cpu)0x0;
  this[0xc77c] = (cpu)0x0;
  *(undefined4 *)(this + 0xc778) = 0;
  *(undefined4 *)(this + 0xc770) = 0;
  *(undefined4 *)(this + 0xc76c) = 0;
  *(undefined4 *)(this + 0xc768) = 0;
  *(undefined4 *)(this + 0xc774) = 0x7fffffff;
  memset(this + 0x12,0,0x8000);
  memset(this + 0x8012,0,0x4000);
  memset(this + 0xc012,0,0x80);
  memset(this + 0xc092,0,0xa0);
  memset(this + 0xc132,0,0x18);
  *(undefined4 *)(this + 0xc364) = 0x1cc;
  *(undefined4 *)(this + 0xc368) = 0;
  *(undefined4 *)(this + 0xc764) = 1;
  return;
}



/* ===== FUNCTION cpu @ 000168dc ===== */

/* cpu::cpu(gb*) */

void __thiscall cpu::cpu(cpu *this,gb *param_1)

{
  bool bVar1;
  undefined2 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  cpu *pcVar11;
  byte bVar12;
  
  *(gb **)this = param_1;
  uVar5 = 0;
  pcVar11 = this + 0xc164;
  uVar7 = 0;
  this[0xc780] = (cpu)0x0;
  bVar12 = 0;
  do {
    uVar10 = uVar5 & 0x10;
    uVar9 = uVar5 & 2;
    uVar4 = uVar5 & 0x80;
    uVar8 = uVar5 & 1;
    uVar6 = uVar5 & 0x20;
    bVar1 = uVar7 != 0;
    uVar5 = uVar5 + 1;
    uVar7 = uVar5 & 0x40;
    *pcVar11 = (cpu)(bVar12 | (byte)(uVar8 << 4) | (uVar10 != 0) << 5 | (uVar9 != 0) << 6);
    bVar12 = 0x80;
    if (uVar7 == 0) {
      bVar12 = 0;
    }
    pcVar11[0x100] = (cpu)((uVar6 != 0) << 4 | bVar1 << 1 | (uVar4 != 0) << 6 | uVar10 != 0);
    pcVar11 = pcVar11 + 1;
  } while (uVar5 != 0x100);
  iVar3 = *(int *)(*(int *)(*(int *)this + 0xc) + 0x20);
  if (iVar3 < 3) {
    uVar2 = 0x13;
    *(undefined2 *)(this + 4) = 0x1b0;
  }
  else {
    uVar2 = 0x13;
    if (iVar3 != 3) {
      uVar2 = 0x113;
    }
    *(undefined2 *)(this + 4) = 0x11b0;
  }
  *(undefined2 *)(this + 6) = uVar2;
  this[0x10] = (cpu)0x0;
  *(undefined2 *)(this + 8) = 0xd8;
  *(undefined2 *)(this + 10) = 0x14d;
  *(undefined2 *)(this + 0xc) = 0xfffe;
  *(undefined2 *)(this + 0xe) = 0x100;
  *(cpu **)(this + 0xc160) = this + 0x1012;
  this[0xc79c] = (cpu)0x0;
  *(undefined4 *)(this + 0xc798) = 0;
  *(cpu **)(this + 0xc15c) = this + 0x8012;
  this[0xc794] = (cpu)0x0;
  *(undefined4 *)(this + 0xc790) = 0;
  this[0xc77f] = (cpu)0x0;
  this[0xc77e] = (cpu)0x0;
  this[0xc77d] = (cpu)0x0;
  this[0xc77c] = (cpu)0x0;
  *(undefined4 *)(this + 0xc778) = 0;
  *(undefined4 *)(this + 0xc770) = 0;
  *(undefined4 *)(this + 0xc76c) = 0;
  *(undefined4 *)(this + 0xc768) = 0;
  *(undefined4 *)(this + 0xc774) = 0x7fffffff;
  memset(this + 0x12,0,0x8000);
  memset(this + 0x8012,0,0x4000);
  memset(this + 0xc012,0,0x80);
  memset(this + 0xc092,0,0xa0);
  memset(this + 0xc132,0,0x18);
  *(undefined4 *)(this + 0xc364) = 0x1cc;
  *(undefined4 *)(this + 0xc368) = 0;
  *(undefined4 *)(this + 0xc764) = 1;
  return;
}



/* ===== FUNCTION save_state @ 0001699c ===== */

/* cpu::save_state(int*) */

void __thiscall cpu::save_state(cpu *this,int *param_1)

{
  cpu cVar1;
  cpu cVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = *(int *)(this + 0xc160) - (int)(this + 0x12);
  cVar1 = this[0xc77f];
  iVar4 = *(int *)(this + 0xc15c) - (int)(this + 0x8012);
  cVar2 = this[0xc77e];
  param_1[2] = (uint)(byte)this[0xc77d];
  param_1[3] = (uint)(byte)cVar1;
  if (iVar3 < 0) {
    iVar3 = iVar3 + 0xfff;
  }
  if (iVar4 < 0) {
    iVar4 = iVar4 + 0x1fff;
  }
  *param_1 = iVar3 >> 0xc;
  param_1[1] = iVar4 >> 0xd;
  param_1[4] = *(int *)(this + 0xc784);
  param_1[5] = *(int *)(this + 0xc788);
  iVar3 = *(int *)(this + 0xc78c);
  param_1[7] = (uint)(byte)cVar2;
  param_1[6] = iVar3;
  return;
}



/* ===== FUNCTION save_state_ex @ 00016a1c ===== */

/* cpu::save_state_ex(int*) */

void __thiscall cpu::save_state_ex(cpu *this,int *param_1)

{
  *param_1 = *(int *)(this + 0xc778);
  param_1[1] = *(int *)(this + 0xc76c);
  param_1[2] = *(int *)(this + 0xc770);
  param_1[3] = *(int *)(this + 0xc768);
  return;
}



/* ===== FUNCTION restore_state @ 00016a48 ===== */

/* cpu::restore_state(int*) */

void __thiscall cpu::restore_state(cpu *this,int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = *param_1;
  iVar3 = param_1[1];
  iVar2 = param_1[2];
  iVar1 = param_1[3];
  *(int *)(this + 0xc784) = param_1[4];
  *(int *)(this + 0xc788) = param_1[5];
  iVar4 = param_1[6];
  this[0xc77f] = (cpu)(iVar1 != 0);
  *(cpu **)(this + 0xc15c) = this + iVar3 * 0x2000 + 0x8012;
  *(int *)(this + 0xc78c) = iVar4;
  *(cpu **)(this + 0xc160) = this + iVar5 * 0x1000 + 0x12;
  this[0xc77d] = (cpu)(iVar2 != 0);
  this[0xc77e] = (cpu)(param_1[7] != 0);
  return;
}



/* ===== FUNCTION restore_state_ex @ 00016abc ===== */

/* cpu::restore_state_ex(int*) */

void __thiscall cpu::restore_state_ex(cpu *this,int *param_1)

{
  *(int *)(this + 0xc778) = *param_1;
  *(int *)(this + 0xc76c) = param_1[1];
  *(int *)(this + 0xc770) = param_1[2];
  *(int *)(this + 0xc768) = param_1[3];
  return;
}



/* ===== FUNCTION read_direct @ 00016ae8 ===== */

/* cpu::read_direct(unsigned short) */

void __thiscall cpu::read_direct(cpu *this,ushort param_1)

{
  undefined2 in_register_00000016;
  
                    /* WARNING: Could not recover jumptable at 0x00016b20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(apu_snd::render(short*,int)::filter +
            *(int *)(&DAT_00033c00 + (CONCAT22(in_register_00000016,param_1) >> 0xd) * 4) + 0x7ccc))
            ();
  return;
}



/* ===== FUNCTION write @ 000171d0 ===== */

/* cpu::write(unsigned short, unsigned char) */

void __thiscall cpu::write(cpu *this,ushort param_1,uchar param_2)

{
  ushort uVar1;
  byte bVar2;
  undefined2 uVar3;
  int iVar4;
  uint uVar5;
  mbc *this_00;
  ushort *puVar6;
  undefined2 in_register_00000016;
  cpu *pcVar7;
  undefined3 in_register_00000019;
  int iVar8;
  int iVar9;
  uint uVar10;
  
  iVar8 = CONCAT31(in_register_00000019,param_2);
  iVar9 = CONCAT22(in_register_00000016,param_1);
  iVar4 = iVar9 >> 0xd;
  if (iVar4 == 5) {
    this_00 = *(mbc **)(*(int *)this + 0x10);
    if (this_00[0x11] == (mbc)0x0) {
      mbc::ext_write(this_00,param_1,param_2);
      return;
    }
    *(uchar *)(*(int *)(this_00 + 4) + (param_1 & 0x1fff)) = param_2;
    return;
  }
  if (iVar4 < 6) {
    if (iVar4 == 4) {
      *(uchar *)(*(int *)(this + 0xc15c) + (param_1 & 0x1fff)) = param_2;
      return;
    }
LAB_000172ac:
    mbc::write((ushort)*(undefined4 *)(*(int *)this + 0x10),(uchar)param_1);
    return;
  }
  if (iVar4 == 6) {
LAB_0001727c:
    if ((param_1 & 0x1000) == 0) {
      this[(param_1 & 0xfff) + 0x12] = (cpu)param_2;
      return;
    }
    *(uchar *)(*(int *)(this + 0xc160) + (param_1 & 0xfff)) = param_2;
    return;
  }
  if (iVar4 != 7) goto LAB_000172ac;
  if (iVar9 < 0xfe00) goto LAB_0001727c;
  if (iVar9 < 0xfea0) {
    this[iVar9 + -0x3d6e] = (cpu)param_2;
    return;
  }
  if (iVar9 < 0xff00) {
    this[((iVar9 + -0xffa0 >> 5) << 3 | param_1 & 7) + 0xc132] = (cpu)param_2;
    return;
  }
  if ((0xff7f < iVar9) && (iVar9 != 0xffff)) {
    this[iVar9 + -0x3f6e] = (cpu)param_2;
    return;
  }
  uVar1 = (ushort)iVar8;
  switch(iVar9 + 0x100U & 0xffff) {
  case 0:
    *(uchar *)(*(int *)this + 0x20) = param_2;
    break;
  case 1:
    *(uchar *)(*(int *)this + 0x21) = param_2;
    break;
  case 2:
    iVar4 = *(int *)this;
    if (*(int *)(*(int *)(iVar4 + 0xc) + 0x20) == 1) {
      *(byte *)(iVar4 + 0x22) = param_2 & 0x81;
      if (((char)param_2 < '\0') && ((param_2 & 1) != 0)) {
        *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x200;
      }
    }
    else {
      *(byte *)(iVar4 + 0x22) = param_2 & 0x83;
      if (((char)param_2 < '\0') && ((param_2 & 1) != 0)) {
        if ((param_2 & 2) == 0) {
          *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x1000;
        }
        else {
          *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x80;
        }
      }
    }
    break;
  default:
    if ((iVar9 + 0xf0U & 0xffff) < 0x30) {
      apu::write(*(apu **)(*(int *)this + 8),param_1,param_2,*(int *)(this + 0xc768));
      return;
    }
    if ((iVar9 + 0x8fU & 0xffff) < 0xf) {
      this[iVar9 + -0x3e27] = (cpu)param_2;
    }
    break;
  case 4:
    *(undefined1 *)(*(int *)this + 0x23) = 0;
    break;
  case 5:
    *(uchar *)(*(int *)this + 0x24) = param_2;
    break;
  case 6:
    *(uchar *)(*(int *)this + 0x25) = param_2;
    break;
  case 7:
    if ((param_2 & 4) == 0) {
      iVar4 = *(int *)this;
    }
    else {
      iVar4 = *(int *)this;
      if ((*(byte *)(iVar4 + 0x26) & 4) == 0) {
        *(undefined4 *)(this + 0xc770) = 0;
        *(uchar *)(iVar4 + 0x26) = param_2;
        return;
      }
    }
    *(uchar *)(iVar4 + 0x26) = param_2;
    break;
  case 0xf:
    *(uchar *)(*(int *)this + 0x27) = param_2;
    break;
  case 0x40:
    if ((char)param_2 < '\0') {
      iVar4 = *(int *)this;
      if (-1 < *(char *)(iVar4 + 0x28)) {
        *(undefined1 *)(iVar4 + 0x2c) = 0;
        *(undefined4 *)(*(int *)(iVar4 + 4) + 0x39c) = 9;
        *(uchar *)(iVar4 + 0x28) = param_2;
        return;
      }
    }
    else {
      iVar4 = *(int *)this;
    }
    *(uchar *)(iVar4 + 0x28) = param_2;
    break;
  case 0x41:
    iVar4 = *(int *)this;
    if (*(int *)(*(int *)(iVar4 + 0xc) + 0x20) == 1) {
      bVar2 = *(byte *)(iVar4 + 0x29);
      if ((bVar2 & 2) == 0) {
        *(byte *)(iVar4 + 0x27) = *(byte *)(iVar4 + 0x27) | 2;
      }
    }
    else {
      bVar2 = *(byte *)(iVar4 + 0x29);
    }
    *(byte *)(iVar4 + 0x29) = bVar2 & 7 | param_2 & 0x78;
    break;
  case 0x42:
    *(uchar *)(*(int *)this + 0x2a) = param_2;
    break;
  case 0x43:
    *(uchar *)(*(int *)this + 0x2b) = param_2;
    break;
  case 0x44:
    *(undefined4 *)(*(int *)(*(int *)this + 4) + 0x39c) = 9;
    break;
  case 0x45:
    *(uchar *)(*(int *)this + 0x2d) = param_2;
    break;
  case 0x46:
                    /* WARNING: Could not recover jumptable at 0x00024f80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_000357b0 + (iVar8 >> 5) * 4) + 0x7ccc))();
    return;
  case 0x47:
    *(uchar *)(*(int *)this + 0x2f) = param_2;
    break;
  case 0x48:
    *(uchar *)(*(int *)this + 0x30) = param_2;
    break;
  case 0x49:
    *(uchar *)(*(int *)this + 0x31) = param_2;
    break;
  case 0x4a:
    *(uchar *)(*(int *)this + 0x32) = param_2;
    break;
  case 0x4b:
    *(uchar *)(*(int *)this + 0x33) = param_2;
    break;
  case 0x4d:
    *(byte *)(*(int *)this + 0x35) = param_2 & 1;
    this[0xc77e] = (cpu)(param_2 & 1);
    break;
  case 0x4f:
    if (this[0xc77f] == (cpu)0x0) {
      *(cpu **)(this + 0xc15c) = this + (param_2 & 1) * 0x2000 + 0x8012;
      *(uchar *)(*(int *)this + 0x36) = param_2;
    }
    break;
  case 0x51:
    *(uint *)(this + 0xc784) = *(uint *)(this + 0xc784) & 0xf0 | iVar8 << 8;
    break;
  case 0x52:
    *(uint *)(this + 0xc784) = *(uint *)(this + 0xc784) & 0xff00 | param_2 & 0xf0;
    break;
  case 0x53:
    *(uint *)(this + 0xc788) = *(uint *)(this + 0xc788) & 0xf0 | iVar8 << 8;
    break;
  case 0x54:
    *(uint *)(this + 0xc788) = *(uint *)(this + 0xc788) & 0xff00 | param_2 & 0xf0;
    break;
  case 0x55:
    uVar10 = *(uint *)(this + 0xc784);
    if ((uVar10 - 0x8000 < 0x2000) || (0xdfff < (int)uVar10)) {
      *(undefined1 *)(*(int *)this + 0x3b) = 0;
      return;
    }
    if ((char)param_2 < '\0') {
      if (this[0xc77f] == (cpu)0x0) {
        this[0xc77f] = (cpu)0x1;
        *(uint *)(this + 0xc78c) = (param_2 & 0x7f) + 1;
        this[0xc794] = (cpu)0x1;
        *(undefined1 *)(*(int *)this + 0x3b) = 0;
        return;
      }
LAB_00025760:
      this[0xc77f] = (cpu)0x0;
      *(undefined4 *)(this + 0xc78c) = 0;
      *(undefined1 *)(*(int *)this + 0x3b) = 0xff;
      return;
    }
    if (this[0xc77f] != (cpu)0x0) goto LAB_00025760;
    iVar4 = *(int *)this;
    uVar5 = *(uint *)(this + 0xc788);
    *(undefined4 *)(this + 0xc78c) = 0;
    *(undefined1 *)(iVar4 + 0x3b) = 0xff;
    switch((int)uVar10 >> 0xd) {
    case 0:
    case 1:
      iVar4 = *(int *)(*(int *)(iVar4 + 0xc) + 0x2c);
      goto LAB_000255e0;
    case 2:
    case 3:
      iVar4 = **(int **)(iVar4 + 0x10);
LAB_000255e0:
      memcpy((void *)(*(int *)(this + 0xc15c) + (uVar5 & 0x1ff0)),(void *)(iVar4 + uVar10),
             (iVar8 + 1) * 0x10);
      break;
    default:
      break;
    case 5:
      pcVar7 = *(cpu **)(*(int *)(iVar4 + 0x10) + 4);
      uVar10 = uVar10 & 0x1fff;
      iVar4 = *(int *)(this + 0xc15c);
LAB_00025530:
      memcpy((void *)(iVar4 + (uVar5 & 0x1ff0)),pcVar7 + uVar10,(iVar8 + 1) * 0x10);
      break;
    case 6:
      if ((uVar10 & 0x1000) == 0) {
        pcVar7 = this + 0x12;
        uVar10 = uVar10 & 0xfff;
        iVar4 = *(int *)(this + 0xc15c);
        goto LAB_00025530;
      }
      memcpy((void *)(*(int *)(this + 0xc15c) + (uVar5 & 0x1ff0)),
             (void *)(*(int *)(this + 0xc160) + (uVar10 & 0xfff)),(iVar8 + 1) * 0x10);
    }
    iVar4 = (iVar8 + 1) * 0x10;
    *(int *)(this + 0xc784) = *(int *)(this + 0xc784) + iVar4;
    *(int *)(this + 0xc788) = *(int *)(this + 0xc788) + iVar4;
    *(uint *)(this + 0xc790) = ((this[0xc77d] != (cpu)0x0) + 1) * (iVar8 + 1) * 0x20 + 0x390;
    break;
  case 0x56:
    iVar4 = *(int *)(this + 0xc764);
    uVar1 = *(ushort *)(this + 0xc76c);
    *(int *)(this + 0xc764) = iVar4 + 1;
    *(undefined4 *)(this + iVar4 * 4 + 0xc368) = 0;
    *(uint *)(this + iVar4 * 4 + 0xc364) = (uint)uVar1 | iVar8 << 0x10;
    *(uchar *)(*(int *)this + 0x3c) = param_2;
    break;
  case 0x68:
    *(uchar *)(*(int *)this + 0x3d) = param_2;
    break;
  case 0x69:
    iVar4 = *(int *)this;
    bVar2 = *(byte *)(iVar4 + 0x3d);
    iVar9 = *(int *)(iVar4 + 4);
    uVar10 = (int)(uint)bVar2 >> 3;
    uVar5 = bVar2 & 6;
    if ((bVar2 & 1) == 0) {
      puVar6 = (ushort *)(iVar9 + ((uVar10 & 7) + 3) * 8 + uVar5);
      *puVar6 = *puVar6 & 0xff00 | uVar1;
    }
    else {
      puVar6 = (ushort *)(iVar9 + ((uVar10 & 7) + 3) * 8 + uVar5);
      *puVar6 = (ushort)(byte)*puVar6 | uVar1 << 8;
    }
    uVar3 = (**(code **)(**(int **)(iVar4 + 0x14) + 0x10))(*(int **)(iVar4 + 0x14));
    iVar4 = *(int *)this;
    *(undefined2 *)(iVar9 + ((uVar10 & 7) + 0x13) * 8 + uVar5) = uVar3;
    *(uchar *)(iVar4 + 0x3e) = param_2;
    if (*(char *)(iVar4 + 0x3d) < '\0') {
      *(byte *)(iVar4 + 0x3d) = *(char *)(iVar4 + 0x3d) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6a:
    *(uchar *)(*(int *)this + 0x3f) = param_2;
    break;
  case 0x6b:
    iVar4 = *(int *)this;
    uVar5 = (uint)*(byte *)(iVar4 + 0x3f);
    uVar10 = (int)uVar5 >> 3;
    if ((*(byte *)(iVar4 + 0x3f) & 1) == 0) {
      iVar9 = *(int *)(iVar4 + 4);
      puVar6 = (ushort *)(iVar9 + ((uVar10 & 7) + 0xb) * 8 + (uVar5 & 6));
      *puVar6 = *puVar6 & 0xff00 | uVar1;
    }
    else {
      iVar9 = *(int *)(iVar4 + 4);
      puVar6 = (ushort *)(iVar9 + ((uVar10 & 7) + 0xb) * 8 + (uVar5 & 6));
      *puVar6 = (ushort)(byte)*puVar6 | uVar1 << 8;
    }
    uVar3 = (**(code **)(**(int **)(iVar4 + 0x14) + 0x10))(*(int **)(iVar4 + 0x14));
    iVar4 = *(int *)this;
    *(undefined2 *)(iVar9 + ((uVar10 & 7) + 0x1b) * 8 + (uVar5 & 6)) = uVar3;
    *(uchar *)(iVar4 + 0x40) = param_2;
    if (*(char *)(iVar4 + 0x3f) < '\0') {
      *(byte *)(iVar4 + 0x3f) = *(char *)(iVar4 + 0x3f) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6c:
    this[0xc7a8] = (cpu)(param_2 & 1);
    break;
  case 0x70:
    uVar10 = param_2 & 7;
    iVar4 = 0x1000;
    if ((param_2 & 7) == 0) {
      uVar10 = 1;
    }
    else {
      iVar4 = uVar10 << 0xc;
    }
    *(char *)(*(int *)this + 0x41) = (char)uVar10;
    *(cpu **)(this + 0xc160) = this + iVar4 + 0x12;
    break;
  case 0x72:
    this[0xc7a9] = (cpu)param_2;
    break;
  case 0x73:
    this[0xc7aa] = (cpu)param_2;
    break;
  case 0x74:
    this[0xc7ab] = (cpu)param_2;
    break;
  case 0x75:
    this[0xc7ac] = (cpu)(param_2 & 0x70);
    break;
  case 0xff:
    *(uchar *)(*(int *)this + 0x34) = param_2;
  }
  return;
}



/* ===== FUNCTION irq @ 0001733c ===== */

/* cpu::irq(int) */

void __thiscall cpu::irq(cpu *this,int param_1)

{
  int iVar1;
  
  if (param_1 - 1U < 2) {
    iVar1 = *(int *)this;
    if (-1 < *(char *)(iVar1 + 0x28)) {
      return;
    }
  }
  else {
    iVar1 = *(int *)this;
  }
  *(byte *)(iVar1 + 0x27) = (byte)param_1 | *(byte *)(iVar1 + 0x27);
  return;
}



/* ===== FUNCTION seri_send @ 00017378 ===== */

/* cpu::seri_send(unsigned char) */

undefined1 __thiscall cpu::seri_send(cpu *this,uchar param_1)

{
  undefined1 uVar1;
  int iVar2;
  
  iVar2 = *(int *)this;
  if ((*(byte *)(iVar2 + 0x22) & 0x81) != 0x80) {
    return 0xff;
  }
  uVar1 = *(undefined1 *)(iVar2 + 0x21);
  *(byte *)(iVar2 + 0x22) = *(byte *)(iVar2 + 0x22) & 1;
  *(uchar *)(iVar2 + 0x21) = param_1;
  irq(this,8);
  return uVar1;
}



/* ===== FUNCTION exec @ 000173e4 ===== */

/* cpu::exec(int) */

void __thiscall cpu::exec(cpu *this,int param_1)

{
  undefined2 uVar1;
  byte bVar2;
  cpu cVar3;
  char cVar4;
  uchar uVar5;
  byte bVar6;
  undefined1 uVar7;
  ushort uVar8;
  uint uVar9;
  short sVar10;
  int iVar11;
  cheat *pcVar12;
  uint uVar13;
  ushort uVar14;
  int iVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  int iVar19;
  
  iVar15 = param_1 << (this[0xc77d] != (cpu)0x0);
  *(undefined4 *)(this + 0xc764) = 1;
  *(undefined4 *)(this + 0xc368) = 0;
  iVar16 = iVar15 + *(int *)(this + 0xc76c);
  *(int *)(this + 0xc364) = iVar15 + 8;
  *(int *)(this + 0xc76c) = iVar16;
  iVar15 = *(int *)(this + 0xc790);
  if (iVar15 != 0) {
    if (iVar15 < iVar16) {
      iVar16 = iVar16 - iVar15;
      *(int *)(this + 0xc76c) = iVar16;
      *(int *)(this + 0xc770) = *(int *)(this + 0xc770) + iVar15;
      *(int *)(this + 0xc768) = *(int *)(this + 0xc768) + iVar15;
      *(int *)(this + 0xc778) = *(int *)(this + 0xc778) + iVar15;
      *(undefined4 *)(this + 0xc790) = 0;
    }
    else {
      *(int *)(this + 0xc790) = iVar15 - iVar16;
      iVar15 = *(int *)(this + 0xc768) + iVar16;
      *(undefined4 *)(this + 0xc76c) = 0;
      iVar11 = *(int *)(this + 0xc778) + iVar16;
      *(int *)(this + 0xc770) = *(int *)(this + 0xc770) + iVar16;
      iVar16 = 0;
      *(int *)(this + 0xc768) = iVar15;
      *(int *)(this + 0xc778) = iVar11;
    }
  }
LAB_000174c0:
  do {
    if (iVar16 < 1) {
      return;
    }
    if (this[0xc79c] == (cpu)0x0) {
      iVar15 = *(int *)this;
      if ((*(byte *)(iVar15 + 0x27) & *(byte *)(iVar15 + 0x34)) == 0) {
LAB_00017644:
        uVar14 = *(ushort *)(this + 0xe);
        sVar10 = uVar14 + 1;
        iVar16 = (uint)uVar14 << 2;
      }
      else {
        if (this[0x10] == (cpu)0x0) {
          if (this[0xc77c] == (cpu)0x0) goto LAB_00017644;
LAB_00017504:
          sVar10 = *(short *)(this + 0xe) + 1;
          *(short *)(this + 0xe) = sVar10;
        }
        else {
          if (this[0xc77c] != (cpu)0x0) goto LAB_00017504;
          sVar10 = *(short *)(this + 0xe);
        }
        write(this,*(short *)(this + 0xc) - 2,(uchar)sVar10);
        write(this,*(short *)(this + 0xc) - 1,(uchar)((ushort)*(undefined2 *)(this + 0xe) >> 8));
        iVar15 = *(int *)this;
        *(short *)(this + 0xc) = *(short *)(this + 0xc) + -2;
        bVar6 = *(byte *)(iVar15 + 0x27);
        bVar2 = bVar6 & *(byte *)(iVar15 + 0x34);
        if ((bVar2 & 1) == 0) {
          if ((bVar2 & 2) == 0) {
            if ((bVar2 & 4) == 0) {
              if ((bVar2 & 8) == 0) {
                if ((bVar2 & 0x10) == 0) {
                  uVar14 = *(ushort *)(this + 0xe);
                  sVar10 = uVar14 + 1;
                  iVar16 = (uint)uVar14 << 2;
                }
                else {
                  *(byte *)(iVar15 + 0x27) = bVar6 & 0xef;
                  iVar16 = 0x180;
                  *(undefined4 *)(this + 0xc798) = 0x10;
                  sVar10 = 0x61;
                  uVar14 = 0x60;
                }
              }
              else {
                *(byte *)(iVar15 + 0x27) = bVar6 & 0xf7;
                iVar16 = 0x160;
                *(undefined4 *)(this + 0xc798) = 8;
                sVar10 = 0x59;
                uVar14 = 0x58;
              }
            }
            else {
              *(byte *)(iVar15 + 0x27) = bVar6 & 0xfb;
              iVar16 = 0x140;
              *(undefined4 *)(this + 0xc798) = 4;
              sVar10 = 0x51;
              uVar14 = 0x50;
            }
          }
          else {
            *(byte *)(iVar15 + 0x27) = bVar6 & 0xfd;
            iVar16 = 0x120;
            *(undefined4 *)(this + 0xc798) = 2;
            sVar10 = 0x49;
            uVar14 = 0x48;
          }
        }
        else {
          *(byte *)(iVar15 + 0x27) = bVar6 & 0xfe;
          iVar16 = 0x100;
          *(undefined4 *)(this + 0xc798) = 1;
          sVar10 = 0x41;
          uVar14 = 0x40;
        }
        this[0xc77c] = (cpu)0x0;
        this[0x10] = (cpu)0x0;
      }
      pcVar12 = *(cheat **)(iVar15 + 0x18);
      iVar15 = *(int *)(pcVar12 + iVar16 + 0xc);
      *(short *)(this + 0xe) = sVar10;
      if (iVar15 == 0) goto LAB_000175c8;
LAB_00017630:
      iVar15 = cheat::cheat_read(pcVar12,uVar14);
    }
    else {
      uVar14 = *(ushort *)(this + 0xe);
      this[0xc79c] = (cpu)0x0;
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 != 0) goto LAB_00017630;
LAB_000175c8:
      iVar15 = read_direct(this,uVar14);
    }
    iVar16 = *(int *)(cycles + iVar15 * 4);
    switch(iVar15) {
    default:
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 1:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      *(ushort *)(this + 6) = uVar8 | sVar10 << 8;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 2:
      write(this,*(ushort *)(this + 6),(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 3:
      iVar19 = *(int *)this;
      *(short *)(this + 6) = *(short *)(this + 6) + 1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 4:
      cVar3 = (cpu)((char)this[7] + 1);
      iVar19 = *(int *)this;
      this[7] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 5:
      cVar3 = (cpu)((char)this[7] - 1);
      iVar19 = *(int *)this;
      this[7] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     ZTable[(byte)cVar3] | (byte)this[4] & 1 | 2);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 6:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[7] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 7:
      iVar19 = *(int *)this;
      cVar3 = SUB41((int)(uint)(byte)this[5] >> 7,0);
      this[4] = cVar3;
      this[5] = (cpu)((char)this[5] << 1 | (byte)cVar3);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 8:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar1 = *(undefined2 *)(this + 0xc);
      uVar8 = uVar8 | sVar10 << 8;
      write(this,uVar8,(uchar)uVar1);
      write(this,uVar8 + 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 9:
      uVar14 = *(ushort *)(this + 10);
      iVar19 = *(int *)this;
      uVar13 = (uint)uVar14 + (uint)*(ushort *)(this + 6);
      *(short *)(this + 10) = (short)uVar13;
      this[4] = (cpu)((((uVar13 & 0xffff ^ (uint)(uVar14 ^ *(ushort *)(this + 6))) & 0x1000) != 0)
                      << 4 | (byte)this[4] & 0x40 | (byte)(uVar13 >> 0x10));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 10:
      uVar14 = *(ushort *)(this + 6);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb:
      iVar19 = *(int *)this;
      *(short *)(this + 6) = *(short *)(this + 6) + -1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xc:
      cVar3 = (cpu)((char)this[6] + 1);
      iVar19 = *(int *)this;
      this[6] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xd:
      cVar3 = (cpu)((char)this[6] - 1);
      iVar19 = *(int *)this;
      this[6] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     ZTable[(byte)cVar3] | (byte)this[4] & 1 | 2);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xe:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[6] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[4] = (cpu)((byte)cVar3 & 1);
      this[5] = (cpu)((byte)cVar3 >> 1 | (char)cVar3 << 7);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x10:
      if (this[0xc77e] == (cpu)0x0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        this[0xc77c] = (cpu)0x1;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + -1;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        this[0xc77d] = (cpu)((byte)this[0xc77d] ^ 1);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        this[0xc77e] = (cpu)0x0;
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 1;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0x11:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      *(ushort *)(this + 8) = uVar8 | sVar10 << 8;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x12:
      write(this,*(ushort *)(this + 8),(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x13:
      iVar19 = *(int *)this;
      *(short *)(this + 8) = *(short *)(this + 8) + 1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x14:
      cVar3 = (cpu)((char)this[9] + 1);
      iVar19 = *(int *)this;
      this[9] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x15:
      cVar3 = (cpu)((char)this[9] - 1);
      iVar19 = *(int *)this;
      this[9] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     (byte)this[4] & 1 | 2 | ZTable[(byte)cVar3]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x16:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[9] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x17:
      cVar3 = this[4];
      iVar19 = *(int *)this;
      this[4] = (cpu)((byte)this[5] >> 7);
      this[5] = (cpu)((byte)cVar3 & 1 | (char)this[5] << 1);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x18:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar4 = read_direct(this,uVar14);
      }
      else {
        cVar4 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xe) = *(short *)(this + 0xe) + (short)cVar4;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0x19:
      uVar14 = *(ushort *)(this + 10);
      iVar19 = *(int *)this;
      uVar13 = (uint)uVar14 + (uint)*(ushort *)(this + 8);
      *(short *)(this + 10) = (short)uVar13;
      this[4] = (cpu)((((uVar13 & 0xffff ^ (uint)(uVar14 ^ *(ushort *)(this + 8))) & 0x1000) != 0)
                      << 4 | (byte)this[4] & 0x40 | (byte)(uVar13 >> 0x10));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1a:
      uVar14 = *(ushort *)(this + 8);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1b:
      iVar19 = *(int *)this;
      *(short *)(this + 8) = *(short *)(this + 8) + -1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1c:
      cVar3 = (cpu)((char)this[8] + 1);
      iVar19 = *(int *)this;
      this[8] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1d:
      cVar3 = (cpu)((char)this[8] - 1);
      iVar19 = *(int *)this;
      this[8] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     ZTable[(byte)cVar3] | (byte)this[4] & 1 | 2);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1e:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[8] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x1f:
      cVar3 = this[4];
      iVar19 = *(int *)this;
      this[4] = (cpu)((byte)this[5] & 1);
      this[5] = (cpu)((char)cVar3 << 7 | (byte)((int)(uint)(byte)this[5] >> 1));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x20:
      if (((byte)this[4] & 0x40) == 0) {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 1;
        if (iVar15 == 0) {
          cVar4 = read_direct(this,uVar14);
        }
        else {
          cVar4 = cheat::cheat_read(pcVar12,uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0xc;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + (short)cVar4;
        iVar11 = *(int *)(this + 0xc76c) + -0xc;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 1;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0x21:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      *(ushort *)(this + 10) = uVar8 | sVar10 << 8;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x22:
      write(this,*(ushort *)(this + 10),(uchar)this[5]);
      iVar19 = *(int *)this;
      *(short *)(this + 10) = *(short *)(this + 10) + 1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x23:
      iVar19 = *(int *)this;
      *(short *)(this + 10) = *(short *)(this + 10) + 1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x24:
      cVar3 = (cpu)((char)this[0xb] + 1);
      iVar19 = *(int *)this;
      this[0xb] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x25:
      cVar3 = (cpu)((char)this[0xb] - 1);
      iVar19 = *(int *)this;
      this[0xb] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     ZTable[(byte)cVar3] | (byte)this[4] & 1 | 2);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x26:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[0xb] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x27:
      cVar3 = this[4];
      uVar13 = (uint)(byte)this[5];
      if (((byte)cVar3 & 2) == 0) {
        if (((byte)cVar3 & 1) == 0) {
          uVar17 = 0x600;
          if (((byte)cVar3 & 0x10) == 0) {
            if ((uVar13 & 0xf) < 10) {
              uVar17 = 0x6001;
              if (uVar13 < 0xa0) {
                uVar17 = 0;
              }
            }
            else if (0x8f < uVar13) {
              uVar17 = 0x6601;
            }
          }
          else if (0x9f < uVar13) {
            uVar17 = 0x6601;
          }
        }
        else {
          uVar17 = 0x6601;
          if (((byte)cVar3 & 0x10) == 0) {
            if ((uVar13 & 0xf) < 10) {
              uVar17 = 0x6001;
            }
          }
          else {
            uVar17 = 0x6601;
          }
        }
      }
      else if (((byte)cVar3 & 1) == 0) {
        uVar17 = 64000;
        if (((byte)cVar3 & 0x10) == 0) {
          uVar17 = 0;
        }
      }
      else {
        uVar17 = 0xa001;
        if (((byte)cVar3 & 0x10) != 0) {
          uVar17 = 0x9a01;
        }
      }
      iVar19 = *(int *)this;
      uVar13 = uVar13 + (uVar17 >> 8);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)cVar3 & 2 | (byte)uVar17 | ZTable[uVar13 & 0xff]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x28:
      if (((byte)this[4] & 0x40) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 1;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 1;
        if (iVar15 == 0) {
          cVar4 = read_direct(this,uVar14);
        }
        else {
          cVar4 = cheat::cheat_read(pcVar12,uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0xc;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + (short)cVar4;
        iVar11 = *(int *)(this + 0xc76c) + -0xc;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0x29:
      iVar19 = *(int *)this;
      uVar13 = (uint)*(ushort *)(this + 10) << 1;
      *(short *)(this + 10) = (short)uVar13;
      this[4] = (cpu)(((uVar13 & 0x1000) != 0) << 4 | (byte)this[4] & 0x40 | (byte)(uVar13 >> 0x10))
      ;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2a:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      *(short *)(this + 10) = *(short *)(this + 10) + 1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2b:
      iVar19 = *(int *)this;
      *(short *)(this + 10) = *(short *)(this + 10) + -1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2c:
      cVar3 = (cpu)((char)this[10] + 1);
      iVar19 = *(int *)this;
      this[10] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2d:
      cVar3 = (cpu)((char)this[10] - 1);
      iVar19 = *(int *)this;
      this[10] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     (byte)this[4] & 1 | 2 | ZTable[(byte)cVar3]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2e:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[10] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x2f:
      iVar19 = *(int *)this;
      this[5] = (cpu)~(byte)this[5];
      this[4] = (cpu)((byte)this[4] | 0x12);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x30:
      if (((byte)this[4] & 1) == 0) {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 1;
        if (iVar15 == 0) {
          cVar4 = read_direct(this,uVar14);
        }
        else {
          cVar4 = cheat::cheat_read(pcVar12,uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0xc;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + (short)cVar4;
        iVar11 = *(int *)(this + 0xc76c) + -0xc;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 1;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0x31:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(ushort *)(this + 0xc) = uVar8 | sVar10 << 8;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0x32:
      write(this,*(ushort *)(this + 10),(uchar)this[5]);
      iVar19 = *(int *)this;
      *(short *)(this + 10) = *(short *)(this + 10) + -1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x33:
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 1;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0x34:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar4 = read_direct(this,uVar14);
      }
      else {
        cVar4 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      bVar6 = cVar4 + 1;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[bVar6] | ((bVar6 & 0xf) == 0) << 4);
      write(this,*(ushort *)(this + 10),bVar6);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x35:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar4 = read_direct(this,uVar14);
      }
      else {
        cVar4 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      bVar6 = cVar4 - 1;
      this[4] = (cpu)(((bVar6 & 0xf) == 0xf) << 4 | ZTable[bVar6] | (byte)this[4] & 1 | 2);
      write(this,*(ushort *)(this + 10),bVar6);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x36:
      uVar14 = *(ushort *)(this + 0xe);
      uVar8 = *(ushort *)(this + 10);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        uVar5 = read_direct(this,uVar14);
      }
      else {
        uVar5 = cheat::cheat_read(pcVar12,uVar14);
      }
      write(this,uVar8,uVar5);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x37:
      iVar19 = *(int *)this;
      this[4] = (cpu)((byte)this[4] & 0xec | 1);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x38:
      if (((byte)this[4] & 1) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 1;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 1;
        if (iVar15 == 0) {
          cVar4 = read_direct(this,uVar14);
        }
        else {
          cVar4 = cheat::cheat_read(pcVar12,uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0xc;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + (short)cVar4;
        iVar11 = *(int *)(this + 0xc76c) + -0xc;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0x39:
      uVar14 = *(ushort *)(this + 10);
      iVar19 = *(int *)this;
      uVar13 = (uint)uVar14 + (uint)*(ushort *)(this + 0xc);
      *(short *)(this + 10) = (short)uVar13;
      this[4] = (cpu)((((uVar13 & 0xffff ^ (uint)(uVar14 ^ *(ushort *)(this + 0xc))) & 0x1000) != 0)
                      << 4 | (byte)this[4] & 0x40 | (byte)(uVar13 >> 0x10));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x3a:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      *(short *)(this + 10) = *(short *)(this + 10) + -1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x3b:
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + -1;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0x3c:
      cVar3 = (cpu)((char)this[5] + 1);
      iVar19 = *(int *)this;
      this[5] = cVar3;
      this[4] = (cpu)((byte)this[4] & 1 | ZTable[(byte)cVar3] | (((byte)cVar3 & 0xf) == 0) << 4);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x3d:
      cVar3 = (cpu)((char)this[5] - 1);
      iVar19 = *(int *)this;
      this[5] = cVar3;
      this[4] = (cpu)((((byte)cVar3 & 0xf) == 0xf) << 4 |
                     ZTable[(byte)cVar3] | (byte)this[4] & 1 | 2);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x3e:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x3f:
      iVar19 = *(int *)this;
      this[4] = (cpu)(((byte)this[4] ^ 1) & 0xed);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x41:
      iVar19 = *(int *)this;
      this[7] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x42:
      iVar19 = *(int *)this;
      this[7] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x43:
      iVar19 = *(int *)this;
      this[7] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x44:
      iVar19 = *(int *)this;
      this[7] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x45:
      iVar19 = *(int *)this;
      this[7] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x46:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[7] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x47:
      iVar19 = *(int *)this;
      this[7] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x48:
      iVar19 = *(int *)this;
      this[6] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4a:
      iVar19 = *(int *)this;
      this[6] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4b:
      iVar19 = *(int *)this;
      this[6] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4c:
      iVar19 = *(int *)this;
      this[6] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4d:
      iVar19 = *(int *)this;
      this[6] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[6] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x4f:
      iVar19 = *(int *)this;
      this[6] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x50:
      iVar19 = *(int *)this;
      this[9] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x51:
      iVar19 = *(int *)this;
      this[9] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x53:
      iVar19 = *(int *)this;
      this[9] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x54:
      iVar19 = *(int *)this;
      this[9] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x55:
      iVar19 = *(int *)this;
      this[9] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x56:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[9] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x57:
      iVar19 = *(int *)this;
      this[9] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x58:
      iVar19 = *(int *)this;
      this[8] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x59:
      iVar19 = *(int *)this;
      this[8] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x5a:
      iVar19 = *(int *)this;
      this[8] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x5c:
      iVar19 = *(int *)this;
      this[8] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x5d:
      iVar19 = *(int *)this;
      this[8] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x5e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[8] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x5f:
      iVar19 = *(int *)this;
      this[8] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x60:
      iVar19 = *(int *)this;
      this[0xb] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x61:
      iVar19 = *(int *)this;
      this[0xb] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x62:
      iVar19 = *(int *)this;
      this[0xb] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 99:
      iVar19 = *(int *)this;
      this[0xb] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x65:
      iVar19 = *(int *)this;
      this[0xb] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x66:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[0xb] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x67:
      iVar19 = *(int *)this;
      this[0xb] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x68:
      iVar19 = *(int *)this;
      this[10] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x69:
      iVar19 = *(int *)this;
      this[10] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x6a:
      iVar19 = *(int *)this;
      this[10] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x6b:
      iVar19 = *(int *)this;
      this[10] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x6c:
      iVar19 = *(int *)this;
      this[10] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x6e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[10] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x6f:
      iVar19 = *(int *)this;
      this[10] = this[5];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x70:
      write(this,*(ushort *)(this + 10),(uchar)this[7]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x71:
      write(this,*(ushort *)(this + 10),(uchar)this[6]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x72:
      write(this,*(ushort *)(this + 10),(uchar)this[9]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x73:
      write(this,*(ushort *)(this + 10),(uchar)this[8]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x74:
      write(this,*(ushort *)(this + 10),(uchar)this[0xb]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x75:
      write(this,*(ushort *)(this + 10),(uchar)this[10]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x76:
      iVar19 = *(int *)this;
      if ((*(byte *)(iVar19 + 0x26) & 4) != 0) {
        uVar13 = *(byte *)(iVar19 + 0x26) & 3;
        iVar18 = *(int *)(this + 0xc76c);
        iVar15 = *(int *)(exec(int)::timer_clocks + uVar13 * 4);
        uVar17 = *(int *)(this + 0xc770) + iVar18;
        if (iVar15 == 0) {
          trap(7);
        }
        uVar9 = (int)uVar17 / iVar15 + (uint)*(byte *)(iVar19 + 0x24);
        if ((uVar9 & 0xff00) != 0) {
          iVar16 = 0;
          iVar15 = (0x100 - (uint)*(byte *)(iVar19 + 0x24)) * iVar15 - *(int *)(this + 0xc770);
          *(int *)(this + 0xc768) = *(int *)(this + 0xc768) + iVar15;
          *(int *)(this + 0xc76c) = iVar18 - iVar15;
          *(undefined1 *)(iVar19 + 0x24) = *(undefined1 *)(iVar19 + 0x25);
          this[0xc77c] = (cpu)0x1;
          *(short *)(this + 0xe) = *(short *)(this + 0xe) + -1;
          irq(this,4);
          iVar19 = *(int *)this;
          iVar11 = *(int *)(this + 0xc76c);
          uVar13 = (uint)*(byte *)(iVar19 + 0x26);
          iVar18 = *(int *)(this + 0xc768);
          iVar15 = *(int *)(this + 0xc778);
          uVar17 = uVar13 & 4;
          *(uint *)(this + 0xc770) =
               *(int *)(exec(int)::timer_clocks + (uVar13 & 3) * 4) - 1U &
               iVar11 + *(int *)(this + 0xc770);
          break;
        }
        *(char *)(iVar19 + 0x24) = (char)uVar9;
        iVar16 = 0;
        uVar9 = *(uint *)(this + 0xc778);
        *(uint *)(this + 0xc770) = iVar15 - 1U & uVar17;
        iVar18 = iVar18 + *(int *)(this + 0xc768);
        this[0xc77c] = (cpu)0x1;
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + -1;
        *(int *)(this + 0xc768) = iVar18;
        *(undefined4 *)(this + 0xc76c) = 0;
        *(uint *)(this + 0xc778) = uVar9;
        goto LAB_00017854;
      }
      this[0xc77c] = (cpu)0x1;
      iVar18 = *(int *)(this + 0xc76c) + *(int *)(this + 0xc768);
      *(short *)(this + 0xe) = *(short *)(this + 0xe) + -1;
      uVar9 = *(int *)(this + 0xc76c) + *(int *)(this + 0xc778);
      *(undefined4 *)(this + 0xc76c) = 0;
      *(int *)(this + 0xc768) = iVar18;
      *(uint *)(this + 0xc778) = uVar9;
      goto LAB_0001789c;
    case 0x77:
      write(this,*(ushort *)(this + 10),(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0x78:
      iVar19 = *(int *)this;
      this[5] = this[7];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x79:
      iVar19 = *(int *)this;
      this[5] = this[6];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x7a:
      iVar19 = *(int *)this;
      this[5] = this[9];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x7b:
      iVar19 = *(int *)this;
      this[5] = this[8];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x7c:
      iVar19 = *(int *)this;
      this[5] = this[0xb];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x7d:
      iVar19 = *(int *)this;
      this[5] = this[10];
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x7e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x80:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[7];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[7]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x81:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[6];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[6]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x82:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[9];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[9]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x83:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[8];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[8]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x84:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[0xb];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[0xb]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x85:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[10];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[10]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x86:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x87:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] << 1;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | (byte)SUB41(uVar13,0) & 0x10 | ZTable[uVar13 & 0xff]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x88:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[7] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[7]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x89:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[6] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[6]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8a:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[9] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[9]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8b:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[8] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[8]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8c:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[0xb] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[0xb]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8d:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + (uint)(byte)this[10] + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[10]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + ((byte)this[4] & 1) + iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x8f:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] * 2 + ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | (byte)SUB41(uVar13,0) & 0x10 | ZTable[uVar13 & 0xff]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x90:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[7];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[7]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x91:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[6];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[6]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x92:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[9];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[9]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x93:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[8];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[8]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x94:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[0xb];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[0xb]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x95:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - (uint)(byte)this[10];
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[10]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x96:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x97:
      iVar19 = *(int *)this;
      this[5] = (cpu)0x0;
      this[4] = (cpu)0x42;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x98:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[7]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[7]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x99:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[6]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[6]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9a:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[9]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[9]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9b:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[8]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[8]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9c:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[0xb]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[0xb]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9d:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - (uint)(byte)this[10]) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)SUB41(uVar13,0) ^ (byte)cVar3 ^ (byte)this[10]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9e:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - iVar15) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0x9f:
      iVar19 = *(int *)this;
      uVar13 = -((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | (byte)SUB41(uVar13,0) & 0x10 | 2 |
                     ZTable[uVar13 & 0xff]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa0:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[7]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[7]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa1:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[6]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[6]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa2:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[9]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[9]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa3:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[8]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[8]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa4:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[0xb]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[0xb]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa5:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 & (byte)this[10]);
      this[4] = (cpu)(ZTable[(byte)cVar3 & (byte)this[10]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa6:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        uVar13 = read_direct(this,uVar14);
      }
      else {
        uVar13 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = SUB41(uVar13 & (byte)cVar3,0);
      this[4] = (cpu)(ZTable[uVar13 & (byte)cVar3] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa7:
      iVar19 = *(int *)this;
      this[4] = (cpu)(ZTable[(byte)this[5]] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa8:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[7]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[7]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xa9:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[6]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[6]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xaa:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[9]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[9]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xab:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[8]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[8]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xac:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[0xb]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[0xb]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xad:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 ^ (byte)this[10]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 ^ (byte)this[10]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xae:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        bVar6 = read_direct(this,uVar14);
      }
      else {
        bVar6 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)(bVar6 ^ (byte)cVar3);
      this[4] = *(cpu *)(ZTable + (bVar6 ^ (byte)cVar3));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xaf:
      iVar19 = *(int *)this;
      this[5] = (cpu)0x0;
      this[4] = (cpu)0x40;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb0:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[7]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[7]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb1:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[6]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[6]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb2:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[9]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[9]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb3:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[8]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[8]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb4:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[0xb]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[0xb]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb5:
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)((byte)cVar3 | (byte)this[10]);
      this[4] = *(cpu *)(ZTable + ((byte)cVar3 | (byte)this[10]));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb6:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        uVar13 = read_direct(this,uVar14);
      }
      else {
        uVar13 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      uVar13 = uVar13 & 0xff | (uint)(byte)this[5];
      this[5] = SUB41(uVar13,0);
      this[4] = *(cpu *)(ZTable + uVar13);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb7:
      iVar19 = *(int *)this;
      this[4] = *(cpu *)(ZTable + (byte)this[5]);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb8:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[7];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[7]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xb9:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[6];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[6]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xba:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[9];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[9]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xbb:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[8];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[8]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xbc:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[0xb];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[0xb]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xbd:
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - (uint)(byte)this[10];
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)uVar13 ^ (byte)this[5] ^ (byte)this[10]) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xbe:
      uVar14 = *(ushort *)(this + 10);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - iVar15;
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)this[5] ^ (byte)uVar13) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xbf:
      iVar19 = *(int *)this;
      this[4] = (cpu)0x42;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xc0:
      if (((byte)this[4] & 0x40) == 0) {
        uVar14 = *(ushort *)(this + 0xc);
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x14;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
        iVar11 = *(int *)(this + 0xc76c) + -0x14;
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
      }
      break;
    case 0xc1:
      uVar14 = *(short *)(this + 0xc) + 1;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar14 = *(ushort *)(this + 0xc);
      this[7] = cVar3;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[6] = cVar3;
      iVar18 = *(int *)(this + 0xc768);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xc2:
      if (((byte)this[4] & 0x40) == 0) {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x10;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x10;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xc3:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xc4:
      if (((byte)this[4] & 0x40) == 0) {
        sVar10 = *(short *)(this + 0xc);
        uVar14 = *(ushort *)(this + 0xe);
        uVar8 = sVar10 - 2;
        *(ushort *)(this + 0xc) = uVar8;
        write(this,uVar8,(uchar)(uVar14 + 2));
        write(this,sVar10 - 1,(uchar)(uVar14 + 2 >> 8));
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x18;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x18;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xc5:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 6);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xc6:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 199:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(undefined2 *)(this + 0xe) = 0;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 200:
      if (((byte)this[4] & 0x40) == 0) {
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
      }
      else {
        uVar14 = *(ushort *)(this + 0xc);
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x14;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
        iVar11 = *(int *)(this + 0xc76c) + -0x14;
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xc9:
      uVar14 = *(ushort *)(this + 0xc);
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xca:
      if (((byte)this[4] & 0x40) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x10;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x10;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xcb:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar16 = *(int *)(cycles_cb + iVar15 * 4);
      switch(iVar15) {
      case 0:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[7] >> 7;
        uVar13 = ((byte)this[7] & 0x7f) << 1 | uVar17;
        this[7] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 1:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[6] >> 7;
        uVar13 = ((byte)this[6] & 0x7f) << 1 | uVar17;
        this[6] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 2:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[9] >> 7;
        uVar13 = ((byte)this[9] & 0x7f) << 1 | uVar17;
        this[9] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 3:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[8] >> 7;
        uVar13 = ((byte)this[8] & 0x7f) << 1 | uVar17;
        this[8] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 4:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[0xb] >> 7;
        uVar13 = ((byte)this[0xb] & 0x7f) << 1 | uVar17;
        this[0xb] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 5:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[10] >> 7;
        uVar13 = ((byte)this[10] & 0x7f) << 1 | uVar17;
        this[10] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 6:
        iVar15 = read(this,*(ushort *)(this + 10));
        uVar13 = iVar15 << 1 | iVar15 >> 7;
        this[4] = (cpu)((byte)(iVar15 >> 7) | ZTable[uVar13 & 0xff]);
        write(this,*(ushort *)(this + 10),(uchar)uVar13);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 7:
        iVar19 = *(int *)this;
        uVar17 = (int)(uint)(byte)this[5] >> 7;
        uVar13 = ((byte)this[5] & 0x7f) << 1 | uVar17;
        this[5] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 8:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[7] & 1;
        uVar13 = (int)(uint)(byte)this[7] >> 1 | uVar17 << 7;
        this[7] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 9:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[6] & 1;
        uVar13 = (int)(uint)(byte)this[6] >> 1 | uVar17 << 7;
        this[6] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 10:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[9] & 1;
        uVar13 = (int)(uint)(byte)this[9] >> 1 | uVar17 << 7;
        this[9] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[8] & 1;
        uVar13 = (int)(uint)(byte)this[8] >> 1 | uVar17 << 7;
        this[8] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[0xb] & 1;
        uVar13 = (int)(uint)(byte)this[0xb] >> 1 | uVar17 << 7;
        this[0xb] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[10] & 1;
        uVar13 = (int)(uint)(byte)this[10] >> 1 | uVar17 << 7;
        this[10] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe:
        uVar13 = read(this,*(ushort *)(this + 10));
        uVar17 = (int)uVar13 >> 1 & 0xffU | (uVar13 & 1) << 7;
        this[4] = (cpu)((byte)(uVar13 & 1) | ZTable[uVar17]);
        write(this,*(ushort *)(this + 10),(uchar)uVar17);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xf:
        iVar19 = *(int *)this;
        uVar17 = (byte)this[5] & 1;
        uVar13 = (int)(uint)(byte)this[5] >> 1 | uVar17 << 7;
        this[5] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)uVar17 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x10:
        cVar3 = this[7];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[7] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x11:
        cVar3 = this[6];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[6] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x12:
        cVar3 = this[9];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[9] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x13:
        cVar3 = this[8];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[8] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x14:
        cVar3 = this[0xb];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[0xb] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x15:
        cVar3 = this[10];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[10] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x16:
        uVar13 = read(this,*(ushort *)(this + 10));
        uVar17 = (uVar13 & 0x7f) << 1 | (byte)this[4] & 1;
        this[4] = (cpu)((byte)((int)uVar13 >> 7) | ZTable[uVar17]);
        write(this,*(ushort *)(this + 10),(uchar)uVar17);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x17:
        cVar3 = this[5];
        iVar19 = *(int *)this;
        uVar13 = ((byte)cVar3 & 0x7f) << 1 | (byte)this[4] & 1;
        this[5] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x18:
        cVar3 = this[7];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[7] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x19:
        cVar3 = this[6];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[6] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x1a:
        cVar3 = this[9];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[9] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x1b:
        cVar3 = this[8];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[8] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x1c:
        cVar3 = this[0xb];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[0xb] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x1d:
        cVar3 = this[10];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[10] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x1e:
        iVar15 = read(this,*(ushort *)(this + 10));
        uVar13 = iVar15 >> 1 & 0xffU | ((byte)this[4] & 1) << 7;
        this[4] = (cpu)((byte)iVar15 & 1 | ZTable[uVar13]);
        write(this,*(ushort *)(this + 10),(uchar)uVar13);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x1f:
        cVar3 = this[5];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | ((byte)this[4] & 1) << 7;
        this[5] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x20:
        cVar3 = this[7];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[7] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x21:
        cVar3 = this[6];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[6] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x22:
        cVar3 = this[9];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[9] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x23:
        cVar3 = this[8];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[8] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x24:
        cVar3 = this[0xb];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[0xb] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x25:
        cVar3 = this[10];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[10] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x26:
        uVar13 = read(this,*(ushort *)(this + 10));
        iVar15 = (uVar13 & 0x7f) * 2;
        this[4] = (cpu)((byte)((int)uVar13 >> 7) | ZTable[iVar15]);
        write(this,*(ushort *)(this + 10),(uchar)iVar15);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x27:
        cVar3 = this[5];
        iVar19 = *(int *)this;
        iVar15 = ((byte)cVar3 & 0x7f) * 2;
        this[5] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)((int)(uint)(byte)cVar3 >> 7) | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x28:
        cVar3 = this[7];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[7] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x29:
        cVar3 = this[6];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[6] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x2a:
        cVar3 = this[9];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[9] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x2b:
        cVar3 = this[8];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[8] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x2c:
        cVar3 = this[0xb];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[0xb] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x2d:
        cVar3 = this[10];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[10] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x2e:
        uVar13 = read(this,*(ushort *)(this + 10));
        uVar17 = uVar13 & 0x180 | (int)uVar13 >> 1;
        this[4] = (cpu)((byte)uVar13 & 1 | ZTable[uVar17 & 0xff]);
        write(this,*(ushort *)(this + 10),(uchar)uVar17);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x2f:
        cVar3 = this[5];
        iVar19 = *(int *)this;
        uVar13 = (int)(uint)(byte)cVar3 >> 1 | (byte)cVar3 & 0x80;
        this[5] = SUB41(uVar13,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[uVar13]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x30:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[7] & 0xf) << 4 | (uint)((byte)this[7] >> 4);
        this[7] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x31:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[6] & 0xf) << 4 | (uint)((byte)this[6] >> 4);
        this[6] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x32:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[9] & 0xf) << 4 | (uint)((byte)this[9] >> 4);
        this[9] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x33:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[8] & 0xf) << 4 | (uint)((byte)this[8] >> 4);
        this[8] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x34:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[0xb] & 0xf) << 4 | (uint)((byte)this[0xb] >> 4);
        this[0xb] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x35:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[10] & 0xf) << 4 | (uint)((byte)this[10] >> 4);
        this[10] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x36:
        uVar13 = read(this,*(ushort *)(this + 10));
        uVar13 = uVar13 << 4 | uVar13 >> 4;
        this[4] = *(cpu *)(ZTable + (uVar13 & 0xff));
        write(this,*(ushort *)(this + 10),(uchar)uVar13);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x37:
        iVar19 = *(int *)this;
        uVar13 = ((byte)this[5] & 0xf) << 4 | (uint)((byte)this[5] >> 4);
        this[5] = SUB41(uVar13,0);
        this[4] = *(cpu *)(ZTable + uVar13);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x38:
        cVar3 = this[7];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[7] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x39:
        cVar3 = this[6];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[6] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x3a:
        cVar3 = this[9];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[9] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x3b:
        cVar3 = this[8];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[8] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x3c:
        cVar3 = this[0xb];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[0xb] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x3d:
        cVar3 = this[10];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[10] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x3e:
        iVar15 = read(this,*(ushort *)(this + 10));
        this[4] = (cpu)((byte)iVar15 & 1 | ZTable[iVar15 >> 1]);
        write(this,*(ushort *)(this + 10),(uchar)(iVar15 >> 1));
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x3f:
        cVar3 = this[5];
        iVar19 = *(int *)this;
        iVar15 = (int)(uint)(byte)cVar3 >> 1;
        this[5] = SUB41(iVar15,0);
        this[4] = (cpu)((byte)cVar3 & 1 | ZTable[iVar15]);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x40:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x41:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x42:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x43:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x44:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x45:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x46:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x47:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 6) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x48:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x49:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4a:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4b:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4c:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4d:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4e:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x4f:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 5) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x50:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x51:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x52:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x53:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x54:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x55:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x56:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x57:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 4) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x58:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x59:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5a:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5b:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5c:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5d:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5e:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x5f:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 3) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x60:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x61:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x62:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 99:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 100:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x65:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x66:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x67:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 2) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x68:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[7] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x69:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[6] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6a:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[9] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6b:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[8] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6c:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[0xb] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6d:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[10] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6e:
        cVar4 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(cVar4 << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x6f:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~((char)this[5] << 1) & 0x40U | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x70:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[7] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x71:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[6] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x72:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[9] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x73:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[8] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x74:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[0xb] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x75:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[10] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x76:
        bVar6 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~bVar6 & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x77:
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)this[5] & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x78:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[7] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x79:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[6] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7a:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[9] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7b:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[8] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7c:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[0xb] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7d:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[10] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7e:
        iVar15 = read(this,*(ushort *)(this + 10));
        iVar19 = *(int *)this;
        this[4] = (cpu)((byte)this[4] & 1 | 0x10 | ~(byte)(iVar15 >> 1) & 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x7f:
        iVar19 = *(int *)this;
        this[4] = (cpu)(~(byte)((int)(uint)(byte)this[5] >> 1) & 0x40 | (byte)this[4] & 1 | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x80:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x81:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x82:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x83:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x84:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x85:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x86:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xfe);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x87:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xfe);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x88:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x89:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x8a:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x8b:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x8c:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x8d:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x8e:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xfd);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x8f:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xfd);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x90:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x91:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x92:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x93:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x94:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x95:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x96:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xfb);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x97:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xfb);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x98:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x99:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x9a:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x9b:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x9c:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x9d:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0x9e:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xf7);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0x9f:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xf7);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xef);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xa7:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xef);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa8:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xa9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xaa:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xab:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xac:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xad:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xae:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xdf);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xaf:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xdf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0xbf);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xb7:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0xbf);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb8:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xb9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xba:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xbb:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xbc:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xbd:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xbe:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0x7f);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xbf:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] & 0x7f);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 1);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 199:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 1);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 200:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xc9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xca:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xcb:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xcc:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xcd:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xce:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 2);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xcf:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 2);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 4);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xd7:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 4);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd8:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xd9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xda:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xdb:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xdc:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xdd:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xde:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 8);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xdf:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 8);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 0x10);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xe7:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 0x10);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe8:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xe9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xea:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xeb:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xec:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xed:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xee:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 0x20);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xef:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 0x20);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf0:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf1:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf2:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf3:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf4:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf5:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf6:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 | 0x40);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xf7:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 0x40);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf8:
        iVar19 = *(int *)this;
        this[7] = (cpu)((byte)this[7] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xf9:
        iVar19 = *(int *)this;
        this[6] = (cpu)((byte)this[6] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xfa:
        iVar19 = *(int *)this;
        this[9] = (cpu)((byte)this[9] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xfb:
        iVar19 = *(int *)this;
        this[8] = (cpu)((byte)this[8] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xfc:
        iVar19 = *(int *)this;
        this[0xb] = (cpu)((byte)this[0xb] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xfd:
        iVar19 = *(int *)this;
        this[10] = (cpu)((byte)this[10] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      case 0xfe:
        bVar6 = read(this,*(ushort *)(this + 10));
        write(this,*(ushort *)(this + 10),bVar6 & 0x7f | 0x80);
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar17 = uVar13 & 4;
        break;
      case 0xff:
        iVar19 = *(int *)this;
        this[5] = (cpu)((byte)this[5] | 0x80);
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        uVar17 = uVar13 & 4;
        iVar15 = *(int *)(this + 0xc778);
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        break;
      default:
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
      }
      break;
    case 0xcc:
      if (((byte)this[4] & 0x40) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        sVar10 = *(short *)(this + 0xc);
        uVar14 = *(ushort *)(this + 0xe);
        uVar8 = sVar10 - 2;
        *(ushort *)(this + 0xc) = uVar8;
        write(this,uVar8,(uchar)(uVar14 + 2));
        write(this,sVar10 - 1,(uchar)(uVar14 + 2 >> 8));
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x18;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x18;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xcd:
      sVar10 = *(short *)(this + 0xc);
      uVar14 = *(ushort *)(this + 0xe);
      uVar8 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar8;
      write(this,uVar8,(uchar)(uVar14 + 2));
      write(this,sVar10 - 1,(uchar)(uVar14 + 2 >> 8));
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xce:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 + ((byte)this[4] & 1) + iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)((byte)(uVar13 >> 8) | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xcf:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 8;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xd0:
      if (((byte)this[4] & 1) == 0) {
        uVar14 = *(ushort *)(this + 0xc);
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x14;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
        iVar11 = *(int *)(this + 0xc76c) + -0x14;
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
      }
      break;
    case 0xd1:
      uVar14 = *(short *)(this + 0xc) + 1;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar14 = *(ushort *)(this + 0xc);
      this[9] = cVar3;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[8] = cVar3;
      iVar18 = *(int *)(this + 0xc768);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xd2:
      if (((byte)this[4] & 1) == 0) {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x10;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x10;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xd4:
      if (((byte)this[4] & 1) == 0) {
        sVar10 = *(short *)(this + 0xc);
        uVar14 = *(ushort *)(this + 0xe);
        uVar8 = sVar10 - 2;
        *(ushort *)(this + 0xc) = uVar8;
        write(this,uVar8,(uchar)(uVar14 + 2));
        write(this,sVar10 - 1,(uchar)(uVar14 + 2 >> 8));
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x18;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x18;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      else {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xd5:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 8);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xd6:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)cVar3 - iVar15;
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xd7:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x10;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xd8:
      if (((byte)this[4] & 1) == 0) {
        iVar19 = *(int *)this;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
      }
      else {
        uVar14 = *(ushort *)(this + 0xc);
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x14;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
        iVar11 = *(int *)(this + 0xc76c) + -0x14;
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xd9:
      uVar14 = *(ushort *)(this + 0xc);
      this[0x10] = (cpu)0x1;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      this[0xc79c] = (cpu)0x1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xda:
      if (((byte)this[4] & 1) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x10;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x10;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xdc:
      if (((byte)this[4] & 1) == 0) {
        iVar19 = *(int *)this;
        iVar18 = *(int *)(this + 0xc768);
        iVar15 = *(int *)(this + 0xc778);
        *(short *)(this + 0xe) = *(short *)(this + 0xe) + 2;
        iVar11 = *(int *)(this + 0xc76c) - iVar16;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        uVar17 = uVar13 & 4;
      }
      else {
        sVar10 = *(short *)(this + 0xc);
        uVar14 = *(ushort *)(this + 0xe);
        uVar8 = sVar10 - 2;
        *(ushort *)(this + 0xc) = uVar8;
        write(this,uVar8,(uchar)(uVar14 + 2));
        write(this,sVar10 - 1,(uchar)(uVar14 + 2 >> 8));
        uVar14 = *(ushort *)(this + 0xe);
        pcVar12 = *(cheat **)(*(int *)this + 0x18);
        iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
        *(ushort *)(this + 0xe) = uVar14 + 2;
        if (iVar15 == 0) {
          uVar8 = read_direct(this,uVar14);
        }
        else {
          uVar8 = cheat::cheat_read(pcVar12,uVar14);
        }
        uVar13 = uVar14 + 1;
        uVar14 = (ushort)uVar13;
        if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
          sVar10 = read_direct(this,uVar14);
        }
        else {
          sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
        }
        iVar19 = *(int *)this;
        iVar16 = 0x18;
        iVar18 = *(int *)(this + 0xc768);
        *(ushort *)(this + 0xe) = uVar8 | sVar10 << 8;
        uVar13 = (uint)*(byte *)(iVar19 + 0x26);
        iVar11 = *(int *)(this + 0xc76c) + -0x18;
        iVar15 = *(int *)(this + 0xc778);
        uVar17 = uVar13 & 4;
      }
      break;
    case 0xde:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      uVar13 = ((uint)(byte)cVar3 - iVar15) - ((byte)this[4] & 1);
      this[5] = SUB41(uVar13,0);
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)cVar3 ^ (byte)SUB41(uVar13,0)) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xdf:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x18;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xe0:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(pcVar12,uVar14);
      }
      write(this,sVar10 - 0x100,(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xe1:
      uVar14 = *(short *)(this + 0xc) + 1;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar14 = *(ushort *)(this + 0xc);
      this[0xb] = cVar3;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[10] = cVar3;
      iVar18 = *(int *)(this + 0xc768);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xe2:
      write(this,(byte)this[6] - 0x100,(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xe5:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 10);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xe6:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        uVar13 = read_direct(this,uVar14);
      }
      else {
        uVar13 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = SUB41(uVar13 & (byte)cVar3,0);
      this[4] = (cpu)(ZTable[uVar13 & (byte)cVar3] | 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xe7:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x20;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xe8:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar4 = read_direct(this,uVar14);
      }
      else {
        cVar4 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + (short)cVar4;
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xe9:
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = *(undefined2 *)(this + 10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xea:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      write(this,uVar8 | sVar10 << 8,(uchar)this[5]);
      iVar19 = *(int *)this;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xee:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        bVar6 = read_direct(this,uVar14);
      }
      else {
        bVar6 = cheat::cheat_read(pcVar12,uVar14);
      }
      cVar3 = this[5];
      iVar19 = *(int *)this;
      this[5] = (cpu)(bVar6 ^ (byte)cVar3);
      this[4] = *(cpu *)(ZTable + (bVar6 ^ (byte)cVar3));
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xef:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x28;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xf0:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar14 = sVar10 - 0x100;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf1:
      uVar14 = *(short *)(this + 0xc) + 1;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar14 = *(ushort *)(this + 0xc);
      this[5] = cVar3;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        uVar13 = read_direct(this,uVar14);
      }
      else {
        uVar13 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      cVar3 = this[(uVar13 & 0xf0) + 0xc264];
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + 2;
      this[4] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf2:
      uVar14 = (byte)this[6] - 0x100;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar14 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar14);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf3:
      iVar19 = *(int *)this;
      this[0x10] = (cpu)0x0;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf5:
      write(this,*(short *)(this + 0xc) - 2,(byte)this[(byte)this[4] + 0xc164] | 0xe);
      write(this,*(short *)(this + 0xc) - 1,(uchar)this[5]);
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      *(short *)(this + 0xc) = *(short *)(this + 0xc) + -2;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      uVar17 = uVar13 & 4;
      break;
    case 0xf6:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        uVar13 = read_direct(this,uVar14);
      }
      else {
        uVar13 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      uVar13 = uVar13 & 0xff | (uint)(byte)this[5];
      this[5] = SUB41(uVar13,0);
      this[4] = *(cpu *)(ZTable + uVar13);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      uVar17 = uVar13 & 4;
      break;
    case 0xf7:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x30;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xf8:
      uVar14 = *(ushort *)(this + 0xe);
      sVar10 = *(short *)(this + 0xc);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        cVar4 = read_direct(this,uVar14);
      }
      else {
        cVar4 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      *(short *)(this + 10) = sVar10 + cVar4;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xf9:
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xc) = *(undefined2 *)(this + 10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
      break;
    case 0xfa:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 2;
      if (iVar15 == 0) {
        uVar8 = read_direct(this,uVar14);
      }
      else {
        uVar8 = cheat::cheat_read(pcVar12,uVar14);
      }
      uVar13 = uVar14 + 1;
      uVar14 = (ushort)uVar13;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uVar13 & 0xffff) * 4 + 0xc) == 0) {
        sVar10 = read_direct(this,uVar14);
      }
      else {
        sVar10 = cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar14);
      }
      uVar8 = uVar8 | sVar10 << 8;
      if (*(int *)(*(cheat **)(*(int *)this + 0x18) + (uint)uVar8 * 4 + 0xc) == 0) {
        cVar3 = (cpu)read_direct(this,uVar8);
      }
      else {
        cVar3 = (cpu)cheat::cheat_read(*(cheat **)(*(int *)this + 0x18),uVar8);
      }
      iVar19 = *(int *)this;
      this[5] = cVar3;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xfb:
      iVar19 = *(int *)this;
      this[0x10] = (cpu)0x1;
      this[0xc79c] = (cpu)0x1;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xfe:
      uVar14 = *(ushort *)(this + 0xe);
      pcVar12 = *(cheat **)(*(int *)this + 0x18);
      iVar15 = *(int *)(pcVar12 + (uint)uVar14 * 4 + 0xc);
      *(ushort *)(this + 0xe) = uVar14 + 1;
      if (iVar15 == 0) {
        iVar15 = read_direct(this,uVar14);
      }
      else {
        iVar15 = cheat::cheat_read(pcVar12,uVar14);
      }
      iVar19 = *(int *)this;
      uVar13 = (uint)(byte)this[5] - iVar15;
      this[4] = (cpu)(-(char)(uVar13 >> 8) | 2U | ZTable[uVar13 & 0xff] |
                     ((byte)iVar15 ^ (byte)this[5] ^ (byte)uVar13) & 0x10);
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar18 = *(int *)(this + 0xc768);
      uVar17 = uVar13 & 4;
      iVar15 = *(int *)(this + 0xc778);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      break;
    case 0xff:
      sVar10 = *(short *)(this + 0xc);
      uVar1 = *(undefined2 *)(this + 0xe);
      uVar14 = sVar10 - 2;
      *(ushort *)(this + 0xc) = uVar14;
      write(this,uVar14,(uchar)uVar1);
      write(this,sVar10 - 1,(uchar)((ushort)uVar1 >> 8));
      iVar19 = *(int *)this;
      iVar18 = *(int *)(this + 0xc768);
      *(undefined2 *)(this + 0xe) = 0x38;
      uVar13 = (uint)*(byte *)(iVar19 + 0x26);
      iVar11 = *(int *)(this + 0xc76c) - iVar16;
      iVar15 = *(int *)(this + 0xc778);
      uVar17 = uVar13 & 4;
    }
    uVar9 = iVar16 + iVar15;
    iVar18 = iVar16 + iVar18;
    *(int *)(this + 0xc76c) = iVar11;
    *(int *)(this + 0xc768) = iVar18;
    *(uint *)(this + 0xc778) = uVar9;
    if (uVar17 != 0) {
      uVar13 = uVar13 & 3;
LAB_00017854:
      iVar15 = *(int *)(exec(int)::timer_clocks + uVar13 * 4);
      uVar13 = iVar16 + *(int *)(this + 0xc770);
      *(uint *)(this + 0xc770) = uVar13;
      if (iVar15 < (int)uVar13) {
        *(uint *)(this + 0xc770) = iVar15 - 1U & uVar13;
        cVar4 = *(char *)(iVar19 + 0x24) + '\x01';
        *(char *)(iVar19 + 0x24) = cVar4;
        if (cVar4 == '\0') {
          irq(this,4);
          *(undefined1 *)(*(int *)this + 0x24) = *(undefined1 *)(*(int *)this + 0x25);
          iVar18 = *(int *)(this + 0xc768);
          uVar9 = *(uint *)(this + 0xc778);
        }
      }
    }
LAB_0001789c:
    if ((uVar9 & 0x100) != 0) {
      *(char *)(*(int *)this + 0x23) = *(char *)(*(int *)this + 0x23) - (char)(uVar9 >> 8);
      *(uint *)(this + 0xc778) = uVar9 & 0xff;
    }
    if (*(int *)(this + 0xc774) < iVar18) {
      iVar15 = *(int *)this;
      *(undefined4 *)(this + 0xc774) = 0x7fffffff;
      if (*(undefined4 **)(iVar15 + 0x1c) == (undefined4 *)0x0) {
        if (*(char *)(iVar15 + 0x1379c) != '\0') {
          uVar7 = (**(code **)(iVar15 + 0x13784))(*(undefined1 *)(iVar15 + 0x21));
          goto LAB_0001cf04;
        }
        *(undefined1 *)(iVar15 + 0x21) = 0xff;
        *(byte *)(iVar15 + 0x22) = *(byte *)(iVar15 + 0x22) & 3;
      }
      else {
        uVar7 = seri_send((cpu *)**(undefined4 **)(iVar15 + 0x1c),*(uchar *)(iVar15 + 0x21));
LAB_0001cf04:
        iVar15 = *(int *)this;
        *(undefined1 *)(iVar15 + 0x21) = uVar7;
        *(byte *)(iVar15 + 0x22) = *(byte *)(iVar15 + 0x22) & 3;
      }
      irq(this,8);
      iVar16 = *(int *)(this + 0xc76c);
      goto LAB_000174c0;
    }
    iVar16 = *(int *)(this + 0xc76c);
  } while( true );
}



/* ===== FUNCTION serialize @ 00022af8 ===== */

/* cpu::serialize(serializer&) */

void cpu::serialize(serializer *param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  int iVar19;
  uint *puVar20;
  undefined4 *puVar21;
  uint uVar22;
  serializer *psVar23;
  uint uVar24;
  int *piVar25;
  undefined4 uVar26;
  int iVar27;
  uint uVar28;
  uint *in_a1;
  undefined4 uVar29;
  uint uVar30;
  undefined4 uVar31;
  serializer *in_a3;
  undefined4 uVar32;
  serializer *in_t0;
  undefined4 *puVar33;
  uint in_t1;
  serializer *psVar34;
  serializer *psVar35;
  serializer *psVar36;
  code *pcVar37;
  int local_20 [3];
  
  psVar36 = param_1 + 0x12;
  local_20[0] = *(int *)(param_1 + 0xc160) - (int)psVar36;
  if (local_20[0] < 0) {
    local_20[0] = local_20[0] + 0xfff;
  }
  local_20[0] = local_20[0] >> 0xc;
  switch(*in_a1) {
  case 0:
    iVar19 = *(int *)(param_1 + 0xc15c) - (int)(param_1 + 0x8012);
    *(int *)in_a1[1] = *(int *)in_a1[1] + 4;
    if (iVar19 < 0) {
      iVar19 = iVar19 + 0x1fff;
    }
    *(serializer **)(param_1 + 0xc160) = psVar36 + local_20[0] * 0x1000;
    local_20[0] = iVar19 >> 0xd;
    goto switchD_00022bf8_caseD_0;
  case 1:
    *(int *)in_a1[1] = local_20[0];
    uVar30 = *in_a1;
    in_a1[1] = in_a1[1] + 4;
    break;
  case 2:
    iVar19 = *(int *)in_a1[1];
    in_a1[1] = (uint)((int *)in_a1[1] + 1);
    *(serializer **)(param_1 + 0xc160) = psVar36 + iVar19 * 0x1000;
    goto switchD_00022bf8_caseD_2;
  case 3:
    pcVar37 = fwrite;
    goto LAB_00022b80;
  case 4:
    pcVar37 = fread;
LAB_00022b80:
    in_a3 = (serializer *)in_a1[1];
    (*pcVar37)(local_20,1,4);
    uVar30 = *in_a1;
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar30 = *in_a1;
  }
  psVar34 = param_1 + 0x8012;
  iVar19 = *(int *)(param_1 + 0xc15c) - (int)psVar34;
  *(serializer **)(param_1 + 0xc160) = psVar36 + local_20[0] * 0x1000;
  if (iVar19 < 0) {
    iVar19 = iVar19 + 0x1fff;
  }
  local_20[0] = iVar19 >> 0xd;
  switch(uVar30) {
  case 0:
switchD_00022bf8_caseD_0:
    *(int *)in_a1[1] = *(int *)in_a1[1] + 4;
    *(serializer **)(param_1 + 0xc15c) = param_1 + local_20[0] * 0x2000 + 0x8012;
    goto switchD_00022c5c_caseD_22e3c;
  case 1:
    *(int *)in_a1[1] = local_20[0];
    uVar30 = *in_a1;
    in_a1[1] = in_a1[1] + 4;
    *(serializer **)(param_1 + 0xc15c) = psVar34 + local_20[0] * 0x2000;
    if (uVar30 < 5) goto LAB_00022c44;
    goto LAB_00022da0;
  case 2:
switchD_00022bf8_caseD_2:
    local_20[0] = *(int *)in_a1[1];
    in_a1[1] = (uint)((int *)in_a1[1] + 1);
    *(serializer **)(param_1 + 0xc15c) = param_1 + local_20[0] * 0x2000 + 0x8012;
    goto switchD_00022c5c_caseD_23004;
  case 3:
    pcVar37 = fwrite;
    break;
  case 4:
    pcVar37 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar30 = *in_a1;
    goto LAB_00022c20;
  }
  in_a3 = (serializer *)in_a1[1];
  (*pcVar37)(local_20,1,4);
  uVar30 = *in_a1;
LAB_00022c20:
  *(serializer **)(param_1 + 0xc15c) = psVar34 + local_20[0] * 0x2000;
  if (uVar30 < 5) {
LAB_00022c44:
    switch((&switchD_00022c5c::switchdataD_00034848)[uVar30] + 0x50250 & 0xfffffffe) {
    case 0x22c64:
      pcVar37 = fwrite;
      break;
    case 0x22cc0:
      puVar20 = (uint *)in_a1[1];
      psVar34 = *(serializer **)(param_1 + 4);
      uVar30 = *(uint *)(param_1 + 8);
      uVar24 = *(uint *)(param_1 + 0xc);
      *puVar20 = (uint)psVar34;
      puVar20[1] = uVar30;
      puVar20[2] = uVar24;
      *(serializer *)(puVar20 + 3) = param_1[0x10];
      *(serializer *)((int)puVar20 + 0xd) = param_1[0x11];
      in_a1[1] = in_a1[1] + 0xe;
      iVar19 = *(int *)(*(int *)(*(int *)param_1 + 0xc) + 0x20);
      goto joined_r0x00022d20;
    case 0x22d54:
      pcVar37 = fread;
      break;
    case 0x22e3c:
switchD_00022c5c_caseD_22e3c:
      piVar25 = (int *)in_a1[1];
      iVar27 = *(int *)(*(int *)param_1 + 0xc);
      iVar19 = *piVar25 + 0xe;
      *piVar25 = iVar19;
      if (2 < *(int *)(iVar27 + 0x20)) goto LAB_00024a7c;
      goto switchD_00022cb8_caseD_0;
    case 0x23004:
switchD_00022c5c_caseD_23004:
      puVar20 = (uint *)in_a1[1];
      psVar34 = (serializer *)*puVar20;
      uVar30 = puVar20[1];
      uVar24 = puVar20[2];
      *(serializer **)(param_1 + 4) = psVar34;
      *(uint *)(param_1 + 8) = uVar30;
      *(uint *)(param_1 + 0xc) = uVar24;
      param_1[0x10] = *(serializer *)(puVar20 + 3);
      param_1[0x11] = *(serializer *)((int)puVar20 + 0xd);
      in_a1[1] = in_a1[1] + 0xe;
      goto LAB_00022c7c;
    }
    in_a3 = (serializer *)in_a1[1];
    psVar34 = (serializer *)0xe;
    uVar30 = 1;
    (*pcVar37)();
  }
  else {
LAB_00022da0:
    uVar30 = 0;
    psVar34 = param_1 + 0x10000;
    puts("ERROR: invalid serializer!");
  }
LAB_00022c7c:
  iVar19 = *(int *)(*(int *)(*(int *)param_1 + 0xc) + 0x20);
joined_r0x00022d20:
  if (2 < iVar19) {
    psVar35 = param_1 + 0x8012;
    switch(*in_a1) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25;
LAB_00024a7c:
      *piVar25 = iVar19 + 0x8000;
      iVar19 = iVar19 + 0xc000;
      *piVar25 = iVar19;
      goto LAB_00024a8c;
    case 1:
      puVar20 = (uint *)in_a1[1];
      in_a3 = param_1 + 0x8012;
      do {
        psVar34 = *(serializer **)psVar36;
        uVar30 = *(uint *)(psVar36 + 4);
        uVar28 = *(uint *)(psVar36 + 8);
        uVar24 = *(uint *)(psVar36 + 0xc);
        psVar36 = psVar36 + 0x10;
        *puVar20 = (uint)psVar34;
        puVar20[1] = uVar30;
        puVar20[2] = uVar28;
        puVar20[3] = uVar24;
        puVar20 = puVar20 + 4;
      } while (psVar36 != in_a3);
      uVar24 = in_a1[1];
      break;
    case 2:
      psVar23 = (serializer *)in_a1[1];
      in_a3 = psVar23 + 0x8000;
      do {
        psVar34 = *(serializer **)psVar23;
        uVar30 = *(uint *)(psVar23 + 4);
        uVar28 = *(uint *)(psVar23 + 8);
        uVar24 = *(uint *)(psVar23 + 0xc);
        psVar23 = psVar23 + 0x10;
        *(serializer **)psVar36 = psVar34;
        *(uint *)(psVar36 + 4) = uVar30;
        *(uint *)(psVar36 + 8) = uVar28;
        *(uint *)(psVar36 + 0xc) = uVar24;
        psVar36 = psVar36 + 0x10;
      } while (psVar23 != in_a3);
      uVar24 = in_a1[1];
      break;
    case 3:
      pcVar37 = fwrite;
      goto LAB_000247c0;
    case 4:
      pcVar37 = fread;
LAB_000247c0:
      in_a3 = (serializer *)in_a1[1];
      psVar34 = (serializer *)0x8000;
      uVar30 = 1;
      (*pcVar37)(psVar36);
      goto LAB_000247d8;
    default:
      puts("ERROR: invalid serializer!");
      goto LAB_000247d8;
    }
    in_a1[1] = uVar24 + 0x8000;
LAB_000247d8:
    switch(*in_a1) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 0x4000;
      *piVar25 = iVar19;
LAB_00024a8c:
      iVar19 = iVar19 + 0x80;
      *piVar25 = iVar19;
      goto LAB_00022e84;
    case 1:
      puVar20 = (uint *)in_a1[1];
      in_a3 = param_1 + 0xc012;
      do {
        psVar34 = *(serializer **)psVar35;
        uVar30 = *(uint *)(psVar35 + 4);
        uVar28 = *(uint *)(psVar35 + 8);
        uVar24 = *(uint *)(psVar35 + 0xc);
        psVar35 = psVar35 + 0x10;
        *puVar20 = (uint)psVar34;
        puVar20[1] = uVar30;
        puVar20[2] = uVar28;
        puVar20[3] = uVar24;
        puVar20 = puVar20 + 4;
      } while (psVar35 != in_a3);
      uVar24 = in_a1[1];
      goto LAB_00024868;
    case 2:
      psVar36 = (serializer *)in_a1[1];
      in_a3 = psVar36 + 0x4000;
      do {
        psVar34 = *(serializer **)psVar36;
        uVar30 = *(uint *)(psVar36 + 4);
        uVar28 = *(uint *)(psVar36 + 8);
        uVar24 = *(uint *)(psVar36 + 0xc);
        psVar36 = psVar36 + 0x10;
        *(serializer **)psVar35 = psVar34;
        *(uint *)(psVar35 + 4) = uVar30;
        *(uint *)(psVar35 + 8) = uVar28;
        *(uint *)(psVar35 + 0xc) = uVar24;
        psVar35 = psVar35 + 0x10;
      } while (psVar36 != in_a3);
      uVar24 = in_a1[1];
LAB_00024868:
      in_a1[1] = uVar24 + 0x4000;
      goto LAB_000230bc;
    case 3:
      pcVar37 = fwrite;
      psVar34 = (serializer *)0x4000;
      break;
    case 4:
      pcVar37 = fread;
      psVar34 = (serializer *)0x4000;
      break;
    default:
      goto switchD_00023098_default;
    }
    goto LAB_000230a8;
  }
  psVar35 = param_1 + 0x8012;
  switch(*in_a1) {
  case 0:
switchD_00022cb8_caseD_0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25;
    *piVar25 = iVar19 + 0x2000;
    iVar19 = iVar19 + 0x4000;
    *piVar25 = iVar19;
    goto LAB_00022e7c;
  case 1:
    puVar20 = (uint *)in_a1[1];
    in_a3 = param_1 + 0x2012;
    do {
      psVar34 = *(serializer **)psVar36;
      uVar30 = *(uint *)(psVar36 + 4);
      uVar28 = *(uint *)(psVar36 + 8);
      uVar24 = *(uint *)(psVar36 + 0xc);
      psVar36 = psVar36 + 0x10;
      *puVar20 = (uint)psVar34;
      puVar20[1] = uVar30;
      puVar20[2] = uVar28;
      puVar20[3] = uVar24;
      puVar20 = puVar20 + 4;
    } while (psVar36 != in_a3);
    uVar24 = in_a1[1];
    break;
  case 2:
    psVar23 = (serializer *)in_a1[1];
    in_a3 = psVar23 + 0x2000;
    do {
      psVar34 = *(serializer **)psVar23;
      uVar30 = *(uint *)(psVar23 + 4);
      uVar28 = *(uint *)(psVar23 + 8);
      uVar24 = *(uint *)(psVar23 + 0xc);
      psVar23 = psVar23 + 0x10;
      *(serializer **)psVar36 = psVar34;
      *(uint *)(psVar36 + 4) = uVar30;
      *(uint *)(psVar36 + 8) = uVar28;
      *(uint *)(psVar36 + 0xc) = uVar24;
      psVar36 = psVar36 + 0x10;
    } while (psVar23 != in_a3);
    uVar24 = in_a1[1];
    break;
  case 3:
    pcVar37 = fwrite;
    goto LAB_0002305c;
  case 4:
    pcVar37 = fread;
LAB_0002305c:
    in_a3 = (serializer *)in_a1[1];
    psVar34 = (serializer *)0x2000;
    uVar30 = 1;
    (*pcVar37)(psVar36);
    goto LAB_00023074;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00023074;
  }
  in_a1[1] = uVar24 + 0x2000;
LAB_00023074:
  switch(*in_a1) {
  case 0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25 + 0x2000;
    *piVar25 = iVar19;
LAB_00022e7c:
    iVar19 = iVar19 + 0x80;
    *piVar25 = iVar19;
    goto LAB_00022e84;
  case 1:
    puVar20 = (uint *)in_a1[1];
    in_a3 = param_1 + 0xa012;
    do {
      psVar34 = *(serializer **)psVar35;
      uVar30 = *(uint *)(psVar35 + 4);
      uVar28 = *(uint *)(psVar35 + 8);
      uVar24 = *(uint *)(psVar35 + 0xc);
      psVar35 = psVar35 + 0x10;
      *puVar20 = (uint)psVar34;
      puVar20[1] = uVar30;
      puVar20[2] = uVar28;
      puVar20[3] = uVar24;
      puVar20 = puVar20 + 4;
    } while (psVar35 != in_a3);
    uVar24 = in_a1[1];
    break;
  case 2:
    psVar36 = (serializer *)in_a1[1];
    in_a3 = psVar36 + 0x2000;
    do {
      psVar34 = *(serializer **)psVar36;
      uVar30 = *(uint *)(psVar36 + 4);
      uVar28 = *(uint *)(psVar36 + 8);
      uVar24 = *(uint *)(psVar36 + 0xc);
      psVar36 = psVar36 + 0x10;
      *(serializer **)psVar35 = psVar34;
      *(uint *)(psVar35 + 4) = uVar30;
      *(uint *)(psVar35 + 8) = uVar28;
      *(uint *)(psVar35 + 0xc) = uVar24;
      psVar35 = psVar35 + 0x10;
    } while (psVar36 != in_a3);
    uVar24 = in_a1[1];
    break;
  case 3:
    pcVar37 = fwrite;
    goto LAB_000230a4;
  case 4:
    pcVar37 = fread;
LAB_000230a4:
    psVar34 = (serializer *)0x2000;
LAB_000230a8:
    in_a3 = (serializer *)in_a1[1];
    uVar30 = 1;
    (*pcVar37)(param_1 + 0x8012);
    goto LAB_000230bc;
  default:
switchD_00023098_default:
    puts("ERROR: invalid serializer!");
    goto LAB_000230bc;
  }
  in_a1[1] = uVar24 + 0x2000;
LAB_000230bc:
  psVar36 = param_1 + 0xc012;
  switch(*in_a1) {
  case 0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25 + 0x80;
    *piVar25 = iVar19;
LAB_00022e84:
    iVar19 = iVar19 + 0xa0;
    *piVar25 = iVar19;
    goto LAB_00022e8c;
  case 1:
    puVar20 = (uint *)in_a1[1];
    in_t0 = param_1 + 0xc092;
    do {
      in_a3 = *(serializer **)psVar36;
      psVar34 = *(serializer **)(psVar36 + 4);
      uVar30 = *(uint *)(psVar36 + 8);
      uVar24 = *(uint *)(psVar36 + 0xc);
      psVar36 = psVar36 + 0x10;
      *puVar20 = (uint)in_a3;
      puVar20[1] = (uint)psVar34;
      puVar20[2] = uVar30;
      puVar20[3] = uVar24;
      puVar20 = puVar20 + 4;
    } while (psVar36 != in_t0);
    uVar24 = in_a1[1];
    break;
  case 2:
    psVar35 = (serializer *)in_a1[1];
    in_t0 = psVar35 + 0x80;
    do {
      in_a3 = *(serializer **)psVar35;
      psVar34 = *(serializer **)(psVar35 + 4);
      uVar30 = *(uint *)(psVar35 + 8);
      uVar24 = *(uint *)(psVar35 + 0xc);
      psVar35 = psVar35 + 0x10;
      *(serializer **)psVar36 = in_a3;
      *(serializer **)(psVar36 + 4) = psVar34;
      *(uint *)(psVar36 + 8) = uVar30;
      *(uint *)(psVar36 + 0xc) = uVar24;
      psVar36 = psVar36 + 0x10;
    } while (psVar35 != in_t0);
    uVar24 = in_a1[1];
    break;
  case 3:
    pcVar37 = fwrite;
    goto LAB_000230f4;
  case 4:
    pcVar37 = fread;
LAB_000230f4:
    in_a3 = (serializer *)in_a1[1];
    psVar34 = (serializer *)0x80;
    uVar30 = 1;
    (*pcVar37)();
    goto LAB_00023108;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00023108;
  }
  in_a1[1] = uVar24 + 0x80;
LAB_00023108:
  psVar36 = param_1 + 0xc092;
  switch(*in_a1) {
  case 0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25 + 0xa0;
    *piVar25 = iVar19;
LAB_00022e8c:
    iVar19 = iVar19 + 0x18;
    *piVar25 = iVar19;
    goto LAB_00022e94;
  case 1:
    puVar20 = (uint *)in_a1[1];
    in_t0 = param_1 + 0xc132;
    do {
      in_a3 = *(serializer **)psVar36;
      psVar34 = *(serializer **)(psVar36 + 4);
      uVar30 = *(uint *)(psVar36 + 8);
      uVar24 = *(uint *)(psVar36 + 0xc);
      psVar36 = psVar36 + 0x10;
      *puVar20 = (uint)in_a3;
      puVar20[1] = (uint)psVar34;
      puVar20[2] = uVar30;
      puVar20[3] = uVar24;
      puVar20 = puVar20 + 4;
    } while (psVar36 != in_t0);
    uVar24 = in_a1[1];
    break;
  case 2:
    psVar35 = (serializer *)in_a1[1];
    in_t0 = psVar35 + 0xa0;
    do {
      in_a3 = *(serializer **)psVar35;
      psVar34 = *(serializer **)(psVar35 + 4);
      uVar30 = *(uint *)(psVar35 + 8);
      uVar24 = *(uint *)(psVar35 + 0xc);
      psVar35 = psVar35 + 0x10;
      *(serializer **)psVar36 = in_a3;
      *(serializer **)(psVar36 + 4) = psVar34;
      *(uint *)(psVar36 + 8) = uVar30;
      *(uint *)(psVar36 + 0xc) = uVar24;
      psVar36 = psVar36 + 0x10;
    } while (psVar35 != in_t0);
    uVar24 = in_a1[1];
    break;
  case 3:
    pcVar37 = fwrite;
    goto LAB_00023140;
  case 4:
    pcVar37 = fread;
LAB_00023140:
    in_a3 = (serializer *)in_a1[1];
    psVar34 = (serializer *)0xa0;
    uVar30 = 1;
    (*pcVar37)();
    goto LAB_00023154;
  default:
    puts("ERROR: invalid serializer!");
    uVar24 = *in_a1;
    goto joined_r0x000249d0;
  }
  in_a1[1] = uVar24 + 0xa0;
LAB_00023154:
  uVar24 = *in_a1;
joined_r0x000249d0:
  if (uVar24 < 5) {
    psVar36 = param_1 + 0xc132;
    switch(uVar24) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 0x18;
      *piVar25 = iVar19;
LAB_00022e94:
      iVar19 = iVar19 + 0x10;
      *piVar25 = iVar19;
      goto LAB_00022e9c;
    case 1:
      uVar28 = (uint)(param_1 + 0xc135) & 3;
      uVar1 = (uint)(param_1 + 0xc139) & 3;
      iVar19 = *(int *)(param_1 + 0xc139 + -uVar1);
      uVar2 = (uint)(param_1 + 0xc13d) & 3;
      iVar27 = *(int *)(param_1 + 0xc13d + -uVar2);
      puVar20 = (uint *)in_a1[1];
      uVar6 = (uint)(param_1 + 0xc141) & 3;
      iVar3 = *(int *)(param_1 + 0xc141 + -uVar6);
      uVar7 = (uint)(param_1 + 0xc145) & 3;
      iVar4 = *(int *)(param_1 + 0xc145 + -uVar7);
      uVar8 = (uint)(param_1 + 0xc149) & 3;
      iVar5 = *(int *)(param_1 + 0xc149 + -uVar8);
      uVar9 = (uint)psVar36 & 3;
      uVar11 = (uint)(param_1 + 0xc13a) & 3;
      uVar14 = *(uint *)(param_1 + 0xc13a + -uVar11);
      uVar12 = (uint)(param_1 + 0xc136) & 3;
      uVar15 = *(uint *)(param_1 + 0xc136 + -uVar12);
      uVar13 = (uint)(param_1 + 0xc13e) & 3;
      uVar16 = *(uint *)(param_1 + 0xc13e + -uVar13);
      uVar22 = (uint)(param_1 + 0xc142) & 3;
      uVar17 = *(uint *)(param_1 + 0xc142 + -uVar22);
      uVar10 = (uint)(param_1 + 0xc146) & 3;
      uVar18 = *(uint *)(param_1 + 0xc146 + -uVar10);
      *puVar20 = (*(int *)(param_1 + 0xc135 + -uVar28) << (3 - uVar28) * 8 |
                 in_t1 & 0xffffffffU >> (uVar28 + 1) * 8) & -1 << (4 - uVar9) * 8 |
                 *(uint *)(psVar36 + -uVar9) >> uVar9 * 8;
      puVar20[1] = (iVar19 << (3 - uVar1) * 8 | (uint)in_t0 & 0xffffffffU >> (uVar1 + 1) * 8) &
                   -1 << (4 - uVar12) * 8 | uVar15 >> uVar12 * 8;
      puVar20[2] = (iVar27 << (3 - uVar2) * 8 | (uint)in_a3 & 0xffffffffU >> (uVar2 + 1) * 8) &
                   -1 << (4 - uVar11) * 8 | uVar14 >> uVar11 * 8;
      puVar20[3] = (iVar3 << (3 - uVar6) * 8 | (uint)psVar34 & 0xffffffffU >> (uVar6 + 1) * 8) &
                   -1 << (4 - uVar13) * 8 | uVar16 >> uVar13 * 8;
      puVar20[4] = (iVar4 << (3 - uVar7) * 8 | uVar30 & 0xffffffffU >> (uVar7 + 1) * 8) &
                   -1 << (4 - uVar22) * 8 | uVar17 >> uVar22 * 8;
      puVar20[5] = (iVar5 << (3 - uVar8) * 8 | uVar24 * 4 & 0xffffffffU >> (uVar8 + 1) * 8) &
                   -1 << (4 - uVar10) * 8 | uVar18 >> uVar10 * 8;
      in_a1[1] = in_a1[1] + 0x18;
      goto LAB_000231a0;
    case 2:
      uVar22 = in_a1[1];
      uVar24 = uVar22 + 3 & 3;
      uVar28 = uVar22 + 7 & 3;
      iVar19 = *(int *)((uVar22 + 7) - uVar28);
      uVar1 = uVar22 + 0xf & 3;
      iVar27 = *(int *)((uVar22 + 0xf) - uVar1);
      uVar2 = uVar22 + 0x13 & 3;
      iVar3 = *(int *)((uVar22 + 0x13) - uVar2);
      uVar6 = uVar22 & 3;
      uVar29 = *(undefined4 *)(uVar22 + 8);
      uVar7 = uVar22 + 4 & 3;
      uVar11 = *(uint *)((uVar22 + 4) - uVar7);
      uVar26 = *(undefined4 *)(uVar22 + 0x14);
      uVar8 = uVar22 + 0xc & 3;
      uVar12 = *(uint *)((uVar22 + 0xc) - uVar8);
      uVar9 = uVar22 + 0x10 & 3;
      uVar13 = *(uint *)((uVar22 + 0x10) - uVar9);
      *(uint *)psVar36 =
           (*(int *)((uVar22 + 3) - uVar24) << (3 - uVar24) * 8 |
           in_t1 & 0xffffffffU >> (uVar24 + 1) * 8) & -1 << (4 - uVar6) * 8 |
           *(uint *)(uVar22 - uVar6) >> uVar6 * 8;
      *(uint *)(param_1 + 0xc136) =
           (iVar19 << (3 - uVar28) * 8 | (uint)in_t0 & 0xffffffffU >> (uVar28 + 1) * 8) &
           -1 << (4 - uVar7) * 8 | uVar11 >> uVar7 * 8;
      *(undefined4 *)(param_1 + 0xc13a) = uVar29;
      *(uint *)(param_1 + 0xc13e) =
           (iVar27 << (3 - uVar1) * 8 | (uint)psVar34 & 0xffffffffU >> (uVar1 + 1) * 8) &
           -1 << (4 - uVar8) * 8 | uVar12 >> uVar8 * 8;
      *(uint *)(param_1 + 0xc142) =
           (iVar3 << (3 - uVar2) * 8 | uVar30 & 0xffffffffU >> (uVar2 + 1) * 8) &
           -1 << (4 - uVar9) * 8 | uVar13 >> uVar9 * 8;
      *(undefined4 *)(param_1 + 0xc146) = uVar26;
      in_a1[1] = in_a1[1] + 0x18;
      uVar30 = *in_a1;
      goto joined_r0x00024400;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,0x18,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000231a0:
  uVar30 = *in_a1;
joined_r0x00024400:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc14a;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 0x10;
      *piVar25 = iVar19;
LAB_00022e9c:
      iVar19 = iVar19 + 0x400;
      *piVar25 = iVar19;
      goto LAB_00022ea4;
    case 1:
      puVar21 = (undefined4 *)in_a1[1];
      uVar31 = *(undefined4 *)(param_1 + 0xc14e);
      uVar29 = *(undefined4 *)(param_1 + 0xc152);
      uVar26 = *(undefined4 *)(param_1 + 0xc156);
      *puVar21 = *(undefined4 *)psVar36;
      puVar21[1] = uVar31;
      puVar21[2] = uVar29;
      puVar21[3] = uVar26;
      in_a1[1] = in_a1[1] + 0x10;
      goto LAB_000231ec;
    case 2:
      puVar21 = (undefined4 *)in_a1[1];
      uVar31 = puVar21[1];
      uVar29 = puVar21[2];
      uVar26 = puVar21[3];
      *(undefined4 *)psVar36 = *puVar21;
      *(undefined4 *)(param_1 + 0xc14e) = uVar31;
      *(undefined4 *)(param_1 + 0xc152) = uVar29;
      *(undefined4 *)(param_1 + 0xc156) = uVar26;
      in_a1[1] = in_a1[1] + 0x10;
      uVar30 = *in_a1;
      goto joined_r0x000242e0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,0x10,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000231ec:
  uVar30 = *in_a1;
joined_r0x000242e0:
  if (4 < uVar30) {
    puts("ERROR: invalid serializer!");
    uVar30 = *in_a1;
    goto joined_r0x00024308;
  }
  psVar36 = param_1 + 0xc364;
  switch(uVar30) {
  case 0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25 + 0x400;
    *piVar25 = iVar19;
LAB_00022ea4:
    iVar19 = iVar19 + 4;
    *piVar25 = iVar19;
    goto LAB_00022eac;
  case 1:
    puVar21 = (undefined4 *)in_a1[1];
    do {
      uVar32 = *(undefined4 *)psVar36;
      uVar31 = *(undefined4 *)(psVar36 + 4);
      uVar29 = *(undefined4 *)(psVar36 + 8);
      uVar26 = *(undefined4 *)(psVar36 + 0xc);
      psVar36 = psVar36 + 0x10;
      *puVar21 = uVar32;
      puVar21[1] = uVar31;
      puVar21[2] = uVar29;
      puVar21[3] = uVar26;
      puVar21 = puVar21 + 4;
    } while (psVar36 != param_1 + 0xc764);
    uVar30 = in_a1[1];
    break;
  case 2:
    puVar21 = (undefined4 *)in_a1[1];
    puVar33 = puVar21 + 0x100;
    do {
      uVar32 = *puVar21;
      uVar31 = puVar21[1];
      uVar29 = puVar21[2];
      uVar26 = puVar21[3];
      puVar21 = puVar21 + 4;
      *(undefined4 *)psVar36 = uVar32;
      *(undefined4 *)(psVar36 + 4) = uVar31;
      *(undefined4 *)(psVar36 + 8) = uVar29;
      *(undefined4 *)(psVar36 + 0xc) = uVar26;
      psVar36 = psVar36 + 0x10;
    } while (puVar21 != puVar33);
    uVar30 = in_a1[1];
    break;
  case 3:
    pcVar37 = fwrite;
    goto LAB_00023224;
  case 4:
    pcVar37 = fread;
LAB_00023224:
    (*pcVar37)(psVar36,1,0x400,in_a1[1]);
    goto LAB_00023238;
  }
  in_a1[1] = uVar30 + 0x400;
LAB_00023238:
  uVar30 = *in_a1;
joined_r0x00024308:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc764;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022eac:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022eb4;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_00023284;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00024170;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023284:
  uVar30 = *in_a1;
joined_r0x00024170:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc768;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022eb4:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022ebc;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_000232d0;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x000240f8;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000232d0:
  uVar30 = *in_a1;
joined_r0x000240f8:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc76c;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022ebc:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022ec4;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_0002331c;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00024080;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002331c:
  uVar30 = *in_a1;
joined_r0x00024080:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc770;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022ec4:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022ecc;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_00023368;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00024008;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023368:
  uVar30 = *in_a1;
joined_r0x00024008:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc778;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022ecc:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022ed4;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_000233b4;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023f90;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000233b4:
  uVar30 = *in_a1;
joined_r0x00023f90:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc774;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022ed4:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022edc;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_00023400;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023f18;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023400:
  uVar30 = *in_a1;
joined_r0x00023f18:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022edc:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022ee4;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc77c];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_0002344c;
    case 2:
      param_1[0xc77c] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023ea0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc77c,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002344c:
  uVar30 = *in_a1;
joined_r0x00023ea0:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022ee4:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022eec;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc77d];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_00023498;
    case 2:
      param_1[0xc77d] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023e28;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc77d,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023498:
  uVar30 = *in_a1;
joined_r0x00023e28:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022eec:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022ef4;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc77e];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_000234e4;
    case 2:
      param_1[0xc77e] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023db0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc77e,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000234e4:
  uVar30 = *in_a1;
joined_r0x00023db0:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022ef4:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022efc;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc77f];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_00023530;
    case 2:
      param_1[0xc77f] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023d38;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc77f,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023530:
  uVar30 = *in_a1;
joined_r0x00023d38:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc784;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022efc:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022f04;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_0002357c;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023cc0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002357c:
  uVar30 = *in_a1;
joined_r0x00023cc0:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc788;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022f04:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022f0c;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_000235c8;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023c48;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000235c8:
  uVar30 = *in_a1;
joined_r0x00023c48:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc78c;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022f0c:
      iVar19 = iVar19 + 4;
      *piVar25 = iVar19;
      goto LAB_00022f14;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_00023614;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023bd0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023614:
  uVar30 = *in_a1;
joined_r0x00023bd0:
  if (uVar30 < 5) {
    psVar36 = param_1 + 0xc790;
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 4;
      *piVar25 = iVar19;
LAB_00022f14:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f1c;
    case 1:
      *(undefined4 *)in_a1[1] = *(undefined4 *)psVar36;
      in_a1[1] = in_a1[1] + 4;
      goto LAB_00023660;
    case 2:
      *(undefined4 *)psVar36 = *(undefined4 *)in_a1[1];
      in_a1[1] = in_a1[1] + 4;
      uVar30 = *in_a1;
      goto joined_r0x00023b58;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(psVar36,1,4,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023660:
  uVar30 = *in_a1;
joined_r0x00023b58:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022f1c:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f24;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc794];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_000236ac;
    case 2:
      param_1[0xc794] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023ae0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc794,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000236ac:
  uVar30 = *in_a1;
joined_r0x00023ae0:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022f24:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f2c;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc7a8];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_000236f8;
    case 2:
      param_1[0xc7a8] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023a68;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc7a8,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000236f8:
  uVar30 = *in_a1;
joined_r0x00023a68:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022f2c:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f34;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc7a9];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_00023744;
    case 2:
      param_1[0xc7a9] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x000239f0;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc7a9,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023744:
  uVar30 = *in_a1;
joined_r0x000239f0:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
LAB_00022f34:
      iVar19 = iVar19 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f3c;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc7aa];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_00023790;
    case 2:
      param_1[0xc7aa] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x00023978;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc7aa,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00023790:
  uVar30 = *in_a1;
joined_r0x00023978:
  if (uVar30 < 5) {
    switch(uVar30) {
    case 0:
      piVar25 = (int *)in_a1[1];
      iVar19 = *piVar25 + 1;
      *piVar25 = iVar19;
      goto LAB_00022f3c;
    case 1:
      *(serializer *)in_a1[1] = param_1[0xc7ab];
      in_a1[1] = in_a1[1] + 1;
      goto LAB_000237dc;
    case 2:
      param_1[0xc7ab] = *(serializer *)in_a1[1];
      in_a1[1] = in_a1[1] + 1;
      uVar30 = *in_a1;
      goto joined_r0x000238ec;
    case 3:
      pcVar37 = fwrite;
      break;
    case 4:
      pcVar37 = fread;
    }
    (*pcVar37)(param_1 + 0xc7ab,1,1,in_a1[1]);
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000237dc:
  uVar30 = *in_a1;
joined_r0x000238ec:
  if (4 < uVar30) {
    puts("ERROR: invalid serializer!");
    return;
  }
  switch(uVar30) {
  case 0:
    piVar25 = (int *)in_a1[1];
    iVar19 = *piVar25;
LAB_00022f3c:
    *piVar25 = iVar19 + 1;
    return;
  case 1:
    *(serializer *)in_a1[1] = param_1[0xc7ac];
    in_a1[1] = in_a1[1] + 1;
    return;
  case 2:
    param_1[0xc7ac] = *(serializer *)in_a1[1];
    in_a1[1] = in_a1[1] + 1;
    return;
  case 3:
    pcVar37 = fwrite;
    break;
  case 4:
    pcVar37 = fread;
  }
  (*pcVar37)(param_1 + 0xc7ac,1,1,in_a1[1]);
  return;
}



/* ===== FUNCTION read @ 00024cd0 ===== */

/* cpu::read(unsigned short) */

uint __thiscall cpu::read(cpu *this,ushort param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  undefined2 in_register_00000016;
  int *piVar6;
  int *piVar7;
  
  uVar3 = CONCAT22(in_register_00000016,param_1);
  piVar5 = *(int **)(*(int *)this + 0x18);
  if (piVar5[uVar3 + 3] != 0) {
    piVar7 = (int *)*piVar5;
    if (piVar7 != piVar5) {
      do {
        if ((char)piVar7[2] != '\0') {
          uVar1 = (uint)*(byte *)((int)piVar7 + 9);
          piVar6 = piVar7 + 2;
LAB_0001604c:
          do {
LAB_00016068:
            do {
              if (uVar1 == 0x21) {
                iVar2 = read_direct(*(cpu **)piVar5[0x10003],*(ushort *)((int)piVar6 + 2));
                if ((int)(uint)*(byte *)(piVar6 + 1) <= iVar2) {
                  piVar7 = (int *)*piVar7;
                  goto LAB_000160a0;
                }
LAB_00016190:
                piVar6 = (int *)piVar6[0x41];
                if (piVar6 == (int *)0x0) goto LAB_0001609c;
                uVar1 = (uint)*(byte *)((int)piVar6 + 1);
                goto LAB_0001604c;
              }
              if (uVar1 < 0x22) {
                if (uVar1 == 0x10) {
                  uVar1 = (uint)*(ushort *)(piVar6[0x41] + 2);
                  iVar2 = uVar3 - uVar1;
                  if (((int)uVar1 <= (int)uVar3) &&
                     (iVar4 = *(ushort *)((int)piVar6 + 2) + 1,
                     iVar2 < (int)((uint)*(byte *)(piVar6 + 1) * iVar4))) {
                    if (iVar4 == 0) {
                      trap(7);
                    }
                    if (iVar2 % iVar4 == 0) {
                      return (uint)*(byte *)(piVar6[0x41] + 4);
                    }
                  }
                  goto LAB_0001609c;
                }
                if (uVar1 == 0x20) {
                  uVar1 = read_direct(*(cpu **)piVar5[0x10003],*(ushort *)((int)piVar6 + 2));
                  if (*(byte *)(piVar6 + 1) == uVar1) goto LAB_00016190;
                  piVar7 = (int *)*piVar7;
                  goto LAB_000160a0;
                }
                if (uVar1 == 1) {
                  if (*(ushort *)((int)piVar6 + 2) != uVar3) goto LAB_0001609c;
                  goto LAB_00016140;
                }
                goto LAB_00016068;
              }
              if (uVar1 == 0x22) {
                iVar2 = read_direct(*(cpu **)piVar5[0x10003],*(ushort *)((int)piVar6 + 2));
                if ((int)(uint)*(byte *)(piVar6 + 1) < iVar2) goto LAB_00016190;
                goto LAB_0001609c;
              }
            } while (7 < uVar1 - 0x90);
          } while (*(ushort *)((int)piVar6 + 2) != uVar3);
          if (0xfff < (uVar3 + 0x3000 & 0xffff)) {
LAB_00016140:
            return (uint)*(byte *)(piVar6 + 1);
          }
          iVar2 = *(int *)(*(int *)piVar5[0x10003] + 0xc160) - (*(int *)piVar5[0x10003] + 0x12);
          if (iVar2 < 0) {
            iVar2 = iVar2 + 0xfff;
          }
          if (iVar2 >> 0xc == uVar1 - 0x90) goto LAB_00016140;
        }
LAB_0001609c:
        piVar7 = (int *)*piVar7;
LAB_000160a0:
      } while (piVar7 != piVar5);
    }
    this = *(cpu **)piVar5[0x10003];
  }
                    /* WARNING: Could not recover jumptable at 0x00016b20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*(code *)(apu_snd::render(short*,int)::filter +
                    *(int *)(&DAT_00033c00 + ((int)uVar3 >> 0xd) * 4) + 0x7ccc))(this);
  return uVar3;
}



/* ===== FUNCTION io_write @ 00024d0c ===== */

/* cpu::io_write(unsigned short, unsigned char) */

void __thiscall cpu::io_write(cpu *this,ushort param_1,uchar param_2)

{
  ushort uVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  ushort *puVar5;
  undefined2 in_register_00000016;
  int iVar6;
  cpu *pcVar7;
  undefined3 in_register_00000019;
  int iVar8;
  uint uVar9;
  
  iVar8 = CONCAT31(in_register_00000019,param_2);
  iVar6 = CONCAT22(in_register_00000016,param_1);
  uVar1 = (ushort)iVar8;
  switch(iVar6 + 0x100U & 0xffff) {
  case 0:
    *(uchar *)(*(int *)this + 0x20) = param_2;
    break;
  case 1:
    *(uchar *)(*(int *)this + 0x21) = param_2;
    break;
  case 2:
    iVar6 = *(int *)this;
    if (*(int *)(*(int *)(iVar6 + 0xc) + 0x20) == 1) {
      *(byte *)(iVar6 + 0x22) = param_2 & 0x81;
      if (((char)param_2 < '\0') && ((param_2 & 1) != 0)) {
        *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x200;
      }
    }
    else {
      *(byte *)(iVar6 + 0x22) = param_2 & 0x83;
      if (((char)param_2 < '\0') && ((param_2 & 1) != 0)) {
        if ((param_2 & 2) == 0) {
          *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x1000;
        }
        else {
          *(int *)(this + 0xc774) = *(int *)(this + 0xc768) + 0x80;
        }
      }
    }
    break;
  default:
    if ((iVar6 + 0xf0U & 0xffff) < 0x30) {
      apu::write(*(apu **)(*(int *)this + 8),param_1,param_2,*(int *)(this + 0xc768));
      return;
    }
    if ((iVar6 + 0x8fU & 0xffff) < 0xf) {
      this[iVar6 + -0x3e27] = (cpu)param_2;
    }
    break;
  case 4:
    *(undefined1 *)(*(int *)this + 0x23) = 0;
    break;
  case 5:
    *(uchar *)(*(int *)this + 0x24) = param_2;
    break;
  case 6:
    *(uchar *)(*(int *)this + 0x25) = param_2;
    break;
  case 7:
    if ((param_2 & 4) == 0) {
      iVar6 = *(int *)this;
    }
    else {
      iVar6 = *(int *)this;
      if ((*(byte *)(iVar6 + 0x26) & 4) == 0) {
        *(undefined4 *)(this + 0xc770) = 0;
        *(uchar *)(iVar6 + 0x26) = param_2;
        return;
      }
    }
    *(uchar *)(iVar6 + 0x26) = param_2;
    break;
  case 0xf:
    *(uchar *)(*(int *)this + 0x27) = param_2;
    break;
  case 0x40:
    if ((char)param_2 < '\0') {
      iVar6 = *(int *)this;
      if (-1 < *(char *)(iVar6 + 0x28)) {
        *(undefined1 *)(iVar6 + 0x2c) = 0;
        *(undefined4 *)(*(int *)(iVar6 + 4) + 0x39c) = 9;
        *(uchar *)(iVar6 + 0x28) = param_2;
        return;
      }
    }
    else {
      iVar6 = *(int *)this;
    }
    *(uchar *)(iVar6 + 0x28) = param_2;
    break;
  case 0x41:
    iVar6 = *(int *)this;
    if (*(int *)(*(int *)(iVar6 + 0xc) + 0x20) == 1) {
      bVar2 = *(byte *)(iVar6 + 0x29);
      if ((bVar2 & 2) == 0) {
        *(byte *)(iVar6 + 0x27) = *(byte *)(iVar6 + 0x27) | 2;
      }
    }
    else {
      bVar2 = *(byte *)(iVar6 + 0x29);
    }
    *(byte *)(iVar6 + 0x29) = bVar2 & 7 | param_2 & 0x78;
    break;
  case 0x42:
    *(uchar *)(*(int *)this + 0x2a) = param_2;
    break;
  case 0x43:
    *(uchar *)(*(int *)this + 0x2b) = param_2;
    break;
  case 0x44:
    *(undefined4 *)(*(int *)(*(int *)this + 4) + 0x39c) = 9;
    break;
  case 0x45:
    *(uchar *)(*(int *)this + 0x2d) = param_2;
    break;
  case 0x46:
                    /* WARNING: Could not recover jumptable at 0x00024f80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_000357b0 + (iVar8 >> 5) * 4) + 0x7ccc))();
    return;
  case 0x47:
    *(uchar *)(*(int *)this + 0x2f) = param_2;
    break;
  case 0x48:
    *(uchar *)(*(int *)this + 0x30) = param_2;
    break;
  case 0x49:
    *(uchar *)(*(int *)this + 0x31) = param_2;
    break;
  case 0x4a:
    *(uchar *)(*(int *)this + 0x32) = param_2;
    break;
  case 0x4b:
    *(uchar *)(*(int *)this + 0x33) = param_2;
    break;
  case 0x4d:
    *(byte *)(*(int *)this + 0x35) = param_2 & 1;
    this[0xc77e] = (cpu)(param_2 & 1);
    break;
  case 0x4f:
    if (this[0xc77f] == (cpu)0x0) {
      *(cpu **)(this + 0xc15c) = this + (param_2 & 1) * 0x2000 + 0x8012;
      *(uchar *)(*(int *)this + 0x36) = param_2;
    }
    break;
  case 0x51:
    *(uint *)(this + 0xc784) = *(uint *)(this + 0xc784) & 0xf0 | iVar8 << 8;
    break;
  case 0x52:
    *(uint *)(this + 0xc784) = *(uint *)(this + 0xc784) & 0xff00 | param_2 & 0xf0;
    break;
  case 0x53:
    *(uint *)(this + 0xc788) = *(uint *)(this + 0xc788) & 0xf0 | iVar8 << 8;
    break;
  case 0x54:
    *(uint *)(this + 0xc788) = *(uint *)(this + 0xc788) & 0xff00 | param_2 & 0xf0;
    break;
  case 0x55:
    uVar9 = *(uint *)(this + 0xc784);
    if ((uVar9 - 0x8000 < 0x2000) || (0xdfff < (int)uVar9)) {
      *(undefined1 *)(*(int *)this + 0x3b) = 0;
      return;
    }
    if ((char)param_2 < '\0') {
      if (this[0xc77f] == (cpu)0x0) {
        this[0xc77f] = (cpu)0x1;
        *(uint *)(this + 0xc78c) = (param_2 & 0x7f) + 1;
        this[0xc794] = (cpu)0x1;
        *(undefined1 *)(*(int *)this + 0x3b) = 0;
        return;
      }
LAB_00025760:
      this[0xc77f] = (cpu)0x0;
      *(undefined4 *)(this + 0xc78c) = 0;
      *(undefined1 *)(*(int *)this + 0x3b) = 0xff;
      return;
    }
    if (this[0xc77f] != (cpu)0x0) goto LAB_00025760;
    iVar6 = *(int *)this;
    uVar4 = *(uint *)(this + 0xc788);
    *(undefined4 *)(this + 0xc78c) = 0;
    *(undefined1 *)(iVar6 + 0x3b) = 0xff;
    switch((int)uVar9 >> 0xd) {
    case 0:
    case 1:
      iVar6 = *(int *)(*(int *)(iVar6 + 0xc) + 0x2c);
      goto LAB_000255e0;
    case 2:
    case 3:
      iVar6 = **(int **)(iVar6 + 0x10);
LAB_000255e0:
      memcpy((void *)(*(int *)(this + 0xc15c) + (uVar4 & 0x1ff0)),(void *)(iVar6 + uVar9),
             (iVar8 + 1) * 0x10);
      break;
    default:
      break;
    case 5:
      pcVar7 = *(cpu **)(*(int *)(iVar6 + 0x10) + 4);
      uVar9 = uVar9 & 0x1fff;
      iVar6 = *(int *)(this + 0xc15c);
LAB_00025530:
      memcpy((void *)(iVar6 + (uVar4 & 0x1ff0)),pcVar7 + uVar9,(iVar8 + 1) * 0x10);
      break;
    case 6:
      if ((uVar9 & 0x1000) == 0) {
        pcVar7 = this + 0x12;
        uVar9 = uVar9 & 0xfff;
        iVar6 = *(int *)(this + 0xc15c);
        goto LAB_00025530;
      }
      memcpy((void *)(*(int *)(this + 0xc15c) + (uVar4 & 0x1ff0)),
             (void *)(*(int *)(this + 0xc160) + (uVar9 & 0xfff)),(iVar8 + 1) * 0x10);
    }
    iVar6 = (iVar8 + 1) * 0x10;
    *(int *)(this + 0xc784) = *(int *)(this + 0xc784) + iVar6;
    *(int *)(this + 0xc788) = *(int *)(this + 0xc788) + iVar6;
    *(uint *)(this + 0xc790) = ((this[0xc77d] != (cpu)0x0) + 1) * (iVar8 + 1) * 0x20 + 0x390;
    break;
  case 0x56:
    iVar6 = *(int *)(this + 0xc764);
    uVar1 = *(ushort *)(this + 0xc76c);
    *(int *)(this + 0xc764) = iVar6 + 1;
    *(undefined4 *)(this + iVar6 * 4 + 0xc368) = 0;
    *(uint *)(this + iVar6 * 4 + 0xc364) = (uint)uVar1 | iVar8 << 0x10;
    *(uchar *)(*(int *)this + 0x3c) = param_2;
    break;
  case 0x68:
    *(uchar *)(*(int *)this + 0x3d) = param_2;
    break;
  case 0x69:
    iVar6 = *(int *)this;
    bVar2 = *(byte *)(iVar6 + 0x3d);
    iVar8 = *(int *)(iVar6 + 4);
    uVar9 = (int)(uint)bVar2 >> 3;
    uVar4 = bVar2 & 6;
    if ((bVar2 & 1) == 0) {
      puVar5 = (ushort *)(iVar8 + ((uVar9 & 7) + 3) * 8 + uVar4);
      *puVar5 = *puVar5 & 0xff00 | uVar1;
    }
    else {
      puVar5 = (ushort *)(iVar8 + ((uVar9 & 7) + 3) * 8 + uVar4);
      *puVar5 = (ushort)(byte)*puVar5 | uVar1 << 8;
    }
    uVar3 = (**(code **)(**(int **)(iVar6 + 0x14) + 0x10))(*(int **)(iVar6 + 0x14));
    iVar6 = *(int *)this;
    *(undefined2 *)(iVar8 + ((uVar9 & 7) + 0x13) * 8 + uVar4) = uVar3;
    *(uchar *)(iVar6 + 0x3e) = param_2;
    if (*(char *)(iVar6 + 0x3d) < '\0') {
      *(byte *)(iVar6 + 0x3d) = *(char *)(iVar6 + 0x3d) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6a:
    *(uchar *)(*(int *)this + 0x3f) = param_2;
    break;
  case 0x6b:
    iVar6 = *(int *)this;
    uVar4 = (uint)*(byte *)(iVar6 + 0x3f);
    uVar9 = (int)uVar4 >> 3;
    if ((*(byte *)(iVar6 + 0x3f) & 1) == 0) {
      iVar8 = *(int *)(iVar6 + 4);
      puVar5 = (ushort *)(iVar8 + ((uVar9 & 7) + 0xb) * 8 + (uVar4 & 6));
      *puVar5 = *puVar5 & 0xff00 | uVar1;
    }
    else {
      iVar8 = *(int *)(iVar6 + 4);
      puVar5 = (ushort *)(iVar8 + ((uVar9 & 7) + 0xb) * 8 + (uVar4 & 6));
      *puVar5 = (ushort)(byte)*puVar5 | uVar1 << 8;
    }
    uVar3 = (**(code **)(**(int **)(iVar6 + 0x14) + 0x10))(*(int **)(iVar6 + 0x14));
    iVar6 = *(int *)this;
    *(undefined2 *)(iVar8 + ((uVar9 & 7) + 0x1b) * 8 + (uVar4 & 6)) = uVar3;
    *(uchar *)(iVar6 + 0x40) = param_2;
    if (*(char *)(iVar6 + 0x3f) < '\0') {
      *(byte *)(iVar6 + 0x3f) = *(char *)(iVar6 + 0x3f) + 1U & 0x3f | 0x80;
    }
    break;
  case 0x6c:
    this[0xc7a8] = (cpu)(param_2 & 1);
    break;
  case 0x70:
    uVar9 = param_2 & 7;
    iVar6 = 0x1000;
    if ((param_2 & 7) == 0) {
      uVar9 = 1;
    }
    else {
      iVar6 = uVar9 << 0xc;
    }
    *(char *)(*(int *)this + 0x41) = (char)uVar9;
    *(cpu **)(this + 0xc160) = this + iVar6 + 0x12;
    break;
  case 0x72:
    this[0xc7a9] = (cpu)param_2;
    break;
  case 0x73:
    this[0xc7aa] = (cpu)param_2;
    break;
  case 0x74:
    this[0xc7ab] = (cpu)param_2;
    break;
  case 0x75:
    this[0xc7ac] = (cpu)(param_2 & 0x70);
    break;
  case 0xff:
    *(uchar *)(*(int *)this + 0x34) = param_2;
  }
  return;
}



/* ===== FUNCTION ~gb @ 00025840 ===== */

/* gb::~gb() */

void __thiscall gb::~gb(gb *this)

{
  cpu *this_00;
  mbc *this_01;
  rom *this_02;
  apu *this_03;
  lcd *this_04;
  
  this_01 = *(mbc **)(this + 0x10);
  *(undefined4 *)(*(int *)(this + 0x14) + 4) = 0;
  if (this_01 != (mbc *)0x0) {
    mbc::~mbc(this_01);
    operator_delete(this_01);
  }
  this_02 = *(rom **)(this + 0xc);
  if (this_02 != (rom *)0x0) {
    rom::~rom(this_02);
    operator_delete(this_02);
  }
  this_03 = *(apu **)(this + 8);
  if (this_03 != (apu *)0x0) {
    apu::~apu(this_03);
    operator_delete(this_03);
  }
  this_04 = *(lcd **)(this + 4);
  if (this_04 != (lcd *)0x0) {
    lcd::~lcd(this_04);
    operator_delete(this_04);
  }
  this_00 = *(cpu **)this;
  if (this_00 != (cpu *)0x0) {
    cpu::~cpu(this_00);
                    /* WARNING: Could not recover jumptable at 0x00025938. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    operator_delete(this_00);
    return;
  }
  return;
}



/* ===== FUNCTION reset @ 00025954 ===== */

/* gb::reset() */

void __thiscall gb::reset(gb *this)

{
  char cVar1;
  int iVar2;
  char *local_20 [4];
  char *local_10;
  
  this[0x26] = (gb)0x0;
  this[0x25] = (gb)0x0;
  this[0x24] = (gb)0x0;
  this[0x23] = (gb)0x0;
  this[0x22] = (gb)0x0;
  this[0x30] = (gb)0xff;
  this[0x28] = (gb)0x91;
  this[0x31] = (gb)0xff;
  this[0x2c] = (gb)0x99;
  this[0x27] = (gb)0x0;
  this[0x29] = (gb)0x0;
  this[0x2a] = (gb)0x0;
  this[0x2b] = (gb)0x0;
  this[0x2d] = (gb)0x0;
  this[0x32] = (gb)0x0;
  this[0x33] = (gb)0x0;
  this[0x34] = (gb)0x0;
  this[0x35] = (gb)0x0;
  this[0x36] = (gb)0x0;
  this[0x37] = (gb)0x0;
  this[0x38] = (gb)0x0;
  this[0x39] = (gb)0x0;
  this[0x3a] = (gb)0x0;
  this[0x3b] = (gb)0x0;
  this[0x3c] = (gb)0x0;
  this[0x3d] = (gb)0x0;
  this[0x3e] = (gb)0x0;
  this[0x2f] = (gb)0xfc;
  this[0x3f] = (gb)0x0;
  this[0x40] = (gb)0x0;
  this[0x41] = (gb)0x0;
  iVar2 = *(int *)(this + 0xc);
  if (*(char *)(iVar2 + 0x30) != '\0') {
    if (*(char *)(*(int *)(iVar2 + 0x2c) + 0x143) < '\0') {
      *(uint *)(iVar2 + 0x20) = (this[0x1379d] != (gb)0x0) + 3;
    }
    else {
      *(undefined4 *)(iVar2 + 0x20) = 1;
    }
  }
  cpu::reset(*(cpu **)this);
  lcd::reset(*(lcd **)(this + 4));
  apu::reset(*(apu **)(this + 8));
  mbc::reset(*(mbc **)(this + 0x10));
  cVar1 = *(char *)(*(int *)(this + 0xc) + 0x30);
  *(undefined4 *)(this + 0x1378c) = 0;
  *(undefined4 *)(this + 0x13790) = 0;
  *(undefined4 *)(this + 0x13794) = 0;
  *(undefined4 *)(this + 0x13798) = 0;
  local_20[0] = "Invalid";
  local_20[1] = "Gameboy";
  local_20[2] = "SuperGameboy";
  local_20[3] = "Gameboy Color";
  local_10 = "Gameboy Advance";
  if (cVar1 != '\0') {
                    /* WARNING: Could not recover jumptable at 0x00025b04. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(this + 0x14) + 0x28))
              (*(int **)(this + 0x14),"Current GB Type : %s \n",
               local_20[*(int *)(*(int *)(this + 0xc) + 0x20)]);
    return;
  }
  return;
}



/* ===== FUNCTION gb @ 00025b28 ===== */

/* gb::gb(renderer*, bool, bool) */

void __thiscall gb::gb(gb *this,renderer *param_1,bool param_2,bool param_3)

{
  lcd *this_00;
  rom *this_01;
  apu *this_02;
  mbc *this_03;
  cpu *this_04;
  cheat *this_05;
  undefined4 uVar1;
  undefined3 in_register_0000001d;
  code *pcVar2;
  
  *(renderer **)(this + 0x14) = param_1;
  this_00 = operator_new(0x3b0);
                    /* try { // try from 00025b74 to 00025b7b has its CatchHandler @ 00025d0c */
  lcd::lcd(this_00,this);
  *(lcd **)(this + 4) = this_00;
  this_01 = operator_new(0x34);
                    /* try { // try from 00025b9c to 00025ba3 has its CatchHandler @ 00025d04 */
  rom::rom(this_01);
  *(rom **)(this + 0xc) = this_01;
  this_02 = operator_new(8);
                    /* try { // try from 00025bc8 to 00025bcf has its CatchHandler @ 00025cfc */
  apu::apu(this_02,this);
  *(apu **)(this + 8) = this_02;
  this_03 = operator_new(0x2c);
                    /* try { // try from 00025bf4 to 00025bfb has its CatchHandler @ 00025cf4 */
  mbc::mbc(this_03,this);
  *(mbc **)(this + 0x10) = this_03;
  this_04 = operator_new(0xc7b0);
                    /* try { // try from 00025c20 to 00025c27 has its CatchHandler @ 00025cec */
  cpu::cpu(this_04,this);
  *(cpu **)this = this_04;
  this_05 = operator_new(0x40010);
                    /* try { // try from 00025c50 to 00025c57 has its CatchHandler @ 00025cc8 */
  cheat::cheat(this_05,this);
  pcVar2 = *(code **)**(undefined4 **)(this + 0x14);
  *(cheat **)(this + 0x18) = this_05;
  *(undefined4 *)(this + 0x1c) = 0;
  (*pcVar2)(*(undefined4 **)(this + 0x14));
  if (CONCAT31(in_register_0000001d,param_3) == 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)(*(int *)(this + 8) + 4);
  }
  *(undefined4 *)(*(int *)(this + 0x14) + 4) = uVar1;
  reset(this);
  this[0x1379c] = (gb)0x0;
  this[0x1379d] = (gb)0x0;
  return;
}



/* ===== FUNCTION hook_extport @ 00025d14 ===== */

/* gb::hook_extport(ext_hook*) */

void __thiscall gb::hook_extport(gb *this,ext_hook *param_1)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)param_1;
  *(undefined4 *)(this + 0x13788) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 0x13784) = uVar1;
  this[0x1379c] = (gb)0x1;
  return;
}



/* ===== FUNCTION unhook_extport @ 00025d38 ===== */

/* gb::unhook_extport() */

void __thiscall gb::unhook_extport(gb *this)

{
  this[0x1379c] = (gb)0x0;
  return;
}



/* ===== FUNCTION set_skip @ 00025d48 ===== */

/* gb::set_skip(int) */

void __thiscall gb::set_skip(gb *this,int param_1)

{
  *(int *)(this + 0x13790) = param_1;
  return;
}



/* ===== FUNCTION load_rom @ 00025d58 ===== */

/* gb::load_rom(unsigned char*, int, unsigned char*, int) */

int __thiscall gb::load_rom(gb *this,uchar *param_1,int param_2,uchar *param_3,int param_4)

{
  int iVar1;
  
  iVar1 = rom::load_rom(*(rom **)(this + 0xc),param_1,param_2,param_3,param_4);
  if (iVar1 != 0) {
    reset(this);
  }
  return iVar1;
}



/* ===== FUNCTION serialize_legacy @ 00025db8 ===== */

/* gb::serialize_legacy(serializer&) */

void __thiscall gb::serialize_legacy(gb *this,serializer *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  bool bVar7;
  undefined4 *puVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  mbc *this_00;
  uint *puVar17;
  gb *pgVar18;
  apu *paVar19;
  uint uVar20;
  void *__dest;
  uint *puVar21;
  undefined4 uVar22;
  int *piVar23;
  int iVar24;
  uint uVar25;
  undefined4 uVar26;
  int *piVar27;
  undefined4 uVar28;
  int iVar29;
  undefined4 uVar30;
  uint *puVar31;
  void *__src;
  uint uVar32;
  int iVar33;
  size_t __n;
  code *pcVar34;
  int local_188 [67];
  uint local_7c;
  uint local_78;
  uint local_74;
  uint local_70;
  uint local_6c;
  int local_48 [6];
  int *local_30;
  int local_2c;
  undefined4 local_28;
  uint local_24;
  uint local_20;
  int local_1c;
  
  iVar12 = *(int *)(this + 0xc);
  local_48[0] = 1;
  local_48[1] = 1;
  local_48[2] = 1;
  local_48[5] = 8;
  local_48[3] = 4;
  local_48[4] = 0x10;
  if (*(uint *)param_1 < 5) {
    puVar16 = (undefined4 *)(iVar12 + 0x20);
    switch(*(uint *)param_1) {
    case 0:
      piVar27 = *(int **)(param_1 + 4);
      iVar33 = *piVar27 + 4;
      *piVar27 = iVar33;
      bVar7 = *(int *)(iVar12 + 0x20) < 3;
      if (bVar7) goto switchD_00025e9c_caseD_0;
      goto LAB_00025f58;
    case 1:
      **(undefined4 **)(param_1 + 4) = *puVar16;
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      iVar12 = *(int *)(this + 0xc);
      iVar33 = *(int *)(iVar12 + 0x20);
      goto joined_r0x00025e68;
    case 2:
      *puVar16 = **(undefined4 **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      goto LAB_00025e5c;
    case 3:
      pcVar34 = fwrite;
      break;
    case 4:
      pcVar34 = fread;
    }
    (*pcVar34)(puVar16,1,4,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00025e5c:
  iVar12 = *(int *)(this + 0xc);
  iVar33 = *(int *)(iVar12 + 0x20);
joined_r0x00025e68:
  bVar7 = iVar33 < 3;
  if (bVar7) {
    iVar33 = *(int *)this;
    puVar16 = (undefined4 *)(iVar33 + 0x12);
    switch(*(undefined4 *)param_1) {
    case 0:
switchD_00025e9c_caseD_0:
      piVar27 = *(int **)(param_1 + 4);
      iVar33 = *piVar27 + 0x2000;
      *piVar27 = iVar33;
      goto LAB_000277b8;
    case 1:
      puVar8 = *(undefined4 **)(param_1 + 4);
      do {
        uVar30 = *puVar16;
        uVar28 = puVar16[1];
        uVar22 = puVar16[2];
        uVar26 = puVar16[3];
        puVar16 = puVar16 + 4;
        *puVar8 = uVar30;
        puVar8[1] = uVar28;
        puVar8[2] = uVar22;
        puVar8[3] = uVar26;
        puVar8 = puVar8 + 4;
      } while (puVar16 != (undefined4 *)(iVar33 + 0x2012));
      iVar12 = *(int *)(param_1 + 4);
      break;
    case 2:
      puVar8 = *(undefined4 **)(param_1 + 4);
      puVar14 = puVar8 + 0x800;
      do {
        uVar30 = *puVar8;
        uVar28 = puVar8[1];
        uVar22 = puVar8[2];
        uVar26 = puVar8[3];
        puVar8 = puVar8 + 4;
        *puVar16 = uVar30;
        puVar16[1] = uVar28;
        puVar16[2] = uVar22;
        puVar16[3] = uVar26;
        puVar16 = puVar16 + 4;
      } while (puVar8 != puVar14);
      iVar12 = *(int *)(param_1 + 4);
      break;
    case 3:
      pcVar34 = fwrite;
      goto LAB_00026010;
    case 4:
      pcVar34 = fread;
LAB_00026010:
      (*pcVar34)(puVar16,1,0x2000,*(undefined4 *)(param_1 + 4));
      goto LAB_00026024;
    default:
      puts("ERROR: invalid serializer!");
      goto LAB_00026024;
    }
    *(int *)(param_1 + 4) = iVar12 + 0x2000;
LAB_00026024:
    iVar12 = *(int *)this;
    puVar16 = (undefined4 *)(iVar12 + 0x8012);
    switch(*(undefined4 *)param_1) {
    case 0:
      piVar27 = *(int **)(param_1 + 4);
      iVar12 = *(int *)(this + 0xc);
      iVar33 = *piVar27;
LAB_000277b8:
      iVar33 = iVar33 + 0x2000;
      *piVar27 = iVar33;
      goto LAB_00025f6c;
    case 1:
      puVar8 = *(undefined4 **)(param_1 + 4);
      do {
        uVar30 = *puVar16;
        uVar28 = puVar16[1];
        uVar22 = puVar16[2];
        uVar26 = puVar16[3];
        puVar16 = puVar16 + 4;
        *puVar8 = uVar30;
        puVar8[1] = uVar28;
        puVar8[2] = uVar22;
        puVar8[3] = uVar26;
        puVar8 = puVar8 + 4;
      } while (puVar16 != (undefined4 *)(iVar12 + 0xa012));
      iVar12 = *(int *)(param_1 + 4);
      break;
    case 2:
      puVar8 = *(undefined4 **)(param_1 + 4);
      puVar14 = puVar8 + 0x800;
      do {
        uVar30 = *puVar8;
        uVar28 = puVar8[1];
        uVar22 = puVar8[2];
        uVar26 = puVar8[3];
        puVar8 = puVar8 + 4;
        *puVar16 = uVar30;
        puVar16[1] = uVar28;
        puVar16[2] = uVar22;
        puVar16[3] = uVar26;
        puVar16 = puVar16 + 4;
      } while (puVar8 != puVar14);
      iVar12 = *(int *)(param_1 + 4);
      break;
    case 3:
      pcVar34 = fwrite;
      goto LAB_00026f18;
    case 4:
      pcVar34 = fread;
LAB_00026f18:
      (*pcVar34)(puVar16,1,0x2000,*(undefined4 *)(param_1 + 4));
      goto LAB_00026f2c;
    default:
      goto LAB_00027778;
    }
    *(int *)(param_1 + 4) = iVar12 + 0x2000;
    goto LAB_00026f2c;
  }
  iVar33 = *(int *)this;
  puVar16 = (undefined4 *)(iVar33 + 0x12);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar27 = *(int **)(param_1 + 4);
    iVar33 = *piVar27;
LAB_00025f58:
    iVar33 = iVar33 + 0x8000;
    *piVar27 = iVar33;
    goto LAB_00025f64;
  case 1:
    puVar8 = *(undefined4 **)(param_1 + 4);
    do {
      uVar30 = *puVar16;
      uVar28 = puVar16[1];
      uVar22 = puVar16[2];
      uVar26 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar8 = uVar30;
      puVar8[1] = uVar28;
      puVar8[2] = uVar22;
      puVar8[3] = uVar26;
      puVar8 = puVar8 + 4;
    } while (puVar16 != (undefined4 *)(iVar33 + 0x8012));
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar8 + 0x2000;
    do {
      uVar30 = *puVar8;
      uVar28 = puVar8[1];
      uVar22 = puVar8[2];
      uVar26 = puVar8[3];
      puVar8 = puVar8 + 4;
      *puVar16 = uVar30;
      puVar16[1] = uVar28;
      puVar16[2] = uVar22;
      puVar16[3] = uVar26;
      puVar16 = puVar16 + 4;
    } while (puVar8 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_0002734c;
  case 4:
    pcVar34 = fread;
LAB_0002734c:
    (*pcVar34)(puVar16,1,0x8000,*(undefined4 *)(param_1 + 4));
    goto LAB_00027360;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00027360;
  }
  *(int *)(param_1 + 4) = iVar12 + 0x8000;
LAB_00027360:
  iVar12 = *(int *)this;
  puVar16 = (undefined4 *)(iVar12 + 0x8012);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar27 = *(int **)(param_1 + 4);
    iVar12 = *(int *)(this + 0xc);
    iVar33 = *piVar27;
LAB_00025f64:
    iVar33 = iVar33 + 0x4000;
    *piVar27 = iVar33;
LAB_00025f6c:
    iVar33 = local_48[*(byte *)(iVar12 + 0x1d)] * 0x2000 + iVar33;
    *piVar27 = iVar33;
    goto LAB_00025f8c;
  case 1:
    puVar8 = *(undefined4 **)(param_1 + 4);
    do {
      uVar30 = *puVar16;
      uVar28 = puVar16[1];
      uVar22 = puVar16[2];
      uVar26 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar8 = uVar30;
      puVar8[1] = uVar28;
      puVar8[2] = uVar22;
      puVar8[3] = uVar26;
      puVar8 = puVar8 + 4;
    } while (puVar16 != (undefined4 *)(iVar12 + 0xc012));
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar8 + 0x1000;
    do {
      uVar30 = *puVar8;
      uVar28 = puVar8[1];
      uVar22 = puVar8[2];
      uVar26 = puVar8[3];
      puVar8 = puVar8 + 4;
      *puVar16 = uVar30;
      puVar16[1] = uVar28;
      puVar16[2] = uVar22;
      puVar16[3] = uVar26;
      puVar16 = puVar16 + 4;
    } while (puVar8 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_0002739c;
  case 4:
    pcVar34 = fread;
LAB_0002739c:
    (*pcVar34)(puVar16,1,0x4000,*(undefined4 *)(param_1 + 4));
    iVar12 = *(int *)(this + 0xc);
    uVar20 = *(uint *)param_1;
    __src = *(void **)(iVar12 + 0x28);
    iVar33 = local_48[*(byte *)(iVar12 + 0x1d)];
    goto joined_r0x000273d4;
  default:
LAB_00027778:
    puts("ERROR: invalid serializer!");
    goto LAB_00026f2c;
  }
  *(int *)(param_1 + 4) = iVar12 + 0x4000;
LAB_00026f2c:
  iVar12 = *(int *)(this + 0xc);
  uVar20 = *(uint *)param_1;
  __src = *(void **)(iVar12 + 0x28);
  iVar33 = local_48[*(byte *)(iVar12 + 0x1d)];
joined_r0x000273d4:
  if (4 < uVar20) {
    puts("ERROR: invalid serializer!");
    goto LAB_00026f94;
  }
  __n = iVar33 * 0x2000;
  switch(uVar20) {
  case 0:
    piVar27 = *(int **)(param_1 + 4);
    iVar33 = __n + *piVar27;
    *piVar27 = iVar33;
    goto LAB_00025f8c;
  case 1:
    __dest = *(void **)(param_1 + 4);
    break;
  case 2:
    __dest = __src;
    __src = *(void **)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00026f7c;
  case 4:
    pcVar34 = fread;
LAB_00026f7c:
    (*pcVar34)(__src,1,__n,*(undefined4 *)(param_1 + 4));
LAB_00026f94:
    uVar20 = *(uint *)param_1;
    iVar12 = *(int *)this;
    goto joined_r0x00026fa4;
  }
  memcpy(__dest,__src,__n);
  uVar20 = *(uint *)param_1;
  *(size_t *)(param_1 + 4) = *(int *)(param_1 + 4) + __n;
  iVar12 = *(int *)this;
joined_r0x00026fa4:
  if (4 < uVar20) {
    puts("ERROR: invalid serializer!");
    uVar20 = *(uint *)param_1;
    iVar12 = *(int *)this;
    goto joined_r0x0002723c;
  }
  puVar16 = (undefined4 *)(iVar12 + 0xc092);
  switch(uVar20) {
  case 0:
    piVar27 = *(int **)(param_1 + 4);
    iVar12 = *(int *)(this + 0xc);
    iVar33 = *piVar27;
LAB_00025f8c:
    iVar33 = iVar33 + 0xa0;
    *piVar27 = iVar33;
    goto LAB_00025f94;
  case 1:
    puVar8 = *(undefined4 **)(param_1 + 4);
    do {
      uVar30 = *puVar16;
      uVar28 = puVar16[1];
      uVar22 = puVar16[2];
      uVar26 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar8 = uVar30;
      puVar8[1] = uVar28;
      puVar8[2] = uVar22;
      puVar8[3] = uVar26;
      puVar8 = puVar8 + 4;
    } while (puVar16 != (undefined4 *)(iVar12 + 0xc132));
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar8 + 0x28;
    do {
      uVar30 = *puVar8;
      uVar28 = puVar8[1];
      uVar22 = puVar8[2];
      uVar26 = puVar8[3];
      puVar8 = puVar8 + 4;
      *puVar16 = uVar30;
      puVar16[1] = uVar28;
      puVar16[2] = uVar22;
      puVar16[3] = uVar26;
      puVar16 = puVar16 + 4;
    } while (puVar8 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00026fd0;
  case 4:
    pcVar34 = fread;
LAB_00026fd0:
    (*pcVar34)(puVar16,1,0xa0,*(undefined4 *)(param_1 + 4));
    goto LAB_00026fe4;
  }
  *(int *)(param_1 + 4) = iVar12 + 0xa0;
LAB_00026fe4:
  uVar20 = *(uint *)param_1;
  iVar12 = *(int *)this;
joined_r0x0002723c:
  if (4 < uVar20) {
    puts("ERROR: invalid serializer!");
    uVar26 = *(undefined4 *)param_1;
    iVar12 = *(int *)(this + 0xc);
    goto LAB_00025fa0;
  }
  puVar16 = (undefined4 *)(iVar12 + 0xc012);
  switch(uVar20) {
  case 0:
    piVar27 = *(int **)(param_1 + 4);
    iVar12 = *(int *)(this + 0xc);
    iVar33 = *piVar27;
LAB_00025f94:
    uVar26 = 0;
    *piVar27 = iVar33 + 0x80;
    goto LAB_00025fa0;
  case 1:
    puVar8 = *(undefined4 **)(param_1 + 4);
    do {
      uVar30 = *puVar16;
      uVar28 = puVar16[1];
      uVar22 = puVar16[2];
      uVar26 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar8 = uVar30;
      puVar8[1] = uVar28;
      puVar8[2] = uVar22;
      puVar8[3] = uVar26;
      puVar8 = puVar8 + 4;
    } while (puVar16 != (undefined4 *)(iVar12 + 0xc092));
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar8 + 0x20;
    do {
      uVar30 = *puVar8;
      uVar28 = puVar8[1];
      uVar22 = puVar8[2];
      uVar26 = puVar8[3];
      puVar8 = puVar8 + 4;
      *puVar16 = uVar30;
      puVar16[1] = uVar28;
      puVar16[2] = uVar22;
      puVar16[3] = uVar26;
      puVar16 = puVar16 + 4;
    } while (puVar8 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00027020;
  case 4:
    pcVar34 = fread;
LAB_00027020:
    (*pcVar34)(puVar16,1,0x80,*(undefined4 *)(param_1 + 4));
    uVar26 = *(undefined4 *)param_1;
    iVar12 = *(int *)(this + 0xc);
    goto LAB_00025fa0;
  }
  uVar26 = *(undefined4 *)param_1;
  *(int *)(param_1 + 4) = iVar12 + 0x80;
  iVar12 = *(int *)(this + 0xc);
LAB_00025fa0:
  piVar27 = local_188;
  this_00 = *(mbc **)(this + 0x10);
  uVar13 = *(int *)this_00 - *(int *)(iVar12 + 0x2c);
  local_20 = *(int *)(this_00 + 4) - *(int *)(iVar12 + 0x28);
  uVar32 = uVar13 + 0x3fff;
  uVar20 = local_20 + 0x1fff;
  puVar31 = (uint *)(uint)((int)local_20 < 0);
  if ((int)uVar13 < 0) {
    uVar13 = uVar32;
  }
  if (puVar31 != (uint *)0x0) {
    local_20 = uVar20;
  }
  local_1c = (int)uVar13 >> 0xe;
  local_20 = (int)local_20 >> 0xd;
  switch(uVar26) {
  case 0:
    piVar23 = *(int **)(param_1 + 4);
    iVar12 = *piVar23;
    *piVar23 = iVar12 + 4;
    *piVar23 = iVar12 + 8;
    goto LAB_0002618c;
  case 1:
    **(int **)(param_1 + 4) = local_1c;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    uVar13 = *(uint *)param_1;
    if (uVar13 < 5) goto LAB_00026150;
    goto LAB_00026e18;
  case 2:
    puVar21 = (uint *)(*(int **)(param_1 + 4) + 1);
    local_1c = **(int **)(param_1 + 4);
    *(uint **)(param_1 + 4) = puVar21;
    goto LAB_00026dd4;
  case 3:
    pcVar34 = fwrite;
    break;
  case 4:
    pcVar34 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00026140;
  }
  uVar20 = *(uint *)(param_1 + 4);
  (*pcVar34)(&local_1c,1,4);
LAB_00026140:
  uVar13 = *(uint *)param_1;
  if (4 < uVar13) {
LAB_00026e18:
    puts("ERROR: invalid serializer!");
    this_00 = *(mbc **)(this + 0x10);
    goto LAB_0002618c;
  }
LAB_00026150:
  switch((&switchD_00026164::switchdataD_000358b4)[uVar13] + 0x50250 & 0xfffffffe) {
  case 0x2616c:
    pcVar34 = fwrite;
    goto LAB_00026170;
  case 0x26d38:
    **(uint **)(param_1 + 4) = local_20;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    uVar13 = local_20;
    mbc::set_page(*(mbc **)(this + 0x10),local_1c,local_20);
    goto joined_r0x00026d68;
  case 0x26db4:
    pcVar34 = fread;
LAB_00026170:
    uVar20 = *(uint *)(param_1 + 4);
    (*pcVar34)(&local_20,1,4);
    this_00 = *(mbc **)(this + 0x10);
    break;
  case 0x27b38:
    this_00 = *(mbc **)(this + 0x10);
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 0x27b68:
    puVar21 = *(uint **)(param_1 + 4);
    this_00 = *(mbc **)(this + 0x10);
LAB_00026dd4:
    local_20 = *puVar21;
    *(uint **)(param_1 + 4) = puVar21 + 1;
  }
LAB_0002618c:
  uVar13 = local_20;
  mbc::set_page(this_00,local_1c,local_20);
joined_r0x00026d68:
  if (bVar7) goto LAB_000261a4;
  cpu::save_state(*(cpu **)this,local_188 + 0x40);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar23 = *(int **)(param_1 + 4);
    iVar33 = *(int *)this;
    iVar12 = *piVar23 + 8;
    *piVar23 = iVar12;
    goto LAB_00026eb4;
  case 1:
    puVar16 = *(undefined4 **)(param_1 + 4);
    *puVar16 = local_188[0x40];
    puVar16[1] = local_188[0x41];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 8;
    break;
  case 2:
    puVar16 = *(undefined4 **)(param_1 + 4);
    puVar21 = puVar16 + 2;
    local_188[0x40] = *puVar16;
    local_188[0x41] = puVar16[1];
    *(uint **)(param_1 + 4) = puVar21;
    puVar17 = (uint *)(*(int *)this + 4);
    goto LAB_00026e5c;
  case 3:
    pcVar34 = fwrite;
    goto LAB_000279f4;
  case 4:
    pcVar34 = fread;
LAB_000279f4:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 8;
    (*pcVar34)(local_188 + 0x40,1);
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
LAB_000261a4:
  iVar33 = *(int *)this;
  puVar17 = (uint *)(iVar33 + 4);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar23 = *(int **)(param_1 + 4);
    iVar12 = *piVar23;
LAB_00026eb4:
    iVar12 = iVar12 + 0xe;
    *piVar23 = iVar12;
    goto LAB_00026ebc;
  case 1:
    puVar21 = *(uint **)(param_1 + 4);
    uVar13 = *puVar17;
    uVar25 = *(uint *)(iVar33 + 8);
    uVar9 = *(uint *)(iVar33 + 0xc);
    *puVar21 = uVar13;
    puVar21[1] = uVar25;
    puVar21[2] = uVar9;
    *(undefined1 *)(puVar21 + 3) = *(undefined1 *)(iVar33 + 0x10);
    *(undefined1 *)((int)puVar21 + 0xd) = *(undefined1 *)(iVar33 + 0x11);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xe;
    uVar9 = *(uint *)param_1;
    if (uVar9 < 5) goto LAB_00026200;
    goto LAB_00026d18;
  case 2:
    puVar21 = *(uint **)(param_1 + 4);
LAB_00026e5c:
    uVar13 = *puVar21;
    uVar25 = puVar21[1];
    uVar9 = puVar21[2];
    *puVar17 = uVar13;
    puVar17[1] = uVar25;
    puVar17[2] = uVar9;
    *(char *)(puVar17 + 3) = (char)puVar21[3];
    *(undefined1 *)((int)puVar17 + 0xd) = *(undefined1 *)((int)puVar21 + 0xd);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xe;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_000261dc;
  case 4:
    pcVar34 = fread;
LAB_000261dc:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 0xe;
    (*pcVar34)(puVar17,1);
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  uVar9 = *(uint *)param_1;
  if (4 < uVar9) {
LAB_00026d18:
    puts("ERROR: invalid serializer!");
    goto LAB_00026238;
  }
LAB_00026200:
  pgVar18 = this + 0x20;
  switch((&switchD_00026218::switchdataD_000358dc)[uVar9] + 0x50250 & 0xfffffffe) {
  case 0x26220:
    pcVar34 = fwrite;
    goto LAB_00026224;
  case 0x262b0:
    puVar16 = *(undefined4 **)(param_1 + 4);
    uVar9 = (int)puVar16 + 0xbU & 3;
    puVar31 = (uint *)*puVar16;
    uVar20 = puVar16[1];
    uVar22 = puVar16[3];
    uVar26 = puVar16[4];
    uVar25 = (uint)(puVar16 + 2) & 3;
    uVar13 = (*(int *)(((int)puVar16 + 0xbU) - uVar9) << (3 - uVar9) * 8 |
             uVar13 & 0xffffffffU >> (uVar9 + 1) * 8) & -1 << (4 - uVar25) * 8 |
             *(uint *)((int)(puVar16 + 2) - uVar25) >> uVar25 * 8;
    *(uint **)pgVar18 = puVar31;
    *(uint *)(this + 0x24) = uVar20;
    *(uint *)(this + 0x28) = uVar13;
    *(undefined4 *)(this + 0x2c) = uVar22;
    *(undefined4 *)(this + 0x30) = uVar26;
    this[0x34] = *(gb *)(puVar16 + 5);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x15;
    break;
  case 0x2631c:
    uVar9 = (uint)(this + 0x23) & 3;
    uVar25 = (uint)(this + 0x27) & 3;
    puVar16 = *(undefined4 **)(param_1 + 4);
    uVar1 = (uint)(this + 0x2b) & 3;
    uVar5 = (uint)pgVar18 & 3;
    puVar31 = (uint *)((*(int *)(this + 0x23 + -uVar9) << (3 - uVar9) * 8 |
                       (uint)puVar31 & 0xffffffffU >> (uVar9 + 1) * 8) & -1 << (4 - uVar5) * 8 |
                      *(uint *)(pgVar18 + -uVar5) >> uVar5 * 8);
    uVar9 = (uint)(this + 0x24) & 3;
    uVar20 = (*(int *)(this + 0x27 + -uVar25) << (3 - uVar25) * 8 |
             uVar20 & 0xffffffffU >> (uVar25 + 1) * 8) & -1 << (4 - uVar9) * 8 |
             *(uint *)(this + 0x24 + -uVar9) >> uVar9 * 8;
    uVar22 = *(undefined4 *)(this + 0x2c);
    uVar26 = *(undefined4 *)(this + 0x30);
    uVar9 = (uint)(this + 0x28) & 3;
    uVar13 = (*(int *)(this + 0x2b + -uVar1) << (3 - uVar1) * 8 |
             uVar13 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar9) * 8 |
             *(uint *)(this + 0x28 + -uVar9) >> uVar9 * 8;
    *puVar16 = puVar31;
    puVar16[1] = uVar20;
    puVar16[2] = uVar13;
    puVar16[3] = uVar22;
    puVar16[4] = uVar26;
    *(gb *)(puVar16 + 5) = this[0x34];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x15;
    break;
  case 0x26388:
    pcVar34 = fread;
LAB_00026224:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 0x15;
    (*pcVar34)(pgVar18,1);
    break;
  case 0x27a8c:
    piVar23 = *(int **)(param_1 + 4);
    iVar33 = *(int *)this;
    iVar12 = *piVar23;
LAB_00026ebc:
    iVar12 = iVar12 + 0x15;
    *piVar23 = iVar12;
    if (!bVar7) goto LAB_00027af8;
    goto LAB_00026ec8;
  }
LAB_00026238:
  if (bVar7) goto LAB_00026240;
  pgVar18 = this + 0x35;
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar23 = *(int **)(param_1 + 4);
    iVar33 = *(int *)this;
    iVar12 = *piVar23;
LAB_00027af8:
    *piVar23 = iVar12 + 0xd;
    *piVar23 = iVar12 + 0x8d;
    goto LAB_00026ec8;
  case 1:
    puVar21 = *(uint **)(param_1 + 4);
    uVar13 = *(uint *)pgVar18;
    uVar25 = *(uint *)(this + 0x39);
    uVar9 = *(uint *)(this + 0x3d);
    *puVar21 = uVar13;
    puVar21[1] = uVar25;
    puVar21[2] = uVar9;
    *(gb *)(puVar21 + 3) = this[0x41];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xd;
    break;
  case 2:
    puVar21 = *(uint **)(param_1 + 4);
    uVar13 = *puVar21;
    uVar25 = puVar21[1];
    uVar9 = puVar21[2];
    *(uint *)pgVar18 = uVar13;
    *(uint *)(this + 0x39) = uVar25;
    *(uint *)(this + 0x3d) = uVar9;
    this[0x41] = *(gb *)(puVar21 + 3);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xd;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00027814;
  case 4:
    pcVar34 = fread;
LAB_00027814:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 0xd;
    (*pcVar34)(pgVar18,1);
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  puVar21 = (uint *)(*(int *)(this + 4) + 0x18);
  switch(*(undefined4 *)param_1) {
  case 0:
    iVar33 = *(int *)this;
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x80;
LAB_00026ec8:
    local_24 = (uint)*(byte *)(iVar33 + 0xc77c);
    goto switchD_00026278_caseD_0;
  case 1:
    puVar17 = *(uint **)(param_1 + 4);
    puVar31 = (uint *)(*(int *)(this + 4) + 0x98);
    do {
      uVar20 = *puVar21;
      uVar13 = puVar21[1];
      uVar25 = puVar21[2];
      uVar9 = puVar21[3];
      puVar21 = puVar21 + 4;
      *puVar17 = uVar20;
      puVar17[1] = uVar13;
      puVar17[2] = uVar25;
      puVar17[3] = uVar9;
      puVar17 = puVar17 + 4;
    } while (puVar21 != puVar31);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar17 = *(uint **)(param_1 + 4);
    puVar31 = puVar17 + 0x20;
    do {
      uVar20 = *puVar17;
      uVar13 = puVar17[1];
      uVar25 = puVar17[2];
      uVar9 = puVar17[3];
      puVar17 = puVar17 + 4;
      *puVar21 = uVar20;
      puVar21[1] = uVar13;
      puVar21[2] = uVar25;
      puVar21[3] = uVar9;
      puVar21 = puVar21 + 4;
    } while (puVar17 != puVar31);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00027860;
  case 4:
    pcVar34 = fread;
LAB_00027860:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 0x80;
    (*pcVar34)(puVar21,1);
    uVar26 = *(undefined4 *)param_1;
    iVar33 = *(int *)this;
    goto LAB_00026248;
  default:
    puts("ERROR: invalid serializer!");
LAB_00026240:
    uVar26 = *(undefined4 *)param_1;
    iVar33 = *(int *)this;
    goto LAB_00026248;
  }
  uVar26 = *(undefined4 *)param_1;
  *(int *)(param_1 + 4) = iVar12 + 0x80;
  iVar33 = *(int *)this;
LAB_00026248:
  local_24 = (uint)*(byte *)(iVar33 + 0xc77c);
  switch(uVar26) {
  case 0:
switchD_00026278_caseD_0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    *(bool *)(iVar33 + 0xc77c) = local_24 != 0;
    goto switchD_000263ec_caseD_26f00;
  case 1:
    **(uint **)(param_1 + 4) = local_24;
    uVar9 = *(uint *)param_1;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    *(bool *)(*(int *)this + 0xc77c) = local_24 != 0;
    if (uVar9 < 5) goto LAB_000263d4;
    goto LAB_00026c98;
  case 2:
    local_24 = **(uint **)(param_1 + 4);
    *(uint **)(param_1 + 4) = *(uint **)(param_1 + 4) + 1;
    *(bool *)(iVar33 + 0xc77c) = local_24 != 0;
    goto switchD_000263ec_caseD_26c38;
  case 3:
    pcVar34 = fwrite;
    break;
  case 4:
    pcVar34 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar9 = *(uint *)param_1;
    iVar12 = *(int *)this;
    goto LAB_000263b4;
  }
  uVar20 = *(uint *)(param_1 + 4);
  uVar13 = 4;
  (*pcVar34)(&local_24,1);
  uVar9 = *(uint *)param_1;
  iVar12 = *(int *)this;
LAB_000263b4:
  *(bool *)(iVar12 + 0xc77c) = local_24 != 0;
  if (4 < uVar9) {
LAB_00026c98:
    local_28 = 0;
    puts("ERROR: invalid serializer!");
    goto LAB_00026410;
  }
LAB_000263d4:
  local_28 = 0;
  switch((&switchD_000263ec::switchdataD_00035918)[uVar9] + 0x50250 & 0xfffffffe) {
  case 0x263f4:
    pcVar34 = fwrite;
    goto LAB_000263f8;
  case 0x26be0:
    **(undefined4 **)(param_1 + 4) = 0;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    break;
  case 0x26c00:
    pcVar34 = fread;
LAB_000263f8:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 4;
    (*pcVar34)(&local_28,1);
    break;
  case 0x26c38:
switchD_000263ec_caseD_26c38:
    local_28 = **(undefined4 **)(param_1 + 4);
    *(undefined4 **)(param_1 + 4) = *(undefined4 **)(param_1 + 4) + 1;
    break;
  case 0x26f00:
switchD_000263ec_caseD_26f00:
    local_28 = 0;
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
  }
LAB_00026410:
  local_2c = mbc::get_state(*(mbc **)(this + 0x10));
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 1:
    **(int **)(param_1 + 4) = local_2c;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    break;
  case 2:
    local_2c = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = *(int **)(param_1 + 4) + 1;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00026454;
  case 4:
    pcVar34 = fread;
LAB_00026454:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 4;
    (*pcVar34)(&local_2c,1);
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  mbc::set_state(*(mbc **)(this + 0x10),local_2c);
  uVar9 = *(uint *)(this + 0x10);
  piVar23 = (int *)(uint)*(byte *)(uVar9 + 0x11);
  local_30 = piVar23;
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 1:
    **(undefined4 **)(param_1 + 4) = piVar23;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    uVar9 = *(uint *)(this + 0x10);
    break;
  case 2:
    piVar23 = *(int **)(param_1 + 4) + 1;
    local_30 = (int *)**(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = piVar23;
    *(bool *)(uVar9 + 0x11) = local_30 != (int *)0x0;
    goto joined_r0x00026b14;
  case 3:
    pcVar34 = fwrite;
    goto LAB_000264bc;
  case 4:
    pcVar34 = fread;
LAB_000264bc:
    uVar20 = *(uint *)(param_1 + 4);
    uVar13 = 4;
    piVar23 = (int *)0x1;
    (*pcVar34)(&local_30);
    uVar9 = *(uint *)(this + 0x10);
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar9 = *(uint *)(this + 0x10);
  }
  *(bool *)(uVar9 + 0x11) = local_30 != (int *)0x0;
joined_r0x00026b14:
  if (bVar7) {
    puVar16 = (undefined4 *)apu::get_stat(*(apu **)(this + 8));
    uVar26 = *(undefined4 *)param_1;
    goto code_r0x000264fc;
  }
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x18;
    break;
  case 1:
    puVar31 = *(uint **)(param_1 + 4);
    *puVar31 = local_188[0x42];
    puVar31[1] = local_7c;
    puVar31[2] = local_78;
    puVar31[3] = local_74;
    puVar31[4] = local_70;
    puVar31[5] = local_6c;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x18;
    break;
  case 2:
    uVar11 = *(uint *)(param_1 + 4);
    uVar25 = uVar11 + 3 & 3;
    uVar1 = uVar11 + 7 & 3;
    uVar5 = uVar11 + 0xb & 3;
    uVar2 = uVar11 + 0xf & 3;
    uVar3 = uVar11 + 0x13 & 3;
    uVar4 = uVar11 + 0x17 & 3;
    uVar6 = uVar11 & 3;
    local_188[0x42] =
         (*(int *)((uVar11 + 3) - uVar25) << (3 - uVar25) * 8 |
         uVar32 & 0xffffffffU >> (uVar25 + 1) * 8) & -1 << (4 - uVar6) * 8 |
         *(uint *)(uVar11 - uVar6) >> uVar6 * 8;
    uVar32 = uVar11 + 4 & 3;
    local_7c = (*(int *)((uVar11 + 7) - uVar1) << (3 - uVar1) * 8 |
               (uint)puVar31 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar32) * 8 |
               *(uint *)((uVar11 + 4) - uVar32) >> uVar32 * 8;
    uVar32 = uVar11 + 8 & 3;
    local_78 = (*(int *)((uVar11 + 0xb) - uVar5) << (3 - uVar5) * 8 |
               uVar20 & 0xffffffffU >> (uVar5 + 1) * 8) & -1 << (4 - uVar32) * 8 |
               *(uint *)((uVar11 + 8) - uVar32) >> uVar32 * 8;
    uVar20 = uVar11 + 0xc & 3;
    local_74 = (*(int *)((uVar11 + 0xf) - uVar2) << (3 - uVar2) * 8 |
               uVar13 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar20) * 8 |
               *(uint *)((uVar11 + 0xc) - uVar20) >> uVar20 * 8;
    uVar20 = uVar11 + 0x10 & 3;
    local_70 = (*(int *)((uVar11 + 0x13) - uVar3) << (3 - uVar3) * 8 |
               (uint)piVar23 & 0xffffffffU >> (uVar3 + 1) * 8) & -1 << (4 - uVar20) * 8 |
               *(uint *)((uVar11 + 0x10) - uVar20) >> uVar20 * 8;
    uVar20 = uVar11 + 0x14 & 3;
    local_6c = (*(int *)((uVar11 + 0x17) - uVar4) << (3 - uVar4) * 8 |
               uVar9 & 0xffffffffU >> (uVar4 + 1) * 8) & -1 << (4 - uVar20) * 8 |
               *(uint *)((uVar11 + 0x14) - uVar20) >> uVar20 * 8;
    *(uint *)(param_1 + 4) = uVar11 + 0x18;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00027594;
  case 4:
    pcVar34 = fread;
LAB_00027594:
    (*pcVar34)(local_188 + 0x42,1,0x18,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  cpu::restore_state(*(cpu **)this,local_188 + 0x40);
  puVar16 = (undefined4 *)apu::get_stat(*(apu **)(this + 8));
  uVar26 = *(undefined4 *)param_1;
code_r0x000264fc:
  switch(uVar26) {
  case 0:
    paVar19 = *(apu **)(this + 8);
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0xec;
    puVar16 = (undefined4 *)apu::get_mem(paVar19);
    uVar20 = *(uint *)param_1;
    if (uVar20 < 5) goto LAB_0002655c;
    goto LAB_00026ad4;
  case 1:
    puVar8 = puVar16;
    puVar14 = *(undefined4 **)(param_1 + 4);
    do {
      puVar15 = puVar14;
      puVar10 = puVar8;
      uVar28 = puVar10[1];
      uVar22 = puVar10[2];
      uVar26 = puVar10[3];
      puVar8 = puVar10 + 4;
      *puVar15 = *puVar10;
      puVar15[1] = uVar28;
      puVar15[2] = uVar22;
      puVar15[3] = uVar26;
      puVar14 = puVar15 + 4;
    } while (puVar8 != puVar16 + 0x38);
    uVar22 = puVar10[5];
    uVar26 = puVar10[6];
    puVar15[4] = *puVar8;
    puVar15[5] = uVar22;
    puVar15[6] = uVar26;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 2:
    puVar14 = *(undefined4 **)(param_1 + 4);
    puVar8 = puVar14;
    do {
      puVar15 = puVar8;
      puVar10 = puVar16;
      uVar28 = puVar15[1];
      uVar22 = puVar15[2];
      uVar26 = puVar15[3];
      puVar8 = puVar15 + 4;
      *puVar10 = *puVar15;
      puVar10[1] = uVar28;
      puVar10[2] = uVar22;
      puVar10[3] = uVar26;
      puVar16 = puVar10 + 4;
    } while (puVar8 != puVar14 + 0x38);
    uVar22 = puVar15[5];
    uVar26 = puVar15[6];
    puVar10[4] = *puVar8;
    puVar10[5] = uVar22;
    puVar10[6] = uVar26;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_00026528;
  case 4:
    pcVar34 = fread;
LAB_00026528:
    (*pcVar34)(puVar16,1,0xec,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  puVar16 = (undefined4 *)apu::get_mem(*(apu **)(this + 8));
  uVar20 = *(uint *)param_1;
  if (4 < uVar20) {
LAB_00026ad4:
    puts("ERROR: invalid serializer!");
    goto LAB_00026598;
  }
LAB_0002655c:
  switch((&switchD_00026574::switchdataD_00035968)[uVar20] + 0x50250 & 0xfffffffe) {
  case 0x2657c:
    pcVar34 = fwrite;
    goto LAB_00026580;
  case 0x26870:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar8 + 0xc;
    do {
      uVar30 = *puVar8;
      uVar28 = puVar8[1];
      uVar22 = puVar8[2];
      uVar26 = puVar8[3];
      puVar8 = puVar8 + 4;
      *puVar16 = uVar30;
      puVar16[1] = uVar28;
      puVar16[2] = uVar22;
      puVar16[3] = uVar26;
      puVar16 = puVar16 + 4;
    } while (puVar8 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 0x268d4:
    puVar8 = *(undefined4 **)(param_1 + 4);
    puVar14 = puVar16 + 0xc;
    do {
      uVar30 = *puVar16;
      uVar28 = puVar16[1];
      uVar22 = puVar16[2];
      uVar26 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar8 = uVar30;
      puVar8[1] = uVar28;
      puVar8[2] = uVar22;
      puVar8[3] = uVar26;
      puVar8 = puVar8 + 4;
    } while (puVar16 != puVar14);
    iVar12 = *(int *)(param_1 + 4);
    break;
  case 0x26930:
    pcVar34 = fread;
LAB_00026580:
    (*pcVar34)(puVar16,1,0x30,*(undefined4 *)(param_1 + 4));
    goto LAB_00026598;
  case 0x26938:
    paVar19 = *(apu **)(this + 8);
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x30;
    puVar16 = (undefined4 *)apu::get_stat_cpy(paVar19);
    uVar26 = *(undefined4 *)param_1;
    goto code_r0x0002695c;
  }
  *(int *)(param_1 + 4) = iVar12 + 0x30;
LAB_00026598:
  puVar16 = (undefined4 *)apu::get_stat_cpy(*(apu **)(this + 8));
  uVar26 = *(undefined4 *)param_1;
code_r0x0002695c:
  switch(uVar26) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0xec;
    memset(piVar27,0,0x100);
    goto switchD_0002662c_caseD_0;
  case 1:
    puVar8 = puVar16;
    puVar14 = *(undefined4 **)(param_1 + 4);
    do {
      puVar15 = puVar14;
      puVar10 = puVar8;
      uVar28 = puVar10[1];
      uVar22 = puVar10[2];
      uVar26 = puVar10[3];
      puVar8 = puVar10 + 4;
      *puVar15 = *puVar10;
      puVar15[1] = uVar28;
      puVar15[2] = uVar22;
      puVar15[3] = uVar26;
      puVar14 = puVar15 + 4;
    } while (puVar8 != puVar16 + 0x38);
    uVar22 = puVar10[5];
    uVar26 = puVar10[6];
    puVar15[4] = *puVar8;
    puVar15[5] = uVar22;
    puVar15[6] = uVar26;
    uVar26 = *(undefined4 *)param_1;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 2:
    puVar14 = *(undefined4 **)(param_1 + 4);
    puVar8 = puVar14;
    do {
      puVar15 = puVar8;
      puVar10 = puVar16;
      uVar28 = puVar15[1];
      uVar22 = puVar15[2];
      uVar26 = puVar15[3];
      puVar8 = puVar15 + 4;
      *puVar10 = *puVar15;
      puVar10[1] = uVar28;
      puVar10[2] = uVar22;
      puVar10[3] = uVar26;
      puVar16 = puVar10 + 4;
    } while (puVar8 != puVar14 + 0x38);
    uVar22 = puVar15[5];
    uVar26 = puVar15[6];
    puVar10[4] = *puVar8;
    puVar10[5] = uVar22;
    puVar10[6] = uVar26;
    uVar26 = *(undefined4 *)param_1;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 3:
    pcVar34 = fwrite;
    goto LAB_000265d8;
  case 4:
    pcVar34 = fread;
LAB_000265d8:
    (*pcVar34)(puVar16,1,0xec,*(undefined4 *)(param_1 + 4));
    uVar26 = *(undefined4 *)param_1;
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar26 = *(undefined4 *)param_1;
  }
  memset(piVar27,0,0x100);
  switch(uVar26) {
  case 0:
switchD_0002662c_caseD_0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x100;
    return;
  case 1:
    piVar23 = *(int **)(param_1 + 4);
    do {
      iVar29 = *piVar27;
      iVar24 = piVar27[1];
      iVar33 = piVar27[2];
      iVar12 = piVar27[3];
      piVar27 = piVar27 + 4;
      *piVar23 = iVar29;
      piVar23[1] = iVar24;
      piVar23[2] = iVar33;
      piVar23[3] = iVar12;
      piVar23 = piVar23 + 4;
    } while (piVar27 != local_188 + 0x40);
  case 2:
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x100;
    return;
  case 3:
    pcVar34 = fwrite;
    break;
  case 4:
    pcVar34 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    return;
  }
  (*pcVar34)(piVar27,1,0x100,*(undefined4 *)(param_1 + 4));
  return;
}



/* ===== FUNCTION serialize_firstrev @ 00027ba0 ===== */

/* gb::serialize_firstrev(serializer&) */

void __thiscall gb::serialize_firstrev(gb *this,serializer *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  bool bVar6;
  uint uVar7;
  undefined4 *puVar8;
  uint *puVar9;
  undefined4 *puVar10;
  int iVar11;
  undefined4 *puVar12;
  undefined4 *puVar13;
  uint uVar14;
  undefined4 *puVar15;
  mbc *this_00;
  uint *puVar16;
  gb *pgVar17;
  apu *paVar18;
  uint uVar19;
  void *__dest;
  uint uVar20;
  uint uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  int *piVar24;
  uint uVar25;
  uint uVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  uint *puVar29;
  void *__src;
  int iVar30;
  size_t __n;
  code *pcVar31;
  uint local_80 [16];
  int local_40 [7];
  int local_24;
  int local_20;
  undefined1 local_1c [4];
  
  iVar11 = *(int *)(this + 0xc);
  local_40[0] = 1;
  local_40[1] = 1;
  local_40[2] = 1;
  local_40[5] = 8;
  local_40[3] = 4;
  local_40[4] = 0x10;
  if (*(uint *)param_1 < 5) {
    puVar15 = (undefined4 *)(iVar11 + 0x20);
    switch(*(uint *)param_1) {
    case 0:
      piVar24 = *(int **)(param_1 + 4);
      iVar30 = *piVar24 + 4;
      *piVar24 = iVar30;
      bVar6 = *(int *)(iVar11 + 0x20) < 3;
      if (bVar6) goto switchD_00027c84_caseD_0;
      goto LAB_00027d44;
    case 1:
      **(undefined4 **)(param_1 + 4) = *puVar15;
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      iVar11 = *(int *)(this + 0xc);
      iVar30 = *(int *)(iVar11 + 0x20);
      goto joined_r0x00027c50;
    case 2:
      *puVar15 = **(undefined4 **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      goto LAB_00027c44;
    case 3:
      pcVar31 = fwrite;
      break;
    case 4:
      pcVar31 = fread;
    }
    (*pcVar31)(puVar15,1,4,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00027c44:
  iVar11 = *(int *)(this + 0xc);
  iVar30 = *(int *)(iVar11 + 0x20);
joined_r0x00027c50:
  bVar6 = iVar30 < 3;
  if (bVar6) {
    iVar30 = *(int *)this;
    puVar15 = (undefined4 *)(iVar30 + 0x12);
    switch(*(undefined4 *)param_1) {
    case 0:
switchD_00027c84_caseD_0:
      piVar24 = *(int **)(param_1 + 4);
      iVar30 = *piVar24 + 0x2000;
      *piVar24 = iVar30;
      goto LAB_0002950c;
    case 1:
      puVar10 = *(undefined4 **)(param_1 + 4);
      do {
        uVar28 = *puVar15;
        uVar27 = puVar15[1];
        uVar23 = puVar15[2];
        uVar22 = puVar15[3];
        puVar15 = puVar15 + 4;
        *puVar10 = uVar28;
        puVar10[1] = uVar27;
        puVar10[2] = uVar23;
        puVar10[3] = uVar22;
        puVar10 = puVar10 + 4;
      } while (puVar15 != (undefined4 *)(iVar30 + 0x2012));
      iVar11 = *(int *)(param_1 + 4);
      break;
    case 2:
      puVar10 = *(undefined4 **)(param_1 + 4);
      puVar12 = puVar10 + 0x800;
      do {
        uVar28 = *puVar10;
        uVar27 = puVar10[1];
        uVar23 = puVar10[2];
        uVar22 = puVar10[3];
        puVar10 = puVar10 + 4;
        *puVar15 = uVar28;
        puVar15[1] = uVar27;
        puVar15[2] = uVar23;
        puVar15[3] = uVar22;
        puVar15 = puVar15 + 4;
      } while (puVar10 != puVar12);
      iVar11 = *(int *)(param_1 + 4);
      break;
    case 3:
      pcVar31 = fwrite;
      goto LAB_00028aa4;
    case 4:
      pcVar31 = fread;
LAB_00028aa4:
      (*pcVar31)(puVar15,1,0x2000,*(undefined4 *)(param_1 + 4));
      goto LAB_00028ab8;
    default:
      puts("ERROR: invalid serializer!");
      goto LAB_00028ab8;
    }
    *(int *)(param_1 + 4) = iVar11 + 0x2000;
LAB_00028ab8:
    iVar11 = *(int *)this;
    puVar15 = (undefined4 *)(iVar11 + 0x8012);
    switch(*(undefined4 *)param_1) {
    case 0:
      piVar24 = *(int **)(param_1 + 4);
      iVar11 = *(int *)(this + 0xc);
      iVar30 = *piVar24;
LAB_0002950c:
      iVar30 = iVar30 + 0x2000;
      *piVar24 = iVar30;
      goto LAB_00027d58;
    case 1:
      puVar10 = *(undefined4 **)(param_1 + 4);
      do {
        uVar28 = *puVar15;
        uVar27 = puVar15[1];
        uVar23 = puVar15[2];
        uVar22 = puVar15[3];
        puVar15 = puVar15 + 4;
        *puVar10 = uVar28;
        puVar10[1] = uVar27;
        puVar10[2] = uVar23;
        puVar10[3] = uVar22;
        puVar10 = puVar10 + 4;
      } while (puVar15 != (undefined4 *)(iVar11 + 0xa012));
      iVar11 = *(int *)(param_1 + 4);
      break;
    case 2:
      puVar10 = *(undefined4 **)(param_1 + 4);
      puVar12 = puVar10 + 0x800;
      do {
        uVar28 = *puVar10;
        uVar27 = puVar10[1];
        uVar23 = puVar10[2];
        uVar22 = puVar10[3];
        puVar10 = puVar10 + 4;
        *puVar15 = uVar28;
        puVar15[1] = uVar27;
        puVar15[2] = uVar23;
        puVar15[3] = uVar22;
        puVar15 = puVar15 + 4;
      } while (puVar10 != puVar12);
      iVar11 = *(int *)(param_1 + 4);
      break;
    case 3:
      pcVar31 = fwrite;
      goto LAB_00028af4;
    case 4:
      pcVar31 = fread;
LAB_00028af4:
      (*pcVar31)(puVar15,1,0x2000,*(undefined4 *)(param_1 + 4));
      goto LAB_00028b08;
    default:
      goto LAB_000294cc;
    }
    *(int *)(param_1 + 4) = iVar11 + 0x2000;
    goto LAB_00028b08;
  }
  iVar30 = *(int *)this;
  puVar15 = (undefined4 *)(iVar30 + 0x12);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar30 = *piVar24;
LAB_00027d44:
    iVar30 = iVar30 + 0x8000;
    *piVar24 = iVar30;
    goto LAB_00027d50;
  case 1:
    puVar10 = *(undefined4 **)(param_1 + 4);
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != (undefined4 *)(iVar30 + 0x8012));
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0x2000;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00028ff0;
  case 4:
    pcVar31 = fread;
LAB_00028ff0:
    (*pcVar31)(puVar15,1,0x8000,*(undefined4 *)(param_1 + 4));
    goto LAB_00029004;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00029004;
  }
  *(int *)(param_1 + 4) = iVar11 + 0x8000;
LAB_00029004:
  iVar11 = *(int *)this;
  puVar15 = (undefined4 *)(iVar11 + 0x8012);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *(int *)(this + 0xc);
    iVar30 = *piVar24;
LAB_00027d50:
    iVar30 = iVar30 + 0x4000;
    *piVar24 = iVar30;
LAB_00027d58:
    iVar30 = local_40[*(byte *)(iVar11 + 0x1d)] * 0x2000 + iVar30;
    *piVar24 = iVar30;
    goto LAB_00027d78;
  case 1:
    puVar10 = *(undefined4 **)(param_1 + 4);
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != (undefined4 *)(iVar11 + 0xc012));
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0x1000;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00029040;
  case 4:
    pcVar31 = fread;
LAB_00029040:
    (*pcVar31)(puVar15,1,0x4000,*(undefined4 *)(param_1 + 4));
    iVar11 = *(int *)(this + 0xc);
    uVar19 = *(uint *)param_1;
    __src = *(void **)(iVar11 + 0x28);
    iVar30 = local_40[*(byte *)(iVar11 + 0x1d)];
    goto joined_r0x00029078;
  default:
LAB_000294cc:
    puts("ERROR: invalid serializer!");
    goto LAB_00028b08;
  }
  *(int *)(param_1 + 4) = iVar11 + 0x4000;
LAB_00028b08:
  iVar11 = *(int *)(this + 0xc);
  uVar19 = *(uint *)param_1;
  __src = *(void **)(iVar11 + 0x28);
  iVar30 = local_40[*(byte *)(iVar11 + 0x1d)];
joined_r0x00029078:
  if (4 < uVar19) {
    puts("ERROR: invalid serializer!");
    goto LAB_00028b70;
  }
  __n = iVar30 * 0x2000;
  switch(uVar19) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar30 = __n + *piVar24;
    *piVar24 = iVar30;
    goto LAB_00027d78;
  case 1:
    __dest = *(void **)(param_1 + 4);
    break;
  case 2:
    __dest = __src;
    __src = *(void **)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00028b58;
  case 4:
    pcVar31 = fread;
LAB_00028b58:
    (*pcVar31)(__src,1,__n,*(undefined4 *)(param_1 + 4));
LAB_00028b70:
    uVar19 = *(uint *)param_1;
    iVar11 = *(int *)this;
    goto joined_r0x00028b80;
  }
  memcpy(__dest,__src,__n);
  uVar19 = *(uint *)param_1;
  *(size_t *)(param_1 + 4) = *(int *)(param_1 + 4) + __n;
  iVar11 = *(int *)this;
joined_r0x00028b80:
  if (4 < uVar19) {
    puts("ERROR: invalid serializer!");
    uVar19 = *(uint *)param_1;
    iVar11 = *(int *)this;
    goto joined_r0x00028e10;
  }
  puVar15 = (undefined4 *)(iVar11 + 0xc092);
  switch(uVar19) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *(int *)(this + 0xc);
    iVar30 = *piVar24;
LAB_00027d78:
    iVar30 = iVar30 + 0xa0;
    *piVar24 = iVar30;
    goto LAB_00027d80;
  case 1:
    puVar10 = *(undefined4 **)(param_1 + 4);
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != (undefined4 *)(iVar11 + 0xc132));
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0x28;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00028bac;
  case 4:
    pcVar31 = fread;
LAB_00028bac:
    (*pcVar31)(puVar15,1,0xa0,*(undefined4 *)(param_1 + 4));
    goto LAB_00028bc0;
  }
  *(int *)(param_1 + 4) = iVar11 + 0xa0;
LAB_00028bc0:
  uVar19 = *(uint *)param_1;
  iVar11 = *(int *)this;
joined_r0x00028e10:
  if (4 < uVar19) {
    puts("ERROR: invalid serializer!");
    uVar22 = *(undefined4 *)param_1;
    iVar11 = *(int *)(this + 0xc);
    goto LAB_00027d8c;
  }
  puVar15 = (undefined4 *)(iVar11 + 0xc012);
  switch(uVar19) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *(int *)(this + 0xc);
    iVar30 = *piVar24;
LAB_00027d80:
    uVar22 = 0;
    *piVar24 = iVar30 + 0x80;
    goto LAB_00027d8c;
  case 1:
    puVar10 = *(undefined4 **)(param_1 + 4);
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != (undefined4 *)(iVar11 + 0xc092));
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0x20;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00028bfc;
  case 4:
    pcVar31 = fread;
LAB_00028bfc:
    (*pcVar31)(puVar15,1,0x80,*(undefined4 *)(param_1 + 4));
    uVar22 = *(undefined4 *)param_1;
    iVar11 = *(int *)(this + 0xc);
    goto LAB_00027d8c;
  }
  uVar22 = *(undefined4 *)param_1;
  *(int *)(param_1 + 4) = iVar11 + 0x80;
  iVar11 = *(int *)(this + 0xc);
LAB_00027d8c:
  this_00 = *(mbc **)(this + 0x10);
  local_20 = *(int *)this_00 - *(int *)(iVar11 + 0x2c);
  uVar7 = *(int *)(this_00 + 4) - *(int *)(iVar11 + 0x28);
  uVar19 = uVar7 + 0x1fff;
  puVar29 = (uint *)(uint)((int)uVar7 < 0);
  if (local_20 < 0) {
    local_20 = local_20 + 0x3fff;
  }
  if (puVar29 != (uint *)0x0) {
    uVar7 = uVar19;
  }
  local_20 = local_20 >> 0xe;
  local_24 = (int)uVar7 >> 0xd;
  switch(uVar22) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *piVar24;
    *piVar24 = iVar11 + 4;
    *piVar24 = iVar11 + 8;
    goto LAB_00027e60;
  case 1:
    **(int **)(param_1 + 4) = local_20;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    uVar7 = *(uint *)param_1;
    if (uVar7 < 5) goto LAB_00027e24;
    goto LAB_00028a4c;
  case 2:
    piVar24 = *(int **)(param_1 + 4) + 1;
    local_20 = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = piVar24;
    goto LAB_00028a08;
  case 3:
    pcVar31 = fwrite;
    break;
  case 4:
    pcVar31 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    goto LAB_00027e14;
  }
  uVar19 = *(uint *)(param_1 + 4);
  (*pcVar31)(&local_20,1,4);
LAB_00027e14:
  uVar7 = *(uint *)param_1;
  if (4 < uVar7) {
LAB_00028a4c:
    puts("ERROR: invalid serializer!");
    this_00 = *(mbc **)(this + 0x10);
    goto LAB_00027e60;
  }
LAB_00027e24:
  switch((&switchD_00027e38::switchdataD_00035a80)[uVar7] + 0x50250 & 0xfffffffe) {
  case 0x27e40:
    pcVar31 = fwrite;
    goto LAB_00027e44;
  case 0x289c4:
    **(int **)(param_1 + 4) = local_24;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    this_00 = *(mbc **)(this + 0x10);
    break;
  case 0x289e8:
    pcVar31 = fread;
LAB_00027e44:
    uVar19 = *(uint *)(param_1 + 4);
    (*pcVar31)(&local_24,1,4);
    this_00 = *(mbc **)(this + 0x10);
    break;
  case 0x2959c:
    piVar24 = *(int **)(param_1 + 4);
    this_00 = *(mbc **)(this + 0x10);
LAB_00028a08:
    local_24 = *piVar24;
    *(int **)(param_1 + 4) = piVar24 + 1;
    break;
  case 0x295a8:
    this_00 = *(mbc **)(this + 0x10);
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
  }
LAB_00027e60:
  mbc::set_page(this_00,local_20,local_24);
  cpu::save_state(*(cpu **)this,(int *)local_80);
  cpu::save_state_ex(*(cpu **)this,(int *)(local_80 + 8));
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x30;
    break;
  case 1:
    puVar9 = *(uint **)(param_1 + 4);
    puVar29 = local_80 + 0xc;
    puVar16 = local_80;
    do {
      uVar19 = *puVar16;
      uVar26 = puVar16[1];
      uVar25 = puVar16[2];
      uVar7 = puVar16[3];
      puVar16 = puVar16 + 4;
      *puVar9 = uVar19;
      puVar9[1] = uVar26;
      puVar9[2] = uVar25;
      puVar9[3] = uVar7;
      puVar9 = puVar9 + 4;
    } while (puVar16 != puVar29);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x30;
    break;
  case 2:
    puVar9 = *(uint **)(param_1 + 4);
    puVar29 = puVar9 + 0xc;
    puVar16 = local_80;
    do {
      uVar19 = *puVar9;
      uVar26 = puVar9[1];
      uVar25 = puVar9[2];
      uVar7 = puVar9[3];
      puVar9 = puVar9 + 4;
      *puVar16 = uVar19;
      puVar16[1] = uVar26;
      puVar16[2] = uVar25;
      puVar16[3] = uVar7;
      puVar16 = puVar16 + 4;
    } while (puVar9 != puVar29);
    *(uint **)(param_1 + 4) = puVar9;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00027ed0;
  case 4:
    pcVar31 = fread;
LAB_00027ed0:
    uVar19 = *(uint *)(param_1 + 4);
    (*pcVar31)(local_80,1,0x30);
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  cpu::restore_state(*(cpu **)this,(int *)local_80);
  cpu::restore_state_ex(*(cpu **)this,(int *)(local_80 + 8));
  uVar7 = *(uint *)param_1;
  uVar25 = *(uint *)this;
  puVar16 = (uint *)(uVar25 + 4);
  switch(uVar7) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *piVar24 + 0xe;
    *piVar24 = iVar11;
    goto LAB_000288d0;
  case 1:
    puVar9 = *(uint **)(param_1 + 4);
    uVar26 = *puVar16;
    uVar21 = *(uint *)(uVar25 + 8);
    uVar7 = *(uint *)(uVar25 + 0xc);
    *puVar9 = uVar26;
    puVar9[1] = uVar21;
    puVar9[2] = uVar7;
    *(undefined1 *)(puVar9 + 3) = *(undefined1 *)(uVar25 + 0x10);
    *(undefined1 *)((int)puVar9 + 0xd) = *(undefined1 *)(uVar25 + 0x11);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xe;
    break;
  case 2:
    puVar9 = *(uint **)(param_1 + 4);
    uVar19 = *puVar9;
    uVar26 = puVar9[1];
    uVar7 = puVar9[2];
    *puVar16 = uVar19;
    *(uint *)(uVar25 + 8) = uVar26;
    *(uint *)(uVar25 + 0xc) = uVar7;
    *(char *)(uVar25 + 0x10) = (char)puVar9[3];
    *(undefined1 *)(uVar25 + 0x11) = *(undefined1 *)((int)puVar9 + 0xd);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xe;
    uVar7 = *(uint *)param_1;
    uVar26 = uVar25;
    goto joined_r0x0002884c;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00027f48;
  case 4:
    pcVar31 = fread;
LAB_00027f48:
    uVar19 = *(uint *)(param_1 + 4);
    uVar26 = 0xe;
    (*pcVar31)(puVar16,1);
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar26 = uVar25;
  }
  uVar7 = *(uint *)param_1;
joined_r0x0002884c:
  if (uVar7 < 5) {
    pgVar17 = this + 0x20;
    switch(uVar7) {
    case 0:
      piVar24 = *(int **)(param_1 + 4);
      iVar11 = *piVar24;
LAB_000288d0:
      *piVar24 = iVar11 + 0x15;
      if (bVar6) goto LAB_00027fac;
      iVar11 = iVar11 + 0x22;
      *piVar24 = iVar11;
      goto LAB_000288e4;
    case 1:
      uVar7 = (uint)(this + 0x23) & 3;
      uVar25 = (uint)(this + 0x27) & 3;
      iVar11 = *(int *)(this + 0x27 + -uVar25);
      puVar16 = *(uint **)(param_1 + 4);
      uVar21 = (uint)(this + 0x2b) & 3;
      iVar30 = *(int *)(this + 0x2b + -uVar21);
      uVar1 = (uint)pgVar17 & 3;
      uVar2 = (uint)(this + 0x24) & 3;
      uVar4 = *(uint *)(this + 0x24 + -uVar2);
      uVar20 = *(uint *)(this + 0x2c);
      uVar14 = *(uint *)(this + 0x30);
      uVar3 = (uint)(this + 0x28) & 3;
      uVar5 = *(uint *)(this + 0x28 + -uVar3);
      *puVar16 = (*(int *)(this + 0x23 + -uVar7) << (3 - uVar7) * 8 |
                 (uint)puVar29 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar1) * 8 |
                 *(uint *)(pgVar17 + -uVar1) >> uVar1 * 8;
      puVar16[1] = (iVar11 << (3 - uVar25) * 8 | uVar19 & 0xffffffffU >> (uVar25 + 1) * 8) &
                   -1 << (4 - uVar2) * 8 | uVar4 >> uVar2 * 8;
      puVar16[2] = (iVar30 << (3 - uVar21) * 8 | uVar26 & 0xffffffffU >> (uVar21 + 1) * 8) &
                   -1 << (4 - uVar3) * 8 | uVar5 >> uVar3 * 8;
      puVar16[3] = uVar20;
      puVar16[4] = uVar14;
      *(gb *)(puVar16 + 5) = this[0x34];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x15;
      goto LAB_00027fa4;
    case 2:
      puVar15 = *(undefined4 **)(param_1 + 4);
      uVar19 = (int)puVar15 + 0xbU & 3;
      iVar11 = *(int *)(((int)puVar15 + 0xbU) - uVar19);
      uVar27 = puVar15[1];
      uVar23 = puVar15[3];
      uVar22 = puVar15[4];
      uVar7 = (uint)(puVar15 + 2) & 3;
      uVar25 = *(uint *)((int)(puVar15 + 2) - uVar7);
      *(undefined4 *)pgVar17 = *puVar15;
      *(undefined4 *)(this + 0x24) = uVar27;
      *(uint *)(this + 0x28) =
           (iVar11 << (3 - uVar19) * 8 | uVar26 & 0xffffffffU >> (uVar19 + 1) * 8) &
           -1 << (4 - uVar7) * 8 | uVar25 >> uVar7 * 8;
      *(undefined4 *)(this + 0x2c) = uVar23;
      *(undefined4 *)(this + 0x30) = uVar22;
      this[0x34] = *(gb *)(puVar15 + 5);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0x15;
      uVar7 = *(uint *)param_1;
      goto joined_r0x0002874c;
    case 3:
      pcVar31 = fwrite;
      break;
    case 4:
      pcVar31 = fread;
    }
    (*pcVar31)(pgVar17,1,0x15,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_00027fa4:
  uVar7 = *(uint *)param_1;
joined_r0x0002874c:
  if (bVar6) goto LAB_00027fac;
  pgVar17 = this + 0x35;
  switch(uVar7) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *piVar24 + 0xd;
    *piVar24 = iVar11;
LAB_000288e4:
    iVar11 = iVar11 + 0x80;
    *piVar24 = iVar11;
    goto LAB_000288ec;
  case 1:
    puVar15 = *(undefined4 **)(param_1 + 4);
    uVar23 = *(undefined4 *)(this + 0x39);
    uVar22 = *(undefined4 *)(this + 0x3d);
    *puVar15 = *(undefined4 *)pgVar17;
    puVar15[1] = uVar23;
    puVar15[2] = uVar22;
    *(gb *)(puVar15 + 3) = this[0x41];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xd;
    break;
  case 2:
    puVar15 = *(undefined4 **)(param_1 + 4);
    uVar23 = puVar15[1];
    uVar22 = puVar15[2];
    *(undefined4 *)pgVar17 = *puVar15;
    *(undefined4 *)(this + 0x39) = uVar23;
    *(undefined4 *)(this + 0x3d) = uVar22;
    this[0x41] = *(gb *)(puVar15 + 3);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xd;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00029238;
  case 4:
    pcVar31 = fread;
LAB_00029238:
    (*pcVar31)(pgVar17,1,0xd,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  iVar11 = *(int *)(this + 4);
  puVar15 = (undefined4 *)(iVar11 + 0x18);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar24 = *(int **)(param_1 + 4);
    iVar11 = *piVar24 + 0x80;
    *piVar24 = iVar11;
LAB_000288ec:
    *piVar24 = iVar11 + 1;
    goto LAB_00027ff8;
  case 1:
    puVar10 = *(undefined4 **)(param_1 + 4);
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != (undefined4 *)(iVar11 + 0x98));
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 2:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0x20;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00029284;
  case 4:
    pcVar31 = fread;
LAB_00029284:
    (*pcVar31)(puVar15,1,0x80,*(undefined4 *)(param_1 + 4));
    uVar7 = *(uint *)param_1;
    iVar11 = *(int *)this;
    goto joined_r0x000292a8;
  default:
    puts("ERROR: invalid serializer!");
    uVar7 = *(uint *)param_1;
    goto LAB_00027fac;
  }
  uVar7 = *(uint *)param_1;
  *(int *)(param_1 + 4) = iVar11 + 0x80;
LAB_00027fac:
  iVar11 = *(int *)this;
joined_r0x000292a8:
  if (4 < uVar7) {
    puts("ERROR: invalid serializer!");
    goto LAB_00027ff8;
  }
  switch(uVar7) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 1;
    break;
  case 1:
    **(undefined1 **)(param_1 + 4) = *(undefined1 *)(iVar11 + 0xc77c);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    break;
  case 2:
    *(undefined1 *)(iVar11 + 0xc77c) = **(undefined1 **)(param_1 + 4);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00027fe4;
  case 4:
    pcVar31 = fread;
LAB_00027fe4:
    (*pcVar31)(iVar11 + 0xc77c,1,1,*(undefined4 *)(param_1 + 4));
  }
LAB_00027ff8:
  local_40[6] = mbc::get_state(*(mbc **)(this + 0x10));
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 1:
    **(int **)(param_1 + 4) = local_40[6];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    break;
  case 2:
    local_40[6] = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = *(int **)(param_1 + 4) + 1;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_0002803c;
  case 4:
    pcVar31 = fread;
LAB_0002803c:
    (*pcVar31)(local_40 + 6,1,4,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  mbc::set_state(*(mbc **)(this + 0x10),local_40[6]);
  iVar11 = *(int *)(this + 0x10);
  local_1c[0] = *(undefined1 *)(iVar11 + 0x11);
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 1;
    break;
  case 1:
    **(undefined1 **)(param_1 + 4) = local_1c[0];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    iVar11 = *(int *)(this + 0x10);
    break;
  case 2:
    local_1c[0] = **(undefined1 **)(param_1 + 4);
    *(undefined1 **)(param_1 + 4) = *(undefined1 **)(param_1 + 4) + 1;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_000280a4;
  case 4:
    pcVar31 = fread;
LAB_000280a4:
    (*pcVar31)(local_1c,1,1,*(undefined4 *)(param_1 + 4));
    iVar11 = *(int *)(this + 0x10);
    break;
  default:
    puts("ERROR: invalid serializer!");
    iVar11 = *(int *)(this + 0x10);
  }
  paVar18 = *(apu **)(this + 8);
  *(undefined1 *)(iVar11 + 0x11) = local_1c[0];
  puVar15 = (undefined4 *)apu::get_stat(paVar18);
  switch(*(undefined4 *)param_1) {
  case 0:
    paVar18 = *(apu **)(this + 8);
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0xec;
    puVar15 = (undefined4 *)apu::get_mem(paVar18);
    uVar19 = *(uint *)param_1;
    if (uVar19 < 5) goto LAB_0002813c;
    goto LAB_00028560;
  case 1:
    puVar10 = puVar15;
    puVar12 = *(undefined4 **)(param_1 + 4);
    do {
      puVar13 = puVar12;
      puVar8 = puVar10;
      uVar27 = puVar8[1];
      uVar23 = puVar8[2];
      uVar22 = puVar8[3];
      puVar10 = puVar8 + 4;
      *puVar13 = *puVar8;
      puVar13[1] = uVar27;
      puVar13[2] = uVar23;
      puVar13[3] = uVar22;
      puVar12 = puVar13 + 4;
    } while (puVar10 != puVar15 + 0x38);
    uVar23 = puVar8[5];
    uVar22 = puVar8[6];
    puVar13[4] = *puVar10;
    puVar13[5] = uVar23;
    puVar13[6] = uVar22;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 2:
    puVar12 = *(undefined4 **)(param_1 + 4);
    puVar10 = puVar12;
    do {
      puVar13 = puVar10;
      puVar8 = puVar15;
      uVar27 = puVar13[1];
      uVar23 = puVar13[2];
      uVar22 = puVar13[3];
      puVar10 = puVar13 + 4;
      *puVar8 = *puVar13;
      puVar8[1] = uVar27;
      puVar8[2] = uVar23;
      puVar8[3] = uVar22;
      puVar15 = puVar8 + 4;
    } while (puVar10 != puVar12 + 0x38);
    uVar23 = puVar13[5];
    uVar22 = puVar13[6];
    puVar8[4] = *puVar10;
    puVar8[5] = uVar23;
    puVar8[6] = uVar22;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    break;
  case 3:
    pcVar31 = fwrite;
    goto LAB_00028108;
  case 4:
    pcVar31 = fread;
LAB_00028108:
    (*pcVar31)(puVar15,1,0xec,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  puVar15 = (undefined4 *)apu::get_mem(*(apu **)(this + 8));
  uVar19 = *(uint *)param_1;
  if (4 < uVar19) {
LAB_00028560:
    puts("ERROR: invalid serializer!");
    puVar15 = (undefined4 *)apu::get_stat_cpy(*(apu **)(this + 8));
    uVar22 = *(undefined4 *)param_1;
    goto code_r0x0002818c;
  }
LAB_0002813c:
  switch((&switchD_00028154::switchdataD_00035b20)[uVar19] + 0x50250 & 0xfffffffe) {
  case 0x2815c:
    pcVar31 = fwrite;
    goto LAB_00028160;
  case 0x2834c:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar10 + 0xc;
    do {
      uVar28 = *puVar10;
      uVar27 = puVar10[1];
      uVar23 = puVar10[2];
      uVar22 = puVar10[3];
      puVar10 = puVar10 + 4;
      *puVar15 = uVar28;
      puVar15[1] = uVar27;
      puVar15[2] = uVar23;
      puVar15[3] = uVar22;
      puVar15 = puVar15 + 4;
    } while (puVar10 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 0x283b0:
    puVar10 = *(undefined4 **)(param_1 + 4);
    puVar12 = puVar15 + 0xc;
    do {
      uVar28 = *puVar15;
      uVar27 = puVar15[1];
      uVar23 = puVar15[2];
      uVar22 = puVar15[3];
      puVar15 = puVar15 + 4;
      *puVar10 = uVar28;
      puVar10[1] = uVar27;
      puVar10[2] = uVar23;
      puVar10[3] = uVar22;
      puVar10 = puVar10 + 4;
    } while (puVar15 != puVar12);
    iVar11 = *(int *)(param_1 + 4);
    break;
  case 0x285c0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0x30;
    goto LAB_00028178;
  case 0x285d4:
    pcVar31 = fread;
LAB_00028160:
    (*pcVar31)(puVar15,1,0x30,*(undefined4 *)(param_1 + 4));
    goto LAB_00028178;
  }
  *(int *)(param_1 + 4) = iVar11 + 0x30;
LAB_00028178:
  puVar15 = (undefined4 *)apu::get_stat_cpy(*(apu **)(this + 8));
  uVar22 = *(undefined4 *)param_1;
code_r0x0002818c:
  switch(uVar22) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 0xec;
    return;
  case 1:
    puVar10 = puVar15;
    puVar12 = *(undefined4 **)(param_1 + 4);
    do {
      puVar13 = puVar12;
      puVar8 = puVar10;
      uVar27 = puVar8[1];
      uVar23 = puVar8[2];
      uVar22 = puVar8[3];
      puVar10 = puVar8 + 4;
      *puVar13 = *puVar8;
      puVar13[1] = uVar27;
      puVar13[2] = uVar23;
      puVar13[3] = uVar22;
      puVar12 = puVar13 + 4;
    } while (puVar10 != puVar15 + 0x38);
    uVar23 = puVar8[5];
    uVar22 = puVar8[6];
    puVar13[4] = *puVar10;
    puVar13[5] = uVar23;
    puVar13[6] = uVar22;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    return;
  case 2:
    puVar12 = *(undefined4 **)(param_1 + 4);
    puVar10 = puVar12;
    do {
      puVar13 = puVar10;
      puVar8 = puVar15;
      uVar27 = puVar13[1];
      uVar23 = puVar13[2];
      uVar22 = puVar13[3];
      puVar10 = puVar13 + 4;
      *puVar8 = *puVar13;
      puVar8[1] = uVar27;
      puVar8[2] = uVar23;
      puVar8[3] = uVar22;
      puVar15 = puVar8 + 4;
    } while (puVar10 != puVar12 + 0x38);
    uVar23 = puVar13[5];
    uVar22 = puVar13[6];
    puVar8[4] = *puVar10;
    puVar8[5] = uVar23;
    puVar8[6] = uVar22;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xec;
    return;
  case 3:
    pcVar31 = fwrite;
    break;
  case 4:
    pcVar31 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    return;
  }
  (*pcVar31)(puVar15,1,0xec,*(undefined4 *)(param_1 + 4));
  return;
}



/* ===== FUNCTION serialize @ 00029638 ===== */

/* gb::serialize(serializer&) */

void gb::serialize(serializer *param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint *puVar11;
  undefined4 *puVar12;
  int *piVar13;
  uint uVar14;
  undefined4 uVar15;
  serializer *psVar16;
  serializer *in_a1;
  uint uVar17;
  undefined4 uVar18;
  uint in_a2;
  uint in_a3;
  undefined4 uVar19;
  uint in_t0;
  code *pcVar20;
  
  if (*(uint *)in_a1 < 5) {
    psVar16 = param_1 + 0x20;
    switch(*(uint *)in_a1) {
    case 0:
      piVar13 = *(int **)(in_a1 + 4);
      iVar10 = *piVar13;
      *piVar13 = iVar10 + 0x15;
      *piVar13 = iVar10 + 0x22;
      goto LAB_000296ec;
    case 1:
      uVar9 = (uint)(param_1 + 0x23) & 3;
      uVar5 = (uint)(param_1 + 0x27) & 3;
      iVar10 = *(int *)(param_1 + 0x27 + -uVar5);
      puVar11 = *(uint **)(in_a1 + 4);
      uVar8 = (uint)(param_1 + 0x2b) & 3;
      iVar1 = *(int *)(param_1 + 0x2b + -uVar8);
      uVar2 = (uint)psVar16 & 3;
      uVar3 = (uint)(param_1 + 0x24) & 3;
      uVar6 = *(uint *)(param_1 + 0x24 + -uVar3);
      uVar17 = *(uint *)(param_1 + 0x2c);
      uVar14 = *(uint *)(param_1 + 0x30);
      uVar4 = (uint)(param_1 + 0x28) & 3;
      uVar7 = *(uint *)(param_1 + 0x28 + -uVar4);
      *puVar11 = (*(int *)(param_1 + 0x23 + -uVar9) << (3 - uVar9) * 8 |
                 in_t0 & 0xffffffffU >> (uVar9 + 1) * 8) & -1 << (4 - uVar2) * 8 |
                 *(uint *)(psVar16 + -uVar2) >> uVar2 * 8;
      puVar11[1] = (iVar10 << (3 - uVar5) * 8 | in_a3 & 0xffffffffU >> (uVar5 + 1) * 8) &
                   -1 << (4 - uVar3) * 8 | uVar6 >> uVar3 * 8;
      puVar11[2] = (iVar1 << (3 - uVar8) * 8 | in_a2 & 0xffffffffU >> (uVar8 + 1) * 8) &
                   -1 << (4 - uVar4) * 8 | uVar7 >> uVar4 * 8;
      puVar11[3] = uVar17;
      puVar11[4] = uVar14;
      *(serializer *)(puVar11 + 5) = param_1[0x34];
      *(int *)(in_a1 + 4) = *(int *)(in_a1 + 4) + 0x15;
      uVar9 = *(uint *)in_a1;
      goto joined_r0x000296ac;
    case 2:
      puVar12 = *(undefined4 **)(in_a1 + 4);
      uVar9 = (int)puVar12 + 0xbU & 3;
      iVar10 = *(int *)(((int)puVar12 + 0xbU) - uVar9);
      uVar19 = puVar12[1];
      uVar18 = puVar12[3];
      uVar15 = puVar12[4];
      uVar5 = (uint)(puVar12 + 2) & 3;
      uVar8 = *(uint *)((int)(puVar12 + 2) - uVar5);
      *(undefined4 *)psVar16 = *puVar12;
      *(undefined4 *)(param_1 + 0x24) = uVar19;
      *(uint *)(param_1 + 0x28) =
           (iVar10 << (3 - uVar9) * 8 | in_a2 & 0xffffffffU >> (uVar9 + 1) * 8) &
           -1 << (4 - uVar5) * 8 | uVar8 >> uVar5 * 8;
      *(undefined4 *)(param_1 + 0x2c) = uVar18;
      *(undefined4 *)(param_1 + 0x30) = uVar15;
      param_1[0x34] = *(serializer *)(puVar12 + 5);
      *(int *)(in_a1 + 4) = *(int *)(in_a1 + 4) + 0x15;
      goto LAB_000296a4;
    case 3:
      pcVar20 = fwrite;
      break;
    case 4:
      pcVar20 = fread;
    }
    (*pcVar20)(psVar16,1,0x15,*(undefined4 *)(in_a1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_000296a4:
  uVar9 = *(uint *)in_a1;
joined_r0x000296ac:
  if (uVar9 < 5) {
                    /* WARNING: Could not recover jumptable at 0x000296cc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter + *(int *)(&DAT_00035be8 + uVar9 * 4) + 0x7ccc))
              (param_1 + 0x35);
    return;
  }
  puts("ERROR: invalid serializer!");
LAB_000296ec:
  rom::serialize(*(serializer **)(param_1 + 0xc));
  cpu::serialize(*(serializer **)param_1);
  mbc::serialize(*(mbc **)(param_1 + 0x10),in_a1);
  lcd::serialize(*(lcd **)(param_1 + 4),in_a1);
                    /* WARNING: Could not recover jumptable at 0x00029754. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  apu::serialize(*(apu **)(param_1 + 8),in_a1);
  return;
}



/* ===== FUNCTION get_state_size @ 00029948 ===== */

/* gb::get_state_size() */

undefined4 __thiscall gb::get_state_size(gb *this)

{
  serialize((serializer *)this);
  return 0;
}



/* ===== FUNCTION save_state_mem @ 0002998c ===== */

/* gb::save_state_mem(void*) */

void gb::save_state_mem(void *param_1)

{
  serialize(param_1);
  return;
}



/* ===== FUNCTION restore_state_mem @ 000299c8 ===== */

/* gb::restore_state_mem(void*) */

void gb::restore_state_mem(void *param_1)

{
  serialize(param_1);
  return;
}



/* ===== FUNCTION save_state @ 00029a04 ===== */

/* gb::save_state(_IO_FILE*) */

void gb::save_state(_IO_FILE *param_1)

{
  serialize((serializer *)param_1);
  return;
}



/* ===== FUNCTION restore_state @ 00029a40 ===== */

/* gb::restore_state(_IO_FILE*) */

void gb::restore_state(_IO_FILE *param_1)

{
  serialize((serializer *)param_1);
  return;
}



/* ===== FUNCTION refresh_pal @ 00029a7c ===== */

/* gb::refresh_pal() */

void __thiscall gb::refresh_pal(gb *this)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  uVar5 = 0;
  do {
    iVar4 = *(int *)(this + 4);
    iVar2 = (uVar5 & 3) * 2;
    iVar3 = (((int)uVar5 >> 2) + 0x13) * 8;
    uVar5 = uVar5 + 1;
    uVar1 = (**(code **)(**(int **)(this + 0x14) + 0x10))
                      (*(int **)(this + 0x14),*(undefined2 *)(iVar4 + iVar2 + iVar3 + -0x80));
    *(undefined2 *)(iVar4 + iVar2 + iVar3) = uVar1;
  } while (uVar5 != 0x40);
  return;
}



/* ===== FUNCTION run @ 00029b04 ===== */

/* gb::run() */

void __thiscall gb::run(gb *this)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  gb gVar15;
  int iVar16;
  uint uVar17;
  uint *puVar18;
  cpu *pcVar19;
  cpu *this_00;
  gb gVar20;
  int *piVar21;
  uint uVar22;
  
  if (*(char *)(*(int *)(this + 0xc) + 0x30) == '\0') {
    return;
  }
  if ((char)this[0x28] < '\0') {
    gVar15 = this[0x29];
    gVar20 = (gb)((byte)gVar15 & 0xf8);
    this[0x29] = gVar20;
    piVar21 = (int *)0x9a;
    uVar22 = (((byte)this[0x2c] + 1) / 0x9a) * 0x9a;
    uVar17 = ((byte)this[0x2c] + 1) % 0x9a;
    this[0x2c] = SUB41(uVar17,0);
    if (((byte)this[0x2d] == uVar17) &&
       (this[0x29] = (gb)((byte)gVar20 | 4), ((byte)gVar15 & 0x40) != 0)) {
      cpu::irq(*(cpu **)this,2);
      uVar17 = (uint)(byte)this[0x2c];
    }
    if (uVar17 == 0) {
      (**(code **)(**(int **)(this + 0x14) + 4))(*(int **)(this + 0x14));
      if (*(int *)(this + 0x13794) < *(int *)(this + 0x1378c)) {
        *(int *)(this + 0x13794) = *(int *)(this + 0x13794) + 1;
      }
      else {
        piVar21 = (int *)0x90;
        (**(code **)(**(int **)(this + 0x14) + 8))
                  (*(int **)(this + 0x14),this + 0x682,0xa0,0x90,0x10);
        *(undefined4 *)(this + 0x13794) = 0;
      }
      *(undefined4 *)(*(int *)(this + 4) + 0x39c) = 9;
      *(undefined4 *)(this + 0x1378c) = *(undefined4 *)(this + 0x13790);
      uVar17 = (uint)(byte)this[0x2c];
    }
    if (0x8f < uVar17) {
      this[0x29] = (gb)((byte)this[0x29] | 1);
      if (uVar17 == 0x90) {
        cpu::exec(*(cpu **)this,0x48);
        cpu::irq(*(cpu **)this,1);
        if (((byte)this[0x29] & 0x10) != 0) {
          cpu::irq(*(cpu **)this,2);
        }
        iVar16 = 0x178;
        this_00 = *(cpu **)this;
      }
      else {
        this_00 = *(cpu **)this;
        if (uVar17 == 0x99) {
          cpu::exec(this_00,0x50);
          this[0x2c] = (gb)0x0;
          cpu::exec(*(cpu **)this,0x178);
          this[0x2c] = (gb)0x99;
          return;
        }
        iVar16 = 0x1c8;
      }
      goto LAB_00029b7c;
    }
    gVar15 = this[0x29];
    this[0x29] = (gb)((byte)gVar15 | 2);
    if (((byte)gVar15 & 0x20) != 0) {
      cpu::irq(*(cpu **)this,2);
    }
    cpu::exec(*(cpu **)this,0x50);
    this[0x29] = (gb)((byte)this[0x29] | 3);
    cpu::exec(*(cpu **)this,0xa9);
    this_00 = *(cpu **)this;
    if (this_00[0xc77f] == (cpu)0x0) {
      gVar15 = (gb)((byte)this[0x29] & 0xfc);
      this[0x29] = gVar15;
      if (*(int *)(this + 0x1378c) <= *(int *)(this + 0x13794)) {
        lcd::render(*(lcd **)(this + 4),this + 0x682,SUB14(this[0x2c],0));
        gVar15 = this[0x29];
        this_00 = *(cpu **)this;
      }
      if (((byte)gVar15 & 8) == 0) {
        iVar16 = 0xcf;
      }
      else {
        cpu::irq(this_00,2);
        iVar16 = 0xcf;
        this_00 = *(cpu **)this;
      }
      goto LAB_00029b7c;
    }
    if (this_00[0xc794] == (cpu)0x0) {
      uVar17 = *(uint *)(this_00 + 0xc784);
      pcVar19 = *(cpu **)(this_00 + 0xc7a0);
      iVar16 = *(int *)(this_00 + 0xc7a4);
    }
    else {
      uVar17 = *(uint *)(this_00 + 0xc784);
      iVar16 = *(int *)(this_00 + 0xc15c);
      *(int *)(this_00 + 0xc7a4) = iVar16;
      if ((int)uVar17 < 0x4000) {
        piVar21 = *(int **)(this + 0xc);
        pcVar19 = (cpu *)piVar21[0xb];
        *(cpu **)(this_00 + 0xc7a0) = pcVar19;
      }
      else {
        piVar21 = (int *)0xffff0000;
        if ((int)uVar17 < 0x8000) {
          piVar21 = *(int **)(this + 0x10);
          pcVar19 = (cpu *)*piVar21;
          *(cpu **)(this_00 + 0xc7a0) = pcVar19;
        }
        else {
          uVar22 = 0xffff6000;
          if (uVar17 - 0xa000 < 0x2000) {
            pcVar19 = (cpu *)(*(int *)(*(int *)(this + 0x10) + 4) + -0xa000);
            *(cpu **)(this_00 + 0xc7a0) = pcVar19;
          }
          else if (uVar17 - 0xc000 < 0x1000) {
            pcVar19 = this_00 + -0xbfee;
            *(cpu **)(this_00 + 0xc7a0) = pcVar19;
          }
          else {
            piVar21 = (int *)0xffff3000;
            if (uVar17 - 0xd000 < 0x1000) {
              pcVar19 = (cpu *)(*(int *)(this_00 + 0xc160) + -0xd000);
              *(cpu **)(this_00 + 0xc7a0) = pcVar19;
            }
            else {
              pcVar19 = (cpu *)0x0;
              *(undefined4 *)(this_00 + 0xc7a0) = 0;
            }
          }
        }
      }
      this_00[0xc794] = (cpu)0x0;
    }
    pcVar19 = pcVar19 + uVar17;
    uVar1 = (uint)(pcVar19 + 3) & 3;
    uVar2 = (uint)(pcVar19 + 7) & 3;
    iVar5 = *(int *)(pcVar19 + 7 + -uVar2);
    uVar3 = (uint)(pcVar19 + 0xb) & 3;
    iVar6 = *(int *)(pcVar19 + 0xb + -uVar3);
    uVar4 = (uint)(pcVar19 + 0xf) & 3;
    iVar7 = *(int *)(pcVar19 + 0xf + -uVar4);
    puVar18 = (uint *)(iVar16 + (*(uint *)(this_00 + 0xc788) & 0x1ff0));
    uVar8 = (uint)pcVar19 & 3;
    uVar9 = (uint)(pcVar19 + 4) & 3;
    uVar12 = *(uint *)(pcVar19 + 4 + -uVar9);
    uVar10 = (uint)(pcVar19 + 8) & 3;
    uVar13 = *(uint *)(pcVar19 + 8 + -uVar10);
    uVar11 = (uint)(pcVar19 + 0xc) & 3;
    uVar14 = *(uint *)(pcVar19 + 0xc + -uVar11);
    *puVar18 = (*(int *)(pcVar19 + 3 + -uVar1) << (3 - uVar1) * 8 |
               uVar22 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar8) * 8 |
               *(uint *)(pcVar19 + -uVar8) >> uVar8 * 8;
    puVar18[1] = (iVar5 << (3 - uVar2) * 8 | (uint)piVar21 & 0xffffffffU >> (uVar2 + 1) * 8) &
                 -1 << (4 - uVar9) * 8 | uVar12 >> uVar9 * 8;
    puVar18[2] = (iVar6 << (3 - uVar3) * 8 | uVar17 & 0xffffffffU >> (uVar3 + 1) * 8) &
                 -1 << (4 - uVar10) * 8 | uVar13 >> uVar10 * 8;
    puVar18[3] = (iVar7 << (3 - uVar4) * 8 |
                 (uint)(this_00 + 0x10000) & 0xffffffffU >> (uVar4 + 1) * 8) &
                 -1 << (4 - uVar11) * 8 | uVar14 >> uVar11 * 8;
    this_00 = *(cpu **)this;
    iVar16 = *(int *)(this_00 + 0xc78c);
    *(uint *)(this_00 + 0xc784) = *(int *)(this_00 + 0xc784) + 0x10U & 0xfff0;
    *(uint *)(this_00 + 0xc788) = *(int *)(this_00 + 0xc788) + 0x10U & 0xfff0;
    *(int *)(this_00 + 0xc78c) = iVar16 + -1;
    if (iVar16 + -1 == 0) {
      this_00[0xc77f] = (cpu)0x0;
    }
    if (*(int *)(this + 0x1378c) <= *(int *)(this + 0x13794)) {
      lcd::render(*(lcd **)(this + 4),this + 0x682,SUB14(this[0x2c],0));
      this_00 = *(cpu **)this;
    }
    iVar16 = 0xcf;
    gVar15 = (gb)((byte)this[0x29] & 0xfc);
  }
  else {
    this[0x2c] = (gb)0x0;
    iVar16 = *(int *)(this + 0x13798);
    *(int *)(this + 0x13798) = iVar16 + 1;
    if (0x99 < iVar16 + 1) {
      memset(this + 0x682,0xff,0xb400);
      (**(code **)(**(int **)(this + 0x14) + 4))(*(int **)(this + 0x14));
      if (*(int *)(this + 0x13794) < *(int *)(this + 0x1378c)) {
        *(int *)(this + 0x13794) = *(int *)(this + 0x13794) + 1;
      }
      else {
        (**(code **)(**(int **)(this + 0x14) + 8))
                  (*(int **)(this + 0x14),this + 0x682,0xa0,0x90,0x10);
        *(undefined4 *)(this + 0x13794) = 0;
      }
      *(undefined4 *)(*(int *)(this + 4) + 0x39c) = 9;
      *(undefined4 *)(this + 0x13798) = 0;
    }
    iVar16 = 0x1c8;
    this_00 = *(cpu **)this;
    gVar15 = (gb)((byte)this[0x29] & 0xf8);
  }
  this[0x29] = gVar15;
LAB_00029b7c:
  cpu::exec(this_00,iVar16);
  return;
}



/* ===== FUNCTION ~lcd @ 0002a0d0 ===== */

/* lcd::~lcd() */

void __thiscall lcd::~lcd(lcd *this)

{
  return;
}



/* ===== FUNCTION set_enable @ 0002a0d8 ===== */

/* lcd::set_enable(int, bool) */

void __thiscall lcd::set_enable(lcd *this,int param_1,bool param_2)

{
  this[param_1 + 0x3a8] = (lcd)param_2;
  return;
}



/* ===== FUNCTION get_enable @ 0002a0e4 ===== */

/* lcd::get_enable(int) */

lcd __thiscall lcd::get_enable(lcd *this,int param_1)

{
  return this[param_1 + 0x3a8];
}



/* ===== FUNCTION reset @ 0002a0f0 ===== */

/* lcd::reset() */

void __thiscall lcd::reset(lcd *this)

{
  *(undefined4 *)(this + 0x39c) = 0;
  *(undefined4 *)(this + 0x3a4) = 0;
  this[0x3a8] = (lcd)0x1;
  this[0x3a9] = (lcd)0x1;
  this[0x3aa] = (lcd)0x1;
  return;
}



/* ===== FUNCTION lcd @ 0002a10c ===== */

/* lcd::lcd(gb*) */

void __thiscall lcd::lcd(lcd *this,gb *param_1)

{
  undefined2 uVar1;
  uint uVar2;
  lcd *plVar3;
  lcd *plVar4;
  byte *pbVar5;
  byte local_28 [12];
  
  local_28[0] = 0x1f;
  uVar2 = 0x1f;
  local_28[1] = 0x15;
  pbVar5 = local_28;
  local_28[3] = 0;
  *(gb **)(this + 0x3ac) = param_1;
  local_28[2] = 0xb;
  plVar3 = this;
  plVar4 = this + 8;
  while( true ) {
    pbVar5 = pbVar5 + 1;
    uVar1 = (**(code **)(**(int **)(param_1 + 0x14) + 0x10))
                      (*(int **)(param_1 + 0x14),uVar2 << 5 | (uVar2 & 0x3f) << 10 | uVar2);
    *(undefined2 *)plVar3 = uVar1;
    plVar3 = plVar3 + 2;
    *(uint *)plVar4 = uVar2 << 0x10 | uVar2 << 8 | uVar2;
    plVar4 = plVar4 + 4;
    if (this + 8 == plVar3) break;
    uVar2 = (uint)*pbVar5;
    param_1 = *(gb **)(this + 0x3ac);
  }
                    /* WARNING: Could not recover jumptable at 0x0002a1f4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  reset(this);
  return;
}



/* ===== FUNCTION bg_render @ 0002a1fc ===== */

/* lcd::bg_render(void*, int) */

void __thiscall lcd::bg_render(lcd *this,void *param_1,int param_2)

{
  undefined2 *puVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  ushort uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  lcd *plVar12;
  lcd *plVar13;
  int iVar14;
  undefined2 *puVar15;
  uint uVar16;
  lcd *plVar17;
  int *piVar18;
  byte *pbVar19;
  uint uVar20;
  int iVar21;
  uint uVar22;
  uint uVar23;
  int iVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  undefined2 *puVar28;
  int iVar29;
  uint uVar30;
  undefined2 *puVar31;
  undefined2 local_38 [4];
  int local_30;
  int local_2c;
  
  piVar18 = *(int **)(this + 0x3ac);
  bVar2 = *(byte *)(piVar18 + 10);
  if (((char)bVar2 < '\0') && ((bVar2 & 1) != 0)) {
    if ((((uint)param_2 < (uint)*(byte *)((int)piVar18 + 0x32)) ||
        (7 < *(byte *)((int)piVar18 + 0x33))) || ((bVar2 & 0x20) == 0)) {
      iVar14 = 0x1800;
      bVar3 = *(byte *)((int)piVar18 + 0x2b);
      if ((bVar2 & 8) != 0) {
        iVar14 = 0x1c00;
      }
      uVar20 = (uint)*(byte *)((int)piVar18 + 0x2a) + param_2;
      local_2c = (int)(uint)bVar3 >> 3;
      bVar4 = *(byte *)((int)piVar18 + 0x2f);
      if (0xff < (int)uVar20) {
        uVar20 = uVar20 - 0x100;
      }
      iVar21 = *piVar18 + 0x8012;
      pbVar19 = (byte *)(iVar21 + ((int)uVar20 >> 3) * 0x20 + local_2c + iVar14);
      bVar5 = *pbVar19;
      local_38[0] = *(undefined2 *)(this + (bVar4 & 3) * 2);
      local_38[2] = *(undefined2 *)(this + (bVar4 >> 3 & 6));
      local_38[1] = *(undefined2 *)(this + (bVar4 >> 1 & 6));
      iVar24 = (uVar20 & 7) * 2;
      puVar15 = (undefined2 *)((int)param_1 + param_2 * 0x140);
      local_38[3] = *(undefined2 *)(this + (uint)(bVar4 >> 6) * 2);
      local_30 = iVar21 + iVar24;
      iVar21 = iVar21 + iVar24 + (uint)((bVar2 & 0x10) == 0) * 0x1000;
      if ((char)bVar5 < '\0') {
        uVar6 = *(ushort *)(local_30 + (uint)bVar5 * 0x10);
      }
      else {
        uVar6 = *(ushort *)(iVar21 + (uint)bVar5 * 0x10);
      }
      uVar22 = uVar6 >> 8 & 0xaa | uVar6 >> 1 & 0x55;
      uVar16 = uVar6 >> 7 & 0xaa | uVar6 & 0x55;
      uVar30 = uVar22 >> 4 & 3;
      uVar27 = uVar16 >> 4 & 3;
      uVar26 = uVar22 >> 2 & 3;
      uVar25 = uVar16 >> 2 & 3;
      puVar15[6] = local_38[uVar22 & 3];
      puVar15[7] = local_38[uVar16 & 3];
      uVar11 = local_38[uVar22 >> 6];
      uVar7 = local_38[uVar16 >> 6];
      uVar8 = local_38[uVar30];
      uVar23 = bVar3 & 7;
      uVar9 = local_38[uVar26];
      uVar10 = local_38[uVar25];
      iVar24 = 8 - uVar23;
      puVar15[3] = local_38[uVar27];
      puVar31 = puVar15 + uVar23;
      *puVar15 = uVar11;
      plVar17 = this + 0x11c + iVar24;
      puVar15[1] = uVar7;
      puVar15[2] = uVar8;
      puVar15[4] = uVar9;
      puVar15[5] = uVar10;
      this[0x11c] = SUB41(uVar22 >> 6,0);
      this[0x11d] = SUB41(uVar16 >> 6,0);
      this[0x11e] = SUB41(uVar30,0);
      this[0x11f] = SUB41(uVar27,0);
      this[0x120] = SUB41(uVar26,0);
      this[0x121] = SUB41(uVar25,0);
      this[0x122] = SUB41(uVar22 & 3,0);
      this[0x123] = SUB41(uVar16 & 3,0);
      plVar12 = this + 0x11c;
      puVar28 = puVar15;
      do {
        plVar13 = plVar12 + 1;
        *puVar28 = puVar31[1];
        puVar1 = puVar31 + 1;
        puVar31 = puVar31 + 1;
        *plVar12 = SUB21(*puVar1,0);
        plVar12 = plVar13;
        puVar28 = puVar28 + 1;
      } while (plVar13 != plVar17);
      if ((int)uVar20 < 0) {
        uVar20 = uVar20 + 7;
      }
      iVar29 = 0;
      puVar15 = puVar15 + iVar24;
      plVar12 = plVar17;
      do {
        pbVar19 = pbVar19 + 1;
        if (0xf7 < (int)(plVar12 + ((local_2c * 8 - (int)plVar17) - iVar29))) {
          iVar29 = 0x100;
          pbVar19 = (byte *)(**(int **)(this + 0x3ac) + 0x8012 + ((int)uVar20 >> 3) * 0x20 + iVar14)
          ;
        }
        iVar24 = (uint)*pbVar19 * 0x10;
        if ((char)*pbVar19 < '\0') {
          uVar6 = *(ushort *)(local_30 + iVar24);
        }
        else {
          uVar6 = *(ushort *)(iVar21 + iVar24);
        }
        uVar16 = uVar6 >> 7 & 0xaa | uVar6 & 0x55;
        uVar26 = uVar16 >> 4 & 3;
        uVar22 = uVar6 >> 8 & 0xaa | uVar6 >> 1 & 0x55;
        uVar27 = uVar22 >> 4 & 3;
        uVar25 = uVar22 >> 2 & 3;
        uVar23 = uVar16 >> 2 & 3;
        puVar15[3] = local_38[uVar26];
        uVar11 = local_38[uVar16 >> 6];
        uVar7 = local_38[uVar27];
        uVar8 = local_38[uVar25];
        uVar9 = local_38[uVar23];
        uVar10 = local_38[uVar22 & 3];
        *puVar15 = local_38[uVar22 >> 6];
        puVar15[1] = uVar11;
        puVar15[6] = uVar10;
        puVar15[2] = uVar7;
        puVar15[4] = uVar8;
        puVar15[5] = uVar9;
        puVar15[7] = local_38[uVar16 & 3];
        *plVar12 = SUB41(uVar22 >> 6,0);
        plVar12[1] = SUB41(uVar16 >> 6,0);
        plVar12[2] = SUB41(uVar27,0);
        plVar12[3] = SUB41(uVar26,0);
        plVar12[4] = SUB41(uVar25,0);
        plVar12[5] = SUB41(uVar23,0);
        plVar12[6] = SUB41(uVar22 & 3,0);
        plVar13 = plVar12 + 8;
        plVar12[7] = SUB41(uVar16 & 3,0);
        puVar15 = puVar15 + 8;
        plVar12 = plVar13;
      } while (plVar13 != plVar17 + 0xa0);
    }
  }
  else {
    puVar28 = (undefined2 *)((int)param_1 + param_2 * 0x140);
    uVar11 = (**(code **)(*(int *)piVar18[5] + 0x10))((int *)piVar18[5],0x7fff);
    puVar15 = puVar28;
    do {
      puVar31 = puVar15 + 1;
      *puVar15 = uVar11;
      puVar15 = puVar31;
    } while (puVar31 != puVar28 + 0xa0);
  }
  return;
}



/* ===== FUNCTION win_render @ 0002a6c8 ===== */

/* lcd::win_render(void*, int) */

void __thiscall lcd::win_render(lcd *this,void *param_1,int param_2)

{
  byte bVar1;
  byte bVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  ushort uVar9;
  uint uVar10;
  int *piVar11;
  uint uVar12;
  lcd *plVar13;
  undefined2 *puVar14;
  uint uVar15;
  byte *pbVar16;
  byte *pbVar17;
  uint uVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  undefined2 local_28 [4];
  byte *pbVar18;
  
  piVar11 = *(int **)(this + 0x3ac);
  bVar1 = *(byte *)(piVar11 + 10);
  if (((((char)bVar1 < '\0') && ((bVar1 & 0x20) != 0)) &&
      ((int)(uint)*(byte *)((int)piVar11 + 0x32) < param_2 + 1)) &&
     (uVar15 = (uint)*(byte *)((int)piVar11 + 0x33), uVar15 < 0xa7)) {
    iVar21 = *piVar11;
    iVar20 = 0x1c00;
    iVar22 = *(int *)(this + 0x39c) + -1;
    *(int *)(this + 0x39c) = *(int *)(this + 0x39c) + 1;
    bVar2 = *(byte *)((int)piVar11 + 0x2f);
    if ((bVar1 & 0x40) == 0) {
      iVar20 = 0x1800;
    }
    local_28[0] = *(undefined2 *)(this + (bVar2 & 3) * 2);
    local_28[1] = *(undefined2 *)(this + (bVar2 >> 1 & 6));
    iVar21 = iVar21 + 0x8012;
    pbVar16 = (byte *)(iVar21 + ((iVar22 >> 3) + -1) * 0x20 + iVar20);
    local_28[2] = *(undefined2 *)(this + (bVar2 >> 3 & 6));
    local_28[3] = *(undefined2 *)(this + (uint)(bVar2 >> 6) * 2);
    uVar23 = iVar22 * 2 & 0xe;
    plVar13 = this + uVar15 + 0x115;
    puVar14 = (undefined2 *)((int)param_1 + (param_2 * 0xa0 + uVar15 + -7) * 2);
    pbVar18 = pbVar16;
    do {
      pbVar17 = pbVar18 + 1;
      iVar20 = (uint)*pbVar18 * 0x10;
      if ((char)*pbVar18 < '\0') {
        uVar9 = *(ushort *)(iVar21 + iVar20 + uVar23);
      }
      else {
        uVar9 = *(ushort *)(iVar21 + iVar20 + uVar23 + (uint)((bVar1 & 0x10) == 0) * 0x1000);
      }
      uVar10 = uVar9 >> 7 & 0xaa | uVar9 & 0x55;
      uVar12 = uVar9 >> 8 & 0xaa | uVar9 >> 1 & 0x55;
      uVar26 = uVar12 >> 4 & 3;
      uVar25 = uVar10 >> 4 & 3;
      uVar24 = uVar12 >> 2 & 3;
      uVar19 = uVar10 >> 2 & 3;
      uVar3 = local_28[uVar12 & 3];
      uVar4 = local_28[uVar10 >> 6];
      uVar5 = local_28[uVar26];
      uVar6 = local_28[uVar25];
      uVar7 = local_28[uVar24];
      uVar8 = local_28[uVar19];
      *puVar14 = local_28[uVar12 >> 6];
      puVar14[1] = uVar4;
      puVar14[2] = uVar5;
      puVar14[3] = uVar6;
      puVar14[4] = uVar7;
      puVar14[5] = uVar8;
      puVar14[6] = uVar3;
      puVar14[7] = local_28[uVar10 & 3];
      plVar13[6] = SUB41(uVar12 & 3,0);
      plVar13[5] = SUB41(uVar19,0);
      plVar13[4] = SUB41(uVar24,0);
      plVar13[3] = SUB41(uVar25,0);
      plVar13[2] = SUB41(uVar26,0);
      plVar13[1] = SUB41(uVar10 >> 6,0);
      *plVar13 = SUB41(uVar12 >> 6,0);
      plVar13[7] = SUB41(uVar10 & 3,0);
      plVar13 = plVar13 + 8;
      puVar14 = puVar14 + 8;
      pbVar18 = pbVar17;
    } while (pbVar16 + (0x15 - ((int)uVar15 >> 3)) != pbVar17);
    return;
  }
  return;
}



/* ===== FUNCTION sprite_render @ 0002a974 ===== */

/* lcd::sprite_render(void*, int) */

void __thiscall lcd::sprite_render(lcd *this,void *param_1,int param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  lcd lVar4;
  ushort uVar5;
  byte *pbVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int *piVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  int iVar16;
  undefined2 *puVar17;
  int iVar18;
  uint uVar19;
  undefined2 local_38 [4];
  undefined2 local_30;
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  uint uVar10;
  uint uVar11;
  
  piVar12 = *(int **)(this + 0x3ac);
  bVar1 = *(byte *)(piVar12 + 10);
  if ((-1 < (char)bVar1) || ((bVar1 & 2) == 0)) {
    return;
  }
  iVar18 = *piVar12;
  bVar2 = *(byte *)(piVar12 + 0xc);
  bVar3 = *(byte *)((int)piVar12 + 0x31);
  local_38[2] = *(undefined2 *)(this + (bVar2 >> 3 & 6));
  local_38[1] = *(undefined2 *)(this + (bVar2 >> 1 & 6));
  local_38[3] = *(undefined2 *)(this + (uint)(bVar2 >> 6) * 2);
  pbVar6 = (byte *)(iVar18 + 0xc12e);
  local_30 = *(undefined2 *)(this + (bVar3 & 3) * 2);
  local_38[0] = *(undefined2 *)(this + (bVar2 & 3) * 2);
  local_2e = *(undefined2 *)(this + (bVar3 >> 1 & 6));
  local_2c = *(undefined2 *)(this + (bVar3 >> 3 & 6));
  local_2a = *(undefined2 *)(this + (uint)(bVar3 >> 6) * 2);
  do {
    uVar7 = (uint)pbVar6[1];
    uVar15 = (uint)pbVar6[2];
    bVar2 = pbVar6[3];
    iVar8 = uVar7 - 8;
    if ((bVar1 & 4) == 0) {
      if (iVar8 < 0xa1) {
        iVar13 = *pbVar6 - 9;
        if (((iVar13 < 0x98) && (param_2 <= iVar13)) && (iVar13 <= param_2 + 7)) {
          uVar14 = iVar13 - param_2;
          if ((bVar2 & 0x40) == 0) {
            uVar14 = 7 - uVar14;
          }
          uVar5 = *(ushort *)(iVar18 + 0x8012 + uVar15 * 0x10 + (uVar14 & 7) * 2);
LAB_0002ab3c:
          *(int *)(this + 0x3a4) = *(int *)(this + 0x3a4) + 1;
          puVar17 = (undefined2 *)((int)param_1 + iVar8 * 2 + param_2 * 0x140);
          uVar15 = uVar5 & 0x55 | uVar5 >> 7 & 0xaa;
          uVar14 = uVar5 >> 1 & 0x55 | uVar5 >> 8 & 0xaa;
          if ((bVar2 & 0x20) != 0) {
            uVar19 = (int)uVar15 >> 2 & 0x33U | (uVar15 & 0x33) << 2;
            uVar15 = (int)uVar14 >> 2 & 0x33U | (uVar14 & 0x33) << 2;
            uVar15 = (uVar15 & 0xf) << 4 | (int)uVar15 >> 4;
            uVar14 = (uVar19 & 0xf) << 4 | (int)uVar19 >> 4;
          }
          iVar13 = (bVar2 >> 1 & 8) - 0x38;
          uVar19 = (int)uVar14 >> 2;
          uVar9 = (int)uVar14 >> 4;
          uVar10 = (int)uVar15 >> 2;
          uVar11 = (int)uVar15 >> 4;
          iVar16 = (int)uVar15 >> 6;
          if (iVar8 < 0) {
            if ((char)bVar2 < '\0') {
              if (iVar8 == -1) {
                lVar4 = this[0x11c];
                goto joined_r0x0002b014;
              }
              if (iVar8 == -2) goto LAB_0002ae18;
              if (iVar8 == -3) goto LAB_0002ae3c;
              if (iVar8 == -4) goto LAB_0002ae60;
              if (iVar8 == -5) goto LAB_0002ae84;
              if (iVar8 == -6) goto LAB_0002aea8;
              if (iVar8 == -8) goto joined_r0x0002aacc;
              lVar4 = this[uVar7 + 0x11b];
              goto LAB_0002aecc;
            }
            if (iVar8 == -1) {
              if (iVar16 != 0) {
                puVar17[1] = *(undefined2 *)((int)local_38 + iVar16 * 2 + iVar13 + 0x38);
              }
LAB_0002af2c:
              if ((uVar9 & 3) != 0) {
                puVar17[2] = *(undefined2 *)((int)local_38 + (uVar9 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002af48:
              if ((uVar11 & 3) != 0) {
                puVar17[3] = *(undefined2 *)((int)local_38 + (uVar11 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002af64:
              if ((uVar19 & 3) != 0) {
                puVar17[4] = *(undefined2 *)((int)local_38 + (uVar19 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002af80:
              if ((uVar10 & 3) != 0) {
                puVar17[5] = *(undefined2 *)((int)local_38 + (uVar10 & 3) * 2 + iVar13 + 0x38);
              }
            }
            else {
              if (iVar8 == -2) goto LAB_0002af2c;
              if (iVar8 == -3) goto LAB_0002af48;
              if (iVar8 == -4) goto LAB_0002af64;
              if (iVar8 == -5) goto LAB_0002af80;
              if (iVar8 != -6) {
                if (iVar8 == -8) goto joined_r0x0002aacc;
                goto LAB_0002ad9c;
              }
            }
            if ((uVar14 & 3) != 0) {
              puVar17[6] = *(undefined2 *)((int)local_38 + (uVar14 & 3) * 2 + iVar13 + 0x38);
            }
LAB_0002ad9c:
            if ((uVar15 & 3) != 0) {
              puVar17[7] = *(undefined2 *)((int)local_38 + (uVar15 & 3) * 2 + iVar13 + 0x38);
            }
          }
          else {
            iVar8 = (int)uVar14 >> 6;
            if ((char)bVar2 < '\0') {
              if ((this[uVar7 + 0x114] == (lcd)0x0) && (iVar8 != 0)) {
                *puVar17 = *(undefined2 *)((int)local_38 + iVar8 * 2 + iVar13 + 0x38);
              }
              lVar4 = this[uVar7 + 0x115];
joined_r0x0002b014:
              if ((lVar4 == (lcd)0x0) && (iVar16 != 0)) {
                puVar17[1] = *(undefined2 *)((int)local_38 + iVar16 * 2 + iVar13 + 0x38);
              }
LAB_0002ae18:
              if ((this[uVar7 + 0x116] == (lcd)0x0) && ((uVar9 & 3) != 0)) {
                puVar17[2] = *(undefined2 *)((int)local_38 + (uVar9 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002ae3c:
              if ((this[uVar7 + 0x117] == (lcd)0x0) && ((uVar11 & 3) != 0)) {
                puVar17[3] = *(undefined2 *)((int)local_38 + (uVar11 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002ae60:
              if ((this[uVar7 + 0x118] == (lcd)0x0) && ((uVar19 & 3) != 0)) {
                puVar17[4] = *(undefined2 *)((int)local_38 + (uVar19 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002ae84:
              if ((this[uVar7 + 0x119] == (lcd)0x0) && ((uVar10 & 3) != 0)) {
                puVar17[5] = *(undefined2 *)((int)local_38 + (uVar10 & 3) * 2 + iVar13 + 0x38);
              }
LAB_0002aea8:
              if ((this[uVar7 + 0x11a] == (lcd)0x0) && ((uVar14 & 3) != 0)) {
                puVar17[6] = *(undefined2 *)((int)local_38 + (uVar14 & 3) * 2 + iVar13 + 0x38);
              }
              lVar4 = this[uVar7 + 0x11b];
LAB_0002aecc:
              if (lVar4 == (lcd)0x0) goto LAB_0002ad9c;
            }
            else {
              if (iVar8 != 0) {
                *puVar17 = *(undefined2 *)((int)local_38 + iVar8 * 2 + iVar13 + 0x38);
              }
              if (iVar16 != 0) {
                puVar17[1] = *(undefined2 *)((int)local_38 + iVar16 * 2 + iVar13 + 0x38);
              }
              if ((uVar9 & 3) != 0) {
                puVar17[2] = *(undefined2 *)((int)local_38 + (uVar9 & 3) * 2 + iVar13 + 0x38);
              }
              if ((uVar11 & 3) != 0) {
                puVar17[3] = *(undefined2 *)((int)local_38 + (uVar11 & 3) * 2 + iVar13 + 0x38);
              }
              if ((uVar19 & 3) != 0) {
                puVar17[4] = *(undefined2 *)((int)local_38 + (uVar19 & 3) * 2 + iVar13 + 0x38);
              }
              if ((uVar10 & 3) != 0) {
                puVar17[5] = *(undefined2 *)((int)local_38 + (uVar10 & 3) * 2 + iVar13 + 0x38);
              }
              if ((uVar14 & 3) != 0) {
                puVar17[6] = *(undefined2 *)((int)local_38 + (uVar14 & 3) * 2 + iVar13 + 0x38);
              }
              if ((uVar15 & 3) != 0) {
                puVar17[7] = *(undefined2 *)((int)local_38 + (uVar15 & 3) * 2 + iVar13 + 0x38);
              }
            }
          }
        }
      }
    }
    else if (iVar8 < 0xa1) {
      iVar13 = *pbVar6 - 1;
      if (((iVar13 < 0xa0) && (param_2 <= iVar13)) && (iVar13 <= param_2 + 0xf)) {
        if ((param_2 - iVar13) + 0xf < 8) {
          if ((bVar2 & 0x40) == 0) {
            iVar16 = 0;
            iVar13 = (7U - (iVar13 - param_2) & 7) * 2 + (uVar15 & 0xfffffffe) * 0x10;
          }
          else {
            iVar16 = 0x10;
            iVar13 = (iVar13 - param_2 & 7U) * 2 + (uVar15 & 0xfffffffe) * 0x10;
          }
        }
        else if ((bVar2 & 0x40) == 0) {
          iVar16 = 0x10;
          iVar13 = (7U - (iVar13 - param_2) & 7) * 2 + (uVar15 & 0xfffffffe) * 0x10;
        }
        else {
          iVar16 = 0;
          iVar13 = (iVar13 - param_2 & 7U) * 2 + (uVar15 & 0xfffffffe) * 0x10;
        }
        uVar5 = *(ushort *)(iVar18 + 0x8012 + iVar16 + iVar13);
        goto LAB_0002ab3c;
      }
    }
joined_r0x0002aacc:
    pbVar6 = pbVar6 + -4;
    if ((byte *)(iVar18 + 0xc08e) == pbVar6) {
      return;
    }
  } while( true );
}



/* ===== FUNCTION bg_render_color @ 0002b044 ===== */

/* lcd::bg_render_color(void*, int) */

void __thiscall lcd::bg_render_color(lcd *this,void *param_1,int param_2)

{
  lcd *plVar1;
  byte bVar2;
  byte bVar3;
  ushort uVar4;
  undefined2 uVar5;
  lcd *plVar6;
  uint uVar7;
  int iVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  lcd *plVar13;
  undefined2 *puVar14;
  byte *pbVar15;
  lcd *plVar16;
  lcd *plVar17;
  lcd *plVar18;
  int iVar19;
  byte *pbVar20;
  undefined2 *puVar21;
  int iVar22;
  lcd *plVar23;
  int iVar24;
  lcd *plVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  lcd lVar29;
  undefined2 *puVar30;
  int iVar31;
  uint uVar32;
  uint uVar33;
  uint uVar34;
  uint uVar35;
  int iVar36;
  uint uVar37;
  uint uVar38;
  uint uVar39;
  
  piVar9 = *(int **)(this + 0x3ac);
  *(undefined4 *)(this + 0x118) = 0;
  bVar2 = *(byte *)(piVar9 + 10);
  if ((char)bVar2 < '\0') {
    if ((((uint)param_2 < (uint)*(byte *)((int)piVar9 + 0x32)) ||
        (7 < *(byte *)((int)piVar9 + 0x33))) || ((bVar2 & 0x20) == 0)) {
      bVar3 = *(byte *)((int)piVar9 + 0x2b);
      iVar10 = 0x1800;
      iVar31 = (uint)*(byte *)((int)piVar9 + 0x2a) + param_2;
      if ((bVar2 & 8) != 0) {
        iVar10 = 0x1c00;
      }
      iVar27 = (int)(uint)bVar3 >> 3;
      iVar36 = 0x100e;
      if (0xff < iVar31) {
        iVar31 = iVar31 + -0x100;
      }
      iVar28 = *piVar9 + 0x8012;
      uVar38 = iVar31 << 1 & 0xe;
      iVar19 = (iVar31 >> 3) * 0x20 + iVar27 + iVar10;
      pbVar15 = (byte *)(iVar28 + iVar19);
      if ((bVar2 & 0x10) != 0) {
        iVar36 = 0xe;
      }
      iVar11 = 0x1000;
      if ((bVar2 & 0x10) != 0) {
        iVar11 = 0;
      }
      pbVar20 = (byte *)(iVar28 + iVar19 + 0x2000);
      bVar2 = *pbVar20;
      iVar19 = iVar28 + uVar38 + iVar11;
      iVar11 = iVar28 + (~(iVar31 << 1) & 0xeU);
      iVar12 = (bVar2 & 7) + 0x13;
      puVar14 = (undefined2 *)((int)param_1 + param_2 * 0x140);
      plVar6 = this + 0x11c;
      iVar36 = iVar28 + (iVar36 - uVar38);
      plVar16 = this + 0x25c;
      if ((char)*pbVar15 < '\0') {
        iVar8 = iVar11;
        if ((bVar2 & 0x40) == 0) {
          iVar8 = iVar28 + uVar38;
        }
      }
      else {
        iVar8 = iVar19;
        if ((bVar2 & 0x40) != 0) {
          iVar8 = iVar36;
        }
      }
      uVar4 = *(ushort *)(iVar8 + ((uint)*pbVar15 * 8 + (bVar2 & 8) * 0x200) * 2);
      uVar37 = uVar4 >> 7 & 0xaa | uVar4 & 0x55;
      uVar39 = uVar4 >> 8 & 0xaa | uVar4 >> 1 & 0x55;
      if ((bVar2 & 0x20) != 0) {
        uVar7 = uVar37 >> 2 & 0x33 | (uVar37 & 0x33) << 2;
        uVar37 = (int)uVar39 >> 2 & 0x33U | (uVar39 & 0x33) << 2;
        uVar37 = (uVar37 & 0xf) << 4 | uVar37 >> 4;
        uVar39 = (uVar7 & 0xf) << 4 | uVar7 >> 4;
      }
      uVar35 = uVar39 >> 4 & 3;
      *puVar14 = *(undefined2 *)(this + (uVar39 >> 6) * 2 + iVar12 * 8);
      uVar34 = uVar37 >> 4 & 3;
      puVar14[1] = *(undefined2 *)(this + (uVar37 >> 6) * 2 + iVar12 * 8);
      uVar33 = uVar39 >> 2 & 3;
      puVar14[2] = *(undefined2 *)(this + uVar35 * 2 + iVar12 * 8);
      uVar32 = uVar37 >> 2 & 3;
      puVar14[3] = *(undefined2 *)(this + uVar34 * 2 + iVar12 * 8);
      puVar14[4] = *(undefined2 *)(this + uVar33 * 2 + iVar12 * 8);
      uVar7 = bVar3 & 7;
      puVar14[5] = *(undefined2 *)(this + uVar32 * 2 + iVar12 * 8);
      lVar29 = (lcd)(bVar2 & 0x80);
      iVar8 = 8 - uVar7;
      puVar14[6] = *(undefined2 *)(this + (uVar39 & 3) * 2 + iVar12 * 8);
      plVar25 = this + 0x11c + uVar7;
      puVar21 = puVar14 + uVar7;
      plVar18 = plVar16 + uVar7;
      plVar13 = plVar16 + iVar8;
      puVar14[7] = *(undefined2 *)(this + (uVar37 & 3) * 2 + iVar12 * 8);
      this[0x25c] = lVar29;
      this[0x123] = SUB41(uVar37 & 3,0);
      this[0x122] = SUB41(uVar39 & 3,0);
      this[0x121] = SUB41(uVar32,0);
      this[0x120] = SUB41(uVar33,0);
      this[0x11f] = SUB41(uVar34,0);
      this[0x11e] = SUB41(uVar35,0);
      this[0x11d] = SUB41(uVar37 >> 6,0);
      this[0x11c] = SUB41(uVar39 >> 6,0);
      this[0x25d] = lVar29;
      this[0x25e] = lVar29;
      this[0x25f] = lVar29;
      this[0x260] = lVar29;
      this[0x261] = lVar29;
      this[0x262] = lVar29;
      this[0x263] = lVar29;
      puVar30 = puVar14;
      plVar23 = this + 0x11c;
      do {
        plVar17 = plVar16 + 1;
        *puVar30 = puVar21[1];
        puVar21 = puVar21 + 1;
        plVar1 = plVar25 + 1;
        plVar25 = plVar25 + 1;
        *plVar23 = *plVar1;
        plVar1 = plVar18 + 1;
        plVar18 = plVar18 + 1;
        *plVar16 = *plVar1;
        plVar16 = plVar17;
        puVar30 = puVar30 + 1;
        plVar23 = plVar23 + 1;
      } while (plVar17 != plVar13);
      if (iVar31 < 0) {
        iVar31 = iVar31 + 7;
      }
      iVar12 = 0;
      iVar10 = (iVar31 >> 3) * 0x20 + iVar10;
      plVar16 = plVar6 + iVar8;
      puVar30 = puVar14 + iVar8;
      do {
        pbVar15 = pbVar15 + 1;
        pbVar20 = pbVar20 + 1;
        if (0xf7 < (int)(plVar16 + ((iVar27 * 8 - (int)(plVar6 + iVar8)) - iVar12))) {
          iVar12 = 0x100;
          pbVar15 = (byte *)(**(int **)(this + 0x3ac) + 0x8012 + iVar10);
          pbVar20 = (byte *)(**(int **)(this + 0x3ac) + 0x8012 + iVar10 + 0x2000);
        }
        bVar2 = *pbVar20;
        iVar22 = (bVar2 & 7) + 0x13;
        iVar24 = (bVar2 & 8) * 0x200;
        iVar31 = (uint)*pbVar15 * 8;
        if ((char)*pbVar15 < '\0') {
          iVar26 = iVar11;
          if ((bVar2 & 0x40) == 0) {
            iVar26 = iVar28 + uVar38;
          }
          uVar4 = *(ushort *)(iVar26 + (iVar31 + iVar24) * 2);
        }
        else {
          iVar26 = iVar36;
          if ((bVar2 & 0x40) == 0) {
            iVar26 = iVar19;
          }
          uVar4 = *(ushort *)(iVar26 + (iVar31 + iVar24) * 2);
        }
        uVar37 = uVar4 >> 7 & 0xaa | uVar4 & 0x55;
        uVar39 = uVar4 >> 8 & 0xaa | uVar4 >> 1 & 0x55;
        if ((bVar2 & 0x20) != 0) {
          uVar7 = uVar37 >> 2 & 0x33 | (uVar37 & 0x33) << 2;
          uVar37 = (int)uVar39 >> 2 & 0x33U | (uVar39 & 0x33) << 2;
          uVar37 = (uVar37 & 0xf) << 4 | uVar37 >> 4;
          uVar39 = (uVar7 & 0xf) << 4 | uVar7 >> 4;
        }
        uVar34 = uVar39 >> 4 & 3;
        *puVar30 = *(undefined2 *)(this + (uVar39 >> 6) * 2 + iVar22 * 8);
        uVar33 = uVar37 >> 4 & 3;
        puVar30[1] = *(undefined2 *)(this + (uVar37 >> 6) * 2 + iVar22 * 8);
        uVar32 = uVar39 >> 2 & 3;
        puVar30[2] = *(undefined2 *)(this + uVar34 * 2 + iVar22 * 8);
        uVar7 = uVar37 >> 2 & 3;
        puVar30[3] = *(undefined2 *)(this + uVar33 * 2 + iVar22 * 8);
        puVar30[4] = *(undefined2 *)(this + uVar32 * 2 + iVar22 * 8);
        lVar29 = (lcd)(bVar2 & 0x80);
        puVar30[5] = *(undefined2 *)(this + uVar7 * 2 + iVar22 * 8);
        puVar30[6] = *(undefined2 *)(this + (uVar39 & 3) * 2 + iVar22 * 8);
        puVar21 = puVar30 + 8;
        puVar30[7] = *(undefined2 *)(this + (uVar37 & 3) * 2 + iVar22 * 8);
        plVar16[6] = SUB41(uVar39 & 3,0);
        plVar16[5] = SUB41(uVar7,0);
        plVar16[4] = SUB41(uVar32,0);
        plVar16[3] = SUB41(uVar33,0);
        plVar16[2] = SUB41(uVar34,0);
        plVar16[1] = SUB41(uVar37 >> 6,0);
        *plVar16 = SUB41(uVar39 >> 6,0);
        plVar16[7] = SUB41(uVar37 & 3,0);
        *plVar13 = lVar29;
        plVar13[1] = lVar29;
        plVar13[2] = lVar29;
        plVar13[3] = lVar29;
        plVar13[4] = lVar29;
        plVar13[5] = lVar29;
        plVar13[6] = lVar29;
        plVar13[7] = lVar29;
        plVar13 = plVar13 + 8;
        plVar16 = plVar16 + 8;
        puVar30 = puVar21;
      } while (puVar21 != puVar14 + iVar8 + 0xa0);
      if ((*(byte *)(*(int *)(this + 0x3ac) + 0x28) & 1) == 0) {
        (*(code *)(undefined *)0x0)(plVar6,0,0xa0);
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)(undefined *)0x0)();
        (*(code *)0x35e0)();
        return;
      }
    }
  }
  else {
    puVar30 = (undefined2 *)((int)param_1 + param_2 * 0x140);
    uVar5 = (**(code **)(*(int *)piVar9[5] + 0x10))((int *)piVar9[5],0x7fff);
    puVar14 = puVar30;
    do {
      puVar21 = puVar14 + 1;
      *puVar14 = uVar5;
      puVar14 = puVar21;
    } while (puVar21 != puVar30 + 0xa0);
  }
  return;
}



/* ===== FUNCTION win_render_color @ 0002b6f0 ===== */

/* lcd::win_render_color(void*, int) */

void __thiscall lcd::win_render_color(lcd *this,void *param_1,int param_2)

{
  byte bVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  undefined2 *puVar5;
  lcd lVar6;
  int *piVar7;
  int iVar8;
  uint uVar9;
  lcd *plVar10;
  lcd *plVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  byte *pbVar17;
  byte *pbVar18;
  int iVar20;
  int iVar21;
  uint uVar22;
  uint uVar23;
  byte *pbVar24;
  byte *pbVar19;
  
  piVar7 = *(int **)(this + 0x3ac);
  bVar1 = *(byte *)(piVar7 + 10);
  if (((((char)bVar1 < '\0') && ((bVar1 & 0x20) != 0)) &&
      ((int)(uint)*(byte *)((int)piVar7 + 0x32) < param_2 + 1)) &&
     (uVar9 = (uint)*(byte *)((int)piVar7 + 0x33), uVar9 < 0xa7)) {
    iVar21 = *(int *)(this + 0x39c) + -1;
    iVar8 = 0x1800;
    if ((bVar1 & 0x40) != 0) {
      iVar8 = 0x1c00;
    }
    iVar8 = ((iVar21 >> 3) + -1) * 0x20 + iVar8;
    iVar20 = *piVar7 + 0x8012;
    uVar22 = iVar21 * 2;
    iVar21 = (uint)((bVar1 & 0x10) == 0) * 0x1000;
    pbVar17 = (byte *)(iVar20 + iVar8 + 0x2000);
    uVar23 = uVar22 & 0xe;
    uVar22 = ~uVar22 & 0xe;
    *(int *)(this + 0x39c) = *(int *)(this + 0x39c) + 1;
    plVar10 = this + uVar9 + 0x255;
    puVar5 = (undefined2 *)((int)param_1 + (param_2 * 0xa0 + uVar9 + -7) * 2);
    plVar11 = this + uVar9 + 0x115;
    pbVar19 = pbVar17;
    pbVar24 = (byte *)(iVar20 + iVar8);
    do {
      pbVar18 = pbVar19 + 1;
      bVar1 = *pbVar19;
      iVar8 = (bVar1 & 7) + 0x13;
      if ((char)*pbVar24 < '\0') {
        uVar3 = uVar22;
        if ((bVar1 & 0x40) == 0) {
          uVar3 = uVar23;
        }
        iVar13 = iVar20 + uVar3;
      }
      else {
        iVar13 = iVar20 + uVar22 + iVar21;
        if ((bVar1 & 0x40) == 0) {
          iVar13 = iVar20 + uVar23 + iVar21;
        }
      }
      uVar2 = *(ushort *)(iVar13 + ((uint)*pbVar24 * 8 + (bVar1 & 8) * 0x200) * 2);
      uVar3 = uVar2 >> 7 & 0xaa | uVar2 & 0x55;
      uVar4 = uVar2 >> 8 & 0xaa | uVar2 >> 1 & 0x55;
      if ((bVar1 & 0x20) != 0) {
        uVar12 = uVar3 >> 2 & 0x33 | (uVar3 & 0x33) << 2;
        uVar3 = (int)uVar4 >> 2 & 0x33U | (uVar4 & 0x33) << 2;
        uVar3 = (uVar3 & 0xf) << 4 | uVar3 >> 4;
        uVar4 = (uVar12 & 0xf) << 4 | uVar12 >> 4;
      }
      uVar16 = uVar4 >> 4 & 3;
      *puVar5 = *(undefined2 *)(this + (uVar4 >> 6) * 2 + iVar8 * 8);
      uVar15 = uVar3 >> 4 & 3;
      puVar5[1] = *(undefined2 *)(this + (uVar3 >> 6) * 2 + iVar8 * 8);
      uVar14 = uVar4 >> 2 & 3;
      puVar5[2] = *(undefined2 *)(this + uVar16 * 2 + iVar8 * 8);
      uVar12 = uVar3 >> 2 & 3;
      puVar5[3] = *(undefined2 *)(this + uVar15 * 2 + iVar8 * 8);
      puVar5[4] = *(undefined2 *)(this + uVar14 * 2 + iVar8 * 8);
      lVar6 = (lcd)(bVar1 & 0x80);
      puVar5[5] = *(undefined2 *)(this + uVar12 * 2 + iVar8 * 8);
      puVar5[6] = *(undefined2 *)(this + (uVar4 & 3) * 2 + iVar8 * 8);
      puVar5[7] = *(undefined2 *)(this + (uVar3 & 3) * 2 + iVar8 * 8);
      *plVar11 = SUB41(uVar4 >> 6,0);
      plVar11[1] = SUB41(uVar3 >> 6,0);
      plVar11[2] = SUB41(uVar16,0);
      plVar11[3] = SUB41(uVar15,0);
      plVar11[4] = SUB41(uVar14,0);
      plVar11[5] = SUB41(uVar12,0);
      plVar11[6] = SUB41(uVar4 & 3,0);
      plVar11[7] = SUB41(uVar3 & 3,0);
      plVar10[7] = lVar6;
      plVar10[6] = lVar6;
      plVar10[5] = lVar6;
      plVar10[4] = lVar6;
      plVar10[3] = lVar6;
      plVar10[2] = lVar6;
      plVar10[1] = lVar6;
      *plVar10 = lVar6;
      plVar10 = plVar10 + 8;
      puVar5 = puVar5 + 8;
      plVar11 = plVar11 + 8;
      pbVar19 = pbVar18;
      pbVar24 = pbVar24 + 1;
    } while (pbVar17 + (0x15 - ((int)uVar9 >> 3)) != pbVar18);
    return;
  }
  return;
}



/* ===== FUNCTION sprite_render_color @ 0002ba24 ===== */

/* lcd::sprite_render_color(void*, int) */

void __thiscall lcd::sprite_render_color(lcd *this,void *param_1,int param_2)

{
  byte bVar1;
  byte bVar2;
  lcd lVar3;
  ushort uVar4;
  byte *pbVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar11;
  int iVar12;
  uint uVar13;
  uint uVar14;
  undefined2 *puVar15;
  int iVar16;
  int iVar17;
  uint uVar18;
  uint uVar9;
  uint uVar10;
  
  bVar1 = *(byte *)(*(int **)(this + 0x3ac) + 10);
  if ((-1 < (char)bVar1) || ((bVar1 & 2) == 0)) {
    return;
  }
  iVar11 = **(int **)(this + 0x3ac);
  pbVar5 = (byte *)(iVar11 + 0xc12e);
  do {
    bVar2 = pbVar5[3];
    uVar6 = (uint)pbVar5[1];
    uVar14 = (uint)pbVar5[2];
    iVar7 = uVar6 - 8;
    iVar12 = (uint)((bVar2 & 8) != 0) * 0x2000;
    if ((bVar1 & 4) == 0) {
      if (iVar7 < 0xa1) {
        iVar17 = *pbVar5 - 9;
        if (((iVar17 < 0x98) && (param_2 <= iVar17)) && (iVar17 <= param_2 + 7)) {
          uVar18 = iVar17 - param_2;
          if ((bVar2 & 0x40) == 0) {
            uVar18 = 7 - uVar18;
          }
          uVar4 = *(ushort *)(iVar11 + 0x8012 + uVar14 * 0x10 + (uVar18 & 7) * 2 + iVar12);
LAB_0002bb54:
          *(int *)(this + 0x3a4) = *(int *)(this + 0x3a4) + 1;
          puVar15 = (undefined2 *)((int)param_1 + iVar7 * 2 + param_2 * 0x140);
          uVar18 = uVar4 & 0x55 | uVar4 >> 7 & 0xaa;
          uVar14 = uVar4 >> 1 & 0x55 | uVar4 >> 8 & 0xaa;
          if ((bVar2 & 0x20) != 0) {
            uVar13 = (int)uVar14 >> 2 & 0x33U | (uVar14 & 0x33) << 2;
            uVar14 = (int)uVar18 >> 2 & 0x33U | (uVar18 & 0x33) << 2;
            uVar18 = (uVar13 & 0xf) << 4 | (int)uVar13 >> 4;
            uVar14 = (uVar14 & 0xf) << 4 | (int)uVar14 >> 4;
          }
          iVar17 = (bVar2 & 7) + 0x1b;
          uVar13 = (int)uVar14 >> 2;
          uVar8 = (int)uVar14 >> 4;
          uVar9 = (int)uVar18 >> 2;
          uVar10 = (int)uVar18 >> 4;
          iVar12 = (int)uVar18 >> 6;
          if (iVar7 < 0) {
            if ((char)bVar2 < '\0') {
              if (iVar7 == -1) {
                lVar3 = this[0x11c];
                goto joined_r0x0002c20c;
              }
              if (iVar7 == -2) goto LAB_0002bf44;
              if (iVar7 == -3) goto LAB_0002bf68;
              if (iVar7 == -4) goto LAB_0002bf8c;
              if (iVar7 == -5) goto LAB_0002bfb0;
              if (iVar7 == -6) goto LAB_0002bfd4;
              if (iVar7 == -8) goto joined_r0x0002bad4;
              lVar3 = this[uVar6 + 0x11b];
              goto joined_r0x0002bec8;
            }
            if (iVar7 == -1) {
              if (((this[uVar6 + 0x255] == (lcd)0x0) || (this[uVar6 + 0x115] == (lcd)0x0)) &&
                 (iVar12 != 0)) {
                puVar15[1] = *(undefined2 *)(this + iVar12 * 2 + iVar17 * 8);
              }
LAB_0002c098:
              if (((this[uVar6 + 0x256] == (lcd)0x0) || (this[uVar6 + 0x116] == (lcd)0x0)) &&
                 ((uVar8 & 3) != 0)) {
                puVar15[2] = *(undefined2 *)(this + (uVar8 & 3) * 2 + iVar17 * 8);
              }
LAB_0002c0d4:
              if (((this[uVar6 + 599] == (lcd)0x0) || (this[uVar6 + 0x117] == (lcd)0x0)) &&
                 ((uVar10 & 3) != 0)) {
                puVar15[3] = *(undefined2 *)(this + (uVar10 & 3) * 2 + iVar17 * 8);
              }
LAB_0002c110:
              if (((this[uVar6 + 600] == (lcd)0x0) || (this[uVar6 + 0x118] == (lcd)0x0)) &&
                 ((uVar13 & 3) != 0)) {
                puVar15[4] = *(undefined2 *)(this + (uVar13 & 3) * 2 + iVar17 * 8);
              }
LAB_0002c14c:
              if (((this[uVar6 + 0x259] == (lcd)0x0) || (this[uVar6 + 0x119] == (lcd)0x0)) &&
                 ((uVar9 & 3) != 0)) {
                puVar15[5] = *(undefined2 *)(this + (uVar9 & 3) * 2 + iVar17 * 8);
              }
LAB_0002c188:
              if (((this[uVar6 + 0x25a] == (lcd)0x0) || (this[uVar6 + 0x11a] == (lcd)0x0)) &&
                 ((uVar14 & 3) != 0)) {
                puVar15[6] = *(undefined2 *)(this + (uVar14 & 3) * 2 + iVar17 * 8);
              }
            }
            else {
              if (iVar7 == -2) goto LAB_0002c098;
              if (iVar7 == -3) goto LAB_0002c0d4;
              if (iVar7 == -4) goto LAB_0002c110;
              if (iVar7 == -5) goto LAB_0002c14c;
              if (iVar7 == -6) goto LAB_0002c188;
              if (iVar7 == -8) goto joined_r0x0002bad4;
            }
            if (this[uVar6 + 0x25b] != (lcd)0x0) {
              lVar3 = this[uVar6 + 0x11b];
              goto joined_r0x0002bec8;
            }
          }
          else {
            iVar7 = (int)uVar14 >> 6;
            if (-1 < (char)bVar2) {
              if (((this[uVar6 + 0x254] == (lcd)0x0) || (this[uVar6 + 0x114] == (lcd)0x0)) &&
                 (iVar7 != 0)) {
                *puVar15 = *(undefined2 *)(this + iVar7 * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 0x255] == (lcd)0x0) || (this[uVar6 + 0x115] == (lcd)0x0)) &&
                 (iVar12 != 0)) {
                puVar15[1] = *(undefined2 *)(this + iVar12 * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 0x256] == (lcd)0x0) || (this[uVar6 + 0x116] == (lcd)0x0)) &&
                 ((uVar8 & 3) != 0)) {
                puVar15[2] = *(undefined2 *)(this + (uVar8 & 3) * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 599] == (lcd)0x0) || (this[uVar6 + 0x117] == (lcd)0x0)) &&
                 ((uVar10 & 3) != 0)) {
                puVar15[3] = *(undefined2 *)(this + (uVar10 & 3) * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 600] == (lcd)0x0) || (this[uVar6 + 0x118] == (lcd)0x0)) &&
                 ((uVar13 & 3) != 0)) {
                puVar15[4] = *(undefined2 *)(this + (uVar13 & 3) * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 0x259] == (lcd)0x0) || (this[uVar6 + 0x119] == (lcd)0x0)) &&
                 ((uVar9 & 3) != 0)) {
                puVar15[5] = *(undefined2 *)(this + (uVar9 & 3) * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 0x25a] == (lcd)0x0) || (this[uVar6 + 0x11a] == (lcd)0x0)) &&
                 ((uVar14 & 3) != 0)) {
                puVar15[6] = *(undefined2 *)(this + (uVar14 & 3) * 2 + iVar17 * 8);
              }
              if (((this[uVar6 + 0x25b] == (lcd)0x0) || (this[uVar6 + 0x11b] == (lcd)0x0)) &&
                 ((uVar18 & 3) != 0)) {
                puVar15[7] = *(undefined2 *)(this + (uVar18 & 3) * 2 + iVar17 * 8);
              }
              goto joined_r0x0002bad4;
            }
            if ((this[uVar6 + 0x114] == (lcd)0x0) && (iVar7 != 0)) {
              *puVar15 = *(undefined2 *)(this + iVar7 * 2 + iVar17 * 8);
            }
            lVar3 = this[uVar6 + 0x115];
joined_r0x0002c20c:
            if ((lVar3 == (lcd)0x0) && (iVar12 != 0)) {
              puVar15[1] = *(undefined2 *)(this + iVar12 * 2 + iVar17 * 8);
            }
LAB_0002bf44:
            if ((this[uVar6 + 0x116] == (lcd)0x0) && ((uVar8 & 3) != 0)) {
              puVar15[2] = *(undefined2 *)(this + (uVar8 & 3) * 2 + iVar17 * 8);
            }
LAB_0002bf68:
            if ((this[uVar6 + 0x117] == (lcd)0x0) && ((uVar10 & 3) != 0)) {
              puVar15[3] = *(undefined2 *)(this + (uVar10 & 3) * 2 + iVar17 * 8);
            }
LAB_0002bf8c:
            if ((this[uVar6 + 0x118] == (lcd)0x0) && ((uVar13 & 3) != 0)) {
              puVar15[4] = *(undefined2 *)(this + (uVar13 & 3) * 2 + iVar17 * 8);
            }
LAB_0002bfb0:
            if ((this[uVar6 + 0x119] == (lcd)0x0) && ((uVar9 & 3) != 0)) {
              puVar15[5] = *(undefined2 *)(this + (uVar9 & 3) * 2 + iVar17 * 8);
            }
LAB_0002bfd4:
            if ((this[uVar6 + 0x11a] == (lcd)0x0) && ((uVar14 & 3) != 0)) {
              puVar15[6] = *(undefined2 *)(this + (uVar14 & 3) * 2 + iVar17 * 8);
            }
            lVar3 = this[uVar6 + 0x11b];
joined_r0x0002bec8:
            if (lVar3 != (lcd)0x0) goto joined_r0x0002bad4;
          }
          if ((uVar18 & 3) != 0) {
            puVar15[7] = *(undefined2 *)(this + (uVar18 & 3) * 2 + iVar17 * 8);
          }
        }
      }
    }
    else if (iVar7 < 0xa1) {
      iVar17 = *pbVar5 - 1;
      if (((iVar17 < 0xa0) && (param_2 <= iVar17)) && (iVar17 <= param_2 + 0xf)) {
        if ((param_2 - iVar17) + 0xf < 8) {
          if ((bVar2 & 0x40) == 0) {
            iVar16 = 0;
            iVar12 = (7U - (iVar17 - param_2) & 7) * 2 + (uVar14 & 0xfffffffe) * 0x10 + iVar12;
          }
          else {
            iVar16 = 0x10;
            iVar12 = (iVar17 - param_2 & 7U) * 2 + (uVar14 & 0xfffffffe) * 0x10 + iVar12;
          }
        }
        else if ((bVar2 & 0x40) == 0) {
          iVar16 = 0x10;
          iVar12 = (7U - (iVar17 - param_2) & 7) * 2 + (uVar14 & 0xfffffffe) * 0x10 + iVar12;
        }
        else {
          iVar16 = 0;
          iVar12 = (iVar17 - param_2 & 7U) * 2 + (uVar14 & 0xfffffffe) * 0x10 + iVar12;
        }
        uVar4 = *(ushort *)(iVar11 + 0x8012 + iVar16 + iVar12);
        goto LAB_0002bb54;
      }
    }
joined_r0x0002bad4:
    pbVar5 = pbVar5 + -4;
    if ((byte *)(iVar11 + 0xc08e) == pbVar5) {
      return;
    }
  } while( true );
}



/* ===== FUNCTION render @ 0002c244 ===== */

/* lcd::render(void*, int) */

void __thiscall lcd::render(lcd *this,void *param_1,int param_2)

{
  lcd lVar1;
  int iVar2;
  code *UNRECOVERED_JUMPTABLE;
  
  iVar2 = *(int *)(*(int *)(*(int *)(this + 0x3ac) + 0xc) + 0x20);
  *(undefined4 *)(this + 0x3a4) = 0;
  if (iVar2 < 3) {
    if (((this[0x3a8] != (lcd)0x0) && (this[0x3a9] != (lcd)0x0)) && (this[0x3aa] != (lcd)0x0)) {
      bg_render(this,param_1,param_2);
      win_render(this,param_1,param_2);
      UNRECOVERED_JUMPTABLE = sprite_render;
      goto LAB_0002c360;
    }
    memset((void *)((int)param_1 + param_2 * 0x140),0,0x140);
    if (this[0x3a8] != (lcd)0x0) {
      bg_render(this,param_1,param_2);
    }
    if (this[0x3a9] != (lcd)0x0) {
      win_render(this,param_1,param_2);
    }
    lVar1 = this[0x3aa];
    UNRECOVERED_JUMPTABLE = sprite_render;
  }
  else {
    if (((this[0x3a8] != (lcd)0x0) && (this[0x3a9] != (lcd)0x0)) && (this[0x3aa] != (lcd)0x0)) {
      bg_render_color(this,param_1,param_2);
      win_render_color(this,param_1,param_2);
      UNRECOVERED_JUMPTABLE = sprite_render_color;
      goto LAB_0002c360;
    }
    memset((void *)((int)param_1 + param_2 * 0x140),0,0x140);
    if (this[0x3a8] != (lcd)0x0) {
      bg_render_color(this,param_1,param_2);
    }
    if (this[0x3a9] != (lcd)0x0) {
      win_render_color(this,param_1,param_2);
    }
    lVar1 = this[0x3aa];
    UNRECOVERED_JUMPTABLE = sprite_render_color;
  }
  if (lVar1 == (lcd)0x0) {
    return;
  }
LAB_0002c360:
                    /* WARNING: Could not recover jumptable at 0x0002c378. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(this,param_1,param_2);
  return;
}



/* ===== FUNCTION serialize @ 0002c460 ===== */

/* lcd::serialize(serializer&) */

void __thiscall lcd::serialize(lcd *this,serializer *param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  int *piVar5;
  uint uVar6;
  undefined4 uVar7;
  code *pcVar8;
  
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar5 = *(int **)(param_1 + 4);
    iVar3 = *piVar5;
    *piVar5 = iVar3 + 8;
    *piVar5 = iVar3 + 0x18;
    *piVar5 = iVar3 + 0x98;
    *piVar5 = iVar3 + 0x118;
    *piVar5 = iVar3 + 0x11c;
    *piVar5 = iVar3 + 0x25c;
    *piVar5 = iVar3 + 0x39c;
    *piVar5 = iVar3 + 0x3a0;
    *piVar5 = iVar3 + 0x3a4;
    *piVar5 = iVar3 + 0x3a8;
    *piVar5 = iVar3 + 0x3ab;
    return;
  case 1:
    uVar2 = (uint)(this + 3) & 3;
    puVar4 = *(uint **)(param_1 + 4);
    uVar1 = (uint)this & 3;
    uVar6 = *(uint *)(this + 4);
    *puVar4 = (*(int *)(this + 3 + -uVar2) << (3 - uVar2) * 8 |
              (uint)this & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar1) * 8 |
              *(uint *)(this + -uVar1) >> uVar1 * 8;
    puVar4[1] = uVar6;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 8;
    uVar2 = *(uint *)param_1;
    goto joined_r0x0002c4d8;
  case 2:
    uVar7 = (*(undefined4 **)(param_1 + 4))[1];
    *(undefined4 *)this = **(undefined4 **)(param_1 + 4);
    *(undefined4 *)(this + 4) = uVar7;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 8;
    break;
  case 3:
    pcVar8 = fwrite;
    goto LAB_0002c4b8;
  case 4:
    pcVar8 = fread;
LAB_0002c4b8:
    (*pcVar8)(this,1,8,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  uVar2 = *(uint *)param_1;
joined_r0x0002c4d8:
  if (uVar2 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c4f8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter + *(int *)(&DAT_00035c14 + uVar2 * 4) + 0x7ccc))
              (this + 8);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c540. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c28 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x18);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c588. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c3c + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x98);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c5d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c50 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x118);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c618. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c64 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x11c);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c660. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c78 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x25c);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c6a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035c8c + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x39c);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c6f0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035ca0 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x3a0);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c738. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035cb4 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x3a4);
    return;
  }
  puts("ERROR: invalid serializer!");
  if (*(uint *)param_1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002c780. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter +
              *(int *)(&DAT_00035cc8 + *(uint *)param_1 * 4) + 0x7ccc))(this + 0x3a8);
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x0002c890. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  puts("ERROR: invalid serializer!");
  return;
}



/* ===== FUNCTION ~mbc @ 0002d090 ===== */

/* mbc::~mbc() */

void __thiscall mbc::~mbc(mbc *this)

{
  return;
}



/* ===== FUNCTION reset @ 0002d098 ===== */

/* mbc::reset() */

void __thiscall mbc::reset(mbc *this)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *(int *)(*(int *)(this + 0x28) + 0xc);
  *(undefined4 *)(iVar1 + 0x2c) = *(undefined4 *)(iVar1 + 0x24);
  *(undefined4 *)this = *(undefined4 *)(iVar1 + 0x24);
  uVar2 = *(undefined4 *)(iVar1 + 0x28);
  this[9] = (mbc)0x0;
  this[0x1a] = (mbc)0x0;
  this[0x1b] = (mbc)0x0;
  this[0x1d] = (mbc)0x0;
  this[0x20] = (mbc)0x0;
  this[0x21] = (mbc)0x0;
  this[0x24] = (mbc)0x0;
  this[0x26] = (mbc)0x0;
  *(undefined4 *)(this + 4) = uVar2;
  this[8] = (mbc)0x1;
  this[0x11] = (mbc)0x1;
  this[0x18] = (mbc)0x0;
  this[0x19] = (mbc)0x0;
  *(undefined2 *)(this + 0x1e) = 0;
  *(undefined2 *)(this + 0x22) = 0;
  this[0x25] = (mbc)0x1;
  if (*(int *)(iVar1 + 0x18) != 0xfd) {
    return;
  }
  this[0x11] = (mbc)0x0;
  return;
}



/* ===== FUNCTION mbc @ 0002d114 ===== */

/* mbc::mbc(gb*) */

void __thiscall mbc::mbc(mbc *this,gb *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  *(gb **)(this + 0x28) = param_1;
  iVar1 = *(int *)(*(int *)(this + 0x28) + 0xc);
  *(undefined4 *)(iVar1 + 0x2c) = *(undefined4 *)(iVar1 + 0x24);
  *(undefined4 *)this = *(undefined4 *)(iVar1 + 0x24);
  uVar2 = *(undefined4 *)(iVar1 + 0x28);
  this[9] = (mbc)0x0;
  this[0x1a] = (mbc)0x0;
  this[0x1b] = (mbc)0x0;
  this[0x1d] = (mbc)0x0;
  this[0x20] = (mbc)0x0;
  this[0x21] = (mbc)0x0;
  this[0x24] = (mbc)0x0;
  this[0x26] = (mbc)0x0;
  *(undefined4 *)(this + 4) = uVar2;
  this[8] = (mbc)0x1;
  this[0x11] = (mbc)0x1;
  this[0x18] = (mbc)0x0;
  this[0x19] = (mbc)0x0;
  *(undefined2 *)(this + 0x1e) = 0;
  *(undefined2 *)(this + 0x22) = 0;
  this[0x25] = (mbc)0x1;
  if (*(int *)(iVar1 + 0x18) == 0xfd) {
    this[0x11] = (mbc)0x0;
    return;
  }
  return;
}



/* ===== FUNCTION read @ 0002d12c ===== */

/* mbc::read(unsigned short) */

undefined4 mbc::read(ushort param_1)

{
  return 0;
}



/* ===== FUNCTION ext_read @ 0002d134 ===== */

/* mbc::ext_read(unsigned short) */

uint __thiscall mbc::ext_read(mbc *this,ushort param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  ushort uVar5;
  undefined4 uVar6;
  
  iVar4 = *(int *)(this + 0x28);
  iVar1 = *(int *)(*(int *)(iVar4 + 0xc) + 0x18);
  if (iVar1 != 0x22) {
    if (0x22 < iVar1) {
      if (1 < iVar1 - 0xfdU) {
        return 0;
      }
      return 1;
    }
    if (iVar1 - 0xfU < 5) {
      if (this[10] != (mbc)0x0) {
        switch((char)this[0x10] + -8) {
        case '\0':
          return (uint)(byte)this[0xb];
        case '\x01':
          return (uint)(byte)this[0xc];
        case '\x02':
          return (uint)(byte)this[0xd];
        case '\x03':
          return (uint)(byte)this[0xe];
        case '\x04':
          return (uint)(byte)this[0xf];
        }
      }
                    /* WARNING: Could not recover jumptable at 0x0002d270. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar3 = (**(code **)(**(int **)(iVar4 + 0x14) + 0x18))(*(int **)(iVar4 + 0x14));
      return uVar3;
    }
    return 0;
  }
  uVar5 = param_1 & 0xa0f0;
  if (uVar5 == 0xa040) {
    piVar2 = *(int **)(iVar4 + 0x14);
    uVar6 = 0;
LAB_0002d2c4:
    uVar3 = (**(code **)(*piVar2 + 0x20))(piVar2,uVar6);
    uVar3 = uVar3 & 0xff;
  }
  else {
    if (uVar5 < 0xa041) {
      if (uVar5 == 0xa010) {
        return 0;
      }
      if (uVar5 < 0xa011) {
        if (uVar5 == 0xa000) {
          return 0;
        }
        return 0xff;
      }
      if (uVar5 == 0xa020) {
        uVar6 = 1;
        piVar2 = *(int **)(iVar4 + 0x14);
        goto LAB_0002d2c4;
      }
      uVar6 = 1;
      if (uVar5 == 0xa030) {
        piVar2 = *(int **)(iVar4 + 0x14);
LAB_0002d23c:
        iVar1 = (**(code **)(*piVar2 + 0x20))(piVar2,uVar6);
        return iVar1 >> 8 & 0xf;
      }
    }
    else {
      if (uVar5 == 0xa060) {
        return 0;
      }
      if (uVar5 < 0xa061) {
        uVar6 = 0;
        if (uVar5 == 0xa050) {
          piVar2 = *(int **)(iVar4 + 0x14);
          goto LAB_0002d23c;
        }
      }
      else {
        if (uVar5 == 0xa070) {
          return 0;
        }
        if (uVar5 == 0xa080) {
          return (uint)(byte)this[0x20];
        }
      }
    }
    uVar3 = 0xff;
  }
  return uVar3;
}



/* ===== FUNCTION ext_write @ 0002d31c ===== */

/* mbc::ext_write(unsigned short, unsigned char) */

void __thiscall mbc::ext_write(mbc *this,ushort param_1,uchar param_2)

{
  mbc mVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  int iVar5;
  int *piVar6;
  uint uVar7;
  undefined2 *puVar8;
  undefined1 *puVar9;
  undefined2 *puVar10;
  undefined2 in_register_00000016;
  mbc mVar11;
  undefined3 in_register_00000019;
  int iVar12;
  
  iVar12 = *(int *)(*(int *)(this + 0x28) + 0xc);
  iVar5 = *(int *)(iVar12 + 0x18);
  if (iVar5 < 0xf) {
    return;
  }
  if (iVar5 < 0x14) {
    piVar6 = *(int **)(*(int *)(this + 0x28) + 0x14);
                    /* WARNING: Could not recover jumptable at 0x0002d374. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*piVar6 + 0x1c))(piVar6,this[0x10]);
    return;
  }
  if (iVar5 == 0x22) {
    if (CONCAT22(in_register_00000016,param_1) != 0xa080) {
      return;
    }
    mVar1 = this[0x1a];
    iVar5 = CONCAT31(in_register_00000019,param_2) >> 7;
    mVar11 = this[0x1b];
    uVar7 = CONCAT31(in_register_00000019,param_2) >> 6 & 1;
    this[0x1a] = SUB41(iVar5,0);
    this[0x1b] = SUB41(uVar7,0);
    if ((mVar1 == (mbc)0x0) && (iVar5 != 0)) {
      if (this[0x21] == (mbc)0x5) {
        if (this[0x18] != (mbc)0x0) {
          *(char *)(*(int *)(iVar12 + 0x28) + (uint)(byte)this[0x1d] * 2) =
               (char)((ushort)*(undefined2 *)(this + 0x22) >> 8);
          *(char *)(*(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) +
                    (uint)(byte)this[0x1d] * 2 + 1) = (char)*(undefined2 *)(this + 0x22);
          uVar7 = (uint)(byte)this[0x1b];
        }
        this[0x21] = (mbc)0x0;
        this[0x20] = (mbc)0x1;
      }
      else {
        this[0x21] = (mbc)0x0;
        this[0x19] = (mbc)0x1;
      }
    }
    if (mVar11 == (mbc)0x0) {
      if (uVar7 != 0) {
        if (this[0x19] == (mbc)0x0) {
          mVar1 = this[0x21];
          if (mVar1 == (mbc)0x2) {
            sVar4 = *(short *)(this + 0x22);
            mVar1 = this[0x24];
            uVar2 = (ushort)((param_2 & 2) >> 1);
            uVar3 = sVar4 << 1 | uVar2;
            this[0x24] = (mbc)((char)mVar1 + '\x01');
            *(ushort *)(this + 0x22) = uVar3;
            if ((mbc)((char)mVar1 + '\x01') != (mbc)0x8) {
              return;
            }
            this[0x24] = (mbc)0x0;
            this[0x1d] = SUB21(uVar3,0);
            this[0x21] = (mbc)0x3;
            if (this[0x1c] != (mbc)0x0) {
              return;
            }
            sVar4 = (short)(sVar4 << 1 & 0xffU | uVar2) >> 6;
            if (sVar4 == 0) {
              this[0x18] = (mbc)0x0;
              this[0x21] = (mbc)0x0;
              return;
            }
            if (sVar4 != 3) {
              return;
            }
            this[0x21] = (mbc)0x0;
            this[0x18] = (mbc)0x1;
            return;
          }
          if (mVar1 == (mbc)0x3) {
            mVar1 = this[0x1c];
            mVar11 = (mbc)((char)this[0x24] + '\x01');
            *(ushort *)(this + 0x22) = (ushort)((param_2 & 2) >> 1) | *(short *)(this + 0x22) << 1;
            this[0x24] = mVar11;
            if (mVar1 == (mbc)0x1) {
              if (mVar11 == (mbc)0x10) {
                this[0x20] = (mbc)0x0;
                this[0x24] = (mbc)0x0;
                this[0x21] = (mbc)0x5;
                return;
              }
            }
            else if (mVar1 == (mbc)0x0) {
              if (mVar11 == (mbc)0x10) {
                iVar5 = (int)(uint)(byte)this[0x1d] >> 6;
                if (iVar5 == 0) {
                  this[0x18] = (mbc)0x0;
                  this[0x21] = (mbc)0x0;
                }
                else {
                  if (iVar5 == 1) {
                    iVar5 = 0;
                    if (this[0x18] != (mbc)0x0) {
                      do {
                        *(char *)(*(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) + iVar5) =
                             (char)((ushort)*(undefined2 *)(this + 0x22) >> 8);
                        puVar9 = (undefined1 *)
                                 (*(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) + iVar5);
                        iVar5 = iVar5 + 2;
                        *puVar9 = (char)*(undefined2 *)(this + 0x22);
                      } while (iVar5 != 0x200);
                    }
                  }
                  else {
                    if (iVar5 != 2) {
                      this[0x21] = (mbc)0x0;
                      this[0x18] = (mbc)0x1;
                      goto LAB_0002d618;
                    }
                    if (this[0x18] != (mbc)0x0) {
                      puVar8 = *(undefined2 **)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28);
                      puVar10 = puVar8 + 0x100;
                      do {
                        *puVar8 = 0xffff;
                        puVar8 = puVar8 + 1;
                      } while (puVar10 != puVar8);
                    }
                  }
                  this[0x21] = (mbc)0x5;
                }
LAB_0002d618:
                this[0x24] = (mbc)0x0;
                return;
              }
            }
            else if (mVar1 == (mbc)0x2) {
              if (mVar11 == (mbc)0x1) {
                this[0x21] = (mbc)0x4;
                this[0x24] = (mbc)0x0;
                puVar9 = (undefined1 *)
                         (*(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) +
                         (uint)(byte)this[0x1d] * 2);
                *(ushort *)(this + 0x22) = CONCAT11(*puVar9,puVar9[1]);
                return;
              }
            }
            else if ((mVar1 == (mbc)0x3) && (mVar11 == (mbc)0x10)) {
              this[0x20] = (mbc)0x0;
              this[0x24] = (mbc)0x0;
              this[0x21] = (mbc)0x5;
              *(undefined2 *)(this + 0x22) = 0xffff;
              return;
            }
          }
          else if (mVar1 == (mbc)0x1) {
            mVar1 = this[0x24];
            uVar7 = (uint)*(ushort *)(this + 0x22) << 1 | (param_2 & 2) >> 1;
            this[0x24] = (mbc)((char)mVar1 + '\x01');
            *(short *)(this + 0x22) = (short)uVar7;
            if ((mbc)((char)mVar1 + '\x01') != (mbc)0x2) {
              return;
            }
            this[0x21] = (mbc)0x2;
            this[0x1c] = (mbc)((byte)uVar7 & 3);
            this[0x24] = (mbc)0x0;
            return;
          }
        }
        else if ((param_2 & 2) != 0) {
          this[0x19] = (mbc)0x0;
          this[0x24] = (mbc)0x0;
          this[0x21] = (mbc)0x1;
          return;
        }
      }
    }
    else if ((uVar7 == 0) && (this[0x21] == (mbc)0x4)) {
      this[0x20] = SUB21((ushort)*(short *)(this + 0x22) >> 0xf,0);
      *(short *)(this + 0x22) = *(short *)(this + 0x22) << 1;
      if ((mbc)((char)this[0x24] + '\x01') != (mbc)0x10) {
        this[0x24] = (mbc)((char)this[0x24] + '\x01');
        return;
      }
      this[0x21] = (mbc)0x0;
      this[0x24] = (mbc)0x0;
      return;
    }
  }
  return;
}



/* ===== FUNCTION get_state @ 0002d754 ===== */

/* mbc::get_state() */

uint __thiscall mbc::get_state(mbc *this)

{
  int iVar1;
  
  iVar1 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x18);
  if (iVar1 < 0x14) {
    if (0xe < iVar1) {
      return (byte)this[0x10] & 0xf | (uint)(byte)this[0xe] << 0x16 | ((byte)this[10] & 1) << 4 |
             ((byte)this[0xb] & 0x3f) << 5 | ((byte)this[0xc] & 0x3f) << 0xb |
             ((byte)this[0xd] & 0x1f) << 0x11 | ((byte)this[0xf] & 1) << 0x1e;
    }
    if (iVar1 - 1U < 3) {
      return (uint)CONCAT11(this[8] != (mbc)0x0,this[9]);
    }
  }
  else if (0x18 < iVar1) {
    if (iVar1 < 0x1f) {
      return *(uint *)(this + 0x14);
    }
    if (iVar1 == 0xff) {
      return (uint)CONCAT11(this[0x25] != (mbc)0x0,this[0x26]);
    }
  }
  return 0;
}



/* ===== FUNCTION set_state @ 0002d860 ===== */

/* mbc::set_state(int) */

void __thiscall mbc::set_state(mbc *this,int param_1)

{
  byte bVar1;
  int iVar2;
  mbc mVar3;
  
  iVar2 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x18);
  mVar3 = SUB41(param_1,0);
  bVar1 = (byte)((uint)param_1 >> 8);
  if (iVar2 < 0x14) {
    if (0xe < iVar2) {
      this[0x10] = (mbc)((byte)mVar3 & 0xf);
      this[10] = (mbc)((byte)(param_1 >> 4) & 1);
      this[0xb] = (mbc)((byte)(param_1 >> 5) & 0x3f);
      this[0xd] = (mbc)((byte)(param_1 >> 0x11) & 0x1f);
      this[0xe] = SUB41(param_1 >> 0x16,0);
      this[0xf] = (mbc)((byte)(param_1 >> 0x1e) & 1);
      this[0xc] = (mbc)((byte)(param_1 >> 0xb) & 0x3f);
      return;
    }
    if (iVar2 - 1U < 3) {
      this[9] = mVar3;
      this[8] = (mbc)(bVar1 & 1);
      return;
    }
  }
  else if (0x18 < iVar2) {
    if (iVar2 < 0x1f) {
      *(uint *)(this + 0x14) = param_1 & 0xffff;
      return;
    }
    if (iVar2 == 0xff) {
      this[0x26] = mVar3;
      this[0x25] = (mbc)(bVar1 & 1);
      return;
    }
  }
  return;
}



/* ===== FUNCTION set_page @ 0002d938 ===== */

/* mbc::set_page(int, int) */

void __thiscall mbc::set_page(mbc *this,int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(*(int *)(this + 0x28) + 0xc);
  *(int *)this = *(int *)(iVar1 + 0x2c) + param_1 * 0x4000;
  *(int *)(this + 4) = *(int *)(iVar1 + 0x28) + param_2 * 0x2000;
  return;
}



/* ===== FUNCTION mbc1_write @ 0002d964 ===== */

/* mbc::mbc1_write(unsigned short, unsigned char) */

void __thiscall mbc::mbc1_write(mbc *this,ushort param_1,uchar param_2)

{
  undefined2 in_register_00000016;
  int iVar1;
  undefined3 in_register_00000019;
  uint uVar2;
  
  iVar1 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (this[8] == (mbc)0x0) {
    if (iVar1 == 2) {
      *(uint *)(this + 4) =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) + (param_2 & 3) * 0x2000;
      return;
    }
    if (iVar1 == 3) {
      if ((param_2 & 1) == 0) {
        this[8] = (mbc)0x1;
        return;
      }
    }
    else if (iVar1 == 1) {
      uVar2 = 1;
      if (CONCAT31(in_register_00000019,param_2) != 0) {
        uVar2 = param_2 & 0x1f;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
  }
  else {
    if (iVar1 == 2) {
      uVar2 = (param_2 & 3) << 5 | (byte)this[9] & 0x1f;
      this[9] = SUB41(uVar2,0);
      if (uVar2 == 0) {
        uVar2 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
    if (iVar1 == 3) {
      if ((param_2 & 1) != 0) {
        this[8] = (mbc)0x0;
        return;
      }
    }
    else if (iVar1 == 1) {
      uVar2 = (byte)this[9] & 0x60 | param_2 & 0x1f;
      this[9] = SUB41(uVar2,0);
      if (uVar2 == 0) {
        uVar2 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
  }
  return;
}



/* ===== FUNCTION mbc2_write @ 0002db18 ===== */

/* mbc::mbc2_write(unsigned short, unsigned char) */

void __thiscall mbc::mbc2_write(mbc *this,ushort param_1,uchar param_2)

{
  int iVar1;
  undefined2 in_register_00000016;
  
  if (CONCAT22(in_register_00000016,param_1) - 0x2000U < 0x2000) {
    iVar1 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c);
    if ((param_2 & 0xf) == 0) {
      *(int *)this = iVar1;
      return;
    }
    *(uint *)this = iVar1 + ((param_2 & 0xf) - 1) * 0x4000;
  }
  return;
}



/* ===== FUNCTION mbc3_write @ 0002db64 ===== */

/* mbc::mbc3_write(unsigned short, unsigned char) */

void __thiscall mbc::mbc3_write(mbc *this,ushort param_1,uchar param_2)

{
  byte bVar1;
  mbc mVar2;
  undefined2 in_register_00000016;
  uint uVar3;
  int iVar4;
  undefined3 in_register_00000019;
  
  iVar4 = CONCAT31(in_register_00000019,param_2);
  uVar3 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (uVar3 == 1) {
    uVar3 = 1;
    if (iVar4 != 0) {
      uVar3 = param_2 & 0x7f;
    }
    *(uint *)this =
         *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
         (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4)
          - 1U & uVar3) * 0x4000 + -0x4000;
    return;
  }
  if (uVar3 < 2) {
    if (uVar3 == 0) {
      if (iVar4 != 10) {
        this[0x10] = (mbc)0x0;
        this[0x11] = (mbc)0x0;
        return;
      }
      this[0x11] = (mbc)0x1;
    }
  }
  else {
    if (uVar3 == 2) {
      if (iVar4 < 8) {
        bVar1 = *(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1d);
        iVar4 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28);
        this[0x11] = (mbc)0x1;
        *(uint *)(this + 4) =
             iVar4 + (*(int *)(ram_size_tbl + (uint)bVar1 * 4) - 1U & param_2 & 7) * 0x2000;
        return;
      }
      this[0x11] = (mbc)0x0;
      this[0x10] = (mbc)(param_2 & 0xf);
      return;
    }
    if (uVar3 == 3) {
      if (iVar4 == 0) {
        this[10] = (mbc)0x0;
      }
      else if (iVar4 == 1) {
        if (this[10] == (mbc)0x0) {
          mVar2 = (mbc)(**(code **)(**(int **)(*(int *)(this + 0x28) + 0x14) + 0x18))
                                 (*(int **)(*(int *)(this + 0x28) + 0x14),8);
          this[0xb] = mVar2;
          mVar2 = (mbc)(**(code **)(**(int **)(*(int *)(this + 0x28) + 0x14) + 0x18))
                                 (*(int **)(*(int *)(this + 0x28) + 0x14),9);
          this[0xc] = mVar2;
          mVar2 = (mbc)(**(code **)(**(int **)(*(int *)(this + 0x28) + 0x14) + 0x18))
                                 (*(int **)(*(int *)(this + 0x28) + 0x14),10);
          this[0xd] = mVar2;
          mVar2 = (mbc)(**(code **)(**(int **)(*(int *)(this + 0x28) + 0x14) + 0x18))
                                 (*(int **)(*(int *)(this + 0x28) + 0x14),0xb);
          this[0xe] = mVar2;
          mVar2 = (mbc)(**(code **)(**(int **)(*(int *)(this + 0x28) + 0x14) + 0x18))
                                 (*(int **)(*(int *)(this + 0x28) + 0x14),0xc);
          this[0xf] = mVar2;
          this[10] = (mbc)0x1;
        }
        else {
          this[10] = (mbc)0x1;
        }
      }
    }
  }
  return;
}



/* ===== FUNCTION mbc5_write @ 0002dd94 ===== */

/* mbc::mbc5_write(unsigned short, unsigned char) */

void __thiscall mbc::mbc5_write(mbc *this,ushort param_1,uchar param_2)

{
  undefined2 in_register_00000016;
  uint uVar1;
  int iVar2;
  undefined3 in_register_00000019;
  uint uVar3;
  int iVar4;
  
  uVar3 = CONCAT31(in_register_00000019,param_2);
  uVar1 = CONCAT22(in_register_00000016,param_1) >> 0xc;
  if (uVar1 == 3) {
    uVar1 = (uint)(byte)this[0x14];
    uVar3 = (param_2 & 1) << 8;
    iVar2 = *(int *)(*(int *)(this + 0x28) + 0xc);
LAB_0002ddd4:
    *(uint *)(this + 0x14) = uVar1 | uVar3;
    *(uint *)this =
         *(int *)(iVar2 + 0x2c) +
         (*(int *)(rom_size_tbl + (uint)*(byte *)(iVar2 + 0x1c) * 4) - 1U & (uVar1 | uVar3)) *
         0x4000 + -0x4000;
    return;
  }
  if (uVar1 < 4) {
    if (uVar1 == 2) {
      iVar2 = *(int *)(*(int *)(this + 0x28) + 0xc);
      uVar1 = *(uint *)(this + 0x14) & 0x100;
      goto LAB_0002ddd4;
    }
  }
  else if (uVar1 < 6) {
    iVar4 = *(int *)(this + 0x28);
    iVar2 = *(int *)(iVar4 + 0xc);
    if (2 < *(int *)(iVar2 + 0x18) - 0x1cU) {
      *(uint *)(this + 4) =
           *(int *)(iVar2 + 0x28) +
           (*(int *)(ram_size_tbl + (uint)*(byte *)(iVar2 + 0x1d) * 4) - 1U & param_2 & 0xf) *
           0x2000;
      return;
    }
    *(uint *)(this + 4) =
         *(int *)(iVar2 + 0x28) +
         (*(int *)(ram_size_tbl + (uint)*(byte *)(iVar2 + 0x1d) * 4) - 1U & param_2 & 7) * 0x2000;
    if ((param_2 & 8) != 0) {
                    /* WARNING: Could not recover jumptable at 0x0002ded4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(**(int **)(iVar4 + 0x14) + 0x24))(*(int **)(iVar4 + 0x14),1);
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x0002def4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(**(int **)(iVar4 + 0x14) + 0x24))(*(int **)(iVar4 + 0x14),0);
    return;
  }
  return;
}



/* ===== FUNCTION mbc7_write @ 0002defc ===== */

/* mbc::mbc7_write(unsigned short, unsigned char) */

void __thiscall mbc::mbc7_write(mbc *this,ushort param_1,uchar param_2)

{
  int iVar1;
  undefined2 in_register_00000016;
  uint uVar2;
  undefined3 in_register_00000019;
  
  uVar2 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (uVar2 == 1) {
    uVar2 = 1;
    if (CONCAT31(in_register_00000019,param_2) != 0) {
      uVar2 = param_2 & 0x7f;
    }
    *(uint *)this =
         *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
         (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4)
          - 1U & uVar2) * 0x4000 + -0x4000;
    return;
  }
  if (uVar2 == 2) {
    if (7 < CONCAT31(in_register_00000019,param_2)) {
      this[0x11] = (mbc)0x0;
      return;
    }
    iVar1 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28);
    this[0x11] = (mbc)0x0;
    *(uint *)(this + 4) = iVar1 + (param_2 & 3) * 0x2000;
    return;
  }
  return;
}



/* ===== FUNCTION huc1_write @ 0002dfb0 ===== */

/* mbc::huc1_write(unsigned short, unsigned char) */

void __thiscall mbc::huc1_write(mbc *this,ushort param_1,uchar param_2)

{
  undefined2 in_register_00000016;
  int iVar1;
  undefined3 in_register_00000019;
  uint uVar2;
  
  iVar1 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (this[0x25] == (mbc)0x0) {
    if (iVar1 == 2) {
      *(uint *)(this + 4) =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28) + (param_2 & 3) * 0x2000;
      return;
    }
    if (iVar1 == 3) {
      if ((param_2 & 1) == 0) {
        this[0x25] = (mbc)0x1;
      }
      goto LAB_0002e144;
    }
    if (iVar1 == 1) {
      uVar2 = 1;
      if (CONCAT31(in_register_00000019,param_2) != 0) {
        uVar2 = param_2 & 0x3f;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
  }
  else {
    if (iVar1 == 2) {
      uVar2 = (param_2 & 3) * 0x20 + ((byte)this[0x26] & 0x3f);
      this[0x26] = SUB41(uVar2,0);
      if (uVar2 == 0) {
        uVar2 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
    if (iVar1 == 3) {
      if ((param_2 & 1) != 0) {
        this[0x25] = (mbc)0x0;
      }
LAB_0002e144:
      this[0x26] = (mbc)0x0;
      return;
    }
    if (iVar1 == 1) {
      uVar2 = ((byte)this[0x26] & 0x60) + (param_2 & 0x3f);
      this[0x26] = SUB41(uVar2,0);
      if (uVar2 == 0) {
        uVar2 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar2)
           * 0x4000 + -0x4000;
      return;
    }
  }
  return;
}



/* ===== FUNCTION huc3_write @ 0002e160 ===== */

/* mbc::huc3_write(unsigned short, unsigned char) */

void __thiscall mbc::huc3_write(mbc *this,ushort param_1,uchar param_2)

{
  int iVar1;
  undefined2 in_register_00000016;
  uint uVar2;
  undefined3 in_register_00000019;
  
  iVar1 = CONCAT31(in_register_00000019,param_2);
  uVar2 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (uVar2 == 1) {
    uVar2 = 1;
    if (iVar1 != 0) {
      uVar2 = param_2 & 0x7f;
    }
    *(uint *)this =
         *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
         (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4)
          - 1U & uVar2) * 0x4000 + -0x4000;
    return;
  }
  if (uVar2 == 2) {
    if (iVar1 < 8) {
      iVar1 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28);
      this[0x11] = (mbc)0x1;
      *(uint *)(this + 4) = iVar1 + (param_2 & 3) * 0x2000;
      return;
    }
  }
  else if (uVar2 == 0) {
    if (iVar1 != 10) {
      this[0x11] = (mbc)0x0;
      return;
    }
    this[0x11] = (mbc)0x1;
    return;
  }
  return;
}



/* ===== FUNCTION tama5_write @ 0002e230 ===== */

/* mbc::tama5_write(unsigned short, unsigned char) */

void mbc::tama5_write(ushort param_1,uchar param_2)

{
  return;
}



/* ===== FUNCTION mmm01_write @ 0002e238 ===== */

/* mbc::mmm01_write(unsigned short, unsigned char) */

void __thiscall mbc::mmm01_write(mbc *this,ushort param_1,uchar param_2)

{
  int iVar1;
  undefined2 in_register_00000016;
  int iVar2;
  undefined3 in_register_00000019;
  uint uVar3;
  
  uVar3 = CONCAT31(in_register_00000019,param_2);
  iVar2 = CONCAT22(in_register_00000016,param_1) >> 0xd;
  if (this[8] == (mbc)0x0) {
    if (iVar2 == 2) {
      iVar2 = (param_2 & 3) * 0x40000;
      iVar1 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x24) + iVar2;
      *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) = iVar1;
      this[9] = SUB41(param_2 & 3,0);
      *(int *)this = iVar1 + iVar2;
      return;
    }
    if (iVar2 == 3) {
      if ((param_2 & 1) == 0) {
        this[8] = (mbc)0x1;
        return;
      }
    }
    else if (iVar2 == 1) {
      if (uVar3 == 0) {
        uVar3 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (uVar3 & *(int *)(rom_size_tbl +
                            (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U
           & 0xf) * 0x4000 + -0x4000;
      return;
    }
  }
  else {
    if (iVar2 == 2) {
      uVar3 = (param_2 & 3) << 5 | (byte)this[9] & 0x1f;
      this[9] = SUB41(uVar3,0);
      if (uVar3 == 0) {
        uVar3 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar3)
           * 0x4000 + -0x4000;
      return;
    }
    if (iVar2 == 3) {
      if ((param_2 & 1) != 0) {
        this[8] = (mbc)0x0;
      }
      this[9] = (mbc)0x0;
      return;
    }
    if (iVar2 == 1) {
      uVar3 = (byte)this[9] & 0x60 | param_2 & 0x1f;
      this[9] = SUB41(uVar3,0);
      if (uVar3 == 0) {
        uVar3 = 1;
      }
      *(uint *)this =
           *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c) +
           (*(int *)(rom_size_tbl +
                    (uint)*(byte *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x1c) * 4) - 1U & uVar3)
           * 0x4000 + -0x4000;
      return;
    }
  }
  return;
}



/* ===== FUNCTION write @ 0002e3f4 ===== */

/* mbc::write(unsigned short, unsigned char) */

void mbc::write(ushort param_1,uchar param_2)

{
  byte bVar1;
  uint3 uVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  undefined2 in_register_00000012;
  int *piVar6;
  int3 iVar7;
  uint3 in_register_00000015;
  uint uVar8;
  uint in_a2;
  undefined4 in_a3;
  
  piVar6 = (int *)CONCAT22(in_register_00000012,param_1);
  iVar4 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x18);
  iVar7 = (int3)in_register_00000015 >> 5;
  if (iVar4 < 0x1f) {
    if (0x18 < iVar4) {
      uVar2 = in_register_00000015 >> 4;
      if (uVar2 == 3) {
        uVar8 = (uint)*(byte *)(piVar6 + 5);
        in_a2 = (in_a2 & 1) << 8;
        iVar4 = *(int *)(piVar6[10] + 0xc);
LAB_0002ddd4:
        piVar6[5] = uVar8 | in_a2;
        *piVar6 = *(int *)(iVar4 + 0x2c) +
                  (*(int *)(rom_size_tbl + (uint)*(byte *)(iVar4 + 0x1c) * 4) - 1U & (uVar8 | in_a2)
                  ) * 0x4000 + -0x4000;
        return;
      }
      if (uVar2 < 4) {
        if (uVar2 == 2) {
          iVar4 = *(int *)(piVar6[10] + 0xc);
          uVar8 = piVar6[5] & 0x100;
          goto LAB_0002ddd4;
        }
      }
      else if (uVar2 < 6) {
        iVar5 = piVar6[10];
        iVar4 = *(int *)(iVar5 + 0xc);
        if (2 < *(int *)(iVar4 + 0x18) - 0x1cU) {
          piVar6[1] = *(int *)(iVar4 + 0x28) +
                      (*(int *)(ram_size_tbl + (uint)*(byte *)(iVar4 + 0x1d) * 4) - 1U & in_a2 & 0xf
                      ) * 0x2000;
          return;
        }
        piVar6[1] = *(int *)(iVar4 + 0x28) +
                    (*(int *)(ram_size_tbl + (uint)*(byte *)(iVar4 + 0x1d) * 4) - 1U & in_a2 & 7) *
                    0x2000;
        if ((in_a2 & 8) == 0) {
                    /* WARNING: Could not recover jumptable at 0x0002def4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (**(code **)(**(int **)(iVar5 + 0x14) + 0x24))(*(int **)(iVar5 + 0x14),0);
          return;
        }
                    /* WARNING: Could not recover jumptable at 0x0002ded4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(**(int **)(iVar5 + 0x14) + 0x24))(*(int **)(iVar5 + 0x14),1);
        return;
      }
      return;
    }
    if (iVar4 < 7) {
      if (4 < iVar4) {
        if (CONCAT31(in_register_00000015,param_2) - 0x2000U < 0x2000) {
          iVar4 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c);
          if ((in_a2 & 0xf) == 0) {
            *piVar6 = iVar4;
            return;
          }
          *piVar6 = iVar4 + ((in_a2 & 0xf) - 1) * 0x4000;
        }
        return;
      }
      if (iVar4 - 1U < 3) {
        if ((char)piVar6[2] == '\0') {
          if (iVar7 == 2) {
            piVar6[1] = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x28) + (in_a2 & 3) * 0x2000;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) == 0) {
              *(undefined1 *)(piVar6 + 2) = 1;
              return;
            }
          }
          else if (iVar7 == 1) {
            uVar8 = 1;
            if (in_a2 != 0) {
              uVar8 = in_a2 & 0x1f;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
        }
        else {
          if (iVar7 == 2) {
            uVar8 = (in_a2 & 3) << 5 | *(byte *)((int)piVar6 + 9) & 0x1f;
            *(char *)((int)piVar6 + 9) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) != 0) {
              *(undefined1 *)(piVar6 + 2) = 0;
              return;
            }
          }
          else if (iVar7 == 1) {
            uVar8 = *(byte *)((int)piVar6 + 9) & 0x60 | in_a2 & 0x1f;
            *(char *)((int)piVar6 + 9) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
        }
        return;
      }
    }
    else if (iVar4 - 0xfU < 5) {
      uVar2 = in_register_00000015 >> 5;
      if (uVar2 == 1) {
        uVar8 = 1;
        if (in_a2 != 0) {
          uVar8 = in_a2 & 0x7f;
        }
        *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                  (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) -
                   1U & uVar8) * 0x4000 + -0x4000;
        return;
      }
      if (uVar2 < 2) {
        if (uVar2 == 0) {
          if (in_a2 != 10) {
            *(undefined1 *)(piVar6 + 4) = 0;
            *(undefined1 *)((int)piVar6 + 0x11) = 0;
            return;
          }
          *(undefined1 *)((int)piVar6 + 0x11) = 1;
        }
      }
      else {
        if (uVar2 == 2) {
          if (7 < (int)in_a2) {
            *(undefined1 *)((int)piVar6 + 0x11) = 0;
            *(byte *)(piVar6 + 4) = (byte)in_a2 & 0xf;
            return;
          }
          bVar1 = *(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1d);
          iVar4 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x28);
          *(undefined1 *)((int)piVar6 + 0x11) = 1;
          piVar6[1] = iVar4 + (*(int *)(ram_size_tbl + (uint)bVar1 * 4) - 1U & in_a2 & 7) * 0x2000;
          return;
        }
        if (uVar2 == 3) {
          if (in_a2 == 0) {
            *(undefined1 *)((int)piVar6 + 10) = 0;
          }
          else if (in_a2 == 1) {
            if (*(char *)((int)piVar6 + 10) == '\0') {
              uVar3 = (**(code **)(**(int **)(piVar6[10] + 0x14) + 0x18))
                                (*(int **)(piVar6[10] + 0x14),8,1,in_a3,0x50250);
              *(undefined1 *)((int)piVar6 + 0xb) = uVar3;
              uVar3 = (**(code **)(**(int **)(piVar6[10] + 0x14) + 0x18))
                                (*(int **)(piVar6[10] + 0x14),9);
              *(undefined1 *)(piVar6 + 3) = uVar3;
              uVar3 = (**(code **)(**(int **)(piVar6[10] + 0x14) + 0x18))
                                (*(int **)(piVar6[10] + 0x14),10);
              *(undefined1 *)((int)piVar6 + 0xd) = uVar3;
              uVar3 = (**(code **)(**(int **)(piVar6[10] + 0x14) + 0x18))
                                (*(int **)(piVar6[10] + 0x14),0xb);
              *(undefined1 *)((int)piVar6 + 0xe) = uVar3;
              uVar3 = (**(code **)(**(int **)(piVar6[10] + 0x14) + 0x18))
                                (*(int **)(piVar6[10] + 0x14),0xc);
              *(undefined1 *)((int)piVar6 + 0xf) = uVar3;
              *(undefined1 *)((int)piVar6 + 10) = 1;
            }
            else {
              *(undefined1 *)((int)piVar6 + 10) = 1;
            }
          }
        }
      }
      return;
    }
  }
  else {
    if (iVar4 == 0xfe) {
      uVar2 = in_register_00000015 >> 5;
      if (uVar2 == 1) {
        uVar8 = 1;
        if (in_a2 != 0) {
          uVar8 = in_a2 & 0x7f;
        }
        *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                  (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) -
                   1U & uVar8) * 0x4000 + -0x4000;
        return;
      }
      if (uVar2 == 2) {
        if ((int)in_a2 < 8) {
          iVar4 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x28);
          *(undefined1 *)((int)piVar6 + 0x11) = 1;
          piVar6[1] = iVar4 + (in_a2 & 3) * 0x2000;
          return;
        }
      }
      else if (uVar2 == 0) {
        if (in_a2 == 10) {
          *(undefined1 *)((int)piVar6 + 0x11) = 1;
          return;
        }
        *(undefined1 *)((int)piVar6 + 0x11) = 0;
        return;
      }
      return;
    }
    if (iVar4 < 0xff) {
      if (iVar4 == 0x22) {
        if (in_register_00000015 >> 5 == 1) {
          uVar8 = 1;
          if (in_a2 != 0) {
            uVar8 = in_a2 & 0x7f;
          }
          *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                    (*(int *)(rom_size_tbl + (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4)
                     - 1U & uVar8) * 0x4000 + -0x4000;
          return;
        }
        if (in_register_00000015 >> 5 == 2) {
          if ((int)in_a2 < 8) {
            iVar4 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x28);
            *(undefined1 *)((int)piVar6 + 0x11) = 0;
            piVar6[1] = iVar4 + (in_a2 & 3) * 0x2000;
            return;
          }
          *(undefined1 *)((int)piVar6 + 0x11) = 0;
          return;
        }
        return;
      }
      if (iVar4 == 0xfd) {
        return;
      }
    }
    else {
      if (iVar4 == 0xff) {
        if (*(char *)((int)piVar6 + 0x25) == '\0') {
          if (iVar7 == 2) {
            piVar6[1] = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x28) + (in_a2 & 3) * 0x2000;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) == 0) {
              *(undefined1 *)((int)piVar6 + 0x25) = 1;
            }
            goto LAB_0002e144;
          }
          if (iVar7 == 1) {
            uVar8 = 1;
            if (in_a2 != 0) {
              uVar8 = in_a2 & 0x3f;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
        }
        else {
          if (iVar7 == 2) {
            uVar8 = (in_a2 & 3) * 0x20 + (*(byte *)((int)piVar6 + 0x26) & 0x3f);
            *(char *)((int)piVar6 + 0x26) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) != 0) {
              *(undefined1 *)((int)piVar6 + 0x25) = 0;
            }
LAB_0002e144:
            *(undefined1 *)((int)piVar6 + 0x26) = 0;
            return;
          }
          if (iVar7 == 1) {
            uVar8 = (*(byte *)((int)piVar6 + 0x26) & 0x60) + (in_a2 & 0x3f);
            *(char *)((int)piVar6 + 0x26) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
        }
        return;
      }
      if (iVar4 == 0x100) {
        if ((char)piVar6[2] == '\0') {
          if (iVar7 == 2) {
            iVar4 = (in_a2 & 3) * 0x40000;
            iVar5 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x24) + iVar4;
            *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) = iVar5;
            *(char *)((int)piVar6 + 9) = (char)(in_a2 & 3);
            *piVar6 = iVar5 + iVar4;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) == 0) {
              *(undefined1 *)(piVar6 + 2) = 1;
              return;
            }
          }
          else if (iVar7 == 1) {
            if (in_a2 == 0) {
              in_a2 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (in_a2 & *(int *)(rom_size_tbl +
                                       (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U
                      & 0xf) * 0x4000 + -0x4000;
            return;
          }
        }
        else {
          if (iVar7 == 2) {
            uVar8 = (in_a2 & 3) << 5 | *(byte *)((int)piVar6 + 9) & 0x1f;
            *(char *)((int)piVar6 + 9) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
          if (iVar7 == 3) {
            if ((in_a2 & 1) != 0) {
              *(undefined1 *)(piVar6 + 2) = 0;
            }
            *(undefined1 *)((int)piVar6 + 9) = 0;
            return;
          }
          if (iVar7 == 1) {
            uVar8 = *(byte *)((int)piVar6 + 9) & 0x60 | in_a2 & 0x1f;
            *(char *)((int)piVar6 + 9) = (char)uVar8;
            if (uVar8 == 0) {
              uVar8 = 1;
            }
            *piVar6 = *(int *)(*(int *)(piVar6[10] + 0xc) + 0x2c) +
                      (*(int *)(rom_size_tbl +
                               (uint)*(byte *)(*(int *)(piVar6[10] + 0xc) + 0x1c) * 4) - 1U & uVar8)
                      * 0x4000 + -0x4000;
            return;
          }
        }
        return;
      }
    }
  }
  return;
}



/* ===== FUNCTION serialize @ 0002e4f8 ===== */

/* mbc::serialize(serializer&) */

void __thiscall mbc::serialize(mbc *this,serializer *param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  mbc *pmVar5;
  int iVar6;
  int iVar7;
  code *pcVar8;
  int local_20 [3];
  
  iVar7 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x2c);
  iVar6 = *(int *)(*(int *)(*(int *)(this + 0x28) + 0xc) + 0x28);
  local_20[0] = *(int *)this - iVar7;
  if (local_20[0] < 0) {
    local_20[0] = local_20[0] + 0x3fff;
  }
  local_20[0] = local_20[0] >> 0xe;
  switch(*(undefined4 *)param_1) {
  case 0:
    iVar1 = *(int *)(this + 4) - iVar6;
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    if (iVar1 < 0) {
      iVar1 = iVar1 + 0x1fff;
    }
    iVar3 = local_20[0] * 0x4000;
    local_20[0] = iVar1 >> 0xd;
    *(int *)this = iVar7 + iVar3;
    goto switchD_0002e5ec_caseD_2f8c0;
  case 1:
    **(int **)(param_1 + 4) = local_20[0];
    uVar4 = *(uint *)param_1;
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    *(int *)this = iVar7 + local_20[0] * 0x4000;
    local_20[0] = *(int *)(this + 4) - iVar6;
    if (local_20[0] < 0) {
      local_20[0] = local_20[0] + 0x1fff;
    }
    local_20[0] = local_20[0] >> 0xd;
    if (uVar4 < 5) goto LAB_0002e5d4;
    goto LAB_0002f928;
  case 2:
    iVar1 = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = *(int **)(param_1 + 4) + 1;
    *(int *)this = iVar7 + iVar1 * 0x4000;
    goto switchD_0002e5ec_caseD_2f97c;
  case 3:
    pcVar8 = fwrite;
    break;
  case 4:
    pcVar8 = fread;
    break;
  default:
    puts("ERROR: invalid serializer!");
    uVar4 = *(uint *)param_1;
    goto LAB_0002e5a0;
  }
  (*pcVar8)(local_20,1,4,*(undefined4 *)(param_1 + 4));
  uVar4 = *(uint *)param_1;
LAB_0002e5a0:
  iVar1 = *(int *)(this + 4) - iVar6;
  if (iVar1 < 0) {
    iVar1 = iVar1 + 0x1fff;
  }
  *(int *)this = iVar7 + local_20[0] * 0x4000;
  local_20[0] = iVar1 >> 0xd;
  if (4 < uVar4) {
LAB_0002f928:
    puts("ERROR: invalid serializer!");
    goto LAB_0002e610;
  }
LAB_0002e5d4:
  switch((&switchD_0002e5ec::switchdataD_00035d08)[uVar4] + 0x50250 & 0xfffffffe) {
  case 0x2e5f4:
    pcVar8 = fwrite;
    goto LAB_0002e5f8;
  case 0x2f854:
    **(int **)(param_1 + 4) = local_20[0];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    break;
  case 0x2f874:
    pcVar8 = fread;
LAB_0002e5f8:
    (*pcVar8)(local_20,1,4,*(undefined4 *)(param_1 + 4));
    break;
  case 0x2f8c0:
switchD_0002e5ec_caseD_2f8c0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 0x2f97c:
switchD_0002e5ec_caseD_2f97c:
    local_20[0] = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = *(int **)(param_1 + 4) + 1;
  }
LAB_0002e610:
  *(int *)(this + 4) = iVar6 + local_20[0] * 0x2000;
  local_20[0] = get_state(this);
  switch(*(undefined4 *)param_1) {
  case 0:
    **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 4;
    break;
  case 1:
    **(int **)(param_1 + 4) = local_20[0];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
    break;
  case 2:
    local_20[0] = **(int **)(param_1 + 4);
    *(int **)(param_1 + 4) = *(int **)(param_1 + 4) + 1;
    break;
  case 3:
    pcVar8 = fwrite;
    goto LAB_0002e664;
  case 4:
    pcVar8 = fread;
LAB_0002e664:
    (*pcVar8)(local_20,1,4,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  set_state(this,local_20[0]);
  switch(*(undefined4 *)param_1) {
  case 0:
    piVar2 = *(int **)(param_1 + 4);
    iVar6 = *piVar2;
    *piVar2 = iVar6 + 1;
    iVar6 = iVar6 + 2;
    *piVar2 = iVar6;
    goto LAB_0002f728;
  case 1:
    **(mbc **)(param_1 + 4) = this[0x11];
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    break;
  case 2:
    this[0x11] = **(mbc **)(param_1 + 4);
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
    uVar4 = *(uint *)param_1;
    goto joined_r0x0002f6d4;
  case 3:
    pcVar8 = fwrite;
    goto LAB_0002e6c4;
  case 4:
    pcVar8 = fread;
LAB_0002e6c4:
    (*pcVar8)(this + 0x11,1,1,*(undefined4 *)(param_1 + 4));
    break;
  default:
    puts("ERROR: invalid serializer!");
  }
  uVar4 = *(uint *)param_1;
joined_r0x0002f6d4:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f728:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f730;
    case 1:
      **(mbc **)(param_1 + 4) = this[8];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e720;
    case 2:
      this[8] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f670;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 8,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e720:
  uVar4 = *(uint *)param_1;
joined_r0x0002f670:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f730:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f738;
    case 1:
      **(mbc **)(param_1 + 4) = this[9];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e768;
    case 2:
      this[9] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f60c;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 9,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e768:
  uVar4 = *(uint *)param_1;
joined_r0x0002f60c:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f738:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f740;
    case 1:
      **(mbc **)(param_1 + 4) = this[10];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e7b0;
    case 2:
      this[10] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f5a8;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 10,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e7b0:
  uVar4 = *(uint *)param_1;
joined_r0x0002f5a8:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f740:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f748;
    case 1:
      **(mbc **)(param_1 + 4) = this[0xb];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e7f8;
    case 2:
      this[0xb] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f544;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0xb,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e7f8:
  uVar4 = *(uint *)param_1;
joined_r0x0002f544:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f748:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f750;
    case 1:
      **(mbc **)(param_1 + 4) = this[0xc];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e840;
    case 2:
      this[0xc] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f4e0;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0xc,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e840:
  uVar4 = *(uint *)param_1;
joined_r0x0002f4e0:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f750:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f758;
    case 1:
      **(mbc **)(param_1 + 4) = this[0xd];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e888;
    case 2:
      this[0xd] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f47c;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0xd,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e888:
  uVar4 = *(uint *)param_1;
joined_r0x0002f47c:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f758:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f760;
    case 1:
      **(mbc **)(param_1 + 4) = this[0xe];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e8d0;
    case 2:
      this[0xe] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f418;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0xe,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e8d0:
  uVar4 = *(uint *)param_1;
joined_r0x0002f418:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f760:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f768;
    case 1:
      **(mbc **)(param_1 + 4) = this[0xf];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e918;
    case 2:
      this[0xf] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f3b4;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0xf,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e918:
  uVar4 = *(uint *)param_1;
joined_r0x0002f3b4:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f768:
      iVar6 = iVar6 + 4;
      *piVar2 = iVar6;
      goto LAB_0002f770;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x10];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e960;
    case 2:
      this[0x10] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f350;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x10,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e960:
  uVar4 = *(uint *)param_1;
joined_r0x0002f350:
  if (uVar4 < 5) {
    pmVar5 = this + 0x14;
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 4;
      *piVar2 = iVar6;
LAB_0002f770:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f778;
    case 1:
      **(undefined4 **)(param_1 + 4) = *(undefined4 *)pmVar5;
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      goto LAB_0002e9a8;
    case 2:
      *(undefined4 *)pmVar5 = **(undefined4 **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 4;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f2e4;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(pmVar5,1,4,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e9a8:
  uVar4 = *(uint *)param_1;
joined_r0x0002f2e4:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f778:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f780;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x18];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002e9f0;
    case 2:
      this[0x18] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f278;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x18,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002e9f0:
  uVar4 = *(uint *)param_1;
joined_r0x0002f278:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f780:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f788;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x19];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ea38;
    case 2:
      this[0x19] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f214;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x19,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ea38:
  uVar4 = *(uint *)param_1;
joined_r0x0002f214:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f788:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f790;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x1a];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ea80;
    case 2:
      this[0x1a] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f1b0;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x1a,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ea80:
  uVar4 = *(uint *)param_1;
joined_r0x0002f1b0:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f790:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f798;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x1b];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002eac8;
    case 2:
      this[0x1b] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f14c;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x1b,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002eac8:
  uVar4 = *(uint *)param_1;
joined_r0x0002f14c:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f798:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f7a0;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x1c];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002eb10;
    case 2:
      this[0x1c] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f0e8;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x1c,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002eb10:
  uVar4 = *(uint *)param_1;
joined_r0x0002f0e8:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f7a0:
      iVar6 = iVar6 + 2;
      *piVar2 = iVar6;
      goto LAB_0002f7a8;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x1d];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002eb58;
    case 2:
      this[0x1d] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f084;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x1d,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002eb58:
  uVar4 = *(uint *)param_1;
joined_r0x0002f084:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 2;
      *piVar2 = iVar6;
LAB_0002f7a8:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f7b0;
    case 1:
      pmVar5 = *(mbc **)(param_1 + 4);
      *pmVar5 = this[0x1e];
      pmVar5[1] = this[0x1f];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 2;
      goto LAB_0002eba0;
    case 2:
      pmVar5 = *(mbc **)(param_1 + 4);
      this[0x1e] = *pmVar5;
      this[0x1f] = pmVar5[1];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 2;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002f018;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x1e,1,2,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002eba0:
  uVar4 = *(uint *)param_1;
joined_r0x0002f018:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f7b0:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f7b8;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x20];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ebe8;
    case 2:
      this[0x20] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002efac;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x20,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ebe8:
  uVar4 = *(uint *)param_1;
joined_r0x0002efac:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f7b8:
      iVar6 = iVar6 + 2;
      *piVar2 = iVar6;
      goto LAB_0002f7c0;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x21];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ec30;
    case 2:
      this[0x21] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002ef48;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x21,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ec30:
  uVar4 = *(uint *)param_1;
joined_r0x0002ef48:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 2;
      *piVar2 = iVar6;
LAB_0002f7c0:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f7c8;
    case 1:
      pmVar5 = *(mbc **)(param_1 + 4);
      *pmVar5 = this[0x22];
      pmVar5[1] = this[0x23];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 2;
      goto LAB_0002ec78;
    case 2:
      pmVar5 = *(mbc **)(param_1 + 4);
      this[0x22] = *pmVar5;
      this[0x23] = pmVar5[1];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 2;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002eedc;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x22,1,2,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ec78:
  uVar4 = *(uint *)param_1;
joined_r0x0002eedc:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f7c8:
      iVar6 = iVar6 + 1;
      *piVar2 = iVar6;
      goto LAB_0002f7d0;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x24];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ecc0;
    case 2:
      this[0x24] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002ee70;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x24,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ecc0:
  uVar4 = *(uint *)param_1;
joined_r0x0002ee70:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      piVar2 = *(int **)(param_1 + 4);
      iVar6 = *piVar2 + 1;
      *piVar2 = iVar6;
LAB_0002f7d0:
      *piVar2 = iVar6 + 1;
      return;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x25];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      goto LAB_0002ed08;
    case 2:
      this[0x25] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      uVar4 = *(uint *)param_1;
      goto joined_r0x0002edf8;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x25,1,1,*(undefined4 *)(param_1 + 4));
  }
  else {
    puts("ERROR: invalid serializer!");
  }
LAB_0002ed08:
  uVar4 = *(uint *)param_1;
joined_r0x0002edf8:
  if (uVar4 < 5) {
    switch(uVar4) {
    case 0:
      **(int **)(param_1 + 4) = **(int **)(param_1 + 4) + 1;
      return;
    case 1:
      **(mbc **)(param_1 + 4) = this[0x26];
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      return;
    case 2:
      this[0x26] = **(mbc **)(param_1 + 4);
      *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
      return;
    case 3:
      pcVar8 = fwrite;
      break;
    case 4:
      pcVar8 = fread;
    }
    (*pcVar8)(this + 0x26,1,1,*(undefined4 *)(param_1 + 4));
    return;
  }
  puts("ERROR: invalid serializer!");
  return;
}



/* ===== FUNCTION rom @ 0002fbb0 ===== */

/* rom::rom() */

void __thiscall rom::rom(rom *this)

{
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x30] = (rom)0x0;
  return;
}



/* ===== FUNCTION ~rom @ 0002fbc0 ===== */

/* rom::~rom() */

void __thiscall rom::~rom(rom *this)

{
  free(*(void **)(this + 0x24));
                    /* WARNING: Could not recover jumptable at 0x0002fc00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  free(*(void **)(this + 0x28));
  return;
}



/* ===== FUNCTION has_battery @ 0002fc08 ===== */

/* rom::has_battery() */

bool __thiscall rom::has_battery(rom *this)

{
  if (*(int *)(this + 0x18) < 0x21) {
    return *(int *)(has_battery()::has_bat + *(int *)(this + 0x18) * 4) == 1;
  }
  return true;
}



/* ===== FUNCTION get_sram_size @ 0002fc4c ===== */

/* rom::get_sram_size() */

int __thiscall rom::get_sram_size(rom *this)

{
  return *(int *)(get_sram_size()::tbl_ram + (uint)(byte)this[0x1d] * 4) << 0xd;
}



/* ===== FUNCTION load_rom @ 0002fc78 ===== */

/* rom::load_rom(unsigned char*, int, unsigned char*, int) */

undefined4 __thiscall rom::load_rom(rom *this,uchar *param_1,int param_2,uchar *param_3,int param_4)

{
  rom rVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  void *pvVar5;
  size_t __size;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint in_t2;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1d;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  local_27 = 0x4f;
  local_25 = 0x4f;
  local_23 = 0x4f;
  local_28 = 0x4d;
  local_22 = 0x4c;
  local_26 = 0x4d;
  local_21 = 0;
  local_20 = 0;
  local_1f = 0;
  local_1e = 0;
  local_1d = 0;
  local_1c = 0;
  local_1b = 0;
  local_1a = 0;
  local_19 = 0;
  local_24 = 0x43;
  if (this[0x30] != (rom)0x0) {
    free(*(void **)(this + 0x24));
    free(*(void **)(this + 0x28));
  }
  uVar2 = (uint)(param_1 + 0x137) & 3;
  uVar3 = (uint)(param_1 + 0x134) & 3;
  uVar8 = *(undefined4 *)(param_1 + 0x138);
  uVar7 = *(undefined4 *)(param_1 + 0x13c);
  uVar6 = *(undefined4 *)(param_1 + 0x140);
  *(uint *)(this + 4) =
       (*(int *)(param_1 + 0x137 + -uVar2) << (3 - uVar2) * 8 |
       in_t2 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar3) * 8 |
       *(uint *)(param_1 + 0x134 + -uVar3) >> uVar3 * 8;
  *(undefined4 *)(this + 8) = uVar8;
  *(undefined4 *)(this + 0xc) = uVar7;
  *(undefined4 *)(this + 0x10) = uVar6;
  this[0x14] = (rom)0x0;
  this[0x15] = (rom)0x0;
  *(uint *)(this + 0x18) = (uint)param_1[0x147];
  rVar1 = *(rom *)(param_1 + 0x148);
  this[0x1c] = rVar1;
  this[0x1d] = *(rom *)(param_1 + 0x149);
  iVar4 = memcmp(this + 4,&local_28,0x10);
  if (iVar4 == 0) {
    *(undefined4 *)(this + 0x18) = 0x100;
  }
  uVar6 = 1;
  if ((char)param_1[0x143] < '\0') {
    uVar6 = 3;
  }
  *(undefined4 *)(this + 0x20) = uVar6;
  if (8 < (byte)rVar1) {
    return 0;
  }
  pvVar5 = malloc(param_2);
  *(void **)(this + 0x24) = pvVar5;
  pvVar5 = memcpy(pvVar5,param_1,param_2);
  *(void **)(this + 0x2c) = pvVar5;
  __size = get_sram_size(this);
  pvVar5 = malloc(__size);
  *(void **)(this + 0x28) = pvVar5;
  if (param_3 != (uchar *)0x0) {
    memcpy(pvVar5,param_3,param_4 & 0xffffff00);
  }
  this[0x30] = (rom)0x1;
  return 1;
}



/* ===== FUNCTION serialize @ 0002fea0 ===== */

/* rom::serialize(serializer&) */

void rom::serialize(serializer *param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint uVar31;
  uint *puVar32;
  uint uVar33;
  serializer *psVar34;
  uint *in_a1;
  uint in_a2;
  uint in_a3;
  uint in_t0;
  uint in_t1;
  uint in_t2;
  uint in_t3;
  code *pcVar35;
  
  uVar31 = *in_a1;
  if (4 < uVar31) {
    puts("ERROR: invalid serializer!");
    goto LAB_0002ff10;
  }
  psVar34 = param_1 + 4;
  switch(uVar31) {
  case 0:
    *(int *)in_a1[1] = *(int *)in_a1[1] + 0x20;
    break;
  case 1:
    uVar1 = (uint)(param_1 + 7) & 3;
    uVar2 = (uint)(param_1 + 0xb) & 3;
    iVar9 = *(int *)(param_1 + 0xb + -uVar2);
    puVar32 = (uint *)in_a1[1];
    uVar3 = (uint)(param_1 + 0xf) & 3;
    iVar10 = *(int *)(param_1 + 0xf + -uVar3);
    uVar4 = (uint)(param_1 + 0x13) & 3;
    iVar11 = *(int *)(param_1 + 0x13 + -uVar4);
    uVar5 = (uint)(param_1 + 0x17) & 3;
    iVar12 = *(int *)(param_1 + 0x17 + -uVar5);
    uVar6 = (uint)(param_1 + 0x1b) & 3;
    iVar13 = *(int *)(param_1 + 0x1b + -uVar6);
    uVar7 = (uint)(param_1 + 0x1f) & 3;
    iVar14 = *(int *)(param_1 + 0x1f + -uVar7);
    uVar8 = (uint)(param_1 + 0x23) & 3;
    iVar15 = *(int *)(param_1 + 0x23 + -uVar8);
    uVar16 = (uint)psVar34 & 3;
    uVar17 = (uint)(param_1 + 8) & 3;
    uVar24 = *(uint *)(param_1 + 8 + -uVar17);
    uVar18 = (uint)(param_1 + 0xc) & 3;
    uVar25 = *(uint *)(param_1 + 0xc + -uVar18);
    uVar19 = (uint)(param_1 + 0x10) & 3;
    uVar26 = *(uint *)(param_1 + 0x10 + -uVar19);
    uVar20 = (uint)(param_1 + 0x14) & 3;
    uVar27 = *(uint *)(param_1 + 0x14 + -uVar20);
    uVar21 = (uint)(param_1 + 0x18) & 3;
    uVar28 = *(uint *)(param_1 + 0x18 + -uVar21);
    uVar22 = (uint)(param_1 + 0x1c) & 3;
    uVar29 = *(uint *)(param_1 + 0x1c + -uVar22);
    uVar23 = (uint)(param_1 + 0x20) & 3;
    uVar30 = *(uint *)(param_1 + 0x20 + -uVar23);
    *puVar32 = (*(int *)(param_1 + 7 + -uVar1) << (3 - uVar1) * 8 |
               in_t3 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar16) * 8 |
               *(uint *)(psVar34 + -uVar16) >> uVar16 * 8;
    puVar32[1] = (iVar9 << (3 - uVar2) * 8 | in_t2 & 0xffffffffU >> (uVar2 + 1) * 8) &
                 -1 << (4 - uVar17) * 8 | uVar24 >> uVar17 * 8;
    puVar32[2] = (iVar10 << (3 - uVar3) * 8 | in_t1 & 0xffffffffU >> (uVar3 + 1) * 8) &
                 -1 << (4 - uVar18) * 8 | uVar25 >> uVar18 * 8;
    puVar32[3] = (iVar11 << (3 - uVar4) * 8 | in_t0 & 0xffffffffU >> (uVar4 + 1) * 8) &
                 -1 << (4 - uVar19) * 8 | uVar26 >> uVar19 * 8;
    puVar32[4] = (iVar12 << (3 - uVar5) * 8 | in_a3 & 0xffffffffU >> (uVar5 + 1) * 8) &
                 -1 << (4 - uVar20) * 8 | uVar27 >> uVar20 * 8;
    puVar32[5] = (iVar13 << (3 - uVar6) * 8 | in_a2 & 0xffffffffU >> (uVar6 + 1) * 8) &
                 -1 << (4 - uVar21) * 8 | uVar28 >> uVar21 * 8;
    puVar32[6] = (iVar14 << (3 - uVar7) * 8 | (uint)in_a1 & 0xffffffffU >> (uVar7 + 1) * 8) &
                 -1 << (4 - uVar22) * 8 | uVar29 >> uVar22 * 8;
    puVar32[7] = (iVar15 << (3 - uVar8) * 8 | uVar31 * 4 & 0xffffffffU >> (uVar8 + 1) * 8) &
                 -1 << (4 - uVar23) * 8 | uVar30 >> uVar23 * 8;
    in_a1[1] = in_a1[1] + 0x20;
    break;
  case 2:
    uVar33 = in_a1[1];
    uVar1 = uVar33 + 3 & 3;
    uVar2 = uVar33 + 7 & 3;
    iVar9 = *(int *)((uVar33 + 7) - uVar2);
    uVar3 = uVar33 + 0xb & 3;
    iVar10 = *(int *)((uVar33 + 0xb) - uVar3);
    uVar4 = uVar33 + 0xf & 3;
    iVar11 = *(int *)((uVar33 + 0xf) - uVar4);
    uVar5 = uVar33 + 0x13 & 3;
    iVar12 = *(int *)((uVar33 + 0x13) - uVar5);
    uVar6 = uVar33 + 0x17 & 3;
    iVar13 = *(int *)((uVar33 + 0x17) - uVar6);
    uVar7 = uVar33 + 0x1b & 3;
    iVar14 = *(int *)((uVar33 + 0x1b) - uVar7);
    uVar8 = uVar33 + 0x1f & 3;
    iVar15 = *(int *)((uVar33 + 0x1f) - uVar8);
    uVar16 = uVar33 & 3;
    uVar17 = uVar33 + 4 & 3;
    uVar24 = *(uint *)((uVar33 + 4) - uVar17);
    uVar18 = uVar33 + 8 & 3;
    uVar25 = *(uint *)((uVar33 + 8) - uVar18);
    uVar19 = uVar33 + 0xc & 3;
    uVar26 = *(uint *)((uVar33 + 0xc) - uVar19);
    uVar20 = uVar33 + 0x10 & 3;
    uVar27 = *(uint *)((uVar33 + 0x10) - uVar20);
    uVar21 = uVar33 + 0x14 & 3;
    uVar28 = *(uint *)((uVar33 + 0x14) - uVar21);
    uVar22 = uVar33 + 0x18 & 3;
    uVar29 = *(uint *)((uVar33 + 0x18) - uVar22);
    uVar23 = uVar33 + 0x1c & 3;
    uVar30 = *(uint *)((uVar33 + 0x1c) - uVar23);
    *(uint *)psVar34 =
         (*(int *)((uVar33 + 3) - uVar1) << (3 - uVar1) * 8 | in_t3 & 0xffffffffU >> (uVar1 + 1) * 8
         ) & -1 << (4 - uVar16) * 8 | *(uint *)(uVar33 - uVar16) >> uVar16 * 8;
    *(uint *)(param_1 + 8) =
         (iVar9 << (3 - uVar2) * 8 | in_t2 & 0xffffffffU >> (uVar2 + 1) * 8) &
         -1 << (4 - uVar17) * 8 | uVar24 >> uVar17 * 8;
    *(uint *)(param_1 + 0xc) =
         (iVar10 << (3 - uVar3) * 8 | in_t1 & 0xffffffffU >> (uVar3 + 1) * 8) &
         -1 << (4 - uVar18) * 8 | uVar25 >> uVar18 * 8;
    *(uint *)(param_1 + 0x10) =
         (iVar11 << (3 - uVar4) * 8 | in_t0 & 0xffffffffU >> (uVar4 + 1) * 8) &
         -1 << (4 - uVar19) * 8 | uVar26 >> uVar19 * 8;
    *(uint *)(param_1 + 0x14) =
         (iVar12 << (3 - uVar5) * 8 | in_a3 & 0xffffffffU >> (uVar5 + 1) * 8) &
         -1 << (4 - uVar20) * 8 | uVar27 >> uVar20 * 8;
    *(uint *)(param_1 + 0x18) =
         (iVar13 << (3 - uVar6) * 8 | in_a2 & 0xffffffffU >> (uVar6 + 1) * 8) &
         -1 << (4 - uVar21) * 8 | uVar28 >> uVar21 * 8;
    *(uint *)(param_1 + 0x1c) =
         (iVar14 << (3 - uVar7) * 8 | (uint)in_a1 & 0xffffffffU >> (uVar7 + 1) * 8) &
         -1 << (4 - uVar22) * 8 | uVar29 >> uVar22 * 8;
    *(uint *)(param_1 + 0x20) =
         (iVar15 << (3 - uVar8) * 8 | uVar31 * 4 & 0xffffffffU >> (uVar8 + 1) * 8) &
         -1 << (4 - uVar23) * 8 | uVar30 >> uVar23 * 8;
    in_a1[1] = in_a1[1] + 0x20;
    break;
  case 3:
    pcVar35 = fwrite;
    goto LAB_0002fefc;
  case 4:
    pcVar35 = fread;
LAB_0002fefc:
    (*pcVar35)(psVar34,1,0x20,in_a1[1]);
  }
LAB_0002ff10:
  get_sram_size((rom *)param_1);
  if (*in_a1 < 5) {
                    /* WARNING: Could not recover jumptable at 0x0002ff4c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(apu_snd::render(short*,int)::filter + *(int *)(&DAT_00035f64 + *in_a1 * 4) + 0x7ccc))
              ();
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x00030184. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  puts("ERROR: invalid serializer!");
  return;
}



/* ===== FUNCTION map_color @ 000301b0 ===== */

/* dmy_renderer::map_color(unsigned short) */

uint __thiscall dmy_renderer::map_color(dmy_renderer *this,ushort param_1)

{
  undefined2 in_register_00000016;
  
  return (param_1 & 0x3e0) << 1 | CONCAT22(in_register_00000016,param_1) >> 4 & 0x20U |
         (param_1 & 0x1f) << 0xb | CONCAT22(in_register_00000016,param_1) >> 10 & 0x1fU;
}



/* ===== FUNCTION unmap_color @ 000301e0 ===== */

/* dmy_renderer::unmap_color(unsigned short) */

uint __thiscall dmy_renderer::unmap_color(dmy_renderer *this,ushort param_1)

{
  undefined2 in_register_00000016;
  
  return (param_1 & 0x1f) << 10 | (int)CONCAT22(in_register_00000016,param_1) >> 1 & 0x3e0U |
         CONCAT22(in_register_00000016,param_1) >> 0xb;
}



/* ===== FUNCTION check_pad @ 00030200 ===== */

/* dmy_renderer::check_pad() */

void __thiscall dmy_renderer::check_pad(dmy_renderer *this)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  byte bVar9;
  
  iVar1 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,8);
  iVar2 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,0);
  iVar3 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,2);
  iVar4 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,3);
  iVar5 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,5);
  iVar6 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,4);
  iVar7 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,6);
  iVar8 = (*input_state_cb)(*(undefined4 *)(this + 0x14),1,0,7);
  bVar9 = 0x80;
  if (iVar8 == 0) {
    bVar9 = 0;
  }
  this[0xc] = (dmy_renderer)
              (bVar9 | (iVar7 != 0) << 6 |
                       (iVar6 != 0) << 5 |
                       (iVar5 != 0) << 4 |
                       (iVar4 != 0) << 3 | (iVar3 != 0) << 2 | (iVar2 != 0) << 1 | iVar1 != 0);
  return;
}



/* ===== FUNCTION get_time @ 00030364 ===== */

/* dmy_renderer::get_time(int) */

uint __thiscall dmy_renderer::get_time(dmy_renderer *this,int param_1)

{
  uint uVar1;
  
  uVar1 = *(int *)(this + 8) - *(int *)(this + 0x10);
  switch(param_1) {
  case 8:
    return uVar1 % 0x3c;
  case 9:
    return (uVar1 / 0x3c) % 0x3c;
  case 10:
    return (uVar1 / 0xe10) % 0x18;
  case 0xb:
    return uVar1 / 0x15180 & 0xff;
  case 0xc:
    return uVar1 / 0x1518000 & 1;
  default:
    return 0;
  }
}



/* ===== FUNCTION set_time @ 0003048c ===== */

/* dmy_renderer::set_time(int, unsigned char) */

void __thiscall dmy_renderer::set_time(dmy_renderer *this,int param_1,uchar param_2)

{
  uint uVar1;
  undefined3 in_register_00000019;
  uint uVar2;
  int iVar3;
  
  uVar2 = CONCAT31(in_register_00000019,param_2);
  iVar3 = *(int *)(this + 8);
  uVar1 = iVar3 - *(int *)(this + 0x10);
  switch(param_1) {
  case 8:
    *(uint *)(this + 0x10) = iVar3 - (uVar2 % 0x3c + (uVar1 / 0x3c) * 0x3c);
    return;
  case 9:
    *(uint *)(this + 0x10) =
         iVar3 - ((uVar1 / 0xe10) * 0xe10 + (uVar2 % 0x3c) * 0x3c + uVar1 % 0x3c);
    return;
  case 10:
    *(uint *)(this + 0x10) =
         iVar3 - (uVar1 % 0xe10 + (uVar2 % 0x18) * 0xe10 + (uVar1 / 0x15180) * 0x15180);
    return;
  case 0xb:
    *(uint *)(this + 0x10) =
         iVar3 - (uVar1 % 0x15180 + uVar2 * 0x15180 + (uVar1 / 0x1518000) * 0x1518000);
    return;
  case 0xc:
    uVar1 = (param_2 & 1) * 0x1518000 + uVar1 % 0x1518000;
  }
  *(uint *)(this + 0x10) = iVar3 - uVar1;
  return;
}



/* ===== FUNCTION refresh @ 000306a0 ===== */

/* dmy_renderer::refresh() */

void __thiscall dmy_renderer::refresh(dmy_renderer *this)

{
  short *psVar1;
  short sVar2;
  time_t tVar3;
  undefined2 *puVar4;
  short *psVar5;
  uint uVar6;
  short local_b90 [1472];
  
  if ((DAT_00051b64 == 0) || (gblink_enable == '\0')) {
    (**(code **)**(undefined4 **)(this + 4))(*(undefined4 **)(this + 4),refresh()::stream,0x2df);
    (*audio_batch_cb)(refresh()::stream,0x2df);
  }
  else {
    if (audio_2p_mode == 2) {
      psVar5 = local_b90;
      (**(code **)**(undefined4 **)(this + 4))(*(undefined4 **)(this + 4),psVar5,0x2df);
      uVar6 = *(uint *)(this + 0x14);
      puVar4 = (undefined2 *)(refresh()::stream + uVar6 * 2);
      do {
        sVar2 = *psVar5;
        psVar1 = psVar5 + 1;
        psVar5 = psVar5 + 2;
        *puVar4 = (short)(((int)sVar2 + (int)*psVar1) / 2);
        puVar4 = puVar4 + 2;
      } while (psVar5 != local_b90 + 0x5be);
    }
    else {
      uVar6 = *(uint *)(this + 0x14);
      if (audio_2p_mode == uVar6) {
        (**(code **)**(undefined4 **)(this + 4))(*(undefined4 **)(this + 4),refresh()::stream);
        uVar6 = *(uint *)(this + 0x14);
      }
    }
    if (uVar6 == 1) {
      (*audio_batch_cb)(refresh()::stream,0x2df);
      audio_2p_mode = audio_2p_mode & 3;
      memset(refresh()::stream,0,0xb7c);
    }
  }
  tVar3 = time((time_t *)0x0);
  *(time_t *)(this + 8) = tVar3;
  return;
}



/* ===== FUNCTION render_screen @ 00030844 ===== */

/* dmy_renderer::render_screen(unsigned char*, int, int, int) */

void __thiscall
dmy_renderer::render_screen(dmy_renderer *this,uchar *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  void *pvVar2;
  uchar *puVar3;
  int iVar4;
  uchar *puVar5;
  size_t __n;
  
  if (video_buf == (uchar *)0x0) {
    return;
  }
  SYNC(0);
  if ((render_screen(unsigned_char*,int,int,int)::joined_buf == '\0') &&
     (iVar4 = __cxa_guard_acquire(&render_screen(unsigned_char*,int,int,int)::joined_buf),
     iVar4 != 0)) {
    render_screen(unsigned_char*,int,int,int)::joined_buf = video_buf;
    __cxa_guard_release(&render_screen(unsigned_char*,int,int,int)::joined_buf);
  }
  puVar3 = render_screen(unsigned_char*,int,int,int)::joined_buf;
  iVar4 = *(int *)(this + 0x14);
  iVar1 = param_4 + 7;
  if (param_4 + 7 < 0) {
    iVar1 = param_4 + 0xe;
  }
  __n = (iVar1 >> 3) * param_2;
  iVar1 = iVar4;
  if (_screen_switched != '\0') {
    iVar1 = 1 - iVar4;
  }
  puVar5 = param_1;
  if ((DAT_00051b64 != 0) && (gblink_enable != '\0')) {
    puVar5 = puVar3;
    if (_show_player_screens == 2) {
      if (_screen_2p_vertical == '\0') {
        if (0 < param_3) {
          iVar4 = 0;
          puVar3 = render_screen(unsigned_char*,int,int,int)::joined_buf + iVar1 * __n;
          do {
            iVar4 = iVar4 + 1;
            pvVar2 = memcpy(puVar3,param_1,__n);
            puVar3 = (uchar *)((int)pvVar2 + __n * 2);
            param_1 = param_1 + __n;
          } while (param_3 != iVar4);
          iVar4 = *(int *)(this + 0x14);
        }
        if (iVar4 != 1) {
          return;
        }
        __n = __n * 2;
        param_2 = param_2 << 1;
        puVar5 = render_screen(unsigned_char*,int,int,int)::joined_buf;
        goto LAB_00030964;
      }
      puVar3 = render_screen(unsigned_char*,int,int,int)::joined_buf + iVar1 * 2;
      *puVar3 = *param_1;
      puVar3[1] = param_1[1];
      iVar4 = *(int *)(this + 0x14);
      param_3 = param_3 << 1;
    }
    else if (iVar4 == _show_player_screens) {
      *render_screen(unsigned_char*,int,int,int)::joined_buf = *param_1;
      puVar3[1] = param_1[1];
      iVar4 = *(int *)(this + 0x14);
      puVar5 = render_screen(unsigned_char*,int,int,int)::joined_buf;
    }
    if (iVar4 != 1) {
      return;
    }
  }
LAB_00030964:
                    /* WARNING: Could not recover jumptable at 0x0003098c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*video_cb)(puVar5,param_2,param_3,__n);
  return;
}



/* ===== FUNCTION dmy_renderer @ 00030aa4 ===== */

/* dmy_renderer::dmy_renderer(int) */

void __thiscall dmy_renderer::dmy_renderer(dmy_renderer *this,int param_1)

{
  *(int *)(this + 0x14) = param_1;
  this[0x18] = (dmy_renderer)0x1;
  *(undefined ***)this = &PTR_reset_000481d8;
  return;
}



/* ===== FUNCTION ~dmy_renderer @ 00030ad0 ===== */

/* dmy_renderer::~dmy_renderer() */

void __thiscall dmy_renderer::~dmy_renderer(dmy_renderer *this)

{
  return;
}



/* ===== FUNCTION reset @ 00030ad8 ===== */

/* dmy_renderer::reset() */

void dmy_renderer::reset(void)

{
  return;
}



/* ===== FUNCTION get_sensor @ 00030ae0 ===== */

/* dmy_renderer::get_sensor(bool) */

undefined4 dmy_renderer::get_sensor(bool param_1)

{
  return 0;
}



/* ===== FUNCTION output_log @ 00030ae8 ===== */

/* dmy_renderer::output_log(char*, ...) */

void dmy_renderer::output_log(char *param_1,...)

{
  return;
}



/* ===== FUNCTION set_bibrate @ 00030af4 ===== */

/* dmy_renderer::set_bibrate(bool) */

void dmy_renderer::set_bibrate(bool param_1)

{
  return;
}



/* ===== FUNCTION ~dmy_renderer @ 00030afc ===== */

/* dmy_renderer::~dmy_renderer() */

void __thiscall dmy_renderer::~dmy_renderer(dmy_renderer *this)

{
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION check_variables @ 00030b20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */
/* check_variables() */

void check_variables(void)

{
  undefined4 *puVar1;
  code *pcVar2;
  char *pcVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  char *local_20;
  char *local_1c;
  
  local_20 = "tgbdual_gblink_enable";
  local_1c = (char *)0x0;
  iVar4 = (*environ_cb)(0xf,&local_20);
  pcVar3 = local_1c;
  if ((iVar4 == 0) || (local_1c == (char *)0x0)) {
LAB_00030b7c:
    gblink_enable = '\0';
  }
  else if (already_checked_options == '\0') {
    iVar4 = strcmp(local_1c,"disabled");
    if (iVar4 == 0) goto LAB_00030b7c;
    iVar4 = strcmp(pcVar3,"enabled");
    if (iVar4 == 0) {
      gblink_enable = '\x01';
    }
  }
  local_1c = (char *)0x0;
  local_20 = "tgbdual_screen_placement";
  iVar4 = (*environ_cb)(0xf,&local_20);
  pcVar3 = local_1c;
  if (((iVar4 == 0) || (local_1c == (char *)0x0)) ||
     (iVar4 = strcmp(local_1c,"left-right"), iVar4 == 0)) {
    _screen_2p_vertical = '\0';
  }
  else {
    iVar4 = strcmp(pcVar3,"top-down");
    if (iVar4 == 0) {
      _screen_2p_vertical = '\x01';
    }
  }
  local_1c = (char *)0x0;
  local_20 = "tgbdual_switch_screens";
  iVar4 = (*environ_cb)(0xf,&local_20);
  pcVar3 = local_1c;
  if (((iVar4 == 0) || (local_1c == (char *)0x0)) || (iVar4 = strcmp(local_1c,"normal"), iVar4 == 0)
     ) {
    _screen_switched = 0;
  }
  else {
    iVar4 = strcmp(pcVar3,"switched");
    if (iVar4 == 0) {
      _screen_switched = 1;
    }
  }
  local_1c = (char *)0x0;
  local_20 = "tgbdual_single_screen_mp";
  iVar4 = (*environ_cb)(0xf,&local_20);
  pcVar3 = local_1c;
  if (((iVar4 == 0) || (local_1c == (char *)0x0)) ||
     (iVar4 = strcmp(local_1c,"both players"), iVar4 == 0)) {
    _show_player_screens = 2;
    if (gblink_enable != '\0') {
LAB_00030e3c:
      if (_screen_2p_vertical == '\0') {
        uVar6 = 0x90;
        uVar5 = 0x140;
        uVar7 = 0x400e38e4;
      }
      else {
        uVar6 = 0x120;
        uVar5 = 0xa0;
        uVar7 = 0x3f0e38e4;
      }
      goto LAB_00030ca0;
    }
  }
  else {
    iVar4 = strcmp(pcVar3,"player 1 only");
    if (iVar4 == 0) {
      _show_player_screens = 0;
    }
    else {
      iVar4 = strcmp(pcVar3,"player 2 only");
      if (iVar4 == 0) {
        _show_player_screens = 1;
      }
      else if ((gblink_enable != '\0') && (_show_player_screens == 2)) goto LAB_00030e3c;
    }
  }
  uVar6 = 0x90;
  uVar5 = 0xa0;
  uVar7 = 0x3f8e38e4;
LAB_00030ca0:
  pcVar2 = environ_cb;
  puVar1 = my_av_info;
  my_av_info[4] = uVar7;
  *puVar1 = uVar5;
  puVar1[1] = uVar6;
  already_checked_options = 1;
  (*pcVar2)(0x25,puVar1);
  local_1c = (char *)0x0;
  local_20 = "tgbdual_audio_output";
  iVar4 = (*environ_cb)(0xf,&local_20);
  pcVar3 = local_1c;
  if ((iVar4 == 0) || (local_1c == (char *)0x0)) {
    _screen_switched = 0;
  }
  else {
    iVar4 = strcmp(local_1c,"Game Boy #1");
    if (iVar4 == 0) {
      audio_2p_mode = 0;
      return;
    }
    iVar4 = strcmp(pcVar3,"Game Boy #2");
    if (iVar4 == 0) {
      audio_2p_mode = 1;
      return;
    }
  }
  return;
}



/* ===== FUNCTION retro_get_system_info @ 00030f08 ===== */

void retro_get_system_info(undefined4 *param_1)

{
  *(undefined1 *)(param_1 + 3) = 0;
  *param_1 = "TGB Dual";
  param_1[1] = "v0.8.3 9be31d3";
  param_1[2] = "gb|gbc|sgb";
  return;
}



/* ===== FUNCTION retro_get_system_av_info @ 00030f40 ===== */

void retro_get_system_av_info(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  param_1[2] = 0x140;
  param_1[3] = 0x120;
  if ((DAT_00051b64 == 0) || (_show_player_screens != 2)) {
    uVar2 = 0x90;
    uVar6 = 0xa0;
    uVar7 = 0x3f8e38e4;
  }
  else if (_screen_2p_vertical == '\0') {
    uVar2 = 0x90;
    uVar6 = 0x140;
    uVar7 = 0x400e38e4;
  }
  else {
    uVar2 = 0x120;
    uVar6 = 0xa0;
    uVar7 = 0x3f0e38e4;
  }
  *param_1 = uVar6;
  param_1[4] = uVar7;
  param_1[1] = uVar2;
  puVar1 = my_av_info;
  *(undefined8 *)(param_1 + 6) = 0x404ddd1ebd1923a3;
  *(undefined8 *)(param_1 + 8) = 0x40e5888000000000;
  puVar5 = param_1;
  do {
    puVar4 = puVar5;
    puVar3 = puVar1;
    uVar7 = puVar4[1];
    uVar6 = puVar4[2];
    uVar2 = puVar4[3];
    puVar5 = puVar4 + 4;
    *puVar3 = *puVar4;
    puVar3[1] = uVar7;
    puVar3[2] = uVar6;
    puVar3[3] = uVar2;
    puVar1 = puVar3 + 4;
  } while (puVar5 != param_1 + 8);
  uVar2 = puVar4[5];
  puVar3[4] = *puVar5;
  puVar3[5] = uVar2;
  return;
}



/* ===== FUNCTION retro_init @ 00031064 ===== */

void retro_init(void)

{
  int iVar1;
  undefined4 local_10;
  undefined4 local_c;
  
  local_c = 4;
  iVar1 = (*environ_cb)(0x1b,&local_10);
  if (iVar1 == 0) {
    log_cb = 0;
  }
  else {
    log_cb = local_10;
  }
  (*environ_cb)(8,&local_c);
  g_frame_addr = malloc(0x16800);
  DAT_00051b70 = (int)g_frame_addr + 0xb400;
  g_frame_index = 0;
  video_buf = g_frame_addr;
  return;
}



/* ===== FUNCTION retro_deinit @ 00031118 ===== */

void retro_deinit(void)

{
  free(g_frame_addr);
  video_buf = 0;
  return;
}



/* ===== FUNCTION retro_load_game @ 0003115c ===== */

int retro_load_game(int param_1)

{
  gb *pgVar1;
  int iVar2;
  dmy_renderer *pdVar3;
  gb *pgVar4;
  int iVar5;
  undefined1 auStack_178 [4];
  undefined4 local_174;
  undefined4 local_16c;
  char *local_168;
  undefined4 local_160;
  undefined4 local_158;
  char *local_154;
  undefined4 local_14c;
  undefined4 local_144;
  char *local_140;
  undefined4 local_138;
  undefined4 local_130;
  char *local_12c;
  undefined4 local_124;
  undefined *local_118;
  undefined4 local_110;
  undefined4 local_108;
  undefined *local_104;
  undefined4 local_fc;
  undefined4 local_f4;
  char *local_f0;
  undefined4 local_e8;
  undefined4 local_e0;
  char *local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_cc;
  char *local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_b8;
  char *local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a4;
  char *local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_90;
  char *local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined *local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_68;
  undefined *local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_54;
  char *local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_40;
  char *local_3c;
  
  (*environ_cb)(0x10,vars_single);
  check_variables();
  iVar2 = check_encrypty();
  if (iVar2 != 0) {
    memset(auStack_178,0,0x154);
    local_174 = 1;
    local_16c = 6;
    local_160 = 1;
    local_168 = "D-Pad Left";
    local_154 = "D-Pad Up";
    local_140 = "D-Pad Down";
    local_12c = "D-Pad Right";
    local_118 = &DAT_000361c4;
    local_104 = &DAT_000361c8;
    local_f0 = "Select";
    local_158 = 4;
    local_dc = "Start";
    local_14c = 1;
    local_144 = 5;
    local_138 = 1;
    local_130 = 7;
    local_124 = 1;
    local_110 = 1;
    local_108 = 8;
    local_fc = 1;
    local_f4 = 2;
    local_e8 = 1;
    local_e0 = 3;
    local_d8 = 1;
    local_d4 = 1;
    local_cc = 6;
    local_c4 = 1;
    local_c0 = 1;
    local_b8 = 4;
    local_b0 = 1;
    local_ac = 1;
    local_a4 = 5;
    local_9c = 1;
    local_98 = 1;
    local_90 = 7;
    local_88 = 1;
    local_84 = 1;
    local_74 = 1;
    local_70 = 1;
    local_68 = 8;
    local_60 = 1;
    local_5c = 1;
    local_54 = 3;
    local_4c = 1;
    local_48 = 1;
    local_40 = 2;
    if (param_1 != 0) {
      g_gb = (gb *)0x0;
      render = (dmy_renderer *)0x0;
      DAT_00051b5c = (dmy_renderer *)0x0;
      DAT_00051b64 = (gb *)0x0;
      local_c8 = local_168;
      local_b4 = local_154;
      local_a0 = local_140;
      local_8c = local_12c;
      local_78 = local_118;
      local_64 = local_104;
      local_50 = local_dc;
      local_3c = local_f0;
      (*environ_cb)(0xb,auStack_178);
      pdVar3 = operator_new(0x1c);
                    /* try { // try from 00031374 to 0003137b has its CatchHandler @ 00031564 */
      dmy_renderer::dmy_renderer(pdVar3,0);
      render = pdVar3;
      pgVar4 = operator_new(0x137a0);
                    /* try { // try from 000313ac to 000313b3 has its CatchHandler @ 00031550 */
      gb::gb(pgVar4,(renderer *)render,true,true);
      g_gb = pgVar4;
      iVar2 = gb::load_rom(pgVar4,*(uchar **)(param_1 + 4),*(int *)(param_1 + 8),(uchar *)0x0,0);
      if (iVar2 != 0) {
        _serialize_size = 0;
        DAT_00051b7c = 0;
        if (gblink_enable == '\0') {
          mode = 0;
        }
        else {
          mode = 1;
          (*environ_cb)(0x10,vars_dual);
          pdVar3 = operator_new(0x1c);
                    /* try { // try from 000314a8 to 000314af has its CatchHandler @ 00031528 */
          dmy_renderer::dmy_renderer(pdVar3,1);
          DAT_00051b5c = pdVar3;
          pgVar4 = operator_new(0x137a0);
                    /* try { // try from 000314e0 to 000314e7 has its CatchHandler @ 0003156c */
          gb::gb(pgVar4,(renderer *)DAT_00051b5c,true,true);
          DAT_00051b64 = pgVar4;
          iVar5 = gb::load_rom(pgVar4,*(uchar **)(param_1 + 4),*(int *)(param_1 + 8),(uchar *)0x0,0)
          ;
          pgVar1 = DAT_00051b64;
          pgVar4 = g_gb;
          if (iVar5 == 0) {
            return 0;
          }
          *(gb **)(g_gb + 0x1c) = DAT_00051b64;
          *(gb **)(pgVar1 + 0x1c) = pgVar4;
        }
        check_variables();
        return iVar2;
      }
    }
  }
  return 0;
}



/* ===== FUNCTION retro_load_game_special @ 00031574 ===== */

int retro_load_game_special(int param_1,int param_2)

{
  gb *pgVar1;
  dmy_renderer *pdVar2;
  gb *pgVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_1c8 [4];
  undefined4 local_1c4;
  undefined4 local_1bc;
  char *local_1b8;
  undefined4 local_1b0;
  undefined4 local_1a8;
  char *local_1a4;
  undefined4 local_19c;
  undefined4 local_194;
  char *local_190;
  undefined4 local_188;
  undefined4 local_180;
  char *local_17c;
  undefined4 local_174;
  undefined *local_168;
  undefined4 local_160;
  undefined4 local_158;
  undefined *local_154;
  undefined4 local_14c;
  undefined4 local_144;
  char *local_140;
  undefined4 local_138;
  undefined4 local_130;
  char *local_12c;
  undefined4 local_124;
  undefined4 local_11c;
  char *local_118;
  undefined4 local_110;
  undefined4 local_108;
  char *local_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f4;
  char *local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e0;
  char *local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_cc;
  char *local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_b8;
  char *local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined *local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_90;
  undefined *local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_7c;
  char *local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_68;
  char *local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_54;
  char *local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_40;
  char *local_3c;
  
  if (param_1 == 0x101) {
    (*environ_cb)(0x10,vars_dual);
    memset(auStack_1c8,0,0x1a4);
    local_1c4 = 1;
    local_1bc = 6;
    local_1b0 = 1;
    local_1b8 = "D-Pad Left";
    local_1a4 = "D-Pad Up";
    local_190 = "D-Pad Down";
    local_17c = "D-Pad Right";
    local_168 = &DAT_000361c4;
    local_154 = &DAT_000361c8;
    local_140 = "Prev Audio Mode";
    local_12c = "Next Audio Mode";
    local_118 = "Select";
    local_1a8 = 4;
    local_104 = "Start";
    local_19c = 1;
    local_194 = 5;
    local_188 = 1;
    local_180 = 7;
    local_174 = 1;
    local_160 = 1;
    local_158 = 8;
    local_14c = 1;
    local_144 = 10;
    local_138 = 1;
    local_130 = 0xb;
    local_124 = 1;
    local_11c = 2;
    local_110 = 1;
    local_108 = 3;
    local_100 = 1;
    local_fc = 1;
    local_f4 = 6;
    local_ec = 1;
    local_e8 = 1;
    local_e0 = 4;
    local_d8 = 1;
    local_d4 = 1;
    local_cc = 5;
    local_c4 = 1;
    local_c0 = 1;
    local_b8 = 7;
    local_b0 = 1;
    local_ac = 1;
    local_9c = 1;
    local_98 = 1;
    local_90 = 8;
    local_88 = 1;
    local_84 = 1;
    local_7c = 10;
    local_74 = 1;
    local_70 = 1;
    local_68 = 0xb;
    local_60 = 1;
    local_5c = 1;
    local_54 = 3;
    local_4c = 1;
    local_48 = 1;
    local_40 = 2;
    if (param_2 != 0) {
      g_gb = (gb *)0x0;
      render = (dmy_renderer *)0x0;
      DAT_00051b5c = (dmy_renderer *)0x0;
      DAT_00051b64 = (gb *)0x0;
      local_f0 = local_1b8;
      local_dc = local_1a4;
      local_c8 = local_190;
      local_b4 = local_17c;
      local_a0 = local_168;
      local_8c = local_154;
      local_78 = local_140;
      local_64 = local_12c;
      local_50 = local_104;
      local_3c = local_118;
      check_variables();
      (*environ_cb)(0xb,auStack_1c8);
      pdVar2 = operator_new(0x1c);
                    /* try { // try from 000317fc to 00031803 has its CatchHandler @ 00031958 */
      dmy_renderer::dmy_renderer(pdVar2,0);
      render = pdVar2;
      pgVar3 = operator_new(0x137a0);
                    /* try { // try from 00031834 to 0003183b has its CatchHandler @ 00031934 */
      gb::gb(pgVar3,(renderer *)render,true,true);
      g_gb = pgVar3;
      iVar4 = gb::load_rom(pgVar3,*(uchar **)(param_2 + 4),*(int *)(param_2 + 8),(uchar *)0x0,0);
      if (iVar4 != 0) {
        _serialize_size = 0;
        DAT_00051b7c = 0;
        if (gblink_enable != '\0') {
          pdVar2 = operator_new(0x1c);
                    /* try { // try from 000318b4 to 000318bb has its CatchHandler @ 00031988 */
          dmy_renderer::dmy_renderer(pdVar2,1);
          DAT_00051b5c = pdVar2;
          pgVar3 = operator_new(0x137a0);
                    /* try { // try from 000318ec to 000318f3 has its CatchHandler @ 00031960 */
          gb::gb(pgVar3,(renderer *)DAT_00051b5c,true,true);
          DAT_00051b64 = pgVar3;
          iVar5 = gb::load_rom(pgVar3,*(uchar **)(param_2 + 0x14),*(int *)(param_2 + 0x18),
                               (uchar *)0x0,0);
          pgVar1 = DAT_00051b64;
          pgVar3 = g_gb;
          if (iVar5 == 0) {
            return 0;
          }
          *(gb **)(g_gb + 0x1c) = DAT_00051b64;
          *(gb **)(pgVar1 + 0x1c) = pgVar3;
        }
        mode = 2;
        return iVar4;
      }
    }
  }
  return 0;
}



/* ===== FUNCTION retro_unload_game @ 0003199c ===== */

void retro_unload_game(void)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  gb *this;
  
  puVar2 = &g_gb;
  puVar3 = &render;
  do {
    this = (gb *)*puVar2;
    if (this != (gb *)0x0) {
      gb::~gb(this);
      operator_delete(this);
      piVar1 = (int *)*puVar3;
      *puVar2 = 0;
      if (piVar1 != (int *)0x0) {
        if (*(code **)(*piVar1 + 0x30) == dmy_renderer::~dmy_renderer) {
          operator_delete(piVar1);
        }
        else {
          (**(code **)(*piVar1 + 0x30))(piVar1);
        }
      }
      *puVar3 = 0;
    }
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
  } while (puVar2 != &video_buf);
  (*(code *)(undefined *)0x0)(my_av_info);
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION retro_reset @ 00031a84 ===== */

void retro_reset(void)

{
  char cVar1;
  undefined4 *puVar2;
  int iVar3;
  char *apcStack_20 [4];
  
  if (g_gb != 0) {
    gb::reset((gb *)g_gb);
  }
  puVar2 = DAT_00051b64;
  if (DAT_00051b64 == (undefined4 *)0x0) {
    return;
  }
  *(undefined1 *)((int)DAT_00051b64 + 0x26) = 0;
  *(undefined1 *)((int)puVar2 + 0x25) = 0;
  *(undefined1 *)(puVar2 + 9) = 0;
  *(undefined1 *)((int)puVar2 + 0x23) = 0;
  *(undefined1 *)((int)puVar2 + 0x22) = 0;
  *(undefined1 *)(puVar2 + 0xc) = 0xff;
  *(undefined1 *)(puVar2 + 10) = 0x91;
  *(undefined1 *)((int)puVar2 + 0x31) = 0xff;
  *(undefined1 *)(puVar2 + 0xb) = 0x99;
  *(undefined1 *)((int)puVar2 + 0x27) = 0;
  *(undefined1 *)((int)puVar2 + 0x29) = 0;
  *(undefined1 *)((int)puVar2 + 0x2a) = 0;
  *(undefined1 *)((int)puVar2 + 0x2b) = 0;
  *(undefined1 *)((int)puVar2 + 0x2d) = 0;
  *(undefined1 *)((int)puVar2 + 0x32) = 0;
  *(undefined1 *)((int)puVar2 + 0x33) = 0;
  *(undefined1 *)(puVar2 + 0xd) = 0;
  *(undefined1 *)((int)puVar2 + 0x35) = 0;
  *(undefined1 *)((int)puVar2 + 0x36) = 0;
  *(undefined1 *)((int)puVar2 + 0x37) = 0;
  *(undefined1 *)(puVar2 + 0xe) = 0;
  *(undefined1 *)((int)puVar2 + 0x39) = 0;
  *(undefined1 *)((int)puVar2 + 0x3a) = 0;
  *(undefined1 *)((int)puVar2 + 0x3b) = 0;
  *(undefined1 *)(puVar2 + 0xf) = 0;
  *(undefined1 *)((int)puVar2 + 0x3d) = 0;
  *(undefined1 *)((int)puVar2 + 0x3e) = 0;
  *(undefined1 *)((int)puVar2 + 0x2f) = 0xfc;
  *(undefined1 *)((int)puVar2 + 0x3f) = 0;
  *(undefined1 *)(puVar2 + 0x10) = 0;
  *(undefined1 *)((int)puVar2 + 0x41) = 0;
  iVar3 = puVar2[3];
  if (*(char *)(iVar3 + 0x30) != '\0') {
    if (*(char *)(*(int *)(iVar3 + 0x2c) + 0x143) < '\0') {
      *(uint *)(iVar3 + 0x20) = (*(char *)((int)puVar2 + 0x1379d) != '\0') + 3;
    }
    else {
      *(undefined4 *)(iVar3 + 0x20) = 1;
    }
  }
  cpu::reset((cpu *)*puVar2);
  lcd::reset((lcd *)puVar2[1]);
  apu::reset((apu *)puVar2[2]);
  mbc::reset((mbc *)puVar2[4]);
  cVar1 = *(char *)(puVar2[3] + 0x30);
  puVar2[0x4de3] = 0;
  puVar2[0x4de4] = 0;
  puVar2[0x4de5] = 0;
  puVar2[0x4de6] = 0;
  apcStack_20[0] = "Invalid";
  apcStack_20[1] = "Gameboy";
  apcStack_20[2] = "SuperGameboy";
  apcStack_20[3] = "Gameboy Color";
  if (cVar1 != '\0') {
                    /* WARNING: Could not recover jumptable at 0x00025b04. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)puVar2[5] + 0x28))
              ((int *)puVar2[5],"Current GB Type : %s \n",apcStack_20[*(int *)(puVar2[3] + 0x20)]);
    return;
  }
  return;
}



/* ===== FUNCTION SetFrameSkip @ 00031ae8 ===== */

void SetFrameSkip(int param_1)

{
  if (param_1 == 0) {
    g_frame_index = g_frame_index ^ 1;
    video_buf = (&g_frame_addr)[g_frame_index];
    return;
  }
  video_buf = 0;
  return;
}



/* ===== FUNCTION retro_run @ 00031b30 ===== */

void retro_run(void)

{
  int iVar1;
  
  iVar1 = 0x9a;
  do {
    iVar1 = iVar1 + -1;
    if (g_gb != 0) {
      gb::run((gb *)g_gb);
    }
    if (DAT_00051b64 != 0) {
      gb::run((gb *)DAT_00051b64);
    }
  } while (iVar1 != 0);
  return;
}



/* ===== FUNCTION retro_get_memory_data @ 00031ba4 ===== */

int retro_get_memory_data(uint param_1)

{
  if (mode < 2) {
    if (param_1 == 1) goto LAB_00031c94;
    if (param_1 == 0) goto LAB_00031c80;
    if (param_1 == 2) {
      return *g_gb + 0x12;
    }
    if (param_1 == 3) {
      return *g_gb + 0x8012;
    }
  }
  else if (mode != 2) {
    return 0;
  }
  if (param_1 != 0x201) {
    if (param_1 < 0x202) {
      if (param_1 == 0x100) {
LAB_00031c80:
        return *(int *)(g_gb[3] + 0x28);
      }
    }
    else {
      if (param_1 == 0x300) {
        return *(int *)(*(int *)(DAT_00051b64 + 0xc) + 0x28);
      }
      if (param_1 == 0x301) {
        return DAT_00051b5c + 8;
      }
    }
    return 0;
  }
LAB_00031c94:
  return render + 8;
}



/* ===== FUNCTION retro_get_memory_size @ 00031ca0 ===== */

int retro_get_memory_size(uint param_1)

{
  int iVar1;
  
  if (mode < 2) {
    if (param_1 == 1) {
      return 4;
    }
    if (param_1 != 0) {
      if (param_1 == 2) {
        iVar1 = 0x8000;
        if (*(int *)(*(int *)(g_gb + 0xc) + 0x20) < 3) {
          iVar1 = 0x2000;
        }
        return iVar1;
      }
      if (param_1 == 3) {
        iVar1 = 0x4000;
        if (*(int *)(*(int *)(g_gb + 0xc) + 0x20) < 3) {
          iVar1 = 0x2000;
        }
        return iVar1;
      }
      goto LAB_00031cc8;
    }
  }
  else {
    if (mode != 2) {
      return 0;
    }
LAB_00031cc8:
    if (param_1 == 0x201) {
      return 4;
    }
    if (0x201 < param_1) {
      if (param_1 != 0x300) {
        if (param_1 == 0x301) {
          return 4;
        }
        return 0;
      }
      iVar1 = *(int *)(DAT_00051b64 + 0xc);
      goto rom_get_sram_size;
    }
    if (param_1 != 0x100) {
      return 0;
    }
  }
  iVar1 = *(int *)(g_gb + 0xc);
rom_get_sram_size:
  return *(int *)(rom::get_sram_size()::tbl_ram + (uint)*(byte *)(iVar1 + 0x1d) * 4) << 0xd;
}



/* ===== FUNCTION retro_serialize_size @ 00031da4 ===== */

int retro_serialize_size(void)

{
  int iVar1;
  
  iVar1 = _serialize_size + DAT_00051b7c;
  if (iVar1 == 0) {
    if (g_gb != 0) {
      _serialize_size = gb::get_state_size((gb *)g_gb);
    }
    if (DAT_00051b64 == 0) {
      return DAT_00051b7c + _serialize_size;
    }
    DAT_00051b7c = gb::get_state_size((gb *)DAT_00051b64);
    iVar1 = DAT_00051b7c + _serialize_size;
  }
  return iVar1;
}



/* ===== FUNCTION retro_serialize @ 00031e5c ===== */

undefined4 retro_serialize(undefined4 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = retro_serialize_size();
  if (iVar1 == param_2) {
    if (g_gb != (void *)0x0) {
      gb::save_state_mem(g_gb);
    }
    if (DAT_00051b64 != (void *)0x0) {
      gb::save_state_mem(DAT_00051b64);
    }
    return 1;
  }
  return 0;
}



/* ===== FUNCTION retro_unserialize @ 00031f08 ===== */

undefined4 retro_unserialize(undefined4 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = retro_serialize_size();
  if (iVar1 == param_2) {
    if (g_gb != (void *)0x0) {
      gb::restore_state_mem(g_gb);
    }
    if (DAT_00051b64 != (void *)0x0) {
      gb::restore_state_mem(DAT_00051b64);
    }
    return 1;
  }
  return 0;
}



/* ===== FUNCTION retro_cheat_reset @ 00031fb4 ===== */

void retro_cheat_reset(void)

{
  if (g_gb != 0) {
    cheat::clear(*(cheat **)(g_gb + 0x18));
  }
  if (DAT_00051b64 != 0) {
    cheat::clear(*(cheat **)(DAT_00051b64 + 0x18));
    return;
  }
  return;
}



/* ===== FUNCTION retro_cheat_set @ 0003201c ===== */

void retro_cheat_set(void)

{
  return;
}



/* ===== FUNCTION retro_api_version @ 00032024 ===== */

undefined4 retro_api_version(void)

{
  return 1;
}



/* ===== FUNCTION retro_get_region @ 0003202c ===== */

undefined4 retro_get_region(void)

{
  return 0;
}



/* ===== FUNCTION retro_set_controller_port_device @ 00032034 ===== */

void retro_set_controller_port_device(void)

{
  return;
}



/* ===== FUNCTION retro_set_video_refresh @ 0003203c ===== */

void retro_set_video_refresh(undefined4 param_1)

{
  video_cb = param_1;
  return;
}



/* ===== FUNCTION retro_set_audio_sample @ 00032054 ===== */

void retro_set_audio_sample(void)

{
  return;
}



/* ===== FUNCTION retro_set_audio_sample_batch @ 0003205c ===== */

void retro_set_audio_sample_batch(undefined4 param_1)

{
  audio_batch_cb = param_1;
  return;
}



/* ===== FUNCTION retro_set_input_poll @ 00032074 ===== */

void retro_set_input_poll(undefined4 param_1)

{
  input_poll_cb = param_1;
  return;
}



/* ===== FUNCTION retro_set_input_state @ 0003208c ===== */

void retro_set_input_state(undefined4 param_1)

{
  input_state_cb = param_1;
  return;
}



/* ===== FUNCTION retro_set_environment @ 000320a4 ===== */

void retro_set_environment(code *UNRECOVERED_JUMPTABLE)

{
  environ_cb = UNRECOVERED_JUMPTABLE;
                    /* WARNING: Could not recover jumptable at 0x000320c4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(0x22,subsystems);
  return;
}



/* ===== FUNCTION efuse_init @ 000320d0 ===== */

undefined4 efuse_init(void)

{
  int __fd;
  void *pvVar1;
  uint *puVar2;
  undefined4 uVar3;
  int iVar4;
  
  efuse_bitmap = 0;
  DAT_00051b98 = 0;
  DAT_00051b9c = 0;
  DAT_00051ba0 = 0;
  DAT_00051ba4 = 0;
  DAT_00051ba8 = 0;
  DAT_00051bac = 0;
  DAT_00051bb0 = 0;
  __fd = open("/dev/mem",0x82);
  if (__fd < 0) {
    puts("open(/dev/mem) failed.");
    uVar3 = 0xffffffff;
  }
  else {
    pvVar1 = mmap((void *)0x0,0x40,3,1,__fd,0x18800000);
    if (pvVar1 != (void *)0x0) {
      munmap(pvVar1,0x40);
    }
    pvVar1 = mmap((void *)0x0,0xe50,3,1,__fd,0x18818000);
    if (pvVar1 == (void *)0x0) {
      puts("mmap base failed.");
      close(__fd);
      uVar3 = 0xffffffff;
    }
    else {
      puVar2 = (uint *)((int)pvVar1 + 0xe00);
      efuse_register = puVar2;
      if (efuse_inited_4146 == 0) {
        *(uint *)((int)pvVar1 + 0xe00) = *(uint *)((int)pvVar1 + 0xe00) & 0xfffffffc | 1;
        if (-1 < *(int *)((int)pvVar1 + 0xe08)) {
          iVar4 = 4999;
          do {
            iVar4 = iVar4 + -1;
            usleep(1000);
            puVar2 = efuse_register;
            if ((int)efuse_register[2] < 0) break;
          } while (iVar4 != 0);
        }
        efuse_inited_4146 = 1;
        puVar2[2] = puVar2[2];
      }
      *puVar2 = *puVar2 & 0xfffffffc | 1;
      iVar4 = 4999;
      if (-1 < (int)puVar2[2]) {
        while( true ) {
          iVar4 = iVar4 + -1;
          usleep(1000);
          puVar2 = efuse_register;
          if ((int)efuse_register[2] < 0) break;
          if (iVar4 == 0) {
            munmap(pvVar1,0xe50);
            if (__fd == 0) {
              return 0xffffffff;
            }
            close(__fd);
            return 0xffffffff;
          }
        }
      }
      efuse_bitmap = puVar2[4];
      DAT_00051b98 = puVar2[5];
      DAT_00051b9c = puVar2[6];
      DAT_00051ba0 = puVar2[7];
      DAT_00051ba4 = puVar2[8];
      DAT_00051ba8 = puVar2[9];
      DAT_00051bac = puVar2[10];
      DAT_00051bb0 = puVar2[0xb];
      puVar2[2] = puVar2[2];
      munmap(pvVar1,0xe50);
      if (__fd != 0) {
        close(__fd);
      }
      uVar3 = 0;
    }
  }
  return uVar3;
}



/* ===== FUNCTION CheckEncrypty @ 00032400 ===== */

undefined4 CheckEncrypty(int param_1)

{
  size_t sVar1;
  undefined4 uVar2;
  int iVar3;
  uint local_28;
  int local_24;
  int local_20;
  byte abStack_1c [20];
  
  local_24 = 0;
  local_28 = 0;
  while( true ) {
    sVar1 = strlen(key1);
    if (sVar1 <= local_28) break;
    if (local_28 < 8) {
      if (*(char *)(param_1 + local_28) !=
          (byte)((key1[local_28] ^ *(byte *)((int)&UniqueID + (local_28 & 7))) +
                *(char *)(param_1 + local_28 + 0xc0))) {
        local_24 = local_24 + 1;
      }
    }
    else if (*(char *)(param_1 + local_28) !=
             (byte)((key1[local_28] ^ *(byte *)(param_1 + (local_28 - 8))) +
                   *(char *)(param_1 + local_28 + 0xc0))) {
      local_24 = local_24 + 1;
    }
    local_28 = local_28 + 1;
  }
  if (local_24 == 0) {
    iVar3 = efuse_init();
    if (iVar3 < 0) {
      uVar2 = 0;
    }
    else {
      local_20 = 7;
      if (DAT_00051b9c._1_1_ == '@') {
        puts("this is D3000\r");
      }
      else if (DAT_00051b9c._1_1_ == 'A') {
        puts("this is D3100\r");
      }
      else if (DAT_00051b9c._1_1_ == 'E') {
        puts("this is D3101\r");
      }
      else {
        local_20 = 5;
      }
      sprintf((char *)abStack_1c,"%08x%08x",efuse_bitmap,DAT_00051b98);
      local_24 = 0;
      for (local_28 = 0; (int)local_28 < 0x10; local_28 = local_28 + 1) {
        if (local_28 < 8) {
          if (*(char *)(param_1 + Key3Offset + local_28) !=
              (byte)((abStack_1c[local_28] ^ *(byte *)((int)&UniqueID + (local_28 & 7))) +
                    *(char *)(param_1 + local_20 + local_28))) {
            local_24 = local_24 + 1;
          }
        }
        else if (*(char *)(param_1 + Key3Offset + local_28) !=
                 (byte)((abStack_1c[local_28] ^ *(byte *)(param_1 + Key3Offset + local_28 + -8)) +
                       *(char *)(param_1 + local_20 + local_28))) {
          local_24 = local_24 + 1;
        }
      }
      if (local_24 == 0) {
        uVar2 = 1;
      }
      else {
        uVar2 = 0;
      }
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION read_transfer_one @ 00032870 ===== */

void read_transfer_one(int param_1,undefined1 *param_2,undefined1 param_3,char param_4,int param_5,
                      int param_6,int param_7,uint param_8)

{
  uint local_10;
  
  *(undefined1 *)(param_1 + 0x98) = param_3;
  if (param_7 == 0) {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xfe;
  }
  else {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) | 1;
  }
  if (param_4 == '\0') {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xfd;
  }
  else {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) | 2;
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xcf | (param_4 - 1U & 3) << 4;
  }
  if (param_6 < 0) {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xfb;
    param_5 = 0;
  }
  else {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) | 4;
  }
  if (param_8 < 2) {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xbf;
  }
  else {
    *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) | 0x40;
  }
  for (local_10 = 0; local_10 < param_8; local_10 = local_10 + 1) {
    *(undefined1 *)(param_7 + local_10) = param_2[param_5 + local_10];
  }
  if (param_8 == 0) {
    *param_2 = 0;
    usleep(10000);
  }
  *(byte *)(param_1 + 0x99) = *(byte *)(param_1 + 0x99) & 0xbf;
  return;
}



/* ===== FUNCTION wait_empty @ 00032a98 ===== */

void wait_empty(undefined4 param_1,undefined4 param_2,int param_3)

{
  int local_10;
  byte local_c [4];
  
  local_10 = 0;
  read_transfer_one(param_1,param_2,5,0,0,0,local_c,1);
  do {
    if ((local_c[0] & 1) == 0) {
      return;
    }
    read_transfer_one(param_1,param_2,5,0,0,0,local_c,1);
    usleep(1000);
    local_10 = local_10 + 1;
  } while (local_10 != param_3);
  return;
}



/* ===== FUNCTION read_id @ 00032bcc ===== */

void read_id(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined1 auStack_10 [8];
  
  read_transfer_one(param_1,param_2,3,0,0x1000,0,auStack_10,1);
  read_transfer_one(param_1,param_2,0x9f,0,0xffffffff,0xffffffff,param_3,3);
  read_transfer_one(param_1,param_2,3,0,0x1000,0,auStack_10,1);
  wait_empty(param_1,param_2,1000);
  return;
}



/* ===== FUNCTION read_uid @ 00032d10 ===== */

undefined4 read_uid(undefined4 param_1,undefined4 param_2,undefined1 *param_3,undefined4 *param_4)

{
  byte bVar1;
  undefined1 auStack_10 [8];
  
  read_id(param_1,param_2,param_3);
  bVar1 = param_3[2];
  if (CONCAT12(*param_3,CONCAT11(param_3[1],bVar1)) == -0x3ddfe7) {
    read_transfer_one(param_1,param_2,0x5a,1,0x1e0,0,param_3,0x10);
    *param_4 = 0x10;
  }
  else {
    read_transfer_one(param_1,param_2,3,0,0x1000,0,auStack_10,1);
    read_transfer_one(param_1,param_2,0x4b,4,0xffffffff,0xffffffff,param_3,0x10);
    read_transfer_one(param_1,param_2,3,0,0x1000,0,auStack_10,1);
    if (bVar1 < 0x17) {
      *param_4 = 8;
    }
    else {
      *param_4 = 0x10;
    }
  }
  wait_empty(param_1,param_2,1000);
  return 0;
}



/* ===== FUNCTION read_otp @ 00032f68 ===== */

void read_otp(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined1 auStack_c [4];
  
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  read_id(param_1,param_2,&local_1c);
  if (CONCAT12((undefined1)local_1c,CONCAT11(local_1c._1_1_,local_1c._2_1_)) == -0x3ddfe7) {
    read_transfer_one(param_1,param_2,0xb1,0,0xffffffff,0xffffffff,0,0);
    read_transfer_one(param_1,param_2,3,0,0,0,param_3,0x20);
    read_transfer_one(param_1,param_2,0xc1,0,0xffffffff,0xffffffff,0,0);
  }
  else {
    read_transfer_one(param_1,param_2,3,0,0x2000,0,auStack_c,1);
    read_transfer_one(param_1,param_2,0x48,1,0x2000,0,param_3,0x100);
    read_transfer_one(param_1,param_2,3,0,0x2000,0,auStack_c,1);
  }
  wait_empty(param_1,param_2,1000);
  return;
}



/* ===== FUNCTION save_flash_st @ 000331f0 ===== */

void save_flash_st(int param_1)

{
  reg_98 = *(undefined1 *)(param_1 + 0x98);
  reg_99 = *(undefined1 *)(param_1 + 0x99);
  reg_9a = *(undefined1 *)(param_1 + 0x9a);
  reg_9b = *(undefined1 *)(param_1 + 0x9b);
  reg_9c = (char)*(undefined4 *)(param_1 + 0x9c);
  reg_c8 = (char)*(undefined4 *)(param_1 + 200);
  return;
}



/* ===== FUNCTION recover_flash_st @ 0003329c ===== */

void recover_flash_st(int param_1)

{
  *(undefined1 *)(param_1 + 0x98) = reg_98;
  *(undefined1 *)(param_1 + 0x99) = reg_99;
  *(undefined1 *)(param_1 + 0x9a) = reg_9a;
  *(undefined1 *)(param_1 + 0x9b) = reg_9b;
  *(uint *)(param_1 + 0x9c) = (uint)reg_9c;
  *(uint *)(param_1 + 200) = (uint)reg_c8;
  return;
}



/* ===== FUNCTION flash_init @ 00033338 ===== */

void flash_init(int param_1)

{
  *(undefined1 *)(param_1 + 0x98) = 3;
  *(undefined1 *)(param_1 + 0x99) = 0xd;
  *(undefined1 *)(param_1 + 0x9a) = 0;
  *(undefined1 *)(param_1 + 0x9b) = 1;
  *(undefined4 *)(param_1 + 0x9c) = 0;
  *(undefined4 *)(param_1 + 200) = 0;
  return;
}



/* ===== FUNCTION check_encrypty @ 000333a0 ===== */

undefined4 check_encrypty(void)

{
  int __fd;
  ssize_t sVar1;
  int __fd_00;
  void *__addr;
  void *__addr_00;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_140 [256];
  uint local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  int local_20 [2];
  
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  memset(auStack_140,0,0x100);
  local_20[0] = 0;
  __fd = open("/sys/devices/platform/soc/1882e000.spi/protect",2);
  if (__fd < 0) {
    perror("Open lock");
    return 0;
  }
  sVar1 = write(__fd,&DAT_0003641c,4);
  if (sVar1 < 1) {
    perror("Write lock");
    close(__fd);
    return 0;
  }
  __fd_00 = open("/dev/mem",0x82);
  if (__fd_00 < 0) {
    sVar1 = write(__fd,"unlock",6);
    if (0 < sVar1) {
LAB_000336ac:
      close(__fd);
      return 0;
    }
    iVar3 = -1;
  }
  else {
    __addr = mmap((void *)0x0,0x400000,3,1,__fd_00,0x1882e000);
    __addr_00 = mmap((void *)0x0,0x400000,3,1,__fd_00,0x1f000000);
    if ((__addr == (void *)0x0) || (__addr_00 == (void *)0x0)) {
      iVar3 = -1;
      close(__fd_00);
      sVar1 = write(__fd,"unlock",6);
      if (0 < sVar1) goto LAB_000336ac;
    }
    else {
      save_flash_st(__addr);
      flash_init(__addr,__addr_00);
      read_uid(__addr,__addr_00,&local_40,local_20);
      read_otp(__addr,__addr_00,auStack_140);
      recover_flash_st(__addr);
      munmap(__addr,0x400000);
      munmap(__addr_00,0x400000);
      close(__fd_00);
      sVar1 = write(__fd,"unlock",6);
      if (0 < sVar1) {
        close(__fd);
        goto LAB_000335b4;
      }
      iVar3 = 0;
    }
  }
  perror("Write unlock");
  close(__fd);
  if (iVar3 != 0) {
    return 0;
  }
LAB_000335b4:
  if (local_20[0] == 0x10) {
    local_40 = local_38 ^ local_40;
    local_3c = local_34 ^ local_3c;
  }
  UniqueID = local_40;
  DAT_00051bb8 = local_3c;
  uVar2 = CheckEncrypty(auStack_140);
  return uVar2;
}



/* ===== FUNCTION __do_global_ctors_aux @ 00033730 ===== */

void __do_global_ctors_aux(void)

{
  undefined4 *puVar1;
  code *pcVar2;
  
  if (__CTOR_LIST__ != (code *)0xffffffff) {
    puVar1 = &__CTOR_LIST__;
    pcVar2 = __CTOR_LIST__;
    do {
      puVar1 = puVar1 + -1;
      (*pcVar2)();
      pcVar2 = (code *)*puVar1;
    } while (pcVar2 != (code *)0xffffffff);
  }
  return;
}



/* ===== FUNCTION strcmp @ 00033790 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

{
  int iVar1;
  
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION operator.delete @ 000337a0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void operator_delete(void *param_1)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION free @ 000337b0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void free(void *__ptr)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION memcpy @ 000337c0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
  void *pvVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pvVar1 = (void *)(*(code *)0x35e0)();
  return pvVar1;
}



/* ===== FUNCTION fgets @ 000337d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * fgets(char *__s,int __n,FILE *__stream)

{
  char *pcVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pcVar1 = (char *)(*(code *)0x35e0)();
  return pcVar1;
}



/* ===== FUNCTION time @ 000337e0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

time_t time(time_t *__timer)

{
  time_t tVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  tVar1 = (*(code *)0x35e0)();
  return tVar1;
}



/* ===== FUNCTION memcmp @ 000337f0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int memcmp(void *__s1,void *__s2,size_t __n)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION __cxa_guard_release @ 00033800 ===== */

void __cxa_guard_release(void)

{
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION operator.new @ 00033810 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * operator_new(uint param_1)

{
  void *pvVar1;
  
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  (*(code *)(undefined *)0x0)();
  pvVar1 = (void *)(*(code *)0x35e0)();
  return pvVar1;
}



/* ===== FUNCTION perror @ 00033820 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void perror(char *__s)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION fwrite @ 00033830 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fwrite(void *__ptr,size_t __size,size_t __n,FILE *__s)

{
  size_t sVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  sVar1 = (*(code *)0x35e0)();
  return sVar1;
}



/* ===== FUNCTION usleep @ 00033840 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int usleep(__useconds_t __useconds)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION fread @ 00033850 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fread(void *__ptr,size_t __size,size_t __n,FILE *__stream)

{
  size_t sVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  sVar1 = (*(code *)0x35e0)();
  return sVar1;
}



/* ===== FUNCTION strcpy @ 00033860 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcpy(char *__dest,char *__src)

{
  char *pcVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pcVar1 = (char *)(*(code *)0x35e0)();
  return pcVar1;
}



/* ===== FUNCTION malloc @ 00033870 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * malloc(size_t __size)

{
  void *pvVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pvVar1 = (void *)(*(code *)0x35e0)();
  return pvVar1;
}



/* ===== FUNCTION puts @ 00033880 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int puts(char *__s)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION open @ 00033890 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int open(char *__file,int __oflag,...)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION _M_unhook @ 000338a0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_unhook(void)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION feof @ 000338b0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int feof(FILE *__stream)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION mmap @ 000338c0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
  void *pvVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pvVar1 = (void *)(*(code *)0x35e0)();
  return pvVar1;
}



/* ===== FUNCTION strlen @ 000338d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strlen(char *__s)

{
  size_t sVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  sVar1 = (*(code *)0x35e0)();
  return sVar1;
}



/* ===== FUNCTION fprintf @ 000338e0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fprintf(FILE *__stream,char *__format,...)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION write @ 000338f0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t write(int __fd,void *__buf,size_t __n)

{
  ssize_t sVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  sVar1 = (*(code *)0x35e0)();
  return sVar1;
}



/* ===== FUNCTION memset @ 00033900 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
  void *pvVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  pvVar1 = (void *)(*(code *)0x35e0)();
  return pvVar1;
}



/* ===== FUNCTION isalpha @ 00033910 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int isalpha(int param_1)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION munmap @ 00033920 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int munmap(void *__addr,size_t __len)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION toupper @ 00033930 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int toupper(int __c)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION fputc @ 00033940 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fputc(int __c,FILE *__stream)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION sprintf @ 00033950 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sprintf(char *__s,char *__format,...)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION _M_hook @ 00033960 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void std::__detail::_List_node_base::_M_hook(_List_node_base *param_1)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION _Unwind_Resume @ 00033970 ===== */

void _Unwind_Resume(void)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION close @ 00033980 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
  int iVar1;
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (**(code **)(unaff_gp + -0x7ff0))();
  iVar1 = (*(code *)0x35e0)();
  return iVar1;
}



/* ===== FUNCTION __cxa_guard_acquire @ 00033990 ===== */

void __cxa_guard_acquire(void)

{
  int unaff_gp;
  
  (**(code **)(unaff_gp + -0x7ff0))();
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION _fini @ 000339b0 ===== */

void _fini(void)

{
  (*(code *)0x35e0)();
  return;
}



/* ===== FUNCTION _ITM_registerTMCloneTable @ 00052000 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _ITM_registerTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _Jv_RegisterClasses @ 00052004 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _Jv_RegisterClasses(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __gmon_start__ @ 00052008 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __gmon_start__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _ITM_deregisterTMCloneTable @ 0005200c ===== */

/* WARNING: Control flow encountered bad instruction data */

void _ITM_deregisterTMCloneTable(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __cxa_finalize @ 00052010 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __cxa_finalize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
                    /* __cxa_finalize@@GLIBC_2.2 */
  halt_baddata();
}


