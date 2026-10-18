
/* ===== FORCED FUNCTION FUN_89dad8b0 @ 89dad8b0 ===== */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_89dad8b0(undefined4 param_1,undefined4 param_2,int param_3,undefined4 *param_4,uint param_5,
                int param_6)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  code *pcVar4;
  undefined4 uVar5;
  char *pcVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint local_40 [3];
  uint local_34;
  int local_30;

  *(uint *)(param_6 + 0x88) = *(uint *)(param_6 + 0x88) | param_5;
  if ((param_5 & 1) != 0) {
    iVar2 = thunk_FUN_89da6ab8(&DAT_89ec9dc4,0,0x84);
    *(undefined4 *)(iVar2 + 0x84) = 1;
    *(undefined4 *)(iVar2 + 0x88) = 1;
  }
  if ((param_5 & 2) != 0) {
    local_40[1] = 0;
    local_40[0] = 0;
    uVar5 = 0;
    if (0 < param_3) {
      uVar5 = *param_4;
    }
    puVar3 = (undefined4 *)FUN_89dae260(uVar5,local_40 + 1,local_40);
    _DAT_89ec9e18 = 0;
    _DAT_89ec9e14 = (undefined4 *)0x0;
    iVar2 = FUN_89dae2c0(puVar3);
    if (iVar2 == 1) {
      FUN_89dafc90(s____Booting_kernel_from_Legacy_Im_89e7e378,puVar3);
      local_40[2] = _DAT_89ec9e48;
      iVar2 = FUN_89dad6a8(*puVar3);
      if (iVar2 == 0x27051956) {
        iVar2 = FUN_89dadec0(puVar3);
        if (iVar2 == 0) {
          pcVar6 = s_Bad_Header_Checksum_89e7e3c0;
          goto LAB_89dad9b4;
        }
        FUN_89dae170(puVar3);
        if (local_40[2] != 0) {
          FUN_89dafd90(s_Verifying_Checksum_____89e7e3d8);
          iVar2 = FUN_89dadf18(puVar3);
          if (iVar2 == 0) {
            pcVar6 = s_Bad_Data_CRC_89e7e3f4;
            goto LAB_89dada14;
          }
          FUN_89dafd90(&DAT_89e7e404);
        }
        if (*(char *)((int)puVar3 + 0x1d) == '\x05') {
          cVar1 = *(char *)((int)puVar3 + 0x1e);
          if (((cVar1 != '\x02') && (cVar1 != '\x0e')) && (cVar1 != '\x01')) {
            pcVar6 = s_Wrong_Image_Type_89e7e428;
            goto LAB_89dada14;
          }
          _DAT_89ec9e14 = puVar3 + 0x10;
          _DAT_89ec9e18 = FUN_89dad6a8(puVar3[3]);
          thunk_FUN_89e743cc(0x89ec9dc8,puVar3,0x40);
          _DAT_89ec9e08 = 1;
          _DAT_89ec9dc4 = puVar3;
          FUN_89dafc90(s_kernel_data_at_0x_08lx__len___0x_89e7e43c,_DAT_89ec9e14,_DAT_89ec9e18);
          goto LAB_89dadabc;
        }
        FUN_89dafc90(s_Unsupported_Architecture_0x_x_89e7e408);
      }
      else {
        pcVar6 = s_Bad_Magic_Number_89e7e3ac;
LAB_89dad9b4:
        FUN_89dafd90(pcVar6);
      }
      puVar3 = (undefined4 *)0x0;
    }
    else {
      pcVar6 = s_Wrong_Image_Format_89e7e46c;
LAB_89dada14:
      puVar3 = (undefined4 *)0x0;
      FUN_89dafc90(pcVar6);
    }
LAB_89dadabc:
    if (_DAT_89ec9e18 == 0) {
      pcVar6 = s_ERROR__can_t_get_kernel_image__89e7e480;
LAB_89dadac8:
      FUN_89dafc90(pcVar6);
      return 1;
    }
    iVar2 = FUN_89dae2c0(puVar3);
    if (iVar2 != 1) {
      pcVar6 = s_ERROR__unknown_image_format_type_89e7e4a0;
      goto LAB_89dadac8;
    }
    DAT_89ec9e21 = *(byte *)((int)puVar3 + 0x1e);
    uVar7 = (uint)DAT_89ec9e21;
    DAT_89ec9e20 = *(undefined1 *)((int)puVar3 + 0x1f);
    DAT_89ec9e22 = *(char *)(puVar3 + 7);
    iVar2 = FUN_89dad6a8(puVar3[3]);
    _DAT_89ec9e10 = (int)puVar3 + iVar2 + 0x40;
    _DAT_89ec9e1c = (undefined4 *)FUN_89dad6a8(puVar3[4]);
    DAT_89ec9e23 = *(undefined1 *)((int)puVar3 + 0x1d);
    if (_DAT_89ec9e08 == 0) {
      pcVar6 = s_Could_not_find_kernel_entry_poin_89e7e4c4;
      iVar2 = 1;
      goto LAB_89dadb80;
    }
    _DAT_89ec9e24 = FUN_89dad6a8(_DAT_89ec9ddc);
    _DAT_89ec9e0c = puVar3;
    if (uVar7 == 0xe) {
      _DAT_89ec9e24 = _DAT_89ec9e24 + (int)_DAT_89ec9e14;
      _DAT_89ec9e1c = _DAT_89ec9e14;
    }
  }
  if (((((param_5 & 4) != 0) && (((DAT_89ec9e21 - 2 & 0xfd) == 0 || (DAT_89ec9e21 == 0xe)))) &&
      ((DAT_89ec9e22 == '\x05' || (DAT_89ec9e22 == '\x0e')))) &&
     (iVar2 = FUN_89dad700(param_2,param_3,param_4,0,0), iVar2 != 0)) {
    return iVar2;
  }
  if ((param_5 & 8) != 0) {
    local_40[2] = *(uint *)(param_6 + 0x48);
    uVar8 = *(uint *)(param_6 + 0x50);
    local_34 = *(uint *)(param_6 + 0x4c);
    uVar9 = *(uint *)(param_6 + 0x58);
    cVar1 = *(char *)(param_6 + 0x5c);
    local_30 = FUN_89dae048(cVar1,uVar9,uVar8,*(undefined1 *)(param_6 + 0x5d),uVar9,uVar8,
                            *(undefined4 *)(param_6 + 0x54),0x4000000,local_40);
    uVar7 = local_40[0];
    if (local_30 != 0) {
      uVar5 = FUN_89dae158(cVar1);
      if (local_30 == -0x58) {
        return -3;
      }
      if (uVar7 - uVar9 < 0x4000000) {
        FUN_89dafc90(s__s__uncompress_error__d_89e7e524,uVar5);
      }
      else {
        FUN_89dafc90(s_Image_too_large__0x_08lx___incre_89e7e4e8);
      }
      FUN_89dafc90(s_Must_RESET_board_to_recover_89e7e540);
      goto LAB_89dadc70;
    }
    *(uint *)(param_6 + 0x54) = local_40[0] - uVar9;
    FUN_89da621c(uVar9 & 0xffffffe0,(local_40[0] + 0x1f & 0xffffffe0) - (uVar9 & 0xffffffe0));
    FUN_89dafc90(s_kernel_loaded_at_0x_08lx__end___0_89e7e560,uVar9,local_40[0]);
    if (((cVar1 != '\0') || (uVar8 != uVar9)) && ((uVar9 < local_34 && (local_40[2] < local_40[0])))
       ) {
      FUN_89dafc90(s_images_os_start___0x_lX__images__89e7e58c,local_40[2],local_34);
      FUN_89dafc90(s_images_os_load___0x_lx__load_end_89e7e5bc,uVar9,local_40[0]);
      if (*(int *)(param_6 + 0x44) == 0) {
        FUN_89dafd90(s_ERROR__new_format_image_overwrit_89e7e624);
        goto LAB_89dadc70;
      }
      if (*(char *)(param_6 + 0x22) == '\x04') {
        FUN_89dafd90(s_WARNING__legacy_format_multi_com_89e7e5e8);
      }
    }
  }
  pcVar4 = (code *)FUN_89dae740(*(undefined1 *)(param_6 + 0x5e));
  if ((pcVar4 == (code *)0x0) && ((param_5 & 0x7c0) != 0)) {
    uVar5 = FUN_89dae000(*(undefined1 *)(param_6 + 0x5e));
    FUN_89dafc90(s_ERROR__booting_os___s____d__is_n_89e7e66c,uVar5,*(undefined1 *)(param_6 + 0x5e));
    return 1;
  }
  if (((((param_5 & 0x40) != 0) && (iVar2 = (*pcVar4)(0x40,param_3,param_4,param_6), iVar2 != 0)) ||
      (((param_5 & 0x80) != 0 && (iVar2 = (*pcVar4)(0x80,param_3,param_4,param_6), iVar2 != 0)))) ||
     (((param_5 & 0x100) != 0 && (iVar2 = (*pcVar4)(0x100,param_3,param_4,param_6), iVar2 != 0)))) {
    pcVar6 = s_subcommand_not_supported_89e7e69c;
LAB_89dadb80:
    FUN_89dafc90(pcVar6);
    return iVar2;
  }
  if ((param_5 & 0x400) == 0) {
    return 0;
  }
  iVar2 = FUN_89dae6ac(param_3,param_4,0x400,param_6,pcVar4);
  if (iVar2 == -3) {
    return -3;
  }
  if (iVar2 != -1) {
    return iVar2;
  }
LAB_89dadc70:
  FUN_89db9044();
  return -1;
}



/* ===== FORCED FUNCTION FUN_89dae5ec @ 89dae5ec ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_89dae5ec(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  code *pcVar1;

  if ((param_1 & 0x80) == 0) {
    if ((param_1 & 0x140) != 0) {
      return 0;
    }
    if ((param_1 & 0x600) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(param_4 + 0x60);
    FUN_89dafc90(s____Transferring_control_to_Linux_89e7ea5c,pcVar1);
    if (*(int *)(param_4 + 0x70) == 0) {
      return 0;
    }
    if (_DAT_89ec7b24 == 0) {
      FUN_89dbe0a0();
    }
    FUN_89dc104c();
    FUN_89dbab44();
    (*pcVar1)(0xfffffffe,*(undefined4 *)(param_4 + 0x6c),0,0);
  }
  return 0xffffffff;
}



/* ===== FORCED FUNCTION FUN_89dadec0 @ 89dadec0 ===== */

bool FUN_89dadec0(int param_1)

{
  int iVar1;
  int iVar2;
  undefined1 auStack_48 [4];
  undefined4 local_44;

  thunk_FUN_89da6748(auStack_48,param_1,0x40);
  local_44 = 0;
  iVar1 = FUN_89ddd6bc(0,auStack_48,0x40);
  iVar2 = FUN_89dad6d4(*(undefined4 *)(param_1 + 4),iVar1);
  return iVar2 == iVar1;
}



/* ===== FORCED FUNCTION FUN_89dadf18 @ 89dadf18 ===== */

bool FUN_89dadf18(int param_1)

{
  int iVar1;
  int iVar2;

  FUN_89dad6d4(*(undefined4 *)(param_1 + 0xc),param_1 + 0x40);
  iVar1 = thunk_FUN_89ddd6bc(0);
  iVar2 = FUN_89dad6d4(*(undefined4 *)(param_1 + 0x18),iVar1);
  return iVar2 == iVar1;
}
