
/* ===== KEY retro_init @ 00031064 function=retro_init ===== */

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



/* ===== KEY retro_load_game @ 0003115c function=retro_load_game ===== */

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



/* ===== KEY retro_run @ 00031b30 function=retro_run ===== */

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



/* ===== KEY retro_reset @ 00031a84 function=retro_reset ===== */

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



/* ===== KEY retro_unload_game @ 0003199c function=retro_unload_game ===== */

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



/* ===== KEY retro_get_system_info @ 00030f08 function=retro_get_system_info ===== */

void retro_get_system_info(undefined4 *param_1)

{
  *(undefined1 *)(param_1 + 3) = 0;
  *param_1 = "TGB Dual";
  param_1[1] = "v0.8.3 9be31d3";
  param_1[2] = "gb|gbc|sgb";
  return;
}



/* ===== KEY retro_get_system_av_info @ 00030f40 function=retro_get_system_av_info ===== */

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



/* ===== KEY retro_set_environment @ 000320a4 function=retro_set_environment ===== */

void retro_set_environment(code *UNRECOVERED_JUMPTABLE)

{
  environ_cb = UNRECOVERED_JUMPTABLE;
                    /* WARNING: Could not recover jumptable at 0x000320c4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(0x22,subsystems);
  return;
}



/* ===== KEY retro_set_video_refresh @ 0003203c function=retro_set_video_refresh ===== */

void retro_set_video_refresh(undefined4 param_1)

{
  video_cb = param_1;
  return;
}



/* ===== KEY retro_set_input_state @ 0003208c function=retro_set_input_state ===== */

void retro_set_input_state(undefined4 param_1)

{
  input_state_cb = param_1;
  return;
}



/* ===== KEY render @ 00051b58 function=<none> ===== */

/* ===== KEY render @ 0002c244 function=render ===== */

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



/* ===== KEY render @ 00014510 function=render ===== */

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



/* ===== KEY SetFrameSkip @ 00031ae8 function=SetFrameSkip ===== */

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



/* ===== KEY check_encrypty @ 000333a0 function=check_encrypty ===== */

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



/* ===== KEY CheckEncrypty @ 00032400 function=CheckEncrypty ===== */

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



/* ===== KEY wait_empty @ 00032a98 function=wait_empty ===== */

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


