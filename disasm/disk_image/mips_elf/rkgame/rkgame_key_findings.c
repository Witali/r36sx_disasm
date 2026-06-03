/* Key decompiled rkgame snippets: environment, input, core load, run_game, GetFileCore */

/* ===== lines 22074..22290 from decompiled_all.c ===== */
/* ===== FUNCTION environment @ 00431af4 ===== */

undefined4 environment(int param_1,uint *param_2)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  char *pcVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  char *pcVar7;
  undefined *puVar8;
  
  if (param_1 == 10) {
    RARCH_LOG("RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: %u\n",*param_2);
    use_rgb_8888 = 0;
    uVar3 = *param_2;
    if (uVar3 == 2) {
      video_driver_set_colormode(1);
      return 1;
    }
    if (uVar3 == 1) {
      use_rgb_8888 = 1;
      video_driver_set_colormode(3);
      return 1;
    }
    if (uVar3 == 0) {
      video_driver_set_colormode(0);
      return 1;
    }
LAB_00431b58:
    uVar5 = 0;
  }
  else {
    if (param_1 == 0x1b) {
      RARCH_LOG("RETRO_ENVIRONMENT_GET_LOG_INTERFACE\n");
      *param_2 = (uint)&LAB_00431dcc;
      return 1;
    }
    if (param_1 == 1) {
      rotation = *param_2;
      if (rotation != 0) {
        if (soft_rotation != 0) {
          if (rotation_buff != (void *)0x0) {
            free(rotation_buff);
          }
          rotation_buff = malloc(0x96000);
          return 1;
        }
        if (rotation == 2) {
          if (rotation_buff != (void *)0x0) {
            free(rotation_buff);
          }
          rotation_buff = malloc(0x96000);
          soft_rotation = 3;
          return 1;
        }
      }
      video_driver_set_rotation(rotation | 0xff00);
      return 1;
    }
    if (param_1 == 0x25) {
      param_2[8] = 0;
      param_2[9] = 0x40e58880;
      return 1;
    }
    if (param_1 == 9) {
      puVar8 = &DAT_0054d344;
      pcVar4 = stpcpy(&DAT_0054d344,work_path);
      pcVar7 = "cores";
      uVar6._0_1_ = 'c';
      uVar6._1_1_ = 'o';
      uVar6._2_1_ = 'r';
      uVar6._3_1_ = 'e';
    }
    else {
      if (param_1 != 0x1f) {
        if (param_1 == 0xf) {
          RARCH_LOG("RETRO_ENVIRONMENT_GET_VARIABLE:kay->%s\n",*param_2);
          if (corecfg == '\0') {
            return 0;
          }
          get_value_from_items(*param_2,&DAT_0054d144,&corecfg,0x40);
          if (DAT_0054d144 != '\0') {
            param_2[1] = (uint)&DAT_0054d144;
            RARCH_LOG("RETRO_ENVIRONMENT_GET_VARIABLE:value->%s\n");
            return 1;
          }
        }
        goto LAB_00431b58;
      }
      puVar8 = &DAT_0054d244;
      pcVar4 = stpcpy(&DAT_0054d244,work_path);
      pcVar7 = "saves";
      uVar6._0_1_ = 's';
      uVar6._1_1_ = 'a';
      uVar6._2_1_ = 'v';
      uVar6._3_1_ = 'e';
    }
    uVar5 = 1;
    cVar1 = pcVar7[4];
    cVar2 = pcVar7[5];
    *(undefined4 *)pcVar4 = uVar6;
    pcVar4[4] = cVar1;
    pcVar4[5] = cVar2;
    *param_2 = (uint)puVar8;
  }
  return uVar5;
}



/* ===== FUNCTION joystick_poll @ 00431e20 ===== */

/* WARNING: Heritage AFTER dead removal. Example location: r0x0052bf38 : 0x00431ea0 */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

void joystick_poll(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
  if ((joy_key & 0x40) != 0) {
    steering_data = steering_data + 1;
  }
  if ((joy_key & 0x10) != 0) {
    steering_data = steering_data + -1;
  }
  UNRECOVERED_JUMPTABLE = (code *)dlsym(DAT_0054d444,"retro_set_poplepos_steering");
  if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00431ea4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)((undefined1)steering_data);
    return;
  }
  return;
}



/* ===== FUNCTION pcsx_Thread @ 00431eb8 ===== */

void pcsx_Thread(void)

{
  pthread_t __th;
  
  pcsx_load_game(&pcsx_game_info);
  __th = pthread_self();
                    /* WARNING: Could not recover jumptable at 0x00431f00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pthread_detach(__th);
  return;
}



/* ===== FUNCTION joystick_input @ 00431f08 ===== */

bool joystick_input(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  
  if (Filetype == 1) {
    if (MaxPort < param_1) {
      MaxPort = param_1;
      RARCH_LOG("MaxPort:%d\n",param_1);
      goto LAB_00431f44;
    }
    if (3 < param_1) {
      return false;
    }
  }
  else {
LAB_00431f44:
    if (3 < param_1) {
      return false;
    }
    if (Filetype == 0x80) {
      uVar1 = (&game_joy_key)[param_1];
      goto LAB_00431f78;
    }
  }
  uVar1 = (&game_joy_key)[param_1];
  if (uVar1 == GameMenuHotKey) {
    return false;
  }
LAB_00431f78:
  return (uVar1 & (&user_joy_key_mask)[param_1 * 0x10 + param_4]) != 0;
}



/* ===== FUNCTION FUN_00432018 @ 00432018 ===== */

void FUN_00432018(int param_1)

{
  timeval local_10;
  
  fps = (int)(float)param_1;
  if (DAT_0054d448 != 0) {
    fps = fps + DAT_0054d448;
  }
  maxSkips = 3;
  gettimeofday(&local_10,(__timezone_ptr_t)0x0);
  FrameCount0 = 0;
  skipCounter = 0;
  FrameCount = 0;
  initialTicks = local_10.tv_sec * 1000 + local_10.tv_usec / 1000;
  return;
}




/* ===== lines 22940..23210 from decompiled_all.c ===== */
  undefined4 uVar2;
  
  fpsbuf = 0;
  DAT_0120a4f8 = 0;
  DAT_0120a4f4 = 0;
  DAT_0120a4f0 = 0;
  fps_ptr = 0;
  overtime = 0;
  overtime1 = 0;
  errorcount = 0;
  InitKeyMapping0fEmuType();
  RARCH_LOG("Load_Proc1 with core %s\n",param_1);
  if (*param_1 == '\0') {
    if (DAT_0054d444 == 0) {
LAB_004330b8:
      uVar2 = dlerror();
      RARCH_LOG("open %s fail,%s \n",param_1,uVar2);
      return false;
    }
  }
  else {
    DAT_0054d444 = dlopen(param_1,2);
    if (DAT_0054d444 == 0) {
      DAT_0054d444 = 0;
      goto LAB_004330b8;
    }
  }
  iVar1 = run_process("retro_set_video_refresh",DrawFrame);
  if ((((iVar1 != 0) && (iVar1 = run_process("retro_set_audio_sample_batch",PlayFrame), iVar1 != 0))
      && (iVar1 = run_process("retro_set_input_state",joystick_input), iVar1 != 0)) &&
     ((iVar1 = run_process("retro_set_environment",environment), iVar1 != 0 &&
      (iVar1 = run_process("retro_set_input_poll",joystick_poll), iVar1 != 0)))) {
    iVar1 = run_process("retro_init",0);
    return iVar1 != 0;
  }
  return false;
}



/* ===== FUNCTION Load_Proc2 @ 004330f0 ===== */

void Load_Proc2(void)

{
  uint *puVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  code *pcVar5;
  timeval local_50;
  int *local_48;
  uint *local_44;
  undefined4 *local_40;
  code *local_3c;
  undefined4 *local_38;
  undefined4 *local_34;
  undefined *local_30;
  char *local_2c;
  
  local_30 = &DAT_00550000;
  _retro_get_region = (code *)dlsym(DAT_0054d444,"retro_get_region");
  if (_retro_get_region == (code *)0x0) {
    RARCH_LOG("find retro_get_region process fail \n");
    return;
  }
  pal_ntsc = (*_retro_get_region)();
  local_34 = &_retro_run;
  local_40 = &_SetFrameSkip;
  RARCH_LOG("pal_ntsc:%d\n",pal_ntsc);
  uVar2 = DAT_0054d444;
  _retro_run = dlsym(DAT_0054d444,"retro_run");
  _SetFrameSkip = dlsym(uVar2,"SetFrameSkip");
  if (_SetFrameSkip != 0) {
    RARCH_LOG("find _SetFrameSkip process\n");
  }
  if (pal_ntsc == 0) {
    sound_len = 0xb7c;
  }
  else {
    sound_len = 0xdc8;
  }
  RetroInitSound();
  local_38 = &diff_prev;
  gettimeofday((timeval *)&outTimeVal,(__timezone_ptr_t)0x0);
  uVar2 = 0x3c;
  diff_prev = 0;
  if (pal_ntsc != 0) {
    uVar2 = 0x32;
  }
  inTimeVal = outTimeVal;
  local_3c = FUN_00432018;
  DAT_0052f870 = DAT_0052f808;
  FUN_00432018(uVar2);
  local_2c = "saves";
  do {
    local_48 = &initialTicks;
    local_44 = &fps;
    do {
      if (joy_key == 9) {
LAB_00433474:
        joy_key = 0;
        iVar3 = PauseMenu();
        if (iVar3 != 0) {
LAB_0043352c:
          run_process("retro_unload_game",0);
          run_process("retro_deinit",0);
          dlclose(*(undefined4 *)(local_30 + -0x2bbc));
          return;
        }
        gettimeofday((timeval *)&outTimeVal,(__timezone_ptr_t)0x0);
        uVar2 = 0x3c;
        DAT_0052f870 = DAT_0052f808;
        inTimeVal = outTimeVal;
        if (pal_ntsc != 0) {
          uVar2 = 0x32;
        }
        *local_38 = 0;
        (*local_3c)(uVar2);
      }
      else {
        if (joy_key < 0) {
          if (DAT_011f0b48 != 9) goto LAB_0043352c;
          goto LAB_00433474;
        }
        if (DAT_011f0b48 == 9) goto LAB_00433474;
        if (DAT_011f0b48 < 0) goto LAB_0043352c;
      }
      gettimeofday(&local_50,(__timezone_ptr_t)0x0);
      uVar4 = *local_44;
      FrameCount0 = FrameCount0 + 1;
      iVar3 = local_50.tv_sec * 1000 + local_50.tv_usec / 1000;
      if (FrameCount0 < ((iVar3 - *local_48) * uVar4) / 1000) {
        if (skipCounter < maxSkips) {
          skipCounter = skipCounter + 1;
          FrameSkip = (uint)(0 < skipCounter);
          uVar4 = (uint)(skipCounter < 1);
        }
        else {
          gettimeofday(&local_50,(__timezone_ptr_t)0x0);
          uVar4 = 1;
          FrameSkip = 0;
          FrameCount0 = 0;
          skipCounter = 0;
          *local_48 = local_50.tv_sec * 1000 + local_50.tv_usec / 1000;
        }
      }
      else {
        if (uVar4 == 0) {
          trap(7);
        }
        iVar3 = ((FrameCount0 * 1000 - 1000) / uVar4 + *local_48) - iVar3;
        skipCounter = 0;
        if (iVar3 < 1) {
          uVar4 = 1;
          FrameSkip = 0;
        }
        else {
          usleep(iVar3 * 1000);
          FrameSkip = (uint)(0 < skipCounter);
          uVar4 = (uint)(skipCounter < 1);
        }
      }
      pcVar5 = (code *)*local_40;
      iVar3 = fps_ptr + 1;
      if (iVar3 == 0x10) {
        iVar3 = 0;
      }
      puVar1 = &fpsbuf + fps_ptr;
      fps_ptr = iVar3;
      *puVar1 = uVar4;
      if (pcVar5 != (code *)0x0) {
        (*pcVar5)();
      }
      ReadJoystickProc();
      TurboKeyProcess();
      if (joy_key != 9) {
        (*(code *)*local_34)();
      }
    } while (LoadSkipState == 0);
    LoadDefaultState(local_2c);
    LoadSkipState = 0;
    gettimeofday((timeval *)&outTimeVal,(__timezone_ptr_t)0x0);
    uVar2 = 0x3c;
    DAT_0052f870 = DAT_0052f808;
    inTimeVal = outTimeVal;
    if (pal_ntsc != 0) {
      uVar2 = 0x32;
    }
    *local_38 = 0;
    (*local_3c)(uVar2);
  } while( true );
}



/* ===== FUNCTION FUN_00433630 @ 00433630 ===== */

undefined4 FUN_00433630(char *param_1,int param_2)

{
  int iVar1;
  code *pcVar2;
  long lVar3;
  void *__ptr;
  uint __n;
  char acStack_110 [256];
  
  n_input_width = 0x140;
  n_input_height = 0xe0;
  n_input_visible_width = 0x100;
  n_input_visible_height = 0xe0;
  screen_w = 0x100;
  screen_x = 0;
  sprintf(acStack_110,"%s/cores/libemu_sfc.so",work_path);
  iVar1 = Load_Proc1(acStack_110);
  if (iVar1 != 0) {
    strcpy(fileName,param_1);
    __n = ZIP_BUF_SIZE;
    __ptr = ZIP_BUF;
    if (param_2 < 0x10000) {
      RARCH_LOG("\nLoading %s ... \r\n",param_1);
      romfile = fopen(param_1,"rb");
      if (romfile == (FILE *)0x0) {
        RARCH_LOG("%s open fail\r\n",param_1);
        return 0xffffffff;
      }
      fseek(romfile,0,2);
      lVar3 = ftell(romfile);
      __n = lVar3 + 3U & 0xfffffffc;
      fseek(romfile,0,0);
      __ptr = malloc(__n + 4);
      if (__ptr == (void *)0x0) {
        fclose(romfile);
        return 1;
      }
      fread(__ptr,1,__n,romfile);
      fclose(romfile);
    }
    game = fileName;
    DAT_01206500 = __ptr;
    DAT_01206504 = __n;
    pcVar2 = (code *)dlsym(DAT_0054d444,"retro_load_game");
    if (pcVar2 == (code *)0x0) {
      RARCH_LOG("find retro_load_game process fail \n");
      return 0;
    }
    iVar1 = (*pcVar2)(&game);
    if (iVar1 != 0) {
      Load_Proc2();
    }
    if (__ptr != (void *)0x0) {
      free(__ptr);
      return 0;
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00433ff0 @ 00433ff0 ===== */

void * FUN_00433ff0(size_t param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  

/* ===== lines 23519..23665 from decompiled_all.c ===== */
/* ===== FUNCTION Core_Load @ 00434824 ===== */

undefined4 Core_Load(char *param_1,undefined4 param_2)

{
  int iVar1;
  code *pcVar2;
  undefined4 uVar3;
  long lVar4;
  char acStack_180 [256];
  char local_80 [108];
  
  n_input_width = 0x140;
  n_input_height = 0xe0;
  n_input_visible_width = 0x100;
  n_input_visible_height = 0xe0;
  screen_w = 0x100;
  MaxPort = 0;
  screen_x = 0;
  RARCH_LOG("with cores:%s\n");
  sprintf(acStack_180,"%s/cores/%s.cfg",work_path,param_2);
  memset(&corecfg,0,16000);
  get_items_from_file(acStack_180,&corecfg);
  sprintf(acStack_180,"%scores/%s",work_path,param_2);
  DAT_0054d444 = dlopen(acStack_180,2);
  if (DAT_0054d444 == 0) {
    uVar3 = dlerror();
    RARCH_LOG("open %s fail,%s \n",acStack_180,uVar3);
    return 0;
  }
  _retro_is_support = (code *)dlsym(DAT_0054d444,"retro_is_support");
  if (_retro_is_support != (code *)0x0) {
    iVar1 = (*_retro_is_support)(param_1);
    if (iVar1 < 0) {
      RARCH_LOG("unsupport this game rom\n");
      dlclose(DAT_0054d444);
      return 0;
    }
    RARCH_LOG("support this game rom\n");
  }
  iVar1 = Load_Proc1("");
  if (iVar1 == 0) {
    return 0;
  }
  run_process("retro_set_progress_callback",progress);
  strcpy(fileName,param_1);
  if (Filetype == 0x400) {
    _retro_set_device = (code *)dlsym(DAT_0054d444,"retro_set_controller_port_device");
    if (_retro_set_device == (code *)0x0) {
      RARCH_LOG("find retro_set_controller_port_device process fail \n");
    }
    else {
      get_value_from_items("device0_type",local_80,items,0x40);
      pcVar2 = _retro_set_device;
      if (local_80[0] != '\0') {
        lVar4 = strtol(local_80,(char **)0x0,10);
        (*pcVar2)(0,lVar4);
      }
      get_value_from_items("device1_type",local_80,items,0x40);
      pcVar2 = _retro_set_device;
      if (local_80[0] != '\0') {
        lVar4 = strtol(local_80,(char **)0x0,10);
        (*pcVar2)(1,lVar4);
      }
    }
  }
  DAT_01206500 = ZIP_BUF;
  game = fileName;
  DAT_01206504 = ZIP_BUF_SIZE;
  pcVar2 = (code *)dlsym(DAT_0054d444,"retro_load_game");
  if (pcVar2 == (code *)0x0) {
    RARCH_LOG("find retro_load_game process fail \n");
  }
  else {
    iVar1 = (*pcVar2)(&game);
    RARCH_LOG("retcode:%d\n",iVar1);
    if (iVar1 == 0) {
      run_process("retro_unload_game",0);
      run_process("retro_deinit",0);
      dlclose(DAT_0054d444);
    }
    else {
      Load_Proc2();
      video_driver_set_rotation(0xff00);
    }
  }
  video_driver_set_colormode(1);
  if (soft_rotation != 0) {
    if (rotation_buff != 0) {
      free((void *)rotation_buff);
      rotation_buff = 0;
    }
    if (soft_rotation == 3) {
      soft_rotation = 0;
    }
  }
  RARCH_LOG(&DAT_004f3254);
  return 0;
}



/* ===== FUNCTION Gpsp_Load @ 00434ce0 ===== */

undefined4 Gpsp_Load(char *param_1,int param_2)

{
  int iVar1;
  code *pcVar2;
  char acStack_118 [260];
  
  n_input_width = 0xf0;
  n_input_height = 0xa0;
  n_input_visible_width = 0xf0;
  n_input_visible_height = 0xa0;
  screen_w = 0xf0;
  screen_x = 0;
  sprintf(acStack_118,"%s/cores/libemu_gpsp.so",work_path);
  DAT_0054d448 = 0xffffffff;
  iVar1 = Load_Proc1(acStack_118);
  if (iVar1 != 0) {
    strcpy(fileName,param_1);
    if (param_2 < 0x10000) {
      run_process("retro_set_unzip",0xffffffff);
    }
    else {
      run_process("retro_set_unzip",gpsp_unzip);
    }
    DAT_01206500 = 0;
    DAT_01206504 = 0;
    game = fileName;
    pcVar2 = (code *)dlsym(DAT_0054d444,"retro_load_game");
    if (pcVar2 == (code *)0x0) {
      RARCH_LOG("find retro_load_game process fail \n");
    }
    else {
      iVar1 = (*pcVar2)(&game);
      if (iVar1 != 0) {
        Load_Proc2();
      }
    }
    DAT_0054d448 = 0;
    return 0;
  }
  return 0;
}


/* ===== lines 23668..23845 from decompiled_all.c ===== */
/* ===== FUNCTION run_game @ 00434e94 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 run_game(char *param_1)

{
  undefined4 ******ppppppuVar1;
  char *pcVar2;
  int iVar3;
  FILE *__stream;
  size_t sVar4;
  int iVar5;
  int iVar6;
  code *pcVar7;
  uint uVar8;
  undefined4 ******local_168;
  char acStack_164 [4];
  char acStack_160 [288];
  uint local_40;
  char local_38;
  char local_37;
  char local_36;
  char local_35;
  undefined4 ******local_30;
  
  MaxPort = 0;
  ZIP_BUF_SIZE = 0;
  ZIP_BUF = (void *)0x0;
  pcVar2 = (char *)GetFilenameExt();
  strcpy(&local_38,pcVar2);
  strupr(&local_38);
  RARCH_LOG("run_game %s,%s\n",param_1,&local_38);
  _Filetype = 0;
  skipframe_count = 0;
  if ((((local_38 == 'F') && (local_37 == 'B')) && (local_36 == 'A')) && (local_35 == '\0')) {
    _Filetype = 1;
    RARCH_LOG("Load FBA game: %s ...\r\n",param_1);
    FBA_Load(param_1,_Filetype);
    MaxPort = 0;
    RARCH_LOG("Exit FBA back to here...\r\n");
    return 1;
  }
  iVar3 = GetCoreIndex(&local_38);
  uVar8 = _Filetype;
  if (_Filetype < 0x10000) {
    if (_Filetype == 0x10) {
      uVar8 = 0x10;
LAB_00435440:
      skipframe_count = 0x14;
      Gpsp_Load(param_1,uVar8);
      MaxPort = 0;
      return 1;
    }
    if (_Filetype == 0x80) {
      pcVar7 = PCSX_Load;
      uVar8 = 0x80;
      goto LAB_00435048;
    }
    if (_Filetype == 0x400) {
      extract_basepath(&local_168,param_1,100);
      sVar4 = strlen((char *)&local_168);
      builtin_strncpy((char *)((int)&local_168 + sVar4),"/gam",4);
      builtin_strncpy(acStack_164 + sVar4,"e.cf",4);
      acStack_160[sVar4] = 'g';
      acStack_160[sVar4 + 1] = '\0';
      get_items_from_file(&local_168,items);
      Core_Load(param_1,&DAT_0052c064 + iVar3 * 0x44);
      MaxPort = 0;
      return 1;
    }
    __stream = fopen(param_1,"rb");
    if (__stream == (FILE *)0x0) {
      RARCH_LOG("%s open fail\r\n",param_1);
      MaxPort = 0;
      return 0xffffffff;
    }
    fseek(__stream,0,2);
    ZIP_BUF_SIZE = ftell(__stream);
    if ((int)ZIP_BUF_SIZE < 0x8000000) {
      fseek(__stream,0,0);
      uVar8 = ZIP_BUF_SIZE + 3 & 0xfffffffc;
      ZIP_BUF_SIZE = uVar8;
      ZIP_BUF = malloc(uVar8 + 4);
      if (ZIP_BUF == (void *)0x0) {
        fclose(__stream);
        uVar8 = ZIP_BUF_SIZE;
      }
      fread(ZIP_BUF,1,uVar8,__stream);
    }
    else {
      ZIP_BUF_SIZE = 0;
      ZIP_BUF = (void *)0x0;
    }
    fclose(__stream);
  }
  else {
    pcVar2 = strstr(param_1,"/MAME/");
    if (pcVar2 != (char *)0x0) {
      pcVar7 = FBA_Load;
      uVar8 = uVar8 | 1;
      _Filetype = uVar8;
LAB_00435048:
      (*pcVar7)(param_1,uVar8);
      MaxPort = 0;
      return 1;
    }
    local_30 = &local_168;
    memset(local_30,0,0x130);
    iVar5 = OpenZipU(param_1,0,2);
    if (iVar5 != 0) {
      GetZipItemA(iVar5,0xffffffff,local_30);
      ppppppuVar1 = local_30;
      local_30 = local_168;
      iVar6 = GetZipItemA(iVar5,0,ppppppuVar1);
      if (iVar6 == 0) {
        strupr(acStack_164);
        pcVar2 = (char *)GetFilenameExt(acStack_164);
        strcpy(&local_38,pcVar2);
        if (1 < (int)local_30) {
          _Filetype = _Filetype | 1;
          CloseZipU(iVar5);
          FBA_Load(param_1,_Filetype);
          MaxPort = 0;
          return 1;
        }
        iVar3 = GetCoreIndex(&local_38);
        if (Filetype == 0x10) {
          CloseZipU(iVar5);
          uVar8 = _Filetype;
          goto LAB_00435440;
        }
        ZIP_BUF_SIZE = local_40;
        ZIP_BUF = malloc(local_40 + 0x10);
        if (ZIP_BUF == (void *)0x0) {
          CloseZipU(iVar5);
          MaxPort = 0;
          return 0;
        }
        iVar6 = UnzipItem(iVar5,0,ZIP_BUF,0,3);
        if (iVar6 == 0) {
          RARCH_LOG("UnzipItem to ZIP_BUF ZIP_BUF_SIZE:%x\n",ZIP_BUF_SIZE);
          CloseZipU(iVar5);
        }
      }
    }
  }
  if (Filetype == 0x4000) {
    DAT_0054d448 = 0xf;
LAB_004351f8:
    if (Filetype == 0x2000) {
      skipframe_count = 0x14;
    }
LAB_0043513c:
    if (Filetype == 0) goto LAB_00435144;
  }
  else {
    if (Filetype == -0x8000) {
      DAT_0054d448 = 0xffffffff;
      goto LAB_004351f8;
    }
    if (Filetype == 0x40) {
      skipframe_count = 0x3c;
      goto LAB_0043513c;
    }
    if (Filetype != 0x20) {
      if (Filetype != 2) goto LAB_004351f8;
      skipframe_count = 0x14;
      goto LAB_0043513c;
    }
    skipframe_count = 0x1e;
  }
  Core_Load(param_1,&DAT_0052c064 + iVar3 * 0x44);
LAB_00435144:
  DAT_0054d448 = 0;
  video_driver_set_rotation(0xff00);
  if (ZIP_BUF != (void *)0x0) {
    free(ZIP_BUF);

/* ===== lines 14229..14320 from decompiled_all.c ===== */
/* ===== FUNCTION GetFileCore @ 00421d78 ===== */

undefined4 GetFileCore(char *param_1)

{
  int iVar1;
  char *pcVar2;
  undefined4 uVar3;
  
  RARCH_LOG("GetFileCore:%s\n",param_1);
  iVar1 = DAT_0054bf00;
  if (DAT_0054bf00 == 0) {
    RARCH_LOG("0002\n");
  }
  else {
    while (iVar1 = mxmlFindElement(iVar1,DAT_0054bf00,&DAT_004f1abc,0,0,1), iVar1 != 0) {
      pcVar2 = (char *)mxmlElementGetAttr(iVar1,"name");
      pcVar2 = strstr(param_1,pcVar2);
      if (pcVar2 != (char *)0x0) {
        uVar3 = mxmlElementGetAttr(iVar1,&DAT_004f22a0);
        return uVar3;
      }
    }
  }
  return 0;
}



/* ===== FUNCTION FUN_00421eb4 @ 00421eb4 ===== */

undefined4 FUN_00421eb4(void)

{
  return DAT_0052b4bc;
}



/* ===== FUNCTION SoundClose @ 00421ecc ===== */

void SoundClose(int param_1)

{
  int *__ptr;
  
  if ((&DAT_012022a4)[param_1 * 9] != 1) {
    (&SoundPlayer)[param_1 * 9] = 0;
    return;
  }
  __ptr = (int *)(&SoundPlayer)[param_1 * 9];
  if (*__ptr != 0) {
    MP3FreeDecoder();
  }
  if ((void *)__ptr[3] != (void *)0x0) {
    free((void *)__ptr[3]);
  }
  free(__ptr);
  (&SoundPlayer)[param_1 * 9] = 0;
  return;
}



/* ===== FUNCTION FUN_00421fb0 @ 00421fb0 ===== */

void FUN_00421fb0(void)

{
  Soundplayflag = Soundplayflag & 0xfe;
  while (Soundplayflag != 0) {
    usleep(1000);
  }
  SoundClose(0);
  DAT_0054bf08 = DAT_0054bf08 & 0xfe;
  SoundPlayer = 0;
  DAT_012022c4 = 0;
  while (DAT_0054bf08 != 0) {
    usleep(1000);
  }
  return;
}



/* ===== FUNCTION SoundPlay @ 00422050 ===== */

void SoundPlay(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;

/* ===== lines 22232..22290 from decompiled_all.c ===== */
/* ===== FUNCTION joystick_input @ 00431f08 ===== */

bool joystick_input(uint param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  uint uVar1;
  
  if (Filetype == 1) {
    if (MaxPort < param_1) {
      MaxPort = param_1;
      RARCH_LOG("MaxPort:%d\n",param_1);
      goto LAB_00431f44;
    }
    if (3 < param_1) {
      return false;
    }
  }
  else {
LAB_00431f44:
    if (3 < param_1) {
      return false;
    }
    if (Filetype == 0x80) {
      uVar1 = (&game_joy_key)[param_1];
      goto LAB_00431f78;
    }
  }
  uVar1 = (&game_joy_key)[param_1];
  if (uVar1 == GameMenuHotKey) {
    return false;
  }
LAB_00431f78:
  return (uVar1 & (&user_joy_key_mask)[param_1 * 0x10 + param_4]) != 0;
}



/* ===== FUNCTION FUN_00432018 @ 00432018 ===== */

void FUN_00432018(int param_1)

{
  timeval local_10;
  
  fps = (int)(float)param_1;
  if (DAT_0054d448 != 0) {
    fps = fps + DAT_0054d448;
  }
  maxSkips = 3;
  gettimeofday(&local_10,(__timezone_ptr_t)0x0);
  FrameCount0 = 0;
  skipCounter = 0;
  FrameCount = 0;
  initialTicks = local_10.tv_sec * 1000 + local_10.tv_usec / 1000;
  return;
}



