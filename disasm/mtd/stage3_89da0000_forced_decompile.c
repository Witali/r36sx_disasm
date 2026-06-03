
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



/* ===== FORCED FUNCTION FUN_89da97c0 @ 89da97c0 ===== */

int FUN_89da97c0(undefined4 param_1,int param_2)

{
  int iVar1;
  char **ppcVar2;
  undefined1 auStack_78 [64];
  char *local_38;
  char *local_34;
  char *local_30;
  int local_2c;
  char *local_28;
  char *local_24;
  char *local_20;
  char *local_1c;
  char *local_18;
  int local_14;
  
  iVar1 = FUN_89da9474(auStack_78,param_1);
  if (iVar1 < 0) {
    return iVar1;
  }
  iVar1 = FUN_89de2054(auStack_78,s__soc_spi_1882e000_spi_flash_0_pa_89e7d548 + 0x28,
                       s_romfs_89e7d1f8,1,0);
  if (iVar1 < 0) {
    return iVar1;
  }
  if (param_2 == 0) {
    local_20 = s_showlogo_89e7d200;
    local_1c = s__eromfs_popup_bmp_89e7d20c;
    iVar1 = FUN_89dabb60(2,&local_20);
    if (iVar1 != 0) {
      local_28 = s_showlogo_89e7d200;
      local_24 = s__eromfs_popup_bmp_gz_89e7d220;
      iVar1 = FUN_89dabb60(2,&local_28);
      if (iVar1 != 0) {
        local_38 = s_showlogo_89e7d200;
        local_34 = s__eromfs_popup_hc_89e7d238;
        ppcVar2 = &local_38;
        goto LAB_89da98c8;
      }
    }
  }
  else {
    local_18 = s_showlogo_89e7d200;
    local_14 = param_2;
    iVar1 = FUN_89dabb60(2,&local_18);
    ppcVar2 = &local_30;
    if (iVar1 != 0) {
      local_30 = s_showlogo_89e7d200;
      local_2c = param_2;
LAB_89da98c8:
      iVar1 = FUN_89dabb04(2,ppcVar2);
      if (iVar1 == 0) {
        FUN_89dab9e4();
      }
      goto LAB_89da9844;
    }
  }
  iVar1 = 0;
LAB_89da9844:
  FUN_89de225c(s__soc_spi_1882e000_spi_flash_0_pa_89e7d548 + 0x28,0);
  return iVar1;
}



/* ===== FORCED FUNCTION FUN_89da98ec @ 89da98ec ===== */

void FUN_89da98ec(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_89da97c0(s_eromfs_89e7d24c,param_1);
  if ((iVar1 != 0) && (iVar1 = FUN_89da97c0(s_eromfs2_89e7d254,param_1), iVar1 != 0)) {
    FUN_89da97c0(s_eromfs3_89e7d25c,param_1);
    return;
  }
  return;
}



/* ===== FORCED FUNCTION FUN_89da9950 @ 89da9950 ===== */

int FUN_89da9950(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  char *local_18 [2];
  
  iVar3 = 0;
  while (iVar1 = FUN_89dac4c8(param_1), iVar1 != 0) {
    if (iVar3 == 0) {
      iVar3 = 1;
      uVar2 = FUN_89dac534();
      FUN_89da98ec(uVar2);
      local_18[0] = s__hcrtos_backlight_89e7dda8 + 8;
      FUN_89dac0d4(1,local_18);
    }
  }
  return iVar3;
}



/* ===== FORCED FUNCTION FUN_89da99cc @ 89da99cc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_89da99cc(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  char *pcVar7;
  uint uVar8;
  char *local_1a0;
  undefined *local_19c;
  undefined4 *local_198;
  undefined *local_194;
  int local_188;
  byte local_184;
  undefined4 local_100 [16];
  undefined1 auStack_c0 [36];
  char *local_9c;
  undefined4 *local_98;
  undefined4 *local_94;
  char *local_90;
  char *local_8c;
  undefined4 *local_88;
  undefined *local_84;
  undefined4 *local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  char *local_5c;
  char *local_58;
  char *local_54;
  undefined4 local_50;
  char *local_4c;
  char **local_48;
  char *local_44;
  undefined4 *local_40;
  char *local_3c;
  undefined4 *local_38;
  char *local_34;
  char *local_30;
  char *local_2c;
  char *local_28 [3];
  
  thunk_FUN_89da6748(auStack_c0,&PTR_s_hc15_mmc_device_89e75140,0x24);
  if ((DAT_b8818a70 == -0x5b) || (DAT_b8818a70 == 'Z')) {
    DAT_b8818a70 = 0x5a;
  }
  else {
    DAT_b8818a70 = 0xa5;
    DAT_b8818a71 = 0;
  }
  FUN_89dc203c();
  iVar2 = FUN_89ddc4e8(s__hcrtos_gpio_out_def_89e7d264);
  if (-1 < iVar2) {
    local_9c = (char *)0x0;
    iVar3 = FUN_89ddc3e4(iVar2,s_gpio_group_89e7d27c,&local_9c);
    if (iVar3 == 0) {
      local_9c = (char *)0x0;
    }
    local_9c = (char *)((int)local_9c >> 3);
    if (local_9c != (char *)0x0) {
      for (iVar3 = 0; iVar3 < (int)local_9c; iVar3 = iVar3 + 1) {
        FUN_89ddc2e8(iVar2,s_gpio_group_89e7d27c,iVar3 * 2,local_100);
        FUN_89ddc2e8(iVar2,s_gpio_group_89e7d27c,iVar3 * 2 + 1,&local_1a0);
        pcVar7 = local_1a0;
        FUN_89dca3d4(local_100[0],1);
        FUN_89dca32c(local_100[0],pcVar7 == (char *)0x0);
      }
    }
  }
  iVar2 = FUN_89dc1984(&DAT_89e7d1e4,9,auStack_c0);
  if (iVar2 != 0) {
    pcVar7 = s_module_init2__all___9__excludes__89e7d288;
    uVar6 = 0x4aa;
    goto LAB_89daa64c;
  }
  FUN_89dafc90(s_time_to_boot___ld_89e7d34c,_DAT_89eae340);
  FUN_89daffa8(&DAT_89e7d368,s_GMT_8_89e7d360,1);
  FUN_89db21a4();
  local_28[0] = s_boot_lcd_89e7d36c;
  _DAT_89eae33c = 0;
  FUN_89dac2d4(1,local_28);
  FUN_89da9950(0);
  local_1a0 = (char *)0x0;
  iVar2 = FUN_89ddfe90(s__dev_standby_89e7d378,2);
  if (-1 < iVar2) {
    FUN_89ddfa58(iVar2,0x40042709,&local_1a0);
    FUN_89de1330(iVar2);
  }
  uVar8 = (uint)local_1a0 >> 4 & 0xf;
  if (uVar8 < 2) {
    if (((_DAT_89eae33c == 0) && (iVar2 = FUN_89da9474(&local_1a0,s_eromfs_89e7d24c), -1 < iVar2))
       && (iVar2 = FUN_89de2054(&local_1a0,&DAT_89e7d388,s_romfs_89e7d1f8,1,0), -1 < iVar2)) {
      local_5c = s_showlogo_89e7d200;
      local_58 = s__etc_logo_hc_89e7d390;
      FUN_89dabb04(2,&local_5c);
      FUN_89dab9e4();
    }
LAB_89da9b80:
    if (uVar8 == 0) goto LAB_89daa664;
  }
  else {
    if (uVar8 != 3) goto LAB_89da9b80;
LAB_89daa664:
    if (_DAT_89eae33c == 0) {
      iVar2 = FUN_89da9474(&local_1a0,s_showlogo_89e7d200 + 4);
      if (-1 < iVar2) {
        local_54 = s_showlogo_89e7d200;
        local_50 = FUN_89dac774();
        iVar2 = FUN_89dabb04(2,&local_54);
        if (iVar2 == 0) {
          FUN_89dac5d0();
          local_34 = s__hcrtos_backlight_89e7dda8 + 8;
          FUN_89dac0d4(1,&local_34);
          FUN_89dab9e4();
        }
        else {
          local_4c = s_showlogo_89e7d200;
          local_48 = &local_1a0;
          FUN_89dabb60(2,&local_4c);
        }
      }
    }
  }
  local_2c = s__hcrtos_backlight_89e7dda8 + 8;
  FUN_89dac0d4(1,&local_2c);
  iVar2 = FUN_89ddeed4(0x10000);
  local_6c = 0;
  local_68 = 0;
  local_64 = 0;
  local_60 = 0;
  local_7c = 0;
  local_78 = 0;
  local_74 = 0;
  local_70 = 0;
  FUN_89dafc90(s_booting_hclinux_dualcore_firmwar_89e7d3a0);
  FUN_89db02c8(&local_7c,s_0x_08x_89e7d3c4,iVar2);
  DAT_b880006b = '\x01';
  _DAT_b8800004 = iVar2;
  iVar3 = FUN_89da9474(local_100,&DAT_89e7d3cc);
  if (-1 < iVar3) {
    local_9c = s_mtdloadraw_89e7d3d0;
    local_98 = &local_7c;
    local_94 = local_100;
    local_90 = s_0x10000_89e7d3dc;
    FUN_89daa7e8(4,&local_9c);
    thunk_FUN_89da6ab8(&local_1a0,0,0xa0);
    iVar3 = FUN_89dc1d00(&local_1a0);
    if (iVar3 == 0) {
      iVar3 = FUN_89ddd288(iVar2,s__hcrtos_de_engine_89e7d3e4);
      if ((-1 < iVar3) && (-1 < local_188)) {
        FUN_89da93d4(iVar2,iVar3,s_tvtype_89e7d3f8);
      }
      iVar3 = FUN_89ddd288(iVar2,s__hcrtos_i2so_89e7d400);
      if ((-1 < iVar3) && (local_184 < 0x65)) {
        FUN_89da93d4(iVar2,iVar3,s_volume_89e7d410);
      }
    }
    iVar3 = FUN_89dad178(iVar2);
    if (iVar3 != 0) {
      local_44 = (char *)0x0;
      local_8c = (char *)0x0;
      local_1a0 = (char *)0x0;
      iVar3 = FUN_89ddc3c0(s__hcrtos_de_engine_VPInitInfo_rgb_89e7d418);
      iVar4 = FUN_89ddd288(iVar2,s__hcrtos_de_engine_VPInitInfo_rgb_89e7d418);
      if ((iVar3 == 0) || (iVar4 == 0)) {
LAB_89daa6f0:
        FUN_89dafc90(s_warning__screen_w___0_or_screen__89e7d4ec);
      }
      else {
        FUN_89ddc2e8(iVar3,s_h_active_len_89e7d44c,0,&local_44);
        FUN_89ddc2e8(iVar3,s_v_active_len_89e7d45c,0,&local_8c);
        if ((local_44 == (char *)0x0) || (local_8c == (char *)0x0)) goto LAB_89daa6f0;
        FUN_89da93d4(iVar2,iVar4,s_h_active_len_89e7d44c);
        FUN_89da93d4(iVar2,iVar4,s_v_active_len_89e7d45c,local_8c);
        iVar5 = FUN_89ddc2e8(iVar3,s_output_clock_89e7d46c,0,&local_1a0);
        if (iVar5 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_output_clock_89e7d46c,local_1a0);
          iVar5 = FUN_89ddc2e8(iVar3,s_h_total_len_89e7d47c,0,&local_1a0);
          if (iVar5 == 0) {
            FUN_89da93d4(iVar2,iVar4,s_h_total_len_89e7d47c,local_1a0);
            iVar5 = FUN_89ddc2e8(iVar3,s_v_total_len_89e7d488,0,&local_1a0);
            if (iVar5 == 0) {
              FUN_89da93d4(iVar2,iVar4,s_v_total_len_89e7d488,local_1a0);
              iVar5 = FUN_89ddc2e8(iVar3,s_h_front_len_89e7d494,0,&local_1a0);
              if (iVar5 == 0) {
                FUN_89da93d4(iVar2,iVar4,s_h_front_len_89e7d494,local_1a0);
                iVar5 = FUN_89ddc2e8(iVar3,s_h_sync_len_89e7d4a0,0,&local_1a0);
                if (iVar5 == 0) {
                  FUN_89da93d4(iVar2,iVar4,s_h_sync_len_89e7d4a0,local_1a0);
                  iVar5 = FUN_89ddc2e8(iVar3,s_h_back_len_89e7d4ac,0,&local_1a0);
                  if (iVar5 == 0) {
                    FUN_89da93d4(iVar2,iVar4,s_h_back_len_89e7d4ac,local_1a0);
                    iVar5 = FUN_89ddc2e8(iVar3,s_v_front_len_89e7d4b8,0,&local_1a0);
                    if (iVar5 == 0) {
                      FUN_89da93d4(iVar2,iVar4,s_v_front_len_89e7d4b8,local_1a0);
                      iVar5 = FUN_89ddc2e8(iVar3,s_v_sync_len_89e7d4c4,0,&local_1a0);
                      if (iVar5 == 0) {
                        FUN_89da93d4(iVar2,iVar4,s_v_sync_len_89e7d4c4,local_1a0);
                        iVar5 = FUN_89ddc2e8(iVar3,s_v_back_len_89e7d4d0,0,&local_1a0);
                        if (iVar5 == 0) {
                          FUN_89da93d4(iVar2,iVar4,s_v_back_len_89e7d4d0,local_1a0);
                          iVar5 = FUN_89ddc2e8(iVar3,s_dtb_h_sync_level_89e7e164 + 4,0,&local_1a0);
                          if (iVar5 == 0) {
                            FUN_89da93d4(iVar2,iVar4,s_dtb_h_sync_level_89e7e164 + 4,local_1a0);
                            iVar5 = FUN_89ddc2e8(iVar3,s_dtb_v_sync_level_89e7e178 + 4,0,&local_1a0)
                            ;
                            if (iVar5 == 0) {
                              FUN_89da93d4(iVar2,iVar4,s_dtb_v_sync_level_89e7e178 + 4,local_1a0);
                              iVar3 = FUN_89ddc2e8(iVar3,s_active_polarity_89e7d4dc,0,&local_1a0);
                              if (iVar3 == 0) {
                                FUN_89da93d4(iVar2,iVar4,s_active_polarity_89e7d4dc,local_1a0);
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
      iVar3 = FUN_89ddc3c0(s__soc_spi_1882e000_spi_flash_0_pa_89e7d514);
      iVar4 = FUN_89ddd288(iVar2,s__soc_spi_1882e000_spi_flash_0_pa_89e7d514);
      if (((iVar3 != 0) && (iVar4 != 0)) &&
         (iVar5 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,0,&local_1a0), iVar5 == 0)) {
        FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,0,local_1a0);
        iVar3 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,1,&local_1a0);
        if (iVar3 == 0) {
          FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,1,local_1a0);
        }
      }
      iVar3 = FUN_89ddc3c0(s__soc_spi_1882e000_spi_flash_0_pa_89e7d548);
      iVar4 = FUN_89ddd288(iVar2,s__soc_spi_1882e000_spi_flash_0_pa_89e7d548);
      if (((iVar3 != 0) && (iVar4 != 0)) &&
         (iVar5 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,0,&local_1a0), iVar5 == 0)) {
        FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,0,local_1a0);
        iVar3 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,1,&local_1a0);
        if (iVar3 == 0) {
          FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,1,local_1a0);
        }
      }
      iVar3 = FUN_89ddc3c0(s__soc_spi_1882e000_spi_flash_0_pa_89e7d578);
      iVar4 = FUN_89ddd288(iVar2,s__soc_spi_1882e000_spi_flash_0_pa_89e7d578);
      if (((iVar3 != 0) && (iVar4 != 0)) &&
         (iVar5 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,0,&local_1a0), iVar5 == 0)) {
        FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,0,local_1a0);
        iVar3 = FUN_89ddc2e8(iVar3,&DAT_89e7d544,1,&local_1a0);
        if (iVar3 == 0) {
          FUN_89ddc7f8(iVar2,iVar4,&DAT_89e7d544,1,local_1a0);
        }
      }
      iVar3 = FUN_89ddc3c0(s__hcrtos_dsi0_89e7d5b0);
      iVar4 = FUN_89ddd288(iVar2,s__hcrtos_dsi0_89e7d5b0);
      if ((iVar3 != 0) && (iVar4 != 0)) {
        local_1a0 = (char *)0x0;
        uVar6 = FUN_89ddc3e4(iVar3,s_panel_init_sequence_89e7d5c0,&local_1a0);
        if (local_1a0 != (char *)0x0) {
          FUN_89ddd5e4(iVar2,iVar4,s_panel_init_sequence_89e7d5c0,uVar6,local_1a0);
        }
        iVar5 = FUN_89ddc2e8(iVar3,s_dtb_h_sync_level_89e7e164 + 4,0,&local_1a0);
        if (iVar5 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_dtb_h_sync_level_89e7e164 + 4,local_1a0);
        }
        iVar5 = FUN_89ddc2e8(iVar3,s_dtb_v_sync_level_89e7e178 + 4,0,&local_1a0);
        if (iVar5 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_dtb_v_sync_level_89e7e178 + 4,local_1a0);
        }
        iVar5 = FUN_89ddc2e8(iVar3,s_clock_frequency_89e7d5d4,0,&local_1a0);
        if (iVar5 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_clock_frequency_89e7d5d4,local_1a0);
        }
        iVar3 = FUN_89ddc2e8(iVar3,s_dsi_lanes_89e7d5e4,0,&local_1a0);
        if (iVar3 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_dsi_lanes_89e7d5e4,local_1a0);
        }
      }
      iVar3 = FUN_89ddc3c0(s__hcrtos_multiple_init_display__x_89e7e18c + 0x20);
      iVar4 = FUN_89ddd288(iVar2,s__hcrtos_multiple_init_display__x_89e7e18c + 0x20);
      if ((iVar3 != 0) && (iVar4 != 0)) {
        iVar5 = FUN_89ddc2e8(iVar3,s_lcd_type_89e7d5f0,0,&local_1a0);
        if (iVar5 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_lcd_type_89e7d5f0,local_1a0);
        }
        iVar3 = FUN_89ddc2e8(iVar3,s_backlight_delay_89e7d5fc,0,&local_1a0);
        if (iVar3 == 0) {
          FUN_89da93d4(iVar2,iVar4,s_backlight_delay_89e7d5fc,local_1a0);
        }
      }
    }
    uVar8 = *(uint *)(iVar2 + 4);
    FUN_89da621c(iVar2,uVar8 << 0x18 | uVar8 >> 0x18 | (uVar8 >> 8 & 0xff) << 0x10 |
                       uVar8 >> 8 & 0xff00);
  }
  iVar3 = FUN_89da9474(local_100,s_initramdisk_89e7d60c);
  if (-1 < iVar3) {
    local_1a0 = s_mtdloadraw_89e7d3d0;
    local_198 = local_100;
    local_19c = &DAT_89e7d618;
    local_194 = &DAT_89e7d618;
    FUN_89daa7e8(4,&local_1a0);
    uVar1 = _DAT_89eae380;
    uVar6 = _DAT_89eae37c;
    iVar3 = FUN_89ddd288(iVar2,s__chosen_89e7d61c);
    if (((-1 < iVar3) &&
        (iVar3 = FUN_89ddd13c(iVar2,iVar3,s_bootargs_89e7d624,&local_8c), iVar3 != 0)) &&
       (iVar4 = FUN_89db05b0(iVar3,s_rd_start__89e7d630), iVar4 != 0)) {
      FUN_89db02c8(iVar4,s_rd_start_0x_08lX_89e7d63c,uVar1);
      *(undefined1 *)(iVar4 + 0x13) = 0x20;
      iVar3 = FUN_89db05b0(iVar3,s_rd_size__89e7d650);
      if (iVar3 != 0) {
        FUN_89db02c8(iVar3,s_rd_size_0x_08lX_89e7d65c,uVar6);
        *(undefined1 *)(iVar3 + 0x12) = 0x20;
      }
    }
    uVar8 = *(uint *)(iVar2 + 4);
    FUN_89da621c(iVar2,uVar8 << 0x18 | uVar8 >> 0x18 | (uVar8 >> 8 & 0xff) << 0x10 |
                       uVar8 >> 8 & 0xff00);
  }
  iVar2 = FUN_89da9474(local_100,&DAT_89e7d66c);
  if (-1 < iVar2) {
    local_44 = s_mtdloaduImage_89e7d670;
    local_40 = local_100;
    FUN_89daaa28(2,&local_44);
    local_30 = s_bootm_89e7d680;
    FUN_89daa7ac(0,0,1,&local_30);
    if (DAT_b880006b != '\x02') {
      FUN_89db9044();
    }
  }
  iVar2 = FUN_89da9474(local_100,s_linux_89e7d688);
  if (-1 < iVar2) {
    local_3c = s_mtdloaduImage_89e7d670;
    local_38 = local_100;
    FUN_89daaa28(2,&local_3c);
    FUN_89db02c8(&local_6c,s_0x_08lx_89e7d690,_DAT_89eae384);
    local_8c = s_bootm_89e7d680;
    local_84 = &DAT_89e7d698;
    local_80 = &local_7c;
    local_88 = &local_6c;
    FUN_89daa7ac(0,0,4,&local_8c);
  }
  iVar2 = FUN_89dc1cd0(&DAT_89e7d1e4);
  if (iVar2 == 0) {
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  pcVar7 = s_module_init__all______0_89e7d69c;
  uVar6 = 0x4e9;
LAB_89daa64c:
                    /* WARNING: Subroutine does not return */
  FUN_89dddc54(s__data_home_linsen_chen_project_l_89e7d2b0,uVar6,s_app_main_89e7d778,pcVar7);
}


