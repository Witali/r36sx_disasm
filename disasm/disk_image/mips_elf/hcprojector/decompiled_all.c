
/* ===== FUNCTION _init @ 00405664 ===== */

int _init(EVP_PKEY_CTX *ctx)

{
  int iVar1;
  
  __gmon_start__();
  FUN_004063e4();
  iVar1 = FUN_0044ee90();
  return iVar1;
}



/* ===== FUNCTION _ftext @ 004056d0 ===== */

void _ftext(void)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  code *pcVar4;
  int iVar5;
  int iVar6;
  pthread_attr_t local_a0;
  pthread_t local_7c;
  undefined4 local_78;
  char local_74;
  undefined4 local_70;
  undefined *local_6c;
  int local_68;
  undefined *local_64;
  undefined *local_60;
  undefined *local_5c;
  undefined *local_58;
  int local_54;
  undefined4 local_50;
  undefined *local_4c;
  undefined **local_48;
  undefined *local_44;
  undefined *local_40;
  undefined *local_3c;
  char *local_38;
  char *local_34;
  char *local_30;
  
  FUN_0042ec0c();
  puts("\n\n ********* Welcome to Hichip world! *********\n");
  FUN_0042b8a0();
  FUN_00429318();
  FUN_00427724();
  FUN_00427d80();
  lv_fb_hotplug_support_set(0);
  hc_lvgl_init();
  DAT_012169d8 = open("/dev/input/event0",0);
  open("/dev/input/event1",0);
  lv_indev_drv_init(&DAT_012169a4);
  DAT_012169a4 = 2;
  DAT_012169c0 = 0x280050;
  DAT_012169a8 = FUN_00406478;
  _edata = lv_indev_drv_register(&DAT_012169a4);
  local_40 = &DAT_01210000;
  DAT_012169d4 = lv_group_create();
  local_3c = &DAT_01210000;
  lv_group_set_default(DAT_012169d4);
  lv_indev_set_group(_edata,DAT_012169d4);
  FUN_0041a248(6);
  FUN_0044d690();
  uVar2 = lv_disp_get_default();
  lv_palette_main(&local_70,5);
  uVar3 = local_70;
  lv_palette_main(&local_70,0);
  uVar3 = lv_theme_default_init(uVar2,uVar3,local_70,0,lv_font_montserrat_14);
  lv_disp_set_theme(uVar2,uVar3);
  FUN_0042b4c0();
  uVar3 = lv_disp_get_default();
  uVar3 = lv_disp_get_scr_act(uVar3);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  FUN_00406bc0(0x280,0x1e0);
  uVar3 = FUN_00428454(0x14);
  FUN_00433d28(uVar3);
  DAT_0121699c = 6;
  DAT_01216998 = 6;
  FUN_00406afc();
  uVar3 = FUN_00428454(0x13);
  FUN_004067ac(uVar3);
  local_48 = &switchD_00405bf4::switchdataD_0044f090;
  local_74 = '\x01';
  local_44 = &DAT_01210000;
  local_68 = 0x1220000;
  local_6c = &DAT_01210000;
  local_64 = &DAT_01210000;
  local_60 = &DAT_01210000;
  local_5c = &DAT_01210000;
  local_38 = "nd bgm path";
  local_4c = &DAT_011f0000;
  local_58 = &DAT_01210000;
  local_54 = 0x400000;
  local_34 = "MSG_TYPE_POWER_SCREEN_UNLOCK";
  local_30 = "MSG_TYPE_WAKE_UP";
  do {
    if ((DAT_0121699c != DAT_01216998) && (DAT_012169d0 != 0)) {
      FUN_00440608();
      if (DAT_0121699c < 8) {
        switch((uint)local_48[DAT_0121699c] & 0xfffffffe) {
        case 0x405bfc:
          iVar5 = *(int *)(local_68 + -0x2c54);
          if (*(int *)(local_68 + -0x2c54) == 0) {
            FUN_00441640();
            iVar5 = *(int *)(local_68 + -0x2c54);
          }
          break;
        case 0x405c40:
          goto switchD_00405bf4_caseD_405c40;
        case 0x405c48:
          iVar5 = DAT_01216ae4;
          if (DAT_01216ae4 == 0) {
            FUN_00408fc8();
            iVar5 = DAT_01216ae4;
          }
          break;
        case 0x405c64:
          if (DAT_012171f0 == 0) {
            FUN_00416f78();
          }
          DAT_0121699c = 2;
          DAT_01216998 = 2;
          FUN_004286f8(0x13,2);
          iVar5 = DAT_012171f0;
          break;
        case 0x405c90:
          iVar5 = FUN_00415104();
          if (iVar5 == 1) {
            iVar5 = *(int *)(local_60 + 0x7214);
            if (iVar5 == 0) {
              FUN_00416ea0();
              iVar5 = *(int *)(local_60 + 0x7214);
            }
          }
          else if (iVar5 == 0) {
            iVar5 = *(int *)(local_64 + 0x7218);
            if (iVar5 == 0) {
              FUN_00416e30();
              iVar5 = *(int *)(local_64 + 0x7218);
            }
          }
          else if (iVar5 == 2) {
            iVar5 = *(int *)(local_5c + 0x67d8);
            if (iVar5 == 0) {
              FUN_0041dce4();
              iVar5 = *(int *)(local_5c + 0x67d8);
            }
          }
          else {
            if (iVar5 != 3) goto switchD_00405bf4_caseD_405c40;
            if (DAT_012171f0 == 0) {
              FUN_00416f78();
              iVar5 = *(int *)(local_64 + 0x7218);
            }
            else {
              iVar5 = *(int *)(local_64 + 0x7218);
            }
            if (iVar5 == 0) {
              FUN_00416e30();
            }
            if (DAT_0121720c == 0) {
              FUN_00416f0c();
            }
            iVar6 = FUN_0041d32c();
            iVar5 = DAT_0121720c;
            if ((iVar6 == 0) && (iVar6 = FUN_0040b598(), iVar5 = DAT_012171f0, iVar6 == 0)) {
              iVar5 = *(int *)(local_64 + 0x7218);
            }
          }
          break;
        case 0x405cf4:
          if (DAT_0121720c == 0) {
            FUN_00416f0c();
          }
          DAT_0121699c = 2;
          DAT_01216998 = 2;
          FUN_004286f8(0x13,2);
          iVar5 = DAT_0121720c;
          break;
        case 0x405d20:
          iVar5 = *(int *)(local_6c + 0x69e4);
          if (iVar5 == 0) {
            FUN_004071cc();
            iVar5 = *(int *)(local_6c + 0x69e4);
          }
          uVar3 = lv_disp_get_default();
          uVar3 = lv_disp_get_scr_act(uVar3);
          *(undefined4 *)(local_44 + 0x69c8) = uVar3;
          lv_scr_load_anim(iVar5,7,0,0,0);
          FUN_0041238c(1);
          FUN_004227e4(0,0);
          FUN_0040e450(1);
          DAT_01216998 = DAT_0121699c;
          goto LAB_00405a48;
        }
        uVar3 = lv_disp_get_default();
        uVar3 = lv_disp_get_scr_act(uVar3);
        *(undefined4 *)(local_44 + 0x69c8) = uVar3;
        lv_scr_load_anim(iVar5,7,0,0,0);
      }
switchD_00405bf4_caseD_405c40:
      DAT_01216998 = DAT_0121699c;
    }
LAB_00405a48:
    local_a0.__align = 0;
    local_a0._4_4_ = 0;
    local_7c = 0;
    local_78 = 0;
    uVar3 = lv_disp_get_default();
    iVar5 = lv_disp_get_scr_act(uVar3);
    iVar6 = FUN_0042c630(&local_7c);
    if (iVar6 == 0) {
LAB_00405938:
      if (iVar5 != 0) goto LAB_00405940;
LAB_00405960:
      if (local_7c == 0x69) {
        iVar5 = FUN_00428454(0x38);
        FUN_0042ee6c();
        if (iVar5 == 3) {
          uVar3 = *(undefined4 *)(local_3c + 0x69d4);
          DAT_0121699c = 5;
        }
        else {
          uVar3 = *(undefined4 *)(local_3c + 0x69d4);
          DAT_0121699c = 4;
        }
        lv_group_set_default(uVar3);
        lv_indev_set_group(*(undefined4 *)(local_40 + 0x67b0),uVar3);
      }
      else if (local_7c < 0x6a) {
        if (local_7c < 0x14) {
          if ((0x10 < local_7c) || (local_7c - 0xe < 2)) {
LAB_00405dc4:
            FUN_0042b34c(local_7c,local_78);
            FUN_00406948(local_7c);
          }
        }
        else if (local_7c == 0x15) goto LAB_00405dc4;
      }
      else if (local_7c == 0x84) {
LAB_00405ee0:
        puts(local_38 + -0xfc0);
        iVar5 = FUN_0041d32c();
        if (iVar5 != 0) {
          iVar6 = *(int *)(iVar5 + 4);
          *(int *)(local_4c + -0x6510) = iVar6;
          if (iVar6 != 2) {
            FUN_004247bc(iVar5);
          }
          iVar5 = FUN_0042291c();
          if (iVar5 != 0) {
            FUN_004247bc(iVar5);
          }
        }
      }
      else if (local_7c < 0x85) {
        if (local_7c == 0x82) goto LAB_00405e98;
        if (local_7c == 0x83) goto LAB_0040599c;
      }
      else if (local_7c == 0x85) {
LAB_00405e3c:
        puts(local_30);
        iVar5 = FUN_0041d32c();
        if (iVar5 != 0) {
          if (*(int *)(local_4c + -0x6510) == 1) {
            FUN_00424888(iVar5);
          }
          iVar5 = FUN_0042291c();
          if (iVar5 != 0) {
            FUN_00424888(iVar5);
          }
        }
      }
      else if (local_7c == 0x86) goto LAB_00405ac4;
    }
    else {
      iVar6 = FUN_0042c660(&local_a0);
      if (iVar6 != 0) goto LAB_00405960;
      if (local_a0.__align == 0x106) {
        local_7c = 0x82;
        local_78 = local_a0._4_4_;
        if (iVar5 != 0) {
LAB_00405940:
          pcVar4 = (code *)FUN_0042cfc0(iVar5);
          if (pcVar4 != (code *)0x0) {
            (*pcVar4)(&local_7c,0);
          }
          goto LAB_00405960;
        }
LAB_00405e98:
        FUN_004214f8(0);
        FUN_0042f134(0x280,0x1e0);
        fbdev_set_rotate(0,0,0);
        FUN_00433eec();
      }
      else if (local_a0.__align == 0x107) {
        local_7c = 0x83;
        local_78 = local_a0._4_4_;
        if (iVar5 != 0) goto LAB_00405940;
LAB_0040599c:
        FUN_004214f8(1);
        FUN_0042f134(0x280,0x1e0);
        uVar3 = FUN_0042b734();
        local_50 = FUN_0042b888();
        uVar2 = FUN_0042b894();
        fbdev_set_rotate(uVar3,local_50,uVar2);
        FUN_00433eec();
      }
      else {
        if (local_a0.__align == 0x109) {
          local_7c = 0x84;
          local_78 = local_a0._4_4_;
          if (iVar5 != 0) goto LAB_00405940;
          goto LAB_00405ee0;
        }
        if (local_a0.__align == 0x10a) {
          local_7c = 0x85;
          local_78 = local_a0._4_4_;
          if (iVar5 != 0) goto LAB_00405940;
          goto LAB_00405e3c;
        }
        if (local_a0.__align != 0x10d) goto LAB_00405938;
        local_7c = 0x86;
        local_78 = local_a0._4_4_;
        if (iVar5 != 0) goto LAB_00405940;
LAB_00405ac4:
        puts(local_34);
        FUN_00420b60();
      }
    }
    iVar5 = DAT_012169a0 + 1;
    bVar1 = 0x50 < DAT_012169a0;
    DAT_012169a0 = iVar5;
    if (bVar1) {
      FUN_0042e530();
      DAT_012169a0 = 0;
    }
    uVar3 = lv_disp_get_default();
    DAT_012169c4 = lv_disp_get_scr_act(uVar3);
    lv_timer_handler();
    if (local_74 != '\0') {
      FUN_0042c530(0);
      if (*(int *)(local_68 + -0x2c54) == 0) {
        FUN_00441640();
      }
      if (DAT_01216ae4 == 0) {
        FUN_00408fc8();
      }
      if (*(int *)(local_58 + 0x6ac8) == 0) {
        FUN_00408b1c();
      }
      if (*(int *)(local_6c + 0x69e4) == 0) {
        FUN_004071cc();
      }
      if (*(int *)(local_64 + 0x7218) == 0) {
        FUN_00416e30();
      }
      if (*(int *)(local_60 + 0x7214) == 0) {
        FUN_00416ea0();
      }
      if (*(int *)(local_5c + 0x67d8) == 0) {
        FUN_0041dce4();
      }
      if (DAT_0121720c == 0) {
        FUN_00416f0c();
      }
      if (DAT_012171f0 == 0) {
        FUN_00416f78();
      }
      local_7c = 0;
      pthread_attr_init(&local_a0);
      pthread_attr_setstacksize(&local_a0,0x2000);
      pthread_attr_setdetachstate(&local_a0,1);
      pthread_create(&local_7c,&local_a0,(__start_routine *)(local_54 + 0x6430),(void *)0x0);
      pthread_attr_destroy(&local_a0);
      local_74 = '\0';
    }
    usleep(10000);
  } while( true );
}



/* ===== FUNCTION entry @ 00406260 ===== */

void processEntry entry(undefined4 param_1,undefined4 param_2)

{
  undefined1 auStack_20 [16];
  undefined1 *local_10;
  undefined4 local_c;
  undefined1 *local_8;
  
  local_8 = auStack_20;
  local_10 = &LAB_0044ee84;
  local_c = param_1;
  __libc_start_main(_ftext,param_2,&stack0x00000004,&LAB_0044ede0);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004062b0 @ 004062b0 ===== */

/* WARNING: Removing unreachable block (ram,0x004062d0) */
/* WARNING: Removing unreachable block (ram,0x004062dc) */

void FUN_004062b0(void)

{
  return;
}



/* ===== FUNCTION FUN_004062ec @ 004062ec ===== */

/* WARNING: Removing unreachable block (ram,0x00406318) */
/* WARNING: Removing unreachable block (ram,0x00406324) */

void FUN_004062ec(void)

{
  return;
}



/* ===== FUNCTION FUN_00406334 @ 00406334 ===== */

/* WARNING: Removing unreachable block (ram,0x00406390) */

void FUN_00406334(void)

{
  if (DAT_01216980 == '\0') {
    FUN_004062b0();
    DAT_01216980 = '\x01';
  }
  return;
}



/* ===== FUNCTION FUN_004063e4 @ 004063e4 ===== */

/* WARNING: Removing unreachable block (ram,0x0040640c) */

void FUN_004063e4(void)

{
  FUN_004062ec();
  return;
}



/* ===== FUNCTION FUN_00406478 @ 00406478 ===== */

void FUN_00406478(undefined4 param_1,int param_2)

{
  ushort uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  
  iVar2 = FUN_0042a25c();
  if (iVar2 == 0) {
    return;
  }
  if (*(int *)(iVar2 + 0x10) == 1) {
    uVar5 = (uint)*(ushort *)(iVar2 + 0xe);
    iVar3 = FUN_0042d430(uVar5);
    if (iVar3 == 0) {
      *(uint *)(param_2 + 0x10) = (uint)(uVar5 != 0);
    }
    else {
      if (((uVar5 != 0) && (FUN_0044b698(), DAT_011e9af4 = uVar5, DAT_012169cc == '\0')) &&
         (uVar5 != 0x74)) {
        if (uVar5 == 0x8b) {
          puts(">>Channel key");
          *(undefined4 *)(param_2 + 0x10) = 0;
          DAT_0121699c = 6;
          uVar5 = 0;
          goto LAB_004064fc;
        }
        if (uVar5 == 0x16d) {
          puts(">>Setup key");
          DAT_0121699c = 7;
          uVar4 = lv_disp_get_default();
          iVar3 = lv_disp_get_scr_act(uVar4);
          if (DAT_0121d3ac == iVar3) goto LAB_00406674;
        }
        else {
          if ((uVar5 != 0x99) && (uVar5 != 0x18e)) {
            if (uVar5 == 399) {
              if (DAT_0121699c != 7) goto LAB_004065a8;
            }
            else if (uVar5 == 0x66) {
              *(undefined4 *)(param_2 + 0x10) = 1;
              DAT_0121699c = 3;
              goto LAB_004064fc;
            }
LAB_00406674:
            *(undefined4 *)(param_2 + 0x10) = 1;
            goto LAB_004064fc;
          }
          printf("act_key %d\n",uVar5);
          FUN_0042ecf4();
        }
      }
LAB_004065a8:
      *(undefined4 *)(param_2 + 0x10) = 0;
      uVar5 = 0;
    }
  }
  else {
    if (*(int *)(iVar2 + 0x10) != 0) {
      uVar5 = 0;
      if (*(short *)(iVar2 + 0xe) != 0x74) {
        *(undefined4 *)(param_2 + 4) = 0x10000;
        return;
      }
      goto LAB_004064c0;
    }
    uVar1 = *(ushort *)(iVar2 + 0xe);
    *(undefined4 *)(param_2 + 0x10) = 0;
    FUN_00421464(*(undefined2 *)(iVar2 + 0xe));
    FUN_0040fb8c(*(undefined2 *)(iVar2 + 0xe));
    uVar5 = (uint)uVar1;
  }
LAB_004064fc:
  if ((*(short *)(iVar2 + 0xe) == 0x74) && (*(int *)(iVar2 + 0x10) == 0)) {
    FUN_00406d30();
  }
  if (uVar5 == 0x6c) {
    *(undefined4 *)(param_2 + 4) = 0x12;
    return;
  }
  if (uVar5 < 0x6d) {
    if (uVar5 == 0x67) {
      *(undefined4 *)(param_2 + 4) = 0x11;
      return;
    }
    if (uVar5 < 0x68) {
      if (uVar5 == 1) goto LAB_00406540;
      if (uVar5 == 0x1c) {
        *(undefined4 *)(param_2 + 4) = 10;
        return;
      }
    }
    else {
      if (uVar5 == 0x69) {
        *(undefined4 *)(param_2 + 4) = 0x14;
        return;
      }
      if (uVar5 == 0x6a) {
        *(undefined4 *)(param_2 + 4) = 0x13;
        return;
      }
    }
  }
  else {
    if (uVar5 == 0x16d) {
      *(undefined4 *)(param_2 + 4) = 2;
      return;
    }
    if (uVar5 < 0x16e) {
      if (uVar5 == 0xae) {
LAB_00406540:
        *(undefined4 *)(param_2 + 4) = 0x1b;
        return;
      }
      if (uVar5 == 0x160) {
        *(undefined4 *)(param_2 + 4) = 10;
        return;
      }
    }
    else {
      if (uVar5 == 0x197) {
        *(undefined4 *)(param_2 + 4) = 9;
        return;
      }
      if (uVar5 == 0x19c) {
        *(undefined4 *)(param_2 + 4) = 0xb;
        return;
      }
    }
  }
LAB_004064c0:
  *(uint *)(param_2 + 4) = uVar5 | 0x10000;
  return;
}



/* ===== FUNCTION FUN_00406770 @ 00406770 ===== */

void FUN_00406770(undefined1 param_1)

{
  DAT_012169cc = param_1;
  return;
}



/* ===== FUNCTION FUN_0040677c @ 0040677c ===== */

void FUN_0040677c(undefined4 param_1)

{
  lv_group_set_default();
  lv_indev_set_group(_edata,param_1);
  return;
}



/* ===== FUNCTION FUN_004067ac @ 004067ac ===== */

void FUN_004067ac(uint param_1)

{
  undefined4 uVar1;
  
  uVar1 = DAT_012169d4;
  if ((param_1 != 0) &&
     ((DAT_0121699c = param_1, 7 < param_1 || ((1 << (param_1 & 0x1f) & 0xc9U) == 0)))) {
    lv_group_set_default(DAT_012169d4);
    lv_indev_set_group(_edata,uVar1);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_00406818 @ 00406818 ===== */

void FUN_00406818(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = lv_disp_get_default();
  DAT_012169c8 = lv_disp_get_scr_act(uVar1);
  lv_scr_load_anim(param_1,7,param_2,param_3,0);
  return;
}



/* ===== FUNCTION FUN_0040687c @ 0040687c ===== */

void FUN_0040687c(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = DAT_012169c8;
  if (DAT_012169c8 != 0) {
    uVar2 = lv_disp_get_default();
    DAT_012169c8 = lv_disp_get_scr_act(uVar2);
    lv_scr_load_anim(iVar1,7,0,0,0);
  }
  return;
}



/* ===== FUNCTION FUN_004068f4 @ 004068f4 ===== */

void FUN_004068f4(void)

{
  int iVar1;
  
  DAT_01216990 = DAT_01216990 ^ 1;
  iVar1 = FUN_0042d540(DAT_01216990);
  if (iVar1 != 0) {
    DAT_01216990 = DAT_01216990 ^ 1;
    return;
  }
  FUN_0044e6b0();
  return;
}



/* ===== FUNCTION FUN_00406948 @ 00406948 ===== */

void FUN_00406948(uint param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  iVar1 = FUN_0042b4b4();
  if ((iVar1 != 0) && (printf(">>!%s ,%d\n","stroage_hotplug_handle",0x336), param_1 != 0xe)) {
    if (param_1 == 0x12) {
      if (DAT_012169d0 == 0) {
        FUN_00434140();
        DAT_012169d0 = 1;
        iVar1 = FUN_004155dc();
        if ((iVar1 == 2) || (iVar1 = FUN_004155dc(), iVar1 == 3)) {
          FUN_00422980();
          return;
        }
      }
    }
    else if (((param_1 < 0x16) && ((int)((0x2a8000U >> (param_1 & 0x1f)) << 0x1f) < 0)) &&
            ((iVar1 = FUN_0042e3dc(), iVar1 != 0 || (iVar1 = FUN_0041d160(), iVar1 != 0)))) {
      uVar2 = lv_disp_get_default();
      iVar1 = lv_disp_get_scr_act(uVar2);
      if (DAT_0121720c == iVar1) {
        iVar1 = FUN_00420c08();
        if (iVar1 != 0) {
          FUN_00420b60();
        }
        FUN_00412278();
      }
      else {
        FUN_0040f3ac();
      }
      FUN_0041d16c(0);
      FUN_0040a544();
      FUN_0041ff18();
      FUN_00423bb8();
      uVar2 = FUN_0041da2c();
      FUN_0041db84(uVar2);
      uVar2 = DAT_012167d8;
      uVar3 = lv_disp_get_default();
      DAT_012169c8 = lv_disp_get_scr_act(uVar3);
      lv_scr_load_anim(uVar2,7,0,0,0);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00406afc @ 00406afc ===== */

void FUN_00406afc(void)

{
  uint uVar1;
  undefined4 uVar2;
  
  FUN_00426904();
  uVar1 = FUN_004268ec();
  if (uVar1 == 2) {
    FUN_004286f8(0x13,2);
    uVar2 = 1;
  }
  else if (uVar1 < 3) {
    if (uVar1 != 1) goto LAB_00406b58;
    FUN_004286f8(0x13,2);
    uVar2 = 0;
  }
  else if (uVar1 == 3) {
    FUN_004286f8(0x13,2);
    uVar2 = 2;
  }
  else {
    if (uVar1 != 4) goto LAB_00406b58;
    FUN_004286f8(0x13,2);
    uVar2 = 3;
  }
  FUN_004155e8(uVar2);
  FUN_00415110(2);
LAB_00406b58:
  uVar2 = FUN_004268e0();
  FUN_004286f8(0x11,uVar2);
  FUN_00428080();
  return;
}



/* ===== FUNCTION FUN_00406bc0 @ 00406bc0 ===== */

void FUN_00406bc0(undefined4 param_1,undefined4 param_2)

{
  int __fd;
  int iVar1;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  undefined4 local_94;
  undefined4 local_88;
  undefined4 local_7c;
  undefined4 local_70;
  uint local_64;
  
  __fd = open("/dev/fb0",2);
  if (__fd == -1) {
    printf("Error: cannot open framebuffer device");
    return;
  }
  iVar1 = ioctl(__fd,0x4600,&local_b8);
  if (iVar1 != -1) {
    local_a0 = 0x20;
    local_a8 = 0;
    local_a4 = 0;
    local_94 = 8;
    local_88 = 8;
    local_7c = 8;
    local_70 = 8;
    local_64 = local_64 | 0x80;
    local_b8 = param_1;
    local_b4 = param_2;
    local_b0 = param_1;
    local_ac = param_2;
    printf("screen resolution:%dx%d\n",param_1,param_2);
    iVar1 = ioctl(__fd,0x4601,&local_b8);
    if (iVar1 != -1) {
      close(__fd);
      return;
    }
    perror("Error writing fixed information");
    close(__fd);
    return;
  }
  printf("Error reading variable information");
  close(__fd);
  return;
}



/* ===== FUNCTION FUN_00406d30 @ 00406d30 ===== */

void FUN_00406d30(void)

{
  int iVar1;
  undefined4 local_10 [2];
  
  local_10[0] = 0;
  FUN_0042e248(0);
  FUN_0042ca20(0);
  FUN_0042c430();
  FUN_0042c530(0);
  FUN_00408054();
  FUN_00412278();
  FUN_00428080();
  puts("close lcd/backlight/ etc.");
  iVar1 = open("/dev/lvds",2);
  if (iVar1 != 0) {
    ioctl(iVar1,0x20001314,0);
    close(iVar1);
  }
  iVar1 = open("/dev/mipi",2);
  if (iVar1 != 0) {
    ioctl(iVar1,0x20001e0c,0);
    close(iVar1);
  }
  iVar1 = open("/dev/lcddev",2);
  if (iVar1 != 0) {
    local_10[0] = 0;
    write(iVar1,local_10,4);
    close(iVar1);
  }
  FUN_0042c4b0();
  FUN_0042c930(100);
  iVar1 = open("/dev/standby",2);
  if (iVar1 < 0) {
    puts("Open /dev/standby failed!");
    return;
  }
  puts("enter standby!");
  FUN_0042e4ac();
  ioctl(iVar1,0x20002703,0);
  close(iVar1);
  return;
}



/* ===== FUNCTION FUN_00406ee8 @ 00406ee8 ===== */

void FUN_00406ee8(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    if ((DAT_012169e0 == 0) || (iVar3 = lv_group_get_focused(), iVar2 != iVar3)) {
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    }
    else {
      *(undefined4 *)(*(int *)(iVar1 + 0x10) + 0x34) = 0xffffffff;
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 4;
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x3a) = 0;
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    }
    return;
  }
  uVar4 = lv_indev_get_act();
  uVar5 = lv_indev_get_key(uVar4);
  uVar5 = uVar5 & 0xff;
  uVar6 = lv_obj_get_index(iVar2);
  if (uVar5 == 0x14) {
    if (uVar6 == 0) {
      iVar1 = 3;
    }
    else {
      iVar1 = 2;
      if (uVar6 != 1) {
        iVar1 = uVar6 - 2;
      }
    }
  }
  else if (uVar5 == 0x13) {
    if (uVar6 == 2) {
      iVar1 = 1;
    }
    else {
      iVar1 = uVar6 + 2;
      if (uVar6 == 3) {
        iVar1 = 0;
      }
    }
  }
  else {
    if (uVar5 == 10) {
      if (iVar2 == DAT_012169fc) {
        FUN_004067ac(2);
        FUN_004286f8(0x13,2);
      }
      else if (iVar2 == DAT_012169f8) {
        FUN_004067ac(7);
      }
      FUN_00428080();
      return;
    }
    if (1 < uVar5 - 0x11) {
      return;
    }
    uVar5 = uVar6 & 0x80000001;
    if ((int)uVar5 < 0) {
      uVar5 = (uVar5 - 1 | 0xfffffffe) + 1;
    }
    if (uVar5 == 1) {
      iVar1 = uVar6 - 1;
    }
    else {
      if (2 < (int)uVar6) {
        return;
      }
      iVar1 = uVar6 + 1;
    }
  }
  lv_group_focus_obj(*(undefined4 *)(&PTR_DAT_011e9b20)[iVar1]);
  return;
}



/* ===== FUNCTION FUN_00407178 @ 00407178 ===== */

void FUN_00407178(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = lv_event_get_code();
  lv_event_get_target(param_1);
  if (iVar1 != 0x27) {
    return;
  }
  FUN_0040677c(DAT_012169e0);
  return;
}



/* ===== FUNCTION FUN_004071cc @ 004071cc ===== */

void FUN_004071cc(void)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  undefined2 *puVar10;
  uint uVar11;
  byte *pbVar12;
  byte *pbVar13;
  int iVar14;
  undefined **ppuVar15;
  undefined4 *puVar16;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  uint local_70 [4];
  byte local_60 [32];
  undefined4 local_40;
  undefined1 *local_3c;
  int local_38;
  undefined *local_34;
  undefined4 local_30;
  
  pbVar12 = local_60;
  pbVar13 = local_60 + 0x10;
  DAT_012169e4 = lv_obj_create(0);
  DAT_012169e0 = lv_group_create();
  local_30 = lv_group_get_default();
  lv_group_set_default(DAT_012169e0);
  lv_obj_add_event_cb(DAT_012169e4,FUN_00407178,0,0);
  local_40 = DAT_012169e4;
  local_3c = &LAB_00406ee0;
  FUN_0042cef8(&local_40);
  iVar5 = lv_disp_get_hor_res(0);
  iVar6 = lv_disp_get_ver_res(0);
  printf("h is: %d, v is: %d\n",iVar5,iVar6);
  local_34 = &DAT_01210000;
  uVar7 = lv_obj_create(DAT_012169e4);
  lv_obj_set_style_border_width(uVar7,0,0);
  lv_obj_set_style_border_width(uVar7,0,0);
  sVar4 = (short)(iVar6 >> 0x1f);
  iVar14 = (int)(short)((short)((ulonglong)((longlong)iVar6 * 0x2aaaaaab) >> 0x20) - sVar4);
  lv_obj_set_style_pad_top(uVar7,iVar14,0);
  lv_obj_set_style_pad_bottom(uVar7,iVar14,0);
  local_38 = (int)(short)(iVar5 / 9);
  lv_obj_set_style_pad_left(uVar7,local_38,0);
  lv_obj_set_style_pad_right(uVar7,local_38,0);
  lv_obj_set_style_bg_color(uVar7,0xff516475,0);
  lv_obj_align(uVar7,9,0,0);
  lv_obj_set_size(uVar7,iVar5,iVar6);
  lv_obj_set_style_radius(uVar7,0,0);
  lv_obj_add_event_cb(*(undefined4 *)(local_34 + 0x69e4),&LAB_0040711c,0,0);
  lv_obj_set_flex_flow(uVar7,5);
  lv_obj_set_flex_align(uVar7,3,0,0);
  uVar11 = ((iVar5 / 9) * 7) / 2;
  lv_obj_set_style_pad_row(uVar7,(int)(short)(iVar6 / 0x12),0);
  uVar9 = uVar11 & 0xffff;
  uVar3 = uVar9 / 7;
  lv_obj_set_style_pad_column
            (uVar7,(int)(short)(((uVar3 + (uVar9 - uVar3 >> 1)) * 0x4000 >> 0x10) << 1),0);
  local_60[0x10] = 0x85;
  local_60[0x11] = 0;
  local_60[0x12] = 0;
  local_60[0x13] = 0;
  local_70[2] = 0x3c;
  local_60[0x14] = 0x5b;
  local_60[0x15] = 0;
  local_60[0x16] = 0;
  local_60[0x17] = 0;
  local_60[0x1c] = 0x3c;
  local_60[0x1d] = 0;
  local_60[0x1e] = 0;
  local_60[0x1f] = 0;
  local_60[0x18] = 0x51;
  local_60[0x19] = 0;
  local_60[0x1a] = 0;
  local_60[0x1b] = 0;
  local_70[3] = 0x80;
  local_60[0] = 0x69;
  local_60[1] = 0;
  local_60[2] = 0;
  local_60[3] = 0;
  local_60[4] = 0x4b;
  local_60[5] = 0;
  local_60[6] = 0;
  local_60[7] = 0;
  local_60[8] = 0x7d;
  local_60[9] = 0;
  local_60[10] = 0;
  local_60[0xb] = 0;
  local_7c = 0x75;
  local_60[0xc] = 0x71;
  local_60[0xd] = 0;
  local_60[0xe] = 0;
  local_60[0xf] = 0;
  local_78 = 0x74;
  local_80 = 0x73;
  local_70[0] = 0x36;
  local_38 = (int)(short)(((short)((ulonglong)((longlong)iVar6 * 0x55555556) >> 0x20) -
                          ((short)(iVar6 / 0x24) + sVar4)) + -1);
  local_74 = 0x77;
  iVar5 = 0;
  ppuVar15 = &PTR_DAT_011e9b20;
  local_70[1] = 0x70;
  do {
    puVar16 = (undefined4 *)*ppuVar15;
    uVar8 = lv_btn_create(uVar7);
    *puVar16 = uVar8;
    lv_obj_set_flex_flow(*(undefined4 *)*ppuVar15,1);
    lv_obj_set_flex_align(*(undefined4 *)*ppuVar15,2,2,2);
    lv_obj_add_event_cb(*(undefined4 *)*ppuVar15,FUN_00406ee8,0,0);
    lv_obj_set_size(*(undefined4 *)*ppuVar15,(int)(short)(((uVar11 & 0xffff) * 6) / 7),local_38);
    uVar8 = lv_label_create(*(undefined4 *)*ppuVar15);
    lv_obj_align(uVar8,9,0,0);
    bVar1 = *pbVar12;
    bVar2 = *pbVar13;
    pbVar12 = pbVar12 + 4;
    pbVar13 = pbVar13 + 4;
    puVar16 = (undefined4 *)*ppuVar15;
    ppuVar15 = ppuVar15 + 1;
    lv_obj_set_style_bg_color
              (*puVar16,*(uint *)((int)local_70 + iVar5) & 0xff | (uint)bVar1 << 8 |
                        (uint)bVar2 << 0x10 | 0xff000000,0);
    puVar10 = (undefined2 *)((int)&local_80 + iVar5);
    iVar5 = iVar5 + 4;
    FUN_00437890(uVar8,*puVar10,2);
    lv_obj_set_style_text_align(uVar8,2,0);
  } while (iVar5 != 0x10);
  uVar7 = lv_obj_create(*(undefined4 *)(local_34 + 0x69e4));
  lv_obj_set_size(uVar7,0x2028,0x200c);
  lv_obj_set_style_bg_opa(uVar7,0,0);
  lv_obj_align(uVar7,3,0,10);
  lv_obj_set_style_pad_left(uVar7,0,0);
  lv_obj_set_style_pad_right(uVar7,0,0);
  lv_obj_set_style_pad_top(uVar7,0,0);
  lv_obj_set_style_pad_bottom(uVar7,0,0);
  lv_obj_set_style_border_width(uVar7,0,0);
  lv_obj_set_scrollbar_mode(uVar7,0);
  lv_obj_set_grid_dsc_array(uVar7,&DAT_011e9b18,&DAT_011e9b10);
  lv_group_set_default(local_30);
  return;
}



/* ===== FUNCTION FUN_00407710 @ 00407710 ===== */

undefined4 FUN_00407710(void)

{
  int iVar1;
  ssize_t sVar2;
  int *piVar3;
  int iVar4;
  epoll_data_t *peVar5;
  int local_4d0 [256];
  epoll_event aeStack_d0 [10];
  char *local_30;
  char *local_2c;
  
  local_30 = "nd bgm path";
  local_2c = "nd bgm path";
  while (DAT_01216a0c != 0) {
    iVar1 = epoll_wait(DAT_01216a10,aeStack_d0,10,1000);
    if (iVar1 == -1) {
      piVar3 = __errno_location();
      if (*piVar3 != 4) {
        usleep(100000);
      }
    }
    else {
      peVar5 = &aeStack_d0[0].data;
      if (0 < iVar1) {
        iVar4 = 0;
        do {
          memset(local_4d0,0,0x400);
          sVar2 = read(peVar5->fd,local_4d0,0x400);
          if (0 < sVar2) {
            if (local_4d0[0] == 0x20000a08) {
              puts("hdmi rx connect");
              printf("%s:%d: \n","notifier_hdmi_in_plugin",0xfc);
              DAT_01216a18 = 1;
            }
            else if (local_4d0[0] == 0x20000a09) {
              puts("hdmi rx disconnect");
              printf("%s:%d: \n",local_2c + -0xa20,0x10e);
              DAT_01216a18 = 0;
            }
            else {
              printf(local_30 + -0xe70,"receive_hdmi_rx_event_func",0x171);
            }
          }
          iVar4 = iVar4 + 1;
          peVar5 = (epoll_data_t *)(&peVar5->ptr + 4);
        } while (iVar1 != iVar4);
      }
    }
  }
  printf("%s:%d: \n","receive_hdmi_rx_event_func",0x178);
  return 0;
}



/* ===== FUNCTION FUN_004078c8 @ 004078c8 ===== */

undefined4 FUN_004078c8(void)

{
  byte bVar1;
  short *psVar2;
  int iVar3;
  ssize_t sVar4;
  undefined4 uVar5;
  uint uVar6;
  int iVar7;
  byte *pbVar8;
  short *psVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  uint uVar12;
  byte *pbVar13;
  short local_2d8 [2];
  undefined4 local_2d4 [6];
  short local_2bc [129];
  undefined1 local_1ba;
  undefined4 local_1b8;
  byte local_1b4 [4];
  undefined4 local_1b0 [67];
  byte abStack_a4 [16];
  char acStack_94 [68];
  undefined1 auStack_50 [12];
  undefined4 local_44;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  
  FUN_00428400();
  local_28 = 0;
  local_24 = 0;
  printf("%s:%d: \n","hdmirx_start",0x88);
  FUN_0042bcc8(&local_24,&local_28);
  uVar10 = local_24;
  uVar5 = local_28;
  iVar3 = ioctl(DAT_01216a04,0x20000a03,0xcd);
  if (iVar3 != 0) {
    puts("HDMI_RX_SET_AUDIO_DATA_PATH failed");
  }
  iVar3 = ioctl(DAT_01216a04,0x20000a07,0);
  if ((iVar3 != 0) || (iVar3 = ioctl(DAT_01216a04,0x20000a02,4), iVar3 != 0)) {
    puts("HDMI_RX_SET_VIDEO_STOP_MODE failed");
    return 0xffffffff;
  }
  ioctl(DAT_01216a04,0x20000a06,uVar10);
  ioctl(DAT_01216a04,0x20000a0e,uVar5);
  ioctl(DAT_01216a04,0x20000a11,3);
  if (DAT_01216a00 == 0) {
    local_1b8 = 0;
    local_1ba = 1;
    memset(local_1b4,0,0x40);
    iVar3 = FUN_0042d7d8(local_1b4,0x40,"individual");
    if (-1 < iVar3) {
      printf("%s devpath:%s\n","projector_hdcpkey_load",local_1b4);
      iVar3 = open((char *)local_1b4,0);
      if (-1 < iVar3) {
        memset(local_2d8,0,0x11e);
        read(iVar3,local_2d8,0x11e);
        close(iVar3);
        if (local_2d8[0] == -1) {
          iVar3 = FUN_0042b2e4();
          if (iVar3 == 0) {
            uVar5 = FUN_0042b2f0();
            snprintf(acStack_94,0x40,"%s/hdmirxkey.bin",uVar5);
            iVar3 = open(acStack_94,2);
            if (iVar3 < 0) {
              printf("open hdmi key file: %s fail!\n",acStack_94);
            }
            else {
              sVar4 = read(iVar3,local_1b4,0x11e);
              if (sVar4 == 0x11e) {
                close(iVar3);
                printf("******%s*******\n","read udisk");
                pbVar13 = local_1b4;
                uVar12 = 0;
                do {
                  uVar6 = uVar12 & 0xf;
                  uVar12 = uVar12 + 1;
                  if (uVar6 == 0) {
                    putchar(10);
                    bVar1 = *pbVar13;
                  }
                  else {
                    bVar1 = *pbVar13;
                  }
                  pbVar13 = pbVar13 + 1;
                  printf("%02x ",(uint)bVar1);
                } while (uVar12 != 0x11e);
                puts("\n");
                memset(acStack_94,0,0x40);
                iVar3 = FUN_0042d7d8(acStack_94,0x40,"individual");
                if (-1 < iVar3) {
                  iVar3 = open(acStack_94,2);
                  if (iVar3 < 0) {
                    printf("Error: open %s failed \n",acStack_94);
                  }
                  else {
                    ioctl(iVar3,0x40204d01,auStack_50);
                    printf("mtd.erasesize=%d\n",local_44);
                    local_30 = 0;
                    local_2c = local_44;
                    iVar7 = ioctl(iVar3,0x80084d02,&local_30);
                    if (iVar7 < 0) {
                      printf("Error: erase %s failed \n",acStack_94);
                      close(iVar3);
                    }
                    else {
                      sVar4 = write(iVar3,local_1b4,0x11e);
                      if (sVar4 == 0x11e) {
                        puts("Write key to flash/individual success!");
                        fsync(iVar3);
                        close(iVar3);
                        FUN_0042ce18();
                        pbVar13 = local_1b4;
                        psVar2 = local_2d8;
                        do {
                          psVar9 = psVar2;
                          pbVar8 = pbVar13;
                          uVar11 = *(undefined4 *)(pbVar8 + 4);
                          uVar10 = *(undefined4 *)(pbVar8 + 8);
                          uVar5 = *(undefined4 *)(pbVar8 + 0xc);
                          pbVar13 = pbVar8 + 0x10;
                          *(undefined4 *)psVar9 = *(undefined4 *)pbVar8;
                          *(undefined4 *)(psVar9 + 2) = uVar11;
                          *(undefined4 *)(psVar9 + 4) = uVar10;
                          *(undefined4 *)(psVar9 + 6) = uVar5;
                          psVar2 = psVar9 + 8;
                        } while (pbVar13 != (byte *)(local_1b0 + 0x43));
                        uVar10 = *(undefined4 *)(pbVar8 + 0x14);
                        uVar5 = *(undefined4 *)(pbVar8 + 0x18);
                        *(undefined4 *)(psVar9 + 8) = *(undefined4 *)pbVar13;
                        *(undefined4 *)(psVar9 + 10) = uVar10;
                        *(undefined4 *)(psVar9 + 0xc) = uVar5;
                        psVar9[0xe] = *(short *)(pbVar8 + 0x1c);
                        ioctl(DAT_01216a04,0x81240a0d,local_2d8);
                        DAT_01216a00 = 1;
                        goto LAB_004079c4;
                      }
                      printf("Error: write %s failed \n",acStack_94);
                    }
                  }
                }
              }
              else {
                puts("read *.bin fail");
                close(iVar3);
              }
            }
          }
          else {
            puts("No usb disk, can not read hdmi key file!");
          }
          puts("No hdcp rx key");
          DAT_01216a00 = 1;
        }
        else {
          iVar3 = ioctl(DAT_01216a04,0x81240a0d,local_2d8);
          if (iVar3 == 0) {
            puts(" HDMI_RX_SET_HDCP_KEY Success!");
            DAT_01216a00 = 1;
          }
          else {
            puts("Error: HDMI_RX_SET_HDCP_KEY failed");
            DAT_01216a00 = 1;
          }
        }
        goto LAB_004079c4;
      }
      printf("Error:  Open %s failed\n",local_1b4);
    }
    puts("Error: load hdcpkey fail");
  }
LAB_004079c4:
  FUN_0042dc0c(1,5);
  FUN_004480f0();
  FUN_0042d72c();
  iVar3 = ioctl(DAT_01216a04,0x20000a00);
  if (iVar3 == 0) {
    DAT_01216a1a = 1;
    DAT_01216a19 = 6;
    uVar5 = 0;
  }
  else {
    puts("HDMI_RX_START failed");
    uVar5 = 0xffffffff;
  }
  return uVar5;
}



/* ===== FUNCTION FUN_00407e90 @ 00407e90 ===== */

int FUN_00407e90(void)

{
  return (int)DAT_01216a18;
}



/* ===== FUNCTION FUN_00407ea8 @ 00407ea8 ===== */

undefined4 FUN_00407ea8(void)

{
  undefined4 uVar1;
  
  printf("%s:%d: \n","hdmirx_pause",0x1f1);
  if ((DAT_01216a04 < 1) || (DAT_01216a19 == '\x05')) {
    printf("%s-ed %d \n","hdmirx_pause",(int)DAT_01216a19);
    uVar1 = 0xffffffff;
  }
  else {
    ioctl(DAT_01216a04,0x20000a12);
    uVar1 = 0;
    DAT_01216a19 = '\x05';
  }
  return uVar1;
}



/* ===== FUNCTION FUN_00407f40 @ 00407f40 ===== */

undefined4 FUN_00407f40(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 local_20;
  undefined4 local_1c;
  
  printf("%s:%d: \n","hdmirx_resume",0x202);
  FUN_0042dc0c(1,5);
  FUN_004480f0();
  local_20 = 0;
  local_1c = 0;
  FUN_0042bcc8(&local_1c,&local_20);
  uVar1 = local_1c;
  uVar3 = local_20;
  if (DAT_01216a04 < 1) {
    puts("hrx closed");
    uVar3 = 0xffffffff;
  }
  else {
    ioctl(DAT_01216a04,0x20000a13);
    DAT_01216a19 = 6;
    iVar2 = ioctl(DAT_01216a04,0x20000a02,4);
    if (iVar2 == 0) {
      ioctl(DAT_01216a04,0x20000a06,uVar1);
      ioctl(DAT_01216a04,0x20000a0e,uVar3);
      uVar3 = 0;
    }
    else {
      puts("HDMI_RX_SET_VIDEO_STOP_MODE failed");
      uVar3 = 0xffffffff;
    }
  }
  return uVar3;
}



/* ===== FUNCTION FUN_00408054 @ 00408054 ===== */

undefined4 FUN_00408054(void)

{
  printf("%s:%d: \n","hdmi_rx_leave",0x223);
  if (0 < DAT_01216a04) {
    ioctl(DAT_01216a04,0x20000a01);
    close(DAT_01216a04);
    DAT_01216a04 = -1;
    DAT_01216a1a = 0;
    DAT_01216a19 = 3;
    if (0 < DAT_011e9b30) {
      close(DAT_011e9b30);
    }
    DAT_011e9b30 = -1;
    close(DAT_01216a04);
    DAT_01216a04 = -1;
    DAT_01216a19 = 3;
  }
  return 0;
}



/* ===== FUNCTION FUN_00408114 @ 00408114 ===== */

undefined4 FUN_00408114(void)

{
  int iVar1;
  pthread_attr_t pStack_58;
  epoll_event local_30;
  undefined4 local_20;
  undefined4 local_1c;
  
  printf("%s:%d: \n","hdmi_rx_enter",0x236);
  iVar1 = FUN_00428454(0x13);
  if (iVar1 == 1) {
    FUN_00434580(0xffffffff,1);
    FUN_00434844(0xffffffff,1);
  }
  else {
    FUN_00434580(0xffffffff,1);
    FUN_00434844(0xffffffff,0);
  }
  if (0 < DAT_01216a04) {
    if (DAT_01216a19 != '\x05') {
      return 0;
    }
    FUN_00407f40();
    return 0;
  }
  DAT_01216a04 = open("/dev/hdmi_rx",2);
  if (DAT_01216a04 < 0) {
    printf("open /dev/hdmi_rx failed, ret=%d\n",DAT_01216a04);
    return 0xffffffff;
  }
  local_30.events = 0;
  local_30._4_4_ = 0;
  local_30.data.ptr = (void *)0x0;
  DAT_01216a0c = 1;
  DAT_01216a70 = 1;
  DAT_01216a78 = 0x10;
  local_30.data.u64._4_4_ = 0;
  local_20 = 0;
  local_1c = 0;
  pthread_attr_init(&pStack_58);
  pthread_attr_setstacksize(&pStack_58,0x2000);
  iVar1 = pthread_create(&DAT_01216a08,&pStack_58,FUN_00407710,(void *)0x0);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_58);
    DAT_01216a10 = epoll_create1(0);
    if (DAT_01216a10 < 0) {
      printf("%s:%d:err: epoll_create1 failed\n","hdmi_hotplug_rx_enable",0x194);
    }
    else {
      DAT_01216a14 = (void *)ioctl(DAT_01216a04,0x20000c00,0x80000);
      if ((int)DAT_01216a14 < 0) {
        printf("%s:%d:err: KUMSGQ_FD_ACCESS failed\n","hdmi_hotplug_rx_enable",0x19a);
      }
      else {
        local_30.events = 1;
        local_30.data.fd = (int)DAT_01216a14;
        iVar1 = epoll_ctl(DAT_01216a10,1,(int)DAT_01216a14,&local_30);
        if (iVar1 == 0) {
          local_1c = 0;
          local_20 = 0x20000a08;
          iVar1 = ioctl(DAT_01216a04,0x80080c01,&local_20);
          if (iVar1 == 0) {
            local_1c = 0;
            local_20 = 0x20000a09;
            iVar1 = ioctl(DAT_01216a04,0x80080c01,&local_20);
            if (iVar1 == 0) goto LAB_004082f0;
            puts("KUMSGQ_NOTIFIER_SETUP HDMI_RX_NOTIFY_DISCONNECT fail");
          }
          else {
            puts("KUMSGQ_NOTIFIER_SETUP HDMI_RX_NOTIFY_CONNECT fail");
          }
        }
        else {
          printf("%s:%d:err: EPOLL_CTL_ADD failed\n","hdmi_hotplug_rx_enable",0x1a2);
        }
      }
    }
  }
  else {
    puts("pthread_create receive_hdmi_rx_event_func fail");
  }
  DAT_01216a0c = 0;
  iVar1 = pthread_join(DAT_01216a08,(void **)0x0);
  if (iVar1 != 0) {
    puts("thread is not exit...");
  }
  if (-1 < DAT_01216a10) {
    close(DAT_01216a10);
    DAT_01216a10 = -1;
  }
  if (-1 < (int)DAT_01216a14) {
    close((int)DAT_01216a14);
    DAT_01216a14 = (void *)0xffffffff;
  }
LAB_004082f0:
  FUN_004078c8();
  printf("%s:%d:  really\n","hdmi_rx_enter",600);
  return 0;
}



/* ===== FUNCTION FUN_00408520 @ 00408520 ===== */

void FUN_00408520(int param_1)

{
  int iVar1;
  
  lv_msgbox_close(*(undefined4 *)(param_1 + 0xc));
  iVar1 = lv_obj_has_flag(DAT_01216ac8,1);
  if (iVar1 != 0) {
    lv_obj_clear_flag(DAT_01216ac8,1);
  }
  DAT_01216ad4 = 0;
  return;
}



/* ===== FUNCTION FUN_00408570 @ 00408570 ===== */

void FUN_00408570(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  short sVar6;
  
  iVar1 = FUN_00407e90();
  if ((iVar1 == 2) && (iVar1 = lv_obj_has_flag(DAT_01216ac8,1), iVar1 != 0)) {
    if (DAT_01216ad4 == 0) {
      uVar3 = lv_disp_get_default();
      uVar3 = lv_disp_get_layer_top(uVar3);
      uVar4 = FUN_004386cc(0xf3);
      uVar3 = lv_msgbox_create(uVar3,0,uVar4,0,0);
      uVar4 = FUN_0043783c(1);
      lv_obj_set_style_text_font(uVar3,uVar4,0);
      DAT_01216ad4 = lv_timer_create(FUN_00408520,3000,uVar3);
      lv_timer_set_repeat_count(DAT_01216ad4,1);
      lv_timer_reset(DAT_01216ad4);
      return;
    }
  }
  else {
    iVar1 = FUN_00407e90();
    if (iVar1 != 1) {
      iVar1 = lv_obj_has_flag(DAT_01216ac8,1);
      if (iVar1 != 0) {
        lv_obj_clear_flag(DAT_01216ac8,1);
      }
      iVar1 = lv_obj_get_x(DAT_01216ac0);
      iVar2 = lv_obj_get_y(DAT_01216ac0);
      if (DAT_011e9b41 == '\0') {
        iVar5 = (short)((short)((ulonglong)((longlong)(int)DAT_01216ad2 * 0x2aaaaaab) >> 0x20) -
                       (DAT_01216ad2 >> 0xf)) * 5;
        sVar6 = (short)iVar5;
        if (iVar2 + 9 < iVar5) {
          sVar6 = (short)iVar2 + 10;
        }
        iVar2 = (int)sVar6;
        if (iVar5 == iVar2) {
          DAT_011e9b41 = '\x01';
        }
      }
      else if (iVar2 < 0xb) {
        iVar2 = 0;
        DAT_011e9b41 = '\0';
      }
      else {
        iVar2 = (int)(short)((short)iVar2 + -10);
      }
      if (DAT_011e9b40 == '\0') {
        iVar5 = (int)(DAT_01216ad0 / 5) << 2;
        sVar6 = (short)iVar1 + 0x14;
        if (iVar5 <= iVar1 + 0x13) {
          sVar6 = (short)iVar5;
        }
        iVar1 = (int)sVar6;
        if (iVar5 == iVar1) {
          DAT_011e9b40 = '\x01';
        }
      }
      else {
        if (iVar1 < 0x15) {
          DAT_011e9b40 = 0;
          lv_obj_set_pos(DAT_01216ac0,0,iVar2);
          return;
        }
        iVar1 = (int)(short)((short)iVar1 + -0x14);
      }
      lv_obj_set_pos(DAT_01216ac0,iVar1,iVar2);
      return;
    }
    iVar1 = lv_obj_has_flag(DAT_01216ac8,1);
    if (iVar1 == 0) {
      lv_obj_add_flag(DAT_01216ac8,1);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00408808 @ 00408808 ===== */

void FUN_00408808(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_18 [2];
  
  DAT_01216ad8 = 0;
  lv_obj_clean(DAT_01216ac8);
  DAT_01216ac0 = lv_obj_create(DAT_01216ac8);
  lv_obj_set_size(DAT_01216ac0,0x200f,0x200c);
  uVar1 = DAT_01216ac0;
  lv_palette_main(local_18,0x12);
  lv_obj_set_style_bg_color(uVar1,local_18[0],0);
  lv_obj_set_style_radius(DAT_01216ac0,5,0);
  DAT_01216acc = lv_timer_create(FUN_00408570,1000,0);
  lv_timer_set_repeat_count(DAT_01216acc,0xffffffff);
  lv_timer_ready(DAT_01216acc);
  uVar1 = lv_label_create(DAT_01216ac0);
  lv_obj_align(uVar1,9,0,0);
  uVar2 = FUN_004386cc(0xf4);
  lv_label_set_text(uVar1,uVar2);
  uVar2 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar1,uVar2,0);
  iVar3 = FUN_00407e90();
  if (iVar3 != 1) {
    return;
  }
  lv_obj_add_flag(DAT_01216ac8,1);
  return;
}



/* ===== FUNCTION FUN_00408940 @ 00408940 ===== */

void FUN_00408940(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 local_10 [2];
  
  iVar2 = lv_event_get_code();
  if (iVar2 == 0x26) {
    FUN_0040677c(DAT_01216ac4);
    uVar3 = lv_obj_create(DAT_01216ac8);
    lv_palette_lighten(local_10,5,1);
    lv_obj_set_style_bg_color(uVar3,local_10[0],0);
    lv_obj_set_size(uVar3,0x2064,0x2008);
    lv_obj_align(uVar3,5,0,0);
    uVar3 = lv_label_create(uVar3);
    lv_obj_align(uVar3,7,0,0);
    uVar4 = FUN_004386cc(0x73);
    lv_label_set_text(uVar3,uVar4);
    uVar4 = FUN_0043783c(1);
    lv_obj_set_style_text_font(uVar3,uVar4,0);
    FUN_00408114();
    DAT_01216ad8 = lv_timer_create(FUN_00408808,3000,0);
    lv_timer_set_repeat_count(DAT_01216ad8,1);
    lv_timer_reset(DAT_01216ad8);
    return;
  }
  if (iVar2 == 0x25) {
    if (DAT_01216acc != 0) {
      lv_timer_del();
      DAT_01216acc = 0;
    }
    if (DAT_01216ad8 != 0) {
      lv_timer_del();
      DAT_01216ad8 = 0;
    }
    if (DAT_01216ad4 != 0) {
      lv_timer_ready();
    }
    lv_obj_clean(DAT_01216ac8);
    iVar2 = lv_obj_has_flag(DAT_01216ac8,1);
    if (iVar2 != 0) {
      lv_obj_clear_flag(DAT_01216ac8,1);
      return;
    }
  }
  else if (iVar2 == 0xd) {
    uVar3 = lv_indev_get_act();
    cVar1 = lv_indev_get_key(uVar3);
    if (cVar1 == '\x1b') {
      FUN_004067ac(3);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00408b1c @ 00408b1c ===== */

void FUN_00408b1c(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  DAT_01216ac8 = lv_obj_create(0);
  DAT_01216ac4 = lv_group_create();
  uVar1 = lv_group_get_default();
  lv_group_set_default(DAT_01216ac4);
  lv_group_add_obj(DAT_01216ac4,DAT_01216ac8);
  lv_obj_set_style_bg_opa(DAT_01216ac8,0,0);
  lv_obj_add_event_cb(DAT_01216ac8,FUN_00408940,0,0);
  uVar2 = lv_disp_get_default();
  DAT_01216ad2 = lv_disp_get_ver_res(uVar2);
  uVar2 = lv_disp_get_default();
  DAT_01216ad0 = lv_disp_get_hor_res(uVar2);
  lv_group_set_default(uVar1);
  return;
}



/* ===== FUNCTION FUN_00408c1c @ 00408c1c ===== */

void FUN_00408c1c(undefined4 param_1)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  
  iVar2 = lv_event_get_code();
  uVar3 = lv_event_get_target(param_1);
  if (iVar2 == 0x1a) {
    iVar2 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar2 + 0x10) + 0x38) = 0;
  }
  else {
    if (iVar2 == 0xd) {
      lv_timer_pause(DAT_01216af8);
      uVar4 = lv_indev_get_act();
      uVar5 = lv_indev_get_key(uVar4);
      uVar5 = uVar5 & 0xffff;
      bVar1 = lv_btnmatrix_get_selected_btn(uVar3);
      uVar6 = (uint)bVar1;
      if (uVar5 - 0x11 < 2) {
        if ((DAT_01216b00 == uVar6) && (DAT_01216b00 == 0)) {
          lv_btnmatrix_set_selected_btn(uVar3,1);
          bVar1 = 1;
        }
        else {
          DAT_01216b00 = bVar1;
          if (1 < uVar6) {
            DAT_01216b00 = 0;
            lv_btnmatrix_set_selected_btn(uVar3,0);
            bVar1 = 0;
          }
        }
        lv_btnmatrix_set_btn_ctrl(uVar3,bVar1,0x80);
      }
      else {
        if (uVar5 == 10) {
          FUN_004286f8(0x13,*(undefined4 *)(&UNK_0044f6e0 + uVar6 * 4));
          (*(code *)(&PTR_LAB_0044f6d4)[uVar6 * 2])();
          if (DAT_01216af8 == 0) {
            return;
          }
          lv_timer_del();
          DAT_01216af8 = 0;
          return;
        }
        if (uVar5 == 0x1b) {
          lv_timer_ready(DAT_01216af8);
          lv_timer_resume(DAT_01216af8);
          return;
        }
        if (uVar5 - 0x13 < 2) {
          if ((DAT_01216b00 == 0) && (uVar5 == 0x14)) {
            lv_btnmatrix_set_selected_btn(uVar3,1);
          }
          else {
            lv_btnmatrix_set_selected_btn(uVar3);
          }
        }
      }
      lv_timer_resume(DAT_01216af8);
      lv_timer_reset(DAT_01216af8);
      return;
    }
    if (iVar2 == 0xe) {
      DAT_01216b00 = 0xff;
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00408fc8 @ 00408fc8 ===== */

void FUN_00408fc8(void)

{
  undefined2 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 local_20 [3];
  
  DAT_01216ae4 = lv_obj_create(0);
  lv_obj_clear_flag(DAT_01216ae4,0x10);
  lv_obj_set_style_bg_opa(DAT_01216ae4,0,0);
  uVar4 = DAT_01216ae4;
  lv_palette_main(local_20,0x11);
  lv_obj_set_style_bg_color(uVar4,local_20[0],0);
  lv_obj_add_event_cb(DAT_01216ae4,&LAB_00408e58,0x27,0);
  lv_obj_add_event_cb(DAT_01216ae4,&LAB_00408e58,0x28,0);
  DAT_01216ae0 = lv_group_create();
  uVar2 = lv_group_get_default();
  lv_group_set_default(DAT_01216ae0);
  uVar3 = lv_obj_create(DAT_01216ae4);
  lv_obj_set_size(uVar3,0x2019,0x2050);
  lv_obj_align(uVar3,1,0x204b,0x2003);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_pad_row(uVar3,0,0);
  lv_obj_set_style_pad_column(uVar3,0,0);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_flex_flow(uVar3,1);
  lv_obj_set_scrollbar_mode(uVar3,0);
  uVar4 = lv_obj_create(uVar3);
  lv_obj_set_size(uVar4,0x2064,0x200c);
  lv_obj_set_style_radius(uVar4,0,0);
  lv_obj_set_style_border_width(uVar4,0,0);
  lv_obj_set_style_outline_width(uVar4,0,0);
  lv_palette_darken(local_20,0x12,3);
  lv_obj_set_style_bg_color(uVar4,local_20[0],0);
  lv_obj_set_scrollbar_mode(uVar4,0);
  uVar4 = lv_label_create(uVar4);
  lv_obj_align(uVar4,9,0,0);
  lv_obj_set_style_text_color(uVar4,0xffffffff,0);
  FUN_00437890(uVar4,0x98,0);
  DAT_01216afc = lv_btnmatrix_create(uVar3);
  lv_style_init(&DAT_01216af0);
  lv_style_set_pad_left(&DAT_01216af0,0);
  lv_style_set_pad_right(&DAT_01216af0,0);
  lv_style_set_pad_top(&DAT_01216af0,0);
  lv_style_set_pad_bottom(&DAT_01216af0,0);
  lv_style_set_pad_row(&DAT_01216af0,0);
  lv_style_set_pad_column(&DAT_01216af0,0);
  lv_style_set_border_width(&DAT_01216af0,0);
  lv_style_set_outline_width(&DAT_01216af0,0);
  lv_obj_add_style(DAT_01216afc,&DAT_01216af0,0);
  lv_style_init(&DAT_01216ae8);
  lv_style_set_radius(&DAT_01216ae8,0);
  lv_style_set_border_width(&DAT_01216ae8,1);
  lv_style_set_border_opa(&DAT_01216ae8,0x7f);
  lv_style_set_border_color(&DAT_01216ae8,0xff8c8cc6);
  lv_style_set_bg_color(&DAT_01216ae8,0xff6565b1);
  lv_style_set_border_side(&DAT_01216ae8,0x10);
  lv_obj_add_style(DAT_01216afc,&DAT_01216ae8,0x50000);
  lv_obj_set_style_text_align(DAT_01216afc,2,0x50000);
  lv_obj_set_size(DAT_01216afc,0x2064,0x204d);
  uVar4 = DAT_01216afc;
  lv_palette_darken(local_20,0x12,1);
  lv_obj_set_style_bg_color(uVar4,local_20[0],0x50000);
  uVar4 = DAT_01216afc;
  lv_palette_darken(local_20,0xc,1);
  lv_obj_set_style_bg_color(uVar4,local_20[0],0);
  lv_btnmatrix_set_btn_ctrl_all(DAT_01216afc,0x40);
  lv_btnmatrix_set_one_checked(DAT_01216afc,1);
  lv_obj_set_style_radius(DAT_01216afc,0,0x50000);
  FUN_00447298(DAT_01216afc,0x10,2,&DAT_011e9b50);
  lv_group_focus_obj(DAT_01216afc);
  uVar4 = DAT_01216afc;
  uVar1 = FUN_00428454(0x13);
  lv_btnmatrix_set_btn_ctrl(uVar4,uVar1,0x80);
  uVar4 = DAT_01216afc;
  uVar1 = FUN_00428454(0x13);
  lv_btnmatrix_set_selected_btn(uVar4,uVar1);
  DAT_01216b00 = 0xff;
  lv_obj_add_event_cb(DAT_01216afc,FUN_00408c1c,0,DAT_01216af8);
  uVar4 = lv_obj_create(uVar3);
  lv_obj_set_size(uVar4,0x2064,0x200c);
  lv_palette_darken(local_20,0x12,3);
  lv_obj_set_style_bg_color(uVar4,local_20[0],0);
  lv_obj_set_style_border_width(uVar4,0,0);
  lv_obj_set_style_outline_width(uVar4,0,0);
  lv_obj_set_style_radius(uVar4,0,0);
  lv_obj_set_style_shadow_width(uVar4,0,0);
  lv_obj_set_scrollbar_mode(uVar4,0);
  uVar3 = lv_img_create(uVar4);
  lv_obj_align(uVar3,7,0x2011,0);
  lv_img_set_src(uVar3,&DAT_00c6b980);
  uVar4 = lv_label_create(uVar4);
  lv_obj_align_to(uVar4,uVar3,0x13,5,0xfffffffb);
  lv_obj_set_style_text_color(uVar4,0xffffffff,0);
  FUN_00437890(uVar4,0x4b,0);
  lv_group_set_default(uVar2);
  return;
}



/* ===== FUNCTION FUN_00409660 @ 00409660 ===== */

void FUN_00409660(void)

{
  lv_scr_load_anim();
  return;
}



/* ===== FUNCTION FUN_00409680 @ 00409680 ===== */

void FUN_00409680(void)

{
  lv_scr_load_anim();
  return;
}



/* ===== FUNCTION FUN_0040980c @ 0040980c ===== */

void FUN_0040980c(int param_1)

{
  lv_obj_get_y_aligned(*(undefined4 *)(param_1 + 0x18));
  return;
}



/* ===== FUNCTION FUN_00409828 @ 00409828 ===== */

void FUN_00409828(int param_1)

{
  lv_obj_get_width(*(undefined4 *)(param_1 + 0x18));
  return;
}



/* ===== FUNCTION FUN_00409844 @ 00409844 ===== */

void FUN_00409844(int param_1)

{
  lv_obj_get_height(*(undefined4 *)(param_1 + 0x18));
  return;
}



/* ===== FUNCTION FUN_00409860 @ 00409860 ===== */

undefined1 FUN_00409860(int param_1)

{
  undefined1 local_10 [12];
  
  lv_obj_get_style_prop(local_10,*(undefined4 *)(param_1 + 0x18),0,0x60);
  return local_10[0];
}



/* ===== FUNCTION FUN_0040988c @ 0040988c ===== */

void FUN_0040988c(int param_1)

{
  lv_img_get_zoom(*(undefined4 *)(param_1 + 0x18));
  return;
}



/* ===== FUNCTION FUN_004098a8 @ 004098a8 ===== */

void FUN_004098a8(int param_1)

{
  lv_img_get_angle(*(undefined4 *)(param_1 + 0x18));
  return;
}



/* ===== FUNCTION FUN_004098c4 @ 004098c4 ===== */

void FUN_004098c4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 auStack_30 [32];
  
  uVar1 = lv_arc_get_value(param_2);
  lv_snprintf(auStack_30,0x20,"%s%d%s",param_3,uVar1,param_4);
  lv_label_set_text(param_1,auStack_30);
  return;
}



/* ===== FUNCTION FUN_00409930 @ 00409930 ===== */

void FUN_00409930(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined1 auStack_30 [32];
  
  uVar1 = lv_bar_get_value(param_2);
  lv_snprintf(auStack_30,0x20,"%s%d%s",param_3,uVar1,param_4);
  lv_label_set_text(param_1,auStack_30);
  return;
}



/* ===== FUNCTION FUN_0040999c @ 0040999c ===== */

void FUN_0040999c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = lv_obj_has_state(param_2,1);
  if (iVar1 == 0) {
    lv_label_set_text(param_1,param_4);
    return;
  }
  lv_label_set_text(param_1,param_3);
  return;
}



/* ===== FUNCTION FUN_00409a10 @ 00409a10 ===== */

undefined4 FUN_00409a10(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_410 [1028];
  
  uVar1 = FUN_0041d32c();
  FUN_00425c80(uVar1);
  memset(auStack_410,0,0x400);
  iVar2 = FUN_0040fddc(&DAT_01216b20,DAT_01216f28);
  FUN_0040fda8(auStack_410,&DAT_01216b20,iVar2 + 10);
  FUN_00424408(uVar1,auStack_410);
  return 0;
}



/* ===== FUNCTION FUN_00409a88 @ 00409a88 ===== */

undefined4 FUN_00409a88(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  puVar2 = &DAT_01216f30;
  do {
    lv_label_set_text(*puVar2," ");
    lv_obj_clear_state(*puVar2,1);
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    lv_label_set_long_mode(uVar1,4);
  } while (puVar2 != (undefined4 *)0x1216f60);
  return 0;
}



/* ===== FUNCTION FUN_00409afc @ 00409afc ===== */

void FUN_00409afc(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = glist_nth(*(undefined4 *)(param_1 + 0x40c));
  uVar1 = glist_delete_link(*(undefined4 *)(param_1 + 0x40c),uVar1);
  *(undefined4 *)(param_1 + 0x40c) = uVar1;
  return;
}



/* ===== FUNCTION FUN_00409b34 @ 00409b34 ===== */

undefined4 FUN_00409b34(void)

{
  int *piVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  undefined4 *puVar6;
  
  puVar6 = &DAT_01216f30;
  uVar4 = (uint)DAT_01216b14;
  iVar5 = DAT_01216f28 - uVar4;
  puVar3 = puVar6;
  do {
    lv_label_set_text(*puVar3," ");
    lv_obj_clear_state(*puVar3,1);
    uVar2 = *puVar3;
    puVar3 = puVar3 + 1;
    lv_label_set_long_mode(uVar2,4);
  } while (puVar3 != (undefined4 *)0x1216f60);
  do {
    while( true ) {
      piVar1 = (int *)FUN_0040fddc(&DAT_01216b20,uVar4);
      uVar4 = uVar4 + 1;
      if (piVar1 == (int *)0x0) goto LAB_00409c18;
      if (*piVar1 == 0) break;
      uVar2 = *puVar6;
      puVar6 = puVar6 + 1;
      lv_label_set_text_fmt(uVar2,"%s",(int)piVar1 + 10);
      if (puVar6 == (undefined4 *)0x1216f60) goto LAB_00409c18;
    }
    puVar6 = puVar6 + 1;
  } while (puVar6 != (undefined4 *)0x1216f60);
LAB_00409c18:
  lv_obj_add_state((&DAT_01216f30)[iVar5],1);
  lv_label_set_long_mode((&DAT_01216f30)[iVar5],3);
  return 0;
}



/* ===== FUNCTION FUN_00409c68 @ 00409c68 ===== */

undefined2 FUN_00409c68(ushort *param_1)

{
  ushort uVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  
  iVar2 = FUN_0043645c();
  if (iVar2 == 0) {
LAB_00409cac:
    uVar1 = param_1[1];
  }
  else {
    uVar4 = (uint)*param_1;
    puVar7 = &DAT_01216f30;
    if (param_1[1] == 0) goto LAB_00409cf0;
    uVar6 = param_1[1] + uVar4;
    do {
      uVar5 = uVar4;
      if ((int)uVar4 < (int)(uint)param_1[5]) {
        do {
          uVar4 = uVar5 + 1;
          iVar2 = FUN_0040fddc(&DAT_01216b20,uVar5);
          uVar3 = *puVar7;
          puVar7 = puVar7 + 1;
          lv_label_set_text_fmt(uVar3,"%s",iVar2 + 10);
          if (uVar4 == uVar6) goto LAB_00409cac;
          uVar5 = uVar4;
        } while ((int)uVar4 < (int)(uint)param_1[5]);
      }
      uVar3 = *puVar7;
      uVar4 = uVar4 + 1;
      puVar7 = puVar7 + 1;
      lv_label_set_text(uVar3,&DAT_0044f178);
    } while (uVar4 != uVar6);
    uVar1 = param_1[1];
  }
  puVar7 = &DAT_01216f30;
  iVar2 = 0;
  if (uVar1 != 0) {
    do {
      iVar2 = iVar2 + 1;
      lv_obj_clear_state(*puVar7,1);
      uVar3 = *puVar7;
      puVar7 = puVar7 + 1;
      lv_label_set_long_mode(uVar3,4);
    } while (iVar2 < (int)(uint)param_1[1]);
  }
  uVar4 = (uint)*param_1;
LAB_00409cf0:
  lv_obj_add_state((&DAT_01216f30)[-uVar4],1);
  lv_label_set_long_mode((&DAT_01216f30)[-uVar4],3);
  return 0;
}



/* ===== FUNCTION FUN_00409de0 @ 00409de0 ===== */

undefined4 FUN_00409de0(undefined4 param_1)

{
  void *__ptr;
  void *__ptr_00;
  int *piVar1;
  
  __ptr = calloc(1,0x401);
  __ptr_00 = malloc(0x400);
  if (__ptr == (void *)0x0) {
    puts("Not enough memory.");
    piVar1 = (int *)FUN_0040fddc(&DAT_01216b20,param_1);
    if (piVar1 == (int *)0x0) {
      puts("file_mgr_get_file_node() fail!");
      if (__ptr_00 == (void *)0x0) {
        return 0xffffffff;
      }
      free(__ptr_00);
      return 0xffffffff;
    }
  }
  else {
    piVar1 = (int *)FUN_0040fddc(&DAT_01216b20,param_1);
    if (piVar1 == (int *)0x0) {
      puts("file_mgr_get_file_node() fail!");
      free(__ptr);
      return 0xffffffff;
    }
  }
  FUN_0040fda8(__ptr_00,&DAT_01216b20,(int)piVar1 + 10);
  if (*piVar1 == 0) {
    FUN_0040ff40(&DAT_01216b20,__ptr_00);
    DAT_01216f28 = 0;
    FUN_00435d2c(&DAT_01216b14,0xc,DAT_01216f24 + DAT_01216f26,0);
    FUN_00409b34();
  }
  else {
    lv_label_set_text(DAT_012171fc,(int)piVar1 + 10);
    DAT_012167b4 = lv_label_get_text(DAT_012171fc);
    if (DAT_01216f20 == 3) {
      FUN_0040cab0(__ptr_00);
    }
    else {
      FUN_00409a10();
    }
  }
  if (__ptr_00 != (void *)0x0) {
    free(__ptr_00);
  }
  if (__ptr == (void *)0x0) {
    return 0;
  }
  free(__ptr);
  return 0;
}



/* ===== FUNCTION FUN_0040a200 @ 0040a200 ===== */

int FUN_0040a200(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  iVar1 = FUN_0040fddc(param_1,*(undefined2 *)(param_1 + 0x408));
  if (iVar1 != 0) {
    piVar3 = *(int **)(param_2 + 0x40c);
    if (piVar3 != (int *)0x0) {
      iVar2 = *piVar3;
      iVar4 = 0;
      while( true ) {
        if (iVar2 == 0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
        iVar2 = strcmp((char *)(iVar1 + 10),(char *)(iVar2 + 10));
        if ((iVar2 == 0) || (piVar3 = (int *)piVar3[1], piVar3 == (int *)0x0)) break;
        iVar4 = iVar4 + 1;
        iVar2 = *piVar3;
      }
      *(short *)(param_2 + 0x408) = (short)iVar4;
      return iVar4;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_0040a2c0 @ 0040a2c0 ===== */

int FUN_0040a2c0(char *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  iVar2 = 0;
  piVar3 = *(int **)(param_2 + 0x40c);
  iVar1 = *piVar3;
  while( true ) {
    if (iVar1 == 0) {
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    printf("each node.name:%s\n",(char *)(iVar1 + 10));
    iVar1 = strcmp(param_1,(char *)(iVar1 + 10));
    if (iVar1 == 0) break;
    piVar3 = (int *)piVar3[1];
    iVar2 = iVar2 + 1;
    iVar1 = *piVar3;
  }
  *(short *)(param_2 + 0x408) = (short)iVar2;
  return iVar2;
}



/* ===== FUNCTION FUN_0040a364 @ 0040a364 ===== */

undefined4 FUN_0040a364(void)

{
  undefined2 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  size_t sVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  int iVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 *puVar12;
  short sVar13;
  int *piVar14;
  char *__s;
  
  puVar12 = &DAT_01216b20;
  if (DAT_01216f2c != (int *)0x0) {
    return 0;
  }
  puVar2 = (undefined4 *)FUN_0041d338();
  memset(&DAT_01216b20,0,0x410);
  DAT_01216f20 = puVar2[0x100];
  puVar3 = puVar2;
  do {
    uVar11 = *puVar3;
    uVar10 = puVar3[1];
    uVar8 = puVar3[2];
    uVar7 = puVar3[3];
    puVar3 = puVar3 + 4;
    *puVar12 = uVar11;
    puVar12[1] = uVar10;
    puVar12[2] = uVar8;
    puVar12[3] = uVar7;
    puVar12 = puVar12 + 4;
  } while (puVar3 != puVar2 + 0x100);
  piVar14 = (int *)puVar2[0x103];
  if (piVar14 == (int *)0x0) {
    uVar1 = *(undefined2 *)(puVar2 + 0x102);
  }
  else {
    do {
      iVar6 = *(int *)*piVar14;
      __s = (char *)(*piVar14 + 10);
      if (iVar6 != 0) {
        sVar4 = strlen(__s);
        piVar5 = malloc(sVar4 + 0xd);
        *piVar5 = iVar6;
        DAT_01216f26 = DAT_01216f26 + 1;
        strcpy((char *)((int)piVar5 + 10),__s);
        DAT_01216f2c = (int *)glist_append(DAT_01216f2c,piVar5);
      }
      piVar14 = (int *)piVar14[1];
    } while (piVar14 != (int *)0x0);
    uVar1 = *(undefined2 *)(puVar2 + 0x102);
  }
  iVar6 = FUN_0040fddc(puVar2,uVar1);
  sVar13 = DAT_01216f28;
  if ((iVar6 != 0) && (DAT_01216f2c != (int *)0x0)) {
    iVar9 = *DAT_01216f2c;
    piVar14 = DAT_01216f2c;
    sVar13 = 0;
    while( true ) {
      if (iVar9 == 0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar9 = strcmp((char *)(iVar6 + 10),(char *)(iVar9 + 10));
      if (iVar9 == 0) break;
      piVar14 = (int *)piVar14[1];
      if (piVar14 == (int *)0x0) break;
      iVar9 = *piVar14;
      sVar13 = sVar13 + 1;
    }
  }
  DAT_01216f28 = sVar13;
  FUN_00435d2c(&DAT_01216b14,0xc,DAT_01216f26 + DAT_01216f24,DAT_01216f28);
  return 0;
}



/* ===== FUNCTION FUN_0040a544 @ 0040a544 ===== */

undefined4 FUN_0040a544(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  short sVar5;
  
  iVar1 = FUN_0041d338();
  iVar2 = FUN_0040fddc(&DAT_01216b20,DAT_01216f28);
  if (iVar2 != 0) {
    piVar4 = *(int **)(iVar1 + 0x40c);
    if (piVar4 != (int *)0x0) {
      iVar3 = *piVar4;
      sVar5 = 0;
      while( true ) {
        if (iVar3 == 0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
        iVar3 = strcmp((char *)(iVar2 + 10),(char *)(iVar3 + 10));
        if ((iVar3 == 0) || (piVar4 = (int *)piVar4[1], piVar4 == (int *)0x0)) break;
        sVar5 = sVar5 + 1;
        iVar3 = *piVar4;
      }
      *(short *)(iVar1 + 0x408) = sVar5;
    }
  }
  FUN_00410934(&DAT_01216b20);
  return 0;
}



/* ===== FUNCTION FUN_0040a604 @ 0040a604 ===== */

undefined4 FUN_0040a604(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  
  DAT_01216b10 = FUN_0041b8b8(param_1,0x1c,0x43);
  lv_obj_add_event_cb(DAT_01216b10,&LAB_00409fac,0,param_2);
  puVar4 = &DAT_01216f30;
  lv_group_focus_obj(DAT_01216b10);
  do {
    uVar1 = FUN_00422ba8(DAT_01216b10,100,8);
    *puVar4 = uVar1;
    puVar4 = puVar4 + 1;
  } while (puVar4 != (undefined4 *)0x1216f60);
  FUN_00435d2c(&DAT_01216b14,0xc,DAT_01216f26 + DAT_01216f24,DAT_01216f28);
  FUN_00409b34();
  uVar1 = DAT_01216b10;
  uVar2 = lv_obj_get_parent(DAT_01216b10);
  iVar3 = lv_obj_get_child_cnt(uVar2);
  lv_obj_move_to_index(uVar1,iVar3 + -1);
  return DAT_01216b10;
}



/* ===== FUNCTION FUN_0040a6ec @ 0040a6ec ===== */

undefined4 * FUN_0040a6ec(void)

{
  return &DAT_01216b20;
}



/* ===== FUNCTION FUN_0040a6f8 @ 0040a6f8 ===== */

undefined2 FUN_0040a6f8(void)

{
  return DAT_01216f28;
}



/* ===== FUNCTION FUN_0040a7a8 @ 0040a7a8 ===== */

/* WARNING: Type propagation algorithm not settling */

undefined4 FUN_0040a7a8(char *param_1)

{
  bool bVar1;
  undefined *puVar2;
  char cVar3;
  char *pcVar4;
  size_t sVar5;
  uint *puVar6;
  int *piVar7;
  undefined4 uVar8;
  undefined4 extraout_v0;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  FILE *__stream;
  uint uVar14;
  void *__s;
  uint uVar15;
  uint uStack_448;
  char local_442 [1026];
  uint local_40 [2];
  undefined *local_38;
  undefined *local_34;
  uint local_30;
  undefined *local_2c;
  
  local_34 = &DAT_01210000;
  __s = (void *)((int)&uStack_448 - ((uint)DAT_01216fb4 * 8 + 8));
  memset(__s,0,(uint)DAT_01216fb4 * 8);
  DAT_01216f74 = 0;
  DAT_01216f78 = 0;
  DAT_01216f7c = 0;
  DAT_01216f80 = 0;
  DAT_01216f84 = 0;
  DAT_01216f88 = 0;
  DAT_01216f8c = 0;
  DAT_01216f90 = 0;
  DAT_01216f94 = 0;
  DAT_01216f98 = 0;
  DAT_01216f9c = 0;
  DAT_01216fa0 = 0;
  DAT_01216fa4 = 0;
  DAT_01216fa8 = 0;
  memset(local_442 + 2,0,0x400);
  strcpy(local_442 + 2,param_1);
  local_38 = &DAT_011f0000;
  DAT_011e9b90 = 0;
  uVar11 = 0;
  if ((byte)(DAT_01216fb0._2_1_ - 2U) < 4) {
    local_30 = 0;
    local_2c = &DAT_01210000;
LAB_0040aaec:
    uVar11 = 0;
LAB_0040aaf0:
    __stream = *(FILE **)(local_34 + 0x6fac);
    if (__stream != (FILE *)0x0) {
      do {
        iVar12 = feof(__stream);
        if (iVar12 != 0) break;
        uVar14 = 0;
        pcVar4 = local_442;
        do {
          pcVar4 = pcVar4 + 2;
          iVar12 = feof(__stream);
          if (iVar12 != 0) {
LAB_0040ad6c:
            uVar9 = uVar14 & 0xffff;
            local_442[uVar14 + 2] = '\0';
            if (uVar9 == 0) goto LAB_0040ac6c;
            iVar12 = *(int *)(local_38 + -0x6470);
            goto joined_r0x0040ad8c;
          }
          fread(pcVar4,2,1,__stream);
          if ((pcVar4[-2] == '\r') && (pcVar4[-1] == '\0')) {
            cVar3 = *pcVar4;
            if (cVar3 != '\n') goto LAB_0040ab48;
            if (pcVar4[1] == '\0') goto LAB_0040ad68;
          }
          else {
            cVar3 = *pcVar4;
LAB_0040ab48:
            if ((cVar3 == '\0') && (pcVar4[1] == '\n')) {
LAB_0040ad68:
              uVar14 = uVar14 + 2;
              goto LAB_0040ad6c;
            }
          }
          uVar14 = uVar14 + 2;
        } while (uVar14 != 0x400);
        uVar14 = 0x400;
        local_40[0] = local_40[0] & 0xffffff00;
        iVar12 = *(int *)(local_38 + -0x6470);
        uVar9 = 0x400;
joined_r0x0040ad8c:
        if (iVar12 == -1) {
          if (uVar11 == 0) {
            return 0;
          }
          goto LAB_0040ac74;
        }
        uVar13 = (uint)DAT_01216fb6;
        if (uVar13 < uVar9) {
          if (uVar13 == 0) {
            trap(7);
          }
          iVar12 = 0;
          if (uVar9 / uVar13 != 0) {
            do {
              puVar6 = (uint *)((int)__s + uVar11 * 8);
              uVar14 = uVar13 + DAT_01216f84;
              if (DAT_01216fb4 - 1 == 0) {
                trap(7);
              }
              *puVar6 = uVar11;
              puVar6[1] = uVar14;
              DAT_01216f84 = uVar14;
              if ((uVar11 % (DAT_01216fb4 - 1) == 0) && (uVar11 != 0)) {
                uVar11 = 0;
                piVar7 = malloc(0xc);
                puVar2 = local_2c;
                piVar7[2] = uVar14;
                uVar8 = *(undefined4 *)(local_2c + 0x6f64);
                piVar7[1] = uVar14 - local_30;
                *piVar7 = DAT_01216f90;
                DAT_01216f90 = DAT_01216f90 + 1;
                uVar8 = glist_append(uVar8,piVar7);
                uVar13 = (uint)DAT_01216fb6;
                *(undefined4 *)(puVar2 + 0x6f64) = uVar8;
                local_30 = uVar14;
              }
              else {
                uVar11 = uVar11 + 1;
              }
              if (uVar13 == 0) {
                trap(7);
              }
              iVar12 = iVar12 + 1;
            } while (iVar12 < (int)(uVar9 / uVar13));
          }
          if (uVar13 == 0) {
            trap(7);
          }
          if (uVar9 % uVar13 == 0) goto LAB_0040aaf0;
          puVar6 = (uint *)((int)__s + uVar11 * 8);
          uVar14 = uVar9 % uVar13 + DAT_01216f84;
          if (DAT_01216fb4 - 1 == 0) {
            trap(7);
          }
          *puVar6 = uVar11;
          puVar6[1] = uVar14;
          DAT_01216f84 = uVar14;
          if ((uVar11 % (DAT_01216fb4 - 1) == 0) && (uVar11 != 0)) {
            piVar7 = malloc(0xc);
            puVar2 = local_2c;
            piVar7[2] = uVar14;
            uVar8 = *(undefined4 *)(local_2c + 0x6f64);
            piVar7[1] = uVar14 - local_30;
            *piVar7 = DAT_01216f90;
            DAT_01216f90 = DAT_01216f90 + 1;
            uVar8 = glist_append(uVar8,piVar7);
            *(undefined4 *)(puVar2 + 0x6f64) = uVar8;
            local_30 = uVar14;
            goto LAB_0040aaec;
          }
        }
        else {
          puVar6 = (uint *)((int)__s + uVar11 * 8);
          uVar14 = (uVar14 & 0xffff) + DAT_01216f84;
          if (DAT_01216fb4 - 1 == 0) {
            trap(7);
          }
          *puVar6 = uVar11;
          puVar6[1] = uVar14;
          DAT_01216f84 = uVar14;
          if ((uVar11 % (DAT_01216fb4 - 1) == 0) && (uVar11 != 0)) {
            piVar7 = malloc(0xc);
            puVar2 = local_2c;
            piVar7[2] = uVar14;
            uVar8 = *(undefined4 *)(local_2c + 0x6f64);
            piVar7[1] = uVar14 - local_30;
            *piVar7 = DAT_01216f90;
            DAT_01216f90 = DAT_01216f90 + 1;
            uVar8 = glist_append(uVar8,piVar7);
            *(undefined4 *)(puVar2 + 0x6f64) = uVar8;
            local_30 = uVar14;
            goto LAB_0040aaec;
          }
        }
        __stream = *(FILE **)(local_34 + 0x6fac);
        uVar11 = uVar11 + 1;
        if (__stream == (FILE *)0x0) break;
      } while( true );
    }
LAB_0040ac6c:
    if (uVar11 != 0) {
LAB_0040ac74:
      if (uVar11 < DAT_01216fb4) {
        piVar7 = malloc(0xc);
        uVar11 = DAT_01216f84;
        uVar8 = DAT_01216f64;
        *piVar7 = DAT_01216f90;
        piVar7[2] = uVar11;
        DAT_01216f90 = DAT_01216f90 + 1;
        piVar7[1] = uVar11 - local_30;
        DAT_01216f64 = glist_append(uVar8,piVar7);
      }
    }
  }
  else {
LAB_0040a890:
    uVar14 = 0;
LAB_0040a894:
    pcVar4 = fgets(local_442 + 2,0x400,*(FILE **)(local_34 + 0x6fac));
    if (pcVar4 != (char *)0x0) {
      do {
        if (*(int *)(local_38 + -0x6470) == -1) {
          if (uVar14 == 0) {
            return 0;
          }
          goto LAB_0040a9c8;
        }
        sVar5 = strlen(local_442 + 2);
        uVar9 = (uint)DAT_01216fb6;
        uVar13 = sVar5 & 0xffff;
        if (uVar9 < uVar13) {
          if (uVar9 == 0) {
            trap(7);
          }
          if (uVar13 / uVar9 != 0) {
            uVar10 = 0;
            do {
              puVar6 = (uint *)((int)__s + uVar14 * 8);
              uVar15 = uVar9 + DAT_01216f84;
              if (DAT_01216fb4 - 1 == 0) {
                trap(7);
              }
              *puVar6 = uVar14;
              puVar6[1] = uVar15;
              DAT_01216f84 = uVar15;
              if ((uVar14 % (DAT_01216fb4 - 1) == 0) && (uVar14 != 0)) {
                local_30 = uVar10;
                piVar7 = malloc(0xc);
                uVar8 = DAT_01216f64;
                uVar14 = 0;
                piVar7[1] = uVar15 - uVar11;
                *piVar7 = DAT_01216f90;
                piVar7[2] = uVar15;
                DAT_01216f90 = DAT_01216f90 + 1;
                DAT_01216f64 = glist_append(uVar8,piVar7);
                uVar9 = (uint)DAT_01216fb6;
                uVar10 = local_30;
                uVar11 = uVar15;
              }
              else {
                uVar14 = uVar14 + 1;
              }
              if (uVar9 == 0) {
                trap(7);
              }
              uVar10 = uVar10 + 1;
            } while ((int)uVar10 < (int)(uVar13 / uVar9));
          }
          if (uVar9 == 0) {
            trap(7);
          }
          if (uVar13 % uVar9 == 0) goto LAB_0040a894;
          puVar6 = (uint *)((int)__s + uVar14 * 8);
          uVar9 = uVar13 % uVar9 + DAT_01216f84;
          if (DAT_01216fb4 - 1 == 0) {
            trap(7);
          }
          uVar13 = uVar14 % (DAT_01216fb4 - 1);
          *puVar6 = uVar14;
          puVar6[1] = uVar9;
        }
        else {
          puVar6 = (uint *)((int)__s + uVar14 * 8);
          uVar9 = (sVar5 & 0xffff) + DAT_01216f84;
          if (DAT_01216fb4 - 1 == 0) {
            trap(7);
          }
          uVar13 = uVar14 % (DAT_01216fb4 - 1);
          *puVar6 = uVar14;
          puVar6[1] = uVar9;
        }
        DAT_01216f84 = uVar9;
        if ((uVar13 == 0) && (bVar1 = uVar14 != 0, uVar14 = 0, bVar1)) {
          piVar7 = malloc(0xc);
          uVar8 = DAT_01216f64;
          piVar7[1] = uVar9 - uVar11;
          *piVar7 = DAT_01216f90;
          piVar7[2] = uVar9;
          DAT_01216f90 = DAT_01216f90 + 1;
          DAT_01216f64 = glist_append(uVar8,piVar7);
          uVar11 = uVar9;
          goto LAB_0040a890;
        }
        uVar14 = uVar14 + 1;
        pcVar4 = fgets(local_442 + 2,0x400,*(FILE **)(local_34 + 0x6fac));
        if (pcVar4 == (char *)0x0) break;
      } while( true );
    }
    if (uVar14 != 0) {
LAB_0040a9c8:
      if (uVar14 < DAT_01216fb4) {
        piVar7 = malloc(0xc);
        uVar8 = DAT_01216f64;
        iVar12 = DAT_01216f84 - uVar11;
        piVar7[2] = DAT_01216f84;
        *piVar7 = DAT_01216f90;
        piVar7[1] = iVar12;
        DAT_01216f90 = DAT_01216f90 + 1;
        DAT_01216f64 = glist_append(uVar8,piVar7);
      }
    }
  }
  if (*(int *)(local_38 + -0x6470) == -1) {
    return 0;
  }
  *(undefined4 *)(local_38 + -0x6470) = 1;
  local_40[0] = 0x81;
  FUN_0042c5b4(local_40);
  if (0x80000 < DAT_01216fd0) {
    if (DAT_01216fb4 == 0) {
      trap(7);
    }
    DAT_01216fc0 = (DAT_01216fc0 + DAT_01216fc4) - (uint)DAT_01216fb8 / (uint)DAT_01216fb4;
  }
  DAT_01216fc8 = DAT_01216f90;
  uVar8 = glist_nth(DAT_01216f64,DAT_01216fc0 + -1);
  DAT_01216f90 = DAT_01216f90 + -1;
  DAT_01216f64 = uVar8;
  if (DAT_01216f6c != (void *)0x0) {
    free(DAT_01216f6c);
    DAT_01216f6c = (void *)0x0;
    uVar8 = extraout_v0;
  }
  return uVar8;
}



/* ===== FUNCTION FUN_0040af68 @ 0040af68 ===== */

void FUN_0040af68(undefined4 param_1)

{
  int iVar1;
  char *pcVar2;
  char acStack_410 [1028];
  
  if ((DAT_012167ec != (char *)0x0) && (*(short *)(DAT_012167ec + 0x406) != 0)) {
    iVar1 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
    pcVar2 = stpcpy(acStack_410,DAT_012167ec);
    *pcVar2 = '/';
    strcpy(pcVar2 + 1,(char *)(iVar1 + 10));
    FUN_0042922c(acStack_410,param_1);
  }
  FUN_00416760(0);
  return;
}



/* ===== FUNCTION FUN_0040b41c @ 0040b41c ===== */

void FUN_0040b41c(char *param_1,char *param_2,char *param_3)

{
  char *pcVar1;
  
  pcVar1 = stpcpy(param_1,param_2);
  *pcVar1 = '/';
  strcpy(pcVar1 + 1,param_3);
  return;
}



/* ===== FUNCTION FUN_0040b450 @ 0040b450 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0040b450(void)

{
  if (DAT_01216f70 != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_01216f70);
    DAT_01216f70 = 0;
  }
  if (DAT_01216fe8 != (void *)0x0) {
    free(DAT_01216fe8);
    DAT_01216fe8 = (void *)0x0;
  }
  if (DAT_01216fe4 != (void *)0x0) {
    free(DAT_01216fe4);
    DAT_01216fe4 = (void *)0x0;
  }
  if (DAT_01216fec != (void *)0x0) {
    free(DAT_01216fec);
    DAT_01216fec = (void *)0x0;
  }
  fclose(DAT_01216fac);
  DAT_01216fac = (FILE *)0x0;
  DAT_01216f74 = 0;
  DAT_01216fb0 = 0;
  DAT_01216f78 = 0;
  DAT_01216f7c = 0;
  DAT_01216f80 = 0;
  DAT_01216f84 = 0;
  DAT_01216f88 = 0;
  DAT_01216f8c = 0;
  DAT_01216f90 = 0;
  DAT_01216f94 = 0;
  DAT_01216f98 = 0;
  DAT_01216f9c = 0;
  DAT_01216fa0 = 0;
  DAT_01216fa4 = 0;
  DAT_01216fa8 = 0;
  _DAT_01216fb4 = 0;
  _DAT_01216fb8 = 0;
  DAT_01216fbc = 0;
  DAT_01216fc0 = 0;
  DAT_01216fc4 = 0;
  DAT_01216fc8 = 0;
  DAT_01216fcc = 0;
  DAT_01216fd0 = 0;
  DAT_01216fd4 = 0;
  DAT_01216fd8 = 0;
  DAT_01216fdc = 0;
  DAT_01216fe0 = 0;
  glist_free_full(DAT_01216f64,&LAB_0040a790);
  DAT_01216f64 = 0;
  DAT_01216fc8 = 1;
  return;
}



/* ===== FUNCTION FUN_0040b598 @ 0040b598 ===== */

bool FUN_0040b598(void)

{
  return DAT_01216fac != 0;
}



/* ===== FUNCTION FUN_0040b634 @ 0040b634 ===== */

void FUN_0040b634(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  lv_event_get_code();
  uVar1 = lv_event_get_target(param_1);
  uVar2 = lv_event_get_user_data(param_1);
  uVar3 = lv_btnmatrix_get_selected_btn(uVar1);
  puVar4 = (undefined4 *)lv_btnmatrix_get_btn_text(uVar1,uVar3);
  *puVar4 = 0x393939;
  lv_textarea_add_text(uVar2,puVar4);
  return;
}



/* ===== FUNCTION FUN_0040b6a4 @ 0040b6a4 ===== */

void FUN_0040b6a4(byte *param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  uint uVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  ushort *puVar6;
  
  uVar5 = 0;
  while (param_3 = param_3 + -1, param_3 != -1) {
    puVar6 = param_2 + 1;
    uVar1 = *param_2;
    uVar4 = (uint)uVar1;
    if (uVar5 == 0) {
      bVar3 = (byte)uVar1;
      uVar2 = 0;
      uVar5 = uVar2;
      if (uVar4 < 0x80) {
        *param_1 = bVar3;
        param_1 = param_1 + 1;
        param_2 = puVar6;
      }
      else if (uVar4 < 0x800) {
        *param_1 = (byte)(uVar1 >> 6) | 0xc0;
        param_1[1] = bVar3 & 0x3f | 0x80;
        param_1 = param_1 + 2;
        param_2 = puVar6;
      }
      else {
        param_2 = puVar6;
        uVar5 = uVar4;
        if (0x3ff < uVar4 - 0xd800) {
          if (uVar4 - 0xdc00 < 0x400) {
            *param_1 = 0x3f;
            param_1 = param_1 + 1;
            uVar5 = uVar2;
          }
          else {
            *param_1 = (byte)(uVar1 >> 0xc) | 0xe0;
            param_1[1] = (byte)((uVar4 << 0x14) >> 0x1a) | 0x80;
            param_1[2] = bVar3 & 0x3f | 0x80;
            param_1 = param_1 + 3;
            uVar5 = uVar2;
          }
        }
      }
    }
    else if (uVar4 - 0xdc00 < 0x400) {
      uVar5 = (uVar5 - 0xd800) * 0x400 + uVar4 + 0x2400;
      *param_1 = (byte)(uVar5 >> 0x12) | 0xf0;
      param_1[1] = (byte)(uVar5 * 0x4000 >> 0x1a) | 0x80;
      param_1[2] = (byte)(uVar5 * 0x100000 >> 0x1a) | 0x80;
      param_1[3] = (byte)uVar5 & 0x3f | 0x80;
      param_1 = param_1 + 4;
      param_2 = puVar6;
      uVar5 = 0;
    }
    else {
      *param_1 = 0x3f;
      param_1 = param_1 + 1;
      uVar5 = 0;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0040b8b0 @ 0040b8b0 ===== */

/* WARNING: Restarted to delay deadcode elimination for space: stack */

void FUN_0040b8b0(FILE *param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char local_421 [1025];
  
  iVar2 = FUN_00428454(0x40);
  if (param_1 == (FILE *)0x0) {
    return;
  }
  memset(local_421 + 1,0,0x400);
  uVar4 = 0;
  do {
    iVar3 = feof(param_1);
    if (iVar3 != 0) {
      if (((uVar4 & 1) != 0) || ((local_421[uVar4] == '\r' && (local_421[uVar4 + 1] == '\n'))))
      goto LAB_0040b9ec;
LAB_0040b9d8:
      uVar4 = uVar4 + 1;
      if (uVar4 != 0x400) goto LAB_0040b9ec;
      break;
    }
    fread(local_421 + uVar4 + 1,1,1,param_1);
    if (uVar4 == 0) {
      iVar3 = feof(param_1);
      if (iVar3 != 0) {
        uVar4 = 1;
        goto LAB_0040b9ec;
      }
      fread(local_421 + 2,1,1,param_1);
      uVar4 = 1;
      cVar1 = local_421[1];
      if (local_421[1] != '\n') goto LAB_0040b920;
LAB_0040b9b0:
      uVar5 = uVar4 + 1;
      if (local_421[uVar4 + 1] == '\0') {
LAB_0040b9cc:
        if ((uVar4 & 1) == 0) goto LAB_0040b9d8;
        goto LAB_0040b9ec;
      }
    }
    else {
      cVar1 = local_421[uVar4];
      if (cVar1 == '\n') goto LAB_0040b9b0;
LAB_0040b920:
      if (cVar1 == '\0') {
        if (local_421[uVar4 + 1] != '\n') goto LAB_0040b93c;
        goto LAB_0040b9cc;
      }
      if (cVar1 == '\r') {
        uVar5 = uVar4 + 1;
        if (local_421[uVar4 + 1] == '\n') goto LAB_0040b9ec;
      }
      else {
LAB_0040b93c:
        uVar5 = uVar4 + 1;
      }
    }
    uVar4 = uVar5;
  } while (uVar5 != 0x400);
  uVar4 = 0x3ff;
LAB_0040b9ec:
  iVar2 = iVar2 * 4;
  DAT_01216fb4 = (undefined2)*(undefined4 *)(&DAT_0044f898 + iVar2);
  DAT_01216fb6 = (undefined2)*(undefined4 *)(&DAT_0044f8a4 + iVar2);
  if (DAT_012171ec != 0) {
    lv_obj_set_style_text_font(DAT_012171ec,*(undefined4 *)(&UNK_0044f87c + iVar2),0);
  }
  if (local_421[1] == -0x11) {
    if (local_421[2] == -0x45) {
      DAT_01216fb0._2_1_ = 1;
      return;
    }
  }
  else if (local_421[1] == -1) {
    if (local_421[2] == -2) {
      DAT_01216fb0._2_1_ = 3;
      return;
    }
  }
  else if ((local_421[1] == -2) && (local_421[2] == -1)) {
    DAT_01216fb0._2_1_ = 2;
    return;
  }
  if (local_421[uVar4] == '\0') {
    if (local_421[uVar4 + 1] != '\0') {
      DAT_01216fb0._2_1_ = 4;
      return;
    }
  }
  else if (local_421[uVar4 + 1] == '\0') {
    DAT_01216fb0._2_1_ = 5;
    return;
  }
  DAT_01216fb0._2_1_ = 6;
  return;
}



/* ===== FUNCTION FUN_0040bd28 @ 0040bd28 ===== */

void FUN_0040bd28(void)

{
  ushort *puVar1;
  byte bVar2;
  bool bVar3;
  ushort uVar4;
  undefined4 uVar5;
  int iVar6;
  FILE *__stream;
  size_t sVar7;
  ushort *puVar8;
  byte *pbVar9;
  ushort *puVar10;
  uint uVar11;
  undefined2 local_1ca8;
  ushort local_1ca6 [2046];
  undefined1 auStack_caa [2];
  undefined2 local_ca8;
  ushort local_ca6 [1023];
  char acStack_4a8 [1024];
  stat sStack_a8;
  
  uVar5 = lv_label_create();
  lv_obj_set_size(uVar5,0x2064,0x204a);
  lv_obj_set_pos(uVar5,0x2005,0x200f);
  lv_obj_set_style_text_color(uVar5,0xffffffff,0);
  lv_obj_set_style_text_font(uVar5,&DAT_0121ed78,0);
  iVar6 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
  FUN_0040fda8(acStack_4a8,DAT_012167ec,iVar6 + 10);
  __stream = fopen(acStack_4a8,"r");
  if (__stream == (FILE *)0x0) {
    printf("%s,%d,open %s file failed\n","preview_read_ebook",0x256,acStack_4a8);
LAB_0040c160:
    puts("read file fail");
    return;
  }
  iVar6 = __xstat(3,acStack_4a8,&sStack_a8);
  if (iVar6 == -1) {
    puts("stat failed");
    goto LAB_0040c160;
  }
  if (0x1e00000 < sStack_a8.st_blksize) goto LAB_0040c160;
  FUN_0040b8b0(__stream);
  sVar7 = 0x400;
  if (sStack_a8.st_blksize < 0x401) {
    sVar7 = sStack_a8.st_blksize;
  }
  fseek(__stream,0,0);
  sVar7 = fread(&local_ca8,1,sVar7,__stream);
  if (DAT_01216fb0._2_1_ == '\x01') {
LAB_0040c0c8:
    lv_label_set_text(uVar5,&local_ca8);
  }
  else {
    if ((DAT_01216fb0._2_1_ == '\x03') || (DAT_01216fb0._2_1_ == '\x05')) {
      iVar6 = 1;
      puVar8 = &local_ca8;
      pbVar9 = (byte *)&local_1ca8;
      while( true ) {
        uVar11 = iVar6 - 1;
        if ((((byte)*puVar8 == 0) && (*(byte *)((int)puVar8 + 1) == 0)) && ((uVar11 & 1) == 0))
        break;
        *pbVar9 = (byte)*puVar8;
        puVar8 = (ushort *)((int)puVar8 + 1);
        iVar6 = iVar6 + 1;
        pbVar9 = pbVar9 + 1;
      }
      pbVar9 = (byte *)((int)&local_1ca8 + iVar6);
    }
    else {
      puVar8 = &local_ca8;
      if ((DAT_01216fb0._2_1_ != '\x02') &&
         (puVar10 = (ushort *)((int)&local_ca8 + sVar7), DAT_01216fb0._2_1_ != '\x04')) {
        iVar6 = 0;
        if (&local_ca8 < puVar10) {
          do {
            bVar2 = (byte)*puVar8;
            if ((char)bVar2 < '\0') {
              if (bVar2 < 0xc0) {
                iVar6 = iVar6 + 1;
                if (3 < iVar6) goto LAB_0040bedc;
                goto LAB_0040bea8;
              }
              if (0xdf < bVar2) {
                if (bVar2 < 0xf0) {
                  if (auStack_caa + sVar7 <= puVar8) break;
                  if (((*puVar8 & 0xc000) == 0x8000) &&
                     (puVar1 = puVar8 + 1, puVar8 = (ushort *)((int)puVar8 + 3),
                     (*puVar1 & 0xc0) == 0x80)) {
                    bVar3 = puVar8 < puVar10;
                    goto LAB_0040beac;
                  }
                }
LAB_0040bedc:
                FUN_0041ae48(&local_ca8,sVar7,&local_1ca8,DAT_011e9b94);
                puVar8 = &local_1ca8;
                while (local_1ca8 != 0) {
                  *puVar8 = local_1ca8 << 8 | local_1ca8 >> 8;
                  puVar8 = puVar8 + 1;
                  local_1ca8 = *puVar8;
                }
                memset(&local_ca8,0,0x400);
                iVar6 = 1;
                puVar8 = &local_1ca8;
                puVar10 = &local_ca8;
                while( true ) {
                  if ((((byte)*puVar8 == 0) && (*(byte *)((int)puVar8 + 1) == 0)) &&
                     ((iVar6 - 1U & 1) == 0)) break;
                  *(byte *)puVar10 = (byte)*puVar8;
                  puVar8 = (ushort *)((int)puVar8 + 1);
                  iVar6 = iVar6 + 1;
                  puVar10 = (ushort *)((int)puVar10 + 1);
                }
                *(undefined1 *)((int)local_ca6 + iVar6 + -2) = 0;
                auStack_caa[iVar6 + 1] = 0;
                memset(&local_1ca8,0,sVar7 << 1);
                FUN_0040b6a4(&local_1ca8,&local_ca8,sVar7);
                lv_label_set_text(uVar5,&local_1ca8);
                goto LAB_0040c030;
              }
              if (auStack_caa + sVar7 + 1 <= puVar8) break;
              uVar4 = *puVar8;
              puVar8 = puVar8 + 1;
              if ((uVar4 & 0xc000) != 0x8000) goto LAB_0040bedc;
              bVar3 = puVar8 < puVar10;
            }
            else {
LAB_0040bea8:
              puVar8 = (ushort *)((int)puVar8 + 1);
              bVar3 = puVar8 < puVar10;
            }
LAB_0040beac:
          } while (bVar3);
        }
        goto LAB_0040c0c8;
      }
      while (local_ca8 != 0) {
        *puVar8 = local_ca8 << 8 | local_ca8 >> 8;
        local_ca8 = puVar8[1];
        puVar8 = puVar8 + 1;
      }
      iVar6 = 1;
      puVar8 = &local_ca8;
      pbVar9 = (byte *)&local_1ca8;
      while( true ) {
        uVar11 = iVar6 - 1;
        if ((((byte)*puVar8 == 0) && (*(byte *)((int)puVar8 + 1) == 0)) && ((uVar11 & 1) == 0))
        break;
        *pbVar9 = (byte)*puVar8;
        puVar8 = (ushort *)((int)puVar8 + 1);
        iVar6 = iVar6 + 1;
        pbVar9 = pbVar9 + 1;
      }
      pbVar9 = (byte *)((int)&local_1ca8 + iVar6);
    }
    *pbVar9 = 0;
    *(undefined1 *)((int)&local_1ca8 + uVar11) = 0;
    memset(&local_ca8,0,0x400);
    FUN_0040b6a4(&local_ca8,&local_1ca8,sVar7);
    lv_label_set_text(uVar5,&local_ca8);
  }
LAB_0040c030:
  fclose(__stream);
  return;
}



/* ===== FUNCTION FUN_0040c19c @ 0040c19c ===== */

void FUN_0040c19c(int param_1)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  char acStack_50 [64];
  
  memset(acStack_50,0,0x40);
  iVar3 = DAT_01216fc0;
  if (DAT_011e9b90 == 1) {
    if ((DAT_01216f64 != (undefined4 *)0x0) && (param_1 == 1)) {
      piVar2 = (int *)*DAT_01216f64;
      *piVar2 = DAT_01216fc0;
      FUN_0040af68(piVar2);
    }
    sprintf(acStack_50,"%ld / %ld",DAT_01216fc0,DAT_01216fc8);
  }
  else {
    if (DAT_01216fd0 < 0x80000) {
      sprintf(acStack_50,"%ld / %ld",DAT_01216fc0,DAT_01216fc8);
    }
    else {
      if (DAT_01216fb4 == 0) {
        trap(7);
      }
      iVar1 = DAT_01216fc4 - (uint)DAT_01216fb8 / (uint)DAT_01216fb4;
      iVar3 = iVar1 + DAT_01216fc0;
      sprintf(acStack_50,"%ld / %ld",iVar3,iVar1 + DAT_01216fc8);
    }
    if (param_1 == 1) {
      piVar2 = malloc(0xc);
      iVar1 = DAT_01216fc0;
      uVar4 = (uint)DAT_01216fb4;
      *piVar2 = iVar3;
      iVar1 = uVar4 * iVar1;
      iVar3 = DAT_01216fec + iVar1 * 8 + -8;
      piVar2[1] = *(int *)(iVar3 + 4) - *(int *)(DAT_01216fec + (iVar1 - uVar4) * 8 + 4);
      piVar2[2] = *(int *)(iVar3 + 4);
      FUN_0040af68(piVar2);
      free(piVar2);
    }
  }
  lv_label_set_text(DAT_012171e8,acStack_50);
  return;
}



/* ===== FUNCTION FUN_0040c34c @ 0040c34c ===== */

void FUN_0040c34c(int param_1,uint param_2)

{
  ushort uVar1;
  int iVar2;
  ushort *puVar3;
  ushort *puVar4;
  ushort uVar5;
  int iVar6;
  char *pcVar7;
  ushort *puVar8;
  uint uVar9;
  ushort *puVar10;
  size_t sVar11;
  size_t sVar12;
  
  if (param_1 == 0x12) {
    if (DAT_01216fc0 == DAT_01216fc8) {
      return;
    }
    if (DAT_011e9b90 != 1) {
      if (DAT_01216fc8 - 1 <= DAT_01216fc0) {
        return;
      }
      DAT_01216fc0 = DAT_01216fc0 + 1;
      if (DAT_01216fc8 == DAT_01216fc0) {
        DAT_01216fbc = DAT_01216fdc;
        sVar11 = DAT_01216fdc;
      }
      else {
        iVar2 = DAT_01216fb4 * DAT_01216fc0;
        DAT_01216fbc = *(int *)(DAT_01216fec + (iVar2 + 0x1fffffff) * 8 + 4) -
                       *(int *)(DAT_01216fec +
                                ((uint)DAT_01216fb4 * 0x1fffffff + iVar2 + 0x1fffffff) * 8 + 4);
        sVar11 = DAT_01216fbc;
      }
      goto LAB_0040c514;
    }
    DAT_01216fc0 = DAT_01216fc0 + 1;
    DAT_01216f64 = (int *)glist_nth(DAT_01216f64,1);
    iVar2 = *DAT_01216f64;
LAB_0040c3f4:
    sVar11 = *(size_t *)(iVar2 + 4);
    DAT_01216fbc = sVar11;
    fseek(DAT_01216fac,*(int *)(iVar2 + 8) - *(int *)(iVar2 + 4),0);
LAB_0040c410:
    sVar12 = DAT_011e9b94;
    if (DAT_011e9b94 < DAT_01216fbc << 1) {
LAB_0040c52c:
      sVar12 = DAT_011e9b94;
      if (DAT_01216fe4 != (ushort *)0x0) {
        free(DAT_01216fe4);
        sVar12 = DAT_01216fbc << 1;
        DAT_011e9b94 = sVar12;
        DAT_01216fe4 = malloc(DAT_01216fbc << 2);
        if (DAT_01216fe4 == (ushort *)0x0) {
          pcVar7 = "str_mul malloc faild";
          goto LAB_0040ca74;
        }
      }
      if (DAT_01216fe8 != (ushort *)0x0) {
        free(DAT_01216fe8);
        sVar12 = DAT_01216fbc << 1;
        DAT_011e9b94 = sVar12;
        DAT_01216fe8 = malloc(sVar12);
        if (DAT_01216fe8 == (ushort *)0x0) {
          pcVar7 = "get_buff_mul malloc faild";
          DAT_01216fe8 = (ushort *)0x0;
LAB_0040ca74:
          puts(pcVar7);
          return;
        }
      }
    }
  }
  else {
    if (param_1 != 0x11) {
      if (DAT_011e9b90 == 0) {
        return;
      }
      if ((int)param_2 < 1) {
        return;
      }
      if (DAT_01216fc8 < param_2) {
        return;
      }
      if (param_2 == DAT_01216fc0) {
        return;
      }
      if (param_2 < DAT_01216fc0) {
        DAT_01216f64 = (int *)glist_nth_prev(DAT_01216f64,DAT_01216fc0 - param_2);
      }
      else if (DAT_01216fc0 < param_2) {
        DAT_01216f64 = (int *)glist_nth(DAT_01216f64,param_2 - DAT_01216fc0);
      }
      iVar2 = *DAT_01216f64;
      DAT_01216fc0 = param_2;
      goto LAB_0040c3f4;
    }
    if (DAT_01216fc0 == 1) {
      return;
    }
    if (DAT_011e9b90 == 1) {
      DAT_01216fc0 = DAT_01216fc0 - 1;
      DAT_01216f64 = (int *)glist_nth_prev(DAT_01216f64,1);
      iVar2 = *DAT_01216f64;
      sVar11 = *(size_t *)(iVar2 + 4);
      DAT_01216fdc = DAT_01216fe0 + sVar11;
      DAT_01216fbc = sVar11;
      fseek(DAT_01216fac,*(int *)(iVar2 + 8) - *(int *)(iVar2 + 4),0);
      if ((DAT_01216fc0 == 1) && ((byte)(DAT_01216fb0._2_1_ - 1U) < 3)) {
        fseek(DAT_01216fac,2,0);
        goto LAB_0040c514;
      }
      goto LAB_0040c410;
    }
    if (DAT_01216fc0 <= DAT_01216fd4) {
      return;
    }
    DAT_01216fc0 = DAT_01216fc0 - 1;
    if (DAT_01216fc0 == 1) {
      DAT_01216fbc = *(size_t *)(DAT_01216fec + (uint)DAT_01216fb4 * 8 + -4);
    }
    else {
      iVar2 = DAT_01216fb4 * DAT_01216fc0;
      DAT_01216fbc = *(int *)(DAT_01216fec + (iVar2 + 0x1fffffff) * 8 + 4) -
                     *(int *)(DAT_01216fec +
                              (iVar2 + (uint)DAT_01216fb4 * 0x1fffffff + 0x1fffffff) * 8 + 4);
    }
    DAT_01216fdc = DAT_01216fe0 + DAT_01216fbc;
    sVar11 = DAT_01216fbc;
LAB_0040c514:
    sVar12 = DAT_011e9b94;
    if (DAT_011e9b94 < DAT_01216fbc << 1) goto LAB_0040c52c;
  }
  puVar10 = DAT_01216fe8;
  memset(DAT_01216fe4,0,sVar12 << 1);
  memset(puVar10,0,sVar12);
  if (DAT_011e9b90 == 1) {
    fread(puVar10,DAT_01216fbc,1,DAT_01216fac);
    puVar10 = DAT_01216fe8;
  }
  else if ((DAT_01216fc0 == 1) && ((byte)(DAT_01216fb0._2_1_ - 1U) < 3)) {
    memcpy(puVar10,(void *)(DAT_01216f6c + (DAT_01216fd8 - DAT_01216fdc) + 2),DAT_01216fbc);
  }
  else {
    memcpy(puVar10,(void *)(DAT_01216f6c + (DAT_01216fd8 - DAT_01216fdc)),DAT_01216fbc);
  }
  puVar4 = DAT_01216fe4;
  iVar2 = DAT_01216fdc - DAT_01216fbc;
  DAT_01216fe0 = DAT_01216fdc;
  if (DAT_01216fb0._2_1_ != '\x01') {
    if ((DAT_01216fb0._2_1_ == '\x03') || (DAT_01216fb0._2_1_ == '\x05')) {
      if ((DAT_01216fe4 != (ushort *)0x0) && (puVar10 != (ushort *)0x0)) {
        iVar6 = 1;
        puVar3 = puVar10;
        puVar8 = DAT_01216fe4;
        while( true ) {
          DAT_01216fdc = iVar2;
          uVar9 = iVar6 - 1;
          if ((((char)*puVar3 == '\0') && (*(char *)((int)puVar3 + 1) == '\0')) &&
             ((uVar9 & 1) == 0)) break;
          *(char *)puVar8 = (char)*puVar3;
          puVar3 = (ushort *)((int)puVar3 + 1);
          iVar6 = iVar6 + 1;
          puVar8 = (ushort *)((int)puVar8 + 1);
          iVar2 = DAT_01216fdc;
        }
        pcVar7 = (char *)((int)puVar4 + iVar6);
LAB_0040c848:
        *pcVar7 = '\0';
        *(char *)((int)puVar4 + uVar9) = '\0';
        iVar2 = DAT_01216fdc;
      }
    }
    else {
      if ((DAT_01216fb0._2_1_ != '\x02') && (DAT_01216fb0._2_1_ != '\x04')) {
        if (DAT_01216fb0._1_1_ == '\0') {
          DAT_01216fdc = iVar2;
          FUN_0041ae48(puVar10,sVar11,DAT_01216fe4,DAT_011e9b94);
          puVar10 = DAT_01216fe8;
          puVar4 = DAT_01216fe4;
          sVar12 = DAT_011e9b94;
          if (DAT_01216fe4 == (ushort *)0x0) {
            memset(DAT_01216fe8,0,DAT_011e9b94);
          }
          else {
            uVar5 = *DAT_01216fe4;
            puVar3 = DAT_01216fe4;
            while (DAT_011e9b94 = sVar12, DAT_01216fe8 = puVar10, uVar5 != 0) {
              *puVar3 = uVar5 << 8 | uVar5 >> 8;
              puVar3 = puVar3 + 1;
              sVar12 = DAT_011e9b94;
              puVar10 = DAT_01216fe8;
              uVar5 = *puVar3;
            }
            memset(puVar10,0,sVar12);
            iVar2 = 1;
            puVar3 = puVar4;
            puVar8 = puVar10;
            while( true ) {
              if ((((char)*puVar3 == '\0') && (*(char *)((int)puVar3 + 1) == '\0')) &&
                 ((iVar2 - 1U & 1) == 0)) break;
              *(char *)puVar8 = (char)*puVar3;
              puVar3 = (ushort *)((int)puVar3 + 1);
              iVar2 = iVar2 + 1;
              puVar8 = (ushort *)((int)puVar8 + 1);
            }
            *(char *)((int)puVar10 + iVar2) = '\0';
            *(char *)((int)puVar10 + (iVar2 - 1U)) = '\0';
          }
          memset(puVar4,0,sVar12 << 1);
          FUN_0040b6a4(puVar4,puVar10,sVar11);
          lv_label_set_text(DAT_012171ec,puVar4);
          goto LAB_0040c888;
        }
        goto LAB_0040c878;
      }
      if (puVar10 != (ushort *)0x0) {
        uVar5 = *puVar10;
        if (uVar5 != 0) {
          uVar5 = uVar5 << 8 | uVar5 >> 8;
          puVar4 = puVar10;
          do {
            DAT_01216fdc = iVar2;
            *puVar4 = uVar5;
            puVar4 = puVar4 + 1;
            uVar1 = *puVar4;
            uVar5 = uVar1 << 8 | uVar1 >> 8;
            iVar2 = DAT_01216fdc;
          } while (uVar1 != 0);
        }
        DAT_01216fdc = iVar2;
        puVar4 = DAT_01216fe4;
        iVar2 = DAT_01216fdc;
        if (DAT_01216fe4 != (ushort *)0x0) {
          iVar2 = 1;
          puVar3 = puVar10;
          puVar8 = DAT_01216fe4;
          while( true ) {
            uVar9 = iVar2 - 1;
            if ((((char)*puVar3 == '\0') && (*(char *)((int)puVar3 + 1) == '\0')) &&
               ((uVar9 & 1) == 0)) break;
            *(char *)puVar8 = (char)*puVar3;
            puVar3 = (ushort *)((int)puVar3 + 1);
            iVar2 = iVar2 + 1;
            puVar8 = (ushort *)((int)puVar8 + 1);
          }
          pcVar7 = (char *)((int)puVar4 + iVar2);
          goto LAB_0040c848;
        }
      }
    }
    DAT_01216fdc = iVar2;
    memset(puVar10,0,DAT_011e9b94);
    FUN_0040b6a4(puVar10,puVar4,DAT_01216fbc);
    iVar2 = DAT_01216fdc;
  }
LAB_0040c878:
  DAT_01216fdc = iVar2;
  lv_label_set_text(DAT_012171ec,puVar10);
LAB_0040c888:
  FUN_0040c19c(1);
  return;
}



/* ===== FUNCTION FUN_0040cab0 @ 0040cab0 ===== */

undefined4 FUN_0040cab0(char *param_1)

{
  ushort *puVar1;
  byte bVar2;
  ushort uVar3;
  bool bVar4;
  FILE *pFVar5;
  char cVar6;
  undefined2 uVar7;
  ushort uVar8;
  int iVar9;
  int iVar10;
  char *pcVar11;
  size_t sVar12;
  uint uVar13;
  ushort *puVar14;
  void *__s;
  uint uVar15;
  int *piVar16;
  uint *puVar17;
  uint uVar18;
  byte *pbVar19;
  void *pvVar20;
  ushort *puVar21;
  char *pcVar22;
  uint uVar23;
  int iVar24;
  ushort *puVar25;
  char *pcVar26;
  byte *pbVar27;
  undefined *puVar28;
  stat local_c88 [11];
  int local_878;
  int local_874;
  int local_870;
  char acStack_86c [1022];
  char local_46e [1026];
  pthread_attr_t local_6c;
  undefined *local_48;
  undefined *local_44;
  undefined *local_40;
  undefined *local_3c;
  undefined *local_38;
  undefined *local_34;
  uint local_30;
  
  memset(local_46e + 2,0,0x400);
  memset(&local_878,0,0x40c);
  if (DAT_01216fac != (FILE *)0x0) {
    if (DAT_01216f6c != (void *)0x0) {
      free(DAT_01216f6c);
      DAT_01216f6c = (void *)0x0;
    }
    if (DAT_01216f68 != 0) {
      DAT_011e9b90 = 0xffffffff;
      pthread_join(DAT_01216f68,(void **)0x0);
    }
    DAT_01216f68 = 0;
    DAT_011e9b90 = -1;
    usleep(20000);
    FUN_0040b450();
  }
  memset(local_46e + 2,0,0x400);
  if (param_1 == (char *)0x0) {
    iVar9 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
    pcVar11 = stpcpy(local_46e + 2,DAT_012167ec);
    *pcVar11 = '/';
    strcpy(pcVar11 + 1,(char *)(iVar9 + 10));
  }
  else {
    strncpy(local_46e + 2,param_1,0x400);
  }
  iVar9 = FUN_0041d178();
  if ((iVar9 == 1) &&
     (iVar9 = FUN_004165bc(DAT_012167ec,local_46e + 2,&local_878,
                           *(undefined4 *)(DAT_012167ec + 0x400)), -1 < iVar9)) {
    FUN_0041d184(0);
    memset(local_46e + 2,0,0x400);
    strncpy(local_46e + 2,acStack_86c,0x400);
    pcVar22 = strtok(acStack_86c,"/");
    pcVar11 = (char *)0x0;
    if (pcVar22 != (char *)0x0) {
      do {
        pcVar26 = pcVar11;
        pcVar11 = strtok((char *)0x0,"/");
        pcVar22 = DAT_012167ec;
      } while (pcVar11 != (char *)0x0);
      if (pcVar26 != (char *)0x0) {
        uVar7 = FUN_0040feb4(DAT_012167ec,pcVar26);
        *(undefined2 *)(pcVar22 + 0x408) = uVar7;
        DAT_01216fc4 = local_878 + -1;
      }
    }
  }
  else {
    iVar9 = FUN_0041655c(DAT_012167ec,local_46e + 2,&local_878,*(undefined4 *)(DAT_012167ec + 0x400)
                        );
    if (iVar9 < 0) {
      memset(local_c88,0,0x410);
      sVar12 = strlen(local_46e + 2);
      memcpy(&local_c88[0].st_mode,local_46e + 2,sVar12 + 1);
      local_c88[0].st_dev._0_4_ = 3;
      FUN_00428c70(local_c88);
    }
    else {
      DAT_01216fc4 = local_878 + -1;
    }
  }
  FUN_00416760(0);
  DAT_01216fac = fopen(local_46e + 2,"r");
  if (DAT_01216fac == (FILE *)0x0) {
    printf("%s,%d,open %s file failed\n","ebook_read_file",0x3c8,local_46e + 2);
    return 0xffffffff;
  }
  iVar9 = __xstat(3,local_46e + 2,local_c88);
  sVar12 = DAT_011e9b94;
  if (iVar9 == -1) {
    puts("stat failed");
    return 0xffffffff;
  }
  if (0x1e00000 < local_c88[0].st_blksize) {
    return 0xffffffff;
  }
  local_38 = &DAT_01210000;
  local_34 = &DAT_01210000;
  if ((DAT_01216fe8 == (void *)0x0) || (DAT_01216fe4 == (void *)0x0)) {
    local_3c = &DAT_011f0000;
    local_48 = (undefined *)(DAT_011e9b94 << 1);
    __s = malloc(DAT_011e9b94);
    *(void **)(local_38 + 0x6fe8) = __s;
    pvVar20 = malloc((size_t)local_48);
    *(void **)(local_34 + 0x6fe4) = pvVar20;
    puVar28 = local_48;
  }
  else {
    local_3c = &DAT_011f0000;
    pvVar20 = DAT_01216fe4;
    puVar28 = (undefined *)(DAT_011e9b94 << 1);
    __s = DAT_01216fe8;
  }
  memset(pvVar20,0,(size_t)puVar28);
  memset(__s,0,sVar12);
  DAT_01216fbc = 0;
  DAT_01216fc0 = 1;
  DAT_01216fd4 = 0;
  DAT_01216fdc = 0;
  DAT_01216fe0 = 0;
  fseek(DAT_01216fac,0,0);
  FUN_0040b8b0(DAT_01216fac);
  DAT_01216fd8 = local_c88[0].st_blksize;
  DAT_01216fdc = local_c88[0].st_blksize;
  DAT_01216fe0 = local_c88[0].st_blksize;
  if (local_870 == local_874) {
LAB_0040dc54:
    if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
      fseek(DAT_01216fac,2,0);
    }
    else {
      fseek(DAT_01216fac,0,0);
    }
LAB_0040d1a4:
    local_30 = 0;
    if (3 < (byte)(DAT_01216fb0._2_1_ - 2)) goto LAB_0040d1bc;
LAB_0040cd5c:
    if (DAT_01216fac != (FILE *)0x0) {
      local_48 = (undefined *)0x0;
      uVar15 = 0;
      local_44 = (undefined *)0x100001;
      local_40 = (undefined *)0x80001;
      do {
        pFVar5 = DAT_01216fac;
        iVar9 = feof(DAT_01216fac);
        if (iVar9 != 0) break;
        pcVar11 = local_46e;
        uVar23 = 0;
        do {
          pcVar11 = pcVar11 + 2;
          iVar9 = feof(pFVar5);
          if (iVar9 != 0) {
LAB_0040d7d8:
            uVar13 = uVar23 & 0xffff;
            local_46e[uVar23 + 2] = '\0';
            if (uVar13 != 0) goto LAB_0040ce00;
            goto LAB_0040ce9c;
          }
          fread(pcVar11,2,1,pFVar5);
          if ((pcVar11[-2] == '\r') && (pcVar11[-1] == '\0')) {
            cVar6 = *pcVar11;
            if (cVar6 != '\n') goto LAB_0040cdd4;
            if (pcVar11[1] == '\0') goto LAB_0040d7d4;
          }
          else {
            cVar6 = *pcVar11;
LAB_0040cdd4:
            if ((cVar6 == '\0') && (pcVar11[1] == '\n')) {
LAB_0040d7d4:
              uVar23 = uVar23 + 2;
              goto LAB_0040d7d8;
            }
          }
          uVar23 = uVar23 + 2;
        } while (uVar23 != 0x400);
        uVar23 = 0x400;
        local_6c.__size[0] = 0;
        uVar13 = 0x400;
LAB_0040ce00:
        local_48 = local_48 + (uVar23 & 0xffff);
        if (DAT_01216fd4 == 0) {
          if (local_40 <= local_48) {
            if (DAT_01216fb4 == 0) {
              trap(7);
            }
            DAT_01216fd4 = uVar15 / DAT_01216fb4;
            goto LAB_0040ce3c;
          }
        }
        else {
LAB_0040ce3c:
          if (local_44 <= local_48) break;
        }
        uVar23 = (uint)DAT_01216fb6;
        if (uVar23 < uVar13) {
          if (uVar23 == 0) {
            trap(7);
          }
          uVar15 = uVar15 + uVar13 / uVar23;
          if (uVar13 % uVar23 != 0) {
            uVar15 = uVar15 + 1;
          }
        }
        else {
          uVar15 = uVar15 + 1;
        }
        memset(local_46e + 2,0,0x400);
      } while (DAT_01216fac != (FILE *)0x0);
      goto LAB_0040ce9c;
    }
  }
  else {
    uVar15 = local_870 - local_874;
    if ((local_c88[0].st_blksize < 0x100000) || (uVar15 < 0x80000)) {
      if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
        fseek(DAT_01216fac,2,0);
      }
      else {
        fseek(DAT_01216fac,0,0);
      }
      DAT_01216fd0 = (undefined *)(local_870 - local_874);
      goto LAB_0040d1a4;
    }
    if (uVar15 < 0x80001) goto LAB_0040dc54;
    local_30 = uVar15 - 0x80000;
    if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
      fseek(DAT_01216fac,uVar15 - 0x7fffe,0);
    }
    else {
      fseek(DAT_01216fac,local_30,0);
    }
    DAT_01216fd0 = (undefined *)0x100000;
    if ((byte)(DAT_01216fb0._2_1_ - 2) < 4) goto LAB_0040cd5c;
LAB_0040d1bc:
    uVar23 = 0;
    uVar15 = 0;
    while( true ) {
      pcVar11 = fgets(local_46e + 2,0x400,DAT_01216fac);
      if (pcVar11 == (char *)0x0) break;
      sVar12 = strlen(local_46e + 2);
      uVar23 = uVar23 + (sVar12 & 0xffff);
      uVar13 = sVar12 & 0xffff;
      if (DAT_01216fd4 == 0) {
        if (0x80000 < uVar23) {
          if (DAT_01216fb4 == 0) {
            trap(7);
          }
          DAT_01216fd4 = uVar15 / DAT_01216fb4;
          goto LAB_0040d258;
        }
      }
      else {
LAB_0040d258:
        if (0x100000 < uVar23) break;
      }
      uVar18 = (uint)DAT_01216fb6;
      if (uVar18 < uVar13) {
        if (uVar18 == 0) {
          trap(7);
        }
        uVar15 = uVar15 + uVar13 / uVar18;
        if (uVar13 % uVar18 != 0) {
          uVar15 = uVar15 + 1;
        }
      }
      else {
        uVar15 = uVar15 + 1;
      }
      memset(local_46e + 2,0,0x400);
    }
LAB_0040ce9c:
    if (uVar15 != 0) {
      DAT_01216fec = malloc((uVar15 + 1) * 8);
      local_44 = &DAT_01210000;
      memset(local_46e + 2,0,0x400);
      if (local_870 == local_874) {
LAB_0040df24:
        if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
          fseek(DAT_01216fac,2,0);
        }
        else {
          fseek(DAT_01216fac,0,0);
        }
LAB_0040d2b4:
        uVar15 = (uint)DAT_01216fb0._2_1_;
        if (uVar15 - 2 < 4) goto LAB_0040cf54;
LAB_0040d2cc:
        local_48 = (undefined *)0x0;
        bVar4 = false;
        uVar23 = 0;
        DAT_01216fbc = local_30;
        while (pcVar11 = fgets(local_46e + 2,0x400,DAT_01216fac), pcVar11 != (char *)0x0) {
          sVar12 = strlen(local_46e + 2);
          puVar28 = local_48 + (sVar12 & 0xffff);
          uVar15 = sVar12 & 0xffff;
          if ((undefined *)0x100000 < puVar28) break;
          uVar13 = (uint)DAT_01216fb6;
          if (uVar13 < uVar15) {
            if (uVar13 == 0) {
              trap(7);
            }
            uVar18 = 0;
            if (uVar15 / uVar13 != 0) {
              iVar9 = 0;
              piVar16 = (int *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
              do {
                iVar10 = iVar9 + uVar23;
                iVar9 = iVar9 + 1;
                *piVar16 = iVar10;
                DAT_01216fbc = DAT_01216fb6 + DAT_01216fbc;
                piVar16[1] = DAT_01216fbc;
                uVar13 = (uint)DAT_01216fb6;
                if (uVar13 == 0) {
                  trap(7);
                }
                uVar18 = uVar15 / uVar13;
                piVar16 = piVar16 + 2;
              } while (iVar9 < (int)uVar18);
            }
            if (uVar13 == 0) {
              trap(7);
            }
            uVar23 = uVar23 + uVar18;
            if (uVar15 % uVar13 != 0) {
              puVar17 = (uint *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
              *puVar17 = uVar23;
              uVar23 = uVar23 + 1;
              if (DAT_01216fb6 == 0) {
                trap(7);
              }
              DAT_01216fbc = uVar15 % (uint)DAT_01216fb6 + DAT_01216fbc;
              puVar17[1] = DAT_01216fbc;
            }
            if (DAT_01216fd0 == (undefined *)0x100000) goto LAB_0040d780;
LAB_0040d3f0:
            if (DAT_01216fd0 < (undefined *)0x100001) goto LAB_0040d790;
          }
          else {
            puVar17 = (uint *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
            *puVar17 = uVar23;
            DAT_01216fbc = (sVar12 & 0xffff) + DAT_01216fbc;
            puVar17[1] = DAT_01216fbc;
            uVar23 = uVar23 + 1;
            if (DAT_01216fd0 != (undefined *)0x100000) goto LAB_0040d3f0;
LAB_0040d780:
            if ((undefined *)0x7ffff < puVar28) {
              if (bVar4) {
LAB_0040d790:
                if ((DAT_01216fd0 <= puVar28) && (!bVar4)) {
                  bVar4 = true;
                  DAT_01216fb8 = (ushort)uVar23;
                }
              }
              else {
                bVar4 = true;
                DAT_01216fb8 = (ushort)uVar23;
              }
            }
          }
          memset(local_46e + 2,0,0x400);
          local_48 = puVar28;
        }
LAB_0040d11c:
        uVar15 = (uint)DAT_01216fb0._2_1_;
      }
      else {
        uVar15 = local_870 - local_874;
        if ((DAT_01216fdc < 0x100000) || (uVar15 < 0x80000)) {
          if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
            fseek(DAT_01216fac,2,0);
          }
          else {
            fseek(DAT_01216fac,0,0);
          }
          DAT_01216fd0 = (undefined *)(local_870 - local_874);
          goto LAB_0040d2b4;
        }
        if (uVar15 < 0x80001) goto LAB_0040df24;
        local_30 = uVar15 - 0x80000;
        if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
          fseek(DAT_01216fac,uVar15 - 0x7fffe,0);
        }
        else {
          fseek(DAT_01216fac,local_30,0);
        }
        DAT_01216fd0 = (undefined *)0x100000;
        uVar15 = (uint)DAT_01216fb0._2_1_;
        if (3 < uVar15 - 2) goto LAB_0040d2cc;
LAB_0040cf54:
        pFVar5 = DAT_01216fac;
        DAT_01216fbc = local_30;
        if (DAT_01216fac != (FILE *)0x0) {
          local_48 = (undefined *)0x0;
          local_40 = (undefined *)0x0;
          uVar23 = 0;
          iVar9 = feof(DAT_01216fac);
          while (iVar9 == 0) {
            uVar15 = 0;
            pcVar11 = local_46e;
            do {
              pcVar11 = pcVar11 + 2;
              iVar9 = feof(pFVar5);
              if (iVar9 != 0) {
LAB_0040dac4:
                uVar13 = uVar15 & 0xffff;
                local_46e[uVar15 + 2] = '\0';
                if (uVar13 != 0) goto LAB_0040cfe0;
                goto LAB_0040d11c;
              }
              fread(pcVar11,2,1,pFVar5);
              if ((pcVar11[-2] == '\r') && (pcVar11[-1] == '\0')) {
                cVar6 = *pcVar11;
                if (cVar6 != '\n') goto LAB_0040cfb4;
                if (pcVar11[1] == '\0') goto LAB_0040dac0;
              }
              else {
                cVar6 = *pcVar11;
LAB_0040cfb4:
                if ((cVar6 == '\0') && (pcVar11[1] == '\n')) {
LAB_0040dac0:
                  uVar15 = uVar15 + 2;
                  goto LAB_0040dac4;
                }
              }
              uVar15 = uVar15 + 2;
            } while (uVar15 != 0x400);
            uVar15 = 0x400;
            local_6c.__size[0] = 0;
            uVar13 = 0x400;
LAB_0040cfe0:
            puVar28 = local_48 + (uVar15 & 0xffff);
            if ((undefined *)0x100000 < puVar28) break;
            uVar18 = (uint)DAT_01216fb6;
            if (uVar18 < uVar13) {
              if (uVar18 == 0) {
                trap(7);
              }
              uVar15 = 0;
              if (uVar13 / uVar18 != 0) {
                piVar16 = (int *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
                iVar9 = 0;
                do {
                  iVar10 = iVar9 + uVar23;
                  iVar9 = iVar9 + 1;
                  *piVar16 = iVar10;
                  DAT_01216fbc = DAT_01216fb6 + DAT_01216fbc;
                  piVar16[1] = DAT_01216fbc;
                  uVar18 = (uint)DAT_01216fb6;
                  if (uVar18 == 0) {
                    trap(7);
                  }
                  uVar15 = uVar13 / uVar18;
                  piVar16 = piVar16 + 2;
                } while (iVar9 < (int)uVar15);
              }
              if (uVar18 == 0) {
                trap(7);
              }
              uVar23 = uVar23 + uVar15;
              if (uVar13 % uVar18 != 0) {
                puVar17 = (uint *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
                *puVar17 = uVar23;
                uVar23 = uVar23 + 1;
                if (DAT_01216fb6 == 0) {
                  trap(7);
                }
                DAT_01216fbc = uVar13 % (uint)DAT_01216fb6 + DAT_01216fbc;
                puVar17[1] = DAT_01216fbc;
              }
              if (DAT_01216fd0 == (undefined *)0x100000) goto LAB_0040db18;
LAB_0040d0d8:
              if (DAT_01216fd0 < (undefined *)0x100001) goto LAB_0040db8c;
LAB_0040d0ec:
              memset(local_46e + 2,0,0x400);
              pFVar5 = DAT_01216fac;
            }
            else {
              puVar17 = (uint *)(*(int *)(local_44 + 0x6fec) + uVar23 * 8);
              *puVar17 = uVar23;
              DAT_01216fbc = (uVar15 & 0xffff) + DAT_01216fbc;
              puVar17[1] = DAT_01216fbc;
              uVar23 = uVar23 + 1;
              if (DAT_01216fd0 != (undefined *)0x100000) goto LAB_0040d0d8;
LAB_0040db18:
              if (puVar28 < (undefined *)0x80000) goto LAB_0040d0ec;
              if (local_40 != (undefined *)0x0) {
LAB_0040db8c:
                if ((DAT_01216fd0 <= puVar28) && (local_40 == (undefined *)0x0)) {
                  DAT_01216fb8 = (ushort)uVar23;
                  local_40 = (undefined *)0x1;
                }
                goto LAB_0040d0ec;
              }
              DAT_01216fb8 = (ushort)uVar23;
              local_40 = (undefined *)0x1;
              memset(local_46e + 2,0,0x400);
              pFVar5 = DAT_01216fac;
            }
            DAT_01216fac = pFVar5;
            local_48 = puVar28;
            if (pFVar5 == (FILE *)0x0) {
              uVar15 = (uint)DAT_01216fb0._2_1_;
              if (local_870 != local_874) goto LAB_0040d130;
              goto LAB_0040db68;
            }
            iVar9 = feof(pFVar5);
          }
          goto LAB_0040d11c;
        }
        uVar23 = 0;
        local_48 = (undefined *)0x0;
      }
      if (local_870 == local_874) {
LAB_0040db68:
        if (uVar15 - 1 < 3) {
          fseek(DAT_01216fac,2,0);
        }
        else {
          fseek(DAT_01216fac,0,0);
        }
      }
      else {
LAB_0040d130:
        uVar13 = local_870 - local_874;
        if ((DAT_01216fdc < 0x100000) || (uVar13 < 0x80000)) {
          if (uVar15 - 1 < 3) {
            fseek(DAT_01216fac,2,0);
          }
          else {
            fseek(DAT_01216fac,0,0);
          }
          DAT_01216fd0 = (undefined *)(local_870 - local_874);
        }
        else {
          if (uVar13 < 0x80001) goto LAB_0040db68;
          if (uVar15 - 1 < 3) {
            fseek(DAT_01216fac,uVar13 - 0x7fffe,0);
            DAT_01216fd0 = (undefined *)0x100000;
          }
          else {
            fseek(DAT_01216fac,uVar13 - 0x80000,0);
            DAT_01216fd0 = (undefined *)0x100000;
          }
        }
      }
      puVar28 = local_48;
      DAT_01216f6c = malloc((size_t)local_48);
      fread(DAT_01216f6c,1,(size_t)puVar28,DAT_01216fac);
      uVar15 = (uint)DAT_01216fb4;
      if (uVar15 == 0) {
        trap(7);
      }
      DAT_01216fc8 = uVar23 / uVar15;
      if (uVar15 == 0) {
        trap(7);
      }
      DAT_01216fc0 = DAT_01216fb8 / uVar15 + 1;
      uVar13 = DAT_01216fc0 - DAT_01216fd4;
      if (DAT_01216fc0 <= DAT_01216fd4) {
        uVar13 = 0;
      }
      if (uVar23 % uVar15 != 0) {
        DAT_01216fc8 = DAT_01216fc8 + 1;
      }
      DAT_01216fcc = uVar23;
      DAT_01216fd4 = uVar13;
      if ((byte)(DAT_01216fb0._2_1_ - 1) < 3) {
        fseek(DAT_01216fac,2,0);
        if (DAT_01216fc0 != 1) goto LAB_0040d4ec;
LAB_0040d9a0:
        uVar15 = (uint)DAT_01216fb4;
        bVar4 = uVar23 < uVar15;
        iVar9 = *(int *)(local_44 + 0x6fec);
        if ((bVar4) && (uVar23 != 0)) {
          sVar12 = *(uint *)(iVar9 + uVar23 * 8 + -4);
        }
        else {
          sVar12 = *(uint *)(iVar9 + uVar15 * 8 + -4);
        }
        if (*(uint *)(local_3c + -0x646c) < sVar12) {
LAB_0040d9dc:
          puVar28 = local_34;
          pvVar20 = *(void **)(local_34 + 0x6fe4);
          *(size_t *)(local_3c + -0x646c) = sVar12;
          if (pvVar20 != (void *)0x0) {
            pvVar20 = realloc(pvVar20,sVar12 << 1);
            *(void **)(puVar28 + 0x6fe4) = pvVar20;
            if (pvVar20 == (void *)0x0) {
              puts("str_mul realloc faild");
              return 0xffffffff;
            }
          }
          puVar28 = local_38;
          puVar25 = (ushort *)0x0;
          if (*(void **)(local_38 + 0x6fe8) != (void *)0x0) {
            puVar25 = realloc(*(void **)(local_38 + 0x6fe8),sVar12);
            *(ushort **)(puVar28 + 0x6fe8) = puVar25;
            if (puVar25 == (ushort *)0x0) {
              puts("get_buff_mul realloc faild");
              return 0xffffffff;
            }
          }
          if (DAT_01216fc0 != 1) {
            uVar15 = (uint)DAT_01216fb4;
            iVar10 = *(int *)(iVar9 + ((DAT_01216fc0 + 0x1fffffff) * uVar15 + 0x1fffffff) * 8 + 4);
            if (DAT_01216fc0 != DAT_01216fc8) goto LAB_0040d558;
LAB_0040dcc4:
            iVar24 = iVar9 + (uVar23 + 0x1fffffff) * 8;
            memcpy(puVar25,(void *)((int)DAT_01216f6c + iVar10),*(int *)(iVar24 + 4) - iVar10);
            iVar10 = *(int *)(iVar24 + 4);
            DAT_01216fdc = DAT_01216fdc - iVar10;
            iVar10 = iVar10 - *(int *)(iVar9 + ((DAT_01216fc0 + 0x1fffffff) * (uint)DAT_01216fb4 +
                                               0x1fffffff) * 8 + 4);
            goto LAB_0040d5d0;
          }
          uVar15 = (uint)DAT_01216fb4;
          bVar4 = uVar23 < uVar15;
        }
        else {
          puVar25 = *(ushort **)(local_38 + 0x6fe8);
        }
        if ((bVar4) && (uVar23 != 0)) {
          iVar9 = iVar9 + uVar23 * 8 + -8;
          memcpy(puVar25,DAT_01216f6c,*(size_t *)(iVar9 + 4));
          iVar10 = *(int *)(iVar9 + 4);
          DAT_01216fdc = DAT_01216fdc - iVar10;
        }
        else {
          memcpy(puVar25,DAT_01216f6c,*(size_t *)(iVar9 + (uVar15 + 0x1fffffff) * 8 + 4));
          iVar10 = *(int *)(iVar9 + (DAT_01216fb4 * DAT_01216fc0 + 0x1fffffff) * 8 + 4);
          DAT_01216fdc = DAT_01216fdc - iVar10;
        }
      }
      else {
        fseek(DAT_01216fac,0,0);
        if (DAT_01216fc0 == 1) goto LAB_0040d9a0;
LAB_0040d4ec:
        uVar15 = (uint)DAT_01216fb4;
        iVar9 = *(int *)(local_44 + 0x6fec);
        if (DAT_01216fc0 == DAT_01216fc8) {
          iVar10 = *(int *)(iVar9 + ((DAT_01216fc0 + 0x1fffffff) * uVar15 + 0x1fffffff) * 8 + 4);
          sVar12 = *(int *)(iVar9 + (uVar23 + 0x1fffffff) * 8 + 4) - iVar10;
        }
        else {
          iVar10 = *(int *)(iVar9 + (uVar15 * 0x1fffffff + DAT_01216fc0 * uVar15 + 0x1fffffff) * 8 +
                           4);
          sVar12 = *(int *)(iVar9 + (DAT_01216fc0 * uVar15 + 0x1fffffff) * 8 + 4) - iVar10;
        }
        if (*(uint *)(local_3c + -0x646c) < sVar12) goto LAB_0040d9dc;
        puVar25 = *(ushort **)(local_38 + 0x6fe8);
        if (DAT_01216fc0 == DAT_01216fc8) goto LAB_0040dcc4;
LAB_0040d558:
        memcpy(puVar25,(void *)((int)DAT_01216f6c + iVar10),
               *(int *)(iVar9 + (DAT_01216fc0 * uVar15 + 0x1fffffff) * 8 + 4) - iVar10);
        iVar24 = DAT_01216fb4 * DAT_01216fc0;
        iVar10 = *(int *)(iVar9 + (iVar24 + 0x1fffffff) * 8 + 4);
        DAT_01216fdc = DAT_01216fdc - iVar10;
        iVar10 = iVar10 - *(int *)(iVar9 + ((uint)DAT_01216fb4 * 0x1fffffff + iVar24 + 0x1fffffff) *
                                           8 + 4);
      }
LAB_0040d5d0:
      puVar28 = local_34;
      if (DAT_01216fb0._2_1_ == 1) {
LAB_0040d934:
        lv_label_set_text(DAT_012171ec,puVar25);
      }
      else {
        if ((DAT_01216fb0._2_1_ == 3) || (DAT_01216fb0._2_1_ == 5)) {
          pbVar27 = *(byte **)(local_34 + 0x6fe4);
          if (pbVar27 != (byte *)0x0) {
            iVar9 = 1;
            puVar21 = puVar25;
            pbVar19 = pbVar27;
            while( true ) {
              uVar15 = iVar9 - 1;
              if ((((byte)*puVar21 == 0) && (*(byte *)((int)puVar21 + 1) == 0)) &&
                 ((uVar15 & 1) == 0)) break;
              *pbVar19 = (byte)*puVar21;
              puVar21 = (ushort *)((int)puVar21 + 1);
              iVar9 = iVar9 + 1;
              pbVar19 = pbVar19 + 1;
            }
            pbVar19 = pbVar27 + iVar9;
LAB_0040d904:
            *pbVar19 = 0;
            pbVar27[uVar15] = 0;
          }
LAB_0040d910:
          memset(puVar25,0,*(size_t *)(local_3c + -0x646c));
          FUN_0040b6a4(puVar25,pbVar27,iVar10);
          goto LAB_0040d934;
        }
        if ((DAT_01216fb0._2_1_ == 2) ||
           (puVar21 = (ushort *)((int)puVar25 + iVar10), DAT_01216fb0._2_1_ == 4)) {
          uVar3 = *puVar25;
          uVar8 = uVar3 << 8 | uVar3 >> 8;
          puVar21 = puVar25;
          while (uVar3 != 0) {
            *puVar21 = uVar8;
            puVar21 = puVar21 + 1;
            uVar3 = *puVar21;
            uVar8 = uVar3 << 8 | uVar3 >> 8;
          }
          pbVar27 = *(byte **)(local_34 + 0x6fe4);
          if (pbVar27 != (byte *)0x0) {
            iVar9 = 1;
            puVar21 = puVar25;
            pbVar19 = pbVar27;
            while( true ) {
              uVar15 = iVar9 - 1;
              if ((((byte)*puVar21 == 0) && (*(byte *)((int)puVar21 + 1) == 0)) &&
                 ((uVar15 & 1) == 0)) break;
              *pbVar19 = (byte)*puVar21;
              puVar21 = (ushort *)((int)puVar21 + 1);
              iVar9 = iVar9 + 1;
              pbVar19 = pbVar19 + 1;
            }
            pbVar19 = pbVar27 + iVar9;
            goto LAB_0040d904;
          }
          goto LAB_0040d910;
        }
        iVar9 = 0;
        if (puVar25 < puVar21) {
          puVar14 = puVar25;
          do {
            bVar2 = (byte)*puVar14;
            if ((char)bVar2 < '\0') {
              if (bVar2 < 0xc0) {
                iVar9 = iVar9 + 1;
                if (3 < iVar9) goto LAB_0040d65c;
                goto LAB_0040d624;
              }
              if (bVar2 < 0xe0) {
                if (puVar14 < (ushort *)((int)puVar21 + -1)) {
                  uVar3 = *puVar14;
                  puVar14 = puVar14 + 1;
                  if ((uVar3 & 0xc000) != 0x8000) goto LAB_0040d65c;
                  bVar4 = puVar14 < puVar21;
                  goto LAB_0040d628;
                }
                break;
              }
              if (bVar2 < 0xf0) {
                if (puVar21 + -1 <= puVar14) break;
                if (((*puVar14 & 0xc000) == 0x8000) &&
                   (puVar1 = puVar14 + 1, puVar14 = (ushort *)((int)puVar14 + 3),
                   (*puVar1 & 0xc0) == 0x80)) {
                  bVar4 = puVar14 < puVar21;
                  goto LAB_0040d628;
                }
              }
LAB_0040d65c:
              FUN_0041ae48(puVar25,iVar10,*(undefined4 *)(local_34 + 0x6fe4),
                           *(undefined4 *)(local_3c + -0x646c));
              puVar25 = *(ushort **)(puVar28 + 0x6fe4);
              if (puVar25 == (ushort *)0x0) {
                pcVar11 = *(char **)(local_38 + 0x6fe8);
                sVar12 = *(size_t *)(local_3c + -0x646c);
                memset(pcVar11,0,sVar12);
              }
              else {
                uVar3 = *puVar25;
                puVar21 = puVar25;
                while (uVar3 != 0) {
                  *puVar21 = uVar3 << 8 | uVar3 >> 8;
                  puVar21 = puVar21 + 1;
                  uVar3 = *puVar21;
                }
                pcVar11 = *(char **)(local_38 + 0x6fe8);
                sVar12 = *(size_t *)(local_3c + -0x646c);
                memset(pcVar11,0,sVar12);
                iVar9 = 1;
                puVar21 = puVar25;
                pcVar22 = pcVar11;
                while( true ) {
                  if ((((char)*puVar21 == '\0') && (*(char *)((int)puVar21 + 1) == '\0')) &&
                     ((iVar9 - 1U & 1) == 0)) break;
                  *pcVar22 = (char)*puVar21;
                  puVar21 = (ushort *)((int)puVar21 + 1);
                  iVar9 = iVar9 + 1;
                  pcVar22 = pcVar22 + 1;
                }
                pcVar11[iVar9] = '\0';
                pcVar11[iVar9 - 1U] = '\0';
              }
              memset(puVar25,0,sVar12 << 1);
              FUN_0040b6a4(puVar25,pcVar11,iVar10);
              lv_label_set_text(DAT_012171ec,puVar25);
              goto LAB_0040d948;
            }
LAB_0040d624:
            puVar14 = (ushort *)((int)puVar14 + 1);
            bVar4 = puVar14 < puVar21;
LAB_0040d628:
          } while (bVar4);
        }
        DAT_01216fb0._1_1_ = 1;
        lv_label_set_text(DAT_012171ec,puVar25);
      }
LAB_0040d948:
      DAT_01216fe0 = DAT_01216fdc + iVar10;
      FUN_0040c19c(1);
      goto joined_r0x0040d83c;
    }
  }
  lv_label_set_text(DAT_012171ec,&DAT_0044f178);
  DAT_01216fb8 = 0;
  DAT_01216fbc = 0;
  DAT_01216fc0 = 0;
  DAT_01216fc8 = 0;
  DAT_01216fcc = 0;
  DAT_01216fd0 = (undefined *)0x0;
  DAT_01216fe0 = 0;
  FUN_0040c19c(1);
joined_r0x0040d83c:
  if (DAT_011e9b90 == 0) {
    printf("%s(), line: %d. ebook_read_file_task restart!\n","ebook_read_file_task",0x64c);
  }
  else {
    pthread_attr_init((pthread_attr_t *)(local_46e + 0x402));
    pthread_attr_setstacksize((pthread_attr_t *)(local_46e + 0x402),0x5000);
    iVar9 = pthread_create(&DAT_01216f68,(pthread_attr_t *)(local_46e + 0x402),FUN_0040a7a8,param_1)
    ;
    if (iVar9 == 0) {
      pthread_attr_destroy((pthread_attr_t *)(local_46e + 0x402));
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_0040e450 @ 0040e450 ===== */

void FUN_0040e450(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = lv_disp_get_default();
  lv_disp_get_scr_act(uVar1);
  if (((param_1 != 0) || (1 < DAT_0121699c - 6U)) && (DAT_012171cc != 0)) {
    lv_group_remove_all_objs();
    lv_group_del(DAT_012171cc);
    DAT_012171cc = 0;
    if (DAT_01216f70 != 0) {
      lv_timer_pause();
      lv_timer_del(DAT_01216f70);
      DAT_01216f70 = 0;
    }
    FUN_0040a544();
    iVar2 = FUN_0042291c();
    if (iVar2 != 0) {
      FUN_004227e4(0,0);
    }
    if (DAT_01216fac != 0) {
      if (DAT_01216f6c != (void *)0x0) {
        free(DAT_01216f6c);
        DAT_01216f6c = (void *)0x0;
      }
      if (DAT_01216f68 != 0) {
        DAT_011e9b90 = 0xffffffff;
        pthread_join(DAT_01216f68,(void **)0x0);
      }
      DAT_01216f68 = 0;
      DAT_011e9b90 = 0xffffffff;
      usleep(20000);
      FUN_0040b450();
    }
    lv_obj_remove_event_cb(DAT_012171ec,&LAB_0040e0e8);
    FUN_00419a48(0);
    FUN_00423bb8();
    lv_obj_clean(DAT_012171f0);
    DAT_012171ec = 0;
    DAT_012171e8 = 0;
    FUN_0041a59c(1);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0040e5c0 @ 0040e5c0 ===== */

undefined4 FUN_0040e5c0(void)

{
  if (DAT_01216f68 != 0) {
    DAT_011e9b90 = 0xffffffff;
    pthread_join(DAT_01216f68,(void **)0x0);
  }
  DAT_01216f68 = 0;
  DAT_011e9b90 = 0xffffffff;
  return 0;
}



/* ===== FUNCTION FUN_0040e614 @ 0040e614 ===== */

undefined4 FUN_0040e614(void *param_1)

{
  int iVar1;
  pthread_attr_t pStack_38;
  
  if (DAT_011e9b90 == 0) {
    printf("%s(), line: %d. ebook_read_file_task restart!\n","ebook_read_file_task",0x64c);
    return 0;
  }
  pthread_attr_init(&pStack_38);
  pthread_attr_setstacksize(&pStack_38,0x5000);
  iVar1 = pthread_create(&DAT_01216f68,&pStack_38,FUN_0040a7a8,param_1);
  if (iVar1 != 0) {
    return 0xffffffff;
  }
  pthread_attr_destroy(&pStack_38);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0040e6d8 @ 0040e6d8 ===== */

void FUN_0040e6d8(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  DAT_012171ec = lv_label_create();
  lv_obj_set_size(DAT_012171ec,0x2064,0x205d);
  lv_obj_align(DAT_012171ec,1,0,0);
  lv_label_set_long_mode(DAT_012171ec,0);
  lv_label_set_recolor(DAT_012171ec,1);
  lv_obj_set_style_text_color(DAT_012171ec,0xffffffff,0);
  uVar2 = DAT_012171ec;
  lv_obj_set_style_pad_left(DAT_012171ec,0x14,0);
  lv_obj_set_style_pad_right(uVar2,0x14,0);
  uVar2 = DAT_012171ec;
  iVar1 = FUN_00428454(0x40);
  lv_obj_set_style_text_font(uVar2,*(undefined4 *)(&UNK_0044f87c + iVar1 * 4),0);
  DAT_012171e8 = lv_label_create(param_1);
  lv_label_set_long_mode(DAT_012171e8,0);
  lv_label_set_recolor(DAT_012171e8,1);
  lv_obj_set_size(DAT_012171e8,0x201e,0x27d1);
  lv_obj_align_to(DAT_012171e8,DAT_012171ec,0xf,0xffffffec,0);
  lv_obj_set_style_text_color(DAT_012171e8,0xffffffff,0);
  iVar1 = FUN_00428454(0x41);
  lv_obj_set_style_bg_color(DAT_012171f0,*(uint *)(&DAT_0044f888 + iVar1 * 8) | 0xff000000,0);
  lv_obj_set_style_text_color(DAT_012171ec,(uint)(&PTR_DAT_0044f88c)[iVar1 * 2] | 0xff000000,0);
  lv_obj_set_style_text_font(DAT_012171e8,&DAT_0121edb0,0);
  lv_obj_set_style_text_align(DAT_012171e8,3,0);
  uVar2 = lv_btn_create(param_1);
  lv_obj_add_event_cb(uVar2,param_2,0,0);
  lv_group_focus_obj(uVar2);
  lv_obj_add_flag(uVar2,1);
  return;
}



/* ===== FUNCTION FUN_0040e8dc @ 0040e8dc ===== */

void FUN_0040e8dc(void)

{
  int iVar1;
  char *pcVar2;
  char acStack_410 [1024];
  undefined4 local_10;
  undefined4 local_c;
  
  memset(acStack_410,0,0x400);
  if (DAT_01216998 - 6U < 2) {
    if (DAT_012171cc != 0) {
      FUN_004150d0();
      return;
    }
  }
  else {
    iVar1 = lv_group_create();
    DAT_012171cc = iVar1;
    *(ushort *)(iVar1 + 0x1c) = *(ushort *)(iVar1 + 0x1c) & 0xffef | 0x10;
    FUN_004150d0(iVar1);
    FUN_0040e6d8(DAT_012171f0,&LAB_0040e0e8);
    memset(acStack_410,0,0x400);
    iVar1 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
    pcVar2 = stpcpy(acStack_410,DAT_012167ec);
    *pcVar2 = '/';
    strcpy(pcVar2 + 1,(char *)(iVar1 + 10));
    iVar1 = FUN_0040cab0(acStack_410);
    if (iVar1 < 0) {
      FUN_00436c88(0x69,2000,0,0);
      local_10 = 0x3e9;
      local_c = 0x72;
      FUN_0042c5b4(&local_10);
      return;
    }
    FUN_0040a364();
    FUN_004239d4();
    FUN_00415110(3);
  }
  return;
}



/* ===== FUNCTION FUN_0040ea30 @ 0040ea30 ===== */

undefined4 FUN_0040ea30(int param_1)

{
  short sVar1;
  short *psVar2;
  uint uVar3;
  uint uVar4;
  
  if (param_1 == 10) {
    psVar2 = (short *)FUN_0042084c();
    uVar4 = 0;
    if (psVar2 != (short *)0x0) {
      do {
        sVar1 = *psVar2;
        uVar3 = uVar4 & 0xffff;
        uVar4 = uVar4 + 1;
        psVar2 = psVar2 + 2;
        lv_chart_set_value_by_id(DAT_01216ff0,DAT_012167c0,uVar3,(int)sVar1);
      } while (uVar4 != 10);
    }
    return 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_0040ea44 @ 0040ea44 ===== */

undefined4 FUN_0040ea44(void)

{
  short sVar1;
  short *psVar2;
  uint uVar3;
  uint uVar4;
  
  psVar2 = (short *)FUN_0042084c();
  uVar4 = 0;
  if (psVar2 != (short *)0x0) {
    do {
      sVar1 = *psVar2;
      uVar3 = uVar4 & 0xffff;
      uVar4 = uVar4 + 1;
      psVar2 = psVar2 + 2;
      lv_chart_set_value_by_id(DAT_01216ff0,DAT_012167c0,uVar3,(int)sVar1);
    } while (uVar4 != 10);
  }
  return 0;
}



/* ===== FUNCTION FUN_0040eac8 @ 0040eac8 ===== */

undefined4 FUN_0040eac8(void)

{
  undefined4 uVar1;
  int iVar2;
  
  DAT_01216ff0 = lv_chart_create();
  lv_chart_set_type(DAT_01216ff0,2);
  lv_obj_set_size(DAT_01216ff0,0x2028,0x2032);
  lv_obj_align(DAT_01216ff0,7,100,0xffffff9c);
  lv_obj_set_style_radius(DAT_01216ff0,0,0);
  lv_obj_set_style_bg_color(DAT_01216ff0,0xff000000,0);
  lv_obj_set_style_bg_opa(DAT_01216ff0,0xff,0);
  lv_obj_set_style_border_color(DAT_01216ff0,0xff000000,0);
  lv_obj_set_style_border_opa(DAT_01216ff0,0xff,0);
  lv_obj_set_style_pad_row(DAT_01216ff0,0,0);
  lv_obj_set_style_pad_column(DAT_01216ff0,0,0x50000);
  uVar1 = DAT_01216ff0;
  lv_obj_set_style_pad_row(DAT_01216ff0,3,0);
  lv_obj_set_style_pad_column(uVar1,3,0);
  lv_obj_set_style_line_color(DAT_01216ff0,0xff031fff,0);
  lv_obj_set_style_line_opa(DAT_01216ff0,0xff,0);
  lv_obj_set_style_line_color(DAT_01216ff0,0xff031fff,0);
  lv_obj_set_style_line_opa(DAT_01216ff0,0xff,0);
  lv_obj_set_style_line_width(DAT_01216ff0,10,0);
  lv_chart_set_div_line_count(DAT_01216ff0,0,0);
  lv_chart_set_update_mode(DAT_01216ff0,1);
  iVar2 = 10;
  DAT_012167c0 = lv_chart_add_series(DAT_01216ff0,0xff031fff,0);
  do {
    iVar2 = iVar2 + -1;
    lv_chart_set_next_value(DAT_01216ff0,DAT_012167c0,0);
  } while (iVar2 != 0);
  return 0;
}



/* ===== FUNCTION FUN_0040ec74 @ 0040ec74 ===== */

undefined4 FUN_0040ec74(void)

{
  undefined4 local_10;
  undefined4 local_c;
  
  local_10 = 1000;
  local_c = 10;
  FUN_0042c5b4(&local_10);
  return 0;
}



/* ===== FUNCTION FUN_0040ecb0 @ 0040ecb0 ===== */

void FUN_0040ecb0(undefined4 param_1)

{
  int iVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  iVar1 = open("/dev/dis",1);
  if (-1 < iVar1) {
    local_20 = 0;
    local_24 = 2;
    local_28 = 1;
    local_1c = 3;
    local_18 = 1;
    ioctl(iVar1,0x80140e09,&local_28);
    close(iVar1);
  }
  DAT_01217018 = 0;
  DAT_0121701c = 0x4380780;
  iVar1 = FUN_0042f374();
  if (((iVar1 == 0) && (iVar1 = FUN_0042b734(), iVar1 != 0)) &&
     (iVar1 = FUN_0042b734(), iVar1 != 0xb4)) {
    DAT_01217020 = 0x22015e;
    DAT_01217024 = 0x19703d1;
  }
  else {
    DAT_01217020 = 0x155001a;
    DAT_01217024 = 0x22603cf;
  }
  FUN_004241e8(param_1,&DAT_01217018);
  return;
}



/* ===== FUNCTION FUN_0040ef20 @ 0040ef20 ===== */

undefined4 FUN_0040ef20(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = lv_label_create();
  lv_obj_set_width(uVar1,0x2064);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_set_align(uVar1,2);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_font(uVar1,SiYuanHeiTi_Light_3000_28_1b,0);
  uVar2 = lv_label_create(uVar1);
  lv_label_set_long_mode(uVar2,1);
  lv_obj_set_width(uVar2,0x2024);
  lv_obj_set_height(uVar2,0x1e);
  lv_obj_set_align(uVar2,7);
  lv_obj_set_style_text_align(uVar2,1,0);
  lv_label_set_text(uVar2,&DAT_0044f178);
  uVar2 = lv_label_create(uVar1);
  lv_label_set_long_mode(uVar2,1);
  lv_obj_set_width(uVar2,0x2040);
  lv_obj_set_height(uVar2,0x27d1);
  lv_obj_set_align(uVar2,8);
  lv_obj_set_style_text_align(uVar2,3,0);
  lv_label_set_text(uVar2,&DAT_0044f178);
  return uVar1;
}



/* ===== FUNCTION FUN_0040f060 @ 0040f060 ===== */

void FUN_0040f060(undefined4 param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  
  lv_obj_set_flex_flow(param_1,1);
  if (0 < param_2) {
    iVar2 = 1;
    do {
      FUN_0040ef20(param_1);
      bVar1 = param_2 != iVar2;
      iVar2 = iVar2 + 1;
    } while (bVar1);
  }
  return;
}



/* ===== FUNCTION FUN_0040f0b8 @ 0040f0b8 ===== */

void FUN_0040f0b8(int param_1)

{
  undefined4 uVar1;
  undefined1 auStack_138 [304];
  
  memset(auStack_138,0,0x130);
  uVar1 = FUN_0041d32c();
  FUN_0042536c(uVar1,auStack_138);
  FUN_0040fddc(param_1,*(undefined2 *)(param_1 + 0x408));
  return;
}



/* ===== FUNCTION FUN_0040f108 @ 0040f108 ===== */

undefined4 FUN_0040f108(void)

{
  return DAT_01217010;
}



/* ===== FUNCTION FUN_0040f114 @ 0040f114 ===== */

void FUN_0040f114(void)

{
  DAT_01217008 = lv_obj_create();
  lv_obj_set_size(DAT_01217008,0x2064,0x2064);
  lv_obj_align(DAT_01217008,9,0,0xffffffff);
  lv_obj_clear_flag(DAT_01217008,0x10);
  lv_obj_set_style_radius(DAT_01217008,0,0);
  lv_obj_set_style_bg_color(DAT_01217008,0xff000000,0);
  lv_obj_set_style_bg_opa(DAT_01217008,0,0);
  lv_obj_set_style_border_opa(DAT_01217008,0,0);
  return;
}



/* ===== FUNCTION FUN_0040f1a8 @ 0040f1a8 ===== */

void FUN_0040f1a8(void)

{
  int iVar1;
  
  if ((DAT_01217008 != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
    lv_obj_del(DAT_01217008);
    DAT_01217008 = 0;
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0040f200 @ 0040f200 ===== */

int FUN_0040f200(int param_1)

{
  int iVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  iVar1 = open("/dev/dis",1);
  if (-1 < iVar1) {
    local_28 = 1;
    if (param_1 == 1) {
      local_24 = 2;
      local_18 = 1;
    }
    else {
      local_24 = 1;
      local_18 = 2;
    }
    local_1c = 3;
    local_20 = 0;
    ioctl(iVar1,0x80140e09,&local_28);
    iVar1 = close(iVar1);
    return iVar1;
  }
  return -1;
}



/* ===== FUNCTION FUN_0040f3ac @ 0040f3ac ===== */

undefined4 FUN_0040f3ac(void)

{
  int iVar1;
  int iVar2;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  iVar1 = FUN_0041d32c();
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  iVar2 = open("/dev/dis",1);
  if (-1 < iVar2) {
    local_20 = 0;
    local_1c = 3;
    local_28 = 1;
    local_18 = 2;
    local_24 = 1;
    ioctl(iVar2,0x80140e09,&local_28);
    close(iVar2);
  }
  DAT_01217018 = 0;
  DAT_0121701c = 0x4380780;
  DAT_01217020 = 0;
  DAT_01217024 = 0x4380780;
  FUN_004241e8(0,&DAT_01217018);
  if ((DAT_01217008 != 0) && (iVar2 = lv_obj_is_valid(), iVar2 != 0)) {
    lv_obj_del(DAT_01217008);
    DAT_01217008 = 0;
  }
  if (iVar1 != 0) {
    DAT_01217010 = 0;
    FUN_00412278();
  }
  return 0;
}



/* ===== FUNCTION FUN_0040f4b0 @ 0040f4b0 ===== */

void FUN_0040f4b0(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  iVar1 = FUN_0041d32c();
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  iVar2 = open("/dev/dis",1);
  if (-1 < iVar2) {
    local_20 = 0;
    local_1c = 3;
    local_28 = 1;
    local_18 = 2;
    local_24 = 1;
    ioctl(iVar2,0x80140e09,&local_28);
    close(iVar2);
  }
  DAT_01217018 = 0;
  DAT_0121701c = 0x4380780;
  DAT_01217020 = 0;
  DAT_01217024 = 0x4380780;
  FUN_004241e8(0,&DAT_01217018);
  if ((DAT_01217008 != 0) && (iVar2 = lv_obj_is_valid(), iVar2 != 0)) {
    lv_obj_del(DAT_01217008);
    DAT_01217008 = 0;
  }
  if (iVar1 != 0) {
    DAT_01217010 = 0;
    FUN_00412278();
  }
  FUN_00436b1c();
  DAT_01217010 = 0;
  uVar3 = FUN_004480f8();
  uVar4 = FUN_00448100();
  uVar5 = FUN_00448108();
  uVar6 = FUN_00448110();
  FUN_0042db78(0,0,0x780,0x438,uVar3,uVar4,uVar5,uVar6);
  return;
}



/* ===== FUNCTION FUN_0040f604 @ 0040f604 ===== */

void FUN_0040f604(void)

{
  if (DAT_01217028 != 0) {
    lv_timer_del();
    DAT_01217028 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_0040f63c @ 0040f63c ===== */

void FUN_0040f63c(undefined1 param_1)

{
  DAT_0121700c = param_1;
  if (DAT_01217028 != 0) {
    lv_timer_reset(DAT_01217028);
    return;
  }
  DAT_01217028 = lv_timer_create(&LAB_0040edcc,300,0);
  return;
}



/* ===== FUNCTION FUN_0040f698 @ 0040f698 ===== */

void FUN_0040f698(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  
  iVar1 = FUN_0041d338();
  piVar2 = (int *)FUN_0040fddc(iVar1,*(undefined2 *)(iVar1 + 0x408));
  if (*piVar2 == 0) {
    if (DAT_01217008 != 0) {
      lv_obj_clean();
    }
    FUN_0041dea4(*(undefined2 *)(iVar1 + 0x408));
    DAT_0121700c = 1;
  }
  else {
    if (*(int *)(iVar1 + 0x400) == 3) {
      FUN_0041dea4(*(undefined2 *)(iVar1 + 0x408));
      if ((DAT_01217008 == 0) || (iVar1 = lv_obj_is_valid(), iVar1 == 0)) {
        DAT_01217008 = lv_obj_create(DAT_012167dc);
        lv_obj_set_size(DAT_01217008,0x2064,0x2064);
        lv_obj_align(DAT_01217008,9,0,0xffffffff);
        lv_obj_clear_flag(DAT_01217008,0x10);
        lv_obj_set_style_radius(DAT_01217008,0,0);
        lv_obj_set_style_bg_color(DAT_01217008,0xff000000,0);
        lv_obj_set_style_bg_opa(DAT_01217008,0,0);
        lv_obj_set_style_border_opa(DAT_01217008,0,0);
      }
      else {
        lv_obj_clean(DAT_01217008);
      }
      FUN_0040bd28(DAT_01217008);
      return;
    }
    iVar3 = FUN_0041d32c();
    DAT_01217010 = 1;
    if (*(int *)(iVar1 + 0x400) == 0) {
      FUN_0042dc0c(1,4);
    }
    else {
      FUN_0042dc0c(1,5);
    }
    FUN_0040ecb0(1);
    if (iVar3 != 0) {
      FUN_00425c80(iVar3);
    }
    FUN_004122dc();
    if (*(int *)(iVar1 + 0x400) == 1) {
      return;
    }
    DAT_0121700c = 0;
  }
  if (DAT_01217028 == 0) {
    DAT_01217028 = lv_timer_create(&LAB_0040edcc,300,0);
    return;
  }
  lv_timer_reset();
  return;
}



/* ===== FUNCTION FUN_0040fb8c @ 0040fb8c ===== */

void FUN_0040fb8c(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined8 uVar4;
  
  uVar4 = CONCAT44(DAT_01217034,DAT_01217030);
  if (param_1 - 0x67U < 6) {
    if (-1 < (int)((0x2dU >> (param_1 - 0x67U & 0x1f)) << 0x1f)) {
      return;
    }
    uVar1 = lv_disp_get_default();
    iVar2 = lv_disp_get_scr_act(uVar1);
    uVar4 = CONCAT44(DAT_01217034,DAT_01217030);
    if (DAT_012167d8 == iVar2) {
      uVar4 = FUN_0042ebb8();
      uVar3 = (uint)uVar4 - DAT_01217030;
      if (((int)((ulonglong)uVar4 >> 0x20) - DAT_01217034 == (uint)((uint)uVar4 < uVar3)) &&
         (uVar3 < 0x12d)) {
        if (DAT_01217038 != 0) {
          lv_timer_reset();
          return;
        }
        DAT_01217038 = lv_timer_create(&LAB_0040f894,0x96,0);
        uVar4 = CONCAT44(DAT_01217034,DAT_01217030);
      }
      else {
        FUN_0040f698();
        uVar4 = FUN_0042ebb8();
      }
    }
  }
  DAT_01217034 = (int)((ulonglong)uVar4 >> 0x20);
  DAT_01217030 = (int)uVar4;
  return;
}



/* ===== FUNCTION FUN_0040fc90 @ 0040fc90 ===== */

int FUN_0040fc90(int param_1,int param_2)

{
  __int32_t **pp_Var1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  
  pp_Var1 = __ctype_tolower_loc();
  pcVar5 = (char *)(param_2 + 10);
  pcVar6 = (char *)(param_1 + 10);
  do {
    uVar4 = (*pp_Var1)[*pcVar6];
    uVar2 = (*pp_Var1)[*pcVar5];
    uVar3 = uVar4 & 0xff;
    if (uVar3 == 0) break;
    pcVar5 = pcVar5 + 1;
    pcVar6 = pcVar6 + 1;
  } while (uVar3 == (uVar2 & 0xff));
  return (uVar2 & 0xff) - (uVar4 & 0xff);
}



/* ===== FUNCTION FUN_0040fd10 @ 0040fd10 ===== */

int FUN_0040fd10(int param_1,int param_2)

{
  __int32_t **pp_Var1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  
  pp_Var1 = __ctype_tolower_loc();
  pcVar5 = (char *)(param_2 + 10);
  pcVar6 = (char *)(param_1 + 10);
  do {
    uVar2 = (*pp_Var1)[*pcVar6];
    uVar4 = (*pp_Var1)[*pcVar5];
    uVar3 = uVar2 & 0xff;
    if (uVar3 == 0) break;
    pcVar5 = pcVar5 + 1;
    pcVar6 = pcVar6 + 1;
  } while (uVar3 == (uVar4 & 0xff));
  return (uVar2 & 0xff) - (uVar4 & 0xff);
}



/* ===== FUNCTION FUN_0040fda8 @ 0040fda8 ===== */

void FUN_0040fda8(char *param_1,char *param_2,char *param_3)

{
  char *pcVar1;
  
  pcVar1 = stpcpy(param_1,param_2);
  *pcVar1 = '/';
  strcpy(pcVar1 + 1,param_3);
  return;
}



/* ===== FUNCTION FUN_0040fddc @ 0040fddc ===== */

int FUN_0040fddc(int param_1,int param_2)

{
  int iVar1;
  
  if ((int)((uint)*(ushort *)(param_1 + 0x404) + (uint)*(ushort *)(param_1 + 0x406)) < param_2) {
    iVar1 = -1;
  }
  else {
    iVar1 = glist_nth_data(*(undefined4 *)(param_1 + 0x40c));
    if (iVar1 == 0) {
      printf("can not find the file node via the index:%d\n",param_2);
    }
  }
  return iVar1;
}



/* ===== FUNCTION FUN_0040feb4 @ 0040feb4 ===== */

int FUN_0040feb4(int param_1,char *param_2)

{
  size_t __n;
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  
  if (param_2 != (char *)0x0) {
    iVar4 = 0;
    for (puVar2 = *(undefined4 **)(param_1 + 0x40c); puVar2 != (undefined4 *)0x0;
        puVar2 = (undefined4 *)puVar2[1]) {
      piVar3 = (int *)*puVar2;
      if (*piVar3 != 0) {
        __n = strlen(param_2);
        iVar1 = memcmp(param_2,(void *)((int)piVar3 + 10),__n);
        if (iVar1 == 0) {
          return iVar4;
        }
      }
      iVar4 = iVar4 + 1;
    }
  }
  return -1;
}



/* ===== FUNCTION FUN_0040ff40 @ 0040ff40 ===== */

undefined4 FUN_0040ff40(char *param_1,char *param_2)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  DIR *__dirp;
  dirent *pdVar4;
  size_t sVar5;
  int *piVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 *puVar9;
  char *pcVar10;
  char cVar11;
  int iVar12;
  int iVar13;
  byte *pbVar14;
  char *pcVar15;
  byte *pbVar16;
  char cVar17;
  uint uVar18;
  undefined *puVar19;
  char *__haystack;
  uint uVar20;
  undefined *puVar21;
  char local_450 [4];
  undefined4 local_44c;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_44;
  undefined **local_40;
  char *local_3c;
  undefined **local_38;
  char *local_34;
  undefined **local_30;
  char *local_2c;
  
  if (param_1 == (char *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","file_list",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/file_mgr.c"
           ,0x1a8);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __dirp = opendir(param_2);
  if (__dirp == (DIR *)0x0) {
    local_450[0] = '\0';
    local_450[1] = '\0';
    local_450[2] = '\0';
    local_450[3] = '\0';
    local_44c = 0;
    iVar13 = FUN_0042b4b4();
    local_44c = *(undefined4 *)(iVar13 + 8);
    local_450[0] = '\x0f';
    local_450[1] = '\0';
    local_450[2] = '\0';
    local_450[3] = '\0';
    FUN_0042eae0(0);
    FUN_0042c5b4(local_450);
    printf(">>!%s ,%d\n","file_mgr_create_list",0x1b3);
    return 0xffffffff;
  }
  if (*(int *)(param_1 + 0x40c) != 0) {
    glist_free_full(*(int *)(param_1 + 0x40c),&LAB_0040fd90);
    param_1[0x404] = '\0';
    param_1[0x405] = '\0';
    param_1[0x406] = '\0';
    param_1[0x407] = '\0';
    param_1[0x408] = '\0';
    param_1[0x409] = '\0';
    param_1[0x40c] = '\0';
    param_1[0x40d] = '\0';
    param_1[0x40e] = '\0';
    param_1[0x40f] = '\0';
  }
  strcpy(param_1,param_2);
  strcpy(local_450,param_2);
  local_50 = 4;
  local_4c = 0;
  local_44 = 0;
  if (DAT_01217040 != 0) {
    glist_free_full(DAT_01217040,&LAB_0040fd90);
    DAT_01217040 = 0;
  }
  local_3c = "nd bgm path";
  uVar20 = 0;
  printf("%s start\n","file_mgr_create_list");
  local_38 = &PTR_DAT_0044fa30;
  local_34 = "nd bgm path";
  local_30 = &PTR_DAT_0044fa34;
  local_2c = "nd bgm path";
  local_40 = &PTR_PTR_0044fb2c;
  do {
    while( true ) {
      do {
        pdVar4 = readdir(__dirp);
        if (pdVar4 == (dirent *)0x0) goto LAB_004101d4;
        cVar17 = pdVar4->d_name[0];
      } while ((cVar17 == '.') || (pcVar10 = pdVar4->d_name, cVar17 == '$'));
      sVar5 = strlen(pcVar10);
      if (pdVar4->d_type == '\x04') break;
      uVar18 = *(uint *)(param_1 + 0x400);
      if ((uVar18 < 4) && (cVar17 != '\0')) {
        iVar13 = -1;
        iVar12 = 0;
        pcVar15 = pdVar4->d_name + 1;
        cVar11 = cVar17;
        do {
          if (cVar11 == '.') {
            iVar13 = iVar12;
          }
          cVar11 = *pcVar15;
          iVar12 = iVar12 + 1;
          pcVar15 = pcVar15 + 1;
        } while (cVar11 != '\0');
        if (iVar13 != -1) {
          puVar21 = (local_40 + uVar18 * 3)[1];
          pbVar16 = local_40[uVar18 * 3];
          if (0 < (int)puVar21) {
            puVar19 = (undefined *)0x0;
            do {
              bVar1 = *pbVar16;
              pcVar15 = pcVar10 + iVar13 + 1;
              pbVar14 = pbVar16;
              if (pcVar10[iVar13 + 1] == 0) {
                if (bVar1 == 0) {
LAB_00410160:
                  puVar21 = local_40[uVar18 * 3 + 2];
                  if (puVar21 != (undefined *)0x5) {
                    piVar6 = malloc(sVar5 + 0xd);
                    sVar3 = *(short *)(param_1 + 0x406);
                    *piVar6 = (int)puVar21;
                    piVar6[1] = 0;
                    piVar6[2] = 0;
                    *(short *)(param_1 + 0x406) = sVar3 + 1;
                    strcpy((char *)((int)piVar6 + 10),pcVar10);
                    uVar7 = glist_prepend(*(undefined4 *)(param_1 + 0x40c),piVar6);
                    *(undefined4 *)(param_1 + 0x40c) = uVar7;
                    goto LAB_004101bc;
                  }
                  break;
                }
              }
              else {
                do {
                  while( true ) {
                    if (bVar1 == 0) goto LAB_0041031c;
                    bVar2 = *pcVar15;
                    pcVar15 = pcVar15 + 1;
                    bVar1 = *pbVar14;
                    pbVar14 = pbVar14 + 1;
                    if (bVar2 - 0x41 < 0x1a) break;
                    if (bVar2 != bVar1) goto LAB_0041031c;
                    if (bVar1 == 0) goto LAB_00410160;
                  }
                } while (bVar1 == (byte)(bVar2 + 0x20));
              }
LAB_0041031c:
              puVar19 = puVar19 + 1;
              pbVar16 = pbVar16 + 0xc;
            } while (puVar21 != puVar19);
          }
        }
        iVar13 = -1;
        if (uVar18 < 2) {
          iVar12 = 0;
          pcVar15 = pdVar4->d_name + 1;
          do {
            if (cVar17 == '.') {
              iVar13 = iVar12;
            }
            cVar17 = *pcVar15;
            iVar12 = iVar12 + 1;
            pcVar15 = pcVar15 + 1;
          } while (cVar17 != '\0');
          if (iVar13 != -1) {
            __haystack = pcVar10 + iVar13 + 1;
            pcVar15 = strcasestr(__haystack,(char *)local_38);
            if (((((pcVar15 != (char *)0x0) ||
                  (pcVar15 = strcasestr(__haystack,(char *)local_30), pcVar15 != (char *)0x0)) ||
                 (pcVar15 = strcasestr(__haystack,local_2c + -0x5c8), pcVar15 != (char *)0x0)) ||
                ((((pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa3c),
                   pcVar15 != (char *)0x0 ||
                   (pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa40),
                   pcVar15 != (char *)0x0)) ||
                  ((pcVar15 = strcasestr(__haystack,"sami"), pcVar15 != (char *)0x0 ||
                   ((pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa4c),
                    pcVar15 != (char *)0x0 ||
                    (pcVar15 = strcasestr(__haystack,"mpl2"), pcVar15 != (char *)0x0)))))) ||
                 (pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa58), pcVar15 != (char *)0x0
                 )))) || (((pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa5c),
                           pcVar15 != (char *)0x0 ||
                           (pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa60),
                           pcVar15 != (char *)0x0)) ||
                          (pcVar15 = strcasestr(__haystack,(char *)&PTR_DAT_0044fa64),
                          pcVar15 != (char *)0x0)))) {
              uVar7 = __strdup(pcVar10);
              DAT_01217040 = glist_append(DAT_01217040,uVar7);
              printf(local_34 + -0x56c,pcVar10);
            }
          }
        }
      }
    }
    uVar18 = uVar20 + 1;
    puVar9 = malloc(sVar5 + 0xd);
    sVar3 = *(short *)(param_1 + 0x404);
    *puVar9 = 0;
    uVar20 = uVar18 & 0xffff;
    puVar9[1] = 0;
    puVar9[2] = 0;
    *(short *)(param_1 + 0x404) = sVar3 + 1;
    strcpy((char *)((int)puVar9 + 10),pcVar10);
    local_4c = CONCAT22(local_4c._2_2_,(short)uVar18);
    sVar5 = strlen(local_450);
    iVar13 = memcmp(local_450,"/media",sVar5);
    if ((iVar13 == 0) &&
       (pcVar10 = strstr((char *)((int)puVar9 + 10),(char *)&PTR_DAT_0044fa2c),
       pcVar10 == (char *)0x0)) {
      local_44 = glist_prepend(local_44,puVar9);
    }
    else {
      local_44 = glist_append(local_44,puVar9);
    }
LAB_004101bc:
  } while ((uint)*(ushort *)(param_1 + 0x404) + (uint)*(ushort *)(param_1 + 0x406) != 1000);
LAB_004101d4:
  printf("%s end\n",local_3c + -0x4ec);
  uVar7 = glist_sort(*(undefined4 *)(param_1 + 0x40c),FUN_0040fd10,0);
  *(undefined4 *)(param_1 + 0x40c) = uVar7;
  sVar5 = strlen(local_450);
  iVar13 = memcmp(local_450,"/media",sVar5);
  if (iVar13 != 0) {
    local_44 = glist_sort(local_44,FUN_0040fc90,0);
  }
  uVar7 = local_44;
  if (uVar20 != 0) {
    uVar18 = 0;
    do {
      uVar8 = glist_nth_data(uVar7,uVar18);
      uVar18 = uVar18 + 1;
      uVar8 = glist_prepend(*(undefined4 *)(param_1 + 0x40c),uVar8);
      *(undefined4 *)(param_1 + 0x40c) = uVar8;
    } while (uVar18 != uVar20);
  }
  sVar5 = strlen(param_2);
  iVar13 = memcmp(param_2,"/media",sVar5);
  if (iVar13 != 0) {
    puVar9 = malloc(0x10);
    sVar3 = *(short *)(param_1 + 0x404);
    uVar7 = *(undefined4 *)(param_1 + 0x40c);
    puVar9[2] = 0;
    *puVar9 = 0;
    puVar9[1] = 0;
    *(short *)(param_1 + 0x404) = sVar3 + 1;
    *(undefined1 *)(puVar9 + 3) = 0;
    *(undefined2 *)((int)puVar9 + 10) = 0x2e2e;
    uVar7 = glist_prepend(uVar7,puVar9);
    *(undefined4 *)(param_1 + 0x40c) = uVar7;
  }
  closedir(__dirp);
  printf("current dir: %s, file: %d, dir: %d\n",param_2,(uint)*(ushort *)(param_1 + 0x406),
         (uint)*(ushort *)(param_1 + 0x404));
  return 0;
}



/* ===== FUNCTION FUN_004105cc @ 004105cc ===== */

undefined4 FUN_004105cc(char *param_1,char *param_2)

{
  byte bVar1;
  byte bVar2;
  short sVar3;
  DIR *__dirp;
  dirent *pdVar4;
  size_t sVar5;
  int *piVar6;
  undefined4 uVar7;
  char cVar8;
  int iVar9;
  int iVar10;
  byte *pbVar11;
  char *pcVar12;
  uint uVar13;
  byte *pbVar14;
  char *__s;
  undefined4 local_38;
  undefined4 local_34;
  char *local_30;
  int local_2c;
  
  if (param_1 == (char *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","file_list",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/file_mgr.c"
           ,0x227);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __dirp = opendir(param_2);
  if (__dirp == (DIR *)0x0) {
    local_38 = 0;
    local_34 = 0;
    iVar9 = FUN_0042b4b4();
    local_34 = *(undefined4 *)(iVar9 + 8);
    local_38 = 0xf;
    FUN_0042eae0(0);
    FUN_0042c5b4(&local_38);
    printf(">>!%s ,%d\n","file_mgr_create_list_without_dir",0x232);
    return 0xffffffff;
  }
  if (*(int *)(param_1 + 0x40c) != 0) {
    glist_free_full(*(int *)(param_1 + 0x40c),&LAB_0040fd90);
    param_1[0x404] = '\0';
    param_1[0x405] = '\0';
    param_1[0x406] = '\0';
    param_1[0x407] = '\0';
    param_1[0x40c] = '\0';
    param_1[0x40d] = '\0';
    param_1[0x40e] = '\0';
    param_1[0x40f] = '\0';
  }
  strcpy(param_1,param_2);
  local_30 = "nd bgm path";
  printf("%s start\n","file_mgr_create_list_without_dir");
  do {
    do {
      do {
        do {
          do {
            pdVar4 = readdir(__dirp);
            if (pdVar4 == (dirent *)0x0) {
LAB_004107d0:
              printf("%s end\n",local_30 + -0x510);
              uVar7 = glist_sort(*(undefined4 *)(param_1 + 0x40c),FUN_0040fd10,0);
              *(undefined4 *)(param_1 + 0x40c) = uVar7;
              closedir(__dirp);
              printf("current dir: %s, file: %d, dir: %d\n",param_2,
                     (uint)*(ushort *)(param_1 + 0x406),(uint)*(ushort *)(param_1 + 0x404));
              return 0;
            }
            cVar8 = pdVar4->d_name[0];
          } while ((cVar8 == '.') &&
                  ((pdVar4->d_name[1] == '\0' ||
                   ((pdVar4->d_name[1] == '.' && (pdVar4->d_name[2] == '\0'))))));
          __s = pdVar4->d_name;
          sVar5 = strlen(__s);
        } while ((pdVar4->d_type == '\x04') ||
                ((uVar13 = *(uint *)(param_1 + 0x400), 3 < uVar13 || (iVar9 = -1, cVar8 == '\0'))));
        pcVar12 = pdVar4->d_name;
        iVar10 = 0;
        do {
          pcVar12 = pcVar12 + 1;
          if (cVar8 == '.') {
            iVar9 = iVar10;
          }
          cVar8 = *pcVar12;
          iVar10 = iVar10 + 1;
        } while (cVar8 != '\0');
      } while (iVar9 == -1);
      pbVar14 = (&PTR_PTR_0044fb2c)[uVar13 * 3];
    } while (*(int *)(&DAT_0044fb30 + uVar13 * 0xc) < 1);
    iVar10 = 0;
    do {
      bVar1 = *pbVar14;
      pcVar12 = __s + iVar9 + 1;
      pbVar11 = pbVar14;
      if (__s[iVar9 + 1] == 0) {
        if (bVar1 == 0) {
LAB_00410760:
          if (*(int *)(&DAT_0044fb34 + uVar13 * 0xc) != 5) {
            local_2c = *(int *)(&DAT_0044fb34 + uVar13 * 0xc);
            piVar6 = malloc(sVar5 + 0xd);
            sVar3 = *(short *)(param_1 + 0x406);
            piVar6[1] = 0;
            piVar6[2] = 0;
            *piVar6 = local_2c;
            *(short *)(param_1 + 0x406) = sVar3 + 1;
            strcpy((char *)((int)piVar6 + 10),__s);
            uVar7 = glist_prepend(*(undefined4 *)(param_1 + 0x40c),piVar6);
            *(undefined4 *)(param_1 + 0x40c) = uVar7;
            if ((uint)*(ushort *)(param_1 + 0x404) + (uint)*(ushort *)(param_1 + 0x406) == 1000)
            goto LAB_004107d0;
          }
          break;
        }
      }
      else {
        do {
          while( true ) {
            if (bVar1 == 0) goto LAB_00410858;
            bVar2 = *pcVar12;
            pcVar12 = pcVar12 + 1;
            bVar1 = *pbVar11;
            pbVar11 = pbVar11 + 1;
            if (bVar2 - 0x41 < 0x1a) break;
            if (bVar2 != bVar1) goto LAB_00410858;
            if (bVar1 == 0) goto LAB_00410760;
          }
        } while (bVar1 == (byte)(bVar2 + 0x20));
      }
LAB_00410858:
      iVar10 = iVar10 + 1;
      pbVar14 = pbVar14 + 0xc;
    } while (*(int *)(&DAT_0044fb30 + uVar13 * 0xc) != iVar10);
  } while( true );
}



/* ===== FUNCTION FUN_00410934 @ 00410934 ===== */

undefined4 FUN_00410934(void *param_1)

{
  if ((param_1 != (void *)0x0) && (*(int *)((int)param_1 + 0x40c) != 0)) {
    glist_free_full(*(int *)((int)param_1 + 0x40c),&LAB_0040fd90);
    memset(param_1,0,0x410);
    puts("free media_list struct  memory");
    return 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_004109e0 @ 004109e0 ===== */

void FUN_004109e0(void)

{
  if (DAT_01217040 != 0) {
    glist_free_full(DAT_01217040,&LAB_0040fd90);
  }
  DAT_01217040 = 0;
  return;
}



/* ===== FUNCTION FUN_00410a1c @ 00410a1c ===== */

undefined4 FUN_00410a1c(char *param_1,char *param_2)

{
  size_t sVar1;
  size_t __n;
  char *pcVar2;
  
  sVar1 = strlen(param_2);
  if ((int)sVar1 < 1) {
    return 0;
  }
  if (*param_2 == '.') {
    __n = 0;
  }
  else {
    __n = 0;
    pcVar2 = param_2;
    do {
      pcVar2 = pcVar2 + 1;
      __n = __n + 1;
      if (sVar1 == __n) {
        return 0;
      }
    } while (*pcVar2 != '.');
  }
  strncpy(param_1,param_2,__n);
  return 0;
}



/* ===== FUNCTION FUN_00410b30 @ 00410b30 ===== */

void FUN_00410b30(void)

{
  lv_obj_add_flag(DAT_01217054,1);
  if (DAT_01217050 != 0) {
    lv_timer_del();
    DAT_01217050 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_00410b78 @ 00410b78 ===== */

void FUN_00410b78(uint param_1)

{
  if (2 < param_1) {
    param_1 = 2;
  }
  DAT_011ea110 = param_1 & 0xff;
  if (DAT_011ea110 != 2) {
    FUN_004130d0(7);
    lv_obj_add_flag(DAT_01217058,1);
    return;
  }
  lv_obj_clear_flag(DAT_01217058,1);
  return;
}



/* ===== FUNCTION FUN_00410bd0 @ 00410bd0 ===== */

undefined1 FUN_00410bd0(void)

{
  return (undefined1)DAT_011ea110;
}



/* ===== FUNCTION FUN_00410bdc @ 00410bdc ===== */

void FUN_00410bdc(void)

{
  int iVar1;
  
  iVar1 = FUN_00413c60();
  iVar1 = *(int *)(iVar1 + 4);
  if (iVar1 == 1) {
    lv_label_set_text(DAT_01217054,"zoom ratio: 1 : 1.5");
  }
  else if (iVar1 < 2) {
    if (iVar1 == 0) {
      lv_label_set_text(DAT_01217054,"zoom ratio: 1 : 1");
    }
  }
  else {
    if (iVar1 == 2) {
      lv_label_set_text(DAT_01217054,"zoom ratio: 1 : 2");
      lv_obj_clear_flag(DAT_01217054,1);
      goto joined_r0x00410c7c;
    }
    if (iVar1 == 3) {
      lv_label_set_text(DAT_01217054,"zoom ratio: 1 : 3");
    }
  }
  lv_obj_clear_flag(DAT_01217054,1);
joined_r0x00410c7c:
  if (DAT_01217050 == 0) {
    DAT_01217050 = lv_timer_create(FUN_00410b30,3000,0);
    return;
  }
  lv_timer_reset();
  return;
}



/* ===== FUNCTION FUN_00410ce8 @ 00410ce8 ===== */

void FUN_00410ce8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  if ((char)DAT_011ea110 == '\0') {
    uVar1 = FUN_0041d32c();
    uVar2 = FUN_0040a6ec();
    iVar3 = FUN_00420174(uVar2);
    if (iVar3 != 0) {
      FUN_00425c80(uVar1);
      FUN_00424408(uVar1,iVar3);
      FUN_004247bc(uVar1);
      return;
    }
    return;
  }
  if ((char)DAT_011ea110 != '\x01') {
    return;
  }
  FUN_00413ff0(0x11);
  return;
}



/* ===== FUNCTION FUN_00410d08 @ 00410d08 ===== */

void FUN_00410d08(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_00420174(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    FUN_004247bc(uVar1);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_00410d7c @ 00410d7c ===== */

void FUN_00410d7c(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  if ((char)DAT_011ea110 == '\0') {
    uVar1 = FUN_0041d32c();
    uVar2 = FUN_0040a6ec();
    iVar3 = FUN_0042036c(uVar2);
    if (iVar3 != 0) {
      FUN_00425c80(uVar1);
      FUN_00424408(uVar1,iVar3);
      FUN_004247bc(uVar1);
      return;
    }
    return;
  }
  if ((char)DAT_011ea110 != '\x01') {
    return;
  }
  FUN_00413ff0(0x12);
  return;
}



/* ===== FUNCTION FUN_00410d9c @ 00410d9c ===== */

void FUN_00410d9c(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_0042036c(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    FUN_004247bc(uVar1);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_00410e10 @ 00410e10 ===== */

void FUN_00410e10(void)

{
  if ((char)DAT_011ea110 != '\x01') {
    return;
  }
  FUN_00413ff0(0x14);
  return;
}



/* ===== FUNCTION FUN_00410e34 @ 00410e34 ===== */

void FUN_00410e34(void)

{
  if ((char)DAT_011ea110 != '\x01') {
    return;
  }
  FUN_00413ff0(0x13);
  return;
}



/* ===== FUNCTION FUN_00410e58 @ 00410e58 ===== */

void FUN_00410e58(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 != 2) {
    FUN_004247bc(uVar1);
    DAT_011ea110 = 0;
    FUN_004130d0(7);
    lv_obj_add_flag(DAT_01217058,1);
    return;
  }
  FUN_00424888(uVar1);
  DAT_011ea110 = iVar2;
  lv_obj_clear_flag(DAT_01217058,1);
  return;
}



/* ===== FUNCTION FUN_00410eec @ 00410eec ===== */

void FUN_00410eec(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 == 1) {
    FUN_004247bc(uVar1);
    DAT_011ea110 = 0;
    FUN_004130d0(7);
    lv_obj_add_flag(DAT_01217058,1);
  }
  FUN_004130d0(7);
  FUN_0042668c(uVar1,0);
  return;
}



/* ===== FUNCTION FUN_00410f68 @ 00410f68 ===== */

void FUN_00410f68(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  if ((char)DAT_011ea110 != '\x02') {
    iVar1 = FUN_0041d32c();
    iVar2 = FUN_0042527c(iVar1);
    if (iVar2 == 1) {
      FUN_004247bc(iVar1);
      DAT_011ea110 = 0;
      FUN_004130d0(7);
      lv_obj_add_flag(DAT_01217058,1);
      iVar2 = *(int *)(iVar1 + 4);
    }
    else {
      iVar2 = *(int *)(iVar1 + 4);
    }
    if (iVar2 != 9) {
      FUN_00425b88(iVar1,1);
      piVar3 = (int *)FUN_00413c60();
      if (0 < *piVar3) {
        DAT_011ea110 = 1;
        FUN_004130d0(7);
        lv_obj_add_flag(DAT_01217058,1);
        FUN_00413f80(0);
        FUN_00410bdc();
        return;
      }
      DAT_011ea110 = 0;
      FUN_004130d0(7);
      lv_obj_add_flag(DAT_01217058,1);
      FUN_00410bdc();
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0041107c @ 0041107c ===== */

void FUN_0041107c(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  iVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(iVar1);
  if (iVar2 == 1) {
    FUN_004247bc(iVar1);
    DAT_011ea110 = 0;
    FUN_004130d0(7);
    lv_obj_add_flag(DAT_01217058,1);
    iVar2 = *(int *)(iVar1 + 4);
  }
  else {
    iVar2 = *(int *)(iVar1 + 4);
  }
  if (iVar2 != 9) {
    FUN_00425b88(iVar1,0);
    piVar3 = (int *)FUN_00413c60();
    if (0 < *piVar3) {
      DAT_011ea110 = 1;
      FUN_004130d0(7);
      lv_obj_add_flag(DAT_01217058,1);
      FUN_00413f80(0);
      FUN_00410bdc();
      return;
    }
    DAT_011ea110 = 0;
    FUN_004130d0(7);
    lv_obj_add_flag(DAT_01217058,1);
    FUN_00410bdc();
    return;
  }
  return;
}



/* ===== FUNCTION FUN_00411178 @ 00411178 ===== */

void FUN_00411178(void)

{
  if (DAT_01217050 != 0) {
    lv_timer_del();
    DAT_01217050 = 0;
  }
  FUN_0041a59c(1);
  return;
}



/* ===== FUNCTION FUN_004111b4 @ 004111b4 ===== */

undefined4 FUN_004111b4(undefined4 param_1)

{
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  DAT_01217058 = lv_obj_create(param_1);
  lv_obj_set_style_bg_opa(DAT_01217058,0,0);
  lv_obj_align(DAT_01217058,9,0xfffffee3,0xffffff29);
  lv_obj_set_style_bg_img_src(DAT_01217058,&DAT_008254e0,0);
  lv_obj_set_size(DAT_01217058,0x27d1,0x27d1);
  lv_obj_set_style_border_width(DAT_01217058,0,0);
  DAT_01217054 = lv_label_create(param_1);
  lv_label_set_text(DAT_01217054,&DAT_0044f178);
  lv_obj_set_style_text_color(DAT_01217054,0xffffffff,0);
  lv_obj_set_style_text_font(DAT_01217054,&DAT_0121edb0,0);
  lv_obj_set_size(DAT_01217054,0x27d1,0x27d1);
  lv_obj_set_style_text_align(DAT_01217054,9,0);
  lv_obj_align(DAT_01217054,6,0xffffffec,0xfffffff6);
  lv_obj_add_flag(DAT_01217054,1);
  DAT_011ea110 = 2;
  lv_obj_clear_flag(DAT_01217058,1);
  FUN_004130d0(7);
  return 0;
}



/* ===== FUNCTION FUN_00411300 @ 00411300 ===== */

void FUN_00411300(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  iVar1 = lv_event_get_code();
  lv_event_get_target(param_1);
  if (iVar1 != 0xd) {
    return;
  }
  uVar2 = lv_indev_get_act();
  uVar2 = lv_indev_get_key(uVar2);
  uVar3 = FUN_00429cd8(uVar2);
  switch(uVar2) {
  case 10:
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      iVar1 = FUN_00415e34();
      if (iVar1 != 0) {
        FUN_004159c0();
        FUN_00415eac();
        goto LAB_004113d8;
      }
LAB_004115b8:
      FUN_00415e70();
      goto LAB_004113d8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 == 0) {
        FUN_00420b40();
        FUN_00420f80();
      }
      else {
        FUN_00420ad8();
        FUN_00421414();
      }
      goto LAB_004113d8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410e58();
      goto LAB_004113d8;
    }
    break;
  case 0x11:
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      FUN_004158d0();
      goto LAB_004113d8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 == 0) {
        FUN_00420b40();
        FUN_00420dd8();
        goto LAB_004113d8;
      }
      goto LAB_00411750;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410ce8();
      goto LAB_004113d8;
    }
    break;
  case 0x12:
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      FUN_004157e0();
      goto LAB_004113d8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 == 0) {
        FUN_00420b40();
        FUN_00420eac();
        goto LAB_004113d8;
      }
      goto LAB_00411750;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410d7c();
      goto LAB_004113d8;
    }
    break;
  case 0x13:
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      iVar1 = FUN_00415e34();
      if (iVar1 != 0) {
        FUN_00415bc8();
        FUN_00415eac();
        goto LAB_004113d8;
      }
      goto LAB_004115b8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 == 0) {
        FUN_00420b40();
        FUN_00421194();
        goto LAB_004113d8;
      }
      goto LAB_00411750;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410e34();
      goto LAB_004113d8;
    }
    break;
  case 0x14:
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      iVar1 = FUN_00415e34();
      if (iVar1 != 0) {
        FUN_00415a98();
        FUN_00415eac();
        goto LAB_004113d8;
      }
      goto LAB_004115b8;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 == 0) {
        FUN_00420b40();
        FUN_00421044();
        goto LAB_004113d8;
      }
      goto LAB_00411750;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410e10();
      goto LAB_004113d8;
    }
    break;
  case 0x1b:
    iVar1 = FUN_004155dc();
    if ((iVar1 == 1) && (iVar1 = FUN_00420c08(), iVar1 != 0)) {
LAB_00411750:
      FUN_00420ad8();
      goto LAB_004113d8;
    }
    iVar1 = FUN_004155dc();
    if ((iVar1 == 0) && (iVar1 = FUN_00415e34(), iVar1 == 0)) goto LAB_004115b8;
    FUN_00406818(DAT_012167d8,0,0);
  }
LAB_004113d8:
  if (uVar3 == 7) {
    iVar1 = FUN_004155dc();
    if (iVar1 == 0) {
      FUN_00415db4();
      return;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 1) {
      iVar1 = FUN_00420c08();
      if (iVar1 != 0) {
        FUN_00420ad8();
        FUN_00421414();
        return;
      }
      FUN_00420c14();
      return;
    }
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410eec();
      return;
    }
  }
  else if (uVar3 < 8) {
    if (uVar3 == 6) {
      iVar1 = FUN_004155dc();
      if (iVar1 == 0) {
        iVar1 = FUN_00415e34();
        if (iVar1 == 0) {
          FUN_00415e70();
          return;
        }
        FUN_00415cf8();
        FUN_00415eac();
        return;
      }
      iVar1 = FUN_004155dc();
      if (iVar1 == 1) {
        iVar1 = FUN_00420c08();
        if (iVar1 != 0) {
          FUN_00420ad8();
          return;
        }
        FUN_00420b40();
        FUN_004212e0();
        return;
      }
      iVar1 = FUN_004155dc();
      if (iVar1 == 2) {
        FUN_00419ae8(DAT_0121720c);
        return;
      }
    }
  }
  else if (uVar3 == 8) {
    iVar1 = FUN_004155dc();
    if (iVar1 == 2) {
      FUN_00410f68();
      return;
    }
  }
  else if ((uVar3 == 9) && (iVar1 = FUN_004155dc(), iVar1 == 2)) {
    FUN_0041107c();
    return;
  }
  return;
}



/* ===== FUNCTION FUN_004118b8 @ 004118b8 ===== */

void FUN_004118b8(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  
  if (param_1 - 5U < 2) {
    if (DAT_012167e4[0xe] == 0) {
      uVar1 = FUN_0040a6ec();
      piVar2 = (int *)FUN_0042036c(uVar1);
      if (piVar2 == (int *)0x0) {
        iVar3 = *DAT_012167e4;
        goto LAB_00411920;
      }
    }
    else {
      piVar2 = DAT_012167e4 + 0x10;
      if (DAT_012167e4[0xe] != 3) goto LAB_0041191c;
    }
LAB_00411904:
    FUN_00425c80(DAT_012167e4);
    FUN_00424408(DAT_012167e4,piVar2);
  }
  else {
    uVar1 = FUN_0040a6ec();
    piVar2 = (int *)FUN_0042036c(uVar1);
    if (piVar2 != (int *)0x0) {
      if (*DAT_012167e4 == 2) {
        FUN_00410b78(2);
      }
      goto LAB_00411904;
    }
  }
LAB_0041191c:
  iVar3 = *DAT_012167e4;
LAB_00411920:
  if (iVar3 == 0) {
    FUN_0041579c();
    return;
  }
  if (iVar3 != 1) {
    return;
  }
  FUN_00420d90();
  return;
}



/* ===== FUNCTION FUN_004119c8 @ 004119c8 ===== */

void FUN_004119c8(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_410 [1024];
  
  if ((DAT_012167ec != 0) && (*(short *)(DAT_012167ec + 0x406) != 0)) {
    uVar1 = FUN_0040a6ec();
    uVar2 = FUN_0040a6f8();
    iVar3 = FUN_0040fddc(uVar1,uVar2);
    FUN_0040fda8(auStack_410,DAT_012167ec,iVar3 + 10);
    FUN_00428f34(auStack_410,*DAT_012167e4,param_1);
  }
  FUN_00416760(0);
  return;
}



/* ===== FUNCTION FUN_00411a5c @ 00411a5c ===== */

void FUN_00411a5c(undefined4 *param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 auStack_418 [261];
  
  if (param_3 == 2) {
    return;
  }
  puVar6 = param_1 + 0x104;
  puVar1 = auStack_418;
  do {
    uVar5 = *param_1;
    uVar4 = param_1[1];
    uVar3 = param_1[2];
    uVar2 = param_1[3];
    param_1 = param_1 + 4;
    *puVar1 = uVar5;
    puVar1[1] = uVar4;
    puVar1[2] = uVar3;
    puVar1[3] = uVar2;
    puVar1 = puVar1 + 4;
  } while (param_1 != puVar6);
  FUN_00428c70(auStack_418);
  return;
}



/* ===== FUNCTION FUN_00411a70 @ 00411a70 ===== */

void FUN_00411a70(undefined4 *param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 local_418 [261];
  
  puVar6 = param_1 + 0x104;
  puVar1 = local_418;
  do {
    uVar5 = *param_1;
    uVar4 = param_1[1];
    uVar3 = param_1[2];
    uVar2 = param_1[3];
    param_1 = param_1 + 4;
    *puVar1 = uVar5;
    puVar1[1] = uVar4;
    puVar1[2] = uVar3;
    puVar1[3] = uVar2;
    puVar1 = puVar1 + 4;
  } while (param_1 != puVar6);
  FUN_00428c70(local_418);
  return;
}



/* ===== FUNCTION FUN_00411ad0 @ 00411ad0 ===== */

undefined2 FUN_00411ad0(void)

{
  return *(undefined2 *)((int)&PTR_DAT_0044fcd0 + (uint)DAT_01217068 * 2);
}



/* ===== FUNCTION FUN_00411af4 @ 00411af4 ===== */

void FUN_00411af4(uint param_1,char *param_2)

{
  if (param_1 == 0) {
    builtin_strncpy(param_2,"00:00:00",9);
    return;
  }
  if (param_1 / 0xe10 == 0) {
    sprintf(param_2,"%02lu:%02lu");
    return;
  }
  sprintf(param_2,"%02lu:%02lu:%02lu",param_1 / 0xe10,(param_1 % 0xe10) / 0x3c,param_1 % 0x3c);
  return;
}



/* ===== FUNCTION FUN_00411bbc @ 00411bbc ===== */

void FUN_00411bbc(void)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 local_38;
  undefined4 local_34;
  undefined1 local_30;
  undefined4 local_28;
  undefined4 local_24;
  
  local_28 = 0;
  local_24 = 0;
  uVar1 = FUN_004250c0(DAT_012167e4);
  if (uVar1 == 0) {
    local_38 = 0x303a3030;
    local_34 = 0x30303a30;
    local_30 = 0;
  }
  else if (uVar1 / 0xe10 == 0) {
    sprintf((char *)&local_38,"%02lu:%02lu");
  }
  else {
    sprintf((char *)&local_38,"%02lu:%02lu:%02lu",uVar1 / 0xe10,(uVar1 % 0xe10) / 0x3c,uVar1 % 0x3c)
    ;
  }
  uVar2 = lv_obj_get_child(DAT_01217208,2);
  lv_label_set_text(uVar2,&local_38);
  lv_bar_set_value(DAT_01217204,uVar1,1);
  uVar3 = FUN_004251bc(DAT_012167e4);
  if (uVar3 == 0) {
    local_38 = 0x303a3030;
    local_34 = 0x30303a30;
    local_30 = 0;
    uVar2 = lv_obj_get_child(DAT_01217208,4);
    lv_label_set_text(uVar2,&local_38);
    if (uVar1 - 2 == (uVar1 / 0x1e) * 0x1e) goto LAB_00411e68;
  }
  else {
    if (uVar3 / 0xe10 == 0) {
      sprintf((char *)&local_38,"%02lu:%02lu");
    }
    else {
      sprintf((char *)&local_38,"%02lu:%02lu:%02lu",uVar3 / 0xe10,(uVar3 % 0xe10) / 0x3c,
              uVar3 % 0x3c);
    }
    uVar2 = lv_obj_get_child(DAT_01217208,4);
    lv_label_set_text(uVar2,&local_38);
    lv_bar_set_range(DAT_01217204,0,uVar3);
    if (uVar1 - 2 == (uVar1 / 0x1e) * 0x1e) {
LAB_00411e68:
      local_28 = 0x3e9;
      local_24 = 0x6b;
      FUN_0042c5b4(&local_28);
      iVar4 = *DAT_012167e4;
      goto joined_r0x00411e88;
    }
  }
  iVar4 = *DAT_012167e4;
joined_r0x00411e88:
  if (((iVar4 == 1) && (FUN_004259e8(), DAT_012167e4[1] == 3)) && (DAT_012167e4[0xf] == 0)) {
    FUN_00424888();
    FUN_00437890(DAT_012171f8,0x54,1);
    FUN_00437890(DAT_012171f4,0,1);
  }
  return;
}



/* ===== FUNCTION FUN_00411f4c @ 00411f4c ===== */

void FUN_00411f4c(void)

{
  lv_obj_add_flag(DAT_01217208,1);
  DAT_011ea18c = 0;
  return;
}



/* ===== FUNCTION FUN_00411f78 @ 00411f78 ===== */

void FUN_00411f78(void)

{
  int iVar1;
  undefined4 uVar2;
  char *pcVar3;
  char *pcVar4;
  uint uVar5;
  undefined1 auStack_828 [4];
  undefined4 local_824;
  char acStack_81c [1024];
  char acStack_41c [1028];
  
  memset(acStack_41c,0,0x400);
  FUN_0042de94(&LAB_0041d30c);
  iVar1 = lv_group_create();
  DAT_012171d0 = iVar1;
  *(ushort *)(iVar1 + 0x1c) = *(ushort *)(iVar1 + 0x1c) & 0xffef | 0x10;
  FUN_004150d0(iVar1);
  iVar1 = *(int *)(DAT_012167ec + 0x400);
  if (iVar1 == 0) {
    FUN_00415eb8(DAT_0121720c);
  }
  else if (iVar1 == 1) {
    FUN_00421668(DAT_0121720c);
  }
  else if (iVar1 == 2) {
    FUN_004111b4(DAT_0121720c);
    FUN_004239d4();
  }
  iVar1 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
  DAT_012167e4 = *(int **)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4);
  if (DAT_012167e4 == (int *)0x0) {
    DAT_012167e4 = (int *)FUN_0042422c();
    *(int **)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4) = DAT_012167e4;
  }
  uVar2 = lv_btn_create(DAT_0121720c);
  lv_obj_add_event_cb(uVar2,FUN_00411300,0,0);
  lv_group_focus_obj(uVar2);
  lv_obj_add_flag(uVar2,1);
  memset(acStack_41c,0,0x400);
  FUN_0040fda8(acStack_41c,DAT_012167ec,iVar1 + 10);
  iVar1 = FUN_0041d178();
  if ((iVar1 == 1) &&
     (iVar1 = FUN_004165bc(DAT_012167ec,acStack_41c,auStack_828,
                           *(undefined4 *)(DAT_012167ec + 0x400)), -1 < iVar1)) {
    FUN_0041d184(0);
    strcpy(acStack_41c,acStack_81c);
    pcVar3 = strtok(acStack_81c,"/");
    pcVar4 = (char *)0x0;
    if (pcVar3 == (char *)0x0) {
      uVar2 = 0;
    }
    else {
      do {
        pcVar3 = pcVar4;
        pcVar4 = strtok((char *)0x0,"/");
      } while (pcVar4 != (char *)0x0);
      if (pcVar3 == (char *)0x0) {
        uVar2 = 0;
      }
      else {
        uVar5 = FUN_0040feb4(DAT_012167ec,pcVar3);
        iVar1 = DAT_012167ec;
        if ((int)uVar5 < 0) {
          return;
        }
        *(short *)(DAT_012167ec + 0x408) = (short)uVar5;
        iVar1 = FUN_0040fddc(iVar1,uVar5 & 0xffff);
        strcpy((char *)(iVar1 + 10),pcVar3);
        uVar2 = local_824;
      }
    }
  }
  else {
    iVar1 = FUN_0041655c(DAT_012167ec,acStack_41c,auStack_828,*(undefined4 *)(DAT_012167ec + 0x400))
    ;
    uVar2 = 0;
    if (-1 < iVar1) {
      uVar2 = local_824;
    }
  }
  FUN_0040a364();
  FUN_00424408(DAT_012167e4,acStack_41c);
  FUN_00424954(DAT_012167e4,uVar2);
  if (*DAT_012167e4 == 0) {
    DAT_01217064 = lv_timer_create(FUN_00415658,200,0);
  }
  else if (*DAT_012167e4 == 1) {
    DAT_01217064 = lv_timer_create(FUN_00420c44,200,0);
  }
  DAT_012167cc = 3;
  return;
}



/* ===== FUNCTION FUN_00412278 @ 00412278 ===== */

void FUN_00412278(void)

{
  if (DAT_012167e4 != 0) {
    FUN_00425c80();
    FUN_00424308(DAT_012167e4);
    DAT_012167e4 = 0;
    *(undefined4 *)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4) = 0;
  }
  FUN_0042e164(0);
  return;
}



/* ===== FUNCTION FUN_004122dc @ 004122dc ===== */

void FUN_004122dc(void)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_418 [1028];
  
  iVar1 = FUN_0040fddc(DAT_012167ec,*(undefined2 *)(DAT_012167ec + 0x408));
  DAT_012167e4 = *(int *)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4);
  iVar2 = DAT_012167ec;
  if (DAT_012167e4 == 0) {
    DAT_012167e4 = FUN_0042422c();
    iVar2 = DAT_012167ec;
    *(int *)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4) = DAT_012167e4;
  }
  FUN_0040fda8(auStack_418,iVar2,iVar1 + 10);
  FUN_00424408(DAT_012167e4,auStack_418);
  return;
}



/* ===== FUNCTION FUN_0041238c @ 0041238c ===== */

undefined4 FUN_0041238c(void)

{
  undefined4 uVar1;
  int iVar2;
  
  FUN_00436b1c();
  if (DAT_012171d0 != 0) {
    if (DAT_01217064 != 0) {
      lv_timer_pause();
      lv_timer_del(DAT_01217064);
      DAT_01217064 = 0;
    }
    if (DAT_012167e4 != 0) {
      uVar1 = FUN_004250c0();
      FUN_004119c8(uVar1);
    }
    iVar2 = FUN_004155dc();
    if (iVar2 == 1) {
      FUN_004213a0();
      FUN_00420858();
      if ((DAT_01217078 != 0) && (iVar2 = FUN_0042f374(), iVar2 == 0)) {
        FUN_00425478(DAT_01217078);
        printf("backlight:%d\n",(uint)DAT_01217078);
        DAT_01217078 = DAT_01217078 ^ 1;
      }
    }
    else if (iVar2 == 0) {
      FUN_00415dd8();
    }
    else if (iVar2 == 2) {
      iVar2 = FUN_0042291c();
      if (iVar2 != 0) {
        FUN_004227e4(0,0);
      }
      FUN_00411178();
      FUN_00419a48(0);
      FUN_00423bb8();
    }
    FUN_00418ff0();
    lv_group_remove_all_objs(DAT_012171d0);
    lv_group_del(DAT_012171d0);
    DAT_012171d0 = 0;
    FUN_0040a544();
    if (DAT_012167e4 != 0) {
      FUN_00425c80();
      FUN_00424308(DAT_012167e4);
      DAT_012167e4 = 0;
      *(undefined4 *)(&DAT_0121707c + *(int *)(DAT_012167ec + 0x400) * 4) = 0;
    }
    FUN_0042e164(0);
    FUN_0041a62c();
  }
  return 0;
}



/* ===== FUNCTION FUN_00412640 @ 00412640 ===== */

undefined4 FUN_00412640(uint param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  code *pcVar5;
  undefined4 uVar6;
  undefined1 auStack_410 [256];
  char *local_310;
  char *local_308;
  
  if (DAT_012167e4 == (int *)0x0) {
    printf("msg error! %s,%d\n","media_msg_handle",0x425);
    return 0xffffffff;
  }
  uVar4 = (uint)*(ushort *)(DAT_012167e4 + 0x112);
  if (uVar4 != (param_1 & 0xffff)) goto switchD_004126a8_caseD_0;
  switch(param_1 >> 0x10) {
  default:
    goto switchD_004126a8_caseD_0;
  case 1:
    iVar3 = lv_obj_is_valid(DAT_0121730c);
    if (iVar3 != 0) {
      lv_event_send(DAT_0121730c,0x1e,0);
      return 0;
    }
    goto switchD_004126a8_caseD_0;
  case 3:
    if (*(int *)(DAT_012167ec + 0x400) == 1) {
      memset(auStack_410,0,0x130);
      uVar6 = FUN_0041d32c();
      FUN_0042536c(uVar6,auStack_410);
      if ((local_308 == (char *)0x0) || (iVar3 = strcmp(local_308,"unknown"), iVar3 == 0)) {
        uVar6 = lv_obj_get_child(DAT_01217cbc,5);
        FUN_004214c4(uVar6);
      }
      else {
        uVar6 = lv_obj_get_child(DAT_01217cbc,5);
        lv_label_set_text(uVar6,local_308);
      }
      if ((local_310 == (char *)0x0) || (iVar3 = strcmp(local_310,"unknown"), iVar3 == 0)) {
        uVar6 = lv_obj_get_child(DAT_01217cbc,6);
        FUN_004214c4(uVar6);
      }
      else {
        uVar6 = lv_obj_get_child(DAT_01217cbc,6);
        lv_label_set_text(uVar6,local_310);
      }
      printf("singer:%s,title:%s\n",local_310,local_308);
    }
    break;
  case 5:
    if (*(int *)(DAT_012167ec + 0x400) != 2) {
      if (*(int *)(DAT_012167ec + 0x400) == 0) {
        iVar3 = FUN_00415e34();
        if (iVar3 == 0) {
          FUN_00415e70();
        }
        else {
          FUN_00415eac();
        }
        goto switchD_00412a9c_default;
      }
      sVar1 = *(short *)(DAT_012167ec + 0x406);
      goto LAB_004127f8;
    }
    uVar4 = time((time_t *)0x0);
    srand(uVar4);
    iVar3 = rand();
    sVar1 = (short)((uint)DAT_01217074 >> 8);
    switch(iVar3 % 6) {
    case 0:
      DAT_01217070 = 2;
      DAT_01217074 = 0x320000;
      break;
    case 1:
      DAT_01217070 = 3;
      goto LAB_00412ad0;
    case 2:
      DAT_01217070 = 4;
LAB_00412ad0:
      DAT_01217074 = 0x50000;
      break;
    case 3:
      DAT_01217070 = 5;
      DAT_01217074._0_2_ = sVar1 << 8;
      DAT_01217074 = CONCAT22(3,(short)DAT_01217074);
      break;
    case 4:
      DAT_01217070 = 6;
      DAT_01217074._0_2_ = sVar1 << 8;
      DAT_01217074 = CONCAT22(1,(short)DAT_01217074);
      break;
    case -1:
      DAT_01217070 = 0;
      DAT_01217074 = 0;
    }
switchD_00412a9c_default:
    if (*(int *)(DAT_012167ec + 0x400) != 2) {
      sVar1 = *(short *)(DAT_012167ec + 0x406);
LAB_004127f8:
      if (sVar1 != 0) {
        uVar6 = FUN_0040a6ec();
        uVar2 = FUN_0040a6f8();
        iVar3 = FUN_0040fddc(uVar6,uVar2);
        FUN_0040fda8(auStack_410,DAT_012167ec,iVar3 + 10);
        FUN_00428f34(auStack_410,*DAT_012167e4,0);
      }
      FUN_00416760(0);
    }
    FUN_004118b8(5);
    break;
  case 6:
    FUN_004118b8(6);
    break;
  case 7:
    FUN_00424888(DAT_012167e4);
    iVar3 = *DAT_012167e4;
    if (iVar3 == 0) {
      FUN_0041579c();
      iVar3 = *DAT_012167e4;
    }
    else if (iVar3 == 1) {
      FUN_00420d90();
      iVar3 = *DAT_012167e4;
    }
    if (iVar3 == 1) {
      FUN_004259dc();
      return 0;
    }
    goto switchD_004126a8_caseD_0;
  case 9:
  case 10:
  case 0x15:
    FUN_0042612c(DAT_012167e4);
    FUN_00436b04(0x69,2000,FUN_004118b8,param_1 >> 0x10);
    iVar3 = lv_obj_is_valid(DAT_0121730c);
    if (iVar3 != 0) {
      lv_event_send(DAT_0121730c,0x1e,0);
    }
    iVar3 = FUN_0042b4b4();
    FUN_0042eaec(*(undefined4 *)(iVar3 + 8),DAT_012167e4 + 0x10);
    break;
  case 0xc:
    FUN_0042612c(DAT_012167e4);
    if (*DAT_012167e4 == 0) {
      FUN_00436b04(0x6f,2000,FUN_004118b8,0xc);
      return 0;
    }
    if (*DAT_012167e4 == 2) {
      FUN_00436b04(0x70,2000,FUN_004118b8,0xc);
      return 0;
    }
    goto switchD_004126a8_caseD_0;
  case 0xd:
    if (*DAT_012167e4 == 0) {
      FUN_0042612c(DAT_012167e4);
      FUN_00436b04(0x6b,2000,FUN_004118b8,0xd);
      return 0;
    }
switchD_004126a8_caseD_0:
    break;
  case 0xf:
    uVar6 = 0;
    if (*DAT_012167e4 == 0) {
      pcVar5 = (code *)0x0;
    }
    else {
      if (*DAT_012167e4 != 1) goto switchD_004126a8_caseD_0;
      FUN_0042612c(DAT_012167e4,uVar4,param_3,0);
      uVar6 = 0xf;
      pcVar5 = FUN_004118b8;
    }
    FUN_00436b04(0x6e,2000,pcVar5,uVar6);
    break;
  case 0x10:
    uVar6 = 0;
    if (*DAT_012167e4 == 0) {
      pcVar5 = (code *)0x0;
    }
    else {
      if (*DAT_012167e4 != 1) goto switchD_004126a8_caseD_0;
      FUN_0042612c(DAT_012167e4,uVar4,param_3,0);
      uVar6 = 0x10;
      pcVar5 = FUN_004118b8;
    }
    FUN_00436b04(0x6a,2000,pcVar5,uVar6);
  }
  return 0;
}



/* ===== FUNCTION FUN_00412bc0 @ 00412bc0 ===== */

undefined4 FUN_00412bc0(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  if (param_1 == 0x6b) {
    uVar2 = FUN_004250c0(DAT_012167e4);
    FUN_004119c8(uVar2);
    return 0;
  }
  if (param_1 != 0x72) {
    if (param_1 == 0x67) {
      if (DAT_012167e4 == 0) {
        uVar2 = lv_disp_get_default();
        iVar1 = lv_disp_get_scr_act(uVar2);
        if (DAT_012171f0 != iVar1) {
          return 0;
        }
      }
      iVar1 = FUN_0041ab40();
      if (0 < iVar1) {
        uVar2 = lv_disp_get_default();
        iVar1 = lv_disp_get_scr_act(uVar2);
        if ((DAT_012171f0 == iVar1) && (*(int *)(DAT_012167ec + 0x400) == 3)) {
          uVar2 = FUN_0041ab40();
          FUN_0040c34c(10,uVar2);
        }
      }
    }
    return 0;
  }
  FUN_00406818(DAT_012167d8,0,0);
  FUN_00448950(0);
  return 0;
}



/* ===== FUNCTION FUN_00412ee4 @ 00412ee4 ===== */

undefined4 FUN_00412ee4(void)

{
  if (DAT_01217078 == 0) {
    return 0;
  }
  FUN_00425478(1);
  printf("backlight:%d\n",(uint)DAT_01217078);
  DAT_01217078 = DAT_01217078 ^ 1;
  return 0;
}



/* ===== FUNCTION FUN_00412f50 @ 00412f50 ===== */

undefined4 FUN_00412f50(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined **ppuVar4;
  undefined4 uVar5;
  
  if (DAT_012167e4 == 0) {
    return 0;
  }
  switch(*(undefined4 *)(DAT_012167e4 + 4)) {
  default:
    goto switchD_00412f90_caseD_0;
  case 1:
    uVar5 = 0x54;
    break;
  case 2:
    uVar5 = 0x55;
    break;
  case 3:
    uVar5 = 0x58;
    break;
  case 4:
    uVar5 = 0x57;
    break;
  case 6:
    uVar5 = 0x9c;
  }
  FUN_00437890(DAT_012171f8,uVar5,1);
  if (DAT_012167e4 != 0) {
switchD_00412f90_caseD_0:
    iVar1 = FUN_0042527c();
    iVar2 = FUN_004252ec(DAT_012167e4);
    piVar3 = (int *)FUN_00413c60();
    if ((iVar2 != 0) || (*piVar3 != 0)) {
      if (iVar1 == 4) {
        ppuVar4 = &PTR_DAT_011ea170;
LAB_00413074:
        lv_label_set_text(DAT_012171f4,ppuVar4[iVar2]);
        return 0;
      }
      if (iVar1 == 3) {
        ppuVar4 = &PTR_DAT_011ea154;
        goto LAB_00413074;
      }
      if (iVar1 == 6) {
        ppuVar4 = &PTR_DAT_011ea13c;
        goto LAB_00413074;
      }
      if (iVar1 == 5) {
        ppuVar4 = &PTR_DAT_011ea124;
        goto LAB_00413074;
      }
    }
    lv_label_set_text(DAT_012171f4,&DAT_0044f178);
  }
  return 0;
}



/* ===== FUNCTION FUN_004130d0 @ 004130d0 ===== */

void FUN_004130d0(undefined4 param_1)

{
  short sVar1;
  
  sVar1 = (short)((uint)DAT_01217074 >> 8);
  switch(param_1) {
  case 0:
    DAT_01217070 = 0;
    DAT_01217074 = 0;
    return;
  case 1:
    DAT_01217070 = 2;
    DAT_01217074 = 0x320000;
    return;
  case 2:
    DAT_01217070 = 3;
    DAT_01217074 = 0x50000;
    return;
  case 3:
    DAT_01217070 = 4;
    DAT_01217074 = 0x50000;
    return;
  case 4:
    DAT_01217070 = 5;
    DAT_01217074._0_2_ = sVar1 << 8;
    DAT_01217074 = CONCAT22(3,(short)DAT_01217074);
    return;
  case 5:
    DAT_01217070 = 6;
    DAT_01217074._0_2_ = sVar1 << 8;
    DAT_01217074 = CONCAT22(1,(short)DAT_01217074);
    return;
  case 7:
    DAT_01217070 = 1;
  }
  return;
}



/* ===== FUNCTION FUN_004131cc @ 004131cc ===== */

undefined4 * FUN_004131cc(void)

{
  bool bVar1;
  short sVar2;
  ushort uVar3;
  
  if (DAT_0121706c != 6) goto switchD_0041321c_default;
  bVar1 = DAT_011ea120 < 6;
  DAT_011ea120 = DAT_011ea120 + 1;
  if (bVar1) {
    sVar2 = (short)((uint)DAT_01217074 >> 8);
    switch(DAT_011ea120) {
    case 0:
      DAT_01217070 = 0;
      DAT_01217074 = 0;
      return &DAT_01217070;
    case 1:
      goto switchD_0041321c_caseD_1;
    case 2:
      DAT_01217070 = 3;
      uVar3 = 5;
      break;
    case 3:
      DAT_01217070 = 4;
      uVar3 = 5;
      break;
    case 4:
      DAT_01217070 = 5;
      DAT_01217074._0_2_ = sVar2 << 8;
      DAT_01217074 = CONCAT22(3,(short)DAT_01217074);
      return &DAT_01217070;
    case 5:
      DAT_01217070 = 6;
      DAT_01217074._0_2_ = sVar2 << 8;
      DAT_01217074 = CONCAT22(1,(short)DAT_01217074);
    default:
switchD_0041321c_default:
      return &DAT_01217070;
    }
  }
  else {
    DAT_011ea120 = 1;
switchD_0041321c_caseD_1:
    DAT_01217070 = 2;
    uVar3 = 0x32;
  }
  DAT_01217074 = (uint)uVar3 << 0x10;
  return &DAT_01217070;
}



/* ===== FUNCTION FUN_004132d0 @ 004132d0 ===== */

undefined4 * FUN_004132d0(void)

{
  return &DAT_0121706c;
}



/* ===== FUNCTION FUN_004132dc @ 004132dc ===== */

undefined4 FUN_004132dc(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = FUN_00428454(0x33);
  iVar3 = 10;
  if (*DAT_012167e4 == 0) {
    iVar3 = 10;
  }
  else if (*DAT_012167e4 != 2) {
    iVar3 = 0;
  }
  if (iVar1 < 1) {
    if (iVar1 == 0) {
      iVar4 = iVar3 * 4;
      iVar1 = lv_obj_has_state((&DAT_01217120)[iVar3],0x80);
      if (iVar1 != 0) {
        lv_obj_clear_state((&DAT_01217120)[iVar3],0x80);
      }
    }
    else {
      iVar4 = iVar3 << 2;
    }
  }
  else {
    iVar4 = iVar3 * 4;
    iVar1 = lv_obj_has_state((&DAT_01217120)[iVar3],0x80);
    if (iVar1 == 0) {
      lv_obj_add_state((&DAT_01217120)[iVar3],0x80);
    }
  }
  uVar2 = lv_group_get_default();
  iVar3 = lv_group_get_focused(uVar2);
  if (*(int *)((int)&DAT_01217120 + iVar4) != iVar3) {
    return 0;
  }
  uVar2 = lv_group_get_default();
  lv_group_focus_prev(uVar2);
  return 0;
}



/* ===== FUNCTION FUN_00413410 @ 00413410 ===== */

void FUN_00413410(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  
  uVar4 = *(undefined4 *)(param_1 + 0xc);
  uVar3 = 0;
  while( true ) {
    uVar2 = lv_obj_get_child_cnt(uVar4);
    if (uVar2 <= uVar3) break;
    uVar1 = lv_obj_get_child(uVar4,uVar3);
    lv_obj_clear_state(uVar1,4);
    uVar3 = uVar3 + 1;
  }
  return;
}



/* ===== FUNCTION FUN_00413470 @ 00413470 ===== */

undefined4 * FUN_00413470(undefined4 *param_1,float param_2)

{
  uint uVar1;
  uint uVar2;
  uint unaff_s1;
  uint unaff_s2;
  uint unaff_s3;
  uint unaff_s4;
  uint unaff_s5;
  uint unaff_s6;
  uint unaff_s7;
  float fVar3;
  float fVar4;
  undefined4 local_50;
  undefined4 local_4c;
  uint local_48;
  undefined4 local_44;
  undefined4 local_40;
  uint local_3c;
  uint local_38;
  uint local_34;
  
  FUN_00446394(0);
  if (param_2 == 1.0) {
    unaff_s2 = 0x438;
    unaff_s1 = 0x780;
    unaff_s6 = 0;
    local_38 = (uint)(ushort)DAT_012170e0;
    unaff_s7 = 0;
    unaff_s5 = (uint)DAT_012170e0._2_2_;
    unaff_s3 = (uint)(ushort)DAT_012170e4;
    unaff_s4 = (uint)DAT_012170e4._2_2_;
  }
  else if (1.0 <= param_2) {
    if (1.0 < param_2) {
      uVar1 = FUN_00448108();
      fVar4 = (float)(int)uVar1;
      fVar3 = (float)(ushort)DAT_012170e4 * param_2;
      if (fVar4 < fVar3) {
        if (2.1474836e+09 <= fVar4) {
          fVar4 = fVar4 - 2.1474836e+09;
        }
        unaff_s3 = (int)fVar4 & 0xffff;
      }
      else if (fVar3 < 2.1474836e+09) {
        unaff_s3 = (int)fVar3 & 0xffff;
      }
      else {
        unaff_s3 = (int)(fVar3 - 2.1474836e+09) & 0xffff;
      }
      local_38 = FUN_00448110();
      fVar4 = (float)(int)local_38;
      fVar3 = (float)DAT_012170e4._2_2_ * param_2;
      if (fVar4 < fVar3) {
        if (2.1474836e+09 <= fVar4) {
          fVar4 = fVar4 - 2.1474836e+09;
        }
        unaff_s4 = (int)fVar4 & 0xffff;
      }
      else if (fVar3 < 2.1474836e+09) {
        unaff_s4 = (int)fVar3 & 0xffff;
      }
      else {
        unaff_s4 = (int)(fVar3 - 2.1474836e+09) & 0xffff;
      }
      uVar2 = FUN_004480f8();
      local_34 = local_38;
      local_38 = uVar2;
      if (uVar1 != unaff_s3) {
        local_38 = (uVar2 & 0xffff) + (int)(uVar1 - unaff_s3) / 2;
      }
      local_38 = local_38 & 0xffff;
      unaff_s7 = uVar2 & 0xffff;
      uVar2 = FUN_00448100();
      unaff_s6 = uVar2 & 0xffff;
      unaff_s5 = unaff_s6;
      if (local_34 != unaff_s4) {
        unaff_s5 = unaff_s6 + (int)(local_34 - unaff_s4) / 2 & 0xffff;
      }
      local_50 = 0;
      local_4c = 0;
      local_48 = 0;
      local_44 = 0;
      local_40 = 0;
      local_3c = 0;
      FUN_0042dd64(&local_44);
      FUN_0042dcf4(&local_50);
      if ((local_3c & 0xffff) == 0) {
        trap(7);
      }
      if (local_3c >> 0x10 == 0) {
        trap(7);
      }
      uVar2 = ((local_48 >> 0x10) * 0x438) / (local_3c >> 0x10) - 1 & 0xffff;
      if (uVar1 == (((local_48 & 0xffff) * 0x780) / (local_3c & 0xffff) - 2 & 0xffff)) {
        if (local_34 == uVar2) {
          fVar3 = (float)(int)(uVar1 * 10) / (param_2 * 10.0);
          if (2.1474836e+09 <= fVar3) {
            fVar3 = fVar3 - 2.1474836e+09;
          }
          fVar4 = (float)(int)(local_34 * 10) / (param_2 * 10.0);
          unaff_s1 = (int)fVar3 & 0xffff;
          if (2.1474836e+09 <= fVar4) {
            fVar4 = fVar4 - 2.1474836e+09;
          }
          unaff_s2 = (int)fVar4 & 0xffff;
          unaff_s7 = unaff_s7 + (int)(uVar1 - unaff_s1) / 2 & 0xffff;
          unaff_s6 = unaff_s6 + (int)(local_34 - unaff_s2) / 2 & 0xffff;
        }
        else {
          fVar3 = (float)(int)(uVar1 * 10) / (param_2 * 10.0);
          unaff_s2 = local_34 & 0xffff;
          if (2.1474836e+09 <= fVar3) {
            fVar3 = fVar3 - 2.1474836e+09;
          }
          unaff_s1 = (int)fVar3 & 0xffff;
          unaff_s7 = unaff_s7 + (int)(uVar1 - unaff_s1) / 2 & 0xffff;
        }
      }
      else if (local_34 == uVar2) {
        fVar3 = (float)(int)(local_34 * 10) / (param_2 * 10.0);
        if (2.1474836e+09 <= fVar3) {
          fVar3 = fVar3 - 2.1474836e+09;
        }
        unaff_s2 = (int)fVar3 & 0xffff;
        unaff_s1 = uVar1 & 0xffff;
        unaff_s6 = unaff_s6 + (int)(local_34 - unaff_s2) / 2 & 0xffff;
      }
      else {
        fVar3 = (float)(int)(uVar1 * 10) / (param_2 * 10.0);
        if (2.1474836e+09 <= fVar3) {
          fVar3 = fVar3 - 2.1474836e+09;
        }
        fVar4 = (float)(int)(local_34 * 10) / (param_2 * 10.0);
        unaff_s1 = (int)fVar3 & 0xffff;
        if (2.1474836e+09 <= fVar4) {
          fVar4 = fVar4 - 2.1474836e+09;
        }
        unaff_s2 = (int)fVar4 & 0xffff;
        unaff_s7 = unaff_s7 + (int)(uVar1 - unaff_s1) / 2 & 0xffff;
        unaff_s6 = unaff_s6 + (int)(local_34 - unaff_s2) / 2 & 0xffff;
      }
      DAT_012170b8 = unaff_s7 | unaff_s6 << 0x10;
      DAT_012170bc = unaff_s1 | unaff_s2 << 0x10;
      DAT_012170b4 = unaff_s3 | unaff_s4 << 0x10;
      DAT_012170b0 = local_38 & 0xffff | unaff_s5 << 0x10;
    }
  }
  else {
    fVar4 = param_2 * (float)(ushort)DAT_012170e4;
    fVar3 = ((float)(ushort)DAT_012170e4 - fVar4) * 0.5 + (float)(ushort)DAT_012170e0;
    if (2.1474836e+09 <= fVar3) {
      fVar3 = fVar3 - 2.1474836e+09;
    }
    local_38 = (int)fVar3 & 0xffff;
    param_2 = param_2 * (float)DAT_012170e4._2_2_;
    fVar3 = ((float)DAT_012170e4._2_2_ - param_2) * 0.5 + (float)DAT_012170e0._2_2_;
    if (2.1474836e+09 <= fVar3) {
      fVar3 = fVar3 - 2.1474836e+09;
    }
    unaff_s5 = (int)fVar3 & 0xffff;
    if (2.1474836e+09 <= fVar4) {
      fVar4 = fVar4 - 2.1474836e+09;
    }
    unaff_s3 = (int)fVar4 & 0xffff;
    if (2.1474836e+09 <= param_2) {
      param_2 = param_2 - 2.1474836e+09;
    }
    unaff_s4 = (int)param_2 & 0xffff;
    FUN_00446394(1);
    unaff_s2 = 0x438;
    unaff_s1 = 0x780;
    unaff_s6 = 0;
    unaff_s7 = 0;
  }
  param_1[6] = 0;
  param_1[2] = unaff_s7 & 0xffff | unaff_s6 << 0x10;
  param_1[3] = unaff_s1 & 0xffff | unaff_s2 << 0x10;
  param_1[5] = unaff_s3 & 0xffff | unaff_s4 << 0x10;
  *param_1 = 1;
  param_1[1] = 1;
  param_1[4] = local_38 & 0xffff | unaff_s5 << 0x10;
  return param_1;
}



/* ===== FUNCTION FUN_00413bd4 @ 00413bd4 ===== */

void FUN_00413bd4(void)

{
  undefined2 uVar1;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0x4380780;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  uVar1 = FUN_004480f8();
  local_18 = CONCAT22(local_18._2_2_,uVar1);
  uVar1 = FUN_00448100();
  local_18 = CONCAT22(uVar1,(undefined2)local_18);
  uVar1 = FUN_00448108();
  local_14 = CONCAT22(local_14._2_2_,uVar1);
  uVar1 = FUN_00448110();
  local_14 = CONCAT22(uVar1,(undefined2)local_14);
  FUN_0042ddd4(&local_28);
  if (DAT_012170e8 != 0) {
    DAT_012170e8 = 0;
    DAT_012170ec = 0;
  }
  DAT_011ea190 = 1;
  FUN_00446394(0);
  return;
}



/* ===== FUNCTION FUN_00413c60 @ 00413c60 ===== */

undefined4 * FUN_00413c60(void)

{
  return &DAT_012170e8;
}



/* ===== FUNCTION FUN_00413c6c @ 00413c6c ===== */

undefined4 FUN_00413c6c(int param_1,int param_2)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  uint local_60;
  uint local_5c;
  undefined4 local_58;
  undefined4 local_54;
  uint local_50;
  uint local_4c;
  uint local_48;
  undefined4 local_44;
  undefined4 local_40;
  uint local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  uint local_28;
  uint local_24;
  undefined4 local_20;
  
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  local_58 = 0;
  piVar2 = (int *)FUN_0041d32c();
  if ((DAT_012170e8 == 0) && (DAT_011ea190 != '\0')) {
    DAT_011ea190 = '\0';
    local_54 = 0;
    local_50 = 0;
    local_4c = 0;
    local_44 = 0;
    local_40 = 0;
    local_3c = 0;
    FUN_0042dd64(&local_44);
    FUN_0042dcf4(&local_54);
    uVar5 = local_3c & 0xffff;
    uVar1 = local_3c >> 0x10;
    if (uVar5 == 0) {
      trap(7);
    }
    if (uVar5 == 0) {
      trap(7);
    }
    if (uVar1 == 0) {
      trap(7);
    }
    if (uVar1 == 0) {
      trap(7);
    }
    DAT_012170e4 = ((local_4c & 0xffff) * 0x780) / uVar5 - 2 & 0xffff |
                   (((local_4c >> 0x10) * 0x438) / uVar1 - 1) * 0x10000;
    DAT_012170e0 = ((local_50 & 0xffff) * 0x780) / uVar5 + 2 & 0xffff |
                   (((local_50 >> 0x10) * 0x438) / uVar1 + 1) * 0x10000;
  }
  if (param_1 == 0) {
    iVar3 = DAT_012170ec + 1;
    if (2 < DAT_012170ec) {
      iVar3 = 0;
    }
LAB_00413cf4:
    DAT_012170ec = iVar3;
    if (iVar3 != 1) goto LAB_00413d04;
LAB_00413dec:
    uVar4 = 0x3fc00000;
  }
  else {
    if (param_1 == 1) {
      iVar3 = DAT_012170ec + -1;
      if (DAT_012170ec < 1) {
        iVar3 = 0;
      }
      goto LAB_00413cf4;
    }
    if (DAT_012170ec == 1) goto LAB_00413dec;
LAB_00413d04:
    if (DAT_012170ec < 2) {
      if (DAT_012170ec != 0) {
        return 0;
      }
      FUN_00446394(0);
      DAT_012170e8 = 0;
      local_70 = 1;
      local_6c = 1;
      local_68 = 0;
      local_64 = 0x4380780;
      local_58 = 0;
      local_60 = DAT_012170e0;
      local_5c = DAT_012170e4;
      goto LAB_00413d68;
    }
    if (DAT_012170ec == 2) {
      uVar4 = 0x40000000;
    }
    else {
      if (DAT_012170ec != 3) {
        return 0;
      }
      uVar4 = 0x40400000;
    }
  }
  iVar3 = DAT_012170ec;
  FUN_00413470(&local_38,uVar4);
  local_70 = local_38;
  local_6c = local_34;
  local_68 = local_30;
  local_64 = local_2c;
  local_60 = local_28;
  local_5c = local_24;
  local_58 = local_20;
  DAT_012170e8 = iVar3;
LAB_00413d68:
  if ((*piVar2 == 0) && (param_2 != 0)) {
    local_50 = local_64;
    local_4c = local_60;
    local_54 = local_68;
    local_48 = local_5c;
    iVar3 = FUN_00426174(piVar2,&local_54);
    if (iVar3 == -1) {
      FUN_0042ddd4(&local_70);
    }
  }
  else {
    FUN_0042ddd4(&local_70);
    DAT_012170cc = local_68;
    DAT_012170d0 = local_64;
    DAT_012170d4 = local_60;
    DAT_012170d8 = local_5c;
  }
  return 0;
}



/* ===== FUNCTION FUN_00413f80 @ 00413f80 ===== */

undefined4 FUN_00413f80(void)

{
  DAT_012170a0 = DAT_012170b8 >> 0x10;
  DAT_012170a8 = DAT_012170b8 & 0xffff;
  DAT_01217098 = DAT_012170bc;
  DAT_01217094 = DAT_012170b8;
  DAT_012170a4 = (0x438 - (DAT_012170b8 >> 0x10)) - (DAT_012170bc >> 0x10);
  DAT_012170ac = 0x780 - ((DAT_012170bc & 0xffff) + (DAT_012170b8 & 0xffff));
  DAT_0121709c = 0x14;
  return 0;
}



/* ===== FUNCTION FUN_00413ff0 @ 00413ff0 ===== */

undefined4 FUN_00413ff0(short param_1)

{
  uint uVar1;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  int local_14;
  int local_10 [2];
  
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_10[0] = 0;
  FUN_0042bcc8(local_10,&local_14);
  DAT_012170c0 = (undefined2)DAT_012170a8;
  DAT_012170c2 = (undefined2)DAT_012170a0;
  if (local_10[0] == 1) {
    if (param_1 == 0x11) {
LAB_00414230:
      if (local_14 != 1) {
LAB_00414160:
        if ((int)DAT_012170ac < 1) {
          DAT_0121709c = 0x14;
          if (DAT_012170ac == 0) {
LAB_004142a4:
            DAT_0121709c = 0x14;
            goto LAB_004140b0;
          }
          uVar1 = DAT_012170ac - 0x14;
        }
        else {
          uVar1 = DAT_012170ac - 0x14;
          if (DAT_012170ac < DAT_0121709c) {
            uVar1 = 0;
            DAT_0121709c = DAT_012170ac;
          }
          else {
            DAT_0121709c = 0x14;
          }
        }
        DAT_012170a8 = DAT_0121709c + DAT_012170a8;
        DAT_012170c0 = (undefined2)DAT_012170a8;
        DAT_012170ac = uVar1;
        goto LAB_004140b0;
      }
    }
    else {
      if (param_1 != 0x12) {
        if (param_1 == 0x13) goto LAB_004141c8;
        goto LAB_00414074;
      }
LAB_00414150:
      if (local_14 == 1) goto LAB_00414160;
    }
    if ((int)DAT_012170a8 < 1) {
      DAT_0121709c = 0x14;
      if (DAT_012170a8 == 0) {
        DAT_012170c0 = 0;
        goto LAB_004140b0;
      }
      uVar1 = DAT_012170a8 - 0x14;
      DAT_012170c0 = (undefined2)uVar1;
    }
    else {
      uVar1 = DAT_012170a8 - 0x14;
      if (DAT_012170a8 < DAT_0121709c) {
        DAT_012170c0 = 0;
        DAT_0121709c = DAT_012170a8;
        uVar1 = 0;
      }
      else {
        DAT_012170c0 = (undefined2)uVar1;
        DAT_0121709c = 0x14;
      }
    }
    DAT_012170a8 = uVar1;
    DAT_012170ac = DAT_012170ac + DAT_0121709c;
  }
  else {
    if (local_10[0] < 2) {
      if ((local_10[0] == 0) && (param_1 != 0x11)) {
        if (param_1 == 0x12) goto LAB_004141c8;
        if (param_1 == 0x14) goto LAB_00414150;
        if (param_1 == 0x13) goto LAB_00414230;
      }
    }
    else if (local_10[0] == 2) {
      if (param_1 == 0x11) {
LAB_004141c8:
        if ((int)DAT_012170a4 < 1) {
          DAT_0121709c = 0x14;
          if (DAT_012170a4 == 0) goto LAB_004142a4;
          uVar1 = DAT_012170a4 - 0x14;
        }
        else {
          uVar1 = DAT_012170a4 - 0x14;
          if (DAT_012170a4 < DAT_0121709c) {
            uVar1 = 0;
            DAT_0121709c = DAT_012170a4;
          }
          else {
            DAT_0121709c = 0x14;
          }
        }
        DAT_012170a0 = DAT_0121709c + DAT_012170a0;
        DAT_012170c2 = (short)DAT_012170a0;
        DAT_012170a4 = uVar1;
        goto LAB_004140b0;
      }
      if (param_1 != 0x12) {
        if (param_1 == 0x14) goto LAB_00414230;
        if (param_1 == 0x13) goto LAB_00414150;
      }
    }
    else if (local_10[0] == 3) {
      if (param_1 == 0x11) goto LAB_00414150;
      if (param_1 == 0x12) goto LAB_00414230;
      if (param_1 == 0x14) goto LAB_004141c8;
    }
LAB_00414074:
    if ((int)DAT_012170a0 < 1) {
      DAT_0121709c = 0x14;
      if (DAT_012170a0 == 0) {
        DAT_012170c2 = 0;
        goto LAB_004140b0;
      }
      uVar1 = DAT_012170a0 - 0x14;
      DAT_012170c2 = (undefined2)uVar1;
    }
    else {
      uVar1 = DAT_012170a0 - 0x14;
      if (DAT_012170a0 < DAT_0121709c) {
        DAT_012170c2 = 0;
        DAT_0121709c = DAT_012170a0;
        uVar1 = 0;
      }
      else {
        DAT_012170c2 = (undefined2)uVar1;
        DAT_0121709c = 0x14;
      }
    }
    DAT_012170a0 = uVar1;
    DAT_012170a4 = DAT_012170a4 + DAT_0121709c;
  }
LAB_004140b0:
  local_28 = CONCAT22(DAT_012170c2,DAT_012170c0);
  local_18 = 0;
  DAT_012170c4 = (undefined2)DAT_01217098;
  DAT_012170c6 = DAT_01217098._2_2_;
  local_30 = 1;
  local_2c = 1;
  local_24 = CONCAT22(DAT_01217098._2_2_,(undefined2)DAT_01217098);
  local_20 = DAT_012170b0;
  local_1c = DAT_012170b4;
  FUN_0042ddd4(&local_30);
  return 0;
}



/* ===== FUNCTION FUN_00414948 @ 00414948 ===== */

void FUN_00414948(int param_1,int param_2,ushort *param_3)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  ushort uVar4;
  
  uVar4 = *param_3;
  uVar3 = param_3[1];
  uVar1 = param_3[2];
  uVar2 = param_3[3];
  printf("%s rotate:%d,h_flip:%d\n","zoom_transfer_dst_rect_for_screen",param_1,param_2);
  if (param_1 == 2) {
    uVar4 = (0x780 - uVar4) - uVar1;
  }
  else if (param_1 == 3) {
    uVar4 = (ushort)(((uint)uVar3 * 0x780) / 0x438);
    uVar3 = (ushort)((int)(((0x780 - (uint)*param_3) - (uint)uVar1) * 0x438) / 0x780);
    uVar1 = (ushort)(((uint)param_3[3] * 0x780) / 0x438);
    uVar2 = (ushort)(((uint)param_3[2] * 0x438) / 0x780);
  }
  else if (param_1 == 1) {
    uVar4 = (ushort)((int)(((0x438 - (uint)uVar3) - (uint)uVar2) * 0x780) / 0x438);
    uVar3 = (ushort)(((uint)*param_3 * 0x438) / 0x780);
    uVar1 = (ushort)(((uint)param_3[3] * 0x780) / 0x438);
    uVar2 = (ushort)(((uint)param_3[2] * 0x438) / 0x780);
  }
  if (param_2 == 1) {
    uVar4 = (0x780 - uVar4) - uVar1;
  }
  *param_3 = uVar4;
  param_3[1] = uVar3;
  param_3[2] = uVar1;
  param_3[3] = uVar2;
  return;
}



/* ===== FUNCTION FUN_00414b74 @ 00414b74 ===== */

undefined4 * FUN_00414b74(undefined4 *param_1,int param_2,int param_3)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  int iVar4;
  uint uVar5;
  ushort uVar6;
  uint uVar7;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  
  local_2c = 0;
  local_28 = 0;
  iVar4 = FUN_00448108();
  sVar1 = FUN_004480f8();
  uVar2 = (short)((iVar4 * 0x474) / 0x780) + sVar1;
  iVar4 = FUN_00448110();
  sVar1 = FUN_00448100();
  uVar3 = (short)((iVar4 * 0x96) / 0x438) + sVar1;
  iVar4 = FUN_00448108();
  local_2c = CONCAT22(local_2c._2_2_,(short)((iVar4 * 600) / 0x780));
  iVar4 = FUN_00448110();
  local_2c = CONCAT22((short)((iVar4 * 600) / 0x438),(short)local_2c);
  iVar4 = FUN_00448170();
  if (iVar4 != 0) {
    FUN_0042ee7c(&local_2c,(int)&local_2c + 2,2);
  }
  sVar1 = (short)local_2c;
  uVar5 = local_2c & 0xffff;
  uVar6 = local_2c._2_2_;
  uVar7 = (uint)local_2c._2_2_;
  printf("%s rotate:%d,h_flip:%d\n","zoom_transfer_dst_rect_for_screen",param_2,param_3);
  if (param_2 == 2) {
    uVar2 = (0x780 - uVar2) - sVar1;
  }
  else if (param_2 == 3) {
    uVar7 = (uint)uVar2;
    uVar2 = (ushort)(((uint)uVar3 * 0x780) / 0x438);
    uVar3 = (ushort)((int)(((0x780 - uVar7) - uVar5) * 0x438) / 0x780);
    sVar1 = (short)(((uint)local_2c._2_2_ * 0x780) / 0x438);
    uVar6 = (ushort)(((local_2c & 0xffff) * 0x438) / 0x780);
  }
  else if (param_2 == 1) {
    uVar5 = (uint)uVar2;
    uVar2 = (ushort)((int)(((0x438 - (uint)uVar3) - uVar7) * 0x780) / 0x438);
    uVar3 = (ushort)((uVar5 * 0x438) / 0x780);
    sVar1 = (short)(((uint)local_2c._2_2_ * 0x780) / 0x438);
    uVar6 = (ushort)(((local_2c & 0xffff) * 0x438) / 0x780);
  }
  if (param_3 == 1) {
    uVar2 = (0x780 - uVar2) - sVar1;
  }
  local_30 = CONCAT22(uVar3,uVar2);
  *param_1 = 1;
  local_2c = CONCAT22(uVar6,sVar1);
  param_1[1] = 1;
  param_1[2] = 0;
  param_1[3] = 0x4380780;
  param_1[6] = local_28;
  param_1[4] = local_30;
  param_1[5] = local_2c;
  return param_1;
}



/* ===== FUNCTION FUN_00414ee4 @ 00414ee4 ===== */

undefined4 FUN_00414ee4(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if (DAT_012167d8 == iVar2) {
    return 0;
  }
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if (DAT_0121720c != iVar2) {
    uVar1 = lv_disp_get_default(&local_30);
    iVar2 = lv_disp_get_scr_act(uVar1);
    if (DAT_0121d3ac != iVar2) {
      return 0;
    }
  }
  local_50 = 0;
  local_4c = 0;
  local_48 = 0;
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  FUN_00414b74(&local_30,param_1,param_2);
  local_50 = local_30;
  local_4c = local_2c;
  local_48 = local_28;
  local_44 = local_24;
  local_40 = local_20;
  local_3c = local_1c;
  local_38 = local_18;
  FUN_0042ddd4(&local_50);
  return 0;
}



/* ===== FUNCTION FUN_00414ff4 @ 00414ff4 ===== */

void FUN_00414ff4(void)

{
  FUN_0042d9d4();
  if (DAT_012170e8 != 0) {
    DAT_012170e8 = 0;
    DAT_012170ec = 0;
  }
  DAT_011ea190 = 1;
  FUN_00446394(0);
  return;
}



/* ===== FUNCTION FUN_00415038 @ 00415038 ===== */

void FUN_00415038(void)

{
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  if (DAT_012170e8 != 0) {
    local_28 = 0;
    local_24 = 0;
    local_20 = 0;
    local_10 = 0;
    local_1c = 0x4380780;
    local_14 = DAT_012170e4;
    local_18 = DAT_012170e0;
    FUN_0042ddd4(&local_28);
    DAT_012170e8 = 0;
    DAT_012170ec = 0;
  }
  DAT_011ea190 = 1;
  FUN_00446394(0);
  return;
}



/* ===== FUNCTION FUN_004150c0 @ 004150c0 ===== */

undefined4 * FUN_004150c0(void)

{
  return &DAT_012170cc;
}



/* ===== FUNCTION FUN_004150d0 @ 004150d0 ===== */

void FUN_004150d0(undefined4 param_1)

{
  lv_indev_set_group(_edata,param_1);
  lv_group_set_default(param_1);
  return;
}



/* ===== FUNCTION FUN_00415104 @ 00415104 ===== */

undefined4 FUN_00415104(void)

{
  return DAT_012167cc;
}



/* ===== FUNCTION FUN_00415110 @ 00415110 ===== */

undefined4 FUN_00415110(undefined4 param_1)

{
  DAT_012167cc = param_1;
  return 0;
}



/* ===== FUNCTION FUN_00415120 @ 00415120 ===== */

void FUN_00415120(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  
  iVar2 = lv_event_get_code();
  uVar3 = lv_event_get_target(param_1);
  if (iVar2 != 0xd) {
switchD_004151b0_caseD_b:
    return;
  }
  uVar4 = lv_indev_get_act();
  uVar4 = lv_indev_get_key(uVar4);
  piVar5 = (int *)FUN_0042b4b4();
  uVar3 = lv_obj_get_parent(uVar3);
  iVar2 = lv_obj_get_index(uVar3);
  switch(uVar4) {
  case 10:
    break;
  default:
    goto switchD_004151b0_caseD_b;
  case 0x11:
  case 0x13:
    lv_group_focus_next(DAT_012171dc);
    return;
  case 0x12:
  case 0x14:
    lv_group_focus_prev(DAT_012171dc);
    return;
  case 0x1b:
    FUN_004067ac(3);
    return;
  }
  if (piVar5 == (int *)0x0) {
    return;
  }
  if (*piVar5 < 1) {
    return;
  }
  iVar1 = iVar2;
  if (iVar2 != 1) {
    if (iVar2 == 0) {
      DAT_012167c8 = 0;
      iVar1 = DAT_012167c8;
    }
    else if ((iVar2 != 2) && (iVar2 != 3)) goto LAB_0041524c;
  }
  DAT_012167c8 = iVar1;
LAB_0041524c:
  DAT_012170f0 = iVar2;
  FUN_00406818(DAT_012167d8,0,0);
  return;
}



/* ===== FUNCTION FUN_004154a8 @ 004154a8 ===== */

undefined4 FUN_004154a8(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 local_28 [5];
  
  puVar4 = local_28;
  uVar1 = lv_group_create();
  DAT_012171dc = uVar1;
  lv_indev_set_group(_edata,uVar1);
  lv_group_set_default(uVar1);
  FUN_004172e4();
  local_28[0] = 0;
  local_28[1] = 0;
  local_28[2] = 0;
  local_28[3] = 0;
  iVar2 = 0;
  do {
    iVar3 = iVar2 + 1;
    uVar1 = lv_obj_get_child(DAT_012171c0,iVar2);
    uVar1 = lv_obj_get_child(uVar1,1);
    *puVar4 = uVar1;
    puVar4 = puVar4 + 1;
    iVar2 = iVar3;
  } while (iVar3 != 4);
  iVar2 = lv_obj_is_valid(local_28[DAT_012170f0]);
  if (iVar2 != 0) {
    lv_group_focus_obj(local_28[DAT_012170f0]);
  }
  DAT_012167cc = 0;
  return 0;
}



/* ===== FUNCTION FUN_0041559c @ 0041559c ===== */

undefined4 FUN_0041559c(void)

{
  FUN_00436f84();
  lv_group_remove_all_objs(DAT_012171dc);
  lv_group_del(DAT_012171dc);
  FUN_00417774();
  return 0;
}



/* ===== FUNCTION FUN_004155dc @ 004155dc ===== */

undefined4 FUN_004155dc(void)

{
  return DAT_012167c8;
}



/* ===== FUNCTION FUN_004155e8 @ 004155e8 ===== */

void FUN_004155e8(undefined4 param_1)

{
  DAT_012167c8 = param_1;
  return;
}



/* ===== FUNCTION FUN_00415658 @ 00415658 ===== */

void FUN_00415658(void)

{
  float fVar1;
  undefined4 uVar2;
  uint uVar3;
  undefined4 uVar4;
  uint uVar5;
  float fVar6;
  undefined1 auStack_28 [16];
  
  uVar2 = FUN_0041d32c();
  uVar3 = FUN_004250c0(uVar2);
  FUN_00411af4(uVar3,auStack_28);
  uVar4 = lv_obj_get_child(DAT_01217104,3);
  lv_label_set_text(uVar4,auStack_28);
  uVar5 = FUN_004251bc(uVar2);
  FUN_00411af4(uVar5,auStack_28);
  uVar2 = lv_obj_get_child(DAT_01217104,4);
  lv_label_set_text(uVar2,auStack_28);
  if (uVar5 <= uVar3) {
    return;
  }
  fVar6 = (float)(int)uVar3;
  if ((int)uVar3 < 0) {
    fVar6 = fVar6 + 4.2949673e+09;
  }
  fVar1 = (float)(int)uVar5;
  if ((int)uVar5 < 0) {
    fVar1 = fVar1 + 4.2949673e+09;
  }
  fVar6 = fVar6 / fVar1;
  uVar2 = lv_obj_get_child(DAT_01217104,2);
  lv_obj_align(uVar2,7,(int)(short)(int)(fVar6 * 483.0 + 29.0),0xfffffffa);
  return;
}



/* ===== FUNCTION FUN_0041579c @ 0041579c ===== */

void FUN_0041579c(void)

{
  undefined4 uVar1;
  
  uVar1 = lv_obj_get_child(DAT_01217104,0);
  lv_img_set_src(uVar1,&DAT_00ab3040);
  lv_label_set_text(DAT_01217100," ");
  return;
}



/* ===== FUNCTION FUN_004157e0 @ 004157e0 ===== */

void FUN_004157e0(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint unaff_s1;
  
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_0042036c(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    uVar1 = lv_obj_get_child(DAT_01217104,0);
    lv_img_set_src(uVar1,&DAT_00ab3040);
    lv_label_set_text(DAT_01217100," ");
  }
  if (DAT_01217104 != 0) {
    uVar4 = lv_obj_has_flag(DAT_01217104,1);
    unaff_s1 = (uVar4 ^ 1) & 0xff;
  }
  if (unaff_s1 == 0) {
    lv_obj_clear_flag(DAT_01217104,1);
    lv_timer_reset(DAT_01217108);
    lv_timer_resume(DAT_01217108);
    return;
  }
  lv_timer_reset(DAT_01217108);
  return;
}



/* ===== FUNCTION FUN_004158d0 @ 004158d0 ===== */

void FUN_004158d0(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint unaff_s1;
  
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_00420174(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    uVar1 = lv_obj_get_child(DAT_01217104,0);
    lv_img_set_src(uVar1,&DAT_00ab3040);
    lv_label_set_text(DAT_01217100," ");
  }
  if (DAT_01217104 != 0) {
    uVar4 = lv_obj_has_flag(DAT_01217104,1);
    unaff_s1 = (uVar4 ^ 1) & 0xff;
  }
  if (unaff_s1 == 0) {
    lv_obj_clear_flag(DAT_01217104,1);
    lv_timer_reset(DAT_01217108);
    lv_timer_resume(DAT_01217108);
    return;
  }
  lv_timer_reset(DAT_01217108);
  return;
}



/* ===== FUNCTION FUN_004159c0 @ 004159c0 ===== */

void FUN_004159c0(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 != 2) {
    FUN_004247bc();
    uVar1 = lv_obj_get_child(DAT_01217104,0);
    lv_img_set_src(uVar1,&DAT_00917d40);
    lv_timer_pause(DAT_01217108);
    lv_label_set_text(DAT_01217100," ");
    return;
  }
  FUN_00424888(uVar1);
  uVar1 = lv_obj_get_child(DAT_01217104,0);
  lv_img_set_src(uVar1,&DAT_00ab3040);
  lv_timer_reset(DAT_01217108);
  lv_timer_resume(DAT_01217108);
  lv_label_set_text(DAT_01217100," ");
  return;
}



/* ===== FUNCTION FUN_00415a98 @ 00415a98 ===== */

void FUN_00415a98(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  undefined8 uVar8;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_004251bc(uVar1);
  iVar3 = FUN_004250c0(uVar1);
  iVar4 = FUN_0041d32c();
  if (iVar2 <= iVar3) {
LAB_00415b58:
    puts("get video time error");
    return;
  }
  uVar8 = FUN_0042ebb8();
  uVar6 = (uint)uVar8 - *(int *)(iVar4 + 0x28);
  uVar5 = (int)((ulonglong)uVar8 >> 0x20) - *(int *)(iVar4 + 0x2c);
  uVar7 = (uint)((uint)uVar8 < uVar6);
  if (((int)(uVar5 - uVar7) < 1) && ((uVar5 != uVar7 || (uVar6 < 0xfa)))) {
    iVar2 = iVar2 / 0x32;
    if (iVar2 != 0) {
      *(undefined8 *)(iVar4 + 0x28) = uVar8;
      *(int *)(iVar4 + 0x30) = iVar2;
      if (iVar2 < 0) goto LAB_00415b58;
      goto LAB_00415b8c;
    }
    iVar2 = 2;
    *(undefined4 *)(iVar4 + 0x30) = 2;
  }
  else {
    iVar2 = 10;
    *(undefined4 *)(iVar4 + 0x30) = 10;
  }
  *(undefined8 *)(iVar4 + 0x28) = uVar8;
LAB_00415b8c:
  iVar3 = iVar3 - iVar2;
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  FUN_00424954(uVar1,iVar3);
  return;
}



/* ===== FUNCTION FUN_00415bc8 @ 00415bc8 ===== */

void FUN_00415bc8(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  undefined8 uVar9;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_004251bc(uVar1);
  iVar3 = FUN_004250c0(uVar1);
  iVar4 = FUN_0041d32c();
  if (iVar2 <= iVar3) {
LAB_00415c88:
    puts("get video time error");
    return;
  }
  uVar9 = FUN_0042ebb8();
  uVar6 = (uint)uVar9 - *(int *)(iVar4 + 0x28);
  uVar5 = (int)((ulonglong)uVar9 >> 0x20) - *(int *)(iVar4 + 0x2c);
  uVar7 = (uint)((uint)uVar9 < uVar6);
  if (((int)(uVar5 - uVar7) < 1) && ((uVar5 != uVar7 || (uVar6 < 0xfa)))) {
    iVar8 = iVar2 / 0x32;
    if (iVar8 != 0) {
      *(undefined8 *)(iVar4 + 0x28) = uVar9;
      *(int *)(iVar4 + 0x30) = iVar8;
      if (iVar8 < 0) goto LAB_00415c88;
      goto LAB_00415cbc;
    }
    *(undefined4 *)(iVar4 + 0x30) = 2;
    iVar8 = 2;
  }
  else {
    iVar8 = 10;
    *(undefined4 *)(iVar4 + 0x30) = 10;
  }
  *(undefined8 *)(iVar4 + 0x28) = uVar9;
LAB_00415cbc:
  iVar4 = iVar3 + iVar8;
  if (iVar2 <= iVar3 + iVar8) {
    iVar4 = iVar2;
  }
  FUN_00424954(uVar1,iVar4);
  return;
}



/* ===== FUNCTION FUN_00415cf8 @ 00415cf8 ===== */

void FUN_00415cf8(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 == 2) {
    FUN_00424888(uVar1);
    uVar3 = lv_obj_get_child(DAT_01217104,0);
    lv_img_set_src(uVar3,&DAT_00ab3040);
  }
  FUN_00424a30(uVar1);
  iVar2 = FUN_004252ec(uVar1);
  if (iVar2 != 0) {
    lv_label_set_text(DAT_01217100,(&PTR_DAT_0044ff40)[iVar2]);
    return;
  }
  lv_label_set_text(DAT_01217100," ");
  return;
}



/* ===== FUNCTION FUN_00415db4 @ 00415db4 ===== */

void FUN_00415db4(void)

{
  undefined4 uVar1;
  
  uVar1 = FUN_0041d32c();
  FUN_0042668c(uVar1,0);
  return;
}



/* ===== FUNCTION FUN_00415dd8 @ 00415dd8 ===== */

void FUN_00415dd8(void)

{
  int iVar1;
  
  iVar1 = lv_obj_is_valid(DAT_01217104);
  if (iVar1 != 0) {
    lv_obj_del(DAT_01217104);
    DAT_01217104 = 0;
  }
  if (DAT_01217108 != 0) {
    lv_timer_del();
    DAT_01217108 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_00415e34 @ 00415e34 ===== */

undefined * FUN_00415e34(void)

{
  uint uVar1;
  
  if (DAT_01217104 != 0) {
    uVar1 = lv_obj_has_flag(DAT_01217104,1);
    return (undefined *)((uVar1 ^ 1) & 0xff);
  }
  return &DAT_01210000;
}



/* ===== FUNCTION FUN_00415e70 @ 00415e70 ===== */

void FUN_00415e70(void)

{
  lv_obj_clear_flag(DAT_01217104,1);
  lv_timer_reset(DAT_01217108);
  lv_timer_resume(DAT_01217108);
  return;
}



/* ===== FUNCTION FUN_00415eac @ 00415eac ===== */

void FUN_00415eac(void)

{
  lv_timer_reset(DAT_01217108);
  return;
}



/* ===== FUNCTION FUN_00415eb8 @ 00415eb8 ===== */

undefined4 FUN_00415eb8(undefined4 param_1)

{
  undefined4 uVar1;
  
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  DAT_01217104 = lv_img_create(param_1);
  lv_img_set_src(DAT_01217104,&DAT_0080e820);
  lv_obj_set_size(DAT_01217104,0x27d1,0x27d1);
  lv_obj_align(DAT_01217104,5,0,0xfffffffb);
  lv_obj_set_style_bg_opa(DAT_01217104,0,0);
  lv_obj_set_style_border_width(DAT_01217104,0,0);
  lv_obj_clear_flag(DAT_01217104,0x10);
  uVar1 = lv_img_create(DAT_01217104);
  lv_img_set_src(uVar1,&DAT_00ab3040);
  lv_obj_align(uVar1,7,0x11,2);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_img_create(DAT_01217104);
  lv_obj_align(uVar1,9,10,0xfffffff9);
  lv_img_set_src(uVar1,&DAT_007bbb80);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_img_create(DAT_01217104);
  lv_obj_align(uVar1,7,0x1d,0xfffffffa);
  lv_img_set_src(uVar1,&DAT_007bb640);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_label_create(DAT_01217104);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align(uVar1,4,0x30,5);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  uVar1 = lv_label_create(DAT_01217104);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align(uVar1,6,0xffffffe7,5);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  DAT_01217100 = lv_label_create(DAT_0121720c);
  lv_obj_set_width(DAT_01217100,0x27d1);
  lv_obj_set_height(DAT_01217100,0x27d1);
  lv_obj_align(DAT_01217100,3,0xfffffff6,0);
  lv_label_set_text(DAT_01217100,&DAT_0044f178);
  lv_obj_set_style_text_color(DAT_01217100,0xffffffff,0);
  lv_obj_set_style_text_opa(DAT_01217100,0xff,0);
  lv_obj_set_style_text_font(DAT_01217100,&DAT_0121edb0,0);
  DAT_01217108 = lv_timer_create(&LAB_00415600,3000,0);
  lv_timer_set_repeat_count(DAT_01217108,0xffffffff);
  return 0;
}



/* ===== FUNCTION FUN_00416230 @ 00416230 ===== */

undefined4 FUN_00416230(void)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  char *__s;
  int iVar5;
  size_t sVar6;
  char *pcVar7;
  char *pcVar8;
  int iVar9;
  int iVar10;
  char *__s1;
  char *pcVar11;
  char acStack_480 [1024];
  char acStack_80 [64];
  int local_40;
  undefined4 local_3c;
  undefined4 local_38;
  int iStack_34;
  int *local_30;
  int local_2c;
  
  local_40 = 0;
  local_3c = 1;
  local_38 = 3;
  uVar2 = lv_disp_get_default();
  iVar3 = lv_disp_get_scr_act(uVar2);
  if ((((iVar3 == DAT_012169e4) || (iVar3 == DAT_0121d644)) || (iVar3 == DAT_01217218)) ||
     ((iVar3 == DAT_01217214 || (iVar3 == DAT_012167d8)))) {
    piVar4 = (int *)FUN_0042b4b4();
    __s = (char *)FUN_0042e3a4(*piVar4 + -1);
    if (__s != (char *)0x0) {
      memset(acStack_480,0,0x400);
      local_30 = &local_40;
      iVar3 = local_40;
      while (local_2c = iVar3, iVar5 = FUN_00428c28(iVar3), iVar5 != 0) {
        iVar9 = 1;
        if (iVar3 != 1) {
          iVar9 = 4;
        }
        __s1 = (char *)(iVar5 + 0x10);
        iVar3 = 0;
        do {
          sVar6 = strlen(__s);
          iVar5 = strncmp(__s1,__s,sVar6);
          pcVar11 = __s1 + -0x10;
          if (iVar5 == 0) {
            iVar5 = access(__s1,0);
            if (iVar5 == 0) {
              FUN_004286f8(0x38,local_2c);
              if (iVar3 != 0) {
                FUN_00411a5c(pcVar11,__s1,*(undefined4 *)pcVar11);
                return 0;
              }
              return 0;
            }
          }
          else {
            iVar5 = 0;
            memset(acStack_80,0,0x40);
            strncpy(acStack_80,__s,0x3f);
            pcVar7 = strtok(acStack_80,"/");
            while (pcVar7 != (char *)0x0) {
              pcVar7 = strtok((char *)0x0,"/");
              iVar5 = iVar5 + 1;
            }
            iVar10 = 0;
            pcVar7 = __s1;
            do {
              cVar1 = *pcVar7;
              pcVar7 = pcVar7 + 1;
              if (cVar1 == '\0') goto LAB_00416384;
            } while ((cVar1 != '/') || (iVar10 = iVar10 + 1, iVar10 <= iVar5));
            pcVar8 = stpcpy(acStack_480,__s);
            sVar6 = strlen(__s);
            if (__s[sVar6 - 1] != '/') {
              *pcVar8 = '/';
              pcVar8[1] = '\0';
            }
            strcat(acStack_480,pcVar7);
            iVar5 = access(acStack_480,0);
            if (iVar5 == 0) {
              strcpy(__s1,acStack_480);
              FUN_004286f8(0x38,local_2c);
              FUN_00411a5c(pcVar11,__s1,*(undefined4 *)pcVar11);
              printf("change memory play:%s\n",__s1);
              return 0;
            }
          }
LAB_00416384:
          iVar3 = iVar3 + 1;
          __s1 = __s1 + 0x410;
        } while (iVar9 != iVar3);
        local_30 = local_30 + 1;
        if (&iStack_34 == local_30) {
          return 0xffffffff;
        }
        iVar3 = *local_30;
      }
    }
  }
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0041655c @ 0041655c ===== */

undefined4
FUN_0041655c(undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_00428a7c(param_2,param_4);
  uVar2 = 0xffffffff;
  if ((iVar1 != 0) && (uVar2 = 0xffffffff, *(char *)(iVar1 + 0x10) != '\0')) {
    uVar2 = 0;
    param_3[1] = *(undefined4 *)(iVar1 + 4);
    param_3[2] = *(undefined4 *)(iVar1 + 8);
    *param_3 = *(undefined4 *)(iVar1 + 0xc);
  }
  return uVar2;
}



/* ===== FUNCTION FUN_004165bc @ 004165bc ===== */

undefined4
FUN_004165bc(undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  size_t sVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  char *__s;
  char *__s_00;
  undefined4 *__s_01;
  undefined4 uVar9;
  
  iVar1 = FUN_0042b4b4();
  iVar2 = FUN_00428c28(param_4);
  __s = (char *)(iVar2 + 0x10);
  if ((iVar2 != 0) && (sVar3 = strlen(__s), 2 < sVar3)) {
    __s_00 = *(char **)(iVar1 + 8);
    sVar3 = strlen(__s_00);
    iVar1 = memcmp(__s,__s_00,sVar3);
    if (iVar1 == 0) {
      iVar1 = access(__s,0);
      if (iVar1 == 0) {
        uVar9 = *(undefined4 *)(iVar2 + 4);
        __s_01 = param_3 + 3;
        param_3[1] = uVar9;
        memset(__s_01,0,0x400);
        puVar4 = __s_01;
        do {
          uVar8 = *(undefined4 *)__s;
          uVar7 = *(undefined4 *)(__s + 4);
          uVar6 = *(undefined4 *)(__s + 8);
          uVar5 = *(undefined4 *)(__s + 0xc);
          __s = __s + 0x10;
          *puVar4 = uVar8;
          puVar4[1] = uVar7;
          puVar4[2] = uVar6;
          puVar4[3] = uVar5;
          puVar4 = puVar4 + 4;
        } while (__s != (char *)(iVar2 + 0x410));
        param_3[2] = *(undefined4 *)(iVar2 + 8);
        *param_3 = *(undefined4 *)(iVar2 + 0xc);
        printf("%s(). mem play:%s, time:%d\n","auto_playing_from_disk",__s_01,uVar9);
        uVar9 = 0;
      }
      else {
        printf("%s(), %d. access %s fail!\n","auto_playing_from_disk",0x3c,__s);
        uVar9 = 0xffffffff;
      }
      return uVar9;
    }
  }
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00416760 @ 00416760 ===== */

void FUN_00416760(undefined4 param_1)

{
  DAT_01217114 = param_1;
  return;
}



/* ===== FUNCTION FUN_00416778 @ 00416778 ===== */

undefined4 FUN_00416778(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_18;
  undefined4 local_14;
  
  if ((DAT_01217114 == 0) && (iVar1 = FUN_00416230(), iVar1 == 0)) {
    local_18 = 0;
    local_14 = 0;
    iVar1 = FUN_0041d190();
    if (iVar1 == 0) {
      DAT_01217114 = 1;
      local_18 = 0x69;
      FUN_0042c5b4(&local_18);
      return 0;
    }
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_00416cb4 @ 00416cb4 ===== */

size_t FUN_00416cb4(undefined4 *param_1,void *param_2,uint param_3)

{
  uint uVar1;
  size_t sVar2;
  
  if ((FILE *)*param_1 == (FILE *)0x0) {
    sVar2 = 0xffffffff;
  }
  else {
    uVar1 = param_1[1];
    if ((uVar1 != 0) && (uVar1 < param_3)) {
      param_3 = uVar1;
    }
    sVar2 = fread(param_2,1,param_3,(FILE *)*param_1);
    param_1[1] = param_1[1] - param_3;
  }
  return sVar2;
}



/* ===== FUNCTION FUN_00416d68 @ 00416d68 ===== */

void FUN_00416d68(void)

{
  int iVar1;
  
  iVar1 = lv_event_get_code();
  if (iVar1 == 0x26) {
    FUN_0041d0cc();
    return;
  }
  if (iVar1 != 0x25) {
    return;
  }
  FUN_0041d124();
  return;
}



/* ===== FUNCTION FUN_00416da8 @ 00416da8 ===== */

void FUN_00416da8(void)

{
  int iVar1;
  
  iVar1 = lv_event_get_code();
  if (iVar1 == 0x26) {
    FUN_00411f78();
    return;
  }
  if (iVar1 != 0x25) {
    return;
  }
  FUN_0041238c(0);
  return;
}



/* ===== FUNCTION FUN_00416dec @ 00416dec ===== */

void FUN_00416dec(void)

{
  int iVar1;
  
  iVar1 = lv_event_get_code();
  if (iVar1 == 0x26) {
    FUN_0040e8dc();
    return;
  }
  if (iVar1 != 0x25) {
    return;
  }
  FUN_0040e450(0);
  return;
}



/* ===== FUNCTION FUN_00416e30 @ 00416e30 ===== */

void FUN_00416e30(void)

{
  DAT_01217218 = lv_obj_create(0);
  lv_obj_clear_flag(DAT_01217218,0x10);
  lv_obj_add_event_cb(DAT_01217218,&LAB_00416d20,0,0);
  lv_obj_set_style_bg_color(DAT_01217218,0xff031fff,0);
  lv_obj_set_style_bg_opa(DAT_01217218,0xff,0);
  return;
}



/* ===== FUNCTION FUN_00416ea0 @ 00416ea0 ===== */

void FUN_00416ea0(void)

{
  DAT_01217214 = lv_obj_create(0);
  lv_obj_clear_flag(DAT_01217214,0x10);
  lv_obj_add_event_cb(DAT_01217214,FUN_00416d68,0,0);
  lv_obj_set_style_bg_color(DAT_01217214,0xff000000,0);
  lv_obj_set_style_bg_opa(DAT_01217214,0xff,0);
  return;
}



/* ===== FUNCTION FUN_00416f0c @ 00416f0c ===== */

void FUN_00416f0c(void)

{
  undefined4 local_10;
  undefined1 *local_c;
  
  DAT_0121720c = lv_obj_create(0);
  lv_obj_clear_flag(DAT_0121720c,0x10);
  lv_obj_add_event_cb(DAT_0121720c,FUN_00416da8,0,0);
  local_10 = DAT_0121720c;
  local_c = &LAB_00412cdc;
  FUN_0042cef8(&local_10);
  return;
}



/* ===== FUNCTION FUN_00416f78 @ 00416f78 ===== */

void FUN_00416f78(void)

{
  undefined4 local_10;
  undefined1 *local_c;
  
  DAT_012171f0 = lv_obj_create(0);
  lv_obj_clear_flag(DAT_012171f0,0x10);
  lv_obj_set_width(DAT_012171f0,0x2064);
  lv_obj_set_height(DAT_012171f0,0x2064);
  lv_obj_add_event_cb(DAT_012171f0,FUN_00416dec,0,0);
  lv_obj_set_style_bg_color(DAT_012171f0,0xff000000,0);
  lv_obj_set_style_bg_opa(DAT_012171f0,0xff,0);
  local_10 = DAT_012171f0;
  local_c = &LAB_00412cdc;
  FUN_0042cef8(&local_10);
  return;
}



/* ===== FUNCTION FUN_0041701c @ 0041701c ===== */

undefined4 FUN_0041701c(void)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  char *pcVar6;
  char *pcVar7;
  int iVar8;
  int iVar9;
  
  uVar2 = lv_obj_create();
  lv_obj_set_width(uVar2,0x2064);
  lv_obj_set_height(uVar2,0x2008);
  lv_obj_clear_flag(uVar2,0x10);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_set_style_bg_color(uVar2,0xff031fff,0);
  lv_obj_set_style_bg_opa(uVar2,0xff,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  sVar1 = FUN_0043733c(100);
  lv_obj_set_style_pad_left(uVar2,(int)sVar1,0);
  lv_obj_set_style_pad_right(uVar2,(int)sVar1,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_text_color(uVar2,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar2,0xff,0);
  lv_obj_set_flex_flow(uVar2,0);
  uVar3 = FUN_00437878(0,0);
  lv_obj_set_style_text_font(uVar2,uVar3,0);
  uVar3 = lv_label_create(uVar2);
  lv_obj_set_size(uVar3,0x2007,0x27d1);
  lv_label_set_text(uVar3,&DAT_0044f178);
  uVar4 = lv_label_create(uVar2);
  lv_obj_set_size(uVar4,0x2007,0x27d1);
  lv_label_set_text(uVar4,&DAT_0044f178);
  piVar5 = (int *)FUN_0042b4b4();
  if ((piVar5 == (int *)0x0) || (*piVar5 == 0)) {
    lv_obj_clean(uVar2);
    uVar3 = lv_label_create(uVar2);
    FUN_00437890(uVar3,0xd8,2);
  }
  else if (0 < *piVar5) {
    iVar8 = 0;
    do {
      iVar9 = iVar8 + 1;
      pcVar6 = (char *)glist_nth_data(piVar5[1],iVar8);
      pcVar7 = strstr(pcVar6,"sd");
      if (((pcVar7 != (char *)0x0) || (pcVar7 = strstr(pcVar6,"hd"), pcVar7 != (char *)0x0)) ||
         (pcVar7 = strstr(pcVar6,(char *)&PTR_DAT_0044fef8), pcVar7 != (char *)0x0)) {
        lv_label_set_text(uVar3,&PTR_DAT_0044fefc);
      }
      pcVar6 = strstr(pcVar6,(char *)&PTR_DAT_0044fa2c);
      if (pcVar6 != (char *)0x0) {
        lv_label_set_text(uVar4,&PTR_DAT_0044ff00);
      }
      iVar8 = iVar9;
    } while (iVar9 < *piVar5);
  }
  return uVar2;
}



/* ===== FUNCTION FUN_004172e4 @ 004172e4 ===== */

undefined4 FUN_004172e4(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  
  iVar4 = 4;
  DAT_012167d0 = FUN_0041701c(DAT_01217218);
  uVar1 = lv_obj_create(DAT_01217218);
  lv_obj_align_to(uVar1,DAT_012167d0,0xd,0,0);
  lv_obj_set_size(uVar1,0x2064,0x205c);
  lv_obj_clear_flag(uVar1,0x10);
  lv_obj_set_style_radius(uVar1,0,0);
  lv_obj_set_style_bg_color(uVar1,0xff031fff,0);
  lv_obj_set_style_bg_opa(uVar1,0xff,0);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_set_style_flex_flow(uVar1,4,0);
  lv_obj_set_flex_align(uVar1,4,2,2);
  do {
    uVar2 = lv_obj_create(uVar1);
    lv_obj_set_size(uVar2,0x2014,0x2035);
    lv_obj_clear_flag(uVar2,0x10);
    lv_obj_set_style_radius(uVar2,0,0);
    lv_obj_set_style_bg_color(uVar2,0xff031fff,0);
    lv_obj_set_style_bg_opa(uVar2,0xff,0);
    lv_obj_set_style_border_width(uVar2,0,0);
    uVar3 = lv_img_create(uVar2);
    lv_obj_set_align(uVar3,2);
    uVar2 = lv_btn_create(uVar2);
    lv_obj_set_size(uVar2,0x2050,0x2019);
    lv_obj_set_align(uVar2,5);
    lv_obj_clear_flag(uVar2,0x10);
    lv_obj_set_style_bg_color(uVar2,0xff031fff,0);
    lv_obj_set_style_bg_opa(uVar2,0,0);
    lv_obj_set_style_shadow_color(uVar2,0xff031fff,0);
    lv_obj_set_style_shadow_opa(uVar2,0,0);
    lv_obj_set_style_text_color(uVar2,0xffffffff,0);
    lv_obj_set_style_text_opa(uVar2,0xff,0);
    lv_obj_set_style_text_align(uVar2,2,0);
    lv_obj_set_style_bg_color(uVar2,0xff031fff,4);
    lv_obj_set_style_bg_opa(uVar2,0xff,4);
    lv_obj_set_style_outline_color(uVar2,0xfffad665,4);
    lv_obj_set_style_outline_opa(uVar2,0xff,4);
    lv_obj_set_style_outline_width(uVar2,3,4);
    lv_obj_set_style_outline_pad(uVar2,0,4);
    lv_obj_set_style_text_color(uVar2,0xffffff00,4);
    lv_obj_set_style_text_opa(uVar2,0xff,4);
    uVar3 = lv_label_create(uVar2);
    lv_obj_set_align(uVar3,9);
    iVar4 = iVar4 + -1;
    lv_obj_add_event_cb(uVar2,FUN_00415120,0,0);
  } while (iVar4 != 0);
  uVar2 = lv_obj_get_child(uVar1,0);
  uVar2 = lv_obj_get_child(uVar2,0);
  lv_img_set_src(uVar2,&DAT_00bd8160);
  uVar2 = lv_obj_get_child(uVar1,0);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_obj_get_child(uVar2,0);
  FUN_00437890(uVar2,0x4f,0);
  uVar2 = lv_obj_get_child(uVar1,1);
  uVar2 = lv_obj_get_child(uVar2,0);
  lv_img_set_src(uVar2,&DAT_00afe9c0);
  uVar2 = lv_obj_get_child(uVar1,1);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_obj_get_child(uVar2,0);
  FUN_00437890(uVar2,0x50,0);
  uVar2 = lv_obj_get_child(uVar1,2);
  uVar2 = lv_obj_get_child(uVar2,0);
  lv_img_set_src(uVar2,&DAT_00b81960);
  uVar2 = lv_obj_get_child(uVar1,2);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_obj_get_child(uVar2,0);
  FUN_00437890(uVar2,0x51,0);
  uVar2 = lv_obj_get_child(uVar1,3);
  uVar2 = lv_obj_get_child(uVar2,0);
  lv_img_set_src(uVar2,&DAT_00b3d940);
  uVar2 = lv_obj_get_child(uVar1,3);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_obj_get_child(uVar2,0);
  FUN_00437890(uVar2,0x52,0);
  DAT_012171c0 = uVar1;
  lv_obj_add_event_cb(DAT_012167d0,&LAB_0041527c,0x1e,0);
  return 0;
}



/* ===== FUNCTION FUN_00417774 @ 00417774 ===== */

undefined4 FUN_00417774(void)

{
  lv_obj_clean(DAT_01217218);
  return 0;
}



/* ===== FUNCTION FUN_00417798 @ 00417798 ===== */

undefined4 FUN_00417798(int param_1)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  char acStack_30 [8];
  
  iVar5 = 0;
  FUN_00428454(0x11);
  uVar2 = lv_obj_create(DAT_01217214);
  lv_obj_set_layout(uVar2,LV_LAYOUT_FLEX);
  lv_obj_set_size(uVar2,0x2064,0x205c);
  lv_obj_set_align(uVar2,6);
  lv_obj_add_flag(uVar2,0x10);
  lv_obj_set_style_bg_color(uVar2,0xff000000,0);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  lv_obj_set_scrollbar_mode(uVar2,0);
  lv_obj_set_flex_flow(uVar2,0);
  lv_obj_set_flex_align(uVar2,0,2,2);
  if (-1 < param_1) {
    do {
      uVar3 = lv_obj_create(uVar2,0);
      lv_obj_set_size(uVar3,0x2017,0x2037);
      lv_obj_set_style_bg_color(uVar3,0xff000000,0);
      lv_obj_set_style_radius(uVar3,0,0);
      lv_obj_set_style_border_width(uVar3,0,0);
      lv_obj_clear_flag(uVar3,0x10);
      iVar5 = iVar5 + 1;
      lv_obj_set_scrollbar_mode(uVar3,0);
    } while (iVar5 <= param_1);
  }
  uVar3 = lv_obj_get_child(uVar2);
  uVar3 = lv_btn_create(uVar3);
  lv_obj_set_size(uVar3,0x2046,0x2019);
  lv_obj_align(uVar3,5,0,0);
  lv_obj_add_flag(uVar3,0x400);
  lv_obj_clear_flag(uVar3,0x10);
  lv_obj_set_style_bg_color(uVar3,0xff031fff,0);
  lv_obj_set_style_bg_opa(uVar3,0xff,0);
  lv_obj_set_style_shadow_width(uVar3,0,0);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  lv_obj_set_style_text_align(uVar3,2,0);
  lv_obj_set_style_bg_color(uVar3,0xff031fff,4);
  lv_obj_set_style_bg_opa(uVar3,0xff,4);
  lv_obj_set_style_outline_color(uVar3,0xfffad665,4);
  lv_obj_set_style_outline_opa(uVar3,0xff,4);
  lv_obj_set_style_outline_width(uVar3,3,4);
  lv_obj_set_style_outline_pad(uVar3,0,4);
  lv_obj_set_style_text_color(uVar3,0xffffff00,4);
  lv_obj_set_style_text_opa(uVar3,0xff,4);
  uVar3 = lv_label_create(uVar3);
  lv_obj_set_width(uVar3,0x27d1);
  lv_obj_set_height(uVar3,0x27d1);
  lv_obj_set_x(uVar3,0);
  lv_obj_set_y(uVar3,0);
  lv_obj_set_align(uVar3,9);
  FUN_00437890(uVar3,0x53,0);
  uVar3 = lv_obj_get_child(uVar2,0);
  uVar3 = lv_img_create(uVar3);
  lv_img_set_src(uVar3,&DAT_00b72d20);
  lv_obj_set_width(uVar3,0x27d1);
  lv_obj_set_height(uVar3,0x27d1);
  lv_obj_align(uVar3,9,0,0xffffffec);
  lv_obj_add_flag(uVar3,0x10000);
  lv_obj_clear_flag(uVar3,0x10);
  if (0 < param_1) {
    iVar5 = 1;
    do {
      uVar3 = lv_obj_get_child(uVar2,iVar5);
      uVar3 = lv_btn_create(uVar3);
      lv_obj_set_size(uVar3,0x2046,0x2019);
      lv_obj_set_align(uVar3,5);
      lv_obj_add_flag(uVar3,0x400);
      lv_obj_clear_flag(uVar3,0x10);
      lv_obj_set_style_bg_color(uVar3,0xff031fff,0);
      lv_obj_set_style_bg_opa(uVar3,0xff,0);
      lv_obj_set_style_shadow_width(uVar3,0,0);
      lv_obj_set_style_text_color(uVar3,0xffffffff,0);
      lv_obj_set_style_text_opa(uVar3,0xff,0);
      lv_obj_set_style_text_align(uVar3,2,0);
      uVar4 = FUN_00437878(0,0);
      lv_obj_set_style_text_font(uVar3,uVar4,0);
      lv_obj_set_style_bg_color(uVar3,0xff031fff,4);
      lv_obj_set_style_bg_opa(uVar3,0xff,4);
      lv_obj_set_style_outline_color(uVar3,0xfffad665,4);
      lv_obj_set_style_outline_opa(uVar3,0xff,4);
      lv_obj_set_style_outline_width(uVar3,3,4);
      lv_obj_set_style_outline_pad(uVar3,0,4);
      lv_obj_set_style_text_color(uVar3,0xffffff00,4);
      lv_obj_set_style_text_opa(uVar3,0xff,4);
      uVar4 = lv_obj_get_child(uVar2,iVar5);
      uVar4 = lv_img_create(uVar4);
      lv_img_set_src(uVar4,&DAT_00bbb700);
      lv_obj_set_width(uVar4,0x27d1);
      lv_obj_set_height(uVar4,0x27d1);
      lv_obj_add_flag(uVar4,0x10000);
      lv_obj_clear_flag(uVar4,0x10);
      lv_obj_align(uVar4,9,0,0);
      uVar3 = lv_label_create(uVar3);
      lv_obj_set_width(uVar3,0x27d1);
      lv_obj_set_height(uVar3,0x27d1);
      lv_obj_set_align(uVar3,9);
      sprintf(acStack_30,"%c",iVar5 + 0x42);
      FUN_00437890(uVar3,0,0);
      lv_label_set_text(uVar3,acStack_30);
      bVar1 = param_1 != iVar5;
      iVar5 = iVar5 + 1;
    } while (bVar1);
  }
  return uVar2;
}



/* ===== FUNCTION FUN_00417d5c @ 00417d5c ===== */

undefined4 FUN_00417d5c(void)

{
  short sVar1;
  int *piVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  
  FUN_00428454(0x11);
  piVar2 = (int *)FUN_0042b4b4();
  uVar3 = lv_obj_create(DAT_01217214);
  lv_obj_set_size(uVar3,0x2064,0x2008);
  lv_obj_clear_flag(uVar3,0x10);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_style_bg_color(uVar3,0xff000000,0);
  lv_obj_set_style_bg_opa(uVar3,0xff,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  sVar1 = FUN_0043733c(100);
  lv_obj_set_style_pad_left(uVar3,(int)sVar1,0);
  lv_obj_set_style_pad_right(uVar3,(int)sVar1,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  lv_obj_set_flex_flow(uVar3,0);
  uVar3 = lv_label_create(uVar3);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  uVar4 = FUN_00417798(*piVar2);
  if (-1 < *piVar2) {
    iVar6 = 0;
    do {
      uVar5 = lv_obj_get_child(uVar4,iVar6);
      uVar5 = lv_obj_get_child(uVar5,0);
      lv_obj_add_event_cb(uVar5,&LAB_0041cf30,0,0);
      iVar6 = iVar6 + 1;
    } while (iVar6 <= *piVar2);
  }
  iVar6 = FUN_004155dc();
  if (iVar6 == 1) {
    FUN_00437890(uVar3,0x50,0);
    return 0;
  }
  if (iVar6 == 0) {
    FUN_00437890(uVar3,0x4f,0);
    return 0;
  }
  if (iVar6 != 2) {
    if (iVar6 == 3) {
      FUN_00437890(uVar3,0x52,0);
    }
    return 0;
  }
  FUN_00437890(uVar3,0x51,0);
  return 0;
}



/* ===== FUNCTION FUN_00417ff0 @ 00417ff0 ===== */

undefined4 FUN_00417ff0(void)

{
  lv_obj_clean(DAT_01217214);
  return 0;
}



/* ===== FUNCTION FUN_00418014 @ 00418014 ===== */

undefined4 FUN_00418014(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  lv_obj_set_style_radius(param_2,0,0);
  lv_obj_set_style_bg_color(param_2,0xff000000,0);
  lv_obj_set_style_bg_opa(param_2,0xff,0);
  lv_obj_set_style_shadow_width(param_2,0,0);
  lv_obj_set_style_border_side(param_2,0xf,4);
  lv_obj_set_style_border_color(param_2,0xff0000ff,4);
  lv_obj_set_style_border_opa(param_2,0xff,4);
  lv_obj_set_style_border_width(param_2,3,4);
  lv_obj_clear_flag(param_2,0x10);
  uVar1 = lv_img_create(param_2);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_set_align(uVar1,9);
  lv_obj_add_flag(uVar1,0x10000);
  lv_obj_clear_flag(uVar1,0x10);
  lv_obj_set_style_outline_color(uVar1,0xff0000ff,4);
  lv_obj_set_style_outline_opa(uVar1,0xff,4);
  lv_obj_set_style_outline_width(uVar1,3,4);
  lv_obj_set_style_outline_pad(uVar1,0,4);
  uVar1 = lv_label_create(param_1);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_font(uVar1,SiYuanHeiTi_Light_3000_28_1b,0);
  lv_obj_set_style_text_align(uVar1,2,0);
  lv_obj_set_width(uVar1,0x96);
  lv_obj_set_height(uVar1,0x27d1);
  lv_label_set_long_mode(uVar1,1);
  lv_obj_align_to(uVar1,param_2,0xe,0,8);
  return uVar1;
}



/* ===== FUNCTION FUN_00418200 @ 00418200 ===== */

undefined4 FUN_00418200(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar1 = lv_label_create(DAT_01217210);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align(uVar1,2,0,0x14);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  iVar2 = FUN_004155dc();
  if (iVar2 == 1) {
    FUN_00437890(uVar1,0x50,0);
  }
  else if (iVar2 == 0) {
    FUN_00437890(uVar1,0x4f,0);
  }
  else if (iVar2 == 2) {
    FUN_00437890(uVar1,0x51,0);
  }
  else if (iVar2 == 3) {
    FUN_00437890(uVar1,0x52,0);
  }
  uVar3 = lv_img_create(DAT_01217210);
  lv_img_set_src(uVar3,&DAT_00ace6c0);
  lv_obj_set_width(uVar3,0x27d1);
  lv_obj_set_height(uVar3,0x27d1);
  lv_obj_align_to(uVar3,uVar1,0xe,0,0);
  lv_obj_add_flag(uVar3,0x10000);
  lv_obj_clear_flag(uVar3,0x10);
  uVar1 = lv_label_create(uVar3);
  lv_obj_set_width(uVar1,800);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align(uVar1,7,10,0);
  lv_label_set_text(uVar1,&DAT_0044ffe8);
  lv_label_set_long_mode(uVar1,3);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_text_align(uVar1,1,0);
  uVar4 = FUN_00437878(1,1);
  lv_obj_set_style_text_font(uVar1,uVar4,0);
  uVar1 = lv_label_create(uVar3);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align(uVar1,8,0xfffffff6,0);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  uVar3 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar1,uVar3,0);
  return 0;
}



/* ===== FUNCTION FUN_004184dc @ 004184dc ===== */

undefined4 FUN_004184dc(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = lv_btn_create();
  lv_obj_set_width(uVar1,0x78);
  lv_obj_set_height(uVar1,0x6e);
  lv_obj_set_x(uVar1,0);
  lv_obj_set_y(uVar1,0);
  lv_obj_set_align(uVar1,7);
  lv_obj_add_flag(uVar1,0x400);
  lv_obj_clear_flag(uVar1,0x10);
  lv_obj_set_style_bg_color(uVar1,0xff323232,0);
  lv_obj_set_style_bg_opa(uVar1,0xff,0);
  lv_obj_set_style_outline_color(uVar1,0xff847a7a,0);
  lv_obj_set_style_outline_opa(uVar1,0xff,0);
  lv_obj_set_style_outline_width(uVar1,1,0);
  lv_obj_set_style_outline_pad(uVar1,0,0);
  lv_obj_set_style_shadow_color(uVar1,0xff000000,0);
  lv_obj_set_style_shadow_opa(uVar1,0,0);
  lv_obj_set_style_outline_color(uVar1,0xff3260e9,4);
  lv_obj_set_style_outline_opa(uVar1,0xff,4);
  lv_obj_set_style_outline_width(uVar1,5,4);
  lv_obj_set_style_outline_pad(uVar1,0,4);
  lv_obj_set_style_bg_color(uVar1,0xff323232,0x80);
  lv_obj_set_style_bg_opa(uVar1,0xff,0x80);
  lv_obj_set_style_pad_bottom(uVar1,1,0);
  uVar2 = lv_label_create(uVar1);
  lv_obj_set_width(uVar2,0x27d1);
  lv_obj_set_height(uVar2,0x27d1);
  lv_obj_set_align(uVar2,5);
  lv_label_set_text(uVar2,&DAT_0044f178);
  return uVar1;
}



/* ===== FUNCTION FUN_004186a4 @ 004186a4 ===== */

undefined4 FUN_004186a4(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  
  DAT_01217208 = lv_obj_create();
  lv_obj_set_width(DAT_01217208,0x2064);
  lv_obj_set_height(DAT_01217208,0x201f);
  lv_obj_set_align(DAT_01217208,4);
  lv_obj_clear_flag(DAT_01217208,0x10);
  lv_obj_set_style_radius(DAT_01217208,0,0);
  lv_obj_set_style_bg_color(DAT_01217208,0xff323232,0);
  lv_obj_set_style_bg_opa(DAT_01217208,0xff,0);
  lv_obj_set_style_border_width(DAT_01217208,0,0);
  DAT_01217200 = lv_obj_create(DAT_01217208);
  lv_obj_set_size(DAT_01217200,0x204a,0x2043);
  lv_obj_set_align(DAT_01217200,9);
  lv_obj_add_flag(DAT_01217200,0x400);
  lv_obj_add_flag(DAT_01217200,0x800);
  lv_obj_set_scrollbar_mode(DAT_01217200,0);
  lv_obj_set_style_radius(DAT_01217200,0,0);
  lv_obj_set_style_bg_color(DAT_01217200,0xff323232,0);
  lv_obj_set_style_bg_opa(DAT_01217200,0xff,0);
  lv_obj_set_style_border_color(DAT_01217200,0xff323232,0);
  lv_obj_set_style_border_opa(DAT_01217200,0,0);
  lv_obj_set_style_pad_top(DAT_01217200,4,0);
  uVar1 = DAT_01217200;
  lv_obj_set_style_pad_left(DAT_01217200,5,0);
  lv_obj_set_style_pad_right(uVar1,5,0);
  DAT_01217204 = lv_slider_create(DAT_01217208);
  lv_obj_set_width(DAT_01217204,0x203c);
  lv_obj_set_height(DAT_01217204,0x200c);
  lv_obj_align_to(DAT_01217204,DAT_01217200,10,0xfffffff8,0xfffffff6);
  lv_obj_set_style_radius(DAT_01217204,0,0);
  lv_obj_set_style_bg_color(DAT_01217204,0xffffffff,0);
  lv_obj_set_style_bg_opa(DAT_01217204,0xff,0);
  lv_obj_set_style_outline_color(DAT_01217204,0xff3260e9,4);
  lv_obj_set_style_outline_opa(DAT_01217204,0xff,4);
  lv_obj_set_style_outline_width(DAT_01217204,5,4);
  lv_obj_set_style_outline_pad(DAT_01217204,0,4);
  lv_obj_set_style_radius(DAT_01217204,0,0x20000);
  lv_obj_set_style_bg_color(DAT_01217204,0xffffff00,0x20000);
  lv_obj_set_style_bg_opa(DAT_01217204,0xff,0x20000);
  lv_obj_set_style_bg_color(DAT_01217204,0xffc4baba,0x30000);
  lv_obj_set_style_bg_opa(DAT_01217204,0xff,0x30000);
  lv_obj_set_flex_flow(DAT_01217200,0);
  puVar6 = &DAT_01217120;
  do {
    uVar1 = FUN_004184dc(DAT_01217200);
    *puVar6 = uVar1;
    puVar6 = puVar6 + 1;
    lv_obj_add_event_cb(uVar1,param_2,0,0);
  } while (puVar6 != (undefined4 *)0x1217160);
  lv_obj_add_event_cb(DAT_01217204,param_2,0,0);
  uVar1 = lv_label_create(DAT_01217208);
  lv_obj_set_width(uVar1,0x27d1);
  lv_obj_set_height(uVar1,0x27d1);
  lv_obj_align_to(uVar1,DAT_01217204,0x13,0x1e,0xfffffffb);
  lv_label_set_text(uVar1,&DAT_0044fce0);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  uVar2 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar1,uVar2,0);
  uVar2 = lv_label_create(DAT_01217208);
  lv_obj_set_width(uVar2,0x27d1);
  lv_obj_set_height(uVar2,0x27d1);
  lv_obj_align_to(uVar2,uVar1,0x13,0,0);
  lv_label_set_text(uVar2,&DAT_0044f7d8);
  lv_obj_set_style_text_color(uVar2,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar2,0xff,0);
  uVar3 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar2,uVar3,0);
  uVar3 = lv_label_create(DAT_01217208);
  lv_obj_set_width(uVar3,0x27d1);
  lv_obj_set_height(uVar3,0x27d1);
  lv_obj_align_to(uVar3,uVar2,0x13,0,0);
  lv_label_set_text(uVar3,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  uVar4 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar3,uVar4,0);
  DAT_012171fc = lv_label_create(DAT_01217208);
  lv_obj_set_size(DAT_012171fc,0x203c,0x27d1);
  lv_obj_align_to(DAT_012171fc,DAT_01217200,0xd,0,10);
  lv_label_set_long_mode(DAT_012171fc,3);
  lv_label_set_text(DAT_012171fc,&DAT_0044f178);
  lv_obj_set_style_text_color(DAT_012171fc,0xffffffff,0);
  lv_obj_set_style_text_opa(DAT_012171fc,0xff,0);
  lv_obj_set_style_text_align(DAT_012171fc,0,0);
  lv_obj_set_style_text_font(DAT_012171fc,SiYuanHeiTi_Light_3000_28_1b,0);
  DAT_012171f8 = lv_label_create(DAT_01217208);
  lv_obj_align_to(DAT_012171f8,DAT_012171fc,0x13,0x1e,0);
  lv_label_set_text(DAT_012171f8,"       ");
  lv_obj_set_style_text_color(DAT_012171f8,0xffffffff,0);
  lv_obj_set_style_text_opa(DAT_012171f8,0xff,0);
  lv_obj_set_style_text_font(DAT_012171f8,SiYuanHeiTi_Light_3000_28_1b,0);
  DAT_012171f4 = lv_label_create(DAT_01217208);
  lv_obj_set_width(DAT_012171f4,0x27d1);
  lv_obj_set_height(DAT_012171f4,0x27d1);
  lv_obj_align_to(DAT_012171f4,DAT_012171f8,0x13,0x3c,0);
  lv_label_set_text(DAT_012171f4,&DAT_0044f178);
  lv_obj_set_style_text_color(DAT_012171f4,0xffffffff,0);
  lv_obj_set_style_text_opa(DAT_012171f4,0xff,0);
  lv_obj_set_style_text_font(DAT_012171f4,SiYuanHeiTi_Light_3000_28_1b,0);
  uVar4 = lv_label_create(DAT_01217208);
  lv_obj_set_width(uVar4,0x27d1);
  lv_obj_set_height(uVar4,0x27d1);
  lv_obj_set_x(uVar4,0);
  lv_obj_set_y(uVar4,0);
  lv_obj_align_to(uVar4,DAT_01217200,0x11,0,0xfffffff6);
  lv_obj_add_flag(uVar4,0x10000);
  lv_obj_clear_flag(uVar4,0x10);
  uVar5 = FUN_00437878(0,0);
  lv_obj_set_style_text_font(uVar4,uVar5,0);
  lv_label_set_text(uVar4,&PTR_DAT_0044fe54);
  uVar4 = lv_label_create(DAT_01217208);
  lv_obj_set_width(uVar4,0x27d1);
  lv_obj_set_height(uVar4,0x27d1);
  lv_obj_set_x(uVar4,0);
  lv_obj_set_y(uVar4,0);
  lv_obj_align_to(uVar4,DAT_01217200,0x14,0,0xfffffff6);
  lv_obj_add_flag(uVar4,0x10000);
  lv_obj_clear_flag(uVar4,0x10);
  uVar5 = FUN_00437878(0,0);
  lv_obj_set_style_text_font(uVar4,uVar5,0);
  lv_label_set_text(uVar4,&PTR_DAT_0044fe5c);
  lv_obj_set_style_bg_img_src(DAT_01217120,&DAT_00bc7c40,0);
  lv_obj_set_style_bg_img_src(DAT_01217124,&DAT_00bb0e00,0);
  lv_obj_set_style_bg_img_src(DAT_01217128,&DAT_00af5060,0);
  lv_obj_set_style_bg_img_src(DAT_0121712c,&DAT_00accda0,0);
  lv_obj_set_style_bg_img_src(DAT_01217130,&DAT_00bd2200,0);
  lv_obj_set_style_bg_img_src(DAT_01217134,&DAT_00b299e0,0);
  lv_obj_set_style_bg_img_src(DAT_01217138,&DAT_00af9bc0,0);
  uVar4 = lv_obj_get_child(DAT_01217120,0);
  FUN_00437890(uVar4,0x55,1);
  uVar4 = lv_obj_get_child(DAT_01217124,0);
  FUN_00437890(uVar4,0x58,1);
  uVar4 = lv_obj_get_child(DAT_01217128,0);
  FUN_00437890(uVar4,0x57,1);
  uVar4 = lv_obj_get_child(DAT_0121712c,0);
  FUN_00437890(uVar4,0x120,1);
  uVar4 = lv_obj_get_child(DAT_01217130,0);
  FUN_00437890(uVar4,0x11f,1);
  uVar4 = lv_obj_get_child(DAT_01217134,0);
  FUN_00437890(uVar4,0x56,1);
  uVar4 = lv_obj_get_child(DAT_01217138,0);
  FUN_00437890(uVar4,0x5c,1);
  lv_group_remove_obj(DAT_01217204);
  lv_group_focus_obj(DAT_01217120);
  lv_obj_clear_flag(DAT_01217204,1);
  lv_obj_clear_flag(uVar1,1);
  lv_obj_clear_flag(uVar3,1);
  lv_obj_clear_flag(uVar2,1);
  return 0;
}



/* ===== FUNCTION FUN_00418ff0 @ 00418ff0 ===== */

undefined4 FUN_00418ff0(void)

{
  lv_obj_clean(DAT_0121720c);
  return 0;
}



/* ===== FUNCTION FUN_00419014 @ 00419014 ===== */

void FUN_00419014(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  lv_obj_set_style_bg_img_src(DAT_0121713c,&DAT_00afd0a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217140,&DAT_00af82a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217144,&DAT_00bc6320,0);
  lv_obj_set_style_bg_img_src(DAT_01217148,&DAT_00b280c0,0);
  lv_obj_set_style_bg_img_src(DAT_0121714c,&DAT_00af1e20,0);
  lv_obj_set_style_bg_img_src(DAT_01217150,&DAT_00b63e00,0);
  lv_obj_set_style_bg_img_src(DAT_01217154,&DAT_00b3c020,0);
  uVar1 = lv_obj_get_child(DAT_0121713c,0);
  FUN_00437890(uVar1,0x9b,1);
  uVar1 = lv_obj_get_child(DAT_01217140,0);
  FUN_00437890(uVar1,0x9a,1);
  uVar1 = lv_obj_get_child(DAT_01217144,0);
  FUN_00437890(uVar1,0x5e,1);
  uVar1 = lv_obj_get_child(DAT_01217148,0);
  FUN_00437890(uVar1,0x5f,1);
  uVar1 = lv_obj_get_child(DAT_0121714c,0);
  FUN_00437890(uVar1,0x9c,1);
  uVar1 = lv_obj_get_child(DAT_01217150,0);
  FUN_00437890(uVar1,0x9d,1);
  uVar1 = lv_obj_get_child(DAT_01217154,0);
  iVar2 = FUN_00428454(0x12);
  iVar3 = 0xaf;
  if (iVar2 != 2) {
    iVar2 = FUN_00428454(0x12,0xaf);
    iVar3 = (iVar2 != 0) + 0xb0;
  }
  FUN_00437890(uVar1,iVar3,1);
  FUN_004132dc();
  lv_obj_del(DAT_01217158);
  lv_obj_del(DAT_0121715c);
  return;
}



/* ===== FUNCTION FUN_004191e0 @ 004191e0 ===== */

void FUN_004191e0(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)FUN_004132d0();
  switch(*puVar1) {
  case 0:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x62,1);
    return;
  case 1:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,100,1);
    return;
  case 2:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x65,1);
    return;
  case 3:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x66,1);
    return;
  case 4:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x117,1);
    return;
  case 5:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x68,1);
    return;
  case 6:
    uVar2 = lv_obj_get_child(param_1,0);
    FUN_00437890(uVar2,0x67,1);
    return;
  default:
    return;
  }
}



/* ===== FUNCTION FUN_0041932c @ 0041932c ===== */

undefined4 FUN_0041932c(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  lv_obj_add_flag(DAT_01217204,1);
  uVar1 = lv_obj_get_child(DAT_01217208,2);
  lv_obj_add_flag(uVar1,1);
  uVar1 = lv_obj_get_child(DAT_01217208,3);
  lv_obj_add_flag(uVar1,1);
  uVar1 = lv_obj_get_child(DAT_01217208,4);
  lv_obj_add_flag(uVar1,1);
  lv_obj_set_style_bg_img_src(DAT_01217124,&DAT_00bd4520,0);
  lv_obj_set_style_bg_img_src(DAT_01217128,&DAT_00b267a0,0);
  lv_obj_set_style_bg_img_src(DAT_0121713c,&DAT_00afd0a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217140,&DAT_00af82a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217144,&DAT_00bc6320,0);
  lv_obj_set_style_bg_img_src(DAT_01217148,&DAT_00b280c0,0);
  lv_obj_set_style_bg_img_src(DAT_0121714c,&DAT_00af6980,0);
  lv_obj_set_style_bg_img_src(DAT_01217150,&DAT_00bd5e40,0);
  lv_obj_set_style_bg_img_src(DAT_01217154,&DAT_00b24e80,0);
  lv_obj_set_style_bg_img_src(DAT_01217158,&DAT_00af3740,0);
  lv_obj_set_style_bg_img_src(DAT_0121715c,&DAT_00af3740,0);
  uVar1 = lv_obj_get_child(DAT_01217120,0);
  FUN_00437890(uVar1,0x55,1);
  uVar1 = lv_obj_get_child(DAT_01217124,0);
  FUN_00437890(uVar1,0x61,1);
  uVar1 = lv_obj_get_child(DAT_01217128,0);
  FUN_00437890(uVar1,0x60,1);
  uVar1 = lv_obj_get_child(DAT_0121712c,0);
  FUN_00437890(uVar1,0x122,1);
  uVar1 = lv_obj_get_child(DAT_01217130,0);
  FUN_00437890(uVar1,0x121,1);
  uVar1 = lv_obj_get_child(DAT_0121713c,0);
  FUN_00437890(uVar1,0x9b,1);
  uVar1 = lv_obj_get_child(DAT_01217140,0);
  FUN_00437890(uVar1,0x9a,1);
  uVar1 = lv_obj_get_child(DAT_01217144,0);
  FUN_00437890(uVar1,0x5e,1);
  uVar1 = lv_obj_get_child(DAT_01217148,0);
  FUN_00437890(uVar1,0x5f,1);
  uVar1 = lv_obj_get_child(DAT_0121714c,0);
  FUN_00437890(uVar1,0xd7,1);
  uVar1 = lv_obj_get_child(DAT_01217150,0);
  FUN_00437890(uVar1,0xaa,1);
  uVar1 = lv_obj_get_child(DAT_01217154,0);
  FUN_00437890(uVar1,0xab,1);
  uVar1 = lv_obj_get_child(DAT_01217158,0);
  FUN_00437890(uVar1,0xab,1);
  FUN_004191e0(DAT_01217158);
  uVar1 = lv_obj_get_child(DAT_0121715c,0);
  uVar2 = FUN_00411ad0();
  FUN_00437890(uVar1,uVar2,1);
  FUN_004132dc();
  lv_obj_add_state(DAT_0121714c,0x80);
  lv_obj_del(DAT_0121715c);
  return 0;
}



/* ===== FUNCTION FUN_00419660 @ 00419660 ===== */

void FUN_00419660(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  FUN_0040eac8(DAT_0121720c);
  uVar1 = lv_obj_get_child(DAT_0121712c,0);
  FUN_00437890(uVar1,0x5a,1);
  uVar1 = lv_obj_get_child(DAT_01217130,0);
  FUN_00437890(uVar1,0x59,1);
  lv_obj_set_style_bg_img_src(DAT_0121713c,&DAT_00afd0a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217140,&DAT_00af82a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217144,&DAT_00b342e0,0);
  lv_obj_set_style_bg_img_src(DAT_01217148,&DAT_00acb480,0);
  uVar1 = lv_obj_get_child(DAT_0121713c,0);
  FUN_00437890(uVar1,0x9b,1);
  uVar1 = lv_obj_get_child(DAT_01217140,0);
  FUN_00437890(uVar1,0x9a,1);
  uVar1 = lv_obj_get_child(DAT_01217144,0);
  FUN_00437890(uVar1,0xa7,1);
  puVar2 = &DAT_0121714c;
  uVar1 = lv_obj_get_child(DAT_01217148,0);
  FUN_00437890(uVar1,0xa8,1);
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    lv_obj_del(uVar1);
  } while (puVar2 != (undefined4 *)0x1217160);
  return;
}



/* ===== FUNCTION FUN_004197c0 @ 004197c0 ===== */

undefined4 FUN_004197c0(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = lv_obj_is_valid();
  if (iVar1 != 0) {
    lv_obj_del(param_1);
  }
  return 0;
}



/* ===== FUNCTION FUN_004197f8 @ 004197f8 ===== */

void FUN_004197f8(void)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  
  lv_obj_add_flag(DAT_01217204,1);
  uVar2 = lv_obj_get_child(DAT_01217208,2);
  lv_obj_add_flag(uVar2,1);
  uVar2 = lv_obj_get_child(DAT_01217208,3);
  lv_obj_add_flag(uVar2,1);
  puVar3 = &DAT_01217120;
  uVar2 = lv_obj_get_child(DAT_01217208,4);
  lv_obj_add_flag(uVar2,1);
  lv_obj_set_style_bg_img_src(DAT_01217120,&DAT_00bb0e00,0);
  lv_obj_set_style_bg_img_src(DAT_01217124,&DAT_00af5060,0);
  lv_obj_set_style_bg_img_src(DAT_01217128,&DAT_00accda0,0);
  lv_obj_set_style_bg_img_src(DAT_0121712c,&DAT_00bd2200,0);
  lv_obj_set_style_bg_img_src(DAT_01217130,&DAT_00b299e0,0);
  lv_obj_set_style_bg_img_src(DAT_01217134,&DAT_00bd5e40,0);
  lv_obj_set_style_bg_img_src(DAT_01217138,&DAT_00af82a0,0);
  lv_obj_set_style_bg_img_src(DAT_0121713c,&DAT_00afd0a0,0);
  lv_obj_set_style_bg_img_src(DAT_01217140,&DAT_00b24e80,0);
  uVar2 = lv_obj_get_child(DAT_01217120,0);
  FUN_00437890(uVar2,0xd2,1);
  uVar2 = lv_obj_get_child(DAT_01217124,0);
  FUN_00437890(uVar2,0xd3,1);
  uVar2 = lv_obj_get_child(DAT_01217128,0);
  FUN_00437890(uVar2,0xd4,1);
  uVar2 = lv_obj_get_child(DAT_0121712c,0);
  FUN_00437890(uVar2,0xd5,1);
  uVar2 = lv_obj_get_child(DAT_01217130,0);
  FUN_00437890(uVar2,0x56,1);
  uVar2 = lv_obj_get_child(DAT_01217134,0);
  FUN_00437890(uVar2,0xaa,1);
  uVar2 = lv_obj_get_child(DAT_01217138,0);
  FUN_00437890(uVar2,0x9a,1);
  uVar2 = lv_obj_get_child(DAT_0121713c,0);
  FUN_00437890(uVar2,0x9b,1);
  uVar2 = lv_obj_get_child(DAT_01217140,0);
  FUN_00437890(uVar2,0xab,1);
  do {
    puVar1 = puVar3 + 9;
    puVar3 = puVar3 + 1;
    lv_obj_del(*puVar1);
  } while (puVar3 != &DAT_0121713c);
  return;
}



/* ===== FUNCTION FUN_00419a48 @ 00419a48 ===== */

void FUN_00419a48(undefined1 param_1)

{
  DAT_012171c8 = param_1;
  return;
}



/* ===== FUNCTION FUN_00419a60 @ 00419a60 ===== */

void FUN_00419a60(void)

{
  DAT_012171c4 = lv_obj_create();
  lv_obj_set_style_bg_opa(DAT_012171c4,0,0);
  lv_obj_set_style_bg_img_src(DAT_012171c4,&DAT_007b0780,0);
  lv_obj_set_size(DAT_012171c4,0x27d1,0x27d1);
  lv_obj_set_style_border_width(DAT_012171c4,0,0);
  lv_obj_clear_flag(DAT_012171c4,0x10);
  lv_obj_align(DAT_012171c4,9,0x113,0xffffff29);
  return;
}



/* ===== FUNCTION FUN_00419ae8 @ 00419ae8 ===== */

void FUN_00419ae8(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_00422974();
  if (iVar1 == 0) {
    puts("can\'t find bgm path");
    return;
  }
  iVar1 = FUN_00423b88();
  if (((iVar1 != 0) && (*(int *)(iVar1 + 0x40c) != 0)) && (*(short *)(iVar1 + 0x406) != 0)) {
    DAT_012171c8 = DAT_012171c8 ^ 1;
    if (DAT_012171c8 == 0) {
      FUN_004227e4(0,0);
      iVar1 = lv_obj_is_valid(DAT_012171c4);
      if (iVar1 != 0) {
        lv_obj_del(DAT_012171c4);
        DAT_012171c4 = 0;
      }
      return;
    }
    FUN_00422770();
    iVar1 = lv_obj_is_valid(DAT_012171c4);
    if (iVar1 != 0) {
      lv_obj_del(DAT_012171c4);
      DAT_012171c4 = 0;
    }
    DAT_012171c4 = lv_obj_create(param_1);
    lv_obj_set_style_bg_opa(DAT_012171c4,0,0);
    lv_obj_set_style_bg_img_src(DAT_012171c4,&DAT_007b0780,0);
    lv_obj_set_size(DAT_012171c4,0x27d1,0x27d1);
    lv_obj_set_style_border_width(DAT_012171c4,0,0);
    lv_obj_clear_flag(DAT_012171c4,0x10);
    lv_obj_align(DAT_012171c4,9,0x113,0xffffff29);
    return;
  }
  puts("bgm list is null");
  return;
}



/* ===== FUNCTION FUN_00419c60 @ 00419c60 ===== */

uint FUN_00419c60(uint param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  
  if (param_1 < 0x6b) {
    if (0x68 < param_1) {
      return 1;
    }
    if (param_1 != 0x67) {
      return 0;
    }
  }
  else {
    if ((param_1 == 0x160) || (param_1 == 0x207)) {
      uVar3 = FUN_00420c08();
      return uVar3;
    }
    if (param_1 != 0x6c) {
      return 0;
    }
  }
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if (DAT_0121720c == iVar2) {
    iVar2 = FUN_004155dc();
    if (iVar2 != 2) {
      return 0;
    }
    iVar2 = FUN_00410bd0();
    uVar3 = (uint)(iVar2 == 1);
  }
  else {
    uVar1 = lv_disp_get_default();
    iVar2 = lv_disp_get_scr_act(uVar1);
    uVar3 = 2;
    if (DAT_012167d8 != iVar2) {
      uVar1 = lv_disp_get_default();
      iVar2 = lv_disp_get_scr_act(uVar1);
      uVar3 = 2;
      if (DAT_012171f0 != iVar2) {
        return 0;
      }
    }
  }
  return uVar3;
}



/* ===== FUNCTION FUN_00419d60 @ 00419d60 ===== */

int FUN_00419d60(void)

{
  int __fd;
  int iVar1;
  undefined1 uStack_288;
  undefined1 local_287;
  undefined1 local_286;
  undefined1 local_285;
  undefined4 local_284;
  undefined4 local_27c;
  undefined4 local_58;
  undefined4 local_3c;
  undefined4 local_38;
  
  memset(&uStack_288,0,0x278);
  __fd = open("/dev/auddec",2);
  if (__fd < 0) {
    puts("open auddec err");
  }
  else {
    local_287 = 0;
    local_285 = 2;
    local_286 = 0x10;
    local_284 = 0x10000;
    local_27c = 0x5622;
    local_38 = 100;
    local_3c = 0x96;
    local_58 = 0x9c4;
    iVar1 = ioctl(__fd,0x82780301,&uStack_288);
    if (iVar1 < 0) {
      close(__fd);
      return iVar1;
    }
    iVar1 = ioctl(__fd,0x20000304,0);
    if (iVar1 < 0) {
      close(__fd);
      return iVar1;
    }
  }
  return __fd;
}



/* ===== FUNCTION FUN_0041a1a4 @ 0041a1a4 ===== */

void FUN_0041a1a4(void)

{
  undefined4 uVar1;
  int iVar2;
  
  lv_timer_del(DAT_01217220);
  DAT_01217220 = 0;
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if (DAT_012167d8 != iVar2) {
    DAT_01217278 = 1;
    if (DAT_01217274 != 0) {
      pthread_join(DAT_01217274,(void **)0x0);
      DAT_01217274 = 0;
    }
    if (-1 < DAT_011ea1a0) {
      close(DAT_011ea1a0);
    }
    DAT_011ea1a0 = 0xffffffff;
    puts("cube wav task exit");
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0041a248 @ 0041a248 ===== */

undefined4 FUN_0041a248(uint param_1)

{
  void *__ptr;
  size_t sVar1;
  FILE *__stream;
  long lVar2;
  size_t sVar3;
  size_t *psVar4;
  undefined **ppuVar5;
  int iVar6;
  uint local_res0;
  char local_70 [52];
  size_t local_3c;
  size_t local_38;
  char *local_34;
  undefined4 *local_30;
  
  DAT_01217230 = 1000;
  local_3c = 0;
  local_30 = &DAT_0121722c;
  if (param_1 < 7) {
    DAT_0121722c = calloc(1000,1);
    if (DAT_0121722c == (void *)0x0) goto LAB_0041a4c0;
    local_res0 = param_1;
    if ((int)param_1 < 2) {
      return 0;
    }
  }
  else {
    DAT_0121722c = calloc(1000,1);
    if (DAT_0121722c == (void *)0x0) goto LAB_0041a4c0;
    local_res0 = 6;
  }
  ppuVar5 = &PTR_s_PageBack_media_wav_004501e4;
  psVar4 = &DAT_01217238;
  iVar6 = 1;
  local_34 = "nd bgm path";
  while( true ) {
    local_70[0] = '\0';
    local_70[1] = '\0';
    local_70[2] = '\0';
    local_70[3] = '\0';
    local_70[4] = '\0';
    local_70[5] = '\0';
    local_70[6] = '\0';
    local_70[7] = '\0';
    local_70[8] = '\0';
    local_70[9] = '\0';
    local_70[10] = '\0';
    local_70[0xb] = '\0';
    local_70[0xc] = '\0';
    local_70[0xd] = '\0';
    local_70[0xe] = '\0';
    local_70[0xf] = '\0';
    local_70[0x10] = '\0';
    local_70[0x11] = '\0';
    local_70[0x12] = '\0';
    local_70[0x13] = '\0';
    local_70[0x14] = '\0';
    local_70[0x15] = '\0';
    local_70[0x16] = '\0';
    local_70[0x17] = '\0';
    local_70[0x18] = '\0';
    local_70[0x19] = '\0';
    local_70[0x1a] = '\0';
    local_70[0x1b] = '\0';
    local_70[0x1c] = '\0';
    local_70[0x1d] = '\0';
    local_70[0x1e] = '\0';
    local_70[0x1f] = '\0';
    local_70[0x20] = '\0';
    local_70[0x21] = '\0';
    local_70[0x22] = '\0';
    local_70[0x23] = '\0';
    local_70[0x24] = '\0';
    local_70[0x25] = '\0';
    local_70[0x26] = '\0';
    local_70[0x27] = '\0';
    local_70[0x28] = '\0';
    local_70[0x29] = '\0';
    local_70[0x2a] = '\0';
    local_70[0x2b] = '\0';
    local_70[0x2c] = '\0';
    local_70[0x2d] = '\0';
    local_70[0x2e] = '\0';
    local_70[0x2f] = '\0';
    local_70[0x30] = '\0';
    local_70[0x31] = '\0';
    sVar1 = strlen(local_70);
    builtin_strncpy(local_70 + sVar1,"/mnt/sdcard/cubegm/",0x14);
    strcpy(local_70 + sVar1 + 0x13,*ppuVar5);
    __stream = fopen(local_70,local_34 + -0x30);
    if (__stream == (FILE *)0x0) {
      printf("fopen file \"%s\" fail, please check the file\n",local_70);
      return 0xffffffff;
    }
    fseek(__stream,0x28,0);
    fread(&local_3c,1,4,__stream);
    fseek(__stream,0,2);
    lVar2 = ftell(__stream);
    sVar1 = local_3c;
    if (lVar2 + -0x2c <= (int)local_3c) {
      lVar2 = ftell(__stream);
      sVar1 = lVar2 - 0x2c;
    }
    *psVar4 = sVar1;
    fseek(__stream,0x2c,0);
    ppuVar5 = ppuVar5 + 1;
    local_38 = *psVar4;
    __ptr = malloc(local_38);
    psVar4[-1] = (size_t)__ptr;
    if (__ptr == (void *)0x0) break;
    sVar1 = fread(__ptr,1,local_38,__stream);
    sVar3 = *psVar4;
    psVar4 = psVar4 + 2;
    if (sVar1 != sVar3) {
      fclose(__stream);
      free((void *)local_30[iVar6 * 2]);
      perror("Error reading file");
      return 0xffffffff;
    }
    iVar6 = iVar6 + 1;
    fclose(__stream);
    if ((int)local_res0 <= iVar6) {
      return 0;
    }
  }
  fclose(__stream);
LAB_0041a4c0:
  perror("Error allocating memory");
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0041a59c @ 0041a59c ===== */

void FUN_0041a59c(int param_1)

{
  int iVar1;
  
  if (DAT_01217278 != 0) {
    return;
  }
  iVar1 = FUN_004155dc();
  if ((iVar1 == 1) && (param_1 != 5)) {
    return;
  }
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121725c);
  DAT_01217228 = DAT_01217228 + 1;
  DAT_011ea1a4 = param_1;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121725c);
  return;
}



/* ===== FUNCTION FUN_0041a62c @ 0041a62c ===== */

undefined4 FUN_0041a62c(void)

{
  int iVar1;
  undefined4 uVar2;
  pthread_attr_t pStack_38;
  
  if (DAT_01217274 == 0) {
    puts("cube_bg_music_play_task_start ");
    DAT_01217278 = 0;
    if (DAT_011ea1a0 < 0) {
      DAT_011ea1a0 = FUN_00419d60();
      printf("audio_fd = %d\n",DAT_011ea1a0);
      if (DAT_011ea1a0 < 0) {
        puts("audio_fd <0,return");
        return 0;
      }
    }
    pthread_attr_init(&pStack_38);
    pthread_attr_setstacksize(&pStack_38,0x4000);
    iVar1 = pthread_create((pthread_t *)&DAT_01217274,&pStack_38,(__start_routine *)&LAB_00419e80,
                           (void *)0x0);
    if (iVar1 == 0) {
      pthread_attr_destroy(&pStack_38);
      uVar2 = 0;
    }
    else {
      puts("cube wav pthread create failed");
      uVar2 = 0xffffffff;
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0041a77c @ 0041a77c ===== */

void FUN_0041a77c(void)

{
  DAT_01217278 = 1;
  if (DAT_01217274 != 0) {
    pthread_join(DAT_01217274,(void **)0x0);
    DAT_01217274 = 0;
  }
  if (-1 < DAT_011ea1a0) {
    close(DAT_011ea1a0);
  }
  DAT_011ea1a0 = 0xffffffff;
  puts("cube wav task exit");
  return;
}



/* ===== FUNCTION FUN_0041a8c8 @ 0041a8c8 ===== */

void FUN_0041a8c8(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  char local_30 [16];
  undefined4 local_20;
  undefined4 local_1c;
  
  local_30[0] = '\0';
  local_30[1] = '\0';
  local_30[2] = '\0';
  local_30[3] = '\0';
  local_30[4] = '\0';
  local_30[5] = '\0';
  local_30[6] = '\0';
  local_30[7] = '\0';
  local_30[8] = '\0';
  local_30[9] = '\0';
  local_30[10] = '\0';
  local_30[0xb] = '\0';
  local_30[0xc] = '\0';
  local_30[0xd] = '\0';
  local_30[0xe] = '\0';
  local_30[0xf] = '\0';
  iVar1 = lv_event_get_code();
  lv_event_get_target(param_1);
  lv_event_get_user_data(param_1);
  lv_timer_reset(DAT_0121728c);
  if (iVar1 == 0xd) {
    uVar2 = lv_indev_get_act();
    iVar1 = lv_indev_get_key(uVar2);
    uVar3 = FUN_00429cd8(iVar1);
    if (iVar1 == 10) {
      printf("%s,%d\n","key_num_keyinput_event_cb",0x33);
      lv_timer_pause(DAT_0121728c);
      uVar2 = DAT_01217280;
      lv_group_remove_all_objs(DAT_01217288);
      lv_group_del(DAT_01217288);
      if (DAT_01217290 != 0) {
        lv_obj_add_flag(DAT_01217290,1);
        lv_obj_clean(DAT_01217290);
      }
      if (DAT_0121728c != 0) {
        lv_timer_pause();
        lv_timer_del(DAT_0121728c);
        DAT_0121728c = 0;
      }
      local_20 = 0x3e9;
      local_1c = 0x67;
      FUN_0042c5b4(&local_20);
      printf("%s,%d\n","close_digital_num",0x86);
      FUN_004150d0(uVar2);
    }
    else if (uVar3 < 10) {
      if (DAT_01217284 < 100000) {
        DAT_01217284 = DAT_01217284 * 10 + uVar3;
        sprintf(local_30,"%d",DAT_01217284);
        uVar2 = lv_obj_get_child(DAT_01217290,0);
        lv_label_set_text(uVar2,local_30);
      }
    }
    else if (iVar1 == 0x1b) {
      DAT_01217284 = -1;
      lv_timer_pause(DAT_0121728c);
      uVar2 = DAT_01217280;
      lv_group_remove_all_objs(DAT_01217288);
      lv_group_del(DAT_01217288);
      if (DAT_01217290 != 0) {
        lv_obj_add_flag(DAT_01217290,1);
        lv_obj_clean(DAT_01217290);
      }
      if (DAT_0121728c != 0) {
        lv_timer_pause();
        lv_timer_del(DAT_0121728c);
        DAT_0121728c = 0;
      }
      local_20 = 0x3e9;
      local_1c = 0x67;
      FUN_0042c5b4(&local_20);
      printf("%s,%d\n","close_digital_num",0x86);
      FUN_004150d0(uVar2);
    }
  }
  return;
}



/* ===== FUNCTION FUN_0041ab40 @ 0041ab40 ===== */

undefined4 FUN_0041ab40(void)

{
  return DAT_01217284;
}



/* ===== FUNCTION FUN_0041ab4c @ 0041ab4c ===== */

void FUN_0041ab4c(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  char local_30 [16];
  undefined4 local_20 [2];
  
  local_30[0] = '\0';
  local_30[1] = '\0';
  local_30[2] = '\0';
  local_30[3] = '\0';
  local_30[4] = '\0';
  local_30[5] = '\0';
  local_30[6] = '\0';
  local_30[7] = '\0';
  local_30[8] = '\0';
  local_30[9] = '\0';
  local_30[10] = '\0';
  local_30[0xb] = '\0';
  local_30[0xc] = '\0';
  local_30[0xd] = '\0';
  local_30[0xe] = '\0';
  local_30[0xf] = '\0';
  DAT_01217288 = lv_group_create();
  FUN_004150d0(DAT_01217288);
  DAT_01217280 = param_3;
  printf("%d\n",0x58);
  DAT_01217290 = lv_obj_create(param_1);
  lv_obj_set_pos(DAT_01217290,0x200c,0x2006);
  lv_obj_set_style_text_color(DAT_01217290,0xff000000,0);
  lv_obj_set_size(DAT_01217290,0x200f,0x200a);
  lv_obj_set_style_border_width(DAT_01217290,3,0);
  uVar2 = DAT_01217290;
  lv_palette_main(local_20,5);
  lv_obj_set_style_border_color(uVar2,local_20[0],0);
  lv_obj_set_style_outline_width(DAT_01217290,5,0);
  uVar2 = DAT_01217290;
  lv_palette_main(local_20,0x12);
  lv_obj_set_style_outline_color(uVar2,local_20[0],0);
  lv_obj_set_style_radius(DAT_01217290,5,0);
  uVar2 = DAT_01217290;
  uVar1 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar2,uVar1,0);
  lv_group_add_obj(DAT_01217288,DAT_01217290);
  DAT_01217284 = param_2;
  uVar2 = lv_label_create(DAT_01217290);
  sprintf(local_30,"%d",DAT_01217284);
  lv_label_set_text(uVar2,local_30);
  lv_obj_add_event_cb(DAT_01217290,FUN_0041a8c8,0xd,0);
  DAT_0121728c = lv_timer_create(&LAB_0041a7f0,5000,param_3);
  return;
}



/* ===== FUNCTION FUN_0041ad10 @ 0041ad10 ===== */

void FUN_0041ad10(undefined4 param_1,undefined4 param_2)

{
  undefined4 local_18;
  undefined4 local_14;
  
  lv_group_remove_all_objs(DAT_01217288);
  lv_group_del(DAT_01217288);
  if (DAT_01217290 != 0) {
    lv_obj_add_flag(DAT_01217290,1);
    lv_obj_clean(DAT_01217290);
  }
  if (DAT_0121728c != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_0121728c);
    DAT_0121728c = 0;
  }
  local_18 = 0x3e9;
  local_14 = 0x67;
  FUN_0042c5b4(&local_18);
  printf("%s,%d\n","close_digital_num",0x86);
  FUN_004150d0(param_2);
  return;
}



/* ===== FUNCTION FUN_0041ae48 @ 0041ae48 ===== */

int FUN_0041ae48(int param_1,int param_2,undefined2 *param_3,int param_4)

{
  byte bVar1;
  ushort uVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  byte *pbVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  ushort *puVar11;
  uint uVar12;
  
  if ((((param_3 == (undefined2 *)0x0) || (param_1 == 0)) || (param_2 == 0)) || (param_4 == 0)) {
    return 0;
  }
  if ((param_2 < 1) || (param_4 < 1)) {
    *param_3 = 0;
    return 0;
  }
  bVar3 = false;
  iVar4 = 0;
  uVar8 = 0;
  uVar12 = DAT_012172a0;
  do {
    pbVar7 = (byte *)(param_1 + uVar8);
    bVar1 = *pbVar7;
    if ((char)bVar1 < '\0') {
      if (bVar1 == 0xe0) {
        if ((param_2 + -1 <= (int)uVar8) || (0x1f < (pbVar7[1] ^ 0x80))) goto LAB_0041af48;
      }
      else {
        if (bVar1 < 0xa0) {
          uVar8 = uVar8 + 1;
          goto LAB_0041aedc;
        }
LAB_0041af48:
        if (param_2 <= (int)(uVar8 + 1)) {
LAB_0041afd0:
          if (bVar3) {
            DAT_012172a0 = uVar12;
          }
          param_3[iVar4] = 0;
          return iVar4;
        }
        puVar11 = param_3 + iVar4;
        iVar9 = 0x5450;
        iVar10 = -1;
        do {
          while( true ) {
            iVar5 = iVar10 + iVar9 >> 1;
            uVar2 = *(ushort *)(iVar5 * 4 + 0x11ff2f0);
            if (uVar2 <= CONCAT11(bVar1,pbVar7[1])) break;
            iVar9 = iVar5;
            if (iVar5 - iVar10 < 2) goto LAB_0041afc0;
          }
          if (CONCAT11(bVar1,pbVar7[1]) <= uVar2) {
            uVar2 = *(ushort *)(iVar5 * 4 + 0x11ff2f2);
            *puVar11 = uVar2;
            if (uVar2 != 0xffff) {
              iVar4 = iVar4 + 1;
              *puVar11 = uVar2 >> 8 | uVar2 << 8;
              goto LAB_0041afc8;
            }
            break;
          }
          iVar10 = iVar5;
        } while (1 < iVar9 - iVar5);
LAB_0041afc0:
        iVar4 = iVar4 + 1;
        *puVar11 = 0x30;
      }
LAB_0041afc8:
      uVar8 = uVar8 + 2;
    }
    else {
      psVar6 = param_3 + iVar4;
      uVar8 = uVar8 + 1;
      iVar4 = iVar4 + 1;
      bVar3 = true;
      *psVar6 = (ushort)bVar1 << 8;
      uVar12 = uVar8 & 1;
    }
LAB_0041aedc:
    if ((param_2 <= (int)uVar8) || (param_4 <= iVar4)) goto LAB_0041afd0;
  } while( true );
}



/* ===== FUNCTION FUN_0041b3d4 @ 0041b3d4 ===== */

void FUN_0041b3d4(undefined4 param_1)

{
  char in_v0;
  int in_v1;
  int in_t1;
  
  *(undefined4 *)(&DAT_012172b4 + in_t1) = param_1;
  *(char *)(in_v1 + 0x72b0) = in_v0 + '\x01';
  return;
}



/* ===== FUNCTION FUN_0041b450 @ 0041b450 ===== */

undefined4 FUN_0041b450(int param_1)

{
  int iVar1;
  char acStack_408 [1024];
  
  memset(acStack_408,0,0x400);
  iVar1 = FUN_0040fddc(param_1,*(undefined2 *)(param_1 + 0x408));
  FUN_0040fda8(acStack_408,param_1,iVar1 + 10);
  iVar1 = __xstat(3,acStack_408,(stat *)&DAT_0121d7b4);
  if (iVar1 == 0) {
    DAT_0121d7a8 = (float)DAT_0121d7e4 * 0.0009765625;
    return 0;
  }
  memset(&DAT_0121d7a8,0,0x130);
  return 0;
}



/* ===== FUNCTION FUN_0041b504 @ 0041b504 ===== */

undefined4
FUN_0041b504(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,uint param_5)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 local_20 [3];
  
  DAT_01217314 = lv_list_add_text(param_1," ");
  lv_obj_set_style_text_align(DAT_01217314,2,0);
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(DAT_01217314,(int)(short)(uVar1 | 0x2000),0x27d1);
  uVar2 = DAT_01217314;
  lv_obj_set_style_pad_top(DAT_01217314,3,0);
  lv_obj_set_style_pad_bottom(uVar2,3,0);
  lv_obj_set_style_border_width(DAT_01217314,2,0);
  lv_obj_set_style_border_color(DAT_01217314,0xffffffff,0);
  lv_obj_set_style_border_opa(DAT_01217314,0,0);
  lv_obj_set_style_bg_opa(DAT_01217314,0,0);
  lv_obj_set_style_bg_opa(DAT_01217314,0xff,4);
  uVar2 = DAT_01217314;
  lv_palette_main(local_20,5);
  lv_obj_set_style_bg_color(uVar2,local_20[0],4);
  lv_obj_set_style_text_color(DAT_01217314,0xffffffff,0);
  lv_obj_set_style_text_color(DAT_01217314,0xff000000,4);
  if (param_5 == 0) {
    FUN_00437890(DAT_01217314,0,1);
    lv_label_set_text(DAT_01217314,param_4);
  }
  else {
    FUN_00437890(DAT_01217314,param_5 & 0xffff,1);
  }
  lv_label_set_long_mode(DAT_01217314,3);
  return DAT_01217314;
}



/* ===== FUNCTION FUN_0041b688 @ 0041b688 ===== */

undefined4 FUN_0041b688(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 local_18 [3];
  
  uVar1 = lv_list_add_btn(param_1,0," ");
  lv_obj_set_size(uVar1,0x2064,0x27d1);
  lv_obj_set_style_pad_top(uVar1,3,0);
  lv_obj_set_style_pad_bottom(uVar1,3,0);
  lv_obj_set_style_border_side(uVar1,0xf,0);
  lv_obj_set_style_border_width(uVar1,2,0);
  lv_obj_set_style_border_color(uVar1,0xffffffff,0);
  lv_obj_set_style_border_opa(uVar1,0,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_style_bg_opa(uVar1,0xff,4);
  lv_palette_main(local_18,5);
  lv_obj_set_style_bg_color(uVar1,local_18[0],4);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_color(uVar1,0xff000000,4);
  uVar2 = lv_obj_get_child(uVar1,0);
  lv_obj_align(uVar2,7,0,0);
  lv_label_set_long_mode(uVar2,1);
  lv_obj_set_size(uVar2,0x2032,0x27d1);
  uVar2 = lv_label_create(uVar1);
  lv_obj_align(uVar2,8,0,0);
  lv_obj_set_width(uVar2,0x2032);
  lv_label_set_long_mode(uVar2,1);
  return uVar1;
}



/* ===== FUNCTION FUN_0041b808 @ 0041b808 ===== */

undefined4 FUN_0041b808(undefined4 param_1,uint param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = FUN_0041b688();
  uVar2 = lv_obj_get_child(uVar1,0);
  if ((int)param_2 < 0) {
    lv_label_set_text(uVar2," ",1);
  }
  else {
    FUN_00437890(uVar2,param_2 & 0xffff);
  }
  uVar2 = lv_obj_get_child(uVar1,1);
  FUN_00437890(uVar2,1,1);
  lv_label_set_text(uVar2,param_3);
  lv_label_set_long_mode(uVar2,3);
  return uVar1;
}



/* ===== FUNCTION FUN_0041b8b8 @ 0041b8b8 ===== */

undefined4 FUN_0041b8b8(undefined4 param_1,int param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar2 = lv_list_create();
  lv_obj_align(uVar2,3,0xffffffce,10);
  lv_obj_clear_flag(uVar2,0x10);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_set_style_bg_color(uVar2,0xff323232,0);
  lv_obj_set_style_bg_opa(uVar2,0xff,0);
  lv_obj_set_style_border_color(uVar2,0xff323232,0);
  lv_obj_set_style_border_opa(uVar2,0xff,0);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(uVar2,(int)(short)(uVar1 | 0x2000),0x27d1);
  uVar3 = lv_group_get_default();
  lv_group_add_obj(uVar3,uVar2);
  return uVar2;
}



/* ===== FUNCTION FUN_0041ba04 @ 0041ba04 ===== */

void FUN_0041ba04(int param_1,undefined4 param_2,float param_3,int param_4,int param_5)

{
  undefined4 uVar1;
  undefined4 uVar2;
  tm *ptVar3;
  char *__s;
  size_t __n;
  double dVar4;
  int in_stack_000000a8;
  undefined4 in_stack_000000e0;
  undefined4 in_stack_000000e4;
  int in_stack_000000f8;
  char *in_stack_00000108;
  char *in_stack_0000010c;
  undefined4 local_108;
  undefined *local_104;
  undefined2 local_100;
  char acStack_88 [104];
  undefined4 local_20;
  
  memset(&local_108,0,0x80);
  uVar1 = lv_label_get_text(DAT_012171fc);
  memset(acStack_88,0,100);
  local_20 = DAT_0121730c;
  DAT_012172e4 = FUN_0041b504(DAT_0121730c,100,0xb,uVar1,0);
  lv_obj_set_style_text_font(DAT_012172e4,SiYuanHeiTi_Light_3000_28_1b,0);
  if (param_1 == 1) {
    if (in_stack_0000010c == (char *)0x0) {
      local_108 = 0x6e6b6e55;
      local_104 = &DAT_006e776f;
    }
    else {
      strcpy((char *)&local_108,in_stack_0000010c);
    }
    uVar1 = FUN_0041b688(DAT_0121730c);
    uVar2 = lv_obj_get_child(uVar1,0);
    FUN_00437890(uVar2,0xa3,1);
    uVar2 = lv_obj_get_child(uVar1,1);
    FUN_00437890(uVar2,1,1);
    lv_label_set_text(uVar2,&local_108);
    lv_label_set_long_mode(uVar2,3);
    DAT_012172e8 = uVar1;
    memset(&local_108,0,0x80);
    if (in_stack_00000108 == (char *)0x0) {
      local_108 = 0x6e6b6e55;
      local_104 = &DAT_006e776f;
    }
    else {
      strcpy((char *)&local_108,in_stack_00000108);
    }
    uVar1 = FUN_0041b688(DAT_0121730c);
    uVar2 = lv_obj_get_child(uVar1,0);
    FUN_00437890(uVar2,0xa4,1);
    uVar2 = lv_obj_get_child(uVar1,1);
    FUN_00437890(uVar2,1,1);
    lv_label_set_text(uVar2,&local_108);
    lv_label_set_long_mode(uVar2,3);
    DAT_012172ec = uVar1;
    memset(&local_108,0,0x80);
    sprintf((char *)&local_108,"%.2fMB",SUB84((double)param_3,0),
            (int)((ulonglong)(double)param_3 >> 0x20));
    uVar1 = FUN_0041b688(DAT_0121730c);
    uVar2 = lv_obj_get_child(uVar1,0);
    FUN_00437890(uVar2,0xa2,1);
    uVar2 = lv_obj_get_child(uVar1,1);
    FUN_00437890(uVar2,1,1);
    lv_label_set_text(uVar2,&local_108);
    lv_label_set_long_mode(uVar2,3);
    DAT_012172f0 = uVar1;
    memset(&local_108,0,0x80);
    local_20 = DAT_0121730c;
    DAT_012172f4 = FUN_0041b504(DAT_0121730c,100,0xb,0,0xa6);
  }
  else {
    if (param_1 == 0) {
      sprintf((char *)&local_108,"%dx%d",in_stack_000000e0,in_stack_000000e4);
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0x9e,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172e8 = uVar1;
      memset(&local_108,0,0x80);
      if (param_4 == 0) {
        local_108 = 0x2d2d203c;
        local_104 = (undefined *)0x202d2d2f;
        local_100 = 0x3e;
      }
      else {
        sprintf((char *)&local_108,"< %d/%d >",in_stack_000000a8 + 1);
      }
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xa0,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172ec = uVar1;
      memset(&local_108,0,0x80);
      if (param_5 == 0) {
        local_108 = 0x2d2d203c;
        local_104 = (undefined *)0x202d2d2f;
        local_100 = 0x3e;
      }
      else {
        sprintf((char *)&local_108,"< %d/%d >",in_stack_000000f8 + 1);
      }
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xa1,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172f0 = uVar1;
      memset(&local_108,0,0x80);
      dVar4 = (double)param_3;
      if (dVar4 < 0.01) {
        dVar4 = 0.009999999776482582;
      }
      sprintf((char *)&local_108,"%.2fMB",SUB84(dVar4,0),(int)((ulonglong)dVar4 >> 0x20));
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xa2,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      __n = 0x80;
      __s = (char *)&local_108;
      DAT_012172f4 = uVar1;
    }
    else {
      if (param_1 != 2) {
        if (param_1 == 3) {
          if (param_3 <= 1024.0) {
            sprintf((char *)&local_108,"%.2fKB",SUB84((double)param_3,0),
                    (int)((ulonglong)(double)param_3 >> 0x20));
          }
          else {
            sprintf((char *)&local_108,"%.2fMB",SUB84((double)(param_3 * 0.0009765625),0),
                    (int)((ulonglong)(double)(param_3 * 0.0009765625) >> 0x20));
          }
          uVar1 = FUN_0041b688(DAT_0121730c);
          uVar2 = lv_obj_get_child(uVar1,0);
          FUN_00437890(uVar2,0xa2,1);
          uVar2 = lv_obj_get_child(uVar1,1);
          FUN_00437890(uVar2,1,1);
          lv_label_set_text(uVar2,&local_108);
          lv_label_set_long_mode(uVar2,3);
          DAT_012172e8 = uVar1;
          memset(&local_108,0,0x80);
          local_20 = DAT_0121730c;
          DAT_012172ec = FUN_0041b504(DAT_0121730c,100,0xb,0,0xa6);
        }
        goto LAB_0041bb8c;
      }
      sprintf((char *)&local_108,"%dx%d",in_stack_000000e0,in_stack_000000e4);
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0x9e,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172e8 = uVar1;
      memset(&local_108,0,0x80);
      dVar4 = (double)param_3;
      if (0.01 <= dVar4) {
        sprintf((char *)&local_108,"%.2fMB",SUB84(dVar4,0),(int)((ulonglong)dVar4 >> 0x20));
      }
      else {
        sprintf((char *)&local_108,"%.2fKB",SUB84((double)(param_3 * 1024.0),0),
                (int)((ulonglong)(double)(param_3 * 1024.0) >> 0x20));
      }
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xa2,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,&local_108);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172ec = uVar1;
      memset(&local_108,0,0x80);
      ptVar3 = localtime((time_t *)&stack0x00000054);
      strftime(acStack_88,100,"%Y-%m-%d",ptVar3);
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xac,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,acStack_88);
      lv_label_set_long_mode(uVar2,3);
      DAT_012172f0 = uVar1;
      memset(acStack_88,0,100);
      ptVar3 = localtime((time_t *)&stack0x00000054);
      strftime(acStack_88,100,"%H:%M:%S",ptVar3);
      uVar1 = FUN_0041b688(DAT_0121730c);
      uVar2 = lv_obj_get_child(uVar1,0);
      FUN_00437890(uVar2,0xad,1);
      uVar2 = lv_obj_get_child(uVar1,1);
      FUN_00437890(uVar2,1,1);
      lv_label_set_text(uVar2,acStack_88);
      lv_label_set_long_mode(uVar2,3);
      __n = 100;
      __s = acStack_88;
      DAT_012172f4 = uVar1;
    }
    memset(__s,0,__n);
    local_20 = DAT_0121730c;
    DAT_012172f8 = FUN_0041b504(DAT_0121730c,100,0xb,0,0xa6);
  }
LAB_0041bb8c:
  lv_group_focus_obj(DAT_0121730c);
  return;
}



/* ===== FUNCTION FUN_0041d0cc @ 0041d0cc ===== */

void FUN_0041d0cc(void)

{
  undefined4 uVar1;
  
  DAT_012171d8 = lv_group_create();
  FUN_004150d0(DAT_012171d8);
  FUN_00417d5c();
  DAT_01217320 = FUN_0042b4b4();
  DAT_012167cc = 1;
  uVar1 = lv_group_get_default();
  lv_group_focus_next(uVar1);
  return;
}



/* ===== FUNCTION FUN_0041d124 @ 0041d124 ===== */

void FUN_0041d124(void)

{
  lv_group_remove_all_objs(DAT_012171d8);
  lv_group_del(DAT_012171d8);
  FUN_00417ff0();
  return;
}



/* ===== FUNCTION FUN_0041d160 @ 0041d160 ===== */

undefined4 FUN_0041d160(void)

{
  return DAT_01217874;
}



/* ===== FUNCTION FUN_0041d16c @ 0041d16c ===== */

void FUN_0041d16c(undefined4 param_1)

{
  DAT_01217858 = param_1;
  return;
}



/* ===== FUNCTION FUN_0041d178 @ 0041d178 ===== */

undefined4 FUN_0041d178(void)

{
  return DAT_01217758;
}



/* ===== FUNCTION FUN_0041d184 @ 0041d184 ===== */

void FUN_0041d184(undefined4 param_1)

{
  DAT_01217758 = param_1;
  return;
}



/* ===== FUNCTION FUN_0041d190 @ 0041d190 ===== */

undefined4 FUN_0041d190(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  char *pcVar4;
  size_t sVar5;
  size_t sVar6;
  undefined *__dest;
  char *__src;
  char acStack_418 [1028];
  
  memset(acStack_418,0,0x400);
  piVar1 = (int *)FUN_0042b4b4();
  uVar2 = FUN_00428454(0x38);
  iVar3 = FUN_00428c28(uVar2);
  if (iVar3 == 0) {
    uVar2 = 0xffffffff;
  }
  else {
    __src = (char *)(iVar3 + 0x10);
    FUN_00410934(&DAT_0121d8d8);
    sVar6 = 0xffffffff;
    FUN_00410934(&DAT_0121dce8);
    FUN_00410934(&DAT_0121e0f8);
    FUN_00410934(&DAT_0121e508);
    FUN_00423bb8();
    DAT_01217758 = 1;
    DAT_012167c8 = FUN_00428454(0x38);
    __dest = &DAT_0121d8d8 + DAT_012167c8 * 0x410;
    *(int *)(&DAT_0121dcd8 + DAT_012167c8 * 0x410) = DAT_012167c8;
    FUN_0042e35c(*piVar1 + -1);
    memset(acStack_418,0,0x400);
    strcpy(acStack_418,__src);
    pcVar4 = strtok(acStack_418,"/");
    if (pcVar4 != (char *)0x0) {
      while (pcVar4 = strtok((char *)0x0,"/"), pcVar4 != (char *)0x0) {
        sVar6 = strlen(pcVar4);
      }
    }
    sVar5 = strlen(__src);
    memcpy(__dest,__src,(sVar5 - 1) - sVar6);
    FUN_0040ff40(__dest,__dest);
    uVar2 = 0;
    DAT_012167ec = __dest;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0041d32c @ 0041d32c ===== */

undefined4 FUN_0041d32c(void)

{
  return DAT_012167e4;
}



/* ===== FUNCTION FUN_0041d338 @ 0041d338 ===== */

undefined4 FUN_0041d338(void)

{
  return DAT_012167ec;
}



/* ===== FUNCTION FUN_0041d358 @ 0041d358 ===== */

void FUN_0041d358(int param_1,uint *param_2,int param_3)

{
  bool bVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  char *__format;
  int iVar9;
  uint uVar10;
  char acStack_450 [1024];
  char local_50 [32];
  char *local_30;
  
  memset(acStack_450,0,0x400);
  local_50[0] = '\0';
  local_50[1] = '\0';
  local_50[2] = '\0';
  local_50[3] = '\0';
  local_50[4] = '\0';
  local_50[5] = '\0';
  local_50[6] = '\0';
  local_50[7] = '\0';
  local_50[8] = '\0';
  local_50[9] = '\0';
  local_50[10] = '\0';
  local_50[0xb] = '\0';
  local_50[0xc] = '\0';
  local_50[0xd] = '\0';
  local_50[0xe] = '\0';
  local_50[0xf] = '\0';
  local_50[0x10] = '\0';
  local_50[0x11] = '\0';
  local_50[0x12] = '\0';
  local_50[0x13] = '\0';
  local_50[0x14] = '\0';
  local_50[0x15] = '\0';
  local_50[0x16] = '\0';
  local_50[0x17] = '\0';
  local_50[0x18] = '\0';
  local_50[0x19] = '\0';
  local_50[0x1a] = '\0';
  local_50[0x1b] = '\0';
  local_50[0x1c] = '\0';
  local_50[0x1d] = '\0';
  local_50[0x1e] = '\0';
  local_50[0x1f] = '\0';
  FUN_00428454(0x11);
  if (param_1 == 0x11) {
    FUN_0041a59c(3);
    uVar2 = param_2[1];
    if (uVar2 == 0) {
      uVar8 = param_2[2];
      uVar2 = param_2[3];
      if ((int)uVar8 < (int)uVar2) {
        *param_2 = 0;
        param_2[1] = uVar8 - 1;
        bVar1 = false;
        if (param_3 != DAT_012167e0) goto LAB_0041d400;
      }
      else {
        *param_2 = uVar8 - uVar2;
        param_2[1] = uVar8 - 1;
        if (param_3 != DAT_012167e0) goto LAB_0041d6b4;
        bVar1 = true;
      }
      if ((int)(uVar8 - 1) < 10) {
        uVar3 = lv_obj_get_child(DAT_01217884);
        lv_group_focus_obj(uVar3);
      }
      else {
        lv_group_focus_prev(param_3);
      }
      if (!bVar1) goto LAB_0041d8e0;
      goto LAB_0041d6b4;
    }
    if (uVar2 == *param_2) {
      *param_2 = uVar2 - 1;
      param_2[1] = uVar2 - 1;
      goto LAB_0041d6b4;
    }
    param_2[1] = uVar2 - 1;
    lv_group_focus_prev(param_3);
LAB_0041d8e0:
    uVar2 = param_2[3];
  }
  else {
    if (param_1 == 0x12) {
      FUN_0041a59c(3);
      uVar2 = param_2[1];
      if (((int)(param_2[2] - uVar2) < 2) || ((int)(uVar2 - *param_2) < (int)(param_2[3] - 1))) {
        if (param_2[2] != uVar2 + 1) {
          param_2[1] = uVar2 + 1;
          lv_group_focus_next(param_3);
          uVar2 = param_2[3];
          goto LAB_0041d400;
        }
        *param_2 = 0;
        param_2[1] = 0;
        uVar2 = 0;
        if (param_3 == DAT_012167e0) goto LAB_0041d69c;
      }
      else {
        *param_2 = *param_2 + 1;
        param_2[1] = uVar2 + 1;
      }
    }
    else if (param_1 == 0x14) {
      FUN_0041a59c(4);
      uVar2 = param_2[1];
      uVar8 = param_2[3];
      uVar6 = *param_2;
      if ((int)uVar2 < (int)uVar8) {
        if ((int)uVar6 < (int)uVar2) {
          uVar2 = 0;
          param_2[1] = 0;
          goto LAB_0041d6dc;
        }
        uVar2 = uVar2 - uVar6;
      }
      else {
        uVar2 = uVar2 - uVar8;
        param_2[1] = uVar2;
        if ((int)uVar8 < (int)uVar6) {
          *param_2 = uVar6 - uVar8;
          uVar2 = uVar2 - (uVar6 - uVar8);
        }
        else {
LAB_0041d6dc:
          *param_2 = 0;
        }
      }
LAB_0041d69c:
      uVar3 = lv_obj_get_child(DAT_01217884,uVar2);
      lv_group_focus_obj(uVar3);
    }
    else {
      if (param_1 == 0x13) {
        FUN_0041a59c(4);
        uVar8 = param_2[1];
        uVar2 = param_2[3];
        uVar6 = param_2[2];
        uVar7 = uVar8 + uVar2;
        uVar10 = uVar8 - *param_2;
        if ((int)uVar7 < (int)uVar6) {
          param_2[1] = uVar7;
          if ((int)uVar6 < (int)(uVar2 + uVar7)) {
            uVar8 = uVar6 - uVar2;
            *param_2 = uVar8;
          }
          else {
            uVar8 = *param_2 + uVar2;
            *param_2 = uVar8;
          }
        }
        else {
          uVar2 = uVar10;
          if (uVar7 != uVar6) goto LAB_0041d69c;
          uVar7 = uVar7 - 1;
          *param_2 = uVar8;
          param_2[1] = uVar7;
        }
        iVar4 = 0;
        uVar2 = uVar7 - uVar8;
        if ((int)uVar10 < (int)(uVar7 - uVar8)) {
          do {
            lv_group_focus_next(param_3);
            iVar4 = iVar4 + 1;
            uVar2 = param_2[1] - *param_2;
          } while (iVar4 < (int)(uVar2 - uVar10));
        }
        goto LAB_0041d69c;
      }
      if (param_1 != 10) {
        uVar2 = param_2[3];
        goto LAB_0041d400;
      }
      iVar4 = FUN_0040a6ec();
      uVar2 = (uint)*(ushort *)(iVar4 + 0x408);
      *(ushort *)(DAT_012167ec + 0x408) = *(ushort *)(iVar4 + 0x408);
      param_2[1] = uVar2;
      if (uVar2 == 0) {
        *param_2 = 0;
        uVar8 = 0;
      }
      else if ((int)param_2[2] < 0xb) {
        uVar8 = uVar2 - *param_2;
      }
      else {
        uVar6 = *param_2;
        uVar7 = param_2[3];
        uVar8 = uVar2 - uVar6;
        if ((int)uVar7 < (int)uVar8) {
          uVar6 = (uVar2 - uVar7) + 1;
          uVar8 = uVar2 - uVar6;
          *param_2 = uVar6;
        }
        else if (uVar8 == uVar7) {
          uVar8 = uVar2 - (uVar6 + 1);
          *param_2 = uVar6 + 1;
        }
        else if ((int)uVar2 < (int)uVar6) {
          uVar8 = 0;
          *param_2 = uVar2;
        }
      }
      uVar3 = lv_obj_get_child(DAT_01217884,uVar8);
      lv_group_focus_obj(uVar3);
    }
LAB_0041d6b4:
    if (DAT_01217874 == 0) {
      lv_obj_add_flag(DAT_01217888,1);
      if (0 < (int)param_2[3]) {
        local_30 = "nd bgm path";
        iVar4 = 0;
        do {
          memset(acStack_450,0,0x400);
          piVar5 = (int *)FUN_0040fddc(DAT_012167ec,iVar4 + DAT_0121785c);
          if (piVar5 == (int *)0x0) break;
          __format = "%d: <%s>";
          if (*piVar5 != 0) {
            __format = "%d: %s";
          }
          sprintf(acStack_450,__format,iVar4 + DAT_0121785c,(int)piVar5 + 10);
          if ((iVar4 + DAT_0121785c == 0) && (DAT_01217874 == 0)) {
            uVar3 = lv_obj_get_child(DAT_01217884,0);
            uVar3 = lv_obj_get_child(uVar3,1);
            lv_label_set_text(uVar3,local_30 + -0xe88);
            uVar3 = lv_obj_get_child(DAT_01217884,0);
            uVar3 = lv_obj_get_child(uVar3,1);
            lv_label_set_long_mode(uVar3,1);
            lv_obj_clear_flag(DAT_01217888,1);
            uVar2 = param_2[3];
          }
          else {
            uVar3 = lv_obj_get_child(DAT_01217884,iVar4);
            uVar3 = lv_obj_get_child(uVar3,1);
            lv_label_set_text(uVar3,acStack_450);
            uVar3 = lv_obj_get_child(DAT_01217884,iVar4);
            uVar3 = lv_obj_get_child(uVar3,1);
            lv_label_set_long_mode(uVar3,1);
            uVar2 = param_2[3];
          }
          iVar4 = iVar4 + 1;
        } while (iVar4 < (int)uVar2);
      }
      goto LAB_0041d444;
    }
    uVar2 = param_2[3];
  }
LAB_0041d400:
  iVar4 = 0;
  if (0 < (int)uVar2) {
    do {
      iVar9 = iVar4 + 1;
      uVar3 = lv_obj_get_child(DAT_01217884,iVar4);
      uVar3 = lv_obj_get_child(uVar3,1);
      lv_label_set_long_mode(uVar3,1);
      iVar4 = iVar9;
    } while (iVar9 < (int)param_2[3]);
  }
LAB_0041d444:
  local_50[0] = '\0';
  local_50[1] = '\0';
  local_50[2] = '\0';
  local_50[3] = '\0';
  local_50[4] = '\0';
  local_50[5] = '\0';
  local_50[6] = '\0';
  local_50[7] = '\0';
  local_50[8] = '\0';
  local_50[9] = '\0';
  local_50[10] = '\0';
  local_50[0xb] = '\0';
  local_50[0xc] = '\0';
  local_50[0xd] = '\0';
  local_50[0xe] = '\0';
  local_50[0xf] = '\0';
  local_50[0x10] = '\0';
  local_50[0x11] = '\0';
  local_50[0x12] = '\0';
  local_50[0x13] = '\0';
  local_50[0x14] = '\0';
  local_50[0x15] = '\0';
  local_50[0x16] = '\0';
  local_50[0x17] = '\0';
  local_50[0x18] = '\0';
  local_50[0x19] = '\0';
  local_50[0x1a] = '\0';
  local_50[0x1b] = '\0';
  local_50[0x1c] = '\0';
  local_50[0x1d] = '\0';
  local_50[0x1e] = '\0';
  local_50[0x1f] = '\0';
  if (DAT_01217874 == 0) {
    if (param_2[1] == 0) {
      lv_label_set_text(DAT_0121787c,&DAT_0044f178);
    }
    else {
      sprintf(local_50,"%d / %d",param_2[1],
              (uint)*(ushort *)(DAT_012167ec + 0x406) + (uint)*(ushort *)(DAT_012167ec + 0x404) + -1
             );
      lv_label_set_text(DAT_0121787c,local_50);
    }
  }
  uVar3 = DAT_01217884;
  *(short *)(DAT_012167ec + 0x408) = (short)param_2[1];
  uVar3 = lv_obj_get_child(uVar3,param_2[1] - *param_2);
  uVar3 = lv_obj_get_child(uVar3,1);
  lv_label_set_long_mode(uVar3,2);
  return;
}



/* ===== FUNCTION FUN_0041da2c @ 0041da2c ===== */

int FUN_0041da2c(void)

{
  size_t sVar1;
  int iVar2;
  void *__s2;
  
  __s2 = (void *)(DAT_012167ec + 7);
  if (DAT_012177ac._1_1_ != '\0') {
    sVar1 = strlen(&DAT_0121778d);
    iVar2 = memcmp(&DAT_0121778d,__s2,sVar1);
    if (iVar2 == 0) {
      return 1;
    }
  }
  if (DAT_012177d4._2_1_ != '\0') {
    sVar1 = strlen(&DAT_012177b6);
    iVar2 = memcmp(&DAT_012177b6,__s2,sVar1);
    if (iVar2 == 0) {
      return 2;
    }
  }
  if (DAT_012177ff != '\0') {
    sVar1 = strlen(&DAT_012177df);
    iVar2 = memcmp(&DAT_012177df,__s2,sVar1);
    if (iVar2 == 0) {
      return 3;
    }
  }
  if ((char)DAT_01217828 != '\0') {
    sVar1 = strlen(&DAT_01217808);
    iVar2 = memcmp(&DAT_01217808,__s2,sVar1);
    if (iVar2 == 0) {
      return 4;
    }
  }
  if (DAT_01217851 != '\0') {
    sVar1 = strlen(&DAT_01217831);
    iVar2 = memcmp(&DAT_01217831,__s2,sVar1);
    if (iVar2 == 0) {
      iVar2 = 5;
    }
    return iVar2;
  }
  return -1;
}



/* ===== FUNCTION FUN_0041db84 @ 0041db84 ===== */

void FUN_0041db84(uint param_1)

{
  if (5 < param_1) {
    return;
  }
  memset((void *)((int)&DAT_0121775c + param_1 * 0x29),0,0x29);
  return;
}



/* ===== FUNCTION FUN_0041dce4 @ 0041dce4 ===== */

void FUN_0041dce4(void)

{
  undefined4 uVar1;
  undefined4 local_18;
  code *local_14;
  
  DAT_012167d8 = lv_obj_create(0);
  lv_obj_clear_flag(DAT_012167d8,0x10);
  lv_obj_add_event_cb(DAT_012167d8,&LAB_0041f408,0,0);
  lv_obj_set_style_bg_color(DAT_012167d8,0xff031fff,0);
  lv_obj_set_style_bg_opa(DAT_012167d8,0xff,0);
  uVar1 = DAT_012167d8;
  lv_obj_set_style_pad_left(DAT_012167d8,0,0);
  lv_obj_set_style_pad_right(uVar1,0,0);
  lv_obj_set_style_pad_top(uVar1,0,0);
  lv_obj_set_style_pad_bottom(uVar1,0,0);
  lv_obj_set_style_border_width(DAT_012167d8,0,0);
  local_18 = DAT_012167d8;
  local_14 = FUN_0041fe5c;
  FUN_0042cef8(&local_18);
  return;
}



/* ===== FUNCTION FUN_0041ddd0 @ 0041ddd0 ===== */

void FUN_0041ddd0(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = lv_obj_is_valid(DAT_01217878);
  if ((iVar1 == 0) || (DAT_01217878 == 0)) {
    DAT_01217878 = lv_img_create(param_1);
  }
  else {
    lv_obj_clear_flag(DAT_01217878,1);
  }
  lv_obj_align(DAT_01217878,9,5,9);
  lv_img_set_src(DAT_01217878,param_2);
  return;
}



/* ===== FUNCTION FUN_0041dea4 @ 0041dea4 ===== */

void FUN_0041dea4(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  int iVar3;
  void *__s1;
  
  piVar2 = (int *)FUN_0040fddc(DAT_012167ec,param_1);
  uVar1 = DAT_012167dc;
  if (piVar2 == (int *)0x0) {
    return;
  }
  if (DAT_01217874 == 0) {
    if (*piVar2 == 0) {
      if (param_1 != 0) {
        iVar3 = lv_obj_is_valid(DAT_01217878);
        if ((iVar3 == 0) || (DAT_01217878 == 0)) {
          DAT_01217878 = lv_img_create(uVar1);
        }
        else {
          lv_obj_clear_flag(DAT_01217878,1);
        }
        lv_obj_align(DAT_01217878,9,5,9);
        lv_img_set_src(DAT_01217878,&DAT_00874fa0);
        DAT_01217870 = 0;
        return;
      }
      iVar3 = lv_obj_is_valid(DAT_01217878);
      if ((iVar3 == 0) || (DAT_01217878 == 0)) {
        DAT_01217878 = lv_img_create(uVar1);
      }
      else {
        lv_obj_clear_flag(DAT_01217878,1);
      }
      lv_obj_align(DAT_01217878,9,5,9);
      lv_img_set_src(DAT_01217878,&DAT_0071de00);
    }
    else {
      if (*piVar2 == 2) {
        if (DAT_01217870 != '\0') {
          DAT_01217870 = 0;
          return;
        }
        iVar3 = lv_obj_is_valid(DAT_01217878);
        if ((iVar3 == 0) || (DAT_01217878 == 0)) {
          DAT_01217878 = lv_img_create(uVar1);
        }
        else {
          lv_obj_clear_flag(DAT_01217878,1);
        }
        lv_obj_align(DAT_01217878,9,5,9);
        lv_img_set_src(DAT_01217878,&DAT_00829f80);
        DAT_01217870 = 0;
        return;
      }
      iVar3 = lv_obj_is_valid(DAT_01217878);
      if ((iVar3 != 0) && (DAT_01217878 != 0)) {
        lv_obj_add_flag(DAT_01217878,1);
        DAT_01217870 = 0;
        return;
      }
    }
  }
  else {
    __s1 = (void *)((int)piVar2 + 10);
    iVar3 = memcmp(__s1,&PTR_DAT_0044fa2c,3);
    uVar1 = DAT_012167dc;
    if (iVar3 == 0) {
      iVar3 = lv_obj_is_valid(DAT_01217878);
      if ((iVar3 == 0) || (DAT_01217878 == 0)) {
        DAT_01217878 = lv_img_create(uVar1);
      }
      else {
        lv_obj_clear_flag(DAT_01217878,1);
      }
      lv_obj_align(DAT_01217878,9,5,9);
      lv_img_set_src(DAT_01217878,&DAT_007c3800);
      DAT_01217870 = 0;
      return;
    }
    iVar3 = memcmp(__s1,&DAT_0044fef0,2);
    if (((iVar3 == 0) || (iVar3 = memcmp(__s1,&DAT_0044fef4,2), iVar3 == 0)) ||
       (iVar3 = memcmp(__s1,&PTR_DAT_0044fef8,3), iVar3 == 0)) {
      uVar1 = DAT_012167dc;
      iVar3 = lv_obj_is_valid(DAT_01217878);
      if ((iVar3 == 0) || (DAT_01217878 == 0)) {
        DAT_01217878 = lv_img_create(uVar1);
      }
      else {
        lv_obj_clear_flag(DAT_01217878,1);
      }
      lv_obj_align(DAT_01217878,9,5,9);
      lv_img_set_src(DAT_01217878,&DAT_008c2600);
      DAT_01217870 = 0;
      return;
    }
  }
  DAT_01217870 = 0;
  return;
}



/* ===== FUNCTION FUN_0041e1e4 @ 0041e1e4 ===== */

void FUN_0041e1e4(int param_1)

{
  undefined *puVar1;
  undefined4 uVar2;
  int iVar3;
  size_t sVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  char *__format;
  int iVar8;
  char *pcVar9;
  void *__s1;
  void *__s2;
  char acStack_878 [1024];
  char acStack_478 [1024];
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 *local_58;
  undefined *local_54;
  undefined *local_50;
  undefined *local_4c;
  char *local_48;
  undefined4 local_44;
  char *local_40;
  char *local_3c;
  int local_38;
  undefined *local_34;
  char *local_30;
  undefined4 local_2c;
  
  iVar8 = 0;
  memset(acStack_478,0,0x400);
  local_78 = 0;
  local_74 = 0;
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  lv_style_init(&DAT_01217750);
  lv_style_set_border_width(&DAT_01217750,0);
  lv_style_set_pad_left(&DAT_01217750,5);
  DAT_01217860 = (uint)*(ushort *)(param_1 + 0x408);
  local_54 = &DAT_01210000;
  DAT_01217864 = (uint)*(ushort *)(DAT_012167ec + 0x406) + (uint)*(ushort *)(DAT_012167ec + 0x404);
  uVar2 = lv_obj_get_child(DAT_01217884,DAT_0121785c);
  lv_group_focus_obj(uVar2);
  local_3c = "nd bgm path";
  DAT_01217868 = 10;
  if (DAT_01217864 < 10) {
    DAT_01217868 = DAT_01217864 + 1;
  }
  do {
    uVar2 = lv_obj_get_child(DAT_01217884,iVar8);
    uVar2 = lv_obj_get_child(uVar2,0);
    lv_label_set_text(uVar2,&DAT_0044f178);
    uVar2 = lv_obj_get_child(DAT_01217884,iVar8);
    uVar2 = lv_obj_get_child(uVar2,1);
    lv_label_set_text(uVar2,&DAT_0044f178);
    uVar2 = lv_obj_get_child(DAT_01217884,iVar8);
    uVar2 = lv_obj_get_child(uVar2,1);
    iVar3 = lv_obj_get_child(uVar2,0);
    if (iVar3 != 0) {
      uVar2 = lv_obj_get_child(DAT_01217884,iVar8);
      uVar2 = lv_obj_get_child(uVar2,1);
      uVar2 = lv_obj_get_child(uVar2,0);
      lv_obj_add_flag(uVar2,1);
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 != 10);
  if (DAT_01217860 < 10) {
    DAT_0121785c = 0;
LAB_0041e3a0:
    if (DAT_01217888 != 0) goto LAB_0041e3b4;
LAB_0041ea20:
    local_4c = &DAT_01210000;
    uVar2 = lv_obj_get_child(DAT_01217884,0);
    uVar2 = lv_obj_get_child(uVar2,1);
    uVar2 = lv_img_create(uVar2);
    puVar1 = local_4c;
    *(undefined4 *)(local_4c + 0x7888) = uVar2;
    lv_img_set_src(uVar2,&DAT_00770640);
    lv_obj_set_style_transform_zoom(*(undefined4 *)(puVar1 + 0x7888),0xa0,0);
  }
  else {
    if ((DAT_01217864 < 0xb) ||
       (DAT_0121785c = DAT_01217864 + -10, (int)(DAT_01217864 - DAT_01217860) < 5))
    goto LAB_0041e3a0;
    DAT_0121785c = DAT_01217860 - 5;
    if (DAT_01217888 == 0) goto LAB_0041ea20;
LAB_0041e3b4:
    local_4c = &DAT_01210000;
    lv_obj_add_flag(DAT_01217888,1);
  }
  iVar8 = 0;
  pcVar9 = *(char **)(local_54 + 0x67ec);
  sVar4 = strlen(pcVar9);
  local_40 = "nd bgm path";
  iVar3 = memcmp(pcVar9,"/media",sVar4);
  DAT_01217874 = (uint)(iVar3 == 0);
  iVar3 = 0;
  DAT_01217870 = 1;
  FUN_0041dea4(*(undefined2 *)(pcVar9 + 0x408));
  local_30 = "nd bgm path";
  local_48 = "nd bgm path";
  local_2c = 0x28425355;
  local_44 = 0x43284654;
  local_50 = &DAT_01210000;
  local_34 = &DAT_01210000;
  do {
    piVar5 = (int *)FUN_0040fddc(param_1,iVar3 + DAT_0121785c);
    if (piVar5 == (int *)0x0) break;
    memset(acStack_478,0,0x400);
    pcVar9 = (char *)((int)piVar5 + 10);
    if (*piVar5 == 0) {
      __format = "%d: <%s>";
    }
    else {
      __format = "%d: %s";
    }
    sprintf(acStack_478,__format,iVar3 + DAT_0121785c,pcVar9);
    if (iVar3 + DAT_0121785c == 0) {
      if (DAT_01217874 == 0) {
        uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
        uVar2 = lv_obj_get_child(uVar2,1);
        iVar6 = lv_obj_get_child(uVar2,0);
        if (iVar6 != 0) {
          uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
          uVar2 = lv_obj_get_child(uVar2,1);
          uVar2 = lv_obj_get_child(uVar2,0);
          lv_obj_add_flag(uVar2,1);
        }
        puVar1 = local_4c;
        lv_img_set_src(*(undefined4 *)(local_4c + 0x7888),&DAT_00770640);
        lv_obj_clear_flag(*(undefined4 *)(puVar1 + 0x7888),1);
      }
      else {
        if (DAT_01217874 != 1) goto LAB_0041e7e8;
LAB_0041e4ac:
        iVar6 = memcmp(pcVar9,&PTR_DAT_0044fa2c,3);
        if (iVar6 == 0) {
          uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
          uVar2 = lv_obj_get_child(uVar2,1);
          iVar6 = lv_obj_get_child(uVar2,0);
          if (iVar6 == 0) {
            uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
            uVar2 = lv_obj_get_child(uVar2,1);
            uVar2 = lv_img_create(uVar2);
          }
          else {
            uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
            uVar2 = lv_obj_get_child(uVar2,1);
            uVar2 = lv_obj_get_child(uVar2,0);
          }
          sVar4 = strlen(pcVar9);
          strncpy(&DAT_01217764,pcVar9,sVar4);
          uVar7 = *(undefined4 *)(local_48 + 0x360);
          *(undefined4 *)(local_50 + 0x775c) = local_44;
          local_50[0x7784] = 1;
          *(undefined4 *)(local_50 + 0x7760) = uVar7;
          lv_img_set_src(uVar2,&DAT_005ae4c0);
          lv_obj_align(uVar2,7,0,0x200f);
        }
        else {
          iVar6 = memcmp(pcVar9,&DAT_0044fef0,2);
          if (((iVar6 != 0) && (iVar6 = memcmp(pcVar9,&DAT_0044fef4,2), iVar6 != 0)) &&
             (iVar6 = memcmp(pcVar9,&PTR_DAT_0044fef8,3), iVar6 != 0)) goto LAB_0041e664;
          if (5 < iVar3) {
            local_78 = 0;
            local_74 = 0;
            local_70 = 0;
            local_6c = 0;
            local_68 = 0;
            local_64 = 0;
            local_60 = 0;
            local_5c = 0;
            goto LAB_0041eab0;
          }
          uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
          uVar2 = lv_obj_get_child(uVar2,1);
          iVar6 = lv_obj_get_child(uVar2,0);
          if (iVar6 == 0) {
            uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
            uVar2 = lv_obj_get_child(uVar2,1);
            uVar2 = lv_img_create(uVar2);
          }
          else {
            uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
            uVar2 = lv_obj_get_child(uVar2,1);
            uVar2 = lv_obj_get_child(uVar2,0);
          }
          iVar6 = iVar8 + 1;
          local_58 = (undefined4 *)(local_50 + 0x775c);
          local_38 = iVar6 * 4;
          lv_img_set_src(uVar2,(&PTR_DAT_01214464)[iVar8]);
          lv_obj_align(uVar2,7,0,0x200f);
          sVar4 = strlen(pcVar9);
          strncpy((char *)((int)local_58 + iVar6 * 0x29 + 8),pcVar9,sVar4);
          switch(iVar6) {
          case 1:
            DAT_01217784 = DAT_01217784 & 0xff | 0x42535500;
            DAT_01217788 = 0x293a4428;
            uRam0121778c = 0;
            break;
          case 2:
            DAT_012177b0._0_2_ = 0x2842;
            DAT_012177ac = DAT_012177ac & 0xffff | 0x53550000;
            DAT_012177b0._2_2_ = 0x3a45;
            uRam012177b4 = 0x29;
            break;
          case 3:
            DAT_012177d4 = DAT_012177d4 & 0xffffff | 0x55000000;
            DAT_012177d8 = 0x46284253;
            uRam012177dc = 0x293a;
            break;
          case 4:
            uVar7 = *(undefined4 *)(local_30 + 0x380);
            *(undefined4 *)(local_34 + 0x7800) = local_2c;
            *(undefined4 *)(local_34 + 0x7804) = uVar7;
            break;
          case 5:
            DAT_01217828 = DAT_01217828 & 0xff | 0x42535500;
            DAT_0121782c = 0x293a4828;
            uRam01217830 = 0;
          }
          *(undefined1 *)((int)local_58 + iVar6 * 0x29 + 0x28) = 1;
          iVar8 = iVar6;
        }
        lv_obj_clear_flag(uVar2,1);
        lv_obj_set_style_transform_zoom(uVar2,0xa0,0);
      }
    }
    else {
      if (DAT_01217874 == 1) goto LAB_0041e4ac;
LAB_0041e7e8:
      uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
      uVar2 = lv_obj_get_child(uVar2,1);
      iVar6 = lv_obj_get_child(uVar2,0);
      if (iVar6 != 0) {
        uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
        uVar2 = lv_obj_get_child(uVar2,1);
        uVar2 = lv_obj_get_child(uVar2,0);
        lv_obj_add_flag(uVar2,1);
      }
      uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
      uVar2 = lv_obj_get_child(uVar2,1);
      lv_label_set_text(uVar2,acStack_478);
      uVar2 = lv_obj_get_child(DAT_01217884,iVar3);
      uVar2 = lv_obj_get_child(uVar2,1);
      lv_label_set_long_mode(uVar2,1);
    }
LAB_0041e664:
    iVar3 = iVar3 + 1;
  } while (iVar3 != 10);
  local_78 = 0;
  local_74 = 0;
  local_70 = 0;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  if (DAT_01217874 == 0) {
    iVar8 = *(int *)(local_54 + 0x67ec);
    if (*(ushort *)(iVar8 + 0x408) == 0) {
      lv_label_set_text(DAT_0121787c,local_3c + -0xe88);
    }
    else {
      sprintf((char *)&local_78,"%d / %d",(uint)*(ushort *)(iVar8 + 0x408),
              (uint)*(ushort *)(iVar8 + 0x406) + (uint)*(ushort *)(iVar8 + 0x404) + -1);
      lv_label_set_text(DAT_0121787c,&local_78);
    }
    if (DAT_01217874 == 0) {
      memset(acStack_878,0,0x400);
      iVar8 = FUN_0042b4b4();
      __s1 = *(void **)(local_54 + 0x67ec);
      iVar3 = memcmp(__s1,"/media/mmc",10);
      if (iVar3 == 0) {
        pcVar9 = (char *)&DAT_0121775c;
      }
      else {
        local_58 = &DAT_0121775c;
        __s2 = (void *)((int)__s1 + 7);
        if (DAT_012177ac._1_1_ == '\0') {
LAB_0041e75c:
          if (*(char *)((int)local_58 + 0x7a) != '\0') {
            sVar4 = strlen(&DAT_012177b6);
            iVar3 = memcmp(&DAT_012177b6,__s2,sVar4);
            if (iVar3 == 0) {
              iVar3 = 0x52;
              goto LAB_0041ebf8;
            }
          }
          if (*(char *)((int)local_58 + 0xa3) != '\0') {
            sVar4 = strlen(&DAT_012177df);
            iVar3 = memcmp(&DAT_012177df,__s2,sVar4);
            if (iVar3 == 0) {
              iVar3 = 0x7b;
              goto LAB_0041ebf8;
            }
          }
          if (*(char *)(local_58 + 0x33) != '\0') {
            sVar4 = strlen(&DAT_01217808);
            iVar3 = memcmp(&DAT_01217808,__s2,sVar4);
            if (iVar3 == 0) {
              iVar3 = 0xa4;
              goto LAB_0041ebf8;
            }
          }
          if (*(char *)((int)local_58 + 0xf5) != '\0') {
            sVar4 = strlen(&DAT_01217831);
            iVar6 = memcmp(&DAT_01217831,__s2,sVar4);
            iVar3 = 0xcd;
            if (iVar6 == 0) goto LAB_0041ebf8;
          }
          lv_label_set_text(DAT_0121788c,local_3c + -0xe88);
          goto LAB_0041eac8;
        }
        sVar4 = strlen(&DAT_0121778d);
        iVar3 = memcmp(&DAT_0121778d,__s2,sVar4);
        if (iVar3 != 0) goto LAB_0041e75c;
        iVar3 = 0x29;
LAB_0041ebf8:
        pcVar9 = (char *)((int)local_58 + iVar3);
      }
      strncpy(acStack_878,pcVar9,8);
      sVar4 = strlen(*(char **)(iVar8 + 8));
      strcat(acStack_878,(char *)((int)__s1 + sVar4));
      lv_label_set_text(DAT_0121788c,acStack_878);
      goto LAB_0041eac8;
    }
  }
LAB_0041eab0:
  lv_label_set_text(DAT_0121788c,local_40 + -0x5dc);
LAB_0041eac8:
  uVar2 = lv_obj_get_child(DAT_01217884,DAT_01217860 - DAT_0121785c);
  lv_group_focus_obj(uVar2);
  return;
}



/* ===== FUNCTION FUN_0041fe5c @ 0041fe5c ===== */

undefined4 FUN_0041fe5c(int *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_138 [276];
  int local_24;
  
  if (((param_1 == (int *)0x0) || (DAT_012167e4 == 0)) || (DAT_012167ec == 0)) {
    printf("msg error! %s,%d\n","media_list_msg_handle",0x487);
    uVar1 = 0xffffffff;
  }
  else {
    if (*param_1 == 10) {
      if ((uint)*(ushort *)(DAT_012167e4 + 0x448) != (param_1[1] & 0xffffU)) {
        return 0;
      }
      if ((uint)param_1[1] >> 0x10 == 3) {
        if (*(int *)(DAT_012167ec + 0x400) != 1) {
          return 0;
        }
        iVar2 = FUN_0040f108();
        if (iVar2 == 1) {
          memset(auStack_138,0,0x130);
          FUN_0042536c(DAT_012167e4,auStack_138);
          if (local_24 == 0) {
            FUN_0042e164(0);
            FUN_0042e5b8();
            FUN_0041dea4(*(undefined2 *)(DAT_012167ec + 0x408));
            return 0;
          }
          iVar2 = lv_obj_is_valid(DAT_01217878);
          if ((iVar2 != 0) && (DAT_01217878 != 0)) {
            lv_obj_add_flag(DAT_01217878,1);
          }
          return 0;
        }
      }
    }
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0041ff18 @ 0041ff18 ===== */

undefined4 FUN_0041ff18(void)

{
  FUN_00410934(&DAT_0121d8d8);
  FUN_00410934(&DAT_0121dce8);
  FUN_00410934(&DAT_0121e0f8);
  FUN_00410934(&DAT_0121e508);
  return 0;
}



/* ===== FUNCTION FUN_0041ff60 @ 0041ff60 ===== */

bool FUN_0041ff60(char *param_1)

{
  int iVar1;
  
  iVar1 = strcmp(param_1,"/media");
  return iVar1 == 0;
}



/* ===== FUNCTION FUN_0041ff84 @ 0041ff84 ===== */

bool FUN_0041ff84(char *param_1)

{
  bool bVar1;
  int iVar2;
  
  iVar2 = FUN_0042b4b4();
  bVar1 = false;
  if (iVar2 != 0) {
    iVar2 = strcmp(param_1,*(char **)(iVar2 + 8));
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}



/* ===== FUNCTION FUN_0041ffc0 @ 0041ffc0 ===== */

void FUN_0041ffc0(char *param_1,char *param_2)

{
  char cVar1;
  char *pcVar2;
  uint uVar3;
  char *pcVar4;
  
  pcVar2 = stpcpy(param_1,param_2);
  pcVar4 = param_1 + ((int)pcVar2 - (int)param_1 & 0xffff);
  uVar3 = (int)pcVar2 - (int)param_1 & 0xffff;
  cVar1 = *pcVar4;
  while (cVar1 != '/') {
    if (uVar3 == 0) goto LAB_00420038;
    uVar3 = uVar3 - 1 & 0xffff;
    pcVar4 = param_1 + uVar3;
    cVar1 = *pcVar4;
  }
  if (uVar3 != 0) {
    *pcVar4 = '\0';
    return;
  }
LAB_00420038:
  param_1[1] = '\0';
  return;
}



/* ===== FUNCTION FUN_0042004c @ 0042004c ===== */

short FUN_0042004c(int param_1,char *param_2)

{
  short sVar1;
  int iVar2;
  int *piVar3;
  char acStack_420 [1036];
  
  memset(acStack_420,0,0x401);
  piVar3 = *(int **)(param_1 + 0x40c);
  sVar1 = 0;
  while( true ) {
    if (piVar3 == (int *)0x0) {
      return -1;
    }
    FUN_0040fda8(acStack_420,param_1,*piVar3 + 10);
    iVar2 = strcmp(param_2,acStack_420);
    if (iVar2 == 0) break;
    piVar3 = (int *)piVar3[1];
    sVar1 = sVar1 + 1;
  }
  return sVar1;
}



/* ===== FUNCTION FUN_0042010c @ 0042010c ===== */

int FUN_0042010c(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0040fddc(param_1,*(undefined2 *)(param_1 + 0x408));
  return iVar1 + 10;
}



/* ===== FUNCTION FUN_0042012c @ 0042012c ===== */

undefined * FUN_0042012c(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_0040fddc(param_1,*(undefined2 *)(param_1 + 0x408));
  FUN_0040fda8(&DAT_01217890,param_1,iVar1 + 10);
  return &DAT_01217890;
}



/* ===== FUNCTION FUN_00420174 @ 00420174 ===== */

undefined * FUN_00420174(int param_1)

{
  ushort uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int *piVar4;
  int iVar5;
  short sVar6;
  uint uVar7;
  uint uVar8;
  
  sVar6 = *(short *)(param_1 + 0x408);
  uVar3 = *(undefined4 *)(param_1 + 0x40c);
  if (sVar6 == 0) {
    printf("%s(), line:%d. index: %d, reversed!\n","win_media_get_pre_file",0x7a,0);
    sVar6 = *(short *)(param_1 + 0x404) + *(short *)(param_1 + 0x406);
  }
  uVar7 = (uint)(ushort)(sVar6 - 1);
  for (puVar2 = (undefined4 *)glist_nth(uVar3,uVar7); puVar2 != (undefined4 *)0x0;
      puVar2 = (undefined4 *)puVar2[2]) {
    piVar4 = (int *)*puVar2;
    iVar5 = (int)piVar4 + 10;
    uVar8 = uVar7;
    printf("%s(), line:%d. prev file: %s, index:%d\n","win_media_get_pre_file",0x84,iVar5,uVar7);
    uVar3 = 0x88;
    if (*piVar4 != 0) goto LAB_00420218;
  }
  puVar2 = (undefined4 *)glist_last(*(undefined4 *)(param_1 + 0x40c));
  while( true ) {
    if (puVar2 == (undefined4 *)0x0) {
      printf("%s(), line:%d. not fond pre file!\n","win_media_get_pre_file",0xa3);
      return (undefined *)0x0;
    }
    piVar4 = (int *)*puVar2;
    iVar5 = (int)piVar4 + 10;
    uVar8 = uVar7;
    printf("%s(), line:%d. prev file: %s, index:%d\n","win_media_get_pre_file",0x95,iVar5,uVar7);
    uVar3 = 0x99;
    if (*piVar4 != 0) break;
    puVar2 = (undefined4 *)puVar2[2];
  }
LAB_00420218:
  printf("%s(), line:%d. found prev file: %s\n","win_media_get_pre_file",uVar3,iVar5,uVar8);
  FUN_0040fda8(&DAT_01217890,param_1,iVar5);
  uVar1 = glist_index(*(undefined4 *)(param_1 + 0x40c),piVar4);
  *(ushort *)(param_1 + 0x408) = uVar1;
  printf("%s(), file:%s, index:%d\n","win_media_get_pre_file",&DAT_01217890,(uint)uVar1);
  return &DAT_01217890;
}



/* ===== FUNCTION FUN_0042036c @ 0042036c ===== */

undefined * FUN_0042036c(int param_1)

{
  undefined2 uVar1;
  undefined4 *puVar2;
  uint uVar3;
  int *piVar4;
  
  if ((uint)*(ushort *)(param_1 + 0x408) ==
      ((uint)*(ushort *)(param_1 + 0x404) + (uint)*(ushort *)(param_1 + 0x406)) - 1) {
    uVar3 = 0;
  }
  else {
    uVar3 = *(ushort *)(param_1 + 0x408) + 1 & 0xffff;
  }
  puVar2 = (undefined4 *)glist_nth(*(undefined4 *)(param_1 + 0x40c),uVar3);
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = *(undefined4 **)(param_1 + 0x40c);
  }
  else {
    do {
      piVar4 = (int *)*puVar2;
      if (*piVar4 != 0) goto LAB_004203e0;
      puVar2 = (undefined4 *)puVar2[1];
    } while (puVar2 != (undefined4 *)0x0);
    puVar2 = *(undefined4 **)(param_1 + 0x40c);
  }
  while( true ) {
    if (puVar2 == (undefined4 *)0x0) {
      return (undefined *)0x0;
    }
    piVar4 = (int *)*puVar2;
    if (*piVar4 != 0) break;
    puVar2 = (undefined4 *)puVar2[1];
  }
LAB_004203e0:
  FUN_0040fda8(&DAT_01217890,param_1,(int)piVar4 + 10);
  uVar1 = glist_index(*(undefined4 *)(param_1 + 0x40c),piVar4);
  *(undefined2 *)(param_1 + 0x408) = uVar1;
  return &DAT_01217890;
}



/* ===== FUNCTION FUN_0042084c @ 0042084c ===== */

undefined4 FUN_0042084c(void)

{
  return DAT_01217ca0;
}



/* ===== FUNCTION FUN_00420858 @ 00420858 ===== */

undefined4 FUN_00420858(void)

{
  DAT_01217ca8 = 1;
  if (DAT_01217ca4 != 0) {
    pthread_join(DAT_01217ca4,(void **)0x0);
  }
  DAT_01217ca4 = 0;
  printf("\n>>> %s\n\n","music_spectrum_stop");
  return 0;
}



/* ===== FUNCTION FUN_004208b0 @ 004208b0 ===== */

undefined4 FUN_004208b0(void)

{
  int iVar1;
  pthread_attr_t pStack_38;
  
  if (DAT_01217ca4 != 0) {
    DAT_01217ca8 = 1;
    pthread_join(DAT_01217ca4,(void **)0x0);
    DAT_01217ca4 = 0;
    printf("\n>>> %s\n\n","music_spectrum_stop");
  }
  DAT_01217ca8 = 0;
  pthread_attr_init(&pStack_38);
  pthread_attr_setstacksize(&pStack_38,0x1000);
  iVar1 = pthread_create(&DAT_01217ca4,&pStack_38,(__start_routine *)&LAB_00420470,(void *)0x0);
  if (iVar1 != 0) {
    puts("create i2so spectrum thread failed");
  }
  printf("\n>>> %s\n\n","music_spectrum_start");
  pthread_attr_destroy(&pStack_38);
  return 0;
}



/* ===== FUNCTION FUN_004209bc @ 004209bc ===== */

void FUN_004209bc(void)

{
  FUN_00425478(0);
  if (DAT_01217cc4 != 0) {
    lv_timer_pause();
  }
  DAT_01217cb0 = 1;
  lv_obj_clear_flag(DAT_01217cb4,1);
  FUN_0042ef78(1);
  puts("Screen locked!");
  return;
}



/* ===== FUNCTION FUN_00420a1c @ 00420a1c ===== */

void FUN_00420a1c(void)

{
  int iVar1;
  
  if (DAT_01217cc0 != 0) {
    lv_timer_del();
    DAT_01217cc0 = 0;
  }
  lv_obj_add_flag(DAT_01217cb4,1);
  iVar1 = FUN_0042f374();
  if (iVar1 == 0) {
    FUN_00425478(1);
  }
  DAT_01217cb0 = 0;
  if (DAT_01217cc4 != 0) {
    lv_timer_reset();
    lv_timer_resume(DAT_01217cc4);
  }
  FUN_0042ef78(0);
  puts("Screen unlocked successfully!");
  if (DAT_01217cc8 != 0) {
    lv_timer_del();
    DAT_01217cc8 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_00420ad8 @ 00420ad8 ===== */

void FUN_00420ad8(void)

{
  int iVar1;
  
  if (DAT_01217cc0 == 0) {
    DAT_01217cc0 = lv_timer_create(&LAB_00420980,3000,0);
  }
  else {
    lv_timer_reset();
  }
  iVar1 = FUN_0042f374();
  if (iVar1 != 0) {
    return;
  }
  FUN_00425478(1);
  return;
}



/* ===== FUNCTION FUN_00420b40 @ 00420b40 ===== */

void FUN_00420b40(void)

{
  if (DAT_01217cc4 != 0) {
    lv_timer_reset();
    return;
  }
  return;
}



/* ===== FUNCTION FUN_00420b60 @ 00420b60 ===== */

void FUN_00420b60(void)

{
  int iVar1;
  
  if (DAT_01217cc0 != 0) {
    lv_timer_del();
    DAT_01217cc0 = 0;
  }
  lv_obj_add_flag(DAT_01217cb4,1);
  iVar1 = FUN_0042f374();
  if (iVar1 == 0) {
    FUN_00425478(1);
  }
  DAT_01217cb0 = 0;
  if (DAT_01217cc4 != 0) {
    lv_timer_reset();
    lv_timer_resume(DAT_01217cc4);
  }
  FUN_0042ef78(0);
  puts("Screen unlocked successfully!");
  return;
}



/* ===== FUNCTION FUN_00420c08 @ 00420c08 ===== */

undefined1 FUN_00420c08(void)

{
  return DAT_01217cb0;
}



/* ===== FUNCTION FUN_00420c14 @ 00420c14 ===== */

void FUN_00420c14(void)

{
  int iVar1;
  
  iVar1 = FUN_0042f374();
  if (iVar1 != 0) {
    return;
  }
  lv_timer_ready(DAT_01217cc4);
  return;
}



/* ===== FUNCTION FUN_00420c44 @ 00420c44 ===== */

void FUN_00420c44(void)

{
  float fVar1;
  undefined4 uVar2;
  uint uVar3;
  undefined4 uVar4;
  uint uVar5;
  float fVar6;
  undefined1 auStack_28 [16];
  
  uVar2 = FUN_0041d32c();
  uVar3 = FUN_004250c0(uVar2);
  if (uVar3 != 0) {
    FUN_00411af4(uVar3,auStack_28);
    uVar4 = lv_obj_get_child(DAT_01217cbc,7);
    lv_label_set_text(uVar4,auStack_28);
    uVar5 = FUN_004251bc(uVar2);
    FUN_00411af4(uVar5,auStack_28);
    uVar2 = lv_obj_get_child(DAT_01217cbc,8);
    lv_label_set_text(uVar2,auStack_28);
    if (uVar3 < uVar5) {
      fVar6 = (float)(int)uVar3;
      if ((int)uVar3 < 0) {
        fVar6 = fVar6 + 4.2949673e+09;
      }
      fVar1 = (float)(int)uVar5;
      if ((int)uVar5 < 0) {
        fVar1 = fVar1 + 4.2949673e+09;
      }
      fVar6 = fVar6 / fVar1;
      uVar2 = lv_obj_get_child(DAT_01217cbc,4);
      lv_obj_align(uVar2,9,(int)(short)(int)(fVar6 * 400.0 - 200.0),0x71);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00420d90 @ 00420d90 ===== */

void FUN_00420d90(void)

{
  undefined4 uVar1;
  
  uVar1 = lv_obj_get_child(DAT_01217cbc,0);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007a6b00,0);
  lv_label_set_text(DAT_01217cb8," ");
  return;
}



/* ===== FUNCTION FUN_00420dd8 @ 00420dd8 ===== */

void FUN_00420dd8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = lv_obj_get_child(DAT_01217cbc,1);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_00826720,0);
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_00420174(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    uVar1 = lv_obj_get_child(DAT_01217cbc,0);
    lv_obj_set_style_bg_img_src(uVar1,&DAT_007a6b00,0);
    lv_label_set_text(DAT_01217cb8," ");
  }
  uVar1 = lv_obj_get_child(DAT_01217cbc,1);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007bffa0,0);
  return;
}



/* ===== FUNCTION FUN_00420eac @ 00420eac ===== */

void FUN_00420eac(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = lv_obj_get_child(DAT_01217cbc,2);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007b19c0,0);
  uVar1 = FUN_0041d32c();
  uVar2 = FUN_0040a6ec();
  iVar3 = FUN_0042036c(uVar2);
  if (iVar3 != 0) {
    FUN_00425c80(uVar1);
    FUN_00424408(uVar1,iVar3);
    uVar1 = lv_obj_get_child(DAT_01217cbc,0);
    lv_obj_set_style_bg_img_src(uVar1,&DAT_007a6b00,0);
    lv_label_set_text(DAT_01217cb8," ");
  }
  uVar1 = lv_obj_get_child(DAT_01217cbc,2);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007acf20,0);
  return;
}



/* ===== FUNCTION FUN_00420f80 @ 00420f80 ===== */

void FUN_00420f80(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 != 2) {
    FUN_004247bc();
    uVar1 = lv_obj_get_child(DAT_01217cbc,0);
    lv_obj_set_style_bg_img_src(uVar1,&DAT_007b5220,0);
    lv_label_set_text(DAT_01217cb8," ");
    return;
  }
  FUN_00424888(uVar1);
  uVar1 = lv_obj_get_child(DAT_01217cbc,0);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007a6b00,0);
  lv_label_set_text(DAT_01217cb8," ");
  return;
}



/* ===== FUNCTION FUN_00421044 @ 00421044 ===== */

void FUN_00421044(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  undefined8 uVar8;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_004251bc(uVar1);
  iVar3 = FUN_004250c0(uVar1);
  if (iVar3 == 0) {
    return;
  }
  iVar4 = FUN_0041d32c();
  if (iVar2 <= iVar3) {
LAB_00421120:
    puts("get music time error");
    return;
  }
  uVar8 = FUN_0042ebb8();
  uVar6 = (uint)uVar8 - *(int *)(iVar4 + 0x28);
  uVar5 = (int)((ulonglong)uVar8 >> 0x20) - *(int *)(iVar4 + 0x2c);
  uVar7 = (uint)((uint)uVar8 < uVar6);
  if (((int)(uVar5 - uVar7) < 1) && ((uVar5 != uVar7 || (uVar6 < 0xfa)))) {
    iVar2 = iVar2 / 0x32;
    if (iVar2 != 0) {
      *(undefined8 *)(iVar4 + 0x28) = uVar8;
      *(int *)(iVar4 + 0x30) = iVar2;
      if (iVar2 < 0) goto LAB_00421120;
      goto LAB_00421158;
    }
    iVar2 = 2;
    *(undefined4 *)(iVar4 + 0x30) = 2;
  }
  else {
    iVar2 = 10;
    *(undefined4 *)(iVar4 + 0x30) = 10;
  }
  *(undefined8 *)(iVar4 + 0x28) = uVar8;
LAB_00421158:
  iVar3 = iVar3 - iVar2;
  if (iVar3 < 0) {
    iVar3 = 0;
  }
  FUN_00424954(uVar1,iVar3);
  return;
}



/* ===== FUNCTION FUN_00421194 @ 00421194 ===== */

void FUN_00421194(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  undefined8 uVar9;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_004251bc(uVar1);
  iVar3 = FUN_004250c0(uVar1);
  if (iVar3 == 0) {
    return;
  }
  iVar4 = FUN_0041d32c();
  if (iVar2 <= iVar3) {
LAB_0042126c:
    puts("get music time error");
    return;
  }
  uVar9 = FUN_0042ebb8();
  uVar6 = (uint)uVar9 - *(int *)(iVar4 + 0x28);
  uVar5 = (int)((ulonglong)uVar9 >> 0x20) - *(int *)(iVar4 + 0x2c);
  uVar8 = (uint)((uint)uVar9 < uVar6);
  if (((int)(uVar5 - uVar8) < 1) && ((uVar5 != uVar8 || (uVar6 < 0xfa)))) {
    iVar7 = iVar2 / 0x32;
    if (iVar7 != 0) {
      *(undefined8 *)(iVar4 + 0x28) = uVar9;
      *(int *)(iVar4 + 0x30) = iVar7;
      if (iVar7 < 0) goto LAB_0042126c;
      goto LAB_004212a4;
    }
    *(undefined4 *)(iVar4 + 0x30) = 2;
    iVar7 = 2;
  }
  else {
    iVar7 = 10;
    *(undefined4 *)(iVar4 + 0x30) = 10;
  }
  *(undefined8 *)(iVar4 + 0x28) = uVar9;
LAB_004212a4:
  iVar4 = iVar3 + iVar7;
  if (iVar2 <= iVar3 + iVar7) {
    iVar4 = iVar2;
  }
  FUN_00424954(uVar1,iVar4);
  return;
}



/* ===== FUNCTION FUN_004212e0 @ 004212e0 ===== */

void FUN_004212e0(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar1 = FUN_0041d32c();
  iVar2 = FUN_0042527c(uVar1);
  if (iVar2 == 2) {
    FUN_00424888(uVar1);
    uVar3 = lv_obj_get_child(DAT_01217cbc,0);
    lv_obj_set_style_bg_img_src(uVar3,&DAT_007a6b00,0);
  }
  FUN_00424a30(uVar1);
  iVar2 = FUN_004252ec(uVar1);
  if (iVar2 != 0) {
    lv_label_set_text(DAT_01217cb8,(&PTR_DAT_00450670)[iVar2]);
    return;
  }
  lv_label_set_text(DAT_01217cb8," ");
  return;
}



/* ===== FUNCTION FUN_004213a0 @ 004213a0 ===== */

void FUN_004213a0(void)

{
  int iVar1;
  
  if (DAT_01217cc4 != 0) {
    lv_timer_del();
    DAT_01217cc4 = 0;
  }
  if (DAT_01217cc0 != 0) {
    lv_timer_del();
    DAT_01217cc0 = 0;
  }
  iVar1 = lv_obj_is_valid(DAT_01217cbc);
  if (iVar1 != 0) {
    lv_obj_del(DAT_01217cbc);
    DAT_01217cbc = 0;
  }
  return;
}



/* ===== FUNCTION FUN_00421414 @ 00421414 ===== */

void FUN_00421414(void)

{
  if (DAT_01217cc8 != 0) {
    return;
  }
  DAT_01217cc8 = lv_timer_create(FUN_00420a1c,3000,0);
  return;
}



/* ===== FUNCTION FUN_00421464 @ 00421464 ===== */

void FUN_00421464(int param_1)

{
  if ((DAT_01217cb0 != '\0') && ((param_1 == 0x160 || (param_1 == 0x207)))) {
    if (DAT_01217cc8 != 0) {
      lv_timer_del();
      DAT_01217cc8 = 0;
    }
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0042148c @ 0042148c ===== */

void FUN_0042148c(void)

{
  if (DAT_01217cc8 != 0) {
    lv_timer_del();
    DAT_01217cc8 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_004214c4 @ 004214c4 ===== */

void FUN_004214c4(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_0040a6ec();
  uVar1 = FUN_0042010c(uVar1);
  lv_label_set_text(param_1,uVar1);
  return;
}



/* ===== FUNCTION FUN_004214f8 @ 004214f8 ===== */

void FUN_004214f8(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if ((DAT_0121720c == iVar2) && (iVar2 = FUN_004155dc(), iVar2 == 1)) {
    if (param_1 != 0) {
      if (DAT_01217cc4 == 0) {
        DAT_01217cc4 = lv_timer_create(FUN_004209bc,60000,0);
        lv_timer_set_repeat_count(DAT_01217cc4,0xffffffff);
        return;
      }
      lv_timer_reset();
      lv_timer_resume(DAT_01217cc4);
      return;
    }
    if (DAT_01217cb0 != '\0') {
      if (DAT_01217cc0 != 0) {
        lv_timer_del();
        DAT_01217cc0 = 0;
      }
      lv_obj_add_flag(DAT_01217cb4,1);
      iVar2 = FUN_0042f374();
      if (iVar2 == 0) {
        FUN_00425478(1);
      }
      DAT_01217cb0 = '\0';
      if (DAT_01217cc4 != 0) {
        lv_timer_reset();
        lv_timer_resume(DAT_01217cc4);
      }
      FUN_0042ef78(0);
      puts("Screen unlocked successfully!");
    }
    if (DAT_01217cc4 != 0) {
      lv_timer_pause();
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00421668 @ 00421668 ===== */

undefined4 FUN_00421668(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  lv_obj_set_style_bg_opa(DAT_0121720c,0,0);
  DAT_01217cbc = lv_obj_create(param_1);
  lv_obj_set_style_bg_img_src(DAT_01217cbc,&DAT_00da7420,0);
  lv_obj_set_style_bg_opa(DAT_01217cbc,0,0);
  lv_obj_set_size(DAT_01217cbc,0x2064,0x2064);
  lv_obj_set_style_border_width(DAT_01217cbc,0,0);
  lv_obj_clear_flag(DAT_01217cbc,0x10);
  uVar1 = lv_obj_create(DAT_01217cbc);
  lv_obj_align(uVar1,9,0,0x9b);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007a6b00,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x4e,0x52);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_obj_create(DAT_01217cbc);
  lv_obj_align(uVar1,9,0xffffffc4,0x9b);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007bffa0,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x3a,0x3e);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_obj_create(DAT_01217cbc);
  lv_obj_align(uVar1,9,0x3c,0x9b);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007acf20,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x3a,0x3e);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_obj_create(DAT_01217cbc);
  lv_obj_align(uVar1,9,0,0x71);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_010cd9c0,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x2046,0x27d1);
  lv_obj_set_style_border_width(uVar1,0,0);
  uVar1 = lv_obj_create(DAT_01217cbc);
  lv_obj_align(uVar1,9,0xffffff38,0x71);
  lv_obj_set_style_bg_img_src(uVar1,&DAT_007bb640,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_set_style_border_width(uVar1,0,0);
  DAT_01217cb4 = lv_obj_create(param_1);
  lv_obj_set_style_bg_img_src(DAT_01217cb4,&DAT_00919e40,0);
  lv_obj_set_style_bg_opa(DAT_01217cb4,0,0);
  lv_obj_set_size(DAT_01217cb4,0x2064,0x2064);
  lv_obj_set_style_border_width(DAT_01217cb4,0,0);
  lv_obj_clear_flag(DAT_01217cb4,0x10);
  lv_obj_add_flag(DAT_01217cb4,1);
  uVar1 = lv_label_create(DAT_01217cbc);
  lv_obj_set_size(uVar1,0x2030,0x200a);
  lv_obj_align(uVar1,7,0x82,0xffffff74);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  lv_obj_set_style_text_align(uVar1,1,0);
  lv_label_set_long_mode(uVar1,1);
  uVar1 = lv_label_create(DAT_01217cbc);
  lv_obj_set_size(uVar1,0x2030,0x200a);
  lv_obj_align(uVar1,7,0x82,0xffffff92);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  lv_obj_set_style_text_align(uVar1,1,0);
  lv_label_set_long_mode(uVar1,1);
  uVar1 = lv_label_create(DAT_01217cbc);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_align(uVar1,7,0x78,0x82);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  uVar1 = lv_label_create(DAT_01217cbc);
  lv_obj_set_size(uVar1,0x27d1,0x27d1);
  lv_obj_align(uVar1,8,0xffffff88,0x82);
  lv_label_set_text(uVar1,&DAT_0044f178);
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_style_text_font(uVar1,&DAT_0121edb0,0);
  DAT_01217cb8 = lv_label_create(DAT_01217cbc);
  lv_obj_set_size(DAT_01217cb8,0x27d1,0x27d1);
  lv_obj_align(DAT_01217cb8,9,0xa0,0xffffff51);
  lv_label_set_text(DAT_01217cb8,&DAT_0044f178);
  lv_obj_set_style_text_color(DAT_01217cb8,0xff000000,0);
  lv_obj_set_style_text_opa(DAT_01217cb8,0xff,0);
  lv_obj_set_style_text_font(DAT_01217cb8,&DAT_0121edb0,0);
  DAT_01217cb0 = 0;
  iVar2 = FUN_0042f374();
  if (iVar2 == 0) {
    DAT_01217cc4 = lv_timer_create(FUN_004209bc,60000,0);
    lv_timer_set_repeat_count(DAT_01217cc4,0xffffffff);
  }
  return 0;
}



/* ===== FUNCTION FUN_00421c00 @ 00421c00 ===== */

int * FUN_00421c00(int param_1)

{
  int *__arg;
  int iVar1;
  pthread_attr_t pStack_38;
  
  __arg = calloc(0x470,1);
  *__arg = param_1;
  __arg[1] = 0;
  __arg[0x111] = -1;
  if (param_1 == 2) {
    __arg[0xd] = 2000;
  }
  __arg[0xe] = 4;
  iVar1 = FUN_0042a538(100,0x10);
  __arg[0x111] = iVar1;
  pthread_attr_init(&pStack_38);
  pthread_attr_setstacksize(&pStack_38,0x2000);
  iVar1 = pthread_create(&DAT_01217cd8,&pStack_38,FUN_00422370,__arg);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_38);
  }
  pthread_mutex_init((pthread_mutex_t *)(__arg + 0x116),(pthread_mutexattr_t *)0x0);
  return __arg;
}



/* ===== FUNCTION FUN_00421cc0 @ 00421cc0 ===== */

void FUN_00421cc0(void *param_1)

{
  bool bVar1;
  pthread_t __th;
  int iVar2;
  
  __th = DAT_01217cd8;
  if (param_1 != (void *)0x0) {
    iVar2 = *(int *)((int)param_1 + 0x444);
    if (iVar2 != -1) {
      bVar1 = DAT_01217cd8 != 0;
      *(undefined1 *)((int)param_1 + 0x450) = 1;
      if (bVar1) {
        pthread_join(__th,(void **)0x0);
        iVar2 = *(int *)((int)param_1 + 0x444);
      }
      FUN_0042a5d8(iVar2);
      *(undefined4 *)((int)param_1 + 0x444) = 0xffffffff;
    }
    pthread_mutex_destroy((pthread_mutex_t *)((int)param_1 + 0x458));
    free(param_1);
    return;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/backstage_player.c"
         ,0x10c);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00421d54 @ 00421d54 ===== */

undefined4 FUN_00421d54(int *param_1,char *param_2)

{
  int iVar1;
  pthread_mutex_t *__mutex;
  char *local_110 [5];
  int local_fc;
  undefined4 local_f8;
  undefined4 local_bc;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  int *local_80;
  undefined4 local_7c;
  
  if ((param_1 == (int *)0x0) || (param_2 == (char *)0x0)) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld && media_src",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/backstage_player.c"
           ,0x115);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x116);
  pthread_mutex_lock(__mutex);
  strncpy((char *)(param_1 + 0x10),param_2,0x3ff);
  printf("%s(), line:%d. play: %s.\n","backstage_player_play",0x11b,param_2);
  memset(local_110,0,0xf8);
  local_fc = param_1[0x111];
  local_f8 = 2;
  local_110[0] = param_2;
  local_80 = param_1;
  FUN_0042612c(param_1);
  local_80 = (int *)(uint)*(ushort *)(param_1 + 0x112);
  iVar1 = FUN_00428454(0x3a);
  if (iVar1 == 0) {
LAB_00421dfc:
    iVar1 = *param_1;
    local_7c = 0;
  }
  else {
    iVar1 = FUN_00428454(0x3a);
    if (iVar1 == 1) {
      iVar1 = FUN_00428454(0x3b);
      if (iVar1 == 0) {
        local_7c = 1;
      }
      else if (iVar1 == 1) {
        local_7c = 3;
      }
      else {
        if (iVar1 != 2) goto LAB_00421dfc;
        local_7c = 2;
      }
    }
    iVar1 = *param_1;
  }
  if (iVar1 == 1) {
    local_a0 = 0;
    iVar1 = hcplayer_create(local_110);
    param_1[0x110] = iVar1;
  }
  else {
    if (iVar1 == 2) {
      local_a4 = 0;
      local_bc = 4;
      local_ac = 3000;
      local_a8 = 0x32;
    }
    iVar1 = hcplayer_create(local_110);
    param_1[0x110] = iVar1;
  }
  if (iVar1 == 0) {
    puts("hcplayer_create() fail!");
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  hcplayer_play(iVar1);
  *(undefined1 *)(param_1 + 2) = 0;
  param_1[1] = 1;
  pthread_mutex_unlock(__mutex);
  FUN_0042d72c();
  return 0;
}



/* ===== FUNCTION FUN_00421f58 @ 00421f58 ===== */

undefined4 FUN_00421f58(int param_1)

{
  uint __seed;
  int *__arg;
  int iVar1;
  pthread_attr_t apStack_420 [28];
  
  iVar1 = *(int *)(param_1 + 0x400);
  DAT_01217cd4 = (int *)(&DAT_0121ed3c)[iVar1];
  if (DAT_01217cd4 == (int *)0x0) {
    __arg = calloc(0x470,1);
    *__arg = iVar1;
    __arg[1] = 0;
    __arg[0x111] = -1;
    if (iVar1 == 2) {
      __arg[0xd] = 2000;
    }
    __arg[0xe] = 4;
    iVar1 = FUN_0042a538(100,0x10);
    __arg[0x111] = iVar1;
    pthread_attr_init(apStack_420);
    pthread_attr_setstacksize(apStack_420,0x2000);
    iVar1 = pthread_create(&DAT_01217cd8,apStack_420,FUN_00422370,__arg);
    if (iVar1 == 0) {
      pthread_attr_destroy(apStack_420);
    }
    pthread_mutex_init((pthread_mutex_t *)(__arg + 0x116),(pthread_mutexattr_t *)0x0);
    DAT_01217cd4 = __arg;
    (&DAT_0121ed3c)[*(int *)(param_1 + 0x400)] = __arg;
  }
  __seed = time((time_t *)0x0);
  srand(__seed);
  iVar1 = rand();
  if (*(ushort *)(param_1 + 0x406) == 0) {
    trap(7);
  }
  iVar1 = FUN_0040fddc(param_1,iVar1 % (int)(uint)*(ushort *)(param_1 + 0x406) + 1);
  memset(apStack_420,0,0x400);
  FUN_0040fda8(apStack_420,param_1,iVar1 + 10);
  printf("fullname:%s\n",apStack_420);
  FUN_00421d54(DAT_01217cd4,apStack_420);
  return 0;
}



/* ===== FUNCTION FUN_00422118 @ 00422118 ===== */

undefined4 FUN_00422118(int param_1)

{
  if (param_1 != 0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x458));
    hcplayer_stop2(*(undefined4 *)(param_1 + 0x440),0,0);
    *(undefined4 *)(param_1 + 4) = 0;
    *(undefined1 *)(param_1 + 8) = 0;
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x458));
    return 0;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/backstage_player.c"
         ,0x153);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_0042219c @ 0042219c ===== */

undefined4 FUN_0042219c(void)

{
  bool bVar1;
  void *pvVar2;
  pthread_t __th;
  int iVar3;
  pthread_mutex_t *__mutex;
  
  pvVar2 = DAT_01217cd4;
  __mutex = (pthread_mutex_t *)((int)DAT_01217cd4 + 0x458);
  if (DAT_01217cd4 != (void *)0x0) {
    pthread_mutex_lock(__mutex);
    hcplayer_stop2(*(undefined4 *)((int)pvVar2 + 0x440),0,0);
    *(undefined4 *)((int)pvVar2 + 4) = 0;
    *(undefined1 *)((int)pvVar2 + 8) = 0;
    pthread_mutex_unlock(__mutex);
    __th = DAT_01217cd8;
    pvVar2 = DAT_01217cd4;
    if (DAT_01217cd4 == (void *)0x0) {
      printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
             "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/backstage_player.c"
             ,0x10c);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    iVar3 = *(int *)((int)DAT_01217cd4 + 0x444);
    if (iVar3 != -1) {
      bVar1 = DAT_01217cd8 != 0;
      *(undefined1 *)((int)DAT_01217cd4 + 0x450) = 1;
      if (bVar1) {
        pthread_join(__th,(void **)0x0);
        iVar3 = *(int *)((int)pvVar2 + 0x444);
      }
      FUN_0042a5d8(iVar3);
      *(undefined4 *)((int)pvVar2 + 0x444) = 0xffffffff;
    }
    pthread_mutex_destroy((pthread_mutex_t *)((int)pvVar2 + 0x458));
    free(pvVar2);
    DAT_01217cd4 = (void *)0x0;
    DAT_0121ed3c = 0;
    DAT_0121ed40 = 0;
    DAT_0121ed44 = 0;
    DAT_0121ed48 = 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_0042229c @ 0042229c ===== */

undefined4 FUN_0042229c(int param_1,int param_2)

{
  uint __seed;
  int iVar1;
  undefined1 auStack_410 [1024];
  
  if (*(int *)(param_1 + 4) != 0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x458));
    hcplayer_stop2(*(undefined4 *)(param_1 + 0x440),0,0);
    *(undefined4 *)(param_1 + 4) = 0;
    *(undefined1 *)(param_1 + 8) = 0;
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x458));
  }
  __seed = time((time_t *)0x0);
  srand(__seed);
  iVar1 = rand();
  if (*(ushort *)(param_2 + 0x406) == 0) {
    trap(7);
  }
  iVar1 = FUN_0040fddc(param_2,iVar1 % (int)(uint)*(ushort *)(param_2 + 0x406) + 1);
  memset(auStack_410,0,0x400);
  FUN_0040fda8(auStack_410,param_2,iVar1 + 10);
  FUN_00421d54(DAT_01217cd4,auStack_410);
  return 0;
}



/* ===== FUNCTION FUN_00422370 @ 00422370 ===== */

undefined4 FUN_00422370(int param_1)

{
  ssize_t sVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  undefined **ppuVar7;
  char *pcVar8;
  int local_60;
  int local_5c;
  undefined4 local_38;
  uint local_34;
  int local_30;
  
  if (*(char *)(param_1 + 0x450) == '\0') {
    pcVar8 = "unknow";
    do {
      sVar1 = msgrcv(*(int *)(param_1 + 0x444),&local_38,0xc,0,0x800);
      if ((sVar1 == -1) || (local_34 != *(ushort *)(param_1 + 0x448)))
      goto switchD_00422434_caseD_0;
      printf("%s(), msg->type:%d\n","media_msg_proc",local_38);
      switch(local_38) {
      case 1:
        puts(">>from player 2, player playing");
        break;
      case 2:
        puts(">>from player 2, player paused");
        break;
      case 3:
        puts(">>from player 2, player ready");
        break;
      case 4:
        printf(">>from player 2, buffering %d\n",local_30);
        break;
      case 5:
        puts(">> from player 2, app get eos, normal play end!");
        *(undefined4 *)(param_1 + 4) = 9;
      case 9:
      case 10:
      case 0x15:
switchD_00422434_caseD_9:
        uVar2 = FUN_00423b88();
        FUN_0042229c(param_1,uVar2);
        break;
      case 6:
        puts(">> from player 2,app get trick eos, fast play to end");
        *(undefined4 *)(param_1 + 4) = 9;
        break;
      case 7:
        puts(">>from player 2, app get trick bos, fast back play to begining!");
        break;
      case 8:
        puts(">>from player 2, player read timeout");
        break;
      case 0xb:
        iVar3 = hcplayer_get_nth_video_stream_info
                          (*(undefined4 *)(param_1 + 0x440),local_30,&local_60);
        pcVar6 = pcVar8;
        if ((iVar3 == 0) && (local_5c == 0xad)) {
          pcVar6 = "h265";
        }
        printf("from player 2,Unsupport Video Type:%s",pcVar6);
        break;
      case 0xc:
        printf("video dec err, video idx %d\n",local_30);
        if ((((*(int *)(param_1 + 0x440) != 0) &&
             (iVar3 = hcplayer_get_video_streams_count(), -1 < local_30)) && (local_30 + 1 < iVar3))
           && ((iVar4 = hcplayer_get_cur_video_stream_info
                                  (*(undefined4 *)(param_1 + 0x440),&local_60), iVar4 == 0 &&
               (iVar4 = local_60 + 1, local_60 == local_30)))) {
          uVar2 = *(undefined4 *)(param_1 + 0x440);
          while( true ) {
            iVar5 = hcplayer_change_video_track(uVar2,iVar4);
            iVar4 = iVar4 + 1;
            if ((iVar5 == 0) || (iVar3 <= iVar4)) break;
            uVar2 = *(undefined4 *)(param_1 + 0x440);
          }
        }
        break;
      case 0xd:
        printf("Video Track Unsupport");
        break;
      case 0xe:
        iVar3 = hcplayer_get_nth_audio_stream_info
                          (*(undefined4 *)(param_1 + 0x440),local_30,&local_60);
        ppuVar7 = (undefined **)pcVar8;
        if ((iVar3 == 0) && (ppuVar7 = &PTR_DAT_004507b8, 0x10fff < local_5c)) {
          if (local_5c < 0x12000) {
            ppuVar7 = (undefined **)"adpcm";
          }
          else if (local_5c == 0x15004) {
            ppuVar7 = &PTR_DAT_004507c4;
          }
          else if (local_5c == 0x15028) {
            ppuVar7 = (undefined **)&DAT_004507c8;
          }
          else {
            ppuVar7 = (undefined **)pcVar8;
            if (local_5c == 0x15020) {
              ppuVar7 = &PTR_DAT_004507d0;
            }
          }
        }
        printf("from player 2,Unsupport Audio Type:%s",ppuVar7);
        break;
      case 0xf:
        printf("audio dec err, audio idx %d\n",local_30);
        if (*(int *)(param_1 + 0x440) != 0) {
          iVar3 = hcplayer_get_audio_streams_count();
          if ((local_30 < 0) || (iVar3 <= local_30 + 1)) {
            hcplayer_change_audio_track(*(undefined4 *)(param_1 + 0x440),0xffffffff);
          }
          else {
            iVar4 = hcplayer_get_cur_audio_stream_info(*(undefined4 *)(param_1 + 0x440),&local_60);
            if ((iVar4 == 0) && (iVar4 = local_60 + 1, local_60 == local_30)) {
              uVar2 = *(undefined4 *)(param_1 + 0x440);
              while( true ) {
                iVar5 = hcplayer_change_audio_track(uVar2,iVar4);
                iVar4 = iVar4 + 1;
                if ((iVar5 == 0) || (iVar3 <= iVar4)) break;
                uVar2 = *(undefined4 *)(param_1 + 0x440);
              }
            }
          }
        }
        break;
      case 0x10:
        printf("Audio Track Unsupport");
        goto switchD_00422434_caseD_9;
      }
switchD_00422434_caseD_0:
      FUN_0042c930(10);
    } while (*(char *)(param_1 + 0x450) == '\0');
  }
  puts("exit media_monitor_task()");
  return 0;
}



/* ===== FUNCTION FUN_00422770 @ 00422770 ===== */

undefined4 FUN_00422770(void)

{
  int iVar1;
  undefined4 uVar2;
  pthread_attr_t pStack_30;
  
  pthread_attr_init(&pStack_30);
  pthread_attr_setstacksize(&pStack_30,0x2000);
  pthread_attr_setdetachstate(&pStack_30,1);
  iVar1 = pthread_create(&DAT_01217cd0,&pStack_30,(__start_routine *)&LAB_004220f0,(void *)0x0);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_30);
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_004227e4 @ 004227e4 ===== */

undefined4 FUN_004227e4(void)

{
  bool bVar1;
  void *pvVar2;
  pthread_t __th;
  int iVar3;
  pthread_mutex_t *__mutex;
  
  pvVar2 = DAT_01217cd4;
  if (DAT_01217cd0 == 0) {
    return 0;
  }
  __mutex = (pthread_mutex_t *)((int)DAT_01217cd4 + 0x458);
  if (DAT_01217cd4 != (void *)0x0) {
    pthread_mutex_lock(__mutex);
    hcplayer_stop2(*(undefined4 *)((int)pvVar2 + 0x440),0,0);
    *(undefined4 *)((int)pvVar2 + 4) = 0;
    *(undefined1 *)((int)pvVar2 + 8) = 0;
    pthread_mutex_unlock(__mutex);
    __th = DAT_01217cd8;
    pvVar2 = DAT_01217cd4;
    if (DAT_01217cd4 == (void *)0x0) {
      printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
             "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/backstage_player.c"
             ,0x10c);
      do {
                    /* WARNING: Do nothing block with infinite loop */
      } while( true );
    }
    iVar3 = *(int *)((int)DAT_01217cd4 + 0x444);
    if (iVar3 != -1) {
      bVar1 = DAT_01217cd8 != 0;
      *(undefined1 *)((int)DAT_01217cd4 + 0x450) = 1;
      if (bVar1) {
        pthread_join(__th,(void **)0x0);
        iVar3 = *(int *)((int)pvVar2 + 0x444);
      }
      FUN_0042a5d8(iVar3);
      *(undefined4 *)((int)pvVar2 + 0x444) = 0xffffffff;
    }
    pthread_mutex_destroy((pthread_mutex_t *)((int)pvVar2 + 0x458));
    free(pvVar2);
    DAT_01217cd4 = (void *)0x0;
    DAT_0121ed3c = 0;
    DAT_0121ed40 = 0;
    DAT_0121ed44 = 0;
    DAT_0121ed48 = 0;
  }
  DAT_01217cd0 = 0;
  return 0;
}



/* ===== FUNCTION FUN_0042291c @ 0042291c ===== */

undefined4 FUN_0042291c(void)

{
  return DAT_01217cd4;
}



/* ===== FUNCTION FUN_00422930 @ 00422930 ===== */

void FUN_00422930(void)

{
  if (DAT_01218140 != (void *)0x0) {
    free(DAT_01218140);
    DAT_01218140 = (void *)0x0;
  }
  return;
}



/* ===== FUNCTION FUN_00422974 @ 00422974 ===== */

undefined1 FUN_00422974(void)

{
  return DAT_0121813c;
}



/* ===== FUNCTION FUN_00422980 @ 00422980 ===== */

void FUN_00422980(void)

{
  int *piVar1;
  char *pcVar2;
  char *pcVar3;
  size_t sVar4;
  int iVar5;
  
  DAT_0121813c = 0;
  piVar1 = (int *)FUN_0042b4b4();
  if (piVar1 == (int *)0x0) {
    pcVar2 = "no any partition device";
  }
  else {
    if (0 < *piVar1) {
      iVar5 = 0;
      do {
        pcVar2 = (char *)glist_nth_data(piVar1[1],iVar5);
        pcVar3 = strstr(pcVar2,(char *)&PTR_DAT_0044fa2c);
        if (pcVar3 != (char *)0x0) {
          if (iVar5 < *piVar1) {
            sVar4 = strlen(pcVar2);
            if (0x20 < sVar4 + 4) {
              puts("bgm_path string is too long !");
              return;
            }
            if ((DAT_01218140 != (char *)0x0) ||
               (DAT_01218140 = malloc(0x20), DAT_01218140 != (char *)0x0)) {
              pcVar3 = DAT_01218140;
              memset(DAT_01218140,0,0x20);
              sprintf(pcVar3,"%s/BGM",pcVar2);
              pcVar2 = DAT_01218140;
              iVar5 = access(DAT_01218140,0);
              if (iVar5 != 0) {
                puts("bgm_path isn\'t exist");
                free(DAT_01218140);
                DAT_01218140 = (char *)0x0;
                return;
              }
              DAT_0121813c = 1;
              if (DAT_012180fc == 0) {
                memset(&DAT_01217cf0,0,0x410);
                DAT_012180f0 = 1;
                strncpy(&DAT_01217cf0,pcVar2,0x3ff);
                FUN_0040ff40(&DAT_01217cf0,&DAT_01217cf0);
                return;
              }
              return;
            }
            pcVar2 = "malloc failed,bgm path is NULL";
            DAT_01218140 = (char *)0x0;
            goto LAB_00422aac;
          }
          break;
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < *piVar1);
    }
    pcVar2 = "not find tf card!";
  }
LAB_00422aac:
  puts(pcVar2);
  return;
}



/* ===== FUNCTION FUN_00422ba8 @ 00422ba8 ===== */

undefined4 FUN_00422ba8(undefined4 param_1,int param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 local_18 [3];
  
  DAT_01218130 = lv_list_add_text(param_1," ");
  lv_obj_set_style_text_align(DAT_01218130,1,0);
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(DAT_01218130,(int)(short)(uVar1 | 0x2000),0x27d1);
  uVar2 = DAT_01218130;
  lv_obj_set_style_pad_left(DAT_01218130,5,0);
  lv_obj_set_style_pad_right(uVar2,5,0);
  uVar2 = DAT_01218130;
  lv_obj_set_style_pad_top(DAT_01218130,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_border_width(DAT_01218130,2,0);
  lv_obj_set_style_border_color(DAT_01218130,0xffffffff,0);
  lv_obj_set_style_border_opa(DAT_01218130,0,0);
  lv_obj_set_style_bg_opa(DAT_01218130,0,0);
  lv_obj_set_style_bg_opa(DAT_01218130,0xff,1);
  uVar2 = DAT_01218130;
  lv_palette_main(local_18,5);
  lv_obj_set_style_bg_color(uVar2,local_18[0],1);
  lv_obj_set_style_text_color(DAT_01218130,0xffffffff,0);
  lv_obj_set_style_text_color(DAT_01218130,0xff000000,1);
  lv_obj_set_style_text_font(DAT_01218130,SiYuanHeiTi_Light_3000_28_1b,0);
  lv_label_set_long_mode(DAT_01218130,4);
  return DAT_01218130;
}



/* ===== FUNCTION FUN_00422fac @ 00422fac ===== */

undefined4 FUN_00422fac(char *param_1,int param_2)

{
  int iVar1;
  char *__s2;
  undefined4 *puVar2;
  
  puVar2 = *(undefined4 **)(param_2 + 4);
  if (puVar2 != (undefined4 *)0x0) {
    __s2 = (char *)*puVar2;
    while (__s2 != (char *)0x0) {
      iVar1 = strcmp(param_1,__s2);
      if (iVar1 == 0) {
        return 1;
      }
      puVar2 = (undefined4 *)puVar2[1];
      if (puVar2 == (undefined4 *)0x0) {
        return 0;
      }
      __s2 = (char *)*puVar2;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00423024 @ 00423024 ===== */

undefined4 FUN_00423024(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  char acStack_420 [1024];
  
  iVar5 = (uint)*(ushort *)(param_1 + 0x406) + (uint)*(ushort *)(param_1 + 0x404);
  if (iVar5 != 0) {
    iVar4 = 0;
    do {
      iVar1 = FUN_0040fddc(&DAT_01217cf0,iVar4);
      memset(acStack_420,0,0x400);
      FUN_0040fda8(acStack_420,&DAT_01217cf0,iVar1 + 10);
      puVar3 = *(undefined4 **)(DAT_01217ce0 + 4);
      if (puVar3 == (undefined4 *)0x0) {
        *(undefined1 *)(iVar1 + 9) = 0;
      }
      else {
        do {
          if ((char *)*puVar3 == (char *)0x0) break;
          iVar2 = strcmp(acStack_420,(char *)*puVar3);
          if (iVar2 == 0) {
            *(undefined1 *)(iVar1 + 9) = 1;
            goto LAB_004230e0;
          }
          puVar3 = (undefined4 *)puVar3[1];
        } while (puVar3 != (undefined4 *)0x0);
        *(undefined1 *)(iVar1 + 9) = 0;
      }
LAB_004230e0:
      iVar4 = iVar4 + 1;
    } while (iVar5 != iVar4);
  }
  return 0;
}



/* ===== FUNCTION FUN_004231b8 @ 004231b8 ===== */

undefined4 FUN_004231b8(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  
  puVar7 = &DAT_01218100;
  uVar4 = (uint)DAT_01217ce4;
  iVar6 = DAT_012180f8 - uVar4;
  puVar3 = puVar7;
  do {
    lv_label_set_text(*puVar3," ");
    uVar1 = lv_obj_get_parent(*puVar3);
    uVar1 = lv_obj_get_parent(uVar1);
    lv_obj_clear_state(uVar1,4);
    uVar1 = lv_obj_get_parent(*puVar3);
    lv_obj_clear_state(uVar1,1);
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    lv_label_set_long_mode(uVar1,4);
  } while (puVar3 != &DAT_01218130);
  uVar5 = uVar4 + 0xc;
  do {
    while( true ) {
      iVar2 = FUN_0040fddc(&DAT_01217cf0,uVar4);
      if (iVar2 == 0) goto LAB_004232dc;
      uVar4 = uVar4 + 1;
      lv_label_set_text_fmt(*puVar7,"%s",iVar2 + 10);
      uVar1 = *puVar7;
      puVar7 = puVar7 + 1;
      if (*(char *)(iVar2 + 9) != '\x01') break;
      uVar1 = lv_obj_get_parent(uVar1);
      lv_obj_add_state(uVar1,1);
      if (uVar5 == uVar4) goto LAB_004232dc;
    }
    uVar1 = lv_obj_get_parent(uVar1);
    lv_obj_clear_state(uVar1,1);
  } while (uVar4 != uVar5);
LAB_004232dc:
  uVar1 = lv_obj_get_parent((&DAT_01218100)[iVar6]);
  uVar1 = lv_obj_get_parent(uVar1);
  lv_obj_add_state(uVar1,4);
  lv_label_set_long_mode((&DAT_01218100)[iVar6],3);
  return 0;
}



/* ===== FUNCTION FUN_004233cc @ 004233cc ===== */

undefined4 FUN_004233cc(int param_1)

{
  char *__dest;
  char *__s1;
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  void *pvVar5;
  undefined4 *puVar6;
  char *__s2;
  
  __dest = calloc(1,0x401);
  __s1 = malloc(0x400);
  if (__dest == (char *)0x0) {
    puts("Not enough memory.");
  }
  iVar1 = FUN_0041ff60(&DAT_01217cf0);
  if (param_1 == 0) {
    if (iVar1 == 0) {
      pvVar5 = calloc(1,0x400);
      if ((pvVar5 == (void *)0x0) && (__dest != (char *)0x0)) {
        free(__dest);
        __dest = (char *)0x0;
      }
      strcpy(__dest,&DAT_01217cf0);
      FUN_0041ffc0(pvVar5,&DAT_01217cf0);
      FUN_0040ff40(&DAT_01217cf0,pvVar5);
      iVar1 = FUN_0042004c(&DAT_01217cf0,__dest);
      if (iVar1 == 0xffff) {
        iVar1 = 0;
      }
      DAT_012180f8 = (undefined2)iVar1;
      FUN_00435d2c(&DAT_01217ce4,0xc,DAT_012180f4 + DAT_012180f6,iVar1);
      FUN_004231b8();
      free(pvVar5);
    }
    free(__dest);
    __dest = (char *)0x0;
    if (__s1 == (char *)0x0) {
      return 0;
    }
  }
  else {
    piVar2 = (int *)FUN_0040fddc(&DAT_01217cf0,param_1);
    if (piVar2 == (int *)0x0) {
      puts("file_mgr_get_file_node() fail!");
      if (__dest != (char *)0x0) {
        free(__dest);
        return 0xffffffff;
      }
      if (__s1 == (char *)0x0) {
        return 0xffffffff;
      }
      free(__s1);
      return 0xffffffff;
    }
    FUN_0040fda8(__s1,&DAT_01217cf0,(int)piVar2 + 10);
    if (*piVar2 == 0) {
      FUN_0040ff40(&DAT_01217cf0,__s1);
      DAT_012180f8 = 0;
      FUN_00435d2c(&DAT_01217ce4,0xc,DAT_012180f4 + DAT_012180f6,0);
      FUN_00423024(&DAT_01217cf0);
      FUN_004231b8();
    }
    else if (*(char *)((int)piVar2 + 9) == '\0') {
      uVar3 = lv_obj_get_parent((&DAT_01218100)[param_1 - (uint)DAT_01217ce4]);
      lv_obj_add_state(uVar3,1);
      iVar1 = DAT_01217ce0;
      pvVar5 = malloc(0x400);
      iVar4 = FUN_0040fddc(&DAT_01217cf0,DAT_012180f8);
      FUN_0040fda8(pvVar5,&DAT_01217cf0,iVar4 + 10);
      glist_append(iVar1,pvVar5);
      *(undefined1 *)((int)piVar2 + 9) = 1;
    }
    else {
      uVar3 = lv_obj_get_parent((&DAT_01218100)[param_1 - (uint)DAT_01217ce4]);
      lv_obj_clear_state(uVar3,1);
      iVar1 = DAT_01217ce0;
      for (puVar6 = *(undefined4 **)(DAT_01217ce0 + 4); puVar6 != (undefined4 *)0x0;
          puVar6 = (undefined4 *)puVar6[1]) {
        __s2 = (char *)*puVar6;
        if (__s2 == (char *)0x0) {
          do {
                    /* WARNING: Do nothing block with infinite loop */
          } while( true );
        }
        iVar4 = strcmp(__s1,__s2);
        if (iVar4 == 0) {
          free(__s2);
          *puVar6 = 0;
          glist_delete_link(iVar1,puVar6);
          break;
        }
      }
      *(undefined1 *)((int)piVar2 + 9) = 0;
    }
    if (__s1 == (char *)0x0) goto LAB_00423444;
  }
  free(__s1);
LAB_00423444:
  if (__dest != (char *)0x0) {
    free(__dest);
    return 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_004239d4 @ 004239d4 ===== */

undefined4 FUN_004239d4(void)

{
  if (DAT_0121813c == '\0') {
    puts("can\'t find bgm path");
    return 0;
  }
  if (DAT_012180fc == 0) {
    memset(&DAT_01217cf0,0,0x410);
    DAT_012180f0 = 1;
    strncpy(&DAT_01217cf0,DAT_01218140,0x3ff);
    FUN_0040ff40(&DAT_01217cf0,&DAT_01217cf0);
  }
  return 0;
}



/* ===== FUNCTION FUN_00423b88 @ 00423b88 ===== */

undefined * FUN_00423b88(void)

{
  return &DAT_01217cf0;
}



/* ===== FUNCTION FUN_00423bb8 @ 00423bb8 ===== */

undefined4 FUN_00423bb8(void)

{
  FUN_00410934(&DAT_01217cf0);
  return 0;
}



/* ===== FUNCTION FUN_00423bdc @ 00423bdc ===== */

undefined4 FUN_00423bdc(uint param_1,char *param_2)

{
  int iVar1;
  char *pcVar2;
  
  if ((param_1 < 0x16) && ((int)((0x2a8000U >> (param_1 & 0x1f)) << 0x1f) < 0)) {
    if ((DAT_01218138 != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
      iVar1 = strcmp(&DAT_01217cf0,DAT_01218140);
      if (iVar1 == 0) {
        FUN_00410934();
        memset(&DAT_01217cf0,0,0x410);
        if (DAT_0121813c == '\0') {
          puts("can\'t find bgm path");
        }
        else {
          memset(&DAT_01217cf0,0,0x410);
          DAT_012180f0 = 1;
          strncpy(&DAT_01217cf0,DAT_01218140,0x3ff);
          FUN_0040ff40(&DAT_01217cf0,&DAT_01217cf0);
        }
      }
      else {
        pcVar2 = strstr(&DAT_01217cf0,param_2);
        if (pcVar2 != (char *)0x0) {
          FUN_00410934(&DAT_01217cf0);
          memset(&DAT_01217cf0,0,0x410);
          lv_obj_del(DAT_01218138);
          DAT_01218138 = 0;
          lv_group_focus_obj(DAT_01218134);
        }
      }
    }
    return 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_004241e8 @ 004241e8 ===== */

void FUN_004241e8(undefined4 param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint in_t0;
  
  uVar1 = param_2 + 3 & 3;
  uVar2 = param_2 & 3;
  DAT_01218158 = (*(int *)((param_2 + 3) - uVar1) << (3 - uVar1) * 8 |
                 in_t0 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar2) * 8 |
                 *(uint *)(param_2 - uVar2) >> uVar2 * 8;
  DAT_0121815c = *(undefined4 *)(param_2 + 4);
  DAT_01218168 = param_1;
  DAT_01218160 = *(undefined4 *)(param_2 + 8);
  DAT_01218164 = *(undefined4 *)(param_2 + 0xc);
  return;
}



/* ===== FUNCTION FUN_0042422c @ 0042422c ===== */

int * FUN_0042422c(int param_1)

{
  int *__arg;
  int iVar1;
  pthread_attr_t pStack_38;
  
  __arg = calloc(0x470,1);
  *__arg = param_1;
  __arg[1] = 0;
  __arg[0x111] = -1;
  if (param_1 == 2) {
    __arg[0xd] = 3000;
  }
  __arg[0xe] = 0;
  iVar1 = FUN_00428454(0x12);
  __arg[6] = iVar1;
  if (__arg[0x111] == -1) {
    iVar1 = FUN_0042a538(100,0x10);
    __arg[0x111] = iVar1;
    pthread_attr_init(&pStack_38);
    pthread_attr_setstacksize(&pStack_38,0x2000);
    iVar1 = pthread_create(&DAT_01218154,&pStack_38,(__start_routine *)&LAB_00423d30,__arg);
    if (iVar1 == 0) {
      pthread_attr_destroy(&pStack_38);
    }
  }
  pthread_mutex_init((pthread_mutex_t *)(__arg + 0x116),(pthread_mutexattr_t *)0x0);
  return __arg;
}



/* ===== FUNCTION FUN_00424308 @ 00424308 ===== */

void FUN_00424308(uint *param_1)

{
  bool bVar1;
  pthread_t __th;
  int iVar2;
  uint uVar3;
  
  if (param_1 == (uint *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x1b1);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (DAT_0121816c == '\0') {
    if ((*param_1 & 0xfffffffd) == 0) {
      FUN_0042c530(0);
      iVar2 = FUN_0042e61c();
    }
    else {
      iVar2 = FUN_0042e61c();
    }
    if (iVar2 != 0) {
      FUN_0042e5b8();
      uVar3 = param_1[6];
      goto LAB_0042434c;
    }
  }
  uVar3 = param_1[6];
LAB_0042434c:
  FUN_004286f8(0x12,uVar3);
  __th = DAT_01218154;
  uVar3 = param_1[0x111];
  if (uVar3 != 0xffffffff) {
    bVar1 = DAT_01218154 != 0;
    *(undefined1 *)(param_1 + 0x114) = 1;
    if (bVar1) {
      pthread_join(__th,(void **)0x0);
      uVar3 = param_1[0x111];
    }
    FUN_0042a5d8(uVar3);
    param_1[0x111] = 0xffffffff;
  }
  pthread_mutex_destroy((pthread_mutex_t *)(param_1 + 0x116));
  free(param_1);
  return;
}



/* ===== FUNCTION FUN_00424408 @ 00424408 ===== */

undefined4 FUN_00424408(int *param_1,char *param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  pthread_mutex_t *__mutex;
  char *local_140 [5];
  int local_12c;
  undefined4 local_128;
  undefined1 local_f8;
  int local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  int local_e8;
  int local_e4;
  int local_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_c2;
  undefined4 local_be;
  undefined4 local_ba;
  undefined4 local_b6;
  undefined1 local_b2;
  uint local_b0;
  undefined4 local_ac;
  undefined1 local_9c;
  undefined1 local_56;
  undefined1 auStack_48 [8];
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_2c;
  int local_28 [2];
  
  if ((param_1 == (int *)0x0) || (param_2 == (char *)0x0)) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld && media_src",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x1c3);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  local_2c = 0;
  local_28[0] = 0;
  __mutex = (pthread_mutex_t *)(param_1 + 0x116);
  FUN_0042bcc8(local_28,&local_2c);
  iVar2 = local_28[0];
  uVar1 = local_2c;
  pthread_mutex_lock(__mutex);
  if (param_1[1] == 1) {
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  strncpy((char *)(param_1 + 0x10),param_2,0x3ff);
  *(short *)(param_1 + 0x112) = (short)param_1[0x112] + 1;
  printf("%s(), line:%d. play: %s.\n","media_play",0x1d9,param_2);
  memset(local_140,0,0xf8);
  local_b0 = (uint)*(ushort *)(param_1 + 0x112);
  local_12c = param_1[0x111];
  local_128 = 2;
  local_140[0] = param_2;
  iVar3 = FUN_00428454(0x3a);
  if (iVar3 == 0) {
LAB_004244e4:
    local_ac = 0;
  }
  else {
    iVar3 = FUN_00428454(0x3a);
    if (iVar3 == 1) {
      iVar3 = FUN_00428454(0x3b);
      if (iVar3 == 0) {
        local_ac = 1;
      }
      else if (iVar3 == 1) {
        local_ac = 3;
      }
      else {
        if (iVar3 != 2) goto LAB_004244e4;
        local_ac = 2;
      }
    }
  }
  local_f4 = iVar2;
  local_f0 = uVar1;
  local_f8 = 1;
  FUN_00414b74(auStack_48,iVar2,uVar1);
  iVar3 = *param_1;
  if (DAT_01218168 != 1) {
    if (iVar3 == 1) {
      local_c2 = local_40;
      local_be = local_3c;
      local_d0 = 0;
      DAT_0121816c = 1;
      local_ba = local_38;
      local_b6 = local_34;
      *(undefined1 *)((int)param_1 + 0x451) = 1;
      local_b2 = DAT_0121816c;
      local_56 = DAT_0121816c;
    }
    else if (iVar3 == 0) {
      local_d0 = 1;
      local_b2 = 0;
    }
    else if (iVar3 == 2) {
      local_dc = param_1[0xd];
      local_d4 = 0;
      local_b2 = 0;
      DAT_0121816c = 0;
      local_d8 = 0x32;
      local_ec = DAT_0121468c;
      piVar4 = (int *)FUN_004131cc();
      param_1[0x115] = *piVar4;
      if (*piVar4 != 0) {
        local_e8 = *piVar4;
        local_e4 = piVar4[1];
      }
    }
    goto LAB_0042457c;
  }
  if (iVar3 == 2) {
LAB_00424764:
    local_ec = 8;
  }
  else {
    if (iVar3 == 1) {
      local_d0 = 1;
      goto LAB_00424764;
    }
    if (iVar3 == 0) {
      local_9c = 1;
    }
  }
  local_b2 = 1;
  local_c2 = DAT_01218158;
  local_be = DAT_0121815c;
  local_ba = DAT_01218160;
  local_b6 = DAT_01218164;
  DAT_0121816c = 0;
LAB_0042457c:
  hcplayer_init(3);
  iVar3 = hcplayer_create(local_140);
  param_1[0x110] = iVar3;
  if (iVar3 == 0) {
    puts("hcplayer_create() fail!");
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  hcplayer_play(iVar3);
  param_1[3] = iVar2;
  param_1[1] = 1;
  *(undefined1 *)(param_1 + 2) = 0;
  *(undefined1 *)((int)param_1 + 0x452) = 0;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_004247bc @ 004247bc ===== */

undefined4 FUN_004247bc(int param_1)

{
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x2a3);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) != 0) {
    if (*(char *)(param_1 + 0x452) == '\0') {
      hcplayer_pause();
    }
    else {
      hcplayer_multi_pause();
    }
    *(undefined1 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 4) = 2;
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  pthread_mutex_unlock(__mutex);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00424888 @ 00424888 ===== */

undefined4 FUN_00424888(int param_1)

{
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,699);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) != 0) {
    if (*(char *)(param_1 + 0x452) == '\0') {
      hcplayer_resume();
    }
    else {
      hcplayer_multi_play();
    }
    *(undefined1 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 4) = 1;
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  pthread_mutex_unlock(__mutex);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00424954 @ 00424954 ===== */

undefined4 FUN_00424954(int param_1)

{
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x2d2);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) != 0) {
    if (*(char *)(param_1 + 0x452) == '\0') {
      hcplayer_seek();
    }
    else {
      hcplayer_multi_seek();
    }
    *(undefined1 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 4) = 1;
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  pthread_mutex_unlock(__mutex);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00424a30 @ 00424a30 ===== */

undefined4 FUN_00424a30(int *param_1)

{
  uint uVar1;
  undefined1 uVar2;
  pthread_mutex_t *__mutex;
  int iVar3;
  undefined4 uVar4;
  
  if (param_1 == (int *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x2e7);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x116);
  pthread_mutex_lock(__mutex);
  if (param_1[0x110] != 0) {
    if (*param_1 == 1) {
      if (param_1[1] == 4) {
        uVar1 = (*(byte *)(param_1 + 2) + 1) % 5;
        uVar2 = (undefined1)uVar1;
        uVar4 = *(undefined4 *)(&DAT_00450fd0 + uVar1 * 4);
      }
      else {
        uVar2 = 1;
        uVar1 = 1;
        uVar4 = 0x3fa00000;
      }
      printf("%s(), line:%d. speed: %f\n","media_fastforward",0x2fa);
      hcplayer_set_speed_rate(param_1[0x110],uVar4);
    }
    else {
      uVar1 = 0;
      if (*param_1 == 0) {
        if (param_1[1] == 4) {
          uVar1 = *(byte *)(param_1 + 2) + 1 & 3;
          iVar3 = *(int *)(&DAT_00450fc0 + uVar1 * 4);
        }
        else {
          uVar1 = 1;
          iVar3 = 2;
        }
        uVar2 = (undefined1)uVar1;
        printf("%s(), line:%d. speed: %d\n","media_fastforward",0x305,iVar3);
        hcplayer_set_speed_rate(param_1[0x110],(float)iVar3);
      }
      else {
        uVar2 = 0;
      }
    }
    *(undefined1 *)(param_1 + 2) = uVar2;
    iVar3 = 1;
    if (uVar1 != 0) {
      iVar3 = 4;
    }
    param_1[1] = iVar3;
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  pthread_mutex_unlock(__mutex);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00424c40 @ 00424c40 ===== */

undefined4 FUN_00424c40(int param_1)

{
  uint uVar1;
  pthread_mutex_t *__mutex;
  undefined4 uVar2;
  int iVar3;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x316);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) == 0) {
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  if (*(int *)(param_1 + 4) == 3) {
    uVar1 = (*(byte *)(param_1 + 8) + 1) % 7;
    iVar3 = *(int *)(&DAT_00450fa4 + uVar1 * 4);
    printf("%s(), line:%d. speed: %d\n","media_fastbackward",0x329,iVar3);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),(float)iVar3);
    *(char *)(param_1 + 8) = (char)uVar1;
    uVar2 = 3;
    if (uVar1 == 0) {
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 3;
    printf("%s(), line:%d. speed: %d\n","media_fastbackward",0x329,0xfffffffe);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),0xc0000000);
    *(undefined1 *)(param_1 + 8) = 1;
  }
  *(undefined4 *)(param_1 + 4) = uVar2;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_00424dc4 @ 00424dc4 ===== */

undefined4 FUN_00424dc4(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x339);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) == 0) {
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  if (*(int *)(param_1 + 4) == 6) {
    uVar1 = (*(byte *)(param_1 + 8) + 1) % 6;
    uVar2 = *(undefined4 *)(&DAT_00450f8c + uVar1 * 4);
    printf("%s(), line:%d. speed: %f\n","media_slowforward",0x34d);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),uVar2);
    *(char *)(param_1 + 8) = (char)uVar1;
    uVar2 = 6;
    if (uVar1 == 0) {
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 6;
    printf("%s(), line:%d. speed: %f\n","media_slowforward",0x34d);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),0x3f000000);
    *(undefined1 *)(param_1 + 8) = 1;
  }
  *(undefined4 *)(param_1 + 4) = uVar2;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_00424f48 @ 00424f48 ===== */

undefined4 FUN_00424f48(int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x35d);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) == 0) {
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  if (*(int *)(param_1 + 4) == 5) {
    uVar1 = (*(byte *)(param_1 + 8) + 1) % 6;
    uVar2 = *(undefined4 *)(&DAT_00450f74 + uVar1 * 4);
    printf("%s(), line:%d. speed: %f\n","media_slowbackward",0x371);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),uVar2);
    *(char *)(param_1 + 8) = (char)uVar1;
    uVar2 = 5;
    if (uVar1 == 0) {
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 5;
    printf("%s(), line:%d. speed: %f\n","media_slowbackward",0x371);
    hcplayer_set_speed_rate(*(undefined4 *)(param_1 + 0x440),0);
    *(undefined1 *)(param_1 + 8) = 1;
  }
  *(undefined4 *)(param_1 + 4) = uVar2;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_004250c0 @ 004250c0 ===== */

uint FUN_004250c0(int param_1)

{
  uint uVar1;
  pthread_mutex_t *__mutex;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x381);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) != 0) {
    if (*(char *)(param_1 + 0x452) == '\0') {
      uVar1 = hcplayer_get_position();
    }
    else {
      uVar1 = hcplayer_multi_position();
    }
    if (-1 < (int)uVar1) {
      *(uint *)(param_1 + 0x1c) = uVar1 / 1000;
      pthread_mutex_unlock(__mutex);
      return uVar1 / 1000;
    }
    uVar1 = *(uint *)(param_1 + 0x1c);
    pthread_mutex_unlock(__mutex);
    return uVar1;
  }
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_004251bc @ 004251bc ===== */

int FUN_004251bc(int param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (param_1 == 0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x39b);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  if (*(int *)(param_1 + 0x440) != 0) {
    if (*(char *)(param_1 + 0x452) == '\0') {
      uVar2 = hcplayer_get_duration();
      iVar1 = FUN_0044e8c8((int)uVar2,(int)((ulonglong)uVar2 >> 0x20),1000,0);
      *(int *)(param_1 + 0x20) = iVar1;
    }
    else {
      iVar1 = hcplayer_multi_duration();
      iVar1 = iVar1 / 1000;
      *(int *)(param_1 + 0x20) = iVar1;
    }
    return iVar1;
  }
  return *(int *)(param_1 + 0x20);
}



/* ===== FUNCTION FUN_0042527c @ 0042527c ===== */

undefined4 FUN_0042527c(int param_1)

{
  undefined4 uVar1;
  
  if (param_1 != 0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x458));
    uVar1 = *(undefined4 *)(param_1 + 4);
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x458));
    return uVar1;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x3a9);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004252ec @ 004252ec ===== */

undefined1 FUN_004252ec(int param_1)

{
  if (param_1 != 0) {
    return *(undefined1 *)(param_1 + 8);
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x3b3);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004252fc @ 004252fc ===== */

void FUN_004252fc(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x3b3);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_0042533c @ 0042533c ===== */

void FUN_0042533c(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x3b9);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_0042536c @ 0042536c ===== */

undefined4 FUN_0042536c(int param_1,float *param_2)

{
  float fVar1;
  float in_f0;
  undefined8 uVar2;
  
  if (param_1 != 0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x458));
    if (*(int *)(param_1 + 0x440) != 0) {
      uVar2 = hcplayer_get_filesize();
      FUN_0044e808((int)uVar2,(int)((ulonglong)uVar2 >> 0x20));
      *param_2 = in_f0 * 0.0009765625 * 0.0009765625;
      hcplayer_get_cur_audio_stream_info(*(undefined4 *)(param_1 + 0x440),param_2 + 0x28);
      hcplayer_get_cur_video_stream_info(*(undefined4 *)(param_1 + 0x440),param_2 + 0x32);
      hcplayer_get_cur_subtitle_stream_info(*(undefined4 *)(param_1 + 0x440),param_2 + 0x3c);
      hcplayer_get_media_info(*(undefined4 *)(param_1 + 0x440),param_2 + 0x40);
      fVar1 = (float)hcplayer_get_audio_streams_count(*(undefined4 *)(param_1 + 0x440));
      param_2[1] = fVar1;
      fVar1 = (float)hcplayer_get_subtitle_streams_count(*(undefined4 *)(param_1 + 0x440));
      param_2[2] = fVar1;
      __xstat(3,(char *)(param_1 + 0x40),(stat *)(param_2 + 3));
    }
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x458));
    return 0;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x3bf);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00425478 @ 00425478 ===== */

undefined4 FUN_00425478(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  DAT_01218150 = FUN_004293bc();
  if (param_1 == 0) {
    uVar2 = FUN_0042e248(0);
    return uVar2;
  }
  iVar1 = FUN_0042e1f0();
  if (iVar1 != 0) {
    return 0;
  }
  uVar2 = FUN_0042e248(DAT_01218150);
  return uVar2;
}



/* ===== FUNCTION FUN_00425500 @ 00425500 ===== */

undefined4 FUN_00425500(int *param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  
  if (param_1 != (int *)0x0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x116));
    iVar2 = param_1[0x110];
    if (iVar2 != 0) {
      param_1[3] = param_2;
      param_1[5] = param_1[5] | 2;
      if (*param_1 == 2) {
        uVar1 = FUN_004131cc();
        hcplayer_change_rotate_mirror_type2(iVar2,param_2,0,uVar1);
      }
      else {
        hcplayer_change_rotate_mirror_type(iVar2,param_2);
      }
    }
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x116));
    return 0;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x43b);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004256c8 @ 004256c8 ===== */

void FUN_004256c8(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld && media_urls",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x44f);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00425720 @ 00425720 ===== */

undefined4 FUN_00425720(int param_1)

{
  if (param_1 != 0) {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x458));
    hcplayer_multi_destroy(*(undefined4 *)(param_1 + 0x440));
    *(undefined4 *)(param_1 + 4) = 0;
    *(undefined1 *)(param_1 + 8) = 0;
    *(undefined4 *)(param_1 + 0x440) = 0;
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x458));
    return 0;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x489);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004257fc @ 004257fc ===== */

void FUN_004257fc(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x495);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004259dc @ 004259dc ===== */

void FUN_004259dc(int param_1)

{
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0x3c) = 0;
  return;
}



/* ===== FUNCTION FUN_004259e8 @ 004259e8 ===== */

undefined4 FUN_004259e8(int param_1)

{
  uint uVar1;
  int iVar2;
  pthread_mutex_t *__mutex;
  undefined8 uVar3;
  
  if (*(int *)(param_1 + 0x3c) == 0) {
    return 0;
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) == 0) {
    pthread_mutex_unlock(__mutex);
    uVar1 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x452) == '\0') {
      uVar1 = hcplayer_get_position();
    }
    else {
      uVar1 = hcplayer_multi_position();
    }
    if ((int)uVar1 < 0) {
      uVar1 = *(uint *)(param_1 + 0x1c);
      pthread_mutex_unlock(__mutex);
      iVar2 = *(int *)(param_1 + 0x440);
      goto joined_r0x00425b2c;
    }
    uVar1 = uVar1 / 1000;
    *(uint *)(param_1 + 0x1c) = uVar1;
    pthread_mutex_unlock(__mutex);
  }
  iVar2 = *(int *)(param_1 + 0x440);
joined_r0x00425b2c:
  if (iVar2 == 0) {
    iVar2 = *(int *)(param_1 + 0x20);
  }
  else if (*(char *)(param_1 + 0x452) == '\0') {
    uVar3 = hcplayer_get_duration();
    iVar2 = FUN_0044e8c8((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),1000,0);
    *(int *)(param_1 + 0x20) = iVar2;
  }
  else {
    iVar2 = hcplayer_multi_duration();
    iVar2 = iVar2 / 1000;
    *(int *)(param_1 + 0x20) = iVar2;
  }
  if ((int)uVar1 < iVar2) {
    iVar2 = uVar1 + *(int *)(param_1 + 0x3c);
    if ((iVar2 < 0) || (iVar2 == 0)) {
      *(undefined1 *)(param_1 + 8) = 0;
      *(undefined4 *)(param_1 + 0x3c) = 0;
    }
    hcplayer_seek(*(undefined4 *)(param_1 + 0x440));
  }
  return 0;
}



/* ===== FUNCTION FUN_004259fc @ 004259fc ===== */

undefined4 FUN_004259fc(int param_1)

{
  uint uVar1;
  int iVar2;
  pthread_mutex_t *__mutex;
  undefined8 uVar3;
  
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 0x440) == 0) {
    pthread_mutex_unlock(__mutex);
    uVar1 = 0;
  }
  else {
    if (*(char *)(param_1 + 0x452) == '\0') {
      uVar1 = hcplayer_get_position();
    }
    else {
      uVar1 = hcplayer_multi_position();
    }
    if ((int)uVar1 < 0) {
      uVar1 = *(uint *)(param_1 + 0x1c);
      pthread_mutex_unlock(__mutex);
      iVar2 = *(int *)(param_1 + 0x440);
      goto joined_r0x00425b2c;
    }
    uVar1 = uVar1 / 1000;
    *(uint *)(param_1 + 0x1c) = uVar1;
    pthread_mutex_unlock(__mutex);
  }
  iVar2 = *(int *)(param_1 + 0x440);
joined_r0x00425b2c:
  if (iVar2 == 0) {
    iVar2 = *(int *)(param_1 + 0x20);
  }
  else if (*(char *)(param_1 + 0x452) == '\0') {
    uVar3 = hcplayer_get_duration();
    iVar2 = FUN_0044e8c8((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),1000,0);
    *(int *)(param_1 + 0x20) = iVar2;
  }
  else {
    iVar2 = hcplayer_multi_duration();
    iVar2 = iVar2 / 1000;
    *(int *)(param_1 + 0x20) = iVar2;
  }
  if ((int)uVar1 < iVar2) {
    iVar2 = uVar1 + *(int *)(param_1 + 0x3c);
    if ((iVar2 < 0) || (iVar2 == 0)) {
      *(undefined1 *)(param_1 + 8) = 0;
      *(undefined4 *)(param_1 + 0x3c) = 0;
    }
    hcplayer_seek(*(undefined4 *)(param_1 + 0x440));
  }
  return 0;
}



/* ===== FUNCTION FUN_00425b88 @ 00425b88 ===== */

void FUN_00425b88(int *param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  
  pthread_mutex_lock((pthread_mutex_t *)(param_1 + 0x116));
  if (*param_1 == 0) {
    FUN_0042dc0c(1,5);
    uVar2 = param_1[1];
  }
  else {
    uVar2 = param_1[1];
  }
  FUN_00413c6c(param_2,(uVar2 & 0xfffffffd) != 0);
  iVar1 = FUN_00413c60();
  param_1[4] = iVar1;
  param_1[5] = param_1[5] | 4;
  pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 0x116));
  return;
}



/* ===== FUNCTION FUN_00425c18 @ 00425c18 ===== */

void FUN_00425c18(int *param_1)

{
  if (*param_1 != 0) {
    FUN_00413bd4();
    param_1[4] = 0;
    param_1[5] = param_1[5] & 0xfffffffb;
    return;
  }
  FUN_00414ff4(param_1[6]);
  param_1[4] = 0;
  param_1[5] = param_1[5] & 0xfffffffb;
  return;
}



/* ===== FUNCTION FUN_00425c80 @ 00425c80 ===== */

undefined4 FUN_00425c80(uint *param_1)

{
  bool bVar1;
  uint uVar2;
  pthread_mutex_t *__mutex;
  
  if (param_1 == (uint *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x268);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x116);
  pthread_mutex_lock(__mutex);
  if ((param_1[0x110] == 0) || (param_1[1] == 0)) {
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  hcplayer_stop2(param_1[0x110],DAT_0121816c,0);
  bVar1 = DAT_0121816c == '\0';
  param_1[0x110] = 0;
  if (bVar1) {
    if (*param_1 < 2) {
LAB_00425d58:
      FUN_0042e164(0);
      FUN_0042e540();
      goto LAB_00425d68;
    }
    if (*param_1 == 2) {
      if (param_1[0x115] == 0) goto LAB_00425d58;
      uVar2 = param_1[5];
      if ((uVar2 & 2) == 0) goto LAB_00425cf4;
      FUN_0042e540();
      uVar2 = param_1[5] & 0xfffffffd;
      param_1[5] = uVar2;
      goto LAB_00425d6c;
    }
    uVar2 = param_1[5];
LAB_00425cf4:
    if ((uVar2 & 4) == 0) goto LAB_00425d88;
LAB_00425d00:
    FUN_0042c530(0);
    if (*param_1 == 0) {
      FUN_00414ff4(param_1[6]);
      uVar2 = param_1[5];
    }
    else {
      FUN_00413bd4();
      uVar2 = param_1[5];
    }
    param_1[4] = 0;
    param_1[5] = uVar2 & 0xfffffffb;
    uVar2 = *param_1;
  }
  else {
LAB_00425d68:
    uVar2 = param_1[5];
LAB_00425d6c:
    if ((uVar2 & 4) != 0) goto LAB_00425d00;
    uVar2 = *param_1;
  }
  if ((uVar2 == 1) && (*(char *)((int)param_1 + 0x451) == '\x01')) {
    FUN_00425c18(param_1);
    *(undefined1 *)((int)param_1 + 0x451) = 0;
  }
LAB_00425d88:
  param_1[1] = 0;
  *(undefined1 *)(param_1 + 2) = 0;
  param_1[0xf] = 0;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_0042612c @ 0042612c ===== */

undefined4 FUN_0042612c(int param_1)

{
  if (param_1 != 0) {
    *(short *)(param_1 + 0x448) = *(short *)(param_1 + 0x448) + 1;
    return 0;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x55f);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00426144 @ 00426144 ===== */

void FUN_00426144(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x55f);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00426174 @ 00426174 ===== */

void FUN_00426174(int param_1)

{
  if (param_1 != 0) {
    hcplayer_set_display_rect(*(undefined4 *)(param_1 + 0x440));
    return;
  }
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x56a);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_004261c4 @ 004261c4 ===== */

void FUN_004261c4(void)

{
  printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
         "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
         ,0x570);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_00426380 @ 00426380 ===== */

undefined4 FUN_00426380(int param_1,char *param_2,undefined4 param_3,int param_4,char param_5)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  pthread_mutex_t *__mutex;
  char *local_138 [5];
  undefined4 local_124;
  undefined4 local_120;
  undefined1 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  int local_e0;
  int local_dc;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_cc;
  undefined4 local_ba;
  undefined4 local_b6;
  undefined4 local_b2;
  undefined4 local_ae;
  undefined1 local_aa;
  uint local_a8;
  undefined4 local_a4;
  undefined1 auStack_40 [36];
  
  if ((param_1 == 0) || (param_2 == (char *)0x0)) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld && media_src",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x5a0);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x458);
  pthread_mutex_lock(__mutex);
  if (*(int *)(param_1 + 4) == 1) {
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  if (param_5 != '\0') {
    FUN_0042c530(0);
  }
  strncpy((char *)(param_1 + 0x40),param_2,0x3ff);
  *(short *)(param_1 + 0x448) = *(short *)(param_1 + 0x448) + 1;
  printf("%s(), line:%d. play: %s.\n","media_photo_replay",0x5af,param_2);
  memset(local_138,0,0xf8);
  local_a8 = (uint)*(ushort *)(param_1 + 0x448);
  local_124 = *(undefined4 *)(param_1 + 0x444);
  local_120 = 2;
  local_138[0] = param_2;
  iVar1 = FUN_00428454(0x3a);
  if (iVar1 == 0) {
LAB_00426454:
    local_a4 = 0;
  }
  else {
    iVar1 = FUN_00428454(0x3a);
    if (iVar1 == 1) {
      iVar1 = FUN_00428454(0x3b);
      if (iVar1 == 0) {
        local_a4 = 1;
      }
      else if (iVar1 == 1) {
        local_a4 = 3;
      }
      else {
        if (iVar1 != 2) goto LAB_00426454;
        local_a4 = 2;
      }
    }
  }
  local_e8 = 0;
  local_f0 = 1;
  local_ec = param_3;
  FUN_00414b74(auStack_40,param_3,0);
  DAT_0121816c = 0;
  local_e4 = 4;
  if (param_4 != 0) {
    puVar3 = (undefined4 *)FUN_004150c0();
    if (*(short *)(puVar3 + 3) != 0) {
      local_ba = *puVar3;
      local_b6 = puVar3[1];
      local_b2 = puVar3[2];
      local_ae = puVar3[3];
      local_aa = 1;
      goto LAB_00426490;
    }
  }
  local_aa = 0;
LAB_00426490:
  local_d4 = *(undefined4 *)(param_1 + 0x34);
  local_cc = 0;
  local_d0 = 0x32;
  piVar2 = (int *)FUN_004131cc();
  *(int *)(param_1 + 0x454) = *piVar2;
  if (*piVar2 != 0) {
    local_e0 = *piVar2;
    local_dc = piVar2[1];
  }
  hcplayer_init(3);
  iVar1 = hcplayer_create(local_138);
  *(int *)(param_1 + 0x440) = iVar1;
  if (iVar1 == 0) {
    puts("hcplayer_create() fail!");
    pthread_mutex_unlock(__mutex);
    return 0xffffffff;
  }
  hcplayer_play(iVar1);
  *(undefined4 *)(param_1 + 0xc) = param_3;
  *(undefined4 *)(param_1 + 4) = 1;
  *(undefined1 *)(param_1 + 8) = 0;
  *(undefined1 *)(param_1 + 0x452) = 0;
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_0042668c @ 0042668c ===== */

undefined4 FUN_0042668c(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  pthread_mutex_t *__mutex;
  
  if (param_1 == (int *)0x0) {
    printf("assertion(%s) failed: file \"%s\", line %d\n","media_hld",
           "/data/home/lucien.chen/cube8y/buildroot/output/cubegm/build/cubemp5app-p2-f66b878e763ecbc32a670e5a77ef468862e7492c/hcprojector_app/channel/local_mp/media_player.c"
           ,0x3f2);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  __mutex = (pthread_mutex_t *)(param_1 + 0x116);
  FUN_0042e540();
  pthread_mutex_lock(__mutex);
  if (param_1[0x110] == 0) goto LAB_0042688c;
  iVar1 = param_1[3];
  if (param_2 == 0) {
    if (iVar1 == 1) {
      iVar1 = 2;
      param_1[3] = 2;
    }
    else if (iVar1 == 0) {
      iVar1 = 1;
      param_1[3] = 1;
    }
    else {
      if (iVar1 == 2) goto LAB_0042683c;
      if (iVar1 == 3) goto LAB_004266f8;
    }
LAB_004267e8:
    iVar2 = *param_1;
    param_1[5] = param_1[5] | 2;
  }
  else {
    if (iVar1 != 1) {
      if (iVar1 == 0) {
LAB_0042683c:
        iVar1 = 3;
        param_1[3] = 3;
      }
      else if (iVar1 == 2) {
        iVar1 = 1;
        param_1[3] = 1;
      }
      else if (iVar1 == 3) {
        iVar1 = 2;
        param_1[3] = 2;
      }
      goto LAB_004267e8;
    }
LAB_004266f8:
    iVar2 = *param_1;
    iVar1 = 0;
    param_1[3] = 0;
    param_1[5] = param_1[5] | 2;
  }
  if (iVar2 != 2) {
    hcplayer_change_rotate_type(param_1[0x110],iVar1);
    pthread_mutex_unlock(__mutex);
    return 0;
  }
  pthread_mutex_unlock(__mutex);
  iVar1 = param_1[3];
  param_1[5] = param_1[5] & 0xfffffffb;
  FUN_00425c80(param_1);
  FUN_00426380(param_1,param_1 + 0x10,iVar1,1,0);
  param_1[5] = param_1[5] & 4;
  pthread_mutex_lock(__mutex);
  if (param_1[0x110] != 0) {
    if (*(char *)((int)param_1 + 0x452) == '\0') {
      hcplayer_pause();
      *(undefined1 *)(param_1 + 2) = 0;
      param_1[1] = 2;
      param_1[0xf] = 0;
      pthread_mutex_unlock(__mutex);
    }
    else {
      hcplayer_multi_pause();
      *(undefined1 *)(param_1 + 2) = 0;
      param_1[1] = 2;
      param_1[0xf] = 0;
      pthread_mutex_unlock(__mutex);
    }
    return 0;
  }
LAB_0042688c:
  pthread_mutex_unlock(__mutex);
  return 0;
}



/* ===== FUNCTION FUN_004268e0 @ 004268e0 ===== */

undefined1 FUN_004268e0(void)

{
  return DAT_012181e0;
}



/* ===== FUNCTION FUN_004268ec @ 004268ec ===== */

undefined4 FUN_004268ec(void)

{
  return DAT_012181dc;
}



/* ===== FUNCTION FUN_00426904 @ 00426904 ===== */

int FUN_00426904(void)

{
  int iVar1;
  int iVar2;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined *local_10;
  
  iVar1 = open("/dev/persistentmem",2);
  if (iVar1 < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",iVar1);
  }
  else {
    local_16 = 0;
    local_18 = 3;
    local_14 = 0x104;
    local_10 = &DAT_01218170;
    iVar2 = ioctl(iVar1,0x400c2602,&local_18);
    if (-1 < iVar2) {
      iVar1 = close(iVar1);
      return iVar1;
    }
    puts("get avparam node fail");
    close(iVar1);
  }
  return -1;
}



/* ===== FUNCTION FUN_00426c10 @ 00426c10 ===== */

undefined1 FUN_00426c10(void)

{
  return DAT_01214690;
}



/* ===== FUNCTION FUN_00426c1c @ 00426c1c ===== */

void FUN_00426c1c(int param_1)

{
  char *pcVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar3 = DAT_01214690 ^ 1;
  DAT_01214690 = (byte)uVar3;
  if (uVar3 == 0) {
    pcVar1 = "MUTE!";
  }
  else {
    pcVar1 = "UNMUTE!";
  }
  printf("%s(), line:%d. m_mute_state:%d(%s)\n","win_mute_on_off",0x22,uVar3,pcVar1);
  if (DAT_01218284 == 0) {
    uVar2 = lv_disp_get_default();
    uVar2 = lv_disp_get_scr_act(uVar2);
    DAT_01218284 = lv_obj_create(uVar2);
    lv_obj_set_size(DAT_01218284,0x3c,0x3c);
    lv_obj_set_style_bg_color(DAT_01218284,0xff303030,0);
    lv_obj_set_style_border_opa(DAT_01218284,0,0);
    lv_obj_align(DAT_01218284,3,0xffffff9c,0x28);
    lv_obj_clear_flag(DAT_01218284,0x10);
    lv_obj_set_style_radius(DAT_01218284,10,0);
    DAT_01218280 = lv_img_create(DAT_01218284);
    lv_img_set_src(DAT_01218280,&DAT_00d475c0);
    lv_obj_align(DAT_01218280,9,0,0);
  }
  if (DAT_01214690 == '\0') {
    FUN_0042d540(1);
  }
  else {
    FUN_0042d540(0);
  }
  if (param_1 == 0) {
    return;
  }
  if (DAT_01214690 != '\0') {
    lv_obj_add_flag();
    return;
  }
  lv_obj_clear_flag(DAT_01218284,1);
  return;
}



/* ===== FUNCTION FUN_00426e54 @ 00426e54 ===== */

undefined4 FUN_00426e54(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = param_1 & 0xff;
  if (DAT_012146a0 != 0) {
    uVar2 = FUN_00428454(0x16);
    uVar2 = uVar2 & 0xff;
    if (param_1 >> 0x10 == 0x1c) {
      if (uVar2 < 0x5a) {
        uVar3 = uVar2 + 10 & 0xff;
      }
      else {
        uVar3 = 100;
      }
    }
    else if (param_1 >> 0x10 == 0x1d) {
      if (uVar2 < 0xb) {
        uVar3 = 0;
      }
      else {
        uVar3 = uVar2 - 10 & 0xff;
      }
    }
    FUN_0042d640(uVar3);
    FUN_004286f8(0x16,uVar3);
  }
  iVar1 = FUN_00426c10();
  if (iVar1 == 0) {
    FUN_00426c1c(1);
  }
  if (uVar3 < 0x32) {
    lv_obj_set_style_text_color(DAT_012182a0,0xffffffff,0);
    if (uVar3 == 0) {
      lv_img_set_src(DAT_01218298,&DAT_00d475c0);
      goto LAB_00426ec8;
    }
  }
  else {
    lv_obj_set_style_text_color(DAT_012182a0,0xff000000,0);
  }
  lv_img_set_src(DAT_01218298,&DAT_00d50140);
LAB_00426ec8:
  lv_bar_set_value(DAT_012182a0,uVar3,0);
  lv_label_set_text_fmt(DAT_0121829c,&DAT_00452028,uVar3);
  lv_obj_clear_flag(DAT_012182a4,1);
  if (3 < DAT_011e9b00) {
    printf("[app]%s(), open show: %d\n\n","show_play_bar",1);
  }
  lv_timer_reset(DAT_01218294);
  lv_timer_resume(DAT_01218294);
  return 0;
}



/* ===== FUNCTION FUN_004270c4 @ 004270c4 ===== */

undefined4 FUN_004270c4(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  DAT_01218290 = 0;
  DAT_012146a0 = param_2;
  if (DAT_012182a4 != 0) {
    uVar1 = FUN_00426e54();
    return uVar1;
  }
  printf("%s()!\n","win_volume_open");
  uVar1 = lv_disp_get_default();
  uVar1 = lv_disp_get_scr_act(uVar1);
  DAT_012182a4 = lv_obj_create(uVar1);
  lv_obj_set_size(DAT_012182a4,0x32,0x1e0);
  lv_obj_set_style_bg_color(DAT_012182a4,0xff303030,0);
  lv_obj_set_style_border_opa(DAT_012182a4,0,0);
  lv_obj_align(DAT_012182a4,3,0xffffffd8,0x28);
  lv_obj_clear_flag(DAT_012182a4,0x10);
  lv_obj_set_style_radius(DAT_012182a4,10,0);
  DAT_01218298 = lv_img_create(DAT_012182a4);
  lv_obj_align(DAT_01218298,2,0,0xfffffff8);
  DAT_012182a0 = lv_bar_create(DAT_012182a4);
  lv_obj_align(DAT_012182a0,5,0,4);
  lv_obj_set_size(DAT_012182a0,0x1e,400);
  lv_bar_set_range(DAT_012182a0,0,100);
  DAT_0121829c = lv_label_create(DAT_012182a0);
  lv_obj_align(DAT_0121829c,9,0,0);
  DAT_01218294 = lv_timer_create(&LAB_00426dd0,3000,0);
  FUN_00426e54(param_1);
  return 0;
}



/* ===== FUNCTION FUN_00427270 @ 00427270 ===== */

undefined4 FUN_00427270(short *param_1)

{
  short sVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  short *psVar5;
  void *pvVar6;
  void *__src;
  void *__s2;
  undefined1 *__s1;
  uint __n;
  ushort uVar7;
  short *local_28 [2];
  
  sVar1 = *param_1;
  if (sVar1 == 2) {
    uVar7 = param_1[1];
    __s1 = &DAT_0121a99c + uVar7;
    __s2 = (void *)((int)&DAT_01218370 + (uint)uVar7);
  }
  else {
    if (sVar1 != 1) {
      return 0;
    }
    uVar7 = param_1[1];
    __s1 = (undefined1 *)((int)&DAT_0121a8fc + (uint)uVar7);
    __s2 = (void *)((int)&DAT_012182d0 + (uint)uVar7);
  }
  uVar2 = param_1[2];
  __n = (uint)uVar2;
  iVar3 = memcmp(__s1,__s2,__n);
  uVar4 = 0;
  if (iVar3 != 0) {
    psVar5 = malloc(0xc);
    pvVar6 = malloc(__n);
    if ((psVar5 != (short *)0x0) && (pvVar6 != (void *)0x0)) {
      __src = *(void **)(param_1 + 4);
      *psVar5 = sVar1;
      psVar5[1] = uVar7;
      psVar5[2] = uVar2;
      pvVar6 = memcpy(pvVar6,__src,__n);
      *(void **)(psVar5 + 4) = pvVar6;
      memcpy(__s2,__s1,__n);
      local_28[0] = psVar5;
      if ((DAT_012146b4 == -1) && (DAT_012146b4 = FUN_0042a538(100,4), DAT_012146b4 == -1)) {
        DAT_012146b4 = 0xffffffff;
        return 0;
      }
      FUN_0042a628(DAT_012146b4,local_28,4);
      sem_post((sem_t *)&DAT_0121ed4c);
      return 0;
    }
    printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
    uVar4 = 0xffffffff;
  }
  return uVar4;
}



/* ===== FUNCTION FUN_00427548 @ 00427548 ===== */

undefined4 FUN_00427548(undefined1 *param_1)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_c0 [32];
  undefined1 local_a0;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined1 *local_18;
  
  __fd = open("/dev/persistentmem",0x4012);
  if (__fd < 0) {
    puts("open /dev/persistentmem failed");
    uVar2 = 0xffffffff;
  }
  else {
    local_1e = 0;
    local_20 = 1;
    local_1c = 0xa0;
    local_18 = auStack_c0;
    iVar1 = ioctl(__fd,0x400c2602,&local_20);
    if (iVar1 < 0) {
      close(__fd);
      uVar2 = 0xffffffff;
    }
    else {
      close(__fd);
      uVar2 = 0;
      *param_1 = local_a0;
    }
  }
  return uVar2;
}



/* ===== FUNCTION FUN_004275f8 @ 004275f8 ===== */

undefined4 FUN_004275f8(undefined1 param_1)

{
  char cVar1;
  undefined2 *puVar2;
  undefined1 *puVar3;
  undefined2 *local_20 [2];
  
  cVar1 = DAT_0121a91c;
  if (DAT_012182f0 != DAT_0121a91c) {
    puVar2 = malloc(0xc);
    puVar3 = malloc(1);
    if ((puVar2 == (undefined2 *)0x0) || (puVar3 == (undefined1 *)0x0)) {
      printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
    }
    else {
      *puVar3 = param_1;
      DAT_012182f0 = cVar1;
      *(undefined1 **)(puVar2 + 4) = puVar3;
      *puVar2 = 1;
      puVar2[2] = 1;
      puVar2[1] = 0x20;
      local_20[0] = puVar2;
      if ((DAT_012146b4 != -1) || (DAT_012146b4 = FUN_0042a538(100,4), DAT_012146b4 != -1)) {
        FUN_0042a628(DAT_012146b4,local_20,4);
        sem_post((sem_t *)&DAT_0121ed4c);
        return 0;
      }
      DAT_012146b4 = -1;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00427724 @ 00427724 ===== */

void FUN_00427724(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  undefined4 local_1c8 [4];
  undefined1 local_1b8 [4];
  undefined1 local_1b4 [4];
  undefined1 local_1b0;
  undefined1 local_1ac;
  undefined1 local_1a8;
  undefined1 local_1a4;
  undefined1 local_1a0;
  undefined1 local_19c;
  undefined1 local_198;
  undefined1 local_194;
  undefined1 local_190;
  undefined1 local_18c;
  undefined1 local_188;
  undefined1 local_184;
  undefined1 local_180;
  undefined1 local_17c;
  undefined1 local_178;
  undefined1 local_174;
  undefined1 local_170;
  undefined1 local_16c;
  undefined1 local_168;
  undefined1 local_164;
  undefined1 local_160;
  undefined1 local_15c;
  undefined1 local_158;
  undefined1 local_154;
  undefined1 local_150;
  undefined1 local_14c;
  undefined1 local_148;
  undefined1 local_144;
  undefined1 local_140;
  undefined1 local_13c;
  undefined1 local_138;
  undefined1 local_134;
  undefined1 local_130;
  undefined1 local_12c;
  undefined1 local_128;
  undefined1 local_124;
  undefined1 local_120;
  undefined1 local_11c;
  undefined1 local_118;
  undefined1 local_114;
  undefined1 local_110;
  undefined1 local_10c;
  undefined1 local_108;
  undefined1 local_104;
  undefined1 local_100;
  undefined1 local_fc;
  undefined1 local_f8;
  undefined1 local_f4;
  undefined1 local_f0;
  undefined1 local_ec;
  undefined1 local_e8;
  undefined1 local_e4;
  undefined1 local_e0;
  undefined1 local_dc;
  undefined1 local_d8;
  undefined1 local_d4;
  undefined1 local_d0;
  undefined1 local_cc;
  undefined1 local_c8;
  undefined1 local_c4;
  undefined1 local_c0;
  undefined1 local_bc;
  undefined1 local_b8;
  undefined1 local_b4;
  undefined4 local_b0 [40];
  undefined4 uStack_10;
  
  puVar2 = local_b0;
  puVar8 = &DAT_0121a8fc;
  puVar1 = puVar8;
  do {
    uVar7 = *puVar1;
    uVar6 = puVar1[1];
    uVar5 = puVar1[2];
    uVar4 = puVar1[3];
    puVar1 = puVar1 + 4;
    *puVar2 = uVar7;
    puVar2[1] = uVar6;
    puVar2[2] = uVar5;
    puVar2[3] = uVar4;
    puVar2 = puVar2 + 4;
  } while (puVar1 != (undefined4 *)&DAT_0121a99c);
  memset(&DAT_0121a8fc,0,0x262c);
  puVar1 = local_b0;
  do {
    uVar7 = *puVar1;
    uVar6 = puVar1[1];
    uVar5 = puVar1[2];
    uVar4 = puVar1[3];
    puVar1 = puVar1 + 4;
    *puVar8 = uVar7;
    puVar8[1] = uVar6;
    puVar8[2] = uVar5;
    puVar8[3] = uVar4;
    puVar8 = puVar8 + 4;
  } while (puVar1 != &uStack_10);
  DAT_0121a910 = 0;
  DAT_0121a918._1_1_ = 0;
  DAT_0121a914 = 0xf;
  DAT_0121a99e = 0x32;
  DAT_0121a9a4 = 0x32;
  DAT_0121a9a6 = 0x32;
  DAT_0121a9a7 = 0x32;
  DAT_0121a9a9 = 0x32;
  DAT_0121a9aa = 0x32;
  DAT_0121a99c = 3;
  DAT_0121a9a8 = 5;
  DAT_0121a9ab = 1;
  puVar1 = &DAT_00451310;
  puVar2 = local_1c8;
  do {
    puVar3 = puVar2;
    puVar8 = puVar1;
    uVar6 = puVar8[1];
    uVar5 = puVar8[2];
    uVar4 = puVar8[3];
    *puVar3 = *puVar8;
    puVar3[1] = uVar6;
    puVar3[2] = uVar5;
    puVar3[3] = uVar4;
    puVar1 = puVar8 + 4;
    puVar2 = puVar3 + 4;
  } while (puVar8 + 4 != (undefined4 *)&UNK_00451420);
  uVar4 = puVar8[5];
  puVar3[4] = 0;
  puVar3[5] = uVar4;
  DAT_0121a9b2 = (undefined1)local_1c8[0];
  DAT_0121a9b3 = (undefined1)local_1c8[1];
  DAT_0121a9b4 = (undefined1)local_1c8[2];
  DAT_0121a9b5 = (undefined1)local_1c8[3];
  DAT_0121a9b6 = local_1b8[0];
  DAT_0121a9b7 = local_1b4[0];
  DAT_0121a9b8 = local_1b0;
  DAT_0121a9b9 = local_1ac;
  DAT_0121a9ba = local_1a8;
  DAT_0121a9bb = local_1a4;
  DAT_0121a9bc = local_1a0;
  DAT_0121a9bd = local_19c;
  DAT_0121a9be = local_198;
  DAT_0121a9bf = local_194;
  DAT_0121a9c0 = local_190;
  DAT_0121a9c1 = local_18c;
  DAT_0121a9c2 = local_188;
  DAT_0121a9c3 = local_184;
  DAT_0121a9c4 = local_180;
  DAT_0121a9c5 = local_17c;
  DAT_0121a9c6 = local_178;
  DAT_0121a9c7 = local_174;
  DAT_0121a9c8 = local_170;
  DAT_0121a9c9 = local_16c;
  DAT_0121a9ca = local_168;
  DAT_0121a9cb = local_164;
  DAT_0121a9cc = local_160;
  DAT_0121a9cd = local_15c;
  DAT_0121a9ce = local_158;
  DAT_0121a9cf = local_154;
  DAT_0121a9d0 = local_150;
  DAT_0121a9d1 = local_14c;
  DAT_0121a9d2 = local_148;
  DAT_0121a9d3 = local_144;
  DAT_0121a9d4 = local_140;
  DAT_0121a9d5 = local_13c;
  DAT_0121a9d6 = local_138;
  DAT_0121a9d7 = local_134;
  DAT_0121a9d8 = local_130;
  DAT_0121a9d9 = local_12c;
  DAT_0121a9da = local_128;
  DAT_0121a9db = local_124;
  DAT_0121a9dc = local_120;
  DAT_0121a9dd = local_11c;
  DAT_0121a9de = local_118;
  DAT_0121a9df = local_114;
  DAT_0121a9e0 = local_110;
  DAT_0121a9e1 = local_10c;
  DAT_0121a9e2 = local_108;
  DAT_0121a9e3 = local_104;
  DAT_0121a9e4 = local_100;
  DAT_0121a9e5 = local_fc;
  DAT_0121a9e6 = local_f8;
  DAT_0121a9e7 = local_f4;
  DAT_0121a9e8 = local_f0;
  DAT_0121a9e9 = local_ec;
  DAT_0121a9ea = local_e8;
  DAT_0121a9eb = local_e4;
  DAT_0121a9ec = local_e0;
  DAT_0121a9ed = local_dc;
  DAT_0121a9ee = local_d8;
  DAT_0121a9ef = local_d4;
  DAT_0121aa60 = 1;
  DAT_0121aa64 = 1;
  DAT_0121a9f0 = local_d0;
  DAT_0121aa68 = 1;
  DAT_0121aa70 = 1;
  DAT_0121a9f1 = local_cc;
  DAT_0121aa78 = 1;
  DAT_0121aa80 = 1;
  DAT_0121a9f2 = local_c8;
  DAT_0121aa84 = 1;
  DAT_0121aa58 = 2;
  DAT_0121a9f3 = local_c4;
  DAT_0121aa6c = 2;
  DAT_0121aa7c = 0x24;
  DAT_0121a9f4 = local_c0;
  DAT_0121a9f5 = local_bc;
  DAT_0121a9f6 = local_b8;
  DAT_0121a9f7 = local_b4;
  DAT_0121aa10 = 0;
  DAT_0121a9fc = 1;
  DAT_0121a9fd = 2;
  DAT_0121aa09 = 3;
  DAT_0121aa0a = 500;
  DAT_0121a9a0 = 6;
  DAT_0121aa14 = 0;
  DAT_0121aa1c = 0;
  DAT_0121aa20 = 0;
  DAT_0121aa24 = 0;
  DAT_0121aa28 = 0;
  DAT_0121aa2c = 0;
  DAT_0121aa18 = 0;
  DAT_0121aa30 = 0x34333231;
  DAT_0121aa38 = 0;
  DAT_0121aa34 = 0x38373635;
  memset(&DAT_0121aa8c,0,0x1040);
  memset(&DAT_0121bacc,0,0x1040);
  memset(&DAT_0121cb0c,0,0x410);
  DAT_0121cf24 = 2;
  return;
}



/* ===== FUNCTION FUN_00427b7c @ 00427b7c ===== */

void FUN_00427b7c(void)

{
  int iVar1;
  int iVar2;
  undefined2 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  undefined4 *puVar11;
  undefined2 *local_18 [2];
  
  printf("%s(), line:%d.\n","projector_factory_reset",0xf1);
  iVar1 = open("/dev/persistentmem",0x4012);
  if (-1 < iVar1) {
    iVar2 = ioctl(iVar1,0x20002601,2);
    if (iVar2 < 0) {
      printf("%s(), line:%d. delete app data failed\n","projector_factory_reset",0xfa);
    }
    close(iVar1);
    puVar10 = &DAT_0121a8fc;
    DAT_0121a918._1_1_ = 0;
    iVar1 = memcmp(&DAT_0121a8fc,&DAT_012182d0,0xa0);
    if (iVar1 != 0) {
      puVar3 = malloc(0xc);
      puVar4 = malloc(0xa0);
      if ((puVar3 == (undefined2 *)0x0) || (puVar4 == (undefined4 *)0x0)) {
        printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
      }
      else {
        puVar3[1] = 0;
        *puVar3 = 1;
        puVar3[2] = 0xa0;
        puVar11 = puVar10;
        puVar5 = puVar4;
        do {
          uVar9 = *puVar11;
          uVar8 = puVar11[1];
          uVar7 = puVar11[2];
          uVar6 = puVar11[3];
          puVar11 = puVar11 + 4;
          *puVar5 = uVar9;
          puVar5[1] = uVar8;
          puVar5[2] = uVar7;
          puVar5[3] = uVar6;
          puVar5 = puVar5 + 4;
        } while (puVar11 != (undefined4 *)&DAT_0121a99c);
        puVar11 = &DAT_012182d0;
        *(undefined4 **)(puVar3 + 4) = puVar4;
        do {
          uVar9 = *puVar10;
          uVar8 = puVar10[1];
          uVar7 = puVar10[2];
          uVar6 = puVar10[3];
          puVar10 = puVar10 + 4;
          *puVar11 = uVar9;
          puVar11[1] = uVar8;
          puVar11[2] = uVar7;
          puVar11[3] = uVar6;
          puVar11 = puVar11 + 4;
        } while (puVar10 != (undefined4 *)&DAT_0121a99c);
        local_18[0] = puVar3;
        if ((DAT_012146b4 != -1) || (DAT_012146b4 = FUN_0042a538(100,4), DAT_012146b4 != -1)) {
          FUN_0042a628(DAT_012146b4,local_18,4);
          sem_post((sem_t *)&DAT_0121ed4c);
        }
      }
    }
    usleep(100000);
    return;
  }
  printf("Open /dev/persistentmem failed (%d)\n",iVar1);
  return;
}



/* ===== FUNCTION FUN_00427d80 @ 00427d80 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00427d80(void)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 *puVar10;
  undefined4 local_28;
  undefined2 local_24;
  undefined4 *local_20;
  undefined4 local_1c;
  
  iVar1 = open("/dev/persistentmem",2);
  if (iVar1 < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",iVar1);
  }
  else {
    local_24 = 0xa0;
    local_20 = &DAT_0121a8fc;
    local_28 = 1;
    FUN_0042eee8();
    iVar2 = ioctl(iVar1,0x400c2602,&local_28);
    if (iVar2 < 0) {
      local_1c = &DAT_00a00001;
      iVar2 = ioctl(iVar1,0x80042600,&local_1c);
      if (iVar2 < 0) {
        FUN_0042eef4();
        puts("create sys_data failed");
        close(iVar1);
      }
      else {
        FUN_0042eef4();
        close(iVar1);
        local_20 = &DAT_0121a8fc;
        local_28 = 1;
        local_1c = (undefined *)CONCAT22(0xa0,(undefined2)local_1c);
        FUN_00427270(&local_28);
      }
    }
    else {
      FUN_0042eef4();
      close(iVar1);
    }
  }
  puVar3 = &DAT_0121a8fc;
  puVar10 = &DAT_012182d0;
  do {
    uVar9 = *puVar3;
    uVar8 = puVar3[1];
    uVar7 = puVar3[2];
    uVar6 = puVar3[3];
    puVar3 = puVar3 + 4;
    *puVar10 = uVar9;
    puVar10[1] = uVar8;
    puVar10[2] = uVar7;
    puVar10[3] = uVar6;
    puVar10 = puVar10 + 4;
  } while (puVar3 != (undefined4 *)&DAT_0121a99c);
  iVar1 = open("/dev/persistentmem",2);
  if (iVar1 < 0) {
    printf("Open /dev/persistentmem failed (%d)\n",iVar1);
    uVar6 = 0xffffffff;
  }
  else {
    local_20 = (undefined4 *)&DAT_0121a99c;
    local_28 = 2;
    local_24 = 0x258c;
    FUN_0042eee8();
    iVar2 = ioctl(iVar1,0x400c2602,&local_28);
    if (iVar2 < 0) {
      local_1c = (undefined *)0x258c0002;
      iVar2 = ioctl(iVar1,0x80042600,&local_1c);
      if (iVar2 < 0) {
        puts("get sys_data failed");
        FUN_0042eef4();
        close(iVar1);
        uVar6 = 0xffffffff;
      }
      else {
        close(iVar1);
        FUN_0042eef4();
        local_28 = 2;
        local_24 = 0x258c;
        local_20 = (undefined4 *)&DAT_0121a99c;
        FUN_00427270(&local_28);
        puVar3 = &DAT_01218370;
        puVar10 = (undefined4 *)&DAT_0121a99c;
        do {
          puVar5 = puVar10;
          puVar4 = puVar3;
          uVar8 = puVar5[1];
          uVar7 = puVar5[2];
          uVar6 = puVar5[3];
          *puVar4 = *puVar5;
          puVar4[1] = uVar8;
          puVar4[2] = uVar7;
          puVar4[3] = uVar6;
          puVar3 = puVar4 + 4;
          puVar10 = puVar5 + 4;
        } while (puVar5 + 4 != (undefined4 *)&DAT_0121cf1c);
        uVar6 = 0;
        uVar8 = puVar5[5];
        uVar7 = puVar5[6];
        puVar4[4] = _DAT_0121cf1c;
        puVar4[5] = uVar8;
        puVar4[6] = uVar7;
      }
    }
    else {
      FUN_0042eef4();
      puVar3 = (undefined4 *)&DAT_0121a99c;
      puVar10 = &DAT_01218370;
      do {
        puVar5 = puVar10;
        puVar4 = puVar3;
        uVar8 = puVar4[1];
        uVar7 = puVar4[2];
        uVar6 = puVar4[3];
        *puVar5 = *puVar4;
        puVar5[1] = uVar8;
        puVar5[2] = uVar7;
        puVar5[3] = uVar6;
        puVar3 = puVar4 + 4;
        puVar10 = puVar5 + 4;
      } while (puVar4 + 4 != (undefined4 *)&DAT_0121cf1c);
      uVar7 = puVar4[5];
      uVar6 = puVar4[6];
      puVar5[4] = _DAT_0121cf1c;
      puVar5[5] = uVar7;
      puVar5[6] = uVar6;
      close(iVar1);
      uVar6 = 0;
    }
  }
  return uVar6;
}



/* ===== FUNCTION FUN_00428080 @ 00428080 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00428080(void)

{
  int iVar1;
  int iVar2;
  undefined2 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined2 *local_c8 [8];
  undefined1 local_a8;
  undefined2 local_28;
  undefined2 local_26;
  undefined2 local_24;
  undefined2 **local_20;
  
  iVar1 = open("/dev/persistentmem",0x4012);
  if (iVar1 < 0) {
    puts("open /dev/persistentmem failed");
  }
  else {
    local_26 = 0;
    local_28 = 1;
    local_24 = 0xa0;
    local_20 = local_c8;
    iVar2 = ioctl(iVar1,0x400c2602,&local_28);
    if (iVar2 < 0) {
      close(iVar1);
    }
    else {
      close(iVar1);
      DAT_0121a91c = local_a8;
    }
  }
  iVar1 = memcmp(&DAT_0121a8fc,&DAT_012182d0,0xa0);
  if (iVar1 != 0) {
    puVar3 = malloc(0xc);
    puVar4 = malloc(0xa0);
    if ((puVar3 == (undefined2 *)0x0) || (puVar5 = &DAT_0121a8fc, puVar4 == (undefined4 *)0x0)) {
      printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
    }
    else {
      puVar3[1] = 0;
      *puVar3 = 1;
      puVar3[2] = 0xa0;
      puVar6 = puVar5;
      puVar7 = puVar4;
      do {
        uVar11 = *puVar6;
        uVar12 = puVar6[1];
        uVar10 = puVar6[2];
        uVar9 = puVar6[3];
        puVar6 = puVar6 + 4;
        *puVar7 = uVar11;
        puVar7[1] = uVar12;
        puVar7[2] = uVar10;
        puVar7[3] = uVar9;
        puVar7 = puVar7 + 4;
      } while (puVar6 != (undefined4 *)&DAT_0121a99c);
      puVar6 = &DAT_012182d0;
      *(undefined4 **)(puVar3 + 4) = puVar4;
      do {
        uVar11 = *puVar5;
        uVar12 = puVar5[1];
        uVar10 = puVar5[2];
        uVar9 = puVar5[3];
        puVar5 = puVar5 + 4;
        *puVar6 = uVar11;
        puVar6[1] = uVar12;
        puVar6[2] = uVar10;
        puVar6[3] = uVar9;
        puVar6 = puVar6 + 4;
      } while (puVar5 != (undefined4 *)&DAT_0121a99c);
      local_c8[0] = puVar3;
      if ((DAT_012146b4 != -1) || (DAT_012146b4 = FUN_0042a538(100,4), DAT_012146b4 != -1)) {
        FUN_0042a628(DAT_012146b4,local_c8,4);
        sem_post((sem_t *)&DAT_0121ed4c);
      }
    }
  }
  iVar1 = memcmp(&DAT_0121a99c,&DAT_01218370,0x258c);
  if (iVar1 != 0) {
    puVar3 = malloc(0xc);
    puVar4 = malloc(0x258c);
    if ((puVar3 == (undefined2 *)0x0) || (puVar4 == (undefined4 *)0x0)) {
      printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
    }
    else {
      puVar3[1] = 0;
      *puVar3 = 2;
      puVar3[2] = 0x258c;
      puVar5 = (undefined4 *)&DAT_0121a99c;
      puVar6 = puVar4;
      do {
        puVar8 = puVar6;
        puVar7 = puVar5;
        uVar12 = puVar7[1];
        uVar10 = puVar7[2];
        uVar9 = puVar7[3];
        *puVar8 = *puVar7;
        puVar8[1] = uVar12;
        puVar8[2] = uVar10;
        puVar8[3] = uVar9;
        puVar5 = puVar7 + 4;
        puVar6 = puVar8 + 4;
      } while (puVar7 + 4 != (undefined4 *)&DAT_0121cf1c);
      uVar10 = puVar7[5];
      uVar9 = puVar7[6];
      puVar8[4] = _DAT_0121cf1c;
      puVar8[5] = uVar10;
      puVar8[6] = uVar9;
      *(undefined4 **)(puVar3 + 4) = puVar4;
      puVar4 = (undefined4 *)&DAT_0121a99c;
      puVar5 = &DAT_01218370;
      do {
        puVar7 = puVar5;
        puVar6 = puVar4;
        uVar12 = puVar6[1];
        uVar10 = puVar6[2];
        uVar9 = puVar6[3];
        *puVar7 = *puVar6;
        puVar7[1] = uVar12;
        puVar7[2] = uVar10;
        puVar7[3] = uVar9;
        iVar1 = DAT_012146b4;
        puVar4 = puVar6 + 4;
        puVar5 = puVar7 + 4;
      } while (puVar6 + 4 != (undefined4 *)&DAT_0121cf1c);
      uVar10 = puVar6[5];
      uVar9 = puVar6[6];
      puVar7[4] = _DAT_0121cf1c;
      puVar7[5] = uVar10;
      puVar7[6] = uVar9;
      local_c8[0] = puVar3;
      if ((iVar1 != -1) || (iVar1 = FUN_0042a538(100,4), DAT_012146b4 = iVar1, iVar1 != -1)) {
        FUN_0042a628(iVar1,local_c8,4);
        sem_post((sem_t *)&DAT_0121ed4c);
        return 0;
      }
      DAT_012146b4 = -1;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00428400 @ 00428400 ===== */

undefined4 * FUN_00428400(void)

{
  return &DAT_0121a8fc;
}



/* ===== FUNCTION FUN_0042840c @ 0042840c ===== */

undefined * FUN_0042840c(void)

{
  snprintf(&DAT_012182b0,0x20,"%s-%u",&DAT_0121a8fc,DAT_0121a90c);
  return &DAT_012182b0;
}



/* ===== FUNCTION FUN_00428454 @ 00428454 ===== */

undefined4 * FUN_00428454(undefined4 param_1)

{
  switch(param_1) {
  case 0:
    return (undefined4 *)(uint)DAT_0121a9a5;
  case 1:
    return (undefined4 *)(uint)DAT_0121a9a6;
  case 2:
    return (undefined4 *)(uint)DAT_0121a9a7;
  case 3:
    return (undefined4 *)(uint)DAT_0121a9a8;
  case 4:
    return (undefined4 *)(uint)DAT_0121a9a9;
  case 5:
    return (undefined4 *)(uint)DAT_0121a9aa;
  case 6:
    return (undefined4 *)(uint)DAT_0121a9ab;
  case 7:
    return (undefined4 *)(uint)DAT_0121a9ac;
  case 8:
    return (undefined4 *)(uint)DAT_0121a9ad;
  case 9:
    return (undefined4 *)(int)DAT_0121a9f8;
  case 10:
    return (undefined4 *)(int)DAT_0121a9f9;
  case 0xb:
    return (undefined4 *)(int)DAT_0121a9ae;
  case 0xc:
    return (undefined4 *)(uint)DAT_0121a9af;
  default:
    return (undefined4 *)0xffffffff;
  case 0xf:
    return (undefined4 *)(int)DAT_0121a9b0;
  case 0x10:
    return (undefined4 *)(int)DAT_0121a9b1;
  case 0x11:
    return (undefined4 *)(uint)DAT_0121a9fc;
  case 0x12:
    return (undefined4 *)(uint)DAT_0121a9fd;
  case 0x13:
    return (undefined4 *)(uint)DAT_0121a99c;
  case 0x14:
    return (undefined4 *)(uint)DAT_0121a918._1_1_;
  case 0x15:
    return (undefined4 *)(uint)DAT_0121aa09;
  case 0x16:
    return (undefined4 *)(uint)DAT_0121a99e;
  case 0x19:
    return (undefined4 *)(uint)DAT_0121aa08;
  case 0x1b:
    return DAT_0121aa00;
  case 0x1c:
    return DAT_0121aa04;
  case 0x1e:
    return &DAT_0121a8fc;
  case 0x1f:
    return DAT_0121a90c;
  case 0x20:
    return DAT_0121aa60;
  case 0x21:
    return DAT_0121aa68;
  case 0x22:
    return DAT_0121aa64;
  case 0x23:
    return DAT_0121aa58;
  case 0x24:
    return DAT_0121a9a0;
  case 0x26:
    return &DAT_0121aa30;
  case 0x27:
    return DAT_0121aa6c;
  case 0x28:
    break;
  case 0x29:
    return DAT_0121aa74;
  case 0x2a:
    return DAT_0121aa78;
  case 0x2b:
    return DAT_0121a914;
  case 0x2c:
    return (undefined4 *)(uint)DAT_0121a99f;
  case 0x2f:
    return DAT_0121aa70;
  case 0x30:
    return DAT_0121aa7c;
  case 0x38:
    return DAT_0121aa88;
  case 0x39:
    return (undefined4 *)(uint)DAT_0121aa0a;
  case 0x3a:
    return (undefined4 *)(int)DAT_0121a9fa;
  case 0x3b:
    return (undefined4 *)(int)DAT_0121a9fb;
  case 0x3c:
    return DAT_0121aa80;
  case 0x3d:
    return (undefined4 *)(uint)DAT_0121a9a4;
  case 0x3e:
    return (undefined4 *)(uint)DAT_0121cf24;
  case 0x3f:
    return DAT_0121aa84;
  case 0x40:
    return (undefined4 *)(uint)DAT_0121cf1c;
  case 0x41:
    return DAT_0121cf20;
  }
  if (DAT_0121aa6c == (undefined4 *)0x2) {
    return DAT_0121aa7c;
  }
  return DAT_0121aa70;
}



/* ===== FUNCTION FUN_004286f8 @ 004286f8 ===== */

void FUN_004286f8(undefined4 param_1,char *param_2)

{
  undefined1 uVar1;
  
  uVar1 = SUB41(param_2,0);
  switch(param_1) {
  case 0:
    DAT_0121a9a5 = uVar1;
    return;
  case 1:
    DAT_0121a9a6 = uVar1;
    return;
  case 2:
    DAT_0121a9a7 = uVar1;
    return;
  case 3:
    DAT_0121a9a8 = uVar1;
    return;
  case 4:
    DAT_0121a9a9 = uVar1;
    return;
  case 5:
    DAT_0121a9aa = uVar1;
    return;
  case 6:
    DAT_0121a9ab = uVar1;
    return;
  case 7:
    DAT_0121a9ac = uVar1;
    return;
  case 8:
    DAT_0121a9ad = uVar1;
    return;
  case 9:
    DAT_0121a9f8 = uVar1;
    return;
  case 10:
    DAT_0121a9f9 = uVar1;
    return;
  case 0xb:
    DAT_0121a9ae = uVar1;
    return;
  case 0xc:
    DAT_0121a9af = uVar1;
    return;
  default:
    return;
  case 0xf:
    DAT_0121a9b0 = uVar1;
    return;
  case 0x10:
    DAT_0121a9b1 = uVar1;
    return;
  case 0x11:
    DAT_0121a9fc = uVar1;
    return;
  case 0x12:
    DAT_0121a9fd = uVar1;
    return;
  case 0x13:
    DAT_0121a99c = uVar1;
    return;
  case 0x14:
    DAT_0121a918._1_1_ = uVar1;
    return;
  case 0x15:
    DAT_0121aa09 = uVar1;
    return;
  case 0x16:
    DAT_0121a99e = uVar1;
    return;
  case 0x19:
    DAT_0121aa08 = uVar1;
    return;
  case 0x1b:
    DAT_0121aa00 = param_2;
    return;
  case 0x1c:
    DAT_0121aa04 = param_2;
    return;
  case 0x20:
    DAT_0121aa60 = param_2;
    return;
  case 0x21:
    DAT_0121aa68 = param_2;
    return;
  case 0x22:
    DAT_0121aa64 = param_2;
    return;
  case 0x23:
    DAT_0121aa58 = param_2;
    return;
  case 0x24:
    DAT_0121a9a0 = param_2;
    return;
  case 0x25:
    DAT_0121aa56 = 1;
    strncpy((char *)&DAT_0121aa10,param_2,0x20);
    return;
  case 0x26:
    strncpy((char *)&DAT_0121aa30,param_2,0x20);
    return;
  case 0x27:
    DAT_0121aa6c = param_2;
    return;
  case 0x28:
    break;
  case 0x29:
    DAT_0121aa74 = param_2;
    return;
  case 0x2a:
    DAT_0121aa78 = param_2;
    return;
  case 0x2c:
    DAT_0121a99f = uVar1;
    return;
  case 0x38:
    DAT_0121aa88 = param_2;
    return;
  case 0x39:
    DAT_0121aa0a = (short)param_2;
    return;
  case 0x3a:
    DAT_0121a9fa = uVar1;
    return;
  case 0x3b:
    DAT_0121a9fb = uVar1;
    return;
  case 0x3c:
    DAT_0121aa80 = param_2;
    return;
  case 0x3d:
    DAT_0121a9a4 = uVar1;
    return;
  case 0x3f:
    DAT_0121aa84 = param_2;
    return;
  case 0x40:
    DAT_0121cf1c = uVar1;
    return;
  case 0x41:
    DAT_0121cf20 = param_2;
    return;
  }
  if ((int)param_2 < 0x22) {
    DAT_0121aa70 = param_2;
    return;
  }
  DAT_0121aa7c = param_2;
  return;
}



/* ===== FUNCTION FUN_00428968 @ 00428968 ===== */

void FUN_00428968(uint param_1,int *param_2)

{
  int iVar1;
  
  if (param_1 < 7) {
    iVar1 = param_1 * 10;
    *param_2 = (int)(char)(&DAT_0121a9b2)[iVar1];
    param_2[1] = (int)(char)(&DAT_0121a9b3)[iVar1];
    param_2[2] = (int)(char)(&DAT_0121a9b4)[iVar1];
    param_2[3] = (int)(char)(&DAT_0121a9b5)[iVar1];
    param_2[4] = (int)(char)(&DAT_0121a9b6)[iVar1];
    param_2[5] = (int)(char)(&DAT_0121a9b7)[iVar1];
    param_2[6] = (int)(char)(&DAT_0121a9b8)[iVar1];
    param_2[7] = (int)(char)(&DAT_0121a9b9)[iVar1];
    param_2[8] = (int)(char)(&DAT_0121a9ba)[iVar1];
    param_2[9] = (int)(char)(&DAT_0121a9bb)[iVar1];
  }
  return;
}



/* ===== FUNCTION FUN_004289e4 @ 004289e4 ===== */

void FUN_004289e4(uint param_1,uint param_2,undefined1 param_3)

{
  if ((param_1 < 7) && (param_2 < 10)) {
    (&DAT_0121a9b2)[param_1 * 10 + param_2] = param_3;
  }
  return;
}



/* ===== FUNCTION FUN_00428a24 @ 00428a24 ===== */

void FUN_00428a24(int param_1)

{
  DAT_0121a9a0 = param_1;
  if (param_1 != 6) {
    DAT_0121a914 = FUN_00437c38();
    return;
  }
  DAT_0121a914 = FUN_00437c2c();
  return;
}



/* ===== FUNCTION FUN_00428a7c @ 00428a7c ===== */

undefined4 * FUN_00428a7c(char *param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  
  if (param_2 != 1) {
    if (param_2 == 0) {
      iVar1 = strncmp(&DAT_0121aa9c,param_1,0x3ff);
      if (iVar1 == 0) {
        iVar1 = 0;
      }
      else {
        iVar1 = strncmp(&DAT_0121aeac,param_1,0x3ff);
        if (iVar1 == 0) {
          iVar1 = 0x410;
        }
        else {
          iVar1 = strncmp(&DAT_0121b2bc,param_1,0x3ff);
          if (iVar1 == 0) {
            iVar1 = 0x820;
          }
          else {
            iVar1 = strncmp(&DAT_0121b6cc,param_1,0x3ff);
            if (iVar1 != 0) {
              return (undefined4 *)0x0;
            }
            iVar1 = 0xc30;
          }
        }
      }
      puVar2 = (undefined4 *)(&DAT_0121aa8c + iVar1);
    }
    else {
      if (param_2 != 3) {
        return (undefined4 *)0x0;
      }
      iVar1 = strncmp(&DAT_0121badc,param_1,0x3ff);
      if (iVar1 == 0) {
        iVar1 = 0;
      }
      else {
        iVar1 = strncmp(&DAT_0121beec,param_1,0x3ff);
        if (iVar1 == 0) {
          iVar1 = 0x410;
        }
        else {
          iVar1 = strncmp(&DAT_0121c2fc,param_1,0x3ff);
          if (iVar1 == 0) {
            iVar1 = 0x820;
          }
          else {
            iVar1 = strncmp(&DAT_0121c70c,param_1,0x3ff);
            if (iVar1 != 0) {
              return (undefined4 *)0x0;
            }
            iVar1 = 0xc30;
          }
        }
      }
      puVar2 = (undefined4 *)((int)&DAT_0121bacc + iVar1);
    }
    return puVar2;
  }
  iVar1 = strncmp(&DAT_0121cb1c,param_1,0x3ff);
  if (iVar1 != 0) {
    return (undefined4 *)0x0;
  }
  return &DAT_0121cb0c;
}



/* ===== FUNCTION FUN_00428c28 @ 00428c28 ===== */

undefined4 * FUN_00428c28(int param_1)

{
  if (param_1 == 1) {
    return &DAT_0121cb0c;
  }
  if (param_1 != 0) {
    if (param_1 == 3) {
      return &DAT_0121bacc;
    }
    return (undefined4 *)0x0;
  }
  return (undefined4 *)&DAT_0121aa8c;
}



/* ===== FUNCTION FUN_00428c70 @ 00428c70 ===== */

void FUN_00428c70(int *param_1)

{
  int iVar1;
  size_t sVar2;
  int iVar3;
  int *__s;
  int *piVar4;
  int *__s_00;
  undefined2 uVar5;
  undefined2 local_38;
  undefined2 local_36;
  undefined4 local_34;
  undefined4 *local_30;
  
  DAT_0121aa88 = *param_1;
  local_38 = 2;
  local_36 = 0xec;
  local_34 = 4;
  local_30 = &DAT_0121aa88;
  FUN_00427270(&local_38);
  iVar1 = *param_1;
  if (iVar1 == 0) {
    iVar1 = 0xf0;
    uVar5 = 0xf0;
    piVar4 = (int *)&DAT_0121aa8c;
  }
  else {
    if (iVar1 != 3) {
      piVar4 = param_1 + 4;
      if (iVar1 != 1) {
        return;
      }
      sVar2 = strlen((char *)piVar4);
      iVar1 = memcmp(&DAT_0121cb1c,piVar4,sVar2 + 1);
      if (iVar1 == 0) {
        return;
      }
      memcpy(&DAT_0121cb1c,piVar4,sVar2 + 1);
      DAT_0121cb10 = param_1[1];
      local_36 = 0x2170;
      local_34 = 0x410;
      DAT_0121cb14 = param_1[2];
      local_38 = 2;
      DAT_0121cb0c = *param_1;
      local_30 = &DAT_0121cb0c;
      DAT_0121cb18 = param_1[3];
      FUN_00427270(&local_38);
      return;
    }
    iVar1 = 0x1130;
    uVar5 = 0x1130;
    piVar4 = &DAT_0121bacc;
  }
  __s_00 = param_1 + 4;
  sVar2 = strlen((char *)__s_00);
  sVar2 = sVar2 + 1;
  iVar3 = memcmp(piVar4 + 4,__s_00,sVar2);
  if (iVar3 != 0) {
    iVar3 = memcmp(piVar4 + 0x108,__s_00,sVar2);
    if (iVar3 == 0) {
      iVar3 = 1;
    }
    else {
      iVar3 = memcmp(piVar4 + 0x20c,__s_00,sVar2);
      if (iVar3 == 0) {
        iVar3 = 2;
      }
      else {
        iVar3 = 3;
      }
    }
    __s = piVar4 + iVar3 * 0x104;
    do {
      while( true ) {
        iVar3 = iVar3 + -1;
        if ((char)__s[-0x100] == '\0') break;
        memset(__s,0,0x410);
        *__s = __s[-0x104];
        __s[1] = __s[-0x103];
        __s[2] = __s[-0x102];
        __s[3] = __s[-0x101];
        sVar2 = strlen((char *)(__s + -0x100));
        strncpy((char *)(__s + 4),(char *)(__s + -0x100),sVar2 + 1);
        __s = __s + -0x104;
        if (iVar3 == 0) goto LAB_00428e68;
      }
      __s = __s + -0x104;
    } while (iVar3 != 0);
  }
LAB_00428e68:
  memset(piVar4,0,0x410);
  *piVar4 = *param_1;
  piVar4[1] = param_1[1];
  piVar4[2] = param_1[2];
  piVar4[3] = param_1[3];
  sVar2 = strlen((char *)__s_00);
  strncpy((char *)(piVar4 + 4),(char *)__s_00,sVar2 + 1);
  local_38 = 2;
  local_30 = (undefined4 *)(&DAT_0121a99c + iVar1);
  local_34 = 0x1040;
  local_36 = uVar5;
  FUN_00427270(&local_38);
  return;
}



/* ===== FUNCTION FUN_00428f34 @ 00428f34 ===== */

void FUN_00428f34(char *param_1,int param_2,undefined4 param_3)

{
  size_t sVar1;
  int iVar2;
  undefined2 *puVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  char *pcVar9;
  undefined4 *puVar10;
  undefined4 *__s2;
  undefined2 uVar11;
  undefined4 local_438;
  undefined4 local_434;
  undefined4 *local_430;
  undefined4 local_428 [256];
  undefined2 *local_28;
  char local_24;
  
  local_24 = '\0';
  if (param_2 == 0) {
    sVar1 = strlen(param_1);
    iVar2 = memcmp(&DAT_0121aa9c,param_1,sVar1);
    if (iVar2 != 0) {
      local_24 = '\x01';
    }
    uVar11 = 0xf4;
    if (local_24 != '\0') {
LAB_0042904c:
      memset(&local_438,0,0x410);
      puVar10 = local_428;
      pcVar9 = param_1 + 0x400;
      do {
        uVar8 = *(undefined4 *)param_1;
        uVar7 = *(undefined4 *)(param_1 + 4);
        uVar6 = *(undefined4 *)(param_1 + 8);
        uVar5 = *(undefined4 *)(param_1 + 0xc);
        param_1 = param_1 + 0x10;
        *puVar10 = uVar8;
        puVar10[1] = uVar7;
        puVar10[2] = uVar6;
        puVar10[3] = uVar5;
        puVar10 = puVar10 + 4;
      } while (param_1 != pcVar9);
      local_438 = param_2;
      local_434 = param_3;
      FUN_00428c70(&local_438);
      return;
    }
    puVar10 = &DAT_0121aa90;
    __s2 = &DAT_01218464;
    DAT_0121aa90 = param_3;
  }
  else if (param_2 == 3) {
    sVar1 = strlen(param_1);
    iVar2 = memcmp(&DAT_0121badc,param_1,sVar1);
    if (iVar2 != 0) {
      local_24 = '\x01';
    }
    uVar11 = 0x1134;
    if (local_24 != '\0') goto LAB_0042904c;
    puVar10 = &DAT_0121bad0;
    __s2 = (undefined4 *)&DAT_012194a4;
    DAT_0121bad0 = param_3;
  }
  else if (param_2 == 1) {
    sVar1 = strlen(param_1);
    iVar2 = memcmp(&DAT_0121cb1c,param_1,sVar1);
    if (iVar2 != 0) {
      local_24 = '\x01';
    }
    uVar11 = 0x2174;
    if (local_24 != '\0') goto LAB_0042904c;
    puVar10 = &DAT_0121cb10;
    __s2 = (undefined4 *)&DAT_0121a4e4;
    DAT_0121cb10 = param_3;
  }
  else {
    uVar11 = 0;
    __s2 = &DAT_01218370;
    puVar10 = (undefined4 *)&DAT_0121a99c;
  }
  local_438 = CONCAT22(uVar11,2);
  local_434 = 4;
  local_430 = puVar10;
  iVar2 = memcmp(puVar10,__s2,4);
  if (iVar2 != 0) {
    puVar3 = malloc(0xc);
    puVar4 = malloc(4);
    if ((puVar3 == (undefined2 *)0x0) || (puVar4 == (undefined4 *)0x0)) {
      printf("%s(), line:%d. malloc memory fail!\n","projector_memory_save",0x611);
    }
    else {
      uVar5 = *puVar10;
      *puVar3 = 2;
      puVar3[1] = uVar11;
      *puVar4 = uVar5;
      *__s2 = uVar5;
      puVar3[2] = 4;
      *(undefined4 **)(puVar3 + 4) = puVar4;
      local_28 = puVar3;
      if ((DAT_012146b4 != -1) || (DAT_012146b4 = FUN_0042a538(100,4), DAT_012146b4 != -1)) {
        FUN_0042a628(DAT_012146b4,&local_28,4);
        sem_post((sem_t *)&DAT_0121ed4c);
        return;
      }
      DAT_012146b4 = -1;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0042922c @ 0042922c ===== */

void FUN_0042922c(char *param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 local_420;
  undefined4 local_41c;
  undefined4 *local_418;
  undefined4 local_414;
  char acStack_410 [1028];
  
  iVar1 = strncmp(&DAT_0121badc,param_1,0x3ff);
  if (iVar1 != 0) {
    memset(&local_420,0,0x410);
    local_420 = 3;
    strncpy(acStack_410,param_1,0x3ff);
    local_414 = *param_2;
    local_41c = param_2[1];
    local_418 = (undefined4 *)param_2[2];
    FUN_00428c70(&local_420);
    return;
  }
  DAT_0121bad0 = param_2[1];
  DAT_0121bad4 = param_2[2];
  local_420 = 0x11340002;
  local_41c = 0xc;
  local_418 = &DAT_0121bad0;
  DAT_0121bad8 = *param_2;
  FUN_00427270();
  return;
}



/* ===== FUNCTION FUN_00429318 @ 00429318 ===== */

void FUN_00429318(void)

{
  int iVar1;
  pthread_attr_t pStack_30;
  pthread_t local_c [2];
  
  local_c[0] = 0;
  pthread_attr_init(&pStack_30);
  pthread_attr_setstacksize(&pStack_30,0x2000);
  pthread_attr_setdetachstate(&pStack_30,1);
  iVar1 = sem_init((sem_t *)&DAT_0121ed4c,0,0);
  if (iVar1 == -1) {
    printf("%s(), line:%d. creat sem fail\n","projector_memory_save_init",0x651);
  }
  iVar1 = pthread_create(local_c,&pStack_30,(__start_routine *)&LAB_00427414,(void *)0x0);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_30);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_004293bc @ 004293bc ===== */

undefined4 FUN_004293bc(void)

{
  int __fd;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined4 *local_10;
  undefined4 local_c;
  
  local_c = 0;
  local_18 = 1;
  local_14 = 1;
  local_10 = &local_c;
  local_16 = 0x1e;
  __fd = open("/dev/persistentmem",0x4012);
  if (__fd < 0) {
    puts("open /dev/persistentmem failed");
  }
  else {
    ioctl(__fd,0x400c2602,&local_18);
    close(__fd);
  }
  return local_c;
}



/* ===== FUNCTION FUN_00429450 @ 00429450 ===== */

void FUN_00429450(void)

{
  return;
}



/* ===== FUNCTION FUN_00429460 @ 00429460 ===== */

undefined4 FUN_00429460(void)

{
  int iVar1;
  pollfd *__fds;
  ssize_t sVar2;
  int *piVar3;
  char *pcVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  nfds_t nVar9;
  int *piVar10;
  pthread_mutex_t *__mutex;
  short *psVar11;
  int local_b8 [10];
  char acStack_90 [32];
  int local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  uint local_60;
  ssize_t local_58;
  int local_54;
  nfds_t local_50;
  char *local_4c;
  short *local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  int local_30;
  
  nVar9 = 0;
  piVar10 = local_b8;
  local_b8[0] = 0;
  local_b8[1] = 0;
  local_b8[2] = 0;
  local_b8[3] = 0;
  local_b8[4] = 0;
  local_b8[5] = 0;
  local_b8[6] = 0;
  local_b8[7] = 0;
  local_b8[8] = 0;
  local_b8[9] = 0;
  do {
    sprintf(acStack_90,"/dev/input/event%d",nVar9);
    iVar1 = open(acStack_90,0);
    *piVar10 = iVar1;
    piVar10 = piVar10 + 1;
    if (iVar1 < 0) {
      if (nVar9 == 0) {
        printf("%s(), line:%d. No Input device!!!\n","_key_task",0x208);
        return 0;
      }
      local_50 = nVar9;
      printf("%s(), input device num: %d.\n","_key_task",nVar9);
      __fds = calloc(nVar9 << 3,1);
      __fds->events = 0x41;
      __fds->fd = local_b8[0];
      if (nVar9 == 1) goto LAB_004295fc;
      __fds[1].events = 0x41;
      __fds[1].fd = local_b8[1];
      if (nVar9 != 2) goto LAB_00429560;
      goto LAB_004295fc;
    }
    nVar9 = nVar9 + 1;
  } while (nVar9 != 10);
  printf("%s(), input device num: %d.\n","_key_task",10);
  __fds = calloc(0x50,1);
  __fds->events = 0x41;
  local_50 = 10;
  __fds[1].events = 0x41;
  __fds->fd = local_b8[0];
  __fds[1].fd = local_b8[1];
LAB_00429560:
  __fds[2].events = 0x41;
  __fds[2].fd = local_b8[2];
  if (nVar9 != 3) {
    __fds[3].events = 0x41;
    __fds[3].fd = local_b8[3];
    if (nVar9 != 4) {
      __fds[4].events = 0x41;
      __fds[4].fd = local_b8[4];
      if (nVar9 != 5) {
        __fds[5].events = 0x41;
        __fds[5].fd = local_b8[5];
        if (nVar9 != 6) {
          __fds[6].events = 0x41;
          __fds[6].fd = local_b8[6];
          if (nVar9 != 7) {
            __fds[7].events = 0x41;
            __fds[7].fd = local_b8[7];
            if (nVar9 != 8) {
              __fds[8].events = 0x41;
              __fds[8].fd = local_b8[8];
              if (nVar9 == 10) {
                __fds[9].events = 0x41;
                __fds[9].fd = local_b8[9];
              }
              else {
                nVar9 = 9;
              }
            }
          }
        }
      }
    }
  }
LAB_004295fc:
  local_48 = &__fds->revents;
  local_4c = "nd bgm path";
  local_3c = 0x1220000;
  local_38 = 0x1220000;
  local_34 = 0x1220000;
  local_30 = 0x1220000;
  local_44 = 0x1220000;
  local_40 = 0x66666667;
  do {
    do {
      iVar1 = poll(__fds,local_50,-1);
    } while (iVar1 < 1);
    iVar1 = 0;
    piVar10 = local_b8;
    __mutex = (pthread_mutex_t *)(local_44 + -0x304c);
    psVar11 = local_48;
    do {
      if (*psVar11 != 0) {
        sVar2 = read(*piVar10,&local_6c,0x10);
        if (DAT_0121cf40 == '\0') {
          local_58 = sVar2;
          if (sVar2 != 0x10) {
            piVar3 = __errno_location();
            local_54 = *piVar3;
            pcVar4 = strerror(local_54);
            printf(local_4c + 0x16f8,local_58,local_54,pcVar4);
            FUN_0042c930(10);
          }
          if (local_60 < 2) {
            if (local_60 == 0) {
              if (local_64._2_2_ == 0) goto joined_r0x00429688;
              if ((local_64 & 0xffff) != 1) goto joined_r0x00429710;
              uVar6 = *(undefined4 *)(local_38 + -0x30c8);
              *(undefined4 *)(local_34 + -0x30cc) = 0;
              *(undefined4 *)(local_38 + -0x30c8) = 0;
              local_64 = CONCAT22((short)uVar6,(undefined2)local_64);
              *(undefined4 *)(local_3c + -0x30c4) = 0;
              if (iVar1 == 0) goto LAB_004298b8;
LAB_00429790:
              if ((iVar1 == 1) || (iVar1 == 2)) {
                local_70 = iVar1;
              }
            }
            else {
              if ((local_64 & 0xffff) != 1) goto joined_r0x00429710;
              uVar6 = custom_tick_get();
              *(undefined4 *)(local_3c + -0x30c4) = uVar6;
              *(undefined4 *)(local_34 + -0x30cc) = 1;
              *(uint *)(local_38 + -0x30c8) = local_64 >> 0x10;
LAB_00429788:
              if (iVar1 != 0) goto LAB_00429790;
LAB_004298b8:
              local_70 = 0;
            }
            if (DAT_0121cfb0 != 5) {
              pthread_mutex_lock(__mutex);
              iVar7 = DAT_0121cfac + 1;
              DAT_0121cfb0 = DAT_0121cfb0 + 1;
              iVar5 = DAT_0121cfac * 0x14;
              DAT_0121cfac = iVar7 + ((int)((longlong)iVar7 * (longlong)local_40 >> 0x21) -
                                     (iVar7 >> 0x1f)) * -5;
              *(int *)(&DAT_0121cf44 + iVar5) = local_70;
              *(undefined4 *)(&DAT_0121cf4c + iVar5) = local_68;
              *(uint *)(&DAT_0121cf50 + iVar5) = local_64;
              *(undefined4 *)(&DAT_0121cf48 + iVar5) = local_6c;
              *(uint *)(&DAT_0121cf54 + iVar5) = local_60;
              pthread_mutex_unlock(__mutex);
            }
          }
          else {
joined_r0x00429710:
            if ((local_64 & 0xffff) == 4) {
              iVar5 = custom_tick_get();
              iVar7 = *(int *)(local_3c + -0x30c4);
              if ((iVar5 != iVar7) && (*(int *)(local_38 + -0x30c8) != 0)) {
                uVar8 = 0xfa;
                if (*(int *)(local_34 + -0x30cc) == 0) {
                  uVar8 = 100;
                }
                *(uint *)(local_30 + -0x30d0) = uVar8;
                if (uVar8 < (uint)(iVar5 - iVar7)) {
                  uVar6 = custom_tick_get();
                  local_60 = 1;
                  local_64 = CONCAT22((short)*(undefined4 *)(local_38 + -0x30c8),
                                      (undefined2)local_64);
                  *(undefined4 *)(local_3c + -0x30c4) = uVar6;
                  *(undefined4 *)(local_34 + -0x30cc) = 0;
                  goto LAB_00429788;
                }
              }
            }
          }
        }
        else {
          FUN_0042c930(10);
        }
      }
joined_r0x00429688:
      iVar1 = iVar1 + 1;
      piVar10 = piVar10 + 1;
      psVar11 = psVar11 + 4;
    } while (iVar1 < (int)nVar9);
    FUN_0042c930(5);
  } while( true );
}



/* ===== FUNCTION FUN_00429cd8 @ 00429cd8 ===== */

undefined4 FUN_00429cd8(uint param_1)

{
  undefined4 uVar1;
  
  if ((int)(param_1 << 0xf) < 0) {
    param_1 = param_1 & 0xffff;
    if (param_1 == 0x179) {
      return 0x17;
    }
    if (param_1 < 0x17a) {
      if (param_1 == 0x81) {
        return 0x3a;
      }
      if (param_1 < 0x82) {
        if (param_1 == 0x71) {
          return 0x15;
        }
        if (param_1 < 0x72) {
          if (param_1 == 0x36) {
            return 0x28;
          }
          if (param_1 == 0x66) {
            return 0x49;
          }
          if (param_1 == 0x2a) {
            return 0x29;
          }
        }
        else {
          if (param_1 == 0x77) {
            return 0x26;
          }
          if (param_1 == 0x80) {
            return 0x27;
          }
          if (param_1 == 0x74) {
            return 0x18;
          }
        }
      }
      else {
        if (param_1 == 0xcf) {
          return 0x25;
        }
        if (param_1 < 0xd0) {
          if (param_1 == 0xa7) {
            return 0x2e;
          }
          if (param_1 == 0xae) {
            return 0x12;
          }
          if (param_1 == 0x8b) {
            return 0xf;
          }
        }
        else {
          if (param_1 == 0x16d) {
            return 0x3f;
          }
          if (param_1 < 0x16e) {
            if (param_1 == 0x16c) {
              return 0x39;
            }
          }
          else {
            if (param_1 == 0x172) {
              return 0x37;
            }
            if (param_1 == 0x174) {
              return 0x45;
            }
          }
        }
      }
    }
    else {
      if (param_1 == 0x201) {
        return 1;
      }
      if (param_1 < 0x202) {
        if (param_1 == 0x18e) {
          return 0x30;
        }
        if (param_1 < 399) {
          if (param_1 == 0x188) {
            return 0x19;
          }
          if (param_1 == 0x18b) {
            return 0x40;
          }
          if (param_1 == 0x184) {
            return 0x36;
          }
        }
        else {
          if (param_1 == 400) {
            return 0x32;
          }
          if (param_1 < 400) {
            return 0x31;
          }
          if (param_1 == 0x191) {
            return 0x33;
          }
          if (param_1 == 0x200) {
            return 0;
          }
        }
      }
      else {
        if (param_1 == 0x205) {
          return 5;
        }
        if (param_1 < 0x206) {
          if (param_1 != 0x203) {
            uVar1 = 2;
            if (0x203 < param_1) {
              uVar1 = 4;
            }
            return uVar1;
          }
          return 3;
        }
        if (param_1 == 0x207) {
          return 7;
        }
        if (param_1 < 0x207) {
          return 6;
        }
        if (param_1 == 0x208) {
          return 8;
        }
        if (param_1 == 0x209) {
          return 9;
        }
      }
    }
  }
  return 0xffff;
}



/* ===== FUNCTION FUN_0042a0e8 @ 0042a0e8 ===== */

void FUN_0042a0e8(void)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121cfb4);
  DAT_0121cfa8 = 0;
  DAT_0121cfac = 0;
  DAT_0121cfb0 = 0;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfb4);
  return;
}



/* ===== FUNCTION FUN_0042a1d8 @ 0042a1d8 ===== */

void FUN_0042a1d8(void)

{
  int iVar1;
  pthread_attr_t pStack_30;
  pthread_t local_c [2];
  
  local_c[0] = 0;
  DAT_0121cfb0 = 0;
  DAT_0121cfac = 0;
  DAT_0121cfa8 = 0;
  pthread_attr_init(&pStack_30);
  pthread_attr_setstacksize(&pStack_30,0x2000);
  pthread_attr_setdetachstate(&pStack_30,1);
  iVar1 = pthread_create(local_c,&pStack_30,FUN_00429460,(void *)0x0);
  if (iVar1 != 0) {
    return;
  }
  pthread_attr_destroy(&pStack_30);
  FUN_00429450();
  return;
}



/* ===== FUNCTION FUN_0042a25c @ 0042a25c ===== */

undefined * FUN_0042a25c(void)

{
  int iVar1;
  
  if (DAT_0121cfb0 == 0) {
    return (undefined *)0x0;
  }
  DAT_0121cfb0 = DAT_0121cfb0 + -1;
  iVar1 = DAT_0121cfa8 * 0x14;
  DAT_0121cfa8 = (DAT_0121cfa8 + 1) % 5;
  return &DAT_0121cf44 + iVar1;
}



/* ===== FUNCTION FUN_0042a2c8 @ 0042a2c8 ===== */

void FUN_0042a2c8(undefined2 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 local_1c;
  
  local_1c = CONCAT22(param_1,1);
  if (DAT_0121cfb0 != 5) {
    pthread_mutex_lock((pthread_mutex_t *)&DAT_0121cfb4);
    DAT_0121cfb0 = DAT_0121cfb0 + 1;
    iVar1 = DAT_0121cfac * 0x14;
    DAT_0121cfac = (DAT_0121cfac + 1) % 5;
    *(undefined4 *)(&DAT_0121cf44 + iVar1) = 0;
    *(undefined4 *)(&DAT_0121cf4c + iVar1) = 0;
    *(undefined4 *)(&DAT_0121cf50 + iVar1) = local_1c;
    *(undefined4 *)(&DAT_0121cf48 + iVar1) = 0;
    *(undefined4 *)(&DAT_0121cf54 + iVar1) = param_2;
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfb4);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0042a440 @ 0042a440 ===== */

int FUN_0042a440(char *param_1)

{
  FILE *__stream;
  key_t __key;
  int iVar1;
  char *__s;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121cfd0);
  __stream = fopen(param_1,"w");
  if (__stream == (FILE *)0x0) {
    __s = "Error creating file";
  }
  else {
    fclose(__stream);
    __key = ftok(param_1,0x6b);
    if (__key != -1) {
      iVar1 = msgget(__key,0x3b6);
      if (iVar1 != -1) {
        pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfd0);
        printf("create msg id: %d\n",iVar1);
        return iVar1;
      }
      perror("msgget failed");
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfd0);
      return -1;
    }
    __s = "ftok";
  }
  perror(__s);
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfd0);
                    /* WARNING: Subroutine does not return */
  exit(1);
}



/* ===== FUNCTION FUN_0042a538 @ 0042a538 ===== */

int FUN_0042a538(void)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121cfd0);
  iVar1 = msgget(0,0x3b6);
  if (iVar1 < 1) {
    perror("msgget failed");
    iVar1 = msgget(0,0x3b6);
    if (iVar1 < 1) {
      perror("msgget failed again");
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfd0);
      return -1;
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121cfd0);
  printf("create msg id: %d\n",iVar1);
  return iVar1;
}



/* ===== FUNCTION FUN_0042a5d8 @ 0042a5d8 ===== */

undefined4 FUN_0042a5d8(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = msgctl(param_1,0,(msqid_ds *)0x0);
  if (iVar1 == -1) {
    puts("msg delete failed");
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042a628 @ 0042a628 ===== */

int FUN_0042a628(int param_1,void *param_2,size_t param_3)

{
  undefined4 local_a0;
  undefined1 auStack_9c [132];
  int local_18;
  
  if ((int)param_3 < 0x7f) {
    local_a0 = 1;
    memcpy(auStack_9c,param_2,param_3);
    local_18 = msgsnd(param_1,&local_a0,param_3,0);
    if (local_18 == -1) {
      perror("msgsnd failed");
    }
    else {
      local_18 = 0;
    }
  }
  else {
    puts("Message size too large!");
    local_18 = -1;
  }
  return local_18;
}



/* ===== FUNCTION FUN_0042a720 @ 0042a720 ===== */

undefined4 FUN_0042a720(int param_1,void *param_2,size_t param_3,int param_4)

{
  ssize_t sVar1;
  int iVar2;
  undefined1 auStack_a8 [4];
  undefined1 auStack_a4 [132];
  
  if (param_4 < 1) {
    return 0xffffffff;
  }
  iVar2 = 0;
  do {
    sVar1 = msgrcv(param_1,auStack_a8,param_3,0,0x800);
    if (sVar1 != -1) {
      memcpy(param_2,auStack_a4,param_3);
      return 0;
    }
    iVar2 = iVar2 + 1;
    usleep(1000);
  } while (param_4 != iVar2);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0042a808 @ 0042a808 ===== */

msgqnum_t FUN_0042a808(int param_1)

{
  int iVar1;
  msqid_ds mStack_60;
  
  iVar1 = msgctl(param_1,2,&mStack_60);
  if (iVar1 == -1) {
    printf("%s() error!\n","api_message_get_count");
    mStack_60.msg_qnum = 0xffffffff;
  }
  return mStack_60.msg_qnum;
}



/* ===== FUNCTION FUN_0042a8b4 @ 0042a8b4 ===== */

void FUN_0042a8b4(undefined4 param_1,undefined4 param_2)

{
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0;
  switch(param_1) {
  case 0:
    local_18 = 0xe;
    break;
  case 1:
    local_18 = 0xf;
    break;
  case 2:
    local_18 = 0x10;
    break;
  case 3:
    local_18 = 0x11;
    break;
  case 7:
    local_18 = 0x12;
    break;
  case 8:
    local_18 = 0x13;
    break;
  case 9:
    local_18 = 0x14;
    break;
  case 10:
    local_18 = 0x15;
  }
  local_14 = param_2;
  if ((DAT_012146d4 != -1) || (DAT_012146d4 = FUN_0042a538(100,8), DAT_012146d4 != -1)) {
    FUN_0042a628(DAT_012146d4,&local_18,8);
  }
  return;
}



/* ===== FUNCTION FUN_0042b2e4 @ 0042b2e4 ===== */

undefined4 FUN_0042b2e4(void)

{
  return DAT_012146d0;
}



/* ===== FUNCTION FUN_0042b2f0 @ 0042b2f0 ===== */

undefined * FUN_0042b2f0(void)

{
  char *__s;
  size_t __n;
  undefined *puVar1;
  
  __s = (char *)glist_nth_data(*(undefined4 *)(DAT_0121d1ac + 4),0);
  puVar1 = (undefined *)0x0;
  if (__s != (char *)0x0) {
    __n = strlen(__s);
    puVar1 = &DAT_0121d0fc;
    memcpy(&DAT_0121d0fc,__s,__n);
  }
  return puVar1;
}



/* ===== FUNCTION FUN_0042b34c @ 0042b34c ===== */

undefined4 FUN_0042b34c(uint param_1,char *param_2)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  char *__s2;
  undefined4 *puVar5;
  
  piVar3 = DAT_0121d1ac;
  if ((param_1 == 0xe) || (param_1 == 0x12)) {
    puVar5 = (undefined4 *)DAT_0121d1ac[1];
    for (puVar4 = puVar5; puVar4 != (undefined4 *)0x0; puVar4 = (undefined4 *)puVar4[1]) {
      if ((char *)*puVar4 == (char *)0x0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar2 = strcmp(param_2,(char *)*puVar4);
      if (iVar2 == 0) {
        return 0;
      }
    }
    *piVar3 = *piVar3 + 1;
    iVar2 = glist_append(puVar5,param_2);
    piVar1 = DAT_0121d1ac;
    piVar3[1] = iVar2;
    piVar3 = piVar1;
  }
  else if ((param_1 < 0x16) && ((int)((0x2a8000U >> (param_1 & 0x1f)) << 0x1f) < 0)) {
    for (puVar4 = (undefined4 *)DAT_0121d1ac[1]; puVar4 != (undefined4 *)0x0;
        puVar4 = (undefined4 *)puVar4[1]) {
      __s2 = (char *)*puVar4;
      if (__s2 == (char *)0x0) {
        do {
                    /* WARNING: Do nothing block with infinite loop */
        } while( true );
      }
      iVar2 = strcmp(param_2,__s2);
      if (iVar2 == 0) {
        free(__s2);
        *puVar4 = 0;
        iVar2 = glist_delete_link(piVar3[1],puVar4);
        piVar3[1] = iVar2;
        piVar3 = DAT_0121d1ac;
        iVar2 = *DAT_0121d1ac + -1;
        if (iVar2 < 0) {
          iVar2 = 0;
        }
        *DAT_0121d1ac = iVar2;
        break;
      }
    }
  }
  piVar3[3] = param_1;
  return 0;
}



/* ===== FUNCTION FUN_0042b4b4 @ 0042b4b4 ===== */

undefined4 FUN_0042b4b4(void)

{
  return DAT_0121d1ac;
}



/* ===== FUNCTION FUN_0042b4c0 @ 0042b4c0 ===== */

void FUN_0042b4c0(void)

{
  int iVar1;
  ssize_t sVar2;
  int iVar3;
  int iVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  char local_40 [16];
  uint local_30 [2];
  int local_28;
  
  local_40[0] = '\0';
  local_40[1] = '\0';
  local_40[2] = '\0';
  local_40[3] = '\0';
  local_40[4] = '\0';
  local_40[5] = '\0';
  local_40[6] = '\0';
  local_40[7] = '\0';
  local_40[8] = '\0';
  local_40[9] = '\0';
  local_40[10] = '\0';
  local_40[0xb] = '\0';
  local_40[0xc] = '\0';
  local_40[0xd] = '\0';
  local_40[0xe] = '\0';
  local_40[0xf] = '\0';
  iVar1 = open("/proc/device-tree/hcrtos/rotate/status",0);
  if (-1 < iVar1) {
    read(iVar1,local_40,0x10);
    close(iVar1);
  }
  iVar1 = strcmp(local_40,"okay");
  uVar5 = 0;
  if (iVar1 != 0) {
    uVar6 = 0;
    uVar7 = 0;
    iVar4 = 0;
    iVar3 = 0;
    iVar1 = 0;
    goto LAB_0042b550;
  }
  iVar1 = open("/proc/device-tree/hcrtos/rotate/rotate",0);
  if (iVar1 < 0) {
    uVar7 = 0xffff;
    iVar3 = -1;
LAB_0042b5f0:
    printf("fd:%d ,dts value: %x\n",iVar1,iVar3);
  }
  else {
    sVar2 = read(iVar1,local_30,4);
    if (sVar2 == 4) {
      close(iVar1);
      iVar3 = (((local_30[0] >> 0x10) << 0x18 | (local_30[0] >> 0x18) << 0x10) >> 0x10) +
              ((local_30[0] & 0xff) << 8 | local_30[0] >> 8 & 0xff) * 0x10000;
      uVar7 = (undefined2)iVar3;
      goto LAB_0042b5f0;
    }
    uVar7 = 0xffff;
    close(iVar1);
    iVar3 = -1;
  }
  iVar1 = open("/proc/device-tree/hcrtos/rotate/h_flip",0);
  if (iVar1 < 0) {
    uVar6 = 0xffff;
    iVar4 = -1;
LAB_0042b650:
    printf("fd:%d ,dts value: %x\n",iVar1,iVar4);
  }
  else {
    sVar2 = read(iVar1,local_30,4);
    if (sVar2 == 4) {
      close(iVar1);
      iVar4 = (((local_30[0] >> 0x10) << 0x18 | (local_30[0] >> 0x18) << 0x10) >> 0x10) +
              ((local_30[0] & 0xff) << 8 | local_30[0] >> 8 & 0xff) * 0x10000;
      uVar6 = (undefined2)iVar4;
      goto LAB_0042b650;
    }
    uVar6 = 0xffff;
    close(iVar1);
    iVar4 = -1;
  }
  iVar1 = open("/proc/device-tree/hcrtos/rotate/v_flip",0);
  if (iVar1 < 0) {
    uVar5 = 0xffff;
    local_28 = -1;
  }
  else {
    sVar2 = read(iVar1,local_30,4);
    if (sVar2 != 4) {
      uVar5 = 0xffff;
      close(iVar1);
      iVar1 = -1;
      goto LAB_0042b550;
    }
    close(iVar1);
    local_28 = (((local_30[0] >> 0x10) << 0x18 | (local_30[0] >> 0x18) << 0x10) >> 0x10) +
               ((local_30[0] & 0xff) << 8 | local_30[0] >> 8 & 0xff) * 0x10000;
    uVar5 = (undefined2)local_28;
  }
  printf("fd:%d ,dts value: %x\n",iVar1,local_28);
  iVar1 = local_28;
LAB_0042b550:
  DAT_0121d0d4 = uVar5;
  DAT_0121d0d6 = uVar6;
  DAT_0121d0d8 = uVar7;
  printf("->>> init_rotate = %u h_flip %u v_flip = %u\n",iVar3,iVar4,iVar1);
  return;
}



/* ===== FUNCTION FUN_0042b734 @ 0042b734 ===== */

undefined2 FUN_0042b734(void)

{
  uint uVar1;
  int iVar2;
  key_t __key;
  stat sStack_a0;
  
  if (DAT_0121cff4 == (uint *)0x0) {
    iVar2 = __xstat(3,"/tmp/cubegame_status",&sStack_a0);
    if (iVar2 != 0) {
      return DAT_0121d0d8;
    }
    if ((sStack_a0.st_nlink & 0xf000) != 0x4000) {
      return DAT_0121d0d8;
    }
    printf("mkdir \"%s\" success\n","/tmp/cubegame_status");
    __key = ftok("/tmp/cubegame_status",0x61);
    if (__key == -1) {
      perror("ftok");
      return DAT_0121d0d8;
    }
    iVar2 = shmget(__key,4,0x3b6);
    if (iVar2 == -1) {
      perror("shmget");
      return DAT_0121d0d8;
    }
    DAT_0121cff4 = shmat(iVar2,(void *)0x0,0);
    if (DAT_0121cff4 == (uint *)0x0) {
      perror("shmat");
      return DAT_0121d0d8;
    }
    printf("%s %d %d\n","cube_shmget_hdmi_tx_in_init",0xdfa,*DAT_0121cff4);
    if (DAT_0121cff4 == (uint *)0x0) {
      return DAT_0121d0d8;
    }
    uVar1 = *DAT_0121cff4;
  }
  else {
    uVar1 = *DAT_0121cff4;
  }
  if ((uVar1 & 1) == 0) {
    return DAT_0121d0d8;
  }
  return 0;
}



/* ===== FUNCTION FUN_0042b888 @ 0042b888 ===== */

undefined2 FUN_0042b888(void)

{
  return DAT_0121d0d6;
}



/* ===== FUNCTION FUN_0042b894 @ 0042b894 ===== */

undefined2 FUN_0042b894(void)

{
  return DAT_0121d0d4;
}



/* ===== FUNCTION FUN_0042b8a0 @ 0042b8a0 ===== */

void FUN_0042b8a0(void)

{
  if (DAT_0121d1ac != (void *)0x0) {
    return;
  }
  DAT_0121d1ac = calloc(0x10,1);
  return;
}



/* ===== FUNCTION FUN_0042b8e8 @ 0042b8e8 ===== */

undefined4 FUN_0042b8e8(void)

{
  int iVar1;
  int iVar2;
  undefined1 local_d0 [14];
  undefined4 local_c2;
  undefined *local_be;
  pthread_attr_t pStack_60;
  epoll_event local_38;
  undefined4 local_28;
  undefined4 local_24;
  pthread_t apStack_20 [2];
  
  system("/etc/wifiprobe.sh &");
  local_28 = 0;
  local_24 = 0;
  pthread_attr_init(&pStack_60);
  pthread_attr_setstacksize(&pStack_60,0x2000);
  pthread_attr_setdetachstate(&pStack_60,1);
  iVar1 = pthread_create(apStack_20,&pStack_60,(__start_routine *)&LAB_0042a9a0,(void *)0x0);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_60);
    DAT_0121d0f8 = socket(1,2,0);
    if (DAT_0121d0f8 < 0) {
      puts("socket error");
    }
    else {
      puts("socket success");
      unlink("/tmp/hotplug.socket");
      memset(local_d0,0,0x6e);
      local_d0._0_2_ = 1;
      local_d0[2] = '/';
      local_d0[3] = 't';
      local_d0[4] = 'm';
      local_d0[5] = 'p';
      local_d0[6] = '/';
      local_d0[7] = 'h';
      local_d0[8] = 'o';
      local_d0[9] = 't';
      local_d0[10] = 'p';
      local_d0[0xb] = 'l';
      local_d0[0xc] = 'u';
      local_d0[0xd] = 'g';
      local_c2 = 0x636f732e;
      local_be = &DAT_0074656b;
      iVar1 = bind(DAT_0121d0f8,(sockaddr *)local_d0,0x6e);
      if (iVar1 < 0) {
        puts("bind error");
      }
      else {
        puts("bind success");
        iVar1 = listen(DAT_0121d0f8,1);
        if (iVar1 < 0) {
          puts("listen error");
        }
        else {
          puts("listen success");
          DAT_0121d0ec = epoll_create1(0);
          local_38.events = 1;
          DAT_0121d0e4 = DAT_0121d0f8;
          DAT_0121d0e8 = 1;
          local_38.data.ptr = &DAT_0121d0e4;
          iVar1 = epoll_ctl(DAT_0121d0ec,1,DAT_0121d0f8,&local_38);
          if (iVar1 != 0) {
            puts("EPOLL_CTL_ADD hotplug fail");
            iVar1 = open("/dev/sndC0i2so",1);
            goto joined_r0x0042bc24;
          }
          puts("EPOLL_CTL_ADD hotplug success");
          iVar1 = open("/dev/hdmi",2);
          DAT_0121d0f0 = iVar1;
          if (iVar1 < 0) {
            printf("%s(), line:%d. open device: %s error!\n","hotplug_init",0x3a9,"/dev/hdmi");
          }
          else {
            DAT_0121d0dc = ioctl(iVar1,0x20000c00,0x80000);
            local_38.events = 1;
            DAT_0121d0e0 = 0;
            local_38.data.ptr = &DAT_0121d0dc;
            DAT_0121d0f4 = DAT_0121d0dc;
            iVar2 = epoll_ctl(DAT_0121d0ec,1,DAT_0121d0dc,&local_38);
            if (iVar2 == 0) {
              local_24 = 0;
              local_28 = 0x20001c08;
              iVar2 = ioctl(iVar1,0x80080c01,&local_28);
              if (iVar2 == 0) {
                local_24 = 0;
                local_28 = 0x20001c09;
                iVar2 = ioctl(iVar1,0x80080c01,&local_28);
                if (iVar2 == 0) {
                  local_24 = 0;
                  local_28 = 0x40601c0a;
                  iVar1 = ioctl(iVar1,0x80080c01,&local_28);
                  if (iVar1 == 0) goto LAB_0042bb70;
                }
              }
              printf("KUMSGQ_NOTIFIER_SETUP 0x%08x fail\n",local_28);
            }
            else {
              puts("EPOLL_CTL_ADD fail");
            }
          }
        }
      }
    }
  }
  else {
    puts("pthread_create receive_event_func fail");
  }
LAB_0042bb70:
  iVar1 = open("/dev/sndC0i2so",1);
joined_r0x0042bc24:
  if (iVar1 < 0) {
    printf("open snd_fd %d failed\n",iVar1);
  }
  else {
    ioctl(iVar1,0x2000080f,0);
    close(iVar1);
  }
  signal(0xf,FUN_0042e4b4);
  signal(2,FUN_0042e4b4);
  signal(0xb,FUN_0042e4b4);
  signal(10,FUN_0042e4b4);
  signal(0xd,(__sighandler_t)&LAB_0042a860);
  return 0;
}



/* ===== FUNCTION FUN_0042bcc8 @ 0042bcc8 ===== */

undefined4 FUN_0042bcc8(uint *param_1,uint *param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  key_t __key;
  short sVar4;
  uint uVar5;
  stat sStack_b8;
  uint local_28;
  uint local_24;
  uint local_20 [3];
  
  local_28 = 0;
  local_24 = 0;
  local_20[0] = 0;
  if (DAT_0121cff4 == (uint *)0x0) {
    iVar3 = __xstat(3,"/tmp/cubegame_status",&sStack_b8);
    if ((iVar3 == 0) && ((sStack_b8.st_nlink & 0xf000) == 0x4000)) {
      printf("mkdir \"%s\" success\n","/tmp/cubegame_status");
      __key = ftok("/tmp/cubegame_status",0x61);
      if (__key == -1) {
        perror("ftok");
      }
      else {
        iVar3 = shmget(__key,4,0x3b6);
        if (iVar3 == -1) {
          perror("shmget");
        }
        else {
          DAT_0121cff4 = shmat(iVar3,(void *)0x0,0);
          if (DAT_0121cff4 == (uint *)0x0) {
            perror("shmat");
          }
          else {
            printf("%s %d %d\n","cube_shmget_hdmi_tx_in_init",0xdfa,*DAT_0121cff4);
            if (DAT_0121cff4 != (uint *)0x0) {
              uVar1 = *DAT_0121cff4;
              goto LAB_0042bd04;
            }
          }
        }
      }
    }
LAB_0042bd14:
    sVar4 = DAT_0121d0d8;
    uVar5 = (uint)DAT_0121d0d6;
    uVar2 = FUN_00428454(0x14);
    FUN_00433cd0(uVar2,local_20,&local_24,&local_28);
    uVar1 = local_24;
    if (((sVar4 == 0) || (sVar4 == 0xb4)) || (uVar1 = local_28, local_28 = local_24, sVar4 != 0x10e)
       ) goto LAB_0042bdcc;
    local_20[0] = local_20[0] + 1;
LAB_0042bd64:
    local_20[0] = local_20[0] & 3;
  }
  else {
    uVar1 = *DAT_0121cff4;
LAB_0042bd04:
    sVar4 = 0;
    if ((uVar1 & 1) == 0) goto LAB_0042bd14;
    uVar5 = (uint)DAT_0121d0d6;
    uVar2 = FUN_00428454(0x14);
    FUN_00433cd0(uVar2,local_20,&local_24,&local_28);
    uVar1 = local_24;
LAB_0042bdcc:
    if (sVar4 == 0x5a) {
      local_20[0] = local_20[0] + 3;
      goto LAB_0042bd64;
    }
    if (sVar4 == 0xb4) {
      local_20[0] = local_20[0] + 2 & 3;
    }
  }
  if (local_28 == 1) {
    uVar1 = 1;
    if (local_20[0] == 1) {
      local_20[0] = 3;
    }
    else {
      if ((int)local_20[0] < 2) {
        if (local_20[0] == 0) {
          uVar1 = 1;
          local_20[0] = 2;
          goto LAB_0042bd70;
        }
      }
      else {
        uVar1 = 1;
        if (local_20[0] == 2) {
          local_20[0] = 0;
          goto LAB_0042bd70;
        }
        if (local_20[0] == 3) {
          uVar1 = 1;
          local_20[0] = 1;
          goto LAB_0042bd70;
        }
      }
      uVar1 = 1;
    }
  }
LAB_0042bd70:
  *param_1 = local_20[0];
  *param_2 = uVar1 ^ uVar5;
  return 0;
}



/* ===== FUNCTION FUN_0042c048 @ 0042c048 ===== */

undefined4 FUN_0042c048(void)

{
  if (DAT_0121d0d0 != 0) {
    FUN_0042f6c0(0,0);
  }
  DAT_0121d0d0 = 0;
  return 0;
}



/* ===== FUNCTION FUN_0042c088 @ 0042c088 ===== */

undefined4 FUN_0042c088(void)

{
  int iVar1;
  undefined4 local_18;
  undefined4 local_14 [2];
  
  if ((DAT_0121d0d0 != 0) && (DAT_0121d050 != '\0')) {
    FUN_0042f6c0(0,0);
    DAT_0121d0d0 = 0;
    local_18 = 0;
    local_14[0] = 0;
    FUN_0042bcc8(local_14,&local_18);
    iVar1 = FUN_0042f7fc(&DAT_0121d050,local_14[0],local_18);
    if (-1 < iVar1) {
      DAT_0121d0d0 = 1;
      printf("Show logo: %s ok!\n",&DAT_0121d050);
      return 0;
    }
    puts("com_logo_show() fail!");
  }
  return 0;
}



/* ===== FUNCTION FUN_0042c144 @ 0042c144 ===== */

undefined4 FUN_0042c144(char *param_1)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  key_t __key;
  uint uVar4;
  uint uVar5;
  short sVar6;
  uint uVar7;
  stat sStack_b8;
  uint local_28;
  uint local_24;
  uint local_20 [3];
  
  if (DAT_0121d0d0 != 0) {
    FUN_0042f6c0(1,0);
  }
  if (param_1 == (char *)0x0) {
    param_1 = "/hcdemo_files/HCCAST_1920x1080.264";
  }
  DAT_0121d0d0 = 0;
  strcpy(&DAT_0121d050,param_1);
  local_28 = 0;
  local_24 = 0;
  local_20[0] = 0;
  if (DAT_0121cff4 == (uint *)0x0) {
    iVar3 = __xstat(3,"/tmp/cubegame_status",&sStack_b8);
    if ((iVar3 == 0) && ((sStack_b8.st_nlink & 0xf000) == 0x4000)) {
      printf("mkdir \"%s\" success\n","/tmp/cubegame_status");
      __key = ftok("/tmp/cubegame_status",0x61);
      if (__key == -1) {
        perror("ftok");
      }
      else {
        iVar3 = shmget(__key,4,0x3b6);
        if (iVar3 == -1) {
          perror("shmget");
        }
        else {
          DAT_0121cff4 = shmat(iVar3,(void *)0x0,0);
          if (DAT_0121cff4 == (uint *)0x0) {
            perror("shmat");
          }
          else {
            printf("%s %d %d\n","cube_shmget_hdmi_tx_in_init",0xdfa,*DAT_0121cff4);
            if (DAT_0121cff4 != (uint *)0x0) {
              uVar1 = *DAT_0121cff4;
              goto LAB_0042c1a0;
            }
          }
        }
      }
    }
LAB_0042c1b0:
    sVar6 = DAT_0121d0d8;
    uVar7 = (uint)DAT_0121d0d6;
    uVar2 = FUN_00428454(0x14);
    FUN_00433cd0(uVar2,local_20,&local_24,&local_28);
    uVar1 = local_28;
    uVar5 = local_24;
    if (((sVar6 == 0) || (sVar6 == 0xb4)) || (uVar1 = local_24, uVar5 = local_28, sVar6 != 0x10e))
    goto LAB_0042c284;
    uVar4 = local_20[0] + 1;
LAB_0042c200:
    uVar4 = uVar4 & 3;
  }
  else {
    uVar1 = *DAT_0121cff4;
LAB_0042c1a0:
    if ((uVar1 & 1) == 0) goto LAB_0042c1b0;
    sVar6 = 0;
    uVar7 = (uint)DAT_0121d0d6;
    uVar2 = FUN_00428454(0x14);
    FUN_00433cd0(uVar2,local_20,&local_24,&local_28);
    uVar1 = local_28;
    uVar5 = local_24;
LAB_0042c284:
    if (sVar6 == 0x5a) {
      uVar4 = local_20[0] + 3;
      goto LAB_0042c200;
    }
    uVar4 = local_20[0];
    if (sVar6 == 0xb4) {
      uVar4 = local_20[0] + 2 & 3;
    }
  }
  if (uVar1 == 1) {
    if (uVar4 == 1) {
      uVar5 = 1;
      uVar4 = 3;
    }
    else {
      if ((int)uVar4 < 2) {
        if (uVar4 == 0) {
          uVar5 = 1;
          uVar4 = 2;
          goto LAB_0042c20c;
        }
      }
      else {
        if (uVar4 == 2) {
          uVar5 = 1;
          uVar4 = 0;
          goto LAB_0042c20c;
        }
        if (uVar4 == 3) {
          uVar5 = 1;
          uVar4 = 1;
          goto LAB_0042c20c;
        }
      }
      uVar5 = 1;
    }
  }
LAB_0042c20c:
  iVar3 = FUN_0042f7fc(param_1,uVar4,uVar5 ^ uVar7);
  if (iVar3 < 0) {
    puts("com_logo_show() fail!");
    uVar2 = 0xffffffff;
  }
  else {
    DAT_0121d0d0 = 1;
    printf("Show logo: %s ok!\n",param_1);
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042c430 @ 0042c430 ===== */

undefined4 FUN_0042c430(void)

{
  if (DAT_0121d0d0 != 0) {
    FUN_0042f6c0(1,0);
  }
  DAT_0121d0d0 = 0;
  return 0;
}



/* ===== FUNCTION FUN_0042c470 @ 0042c470 ===== */

undefined4 FUN_0042c470(void)

{
  FUN_0042f6c0();
  return 0;
}



/* ===== FUNCTION FUN_0042c490 @ 0042c490 ===== */

undefined4 FUN_0042c490(void)

{
  FUN_0042f5d4();
  return 0;
}



/* ===== FUNCTION FUN_0042c4b0 @ 0042c4b0 ===== */

undefined4 FUN_0042c4b0(void)

{
  int __fd;
  undefined4 uVar1;
  undefined4 local_10 [2];
  
  local_10[0] = 0;
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    printf("%s(), line:%d, open dis error!\n","api_dis_suspend",0x50c);
    uVar1 = 0xffffffff;
  }
  else {
    local_10[0] = 1;
    ioctl(__fd,0x80040e1b,local_10);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042c530 @ 0042c530 ===== */

undefined4 FUN_0042c530(undefined1 param_1)

{
  int __fd;
  undefined4 uVar1;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    local_18 = CONCAT31(local_18._1_3_,param_1);
    local_20 = 1;
    local_1c = 1;
    ioctl(__fd,0x800c0e04,&local_20);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042c5b4 @ 0042c5b4 ===== */

void FUN_0042c5b4(undefined4 param_1)

{
  if (DAT_012146d4 == -1) {
    DAT_012146d4 = FUN_0042a538(100,8);
    if (DAT_012146d4 == -1) {
      return;
    }
  }
  FUN_0042a628(DAT_012146d4,param_1,8);
  return;
}



/* ===== FUNCTION FUN_0042c630 @ 0042c630 ===== */

undefined4 FUN_0042c630(undefined4 param_1)

{
  undefined4 uVar1;
  
  if (DAT_012146d4 != -1) {
    uVar1 = FUN_0042a720(DAT_012146d4,param_1,8,5);
    return uVar1;
  }
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0042c660 @ 0042c660 ===== */

void FUN_0042c660(undefined4 param_1)

{
  if (DAT_012146c8 == -1) {
    DAT_012146c8 = FUN_0042a440("/tmp/cube_process_msg",100,8);
    if (DAT_012146c8 == -1) {
      return;
    }
  }
  FUN_0042a720(DAT_012146c8,param_1,8,5);
  return;
}



/* ===== FUNCTION FUN_0042c6e4 @ 0042c6e4 ===== */

void FUN_0042c6e4(void)

{
  int iVar1;
  undefined1 auStack_18 [12];
  
  do {
    if (DAT_012146d4 == -1) {
      return;
    }
    iVar1 = FUN_0042a720(DAT_012146d4,auStack_18,8,5);
  } while (iVar1 == 0);
  return;
}



/* ===== FUNCTION FUN_0042c748 @ 0042c748 ===== */

undefined4 FUN_0042c748(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0;
  local_14 = param_1;
  if ((DAT_012146d4 == -1) && (DAT_012146d4 = FUN_0042a538(100,8), DAT_012146d4 == -1)) {
    return 0xffffffff;
  }
  uVar1 = FUN_0042a628(DAT_012146d4,&local_18,8);
  return uVar1;
}



/* ===== FUNCTION FUN_0042c7cc @ 0042c7cc ===== */

bool FUN_0042c7cc(char *param_1)

{
  int iVar1;
  char acStack_60 [64];
  undefined4 local_20;
  int local_1c;
  uint local_18;
  uint local_14 [2];
  
  iVar1 = __isoc99_sscanf(param_1,"%d.%d.%d.%d",local_14,&local_18,&local_1c,&local_20);
  if (iVar1 == 4) {
    sprintf(acStack_60,"%d.%d.%d.%d",local_14[0],local_18,local_1c,local_20);
    iVar1 = strcmp(acStack_60,param_1);
    if (((iVar1 == 0) && (local_14[0] < 0x100)) && (local_18 < 0x100)) {
      return local_1c < 0x100;
    }
  }
  return false;
}



/* ===== FUNCTION FUN_0042c87c @ 0042c87c ===== */

void FUN_0042c87c(void)

{
  if (DAT_0121d1b0 == '\0') {
    hcplayer_init(1);
    DAT_0121d1b0 = '\x01';
  }
  return;
}



/* ===== FUNCTION FUN_0042c8b8 @ 0042c8b8 ===== */

void FUN_0042c8b8(void)

{
  hcplayer_deinit();
  DAT_0121d1b0 = 0;
  return;
}



/* ===== FUNCTION FUN_0042c8dc @ 0042c8dc ===== */

void FUN_0042c8dc(void)

{
  if (DAT_012146d4 != -1) {
    FUN_0042a5d8();
    DAT_012146d4 = -1;
  }
  hcplayer_deinit();
  DAT_0121d1b0 = 0;
  return;
}



/* ===== FUNCTION FUN_0042c930 @ 0042c930 ===== */

void FUN_0042c930(int param_1)

{
  usleep(param_1 * 1000);
  return;
}



/* ===== FUNCTION FUN_0042ca20 @ 0042ca20 ===== */

undefined4 FUN_0042ca20(uint param_1)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d18c);
  __fd = open("/dev/fb0",2);
  if (__fd == -1) {
    printf("%s(), line: %d. Error: cannot open framebuffer device","api_osd_show_onoff",0x61d);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
    uVar2 = 0xffffffff;
  }
  else {
    DAT_0121d1a4 = 0;
    iVar1 = ioctl(__fd,0x4611,param_1 ^ 1);
    if (iVar1 != 0) {
      printf("%s(), line: %d. Error: FBIOBLANK","api_osd_show_onoff",0x62a);
    }
    close(__fd);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042caf8 @ 0042caf8 ===== */

undefined4 FUN_0042caf8(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 10;
  local_14 = param_1;
  if ((DAT_012146d4 == -1) && (DAT_012146d4 = FUN_0042a538(100,8), DAT_012146d4 == -1)) {
    return 0xffffffff;
  }
  uVar1 = FUN_0042a628(DAT_012146d4,&local_18,8);
  return uVar1;
}



/* ===== FUNCTION FUN_0042cbfc @ 0042cbfc ===== */

void FUN_0042cbfc(char *param_1,void *param_2,size_t param_3)

{
  int __fd;
  
  __fd = open(param_1,0);
  if (-1 < __fd) {
    read(__fd,param_2,param_3);
    close(__fd);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0042cc6c @ 0042cc6c ===== */

void FUN_0042cc6c(void)

{
  int __fd;
  void *pvVar1;
  
  if ((DAT_0121d04c == (void *)0x0) && (__fd = open("/dev/mem",0x4012), -1 < __fd)) {
    printf("%s(), addr_align=0x%x, addr_offse=0x%x!\n","api_hw_watchdog_mmap_open",0x18818000,0x500)
    ;
    pvVar1 = mmap((void *)0x0,0x1000,3,1,__fd,0x18818000);
    printf("%s(), return map_addr=0x%x!\n","api_hw_watchdog_mmap_open",pvVar1);
    if ((int)pvVar1 - 1U < 0xfffffffe) {
      DAT_0121d044 = (int)pvVar1 + 0x500;
      DAT_0121d048 = __fd;
      DAT_0121d04c = pvVar1;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0042cd48 @ 0042cd48 ===== */

void FUN_0042cd48(void)

{
  if (DAT_0121d04c != (void *)0x0) {
    munmap(DAT_0121d04c,0x1000);
  }
  if (0 < DAT_0121d048) {
    close(DAT_0121d048);
  }
  DAT_0121d044 = 0;
  DAT_0121d048 = 0;
  DAT_0121d04c = (void *)0x0;
  return;
}



/* ===== FUNCTION FUN_0042cda8 @ 0042cda8 ===== */

void FUN_0042cda8(void)

{
  undefined4 *puVar1;
  
  printf("%s()!\n","api_hw_watchdog_reboot");
  puVar1 = DAT_0121d044;
  if (DAT_0121d04c != 0) {
    *DAT_0121d044 = 0xfffffffa;
    puVar1[1] = 0x26;
    return;
  }
  printf("%s(), No mmap!","api_hw_watchdog_reboot");
  return;
}



/* ===== FUNCTION FUN_0042ce18 @ 0042ce18 ===== */

void FUN_0042ce18(void)

{
  int __fd;
  void *pvVar1;
  
  printf("%s(): reboot now!!\n","api_system_reboot");
  if ((DAT_0121d04c == (void *)0x0) && (__fd = open("/dev/mem",0x4012), -1 < __fd)) {
    printf("%s(), addr_align=0x%x, addr_offse=0x%x!\n","api_hw_watchdog_mmap_open",0x18818000,0x500)
    ;
    pvVar1 = mmap((void *)0x0,0x1000,3,1,__fd,0x18818000);
    printf("%s(), return map_addr=0x%x!\n","api_hw_watchdog_mmap_open",pvVar1);
    if ((int)pvVar1 - 1U < 0xfffffffe) {
      DAT_0121d044 = (int)pvVar1 + 0x500;
      DAT_0121d048 = __fd;
      DAT_0121d04c = pvVar1;
    }
  }
  FUN_0042cda8();
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_0042cef8 @ 0042cef8 ===== */

void FUN_0042cef8(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  
  puVar1 = DAT_0121d040;
  if (DAT_0121d040 != (undefined4 *)0x0) {
    piVar3 = (int *)*DAT_0121d040;
    if (*param_1 == *piVar3) {
LAB_0042cfa0:
      piVar3[1] = param_1[1];
      return;
    }
    for (puVar2 = (undefined4 *)DAT_0121d040[1]; puVar2 != (undefined4 *)0x0;
        puVar2 = (undefined4 *)puVar2[1]) {
      piVar3 = (int *)*puVar2;
      if (*piVar3 == *param_1) goto LAB_0042cfa0;
    }
  }
  piVar3 = malloc(8);
  iVar4 = param_1[1];
  *piVar3 = *param_1;
  piVar3[1] = iVar4;
  DAT_0121d040 = (undefined4 *)glist_append(puVar1,piVar3);
  return;
}



/* ===== FUNCTION FUN_0042cfc0 @ 0042cfc0 ===== */

int FUN_0042cfc0(int param_1)

{
  int *piVar1;
  
  piVar1 = DAT_0121d040;
  if (DAT_0121d040 == (int *)0x0) {
    return 0;
  }
  do {
    if (*(int *)*piVar1 == param_1) {
      return ((int *)*piVar1)[1];
    }
    piVar1 = (int *)piVar1[1];
  } while (piVar1 != (int *)0x0);
  return 0;
}



/* ===== FUNCTION FUN_0042d004 @ 0042d004 ===== */

int * FUN_0042d004(int param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  
  if (DAT_0121d040 != (undefined4 *)0x0) {
    piVar1 = (int *)*DAT_0121d040;
    iVar3 = *piVar1;
    puVar2 = DAT_0121d040;
    while( true ) {
      if (param_1 == iVar3) {
        return piVar1;
      }
      puVar2 = (undefined4 *)puVar2[1];
      if (puVar2 == (undefined4 *)0x0) break;
      piVar1 = (int *)*puVar2;
      iVar3 = *piVar1;
    }
  }
  return (int *)0x0;
}



/* ===== FUNCTION FUN_0042d058 @ 0042d058 ===== */

void FUN_0042d058(void)

{
  int __fd;
  int iVar1;
  int iVar2;
  undefined4 local_20;
  undefined4 local_1c;
  uint local_18;
  
  __fd = open("/dev/standby",2);
  if (__fd < 0) {
    printf("%s(), line:%d. open standby device error!\n","api_system_standby",0x702);
    return;
  }
  printf("%s(), line:%d.\n","api_system_standby",0x706);
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d18c);
  iVar1 = open("/dev/fb0",2);
  if (iVar1 == -1) {
    printf("%s(), line: %d. Error: cannot open framebuffer device","api_osd_show_onoff",0x61d);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  else {
    DAT_0121d1a4 = 0;
    iVar2 = ioctl(iVar1,0x4611,1);
    if (iVar2 != 0) {
      printf("%s(), line: %d. Error: FBIOBLANK","api_osd_show_onoff",0x62a);
    }
    close(iVar1);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  if (DAT_0121d0d0 != 0) {
    FUN_0042f6c0(1,0);
  }
  DAT_0121d0d0 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  iVar1 = open("/dev/dis",1);
  if (-1 < iVar1) {
    local_18 = local_18 & 0xffffff00;
    local_20 = 1;
    local_1c = 1;
    ioctl(iVar1,0x800c0e04,&local_20);
    close(iVar1);
  }
  usleep(100000);
  ioctl(__fd,0x20002703,0);
  close(__fd);
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION FUN_0042d248 @ 0042d248 ===== */

void FUN_0042d248(void *param_1)

{
  int iVar1;
  int iVar2;
  pthread_attr_t pStack_40;
  pthread_t local_1c [2];
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d18c);
  iVar1 = open("/dev/fb0",2);
  if (iVar1 == -1) {
    printf("%s(), line: %d. Error: cannot open framebuffer device","api_osd_show_onoff",0x61d);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  else {
    DAT_0121d1a4 = 0;
    iVar2 = ioctl(iVar1,0x4611,1);
    if (iVar2 != 0) {
      printf("%s(), line: %d. Error: FBIOBLANK","api_osd_show_onoff",0x62a);
    }
    close(iVar1);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d18c);
  DAT_0121d1a4 = 1;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  local_1c[0] = 0;
  pthread_attr_init(&pStack_40);
  pthread_attr_setstacksize(&pStack_40,0x1000);
  pthread_attr_setdetachstate(&pStack_40,1);
  iVar1 = pthread_create(local_1c,&pStack_40,(__start_routine *)&LAB_0042b198,param_1);
  if (iVar1 == 0) {
    pthread_attr_destroy(&pStack_40);
  }
  return;
}



/* ===== FUNCTION FUN_0042d3c4 @ 0042d3c4 ===== */

void FUN_0042d3c4(void *param_1,undefined4 param_2,int param_3)

{
  memcpy(&DAT_0121d018,param_1,param_3 << 2);
  DAT_012146c6 = 0;
  return;
}



/* ===== FUNCTION FUN_0042d430 @ 0042d430 ===== */

bool FUN_0042d430(int param_1)

{
  if (DAT_012146c6 != '\0') {
    return (bool)DAT_012146c6;
  }
  if (DAT_0121d014 == 0) {
    return false;
  }
  if (param_1 != DAT_0121d018) {
    if (DAT_0121d014 < 2) {
      return false;
    }
    if (DAT_0121d01c != param_1) {
      if (DAT_0121d014 == 2) {
        return false;
      }
      if (param_1 != DAT_0121d020) {
        if (DAT_0121d014 == 3) {
          return false;
        }
        if (param_1 != DAT_0121d024) {
          if (DAT_0121d014 == 4) {
            return false;
          }
          if (param_1 != DAT_0121d028) {
            if (DAT_0121d014 == 5) {
              return false;
            }
            if (param_1 != DAT_0121d02c) {
              if (DAT_0121d014 == 6) {
                return false;
              }
              if (param_1 != DAT_0121d030) {
                if (DAT_0121d014 == 7) {
                  return false;
                }
                if (param_1 != DAT_0121d034) {
                  if (DAT_0121d014 == 8) {
                    return false;
                  }
                  if (param_1 != DAT_0121d038) {
                    if (DAT_0121d014 == 9) {
                      return false;
                    }
                    return param_1 == DAT_0121d03c;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return true;
}



/* ===== FUNCTION FUN_0042d540 @ 0042d540 ===== */

undefined4 FUN_0042d540(int param_1)

{
  int __fd;
  undefined4 uVar1;
  byte local_18 [12];
  
  __fd = open("/dev/sndC0i2so",1);
  if (__fd < 0) {
    puts("open snd_fd_i2so fail");
    return 0xffffffff;
  }
  local_18[0] = 0;
  if (param_1 == 0) {
    local_18[0] = DAT_012146c5;
    ioctl(__fd,0x8001080b,local_18);
    uVar1 = 0;
    if (DAT_0121d010 != 0) goto LAB_0042d5a8;
  }
  else {
    ioctl(__fd,0x4001080c,&DAT_012146c5);
    ioctl(__fd,0x8001080b,local_18);
    uVar1 = 1;
    if (DAT_0121d010 == 0) goto LAB_0042d5a8;
  }
  ioctl(__fd,0x2000080f,uVar1);
LAB_0042d5a8:
  printf("mute is %d, vol: %d\n",param_1,(uint)local_18[0]);
  close(__fd);
  return 0;
}



/* ===== FUNCTION FUN_0042d640 @ 0042d640 ===== */

undefined4 FUN_0042d640(undefined1 param_1)

{
  int __fd;
  undefined4 uVar1;
  undefined1 local_res0 [16];
  
  local_res0[0] = param_1;
  __fd = open("/dev/sndC0i2so",1);
  if (__fd < 0) {
    printf("open snd_fd %d failed\n",__fd);
    uVar1 = 0xffffffff;
  }
  else {
    ioctl(__fd,0x8001080b,local_res0);
    local_res0[0] = 0;
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042d72c @ 0042d72c ===== */

void FUN_0042d72c(void)

{
  return;
}



/* ===== FUNCTION FUN_0042d734 @ 0042d734 ===== */

void FUN_0042d734(byte param_1)

{
  int __fd;
  byte local_res0 [16];
  
  local_res0[0] = param_1;
  __fd = open("/dev/sndC0i2so",1);
  if (-1 < __fd) {
    ioctl(__fd,0x8001080b,local_res0);
    local_res0[0] = 0;
    ioctl(__fd,0x4001080c,local_res0);
    printf("%s volume is %d\n","api_set_volume",(uint)local_res0[0]);
    close(__fd);
    return;
  }
  printf("open snd_fd %d failed\n",__fd);
  return;
}



/* ===== FUNCTION FUN_0042d7d8 @ 0042d7d8 ===== */

uint FUN_0042d7d8(char *param_1,size_t param_2,char *param_3)

{
  int iVar1;
  ssize_t sVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint local_c8 [32];
  char acStack_48 [36];
  
  iVar1 = open("/proc/device-tree/hcrtos/sfspi/spi_nor_flash/partitions/part-num",0);
  if (iVar1 < 0) {
    printf("fd:%d ,dts value: %x\n",iVar1,0xffffffff);
    uVar5 = 0xffffffff;
  }
  else {
    sVar2 = read(iVar1,local_c8,4);
    if (sVar2 == 4) {
      close(iVar1);
      uVar5 = (((local_c8[0] >> 0x10) << 0x18 | (local_c8[0] >> 0x18) << 0x10) >> 0x10) +
              ((local_c8[0] & 0xff) << 8 | local_c8[0] >> 8 & 0xff) * 0x10000;
      printf("fd:%d ,dts value: %x\n",iVar1,uVar5);
      if (uVar5 == 0) goto LAB_0042d95c;
    }
    else {
      uVar5 = 0xffffffff;
      close(iVar1);
    }
  }
  uVar3 = 1;
  do {
    snprintf((char *)local_c8,0x80,
             "/proc/device-tree/hcrtos/sfspi/spi_nor_flash/partitions/part%d-label",uVar3);
    iVar1 = open((char *)local_c8,0);
    if (-1 < iVar1) {
      read(iVar1,acStack_48,0x20);
      close(iVar1);
    }
    iVar1 = strcmp(acStack_48,param_3);
    uVar4 = uVar3 + 1;
    if (iVar1 == 0) {
      memset(param_1,0,param_2);
      snprintf(param_1,param_2,"/dev/mtd%d",uVar3);
      printf("%s(), line:%d. devpath:%s\n","api_get_mtdblock_devpath",0x8af,param_1);
      return uVar3;
    }
    uVar3 = uVar4;
  } while (uVar4 <= uVar5);
LAB_0042d95c:
  printf("%s(), line:%d. cannot find mtd dev path!\n","api_get_mtdblock_devpath",0x8b3);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0042d9d4 @ 0042d9d4 ===== */

void FUN_0042d9d4(int param_1)

{
  int __fd;
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined2 local_48;
  undefined2 local_46;
  uint local_44;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  uint local_24 [2];
  
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  __fd = open("/dev/dis",1);
  if (-1 < __fd) {
    local_58 = 1;
    local_54 = 1;
    local_24[0] = FUN_00448108();
    local_28 = FUN_00448110();
    printf("h1: %d, v1: %d\n",local_24[0],local_28);
    FUN_004481a4(local_24,&local_28,param_1);
    iVar1 = FUN_004480f8();
    iVar2 = FUN_00448108();
    iVar2 = iVar2 - local_24[0];
    iVar3 = FUN_00448100();
    iVar4 = FUN_00448110();
    iVar1 = iVar2 / 2 + iVar1;
    iVar3 = (iVar4 - local_28) / 2 + iVar3;
    printf("h2: %d, v2: %d, x: %d, y: %d\n",local_24[0],local_28,iVar1,iVar3);
    local_50 = 0;
    local_48 = (undefined2)iVar1;
    local_46 = (undefined2)iVar3;
    local_3c = 1;
    local_44 = local_24[0] & 0xffff | local_28 << 0x10;
    local_38 = 1;
    local_4c = 0x4380780;
    if (param_1 == 2) {
      local_34 = 3;
    }
    else {
      local_34 = 5;
    }
    ioctl(__fd,0x801c0e02,&local_58);
    usleep(30000);
    ioctl(__fd,0x80140e03,&local_3c);
    close(__fd);
  }
  return;
}



/* ===== FUNCTION FUN_0042db78 @ 0042db78 ===== */

void FUN_0042db78(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
                 undefined2 param_5,undefined2 param_6,undefined2 param_7,undefined2 param_8)

{
  int __fd;
  undefined4 local_28;
  undefined4 local_24;
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined4 local_10;
  
  local_28 = 1;
  local_24 = 1;
  local_18 = param_5;
  local_10 = 0;
  local_16 = param_6;
  local_14 = param_7;
  local_12 = param_8;
  local_20 = param_1;
  local_1e = param_2;
  local_1c = param_3;
  local_1a = param_4;
  __fd = open("/dev/dis",1);
  if (-1 < __fd) {
    ioctl(__fd,0x801c0e02,&local_28);
    close(__fd);
  }
  return;
}



/* ===== FUNCTION FUN_0042dc0c @ 0042dc0c ===== */

void FUN_0042dc0c(int param_1,undefined4 param_2)

{
  int __fd;
  int iVar1;
  undefined4 local_28;
  int local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  printf("ratio: %d, dis_mode: %d\n",param_1,param_2);
  __fd = open("/dev/dis",1);
  if (-1 < __fd) {
    if (param_1 == 2) {
      param_1 = 1;
    }
    local_28 = 1;
    local_1c = 1;
    local_24 = param_1;
    local_20 = param_2;
    iVar1 = ioctl(__fd,0x80140e03,&local_28);
    if (iVar1 != 0) {
      printf("%s:%d: err: DIS_SET_ASPECT_MODE failed\n","api_set_display_aspect",0x955);
      close(__fd);
      return;
    }
    close(__fd);
  }
  return;
}



/* ===== FUNCTION FUN_0042dcf4 @ 0042dcf4 ===== */

undefined4 FUN_0042dcf4(undefined4 *param_1)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    *param_1 = 1;
    ioctl(__fd,0xc00c0e15,param_1);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042dd64 @ 0042dd64 ===== */

undefined4 FUN_0042dd64(undefined4 *param_1)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    *param_1 = 1;
    ioctl(__fd,0xc00c0e0c,param_1);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042ddd4 @ 0042ddd4 ===== */

undefined4 FUN_0042ddd4(int param_1,undefined4 param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int __fd;
  undefined4 uVar5;
  uint in_t0;
  undefined4 local_28;
  undefined4 local_24;
  uint local_20;
  uint local_1c;
  uint local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  uVar1 = param_1 + 0xbU & 3;
  uVar2 = param_1 + 0xfU & 3;
  uVar3 = param_1 + 0x13U & 3;
  uVar4 = param_1 + 8U & 3;
  local_20 = (*(int *)((param_1 + 0xbU) - uVar1) << (3 - uVar1) * 8 |
             in_t0 & 0xffffffffU >> (uVar1 + 1) * 8) & -1 << (4 - uVar4) * 8 |
             *(uint *)((param_1 + 8U) - uVar4) >> uVar4 * 8;
  uVar1 = param_1 + 0xcU & 3;
  local_1c = (*(int *)((param_1 + 0xfU) - uVar2) << (3 - uVar2) * 8 |
             param_4 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar1) * 8 |
             *(uint *)((param_1 + 0xcU) - uVar1) >> uVar1 * 8;
  uVar1 = param_1 + 0x10U & 3;
  local_18 = (*(int *)((param_1 + 0x13U) - uVar3) << (3 - uVar3) * 8 |
             param_3 & 0xffffffffU >> (uVar3 + 1) * 8) & -1 << (4 - uVar1) * 8 |
             *(uint *)((param_1 + 0x10U) - uVar1) >> uVar1 * 8;
  local_14 = *(undefined4 *)(param_1 + 0x14);
  local_28 = 1;
  local_24 = 1;
  local_10 = 0;
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar5 = 0xffffffff;
  }
  else {
    ioctl(__fd,0x801c0e02,&local_28);
    close(__fd);
    uVar5 = 0;
  }
  return uVar5;
}



/* ===== FUNCTION FUN_0042de74 @ 0042de74 ===== */

undefined4 FUN_0042de74(void)

{
  undefined4 uVar1;
  
  if (DAT_0121d00c != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0042de84. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_0121d00c)();
    return uVar1;
  }
  return 0;
}



/* ===== FUNCTION FUN_0042de94 @ 0042de94 ===== */

void FUN_0042de94(undefined4 param_1)

{
  DAT_0121d00c = param_1;
  return;
}



/* ===== FUNCTION FUN_0042dea0 @ 0042dea0 ===== */

int FUN_0042dea0(int param_1,int param_2)

{
  DIR *__dirp;
  dirent *pdVar1;
  int iVar2;
  char *__s;
  
  iVar2 = 0;
  __dirp = opendir("/media");
  if (__dirp == (DIR *)0x0) {
    printf("%s(), line: %d. open dir:%s error!\n","api_usb_dev_path_get",0x9b1,"/media");
    iVar2 = 0;
  }
  else {
    do {
      while( true ) {
        pdVar1 = readdir(__dirp);
        if (pdVar1 == (dirent *)0x0) {
          return iVar2;
        }
        if (pdVar1->d_name[0] == '.') break;
LAB_0042df04:
        if ((pdVar1->d_name[0] != '\0') && (pdVar1->d_type == '\x04')) {
          __s = (char *)(param_1 + iVar2 * 0x40);
          sprintf(__s,"%s/%s","/media",pdVar1->d_name);
          printf("%s(), line: %d. found USB device: %s!\n","api_usb_dev_path_get",0x9c2,__s);
          iVar2 = iVar2 + 1;
        }
LAB_0042df18:
        if (param_2 <= iVar2) {
          return iVar2;
        }
      }
      if (pdVar1->d_name[1] == '\0') goto LAB_0042df18;
      if ((pdVar1->d_name[1] != '.') || (pdVar1->d_name[2] != '\0')) goto LAB_0042df04;
    } while (iVar2 < param_2);
  }
  return iVar2;
}



/* ===== FUNCTION FUN_0042dff8 @ 0042dff8 ===== */

void FUN_0042dff8(int param_1,uint param_2,undefined4 param_3,uint *param_4,uint *param_5,
                 uint *param_6,undefined4 *param_7)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 local_10 [4];
  
  uVar1 = *param_4;
  local_10[1] = 0x10e;
  local_10[0] = 0;
  local_10[2] = 0xb4;
  local_10[3] = 0x5a;
  if ((param_1 == 0) || (param_1 == 0xb4)) {
    uVar3 = *param_5;
    uVar2 = *param_6;
    if (param_1 != 0x10e) goto LAB_0042e09c;
LAB_0042e040:
    uVar1 = uVar1 + 1;
  }
  else {
    uVar2 = *param_5;
    uVar3 = *param_6;
    if (param_1 == 0x10e) goto LAB_0042e040;
LAB_0042e09c:
    if (param_1 != 0x5a) {
      if (param_1 == 0xb4) {
        uVar1 = uVar1 + 2 & 3;
      }
      goto joined_r0x0042e0b4;
    }
    uVar1 = uVar1 + 3;
  }
  uVar1 = uVar1 & 3;
joined_r0x0042e0b4:
  if (uVar2 == 1) {
    if (uVar1 == 1) {
      uVar1 = 3;
      uVar3 = param_2 ^ 1;
    }
    else if ((int)uVar1 < 2) {
      uVar3 = param_2 ^ 1;
      if (uVar1 == 0) {
        uVar1 = 2;
      }
    }
    else if (uVar1 == 2) {
      uVar1 = 0;
      uVar3 = param_2 ^ 1;
    }
    else {
      if (uVar1 == 3) {
        uVar1 = 1;
      }
      uVar3 = param_2 ^ 1;
    }
  }
  else {
    uVar3 = uVar3 ^ param_2;
  }
  *param_4 = uVar1;
  if (param_5 != (uint *)0x0) {
    *param_5 = uVar3;
  }
  if (param_6 != (uint *)0x0) {
    *param_6 = 0;
  }
  if (param_7 != (undefined4 *)0x0) {
    *param_7 = local_10[uVar1];
  }
  return;
}



/* ===== FUNCTION FUN_0042e110 @ 0042e110 ===== */

void FUN_0042e110(void)

{
  pthread_attr_t pStack_30;
  pthread_t apStack_c [2];
  
  pthread_attr_init(&pStack_30);
  pthread_attr_setdetachstate(&pStack_30,1);
  pthread_attr_setstacksize(&pStack_30,0x1000);
  pthread_create(apStack_c,&pStack_30,(__start_routine *)&LAB_0042af20,(void *)0x1770);
  pthread_attr_destroy(&pStack_30);
  return;
}



/* ===== FUNCTION FUN_0042e164 @ 0042e164 ===== */

undefined4 FUN_0042e164(int param_1)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open("/dev/vidsink",1);
  if (__fd < 0) {
    puts("open vidsink failed");
    uVar1 = 0xffffffff;
  }
  else {
    if (param_1 == 0) {
      ioctl(__fd,0x20001a02,0);
    }
    else {
      ioctl(__fd,0x20001a01,0);
    }
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042e1f0 @ 0042e1f0 ===== */

undefined4 FUN_0042e1f0(void)

{
  int __fd;
  undefined4 local_10 [2];
  
  local_10[0] = 0xffffffff;
  __fd = open("/dev/backlight",2);
  if (0 < __fd) {
    read(__fd,local_10,4);
    close(__fd);
  }
  return local_10[0];
}



/* ===== FUNCTION FUN_0042e248 @ 0042e248 ===== */

undefined4 FUN_0042e248(undefined4 param_1)

{
  int iVar1;
  int __fd;
  undefined4 uVar2;
  undefined4 local_res0 [4];
  
  local_res0[0] = param_1;
  iVar1 = open("/dev/backlight",2);
  __fd = open("/dev/lvds",2);
  if (__fd < 0) {
    if (iVar1 < 0) {
      puts("open backlight failed");
      return 0xffffffff;
    }
    if (iVar1 != 0) goto LAB_0042e294;
  }
  else {
    if (0 < iVar1) {
LAB_0042e294:
      write(iVar1,local_res0,4);
      uVar2 = 0;
      close(iVar1);
      if (__fd == 0) {
        return 0;
      }
      goto LAB_0042e2dc;
    }
    if (__fd == 0) {
      return 0xffffffff;
    }
  }
  uVar2 = 0xffffffff;
LAB_0042e2dc:
  iVar1 = ioctl(__fd,0x20001312,local_res0[0]);
  if (iVar1 != -1) {
    uVar2 = 0;
  }
  iVar1 = ioctl(__fd,0x20001311,local_res0[0]);
  if (iVar1 != -1) {
    uVar2 = 0;
  }
  close(__fd);
  return uVar2;
}



/* ===== FUNCTION FUN_0042e35c @ 0042e35c ===== */

undefined4 FUN_0042e35c(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = DAT_0121d1ac;
  if ((DAT_0121d1ac != (int *)0x0) && (0 < *DAT_0121d1ac)) {
    iVar2 = glist_nth_data(DAT_0121d1ac[1],param_1);
    piVar1[2] = iVar2;
  }
  return 0;
}



/* ===== FUNCTION FUN_0042e3a4 @ 0042e3a4 ===== */

undefined4 FUN_0042e3a4(int param_1)

{
  undefined4 uVar1;
  
  if (((DAT_0121d1ac != (int *)0x0) && (0 < *DAT_0121d1ac)) && (param_1 <= *DAT_0121d1ac)) {
    uVar1 = glist_nth_data(DAT_0121d1ac[1],param_1);
    return uVar1;
  }
  return 0;
}



/* ===== FUNCTION FUN_0042e3dc @ 0042e3dc ===== */

undefined4 FUN_0042e3dc(void)

{
  int *piVar1;
  undefined4 in_v0;
  char *__s1;
  int iVar2;
  int iVar3;
  
  if ((DAT_0121d1ac != (int *)0x0) && (in_v0 = 0, DAT_0121d1ac[2] != 0)) {
    if (*DAT_0121d1ac < 1) {
      return 1;
    }
    iVar2 = DAT_0121d1ac[1];
    iVar3 = 0;
    while( true ) {
      __s1 = (char *)glist_nth_data(iVar2,iVar3);
      piVar1 = DAT_0121d1ac;
      iVar2 = strcmp(__s1,(char *)DAT_0121d1ac[2]);
      in_v0 = 0;
      if (iVar2 == 0) break;
      if (*piVar1 <= iVar3 + 1) {
        return 1;
      }
      iVar2 = piVar1[1];
      iVar3 = iVar3 + 1;
    }
  }
  return in_v0;
}



/* ===== FUNCTION FUN_0042e4ac @ 0042e4ac ===== */

undefined4 FUN_0042e4ac(void)

{
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0042e4b4 @ 0042e4b4 ===== */

void FUN_0042e4b4(int param_1)

{
  int *piVar1;
  char *pcVar2;
  
  piVar1 = __errno_location();
  pcVar2 = strerror(*piVar1);
  printf("%s(), signo: %d, error: %s\n","exit_console",param_1,pcVar2);
  FUN_004227e4(0,0);
  FUN_00412278();
  FUN_0042c530(0);
  if (param_1 - 10U < 2) {
    system("/mnt/sdcard/cubegm/icube_start.sh &");
  }
                    /* WARNING: Subroutine does not return */
  exit(0);
}



/* ===== FUNCTION FUN_0042e530 @ 0042e530 ===== */

undefined4 FUN_0042e530(void)

{
  return 0xffffffff;
}



/* ===== FUNCTION custom_tick_get @ 0042e538 ===== */

void custom_tick_get(void)

{
  custom_tick_get();
  return;
}



/* ===== FUNCTION FUN_0042e540 @ 0042e540 ===== */

undefined4 FUN_0042e540(void)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    usleep(60000);
    ioctl(__fd,0x20000e16,1);
    usleep(60000);
    close(__fd);
    uVar1 = 0;
    DAT_0121d008 = 1;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042e5b8 @ 0042e5b8 ===== */

undefined4 FUN_0042e5b8(void)

{
  int __fd;
  undefined4 uVar1;
  
  __fd = open("/dev/dis",1);
  if (__fd < 0) {
    uVar1 = 0xffffffff;
  }
  else {
    ioctl(__fd,0x20000e17,1);
    close(__fd);
    uVar1 = 0;
    DAT_0121d008 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0042e61c @ 0042e61c ===== */

undefined1 FUN_0042e61c(void)

{
  return DAT_0121d008;
}



/* ===== FUNCTION FUN_0042e628 @ 0042e628 ===== */

undefined4 FUN_0042e628(int param_1)

{
  int iVar1;
  
  iVar1 = _lv_ll_get_head(_lv_group_ll);
  while( true ) {
    if (iVar1 == 0) {
      return 0;
    }
    if (iVar1 == param_1) break;
    iVar1 = _lv_ll_get_next(_lv_group_ll,iVar1);
  }
  return 1;
}



/* ===== FUNCTION FUN_0042e734 @ 0042e734 ===== */

void FUN_0042e734(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  uint local_50;
  uint local_3c;
  uint local_38;
  uint local_34;
  uint local_30;
  
  iVar1 = lv_obj_get_child(param_1,0);
  local_3c = 0;
  do {
    uVar2 = lv_obj_get_child_cnt(param_1);
    if ((uVar2 <= local_3c) ||
       ((*(int *)(iVar1 + 8) != 0 && (*(int *)(*(int *)(iVar1 + 8) + 8) != 0)))) {
      return;
    }
    iVar3 = lv_obj_get_child_cnt(iVar1);
    if (iVar3 != 0) {
      iVar3 = lv_obj_get_child(iVar1,0);
      for (local_38 = 0; uVar2 = lv_obj_get_child_cnt(iVar1), local_38 < uVar2;
          local_38 = local_38 + 1) {
        if ((*(int *)(iVar3 + 8) != 0) && (*(int *)(*(int *)(iVar3 + 8) + 8) != 0)) {
          return;
        }
        iVar15 = lv_obj_get_child_cnt(iVar3);
        if (iVar15 != 0) {
          iVar15 = lv_obj_get_child(iVar3,0);
          for (local_34 = 0; uVar2 = lv_obj_get_child_cnt(iVar3), local_34 < uVar2;
              local_34 = local_34 + 1) {
            if ((*(int *)(iVar15 + 8) != 0) && (*(int *)(*(int *)(iVar15 + 8) + 8) != 0)) {
              return;
            }
            iVar4 = lv_obj_get_child_cnt(iVar15);
            if (iVar4 != 0) {
              iVar4 = lv_obj_get_child(iVar15,0);
              for (local_30 = 0; uVar2 = lv_obj_get_child_cnt(iVar15), local_30 < uVar2;
                  local_30 = local_30 + 1) {
                if ((*(int *)(iVar4 + 8) != 0) && (*(int *)(*(int *)(iVar4 + 8) + 8) != 0)) {
                  return;
                }
                iVar5 = lv_obj_get_child_cnt(iVar4);
                if (iVar5 != 0) {
                  iVar5 = lv_obj_get_child(iVar4,0);
                  for (local_50 = 0; uVar2 = lv_obj_get_child_cnt(iVar4), local_50 < uVar2;
                      local_50 = local_50 + 1) {
                    if ((*(int *)(iVar5 + 8) != 0) && (*(int *)(*(int *)(iVar5 + 8) + 8) != 0)) {
                      return;
                    }
                    iVar6 = lv_obj_get_child_cnt(iVar5);
                    if (iVar6 != 0) {
                      iVar6 = lv_obj_get_child(iVar5,0);
                      for (uVar2 = 0; uVar7 = lv_obj_get_child_cnt(iVar5), uVar2 < uVar7;
                          uVar2 = uVar2 + 1) {
                        if ((*(int *)(iVar6 + 8) != 0) && (*(int *)(*(int *)(iVar6 + 8) + 8) != 0))
                        {
                          return;
                        }
                        iVar8 = lv_obj_get_child_cnt(iVar6);
                        if (iVar8 != 0) {
                          iVar8 = lv_obj_get_child(iVar6,0);
                          for (uVar7 = 0; uVar9 = lv_obj_get_child_cnt(iVar6), uVar7 < uVar9;
                              uVar7 = uVar7 + 1) {
                            if ((*(int *)(iVar8 + 8) != 0) &&
                               (*(int *)(*(int *)(iVar8 + 8) + 8) != 0)) {
                              return;
                            }
                            iVar10 = lv_obj_get_child_cnt(iVar8);
                            if (iVar10 != 0) {
                              iVar10 = lv_obj_get_child(iVar8,0);
                              for (uVar9 = 0; uVar11 = lv_obj_get_child_cnt(iVar8), uVar9 < uVar11;
                                  uVar9 = uVar9 + 1) {
                                if ((*(int *)(iVar10 + 8) != 0) &&
                                   (*(int *)(*(int *)(iVar10 + 8) + 8) != 0)) {
                                  return;
                                }
                                iVar12 = lv_obj_get_child_cnt(iVar10);
                                if (iVar12 != 0) {
                                  iVar12 = lv_obj_get_child(iVar10,0);
                                  uVar11 = 0;
                                  while (uVar13 = lv_obj_get_child_cnt(iVar10), uVar11 < uVar13) {
                                    uVar11 = uVar11 + 1;
                                    if ((*(int *)(iVar12 + 8) != 0) &&
                                       (*(int *)(*(int *)(iVar12 + 8) + 8) != 0)) {
                                      return;
                                    }
                                    iVar14 = lv_obj_get_child_cnt(iVar12);
                                    if ((iVar14 != 0) &&
                                       (iVar14 = FUN_0042e734(iVar12), iVar14 != 0)) {
                                      return;
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    local_3c = local_3c + 1;
  } while( true );
}



/* ===== FUNCTION FUN_0042eae0 @ 0042eae0 ===== */

void FUN_0042eae0(undefined1 param_1)

{
  DAT_012146c4 = param_1;
  return;
}



/* ===== FUNCTION FUN_0042eaec @ 0042eaec ===== */

undefined4 FUN_0042eaec(undefined4 param_1,char *param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0;
  local_14 = 0;
  iVar1 = access(param_2,0);
  if (iVar1 == -1) {
    local_18 = 0xf;
    DAT_012146c4 = 0;
    local_14 = param_1;
    if ((DAT_012146d4 != -1) || (DAT_012146d4 = FUN_0042a538(100,8), DAT_012146d4 != -1)) {
      FUN_0042a628(DAT_012146d4,&local_18,8);
    }
    printf(">>!%s ,%d\n","api_storage_devinfo_check",0xcab);
    uVar2 = 0xffffffff;
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042ebb8 @ 0042ebb8 ===== */

void FUN_0042ebb8(void)

{
  timeval local_10;
  
  gettimeofday(&local_10,(__timezone_ptr_t)0x0);
  return;
}



/* ===== FUNCTION FUN_0042ec0c @ 0042ec0c ===== */

void FUN_0042ec0c(void)

{
  timeval local_10;
  
  gettimeofday(&local_10,(__timezone_ptr_t)0x0);
  DAT_0121cff8 = local_10.tv_sec * 1000000 + local_10.tv_usec;
  DAT_0121d000 = DAT_0121cff8;
  DAT_0121cffc = DAT_0121cff8 >> 0x1f;
  DAT_0121d004 = DAT_0121cff8 >> 0x1f;
  return;
}



/* ===== FUNCTION FUN_0042ec60 @ 0042ec60 ===== */

void FUN_0042ec60(uint *param_1)

{
  uint uVar1;
  timeval local_10;
  
  gettimeofday(&local_10,(__timezone_ptr_t)0x0);
  uVar1 = local_10.tv_sec * 1000000 + local_10.tv_usec;
  if (param_1 != (uint *)0x0) {
    *param_1 = uVar1 - DAT_0121cff8;
    param_1[1] = (((int)uVar1 >> 0x1f) - DAT_0121cffc) - (uint)(uVar1 < uVar1 - DAT_0121cff8);
  }
  DAT_0121cff8 = uVar1;
  DAT_0121cffc = (int)uVar1 >> 0x1f;
  return;
}



/* ===== FUNCTION FUN_0042ecf4 @ 0042ecf4 ===== */

undefined4 FUN_0042ecf4(void)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d174);
  if (DAT_012146cc != '\0') {
    FUN_004439b4();
    FUN_00428080();
    if ((DAT_0121d0d0 != 0) && (DAT_0121d050 != '\0')) {
      FUN_0042f6c0(0,0);
      DAT_0121d0d0 = 0;
      FUN_0042c144(&DAT_0121d050);
    }
  }
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d174);
  return 0;
}



/* ===== FUNCTION FUN_0042edd8 @ 0042edd8 ===== */

undefined4 FUN_0042edd8(undefined4 param_1)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d158);
  DAT_0121d170 = param_1;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d158);
  return 0;
}



/* ===== FUNCTION FUN_0042ee20 @ 0042ee20 ===== */

undefined4 FUN_0042ee20(void)

{
  undefined4 uVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d158);
  uVar1 = DAT_0121d170;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d158);
  return uVar1;
}



/* ===== FUNCTION FUN_0042ee6c @ 0042ee6c ===== */

undefined4 FUN_0042ee6c(void)

{
  return 0;
}



/* ===== FUNCTION FUN_0042ee7c @ 0042ee7c ===== */

void FUN_0042ee7c(byte *param_1,byte *param_2,int param_3)

{
  byte bVar1;
  byte bVar2;
  byte *pbVar3;
  
  if (0 < param_3) {
    pbVar3 = param_1 + param_3;
    do {
      bVar2 = *param_2;
      bVar1 = *param_1;
      *param_1 = bVar1 ^ bVar2;
      bVar2 = bVar1 ^ bVar2 ^ *param_2;
      *param_2 = bVar2;
      *param_1 = bVar2 ^ *param_1;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
    } while (param_1 != pbVar3);
  }
  return;
}



/* ===== FUNCTION FUN_0042eec8 @ 0042eec8 ===== */

void FUN_0042eec8(undefined1 param_1)

{
  DAT_0121d154 = param_1;
  return;
}



/* ===== FUNCTION FUN_0042eee8 @ 0042eee8 ===== */

void FUN_0042eee8(void)

{
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d13c);
  return;
}



/* ===== FUNCTION FUN_0042eef4 @ 0042eef4 ===== */

void FUN_0042eef4(void)

{
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d13c);
  return;
}



/* ===== FUNCTION FUN_0042ef00 @ 0042ef00 ===== */

void FUN_0042ef00(int param_1)

{
  int local_18 [3];
  
  local_18[0] = (param_1 == 0) + 0x104;
  local_18[1] = 0;
  if ((DAT_012146c0 != -1) ||
     (DAT_012146c0 = FUN_0042a440("/tmp/process_msg",100,8), DAT_012146c0 != -1)) {
    FUN_0042a628(DAT_012146c0,local_18,8);
  }
  return;
}



/* ===== FUNCTION FUN_0042ef78 @ 0042ef78 ===== */

void FUN_0042ef78(int param_1)

{
  int local_18 [3];
  
  local_18[0] = (param_1 == 0) + 0x10b;
  local_18[1] = 0;
  printf("msg:%d\n",local_18[0]);
  if ((DAT_012146c0 != -1) ||
     (DAT_012146c0 = FUN_0042a440("/tmp/process_msg",100,8), DAT_012146c0 != -1)) {
    FUN_0042a628(DAT_012146c0,local_18,8);
  }
  return;
}



/* ===== FUNCTION FUN_0042f000 @ 0042f000 ===== */

void FUN_0042f000(void)

{
  int __fd;
  int iVar1;
  
  FUN_00412278();
  FUN_0042219c();
  FUN_0041a59c(1);
  usleep(400000);
  FUN_0041a77c();
  if (DAT_0121cff0 != 0) {
    return;
  }
  DAT_0121cff0 = 1;
  printf("%s %d\n","find_icube_enter_multi_os",0xda5);
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d18c);
  __fd = open("/dev/fb0",2);
  if (__fd == -1) {
    printf("%s(), line: %d. Error: cannot open framebuffer device","api_osd_show_onoff",0x61d);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  else {
    DAT_0121d1a4 = 0;
    iVar1 = ioctl(__fd,0x4611,1);
    if (iVar1 != 0) {
      printf("%s(), line: %d. Error: FBIOBLANK","api_osd_show_onoff",0x62a);
    }
    close(__fd);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d18c);
  }
  system("/mnt/sdcard/cubegm/icube_start.sh &");
  return;
}



/* ===== FUNCTION FUN_0042f134 @ 0042f134 ===== */

void FUN_0042f134(ushort param_1,ushort param_2)

{
  int iVar1;
  int iVar2;
  undefined4 local_20;
  undefined4 local_1c;
  uint local_18;
  ushort local_14;
  ushort local_12;
  undefined *local_10;
  
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_10 = &DAT_00f00140;
  local_14 = param_1;
  local_12 = param_2;
  iVar1 = open("/dev/dis",2);
  if (-1 < iVar1) {
    local_20 = 1;
    iVar2 = ioctl(iVar1,0xc00c0e0c,&local_20);
    if (iVar2 != 0) {
      close(iVar1);
      puts("open /dev/dis failed");
      return;
    }
    close(iVar1);
    local_10 = (undefined *)local_18;
    iVar1 = open("/dev/fb0",1);
    if (0 < iVar1) {
      ioctl(iVar1,0x80080d00,&local_14);
      close(iVar1);
    }
    printf("scale.h_div = %d\r\n",(uint)local_14);
    printf("scale.v_div = %d\r\n",(uint)local_12);
    printf("scale.h_mul = %d\r\n",(uint)local_10 & 0xffff);
    printf("scale.v_mul = %d\r\n",(uint)local_10 >> 0x10);
  }
  return;
}



/* ===== FUNCTION FUN_0042f254 @ 0042f254 ===== */

undefined4 * FUN_0042f254(void)

{
  int iVar1;
  key_t __key;
  undefined4 *puVar2;
  stat sStack_a0;
  
  puVar2 = DAT_0121cff4;
  if (((DAT_0121cff4 == (undefined4 *)0x0) &&
      (iVar1 = __xstat(3,"/tmp/cubegame_status",&sStack_a0), iVar1 == 0)) &&
     ((sStack_a0.st_nlink & 0xf000) == 0x4000)) {
    printf("mkdir \"%s\" success\n","/tmp/cubegame_status");
    __key = ftok("/tmp/cubegame_status",0x61);
    if (__key == -1) {
      perror("ftok");
    }
    else {
      iVar1 = shmget(__key,4,0x3b6);
      if (iVar1 == -1) {
        perror("shmget");
      }
      else {
        puVar2 = shmat(iVar1,(void *)0x0,0);
        DAT_0121cff4 = puVar2;
        if (puVar2 == (undefined4 *)0x0) {
          perror("shmat");
        }
        else {
          printf("%s %d %d\n","cube_shmget_hdmi_tx_in_init",0xdfa,*puVar2);
          puVar2 = DAT_0121cff4;
        }
      }
    }
  }
  return puVar2;
}



/* ===== FUNCTION FUN_0042f374 @ 0042f374 ===== */

uint FUN_0042f374(void)

{
  uint uVar1;
  int iVar2;
  key_t __key;
  stat sStack_a0;
  
  if (DAT_0121cff4 == (uint *)0x0) {
    iVar2 = __xstat(3,"/tmp/cubegame_status",&sStack_a0);
    if (iVar2 != 0) {
      return 0;
    }
    if ((sStack_a0.st_nlink & 0xf000) != 0x4000) {
      return 0;
    }
    printf("mkdir \"%s\" success\n","/tmp/cubegame_status");
    __key = ftok("/tmp/cubegame_status",0x61);
    if (__key == -1) {
      perror("ftok");
      return 0;
    }
    iVar2 = shmget(__key,4,0x3b6);
    if (iVar2 == -1) {
      perror("shmget");
      return 0;
    }
    DAT_0121cff4 = shmat(iVar2,(void *)0x0,0);
    if (DAT_0121cff4 == (uint *)0x0) {
      perror("shmat");
      return 0;
    }
    printf("%s %d %d\n","cube_shmget_hdmi_tx_in_init",0xdfa,*DAT_0121cff4);
    if (DAT_0121cff4 == (uint *)0x0) {
      return 0;
    }
    uVar1 = *DAT_0121cff4;
  }
  else {
    uVar1 = *DAT_0121cff4;
  }
  return uVar1 & 1;
}



/* ===== FUNCTION FUN_0042f4a0 @ 0042f4a0 ===== */

undefined4 FUN_0042f4a0(undefined4 param_1,undefined4 param_2)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  undefined4 local_2a8 [2];
  undefined1 local_29d;
  undefined1 local_29a;
  undefined4 local_294;
  undefined4 local_290;
  undefined4 local_28c;
  undefined4 local_288;
  undefined4 local_284;
  undefined4 local_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_48;
  
  memset(local_2a8,0,0x298);
  local_2a8[0] = 0x1b;
  local_29a = 1;
  local_5c = 3;
  local_288 = 1;
  local_29d = 5;
  local_284 = 1;
  local_290 = 0x780;
  local_28c = 0x438;
  local_48 = 0x100000;
  local_294 = 60000;
  local_54 = param_2;
  local_50 = param_1;
  __fd = open("/dev/viddec",2);
  DAT_012146e0 = __fd;
  if (__fd < 0) {
    printf("%s Open /dev/viddec error!\n","com_logo_h264_video_open");
    uVar2 = 0xffffffff;
  }
  else {
    iVar1 = ioctl(__fd,0x82980400,local_2a8);
    if (iVar1 == 0) {
      ioctl(__fd,0x20000403,0);
      uVar2 = 0;
    }
    else {
      uVar2 = 0xffffffff;
      printf("%s Init viddec error!\n","com_logo_h264_video_open");
      close(DAT_012146e0);
      DAT_012146e0 = -1;
    }
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042f5d4 @ 0042f5d4 ===== */

undefined4 FUN_0042f5d4(void)

{
  int __fd;
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0;
  if (DAT_0121d1c0 != 0) {
    __fd = open("/dev/dis",1);
    if (__fd < 0) {
      printf("[%s] open dis error\n","com_logo_dis_backup_free");
      uVar2 = 0xffffffff;
    }
    else {
      iVar1 = ioctl(__fd,0x20000e17,1);
      if (iVar1 == 0) {
        close(__fd);
        DAT_0121d1c0 = 0;
        printf("[%s] free backup logo dis.\n","com_logo_dis_backup_free");
        return 0;
      }
      puts("dis free backup mp fail.");
      close(__fd);
      uVar2 = 0xffffffff;
    }
  }
  return uVar2;
}



/* ===== FUNCTION FUN_0042f6c0 @ 0042f6c0 ===== */

void FUN_0042f6c0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  undefined1 local_20;
  undefined1 local_1f;
  
  iVar1 = DAT_012146e0;
  local_20 = param_1 != 0;
  local_1f = param_2 != 0;
  if (0 < DAT_012146e0) {
    if (((param_2 == 0) && (param_1 == 0)) &&
       (ioctl(DAT_012146e0,0x80020401,&local_20), DAT_0121d1c0 == 0)) {
      iVar1 = open("/dev/dis",1);
      if (iVar1 < 0) {
        printf("[%s] open dis error\n","com_logo_dis_backup");
        iVar1 = DAT_012146e0;
      }
      else {
        usleep(100000);
        iVar2 = ioctl(iVar1,0x20000e16,1);
        if (iVar2 == 0) {
          usleep(100000);
          close(iVar1);
          DAT_0121d1c0 = 1;
          printf("[%s] backup logo dis.\n","com_logo_dis_backup");
          iVar1 = DAT_012146e0;
        }
        else {
          puts("dis backup mp fail.");
          close(iVar1);
          iVar1 = DAT_012146e0;
        }
      }
    }
    close(iVar1);
    DAT_012146e0 = -1;
  }
  return;
}



/* ===== FUNCTION FUN_0042f7fc @ 0042f7fc ===== */

undefined4 FUN_0042f7fc(char *param_1,undefined4 param_2,undefined4 param_3)

{
  FILE *__stream;
  size_t __size;
  void *__ptr;
  size_t sVar1;
  ssize_t sVar2;
  size_t sVar3;
  int iVar4;
  undefined4 local_38;
  undefined4 local_34;
  uint local_30;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined *local_28;
  undefined1 local_24;
  
  local_24 = 9;
  local_28 = &DAT_01000000;
  if (param_1 == (char *)0x0) {
    printf("[%s] invalid file path param\n","com_logo_show");
  }
  else {
    __stream = fopen(param_1,"rb");
    if (__stream == (FILE *)0x0) {
      printf("[%s] open %s file fail\n","com_logo_show",param_1);
    }
    else {
      fseek(__stream,0,2);
      __size = ftell(__stream);
      if ((int)__size < 1) {
        printf("[%s] file len error: %d\n","com_logo_show",__size);
      }
      else {
        fseek(__stream,0,0);
        __ptr = malloc(__size);
        if (__ptr != (void *)0x0) {
          iVar4 = 0;
          sVar3 = __size;
          do {
            sVar1 = fread((void *)((int)__ptr + iVar4),1,sVar3,__stream);
            sVar3 = sVar3 - sVar1;
            if ((int)sVar1 < 0) {
              printf("[%s] fread error.\n","com_logo_show");
              goto LAB_0042f9d8;
            }
            iVar4 = iVar4 + sVar1;
          } while (sVar3 != 0);
          iVar4 = FUN_0042f4a0(param_2,param_3);
          if (-1 < iVar4) {
            local_34 = 0;
            local_30 = __size & 0x3fffffff;
            local_38 = 0xffffffff;
            local_2c = (short)param_2;
            local_2a = (short)param_3;
            sVar2 = write(DAT_012146e0,&local_38,0x10);
            if (sVar2 == 0x10) {
              sVar3 = write(DAT_012146e0,__ptr,__size);
              if (__size != sVar3) {
                printf("%s Write video_frame error fail!\n","com_logo_h264_video_feed");
              }
            }
            else {
              printf("%s Write AvPktHd fail!\n","com_logo_h264_video_feed");
            }
            local_34 = 0;
            local_38 = 0xffffffff;
            local_30 = 5;
            local_2c = (short)param_2;
            local_2a = (short)param_3;
            sVar2 = write(DAT_012146e0,&local_38,0x10);
            if (sVar2 == 0x10) {
              sVar2 = write(DAT_012146e0,&local_28,5);
              if (sVar2 != 5) {
                printf("%s Write video_frame error fail!\n","com_logo_h264_video_feed");
              }
            }
            else {
              printf("%s Write AvPktHd fail!\n","com_logo_h264_video_feed");
            }
            fclose(__stream);
            free(__ptr);
            return 0;
          }
LAB_0042f9d8:
          fclose(__stream);
          free(__ptr);
          return 0xffffffff;
        }
        printf("[%s] data malloc error.\n","com_logo_show");
      }
      fclose(__stream);
    }
  }
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0042fb4c @ 0042fb4c ===== */

void FUN_0042fb4c(undefined4 param_1,undefined4 param_2)

{
  zip_name_locate(param_1,param_2,0);
  return;
}



/* ===== FUNCTION FUN_0042fb68 @ 0042fb68 ===== */

void FUN_0042fb68(void)

{
  zip_stat_index();
  return;
}



/* ===== FUNCTION FUN_00430004 @ 00430004 ===== */

void FUN_00430004(undefined4 param_1,int param_2,void *param_3,uint *param_4)

{
  char *__format;
  int iVar1;
  uint uVar2;
  longlong lVar3;
  undefined4 uVar4;
  char *pcVar5;
  char local_60 [8];
  char *local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  uint local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  uint local_28;
  undefined1 local_24;
  uint local_20;
  undefined1 local_1c;
  
  pcVar5 = local_60;
  local_60[0] = '\0';
  local_60[1] = '\0';
  local_60[2] = '\0';
  local_60[3] = '\0';
  local_60[4] = '\0';
  local_60[5] = '\0';
  local_60[6] = '\0';
  local_60[7] = '\0';
  local_58 = (char *)0x0;
  local_54 = 0;
  local_50 = 0;
  local_4c = 0;
  local_48 = 0;
  local_44 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  iVar1 = zip_stat_index(param_1,param_2,param_2,param_2 >> 0x1f,0,pcVar5);
  if (iVar1 == 0) {
    printf("File %s (Size: %llu)\n",local_58,local_48,local_44);
    param_3 = calloc(local_48 + 1,1);
    if (param_3 == (void *)0x0) {
      puts("zip buff malloc failed");
    }
    else {
      uVar4 = 0;
      if (local_30._2_2_ == 0) {
        iVar1 = zip_fopen_index(param_1);
      }
      else {
        pcVar5 = "hichip123";
        iVar1 = zip_fopen_index_encrypted(param_1);
      }
      __format = stderr;
      if (iVar1 == 0) {
        uVar4 = zip_strerror(param_1);
        printf(__format,"Failed to open file in ZIP %s\n",uVar4);
      }
      else {
        lVar3 = zip_fread(iVar1,param_3,local_48,local_44,uVar4,pcVar5);
        pcVar5 = stderr;
        if (lVar3 < 0) {
          uVar4 = zip_file_strerror(iVar1);
          printf(pcVar5,"Failed to read file in ZIP: %s\n",uVar4);
        }
        else {
          *(undefined1 *)((int)param_3 + (int)lVar3) = 0;
          printf("Read %lld bytes:\n");
          zip_fclose(iVar1);
        }
      }
    }
  }
  else {
    puts("get zip file info failed");
  }
  pcVar5 = strstr(local_58,(char *)&PTR_DAT_00452624);
  if (pcVar5 != (char *)0x0) {
    local_24 = 0;
    local_1c = 0;
    local_20 = *(uint *)((int)param_3 + 0x10);
    local_28 = *(uint *)((int)param_3 + 0x14);
    *param_4 = *param_4 & 0x3ff | ((local_20 >> 0x10 & 7) << 8 | local_20 >> 0x18) << 10 |
               ((local_28 >> 0x10 & 7) << 8 | local_28 >> 0x18) << 0x15;
    printf("w:%d, h:%d\n");
    uVar2 = *param_4;
    param_4[2] = (uint)param_3;
    *param_4 = uVar2 & 0xffffffe0 | 2;
    *param_4 = uVar2 & 0xffffff00 | 2;
    param_4[1] = local_48;
    return;
  }
  uVar2 = *param_4;
  param_4[2] = (uint)param_3;
  *param_4 = uVar2 & 0xffffffe0 | 1;
  *param_4 = uVar2 & 0xffffff00 | 1;
  param_4[1] = local_48;
  return;
}



/* ===== FUNCTION FUN_00430568 @ 00430568 ===== */

undefined4 FUN_00430568(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = (int *)(DAT_0121d1d0 + 8);
  iVar1 = 0;
  do {
    iVar2 = iVar1 + 1;
    if (*piVar3 != 0) {
      printf("%3d. %s",iVar1,*piVar3);
      puts("\r");
    }
    piVar3 = piVar3 + 1;
    iVar1 = iVar2;
  } while (iVar2 != 10);
  return 0;
}



/* ===== FUNCTION FUN_004305fc @ 004305fc ===== */

int FUN_004305fc(int param_1)

{
  char cVar1;
  size_t sVar2;
  size_t sVar3;
  char *pcVar4;
  char *pcVar5;
  
  pcVar4 = *(char **)(param_1 + 0x38);
  if (pcVar4 == (char *)0x0) {
    sVar2 = 0;
  }
  else {
    sVar2 = strlen(pcVar4);
    if (0 < (int)sVar2) {
      pcVar5 = pcVar4 + sVar2;
      do {
        cVar1 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        _IO_putc((int)cVar1,stdout);
      } while (pcVar5 != pcVar4);
    }
  }
  pcVar4 = *(char **)(param_1 + 0x3c);
  if (pcVar4 != (char *)0x0) {
    sVar3 = strlen(pcVar4);
    if (0 < (int)sVar3) {
      pcVar5 = pcVar4 + sVar3;
      do {
        cVar1 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        _IO_putc((int)cVar1,stdout);
      } while (pcVar5 != pcVar4);
    }
    sVar2 = sVar2 + sVar3;
  }
  pcVar4 = *(char **)(param_1 + 0x34);
  sVar3 = strlen(pcVar4);
  if (0 < (int)sVar3) {
    pcVar5 = pcVar4 + sVar3;
    do {
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
      _IO_putc((int)cVar1,stdout);
    } while (pcVar4 != pcVar5);
  }
  return sVar2 + sVar3;
}



/* ===== FUNCTION FUN_00430f78 @ 00430f78 ===== */

undefined4 FUN_00430f78(void)

{
  undefined4 *puVar1;
  char *pcVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  char *pcVar6;
  char *local_30 [2];
  
  puVar1 = DAT_0121d1d0;
  printf("\r\nCommands available:");
  puts("\r");
  puVar4 = (undefined4 *)*puVar1;
  if (puVar4 != (undefined4 *)0x0) {
    do {
      while (*(short *)((int)puVar4 + 10) == 1) {
        local_30[0] = (char *)puVar1[0x10];
        if (local_30[0] != (char *)0x0) {
          free(local_30[0]);
        }
        local_30[0] = calloc(1,1);
        puVar5 = puVar4;
        if (local_30[0] == (char *)0x0) {
          pcVar6 = (char *)0x0;
        }
        else {
          do {
            pcVar6 = local_30[0];
            if (*(undefined4 **)(puVar1[1] + 0x14) == puVar5) break;
            local_30[0] = (char *)0x0;
            pcVar2 = " ";
            if (*pcVar6 == '\0') {
              pcVar2 = "";
            }
            asprintf(local_30,"%s%s%s",*puVar5,pcVar2,pcVar6);
            puVar5 = (undefined4 *)puVar5[5];
            free(pcVar6);
          } while (puVar5 != (undefined4 *)0x0);
          puVar1[0x10] = local_30[0];
          pcVar6 = local_30[0];
        }
        puVar3 = (undefined *)puVar4[3];
        if ((undefined *)puVar4[3] == (undefined *)0x0) {
          puVar3 = &DAT_0044f178;
        }
        printf("  %-20s %s",pcVar6,puVar3);
        puts("\r");
        puVar4 = (undefined4 *)puVar4[6];
        if (puVar4 == (undefined4 *)0x0) goto LAB_004310ac;
      }
      puVar4 = (undefined4 *)puVar4[6];
    } while (puVar4 != (undefined4 *)0x0);
  }
LAB_004310ac:
  puVar4 = (undefined4 *)puVar1[1];
  if (puVar4 != (undefined4 *)0x0) {
    do {
      while (*(short *)((int)puVar4 + 10) != 0) {
        puVar4 = (undefined4 *)puVar4[6];
        if (puVar4 == (undefined4 *)0x0) {
          return 0;
        }
      }
      local_30[0] = (char *)puVar1[0x10];
      if (local_30[0] != (char *)0x0) {
        free(local_30[0]);
      }
      local_30[0] = calloc(1,1);
      puVar5 = puVar4;
      if (local_30[0] == (char *)0x0) {
        pcVar6 = (char *)0x0;
      }
      else {
        do {
          pcVar6 = local_30[0];
          if (*(undefined4 **)(puVar1[1] + 0x14) == puVar5) break;
          local_30[0] = (char *)0x0;
          pcVar2 = " ";
          if (*pcVar6 == '\0') {
            pcVar2 = "";
          }
          asprintf(local_30,"%s%s%s",*puVar5,pcVar2,pcVar6);
          puVar5 = (undefined4 *)puVar5[5];
          free(pcVar6);
        } while (puVar5 != (undefined4 *)0x0);
        puVar1[0x10] = local_30[0];
        pcVar6 = local_30[0];
      }
      puVar3 = (undefined *)puVar4[3];
      if ((undefined *)puVar4[3] == (undefined *)0x0) {
        puVar3 = &DAT_0044f178;
      }
      printf("  %-20s %s",pcVar6,puVar3);
      puts("\r");
      puVar4 = (undefined4 *)puVar4[6];
    } while (puVar4 != (undefined4 *)0x0);
  }
  return 0;
}



/* ===== FUNCTION FUN_004313cc @ 004313cc ===== */

undefined4 FUN_004313cc(undefined4 param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  char *pcVar10;
  char cVar11;
  char *pcVar12;
  char cVar13;
  undefined4 *puVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  undefined4 *puVar18;
  undefined4 *puVar19;
  undefined4 *puVar20;
  undefined4 *puVar21;
  undefined4 *puVar22;
  undefined4 *puVar23;
  
  puVar9 = param_2;
  do {
    if (puVar9 == (undefined4 *)0x0) {
      return 0;
    }
    *(undefined2 *)(puVar9 + 2) = 1;
    puVar14 = param_2;
    do {
      if (puVar9 != puVar14) {
        pcVar10 = (char *)*puVar9;
        pcVar12 = (char *)*puVar14;
        if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar10 != *pcVar12)) {
          iVar1 = 1;
        }
        else {
          iVar1 = 1;
          do {
            pcVar12 = pcVar12 + 1;
            pcVar10 = pcVar10 + 1;
            iVar1 = iVar1 + 1;
            if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
          } while (*pcVar10 == *pcVar12);
        }
        if ((int)(uint)*(ushort *)(puVar9 + 2) < iVar1) {
          *(short *)(puVar9 + 2) = (short)iVar1;
        }
      }
      puVar14 = (undefined4 *)puVar14[6];
    } while (puVar14 != (undefined4 *)0x0);
    puVar2 = (undefined4 *)puVar9[4];
    for (puVar14 = puVar2; puVar14 != (undefined4 *)0x0; puVar14 = (undefined4 *)puVar14[6]) {
      *(undefined2 *)(puVar14 + 2) = 1;
      puVar15 = puVar2;
      do {
        if (puVar14 != puVar15) {
          pcVar10 = (char *)*puVar14;
          pcVar12 = (char *)*puVar15;
          if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar12 != *pcVar10)) {
            iVar1 = 1;
          }
          else {
            iVar1 = 1;
            do {
              pcVar12 = pcVar12 + 1;
              pcVar10 = pcVar10 + 1;
              iVar1 = iVar1 + 1;
              if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
            } while (*pcVar10 == *pcVar12);
          }
          if ((int)(uint)*(ushort *)(puVar14 + 2) < iVar1) {
            *(short *)(puVar14 + 2) = (short)iVar1;
          }
        }
        puVar15 = (undefined4 *)puVar15[6];
      } while (puVar15 != (undefined4 *)0x0);
      puVar3 = (undefined4 *)puVar14[4];
      for (puVar15 = puVar3; puVar15 != (undefined4 *)0x0; puVar15 = (undefined4 *)puVar15[6]) {
        *(undefined2 *)(puVar15 + 2) = 1;
        puVar16 = puVar3;
        do {
          if (puVar15 != puVar16) {
            pcVar10 = (char *)*puVar15;
            pcVar12 = (char *)*puVar16;
            if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar12 != *pcVar10)) {
              iVar1 = 1;
            }
            else {
              iVar1 = 1;
              do {
                pcVar12 = pcVar12 + 1;
                pcVar10 = pcVar10 + 1;
                iVar1 = iVar1 + 1;
                if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
              } while (*pcVar10 == *pcVar12);
            }
            if ((int)(uint)*(ushort *)(puVar15 + 2) < iVar1) {
              *(short *)(puVar15 + 2) = (short)iVar1;
            }
          }
          puVar16 = (undefined4 *)puVar16[6];
        } while (puVar16 != (undefined4 *)0x0);
        puVar4 = (undefined4 *)puVar15[4];
        for (puVar16 = puVar4; puVar16 != (undefined4 *)0x0; puVar16 = (undefined4 *)puVar16[6]) {
          *(undefined2 *)(puVar16 + 2) = 1;
          puVar17 = puVar4;
          do {
            if (puVar16 != puVar17) {
              pcVar10 = (char *)*puVar16;
              pcVar12 = (char *)*puVar17;
              if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar12 != *pcVar10)) {
                iVar1 = 1;
              }
              else {
                iVar1 = 1;
                do {
                  pcVar12 = pcVar12 + 1;
                  pcVar10 = pcVar10 + 1;
                  iVar1 = iVar1 + 1;
                  if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
                } while (*pcVar10 == *pcVar12);
              }
              if ((int)(uint)*(ushort *)(puVar16 + 2) < iVar1) {
                *(short *)(puVar16 + 2) = (short)iVar1;
              }
            }
            puVar17 = (undefined4 *)puVar17[6];
          } while (puVar17 != (undefined4 *)0x0);
          puVar5 = (undefined4 *)puVar16[4];
          for (puVar17 = puVar5; puVar17 != (undefined4 *)0x0; puVar17 = (undefined4 *)puVar17[6]) {
            *(undefined2 *)(puVar17 + 2) = 1;
            puVar18 = puVar5;
            do {
              if (puVar17 != puVar18) {
                pcVar10 = (char *)*puVar17;
                pcVar12 = (char *)*puVar18;
                if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar12 != *pcVar10)) {
                  iVar1 = 1;
                }
                else {
                  iVar1 = 1;
                  do {
                    pcVar12 = pcVar12 + 1;
                    pcVar10 = pcVar10 + 1;
                    iVar1 = iVar1 + 1;
                    if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
                  } while (*pcVar10 == *pcVar12);
                }
                if ((int)(uint)*(ushort *)(puVar17 + 2) < iVar1) {
                  *(short *)(puVar17 + 2) = (short)iVar1;
                }
              }
              puVar18 = (undefined4 *)puVar18[6];
            } while (puVar18 != (undefined4 *)0x0);
            puVar6 = (undefined4 *)puVar17[4];
            for (puVar18 = puVar6; puVar18 != (undefined4 *)0x0; puVar18 = (undefined4 *)puVar18[6])
            {
              *(undefined2 *)(puVar18 + 2) = 1;
              puVar19 = puVar6;
              do {
                if (puVar18 != puVar19) {
                  pcVar10 = (char *)*puVar18;
                  pcVar12 = (char *)*puVar19;
                  if (((*pcVar10 == '\0') || (*pcVar12 == '\0')) || (*pcVar12 != *pcVar10)) {
                    iVar1 = 1;
                  }
                  else {
                    iVar1 = 1;
                    do {
                      pcVar12 = pcVar12 + 1;
                      pcVar10 = pcVar10 + 1;
                      iVar1 = iVar1 + 1;
                      if ((*pcVar10 == '\0') || (*pcVar12 == '\0')) break;
                    } while (*pcVar10 == *pcVar12);
                  }
                  if ((int)(uint)*(ushort *)(puVar18 + 2) < iVar1) {
                    *(short *)(puVar18 + 2) = (short)iVar1;
                  }
                }
                puVar19 = (undefined4 *)puVar19[6];
              } while (puVar19 != (undefined4 *)0x0);
              puVar7 = (undefined4 *)puVar18[4];
              for (puVar19 = puVar7; puVar19 != (undefined4 *)0x0;
                  puVar19 = (undefined4 *)puVar19[6]) {
                *(undefined2 *)(puVar19 + 2) = 1;
                puVar20 = puVar7;
                do {
                  if (puVar19 != puVar20) {
                    pcVar10 = (char *)*puVar19;
                    cVar13 = *pcVar10;
                    pcVar12 = (char *)*puVar20;
                    if ((cVar13 == '\0') || (cVar11 = *pcVar12, cVar11 == '\0')) {
                      iVar1 = 1;
                    }
                    else {
                      iVar1 = 1;
                      while( true ) {
                        pcVar12 = pcVar12 + 1;
                        pcVar10 = pcVar10 + 1;
                        if (cVar13 != cVar11) break;
                        cVar11 = *pcVar10;
                        iVar1 = iVar1 + 1;
                        if ((cVar11 == '\0') || (cVar13 = *pcVar12, cVar13 == '\0')) break;
                      }
                    }
                    if ((int)(uint)*(ushort *)(puVar19 + 2) < iVar1) {
                      *(short *)(puVar19 + 2) = (short)iVar1;
                    }
                  }
                  puVar20 = (undefined4 *)puVar20[6];
                } while (puVar20 != (undefined4 *)0x0);
                puVar8 = (undefined4 *)puVar19[4];
                for (puVar20 = puVar8; puVar20 != (undefined4 *)0x0;
                    puVar20 = (undefined4 *)puVar20[6]) {
                  *(undefined2 *)(puVar20 + 2) = 1;
                  puVar21 = puVar8;
                  do {
                    if (puVar20 != puVar21) {
                      pcVar10 = (char *)*puVar20;
                      cVar13 = *pcVar10;
                      pcVar12 = (char *)*puVar21;
                      if ((cVar13 == '\0') || (cVar11 = *pcVar12, cVar11 == '\0')) {
                        iVar1 = 1;
                      }
                      else {
                        iVar1 = 1;
                        while( true ) {
                          pcVar12 = pcVar12 + 1;
                          pcVar10 = pcVar10 + 1;
                          if (cVar13 != cVar11) break;
                          cVar11 = *pcVar10;
                          iVar1 = iVar1 + 1;
                          if ((cVar11 == '\0') || (cVar13 = *pcVar12, cVar13 == '\0')) break;
                        }
                      }
                      if ((int)(uint)*(ushort *)(puVar20 + 2) < iVar1) {
                        *(short *)(puVar20 + 2) = (short)iVar1;
                      }
                    }
                    puVar21 = (undefined4 *)puVar21[6];
                  } while (puVar21 != (undefined4 *)0x0);
                  puVar23 = (undefined4 *)puVar20[4];
                  for (puVar21 = puVar23; puVar21 != (undefined4 *)0x0;
                      puVar21 = (undefined4 *)puVar21[6]) {
                    *(undefined2 *)(puVar21 + 2) = 1;
                    puVar22 = puVar23;
                    do {
                      if (puVar21 != puVar22) {
                        pcVar10 = (char *)*puVar21;
                        cVar13 = *pcVar10;
                        pcVar12 = (char *)*puVar22;
                        if ((cVar13 == '\0') || (cVar11 = *pcVar12, cVar11 == '\0')) {
                          iVar1 = 1;
                        }
                        else {
                          iVar1 = 1;
                          while( true ) {
                            pcVar12 = pcVar12 + 1;
                            pcVar10 = pcVar10 + 1;
                            if (cVar13 != cVar11) break;
                            cVar13 = *pcVar10;
                            iVar1 = iVar1 + 1;
                            if ((cVar13 == '\0') || (cVar11 = *pcVar12, cVar11 == '\0')) break;
                          }
                        }
                        if ((int)(uint)*(ushort *)(puVar21 + 2) < iVar1) {
                          *(short *)(puVar21 + 2) = (short)iVar1;
                        }
                      }
                      puVar22 = (undefined4 *)puVar22[6];
                    } while (puVar22 != (undefined4 *)0x0);
                    if (puVar21[4] != 0) {
                      FUN_004313cc(param_1);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    puVar9 = (undefined4 *)puVar9[6];
  } while( true );
}



/* ===== FUNCTION FUN_00431a08 @ 00431a08 ===== */

int * FUN_00431a08(int *param_1,int param_2,undefined1 *param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  char *pcVar5;
  char cVar6;
  char cVar7;
  int *piVar8;
  int *piVar9;
  undefined4 uVar10;
  int *piVar11;
  
  if (param_2 == 0) {
    piVar11 = (int *)0x0;
    piVar1 = (int *)*param_1;
  }
  else {
    piVar1 = calloc(0x1c,1);
    piVar11 = piVar1;
    if (piVar1 != (int *)0x0) {
      if (param_3 == (undefined1 *)0x0) {
        param_3 = &LAB_00430560;
      }
      piVar1[1] = (int)param_3;
      piVar1[4] = 0;
      *(undefined2 *)((int)piVar1 + 10) = 1;
      piVar1[6] = 0;
      iVar2 = __strdup(param_2);
      *piVar1 = iVar2;
      if (iVar2 == 0) {
        free(piVar1);
        piVar11 = (int *)0x0;
        piVar1 = (int *)*param_1;
        goto LAB_00431abc;
      }
      piVar1[5] = 0;
      if (param_4 != 0) {
        iVar2 = __strdup(param_4);
        piVar1[3] = iVar2;
        if (iVar2 == 0) {
          free((void *)*piVar1);
          piVar11 = (int *)0x0;
          free(piVar1);
          piVar1 = (int *)*param_1;
          goto LAB_00431abc;
        }
      }
      iVar2 = *param_1;
      if (*param_1 == 0) {
        *param_1 = (int)piVar1;
        param_1[1] = (int)piVar1;
        goto LAB_00431acc;
      }
      do {
        iVar4 = iVar2;
        iVar2 = *(int *)(iVar4 + 0x18);
      } while (iVar2 != 0);
      *(int **)(iVar4 + 0x18) = piVar1;
    }
    piVar1 = (int *)*param_1;
  }
LAB_00431abc:
  if (piVar1 == (int *)0x0) {
    return piVar11;
  }
LAB_00431acc:
  uVar10 = 1;
  piVar9 = piVar1;
  do {
    *(short *)(piVar1 + 2) = (short)uVar10;
    piVar8 = piVar9;
    do {
      if (piVar8 != piVar1) {
        pcVar3 = (char *)*piVar1;
        cVar6 = *pcVar3;
        pcVar5 = (char *)*piVar8;
        if ((cVar6 == '\0') || (cVar7 = *pcVar5, cVar7 == '\0')) {
          iVar2 = 1;
        }
        else {
          iVar2 = 1;
          while( true ) {
            pcVar5 = pcVar5 + 1;
            pcVar3 = pcVar3 + 1;
            if (cVar7 != cVar6) break;
            cVar6 = *pcVar3;
            iVar2 = iVar2 + 1;
            if ((cVar6 == '\0') || (cVar7 = *pcVar5, cVar7 == '\0')) break;
          }
        }
        if ((int)(uint)*(ushort *)(piVar1 + 2) < iVar2) {
          *(short *)(piVar1 + 2) = (short)iVar2;
        }
      }
      piVar8 = (int *)piVar8[6];
    } while (piVar8 != (int *)0x0);
    if (piVar1[4] != 0) {
      FUN_004313cc(param_1);
    }
    piVar1 = (int *)piVar1[6];
    if (piVar1 == (int *)0x0) {
      return piVar11;
    }
  } while( true );
}



/* ===== FUNCTION FUN_00433634 @ 00433634 ===== */

void FUN_00433634(char *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = DAT_0121d1d0;
  if (*(void **)(DAT_0121d1d0 + 0x38) != (void *)0x0) {
    free(*(void **)(DAT_0121d1d0 + 0x38));
    *(undefined4 *)(iVar1 + 0x38) = 0;
  }
  if ((param_1 != (char *)0x0) && (*param_1 != '\0')) {
    uVar2 = __strdup(param_1);
    *(undefined4 *)(iVar1 + 0x38) = uVar2;
  }
  return;
}



/* ===== FUNCTION FUN_0043369c @ 0043369c ===== */

int * FUN_0043369c(int param_1,int param_2,undefined1 *param_3,undefined2 param_4,int param_5)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  char cVar7;
  char cVar8;
  int *piVar9;
  int *piVar10;
  undefined4 uVar11;
  int *piVar12;
  
  piVar1 = DAT_0121d1d0;
  if (param_2 == 0) {
    piVar12 = (int *)0x0;
    piVar2 = (int *)*DAT_0121d1d0;
  }
  else {
    piVar2 = calloc(0x1c,1);
    piVar12 = piVar2;
    if (piVar2 == (int *)0x0) {
LAB_00433870:
      piVar2 = (int *)*piVar1;
    }
    else {
      if (param_3 == (undefined1 *)0x0) {
        param_3 = &LAB_00430560;
      }
      piVar2[1] = (int)param_3;
      *(undefined2 *)((int)piVar2 + 10) = param_4;
      piVar2[4] = 0;
      piVar2[6] = 0;
      iVar3 = __strdup(param_2);
      *piVar2 = iVar3;
      if (iVar3 == 0) {
        free(piVar2);
        piVar12 = (int *)0x0;
        piVar2 = (int *)*piVar1;
      }
      else {
        piVar2[5] = param_1;
        if (param_5 != 0) {
          iVar3 = __strdup(param_5);
          piVar2[3] = iVar3;
          if (iVar3 == 0) {
            free((void *)*piVar2);
            piVar12 = (int *)0x0;
            free(piVar2);
            piVar2 = (int *)*piVar1;
            goto LAB_00433768;
          }
        }
        if (param_1 == 0) {
          iVar3 = *piVar1;
          if (*piVar1 == 0) {
            *piVar1 = (int)piVar2;
            piVar1[1] = (int)piVar2;
            goto LAB_00433778;
          }
          do {
            iVar5 = iVar3;
            iVar3 = *(int *)(iVar5 + 0x18);
          } while (iVar3 != 0);
          *(int **)(iVar5 + 0x18) = piVar2;
          goto LAB_00433870;
        }
        iVar3 = *(int *)(param_1 + 0x10);
        if (*(int *)(param_1 + 0x10) == 0) {
          *(int **)(param_1 + 0x10) = piVar2;
          piVar2 = (int *)*piVar1;
        }
        else {
          do {
            iVar5 = iVar3;
            iVar3 = *(int *)(iVar5 + 0x18);
          } while (iVar3 != 0);
          *(int **)(iVar5 + 0x18) = piVar2;
          piVar2 = (int *)*piVar1;
        }
      }
    }
  }
LAB_00433768:
  if (piVar2 == (int *)0x0) {
    return piVar12;
  }
LAB_00433778:
  uVar11 = 1;
  piVar10 = piVar2;
  do {
    *(short *)(piVar2 + 2) = (short)uVar11;
    piVar9 = piVar10;
    do {
      if (piVar2 != piVar9) {
        pcVar4 = (char *)*piVar2;
        cVar7 = *pcVar4;
        pcVar6 = (char *)*piVar9;
        if ((cVar7 == '\0') || (cVar8 = *pcVar6, cVar8 == '\0')) {
          iVar3 = 1;
        }
        else {
          iVar3 = 1;
          while( true ) {
            pcVar6 = pcVar6 + 1;
            pcVar4 = pcVar4 + 1;
            if (cVar8 != cVar7) break;
            cVar7 = *pcVar4;
            iVar3 = iVar3 + 1;
            if ((cVar7 == '\0') || (cVar8 = *pcVar6, cVar8 == '\0')) break;
          }
        }
        if ((int)(uint)*(ushort *)(piVar2 + 2) < iVar3) {
          *(short *)(piVar2 + 2) = (short)iVar3;
        }
      }
      piVar9 = (int *)piVar9[6];
    } while (piVar9 != (int *)0x0);
    if (piVar2[4] != 0) {
      FUN_004313cc(piVar1);
    }
    piVar2 = (int *)piVar2[6];
    if (piVar2 == (int *)0x0) {
      return piVar12;
    }
  } while( true );
}



/* ===== FUNCTION FUN_00433a54 @ 00433a54 ===== */

void FUN_00433a54(char *param_1)

{
  int *piVar1;
  int iVar2;
  int *__ptr;
  char *pcVar3;
  int iVar4;
  char *pcVar5;
  char cVar6;
  char cVar7;
  int *piVar8;
  int *piVar9;
  undefined4 uVar10;
  
  piVar1 = calloc(0x44,1);
  if (piVar1 == (int *)0x0) {
    DAT_0121d1d0 = piVar1;
    return;
  }
  DAT_0121d1d0 = piVar1;
  if ((void *)piVar1[0xd] != (void *)0x0) {
    free((void *)piVar1[0xd]);
    piVar1[0xd] = 0;
  }
  iVar2 = __strdup(&DAT_004526cc);
  piVar1[0xd] = iVar2;
  if ((void *)piVar1[0xe] != (void *)0x0) {
    free((void *)piVar1[0xe]);
    piVar1[0xe] = 0;
  }
  if ((param_1 != (char *)0x0) && (*param_1 != '\0')) {
    iVar2 = __strdup(param_1);
    piVar1[0xe] = iVar2;
  }
  if ((void *)piVar1[0xf] != (void *)0x0) {
    free((void *)piVar1[0xf]);
    piVar1[0xf] = 0;
  }
  FUN_00431a08(piVar1,&DAT_004526e4,FUN_00430f78,"Show available cmds");
  FUN_00431a08(piVar1,&DAT_00452708,&LAB_004311d4,"Exit from current cmd set");
  __ptr = calloc(0x1c,1);
  if (__ptr == (int *)0x0) {
LAB_00433bb0:
    __ptr = (int *)*piVar1;
  }
  else {
    __ptr[4] = 0;
    __ptr[6] = 0;
    __ptr[1] = (int)FUN_00430568;
    *(undefined2 *)((int)__ptr + 10) = 1;
    iVar2 = __strdup("history");
    *__ptr = iVar2;
    if (iVar2 == 0) {
      free(__ptr);
      __ptr = (int *)*piVar1;
    }
    else {
      __ptr[5] = 0;
      iVar2 = __strdup("Show history cmds");
      __ptr[3] = iVar2;
      if (iVar2 != 0) {
        iVar2 = *piVar1;
        if (*piVar1 == 0) {
          *piVar1 = (int)__ptr;
          piVar1[1] = (int)__ptr;
          goto LAB_00433bc4;
        }
        do {
          iVar4 = iVar2;
          iVar2 = *(int *)(iVar4 + 0x18);
        } while (iVar2 != 0);
        *(int **)(iVar4 + 0x18) = __ptr;
        goto LAB_00433bb0;
      }
      free((void *)*__ptr);
      free(__ptr);
      __ptr = (int *)*piVar1;
    }
  }
  if (__ptr == (int *)0x0) {
    return;
  }
LAB_00433bc4:
  uVar10 = 1;
  piVar9 = __ptr;
  do {
    *(short *)(piVar9 + 2) = (short)uVar10;
    piVar8 = __ptr;
    do {
      if (piVar8 != piVar9) {
        pcVar3 = (char *)*piVar9;
        cVar6 = *pcVar3;
        pcVar5 = (char *)*piVar8;
        if ((cVar6 == '\0') || (cVar7 = *pcVar5, cVar7 == '\0')) {
          iVar2 = 1;
        }
        else {
          iVar2 = 1;
          while( true ) {
            pcVar5 = pcVar5 + 1;
            pcVar3 = pcVar3 + 1;
            if (cVar7 != cVar6) break;
            cVar6 = *pcVar3;
            iVar2 = iVar2 + 1;
            if ((cVar6 == '\0') || (cVar7 = *pcVar5, cVar7 == '\0')) break;
          }
        }
        if ((int)(uint)*(ushort *)(piVar9 + 2) < iVar2) {
          *(short *)(piVar9 + 2) = (short)iVar2;
        }
      }
      piVar8 = (int *)piVar8[6];
    } while (piVar8 != (int *)0x0);
    if (piVar9[4] != 0) {
      FUN_004313cc(piVar1);
    }
    piVar9 = (int *)piVar9[6];
    if (piVar9 == (int *)0x0) {
      return;
    }
  } while( true );
}



/* ===== FUNCTION FUN_00433cd0 @ 00433cd0 ===== */

void FUN_00433cd0(int param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  if (param_1 - 1U < 3) {
    iVar2 = (param_1 - 1U) * 4;
    uVar1 = *(undefined4 *)(&DAT_00452750 + iVar2);
    *param_2 = *(undefined4 *)(&DAT_0045275c + iVar2);
    *param_3 = uVar1;
    *param_4 = 0;
    return;
  }
  *param_2 = 0;
  *param_3 = 0;
  *param_4 = 0;
  return;
}



/* ===== FUNCTION FUN_00433d28 @ 00433d28 ===== */

undefined4 FUN_00433d28(int param_1)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  iVar1 = FUN_0042de74();
  piVar2 = (int *)FUN_0041d32c();
  uVar3 = FUN_0042b734();
  uVar4 = FUN_0042b888();
  uVar5 = FUN_0042b894();
  if (param_1 - 1U < 3) {
    iVar6 = (param_1 - 1U) * 4;
    local_1c = *(undefined4 *)(&DAT_0045275c + iVar6);
    local_20 = *(undefined4 *)(&DAT_00452750 + iVar6);
  }
  else {
    local_20 = 0;
    local_1c = 0;
  }
  local_24 = 0;
  FUN_0042dff8(uVar3,uVar4,uVar5,&local_1c,&local_20,&local_24,&local_28);
  fbdev_set_rotate(local_28,local_20,local_24);
  uVar3 = lv_disp_get_default();
  uVar3 = lv_disp_get_layer_top(uVar3);
  lv_obj_invalidate(uVar3);
  if (iVar1 == 0) {
    if (piVar2 == (int *)0x0) goto LAB_00433e38;
  }
  else {
    if (piVar2 == (int *)0x0) {
      hcplayer_change_rotate_mirror_type(iVar1,local_1c,local_20);
      goto LAB_00433e38;
    }
    FUN_00425500(piVar2,local_1c);
  }
  if (*piVar2 == 1) {
    FUN_00414ee4(local_1c,local_20);
  }
LAB_00433e38:
  printf("rotate = %u mode =%d h_flip =%d\n",local_1c,param_1,local_20);
  FUN_00428454(0x13);
  return 0;
}



/* ===== FUNCTION FUN_00433eec @ 00433eec ===== */

void FUN_00433eec(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  piVar1 = (int *)FUN_0041d32c();
  if (piVar1 != (int *)0x0) {
    uVar2 = lv_disp_get_default();
    uVar2 = lv_disp_get_layer_top(uVar2);
    lv_obj_invalidate(uVar2);
    uVar2 = lv_disp_get_default();
    iVar3 = lv_disp_get_scr_act(uVar2);
    if (DAT_012167d8 == iVar3) {
      if (*piVar1 == 2) {
        FUN_0040f698();
        return;
      }
      if (*piVar1 == 1) {
        uVar2 = FUN_004250c0(piVar1);
        FUN_0040f698();
        uVar4 = FUN_0041d32c();
        FUN_00424954(uVar4,uVar2);
        return;
      }
    }
    else {
      uVar2 = lv_disp_get_default();
      iVar3 = lv_disp_get_scr_act(uVar2);
      if ((DAT_0121720c == iVar3) && (*piVar1 == 2)) {
        iVar5 = piVar1[3];
        iVar3 = piVar1[1];
        piVar1[5] = piVar1[5] & 0xfffffffb;
        FUN_00425c80(piVar1);
        FUN_00426380(piVar1,piVar1 + 0x10,iVar5,1,1);
        piVar1[5] = piVar1[5] & 4;
        if (iVar3 == 2) {
          FUN_004247bc(piVar1);
          return;
        }
      }
    }
  }
  return;
}



/* ===== FUNCTION FUN_00434108 @ 00434108 ===== */

void FUN_00434108(void)

{
  if (DAT_0121d1e4 != (void *)0x0) {
    free(DAT_0121d1e4);
    DAT_0121d1e4 = (void *)0x0;
  }
  return;
}



/* ===== FUNCTION FUN_00434140 @ 00434140 ===== */

void FUN_00434140(void)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  FILE *__stream;
  size_t __nmemb;
  void *pvVar4;
  char acStack_2a0 [512];
  undefined4 local_a0;
  undefined4 local_9c;
  undefined4 local_98;
  undefined4 local_94;
  undefined4 local_90;
  undefined4 local_8c;
  undefined4 local_88;
  char *local_84;
  void *local_80;
  size_t local_7c;
  undefined4 *local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  char *local_68;
  void *local_64;
  size_t local_60;
  undefined4 *local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  char *local_4c;
  void *local_48;
  size_t local_44;
  undefined4 *local_40;
  undefined4 local_3c;
  char *local_30;
  void *local_2c;
  size_t local_28;
  undefined4 *local_24;
  undefined4 local_20;
  
  piVar1 = (int *)FUN_0042b4b4();
  uVar2 = FUN_0042e3a4(*piVar1 + -1);
  iVar3 = FUN_00428454(0x11);
  if (iVar3 == 0x12) {
LAB_004344c8:
    sprintf(acStack_2a0,"%s/cubegm/Tahoma.ttf",uVar2);
  }
  else if (iVar3 < 0x13) {
    if (iVar3 == 4) {
      sprintf(acStack_2a0,"%s/cubegm/Arial_kr.ttf",uVar2);
    }
    else {
LAB_00434510:
      sprintf(acStack_2a0,"%s/cubegm/font.ttf",uVar2);
    }
  }
  else {
    if (iVar3 == 0x14) goto LAB_004344c8;
    if (iVar3 != 0x18) goto LAB_00434510;
    sprintf(acStack_2a0,"%s/cubegm/Arial_en.ttf",uVar2);
  }
  printf("mount_path:%s\n",acStack_2a0);
  lv_freetype_init(1,1,0);
  __stream = fopen(acStack_2a0,"rb");
  if (__stream == (FILE *)0x0) {
    puts("open font file failed!");
  }
  else {
    fseek(__stream,0,2);
    __nmemb = ftell(__stream);
    fseek(__stream,0,0);
    DAT_0121d1e4 = calloc(__nmemb,1);
    if (DAT_0121d1e4 == (void *)0x0) {
      printf("%s font_buf calloc error\n","create_freetype_font_mem");
      fclose(__stream);
    }
    else {
      DAT_0121d1e0 = fread(DAT_0121d1e4,1,__nmemb,__stream);
      fclose(__stream);
      pvVar4 = DAT_0121d1e4;
      if (DAT_0121d1e4 != (void *)0x0) goto LAB_0043425c;
    }
  }
  puts("warning: freetype font mem is NULL !!!");
  pvVar4 = (void *)0x0;
LAB_0043425c:
  local_7c = DAT_0121d1e0;
  local_78 = (undefined4 *)0x0;
  local_74 = 0xc;
  local_84 = acStack_2a0;
  local_80 = pvVar4;
  iVar3 = lv_ft_font_init(&local_84);
  if (iVar3 == 0) {
    puts("create freetype font failed !");
  }
  DAT_0121ed8c = local_78[5];
  DAT_0121ed88 = local_78[4];
  DAT_0121ed84 = local_78[3];
  DAT_0121ed80 = local_78[2];
  DAT_0121ed7c = local_78[1];
  DAT_0121ed78 = *local_78;
  DAT_0121ed90 = local_78[6];
  local_60 = DAT_0121d1e0;
  local_5c = (undefined4 *)0x0;
  local_58 = 0x12;
  local_a0 = DAT_0121ed78;
  local_9c = DAT_0121ed7c;
  local_98 = DAT_0121ed80;
  local_94 = DAT_0121ed84;
  local_90 = DAT_0121ed88;
  local_8c = DAT_0121ed8c;
  local_88 = DAT_0121ed90;
  local_68 = acStack_2a0;
  local_64 = pvVar4;
  iVar3 = lv_ft_font_init(&local_68);
  if (iVar3 == 0) {
    puts("create freetype font failed !");
  }
  DAT_0121edc4 = local_5c[5];
  DAT_0121edc0 = local_5c[4];
  DAT_0121edbc = local_5c[3];
  DAT_0121edb8 = local_5c[2];
  DAT_0121edb4 = local_5c[1];
  DAT_0121edb0 = (char *)*local_5c;
  DAT_0121edc8 = local_5c[6];
  local_44 = DAT_0121d1e0;
  local_40 = (undefined4 *)0x0;
  local_3c = 0x18;
  local_84 = DAT_0121edb0;
  local_80 = (void *)DAT_0121edb4;
  local_7c = DAT_0121edb8;
  local_78 = (undefined4 *)DAT_0121edbc;
  local_74 = DAT_0121edc0;
  local_70 = DAT_0121edc4;
  local_6c = DAT_0121edc8;
  local_4c = acStack_2a0;
  local_48 = pvVar4;
  iVar3 = lv_ft_font_init(&local_4c);
  if (iVar3 == 0) {
    puts("create freetype font failed !");
  }
  DAT_0121ed5c = (char *)*local_40;
  DAT_0121ed60 = local_40[1];
  DAT_0121ed64 = local_40[2];
  DAT_0121ed6c = local_40[4];
  DAT_0121ed68 = local_40[3];
  DAT_0121ed70 = local_40[5];
  DAT_0121ed74 = local_40[6];
  local_28 = DAT_0121d1e0;
  local_24 = (undefined4 *)0x0;
  local_20 = 0x1e;
  local_68 = DAT_0121ed5c;
  local_64 = (void *)DAT_0121ed60;
  local_60 = DAT_0121ed64;
  local_5c = (undefined4 *)DAT_0121ed68;
  local_58 = DAT_0121ed6c;
  local_54 = DAT_0121ed70;
  local_50 = DAT_0121ed74;
  local_30 = acStack_2a0;
  local_2c = pvVar4;
  iVar3 = lv_ft_font_init(&local_30);
  if (iVar3 == 0) {
    puts("create freetype font failed !");
  }
  DAT_0121ed98 = local_24[1];
  DAT_0121ed9c = local_24[2];
  DAT_0121eda0 = local_24[3];
  DAT_0121eda4 = local_24[4];
  DAT_0121eda8 = local_24[5];
  DAT_0121edac = local_24[6];
  DAT_0121ed94 = *local_24;
  return;
}



/* ===== FUNCTION FUN_00434580 @ 00434580 ===== */

undefined4 FUN_00434580(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  DIR *__dirp;
  int *piVar3;
  size_t sVar4;
  undefined4 local_98;
  char acStack_58 [64];
  
  if (param_1 == -1) {
    return 0xffffffff;
  }
  if (DAT_0121d1f0 == (int *)0x0) {
LAB_004345f0:
    sprintf(acStack_58,"/sys/class/gpio/gpio%d",param_1);
    __dirp = opendir(acStack_58);
    if (__dirp == (DIR *)0x0) {
      iVar1 = open("/sys/class/gpio/export",1);
      if (iVar1 < 0) {
        printf("%s(), line:%d. open export fail!, fd:%d\n","gpio_open",0x4b,iVar1);
        return 0xffffffff;
      }
      sprintf(acStack_58,"%d",param_1);
      sVar4 = strlen(acStack_58);
      write(iVar1,acStack_58,sVar4);
      close(iVar1);
      FUN_0042c930(10);
    }
    else {
      closedir(__dirp);
    }
    sprintf((char *)&local_98,"/sys/class/gpio/gpio%d/direction",param_1);
    iVar1 = open((char *)&local_98,1);
    if (iVar1 < 0) {
      printf("%s(), line:%d. open %s fail!\n","gpio_dir_set",0x30,&local_98);
    }
    else {
      if (param_2 == 0) {
        local_98._2_2_ = (ushort)((uint)local_98 >> 0x10) & 0xff00;
        local_98 = (undefined *)CONCAT22(local_98._2_2_,0x6e69);
      }
      else {
        local_98 = &DAT_0074756f;
      }
      sVar4 = strlen((char *)&local_98);
      write(iVar1,&local_98,sVar4);
      close(iVar1);
    }
    piVar3 = calloc(8,1);
    piVar2 = DAT_0121d1f0;
    *piVar3 = param_1;
    piVar3[1] = param_2;
    DAT_0121d1f0 = (int *)glist_append(piVar2,piVar3);
  }
  else {
    piVar2 = DAT_0121d1f0;
    if (param_1 == *(int *)*DAT_0121d1f0) {
      iVar1 = ((int *)*DAT_0121d1f0)[1];
    }
    else {
      do {
        piVar2 = (int *)piVar2[1];
        if (piVar2 == (int *)0x0) goto LAB_004345f0;
      } while (param_1 != *(int *)*piVar2);
      iVar1 = ((int *)*piVar2)[1];
    }
    if (iVar1 == param_2) {
      return 0;
    }
    sprintf((char *)&local_98,"/sys/class/gpio/gpio%d/direction",param_1);
    iVar1 = open((char *)&local_98,1);
    if (iVar1 < 0) {
      printf("%s(), line:%d. open %s fail!\n","gpio_dir_set",0x30,&local_98);
    }
    else {
      if (param_2 == 0) {
        local_98 = (undefined *)((uint)local_98._3_1_ << 0x18);
        local_98 = (undefined *)CONCAT22(local_98._2_2_,0x6e69);
      }
      else {
        local_98 = &DAT_0074756f;
      }
      sVar4 = strlen((char *)&local_98);
      write(iVar1,&local_98,sVar4);
      close(iVar1);
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00434844 @ 00434844 ===== */

void FUN_00434844(int param_1,int param_2)

{
  undefined4 *puVar1;
  int __fd;
  size_t __n;
  char local_50 [68];
  
  if ((param_1 != -1) && (puVar1 = DAT_0121d1f0, DAT_0121d1f0 != (undefined4 *)0x0)) {
    while (param_1 != *(int *)*puVar1) {
      puVar1 = (undefined4 *)puVar1[1];
      if (puVar1 == (undefined4 *)0x0) {
        return;
      }
    }
    sprintf(local_50,"/sys/class/gpio/gpio%d/value");
    __fd = open(local_50,1);
    if (__fd < 0) {
      printf("%s(), line:%d. open %s fail!\n","gpio_write",0x9f,local_50);
    }
    else {
      if (param_2 == 0) {
        local_50[0] = '0';
        local_50[1] = '\0';
      }
      else {
        local_50[0] = '1';
        local_50[1] = '\0';
      }
      __n = strlen(local_50);
      write(__fd,local_50,__n);
      close(__fd);
    }
  }
  return;
}



/* ===== FUNCTION FUN_00434e6c @ 00434e6c ===== */

int * FUN_00434e6c(int *param_1,int param_2)

{
  int *piVar1;
  
  if ((param_1 == (int *)0x0) || (param_2 == 0)) {
    piVar1 = (int *)0x0;
  }
  else {
    pthread_mutex_lock((pthread_mutex_t *)(param_1 + 3));
    piVar1 = (int *)*param_1;
    if (param_1 == piVar1) {
      piVar1 = (int *)0x0;
    }
    else {
      do {
        if (param_2 == piVar1[4]) {
          piVar1 = piVar1 + 2;
          goto LAB_00434ecc;
        }
        piVar1 = (int *)*piVar1;
      } while (param_1 != piVar1);
      piVar1 = (int *)0x0;
    }
LAB_00434ecc:
    pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 3));
  }
  return piVar1;
}



/* ===== FUNCTION FUN_00435038 @ 00435038 ===== */

int FUN_00435038(int *param_1,int param_2,int param_3)

{
  char cVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  int iVar7;
  
  if ((param_1 == (int *)0x0) || (param_2 == 0)) {
    return 0;
  }
  if (3 < param_3 - 0xbU) {
    printf("%s(), not support the key:%d\n","obj_mgr_draw_obj_by_key");
    return 0;
  }
  pthread_mutex_lock((pthread_mutex_t *)(param_1 + 3));
  piVar4 = (int *)*param_1;
  if (param_1 == piVar4) {
LAB_00435108:
    iVar7 = 0;
  }
  else {
    piVar6 = piVar4;
    if (param_3 == 0xe) {
      do {
        if (param_2 == piVar6[4]) {
          cVar1 = (char)piVar6[6];
          iVar7 = 0;
          if (cVar1 != '\0') {
            cVar2 = *(char *)((int)piVar6 + 0x1a);
            goto joined_r0x00435318;
          }
          break;
        }
        piVar6 = (int *)*piVar6;
        iVar7 = 0;
      } while (param_1 != piVar6);
    }
    else if (param_3 == 0xd) {
      do {
        if (param_2 == piVar6[4]) {
          cVar1 = (char)piVar6[6];
          iVar7 = 0;
          if (cVar1 != '\0') {
            cVar2 = *(char *)((int)piVar6 + 0x19);
            goto joined_r0x00435318;
          }
          break;
        }
        piVar6 = (int *)*piVar6;
        iVar7 = 0;
      } while (param_1 != piVar6);
    }
    else {
      if (param_3 != 0xc) {
LAB_004350f0:
        if (piVar6[4] != param_2) goto code_r0x004350fc;
        cVar1 = (char)piVar6[6];
        iVar7 = 0;
        if (cVar1 != '\0') {
          cVar2 = *(char *)((int)piVar6 + 0x1b);
joined_r0x00435318:
          iVar7 = 0;
          if (cVar2 == '\0') goto LAB_0043510c;
          goto LAB_00435158;
        }
        goto LAB_0043510c;
      }
      while (param_2 != piVar6[4]) {
        piVar6 = (int *)*piVar6;
        iVar7 = 0;
        if (param_1 == piVar6) goto LAB_0043510c;
      }
      cVar1 = (char)piVar6[6];
      if ((cVar1 == '\0') || (cVar2 = (char)piVar6[7], cVar2 == '\0')) goto LAB_00435108;
LAB_00435158:
      iVar7 = 0;
      if (cVar2 != cVar1) {
        do {
          if ((char)piVar4[6] == cVar2) {
            iVar7 = piVar4[4];
            if (piVar6[8] == 0) {
LAB_004351a0:
              piVar5 = (int *)piVar6[2];
            }
            else if (piVar6[5] == 0) {
              lv_img_set_src(param_2);
              piVar5 = (int *)piVar6[2];
            }
            else {
              if (piVar6[5] != 1) goto LAB_004351a0;
              lv_obj_add_style(param_2,piVar6[8],0);
              piVar5 = (int *)piVar6[2];
            }
            if (piVar6 + 2 != piVar5) goto LAB_004351cc;
            iVar3 = piVar4[4];
            goto LAB_00435204;
          }
          piVar4 = (int *)*piVar4;
          iVar7 = 0;
        } while (param_1 != piVar4);
      }
    }
  }
  goto LAB_0043510c;
code_r0x004350fc:
  piVar6 = (int *)*piVar6;
  if (param_1 == piVar6) goto LAB_00435108;
  goto LAB_004350f0;
LAB_004351cc:
  do {
    if ((piVar5[2] == 0) || (piVar5[4] == 0)) {
LAB_004351c0:
      piVar5 = (int *)*piVar5;
    }
    else if (piVar5[3] == 0) {
      lv_img_set_src();
      piVar5 = (int *)*piVar5;
    }
    else {
      if (piVar5[3] != 1) goto LAB_004351c0;
      lv_obj_add_style(piVar5[2],piVar5[4],0);
      piVar5 = (int *)*piVar5;
    }
  } while (piVar6 + 2 != piVar5);
  iVar3 = piVar4[4];
LAB_00435204:
  if ((iVar3 == 0) || (piVar4[9] == 0)) {
LAB_00435228:
    piVar6 = (int *)piVar4[2];
  }
  else if (piVar4[5] == 0) {
    lv_img_set_src();
    piVar6 = (int *)piVar4[2];
  }
  else {
    if (piVar4[5] != 1) goto LAB_00435228;
    lv_obj_add_style(iVar3,piVar4[9],0);
    piVar6 = (int *)piVar4[2];
  }
  if (piVar4 + 2 != piVar6) {
    iVar3 = piVar6[2];
    do {
      if ((iVar3 == 0) || (piVar6[5] == 0)) {
LAB_00435244:
        piVar6 = (int *)*piVar6;
      }
      else if (piVar6[3] == 0) {
        lv_img_set_src();
        piVar6 = (int *)*piVar6;
      }
      else {
        if (piVar6[3] != 1) goto LAB_00435244;
        lv_obj_add_style(iVar3,piVar6[5],0);
        piVar6 = (int *)*piVar6;
      }
      if (piVar4 + 2 == piVar6) break;
      iVar3 = piVar6[2];
    } while( true );
  }
LAB_0043510c:
  pthread_mutex_unlock((pthread_mutex_t *)(param_1 + 3));
  return iVar7;
}



/* ===== FUNCTION FUN_004356a0 @ 004356a0 ===== */

void FUN_004356a0(byte *param_1,ushort *param_2,int param_3)

{
  ushort uVar1;
  uint uVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  ushort *puVar6;
  
  uVar5 = 0;
  while (param_3 = param_3 + -1, param_3 != -1) {
    puVar6 = param_2 + 1;
    uVar1 = *param_2;
    uVar4 = (uint)uVar1;
    if (uVar5 == 0) {
      bVar3 = (byte)uVar1;
      uVar2 = 0;
      uVar5 = uVar2;
      if (uVar4 < 0x80) {
        *param_1 = bVar3;
        param_1 = param_1 + 1;
        param_2 = puVar6;
      }
      else if (uVar4 < 0x800) {
        *param_1 = (byte)(uVar1 >> 6) | 0xc0;
        param_1[1] = bVar3 & 0x3f | 0x80;
        param_1 = param_1 + 2;
        param_2 = puVar6;
      }
      else {
        param_2 = puVar6;
        uVar5 = uVar4;
        if (0x3ff < uVar4 - 0xd800) {
          if (uVar4 - 0xdc00 < 0x400) {
            *param_1 = 0x3f;
            param_1 = param_1 + 1;
            uVar5 = uVar2;
          }
          else {
            *param_1 = (byte)(uVar1 >> 0xc) | 0xe0;
            param_1[1] = (byte)((uVar4 << 0x14) >> 0x1a) | 0x80;
            param_1[2] = bVar3 & 0x3f | 0x80;
            param_1 = param_1 + 3;
            uVar5 = uVar2;
          }
        }
      }
    }
    else if (uVar4 - 0xdc00 < 0x400) {
      uVar5 = (uVar5 - 0xd800) * 0x400 + uVar4 + 0x2400;
      *param_1 = (byte)(uVar5 >> 0x12) | 0xf0;
      param_1[1] = (byte)(uVar5 * 0x4000 >> 0x1a) | 0x80;
      param_1[2] = (byte)(uVar5 * 0x100000 >> 0x1a) | 0x80;
      param_1[3] = (byte)uVar5 & 0x3f | 0x80;
      param_1 = param_1 + 4;
      param_2 = puVar6;
      uVar5 = 0;
    }
    else {
      *param_1 = 0x3f;
      param_1 = param_1 + 1;
      uVar5 = 0;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0043591c @ 0043591c ===== */

void FUN_0043591c(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  lv_event_get_code();
  iVar1 = lv_event_get_target(param_1);
  uVar3 = *(uint *)(iVar1 + 0x10);
  if ((uVar3 << 0x10) >> 0x14 != 0) {
    uVar2 = FUN_004386cc();
    lv_label_set_text(iVar1,uVar2);
    FUN_00428454(0x11);
    lv_obj_set_style_text_font(iVar1,(&PTR_PTR_012146f0)[uVar3 & 0xf],0);
    return;
  }
  lv_label_set_text(iVar1," ");
  return;
}



/* ===== FUNCTION FUN_004359b8 @ 004359b8 ===== */

undefined4 FUN_004359b8(undefined4 param_1,undefined4 param_2,short param_3,short param_4)

{
  undefined4 uVar1;
  
  uVar1 = lv_img_create();
  lv_img_set_src(uVar1,param_2);
  lv_obj_set_pos(uVar1,(int)param_3,(int)param_4);
  return uVar1;
}



/* ===== FUNCTION FUN_00435a18 @ 00435a18 ===== */

undefined4 FUN_00435a18(undefined4 param_1,short param_2,short param_3,short param_4,int param_5)

{
  undefined4 uVar1;
  
  uVar1 = lv_label_create();
  lv_obj_set_pos(uVar1,(int)param_2,(int)param_3);
  lv_obj_set_width(uVar1,(int)param_4);
  lv_obj_set_height(uVar1,0x3c);
  if (param_5 != 0) {
    lv_label_set_text(uVar1,param_5);
  }
  return uVar1;
}



/* ===== FUNCTION FUN_00435ac8 @ 00435ac8 ===== */

void FUN_00435ac8(undefined4 param_1,int param_2)

{
  if (param_2 == 0) {
    lv_obj_set_style_bg_color(param_1,0xff303841);
  }
  else {
    lv_obj_set_style_bg_opa(param_1,0,0);
  }
  lv_obj_set_pos(param_1,0,0);
  lv_obj_set_size(param_1,0x2064,0x2064);
  lv_obj_set_style_border_opa(param_1,0,0);
  return;
}



/* ===== FUNCTION FUN_00435d2c @ 00435d2c ===== */

void FUN_00435d2c(short *param_1,int param_2,short param_3,int param_4)

{
  short sVar1;
  short sVar2;
  
  sVar1 = (short)param_2;
  sVar2 = (short)param_4;
  if (param_4 < param_2) {
    *param_1 = 0;
    param_1[2] = sVar2;
    param_1[3] = sVar2;
    param_1[1] = sVar1;
    param_1[5] = param_3;
    return;
  }
  param_1[2] = sVar2;
  param_1[3] = sVar2;
  param_1[1] = sVar1;
  param_1[5] = param_3;
  *param_1 = (sVar2 + 1) - sVar1;
  return;
}



/* ===== FUNCTION FUN_0043645c @ 0043645c ===== */

bool FUN_0043645c(ushort *param_1,int param_2,undefined4 param_3,ushort *param_4)

{
  uint uVar1;
  ushort uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  ushort uVar12;
  ushort uVar13;
  uint uVar14;
  
  if (param_2 == 0x1e) {
    uVar2 = -param_1[1];
LAB_004365c4:
    uVar3 = (uint)(short)uVar2;
    uVar6 = (uint)param_1[5];
    if ((uVar6 == 0) || (uVar3 == 0)) {
LAB_004365d8:
      *param_4 = 0;
      return false;
    }
LAB_00436488:
    uVar8 = uVar3 & 0xffff;
  }
  else {
    if (param_2 == 0x1f) {
      uVar2 = param_1[1];
      goto LAB_004365c4;
    }
    if (param_2 != 0xe) {
      uVar6 = (uint)param_1[5];
      if (uVar6 == 0) goto LAB_004365d8;
      uVar3 = 0xffffffff;
      goto LAB_00436488;
    }
    uVar6 = (uint)param_1[5];
    if (uVar6 == 0) goto LAB_004365d8;
    uVar8 = 1;
    uVar3 = 1;
  }
  uVar2 = *param_1;
  uVar11 = (uint)uVar2;
  uVar4 = (uint)param_1[3];
  if (param_1[3] < uVar11) {
    param_1[2] = uVar2;
    param_1[3] = uVar2;
    uVar4 = uVar11;
  }
  if (uVar6 < uVar4) {
    uVar7 = (uint)param_1[1];
    uVar1 = 0;
    *param_1 = 0;
    uVar12 = 0;
    param_1[2] = 0;
    uVar14 = 0;
    param_1[3] = 0;
    uVar4 = 0;
    if (uVar3 == uVar7) goto LAB_00436614;
LAB_00436500:
    uVar10 = uVar4 + uVar8 & 0xffff;
    iVar9 = (int)(short)(uVar4 + uVar8);
    uVar3 = (uint)(-uVar7 == uVar3);
    if (iVar9 < (int)uVar1) goto LAB_00436630;
LAB_00436524:
    uVar2 = (ushort)uVar10;
    if ((int)(uVar7 + uVar1) <= iVar9) goto LAB_00436630;
    if (uVar1 == uVar14) goto LAB_00436644;
LAB_0043653c:
    uVar2 = (ushort)uVar10;
    if ((int)uVar1 < 0) {
      printf("%d: top < 0\n",0x10a);
      if (uVar14 != 0) {
        printf("%d: shift_top > 0\n",0x10d);
        iVar5 = iVar9;
        if (uVar3 != 0) {
          uVar10 = 0;
          iVar9 = (int)(short)uVar12;
          iVar5 = 0;
          uVar2 = uVar12;
        }
        uVar12 = uVar2;
        printf("%d: page moving=%ld: top=%d, point = %d\n",0x120,uVar3,iVar5,iVar9);
        *param_1 = (ushort)uVar10;
        goto LAB_00436564;
      }
      printf("%d: shift_top == 0\n",0x126);
      uVar2 = param_1[5];
      uVar6 = (uint)(short)(uVar2 - param_1[1]);
      if ((int)uVar6 < 0) {
        uVar6 = 0;
      }
      uVar10 = uVar6 & 0xffff;
      if (uVar3 == 0) {
        uVar13 = uVar2 - 1;
      }
      else {
        uVar13 = (short)uVar6 + uVar12;
      }
      uVar12 = uVar2 - 1;
      if ((int)(short)uVar13 < (int)(uint)uVar2) {
LAB_00436768:
        uVar12 = uVar13;
      }
    }
    else if (iVar9 < (int)uVar6) {
      uVar10 = uVar1 & 0xffff;
      puts("?");
      uVar12 = uVar2;
    }
    else {
LAB_00436668:
      printf("%d: point >= list_ctrl->count\n",0x14c);
      if ((uint)param_1[5] <= param_1[1] + uVar14) {
        if (uVar3 == 0) {
          uVar12 = 0;
        }
        uVar10 = 0;
        printf("%d: Need move to first page\n",0x171);
        *param_1 = 0;
        goto LAB_00436564;
      }
      printf("%d: shift_top + list_ctrl->depth < list_ctrl->count\n",0x152);
      uVar2 = param_1[5];
      uVar6 = (uint)(short)(uVar2 - param_1[1]);
      if ((int)uVar6 < 0) {
        uVar6 = 0;
      }
      uVar10 = uVar6 & 0xffff;
      if (uVar3 == 0) {
        uVar13 = uVar2 - 1;
      }
      else {
        uVar13 = (short)uVar6 + uVar12;
      }
      if ((int)(short)uVar13 < (int)(uint)uVar2) goto LAB_00436768;
      uVar12 = uVar2 - 1;
    }
  }
  else {
    uVar7 = (uint)param_1[1];
    uVar12 = (short)uVar4 - uVar2;
    uVar1 = (uint)(short)uVar2;
    uVar14 = uVar11;
    if (uVar3 != uVar7) goto LAB_00436500;
LAB_00436614:
    uVar10 = uVar4 + uVar8 & 0xffff;
    iVar9 = (int)(short)(uVar4 + uVar8);
    uVar3 = 1;
    if ((int)uVar1 <= iVar9) goto LAB_00436524;
LAB_00436630:
    uVar2 = (ushort)uVar10;
    uVar1 = (uint)(short)((short)uVar14 + (short)uVar8);
    if (uVar1 != uVar14) goto LAB_0043653c;
LAB_00436644:
    if ((int)uVar6 <= iVar9) goto LAB_00436668;
    uVar10 = uVar1 & 0xffff;
    uVar12 = uVar2;
  }
  *param_1 = (ushort)uVar10;
LAB_00436564:
  param_1[2] = uVar12;
  param_1[3] = uVar12;
  *param_4 = uVar12;
  return uVar11 != uVar10;
}



/* ===== FUNCTION FUN_00436b04 @ 00436b04 ===== */

void FUN_00436b04(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  char *__format;
  undefined4 uVar2;
  char acStack_128 [256];
  undefined1 *local_28;
  
  if ((DAT_0121d27c == 0) || (iVar1 = lv_obj_is_valid(), iVar1 == 0)) {
    uVar2 = lv_disp_get_default();
    uVar2 = lv_disp_get_scr_act(uVar2);
    DAT_0121d27c = lv_obj_create(uVar2);
    lv_obj_set_size(DAT_0121d27c,0x203c,0x2023);
    lv_obj_set_x(DAT_0121d27c,0);
    lv_obj_set_y(DAT_0121d27c,0xffffff9c);
    lv_obj_set_align(DAT_0121d27c,9);
    lv_obj_clear_flag(DAT_0121d27c,0x10);
    lv_obj_set_style_radius(DAT_0121d27c,0,0);
    lv_obj_set_style_bg_color(DAT_0121d27c,0xff4d72e0,0);
    lv_obj_set_style_bg_opa(DAT_0121d27c,0xff,0);
    DAT_0121d254 = lv_img_create(DAT_0121d27c);
    lv_obj_set_width(DAT_0121d254,0x27d1);
    lv_obj_set_height(DAT_0121d254,0x27d1);
    lv_obj_set_x(DAT_0121d254,0);
    lv_obj_set_y(DAT_0121d254,0);
    lv_img_set_src(DAT_0121d254,&DAT_00b35c00);
    lv_obj_set_align(DAT_0121d254,9);
    lv_obj_add_flag(DAT_0121d254,0x10000);
    lv_obj_clear_flag(DAT_0121d254,0x10);
    DAT_0121d250 = lv_label_create(DAT_0121d27c);
    lv_obj_set_width(DAT_0121d250,0x27d1);
    lv_obj_set_height(DAT_0121d250,0x27d1);
    lv_obj_set_x(DAT_0121d250,0);
    lv_obj_set_y(DAT_0121d250,0);
    lv_obj_set_align(DAT_0121d250,5);
    lv_label_set_long_mode(DAT_0121d250,3);
    lv_obj_set_style_text_color(DAT_0121d250,0xffffffff,0);
    lv_obj_set_style_text_opa(DAT_0121d250,0xff,0);
    lv_obj_set_style_text_font(DAT_0121d250,lv_font_montserrat_28,0);
    if (param_2 - 1U < 0xfffffffe) {
      DAT_0121d278 = lv_timer_create(&LAB_00435804,param_2,0);
    }
  }
  if (DAT_0121d278 != 0) {
    lv_timer_reset();
    lv_timer_set_period(DAT_0121d278,param_2);
  }
  __format = (char *)FUN_004386cc(param_1);
  memset(acStack_128,0,0x100);
  local_28 = &stack0x00000010;
  vsnprintf(acStack_128,0x100,__format,local_28);
  lv_label_set_text(DAT_0121d250,acStack_128);
  uVar2 = DAT_0121d250;
  FUN_00428454(0x11);
  lv_obj_set_style_text_font(uVar2,PTR_SiYuanHeiTi_Light_3000_28_1b_012146f4,0);
  if (param_3 == 0) {
    DAT_0121d274 = 0;
    DAT_0121d270 = 0;
    return;
  }
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d258);
  DAT_0121d270 = param_4;
  DAT_0121d274 = param_3;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
  return;
}



/* ===== FUNCTION FUN_00436b1c @ 00436b1c ===== */

void FUN_00436b1c(void)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d258);
  if (DAT_0121d278 != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_0121d278);
  }
  DAT_0121d278 = 0;
  if ((DAT_0121d27c != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
    lv_obj_del(DAT_0121d27c);
    DAT_0121d27c = 0;
    DAT_0121d274 = 0;
    DAT_0121d270 = 0;
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
    return;
  }
  DAT_0121d27c = 0;
  DAT_0121d274 = 0;
  DAT_0121d270 = 0;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
  return;
}



/* ===== FUNCTION FUN_00436be4 @ 00436be4 ===== */

void FUN_00436be4(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  if (DAT_0121d27c != 0) {
    iVar1 = lv_obj_is_valid(DAT_0121d27c);
    if (iVar1 != 0) {
      lv_obj_align(DAT_0121d27c,param_1,param_2,param_3);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00436c88 @ 00436c88 ===== */

void FUN_00436c88(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  iVar1 = lv_obj_is_valid(DAT_0121d27c);
  if (iVar1 == 0) {
    if ((DAT_0121d27c == 0) || (iVar1 = lv_obj_is_valid(), iVar1 == 0)) {
      uVar2 = lv_disp_get_default();
      uVar2 = lv_disp_get_layer_top(uVar2);
      DAT_0121d27c = lv_obj_create(uVar2);
      lv_obj_set_size(DAT_0121d27c,0x2028,0x2023);
      lv_obj_set_x(DAT_0121d27c,0);
      lv_obj_set_y(DAT_0121d27c,0xffffff9c);
      lv_obj_set_align(DAT_0121d27c,9);
      lv_obj_clear_flag(DAT_0121d27c,0x10);
      lv_obj_set_style_radius(DAT_0121d27c,0,0);
      lv_obj_set_style_bg_color(DAT_0121d27c,0xff4d72e0,0);
      lv_obj_set_style_bg_opa(DAT_0121d27c,0xff,0);
      DAT_0121d254 = lv_img_create(DAT_0121d27c);
      lv_obj_set_width(DAT_0121d254,0x27d1);
      lv_obj_set_height(DAT_0121d254,0x27d1);
      lv_obj_set_x(DAT_0121d254,0);
      lv_obj_set_y(DAT_0121d254,0);
      lv_img_set_src(DAT_0121d254,&DAT_00b35c00);
      lv_obj_set_align(DAT_0121d254,9);
      lv_obj_add_flag(DAT_0121d254,0x10000);
      lv_obj_clear_flag(DAT_0121d254,0x10);
      DAT_0121d250 = lv_label_create(DAT_0121d27c);
      lv_obj_set_width(DAT_0121d250,0x27d1);
      lv_obj_set_height(DAT_0121d250,0x27d1);
      lv_obj_set_x(DAT_0121d250,0);
      lv_obj_set_y(DAT_0121d250,0);
      lv_obj_set_align(DAT_0121d250,5);
      lv_label_set_long_mode(DAT_0121d250,3);
      lv_obj_set_style_text_color(DAT_0121d250,0xffffffff,0);
      lv_obj_set_style_text_opa(DAT_0121d250,0xff,0);
      lv_obj_set_style_text_font(DAT_0121d250,lv_font_montserrat_28,0);
      if (param_2 - 1U < 0xfffffffe) {
        DAT_0121d278 = lv_timer_create(&LAB_00435804,param_2,0);
      }
    }
    if (DAT_0121d278 != 0) {
      lv_timer_reset();
      lv_timer_set_period(DAT_0121d278,param_2);
    }
    uVar2 = DAT_0121d250;
    uVar3 = FUN_004386cc(param_1);
    lv_label_set_text(uVar2,uVar3);
    uVar2 = DAT_0121d250;
    FUN_00428454(0x11);
    lv_obj_set_style_text_font(uVar2,PTR_SiYuanHeiTi_Light_3000_28_1b_012146f4,0);
    if (param_3 != 0) {
      pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d258);
      DAT_0121d270 = param_4;
      DAT_0121d274 = param_3;
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
      return;
    }
    DAT_0121d274 = 0;
    DAT_0121d270 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_00436f84 @ 00436f84 ===== */

void FUN_00436f84(void)

{
  int iVar1;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d258);
  if (DAT_0121d278 != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_0121d278);
  }
  DAT_0121d278 = 0;
  if ((DAT_0121d27c != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
    lv_obj_del(DAT_0121d27c);
    DAT_0121d27c = 0;
    DAT_0121d274 = 0;
    DAT_0121d270 = 0;
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
    return;
  }
  DAT_0121d27c = 0;
  DAT_0121d274 = 0;
  DAT_0121d270 = 0;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
  return;
}



/* ===== FUNCTION FUN_0043704c @ 0043704c ===== */

void FUN_0043704c(int param_1)

{
  undefined4 uVar1;
  
  if (DAT_0121d24c != 0) {
    lv_obj_del();
  }
  if (param_1 == 0) {
    uVar1 = lv_disp_get_default();
    uVar1 = lv_disp_get_scr_act(uVar1);
    DAT_0121d24c = lv_arc_create(uVar1);
  }
  else {
    DAT_0121d24c = lv_arc_create(param_1);
  }
  DAT_0121d200 = 0;
  lv_arc_set_rotation(DAT_0121d24c,0);
  lv_obj_set_size(DAT_0121d24c,0x50,0x50);
  lv_arc_set_bg_angles(DAT_0121d24c,0,0x168);
  lv_obj_remove_style(DAT_0121d24c,0,0x30000);
  lv_obj_set_style_arc_color(DAT_0121d24c,0xffd0d0d0,0);
  lv_anim_init(&DAT_0121d204);
  DAT_0121d224 = 0;
  DAT_0121d204 = DAT_0121d24c;
  DAT_0121d228 = 0;
  DAT_0121d230 = 5000;
  DAT_0121d240 = 0;
  DAT_0121d244 = 0xffff;
  DAT_0121d208 = &LAB_004358b4;
  DAT_0121d22c = 0x3c;
  lv_anim_start(&DAT_0121d204);
  lv_obj_align(DAT_0121d24c,9,0,0);
  DAT_0121d248 = lv_label_create(DAT_0121d24c);
  lv_obj_align(DAT_0121d248,9,0,0);
  lv_obj_set_style_text_color(DAT_0121d248,0xffffffff,0);
  lv_obj_set_style_text_font(DAT_0121d248,lv_font_montserrat_22,0);
  lv_label_set_text(DAT_0121d248,&PTR_DAT_00452b20);
  return;
}



/* ===== FUNCTION FUN_0043733c @ 0043733c ===== */

int FUN_0043733c(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_hor_res(uVar1);
  return (iVar2 * param_1) / 0x500;
}



/* ===== FUNCTION FUN_00437388 @ 00437388 ===== */

int FUN_00437388(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_ver_res(uVar1);
  return (iVar2 * param_1) / 0x2d0;
}



/* ===== FUNCTION FUN_004373d8 @ 004373d8 ===== */

undefined4 FUN_004373d8(byte *param_1,undefined4 *param_2)

{
  size_t sVar1;
  char *pcVar2;
  byte *pbVar3;
  undefined4 uVar4;
  byte bVar5;
  undefined4 uVar6;
  ushort *puVar7;
  undefined4 uVar8;
  byte *pbVar9;
  undefined4 uVar10;
  int iVar11;
  ushort *__s;
  size_t __n;
  char cStack_419;
  undefined2 local_418;
  ushort uStack_416;
  undefined4 local_414 [126];
  undefined1 uStack_219;
  undefined2 local_218;
  ushort uStack_216;
  undefined4 local_214 [127];
  
  __s = &local_218;
  memset(__s,0,0x200);
  memset(&local_418,0,0x200);
  sVar1 = strlen((char *)param_1);
  __n = sVar1 + 1;
  if (0x200 < (int)__n) {
    printf("%s,str too long,str size:%d\n","string_fmt_conv_to_utf8",__n);
    return 0xffffffff;
  }
  memcpy(__s,param_1,__n);
  bVar5 = *param_1;
  if (bVar5 == 0xef) {
    if (param_1[1] == 0xbb) goto LAB_004376e0;
  }
  else if (bVar5 == 0xff) {
    if (param_1[1] == 0xfe) {
      pcVar2 = (char *)&local_418;
      iVar11 = 1;
      puVar7 = __s;
      while( true ) {
        if ((((char)*puVar7 == '\0') && (*(char *)((int)puVar7 + 1) == '\0')) &&
           ((iVar11 - 1U & 1) == 0)) break;
        *pcVar2 = (char)*puVar7;
        puVar7 = (ushort *)((int)puVar7 + 1);
        iVar11 = iVar11 + 1;
        pcVar2 = pcVar2 + 1;
      }
      *(undefined1 *)((int)local_414 + iVar11 + -4) = 0;
      (&cStack_419)[iVar11] = '\0';
      memset(__s,0,0x200);
      FUN_004356a0(__s,&local_418,__n);
      do {
        uVar10 = *(undefined4 *)__s;
        uVar8 = *(undefined4 *)(__s + 2);
        uVar6 = *(undefined4 *)(__s + 4);
        uVar4 = *(undefined4 *)(__s + 6);
        __s = __s + 8;
        *param_2 = uVar10;
        param_2[1] = uVar8;
        param_2[2] = uVar6;
        param_2[3] = uVar4;
        param_2 = param_2 + 4;
      } while (__s != (ushort *)&stack0xffffffe8);
      return 0;
    }
  }
  else if ((bVar5 == 0xfe) && (puVar7 = __s, param_1[1] == 0xff)) {
    while (local_218 != 0) {
      *puVar7 = local_218 << 8 | local_218 >> 8;
      local_218 = puVar7[1];
      puVar7 = puVar7 + 1;
    }
    pcVar2 = (char *)&local_418;
    iVar11 = 1;
    puVar7 = __s;
    while( true ) {
      if ((((char)*puVar7 == '\0') && (*(char *)((int)puVar7 + 1) == '\0')) &&
         ((iVar11 - 1U & 1) == 0)) break;
      *pcVar2 = (char)*puVar7;
      puVar7 = (ushort *)((int)puVar7 + 1);
      iVar11 = iVar11 + 1;
      pcVar2 = pcVar2 + 1;
    }
    *(undefined1 *)((int)local_414 + iVar11 + -4) = 0;
    (&cStack_419)[iVar11] = '\0';
    memset(__s,0,0x200);
    FUN_004356a0(__s,&local_418,__n);
    do {
      uVar10 = *(undefined4 *)__s;
      uVar8 = *(undefined4 *)(__s + 2);
      uVar6 = *(undefined4 *)(__s + 4);
      uVar4 = *(undefined4 *)(__s + 6);
      __s = __s + 8;
      *param_2 = uVar10;
      param_2[1] = uVar8;
      param_2[2] = uVar6;
      param_2[3] = uVar4;
      param_2 = param_2 + 4;
    } while (__s != (ushort *)&stack0xffffffe8);
    return 0;
  }
  pbVar9 = param_1 + __n;
  iVar11 = 0;
  if (param_1 < pbVar9) {
    pbVar3 = param_1;
    do {
      if ((char)bVar5 < '\0') {
        if (bVar5 < 0xc0) {
          iVar11 = iVar11 + 1;
          if (3 < iVar11) goto LAB_004374d0;
          goto LAB_0043749c;
        }
        if (bVar5 < 0xe0) {
          if (param_1 + sVar1 <= pbVar3) break;
          bVar5 = pbVar3[1];
          pbVar3 = pbVar3 + 2;
        }
        else {
          if (0xef < bVar5) goto LAB_004374d0;
          if (pbVar9 + -2 <= pbVar3) break;
          if ((pbVar3[1] & 0xc0) != 0x80) goto LAB_004374d0;
          bVar5 = pbVar3[2];
          pbVar3 = pbVar3 + 3;
        }
        if ((bVar5 & 0xc0) != 0x80) {
LAB_004374d0:
          FUN_0041ae48(__s,__n,&local_418,0x200);
          puVar7 = &local_418;
          while (local_418 != 0) {
            *puVar7 = local_418 << 8 | local_418 >> 8;
            puVar7 = puVar7 + 1;
            local_418 = *puVar7;
          }
          pcVar2 = (char *)&local_418;
          iVar11 = 1;
          puVar7 = __s;
          while( true ) {
            if (((*pcVar2 == '\0') && (pcVar2[1] == '\0')) && ((iVar11 - 1U & 1) == 0)) break;
            *(char *)puVar7 = *pcVar2;
            pcVar2 = pcVar2 + 1;
            iVar11 = iVar11 + 1;
            puVar7 = (ushort *)((int)puVar7 + 1);
          }
          *(undefined1 *)((int)local_214 + iVar11 + -4) = 0;
          (&uStack_219)[iVar11] = 0;
          memset(&local_418,0,0x200);
          FUN_004356a0(&local_418,__s,__n);
          puVar7 = &local_418;
          do {
            uVar10 = *(undefined4 *)puVar7;
            uVar8 = *(undefined4 *)(puVar7 + 2);
            uVar6 = *(undefined4 *)(puVar7 + 4);
            uVar4 = *(undefined4 *)(puVar7 + 6);
            puVar7 = puVar7 + 8;
            *param_2 = uVar10;
            param_2[1] = uVar8;
            param_2[2] = uVar6;
            param_2[3] = uVar4;
            param_2 = param_2 + 4;
          } while (puVar7 != __s);
          return 0;
        }
      }
      else {
LAB_0043749c:
        pbVar3 = pbVar3 + 1;
      }
      if (pbVar9 <= pbVar3) break;
      bVar5 = *pbVar3;
    } while( true );
  }
LAB_004376e0:
  memcpy(param_2,param_1,__n);
  return 0;
}



/* ===== FUNCTION FUN_0043783c @ 0043783c ===== */

undefined * FUN_0043783c(int param_1)

{
  FUN_00428454(0x11);
  return (&PTR_PTR_012146f0)[param_1];
}



/* ===== FUNCTION FUN_00437878 @ 00437878 ===== */

undefined * FUN_00437878(undefined4 param_1,int param_2)

{
  return (&PTR_PTR_012146f0)[param_2];
}



/* ===== FUNCTION FUN_00437890 @ 00437890 ===== */

void FUN_00437890(int param_1,int param_2,uint param_3)

{
  *(uint *)(param_1 + 0x10) = param_2 << 4 | param_3;
  lv_obj_add_event_cb(param_1,FUN_0043591c,0x1e,0);
  lv_event_send(param_1,0x1e,0);
  return;
}



/* ===== FUNCTION FUN_004378dc @ 004378dc ===== */

void FUN_004378dc(void)

{
  int iVar1;
  
  if (DAT_0121d24c != 0) {
    lv_obj_del();
  }
  DAT_0121d24c = 0;
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0121d258);
  if (DAT_0121d278 != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_0121d278);
  }
  DAT_0121d278 = 0;
  if ((DAT_0121d27c != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
    lv_obj_del(DAT_0121d27c);
    DAT_0121d27c = 0;
    DAT_0121d274 = 0;
    DAT_0121d270 = 0;
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
    return;
  }
  DAT_0121d27c = 0;
  DAT_0121d274 = 0;
  DAT_0121d270 = 0;
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_0121d258);
  return;
}



/* ===== FUNCTION FUN_004379bc @ 004379bc ===== */

bool FUN_004379bc(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = lv_disp_get_default();
  iVar2 = lv_disp_get_scr_act(uVar1);
  if (DAT_0121720c != iVar2) {
    return iVar2 == DAT_012171f0;
  }
  return true;
}



/* ===== FUNCTION FUN_00437a10 @ 00437a10 ===== */

void FUN_00437a10(void)

{
  int iVar1;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c [2];
  
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  local_c[0] = 0;
  if (DAT_0121d280 != 0) {
    return;
  }
  DAT_0121d280 = 1;
  iVar1 = hudi_flash_open(local_c,1);
  if (iVar1 != 0) {
    puts("hudi flash open fail");
    return;
  }
  puts("hudi flash open success");
  hudi_flash_uid_read(local_c[0],&local_20,&local_10);
  hudi_flash_otp_read(local_c[0],3,0,&DAT_0121d284,0x100);
  hudi_flash_close(local_c[0]);
  return;
}



/* ===== FUNCTION FUN_00437acc @ 00437acc ===== */

void FUN_00437acc(void)

{
  int iVar1;
  undefined4 local_10 [3];
  
  local_10[0] = 0;
  iVar1 = hudi_flash_open(local_10,1);
  if (iVar1 == 0) {
    puts("hudi flash open success");
    hudi_flash_otp_write(local_10[0],3,0,&DAT_0121d284,0x100);
    hudi_flash_close(local_10[0]);
    return;
  }
  puts("hudi flash open fail");
  return;
}



/* ===== FUNCTION FUN_00437b50 @ 00437b50 ===== */

undefined4 FUN_00437b50(undefined4 param_1,undefined4 *param_2)

{
  undefined4 uVar1;
  
  switch(param_1) {
  default:
    uVar1 = 7;
    *param_2 = 7;
    break;
  case 1:
  case 2:
    uVar1 = 1;
    *param_2 = 1;
    break;
  case 3:
  case 4:
    uVar1 = 0;
    *param_2 = 0;
    break;
  case 7:
  case 10:
    uVar1 = 8;
    *param_2 = 8;
    break;
  case 8:
  case 0xb:
    uVar1 = 9;
    *param_2 = 9;
    break;
  case 9:
    uVar1 = 0xc;
    *param_2 = 0xc;
    break;
  case 0xc:
    uVar1 = 10;
    *param_2 = 10;
    break;
  case 0xd:
    uVar1 = 0xb;
    *param_2 = 0xb;
    break;
  case 0xe:
  case 0xf:
    uVar1 = 0x18;
    *param_2 = 0x18;
  }
  printf("%s:%d: tvtype=%d\n","_tvsys_to_tvtype",0x83,uVar1);
  return 0;
}



/* ===== FUNCTION FUN_00437c2c @ 00437c2c ===== */

undefined4 FUN_00437c2c(void)

{
  return DAT_01214758;
}



/* ===== FUNCTION FUN_00437c38 @ 00437c38 ===== */

undefined4 FUN_00437c38(int param_1)

{
  if (param_1 - 1U < 5) {
    return *(undefined4 *)(&DAT_00452ee0 + (param_1 - 1U) * 4);
  }
  return 0xb;
}



/* ===== FUNCTION FUN_00437c68 @ 00437c68 ===== */

undefined4 FUN_00437c68(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  char *pcVar4;
  undefined4 uVar5;
  char *pcVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint local_50 [13];
  undefined4 local_1c;
  
  if (param_1 - 1U < 5) {
    uVar9 = *(uint *)(&DAT_00452ee0 + (param_1 - 1U) * 4);
  }
  else {
    uVar9 = 0xb;
  }
  local_50[6] = 0;
  local_50[10] = 0;
  local_50[0xb] = 0;
  local_50[0] = 0xb;
  local_50[0xc] = 0;
  local_1c = 0;
  iVar2 = open("/dev/hdmi",2);
  if (iVar2 < 0) {
    pcVar6 = "/dev/hdmi";
    uVar5 = 0xb9;
    pcVar4 = "_tv_sys_edid_get";
  }
  else {
    iVar7 = 3;
    while( true ) {
      iVar3 = ioctl(iVar2,0x40041c00,local_50 + 6);
      iVar7 = iVar7 + -1;
      if (-1 < iVar3) break;
      FUN_0042c930(100);
      if (iVar7 == 0) {
        printf("%s(), line:%d, HDMI_TX_GET_EDID_TVSYS error\n","_tv_sys_edid_get",0xc5);
        close(iVar2);
        return 0xffffffff;
      }
    }
    FUN_00437b50(local_50[6],local_50);
    DAT_01214758 = local_50[0];
    close(iVar2);
    uVar1 = local_50[0];
    if ((int)local_50[0] < 0) {
      return 0xffffffff;
    }
    iVar2 = open("/dev/dis",1);
    if (-1 < iVar2) {
      local_50[6] = 0;
      local_50[7] = 0;
      local_50[8] = 0;
      local_50[9] = 0;
      ioctl(iVar2,0xc0100e01,local_50 + 6);
      printf("%s(), line:%d, current true TV sys: %d!\n","_tv_sys_set",0xea,local_50[8]);
      uVar8 = uVar1;
      if ((param_1 != 6) && (uVar8 = uVar9, uVar1 < uVar9)) {
        printf("tv not support tvtype: tv_type=%d, set_tv_type:%d\n",uVar1,uVar9);
        return 0xffffffff;
      }
      local_50[10] = 1;
      local_50[0xb] = 1;
      if ((uVar8 < 0x19) && ((1 << (uVar8 & 0x1f) & 0x1800cc0U) != 0)) {
        local_1c = CONCAT31(local_1c._1_3_,1);
      }
      local_50[0] = 0;
      local_50[1] = 0;
      local_50[2] = 0;
      local_50[3] = 0;
      local_50[4] = 0;
      local_50[5] = 0;
      local_50[0xc] = uVar8;
      ioctl(iVar2,0x80180e13,local_50);
      printf("%s(), line: %d, single output set TV sys: %d\n","_tv_sys_set",0x124,local_50[0xc]);
      ioctl(iVar2,0x80100e00,local_50 + 10);
      local_50[0] = 0;
      local_50[1] = 0;
      local_50[2] = 1;
      local_50[3] = 1;
      local_50[4] = 0;
      local_50[5] = 0;
      ioctl(iVar2,0x80180e12,local_50);
      local_50[0] = 1;
      local_50[1] = 0;
      local_50[2] = 0;
      iVar7 = ioctl(iVar2,0xc00c0e0c,local_50);
      if (iVar7 == 0) {
        iVar7 = open("/dev/fb0",2);
        if (iVar7 < 0) {
          printf("%s(), line:%d. open device: %s error!\n","_tv_sys_scale_out",0x97,"/dev/fb0");
        }
        else {
          DAT_01214754 = local_50[2];
          ioctl(iVar7,0x80080d00,&DAT_01214750);
          close(iVar7);
        }
      }
      close(iVar2);
      return 0;
    }
    pcVar6 = "/dev/dis";
    uVar5 = 0xe3;
    pcVar4 = "_tv_sys_set";
  }
  printf("%s(), line:%d. open device: %s error!\n",pcVar4,uVar5,pcVar6);
  return 0xffffffff;
}



/* ===== FUNCTION FUN_00438630 @ 00438630 ===== */

undefined * FUN_00438630(uint param_1,int param_2)

{
  int iVar1;
  undefined *puVar2;
  uint uVar3;
  
  uVar3 = param_2 - 1;
  if (DAT_01214760 == param_1) {
    iVar1 = 0;
  }
  else {
    if (DAT_01214768 != param_1) {
      return (undefined *)0x0;
    }
    iVar1 = 1;
  }
  puVar2 = (&PTR_DAT_01214764)[iVar1 * 2];
  if ((puVar2 != (undefined *)0x0) &&
     (uVar3 < (uint)(byte)puVar2[2] * 0x100 + (uint)(byte)puVar2[3])) {
    return puVar2 + (uint)(byte)puVar2[uVar3 * 3 + 4] * 0x10000 +
                    (uint)(byte)puVar2[uVar3 * 3 + 5] * 0x100 + (uint)(byte)puVar2[uVar3 * 3 + 6];
  }
  return (undefined *)0x0;
}



/* ===== FUNCTION FUN_004386cc @ 004386cc ===== */

undefined * FUN_004386cc(int param_1)

{
  int iVar1;
  undefined *puVar2;
  uint uVar3;
  
  FUN_00428454(0x11);
  uVar3 = param_1 - 1;
  if (DAT_01214760 == 0) {
    iVar1 = 0;
  }
  else {
    if (DAT_01214768 != 0) {
      return (undefined *)0x0;
    }
    iVar1 = 1;
  }
  puVar2 = (&PTR_DAT_01214764)[iVar1 * 2];
  if ((puVar2 != (undefined *)0x0) &&
     (uVar3 < (uint)(byte)puVar2[2] * 0x100 + (uint)(byte)puVar2[3])) {
    return puVar2 + (uint)(byte)puVar2[uVar3 * 3 + 4] * 0x10000 +
                    (uint)(byte)puVar2[uVar3 * 3 + 5] * 0x100 + (uint)(byte)puVar2[uVar3 * 3 + 6];
  }
  return (undefined *)0x0;
}



/* ===== FUNCTION FUN_004387a8 @ 004387a8 ===== */

void FUN_004387a8(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar3 = *(int *)(param_1 + 0xc);
  iVar1 = lv_obj_is_valid(iVar3);
  if (iVar1 != 0) {
    uVar2 = FUN_00428454(*(undefined4 *)(iVar3 + 0x10));
    lv_label_set_text_fmt(iVar3,"<   %d   >",uVar2);
  }
  DAT_0121d3d4 = 0;
  return;
}



/* ===== FUNCTION FUN_004387fc @ 004387fc ===== */

void FUN_004387fc(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  char acStack_98 [128];
  undefined4 local_18 [3];
  
  DAT_0121d3bc = FUN_004386cc(0x8c);
  DAT_0121d3c0 = FUN_004386cc(0x8e);
  DAT_0121d3c4 = &DAT_0044f178;
  memset(acStack_98,0,0x80);
  uVar1 = FUN_004386cc(0x34);
  sprintf(acStack_98,"%s?",uVar1);
  uVar1 = lv_msgbox_create(DAT_0121d398,&DAT_0044f178,acStack_98,&DAT_0121d3bc,0);
  DAT_0121d390 = uVar1;
  uVar2 = lv_msgbox_get_content(uVar1);
  uVar3 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar2,uVar3,0);
  uVar2 = lv_msgbox_get_btns(uVar1);
  uVar3 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar2,uVar3,0);
  iVar4 = FUN_0043783c(1);
  lv_obj_set_style_height(uVar2,(int)(short)(*(short *)(iVar4 + 8) + 0xd),0);
  lv_palette_darken(local_18,0x12,1);
  lv_obj_set_style_bg_color(uVar1,local_18[0],0);
  lv_obj_add_event_cb(uVar1,FUN_00439698,0,0);
  lv_obj_align(uVar1,9,0,0);
  uVar1 = lv_msgbox_get_btns(uVar1);
  lv_group_focus_obj(uVar1);
  return;
}



/* ===== FUNCTION FUN_00438964 @ 00438964 ===== */

void FUN_00438964(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0xc);
  if ((iVar2 != 0) && (iVar1 = lv_obj_is_valid(iVar2), iVar1 != 0)) {
    lv_obj_del(iVar2);
    DAT_0121d3cc = 0;
    DAT_0121d3d0 = 0;
    return;
  }
  DAT_0121d3cc = 0;
  DAT_0121d3d0 = 0;
  return;
}



/* ===== FUNCTION FUN_00438bb8 @ 00438bb8 ===== */

undefined4 FUN_00438bb8(undefined4 param_1,undefined4 param_2,uint param_3,int param_4)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar1 = lv_obj_create(DAT_0121d398);
  DAT_0121d390 = uVar1;
  lv_obj_align(uVar1,2,0x202c,0x14);
  lv_obj_set_size(uVar1,0x2005,0x2046);
  lv_obj_set_style_bg_color(uVar1,0xff303030,0);
  lv_obj_set_flex_flow(uVar1,1);
  lv_obj_set_flex_align(uVar1,2,2,2);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_clear_flag(uVar1,0x10);
  lv_obj_set_style_pad_left(uVar1,0,0);
  lv_obj_set_style_pad_right(uVar1,0,0);
  lv_group_add_obj(DAT_0121d3a4,uVar1);
  iVar2 = lv_slider_create(uVar1);
  lv_obj_set_size(iVar2,0x2046,0x2064);
  lv_bar_set_range(iVar2,0,param_2);
  lv_bar_set_value(iVar2,param_1,0);
  lv_obj_set_style_bg_opa(iVar2,0,0x30000);
  uVar3 = lv_label_create(iVar2);
  lv_label_set_text_fmt(uVar3,&DAT_00452028,param_1);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  uVar4 = FUN_0043783c(2);
  lv_obj_set_style_text_font(uVar3,uVar4,0);
  lv_obj_align(uVar3,9,0,0);
  *(uint *)(iVar2 + 0x10) = param_4 << 0x10 | param_3 & 0xffff;
  return uVar1;
}



/* ===== FUNCTION FUN_00438d84 @ 00438d84 ===== */

void FUN_00438d84(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  
  uVar2 = 0;
  piVar5 = &DAT_00455cf8;
  do {
    iVar4 = *piVar5;
    uVar2 = uVar2 + 1;
    iVar1 = FUN_00428454(0x14,param_2);
    piVar5 = piVar5 + 1;
    if (iVar4 == iVar1) {
      iVar1 = (uVar2 & 3) * 4;
      uVar3 = (&DAT_00455cf8)[uVar2 & 3];
      goto LAB_00438de4;
    }
    param_2 = 0;
  } while (uVar2 != 4);
  uVar3 = 0;
  iVar1 = 0;
LAB_00438de4:
  FUN_004286f8(0x14);
  FUN_00433d28(uVar3);
  uVar3 = lv_obj_get_child(param_1,1);
  FUN_00437890(uVar3,*(undefined2 *)((int)&DAT_0121477c + iVar1),2);
  return;
}



/* ===== FUNCTION FUN_00439698 @ 00439698 ===== */

void FUN_00439698(undefined4 param_1)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  uVar2 = lv_event_get_current_target();
  iVar3 = lv_event_get_code(param_1);
  iVar4 = lv_event_get_target(param_1);
  if (iVar3 == 0xd) {
    uVar5 = lv_indev_get_act();
    sVar1 = lv_indev_get_key(uVar5);
    if (sVar1 == 10) {
      iVar3 = lv_msgbox_get_active_btn(uVar2);
      if (iVar3 != 0) {
        lv_obj_del(*(undefined4 *)(iVar4 + 4));
        if (DAT_0121d3ac != 0) {
          lv_group_focus_obj(DAT_0121d3fc);
        }
        DAT_0121d390 = 0;
        lv_obj_clear_flag(DAT_0121d3a8,1);
        FUN_00428080();
        return;
      }
      FUN_00427b7c();
      FUN_0042ce18();
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0043978c @ 0043978c ===== */

void FUN_0043978c(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined4 uVar6;
  uint uVar7;
  
  iVar1 = lv_event_get_code();
  uVar2 = lv_event_get_target(param_1);
  iVar3 = lv_obj_get_child(uVar2,0);
  uVar4 = lv_event_get_user_data(param_1);
  if (iVar1 == 0xd) {
    uVar5 = *(uint *)(iVar3 + 0x10);
    uVar7 = 100;
    if (uVar5 == 0) {
      uVar5 = 0;
    }
    else {
      uVar7 = uVar5 >> 0x10;
      uVar5 = uVar5 & 0xffff;
    }
    if (DAT_0121d3a0 != 0) {
      lv_timer_reset();
      lv_timer_resume(DAT_0121d3a0);
    }
    uVar6 = lv_indev_get_act();
    DAT_0121d3b8 = lv_indev_get_key(uVar6);
    DAT_0121d3b0 = DAT_0121d3b4;
    if ((DAT_0121d3b8 & 0xfd) == 0x11) {
      DAT_0121d3b4 = DAT_0121d3b4 + 1;
      if ((int)DAT_0121d3b4 <= (int)uVar7) {
        if ((uVar4 - 1 < 2) || (uVar4 - 4 < 2)) {
          FUN_0044c8a8(DAT_0121d3b4,uVar4 & 0xff);
        }
        else if (uVar4 == 3) {
          FUN_0044c8a8(DAT_0121d3b4,3);
        }
        iVar1 = 0;
        if (0 < (int)(DAT_0121d3b4 - DAT_0121d3b0)) {
          do {
            lv_event_send(iVar3,0xd,&DAT_0121d3b8);
            iVar1 = iVar1 + 1;
          } while (iVar1 < (int)(DAT_0121d3b4 - DAT_0121d3b0));
        }
LAB_004399c8:
        uVar2 = lv_obj_get_child(iVar3,0);
        lv_label_set_text_fmt(uVar2,&DAT_00452028,DAT_0121d3b4);
        uVar2 = lv_obj_get_child(DAT_0121d3fc,1);
        lv_label_set_text_fmt(uVar2,&DAT_00452028,DAT_0121d3b4);
        return;
      }
    }
    else if ((DAT_0121d3b8 == 0x12) || (DAT_0121d3b8 == 0x14)) {
      DAT_0121d3b4 = DAT_0121d3b4 - 1;
      uVar7 = uVar5;
      if ((int)uVar5 <= (int)DAT_0121d3b4) {
        if ((uVar4 - 1 < 2) || (uVar4 - 4 < 2)) {
          FUN_0044c8a8(DAT_0121d3b4,uVar4 & 0xff);
        }
        else if (uVar4 == 3) {
          FUN_0044c8a8(DAT_0121d3b4,3);
        }
        iVar1 = 0;
        if (0 < (int)(DAT_0121d3b0 - DAT_0121d3b4)) {
          do {
            lv_event_send(iVar3,0xd,&DAT_0121d3b8);
            iVar1 = iVar1 + 1;
          } while (iVar1 < (int)(DAT_0121d3b0 - DAT_0121d3b4));
        }
        goto LAB_004399c8;
      }
    }
    else if ((DAT_0121d3b8 == 0x1b) || (uVar7 = DAT_0121d3b4, (DAT_0121d3b8 & 0xf7) == 2)) {
      lv_obj_del(uVar2);
      uVar2 = lv_obj_get_child(DAT_0121d3fc,1);
      lv_label_set_text_fmt(uVar2,&DAT_00452028,DAT_0121d3b4);
      if (DAT_0121d3ac != 0) {
        lv_group_focus_obj(DAT_0121d3fc);
      }
      DAT_0121d390 = 0;
      lv_obj_clear_flag(DAT_0121d3a8,1);
      FUN_00428080();
      return;
    }
  }
  else if (iVar1 == 0xe) {
    uVar7 = FUN_00428454(uVar4);
  }
  else {
    uVar7 = DAT_0121d3b4;
    if (((iVar1 == 0x1a) && (iVar1 = lv_event_get_param(param_1), uVar7 = DAT_0121d3b4, iVar1 != 0))
       && (*(int *)(iVar1 + 0x10) != 0)) {
      *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
      return;
    }
  }
  DAT_0121d3b4 = uVar7;
  return;
}



/* ===== FUNCTION FUN_0043a53c @ 0043a53c ===== */

void FUN_0043a53c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043a7b0:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x1d);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043aa1c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043a7d8:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x1d;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x1d,iVar5);
      goto LAB_0043a644;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043a7b0;
      iVar5 = FUN_00428454(0x1d);
      goto LAB_0043aa1c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        DAT_0121d3c8 = DAT_0121d3c8 + '\x01';
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x1d);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043a644;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043a7b0;
      iVar5 = FUN_00428454(0x1d);
      goto LAB_0043a7d8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043a644:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043ab70 @ 0043ab70 ===== */

void FUN_0043ab70(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043ade4:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x17);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043b050:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043ae0c:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x17;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x17,iVar5);
      goto LAB_0043ac78;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ade4;
      iVar5 = FUN_00428454(0x17);
      goto LAB_0043b050;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_004387fc(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x17);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043ac78;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ade4;
      iVar5 = FUN_00428454(0x17);
      goto LAB_0043ae0c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043ac78:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043b1b0 @ 0043b1b0 ===== */

void FUN_0043b1b0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043b424:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x14);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043b690:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043b44c:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x14;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x14,iVar5);
      goto LAB_0043b2b8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043b424;
      iVar5 = FUN_00428454(0x14);
      goto LAB_0043b690;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        FUN_00438d84(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x14);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043b2b8;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043b424;
      iVar5 = FUN_00428454(0x14);
      goto LAB_0043b44c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043b2b8:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043b7e0 @ 0043b7e0 ===== */

void FUN_0043b7e0(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043ba54:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x11);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043bcc0:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043ba7c:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x11;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x11,iVar5);
      goto LAB_0043b8e8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ba54;
      iVar5 = FUN_00428454(0x11);
      goto LAB_0043bcc0;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_00447d38(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x11);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043b8e8;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ba54;
      iVar5 = FUN_00428454(0x11);
      goto LAB_0043ba7c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043b8e8:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043be20 @ 0043be20 ===== */

void FUN_0043be20(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043c094:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043c300:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043c0bc:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0,iVar5);
      goto LAB_0043bf28;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043c094;
      iVar5 = FUN_00428454(0);
      goto LAB_0043c300;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044c7cc(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043bf28;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043c094;
      iVar5 = FUN_00428454(0);
      goto LAB_0043c0bc;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043bf28:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043c45c @ 0043c45c ===== */

void FUN_0043c45c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043c6d0:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x3b);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043c93c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043c6f8:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x3b;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x3b,iVar5);
      goto LAB_0043c564;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043c6d0;
      iVar5 = FUN_00428454(0x3b);
      goto LAB_0043c93c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044a2b8(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x3b);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043c564;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043c6d0;
      iVar5 = FUN_00428454(0x3b);
      goto LAB_0043c6f8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043c564:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043ca9c @ 0043ca9c ===== */

void FUN_0043ca9c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043cd10:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x18);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043cf7c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043cd38:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x18;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x18,iVar5);
      goto LAB_0043cba4;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043cd10;
      iVar5 = FUN_00428454(0x18);
      goto LAB_0043cf7c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044c474(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x18);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043cba4;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043cd10;
      iVar5 = FUN_00428454(0x18);
      goto LAB_0043cd38;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043cba4:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043d0dc @ 0043d0dc ===== */

void FUN_0043d0dc(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043d350:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x15);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043d5bc:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043d378:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x15;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x15,iVar5);
      goto LAB_0043d1e4;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043d350;
      iVar5 = FUN_00428454(0x15);
      goto LAB_0043d5bc;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_00446490(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x15);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043d1e4;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043d350;
      iVar5 = FUN_00428454(0x15);
      goto LAB_0043d378;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043d1e4:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043d71c @ 0043d71c ===== */

void FUN_0043d71c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043d990:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x3a);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043dbfc:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043d9b8:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x3a;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x3a,iVar5);
      goto LAB_0043d824;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043d990;
      iVar5 = FUN_00428454(0x3a);
      goto LAB_0043dbfc;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044a248(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x3a);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043d824;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043d990;
      iVar5 = FUN_00428454(0x3a);
      goto LAB_0043d9b8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043d824:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043dd5c @ 0043dd5c ===== */

void FUN_0043dd5c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043dfd0:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(8);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043e23c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043dff8:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 8;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(8,iVar5);
      goto LAB_0043de64;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043dfd0;
      iVar5 = FUN_00428454(8);
      goto LAB_0043e23c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044a180(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(8);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043de64;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043dfd0;
      iVar5 = FUN_00428454(8);
      goto LAB_0043dff8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043de64:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043e39c @ 0043e39c ===== */

void FUN_0043e39c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043e610:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x19);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043e87c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_0043e638:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 0x19;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x19,iVar5);
      goto LAB_0043e4a4;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043e610;
      iVar5 = FUN_00428454(0x19);
      goto LAB_0043e87c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_0044b628(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x19);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043e4a4;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043e610;
      iVar5 = FUN_00428454(0x19);
      goto LAB_0043e638;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043e4a4:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043e9dc @ 0043e9dc ===== */

void FUN_0043e9dc(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    uVar3 = lv_obj_get_child(iVar2,1);
    uVar4 = FUN_00428454(0x10);
    lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_0043ec78:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(0x10);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0043ef00:
        iVar1 = -10;
        if (-0xb < iVar5 + -1) {
          iVar1 = iVar5 + -1;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar1);
      }
      else {
LAB_0043eca0:
        iVar1 = 10;
        if (iVar5 + 1 < 0xb) {
          iVar1 = iVar5 + 1;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar1);
      }
      uVar3 = FUN_00428454(0xf);
      FUN_0044a334(4,uVar3,iVar1);
      if (DAT_0121d3d4 == 0) {
        iVar2 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar2 + 0x10) = 0x10;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar2);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(0x10,iVar1);
      goto LAB_0043eae4;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ec78;
      iVar5 = FUN_00428454(0x10);
      goto LAB_0043ef00;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_group_focus_freeze(DAT_0121d3a4,1);
        lv_obj_set_style_bg_opa(iVar2,0,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x10);
        lv_label_set_text_fmt(uVar3,"<   %d   >",uVar4);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(0x10);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0043eae4;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_0043ec78;
      iVar5 = FUN_00428454(0x10);
      goto LAB_0043eca0;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0043eae4:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0043ff20 @ 0043ff20 ===== */

void FUN_0043ff20(undefined4 param_1,undefined4 param_2,uint param_3)

{
  undefined4 uVar1;
  undefined4 local_18 [2];
  
  uVar1 = lv_obj_create();
  lv_obj_set_scrollbar_mode(uVar1,0);
  lv_obj_set_style_radius(uVar1,param_2,0);
  if ((int)param_3 < 0) {
    param_3 = (uint)(short)(1000U - (short)param_3 | 0x2000);
  }
  else {
    param_3 = param_3 | 0x2000;
  }
  lv_obj_set_size(uVar1,param_3,0x2064);
  lv_palette_lighten(local_18,0x12,4);
  lv_obj_set_style_bg_color(uVar1,local_18[0],0);
  return;
}



/* ===== FUNCTION FUN_0043ffc4 @ 0043ffc4 ===== */

undefined4 FUN_0043ffc4(undefined4 param_1,int param_2,int param_3)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 local_18 [2];
  
  uVar3 = lv_obj_create();
  lv_obj_set_style_radius(uVar3,0,0);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_align(uVar3,5,0x2001,0x23ea);
  lv_palette_lighten(local_18,0x12,2);
  lv_obj_set_style_bg_color(uVar3,local_18[0],0);
  lv_obj_set_style_bg_opa(uVar3,0x7f,0);
  lv_obj_set_flex_flow(uVar3,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_pad_row(uVar3,0,0);
  lv_obj_set_style_pad_column(uVar3,0,0);
  lv_obj_set_flex_align(uVar3,0,2,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00440134 @ 00440134 ===== */

undefined4 FUN_00440134(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar3 = lv_obj_create();
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_align(uVar3,7,0,0);
  lv_obj_set_style_radius(uVar3,0,0);
  uVar1 = (ushort)param_3;
  uVar2 = (ushort)param_4;
  if (param_3 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_4 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_set_style_bg_opa(uVar3,0,0);
  uVar4 = lv_label_create(uVar3);
  lv_label_set_text(uVar4,param_2);
  uVar5 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar4,uVar5,0);
  lv_obj_align(uVar4,9,0,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00440290 @ 00440290 ===== */

undefined4 FUN_00440290(undefined4 param_1,int param_2,int param_3,int param_4,ushort param_5)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 local_30 [3];
  
  uVar3 = lv_obj_create();
  if (0 < param_4) {
    if ((short)param_5 < 0) {
      param_5 = 1000 - param_5;
    }
    iVar5 = 0;
    do {
      uVar4 = lv_obj_create(uVar3);
      lv_obj_set_scrollbar_mode(uVar4,0);
      lv_obj_set_style_radius(uVar4,8,0);
      lv_obj_set_size(uVar4,(int)(short)(param_5 | 0x2000),0x2064);
      lv_palette_lighten(local_30,0x12,4);
      iVar5 = iVar5 + 1;
      lv_obj_set_style_bg_color(uVar4,local_30[0],0);
    } while (param_4 != iVar5);
  }
  lv_palette_darken(local_30,0x12,2);
  lv_obj_set_style_bg_color(uVar3,local_30[0],0);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_pad_row(uVar3,1,0);
  lv_obj_set_style_pad_column(uVar3,1,0);
  lv_obj_set_style_pad_left(uVar3,1,0);
  lv_obj_set_style_pad_right(uVar3,1,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_flex_flow(uVar3,0);
  uVar4 = lv_group_get_default();
  lv_group_add_obj(uVar4,uVar3);
  lv_group_focus_obj(uVar3);
  return uVar3;
}



/* ===== FUNCTION FUN_004404b8 @ 004404b8 ===== */

undefined4 FUN_004404b8(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  
  uVar3 = lv_obj_create();
  lv_obj_set_scrollbar_mode(uVar3,0);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  uVar4 = lv_label_create(uVar3);
  lv_label_set_text_fmt(uVar4,&DAT_00452028,param_4);
  uVar5 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar4,uVar5,0);
  lv_obj_align(uVar4,9,0,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00440608 @ 00440608 ===== */

void FUN_00440608(void)

{
  int iVar1;
  
  if ((DAT_0121d390 != 0) && (iVar1 = lv_obj_is_valid(), iVar1 != 0)) {
    lv_obj_del(DAT_0121d390);
    if (DAT_0121d3ac != 0) {
      lv_group_focus_obj(DAT_0121d3fc);
    }
    DAT_0121d390 = 0;
    lv_obj_clear_flag(DAT_0121d3a8,1);
    FUN_00428080();
    DAT_0121d390 = 0;
    return;
  }
  DAT_0121d390 = 0;
  return;
}



/* ===== FUNCTION FUN_00440694 @ 00440694 ===== */

void FUN_00440694(undefined4 param_1)

{
  DAT_0121d390 = param_1;
  return;
}



/* ===== FUNCTION FUN_004406a0 @ 004406a0 ===== */

void FUN_004406a0(undefined4 param_1,char *param_2,undefined4 param_3)

{
  size_t sVar1;
  char local_48 [60];
  
  local_48[0] = '\0';
  local_48[1] = '\0';
  local_48[2] = '\0';
  local_48[3] = '\0';
  local_48[4] = '\0';
  local_48[5] = '\0';
  local_48[6] = '\0';
  local_48[7] = '\0';
  local_48[8] = '\0';
  local_48[9] = '\0';
  local_48[10] = '\0';
  local_48[0xb] = '\0';
  local_48[0xc] = '\0';
  local_48[0xd] = '\0';
  local_48[0xe] = '\0';
  local_48[0xf] = '\0';
  local_48[0x10] = '\0';
  local_48[0x11] = '\0';
  local_48[0x12] = '\0';
  local_48[0x13] = '\0';
  local_48[0x14] = '\0';
  local_48[0x15] = '\0';
  local_48[0x16] = '\0';
  local_48[0x17] = '\0';
  local_48[0x18] = '\0';
  local_48[0x19] = '\0';
  local_48[0x1a] = '\0';
  local_48[0x1b] = '\0';
  local_48[0x1c] = '\0';
  local_48[0x1d] = '\0';
  local_48[0x1e] = '\0';
  local_48[0x1f] = '\0';
  local_48[0x20] = '\0';
  local_48[0x21] = '\0';
  local_48[0x22] = '\0';
  local_48[0x23] = '\0';
  local_48[0x24] = '\0';
  local_48[0x25] = '\0';
  local_48[0x26] = '\0';
  local_48[0x27] = '\0';
  local_48[0x28] = '\0';
  local_48[0x29] = '\0';
  local_48[0x2a] = '\0';
  local_48[0x2b] = '\0';
  local_48[0x2c] = '\0';
  local_48[0x2d] = '\0';
  local_48[0x2e] = '\0';
  local_48[0x2f] = '\0';
  local_48[0x30] = '\0';
  local_48[0x31] = '\0';
  snprintf(local_48,0x32,"%s",param_3);
  if (*param_2 == '#') {
    sVar1 = strlen(param_2);
    strncpy(local_48 + 8,param_2 + 8,sVar1 - 9);
  }
  else {
    sVar1 = strlen(param_2);
    strncpy(local_48 + 8,param_2,sVar1);
  }
  sVar1 = strlen(local_48);
  local_48[sVar1 + 1] = '\0';
  local_48[sVar1] = '#';
  lv_label_set_text(param_1,local_48);
  return;
}



/* ===== FUNCTION FUN_00440788 @ 00440788 ===== */

undefined4 FUN_00440788(undefined4 param_1,short *param_2)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 local_18 [2];
  
  uVar2 = lv_btn_create();
  lv_obj_set_size(uVar2,0x2050,0x200b);
  lv_obj_set_style_pad_left(uVar2,3,0);
  lv_obj_set_style_pad_right(uVar2,3,0);
  lv_obj_set_style_border_side(uVar2,0xf,0);
  lv_obj_set_style_radius(uVar2,5,0);
  lv_obj_set_style_border_width(uVar2,2,0);
  lv_obj_set_style_border_color(uVar2,0xffffffff,0);
  lv_obj_set_style_border_opa(uVar2,0,0);
  lv_obj_set_style_border_opa(uVar2,0xff,0x1000);
  lv_obj_set_style_bg_opa(uVar2,0,0);
  lv_palette_darken(local_18,5,2);
  lv_obj_set_style_bg_color(uVar2,local_18[0],0);
  if (*(int *)(param_2 + 4) != 0) {
    lv_obj_add_event_cb(uVar2,*(int *)(param_2 + 4),0,uVar2);
  }
  uVar3 = lv_label_create(uVar2);
  lv_label_set_recolor(uVar3,1);
  lv_obj_align(uVar3,7,0,0);
  lv_obj_set_style_text_color(uVar2,0xffffffff,0);
  lv_obj_set_style_text_color(uVar2,0xff7d7d7d,0x80);
  FUN_00437890(uVar3,*param_2,2);
  if (*(char *)((int)param_2 + 5) == '\0') {
    sVar1 = *param_2;
  }
  else {
    lv_obj_add_state(uVar2,0x80);
    sVar1 = *param_2;
  }
  if (sVar1 == 0x32) {
    DAT_0121d3d8 = uVar2;
  }
  uVar3 = lv_label_create(uVar2);
  lv_obj_align(uVar3,8,0,0);
  lv_label_set_recolor(uVar3,1);
  lv_obj_set_width(uVar3,0x202d);
  lv_obj_set_style_text_align(uVar3,2,0);
  if ((char)param_2[2] == '\0') {
    if (param_2[1] == -1) {
      lv_label_set_text(uVar3," ",2);
      return uVar2;
    }
    FUN_00437890(uVar3,param_2[1]);
    return uVar2;
  }
  lv_label_set_text_fmt(uVar3,&DAT_00452028,(int)param_2[1]);
  lv_obj_set_style_text_font(uVar3,lv_font_montserrat_26,0);
  return uVar2;
}



/* ===== FUNCTION FUN_00440d54 @ 00440d54 ===== */

undefined4 FUN_00440d54(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  lv_obj_set_style_pad_left(param_1,0,0);
  lv_obj_set_style_pad_right(param_1,0,0);
  lv_obj_set_flex_flow(param_1,0);
  lv_obj_set_style_pad_row(param_1,0,0);
  lv_obj_set_style_pad_column(param_1,0,0);
  lv_obj_set_style_pad_top(param_1,0,0);
  lv_obj_set_style_pad_bottom(param_1,0,0);
  lv_obj_set_style_bg_opa(param_1,0x7f,0);
  lv_obj_set_style_bg_color(param_1,0xff6565b1,0);
  uVar1 = lv_obj_create(param_1);
  lv_obj_set_style_radius(uVar1,0,0);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_set_size(uVar1,0x2016,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  lv_obj_set_style_pad_left(uVar1,0,0);
  lv_obj_set_style_pad_right(uVar1,0,0);
  lv_obj_set_scrollbar_mode(uVar1,0);
  uVar1 = lv_label_create(uVar1);
  lv_label_set_recolor(uVar1,1);
  lv_obj_align(uVar1,9,0,0);
  uVar1 = lv_obj_create(param_1);
  lv_obj_set_size(uVar1,0x2018,0x2064);
  lv_obj_set_style_bg_color(uVar1,0xff4b4b4b,0);
  lv_obj_set_style_radius(uVar1,0,0);
  lv_obj_set_style_pad_left(uVar1,0,0);
  lv_obj_set_style_pad_right(uVar1,0,0);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_style_img_opa(uVar1,0xff,0);
  uVar1 = lv_obj_create(param_1);
  lv_obj_set_style_pad_left(uVar1,0,0);
  lv_obj_set_style_pad_right(uVar1,0,0);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_set_style_outline_width(uVar1,0,0);
  lv_obj_set_style_radius(uVar1,0,0);
  lv_obj_set_size(uVar1,0x2036,0x2064);
  iVar2 = lv_disp_get_ver_res(0);
  iVar2 = (int)(short)(iVar2 / 10);
  lv_obj_set_style_pad_top(uVar1,iVar2,0);
  lv_obj_set_style_pad_bottom(uVar1,iVar2,0);
  lv_obj_set_scrollbar_mode(uVar1,0);
  lv_obj_set_flex_flow(uVar1,1);
  lv_obj_set_style_pad_row(uVar1,0,0);
  lv_obj_set_style_pad_column(uVar1,0,0);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_obj_set_style_text_opa(uVar1,0xff,0);
  iVar2 = param_2;
  do {
    iVar3 = iVar2 + 0xc;
    FUN_00440788(uVar1,iVar2);
    iVar2 = iVar3;
  } while (param_2 + 0x54 != iVar3);
  return uVar1;
}



/* ===== FUNCTION FUN_00441084 @ 00441084 ===== */

undefined4 FUN_00441084(undefined4 param_1)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined2 local_78;
  undefined2 local_76;
  undefined1 local_74;
  undefined1 local_73;
  code *local_70;
  undefined2 local_6c;
  undefined2 uStack_6a;
  undefined1 local_68;
  undefined1 local_67;
  code *local_64;
  undefined2 local_60;
  undefined2 local_5e;
  undefined1 local_5c;
  undefined1 local_5b;
  code *local_58;
  undefined2 local_54;
  undefined2 local_52;
  undefined1 local_50;
  undefined1 local_4f;
  code *local_4c;
  undefined2 local_48;
  undefined2 local_46;
  undefined1 local_44;
  undefined1 local_43;
  undefined1 *local_40;
  undefined2 local_3c;
  undefined2 local_3a;
  undefined1 local_38;
  undefined1 local_37;
  code *local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  
  iVar2 = FUN_00428454(0);
  local_78 = 0xc;
  bVar1 = iVar2 != 3;
  iVar2 = FUN_00428454(0);
  local_74 = 0;
  local_73 = 0;
  local_6c = 0xd;
  local_76 = (undefined2)*(undefined4 *)(&DAT_012166e0 + iVar2 * 8);
  local_70 = FUN_0043be20;
  uStack_6a = FUN_00428454(1);
  local_68 = 1;
  local_60 = 0xf;
  local_64 = FUN_00445104;
  local_67 = bVar1;
  local_5e = FUN_00428454(2);
  local_5c = 1;
  local_54 = 0x10;
  local_58 = FUN_00445d64;
  local_5b = bVar1;
  local_52 = FUN_00428454(4);
  local_50 = 1;
  local_48 = 0x11;
  local_4c = FUN_00445734;
  local_4f = bVar1;
  local_46 = FUN_00428454(3);
  local_44 = 1;
  local_3c = 0x12;
  local_40 = &LAB_00444ad4;
  local_43 = bVar1;
  iVar2 = FUN_00428454(6);
  local_38 = 0;
  local_37 = 0;
  local_34 = FUN_0044303c;
  local_3a = (undefined2)(&DAT_01214790)[iVar2 * 2];
  uVar3 = FUN_00428454(1);
  FUN_0044c8a8(uVar3,1);
  uVar3 = FUN_00428454(2);
  FUN_0044c8a8(uVar3,2);
  uVar3 = FUN_00428454(4);
  FUN_0044c8a8(uVar3,4);
  uVar3 = FUN_00428454(3);
  FUN_0044c8a8(uVar3,3);
  uVar3 = FUN_00428454(5);
  FUN_0044c8a8(uVar3,5);
  uVar3 = FUN_00428454(6);
  FUN_00446580(uVar3);
  lv_obj_set_style_pad_left(param_1,0,0);
  lv_obj_set_style_pad_right(param_1,0,0);
  lv_obj_set_flex_flow(param_1,0);
  lv_obj_set_style_pad_row(param_1,0,0);
  lv_obj_set_style_pad_column(param_1,0,0);
  lv_obj_set_style_pad_top(param_1,0,0);
  lv_obj_set_style_pad_bottom(param_1,0,0);
  lv_obj_set_style_bg_opa(param_1,0x7f,0);
  lv_obj_set_style_bg_color(param_1,0xff6565b1,0);
  uVar3 = lv_obj_create(param_1);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_size(uVar3,0x2016,0x2064);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  uVar3 = lv_label_create(uVar3);
  lv_label_set_recolor(uVar3,1);
  lv_obj_align(uVar3,9,0,0);
  uVar3 = lv_obj_create(param_1);
  lv_obj_set_size(uVar3,0x2018,0x2064);
  lv_obj_set_style_bg_color(uVar3,0xff4b4b4b,0);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  lv_obj_set_style_img_opa(uVar3,0xff,0);
  uVar3 = lv_obj_create(param_1);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_size(uVar3,0x2036,0x2064);
  iVar2 = lv_disp_get_ver_res(0);
  iVar2 = (int)(short)(iVar2 / 10);
  lv_obj_set_style_pad_top(uVar3,iVar2,0);
  lv_obj_set_style_pad_bottom(uVar3,iVar2,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_flex_flow(uVar3,1);
  lv_obj_set_style_pad_row(uVar3,0,0);
  lv_obj_set_style_pad_column(uVar3,0,0);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  puVar7 = (undefined4 *)&local_78;
  do {
    puVar6 = puVar7 + 3;
    FUN_00440788(uVar3,puVar7);
    puVar7 = puVar6;
  } while (puVar6 != &local_30);
  local_30 = 1;
  puVar7 = &DAT_0121eddc;
  local_2c = 2;
  iVar2 = 0;
  local_28 = 4;
  local_24 = 3;
  do {
    iVar2 = iVar2 + 1;
    uVar4 = lv_obj_get_child(uVar3,iVar2);
    uVar5 = *puVar6;
    puVar6 = puVar6 + 1;
    *puVar7 = uVar4;
    puVar7[1] = uVar5;
    puVar7 = puVar7 + 2;
  } while (iVar2 != 4);
  uVar4 = lv_obj_get_child(param_1,0);
  uVar4 = lv_obj_get_child(uVar4,0);
  lv_obj_set_style_text_color(uVar4,0xffffffff,0);
  FUN_00437890(uVar4,0x47,2);
  uVar4 = lv_obj_get_child(param_1,1);
  uVar4 = lv_img_create(uVar4);
  lv_img_set_src(uVar4,&DAT_00d17ba0);
  lv_obj_align(uVar4,9,0,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00441640 @ 00441640 ===== */

void FUN_00441640(void)

{
  undefined2 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  uint uVar10;
  undefined *puVar11;
  undefined **ppuVar12;
  undefined4 *puVar13;
  undefined4 local_a0;
  undefined1 *local_9c;
  code *local_98;
  undefined4 local_94;
  undefined1 local_90;
  undefined1 local_8f;
  code *local_8c;
  short local_88;
  undefined2 local_86;
  undefined1 local_84;
  undefined1 local_83;
  code *local_80;
  short local_7c;
  undefined2 local_7a;
  undefined1 local_78;
  undefined1 local_77;
  code *local_74;
  short local_70;
  undefined2 local_6e;
  undefined1 local_6c;
  undefined1 local_6b;
  code *local_68;
  short local_64;
  undefined2 local_62;
  undefined1 local_60;
  undefined1 local_5f;
  code *local_5c;
  short local_58;
  undefined2 local_56;
  undefined1 local_54;
  undefined1 local_53;
  code *local_50;
  undefined *local_4c [4];
  undefined *puStack_3c;
  undefined4 local_38;
  uint local_34;
  int local_30;
  
  FUN_00428400();
  DAT_0121d3ac = lv_obj_create(0);
  lv_obj_set_style_bg_opa(DAT_0121d3ac,0,0);
  uVar2 = lv_obj_create(DAT_0121d3ac);
  lv_obj_set_size(uVar2,0x2064,0x2064);
  lv_obj_align(uVar2,9,0,0);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_pad_row(uVar2,0,0);
  lv_obj_set_style_pad_column(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  DAT_0121d3a8 = uVar2;
  lv_obj_set_style_bg_opa(uVar2,0x7f,0);
  lv_obj_set_style_bg_color(DAT_0121d3a8,0xff202040,0);
  uVar2 = lv_obj_create(DAT_0121d3ac);
  lv_obj_set_size(uVar2,0x2064,0x2064);
  lv_obj_align(uVar2,9,0,0);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_pad_row(uVar2,0,0);
  lv_obj_set_style_pad_column(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  DAT_0121d398 = uVar2;
  lv_obj_set_style_bg_opa(uVar2,0,0);
  lv_obj_add_event_cb(DAT_0121d3ac,&LAB_004389d8,0x27,0);
  lv_obj_add_event_cb(DAT_0121d3ac,&LAB_004389d8,0x28,0);
  lv_obj_add_event_cb(DAT_0121d3ac,&LAB_004389d8,0x25,0);
  DAT_0121d3a4 = lv_group_create();
  local_38 = lv_group_get_default();
  lv_group_set_default(DAT_0121d3a4);
  uVar2 = lv_tabview_create(DAT_0121d3a8,4,0x2008);
  DAT_0121d3ec = uVar2;
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_border_width(DAT_0121d3ec,0,0);
  lv_obj_set_style_outline_width(DAT_0121d3ec,0,0);
  lv_obj_set_style_bg_opa(DAT_0121d3ec,0x7f,0);
  lv_obj_set_size(DAT_0121d3ec,0x2064,0x205d);
  lv_obj_set_style_bg_color(DAT_0121d3ec,0xff000000,0);
  lv_obj_set_pos(DAT_0121d3ec,0,0);
  lv_tabview_add_tab(DAT_0121d3ec,"#ffffff <#");
  DAT_0121d3f0 = lv_tabview_add_tab(DAT_0121d3ec," ");
  DAT_0121d3f4 = lv_tabview_add_tab(DAT_0121d3ec," ");
  DAT_0121d3f8 = lv_tabview_add_tab(DAT_0121d3ec," ");
  lv_tabview_add_tab(DAT_0121d3ec,"#ffffff >#");
  FUN_00441084(DAT_0121d3f0);
  uVar2 = DAT_0121d3f4;
  iVar3 = FUN_00428454(8);
  iVar4 = FUN_00428454(0x3a);
  local_34 = (uint)(iVar4 == 0);
  iVar4 = FUN_00428454(9);
  local_a0 = (undefined *)0x0;
  local_9c = (undefined1 *)0x0;
  local_98 = (code *)0x0;
  local_94 = 0;
  FUN_0042cbfc("/proc/device-tree/hcrtos/spdif-out/status",&local_a0,0x10);
  iVar5 = strcmp((char *)&local_a0,"okay");
  iVar6 = FUN_00428454(0xc);
  if (iVar6 == 1) {
    DAT_0121478c = 0x7e;
  }
  iVar6 = FUN_00428454(9);
  if (iVar6 == 0) {
    local_30 = 0x7d;
  }
  else {
    iVar6 = FUN_00428454(10);
    local_30 = (int)*(short *)(&DAT_01216550 + iVar6 * 2);
  }
  local_a0 = (undefined *)CONCAT22(local_a0._2_2_,0x13);
  iVar6 = FUN_00428454(8);
  local_9c = (undefined1 *)((uint)local_9c & 0xffff0000);
  local_94 = CONCAT22(local_94._2_2_,0x14);
  local_a0 = (undefined *)
             CONCAT22((short)*(undefined4 *)(&DAT_012165c0 + iVar6 * 8),(undefined2)local_a0);
  local_98 = FUN_0043dd5c;
  uVar1 = FUN_00428454(0xf);
  local_94 = CONCAT22(uVar1,(undefined2)local_94);
  local_90 = 1;
  local_88 = 0x15;
  local_8c = (code *)&LAB_0043f098;
  local_8f = iVar3 != 4;
  local_86 = FUN_00428454(0x10);
  local_7c = 0x16;
  local_84 = 1;
  local_80 = FUN_0043e9dc;
  local_83 = iVar3 != 4;
  local_7a = FUN_00428454(0xb);
  local_78 = 1;
  local_70 = 0x106;
  local_77 = 0;
  local_64 = 0x118;
  local_6c = 0;
  local_6b = 0;
  local_74 = (code *)&LAB_00439e90;
  local_6e = (undefined2)local_30;
  local_68 = (code *)&LAB_00438f9c;
  iVar3 = FUN_00428454(0x3a);
  local_60 = 0;
  local_5c = FUN_0043d71c;
  local_62 = (undefined2)*(undefined4 *)(&DAT_012165a8 + iVar3 * 8);
  local_58 = 0x11a;
  local_5f = iVar5 != 0;
  iVar3 = FUN_00428454(0x3b);
  local_54 = 0;
  local_53 = (undefined1)local_34;
  local_50 = FUN_0043c45c;
  local_56 = (undefined2)*(undefined4 *)(&DAT_01216588 + iVar3 * 8);
  if (iVar4 == 0) {
    uVar7 = FUN_00428454(8);
    uVar8 = FUN_00428454(0xf);
    uVar9 = FUN_00428454(0x10);
    FUN_0044a334(uVar7,uVar8,uVar9);
  }
  uVar7 = FUN_00428454(0xb);
  FUN_0044a448(uVar7);
  FUN_0044a614();
  uVar7 = FUN_00440d54(uVar2,&local_a0);
  uVar10 = (uint)local_a0 & 0xffff;
  if (uVar10 == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,0);
    FUN_0044a328(uVar8);
  }
  else if (uVar10 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,0);
    DAT_0121edd0 = 0xf;
  }
  else if (uVar10 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,0);
    DAT_0121edd8 = 0x10;
  }
  uVar10 = local_94 & 0xffff;
  if (uVar10 == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,1);
    FUN_0044a328(uVar8);
  }
  else if (uVar10 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,1);
    DAT_0121edd0 = 0xf;
  }
  else if (uVar10 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,1);
    DAT_0121edd8 = 0x10;
  }
  if (local_88 == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,2);
    FUN_0044a328(uVar8);
  }
  else if (local_88 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,2);
    DAT_0121edd0 = 0xf;
  }
  else if (local_88 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,2);
    DAT_0121edd8 = 0x10;
  }
  if (local_7c == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,3);
    FUN_0044a328(uVar8);
  }
  else if (local_7c == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,3);
    DAT_0121edd0 = 0xf;
  }
  else if (local_7c == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,3);
    DAT_0121edd8 = 0x10;
  }
  if (local_70 == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,4);
    FUN_0044a328(uVar8);
  }
  else if (local_70 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,4);
    DAT_0121edd0 = 0xf;
  }
  else if (local_70 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,4);
    DAT_0121edd8 = 0x10;
  }
  if (local_64 == 0x11a) {
    uVar8 = lv_obj_get_child(uVar7,5);
    FUN_0044a328(uVar8);
  }
  else if (local_64 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,5);
    DAT_0121edd0 = 0xf;
  }
  else if (local_64 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,5);
    DAT_0121edd8 = 0x10;
  }
  if (local_58 == 0x11a) {
    uVar7 = lv_obj_get_child(uVar7,6);
    FUN_0044a328(uVar7);
  }
  else if (local_58 == 0x14) {
    DAT_0121edcc = lv_obj_get_child(uVar7,6);
    DAT_0121edd0 = 0xf;
  }
  else if (local_58 == 0x15) {
    DAT_0121edd4 = lv_obj_get_child(uVar7,6);
    DAT_0121edd8 = 0x10;
  }
  uVar7 = lv_obj_get_child(uVar2,0);
  uVar7 = lv_obj_get_child(uVar7,0);
  lv_obj_set_style_text_color(uVar7,0xffffffff,0);
  FUN_00437890(uVar7,0x48,2);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_img_create(uVar2);
  lv_img_set_src(uVar2,&DAT_00cf6640);
  lv_obj_align(uVar2,9,0,0);
  uVar2 = DAT_0121d3f8;
  ppuVar12 = local_4c;
  local_a0 = &UNK_00dc0031;
  local_9c = (undefined1 *)((uint)local_9c & 0xffff0000);
  local_94 = CONCAT22(local_94._2_2_,0x32);
  local_98 = FUN_0043b7e0;
  iVar3 = FUN_00428454(0x14);
  local_86 = 0xffff;
  local_7a = 0xffff;
  local_90 = 0;
  local_8f = 0;
  local_84 = 0;
  local_83 = 0;
  local_88 = 0x34;
  local_78 = 0;
  local_7c = 0x35;
  local_77 = 0;
  local_70 = 0x39;
  local_8c = FUN_0043b1b0;
  local_94 = CONCAT22((short)*(undefined4 *)(&DAT_0121477c + iVar3 * 2),(undefined2)local_94);
  local_80 = FUN_0043ab70;
  local_74 = FUN_0043ca9c;
  iVar3 = FUN_00428454(0x19);
  local_6c = 0;
  local_6b = 0;
  local_64 = 0x3a;
  local_6e = (undefined2)*(undefined4 *)(&DAT_01216694 + iVar3 * 8);
  local_68 = FUN_0043e39c;
  iVar3 = FUN_00428454(0x15);
  local_56 = 0xffff;
  local_60 = 0;
  local_5f = 0;
  local_54 = 0;
  local_58 = 0x46;
  local_62 = (undefined2)*(undefined4 *)(&DAT_012147d0 + iVar3 * 8);
  local_53 = 1;
  local_5c = FUN_0043d0dc;
  local_50 = FUN_0043a53c;
  uVar7 = FUN_00440d54(uVar2,&local_a0);
  uVar8 = lv_obj_get_child(uVar7,6);
  uVar8 = lv_obj_get_child(uVar8,1);
  uVar9 = FUN_0042840c();
  lv_label_set_text(uVar8,uVar9);
  uVar7 = lv_obj_get_child(uVar7,6);
  uVar7 = lv_obj_get_child(uVar7,1);
  lv_obj_set_style_text_font(uVar7,lv_font_montserrat_26,0);
  uVar7 = lv_obj_get_child(uVar2,0);
  uVar7 = lv_obj_get_child(uVar7,0);
  lv_obj_set_style_text_color(uVar7,0xffffffff,0);
  FUN_00437890(uVar7,0x49,2);
  uVar2 = lv_obj_get_child(uVar2,1);
  uVar2 = lv_img_create(uVar2);
  lv_img_set_src(uVar2,&DAT_00cd68e0);
  lv_obj_align(uVar2,9,0,0);
  iVar3 = lv_disp_get_hor_res(0);
  DAT_0121683c = lv_tabview_get_tab_btns(DAT_0121d3ec);
  lv_group_focus_obj(DAT_0121683c);
  lv_btnmatrix_set_btn_ctrl_all(DAT_0121683c,0x1000);
  lv_btnmatrix_clear_btn_ctrl_all(DAT_0121683c,0x40);
  uVar2 = DAT_0121683c;
  iVar3 = (int)(short)((short)(iVar3 / 0xb) * 4);
  lv_obj_set_style_pad_left(DAT_0121683c,iVar3,0);
  lv_obj_set_style_pad_right(uVar2,iVar3,0);
  lv_obj_set_style_bg_color(DAT_0121683c,0xff202040,0);
  puVar13 = &local_a0;
  lv_obj_add_event_cb(DAT_0121683c,&LAB_0043f754,0,0);
  DAT_01216838 = lv_obj_create(DAT_0121d3a8);
  lv_obj_set_size(DAT_01216838,0x2064,0x2008);
  lv_obj_align(DAT_01216838,5,0,0);
  lv_obj_set_style_bg_color(DAT_01216838,0xff202040,0);
  lv_obj_set_style_bg_opa(DAT_01216838,0xff,0);
  lv_obj_set_style_border_width(DAT_01216838,2,0);
  lv_obj_set_style_border_side(DAT_01216838,1,0);
  lv_obj_set_style_border_color(DAT_01216838,0xff202040,0);
  lv_obj_set_style_border_opa(DAT_01216838,0xff,0);
  lv_obj_set_style_outline_width(DAT_01216838,0,0);
  uVar2 = DAT_01216838;
  lv_obj_set_style_pad_left(DAT_01216838,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  uVar2 = DAT_01216838;
  lv_obj_set_style_pad_row(DAT_01216838,0,0);
  lv_obj_set_style_pad_column(uVar2,0,0);
  lv_obj_set_style_radius(DAT_01216838,0,0);
  lv_obj_set_flex_flow(DAT_01216838,0);
  local_4c[0] = &DAT_00d3b520;
  local_4c[1] = &DAT_00d39c00;
  local_4c[2] = &DAT_00c6b980;
  local_4c[3] = &DAT_00c09200;
  local_a0 = (undefined *)0x4d;
  local_9c = (undefined1 *)0x4c;
  local_98 = (code *)0x4b;
  local_94 = 0x4e;
  do {
    uVar2 = lv_obj_create(DAT_01216838);
    lv_obj_set_size(uVar2,0x2018,0x2064);
    lv_obj_set_style_pad_top(uVar2,2,0);
    lv_obj_set_style_pad_bottom(uVar2,2,0);
    lv_obj_set_flex_flow(uVar2,0);
    lv_obj_set_flex_align(uVar2,2,2,2);
    lv_obj_set_style_pad_left(uVar2,0,0);
    lv_obj_set_style_pad_right(uVar2,0,0);
    lv_obj_set_style_border_width(uVar2,0,0);
    lv_obj_set_style_outline_width(uVar2,0,0);
    lv_obj_set_style_radius(uVar2,0,0);
    lv_obj_set_scrollbar_mode(uVar2,0);
    lv_obj_set_style_bg_opa(uVar2,0,0);
    uVar7 = lv_img_create(uVar2);
    puVar11 = *ppuVar12;
    ppuVar12 = ppuVar12 + 1;
    lv_img_set_src(uVar7,puVar11);
    uVar2 = lv_label_create(uVar2);
    lv_obj_align_to(uVar2,uVar7,0x14,2,0xfffffff6);
    lv_label_set_recolor(uVar2,1);
    lv_obj_set_style_text_color(uVar2,0xffffffff,0);
    uVar1 = *(undefined2 *)puVar13;
    puVar13 = puVar13 + 1;
    FUN_00437890(uVar2,uVar1,2);
  } while (&puStack_3c != ppuVar12);
  lv_group_set_default(local_38);
  FUN_0044b060();
  local_a0 = (undefined *)DAT_0121d3ac;
  local_9c = &LAB_004387a0;
  FUN_0042cef8(&local_a0);
  return;
}



/* ===== FUNCTION FUN_00442630 @ 00442630 ===== */

undefined4 FUN_00442630(undefined4 param_1,undefined2 param_2,int param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar2 = lv_obj_create();
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  lv_obj_set_style_outline_width(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  uVar1 = (ushort)param_3;
  if (param_3 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(uVar2,0x2064,(int)(short)(uVar1 | 0x2000));
  lv_obj_set_style_border_width(uVar2,0,0);
  lv_obj_set_style_bg_color(uVar2,0xff202040,0);
  lv_obj_set_scrollbar_mode(uVar2,0);
  uVar3 = lv_label_create(uVar2);
  lv_label_set_recolor(uVar3,1);
  lv_obj_set_style_text_color(uVar3,0xffffffff,0);
  FUN_00437890(uVar3,param_2,2);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_align(uVar3,9,0,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00442788 @ 00442788 ===== */

void FUN_00442788(undefined4 param_1,int param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  
  uVar2 = lv_obj_create();
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(uVar2,0x2064,(int)(short)(uVar1 | 0x2000));
  lv_obj_set_flex_flow(uVar2,0);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_pad_row(uVar2,0,0);
  lv_obj_set_style_pad_column(uVar2,0,0);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  lv_obj_set_style_outline_width(uVar2,0,0);
  lv_obj_set_style_bg_color(uVar2,0xff202040,0);
  iVar5 = 1;
  lv_obj_set_style_radius(uVar2,0,0);
  do {
    uVar3 = lv_obj_create(uVar2);
    lv_obj_set_size(uVar3,0x2032,0x2064);
    lv_obj_set_style_pad_left(uVar3,0,0);
    lv_obj_set_style_pad_right(uVar3,0,0);
    lv_obj_set_style_border_width(uVar3,0,0);
    lv_obj_set_style_outline_width(uVar3,0,0);
    lv_obj_set_style_pad_top(uVar3,0,0);
    lv_obj_set_style_pad_bottom(uVar3,0,0);
    lv_obj_set_style_radius(uVar3,0,0);
    lv_obj_set_style_bg_opa(uVar3,0,0);
    uVar3 = lv_obj_create(uVar3);
    lv_obj_remove_style(uVar3,0,0xfffff);
    lv_obj_set_style_bg_opa(uVar3,0,0);
    lv_obj_align(uVar3,9,0,0);
    lv_obj_set_style_radius(uVar3,0,0);
    lv_obj_set_style_pad_left(uVar3,0,0);
    lv_obj_set_style_pad_right(uVar3,0,0);
    lv_obj_set_style_border_width(uVar3,0,0);
    lv_obj_set_style_outline_width(uVar3,0,0);
    lv_obj_set_style_pad_top(uVar3,0,0);
    lv_obj_set_style_pad_bottom(uVar3,0,0);
    lv_obj_set_size(uVar3,0x204b,0x2064);
    uVar4 = lv_img_create(uVar3);
    lv_obj_align(uVar4,7,0,0);
    if (iVar5 == 1) {
      lv_img_set_src(uVar4,&DAT_00c6b980);
      uVar3 = lv_label_create(uVar3);
      lv_obj_set_size(uVar3,0x27d1,0x27d1);
      FUN_00437890(uVar3,0x4b,2);
      lv_obj_set_style_text_color(uVar3,0xffffffff,0);
      lv_obj_align_to(uVar3,uVar4,0x14,5,0xfffffffd);
    }
    else {
      lv_img_set_src(uVar4,&DAT_00c09200);
      uVar3 = lv_label_create(uVar3);
      lv_obj_set_size(uVar3,0x27d1,0x27d1);
      FUN_00437890(uVar3,0x4c,2);
      lv_obj_set_style_text_color(uVar3,0xffffffff,0);
      lv_obj_align_to(uVar3,uVar4,0x14,5,0xfffffffd);
      if (iVar5 == 2) {
        return;
      }
    }
    iVar5 = iVar5 + 1;
  } while( true );
}



/* ===== FUNCTION FUN_00442b5c @ 00442b5c ===== */

undefined4 FUN_00442b5c(int param_1,int param_2)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  
  uVar3 = lv_obj_create(DAT_0121d398);
  uVar1 = (ushort)param_1;
  uVar2 = (ushort)param_2;
  if (param_1 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_2 < 0) {
    uVar2 = 1000 - uVar2;
  }
  DAT_0121d390 = uVar3;
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_align(uVar3,9,0,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_style_radius(uVar3,0x14,0);
  lv_obj_set_style_pad_row(uVar3,0,0);
  lv_obj_set_style_pad_column(uVar3,0,0);
  lv_obj_set_style_text_opa(uVar3,0xff,0);
  lv_obj_set_style_img_opa(uVar3,0xff,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_shadow_width(uVar3,0,0);
  lv_obj_set_flex_flow(uVar3,1);
  lv_obj_set_style_bg_color(uVar3,0xff000000,0);
  return uVar3;
}



/* ===== FUNCTION FUN_00442cf0 @ 00442cf0 ===== */

undefined4
FUN_00442cf0(undefined4 param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  
  uVar3 = lv_btnmatrix_create();
  lv_group_focus_obj(uVar3);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  FUN_00447298(uVar3,param_5,2,param_4);
  lv_btnmatrix_set_btn_ctrl_all(uVar3,0x40);
  lv_btnmatrix_set_one_checked(uVar3,1);
  lv_style_init(&DAT_0121d3e4);
  lv_style_set_pad_left(&DAT_0121d3e4,0);
  lv_style_set_pad_right(&DAT_0121d3e4,0);
  lv_style_set_pad_top(&DAT_0121d3e4,0);
  lv_style_set_pad_bottom(&DAT_0121d3e4,0);
  lv_style_set_pad_row(&DAT_0121d3e4,0);
  lv_style_set_pad_column(&DAT_0121d3e4,0);
  lv_style_set_border_width(&DAT_0121d3e4,0);
  lv_style_set_outline_width(&DAT_0121d3e4,0);
  lv_obj_add_style(uVar3,&DAT_0121d3e4,0);
  lv_style_init(&DAT_0121d3dc);
  lv_style_set_radius(&DAT_0121d3dc,0);
  lv_style_set_border_width(&DAT_0121d3dc,1);
  lv_style_set_border_opa(&DAT_0121d3dc,0x7f);
  lv_style_set_border_color(&DAT_0121d3dc,0xff8c8cc6);
  lv_style_set_bg_color(&DAT_0121d3dc,0xff6565b1);
  lv_style_set_border_side(&DAT_0121d3dc,0x10);
  lv_obj_add_style(uVar3,&DAT_0121d3dc,0x50000);
  return uVar3;
}



/* ===== FUNCTION FUN_00442ea4 @ 00442ea4 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00442ea4(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char *__s1;
  char *__s2;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  
  iVar8 = 0;
  uVar1 = FUN_00428454(6);
  uVar2 = FUN_00442b5c(0x21,0x32);
  FUN_00442630(uVar2,0x12,0xf);
  uVar3 = FUN_00442cf0(uVar2,100,0x48,&DAT_01214790,0xc);
  uVar4 = lv_obj_get_child(param_1,1);
  __s1 = (char *)lv_label_get_text(uVar4);
  piVar7 = &DAT_01214790;
  do {
    while( true ) {
      iVar6 = *piVar7;
      piVar7 = piVar7 + 1;
      if ((1 < iVar6 + 2U) && (iVar6 != -3)) break;
LAB_00442f58:
      iVar8 = iVar8 + 1;
      if (iVar8 == 0xc) goto LAB_00442fd8;
    }
    __s2 = (char *)FUN_004386cc(iVar6);
    iVar6 = strcmp(__s1,__s2);
    uVar5 = iVar8 >> 1;
    if (iVar6 != 0) goto LAB_00442f58;
    lv_btnmatrix_set_selected_btn(uVar3,uVar5 & 0xffff);
    lv_btnmatrix_set_btn_ctrl(uVar3,uVar5 & 0xffff,0x80);
    iVar8 = iVar8 + 1;
    _DAT_0121d394 = uVar5;
  } while (iVar8 != 0xc);
LAB_00442fd8:
  *(undefined4 *)(param_1 + 0x10) = uVar1;
  FUN_00442788(uVar2,0xe,param_1);
  uVar1 = lv_obj_get_child(uVar2,1);
  lv_obj_add_event_cb(uVar1,&LAB_00439b68,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044303c @ 0044303c ===== */

void FUN_0044303c(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_004432b0:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(6);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_0044351c:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_004432d8:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 6;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(6,iVar5);
      goto LAB_00443144;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_004432b0;
      iVar5 = FUN_00428454(6);
      goto LAB_0044351c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        lv_obj_add_flag(DAT_0121d3a8,1);
        FUN_00442ea4(iVar2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(6);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_00443144;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_004432b0;
      iVar5 = FUN_00428454(6);
      goto LAB_004432d8;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_00443144:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_0044367c @ 0044367c ===== */

undefined4 FUN_0044367c(undefined4 param_1,int param_2,int param_3,undefined4 param_4)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  
  uVar3 = lv_btnmatrix_create();
  lv_group_focus_obj(uVar3);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_btnmatrix_set_map(uVar3,param_4);
  lv_btnmatrix_set_btn_ctrl_all(uVar3,0x40);
  lv_btnmatrix_set_one_checked(uVar3,1);
  lv_style_init(&DAT_0121d3e4);
  lv_style_set_pad_left(&DAT_0121d3e4,0);
  lv_style_set_pad_right(&DAT_0121d3e4,0);
  lv_style_set_pad_top(&DAT_0121d3e4,0);
  lv_style_set_pad_bottom(&DAT_0121d3e4,0);
  lv_style_set_pad_row(&DAT_0121d3e4,0);
  lv_style_set_pad_column(&DAT_0121d3e4,0);
  lv_style_set_border_width(&DAT_0121d3e4,0);
  lv_style_set_outline_width(&DAT_0121d3e4,0);
  lv_obj_add_style(uVar3,&DAT_0121d3e4,0);
  lv_style_init(&DAT_0121d3dc);
  lv_style_set_radius(&DAT_0121d3dc,0);
  lv_style_set_border_width(&DAT_0121d3dc,1);
  lv_style_set_border_opa(&DAT_0121d3dc,0x7f);
  lv_style_set_border_color(&DAT_0121d3dc,0xff8c8cc6);
  lv_style_set_bg_color(&DAT_0121d3dc,0xff6565b1);
  lv_style_set_border_side(&DAT_0121d3dc,0x10);
  lv_obj_add_style(uVar3,&DAT_0121d3dc,0x50000);
  return uVar3;
}



/* ===== FUNCTION FUN_00443828 @ 00443828 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_00443828(int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,int param_6,
            int param_7)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  char *__s1;
  char *__s2;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = 0x21;
  if (0 < param_6) {
    iVar6 = param_6;
  }
  iVar5 = 0x32;
  if (0 < param_7) {
    iVar5 = param_7;
  }
  uVar1 = FUN_00442b5c(iVar6,iVar5);
  FUN_00442630(uVar1,param_2,0xf);
  uVar2 = FUN_00442cf0(uVar1,100,0x48,param_3,param_5);
  uVar3 = lv_obj_get_child(param_1,1);
  __s1 = (char *)lv_label_get_text(uVar3);
  if (0 < param_5) {
    iVar6 = 0;
    do {
      while( true ) {
        iVar5 = *param_3;
        param_3 = param_3 + 1;
        if ((1 < iVar5 + 2U) && (iVar5 != -3)) break;
LAB_004438ec:
        iVar6 = iVar6 + 1;
        if (param_5 == iVar6) goto LAB_0044396c;
      }
      __s2 = (char *)FUN_004386cc(iVar5);
      iVar5 = strcmp(__s1,__s2);
      uVar4 = iVar6 >> 1;
      if (iVar5 != 0) goto LAB_004438ec;
      lv_btnmatrix_set_selected_btn(uVar2,uVar4 & 0xffff);
      lv_btnmatrix_set_btn_ctrl(uVar2,uVar4 & 0xffff,0x80);
      iVar6 = iVar6 + 1;
      _DAT_0121d394 = uVar4;
    } while (param_5 != iVar6);
  }
LAB_0044396c:
  *(undefined4 *)(param_1 + 0x10) = param_4;
  FUN_00442788(uVar1,0xe,param_1);
  return uVar1;
}



/* ===== FUNCTION FUN_004439b4 @ 004439b4 ===== */

uint FUN_004439b4(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int *piVar5;
  
  uVar2 = 0;
  piVar5 = &DAT_00455cf8;
  do {
    iVar4 = *piVar5;
    uVar2 = uVar2 + 1;
    iVar1 = FUN_00428454(0x14,param_2);
    piVar5 = piVar5 + 1;
    if (iVar4 == iVar1) {
      uVar2 = uVar2 & 3;
      uVar3 = (&DAT_00455cf8)[uVar2];
      goto LAB_00443a0c;
    }
    param_2 = 0;
  } while (uVar2 != 4);
  uVar3 = 0;
  uVar2 = 0;
LAB_00443a0c:
  FUN_004286f8(0x14);
  FUN_00433d28(uVar3);
  return uVar2;
}



/* ===== FUNCTION FUN_00443a5c @ 00443a5c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00443a5c(undefined4 param_1,code *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  char *pcVar5;
  uint uVar6;
  uint uVar7;
  
  iVar1 = lv_event_get_target();
  iVar2 = lv_event_get_code(param_1);
  iVar3 = lv_event_get_user_data(param_1);
  if (iVar2 != 0xd) {
    if (iVar2 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    return;
  }
  uVar7 = *(uint *)(iVar3 + 0x10);
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar4 = lv_indev_get_act();
  iVar2 = lv_indev_get_key(uVar4);
  if (iVar2 == 10) {
    iVar2 = lv_btnmatrix_get_selected_btn(iVar1);
    uVar4 = lv_btnmatrix_get_btn_text(iVar1,iVar2);
    iVar3 = lv_obj_get_child(iVar3,1);
    *(uint *)(iVar3 + 0x10) =
         *(int *)(*(int *)(*(int *)(iVar1 + 0x10) + 8) + iVar2 * 8) << 4 |
         *(uint *)(iVar3 + 0x10) & 0xf;
    lv_label_set_text(iVar3,uVar4);
    if (param_2 != (code *)0x0) {
      (*param_2)(iVar2);
    }
  }
  else {
    if (iVar2 - 0x11U < 2) {
      if (iVar2 == 0x12) {
        _DAT_0121d394 = _DAT_0121d394 + 1;
        if ((int)_DAT_0121d394 < (int)(uint)(**(ushort **)(iVar1 + 0x10) >> 1)) {
          pcVar5 = (char *)lv_btnmatrix_get_btn_text(iVar1,_DAT_0121d394 & 0xffff);
          if (*pcVar5 != ' ') goto LAB_00443c04;
          uVar6 = _DAT_0121d394 & 0xffff;
          if (pcVar5[1] == '\0') goto LAB_00443cb8;
        }
        else {
LAB_00443cb8:
          lv_btnmatrix_set_selected_btn(iVar1,0);
          uVar6 = 0;
          _DAT_0121d394 = 0;
        }
        lv_btnmatrix_set_selected_btn(iVar1,uVar6);
        lv_btnmatrix_set_btn_ctrl(iVar1,_DAT_0121d394 & 0xffff,0x80);
        if (param_2 == (code *)0x0) goto LAB_00443bb0;
      }
      else {
        _DAT_0121d394 = _DAT_0121d394 - 1;
        if ((int)_DAT_0121d394 < 0) {
          _DAT_0121d394 = (uint)(**(ushort **)(iVar1 + 0x10) >> 1);
          do {
            _DAT_0121d394 = _DAT_0121d394 - 1;
            pcVar5 = (char *)lv_btnmatrix_get_btn_text(iVar1,_DAT_0121d394 & 0xffff);
            if (*pcVar5 != ' ') break;
          } while (pcVar5[1] == '\0');
        }
LAB_00443c04:
        lv_btnmatrix_set_selected_btn(iVar1,_DAT_0121d394 & 0xffff);
        lv_btnmatrix_set_btn_ctrl(iVar1,_DAT_0121d394 & 0xffff,0x80);
        if (param_2 == (code *)0x0) goto LAB_00443b30;
      }
      (*param_2)(_DAT_0121d394);
    }
LAB_00443b30:
    if (iVar2 - 0x13U < 2) {
      lv_btnmatrix_set_selected_btn(iVar1,_DAT_0121d394 & 0xffff);
      goto LAB_00443bb0;
    }
    if ((iVar2 != 2) && (iVar2 != 0x1b)) goto LAB_00443bb0;
    if ((uVar7 != _DAT_0121d394) && (param_2 != (code *)0x0)) {
      (*param_2)(uVar7);
    }
  }
  lv_obj_del(*(undefined4 *)(iVar1 + 4));
  if (DAT_0121d3ac != 0) {
    lv_group_focus_obj(DAT_0121d3fc);
  }
  DAT_0121d390 = 0;
  lv_obj_clear_flag(DAT_0121d3a8,1);
  FUN_00428080();
LAB_00443bb0:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_00443e30 @ 00443e30 ===== */

void FUN_00443e30(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = lv_event_get_code();
  if (iVar1 != 0x1a) {
    return;
  }
  iVar1 = lv_event_get_param(param_1);
  *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
  *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
  return;
}



/* ===== FUNCTION FUN_00443e84 @ 00443e84 ===== */

void FUN_00443e84(void)

{
  if (DAT_0121d3ac != 0) {
    lv_group_focus_obj(DAT_0121d3fc);
  }
  DAT_0121d390 = 0;
  lv_obj_clear_flag(DAT_0121d3a8,1);
  FUN_00428080();
  return;
}



/* ===== FUNCTION FUN_00443ec8 @ 00443ec8 ===== */

void FUN_00443ec8(void)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00428454(0x13);
  FUN_004067ac(uVar1);
  FUN_00428080();
  return;
}



/* ===== FUNCTION FUN_00443eec @ 00443eec ===== */

undefined4 FUN_00443eec(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 local_20 [3];
  
  puts("msgbox");
  if (DAT_0121d3cc != 0) {
    lv_obj_del();
  }
  if (DAT_0121d3d0 != 0) {
    lv_timer_del();
  }
  uVar1 = lv_disp_get_default();
  uVar1 = lv_disp_get_layer_top(uVar1);
  uVar1 = lv_obj_create(uVar1);
  DAT_0121d3cc = uVar1;
  lv_obj_set_style_text_color(uVar1,0xffffffff,0);
  lv_obj_set_size(uVar1,0x27d1,0x200a);
  lv_obj_align(uVar1,9,0,0);
  lv_palette_darken(local_20,0x12,1);
  lv_obj_set_style_bg_color(uVar1,local_20[0],0);
  lv_obj_set_scrollbar_mode(uVar1,0);
  lv_obj_set_style_bg_opa(uVar1,0xb2,0);
  lv_obj_set_style_border_width(uVar1,1,0);
  lv_palette_main(local_20,5);
  lv_obj_set_style_border_color(uVar1,local_20[0],0);
  lv_obj_set_style_outline_width(uVar1,0,0);
  lv_obj_set_style_radius(uVar1,10,0);
  uVar2 = lv_label_create(uVar1);
  lv_obj_align(uVar2,9,0,0);
  lv_label_set_text(uVar2,param_1);
  uVar3 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar2,uVar3,0);
  uVar2 = lv_timer_create(FUN_00438964,3000,uVar1);
  DAT_0121d3d0 = uVar2;
  lv_timer_set_repeat_count(uVar2,1);
  lv_timer_reset(uVar2);
  return uVar1;
}



/* ===== FUNCTION FUN_004440b0 @ 004440b0 ===== */

undefined4
FUN_004440b0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5
            ,int param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  short sVar6;
  undefined4 local_20 [2];
  
  PTR_s__01214770 = (undefined *)FUN_004386cc(param_2);
  PTR_s__01214774 = (undefined *)FUN_004386cc(param_3);
  uVar1 = lv_disp_get_default();
  uVar1 = lv_disp_get_layer_top(uVar1);
  uVar2 = FUN_004386cc(param_1);
  uVar1 = lv_msgbox_create(uVar1,0,uVar2,&PTR_s__01214770,0);
  lv_obj_set_flex_flow(uVar1,1);
  lv_obj_set_flex_align(uVar1,2,2,2);
  lv_palette_darken(local_20,0x12,1);
  lv_obj_set_style_bg_color(uVar1,local_20[0],0);
  uVar4 = 0x2019;
  if (param_5 < 1) {
LAB_0044418c:
    uVar5 = 0x2014;
    if (param_6 < 1) goto LAB_004441a4;
  }
  else {
    sVar6 = (short)param_5;
    if (-1 < sVar6) {
      uVar4 = (int)sVar6 | 0x2000;
      goto LAB_0044418c;
    }
    uVar4 = (uint)(short)(1000U - sVar6 | 0x2000);
    if (param_6 < 1) {
      uVar5 = 0x2014;
      goto LAB_004441a4;
    }
  }
  sVar6 = (short)param_6;
  if (sVar6 < 0) {
    uVar5 = (uint)(short)(1000U - sVar6 | 0x2000);
  }
  else {
    uVar5 = (int)sVar6 | 0x2000;
  }
LAB_004441a4:
  lv_obj_set_size(uVar1,uVar4,uVar5);
  lv_obj_add_event_cb(uVar1,param_4,0,0);
  uVar2 = lv_msgbox_get_btns(uVar1);
  lv_obj_set_width(uVar2,0x2064);
  lv_obj_align(uVar2,5,0,0);
  iVar3 = FUN_0043783c(1);
  lv_obj_set_style_height(uVar2,(int)(short)(*(short *)(iVar3 + 8) + 0xd),0);
  lv_obj_align(uVar1,9,0,0);
  uVar2 = FUN_0043783c(1);
  lv_obj_set_style_text_font(uVar1,uVar2,0);
  uVar2 = lv_msgbox_get_btns(uVar1);
  lv_group_focus_obj(uVar2);
  uVar2 = lv_msgbox_get_btns(uVar1);
  lv_obj_set_style_bg_color(uVar2,0xffffff00,0x50004);
  lv_obj_set_style_text_color(uVar2,0xff000000,0x50004);
  lv_obj_set_style_outline_color(uVar2,0xff0000b0,0x50004);
  lv_obj_set_style_outline_width(uVar2,4,0x50004);
  return uVar1;
}



/* ===== FUNCTION FUN_00444430 @ 00444430 ===== */

undefined4 FUN_00444430(undefined4 param_1,int param_2,int param_3)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar3 = lv_list_create();
  lv_obj_set_style_radius(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  uVar1 = (ushort)param_2;
  uVar2 = (ushort)param_3;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  if (param_3 < 0) {
    uVar2 = 1000 - uVar2;
  }
  lv_obj_set_size(uVar3,(int)(short)(uVar1 | 0x2000),(int)(short)(uVar2 | 0x2000));
  lv_obj_align(uVar3,9,0,0);
  uVar4 = lv_group_get_default();
  lv_group_add_obj(uVar4,uVar3);
  return uVar3;
}



/* ===== FUNCTION FUN_004446c0 @ 004446c0 ===== */

undefined4 FUN_004446c0(undefined4 param_1,int param_2,undefined4 param_3,undefined2 param_4)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 local_18 [2];
  
  uVar2 = lv_list_add_text(param_1," ");
  lv_obj_set_style_text_align(uVar2,2,0);
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(uVar2,(int)(short)(uVar1 | 0x2000),0x27d1);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_border_side(uVar2,0xf,1);
  lv_obj_set_style_border_width(uVar2,2,0);
  lv_obj_set_style_border_color(uVar2,0xffffffff,0);
  lv_obj_set_style_border_opa(uVar2,0,0);
  lv_obj_set_style_border_opa(uVar2,0xff,1);
  lv_obj_set_style_bg_opa(uVar2,0,0);
  lv_obj_set_style_bg_opa(uVar2,0xff,1);
  lv_palette_main(local_18,5);
  lv_obj_set_style_bg_color(uVar2,local_18[0],1);
  lv_obj_set_style_text_color(uVar2,0xffafafaf,0);
  lv_obj_set_style_text_color(uVar2,0xff000000,1);
  lv_obj_set_style_text_align(uVar2,2,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  uVar3 = lv_label_create(uVar2);
  lv_obj_align(uVar3,9,0,0);
  FUN_00437890(uVar3,param_4,2);
  return uVar2;
}



/* ===== FUNCTION FUN_00444868 @ 00444868 ===== */

undefined4 FUN_00444868(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 local_18 [2];
  
  uVar2 = lv_list_add_text(param_1," ");
  lv_obj_set_style_text_align(uVar2,2,0);
  uVar1 = (ushort)param_2;
  if (param_2 < 0) {
    uVar1 = 1000 - uVar1;
  }
  lv_obj_set_size(uVar2,(int)(short)(uVar1 | 0x2000),0x27d1);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_style_border_side(uVar2,0xf,1);
  lv_obj_set_style_border_width(uVar2,2,0);
  lv_obj_set_style_border_color(uVar2,0xffffffff,0);
  lv_obj_set_style_border_opa(uVar2,0,0);
  lv_obj_set_style_border_opa(uVar2,0xff,1);
  lv_obj_set_style_bg_opa(uVar2,0,0);
  lv_obj_set_style_bg_opa(uVar2,0xff,1);
  lv_palette_main(local_18,5);
  lv_obj_set_style_bg_color(uVar2,local_18[0],1);
  lv_obj_set_style_text_color(uVar2,0xffafafaf,0);
  lv_obj_set_style_text_color(uVar2,0xff000000,1);
  lv_label_set_text(uVar2,param_4);
  lv_obj_set_style_text_font(uVar2,lv_font_montserrat_26,0);
  return uVar2;
}



/* ===== FUNCTION FUN_004449e0 @ 004449e0 ===== */

void FUN_004449e0(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  lv_obj_add_flag(DAT_0121d3a8,1);
  uVar1 = FUN_00428454(param_1);
  switch(param_1) {
  default:
    goto switchD_00444a28_caseD_0;
  case 1:
    uVar3 = 10;
    goto LAB_00444a38;
  case 2:
  case 4:
  case 5:
    uVar3 = 0;
LAB_00444a38:
    iVar2 = FUN_00438bb8(uVar1,100,uVar3,100);
    break;
  case 3:
    iVar2 = FUN_00438bb8(uVar1,10,0,10);
    break;
  case 0x39:
    iVar2 = FUN_00438bb8(uVar1,2000,0,2000);
  }
  if (iVar2 != 0) {
    lv_obj_add_event_cb(iVar2,FUN_0043978c,0,param_1);
    lv_group_focus_obj(iVar2);
    return;
  }
switchD_00444a28_caseD_0:
  return;
}



/* ===== FUNCTION FUN_00445104 @ 00445104 ===== */

void FUN_00445104(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_00445378:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(1);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_004455e4:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_004453a0:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 1;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(1,iVar5);
      goto LAB_0044520c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_00445378;
      iVar5 = FUN_00428454(1);
      goto LAB_004455e4;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        FUN_004449e0(1);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(1);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0044520c;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_00445378;
      iVar5 = FUN_00428454(1);
      goto LAB_004453a0;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0044520c:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_00445734 @ 00445734 ===== */

void FUN_00445734(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_004459a8:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(4);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_00445c14:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_004459d0:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 4;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(4,iVar5);
      goto LAB_0044583c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_004459a8;
      iVar5 = FUN_00428454(4);
      goto LAB_00445c14;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        FUN_004449e0(4);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(4);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_0044583c;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_004459a8;
      iVar5 = FUN_00428454(4);
      goto LAB_004459d0;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_0044583c:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_00445d64 @ 00445d64 ===== */

void FUN_00445d64(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  code *pcVar6;
  
  iVar1 = lv_event_get_code();
  iVar2 = lv_event_get_target(param_1);
  if (iVar1 == 1) {
    lv_obj_clear_state(iVar2,0x20);
    return;
  }
  if (iVar1 == 0xf) {
    DAT_0121d3c8 = 0;
    lv_obj_set_style_bg_opa(iVar2,0,0);
    lv_obj_set_style_border_opa(iVar2,0,0);
    iVar1 = lv_obj_has_state(iVar2,0x80);
    if (iVar1 == 0) {
      uVar3 = 0xffffffff;
    }
    else {
      uVar3 = 0xff7d7d7d;
    }
    lv_obj_set_style_text_color(iVar2,uVar3,0);
    return;
  }
  if (iVar1 == 0xe) {
    lv_obj_set_style_bg_opa(iVar2,0xff,0);
    lv_obj_set_style_border_opa(iVar2,0xff,0);
    lv_obj_add_state(iVar2,0x1000);
    lv_obj_set_style_text_color(iVar2,0xff000000,0);
    return;
  }
  if (iVar1 != 0xd) {
    if (iVar1 != 0x1a) {
      return;
    }
    iVar1 = lv_event_get_param(param_1);
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x38) = 0;
    *(undefined2 *)(*(int *)(iVar1 + 0x10) + 0x44) = 0;
    return;
  }
  if (DAT_0121d3a0 != 0) {
    lv_timer_pause();
  }
  uVar3 = lv_indev_get_act();
  iVar1 = lv_indev_get_key(uVar3);
  if (iVar1 - 0x11U < 2) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) {
LAB_00445fd8:
        lv_timer_pause();
      }
      iVar5 = FUN_00428454(2);
      if ((iVar1 == 0x12) || (iVar1 == 0x14)) {
LAB_00446244:
        iVar5 = iVar5 + -1;
        if (iVar5 < 0) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"#ffffff <#  %d   >",iVar5);
      }
      else {
LAB_00446000:
        iVar5 = iVar5 + 1;
        if (0 < iVar5) {
          iVar5 = 0;
        }
        uVar3 = lv_obj_get_child(iVar2,1);
        lv_label_set_text_fmt(uVar3,"<   %d   #ffffff >#",iVar5);
      }
      if (DAT_0121d3d4 == 0) {
        iVar1 = lv_obj_get_child(iVar2,1);
        *(undefined4 *)(iVar1 + 0x10) = 2;
        DAT_0121d3d4 = lv_timer_create(FUN_004387a8,1000,iVar1);
        lv_timer_set_repeat_count(DAT_0121d3d4,1);
        lv_timer_reset(DAT_0121d3d4);
      }
      else {
        lv_timer_reset();
        lv_timer_resume(DAT_0121d3d4);
      }
      FUN_004286f8(2,iVar5);
      goto LAB_00445e6c;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    if (iVar1 - 0x12U < 2) {
      pcVar6 = lv_group_focus_next;
    }
    else {
      pcVar6 = lv_group_focus_prev;
    }
  }
  else if (iVar1 == 0x14) {
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_00445fd8;
      iVar5 = FUN_00428454(2);
      goto LAB_00446244;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_prev;
  }
  else {
    if (iVar1 != 0x13) {
      if (iVar1 == 10) {
        FUN_004449e0(2);
      }
      else if ((iVar1 == 0x1b) || (iVar1 == 2)) {
        iVar1 = lv_group_get_default();
        if ((*(uint *)(iVar1 + 0x1c) & 1) == 0) {
          lv_obj_clear_state(iVar2,0x1000);
          uVar3 = FUN_00428454(0x13);
          FUN_004067ac(uVar3);
          FUN_00428080();
          return;
        }
        lv_group_focus_freeze(DAT_0121d3a4,0);
        lv_obj_set_style_bg_opa(iVar2,0xff,0);
        uVar3 = lv_obj_get_child(iVar2,1);
        uVar4 = FUN_00428454(2);
        lv_label_set_text_fmt(uVar3,&DAT_00452028,uVar4);
        if (DAT_0121d3d4 != 0) {
          lv_timer_pause();
          lv_timer_del(DAT_0121d3d4);
        }
        DAT_0121d3d4 = 0;
      }
      goto LAB_00445e6c;
    }
    if ((*(uint *)(DAT_0121d3a4 + 0x1c) & 1) != 0) {
      if (DAT_0121d3d4 != 0) goto LAB_00445fd8;
      iVar5 = FUN_00428454(2);
      goto LAB_00446000;
    }
    uVar3 = lv_group_get_default();
    lv_obj_clear_state(iVar2,0x1000);
    pcVar6 = lv_group_focus_next;
  }
  (*pcVar6)(uVar3);
  iVar1 = lv_group_get_focused(uVar3);
  uVar4 = lv_tabview_get_tab_act(DAT_0121d3ec);
  while ((((*(int *)(*(int *)(iVar1 + 4) + 4) != *(int *)(*(int *)(iVar2 + 4) + 4) ||
           (iVar5 = lv_obj_has_state(iVar1,0x80), iVar5 != 0)) && (DAT_0121683c != iVar1)) &&
         (iVar1 != DAT_01216838))) {
    lv_obj_clear_state(iVar1,0x1000);
    lv_obj_set_style_bg_opa(iVar1,0,0);
    lv_obj_set_style_border_opa(iVar1,0,0);
    (*pcVar6)(uVar3);
    iVar1 = lv_group_get_focused(uVar3);
  }
  DAT_0121d3fc = iVar1;
  lv_tabview_set_act(DAT_0121d3ec,uVar4,0);
LAB_00445e6c:
  if (DAT_0121d3a0 == 0) {
    return;
  }
  lv_timer_reset();
  lv_timer_resume(DAT_0121d3a0);
  return;
}



/* ===== FUNCTION FUN_00446394 @ 00446394 ===== */

void FUN_00446394(void)

{
  return;
}



/* ===== FUNCTION FUN_004463ac @ 004463ac ===== */

undefined4 FUN_004463ac(uint param_1)

{
  if ((param_1 < 7) && (FUN_004286f8(0x15,param_1), param_1 == 0)) {
    if (DAT_0121d3a0 != 0) {
      DAT_0121d3a0 = 0;
      lv_timer_del();
      return 0xffffffff;
    }
    return 0xffffffff;
  }
  if (DAT_0121d3a0 == 0) {
    DAT_0121d3a0 = lv_timer_create(&LAB_00438e54,param_1 * 5000,0);
    lv_timer_set_repeat_count(DAT_0121d3a0,1);
    lv_timer_reset(DAT_0121d3a0);
  }
  else {
    lv_timer_set_period();
    printf("time: %d",param_1);
  }
  return 0;
}



/* ===== FUNCTION FUN_00446490 @ 00446490 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00446490(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  uVar1 = FUN_00442b5c(0x21,0x38);
  FUN_00442630(uVar1,0x3a,0xe);
  uVar2 = FUN_00442cf0(uVar1,100,0x4a,&DAT_012147d0,0xe);
  _DAT_0121d394 = FUN_00428454(0x15);
  printf("btns choose id: %d \n",_DAT_0121d394);
  lv_btnmatrix_set_selected_btn(uVar2,_DAT_0121d394 & 0xffff);
  lv_btnmatrix_set_btn_ctrl(uVar2,_DAT_0121d394 & 0xffff,0x80);
  uVar3 = FUN_00428454(0x15);
  *(undefined4 *)(param_1 + 0x10) = uVar3;
  FUN_00442788(uVar1,0xd,param_1);
  lv_obj_add_event_cb(uVar2,&LAB_004463a0,0,param_1);
  return;
}



/* ===== FUNCTION FUN_00446580 @ 00446580 ===== */

undefined4 FUN_00446580(int param_1)

{
  int __fd;
  undefined **ppuVar1;
  
  __fd = open("/dev/pq",1);
  if (__fd < 0) {
    return 0xffffffff;
  }
  puts("pq_start");
  if (param_1 == 1) {
    ppuVar1 = &PTR_s_standard_0121483c;
  }
  else if (param_1 == 2) {
    ppuVar1 = &PTR_DAT_01215b8c;
  }
  else {
    if (param_1 != 0) goto LAB_004465d8;
    ppuVar1 = &PTR_DAT_012151e4;
  }
  ioctl(__fd,0x89a82502,ppuVar1);
LAB_004465d8:
  FUN_004286f8(6,param_1);
  FUN_00428080();
  ioctl(__fd,0x20002500);
  close(__fd);
  return 0;
}



/* ===== FUNCTION FUN_00446730 @ 00446730 ===== */

void FUN_00446730(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = lv_event_get_code();
  puVar2 = (undefined4 *)lv_event_get_target(param_1);
  if (iVar1 == 0x21) {
    if ((undefined1 *)*puVar2 == lv_btnmatrix_class) {
      if (puVar2[4] != 0) {
        lv_mem_free(*(undefined4 *)(puVar2[4] + 4));
        lv_mem_free(puVar2[4]);
        puVar2[4] = 0;
      }
    }
    else if (((undefined1 *)*puVar2 == lv_label_class) && (puVar2[4] != 0)) {
      puVar2[4] = 0;
    }
  }
  return;
}



/* ===== FUNCTION FUN_004467c8 @ 004467c8 ===== */

void FUN_004467c8(undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  uint uVar9;
  undefined4 *puVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint local_4c;
  uint local_44;
  uint local_3c;
  uint local_34;
  
  local_3c = 0;
  iVar1 = lv_obj_get_child_cnt();
  if (iVar1 != 0) {
    do {
      puVar2 = (undefined4 *)lv_obj_get_child(param_1,local_3c);
      puVar3 = (undefined1 *)*puVar2;
      if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
         (puVar3 == lv_list_text_class)) {
        lv_event_send(puVar2,0x1e,0);
        iVar1 = lv_obj_get_child_cnt(puVar2);
        if (iVar1 != 0) {
          FUN_004467c8(puVar2);
        }
      }
      else {
        local_34 = 0;
        iVar1 = lv_obj_get_child_cnt(puVar2);
        if (iVar1 != 0) {
          do {
            puVar4 = (undefined4 *)lv_obj_get_child(puVar2,local_34);
            puVar3 = (undefined1 *)*puVar4;
            if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
               (puVar3 == lv_list_text_class)) {
              lv_event_send(puVar4,0x1e,0);
              iVar1 = lv_obj_get_child_cnt(puVar4);
              if (iVar1 != 0) {
                FUN_004467c8(puVar4);
              }
            }
            else {
              local_44 = 0;
              iVar1 = lv_obj_get_child_cnt(puVar4);
              if (iVar1 != 0) {
                do {
                  puVar5 = (undefined4 *)lv_obj_get_child(puVar4,local_44);
                  puVar3 = (undefined1 *)*puVar5;
                  if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
                     (puVar3 == lv_list_text_class)) {
                    lv_event_send(puVar5,0x1e,0);
                    iVar1 = lv_obj_get_child_cnt(puVar5);
                    if (iVar1 != 0) {
                      FUN_004467c8(puVar5);
                    }
                  }
                  else {
                    local_4c = 0;
                    iVar1 = lv_obj_get_child_cnt(puVar5);
                    if (iVar1 != 0) {
                      do {
                        puVar6 = (undefined4 *)lv_obj_get_child(puVar5,local_4c);
                        puVar3 = (undefined1 *)*puVar6;
                        if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
                           (puVar3 == lv_list_text_class)) {
                          lv_event_send(puVar6,0x1e,0);
                          iVar1 = lv_obj_get_child_cnt(puVar6);
                          if (iVar1 != 0) {
                            for (uVar13 = 0; uVar12 = lv_obj_get_child_cnt(puVar6), uVar13 < uVar12;
                                uVar13 = uVar13 + 1) {
                              puVar7 = (undefined4 *)lv_obj_get_child(puVar6,uVar13);
                              puVar3 = (undefined1 *)*puVar7;
                              if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
                                 (uVar12 = 0, puVar3 == lv_list_text_class)) {
                                lv_event_send(puVar7,0x1e,0);
                                iVar1 = lv_obj_get_child_cnt(puVar7);
                                if (iVar1 != 0) {
                                  for (uVar12 = 0; uVar9 = lv_obj_get_child_cnt(puVar7),
                                      uVar12 < uVar9; uVar12 = uVar12 + 1) {
                                    puVar8 = (undefined4 *)lv_obj_get_child(puVar7,uVar12);
                                    puVar3 = (undefined1 *)*puVar8;
                                    if (((puVar3 == lv_label_class) ||
                                        (puVar3 == lv_btnmatrix_class)) ||
                                       (uVar9 = 0, puVar3 == lv_list_text_class)) {
                                      lv_event_send(puVar8,0x1e,0);
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      if (iVar1 != 0) {
                                        uVar9 = 0;
                                        while (uVar11 = lv_obj_get_child_cnt(puVar8), uVar9 < uVar11
                                              ) {
                                          puVar10 = (undefined4 *)lv_obj_get_child(puVar8,uVar9);
                                          puVar3 = (undefined1 *)*puVar10;
                                          if (((puVar3 == lv_label_class) ||
                                              (puVar3 == lv_btnmatrix_class)) ||
                                             (puVar3 == lv_list_text_class)) {
                                            lv_event_send(puVar10,0x1e,0);
                                            iVar1 = lv_obj_get_child_cnt(puVar10);
                                            if (iVar1 != 0) goto LAB_00446f08;
                                            uVar9 = uVar9 + 1;
                                          }
                                          else {
LAB_00446f08:
                                            FUN_004467c8(puVar10);
                                            uVar9 = uVar9 + 1;
                                          }
                                        }
                                      }
                                    }
                                    else {
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      if (iVar1 != 0) {
                                        do {
                                          puVar10 = (undefined4 *)lv_obj_get_child(puVar8,uVar9);
                                          puVar3 = (undefined1 *)*puVar10;
                                          if (((puVar3 == lv_label_class) ||
                                              (puVar3 == lv_btnmatrix_class)) ||
                                             (puVar3 == lv_list_text_class)) {
                                            lv_event_send(puVar10,0x1e,0);
                                            iVar1 = lv_obj_get_child_cnt(puVar10);
                                            if (iVar1 != 0) goto LAB_00446e54;
                                          }
                                          else {
LAB_00446e54:
                                            FUN_004467c8(puVar10);
                                          }
                                          uVar9 = uVar9 + 1;
                                          uVar11 = lv_obj_get_child_cnt(puVar8);
                                        } while (uVar9 < uVar11);
                                      }
                                    }
                                  }
                                }
                              }
                              else {
                                iVar1 = lv_obj_get_child_cnt(puVar7);
                                if (iVar1 != 0) {
                                  do {
                                    puVar8 = (undefined4 *)lv_obj_get_child(puVar7,uVar12);
                                    puVar3 = (undefined1 *)*puVar8;
                                    if (((puVar3 == lv_label_class) ||
                                        (puVar3 == lv_btnmatrix_class)) ||
                                       (puVar3 == lv_list_text_class)) {
                                      lv_event_send(puVar8,0x1e,0);
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      uVar9 = 0;
                                      if (iVar1 != 0) {
                                        while (uVar11 = lv_obj_get_child_cnt(puVar8), uVar9 < uVar11
                                              ) {
                                          puVar10 = (undefined4 *)lv_obj_get_child(puVar8,uVar9);
                                          puVar3 = (undefined1 *)*puVar10;
                                          if (((puVar3 == lv_label_class) ||
                                              (puVar3 == lv_btnmatrix_class)) ||
                                             (puVar3 == lv_list_text_class)) {
                                            lv_event_send(puVar10,0x1e,0);
                                            iVar1 = lv_obj_get_child_cnt(puVar10);
                                            if (iVar1 != 0) goto LAB_00446d80;
                                            uVar9 = uVar9 + 1;
                                          }
                                          else {
LAB_00446d80:
                                            FUN_004467c8(puVar10);
                                            uVar9 = uVar9 + 1;
                                          }
                                        }
                                      }
                                    }
                                    else {
                                      FUN_004467c8(puVar8);
                                    }
                                    uVar12 = uVar12 + 1;
                                    uVar9 = lv_obj_get_child_cnt(puVar7);
                                  } while (uVar12 < uVar9);
                                }
                              }
                            }
                          }
                        }
                        else {
                          uVar13 = 0;
                          iVar1 = lv_obj_get_child_cnt(puVar6);
                          if (iVar1 != 0) {
                            do {
                              puVar7 = (undefined4 *)lv_obj_get_child(puVar6,uVar13);
                              puVar3 = (undefined1 *)*puVar7;
                              if (((puVar3 == lv_label_class) || (puVar3 == lv_btnmatrix_class)) ||
                                 (uVar12 = 0, puVar3 == lv_list_text_class)) {
                                lv_event_send(puVar7,0x1e,0);
                                iVar1 = lv_obj_get_child_cnt(puVar7);
                                if (iVar1 != 0) {
                                  for (uVar12 = 0; uVar9 = lv_obj_get_child_cnt(puVar7),
                                      uVar12 < uVar9; uVar12 = uVar12 + 1) {
                                    puVar8 = (undefined4 *)lv_obj_get_child(puVar7,uVar12);
                                    puVar3 = (undefined1 *)*puVar8;
                                    if (((puVar3 == lv_label_class) ||
                                        (puVar3 == lv_btnmatrix_class)) ||
                                       (uVar9 = 0, puVar3 == lv_list_text_class)) {
                                      lv_event_send(puVar8,0x1e,0);
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      if (iVar1 != 0) {
                                        uVar9 = 0;
                                        while (uVar11 = lv_obj_get_child_cnt(puVar8), uVar9 < uVar11
                                              ) {
                                          puVar10 = (undefined4 *)lv_obj_get_child(puVar8,uVar9);
                                          puVar3 = (undefined1 *)*puVar10;
                                          if (((puVar3 == lv_label_class) ||
                                              (puVar3 == lv_btnmatrix_class)) ||
                                             (puVar3 == lv_list_text_class)) {
                                            lv_event_send(puVar10,0x1e,0);
                                            iVar1 = lv_obj_get_child_cnt(puVar10);
                                            if (iVar1 != 0) goto LAB_00446c24;
                                            uVar9 = uVar9 + 1;
                                          }
                                          else {
LAB_00446c24:
                                            FUN_004467c8(puVar10);
                                            uVar9 = uVar9 + 1;
                                          }
                                        }
                                      }
                                    }
                                    else {
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      if (iVar1 != 0) {
                                        do {
                                          puVar10 = (undefined4 *)lv_obj_get_child(puVar8,uVar9);
                                          puVar3 = (undefined1 *)*puVar10;
                                          if (((puVar3 == lv_label_class) ||
                                              (puVar3 == lv_btnmatrix_class)) ||
                                             (puVar3 == lv_list_text_class)) {
                                            lv_event_send(puVar10,0x1e,0);
                                            iVar1 = lv_obj_get_child_cnt(puVar10);
                                            if (iVar1 != 0) goto LAB_00446b70;
                                          }
                                          else {
LAB_00446b70:
                                            FUN_004467c8(puVar10);
                                          }
                                          uVar9 = uVar9 + 1;
                                          uVar11 = lv_obj_get_child_cnt(puVar8);
                                        } while (uVar9 < uVar11);
                                      }
                                    }
                                  }
                                }
                              }
                              else {
                                iVar1 = lv_obj_get_child_cnt(puVar7);
                                if (iVar1 != 0) {
                                  do {
                                    puVar8 = (undefined4 *)lv_obj_get_child(puVar7,uVar12);
                                    puVar3 = (undefined1 *)*puVar8;
                                    if (((puVar3 == lv_label_class) ||
                                        (puVar3 == lv_btnmatrix_class)) ||
                                       (puVar3 == lv_list_text_class)) {
                                      lv_event_send(puVar8,0x1e,0);
                                      iVar1 = lv_obj_get_child_cnt(puVar8);
                                      if (iVar1 != 0) goto LAB_004469ac;
                                    }
                                    else {
LAB_004469ac:
                                      FUN_004467c8(puVar8);
                                    }
                                    uVar12 = uVar12 + 1;
                                    uVar9 = lv_obj_get_child_cnt(puVar7);
                                  } while (uVar12 < uVar9);
                                }
                              }
                              uVar13 = uVar13 + 1;
                              uVar12 = lv_obj_get_child_cnt(puVar6);
                            } while (uVar13 < uVar12);
                          }
                        }
                        local_4c = local_4c + 1;
                        uVar13 = lv_obj_get_child_cnt(puVar5);
                      } while (local_4c < uVar13);
                    }
                  }
                  local_44 = local_44 + 1;
                  uVar13 = lv_obj_get_child_cnt(puVar4);
                } while (local_44 < uVar13);
              }
            }
            local_34 = local_34 + 1;
            uVar13 = lv_obj_get_child_cnt(puVar2);
          } while (local_34 < uVar13);
        }
      }
      local_3c = local_3c + 1;
      uVar13 = lv_obj_get_child_cnt(param_1);
    } while (local_3c < uVar13);
  }
  return;
}



/* ===== FUNCTION FUN_00447154 @ 00447154 ===== */

void FUN_00447154(int param_1,uint param_2,int param_3,undefined4 param_4)

{
  char *__src;
  int iVar1;
  int *piVar2;
  undefined **ppuVar3;
  char local_30 [8];
  undefined1 local_28;
  
  __src = (char *)lv_label_get_text();
  piVar2 = *(int **)(param_1 + 0x10);
  if (piVar2 == (int *)0x0) goto LAB_0044720c;
  if (param_2 < 0x19) {
    ppuVar3 = *(undefined ***)(piVar2[1] + param_2 * 4);
    if (param_3 != 0) goto LAB_004471a8;
LAB_0044724c:
    strncpy(local_30,__src,8);
    local_28 = 0;
    if (local_30[0] == '#') {
      FUN_004406a0(param_1,ppuVar3,local_30);
      iVar1 = *(int *)(param_1 + 0x10);
    }
    else {
      lv_label_set_text(param_1,ppuVar3);
      iVar1 = *(int *)(param_1 + 0x10);
    }
  }
  else {
    ppuVar3 = &PTR_DAT_00450358;
    if (param_3 == 0) goto LAB_0044724c;
LAB_004471a8:
    FUN_004406a0(param_1,ppuVar3,param_3);
    iVar1 = *(int *)(param_1 + 0x10);
  }
  if (iVar1 != 0) {
    if (*piVar2 != 0) {
      lv_obj_set_style_text_font(param_1,*(undefined4 *)(*piVar2 + param_2 * 4),0);
      return;
    }
  }
LAB_0044720c:
  lv_obj_set_style_text_font(param_1,param_4,0);
  return;
}



/* ===== FUNCTION FUN_00447298 @ 00447298 ===== */

void FUN_00447298(int param_1,int param_2,undefined4 param_3,int param_4)

{
  undefined2 *puVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  lv_obj_add_event_cb(param_1,FUN_00446730,0x21,0);
  puVar1 = (undefined2 *)lv_mem_alloc(0xc);
  *puVar1 = (short)param_2;
  uVar2 = lv_mem_alloc(param_2 << 2);
  *(undefined4 *)(puVar1 + 2) = uVar2;
  *(int *)(puVar1 + 4) = param_4;
  if (0 < param_2) {
    iVar3 = 0;
    while( true ) {
      iVar5 = iVar3 * 4;
      iVar4 = *(int *)(param_4 + iVar5);
      if (iVar4 < 0) {
        if (iVar4 == -2) {
          *(undefined **)(*(int *)(puVar1 + 2) + iVar5) = &DAT_004505a4;
        }
        else if (iVar4 == -1) {
          *(char **)(*(int *)(puVar1 + 2) + iVar5) = " ";
        }
        else if (iVar4 == -3) {
          *(undefined **)(*(int *)(puVar1 + 2) + iVar5) = &DAT_0044f178;
        }
      }
      if (param_2 == iVar3 + 1) break;
      iVar3 = iVar3 + 1;
      param_4 = *(int *)(puVar1 + 4);
    }
  }
  *(undefined2 **)(param_1 + 0x10) = puVar1;
  lv_obj_add_event_cb(param_1,&LAB_00446650,0x1e,param_3);
  lv_event_send(param_1,0x1e,0);
  return;
}



/* ===== FUNCTION FUN_004473e0 @ 004473e0 ===== */

void FUN_004473e0(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined1 *puVar4;
  uint uVar5;
  uint uVar6;
  
  uVar6 = 0;
  lv_disp_get_default();
  uVar1 = DAT_0121d3ac;
  iVar2 = lv_obj_get_child_cnt(DAT_0121d3ac);
  if (iVar2 != 0) {
    do {
      puVar3 = (undefined4 *)lv_obj_get_child(uVar1,uVar6);
      puVar4 = (undefined1 *)*puVar3;
      if (((puVar4 == lv_label_class) || (puVar4 == lv_btnmatrix_class)) ||
         (puVar4 == lv_list_text_class)) {
        lv_event_send(puVar3,0x1e,0);
        iVar2 = lv_obj_get_child_cnt(puVar3);
        if (iVar2 != 0) goto LAB_00447470;
      }
      else {
LAB_00447470:
        FUN_004467c8(puVar3);
      }
      uVar6 = uVar6 + 1;
      uVar5 = lv_obj_get_child_cnt(uVar1);
    } while (uVar6 < uVar5);
  }
  uVar1 = DAT_012169e4;
  uVar6 = 0;
  iVar2 = lv_obj_get_child_cnt(DAT_012169e4);
  if (iVar2 != 0) {
    do {
      puVar3 = (undefined4 *)lv_obj_get_child(uVar1,uVar6);
      puVar4 = (undefined1 *)*puVar3;
      if (((puVar4 == lv_label_class) || (puVar4 == lv_btnmatrix_class)) ||
         (puVar4 == lv_list_text_class)) {
        lv_event_send(puVar3,0x1e,0);
        iVar2 = lv_obj_get_child_cnt(puVar3);
        if (iVar2 != 0) goto LAB_00447500;
      }
      else {
LAB_00447500:
        FUN_004467c8(puVar3);
      }
      uVar6 = uVar6 + 1;
      uVar5 = lv_obj_get_child_cnt(uVar1);
    } while (uVar6 < uVar5);
  }
  uVar1 = DAT_01216ae4;
  uVar6 = 0;
  iVar2 = lv_obj_get_child_cnt(DAT_01216ae4);
  if (iVar2 != 0) {
    do {
      puVar3 = (undefined4 *)lv_obj_get_child(uVar1,uVar6);
      puVar4 = (undefined1 *)*puVar3;
      if (((puVar4 == lv_label_class) || (puVar4 == lv_btnmatrix_class)) ||
         (puVar4 == lv_list_text_class)) {
        lv_event_send(puVar3,0x1e,0);
        iVar2 = lv_obj_get_child_cnt(puVar3);
        if (iVar2 != 0) goto LAB_00447590;
      }
      else {
LAB_00447590:
        FUN_004467c8(puVar3);
      }
      uVar6 = uVar6 + 1;
      uVar5 = lv_obj_get_child_cnt(uVar1);
    } while (uVar6 < uVar5);
  }
  uVar1 = DAT_0121720c;
  uVar6 = 0;
  iVar2 = lv_obj_get_child_cnt(DAT_0121720c);
  if (iVar2 != 0) {
    do {
      puVar3 = (undefined4 *)lv_obj_get_child(uVar1,uVar6);
      puVar4 = (undefined1 *)*puVar3;
      if (((puVar4 == lv_label_class) || (puVar4 == lv_btnmatrix_class)) ||
         (puVar4 == lv_list_text_class)) {
        lv_event_send(puVar3,0x1e,0);
        iVar2 = lv_obj_get_child_cnt(puVar3);
        if (iVar2 != 0) goto LAB_00447620;
      }
      else {
LAB_00447620:
        FUN_004467c8(puVar3);
      }
      uVar6 = uVar6 + 1;
      uVar5 = lv_obj_get_child_cnt(uVar1);
    } while (uVar6 < uVar5);
  }
  uVar1 = DAT_012171f0;
  uVar6 = 0;
  iVar2 = lv_obj_get_child_cnt(DAT_012171f0);
  if (iVar2 != 0) {
    do {
      puVar3 = (undefined4 *)lv_obj_get_child(uVar1,uVar6);
      puVar4 = (undefined1 *)*puVar3;
      if (((puVar4 == lv_label_class) || (puVar4 == lv_btnmatrix_class)) ||
         (puVar4 == lv_list_text_class)) {
        lv_event_send(puVar3,0x1e,0);
        iVar2 = lv_obj_get_child_cnt(puVar3);
        if (iVar2 != 0) goto LAB_004476b0;
      }
      else {
LAB_004476b0:
        FUN_004467c8(puVar3);
      }
      uVar6 = uVar6 + 1;
      uVar5 = lv_obj_get_child_cnt(uVar1);
    } while (uVar6 < uVar5);
  }
  return;
}



/* ===== FUNCTION FUN_00447d38 @ 00447d38 ===== */

void FUN_00447d38(undefined4 param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  undefined4 local_30 [3];
  
  uVar6 = 0;
  uVar1 = FUN_00442b5c(0x21,0x21);
  lv_obj_set_style_opa(uVar1,0xff,0);
  FUN_00442630(uVar1,0x30,0x21);
  uVar2 = lv_obj_create(uVar1);
  lv_obj_set_style_pad_left(uVar2,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_border_width(uVar2,0,0);
  lv_obj_set_style_outline_width(uVar2,0,0);
  lv_obj_set_style_pad_top(uVar2,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  lv_obj_set_size(uVar2,0x2064,0x202c);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_set_scrollbar_mode(uVar2,0);
  uVar3 = lv_obj_create(uVar2);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_size(uVar3,0x2341,0x2064);
  lv_obj_set_style_radius(uVar3,0,0);
  puVar7 = &DAT_0121d410;
  lv_palette_darken(local_30,0x12,1);
  lv_obj_set_style_bg_color(uVar3,local_30[0],0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  do {
    uVar5 = uVar6 & 0xffff;
    uVar6 = uVar6 + 1;
    uVar4 = FUN_00438630(uVar5,0xdc);
    *puVar7 = uVar4;
    puVar7 = puVar7 + 1;
  } while (uVar6 != 0x19);
  DAT_0121d474 = &DAT_0044f178;
  uVar3 = FUN_0044367c(uVar3,100,100,&DAT_0121d410);
  lv_obj_add_event_cb(uVar3,&LAB_00447798,0,param_1);
  lv_obj_set_style_text_font(uVar3,lv_font_montserrat_26,0);
  uVar4 = lv_btn_create(uVar2);
  lv_obj_set_style_shadow_width(uVar4,0,0);
  lv_obj_set_style_radius(uVar4,0,0);
  lv_obj_set_size(uVar4,0x2008,0x2064);
  lv_obj_align(uVar4,7,0,0);
  lv_obj_set_style_bg_opa(uVar4,0x33,0);
  uVar4 = lv_label_create(uVar4);
  lv_obj_set_style_bg_opa(uVar4,0,0);
  lv_label_set_text(uVar4,&DAT_00455d7c);
  lv_obj_align(uVar4,9,0,0);
  uVar2 = lv_btn_create(uVar2);
  lv_obj_set_style_shadow_width(uVar2,0,0);
  lv_obj_set_style_radius(uVar2,0,0);
  lv_obj_set_size(uVar2,0x2008,0x2064);
  lv_obj_align(uVar2,8,0,0);
  lv_obj_set_style_bg_opa(uVar2,0x33,0);
  uVar2 = lv_label_create(uVar2);
  lv_obj_set_style_bg_opa(uVar2,0,0);
  lv_label_set_text(uVar2,&DAT_0045027c);
  lv_obj_align(uVar2,9,0,0);
  FUN_00442788(uVar1,0x18,param_1);
  lv_group_focus_obj(uVar3);
  return;
}



/* ===== FUNCTION FUN_004480f0 @ 004480f0 ===== */

void FUN_004480f0(void)

{
  return;
}



/* ===== FUNCTION FUN_004480f8 @ 004480f8 ===== */

undefined4 FUN_004480f8(void)

{
  return 0;
}



/* ===== FUNCTION FUN_00448100 @ 00448100 ===== */

undefined4 FUN_00448100(void)

{
  return 0;
}



/* ===== FUNCTION FUN_00448108 @ 00448108 ===== */

undefined4 FUN_00448108(void)

{
  return 0x780;
}



/* ===== FUNCTION FUN_00448110 @ 00448110 ===== */

undefined4 FUN_00448110(void)

{
  return 0x438;
}



/* ===== FUNCTION FUN_00448118 @ 00448118 ===== */

uint FUN_00448118(void)

{
  undefined4 local_18;
  undefined4 local_14;
  uint local_10;
  
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  FUN_0042dd64(&local_18);
  return local_10 & 0xffff;
}



/* ===== FUNCTION FUN_00448144 @ 00448144 ===== */

undefined2 FUN_00448144(void)

{
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  FUN_0042dd64(&local_18);
  return local_10._2_2_;
}



/* ===== FUNCTION FUN_00448170 @ 00448170 ===== */

bool FUN_00448170(void)

{
  undefined4 local_18;
  undefined4 local_14;
  uint local_10;
  
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  FUN_0042dd64(&local_18);
  return (local_10 & 0xffff) < local_10 >> 0x10;
}



/* ===== FUNCTION FUN_004481a4 @ 004481a4 ===== */

void FUN_004481a4(int *param_1,int *param_2,int param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  undefined4 local_30;
  undefined4 local_2c;
  uint local_28;
  int local_24;
  int local_20 [2];
  
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  FUN_0042dd64(&local_30);
  uVar7 = local_28 & 0xffff;
  uVar8 = local_28 >> 0x10;
  uVar4 = uVar8;
  uVar6 = uVar7;
  if (param_3 != 2) {
    local_20[0] = 0x10;
    local_24 = 9;
    if (param_3 == 0) {
      local_20[0] = 4;
      local_24 = 3;
      iVar2 = 3;
      iVar1 = 4;
      if (uVar8 <= uVar7) goto LAB_00448214;
LAB_004482ac:
      FUN_0042ee7c(local_20,&local_24,4);
      uVar5 = local_28 & 0xffff;
      uVar6 = uVar7 * local_24;
      uVar3 = uVar8 * local_20[0];
      uVar4 = local_28 >> 0x10;
      if ((int)uVar6 <= (int)uVar3) {
LAB_004482dc:
        iVar2 = uVar7 << 4;
        if ((int)uVar6 < (int)uVar3) {
          if (local_20[0] == 0) {
            trap(7);
          }
          uVar8 = (int)uVar6 / local_20[0];
        }
        goto LAB_0044823c;
      }
    }
    else {
      iVar2 = 9;
      iVar1 = 0x10;
      if (uVar7 < uVar8) goto LAB_004482ac;
LAB_00448214:
      local_20[0] = iVar1;
      local_24 = iVar2;
      uVar6 = uVar7 * local_24;
      uVar3 = uVar8 * local_20[0];
      uVar5 = uVar7;
      if (uVar6 <= uVar3) goto LAB_004482dc;
    }
    if (local_24 == 0) {
      trap(7);
    }
    uVar7 = uVar5;
    uVar6 = (int)uVar3 / local_24;
  }
  iVar2 = uVar6 << 4;
  uVar5 = uVar7;
  uVar7 = uVar6;
LAB_0044823c:
  if (uVar5 == 0) {
    trap(7);
  }
  if (uVar4 == 0) {
    trap(7);
  }
  *param_1 = (int)((iVar2 - uVar7) * 0x80) / (int)uVar5;
  *param_2 = (int)(uVar8 * 0x438) / (int)uVar4;
  return;
}



/* ===== FUNCTION FUN_00448310 @ 00448310 ===== */

undefined4 FUN_00448310(int param_1)

{
  int iVar1;
  undefined4 local_30;
  undefined4 local_2c;
  int local_28;
  int local_24;
  uint local_20;
  undefined4 local_1c;
  undefined4 local_18;
  
  local_30 = 1;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  local_18 = 0;
  DAT_0121d488 = FUN_00428454(0x1b);
  DAT_0121d486 = FUN_00428454(0x1c);
  local_28 = (int)DAT_0121d488;
  local_24 = (int)DAT_0121d486;
  DAT_0121d484 = DAT_0121d486;
  if (local_24 < local_28) {
    DAT_0121d484 = DAT_0121d488;
  }
  local_20 = local_20 & 0xffffff00;
  iVar1 = (int)DAT_0121d484;
  if (param_1 == 0) {
    DAT_0121d488 = DAT_0121d488 + -8;
    if (local_24 < iVar1) {
      DAT_0121d486 = DAT_0121d486 + 8;
      if (iVar1 < DAT_0121d486) {
        DAT_0121d486 = DAT_0121d484;
      }
      local_24 = (int)DAT_0121d486;
      local_28 = iVar1;
      DAT_0121d488 = DAT_0121d484;
    }
    else {
      local_28 = (int)DAT_0121d488;
      local_24 = iVar1;
      DAT_0121d486 = DAT_0121d484;
      if (local_28 < 0x1e0) {
        DAT_0121d488 = 0x1e0;
        local_28 = 0x1e0;
      }
    }
  }
  else if (param_1 == 1) {
    DAT_0121d486 = DAT_0121d486 + -8;
    if (local_28 < iVar1) {
      DAT_0121d488 = DAT_0121d488 + 8;
      if (iVar1 < DAT_0121d488) {
        DAT_0121d488 = DAT_0121d484;
      }
      local_24 = iVar1;
      local_28 = (int)DAT_0121d488;
      DAT_0121d486 = DAT_0121d484;
    }
    else {
      local_24 = (int)DAT_0121d486;
      local_28 = iVar1;
      DAT_0121d488 = DAT_0121d484;
      if (DAT_0121d486 < 0x1e0) {
        DAT_0121d486 = 0x1e0;
        local_24 = 0x1e0;
      }
    }
  }
  local_2c = CONCAT31(local_2c._1_3_,1);
  FUN_004286f8(0x1b,local_28);
  FUN_004286f8(0x1c,(int)DAT_0121d486);
  FUN_00428080();
  printf("top width:  %d, bottom width: %d\n",local_28,local_24);
  ioctl(DAT_01216540,0x801c0e10,&local_30);
  return 0;
}



/* ===== FUNCTION FUN_00448950 @ 00448950 ===== */

void FUN_00448950(void)

{
  return;
}



/* ===== FUNCTION FUN_00448958 @ 00448958 ===== */

void FUN_00448958(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 local_24 [2];
  short local_1c;
  
  DAT_01216844 = FUN_00444430(param_1,0x4d,100);
  FUN_00444868(DAT_01216844,100,0xb,&DAT_00455da4);
  uVar1 = lv_obj_get_child(DAT_01216844,0);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  uVar1 = lv_obj_get_child(DAT_01216844,0);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_00455d7c);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_align(uVar1,7,0,0);
  uVar1 = lv_obj_get_child(DAT_01216844,0);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_0045027c);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_align(uVar1,8,0,0);
  FUN_004446c0(DAT_01216844,100,0xb,0xce);
  uVar1 = lv_obj_get_child(DAT_01216844,1);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_00455d7c);
  lv_obj_align(uVar1,7,0,0);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  uVar1 = lv_obj_get_child(DAT_01216844,1);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_0045027c);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_align(uVar1,8,0,0);
  FUN_00444868(DAT_01216844,100,0xb,&DAT_00455da8);
  uVar1 = lv_obj_get_child(DAT_01216844,2);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  uVar1 = lv_obj_get_child(DAT_01216844,2);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_00455d7c);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_align(uVar1,7,0,0);
  uVar1 = lv_obj_get_child(DAT_01216844,2);
  uVar1 = lv_label_create(uVar1);
  lv_obj_set_size(uVar1,0x200f,0x2064);
  lv_obj_set_style_bg_opa(uVar1,0,0);
  lv_label_set_text(uVar1,&DAT_0045027c);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_align(uVar1,8,0,0);
  uVar1 = lv_obj_get_child(DAT_01216844,2);
  lv_obj_set_style_text_font(uVar1,lv_font_montserrat_40,0);
  lv_obj_add_event_cb(DAT_01216844,&LAB_00448528,0,0);
  if ((DAT_01216540 < 0) && (DAT_01216540 = open("/dev/dis",2), -1 < DAT_01216540)) {
    local_24[0] = 1;
    iVar2 = ioctl(DAT_01216540,0xc00c0e0c,local_24);
    if (iVar2 != 0) {
      perror("ioctl(get screeninfo)");
    }
    DAT_0121d484 = local_1c;
    DAT_0121d486 = local_1c;
    DAT_0121d488 = local_1c;
    iVar2 = FUN_00428454(0x1b);
    iVar3 = (int)DAT_0121d488;
    if (iVar2 != 0) {
      iVar2 = FUN_00428454(0x1b,iVar3);
      if (iVar2 != 0xff) {
        return;
      }
      iVar3 = (int)DAT_0121d488;
    }
    FUN_004286f8(0x1b,iVar3);
    FUN_004286f8(0x1c,(int)DAT_0121d486);
    FUN_00428080();
  }
  return;
}



/* ===== FUNCTION FUN_00448de4 @ 00448de4 ===== */

undefined4 FUN_00448de4(undefined4 param_1)

{
  FUN_004286f8(0x3b,param_1);
  return 0;
}



/* ===== FUNCTION FUN_00448e08 @ 00448e08 ===== */

void FUN_00448e08(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar1 = lv_event_get_target();
  iVar2 = lv_event_get_code(param_1);
  if (iVar2 == 0x1a) {
    uVar3 = lv_btnmatrix_get_selected_btn(uVar1);
    lv_btnmatrix_set_btn_ctrl(uVar1,uVar3,0x80);
    iVar2 = lv_event_get_draw_part_dsc(param_1);
    if (*(int *)(iVar2 + 0x34) == 0x50000) {
      *(undefined4 *)(*(int *)(iVar2 + 0x14) + 0xc) = 0xffffffff;
    }
  }
  return;
}



/* ===== FUNCTION FUN_00448e8c @ 00448e8c ===== */

undefined4 FUN_00448e8c(int param_1)

{
  int iVar1;
  
  printf("%s(), set mode >> %d \n","change_sound_mode_output",param_1);
  FUN_004286f8(0x3a,param_1);
  if (param_1 == 0) {
    if ((DAT_0121d494 != 0) && (iVar1 = lv_obj_has_state(DAT_0121d494,0x80), iVar1 == 0)) {
      lv_obj_add_state(DAT_0121d494,0x80);
      return 0;
    }
  }
  else if (((param_1 == 1) && (DAT_0121d494 != 0)) &&
          (iVar1 = lv_obj_has_state(DAT_0121d494,0x80), iVar1 != 0)) {
    lv_obj_clear_state(DAT_0121d494,0x80);
  }
  return 0;
}



/* ===== FUNCTION FUN_00448f50 @ 00448f50 ===== */

undefined4 FUN_00448f50(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  int *piVar8;
  undefined4 local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28 [3];
  
  local_28[0] = 0;
  local_28[1] = 0;
  if (param_1 == 1) {
    local_28[0] = 5;
    local_28[1] = 5;
    goto LAB_00448fb0;
  }
  if (param_1 < 2) {
    if (param_1 == 0) goto LAB_00448fb0;
  }
  else {
    if (param_1 == 2) {
      local_28[0] = 6;
      local_28[1] = 5;
      goto LAB_00448fb0;
    }
    if (param_1 == 3) {
      local_28[0] = -3;
      local_28[1] = -3;
      goto LAB_00448fb0;
    }
  }
  local_28[0] = FUN_00428454(0x10);
  local_28[1] = FUN_00428454(0xf);
  iVar1 = DAT_0121d4a4;
  iVar3 = DAT_0121d4a0;
  if (param_1 < 0) {
    return 0;
  }
  if (3 < param_1) {
    if (param_1 != 4) {
      return 0;
    }
    local_38 = 0;
    local_34 = 0;
    puVar5 = &DAT_0121d4a0;
    local_30 = 0;
    local_2c = 0;
    iVar2 = open("/dev/sndC0i2so",1);
    if (iVar2 < 0) {
      printf("twotone open snd_fd %d failed\n",iVar2);
    }
    else {
      local_38 = 1;
      local_34 = param_1;
      if (iVar1 + 10U < 0x15) {
        local_30 = iVar1;
        FUN_004286f8(0x10,iVar1);
      }
      if (iVar3 + 10U < 0x15) {
        local_2c = iVar3;
        FUN_004286f8(0xf,iVar3);
      }
      FUN_004286f8(8,4);
      FUN_00428080();
      ioctl(iVar2,0x80100813,&local_38);
      close(iVar2);
    }
    uVar7 = DAT_0121edcc;
    puVar6 = &DAT_0121edd4;
    iVar3 = lv_obj_has_state(DAT_0121edcc,0x80);
    while( true ) {
      if (iVar3 != 0) {
        lv_obj_clear_state(uVar7,0x80);
      }
      lv_obj_set_style_text_color(uVar7,0xffffffff,0);
      uVar7 = lv_obj_get_child(uVar7,1);
      uVar4 = *puVar5;
      puVar5 = puVar5 + 1;
      lv_label_set_text_fmt(uVar7,&DAT_00452028,uVar4);
      if (puVar6 == &DAT_0121eddc) break;
      uVar7 = *puVar6;
      puVar6 = puVar6 + 2;
      iVar3 = lv_obj_has_state(uVar7,0x80);
    }
    return 0;
  }
LAB_00448fb0:
  iVar1 = local_28[1];
  iVar3 = local_28[0];
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  iVar2 = open("/dev/sndC0i2so",1);
  if (iVar2 < 0) {
    printf("twotone open snd_fd %d failed\n",iVar2);
  }
  else {
    local_38 = 1;
    local_34 = param_1;
    if (iVar1 + 10U < 0x15) {
      local_30 = iVar1;
      FUN_004286f8(0x10,iVar1);
    }
    if (iVar3 + 10U < 0x15) {
      local_2c = iVar3;
      FUN_004286f8(0xf,iVar3);
    }
    FUN_004286f8(8,param_1);
    FUN_00428080();
    ioctl(iVar2,0x80100813,&local_38);
    close(iVar2);
  }
  puVar5 = &DAT_0121edcc;
  piVar8 = local_28;
  do {
    uVar7 = *puVar5;
    puVar5 = puVar5 + 2;
    iVar3 = lv_obj_has_state(uVar7,0x80);
    if (iVar3 == 0) {
      lv_obj_add_state(uVar7,0x80);
    }
    lv_obj_set_style_text_color(uVar7,0xff7d7d7d,0);
    uVar7 = lv_obj_get_child(uVar7,1);
    iVar3 = *piVar8;
    piVar8 = piVar8 + 1;
    lv_label_set_text_fmt(uVar7,&DAT_00452028,iVar3);
  } while (puVar5 != &DAT_0121eddc);
  return 0;
}



/* ===== FUNCTION FUN_0044a180 @ 0044a180 ===== */

void FUN_0044a180(undefined4 param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = FUN_00428454(8);
  uVar1 = FUN_00443828(param_1,0x13,&DAT_012165c0,uVar1,0xc,0,0);
  iVar2 = FUN_00428454(8);
  if (iVar2 == 4) {
    DAT_0121d4a0 = FUN_00428454(DAT_0121edd0,1);
    DAT_0121d4a4 = FUN_00428454(DAT_0121edd8);
  }
  uVar1 = lv_obj_get_child(uVar1,1);
  lv_obj_add_event_cb(uVar1,&LAB_00448dc0,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044a248 @ 0044a248 ===== */

void FUN_0044a248(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00428454(0x3a);
  uVar1 = FUN_00443828(param_1,0x118,&DAT_012165a8,uVar1,6,0,0);
  uVar1 = lv_obj_get_child(uVar1,1);
  lv_obj_add_event_cb(uVar1,&LAB_00448dcc,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044a2b8 @ 0044a2b8 ===== */

void FUN_0044a2b8(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00428454(0x3b);
  uVar1 = FUN_00443828(param_1,0x11a,&DAT_01216588,uVar1,8,0,0);
  uVar1 = lv_obj_get_child(uVar1,1);
  lv_obj_add_event_cb(uVar1,&LAB_00448dd8,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044a328 @ 0044a328 ===== */

void FUN_0044a328(undefined4 param_1)

{
  DAT_0121d494 = param_1;
  return;
}



/* ===== FUNCTION FUN_0044a334 @ 0044a334 ===== */

undefined4 FUN_0044a334(undefined4 param_1,int param_2,int param_3)

{
  int __fd;
  undefined4 uVar1;
  undefined4 local_28;
  undefined4 local_24;
  int local_20;
  int local_1c;
  
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  __fd = open("/dev/sndC0i2so",1);
  if (__fd < 0) {
    printf("twotone open snd_fd %d failed\n",__fd);
    uVar1 = 0xffffffff;
  }
  else {
    local_28 = 1;
    local_24 = param_1;
    if (param_3 + 10U < 0x15) {
      local_20 = param_3;
      FUN_004286f8(0x10,param_3);
    }
    if (param_2 + 10U < 0x15) {
      local_1c = param_2;
      FUN_004286f8(0xf,param_2);
    }
    FUN_004286f8(8,param_1);
    FUN_00428080();
    ioctl(__fd,0x80100813,&local_28);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0044a448 @ 0044a448 ===== */

undefined4 FUN_0044a448(undefined4 param_1)

{
  int __fd;
  undefined4 uVar1;
  undefined4 local_18;
  undefined4 local_14;
  
  local_18 = 0;
  local_14 = 0;
  __fd = open("/dev/sndC0i2so",1);
  if (__fd < 0) {
    printf("lr_balance open snd_fd %d failed\n",__fd);
    uVar1 = 0xffffffff;
  }
  else {
    local_18 = 1;
    local_14 = param_1;
    FUN_004286f8(0xb,param_1);
    FUN_00428080();
    ioctl(__fd,0x80080814,&local_18);
    close(__fd);
    uVar1 = 0;
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0044a614 @ 0044a614 ===== */

void FUN_0044a614(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  puVar3 = (undefined4 *)&DAT_012165f0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  local_28 = 0;
  local_24 = 0;
  local_20 = 0;
  local_1c = 0;
  uVar1 = FUN_00428454(10);
  FUN_00428968(uVar1,&local_40);
  DAT_012165fc = local_40;
  DAT_0121660c = local_3c;
  DAT_0121661c = local_38;
  DAT_0121662c = local_34;
  DAT_0121663c = local_30;
  DAT_0121664c = local_2c;
  DAT_0121665c = local_28;
  DAT_0121666c = local_24;
  DAT_0121667c = local_20;
  DAT_0121668c = local_1c;
  iVar2 = open("/dev/sndC0i2so",1);
  if (iVar2 < 0) {
    printf("lr_balance open snd_fd %d failed\n",iVar2);
  }
  else {
    do {
      puVar4 = puVar3 + 4;
      ioctl(iVar2,0x80100819,puVar3);
      puVar3 = puVar4;
    } while (puVar4 != &DAT_01216690);
    close(iVar2);
  }
  iVar2 = FUN_00428454(9);
  if (iVar2 == 0) {
    opterr = 0;
    optind = 0;
    iVar2 = open("/dev/sndC0i2so",1);
    uVar1 = 0;
    if (iVar2 < 0) {
LAB_0044a7a0:
      printf("eq_enable open snd_fd %d failed\n",iVar2);
      return;
    }
  }
  else {
    opterr = 0;
    optind = 0;
    iVar2 = open("/dev/sndC0i2so",1);
    if (iVar2 < 0) goto LAB_0044a7a0;
    uVar1 = 1;
  }
  ioctl(iVar2,0x20000818,uVar1);
  close(iVar2);
  return;
}



/* ===== FUNCTION FUN_0044a7d0 @ 0044a7d0 ===== */

void FUN_0044a7d0(undefined4 param_1)

{
  int *piVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 *puVar8;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  char *local_40;
  undefined4 *local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  
  uVar3 = lv_obj_create(DAT_0121d398);
  local_30 = uVar3;
  lv_obj_align(uVar3,9,0,0);
  lv_obj_set_size(uVar3,0x2046,0x2046);
  lv_obj_set_flex_flow(uVar3,0);
  lv_obj_set_flex_align(uVar3,0,0,2);
  lv_obj_set_style_bg_color(uVar3,0xff202040,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_scrollbar_mode(uVar3,0);
  uVar3 = lv_obj_create(uVar3);
  lv_obj_set_scrollbar_mode(uVar3,0);
  lv_obj_set_style_bg_opa(uVar3,0,0);
  lv_obj_set_style_border_width(uVar3,0,0);
  lv_obj_set_style_pad_left(uVar3,0,0);
  lv_obj_set_style_pad_right(uVar3,0,0);
  lv_obj_set_style_pad_top(uVar3,0,0);
  lv_obj_set_style_pad_bottom(uVar3,0,0);
  lv_obj_set_style_outline_width(uVar3,2,0);
  lv_obj_set_style_outline_color(uVar3,0xffffffff,0);
  lv_obj_set_size(uVar3,0x2014,0x2064);
  uVar4 = lv_group_get_default();
  lv_group_add_obj(uVar4,uVar3);
  lv_obj_add_event_cb(uVar3,&LAB_0044971c,0,param_1);
  DAT_0121d49c = uVar3;
  uVar4 = lv_switch_create(uVar3);
  lv_obj_align(uVar4,7,0,0);
  lv_palette_main(&local_48,5);
  lv_obj_set_style_outline_color(uVar4,local_48,0);
  lv_obj_set_style_outline_width(uVar4,2,0);
  lv_obj_set_style_outline_pad(uVar4,1,0);
  lv_obj_set_width(uVar4,0x2028);
  iVar5 = FUN_00428454(9);
  if (iVar5 != 0) {
    lv_obj_add_state(uVar4,1);
    lv_obj_set_style_outline_color(uVar4,0xffffffff,0);
    lv_obj_set_style_outline_width(uVar4,0,0);
  }
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216550);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216554);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216558);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_0121655c);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216560);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216564);
  uVar2 = FUN_00428454(0x11);
  FUN_00438630(uVar2,DAT_01216568);
  uVar4 = lv_btnmatrix_create(uVar3);
  lv_obj_add_event_cb(uVar4,FUN_00448e08,0,0);
  lv_obj_set_size(uVar4,0x203c,0x203c);
  lv_obj_align(uVar4,8,0,0);
  FUN_00447298(uVar4,0xe,3,&DAT_01216550);
  lv_btnmatrix_set_btn_ctrl_all(uVar4,0x40);
  lv_btnmatrix_set_one_checked(uVar4,1);
  lv_obj_set_style_bg_opa(uVar4,0,0);
  lv_obj_set_style_bg_opa(uVar4,0,0x50000);
  lv_obj_set_style_text_color(uVar4,0xffffffff,0);
  lv_obj_set_style_border_width(uVar4,0,0);
  lv_obj_set_style_pad_left(uVar4,5,0);
  lv_obj_set_style_pad_right(uVar4,5,0);
  lv_obj_set_style_pad_top(uVar4,5,0);
  lv_obj_set_style_pad_bottom(uVar4,5,0);
  lv_obj_set_style_shadow_width(uVar4,0,0x50000);
  lv_obj_set_style_radius(uVar4,0,0x50000);
  lv_obj_set_style_radius(uVar4,0,0);
  iVar5 = FUN_00428454(9);
  if (iVar5 != 0) {
    uVar2 = FUN_00428454(10);
    lv_btnmatrix_set_selected_btn(uVar4,uVar2);
    uVar2 = FUN_00428454(10);
    lv_btnmatrix_set_btn_ctrl(uVar4,uVar2,0x80);
  }
  uVar4 = lv_obj_create(local_30);
  DAT_0121d498 = uVar4;
  lv_obj_align(uVar4,9,0,0);
  lv_obj_set_size(uVar4,0x2050,0x2064);
  lv_obj_set_flex_flow(uVar4,0);
  lv_obj_set_flex_align(uVar4,5,2,2);
  lv_obj_set_style_bg_color(uVar4,0xff202040,0);
  lv_obj_set_style_pad_row(uVar4,0x11,0);
  lv_obj_set_style_pad_column(uVar4,0x11,0);
  lv_obj_set_style_border_width(uVar4,0,0);
  local_70 = 0;
  puVar8 = &DAT_012165fc;
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_5c = 0;
  local_58 = 0;
  local_54 = 0;
  local_50 = 0;
  local_4c = 0;
  uVar6 = FUN_00428454(10);
  FUN_00428968(uVar6,&local_70);
  local_3c = (undefined4 *)&DAT_0121669c;
  DAT_012165fc = local_70;
  local_40 = "nd bgm path";
  DAT_0121660c = local_6c;
  local_44 = 0xff404080;
  DAT_0121661c = local_68;
  DAT_0121662c = local_64;
  local_38 = "%dk\nHz";
  DAT_0121663c = local_60;
  local_34 = "%d\nHz";
  DAT_0121664c = local_5c;
  DAT_0121665c = local_58;
  DAT_0121666c = local_54;
  DAT_0121667c = local_50;
  DAT_0121668c = local_4c;
  do {
    while( true ) {
      uVar6 = lv_obj_create(uVar4);
      lv_obj_set_height(uVar6,0x2064);
      lv_obj_set_style_pad_left(uVar6,4,0);
      lv_obj_set_style_pad_right(uVar6,4,0);
      lv_obj_set_style_pad_top(uVar6,5,0);
      lv_obj_set_style_pad_bottom(uVar6,5,0);
      lv_obj_set_scrollbar_mode(uVar6,0);
      lv_obj_set_style_border_color(uVar6,0xffffffff,2);
      lv_obj_add_event_cb(uVar6,&LAB_00449304,0,0);
      uVar7 = lv_group_get_default();
      lv_group_add_obj(uVar7,uVar6);
      lv_obj_set_style_bg_color(uVar6,local_44,0);
      lv_obj_set_style_border_width(uVar6,0,0);
      lv_obj_set_style_border_width(uVar6,2,2);
      lv_obj_set_style_text_color(uVar6,0xffffffff,0);
      lv_obj_set_flex_flow(uVar6,1);
      lv_obj_set_flex_align(uVar6,2,2,2);
      uVar7 = lv_bar_create(uVar6);
      lv_bar_set_range(uVar7,0xffffff88,0x78);
      lv_bar_set_value(uVar7,*puVar8,0);
      lv_palette_lighten(&local_48,0x12,1);
      lv_obj_set_style_bg_color(uVar7,local_48,0);
      lv_obj_set_style_bg_grad_dir(uVar7,1,0x20000);
      lv_obj_set_style_bg_color(uVar7,0xff00ff00,0x20000);
      lv_obj_set_style_bg_grad_color(uVar7,0xff0080ff,0x20000);
      lv_obj_set_flex_grow(uVar7,0xf);
      lv_obj_set_width(uVar7,0x2050);
      uVar7 = lv_label_create(uVar7);
      lv_obj_align(uVar7,9,0,0);
      lv_obj_set_style_text_color(uVar7,0xffffffff,0);
      lv_label_set_text_fmt(uVar7,local_40 + 0x2028,*puVar8);
      uVar7 = lv_label_create(uVar6);
      lv_obj_set_width(uVar7,0x2064);
      lv_obj_set_flex_grow(uVar7,1);
      lv_obj_set_style_border_color(uVar7,0xff000080,2);
      lv_obj_set_style_border_width(uVar7,1,2);
      lv_obj_set_style_text_align(uVar7,2,0);
      piVar1 = puVar8 + -2;
      if (999 < *piVar1) break;
      puVar8 = puVar8 + 4;
      lv_label_set_text_fmt(uVar7,local_34);
      lv_obj_set_flex_grow(uVar6,1);
      if (local_3c == puVar8) goto LAB_0044b01c;
    }
    puVar8 = puVar8 + 4;
    lv_label_set_text_fmt(uVar7,local_38,*piVar1 / 1000);
    lv_obj_set_flex_grow(uVar6,1);
  } while (local_3c != puVar8);
LAB_0044b01c:
  FUN_00440694(local_30);
  lv_group_focus_obj(uVar3);
  return;
}



/* ===== FUNCTION FUN_0044b060 @ 0044b060 ===== */

void FUN_0044b060(void)

{
  DAT_0121d4b0 = lv_group_create();
  return;
}



/* ===== FUNCTION FUN_0044b084 @ 0044b084 ===== */

void FUN_0044b084(void)

{
  undefined4 uVar1;
  
  FUN_0040677c(DAT_0121d4b0);
  uVar1 = FUN_00442b5c(0x3c,0x32);
  FUN_00442630(uVar1,0x46,0xf);
  uVar1 = lv_obj_create(uVar1);
  lv_obj_set_size(uVar1,0x2064,0x2055);
  lv_obj_set_flex_flow(uVar1,5);
  lv_obj_set_style_bg_color(uVar1,0xff6565b1,0);
  return;
}



/* ===== FUNCTION FUN_0044b1b0 @ 0044b1b0 ===== */

undefined4 FUN_0044b1b0(int param_1)

{
  printf("set auto >> %d \n",param_1);
  FUN_004286f8(0x19,param_1);
  if (param_1 != 0) {
    if (DAT_0121d4c0 == 0) {
      DAT_0121d4c0 = lv_timer_create(FUN_0044b3d8,3600000,0);
      lv_timer_set_repeat_count(DAT_0121d4c0,0xffffffff);
    }
    if (param_1 == 1) {
      lv_timer_set_period(DAT_0121d4c0,3600000);
    }
    else if (param_1 == 2) {
      lv_timer_set_period(DAT_0121d4c0,&DAT_006ddd00);
    }
    else {
      lv_timer_set_period(DAT_0121d4c0,&DAT_00a4cb80);
    }
    lv_timer_reset(DAT_0121d4c0);
    return 1;
  }
  if (DAT_0121d4c0 != 0) {
    lv_timer_del();
    DAT_0121d4c0 = 0;
    return 0;
  }
  return 0;
}



/* ===== FUNCTION FUN_0044b3d8 @ 0044b3d8 ===== */

void FUN_0044b3d8(void)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 local_18 [2];
  
  iVar1 = FUN_004379bc();
  if (iVar1 != 0) {
    if (DAT_0121d4c0 != 0) {
      lv_timer_reset();
      if (DAT_0121d4c4 != 0) {
        lv_timer_del();
      }
      DAT_0121d4c4 = 0;
      if (DAT_0121d4cc != 0) {
        lv_obj_del();
      }
      DAT_0121d4cc = 0;
      if (DAT_0121d4c8 != 0) {
        lv_group_focus_obj();
      }
      DAT_0121d4c8 = 0;
    }
    return;
  }
  DAT_01216690 = 0x3c;
  uVar2 = lv_disp_get_default();
  uVar2 = lv_disp_get_layer_top(uVar2);
  DAT_0121d4cc = lv_obj_create(uVar2);
  lv_obj_set_style_text_color(DAT_0121d4cc,0xffffffff,0);
  lv_obj_set_size(DAT_0121d4cc,0x2021,0x2021);
  lv_obj_align(DAT_0121d4cc,9,0,0);
  uVar2 = DAT_0121d4cc;
  lv_obj_set_style_pad_left(DAT_0121d4cc,0,0);
  lv_obj_set_style_pad_right(uVar2,0,0);
  lv_obj_set_style_border_width(DAT_0121d4cc,0,0);
  lv_obj_set_style_outline_width(DAT_0121d4cc,0,0);
  uVar2 = DAT_0121d4cc;
  lv_obj_set_style_pad_top(DAT_0121d4cc,0,0);
  lv_obj_set_style_pad_bottom(uVar2,0,0);
  uVar2 = DAT_0121d4cc;
  lv_palette_darken(local_18,0x12,2);
  lv_obj_set_style_bg_color(uVar2,local_18[0],0);
  lv_obj_set_style_bg_opa(DAT_0121d4cc,0xe5,0);
  uVar2 = lv_label_create(DAT_0121d4cc);
  lv_obj_align(uVar2,9,0,0);
  if (DAT_0121d4c4 == 0) {
    DAT_0121d4c4 = lv_timer_create(&LAB_0044b2d0,1000,uVar2);
  }
  lv_timer_set_repeat_count(DAT_0121d4c4,0x3d);
  lv_timer_ready(DAT_0121d4c4);
  uVar2 = lv_group_get_default();
  lv_group_add_obj(uVar2,DAT_0121d4cc);
  uVar2 = lv_group_get_default();
  DAT_0121d4c8 = lv_group_get_focused(uVar2);
  lv_group_focus_obj(DAT_0121d4cc);
  lv_obj_add_event_cb(DAT_0121d4cc,&LAB_0044b11c,0,0);
  return;
}



/* ===== FUNCTION FUN_0044b628 @ 0044b628 ===== */

void FUN_0044b628(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_00428454(0x19);
  uVar1 = FUN_00443828(param_1,0x39,&DAT_01216694,uVar1,0xc,0,0);
  uVar1 = lv_obj_get_child(uVar1,1);
  lv_obj_add_event_cb(uVar1,&LAB_0044b110,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044b698 @ 0044b698 ===== */

void FUN_0044b698(void)

{
  if (DAT_0121d4c0 != 0) {
    lv_timer_reset();
    if (DAT_0121d4c4 != 0) {
      lv_timer_del();
    }
    DAT_0121d4c4 = 0;
    if (DAT_0121d4cc != 0) {
      lv_obj_del();
    }
    DAT_0121d4cc = 0;
    if (DAT_0121d4c8 != 0) {
      lv_group_focus_obj();
    }
    DAT_0121d4c8 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_0044b720 @ 0044b720 ===== */

void FUN_0044b720(undefined4 param_1)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  iVar2 = lv_event_get_code();
  uVar3 = lv_event_get_target(param_1);
  if (iVar2 == 0xd) {
    uVar4 = lv_indev_get_act();
    sVar1 = lv_indev_get_key(uVar4);
    if ((sVar1 == 0x1b) || (sVar1 == 2)) {
      lv_obj_del(uVar3);
      FUN_00443e84();
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0044b7a4 @ 0044b7a4 ===== */

undefined4 FUN_0044b7a4(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_ac [35];
  undefined2 local_20;
  undefined2 local_1e;
  undefined2 local_1c;
  undefined4 *local_18;
  
  FUN_00406770(1);
  memset(&local_c0,0,0xa0);
  iVar1 = open("/dev/persistentmem",0x4012);
  if (iVar1 < 0) {
    puts("open /dev/persistentmem failed");
  }
  else {
    local_ac[0] = 1;
    local_20 = 1;
    local_1e = 0x14;
    local_1c = 4;
    local_18 = local_ac;
    iVar2 = ioctl(iVar1,0x800c2603,&local_20);
    if (-1 < iVar2) goto LAB_0044b828;
    puts("put sysdata failed");
    close(iVar1);
  }
  puts("set ota detect mode failed");
LAB_0044b828:
  FUN_0042cc6c();
  FUN_00427548(&DAT_0121d584);
  FUN_0042eee8();
  iVar1 = hcfota_url(DAT_0121d580,&LAB_0044ba18,0);
  FUN_0042eef4();
  local_bc = 0;
  local_c0 = 0x1c;
  if (iVar1 == 0) {
    local_bc = 5;
    FUN_004275f8(DAT_0121d584);
    uVar3 = lv_timer_create(&LAB_0044c338,1000,0);
    lv_timer_reset(uVar3);
    lv_timer_set_repeat_count(uVar3,4);
  }
  else {
    FUN_0042cd48();
    lv_mem_free(DAT_0121d580);
    FUN_00406770(0);
    switch(iVar1) {
    case -0x10:
      printf("version err!");
      local_bc = 7;
      break;
    case -0xf:
      printf("upgrade err");
      local_bc = 10;
      break;
    case -0xe:
      printf("decompress err!");
      local_bc = 9;
      break;
    case -0xd:
      printf("load file err!");
      local_bc = 3;
      break;
    case -10:
      printf("header crc err!");
      local_bc = 8;
    }
    FUN_0042c5b4(&local_c0);
    if (DAT_0121d39c != 0) {
      lv_timer_resume();
      lv_timer_reset(DAT_0121d39c);
      DAT_0121d3a0 = DAT_0121d39c;
      DAT_0121d39c = 0;
    }
  }
  if (DAT_01216848 != 0) {
    lv_timer_pause();
    lv_timer_del(DAT_01216848);
    DAT_01216848 = 0;
  }
  FUN_0042eec8(0);
  return 0;
}



/* ===== FUNCTION FUN_0044bad4 @ 0044bad4 ===== */

void FUN_0044bad4(uint *param_1)

{
  uint uVar1;
  size_t sVar2;
  char *__dest;
  undefined4 uVar3;
  int iVar4;
  void *__arg;
  undefined4 uVar5;
  undefined4 uVar6;
  pthread_attr_t pStack_48;
  pthread_t local_24;
  undefined4 local_20;
  undefined4 local_1c;
  
  if (DAT_0121d4d0 != (code *)0x0) {
    (*DAT_0121d4d0)();
  }
  uVar3 = DAT_0121d4d8;
  uVar1 = *param_1;
  if (uVar1 != 0x1c) {
    if (uVar1 < 0x1d) {
      if (uVar1 == 0x1a) {
        sVar2 = strlen((char *)&DAT_0121d4e0);
        __dest = (char *)lv_mem_alloc(sVar2 + 1);
        DAT_0121d580 = __dest;
        sVar2 = strlen((char *)&DAT_0121d4e0);
        memcpy(__dest,&DAT_0121d4e0,sVar2 + 1);
        FUN_0042eec8(1);
        puts(DAT_0121d580);
        uVar3 = lv_disp_get_default();
        iVar4 = lv_disp_get_scr_act(uVar3);
        if (DAT_0121d3ac == iVar4) {
          __arg = (void *)lv_obj_create(DAT_0121d398);
        }
        else {
          uVar3 = lv_disp_get_default();
          uVar3 = lv_disp_get_layer_top(uVar3);
          __arg = (void *)lv_obj_create(uVar3);
        }
        lv_obj_set_style_text_color(__arg,0xffffffff,0);
        lv_obj_set_style_outline_width(__arg,0,0);
        lv_obj_set_style_border_width(__arg,0,0);
        lv_palette_darken(&local_20,0x12,2);
        lv_obj_set_style_bg_color(__arg,local_20,0);
        lv_obj_set_scrollbar_mode(__arg,0);
        lv_obj_set_style_radius(__arg,0x14,0);
        lv_obj_set_size(__arg,0x2028,0x2019);
        lv_obj_align(__arg,9,0,0);
        uVar5 = lv_bar_create(__arg);
        lv_obj_align(uVar5,1,0,0);
        lv_obj_set_size(uVar5,0x2064,0x2014);
        lv_bar_set_range(uVar5,0,100);
        lv_bar_set_value(uVar5,0,0);
        lv_obj_align(uVar5,9,0,0);
        DAT_0121d4d4 = uVar5;
        uVar3 = lv_label_create(uVar5);
        lv_label_set_text(uVar3,&DAT_00455f80);
        lv_obj_align(uVar3,9,0,0);
        lv_obj_set_style_text_color(uVar3,0xffffffff,0);
        uVar3 = lv_label_create(__arg);
        DAT_0121d4d8 = uVar3;
        uVar6 = FUN_004386cc(0x37);
        lv_label_set_text(uVar3,uVar6);
        lv_obj_set_width(DAT_0121d4d8,0x2064);
        lv_obj_set_style_text_color(DAT_0121d4d8,0xffff0000,0);
        uVar3 = DAT_0121d4d8;
        uVar6 = FUN_0043783c(1);
        lv_obj_set_style_text_font(uVar3,uVar6,0);
        local_1c = DAT_0121d4d8;
        lv_obj_align_to(DAT_0121d4d8,uVar5,0xb,0,0xfffffffd);
        lv_obj_set_style_text_align(DAT_0121d4d8,2,0);
        uVar3 = lv_label_create(__arg);
        uVar6 = FUN_004386cc(0x36);
        lv_label_set_text(uVar3,uVar6);
        uVar6 = FUN_0043783c(1);
        lv_obj_set_style_text_font(uVar3,uVar6,0);
        lv_obj_set_style_text_color(uVar3,0xffff0000,0);
        lv_obj_align_to(uVar3,uVar5,0xe,0,3);
        lv_obj_set_style_text_align(uVar3,2,0);
        if (DAT_0121d3a0 != 0) {
          lv_timer_pause();
          DAT_0121d39c = DAT_0121d3a0;
          DAT_0121d3a0 = 0;
        }
        local_24 = 0;
        pthread_attr_init(&pStack_48);
        pthread_attr_setstacksize(&pStack_48,0x1000);
        pthread_attr_setdetachstate(&pStack_48,1);
        pthread_create(&local_24,&pStack_48,FUN_0044b7a4,__arg);
        pthread_attr_destroy(&pStack_48);
      }
    }
    else if (uVar1 < 0x1f) {
      uVar5 = 0x37;
      if (uVar1 != 0x1d) {
        uVar5 = 0x38;
      }
      uVar5 = FUN_004386cc(uVar5);
      lv_label_set_text(uVar3,uVar5);
      if (DAT_0121d4dc != DAT_012166d4) {
        lv_bar_set_value(DAT_0121d4d4,DAT_012166d4,0);
        uVar3 = lv_obj_get_child(DAT_0121d4d4,0);
        lv_label_set_text_fmt(uVar3,"%ld%%",DAT_012166d4);
        DAT_0121d4dc = DAT_012166d4;
        return;
      }
    }
    return;
  }
  switch(param_1[1]) {
  case 3:
    uVar5 = 0x3d;
    break;
  default:
    goto switchD_0044bf2c_caseD_4;
  case 5:
    uVar5 = FUN_004386cc(0x43);
    uVar6 = FUN_004386cc(0x44);
    lv_label_set_text_fmt(uVar3,"%s %d%s",uVar5,4,uVar6);
    goto LAB_0044bf54;
  case 7:
    uVar5 = 0x3f;
    break;
  case 8:
  case 10:
    uVar5 = 0x42;
    break;
  case 9:
    uVar5 = 0x41;
  }
  uVar5 = FUN_004386cc(uVar5);
  lv_label_set_text(uVar3,uVar5);
switchD_0044bf2c_caseD_4:
LAB_0044bf54:
  uVar3 = lv_timer_create(&LAB_0044ba80,3000,0);
  lv_timer_set_repeat_count(uVar3,1);
  lv_timer_reset(uVar3);
  return;
}



/* ===== FUNCTION FUN_0044c000 @ 0044c000 ===== */

undefined4 FUN_0044c000(void)

{
  __dev_t *p_Var1;
  int *piVar2;
  int iVar3;
  FILE *__stream;
  long lVar4;
  size_t sVar5;
  undefined4 uVar6;
  stat *psVar7;
  undefined4 *puVar8;
  char *__format;
  __ino_t _Var9;
  undefined4 uVar10;
  int iVar11;
  size_t __n;
  stat local_1a0;
  stat sStack_100;
  undefined1 auStack_70 [6];
  char local_6a;
  int local_68;
  undefined1 auStack_60 [48];
  char local_30 [20];
  
  memset(&local_1a0,0,0xa0);
  piVar2 = (int *)FUN_0042b4b4();
  if ((piVar2 != (int *)0x0) && (0 < *piVar2)) {
    iVar11 = 0;
    do {
      iVar3 = FUN_0042e3a4(iVar11);
      if (iVar3 == 0) {
        return 1;
      }
      memset(&local_1a0,0,0xa0);
      snprintf((char *)&local_1a0,0xa0,"%s/%s",iVar3,"HCFOTA.bin");
      iVar3 = __xstat(3,(char *)&local_1a0,&sStack_100);
      if (iVar3 == 0) {
        if (*piVar2 == iVar11) {
          return 1;
        }
        memset(auStack_70,0,0x40);
        __stream = fopen((char *)&local_1a0,"rb+");
        if (__stream == (FILE *)0x0) {
          printf("%s fopen:%s error.\n","version_check",&local_1a0);
          return 2;
        }
        fseek(__stream,0,2);
        lVar4 = ftell(__stream);
        fseek(__stream,0,0);
        if (lVar4 < 0x41) {
          __format = "%s fw header len error.\n";
          goto LAB_0044c1a8;
        }
        __n = 0x40;
        iVar11 = 0;
        goto LAB_0044c164;
      }
      iVar11 = iVar11 + 1;
    } while (iVar11 < *piVar2);
  }
  return 1;
  while (iVar11 = iVar11 + sVar5, __n != 0) {
LAB_0044c164:
    sVar5 = fread(auStack_70 + iVar11 * 0x40,1,__n,__stream);
    __n = __n - sVar5;
    if ((int)sVar5 < 1) break;
  }
  if (iVar11 == 0x40) {
    fclose(__stream);
    uVar6 = FUN_00428454(0x1e);
    printf("local product:%s, fw product:%s\n",uVar6,auStack_60);
    uVar6 = FUN_00428454(0x1f);
    printf("local version:%u, fw version:%u\n",uVar6,local_68);
    local_30[0] = '\0';
    local_30[1] = '\0';
    local_30[2] = '\0';
    local_30[3] = '\0';
    local_30[4] = '\0';
    local_30[5] = '\0';
    local_30[6] = '\0';
    local_30[7] = '\0';
    local_30[8] = '\0';
    local_30[9] = '\0';
    local_30[10] = '\0';
    local_30[0xb] = '\0';
    local_30[0xc] = '\0';
    local_30[0xd] = '\0';
    local_30[0xe] = '\0';
    local_30[0xf] = '\0';
    uVar6 = FUN_00428454(0x1e);
    snprintf(local_30,0x10,"%s",uVar6);
    iVar11 = memcmp(local_30,auStack_60,0x10);
    if (iVar11 == 0) {
      if ((local_6a == '\0') && (iVar11 = FUN_00428454(0x1f), local_68 == iVar11)) {
        uVar6 = 4;
      }
      else {
        printf("%s(), line: %d. found upg file:%s!\n","version_check",0x87,&local_1a0);
        psVar7 = &local_1a0;
        puVar8 = &DAT_0121d4e0;
        do {
          p_Var1 = &psVar7->st_dev;
          uVar10 = *(undefined4 *)((int)&psVar7->st_dev + 4);
          uVar6 = *(undefined4 *)&psVar7->__pad1;
          _Var9 = psVar7->st_ino;
          psVar7 = (stat *)&psVar7->st_mode;
          *puVar8 = (int)*p_Var1;
          puVar8[1] = uVar10;
          puVar8[2] = uVar6;
          puVar8[3] = _Var9;
          puVar8 = puVar8 + 4;
        } while (psVar7 != &sStack_100);
        printf("update file: %s\n",&DAT_0121d4e0);
        uVar6 = 0;
      }
    }
    else {
      uVar6 = 3;
    }
    return uVar6;
  }
  __format = "%s fw header len not match.\n";
LAB_0044c1a8:
  printf(__format,"version_check");
  fclose(__stream);
  return 2;
}



/* ===== FUNCTION FUN_0044c474 @ 0044c474 ===== */

void FUN_0044c474(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 local_20;
  code *local_1c;
  undefined4 local_18 [3];
  
  uVar1 = lv_obj_create(DAT_0121d398);
  DAT_0121d390 = uVar1;
  lv_obj_set_style_radius(uVar1,0x14,0);
  lv_obj_set_style_border_width(uVar1,0,0);
  lv_obj_set_style_outline_width(uVar1,0,0);
  lv_group_add_obj(DAT_0121d3a4,uVar1);
  lv_group_focus_obj(uVar1);
  lv_obj_add_event_cb(uVar1,FUN_0044b720,0,0);
  lv_palette_darken(local_18,0x12,2);
  lv_obj_set_style_bg_color(uVar1,local_18[0],0);
  lv_obj_set_size(uVar1,0x2019,0x2019);
  lv_obj_align(uVar1,9,0,0);
  lv_obj_set_scrollbar_mode(uVar1,0);
  opendir("/media/");
  iVar2 = FUN_0044c000();
  if (iVar2 == 0) {
    lv_obj_del(uVar1);
    puts("upgrade entry init");
    uVar1 = lv_disp_get_default();
    uVar1 = lv_disp_get_scr_act(uVar1);
    iVar2 = FUN_0042d004(uVar1);
    if (iVar2 == 0) {
      uVar1 = lv_disp_get_default();
      local_20 = lv_disp_get_scr_act(uVar1);
      local_1c = FUN_0044bad4;
      FUN_0042cef8(&local_20);
    }
    else {
      if (*(code **)(iVar2 + 4) != FUN_0044bad4) {
        DAT_0121d4d0 = *(code **)(iVar2 + 4);
      }
      *(code **)(iVar2 + 4) = FUN_0044bad4;
    }
    local_1c = (code *)0x0;
    local_20 = 0x1a;
    FUN_0042c5b4(&local_20);
  }
  else {
    uVar1 = lv_label_create(uVar1);
    if (iVar2 == 2) {
      uVar3 = FUN_004386cc(0x3d);
    }
    else if (iVar2 == 3) {
      uVar3 = FUN_004386cc(0x40);
    }
    else if (iVar2 == 4) {
      uVar3 = FUN_004386cc(0x3f);
    }
    else {
      iVar2 = FUN_0042b2e4();
      if ((iVar2 == 1) || (iVar2 == 6)) {
        uVar3 = FUN_004386cc(0x3b);
      }
      else {
        uVar3 = FUN_004386cc(0x3c);
      }
    }
    lv_obj_align(uVar1,9,0,0);
    lv_obj_set_size(uVar1,0x2064,0x2028);
    lv_label_set_recolor(uVar1,1);
    lv_label_set_text(uVar1,uVar3);
    uVar3 = FUN_0043783c(1);
    lv_obj_set_style_text_font(uVar1,uVar3,0);
    if (DAT_0121d3a0 != 0) {
      lv_timer_reset();
      lv_timer_resume(DAT_0121d3a0);
      return;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0044c7cc @ 0044c7cc ===== */

void FUN_0044c7cc(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  
  uVar2 = FUN_00428454(0);
  uVar2 = FUN_00443828(param_1,0xc,&DAT_012166e0,uVar2,0xc,0,0);
  iVar3 = FUN_00428454(0);
  if (iVar3 == 3) {
    puVar5 = &DAT_0121eddc;
    puVar6 = &DAT_01216710;
    do {
      puVar1 = puVar5 + 1;
      puVar5 = puVar5 + 2;
      uVar4 = FUN_00428454(*puVar1);
      *puVar6 = uVar4;
      puVar6 = puVar6 + 1;
    } while (puVar5 != (undefined4 *)&_end);
  }
  uVar2 = lv_obj_get_child(uVar2,1);
  lv_obj_add_event_cb(uVar2,&LAB_0044c7c0,0,param_1);
  return;
}



/* ===== FUNCTION FUN_0044c8a8 @ 0044c8a8 ===== */

void FUN_0044c8a8(undefined4 param_1,undefined4 param_2)

{
  int __fd;
  int iVar1;
  int iVar2;
  char *__format;
  undefined4 uVar3;
  undefined2 uVar4;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_2c = 0;
  __fd = open("/dev/fb0",2);
  if (__fd < 0) {
    printf("open /dev/fb0 failed, ret=%d\n",__fd);
    return;
  }
  iVar1 = ioctl(__fd,0x40180d02,&local_40);
  if (iVar1 == 0) {
    switch(param_2) {
    default:
      uVar4 = 0;
      break;
    case 1:
      local_3c = param_1;
      FUN_004286f8(1,param_1);
      uVar4 = 2;
      break;
    case 2:
      local_40 = param_1;
      FUN_004286f8(2,param_1);
      uVar4 = 1;
      break;
    case 3:
      local_30 = param_1;
      FUN_004286f8(3,param_1);
      uVar4 = 8;
      break;
    case 4:
      local_38 = param_1;
      FUN_004286f8(4,param_1);
      uVar4 = 4;
      break;
    case 5:
      uVar4 = 0x10;
      local_34 = param_1;
      FUN_004286f8(5,param_1);
    }
    iVar1 = ioctl(__fd,0x80180d01,&local_40);
    if (iVar1 == 0) {
      local_28 = 0;
      local_24 = 0;
      local_20 = 0;
      iVar1 = open("/dev/dis",2);
      if (iVar1 < 0) {
        printf("open /dev/dis failed, ret=%d\n",iVar1);
      }
      else {
        local_24 = CONCAT22((short)param_1,uVar4);
        local_28 = 1;
        iVar2 = ioctl(iVar1,0x800c0e06,&local_28);
        if (iVar2 == 0) {
          close(iVar1);
        }
        else {
          printf("%s:%d: warning: HCFBIOSET_VENHANCE failed\n","set_venhance1",0x79);
          close(iVar1);
        }
      }
      close(__fd);
      return;
    }
    uVar3 = 0xb8;
    __format = "%s:%d: warning: HCFBIOSET_ENHANCE failed\n";
  }
  else {
    uVar3 = 0x8d;
    __format = "%s:%d: warning: HCFBIOGET_ENHANCE failed\n";
  }
  printf(__format,"set_enhance1",uVar3);
  close(__fd);
  return;
}



/* ===== FUNCTION FUN_0044cb04 @ 0044cb04 ===== */

undefined4 FUN_0044cb04(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined **ppuVar7;
  undefined4 *puVar8;
  undefined **ppuVar9;
  char *__nptr;
  undefined *local_38 [4];
  undefined *local_28 [2];
  
  FUN_004286f8(0,param_1);
  local_28[0] = (undefined *)0x3040201;
  if (param_1 == 0) {
    local_38[0] = &DAT_00456194;
    local_38[1] = &DAT_00456194;
    local_38[2] = &DAT_00456194;
LAB_0044cc5c:
    local_38[3] = &DAT_00456198;
  }
  else {
    if (param_1 == 1) {
      local_38[0] = &DAT_004561a0;
      local_38[1] = &DAT_004561a4;
      local_38[2] = &DAT_004561a0;
      goto LAB_0044cc5c;
    }
    local_38[0] = &DAT_0045619c;
    local_38[2] = &DAT_0045619c;
    local_38[1] = &DAT_00456194;
    local_38[3] = &DAT_0044fda8;
    if (param_1 < 0) {
      return 0;
    }
    if (2 < param_1) {
      if (param_1 != 3) {
        return 0;
      }
      puVar5 = &DAT_01216710;
      ppuVar9 = local_28;
      puVar8 = &DAT_0121eddc;
      do {
        uVar1 = *(undefined1 *)ppuVar9;
        ppuVar9 = (undefined **)((int)ppuVar9 + 1);
        FUN_0044c8a8(*puVar5,uVar1);
        uVar6 = *puVar8;
        puVar8 = puVar8 + 2;
        iVar2 = lv_obj_has_state(uVar6,0x80);
        if (iVar2 != 0) {
          lv_obj_clear_state(uVar6,0x80);
        }
        lv_obj_set_style_text_color(uVar6,0xffffffff,0);
        uVar6 = lv_obj_get_child(uVar6,1);
        uVar4 = *puVar5;
        puVar5 = puVar5 + 1;
        lv_label_set_text_fmt(uVar6,&DAT_00452028,uVar4);
      } while (puVar5 != &DAT_01216720);
      goto LAB_0044cc14;
    }
  }
  ppuVar7 = local_38;
  puVar5 = &DAT_0121eddc;
  ppuVar9 = local_28;
  do {
    __nptr = *ppuVar7;
    ppuVar7 = ppuVar7 + 1;
    lVar3 = strtol(__nptr,(char **)0x0,10);
    uVar1 = *(undefined1 *)ppuVar9;
    ppuVar9 = (undefined **)((int)ppuVar9 + 1);
    FUN_0044c8a8(lVar3,uVar1);
    uVar6 = *puVar5;
    puVar5 = puVar5 + 2;
    iVar2 = lv_obj_has_state(uVar6,0x80);
    if (iVar2 == 0) {
      lv_obj_add_state(uVar6,0x80);
    }
    lv_obj_set_style_text_color(uVar6,0xff7d7d7d,0);
    uVar6 = lv_obj_get_child(uVar6,1);
    lv_label_set_text(uVar6,__nptr);
  } while (local_28 != ppuVar7);
LAB_0044cc14:
  putchar(10);
  return 0;
}



/* ===== FUNCTION FUN_0044cdf0 @ 0044cdf0 ===== */

void FUN_0044cdf0(int param_1,int param_2,int param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  double dVar4;
  
  if (DAT_0121d594 == 0) {
    uVar2 = lv_disp_get_default();
    uVar2 = lv_disp_get_layer_top(uVar2);
    DAT_0121d594 = lv_obj_create(uVar2);
    lv_obj_set_size(DAT_0121d594,0x2064,0x2064);
    lv_obj_set_style_bg_opa(DAT_0121d594,0,0);
    lv_obj_clear_flag(DAT_0121d594,0x10);
    lv_obj_set_style_border_width(DAT_0121d594,0,0);
  }
  if (DAT_0121d5a8 == 0) {
    DAT_0121d598 = FUN_00428454(0x16);
    dVar4 = (double)(int)DAT_0121d598;
    DAT_0121d5a8 = lv_obj_create(DAT_0121d594);
    lv_obj_set_size(DAT_0121d5a8,0x5a,0x131);
    lv_obj_align(DAT_0121d5a8,8,0x20,0);
    lv_obj_set_style_bg_opa(DAT_0121d5a8,0,0);
    lv_obj_clear_flag(DAT_0121d5a8,0x10);
    lv_obj_set_style_bg_img_src(DAT_0121d5a8,&DAT_0090f800,0);
    lv_obj_set_style_border_width(DAT_0121d5a8,0,0);
    DAT_0121d5a4 = lv_obj_create(DAT_0121d5a8);
    lv_obj_set_style_radius(DAT_0121d5a4,10,0);
    uVar1 = (ushort)(int)(dVar4 * 0.788);
    if ((int)(dVar4 * 0.788) < 0) {
      uVar1 = 1000 - uVar1;
    }
    lv_obj_set_size(DAT_0121d5a4,0x2026,(int)(short)(uVar1 | 0x2000));
    lv_obj_align(DAT_0121d5a4,5,0,0x23ec);
    lv_obj_set_style_bg_opa(DAT_0121d5a4,0,0);
    lv_obj_clear_flag(DAT_0121d5a4,0x10);
    lv_obj_set_style_bg_img_src(DAT_0121d5a4,&DAT_008bffc0,0);
    lv_obj_set_style_border_width(DAT_0121d5a4,0,0);
    DAT_0121d59c = lv_obj_create(DAT_0121d5a8);
    lv_obj_set_style_radius(DAT_0121d59c,10,0);
    lv_obj_set_size(DAT_0121d59c,0x2028,0x2050);
    lv_obj_align(DAT_0121d59c,5,0,0x23eb);
    lv_obj_set_style_bg_opa(DAT_0121d59c,0,0);
    lv_obj_set_style_border_color(DAT_0121d59c,0xff808080,0);
    lv_obj_set_style_border_width(DAT_0121d59c,6,0);
    if (DAT_0121d590 == 0) {
      DAT_0121d590 = lv_timer_create(&LAB_0044cd40,3000,DAT_0121d5a4);
      lv_timer_set_repeat_count(DAT_0121d590,1);
      lv_timer_reset(DAT_0121d590);
    }
    goto LAB_0044ce98;
  }
  if (param_2 == param_1) {
    DAT_0121d598 = DAT_0121d598 + 3;
  }
  else if (param_1 == param_3) {
    DAT_0121d598 = DAT_0121d598 - 3;
  }
  iVar3 = 0x4e;
  if ((int)DAT_0121d598 < 100) {
    if ((int)DAT_0121d598 < 0) {
      DAT_0121d598 = 0;
      iVar3 = 0;
      goto LAB_0044ce64;
    }
    iVar3 = (int)((double)(int)DAT_0121d598 * 0.788);
    if (-1 < iVar3) goto LAB_0044ce64;
    uVar1 = 1000 - (short)iVar3;
  }
  else {
    DAT_0121d598 = 99;
LAB_0044ce64:
    uVar1 = (ushort)iVar3;
  }
  lv_obj_set_size(DAT_0121d5a4,0x2026,(int)(short)(uVar1 | 0x2000));
  FUN_0042d734(DAT_0121d598 & 0xff);
  if (DAT_0121d590 != 0) {
    lv_timer_reset();
  }
LAB_0044ce98:
  if ((int)DAT_0121d598 < 1) {
    if (DAT_0121d5a0 == 0) {
      DAT_0121d5a0 = lv_obj_create(DAT_0121d5a8);
      lv_obj_set_size(DAT_0121d5a0,0x2064,0x2008);
      lv_obj_align(DAT_0121d5a0,2,0x2014,0x2005);
      lv_obj_set_style_bg_opa(DAT_0121d5a0,0,0);
      lv_obj_clear_flag(DAT_0121d5a0,0x10);
      lv_obj_set_style_bg_img_src(DAT_0121d5a0,&DAT_00ab1e00,0);
      lv_obj_set_style_border_width(DAT_0121d5a0,0,0);
      return;
    }
  }
  else if (DAT_0121d5a0 != 0) {
    lv_obj_del();
    DAT_0121d5a0 = 0;
  }
  return;
}



/* ===== FUNCTION FUN_0044d240 @ 0044d240 ===== */

void FUN_0044d240(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined2 *puVar4;
  undefined8 uVar5;
  
  iVar2 = FUN_0042ebb8();
  iVar1 = DAT_0121d5c8;
  puVar4 = &DAT_01216724 + param_1;
  iVar3 = FUN_00419c60(*puVar4);
  uVar5 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
  if (iVar3 != 0) {
    if (iVar3 == 1) {
      uVar5 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
      if (200 < (uint)(iVar2 - iVar1)) {
        FUN_0042a2c8(*puVar4,1);
        uVar5 = FUN_0042ebb8();
      }
    }
    else {
      uVar5 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
      if ((iVar3 == 2) &&
         (uVar5 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8), DAT_01216720 < (uint)(iVar2 - iVar1))) {
        FUN_0042a2c8(*puVar4,1);
        uVar5 = FUN_0042ebb8();
        if ((int)DAT_01216720 < 0x1a) {
          DAT_01216720 = 0x10;
        }
        else {
          DAT_01216720 = DAT_01216720 - 10;
        }
      }
    }
  }
  DAT_0121d5cc = (undefined4)((ulonglong)uVar5 >> 0x20);
  DAT_0121d5c8 = (int)uVar5;
  return;
}



/* ===== FUNCTION FUN_0044d354 @ 0044d354 ===== */

bool FUN_0044d354(int param_1)

{
  FUN_0042ebb8();
  return *(char *)((int)&DAT_0121d5b4 + param_1) == '\x01';
}



/* ===== FUNCTION FUN_0044d390 @ 0044d390 ===== */

void FUN_0044d390(uint param_1)

{
  int iVar1;
  uint uVar2;
  char *pcVar3;
  undefined4 *puVar4;
  uint uVar5;
  short *psVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined *local_30;
  
  uVar8 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
  pcVar3 = (char *)&DAT_0121d5b4;
  psVar6 = &DAT_01216724;
  puVar4 = &local_40;
  uVar5 = 0;
  local_40 = 0;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = &DAT_01210000;
  do {
    DAT_0121d5cc = (int)((ulonglong)uVar8 >> 0x20);
    DAT_0121d5c8 = (int)uVar8;
    if (((param_1 >> (uVar5 & 0x1f) & 1) == 0) || (*psVar6 == -1)) {
      if (*pcVar3 == '\x01') {
        FUN_0042a0e8();
        FUN_0042a2c8(*psVar6,0);
        uVar8 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
        DAT_0121d5b0 = '\x01';
      }
    }
    else {
      *(undefined1 *)puVar4 = 1;
      FUN_0042ebb8();
      if (*pcVar3 == '\x01') {
        FUN_0044d240(uVar5);
        uVar8 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
      }
      else {
        iVar1 = FUN_00419c60(*psVar6);
        if (iVar1 == 2) {
          *(undefined4 *)(local_30 + 0x6720) = 0xd0;
        }
        uVar7 = FUN_0042ebb8();
        uVar8 = CONCAT44(DAT_0121d5cc,DAT_0121d5c8);
        uVar2 = (uint)uVar7 - DAT_0121d5c8;
        if (((int)((ulonglong)uVar7 >> 0x20) - DAT_0121d5cc != (uint)((uint)uVar7 < uVar2)) ||
           (200 < uVar2)) {
          FUN_0042a2c8(*psVar6,1);
          uVar8 = FUN_0042ebb8();
        }
      }
      DAT_0121d5b0 = '\x01';
    }
    DAT_0121d5cc = (int)((ulonglong)uVar8 >> 0x20);
    DAT_0121d5c8 = (int)uVar8;
    uVar5 = uVar5 + 1;
    pcVar3 = pcVar3 + 1;
    psVar6 = psVar6 + 1;
    puVar4 = (undefined4 *)((int)puVar4 + 1);
  } while (uVar5 != 0x10);
  if (DAT_0121d5b0 != '\0') {
    DAT_0121d5b0 = '\0';
    DAT_0121d5b4 = local_40;
    DAT_0121d5b8 = local_3c;
    DAT_0121d5bc = local_38;
    DAT_0121d5c0 = local_34;
  }
  return;
}



/* ===== FUNCTION FUN_0044d690 @ 0044d690 ===== */

void FUN_0044d690(void)

{
  int iVar1;
  pthread_attr_t pStack_30;
  pthread_t local_c [2];
  
  local_c[0] = 0;
  pthread_attr_init(&pStack_30);
  pthread_attr_setstacksize(&pStack_30,0x1000);
  pthread_attr_setdetachstate(&pStack_30,1);
  iVar1 = pthread_create(local_c,&pStack_30,(__start_routine *)&LAB_0044d554,(void *)0x0);
  if (iVar1 != 0) {
    return;
  }
  pthread_attr_destroy(&pStack_30);
  return;
}



/* ===== FUNCTION FUN_0044d700 @ 0044d700 ===== */

void FUN_0044d700(void)

{
  if (DAT_0121d5e0 != 0) {
    FUN_004286f8(2,DAT_0121d5d4);
    FUN_00428080();
    if (DAT_0121d5e0 != 0) {
      lv_obj_del();
      DAT_0121d5e0 = 0;
    }
    if (DAT_0121d5d0 != 0) {
      lv_timer_del();
      DAT_0121d5d0 = 0;
    }
  }
  return;
}



/* ===== FUNCTION FUN_0044d78c @ 0044d78c ===== */

void FUN_0044d78c(int param_1,int param_2,int param_3)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  double dVar4;
  
  if (DAT_0121d594 == 0) {
    uVar2 = lv_disp_get_default();
    uVar2 = lv_disp_get_layer_top(uVar2);
    DAT_0121d594 = lv_obj_create(uVar2);
    lv_obj_set_size(DAT_0121d594,0x2064,0x2064);
    lv_obj_set_style_bg_opa(DAT_0121d594,0,0);
    lv_obj_clear_flag(DAT_0121d594,0x10);
    lv_obj_set_style_border_width(DAT_0121d594,0,0);
  }
  if (DAT_0121d5e0 == 0) {
    DAT_0121d5d4 = FUN_00428454(2);
    dVar4 = (double)DAT_0121d5d4;
    DAT_0121d5e0 = lv_obj_create(DAT_0121d594);
    lv_obj_set_size(DAT_0121d5e0,0x5a,0x131);
    lv_obj_align(DAT_0121d5e0,8,0x20,0);
    lv_obj_set_style_bg_opa(DAT_0121d5e0,0,0);
    lv_obj_clear_flag(DAT_0121d5e0,0x10);
    lv_obj_set_style_bg_img_src(DAT_0121d5e0,&DAT_00aaa360,0);
    lv_obj_set_style_border_width(DAT_0121d5e0,0,0);
    DAT_0121d5dc = lv_obj_create(DAT_0121d5e0);
    lv_obj_set_style_radius(DAT_0121d5dc,10,0);
    uVar1 = (ushort)(int)(dVar4 * 0.788);
    if ((int)(dVar4 * 0.788) < 0) {
      uVar1 = 1000 - uVar1;
    }
    lv_obj_set_size(DAT_0121d5dc,0x2026,(int)(short)(uVar1 | 0x2000));
    lv_obj_align(DAT_0121d5dc,5,0,0x23ec);
    lv_obj_set_style_bg_opa(DAT_0121d5dc,0,0);
    lv_obj_clear_flag(DAT_0121d5dc,0x10);
    lv_obj_set_style_bg_img_src(DAT_0121d5dc,&DAT_008bffc0,0);
    lv_obj_set_style_border_width(DAT_0121d5dc,0,0);
    DAT_0121d5d8 = lv_obj_create(DAT_0121d5e0);
    lv_obj_set_style_radius(DAT_0121d5d8,10,0);
    lv_obj_set_size(DAT_0121d5d8,0x2028,0x2050);
    lv_obj_align(DAT_0121d5d8,5,0,0x23eb);
    lv_obj_set_style_bg_opa(DAT_0121d5d8,0,0);
    lv_obj_set_style_border_color(DAT_0121d5d8,0xff808080,0);
    lv_obj_set_style_border_width(DAT_0121d5d8,6,0);
    if (DAT_0121d5d0 != 0) {
      return;
    }
    DAT_0121d5d0 = lv_timer_create(FUN_0044d700,3000,DAT_0121d5dc);
    lv_timer_set_repeat_count(DAT_0121d5d0,1);
    goto LAB_0044d82c;
  }
  if (param_2 == param_1) {
    DAT_0121d5d4 = DAT_0121d5d4 + 1;
  }
  else if (param_1 == param_3) {
    DAT_0121d5d4 = DAT_0121d5d4 + -1;
  }
  iVar3 = 0x4e;
  if (DAT_0121d5d4 < 0x65) {
    if (DAT_0121d5d4 < 0) {
      DAT_0121d5d4 = 0;
      iVar3 = 0;
      goto LAB_0044d800;
    }
    iVar3 = (int)((double)DAT_0121d5d4 * 0.788);
    if (-1 < iVar3) goto LAB_0044d800;
    uVar1 = 1000 - (short)iVar3;
  }
  else {
    DAT_0121d5d4 = 100;
LAB_0044d800:
    uVar1 = (ushort)iVar3;
  }
  lv_obj_set_size(DAT_0121d5dc,0x2026,(int)(short)(uVar1 | 0x2000));
  FUN_0044c8a8(DAT_0121d5d4,2);
  if (DAT_0121d5d0 == 0) {
    return;
  }
LAB_0044d82c:
  lv_timer_reset(DAT_0121d5d0);
  return;
}



/* ===== FUNCTION FUN_0044db30 @ 0044db30 ===== */

void FUN_0044db30(int param_1,int param_2)

{
  DAT_0121d5f8 = param_1 + (0x188004e0 - param_2);
  DAT_0121d5fc = param_1 + (0x18800520 - param_2);
  DAT_0121d600 = param_1 + (0x18800560 - param_2);
  DAT_0121d5f4 = param_1 + (0x188004a0 - param_2);
  DAT_0121d5f0 = param_1 + (0x18800184 - param_2);
  return;
}



/* ===== FUNCTION FUN_0044dc24 @ 0044dc24 ===== */

undefined4 FUN_0044dc24(uint param_1)

{
  int in_v0;
  
  **(uint **)(in_v0 + -0x2a10) = **(uint **)(in_v0 + -0x2a10) | 0x1000000;
  if (param_1 < 0x60) {
    *(undefined1 *)(DAT_0121d5fc + -0x40 + param_1) = 0x38;
    return 0;
  }
  if (param_1 < 0x80) {
    *(undefined1 *)(DAT_0121d600 + -0x60 + param_1) = 0x38;
    return 0;
  }
  return 0xffffffff;
}



/* ===== FUNCTION FUN_0044dca0 @ 0044dca0 ===== */

undefined4 FUN_0044dca0(void)

{
  int __fd;
  undefined4 uVar1;
  
  uVar1 = 0;
  if (DAT_0121d620 == (void *)0x0) {
    __fd = open("/dev/mem",2);
    if (__fd < 0) {
      uVar1 = 0xfffffffb;
    }
    else {
      DAT_0121d620 = mmap((void *)0x0,0x2020,3,1,__fd,0x18800000);
      if (DAT_0121d620 == (void *)0xffffffff) {
        DAT_0121d620 = (void *)0x0;
        uVar1 = 0xffffffff;
      }
      else {
        DAT_0121d610 = (int)DAT_0121d620 + 0x44;
        DAT_0121d614 = (int)DAT_0121d620 + 0xc4;
        DAT_0121d618 = (int)DAT_0121d620 + 0xe4;
        DAT_0121d61c = (int)DAT_0121d620 + 0x344;
        FUN_0044db30(DAT_0121d620,0x18800000);
        uVar1 = 0;
      }
    }
  }
  return uVar1;
}



/* ===== FUNCTION FUN_0044df60 @ 0044df60 ===== */

void FUN_0044df60(void)

{
  if (DAT_0121d634 != 0) {
    lv_obj_del();
    DAT_0121d634 = 0;
  }
  DAT_0121d648 = 0;
  return;
}



/* ===== FUNCTION FUN_0044dfa0 @ 0044dfa0 ===== */

void FUN_0044dfa0(void)

{
  int iVar1;
  
  FUN_004286f8(0x16,DAT_0121d630);
  FUN_00428080();
  if (DAT_0121684c != 0) {
    lv_obj_del();
    DAT_0121684c = 0;
  }
  FUN_00448950(0);
  iVar1 = lv_obj_is_valid(DAT_0121d638);
  if ((iVar1 != 0) &&
     ((iVar1 = lv_group_get_default(), DAT_0121d640 == iVar1 ||
      (iVar1 = lv_group_get_default(), DAT_0121d63c == iVar1)))) {
    lv_group_set_default(DAT_0121d63c);
    lv_indev_set_group(_edata,DAT_0121d63c);
    lv_group_focus_obj(DAT_0121d638);
  }
  DAT_0121d64c = 0;
  return;
}



/* ===== FUNCTION FUN_0044e38c @ 0044e38c ===== */

void FUN_0044e38c(void)

{
  undefined4 uVar1;
  int __fd;
  byte local_10 [8];
  
  uVar1 = lv_disp_get_default();
  DAT_0121d644 = lv_disp_get_layer_top(uVar1);
  local_10[0] = FUN_00428454(0x16);
  __fd = open("/dev/sndC0i2so",1);
  if (-1 < __fd) {
    ioctl(__fd,0x8001080b,local_10);
    local_10[0] = 0;
    ioctl(__fd,0x4001080c,local_10);
    printf("%s volume is %d\n","set_volume1",(uint)local_10[0]);
    close(__fd);
    return;
  }
  printf("open snd_fd %d failed\n",__fd);
  return;
}



/* ===== FUNCTION FUN_0044e44c @ 0044e44c ===== */

void FUN_0044e44c(void)

{
  if (DAT_0121d64c != 0) {
    lv_timer_del();
    DAT_0121d64c = 0;
  }
  if (DAT_0121684c != 0) {
    lv_obj_del();
    DAT_0121684c = 0;
    lv_group_del(DAT_0121d640);
    DAT_0121d640 = 0;
    FUN_00448950(0);
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0044e4c8 @ 0044e4c8 ===== */

void FUN_0044e4c8(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (DAT_0121684c == 0) {
    FUN_00448950(1);
    DAT_0121d63c = lv_group_get_default();
    DAT_0121d638 = lv_group_get_focused(DAT_0121d63c);
    DAT_0121d640 = lv_group_create();
    lv_group_set_default(DAT_0121d640);
    lv_indev_set_group(_edata,DAT_0121d640);
    DAT_0121684c = FUN_0043ffc4(DAT_0121d644,0x46,9);
    DAT_0121d630 = FUN_00428454(0x16);
    lv_obj_set_style_bg_opa(DAT_0121684c,0x99,0);
    lv_obj_set_style_outline_width(DAT_0121684c,0,0);
    iVar1 = DAT_0121684c;
    uVar2 = FUN_004386cc(0x99);
    FUN_00440134(iVar1,uVar2,0x19,100);
    uVar2 = FUN_00428454(0x16);
    printf("%d\n",uVar2);
    iVar1 = DAT_0121684c;
    iVar3 = FUN_00428454(0x16);
    uVar2 = FUN_00440290(iVar1,0x41,0x2c,iVar3 / 3,3);
    lv_obj_add_event_cb(uVar2,&LAB_0044e07c,0,0);
    iVar1 = DAT_0121684c;
    iVar3 = FUN_00428454(0x16);
    FUN_004404b8(iVar1,10,100,iVar3 / 3);
    if (DAT_0121d64c != 0) {
      return;
    }
    DAT_0121d64c = lv_timer_create(FUN_0044dfa0,3000,uVar2);
    lv_timer_set_repeat_count(DAT_0121d64c,1);
  }
  else if (DAT_0121d64c == 0) {
    return;
  }
  lv_timer_reset(DAT_0121d64c);
  return;
}



/* ===== FUNCTION FUN_0044e6b0 @ 0044e6b0 ===== */

void FUN_0044e6b0(void)

{
  if (DAT_01216750 != '\0') {
    if (DAT_0121d634 == 0) {
      DAT_0121d634 = lv_img_create(DAT_0121d644);
      lv_obj_align(DAT_0121d634,1,0x1e,0x1e);
    }
    lv_img_set_src(DAT_0121d634,&DAT_00b6d5e0);
    if (DAT_0121d648 != 0) {
      lv_timer_pause();
    }
    DAT_01216750 = 0;
    return;
  }
  if (DAT_0121d634 == 0) {
    DAT_0121d634 = lv_img_create(DAT_0121d644);
    lv_obj_align(DAT_0121d634,1,0x1e,0x1e);
  }
  lv_img_set_src(DAT_0121d634,&DAT_00afb4e0);
  DAT_01216750 = 1;
  if (DAT_0121d648 == 0) {
    DAT_0121d648 = lv_timer_create(FUN_0044df60,3000,0);
    lv_timer_set_repeat_count(DAT_0121d648,1);
    lv_timer_reset(DAT_0121d648);
    return;
  }
  lv_timer_resume();
  lv_timer_reset(DAT_0121d648);
  return;
}



/* ===== FUNCTION FUN_0044e808 @ 0044e808 ===== */

undefined8 FUN_0044e808(uint param_1,int param_2)

{
  uint uVar1;
  double dVar2;
  
  uVar1 = (uint)(param_1 - 1 < param_1) + param_2 + 0x1fffff;
  if (((0x3fffff < uVar1) || ((uVar1 == 0x3fffff && (param_1 - 1 == 0xffffffff)))) &&
     ((param_1 & 0x7ff) != 0)) {
    param_1 = param_1 & 0xfffff800 | 0x800;
  }
  if ((int)param_1 < 0) {
    dVar2 = (double)(int)param_1 + 4294967296.0 + (double)param_2 * 4294967296.0;
    return CONCAT44((int)((ulonglong)dVar2 >> 0x20),(float)dVar2);
  }
  dVar2 = (double)(int)param_1 + (double)param_2 * 4294967296.0;
  return CONCAT44((int)((ulonglong)dVar2 >> 0x20),(float)dVar2);
}



/* ===== FUNCTION FUN_0044e8c8 @ 0044e8c8 ===== */

void FUN_0044e8c8(int param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  
  if ((int)param_2 < 0) {
    param_2 = -(uint)(param_1 != 0) - param_2;
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
  }
  if ((int)param_4 < 0) {
    param_3 = -param_3;
    param_4 = -(uint)(param_3 != 0) - param_4;
    uVar1 = ~uVar1;
  }
  if (param_4 == 0) {
    if (param_2 < param_3) {
      if (LZCOUNT(param_3) != 0) {
        param_3 = param_3 << (LZCOUNT(param_3) & 0x1fU);
      }
      if (param_3 >> 0x10 == 0) {
        trap(7);
        trap(7);
        trap(7);
        trap(7);
      }
    }
    else {
      if (param_3 == 0) {
        trap(7);
        param_3 = 1 / 0;
      }
      if (LZCOUNT(param_3) == 0) {
        param_3 = param_3 >> 0x10;
      }
      else {
        param_3 = (param_3 << (LZCOUNT(param_3) & 0x1fU)) >> 0x10;
        if (param_3 == 0) {
          trap(7);
          trap(7);
          trap(7);
          trap(7);
        }
      }
      if (param_3 == 0) {
        trap(7);
        trap(7);
        trap(7);
        trap(7);
      }
    }
  }
  else {
    uVar2 = LZCOUNT(param_4);
    if (((param_4 <= param_2) && (uVar2 != 0)) &&
       ((param_3 >> (0x20 - uVar2 & 0x1f) | param_4 << (uVar2 & 0x1f)) >> 0x10 == 0)) {
      trap(7);
      trap(7);
      trap(7);
      trap(7);
    }
  }
  if (uVar1 != 0) {
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0044e8d0 @ 0044e8d0 ===== */

void FUN_0044e8d0(int param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  uint uVar2;
  
  if ((int)param_2 < 0) {
    param_2 = -(uint)(param_1 != 0) - param_2;
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = 0;
  }
  if ((int)param_4 < 0) {
    param_3 = -param_3;
    param_4 = -(uint)(param_3 != 0) - param_4;
    uVar1 = ~uVar1;
  }
  if (param_4 == 0) {
    if (param_2 < param_3) {
      if (LZCOUNT(param_3) != 0) {
        param_3 = param_3 << (LZCOUNT(param_3) & 0x1fU);
      }
      if (param_3 >> 0x10 == 0) {
        trap(7);
        trap(7);
        trap(7);
        trap(7);
      }
    }
    else {
      if (param_3 == 0) {
        trap(7);
        param_3 = 1 / 0;
      }
      if (LZCOUNT(param_3) == 0) {
        param_3 = param_3 >> 0x10;
      }
      else {
        param_3 = (param_3 << (LZCOUNT(param_3) & 0x1fU)) >> 0x10;
        if (param_3 == 0) {
          trap(7);
          trap(7);
          trap(7);
          trap(7);
        }
      }
      if (param_3 == 0) {
        trap(7);
        trap(7);
        trap(7);
        trap(7);
      }
    }
  }
  else {
    uVar2 = LZCOUNT(param_4);
    if (((param_4 <= param_2) && (uVar2 != 0)) &&
       ((param_3 >> (0x20 - uVar2 & 0x1f) | param_4 << (uVar2 & 0x1f)) >> 0x10 == 0)) {
      trap(7);
      trap(7);
      trap(7);
      trap(7);
    }
  }
  if (uVar1 != 0) {
    return;
  }
  return;
}



/* ===== FUNCTION FUN_0044ee90 @ 0044ee90 ===== */

void FUN_0044ee90(void)

{
  undefined4 *puVar1;
  code *pcVar2;
  
  if (DAT_011e94a0 != (code *)0xffffffff) {
    puVar1 = &DAT_011e94a0;
    pcVar2 = DAT_011e94a0;
    do {
      puVar1 = puVar1 + -1;
      (*pcVar2)();
      pcVar2 = (code *)*puVar1;
    } while (pcVar2 != (code *)0xffffffff);
  }
  return;
}



/* ===== FUNCTION _fini @ 0044eef0 ===== */

void _fini(void)

{
  FUN_00406334();
  return;
}



/* ===== FUNCTION lv_obj_set_style_transform_zoom @ 011d7c40 ===== */

void lv_obj_set_style_transform_zoom(void)

{
  lv_obj_set_style_transform_zoom();
  return;
}



/* ===== FUNCTION open @ 011d7c50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int open(char *__file,int __oflag,...)

{
  int iVar1;
  
  iVar1 = open(__file,__oflag);
  return iVar1;
}



/* ===== FUNCTION pthread_mutex_destroy @ 011d7c60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_destroy(pthread_mutex_t *__mutex)

{
  int iVar1;
  
  iVar1 = pthread_mutex_destroy(__mutex);
  return iVar1;
}



/* ===== FUNCTION lv_obj_add_style @ 011d7c70 ===== */

void lv_obj_add_style(void)

{
  lv_obj_add_style();
  return;
}



/* ===== FUNCTION lv_mem_alloc @ 011d7c80 ===== */

void lv_mem_alloc(void)

{
  lv_mem_alloc();
  return;
}



/* ===== FUNCTION strcat @ 011d7c90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcat(char *__dest,char *__src)

{
  char *pcVar1;
  
  pcVar1 = strcat(__dest,__src);
  return pcVar1;
}



/* ===== FUNCTION lv_font_get_glyph_dsc_fmt_txt @ 011d7ca0 ===== */

void lv_font_get_glyph_dsc_fmt_txt(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION zip_fclose @ 011d7cb0 ===== */

void zip_fclose(void)

{
  zip_fclose();
  return;
}



/* ===== FUNCTION lv_obj_get_index @ 011d7cc0 ===== */

void lv_obj_get_index(void)

{
  lv_obj_get_index();
  return;
}



/* ===== FUNCTION fread @ 011d7cd0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fread(void *__ptr,size_t __size,size_t __n,FILE *__stream)

{
  size_t sVar1;
  
  sVar1 = fread(__ptr,__size,__n,__stream);
  return sVar1;
}



/* ===== FUNCTION lv_group_remove_obj @ 011d7ce0 ===== */

void lv_group_remove_obj(void)

{
  lv_group_remove_obj();
  return;
}



/* ===== FUNCTION lv_disp_get_hor_res @ 011d7cf0 ===== */

void lv_disp_get_hor_res(void)

{
  lv_disp_get_hor_res();
  return;
}



/* ===== FUNCTION lv_palette_main @ 011d7d00 ===== */

void lv_palette_main(void)

{
  lv_palette_main();
  return;
}



/* ===== FUNCTION hcplayer_stop2 @ 011d7d10 ===== */

void hcplayer_stop2(void)

{
  hcplayer_stop2();
  return;
}



/* ===== FUNCTION lv_list_create @ 011d7d20 ===== */

void lv_list_create(void)

{
  lv_list_create();
  return;
}



/* ===== FUNCTION __ctype_tolower_loc @ 011d7d30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__int32_t ** __ctype_tolower_loc(void)

{
  __int32_t **pp_Var1;
  
  pp_Var1 = __ctype_tolower_loc();
  return pp_Var1;
}



/* ===== FUNCTION lv_obj_set_flex_align @ 011d7d40 ===== */

void lv_obj_set_flex_align(void)

{
  lv_obj_set_flex_align();
  return;
}



/* ===== FUNCTION socket @ 011d7d50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int socket(int __domain,int __type,int __protocol)

{
  int iVar1;
  
  iVar1 = socket(__domain,__type,__protocol);
  return iVar1;
}



/* ===== FUNCTION access @ 011d7d60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int access(char *__name,int __type)

{
  int iVar1;
  
  iVar1 = access(__name,__type);
  return iVar1;
}



/* ===== FUNCTION lv_draw_img @ 011d7d70 ===== */

void lv_draw_img(void)

{
  lv_draw_img();
  return;
}



/* ===== FUNCTION hcplayer_deinit @ 011d7d80 ===== */

void hcplayer_deinit(void)

{
  hcplayer_deinit();
  return;
}



/* ===== FUNCTION lv_btnmatrix_clear_btn_ctrl_all @ 011d7d90 ===== */

void lv_btnmatrix_clear_btn_ctrl_all(void)

{
  lv_btnmatrix_clear_btn_ctrl_all();
  return;
}



/* ===== FUNCTION lv_tabview_add_tab @ 011d7da0 ===== */

void lv_tabview_add_tab(void)

{
  lv_tabview_add_tab();
  return;
}



/* ===== FUNCTION hcplayer_change_video_track @ 011d7db0 ===== */

void hcplayer_change_video_track(void)

{
  hcplayer_change_video_track();
  return;
}



/* ===== FUNCTION mkdir @ 011d7dc0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int mkdir(char *__path,__mode_t __mode)

{
  int iVar1;
  
  iVar1 = mkdir(__path,__mode);
  return iVar1;
}



/* ===== FUNCTION lv_obj_has_flag @ 011d7de0 ===== */

void lv_obj_has_flag(void)

{
  lv_obj_has_flag();
  return;
}



/* ===== FUNCTION glist_nth_prev @ 011d7df0 ===== */

void glist_nth_prev(void)

{
  glist_nth_prev();
  return;
}



/* ===== FUNCTION hcplayer_set_display_rect @ 011d7e00 ===== */

void hcplayer_set_display_rect(void)

{
  hcplayer_set_display_rect();
  return;
}



/* ===== FUNCTION strstr @ 011d7e10 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strstr(char *__haystack,char *__needle)

{
  char *pcVar1;
  
  pcVar1 = strstr(__haystack,__needle);
  return pcVar1;
}



/* ===== FUNCTION _lv_ll_get_next @ 011d7e20 ===== */

void _lv_ll_get_next(void)

{
  _lv_ll_get_next();
  return;
}



/* ===== FUNCTION fopen @ 011d7e30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fopen(char *__filename,char *__modes)

{
  FILE *pFVar1;
  
  pFVar1 = fopen(__filename,__modes);
  return pFVar1;
}



/* ===== FUNCTION lv_scr_load_anim @ 011d7e40 ===== */

void lv_scr_load_anim(void)

{
  lv_scr_load_anim();
  return;
}



/* ===== FUNCTION lv_style_set_pad_top @ 011d7e50 ===== */

void lv_style_set_pad_top(void)

{
  lv_style_set_pad_top();
  return;
}



/* ===== FUNCTION pthread_attr_destroy @ 011d7e60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_destroy(pthread_attr_t *__attr)

{
  int iVar1;
  
  iVar1 = pthread_attr_destroy(__attr);
  return iVar1;
}



/* ===== FUNCTION lv_group_set_default @ 011d7e70 ===== */

void lv_group_set_default(void)

{
  lv_group_set_default();
  return;
}



/* ===== FUNCTION lv_label_get_text @ 011d7e90 ===== */

void lv_label_get_text(void)

{
  lv_label_get_text();
  return;
}



/* ===== FUNCTION lv_obj_set_style_bg_grad_dir @ 011d7ea0 ===== */

void lv_obj_set_style_bg_grad_dir(void)

{
  lv_obj_set_style_bg_grad_dir();
  return;
}



/* ===== FUNCTION lv_style_set_text_color @ 011d7eb0 ===== */

void lv_style_set_text_color(void)

{
  lv_style_set_text_color();
  return;
}



/* ===== FUNCTION lv_style_set_border_side @ 011d7ec0 ===== */

void lv_style_set_border_side(void)

{
  lv_style_set_border_side();
  return;
}



/* ===== FUNCTION lv_group_create @ 011d7ee0 ===== */

void lv_group_create(void)

{
  lv_group_create();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_top @ 011d7ef0 ===== */

void lv_obj_set_style_pad_top(void)

{
  lv_obj_set_style_pad_top();
  return;
}



/* ===== FUNCTION lv_obj_set_style_outline_color @ 011d7f00 ===== */

void lv_obj_set_style_outline_color(void)

{
  lv_obj_set_style_outline_color();
  return;
}



/* ===== FUNCTION lv_style_set_outline_width @ 011d7f10 ===== */

void lv_style_set_outline_width(void)

{
  lv_style_set_outline_width();
  return;
}



/* ===== FUNCTION calloc @ 011d7f20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * calloc(size_t __nmemb,size_t __size)

{
  void *pvVar1;
  
  pvVar1 = calloc(__nmemb,__size);
  return pvVar1;
}



/* ===== FUNCTION lv_obj_set_style_text_color @ 011d7f30 ===== */

void lv_obj_set_style_text_color(void)

{
  lv_obj_set_style_text_color();
  return;
}



/* ===== FUNCTION lv_obj_clear_flag @ 011d7f40 ===== */

void lv_obj_clear_flag(void)

{
  lv_obj_clear_flag();
  return;
}



/* ===== FUNCTION lv_obj_add_event_cb @ 011d7f50 ===== */

void lv_obj_add_event_cb(void)

{
  lv_obj_add_event_cb();
  return;
}



/* ===== FUNCTION lv_chart_create @ 011d7f60 ===== */

void lv_chart_create(void)

{
  lv_chart_create();
  return;
}



/* ===== FUNCTION lv_style_set_bg_img_src @ 011d7f70 ===== */

void lv_style_set_bg_img_src(void)

{
  lv_style_set_bg_img_src();
  return;
}



/* ===== FUNCTION lv_tabview_get_tab_act @ 011d7f80 ===== */

void lv_tabview_get_tab_act(void)

{
  lv_tabview_get_tab_act();
  return;
}



/* ===== FUNCTION lv_img_set_src @ 011d7f90 ===== */

void lv_img_set_src(void)

{
  lv_img_set_src();
  return;
}



/* ===== FUNCTION lv_group_del @ 011d7fa0 ===== */

void lv_group_del(void)

{
  lv_group_del();
  return;
}



/* ===== FUNCTION hcplayer_multi_destroy @ 011d7fb0 ===== */

void hcplayer_multi_destroy(void)

{
  hcplayer_multi_destroy();
  return;
}



/* ===== FUNCTION hcplayer_get_video_streams_count @ 011d7fc0 ===== */

void hcplayer_get_video_streams_count(void)

{
  hcplayer_get_video_streams_count();
  return;
}



/* ===== FUNCTION lv_timer_create @ 011d7fd0 ===== */

void lv_timer_create(void)

{
  lv_timer_create();
  return;
}



/* ===== FUNCTION lv_obj_set_style_shadow_width @ 011d7fe0 ===== */

void lv_obj_set_style_shadow_width(void)

{
  lv_obj_set_style_shadow_width();
  return;
}



/* ===== FUNCTION hcplayer_init @ 011d7ff0 ===== */

void hcplayer_init(void)

{
  hcplayer_init();
  return;
}



/* ===== FUNCTION epoll_wait @ 011d8000 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_wait(int __epfd,epoll_event *__events,int __maxevents,int __timeout)

{
  int iVar1;
  
  iVar1 = epoll_wait(__epfd,__events,__maxevents,__timeout);
  return iVar1;
}



/* ===== FUNCTION zip_stat_index @ 011d8010 ===== */

void zip_stat_index(void)

{
  zip_stat_index();
  return;
}



/* ===== FUNCTION zip_strerror @ 011d8020 ===== */

void zip_strerror(void)

{
  zip_strerror();
  return;
}



/* ===== FUNCTION lv_obj_clean @ 011d8040 ===== */

void lv_obj_clean(void)

{
  lv_obj_clean();
  return;
}



/* ===== FUNCTION hcplayer_change_audio_track @ 011d8050 ===== */

void hcplayer_change_audio_track(void)

{
  hcplayer_change_audio_track();
  return;
}



/* ===== FUNCTION fgets @ 011d8060 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * fgets(char *__s,int __n,FILE *__stream)

{
  char *pcVar1;
  
  pcVar1 = fgets(__s,__n,__stream);
  return pcVar1;
}



/* ===== FUNCTION lv_indev_drv_init @ 011d8070 ===== */

void lv_indev_drv_init(void)

{
  lv_indev_drv_init();
  return;
}



/* ===== FUNCTION lv_font_get_bitmap_fmt_txt @ 011d8080 ===== */

void lv_font_get_bitmap_fmt_txt(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION memcpy @ 011d8090 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memcpy(__dest,__src,__n);
  return pvVar1;
}



/* ===== FUNCTION hcplayer_get_nth_video_stream_info @ 011d80a0 ===== */

void hcplayer_get_nth_video_stream_info(void)

{
  hcplayer_get_nth_video_stream_info();
  return;
}



/* ===== FUNCTION lv_obj_set_style_shadow_opa @ 011d80b0 ===== */

void lv_obj_set_style_shadow_opa(void)

{
  lv_obj_set_style_shadow_opa();
  return;
}



/* ===== FUNCTION lv_obj_set_scrollbar_mode @ 011d80c0 ===== */

void lv_obj_set_scrollbar_mode(void)

{
  lv_obj_set_scrollbar_mode();
  return;
}



/* ===== FUNCTION lv_tabview_get_tab_btns @ 011d80d0 ===== */

void lv_tabview_get_tab_btns(void)

{
  lv_tabview_get_tab_btns();
  return;
}



/* ===== FUNCTION write @ 011d80e0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t write(int __fd,void *__buf,size_t __n)

{
  ssize_t sVar1;
  
  sVar1 = write(__fd,__buf,__n);
  return sVar1;
}



/* ===== FUNCTION malloc @ 011d80f0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * malloc(size_t __size)

{
  void *pvVar1;
  
  pvVar1 = malloc(__size);
  return pvVar1;
}



/* ===== FUNCTION lv_btnmatrix_set_one_checked @ 011d8100 ===== */

void lv_btnmatrix_set_one_checked(void)

{
  lv_btnmatrix_set_one_checked();
  return;
}



/* ===== FUNCTION lv_obj_set_style_outline_pad @ 011d8110 ===== */

void lv_obj_set_style_outline_pad(void)

{
  lv_obj_set_style_outline_pad();
  return;
}



/* ===== FUNCTION lv_indev_set_group @ 011d8120 ===== */

void lv_indev_set_group(void)

{
  lv_indev_set_group();
  return;
}



/* ===== FUNCTION lv_timer_del @ 011d8130 ===== */

void lv_timer_del(void)

{
  lv_timer_del();
  return;
}



/* ===== FUNCTION lv_freetype_init @ 011d8140 ===== */

void lv_freetype_init(void)

{
  lv_freetype_init();
  return;
}



/* ===== FUNCTION strlen @ 011d8150 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strlen(char *__s)

{
  size_t sVar1;
  
  sVar1 = strlen(__s);
  return sVar1;
}



/* ===== FUNCTION lv_btnmatrix_set_btn_ctrl @ 011d8160 ===== */

void lv_btnmatrix_set_btn_ctrl(void)

{
  lv_btnmatrix_set_btn_ctrl();
  return;
}



/* ===== FUNCTION munmap @ 011d8170 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int munmap(void *__addr,size_t __len)

{
  int iVar1;
  
  iVar1 = munmap(__addr,__len);
  return iVar1;
}



/* ===== FUNCTION usleep @ 011d8180 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int usleep(__useconds_t __useconds)

{
  int iVar1;
  
  iVar1 = usleep(__useconds);
  return iVar1;
}



/* ===== FUNCTION hcplayer_play @ 011d8190 ===== */

void hcplayer_play(void)

{
  hcplayer_play();
  return;
}



/* ===== FUNCTION lv_ft_font_init @ 011d81a0 ===== */

void lv_ft_font_init(void)

{
  lv_ft_font_init();
  return;
}



/* ===== FUNCTION accept @ 011d81b0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int accept(int __fd,sockaddr *__addr,socklen_t *__addr_len)

{
  int iVar1;
  
  iVar1 = accept(__fd,__addr,__addr_len);
  return iVar1;
}



/* ===== FUNCTION lv_style_set_pad_row @ 011d81c0 ===== */

void lv_style_set_pad_row(void)

{
  lv_style_set_pad_row();
  return;
}



/* ===== FUNCTION lv_obj_set_style_arc_color @ 011d81d0 ===== */

void lv_obj_set_style_arc_color(void)

{
  lv_obj_set_style_arc_color();
  return;
}



/* ===== FUNCTION hcplayer_multi_play @ 011d81e0 ===== */

void hcplayer_multi_play(void)

{
  hcplayer_multi_play();
  return;
}



/* ===== FUNCTION hcplayer_multi_position @ 011d81f0 ===== */

void hcplayer_multi_position(void)

{
  hcplayer_multi_position();
  return;
}



/* ===== FUNCTION lv_anim_start @ 011d8210 ===== */

void lv_anim_start(void)

{
  lv_anim_start();
  return;
}



/* ===== FUNCTION lv_disp_get_scr_act @ 011d8220 ===== */

void lv_disp_get_scr_act(void)

{
  lv_disp_get_scr_act();
  return;
}



/* ===== FUNCTION lv_obj_set_style_shadow_color @ 011d8230 ===== */

void lv_obj_set_style_shadow_color(void)

{
  lv_obj_set_style_shadow_color();
  return;
}



/* ===== FUNCTION readdir @ 011d8240 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

dirent * readdir(DIR *__dirp)

{
  dirent *pdVar1;
  
  pdVar1 = readdir(__dirp);
  return pdVar1;
}



/* ===== FUNCTION sprintf @ 011d8250 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sprintf(char *__s,char *__format,...)

{
  int iVar1;
  
  iVar1 = sprintf(__s,__format);
  return iVar1;
}



/* ===== FUNCTION __xstat @ 011d8260 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __xstat(int __ver,char *__filename,stat *__stat_buf)

{
  int iVar1;
  
  iVar1 = __xstat(__ver,__filename,__stat_buf);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_text_opa @ 011d8270 ===== */

void lv_obj_set_style_text_opa(void)

{
  lv_obj_set_style_text_opa();
  return;
}



/* ===== FUNCTION pthread_attr_setstacksize @ 011d8280 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_setstacksize(pthread_attr_t *__attr,size_t __stacksize)

{
  int iVar1;
  
  iVar1 = pthread_attr_setstacksize(__attr,__stacksize);
  return iVar1;
}



/* ===== FUNCTION lv_bar_set_value @ 011d8290 ===== */

void lv_bar_set_value(void)

{
  lv_bar_set_value();
  return;
}



/* ===== FUNCTION snd_spectrum_stop @ 011d82a0 ===== */

void snd_spectrum_stop(void)

{
  snd_spectrum_stop();
  return;
}



/* ===== FUNCTION lv_label_set_text @ 011d82b0 ===== */

void lv_label_set_text(void)

{
  lv_label_set_text();
  return;
}



/* ===== FUNCTION lv_label_set_recolor @ 011d82c0 ===== */

void lv_label_set_recolor(void)

{
  lv_label_set_recolor();
  return;
}



/* ===== FUNCTION msgsnd @ 011d82d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgsnd(int __msqid,void *__msgp,size_t __msgsz,int __msgflg)

{
  int iVar1;
  
  iVar1 = msgsnd(__msqid,__msgp,__msgsz,__msgflg);
  return iVar1;
}



/* ===== FUNCTION glist_delete_link @ 011d82e0 ===== */

void glist_delete_link(void)

{
  glist_delete_link();
  return;
}



/* ===== FUNCTION lv_btnmatrix_get_btn_text @ 011d82f0 ===== */

void lv_btnmatrix_get_btn_text(void)

{
  lv_btnmatrix_get_btn_text();
  return;
}



/* ===== FUNCTION strcmp @ 011d8300 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

{
  int iVar1;
  
  iVar1 = strcmp(__s1,__s2);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_line_color @ 011d8320 ===== */

void lv_obj_set_style_line_color(void)

{
  lv_obj_set_style_line_color();
  return;
}



/* ===== FUNCTION strcasestr @ 011d8330 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcasestr(char *__haystack,char *__needle)

{
  char *pcVar1;
  
  pcVar1 = strcasestr(__haystack,__needle);
  return pcVar1;
}



/* ===== FUNCTION glist_sort @ 011d8340 ===== */

void glist_sort(void)

{
  glist_sort();
  return;
}



/* ===== FUNCTION time @ 011d8350 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

time_t time(time_t *__timer)

{
  time_t tVar1;
  
  tVar1 = time(__timer);
  return tVar1;
}



/* ===== FUNCTION hc_lvgl_init @ 011d8360 ===== */

void hc_lvgl_init(void)

{
  hc_lvgl_init();
  return;
}



/* ===== FUNCTION lv_obj_add_flag @ 011d8370 ===== */

void lv_obj_add_flag(void)

{
  lv_obj_add_flag();
  return;
}



/* ===== FUNCTION lv_obj_set_flex_grow @ 011d8380 ===== */

void lv_obj_set_flex_grow(void)

{
  lv_obj_set_flex_grow();
  return;
}



/* ===== FUNCTION fsync @ 011d8390 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fsync(int __fd)

{
  int iVar1;
  
  iVar1 = fsync(__fd);
  return iVar1;
}



/* ===== FUNCTION __errno_location @ 011d83b0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int * __errno_location(void)

{
  int *piVar1;
  
  piVar1 = __errno_location();
  return piVar1;
}



/* ===== FUNCTION fclose @ 011d83c0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fclose(FILE *__stream)

{
  int iVar1;
  
  iVar1 = fclose(__stream);
  return iVar1;
}



/* ===== FUNCTION strncpy @ 011d83d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strncpy(char *__dest,char *__src,size_t __n)

{
  char *pcVar1;
  
  pcVar1 = strncpy(__dest,__src,__n);
  return pcVar1;
}



/* ===== FUNCTION lv_obj_invalidate @ 011d83e0 ===== */

void lv_obj_invalidate(void)

{
  lv_obj_invalidate();
  return;
}



/* ===== FUNCTION lv_indev_get_key @ 011d83f0 ===== */

void lv_indev_get_key(void)

{
  lv_indev_get_key();
  return;
}



/* ===== FUNCTION memcmp @ 011d8400 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int memcmp(void *__s1,void *__s2,size_t __n)

{
  int iVar1;
  
  iVar1 = memcmp(__s1,__s2,__n);
  return iVar1;
}



/* ===== FUNCTION lv_style_set_radius @ 011d8410 ===== */

void lv_style_set_radius(void)

{
  lv_style_set_radius();
  return;
}



/* ===== FUNCTION glist_append @ 011d8420 ===== */

void glist_append(void)

{
  glist_append();
  return;
}



/* ===== FUNCTION _lv_ll_get_head @ 011d8430 ===== */

void _lv_ll_get_head(void)

{
  _lv_ll_get_head();
  return;
}



/* ===== FUNCTION lv_group_focus_prev @ 011d8440 ===== */

void lv_group_focus_prev(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION lv_obj_set_width @ 011d8450 ===== */

void lv_obj_set_width(void)

{
  lv_obj_set_width();
  return;
}



/* ===== FUNCTION lv_obj_remove_style @ 011d8460 ===== */

void lv_obj_remove_style(void)

{
  lv_obj_remove_style();
  return;
}



/* ===== FUNCTION hcplayer_get_nth_audio_stream_info @ 011d8470 ===== */

void hcplayer_get_nth_audio_stream_info(void)

{
  hcplayer_get_nth_audio_stream_info();
  return;
}



/* ===== FUNCTION glist_prepend @ 011d8480 ===== */

void glist_prepend(void)

{
  glist_prepend();
  return;
}



/* ===== FUNCTION lv_msgbox_close @ 011d8490 ===== */

void lv_msgbox_close(void)

{
  lv_msgbox_close();
  return;
}



/* ===== FUNCTION lv_group_get_default @ 011d84a0 ===== */

void lv_group_get_default(void)

{
  lv_group_get_default();
  return;
}



/* ===== FUNCTION lv_label_set_long_mode @ 011d84b0 ===== */

void lv_label_set_long_mode(void)

{
  lv_label_set_long_mode();
  return;
}



/* ===== FUNCTION lv_btnmatrix_clear_btn_ctrl @ 011d84d0 ===== */

void lv_btnmatrix_clear_btn_ctrl(void)

{
  lv_btnmatrix_clear_btn_ctrl();
  return;
}



/* ===== FUNCTION lv_tabview_set_act @ 011d84e0 ===== */

void lv_tabview_set_act(void)

{
  lv_tabview_set_act();
  return;
}



/* ===== FUNCTION lv_obj_clear_state @ 011d84f0 ===== */

void lv_obj_clear_state(void)

{
  lv_obj_clear_state();
  return;
}



/* ===== FUNCTION lv_group_focus_next @ 011d8500 ===== */

void lv_group_focus_next(void)

{
  do {
                    /* WARNING: Do nothing block with infinite loop */
  } while( true );
}



/* ===== FUNCTION fseek @ 011d8510 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fseek(FILE *__stream,long __off,int __whence)

{
  int iVar1;
  
  iVar1 = fseek(__stream,__off,__whence);
  return iVar1;
}



/* ===== FUNCTION lv_indev_get_act @ 011d8520 ===== */

void lv_indev_get_act(void)

{
  lv_indev_get_act();
  return;
}



/* ===== FUNCTION hcplayer_get_cur_audio_stream_info @ 011d8530 ===== */

void hcplayer_get_cur_audio_stream_info(void)

{
  hcplayer_get_cur_audio_stream_info();
  return;
}



/* ===== FUNCTION lv_fb_hotplug_support_set @ 011d8540 ===== */

void lv_fb_hotplug_support_set(void)

{
  lv_fb_hotplug_support_set();
  return;
}



/* ===== FUNCTION lv_obj_set_flex_flow @ 011d8550 ===== */

void lv_obj_set_flex_flow(void)

{
  lv_obj_set_flex_flow();
  return;
}



/* ===== FUNCTION lv_obj_set_style_bg_img_src @ 011d8560 ===== */

void lv_obj_set_style_bg_img_src(void)

{
  lv_obj_set_style_bg_img_src();
  return;
}



/* ===== FUNCTION hcplayer_change_rotate_type @ 011d8570 ===== */

void hcplayer_change_rotate_type(void)

{
  hcplayer_change_rotate_type();
  return;
}



/* ===== FUNCTION hcplayer_change_rotate_mirror_type2 @ 011d8580 ===== */

void hcplayer_change_rotate_mirror_type2(void)

{
  hcplayer_change_rotate_mirror_type2();
  return;
}



/* ===== FUNCTION hcplayer_seek @ 011d85a0 ===== */

void hcplayer_seek(void)

{
  hcplayer_seek();
  return;
}



/* ===== FUNCTION lv_obj_set_style_text_align @ 011d85b0 ===== */

void lv_obj_set_style_text_align(void)

{
  lv_obj_set_style_text_align();
  return;
}



/* ===== FUNCTION srand @ 011d85c0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void srand(uint __seed)

{
  srand(__seed);
  return;
}



/* ===== FUNCTION msgrcv @ 011d85d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t msgrcv(int __msqid,void *__msgp,size_t __msgsz,long __msgtyp,int __msgflg)

{
  ssize_t sVar1;
  
  sVar1 = msgrcv(__msqid,__msgp,__msgsz,__msgtyp,__msgflg);
  return sVar1;
}



/* ===== FUNCTION shmat @ 011d85f0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * shmat(int __shmid,void *__shmaddr,int __shmflg)

{
  void *pvVar1;
  
  pvVar1 = shmat(__shmid,__shmaddr,__shmflg);
  return pvVar1;
}



/* ===== FUNCTION __libc_start_main @ 011d8600 ===== */

void __libc_start_main(void)

{
  __libc_start_main();
  return;
}



/* ===== FUNCTION __isoc99_sscanf @ 011d8610 ===== */

void __isoc99_sscanf(void)

{
  __isoc99_sscanf();
  return;
}



/* ===== FUNCTION lv_obj_create @ 011d8620 ===== */

void lv_obj_create(void)

{
  lv_obj_create();
  return;
}



/* ===== FUNCTION pthread_mutex_unlock @ 011d8630 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_unlock(pthread_mutex_t *__mutex)

{
  int iVar1;
  
  iVar1 = pthread_mutex_unlock(__mutex);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_outline_width @ 011d8640 ===== */

void lv_obj_set_style_outline_width(void)

{
  lv_obj_set_style_outline_width();
  return;
}



/* ===== FUNCTION vsnprintf @ 011d8650 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int vsnprintf(char *__s,size_t __maxlen,char *__format,__gnuc_va_list __arg)

{
  int iVar1;
  
  iVar1 = vsnprintf(__s,__maxlen,__format,__arg);
  return iVar1;
}



/* ===== FUNCTION ftok @ 011d8660 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

key_t ftok(char *__pathname,int __proj_id)

{
  key_t kVar1;
  
  kVar1 = ftok(__pathname,__proj_id);
  return kVar1;
}



/* ===== FUNCTION lv_obj_set_style_height @ 011d8670 ===== */

void lv_obj_set_style_height(void)

{
  lv_obj_set_style_height();
  return;
}



/* ===== FUNCTION glist_last @ 011d8680 ===== */

void glist_last(void)

{
  glist_last();
  return;
}



/* ===== FUNCTION zip_file_strerror @ 011d8690 ===== */

void zip_file_strerror(void)

{
  zip_file_strerror();
  return;
}



/* ===== FUNCTION signal @ 011d86a0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__sighandler_t signal(int __sig,__sighandler_t __handler)

{
  __sighandler_t p_Var1;
  
  p_Var1 = signal(__sig,__handler);
  return p_Var1;
}



/* ===== FUNCTION lv_obj_get_x @ 011d86b0 ===== */

void lv_obj_get_x(void)

{
  lv_obj_get_x();
  return;
}



/* ===== FUNCTION lv_btn_create @ 011d86c0 ===== */

void lv_btn_create(void)

{
  lv_btn_create();
  return;
}



/* ===== FUNCTION lv_obj_get_y_aligned @ 011d86d0 ===== */

void lv_obj_get_y_aligned(void)

{
  lv_obj_get_y_aligned();
  return;
}



/* ===== FUNCTION lv_mem_free @ 011d86e0 ===== */

void lv_mem_free(void)

{
  lv_mem_free();
  return;
}



/* ===== FUNCTION lv_img_create @ 011d86f0 ===== */

void lv_img_create(void)

{
  lv_img_create();
  return;
}



/* ===== FUNCTION lv_obj_set_style_line_width @ 011d8710 ===== */

void lv_obj_set_style_line_width(void)

{
  lv_obj_set_style_line_width();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_bottom @ 011d8720 ===== */

void lv_obj_set_style_pad_bottom(void)

{
  lv_obj_set_style_pad_bottom();
  return;
}



/* ===== FUNCTION gettimeofday @ 011d8730 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int gettimeofday(timeval *__tv,__timezone_ptr_t __tz)

{
  int iVar1;
  
  iVar1 = gettimeofday(__tv,__tz);
  return iVar1;
}



/* ===== FUNCTION lv_btnmatrix_set_btn_ctrl_all @ 011d8740 ===== */

void lv_btnmatrix_set_btn_ctrl_all(void)

{
  lv_btnmatrix_set_btn_ctrl_all();
  return;
}



/* ===== FUNCTION lv_obj_get_height @ 011d8750 ===== */

void lv_obj_get_height(void)

{
  lv_obj_get_height();
  return;
}



/* ===== FUNCTION pthread_mutex_init @ 011d8770 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_init(pthread_mutex_t *__mutex,pthread_mutexattr_t *__mutexattr)

{
  int iVar1;
  
  iVar1 = pthread_mutex_init(__mutex,__mutexattr);
  return iVar1;
}



/* ===== FUNCTION lv_btnmatrix_set_map @ 011d8780 ===== */

void lv_btnmatrix_set_map(void)

{
  lv_btnmatrix_set_map();
  return;
}



/* ===== FUNCTION lv_msgbox_get_active_btn @ 011d8790 ===== */

void lv_msgbox_get_active_btn(void)

{
  lv_msgbox_get_active_btn();
  return;
}



/* ===== FUNCTION lv_obj_set_y @ 011d87b0 ===== */

void lv_obj_set_y(void)

{
  lv_obj_set_y();
  return;
}



/* ===== FUNCTION rand @ 011d87c0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int rand(void)

{
  int iVar1;
  
  iVar1 = rand();
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_border_width @ 011d87d0 ===== */

void lv_obj_set_style_border_width(void)

{
  lv_obj_set_style_border_width();
  return;
}



/* ===== FUNCTION lv_event_get_user_data @ 011d87e0 ===== */

void lv_event_get_user_data(void)

{
  lv_event_get_user_data();
  return;
}



/* ===== FUNCTION hcplayer_get_position @ 011d87f0 ===== */

void hcplayer_get_position(void)

{
  hcplayer_get_position();
  return;
}



/* ===== FUNCTION lv_msgbox_get_btns @ 011d8800 ===== */

void lv_msgbox_get_btns(void)

{
  lv_msgbox_get_btns();
  return;
}



/* ===== FUNCTION lv_obj_set_style_border_color @ 011d8820 ===== */

void lv_obj_set_style_border_color(void)

{
  lv_obj_set_style_border_color();
  return;
}



/* ===== FUNCTION hcplayer_set_speed_rate @ 011d8830 ===== */

void hcplayer_set_speed_rate(void)

{
  hcplayer_set_speed_rate();
  return;
}



/* ===== FUNCTION hudi_flash_otp_write @ 011d8840 ===== */

void hudi_flash_otp_write(void)

{
  hudi_flash_otp_write();
  return;
}



/* ===== FUNCTION lv_arc_create @ 011d8850 ===== */

void lv_arc_create(void)

{
  lv_arc_create();
  return;
}



/* ===== FUNCTION pthread_mutex_lock @ 011d8860 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_lock(pthread_mutex_t *__mutex)

{
  int iVar1;
  
  iVar1 = pthread_mutex_lock(__mutex);
  return iVar1;
}



/* ===== FUNCTION strtol @ 011d8870 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long strtol(char *__nptr,char **__endptr,int __base)

{
  long lVar1;
  
  lVar1 = strtol(__nptr,__endptr,__base);
  return lVar1;
}



/* ===== FUNCTION lv_msgbox_get_content @ 011d8890 ===== */

void lv_msgbox_get_content(void)

{
  lv_msgbox_get_content();
  return;
}



/* ===== FUNCTION lv_style_set_border_color @ 011d88a0 ===== */

void lv_style_set_border_color(void)

{
  lv_style_set_border_color();
  return;
}



/* ===== FUNCTION lv_obj_get_style_prop @ 011d88b0 ===== */

void lv_obj_get_style_prop(void)

{
  lv_obj_get_style_prop();
  return;
}



/* ===== FUNCTION hudi_flash_uid_read @ 011d88c0 ===== */

void hudi_flash_uid_read(void)

{
  hudi_flash_uid_read();
  return;
}



/* ===== FUNCTION hcplayer_change_rotate_mirror_type @ 011d88d0 ===== */

void hcplayer_change_rotate_mirror_type(void)

{
  hcplayer_change_rotate_mirror_type();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_row @ 011d88e0 ===== */

void lv_obj_set_style_pad_row(void)

{
  lv_obj_set_style_pad_row();
  return;
}



/* ===== FUNCTION msgget @ 011d8900 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgget(key_t __key,int __msgflg)

{
  int iVar1;
  
  iVar1 = msgget(__key,__msgflg);
  return iVar1;
}



/* ===== FUNCTION hcplayer_get_audio_streams_count @ 011d8910 ===== */

void hcplayer_get_audio_streams_count(void)

{
  hcplayer_get_audio_streams_count();
  return;
}



/* ===== FUNCTION lv_style_set_border_opa @ 011d8920 ===== */

void lv_style_set_border_opa(void)

{
  lv_style_set_border_opa();
  return;
}



/* ===== FUNCTION lv_obj_set_style_flex_flow @ 011d8930 ===== */

void lv_obj_set_style_flex_flow(void)

{
  lv_obj_set_style_flex_flow();
  return;
}



/* ===== FUNCTION pthread_create @ 011d8940 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_create(pthread_t *__newthread,pthread_attr_t *__attr,__start_routine *__start_routine,
                  void *__arg)

{
  int iVar1;
  
  iVar1 = pthread_create(__newthread,__attr,__start_routine,__arg);
  return iVar1;
}



/* ===== FUNCTION lv_style_set_pad_left @ 011d8950 ===== */

void lv_style_set_pad_left(void)

{
  lv_style_set_pad_left();
  return;
}



/* ===== FUNCTION lv_label_set_text_fmt @ 011d8960 ===== */

void lv_label_set_text_fmt(void)

{
  lv_label_set_text_fmt();
  return;
}



/* ===== FUNCTION glist_nth_data @ 011d8970 ===== */

void glist_nth_data(void)

{
  glist_nth_data();
  return;
}



/* ===== FUNCTION lv_timer_handler @ 011d8980 ===== */

void lv_timer_handler(void)

{
  lv_timer_handler();
  return;
}



/* ===== FUNCTION free @ 011d8990 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void free(void *__ptr)

{
  free(__ptr);
  return;
}



/* ===== FUNCTION lv_tabview_create @ 011d89a0 ===== */

void lv_tabview_create(void)

{
  lv_tabview_create();
  return;
}



/* ===== FUNCTION lv_obj_has_state @ 011d89b0 ===== */

void lv_obj_has_state(void)

{
  lv_obj_has_state();
  return;
}



/* ===== FUNCTION lv_timer_ready @ 011d89c0 ===== */

void lv_timer_ready(void)

{
  lv_timer_ready();
  return;
}



/* ===== FUNCTION lv_timer_reset @ 011d89d0 ===== */

void lv_timer_reset(void)

{
  lv_timer_reset();
  return;
}



/* ===== FUNCTION lv_btnmatrix_set_selected_btn @ 011d89e0 ===== */

void lv_btnmatrix_set_selected_btn(void)

{
  lv_btnmatrix_set_selected_btn();
  return;
}



/* ===== FUNCTION asprintf @ 011d89f0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int asprintf(char **__ptr,char *__fmt,...)

{
  int iVar1;
  
  iVar1 = asprintf(__ptr,__fmt);
  return iVar1;
}



/* ===== FUNCTION lv_label_create @ 011d8a00 ===== */

void lv_label_create(void)

{
  lv_label_create();
  return;
}



/* ===== FUNCTION lv_switch_create @ 011d8a10 ===== */

void lv_switch_create(void)

{
  lv_switch_create();
  return;
}



/* ===== FUNCTION lv_obj_set_align @ 011d8a20 ===== */

void lv_obj_set_align(void)

{
  lv_obj_set_align();
  return;
}



/* ===== FUNCTION hcplayer_get_duration @ 011d8a30 ===== */

void hcplayer_get_duration(void)

{
  hcplayer_get_duration();
  return;
}



/* ===== FUNCTION lv_list_add_text @ 011d8a40 ===== */

void lv_list_add_text(void)

{
  lv_list_add_text();
  return;
}



/* ===== FUNCTION lv_obj_get_y @ 011d8a50 ===== */

void lv_obj_get_y(void)

{
  lv_obj_get_y();
  return;
}



/* ===== FUNCTION epoll_create1 @ 011d8a70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_create1(int __flags)

{
  int iVar1;
  
  iVar1 = epoll_create1(__flags);
  return iVar1;
}



/* ===== FUNCTION snd_spectrum_run @ 011d8a80 ===== */

void snd_spectrum_run(void)

{
  snd_spectrum_run();
  return;
}



/* ===== FUNCTION strncmp @ 011d8a90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncmp(char *__s1,char *__s2,size_t __n)

{
  int iVar1;
  
  iVar1 = strncmp(__s1,__s2,__n);
  return iVar1;
}



/* ===== FUNCTION lv_bar_create @ 011d8aa0 ===== */

void lv_bar_create(void)

{
  lv_bar_create();
  return;
}



/* ===== FUNCTION lv_bar_get_value @ 011d8ab0 ===== */

void lv_bar_get_value(void)

{
  lv_bar_get_value();
  return;
}



/* ===== FUNCTION hcplayer_multi_pause @ 011d8ac0 ===== */

void hcplayer_multi_pause(void)

{
  hcplayer_multi_pause();
  return;
}



/* ===== FUNCTION lv_obj_move_to_index @ 011d8ad0 ===== */

void lv_obj_move_to_index(void)

{
  lv_obj_move_to_index();
  return;
}



/* ===== FUNCTION lv_theme_default_init @ 011d8ae0 ===== */

void lv_theme_default_init(void)

{
  lv_theme_default_init();
  return;
}



/* ===== FUNCTION lv_obj_set_x @ 011d8af0 ===== */

void lv_obj_set_x(void)

{
  lv_obj_set_x();
  return;
}



/* ===== FUNCTION hudi_flash_close @ 011d8b00 ===== */

void hudi_flash_close(void)

{
  hudi_flash_close();
  return;
}



/* ===== FUNCTION lv_timer_pause @ 011d8b20 ===== */

void lv_timer_pause(void)

{
  lv_timer_pause();
  return;
}



/* ===== FUNCTION lv_obj_scroll_by @ 011d8b30 ===== */

void lv_obj_scroll_by(void)

{
  lv_obj_scroll_by();
  return;
}



/* ===== FUNCTION lv_chart_set_update_mode @ 011d8b40 ===== */

void lv_chart_set_update_mode(void)

{
  lv_chart_set_update_mode();
  return;
}



/* ===== FUNCTION epoll_ctl @ 011d8b50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_ctl(int __epfd,int __op,int __fd,epoll_event *__event)

{
  int iVar1;
  
  iVar1 = epoll_ctl(__epfd,__op,__fd,__event);
  return iVar1;
}



/* ===== FUNCTION opendir @ 011d8b70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

DIR * opendir(char *__name)

{
  DIR *pDVar1;
  
  pDVar1 = opendir(__name);
  return pDVar1;
}



/* ===== FUNCTION lv_event_get_code @ 011d8b80 ===== */

void lv_event_get_code(void)

{
  lv_event_get_code();
  return;
}



/* ===== FUNCTION pthread_join @ 011d8b90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_join(pthread_t __th,void **__thread_return)

{
  int iVar1;
  
  iVar1 = pthread_join(__th,__thread_return);
  return iVar1;
}



/* ===== FUNCTION lv_textarea_add_text @ 011d8ba0 ===== */

void lv_textarea_add_text(void)

{
  lv_textarea_add_text();
  return;
}



/* ===== FUNCTION lv_obj_add_state @ 011d8bb0 ===== */

void lv_obj_add_state(void)

{
  lv_obj_add_state();
  return;
}



/* ===== FUNCTION lv_obj_set_grid_cell @ 011d8bc0 ===== */

void lv_obj_set_grid_cell(void)

{
  lv_obj_set_grid_cell();
  return;
}



/* ===== FUNCTION lv_timer_set_period @ 011d8bd0 ===== */

void lv_timer_set_period(void)

{
  lv_timer_set_period();
  return;
}



/* ===== FUNCTION lv_obj_get_parent @ 011d8be0 ===== */

void lv_obj_get_parent(void)

{
  lv_obj_get_parent();
  return;
}



/* ===== FUNCTION lv_arc_set_bg_angles @ 011d8bf0 ===== */

void lv_arc_set_bg_angles(void)

{
  lv_arc_set_bg_angles();
  return;
}



/* ===== FUNCTION closedir @ 011d8c00 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int closedir(DIR *__dirp)

{
  int iVar1;
  
  iVar1 = closedir(__dirp);
  return iVar1;
}



/* ===== FUNCTION lv_chart_set_type @ 011d8c10 ===== */

void lv_chart_set_type(void)

{
  lv_chart_set_type();
  return;
}



/* ===== FUNCTION sem_wait @ 011d8c20 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_wait(sem_t *__sem)

{
  int iVar1;
  
  iVar1 = sem_wait(__sem);
  return iVar1;
}



/* ===== FUNCTION lv_style_set_bg_color @ 011d8c30 ===== */

void lv_style_set_bg_color(void)

{
  lv_style_set_bg_color();
  return;
}



/* ===== FUNCTION lv_event_send @ 011d8c40 ===== */

void lv_event_send(void)

{
  lv_event_send();
  return;
}



/* ===== FUNCTION lv_bar_set_range @ 011d8c50 ===== */

void lv_bar_set_range(void)

{
  lv_bar_set_range();
  return;
}



/* ===== FUNCTION exit @ 011d8c60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void exit(int __status)

{
                    /* WARNING: Subroutine does not return */
  exit(__status);
}



/* ===== FUNCTION lv_list_add_btn @ 011d8c70 ===== */

void lv_list_add_btn(void)

{
  lv_list_add_btn();
  return;
}



/* ===== FUNCTION lv_draw_img_dsc_init @ 011d8c80 ===== */

void lv_draw_img_dsc_init(void)

{
  lv_draw_img_dsc_init();
  return;
}



/* ===== FUNCTION printf @ 011d8c90 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

{
  int iVar1;
  
  iVar1 = printf(__format);
  return iVar1;
}



/* ===== FUNCTION lv_obj_del @ 011d8ca0 ===== */

void lv_obj_del(void)

{
  lv_obj_del();
  return;
}



/* ===== FUNCTION lv_arc_set_rotation @ 011d8cb0 ===== */

void lv_arc_set_rotation(void)

{
  lv_arc_set_rotation();
  return;
}



/* ===== FUNCTION strerror @ 011d8cc0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strerror(int __errnum)

{
  char *pcVar1;
  
  pcVar1 = strerror(__errnum);
  return pcVar1;
}



/* ===== FUNCTION zip_name_locate @ 011d8cd0 ===== */

void zip_name_locate(void)

{
  zip_name_locate();
  return;
}



/* ===== FUNCTION lv_img_get_zoom @ 011d8ce0 ===== */

void lv_img_get_zoom(void)

{
  lv_img_get_zoom();
  return;
}



/* ===== FUNCTION lv_snprintf @ 011d8cf0 ===== */

void lv_snprintf(void)

{
  lv_snprintf();
  return;
}



/* ===== FUNCTION zip_fopen_index_encrypted @ 011d8d00 ===== */

void zip_fopen_index_encrypted(void)

{
  zip_fopen_index_encrypted();
  return;
}



/* ===== FUNCTION lv_chart_set_value_by_id @ 011d8d10 ===== */

void lv_chart_set_value_by_id(void)

{
  lv_chart_set_value_by_id();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_left @ 011d8d20 ===== */

void lv_obj_set_style_pad_left(void)

{
  lv_obj_set_style_pad_left();
  return;
}



/* ===== FUNCTION stpcpy @ 011d8d30 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * stpcpy(char *__dest,char *__src)

{
  char *pcVar1;
  
  pcVar1 = stpcpy(__dest,__src);
  return pcVar1;
}



/* ===== FUNCTION glist_nth @ 011d8d50 ===== */

void glist_nth(void)

{
  glist_nth();
  return;
}



/* ===== FUNCTION feof @ 011d8d60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int feof(FILE *__stream)

{
  int iVar1;
  
  iVar1 = feof(__stream);
  return iVar1;
}



/* ===== FUNCTION strtok @ 011d8d70 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strtok(char *__s,char *__delim)

{
  char *pcVar1;
  
  pcVar1 = strtok(__s,__delim);
  return pcVar1;
}



/* ===== FUNCTION puts @ 011d8d80 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int puts(char *__s)

{
  int iVar1;
  
  iVar1 = puts(__s);
  return iVar1;
}



/* ===== FUNCTION glist_free_full @ 011d8d90 ===== */

void glist_free_full(void)

{
  glist_free_full();
  return;
}



/* ===== FUNCTION custom_tick_get @ 011d8da0 ===== */

void custom_tick_get(void)

{
  custom_tick_get();
  return;
}



/* ===== FUNCTION lv_obj_set_pos @ 011d8db0 ===== */

void lv_obj_set_pos(void)

{
  lv_obj_set_pos();
  return;
}



/* ===== FUNCTION lv_palette_lighten @ 011d8dc0 ===== */

void lv_palette_lighten(void)

{
  lv_palette_lighten();
  return;
}



/* ===== FUNCTION perror @ 011d8dd0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void perror(char *__s)

{
  perror(__s);
  return;
}



/* ===== FUNCTION lv_slider_create @ 011d8de0 ===== */

void lv_slider_create(void)

{
  lv_slider_create();
  return;
}



/* ===== FUNCTION lv_disp_get_default @ 011d8df0 ===== */

void lv_disp_get_default(void)

{
  lv_disp_get_default();
  return;
}



/* ===== FUNCTION lv_obj_set_style_bg_opa @ 011d8e10 ===== */

void lv_obj_set_style_bg_opa(void)

{
  lv_obj_set_style_bg_opa();
  return;
}



/* ===== FUNCTION lv_obj_is_valid @ 011d8e20 ===== */

void lv_obj_is_valid(void)

{
  lv_obj_is_valid();
  return;
}



/* ===== FUNCTION lv_obj_get_child_cnt @ 011d8e30 ===== */

void lv_obj_get_child_cnt(void)

{
  lv_obj_get_child_cnt();
  return;
}



/* ===== FUNCTION _IO_putc @ 011d8e40 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _IO_putc(int __c,_IO_FILE *__fp)

{
  int iVar1;
  
  iVar1 = _IO_putc(__c,__fp);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_text_font @ 011d8e60 ===== */

void lv_obj_set_style_text_font(void)

{
  lv_obj_set_style_text_font();
  return;
}



/* ===== FUNCTION lv_timer_set_repeat_count @ 011d8e70 ===== */

void lv_timer_set_repeat_count(void)

{
  lv_timer_set_repeat_count();
  return;
}



/* ===== FUNCTION hcfota_url @ 011d8e80 ===== */

void hcfota_url(void)

{
  hcfota_url();
  return;
}



/* ===== FUNCTION lv_obj_set_style_img_opa @ 011d8ea0 ===== */

void lv_obj_set_style_img_opa(void)

{
  lv_obj_set_style_img_opa();
  return;
}



/* ===== FUNCTION lv_obj_set_size @ 011d8eb0 ===== */

void lv_obj_set_size(void)

{
  lv_obj_set_size();
  return;
}



/* ===== FUNCTION lv_anim_init @ 011d8ec0 ===== */

void lv_anim_init(void)

{
  lv_anim_init();
  return;
}



/* ===== FUNCTION lv_obj_align_to @ 011d8ed0 ===== */

void lv_obj_align_to(void)

{
  lv_obj_align_to();
  return;
}



/* ===== FUNCTION lv_obj_remove_event_cb @ 011d8ee0 ===== */

void lv_obj_remove_event_cb(void)

{
  lv_obj_remove_event_cb();
  return;
}



/* ===== FUNCTION fbdev_set_rotate @ 011d8ef0 ===== */

void fbdev_set_rotate(void)

{
  fbdev_set_rotate();
  return;
}



/* ===== FUNCTION lv_group_remove_all_objs @ 011d8f00 ===== */

void lv_group_remove_all_objs(void)

{
  lv_group_remove_all_objs();
  return;
}



/* ===== FUNCTION hcplayer_get_cur_video_stream_info @ 011d8f20 ===== */

void hcplayer_get_cur_video_stream_info(void)

{
  hcplayer_get_cur_video_stream_info();
  return;
}



/* ===== FUNCTION lv_img_decoder_get_info @ 011d8f30 ===== */

void lv_img_decoder_get_info(void)

{
  lv_img_decoder_get_info();
  return;
}



/* ===== FUNCTION lv_btnmatrix_create @ 011d8f40 ===== */

void lv_btnmatrix_create(void)

{
  lv_btnmatrix_create();
  return;
}



/* ===== FUNCTION memset @ 011d8f50 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
  void *pvVar1;
  
  pvVar1 = memset(__s,__c,__n);
  return pvVar1;
}



/* ===== FUNCTION putchar @ 011d8f60 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int putchar(int __c)

{
  int iVar1;
  
  iVar1 = putchar(__c);
  return iVar1;
}



/* ===== FUNCTION lv_disp_get_ver_res @ 011d8f80 ===== */

void lv_disp_get_ver_res(void)

{
  lv_disp_get_ver_res();
  return;
}



/* ===== FUNCTION lv_style_set_border_width @ 011d8f90 ===== */

void lv_style_set_border_width(void)

{
  lv_style_set_border_width();
  return;
}



/* ===== FUNCTION zip_fread @ 011d8fa0 ===== */

void zip_fread(void)

{
  zip_fread();
  return;
}



/* ===== FUNCTION lv_obj_set_style_outline_opa @ 011d8fb0 ===== */

void lv_obj_set_style_outline_opa(void)

{
  lv_obj_set_style_outline_opa();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_column @ 011d8fc0 ===== */

void lv_obj_set_style_pad_column(void)

{
  lv_obj_set_style_pad_column();
  return;
}



/* ===== FUNCTION lv_indev_drv_register @ 011d8fd0 ===== */

void lv_indev_drv_register(void)

{
  lv_indev_drv_register();
  return;
}



/* ===== FUNCTION strftime @ 011d8fe0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strftime(char *__s,size_t __maxsize,char *__format,tm *__tp)

{
  size_t sVar1;
  
  sVar1 = strftime(__s,__maxsize,__format,__tp);
  return sVar1;
}



/* ===== FUNCTION lv_obj_align @ 011d8ff0 ===== */

void lv_obj_align(void)

{
  lv_obj_align();
  return;
}



/* ===== FUNCTION lv_style_set_pad_column @ 011d9000 ===== */

void lv_style_set_pad_column(void)

{
  lv_style_set_pad_column();
  return;
}



/* ===== FUNCTION lv_disp_get_layer_top @ 011d9010 ===== */

void lv_disp_get_layer_top(void)

{
  lv_disp_get_layer_top();
  return;
}



/* ===== FUNCTION lv_obj_set_layout @ 011d9020 ===== */

void lv_obj_set_layout(void)

{
  lv_obj_set_layout();
  return;
}



/* ===== FUNCTION sem_init @ 011d9030 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_init(sem_t *__sem,int __pshared,uint __value)

{
  int iVar1;
  
  iVar1 = sem_init(__sem,__pshared,__value);
  return iVar1;
}



/* ===== FUNCTION lv_event_get_current_target @ 011d9040 ===== */

void lv_event_get_current_target(void)

{
  lv_event_get_current_target();
  return;
}



/* ===== FUNCTION lv_event_get_param @ 011d9050 ===== */

void lv_event_get_param(void)

{
  lv_event_get_param();
  return;
}



/* ===== FUNCTION lv_obj_set_height @ 011d9060 ===== */

void lv_obj_set_height(void)

{
  lv_obj_set_height();
  return;
}



/* ===== FUNCTION listen @ 011d9070 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int listen(int __fd,int __n)

{
  int iVar1;
  
  iVar1 = listen(__fd,__n);
  return iVar1;
}



/* ===== FUNCTION hudi_flash_open @ 011d9080 ===== */

void hudi_flash_open(void)

{
  hudi_flash_open();
  return;
}



/* ===== FUNCTION snprintf @ 011d9090 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int snprintf(char *__s,size_t __maxlen,char *__format,...)

{
  int iVar1;
  
  iVar1 = snprintf(__s,__maxlen,__format);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_style_line_opa @ 011d90a0 ===== */

void lv_obj_set_style_line_opa(void)

{
  lv_obj_set_style_line_opa();
  return;
}



/* ===== FUNCTION __strdup @ 011d90b0 ===== */

void __strdup(void)

{
  __strdup();
  return;
}



/* ===== FUNCTION lv_obj_set_style_opa @ 011d90c0 ===== */

void lv_obj_set_style_opa(void)

{
  lv_obj_set_style_opa();
  return;
}



/* ===== FUNCTION lv_obj_set_style_bg_color @ 011d90d0 ===== */

void lv_obj_set_style_bg_color(void)

{
  lv_obj_set_style_bg_color();
  return;
}



/* ===== FUNCTION close @ 011d90e0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
  int iVar1;
  
  iVar1 = close(__fd);
  return iVar1;
}



/* ===== FUNCTION lv_chart_set_next_value @ 011d9100 ===== */

void lv_chart_set_next_value(void)

{
  lv_chart_set_next_value();
  return;
}



/* ===== FUNCTION lv_palette_darken @ 011d9110 ===== */

void lv_palette_darken(void)

{
  lv_palette_darken();
  return;
}



/* ===== FUNCTION read @ 011d9120 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
  ssize_t sVar1;
  
  sVar1 = read(__fd,__buf,__nbytes);
  return sVar1;
}



/* ===== FUNCTION lv_obj_set_style_radius @ 011d9130 ===== */

void lv_obj_set_style_radius(void)

{
  lv_obj_set_style_radius();
  return;
}



/* ===== FUNCTION lv_obj_get_width @ 011d9140 ===== */

void lv_obj_get_width(void)

{
  lv_obj_get_width();
  return;
}



/* ===== FUNCTION shmget @ 011d9150 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmget(key_t __key,size_t __size,int __shmflg)

{
  int iVar1;
  
  iVar1 = shmget(__key,__size,__shmflg);
  return iVar1;
}



/* ===== FUNCTION lv_group_get_focused @ 011d9160 ===== */

void lv_group_get_focused(void)

{
  lv_group_get_focused();
  return;
}



/* ===== FUNCTION lv_obj_set_style_border_opa @ 011d9170 ===== */

void lv_obj_set_style_border_opa(void)

{
  lv_obj_set_style_border_opa();
  return;
}



/* ===== FUNCTION hudi_flash_otp_read @ 011d9180 ===== */

void hudi_flash_otp_read(void)

{
  hudi_flash_otp_read();
  return;
}



/* ===== FUNCTION hcplayer_create @ 011d9190 ===== */

void hcplayer_create(void)

{
  hcplayer_create();
  return;
}



/* ===== FUNCTION lv_chart_add_series @ 011d91a0 ===== */

void lv_chart_add_series(void)

{
  lv_chart_add_series();
  return;
}



/* ===== FUNCTION lv_obj_get_child @ 011d91b0 ===== */

void lv_obj_get_child(void)

{
  lv_obj_get_child();
  return;
}



/* ===== FUNCTION lv_obj_set_style_border_side @ 011d91c0 ===== */

void lv_obj_set_style_border_side(void)

{
  lv_obj_set_style_border_side();
  return;
}



/* ===== FUNCTION lv_img_get_angle @ 011d91d0 ===== */

void lv_img_get_angle(void)

{
  lv_img_get_angle();
  return;
}



/* ===== FUNCTION poll @ 011d91e0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int poll(pollfd *__fds,nfds_t __nfds,int __timeout)

{
  int iVar1;
  
  iVar1 = poll(__fds,__nfds,__timeout);
  return iVar1;
}



/* ===== FUNCTION lv_obj_set_grid_dsc_array @ 011d9200 ===== */

void lv_obj_set_grid_dsc_array(void)

{
  lv_obj_set_grid_dsc_array();
  return;
}



/* ===== FUNCTION pthread_attr_init @ 011d9210 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_init(pthread_attr_t *__attr)

{
  int iVar1;
  
  iVar1 = pthread_attr_init(__attr);
  return iVar1;
}



/* ===== FUNCTION lv_style_set_pad_right @ 011d9230 ===== */

void lv_style_set_pad_right(void)

{
  lv_style_set_pad_right();
  return;
}



/* ===== FUNCTION lv_btnmatrix_get_selected_btn @ 011d9240 ===== */

void lv_btnmatrix_get_selected_btn(void)

{
  lv_btnmatrix_get_selected_btn();
  return;
}



/* ===== FUNCTION lv_event_get_target @ 011d9250 ===== */

void lv_event_get_target(void)

{
  lv_event_get_target();
  return;
}



/* ===== FUNCTION lv_group_focus_freeze @ 011d9260 ===== */

void lv_group_focus_freeze(void)

{
  lv_group_focus_freeze();
  return;
}



/* ===== FUNCTION unlink @ 011d9270 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int unlink(char *__name)

{
  int iVar1;
  
  iVar1 = unlink(__name);
  return iVar1;
}



/* ===== FUNCTION lv_arc_get_value @ 011d9280 ===== */

void lv_arc_get_value(void)

{
  lv_arc_get_value();
  return;
}



/* ===== FUNCTION ioctl @ 011d9290 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ioctl(int __fd,ulong __request,...)

{
  int iVar1;
  
  iVar1 = ioctl(__fd,__request);
  return iVar1;
}



/* ===== FUNCTION hcplayer_get_media_info @ 011d92a0 ===== */

void hcplayer_get_media_info(void)

{
  hcplayer_get_media_info();
  return;
}



/* ===== FUNCTION lv_obj_set_style_pad_right @ 011d92c0 ===== */

void lv_obj_set_style_pad_right(void)

{
  lv_obj_set_style_pad_right();
  return;
}



/* ===== FUNCTION hcplayer_multi_duration @ 011d92d0 ===== */

void hcplayer_multi_duration(void)

{
  hcplayer_multi_duration();
  return;
}



/* ===== FUNCTION hcplayer_get_filesize @ 011d92e0 ===== */

void hcplayer_get_filesize(void)

{
  hcplayer_get_filesize();
  return;
}



/* ===== FUNCTION glist_index @ 011d92f0 ===== */

void glist_index(void)

{
  glist_index();
  return;
}



/* ===== FUNCTION hcplayer_get_subtitle_streams_count @ 011d9300 ===== */

void hcplayer_get_subtitle_streams_count(void)

{
  hcplayer_get_subtitle_streams_count();
  return;
}



/* ===== FUNCTION system @ 011d9310 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

{
  int iVar1;
  
  iVar1 = system(__command);
  return iVar1;
}



/* ===== FUNCTION hcplayer_resume @ 011d9320 ===== */

void hcplayer_resume(void)

{
  hcplayer_resume();
  return;
}



/* ===== FUNCTION lv_event_get_draw_part_dsc @ 011d9330 ===== */

void lv_event_get_draw_part_dsc(void)

{
  lv_event_get_draw_part_dsc();
  return;
}



/* ===== FUNCTION pthread_attr_setdetachstate @ 011d9340 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_setdetachstate(pthread_attr_t *__attr,int __detachstate)

{
  int iVar1;
  
  iVar1 = pthread_attr_setdetachstate(__attr,__detachstate);
  return iVar1;
}



/* ===== FUNCTION strcpy @ 011d9350 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcpy(char *__dest,char *__src)

{
  char *pcVar1;
  
  pcVar1 = strcpy(__dest,__src);
  return pcVar1;
}



/* ===== FUNCTION zip_fopen_index @ 011d9360 ===== */

void zip_fopen_index(void)

{
  zip_fopen_index();
  return;
}



/* ===== FUNCTION mmap @ 011d9370 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
  void *pvVar1;
  
  pvVar1 = mmap(__addr,__len,__prot,__flags,__fd,__offset);
  return pvVar1;
}



/* ===== FUNCTION localtime @ 011d9380 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

tm * localtime(time_t *__timer)

{
  tm *ptVar1;
  
  ptVar1 = localtime(__timer);
  return ptVar1;
}



/* ===== FUNCTION lv_style_init @ 011d9390 ===== */

void lv_style_init(void)

{
  lv_style_init();
  return;
}



/* ===== FUNCTION lv_group_add_obj @ 011d93a0 ===== */

void lv_group_add_obj(void)

{
  lv_group_add_obj();
  return;
}



/* ===== FUNCTION lv_style_set_pad_bottom @ 011d93b0 ===== */

void lv_style_set_pad_bottom(void)

{
  lv_style_set_pad_bottom();
  return;
}



/* ===== FUNCTION lv_timer_resume @ 011d93c0 ===== */

void lv_timer_resume(void)

{
  lv_timer_resume();
  return;
}



/* ===== FUNCTION sem_post @ 011d93d0 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_post(sem_t *__sem)

{
  int iVar1;
  
  iVar1 = sem_post(__sem);
  return iVar1;
}



/* ===== FUNCTION hcplayer_get_cur_subtitle_stream_info @ 011d93e0 ===== */

void hcplayer_get_cur_subtitle_stream_info(void)

{
  hcplayer_get_cur_subtitle_stream_info();
  return;
}



/* ===== FUNCTION hcplayer_multi_seek @ 011d93f0 ===== */

void hcplayer_multi_seek(void)

{
  hcplayer_multi_seek();
  return;
}



/* ===== FUNCTION lv_obj_set_style_bg_grad_color @ 011d9400 ===== */

void lv_obj_set_style_bg_grad_color(void)

{
  lv_obj_set_style_bg_grad_color();
  return;
}



/* ===== FUNCTION lv_chart_set_div_line_count @ 011d9410 ===== */

void lv_chart_set_div_line_count(void)

{
  lv_chart_set_div_line_count();
  return;
}



/* ===== FUNCTION lv_disp_set_theme @ 011d9420 ===== */

void lv_disp_set_theme(void)

{
  lv_disp_set_theme();
  return;
}



/* ===== FUNCTION lv_group_focus_obj @ 011d9430 ===== */

void lv_group_focus_obj(void)

{
  lv_group_focus_obj();
  return;
}



/* ===== FUNCTION hcplayer_pause @ 011d9440 ===== */

void hcplayer_pause(void)

{
  hcplayer_pause();
  return;
}



/* ===== FUNCTION msgctl @ 011d9450 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgctl(int __msqid,int __cmd,msqid_ds *__buf)

{
  int iVar1;
  
  iVar1 = msgctl(__msqid,__cmd,__buf);
  return iVar1;
}



/* ===== FUNCTION lv_msgbox_create @ 011d9460 ===== */

void lv_msgbox_create(void)

{
  lv_msgbox_create();
  return;
}



/* ===== FUNCTION realloc @ 011d9470 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * realloc(void *__ptr,size_t __size)

{
  void *pvVar1;
  
  pvVar1 = realloc(__ptr,__size);
  return pvVar1;
}



/* ===== FUNCTION bind @ 011d9480 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int bind(int __fd,sockaddr *__addr,socklen_t __len)

{
  int iVar1;
  
  iVar1 = bind(__fd,__addr,__len);
  return iVar1;
}



/* ===== FUNCTION ftell @ 011d9490 ===== */

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long ftell(FILE *__stream)

{
  long lVar1;
  
  lVar1 = ftell(__stream);
  return lVar1;
}



/* ===== FUNCTION lv_obj_set_style_transform_zoom @ 0121f000 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_transform_zoom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION open @ 0121f004 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int open(char *__file,int __oflag,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_mutex_destroy @ 0121f008 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_destroy(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_add_style @ 0121f00c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_add_style(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_mem_alloc @ 0121f010 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_mem_alloc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strcat @ 0121f014 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcat(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fclose @ 0121f018 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fclose(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_index @ 0121f01c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fread @ 0121f020 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t fread(void *__ptr,size_t __size,size_t __n,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_remove_obj @ 0121f024 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_remove_obj(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_get_hor_res @ 0121f028 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_get_hor_res(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_palette_main @ 0121f02c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_palette_main(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_stop2 @ 0121f030 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_stop2(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_list_create @ 0121f034 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_list_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __ctype_tolower_loc @ 0121f038 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__int32_t ** __ctype_tolower_loc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_flex_align @ 0121f03c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_flex_align(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION socket @ 0121f040 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int socket(int __domain,int __type,int __protocol)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION access @ 0121f044 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int access(char *__name,int __type)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_draw_img @ 0121f048 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_draw_img(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_deinit @ 0121f04c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_deinit(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_clear_btn_ctrl_all @ 0121f050 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_clear_btn_ctrl_all(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_tabview_add_tab @ 0121f054 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_tabview_add_tab(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_video_track @ 0121f058 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_video_track(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION mkdir @ 0121f05c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int mkdir(char *__path,__mode_t __mode)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_checkbox_create @ 0121f060 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_checkbox_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_has_flag @ 0121f064 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_has_flag(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_nth_prev @ 0121f068 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_nth_prev(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_set_display_rect @ 0121f06c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_set_display_rect(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strstr @ 0121f070 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strstr(char *__haystack,char *__needle)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _lv_ll_get_next @ 0121f074 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _lv_ll_get_next(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fopen @ 0121f078 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fopen(char *__filename,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_scr_load_anim @ 0121f07c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_scr_load_anim(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_top @ 0121f080 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_top(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_attr_destroy @ 0121f084 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_destroy(pthread_attr_t *__attr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_set_default @ 0121f088 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_set_default(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strncasecmp @ 0121f08c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncasecmp(char *__s1,char *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_get_text @ 0121f090 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_get_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_bg_grad_dir @ 0121f094 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_bg_grad_dir(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_text_color @ 0121f098 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_text_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_border_side @ 0121f09c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_border_side(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_create @ 0121f0a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_create @ 0121f0a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_top @ 0121f0a8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_top(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_outline_color @ 0121f0ac ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_outline_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_outline_width @ 0121f0b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_outline_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION calloc @ 0121f0b4 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * calloc(size_t __nmemb,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_text_color @ 0121f0b8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_text_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_clear_flag @ 0121f0bc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_clear_flag(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_add_event_cb @ 0121f0c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_add_event_cb(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_create @ 0121f0c4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_bg_img_src @ 0121f0c8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_bg_img_src(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_tabview_get_tab_act @ 0121f0cc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_tabview_get_tab_act(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_set_src @ 0121f0d0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_set_src(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_del @ 0121f0d4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_del(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_destroy @ 0121f0d8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_destroy(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_video_streams_count @ 0121f0dc ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_video_streams_count(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_create @ 0121f0e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_shadow_width @ 0121f0e4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_shadow_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_init @ 0121f0e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION epoll_wait @ 0121f0ec ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_wait(int __epfd,epoll_event *__events,int __maxevents,int __timeout)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_stat_index @ 0121f0f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_stat_index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_strerror @ 0121f0f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_strerror(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_x_aligned @ 0121f0f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_x_aligned(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_clean @ 0121f0fc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_clean(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_audio_track @ 0121f100 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_audio_track(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fgets @ 0121f104 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * fgets(char *__s,int __n,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_indev_drv_init @ 0121f108 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_indev_drv_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION memcpy @ 0121f10c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_nth_video_stream_info @ 0121f110 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_nth_video_stream_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_shadow_opa @ 0121f114 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_shadow_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_scrollbar_mode @ 0121f118 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_scrollbar_mode(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_tabview_get_tab_btns @ 0121f11c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_tabview_get_tab_btns(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION write @ 0121f120 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t write(int __fd,void *__buf,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION malloc @ 0121f124 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * malloc(size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_set_one_checked @ 0121f128 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_set_one_checked(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_outline_pad @ 0121f12c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_outline_pad(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_indev_set_group @ 0121f130 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_indev_set_group(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_del @ 0121f134 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_del(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_freetype_init @ 0121f138 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_freetype_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strlen @ 0121f13c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strlen(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_set_btn_ctrl @ 0121f140 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_set_btn_ctrl(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION munmap @ 0121f144 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int munmap(void *__addr,size_t __len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION usleep @ 0121f148 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int usleep(__useconds_t __useconds)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_play @ 0121f14c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_play(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_ft_font_init @ 0121f150 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_ft_font_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION accept @ 0121f154 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int accept(int __fd,sockaddr *__addr,socklen_t *__addr_len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_row @ 0121f158 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_row(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_arc_color @ 0121f15c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_arc_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_play @ 0121f160 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_play(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_position @ 0121f164 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_position(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_text_font @ 0121f168 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_text_font(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_anim_start @ 0121f16c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_anim_start(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_get_scr_act @ 0121f170 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_get_scr_act(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_shadow_color @ 0121f174 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_shadow_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION readdir @ 0121f178 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

dirent * readdir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION sprintf @ 0121f17c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sprintf(char *__s,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __xstat @ 0121f180 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int __xstat(int __ver,char *__filename,stat *__stat_buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_text_opa @ 0121f184 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_text_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_attr_setstacksize @ 0121f188 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_setstacksize(pthread_attr_t *__attr,size_t __stacksize)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_bar_set_value @ 0121f18c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_bar_set_value(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION snd_spectrum_stop @ 0121f190 ===== */

/* WARNING: Control flow encountered bad instruction data */

void snd_spectrum_stop(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_set_text @ 0121f194 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_set_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_set_recolor @ 0121f198 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_set_recolor(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION msgsnd @ 0121f19c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgsnd(int __msqid,void *__msgp,size_t __msgsz,int __msgflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_delete_link @ 0121f1a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_delete_link(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_get_btn_text @ 0121f1a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_get_btn_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strcmp @ 0121f1a8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_roller_set_selected @ 0121f1ac ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_roller_set_selected(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_line_color @ 0121f1b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_line_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strcasestr @ 0121f1b4 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcasestr(char *__haystack,char *__needle)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_sort @ 0121f1b8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_sort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION time @ 0121f1bc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

time_t time(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hc_lvgl_init @ 0121f1c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hc_lvgl_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_add_flag @ 0121f1c4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_add_flag(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_flex_grow @ 0121f1c8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_flex_grow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fsync @ 0121f1cc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fsync(int __fd)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pclose @ 0121f1d0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pclose(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __errno_location @ 0121f1d4 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int * __errno_location(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fclose @ 0121f1d8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fclose(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strncpy @ 0121f1dc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strncpy(char *__dest,char *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_invalidate @ 0121f1e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_invalidate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_indev_get_key @ 0121f1e4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_indev_get_key(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION memcmp @ 0121f1e8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int memcmp(void *__s1,void *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_radius @ 0121f1ec ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_radius(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_append @ 0121f1f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_append(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _lv_ll_get_head @ 0121f1f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void _lv_ll_get_head(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_width @ 0121f1f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_remove_style @ 0121f1fc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_remove_style(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_nth_audio_stream_info @ 0121f200 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_nth_audio_stream_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_prepend @ 0121f204 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_prepend(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_msgbox_close @ 0121f208 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_msgbox_close(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_get_default @ 0121f20c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_get_default(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_set_long_mode @ 0121f210 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_set_long_mode(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_checkbox_set_text @ 0121f214 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_checkbox_set_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_clear_btn_ctrl @ 0121f218 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_clear_btn_ctrl(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_tabview_set_act @ 0121f21c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_tabview_set_act(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_clear_state @ 0121f220 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_clear_state(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fseek @ 0121f224 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fseek(FILE *__stream,long __off,int __whence)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_indev_get_act @ 0121f228 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_indev_get_act(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_cur_audio_stream_info @ 0121f22c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_cur_audio_stream_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_fb_hotplug_support_set @ 0121f230 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_fb_hotplug_support_set(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_flex_flow @ 0121f234 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_flex_flow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_bg_img_src @ 0121f238 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_bg_img_src(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_rotate_type @ 0121f23c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_rotate_type(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_rotate_mirror_type2 @ 0121f240 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_rotate_mirror_type2(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_subtitle_track @ 0121f244 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_subtitle_track(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_seek @ 0121f248 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_seek(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_text_align @ 0121f24c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_text_align(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION srand @ 0121f250 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void srand(uint __seed)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION msgrcv @ 0121f254 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t msgrcv(int __msqid,void *__msgp,size_t __msgsz,long __msgtyp,int __msgflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_set_angle @ 0121f258 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_set_angle(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION shmat @ 0121f25c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * shmat(int __shmid,void *__shmaddr,int __shmflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __libc_start_main @ 0121f260 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __libc_start_main(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __isoc99_sscanf @ 0121f264 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __isoc99_sscanf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_create @ 0121f268 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_mutex_unlock @ 0121f26c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_unlock(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_outline_width @ 0121f270 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_outline_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION vsnprintf @ 0121f274 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int vsnprintf(char *__s,size_t __maxlen,char *__format,__gnuc_va_list __arg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION ftok @ 0121f278 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

key_t ftok(char *__pathname,int __proj_id)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_height @ 0121f27c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_height(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_last @ 0121f280 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_last(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_file_strerror @ 0121f284 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_file_strerror(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION signal @ 0121f288 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__sighandler_t signal(int __sig,__sighandler_t __handler)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_x @ 0121f28c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_x(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btn_create @ 0121f290 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btn_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_y_aligned @ 0121f294 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_y_aligned(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_mem_free @ 0121f298 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_mem_free(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_create @ 0121f29c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_remove @ 0121f2a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_remove(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_line_width @ 0121f2a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_line_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_bottom @ 0121f2a8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_bottom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION gettimeofday @ 0121f2ac ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int gettimeofday(timeval *__tv,__timezone_ptr_t __tz)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_set_btn_ctrl_all @ 0121f2b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_set_btn_ctrl_all(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_height @ 0121f2b4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_height(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _IO_getc @ 0121f2b8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _IO_getc(_IO_FILE *__fp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_mutex_init @ 0121f2bc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_init(pthread_mutex_t *__mutex,pthread_mutexattr_t *__mutexattr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_set_map @ 0121f2c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_set_map(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_msgbox_get_active_btn @ 0121f2c4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_msgbox_get_active_btn(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __ctype_b_loc @ 0121f2c8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ushort ** __ctype_b_loc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_y @ 0121f2cc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_y(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION rand @ 0121f2d0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int rand(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_border_width @ 0121f2d4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_border_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_user_data @ 0121f2d8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_user_data(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_position @ 0121f2dc ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_position(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_msgbox_get_btns @ 0121f2e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_msgbox_get_btns(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_dropdown_set_selected @ 0121f2e4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_dropdown_set_selected(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_border_color @ 0121f2e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_border_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_set_speed_rate @ 0121f2ec ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_set_speed_rate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hudi_flash_otp_write @ 0121f2f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hudi_flash_otp_write(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_arc_create @ 0121f2f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_arc_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_mutex_lock @ 0121f2f8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_lock(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strtol @ 0121f2fc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long strtol(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcfota_memory @ 0121f300 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcfota_memory(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_msgbox_get_content @ 0121f304 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_msgbox_get_content(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_border_color @ 0121f308 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_border_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_style_prop @ 0121f30c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_style_prop(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hudi_flash_uid_read @ 0121f310 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hudi_flash_uid_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_change_rotate_mirror_type @ 0121f318 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_change_rotate_mirror_type(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_row @ 0121f31c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_row(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_set_zoom @ 0121f320 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_set_zoom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION msgget @ 0121f324 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgget(key_t __key,int __msgflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_audio_streams_count @ 0121f328 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_audio_streams_count(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_border_opa @ 0121f32c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_border_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_flex_flow @ 0121f330 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_flex_flow(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_create @ 0121f334 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_create(pthread_t *__newthread,pthread_attr_t *__attr,__start_routine *__start_routine,
                  void *__arg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_left @ 0121f338 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_left(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_set_text_fmt @ 0121f33c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_set_text_fmt(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_nth_data @ 0121f340 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_nth_data(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_handler @ 0121f344 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_handler(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION free @ 0121f348 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void free(void *__ptr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_tabview_create @ 0121f34c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_tabview_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_has_state @ 0121f350 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_has_state(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_ready @ 0121f354 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_ready(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_reset @ 0121f358 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_reset(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_set_selected_btn @ 0121f35c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_set_selected_btn(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION asprintf @ 0121f360 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int asprintf(char **__ptr,char *__fmt,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_label_create @ 0121f364 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_label_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_switch_create @ 0121f368 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_switch_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_align @ 0121f36c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_align(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_duration @ 0121f370 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_duration(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_list_add_text @ 0121f374 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_list_add_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_y @ 0121f378 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_y(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_arc_set_value @ 0121f37c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_arc_set_value(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION epoll_create1 @ 0121f380 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_create1(int __flags)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION snd_spectrum_run @ 0121f384 ===== */

/* WARNING: Control flow encountered bad instruction data */

void snd_spectrum_run(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strncmp @ 0121f388 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncmp(char *__s1,char *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_bar_create @ 0121f38c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_bar_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_bar_get_value @ 0121f390 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_bar_get_value(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_pause @ 0121f394 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_pause(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_move_to_index @ 0121f398 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_move_to_index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_theme_default_init @ 0121f39c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_theme_default_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_x @ 0121f3a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_x(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hudi_flash_close @ 0121f3a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hudi_flash_close(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strcasecmp @ 0121f3a8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcasecmp(char *__s1,char *__s2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_pause @ 0121f3ac ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_pause(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_scroll_by @ 0121f3b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_scroll_by(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_set_update_mode @ 0121f3b4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_set_update_mode(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION epoll_ctl @ 0121f3b8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int epoll_ctl(int __epfd,int __op,int __fd,epoll_event *__event)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fgetc @ 0121f3bc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fgetc(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION opendir @ 0121f3c0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

DIR * opendir(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_code @ 0121f3c4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_code(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_join @ 0121f3c8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_join(pthread_t __th,void **__thread_return)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_textarea_add_text @ 0121f3cc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_textarea_add_text(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_add_state @ 0121f3d0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_add_state(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_grid_cell @ 0121f3d4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_grid_cell(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_set_period @ 0121f3d8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_set_period(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_parent @ 0121f3dc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_parent(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_arc_set_bg_angles @ 0121f3e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_arc_set_bg_angles(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION closedir @ 0121f3e4 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int closedir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_set_type @ 0121f3e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_set_type(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION sem_wait @ 0121f3ec ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_wait(sem_t *__sem)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_bg_color @ 0121f3f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_bg_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_send @ 0121f3f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_send(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_bar_set_range @ 0121f3f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_bar_set_range(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION exit @ 0121f3fc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void exit(int __status)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_list_add_btn @ 0121f400 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_list_add_btn(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_draw_img_dsc_init @ 0121f404 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_draw_img_dsc_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION printf @ 0121f408 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int printf(char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_del @ 0121f40c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_del(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_arc_set_rotation @ 0121f410 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_arc_set_rotation(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strerror @ 0121f414 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strerror(int __errnum)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_name_locate @ 0121f418 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_name_locate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_get_zoom @ 0121f41c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_get_zoom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_snprintf @ 0121f420 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_snprintf(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fopen_index_encrypted @ 0121f424 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fopen_index_encrypted(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_set_value_by_id @ 0121f428 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_set_value_by_id(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_left @ 0121f42c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_left(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION stpcpy @ 0121f430 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * stpcpy(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_close @ 0121f434 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_close(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_nth @ 0121f438 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_nth(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION feof @ 0121f43c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int feof(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strtok @ 0121f440 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strtok(char *__s,char *__delim)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION puts @ 0121f444 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int puts(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_free_full @ 0121f448 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_free_full(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION custom_tick_get @ 0121f44c ===== */

/* WARNING: Control flow encountered bad instruction data */

void custom_tick_get(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_pos @ 0121f450 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_pos(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_palette_lighten @ 0121f454 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_palette_lighten(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION perror @ 0121f458 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void perror(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_slider_create @ 0121f45c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_slider_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_get_default @ 0121f460 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_get_default(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fopen_encrypted @ 0121f464 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fopen_encrypted(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_bg_opa @ 0121f468 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_bg_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_is_valid @ 0121f46c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_is_valid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_child_cnt @ 0121f470 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_child_cnt(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION _IO_putc @ 0121f474 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int _IO_putc(int __c,_IO_FILE *__fp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION popen @ 0121f478 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * popen(char *__command,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_text_font @ 0121f47c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_text_font(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_set_repeat_count @ 0121f480 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_set_repeat_count(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcfota_url @ 0121f484 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcfota_url(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fopen @ 0121f488 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fopen(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_img_opa @ 0121f48c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_img_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_size @ 0121f490 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_size(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_anim_init @ 0121f494 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_anim_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_align_to @ 0121f498 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_align_to(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_remove_event_cb @ 0121f49c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_remove_event_cb(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION fbdev_set_rotate @ 0121f4a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void fbdev_set_rotate(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_remove_all_objs @ 0121f4a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_remove_all_objs(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_stat @ 0121f4a8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_stat(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_cur_video_stream_info @ 0121f4ac ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_cur_video_stream_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_decoder_get_info @ 0121f4b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_decoder_get_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_create @ 0121f4b4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION memset @ 0121f4b8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION putchar @ 0121f4bc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int putchar(int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_open @ 0121f4c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_open(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_get_ver_res @ 0121f4c4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_get_ver_res(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_border_width @ 0121f4c8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_border_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fread @ 0121f4cc ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fread(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_outline_opa @ 0121f4d0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_outline_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_column @ 0121f4d4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_column(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_indev_drv_register @ 0121f4d8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_indev_drv_register(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strftime @ 0121f4dc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strftime(char *__s,size_t __maxsize,char *__format,tm *__tp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_align @ 0121f4e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_align(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_column @ 0121f4e4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_column(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_get_layer_top @ 0121f4e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_get_layer_top(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_layout @ 0121f4ec ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_layout(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION sem_init @ 0121f4f0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_init(sem_t *__sem,int __pshared,uint __value)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_current_target @ 0121f4f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_current_target(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_param @ 0121f4f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_param(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_height @ 0121f4fc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_height(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION listen @ 0121f500 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int listen(int __fd,int __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hudi_flash_open @ 0121f504 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hudi_flash_open(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION snprintf @ 0121f508 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int snprintf(char *__s,size_t __maxlen,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_line_opa @ 0121f50c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_line_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __strdup @ 0121f510 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __strdup(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_opa @ 0121f514 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_bg_color @ 0121f518 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_bg_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION close @ 0121f51c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_get_num_entries @ 0121f520 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_get_num_entries(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_set_next_value @ 0121f524 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_set_next_value(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_palette_darken @ 0121f528 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_palette_darken(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION read @ 0121f52c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_radius @ 0121f530 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_radius(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_width @ 0121f534 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_width(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION shmget @ 0121f538 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int shmget(key_t __key,size_t __size,int __shmflg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_get_focused @ 0121f53c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_get_focused(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_border_opa @ 0121f540 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_border_opa(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hudi_flash_otp_read @ 0121f544 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hudi_flash_otp_read(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_create @ 0121f54c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_add_series @ 0121f550 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_add_series(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_get_child @ 0121f554 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_get_child(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_border_side @ 0121f558 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_border_side(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_img_get_angle @ 0121f55c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_img_get_angle(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION poll @ 0121f560 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int poll(pollfd *__fds,nfds_t __nfds,int __timeout)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION memmove @ 0121f564 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memmove(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_grid_dsc_array @ 0121f568 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_grid_dsc_array(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_attr_init @ 0121f56c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_init(pthread_attr_t *__attr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION ctime @ 0121f570 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * ctime(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_right @ 0121f574 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_right(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_btnmatrix_get_selected_btn @ 0121f578 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_btnmatrix_get_selected_btn(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_target @ 0121f57c ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_target(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_focus_freeze @ 0121f580 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_focus_freeze(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION unlink @ 0121f584 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int unlink(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_arc_get_value @ 0121f588 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_arc_get_value(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION ioctl @ 0121f58c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ioctl(int __fd,ulong __request,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_media_info @ 0121f590 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_media_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_text_align @ 0121f594 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_text_align(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_pad_right @ 0121f598 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_pad_right(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_duration @ 0121f59c ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_duration(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_filesize @ 0121f5a0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_filesize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION glist_index @ 0121f5a4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void glist_index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_subtitle_streams_count @ 0121f5a8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_subtitle_streams_count(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION system @ 0121f5ac ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int system(char *__command)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_resume @ 0121f5b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_resume(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_event_get_draw_part_dsc @ 0121f5b4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_event_get_draw_part_dsc(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION pthread_attr_setdetachstate @ 0121f5b8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_attr_setdetachstate(pthread_attr_t *__attr,int __detachstate)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION strcpy @ 0121f5bc ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcpy(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION zip_fopen_index @ 0121f5c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void zip_fopen_index(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION mmap @ 0121f5c4 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION localtime @ 0121f5c8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

tm * localtime(time_t *__timer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_init @ 0121f5d0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_init(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_add_obj @ 0121f5d4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_add_obj(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_style_set_pad_bottom @ 0121f5d8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_style_set_pad_bottom(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_timer_resume @ 0121f5dc ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_timer_resume(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION sem_post @ 0121f5e0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sem_post(sem_t *__sem)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_get_cur_subtitle_stream_info @ 0121f5e4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_get_cur_subtitle_stream_info(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_multi_seek @ 0121f5e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_multi_seek(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_obj_set_style_bg_grad_color @ 0121f5ec ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_obj_set_style_bg_grad_color(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_chart_set_div_line_count @ 0121f5f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_chart_set_div_line_count(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_disp_set_theme @ 0121f5f4 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_disp_set_theme(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_group_focus_obj @ 0121f5f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_group_focus_obj(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION hcplayer_pause @ 0121f5fc ===== */

/* WARNING: Control flow encountered bad instruction data */

void hcplayer_pause(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION msgctl @ 0121f600 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int msgctl(int __msqid,int __cmd,msqid_ds *__buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION lv_msgbox_create @ 0121f604 ===== */

/* WARNING: Control flow encountered bad instruction data */

void lv_msgbox_create(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION realloc @ 0121f608 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * realloc(void *__ptr,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION bind @ 0121f60c ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int bind(int __fd,sockaddr *__addr,socklen_t __len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION ftell @ 0121f610 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long ftell(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}



/* ===== FUNCTION __gmon_start__ @ 0121f614 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __gmon_start__(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


