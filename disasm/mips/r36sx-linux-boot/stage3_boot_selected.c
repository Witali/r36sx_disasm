
/* ===== FORCED FUNCTION FUN_89da926c @ 89da926c ===== */

undefined4 FUN_89da926c(undefined4 param_1,undefined *param_2,undefined *param_3)

{
  int iVar1;
  int iVar2;
  undefined **ppuVar3;
  undefined **ppuVar4;
  undefined *puVar5;
  undefined1 auStack_100 [128];
  undefined1 auStack_80 [88];

  if (DAT_89ea5290 == 0) {
    ppuVar3 = &PTR_s_vmlinux_uImage_89ea51d0;
    do {
      ppuVar4 = ppuVar3;
      if (*ppuVar4 == (undefined *)0x0) {
        return 0xffffffff;
      }
      iVar1 = FUN_89da035c(*ppuVar4,param_3);
      ppuVar3 = ppuVar4 + 2;
    } while (iVar1 != 0);
    param_3 = ppuVar4[1];
  }
  if (param_3 != (undefined *)0x0) {
    ppuVar3 = &PTR_LAB_89ea5208;
    for (iVar1 = 0; iVar1 < DAT_89ea5214; iVar1 = iVar1 + 1) {
      thunk_FUN_89da6ab8(auStack_100,0,0x80);
      if (param_2 == (undefined *)0x0) {
        puVar5 = param_3;
        FUN_89db0224(auStack_100,0x80,s__media__s__s_89e7d128,ppuVar3[4],param_3);
      }
      else {
        puVar5 = param_2;
        FUN_89db0224(auStack_100,0x80,s__media__s__s__s_89e7d118,ppuVar3[4],param_2,param_3);
      }
      iVar2 = FUN_89de0894(auStack_100,auStack_80);
      ppuVar3 = ppuVar3 + 1;
      if (iVar2 == 0) {
        FUN_89db0224(param_1,0x40,&DAT_89ea12e4,auStack_100,puVar5);
        return 0;
      }
    }
  }
  return 0xffffffff;
}



/* ===== FORCED FUNCTION FUN_89da9474 @ 89da9474 ===== */

int FUN_89da9474(undefined4 param_1,undefined4 param_2)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  undefined1 auStack_128 [128];
  undefined1 auStack_a8 [88];
  undefined1 auStack_50 [8];
  char *local_48;
  undefined4 local_44;
  undefined4 local_40;
  int local_3c;
  uint local_38 [2];
  int local_30;

  iVar3 = FUN_89dce274(param_2);
  if (-1 < iVar3) {
    FUN_89db0224(param_1,0x40,s__dev_mtdblock_d_89e7d138,iVar3);
    return iVar3;
  }
  local_38[0] = 0;
  iVar3 = FUN_89ddc3c0(s__hcrtos_external_partitions_89e7d148);
  if (iVar3 < 0) {
    iVar3 = FUN_89ddc3c0(s__hcrtos_external_files_89e7d164);
    if (iVar3 < 0) {
      return -1;
    }
    FUN_89dc1cdc(&PTR_LAB_89ea5208);
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  FUN_89ddc2e8(iVar3,s_part_num_89e7d17c,0,local_38);
  uVar7 = 1;
  do {
    if (local_38[0] < uVar7) {
      return -1;
    }
    local_3c = 0;
    local_40 = 0;
    thunk_FUN_89da6ab8(auStack_128,0,0x80);
    FUN_89db0224(auStack_128,0x80,s_part_d_label_89e7d188,uVar7);
    FUN_89ddc438(iVar3,auStack_128,0,&local_3c);
    if (local_3c != 0) {
      local_30 = local_3c;
      uVar4 = FUN_89da059c(local_3c);
      uVar5 = FUN_89da059c(param_2);
      if (uVar4 < uVar5) {
        uVar4 = uVar5;
      }
      iVar6 = FUN_89db0564(local_30,param_2,uVar4);
      if (iVar6 == 0) {
        if (bVar1) {
          FUN_89ddc438(iVar3,s_path_prefix_89e7d198,0,&local_40);
          thunk_FUN_89da6ab8(auStack_128,0,0x80);
          FUN_89db0224(auStack_128,0x80,s_part_d_filename_89e7d1a4,uVar7);
          FUN_89ddc438(iVar3,auStack_128,0,&local_3c);
          if (local_3c == 0) goto LAB_89da975c;
          iVar3 = FUN_89da926c(param_1,local_40);
        }
        else {
          FUN_89db0224(param_1,0x40,s__dev_mmcblk0p_d_89e7d1b4,uVar7);
          iVar3 = FUN_89de0894(param_1,auStack_a8);
        }
        if (iVar3 == 0) {
          return 0;
        }
        FUN_89dc1cd0(s_hc15_mmc_device_89e7d1c4);
        FUN_89dc1cd0(s_hcmmc_device_89e7d1d4);
        if (bVar1) {
          FUN_89dc1cd0(&DAT_89e7d1e4);
        }
        local_44 = 2000;
        bVar2 = false;
        FUN_89dbf264(auStack_50);
        break;
      }
    }
LAB_89da975c:
    uVar7 = uVar7 + 1;
  } while( true );
LAB_89da96ac:
  iVar3 = FUN_89da9950(1);
  if (iVar3 == 0) {
    if (!bVar2) goto LAB_89da96cc;
  }
  else {
    local_44 = 2000;
    FUN_89dbf264(auStack_50);
LAB_89da96cc:
    iVar3 = FUN_89dbf2f8(auStack_50,&local_44);
    bVar2 = false;
    if (iVar3 != 0) {
      FUN_89dafc90(s_show_popup_89e7d1e8);
      FUN_89da98ec(0);
      local_48 = s__hcrtos_backlight_89e7dda8 + 8;
      bVar2 = true;
      FUN_89dac0d4(1,&local_48);
    }
  }
  if (bVar1) {
    iVar3 = FUN_89da926c(param_1,local_40,local_3c);
  }
  else {
    FUN_89db0224(param_1,0x40,s__dev_mmcblk0p_d_89e7d1b4,uVar7);
    iVar3 = FUN_89de0894(param_1,auStack_a8);
  }
  if (iVar3 == 0) {
    return 0;
  }
  FUN_89dddc0c(1);
  goto LAB_89da96ac;
}



/* ===== FORCED FUNCTION FUN_89daa7ac @ 89daa7ac ===== */

void FUN_89daa7ac(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  FUN_89dad8b0(param_1,param_2,param_3 + -1,param_4 + 4,0x74f,&DAT_89ec9dc4,1);
  return;
}



/* ===== FORCED FUNCTION FUN_89daaa28 @ 89daaa28 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_89daaa28(int param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  longlong lVar8;
  undefined1 auStack_58 [12];
  uint local_4c;

  uVar7 = *(undefined4 *)(param_2 + 4);
  if (param_1 < 2) {
    FUN_89dafc90(s_ERROR_arguments__Usage__<_dev_mt_89e7db08);
    return 0xffffffff;
  }
  iVar1 = FUN_89ddfe90(uVar7,0);
  if (iVar1 < 0) {
    FUN_89dafc90(s_ERROR__open__s_89e7da40,uVar7);
    return 0xffffffff;
  }
  lVar8 = FUN_89ddfbd0(iVar1);
  if (lVar8 == 0) {
    iVar3 = FUN_89de0594(iVar1,auStack_58,0x40);
    if (iVar3 == 0x40) {
      iVar3 = (local_4c >> 0x18 | local_4c << 0x18 | local_4c >> 8 & 0xff00 |
              (local_4c & 0xff00) << 8) + 0x40;
      if (_DAT_89eae384 == 0) {
        _DAT_89eae384 = FUN_89ddeed4(iVar3);
      }
      else {
        _DAT_89eae384 = FUN_89ddef48(_DAT_89eae384,iVar3);
      }
      if (_DAT_89eae384 == 0) {
        FUN_89dafc90(s_ERROR__malloc_for_image_with_siz_89e7db34,iVar3);
        goto LAB_89daab68;
      }
      FUN_89dafc90(s_default_image_load_address___0x__89e7db5c);
      lVar8 = FUN_89ddfbd0(iVar1);
      if (lVar8 != 0) goto LAB_89daaaac;
      iVar4 = FUN_89de0594(iVar1,_DAT_89eae384,iVar3);
      if (iVar3 == iVar4) {
        FUN_89de1330(iVar1);
        return 0;
      }
    }
    puVar2 = (undefined4 *)FUN_89ddf6b4();
    uVar6 = *puVar2;
    pcVar5 = s_ERROR__read_from__s_failed___d_89e7da78;
  }
  else {
LAB_89daaaac:
    puVar2 = (undefined4 *)FUN_89ddf6b4();
    uVar6 = *puVar2;
    uVar7 = 0;
    pcVar5 = s_ERROR__lseek_to_offset__ld_faile_89e7da50;
  }
  FUN_89dafc90(pcVar5,uVar7,uVar6);
LAB_89daab68:
  FUN_89de1330(iVar1);
  return 0xffffffff;
}
